#include "stdafx.h"
#include "UDPServer.h"
#define UDP_PER_RECV_COUNT  3
#define UDP_CMD_VECTOR_SIZE 32
#define UDP_MTU 512
#define UDP_MAX_SEND_SIZE 2048
#define BUFF_MIN_SIZE 4096
#define UDP_CLIENT_MIN_SIZE 4
#define THREAD_ACCEPT_COUNT		512		//单个线程接收的容器数量
#define UDP_VALIDATION_TICK		50
#define UDP_VALIDATION_ID		0x8fffffff
#define UDP_BUFF_SIZE			2048
#define HEARBEAT_TICK			1000
#define UDP_MIN_INTERVAL		5
const int UDP_MIN_CMD_SIZE = sizeof(UINT) + sizeof(UINT) + sizeof(NetCmd);
static UINT WINAPI ThreadAccept(void *p)
{
	((NewUDPServer*)p)->_ThreadAccept();
	return 0;
}
static UINT WINAPI ThreadRecv(void *p)
{
	((NewUDPServer*)p)->_ThreadRecv();
	return 0;
}

enum
{
	SEND_NONE,
	SEND_WAIT_NEWPORT,
	SEND_WAIT_HEARBEAT,
};
struct AcceptUDPClient
{
	SOCKADDR_IN		ClientAddr;
	SOCKET			ClientSocket;
	SOCKET			NewSocket;
	USHORT			NewPort;
	UINT			Tick;
	UINT			SendTick;
	UINT			Rand1;
	UINT			Rand2;
	BYTE			SendType;
	BYTE			SendCount;
	bool			Removed;
	UINT			NewServerIP;
	USHORT			NewServerPort;
};
NewUDPServer::NewUDPServer()
{

}
NewUDPServer::~NewUDPServer()
{

}
static void InnerGetIPString(SOCKADDR_IN &addr, char *buff)
{
	UINT ip = addr.sin_addr.S_un.S_addr;
	ushort port = ntohs(addr.sin_port);
	GetIPString(ip, port, buff);
}
bool NewUDPServer::Init(const ServerInitData &data, bool btcp)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		Log("WSAStartup Failed.");
		return false;
	}

	m_bAccept = true;
	memset(m_RecvThreadData, 0, sizeof(m_RecvThreadData));
	m_ExitIndex = 0;
	m_OnlineNum = 0;
	m_RecvIndex = 0;
	m_bRun = true;
	memcpy(&m_InitData, &data, sizeof(data));

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
		m_RecvThreadData[i] = new UDPRecvThreadData(THREAD_ACCEPT_COUNT);
		m_RecvThreadData[i]->OnlineNum = 0;
	}

	//创建线程
	::_beginthreadex(0, 0, ThreadAccept, this, 0, 0);
	for (int i = 0; i < data.RecvThreadNum; ++i)
	{
		::_beginthreadex(0, 0, ThreadRecv, this, 0, 0);
	}

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

//优化UDP：
//1.有新命令立即发送
//2.一直读取数据，直到无法读取。
void NewUDPServer::GetRecvIndex(ushort &recvIdx)
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

