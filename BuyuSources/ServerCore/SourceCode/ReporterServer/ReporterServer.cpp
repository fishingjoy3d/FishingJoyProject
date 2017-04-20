// DBServer.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#define		REPORT_SIZE		2048
#define		REPORT_PORT		45065
struct RSClientData
{
	SOCKET  Socket;
	bool	Removed;
	UINT	IP;
	USHORT	Port;
	UINT	Tick;
	UINT	RecvSize;
	//=================
	//以下信息固定长度
	UINT	MAGIC;
	USHORT	ExceptionLength;
	char	Buff[1];
};
SafeArray<RSClientData*> newReportList(1024);
UINT WINAPI RecvThread(void *p)
{
	SOCKET socket;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		Log("WSAStartup Failed.");
		return 0;
	}

	CreateSocketData csd;
	bool bret = CreateSocket(CST_TCP | CST_BIND, REPORT_PORT, 2048, 2048, csd);
	if (!bret)
	{
		Log("CreateSocket Failed.");
		return 0;
	}

	socket = csd.Socket;
	int nRet = ::listen(socket, 50);
	if (nRet != 0)
	{
		Log("Listen Failed!");
		return 0;
	}
	SOCKADDR_IN addr;
	int addrSize = sizeof(addr);
	vector<RSClientData*> clientList;
	FD_SET	*pSet = CreateFDSet();
	timeval time = { 0, 0 };
	while (1)
	{
		UINT tick = timeGetTime();
		for (int i = 0; i < 10; ++i)
		{
			SOCKET s = accept(socket, (sockaddr*)&addr, &addrSize);
			if (s == INVALID_SOCKET)
				break;
			RSClientData *pData = (RSClientData*)malloc(sizeof(RSClientData)+REPORT_SIZE);
			if (pData == null)
				break;
			pData->IP		= addr.sin_addr.S_un.S_addr;
			pData->Port		= ntohs(addr.sin_port);
			pData->Socket	= s;
			pData->Tick		= tick;
			pData->RecvSize = 0;
			pData->Removed	= false;
			clientList.push_back(pData);
		}
		FD_ZERO(pSet);
		for (UINT i = 0; i < clientList.size();)
		{
			RSClientData *pclient = clientList[i];
			if (pclient->Removed || tick - pclient->Tick > 3000)
			{
				closesocket(pclient->Socket);
				if (pclient->RecvSize > 0)
				{
					if (newReportList.HasSpace())
					{
						newReportList.AddItem(pclient);
					}
					else
					{
						free(pclient);
						Log("Report空间不够.");
					}
				}
				ListRemoveAt(clientList, i);
				continue;
			}

			FD_ADD(pclient->Socket, pSet);
			++i;
		}
		if (FD_COUNT(pSet) == 0)
			goto SLEEP;
		int kk = select(0, pSet, 0, 0, &time);
		for (UINT i = 0; i < clientList.size(); ++i)
		{
			RSClientData *pclient = clientList[i];
			if (FD_ISSET(pclient->Socket, pSet) == false)
				continue;
			char *pbuff = ((char*)&pclient->MAGIC) + pclient->RecvSize;
			int nSize = recv(pclient->Socket, pbuff, REPORT_SIZE - pclient->RecvSize, 0);
			if (nSize == 0 || (nSize == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
			{
				pclient->Removed = true;
			}
			else if (nSize > 0)
			{
				pclient->RecvSize += nSize;
			}
		}
	SLEEP:
		Sleep(10);
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	::_beginthreadex(0, 0, RecvThread, 0, 0, 0);
	char file[512];
	GetCurrentDirectoryA(sizeof(file), file);
	strcat_s(file, sizeof(file), "\\Exception.txt");
	Log("报告服务器已启动,保存路径:%s", file);
	char ipstr[100];
	char timestr[100];
	UINT count = 0;
	while (1)
	{
		time_t t = time(NULL);
		tm curtime;
		localtime_s(&curtime, &t);
		int strSize = sprintf_s(timestr, sizeof(timestr), " Time:%04d-%02d-%02d %02d:%02d:%02d\r\n", 1900 + curtime.tm_year, 1 + curtime.tm_mon, curtime.tm_mday, curtime.tm_hour, curtime.tm_min, curtime.tm_sec);
		
		if (newReportList.HasItem())
		{
			FILE *pfile;
			fopen_s(&pfile, file, "a+");
			if (pfile == NULL)
			{
				Log("文件打开失败:%s", file);
			}
			else
			{
				while (newReportList.HasItem())
				{
					RSClientData *pc = newReportList.GetItem();
					GetIPString(pc->IP, pc->Port, ipstr);
					int ipSize = strlen(ipstr);
					int strLength = min(pc->ExceptionLength, int(pc->RecvSize - 6));
					if (strLength < 0)
						strLength = 0;
					fwrite(pc->Buff, 1, strLength, pfile);
					fwrite(" IP:", 1, 4, pfile);
					fwrite(ipstr, 1, ipSize, pfile);
					fwrite(timestr, 1, strSize, pfile);
					++count;
					Log("收到报告 Num:%d，Size:%d, IP:%s,%s", count, strLength, ipstr, timestr);
				}
				fclose(pfile);
			}
		}
		Sleep(100);
	}
	return 0;
}

