#include "stdafx.h"
#include "NewServer.h"
static const int SOCKADDR_SIZE_16 = sizeof(SOCKADDR_IN)+16;
static GUID GuidAcceptEx = WSAID_ACCEPTEX;
static GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
#define THREAD_ACCEPT_COUNT		256			//单个线程接收的容器数量
#define ACCEPT_CLIENT_COUNT		256			//ACCEPT同时接收的最大数量
#define HEARBEAT_TICK			2000
#define ACCEPT_WAIT_TIMEOUT		3000		//连接验证超时
#define USE_PING_TEST			0			//开启测速
#define UDP_MTU					512			//UDP每个包的最大传输长度
#define UDP_SEND_INTERVAL		10			//UDP间隔多长时间发送一个包
#define UDP_BUFF_SIZE			1024
UINT CONNECT_OK = SERVER_CONNECT_MAGIC;


UINT WINAPI ThreadRecvTCP(void *p)
{
	((NewServer*)p)->_ThreadRecvTCP();
	return 0;
}

UINT WINAPI ThreadSendTCP(void *p)
{
	((NewServer*)p)->_ThreadSendTCP();
	return 0;
}
UINT WINAPI ThreadAccept(void *p)
{
	((NewServer*)p)->_ThreadAccept();
	return 0;
}
NewServer::NewServer() :m_NewClientList(ACCEPT_CLIENT_COUNT)
{
	m_pHandler = NULL;
}
NewServer::~NewServer()
{

}

bool NewServer::Init(const ServerInitData &data, bool bTCP)
{
	m_bAccept = true;
	memset(m_RecvThreadData, 0, sizeof(m_RecvThreadData));
	memset(m_SendThreadData, 0, sizeof(m_SendThreadData));
	m_ExitIndex = 0;
	m_OnlineNum = 0;
	m_RecvIndex = 0;
	m_SendIndex = 0;
	m_SendDataIndex = 0;
	m_bRun		= true;
	memcpy(&m_InitData, &data, sizeof(data));
	
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		Log("WSAStartup Failed.");
		return false;
	}

	CreateSocketData csd;
	bool bret = CreateSocket(CST_TCP | CST_BIND, data.Port, data.SocketRecvSize, data.SocketSendSize, csd);
	if (!bret)
	{
		Log("CreateSocket Failed.");
		return false;
	}
	m_Socket = csd.Socket;
	int nRet = ::listen(m_Socket, data.ListenCount);
	if (nRet != 0)
	{
		Log("listen Failed:%d.", WSAGetLastError());
		return false;
	}

	//创建线程数据
	for (int i = 0; i < data.RecvThreadNum; ++i)
	{
		m_RecvThreadData[i] = new RecvThreadData(THREAD_ACCEPT_COUNT);
		m_RecvThreadData[i]->OnlineNum = 0;
	}
	for (int i = 0; i < data.SendThreadNum; ++i)
	{
		m_SendThreadData[i] = new SendThreadData(THREAD_ACCEPT_COUNT);
		m_SendThreadData[i]->OnlineNum = 0;
	}
	
	//创建线程
	::_beginthreadex(0, 0, ThreadAccept, this, 0, 0);
	for (int i = 0; i < data.RecvThreadNum; ++i)
	{
		::_beginthreadex(0, 0, ThreadRecvTCP, this, 0, 0);
	}
	for (int i = 0; i < data.SendThreadNum; ++i)
	{
		::_beginthreadex(0, 0, ThreadSendTCP, this, 0, 0);
	}
	//ClientIO *pio = new ClientIO;
	//PostAccept(pio);
	Log("%d号服务已启动(SendThreadNum:%d, RecvThreadNum:%d, SendBuff:%d, RecvBuff:%d, SendCmdCount:%d, RecvCmdCount:%d, BuffSize:%d, SceneTick:%d, Timeout:%d, Valid:%d)",
		m_InitData.ServerID,
		m_InitData.SendThreadNum,
		m_InitData.RecvThreadNum,
		m_InitData.SocketSendSize,
		m_InitData.SocketRecvSize,
		m_InitData.MaxSendCmdCount,
		m_InitData.MaxRecvCmdCount,
		m_InitData.BuffSize,
		m_InitData.SceneHearbeatTick,
		m_InitData.Timeout,
		m_InitData.AcceptRecvData
		);

	return true;
}

