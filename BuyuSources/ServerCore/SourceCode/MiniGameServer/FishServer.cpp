#include "stdafx.h"
#include "FishServer.h"
#include "..\CommonFile\FishServerConfig.h"
void SendDB(NetCmd* pCmd)
{
	g_FishServer.SendNetCmdToDB(pCmd);
}
void SendLogDB(NetCmd* pCmd)
{
	g_FishServer.SendNetCmdToLogDB(pCmd);
}

FishServer::FishServer()
{
	m_IsClose = false;
	m_DBIsConnect = false;
	m_IsReloadConfig = false;
	m_LogDBIsConnect = false;
	m_ControlIsConnect = false;
	m_MiniGameServerID = 0;
}
FishServer::~FishServer()
{

}
bool FishServer::InitServer()
{
	m_pDump.OnInit();//开启记录dump
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

	MiniGameServerConfig* pConfig = g_FishServerConfig.GetMiniGameServerConfig();
	if (!pConfig)
	{
		ASSERT(false);
		return false;
	}

	m_MiniGameServerID = pConfig->NetworkID;

	if (!m_NiuNiuManager.OnInit())
	{
		ASSERT(false);
		return false;
	}

	if (!m_DialManager.OnInit())
	{
		ASSERT(false);
		return false;
	}
	if (!m_CarManager.OnInit())
	{
		ASSERT(false);
		return false;
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
	if (!ConnectLogDB())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectLogDB())
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
	if (!ConnectControl())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectControl())
				break;
		}
	}
	m_RobotManager.OnLoadRobotInfoBySum(pConfig->BeginUserID, pConfig->EndUserID, SendDB);
	return true;
}
void FishServer::OnServerIsFinish()
{
	if (!ConnectClient())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectClient())
				break;
		}
	}
	ShowInfoToWin("控制器服务器启动成功");
}
bool FishServer::ConnectDB()
{
	MiniGameServerConfig* pControlServerConfig = g_FishServerConfig.GetMiniGameServerConfig();
	if (!pControlServerConfig)
	{
		ASSERT(false);
		return false;
	}
	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pControlServerConfig->DBNetworkID);
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
	if (!m_DBTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_MiniGameServerID, sizeof(BYTE)))
	{
		ShowInfoToWin("数据库连接失败");
		return false;
	}
	OnTcpClientConnect(&m_DBTcp);
	return true;
}
bool FishServer::ConnectLogDB()
{
	MiniGameServerConfig* pControlServerConfig = g_FishServerConfig.GetMiniGameServerConfig();
	if (!pControlServerConfig)
	{
		ASSERT(false);
		return false;
	}
	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pControlServerConfig->LogDBNetworkID);
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
	if (!m_LogDBTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_MiniGameServerID, sizeof(BYTE)))
	{
		ShowInfoToWin("Log数据库连接失败");
		return false;
	}
	OnTcpClientConnect(&m_LogDBTcp);
	return true;
}
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
	pCenterData.SleepTime = pCenterConfig->SleepTime;
	pCenterData.SocketRecvSize = pCenterConfig->RecvBuffSize;
	pCenterData.SocketSendSize = pCenterConfig->SendBuffSize;
	pCenterData.ThreadNum = 1;
	pCenterData.Timeout = pCenterConfig->TimeOut;
	pCenterData.CmdHearbeat = 0;
	pCenterData.SendArraySize = pCenterConfig->MaxSendCmdCount;
	pCenterData.RecvArraySize = pCenterConfig->MaxRecvCmdCount;
	if (!m_CenterTcp.Init(pCenterConfig->CenterListenIP, pCenterData, (void*)&m_MiniGameServerID, sizeof(BYTE)))
	{
		ShowInfoToWin("中央服务器连接失败");
		return false;
	}
	OnTcpClientConnect(&m_CenterTcp);
	return true;
}
bool FishServer::ConnectControl()
{
	//连接到控制服务器
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

	if (!m_ControlTcp.Init(pConfig->ControlServerListenIP, pControlData, (void*)&m_MiniGameServerID, sizeof(BYTE)))
	{
		ShowInfoToWin("ControlServer连接失败");
		return false;
	}
	OnTcpClientConnect(&m_ControlTcp);
	return true;
}
bool FishServer::ConnectClient()
{
	MiniGameServerConfig* pConfig = g_FishServerConfig.GetMiniGameServerConfig();
	if (!pConfig)
	{
		ASSERT(false);
		return false;
	}
	ServerInitData pCLientData;
	pCLientData.ServerID = pConfig->NetworkID;
	pCLientData.BuffSize = pConfig->BuffSize;
	pCLientData.CmdHearbeat = 0;
	pCLientData.ListenCount = pConfig->ListenCount;
	pCLientData.MaxRecvCmdCount = pConfig->MaxRecvCmdCount;
	pCLientData.MaxSendCountPerFrame = pConfig->MaxSendCountPerFrame;
	pCLientData.Port = pConfig->GameListenPort;
	pCLientData.SocketRecvSize = pConfig->RecvBuffSize;
	pCLientData.SocketSendSize = pConfig->SendBuffSize;
	pCLientData.Timeout = pConfig->TimeOut;
	pCLientData.RecvThreadNum = pConfig->RecvThreadSum;
	pCLientData.SendThreadNum = pConfig->SendThreadSum;
	pCLientData.SleepTime = pConfig->SleepTime;
	pCLientData.AcceptSleepTime = pConfig->AcceptSleepTime;
	pCLientData.MaxAcceptNumPerFrame = pConfig->MaxAcceptNumPerFrame;
	pCLientData.SceneHearbeatTick = pConfig->SceneHearbeatTick;
	pCLientData.AcceptRecvData = true;
	pCLientData.SceneHearbeatTick = pConfig->SceneHearbeatTick;
	pCLientData.MaxSendCmdCount = pConfig->MaxSendCmdCount;
	m_ClientTcp.SetCmdHandler(this);
	if (!m_ClientTcp.Init(pCLientData))
	{
		ShowInfoToWin("对内网络库启动失败");
		return false;
	}

	return true;
}
void FishServer::OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//往网络库中有一个客户端离开的时候
	if (ServerID == m_MiniGameServerID)
	{
		//ShowInfoToWin("一个内部服务器离开了控制器 ID:%d",ServerID);
	}
	else
	{
		ASSERT(false);
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
	if (ServerID == m_MiniGameServerID)
	{
		//std::cout << "一个Client加入了CentralServer\n";
		//ShowInfoToWin("一个内部服务器加入控制器 ID:%d",ServerID);
	}
	else
	{
		ASSERT(false);
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
	if (pClient == &m_DBTcp)
	{
		m_DBIsConnect = true;
		ShowInfoToWin("已经与DB连接成功了");
	}
	else if (pClient == &m_LogDBTcp)
	{
		m_LogDBIsConnect = true;
		ShowInfoToWin("已经与Log数据库连接成功了");
	}
	else if (pClient == &m_ControlTcp)
	{
		m_ControlIsConnect = true;
		ShowInfoToWin("已经与ControlServer连接成功了");
	}
	else if (pClient == &m_CenterTcp)
	{
		m_CenterIsConnect = true;
		ShowInfoToWin("已经与CenterServer连接成功了");
	}
	else
	{
		ASSERT(false);
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
		ShowInfoToWin("已经与DB断开连接了");
		m_DBIsConnect = false;
		m_DBTcp.Shutdown();
	}
	else if (pClient == &m_LogDBTcp)
	{
		ShowInfoToWin("已经与Log数据库断开连接了");
		m_LogDBIsConnect = false;
		m_LogDBTcp.Shutdown();
	}
	else if (pClient == &m_ControlTcp)
	{
		m_ControlIsConnect = false;
		ShowInfoToWin("已经与ControlServer断开连接了");
		m_ControlTcp.Shutdown();
	}
	else if (pClient == &m_CenterTcp)
	{
		m_CenterIsConnect = false;
		m_CenterTcp.Shutdown();
		ShowInfoToWin("已经与CenterServer断开连接了");
	}
	else
	{
		ASSERT(false);
	}
	return;
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
bool FishServer::MainUpdate()
{
	//主循环处理
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

		m_NiuNiuManager.OnUpdate(dwTimer);//更新牛牛服务器
		m_DialManager.OnUpdate(dwTimer);
		m_CarManager.OnUpdate(dwTimer);

		//Center服务器的每一帧处理的命令  不可能一帧将全部的命令处理了
		//1.处理GameServer 的命令
		if (!m_ClientList.empty())
		{
			HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClientList.begin();
			for (; Iter != m_ClientList.end();)
			{
				if (!Iter->second)
				{
					ASSERT(false);
					continue;
				}
				if (Iter->second->Removed)
				{
					OnTcpServerLeave(m_MiniGameServerID, Iter->second);
					m_ClientTcp.Kick(Iter->second);
					Iter = m_ClientList.erase(Iter);
					continue;
				}
				int Sum = 0;
				while (Iter->second->RecvList.HasItem() && Sum < Msg_OnceSum)//处理命令的数量
				{
					NetCmd* pCmd = Iter->second->RecvList.GetItem();
					if (!pCmd)
					{
						ASSERT(false);
						continue;
					}
					//处理网络命令 客户端发送来的登陆 注册 等命令
					HandleClientMsg(Iter->second, pCmd);
					free(pCmd);
					++Sum;
				}
				++Iter;
			}
		}
		//2.处理DB的命令
		if (m_DBTcp.IsConnected())
		{
			if (!m_DBIsConnect)
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
			if (m_DBIsConnect)
				OnTcpClientLeave(&m_DBTcp);

			//重新连接DB
			ConnectDB();
		}
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
		//3.Control
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
		//LogDB
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
	//处理外部发送来的处理命令
	if (!pClient || !pCmd)
	{
		ASSERT(false); 
		return;
	}
	if (pCmd->CmdType == Main_MiniGame)
	{
		switch (pCmd->SubCmdType)
		{
		case GM_RoleJoinMiniGame:
			{
				GM_Cmd_RoleJoinMiniGame* pMsg = (GM_Cmd_RoleJoinMiniGame*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				m_RoleManager.OnRsgRole(&pMsg->RoleInfo,static_cast<BYTE>(pClient->OutsideExtraData));
				return;
			}
			break;
		case GM_RoleLeaveMiniGame:
			{
				GM_Cmd_RoleLeaveMiniGame* pMsg = (GM_Cmd_RoleLeaveMiniGame*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				m_RoleManager.OnUnRsgRole(pMsg->dwUserID);
				return;
			}
			break;
		case GM_RoleChangeNickName:
			{
				GM_Cmd_RoleChangeNickName* pMsg = (GM_Cmd_RoleChangeNickName*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				Role* pRole = m_RoleManager.QueryRole(pMsg->dwUserID);
				if (!pRole)
				{
					//ASSERT(false);
					return;
				}
				pRole->OnChangeRoleNickName(pMsg->NickName);
				return;
			}
			break;
		case GM_RoleChangeFaceID:
			{
				GM_Cmd_RoleChangeFaceID* pMsg = (GM_Cmd_RoleChangeFaceID*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				Role* pRole = m_RoleManager.QueryRole(pMsg->dwUserID);
				if (!pRole)
				{
					//ASSERT(false);
					return;
				}
				pRole->OnChangeRoleFaceID(pMsg->FaceID);
				return;
			}
			break;
		case GM_RoleChangeGlobel:
			{
				GM_Cmd_RoleChangeGlobel* pMsg = (GM_Cmd_RoleChangeGlobel*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				Role* pRole = m_RoleManager.QueryRole(pMsg->dwUserID);
				if (!pRole)
				{
					//ASSERT(false);
					return;
				}
				pRole->OnChangeRoleGlobelSum(pMsg->dwGlobel,true);
				return;
			}
			break;
		case GM_RoleChangeMadel:
			{
				GM_Cmd_RoleChangeMadel* pMsg = (GM_Cmd_RoleChangeMadel*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				Role* pRole = m_RoleManager.QueryRole(pMsg->dwUserID);
				if (!pRole)
				{
					//ASSERT(false);
					return;
				}
				pRole->OnChangeRoleMadleSum(pMsg->dwMadel);
				return;
			}
			break;
		case GM_RoleChangeCurrcey:
			{
				GM_Cmd_RoleChangeCurrcey* pMsg = (GM_Cmd_RoleChangeCurrcey*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				Role* pRole = m_RoleManager.QueryRole(pMsg->dwUserID);
				if (!pRole)
				{
					//ASSERT(false);
					return;
				}
				pRole->OnChangeRoleCurrceySum(pMsg->dwCurrcey);
				return;
			}
			break;
		case GM_RoleChangeVipLevel:
			{
				GM_Cmd_RoleChangeVipLevel* pMsg = (GM_Cmd_RoleChangeVipLevel*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				Role* pRole = m_RoleManager.QueryRole(pMsg->dwUserID);
				if (!pRole)
				{
					//ASSERT(false);
					return;
				}
				pRole->OnChangeRoleVipLevel(pMsg->VipLevel);
				return;
			}
			break;
		case GM_ChangeRoleParticularStates:
			{
				GM_Cmd_ChangeRoleParticularStates* pMsg = (GM_Cmd_ChangeRoleParticularStates*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return;
				}
				Role* pRole = m_RoleManager.QueryRole(pMsg->dwUserID);
				if (!pRole)
				{
					//ASSERT(false);
					return;
				}
				pRole->OnChangeRoleParticularStates(pMsg->ParticularStates);
				return;
			}
			break;
		}
	}
	else if (pCmd->CmdType == Main_NiuNiu)
	{
		HandleNiuNiuMsg(pCmd);
		return;
	}
	else if (pCmd->CmdType == Main_Dial)
	{
		HandleDialMsg(pCmd);
		return;
	}
	else if (pCmd->CmdType == Main_Car)
	{
		HandleCarMsg(pCmd);
		return;
	}
	return;
}
void FishServer::HandleCarMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (pCmd->CmdType != Main_Car)
	{
		ASSERT(false);
		return;
	}
	switch (pCmd->SubCmdType)
		{
			case GM_CarRoleJoinTable:
				{
					GM_Cmd_CarRoleJoinTable* pMsg = (GM_Cmd_CarRoleJoinTable*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleJoinCarTable(pMsg->dwUserID);
					return;
				}
				break;
			case GM_CarRoleLeaveTable:
				{
					GM_Cmd_CarRoleLeaveTable* pMsg = (GM_Cmd_CarRoleLeaveTable*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleLeaveCarTable(pMsg->dwUserID);
					return;
				}
				break;
			case GM_CarRoleBetGlobel:
				{
					GM_Cmd_CarRoleBetGlobel* pMsg = (GM_Cmd_CarRoleBetGlobel*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleBetGlobel(pMsg->dwUserID, pMsg->Index, pMsg->AddGlobel);
					return;
				}
				break;
			case GM_CarGetBankerList:
				{
					GM_Cmd_CarGetBankerList* pMsg = (GM_Cmd_CarGetBankerList*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleGetWriteBankerList(pMsg->dwUserID);
					return;
				}
				break;
			case GM_CarRoleJoinBankerList:
				{
					GM_Cmd_CarRoleJoinBankerList* pMsg = (GM_Cmd_CarRoleJoinBankerList*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleJoinWriteBankerList(pMsg->dwUserID);
					return;
				}
				break;
			case GM_CarRoleGetBankerFirstSeat:
				{
					GM_Cmd_CarRoleGetBankerFirstSeat* pMsg = (GM_Cmd_CarRoleGetBankerFirstSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleGetWriteBankerFirstSeat(pMsg->dwUserID);
					return;
				}
				break;
			case GM_CarRoleLeaveBankerList:
				{
					GM_Cmd_CarRoleLeaveBankerList* pMsg = (GM_Cmd_CarRoleLeaveBankerList*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleLeaveWriteBankerList(pMsg->dwUserID);
					return;
				}
				break;
			case GM_CarRoleCanelBankerSeat:
				{
					GM_Cmd_CarRoleCanelBankerSeat* pMsg = (GM_Cmd_CarRoleCanelBankerSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleCanelBanker(pMsg->dwUserID);
					return;
				}
				break;
			case GM_CarRoleJoinVipSeat:
				{
					GM_Cmd_CarRoleJoinVipSeat* pMsg = (GM_Cmd_CarRoleJoinVipSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleJoinVipSeat(pMsg->dwUserID,pMsg->VipSeatIndex);
					return;
				}
				break;
			case GM_CarRoleLeaveVipSeat:
				{
					GM_Cmd_CarRoleLeaveVipSeat* pMsg = (GM_Cmd_CarRoleLeaveVipSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleLeaveVipSeat(pMsg->dwUserID);
					return;
				}
				break;
			case GM_CarGetNormalSeatInfo:
				{
					GM_Cmd_CarGetNormalSeatInfo* pMsg = (GM_Cmd_CarGetNormalSeatInfo*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleGetNormalInfoByPage(pMsg->dwUserID, pMsg->Page);
					return;
				}
				break;
			case GM_CarRoleBetGlobelByLog:
				{
					GM_Cmd_CarRoleBetGlobelByLog* pMsg = (GM_Cmd_CarRoleBetGlobelByLog*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_CarManager.OnRoleBetGlobelByLog(pMsg->dwUserID, pMsg->betGlobel);
					return;
				}
				break;
		}
}
void FishServer::HandleDialMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (pCmd->CmdType != Main_Dial)
	{
		ASSERT(false);
		return;
	}
	switch (pCmd->SubCmdType)
		{
			case GM_DialRoleJoinTable:
				{
					GM_Cmd_DialRoleJoinTable* pMsg = (GM_Cmd_DialRoleJoinTable*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleJoinDialTable(pMsg->dwUserID);
					return;
				}
				break;
			case GM_DialRoleLeaveTable:
				{
					GM_Cmd_DialRoleLeaveTable* pMsg = (GM_Cmd_DialRoleLeaveTable*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleLeaveDialTable(pMsg->dwUserID);
					return;
				}
				break;
			case GM_DialRoleBetGlobel:
				{
					GM_Cmd_DialRoleBetGlobel* pMsg = (GM_Cmd_DialRoleBetGlobel*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleBetGlobel(pMsg->dwUserID, pMsg->Index, pMsg->AddGlobel);
					return;
				}
				break;
			case GM_DialGetBankerList:
				{
					GM_Cmd_DialGetBankerList* pMsg = (GM_Cmd_DialGetBankerList*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleGetWriteBankerList(pMsg->dwUserID);
					return;
				}
				break;
			case GM_DialRoleJoinBankerList:
				{
					GM_Cmd_DialRoleJoinBankerList* pMsg = (GM_Cmd_DialRoleJoinBankerList*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleJoinWriteBankerList(pMsg->dwUserID);
					return;
				}
				break;
			case GM_DialRoleGetBankerFirstSeat:
				{
					GM_Cmd_DialRoleGetBankerFirstSeat* pMsg = (GM_Cmd_DialRoleGetBankerFirstSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleGetWriteBankerFirstSeat(pMsg->dwUserID);
					return;
				}
				break;
			case GM_DialRoleLeaveBankerList:
				{
					GM_Cmd_DialRoleLeaveBankerList* pMsg = (GM_Cmd_DialRoleLeaveBankerList*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleLeaveWriteBankerList(pMsg->dwUserID);
					return;
				}
				break;
			case GM_DialRoleCanelBankerSeat:
				{
					GM_Cmd_DialRoleCanelBankerSeat* pMsg = (GM_Cmd_DialRoleCanelBankerSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleCanelBanker(pMsg->dwUserID);
					return;
				}
				break;
			case GM_DialRoleJoinVipSeat:
				{
					GM_Cmd_DialRoleJoinVipSeat* pMsg = (GM_Cmd_DialRoleJoinVipSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleJoinVipSeat(pMsg->dwUserID,pMsg->VipSeatIndex);
					return;
				}
				break;
			case GM_DialRoleLeaveVipSeat:
				{
					GM_Cmd_DialRoleLeaveVipSeat* pMsg = (GM_Cmd_DialRoleLeaveVipSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleLeaveVipSeat(pMsg->dwUserID);
					return;
				}
				break;
			case GM_DialGetNormalSeatInfo:
				{
					GM_Cmd_DialGetNormalSeatInfo* pMsg = (GM_Cmd_DialGetNormalSeatInfo*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleGetNormalInfoByPage(pMsg->dwUserID, pMsg->Page);
					return;
				}
				break;
			case GM_DialRoleBetGlobelByLog:
				{
					GM_Cmd_DialRoleBetGlobelByLog* pMsg = (GM_Cmd_DialRoleBetGlobelByLog*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_DialManager.OnRoleBetGlobelByLog(pMsg->dwUserID, pMsg->betGlobel);
					return;
				}
				break;
		}
}
void FishServer::HandleNiuNiuMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (pCmd->CmdType != Main_NiuNiu)
	{
		ASSERT(false);
		return;
	}
	switch (pCmd->SubCmdType)
		{
			case GM_RoleJoinTable:
				{
					GM_Cmd_RoleJoinTable* pMsg = (GM_Cmd_RoleJoinTable*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleJoinNiuNiuTable(pMsg->dwUserID);
					return;
				}
				break;
			case GM_RoleLeaveTable:
				{
					GM_Cmd_RoleLeaveTable* pMsg = (GM_Cmd_RoleLeaveTable*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleLeaveNiuNiuTable(pMsg->dwUserID);
					return;
				}
				break;
			case GM_RoleBetGlobel:
				{
					GM_Cmd_RoleBetGlobel* pMsg = (GM_Cmd_RoleBetGlobel*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleBetGlobel(pMsg->dwUserID, pMsg->Index, pMsg->AddGlobel);
					return;
				}
				break;
			case GM_GetBankerList:
				{
					GM_Cmd_GetBankerList* pMsg = (GM_Cmd_GetBankerList*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleGetWriteBankerList(pMsg->dwUserID);
					return;
				}
				break;
			case GM_RoleJoinBankerList:
				{
					GM_Cmd_RoleJoinBankerList* pMsg = (GM_Cmd_RoleJoinBankerList*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleJoinWriteBankerList(pMsg->dwUserID);
					return;
				}
				break;
			case GM_RoleGetBankerFirstSeat:
				{
					GM_Cmd_RoleGetBankerFirstSeat* pMsg = (GM_Cmd_RoleGetBankerFirstSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleGetWriteBankerFirstSeat(pMsg->dwUserID);
					return;
				}
				break;
			case GM_RoleLeaveBankerList:
				{
					GM_Cmd_RoleLeaveBankerList* pMsg = (GM_Cmd_RoleLeaveBankerList*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleLeaveWriteBankerList(pMsg->dwUserID);
					return;
				}
				break;
			case GM_RoleCanelBankerSeat:
				{
					GM_Cmd_RoleCanelBankerSeat* pMsg = (GM_Cmd_RoleCanelBankerSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleCanelBanker(pMsg->dwUserID);
					return;
				}
				break;
			case GM_RoleJoinVipSeat:
				{
					GM_Cmd_RoleJoinVipSeat* pMsg = (GM_Cmd_RoleJoinVipSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleJoinVipSeat(pMsg->dwUserID,pMsg->VipSeatIndex);
					return;
				}
				break;
			case GM_RoleLeaveVipSeat:
				{
					GM_Cmd_RoleLeaveVipSeat* pMsg = (GM_Cmd_RoleLeaveVipSeat*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleLeaveVipSeat(pMsg->dwUserID);
					return;
				}
				break;
			case GM_GetNormalSeatInfo:
				{
					GM_Cmd_GetNormalSeatInfo* pMsg = (GM_Cmd_GetNormalSeatInfo*)pCmd;
					if (!pMsg)
					{
						ASSERT(false);
						return;
					}
					m_NiuNiuManager.OnRoleGetNormalInfoByPage(pMsg->dwUserID, pMsg->Page);
					return;
				}
				break;
		}
}
void FishServer::HandleDataBaseMsg(NetCmd* pCmd)
{
	//处理数据库发送来的命令
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	switch (pCmd->GetCmdType())
	{
		case DBO_LoadRobotInfo:
			{
				DBO_Cmd_LoadRobotInfo* pMsg = (DBO_Cmd_LoadRobotInfo*)pCmd;
				m_RobotManager.OnLoadRobotInfoResult(pMsg);
				if (pMsg->States & MsgEnd)
				{
					vector<RobotUserInfo*>& pVec = m_RobotManager.GetAllNonRobotVec();
					vector<RobotUserInfo*>::iterator Iter = pVec.begin();
					for (; Iter != pVec.end(); ++Iter)
					{
						tagMiniGameRoleInfo pInfo;
						pInfo.dwUserID = (*Iter)->dwUserID;
						pInfo.dwCurrceySum = (*Iter)->dwCurrencyNum;
						pInfo.dwGlobelSum = (*Iter)->dwGlobeNum;
						pInfo.dwMadleSum = (*Iter)->dwMedalNum;
						pInfo.FaceID = (*Iter)->dwFaceID;
						pInfo.GameConfigID = 0;
						pInfo.VipLevel = (*Iter)->VipLevel;
						TCHARCopy(pInfo.NickName, CountArray(pInfo.NickName), (*Iter)->NickName, _tcslen((*Iter)->NickName));
						m_RoleManager.OnRsgRole(&pInfo, 0);
					}
					OnServerIsFinish();
				}
				return;
			}
			break;
	}
	return;
}
bool FishServer::OnDestroy()//当控制服务器关闭的时候
{
	HandleAllMsg();
	while (m_AfxAddClient.HasItem())
	{
		AfxNetworkClientOnce* pOnce = m_AfxAddClient.GetItem();
		delete pOnce;
	}
	return true;
}
void FishServer::HandleAllMsg()
{
	//将全部命令全部处理掉 
	if (!m_ClientList.empty())
	{
		HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClientList.begin();
		for (; Iter != m_ClientList.end();)
		{
			if (!Iter->second)
				continue;
			while (Iter->second->RecvList.HasItem())
			{
				NetCmd* pCmd = Iter->second->RecvList.GetItem();
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

	pCmd = m_CenterTcp.GetCmd();
	while (pCmd)
	{
		HandleCenterMsg(pCmd);
		free(pCmd);
		pCmd = m_CenterTcp.GetCmd();
	}

	pCmd = m_ControlTcp.GetCmd();
	while (pCmd)
	{
		HandleControlMsg(pCmd);
		free(pCmd);
		pCmd = m_ControlTcp.GetCmd();
	}
}
uint FishServer::CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize)
{
	//中央服务器只有在FTP 和 DB都连接成功后才工作
	return CONNECT_CHECK_OK;
}
bool FishServer::NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize)
{
	if (!pClient)
	{
		ASSERT(false);
		return false;
	}
	if (!pData || SeverID != m_MiniGameServerID || recvSize != sizeof(BYTE))
	{
		m_ClientTcp.Kick(pClient);
		return false;
	}
	AfxNetworkClientOnce* pOnce = new AfxNetworkClientOnce();
	if (!pOnce)
	{
		m_ClientTcp.Kick(pClient);
		ASSERT(false);
		return false;
	}
	pOnce->SeverID = SeverID;
	pOnce->pClient = pClient;
	if (recvSize > 0)
	{
		pOnce->pPoint = (void*)malloc(recvSize);
		if (!pOnce->pPoint)
		{
			m_ClientTcp.Kick(pClient);
			delete pOnce;
			ASSERT(false);
			return false;
		}
		memcpy_s(pOnce->pPoint, recvSize, pData, recvSize);
	}
	pOnce->Length = recvSize;
	m_AfxAddClient.AddItem(pOnce);
	return true;
}
void FishServer::Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt)
{
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
						  ShowInfoToWin("客户端 ID:%d 离开原因为普通", pClient->OutsideExtraData);
	}
		break;
	case REMOVE_RECV_ERROR:
	{
							  ShowInfoToWin("客户端 ID:%d 离开原因为接收错误", pClient->OutsideExtraData);
	}
		break;
	case REMOVE_SEND_ERROR:
	{
							  ShowInfoToWin("客户端 ID:%d 离开原因为发送错误", pClient->OutsideExtraData);
	}
		break;
	case REMOVE_TIMEOUT:
	{
						   ShowInfoToWin("客户端 ID:%d 离开原因为超时", pClient->OutsideExtraData);
	}
		break;
	case REMOVE_CMD_SEND_OVERFLOW:
	{
									 ShowInfoToWin("客户端 ID:%d 离开原因为发送太多", pClient->OutsideExtraData);
	}
		break;
	case REMOVE_CMD_RECV_OVERFLOW:
	{
									 ShowInfoToWin("客户端 ID:%d 离开原因为接收太多", pClient->OutsideExtraData);
	}
		break;
	case REMOVE_CMD_SIZE_ERROR:
	{
								  ShowInfoToWin("客户端 ID:%d 离开原因为命令大小", pClient->OutsideExtraData);
	}
		break;
	case REMOVE_RECVBACK_NOT_SPACE:
	{
									  ShowInfoToWin("客户端 ID:%d 离开原因为接收缓冲区溢出", pClient->OutsideExtraData);
	}
		break;
	}
}
void FishServer::OnAddClient()
{
	while (m_AfxAddClient.HasItem())
	{
		AfxNetworkClientOnce* pOnce = m_AfxAddClient.GetItem();
		if (!pOnce)
		{
			continue;
		}
		if (pOnce->SeverID == m_MiniGameServerID)
		{
			if (!pOnce->pPoint || pOnce->Length != sizeof(BYTE))
			{
				m_ClientTcp.Kick(pOnce->pClient);
				//ASSERT(false);
				if (pOnce->pPoint)
					free(pOnce->pPoint);
				delete pOnce;
				continue;
			}
			BYTE ClientID = *((BYTE*)pOnce->pPoint);
			if (ClientID == 0)
			{
				m_ClientTcp.Kick(pOnce->pClient);
				//ASSERT(false);
				if (pOnce->pPoint)
					free(pOnce->pPoint);
				delete pOnce;
				continue;
			}
			ShowInfoToWin("内部客户端 ID:%d 加入", ClientID);

			pOnce->pClient->OutsideExtraData = ClientID;
			HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClientList.find(ClientID);
			if (Iter != m_ClientList.end())
			{
				ASSERT(false);
				m_ServerTcp.Kick(pOnce->pClient);
				if (pOnce->pPoint)
					free(pOnce->pPoint);
				delete pOnce;
				continue;
			}
			m_ClientList.insert(HashMap<BYTE, ServerClientData*>::value_type(ClientID, pOnce->pClient));
			//触发玩家加入的事件
			OnTcpServerJoin(pOnce->SeverID, pOnce->pClient);
			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
			continue;
		}
		else
		{
			delete pOnce;
			//ASSERT(false);
			continue;
		}
	}
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
void FishServer::SendNetCmdToClient(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd || !pClient)
	{
		ASSERT(false);
		return;
	}
	if (!m_ClientTcp.Send(pClient, pCmd))
	{
		if (pClient->OutsideExtraData != 0)
			ASSERT(false);
	}
}
void FishServer::SendNetCmdToClient(BYTE GameID, NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClientList.find(GameID);
	if (Iter == m_ClientList.end())
	{
		if (GameID != 0)
			ASSERT(false);
		return;
	}
	if (!m_ClientTcp.Send(Iter->second, pCmd))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToAllClient(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClientList.begin();
	for (; Iter != m_ClientList.end(); ++Iter)
	{
		if (!m_ClientTcp.Send(Iter->second, pCmd))
		{
			ASSERT(false);
		}
	}
}
void FishServer::SendNetCmdToControl(NetCmd* pCmd)
{
	//发送命令到控制端去
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_ControlTcp.Send(pCmd, true))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToCenter(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_CenterTcp.Send(pCmd, true))
	{
		ASSERT(false);
	}
}
void  FishServer::SendNetCmdToDB(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_DBTcp.Send(pCmd, true))
	{
		ASSERT(false);
	}
}
void  FishServer::SendNetCmdToLogDB(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_LogDBTcp.Send(pCmd, true))
	{
		ASSERT(false);
	}
}
void FishServer::HandleControlMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	switch (pCmd->CmdType)
	{
	case Main_Control:
	{
						 switch (pCmd->SubCmdType)
						 {
						 case CL_OxAdmin:
						 {
											m_Oxcontrol.Control(pCmd);
											break;
						 }
						 case CL_AnimalAdmin:
						 {
												m_AninalControl.Control(pCmd);
												break;
						 }
						 case CL_CarAdmin:
						 {
											 m_CarControl.Control(pCmd);
											 break;
						 }

						 }

						 break;
	}

	}

}
void FishServer::HandleCenterMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	

}
void FishServer::UpdateInfoToControl(DWORD dwTimer)
{
	static DWORD LogUpdateCenterTime = 0;
	if (LogUpdateCenterTime == 0 || dwTimer - LogUpdateCenterTime >= 10000)
	{
		LogUpdateCenterTime = dwTimer;
		LC_Cmd_MiNiGameInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Control, LC_MiNiGameInfo), sizeof(LC_Cmd_MiNiGameInfo));
		msg.MiniGameID = m_MiniGameServerID;
		msg.NowNiuNiuSystemGlobel = m_NiuNiuManager.GetTableSystemGlobel();
		SendNetCmdToControl(&msg);
	}
}
FishServer g_FishServer;