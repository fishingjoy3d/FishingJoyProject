#include "stdafx.h"
#include "UDPServer.h"
#include "Cmd_Fish.h"
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

const int UDP_MIN_CMD_SIZE = sizeof(UINT)+sizeof(UINT)+sizeof(NetCmd);
const UINT LOGON_SUCCESS_ID = (Main_Logon << 8) | LC_AccountOnlyID;
const UINT GAME_SUCCESS_ID = (Main_Logon << 8) | LC_AccountOnlyIDSuccess;
static UINT WINAPI ThreadAccept(void *p)
{
	((NewGateServer*)p)->_ThreadAccept();
	return 0;
}
static UINT WINAPI ThreadRecv(void *p)
{
	((NewGateServer*)p)->_ThreadRecv();
	return 0;
}
static UINT WINAPI LogonThreadRecv(void *p)
{
	((NewGateServer*)p)->_ThreadLogonRecv();
	return 0;
}
static UINT WINAPI LogonThreadAccept(void *p)
{
	((NewGateServer*)p)->_ThreadLogonAccept();
	return 0;
}
static UINT WINAPI OperationThreadRecv(void *p)
{
	((NewGateServer*)p)->_ThreadOperationRecv();
	return 0;
}
static UINT WINAPI OperationThreadAccept(void *p)
{
	((NewGateServer*)p)->_ThreadOperationAccept();
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
	UINT			ServerIP;
	USHORT			ServerPort;
	UINT			Rand1;
	UINT			Rand2;
	BYTE			SendType;
	BYTE			SendCount;
	bool			Removed;
};
NewGateServer::NewGateServer() :m_OperationThreadData(256)
{

}
NewGateServer::~NewGateServer()
{

}
static void InnerGetIPString(SOCKADDR_IN &addr, char *buff)
{
	UINT ip = addr.sin_addr.S_un.S_addr;
	ushort port = ntohs(addr.sin_port);
	GetIPString(ip, port, buff);
}
bool NewGateServer::Init(const GWInitData &data, bool btcp)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		Log("WSAStartup Failed.");
		return false;
	}
	m_LogonIndex = 0;
	m_bAccept = true;
	memset(m_RecvThreadData, 0, sizeof(m_RecvThreadData));
	m_ExitIndex = 0;
	m_OnlineNum = 0;
	m_RecvIndex = 0;
	m_bRun = true;
	memcpy(&m_InitData, &data, sizeof(data));
	m_InitData.LogonThreadNum = min(m_InitData.LogonThreadNum, MAX_THREAD_NUM);
	m_InitData.RecvThreadNum = min(m_InitData.RecvThreadNum, MAX_THREAD_NUM);
	inet_pton(AF_INET, m_InitData.LocalServerIP, &m_LocalIP);

	CreateSocketData csd;
	bool bret = CreateSocket(CST_TCP | CST_BIND, m_InitData.Port, data.SocketRecvSize, data.SocketSendSize, csd);
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
	//Logon
	//-------------------------------------------------
	nRet = CreateSocket(CST_BIND | CST_TCP, m_InitData.LogonClientPort, 4096, 4096, csd);
	if (nRet == false)
	{
		Log("CreateSocket Failed:%d.", WSAGetLastError());
		return false;
	}
	m_LogonSocket = csd.Socket;

	nRet = ::listen(m_LogonSocket, m_InitData.ListenCount);
	if (nRet != 0)
	{
		Log("listen Failed:%d.", WSAGetLastError());
		return false;
	}
	//Operation
	//-------------------------------------------------
	nRet = CreateSocket(CST_BIND | CST_TCP, m_InitData.OperationClientPort, 4096, 4096, csd);
	if (nRet == false)
	{
		Log("CreateSocket Failed:%d.", WSAGetLastError());
		return false;
	}
	m_OperationSocket = csd.Socket;

	nRet = ::listen(m_OperationSocket, m_InitData.ListenCount);
	if (nRet != 0)
	{
		Log("listen Failed:%d.", WSAGetLastError());
		return false;
	}

	//创建线程数据
	for (int i = 0; i < data.RecvThreadNum; ++i)
	{
		m_RecvThreadData[i] = new GWRecvThreadData(THREAD_ACCEPT_COUNT);
		m_RecvThreadData[i]->OnlineNum = 0;
	}
	//创建线程
	::_beginthreadex(0, 0, ThreadAccept, this, 0, 0);
	for (int i = 0; i < data.RecvThreadNum; ++i)
	{
		::_beginthreadex(0, 0, ThreadRecv, this, 0, 0);
	}

	::_beginthreadex(0, 0, LogonThreadAccept, this, 0, 0);

	//创建Logon线程
	//=====================================================
	for (UINT i = 0; i < data.LogonThreadNum; ++i)
	{
		m_LogonThreadData[i] = new GWLogonThreadData(THREAD_ACCEPT_COUNT);
		m_LogonThreadData[i]->OnlineNum = 0;
	}
	for (UINT i = 0; i < data.LogonThreadNum; ++i)
	{
		::_beginthreadex(0, 0, LogonThreadRecv, this, 0, 0);
	}

	//创建Operation线程
	//=====================================================
	::_beginthreadex(0, 0, OperationThreadAccept, this, 0, 0);
	::_beginthreadex(0, 0, OperationThreadRecv, this, 0, 0);

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
void NewGateServer::GetRecvIndex(ushort &recvIdx)
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
bool NewGateServer::RecvDataByUDPServer(GWClientData *pc, char *Buff, int nSize, UINT tick)
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
		}
		else if (IS_ANSWER(recvID))	//收到回复 
		{
			if (IS_GW_ANSWER(recvID))
			{
				/*if (pc->InnerIsInScene == false)
				{
					Log("<<进入场景>>");
				}*/
				pc->InnerIsInScene = true;
				RESET_GW_ANSWER(recvID);
			}
			else
			{
				/*if (pc->InnerIsInScene)
				{
					Log("<<离开场景>>");
				}*/
				pc->InnerIsInScene = false;
				RESET_ANSWER(recvID);
			}
			//Log("Recv Answer:%d, sendSize:%d", recvID, pc->SendSize);
			if (recvID == pc->ServerSendID && pc->pServerSendBuff != null)
			{
				free(pc->pServerSendBuff);
				pc->pServerSendBuff = NULL;
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
			else if (recvID == pc->ServerRecvID + 1)
			{
				//对方第一次发送的命令
				++pc->ServerRecvID;
				UINT *pRecvCmd = (UINT*)malloc(pcmd->GetCmdSize() + sizeof(UINT)* 2);
				memcpy(pRecvCmd + 2, pcmd, pcmd->GetCmdSize());
				if (pc->RecvServerList.HasSpace())
					pc->RecvServerList.AddItem(pRecvCmd);
				else
				{
					Log("UDP Server接收空间已满");
					pc->RemoveCode = REMOVE_CMD_RECV_OVERFLOW;
					pc->Removed = true;
				}
			}
			else
			{
				//Log("Recv OldID:%d, CurID:%d", recvID, pc->RecvID);
			}
			pc->bServerSendBackID = true;
			pBuff += pcmd->GetCmdSize();
		}
	}//end while
	return true;
}

