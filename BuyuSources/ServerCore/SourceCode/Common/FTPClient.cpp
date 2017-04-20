#include "stdafx.h"
#include "FTPClient.h"
volatile UINT OKCount = 0;

const char* FILE_LIST[] = { "fishpublish/rar/1.rar", "22.rar", "33.rar", "44.rar", "55.rar", "66.rar", "77.rar", "88.rar" };
const UINT FILE_COUNT   = sizeof(FILE_LIST) / sizeof(FILE_LIST[0]);
volatile UINT FILE_IDX = 0;
UINT WINAPI ThreadStart(void *p)
{
	((FTPClient*)p)->_Thread();
	return 0;
}
bool	FTPClient::Init(const char *IP, USHORT port, UINT threadNum)
{
	m_ThreadNum = threadNum;
	m_RecvCompletion = 0;
	m_RequestNum = 0;
	m_Run = true;
	m_ThreadIdx = 0;
	memset(&m_Addr, 0, sizeof(m_Addr));
	m_Addr.sin_family = AF_INET;
	inet_pton(AF_INET, IP, (void *)&m_Addr.sin_addr);
	m_Addr.sin_port = htons(port);

	for (UINT i = 0; i < threadNum; ++i)
		::_beginthreadex(0, 0, ThreadStart, this, 0, 0);
	return true;
}
bool	FTPClient::GetFile(const char *pcFile)
{
	FTPRequestData rd;
	rd.Version = false;
	rd.FileSize = strlen(pcFile);
	strncpy_s(rd.File, sizeof(rd.File), pcFile, rd.FileSize);
	UINT idx = ++m_RequestNum % m_ThreadNum;
	m_ThreadData[idx].NewClients.AddItem(rd);
	return true;
}
bool	FTPClient::GetVersion(const char *pcVer)
{
	FTPRequestData rd;
	rd.Version = true;
	rd.FileSize = strlen(pcVer);
	strncpy_s(rd.File, sizeof(rd.File), pcVer, rd.FileSize);
	UINT idx = ++m_RequestNum % m_ThreadNum;
	m_ThreadData[idx].NewClients.AddItem(rd);
	return true;
}

