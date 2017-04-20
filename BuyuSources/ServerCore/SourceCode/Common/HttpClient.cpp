#include "stdafx.h"
#pragma warning(disable:4365) 
#include <WS2tcpip.h>
#define GET_REQUEST "GET /%s HTTP/1.1\r\nHost: %s:%d\r\nConnection: keep-alive\r\nAccept: text/html, application/xhtml+xml, application/xml;q=0.9, image/webp, */*;q=0.8\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64)\r\nAccept-Encoding: *;q=0\r\nAccept-Language: zh-CN,zh;q=0.8\r\n\r\n"
#define POST_REQUEST "POST /%s HTTP/1.1\r\nContent-Type: application/json;charset=utf-8\r\nHost: %s:%d\r\nContent-Length: %d\r\nAccept-Encoding: *;q=0\r\nConnection: keep-alive\r\n\r\n%s"

#define REQUEST_BUFF_SIZE 20480
static UINT WINAPI ThreadFunc(void *p)
{
	((HttpClient*)p)->_Thread();
	return 0;
}
bool HttpClient::Init(const HttpClientInitData &d)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		Log("WSAStartup Failed.");
		return false;
	}
	m_InitData = d;
	m_bRun = true;
	m_ExitIdx = 0;
	m_ThreadIdx = 0;
	for (UINT i = 0; i < m_InitData.ThreadNum; ++i)
		::_beginthreadex(0, 0, ThreadFunc, this, 0, 0);
	return true;
}
void HttpClient::Shutdown()
{
	m_bRun = false;
	while (m_ExitIdx != m_InitData.ThreadNum)
		Sleep(10);
}
USHORT HttpClient::AddHost(const char *pcURL, USHORT port)
{
	char ip[30];
	if (GetIPByName(pcURL, ip))
	{
		HttpHostData hhd;
		size_t URLLength = strlen(pcURL);
		if (URLLength >= 0xff)
		{
			ASSERT(false);
			return HOST_IDX_ERR;
		}
		hhd.HostSize = static_cast<BYTE>(URLLength);
		strncpy_s(hhd.Host, sizeof(hhd.Host), pcURL, hhd.HostSize);
		inet_pton(AF_INET, ip, &hhd.IP);
		hhd.Port = (port);
		USHORT idx = (USHORT)m_HostList.size();
		m_HostList.push_back(hhd);
		return idx;
	}
	else
	{
		return HOST_IDX_ERR;
	}
}
USHORT HttpClient::AddIP(const char *pcIP, USHORT port)
{
	size_t IPLength = strlen(pcIP);
	if (IPLength >= 0xff)
	{
		ASSERT(false);
		return HOST_IDX_ERR;
	}
	HttpHostData hhd;
	hhd.HostSize = static_cast<BYTE>(strlen(pcIP));

	strncpy_s(hhd.Host, sizeof(hhd.Host), pcIP, hhd.HostSize);
	inet_pton(AF_INET, pcIP, &hhd.IP);
	hhd.Port = (port);
	USHORT idx = (USHORT)m_HostList.size();
	m_HostList.push_back(hhd);
	return idx;
	
}
bool HttpClient::GetIPByName(const char *pcName, char ip[30])
{
	HOSTENT* pHS = gethostbyname(pcName);
	if (pHS != NULL)
	{
		in_addr addr;
		memcpy(&addr.S_un.S_addr, pHS->h_addr_list[0], pHS->h_length);
		GetIPString(addr.S_un.S_addr, ip);
		return true;
	}
	else
	{
		return false;
	}
}
#define TRANS_ENCODING	"Transfer-Encoding: chunked"
#define RESPONSE_END	"0\r\n\r\n"
#define CONTENT_LINE	"\r\n\r\n"
#define CONTENT_LENGTH  "Content-Length: "
const int CONTENT_LENGTH_COUNT = strlen(CONTENT_LENGTH);
enum CheckResult
{
	CHECK_WAIT,
	CHECK_FAILED,
	CHECK_OK,
};

