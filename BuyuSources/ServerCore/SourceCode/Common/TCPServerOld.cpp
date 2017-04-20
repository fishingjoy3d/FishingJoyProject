#include "stdafx.h"
//#define SLEEP_TIME 10
//
//static UINT WINAPI ThreadFunc1(void *p)
//{
//	((TCPServerOld*)p)->AcceptThread();
//	return 0;
//}
//static UINT WINAPI ThreadFunc2(void *p)
//{
//	((TCPServerOld*)p)->SendThread();
//	return 0;
//}
//static UINT WINAPI ThreadFunc3(void *p)
//{
//	((TCPServerOld*)p)->RecvThread();
//	return 0;
//}
//
//TCPServerOld::TCPServerOld()
//{
//	m_bEnableAccept = true;
//	m_RecvIndex = 0;
//	m_SendIndex = 0;
//	m_JoinNum = 0;
//	m_ExitCount = 0;
//	m_CmdHandler = NULL;
//	memset((void*)m_RecvThreadData, 0, sizeof(m_RecvThreadData));
//	memset((void*)m_SendThreadData, 0, sizeof(m_SendThreadData));
//}
//TCPServerOld::~TCPServerOld()
//{
//
//}
//
//bool TCPServerOld::Init(const ServerInitData &data)
//{
//	memcpy(&m_InitData, &data, sizeof(data));
//	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (m_Socket == INVALID_SOCKET)
//		return false;
//
//	SOCKADDR_IN addr;
//	memset(&addr, 0, sizeof(addr));
//	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(m_InitData.Port);
//	int ret = ::bind(m_Socket, (SOCKADDR*)&addr, sizeof(addr));
//	if (ret != 0)
//		return false;
//
//	setsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, (const char*)&data.SocketRecvSize, sizeof(uint));
//	setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, (const char*)&data.SocketSendSize, sizeof(uint));
//
//	long noDelay = 1;  
//	int len = sizeof(long);
//	setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&noDelay, len);
//
//	unsigned long arg = 1;
//	ioctlsocket(m_Socket, FIONBIO, &arg);
//	listen(m_Socket, m_InitData.ListenCount);
//
//	m_bRun = true;
//	::_beginthreadex(0, 0, (ThreadFunc1), this, 0, 0);
//	for (int i = 0; i < min(MAX_SOCKET_THREAD, m_InitData.SendThreadNum); ++i)
//		::_beginthreadex(0, 0, (ThreadFunc2), this, 0, 0);
//	for (int i = 0; i < min(MAX_SOCKET_THREAD, m_InitData.RecvThreadNum); ++i)
//		::_beginthreadex(0, 0, (ThreadFunc3), this, 0, 0);
//
//	m_HeartCmd.SetCmdSize(sizeof(m_HeartCmd));
//	m_HeartCmd.SetCmdType(m_InitData.CmdHearbeat);
//	m_HeartCmd.CreateRandMask();
//	return true;
//}
//bool TCPServerOld::RecvData(TCPClientData *pscd, int nRecvSize)
//{
//	pscd->RecvSize += nRecvSize;
//	if (pscd->RecvSize > m_InitData.BuffSize)
//	{
//		Log(L"Recv Size Error1:%d", pscd->RecvSize);
//		return false;
//	}
//	while (pscd->RecvSize >= sizeof(UINT))
//	{
//		BYTE *pBuff = pscd->Buff + pscd->Offset;
//		UINT *recvID = (UINT*)pBuff;
//		if (*recvID == HEARBEAT_ID)
//		{
//			pscd->RecvSize -= sizeof(UINT);
//			pscd->Offset += sizeof(UINT);
//		}
//		else if (*recvID == PING_ID)
//		{
//			pscd->RecvSize -= sizeof(UINT);
//			pscd->Offset += sizeof(UINT);
//		}
//		else if (pscd->RecvSize > sizeof(NetCmd))
//		{
//			NetCmd *pCmdRecv = (NetCmd*)pBuff;
//			UINT cmdSize = *((USHORT*)&pBuff[0]);
//			if (cmdSize > m_InitData.BuffSize)
//			{
//				Log(L"Recv Size Error2:%d", cmdSize);
//				return false;
//			}
//			if (pscd->RecvSize >= cmdSize)
//			{
//				NetCmd *pCmd = (NetCmd*)pBuff;
//				pCmd = CreateCmd(cmdSize, pBuff);
//				if (pscd->RecvList.HasSpace())
//					pscd->RecvList.AddItem(pCmd);
//				else
//					RemoveClient(pscd);
//				pscd->Offset += cmdSize;
//				pscd->RecvSize -= cmdSize;
//			}
//			else
//			{
//				break;
//			}
//		}
//		else
//			break;
//	}
//	UINT freeBuffSize = m_InitData.BuffSize - (pscd->Offset + pscd->RecvSize);
//	if (freeBuffSize < 32 )
//	{
//		memmove(pscd->Buff, pscd->Buff + pscd->Offset, pscd->RecvSize);
//		pscd->Offset = 0;
//	}
//
//	return true;
//}
//ushort	TCPServerOld::GetRecvThreadMin()
//{
//	ushort n = USHRT_MAX;
//	ushort idx = 0;
//	for (ushort i = 0; i < m_InitData.RecvThreadNum; ++i)
//	{
//		if (m_RecvThreadData[i].Num < n)
//		{
//			n = (ushort)m_RecvThreadData[i].Num;
//			idx = i;
//		}
//	}
//	return idx;
//}
//ushort	TCPServerOld::GetSendThreadMin()
//{
//	ushort n = USHRT_MAX;
//	ushort idx = 0;
//	for (ushort i = 0; i < m_InitData.SendThreadNum; ++i)
//	{
//		if (m_SendThreadData[i].Num < n)
//		{
//			n = (ushort)m_SendThreadData[i].Num;
//			idx = i;
//		}
//	}
//	return idx;
//}
//void TCPServerOld::AcceptThread()
//{
//	UINT clientDataSize = sizeof(TCPClientData) + m_InitData.BuffSize;
//	const char chOpt = 1;
//	timeval time = { 0, 0 };
//	SOCKADDR_IN addrClient;
//	int sockLen = sizeof(SOCKADDR);
//	unsigned long arg = 1;
//	while (m_bRun)
//	{
//		UINT addCount = 0;
//		if (m_bEnableAccept)
//		while (addCount++ < m_InitData.MaxAcceptNumPerFrame)
//		{
//			SOCKET socket = accept(m_Socket, (sockaddr*)&addrClient, &sockLen);
//			if (socket == INVALID_SOCKET)
//				break;
//
//			//初始信息
//			if (m_CmdHandler->CanConnected(m_InitData.ServerID, addrClient.sin_addr.S_un.S_addr, addrClient.sin_port))
//			{
//				ioctlsocket(socket, FIONBIO, &arg);
//				int noDelay = 1;
//				int len = sizeof(noDelay);
//				setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&noDelay, len);
//				TCPClientData *pscd = ::new(malloc(clientDataSize)) TCPClientData;
//				pscd->Socket = socket;
//				pscd->IP = addrClient.sin_addr.S_un.S_addr;
//				pscd->Port = addrClient.sin_port;
//				pscd->Removed = false;
//				pscd->SendTick = 0;
//				pscd->RecvTick = GetTickCount();
//				pscd->Offset = 0;
//				pscd->RecvSize = 0;
//				pscd->RefCount = 3;
//				pscd->RemoveCode = REMOVE_NORMAL;
//				if (m_CmdHandler->NewClient(m_InitData.ServerID, pscd/*, NULL, 0*/))	//外部在NewClient为false时，外部不要调用Kick
//				{
//					AddJoinNum();
//					int idx1 = GetRecvThreadMin();
//					int idx2 = GetSendThreadMin();
//					AddRecvThreadNum(idx1);
//					AddSendThreadNum(idx2);
//					m_NewRecvList[idx1].AddItem(pscd);
//					m_NewSendList[idx2].AddItem(pscd);
//				}
//				else
//				{
//					free(pscd);
//					closesocket(socket);
//				}
//			}
//		}
//
//		Sleep(m_InitData.AcceptSleepTime);
//	}
//	Log("exit.");
//	::InterlockedIncrement(&m_ExitCount);
//}
//void TCPServerOld::RecvThread()
//{
//	int threadIdx = ::InterlockedIncrement(&m_RecvIndex) - 1;
//	fd_set  *pSet = CreateFDSet();
//	timeval time = { 0, 0 };
//	ClientVector recvList;
//	ClientList &newList = m_NewRecvList[threadIdx];
//	while (m_bRun)
//	{
//	
//		while (newList.HasItem())
//		{
//			recvList.push_back(newList.GetItem());
//		}
//		FD_ZERO(pSet);
//		UINT tick = GetTickCount();
//		for (UINT i = 0; i < recvList.size();)
//		{
//			TCPClientData *pscd = recvList[i];
//			if (pscd->Removed || tick - pscd->RecvTick > m_InitData.Timeout)
//			{
//				if (tick - pscd->RecvTick > m_InitData.Timeout)
//				{
//					//Log(L"Timeout:%d", tick - pscd->RecvTick);
//				}
//				RemoveClient(pscd);
//				ListRemoveAt(recvList, i);
//				RemoveRecvThreadNum(threadIdx);
//				continue;
//			}
//			FD_ADD(pscd->Socket, pSet);
//			++i;
//		}
//		if (FD_COUNT(pSet) > 0)
//		{
//		
//			int nRet = select(0, pSet, NULL, NULL, &time);
//			
//			if (nRet != 0)
//			for (uint i = 0; i < recvList.size(); ++i)
//			{
//				TCPClientData *pscd = recvList[i];
//				if (!FD_ISSET(pscd->Socket, pSet))
//					continue;
//
//				int curPos = pscd->Offset + pscd->RecvSize;
//				int nSize = recv(pscd->Socket, (char*)pscd->Buff + curPos, m_InitData.BuffSize - curPos, 0);
//				if (nSize == 0 || (nSize == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
//				{
//					//Log(L"recv error:%d, LastCode:%d", nSize, WSAGetLastError());
//					pscd->Removed = true;
//				}
//				else if (!RecvData(pscd, nSize) || pscd->RecvList.Count() > (int)m_InitData.MaxRecvCmdCount)
//				{
//					//Log(L"recv size error:%d, LastCode:%d", nSize, WSAGetLastError());
//					pscd->Removed = true;
//				}
//				else
//				{
//					pscd->RecvTick = tick;
//				}
//			}// end for
//			
//		}//end if
//		
//		Sleep(m_InitData.SleepTime);
//	}
//	DeleteFDSet(pSet);
//
//	for (UINT i = 0; i < recvList.size(); ++i)
//		RemoveClient(recvList[i]);
//	recvList.clear();
//	::InterlockedIncrement(&m_ExitCount);
//}
//
//void TCPServerOld::SendThread()
//{
//	int threadIdx = ::InterlockedIncrement(&m_SendIndex) - 1;
//	ClientList &clientList = m_NewSendList[threadIdx];
//	ClientVector sendList;
//	
//	UINT sendInterval = 2000;	//2秒一次心跳
//	UINT hearbeat = HEARBEAT_ID;
//	
//	while (m_bRun)
//	{
//	
//		while (clientList.HasItem())
//		{
//			sendList.push_back(clientList.GetItem());
//		}
//		UINT tick = GetTickCount();
//		for (UINT i = 0; i < sendList.size(); )
//		{
//			TCPClientData *pscd = sendList[i];
//			if (pscd->Removed)
//			{
//				RemoveClient(pscd);
//				ListRemoveAt(sendList, i);
//				RemoveSendThreadNum(threadIdx);
//				continue;
//			}
//			uint sendCount = 0;
//			while (pscd->SendList.HasItem() && sendCount < m_InitData.MaxSendCountPerFrame)
//			{
//				
//				NetCmd *pcmd = pscd->SendList.GetItemNoRemove();
//				pcmd->CreateRandMask();
//				int nRet = send(pscd->Socket, (const char*)pcmd, pcmd->GetCmdSize(), 0);
//				pscd->SendTick = timeGetTime();
//				if (nRet != pcmd->GetCmdSize())
//				{
//					if (WSAGetLastError() == 10035)
//					{
//						//继续下帧发送
//					}
//					else
//						Log(L"Send sendsize Error:%d, ErrorCode:%d", nRet, WSAGetLastError());
//				}
//				else
//				{
//					NetCmd* pcmd2 = pscd->SendList.GetItem();
//				
//					free(pcmd2);
//				}
//				++sendCount;
//			}
//			if (tick - pscd->SendTick > sendInterval)
//			{
//				pscd->SendTick = tick;
//				int nRet = send(pscd->Socket, (const char*)&hearbeat, sizeof(hearbeat), 0);
//			}
//			++i;
//		}
//		//Log(L"send :%f, count:%d, thread:%d", t.Elapsed, sendList.size(), threadIdx);
//		Sleep(m_InitData.SleepTime);
//	}
//	for (UINT i = 0; i < sendList.size(); ++i)
//		RemoveClient(sendList[i]);
//	sendList.clear();
//	::InterlockedIncrement(&m_ExitCount);
//}
//
//bool TCPServerOld::Kick(ServerClientData *pClient)
//{
//	TCPClientData *pscd = (TCPClientData*)pClient;
//	RemoveClient(pscd);
//	return true;
//}
//void TCPServerOld::RemoveClient(TCPClientData *pscd)
//{
//	pscd->Removed = true;
//	if (::InterlockedDecrement(&pscd->RefCount) <= 0)
//	{
//		m_CmdHandler->Disconnect(m_InitData.ServerID, pscd, REMOVE_NORMAL);
//		RemoveClientImmediate(pscd);
//		
//	}
//}
//bool TCPServerOld::Send(ServerClientData *pClient, NetCmd *pCmd)
//{
//	if (((TCPClientData*)pClient)->SendList.HasSpace() == false)
//	{
//		RemoveClient((TCPClientData*)pClient);
//		return false;
//	}
//	NetCmd *pNewCmd = CreateCmd(pCmd->GetCmdType(), pCmd->GetCmdSize());
//	if (!pNewCmd)
//		return false;
//	memcpy_s(pNewCmd, pCmd->GetCmdSize(), pCmd, pCmd->GetCmdSize());
//	((TCPClientData*)pClient)->SendList.AddItem(pNewCmd);
//	return true;
//}
//void TCPServerOld::RemoveClientImmediate(TCPClientData *pscd)
//{
//	RemoveJoinNum();
//	closesocket(pscd->Socket);
//	while (pscd->RecvList.HasItem())
//		free(pscd->RecvList.GetItem());
//	while (pscd->SendList.HasItem())
//		free(pscd->SendList.GetItem());
//	pscd->RecvList.~SafeArray();
//	pscd->SendList.~SafeArray();
//	free(pscd);
//}
//
//void TCPServerOld::Shutdown()
//{
//	m_ExitCount = 0;
//	m_bRun = false;
//	closesocket(m_Socket);
//	int threadNum = m_InitData.RecvThreadNum + m_InitData.SendThreadNum + 1;
//	while (m_ExitCount != threadNum)
//		Sleep(SLEEP_TIME);
//
//	for (int i = 0; i < MAX_SOCKET_THREAD; ++i)
//	{
//		while (m_NewSendList[i].HasItem())
//			RemoveClientImmediate(m_NewSendList[i].GetItem());
//	}
//	for (int i = 0; i < MAX_SOCKET_THREAD; ++i)
//	{
//		while (m_NewRecvList[i].HasItem())
//			RemoveClientImmediate(m_NewRecvList[i].GetItem());
//	}
//	Log(L"join num:%d", m_JoinNum);
//}