void	FTPClient::_Thread()
{
	UINT idx = ::InterlockedIncrement(&m_ThreadIdx) - 1;
	FTPClientThreadData *pdata = &m_ThreadData[idx];
	vector<FTPClientInfo*> clientList;
	CreateSocketData csd;
	FD_SET *pSet = CreateFDSet();
	timeval time = { 0, 0 };
	CHAR buff[128];
	UINT recvid = 0;
	int ret = 0;
	while (m_Run)
	{
		UINT tick = timeGetTime();
		while (pdata->NewClients.HasItem())
		{
			FTPRequestData rd = pdata->NewClients.GetItem();
			FTPClientInfo *pc = new FTPClientInfo;
			if (CreateSocket(CST_TCP, 0, 65536 * 2, 2048, csd) == false)
			{
				Log("创建SOCKET失败.");
				continue;
			}
			pc->Socket = csd.Socket;
			pc->OnlineTick = tick;
			ret = connect(pc->Socket, (sockaddr*)&m_Addr, sizeof(m_Addr));
			if (ret != 0 && WSAGetLastError() != 10035)
			{
				Log("连接服务器失败:%d", WSAGetLastError());
				continue;
			}
			if (rd.Version)
			{
				UINT cmdSize = sizeof(NetCmdCheckVersion) + (rd.FileSize - 1);
				NetCmdCheckVersion *pver = (NetCmdCheckVersion*)malloc(cmdSize);
				pver->SetCmdSize(cmdSize);
				pver->SetCmdType(CMD_CHECK_VERSION);
				strncpy_s(pver->Package, sizeof(pver->Package), "com.leduo.buyu.nearme.gamecenter", rd.FileSize);
				pver->Plateform		= 1;
				pver->ScreenHeight	= 100;
				pver->ScreenWidth	= 100;
				pver->Version		= 12312;
				pver->Length		= rd.FileSize;
				memcpy_s(pver->FileName, rd.FileSize, rd.File, rd.FileSize);
				pc->Cmd = pver;
				pc->FileCount = 1;
				clientList.push_back(pc);
			}
			else
			{
				UINT dd = 1;// RandRange(1, FILE_COUNT);
				UINT size = 0;
				for (UINT i = 0; i < dd; ++i)
					size += strlen(FILE_LIST[i]) + 1;
				UINT cmdSize = sizeof(NetCmdFileRequest) + (size - 1);
				NetCmdFileRequest *pfile = (NetCmdFileRequest*)malloc(cmdSize);
				pfile->SetCmdSize(cmdSize);
				pfile->SetCmdType(CMD_FILE_REQUEST);
				pfile->FileCount	= dd;
				pfile->Length		= size;
				pc->FileCount = dd;
				UINT idx = 0;
				for (UINT i = 0; i < dd; ++i)
				{
					UINT strsize = strlen(FILE_LIST[i]);
					memcpy(pfile->FileName + idx, FILE_LIST[i], strsize);
					idx += strsize;
					pfile->FileName[idx] = 0;
					++idx;
				}
				pc->Cmd = pfile;
				clientList.push_back(pc);
			}
		}
		FD_ZERO(pSet);
		tick = timeGetTime();
		for (UINT i = 0; i < clientList.size();)
		{
			FTPClientInfo *pc = clientList[i];
			if (tick - pc->Tick > 3000 || pc->Socket == NULL)
			{
				if (pc->Socket != NULL)
				{
					if (pc->Cmd != NULL)
						Log("发送命令超时,发送tick:%d", tick - pc->SendTick);
					else
						Log("接收数据超时,已接收大小:%d", pc->RecvSize);
				}
				else
					closesocket(pc->Socket);
				ListRemoveAt(clientList, i);
				continue;
			}
			if (pc->Cmd != NULL)
			{
				if (tick - pc->SendTick > 200)
				{
					pc->SendTick = tick;
					ret = send(pc->Socket, (char*)pc->Cmd, pc->Cmd->GetCmdSize(), 0);
					if (ret == pc->Cmd->GetCmdSize())
					{
						pc->Tick = tick;
						SAFE_FREE(pc->Cmd);
					}
					else
					{
						if (WSAGetLastError() == 10057)
						{
							//还没有连接
							pc->Tick = tick;
						}
					}
				}
			}
			else
			{
				FD_ADD(pc->Socket, pSet);
			}
			++i;
		}
		if (FD_COUNT(pSet) == 0)
			goto SLEEP;
		select(0, pSet, 0, 0, &time);
		for (UINT i = 0; i < clientList.size(); ++i)
		{
			FTPClientInfo *pc = clientList[i];
			if (FD_ISSET(pc->Socket, pSet) == false)
				continue;

			if (pc->Buff == NULL)
			{
				ret = recv(pc->Socket, buff, 4, 0);
				if (ret == 0 || (ret == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
				{
					Log("接收数据大小断开");
					closesocket(pc->Socket);
					pc->Socket = NULL;
					continue;
				}
				if(ret >= 4)
				{
					pc->Tick = tick;
					UINT recvid = *(UINT*)(buff);
					if (recvid == 0xEFFFFFFF)
					{
						//正在等待打开文件中
					}
					else if ((recvid & 0xF0000000) == 0xF0000000)
					{
						//文件大小
						recvid &= 0x0fffffff;
						Log("Recv Size:%d", recvid);
						pc->Buff = (char*)malloc(recvid);
						if (pc->Buff == NULL)
						{
							Log("***<malloc failed:%d, %u>***", recvid, GetLastError());
						}
						pc->BuffLength = recvid;
						if (ret > 4)
						{
							memcpy_s(pc->Buff, pc->BuffLength, buff + 4, ret - 4);
							pc->RecvSize = ret - 4;
						}
						pc->OnlineTick = tick;
					}
					else if ((recvid & 0xC0000000) == 0xC0000000)
					{
					}
					else if ((recvid & 0x80000000) != 0)
					{
						
					}
					pc->OnlineTick = tick;
				}
			}
			else
			{
				ret = recv(pc->Socket, pc->Buff + pc->RecvSize, pc->BuffLength - pc->RecvSize, 0);
				if (ret == 0 || (ret == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
				{
					closesocket(pc->Socket);
					pc->Socket = NULL;
					UINT recvBuffSize = pc->BuffLength - pc->RecvSize;
					Log("接收数据断开，已接收:%d Bytes, 总大小:%d Bytes，时长:%0.2f, Code:%u/%u", pc->RecvSize, pc->BuffLength, (tick - pc->OnlineTick) / 1000.0f, ret, WSAGetLastError());
					continue;
				}
				if (ret > 0)
				{
					pc->RecvSize += ret;
					pc->Tick = tick;
					if (pc->RecvSize == pc->BuffLength)
					{
						
						/*FILE *pFile;
						char xx[100];
						UINT idx =InterlockedIncrement(&FILE_IDX);
						sprintf_s(xx, 100, "recv_file_%d.rar", idx);
						fopen_s(&pFile, xx, "wb");
						fwrite(pc->Buff, 1, pc->BuffLength, pFile);
						fclose(pFile);*/
						++pc->RecvFileCount;
						if (pc->RecvFileCount == pc->FileCount)
						{
							float time = (tick - pc->OnlineTick) / 1000.0f;
							float spd = (pc->BuffLength / 1024.0f) / time;
							Log("%d | 接收完成 大小:%d kb，时长:%0.2f, 速度:%0.2f KB", ::InterlockedIncrement(&OKCount), pc->BuffLength / 1024, time, spd);
							SAFE_FREE(pc->Buff);
							closesocket(pc->Socket);
							pc->Socket = NULL;
						}
						else
						{
							Log("接收文件:%d/%d", pc->RecvFileCount, pc->FileCount);
							SAFE_FREE(pc->Buff);
							pc->BuffLength = 0;
							pc->RecvSize = 0;
						}
					}
				}
			}
			
		}

	SLEEP:
		Sleep(10);
	}
}
