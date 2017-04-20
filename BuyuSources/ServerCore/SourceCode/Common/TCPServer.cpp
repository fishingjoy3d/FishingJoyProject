#include "stdafx.h"
//#include <process.h>
//#define HEARBEAT_AUTO			0
//#define THREAD_IOBUFF_NUM		256
//#define THREAD_IOBUFF_MAX		2048
//#define THREAD_IOBUFF_ADD_NUM	32
//#define HEARBEAT_TICK			2000
//#define REMOVE_DELAY			1000
//#define DISP_IDX_INVALID		255
//static GUID GuidAcceptEx = WSAID_ACCEPTEX;
//static GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
//static DWORD FLAG = 0;
//static DWORD TRANS_BYTES = 0;
//const int SOCKADDR_SIZE_16 = sizeof(SOCKADDR_IN) + 16;
//#define SetOverlapped(X) memset(&(X), 0, sizeof(X))
//static UINT WINAPI RecvThread(void *p)
//{
//	((TCPServer*)p)->_RecvThread();
//	return 0;
//}
//static UINT WINAPI DispatchThread(void *p)
//{
//	((TCPServer*)p)->_DispatchThread();
//	return 0;
//}
//static UINT WINAPI RecoverThread(void *p)
//{
//	((TCPServer*)p)->_RecoverThread();
//	return 0;
//}
//static UINT64 GetClientID(SOCKADDR_IN &addr)
//{
//	UINT64 key = addr.sin_addr.S_un.S_addr;
//	key = (key << 32) | addr.sin_port;
//	return key;
//}
//TCPServer::TCPServer()
//{
//}
//TCPServer::~TCPServer()
//{
//
//}
//bool TCPServer::Init(const ServerInitData &data)
//{
//	m_ExitCount = 0;
//	m_DispIndex = 0;
//	m_JoinNum = 0;
//	m_RecvIndex = 0;
//	m_bRun = true;
//
//	WSADATA wsaData;
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return false;
//	memcpy(&m_InitData, &data, sizeof(data));
//	m_Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
//	if (m_Socket == INVALID_SOCKET)
//		return false;
//
//	m_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, m_InitData.RecvThreadNum);
//	if (m_CompletionPort == NULL)
//		return false;
//	
//	HANDLE h = CreateIoCompletionPort((HANDLE)m_Socket, m_CompletionPort, m_Socket, NULL);
//	if (h == NULL)
//		return false;
//
//	SOCKADDR_IN addr;
//	memset(&addr, 0, sizeof(addr));
//	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(m_InitData.Port);
//	unsigned long arg = 1;
//	setsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, (const char*)&data.SocketRecvSize, sizeof(uint));
//	setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, (const char*)&data.SocketSendSize, sizeof(uint));
//	setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(arg));
//	setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&arg, sizeof(arg));
//	int ret = ::bind(m_Socket, (SOCKADDR*)&addr, sizeof(addr));
//	if (ret != 0)
//		return false;
//
//	ret = listen(m_Socket, m_InitData.ListenCount);
//	if (ret != 0)
//		return false;
//
//	DWORD dwBytes;
//	WSAIoctl(m_Socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx), &m_pAcceptEx, sizeof(m_pAcceptEx), &dwBytes, NULL, NULL);
//	WSAIoctl(m_Socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidGetAcceptExSockAddrs, sizeof(GuidGetAcceptExSockAddrs), &m_pGetAcceptEx, sizeof(m_pGetAcceptEx), &dwBytes, NULL, NULL);
//
//	m_DispNum = m_InitData.DispatchThreadNum;
//	m_pDispatchData = new MyDispatchData[m_DispNum];
//	::_beginthreadex(0, 0, RecoverThread, this, 0, 0);
//	for (int i = 0; i < min(MAX_SOCKET_THREAD, m_InitData.RecvThreadNum); ++i)
//	{
//		::_beginthreadex(0, 0, RecvThread, this, 0, 0);
//	}
//
//	for (int i = 0; i < min(MAX_SOCKET_THREAD, m_DispNum); ++i)
//	{
//		m_pDispatchData[i].OnlineNum = 0;
//		::_beginthreadex(0, 0, DispatchThread, this, 0, 0);
//	}
//
//	m_pAcceptData = new AcceptBuffData[m_InitData.MaxAcceptNumPerFrame];
//	for (UINT i = 0; i < 1; ++i)
//	{
//		AcceptBuffData *pabd = m_pAcceptData + i;
//		memset(pabd, 0, sizeof(AcceptBuffData));
//		pabd->PerIO.WsaBuff.buf = pabd->Buff;
//		pabd->PerIO.WsaBuff.len = ACCEPT_BUFF_SIZE;
//		pabd->PerIO.OptType = IO_ACCEPT;
//		_PostAccept(pabd);
//	}
//	return true;
//}
//void TCPServer::_PostSendFirstData(MyClientData *pc)
//{
//	pc->SendFirstData	= true;
//	ClientIOData *pd	= &pc->SendIO;
//	pd->OptType			= IO_SEND_FIRST_DATA;
//	pd->AttachData		= HEARBEAT_ID;
//	pd->WsaBuff.buf		= pc->Buff;
//	pd->WsaBuff.len		= sizeof(UINT);
//
//	pc->SendTick = timeGetTime();
//	SetOverlapped(pd->Overlapped);
//	DWORD bytes;
//	int ret = WSASend(pc->Socket, &pd->WsaBuff, 1, &bytes, FLAG, &pd->Overlapped, NULL);
//	if (ret == 0)
//	{
//		//_HandleSendBack(pc, pd, bytes, 0);
//	}
//	else if (WSAGetLastError() != ERROR_IO_PENDING)
//	{
//		//RemoveClient(pc ,REMOVE_SEND_ERROR, false);
//		//Log("_PostSendPort Error, Ret: %d, LastCode:%d", ret, WSAGetLastError());
//	}
//}
//MyClientData* TCPServer::_NewClient(NewClientData &ncd)
//{
//	int dispIdx = _GetDispIdx();
//	if (dispIdx == -1)
//	{
//		closesocket(ncd.AttachData);
//		return NULL;
//	}
//
//	SOCKADDR_IN* remote = &ncd.Addr;
//	int addrSize	= sizeof(SOCKADDR_IN);
//	UINT	IP		= remote->sin_addr.S_un.S_addr;
//	USHORT Port		= ntohs(remote->sin_port);
//
//	if (m_CmdHandler->CanConnected(m_InitData.ServerID, IP, Port) == false)
//	{
//		closesocket(ncd.AttachData);
//		return NULL;
//	}
//
//	UINT clientDataSize = sizeof(MyClientData) + m_InitData.BuffSize;
//	void *pdata = malloc(clientDataSize);
//	MyClientData *pc = new (pdata)MyClientData;
//	pc->Socket		= (SOCKET)ncd.AttachData;
//	pc->IP			= IP;
//	pc->Port		= Port;
//	pc->Removed		= false;
//	pc->RefCount	= 2;	//外部一个引用，内部一个DISPATCH引用
//	pc->RemoveCode	= REMOVE_NONE;
//	pc->SendID		= 1;
//	pc->RecvID		= 1;
//	pc->ServerPort	= 0;
//	pc->ClientAddr	= *remote;
//	pc->SendState	= CLIENT_SEND_NONE;
//	pc->WaitRemove	= false;
//	pc->bWaitAnswer = false;
//	pc->SendBackIO.WsaBuff.buf = (char*)&pc->SendBackIO.AttachData;
//	pc->SendBackIO.WsaBuff.len = sizeof(UINT);
//	pc->SendBackIO.OptType = IO_SEND_BACK;
//
//	pc->HearbeatIO.AttachData = HEARBEAT_ID;
//	pc->HearbeatIO.WsaBuff.buf = (char*)&pc->HearbeatIO.AttachData;
//	pc->HearbeatIO.WsaBuff.len = sizeof(UINT);
//	pc->HearbeatIO.OptType = IO_HEARBEAT;
//
//	pc->RecvIO.OptType		= IO_RECV;
//	pc->RecvIO.WsaBuff.buf	= pc->Buff;
//	pc->RecvIO.WsaBuff.len	= m_InitData.BuffSize;
//
//	pc->DispIdx			= dispIdx;
//	pc->RecvSize		= 0;
//	pc->Offset			= 0;
//	pc->SendFirstData	= false;
//	pc->RecvTick		= timeGetTime();
//	pc->SendTick		= 0;
//	pc->IsInScene		= false;
//	HANDLE h = CreateIoCompletionPort((HANDLE)ncd.AttachData, m_CompletionPort, (ULONG_PTR)pc, NULL);
//	if (h == NULL)
//	{
//		Log("_PostAccept CreateIoCompletionPort Failed:%d", WSAGetLastError());
//		closesocket(pc->Socket);
//		free(pc);
//		return NULL;
//	}
//
//#if USE_ACCEPT_RECV_DATA
//	m_CmdHandler->NewClient(m_InitData.ServerID, pc, ncd.Buff, ncd.RecvSize);
//#else
//	m_CmdHandler->NewClient(m_InitData.ServerID, pc);
//#endif
//	//只有版本号比较才会发送初始数据。
//	bool bSendFirst = m_InitData.AcceptRecvData && m_InitData.Version != 0;
//	if (bSendFirst)
//		_PostSendFirstData(pc);
//	m_pDispatchData[dispIdx].NewClients.AddItem(pc);
//	++m_JoinNum;
//	++m_pDispatchData[dispIdx].OnlineNum;
//	_PostRecv(pc);
//
//	return pc;
//}
//int TCPServer::_GetDispIdx()
//{
//	USHORT n = USHRT_MAX;
//	int idx = -1;
//	for (int i = 0; i < m_DispNum; ++i)
//	{
//		MyDispatchData *p = m_pDispatchData + i;
//		if (p->OnlineNum < n && p->NewClients.HasSpace())
//		{
//			idx = i;
//			n = (USHORT)p->OnlineNum;
//		}
//	}
//	return idx;
//}
//void TCPServer::_PostAccept(AcceptBuffData *abd)
//{
//	struct linger linger = { 1, 1 };
//	const unsigned long arg = 1;
//	DWORD dwBytes = 0;
//	ClientIOData *pIoData = &abd->PerIO;
//
//	SOCKET s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
//	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&arg, sizeof(arg));
//	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(arg));
//	setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const char*)&m_InitData.SocketRecvSize, sizeof(uint));
//	setsockopt(s, SOL_SOCKET, SO_SNDBUF, (const char*)&m_InitData.SocketSendSize, sizeof(uint));
//	setsockopt(s, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger));
//	SetOverlapped(pIoData->Overlapped);
//	pIoData->WsaBuff.buf	= abd->Buff;
//	pIoData->WsaBuff.len	= ACCEPT_BUFF_SIZE;
//	pIoData->OptType		= IO_ACCEPT;
//	pIoData->AttachData		= s;
//	int size = 0;
//	if (m_InitData.AcceptRecvData)
//		size = pIoData->WsaBuff.len - SOCKADDR_SIZE_16 * 2;
//	int ret = m_pAcceptEx(m_Socket, s, pIoData->WsaBuff.buf, size,
//		SOCKADDR_SIZE_16, SOCKADDR_SIZE_16, &dwBytes, &pIoData->Overlapped);
//	if (ret != 0)
//		Log("Accept Error:%u", WSAGetLastError());
//
//}
//void TCPServer::_PostRecv(MyClientData *pc)
//{
//	pc->RecvIO.WsaBuff.buf = pc->Buff + pc->Offset;
//	pc->RecvIO.WsaBuff.len = m_InitData.BuffSize - (pc->Offset	+ pc->RecvSize);
//	SetOverlapped(pc->RecvIO.Overlapped);
//	DWORD flag = 0;
//	DWORD bytes;
//	int ret = WSARecv(pc->Socket, &pc->RecvIO.WsaBuff, 1, &bytes, &flag, &pc->RecvIO.Overlapped, NULL);
//	if (ret == 0)
//	{
//		//_HandleRecvBack(pc, &pc->RecvIO, bytes, 0);
//	}
//	else if (WSAGetLastError() != ERROR_IO_PENDING)
//	{
//		//RemoveClient(pc, REMOVE_RECV_ERROR);
//		//Log("PostRecv Error: LastCode:%d", WSAGetLastError());
//	}
//}
//void TCPServer::_PostSendPing(MyClientData* pc)
//{
//	ClientIOData *pd = &pc->SendBackIO;
//	pc->SendTick = timeGetTime();
//	pc->SendBackIO.AttachData = PING_ID;
//	pc->SendBackIO.OptType = IO_PING;
//	SetOverlapped(pd->Overlapped);
//	DWORD bytes;
//	int ret = WSASend(pc->Socket, &pd->WsaBuff, 1, &bytes, FLAG, &pd->Overlapped, NULL);
//	if (ret != 0)
//	{
//		if (WSAGetLastError() != ERROR_IO_PENDING)
//		{
//			//Log("_PostSendHearbeat Err LastCode:%d", WSAGetLastError());
//			//RemoveClient(pc, REMOVE_SEND_ERROR);
//		}
//	}
//}
//
//void	TCPServer::_PostSendBack(MyClientData *pc, UINT id)
//{
//	ClientIOData *pd	= &pc->SendBackIO;
//	pd->AttachData		= id;
//	pd->OptType			= IO_SEND_BACK;
//	pc->SendTick		= timeGetTime();
//	
//	SetOverlapped(pd->Overlapped);
//	DWORD bytes;
//	int ret = WSASend(pc->Socket, &pd->WsaBuff, 1, &bytes, FLAG, &pd->Overlapped, NULL);
//	if (ret == 0)
//	{
//		//_HandleSendBack(pc, pd, bytes, 0);
//	}
//	else if (WSAGetLastError() != ERROR_IO_PENDING)
//	{
//		//RemoveClient(pc ,REMOVE_SEND_ERROR, false);
//		//Log("_PostSendBack Error, Ret: %d, LastCode:%d", ret, WSAGetLastError());
//	}
//}
//void TCPServer::_PostSendHearbeat(MyClientData *pc)
//{
//	ClientIOData *pd	= &pc->HearbeatIO;
//	pc->SendTick		= timeGetTime();
//	SetOverlapped(pd->Overlapped);
//	DWORD bytes;
//	int ret = WSASend(pc->Socket, &pd->WsaBuff, 1, &bytes, FLAG, &pd->Overlapped, NULL);
//	if (ret != 0)
//	{
//		if (WSAGetLastError() != ERROR_IO_PENDING)
//		{
//			//Log("_PostSendHearbeat Err LastCode:%d", WSAGetLastError());
//			//RemoveClient(pc, REMOVE_SEND_ERROR);
//		}
//	}
//}
//void TCPServer::_PostSend(MyClientData *pc, bool bResend)
//{
//	ClientIOData *pd = &pc->SendIO;
//	NetCmd*pcmd = (NetCmd*)pc->SendList.GetItem();
//	pc->bWaitAnswer = true;
//	pd->OptType		= IO_SEND;
//	pd->WsaBuff.buf = (char*)pcmd;
//	pd->WsaBuff.len = pcmd->GetCmdSize();
//	pc->SendState	= CLIENT_SEND_POST;
//	pc->SendCmdTick = pc->SendTick = timeGetTime();
//	SetOverlapped(pd->Overlapped);
//	DWORD bytes;
//	int ret = WSASend(pc->Socket, &pd->WsaBuff, 1, &bytes, FLAG, &pd->Overlapped, NULL);
//	if (ret == 0)
//	{
//		//_HandleSendBack(pc, pd, bytes, 0);
//	}
//	else if (WSAGetLastError() != ERROR_IO_PENDING)
//	{
//		//RemoveClient(pc ,REMOVE_SEND_ERROR, false);
//		//Log("PostSend Error, Ret: %d, LastCode:%d", ret, WSAGetLastError());
//	}
//}
//void TCPServer::Shutdown()
//{
//	m_ExitCount = 0;
//	m_bRun = false;
//	int count = m_InitData.RecvThreadNum + m_InitData.DispatchThreadNum + 1;
//	while (m_ExitCount != count)
//		Sleep(100);
//}
//
//void TCPServer::_PostSendVersionError(AcceptBuffData *pc)
//{
//	ClientIOData *pIoData = &pc->PerIO;
//	SetOverlapped(pIoData->Overlapped);
//	pc->AddrSize			= sizeof(pc->Addr);
//	pIoData->OptType		= IO_SEND_VERSION;
//	pIoData->WsaBuff.len	= sizeof(UINT) * 3;
//	UINT mask = -1;
//	memcpy(pc->Buff, &mask, sizeof(UINT));
//	memcpy(pc->Buff + 4, &m_InitData.Version, sizeof(UINT));
//	memcpy(pc->Buff + 8, &m_InitData.PathCrc, sizeof(UINT));
//	DWORD bytes;
//	int ret = WSASend((SOCKET)pIoData->AttachData, &pIoData->WsaBuff, 1, &bytes, FLAG, &pIoData->Overlapped, NULL);
//	if (ret == 0)
//	{
//		//_HandleSendBack(pc, pd, bytes, 0);
//	}
//	else if (WSAGetLastError() != ERROR_IO_PENDING)
//	{
//		//RemoveClient(pc ,REMOVE_SEND_ERROR, false);
//		//Log("_PostSendPort Error, Ret: %d, LastCode:%d", ret, WSAGetLastError());
//	}
//	//此时的SOCKET未绑定到完成端口。
//	closesocket(pIoData->AttachData);
//}
//void TCPServer::_HandleAcceptBack(AcceptBuffData *pdata, UINT recvSize)
//{
//	if (recvSize > ACCEPT_ALLOW_BUFF_SIZE || m_NewClientList.HasSpace() == false)
//	{
//		closesocket((SOCKET)pdata->PerIO.AttachData);
//		_PostAccept(pdata);
//		return;
//	}
//
//	//版本和路径RCRC比较
//	bool bcheckVersion = m_InitData.AcceptRecvData;
//	if (bcheckVersion)
//	{
//		bool ret = true;
//		UINT *pver = (UINT*)pdata->Buff;
//		if (recvSize < sizeof(UINT) * 2)
//		{
//			ret = false;
//		}
//		else
//		{
//			if (m_InitData.Version != 0 && *pver < m_InitData.Version)
//				ret = false;
//			if (m_InitData.PathCrc != 0 && *(pver + 1) != m_InitData.PathCrc)
//				ret = false;
//		}
//		if (ret == false)
//		{
//			_PostSendVersionError(pdata);
//			_PostAccept(pdata);
//			return;
//		}
//	}
//
//	SOCKADDR_IN* remote = NULL;
//	SOCKADDR_IN* local = NULL;
//	int remoteLen = sizeof(SOCKADDR_IN);
//	int localLen = sizeof(SOCKADDR_IN);
//	int size = 0;
//	if (m_InitData.AcceptRecvData)
//	{
//		size = pdata->PerIO.WsaBuff.len - SOCKADDR_SIZE_16 * 2;
//	}
//	m_pGetAcceptEx(pdata->PerIO.WsaBuff.buf, size,
//		SOCKADDR_SIZE_16, SOCKADDR_SIZE_16, (LPSOCKADDR*)&local, &localLen, (LPSOCKADDR*)&remote, &remoteLen);
//
//	NewClientData ncd;
//	ncd.AttachData = pdata->PerIO.AttachData;
//	ncd.Addr = *remote;
//	ncd.RecvSize = recvSize;
//	memcpy(ncd.Buff, pdata->Buff, recvSize);
//	m_NewClientList.AddItem(ncd);
//	_PostAccept(pdata);
//}
//bool TCPServer::Send(ServerClientData *pClient, NetCmd *pCmd)
//{
//	MyClientData *pc = (MyClientData*)pClient;
//	if (pc->SendList.HasSpace() == false)
//	{
//		RemoveClient(pc, REMOVE_CMD_SEND_OVERFLOW, false);
//		return false;
//	}
//	NetCmd *pNewCmd = CreateCmd(pCmd->GetCmdType(), pCmd->GetCmdSize());
//	if (!pNewCmd)
//		return false;
//	memcpy_s(pNewCmd, pCmd->GetCmdSize(), pCmd, pCmd->GetCmdSize());
//	pc->SendList.AddItem(pNewCmd);
//	return true;
//}
//void TCPServer::_RecoverThread()
//{
//	typedef vector<MyClientData*> ClientList;
//	ClientList	clientList;
//	//int updateCount	= 0;
//	while (m_bRun)
//	{
//		UINT count = 0;
//		while (m_NewClientList.HasItem() && count < m_InitData.MaxAcceptNumPerFrame)
//		{
//			NewClientData ncd = m_NewClientList.GetItem();
//			MyClientData *pc = _NewClient(ncd);
//			if (pc)
//			{
//				clientList.push_back(pc);
//				++count;
//			}
//		}
//		//检查是否有移除的玩家
//		UINT tick = timeGetTime();
//		for (UINT i = 0; i < clientList.size();)
//		{
//			MyClientData *pc = clientList[i];
//			if (pc->WaitRemove)
//			{
//				if (tick - pc->SendTick > REMOVE_DELAY)
//				{
//					RemoveClientImmediate(pc);
//					ListRemoveAt(clientList, i);
//					--m_JoinNum;
//					continue;
//				}
//			}
//			else if (pc->Removed)
//			{
//				//等待删除
//				if (pc->DispIdx != DISP_IDX_INVALID)
//				{
//					closesocket(pc->Socket);
//					pc->Socket = NULL;
//					--m_pDispatchData[pc->DispIdx].OnlineNum;
//					pc->DispIdx = DISP_IDX_INVALID;
//				}
//				if (pc->RefCount <= 0)
//				{
//					m_CmdHandler->Disconnect(m_InitData.ServerID, pc, pc->RemoveCode);
//					pc->WaitRemove = true;
//					pc->SendTick = tick;
//				}
//			}
//			++i;
//		}
//		//if (++updateCount > 100)
//		//{
//		//	//Log("Online:%d ", m_JoinNum);
//		//	updateCount = 0;
//		//}
//		Sleep(10);
//	}
//	::InterlockedIncrement(&m_ExitCount);
//}
//void TCPServer::_DispatchThread()
//{
//	int idx = ::InterlockedIncrement(&m_DispIndex) - 1;
//	vector<MyClientData*> clientList;
//	MyDispatchData &disp = m_pDispatchData[idx];
//	UINT timeout = min(HEARBEAT_TICK, m_InitData.Timeout >> 1);
//	while (m_bRun)
//	{
//		//Timer t;
//		//t.Begin();
//		while (disp.NewClients.HasItem())
//		{
//			MyClientData *pc = disp.NewClients.GetItem();
//			clientList.push_back(pc);
//		}
//		UINT tick = timeGetTime();
//		for (UINT i = 0; i < clientList.size();)
//		{
//			MyClientData *pc = clientList[i];
//			//状态被冻结
//			//-----------------------------------------------------------
//			bool bRemove = pc->Removed;
//			if (bRemove || tick - pc->RecvTick > m_InitData.Timeout)
//			{
//				RemoveClient(pc, bRemove ? REMOVE_NONE : REMOVE_TIMEOUT, true);
//				ListRemoveAt(clientList, i);
//				continue;
//			}
//			//1.在投递一个发送之前，不能发送其他数据
//			if (pc->SendFirstData || pc->SendState == CLIENT_SEND_POST)
//			{
//				++i;
//				continue;
//			}
//			//-----------------------------------------------------------
//			//有反馈
//			if (pc->SendList.HasItem())
//			{
//				_PostSend(pc, false);
//			}
//#if !HEARBEAT_AUTO
//			else if (tick - pc->SendTick > timeout)
//			{
//				_PostSendHearbeat(pc);
//			}
//#endif
//			++i;
//		}
//		//float d = t.Intecept();
//		//if (d > 2.f)
//		//	Log("Update Time:%f", d);
//		Sleep(m_InitData.SleepTime);
//	}
//	::InterlockedIncrement(&m_ExitCount);
//}
//void TCPServer::_RecvThread()
//{
//	int idx			= ::InterlockedIncrement(&m_RecvIndex) - 1;
//	ClientIOData	*pio = NULL;
//	OVERLAPPED		*pOverlapped = NULL;
//	DWORD			transSize;
//	MyClientData    *pc;
//	//创建缓冲区
//	while (m_bRun)
//	{
//		BOOL ret = ::GetQueuedCompletionStatus(m_CompletionPort, &transSize, (PULONG_PTR)&pc, &pOverlapped, INFINITE);
//		if (pOverlapped == NULL)
//		{
//			Log("Overlapped is null, trasnsSize:%d, LastCode:%d", transSize, WSAGetLastError());
//			break;
//		}
//		pio = CONTAINING_RECORD(pOverlapped, ClientIOData, Overlapped);
//		if (ret == 0)
//		{
//			//失败
//			//Log("PostFailed-> OptType:%d, TransSize:%d, ErrCode:%d", pio->OptType, transSize, WSAGetLastError());
//			if (transSize == 0)
//			{
//				if (pio->OptType != IO_ACCEPT && pio->OptType != IO_SEND_VERSION)
//				{
//					RemoveClient(pc, (pio->OptType == IO_SEND) ? REMOVE_SEND_ERROR : REMOVE_RECV_ERROR, false);
//				}
//				else
//				{
//					closesocket((SOCKET)pio->AttachData);
//					_PostAccept((CONTAINING_RECORD(pio, AcceptBuffData, PerIO)));
//				}
//				continue;
//			}
//			continue;
//		}
//		switch (pio->OptType)
//		{
//		case IO_RECV:
//			_HandleRecvBack(pc, pio, transSize);
//			break;
//		case IO_SEND:
//			pc->SendState = CLIENT_SEND_WAIT;
//			break;
//		case IO_HEARBEAT:
//#if HEARBEAT_AUTO
//			_PostRecv(pc);
//#endif
//			break;
//		case IO_ACCEPT:
//			_HandleAcceptBack(CONTAINING_RECORD(pio, AcceptBuffData, PerIO), transSize);
//			break;
//		case IO_SEND_FIRST_DATA:
//			_HandleSendFirstData(pc, pio);
//			break;
//		case IO_SEND_BACK:
//			_PostRecv(pc);
//			break;
//		case IO_PING:
//			_PostRecv(pc);
//			break;
//		case IO_SEND_VERSION:
//			closesocket((SOCKET)pio->AttachData);
//			_PostAccept(CONTAINING_RECORD(pio, AcceptBuffData, PerIO));
//		default:
//			Log("未知的OPT:%d", pio->OptType);
//			break;
//		}
//	}
//	::InterlockedIncrement(&m_ExitCount);
//}
//
//void TCPServer::_HandleSendFirstData(MyClientData *pc, ClientIOData* pd)
//{
//	pc->SendFirstData	= false;
//}
//void TCPServer::_HandleRecvBack(MyClientData *pc, ClientIOData *pd, UINT recvSize)
//{
//	pc->RecvTick = timeGetTime();
//	pc->RecvSize += recvSize;
//	bool bSendPing = false;
//	int count = 0;
//	while (pc->RecvSize >= sizeof(UINT))
//	{
//		if (++count > 100)
//			Log("_HandlRecv Loop:%d", pc->RecvSize);
//
//		char *pBuff = pc->Buff + pc->Offset;
//		UINT *recvID = (UINT*)(pBuff);
//		if (*recvID == HEARBEAT_ID)
//		{
//			pc->Offset	+= sizeof(UINT);
//			pc->RecvSize -= sizeof(UINT);
//		}
//		else if (*recvID == PING_ID)
//		{
//			pc->Offset += sizeof(UINT);
//			pc->RecvSize -= sizeof(UINT);
//			bSendPing = true;
//		}
//		else if (pc->RecvSize >= sizeof(NetCmd))
//		{
//			NetCmd *pCmdRecv = (NetCmd*)pBuff;
//			UINT cmdSize = *((USHORT*)&pBuff[0]);
//			if (cmdSize > m_InitData.BuffSize)
//			{
//				RemoveClient(pc, REMOVE_CMD_SIZE_ERROR, false);
//				return;
//			}
//			if (pc->RecvSize >= cmdSize)
//			{
//				NetCmd *pCmd = CreateCmd(cmdSize, pBuff);
//				pc->RecvList.AddItem(pCmd);
//				pc->Offset += cmdSize;
//				pc->RecvSize -= cmdSize;
//			}
//			else
//				break;
//		}
//		else
//			break;
//	}
//	UINT freeBuffSize = m_InitData.BuffSize - (pc->Offset + pc->RecvSize);
//	if (freeBuffSize < 64)
//	{
//		if (pc->RecvSize > 0)
//			memmove(pc->Buff, pc->Buff + pc->Offset, pc->RecvSize);
//		pc->Offset = 0;
//	}
//	if (bSendPing)
//		_PostSendPing(pc);
//	else
//		_PostRecv(pc);
//}
//void TCPServer::_HandleSendBack(MyClientData *pc, ClientIOData *pd, UINT sendSize)
//{
//	//投递成功
//}
//
//
//void TCPServer::RemoveClient(MyClientData *pc, RemoveType rt, bool subRefCount)
//{
//	pc->Removed = true;
//	if (pc->RemoveCode == REMOVE_NONE)
//	{
//		//Log("Remove:%s", GetRemoveTypeString(rt));
//		pc->RemoveCode = rt;
//	}
//
//	if (subRefCount)
//		::InterlockedDecrement(&pc->RefCount);
//}
//void TCPServer::RemoveClientImmediate(MyClientData *pc)
//{
//	while (pc->SendList.HasItem())
//		free(pc->SendList.GetItem());
//	while (pc->RecvList.HasItem())
//		free(pc->RecvList.GetItem());
//	pc->~MyClientData();
//	free(pc);
//
//}