void NewServer::GetSendAndRecvIndex(ushort &sendIdx, ushort &recvIdx)
{
	recvIdx = USHRT_MAX;
	int num = USHRT_MAX;
	for (WORD i = 0; i < m_InitData.RecvThreadNum; ++i)
	{
		if (m_RecvThreadData[i]->NewClientList.HasSpace() && m_RecvThreadData[i]->OnlineNum < num)
		{
			num = m_RecvThreadData[i]->OnlineNum;
			recvIdx = i;
		}
	}
	sendIdx = USHRT_MAX;
	num = USHRT_MAX;
	for (WORD i = 0; i < m_InitData.SendThreadNum; ++i)
	{
		if (m_SendThreadData[i]->NewClientList.HasSpace() && m_SendThreadData[i]->OnlineNum < num)
		{
			num = m_SendThreadData[i]->OnlineNum;
			sendIdx = i;
		}
	}
}
void NewServer::GetRecvIndex(ushort &recvIdx)
{
	recvIdx = USHRT_MAX;
	int num = USHRT_MAX;
	for (WORD i = 0; i < m_InitData.RecvThreadNum; ++i)
	{
		if (m_RecvThreadData[i]->NewClientList.HasSpace() && m_RecvThreadData[i]->OnlineNum < num)
		{
			num = m_RecvThreadData[i]->OnlineNum;
			recvIdx = i;
		}
	}
}
bool NewServer::RecvDataByTCP(ClientData *pc, int nSize)
{
	pc->RecvSize += nSize;
	int count = 0;
	while (pc->RecvSize >= sizeof(UINT))
	{
		++count;
		if (count > 1000)
		{
			Log("****RecvDataByTCP Loop Count:%d, recvSie:%d****", count, pc->RecvSize);
			pc->RemoveCode = REMOVE_RECVBACK_NOT_SPACE;
			pc->Removed = true;
			break;
		}
		/*else if (count > 10)
		{
			Log("****RecvDataByTCP Loop Count:%d, recvSie:%d****", count, pc->RecvSize);
		}*/

		char *pBuff = pc->Buff + pc->Offset;
		UINT recvID = *((UINT*)(pBuff));
		if (recvID == HEARBEAT_ID)
		{
			pc->Offset += sizeof(UINT);
			pc->RecvSize -= sizeof(UINT);
		}
		else if (recvID == PING_ID)
		{
			pc->Offset += sizeof(UINT);
			pc->RecvSize -= sizeof(UINT);
		}
		else if (pc->RecvSize >= sizeof(NetCmd))
		{
			NetCmd *pCmdRecv = (NetCmd*)pBuff;
			UINT cmdSize = pCmdRecv->GetCmdSize();
			if (cmdSize == 0)
			{
				ASSERT(false);
			}
			if (cmdSize > m_InitData.BuffSize)
			{
				pc->RemoveCode = REMOVE_CMD_SIZE_ERROR;
				pc->Removed = true;
				return false;
			}
			if (pc->RecvSize >= cmdSize)
			{
				if (pc->RecvList.HasSpace())
				{
					NetCmd *pCmd = CreateCmd(static_cast<WORD>(cmdSize), pBuff);
					pc->RecvList.AddItem(pCmd);
					pc->Offset += cmdSize;
					pc->RecvSize -= cmdSize;
				}
				else
				{
					pc->RemoveCode = REMOVE_CMD_RECV_OVERFLOW;
					pc->Removed = true;
					return false;
				}
			}
			else
				break;
		}
		else
			break;
	}

	UINT freeBuffSize = m_InitData.BuffSize - (pc->Offset + pc->RecvSize);
	if (freeBuffSize < 128)
	{
		if (pc->RecvSize > 0)
			memmove(pc->Buff, pc->Buff + pc->Offset, pc->RecvSize);
		pc->Offset = 0;
	}
	return true;
}
void NewServer::_ThreadRecvTCP()
{
	int idx = ::InterlockedIncrement(&m_RecvIndex) - 1;
	vector<ClientData*> clientList;
	RecvThreadData *pRecvData = m_RecvThreadData[idx];
	fd_set  *pSet = CreateFDSet();
	timeval time = { 0, 0 };
	while (m_bRun)
	{
		//1.接收新客户端
		//-----------------------------------------------------
		UINT tick = timeGetTime();
		while (pRecvData->NewClientList.HasItem())
		{
			ClientData *pc = pRecvData->NewClientList.GetItem();
			pc->RecvTick = tick;
			clientList.push_back(pc);
		}

		//2.检查状态
		//-----------------------------------------------------
		FD_ZERO(pSet);
		for (UINT i = 0; i < clientList.size();)
		{
			ClientData *pc = clientList[i];
			bool bTimeOut = int(tick - pc->RecvTick) > m_InitData.Timeout;
			if (pc->Removed || bTimeOut)
			{
				RemoveClient(pc, REMOVE_TIMEOUT);
				ListRemoveAt(clientList, i);
				::InterlockedDecrement(&pRecvData->OnlineNum);
				continue;
			}
			FD_ADD(pc->Socket, pSet);
			++i;
		}

		//3.接收数据
		//-----------------------------------------------------
		if (FD_COUNT(pSet) == 0)
			goto SLEEP;
		int nRet = select(0, pSet, NULL, NULL, &time);
		if (nRet == 0)
			goto SLEEP;
		tick = timeGetTime();
		for (uint i = 0; i < clientList.size(); ++i)
		{
			ClientData *pc = clientList[i];
			if (!FD_ISSET(pc->Socket, pSet))
				continue;

			int curPos = pc->Offset + pc->RecvSize;
			int nSize = recv(pc->Socket, (char*)pc->Buff + curPos, m_InitData.BuffSize - curPos, 0);
			if (nSize == 0 || (nSize == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
			{
				if (m_InitData.BuffSize - curPos == 0)
				{
					Log("接收缓冲区已满!buffSize:%d, curSize:%d", m_InitData.BuffSize, curPos);
				}
				pc->Removed = true;
			}
			else
			{
				RecvDataByTCP(pc, nSize);
				pc->RecvTick = tick;
			}
		}// end for
SLEEP:
		Sleep(m_InitData.SleepTime);
	}
	DeleteFDSet(pSet);
	::InterlockedIncrement(&m_ExitIndex);
}

void NewServer::_ThreadSendTCP()
{
	int idx = ::InterlockedIncrement(&m_SendIndex) - 1;
	SendThreadData *pSendData = m_SendThreadData[idx];
	vector<ClientData*> clientList;
	//int scenetick = m_InitData.SceneHearbeatTick;
	int	halfTimeout = min(HEARBEAT_TICK, m_InitData.Timeout >> 1);
	UINT hearbeat = HEARBEAT_ID;
	//UINT ping = PING_ID;
	NetCmd hearbeatCmd;
	hearbeatCmd.SetCmdSize(sizeof(NetCmd));
	hearbeatCmd.SetCmdType(m_InitData.CmdHearbeat);
	while (m_bRun)
	{
		UINT tick = timeGetTime();
		while (pSendData->NewClientList.HasItem())
			clientList.push_back(pSendData->NewClientList.GetItem());

		for (UINT i = 0; i < clientList.size(); )
		{
			ClientData *pc = clientList[i];
			if (pc->Removed)
			{
				RemoveClient(pc, REMOVE_NORMAL);
				ListRemoveAt(clientList, i);
				::InterlockedDecrement(&pSendData->OnlineNum);
				continue;
			}
			UINT k = 0;
			while (k < m_InitData.MaxSendCountPerFrame && pc->SendList.HasItem())
			{
				NetCmd *pcmd = pc->SendList.GetItemNoRemove();
				int ret = send(pc->Socket, (char*)pcmd, pcmd->GetCmdSize(), 0);
				if (ret == pcmd->GetCmdSize())
				{
					free(pc->SendList.GetItem());
					pc->SendTick = tick;
					pc->SendError = 0;
				}
				else
				{
					if (++pc->SendError >= 10)
					{
						//发送失败，对方缓冲区已满;
						Log("发送缓冲区已满!CmdSize:%d, LastCode:%d", pcmd->GetCmdSize(), WSAGetLastError());
						pc->Removed = true;
					}
					break;
				}
				++k;
			}
			if (k == 0)
			{
				//心跳
				if (int(tick - pc->SendTick) > halfTimeout)
				{
					//Log("SendHearbeat:%d, %d", scenetick , halfTimeout);
					int ret = send(pc->Socket, (char*)&hearbeatCmd, hearbeatCmd.GetCmdSize(), 0);
					if (ret == hearbeatCmd.GetCmdSize())
						 pc->SendTick = tick;
				}
			}
			++i;
		}
		Sleep(m_InitData.SleepTime);
	}
	::InterlockedIncrement(&m_ExitIndex);
}
void NewServer::AddNewClient(const AcceptClientData &acd)
{
	// 寻找适合的线程
	ushort recvIdx =0, sendIdx=0;
	GetSendAndRecvIndex(sendIdx, recvIdx);
	if (recvIdx == USHRT_MAX || sendIdx == USHRT_MAX)
	{
		Log("没有适合的空间加入新玩家:%d, %d", sendIdx, recvIdx);
		closesocket(acd.Socket);
		return;
	}

	//成功
	UINT size  = sizeof(ClientData) + m_InitData.BuffSize;
	ClientData *pc = new(malloc(size))ClientData(m_InitData.MaxSendCmdCount, m_InitData.MaxRecvCmdCount);
	pc->Removed = false;
	pc->RemoveCode = REMOVE_NONE;
	pc->IsInScene = false;
	pc->Socket = acd.Socket;
	pc->IP = acd.IP;
	pc->Port = acd.Port;
	pc->OutsideExtraData = NULL;
	pc->RefCount = 3;
	pc->SendTick = 0;
	pc->SendCmdTick = 0;
	pc->Offset = 0;
	pc->RecvSize = 0;
	pc->SendError = 0;
	m_pHandler->NewClient(m_InitData.ServerID, pc, (void*)acd.Buff, acd.RecvSize);

	RecvThreadData *precv = m_RecvThreadData[recvIdx];
	SendThreadData *psend = m_SendThreadData[sendIdx];
	::InterlockedIncrement(&m_OnlineNum);
	::InterlockedIncrement(&precv->OnlineNum);
	::InterlockedIncrement(&psend->OnlineNum);
	precv->NewClientList.AddItem(pc);
	psend->NewClientList.AddItem(pc);
	
}
bool SendFirstDataToUDPClient(AcceptClientData &acd, int recvbuff, int sendbuff)
{
	//发送新的端口到客户端
	CreateSocketData csd;
	if (!CreateSocket(CST_UDP | CST_BIND, 0, (uint)recvbuff, sendbuff, csd))
		return false;
	UINT sendData[3];
	UINT randID = RandUInt();
	sendData[0] = CONNECT_OK;
	sendData[1] = randID;
	sendData[2] = csd.Port;

	int ret = send(acd.Socket, (char*)sendData, sizeof(sendData), 0);
	if (ret != sizeof(sendData))
	{
		Log("SendFirstDataToUDPClient FAILED.");
		return false;
	}
	closesocket(acd.Socket);
	acd.Socket = csd.Socket;
	acd.RandID = randID;
	acd.Tick = timeGetTime();
	return true;
}
bool NewServer::CheckNewClient(AcceptClientData &data)
{
	//外部进行验证
	UINT ret = m_pHandler->CanConnected(m_InitData.ServerID, data.IP, data.Port, (void*)data.Buff, data.RecvSize);
	if (ret == CONNECT_CHECK_FAILED)
	{
		//关闭
		return false;
	}
	else if (ret > 0)
	{
		//发送错误数据再关闭
		send(data.Socket, (char*)data.Buff, ret, 0);
		return false;
	}
	else
	{
		int ret = send(data.Socket, (char*)&CONNECT_OK, sizeof(CONNECT_OK), 0);
		if (ret != sizeof(CONNECT_OK))
			return false;
		else
			return true;
	}
}

void NewServer::_ThreadAccept()
{
	const int sleepTime = 10;
	typedef list<AcceptClientData> AcceptList;
	vector<AcceptClientData> clientList;
	SOCKADDR_IN addr;
	int addrSize = sizeof(addr);
	fd_set  *pSet = CreateFDSet();
	timeval time = { 0, 0 };
	AcceptList::iterator it;
	UINT hearbeat = HEARBEAT_ID;
	while (m_bRun)
	{
		UINT addCount = 0;
		UINT tick = timeGetTime();
		while (addCount++ < m_InitData.MaxAcceptNumPerFrame)
		{
			SOCKET socket = accept(m_Socket, (sockaddr*)&addr, &addrSize);
			if (socket == INVALID_SOCKET)
				break;

			AcceptClientData acd;
			UINT ip			= addr.sin_addr.S_un.S_addr;
			ushort port		= ntohs(addr.sin_port);
			acd.IP			= ip;
			acd.Port		= port;
			acd.Socket		= socket;
			acd.RecvSize	= 0;
			acd.Tick		= tick;
			InitSocket(socket, m_InitData.SocketSendSize, m_InitData.SocketRecvSize, true);

			if (m_InitData.AcceptRecvData)
			{
				//接收数据。
				acd.WaitType = WAIT_0;
			}
			else
			{
				AddNewClient(acd);
				continue;
			}
			clientList.push_back(acd);
		}
		FD_ZERO(pSet);
		tick = timeGetTime();
		for (uint i = 0; i < clientList.size();)
		{
			AcceptClientData &acd = clientList[i];
			if (acd.Socket == NULL || tick - acd.Tick > ACCEPT_WAIT_TIMEOUT)
			{
				if (acd.Socket != NULL)
					closesocket(acd.Socket);
				ListRemoveAt(clientList, i);
				if (acd.Socket != NULL)
				{
					char xx[100];
					GetIPString(acd.IP, acd.Port, xx);
					Log("TCP Client recv timeout, level:%d, ip:%s", acd.WaitType, xx);
				}
				continue;
			}
			FD_ADD(acd.Socket, pSet);
			++i;
		}

		if (FD_COUNT(pSet) == 0)
			goto EXIT;

		int nRet = select(0, pSet, NULL, NULL, &time);
		tick = timeGetTime();
		for (UINT i = 0; i < clientList.size(); ++i)
		{
			AcceptClientData &acd = clientList[i];
			SOCKET s = acd.Socket;
			if (!FD_ISSET(s, pSet))
				continue;
			int ret = recv(s, acd.Buff, sizeof(acd.Buff), 0);
			if (ret == 0 || (ret == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
			{
				closesocket(acd.Socket);
				acd.Socket = NULL;
				continue;
			}
			acd.RecvSize += ret;
			if (CheckNewClient(acd) == false)
			{
				Log("验证失败.");
				closesocket(acd.Socket);
				closesocket(acd.Socket);
				acd.Socket = NULL;
				continue;
			}
			AddNewClient(acd);
			acd.Socket = NULL;
		}
	EXIT:
		Sleep(sleepTime);
		/*static UINT k = 0; 
		if (tick - k > 2000)
		{
			k = tick;
			if (curNum != m_OnlineNum)
			{
				curNum = m_OnlineNum;
				Log("Onlien:%d", m_OnlineNum);
			}
		}*/
	}
	DeleteFDSet(pSet);
	::InterlockedIncrement(&m_ExitIndex);
}
bool NewServer::Kick(ServerClientData *pClient, RemoveType rt)
{
	RemoveClient((ClientData*)pClient, rt);
	return true;
}
void NewServer::Shutdown()
{
	m_bRun = false;
	//PostQueuedCompletionStatus(m_Handle, 0, NULL, NULL);

	int count = m_InitData.RecvThreadNum + m_InitData.SendThreadNum + 1;
	while (m_ExitIndex != count)
		Sleep(100);
}
void NewServer::SetCmdHandler(INetHandler *pHandler)
{
	m_pHandler = pHandler;
}
bool NewServer::Send(ServerClientData *pClient, NetCmd *pCmd)
{
	ClientData *pc = (ClientData*)pClient;
	if (pc->SendList.HasSpace() == false)
	{
		pc->Removed = true;
		pc->RemoveCode = REMOVE_CMD_SEND_OVERFLOW;
		return false;
	}
	NetCmd *pNewCmd = CreateCmd(pCmd->GetCmdType(), pCmd->GetCmdSize());
	if (!pNewCmd)
		return false;
	memcpy_s(pNewCmd, pCmd->GetCmdSize(), pCmd, pCmd->GetCmdSize());
	pc->SendList.AddItem(pNewCmd);
	
	return true;
}
UINT NewServer::JoinNum()const
{
	return m_OnlineNum;
}
void NewServer::SwitchAccept(bool bEnable)
{
	if (m_bAccept != bEnable)
	{
		m_bAccept = bEnable;
	}
}
void NewServer::RemoveClient(ClientData *pc, RemoveType rt)
{
	pc->Removed = true;
	if (pc->RemoveCode == REMOVE_NONE)
		pc->RemoveCode = rt;

	if (::InterlockedDecrement(&pc->RefCount) == 0)
	{
		closesocket(pc->Socket);
		m_pHandler->Disconnect(m_InitData.ServerID, pc, pc->RemoveCode);
		while (pc->SendList.HasItem())
			free(pc->SendList.GetItem());
		while (pc->RecvList.HasItem())
			free(pc->RecvList.GetItem());
		pc->~ClientData();
		free(pc);
		::InterlockedDecrement(&m_OnlineNum);
	}
}