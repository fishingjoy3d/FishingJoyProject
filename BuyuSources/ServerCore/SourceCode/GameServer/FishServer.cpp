#include "stdafx.h"
#include "FishServer.h"
#include "FishLogic\NetCmd.h"
#include "..\CommonFile\FishServerConfig.h"
#include "..\CommonFile\ip.h"
#include "..\CommonFile\DBLogManager.h"
#include "IApppay\SignUtils.h"
#include "IApppay\CryptHelper.h"
FishServer g_FishServer;
void SendLogDB(NetCmd* pCmd)
{
	g_FishServer.SendNetCmdToLogDB(pCmd);
}
FishServer::FishServer()
{
	m_IsClose = false;
	m_CenterTcpStates = false;
	m_DBTcpStates = false;
	m_DBSaveTcpStates = false;
	m_FtpTcpStates = false;
	m_OperatorStates = false;
	m_UserIndex = 0;
	m_GameNetworkID = 0;
	m_DelSocketVec.clear();

	m_DBLogTcpStates = false;
	m_IsReloadConfig = false;

	m_MiniGameIsConnect = false;
	m_ControlIsConnect = false;
}
FishServer::~FishServer()
{
	m_ClintList.clear();
	m_UserIndex = 0;
	
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
bool FishServer::InitServer(int Index)
{
	m_pDump.OnInit();
	//1.读取配置文件 服务器配置的配置文件
	if (!g_FishServerConfig.LoadServerConfigFile())
	{
		ShowInfoToWin("服务器配置文件读取失败");
		return false;
	}

	m_GameNetworkID = ConvertIntToBYTE(Index);
	GameServerConfig* pGameConfig = g_FishServerConfig.GetGameServerConfig(m_GameNetworkID);
	if (!pGameConfig)
	{
		ASSERT(false);
		return false;
	}

	//4.各种管理器进行初始化
	if (!m_FishConfig.LoadConfigFilePath())
	{
		ASSERT(false);
		return false;
	}
	//5.
	if (!m_RoleManager.OnInit())
	{
		ASSERT(false);
		return false;
	}
	ShowInfoToWin("玩家管理器初始化成功");
	m_TableManager.OnInit();
	ShowInfoToWin("桌子管理器初始化成功");
	m_ShopManager.OnInit();
	ShowInfoToWin("商店管理器初始化成功");
	m_EventManager.OnInit();
	ShowInfoToWin("事件管理器初始化成功");

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
	if (!ConnectSaveDB())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectSaveDB())
				break;
		}
	}
	if (!ConnectFTP())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectFTP())
				break;
		}
	}
	m_RobotManager.OnLoadAllGameRobot(pGameConfig->BeginRobotUserID, pGameConfig->EndRobotUserID);//加载新的玩家
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
	if (!ConnectMiniGame())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectMiniGame())
				break;
		}
	}
	if (!ConnectClient())
	{
		ASSERT(false);
		return false;
	}
	ShowInfoToWin("GameServer启动成功");
	return true;
}
void FishServer::OnLoadFinish()
{
	//m_RobotManager.OnInit(pGameConfig->BeginRobotUserID, pGameConfig->EndRobotUserID);//加载新的玩家
}
void FishServer::Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	if (ServerID == m_GameNetworkID)
	{
		return;//客户端离开不做处理
	}
	//输出 客户端离开的状态
	switch (rt)
	{
	case REMOVE_NONE:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为None", pClient->OutsideExtraData);
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
bool FishServer::ConnectFTP()
{
	FTPServerConfig* pFTPConfig = g_FishServerConfig.GetFTPServerConfig();
	if (!pFTPConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pFtpData;
	pFtpData.BuffSize = pFTPConfig->BuffSize;
	pFtpData.Port = pFTPConfig->FTPListenPort;
	pFtpData.SleepTime = pFTPConfig->SleepTime;
	pFtpData.SocketRecvSize = pFTPConfig->RecvBuffSize;
	pFtpData.SocketSendSize = pFTPConfig->SendBuffSize;
	pFtpData.ThreadNum = 1;
	pFtpData.Timeout = pFTPConfig->TimeOut;
	pFtpData.CmdHearbeat = 0;
	pFtpData.SendArraySize = pFTPConfig->MaxSendCmdCount;
	pFtpData.RecvArraySize = pFTPConfig->MaxRecvCmdCount;
	if (!m_FtpTcp.Init(pFTPConfig->FTPListenIP, pFtpData, (void*)&m_GameNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("FTP服务器连接失败");
		return false;
	}
	OnTcpClientConnect(&m_FtpTcp);
	return true;
}
bool FishServer::ConnectDB()
{
	GameServerConfig* pGameConfig = g_FishServerConfig.GetGameServerConfig(m_GameNetworkID);
	if (!pGameConfig)
	{
		ASSERT(false);
		return false;
	}


	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pGameConfig->DBNetworkID);
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
	if (!m_DBTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_GameNetworkID,sizeof(BYTE)))
	{
		ShowInfoToWin("DB服务器连接失败");
		return false;
	}
	OnTcpClientConnect(&m_DBTcp);
	return true;
}
bool FishServer::ConnectSaveDB()
{
	//连接到保存数据库 和 普通的读取 修改数据库不是一个数据库
	GameServerConfig* pGameConfig = g_FishServerConfig.GetGameServerConfig(m_GameNetworkID);
	if (!pGameConfig)
	{
		ASSERT(false);
		return false;
	}

	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pGameConfig->SaveDBNetworkID);
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
	if (!m_DBSaveTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_GameNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("SaveDB服务器连接失败");
		return false;
	}
	OnTcpClientConnect(&m_DBSaveTcp);
	return true;
}
bool FishServer::ConnectLogDB()
{
	//连接到保存数据库 和 普通的读取 修改数据库不是一个数据库
	GameServerConfig* pGameConfig = g_FishServerConfig.GetGameServerConfig(m_GameNetworkID);
	if (!pGameConfig)
	{
		ASSERT(false);
		return false;
	}

	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pGameConfig->LogDBNetworkID);
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
	if (!m_DBLogTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_GameNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("LogDB服务器连接失败");
		return false;
	}
	OnTcpClientConnect(&m_DBLogTcp);
	return true;
}
//bool FishServer::ConnectRank()
//{
//	RankServerConfig* pRankConfig = g_FishServerConfig.GetRankServerConfig();
//	if (!pRankConfig)
//	{
//		ASSERT(false);
//		return false;
//	}
//	ClientInitData pRankData;
//	pRankData.BuffSize = pRankConfig->BuffSize;
//	pRankData.Port = pRankConfig->RankListenPort;
//	pRankData.SleepTime = pRankConfig->SleepTime;
//	pRankData.SocketRecvSize = pRankConfig->RecvBuffSize;
//	pRankData.SocketSendSize = pRankConfig->SendBuffSize;
//	pRankData.ThreadNum = 1;
//	pRankData.Timeout = pRankConfig->TimeOut;
//	pRankData.CmdHearbeat = 0;
//	if (!m_RankTcp.Init(pRankConfig->RankListenIP, pRankData))
//	{
//		std::cout << "排行榜服务器连接失败\n";
//		return false;
//	}
//	OnTcpClientConnect(&m_RankTcp);
//	return true;
//}
//bool FishServer::ConnectLogon()
//{
//	LogonServerConfig* pLogonConfig = g_FishServerConfig.GetLogonServerConfig();
//	if (!pLogonConfig)
//	{
//		ASSERT(false);
//		return false;
//	}
//	ClientInitData pLogonData;
//	pLogonData.BuffSize = pLogonConfig->LogonServerBuffSize;
//	pLogonData.Port = pLogonConfig->LogonServerListenPort;
//	pLogonData.SleepTime = 1;
//	pLogonData.SocketRecvSize = pLogonConfig->LogonServerRecvMaxSize;
//	pLogonData.SocketSendSize = pLogonConfig->LogonServerSendMaxSize;
//	pLogonData.ThreadNum = 1;
//	pLogonData.Timeout = pLogonConfig->TimeOut;
//	pLogonData.CmdHearbeat = 0;
//	if (!m_LogonTcp.Init(pLogonConfig->LogonServerListenIP, pLogonData))
//	{
//		std::cout << "登陆服务器连接失败\n";
//		return false;
//	}
//	OnTcpClientConnect(&m_LogonTcp);
//	return true;
//}
bool FishServer::ConnectCenter()
{
	CenterServerConfig* pCenterConfig = g_FishServerConfig.GetCenterServerConfig();
	if (!pCenterConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pCenterData;
	pCenterData.BuffSize = pCenterConfig->BuffSize;
	pCenterData.Port = pCenterConfig->CenterListenPort;
	pCenterData.SleepTime = pCenterConfig ->SleepTime ;
	pCenterData.SocketRecvSize = pCenterConfig->RecvBuffSize;
	pCenterData.SocketSendSize = pCenterConfig->SendBuffSize;
	pCenterData.ThreadNum = 1;
	pCenterData.Timeout = pCenterConfig->TimeOut;
	pCenterData.CmdHearbeat = 0;
	pCenterData.SendArraySize = pCenterConfig->MaxSendCmdCount;
	pCenterData.RecvArraySize = pCenterConfig->MaxRecvCmdCount;
	if (!m_CenterTcp.Init(pCenterConfig->CenterListenIP, pCenterData, (void*)&m_GameNetworkID, sizeof(BYTE)))
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
		ASSERT(false);
		return false;
	}
	ClientInitData pOperateData;
	pOperateData.BuffSize = pOperateConfig->BuffSize;
	pOperateData.Port = pOperateConfig->OperateListenPort;
	pOperateData.SleepTime = pOperateConfig->SleepTime;
	pOperateData.SocketRecvSize = pOperateConfig->RecvBuffSize;
	pOperateData.SocketSendSize = pOperateConfig->SendBuffSize;
	pOperateData.ThreadNum = 1; 
	pOperateData.Timeout = pOperateConfig->TimeOut;
	pOperateData.CmdHearbeat = 0;
	pOperateData.SendArraySize = pOperateConfig->MaxSendCmdCount;
	pOperateData.RecvArraySize = pOperateConfig->MaxRecvCmdCount;
	if (!m_OperatorTcp.Init(pOperateConfig->OperateListenIP, pOperateData, (void*)&m_GameNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("运营服务器连接失败");
		return false;
	}

	// 运营服务器Ip 转DWORD
	//m_OperateIP = inet_addr(pOperateConfig->OperateListenIP);
	inet_pton(AF_INET, pOperateConfig->OperateListenIP, (void *)&m_OperateIP);

	OnTcpClientConnect(&m_OperatorTcp);
	return true;
}
bool FishServer::ConnectClient()
{
	GameServerConfig* pGameConfig = g_FishServerConfig.GetGameServerConfig(m_GameNetworkID);
	if (!pGameConfig)
	{
		ASSERT(false);
		return false;
	}
	ServerInitData pCLientData;
	pCLientData.ServerID = pGameConfig->NetworkID;
	pCLientData.BuffSize = pGameConfig->BuffSize;
	pCLientData.CmdHearbeat = 0;
	pCLientData.Port = pGameConfig->GameListenPort;
	pCLientData.SocketRecvSize = pGameConfig->RecvBuffSize;
	pCLientData.SocketSendSize = pGameConfig->SendBuffSize;
	pCLientData.Timeout = pGameConfig->TimeOut;
	pCLientData.RecvThreadNum = pGameConfig->RecvThreadSum;
	pCLientData.SendThreadNum = pGameConfig->SendThreadSum;
	pCLientData.SleepTime = pGameConfig->SleepTime;
	pCLientData.AcceptSleepTime = pGameConfig->AcceptSleepTime;
	pCLientData.AcceptRecvData = false;		//表示接收初始数据

	pCLientData.SceneHearbeatTick = pGameConfig->SceneHearbeatTick;
	pCLientData.MaxSendCmdCount = pGameConfig->MaxSendCmdCount;
	pCLientData.MaxAcceptNumPerFrame = pGameConfig->MaxAcceptNumPerFrame;
	pCLientData.MaxRecvCmdCount = pGameConfig->MaxRecvCmdCount;
	pCLientData.MaxSendCountPerFrame = pGameConfig->MaxSendCountPerFrame;
	pCLientData.ListenCount = pGameConfig->ListenCount;

	m_ClientTcp.SetCmdHandler(this);

	m_GameNetworkID = pGameConfig->NetworkID;

	if (!m_ClientTcp.Init(pCLientData, false))
	{
		ShowInfoToWin("对内网络库启动失败");
		return false;
	}
	
	
	return true;
}
uint FishServer::CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize)
{
	//中央服务器只有在FTP 和 DB都连接成功后才工作
	bool IsNeed = m_DBTcp.IsConnected() && m_CenterTcp.IsConnected() && m_OperatorTcp.IsConnected() && m_DBSaveTcp.IsConnected() & m_DBLogTcp.IsConnected() && m_FtpTcp.IsConnected();
	if (IsNeed)
	{
		if (recvSize != 8 || !pData)
			return ConvertIntToDWORD(CONNECT_CHECK_FAILED);
		DWORD* pInfo = (DWORD*)pData;
		if (!m_FishConfig.CheckVersionAndPathCrc(pInfo[0], pInfo[1]))
		{
			pInfo[0] = m_FishConfig.GetSystemConfig().VersionID;
			pInfo[1] = m_FishConfig.GetSystemConfig().PathCrc;
			return 8;
		}
		return ConvertIntToDWORD(CONNECT_CHECK_OK);
	}
	else
		return ConvertIntToDWORD(CONNECT_CHECK_FAILED);
}
bool FishServer::NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize)
{
	if (!pClient)
	{
		ASSERT(false);
		return false;
	}
	/*if (!pData || recvSize == 0)
	{
		ASSERT(false);
		return false;
	}*/
	AfxNetworkClientOnce* pOnce = new AfxNetworkClientOnce();
	if (!pOnce)
	{
		m_ClientTcp.Kick(pClient);
		ASSERT(false);
		return false;
	}
	pOnce->SeverID = SeverID;
	pOnce->pClient = pClient;
	pOnce->pPoint = (void*)malloc(recvSize);
	if (!pOnce->pPoint)
	{
		m_ClientTcp.Kick(pClient);
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
			//ASSERT(false);
			continue;
		}
		if (pOnce->SeverID == m_GameNetworkID)
		{
			++m_UserIndex;
			DWORD ClientID = m_UserIndex;
			pOnce->pClient->OutsideExtraData = ClientID;//玩家的ID
			m_ClintList.insert(HashMap<DWORD, ServerClientData*>::value_type(pOnce->pClient->OutsideExtraData, pOnce->pClient));
			//触发玩家加入的事件
			OnTcpServerJoin(pOnce->SeverID, pOnce->pClient);

			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
		}
		else
		{
			delete pOnce;
			//ASSERT(false);
		}
	}
}
void FishServer::OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient)
{
	//往网络库中有一个客户端离开的时候
	if (ServerID == m_GameNetworkID)
	{
		//ShowInfoToWin("一个Client离开了GameServer");
		OnGateLeaveGameRoom(pClient->OutsideExtraData);
	}
	return;
}
void FishServer::OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient)
{
	//往网络库中有一个客户端加入的时候
	if (ServerID == m_GameNetworkID)
	{
		//ShowInfoToWin("一个Client加入了GameServer");
	}
	return;
}
void FishServer::OnTcpClientConnect(TCPClient* pClient)
{
	if (!pClient)
		return;
	//网络库连接成功的时候
	if (pClient == &m_CenterTcp)
	{
		m_CenterTcpStates = true;
		ShowInfoToWin("已经与中央服务器连接成功了");

		OnConnectionCenterServer();
	}
	else if (pClient == &m_DBTcp)
	{
		m_DBTcpStates = true;

		ShowInfoToWin("已经与数据库连接成功了");
	}
	else if (pClient == &m_DBSaveTcp)
	{
		m_DBSaveTcpStates = true;
		ShowInfoToWin("已经与Save数据库连接成功了");
	}
	else if (pClient == &m_DBLogTcp)
	{
		m_DBLogTcpStates = true;
		ShowInfoToWin("已经与Log数据库连接成功了");
	}
	/*else if (pClient == &m_RankTcp)g
	{
		m_RankTcpStates = true;
		std::cout << "已经与排行榜服务器连接成功了\n";
	}*/
	else if (pClient == &m_OperatorTcp)
	{
		m_OperatorStates = true;
		ShowInfoToWin("已经与运营服务器连接成功了");

	}
	else if (pClient == &m_ControlTcp)
	{
		m_ControlIsConnect = true;
		ShowInfoToWin("已经与ControlServer连接成功了");
	}
	else if (pClient == &m_MiniGameTcp)
	{
		m_MiniGameIsConnect = true;
		ShowInfoToWin("已经与MiniGame连接成功了");
	}
	//else if (pClient == &m_LogonTcp)
	//{
	//	m_LogonTcpStates = true;
	//	//获取当前的IP 以及端口
	//	GL_Cmd_RsgGameServer msg;
	//	SetMsgInfo(msg,GetMsgType(Main_Logon, GL_RsgGameServer), sizeof(GL_Cmd_RsgGameServer));
	//	//获取本机的IP地址 发送给LogonServer
	//	GameServerConfig* pGameConfig = g_FishServerConfig.GetGameServerConfig(m_GameNetworkID);
	//	if (!pGameConfig)
	//		return;
	//	msg.Ip = inet_addr(pGameConfig->GameListenIP);
	//	msg.Port = pGameConfig->GameListenPort;
	//	SendNetCmdToLogon(&msg);
	//	std::cout << "已经与登陆服务器连接成功了\n";
	//}
	else if (pClient == &m_FtpTcp)
	{
		m_FtpTcpStates = true;
		ShowInfoToWin("已经与FTP服务器连接成功了");
	}
	else
		return;
}
void FishServer::OnConnectionCenterServer()
{
	//当连接上中央服务器的时候
	//0.注册GameServer
	//m_GameServerManager.OnRsgGameToCenter();//将GameServer注册到Center
	//1.获取全部的玩家 将玩家的数据进行上传到中央服务器去 包括关系数据
	HashMap<DWORD, CRoleEx*>& pMap = m_RoleManager.GetAllRole();
	HashMap<DWORD, CRoleEx*>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		if (Iter->second)
			Iter->second->SendUserInfoToCenter();
	}

	m_AnnouncementManager.OnConnectionCenter();//申请获取中央服务器公告数据
	_SystemMailManager.OnConnectCentralServer();
	m_IsSendUserInfo = true;
}
void FishServer::OnLeaveCenterServer()
{
	//当GameServer离开Center的时候 我们让玩家全部离线
	//让全部玩家离线
	m_RoleManager.OnKickAllUser();
	//断开全部的网络
	HashMap<DWORD, ServerClientData*>::iterator Iter = m_ClintList.begin();
	for (; Iter != m_ClintList.end(); ++Iter)
	{
		m_ClientTcp.Kick(Iter->second);
	}
	m_ClintList.clear();
}
void FishServer::OnTcpClientLeave(TCPClient* pClient)
{
	if (pClient == &m_CenterTcp)
	{
		m_CenterTcpStates = false;
		ShowInfoToWin("已经与中央服务器断开连接了");
		m_IsSendUserInfo = false;
		m_CenterTcp.Shutdown();
		//m_GameServerManager.OnGameLeavCenter();//与中央服务器断开连接
		OnLeaveCenterServer();
	}
	else if (pClient == &m_DBTcp)
	{
		m_DBTcpStates = false;
		m_DBTcp.Shutdown();
		ShowInfoToWin("已经与数据库断开连接了");
	}
	else if (pClient == &m_DBSaveTcp)
	{
		m_DBSaveTcpStates = false;
		m_DBSaveTcp.Shutdown();
		ShowInfoToWin("已经与Save数据库断开连接了");
	}
	else if (pClient == &m_DBLogTcp)
	{
		m_DBLogTcpStates = false;
		m_DBLogTcp.Shutdown();
		ShowInfoToWin("已经与Log数据库断开连接了");
	}
	/*else if (pClient == &m_RankTcp)
	{
		m_RankTcpStates = false;
		std::cout << "已经与排行榜服务器断开连接了\n";
	}*/
	/*else if (pClient == &m_LogonTcp)
	{
		m_LogonTcpStates = false;
		std::cout << "已经与登陆服务器断开连接了\n";
	}*/
	else if (pClient == &m_OperatorTcp)
	{
		m_OperatorStates = false;
		m_OperatorTcp.Shutdown();
		ShowInfoToWin("已经与运营服务器断开连接了");
	}
	else if (pClient == &m_ControlTcp)
	{
		m_ControlIsConnect = false;
		m_ControlTcp.Shutdown();
		ShowInfoToWin("已经与ControlServer断开连接了");
	}
	else if (pClient == &m_FtpTcp)
	{
		m_FtpTcpStates = false;
		m_FtpTcp.Shutdown();
		ShowInfoToWin("已经与FTP服务器断开连接了");
	}
	else if (pClient == &m_MiniGameTcp)
	{
		m_MiniGameIsConnect = false;
		m_MiniGameTcp.Shutdown();
		ShowInfoToWin("已经与MiniGame断开连接了");
	}
	else
		return;
}
void FishServer::CloseClientSocket(DWORD SocketID)
{
	ServerClientData* pClient = GetUserClientDataByIndex(SocketID);
	if (pClient)
		pClient->Removed = true;
}
void FishServer::SendNetCmdToDB(NetCmd* pCmd)
{
	if (!m_DBTcp.IsConnected() || !pCmd)
		return;
	if (!m_DBTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToCenter(NetCmd* pCmd)
{
	if (!m_CenterTcp.IsConnected() || !pCmd)
		return;
	if (!m_CenterTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}

//void FishServer::SendNetCmdToRank(NetCmd* pCmd)
//{
//	if (!m_RankTcp.IsConnected() || !pCmd)
//		return;
//	m_RankTcp.Send(pCmd, false);
//}
void FishServer::SendNetCmdToFTP(NetCmd* pCmd)
{
	if (!m_FtpTcp.IsConnected() || !pCmd)
		return;
	if (!m_FtpTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToOperate(NetCmd* pCmd)
{
	if (!m_OperatorTcp.IsConnected() || !pCmd)
		return;
	if (!m_OperatorTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToSaveDB(NetCmd* pCmd)
{
	if (!m_DBSaveTcp.IsConnected() || !pCmd)
		return;
	if (!m_DBSaveTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToLogDB(NetCmd* pCmd)
{
	if (!m_DBLogTcp.IsConnected() || !pCmd)
		return;
	if (!m_DBLogTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
//void FishServer::SendNetCmdToLogon(NetCmd* pCmd)
//{
//	if (!m_LogonTcp.IsConnected() || !pCmd)
//		return;
//	m_LogonTcp.Send(pCmd, false);
//}
void FishServer::SendNewCmdToClient(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return;
	//LogSendCmdMsg(pClient->OutsideExtraData,pCmd->CmdType, pCmd->SubCmdType, pCmd->CmdSize);
	if (!m_ClientTcp.Send(pClient, pCmd))
	{
		ASSERT(false);
	}
}
void FishServer::SendNewCmdToAllClient(NetCmd* pCmd)
{
	if (!pCmd)
		return;
	if (!m_ClintList.empty())
	{
		HashMap<DWORD, ServerClientData*>::iterator Iter = m_ClintList.begin();
		for (; Iter != m_ClintList.end(); ++Iter)
		{
			if (!m_ClientTcp.Send(Iter->second, pCmd))
			{
				ASSERT(false);
			}
		}
	}
}
void FishServer::OnHandleAllMsg()
{
	if (!m_ClintList.empty())
	{
		HashMap<DWORD, ServerClientData*>::iterator Iter = m_ClintList.begin();
		for (; Iter != m_ClintList.end();)
		{
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
	NetCmd* pCmd = m_DBTcp.GetCmd();
	while (pCmd)
	{
		HandleDataBaseMsg(pCmd);
		free(pCmd);
		pCmd = m_DBTcp.GetCmd();
	}

	pCmd = m_DBSaveTcp.GetCmd();
	while (pCmd)
	{
		HandleDataBaseMsg(pCmd);
		free(pCmd);
		pCmd = m_DBSaveTcp.GetCmd();
	}

	//NetCmd** pArray = m_DBTcpList.CheckAllTcpClient(false);
	//while (pArray[0])
	//{
	//	for (int i = 0; i < m_DBTcpList.GetTcoClientSum(); ++i)
	//	{
	//		NetCmd* pCmd = pArray[i];
	//		if (pCmd)
	//		{
	//			HandleDataBaseMsg(pCmd);
	//			free(pCmd);
	//		}
	//	}
	//	pArray = m_DBTcpList.CheckAllTcpClient(false);
	//}
	

	/*vector<NetCmd*> pVec= m_DBTcpList.CheckAllTcpClient(false);
	while (!pVec.empty())
	{
		vector<NetCmd*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			HandleDataBaseMsg(*Iter);
			free(*Iter);
		}
		pVec.clear();
		pVec = m_DBTcpList.CheckAllTcpClient(false);
	}*/

	pCmd = m_CenterTcp.GetCmd();
	while (pCmd)
	{
		HandleCenterMsg(pCmd);
		free(pCmd);
		pCmd = m_CenterTcp.GetCmd();
	}

	/*pCmd = m_RankTcp.GetCmd();
	while (pCmd)
	{
		HandleRankMsg(pCmd);
		free(pCmd);
		pCmd = m_RankTcp.GetCmd();
	}*/
	/*pCmd = m_LogonTcp.GetCmd();
	while (pCmd)
	{
		HandleLogonMsg(pCmd);
		free(pCmd);
		pCmd = m_LogonTcp.GetCmd();
	}*/

	//m_GameServerManager.OnHandleLogonMsg();

	pCmd = m_FtpTcp.GetCmd();
	while (pCmd)
	{
		HandleFtpMsg(pCmd);
		free(pCmd);
		pCmd = m_FtpTcp.GetCmd();
	}

	pCmd = m_OperatorTcp.GetCmd();
	while (pCmd)
	{
		HandleOperateMsg(pCmd);
		free(pCmd);
		pCmd = m_OperatorTcp.GetCmd();
	}

	pCmd = m_ControlTcp.GetCmd();
	while (pCmd)
	{
		HandleControlMsg(pCmd);
		free(pCmd);
		pCmd = m_ControlTcp.GetCmd();
	}

	pCmd = m_MiniGameTcp.GetCmd();
	while (pCmd)
	{
		HandleMiniGameMsg(pCmd);
		free(pCmd);
		pCmd = m_MiniGameTcp.GetCmd();
	}
}
//void FishServer::HandlePlayerSum(DWORD dwTimer)
//{
//	if (dwTimer - m_LogPlayerTime < 1000)
//		return;
//	m_LogPlayerTime = dwTimer;
//	if (m_ClintList.size() != m_LogPlayerSum)
//	{
//		m_LogPlayerSum = m_ClintList.size();
//
//		GL_Cmd_OnlinePlayerSum msgOnline;
//		SetMsgInfo(msgOnline, GetMsgType(Main_Logon, GL_OnlinePlayerSum), sizeof(GL_Cmd_OnlinePlayerSum));
//		msgOnline.PlayerSum = m_LogPlayerSum;
//		SendNetCmdToLogon(&msgOnline);
//	}
//}
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

		OnAddClient();

		UpdateInfoToControl(dwTimer);

		CheckDelSocket(dwTimer);//处理离线的socket

		UpdateByMin(dwTimer);
		m_ShopManager.UpdateByMin();//更新函
		OnSaveInfoToDB(dwTimer);

		m_RobotManager.Update();
		//HandlePlayerSum(dwTimer);

		m_RoleQueueManager.OnUpdateQueue(dwTimer);
		//1.处理Client 的命令
		if (!m_ClintList.empty())
		{
			HashMap<DWORD, ServerClientData*>::iterator Iter = m_ClintList.begin();
			for (; Iter != m_ClintList.end();)
			{
				if (Iter->second->Removed)
				{
					OnTcpServerLeave(m_GameNetworkID, Iter->second);
					m_ClientTcp.Kick(Iter->second);
					Iter = m_ClintList.erase(Iter);
					continue;
				}
				int Sum = 0;
				while (Iter->second->RecvList.HasItem() && Sum < Client_Msg_OnceSum)
				{
					NetCmd* pCmd = Iter->second->RecvList.GetItem();
					HandleClientMsg(Iter->second, pCmd);
					free(pCmd);
					++Sum;
				}
				++Iter;
			}
		}
		//2.处理数据库的命令
		if (m_DBTcp.IsConnected())
		{
			if (!m_DBTcpStates)
				OnTcpClientConnect(&m_DBTcp);
			NetCmd* pCmd = m_DBTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
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
			if (m_DBTcpStates)
				OnTcpClientLeave(&m_DBTcp);

			//重新连接DB
			ConnectDB();
		}

		if (m_DBSaveTcp.IsConnected())
		{
			if (!m_DBSaveTcpStates)
				OnTcpClientConnect(&m_DBSaveTcp);
			NetCmd* pCmd = m_DBSaveTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleDataBaseMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_DBSaveTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleDataBaseMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_DBSaveTcpStates)
				OnTcpClientLeave(&m_DBSaveTcp);

			//重新连接DB
			ConnectSaveDB();
		}

		if (m_DBLogTcp.IsConnected())
		{
			if (!m_DBLogTcpStates)
				OnTcpClientConnect(&m_DBLogTcp);
		}
		else
		{
			if (m_DBLogTcpStates)
				OnTcpClientLeave(&m_DBLogTcp);

			//重新连接DB
			ConnectLogDB();
		}

		/*NetCmd** pArray = m_DBTcpList.CheckAllTcpClient(true);
		int HandleDBSum = 0;
		while (pArray[0] && HandleDBSum < Msg_OnceSum)
		{
			for (int i = 0; i < m_DBTcpList.GetTcoClientSum(); ++i)
			{
				NetCmd* pCmd = pArray[i];
				if (pCmd)
				{
					HandleDataBaseMsg(pCmd);
					free(pCmd);
					++HandleDBSum;
				}
			}
			pArray = m_DBTcpList.CheckAllTcpClient(false);
		}*/

		//3.中央服务器
		if (m_CenterTcp.IsConnected())
		{
			if (!m_CenterTcpStates)
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
			if (m_CenterTcpStates)
				OnTcpClientLeave(&m_CenterTcp);

			ConnectCenter();
		}

		//4.排行榜服务器
		//if (m_RankTcp.IsConnected())
		//{
		//	if (!m_RankTcpStates)
		//		OnTcpClientConnect(&m_RankTcp);
		//	NetCmd* pCmd = m_RankTcp.GetCmd();
		//	int Sum = 0;
		//	while (pCmd && Sum < Msg_OnceSum)
		//	{
		//		HandleRankMsg(pCmd);
		//		free(pCmd);
		//		++Sum;
		//		pCmd = m_RankTcp.GetCmd();
		//	}
		//}
		//else
		//{
		//	if (m_RankTcpStates)
		//		OnTcpClientLeave(&m_RankTcp);

		//	//重新连接DB
		//	ConnectRank();
		//}
		//5.Logon 
		//m_GameServerManager.OnHandleLogonMsg();//处理Logon上的命令

		//6.运营服务器
		if (m_OperatorTcp.IsConnected())
		{
			if (!m_OperatorStates)
				OnTcpClientConnect(&m_OperatorTcp);
			NetCmd* pCmd = m_OperatorTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleOperateMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_OperatorTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleOperateMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_OperatorStates)
				OnTcpClientLeave(&m_OperatorTcp);

			//重新连接DB
			ConnectOperate();
		}
		

		//if (m_LogonTcp.IsConnected())
		//{
		//	if (!m_LogonTcpStates)
		//		OnTcpClientConnect(&m_LogonTcp);
		//	NetCmd* pCmd = m_LogonTcp.GetCmd();
		//	//处理数据库发送来的数据
		//	HandleLogonMsg(pCmd);
		//	free(pCmd);
		//}
		//else
		//{
		//	if (m_LogonTcpStates)
		//		OnTcpClientLeave(&m_LogonTcp);

		//	//重新连接DB
		//	ConnectLogon();
		//}
		//6.FTP
		if (m_FtpTcp.IsConnected())
		{
			if (!m_FtpTcpStates)
				OnTcpClientConnect(&m_FtpTcp);
			NetCmd* pCmd = m_FtpTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleFtpMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_FtpTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleFtpMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_FtpTcpStates)
				OnTcpClientLeave(&m_FtpTcp);

			//重新连接DB
			ConnectFTP();
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

		if (m_MiniGameTcp.IsConnected())
		{
			if (!m_MiniGameIsConnect)
				OnTcpClientConnect(&m_MiniGameTcp);
			NetCmd* pCmd = m_MiniGameTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleMiniGameMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_MiniGameTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleMiniGameMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_MiniGameIsConnect)
				OnTcpClientLeave(&m_MiniGameTcp);

			//重新连接DB
			ConnectMiniGame();
		}
		

		//管理器更新
		m_LogonManager.OnUpdate(dwTimer);
		//FTP 更新
		//OnFtpUpdate();
		//玩家更新
		m_RoleManager.OnUpdate(dwTimer);//更新比赛的变化
		//命令处理完毕后 我们开始处理 其他的更新函数 其实就是 不断的Send命令到其他服务器或者客户端去
		m_TableManager.Update(dwTimer); //更新桌子


		Sleep(1);
	}
	return OnDestroy();
}
bool FishServer::OnDestroy()
{
	m_TableManager.OnStopService();
	m_RoleManager.Destroy();

	OnHandleAllMsg();
	OnClearAllChannelInfo();
	while (m_AfxAddClient.HasItem())
	{
		AfxNetworkClientOnce* pOnce = m_AfxAddClient.GetItem();
		delete pOnce;
	}
	return true;
}
//开始命令处理类了 
bool FishServer::HandleClientMsg(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	//客户端发送来的命令
	switch (pCmd->CmdType)
	{
	case Main_Logon:
		return OnHandClientLogonMsg(pClient, pCmd);
	case Main_Table:
		return OnHandleTCPNetworkTable(pClient, pCmd);
	case Main_Game:
		return OnHandleTCPNetworkGame(pClient, pCmd);
	case Main_Center:
		{
			//if (pCmd->SubCmdType == CL_Sub_RsgFinish)
			//{
			//	//CTraceService::TraceString(TEXT("一个网关注册到当前房间"), TraceLevel_Normal);
			//	return true;
			//}
		}
		return true;
	case Main_Item:
		return OnHandleTCPNetworkItem(pClient, pCmd);
	case Main_Relation:
		return OnHandleTCPNetworkRelation(pClient, pCmd);
	case Main_Mail:
		return OnHandleTCPNetworkMail(pClient, pCmd);
	case Main_Role:
		return OnHandleTCPNetworkRole(pClient, pCmd);
	case Main_Query:
		return OnHandleTCPNetworkQuery(pClient, pCmd);
	case Main_Check:
		return OnHandleTCPNetworkCheck(pClient,pCmd);
	case Main_Task:
		return OnHandleTCPNetworkTask(pClient,pCmd);
	case Main_Achievement:
		return OnHandleTCPNetworkAchievement(pClient,pCmd);
	case Main_Month:
		return OnHandleTCPNetworkMonth(pClient,pCmd);
	case Main_Title:
		return OnHandleTCPNetworkTitle(pClient,pCmd);
	case Main_Rank:
		return OnHandleTCPNetworkRank(pClient,pCmd);
	case Main_Chest:
		return OnHandleTCPNetworkChest(pClient,pCmd);
	case Main_Charm:
		return OnHandleTCPNetworkCharm(pClient,pCmd);
	case Main_Shop:
		return OnHandleTCPNetworkShop(pClient,pCmd);
	case Main_Entity:
		return OnHandleTCPNetworlEntity(pClient,pCmd);
	case Main_Action:
		return OnHandleTCPNetworkAction(pClient,pCmd);
	case Main_Giff:
		return OnHandleTCPNetworkGiff(pClient,pCmd);
	/*case Main_GlobelShop:
		return OnHandleTCPNetworkGlobelShop(pClient,pCmd);*/
	case Main_OnlineReward:
		return OnHandleTCPNetworkOnlineReward(pClient,pCmd);
	case Main_Package:
		return OnHandleTCPNetworkPackage(pClient, pCmd);
	case Main_GameData:
		return OnHandleTCPNetworkGameData(pClient, pCmd);
	case Main_Message:
		return OnHandleTCPNetworkMessage(pClient, pCmd);
	case Main_Recharge:
		return OnHandleTCPNetworkRecharge(pClient, pCmd);
	case Main_Announcement:
		return OnHandleTCPNetworkAnnouncement(pClient, pCmd);
	case Main_Operate:
		return OnHandleTCpNetworkOperate(pClient, pCmd);
	case Main_Exchange:
		return OnHandleTCPNetworkExChange(pClient, pCmd);
	case Main_Lottery:
		return OnHandleTCPNetwordLottery(pClient, pCmd);
	case Main_MiniGame:
		return OnHandleTCPNetworkMiniGame(pClient, pCmd);
	case Main_NiuNiu:
		return OnHandleTCPNetworkNiuNiu(pClient, pCmd);
	case Main_Dial:
		return OnHandleTCPNetworkDial(pClient, pCmd);
	case Main_Car:
		return OnHandleTCPNetworkCar(pClient, pCmd);
	case Main_Char:
		return OnHandleTCPNetworkChar(pClient, pCmd);
	case Main_RelationRequest:
		return OnHandleTCPNerwordRelationRequest(pClient, pCmd);
	}
	return true;
}
bool FishServer::HandleDataBaseMsg(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	switch (pCmd->GetCmdType())
	{
	case DBO_GetAccountInfoByUserID:
		return OnHandDBLogonMsg(pCmd);
		/*case DBO_GetRoleAchievementIndex:
			return OnHandleRoleAchievementIndex(pCmd);*/
	case DBO_ChangeAccountPassword:
		return OnHandleResetPasswordResult(pCmd);
		/*case DBO_SetOnline:
			return OnHandleRoleOnline(pCmd);*/
	case DBO_LoadUserItem:
		return OnHandleDateBaseLoadItem(pCmd);
		/*case DBO_LoadUserItemFinish:
			return OnHandleDataBaseLoadItemFinish(pCmd);*/
	case DBO_AddUserItem:
		return OnHandleDataBaseAddItemResult(pCmd);
	case DBO_LoadUserRelation:
		return OnHandleDateBaseLoadRelation(pCmd);
	case DBO_LoadBeUserRelation:
		return OnHandleDataBaseLoadBeRelation(pCmd);
		/*case DBO_LoadBeUserRelationFinish:
			return OnHandleDataBaseLoadBeRelationFinish(pCmd);*/
	case DBO_AddUserRelation:
		return OnHandleDataBaseAddRelation(pCmd);
	case DBO_LoadUserMail:
		return OnHandleDataBaseLoadUserMail(pCmd);
		/*case DBO_LoadUserMailFinish:
			return OnHandleDataBaseLoadUserMailFinish(pCmd);*/
	case DBO_AddUserMail:
		return OnHandleDataBaseSendUserMail(pCmd);
	case DBO_Query_RoleInfo:
		return OnHandleDataBaseLoadQueryUserInfo(pCmd);
	case DBO_Query_RoleInfoByUserID:
		return OnHandleDataBaseLoadQueryUserInfoByUserID(pCmd);
	case DBO_Query_RoleInfoByGameID:
		return OnHandleDataBaseLoadQueryUserInfoByGameID(pCmd);
		/*case DBO_LoadRoleCheckInfo:
			return OnHandleDataBaseLoadUserCheckInfo(pCmd);*/
	case DBO_LoadRoleTask:
		return OnHandleDataBaseLoadUserTaskInfo(pCmd);
	case DBO_LoadRoleAchievement:
		return OnHandleDataBaseLoadUserAchievementInfo(pCmd);
	case DBO_LoadRoleTitle:
		return OnHandleDataBaseLoadUserTitleInfo(pCmd);
	case DBO_LoadRoleEntity:
		return OnHandleDataBaseLoadUserEntityInfo(pCmd);
	case DBO_LoadRoleAction:
		return OnHandleDataBaseLoadUserActionInfo(pCmd);
	case DBO_LoadRoleGiff:
		return OnHandleDataBaseLoadUserGiff(pCmd);
	case DBO_GetNowDayGiff:
		return OnHnaldeDataBaseLoadUserSendGiffInfo(pCmd);
	case DBO_AddRoleGiff:
		return OnHandleDataBaseAddUserGiff(pCmd);
	case DBO_LoadGameData:
		return OnHandleDataBaseLoadGameData(pCmd);
	case DBO_SaveRoleAllInfo:
		return OnHandleSaveRoleAllInfo(pCmd);
	case DBO_LoadWeekRankInfo:
		return OnHandleLoadWeekRankInfo(pCmd);
	case DBO_SaveRoleNickName:
		return OnHandleChangeRoleNickName(pCmd);
	case DBO_ResetAccount:
		return OnHandleResetAccountName(pCmd);
	case DBO_QueryExChange:
		return OnHandleDataBaseQueryExChange(pCmd);
	case DBO_GetRechargeOrderID:
		return OnHandleGetRechargeOrderID(pCmd);
	case DBO_LoadCharInfo:
		return OnHandleLoadCharInfo(pCmd);
	case DBO_LoadRelationRequest:
		return OnHandleDataBaseLoadRelationRequest(pCmd);
	case DBO_AddRelationRequest:
		return OnHandleDataBaseAddRelationRequest(pCmd);
	case DBO_ChangeRoleSecPassword:
		return OnHandleChangeRoleSecPassword(pCmd);
	case DBO_GameIDConvertToUserID:
		return OnHandleGameIDConvertUserID(pCmd);
	case DBO_Deal_Create:
		return OnHandleCreateDeal(pCmd);

	}
	
	return true;
}
//bool FishServer::HandleLogonMsg(BYTE LogonID,NetCmd* pCmd)
//{
//	if (!pCmd)
//		return false;
//	//登陆服务器发送来的数据
//	switch (pCmd->CmdType)	
//	{
//	case Main_Logon:
//		OnHandLogonLogonMsg(LogonID,pCmd);
//		return true;
//	}
//	return true;
//}
bool FishServer::HandleCenterMsg(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	//中央服务器发送来的数据
	switch (pCmd->CmdType)
	{
	case Main_Logon:
		OnHandLogonLogonMsg(pCmd);
		return true;
	case Main_Relation:
		OnHandleSocketRelation(pCmd);
		return true;
	case Main_Mail:
		OnHandleSocketMail(pCmd);
		return true;
	case Main_Month:
		OnHandleSocketMonth(pCmd);
		return true;
	case Main_Charm:
		OnHandleSocketCharm(pCmd);
		return true;
	case Main_Center:
		OnHandleSocketCenter(pCmd);
		return true;
	case Main_Giff:
		OnHandleSocketGiff(pCmd);
		return true;
	case Main_Task:
		OnHandleSocketTask(pCmd);
		return true;
	case Main_Char:
		OnHandleSocketChar(pCmd);
		return true;
	case Main_RelationRequest:
		OnHandleSocketRelationRequest(pCmd);

		return true;

		
	case Main_Achievement:
		{
			switch (pCmd->SubCmdType)
			{
			case LC_AchievementList:
				{
					//处理成就点的排名 
					LC_Cmd_AchievementList* pMsg = (LC_Cmd_AchievementList*)pCmd;
					if ((pMsg->States & MsgBegin) != 0)
					{
						m_AchjievementList.clear();
					}
					for (WORD i = 0; i < pMsg->Sum; ++i)
					{
						HashMap<DWORD, WORD>::iterator Iter = m_AchjievementList.find(pMsg->Array[i]);
						if (Iter == m_AchjievementList.end())
						{
							m_AchjievementList.insert(HashMap<DWORD, WORD>::value_type(pMsg->Array[i], i));
						}
						else
						{
							Iter->second = i;
						}
					}
					if ((pMsg->States & MsgEnd) != 0)
					{
						//便利所有的情况
						HashMap<DWORD, CRoleEx*>& pMap = m_RoleManager.GetAllRole();
						HashMap<DWORD, CRoleEx*>::iterator IterRole = pMap.begin();
						for (; IterRole != pMap.end(); ++IterRole)
						{
							if (!IterRole->second)
								continue;
							HashMap<DWORD, WORD>::iterator IterFind = m_AchjievementList.find(IterRole->first);
							if (IterFind == m_AchjievementList.end())
							{
								//当前玩家不在排名内
								if (IterRole->second->GetRoleInfo().AchievementPointIndex != 0xffffffff)
								{
									//发送命令到客户端  告诉玩家 成就排名变化了
									IterRole->second->GetRoleInfo().AchievementPointIndex = 0xffffffff;
									LC_Cmd_ChangeRoleAchievementIndex msg;
									SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleAchievementIndex), sizeof(LC_Cmd_ChangeRoleAchievementIndex));
									msg.AchievementIndex = 0xffffffff;
									IterRole->second->SendDataToClient(&msg);
								}
							}
							else
							{
								if (IterFind->second != IterRole->second->GetRoleInfo().AchievementPointIndex)
								{
									//发送命令到客户端  告诉玩家 成就排名变化了
									IterRole->second->GetRoleInfo().AchievementPointIndex = IterFind->second;
									LC_Cmd_ChangeRoleAchievementIndex msg;
									SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleAchievementIndex), sizeof(LC_Cmd_ChangeRoleAchievementIndex));
									msg.AchievementIndex = IterFind->second;
									IterRole->second->SendDataToClient(&msg);
								}
							}
						}
					}					   
				}
				break;
			}
		}
		break;
	//case Main_Server:
	//	{
	//		//中央服务器发送来的构架
	//		switch (pCmd->SubCmdType)
	//		{
	//		case CG_RsgGame:
	//			{
	//				CG_Cmd_RsgGame* pMsg = (CG_Cmd_RsgGame*)pCmd;

	//				//先处理 一些中央服务器同步的数据
	//				SendAllMonthPlayerSumToClient(0);//将全部比赛的数据发送到客户端去

	//				m_GameServerManager.OnRsgGameToCenterResult(pMsg);//GameServer向中央服务器注册的结果
	//				return true;
	//			}
	//			break;
	//		case CG_RsgLogon:
	//			{
	//				//一个Logon加入Center了
	//				CG_Cmd_RsgLogon* pMsg = (CG_Cmd_RsgLogon*)pCmd;
	//				m_GameServerManager.OnLogonRsgCenter(pMsg->LogonConfigID);
	//				return true;
	//			}
	//			break;
	//		case CG_UnRsgLogon:
	//			{
	//				//一个Logon离开Center了
	//				CG_Cmd_UnRsgLogon* pMsg = (CG_Cmd_UnRsgLogon*)pCmd;
	//				m_GameServerManager.OnLogonLeaveCenter(pMsg->LogonConfigID);
	//				return true;
	//			}
	//			break;
	//		//case CL_RsgUser:
	//		//	{
	//		//		CL_Cmd_RsgUser* pMsg = (CL_Cmd_RsgUser*)pCmd;
	//		//		//m_OnlineRoleMap.insert(HashMap<DWORD, DWORD>::value_type(pMsg->PlayerInfo.dwUserID, pMsg->PlayerInfo.GameConfigID));//注册在线的玩家
	//		//		if (pMsg->PlayerInfo.GameConfigID != ConvertIntToDWORD(m_GameNetworkID))
	//		//			m_RoleLogonManager.OnDleRoleOnlyInfo(pMsg->PlayerInfo.dwUserID);
	//		//		return true;
	//		//	}
	//		//	break;
	//		//case CL_UnRsgUser:
	//		//	{
	//		//		CL_Cmd_UnRsgUser* pMsg = (CL_Cmd_UnRsgUser*)pCmd;
	//		//		HashMap<DWORD, DWORD>::iterator Iter = m_OnlineRoleMap.find(pMsg->PlayerInfo.dwUserID);
	//		//		if (Iter == m_OnlineRoleMap.end())
	//		//		{
	//		//			ASSERT(false);
	//		//			return false;
	//		//		}
	//		//		if (Iter->second != pMsg->PlayerInfo.GameConfigID)
	//		//			ASSERT(false);
	//		//		m_OnlineRoleMap.erase(Iter);//移除玩家
	//		//		return true;
	//		//	}
	//		//	break;
	//		//case CL_RsgLogon:
	//		//	{
	//		//		//注册其他玩家的数据
	//		//		CL_Cmd_RsgLogon* pMsg = (CL_Cmd_RsgLogon*)pCmd;
	//		//		if ((pMsg->States & MsgBegin) != 0)
	//		//		{
	//		//			m_OnlineRoleMap.clear();
	//		//		}
	//		//		for (size_t i = 0; i < pMsg->Sum; ++i)
	//		//		{
	//		//			m_OnlineRoleMap.insert(HashMap<DWORD, DWORD>::value_type(pMsg->Array[i].dwUserID, pMsg->Array[i].GameConfigID));
	//		//		}
	//		//		return true;
	//		//	}
	//		//	break;
	//		}
	//		return true;
	//	}
	case Main_Message:
		{
			return OnHandleSocketMessage(pCmd);
		}
	case Main_Announcement:
		{
			//处理公告的数据
			switch (pCmd->SubCmdType)
			{
			case CG_GetAllAnnouncement:
				{
					CG_Cmd_GetAllAnnouncement* pMsg = (CG_Cmd_GetAllAnnouncement*)pCmd;
					m_AnnouncementManager.OnLoadAllAnnouncementInfoByCenter(pMsg);
					return true;
				}
			/*case CG_GetAllAnnouncementFinish:
				{
					m_AnnouncementManager.OnLoadAllAnnouncementInfoFinish();
					return true;
				}*/
			case CG_SendNewAnnouncementOnce:
				{
					CG_Cmd_SendNewAnnouncementOnce* pMsg = (CG_Cmd_SendNewAnnouncementOnce*)pCmd;
					m_AnnouncementManager.OnAddNewAnnouncementOnceByCenter(pMsg->pOnce);
					return true;
				}
			}
			ASSERT(false);
			return false;
		}
	case Main_OperatorSystemMail:
		{
			switch (pCmd->SubCmdType)
			{
			case CG_AddOperatorSystemMail:
			{
				CG_Cmd_AddNewOperatorMail* pMsg = (CG_Cmd_AddNewOperatorMail*)pCmd;
				_SystemMailManager.AddNewMail(pMsg->mail);
			}
			break;
			case CG_GetAllSystemMail:
			{
				CG_Cmd_GetAllOperatorSystemMail* pMsg = (CG_Cmd_GetAllOperatorSystemMail*)pCmd;
				_SystemMailManager.OnLoadAllOperatorSystemMailByCentral(pMsg);
			}
			break;
			default:
				break;
			}
			return true;
		}
		break;
	case Main_Control:
		{
			switch (pCmd->SubCmdType)
			{
			case CL_KickUserByID:
				{
					CL_Cmd_KickUserByID* pMsg = (CL_Cmd_KickUserByID*)pCmd;
					LC_Cmd_KickUserResult msg;
					SetMsgInfo(msg, GetMsgType(Main_Control, LC_KickUserResult), sizeof(LC_Cmd_KickUserResult));
					msg.Result = KickUserByID(pMsg->dwUserID,pMsg->FreezeMin);
					msg.dwUserID = pMsg->dwUserID;
					msg.ClientID = pMsg->ClientID;
					SendNetCmdToControl(&msg);
					return true;
				}
				break;
			case GM_CL_QUERY_USER_INFO:
				{
					GM_CL_QueryUserInfoReq* pMsg = (GM_CL_QueryUserInfoReq*)pCmd;
					CE_GM_QueryUserACK msg;
					SetMsgInfo(msg, GetMsgType(Main_Control, CE_GM_QUERY_USER_ACK), sizeof(CE_GM_QueryUserACK));
					QueryUserType cur_type = (QueryUserType)pMsg->Type;
					HashMap<DWORD, CRoleEx*>& roles = GetRoleManager()->GetAllRole();
					HashMap<DWORD, CRoleEx*>::iterator it = roles.begin();
					msg.count = 0;
					for (; it != roles.end(); ++ it)
					{
						bool find = false;
						bool add = false;
						CRoleEx* role = it->second;
						const tagRoleInfo& RoleInfo = role->GetRoleInfo();
						switch (cur_type)
						{
						case 	QueryUserType_All:
						case 	QueryUserType_Online:
						case 	QueryUserType_Offline:
						{
							add = true;

						}
						break;
						case 	QueryUserType_ByNickName:
						{
							if (StrCmpW(pMsg->NickName, RoleInfo.NickName) == true)
							{
								add = true;
							}
							
						}
						break;
						default:
							break;
						}

						if (add)
						{
							msg.Role[msg.count] = RoleInfo;
							msg.count++;
							if (msg.count >= MAXQUERYALLUSERINFO)
							{
								SendNetCmdToControl(&msg);
								msg.count = 0;
							}
						}
					}
					msg.end = true;
					SendNetCmdToControl(&msg);
				}
			break;
			case CL_QueryOnlineUserInfo:
				{
					CL_Cmd_QueryOnlineUserInfo* pMsg = (CL_Cmd_QueryOnlineUserInfo*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return false;
					}
					CRoleEx * pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
					if (pRole)
					{
						LC_Cmd_QueryOnlineUserInfo msg;
						SetMsgInfo(msg, GetMsgType(Main_Control, LC_QueryOnlineUserInfo), sizeof(LC_Cmd_QueryOnlineUserInfo));
						msg.ClientID = pMsg->ClientID;
						msg.Result = true;
						msg.RoleInfo = pRole->GetRoleInfo();
						SendNetCmdToControl(&msg);
						return true;
					}
					else
					{
						LC_Cmd_QueryOnlineUserInfo msg;
						SetMsgInfo(msg, GetMsgType(Main_Control, LC_QueryOnlineUserInfo), sizeof(LC_Cmd_QueryOnlineUserInfo));
						msg.ClientID = pMsg->ClientID;
						msg.Result = false;
						SendNetCmdToControl(&msg);
						return true;
					}
				}
				break;
			case CL_ChangeaNickName:
				{
					CL_Cmd_ChangeaNickName* pMsg = (CL_Cmd_ChangeaNickName*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return false;
					}
					CRoleEx * pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
					if (!pRole)
					{
						ASSERT(false);
						return false;
					}
					else
					{
						pRole->ChangeRoleNickName(pMsg->NickName);
						return true;
					}
				}
				break;
			case CL_ChangeParticularStates:
				{
					CL_Cmd_ChangeParticularStates*pMsg = (CL_Cmd_ChangeParticularStates*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return false;
					}
					CRoleEx * pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
					if (!pRole)
					{
						DBR_Cmd_SaveRoleParticularStates msgDB;
						SetMsgInfo(msgDB, DBR_SaveRoleParticularStates, sizeof(msgDB));
						msgDB.dwUserID = pMsg->dwUserID;
						msgDB.ParticularStates = pMsg->ParticularStates;
						SendNetCmdToSaveDB(&msgDB);

						LC_Cmd_ChangeParticularStates msg;
						SetMsgInfo(msg, GetMsgType(Main_Control, LC_ChangeParticularStates), sizeof(LC_Cmd_ChangeParticularStates));
						msg.dwUserID = pMsg->dwUserID;
						msg.ParticularStates = pMsg->ParticularStates;
						msg.ClientID = pMsg->ClientID;
						msg.Result = false;
						SendNetCmdToControl(&msg);
						return true;
					}
					else
					{
						LC_Cmd_ChangeParticularStates msg;
						SetMsgInfo(msg, GetMsgType(Main_Control, LC_ChangeParticularStates), sizeof(LC_Cmd_ChangeParticularStates));
						msg.dwUserID = pMsg->dwUserID;
						msg.ParticularStates = pMsg->ParticularStates;
						msg.ClientID = pMsg->ClientID;
						msg.Result = pRole->ChangeRoleParticularStates(pMsg->ParticularStates);
						SendNetCmdToControl(&msg);
						return true;
					}
					return true;;
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
			//		//将指定玩家剔除 并且 删除缓存数据
			//		CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
			//		if (!pRole)
			//		{
			//			ASSERT(false);
			//			return false;
			//		}
			//		KickUserByID(pRole->GetUserID());
			//	}
			//	break;
			}
			ASSERT(false);
			return false;
		}
	case Main_Operate:
		{
			switch (pCmd->SubCmdType)
			{
			case CG_Deal_Successful:
				{
					CG_Cmd_Deal_Successful* pMsg = (CG_Cmd_Deal_Successful*)pCmd;
					CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserid);
					int AddRechargeSum = pMsg->dwAddRechargeSum;
					if (!pRole)
					{
						DBR_Cmd_AddRoleTotalRecharge msg;
						SetMsgInfo(msg, DBR_AddRoleTotalRecharge, sizeof(DBR_Cmd_AddRoleTotalRecharge));
						msg.dwUserID = pMsg->dwUserid;
						msg.Sum = AddRechargeSum;
						g_FishServer.SendNetCmdToDB(&msg);
					}
					else
					{						
						pRole->ChangeRoleTotalRechargeSum(AddRechargeSum);//添加玩家总充值数
						pRole->OnHandleEvent(true, true, true, ET_Recharge, 0, pRole->GetRoleInfo().TotalRechargeSum/*Iter->second.dDisCountPrice*/);//充值记录
						CG_Cmd_Deal_NotifyClient msg;
						SetMsgInfo(msg, GetMsgType(Main_Operate, CG_Deal_NotifyClient), sizeof(CG_Cmd_Deal_NotifyClient));
						pRole->SendDataToClient(&msg);
					}
					return true;
				}
			case CG_BindEmail:
				{
					CG_Cmd_BindEmail* pMsg = (CG_Cmd_BindEmail*)pCmd;
					CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
					if (!pRole)
					{
						ASSERT(false);
						return false;
					}
					if (pMsg->Result)
					{
						pRole->GetRoleEntity().OnChangeRoleEmail(pMsg->EMail);
					}
					LC_Cmd_BindEmail msg;
					SetMsgInfo(msg, GetMsgType(Main_Operate, LC_BindEmail), sizeof(LC_Cmd_BindEmail));
					msg.ErrorID = pMsg->ErrorID;
					TCHARCopy(msg.EMail, CountArray(msg.EMail), pMsg->EMail, _tcslen(pMsg->EMail));
					pRole->SendDataToClient(&msg);
					return true;
				}
			case CG_UseRMB:
				{
					//g_FishServer.ShowInfoToWin("接收到渠道服务器进行充值1");
					CG_Cmd_UseRMB* pMsg = (CG_Cmd_UseRMB*)pCmd;
					OnHandleUseRMB(pMsg);
					return true;
				}

			case CG_AddNormalOrderID:
				{
					CG_Cmd_AddNormalOrderID* pMsg = (CG_Cmd_AddNormalOrderID*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return false;
					}
					CRoleEx * pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
					if (!pRole)
					{
						ASSERT(false);
						return false;
					}
					LC_Cmd_AddNormalOrderID msg;
					SetMsgInfo(msg, GetMsgType(Main_Operate, LC_AddNormalOrderID), sizeof(LC_Cmd_AddNormalOrderID));
					msg.OrderID = pMsg->OrderID;
					msg.ShopID = pMsg->ShopID;
					msg.Result = pMsg->Result;
					strncpy_s(msg.Transid, CountArray(msg.Transid), pMsg->Transid, strlen(pMsg->Transid));
					strncpy_s(msg.Sign, CountArray(msg.Sign), pMsg->Sign, strlen(pMsg->Sign));
					pRole->SendDataToClient(&msg);
					return true;
				}
			case CG_PhonePay:
				{
					CG_Cmd_PhonePay* pMsg = (CG_Cmd_PhonePay*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return false;
					}
					OnHandlePhonePay(pMsg);
					return true;
				}	
			}
		}
	}
	return true;
}
void FishServer::OnHandlePhonePay(CG_Cmd_PhonePay* pMsg)
{
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		if (pMsg->Result)
		{
			HashMap<BYTE, tagShopConfig>::iterator IterShop = g_FishServer.GetFishConfig().GetShopConfig().ShopMap.find(pMsg->ShopID);
			if (IterShop == g_FishServer.GetFishConfig().GetShopConfig().ShopMap.end())
			{
				ASSERT(false);
				return ;
			}
			HashMap<BYTE, tagShopItemConfig>::iterator IterShopItem = IterShop->second.ShopItemMap.find(pMsg->ShopOnlyID);
			if (IterShopItem == IterShop->second.ShopItemMap.end())
			{
				ASSERT(false);
				return ;
			}
			if (IterShopItem->second.ShopType != SIT_PhonePay)
			{
				ASSERT(false);
				return ;
			}
			tagItemConfig* pItemConfig = GetFishConfig().GetItemInfo(IterShopItem->second.ItemInfo.ItemID);
			if (!pItemConfig)
			{
				ASSERT(false);
				return ;
			}

			/*DBR_Cmd_AddRoleCashSum msg;
			msg.UserID = pMsg->dwUserID;
			msg.AddCashSum = 1;
			SendNetCmdToDB(&msg);*/

			DWORD FacePice = pItemConfig->ItemParam * IterShopItem->second.ItemInfo.ItemSum * pMsg->ShopSum;
			g_DBLogManager.LogRolePhonePayLogToDB("游戏服务器订单成功:玩家不在线", pMsg->OrderID, pMsg->dwUserID, pMsg->Phone, FacePice, SendLogDB);

			DBR_Cmd_ChangeRoleShareStates msg;
			SetMsgInfo(msg, DBR_ChangeRoleShareStates, sizeof(DBR_Cmd_ChangeRoleShareStates));
			msg.States = false;
			msg.dwUserID = pMsg->dwUserID;
			g_FishServer.SendNetCmdToSaveDB(&msg);

			return ;
		}
		else
		{
			HashMap<BYTE, tagShopConfig>::iterator IterShop = g_FishServer.GetFishConfig().GetShopConfig().ShopMap.find(pMsg->ShopID);
			if (IterShop == g_FishServer.GetFishConfig().GetShopConfig().ShopMap.end())
			{
				ASSERT(false);
				return ;
			}
			HashMap<BYTE, tagShopItemConfig>::iterator IterShopItem = IterShop->second.ShopItemMap.find(pMsg->ShopOnlyID);
			if (IterShopItem == IterShop->second.ShopItemMap.end())
			{
				ASSERT(false);
				return ;
			}
			if (IterShopItem->second.ShopType != SIT_PhonePay)
			{
				ASSERT(false);
				return ;
			}

			//因为充值花费失败 我们想要离线归还玩家的金币
			//DBR_Cmd_ChangeRoleMoney msg;
			//SetMsgInfo(msg, DBR_ChangeRoleMoney, sizeof(DBR_Cmd_ChangeRoleMoney));
			//msg.dwUserID = pMsg->dwUserID;
			////获取需要归还的钱币的数量
			//msg.GlobelSum = IterShopItem->second.PriceGlobel * pMsg->ShopSum;
			//msg.MedalSum = IterShopItem->second.PriceMabel * pMsg->ShopSum;
			//msg.CurrceySum = IterShopItem->second.PriceCurrey * pMsg->ShopSum;
			//SendNetCmdToDB(&msg);

			if (IterShopItem->second.PriceMabel * pMsg->ShopSum > 0)
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("手机话费充值失败 我们将奖牌通过邮件退还给您"), _tcslen(TEXT("手机话费充值失败 我们将奖牌通过邮件退还给您")));
				MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailMedalRewradID;
				MailInfo.RewardSum = IterShopItem->second.PriceMabel * pMsg->ShopSum;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
				DBR_Cmd_AddUserMail msg;
				SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
				msg.dwDestUserID = pMsg->dwUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToDB(&msg);
			}
			if (IterShopItem->second.PriceGlobel * pMsg->ShopSum > 0)
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("手机话费充值失败 我们将金币通过邮件退还给您"), _tcslen(TEXT("手机话费充值失败 我们将奖牌通过邮件退还给您")));
				MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
				MailInfo.RewardSum = IterShopItem->second.PriceGlobel * pMsg->ShopSum;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
				DBR_Cmd_AddUserMail msg;
				SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
				msg.dwDestUserID = pMsg->dwUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToDB(&msg);
			}
			if (IterShopItem->second.PriceCurrey * pMsg->ShopSum > 0)
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("手机话费充值失败 我们将钻石通过邮件退还给您"), _tcslen(TEXT("手机话费充值失败 我们将奖牌通过邮件退还给您")));
				MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailCurrceyRewardID;
				MailInfo.RewardSum = IterShopItem->second.PriceCurrey * pMsg->ShopSum;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
				DBR_Cmd_AddUserMail msg;
				SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
				msg.dwDestUserID = pMsg->dwUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToDB(&msg);
			}

			DBR_Cmd_DelRoleCashSum msg;
			SetMsgInfo(msg, DBR_DelRoleCashSum, sizeof(DBR_Cmd_DelRoleCashSum));
			msg.UserID = pMsg->dwUserID;
			msg.DelCashSum = 1;
			SendNetCmdToDB(&msg);
		}
		return;
	}
	else
	{
		if (pMsg->Result)
		{
			HashMap<BYTE, tagShopConfig>::iterator IterShop = g_FishServer.GetFishConfig().GetShopConfig().ShopMap.find(pMsg->ShopID);
			if (IterShop == g_FishServer.GetFishConfig().GetShopConfig().ShopMap.end())
			{
				ASSERT(false);
				return;
			}
			HashMap<BYTE, tagShopItemConfig>::iterator IterShopItem = IterShop->second.ShopItemMap.find(pMsg->ShopOnlyID);
			if (IterShopItem == IterShop->second.ShopItemMap.end())
			{
				ASSERT(false);
				return;
			}
			if (IterShopItem->second.ShopType != SIT_PhonePay)
			{
				ASSERT(false);
				return;
			}
			tagItemConfig* pItemConfig = GetFishConfig().GetItemInfo(IterShopItem->second.ItemInfo.ItemID);
			if (!pItemConfig)
			{
				ASSERT(false);
				return;
			}

			//pRole->ChangeRoleCashSum(1);

			DWORD FacePice = pItemConfig->ItemParam * IterShopItem->second.ItemInfo.ItemSum * pMsg->ShopSum;
			g_DBLogManager.LogRolePhonePayLogToDB("游戏服务器订单成功", pMsg->OrderID, pMsg->dwUserID, pMsg->Phone, FacePice, SendLogDB);

			GetAnnouncementManager().OnAddNewAnnouncementOnce(pRole->GetRoleInfo().NickName, pMsg->ShopID, pMsg->ShopOnlyID);
			LC_Cmd_PhonePay msg;
			SetMsgInfo(msg, GetMsgType(Main_Operate, LC_PhonePay), sizeof(LC_Cmd_PhonePay));
			msg.ErrorID = pMsg->ErrorID;
			pRole->SendDataToClient(&msg);

			pRole->ChangeRoleShareStates(false);
		}
		else
		{
			//玩家在线 我们直接退换玩家货币 通知客户端
			HashMap<BYTE, tagShopConfig>::iterator IterShop = g_FishServer.GetFishConfig().GetShopConfig().ShopMap.find(pMsg->ShopID);
			if (IterShop == g_FishServer.GetFishConfig().GetShopConfig().ShopMap.end())
			{
				ASSERT(false);
				return;
			}
			HashMap<BYTE, tagShopItemConfig>::iterator IterShopItem = IterShop->second.ShopItemMap.find(pMsg->ShopOnlyID);
			if (IterShopItem == IterShop->second.ShopItemMap.end())
			{
				ASSERT(false);
				return;
			}
			if (IterShopItem->second.ShopType != SIT_PhonePay)
			{
				ASSERT(false);
				return;
			}
			/*pRole->ChangeRoleGlobe(IterShopItem->second.PriceGlobel * pMsg->ShopSum, true);
			pRole->ChangeRoleMedal(IterShopItem->second.PriceMabel * pMsg->ShopSum, TEXT("充值话费失败退还"));
			pRole->ChangeRoleCurrency(IterShopItem->second.PriceCurrey * pMsg->ShopSum, TEXT("充值话费失败退还"));*/

			if (IterShopItem->second.PriceMabel * pMsg->ShopSum > 0)
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("手机话费充值失败 我们将奖牌通过邮件退还给您"), _tcslen(TEXT("手机话费充值失败 我们将奖牌通过邮件退还给您")));
				MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailMedalRewradID;
				MailInfo.RewardSum = IterShopItem->second.PriceMabel * pMsg->ShopSum;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
				DBR_Cmd_AddUserMail msg;
				SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
				msg.dwDestUserID = pMsg->dwUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToDB(&msg);
			}
			if (IterShopItem->second.PriceGlobel * pMsg->ShopSum > 0)
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("手机话费充值失败 我们将金币通过邮件退还给您"), _tcslen(TEXT("手机话费充值失败 我们将奖牌通过邮件退还给您")));
				MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
				MailInfo.RewardSum = IterShopItem->second.PriceGlobel * pMsg->ShopSum;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
				DBR_Cmd_AddUserMail msg;
				SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
				msg.dwDestUserID = pMsg->dwUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToDB(&msg);
			}
			if (IterShopItem->second.PriceCurrey * pMsg->ShopSum > 0)
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("手机话费充值失败 我们将钻石通过邮件退还给您"), _tcslen(TEXT("手机话费充值失败 我们将奖牌通过邮件退还给您")));
				MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailCurrceyRewardID;
				MailInfo.RewardSum = IterShopItem->second.PriceCurrey * pMsg->ShopSum;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
				DBR_Cmd_AddUserMail msg;
				SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
				msg.dwDestUserID = pMsg->dwUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToDB(&msg);
			}

			pRole->ChangeRoleCashSum(-1);

			LC_Cmd_PhonePay msg;
			SetMsgInfo(msg, GetMsgType(Main_Operate, LC_PhonePay), sizeof(LC_Cmd_PhonePay));
			msg.ErrorID = pMsg->ErrorID;
			pRole->SendDataToClient(&msg);
		}
		return;
	}
}
void FishServer::OnHandleUseRMB(CG_Cmd_UseRMB* pMsg)
{
	vector<TCHAR*> pVec;
	GetStringArrayVecByData(pVec, &pMsg->rechargeInfo);
	if (pVec.size() != pMsg->rechargeInfo.HandleSum || pVec.size()<4)
	{
		FreeVec(pVec);
		ASSERT(false);
		return;
	}

	HashMap<DWORD, tagFishRechargeInfo>::iterator Iter = m_FishConfig.GetFishRechargesConfig().m_FishRechargeMap.find(pMsg->rechargeInfo.ShopItemID);
	if (Iter == m_FishConfig.GetFishRechargesConfig().m_FishRechargeMap.end() || Iter->second.dDisCountPrice * 100 != pMsg->rechargeInfo.Price)
	{
		UINT Count = 0;
		char* OrderID = WCharToChar(pVec[0], Count);
		char* ChannelCode = WCharToChar(pVec[1], Count);
		char* ChannelOrderID = WCharToChar(pVec[2], Count);
		char* ChannelLabel = WCharToChar(pVec[3], Count);

		g_DBLogManager.LogUserRechargeLogToDB("游戏服务器验证:价格不正确", OrderID, pMsg->rechargeInfo.UserID, ChannelCode, ChannelOrderID, ChannelLabel, pMsg->rechargeInfo.ShopItemID, pMsg->rechargeInfo.Price, pMsg->rechargeInfo.FreePrice, 0, 0, Iter->second.IsAddGlobel() ? Iter->second.AddMoney : 0, Iter->second.IsAddCurrcey() ? Iter->second.AddMoney : 0, Iter->second.IsAddReward() ? Iter->second.RewardID : 0, SendLogDB);

		FreeVec(pVec);
		free(OrderID);
		free(ChannelCode);
		free(ChannelOrderID);
		free(ChannelLabel);

		ASSERT(false);
		return;
	}

	//获取到玩家进行充值的命令 我们进行处理
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->rechargeInfo.UserID);
	if (!pRole)
	{
		/*DWORD PageSize = sizeof(DBR_Cmd_AddRecharge)+sizeof(BYTE)*(pMsg->rechargeInfo.Sum - 1);
		DBR_Cmd_AddRecharge* msg = (DBR_Cmd_AddRecharge*)malloc(PageSize);
		msg->SetCmdType(DBR_AddRecharge);
		msg->SetCmdSize(ConvertDWORDToWORD(PageSize));
		if (Iter->second.IsCurreyOrGlobel)
		{
		msg->AddCurrceySum = Iter->second.AddMoney;
		msg->AddGlobelSum = 0;
		}
		else
		{
		msg->AddCurrceySum = 0;
		msg->AddGlobelSum = Iter->second.AddMoney;
		}
		msg->IsFirst = Iter->second.IsFirstPay;
		memcpy_s(&msg->rechargeInfo, (pMsg->rechargeInfo.Sum - 1)*sizeof(BYTE)+sizeof(RechageInfo), &pMsg->rechargeInfo, (pMsg->rechargeInfo.Sum - 1)*sizeof(BYTE)+sizeof(RechageInfo));
		SendNetCmdToSaveDB(msg);
		free(msg);*/

		if (Iter->second.IsAddCurrcey())
		{
			/*msg->AddCurrceySum = Iter->second.AddMoney;
			msg->AddGlobelSum = 0;*/

			tagRoleMail	MailInfo;
			MailInfo.bIsRead = false;
			//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
			TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("恭喜您充值成功 因为您不在线 所有充值奖励通过邮件发送给您"), _tcslen(TEXT("恭喜您充值成功 因为您不在线 所有充值奖励通过邮件发送给您")));
			MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailCurrceyRewardID;
			MailInfo.RewardSum = Iter->second.AddMoney;
			MailInfo.MailID = 0;
			MailInfo.SendTimeLog = time(NULL);
			MailInfo.SrcFaceID = 0;
			TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
			MailInfo.SrcUserID = 0;//系统发送
			MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
			DBR_Cmd_AddUserMail msg;
			SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
			msg.dwDestUserID = pMsg->rechargeInfo.UserID;
			msg.MailInfo = MailInfo;
			g_FishServer.SendNetCmdToDB(&msg);
		}
		else if (Iter->second.IsAddGlobel())
		{
			/*msg->AddCurrceySum = 0;
			msg->AddGlobelSum = Iter->second.AddMoney;*/

			tagRoleMail	MailInfo;
			MailInfo.bIsRead = false;
			//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
			TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("恭喜您充值成功 因为您不在线 所有充值奖励通过邮件发送给您"), _tcslen(TEXT("恭喜您充值成功 因为您不在线 所有充值奖励通过邮件发送给您")));
			MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
			MailInfo.RewardSum = Iter->second.AddMoney;
			MailInfo.MailID = 0;
			MailInfo.SendTimeLog = time(NULL);
			MailInfo.SrcFaceID = 0;
			TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
			MailInfo.SrcUserID = 0;//系统发送
			MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
			DBR_Cmd_AddUserMail msg;
			SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
			msg.dwDestUserID = pMsg->rechargeInfo.UserID;
			msg.MailInfo = MailInfo;
			g_FishServer.SendNetCmdToDB(&msg);
		}
		else if (Iter->second.IsAddReward())
		{
			//如果为奖励ID的话 我们按邮件发送
			//发送邮件奖励 系统邮件直接携带RewardID 进行处理
			tagRoleMail	MailInfo;
			MailInfo.bIsRead = false;
			//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
			TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("恭喜您充值成功 因为您不在线 所有充值奖励通过邮件发送给您"), _tcslen(TEXT("恭喜您充值成功 因为您不在线 所有充值奖励通过邮件发送给您")));
			MailInfo.RewardID = Iter->second.RewardID;
			MailInfo.RewardSum = 1;
			MailInfo.MailID = 0;
			MailInfo.SendTimeLog = time(NULL);
			MailInfo.SrcFaceID = 0;
			TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
			MailInfo.SrcUserID = 0;//系统发送
			MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
			DBR_Cmd_AddUserMail msg;
			SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
			msg.dwDestUserID = pMsg->rechargeInfo.UserID;
			msg.MailInfo = MailInfo;
			g_FishServer.SendNetCmdToDB(&msg);
		}
		else
		{
			ASSERT(false);
			FreeVec(pVec);
			return;
		}


		UINT Count = 0;
		char* OrderID = WCharToChar(pVec[0], Count);
		char* ChannelCode = WCharToChar(pVec[1], Count);

		char* ChannelOrderID = WCharToChar(pVec[2], Count);
		char* ChannelLabel = WCharToChar(pVec[3], Count);
		g_DBLogManager.LogUserRechargeLogToDB("游戏服务器充值成功:玩家不在线", OrderID, pMsg->rechargeInfo.UserID, ChannelCode, ChannelOrderID, ChannelLabel, pMsg->rechargeInfo.ShopItemID, pMsg->rechargeInfo.Price, pMsg->rechargeInfo.FreePrice, 0, 0, Iter->second.IsAddGlobel() ? Iter->second.AddMoney : 0, Iter->second.IsAddCurrcey() ? Iter->second.AddMoney : 0, Iter->second.IsAddReward() ? Iter->second.RewardID : 0, SendLogDB);

		DBR_Cmd_AddRoleTotalRecharge msg;
		SetMsgInfo(msg, DBR_AddRoleTotalRecharge, sizeof(DBR_Cmd_AddRoleTotalRecharge));
		msg.dwUserID = pMsg->rechargeInfo.UserID;
		msg.Sum = pMsg->rechargeInfo.Price;
		g_FishServer.SendNetCmdToDB(&msg);

		FreeVec(pVec);
		free(OrderID);
		free(ChannelCode);
		free(ChannelOrderID);
		free(ChannelLabel);
	}
	else
	{
		//因为玩家 操作 基本上玩家一定在想 因为 有玩家缓存存在
		//判断当前玩家的充值是否正确的 
		UINT Count = 0;
		char* OrderID = WCharToChar(pVec[0], Count);
		char* ChannelCode = WCharToChar(pVec[1], Count);
		char* ChannelOrderID = WCharToChar(pVec[2], Count);
		char* ChannelLabel = WCharToChar(pVec[3], Count);

		if (Iter->second.IsAddCurrcey() && Iter->second.IsFirstAdd() && !pRole->GetRoleInfo().bIsFirstPayCurrcey)
		{
			g_DBLogManager.LogUserRechargeLogToDB("游戏服务器充值失败:玩家不是钻石首冲", OrderID, pMsg->rechargeInfo.UserID, ChannelCode, ChannelOrderID, ChannelLabel, pMsg->rechargeInfo.ShopItemID, pMsg->rechargeInfo.Price, pMsg->rechargeInfo.FreePrice, 0, 0, Iter->second.IsAddGlobel() ? Iter->second.AddMoney : 0, Iter->second.IsAddCurrcey() ? Iter->second.AddMoney : 0, Iter->second.IsAddReward() ? Iter->second.RewardID : 0, SendLogDB);
			FreeVec(pVec);
			free(OrderID);
			free(ChannelCode);
			free(ChannelOrderID);
			free(ChannelLabel);
			return;
		}
		if (Iter->second.IsAddGlobel() && Iter->second.IsFirstAdd() && !pRole->GetRoleInfo().bIsFirstPayGlobel)
		{
			g_DBLogManager.LogUserRechargeLogToDB("游戏服务器充值失败:玩家不是金币首冲", OrderID, pMsg->rechargeInfo.UserID, ChannelCode, ChannelOrderID, ChannelLabel, pMsg->rechargeInfo.ShopItemID, pMsg->rechargeInfo.Price, pMsg->rechargeInfo.FreePrice, 0, 0, Iter->second.IsAddGlobel() ? Iter->second.AddMoney : 0, Iter->second.IsAddCurrcey() ? Iter->second.AddMoney : 0, Iter->second.IsAddReward() ? Iter->second.RewardID : 0, SendLogDB);
			FreeVec(pVec);
			free(OrderID);
			free(ChannelCode);
			free(ChannelOrderID);
			free(ChannelLabel);
			return;
		}
		//直接操作玩家
		DWORD AddMoney = static_cast<DWORD>(pRole->GetRoleVip().AddReChargeRate() * Iter->second.AddMoney);
		if (Iter->second.IsAddCurrcey())
		{
			DWORD  OldCurrcey = pRole->GetRoleInfo().dwCurrencyNum;
			DWORD  OldGlobel = pRole->GetRoleInfo().dwGlobeNum;
			pRole->ChangeRoleCurrency(AddMoney, TEXT("充值获得钻石"));
			if (Iter->second.IsFirstAdd())
				pRole->ChangeRoleIsFirstPayCurrcey();
		}
		else if (Iter->second.IsAddGlobel())
		{
			DWORD  OldCurrcey = pRole->GetRoleInfo().dwCurrencyNum;
			DWORD  OldGlobel = pRole->GetRoleInfo().dwGlobeNum;
			pRole->ChangeRoleGlobe(AddMoney, true, true, true);
			if (Iter->second.IsFirstAdd())
				pRole->ChangeRoleIsFirstPayGlobel();
		}
		else if (Iter->second.IsAddReward())
		{
			pRole->OnAddRoleRewardByRewardID(Iter->second.RewardID, TEXT("充值获得奖励"));
		}
		pRole->ChangeRoleTotalRechargeSum(Iter->second.dDisCountPrice);//添加玩家总充值数
		pRole->OnHandleEvent(true, true, true, ET_Recharge, 0, pRole->GetRoleInfo().TotalRechargeSum /*Iter->second.dDisCountPrice*/);//充值记录

		g_DBLogManager.LogUserRechargeLogToDB("游戏服务器充值成功:玩家在线", OrderID, pMsg->rechargeInfo.UserID, ChannelCode, ChannelOrderID, ChannelLabel, pMsg->rechargeInfo.ShopItemID, pMsg->rechargeInfo.Price, pMsg->rechargeInfo.FreePrice, 0, 0, Iter->second.IsAddGlobel() ? Iter->second.AddMoney : 0, Iter->second.IsAddCurrcey() ? Iter->second.AddMoney : 0, Iter->second.IsAddReward() ? Iter->second.RewardID : 0, SendLogDB);
		FreeVec(pVec);
		free(OrderID);
		free(ChannelCode);
		free(ChannelOrderID);
		free(ChannelLabel);

		LC_Cmd_Recharge msg;
		SetMsgInfo(msg, GetMsgType(Main_Recharge, LC_Recharge), sizeof(LC_Cmd_Recharge));
		msg.Result = true;
		msg.ID = pMsg->rechargeInfo.ShopItemID;
		pRole->SendDataToClient(&msg);
		return;
	}
}
bool FishServer::HandleFtpMsg(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	//FTP发送来的数据
	if (pCmd->CmdType == Main_Role)
	{
		switch (pCmd->SubCmdType)
		{
		case FG_SaveImageData:
			{
				//玩家保持头像的结果
				FG_Cmd_SaveImageData* pMsg = (FG_Cmd_SaveImageData*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->ID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				pRole->GetRoleFtpManager().OnUpLoadFaceDataResult(pMsg->Crc, pMsg->Result == 1);
				return true;
			}
			break;
		}
	}
	return true;
} 
void FishServer::UpdateByMin(DWORD dwTimer)//按分钟进行更新 不受中央服务器控制
{
	static DWORD DayChageTimeUpdate = 0;
	if (DayChageTimeUpdate != 0 && dwTimer - DayChageTimeUpdate < 60000)
		return;
	DayChageTimeUpdate = dwTimer;
	time_t NowTime = time(NULL);
	tm pNowTime;
	errno_t Error = localtime_s(&pNowTime, &NowTime);
	if (Error != 0)
	{
		ASSERT(false);
		return;
	}

	bool IsMinChange = true;
	bool IsHourChange = (pNowTime.tm_min == GetFishConfig().GetFishUpdateConfig().UpdateMin);
	bool IsDayChange = ((pNowTime.tm_hour == GetFishConfig().GetFishUpdateConfig().UpdateHour) && IsHourChange);
	bool IsMonthChange = ((pNowTime.tm_mday == 1) && IsDayChange);
	bool IsYearChange = ((pNowTime.tm_mon == 0) && IsMonthChange);
	bool IsNewDay = (pNowTime.tm_hour == 0 && pNowTime.tm_min == 0);
	//具体的关系函数
	//m_ShopManager.UpdateByMin(IsHourChange, IsDayChange, IsMonthChange, IsYearChange);//一分钟更新一次
	m_RoleManager.OnUpdateByMin(IsHourChange, IsDayChange, IsMonthChange, IsYearChange);
	m_RoleCache.UpdateByMin();
	if (IsNewDay)
	{
		//告诉客户端 新的一天了 可以进行处理些数据了
		NetCmd pCmd;
		SetMsgInfo(pCmd, GetMsgType(Main_Role, LC_NewDay), sizeof(NetCmd));
		SendNewCmdToAllClient(&pCmd);
	}
}
void FishServer::OnSaveInfoToDB(DWORD dwTimer)
{

	static DWORD SaveInfoDB = timeGetTime();
	if (dwTimer - SaveInfoDB >= static_cast<DWORD>(GetFishConfig().GetSystemConfig().RoleSaveMin * 60000))
	{
		SaveInfoDB = dwTimer;
		m_RoleManager.OnSaveInfoToDB();
		//m_RoleManager.OnGetRoleAchievementIndex();
	}
}
//1.Logon
bool FishServer::OnHandLogonLogonMsg(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	switch (pCmd->SubCmdType)
	{
	case LG_ChannelInfo:
		{
			LG_Cmd_ChannelInfo* pMsg = (LG_Cmd_ChannelInfo*)pCmd;
			if (!pMsg)
				return false;
			//GameServer获得玩家渠道的数据 我们保存起来 当玩家登陆成功的时候 存放到玩家RoleEx里面去
			OnAddChannelInfo(pMsg->dwUserID, &pMsg->channelUserInfo);
			return true;
		}
		break;
	case LG_AccountOnlyID:
		{
			LG_Cmd_AccountOnlyID* pMsg = (LG_Cmd_AccountOnlyID*)pCmd;
			if (!pMsg)
				return false;
			m_LogonManager.OnAddUserLogonInfo(pMsg->dwUserID, pMsg->dwOnlyID);

			GL_Cmd_AccountOnlyID msg;
			SetMsgInfo(msg,GetMsgType(Main_Logon, GL_AccountOnlyID), sizeof(GL_Cmd_AccountOnlyID));
			msg.dwOnlyID = pMsg->dwOnlyID;
			msg.dwUserID = pMsg->dwUserID;
			msg.ClientID = pMsg->ClientID;
			msg.GameServerID = pMsg->GameServerID;
			msg.LogonTypeID = pMsg->LogonTypeID;
			SendNetCmdToLogon(pMsg->LogonID, &msg);
			//SendNetCmdToLogon(&msg);
			return true;
		}
		break;
	}
	return true;
}
bool FishServer::OnHandClientLogonMsg(ServerClientData* pClient, NetCmd* pCmd)
{
	//处理客户端发送来的登陆数据
	if (!pClient || !pCmd)
		return false;
	switch (pCmd->SubCmdType)
	{
	case CL_AccountOnlyID:
		{
			CL_Cmd_AccountOnlyID* pMsg = (CL_Cmd_AccountOnlyID*)pCmd;
			if (!pMsg)
				return false;
			if (!m_LogonManager.CheckUserLogonInfo(pMsg->dwUserID, pMsg->dwOnlyID))
			{
				LC_Cmd_AccountOnlyIDFailed msg;
				SetMsgInfo(msg,GetMsgType(Main_Logon, LC_AccountOnlyIDFailed), sizeof(LC_Cmd_AccountOnlyIDFailed));
				SendNewCmdToClient(pClient, &msg);

				/*LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);*/

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				LogInfoToFile("LogonError.txt",TEXT("客户端唯一ID错误 连接GameServer失败 玩家ID:%d"),pMsg->dwUserID);

				ASSERT(false);
				return true;
			}
			//表示玩家正式登陆了 我们刷新下玩家的统计数据
			DBR_Cmd_SetRoleClientInfo msgDBSave;
			SetMsgInfo(msgDBSave, DBR_SetRoleClientInfo, sizeof(DBR_Cmd_SetRoleClientInfo));
			msgDBSave.dwUserID = pMsg->dwUserID;
			msgDBSave.PlateFormID = pMsg->PlateFormID;
			msgDBSave.ScreenPoint = pMsg->ScreenPoint;
			SendNetCmdToSaveDB(&msgDBSave);

			//记录玩家当前登陆的Mac地址
			UINT Count = 0;
			char* pMacAddress = WCharToChar(pMsg->MacAddress, Count);
			string MacAddress = pMacAddress;
			HashMap<DWORD, string>::iterator Iter = m_UserMacLog.find(pClient->OutsideExtraData);
			if (Iter != m_UserMacLog.end())
				m_UserMacLog.erase(Iter);
			m_UserMacLog.insert(HashMap<DWORD, string>::value_type(pClient->OutsideExtraData, MacAddress));//记录Mac地址
			free(pMacAddress);

			//玩家通过Logon进行登陆的 

			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (pRole)
			{
				if (pRole->IsRobot())
				{
					ASSERT(false);
					return false;
				}
				if (!pRole->IsAFK())
				{
					//玩家已经登陆了 顶掉 但是不移除玩家的数据 让玩家离线
					//m_TableManager.OnPlayerLeaveTable(pRole->GetUserID());//先离开桌子
					//玩家Socket进行切换
					//被顶掉了
					NetCmd msg;
					SetMsgInfo(msg,GetMsgType(Main_Logon, LC_ServerChangeSocket), sizeof(NetCmd));
					pRole->SendDataToClient(&msg);

					DelClient pDel;
					pDel.LogTime = timeGetTime();
					pDel.SocketID = pRole->GetGameSocketID();
					m_DelSocketVec.push_back(pDel);

					DWORD SocketID = pRole->GetGameSocketID();
					pRole->ChangeRoleSocketID(pClient->OutsideExtraData);

					//告诉客户端玩家登陆成功了
					pRole->OnUserLoadFinish(false);//玩家顶掉 不在桌子里面
				}
				else
				{
					//pRole->SetRoleExIsOnline(pClient->OutsideExtraData);
					m_TableManager.OnPlayerLeaveTable(pRole->GetUserID());//先离开桌子
					m_RoleCache.OnDleRoleCache(pRole->GetUserID());
					pRole->ChangeRoleSocketID(pClient->OutsideExtraData);//修改缓存玩家的Socket
					pRole->OnUserLoadFinish(false);//离线 进入 可能在桌子里面
				}
				if (pRole->IsExit())
				{
					//ShowInfoToWin("玩家进行离线保存 在回发确认命令之前 重新登陆了  回发确认命令 失效 ");
					pRole->SetIsExit(false);
				}
				return true;
			}
			//加入到排队队列里面去
			if (!m_RoleQueueManager.OnAddRoleToQueue(pMsg->dwUserID, pClient->OutsideExtraData, false))
			{
				LC_Cmd_AccountOnlyIDFailed msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountOnlyIDFailed), sizeof(LC_Cmd_AccountOnlyIDFailed));
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);
			}
			return true;
		}
		break;
	case CL_ResetLogonGameServer:
		{
			//客户端请求重新连接到GameServer
			CL_Cmd_ResetLogonGameServer* pMsg = (CL_Cmd_ResetLogonGameServer*)pCmd;
			if (!m_RoleLogonManager.CheckRoleOnlyID(pMsg->UserID, pMsg->RandID))
			{
				LC_Cmd_ResetLogonGameServer msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_ResetLogonGameServer), sizeof(LC_Cmd_ResetLogonGameServer));
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				return true;
			}
			if (!m_FishConfig.CheckVersionAndPathCrc(pMsg->VersionID, pMsg->PathCrc))//验证失败了
			{
				LC_Cmd_CheckVersionError msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_CheckVersionError), sizeof(LC_Cmd_CheckVersionError));
				msg.PathCrc = m_FishConfig.GetSystemConfig().PathCrc;
				msg.VersionID = m_FishConfig.GetSystemConfig().VersionID;
				SendNewCmdToClient(pClient, &msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);

				return true;
			}
			//判断玩家是否在线
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->UserID);
			if (pRole)
			{
				if (pRole->IsRobot())
				{
					ASSERT(false);
					return false;
				}
				if (!pRole->IsAFK())
				{
					//玩家已经登陆了 顶掉 但是不移除玩家的数据 让玩家离线
					//m_TableManager.OnPlayerLeaveTable(pRole->GetUserID());//先离开桌子
					//玩家Socket进行切换
					NetCmd msg;
					SetMsgInfo(msg, GetMsgType(Main_Logon, LC_ServerChangeSocket), sizeof(NetCmd));
					pRole->SendDataToClient(&msg);

					DelClient pDel;
					pDel.LogTime = timeGetTime();
					pDel.SocketID = pRole->GetGameSocketID();
					m_DelSocketVec.push_back(pDel);

					DWORD SocketID = pRole->GetGameSocketID();
					pRole->ChangeRoleSocketID(pClient->OutsideExtraData);
					//CloseClientSocket(SocketID);
					//告诉客户端玩家登陆成功了
					pRole->OnUserLoadFinish(true);
				}
				else
				{
					//pRole->SetRoleExIsOnline(pClient->OutsideExtraData);
					m_RoleCache.OnDleRoleCache(pRole->GetUserID());
					pRole->ChangeRoleSocketID(pClient->OutsideExtraData);//修改缓存玩家的Socket
					pRole->OnUserLoadFinish(true);
				}
				if (pRole->IsExit())
				{
					//ShowInfoToWin("玩家进行离线保存 在回发确认命令之前 重新登陆了  回发确认命令 失效 ");
					pRole->SetIsExit(false);
				}
				return true;
			}
			else
			{
				//加入到排队管理器里面去
				if (!m_RoleQueueManager.OnAddRoleToQueue(pMsg->UserID, pClient->OutsideExtraData, true))
				{
					LC_Cmd_ResetLogonGameServer msg; //登陆失败了
					SetMsgInfo(msg, GetMsgType(Main_Logon, LC_ResetLogonGameServer), sizeof(LC_Cmd_ResetLogonGameServer));
					SendNewCmdToClient(pClient, &msg);

					DelClient pDel;
					pDel.LogTime = timeGetTime();
					pDel.SocketID = pClient->OutsideExtraData;
					m_DelSocketVec.push_back(pDel);
					return true;
				}

				return true;
			}
		}
		break;
	case CL_ResetPassword:
		{
			CL_Cmd_ResetPassword* pMsg = (CL_Cmd_ResetPassword*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = GetRoleManager()->QuertUserBySocketID(pClient->OutsideExtraData);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			if (pRole->GetChannelID() != 0)
			{
				//渠道用户不可以修改
				ASSERT(false);
				return false;
			}
			//发送命令到数据库去
			DBR_Cmd_ChangeAccountPassword msg;
			SetMsgInfo(msg, DBR_ChangeAccountPassword, sizeof(DBR_Cmd_ChangeAccountPassword));
			msg.dwUserID = pRole->GetUserID();
			msg.OldPasswordCrc1 = pMsg->OldPasswordCrc1;
			msg.OldPasswordCrc2 = pMsg->OldPasswordCrc2;
			msg.OldPasswordCrc3 = pMsg->OldPasswordCrc3;
			msg.PasswordCrc1 = pMsg->NewPasswordCrc1;
			msg.PasswordCrc2 = pMsg->NewPasswordCrc2;
			msg.PasswordCrc3 = pMsg->NewPasswordCrc3;
			SendNetCmdToDB(&msg);
			return true;
		}
		break;
	case CL_AccountResetAccount:
		{
			CL_Cmd_AccountResetAccount* pMsg = (CL_Cmd_AccountResetAccount*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = GetRoleManager()->QuertUserBySocketID(pClient->OutsideExtraData);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			if (pRole->GetChannelID() != 0)
			{
				//渠道用户不可以修改
				ASSERT(false);
				return false;
			}
			if (!pRole->GetRoleInfo().IsCanResetAccount)
			{
				ASSERT(false);
				return false;
			}
			DBR_Cmd_ResetAccount msg;
			SetMsgInfo(msg, DBR_ResetAccount, sizeof(DBR_Cmd_ResetAccount));
			msg.dwUserID = pRole->GetUserID();
			msg.PasswordCrc1 = pMsg->PasswordCrc1;
			msg.PasswordCrc2 = pMsg->PasswordCrc2;
			msg.PasswordCrc3 = pMsg->PasswordCrc3;
			TCHARCopy(msg.AccountName, CountArray(msg.AccountName), pMsg->NewAccountName, _tcslen(pMsg->NewAccountName));
			SendNetCmdToDB(&msg);
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandDBLogonMsg(NetCmd* pCmd)
{
	if (!pCmd)
		return false;

	DBO_Cmd_GetAccountInfoByUserID* pMsg = (DBO_Cmd_GetAccountInfoByUserID*)pCmd;
	ServerClientData* pClient = GetUserClientDataByIndex(pMsg->ClientID);
	if (pMsg->ClientID != 0 && !pClient)//pMsg->ClientID == 0 为机器人
	{
		ASSERT(false);
		return false;
	}
	if (pMsg->ClientID == 0 && pMsg->IsRobot == false)
	{
		ASSERT(false);
		return false;//非机器人 被加载被加载为机器人了
	}
	DWORD SocketID = (pClient ? pClient->OutsideExtraData : 0);
	if (pMsg->Result)
	{
		//玩家登陆成功
		//玩家登陆成功了 我们进行处理
		//1.生成玩家对象
		CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->RoleInfo.dwUserID);
		if (pRole)
		{
			ASSERT(false);
			if (pRole->IsRobot())
			{
				ASSERT(false);
				return false;
			}
			if (!pRole->IsAFK())
			{
				NetCmd msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, LC_ServerChangeSocket), sizeof(NetCmd));
				pRole->SendDataToClient(&msg);

				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pRole->GetGameSocketID();
				m_DelSocketVec.push_back(pDel);

				//玩家Socket进行切换
				DWORD SocketID = pRole->GetGameSocketID();
				pRole->ChangeRoleSocketID(pClient->OutsideExtraData);

				//告诉客户端玩家登陆成功了
				pRole->OnUserLoadFinish(pMsg->LogonByGameServer);//玩家登陆 不在桌子里面
			}
			else
			{
				m_TableManager.OnPlayerLeaveTable(pRole->GetUserID());//先离开桌子
				m_RoleCache.OnDleRoleCache(pRole->GetUserID());
				pRole->ChangeRoleSocketID(pClient->OutsideExtraData);//修改缓存玩家的Socket
				pRole->OnUserLoadFinish(false);//离线 进入 可能在桌子里面
			}
			if (pRole->IsExit())
			{
				//ShowInfoToWin("玩家进行离线保存 在回发确认命令之前 重新登陆了  回发确认命令 失效 ");
				pRole->SetIsExit(false);
			}
		}
		else if (!m_RoleManager.CreateRole(&pMsg->RoleInfo, &pMsg->RoleServerInfo, SocketID, pMsg->LastOnlineTime, pMsg->LogonByGameServer, pMsg->IsRobot))
		{
			//创建玩家失败后 让玩家离开
			ASSERT(false);

			LC_Cmd_AccountOnlyIDFailed msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountOnlyIDFailed), sizeof(LC_Cmd_AccountOnlyIDFailed));
			SendNewCmdToClient(pClient, &msg);
			
			DelClient pDel;
			pDel.LogTime = timeGetTime();
			pDel.SocketID = pClient->OutsideExtraData;
			m_DelSocketVec.push_back(pDel);

			return false;
		}
		return true;
	}
	else
	{
		if (pMsg->IsFreeze)
		{
			//账号被冻结了 提示失败
			LC_Cmd_AccountIsFreeze msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountIsFreeze), sizeof(LC_Cmd_AccountIsFreeze));
			msg.EndTime = pMsg->FreezeEndTime;
			SendNewCmdToClient(pClient, &msg);
			
			if (pClient)
			{
				DelClient pDel;
				pDel.LogTime = timeGetTime();
				pDel.SocketID = pClient->OutsideExtraData;
				m_DelSocketVec.push_back(pDel);
			}

			return true;
		}
		//玩家登陆失败了
		LC_Cmd_AccountOnlyIDFailed msg;
		SetMsgInfo(msg,GetMsgType(Main_Logon, LC_AccountOnlyIDFailed), sizeof(LC_Cmd_AccountOnlyIDFailed));
		SendNewCmdToClient(pClient, &msg);

		DelClient pDel;
		pDel.LogTime = timeGetTime();
		pDel.SocketID = pClient->OutsideExtraData;
		m_DelSocketVec.push_back(pDel);

		return true;
	}
}
bool FishServer::OnHandleResetAccountName(NetCmd* pCmd)
{
	DBO_Cmd_ResetAccount* pMsg = (DBO_Cmd_ResetAccount*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	if (pMsg->Result)
	{
		pRole->GetRoleInfo().IsCanResetAccount = false;

		GL_Cmd_ResetAccount msgGL;
		SetMsgInfo(msgGL, GetMsgType(Main_Logon, GL_ResetAccount), sizeof(GL_Cmd_ResetAccount));
		msgGL.dwUserID = pRole->GetUserID();
		msgGL.PasswordCrc1 = pMsg->PasswordCrc1;
		msgGL.PasswordCrc2 = pMsg->PasswordCrc2;
		msgGL.PasswordCrc3 = pMsg->PasswordCrc3;
		TCHARCopy(msgGL.NewAccountName, CountArray(msgGL.NewAccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
		SendNetCmdToLogon(0, &msgGL);
		//m_GameServerManager.SendNetCmdToAllLogon(&msgGL);
	}
	//发送命令到客户端去
	LC_Cmd_AccountResetAccount msg;
	SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountResetAccount), sizeof(LC_Cmd_AccountResetAccount));
	msg.Result = pMsg->Result;
	pRole->SendDataToClient(&msg);
	return true;
}
bool FishServer::OnHandleResetPasswordResult(NetCmd* pCmd)
{
	//修改密码的结果
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	DBO_Cmd_ChangeAccountPassword* pMsg = (DBO_Cmd_ChangeAccountPassword*)pCmd;
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	LC_Cmd_ResetPassword msg;
	SetMsgInfo(msg, GetMsgType(Main_Logon, LC_ResetPassword), sizeof(LC_Cmd_ResetPassword));
	msg.Result = pMsg->Result;
	msg.NewPasswordCrc1 = pMsg->PasswordCrc1;
	msg.NewPasswordCrc2 = pMsg->PasswordCrc2;
	msg.NewPasswordCrc3 = pMsg->PasswordCrc3;
	pRole->SendDataToClient(&msg);

	//修改密码成功后
	if (pMsg->Result)
	{
		GL_Cmd_ResetPassword msgGL;
		SetMsgInfo(msgGL, GetMsgType(Main_Logon, GL_ResetPassword), sizeof(GL_Cmd_ResetPassword));
		msgGL.dwUserID = pRole->GetUserID();
		msgGL.PasswordCrc1 = pMsg->PasswordCrc1;
		msgGL.PasswordCrc2 = pMsg->PasswordCrc2;
		msgGL.PasswordCrc3 = pMsg->PasswordCrc3;
		SendNetCmdToLogon(0, &msgGL);
		// m_GameServerManager.SendNetCmdToAllLogon(&msgGL);
	}

	return true;
}
ServerClientData* FishServer::GetUserClientDataByIndex(DWORD IndexID)
{
	HashMap<DWORD, ServerClientData*>::iterator Iter = m_ClintList.find(IndexID);
	if (Iter == m_ClintList.end())
		return NULL;
	else
		return Iter->second;
}
//bool FishServer::OnHandleRoleOnline(NetCmd* pCmd)
//{
//	//接收到玩家上线的操作 表示玩家的RoleEx对象的全部数据已经构架完毕了 玩家可以正式登陆了
//	if (!pCmd)
//		return false;
//	DBO_Cmd_RoleOnline* pMsg = (DBO_Cmd_RoleOnline*)pCmd;
//	DWORD dwUserID = pMsg->dwUserID;
//	CRoleEx* pRole = m_RoleManager.QueryUser(dwUserID);
//	if (!pRole)
//	{
//		if (pMsg->IsOnline)
//		{
//			ASSERT(false);
//			return false;
//		}
//		else
//			return true;
//	}
//	if (pMsg->Result)
//	{
//		//设置成功
//		if (pMsg->IsOnline) 
//		{
//			//在缓存存在的情况下 玩家无须再向中央服务器发送玩家注册 
//			if (m_IsSendUserInfo)
//				pRole->SendUserInfoToCenter();
//			//玩家上线成功 我们通知玩家登陆成功 并且通知全部在线的玩家 告诉玩家上线了
//			//玩家通过Logon进行的登陆? 也可能是通过 GameServer直接登陆的 也可能是通过Logon登陆的s
//			pRole->OnUserLoadFinish(pRole->LogonByGameServer());//玩家正常登陆 判断是否有凭证
//			return true;
//		}
//	}
//	else
//	{
//		if (pMsg->IsOnline == false)
//		{
//			//玩家离线失败 
//			ASSERT(false);
//			//发送命令 玩家离开房间 发送命令到gate 断开玩家的Socket
//			CloseClientSocket(pRole->GetGameSocketID());
//			m_RoleManager.OnDelUser(dwUserID);//删除玩家对象	
//			return true;
//		}
//		else
//		{
//			//玩家上线失败 玩家登陆失败了 我们删除玩家
//			ASSERT(false);
//			CloseClientSocket(pRole->GetGameSocketID());
//			m_RoleManager.OnDelUser(dwUserID);//删除玩家对象
//			return true;
//		}
//	}
//	return true;
//}
//bool FishServer::OnHandleRoleAchievementIndex(NetCmd* pCmd)
//{
//	if (!pCmd)
//	{
//		ASSERT(false);
//		return false;
//	}
//	DBO_Cmd_GetRoleAchievementIndex* pMsg = (DBO_Cmd_GetRoleAchievementIndex*)pCmd;
//	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
//	if (pRole)
//	{
//		pRole->OnGetRoleAchievenmentIndexResult(pMsg);
//		return true;
//	}
//	else
//	{
//		ASSERT(false);
//		return false;
//	}
//}
//桌子
//---------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkTable(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd || !pClient)
		return false;
	if (pCmd->CmdType == Main_Table)
	{
		//当玩家发送登陆行为的时候
		switch (pCmd->SubCmdType)
		{
		case CL_Sub_JoinTable:
			OnHandleRoleJoinTable(pClient, pCmd);
			return true;
		case CL_Sub_LeaveTable:
			OnHandleRoleLeaveTable(pClient, pCmd);
			return true;
		}
		return true;
	}
	return true;
}
bool FishServer::OnHandleRoleJoinTable(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd || !pClient)
		return false;
	//玩家进入桌子
	CL_JoinTable* pMsg = (CL_JoinTable*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_TableManager.OnPlayerJoinTable(pMsg->bTableTypeID, pRole);//内部处理玩家进入桌子 会处理命令的 不带比赛参数
	return true;
}
bool FishServer::OnHandleRoleLeaveTable(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd || !pClient)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_TableManager.OnPlayerLeaveTable(pRole->GetUserID());
	return true;
}
//------------------------游戏------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkGame(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd || !pClient)
		return false;
	CRoleEx* pUser = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pUser)
	{
		//ASSERT(false);
		return true;
	}
	if (pCmd->CmdType == Main_Game || pCmd->SubCmdType == Main_Table)
	{
		//玩家上传头像 非桌子内部命令 无须放入内部 外部处理就OK
		switch (pCmd->GetCmdType())
		{
		case CMD_UPLOADING_REQUEST:
			{
				NetCmdUploadImgRequest *pImg = (NetCmdUploadImgRequest*)pCmd;
				pUser->GetRoleFtpManager().OnBeginUpLoadFaceData(pImg->Size);
				return true;
			}
			break;
		case CMD_UPLOADING_CHUNK:
			{
				NetCmdUploadImgChunk *pImg = (NetCmdUploadImgChunk*)pCmd;
				pUser->GetRoleFtpManager().OnUpLoadFaceData(pImg->StartIndex, pImg->Size, pImg->ImgData);
				return true;
			}
			break;
		default:
			return m_TableManager.OnHandleTableMsg(pUser->GetUserID(), pCmd);
		}
	}
	return true;
}
//---------------网关-----------------------------------------------------------------------------
bool FishServer::OnGateJoinGameRoom(DWORD dwSocketID)
{
	return true;
}
bool FishServer::OnGateLeaveGameRoom(DWORD dwSocketID)
{
	//当一个网关离开Room的时候 我们想要让当前网关上的全部玩家离线 就是登出  但是不需要发送到客户端去
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(dwSocketID);
	if (!pRole)
	{
		//ASSERT(false);
		return false;
	}
	//m_UserMacLog.erase(dwSocketID);//移除
	//g_DBLogManager.LogRoleOnlineInfo(pRole->GetUserID(), false, SendLogDB);//设置玩家离线
	if (m_RoleCache.IsOpenRoleCache())
	{
		m_RoleCache.OnAddRoleCache(pRole->GetUserID());//玩家加入到离线缓存里面去
		pRole->SetRoleExLeaveServer();//设置玩家离线了 离线缓存 我们保存一定的时间 当时间到后玩家没有正式上线 就直接剔除 其他的时候 与在线的时候 完全一样进行处理
		//g_FishServer.ShowInfoToWin("玩家离线 玩家对象进行缓存保存 玩家名称 %s  玩家ID %d", pRole->GetRoleInfo().NickName, pRole->GetUserID());
	}
	else
	{
		//g_FishServer.ShowInfoToWin("玩家离线 玩家名称 %s  玩家ID %d", pRole->GetRoleInfo().NickName, pRole->GetUserID());
		m_RoleManager.OnDelUser(pRole->GetUserID(),true,true);
	}
	return true;
}
//---------------物品-----------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkItem(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd || !pClient)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return true;
	}
	//玩家物品 加载
	switch (pCmd->SubCmdType)
	{
	case CL_GetUserItem:
		pRole->GetItemManager().OnGetUserItem();
		return true;
	case CL_OnUseItem:
		{
			CL_Cmd_OnUseItem* pMsg = (CL_Cmd_OnUseItem*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			LC_Cmd_OnUseItem msg;
			SetMsgInfo(msg, GetMsgType(Main_Item, LC_OnUseItem), sizeof(LC_Cmd_OnUseItem));
			msg.ItemOnlyID = pMsg->ItemOnlyID;
			msg.ItemID = pMsg->ItemID;
			msg.ItemSum = pMsg->ItemSum;
			msg.Result = pRole->GetItemManager().OnTryUseItem(pMsg->ItemOnlyID, pMsg->ItemID, pMsg->ItemSum);
			pRole->SendDataToClient(&msg);
			return true;
		}
	case CL_OnAcceptItem:
		{
			CL_Cmd_OnAcceptItem* pMsg = (CL_Cmd_OnAcceptItem*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			//将命令进行转化处理 
			//GameID 转化为UserID
			if (pMsg->GameIDorUserID)
			{
				WORD PageSize = sizeof(DBR_Cmd_GameIDConvertToUserID) + pMsg->GetCmdSize() - sizeof(BYTE);
				DBR_Cmd_GameIDConvertToUserID* msgDB = (DBR_Cmd_GameIDConvertToUserID*)malloc(PageSize);
				if (!msgDB)
				{
					ASSERT(false);
					return false;
				}
				msgDB->SetCmdSize(PageSize);
				msgDB->SetCmdType(DBR_GameIDConvertToUserID);
				msgDB->dwGameID = pMsg->dwID;
				msgDB->SrcUserID = pRole->GetUserID();
				memcpy_s((void*)&msgDB->NetCmd, pMsg->GetCmdSize(), pMsg, pMsg->GetCmdSize());
				g_FishServer.SendNetCmdToDB(msgDB);
				free(msgDB);
			}
			else
			{
				LC_Cmd_OnAcceptItem msg;
				SetMsgInfo(msg, GetMsgType(Main_Item, LC_OnAcceptItem), sizeof(LC_Cmd_OnAcceptItem));
				msg.ItemOnlyID = pMsg->ItemOnlyID;
				msg.ItemID = pMsg->ItemID;
				msg.ItemSum = pMsg->ItemSum;
				msg.dwDestUserID = pMsg->dwID;
				msg.Result = pRole->GetItemManager().OnTryAcceptItemToFriend(msg.dwDestUserID, pMsg->ItemOnlyID, pMsg->ItemID, pMsg->ItemSum);//试图赠送物品给其他玩家
				pRole->SendDataToClient(&msg);
			}
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleDateBaseLoadItem(NetCmd* pCmd)
{
	DBO_Cmd_LoadUserItem * pMsg = (DBO_Cmd_LoadUserItem*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return true;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetItemManager().OnLoadUserItemResult(pMsg);
	return true;
}
//bool FishServer::OnHandleDataBaseLoadItemFinish(NetCmd* pCmd)
//{
//	DBO_Cmd_LoadUserItemFinish * pMsg = (DBO_Cmd_LoadUserItemFinish*)pCmd;
//	if (!pMsg)
//	{
//		ASSERT(false);
//		return true;
//	}
//	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
//	if (!pRole)
//	{
//		ASSERT(false);
//		return true;
//	}
//	pRole->GetItemManager().OnLoadUserItemFinish();
//	return true;
//}
bool FishServer::OnHandleDataBaseAddItemResult(NetCmd* pCmd)
{
	DBO_Cmd_AddUserItem * pMsg = (DBO_Cmd_AddUserItem*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return true;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return true;
	}
	pRole->GetItemManager().OnAddUserItemResult(pMsg);
	return true;
}
//---------------关系-----------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkRelation(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd || !pClient)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return true;
	}
	switch (pCmd->SubCmdType)
	{
		case CL_GetUserRelation:
			pRole->GetRelation().OnGetUserRelation();
			return true;
		case CL_AddUserRelation:
		{
			CL_Cmd_AddUserRelation* pMsg = (CL_Cmd_AddUserRelation*)pCmd;
			pRole->GetRelation().OnAddUserRelation(pMsg);
			return true;
		}
		case CL_DelUserRelation:
		{
			CL_Cmd_DelUserRelation* pMsg = (CL_Cmd_DelUserRelation*)pCmd;
			pRole->GetRelation().OnDelUserRelation(pMsg);
			return true;
		}
		/*case CL_ChangeUserRelation:
		{
			CL_Cmd_ChangeUserRelation* pMsg = (CL_Cmd_ChangeUserRelation*)pCmd;
			pRole->GetRelation().OnChangeUserRelation(pMsg);
			return true;
		}*/
	}
	return true;
}
bool FishServer::OnHandleSocketRelation(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	//处理关系数据 接收从中央发送来的关系数据
	switch (pCmd->SubCmdType)
	{
		case CC_AddBeUserRelation:
		{
			CC_Cmd_AddBeUserRelation* pMsg = (CC_Cmd_AddBeUserRelation*)pCmd;
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnAddBeUserRelation(&pMsg->RelationInfo);
			return true;
		}
		case CC_DelBeUserRelation:
		{
			CC_Cmd_DelBeUserRelation* pMsg = (CC_Cmd_DelBeUserRelation*)pCmd;
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnDelBeUserRelation(pMsg->dwDestUserID);
			return true;
		}
		case CC_ChangeBeUserRelation:
		{
			CC_Cmd_ChangeBeUserRelation* pMsg = (CC_Cmd_ChangeBeUserRelation*)pCmd;
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChagneBeUserRelation(pMsg->dwDestUserID, pMsg->RelationType);
			return true;
		}
		case CC_RoleChangeOnline:
		{
			CC_Cmd_ChangeUserOline* pMsg = (CC_Cmd_ChangeUserOline*)pCmd;
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwSrcUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChangeUserOnline(pMsg->dwDestUserID, pMsg->IsOnline);
			return true;
		}
		case CC_ChangeRelationLevel:
		{
			CC_Cmd_ChangeRelationLevel* pMsg = (CC_Cmd_ChangeRelationLevel*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChangeRelationLevel(pMsg->dwDestUserID, pMsg->wLevel);
			return true;
		}
		case CC_ChangeRelationFaceID:
		{
			CC_Cmd_ChangeRelationFaceID* pMsg = (CC_Cmd_ChangeRelationFaceID*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChangeRelationFaceID(pMsg->dwDestUserID, pMsg->dwFaceID);
			return true;
		}
		case CC_ChangeRelationGender:
		{
			CC_Cmd_ChangeRelationGender* pMsg = (CC_Cmd_ChangeRelationGender*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChangeRelationGender(pMsg->dwDestUserID, pMsg->bGender);
			return true;
		}
		case CC_ChangeRelationNickName:
		{
			CC_Cmd_ChangeRelationNickName* pMsg = (CC_Cmd_ChangeRelationNickName*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChangeRelationNickName(pMsg->dwDestUserID, pMsg->cNickName);
			return true;
		}
		case CC_ChangeRelationTitle:
		{
			CC_Cmd_ChangeRelationTitle* pMsg = (CC_Cmd_ChangeRelationTitle*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChangeRelationTitle(pMsg->dwDestUserID, pMsg->TitleID);
			return true;
		}
		case CC_ChangeRelationAchievementPoint:
		{
			CC_Cmd_ChangeRelationAchievementPoint* pMsg = (CC_Cmd_ChangeRelationAchievementPoint*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChangeRelationAchievementPoint(pMsg->dwDestUserID, pMsg->dwAchievementPoint);
			return true;
		}
		case CC_ChangeRelationCharmValue:
		{
			CC_Cmd_ChangeRelationCharmValue* pMsg = (CC_Cmd_ChangeRelationCharmValue*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChangeRelationCharmValue(pMsg->dwDestUserID, pMsg->CharmInfo);
			return true;
		}
		case CC_ChangeRelationIP:
		{
			CC_Cmd_ChangeRelationIP* pMsg = (CC_Cmd_ChangeRelationIP*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				//ASSERT(false);
				return true;
			}
			pRole->GetRelation().OnChangeRelationCLientIP(pMsg->dwDestUserID, pMsg->ClientIP);
			return true;
		}
		case CC_ChangeRelationIsShowIpAddress:
			{
				CC_Cmd_ChangeRelationIsShowIpAddress* pMsg = (CC_Cmd_ChangeRelationIsShowIpAddress*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return true;
				}
				CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
				// ASSERT(false);
					return true;
				}
				//修改关系里的数据
				pRole->GetRelation().OnChangeRelationIsShowIpAddress(pMsg->dwDestUserID, pMsg->IsShowIpAddress);
				return true;
			}
		case CC_ChangeRelationVipLevel:
			{
				CC_Cmd_ChangeRelationVipLevel* pMsg = (CC_Cmd_ChangeRelationVipLevel*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return true;
				}
				CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					// ASSERT(false);
					return true;
				}
				pRole->GetRelation().OnChangeRelationVipLevel(pMsg->dwDestUserID, pMsg->VipLevel);
				return true;
			}
		case CC_ChangeRelationIsInMonthCard:
			{
				CC_Cmd_ChangeRelationIsInMonthCard* pMsg = (CC_Cmd_ChangeRelationIsInMonthCard*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return true;
				}
				CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					// ASSERT(false);
					return true;
				}
				pRole->GetRelation().OnChangeRelationIsInMonthCard(pMsg->dwDestUserID, pMsg->IsInMonthCard);
				return true;
			}
	}
	return true;
}
bool FishServer::OnHandleDateBaseLoadRelation(NetCmd* pCmd)
{
	DBO_Cmd_LoadUserRelation* pMsg = (DBO_Cmd_LoadUserRelation*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRelation().OnLoadUserRelationResult(pMsg);
	return true;
}
bool FishServer::OnHandleDataBaseLoadBeRelation(NetCmd* pCmd)
{
	DBO_Cmd_LoadBeUserRelation* pMsg = (DBO_Cmd_LoadBeUserRelation*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRelation().OnLoadBeUserRelationResult(pMsg);
	return true;
}
//bool FishServer::OnHandleDataBaseLoadBeRelationFinish(NetCmd* pCmd)
//{
//	DBO_Cmd_LoadBeUserRelationFinish* pMsg = (DBO_Cmd_LoadBeUserRelationFinish*)pCmd;
//	if (!pMsg)
//	{
//		ASSERT(false);
//		return false;
//	}
//	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
//	if (!pRole)
//	{
//		ASSERT(false);
//		return false;
//	}
//	pRole->GetRelation().OnLoadBeUserRelationFinish();
//	return true;
//}
bool FishServer::OnHandleDataBaseAddRelation(NetCmd* pCmd)
{
	DBO_Cmd_AddUserRelation* pMsg = (DBO_Cmd_AddUserRelation*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRelation().OnAddUserRelationResult(pMsg);
	return true;
}
//邮件------------------------------------------------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkMail(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	//接收客户端发送来的邮件方面的数据
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return true;
	}
	switch (pCmd->SubCmdType)
	{
		case CL_GetUserMail:
		{
			pRole->GetMailManager().OnGetAllUserMail();
			return true;
		}
		case CL_SendUserMail:
		{
			CL_Cmd_SendUserMail* pMsg = (CL_Cmd_SendUserMail*)pCmd;
			return pRole->GetMailManager().OnSendUserMail(pMsg);
		}
		case CL_DelUserMail:
		{
			CL_Cmd_DelMail* pMsg = (CL_Cmd_DelMail*)pCmd;
			pRole->GetMailManager().OnDelUserMail(pMsg->dwMailID);
			return true;
		}
		case CL_GetMailItem:
		{
			CL_Cmd_GetMailItem* pMsg = (CL_Cmd_GetMailItem*)pCmd;
			pRole->GetMailManager().OnGetUserMailItem(pMsg->dwMailID);
			return true;
		}
		case CL_ReadMail:
		{
			CL_Cmd_GetMailContext* pMsg = (CL_Cmd_GetMailContext*)pCmd;
			pRole->GetMailManager().OnGetUserMailContext(pMsg->dwMailID);
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleDataBaseLoadUserMail(NetCmd* pCmd)
{
	//加载玩家的全部的邮件
	DBO_Cmd_LoadUserMail* pMsg = (DBO_Cmd_LoadUserMail*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetMailManager().OnLoadUserMailByPageResult(pMsg);
	return true;
}
//bool FishServer::OnHandleDataBaseLoadUserMailFinish(NetCmd* pCmd)
//{
//	DBO_Cmd_LoadUserMailFinish* pMsg = (DBO_Cmd_LoadUserMailFinish*)pCmd;
//	if (!pMsg)
//	{
//		ASSERT(false);
//		return false;
//	}
//	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
//	if (!pRole)
//	{
//		ASSERT(false);
//		return false;
//	}
//	pRole->GetMailManager().OnLoadUserMailFinish();
//	return true;
//}
bool FishServer::OnHandleDataBaseSendUserMail(NetCmd* pCmd)
{
	//玩家添加一封邮件 
	DBO_Cmd_AddUserMail* pMsg = (DBO_Cmd_AddUserMail*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pDestRole = m_RoleManager.QueryUser(pMsg->dwDestUserID);
	if (!pDestRole)
	{
		ASSERT(false);
		return false;
	}
	return pDestRole->GetMailManager().OnBeAddUserMailResult(pMsg);
}
bool FishServer::OnHandleSocketMail(NetCmd* pCmd)
{
	//接收中央服务器发送来的邮件消息
	if (pCmd->SubCmdType == CG_SendUserMail)
	{
		CG_Cmd_SendUserMail* pMsg = (CG_Cmd_SendUserMail*)pCmd;
		if (!pMsg)
		{
			ASSERT(false);
			return false;
		}
		CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->DestUserID);
		if (!pRole)
		{
			ASSERT(false);
			return false;
		}
		pRole->GetMailManager().OnBeAddUserMail(&pMsg->MailInfo);
		return true;
	}
	else if (pCmd->SubCmdType == CG_SendUserMailResult)
	{
		CG_Cmd_SendUserMailResult* pMsg = (CG_Cmd_SendUserMailResult*)pCmd;
		if (!pMsg)
		{
			ASSERT(false);
			return false;
		}
		CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->SrcUserID);
		if (!pRole)
		{
			ASSERT(false);
			return false;
		}
		LC_Cmd_SendUserMailResult msg;
		SetMsgInfo(msg, GetMsgType(Main_Mail, LC_SendUserMailResult), sizeof(LC_Cmd_SendUserMailResult));
		msg.Result = pMsg->Result;
		msg.DestUserID = pMsg->DestUserID;
		pRole->SendDataToClient(&msg);
		return true;
	}
	else if (pCmd->SubCmdType == CC_SendBeSystemMail)
	{
		//接收到中央服务器发送来的 系统邮件了 我们可以开始处理了
		CC_Cmd_SendBeSystemMail* pMsg = (CC_Cmd_SendBeSystemMail*)pCmd;
		CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwDestUserID);
		if (!pRole)
		{
			ASSERT(false);
			return true;
		}
		pRole->GetMailManager().OnBeAddUserMail(&pMsg->MailInfo);//将系统邮件加入到集合里面去
		return true;
	}
	return true;
}
//Role--------------------------------------------------------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkRole(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	//接收客户端发送来的 玩家数据 修改
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return true;
	}
	switch (pCmd->SubCmdType)
	{
		case CL_UpdateAccount:
			{
				CL_Cmd_UpdateAccount* msg = (CL_Cmd_UpdateAccount*)pCmd;
				if (!msg)
				{
					ASSERT(false);
					return true;
				}
				//玩家更新账号的处理
				WORD wMsgSize = static_cast<WORD>(sizeof(LO_Cmd_UpdateAccount) + (msg->Info.Sum - 1) * sizeof(BYTE));
				LO_Cmd_UpdateAccount* pMsg = (LO_Cmd_UpdateAccount*)malloc(wMsgSize);
				if (!pMsg)
				{
					ASSERT(false);
					return true;
				}
				pMsg->SetCmdSize(wMsgSize);
				pMsg->SetCmdType(GetMsgType(Main_Role, LO_UpdateAccount));
				pMsg->dwUserID = pRole->GetUserID();
				pMsg->bType = msg->bType;
				memcpy_s(&pMsg->Info, sizeof(AccountSDKInfo) + sizeof(BYTE) * (msg->Info.Sum - 1), &msg->Info, sizeof(AccountSDKInfo) + sizeof(BYTE) * (msg->Info.Sum - 1));
				SendNetCmdToOperate(pMsg);
				free(pMsg);
				return true;
			}
		case CL_ChangeRoleGender:
			{
				CL_Cmd_ChangeRoleGender* msg = (CL_Cmd_ChangeRoleGender*)pCmd;
				if (!msg)
				{
					ASSERT(false);
					return true;
				}
				if (msg->bGender == pRole->GetRoleInfo().bGender)
				{
					return true;
				}
				DWORD ItemID = GetFishConfig().GetSystemConfig().ChangeGenderNeedItemID;
				DWORD ItemSum = GetFishConfig().GetSystemConfig().ChangeGenderNeedItemSum;
				if (ItemID != 0 && ItemSum >0 && !pRole->GetItemManager().OnDelUserItem(ItemID, ItemSum))
				{
					ASSERT(false);
					return true;
				}
				pRole->ChangeRoleGender(msg->bGender);
				return true;
			}
		case CL_ChangeRoleNickName:
			{
				CL_Cmd_ChangeRoleNickName* pMsg = (CL_Cmd_ChangeRoleNickName*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return true;
				}
				if (!GetFishConfig().CheckStringIsError(pMsg->NickName, MIN_NICKNAME, MAX_NICKNAME, SCT_Normal))
				{
					ASSERT(false);
					return true;
				}
				if (_tcscmp(pMsg->NickName, pRole->GetRoleInfo().NickName) == 0)
				{
					ASSERT(false);
					return true;
				}
				if (_tcsnccmp(pRole->GetRoleInfo().NickName, TEXT("游客"), 2) != 0 && _tcsnccmp(pRole->GetRoleInfo().NickName, TEXT("用户"), 2) != 0)//前2个字符比较
				{
					DWORD ItemID = GetFishConfig().GetSystemConfig().ChangeNickNameNeedItemID;
					DWORD ItemSum = GetFishConfig().GetSystemConfig().ChangeNickNameNeedItemSum;
					if (ItemID != 0 && ItemSum >0 && !pRole->GetItemManager().OnDelUserItem(ItemID, ItemSum))
					{
						ASSERT(false);
						return true;
					}
				}
				pRole->ChangeRoleNickName(pMsg->NickName);
				return true;
			}
		case CL_ChangeRoleNormalFaceID:
			{
				CL_Cmd_ChangeRoleNormalFaceID* pMsg = (CL_Cmd_ChangeRoleNormalFaceID*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return true;
				}
				pRole->ChangeRoleFaceID(pMsg->dwFaceID);
				return true;
			}
		case CL_ResetRoleInfo:
			{
				pRole->ResetRoleInfoToClient();
				return true;
			}
		case CL_ChangeRoleIsShowIpAddress:
			{
				CL_Cmd_ChangeRoleIsShowIpAddress* pMsg = (CL_Cmd_ChangeRoleIsShowIpAddress*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return true;
				}
				pRole->ChangeRoleIsShowIpAddress(pMsg->IsShowIpAddress);
				return true;
			}
		case CL_ChangeRoleRateValue:
			{
				CL_Cmd_ChangeRoleRateValue* pMsg = (CL_Cmd_ChangeRoleRateValue*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return true;
				}
				BYTE RateIndex = pMsg->RateIndex;//玩家需要解锁的倍率   //pRole->GetRoleRate().GetCanShowMaxRate();
				if (RateIndex == pRole->GetRoleRate().GetCanUseMaxRate())//倍率没加
				{
					return true;//无须继续开启了
				}
				if (pRole->GetRoleRate().IsCanUseRateIndex(RateIndex))
				{
					//数据已经乱了 我们进行处理
					pRole->GetRoleRate().ResetRateInfo();
					RateIndex = pRole->GetRoleRate().GetCanShowMaxRate();
					if (RateIndex == pRole->GetRoleRate().GetCanUseMaxRate())//倍率没加
					{
						//因为已经乱了 我们重新刷新玩家的RateValue
						LC_Cmd_ChangeRoleRateValue msg;
						SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleRateValue), sizeof(LC_Cmd_ChangeRoleRateValue));
						msg.RateValue = pRole->GetRoleInfo().RateValue;
						msg.OpenRateIndex = pRole->GetRoleRate().GetCanUseMaxRate();
						pRole->SendDataToClient(&msg);
						return true;//无须继续开启了
					}
				}
				if (!pRole->GetRoleRate().IsCanUseRateIndex(RateIndex))
				{
					//玩家开启倍率 我们需要进行处理
					WORD UseCurreyNum = 0;
					if (RateIndex > (pRole->GetRoleRate().GetCanUseMaxRate() + 1))
					{
						//连续开启多级倍率
						for (BYTE i = pRole->GetRoleRate().GetCanUseMaxRate() + 1; i <= RateIndex; ++i)
						{
							UseCurreyNum +=GetTableManager()->GetGameConfig()->RateUnlock(i);
						}
					}
					else
					{
						//开启指定倍率
						UseCurreyNum =  GetTableManager()->GetGameConfig()->RateUnlock(RateIndex);
					}
					if (UseCurreyNum != 0)
					{
						if (pRole->ChangeRoleCurrency(UseCurreyNum*-1, TEXT("开启新的倍率")))
						{
							if (!pRole->GetRoleRate().OnChangeRoleRate(RateIndex))
							{
								pRole->ChangeRoleCurrency(UseCurreyNum, TEXT("开启新的倍率失败退还钻石"));
							}
						}
					}
					else
					{
						pRole->GetRoleRate().OnChangeRoleRate(RateIndex);//免费的话 直接开启倍率
					}
					return true;
				}
				else
				{
					LC_Cmd_ChangeRoleRateValue msg;
					SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleRateValue), sizeof(LC_Cmd_ChangeRoleRateValue));
					msg.RateValue = pRole->GetRoleInfo().RateValue;
					msg.OpenRateIndex = pRole->GetRoleRate().GetCanUseMaxRate();
					pRole->SendDataToClient(&msg);
					ASSERT(false);
					return true;
				}
			}
		case CL_GetMonthCardReward:
			{
				//获取月卡礼包的奖励 每天的
				CL_Cmd_GetMonthCardReward* pMsg = (CL_Cmd_GetMonthCardReward*)pCmd;
				LC_Cmd_GetMonthCardReward msg;
				SetMsgInfo(msg, GetMsgType(Main_Role, LC_GetMonthCardReward), sizeof(LC_Cmd_GetMonthCardReward));
				msg.Result = pRole->GetRoleMonthCard().GetRoleMonthCardReward();
				pRole->SendDataToClient(&msg);
				return true;
			}
		case CL_RoleProtect:
			{
				pRole->GetaRoleProtect().Request();
				return true;
			}
		case CL_ChangeSecondPassword:
			{
				CL_Cmd_ChangeSecondPassword* pMsg = (CL_Cmd_ChangeSecondPassword*)pCmd;
				//转发数据库请求
				DBR_Cmd_ChangeRoleSecPassword msg;
				SetMsgInfo(msg, DBR_ChangeRoleSecPassword, sizeof(DBR_Cmd_ChangeRoleSecPassword));
				msg.dwUserID = pRole->GetUserID();
				msg.dwOldSecPasswordCrc1 = pMsg->dwOldCrc1;
				msg.dwOldSecPasswordCrc2 = pMsg->dwOldCrc2;
				msg.dwOldSecPasswordCrc3 = pMsg->dwOldCrc3;

				msg.dwNewSecPasswordCrc1 = pMsg->dwNewCrc1;
				msg.dwNewSecPasswordCrc2 = pMsg->dwNewCrc2;
				msg.dwNewSecPasswordCrc3 = pMsg->dwNewCrc3;

				SendNetCmdToDB(&msg);
				return true;
			}
			break;
		case CL_ChangeRoleShareStates:
			{
				CL_Cmd_ChangeRoleShareStates* pMsg = (CL_Cmd_ChangeRoleShareStates*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				pRole->ChangeRoleShareStates(pMsg->ShareStates);
				return true;
			}
			break;
			//处理玩家的头像功能
			/*case CL_RequestUserFaceID:
			{
			CL_Cmd_RequestUserFaceID* pMsg = (CL_Cmd_RequestUserFaceID*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			pRole->GetFaceManager().OnRequestUpLoadFacePic(pMsg->Size);
			return true;
		}
		case CL_UpLoadingchunk:
		{
			CL_Cmd_UpLoadingchunk* pMsg = (CL_Cmd_UpLoadingchunk*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return true;
			}
			pRole->GetFaceManager().OnSaveUpLoadPicChunk(pMsg->StartIndex, pMsg->Size, pMsg->ImgData);
			return true;
		}*/
	}
	return true;
}
bool FishServer::OnHandleSaveRoleAllInfo(NetCmd* pCmd)
{
	DBO_Cmd_SaveRoleAllInfo* pMsg = (DBO_Cmd_SaveRoleAllInfo*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	//接收到保存
	GetRoleManager()->OnDelUserResult(pMsg);
	return true;
}
bool FishServer::OnHandleChangeRoleNickName(NetCmd* pCmd)
{
	DBO_Cmd_SaveRoleNickName* pMsg = (DBO_Cmd_SaveRoleNickName*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->ChangeRoleNickNameResult(pMsg);
	return true;
}
bool FishServer::OnHandleChangeRoleSecPassword(NetCmd* pCmd)
{
	DBO_Cmd_ChangeRoleSecPassword* pMsg = (DBO_Cmd_ChangeRoleSecPassword*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	if (pMsg->Result)
		pRole->OnChangeRoleSecPassword(pMsg->dwNewSecPasswordCrc1, pMsg->dwNewSecPasswordCrc2, pMsg->dwNewSecPasswordCrc3,false);//无须保存到数据库去

	LC_Cmd_ChangeSecondPassword msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeSecondPassword), sizeof(LC_Cmd_ChangeSecondPassword));
	msg.Result = pMsg->Result;
	pRole->SendDataToClient(&msg);
	return true;
}

bool FishServer::OnHandleCreateDeal(NetCmd* pCmd)
{
	DBO_Cmd_Deal_Create* pMsg = (DBO_Cmd_Deal_Create*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->user_id);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	UINT count;
	GC_Cmd_CreateOrder msg;
	SetMsgInfo(msg, GetMsgType(Main_Recharge, GC_CreateOrder), sizeof(msg));
	msg.OrderID = pMsg->order_id;
	msg.ItemID = pMsg->shop_id;

	const tagChannelConfig* channel_config = g_FishServer.GetFishConfig().GetChannelConfig(pRole->GetOperatorChannelID());
	if (channel_config == null)
	{
		LogInfoToFile("DomePay.txt", "玩家【%d】支付生成订单失败,没有渠【%d】道信息",pMsg->user_id, pRole->GetOperatorChannelID());
	}
	else
	{
		char* notify_data = WCharToChar(channel_config->notify_pay_url, count);
		std::string notifyUrl = notify_data;
		//LogInfoToFile("DomePay.txt", "签名结果[%s]", data.c_str());
		TCHARCopy(msg.ProductID, CountArray(msg.ProductID), pMsg->good_id, _tcslen(pMsg->good_id));
		WCHAR* url = CharToWChar(notifyUrl.c_str(), count);
		TCHARCopy(msg.notify_url, CountArray(msg.notify_url), url, _tcslen(url));
		free(url);
		pRole->SendDataToClient(&msg);
		DBR_Cmd_Deal_Create_Log msg_log;
		msg_log.ItemID = msg.ItemID;
		msg_log.OrderID = msg.OrderID;
		msg_log.UserID = pMsg->user_id;
		msg_log.ChannelID = pRole->GetOperatorChannelID();
		TCHARCopy(msg_log.ProductID, CountArray(msg_log.ProductID), pMsg->good_id, _tcslen(pMsg->good_id));
		SetMsgInfo(msg_log, DBR_Deal_Create_Log, sizeof(msg_log));
		SendLogDB(&msg_log);		
	}
	return true;
}
bool FishServer::OnHandleGameIDConvertUserID(NetCmd* pCmd)
{
	DBO_Cmd_GameIDConvertToUserID* pMsg = (DBO_Cmd_GameIDConvertToUserID*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	NetCmd* msg = (NetCmd*)&pMsg->NetCmd;
	if (!msg)
	{
		ASSERT(false);
		return false;
	}
	if (msg->CmdType == Main_Item)
	{
		switch (msg->SubCmdType)
		{
		case CL_OnAcceptItem:
			{
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->SrcUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}

				CL_Cmd_OnAcceptItem* pClMsg = (CL_Cmd_OnAcceptItem*)msg;
				pClMsg->dwID = pMsg->dwUserID;
				pClMsg->GameIDorUserID = false;

				LC_Cmd_OnAcceptItem msg;
				SetMsgInfo(msg, GetMsgType(Main_Item, LC_OnAcceptItem), sizeof(LC_Cmd_OnAcceptItem));
				msg.ItemOnlyID = pClMsg->ItemOnlyID;
				msg.ItemID = pClMsg->ItemID;
				msg.ItemSum = pClMsg->ItemSum;
				msg.dwDestUserID = pClMsg->dwID;
				if (pMsg->dwUserID == 0)
					msg.Result = false;
				else
					msg.Result = pRole->GetItemManager().OnTryAcceptItemToFriend(msg.dwDestUserID, pClMsg->ItemOnlyID, pClMsg->ItemID, pClMsg->ItemSum);//试图赠送物品给其他玩家
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		}
	}
	return false;
}
//Query----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkQuery(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	//接收到客户端的查询信息
	switch (pCmd->SubCmdType)
	{
		case CL_QueryUserByNickName:
		{
			//进行查询方面的操作
			CL_Cmd_QueryUserByNickName* pMsg = (CL_Cmd_QueryUserByNickName*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			if (!m_FishConfig.CheckStringIsError(pMsg->NickName, 0, MAX_NICKNAME, SCT_Normal))
			{
				ASSERT(false);
				return false;
			}
			//找到查询条件后 我们将命令转为DBR数据 发送到数据库去
			DBR_Cmd_QueryRoleInfoByNickName msg;
			SetMsgInfo(msg,DBR_Query_RoleInfoByNickName, sizeof(DBR_Cmd_QueryRoleInfoByNickName));
			msg.dwUserID = pRole->GetUserID();
			msg.Page = pMsg->Page;
			msg.IsOnline = pMsg->IsOnline;
			TCHARCopy(msg.QueryNickName, CountArray(msg.QueryNickName), pMsg->NickName, _tcslen(pMsg->NickName));
			SendNetCmdToDB(&msg);
			return true;
		}
		case CL_QueryUserByGameID:
			{
				//通过游戏ID 查询玩家数据
				CL_Cmd_QueryUserByGameID* pMsg = (CL_Cmd_QueryUserByGameID*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				if (pMsg->GameID == pRole->GetRoleInfo().GameID)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pDestRole = m_RoleManager.QueryUserByGameID(pMsg->GameID);
				if (!pDestRole)
				{
					//发送数据库请求
					DBR_Cmd_Query_RoleInfoByGameID msg;
					SetMsgInfo(msg, DBR_Query_RoleInfoByGameID, sizeof(DBR_Cmd_Query_RoleInfoByGameID));
					msg.dwUserID = pRole->GetUserID();
					msg.dwDestGameID = pMsg->GameID;
					SendNetCmdToDB(&msg);
				}
				else
				{
					//直接发送玩家的数据
					LC_Cmd_QueryUserOnce msg;
					SetMsgInfo(msg, GetMsgType(Main_Query, LC_QueryUserOnce), sizeof(LC_Cmd_QueryUserOnce));
					msg.RoleInfo.bGender = pDestRole->GetRoleInfo().bGender;
					msg.RoleInfo.bIsOnline = true;
					msg.RoleInfo.dwAchievementPoint = pDestRole->GetRoleInfo().dwAchievementPoint;
					msg.RoleInfo.dwFaceID = pDestRole->GetRoleInfo().dwFaceID;
					msg.RoleInfo.dwUserID = pDestRole->GetRoleInfo().dwUserID;
					TCHARCopy(msg.RoleInfo.NickName, CountArray(msg.RoleInfo.NickName), pDestRole->GetRoleInfo().NickName, _tcslen(pDestRole->GetRoleInfo().NickName));
					msg.RoleInfo.TitleID = pDestRole->GetRoleInfo().TitleID;
					for (int i = 0; i < MAX_CHARM_ITEMSUM; ++i)
						msg.RoleInfo.CharmArray[i] = pDestRole->GetRoleInfo().CharmArray[i];
					msg.RoleInfo.wLevel = pDestRole->GetRoleInfo().wLevel;
					msg.RoleInfo.GameID = pDestRole->GetRoleInfo().GameID;
					pRole->SendDataToClient(&msg);
				}
				return true;
			}
			break;
		case CL_QueryUserByUserID:
		{
			CL_Cmd_QueryUserByUserID* pMsg = (CL_Cmd_QueryUserByUserID*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			if (pMsg->dwDestUserID == pRole->GetUserID())
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pDestRole = m_RoleManager.QueryUser(pMsg->dwDestUserID);
			if (!pDestRole)
			{
				//发送数据库请求
				DBR_Cmd_Query_RoleInfoByUserID msg;
				SetMsgInfo(msg,DBR_Query_RoleInfoByUserID, sizeof(DBR_Cmd_Query_RoleInfoByUserID));
				msg.dwUserID = pRole->GetUserID();
				msg.dwDestUserID = pMsg->dwDestUserID;
				SendNetCmdToDB(&msg);
			}
			else
			{
				//直接发送玩家的数据
				LC_Cmd_QueryUserOnce msg;
				SetMsgInfo(msg,GetMsgType(Main_Query, LC_QueryUserOnce), sizeof(LC_Cmd_QueryUserOnce));
				msg.RoleInfo.bGender = pDestRole->GetRoleInfo().bGender;
				msg.RoleInfo.bIsOnline = true;
				msg.RoleInfo.dwAchievementPoint = pDestRole->GetRoleInfo().dwAchievementPoint;
				msg.RoleInfo.dwFaceID = pDestRole->GetRoleInfo().dwFaceID;
				msg.RoleInfo.dwUserID = pDestRole->GetRoleInfo().dwUserID;
				TCHARCopy(msg.RoleInfo.NickName, CountArray(msg.RoleInfo.NickName), pDestRole->GetRoleInfo().NickName, _tcslen(pDestRole->GetRoleInfo().NickName));
				msg.RoleInfo.TitleID = pDestRole->GetRoleInfo().TitleID;
				for (int i = 0; i < MAX_CHARM_ITEMSUM;++i)
					msg.RoleInfo.CharmArray[i] = pDestRole->GetRoleInfo().CharmArray[i];
				msg.RoleInfo.wLevel = pDestRole->GetRoleInfo().wLevel;
				msg.RoleInfo.GameID = pDestRole->GetRoleInfo().GameID;
				pRole->SendDataToClient(&msg);
			}
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleDataBaseLoadQueryUserInfo(NetCmd* pCmd)
{
	//接收到DBO的命令了 我们可以开始处理
	DBO_Cmd_QueryRoleInfo * pMsg = (DBO_Cmd_QueryRoleInfo*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	//将命令转化为 LC 命令
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	std::vector<tagQueryRoleInfo>& pVec = pRole->GetQueryRoleVec();
	if ((pMsg->States & MsgBegin) != 0)
	{
		pVec.clear();
	}
	for (WORD i = 0; i < pMsg->Sum; ++i)
	{
		pVec.push_back(pMsg->Array[i]);
	}
	if ((pMsg->States & MsgEnd) != 0)
	{
		//查询完毕了 
		DWORD PageSize = sizeof(LC_Cmd_QueryUserInfo)+sizeof(tagQueryRoleInfoClient)*(pVec.size()-1);
		LC_Cmd_QueryUserInfo * msg = (LC_Cmd_QueryUserInfo*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return false;
		}
		msg->SetCmdType(GetMsgType(Main_Query, LC_QueryUserInfo));
		std::vector<tagQueryRoleInfo>::iterator Iter = pVec.begin();
		for (int i = 0; Iter != pVec.end(); ++Iter, ++i)
		{
			msg->Array[i].bGender = Iter->bGender;
			msg->Array[i].dwUserID = Iter->dwUserID;
			msg->Array[i].wLevel = Iter->wLevel;
			msg->Array[i].dwFaceID = Iter->dwFaceID;
			msg->Array[i].bIsOnline = Iter->bIsOnline;
			msg->Array[i].dwAchievementPoint = Iter->dwAchievementPoint;
			msg->Array[i].TitleID = Iter->TitleID;
			TCHARCopy(msg->Array[i].NickName, CountArray(msg->Array[i].NickName), Iter->NickName, _tcslen(Iter->NickName));
			for (BYTE x = 0; x < MAX_CHARM_ITEMSUM; ++x)
				msg->Array[i].CharmArray[x] = Iter->CharmArray[x];
			if (Iter->IsShowIpAddress)
				GetAddressByIP(Iter->ClientIP, msg->Array[i].IPAddress, CountArray(msg->Array[i].IPAddress));
			else
				TCHARCopy(msg->Array[i].IPAddress, CountArray(msg->Array[i].IPAddress), Defalue_Ip_Address, _tcslen(Defalue_Ip_Address));
			msg->Array[i].VipLevel = Iter->VipLevel;
			msg->Array[i].IsInMonthCard = Iter->IsInMonthCard;
			msg->Array[i].GameID = Iter->GameID;
		}

		std::vector<LC_Cmd_QueryUserInfo*> pVec2;
		SqlitMsg(msg, PageSize, pVec.size(), true, pVec2);
		free(msg);
		if (!pVec2.empty())
		{
			std::vector<LC_Cmd_QueryUserInfo*>::iterator Iter = pVec2.begin();
			for (; Iter != pVec2.end(); ++Iter)
			{
				pRole->SendDataToClient(*Iter);
				free(*Iter);
			}
			pVec2.clear();
		}
		pVec.clear();
	}
	return true;
}
bool FishServer::OnHandleDataBaseLoadQueryUserInfoByGameID(NetCmd* pCmd)
{
	DBO_Cmd_Query_RoleInfoByGameID* pMsg = (DBO_Cmd_Query_RoleInfoByGameID*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	LC_Cmd_QueryUserOnce msg;
	SetMsgInfo(msg, GetMsgType(Main_Query, LC_QueryUserOnce), sizeof(LC_Cmd_QueryUserOnce));

	msg.RoleInfo.bGender = pMsg->RoleInfo.bGender;
	msg.RoleInfo.dwUserID = pMsg->RoleInfo.dwUserID;
	msg.RoleInfo.wLevel = pMsg->RoleInfo.wLevel;
	msg.RoleInfo.dwFaceID = pMsg->RoleInfo.dwFaceID;
	msg.RoleInfo.bIsOnline = pMsg->RoleInfo.bIsOnline;
	msg.RoleInfo.dwAchievementPoint = pMsg->RoleInfo.dwAchievementPoint;
	msg.RoleInfo.TitleID = pMsg->RoleInfo.TitleID;
	msg.RoleInfo.GameID = pMsg->RoleInfo.GameID;
	TCHARCopy(msg.RoleInfo.NickName, CountArray(msg.RoleInfo.NickName), pMsg->RoleInfo.NickName, _tcslen(pMsg->RoleInfo.NickName));
	for (BYTE x = 0; x < MAX_CHARM_ITEMSUM; ++x)
		msg.RoleInfo.CharmArray[x] = pMsg->RoleInfo.CharmArray[x];
	//GetAddressByIP(pMsg->RoleInfo.ClientIP, msg.RoleInfo.IPAddress, CountArray(msg.RoleInfo.IPAddress));

	if (pMsg->RoleInfo.IsShowIpAddress)
		GetAddressByIP(pMsg->RoleInfo.ClientIP, msg.RoleInfo.IPAddress, CountArray(msg.RoleInfo.IPAddress));
	else
		TCHARCopy(msg.RoleInfo.IPAddress, CountArray(msg.RoleInfo.IPAddress), Defalue_Ip_Address, _tcslen(Defalue_Ip_Address));

	msg.RoleInfo.VipLevel = pMsg->RoleInfo.VipLevel;
	msg.RoleInfo.IsInMonthCard = pMsg->RoleInfo.IsInMonthCard;

	pRole->SendDataToClient(&msg);
	return true;
}
bool FishServer::OnHandleDataBaseLoadQueryUserInfoByUserID(NetCmd* pCmd)
{
	DBO_Cmd_Query_RoleInfoByUserID* pMsg = (DBO_Cmd_Query_RoleInfoByUserID*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	LC_Cmd_QueryUserOnce msg;
	SetMsgInfo(msg,GetMsgType(Main_Query, LC_QueryUserOnce), sizeof(LC_Cmd_QueryUserOnce));

	msg.RoleInfo.bGender = pMsg->RoleInfo.bGender;
	msg.RoleInfo.dwUserID = pMsg->RoleInfo.dwUserID;
	msg.RoleInfo.wLevel = pMsg->RoleInfo.wLevel;
	msg.RoleInfo.dwFaceID = pMsg->RoleInfo.dwFaceID;
	msg.RoleInfo.bIsOnline = pMsg->RoleInfo.bIsOnline;
	msg.RoleInfo.dwAchievementPoint = pMsg->RoleInfo.dwAchievementPoint;
	msg.RoleInfo.TitleID = pMsg->RoleInfo.TitleID;
	msg.RoleInfo.GameID = pMsg->RoleInfo.GameID;
	TCHARCopy(msg.RoleInfo.NickName, CountArray(msg.RoleInfo.NickName), pMsg->RoleInfo.NickName, _tcslen(pMsg->RoleInfo.NickName));
	for (BYTE x = 0; x < MAX_CHARM_ITEMSUM; ++x)
		msg.RoleInfo.CharmArray[x] = pMsg->RoleInfo.CharmArray[x];
	//GetAddressByIP(pMsg->RoleInfo.ClientIP, msg.RoleInfo.IPAddress, CountArray(msg.RoleInfo.IPAddress));

	if (pMsg->RoleInfo.IsShowIpAddress)
		GetAddressByIP(pMsg->RoleInfo.ClientIP, msg.RoleInfo.IPAddress, CountArray(msg.RoleInfo.IPAddress));
	else
		TCHARCopy(msg.RoleInfo.IPAddress, CountArray(msg.RoleInfo.IPAddress), Defalue_Ip_Address, _tcslen(Defalue_Ip_Address));

	msg.RoleInfo.VipLevel = pMsg->RoleInfo.VipLevel;
	msg.RoleInfo.IsInMonthCard = pMsg->RoleInfo.IsInMonthCard;

	pRole->SendDataToClient(&msg);
	return true;
}
//Check------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkCheck(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	//接收客户端发送来的签到的消息
	switch (pCmd->SubCmdType)
	{
		/*case CL_GetRoleCheckInfo:
		{
			return pRole->GetRoleCheckManager().GetRoleCheckInfo();
		}*/
		case CL_CheckNowDay:
		{
			return pRole->GetRoleCheckManager().RoleChecking();
		}
		case CL_CheckOtherDay:
		{
			CL_Cmd_CheckOtherDay* pMsg = (CL_Cmd_CheckOtherDay*)pCmd;
			return pRole->GetRoleCheckManager().RoleCheckeOnther(pMsg->Day);
		}
	}
	return true;
}
//bool FishServer::OnHandleDataBaseLoadUserCheckInfo(NetCmd* pCmd)
//{
//	//数据库返回结果
//	DBO_Cmd_LoadRoleCheckInfo* pMsg = (DBO_Cmd_LoadRoleCheckInfo*)pCmd;
//	if (!pMsg)
//	{
//		ASSERT(false);
//		return false;
//	}
//	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
//	if (!pRole)
//	{
//		ASSERT(false);
//		return false;
//	}
//	pRole->GetRoleCheckManager().LoadRoleCheckInfoResult(pMsg);
//	return true;
//}
//Task--------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkTask(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	//处理客户端发送来的任务命令
	switch (pCmd->SubCmdType)
	{
		case CL_GetRoleTaskInfo:
		{
			pRole->GetRoleTaskManager().SendAllTaskToClient();
			return true;
		}
		case CL_GetTaskReward:
		{
			//获取任务奖励
			CL_Cmd_GetTaskReward* pMsg = (CL_Cmd_GetTaskReward*)pCmd;
	;		if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleTaskManager().OnFinishTask(pMsg->TaskID);
			return true;
		}
		case CL_GetOnceTaskInfo:
		{
			CL_Cmd_GetOnceTaskInfo* pMsg = (CL_Cmd_GetOnceTaskInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			RoleTaskBase* pTask = pRole->GetRoleTaskManager().QueryTask(pMsg->TaskID);
			if (!pTask)
			{
				ASSERT(false);
				return false;
			}
			LC_Cmd_GetOnceTaskInfo msg;
			SetMsgInfo(msg,GetMsgType(Main_Task, LC_GetOnceTaskInfo), sizeof(LC_Cmd_GetOnceTaskInfo));
			msg.TaskInfo = pTask->GetTaskInfo();
			pRole->SendDataToClient(&msg);
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleSocketTask(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	switch (pCmd->SubCmdType)
	{
	case CG_GetGlobelTaskInfo:
		{
			CG_Cmd_GetGlobelTaskInfo* pMsg = (CG_Cmd_GetGlobelTaskInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			m_EventManager.OnLoadGlobelTask(pMsg);
			//当接收到全部的任务的数据的时候 我们进行处理
			//1.更新全部玩家的任务列表 进行处理
			m_RoleManager.OnUpdateRoleTask();//关系玩家全部的任务 按中央服务器控制进行
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleDataBaseLoadUserTaskInfo(NetCmd* pCmd)
{
	//从数据库读取任务信息
	DBO_Cmd_LoadRoleTask*  pDB = (DBO_Cmd_LoadRoleTask*)pCmd;
	if (!pDB)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pDB->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleTaskManager().OnLoadAllTaskInfoByDB(pDB);
	return true;
}
//成就---------------------
bool FishServer::OnHandleTCPNetworkAchievement(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	//处理客户端发送来的任务命令
	switch (pCmd->SubCmdType)
	{
	case CL_GetRoleAchievementInfo:
		{
			pRole->GetRoleAchievementManager().SendAllAchievementToClient();
			return true;
		}
	case CL_GetAchievementReward:
		{
			//获取任务奖励
			CL_Cmd_GetAchievementReward* pMsg = (CL_Cmd_GetAchievementReward*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleAchievementManager().OnFinishAchievement(pMsg->AchievementID);
			return true;
		}
	case CL_GetOnceAchievementInfo:
		{
			CL_Cmd_GetOnceAchievementInfo* pMsg = (CL_Cmd_GetOnceAchievementInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			RoleAchievementBase* pAchievement = pRole->GetRoleAchievementManager().QueryAchievement(pMsg->AchievementID);
			if (!pAchievement)
			{
				ASSERT(false);
				return false;
			}
			LC_Cmd_GetOnceAchievementInfo msg;
			SetMsgInfo(msg,GetMsgType(Main_Achievement, LC_GetOnceAchievementInfo), sizeof(LC_Cmd_GetOnceAchievementInfo));
			msg.AchievementInfo = pAchievement->GetAchievementInfo();
			pRole->SendDataToClient(&msg);
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleDataBaseLoadUserAchievementInfo(NetCmd* pCmd)
{
	//从数据库读取任务信息
	DBO_Cmd_LoadRoleAchievement*  pDB = (DBO_Cmd_LoadRoleAchievement*)pCmd;
	if (!pDB)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pDB->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleAchievementManager().OnLoadAllAchievementInfoByDB(pDB);
	return true;
}
//Month---------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkMonth(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
		case CL_SignUpMonth:
		{
			CL_Cmd_SignUpMonth * pMsg = (CL_Cmd_SignUpMonth*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			if (pRole->GetLevel() < GetFishConfig().GetMonthConfig().m_MonthBeginLevel)
			{
				ASSERT(false);
				return false;
			}
			//判断是符合比赛的要求倍率
			HashMap<BYTE, tagMonthConfig>::iterator Iter = GetFishConfig().GetMonthConfig().m_MonthMap.find(pMsg->MonthID);
			if (Iter == GetFishConfig().GetMonthConfig().m_MonthMap.end())
			{
				ASSERT(false);
				return false;
			}
			if (!pRole->GetRoleRate().IsCanUseRateIndex(Iter->second.MinRateLimit))
			{
				ASSERT(false);
				return false;
			}
			//玩家去报名比赛去
			GC_Cmd_SignUpMonth msg;
			SetMsgInfo(msg,GetMsgType(Main_Month, GC_SignUpMonth), sizeof(GC_Cmd_SignUpMonth));
			msg.dwUserID = pRole->GetUserID();
			msg.MonthID = pMsg->MonthID;
			pRole->SendDataToCenter(&msg);
			return true;
		}
		case CL_JoinMonth:
		{
			CL_Cmd_JoinMonth * pMsg = (CL_Cmd_JoinMonth*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			if (pRole->GetLevel() < GetFishConfig().GetMonthConfig().m_MonthBeginLevel)
			{
				ASSERT(false);
				return false;
			}
			HashMap<BYTE, tagMonthConfig>::iterator IterMap = GetFishConfig().GetMonthConfig().m_MonthMap.find(pMsg->MonthID);
			if (IterMap == GetFishConfig().GetMonthConfig().m_MonthMap.end())
			{
				ASSERT(false);
				return false;
			}
			GC_Cmd_JoinMonth msg;
			SetMsgInfo(msg,GetMsgType(Main_Month, GC_JoinMonth), sizeof(GC_Cmd_JoinMonth));
			msg.dwUserID = pRole->GetUserID();
			msg.MonthID = pMsg->MonthID;
			msg.TableID = 0xffff;//无须调配桌子
			pRole->SendDataToCenter(&msg);
			return true;
		}
		case CL_ChangeUserAddMonthGlobelNum:
		{
			pRole->GetRoleMonth().OnRoleAddMonthGlobel();
			return true;
		}
		case CL_LoadRoleSignUpInfo:
		{
			/*GC_Cmd_LoadRoleSignUp msg;
			SetMsgInfo(msg,GetMsgType(Main_Month, GC_LoadRoleSignUp), sizeof(GC_Cmd_LoadRoleSignUp));
			msg.dwUserID = pRole->GetUserID();
			pRole->SendDataToCenter(&msg);*/
			pRole->GetRoleMonth().SendAllSignUpInfoToClient();//将全部的比赛数据发送到客户端去
			return true;
		}
		//case CL_GetMonthRoleSum:
		//{
		//	//分包发送
		//	WORD wSendSize = 0;
		//	LC_Cmd_GetMonthRoleSum * msg = (LC_Cmd_GetMonthRoleSum*)malloc(SOCKET_TCP_PACKET);
		//	WORD PageSize = sizeof(LC_Cmd_GetMonthRoleSum)-sizeof(tagMonthRoleSum);
		//	int DelIndex = 0;

		//	msg->SetCmdSize(PageSize);
		//	msg->SetCmdType(GetMsgType(Main_Month, LC_GetMonthRoleSum));

		//	std::vector<tagMonthRoleSum>::iterator Iter = m_MonthInfo.begin();
		//	for (int i = 0; Iter != m_MonthInfo.end(); ++Iter)
		//	{
		//		if (PageSize + sizeof(tagMonthRoleSum) >= SOCKET_TCP_PACKET)
		//		{
		//			//当前包已经满了 分包发送
		//			pRole->SendDataToClient(msg);
		//			PageSize = sizeof(LC_Cmd_GetMonthRoleSum)-sizeof(tagMonthRoleSum);
		//			DelIndex = i;
		//			free(msg);
		//			msg = (LC_Cmd_GetMonthRoleSum*)malloc(SOCKET_TCP_PACKET);
		//			msg->SetCmdSize(PageSize);
		//			msg->SetCmdType(GetMsgType(Main_Month, LC_GetMonthRoleSum));
		//		}
		//		msg->MonthSum = i - DelIndex + 1;
		//		msg->MonthInfo[i - DelIndex] = (*Iter);
		//		PageSize += sizeof(tagItemInfo);
		//		++i;
		//		msg->SetCmdSize(PageSize);
		//	}
		//	if (PageSize >= sizeof(LC_Cmd_GetMonthRoleSum))
		//	{
		//		pRole->SendDataToClient(msg);
		//		free(msg);
		//	}
		//	else if (msg)
		//	{
		//		free(msg);
		//	}
		//	NetCmd pNull;
		//	SetMsgInfo(pNull,GetMsgType(Main_Month, LC_GetMonthRoleSumFinish), sizeof(NetCmd));
		//	pRole->SendDataToClient(&pNull);
		//	return true;
		//}
		case CL_ResetMonthInfo:
		{
			CL_Cmd_ResetMonthInfo* pMsg = (CL_Cmd_ResetMonthInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}

			//相当于重新报名
			HashMap<BYTE, tagMonthConfig>::iterator Iter = GetFishConfig().GetMonthConfig().m_MonthMap.find(pMsg->MonthID);
			if (Iter == GetFishConfig().GetMonthConfig().m_MonthMap.end())
			{
				ASSERT(false);
				return false;
			}
			if (static_cast<UINT64>(Iter->second.SignGlobel)  > MAXUINT32 ||
				static_cast<UINT64>(Iter->second.SignMadel)  > MAXUINT32 ||
				static_cast<UINT64>(Iter->second.SignCurrey)  > MAXUINT32
				)
			{
				ASSERT(false);
				return false;
			}
			if (!pRole->LostUserMoney(Iter->second.SignGlobel, Iter->second.SignMadel, Iter->second.SignCurrey,TEXT("重置比赛 扣除货币")))
			{
				ASSERT(false);
				return true;
			}
			if (Iter->second.SignItem.ItemID != 0 && Iter->second.SignItem.ItemSum > 0)
			{
				if (!pRole->GetItemManager().OnDelUserItem(Iter->second.SignItem.ItemID, Iter->second.SignItem.ItemSum))
				{
					pRole->ChangeRoleGlobe(Iter->second.SignGlobel,true);
					pRole->ChangeRoleMedal(Iter->second.SignMadel,TEXT("重置比赛 扣除门票失败 归还已经扣除的奖牌"));
					pRole->ChangeRoleCurrency(Iter->second.SignCurrey, TEXT("重置比赛 扣除门票失败 归还已经扣除的钻石"));
					ASSERT(false);
					return false;
				}
			}
			GC_Cmd_ResetMonthInfo msg;
			SetMsgInfo(msg,GetMsgType(Main_Month, GC_ResetMonthInfo), sizeof(GC_Cmd_ResetMonthInfo));
			msg.dwUserID = pRole->GetUserID();
			msg.MonthID = pMsg->MonthID;
			pRole->SendDataToCenter(&msg);
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleSocketMonth(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	//接收到中央服务器发送来的比赛的信息
	switch (pCmd->SubCmdType)
	{
		case CG_SignUpMonth:
		{
			CG_Cmd_SignUpMonth* pMsg = (CG_Cmd_SignUpMonth*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			if (!pMsg->Result)
			{
				LC_Cmd_SignUpMonthFiled msg;
				SetMsgInfo(msg,GetMsgType(Main_Month, LC_SignUpMonthFiled), sizeof(LC_Cmd_SignUpMonthFiled));
				msg.MonthID = pMsg->MonthID;
				pRole->SendDataToClient(&msg);
				return true;
			}
			else
			{
				//中央服务器询问GameServer 玩家扣除比赛报名的门票
				HashMap<BYTE, tagMonthConfig>::iterator Iter = GetFishConfig().GetMonthConfig().m_MonthMap.find(pMsg->MonthID);
				if (Iter == GetFishConfig().GetMonthConfig().m_MonthMap.end())
				{
					LC_Cmd_SignUpMonthFiled msg;
					SetMsgInfo(msg,GetMsgType(Main_Month, LC_SignUpMonthFiled), sizeof(LC_Cmd_SignUpMonthFiled));
					msg.MonthID = pMsg->MonthID;
					pRole->SendDataToClient(&msg);
					return true;
				}
				else if (pRole->IsRobot())
				{
					GC_Cmd_SignUpMonthResult msg;
					SetMsgInfo(msg, GetMsgType(Main_Month, GC_SignUpMonthResult), sizeof(GC_Cmd_SignUpMonthResult));
					msg.dwUserID = pRole->GetUserID();
					msg.MonthID = pMsg->MonthID;
					pRole->SendDataToCenter(&msg);
					return true;
				}
				else
				{
					//1.扣除钱币
					if (static_cast<UINT64>(Iter->second.SignGlobel)  > MAXUINT32 ||
						static_cast<UINT64>(Iter->second.SignMadel)  > MAXUINT32 ||
						static_cast<UINT64>(Iter->second.SignCurrey)  > MAXUINT32
						)
					{
						ASSERT(false);
						return false;
					}
					if (!pRole->LostUserMoney(Iter->second.SignGlobel, Iter->second.SignMadel, Iter->second.SignCurrey,TEXT("报名比赛 扣除货币")))
					{
						LC_Cmd_SignUpMonthFiled msg;
						SetMsgInfo(msg,GetMsgType(Main_Month, LC_SignUpMonthFiled), sizeof(LC_Cmd_SignUpMonthFiled));
						msg.MonthID = pMsg->MonthID;
						pRole->SendDataToClient(&msg);
						return true;
					}
					if (Iter->second.SignItem.ItemID != 0 && Iter->second.SignItem.ItemSum > 0)
					{
						if (!pRole->GetItemManager().OnDelUserItem(Iter->second.SignItem.ItemID, Iter->second.SignItem.ItemSum))
						{
							pRole->ChangeRoleGlobe(Iter->second.SignGlobel,true);
							pRole->ChangeRoleMedal(Iter->second.SignMadel,TEXT("报名比赛 扣除门票失败 归还已经扣除的奖牌"));
							pRole->ChangeRoleCurrency(Iter->second.SignCurrey, TEXT("报名比赛 扣除门票失败 归还已经扣除的钻石"));
							LC_Cmd_SignUpMonthFiled msg;
							SetMsgInfo(msg,GetMsgType(Main_Month, LC_SignUpMonthFiled), sizeof(LC_Cmd_SignUpMonthFiled));
							msg.MonthID = pMsg->MonthID;
							pRole->SendDataToClient(&msg);
							return true;
						}
					}
					//物品扣除成功 通知中央服务器
					GC_Cmd_SignUpMonthResult msg;
					SetMsgInfo(msg,GetMsgType(Main_Month, GC_SignUpMonthResult), sizeof(GC_Cmd_SignUpMonthResult));
					msg.dwUserID = pRole->GetUserID();
					msg.MonthID = pMsg->MonthID;
					pRole->SendDataToCenter(&msg);
					return true;
				}
			}
		}
		case CG_SignUpMonthSucess:
		{
			//玩家报名比赛成功了
			CG_Cmd_SignUpMonthSucess* pMsg = (CG_Cmd_SignUpMonthSucess*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleMonth().OnSignUpSucess(pMsg->MonthID);//玩家报名成功了
			pRole->GetRoleGameData().OnHandleRoleSignUpMonth();//玩家报名成功了
			LC_Cmd_SignUpMonthSucess  msg;
			SetMsgInfo(msg,GetMsgType(Main_Month, LC_SignUpMonthSucess), sizeof(LC_Cmd_SignUpMonthSucess));
			msg.MonthID = pMsg->MonthID;
			pRole->SendDataToClient(&msg);
			return true;
		}
		case CG_JoinMonth:
		{
			//玩家参数比赛的返回消息
			CG_Cmd_JoinMonth* pMsg = (CG_Cmd_JoinMonth*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->MonthInfo.dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			if (!pMsg->Result)
			{
				//LC_Cmd_JoinMonthFiled msg;
				NetCmd pNul;
				SetMsgInfo(pNul,GetMsgType(Main_Month, LC_JoinMonthFiled), sizeof(NetCmd));
				pRole->SendDataToClient(&pNul);
			}
			else
			{
				//参加比赛成功 在GameServer上设置玩家的数据
				pRole->GetRoleMonth().OnLoadMonthInfo(&pMsg->MonthInfo);//在进入桌子之前进行设置属性
				//数据设置完毕后 我们让玩家进入桌子
				//1.去比赛的配置
				HashMap<BYTE, tagMonthConfig>::iterator Iter = GetFishConfig().GetMonthConfig().m_MonthMap.find(pMsg->MonthInfo.bMonthID);
				if (Iter == GetFishConfig().GetMonthConfig().m_MonthMap.end())
				{
					ASSERT(false);
					return false;
				}
				GameTable* pTable = GetTableManager()->GetTable(pMsg->TableID);
				if (pMsg->TableID != 0xffff && pTable && !pTable->IsFull())//桌子存在 并且 桌子为满的情况下
				{
					//玩家进入指定的桌子 我们
					pTable->OnRoleJoinTable(pRole, Iter->second.MonthID, true);
				}
				else
				{
					m_TableManager.OnPlayerJoinTable(Iter->second.TableTypeID, pRole, Iter->second.MonthID);//同步玩家数据 和 玩家比赛的数据
				}
			}
			return true;
		}
		case CG_UserChangeIndex:
		{
			CG_Cmd_UserChangeIndex* pMsg = (CG_Cmd_UserChangeIndex*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleMonth().OnChangeRoleMonthIndex(pMsg->dwIndex, pMsg->dwUpperSocre);
			return true;
		}
		case CG_UserMonthEnd:
		{
			CG_Cmd_UserMonthEnd * pMsg = (CG_Cmd_UserMonthEnd*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleMonth().OnMonthEnd(pMsg->MonthID, pMsg->MonthIndex, pMsg->MonthScores, pMsg->VipScores);
			pRole->GetRoleGameData().OnHandleRoleMonthReward(pMsg->MonthIndex);
			return true;
		}
		case CG_LoadRoleSignUp:
		{
			CG_Cmd_LoadRoleSignUp* pMsg = (CG_Cmd_LoadRoleSignUp*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleMonth().OnLoadSignUpMonthInfo(pMsg);
			return true;
			////将玩家的数据发送到客户端去
			//WORD PageSize = sizeof(LC_Cmd_LoadRoleSignUpInfo)+sizeof(BYTE)*(pMsg->MonthSum - 1);
			//LC_Cmd_LoadRoleSignUpInfo* msg = (LC_Cmd_LoadRoleSignUpInfo*)CreateCmd(GetMsgType(Main_Month, LC_LoadRoleSignUpInfo), PageSize);
			//msg->MonthSum = pMsg->MonthSum;
			//for (int i = 0; i < pMsg->MonthSum; ++i)
			//{
			//	msg->MonthArray[i] = pMsg->MonthArray[i];
			//}
			//pRole->SendDataToClient(msg);
			//free(msg);
			//return true;
		}
		case CG_SendMonthRoleSum:
		{
			CG_Cmd_SendMonthRoleSum* pMsg = (CG_Cmd_SendMonthRoleSum*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			if ((pMsg->States & MsgBegin) != 0)
			{
				m_MonthInfo.clear();
			}
			for (int i = 0; i < pMsg->Sum; ++i)
			{
				HashMap<BYTE, WORD>::iterator Iter = m_MonthInfo.find(pMsg->Array[i].MonthID);
				if (Iter == m_MonthInfo.end())
				{
					m_MonthInfo.insert(HashMap<BYTE, WORD>::value_type(pMsg->Array[i].MonthID, pMsg->Array[i].MonthUserSum));
				}
				else
				{
					Iter->second = pMsg->Array[i].MonthUserSum;
				}
			}
			if ((pMsg->States & MsgEnd) != 0)
			{
				//转发当前命令到客户端去
				DWORD PageSize = sizeof(LC_Cmd_GetMonthRoleSum)+(pMsg->Sum - 1)*sizeof(tagMonthRoleSum);
				LC_Cmd_GetMonthRoleSum* msg = (LC_Cmd_GetMonthRoleSum*)malloc(PageSize);
				if (!msg)
				{
					ASSERT(false);
					return false;
				}
				msg->SetCmdType(GetMsgType(Main_Month, LC_GetMonthRoleSum));
				for (WORD i = 0; i < pMsg->Sum; ++i)
				{
					msg->Array[i] = pMsg->Array[i];
				}

				std::vector<LC_Cmd_GetMonthRoleSum*> pVec;
				SqlitMsg(msg, PageSize, pMsg->Sum, true, pVec);
				free(msg);
				if (!pVec.empty())
				{
					std::vector<LC_Cmd_GetMonthRoleSum*>::iterator Iter = pVec.begin();
					for (; Iter != pVec.end(); ++Iter)
					{
						(*Iter)->States = ((*Iter)->States ^ MsgBegin);//不可以携带开始的状态
						g_FishServer.SendNewCmdToAllClient(*Iter);
						free(*Iter);
					}
					pVec.clear();
				}
			}
			return true;
		}
		case CG_ResetMonthInfo:
		{
			CG_Cmd_ResetMonthInfo* pMsg = (CG_Cmd_ResetMonthInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			//
			if (pMsg->Result)
			{
				pRole->GetRoleMonth().OnResetMonth(pMsg->MonthID);//刷新GameServer上玩家比赛的数据
			}	

			LC_Cmd_ResetMonthInfo msg;
			SetMsgInfo(msg,GetMsgType(Main_Month, LC_ResetMonthInfo), sizeof(LC_Cmd_ResetMonthInfo));
			msg.dwUserID = pRole->GetUserID();
			msg.MonthID = pMsg->MonthID;
			msg.Result = pMsg->Result;
			pRole->SendDataToClient(&msg);
			if (msg.Result)
				pRole->SendDataToTable(&msg);
			return true;
		}
		case CG_GetUserMonthInfo:
		{
			CG_Cmd_GetUserMonthInfo* pMsg = (CG_Cmd_GetUserMonthInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			//将当前里面的全部玩家比赛的数据收集起来
			HashMap<DWORD,CRoleEx*>& pMap =  GetRoleManager()->GetAllRole();
			if (!pMap.empty())
			{
				HashMap<DWORD, CRoleEx*>::iterator Iter = pMap.begin();
				for (; Iter != pMap.end(); ++Iter)
				{
					Iter->second->GetRoleMonth().OnUpMonthInfo(pMsg->MonthID);
				}
			}
			GC_Cmd_GetUserMonthInfoFinish msg;
			SetMsgInfo(msg, GetMsgType(Main_Month, GC_GetUserMonthInfoFinish), sizeof(GC_Cmd_GetUserMonthInfoFinish));
			msg.MonthID = pMsg->MonthID;
			SendNetCmdToCenter(&msg);
			return true;
		}
		case CG_MonthList:
		{
			CG_Cmd_MonthList* pMsg = (CG_Cmd_MonthList*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			//创建新的命令
			LC_Cmd_MonthList msg;
			SetMsgInfo(msg, GetMsgType(Main_Month, LC_MonthList), sizeof(LC_Cmd_MonthList));
			msg.MonthID = pMsg->MonthID;
			ZeroMemory(&msg.MonthInfo, sizeof(tagMonthRoleInfo)*MAX_MonthList_Length);
			for (BYTE i = 0; i < MAX_MonthList_Length; ++i)
			{
				msg.MonthInfo[i] = pMsg->MonthInfo[i];
			}
			//比赛的状态变化了 自动发送到玩家身上去
			HashMap<DWORD, CRoleEx*>& pMap = GetRoleManager()->GetAllRole();
			if (!pMap.empty())
			{
				HashMap<DWORD, CRoleEx*>::iterator Iter = pMap.begin();
				for (; Iter != pMap.end(); ++Iter)
				{
					if (Iter->second->GetRoleMonth().GetMonthID() == pMsg->MonthID)
					{
						Iter->second->SendDataToClient(&msg);
					}
				}
			}
			return true;
		}
		break;
		case CG_MonthStates:
			{
				CG_Cmd_MonthStates* pMsg = (CG_Cmd_MonthStates*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				//接收到比赛状态改变的命令了 我们进行处理
				BYTE MonthID = pMsg->MonthID;
				BYTE MonthStats = pMsg->MonthStates;

				g_FishServer.GetRobotManager().OnHandleMonthStates(MonthID, MonthStats);
				return true;
			}
			break;
		/*case CG_LogonToMonthTable:
			{
				CG_Cmd_LogonToMonthTable* pMsg = (CG_Cmd_LogonToMonthTable*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				pRole->OnUserLogonToMonthResult(pMsg);
				return true;
			}*/
	}
	return true;
}
//Title ----------------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkTitle(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
		case CL_LoadRoleTitle:
		{
			pRole->GetRoleTitleManager().GetRoleTitleToClient();
			return true;
		}
		case CL_ChangeRoleCurrTitle:
		{
			CL_Cmd_ChangeRoleCurrTitle* pMsg = (CL_Cmd_ChangeRoleCurrTitle*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleTitleManager().SetRoleCurrTitleID(pMsg->TitleID);
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleDataBaseLoadUserTitleInfo(NetCmd* pCmd)
{
	DBO_Cmd_LoadRoleTitle* pMsg = (DBO_Cmd_LoadRoleTitle*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleTitleManager().OnLoadRoleTitleResult(pMsg);
	return true;
}
//排行榜----------------------------------------------------------------------------------------------------------------------
//bool FishServer::HandleRankMsg(NetCmd* pCmd)
//{
//	//处理排行榜的数据
//	if (!pCmd)
//		return false;
//	if (pCmd->CmdType == Main_Rank)
//	{
//		switch (pCmd->SubCmdType)
//		{
//			case RL_GetRankReward:
//			{
//				RL_Cmd_GetRankReward* pMsg = (RL_Cmd_GetRankReward*)pCmd;
//				if (!pMsg)
//				{
//					ASSERT(false);
//					return false;
//				}
//				CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
//				if (!pRole)
//				{
//					ASSERT(false);
//					return false;
//				}
//				//玩家领取奖励
//				HashMap<BYTE, tagRankConfig>::iterator Iter = GetFishConfig().GetRankConfig().m_RankMap.find(pMsg->RankID);
//				if (Iter == GetFishConfig().GetRankConfig().m_RankMap.end())
//				{
//					ASSERT(false);
//					return true;
//				}
//				HashMap<BYTE, tagRankReward>::iterator IterVec = Iter->second.RewardMap.find(pMsg->Index);
//				if (IterVec == Iter->second.RewardMap.end())
//				{
//					ASSERT(false);
//					LC_Cmd_GetRankReward msg;
//					SetMsgInfo(msg,GetMsgType(Main_Rank, LC_GetRankReward), sizeof(LC_Cmd_GetRankReward));
//					msg.RankID = pMsg->RankID;
//					pRole->SendDataToClient(&msg);
//					return true;
//				}
//				pRole->OnAddRoleRewardByRewardID(IterVec->second.RewardID);
//				LC_Cmd_GetRankReward msg;
//				SetMsgInfo(msg,GetMsgType(Main_Rank, LC_GetRankReward), sizeof(LC_Cmd_GetRankReward));
//				msg.RankID = pMsg->RankID;
//				pRole->SendDataToClient(&msg);
//				return true;
//			}
//			case RL_GetWeekRankInfo:
//			{
//				RL_Cmd_GetWeekRankInfo* pMsg = (RL_Cmd_GetWeekRankInfo*)pCmd;
//				if (!pMsg)
//				{
//					ASSERT(false);
//					return false;
//				}
//				CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
//				if (!pRole)
//				{
//					ASSERT(false);
//					return false;
//				}
//				//将数据转发到客户端去
//				WORD PageSize = sizeof(LC_Cmd_GetWeekRankInfo)+sizeof(tagRankWeekReward)*(pMsg->RankSum - 1);
//				LC_Cmd_GetWeekRankInfo* msg = (LC_Cmd_GetWeekRankInfo*)CreateCmd(GetMsgType(Main_Rank, LC_GetWeekRankInfo), PageSize);
//				msg->RankSum = pMsg->RankSum;
//				for (int i = 0; i < pMsg->RankSum; ++i)
//				{
//					msg->RankArray[i] = pMsg->RankArray[i];
//				}
//				pRole->SendDataToClient(msg);
//				free(msg);
//				return true;
//			}
//			case RL_GetRankInfoFinish:
//			{
//				RL_Cmd_GetRankInfoFinish* pMsg = (RL_Cmd_GetRankInfoFinish*)pCmd;
//				if (!pMsg)
//				{
//					ASSERT(false);
//					return false;
//				}
//				CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
//				if (!pRole)
//				{
//					ASSERT(false);
//					return false;
//				}
//				NetCmd pNull;
//				SetMsgInfo(pNull,GetMsgType(Main_Rank, LC_GetRankInfoFinish), sizeof(NetCmd));
//				pRole->SendDataToClient(&pNull);
//				return true;
//			}
//		}
//	}
//	return true;
//}
bool FishServer::OnHandleTCPNetworkRank(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
		case CL_GetRankReward:
		{
			CL_Cmd_GetRankReward* pMsg = (CL_Cmd_GetRankReward*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			return pRole->GetRoleRank().GetRankReward(pMsg->RankID);
		}
		case CL_GetRankInfo:
		{
			pRole->GetRoleRank().SendAllRankInfoToClient();
			return true;
		}
	}
	return true;
}
//宝箱---------------------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkChest(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRoleEx = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRoleEx)
	{
		ASSERT(false);
		return false;
	}
	CRole* pRole = m_TableManager.SearchUser(pRoleEx->GetUserID());
	if (!pRole || !pRole->IsActionUser())
	{
		ASSERT(false);
		return false;
	}
	//进行处理宝箱的命令
	switch (pCmd->SubCmdType)
	{
		case CL_GetChestReward:
		{
			CL_Cmd_GetChestReward* pMsg = (CL_Cmd_GetChestReward*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetChestManager().OpenChest(pMsg->ChestOnlyID,pMsg->ChestIndex);
			return true;
		}
		case CL_CloseChest:
		{
			CL_Cmd_CloseChest* pMsg = (CL_Cmd_CloseChest*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetChestManager().CloseChest(pMsg->ChestOnlyID);
			return true;
		}
	}
	return true;
}
//魅力_-------------------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkCharm(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_ChangeOtherUserCharm:
		{
			CL_Cmd_ChangeOtherUserCharm* pMsg = (CL_Cmd_ChangeOtherUserCharm*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pDestRole = m_RoleManager.QueryUser(pMsg->dwDestUserID);
			if (pDestRole)
			{
				//目标玩家直接在线
				//1.扣除物品
				HashMap<BYTE, tagCharmOnce>::iterator Iter = GetFishConfig().GetCharmConfig().CharmIndexMap.find(pMsg->CharmIndex);
				if (Iter == GetFishConfig().GetCharmConfig().CharmIndexMap.end())
				{
					ASSERT(false);
					return false;
				}
				DWORD ItemSum = pRole->GetItemManager().QueryItemCount(Iter->second.ItemID);
				//优先扣除物品 不够的用钱抵扣
				DWORD Need = Iter->second.ItemSum * pMsg->ItemSum;//需要扣除的物品的数量 
				DWORD ConvertItem = (ItemSum >= Need) ? 0 : (Need - ItemSum);
				DWORD DelItem = ((ConvertItem == 0) ? Need : (Need - ConvertItem));
				if (static_cast<UINT64>(Iter->second.UserGlobel) * ConvertItem > MAXUINT32 ||
					static_cast<UINT64>(Iter->second.UserMedal)  * ConvertItem> MAXUINT32 ||
					static_cast<UINT64>(Iter->second.UserCurrcey) * ConvertItem > MAXUINT32
					)
				{
					ASSERT(false);
					return false;
				}
				if (ConvertItem != 0 && !pRole->LostUserMoney(Iter->second.UserGlobel * ConvertItem, Iter->second.UserMedal * ConvertItem, Iter->second.UserCurrcey * ConvertItem,TEXT("赠送魅力物品 扣除货币")))
				{
					ASSERT(false);
					return false;
				}
				//金币已经扣除了 扣除物品
				if (DelItem > 0)
				{
					if (!pRole->GetItemManager().OnDelUserItem(Iter->second.ItemID, DelItem))
					{
						pRole->ChangeRoleGlobe(Iter->second.UserGlobel * ConvertItem,true);
						pRole->ChangeRoleMedal(Iter->second.UserMedal * ConvertItem,TEXT("赠送魅力物品 扣除物品失败 归还已经扣除的奖牌"));
						pRole->ChangeRoleCurrency(Iter->second.UserCurrcey * ConvertItem, TEXT("赠送魅力物品 扣除物品失败 归还已经扣除的钻石"));
						ASSERT(false);
						return false;
					}
				}				
				//2.修改玩家的魅力
				pDestRole->ChangeRoleCharmValue(pMsg->CharmIndex,pMsg->ItemSum);
				if (pDestRole->IsRobot())
				{
					int Value = 0;
					HashMap<BYTE, tagCharmOnce>::iterator Iter = GetFishConfig().GetCharmConfig().CharmIndexMap.find(pMsg->CharmIndex);
					if (Iter != GetFishConfig().GetCharmConfig().CharmIndexMap.end())
					{
						Value = pMsg->ItemSum * Iter->second.ChangeCharmValue;
						g_FishServer.GetRobotManager().OnChangeCharmValue(pRole->GetUserID(), pDestRole->GetUserID(), pMsg->CharmIndex, pMsg->ItemSum, Value);
					}
				}

				//3.发送命令到客户端去
				LC_Cmd_ChangeRoleCharmResult pResult;
				SetMsgInfo(pResult, GetMsgType(Main_Charm, LC_ChangeRoleCharmResult), sizeof(LC_Cmd_ChangeRoleCharmResult));
				pResult.Result = true;
				pResult.dwDestUserID = pDestRole->GetUserID();
				pResult.dwDestUserCharmValue = m_FishConfig.GetCharmValue(pDestRole->GetRoleInfo().CharmArray);
				pRole->SendDataToClient(&pResult);

				//发送命令到全部的桌子上面去  非跨中央服务器的赠送 进行桌子上的表现
				LC_Cmd_TableRoleCharmInfo msgTable;
				SetMsgInfo(msgTable, GetMsgType(Main_Charm, LC_TableRoleCharmInfo), sizeof(LC_Cmd_TableRoleCharmInfo));
				msgTable.SrcUserID = pRole->GetUserID();
				msgTable.DestUserID = pMsg->dwDestUserID;
				msgTable.CharmIndex = pMsg->CharmIndex;
				msgTable.ItemSum = pMsg->ItemSum;
				pRole->SendDataToClient(&msgTable);
				pRole->SendDataToTable(&msgTable);

				return true;
			}
			else
			{
				//1.扣除物品
				HashMap<BYTE, tagCharmOnce>::iterator Iter = GetFishConfig().GetCharmConfig().CharmIndexMap.find(pMsg->CharmIndex);
				if (Iter == GetFishConfig().GetCharmConfig().CharmIndexMap.end())
				{
					ASSERT(false);
					return false;
				}
				DWORD ItemSum = pRole->GetItemManager().QueryItemCount(Iter->second.ItemID);
				//优先扣除物品 不够的用钱抵扣
				DWORD Need = Iter->second.ItemSum * pMsg->ItemSum;//需要扣除的物品的数量 
				DWORD ConvertItem = (ItemSum >= Need) ? 0 : (Need - ItemSum);
				DWORD DelItem = ((ConvertItem == 0) ? Need : (Need - ConvertItem));
				if (static_cast<UINT64>(Iter->second.UserGlobel) * ConvertItem > MAXUINT32 ||
					static_cast<UINT64>(Iter->second.UserMedal)  * ConvertItem> MAXUINT32 ||
					static_cast<UINT64>(Iter->second.UserCurrcey) * ConvertItem > MAXUINT32
					)
				{
					ASSERT(false);
					return false;
				}
				if (ConvertItem != 0 && !pRole->LostUserMoney(Iter->second.UserGlobel * ConvertItem, Iter->second.UserMedal * ConvertItem, Iter->second.UserCurrcey * ConvertItem, TEXT("赠送魅力物品 扣除货币")))
				{
					ASSERT(false);
					return false;
				}
				//金币已经扣除了 扣除物品
				if (DelItem > 0)
				{
					if (!pRole->GetItemManager().OnDelUserItem(Iter->second.ItemID, DelItem))
					{
						pRole->ChangeRoleGlobe(Iter->second.UserGlobel * ConvertItem,true);
						pRole->ChangeRoleMedal(Iter->second.UserMedal * ConvertItem, TEXT("赠送魅力物品 扣除物品失败 归还已经扣除的奖牌"));
						pRole->ChangeRoleCurrency(Iter->second.UserCurrcey * ConvertItem, TEXT("赠送魅力物品 扣除物品失败 归还已经扣除的钻石"));
						ASSERT(false);
						return false;
					}
				}
				//2.发送命令到中央服务器去 如果玩家不存在 也不退物品了  (1.UserID 错误 客户端 负责  2.UserID 已经被删除了)
				CC_Cmd_ChangeOtherUserCharm msg;
				SetMsgInfo(msg, GetMsgType(Main_Charm, CC_ChangeOtherUserCharm), sizeof(CC_Cmd_ChangeOtherUserCharm));
				msg.dwUserID = pRole->GetUserID();
				msg.dwDestUserID = pMsg->dwDestUserID;
				msg.CharmIndex = pMsg->CharmIndex;
				msg.ItemSum = pMsg->ItemSum;
				pRole->SendDataToCenter(&msg);
				return true;


				//HashMap<BYTE, tagCharmOnce>::iterator Iter = GetFishConfig().GetCharmConfig().CharmIndexMap.find(pMsg->CharmIndex);
				//if (Iter == GetFishConfig().GetCharmConfig().CharmIndexMap.end())
				//{
				//	ASSERT(false);
				//	return false;
				//}
				//DWORD GlobelSum = Iter->second.UserGlobel;
				//if (!pRole->ChangeRoleGlobe(GlobelSum*-1 * pMsg->ItemSum, true))
				//{
				//	ASSERT(false);
				//	return false;
				//}
				////2.发送到中央服务器去
				//CC_Cmd_ChangeOtherUserCharm msg;
				//SetMsgInfo(msg,GetMsgType(Main_Charm, CC_ChangeOtherUserCharm), sizeof(CC_Cmd_ChangeOtherUserCharm));
				//msg.dwUserID = pRole->GetUserID();
				//msg.dwDestUserID = pMsg->dwDestUserID;
				//msg.CharmIndex = pMsg->CharmIndex;
				//msg.ItemSum = pMsg->ItemSum;
				//pRole->SendDataToCenter(&msg);
				//return true;
			}
		}
	}
	return true;
}
bool FishServer::OnHandleSocketCharm(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	//处理中央服务器发送来的消息
	switch (pCmd->SubCmdType)
	{
		case CC_ChangeRoleCharmResult:
		{
			CC_Cmd_ChangeRoleCharmResult* pMsg = (CC_Cmd_ChangeRoleCharmResult*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			if (pMsg->Result)
			{
				//告诉客户端 使用添加魅力的物品成功
				LC_Cmd_ChangeRoleCharmResult msg;
				SetMsgInfo(msg,GetMsgType(Main_Charm, LC_ChangeRoleCharmResult), sizeof(LC_Cmd_ChangeRoleCharmResult));
				msg.Result = true;
				msg.dwDestUserID = pMsg->dwDestUserID;
				msg.dwDestUserCharmValue = pMsg->dwDestUserCharmValue;
				pRole->SendDataToClient(&msg);
			}
			else
			{
				HashMap<BYTE, tagCharmOnce>::iterator Iter = GetFishConfig().GetCharmConfig().CharmIndexMap.find(pMsg->CharmIndex);
				if (Iter == GetFishConfig().GetCharmConfig().CharmIndexMap.end())
				{
					ASSERT(false);
					return false;
				}
				tagItemOnce pOnce;
				pOnce.ItemID = Iter->second.ItemID;
				pOnce.ItemSum = Iter->second.ItemSum * pMsg->ItemSum;
				pOnce.LastMin = 0;
				pRole->GetItemManager().OnAddUserItem(pOnce);//添加物品 
				//告诉客户端使用失败了
				LC_Cmd_ChangeRoleCharmResult msg;
				SetMsgInfo(msg,GetMsgType(Main_Charm, LC_ChangeRoleCharmResult), sizeof(LC_Cmd_ChangeRoleCharmResult));
				msg.Result = true;
				msg.dwDestUserID = pMsg->dwDestUserID;
				msg.dwDestUserCharmValue = pMsg->dwDestUserCharmValue;
				pRole->SendDataToClient(&msg);
			}
			return true;
		}
		case CC_ChangeUserCharm:
		{
			CC_Cmd_ChangeUserCharm* pMsg = (CC_Cmd_ChangeUserCharm*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			pRole->ChangeRoleCharmValue(pMsg->CharmIndex,pMsg->ItemSum);
			return true;
		}
	}
	return true;
}
//Shop--------------------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkShop(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRoleEx = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRoleEx)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_ShopItem:
		{
			CL_Cmd_ShopItem* pMsg = (CL_Cmd_ShopItem*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			m_ShopManager.OnShellShopItem(pRoleEx, pMsg->ShopID, pMsg->ShopItemIndex, pMsg->ItemSum);
			return true;
		}
	case CG_RequiredOperatorShopList:
		{
			//CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
			//if (pRole != NULL)
			//{
			//	ASSERT(false);
			//	return false;
			//}
			GC_Cmd_RequiredOperatorShopList msg;
			msg.sum = 0;
			msg.States |= MsgBegin;
			
			SetMsgInfo(msg, GetMsgType(Main_Shop, GC_RequiredOperatorShopList), sizeof(msg));
			const tagFishRechargesMap map = m_FishConfig.GetFishRechargesConfig();
			HashMap<DWORD, tagFishRechargeInfo>::const_iterator it = map.m_FishRechargeMap.begin();
			for (; it != map.m_FishRechargeMap.end(); ++ it)
			{
				if (msg.sum >= MAX_REQUIRED_OPERATOR_SHOP_ITEM_COUT)
				{
					pRoleEx->SendDataToClient(&msg);
					msg.sum = 0;
					msg.States = 0;
				}
				msg.config[msg.sum] = it->second;
				msg.sum++;
			}
			msg.States |= MsgEnd;
			pRoleEx->SendDataToClient(&msg);
		}
		break;
	}
	return true;
}
//实体-------------------------------------------------------------------------------------------------------------
//Entity
bool FishServer::OnHandleTCPNetworlEntity(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
		case CL_LoadUserEntity:
		{
			pRole->GetRoleEntity().GetRoleAddressInfoToClient();
			return true;
		}
		/*case CL_ChangeRoleName:
		{
			CL_Cmd_ChangeRoleName* pMsg = (CL_Cmd_ChangeRoleName*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleEntity().OnChangeRoleName(pMsg->Name);
			return true;
		}
		case CL_ChangeRolePhone:
		{
			CL_Cmd_ChangeRolePhone* pMsg = (CL_Cmd_ChangeRolePhone*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleEntity().OnChangeRolePhone(pMsg->Phone);
			return true;
		}*/
		/*case CL_ChangeRoleEmail:
		{
			CL_Cmd_ChangeRoleEmail* pMsg = (CL_Cmd_ChangeRoleEmail*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleEntity().OnChangeRoleEmail(pMsg->Email);
			return true;
		}*/
		case CL_ChangeRoleEntityItemUseName:
		{
			CL_Cmd_ChangeRoleEntityItemUseName* pMsg = (CL_Cmd_ChangeRoleEntityItemUseName*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleEntity().OnChangeRoleEntityItemUseName(pMsg->EntityItemUseName);
			return true;
		}
		case CL_ChagneRoleEntityItemUsePhone:
		{
			CL_Cmd_ChagneRoleEntityItemUsePhone* pMsg = (CL_Cmd_ChagneRoleEntityItemUsePhone*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleEntity().OnChangeRoleEntityItemUsePhone(pMsg->EntityItemUsePhone);
			return true;
		}
		case CL_ChangeRoleEntityItemUseAddress:
		{
			CL_Cmd_ChangeRoleEntityItemUseAddress* pMsg = (CL_Cmd_ChangeRoleEntityItemUseAddress*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleEntity().OnChangeRoleEntityItemUseAddress(pMsg->EntityItemUseAddres);
			return true;
		}
		/*case CL_ChangeRoleAddress:
		{
			CL_Cmd_ChangeRoleAddress* pMsg = (CL_Cmd_ChangeRoleAddress*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleEntity().OnChangeRoleAddress(pMsg->Addres);
			return true;
		}*/
		/*case CL_ChangeRoleEntityID:
		{
			CL_Cmd_ChangeRoleEntityID* pMsg = (CL_Cmd_ChangeRoleEntityID*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleEntity().OnChangeRoleEntityID(pMsg->IdentityID);
			return true;
		}*/
	}
	return true;
}
bool FishServer::OnHandleDataBaseLoadUserEntityInfo(NetCmd* pCmd)
{
	DBO_Cmd_LoadRoleEntity* pMsg = (DBO_Cmd_LoadRoleEntity*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleEntity().OnLoadRoleAddressInfoResult(pMsg);
	return true;
}
//活动----------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkAction(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	//处理客户端发送来的任务命令
	switch (pCmd->SubCmdType)
	{
		case CL_GetRoleActionInfo:
		{
			pRole->GetRoleActionManager().SendAllActionToClient();
			return true;
		}
		case CL_GetActionReward:
		{
			//获取任务奖励
			CL_Cmd_GetActionReward* pMsg = (CL_Cmd_GetActionReward*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleActionManager().OnFinishAction(pMsg->ActionID, pMsg->OnceID);
			return true;
		}
		case CL_GetOnceActionInfo:
		{
			CL_Cmd_GetOnceActionInfo* pMsg = (CL_Cmd_GetOnceActionInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			RoleActionBase* pAction = pRole->GetRoleActionManager().QueryAction(pMsg->ActionID);
			if (!pAction)
			{
				ASSERT(false);
				return false;
			}
			LC_Cmd_GetOnceActionInfo msg;
			SetMsgInfo(msg,GetMsgType(Main_Action, LC_GetOnceActionInfo), sizeof(LC_Cmd_GetOnceActionInfo));
			msg.ActionInfo = pAction->GetActionInfo();
			pRole->SendDataToClient(&msg);
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleDataBaseLoadUserActionInfo(NetCmd* pCmd)
{
	//从数据库读取任务信息
	DBO_Cmd_LoadRoleAction*  pDB = (DBO_Cmd_LoadRoleAction*)pCmd;
	if (!pDB)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pDB->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleActionManager().OnLoadAllActionInfoByDB(pDB);
	return true;
}
//Center----------------------------------------------------------------------------------
bool FishServer::OnHandleSocketCenter(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	switch (pCmd->SubCmdType)
	{
	case CC_UserLeaveGame:
		{
			CC_Cmd_UserLeaveGame* pMsg = (CC_Cmd_UserLeaveGame*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			//玩家进行登出
			//不要让玩家直接离开 先提升玩家 被顶掉了 然后删除掉
			NetCmd msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, LC_ServerChangeSocket), sizeof(NetCmd));
			pRole->SendDataToClient(&msg);

			//g_FishServer.ShowInfoToWin("玩家被中央服务器移除");
			m_RoleManager.OnDelUser(pRole->GetUserID(), false, false);//离开玩家的集合 直接顶掉 无须离开中央服务器

			DelClient pDel;
			pDel.LogTime = timeGetTime();
			pDel.SocketID = pRole->GetGameSocketID();
			g_FishServer.AddDelRoleSocket(pDel);
			return true;
		}
		break;
	}
	return true;
}
//Giff------------------------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkGiff(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
		case CL_GetRoleGiff:
			pRole->GetRoleGiffManager().GetRoleGiffToClient();
			return true;
		case CL_GetRoleNowDaySendGiff:
			pRole->GetRoleGiffManager().SendNowDaySendGiffToClient();
			return true;
		case CL_AddRoleGiff:
		{
			CL_Cmd_AddRoleGiff* pMsg = (CL_Cmd_AddRoleGiff*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleGiffManager().SendGiffToOtherRole(pMsg->dwDestUserID);
			return true;
		}
		case CL_GetRoleGiffReward:
		{
			CL_Cmd_GetRoleGiffReward* pMsg = (CL_Cmd_GetRoleGiffReward*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleGiffManager().AcceptRoleGiff(pMsg->GiffID);
			return true;
		}
		case CL_GetAllRoleGiffReward:
		{
			pRole->GetRoleGiffManager().AcceptAllGiff();
			return true;
		}
	}
	return true;
}
bool FishServer::OnHandleDataBaseLoadUserGiff(NetCmd* pCmd)
{
	DBO_Cmd_LoadRoleGiff* pMsg = (DBO_Cmd_LoadRoleGiff*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleGiffManager().OnLoadRoleGiffResult(pMsg);
	return true;
}
bool FishServer::OnHnaldeDataBaseLoadUserSendGiffInfo(NetCmd* pCmd)
{
	//获取玩家当天发送过的赠送
	DBO_Cmd_GetNowDayGiff* pMsg = (DBO_Cmd_GetNowDayGiff*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleGiffManager().OnLoadRoleSendGiffInfo(pMsg);
	return true;
}
bool FishServer::OnHandleDataBaseAddUserGiff(NetCmd* pCmd)
{
	DBO_Cmd_AddRoleGiff* pMsg = (DBO_Cmd_AddRoleGiff*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwDestUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleGiffManager().AddBeGiffResult(pMsg);//添加的结果
	return true;
}
bool FishServer::OnHandleSocketGiff(NetCmd* pCmd)
{
	//中央服务器发送来的命令
	if (!pCmd)
		return false;
	switch (pCmd->SubCmdType)
	{
	case CG_AddRoleGiff:
		{
			CG_Cmd_AddRoleGiff* pMsg = (CG_Cmd_AddRoleGiff*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pDestRole = m_RoleManager.QueryUser(pMsg->dwDestUserID);
			if (!pDestRole)
			{
				ASSERT(false);
				return false;
			}
			pDestRole->GetRoleGiffManager().AddBeGiff(pMsg->dwSrcUserID);
			return true;
		}
	case CG_AddRoleGiffResult:
		{
			CG_Cmd_AddRoleGiffResult* pMsg = (CG_Cmd_AddRoleGiffResult*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pSrcRole = m_RoleManager.QueryUser(pMsg->dwSrcUserID);
			if (!pSrcRole)
			{
				ASSERT(false);
				return false;
			}
			//发送赠送 成功或者失败 通知客户端
			if (pMsg->Result)
				pSrcRole->GetRoleGiffManager().SendGiffToOtherRoleResult(pMsg->dwDestUserID);
			//通知客户端
			LC_Cmd_AddRoleGiffResult msg;
			SetMsgInfo(msg, GetMsgType(Main_Giff, LC_AddRoleGiffResult), sizeof(LC_Cmd_AddRoleGiffResult));
			msg.dwDestUserID = pMsg->dwDestUserID;
			msg.Result = pMsg->Result;
			pSrcRole->SendDataToClient(&msg);
			return true;
		}
	}
	return true;
}
//GlobelShop-----------------------------------------------------------------------------------------------------------
//bool FishServer::OnHandleTCPNetworkGlobelShop(ServerClientData* pClient, NetCmd* pCmd)
//{
//	if (!pClient || !pCmd)
//		return false;
//	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
//	if (!pRole)
//	{
//		ASSERT(false);
//		return false;
//	}
//	switch (pCmd->SubCmdType)
//	{
//		case CL_GlobelShopItem:
//		{
//			CL_Cmd_GlobelShopItem* pMsg = (CL_Cmd_GlobelShopItem*)pCmd;
//			if (!pMsg)
//			{
//				ASSERT(false);
//				return false;
//			}
//			m_GlobelShopManager.OnShopItem(pRole, pMsg->ID, pMsg->Sum);
//			return true;
//		}
//		break;
//	}
//	return true;
//}
//OnlineReward-----------------------------------------------------------------------------------------------------------
bool FishServer::OnHandleTCPNetworkOnlineReward(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
		return false;
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
		case CL_GetOnlineReward:
		{
			CL_Cmd_GetOnlineReward* pMsg = (CL_Cmd_GetOnlineReward*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			m_OnlineRewardManager.OnGetOnlineReward(pRole, pMsg->ID);
			return true;
		}
		break;
	}
	return true;
}
//GameData---------------------------------------------------------------------------------
bool FishServer::OnHandleDataBaseLoadGameData( NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	DBO_Cmd_LoadGameData* pMsg = (DBO_Cmd_LoadGameData*)pCmd;
	CRoleEx* pRole = m_RoleManager.QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleGameData().OnLoadRoleGameDataResult(pMsg);
	return true;
}
//Package
bool FishServer::OnHandleTCPNetworkPackage(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	/*switch (pCmd->SubCmdType)
	{
		case CL_OpenPackage:
		{
			CL_Cmd_OpenPackage* pMsg = (CL_Cmd_OpenPackage*)pCmd;
			m_PackageManager.OnOpenFishPackage(pRole, pMsg->PackageItemID);
			return true;
		}
		break;
	}
	return true;*/

	return false;
}
//GameData
bool FishServer::OnHandleTCPNetworkGameData(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_GetGameData:
		{
			pRole->GetRoleGameData().SendRoleGameDataToClient();
			return true;
		}
		break;
	}
	return true;
}

void FishServer::GetAddressByIP(DWORD IP, TCHAR* pAddress,DWORD ArrayCount)
{
	if (!pAddress)
	{
		ASSERT(false);
		return;
	}
	//将IP地址进行 取反
	IP = htonl(IP);
	char Result[MAX_ADDRESS_LENGTH_IP] = { 0 };
	IpLocating(IP, Result, MAX_ADDRESS_LENGTH_IP);
	UINT Count = 0;
	TCHAR* pResult = CharToWChar(Result, Count);
	if (!pResult)
	{
		ASSERT(false);
		return;
	}
	//对TCHAR 进行截取
	size_t len = _tcsclen(pResult);
	size_t beginIndex = 0;
	//字符串生成成功后 进行处理
	for (size_t i = 0; i < len; ++i)
	{
		if (pResult[i] == TEXT('省'))
		{
			beginIndex = i + 1;
			break;
		}
	}
	if (beginIndex < len && beginIndex != 0)
		TCHARCopy(pAddress, ArrayCount, &pResult[beginIndex], _tcslen(pResult) - beginIndex);
	else
		TCHARCopy(pAddress, ArrayCount, pResult, _tcslen(pResult));
	free(pResult);
}
void FishServer::SendAllMonthPlayerSumToClient(DWORD dwUserID)
{
	//将比赛人数发送到客户端去
	CRoleEx* pRole = GetRoleManager()->QueryUser(dwUserID);
	if (dwUserID != 0 && !pRole)
	{
		ASSERT(false);
		return;
	}
	if (pRole && (pRole->IsAFK() || pRole->IsRobot()))
	{
		//ASSERT(false);
		return;
	}
	DWORD PageSize = sizeof(LC_Cmd_GetMonthRoleSum)+(m_MonthInfo.size() - 1)*sizeof(tagMonthRoleSum);
	LC_Cmd_GetMonthRoleSum* msg = (LC_Cmd_GetMonthRoleSum*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Month, LC_GetMonthRoleSum));
	HashMap<BYTE, WORD>::iterator Iter = m_MonthInfo.begin();
	for (WORD i = 0; Iter != m_MonthInfo.end(); ++Iter, ++i)
	{
		msg->Array[i].MonthID = Iter->first;
		msg->Array[i].MonthUserSum = Iter->second;
	}
	std::vector<LC_Cmd_GetMonthRoleSum*> pVec;
	SqlitMsg(msg, PageSize, m_MonthInfo.size(), true, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_GetMonthRoleSum*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			if (dwUserID == 0)
			{
				SendNewCmdToAllClient(*Iter);
			}
			else
			{
				pRole->SendDataToClient(*Iter);
			}
			free(*Iter);
		}
		pVec.clear();
	}
}
DWORD FishServer::GetAchievementIndex(DWORD dwUserID)
{
	HashMap<DWORD, WORD>::iterator Iter = m_AchjievementList.find(dwUserID);
	if (Iter == m_AchjievementList.end())
	{
		return 0xffffffff;
	}
	else
		return Iter->second;
}
//bool FishServer::RoleIsOnlineByCenter(DWORD dwUserID)
//{
//	if (m_OnlineRoleMap.empty())
//		return false;
//	HashMap<DWORD, DWORD>::iterator Iter = m_OnlineRoleMap.find(dwUserID);
//	if (Iter == m_OnlineRoleMap.end())
//		return false;
//	else
//		return true;
//}
//void FishServer::DelRoleOnlineInfo(DWORD dwUserID)
//{
//	m_OnlineRoleMap.erase(dwUserID);
//}

//Message
bool FishServer::OnHandleTCPNetworkMessage(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_SendMessage:
		{
			//接收客户端发送来的命令 
			//1.桌子上的免费发送
			CL_Cmd_SendMessage* pMsg = (CL_Cmd_SendMessage*)pCmd;
			//判断物品是否足够
			DWORD ItemID = g_FishServer.GetFishConfig().GetSystemConfig().SendMessageItemID;
			WORD ItemSum = g_FishServer.GetFishConfig().GetSystemConfig().SendMessageItemSum;
			if (!pRole->GetItemManager().OnDelUserItem(ItemID, ItemSum))
			{
				ASSERT(false);
				return false;
			}
			SendMessageByType(pMsg->Message, pMsg->MessageSize, MessageType::MT_Center, pMsg->MessageColor, pMsg->StepNum, pMsg->StepSec, 0, false);
			return true;
		}
		break;
	}
	return true;
}
bool FishServer::OnHandleSocketMessage(NetCmd* pCmd)
{
	if (!pCmd)
		return false;
	//处理中央服务器发送来的消息
	switch (pCmd->SubCmdType)
	{
	case CG_SendMessage:
		{
			CG_Cmd_SendMessage* pMsg = (CG_Cmd_SendMessage*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			/*if (pMsg->MessageType != MessageType::MT_Center)
			{
			ASSERT(false);
			return false;
			}*/
			SendMessageByType(pMsg->Message, pMsg->MessageSize, pMsg->MessageType, /*MessageType::MT_Game,*/ pMsg->MessageColor, pMsg->StepNum, pMsg->StepSec, pMsg->Param, true);
			return true;
		}
		break;
	}
	return false;
}
void FishServer::SendMessageByType(TCHAR* pMessage, WORD MessageSize, BYTE MessageType, DWORD MessageColor, BYTE StepNum, BYTE StepSec, DWORD Param, bool IsCenterMessage)
{
	//Param 桌子状态下 表示 桌子ID 玩家状态下 表示玩家ID  GameServer 表示GameServer的ID Center无意义
	//发送命令
	if (MessageType == MessageType::MT_Table || MessageType == MessageType::MT_TableMessage)
	{
		DWORD PageSize = sizeof(LC_Cmd_SendMessage)+sizeof(TCHAR)*(MessageSize+1);
		LC_Cmd_SendMessage* msg = (LC_Cmd_SendMessage*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return;
		}
		CheckMsgSize(PageSize);
		msg->SetCmdType(GetMsgType(Main_Message, LC_SendMessage));
		msg->SetCmdSize(ConvertDWORDToWORD(PageSize));
		msg->MessageType = MessageType;
		msg->MessageColor = MessageColor;
		msg->StepNum = StepNum;
		msg->StepSec = StepSec;
		msg->Param = Param;
		//msg->MessageSize = MessageSize;
		//TCHARCopy(msg->Message, msg->MessageSize, pMessage, MessageSize);
		msg->MessageSize = MessageSize+1;
		memcpy_s(msg->Message, msg->MessageSize * sizeof(TCHAR), pMessage, MessageSize* sizeof(TCHAR));
		msg->Message[MessageSize] = 0;

		GameTable* pTable = GetTableManager()->GetTable(ConvertDWORDToWORD(Param));
		if (pTable)
		{
			pTable->SendDataToTableAllUser(msg);
			
		}
		free(msg);
	}
	else if (MessageType == MessageType::MT_Game || MessageType == MessageType::MT_GameMessage)
	{
		if (IsCenterMessage || m_GameNetworkID == ConvertDWORDToBYTE(Param))
		{
			DWORD PageSize = sizeof(LC_Cmd_SendMessage)+sizeof(TCHAR)*(MessageSize+1);
			LC_Cmd_SendMessage* msg = (LC_Cmd_SendMessage*)malloc(PageSize);
			if (!msg)
			{
				ASSERT(false);
				return;
			}
			CheckMsgSize(PageSize);
			msg->SetCmdType(GetMsgType(Main_Message, LC_SendMessage));
			msg->SetCmdSize(ConvertDWORDToWORD(PageSize));
			msg->MessageType = MessageType;
			msg->MessageColor = MessageColor;
			msg->StepNum = StepNum;
			msg->StepSec = StepSec;
			msg->Param = Param;
			//msg->MessageSize = MessageSize;
			//TCHARCopy(msg->Message, msg->MessageSize, pMessage, MessageSize);
			msg->MessageSize = MessageSize + 1;
			memcpy_s(msg->Message, msg->MessageSize * sizeof(TCHAR), pMessage, MessageSize* sizeof(TCHAR));
			msg->Message[MessageSize] = 0;

			//ShowInfoToWin("发送公告到客户端去!");
			SendNewCmdToAllClient(msg);
			free(msg);
		}
		else
		{
			//发送到中央去
			DWORD PageSize = sizeof(GC_Cmd_SendMessage)+sizeof(TCHAR)*(MessageSize + 1);
			GC_Cmd_SendMessage* msg = (GC_Cmd_SendMessage*)malloc(PageSize);
			if (!msg)
			{
				ASSERT(false);
				return;
			}
			CheckMsgSize(PageSize);
			msg->SetCmdType(GetMsgType(Main_Message, GC_SendMessage));
			msg->SetCmdSize(ConvertDWORDToWORD(PageSize));
			msg->MessageType = MessageType;
			msg->MessageColor = MessageColor;
			msg->Param = Param;
			msg->StepNum = StepNum;
			msg->StepSec = StepSec;
			msg->Param = Param;
			//msg->MessageSize = _tcslen(pMessage) + 1;
			//TCHARCopy(msg->Message, msg->MessageSize, pMessage, _tcslen(pMessage));
			msg->MessageSize = MessageSize + 1;
			memcpy_s(msg->Message, msg->MessageSize * sizeof(TCHAR), pMessage, MessageSize* sizeof(TCHAR));
			msg->Message[MessageSize] = 0;

			SendNetCmdToCenter(msg);
			free(msg);
		}
	}
	else if (MessageType == MessageType::MT_Center || MessageType == MessageType::MT_CenterMessage)
	{
		DWORD PageSize = sizeof(GC_Cmd_SendMessage)+sizeof(TCHAR)*(MessageSize + 1);
		GC_Cmd_SendMessage* msg = (GC_Cmd_SendMessage*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return;
		}
		CheckMsgSize(PageSize);
		msg->SetCmdType(GetMsgType(Main_Message, GC_SendMessage));
		msg->SetCmdSize(ConvertDWORDToWORD(PageSize));
		msg->MessageType = MessageType;
		msg->MessageColor = MessageColor;
		msg->StepNum = StepNum;
		msg->StepSec = StepSec;
		msg->Param = Param;
		//msg->MessageSize = _tcslen(pMessage) + 1;
		//TCHARCopy(msg->Message, msg->MessageSize, pMessage, _tcslen(pMessage));
		msg->MessageSize = MessageSize + 1;
		memcpy_s(msg->Message, msg->MessageSize * sizeof(TCHAR), pMessage, MessageSize* sizeof(TCHAR));
		msg->Message[MessageSize] = 0;

		SendNetCmdToCenter(msg);
		free(msg);
	}
	else if (MessageType == MessageType::MT_User || MessageType == MessageType::MT_UserMessage)
	{
		CRoleEx* pDestUser = GetRoleManager()->QueryUser(Param);
		if (pDestUser)
		{
			DWORD PageSize = sizeof(LC_Cmd_SendMessage)+sizeof(TCHAR)*(MessageSize+1);
			LC_Cmd_SendMessage* msg = (LC_Cmd_SendMessage*)malloc(PageSize);
			if (!msg)
			{
				ASSERT(false);
				return;
			}
			CheckMsgSize(PageSize);
			msg->SetCmdType(GetMsgType(Main_Message, LC_SendMessage));
			msg->SetCmdSize(ConvertDWORDToWORD(PageSize));
			msg->MessageType = MessageType;
			msg->MessageColor = MessageColor;
			msg->StepNum = StepNum;
			msg->StepSec = StepSec;
			msg->Param = Param;
			//msg->MessageSize = _tcslen(pMessage) + 1;
			//TCHARCopy(msg->Message, msg->MessageSize, pMessage, _tcslen(pMessage));
			msg->MessageSize = MessageSize + 1;
			memcpy_s(msg->Message, msg->MessageSize * sizeof(TCHAR), pMessage, MessageSize* sizeof(TCHAR));
			msg->Message[MessageSize] = 0;

			//ShowInfoToWin("发送公告到客户端去!");
			SendNewCmdToAllClient(msg);
			free(msg);
		}
		else
		{
			DWORD PageSize = sizeof(GC_Cmd_SendMessage)+sizeof(TCHAR)*(MessageSize + 1);
			GC_Cmd_SendMessage* msg = (GC_Cmd_SendMessage*)malloc(PageSize);
			if (!msg)
			{
				ASSERT(false);
				return;
			}
			CheckMsgSize(PageSize);
			msg->SetCmdType(GetMsgType(Main_Message, GC_SendMessage));
			msg->SetCmdSize(ConvertDWORDToWORD(PageSize));
			msg->MessageType = MessageType;
			msg->MessageColor = MessageColor;
			msg->StepNum = StepNum;
			msg->StepSec = StepSec;
			msg->Param = Param;
			//msg->MessageSize = _tcslen(pMessage) + 1;
			//TCHARCopy(msg->Message, msg->MessageSize, pMessage, _tcslen(pMessage));
			msg->MessageSize = MessageSize + 1;
			memcpy_s(msg->Message, msg->MessageSize * sizeof(TCHAR), pMessage, MessageSize* sizeof(TCHAR));
			msg->Message[MessageSize] = 0;


			SendNetCmdToCenter(msg);
			free(msg);
		}
	}
	else
	{
		ASSERT(false);
		return;
	}
}
//
bool FishServer::OnHandleTCPNetworkRecharge(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CG_CreateOrder:
		{


			CG_Cmd_CreateOrder* pMsg = (CG_Cmd_CreateOrder*)pCmd;
			tagFishRechargesMap config = m_FishConfig.GetFishRechargesConfig();
			HashMap<DWORD, tagFishRechargeInfo>::iterator it = config.m_FishRechargeMap.begin();
			tagFishRechargeInfo entry;
			bool find = false;
			for (; it != config.m_FishRechargeMap.end(); ++ it)
			{
				
				entry = it->second;
				if (entry.ID == pMsg->ShopIndex)
				{
					find = true;
					break;
				}
			}
			if (find)
			{
				DBR_Cmd_Deal_Create msg;
				msg.shop_id = entry.ID;
				msg.channel_id = pRole->GetChannelID();
				msg.user_id = pRole->GetUserID();
				SetMsgInfo(msg, DBR_Deal_Create, sizeof(msg));
				TCHARCopy(msg.good_id, CountArray(msg.good_id), entry.PayNO, _tcslen(entry.PayNO));
				LogInfoToFile("DomePay.txt", "玩家[%d]向数据库申请创建订单[%d]", msg.user_id, pMsg->ShopIndex);
				SendNetCmdToDB(&msg);

				DBR_Cmd_Deal_Apply_Create_Log msg_log;
				msg_log.ItemID = msg.shop_id;
				msg_log.UserID = msg.user_id;
				msg_log.ChannelID = msg.channel_id;
				SetMsgInfo(msg_log, DBR_Deal_Apply_Create_Log, sizeof(msg_log));
				SendNetCmdToLogDB(&msg_log);
				//msg.good_id
			}
			else
			{
				LogInfoToFile("DomePay.txt", "玩家跑[%d]创建订单失败没有找到商品 %d", pRole->GetUserID(), pMsg->ShopIndex);
			}
		}
		break;
	case CL_Recharge:
		{
			//接收客户端发送来的命令 
			//1.桌子上的免费发送
			CL_Cmd_Recharge* pMsg = (CL_Cmd_Recharge*)pCmd;
			//需要进行区分 
			if (!g_FishServerConfig.GetIsOperateTest())
			{
				/*LC_Cmd_Recharge msg;
				SetMsgInfo(msg, GetMsgType(Main_Recharge, LC_Recharge), sizeof(LC_Cmd_Recharge));
				msg.ID = pMsg->ID;
				msg.Result = false;
				pRole->SendDataToClient(&msg);*/

				//生成一个唯一的订单号到客户端去  并且先暂时记录下数据
				DBR_Cmd_GetRechargeOrderID msg;
				SetMsgInfo(msg, DBR_GetRechargeOrderID, sizeof(DBR_Cmd_GetRechargeOrderID));
				msg.dwUserID = pRole->GetUserID();
				msg.ShopID = pMsg->ID;
				msg.IsPC = pMsg->IsPC;
				SendNetCmdToDB(&msg);
				return true;
			}
			else
			{
				HashMap<DWORD, tagFishRechargeInfo>::iterator Iter = m_FishConfig.GetFishRechargesConfig().m_FishRechargeMap.find(pMsg->ID);
				if (Iter == m_FishConfig.GetFishRechargesConfig().m_FishRechargeMap.end())
				{
					ASSERT(false);
					LC_Cmd_Recharge msg;
					SetMsgInfo(msg, GetMsgType(Main_Recharge, LC_Recharge), sizeof(LC_Cmd_Recharge));
					msg.ID = pMsg->ID;
					msg.Result = false;
					pRole->SendDataToClient(&msg);
					return true;
				}
				if (Iter->second.IsAddCurrcey()&& Iter->second.IsFirstAdd() && !pRole->GetRoleInfo().bIsFirstPayCurrcey)
					return false;
				if (Iter->second.IsAddGlobel() && Iter->second.IsFirstAdd() && !pRole->GetRoleInfo().bIsFirstPayGlobel)
					return false;
				DWORD AddMoney = static_cast<DWORD>(pRole->GetRoleVip().AddReChargeRate() * Iter->second.AddMoney);
				if (Iter->second.IsAddCurrcey())
				{
					DWORD OldGlobelNum = pRole->GetRoleInfo().dwGlobeNum;
					DWORD OldCurrceyNum = pRole->GetRoleInfo().dwCurrencyNum;

					pRole->ChangeRoleCurrency(AddMoney, TEXT("测试充值获得钻石"));
					//修改数字
					if (Iter->second.IsFirstAdd())
						pRole->ChangeRoleIsFirstPayCurrcey();

				}
				else if (Iter->second.IsAddGlobel())
				{
					UINT64 AllGlobel = pRole->GetRoleInfo().dwGlobeNum + Iter->second.AddMoney;
					if (AllGlobel >= m_FishConfig.GetSystemConfig().MaxGobelSum)
					{
						//金币上限了
						return false;
					}
					DWORD OldGlobelNum = pRole->GetRoleInfo().dwGlobeNum;
					DWORD OldCurrceyNum = pRole->GetRoleInfo().dwCurrencyNum;

					pRole->ChangeRoleGlobe(AddMoney, true, true,true);//充值的金币 立刻保存起来
					if (Iter->second.IsFirstAdd())
						pRole->ChangeRoleIsFirstPayGlobel();

				}
				else if (Iter->second.IsAddReward())
				{
					pRole->OnAddRoleRewardByRewardID(Iter->second.RewardID, TEXT("测试充值获得奖励"));
				}
				

				string strNickName = "test";
				for (size_t i = 0; i < 32 - 2; ++i)
				{
					BYTE ID = RandUInt() % 62;//0-61
					char ch = 0;
					if (ID >= 0 && ID < 10)
					{
						ch = ID + 48;
						//48-57
					}
					else if (ID >= 10 && ID < 36)
					{
						ch = ID + 55;
						//65-90
					}
					else
					{
						ch = ID + 61;
						//97-122
					}
					strNickName = strNickName + ch;
				}

				g_DBLogManager.LogUserRechargeLogToDB("游戏服务器内部充值成功", strNickName.c_str(), pRole->GetUserID(), "", "", "", pMsg->ID, Iter->second.dDisCountPrice * 100,0, pRole->GetRoleInfo().dwGlobeNum, pRole->GetRoleInfo().dwCurrencyNum, Iter->second.IsAddGlobel() ? Iter->second.AddMoney : 0, Iter->second.IsAddCurrcey() ? Iter->second.AddMoney : 0, Iter->second.IsAddReward() ? Iter->second.RewardID : 0, SendLogDB);

				pRole->ChangeRoleTotalRechargeSum(Iter->second.dDisCountPrice);//添加玩家总充值数
				pRole->OnHandleEvent(true, true, true, ET_Recharge, 0, pRole->GetRoleInfo().TotalRechargeSum/*Iter->second.dDisCountPrice*/);//充值记录

				LC_Cmd_Recharge msg;
				SetMsgInfo(msg, GetMsgType(Main_Recharge, LC_Recharge), sizeof(LC_Cmd_Recharge));
				msg.ID = pMsg->ID;
				msg.Result = true;
				pRole->SendDataToClient(&msg);
				return true;
			}
		}
		break;
	case CL_IOSRecharge:
		{
			CL_Cmd_IOSRecharge* pMsg = (CL_Cmd_IOSRecharge*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			DWORD PageSize = sizeof(GO_Cmd_IOSRecharge)+sizeof(BYTE)* (pMsg->OrderInfo.Sum - 1);
			CheckMsgSizeReturn(PageSize);
			GO_Cmd_IOSRecharge* msg = (GO_Cmd_IOSRecharge*)malloc(static_cast<WORD>(PageSize));
			if (!msg)
			{
				ASSERT(false);
				return false;
			}
			msg->SetCmdSize(static_cast<WORD>(PageSize));
			msg->SetCmdType(GetMsgType(Main_Recharge, GO_IOSRecharge));
			msg->dwUserID = pRole->GetUserID();
			DWORD StringSize = sizeof(IOSRechargeInfo)+sizeof(BYTE)* (pMsg->OrderInfo.Sum - 1);
			memcpy_s(&msg->OrderInfo, StringSize, &pMsg->OrderInfo, StringSize);
			SendNetCmdToOperate(msg);
			free(msg);
			return true;
		}
		break;
	}
	return true;
}
//

bool FishServer::OnHandleTCPNetworkAnnouncement(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_GetAllAnnouncement:
		{
			m_AnnouncementManager.SendNewAnnouncementToClent(pRole->GetUserID());
			return true;
		} 
	}
	ASSERT(false);
	return false;
}

//运营服务器处理命令
bool FishServer::OnHandleTCpNetworkOperate(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false; 
	}
	if (pCmd->CmdType != Main_Operate)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_RealNameVerification:
		{
			//
			CL_Cmd_RealNameVerification* pMsg = (CL_Cmd_RealNameVerification*)pCmd;

			if (!m_FishConfig.CheckStringIsError(pMsg->Name, MIN_NAME_LENTH, MAX_NAME_LENTH, SCT_Normal))
			{
				ASSERT(false);
				return false;
			}
			if (!m_FishConfig.CheckStringIsError(pMsg->IDCard, MIN_IDENTITY_LENGTH, MAX_IDENTITY_LENGTH, SCT_Normal))
			{
				ASSERT(false);
				return false;
			}

			GO_Cmd_RealNameVerification msg;
			SetMsgInfo(msg, GetMsgType(Main_Operate, GO_RealNameVerification), sizeof(GO_Cmd_RealNameVerification));
			msg.dwUserID = pRole->GetUserID();
			TCHARCopy(msg.Name, CountArray(msg.Name), pMsg->Name, _tcslen(pMsg->Name));
			TCHARCopy(msg.IDCard, CountArray(msg.IDCard), pMsg->IDCard, _tcslen(pMsg->IDCard));
			SendNetCmdToOperate(&msg);
			return true;
		}
	case CL_GetPhoneVerificationNum:
		{
			CL_Cmd_GetPhoneVerificationNum* pMsg = (CL_Cmd_GetPhoneVerificationNum*)pCmd;
			if (!PhoneIsCanUse(pMsg->PhoneNumber))
			{
				ASSERT(false);
				return false;
			}
			GO_Cmd_GetPhoneVerificationNum msg;
			SetMsgInfo(msg, GetMsgType(Main_Operate, GO_GetPhoneVerificationNum), sizeof(GO_Cmd_GetPhoneVerificationNum));
			msg.dwUserID = pRole->GetUserID();
			msg.PhoneNumber = pMsg->PhoneNumber;
			SendNetCmdToOperate(&msg);
			return true;
		}
	case CL_BindPhone:
		{
			CL_Cmd_BindPhone* pMsg = (CL_Cmd_BindPhone*)pCmd;
			GO_Cmd_BindPhone msg;
			SetMsgInfo(msg, GetMsgType(Main_Operate, GO_BindPhone), sizeof(GO_Cmd_BindPhone));
			msg.dwUserID = pRole->GetUserID();
			msg.BindNumber = pMsg->BindNumber;
			msg.SecPasswordCrc1 = pMsg->SecPasswordCrc1;
			msg.SecPasswordCrc2 = pMsg->SecPasswordCrc2;
			msg.SecPasswordCrc3 = pMsg->SecPasswordCrc3;
			SendNetCmdToOperate(&msg);
			return true;
		}
	case CL_BindEmail:
		{
			CL_Cmd_BindEmail* pMsg = (CL_Cmd_BindEmail*)pCmd;

			if (!m_FishConfig.CheckStringIsError(pMsg->EMail, MIN_EMAIL_LENGTH, MAX_EMAIL_LENGTH, SCT_Normal))
			{
				ASSERT(false);
				return false;
			}

			GO_Cmd_BindEmail msg;
			SetMsgInfo(msg, GetMsgType(Main_Operate, CL_BindEmail), sizeof(CL_Cmd_BindEmail));
			msg.dwUserID = pRole->GetUserID();
			TCHARCopy(msg.EMail, CountArray(msg.EMail), pMsg->EMail, _tcslen(pMsg->EMail));
			SendNetCmdToOperate(&msg);
			return true;
		}
	}
	ASSERT(false);
	return false;
}
bool FishServer::HandleOperateMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType == Main_Operate)
	{
		switch (pCmd->SubCmdType)
		{
		case OG_RealNameVerification:
			{
				OG_Cmd_RealNameVerification* pMsg = (OG_Cmd_RealNameVerification*)pCmd;
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				//身份证绑定的返回函数
				if (pMsg->Result)
				{
					pRole->GetRoleEntity().OnChangeRoleName(pMsg->Name);//名称
					pRole->GetRoleEntity().OnChangeRoleEntityID(pMsg->IDCard);//身份证
				}
				//将命令发送到客户端去
				LC_Cmd_RealNameVerification msg;
				SetMsgInfo(msg,GetMsgType(Main_Operate, LC_RealNameVerification), sizeof(LC_Cmd_RealNameVerification));
				msg.ErrorID = pMsg->ErrorID;
				TCHARCopy(msg.IDCard, CountArray(msg.IDCard), pMsg->IDCard, _tcslen(pMsg->IDCard));
				TCHARCopy(msg.Name, CountArray(msg.Name), pMsg->Name, _tcslen(pMsg->Name));
				pRole->SendDataToClient(&msg);
				return true;
			}
		case OG_GetPhoneVerificationNum:
			{
				OG_Cmd_GetPhoneVerificationNum* pMsg = (OG_Cmd_GetPhoneVerificationNum*)pCmd;
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				//手机验证码
				LC_Cmd_GetPhoneVerificationNum msg;
				SetMsgInfo(msg, GetMsgType(Main_Operate, LC_GetPhoneVerificationNum), sizeof(LC_Cmd_GetPhoneVerificationNum));
				msg.ErrorID = pMsg->ErrorID;
				msg.PhoneNumber = pMsg->PhoneNumber;
				pRole->SendDataToClient(&msg);
				return true;
			}
		case OG_BindPhone:
			{
				OG_Cmd_BindPhone* pMsg = (OG_Cmd_BindPhone*)pCmd;
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				bool IsFirst = false;
				if (pMsg->Result)
				{
					if (pRole->GetRoleEntity().GetEntityInfo().Phone == 0)
					{
						IsFirst = true;
						//给予玩家奖励
						//玩家首次绑定手机给予奖励
						WORD RewardID = m_FishConfig.GetSystemConfig().FirstBindPhoneRewardID;
						pRole->OnAddRoleRewardByRewardID(RewardID,TEXT("首次绑定手机奖励"));//给玩家添加奖励
					}
					pRole->GetRoleEntity().OnChangeRolePhone(pMsg->PhoneNumber);
					pRole->GetRoleEntity().OnChangeRoleEntityItemUsePhone(pMsg->PhoneNumber);//手机修改了

					pRole->OnChangeRoleSecPassword(pMsg->SecPasswordCrc1, pMsg->SecPasswordCrc2, pMsg->SecPasswordCrc3,true);
				}
				LC_Cmd_BindPhone msg;
				SetMsgInfo(msg, GetMsgType(Main_Operate, LC_BindPhone), sizeof(LC_Cmd_BindPhone));
				msg.ErrorID = pMsg->ErrorID;
				msg.IsFirstBind = IsFirst;
				msg.PhoneNumber = pMsg->PhoneNumber;
				pRole->SendDataToClient(&msg);
				return true;
			}
		case OG_BuyEntityItem:
			{
				//OG_Cmd_BuyEntityItem* pMsg = (OG_Cmd_BuyEntityItem*)pCmd;
				//CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				//if (!pRole)
				//{
				//	ASSERT(false);
				//	return false;
				//}
				////玩家购买物品 结果
				//HashMap<BYTE, tagShopConfig>::iterator Iter = m_FishConfig.GetShopConfig().ShopMap.find(pMsg->ShopID);
				//if (Iter == m_FishConfig.GetShopConfig().ShopMap.end())
				//{
				//	ASSERT(false);
				//	return false;
				//}
				//HashMap<BYTE, tagShopItemConfig>::iterator IterItem = Iter->second.ShopItemMap.find(pMsg->ShopOnlyID);
				//if (IterItem == Iter->second.ShopItemMap.end())
				//{
				//	ASSERT(false);
				//	return false;
				//}
				//if (IterItem->second.ShopType!= SIT_Entity)
				//{
				//	ASSERT(false);
				//	return false;
				//}
				//if (!pMsg->Result)
				//{
				//	//购买实体物品失败了 归还玩家的金钱
				//	pRole->ChangeRoleGlobe(IterItem->second.PriceGlobel * pMsg->ItemSum,true);
				//	pRole->ChangeRoleMedal(IterItem->second.PriceMabel * pMsg->ItemSum,TEXT("购买实体物品失败 归还已经扣除的奖牌"));
				//	pRole->ChangeRoleCurrency(IterItem->second.PriceCurrey * pMsg->ItemSum, TEXT("购买实体物品失败 归还已经扣除的钻石"));
				//}
				//else
				//{
				//	//购买成功的情况下
				//	//为实体物品的时候 我们直接向数据库写入一条数据 
				//	//DBR 命令 将玩家实体数据 玩家物品数据 插入到数据集合  ItemID,ItemSum,UserID,
				//	DBR_Cmd_AddRoleEntityItem msgDB;
				//	SetMsgInfo(msgDB, DBR_AddRoleEntityItem, sizeof(DBR_Cmd_AddRoleEntityItem));
				//	msgDB.dwUserID = pRole->GetUserID();
				//	msgDB.ItemID = IterItem->second.ItemInfo.ItemID;
				//	msgDB.ItemSum = IterItem->second.ItemInfo.ItemSum * pMsg->ItemSum;
				//	msgDB.UseMedal = IterItem->second.PriceMabel * pMsg->ItemSum;//花费的奖牌数量
				//	//设置现实的地址数据
				//	tagRoleAddressInfo& pInfo = pRole->GetRoleEntity().GetEntityInfo();
				//	TCHARCopy(msgDB.Name, CountArray(msgDB.Name), pInfo.EntityItemUseName, _tcslen(pInfo.EntityItemUseName));
				//	msgDB.Phone = pInfo.EntityItemUsePhone;
				//	TCHARCopy(msgDB.Addres, CountArray(msgDB.Addres), pInfo.EntityItemUseAddres, _tcslen(pInfo.EntityItemUseAddres));
				//	SendNetCmdToSaveDB(&msgDB);
				//	GetAnnouncementManager().OnAddNewAnnouncementOnce(pRole->GetRoleInfo().NickName, Iter->second.ShopID, IterItem->second.ShopItemIndex);
				//}
				////将命令发送到客户端去.
				//LC_Cmd_BuyEntityItem msg;
				//SetMsgInfo(msg, GetMsgType(Main_Operate, LC_BuyEntityItem), sizeof(LC_Cmd_BuyEntityItem));
				//msg.ErrorID = pMsg->ErrorID;
				//pRole->SendDataToClient(&msg);
				return true;
			}
		//case OG_UseRMB:
		//	{
		//		OG_Cmd_UseRMB* pMsg = (OG_Cmd_UseRMB*)pCmd;
		//		CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
		//		if (!pRole)
		//		{
		//			ASSERT(false);
		//			return false;
		//		}
		//		HashMap<DWORD, tagFishRechargeInfo>::iterator Iter = m_FishConfig.GetFishRechargesConfig().m_FishRechargeMap.find(pMsg->OnlyID);
		//		if (Iter == m_FishConfig.GetFishRechargesConfig().m_FishRechargeMap.end())
		//		{
		//			ASSERT(false);
		//			return false;
		//		}
		//		if (pMsg->Result)
		//		{
		//			//购买成功了 我们进行处理
		//			if (Iter->second.IsCurreyOrGlobel)
		//			{
		//				pRole->ChangeRoleCurrency(Iter->second.AddMoney,TEXT("充值获得钻石"));
		//				g_DBLogManager.LogRechargeToDB(pRole->GetUserID(), Iter->second.LoseRMB, 0, Iter->second.AddMoney, 1,pMsg->OnceOnlyID);
		//			}
		//			else
		//			{
		//				pRole->ChangeRoleGlobe(Iter->second.AddMoney, true,true);//充值的金币 立刻保存起来
		//				g_DBLogManager.LogRechargeToDB(pRole->GetUserID(), Iter->second.LoseRMB, Iter->second.AddMoney, 0, 1, pMsg->OnceOnlyID);
		//			}
		//			pRole->OnHandleEvent(true, true, true, ET_Recharge, 0, ConvertFloatToDWORD(Iter->second.LoseRMB));//充值记录
		//		}
		//		LC_Cmd_Recharge msg;
		//		SetMsgInfo(msg, GetMsgType(Main_Recharge, LC_Recharge), sizeof(LC_Cmd_Recharge));
		//		msg.ID = pMsg->OnlyID;
		//		msg.Result = pMsg->Result;
		//		_tcscpy_s(msg.OnceOnlyID, CountArray(msg.OnceOnlyID), pMsg->OnceOnlyID);
		//		pRole->SendDataToClient(&msg);
		//		return true;	  
		//	}
		//case OG_BindEmail:
		//	{
		//		OG_Cmd_BindEmail* pMsg = (OG_Cmd_BindEmail*)pCmd;
		//		CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
		//		if (!pRole)
		//		{
		//			ASSERT(false);
		//			return false;
		//		}
		//		if (pMsg->ErrorID == EC_BindEMail_Sucess)
		//		{
		//			//绑定邮箱成功了
		//			pRole->GetRoleEntity().OnChangeRoleEmail(pMsg->EMail);//设置邮箱的变化
		//		}
		//		LC_Cmd_BindEmail msg;
		//		SetMsgInfo(msg, GetMsgType(Main_Operate, LC_BindEmail), sizeof(LC_Cmd_BindEmail));
		//		msg.ErrorID = pMsg->ErrorID;
		//		_tcscpy_s(msg.EMail, CountArray(msg.EMail), pMsg->EMail);
		//		pRole->SendDataToClient(&msg);
		//		return true;
		//	}
		}
	}
	else if (pCmd->CmdType == Main_Role)
	{
		switch (pCmd->SubCmdType)
		{
		case OL_UpdateAccount:
			{
				OL_Cmd_UpdateAccount* pMsg = (OL_Cmd_UpdateAccount*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				pRole->ChangeRoleFaceID(pMsg->dwFaceID);
				pRole->ChangeRoleGender(pMsg->bGender);
				pRole->ChangeRoleNickName(pMsg->NickName);	
				return true;
			}
			break;
		}
	}
	ASSERT(false);
	return false;
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
bool FishServer::OnHandleLoadWeekRankInfo(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	DBO_Cmd_LoadWeekRankInfo* pMsg = (DBO_Cmd_LoadWeekRankInfo*)pCmd;
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleRank().OnLoadWeekRankInfoResult(pMsg);
	return true;
}

//ExChange
bool FishServer::OnHandleTCPNetworkExChange(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd || !pClient)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_Exchange)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = m_RoleManager.QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_RoleUseExChangeCode:
		{
			CL_Cmd_RoleUseExChangeCode* pMsg = (CL_Cmd_RoleUseExChangeCode*)pCmd;
			m_ExChangeManager.OnUseExChangeCode(pRole,pMsg);
			return true;
		}
		break;
	}
	return false;
}
bool FishServer::OnHandleDataBaseQueryExChange(NetCmd* pCmd)
{
	DBO_Cmd_QueryExChange * pMsg = (DBO_Cmd_QueryExChange*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	m_ExChangeManager.OnUseExChangeCodeDBResult(pMsg);
	return true;
}
bool FishServer::OnHandleGetRechargeOrderID(NetCmd* pCmd)
{
	//客户端玩家获取充值的结果 
	DBO_Cmd_GetRechargeOrderID  * pMsg = (DBO_Cmd_GetRechargeOrderID*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	if (pMsg->OrderID != 0 && pMsg->IsPC)
	{
		//为PC端的 我们想要直接服务器下单
		GO_Cmd_AddNormalOrderID msg;
		SetMsgInfo(msg, GetMsgType(Main_Operate, GO_AddNormalOrderID), sizeof(GO_Cmd_AddNormalOrderID));
		msg.dwUserID = pRole->GetUserID();
		msg.ShopID = pMsg->ShopID;
		msg.OrderID = pMsg->OrderID;
		SendNetCmdToOperate(&msg);
		return true;
	}
	LC_Cmd_GetOrderID msg;
	SetMsgInfo(msg, GetMsgType(Main_Recharge, LC_GetOrderID), sizeof(LC_Cmd_GetOrderID));
	msg.OrderID = pMsg->OrderID;
	msg.ShopID = pMsg->ShopID;
	msg.Result = (pMsg->OrderID != 0);
	pRole->SendDataToClient(&msg);
	return true;
}
void FishServer::OnAddChannelInfo(DWORD UserID, ChannelUserInfo* pInfo)
{
	HashMap<DWORD, ChannelUserInfo*>::iterator Iter = m_ChannelInfo.find(UserID);
	if (Iter != m_ChannelInfo.end())
	{
		free(Iter->second);
		m_ChannelInfo.erase(Iter);
	}
	//进行添加
	DWORD InfoSize = sizeof(ChannelUserInfo)+sizeof(BYTE)*(pInfo->Sum - 1);
	ChannelUserInfo* pNewInfo = (ChannelUserInfo*)malloc(InfoSize);
	memcpy_s(pNewInfo, InfoSize, pInfo, InfoSize);
	m_ChannelInfo.insert(HashMap<DWORD, ChannelUserInfo*>::value_type(UserID, pNewInfo));
}
void FishServer::OnDelChannelInfo(DWORD UserID)
{
	HashMap<DWORD, ChannelUserInfo*>::iterator Iter = m_ChannelInfo.find(UserID);
	if (Iter != m_ChannelInfo.end())
	{
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
ChannelUserInfo* FishServer::GetChannelUserInfo(DWORD UserID)
{
	HashMap<DWORD, ChannelUserInfo*>::iterator Iter = m_ChannelInfo.find(UserID);
	if (Iter != m_ChannelInfo.end())
		return Iter->second;
	else
		return NULL;
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
BYTE FishServer::KickUserByID(DWORD dwUserID,DWORD FreezeMin)
{
	if (FreezeMin == 0)
		return 0;
	//踢掉一个玩家 并且不管是否拥有缓存
	CRoleEx* pRole = m_RoleManager.QueryUser(dwUserID);
	if (!pRole)
	{
		return 0;
	}
	//玩家存在在服务器端我们进行处理
	if (pRole->IsAFK())
	{
		//pRole->ChangeRoleIsOnline(false);//先告诉其他玩家 离线了
		pRole->SendUserLeaveToCenter();//告诉中央服务器玩家离线
		GetTableManager()->OnPlayerLeaveTable(pRole->GetUserID());
		GetRoleManager()->OnDelUser(pRole->GetUserID(), true, true);//从内存中移除掉
		m_RoleCache.OnDleRoleCache(dwUserID);
		//发送命令到数据库去 设置玩家账号冻结了
		DBR_Cmd_SetAccountFreeze msg;
		SetMsgInfo(msg, DBR_SetAccountFreeze, sizeof(DBR_Cmd_SetAccountFreeze));
		msg.dwUserID = pRole->GetUserID();
		msg.FreezeMin = FreezeMin;
		SendNetCmdToSaveDB(&msg);
		return 2;
	}
	else if (pRole->IsExit())
	{
		m_RoleLogonManager.OnDleRoleOnlyInfo(pRole->GetUserID());//移除玩家的唯一凭证

		DBR_Cmd_SetAccountFreeze msg;
		SetMsgInfo(msg, DBR_SetAccountFreeze, sizeof(DBR_Cmd_SetAccountFreeze));
		msg.dwUserID = pRole->GetUserID();
		msg.FreezeMin = FreezeMin;
		SendNetCmdToSaveDB(&msg);
		return 3;
	}
	else
	{
		//玩家在普通状态 
		CloseClientSocket(pRole->GetGameSocketID());
		//删除玩家数据
		m_RoleLogonManager.OnDleRoleOnlyInfo(pRole->GetUserID());//移除玩家的唯一凭证
		m_RoleManager.OnDelUser(pRole->GetUserID(), true, true);//删除玩家退出

		DBR_Cmd_SetAccountFreeze msg;
		SetMsgInfo(msg, DBR_SetAccountFreeze, sizeof(DBR_Cmd_SetAccountFreeze));
		msg.dwUserID = pRole->GetUserID();
		msg.FreezeMin = FreezeMin;
		SendNetCmdToSaveDB(&msg);

		return 4;
	}
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
void FishServer::SendNetCmdToMiniGame(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_MiniGameTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToLogon(BYTE LogonID, NetCmd* pCmd)
{
	if (!pCmd)
		return;
	DWORD PageSize = sizeof(SS_Cmd_GameToLogon)+sizeof(BYTE)* (pCmd->CmdSize - 1);
	CheckMsgSize(PageSize);
	SS_Cmd_GameToLogon* msg = (SS_Cmd_GameToLogon*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdSize(static_cast<WORD>(PageSize));
	msg->SetCmdType(GetMsgType(Main_Server, SS_GameToLogon));
	memcpy_s(msg->Array, pCmd->CmdSize, pCmd, pCmd->CmdSize);
	msg->Length = pCmd->CmdSize;
	msg->LogonID = LogonID;
	SendNetCmdToCenter(msg);
	free(msg);
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

	if (!m_ControlTcp.Init(pConfig->ControlServerListenIP, pControlData, (void*)&m_GameNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("ControlServer连接失败");
		return false;
	}
	OnTcpClientConnect(&m_ControlTcp);
	return true;
}
bool FishServer::ConnectMiniGame()
{
	MiniGameServerConfig* pConfig = g_FishServerConfig.GetMiniGameServerConfig();
	if (!pConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pControlData;
	pControlData.BuffSize = pConfig->BuffSize;
	pControlData.Port = pConfig->GameListenPort;
	pControlData.SleepTime = pConfig->SleepTime;
	pControlData.SocketRecvSize = pConfig->RecvBuffSize;
	pControlData.SocketSendSize = pConfig->SendBuffSize;
	pControlData.ThreadNum = 1;
	pControlData.Timeout = pConfig->TimeOut;
	pControlData.CmdHearbeat = 0;
	pControlData.SendArraySize = pConfig->MaxSendCmdCount;
	pControlData.RecvArraySize = pConfig->MaxRecvCmdCount;

	if (!m_MiniGameTcp.Init(pConfig->GameListenIP, pControlData, (void*)&m_GameNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("MiNiGame连接失败");
		return false;
	}
	OnTcpClientConnect(&m_MiniGameTcp);
	return true;
}
bool FishServer::HandleControlMsg(NetCmd* pCmd)
{
	//处理运营服务器发送来的数据
	if (!pCmd)
		return false;
	switch (pCmd->CmdType)
	{
		case Main_Control:
		{
			switch (pCmd->SubCmdType)
			{
			//case CL_KickUserByID:
			//	{
			//		CL_Cmd_KickUserByID* pMsg = (CL_Cmd_KickUserByID*)pCmd;
			//		LC_Cmd_KickUserResult msg;
			//		SetMsgInfo(msg, GetMsgType(Main_Control, LC_KickUserResult), sizeof(LC_Cmd_KickUserResult));
			//		msg.Result = KickUserByID(pMsg->dwUserID,pMsg->FreezeMin);
			//		msg.dwUserID = pMsg->dwUserID;
			//		msg.ClientID = pMsg->ClientID;
			//		//SendNetCmdToCenter(&msg);
			//		SendNetCmdToControl(&msg);
			//		return true;
			//	}
			//	break;
			case CL_ReloadConfig:
				{
					OnReloadConfig();
					return true;
				}
			case CL_QueryFishPool:
			{
									 HashMap<BYTE, TableInfo>& tableinfo = m_FishConfig.GetTableConfig().m_TableConfig;

									 int nSize = sizeof(LC_CMD_QueryFishPoolResult)+sizeof(TableTypePool)*tableinfo.size();
									 LC_CMD_QueryFishPoolResult *pResult = (LC_CMD_QueryFishPoolResult*)malloc(nSize);
									 pResult->SetCmdSize(nSize);
									 pResult->SetCmdType((Main_Control << 8) | LC_QueryFishPoolResult);
									 pResult->ClientID = ((CL_CMD_QueryFishPool*)pCmd)->ClientID;
									 pResult->byServerid = m_GameNetworkID;				
									 int nIndex = 1;
									 for (HashMap<BYTE, TableInfo>::iterator it = tableinfo.begin(); it != tableinfo.end(); it++)
									 {
										 m_TableManager.QueryPool(it->second.TableTypeID, pResult->table[nIndex].bopen, pResult->table[nIndex].npool);										 
										 nIndex++;
									 }				
									 SendNetCmdToControl(pResult);
									 free(pResult);
									 return true;
			}
			case CL_QueryBlackList:
			{
									  std::list<DWORD>blacklist = m_TableManager.GetBlackList();
									  int nSize = sizeof(LC_CMD_QueryFishBlackListResult)+sizeof(DWORD)*blacklist.size();
									  LC_CMD_QueryFishBlackListResult *pResult = (LC_CMD_QueryFishBlackListResult*)malloc(nSize);
									  pResult->SetCmdSize(nSize);
									  pResult->SetCmdType((Main_Control << 8) | LC_QueryBlackListResult);
									  pResult->ClientID = ((LC_CMD_QueryFishBlackList*)pCmd)->ClientID;
									  pResult->byServerid = m_GameNetworkID;
									  int nIndex = 1;
									  for (std::list<DWORD>::iterator it = blacklist.begin(); it != blacklist.end(); it++)
									  {
										  pResult->dwUserID[nIndex] = *it;
										  nIndex++;
									  }
									  SendNetCmdToControl(pResult);
									  free(pResult);
									  return true;
			}
			case CL_SetBlackList://
			{
									 BYTE nCount = (pCmd->GetCmdSize() - sizeof(LC_CMD_SetFishBlackList)) / sizeof(DWORD);
									 m_TableManager.SetBlackList(((LC_CMD_SetFishBlackList*)pCmd)->dwUserID + 1, nCount);

									 LC_CMD_SetFishBlackListResult result;
									 result.SetCmdSize(sizeof(result));
									 result.SetCmdType((Main_Control << 8) | LC_SetBlackListResult);
									 result.ClientID = ((LC_CMD_SetFishBlackList*)pCmd)->ClientID;
									 result.byServerid = m_GameNetworkID;
									 result.byCount = nCount;
									 SendNetCmdToControl(&result);
									 return  true;
			}
				break;
			}
		}
		break;
	}
	return true;
}
//Lottery
bool FishServer::OnHandleTCPNetwordLottery(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_Lottery)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_GetLotteryReward:
		{
			CL_Cmd_GetLotteryReward* pMsg = (CL_Cmd_GetLotteryReward*)pCmd;
			m_LotteryManager.OnRoleGetLotteryReward(pRole->GetUserID(), pMsg->LotteryID);
			return true;
		}
		break;
	case CL_LotteryUIStates:
		{
			CL_Cmd_LotteryUIStates* pMsg = (CL_Cmd_LotteryUIStates*)pCmd;
			LC_Cmd_LotteryUIStates msg;
			SetMsgInfo(msg, GetMsgType(Main_Lottery, LC_LotteryUIStates), sizeof(LC_Cmd_LotteryUIStates));
			msg.dwUserID = pMsg->dwUserID;
			msg.IsOpen = pMsg->IsOpen;
			pRole->SendDataToTable(&msg);
			return true;
		}
	}
	return false;
}
bool FishServer::OnHandleTCPNetworkMiniGame(ServerClientData* pClient, NetCmd* pCmd)
{
	//处理客户端发送来的命令
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_MiniGame)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	return true;
}
bool FishServer::OnHandleTCPNetworkCar(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_Car)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_CarRoleJoinTable:
		{
			GM_Cmd_RoleJoinMiniGame msgJoin;
			SetMsgInfo(msgJoin, GetMsgType(Main_MiniGame, GM_RoleJoinMiniGame), sizeof(GM_Cmd_RoleJoinMiniGame));
			msgJoin.RoleInfo.dwUserID = pRole->GetUserID();
			msgJoin.RoleInfo.dwGlobelSum = pRole->GetRoleInfo().dwGlobeNum;
			msgJoin.RoleInfo.dwMadleSum = pRole->GetRoleInfo().dwMedalNum;
			msgJoin.RoleInfo.dwCurrceySum = pRole->GetRoleInfo().dwCurrencyNum;
			msgJoin.RoleInfo.VipLevel = pRole->GetRoleInfo().VipLevel;
			msgJoin.RoleInfo.FaceID = pRole->GetRoleInfo().dwFaceID;
			msgJoin.RoleInfo.ParticularStates = pRole->GetRoleInfo().ParticularStates;
			msgJoin.RoleInfo.GameID = pRole->GetRoleInfo().GameID;
			TCHARCopy(msgJoin.RoleInfo.NickName, CountArray(msgJoin.RoleInfo.NickName), pRole->GetRoleInfo().NickName, _tcslen(pRole->GetRoleInfo().NickName));
			SendNetCmdToMiniGame(&msgJoin);

			GM_Cmd_CarRoleJoinTable msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarRoleJoinTable), sizeof(GM_Cmd_CarRoleJoinTable));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);

			return true;
		}
		break;
	case CL_CarRoleLeaveTable:
		{
			GM_Cmd_RoleLeaveMiniGame msgLeave;
			SetMsgInfo(msgLeave, GetMsgType(Main_MiniGame, GM_RoleLeaveMiniGame), sizeof(GM_Cmd_RoleLeaveMiniGame));
			msgLeave.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msgLeave);

			DBR_Cmd_TableChange msgDB;//记录玩家进入
			SetMsgInfo(msgDB, DBR_TableChange, sizeof(DBR_Cmd_TableChange));
			msgDB.dwUserID = pRole->GetUserID();
			msgDB.CurrceySum = pRole->GetRoleInfo().dwCurrencyNum;
			msgDB.GlobelSum = pRole->GetRoleInfo().dwGlobeNum;
			msgDB.MedalSum = pRole->GetRoleInfo().dwMedalNum;
			msgDB.JoinOrLeave = false;
			msgDB.LogTime = time(null);
			msgDB.TableTypeID = 251;
			msgDB.TableMonthID = 251;
			g_FishServer.SendNetCmdToSaveDB(&msgDB);
			g_DBLogManager.LogRoleJoinOrLeaveTableInfo(msgDB.dwUserID, msgDB.GlobelSum, msgDB.CurrceySum, msgDB.MedalSum, msgDB.TableTypeID, msgDB.TableMonthID, false, SendLogDB);

			return true;
		}
		break;
	case CL_CarRoleBetGlobel:
		{
			CL_Cmd_CarRoleBetGlobel* pMsg = (CL_Cmd_CarRoleBetGlobel*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			if (pMsg->AddGlobel > GetFishConfig().GetFishMiNiGameConfig().carConfig.MaxAddGlobel)
			{
				ASSERT(false);
				return false;
			}
			if (pMsg->Index >= MAX_CAR_ClientSum)//0-3
			{
				ASSERT(false);
				return false;
			}
			//1.先扣除玩家的金币
			if (!pRole->ChangeRoleGlobe(pMsg->AddGlobel*-1, true))
			{
				ASSERT(false);
				return false;
			}

			GM_Cmd_CarRoleBetGlobel msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarRoleBetGlobel), sizeof(GM_Cmd_CarRoleBetGlobel));
			msg.dwUserID = pRole->GetUserID();
			msg.AddGlobel = pMsg->AddGlobel;
			msg.Index = pMsg->Index;
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_CarGetBankerList:
		{
			GM_Cmd_CarGetBankerList msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarGetBankerList), sizeof(GM_Cmd_CarGetBankerList));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_CarRoleJoinBankerList:
		{
			LC_Cmd_CarRoleJoinBankerList msgResult;
			SetMsgInfo(msgResult, GetMsgType(Main_Car, LC_CarRoleJoinBankerList), sizeof(LC_Cmd_CarRoleJoinBankerList));
			msgResult.Result = false;

			if (pRole->GetRoleInfo().dwGlobeNum < GetFishConfig().GetFishMiNiGameConfig().carConfig.JoinBankerGlobelSum)
			{
				pRole->SendDataToClient(&msgResult);
				ASSERT(false);
				return false;
			}

			GM_Cmd_CarRoleJoinBankerList msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarRoleJoinBankerList), sizeof(GM_Cmd_CarRoleJoinBankerList));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_CarRoleGetBankerFirstSeat:
		{
			LC_Cmd_CarRoleGetBankerFirstSeat msgError;
			SetMsgInfo(msgError, GetMsgType(Main_Car, LC_CarRoleGetBankerFirstSeat), sizeof(LC_Cmd_CarRoleGetBankerFirstSeat));
			msgError.Result = false;
			if (!pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().carConfig.GetNextBankerNeedGlobel*-1, true))
			{
				pRole->SendDataToClient(&msgError);
				ASSERT(false);
				return false;
			}

			GM_Cmd_CarRoleGetBankerFirstSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarRoleGetBankerFirstSeat), sizeof(GM_Cmd_CarRoleGetBankerFirstSeat));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_CarRoleLeaveBankerList:
		{
			GM_Cmd_CarRoleLeaveBankerList msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarRoleLeaveBankerList), sizeof(GM_Cmd_CarRoleLeaveBankerList));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_CarRoleCanelBankerSeat:
		{
			GM_Cmd_CarRoleCanelBankerSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarRoleCanelBankerSeat), sizeof(GM_Cmd_CarRoleCanelBankerSeat));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_CarRoleJoinVipSeat:
		{
			CL_Cmd_CarRoleJoinVipSeat* pMsg = (CL_Cmd_CarRoleJoinVipSeat*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			GM_Cmd_CarRoleJoinVipSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarRoleJoinVipSeat), sizeof(GM_Cmd_CarRoleJoinVipSeat));
			msg.dwUserID = pRole->GetUserID();
			msg.VipSeatIndex = pMsg->VipSeatIndex;
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_CarRoleLeaveVipSeat:
		{
			GM_Cmd_CarRoleLeaveVipSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarRoleLeaveVipSeat), sizeof(GM_Cmd_CarRoleLeaveVipSeat));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_CarGetNormalSeatInfo:
		{
			CL_Cmd_CarGetNormalSeatInfo * pMsg = (CL_Cmd_CarGetNormalSeatInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			GM_Cmd_CarGetNormalSeatInfo msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, GM_CarGetNormalSeatInfo), sizeof(GM_Cmd_CarGetNormalSeatInfo));
			msg.dwUserID = pRole->GetUserID();
			msg.Page = pMsg->Page;
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_CarRoleBetGlobelByLog:
		{
			CL_Cmd_CarRoleBetGlobelByLog* pMsg = (CL_Cmd_CarRoleBetGlobelByLog*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			DWORD AllGlobel = 0;
			for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
				AllGlobel += pMsg->betGlobel[i];
			LC_Cmd_CarRoleBetGlobelByLog msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleBetGlobelByLog), sizeof(LC_Cmd_CarRoleBetGlobelByLog));
			msg.Result = false;
			if (!pRole->ChangeRoleGlobe(AllGlobel*-1, true, false, false))
			{
				pRole->SendDataToClient(&msg);
				return true;
			}
			GM_Cmd_CarRoleBetGlobelByLog msgSend;
			msgSend.dwUserID = pRole->GetUserID();
			SetMsgInfo(msgSend, GetMsgType(Main_Car, GM_CarRoleBetGlobelByLog), sizeof(GM_Cmd_CarRoleBetGlobelByLog));
			for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
				msgSend.betGlobel[i] = pMsg->betGlobel[i];
			SendNetCmdToMiniGame(&msgSend);
			return true;
		}
		break;
	}
	return false;
}
bool FishServer::OnHandleTCPNetworkDial(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_Dial)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_DialRoleJoinTable:
		{
			GM_Cmd_RoleJoinMiniGame msgJoin;
			SetMsgInfo(msgJoin, GetMsgType(Main_MiniGame, GM_RoleJoinMiniGame), sizeof(GM_Cmd_RoleJoinMiniGame));
			msgJoin.RoleInfo.dwUserID = pRole->GetUserID();
			msgJoin.RoleInfo.dwGlobelSum = pRole->GetRoleInfo().dwGlobeNum;
			msgJoin.RoleInfo.dwMadleSum = pRole->GetRoleInfo().dwMedalNum;
			msgJoin.RoleInfo.dwCurrceySum = pRole->GetRoleInfo().dwCurrencyNum;
			msgJoin.RoleInfo.VipLevel = pRole->GetRoleInfo().VipLevel;
			msgJoin.RoleInfo.FaceID = pRole->GetRoleInfo().dwFaceID;
			msgJoin.RoleInfo.ParticularStates = pRole->GetRoleInfo().ParticularStates;
			msgJoin.RoleInfo.GameID = pRole->GetRoleInfo().GameID;
			TCHARCopy(msgJoin.RoleInfo.NickName, CountArray(msgJoin.RoleInfo.NickName), pRole->GetRoleInfo().NickName, _tcslen(pRole->GetRoleInfo().NickName));
			SendNetCmdToMiniGame(&msgJoin);

			GM_Cmd_DialRoleJoinTable msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialRoleJoinTable), sizeof(GM_Cmd_DialRoleJoinTable));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);

			return true;
		}
		break;
	case CL_DialRoleLeaveTable:
		{
			GM_Cmd_RoleLeaveMiniGame msgLeave;
			SetMsgInfo(msgLeave, GetMsgType(Main_MiniGame, GM_RoleLeaveMiniGame), sizeof(GM_Cmd_RoleLeaveMiniGame));
			msgLeave.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msgLeave);

			DBR_Cmd_TableChange msgDB;//记录玩家进入
			SetMsgInfo(msgDB, DBR_TableChange, sizeof(DBR_Cmd_TableChange));
			msgDB.dwUserID = pRole->GetUserID();
			msgDB.CurrceySum = pRole->GetRoleInfo().dwCurrencyNum;
			msgDB.GlobelSum = pRole->GetRoleInfo().dwGlobeNum;
			msgDB.MedalSum = pRole->GetRoleInfo().dwMedalNum;
			msgDB.JoinOrLeave = false;
			msgDB.LogTime = time(null);
			msgDB.TableTypeID = 251;
			msgDB.TableMonthID = 251;
			g_FishServer.SendNetCmdToSaveDB(&msgDB);
			g_DBLogManager.LogRoleJoinOrLeaveTableInfo(msgDB.dwUserID, msgDB.GlobelSum, msgDB.CurrceySum, msgDB.MedalSum, msgDB.TableTypeID, msgDB.TableMonthID, false, SendLogDB);

			return true;
		}
		break;
	case CL_DialRoleBetGlobel:
		{
			CL_Cmd_DialRoleBetGlobel* pMsg = (CL_Cmd_DialRoleBetGlobel*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			if (pMsg->AddGlobel > GetFishConfig().GetFishMiNiGameConfig().dialConfig.MaxAddGlobel)
			{
				ASSERT(false);
				return false;
			}
			if (pMsg->Index >= MAX_DIAL_ClientSum)//0-3
			{
				ASSERT(false);
				return false;
			}
			//1.先扣除玩家的金币
			if (!pRole->ChangeRoleGlobe(pMsg->AddGlobel*-1, true))
			{
				ASSERT(false);
				return false;
			}

			GM_Cmd_DialRoleBetGlobel msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialRoleBetGlobel), sizeof(GM_Cmd_DialRoleBetGlobel));
			msg.dwUserID = pRole->GetUserID();
			msg.AddGlobel = pMsg->AddGlobel;
			msg.Index = pMsg->Index;
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_DialGetBankerList:
		{
			GM_Cmd_DialGetBankerList msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialGetBankerList), sizeof(GM_Cmd_DialGetBankerList));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_DialRoleJoinBankerList:
		{
			LC_Cmd_DialRoleJoinBankerList msgResult;
			SetMsgInfo(msgResult, GetMsgType(Main_Dial, LC_DialRoleJoinBankerList), sizeof(LC_Cmd_DialRoleJoinBankerList));
			msgResult.Result = false;

			if (pRole->GetRoleInfo().dwGlobeNum < GetFishConfig().GetFishMiNiGameConfig().dialConfig.JoinBankerGlobelSum)
			{
				pRole->SendDataToClient(&msgResult);
				ASSERT(false);
				return false;
			}

			GM_Cmd_DialRoleJoinBankerList msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialRoleJoinBankerList), sizeof(GM_Cmd_DialRoleJoinBankerList));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_DialRoleGetBankerFirstSeat:
		{
			LC_Cmd_DialRoleGetBankerFirstSeat msgError;
			SetMsgInfo(msgError, GetMsgType(Main_Dial, LC_DialRoleGetBankerFirstSeat), sizeof(LC_Cmd_DialRoleGetBankerFirstSeat));
			msgError.Result = false;
			if (!pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().dialConfig.GetNextBankerNeedGlobel*-1, true))
			{
				pRole->SendDataToClient(&msgError);
				ASSERT(false);
				return false;
			}

			GM_Cmd_DialRoleGetBankerFirstSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialRoleGetBankerFirstSeat), sizeof(GM_Cmd_DialRoleGetBankerFirstSeat));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_DialRoleLeaveBankerList:
		{
			GM_Cmd_DialRoleLeaveBankerList msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialRoleLeaveBankerList), sizeof(GM_Cmd_DialRoleLeaveBankerList));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_DialRoleCanelBankerSeat:
		{
			GM_Cmd_DialRoleCanelBankerSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialRoleCanelBankerSeat), sizeof(GM_Cmd_DialRoleCanelBankerSeat));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_DialRoleJoinVipSeat:
		{
			CL_Cmd_DialRoleJoinVipSeat* pMsg = (CL_Cmd_DialRoleJoinVipSeat*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			GM_Cmd_DialRoleJoinVipSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialRoleJoinVipSeat), sizeof(GM_Cmd_DialRoleJoinVipSeat));
			msg.dwUserID = pRole->GetUserID();
			msg.VipSeatIndex = pMsg->VipSeatIndex;
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_DialRoleLeaveVipSeat:
		{
			GM_Cmd_DialRoleLeaveVipSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialRoleLeaveVipSeat), sizeof(GM_Cmd_DialRoleLeaveVipSeat));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_DialGetNormalSeatInfo:
		{
			CL_Cmd_DialGetNormalSeatInfo * pMsg = (CL_Cmd_DialGetNormalSeatInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			GM_Cmd_DialGetNormalSeatInfo msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, GM_DialGetNormalSeatInfo), sizeof(GM_Cmd_DialGetNormalSeatInfo));
			msg.dwUserID = pRole->GetUserID();
			msg.Page = pMsg->Page;
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_DialRoleBetGlobelByLog:
		{
			CL_Cmd_DialRoleBetGlobelByLog* pMsg = (CL_Cmd_DialRoleBetGlobelByLog*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			DWORD AllGlobel = 0;
			for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
				AllGlobel += pMsg->betGlobel[i];
			LC_Cmd_DialRoleBetGlobelByLog msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleBetGlobelByLog), sizeof(LC_Cmd_DialRoleBetGlobelByLog));
			msg.Result = false;
			if (!pRole->ChangeRoleGlobe(AllGlobel*-1, true, false, false))
			{
				pRole->SendDataToClient(&msg);
				return true;
			}
			if (!pRole->GetRoleMonthCard().IsInMonthCard())
			{
				pRole->SendDataToClient(&msg);
				return true;
			}
			GM_Cmd_DialRoleBetGlobelByLog msgSend;
			msgSend.dwUserID = pRole->GetUserID();
			SetMsgInfo(msgSend, GetMsgType(Main_Dial, GM_DialRoleBetGlobelByLog), sizeof(GM_Cmd_DialRoleBetGlobelByLog));
			for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
				msgSend.betGlobel[i] = pMsg->betGlobel[i];
			SendNetCmdToMiniGame(&msgSend);
			return true;
		}
		break;
	}
	return false;
}
bool FishServer::OnHandleTCPNetworkNiuNiu(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_NiuNiu)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_RoleJoinTable:
		{
			GM_Cmd_RoleJoinMiniGame msgJoin;
			SetMsgInfo(msgJoin, GetMsgType(Main_MiniGame, GM_RoleJoinMiniGame), sizeof(GM_Cmd_RoleJoinMiniGame));
			msgJoin.RoleInfo.dwUserID = pRole->GetUserID();
			msgJoin.RoleInfo.dwGlobelSum = pRole->GetRoleInfo().dwGlobeNum;
			msgJoin.RoleInfo.dwMadleSum = pRole->GetRoleInfo().dwMedalNum;
			msgJoin.RoleInfo.dwCurrceySum = pRole->GetRoleInfo().dwCurrencyNum;
			msgJoin.RoleInfo.VipLevel = pRole->GetRoleInfo().VipLevel;
			msgJoin.RoleInfo.FaceID = pRole->GetRoleInfo().dwFaceID;
			msgJoin.RoleInfo.ParticularStates = pRole->GetRoleInfo().ParticularStates;
			msgJoin.RoleInfo.GameID = pRole->GetRoleInfo().GameID;
			TCHARCopy(msgJoin.RoleInfo.NickName, CountArray(msgJoin.RoleInfo.NickName), pRole->GetRoleInfo().NickName, _tcslen(pRole->GetRoleInfo().NickName));
			SendNetCmdToMiniGame(&msgJoin);

			GM_Cmd_RoleJoinTable msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_RoleJoinTable), sizeof(GM_Cmd_RoleJoinTable));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);

			return true;
		}
		break;
	case CL_RoleLeaveTable:
		{
			GM_Cmd_RoleLeaveMiniGame msgLeave;
			SetMsgInfo(msgLeave, GetMsgType(Main_MiniGame, GM_RoleLeaveMiniGame), sizeof(GM_Cmd_RoleLeaveMiniGame));
			msgLeave.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msgLeave);

			DBR_Cmd_TableChange msgDB;//记录玩家进入
			SetMsgInfo(msgDB, DBR_TableChange, sizeof(DBR_Cmd_TableChange));
			msgDB.dwUserID = pRole->GetUserID();
			msgDB.CurrceySum = pRole->GetRoleInfo().dwCurrencyNum;
			msgDB.GlobelSum = pRole->GetRoleInfo().dwGlobeNum;
			msgDB.MedalSum = pRole->GetRoleInfo().dwMedalNum;
			msgDB.JoinOrLeave = false;
			msgDB.LogTime = time(null);
			msgDB.TableTypeID = 250;
			msgDB.TableMonthID = 250;
			g_FishServer.SendNetCmdToSaveDB(&msgDB);
			g_DBLogManager.LogRoleJoinOrLeaveTableInfo(msgDB.dwUserID, msgDB.GlobelSum, msgDB.CurrceySum, msgDB.MedalSum, msgDB.TableTypeID, msgDB.TableMonthID, false, SendLogDB);

			/*GM_Cmd_RoleLeaveTable msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_RoleLeaveTable), sizeof(GM_Cmd_RoleLeaveTable));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);*/
			return true;
		}
		break;
	case CL_RoleBetGlobel:
		{
			CL_Cmd_RoleBetGlobel* pMsg = (CL_Cmd_RoleBetGlobel*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			if (pMsg->AddGlobel > GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.MaxAddGlobel)
			{
				ASSERT(false);
				return false;
			}
			if (pRole->GetRoleInfo().dwGlobeNum < GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.MaxRate * pMsg->AddGlobel)
			{
				ASSERT(false);
				return false;
			}
			if (pMsg->Index >= MAX_NIUNIU_ClientSum)//0-3
			{
				ASSERT(false);
				return false;
			}
			//1.先扣除玩家的金币
			if (!pRole->ChangeRoleGlobe(pMsg->AddGlobel*-1, true))
			{
				ASSERT(false);
				return false;
			}

			GM_Cmd_RoleBetGlobel msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_RoleBetGlobel), sizeof(GM_Cmd_RoleBetGlobel));
			msg.dwUserID = pRole->GetUserID();
			msg.AddGlobel = pMsg->AddGlobel;
			msg.Index = pMsg->Index;
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_GetBankerList:
		{
			GM_Cmd_GetBankerList msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_GetBankerList), sizeof(GM_Cmd_GetBankerList));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_RoleJoinBankerList:
		{
			LC_Cmd_RoleJoinBankerList msgResult;
			SetMsgInfo(msgResult, GetMsgType(Main_NiuNiu, LC_RoleJoinBankerList), sizeof(LC_Cmd_RoleJoinBankerList));
			msgResult.Result = false;

			if (pRole->GetRoleInfo().dwGlobeNum < GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum)
			{
				pRole->SendDataToClient(&msgResult);
				ASSERT(false);
				return false;
			}
			//1.先扣除玩家的金币
			/*if (!pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum*-1, true))
			{
				ASSERT(false);
				return false;
			}*/
			GM_Cmd_RoleJoinBankerList msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_RoleJoinBankerList), sizeof(GM_Cmd_RoleJoinBankerList));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_RoleGetBankerFirstSeat:
		{
			LC_Cmd_RoleGetBankerFirstSeat msgError;
			SetMsgInfo(msgError, GetMsgType(Main_NiuNiu, LC_RoleGetBankerFirstSeat), sizeof(LC_Cmd_RoleGetBankerFirstSeat));
			msgError.Result = false;
			if (!pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetNextBankerNeedGlobel*-1, true))
			{
				pRole->SendDataToClient(&msgError);
				ASSERT(false);
				return false;
			}

			GM_Cmd_RoleGetBankerFirstSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_RoleGetBankerFirstSeat), sizeof(GM_Cmd_RoleGetBankerFirstSeat));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_RoleLeaveBankerList:
		{
			GM_Cmd_RoleLeaveBankerList msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_RoleLeaveBankerList), sizeof(GM_Cmd_RoleLeaveBankerList));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_RoleCanelBankerSeat:
		{
			GM_Cmd_RoleCanelBankerSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_RoleCanelBankerSeat), sizeof(GM_Cmd_RoleCanelBankerSeat));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_RoleJoinVipSeat:
		{
			CL_Cmd_RoleJoinVipSeat* pMsg = (CL_Cmd_RoleJoinVipSeat*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			GM_Cmd_RoleJoinVipSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_RoleJoinVipSeat), sizeof(GM_Cmd_RoleJoinVipSeat));
			msg.dwUserID = pRole->GetUserID();
			msg.VipSeatIndex = pMsg->VipSeatIndex;
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_RoleLeaveVipSeat:
		{
			GM_Cmd_RoleLeaveVipSeat msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_RoleLeaveVipSeat), sizeof(GM_Cmd_RoleLeaveVipSeat));
			msg.dwUserID = pRole->GetUserID();
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	case CL_GetNormalSeatInfo:
		{
			CL_Cmd_GetNormalSeatInfo * pMsg = (CL_Cmd_GetNormalSeatInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			GM_Cmd_GetNormalSeatInfo msg;
			SetMsgInfo(msg, GetMsgType(Main_NiuNiu, GM_GetNormalSeatInfo), sizeof(GM_Cmd_GetNormalSeatInfo));
			msg.dwUserID = pRole->GetUserID();
			msg.Page = pMsg->Page;
			SendNetCmdToMiniGame(&msg);
			return true;
		}
		break;
	}
	return false;
}
bool FishServer::OnHandleTCPNetworkChar(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_Char)
	{
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_LoadAllCharInfo:
		{
			pRole->GetRoleCharManager().OnLoadCharMapList();
			return true;
		}
		break;
	case CL_LoadCharListByUserID:
		{	
			CL_Cmd_LoadCharListByUserID* pMsg = (CL_Cmd_LoadCharListByUserID*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleCharManager().OnLoadAllCharInfoByUserID(pMsg->SrcUserID);
			return true;
		}
		break;
	case CL_SendCharInfo:
		{
			CL_Cmd_SendCharInfo* pMsg = (CL_Cmd_SendCharInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pMsg->MessageInfo.LogTime = time(null);
			pRole->GetRoleCharManager().OnSendCharInfo(pMsg->MessageInfo);
			return true;
		}
		break;
	}
	return true;
}
bool FishServer::OnHandleLoadCharInfo(NetCmd* pCmd)
{
	DBO_Cmd_LoadCharInfo* pMsg = (DBO_Cmd_LoadCharInfo*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleCharManager().OnLoadAllCharInfoByDB(pMsg);
	return true;
}
bool FishServer::OnHandleSocketChar(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_Char)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CG_SendCharInfo:
		{
			CG_Cmd_SendCharInfo* pMsg = (CG_Cmd_SendCharInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->MessageInfo.DestUserID);
			if (!pRole)
			{
				DBR_Cmd_AddCharInfo msg;
				SetMsgInfo(msg, DBR_AddCharInfo, sizeof(DBR_Cmd_AddCharInfo));
				msg.MessageInfo = pMsg->MessageInfo;
				SendNetCmdToSaveDB(&msg);
			}
			else
			{
				pRole->GetRoleCharManager().OnAddCharInfo(pMsg->MessageInfo);
			}
			return true;
		}
		break;
	}
	return false;
}
bool FishServer::HandleMiniGameMsg(NetCmd* pCmd)
{
	//处理MiniGame的 数据 
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType == Main_MiniGame)
	{
		return true;
	}
	else if (pCmd->CmdType == Main_NiuNiu)
	{
		return HandleNiuNiuMsg(pCmd);
	}
	else if (pCmd->CmdType == Main_Dial)
	{
		return HandleDialMsg(pCmd);
	}
	else if (pCmd->CmdType == Main_Car)
	{
		return HandleCarMsg(pCmd);
	}
	return true;
}
bool FishServer::HandleCarMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_Car)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
		{
		case MG_CarRoleJoinTable:
			{
				MG_Cmd_CarRoleJoinTable* pMsg = (MG_Cmd_CarRoleJoinTable*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}

				if (pMsg->Result)
				{
					//玩家进入桌子成功 我们进行处理
					//记录到数据库去 Log数据库 玩家进入牛牛桌子成功了
					//刷新玩家的数据 玩家当前状态在牛牛房间里面
					DBR_Cmd_TableChange msgDB;//记录玩家进入
					SetMsgInfo(msgDB, DBR_TableChange, sizeof(DBR_Cmd_TableChange));
					msgDB.dwUserID = pRole->GetUserID();
					msgDB.CurrceySum = pRole->GetRoleInfo().dwCurrencyNum;
					msgDB.GlobelSum = pRole->GetRoleInfo().dwGlobeNum;
					msgDB.MedalSum = pRole->GetRoleInfo().dwMedalNum;
					msgDB.JoinOrLeave = true;
					msgDB.LogTime = time(null);
					msgDB.TableTypeID = 251;
					msgDB.TableMonthID = 251;
					g_FishServer.SendNetCmdToSaveDB(&msgDB);
					g_DBLogManager.LogRoleJoinOrLeaveTableInfo(msgDB.dwUserID, msgDB.GlobelSum, msgDB.CurrceySum, msgDB.MedalSum, msgDB.TableTypeID, msgDB.TableMonthID, true, SendLogDB);
				}

				LC_Cmd_CarRoleJoinTable msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleJoinTable), sizeof(LC_Cmd_CarRoleJoinTable));
				msg.Result = pMsg->Result;
				msg.TableBankerInfo = pMsg->TableBankerInfo;
				msg.TableBankerUseGameSum = pMsg->TableBankerUseGameSum;
				msg.TableStates = pMsg->TableStates;
				msg.TableStatesUpdateSec = pMsg->TableStatesUpdateSec;
				msg.TableResultIndex = pMsg->TableResultIndex;
				for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
				{
					msg.TableBetAreaGlobel[i] = pMsg->TableBetAreaGlobel[i];
					//msg.TableAreaDataRate[i] = pMsg->TableAreaDataRate[i];
				}
				/*for (BYTE i = 0; i < MAX_CAR_GameSum; ++i)
				{
					msg.TableAreaData[i] = pMsg->TableAreaData[i];
				}*/
				for (BYTE i = 0; i < MAX_CARSHOWBRAND_Sum;++i)
					msg.TableWriteBankerList[i] = pMsg->TableWriteBankerList[i];
				for (BYTE i = 0; i < MAX_CARVIPSEAT_Sum; ++i)
					msg.VipSeatList[i] = pMsg->VipSeatList[i];
				msg.TableResultLog = pMsg->TableResultLog;
				msg.TableGameSum = pMsg->TableGameSum;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarRoleBetGlobel:
			{
				MG_Cmd_CarRoleBetGlobel* pMsg = (MG_Cmd_CarRoleBetGlobel*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					if (!pMsg->Result)
					{
						//退还玩家金币
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("奔驰宝马下注失败 退还下注金币"), _tcslen(TEXT("奔驰宝马下注失败 退还下注金币")));
						MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = pMsg->AddGlobel;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
					return false;
				}

				if (!pMsg->Result)
				{
					pRole->ChangeRoleGlobe(pMsg->AddGlobel, true);//退还玩家金币
				}

				LC_Cmd_CarRoleBetGlobel msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleBetGlobel), sizeof(LC_Cmd_CarRoleBetGlobel));
				msg.AddGlobel = pMsg->AddGlobel;
				msg.Result = pMsg->Result;
				msg.Index = pMsg->Index;
				pRole->SendDataToClient(&msg);

				return true;
			}
			break;
		case MG_CarVipSeatGlobelChange:
			{
				MG_Cmd_CarVipSeatGlobelChange* pMsg = (MG_Cmd_CarVipSeatGlobelChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarVipSeatGlobelChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarVipSeatGlobelChange), sizeof(LC_Cmd_CarVipSeatGlobelChange));
				msg.VipSeat = pMsg->VipSeat;
				msg.GlobelSum = pMsg->GlobelSum;
				msg.Index = pMsg->Index;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarTableStopUpdate:
			{
				MG_Cmd_CarTableStopUpdate* pMsg = (MG_Cmd_CarTableStopUpdate*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarTableStopUpdate msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarTableStopUpdate), sizeof(LC_Cmd_CarTableStopUpdate));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarGetBankerList:
			{
				MG_Cmd_CarGetBankerList* pMsg = (MG_Cmd_CarGetBankerList*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarGetBankerList msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarGetBankerList), sizeof(LC_Cmd_CarGetBankerList));
				msg.dwMySeatIndex = pMsg->dwMySeatIndex;
				for (BYTE i = 0; i < MAX_CARSHOWBRAND_Sum; ++i)
					msg.TableWriteBankerList[i] = pMsg->TableWriteBankerList[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarBankerListChange:
			{
				MG_Cmd_CarBankerListChange* pMsg = (MG_Cmd_CarBankerListChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarBankerListChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarBankerListChange), sizeof(LC_Cmd_CarBankerListChange));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarBankerUserChange:
			{
				MG_Cmd_CarBankerUserChange* pMsg = (MG_Cmd_CarBankerUserChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarBankerUserChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarBankerUserChange), sizeof(LC_Cmd_CarBankerUserChange));
				msg.BankerUserInfo = pMsg->BankerUserInfo;
				msg.GameSum = pMsg->GameSum;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarTableJoinBegin:
			{
				MG_Cmd_CarTableJoinBegin* pMsg = (MG_Cmd_CarTableJoinBegin*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarTableJoinBegin msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarTableJoinBegin), sizeof(LC_Cmd_CarTableJoinBegin));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarTableUpdate:
			{
				MG_Cmd_CarTableUpdate* pMsg = (MG_Cmd_CarTableUpdate*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarTableUpdate msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarTableUpdate), sizeof(LC_Cmd_CarTableUpdate));
				for (BYTE i = 0; i < MAX_CAR_ClientSum;++i)
					msg.TableBetAreaGlobel[i] = pMsg->TableBetAreaGlobel[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarTableJoinEnd:
			{
				MG_Cmd_CarTableJoinEnd* pMsg = (MG_Cmd_CarTableJoinEnd*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					//玩家不在线
					if (pMsg->AddGlobelSum > 0)
					{
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("奔驰宝马结果 因为您不在线 奖励金币通过邮件发送给您"), _tcslen(TEXT("奔驰宝马结果 因为您不在线 奖励金币通过邮件发送给您")));
						MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = static_cast<DWORD>(pMsg->AddGlobelSum);
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
						DBR_Cmd_AddUserMail msg;
						SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToDB(&msg);
						return true;
					}
				}
				else
				{
					

					LC_Cmd_CarTableJoinEnd msg;
					SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarTableJoinEnd), sizeof(LC_Cmd_CarTableJoinEnd));
					msg.AddGlobelSum = pMsg->AddGlobelSum;

					for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
					{
						msg.TableBetAreaGlobel[i] = pMsg->TableBetAreaGlobel[i];
					}
					msg.TableResultIndex = pMsg->TableResultIndex;

					
					msg.BankerUserGlobelInfo = pMsg->BankerUserGlobelInfo;
					for (BYTE i = 0; i < MAX_CARSHOWBRAND_Sum; ++i)
						msg.BankerListGlobelInfo[i] = pMsg->BankerListGlobelInfo[i];
					for (BYTE i = 0; i < MAX_CARVIPSEAT_Sum; ++i)
						msg.VipGlobelInfo[i] = pMsg->VipGlobelInfo[i];
					msg.TableResultLog = pMsg->TableResultLog;
					msg.TableGameSum = pMsg->TableGameSum;
					pRole->SendDataToClient(&msg);

					if (pRole->GetRoleInfo().dwGlobeNum + pMsg->AddGlobelSum < 0)
						pRole->ChangeRoleGlobe(pRole->GetRoleInfo().dwGlobeNum * -1, true);
					else
						pRole->ChangeRoleGlobe(static_cast<DWORD>(pMsg->AddGlobelSum), true);

					return true;
				}
				return true;
			}
			break;
		case MG_CarRoleJoinBankerList:
			{
				MG_Cmd_CarRoleJoinBankerList* pMsg = (MG_Cmd_CarRoleJoinBankerList*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					return false;
				}
				LC_Cmd_CarRoleJoinBankerList msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleJoinBankerList), sizeof(LC_Cmd_CarRoleJoinBankerList));
				msg.Result = pMsg->Result;
				msg.SeatIndex = pMsg->SeatIndex;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarRoleGetBankerFirstSeat:
			{
				MG_Cmd_CarRoleGetBankerFirstSeat* pMsg = (MG_Cmd_CarRoleGetBankerFirstSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					if (!pMsg->Result)
					{
						//抢庄失败
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("奔驰宝马抢庄失败 退还金币"), _tcslen(TEXT("奔驰宝马抢庄失败 退还金币")));
						MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = GetFishConfig().GetFishMiNiGameConfig().carConfig.GetNextBankerNeedGlobel;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
					return false;
				}
				if (!pMsg->Result)
				{
					pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().carConfig.GetNextBankerNeedGlobel, true);
				}
				LC_Cmd_CarRoleGetBankerFirstSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleGetBankerFirstSeat), sizeof(LC_Cmd_CarRoleGetBankerFirstSeat));
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarRoleLeaveBankerList:
			{
				MG_Cmd_CarRoleLeaveBankerList* pMsg = (MG_Cmd_CarRoleLeaveBankerList*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					return false;
				}
				LC_Cmd_CarRoleLeaveBankerList msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleLeaveBankerList), sizeof(LC_Cmd_CarRoleLeaveBankerList));
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarRoleCanelBankerSeat:
			{
				MG_Cmd_CarRoleCanelBankerSeat* pMsg = (MG_Cmd_CarRoleCanelBankerSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarRoleCanelBankerSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleCanelBankerSeat), sizeof(LC_Cmd_CarRoleCanelBankerSeat));
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarBankerUserGlobelChange:
			{
				MG_Cmd_CarBankerUserGlobelChange* pMsg = (MG_Cmd_CarBankerUserGlobelChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarBankerUserGlobelChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarBankerUserGlobelChange), sizeof(LC_Cmd_CarBankerUserGlobelChange));
				msg.dwBankerGlobelSum = pMsg->dwBankerGlobelSum;
				msg.dwBankerUserID = pMsg->dwBankerUserID;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarVipGlobelChange:
			{
				MG_Cmd_CarVipGlobelChange* pMsg = (MG_Cmd_CarVipGlobelChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarVipGlobelChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarVipGlobelChange), sizeof(LC_Cmd_CarVipGlobelChange));
				msg.VipUserID = pMsg->VipUserID;
				msg.VipGlobelSum = pMsg->VipGlobelSum;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarWriteBankerUserGlobelMsg:
			{
				MG_Cmd_CarWriteBankerUserGlobelMsg* pMsg = (MG_Cmd_CarWriteBankerUserGlobelMsg*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarWriteBankerUserGlobelMsg msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarWriteBankerUserGlobelMsg), sizeof(LC_Cmd_CarWriteBankerUserGlobelMsg));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarRoleJoinVipSeat:
			{
				MG_Cmd_CarRoleJoinVipSeat* pMsg = (MG_Cmd_CarRoleJoinVipSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarRoleJoinVipSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleJoinVipSeat), sizeof(LC_Cmd_CarRoleJoinVipSeat));
				msg.DestUserInfo = pMsg->DestUserInfo;
				msg.VipSeatIndex = pMsg->VipSeatIndex;
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarRoleLeaveVipSeat:
			{
				MG_Cmd_CarRoleLeaveVipSeat* pMsg = (MG_Cmd_CarRoleLeaveVipSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarRoleLeaveVipSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleLeaveVipSeat), sizeof(LC_Cmd_CarRoleLeaveVipSeat));
				msg.dwDestUserID = pMsg->dwDestUserID;
				msg.VipSeatIndex = pMsg->VipSeatIndex;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarRoleBeLeaveVipSeat:
			{
				MG_Cmd_CarRoleBeLeaveVipSeat* pMsg = (MG_Cmd_CarRoleBeLeaveVipSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarRoleBeLeaveVipSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleBeLeaveVipSeat), sizeof(LC_Cmd_CarRoleBeLeaveVipSeat));
				msg.DestRoleInfo = pMsg->DestRoleInfo;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarGetNormalSeatInfo:
			{
				MG_Cmd_CarGetNormalSeatInfo * pMsg = (MG_Cmd_CarGetNormalSeatInfo*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarGetNormalSeatInfo msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarGetNormalSeatInfo), sizeof(LC_Cmd_CarGetNormalSeatInfo));
				msg.Page = pMsg->Page;
				msg.TotalRoleSum = pMsg->TotalRoleSum;
				for (BYTE i = 0; i < MAX_CARNORMAL_PAGESUM; ++i)
					msg.Array[i] = pMsg->Array[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_CarRoleBetGlobelByLog:
			{
				MG_Cmd_CarRoleBetGlobelByLog* pMsg = (MG_Cmd_CarRoleBetGlobelByLog*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pMsg->Result)
				{
					if (pRole)
					{
						pRole->ChangeRoleGlobe(pMsg->AllGlobel, true, false, false);
					}
					else
					{
						//归还玩家金币
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("奔驰宝马下注失败 退还金币"), _tcslen(TEXT("奔驰宝马下注失败 退还金币")));
						MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = pMsg->AllGlobel;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
				}
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_CarRoleBetGlobelByLog msg;
				SetMsgInfo(msg, GetMsgType(Main_Car, LC_CarRoleBetGlobelByLog), sizeof(LC_Cmd_CarRoleBetGlobelByLog));
				msg.Result = pMsg->Result;
				for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
					msg.betGlobel[i] = pMsg->betGlobel[i];
				pRole->SendDataToClient(&msg);
				return true;						  
			}
			break;
		}
		return false;
}
bool FishServer::HandleDialMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_Dial)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
		{
		case MG_DialRoleJoinTable:
			{
				MG_Cmd_DialRoleJoinTable* pMsg = (MG_Cmd_DialRoleJoinTable*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}

				if (pMsg->Result)
				{
					//玩家进入桌子成功 我们进行处理
					//记录到数据库去 Log数据库 玩家进入牛牛桌子成功了
					//刷新玩家的数据 玩家当前状态在牛牛房间里面
					DBR_Cmd_TableChange msgDB;//记录玩家进入
					SetMsgInfo(msgDB, DBR_TableChange, sizeof(DBR_Cmd_TableChange));
					msgDB.dwUserID = pRole->GetUserID();
					msgDB.CurrceySum = pRole->GetRoleInfo().dwCurrencyNum;
					msgDB.GlobelSum = pRole->GetRoleInfo().dwGlobeNum;
					msgDB.MedalSum = pRole->GetRoleInfo().dwMedalNum;
					msgDB.JoinOrLeave = true;
					msgDB.LogTime = time(null);
					msgDB.TableTypeID = 251;
					msgDB.TableMonthID = 251;
					g_FishServer.SendNetCmdToSaveDB(&msgDB);
					g_DBLogManager.LogRoleJoinOrLeaveTableInfo(msgDB.dwUserID, msgDB.GlobelSum, msgDB.CurrceySum, msgDB.MedalSum, msgDB.TableTypeID, msgDB.TableMonthID, true, SendLogDB);
				}

				LC_Cmd_DialRoleJoinTable msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleJoinTable), sizeof(LC_Cmd_DialRoleJoinTable));
				msg.Result = pMsg->Result;
				msg.TableBankerInfo = pMsg->TableBankerInfo;
				msg.TableBankerUseGameSum = pMsg->TableBankerUseGameSum;
				msg.TableStates = pMsg->TableStates;
				msg.TableStatesUpdateSec = pMsg->TableStatesUpdateSec;
				msg.TableResultIndex = pMsg->TableResultIndex;
				for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
				{
					msg.TableBetAreaGlobel[i] = pMsg->TableBetAreaGlobel[i];
					msg.TableAreaDataRate[i] = pMsg->TableAreaDataRate[i];
				}
				for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
				{
					msg.TableAreaData[i] = pMsg->TableAreaData[i];
				}
				for (BYTE i = 0; i < MAX_DIALSHOWBRAND_Sum;++i)
					msg.TableWriteBankerList[i] = pMsg->TableWriteBankerList[i];
				for (BYTE i = 0; i < MAX_DIALVIPSEAT_Sum; ++i)
					msg.VipSeatList[i] = pMsg->VipSeatList[i];
				msg.TableResultLog = pMsg->TableResultLog;
				msg.TableGameSum = pMsg->TableGameSum;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialRoleBetGlobel:
			{
				MG_Cmd_DialRoleBetGlobel* pMsg = (MG_Cmd_DialRoleBetGlobel*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					if (!pMsg->Result)
					{
						//退还玩家金币
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("森林舞会下注失败 退还下注金币"), _tcslen(TEXT("森林舞会下注失败 退还下注金币")));
						MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = pMsg->AddGlobel;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
					return false;
				}

				if (!pMsg->Result)
				{
					pRole->ChangeRoleGlobe(pMsg->AddGlobel, true);//退还玩家金币
				}

				LC_Cmd_DialRoleBetGlobel msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleBetGlobel), sizeof(LC_Cmd_DialRoleBetGlobel));
				msg.AddGlobel = pMsg->AddGlobel;
				msg.Result = pMsg->Result;
				msg.Index = pMsg->Index;
				pRole->SendDataToClient(&msg);

				return true;
			}
			break;
		case MG_DialVipSeatGlobelChange:
			{
				MG_Cmd_DialVipSeatGlobelChange* pMsg = (MG_Cmd_DialVipSeatGlobelChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialVipSeatGlobelChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialVipSeatGlobelChange), sizeof(LC_Cmd_DialVipSeatGlobelChange));
				msg.VipSeat = pMsg->VipSeat;
				msg.GlobelSum = pMsg->GlobelSum;
				msg.Index = pMsg->Index;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialTableStopUpdate:
			{
				MG_Cmd_DialTableStopUpdate* pMsg = (MG_Cmd_DialTableStopUpdate*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialTableStopUpdate msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialTableStopUpdate), sizeof(LC_Cmd_DialTableStopUpdate));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialGetBankerList:
			{
				MG_Cmd_DialGetBankerList* pMsg = (MG_Cmd_DialGetBankerList*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialGetBankerList msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialGetBankerList), sizeof(LC_Cmd_DialGetBankerList));
				msg.dwMySeatIndex = pMsg->dwMySeatIndex;
				for (BYTE i = 0; i < MAX_DIALSHOWBRAND_Sum; ++i)
					msg.TableWriteBankerList[i] = pMsg->TableWriteBankerList[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialBankerListChange:
			{
				MG_Cmd_DialBankerListChange* pMsg = (MG_Cmd_DialBankerListChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialBankerListChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialBankerListChange), sizeof(LC_Cmd_DialBankerListChange));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialBankerUserChange:
			{
				MG_Cmd_DialBankerUserChange* pMsg = (MG_Cmd_DialBankerUserChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialBankerUserChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialBankerUserChange), sizeof(LC_Cmd_DialBankerUserChange));
				msg.BankerUserInfo = pMsg->BankerUserInfo;
				msg.GameSum = pMsg->GameSum;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialTableJoinBegin:
			{
				MG_Cmd_DialTableJoinBegin* pMsg = (MG_Cmd_DialTableJoinBegin*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialTableJoinBegin msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialTableJoinBegin), sizeof(LC_Cmd_DialTableJoinBegin));
				for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
					msg.TableAreaData[i] = pMsg->TableAreaData[i];
				for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
					msg.TableAreaDataRate[i] = pMsg->TableAreaDataRate[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialTableUpdate:
			{
				MG_Cmd_DialTableUpdate* pMsg = (MG_Cmd_DialTableUpdate*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialTableUpdate msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialTableUpdate), sizeof(LC_Cmd_DialTableUpdate));
				for (BYTE i = 0; i < MAX_DIAL_ClientSum;++i)
					msg.TableBetAreaGlobel[i] = pMsg->TableBetAreaGlobel[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialTableJoinEnd:
			{
				MG_Cmd_DialTableJoinEnd* pMsg = (MG_Cmd_DialTableJoinEnd*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					//玩家不在线
					if (pMsg->AddGlobelSum > 0)
					{
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("森林舞会结果 因为您不在线 奖励金币通过邮件发送给您"), _tcslen(TEXT("森林舞会结果 因为您不在线 奖励金币通过邮件发送给您")));
						MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = static_cast<DWORD>(pMsg->AddGlobelSum);
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
						DBR_Cmd_AddUserMail msg;
						SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToDB(&msg);
						return true;
					}
				}
				else
				{
					LC_Cmd_DialTableJoinEnd msg;
					SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialTableJoinEnd), sizeof(LC_Cmd_DialTableJoinEnd));
					msg.AddGlobelSum = pMsg->AddGlobelSum;

					for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
					{
						msg.TableBetAreaGlobel[i] = pMsg->TableBetAreaGlobel[i];
					}
					msg.TableResultIndex = pMsg->TableResultIndex;

					
					msg.BankerUserGlobelInfo = pMsg->BankerUserGlobelInfo;
					for (BYTE i = 0; i < MAX_DIALSHOWBRAND_Sum; ++i)
						msg.BankerListGlobelInfo[i] = pMsg->BankerListGlobelInfo[i];
					for (BYTE i = 0; i < MAX_DIALVIPSEAT_Sum; ++i)
						msg.VipGlobelInfo[i] = pMsg->VipGlobelInfo[i];
					msg.TableResultLog = pMsg->TableResultLog;
					msg.TableGameSum = pMsg->TableGameSum;
					pRole->SendDataToClient(&msg);

					if (pRole->GetRoleInfo().dwGlobeNum + pMsg->AddGlobelSum < 0)
						pRole->ChangeRoleGlobe(pRole->GetRoleInfo().dwGlobeNum * -1, true);
					else
						pRole->ChangeRoleGlobe(static_cast<DWORD>(pMsg->AddGlobelSum), true);

					return true;
				}
				return true;
			}
			break;
		case MG_DialRoleJoinBankerList:
			{
				MG_Cmd_DialRoleJoinBankerList* pMsg = (MG_Cmd_DialRoleJoinBankerList*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					return false;
				}
				LC_Cmd_DialRoleJoinBankerList msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleJoinBankerList), sizeof(LC_Cmd_DialRoleJoinBankerList));
				msg.Result = pMsg->Result;
				msg.SeatIndex = pMsg->SeatIndex;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialRoleGetBankerFirstSeat:
			{
				MG_Cmd_DialRoleGetBankerFirstSeat* pMsg = (MG_Cmd_DialRoleGetBankerFirstSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					if (!pMsg->Result)
					{
						//抢庄失败
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("森林舞会抢庄失败 退还金币"), _tcslen(TEXT("森林舞会抢庄失败 退还金币")));
						MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = GetFishConfig().GetFishMiNiGameConfig().dialConfig.GetNextBankerNeedGlobel;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
					return false;
				}
				if (!pMsg->Result)
				{
					pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().dialConfig.GetNextBankerNeedGlobel, true);
				}
				LC_Cmd_DialRoleGetBankerFirstSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleGetBankerFirstSeat), sizeof(LC_Cmd_DialRoleGetBankerFirstSeat));
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialRoleLeaveBankerList:
			{
				MG_Cmd_DialRoleLeaveBankerList* pMsg = (MG_Cmd_DialRoleLeaveBankerList*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					return false;
				}
				LC_Cmd_DialRoleLeaveBankerList msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleLeaveBankerList), sizeof(LC_Cmd_DialRoleLeaveBankerList));
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialRoleCanelBankerSeat:
			{
				MG_Cmd_DialRoleCanelBankerSeat* pMsg = (MG_Cmd_DialRoleCanelBankerSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialRoleCanelBankerSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleCanelBankerSeat), sizeof(LC_Cmd_DialRoleCanelBankerSeat));
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialBankerUserGlobelChange:
			{
				MG_Cmd_DialBankerUserGlobelChange* pMsg = (MG_Cmd_DialBankerUserGlobelChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialBankerUserGlobelChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialBankerUserGlobelChange), sizeof(LC_Cmd_DialBankerUserGlobelChange));
				msg.dwBankerGlobelSum = pMsg->dwBankerGlobelSum;
				msg.dwBankerUserID = pMsg->dwBankerUserID;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialVipGlobelChange:
			{
				MG_Cmd_DialVipGlobelChange* pMsg = (MG_Cmd_DialVipGlobelChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialVipGlobelChange msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialVipGlobelChange), sizeof(LC_Cmd_DialVipGlobelChange));
				msg.VipUserID = pMsg->VipUserID;
				msg.VipGlobelSum = pMsg->VipGlobelSum;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialWriteBankerUserGlobelMsg:
			{
				MG_Cmd_DialWriteBankerUserGlobelMsg* pMsg = (MG_Cmd_DialWriteBankerUserGlobelMsg*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialWriteBankerUserGlobelMsg msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialWriteBankerUserGlobelMsg), sizeof(LC_Cmd_DialWriteBankerUserGlobelMsg));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialRoleJoinVipSeat:
			{
				MG_Cmd_DialRoleJoinVipSeat* pMsg = (MG_Cmd_DialRoleJoinVipSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialRoleJoinVipSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleJoinVipSeat), sizeof(LC_Cmd_DialRoleJoinVipSeat));
				msg.DestUserInfo = pMsg->DestUserInfo;
				msg.VipSeatIndex = pMsg->VipSeatIndex;
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialRoleLeaveVipSeat:
			{
				MG_Cmd_DialRoleLeaveVipSeat* pMsg = (MG_Cmd_DialRoleLeaveVipSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialRoleLeaveVipSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleLeaveVipSeat), sizeof(LC_Cmd_DialRoleLeaveVipSeat));
				msg.dwDestUserID = pMsg->dwDestUserID;
				msg.VipSeatIndex = pMsg->VipSeatIndex;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialRoleBeLeaveVipSeat:
			{
				MG_Cmd_DialRoleBeLeaveVipSeat* pMsg = (MG_Cmd_DialRoleBeLeaveVipSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialRoleBeLeaveVipSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleBeLeaveVipSeat), sizeof(LC_Cmd_DialRoleBeLeaveVipSeat));
				msg.DestRoleInfo = pMsg->DestRoleInfo;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialGetNormalSeatInfo:
			{
				MG_Cmd_DialGetNormalSeatInfo * pMsg = (MG_Cmd_DialGetNormalSeatInfo*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialGetNormalSeatInfo msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialGetNormalSeatInfo), sizeof(LC_Cmd_DialGetNormalSeatInfo));
				msg.Page = pMsg->Page;
				msg.TotalRoleSum = pMsg->TotalRoleSum;
				for (BYTE i = 0; i < MAX_DIALNORMAL_PAGESUM; ++i)
					msg.Array[i] = pMsg->Array[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_DialRoleBetGlobelByLog:
			{
				MG_Cmd_DialRoleBetGlobelByLog* pMsg = (MG_Cmd_DialRoleBetGlobelByLog*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pMsg->Result)
				{
					if (pRole)
					{
						pRole->ChangeRoleGlobe(pMsg->AllGlobel, true, false, false);
					}
					else
					{
						//归还玩家金币
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("森林舞会下注失败 退还金币"), _tcslen(TEXT("森林舞会下注失败 退还金币")));
						MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = pMsg->AllGlobel;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
				}
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_DialRoleBetGlobelByLog msg;
				SetMsgInfo(msg, GetMsgType(Main_Dial, LC_DialRoleBetGlobelByLog), sizeof(LC_Cmd_DialRoleBetGlobelByLog));
				msg.Result = pMsg->Result;
				for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
					msg.betGlobel[i] = pMsg->betGlobel[i];
				pRole->SendDataToClient(&msg);
				return true;						  
			}
			break;
		}
		return false;
}
bool FishServer::HandleNiuNiuMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_NiuNiu)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
		{
		case MG_RoleJoinTable:
			{
				MG_Cmd_RoleJoinTable* pMsg = (MG_Cmd_RoleJoinTable*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}

				if (pMsg->Result)
				{
					//玩家进入桌子成功 我们进行处理
					//记录到数据库去 Log数据库 玩家进入牛牛桌子成功了
					//刷新玩家的数据 玩家当前状态在牛牛房间里面
					DBR_Cmd_TableChange msgDB;//记录玩家进入
					SetMsgInfo(msgDB, DBR_TableChange, sizeof(DBR_Cmd_TableChange));
					msgDB.dwUserID = pRole->GetUserID();
					msgDB.CurrceySum = pRole->GetRoleInfo().dwCurrencyNum;
					msgDB.GlobelSum = pRole->GetRoleInfo().dwGlobeNum;
					msgDB.MedalSum = pRole->GetRoleInfo().dwMedalNum;
					msgDB.JoinOrLeave = true;
					msgDB.LogTime = time(null);
					msgDB.TableTypeID = 250;
					msgDB.TableMonthID = 250;
					g_FishServer.SendNetCmdToSaveDB(&msgDB);
					g_DBLogManager.LogRoleJoinOrLeaveTableInfo(msgDB.dwUserID, msgDB.GlobelSum, msgDB.CurrceySum, msgDB.MedalSum, msgDB.TableTypeID, msgDB.TableMonthID, true, SendLogDB);
				}

				LC_Cmd_RoleJoinTable msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_RoleJoinTable), sizeof(LC_Cmd_RoleJoinTable));
				msg.Result = pMsg->Result;
				msg.TableBankerInfo = pMsg->TableBankerInfo;
				msg.TableBankerUseGameSum = pMsg->TableBankerUseGameSum;
				msg.TableStates = pMsg->TableStates;
				msg.TableStatesUpdateSec = pMsg->TableStatesUpdateSec;
				for (BYTE i = 0; i < MAX_NIUNIU_ClientSum;++i)
					msg.TableBetAreaGlobel[i] = pMsg->TableBetAreaGlobel[i];
				for (BYTE i = 0; i < MAX_NIUNIU_ClientSum + 1; ++i)
				{
					msg.TableBrandResult[i] = pMsg->TableBrandResult[i];
					for (BYTE j = 0; j < MAX_NIUNIU_BrandSum;++j)
						msg.TableBrandArray[i][j] = pMsg->TableBrandArray[i][j];
				}
				for (BYTE i = 0; i < MAX_SHOWBRAND_Sum;++i)
					msg.TableWriteBankerList[i] = pMsg->TableWriteBankerList[i];
				for (BYTE i = 0; i < MAX_VIPSEAT_Sum; ++i)
					msg.VipSeatList[i] = pMsg->VipSeatList[i];
				msg.TableResultLog = pMsg->TableResultLog;
				msg.TableGameSum = pMsg->TableGameSum;

				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_RoleBetGlobel:
			{
				MG_Cmd_RoleBetGlobel* pMsg = (MG_Cmd_RoleBetGlobel*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					if (!pMsg->Result)
					{
						//退还玩家金币
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("下注失败 退还下注金币"), _tcslen(TEXT("下注失败 退还下注金币")));
						MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = pMsg->AddGlobel;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
					return false;
				}

				if (!pMsg->Result)
				{
					pRole->ChangeRoleGlobe(pMsg->AddGlobel, true);//退还玩家金币
				}

				LC_Cmd_RoleBetGlobel msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_RoleBetGlobel), sizeof(LC_Cmd_RoleBetGlobel));
				msg.AddGlobel = pMsg->AddGlobel;
				msg.Result = pMsg->Result;
				msg.Index = pMsg->Index;
				pRole->SendDataToClient(&msg);

				return true;
			}
			break;
		case MG_VipSeatGlobelChange:
			{
				MG_Cmd_VipSeatGlobelChange* pMsg = (MG_Cmd_VipSeatGlobelChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_VipSeatGlobelChange msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_VipSeatGlobelChange), sizeof(LC_Cmd_VipSeatGlobelChange));
				msg.VipSeat = pMsg->VipSeat;
				msg.GlobelSum = pMsg->GlobelSum;
				msg.Index = pMsg->Index;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_TableStopUpdate:
			{
				MG_Cmd_TableStopUpdate* pMsg = (MG_Cmd_TableStopUpdate*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_TableStopUpdate msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_TableStopUpdate), sizeof(LC_Cmd_TableStopUpdate));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_GetBankerList:
			{
				MG_Cmd_GetBankerList* pMsg = (MG_Cmd_GetBankerList*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_GetBankerList msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_GetBankerList), sizeof(LC_Cmd_GetBankerList));
				msg.dwMySeatIndex = pMsg->dwMySeatIndex;
				for (BYTE i = 0; i < MAX_SHOWBRAND_Sum; ++i)
					msg.TableWriteBankerList[i] = pMsg->TableWriteBankerList[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_BankerListChange:
			{
				MG_Cmd_BankerListChange* pMsg = (MG_Cmd_BankerListChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_BankerListChange msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_BankerListChange), sizeof(LC_Cmd_BankerListChange));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_BankerUserChange:
			{
				MG_Cmd_BankerUserChange* pMsg = (MG_Cmd_BankerUserChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				//if (pMsg->BankerUserInfo.dwUserID != 0 && pMsg->BankerUserInfo.dwUserID == pMsg->dwUserID)
				//{
				//	//当前玩家为庄家 归还 押注的金钱
				//	pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum, true);
				//}
				LC_Cmd_BankerUserChange msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_BankerUserChange), sizeof(LC_Cmd_BankerUserChange));
				msg.BankerUserInfo = pMsg->BankerUserInfo;
				msg.GameSum = pMsg->GameSum;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_TableJoinBegin:
			{
				MG_Cmd_TableJoinBegin* pMsg = (MG_Cmd_TableJoinBegin*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_TableJoinBegin msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_TableJoinBegin), sizeof(LC_Cmd_TableJoinBegin));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_TableUpdate:
			{
				MG_Cmd_TableUpdate* pMsg = (MG_Cmd_TableUpdate*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_TableUpdate msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_TableUpdate), sizeof(LC_Cmd_TableUpdate));
				for (BYTE i = 0; i < MAX_NIUNIU_ClientSum;++i)
					msg.TableBetAreaGlobel[i] = pMsg->TableBetAreaGlobel[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_TableJoinEnd:
			{
				MG_Cmd_TableJoinEnd* pMsg = (MG_Cmd_TableJoinEnd*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					//玩家不在线
					if (pMsg->AddGlobelSum > 0)
					{
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 .
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("牛牛结果 因为您不在线 奖励金币通过邮件发送给您"), _tcslen(TEXT("牛牛结果 因为您不在线 奖励金币通过邮件发送给您")));
						MailInfo.RewardID = GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = static_cast<DWORD>(pMsg->AddGlobelSum);
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
						DBR_Cmd_AddUserMail msg;
						SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToDB(&msg);
						return true;
					}
					else
					{
						//直接发送数据库命令 扣除金币 
						DBR_Cmd_ChangeRoleMoney msgDB;
						SetMsgInfo(msgDB, DBR_ChangeRoleMoney, sizeof(DBR_Cmd_ChangeRoleMoney));
						msgDB.dwUserID = pMsg->dwUserID;
						msgDB.GlobelSum = static_cast<DWORD>(pMsg->AddGlobelSum);
						msgDB.CurrceySum = 0;
						msgDB.MedalSum = 0;
						SendNetCmdToSaveDB(&msgDB);

						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 
						_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("因为您牛牛游戏失败 扣除您%d 的金币"), pMsg->AddGlobelSum*-1);
						MailInfo.RewardID = 0;
						MailInfo.RewardSum =0;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
						DBR_Cmd_AddUserMail msg;
						SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToDB(&msg);
						return true;
					}
				}
				else
				{
					

					LC_Cmd_TableJoinEnd msg;
					SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_TableJoinEnd), sizeof(LC_Cmd_TableJoinEnd));
					msg.AddGlobelSum = pMsg->AddGlobelSum;
					for (BYTE i = 0; i < MAX_NIUNIU_ClientSum; ++i)
						msg.TableBetAreaGlobel[i] = pMsg->TableBetAreaGlobel[i];
					for (BYTE i = 0; i < MAX_NIUNIU_ClientSum + 1; ++i)
					{
						msg.TableBrandResult[i] = pMsg->TableBrandResult[i];
						for (BYTE j = 0; j < MAX_NIUNIU_BrandSum; ++j)
							msg.TableBrandArray[i][j] = pMsg->TableBrandArray[i][j];
					}
					msg.BankerUserGlobelInfo = pMsg->BankerUserGlobelInfo;
					for (BYTE i = 0; i < MAX_SHOWBRAND_Sum; ++i)
						msg.BankerListGlobelInfo[i] = pMsg->BankerListGlobelInfo[i];
					for (BYTE i = 0; i < MAX_VIPSEAT_Sum; ++i)
						msg.VipGlobelInfo[i] = pMsg->VipGlobelInfo[i];
					msg.TableResultLog = pMsg->TableResultLog;
					msg.TableGameSum = pMsg->TableGameSum;
					pRole->SendDataToClient(&msg);

					if (pRole->GetRoleInfo().dwGlobeNum + pMsg->AddGlobelSum < 0)
						pRole->ChangeRoleGlobe(pRole->GetRoleInfo().dwGlobeNum * -1, true);
					else
						pRole->ChangeRoleGlobe(static_cast<DWORD>(pMsg->AddGlobelSum), true);

					return true;
				}
				return true;
			}
			break;
		case MG_RoleJoinBankerList:
			{
				MG_Cmd_RoleJoinBankerList* pMsg = (MG_Cmd_RoleJoinBankerList*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					//if (!pMsg->Result)
					//{
					//	tagRoleMail	MailInfo;
					//	MailInfo.bIsRead = false;
					//	TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("上庄失败 退还金币"), _tcslen(TEXT("上庄失败 退还金币")));
					//	MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
					//	MailInfo.RewardSum = GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum;
					//	MailInfo.MailID = 0;
					//	MailInfo.SendTimeLog = time(NULL);
					//	MailInfo.SrcFaceID = 0;
					//	TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
					//	MailInfo.SrcUserID = 0;//系统发送
					//	MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

					//	CC_Cmd_SendSystemMail msg;
					//	SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
					//	msg.dwDestUserID = pMsg->dwUserID;
					//	msg.MailInfo = MailInfo;
					//	g_FishServer.SendNetCmdToCenter(&msg);
					//}
					return false;
				}
				/*if (!pMsg->Result)
				{
					pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum, true);
				}*/
				LC_Cmd_RoleJoinBankerList msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_RoleJoinBankerList), sizeof(LC_Cmd_RoleJoinBankerList));
				msg.Result = pMsg->Result;
				msg.SeatIndex = pMsg->SeatIndex;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_RoleGetBankerFirstSeat:
			{
				MG_Cmd_RoleGetBankerFirstSeat* pMsg = (MG_Cmd_RoleGetBankerFirstSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					if (!pMsg->Result)
					{
						//抢庄失败
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("抢庄失败 退还金币"), _tcslen(TEXT("抢庄失败 退还金币")));
						MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
						MailInfo.RewardSum = GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetNextBankerNeedGlobel;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = pMsg->dwUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
					return false;
				}
				if (!pMsg->Result)
				{
					pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetNextBankerNeedGlobel, true);
				}
				LC_Cmd_RoleGetBankerFirstSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_RoleGetBankerFirstSeat), sizeof(LC_Cmd_RoleGetBankerFirstSeat));
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_RoleLeaveBankerList:
			{
				MG_Cmd_RoleLeaveBankerList* pMsg = (MG_Cmd_RoleLeaveBankerList*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					//if (pMsg->Result)
					//{
					//	tagRoleMail	MailInfo;
					//	MailInfo.bIsRead = false;
					//	TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("离开庄家排队队列 退还金币"), _tcslen(TEXT("离开庄家排队队列 退还金币")));
					//	MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailGlobelRewardID;
					//	MailInfo.RewardSum = GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum;
					//	MailInfo.MailID = 0;
					//	MailInfo.SendTimeLog = time(NULL);
					//	MailInfo.SrcFaceID = 0;
					//	TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
					//	MailInfo.SrcUserID = 0;//系统发送
					//	MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);

					//	CC_Cmd_SendSystemMail msg;
					//	SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
					//	msg.dwDestUserID = pMsg->dwUserID;
					//	msg.MailInfo = MailInfo;
					//	g_FishServer.SendNetCmdToCenter(&msg);
					//}
					return false;
				}
				/*if (pMsg->Result)
				{
					pRole->ChangeRoleGlobe(GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum, true);
				}*/
				LC_Cmd_RoleLeaveBankerList msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_RoleLeaveBankerList), sizeof(LC_Cmd_RoleLeaveBankerList));
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_RoleCanelBankerSeat:
			{
				MG_Cmd_RoleCanelBankerSeat* pMsg = (MG_Cmd_RoleCanelBankerSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_RoleCanelBankerSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_RoleCanelBankerSeat), sizeof(LC_Cmd_RoleCanelBankerSeat));
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_BankerUserGlobelChange:
			{
				MG_Cmd_BankerUserGlobelChange* pMsg = (MG_Cmd_BankerUserGlobelChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_BankerUserGlobelChange msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_BankerUserGlobelChange), sizeof(LC_Cmd_BankerUserGlobelChange));
				msg.dwBankerGlobelSum = pMsg->dwBankerGlobelSum;
				msg.dwBankerUserID = pMsg->dwBankerUserID;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_VipGlobelChange:
			{
				MG_Cmd_VipGlobelChange* pMsg = (MG_Cmd_VipGlobelChange*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_VipGlobelChange msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_VipGlobelChange), sizeof(LC_Cmd_VipGlobelChange));
				msg.VipUserID = pMsg->VipUserID;
				msg.VipGlobelSum = pMsg->VipGlobelSum;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_WriteBankerUserGlobelMsg:
			{
				MG_Cmd_WriteBankerUserGlobelMsg* pMsg = (MG_Cmd_WriteBankerUserGlobelMsg*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_WriteBankerUserGlobelMsg msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_WriteBankerUserGlobelMsg), sizeof(LC_Cmd_WriteBankerUserGlobelMsg));
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_RoleJoinVipSeat:
			{
				MG_Cmd_RoleJoinVipSeat* pMsg = (MG_Cmd_RoleJoinVipSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_RoleJoinVipSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_RoleJoinVipSeat), sizeof(LC_Cmd_RoleJoinVipSeat));
				msg.DestUserInfo = pMsg->DestUserInfo;
				msg.VipSeatIndex = pMsg->VipSeatIndex;
				msg.Result = pMsg->Result;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_RoleLeaveVipSeat:
			{
				MG_Cmd_RoleLeaveVipSeat* pMsg = (MG_Cmd_RoleLeaveVipSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_RoleLeaveVipSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_RoleLeaveVipSeat), sizeof(LC_Cmd_RoleLeaveVipSeat));
				msg.dwDestUserID = pMsg->dwDestUserID;
				msg.VipSeatIndex = pMsg->VipSeatIndex;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_RoleBeLeaveVipSeat:
			{
				MG_Cmd_RoleBeLeaveVipSeat* pMsg = (MG_Cmd_RoleBeLeaveVipSeat*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_RoleBeLeaveVipSeat msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_RoleBeLeaveVipSeat), sizeof(LC_Cmd_RoleBeLeaveVipSeat));
				msg.DestRoleInfo = pMsg->DestRoleInfo;
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		case MG_GetNormalSeatInfo:
			{
				MG_Cmd_GetNormalSeatInfo * pMsg = (MG_Cmd_GetNormalSeatInfo*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
				if (!pRole)
				{
					ASSERT(false);
					return false;
				}
				LC_Cmd_GetNormalSeatInfo msg;
				SetMsgInfo(msg, GetMsgType(Main_NiuNiu, LC_GetNormalSeatInfo), sizeof(LC_Cmd_GetNormalSeatInfo));
				msg.Page = pMsg->Page;
				msg.TotalRoleSum = pMsg->TotalRoleSum;
				for (BYTE i = 0; i < MAX_NORMAL_PAGESUM; ++i)
					msg.Array[i] = pMsg->Array[i];
				pRole->SendDataToClient(&msg);
				return true;
			}
			break;
		}
		return false;
}
void FishServer::UpdateInfoToControl(DWORD dwTimer)
{
	static DWORD LogUpdateCenterTime = 0;
	if (LogUpdateCenterTime == 0 || dwTimer - LogUpdateCenterTime >= 10000)
	{
		LogUpdateCenterTime = dwTimer;

		LC_Cmd_GameInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Control, LC_GameInfo), sizeof(LC_Cmd_GameInfo));
		msg.GameID = m_GameNetworkID;
		msg.OnlinePlayerSum = GetRoleManager()->GetAllRole().size();
		msg.TableSum = GetTableManager()->GetTableSum();
		msg.WriteLogonPlayerSum = m_RoleQueueManager.GetWriteRoleSum();
		SendNetCmdToControl(&msg);
	}
}
string FishServer::GetUserMacAddress(DWORD dwUserID)
{
	return "";
	CRoleEx* pRole = GetRoleManager()->QueryUser(dwUserID);
	if (!pRole)
	{
		return "";
	}
	DWORD SocketID = pRole->GetGameSocketID();
	HashMap<DWORD, string>::iterator Iter = m_UserMacLog.find(SocketID);
	if (Iter == m_UserMacLog.end())
		return "";
	else
		return Iter->second;
}
string FishServer::GetUserIpAddress(DWORD dwUserID)
{
	CRoleEx* pRole = GetRoleManager()->QueryUser(dwUserID);
	if (!pRole)
	{
		return "";
	}
	DWORD SocketID = pRole->GetGameSocketID();
	ServerClientData* pClient = GetUserClientDataByIndex(SocketID);
	if (!pClient)
		return "";
	char IpAddress[32] = "";
	sprintf_s(IpAddress, CountArray(IpAddress), "%u.%u.%u.%u", pClient->IP & 0xff, (pClient->IP >> 8) & 0xff,  (pClient->IP >> 16) & 0xff, (pClient->IP >> 24) & 0xff);
	return IpAddress;
}

bool FishServer::OnHandleTCPNerwordRelationRequest(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	CRoleEx* pRole = GetRoleManager()->QuertUserBySocketID(pClient->OutsideExtraData);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_RelationRequest)
	{
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_LoadRelationRequest:
		{
			pRole->GetRoleRelationRequest().SendInfoToClient();
			return true;
		}
		break;
	case CL_SendRelationRequest:
		{
			CL_Cmd_SendRelationRequest* pMsg = (CL_Cmd_SendRelationRequest*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleRelationRequest().OnSendNewRequest(pMsg->Info.RelationType, pMsg->Info.DestUserID, pMsg->Info.MessageInfo);
			return true;
		}
		break;
	case CL_HandleRelationRequest:
		{
			CL_Cmd_HandleRelationRequest* pMsg = (CL_Cmd_HandleRelationRequest*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleRelationRequest().OnHandleRequest(pMsg->ID, pMsg->Result);
			return true;
		}
		break;
	}
	return false;
}
bool FishServer::OnHandleSocketRelationRequest(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType != Main_RelationRequest)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CG_SendRelationRequest:
		{
			CG_Cmd_SendRelationRequest* pMsg = (CG_Cmd_SendRelationRequest*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->Info.DestUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleRelationRequest().OnBeAddNewrequest(pMsg->Info);
			return true;
		}
		break;
	case CG_HandleRelationRequest:
		{
			CG_Cmd_HandleRelationRequest* pMsg = (CG_Cmd_HandleRelationRequest*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->Info.SrcUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleRelationRequest().OnBeHandleRequest(pMsg->Info,pMsg->Result);
			return true;
		}
		break;
	case CG_DestDelRelation:
		{
			CG_Cmd_DestDelRelation* pMsg = (CG_Cmd_DestDelRelation*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->Info.DestUserID);
			if (!pRole)
			{
				ASSERT(false);
				return false;
			}
			pRole->GetRoleRelationRequest().OnDelDestRelation(pMsg->Info);
			return true;
		}
		break;
	}
	return false;
}
bool FishServer::OnHandleDataBaseLoadRelationRequest(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	DBO_Cmd_LoadRelationRequest* pMsg = (DBO_Cmd_LoadRelationRequest*)pCmd;
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleRelationRequest().OnLoadRelationRequestResult(pMsg);
	return true;
}
bool FishServer::OnHandleDataBaseAddRelationRequest(NetCmd* pCmd)
{
	if(!pCmd)
	{
		ASSERT(false);
		return false;
	}
	DBO_Cmd_AddRelationRequest* pMsg = (DBO_Cmd_AddRelationRequest*)pCmd;
	CRoleEx* pRole = GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	pRole->GetRoleRelationRequest().OnSendNewRequestDBResult(pMsg);
	return true;
}