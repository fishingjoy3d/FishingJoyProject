#include "stdafx.h"
#include "HttpServer.h"
static const char *WEEK_STR[]		= { "Sun" ,"Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *MONTH_STR[]		= { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static const char *RESPONSE_TXT		= "HTTP/1.1 200 OK\r\nServer: vata/0.0.1\r\nDate: %s\r\nTransfer-Encoding: chunked\r\nConnection: keep-alive\r\n\r\n%s0\r\n\r\n";
static const char *SUCCESS_TXT		= "7\r\nsuccess\r\n";
static const char *FAIL_TXT			= "4\r\nfail\r\n";
static const char *GET_STR			= "GET /";
static const char *POST_STR			= "POST /";

static const int  SUCCESS_COUNT		= strlen(SUCCESS_TXT);
static const int  FAIL_COUNT		= strlen(FAIL_TXT);
static const int  GET_COUNT			= strlen(GET_STR);
static const int  POST_COUNT		= strlen(POST_STR);
#define HTTP_STR_COUNT 1024


static UINT WINAPI ThreadRecv(void *p)
{
	((HttpServer*)p)->_RecvThread();
	return 0;
}
static UINT WINAPI ThreadAccept(void *p)
{
	((HttpServer*)p)->_AcceptThread();
	return 0;
}

void InitSocket(SOCKET s)
{
	const int recvBuffSize = 1024;
	const int sendBuffSize = 1024;
	unsigned long arg = 1;
	setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const char*)&recvBuffSize, sizeof(uint));
	setsockopt(s, SOL_SOCKET, SO_SNDBUF, (const char*)&sendBuffSize, sizeof(uint));
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(arg));
	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&arg, sizeof(arg));
	ioctlsocket(s, FIONBIO, &arg);
}
bool HttpServer::Init(const HttpServerInitData &hsi)
{
	m_pThreadRecvList = NULL;
	m_Socket = NULL;
	m_bRun = false;
	m_ThreadNum = 0;
	m_ExitNum = 0;
	m_TotalRecvDataCount = 0;
	m_InitData = hsi;
	SOCKET s = INVALID_SOCKET;
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
		return false;

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(hsi.Port);
	
	InitSocket(s);

	int addrSize = sizeof(addr);
	int ret = ::bind(s, (SOCKADDR*)&addr, sizeof(addr));
	if (ret != 0)
	{
		closesocket(s);
		Log("Bind Failed:%d", WSAGetLastError());
		return false;
	}
	ret = ::listen(s, 128);
	if (ret != 0)
	{
		closesocket(s);
		Log("Listen Failed:%d", WSAGetLastError());
		return false;
	}
	m_pThreadRecvList = new ThreadRecvData[hsi.ThreadNum];

	m_bRun = true;
	m_Socket = s;
	
	for (UINT i = 0; i < hsi.ThreadNum; ++i)
		::_beginthreadex(0, 0, ThreadRecv, this, 0, 0);
	::_beginthreadex(0, 0, ThreadAccept, this, 0, 0);
	return true;
}
//返回空格结束的长度
#define TRANS_ENCODING	"Transfer-Encoding: chunked"
#define RESPONSE_END	"0\r\n\r\n"
#define CONTENT_LINE	"\r\n\r\n"
#define CONTENT_LENGTH  "Content-Length: "
static const int CONTENT_LENGTH_COUNT = strlen(CONTENT_LENGTH);
#define END_ERR 0xffffffff
UINT CheckContentPostIsEnd(HttpClientData *pc)
{
	const char *pstr = pc->Buff;
	const char *pend = pc->Buff + pc->RecvSize;
	const char *pfind;
CHECK_LOOP:
	if (pc->Content_Length != -1)
	{
		const BYTE *pstart = (BYTE*)(pc->Buff + pc->Content_Start);
		int ret = int(pc->RecvSize - pc->Content_Start - pc->Content_Length);
		if (ret >= 0)
		{
			int contentlen = pc->Content_Length;
			if (pc->Content_Length > 0)
			{
				if (pc->Content_Length >= 3 && *pstart == 0xef)
				{
					//BOM 三个字节 0xef,0xbb,0xbf
					pc->Content_Length -= 3;
					pstart += 3;
				}
				memmove_s(pc->Buff, sizeof(pc->Buff), pstart, pc->Content_Length);
				pc->RecvSize = pc->Content_Length;
				pc->Buff[pc->RecvSize] = 0;
				return pc->RecvSize;
			}
			else
			{
				pc->Buff[0] = 0;
				pc->RecvSize = 0;
				return pc->RecvSize;
			}
		}
	}
	else if (strstr(pstr, TRANS_ENCODING) != NULL)
	{
		if (strstr(pstr, RESPONSE_END) != NULL)
		{
			//chunked 已经接收完成
			//找到内容的开始
			const char *pstart = strstr(pstr, CONTENT_LINE);
			if (pstart == NULL)
				return END_ERR;
			pstart += 4;
			if (pstart >= pend)
				return END_ERR;

			int idx = 0;
			while (pstart < pend)
			{
				UINT size = strtol(pstart, NULL, 16);
				if (size == 0)
				{
					pc->Buff[idx] = 0;
					pc->RecvSize = idx;
					return pc->RecvSize;
				}
				pstart = strstr(pstart, "\r\n");
				if (pstart == NULL || pstart >= pend)
					break;
				pstart += 2;
				if (pstart >= pend)
					break;
				UINT strSize = pend - pstart;
				if (strSize < size || size > sizeof(pc->Buff) - idx)
					break;
				memmove_s(pc->Buff + idx, sizeof(pc->Buff), pstart, size);
				idx += size;
				pstart += size + 2;
			}
			//内容了
			return END_ERR;
		}
	}
	else if ((pfind = strstr(pstr, CONTENT_LENGTH)) != NULL)
	{
		pfind += CONTENT_LENGTH_COUNT;
		const char *pend = strstr(pfind, CONTENT_LINE);
		if (pend != NULL)
		{
			//长度已经得到
			int size = strtol(pfind, NULL, NULL);
			if (size >= 0)
			{
				pc->Content_Length = static_cast<UINT>(size);
				pc->Content_Start = pend + 4 - pc->Buff;
				goto CHECK_LOOP;
			}
		}
	}
	//继续接收
	return END_ERR;
}
UINT CheckContentIsEnd(HttpClientData *pc)
{
	if (pc->IsPost || (pc->RecvSize > POST_COUNT && _strnicmp(pc->Buff, POST_STR, POST_COUNT) == 0))
	{
		if (pc->IsPost == false)
		{
			int endCount = 0;
			for (int i = POST_COUNT; i < pc->RecvSize; ++i)
			{
				if (pc->Buff[i] == ' ')
				{
					endCount = i;
					break;
				}
			}
			if (endCount == 0)
				return END_ERR;
			int Count = endCount - POST_COUNT;
			if (Count > sizeof(pc->RequestUrl))
				return END_ERR;
			memcpy_s(pc->RequestUrl, sizeof(pc->RequestUrl), pc->Buff + POST_COUNT, Count);
			pc->RequestUrl[Count] = 0;
			pc->IsPost = true;
		}
		return CheckContentPostIsEnd(pc);
	}
	else if (pc->RecvSize > GET_COUNT && _strnicmp(pc->Buff, GET_STR, GET_COUNT) == 0)
	{
		const char *pfind = strchr(pc->Buff + GET_COUNT, ' ');
		if (pfind == NULL)
			return END_ERR;
		pc->IsPost = false;
		return pfind - pc->Buff;
	}
	else
		return END_ERR;
}
void SendResponse(HttpClientData *pc, const char *pcTime, const char *pcText)
{
	int ret = sprintf_s(pc->Buff, sizeof(pc->Buff), RESPONSE_TXT, pcTime, pcText);
	int count = send(pc->Socket, pc->Buff, ret, 0);
	//FILE *pp;
	//fopen_s(&pp, "d:\\httpresponse2.txt", "a+b");
	//fwrite(pc->Buff, 1, ret, pp);
	//fclose(pp);
	//Log("响应:%s, 字节:%d/%d, 发送:%d", pc->Buff, ret, ret, count);
}
//替换&,=符号
static ReplaceStrItem URL_REPLACE_ITEM[] = { ReplaceStrItem("\\u003d", "="), ReplaceStrItem("\\u0026", "&"), ReplaceStrItem("%3d", "="), ReplaceStrItem("%26", "&"), ReplaceStrItem("\\u003D", "="), ReplaceStrItem("%3D", "=") };
static const UINT URL_REPLACE_COUNT = sizeof(URL_REPLACE_ITEM) / sizeof(URL_REPLACE_ITEM[0]);
bool HttpServer::SplitStr(HttpClientData *pc, UINT endSize, HttpRecvData &recvData)
{
	if (strncmp(pc->Buff, GET_STR, GET_COUNT) != 0)
		return false;
	char TEMP_STR[HTTP_STR_COUNT];
	UINT size = ReplaceStr(pc->Buff, endSize, TEMP_STR, HTTP_STR_COUNT, URL_REPLACE_ITEM, URL_REPLACE_COUNT);
	if (size == REPLACE_ERR)
		return false;
	const char *pstr = TEMP_STR + GET_COUNT;
	const char *pstr_end = TEMP_STR + size;
	
	//检查请求的地址
	bool bFind = false;
	WebAddrData *pAddrData = NULL;
	for (UINT i = 0; i < m_InitData.AddrList.size(); ++i)
	{
		pAddrData = &m_InitData.AddrList[i];
		if (_strnicmp(pstr, pAddrData->WebAddr, pAddrData->WebAddrLength) == 0)
		{
			recvData.RequestIdx = i;
			recvData.RequestCode = pAddrData->RequestCode;
			pstr += pAddrData->WebAddrLength;
			bFind = true;
			break;
		}
	}
	if (bFind == false)
		return false;
	
	UINT validFields = 0;
	while (pstr < pstr_end)
	{ 
		const char *pfind = strchr(pstr, '=');
		if (pfind == NULL || pfind > pstr_end || pfind == pstr)
			break;
		USHORT count = pfind - pstr;
		if (pAddrData->bKeyToLower)
		{
			char *pstart = (char*)pstr;
			while (pstart != pfind)
			{
				*pstart = ::tolower(*pstart);
				++pstart;
			}
		}
		string appkey;
		appkey.append(pstr, count);
		UINT key = AECrc32(pstr, count, 0);
		string value;
		pstr = pfind + 1;
		pfind = strchr(pstr, '&');
		if (pfind == NULL || pfind > pstr_end)
			pfind = pstr_end;
		value.append(pstr, pfind - pstr);
		//Log("recv ->key:%s, value:%s, crc:%d", appkey.c_str(), value.c_str(), key);
		recvData.KeyValues.insert(make_pair(key, value));
		if (pAddrData->ValidationFields.size() > 0)
		{
			auto it = pAddrData->ValidationFields.find(key);
			if (it != pAddrData->ValidationFields.end())
				++validFields;
		}
		pstr = pfind + 1;
	}
	return (validFields == pAddrData->ValidationFields.size());
}
bool HttpServer::CheckPostData(HttpClientData *pc, HttpRecvData &recvData)
{
	//检查请求的地址
	bool bFind = false;
	WebAddrData *pAddrData = NULL;
	for (UINT i = 0; i < m_InitData.AddrList.size(); ++i)
	{
		pAddrData = &m_InitData.AddrList[i];
		if (_strnicmp(pc->RequestUrl, pAddrData->WebAddr, pAddrData->WebAddrLength) == 0)
		{
			recvData.RequestIdx = i;
			recvData.RequestCode = pAddrData->RequestCode;
			bFind = true;
			break;
		}
	}
	if (bFind == false)
		return false;
	recvData.PostData = pc->Buff;
	return true;
}
void GetGMTTimeStr(char* buff, int size, tm *newt)
{
	tm newt2;
	if (newt == NULL)
	{
		time_t t = time(NULL);
		gmtime_s(&newt2, &t);
		newt = &newt2;
	}
	sprintf_s(buff, size, "%s, %02d %s %d %02d:%02d:%02d GMT", WEEK_STR[newt->tm_wday], newt->tm_mday, MONTH_STR[newt->tm_mon], 1900 + newt->tm_year, newt->tm_hour, newt->tm_min, newt->tm_sec);
}
void HttpServer::_RecvThread()
{
	int idx = ::InterlockedIncrement(&m_ThreadNum) - 1;
	ThreadRecvData &trd = m_pThreadRecvList[idx];
	vector<HttpClientData*> clientList;
	fd_set  *pSet = CreateFDSet();
	timeval selectTime = { 0, 0 };
	char strTime[128] = { 0 };
	while (m_bRun)
	{
		GetGMTTimeStr(strTime, sizeof(strTime));
		UINT tick = timeGetTime();
		while (trd.NewSockeList.HasItem())
		{
			HttpClientData *pcd = new HttpClientData;
			pcd->Socket = trd.NewSockeList.GetItem();
			pcd->Tick = tick;
			pcd->RecvSize = 0;
			pcd->Content_Length = -1;
			pcd->Content_Start = 0;
			pcd->IsPost = false;
			pcd->Removed = false;
			clientList.push_back(pcd);
		}
		//接收数据。
		FD_ZERO(pSet);
		for (UINT i = 0; i < clientList.size();)
		{
			HttpClientData *pc = clientList[i];
			bool bTimeOut = int(tick - pc->Tick) > m_InitData.Timeout;
			if (bTimeOut || pc->Removed)
			{
				//Log("断开连接.");
				closesocket(pc->Socket);
				delete (pc);
				ListRemoveAt(clientList, i);
			}
			else
			{
				FD_ADD(pc->Socket, pSet);
				++i;
			}
		}
		if (FD_COUNT(pSet) == 0)
			goto SLEEP;

		int nRet = select(0, pSet, NULL, NULL, &selectTime);
		tick = timeGetTime();
		for (uint i = 0; i < clientList.size(); ++i)
		{
			HttpClientData *pc = clientList[i];
			if (FD_ISSET(pc->Socket, pSet) == false)
				continue;

			int nSize = recv(pc->Socket, pc->Buff + pc->RecvSize, sizeof(pc->Buff) - pc->RecvSize, 0);
			if (nSize == 0 || (nSize == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
			{
				pc->Removed = true;
			}
			else if (nSize > 0)
			{
				pc->Tick = tick;
				pc->RecvSize += nSize;
				UINT endSize = CheckContentIsEnd(pc);
				if (endSize != END_ERR)
				{
					//内容已经收到;
					HttpRecvData rd;
					if (pc->IsPost)
					{
						if (CheckPostData(pc, rd))
						{
							trd.ThreadRecvList.AddItem(rd);
							SendResponse(pc, strTime, m_InitData.AddrList[rd.RequestIdx].Response);
						}
						else
							SendResponse(pc, strTime, FAIL_TXT);
					}
					else if (SplitStr(pc, endSize, rd))
					{
						trd.ThreadRecvList.AddItem(rd);
						SendResponse(pc, strTime, m_InitData.AddrList[rd.RequestIdx].Response);
					}
					else
					{
						NormalCall(pc, strTime);
					}						
					pc->Removed = true;
				}
				else if (pc->RecvSize >= sizeof(pc->Buff))
				{
					//收到的内容不符合标准。
					pc->Removed = true;
				}
			}
		}
	SLEEP:
		Sleep(m_InitData.SleepTime);
	}
	::InterlockedDecrement(&m_ExitNum);
}

void HttpServer::NormalCall(HttpClientData *pc, const char* Time)
{
	SendResponse(pc, Time, FAIL_TXT);
}

void HttpServer::_AcceptThread()
{
	SOCKADDR_IN addr;
	int addrSize = sizeof(addr);
	UINT threadIdx = 0;
	while (m_bRun)
	{
		SOCKET s = accept(m_Socket, (sockaddr*)&addr, &addrSize);
		if (s != INVALID_SOCKET)
		{
			//新的加入
			InitSocket(s);
			++threadIdx;
			m_pThreadRecvList[threadIdx % m_InitData.ThreadNum].NewSockeList.AddItem(s);
		}
		for (UINT i = 0; i < m_InitData.ThreadNum; ++i)
		{
			if (m_pThreadRecvList[i].ThreadRecvList.HasItem())
			{
				m_RecvList.AddItem(m_pThreadRecvList[i].ThreadRecvList.GetItem());
				++m_TotalRecvDataCount;
			}
		}
		Sleep(m_InitData.SleepTime);
	}
	::InterlockedDecrement(&m_ExitNum);
}
void HttpServer::Shutdown()
{
	m_ExitNum = 0;
	closesocket(m_Socket);
	m_bRun = false;
	while (m_ExitNum != m_InitData.ThreadNum + 1)
		Sleep(10);
}