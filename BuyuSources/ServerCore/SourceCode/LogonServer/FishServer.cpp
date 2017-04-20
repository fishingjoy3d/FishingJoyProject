#include "stdafx.h"
#include "FishServer.h"
#include "..\CommonFile\FishServerConfig.h"
FishServer g_FishServer;
FishServer::FishServer()
{
	m_IsClose = false;
	m_DBIsConnect = false;
	m_CenterIsConnect = false;
	m_OperateIsConnect = false;
	m_ClientIndex = 0;
	//m_ServerIndex = 0;

	m_ClientNetworkID = 0;
	//m_ServerNetworkID = 0;

	m_IsReloadConfig = false;
	m_ControlIsConnect = false;

	m_LogDBIsConnect = false;
}
FishServer::~FishServer()
{
}
void FishServer::ShowInfoToWin(const char *pcStr, ...)
{
	if (!pcStr)
	{
		ASSERT(false);
		return;
	}
	va_list var;
	va_start(var, pcStr);
	UINT nCount = _vscprintf(pcStr, var);
	char *pBuffer = new char[nCount + 1];
	vsprintf_s(pBuffer, nCount + 1, pcStr, var);
	printf("%s\n", pBuffer);
	//std::cout << pBuffer;
	//std::cout << "\n";
	SAFE_DELETE_ARR(pBuffer);
	va_end(var);
}
void FishServer::Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt)
{
	if (!pClient)
	{
		ASSERT(false);
		return; 
	}
	if (ServerID == m_ClientNetworkID)
	{
		return;//客户端离开不做处理
	}
	//输出 客户端离开的状态
	switch (rt)
	{
	case REMOVE_NONE:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为None",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_NORMAL:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为普通",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_RECV_ERROR:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为接收错误",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_SEND_ERROR:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为发送错误",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_TIMEOUT:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为超时",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_CMD_SEND_OVERFLOW:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为发送太多",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_CMD_RECV_OVERFLOW:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为接收太多",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_CMD_SIZE_ERROR:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为命令大小",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_RECVBACK_NOT_SPACE:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为接收缓冲区溢出",pClient->OutsideExtraData);
		}
		break;
	}
}
bool FishServer::InitServer(DWORD LogonID)
{
	m_pDump.OnInit();
	//初始化登陆服务器 我们进行设置 
	//1.从配置文件里 加载 Logon 的 一些配置 我们想要进行配置 FishServer.xml 里进行读取 对外监听端口 对内监听端口 数据库的端口是多少 等 想要进行处理
	m_LogonConfigID = ConvertDWORDToBYTE(LogonID);
	if (!g_FishServerConfig.LoadServerConfigFile())
	{
		ShowInfoToWin("服务器配置文件读取失败");
		return false;
	}
	if (!m_FishConfig.LoadConfigFilePath())
	{
		ASSERT(false);
		return false;
	}
	ShowInfoToWin("配置文件读取成功");

	if (!ConnectControl())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectControl())
				break;
		}
	}
	if (!ConnectDB())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectDB())
				break;
		}
	}
	if (!ConnectCenter())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectCenter())
				break;
		}
	}
	if (!ConnectOperate())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectOperate())
				break;
		}
	}
	if (!ConnectLogDB())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectLogDB())
				break;
		}
	}
	

 	m_LogonCacheManager.OnLoadAllAccountData();//向数据库请求数据
	return true;
}
bool FishServer::ConnectLogDB()
{
	LogonServerConfig* pLogonConfig = g_FishServerConfig.GetLogonServerConfig(m_LogonConfigID);
	if (!pLogonConfig)
	{
		return false;
	}
	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pLogonConfig->LogDBID);
	if (!pDBConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pDBData;
	pDBData.BuffSize = pDBConfig->BuffSize;
	pDBData.Port = pDBConfig->DBListenPort;
	pDBData.SleepTime = pDBConfig->SleepTime;
	pDBData.SocketRecvSize = pDBConfig->RecvBuffSize;
	pDBData.SocketSendSize = pDBConfig->SendBuffSize;
	pDBData.ThreadNum = 1;
	pDBData.Timeout = pDBConfig->TimeOut;
	pDBData.CmdHearbeat = 0;
	pDBData.SendArraySize = pDBConfig->MaxSendCmdCount;
	pDBData.RecvArraySize = pDBConfig->MaxRecvCmdCount;
	if (!m_LogDBTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_LogonConfigID, sizeof(BYTE)))
	{
		ShowInfoToWin("Log数据库连接失败");
		return false;
	}
	OnTcpClientConnect(&m_LogDBTcp);
	return true;
}
bool FishServer::ConnectDB()
{
	LogonServerConfig* pLogonConfig = g_FishServerConfig.GetLogonServerConfig(m_LogonConfigID);
	if (!pLogonConfig)
	{
		return false;
	}
	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pLogonConfig->DBNetworkID);
	if (!pDBConfig)
	{
		return false;
	}

	ClientInitData pDBData;
	pDBData.BuffSize = pDBConfig->BuffSize;
	pDBData.Port = pDBConfig->DBListenPort;
	pDBData.SleepTime = pDBConfig->SleepTime;
	pDBData.SocketRecvSize = pDBConfig->RecvBuffSize;
	pDBData.SocketSendSize = pDBConfig->SendBuffSize;
	pDBData.ThreadNum = 1;
	pDBData.Timeout = pDBConfig->TimeOut;
	pDBData.CmdHearbeat = 0;
	pDBData.SendArraySize = pDBConfig->MaxSendCmdCount;
	pDBData.RecvArraySize = pDBConfig->MaxRecvCmdCount;
	if (!m_DBTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_LogonConfigID, sizeof(BYTE)))
	{
		ShowInfoToWin("数据库连接失败");
		return false;
	}
	OnTcpClientConnect(&m_DBTcp);
	return true;
}
bool FishServer::ConnectCenter()
{
	CenterServerConfig* pCenterConfig = g_FishServerConfig.GetCenterServerConfig();
	if (!pCenterConfig)
	{
		return false;
	}

	ClientInitData pDBData;
	pDBData.BuffSize = pCenterConfig->BuffSize;
	pDBData.Port = pCenterConfig->CenterListenPort;
	pDBData.SleepTime = pCenterConfig->SleepTime;
	pDBData.SocketRecvSize = pCenterConfig->RecvBuffSize;
	pDBData.SocketSendSize = pCenterConfig->SendBuffSize;
	pDBData.ThreadNum = 1;
	pDBData.Timeout = pCenterConfig->TimeOut;
	pDBData.CmdHearbeat = 0;
	pDBData.SendArraySize = pCenterConfig->MaxSendCmdCount;
	pDBData.RecvArraySize = pCenterConfig->MaxRecvCmdCount;
	if (!m_CenterTcp.Init(pCenterConfig->CenterListenIP, pDBData, (void*)&m_LogonConfigID, sizeof(BYTE)))
	{
		ShowInfoToWin("中央服务器连接失败");
		return false;
	}
	OnTcpClientConnect(&m_CenterTcp);
	return true;
}
bool FishServer::ConnectOperate()
{
	OperateConfig* pOperateConfig = g_FishServerConfig.GetOperateConfig();
	if (!pOperateConfig)
	{
		return false;
	}
	ClientInitData pDBData;
	pDBData.BuffSize = pOperateConfig->BuffSize;
	pDBData.Port = pOperateConfig->OperateListenPort;
	pDBData.SleepTime = pOperateConfig->SleepTime;
	pDBData.SocketRecvSize = pOperateConfig->RecvBuffSize;
	pDBData.SocketSendSize = pOperateConfig->SendBuffSize;
	pDBData.ThreadNum = 1;
	pDBData.Timeout = pOperateConfig->TimeOut;
	pDBData.CmdHearbeat = 0;
	pDBData.SendArraySize = pOperateConfig->MaxSendCmdCount;
	pDBData.RecvArraySize = pOperateConfig->MaxRecvCmdCount;
	if (!m_OperateTcp.Init(pOperateConfig->OperateListenIP, pDBData, (void*)&m_LogonConfigID, sizeof(BYTE)))
	{
		ShowInfoToWin("运营服务器连接失败");
		return false;
	}
	OnTcpClientConnect(&m_OperateTcp);
	return true;
}
bool FishServer::ConnectClient()
{
	LogonServerConfig* pLogonConfig = g_FishServerConfig.GetLogonServerConfig(m_LogonConfigID);
	if (!pLogonConfig)
	{
		return false;
	}

	//发送到客户端的
	ServerInitData pCLientData;
	pCLientData.ServerID = pLogonConfig->ClientNetworkID;
	pCLientData.BuffSize = pLogonConfig->LogonClientBuffSize;
	pCLientData.CmdHearbeat = 0;
	pCLientData.Port = pLogonConfig->LogonClientListenPort;
	pCLientData.SocketRecvSize = pLogonConfig->LogonClientRecvBuffSize;
	pCLientData.SocketSendSize = pLogonConfig->LogonClientSendBuffSize;
	pCLientData.Timeout = pLogonConfig->LogonClientTimeOut;
	pCLientData.RecvThreadNum = pLogonConfig->LogonClientRecvThreadSum;
	pCLientData.SendThreadNum = pLogonConfig->LogonClientSendThreadSum;
	pCLientData.SleepTime = pLogonConfig->LogonClientSleepTime;
	pCLientData.AcceptSleepTime = pLogonConfig->LogonClientAcceptSleepTime;
	pCLientData.AcceptRecvData = false;	//表示接收初始数据

	pCLientData.SceneHearbeatTick = pLogonConfig->LogonClientSceneHearbeatTick;
	pCLientData.MaxSendCmdCount = pLogonConfig->LogonClientMaxSendCmdCount;
	pCLientData.MaxAcceptNumPerFrame = pLogonConfig->LogonClientMaxAcceptNumPerFrame;
	pCLientData.MaxRecvCmdCount = pLogonConfig->LogonClientMaxRecvCmdCount;
	pCLientData.MaxSendCountPerFrame = pLogonConfig->LogonClientMaxSendCountPerFrame;
	pCLientData.ListenCount = pLogonConfig->LogonClientListenCount;

	m_ClientTcp.SetCmdHandler(this);
	m_ClientNetworkID = pLogonConfig->ClientNetworkID;
	if (!m_ClientTcp.Init(pCLientData))
	{
		ShowInfoToWin("对内网络库启动失败");
		return false;
	}
	//对GameServer 的 
	/*ServerInitData pServerData;
	pServerData.ServerID = pLogonConfig->ServerNetworkID;
	pServerData.BuffSize = pLogonConfig->LogonServerBuffSize;
	pServerData.CmdHearbeat = 0;
	pServerData.Port = pLogonConfig->LogonServerListenPort;
	pServerData.SocketRecvSize = pLogonConfig->LogonServerRecvBuffSize;
	pServerData.SocketSendSize = pLogonConfig->LogonServerSendBuffSize;
	pServerData.Timeout = pLogonConfig->LogonServerTimeOut;
	pServerData.RecvThreadNum = pLogonConfig->LogonServerRecvThreadSum;
	pServerData.SendThreadNum = pLogonConfig->LogonServerSendThreadSum;
	pServerData.SleepTime = pLogonConfig->LogonServerSleepTime;
	pServerData.AcceptSleepTime = pLogonConfig->LogonServerAcceptSleepTime;
	pServerData.AcceptRecvData = true;

	pServerData.SceneHearbeatTick = pLogonConfig->LogonServerSceneHearbeatTick;
	pServerData.MaxSendCmdCount = pLogonConfig->LogonServerMaxSendCmdCount;
	pServerData.MaxAcceptNumPerFrame = pLogonConfig->LogonServerMaxAcceptNumPerFrame;
	pServerData.MaxRecvCmdCount = pLogonConfig->LogonServerMaxRecvCmdCount;
	pServerData.MaxSendCountPerFrame = pLogonConfig->LogonServerMaxSendCountPerFrame;
	pServerData.ListenCount = pLogonConfig->LogonServerListenCount;

	m_ServerTcp.SetCmdHandler(this);
	m_ServerNetworkID = pLogonConfig->ServerNetworkID;
	if (!m_ServerTcp.Init(pServerData))
	{
		ShowInfoToWin("对外网络库启动失败");
		return false;
	}*/
	return true;
}
bool FishServer::OnDestroy()
{
	HandleAllMsg();
	OnClearAllChannelInfo();
	while (m_AfxAddClient.HasItem())
	{
		AfxNetworkClientOnce* pOnce = m_AfxAddClient.GetItem();
		delete pOnce;
	}
	return true;
}
uint FishServer::CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize)
{
	//中央服务器只有在FTP 和 DB都连接成功后才工作
	bool IsNeed = m_DBTcp.IsConnected() && m_CenterTcp.IsConnected();
	if (IsNeed)
	{
		/*if (SeverID == m_ServerNetworkID)
			return CONNECT_CHECK_OK;
		else */
		if (SeverID == m_ClientNetworkID)
		{
			if (recvSize != 2*sizeof(DWORD) || !pData)
				return CONNECT_CHECK_FAILED;
			DWORD* pInfo = (DWORD*)pData;
			if (!m_FishConfig.CheckVersionAndPathCrc(pInfo[0], pInfo[1]))
			{
				pInfo[0] = m_FishConfig.GetSystemConfig().VersionID;
				pInfo[1] = m_FishConfig.GetSystemConfig().PathCrc;
				return 2 * sizeof(DWORD);
			}
			return (CONNECT_CHECK_OK);
		}
		else
			return (CONNECT_CHECK_FAILED);
	}
	else
		return (CONNECT_CHECK_FAILED);
}
bool FishServer::NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize)
{
	if (!pClient)
	{
		ASSERT(false);
		return false;
	}
	AfxNetworkClientOnce* pOnce = new AfxNetworkClientOnce();
	if (!pOnce)
	{
		/*if (SeverID == m_ServerNetworkID)
		{
			m_ServerTcp.Kick(pClient);
		}
		else*/
		if (SeverID == m_ClientNetworkID)
		{
			m_ClientTcp.Kick(pClient);
		}
		ASSERT(false);
		return false;
	}
	pOnce->SeverID = SeverID;
	pOnce->pClient = pClient;
	pOnce->pPoint = (void*)malloc(recvSize);
	if (!pOnce->pPoint)
	{
		/*if (SeverID == m_ServerNetworkID)
		{
			m_ServerTcp.Kick(pClient);
		}
		else */
		if (SeverID == m_ClientNetworkID)
		{
			m_ClientTcp.Kick(pClient);
		}
		ASSERT(false);
		delete pOnce;
		return false;
	}
	memcpy_s(pOnce->pPoint, recvSize, pData, recvSize);
	pOnce->Length = recvSize;
	m_AfxAddClient.AddItem(pOnce);
	return true;
}
void FishServer::OnAddClient()
{
	while (m_AfxAddClient.HasItem())
	{
		AfxNetworkClientOnce* pOnce = m_AfxAddClient.GetItem();
		if (!pOnce)
		{
			/*if (pOnce->SeverID == m_ServerNetworkID)
			{
				m_ServerTcp.Kick(pOnce->pClient);
			}
			else */
			if (pOnce->SeverID == m_ClientNetworkID)
			{
				m_ClientTcp.Kick(pOnce->pClient);
			}
			//ASSERT(false);
			continue;
		}
		if (pOnce->SeverID == m_ClientNetworkID)
		{
			pOnce->pClient->OutsideExtraData = ++m_ClientIndex;
			m_ClintList.insert(HashMap<DWORD, ServerClientData*>::value_type(pOnce->pClient->OutsideExtraData, pOnce->pClient));
			//触发玩家加入的事件
			OnTcpServerJoin(pOnce->SeverID, pOnce->pClient);
			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
		}
		//else if (pOnce->SeverID == m_ServerNetworkID)
		//{
		//	if (!pOnce->pPoint || pOnce->Length != sizeof(BYTE))
		//	{
		//		m_ServerTcp.Kick(pOnce->pClient);
		//		//ASSERT(false);
		//		if (pOnce->pPoint)
		//			free(pOnce->pPoint);
		//		delete pOnce;
		//	}
		//	BYTE ClientID = *((BYTE*)pOnce->pPoint);
		//	if (ClientID == 0)
		//	{
		//		m_ServerTcp.Kick(pOnce->pClient);
		//		//ASSERT(false);
		//		if (pOnce->pPoint)
		//			free(pOnce->pPoint);
		//		delete pOnce;
		//		continue;
		//	}
		//	pOnce->pClient->OutsideExtraData = ClientID;//玩家的ID
		//	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ServerList.find(ClientID);
		//	if (Iter != m_ServerList.end())
		//	{
		//		ASSERT(false);
		//		m_ServerTcp.Kick(pOnce->pClient);
		//		if (pOnce->pPoint)
		//			free(pOnce->pPoint);
		//		delete pOnce;
		//		continue;
		//	}
		//	m_ServerList.insert(HashMap<BYTE, ServerClientData*>::value_type(ClientID, pOnce->pClient));
		//	//触发玩家加入的事件
		//	OnTcpServerJoin(pOnce->SeverID, pOnce->pClient);
		//	if (pOnce->pPoint)
		//		free(pOnce->pPoint);
		//	delete pOnce;

		//	m_LogonManager.OnGameRsgLogon(ClientID);
		//}
		else
		{
			//ASSERT(false);
			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
		}
	}
}
void FishServer::OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//往网络库中有一个客户端离开的时候
	//if (ServerID == m_ServerNetworkID)
	//{
	//	/*GameServerInfo* pGameServerInfo = GetGameServerInfo(pClient->OutsideExtraData);
	//	if (pGameServerInfo)
	//		delete pGameServerInfo;*/
	//	//m_GameServerOnlineSumMap.erase((DWORD)pClient->OutsideExtraData);
	//	m_LogonManager.OnGameLeaveLogon(ConvertDWORDToBYTE(pClient->OutsideExtraData));
	//	ShowInfoToWin("一个GameServer离开了LogonServer");
	//}
	//else 
	if (ServerID == m_ClientNetworkID)
	{
		//ShowInfoToWin("一个Client离开了LogonServer");
	}
	else
	{
		ASSERT(false);
		return;
	}
	return;
}
void FishServer::OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//往网络库中有一个客户端加入的时候
	//if (ServerID == m_ServerNetworkID)
	//{
	//	ShowInfoToWin("一个GameServer加入了LogonServer");
	//	//当一个GameServer加入到集合里面去的时候 
	//	m_LogonManager.OnGameJoinLogon(ConvertDWORDToBYTE(pClient->OutsideExtraData));
	//}
	//else 
	if (ServerID == m_ClientNetworkID)
	{
		//ShowInfoToWin("一个Client加入了LogonServer");
	}
	else
	{
		ASSERT(false);
		return;
	}
	return;
}
void FishServer::OnTcpClientConnect(TCPClient* pClient)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//网络库连接成功的时候
	if (pClient == &m_DBTcp)
	{
		m_DBIsConnect = true;
		ShowInfoToWin("已经与数据库连接成功了");
	}
	else if (pClient == &m_CenterTcp)
	{
		m_CenterIsConnect = true;

		m_LogonManager.OnLogonRsgToCenter();//注册Logon到Center去
		ShowInfoToWin("已经与中央服务器连接成功了");
	}
	else if (pClient == &m_OperateTcp)
	{
		m_OperateIsConnect = true;
		ShowInfoToWin("已经与运营服务器连接成功了");
	}
	else if (pClient == &m_ControlTcp)
	{
		m_ControlIsConnect = true;
		ShowInfoToWin("已经与ControlServer连接成功了");
	}
	else if (pClient == &m_LogDBTcp)
	{
		m_LogDBIsConnect = true;
		ShowInfoToWin("已经与Log数据库连接成功了");
	}
	else
	{
		ASSERT(false);
		return;
	}
	return;
}
void FishServer::OnTcpClientLeave(TCPClient* pClient)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//网络库断开连接的时候
	if (pClient == &m_DBTcp)
	{
		ShowInfoToWin("已经与数据库断开连接了");
		m_DBIsConnect = false;
		m_DBTcp.Shutdown();
	}
	else if (pClient == &m_CenterTcp)
	{
		ShowInfoToWin("已经与中央服务器断开连接了");
		m_CenterIsConnect = false;
		m_CenterTcp.Shutdown();
		m_LogonManager.OnLogonLeaveCenter();
	}
	else if (pClient == &m_OperateTcp)
	{
		m_OperateIsConnect = false;
		m_OperateTcp.Shutdown();
		ShowInfoToWin("已经与运营服务器断开连接了");
	}
	else if (pClient == &m_ControlTcp)
	{
		m_ControlIsConnect = false;
		m_ControlTcp.Shutdown();
		ShowInfoToWin("已经与ControlServer断开连接了");
	}
	else if (pClient == &m_LogDBTcp)
	{
		ShowInfoToWin("已经与Log数据库断开连接了");
		m_LogDBIsConnect = false;
		m_LogDBTcp.Shutdown();
	}
	else
	{
		ASSERT(false);
		return;
	}
	return;
}
void FishServer::SendNetCmdToDB(NetCmd* pCmd)
{
	if (!pCmd || !m_DBTcp.IsConnected())
		return;
	if (!m_DBTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToCenter(NetCmd* pCmd)
{
	if (!pCmd || !m_CenterTcp.IsConnected())
		return;
	if (!m_CenterTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToOperate(NetCmd* pCmd)
{
	if (!pCmd || !m_OperateTcp.IsConnected())
		return;
	if (!m_OperateTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToGameServer(BYTE GameID, NetCmd* pCmd)
{
	if (!pCmd)
		return;
	DWORD PageSize = sizeof(SS_Cmd_LogonToGame)+sizeof(BYTE)* (pCmd->CmdSize - 1);
	CheckMsgSize(PageSize);
	SS_Cmd_LogonToGame* msg = (SS_Cmd_LogonToGame*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdSize(static_cast<WORD>(PageSize));
	msg->SetCmdType(GetMsgType(Main_Server, SS_LogonToGame));
	memcpy_s(msg->Array, pCmd->CmdSize, pCmd, pCmd->CmdSize);
	msg->Length = pCmd->CmdSize;
	msg->GameID = GameID;
	SendNetCmdToCenter(msg);
	free(msg);
	//m_ServerTcp.Send(pClient, pCmd);
}
void FishServer::SendNewCmdToClient(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return;
	if (!m_ClientTcp.Send(pClient, pCmd))
	{
		ASSERT(false);
	}
}
void FishServer::HandleAllMsg()
{
	if (!m_ClintList.empty())
	{
		HashMap<DWORD, ServerClientData*>::iterator Iter = m_ClintList.begin();
		for (; Iter != m_ClintList.end();)
		{
			if (!Iter->second)
				continue;
			while (Iter->second->RecvList.HasItem())
			{
				NetCmd* pCmd = Iter->second->RecvList.GetItem();
				//处理网络命令 客户端发送来的登陆 注册 等命令
				HandleClientMsg(Iter->second, pCmd);
				free(pCmd);
			}
			++Iter;
		}
	}

	//if (!m_ServerList.empty())
	//{
	//	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ServerList.begin();
	//	for (; Iter != m_ServerList.end();)
	//	{
	//		if (!Iter->second)
	//			continue;
	//		while (Iter->second->RecvList.HasItem())
	//		{
	//			NetCmd* pCmd = Iter->second->RecvList.GetItem();
	//			//处理网络命令 GameServer发送来的命令
	//			HandleGameServerMsg(Iter->second, pCmd);
	//			free(pCmd);
	//		}
	//		++Iter;
	//	}
	//}

	NetCmd* pCmd = m_DBTcp.GetCmd();
	while (pCmd)
	{
		HandleDataBaseMsg(pCmd);
		free(pCmd);
		pCmd = m_DBTcp.GetCmd();
	}

	pCmd = m_OperateTcp.GetCmd();
	while (pCmd)
	{
		HandleOperateMsg(pCmd);
		free(pCmd);
		pCmd = m_OperateTcp.GetCmd();
	}

	pCmd = m_ControlTcp.GetCmd();
	while (pCmd)
	{
		HandleControlMsg(pCmd);
		free(pCmd);
		pCmd = m_ControlTcp.GetCmd();
	}
}
bool FishServer::MainUpdate()
{
	while (!m_IsClose)
	{
		DWORD dwTimer = timeGetTime();

		if (m_IsReloadConfig)
		{
			m_IsReloadConfig = false;
			OnReloadConfig();//重新加载配置文件
		}
		//0.
		OnAddClient();
		//1.
		OnUpdateWriteGameServerUserMap(dwTimer);

		UpdateInfoToControl(dwTimer);

		//1.处理Client 的命令
		if (!m_ClintList.empty())
		{
			HashMap<DWORD, ServerClientData*>::iterator Iter = m_ClintList.begin();
			for (; Iter != m_ClintList.end();)
			{
				if (!Iter->second)
					continue;
				if (Iter->second->Removed)
				{
					OnTcpServerLeave(m_ClientNetworkID, Iter->second);
					m_ClientTcp.Kick(Iter->second);
					Iter = m_ClintList.erase(Iter);

					//ShowInfoToWin("移除一个客户端");

					continue;
				}
				int Sum = 0;
				while (Iter->second->RecvList.HasItem() && Sum < Client_Msg_OnceSum)
				{
					NetCmd* pCmd = Iter->second->RecvList.GetItem();
					//处理网络命令 客户端发送来的登陆 注册 等命令
					HandleClientMsg(Iter->second, pCmd);
					free(pCmd);
					++Sum;
				}
				++Iter;
			}
		}
		//2.处理Server的命令
		//if (!m_ServerList.empty())
		//{
		//	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ServerList.begin();
		//	for (; Iter != m_ServerList.end();)
		//	{
		//		if (!Iter->second)
		//			continue;
		//		if (Iter->second->Removed)
		//		{
		//			OnTcpServerLeave(m_ServerNetworkID, Iter->second);
		//			m_ServerTcp.Kick(Iter->second);
		//			Iter = m_ServerList.erase(Iter);
		//			continue;
		//		}
		//		int Sum = 0;
		//		while (Iter->second->RecvList.HasItem() && Sum < Msg_OnceSum)
		//		{
		//			NetCmd* pCmd = Iter->second->RecvList.GetItem();
		//			//处理网络命令 GameServer发送来的命令
		//			HandleGameServerMsg(Iter->second, pCmd);
		//			free(pCmd);
		//			++Sum;
		//		}
		//		++Iter;
		//	}
		//}
		//3.处理数据库的命令
		if (m_DBTcp.IsConnected()) 
		{
			if (!m_DBIsConnect)
				OnTcpClientConnect(&m_DBTcp);
			NetCmd* pCmd = m_DBTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				//处理数据库发送来的数据
				HandleDataBaseMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_DBTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleDataBaseMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_DBIsConnect)
				OnTcpClientLeave(&m_DBTcp);

			//重新连接DB
			ConnectDB();
		}

		//4.处理中央服务器的命令
		if (m_CenterTcp.IsConnected())
		{
			if (!m_CenterIsConnect)
				OnTcpClientConnect(&m_CenterTcp);
			NetCmd* pCmd = m_CenterTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleCenterMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_CenterTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleCenterMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_CenterIsConnect)
				OnTcpClientLeave(&m_CenterTcp);

			//重新连接DB
			ConnectCenter();
		}

		//5.处理运营服务器的命令
		if (m_OperateTcp.IsConnected())
		{
			if (!m_OperateIsConnect)
				OnTcpClientConnect(&m_OperateTcp);
			NetCmd* pCmd = m_OperateTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleOperateMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_OperateTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleOperateMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_OperateIsConnect)
				OnTcpClientLeave(&m_OperateTcp);

			//重新连接DB
			ConnectOperate();
		}

		if (m_ControlTcp.IsConnected())
		{
			if (!m_ControlIsConnect)
				OnTcpClientConnect(&m_ControlTcp);
			NetCmd* pCmd = m_ControlTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleControlMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_ControlTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleControlMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_ControlIsConnect)
				OnTcpClientLeave(&m_ControlTcp);

			//重新连接DB
			ConnectControl();
		}

		if (m_LogDBTcp.IsConnected())//LogDB 重新连接
		{
			if (!m_LogDBIsConnect)
				OnTcpClientConnect(&m_LogDBTcp);
		}
		else
		{
			if (m_LogDBIsConnect)
				OnTcpClientLeave(&m_LogDBTcp);

			//重新连接DB
			ConnectLogDB();
		}

		Sleep(1);
	}
	return OnDestroy();
}

void FishServer::HandleClientMsg(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return;
	if (pCmd->CmdType != Main_Logon)
		return;
	switch (pCmd->SubCmdType)
	{
	case CL_AccountLogon:
		{
			CL_Cmd_AccountLogon* pMsg = (CL_Cmd_AccountLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			//先进行 版本 路径 的验证  验证成功 后 我们让玩家进行登陆 登陆必须携带 平台的ID
			if (!m_FishConfig.CheckVersionAndPathCrc(pMsg->VersionID, pMsg->PathCrc))
			{
				//ASSERT(false);
				LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端版本错误 普通登陆失败 %u, %u  vs %u, %u"), pMsg->VersionID, pMsg->PathCrc, m_FishConfig.GetSystemConfig().VersionID, m_FishConfig.GetSystemConfig().PathCrc);

				return;
			}
			if (!m_FishConfig.CheckStringIsError(pMsg->AccountName, ACCOUNT_MIN_LENGTH, ACCOUNT_LENGTH, SCT_AccountName))
			{
				LC_Cmd_AccountOnlyID msgClient;
				SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
				msgClient.dwOnlyID = 0;
				msgClient.dwUserID = 0;
				msgClient.GameIp = 0;
				msgClient.GamePort = 0;
				msgClient.GateIp = 0;
				msgClient.GatePort = 0;
				msgClient.LogonType = 1;
				SendNewCmdToClient(pClient, &msgClient);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);


				LogInfoToFile("LogonError.txt",TEXT("客户端账号名称错误 普通登陆失败 玩家账号:%s 密码:%u,%u,%u"), pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);

				return;
			}
			//先向缓存请求
			DWORD dwUserID = 0;
			bool IsFreeze = false;
			__int64 FreezeEndTime = 0;
			if (!m_LogonCacheManager.CheckAccountData(pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3, dwUserID, IsFreeze, FreezeEndTime))
			{
				DBR_Cmd_AccountLogon msg;
				SetMsgInfo(msg, DBR_AccountLogon, sizeof(DBR_Cmd_AccountLogon));
				msg.ClientID = pClient->OutsideExtraData;
				TCHARCopy(msg.AccountName, CountArray(msg.AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
				msg.PasswordCrc1 = pMsg->PasswordCrc1;
				msg.PasswordCrc2 = pMsg->PasswordCrc2;
				msg.PasswordCrc3 = pMsg->PasswordCrc3;
				TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
				SendNetCmdToDB(&msg);

				//缓存记录
				TempAccountCacheInfo pData;
				pData.ClientID = pClient->OutsideExtraData;
				TCHARCopy(pData.AccountInfo.AccountName, CountArray(pData.AccountInfo.AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
				TCHARCopy(pData.AccountInfo.MacAddress, CountArray(pData.AccountInfo.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
				pData.AccountInfo.PasswordCrc1 = pMsg->PasswordCrc1;
				pData.AccountInfo.PasswordCrc2 = pMsg->PasswordCrc2;
				pData.AccountInfo.PasswordCrc3 = pMsg->PasswordCrc3;
				m_LogonCacheManager.OnAddTempAccountInfo(pData);
			}
			else
			{
				//缓存验证完毕了 我们可以进行处理了 
				if (dwUserID == 0)
				{
					//密码错误 无法登陆
					LC_Cmd_AccountOnlyID msgClient;
					SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
					msgClient.dwOnlyID = 0;
					msgClient.dwUserID = 0;
					msgClient.GameIp = 0;
					msgClient.GamePort = 0;
					msgClient.GateIp = 0;
					msgClient.GatePort = 0;
					msgClient.LogonType = 1;
					SendNewCmdToClient(pClient, &msgClient);

					DelClient pDel;
					pDel.LogTime = timeGetTime();
					pDel.SocketID = pClient->OutsideExtraData;
					m_DelSocketVec.push_back(pDel);

					LogInfoToFile("LogonError.txt", TEXT("客户端账号不存在或者密码错误 普通登陆失败 玩家账号:%s 密码:%u,%u,%u"), pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				}
				else
				{
					if (IsFreeze)
					{
						LC_Cmd_AccountIsFreeze msg;
						SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountIsFreeze), sizeof(LC_Cmd_AccountIsFreeze));
						msg.EndTime = FreezeEndTime;
						SendNewCmdToClient(pClient, &msg);

						DelClient pDel;
						pDel.LogTime = timeGetTime();
						pDel.SocketID = pClient->OutsideExtraData;
						m_DelSocketVec.push_back(pDel);
						LogInfoToFile("LogonError.txt", TEXT("客户端已经被冻结 登陆失败"));
						return;
					}
					GameServerConfig* pConfig = g_FishServerConfig.GetGameServerConfig(m_LogonManager.GetGameServerConfigIDByUserID(dwUserID));//找到一个最合适的GameServer
					if (!pConfig)
					{
						ASSERT(false);
						return;
					}
					/*ServerClientData* pGameServer = GetServerServerData(pConfig->NetworkID);
					if (!pGameServer)
					{
						ASSERT(false);
						return;
					}*/
					if (!OnAddUserWriteGameServer(dwUserID, pClient->OutsideExtraData))
					{
						ASSERT(false);
						return;
					}
					DWORD OnlyID = RandUInt();
					//1.发送命令到GameServer去
					OnSendChannelInfoToGameServer(dwUserID, pClient->OutsideExtraData, pConfig->NetworkID);
					LG_Cmd_AccountOnlyID msgGameServer;
					SetMsgInfo(msgGameServer, GetMsgType(Main_Logon, LG_AccountOnlyID), sizeof(LG_Cmd_AccountOnlyID));
					msgGameServer.dwOnlyID = OnlyID;
					msgGameServer.dwUserID = dwUserID;
					msgGameServer.ClientID = pClient->OutsideExtraData;
					msgGameServer.GameServerID = pConfig->NetworkID;
					msgGameServer.LogonTypeID = 1;
					msgGameServer.LogonID = m_LogonConfigID;
					SendNetCmdToGameServer(pConfig->NetworkID, &msgGameServer);//将命令通过中央服务器发送到Logon去
				}
			}
			return;
		}
		break;
	case CL_AccountRsg:
		{
			CL_Cmd_AccountRsg* pMsg = (CL_Cmd_AccountRsg*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			if (!m_FishConfig.CheckVersionAndPathCrc(pMsg->VersionID, pMsg->PathCrc))
			{
				ASSERT(false);
				LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端版本错误 普通注册失败 玩家账号:%s 密码:%u,%u,%u"), pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				return;
			}
			if (!m_FishConfig.CheckStringIsError(pMsg->AccountName, ACCOUNT_MIN_LENGTH, ACCOUNT_LENGTH, SCT_AccountName))
			{
				LC_Cmd_AccountOnlyID msgClient;
				SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
				msgClient.dwOnlyID = 0;
				msgClient.dwUserID = 0;
				msgClient.GameIp = 0;
				msgClient.GamePort = 0;
				msgClient.GateIp = 0;
				msgClient.GatePort = 0;
				msgClient.LogonType = 2;
				SendNewCmdToClient(pClient, &msgClient);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端账号名称错误 普通注册失败 玩家账号:%s 密码:%u,%u,%u"), pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				return;
			}
			if (m_LogonCacheManager.IsExistsAccount(pMsg->AccountName))
			{
				LC_Cmd_AccountOnlyID msgClient;
				SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
				msgClient.dwOnlyID = 0;
				msgClient.dwUserID = 0;
				msgClient.GameIp = 0;
				msgClient.GamePort = 0;
				msgClient.GateIp = 0;
				msgClient.GatePort = 0;
				msgClient.LogonType = 2;
				SendNewCmdToClient(pClient, &msgClient);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端账号已经存在 普通注册失败 玩家账号:%s 密码:%u,%u,%u"), pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				return;
			}

			DBR_Cmd_AccountRsg msg;
			SetMsgInfo(msg,DBR_AccountRsg, sizeof(DBR_Cmd_AccountRsg));
			msg.ClientID = pClient->OutsideExtraData;
			TCHARCopy(msg.AccountName, CountArray(msg.AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
			TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			msg.PasswordCrc1 = pMsg->PasswordCrc1;
			msg.PasswordCrc2 = pMsg->PasswordCrc2;
			msg.PasswordCrc3 = pMsg->PasswordCrc3;
			msg.ClientIP = pClient->IP;
			SendNetCmdToDB(&msg);

			TempAccountCacheInfo pData;
			pData.ClientID = pClient->OutsideExtraData;
			TCHARCopy(pData.AccountInfo.AccountName, CountArray(pData.AccountInfo.AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
			TCHARCopy(pData.AccountInfo.MacAddress, CountArray(pData.AccountInfo.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			pData.AccountInfo.PasswordCrc1 = pMsg->PasswordCrc1;
			pData.AccountInfo.PasswordCrc2 = pMsg->PasswordCrc2;
			pData.AccountInfo.PasswordCrc3 = pMsg->PasswordCrc3;
			m_LogonCacheManager.OnAddTempAccountInfo(pData);

			return;
		}
		break;
	case CL_GetNewAccount:
		{
			CL_Cmd_GetNewAccount* pMsg = (CL_Cmd_GetNewAccount*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			if (!m_FishConfig.CheckVersionAndPathCrc(pMsg->VersionID, pMsg->PathCrc))
			{
				//ASSERT(false);
				LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端版本错误 获取新账号失败"));
				return;
			}
			//将命令转发到数据库去
			DBR_Cmd_GetNewAccount msg;
			SetMsgInfo(msg, DBR_GetNewAccount, sizeof(DBR_Cmd_GetNewAccount));
			TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			msg.ClientID = pClient->OutsideExtraData;
			msg.PasswordCrc1 = pMsg->PasswordCrc1;
			msg.PasswordCrc2 = pMsg->PasswordCrc2;
			msg.PasswordCrc3 = pMsg->PasswordCrc3;
			msg.ClientIP = pClient->IP;
			SendNetCmdToDB(&msg);
			return;
		}
		break;
	case CL_ChannelLogon:
		{
			//g_FishServer.ShowInfoToWin("接收到玩家进行渠道登陆1");
			CL_Cmd_ChannelLogon* pMsg = (CL_Cmd_ChannelLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			//g_FishServer.ShowInfoToWin("接收到玩家进行渠道登陆2");
			if (!m_FishConfig.CheckVersionAndPathCrc(pMsg->VersionID, pMsg->PathCrc))
			{
				ASSERT(false);
				LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端版本错误 渠道登陆失败"));
				return;
			}
			//g_FishServer.ShowInfoToWin("接收到玩家进行渠道登陆3");
			//接收到渠道登陆 我们直接将命令转发到 Operater  运营服务器去
			DWORD Page = sizeof(LO_Cmd_ChannelLogon)+(pMsg->channelInfo.Sum - 1)*sizeof(BYTE);
			LO_Cmd_ChannelLogon* msg = (LO_Cmd_ChannelLogon*)malloc(Page);
			msg->SetCmdSize(ConvertDWORDToWORD(Page));
			msg->SetCmdType(GetMsgType(Main_Logon, LO_ChannelLogon));
			msg->PlateFormID = pMsg->PlateFormID;
			msg->ClientID = pClient->OutsideExtraData;
			DWORD DataLength = (pMsg->channelInfo.Sum - 1)*sizeof(BYTE)+sizeof(ChannelUserInfo);
			memcpy_s(&msg->channelInfo, DataLength, &pMsg->channelInfo, DataLength);
			TCHARCopy(msg->MacAddress, CountArray(msg->MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			SendNetCmdToOperate(msg);
			free(msg);
			//g_FishServer.ShowInfoToWin("接收到玩家进行渠道登陆4");

			ShowInfoToWin("接收渠道登陆");

			return;
		}	
		break;
	case CL_WeiXinLogon:
		{
			//玩家试图通过微信登陆
			CL_Cmd_WeiXinLogon* pMsg = (CL_Cmd_WeiXinLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			if (!m_FishConfig.CheckVersionAndPathCrc(pMsg->VersionID, pMsg->PathCrc))
			{
				ASSERT(false);
				LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt", TEXT("客户端版本错误 微信登陆失败"));
				return;
			}

			HashMap<DWORD, DWORD>::iterator Iter = m_UserChannelMap.find(pClient->OutsideExtraData);
			if (Iter != m_UserChannelMap.end())
			{
				ASSERT(false);
				m_UserChannelMap.erase(Iter);
			}
			std::vector<char*> pVec;
			GetStringArrayVecByData(pVec, &pMsg->CodeInfo);
			if (pVec.size() != pMsg->CodeInfo.HandleSum)
			{
				ASSERT(false);
				return;
			}
			string OpenID = pVec[0];
			FreeVec(pVec);

			//生成渠道数据添加进去

			//转化为TCHAR
			UINT Count = 0;
			TCHAR* pOpenID = CharToWChar(OpenID.c_str(), Count);
			DWORD dwOpenID= GetCrc32(pOpenID);

			CL_Cmd_QueryLogon msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, CL_QueryLogon), sizeof(CL_Cmd_QueryLogon));
			//1.账号名称
			_stprintf_s(msg.AccountName, CountArray(msg.AccountName), TEXT("WeiXin%u"), dwOpenID);
			TCHAR Password[32];
			int length = _stprintf_s(Password, CountArray(Password), TEXT("WeiXinPass%u"), dwOpenID);
			for (int i = length + 1; i < 32; ++i)
				Password[i] = 0xfefe;
			free(pOpenID);

			AE_CRC_THREE pTree;
			AECrc32(pTree, Password, CountArray(Password) * sizeof(TCHAR), 0, 0x735739, 0x79387731);
			msg.PasswordCrc1 = pTree.Crc1;
			msg.PasswordCrc2 = pTree.Crc2;
			msg.PasswordCrc3 = pTree.Crc3;
			msg.PlateFormID = pMsg->PlateFormID;
			msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
			msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
			TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			msg.LogonType = 2;


			/*LC_Cmd_SaveAccountInfo msgSave;
			SetMsgInfo(msgSave, GetMsgType(Main_Logon, LC_SaveAccountInfo), sizeof(LC_Cmd_SaveAccountInfo));
			TCHARCopy(msgSave.AccountName, CountArray(msgSave.AccountName), msg.AccountName, _tcslen(msg.AccountName));
			msgSave.PasswordCrc1 = pTree.Crc1;
			msgSave.PasswordCrc2 = pTree.Crc2;
			msgSave.PasswordCrc3 = pTree.Crc3;
			SendNewCmdToClient(pClient, &msgSave);*/

			HandleClientMsg(pClient, &msg);

			//接收到渠道登陆 我们直接将命令转发到 Operater  运营服务器去
			/*DWORD Page = sizeof(LO_Cmd_WeiXinLogon)+(pMsg->CodeInfo.Sum - 1)*sizeof(BYTE);
			LO_Cmd_WeiXinLogon* msg = (LO_Cmd_WeiXinLogon*)malloc(Page);
			msg->SetCmdSize(ConvertDWORDToWORD(Page));
			msg->SetCmdType(GetMsgType(Main_Logon, LO_WeiXinLogon));
			msg->PlateFormID = pMsg->PlateFormID;
			msg->ClientID = pClient->OutsideExtraData;
			DWORD DataLength = (pMsg->CodeInfo.Sum - 1)*sizeof(BYTE)+sizeof(WeiXinUserCodeInfo);
			memcpy_s(&msg->CodeInfo, DataLength, &pMsg->CodeInfo, DataLength);
			TCHARCopy(msg->MacAddress, CountArray(msg->MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			SendNetCmdToOperate(msg);
			free(msg);
			ShowInfoToWin("接收微信登陆");*/
			return;
		}
		break;
	case CL_QQLogon:
		{
			//玩家试图通过微信登陆
			CL_Cmd_QQLogon* pMsg = (CL_Cmd_QQLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			if (!m_FishConfig.CheckVersionAndPathCrc(pMsg->VersionID, pMsg->PathCrc))
			{
				ASSERT(false);
				LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt", TEXT("客户端版本错误 QQ登陆失败"));
				return;
			}

			HashMap<DWORD, DWORD>::iterator Iter = m_UserChannelMap.find(pClient->OutsideExtraData);
			if (Iter != m_UserChannelMap.end())
			{
				ASSERT(false);
				m_UserChannelMap.erase(Iter);
			}
			std::vector<char*> pVec;
			GetStringArrayVecByData(pVec, &pMsg->TokenInfo);
			if (pVec.size() != pMsg->TokenInfo.HandleSum)
			{
				ASSERT(false);
				return;
			}
			string OpenID = pVec[0];
			FreeVec(pVec);

			//生成渠道数据添加进去

			//转化为TCHAR
			UINT Count = 0;
			TCHAR* pOpenID = CharToWChar(OpenID.c_str(), Count);
			DWORD dwOpenID = GetCrc32(pOpenID);

			CL_Cmd_QueryLogon msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, CL_QueryLogon), sizeof(CL_Cmd_QueryLogon));
			//1.账号名称
			_stprintf_s(msg.AccountName, CountArray(msg.AccountName), TEXT("QQ%u"), dwOpenID);
			TCHAR Password[32];
			int length = _stprintf_s(Password, CountArray(Password), TEXT("QQPass%u"), dwOpenID);
			for (int i = length + 1; i < 32; ++i)
				Password[i] = 0xfefe;

			free(pOpenID);

			AE_CRC_THREE pTree;
			AECrc32(pTree, Password, CountArray(Password) * sizeof(TCHAR), 0, 0x735739, 0x79387731);
			msg.PasswordCrc1 = pTree.Crc1;
			msg.PasswordCrc2 = pTree.Crc2;
			msg.PasswordCrc3 = pTree.Crc3;
			msg.PlateFormID = pMsg->PlateFormID;
			msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
			msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
			TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			msg.LogonType = 3;

			/*LC_Cmd_SaveAccountInfo msgSave;
			SetMsgInfo(msgSave, GetMsgType(Main_Logon, LC_SaveAccountInfo), sizeof(LC_Cmd_SaveAccountInfo));
			TCHARCopy(msgSave.AccountName, CountArray(msgSave.AccountName), msg.AccountName, _tcslen(msg.AccountName));
			msgSave.PasswordCrc1 = pTree.Crc1;
			msgSave.PasswordCrc2 = pTree.Crc2;
			msgSave.PasswordCrc3 = pTree.Crc3;
			SendNewCmdToClient(pClient, &msgSave);*/


			HandleClientMsg(pClient, &msg);


			//接收到渠道登陆 我们直接将命令转发到 Operater  运营服务器去
			/*DWORD Page = sizeof(LO_Cmd_QQLogon)+(pMsg->TokenInfo.Sum - 1)*sizeof(BYTE);
			LO_Cmd_QQLogon* msg = (LO_Cmd_QQLogon*)malloc(Page);
			msg->SetCmdSize(ConvertDWORDToWORD(Page));
			msg->SetCmdType(GetMsgType(Main_Logon, LO_QQLogon));
			msg->PlateFormID = pMsg->PlateFormID;
			msg->ClientID = pClient->OutsideExtraData;
			DWORD DataLength = (pMsg->TokenInfo.Sum - 1)*sizeof(BYTE)+sizeof(QQUserCodeInfo);
			memcpy_s(&msg->TokenInfo, DataLength, &pMsg->TokenInfo, DataLength);
			TCHARCopy(msg->MacAddress, CountArray(msg->MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			SendNetCmdToOperate(msg);
			free(msg);
			ShowInfoToWin("接收QQ登陆");*/
			return;
		}
		break;
	case CL_QueryLogon:
		{
			//g_FishServer.ShowInfoToWin("客户端进行快速登陆1");
			CL_Cmd_QueryLogon* pMsg = (CL_Cmd_QueryLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			if (!m_FishConfig.CheckVersionAndPathCrc(pMsg->VersionID, pMsg->PathCrc))
			{
				ASSERT(false);
				LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端版本错误 快速登陆失败 玩家账号:%s 密码:%u,%u,%u"), pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				return;
			}
			//g_FishServer.ShowInfoToWin("客户端进行快速登陆2");
			if (!m_FishConfig.CheckStringIsError(pMsg->AccountName, ACCOUNT_MIN_LENGTH, ACCOUNT_LENGTH, SCT_AccountName))
			{
				LC_Cmd_AccountOnlyID msgClient;
				SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
				msgClient.dwOnlyID = 0;
				msgClient.dwUserID = 0;
				msgClient.GameIp = 0;
				msgClient.GamePort = 0;
				msgClient.GateIp = 0;
				msgClient.GatePort = 0;
				msgClient.LogonType = 1;
				SendNewCmdToClient(pClient, &msgClient);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端账号名称错误 快速登陆失败 玩家账号:%s 密码:%u,%u,%u"), pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				return;
			}
			//g_FishServer.ShowInfoToWin("客户端进行快速登陆3");
			//先向缓存请求
			DWORD dwUserID = 0;
			bool IsFreeze = false;
			__int64 FreezeEndTime = 0;
			if (!m_LogonCacheManager.CheckAccountData(pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3, dwUserID, IsFreeze, FreezeEndTime))
			{
				//向数据库进行查询
				DBR_Cmd_QueryLogon msg;
				SetMsgInfo(msg, DBR_QueryLogon, sizeof(DBR_Cmd_QueryLogon));
				TCHARCopy(msg.AccountName, CountArray(msg.AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
				TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
				msg.ClientID = pClient->OutsideExtraData;
				msg.PasswordCrc1 = pMsg->PasswordCrc1;
				msg.PasswordCrc2 = pMsg->PasswordCrc2;
				msg.PasswordCrc3 = pMsg->PasswordCrc3;
				msg.ClientIP = pClient->IP;
				msg.LogonType = pMsg->LogonType;
				SendNetCmdToDB(&msg);
				ShowInfoToWin("发送快速登陆");

				//缓存记录
				TempAccountCacheInfo pData;
				pData.ClientID = pClient->OutsideExtraData;
				TCHARCopy(pData.AccountInfo.AccountName, CountArray(pData.AccountInfo.AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
				TCHARCopy(pData.AccountInfo.MacAddress, CountArray(pData.AccountInfo.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
				pData.AccountInfo.PasswordCrc1 = pMsg->PasswordCrc1;
				pData.AccountInfo.PasswordCrc2 = pMsg->PasswordCrc2;
				pData.AccountInfo.PasswordCrc3 = pMsg->PasswordCrc3;
				m_LogonCacheManager.OnAddTempAccountInfo(pData);

				//g_FishServer.ShowInfoToWin("客户端进行快速登陆4");
			}
			else
			{
				//缓存验证完毕了 我们可以进行处理了 
				if (dwUserID == 0)
				{
					//密码错误 无法登陆
					LC_Cmd_AccountOnlyID msgClient;
					SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
					msgClient.dwOnlyID = 0;
					msgClient.dwUserID = 0;
					msgClient.GameIp = 0;
					msgClient.GamePort = 0;
					msgClient.GateIp = 0;
					msgClient.GatePort = 0;
					msgClient.LogonType = 1;
					SendNewCmdToClient(pClient, &msgClient);

					DelClient pDel;
					pDel.LogTime = timeGetTime();
					pDel.SocketID = pClient->OutsideExtraData;
					m_DelSocketVec.push_back(pDel);

					//g_FishServer.ShowInfoToWin("客户端进行快速登陆5");

					LogInfoToFile("LogonError.txt",TEXT("客户端账号密码错误 快速登陆失败 玩家账号:%s 密码:%u,%u,%u"), pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				}
				else
				{
					if (IsFreeze)
					{
						LC_Cmd_AccountIsFreeze msg;
						SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountIsFreeze), sizeof(LC_Cmd_AccountIsFreeze));
						msg.EndTime = FreezeEndTime;
						SendNewCmdToClient(pClient, &msg);

						DelClient pDel;
						pDel.LogTime = timeGetTime();
						pDel.SocketID = pClient->OutsideExtraData;
						m_DelSocketVec.push_back(pDel);
						LogInfoToFile("LogonError.txt", TEXT("客户端已经被冻结 登陆失败"));
						return;
					}
					GameServerConfig* pConfig = g_FishServerConfig.GetGameServerConfig(m_LogonManager.GetGameServerConfigIDByUserID(dwUserID));//找到一个最合适的GameServer
					if (!pConfig)
					{
						ASSERT(false);
						return;
					}
					/*ServerClientData* pGameServer = GetServerServerData(pConfig->NetworkID);
					if (!pGameServer)
					{
						ASSERT(false);
						return;
					}*/
					if (!OnAddUserWriteGameServer(dwUserID, pClient->OutsideExtraData))
					{
						ASSERT(false);
						return;
					}
					DWORD OnlyID = RandUInt();
					//1.发送命令到GameServer去
					OnSendChannelInfoToGameServer(dwUserID, pClient->OutsideExtraData, pConfig->NetworkID);

					LG_Cmd_AccountOnlyID msgGameServer;
					SetMsgInfo(msgGameServer, GetMsgType(Main_Logon, LG_AccountOnlyID), sizeof(LG_Cmd_AccountOnlyID));
					msgGameServer.dwOnlyID = OnlyID;
					msgGameServer.dwUserID = dwUserID;
					msgGameServer.ClientID = pClient->OutsideExtraData;
					msgGameServer.GameServerID = pConfig->NetworkID;
					msgGameServer.LogonTypeID = 1;
					msgGameServer.LogonID = m_LogonConfigID;
					SendNetCmdToGameServer(pConfig->NetworkID, &msgGameServer);

					//g_FishServer.ShowInfoToWin("客户端进行快速登陆6");
				}
			}
			return;
		}
	case CL_PhoneLogon:
		{
			CL_Cmd_PhoneLogon* pMsg = (CL_Cmd_PhoneLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			//玩家试图进行手机登陆
			LO_Cmd_PhoneLogon msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, LO_PhoneLogon), sizeof(LO_Cmd_PhoneLogon));
			msg.ClientID = pClient->OutsideExtraData;
			msg.PhoneNumber = pMsg->PhoneNumber;
			SendNetCmdToOperate(&msg);
			return;
		}
		break;
	case CL_PhoneLogonCheck:
		{
			CL_Cmd_PhoneLogonCheck * pMsg = (CL_Cmd_PhoneLogonCheck*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			LO_Cmd_PhoneLogonCheck msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, LO_PhoneLogonCheck), sizeof(LO_Cmd_PhoneLogonCheck));
			msg.ClientID = pClient->OutsideExtraData;
			msg.Password = pMsg->Password;
			msg.PhoneNumber = pMsg->PhoneNumber;
			msg.ClientIP = pClient->IP;
			msg.Crc1 = pMsg->Crc1;
			msg.Crc2 = pMsg->Crc2;
			msg.Crc3 = pMsg->Crc3;
			msg.IsFirOrSec = pMsg->IsFirOrSec;
			TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress,_tcslen(pMsg->MacAddress));
			SendNetCmdToOperate(&msg);
			return;
		}
		break;
	case CL_PhoneSecPwd:
		{
			CL_Cmd_PhoneSecPwd* pMsg = (CL_Cmd_PhoneSecPwd*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			if (!m_FishConfig.CheckVersionAndPathCrc(pMsg->VersionID, pMsg->PathCrc))
			{
				ASSERT(false);
				LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt", TEXT("客户端版本错误 手机二级密码登陆失败 玩家手机号:%llu 密码:%u,%u,%u"), pMsg->PhoneNumber, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				return;
			}
			//玩家进行二级密码登陆
			TempAccountCacheInfo pData;
			pData.ClientID = pClient->OutsideExtraData;
			_i64tot_s(pMsg->PhoneNumber, pData.AccountInfo.AccountName, CountArray(pData.AccountInfo.AccountName), 10);
			TCHARCopy(pData.AccountInfo.MacAddress, CountArray(pData.AccountInfo.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			pData.AccountInfo.PasswordCrc1 = pMsg->PasswordCrc1;
			pData.AccountInfo.PasswordCrc2 = pMsg->PasswordCrc2;
			pData.AccountInfo.PasswordCrc3 = pMsg->PasswordCrc3;
			m_LogonCacheManager.OnAddTempAccountInfo(pData);

			DBR_Cmd_PhoneSecPwd msg;
			SetMsgInfo(msg, DBR_PhoneSecPwd, sizeof(DBR_Cmd_PhoneSecPwd));
			msg.ClientID = pClient->OutsideExtraData;
			msg.PhoneNumber = pMsg->PhoneNumber;
			TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
			msg.PasswordCrc1 = pMsg->PasswordCrc1;
			msg.PasswordCrc2 = pMsg->PasswordCrc2;
			msg.PasswordCrc3 = pMsg->PasswordCrc3;
			SendNetCmdToDB(&msg);
			return;
		}
		break;
	}
}

void FishServer::HandleGameServerMsg(NetCmd* pCmd)//处理GameServer发送来的命令
{
	if (!pCmd)
		return;
	if (pCmd->CmdType == Main_Logon)
	{
		switch (pCmd->SubCmdType)
		{
		case GL_AccountOnlyID:
			{
				GL_Cmd_AccountOnlyID* pMsg = (GL_Cmd_AccountOnlyID*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				ServerClientData* pClientData = GetClientServerData(pMsg->ClientID);
				if (!pClientData)
				{
					ASSERT(false);
					return;
				}
			/*	GameServerInfo* pGameServerInfo = GetGameServerInfo(pMsg->GameServerID);
				if (!pGameServerInfo)
					return;*/
				GameServerConfig* pConfig = g_FishServerConfig.GetGameServerConfig(pMsg->GameServerID);
				if (!pConfig)
				{
					ASSERT(false);
					return;
				}

				if (!OnCheckUserWriteGameServerIsExists(pMsg->dwUserID))
				{
					ASSERT(false);
					return;
				}

				LC_Cmd_AccountOnlyID msgClient;
				SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
				msgClient.dwOnlyID = pMsg->dwOnlyID;
				msgClient.dwUserID = pMsg->dwUserID;
				HashMap<DWORD, DWORD>::iterator Iter = m_UserChannelMap.find(pMsg->ClientID);
				if (Iter == m_UserChannelMap.end())
				{
					msgClient.GameIp = 0;
					msgClient.GamePort = 0;
					//msgClient.GateIp = inet_addr(pConfig->GameListenIP);
					inet_pton(AF_INET, pConfig->GameListenIP, (void *)&msgClient.GateIp);
					msgClient.GatePort = pConfig->GameListenPort;
				}
				else
				{
					DWORD ChannelID = Iter->second;
					m_UserChannelMap.erase(Iter);
					//处理数据
					GateWay* pGateWay = g_FishServerConfig.GetGateWay(ChannelID);
					if (!pGateWay)
					{
						msgClient.GameIp = 0;
						msgClient.GamePort = 0;
						//msgClient.GateIp = inet_addr(pConfig->GameListenIP);
						inet_pton(AF_INET, pConfig->GameListenIP, (void *)&msgClient.GateIp);
						msgClient.GatePort = pConfig->GameListenPort;
					}
					else
					{
						//msgClient.GameIp = inet_addr(pConfig->GameListenIP);
						inet_pton(AF_INET, pConfig->GameListenIP, (void *)&msgClient.GameIp);
						msgClient.GamePort = pConfig->GameListenPort;
						msgClient.GateIp = pGateWay->Ip;
						msgClient.GatePort = pGateWay->Port;
					}
				}
				msgClient.HallTimeOut = pConfig->TimeOut;//大厅的超时时间
				msgClient.LogonType = pMsg->LogonTypeID;
				SendNewCmdToClient(pClientData, &msgClient);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClientData->OutsideExtraData;//删除客户端
				m_DelSocketVec.push_back(pDel);
				return;
			}
			break;
		case GL_ResetPassword:
			{
				//玩家修改密码 Logon上也需要进行修改
				GL_Cmd_ResetPassword* pMsg = (GL_Cmd_ResetPassword*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				m_LogonCacheManager.OnChangeAccountPassword(pMsg->dwUserID, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				return;
			}
			break;
		case GL_ResetAccount:
			{
				//玩家修改密码 Logon上也需要进行修改
				GL_Cmd_ResetAccount* pMsg = (GL_Cmd_ResetAccount*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				m_LogonCacheManager.OnResetAccount(pMsg->dwUserID, pMsg->NewAccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);
				return;
			}
			break;
		}
	}
}
ServerClientData* FishServer::GetClientServerData(DWORD ClientID)
{
	HashMap<DWORD, ServerClientData*>::iterator IterFind = m_ClintList.find(ClientID);
	if (IterFind == m_ClintList.end() || !IterFind->second)
		return NULL;
	return IterFind->second;
}
//ServerClientData* FishServer::GetServerServerData(BYTE ServerID)
//{
//	HashMap<BYTE, ServerClientData*>::iterator IterFind = m_ServerList.find(ServerID);
//	if (IterFind == m_ServerList.end() || !IterFind->second)
//		return NULL;
//	return IterFind->second;
//}
void FishServer::OnLogonDBDataSucess()
{
	/*if ( !ConnectClient())
	{
		ASSERT(false);
		return;
	}*/
	while (!ConnectClient())
	{
		Sleep(5);
	}
	ShowInfoToWin("LogonServer 初始化成功");
	return;
}
//GameServerInfo* FishServer::GetGameServerInfo(DWORD GameServerID)
//{
//	HashMap<DWORD, GameServerInfo*>::iterator IterFind = m_GameServerOnlineSumMap.find(GameServerID);
//	if (IterFind == m_GameServerOnlineSumMap.end())
//		return NULL;
//	return IterFind->second;
//}
//ServerClientData* FishServer::GetMaxFreeGameServer()
//{
//	//获取最闲的GameServer
//	if (m_GameServerOnlineSumMap.empty())
//		return NULL;
//
//	DWORD pClientID = 0;
//	WORD PlayerSum = 65535;//一个GameServer不可能有怎么多人 所以相当于最大值
//
//	HashMap<DWORD, GameServerInfo*>::iterator Iter = m_GameServerOnlineSumMap.begin();
//	for (; Iter != m_GameServerOnlineSumMap.end(); ++Iter)
//	{
//		if (Iter->second->OnlineSum < PlayerSum)
//		{
//			pClientID = Iter->first;
//			PlayerSum = Iter->second->OnlineSum;
//		}
//	}
//	if (pClientID == 0)
//		return NULL;
//	return GetServerServerData(pClientID);
//}
void FishServer::HandleDataBaseMsg(NetCmd* pCmd)
{
	if (!pCmd)
		return;
	switch (pCmd->GetCmdType())
	{
	case DBO_AccountLogon:
		{
			DBO_Cmd_AccountLogon* pMsg = (DBO_Cmd_AccountLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			ServerClientData* pClient = GetClientServerData(pMsg->ClientID);//客户端 
			if (!pClient)
			{
				ASSERT(false);
				return;
			}
			if (pMsg->dwUserID == 0)
			{
				LC_Cmd_AccountOnlyID msgClient;
				SetMsgInfo(msgClient,GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
				msgClient.dwOnlyID = 0;
				msgClient.dwUserID = 0;
				msgClient.GameIp = 0;
				msgClient.GamePort = 0;
				msgClient.GateIp = 0;
				msgClient.GatePort = 0;
				msgClient.LogonType = 1;
				SendNewCmdToClient(pClient, &msgClient);
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->dwUserID, false);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端账号密码错误 数据库普通登陆失败"));
				return;
			}
			else if (pMsg->IsFreeze)
			{
				//当前账号已经被冻结了 需要处理
				LC_Cmd_AccountIsFreeze msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountIsFreeze), sizeof(LC_Cmd_AccountIsFreeze));
				msg.EndTime = pMsg->FreezeEndTime;
				SendNewCmdToClient(pClient, &msg);
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->dwUserID, false);
				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt", TEXT("客户端已经被冻结 登陆失败"));
				return;
			}
			/*ServerClientData* pGameServer = GetMaxFreeGameServer();
			if (!pGameServer)
				return;*/

			m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->dwUserID, true);

			GameServerConfig* pConfig = g_FishServerConfig.GetGameServerConfig(m_LogonManager.GetGameServerConfigIDByUserID(pMsg->dwUserID));//找到一个最合适的GameServer
			if (!pConfig)
			{
				ASSERT(false);
				return;
			}
			/*ServerClientData* pGameServer = GetServerServerData(pConfig->NetworkID);
			if (!pGameServer)
			{
				ASSERT(false);
				return;
			}*/
			if (!OnAddUserWriteGameServer(pMsg->dwUserID, pClient->OutsideExtraData))
			{
				ASSERT(false);
				return;
			}
			DWORD OnlyID = RandUInt();
			//1.发送命令到GameServer去
			OnSendChannelInfoToGameServer(pMsg->dwUserID, pClient->OutsideExtraData, pConfig->NetworkID);

			LG_Cmd_AccountOnlyID msgGameServer;
			SetMsgInfo(msgGameServer,GetMsgType(Main_Logon, LG_AccountOnlyID), sizeof(LG_Cmd_AccountOnlyID));
			msgGameServer.dwOnlyID = OnlyID;
			msgGameServer.dwUserID = pMsg->dwUserID;
			msgGameServer.ClientID = pClient->OutsideExtraData;
			msgGameServer.GameServerID = pConfig->NetworkID;
			msgGameServer.LogonTypeID = 1;
			msgGameServer.LogonID = m_LogonConfigID;
			SendNetCmdToGameServer(pConfig->NetworkID, &msgGameServer);
			return;
		}
		break;
	case DBO_AccountRsg:
		{
			DBO_Cmd_AccountRsg* pMsg = (DBO_Cmd_AccountRsg*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			ServerClientData* pClient = GetClientServerData(pMsg->ClientID);
			if (!pClient)//玩家已经离线了 无法处理了
			{
				ASSERT(false);
				return;
			}
			if (pMsg->dwUserID == 0)
			{
				LC_Cmd_AccountOnlyID msgClient;
				SetMsgInfo(msgClient,GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
				msgClient.dwOnlyID = 0;
				msgClient.dwUserID = pMsg->dwUserID;
				msgClient.GameIp = 0;
				msgClient.GamePort = 0;
				msgClient.GateIp = 0;
				msgClient.GatePort = 0;
				msgClient.LogonType = 2;
				SendNewCmdToClient(pClient, &msgClient);

				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->dwUserID, false);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt", TEXT("客户端账号密码错误 数据库普通注册失败"));
				return;
			}
			m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->dwUserID, true);

			GameServerConfig* pConfig = g_FishServerConfig.GetGameServerConfig(m_LogonManager.GetGameServerConfigIDByUserID(pMsg->dwUserID));
			if (!pConfig)
			{
				ASSERT(false);
				return;
			}
			/*ServerClientData* pGameServer = GetServerServerData(pConfig->NetworkID);
			if (!pGameServer)
			{
				ASSERT(false);
				return;
			}*/
			if (!OnAddUserWriteGameServer(pMsg->dwUserID, pClient->OutsideExtraData))
			{
				ASSERT(false);
				return;
			}
			DWORD OnlyID = RandUInt();
			//1.发送命令到GameServer去
			OnSendChannelInfoToGameServer(pMsg->dwUserID, pClient->OutsideExtraData, pConfig->NetworkID);

			LG_Cmd_AccountOnlyID msgGameServer;
			SetMsgInfo(msgGameServer,GetMsgType(Main_Logon, LG_AccountOnlyID), sizeof(LG_Cmd_AccountOnlyID));
			msgGameServer.dwOnlyID = OnlyID;
			msgGameServer.dwUserID = pMsg->dwUserID;
			msgGameServer.ClientID = pClient->OutsideExtraData;
			msgGameServer.GameServerID = pConfig->NetworkID;
			msgGameServer.LogonTypeID = 2;
			msgGameServer.LogonID = m_LogonConfigID;
			SendNetCmdToGameServer(pConfig->NetworkID, &msgGameServer);

			return;
		}
		break;
	case DBO_GetNewAccount:
		{
			DBO_Cmd_GetNewAccount* pMsg = (DBO_Cmd_GetNewAccount*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			ServerClientData* pClient = GetClientServerData(pMsg->ClientID);
			if (!pClient)
			{
				ASSERT(false);
				return;
			}
			if (pMsg->Result)
				m_LogonCacheManager.OnAddAccountInfo(pMsg->dwUserID, pMsg->AccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3);//将已经创建的账号保存起来
			else
			{
				LogInfoToFile("LogonError.txt",TEXT("客户端账号密码错误 数据库获取新账号失败"));
			}
			//将命令转发到客户端去
			LC_Cmd_GetNewAccount msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, LC_GetNewAccount), sizeof(LC_Cmd_GetNewAccount));
			msg.Result = pMsg->Result;
			if (msg.Result)
			{
				TCHARCopy(msg.NewAccountName, CountArray(msg.NewAccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
				msg.PasswordCrc1 = pMsg->PasswordCrc1;
				msg.PasswordCrc2 = pMsg->PasswordCrc2;
				msg.PasswordCrc3 = pMsg->PasswordCrc3;
			}
			SendNewCmdToClient(pClient, &msg);
			if (pMsg->Result)
			{
				CL_Cmd_AccountLogon msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, CL_AccountLogon), sizeof(CL_Cmd_AccountLogon));
				TCHARCopy(msg.AccountName, CountArray(msg.AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
				msg.PasswordCrc1 = pMsg->PasswordCrc1;
				msg.PasswordCrc2 = pMsg->PasswordCrc2;
				msg.PasswordCrc3 = pMsg->PasswordCrc3;
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				HandleClientMsg(pClient, &msg);
			}
			return;
		}
		break;
	case DBO_LoadAllAcoountInfo:
		{
			DBO_Cmd_LoadAllAccountInfo* pMsg = (DBO_Cmd_LoadAllAccountInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			m_LogonCacheManager.OnLoadAllAccountDataResult(pMsg);
			return;
		}
		break;
	case DBO_QueryLogon:
		{
			//玩家进行快速登陆的返回结果
			DBO_Cmd_QueryLogon* pMsg = (DBO_Cmd_QueryLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			ServerClientData* pClient = GetClientServerData(pMsg->ClientID);//客户端 
			if (!pClient)
			{
				ASSERT(false);
				return;
			}
			//已经获取到账号了 我们进行登陆处理
			//1.将账号数据缓存起来
			if (pMsg->dwUserID == 0)
			{
				LC_Cmd_AccountOnlyID msgClient;
				SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
				msgClient.dwOnlyID = 0;
				msgClient.dwUserID = 0;
				msgClient.GameIp = 0;
				msgClient.GamePort = 0;
				msgClient.GateIp = 0;
				msgClient.GatePort = 0;
				msgClient.LogonType = 1;
				SendNewCmdToClient(pClient, &msgClient);

				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->dwUserID, false);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端账号密码错误 数据库快速登陆失败"));
				return;
			}
			else if (pMsg->IsFreeze)
			{
				//当前账号已经被冻结了 需要处理
				LC_Cmd_AccountIsFreeze msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountIsFreeze), sizeof(LC_Cmd_AccountIsFreeze));
				msg.EndTime = pMsg->FreezeEndTime;
				SendNewCmdToClient(pClient, &msg);
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->dwUserID, false);
				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt", TEXT("客户端已经被冻结 登陆失败"));
				return;
			}
			m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->dwUserID, true);
			GameServerConfig* pConfig = g_FishServerConfig.GetGameServerConfig(m_LogonManager.GetGameServerConfigIDByUserID(pMsg->dwUserID));//找到一个最合适的GameServer
			if (!pConfig)
			{
				ASSERT(false);
				return;
			}
			/*ServerClientData* pGameServer = GetServerServerData(pConfig->NetworkID);
			if (!pGameServer)
			{
				ASSERT(false);
				return;
			}*/
			if (!OnAddUserWriteGameServer(pMsg->dwUserID, pClient->OutsideExtraData))
			{
				ASSERT(false);
				return;
			}
			DWORD OnlyID = RandUInt();
			//1.发送命令到GameServer去
			OnSendChannelInfoToGameServer(pMsg->dwUserID, pClient->OutsideExtraData, pConfig->NetworkID);

			LG_Cmd_AccountOnlyID msgGameServer;
			SetMsgInfo(msgGameServer, GetMsgType(Main_Logon, LG_AccountOnlyID), sizeof(LG_Cmd_AccountOnlyID));
			msgGameServer.dwOnlyID = OnlyID;
			msgGameServer.dwUserID = pMsg->dwUserID;
			msgGameServer.ClientID = pClient->OutsideExtraData;
			msgGameServer.GameServerID = pConfig->NetworkID;
			msgGameServer.LogonTypeID = 1;
			msgGameServer.LogonID = m_LogonConfigID;
			SendNetCmdToGameServer(pConfig->NetworkID, &msgGameServer);

			if (pMsg->IsNewRsg)
			{
				LC_Cmd_RsgNewAccount msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_RsgNewAccount), sizeof(LC_Cmd_RsgNewAccount));
				msg.LogonType = pMsg->LogonType;
				SendNewCmdToClient(pClient, &msg);//告诉客户端 当前账号是新注册的
			}
			return;
							  
		}
		break;
	case DBO_PhoneSecPwd:
		{
			DBO_Cmd_PhoneSecPwd* pMsg = (DBO_Cmd_PhoneSecPwd*)pCmd;
			//玩家进行手机二级密码登陆结果
			ServerClientData* pClient = GetClientServerData(pMsg->ClientID);
			if (!pClient)//玩家已经离线了 无法处理了
			{
				ASSERT(false);
				return;
			}
			if (pMsg->dwUserID == 0)
			{
				//提示玩家手机登陆失败了
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->AccountName, pMsg->dwUserID, false);

				LC_Cmd_AccountOnlyID msgClient;
				SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
				msgClient.dwOnlyID = 0;
				msgClient.dwUserID = 0;
				msgClient.GameIp = 0;
				msgClient.GamePort = 0;
				msgClient.GateIp = 0;
				msgClient.GatePort = 0;
				msgClient.LogonType = 1;
				SendNewCmdToClient(pClient, &msgClient);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt", TEXT("手机二级密码登陆错误"));
				return;
			}
			else if (pMsg->IsFreeze)
			{
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->AccountName, pMsg->dwUserID, false);

				//当前账号已经被冻结了 需要处理
				LC_Cmd_AccountIsFreeze msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountIsFreeze), sizeof(LC_Cmd_AccountIsFreeze));
				msg.EndTime = pMsg->FreezeEndTime;
				SendNewCmdToClient(pClient, &msg);
				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt", TEXT("客户端已经被冻结 登陆失败"));
				return;
			}
			GameServerConfig* pConfig = g_FishServerConfig.GetGameServerConfig(m_LogonManager.GetGameServerConfigIDByUserID(pMsg->dwUserID));//找到一个最合适的GameServer
			if (!pConfig)
			{
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->AccountName, pMsg->dwUserID, false);

				ASSERT(false);
				return;
			}
			if (!OnAddUserWriteGameServer(pMsg->dwUserID, pClient->OutsideExtraData))
			{
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->AccountName, pMsg->dwUserID, false);

				ASSERT(false);
				return;
			}
			DWORD OnlyID = RandUInt();
			//1.发送命令到GameServer去
			OnSendChannelInfoToGameServer(pMsg->dwUserID, pClient->OutsideExtraData, pConfig->NetworkID);

			m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->AccountName, pMsg->dwUserID, true);

			LG_Cmd_AccountOnlyID msgGameServer;
			SetMsgInfo(msgGameServer, GetMsgType(Main_Logon, LG_AccountOnlyID), sizeof(LG_Cmd_AccountOnlyID));
			msgGameServer.dwOnlyID = OnlyID;
			msgGameServer.dwUserID = pMsg->dwUserID;
			msgGameServer.ClientID = pClient->OutsideExtraData;
			msgGameServer.GameServerID = pConfig->NetworkID;
			msgGameServer.LogonTypeID = 1;
			msgGameServer.LogonID = m_LogonConfigID;
			SendNetCmdToGameServer(pConfig->NetworkID, &msgGameServer);

			//设置账号进入缓存 设置账号已经使用了
			//m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->dwUserID, true);

			return;
		}
		break;
	case DBO_PhoneLogon:
		{
			DBO_Cmd_PhoneLogon* pMsg = (DBO_Cmd_PhoneLogon*)pCmd;
			ServerClientData* pClient = GetClientServerData(pMsg->ClientID);
			if (!pClient)//玩家已经离线了 无法处理了
			{
				ASSERT(false);
				return;
			}
			//玩家进行随机登陆的结果处理
			if (pMsg->dwUserID == 0)
			{
				//无法登陆 UserID 为 0 表示登陆失败了
				LC_Cmd_PhoneLogonCheck msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_PhoneLogonCheck), sizeof(LC_Cmd_PhoneLogonCheck));
				msg.dwUserID = 0;
				msg.Result = false;
				msg.ErrorID = PLE_SystemError;
				SendNewCmdToClient(pClient, &msg);
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->AccountName, pMsg->dwUserID, false);
				LogInfoToFile("LogonError.txt", TEXT("客户端手机号码登陆 UserID为0 登陆失败"));
				return;
			}
			else if (pMsg->IsFreeze)
			{
				LC_Cmd_SaveAccountInfo msgSave;
				SetMsgInfo(msgSave, GetMsgType(Main_Logon, LC_SaveAccountInfo), sizeof(LC_Cmd_SaveAccountInfo));
				TCHARCopy(msgSave.AccountName, CountArray(msgSave.AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
				msgSave.PasswordCrc1 = pMsg->PasswordCrc1;
				msgSave.PasswordCrc2 = pMsg->PasswordCrc2;
				msgSave.PasswordCrc3 = pMsg->PasswordCrc3;
				SendNewCmdToClient(pClient, &msgSave);

				//账号已经被冻结了
				LC_Cmd_AccountIsFreeze msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountIsFreeze), sizeof(LC_Cmd_AccountIsFreeze));
				msg.EndTime = pMsg->FreezeEndTime;
				SendNewCmdToClient(pClient, &msg);
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->AccountName, pMsg->dwUserID, false);
				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt", TEXT("客户端已经被冻结 登陆失败"));
				return;
			}
			else
			{
				//将账号数据发送到客户端去 保存起来
				LC_Cmd_SaveAccountInfo msgSave;
				SetMsgInfo(msgSave, GetMsgType(Main_Logon, LC_SaveAccountInfo), sizeof(LC_Cmd_SaveAccountInfo));
				TCHARCopy(msgSave.AccountName, CountArray(msgSave.AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
				msgSave.PasswordCrc1 = pMsg->PasswordCrc1;
				msgSave.PasswordCrc2 = pMsg->PasswordCrc2;
				msgSave.PasswordCrc3 = pMsg->PasswordCrc3;
				SendNewCmdToClient(pClient, &msgSave);

				//根据UserID 让玩家进行正常登陆
				m_LogonCacheManager.OnCheckTempAccountInfo(pClient->OutsideExtraData, pMsg->AccountName, pMsg->dwUserID, true);
				GameServerConfig* pConfig = g_FishServerConfig.GetGameServerConfig(m_LogonManager.GetGameServerConfigIDByUserID(pMsg->dwUserID));
				if (!pConfig)
				{
					ASSERT(false);
					return;
				}
				if (!OnAddUserWriteGameServer(pMsg->dwUserID, pClient->OutsideExtraData))
				{
					ASSERT(false);
					return;
				}
				DWORD OnlyID = RandUInt();
				//1.发送命令到GameServer去
				OnSendChannelInfoToGameServer(pMsg->dwUserID, pClient->OutsideExtraData, pConfig->NetworkID);

				LG_Cmd_AccountOnlyID msgGameServer;
				SetMsgInfo(msgGameServer, GetMsgType(Main_Logon, LG_AccountOnlyID), sizeof(LG_Cmd_AccountOnlyID));
				msgGameServer.dwOnlyID = OnlyID;
				msgGameServer.dwUserID = pMsg->dwUserID;
				msgGameServer.ClientID = pClient->OutsideExtraData;
				msgGameServer.GameServerID = pConfig->NetworkID;
				msgGameServer.LogonTypeID = 2;
				msgGameServer.LogonID = m_LogonConfigID;
				SendNetCmdToGameServer(pConfig->NetworkID, &msgGameServer);

				return;
			}
		}
		break;
	}
}
void FishServer::HandleCenterMsg(NetCmd* pCmd)
{
	if (!pCmd)
		return;
	//接收来自中央服务器的命令
	if (pCmd->CmdType == Main_Server)
	{
		switch (pCmd->SubCmdType)
		{
		case CL_RsgLogon:
			{
				CL_Cmd_RsgLogon* pMsg = (CL_Cmd_RsgLogon*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				m_LogonManager.OnLogonRsgResult(pMsg);
				return;
			}
			break;
		case CL_UnRsgGame:
			{
				CL_Cmd_UnRsgGame* pMsg = (CL_Cmd_UnRsgGame*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				/*	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ServerList.find(pMsg->GameConfigID);
				if (Iter != m_ServerList.end())
				{
					Iter->second->Removed = true;
				}*/
				m_LogonManager.OnGameLeaveCenter(pMsg->GameConfigID);
				return;
			}
			break;
		case CL_RsgUser:
			{
				CL_Cmd_RsgUser* pMsg = (CL_Cmd_RsgUser*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				m_LogonManager.OnUserJoinCenter(pMsg->PlayerInfo.dwUserID, pMsg->PlayerInfo.GameConfigID);
				return;
			}
			break;
		case CL_UnRsgUser:
			{
				CL_Cmd_UnRsgUser* pMsg = (CL_Cmd_UnRsgUser*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				m_LogonManager.OnUserLeaveCenter(pMsg->dwUserID);
				return;
			}
			break;
		case CL_GameInfo:
			{
				CL_Cmd_GameInfo* pMsg = (CL_Cmd_GameInfo*)pCmd;
				for (WORD i = 0; i < pMsg->Sum; ++i)
				{
					m_LogonManager.OnGameRsgLogon(pMsg->Array[i]);
				}
				return;
			}
		}
	}
	HandleGameServerMsg(pCmd);//处理中央转发的GameServer的命令
	return;
}
void FishServer::CheckDelSocket(DWORD dwTimer)
{
	if (m_DelSocketVec.empty())
		return;
	vector<DelClient>::iterator Iter = m_DelSocketVec.begin();
	for (; Iter != m_DelSocketVec.end();)
	{
		if (dwTimer - Iter->LogTime >= 50000)
		{
			CloseClientSocket(Iter->SocketID);
			Iter = m_DelSocketVec.erase(Iter);
		}
		else
			++Iter;
	}
}
void FishServer::CloseClientSocket(DWORD SocketID)
{
	ServerClientData* pClient = GetClientServerData(SocketID);
	if (pClient)
		pClient->Removed = true;
	else
	{
		ASSERT(false);
		return;
	}
}
void FishServer::HandleOperateMsg(NetCmd* pCmd)
{
	//处理运营服务器发送来的消息
	if (!pCmd || pCmd->CmdType != Main_Logon)
	{
		ASSERT(false);
		return;
	}
	switch (pCmd->SubCmdType)
	{
	case OL_WeiXinLogon:
		{
			OL_Cmd_WeiXinLogon* pMsg = (OL_Cmd_WeiXinLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			ServerClientData * pClient = GetClientServerData(pMsg->ClientID);
			if (!pClient)
			{
				ASSERT(false);
				return;
			}
			if (pMsg->Result)
			{
				//添加玩家渠道数据 为WeiXin
				HashMap<DWORD, DWORD>::iterator Iter = m_UserChannelMap.find(pMsg->ClientID);
				if (Iter != m_UserChannelMap.end())
				{
					ASSERT(false);
					m_UserChannelMap.erase(Iter);
				}
				std::vector<char*> pVec;
				GetStringArrayVecByData(pVec, &pMsg->OpenInfo);
				if (pVec.size() != pMsg->OpenInfo.HandleSum)
				{
					ASSERT(false);
					return;
				}
				string OpenID = pVec[0];
				FreeVec(pVec);

				//生成渠道数据添加进去

				//转化为TCHAR
				UINT Count = 0;
				TCHAR* pOpenID=CharToWChar(OpenID.c_str(), Count);


				CL_Cmd_QueryLogon msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, CL_QueryLogon), sizeof(CL_Cmd_QueryLogon));
				//1.账号名称
				_stprintf_s(msg.AccountName, CountArray(msg.AccountName), TEXT("WeiXin%s"), pOpenID);
				TCHAR Password[32];
				int length = _stprintf_s(Password, CountArray(Password), TEXT("WeiXinPass%s"),pOpenID);
				for (int i = length + 1; i < 32; ++i)
					Password[i] = 0xfefe;

				free(pOpenID);

				AE_CRC_THREE pTree;
				AECrc32(pTree, Password, CountArray(Password) * sizeof(TCHAR), 0, 0x735739, 0x79387731);
				msg.PasswordCrc1 = pTree.Crc1;
				msg.PasswordCrc2 = pTree.Crc2;
				msg.PasswordCrc3 = pTree.Crc3;
				msg.PlateFormID = pMsg->PlateFormID;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
				msg.LogonType = 2;
				HandleClientMsg(pClient, &msg);
			}
			else
			{
				//将服务器检查的结果发送到客户端去
				LC_Cmd_WeiXinLogon msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_WeiXinLogon), sizeof(LC_Cmd_WeiXinLogon));
				msg.Result = pMsg->Result;
				SendNewCmdToClient(pClient, &msg);
				LogInfoToFile("LogonError.txt", TEXT("客户端微信登陆失败"));
			}
			return;
		}
		break;
	case OL_QQLogon:
		{
			OL_Cmd_QQLogon* pMsg = (OL_Cmd_QQLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			ServerClientData * pClient = GetClientServerData(pMsg->ClientID);
			if (!pClient)
			{
				ASSERT(false);
				return;
			}
			if (pMsg->Result)
			{
				//添加玩家渠道数据 为QQ
				HashMap<DWORD, DWORD>::iterator Iter = m_UserChannelMap.find(pMsg->ClientID);
				if (Iter != m_UserChannelMap.end())
				{
					ASSERT(false);
					m_UserChannelMap.erase(Iter);
				}
				std::vector<char*> pVec;
				GetStringArrayVecByData(pVec, &pMsg->OpenInfo);
				if (pVec.size() != pMsg->OpenInfo.HandleSum)
				{
					ASSERT(false);
					return;
				}
				string OpenID = pVec[0];
				FreeVec(pVec);

				//生成渠道数据添加进去

				//转化为TCHAR
				UINT Count = 0;
				TCHAR* pOpenID=CharToWChar(OpenID.c_str(), Count);


				CL_Cmd_QueryLogon msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, CL_QueryLogon), sizeof(CL_Cmd_QueryLogon));
				//1.账号名称
				_stprintf_s(msg.AccountName, CountArray(msg.AccountName), TEXT("QQ%s"), pOpenID);
				TCHAR Password[32];
				int length = _stprintf_s(Password, CountArray(Password), TEXT("QQPass%s"),pOpenID);
				for (int i = length + 1; i < 32; ++i)
					Password[i] = 0xfefe;

				free(pOpenID);

				AE_CRC_THREE pTree;
				AECrc32(pTree, Password, CountArray(Password) * sizeof(TCHAR), 0, 0x735739, 0x79387731);
				msg.PasswordCrc1 = pTree.Crc1;
				msg.PasswordCrc2 = pTree.Crc2;
				msg.PasswordCrc3 = pTree.Crc3;
				msg.PlateFormID = pMsg->PlateFormID;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
				msg.LogonType = 3;
				HandleClientMsg(pClient, &msg);

			}
			else
			{
				//将服务器检查的结果发送到客户端去
				LC_Cmd_QQLogon msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_QQLogon), sizeof(LC_Cmd_QQLogon));
				msg.Result = pMsg->Result;
				SendNewCmdToClient(pClient, &msg);
				LogInfoToFile("LogonError.txt", TEXT("客户端QQ登陆失败"));
			}
			return;
		}
		break;
	case OL_ChannelLogon:
		{
			//g_FishServer.ShowInfoToWin("接收到玩家进行渠道结果返回1");
			OL_Cmd_ChannelLogon* pMsg = (OL_Cmd_ChannelLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			ServerClientData * pClient = GetClientServerData(pMsg->ClientID);
			if (!pClient)
			{
				ASSERT(false);
				return;
			}
			//g_FishServer.ShowInfoToWin("接收到玩家进行渠道结果返回2");
			//接收到命令 我们进行处理
			if (pMsg->Result)
			{
				OnAddChannelInfo(pMsg->ClientID, &pMsg->channelInfo);//添加到渠道数据去

				HashMap<DWORD, DWORD>::iterator Iter = m_UserChannelMap.find(pMsg->ClientID);
				if (Iter != m_UserChannelMap.end())
				{
					ASSERT(false);
					m_UserChannelMap.erase(Iter);
				}
				m_UserChannelMap.insert(HashMap<DWORD, DWORD>::value_type(pMsg->ClientID, pMsg->ChannelID));

				std::vector<StringArrayData*> pVec;
				GetStringArrayVecByData(pVec, &pMsg->channelInfo);
				if (pVec.size() != pMsg->channelInfo.HandleSum)
				{
					ASSERT(false);
					return;
				}
				//生成一个账号信息  然后进行快速登陆(有账号就登陆 无账号 注册账号登陆 )
				CL_Cmd_QueryLogon msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, CL_QueryLogon), sizeof(CL_Cmd_QueryLogon));
				
				//1.账号名称
				DWORD uidCrc32 = GetCrc32(pVec[0]->Array, pVec[0]->Length/sizeof(TCHAR));
				DWORD channelLabelCrc32 = GetCrc32(pVec[1]->Array, pVec[1]->Length / sizeof(TCHAR));//不可能变化的
				DWORD channeluidCrc32 = GetCrc32(pVec[2]->Array, pVec[2]->Length / sizeof(TCHAR));//不可能变化的
				_stprintf_s(msg.AccountName, CountArray(msg.AccountName), TEXT("ChannelUser%u%u"), uidCrc32,channelLabelCrc32);
				TCHAR Password[32];
				int length = _stprintf_s(Password, CountArray(Password), TEXT("Pass%u%u"), uidCrc32, channeluidCrc32);
				for (int i = length+1; i < 32; ++i)
					Password[i] = 0xfefe;

				AE_CRC_THREE pTree;
				AECrc32(pTree, Password, CountArray(Password) * sizeof(TCHAR), 0, 0x735739, 0x79387731);
				msg.PasswordCrc1 = pTree.Crc1;
				msg.PasswordCrc2 = pTree.Crc2;
				msg.PasswordCrc3 = pTree.Crc3;
				msg.PlateFormID = pMsg->PlateFormID;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
				msg.LogonType = 1;
				HandleClientMsg(pClient, &msg);

				//Log(TEXT("渠道登录 用户名:%s Password:%s  %u  %u  %u"), msg.AccountName, Password, msg.PasswordCrc1, msg.PasswordCrc2, msg.PasswordCrc3);

				/*vector<TCHAR*>::iterator Iter = pVec.begin();
				for (; Iter != pVec.end(); ++Iter)
				{
					free(*Iter);
				}
				pVec.clear();*/

				//g_FishServer.ShowInfoToWin("接收到玩家进行渠道结果返回4");
			}
			else
			{
				//将服务器检查的结果发送到客户端去
				LC_Cmd_ChannelLogon msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_ChannelLogon), sizeof(LC_Cmd_ChannelLogon));
				msg.Result = pMsg->Result;
				SendNewCmdToClient(pClient, &msg);

				LogInfoToFile("LogonError.txt",TEXT("客户端渠道登陆失败"));

				//g_FishServer.ShowInfoToWin("接收到玩家进行渠道结果返回5");
			}
			return;
		}
		break;
	case OL_PhoneLogon:
		{
			OL_Cmd_PhoneLogon* pMsg = (OL_Cmd_PhoneLogon*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			ServerClientData* pClient = GetClientServerData(pMsg->ClientID);
			if (!pClient)
			{
				ASSERT(false);
				return;
			}
			//玩家进行获取手机登陆密码的结果
			LC_Cmd_PhoneLogon msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, LC_PhoneLogon), sizeof(LC_Cmd_PhoneLogon));
			msg.ErrorID = pMsg->ErrorID;
			msg.PhoneNumber = pMsg->PhoneNumber;
			SendNewCmdToClient(pClient, &msg);
			return;
		}
		break;
	case OL_PhoneLogonCheck:
		{
			OL_Cmd_PhoneLogonCheck* pMsg = (OL_Cmd_PhoneLogonCheck*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return;
			}
			ServerClientData* pClient = GetClientServerData(pMsg->ClientID);
			if (!pClient)
			{
				ASSERT(false);
				return;
			}
			if (pMsg->Result)
			{
				//将玩家数据加入到缓存去
				TempAccountCacheInfo pData;
				pData.ClientID = pMsg->ClientID;
				_i64tot_s(pMsg->PhoneNumber, pData.AccountInfo.AccountName, CountArray(pData.AccountInfo.AccountName), 10);
				TCHARCopy(pData.AccountInfo.MacAddress, CountArray(pData.AccountInfo.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
				pData.AccountInfo.PasswordCrc1 = pMsg->Crc1;
				pData.AccountInfo.PasswordCrc2 = pMsg->Crc2;
				pData.AccountInfo.PasswordCrc3 = pMsg->Crc3;
				m_LogonCacheManager.OnAddTempAccountInfo(pData);

				DBR_Cmd_PhoneLogon msg;
				SetMsgInfo(msg, DBR_PhoneLogon, sizeof(DBR_Cmd_PhoneLogon));
				msg.ClientID = pMsg->ClientID;
				msg.PhoneNumber = pMsg->PhoneNumber;
				msg.ClientIP = pMsg->ClientIP;
				msg.PasswordCrc1 = pMsg->Crc1;
				msg.PasswordCrc2 = pMsg->Crc2;
				msg.PasswordCrc3 = pMsg->Crc3;
				msg.IsFirOrSec = pMsg->IsFirOrSec;
				TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
				SendNetCmdToDB(&msg);
				return;
			}
			else
			{
				LC_Cmd_PhoneLogonCheck msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_PhoneLogonCheck), sizeof(LC_Cmd_PhoneLogonCheck));
				msg.dwUserID = 0;
				msg.ErrorID = pMsg->ErrorID;
				msg.Result = pMsg->Result;
				SendNewCmdToClient(pClient, &msg);
				return;
			}
		}
		break;
	}
}
void FishServer::OnAddChannelInfo(DWORD ClientID, ChannelUserInfo* pInfo)
{
	HashMap<DWORD, ChannelUserInfo*>::iterator Iter = m_ChannelInfo.find(ClientID);
	if (Iter != m_ChannelInfo.end())
	{
		free(Iter->second);
		m_ChannelInfo.erase(Iter);
	}
	//进行添加
	DWORD InfoSize = sizeof(ChannelUserInfo)+sizeof(BYTE)*(pInfo->Sum - 1);
	ChannelUserInfo* pNewInfo = (ChannelUserInfo*)malloc(InfoSize);
	memcpy_s(pNewInfo, InfoSize, pInfo, InfoSize);
	m_ChannelInfo.insert(HashMap<DWORD, ChannelUserInfo*>::value_type(ClientID, pNewInfo));
}
void FishServer::OnDelChannelInfo(DWORD ClientID)
{
	HashMap<DWORD, ChannelUserInfo*>::iterator Iter = m_ChannelInfo.find(ClientID);
	if (Iter != m_ChannelInfo.end())
	{
		free(Iter->second);
		m_ChannelInfo.erase(Iter);
	}
}
void FishServer::OnSendChannelInfoToGameServer(DWORD dwUserID, DWORD ClientID, BYTE GameServerID)
{
	/*ServerClientData* pServer = GetServerServerData(GameServerID);
	if (!pServer)
		return;*/
	HashMap<DWORD, ChannelUserInfo*>::iterator Iter = m_ChannelInfo.find(ClientID);
	if (Iter != m_ChannelInfo.end())
	{
		//查找到了 进行发送
		DWORD PageSize = sizeof(LG_Cmd_ChannelInfo)+sizeof(BYTE)*(Iter->second->Sum - 1);
		DWORD InfoSize = sizeof(ChannelUserInfo)+sizeof(BYTE)*(Iter->second->Sum - 1);
		LG_Cmd_ChannelInfo * msg = (LG_Cmd_ChannelInfo*)malloc(PageSize);
		CheckMsgSize(PageSize);
		msg->SetCmdSize(static_cast<WORD>(PageSize));
		msg->SetCmdType(GetMsgType(Main_Logon, LG_ChannelInfo));
		msg->ClientID = ClientID;
		msg->dwUserID = dwUserID;
		memcpy_s(&msg->channelUserInfo, InfoSize, Iter->second, InfoSize);
		SendNetCmdToGameServer(GameServerID, msg);
		free(msg);
		free(Iter->second);
		m_ChannelInfo.erase(Iter);
	}
}
void FishServer::OnClearAllChannelInfo()
{
	if (m_ChannelInfo.empty())
		return;
	HashMap<DWORD, ChannelUserInfo*>::iterator Iter = m_ChannelInfo.begin();
	for (; Iter != m_ChannelInfo.end(); ++Iter)
	{
		free(Iter->second);
	}
	m_ChannelInfo.clear();
}
bool FishServer::OnAddUserWriteGameServer(DWORD dwUserID,DWORD ClientID)
{
	HashMap<DWORD, WriteGameServerUserInfo>::iterator Iter = m_SendGameUserMap.find(dwUserID);
	if (Iter == m_SendGameUserMap.end())
	{
		WriteGameServerUserInfo pOnce;
		pOnce.dwUserID = dwUserID;
		pOnce.ClientID = ClientID;
		pOnce.LogTime = timeGetTime();
		m_SendGameUserMap.insert(HashMap<DWORD, WriteGameServerUserInfo>::value_type(dwUserID, pOnce));
		return true;
	}
	else
	{
		ASSERT(false);
		return false;
	}
}
bool FishServer::OnCheckUserWriteGameServerIsExists(DWORD dwUserID)
{
	HashMap<DWORD, WriteGameServerUserInfo>::iterator Iter = m_SendGameUserMap.find(dwUserID);
	if (Iter == m_SendGameUserMap.end())
	{
		ASSERT(false);
		return false;
	}
	else
	{
		m_SendGameUserMap.erase(Iter);
		return true;
	}
}
void FishServer::OnUpdateWriteGameServerUserMap(DWORD dwTimer)
{
	HashMap<DWORD, WriteGameServerUserInfo>::iterator Iter = m_SendGameUserMap.begin();
	for (; Iter != m_SendGameUserMap.end();)
	{
		//10s
		if (dwTimer - Iter->second.LogTime >= 10000)
		{
			//发送命令到客户端去 告诉玩家 登陆失败了
			ServerClientData* pClient = GetClientServerData(Iter->second.ClientID);
			if (!pClient)
			{
				ASSERT(false);
				Iter = m_SendGameUserMap.erase(Iter);
				continue;
			}
			LC_Cmd_AccountOnlyID msgClient;
			SetMsgInfo(msgClient, GetMsgType(Main_Logon, LC_AccountOnlyID), sizeof(LC_Cmd_AccountOnlyID));
			msgClient.dwOnlyID = 0;
			msgClient.dwUserID = 0;
			msgClient.GameIp = 0;
			msgClient.GamePort = 0;
			msgClient.GateIp = 0;
			msgClient.GatePort = 0;
			msgClient.LogonType = 1;
			SendNewCmdToClient(pClient, &msgClient);

			DelClient pDel;
			pDel.LogTime = timeGetTime();
			pDel.SocketID = pClient->OutsideExtraData;
			m_DelSocketVec.push_back(pDel);

			LogInfoToFile("LogonError.txt",TEXT("客户端连接GameServer超时 登陆失败"));

			Iter = m_SendGameUserMap.erase(Iter);
		}
		else
		{
			++Iter;
			continue;
		}	
	}
}
void FishServer::OnReloadConfig()
{
	FishConfig pNewConfig;
	if (!pNewConfig.LoadConfigFilePath())
	{
		ShowInfoToWin("重新加载FishConfig.xml失败");
		return;
	}
	m_FishConfig = pNewConfig;
	ShowInfoToWin("重新加载FishConfig.xml成功");
}

void FishServer::SendNetCmdToControl(NetCmd*pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_ControlTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
bool FishServer::ConnectControl()
{
	//对控制方开启的端口 允许外部链接到中央服务器来进行登陆处理 携带DWORD 的唯一ID
	ControlServerConfig* pConfig = g_FishServerConfig.GetControlServerConfig();
	if (!pConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pControlData;
	pControlData.BuffSize = pConfig->ControlServerBuffSize;
	pControlData.Port = pConfig->ControlServerListenPort;
	pControlData.SleepTime = pConfig->ControlServerSleepTime;
	pControlData.SocketRecvSize = pConfig->ControlServerRecvBuffSize;
	pControlData.SocketSendSize = pConfig->ControlServerSendBuffSize;
	pControlData.ThreadNum = 1;
	pControlData.Timeout = pConfig->ControlServerTimeOut;
	pControlData.CmdHearbeat = 0;
	pControlData.SendArraySize = pConfig->ControlServerMaxSendCmdCount;
	pControlData.RecvArraySize = pConfig->ControlServerMaxRecvCmdCount;

	if (!m_ControlTcp.Init(pConfig->ControlServerListenIP, pControlData, (void*)&m_LogonConfigID, sizeof(BYTE)))
	{
		ShowInfoToWin("ControlServer连接失败");
		return false;
	}
	OnTcpClientConnect(&m_ControlTcp);
	return true;
}
bool FishServer::HandleControlMsg(NetCmd* pCmd)
{
	//处理运营服务器发送来的数据
	if (!pCmd)
		return false;
	if (pCmd->CmdType != Main_Control)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_ReloadConfig:
		{
			OnReloadConfig();
			return true;
		}
		break;
	case CL_ResetRolePassword:
		{
			CL_Cmd_ResetRolePassword* pMsg = (CL_Cmd_ResetRolePassword*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			m_LogonCacheManager.OnChangeAccountPassword(pMsg->dwUserID, pMsg->Password1, pMsg->Password2, pMsg->Password3);
			return true;
		}
		break;
	//case CL_FreezeAccount:
	//	{
	//		CL_Cmd_FreezeAccount* pMsg = (CL_Cmd_FreezeAccount*)pCmd;
	//		if (!pMsg)
	//		{
	//			ASSERT(false);
	//			return false;
	//		}
	//		//修改指定玩家的冻结时间
	//		time_t FreezeEndTime = time(null) + pMsg->FreezeMin * 60;
	//		m_LogonCacheManager.OnChangeAccountFreezeInfo(pMsg->dwUserID, FreezeEndTime);
	//		return true;
	//	}
	//	break;
	case CL_KickUserByID:
		{
			CL_Cmd_KickUserByID* pMsg = (CL_Cmd_KickUserByID*)pCmd;
			time_t FreezeEndTime = time(null) + pMsg->FreezeMin * 60;
			m_LogonCacheManager.OnChangeAccountFreezeInfo(pMsg->dwUserID, FreezeEndTime);
			return true;
		}
		break;
	}
	return true;
}
void FishServer::SendNetCmdToLogDB(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_LogDBTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::UpdateInfoToControl(DWORD dwTimer)
{
	static DWORD LogUpdateCenterTime = 0;
	if (LogUpdateCenterTime == 0 || dwTimer - LogUpdateCenterTime >= 10000)
	{
		LogUpdateCenterTime = dwTimer;
		LC_Cmd_LogonInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Control, LC_LogonInfo), sizeof(LC_Cmd_LogonInfo));
		msg.LogonID = m_LogonConfigID;
		msg.LogonTablePlayerSum = m_LogonManager.LogonRoleSum();
		SendNetCmdToControl(&msg);
	}
}