bool NewGateServer::RecvDataByUDP(GWClientData *pc, char *Buff, int nSize, UINT tick)
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
				UINT *pRecvCmd = (UINT*)malloc(pcmd->GetCmdSize() + sizeof(UINT)* 2);
				memcpy(pRecvCmd + 2, pcmd, pcmd->GetCmdSize());
				if (pcmd->GetCmdType() == GAME_SUCCESS_ID)
				{
					//替换运营服务器IP
					LC_Cmd_AccountOnlyIDSuccess * pGameCmd = (LC_Cmd_AccountOnlyIDSuccess*)(pRecvCmd + 2);
					pGameCmd->OperateIp = m_LocalIP;
				}
				if (pc->RecvClientList.HasSpace())
					pc->RecvClientList.AddItem(pRecvCmd);
				else
				{
					Log("UDP Client接收空间已满");
					pc->RemoveCode = REMOVE_CMD_RECV_OVERFLOW;
					pc->Removed = true;
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
void SendBackID(GWClientData *pc, UINT tick)
{
	UINT backID = SET_ANSWER(pc->RecvID);
	pc->SendTick = tick;
	int ret = send(pc->Socket, (char*)&backID, 4, 0);
	if (ret == 4)
	{
		pc->bSendBackID = false;
	}
}
void SendCmdData(GWClientData *pc, bool bresend, UINT tick, vector<UINT*> &tempList)
{
	if (pc->SendSize == 0)
		pc->SendSize = 4;

	//1.拼包,计算包大小
	//----------------------------------
	UINT size = pc->SendSize;
	tempList.clear();
	while (size < UDP_MTU && pc->RecvServerList.HasItem())
	{
		UINT  *curData = pc->RecvServerList.GetItemNoRemove();
		NetCmd *pcmd = (NetCmd*)(curData + 2);
		UINT cmdSize = pcmd->GetCmdSize() + 4;
		if (cmdSize + size < UDP_BUFF_SIZE)
		{
			tempList.push_back(curData);
			pc->RecvServerList.RemoveTopItem();
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
	*pc->pSendBuff = SET_ANSWER(pc->RecvID);
	int ret = send(pc->Socket, (char*)pc->pSendBuff, pc->SendSize, 0);
	pc->SendTick = tick;
	if (static_cast<UINT>(ret) == pc->SendSize)
	{
		pc->SendCmdTick = tick;
		pc->bSendBackID = false;
	}
	else
		pc->SendCmdTick = 0;	//下次立即发送
	return;
}
inline void IPToString(UINT ip, CHAR str[20])
{
	sprintf_s(str, 20, "%d.%d.%d.%d", BYTE(ip), BYTE(ip >> 8), BYTE(ip >> 16), BYTE(ip >> 24));
}
bool NewGateServer::InitClientUDP(GWClientData *pc, bool bTCP)
{
	CreateSocketData csd;
	if (!CreateSocket(bTCP ? CST_TCP : CST_UDP, 0, m_InitData.SocketRecvSize, m_InitData.SocketSendSize, csd))
	{
		Log("创建服务器Socket失败.");
		return false;
	}
	pc->ServerSocket = csd.Socket;
	SOCKADDR_IN addr;
	int addrSize = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	char ipstr[100];
	IPToString(pc->ConnectServerIP, ipstr);
	inet_pton(AF_INET, ipstr, &addr.sin_addr.s_addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(pc->ConnectServerPort);
	int ret = connect(pc->ServerSocket, (sockaddr*)&addr, addrSize);
	return ret == 0 || (WSAGetLastError() == 10035);
}
//连接UDP服务器超时
const UINT CONNECT_TIMEOUT = 3000;
const UINT HEARBEAT = HEARBEAT_ID;
void NewGateServer::CheckConnectList(GWRecvThreadData *pRecvData, vector<GWClientData*> &tempClientList, vector<GWClientData*> &clientList, char *pBuff)
{
	uint tick = timeGetTime();
	int ret = 0;
	for (UINT i = 0; i < tempClientList.size();)
	{
		GWClientData *pc = tempClientList[i];
		if (tick - pc->RecvTick > CONNECT_TIMEOUT)
		{
			Log("GW UDP服务器端验证超时!");
			RemoveClient(pc, REMOVE_TIMEOUT);
			ListRemoveAt(tempClientList, i);
			::InterlockedDecrement(&pRecvData->OnlineNum);
			continue;
		}
		if (tick - pc->SendTick > 500)
		{
			send(pc->Socket, (char*)&HEARBEAT, 4, 0);
			pc->SendTick = tick;
		}
		switch (pc->State)
		{
		case GWS_CONNECTING:
			ret = recv(pc->ServerSocket, pBuff, m_InitData.BuffSize, 0);
			if (ret == 16 && *(UINT*)pBuff == SERVER_CONNECT_MAGIC)
			{
				closesocket(pc->ServerSocket);
				pc->ConnectServerPort = *((UINT*)(pBuff + 12));
				if (InitClientUDP(pc, false) == false)
				{
					Log("创建UDP失败.");
					pc->~GWClientData();
					free(pc);
					continue;
				}
				UINT* pSendData = (UINT*)(pBuff + 0);
				pc->State = GWS_SEND_VALID_DATA;
				pc->ValidBuff[0] = UDP_VALIDATION_ID;
				pc->ValidBuff[1] = *(pSendData + 1) | 0xc0000000;
				pc->ValidBuff[2] = *(pSendData + 2) | 0xc0000000;
				pc->ValidBuff[3] = pc->IP;
				pc->ValidBuff[4] = pc->Port;
				pc->SendCmdTick = tick;
				//Log("Rand1:%u, Rand2:%u, IP:%u", pc->ValidBuff[1], pc->ValidBuff[2], pc->ValidBuff[3]);
				send(pc->ServerSocket, (char*)pc->ValidBuff, 18, 0);
			}
			break;
		case GWS_SEND_VALID_DATA:
			ret = recv(pc->ServerSocket, pBuff, m_InitData.BuffSize, 0);
			if (ret == 4 && *(UINT*)pBuff == HEARBEAT_ID)
			{
				//发送三次心跳
				send(pc->ServerSocket, (char*)&HEARBEAT, 4, 0);
				pc->State = GWS_SEND_HEARBEAT;
				pc->ResendCount = 1;
				pc->SendCmdTick = tick;
			}
			else if (tick - pc->SendCmdTick > 500)
			{
				//Log("Send Validation Data.");
				pc->SendCmdTick = tick;
				send(pc->ServerSocket, (char*)pc->ValidBuff, 18, 0);
			}
			break;
		case GWS_SEND_HEARBEAT:
			if (tick - pc->SendCmdTick > 50)
			{
				send(pc->ServerSocket, (char*)&HEARBEAT, 4, 0);
				if (++pc->ResendCount >= 3)
				{
					Log("GW UDP服务器验证成功, tick:%du", tick - pc->RecvTick);
					pc->ResendCount = 0;
					pc->SendCmdTick = 0;
					pc->RecvTick = tick;
					pc->SendTick = 0;
					pc->ServerSendTick = 0;
					pc->pServerSendBuff = NULL;
					pc->ServerRecvTick = tick;
					clientList.push_back(pc);
					ListRemoveAt(tempClientList, i);
					continue;
				}
				else
					pc->SendCmdTick = tick;
			}
			break;
		default:
			Log("未知的UDPClient状态:%d", pc->State);
			break;
		}
		++i;
	}
}
void NewGateServer::_ThreadRecv()
{
	int idx = ::InterlockedIncrement(&m_RecvIndex) - 1;
	vector<GWClientData*> clientList;
	vector<GWClientData*> tempClientList;
	GWRecvThreadData *pRecvData = m_RecvThreadData[idx];
	fd_set  *pSet = CreateFDSet();
	timeval time = { 0, 0 };
	int scenetick = m_InitData.SceneHearbeatTick;
	int	halfTimeout = min(HEARBEAT_TICK, m_InitData.Timeout >> 1);
	UINT ping = PING_ID;
	char *pBuff = (char*)malloc(m_InitData.BuffSize);
	const int RESEND_SAFE_COUNT = 5;		//安全重发次数
	const int RESEND_MAX_COUNT = 15;		//最大重发次数
	const int RESEND_INTERVAL_SETP = 20;	//每次重发的间隔毫秒
	vector<UINT*> tempList;

	while (m_bRun)
	{
		//1.接收新客户端
		//-----------------------------------------------------
		UINT tick = timeGetTime();
		while (pRecvData->NewClientList.HasItem())
		{
			GWClientData *pc = pRecvData->NewClientList.GetItem();
			if (InitClientUDP(pc, true) == false)
			{
				pc->~GWClientData();
				free(pc);
			}
			else
			{
				pc->State = GWS_CONNECTING;
				pc->RecvTick = tick;
				pc->SendTick = 0;	//sendtick 设置为0，继续发送心跳，防止UDP丢包
				tempClientList.push_back(pc);
			}
		}
		//2.检查连接状态
		//-----------------------------------------------------
		CheckConnectList(pRecvData, tempClientList, clientList, pBuff);

		//3.检查状态
		//-----------------------------------------------------
		tick = timeGetTime();
		FD_ZERO(pSet);
		for (UINT i = 0; i < clientList.size();)
		{
			GWClientData *pc = clientList[i];
			bool bTimeOut = int(tick - pc->RecvTick) > m_InitData.Timeout;
			bool bTimeOut2 = int(tick - pc->ServerRecvTick) > m_InitData.Timeout;
			if (pc->Removed || bTimeOut || bTimeOut2 || pc->ResendCount > RESEND_MAX_COUNT)
			{
				Log("UDP 客户端断开");
				RemoveClient(pc, REMOVE_TIMEOUT);
				ListRemoveAt(clientList, i);
				::InterlockedDecrement(&pRecvData->OnlineNum);
				continue;
			}
			FD_ADD(pc->Socket, pSet);
			FD_ADD(pc->ServerSocket, pSet);
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
			GWClientData *pc = clientList[i];
			//服务器端
			//===========================================
			while (1)
			{
				if (FD_ISSET(pc->ServerSocket, pSet))
				{
					int nSize = recv(pc->ServerSocket, pBuff, m_InitData.BuffSize, 0);
					if (nSize > 0)
					{
						RecvDataByUDPServer(pc, pBuff, nSize, tick);
						pc->ServerRecvTick = tick;
					}
				}
				UINT interval = tick - pc->ServerSendTick;
				if (interval < UDP_MIN_INTERVAL)
					break;

				if (pc->ServerResendCount > RESEND_SAFE_COUNT)
				{
					UINT resendInterval = RESEND_INTERVAL_SETP << (pc->ServerResendCount - RESEND_SAFE_COUNT);
					if (interval < resendInterval)
						break;
				}
				//1.是否有正在发送的数据
				if (pc->pServerSendBuff != null)
				{
					if (tick - pc->ServerSendCmdTick > UDP_RESEND_TICK || pc->bServerSendBackID)
					{
						*pc->pServerSendBuff = SET_ANSWER(pc->ServerRecvID);
						pc->ServerSendTick = tick;
						if (send(pc->ServerSocket, (char*)pc->pServerSendBuff, pc->ServerSendSize, 0) == pc->ServerSendSize)
							pc->bServerSendBackID = false;
					}
				}
				else if (pc->RecvClientList.HasItem())
				{
					pc->pServerSendBuff = pc->RecvClientList.GetItem();
					pc->pServerSendBuff[0] = SET_ANSWER(pc->ServerRecvID);
					pc->pServerSendBuff[1] = ++pc->ServerSendID;
					pc->ServerSendSize = ((NetCmd*)(pc->pServerSendBuff + 2))->GetCmdSize() + sizeof(UINT)* 2;
					pc->ServerSendTick = tick;
					if (send(pc->ServerSocket, (char*)pc->pServerSendBuff, pc->ServerSendSize, 0) == pc->ServerSendSize)
					{
						pc->bServerSendBackID = false;
						pc->ServerSendCmdTick = tick;
					}
					else
						pc->ServerSendCmdTick = 0;
				}
				else if (pc->bServerSendBackID || (tick - pc->ServerSendTick > HEARBEAT_TICK))
				{
					UINT backID = SET_ANSWER(pc->ServerRecvID);
					pc->ServerSendTick = tick;
					int ret = send(pc->ServerSocket, (char*)&backID, 4, 0);
					if (ret == 4)
					{
						pc->bServerSendBackID = false;
					}
				}
				break;
			}
			//客户端
			//===========================================
			while (1)
			{
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
					break;

				if (pc->ResendCount > RESEND_SAFE_COUNT)
				{
					UINT resendInterval = RESEND_INTERVAL_SETP << (pc->ResendCount - RESEND_SAFE_COUNT);
					if (interval < resendInterval)
						break;
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
				}
				else if (pc->RecvServerList.HasItem())
				{
					SendCmdData(pc, false, tick, tempList);
				}
				else if (pc->bSendBackID)
				{
					SendBackID(pc, tick);
				}
				else
				{
					int timeout = pc->InnerIsInScene ? scenetick : halfTimeout;
					if (int(tick - pc->SendTick) > timeout)
					{
						SendBackID(pc, tick);//发送recvid，取代心跳
					}
				}
				break;
			}
		}// end for

	SLEEP:
		Sleep(m_InitData.SleepTime);
	}
	free(pBuff);
	DeleteFDSet(pSet);
	::InterlockedIncrement(&m_ExitIndex);
}
bool NewGateServer::Kick(ServerClientData *pClient, RemoveType rt)
{
	RemoveClient((GWClientData*)pClient, rt);
	return true;
}
void NewGateServer::Shutdown()
{
	m_bRun = false;
	//PostQueuedCompletionStatus(m_Handle, 0, NULL, NULL);

	int count = m_InitData.RecvThreadNum + 1;
	while (m_ExitIndex != count)
		Sleep(100);
}
void NewGateServer::SetCmdHandler(INetHandler *pHandler)
{
	m_pHandler = pHandler;
}
bool NewGateServer::Send(ServerClientData *pClient, NetCmd *pCmd)
{

	return true;
}
bool NewGateServer::AddNewClient(void *pData)
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
	UINT size = sizeof(GWClientData)+UDP_BUFF_SIZE;
	GWClientData *pc = new(malloc(size))GWClientData(8, m_InitData.MaxRecvCmdCount, m_InitData.MaxSendCmdCount);
	pc->Removed = false;
	pc->RemoveCode = REMOVE_NONE;
	pc->IsInScene = false;
	pc->Socket = data->NewSocket;
	pc->IP = data->ClientAddr.sin_addr.S_un.S_addr;
	pc->Port = ntohs(data->ClientAddr.sin_port);
	//char xx[100];
	//GetIPString(pc->IP, pc->Port, xx); 
	//Log("ClientIP:%s", xx);
	pc->ConnectServerIP = data->ServerIP;
	pc->ConnectServerPort = data->ServerPort;
	pc->OutsideExtraData = NULL;
	pc->SendID = 1;
	pc->RecvID = 1;
	pc->SendTick = 0;
	pc->SendCmdTick = 0;
	pc->ResendCount = 0;
	pc->bSendBackID = false;
	pc->SendSize = 0;
	pc->pSendBuff = NULL;
	pc->SendError = 0;
	//-------------------------------
	pc->ServerResendCount = 0;
	pc->ServerSendID = 1;
	pc->ServerSendTick = 0;
	pc->ServerSendCmdTick = 0;
	pc->ServerRecvID = 1;
	pc->ServerRecvTick = 0;
	pc->ServerSendSize = 0;
	pc->ServerSendError = 0;
	pc->bServerSendBackID = false;
	pc->pServerSendBuff = NULL;


	m_pHandler->NewClient(m_InitData.ServerID, pc, NULL, 0);

	GWRecvThreadData *precv = m_RecvThreadData[recvIdx];
	//::InterlockedIncrement(&m_OnlineNum);
	::InterlockedIncrement(&precv->OnlineNum);
	precv->NewClientList.AddItem(pc);
	return true;
}
void NewGateServer::_ThreadAccept()
{
	SOCKADDR_IN addr;
	INT addrSize = sizeof(addr);
	UINT idx = 0;
	vector<AcceptUDPClient> newList;
	CreateSocketData csd;
	UINT data[4];
	UINT CONNECT_OK = SERVER_CONNECT_MAGIC;
	const UINT timeout = 4000;
	const UINT MaxSendCount = 5;
	UINT hearbeat = HEARBEAT_ID;
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
			ndc.ClientAddr = addr;
			ndc.ClientSocket = s;
			ndc.NewPort = csd.Port;
			ndc.NewSocket = csd.Socket;
			ndc.Tick = tick;
			ndc.Rand1 = (RandUInt());
			ndc.Rand2 = (RandUInt());
			ndc.SendCount = 1;
			ndc.Removed = false;
			ndc.SendTick = tick;
			data[0] = CONNECT_OK;
			data[1] = ndc.Rand1;
			data[2] = ndc.Rand2;
			data[3] = ndc.NewPort;
			//InnerGetIPString(addr, buff);
			//Log("Socket:%d, Port:%d, clientIP:%s", ndc.NewSocket, ndc.NewPort ,buff);
			if (send(s, (char*)data, sizeof(data), 0) != sizeof(data))
				ndc.SendType = SEND_NONE;
			else
				ndc.SendType = SEND_WAIT_NEWPORT;
			newList.push_back(ndc);
		}

		for (UINT i = 0; i < newList.size();)
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
						nc.SendType  = SEND_WAIT_NEWPORT;
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
				//等待接收验证数据
				int ret = recvfrom(nc.NewSocket, buff, sizeof(buff), 0, (sockaddr*)&addr, &addrSize);
				if (ret == 18) //12个字节的验证 + 6字节的服务器IP
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
							//Log("验证成功.");
							nc.ClientAddr = addr;
							//nc.Tick = tick;
							nc.SendType = SEND_WAIT_HEARBEAT;
							nc.SendTick = tick;
							nc.ServerIP = *(pRecvID + 3);
							nc.ServerPort = *((USHORT*)(pRecvID + 4));
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
						Log("UDP验证:%u, Rand不正确:%u, %u vs %u, %u, IP:%s", *pRecvID, *(pRecvID + 1), *(pRecvID + 2), (nc.Rand1 | 0xc0000000), (nc.Rand2 | 0xc0000000), xxx);
					}
				}
				else if (ret > 0)
				{
					nc.Removed = true;
					InnerGetIPString(nc.ClientAddr, buff);
					Log("UDP验证：接收长度不正确:%d, IP:%s", ret, buff);
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
UINT NewGateServer::JoinNum()const
{
	return m_OnlineNum;
}
void NewGateServer::SwitchAccept(bool bEnable)
{
	if (m_bAccept != bEnable)
	{
		m_bAccept = bEnable;
	}
}
void NewGateServer::RemoveClient(GWClientData *pc, RemoveType rt)
{
	pc->Removed = true;
	if (pc->RemoveCode == REMOVE_NONE)
		pc->RemoveCode = rt;
	closesocket(pc->ServerSocket);
	closesocket(pc->Socket);
	m_pHandler->Disconnect(m_InitData.ServerID, pc, pc->RemoveCode);
	while (pc->RecvServerList.HasItem())
	{
		free(pc->RecvServerList.GetItem());
	}
	while (pc->RecvClientList.HasItem())
	{
		free(pc->RecvClientList.GetItem());
	}
	pc->~GWClientData();
	free(pc);
	//::InterlockedDecrement(&m_OnlineNum);
}
void NewGateServer::_ThreadLogonAccept()
{
	SOCKADDR_IN addr;
	INT addrSize = sizeof(addr);
	UINT idx = 0;
	char buff[128];
	SOCKET logonSocket = m_LogonSocket;
	while (m_bRun)
	{
		for (UINT i = 0; i < m_InitData.LogonAcceptPerFrame; ++i)
		{
			SOCKET s = accept(logonSocket, (sockaddr*)&addr, &addrSize);
			if (s == INVALID_SOCKET)
				break;

			InitSocket(s, m_InitData.SocketSendSize, m_InitData.SocketRecvSize, true);
			UINT n = ++idx % m_InitData.LogonThreadNum;

			if (m_LogonThreadData[n]->NewClientList.HasSpace() == false)
			{
				closesocket(s);
				Log("TCP验证：加入登录服务器失败，空间不够.");
			}
			else
			{
				GWNewLogonClient *pc = new GWNewLogonClient(m_InitData.BuffSize);
				pc->ClientSocket = s;
				InnerGetIPString(addr, buff);
				Log("TCP验证:新的登录客户端加入:%s", buff);
				m_LogonThreadData[n]->NewClientList.AddItem(pc);
			}
		}
		Sleep(m_InitData.SleepTime);
	}
	::InterlockedIncrement(&m_ExitIndex);
}

bool NewGateServer::TCPRecvServerCmd(GWNewLogonClient *pc)
{
	while (pc->ServerRecvSize >= sizeof(UINT))
	{
		BYTE *pBuff = (BYTE*)(pc->pServerBuff + pc->ServerRecvOffset);
		UINT recvID = *((UINT*)(pBuff));
		if (recvID == HEARBEAT_ID || recvID == PING_ID)
		{
			pc->ServerRecvOffset += sizeof(UINT);
			pc->ServerRecvSize -= sizeof(UINT);
		}
		else if (pc->ServerRecvSize >= sizeof(NetCmd))
		{
			NetCmd *pCmdRecv = ((NetCmd*)pBuff);
			UINT cmdSize = ((NetCmd*)pCmdRecv)->GetCmdSize();
			if (cmdSize > m_InitData.BuffSize)
			{
				Log(L"RecvServerSize Error:%d", cmdSize);
				return false;
			}
			if (pc->ServerRecvSize >= cmdSize)
			{
				NetCmd *pCmd = (NetCmd*)pBuff;
				if (pCmd->GetCmdType() != m_InitData.CmdHearbeat)
				{
					pCmd = CreateCmd(ushort(cmdSize), pBuff);
					if (pCmd->GetCmdType() == LOGON_SUCCESS_ID)
					{
						//登录成功，修改GAME ServerIP;
						LC_Cmd_AccountOnlyID *pLogonCmd = (LC_Cmd_AccountOnlyID*)pCmd;
						pLogonCmd->GameIp	= pLogonCmd->GateIp;
						pLogonCmd->GamePort = pLogonCmd->GatePort;
						pLogonCmd->GateIp	= m_LocalIP;
						pLogonCmd->GatePort = m_InitData.Port;
					}
					pc->ServerCmdList.push_back(pCmd);
				}
				pc->ServerRecvOffset += cmdSize;
				pc->ServerRecvSize -= cmdSize;
			}
			else
				break;
		}
		else
			break;
	}
	UINT freeBuffSize = m_InitData.BuffSize - (pc->ServerRecvOffset + pc->ServerRecvSize);
	if (freeBuffSize < 128)
	{
		memmove(pc->pServerBuff, pc->pServerBuff + pc->ServerRecvOffset, pc->ServerRecvSize);
		pc->ServerRecvOffset = 0;
	}
	return true;
}

void NewGateServer::_ThreadLogonRecv()
{
	UINT idx = ::InterlockedIncrement(&m_LogonIndex) - 1;
	GWLogonThreadData &gtd = *(m_LogonThreadData[idx]);
	vector<GWNewLogonClient*> client;
	CreateSocketData csd;
	FD_SET *pset = CreateFDSet();
	UINT timeout = m_InitData.LogonTimeout;
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	inet_pton(AF_INET, m_InitData.LogonServerIP, &addr.sin_addr.s_addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_InitData.LogonServerPort);
	int addrSize = sizeof(addr);
	timeval time = { 0, 0 };
	UINT halftime = 1000;
	UINT hearbeat = HEARBEAT_ID;
	
	while (m_bRun)
	{
		UINT tick = timeGetTime();
		while (gtd.NewClientList.HasItem())
		{
			GWNewLogonClient *gc = gtd.NewClientList.GetItem();
			bool bret = CreateSocket(CST_TCP, 0, m_InitData.SocketRecvSize, m_InitData.SocketSendSize, csd);
			if (bret == false)
			{
				Log("创建TCPSocket 失败");
				closesocket(gc->ClientSocket);
				delete(gc);
			}
			else
			{
				gc->ServerSocket = csd.Socket;
				int ret = connect(gc->ServerSocket, (sockaddr*)&addr, addrSize);
				if (ret != 0 && WSAGetLastError() != 10035)
				{
					Log("LogonClient连接到登录服务器失败, IP:%s, port:%d, err:%u", m_InitData.LogonServerIP, m_InitData.LogonServerPort, WSAGetLastError());
					closesocket(gc->ClientSocket);
					delete(gc);
				}
				else
				{
					gc->RecvTick = tick;
					gc->SendTick = 0;
					gc->State = GWS_OK;
					client.push_back(gc);
				}
			}
		}
		FD_ZERO(pset);
		tick = timeGetTime();
		for (UINT i = 0; i < client.size();)
		{
			GWNewLogonClient *pc = client[i];
			if (tick - pc->RecvTick > timeout)
			{
				Log("Timeout:%u", tick - pc->RecvTick);
				pc->State = GWS_REMOVED;
			}

			if (pc->State == GWS_REMOVED)
			{
				Log("移除客户端.");
				closesocket(pc->ClientSocket);
				closesocket(pc->ServerSocket);
				delete(pc);
				ListRemoveAt(client, i);
				continue;
			}
			FD_ADD(pc->ClientSocket, pset);
			FD_ADD(pc->ServerSocket, pset);
			++i;
		}
		if (FD_COUNT(pset) == 0)
			goto SLEEP;
		int nret = select(0, pset, NULL, NULL, &time);
		tick = timeGetTime();
		for (UINT i = 0; i < client.size(); ++i)
		{
			GWNewLogonClient *pc = client[i];
			//接收数据
			if (FD_ISSET(pc->ServerSocket, pset))
			{
				int offset = pc->ServerRecvOffset + pc->ServerRecvSize;
				int nRet = recv(pc->ServerSocket, pc->pServerBuff + offset, m_InitData.BuffSize - offset, 0);
				if (nRet == 0 || (nRet == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
				{
					Log("接收服务器失败.");
					pc->State = GWS_REMOVED;
					continue;
				}
				if (nRet > 0)
				{
					pc->ServerRecvSize += nRet;
					if (TCPRecvServerCmd(pc) == false)
					{
						pc->State = GWS_REMOVED;
						continue;
					}
				}
			}
			if (FD_ISSET(pc->ClientSocket, pset))
			{
				int nRet = recv(pc->ClientSocket, pc->pClientBuff + pc->ClientRecvSize, m_InitData.BuffSize - pc->ClientRecvSize, 0);
				if (nRet == 0 || (nRet == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
				{
					Log("接收客户端失败.");
					pc->State = GWS_REMOVED;
					continue;
				}
				if (nRet > 0)
				{
					pc->ClientRecvSize += nRet;
					pc->RecvTick = tick;
				}
			}
			//发送数据
			if (pc->ServerCmdList.size() > 0)
			{
				NetCmd *pcmd = pc->ServerCmdList.front();
				int ret = send(pc->ClientSocket, (char*)pcmd, pcmd->GetCmdSize(), 0);
				if (ret == pcmd->GetCmdSize())
				{
					pc->ServerCmdList.pop_front();
					free(pcmd);
				}
			}
			if (pc->ClientRecvSize > 0)
			{
				int ret = send(pc->ServerSocket, pc->pClientBuff, pc->ClientRecvSize, 0);
				if (ret == pc->ClientRecvSize)
				{
					pc->ClientRecvSize = 0;
					pc->SendTick = tick;
				}
			}
			if (tick - pc->SendTick > halftime)
			{
				if (4 == send(pc->ClientSocket, (char*)&hearbeat, 4, 0))
					pc->SendTick = tick;
			}
		}
	SLEEP:
		Sleep(m_InitData.SleepTime);
	}
	DeleteFDSet(pset);
	::InterlockedIncrement(&m_ExitIndex);
}


void NewGateServer::_ThreadOperationAccept()
{
	SOCKADDR_IN addr;
	INT addrSize = sizeof(addr);
	UINT idx = 0;
	char buff[128];
	SOCKET optSocket = m_OperationSocket;
	while (m_bRun)
	{
		for (UINT i = 0; i < m_InitData.OperationAcceptPerFrame; ++i)
		{
			SOCKET s = accept(optSocket, (sockaddr*)&addr, &addrSize);
			if (s == INVALID_SOCKET)
				break;

			InitSocket(s, m_InitData.SocketSendSize, m_InitData.SocketRecvSize, true);
			if (m_OperationThreadData.NewClientList.HasSpace() == false)
			{
				closesocket(s);
				Log("TCP验证：加入运营服务器失败，空间不够.");
			}
			else
			{
				GWOperationClient *pc = new GWOperationClient(m_InitData.BuffSize);
				pc->ClientSocket = s;
				InnerGetIPString(addr, buff);
				Log("TCP验证:新的运营客户端加入:%s", buff);
				m_OperationThreadData.NewClientList.AddItem(pc);
			}
		}
		Sleep(m_InitData.SleepTime);
	}
	::InterlockedIncrement(&m_ExitIndex);
}
void NewGateServer::_ThreadOperationRecv()
{
	vector<GWOperationClient*> client;
	CreateSocketData csd;
	FD_SET *pset = CreateFDSet();
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	inet_pton(AF_INET, m_InitData.OperationServerIP, &addr.sin_addr.s_addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_InitData.OperationServerPort);
	int addrSize = sizeof(addr);
	timeval time = { 0, 0 };
	UINT halftime = 1000;
	UINT hearbeat = HEARBEAT_ID;
	UINT timeout = 5000; //5秒超时
	while (m_bRun)
	{
		UINT tick = timeGetTime();
		while (m_OperationThreadData.NewClientList.HasItem())
		{
			GWOperationClient *gc = m_OperationThreadData.NewClientList.GetItem();
			bool bret = CreateSocket(CST_TCP, 0, m_InitData.SocketRecvSize, m_InitData.SocketSendSize, csd);
			if (bret == false)
			{
				Log("创建TCPSocket 失败");
				closesocket(gc->ClientSocket);
				delete(gc);
			}
			else
			{
				gc->ServerSocket = csd.Socket;
				int ret = connect(gc->ServerSocket, (sockaddr*)&addr, addrSize);
				if (ret != 0 && WSAGetLastError() != 10035)
				{
					Log("LogonClient连接到运营服务器失败, IP:%s, port:%d, err:%u", m_InitData.OperationServerIP, m_InitData.OperationServerPort, WSAGetLastError());
					closesocket(gc->ClientSocket);
					delete(gc);
				}
				else
				{
					gc->RecvTick = tick;
					gc->SendTick = 0;
					gc->State = GWS_OK;
					client.push_back(gc);
				}
			}
		}
		FD_ZERO(pset);
		tick = timeGetTime();
		for (UINT i = 0; i < client.size();)
		{
			GWOperationClient *pc = client[i];
			if (tick - pc->RecvTick > timeout)
			{
				Log("Timeout:%u", tick - pc->RecvTick);
				pc->State = GWS_REMOVED;
			}

			if (pc->State == GWS_REMOVED)
			{
				Log("移除客户端.");
				closesocket(pc->ClientSocket);
				closesocket(pc->ServerSocket);
				delete(pc);
				ListRemoveAt(client, i);
				continue;
			}
			FD_ADD(pc->ClientSocket, pset);
			FD_ADD(pc->ServerSocket, pset);
			++i;
		}
		if (FD_COUNT(pset) == 0)
			goto SLEEP;
		int nret = select(0, pset, NULL, NULL, &time);
		tick = timeGetTime();
		for (UINT i = 0; i < client.size(); ++i)
		{
			GWOperationClient *pc = client[i];
			//接收数据
			if (FD_ISSET(pc->ServerSocket, pset))
			{
				int nRet = recv(pc->ServerSocket, pc->pServerBuff + pc->ServerRecvSize, m_InitData.BuffSize - pc->ServerRecvSize, 0);
				if (nRet == 0 || (nRet == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
				{
					Log("接收服务器失败.");
					pc->State = GWS_REMOVED;
					continue;
				}
				if (nRet > 0)
				{
					pc->ServerRecvSize += nRet;
				}
			}
			if (FD_ISSET(pc->ClientSocket, pset))
			{
				int nRet = recv(pc->ClientSocket, pc->pClientBuff + pc->ClientRecvSize, m_InitData.BuffSize - pc->ClientRecvSize, 0);
				if (nRet == 0 || (nRet == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
				{
					Log("接收客户端失败.");
					pc->State = GWS_REMOVED;
					continue;
				}
				if (nRet > 0)
				{
					pc->ClientRecvSize += nRet;
				}
			}
			//发送数据
			if (pc->ServerRecvSize > 0)
			{
				int ret = send(pc->ClientSocket, pc->pServerBuff, pc->ServerRecvSize, 0);
				if (ret == pc->ServerRecvSize)
				{
					pc->ServerRecvSize = 0;
				}
			}
			if (pc->ClientRecvSize > 0)
			{
				int ret = send(pc->ServerSocket, pc->pClientBuff, pc->ClientRecvSize, 0);
				if (ret == pc->ClientRecvSize)
				{
					pc->ClientRecvSize = 0;
				}
			}
		}
	SLEEP:
		Sleep(m_InitData.SleepTime);
	}
	DeleteFDSet(pset);
	::InterlockedIncrement(&m_ExitIndex);
}