bool NewUDPServer::RecvDataByUDP(UDPClientData *pc, char *Buff, int nSize, UINT tick)
{
	UNREFERENCED_PARAMETER(tick);
	char *pBuff = Buff;
	char *pEnd = Buff + nSize;
	while (pBuff < pEnd)
	{
		UINT recvID = *((UINT*)(pBuff));
		pBuff += 4;
		if (recvID == HEARBEAT_ID || recvID == PING_ID)	//心跳
		{
			//UINT delta = timeGetTime() - pc->SendTick;
			//Log("Recv Hearbeat:%d", delta);
		}
		else if (IS_ANSWER(recvID))	//收到回复 
		{
			RESET_ANSWER(recvID);
			//Log("Recv Answer:%d, sendSize:%d", recvID, pc->SendSize);
			if (recvID == pc->SendID && pc->SendSize != 0)
			{
				//Log("SendCmdOK, count:%d", pc->SendCmdCount);
				pc->ResendCount = 0;
				pc->SendSize = 0;
				if (pc->pSendBuff != (UINT*)pc->Buff)
				{
					free(pc->pSendBuff);
					pc->pSendBuff = NULL;
				}
			}
		}
		else
		{
			//收到命令
			NetCmd *pcmd = (NetCmd*)pBuff;
			if (pcmd->GetCmdSize() > m_InitData.BuffSize || pcmd->GetCmdSize() > pEnd - pBuff)
			{
				Log("命令的长度不正确:%d, %d, %d", pcmd->GetCmdSize(), m_InitData.BuffSize, pEnd - pBuff);
				pc->RemoveCode = REMOVE_CMD_SIZE_ERROR;
				pc->Removed = true;
				return false;
			}
			else if (recvID == pc->RecvID + 1)
			{
				//对方第一次发送的命令
				++pc->RecvID;
				if (pc->RecvList.HasSpace())
				{
					NetCmd *pnewcmd = CreateCmd(pcmd->GetCmdSize(), pBuff);
					pc->RecvList.AddItem(pnewcmd);
				}
				else
				{
					pc->RemoveCode = REMOVE_CMD_RECV_OVERFLOW;
					pc->Removed = true;
					return false;
				}
			}
			else
			{
				//Log("Recv OldID:%d, CurID:%d", recvID, pc->RecvID);
			}
			pc->bSendBackID = true;
			pBuff += pcmd->GetCmdSize();
		}
	}//end while
	return true;
}
void SendBackID(UDPClientData *pc, UINT tick)
{
	UINT backID = pc->GWIsInScene ? SET_GW_ANSWER(pc->RecvID) : SET_ANSWER(pc->RecvID);
	pc->SendTick = tick;
	int ret = send(pc->Socket, (char*)&backID, 4, 0);
	if (ret == 4)
	{
		pc->bSendBackID = false;
	}
}
void SendCmdData(UDPClientData *pc, bool bresend, UINT tick, vector<UINT*> &tempList)
{
	if (pc->SendSize == 0)
		pc->SendSize = 4;

	//1.拼包,计算包大小
	//----------------------------------
	UINT size = pc->SendSize;
	tempList.clear();
	while (size < UDP_MTU && pc->SendList.HasItem())
	{
		UINT  *curData = (UINT*)pc->SendList.GetItemNoRemove();
		NetCmd *pcmd = (NetCmd*)(curData + 2);
		UINT cmdSize = pcmd->GetCmdSize() + 4;
		if (cmdSize + size < UDP_BUFF_SIZE)
		{
			tempList.push_back(curData);
			pc->SendList.RemoveTopItem();
			size += cmdSize;
		}
		else
			break;
	}
	//2.多个包还是单个包
	//----------------------------------
	if (size != pc->SendSize)
	{
		if (bresend || tempList.size() > 1)
		{
			//现有包 + 新包
			if (pc->pSendBuff)
			{
				//使用pc->Buff;
				if (pc->pSendBuff != (UINT*)pc->Buff)
				{
					memcpy(pc->Buff, pc->pSendBuff, pc->SendSize);
					free(pc->pSendBuff);
					pc->pSendBuff = (UINT*)pc->Buff;
				}
			}
			else
				pc->pSendBuff = (UINT*)pc->Buff;

			char *pstr = ((char*)(pc->pSendBuff)) + pc->SendSize;
			for (UINT i = 0; i < tempList.size(); ++i)
			{
				UINT  *curData = tempList[i];
				NetCmd *pcmd = (NetCmd*)(curData + 2);
				*((UINT*)pstr) = ++pc->SendID;
				pstr += 4;
				memcpy(pstr, pcmd, pcmd->GetCmdSize());
				pstr += pcmd->GetCmdSize();
				free(curData);
			}
			//Log("多包发送, count:%d, size:%d, resend:%s", tempList.size() + 1, size, bresend ? "yes":"no");
		}
		else
		{
			//单个包
			pc->pSendBuff = tempList[0];
			*(pc->pSendBuff + 1) = ++pc->SendID;
		}
	}
	pc->SendSize = size;
	*pc->pSendBuff = pc->GWIsInScene ? SET_GW_ANSWER(pc->RecvID) : SET_ANSWER(pc->RecvID);
	int ret = send(pc->Socket, (char*)pc->pSendBuff, pc->SendSize, 0);
	pc->SendTick = tick;
	if (static_cast<UINT>(ret) == pc->SendSize)
	{
		pc->SendCmdTick = tick;
		pc->bSendBackID = false;
	}
	else
		pc->SendCmdTick = 0;	//下次立即发送
	return ;
}
void NewUDPServer::_ThreadRecv()
{
	int idx = ::InterlockedIncrement(&m_RecvIndex) - 1;
	vector<UDPClientData*> clientList;
	UDPRecvThreadData *pRecvData = m_RecvThreadData[idx];
	fd_set  *pSet = CreateFDSet();
	timeval time = { 0, 0 };
	int scenetick = m_InitData.SceneHearbeatTick;
	int	halfTimeout = min(HEARBEAT_TICK, m_InitData.Timeout >> 1);
	UINT hearbeat = HEARBEAT_ID;
	UINT ping = PING_ID;
	char *pBuff = (char*)malloc(m_InitData.BuffSize);
	const int RESEND_SAFE_COUNT		= 5;	//安全重发次数
	const int RESEND_MAX_COUNT		= 15;	//最大重发次数
	const int RESEND_INTERVAL_SETP	= 20;	//每次重发的间隔毫秒
	vector<UINT*> tempList;
	while (m_bRun)
	{
		//1.接收新客户端
		//-----------------------------------------------------
		UINT tick = timeGetTime();
		while (pRecvData->NewClientList.HasItem())
		{
			UDPClientData *pc = pRecvData->NewClientList.GetItem();
			pc->RecvTick = tick;
			pc->SendTick = 0;	//sendtick 设置为0，继续发送心跳，防止UDP丢包
			clientList.push_back(pc);
		}
		//2.检查状态
		//-----------------------------------------------------
		tick = timeGetTime();
		FD_ZERO(pSet);
		for (UINT i = 0; i < clientList.size();)
		{
			UDPClientData *pc = clientList[i];
			bool bTimeOut = int(tick - pc->RecvTick) > m_InitData.Timeout;
			if (pc->Removed || bTimeOut || pc->ResendCount > RESEND_MAX_COUNT)
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
		tick = timeGetTime();
		for (uint i = 0; i < clientList.size(); ++i)
		{
			UDPClientData *pc = clientList[i];
			if (FD_ISSET(pc->Socket, pSet))
			{
				int nSize = recv(pc->Socket, pBuff, m_InitData.BuffSize, 0);
				if (nSize > 0)
				{
					RecvDataByUDP(pc, pBuff, nSize, tick);
					pc->RecvTick = tick;
				}
			}
			UINT interval = tick - pc->SendTick;
			if (interval < UDP_MIN_INTERVAL)
				continue;

			if (pc->ResendCount > RESEND_SAFE_COUNT)
			{
				UINT resendInterval = RESEND_INTERVAL_SETP << (pc->ResendCount - RESEND_SAFE_COUNT);
				if (interval < resendInterval)
					continue;
			}
			
			if (pc->IsGateway)
			{
				//网关要发送场景状态到客户端。
				pc->GWIsInScene = pc->IsInScene;
			}
			//1.是否有正在发送的数据
			if (pc->SendSize != 0)
			{
				if (tick - pc->SendCmdTick > UDP_RESEND_TICK)
				{
					++pc->ResendCount;
					SendCmdData(pc, true, tick, tempList);
				}
				else if (pc->bSendBackID)
				{
					SendCmdData(pc, true, tick, tempList);
				}
				continue;
			}
			else if (pc->SendList.HasItem())
			{
				SendCmdData(pc, false, tick, tempList);
			}
			else if (pc->bSendBackID)
			{
				SendBackID(pc, tick);
			}
			else if (pc->IsGateway)
			{
				if (int(tick - pc->SendTick) > halfTimeout)
				{
					SendBackID(pc, tick);//发送recvid，取代心跳
				}
			}
			else
			{
				bool bInScene = pc->IsInScene;
				int timeout = bInScene ? scenetick : halfTimeout;
				if (int(tick - pc->SendTick) > timeout)
				{
					SendBackID(pc, tick);//发送recvid，取代心跳
				}
			}
		}// end for
	SLEEP:
		
		Sleep(m_InitData.SleepTime);
	}
	free(pBuff);
	DeleteFDSet(pSet);
	::InterlockedIncrement(&m_ExitIndex);
}
bool NewUDPServer::Kick(ServerClientData *pClient, RemoveType rt)
{
	RemoveClient((UDPClientData*)pClient, rt);
	return true;
}
void NewUDPServer::Shutdown()
{
	m_bRun = false;
	//PostQueuedCompletionStatus(m_Handle, 0, NULL, NULL);

	int count = m_InitData.RecvThreadNum + 1;
	while (m_ExitIndex != count)
		Sleep(100);
}
void NewUDPServer::SetCmdHandler(INetHandler *pHandler)
{
	m_pHandler = pHandler;
}
bool NewUDPServer::Send(ServerClientData *pClient, NetCmd *pCmd)
{
	UDPClientData *pc = (UDPClientData*)pClient;
	if (pc->SendList.HasSpace() == false)
	{
		pc->Removed = true;
		pc->RemoveCode = REMOVE_CMD_SEND_OVERFLOW;
		return false;
	}
	if (pCmd->GetCmdSize() > UDP_MAX_SEND_SIZE - 16)
	{
		Log("发送的命令太大:%d", pCmd->GetCmdSize());
		return false;
	}
	UINT *p = (UINT*)malloc(pCmd->GetCmdSize() + sizeof(UINT)* 2);
	memcpy(p + 2, pCmd, pCmd->GetCmdSize());
	pc->SendList.AddItem((NetCmd*)p);
	return true;
}
bool NewUDPServer::AddNewClient(void *pData)
{
	AcceptUDPClient *data = (AcceptUDPClient*)pData;
	USHORT recvIdx;
	GetRecvIndex(recvIdx);
	if (recvIdx == USHRT_MAX)
	{
		Log("没有适合的空间加入新玩家:%d", recvIdx);
		return false;
	}
	//成功
	UINT size = sizeof(UDPClientData)+UDP_BUFF_SIZE;
	UDPClientData *pc = new(malloc(size))UDPClientData(m_InitData.MaxSendCmdCount, m_InitData.MaxRecvCmdCount);
	pc->Removed		= false;
	pc->RemoveCode	= REMOVE_NONE;
	pc->IsInScene	= false;
	pc->Socket		= data->NewSocket;
	if (data->NewServerIP == 0)
	{
		pc->IsGateway	= false;
		pc->IP			= data->ClientAddr.sin_addr.S_un.S_addr;
		pc->Port		= ntohs(data->ClientAddr.sin_port);
	}
	else
	{
		pc->IsGateway	= true;
		pc->IP			= data->NewServerIP;
		pc->Port		= data->NewServerPort;
	}
	pc->OutsideExtraData = NULL;
	pc->RefCount	= 2;
	pc->SendID		= 1;
	pc->RecvID		= 1;
	pc->SendTick	= 0;
	pc->SendCmdTick = 0;
	pc->ResendCount = 0;
	pc->bSendBackID = false;
	pc->SendSize	= 0;
	pc->pSendBuff	= NULL;
	pc->SendError	= 0;
	m_pHandler->NewClient(m_InitData.ServerID, pc, NULL, 0);

	UDPRecvThreadData *precv = m_RecvThreadData[recvIdx];
	//::InterlockedIncrement(&m_OnlineNum);
	::InterlockedIncrement(&precv->OnlineNum);
	precv->NewClientList.AddItem(pc);
	return true;
}
void NewUDPServer::_ThreadAccept()
{
	SOCKADDR_IN addr;
	INT addrSize = sizeof(addr);
	UINT idx = 0;
	vector<AcceptUDPClient> newList;
	CreateSocketData csd;
	UINT data[4];
	UINT CONNECT_OK			= SERVER_CONNECT_MAGIC;
	const UINT timeout		= 4000;
	const UINT MaxSendCount = 5;
	UINT hearbeat			= HEARBEAT_ID;
	char buff[512];
	while (m_bRun)
	{
		UINT tick = timeGetTime();
		for (UINT i = 0; i < m_InitData.MaxAcceptNumPerFrame; ++i)
		{
			
			SOCKET s = accept(m_Socket, (sockaddr*)&addr, &addrSize);
			if (s == INVALID_SOCKET)
				break;
			InitSocket(s, m_InitData.SocketSendSize, m_InitData.SocketRecvSize, true);
			if (!CreateSocket(CST_UDP | CST_BIND, 0, m_InitData.SocketSendSize, m_InitData.SocketRecvSize, csd))
			{
				Log("创建新的Socket失败, LastErrCode:%u", WSAGetLastError());
				continue;
			}
		
			AcceptUDPClient ndc;
			ndc.ClientAddr	= addr;
			ndc.ClientSocket = s;
			ndc.NewPort		= csd.Port;
			ndc.NewSocket	= csd.Socket;
			ndc.Tick		= tick;
			ndc.Rand1		= (RandUInt());
			ndc.Rand2		= (RandUInt());
			ndc.SendCount	= 1;
			ndc.Removed		= false;
			ndc.SendTick	= tick;
			data[0]			= CONNECT_OK;
			data[1]			= ndc.Rand1;
			data[2]			= ndc.Rand2;
			data[3]			= ndc.NewPort;

			InnerGetIPString(addr, buff);
			//Log("Socket:%d, Port:%d, clientIP:%s", ndc.NewSocket, ndc.NewPort ,buff);
			if (send(s, (char*)data, sizeof(data), 0) != sizeof(data))
				ndc.SendType = SEND_NONE;
			else
				ndc.SendType = SEND_WAIT_NEWPORT;
			newList.push_back(ndc);
		}

		for (UINT i = 0; i < newList.size(); )
		{
			tick = timeGetTime();
			AcceptUDPClient &nc = newList[i];
			if (tick - nc.Tick > timeout)
			{
				InnerGetIPString(nc.ClientAddr, buff);
				nc.Removed = true;
				const char *pc = NULL;
				if (nc.SendType == SEND_NONE)
					pc = "SEND_NONE";
				else if (nc.SendType == SEND_WAIT_HEARBEAT)
					pc = "SEND_WAIT_HEARBEAT";
				else if (nc.SendType == SEND_WAIT_NEWPORT)
					pc = "SEND_WAIT_NEWPORT";
				else
					pc = "SEND_UNKNOWN";
				Log("UDP验证：超时:%s, 状态:%s, Timeout:%u", buff, pc, tick - nc.Tick);
			}
			if (nc.Removed)
			{
				//Log("Removed:%d, count:%d", i, newList.size());
				if (nc.ClientSocket != NULL)
					closesocket(nc.ClientSocket);
				if (nc.NewSocket != NULL)
					closesocket(nc.NewSocket);
				ListRemoveAt(newList, i);
				continue;
			}
			++i;
			if (nc.SendType == SEND_NONE)
			{
				if (tick - nc.SendTick > UDP_VALIDATION_TICK)
				{
					nc.SendTick = tick;
					data[0] = CONNECT_OK;
					data[1] = nc.Rand1;
					data[2] = nc.Rand2;
					data[3] = nc.NewPort;
					int ret = ::send(nc.ClientSocket, (char*)data, sizeof(data), 0);
					if (ret == sizeof(data))
					{
						nc.SendType = SEND_WAIT_NEWPORT;
						nc.SendCount = 0;
					}
					else
					{
						if (++nc.SendCount > MaxSendCount)
						{
							InnerGetIPString(nc.ClientAddr, buff);
							Log("UDP验证:发送新端口超量，IP:%s, LastErrCode:%u", buff, WSAGetLastError());
							nc.Removed = true;
						}
					}
				}
			}
			else if (nc.SendType == SEND_WAIT_NEWPORT)
			{
				//等待接收确认数据
				int nRecv = recvfrom(nc.NewSocket, buff, sizeof(buff), 0, (sockaddr*)&addr, &addrSize);
				if (nRecv >= 12) //12个字节的验证 + 6字节的服务器IP
				{
					UINT *pRecvID = (UINT*)buff;
					if (*pRecvID == UDP_VALIDATION_ID && *(pRecvID + 1) == (nc.Rand1 | 0xc0000000) && *(pRecvID + 2) == (nc.Rand2 | 0xc0000000))
					{
						int ret = connect(nc.NewSocket, (sockaddr*)&addr, addrSize);
						if (ret != 0)
						{
							InnerGetIPString(nc.ClientAddr, buff);
							Log("UDP验证:连接到客户端失败, IP:%s, LastErrCode:%d", buff, WSAGetLastError());
							nc.Removed = true;
						}
						else
						{
							nc.ClientAddr = addr;
							//nc.Tick = tick;
							nc.SendType = SEND_WAIT_HEARBEAT;
							nc.SendTick = tick;
							if (nRecv > 12)
							{
								nc.NewServerIP = *(pRecvID + 3);
								nc.NewServerPort = *(ushort*)(pRecvID + 4);
							}
							else
							{
								nc.NewServerIP = 0;
								nc.NewServerPort = 0;
							}
							//Log("客户端NewServer:%u, Port:%d, Ret:%d", nc.NewServerIP, nc.NewServerPort, nRecv);
							//发送一个心跳给客户端
							int ret = send(nc.NewSocket, (char*)&hearbeat, 4, 0);
							if (ret == sizeof(hearbeat))
								nc.SendCount = 1;
							else
								nc.SendCount = 0;
						}
					}
					else
					{
						nc.Removed = true;
						char xxx[50];
						InnerGetIPString(nc.ClientAddr, xxx);
						Log("UDP验证%u, Rand不正确:%u, %u vs %u, %u, IP:%s", *pRecvID, *(pRecvID + 1), *(pRecvID + 2), nc.Rand1 | 0xc0000000, nc.Rand2 | 0xc0000000, xxx);
					}
				}
				else if (nRecv > 0)
				{
					nc.Removed = true;
					InnerGetIPString(nc.ClientAddr, buff);
					Log("UDP验证：接收长度不正确:%d, IP:%s", nRecv, buff);
				}
			}
			else  if (nc.SendType == SEND_WAIT_HEARBEAT)
			{
				//等待接收心跳
				int ret = recv(nc.NewSocket, buff, sizeof(buff), 0);
				if (ret == 4 && *((UINT*)buff) == HEARBEAT_ID)
				{
					//成功
					if (AddNewClient(&nc))
					{
						//Log("UDP验证成功, Timeout:%d", tick - nc.Tick);
						nc.NewSocket = NULL;
						nc.Removed = true;
					}
					else
					{
						Log("UDP验证加入线程失败.");
						nc.Removed = true;
					}
				}
				else
				{
					if (tick - nc.SendTick > UDP_VALIDATION_TICK)
					{
						nc.SendTick = tick;
						int ret = send(nc.NewSocket, (char*)&hearbeat, 4, 0);
						if (ret != 4)
						{
							InnerGetIPString(nc.ClientAddr, buff);
							Log("UDP验证:发送心跳失败，IP:%s, SendRet:%d, LastErrCode:%u", buff, ret, WSAGetLastError());
						}
					}
				}
			}
			else
			{
				Log("UDP验证:未知的状态:%d", nc.SendType);
				nc.Removed = true;
			}
		}// end for
		Sleep(m_InitData.SleepTime);
	}
	::InterlockedIncrement(&m_ExitIndex);
}
UINT NewUDPServer::JoinNum()const
{
	return m_OnlineNum;
}
void NewUDPServer::SwitchAccept(bool bEnable)
{
	if (m_bAccept != bEnable)
	{
		m_bAccept = bEnable;
	}
}
void NewUDPServer::RemoveClient(UDPClientData *pc, RemoveType rt)
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
		pc->~UDPClientData();
		free(pc);
		//::InterlockedDecrement(&m_OnlineNum);
	}
}