CheckResult CheckResponse(HttpClientResult *hcr)
{
	if (hcr->RecvSize == 0)
		return CHECK_WAIT;
	if (hcr->Orignal)
	{
		if (hcr->RecvSize >= *((UINT*)hcr->RecvBuff))
		{
			hcr->RecvSize = *((UINT*)hcr->RecvBuff);
			memmove(hcr->RecvBuff, hcr->RecvBuff + 4, hcr->RecvSize);
			hcr->RecvBuff[hcr->RecvSize] = 0;
			return CHECK_OK;
		}
	}

	const char *pstr = hcr->RecvBuff;
	const char *pend = hcr->RecvBuff + hcr->RecvSize;
	const char *pfind;
CHECK_LOOP:
	if (hcr->Content_Length != -1)
	{
		const BYTE *pstart = (BYTE*)(hcr->RecvBuff + hcr->Content_Start);
		int ret = int(hcr->RecvSize - hcr->Content_Start - hcr->Content_Length);
		if (ret >= 0)
		{
			int contentlen = hcr->Content_Length;
			if (hcr->Content_Length > 0)
			{
				if (hcr->Content_Length >= 3 && *pstart == 0xef)
				{
					//BOM 三个字节 0xef,0xbb,0xbf
					hcr->Content_Length -= 3;
					pstart += 3;
				}
				memmove_s(hcr->RecvBuff, sizeof(hcr->RecvBuff), pstart, hcr->Content_Length);
				hcr->RecvSize = hcr->Content_Length;
				hcr->RecvBuff[hcr->RecvSize] = 0;
				return CHECK_OK;
			}
			else
			{
				hcr->RecvBuff[0] = 0;
				hcr->RecvSize = 0;
				return CHECK_OK;
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
				return CHECK_FAILED;
			pstart += 4;
			if (pstart >= pend)
				return CHECK_FAILED;

			int idx = 0;
			while (pstart < pend)
			{
				UINT size = strtol(pstart, NULL, 16);
				if (size == 0)
				{
					hcr->RecvBuff[idx] = 0;
					hcr->RecvSize = idx;
					return CHECK_OK;
				}
				pstart = strstr(pstart, "\r\n");
				if (pstart == NULL || pstart >= pend)
					break;
				pstart += 2;
				if (pstart >= pend)
					break;
				UINT strSize = pend - pstart;
				if (strSize < size || size > sizeof(hcr->RecvBuff) - idx)
					break;
				memmove_s(hcr->RecvBuff + idx, sizeof(hcr->RecvBuff), pstart, size);
				idx += size;
				pstart += size + 2;
			}
			//内容了
			return CHECK_FAILED;
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
				hcr->Content_Length = static_cast<UINT>(size);
				hcr->Content_Start = pend + 4 - hcr->RecvBuff;
				goto CHECK_LOOP;
			}
		}
	}
	return CHECK_WAIT;
}
bool RecvResponse(HttpClientResult *hcr)
{
	int nSize = recv(hcr->Socket, hcr->RecvBuff + hcr->RecvSize, sizeof(hcr->RecvBuff) - hcr->RecvSize, 0);
	if (nSize == 0 || (nSize == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
		return false;

	if (nSize > 0)
	{
		//Log("Recv:%d, total:%d, string:%s", nSize, hcr->RecvSize + nSize, hcr->RecvBuff);
		hcr->RecvSize += nSize;
		/*
		FILE *pp;
		char xx[100];
		static int kkk = 0;
		sprintf_s(xx, 100, "d:\\request%d.txt", kkk);
		fopen_s(&pp, xx, "a+b");
		fwrite(hcr->RecvBuff, 1, hcr->RecvSize, pp);
		fclose(pp);
		*/
	}
	return true;
}
bool SendRequest(HttpClientResult *hcr, HttpHostData &hhd)
{
	char url[REQUEST_BUFF_SIZE];
	int size = 0;
	if (hcr->Orignal)
	{
		if (hcr->PostSize == -1)
		{
			//https  get模式
			size = send(hcr->Socket, (const char*)&hcr->SendSize, 4, 0);
			strncpy_s(url, hcr->SendBuff, hcr->SendSize);
			size = hcr->SendSize;
		}
		else
		{
			size = send(hcr->Socket, (const char*)&hcr->PostSize, 4, 0);
			strncpy_s(url, hcr->PostBuff, hcr->PostSize);
			size = hcr->PostSize;
		}	
	}
	else if (hcr->PostSize == -1)
		size = sprintf_s(url, sizeof(url), GET_REQUEST, hcr->SendBuff, hhd.Host, hhd.Port);
	else
		size = sprintf_s(url, sizeof(url), POST_REQUEST, hcr->SendBuff, hhd.Host, hhd.Port, hcr->PostSize, hcr->PostBuff);

	/*FILE *pp;
	char xx[100];
	static int kkk = 0;
	sprintf_s(xx, 100, "d:\\post%d.txt", kkk);
	fopen_s(&pp, xx, "a+b");
	fwrite(url, 1, size, pp);
	fclose(pp);*/
	int ret = send(hcr->Socket, url, size, 0);
	if (ret != size)
		return false;
	return true;
}
bool ConnectRequest(HttpClientResult *hcr, HttpHostData &hhd)
{
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr		= hhd.IP;
	addr.sin_family		= AF_INET;
	addr.sin_port		= htons(hhd.Port);
	int ret = connect(hcr->Socket, (sockaddr*)&addr, sizeof(addr));
	if (ret != 0 && WSAGetLastError() != 10035)
	{
		Log("Connect Failed:%d", WSAGetLastError());
		return false;
	}
	return true;
}
bool InitRequest(HttpClientResult *hcr)
{
	CreateSocketData csd;
	if (CreateSocket(CST_TCP, 0, REQUEST_BUFF_SIZE, REQUEST_BUFF_SIZE, csd) == false)
	{
		Log("初始化Request Socket 失败.");
		return false;
	}
	hcr->Socket = csd.Socket;
	return true;
}
bool HttpClient::AddPostRequest(UINT_PTR ID, USHORT hostIdx, const char *pcURL, const char *pcPostData, bool bOrignal)
{
	if (hostIdx >= m_HostList.size())
		return false;

	HttpClientResult *hcr = new HttpClientResult;
	if (hcr == NULL)
		return false;

	UINT size = strlen(pcURL);
	strncpy_s(hcr->SendBuff, sizeof(hcr->SendBuff), pcURL, size);
	UINT postSize = strlen(pcPostData);
	strncpy_s(hcr->PostBuff, sizeof(hcr->PostBuff), pcPostData, postSize);
	hcr->RecvBuff[0] = 0;
	hcr->Orignal = bOrignal;
	hcr->ID = ID;
	hcr->SendSize = size;
	hcr->PostSize = postSize;
	hcr->State = REQUEST_NONE;
	hcr->Socket = NULL;
	hcr->HostIdx = hostIdx;
	hcr->RecvSize = 0;
	hcr->Content_Length = 0xffffffff;
	hcr->Content_Start = 0;
	if (m_RequestList.Push(hcr) == false)
	{
		Log("添加URLRequest失败");
		return false;
	}
	return true;
}
bool HttpClient::AddRequest(UINT_PTR ID, USHORT hostIdx, const char *pcURL, bool bOrignal)
{
	if (hostIdx >= m_HostList.size())
	{
		Log("Add Request Failed.");
		return false;
	}
	HttpClientResult *hcr = new HttpClientResult;
	if (hcr == NULL)
	{
		Log("Add Request Failed.");
		return false;
	}

	UINT size = strlen(pcURL);
	strncpy_s(hcr->SendBuff, sizeof(hcr->SendBuff), pcURL, size);
	hcr->Orignal	= bOrignal;
	hcr->ID			= ID;
	hcr->SendSize	= size;
	hcr->PostSize	= 0xffffffff;
	hcr->State		= REQUEST_NONE;
	hcr->Socket		= NULL;
	hcr->HostIdx	= hostIdx;
	hcr->RecvSize   = 0;
	hcr->Content_Length = 0xffffffff;
	hcr->Content_Start = 0;
	if (m_RequestList.Push(hcr) == false)
	{
		Log("添加URLRequest失败");
		return false;
	}
	//Log("添加HttpClient请求，ID:%u, 字符串:%s", ID, pcURL);
	return true;
}

void HttpClient::_Thread()
{
	UINT idx = ::InterlockedIncrement(&m_ThreadIdx) - 1;
	vector<HttpClientResult*> requestList;
	HttpClientResult *hcr;
	bool bRet;
	FD_SET *pReadSet = CreateFDSet();
	FD_SET *pWriteSet = CreateFDSet();
	timeval time = { 0, 0 };
	while (m_bRun)
	{
		UINT tick = timeGetTime();
		while(m_RequestList.Pop(hcr))
		{
			hcr->Tick = tick;
			requestList.push_back(hcr);
		}
		FD_ZERO(pReadSet);
		FD_ZERO(pWriteSet);
		for (UINT i = 0; i < requestList.size();)
		{
			HttpClientResult *hcr = requestList[i];
			if (tick - hcr->Tick > m_InitData.Timeout)
			{
				Log("Requset Timeout, ID:%u, Time:%d", hcr->ID, tick - hcr->Tick);
				hcr->State = REQUEST_FAILED;
			}

			if (hcr->State == REQUEST_NONE)
			{
				if (InitRequest(hcr))
				{
					hcr->State = REQUEST_CREATE;
				}
				else
				{
					Log("创建新的Request Socket失败.");
					hcr->State = REQUEST_FAILED;
				}
			}
			if (hcr->State == REQUEST_FAILED || hcr->State == REQUEST_OK)
			{
				if (hcr->Socket != NULL)
				{
					closesocket(hcr->Socket);
					hcr->Socket = NULL;
				}
				if (m_ResponseList.Push(hcr))
				{
					ListRemoveAt(requestList, i);
				}
				else
				{
					++i;
					Log("Push Request 到外部失败.");
				}
				continue;
			}
			FD_ADD(hcr->Socket, pReadSet);
			FD_ADD(hcr->Socket, pWriteSet);
			++i;
		};

		if (FD_COUNT(pReadSet) == 0)
			goto SLEEP;
		int ret = select(0, pReadSet, pWriteSet, NULL, &time);
		if (ret < 0)
			goto SLEEP;

		for (UINT i = 0; i < requestList.size(); ++i)
		{
			HttpClientResult *hcr = requestList[i];
			if (hcr->State == REQUEST_FAILED || hcr->State == REQUEST_OK)
				continue;

			switch (hcr->State)
			{
			
			case REQUEST_CREATE:
				if (ConnectRequest(hcr, m_HostList[hcr->HostIdx]))
				{
					hcr->State = REQUEST_CONNECTED;
					//break;继续下面的REQUEST_CONNECTED
				}
				else
				{
					break;
				}
			case REQUEST_CONNECTED:
				if (FD_ISSET(hcr->Socket, pWriteSet))
				{
					if (SendRequest(hcr, m_HostList[hcr->HostIdx]))
					{
						hcr->State = REQUEST_SEND;
						//break;继续下面的REQUEST_SEND
					}
					else
					{
						break;
					}
				}
			case REQUEST_SEND:
				if (FD_ISSET(hcr->Socket, pReadSet))
				{
					bRet = RecvResponse(hcr);
					CheckResult cr = CheckResponse(hcr);
					if (cr == CHECK_OK)
					{
						SYSTEMTIME pTime;
						GetLocalTime(&pTime);
						//Log("接收到数据-> ID:%u, 长度:%d, 主机地址:%s, Time: %02d:%02d:%02d", hcr->ID, hcr->RecvSize, m_HostList[hcr->HostIdx].Host, pTime.wHour, pTime.wMinute, pTime.wSecond);
						hcr->State = REQUEST_OK;
					}
					else if (cr == CHECK_FAILED)
					{
						hcr->State = REQUEST_FAILED;
					}
					else if (bRet == false || hcr->RecvSize >= sizeof(hcr->RecvBuff))
					{
						//已经断开，失败了
						hcr->State = REQUEST_FAILED;
					}
				}
				break;
			default:
				Log("未知的Request状态:%d", hcr->State);
				hcr->State = REQUEST_FAILED;
				break;
			}
			++i;
		}
	SLEEP:
		Sleep(m_InitData.SleepTime);
	}
	DeleteFDSet(pWriteSet);
	DeleteFDSet(pReadSet);
	::InterlockedIncrement(&m_ExitIdx);
}