#include "stdafx.h"
#include "FishServer.h"
#include "..\CommonFile\FishServerConfig.h"
#include<algorithm>
FishServer::FishServer()
{
	m_IsClose = false;
	m_DBIsConnect = false;
	m_IsReloadConfig = false;
	m_ControlServerNetworkID = 0;
	m_ControlClientIndex = 0;
	m_ControlRankValue = 0;
	m_LogDBIsConnect = false;
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

	ControlServerConfig* pConfig = g_FishServerConfig.GetControlServerConfig();
	if (!pConfig)
	{
		ASSERT(false);
		return false;
	}
	m_ControlServerNetworkID = pConfig->ControlServerNetworkID;
	m_ControlClientNetworkID = pConfig->ControlClientNetworkID;
	m_ControlID = pConfig->ControlID;
	m_ControlRankValue = pConfig->ControlClientPassword;
	if (!ConnectControlServer())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectControlServer())
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
	if (!ConnectLogDB())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectLogDB())
				break;
		}
	}
	if (!ConnectControlClient())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectControlClient())
				break;
		}
	}
	ShowInfoToWin("控制器服务器启动成功");
	return true;
}
bool FishServer::ConnectDB()
{
	ControlServerConfig* pControlServerConfig = g_FishServerConfig.GetControlServerConfig();
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
	if (!m_DBTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_ControlID, sizeof(BYTE)))
	{
		ShowInfoToWin("数据库连接失败");
		return false;
	}
	OnTcpClientConnect(&m_DBTcp);
	return true;
}
bool FishServer::ConnectLogDB()
{
	ControlServerConfig* pControlServerConfig = g_FishServerConfig.GetControlServerConfig();
	if (!pControlServerConfig)
	{
		ASSERT(false);
		return false;
	}
	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pControlServerConfig->LogDBID);
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
	if (!m_LogDBTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_ControlID, sizeof(BYTE)))
	{
		ShowInfoToWin("Log数据库连接失败");
		return false;
	}
	OnTcpClientConnect(&m_LogDBTcp);
	return true;
}
bool FishServer::ConnectControlClient()
{
	ControlServerConfig* pControlServerConfig = g_FishServerConfig.GetControlServerConfig();
	if (!pControlServerConfig)
	{
		ASSERT(false);
		return false;
	}
	ServerInitData pCLientData;
	pCLientData.ServerID = pControlServerConfig->ControlClientNetworkID;
	pCLientData.BuffSize = pControlServerConfig->ControlClientBuffSize;
	pCLientData.CmdHearbeat = 0;
	pCLientData.ListenCount = pControlServerConfig->ControlClientListenCount;
	pCLientData.MaxRecvCmdCount = pControlServerConfig->ControlClientMaxRecvCmdCount;
	pCLientData.MaxSendCountPerFrame = pControlServerConfig->ControlClientMaxSendCountPerFrame;
	pCLientData.Port = pControlServerConfig->ControlClientListenPort;
	pCLientData.SocketRecvSize = pControlServerConfig->ControlClientRecvBuffSize;
	pCLientData.SocketSendSize = pControlServerConfig->ControlClientSendBuffSize;
	pCLientData.Timeout = pControlServerConfig->ControlClientTimeOut;
	pCLientData.RecvThreadNum = pControlServerConfig->ControlClientRecvThreadSum;
	pCLientData.SendThreadNum = pControlServerConfig->ControlClientSendThreadSum;
	pCLientData.SleepTime = pControlServerConfig->ControlClientSleepTime;
	pCLientData.AcceptSleepTime = pControlServerConfig->ControlClientAcceptSleepTime;
	pCLientData.MaxAcceptNumPerFrame = pControlServerConfig->ControlClientMaxAcceptNumPerFrame;
	pCLientData.SceneHearbeatTick = pControlServerConfig->ControlClientSceneHearbeatTick;
	pCLientData.AcceptRecvData = false;
	pCLientData.SceneHearbeatTick = pControlServerConfig->ControlClientSceneHearbeatTick;
	pCLientData.MaxSendCmdCount = pControlServerConfig->ControlClientMaxSendCmdCount;
	m_ClientTcp.SetCmdHandler(this);
	if (!m_ClientTcp.Init(pCLientData))
	{
		ShowInfoToWin("对内网络库启动失败");
		return false;
	}

	return true;
}
bool FishServer::ConnectControlServer()
{
	//连接对内的网络
	ControlServerConfig* pControlServerConfig = g_FishServerConfig.GetControlServerConfig();
	if (!pControlServerConfig)
	{
		ASSERT(false);
		return false;
	}
	ServerInitData pCLientData;
	pCLientData.ServerID = pControlServerConfig->ControlServerNetworkID;
	pCLientData.BuffSize = pControlServerConfig->ControlServerBuffSize;
	pCLientData.CmdHearbeat = 0;
	pCLientData.ListenCount = pControlServerConfig->ControlServerListenCount;
	pCLientData.MaxRecvCmdCount = pControlServerConfig->ControlServerMaxRecvCmdCount;
	pCLientData.MaxSendCountPerFrame = pControlServerConfig->ControlServerMaxSendCountPerFrame;
	pCLientData.Port = pControlServerConfig->ControlServerListenPort;
	pCLientData.SocketRecvSize = pControlServerConfig->ControlServerRecvBuffSize;
	pCLientData.SocketSendSize = pControlServerConfig->ControlServerSendBuffSize;
	pCLientData.Timeout = pControlServerConfig->ControlServerTimeOut;
	pCLientData.RecvThreadNum = pControlServerConfig->ControlServerRecvThreadSum;
	pCLientData.SendThreadNum = pControlServerConfig->ControlServerSendThreadSum;
	pCLientData.SleepTime = pControlServerConfig->ControlServerSleepTime;
	pCLientData.AcceptSleepTime = pControlServerConfig->ControlServerAcceptSleepTime;
	pCLientData.MaxAcceptNumPerFrame = pControlServerConfig->ControlServerMaxAcceptNumPerFrame;
	pCLientData.SceneHearbeatTick = pControlServerConfig->ControlServerSceneHearbeatTick;
	pCLientData.AcceptRecvData = true;
	pCLientData.SceneHearbeatTick = pControlServerConfig->ControlServerSceneHearbeatTick;
	pCLientData.MaxSendCmdCount = pControlServerConfig->ControlServerMaxSendCmdCount;
	m_ServerTcp.SetCmdHandler(this);
	if (!m_ServerTcp.Init(pCLientData))
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
	if (ServerID == m_ControlServerNetworkID)
	{
		//ShowInfoToWin("一个内部服务器离开了控制器 ID:%d",ServerID);
	}
	else if (ServerID == m_ControlClientNetworkID)
	{
		//一个控制器离开了
		//ShowInfoToWin("一个外部客户端离开了控制器");
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
	if (ServerID == m_ControlServerNetworkID)
	{
		//std::cout << "一个Client加入了CentralServer\n";
		//ShowInfoToWin("一个内部服务器加入控制器 ID:%d",ServerID);
	}
	else if (ServerID == m_ControlClientNetworkID)
	{
		//一个管理器加入了
		//ShowInfoToWin("一个外部服务器加入控制器");
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

		//Center服务器的每一帧处理的命令  不可能一帧将全部的命令处理了
		//1.处理GameServer 的命令
		if (!m_ControlClientList.empty())
		{
			HashMap<DWORD, ServerClientData*>::iterator Iter = m_ControlClientList.begin();
			for (; Iter != m_ControlClientList.end();)
			{
				if (!Iter->second)
				{
					ASSERT(false);
					continue;
				}
				if (Iter->second->Removed)
				{
					OnTcpServerLeave(m_ControlClientNetworkID, Iter->second);
					m_ClientTcp.Kick(Iter->second);
					Iter = m_ControlClientList.erase(Iter);
					continue;
				}
				int Sum = 0;
				while (Iter->second->RecvList.HasItem() && Sum < Client_Msg_OnceSum)//处理命令的数量
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
		if (!m_ControlServerList.empty())
		{
			HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.begin();
			for (; Iter != m_ControlServerList.end();)
			{
				if (!Iter->second)
				{
					ASSERT(false);
					continue;
				}
				if (Iter->second->Removed)
				{
					OnTcpServerLeave(m_ControlServerNetworkID, Iter->second);
					m_ServerTcp.Kick(Iter->second);
					Iter = m_ControlServerList.erase(Iter);
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
					if (Iter->first == g_FishServer.GetFishConfig().GetGMToolConfig().NetID_)
					{
						HandleGMToolMsg(Iter->second, pCmd);
					}
					else
					{
						HandleServerMsg(Iter->second, pCmd);
					}
					
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
void FishServer::SendNetCmdToLogDB(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	m_LogDBTcp.Send(pCmd, false);
}



void FishServer::HandleClientMsg(ServerClientData* pClient, NetCmd* pCmd)
{
	//处理外部发送来的处理命令
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return;
	}
	if (pCmd->CmdType != Main_Control)
	{
		ASSERT(false);
		return;
	}
	if (pCmd->SubCmdType == CL_CheckClientInfo)//先验证
	{
		CL_Cmd_CheckClientInfo* pMsg = (CL_Cmd_CheckClientInfo*)pCmd;
		if (!pMsg)
		{
			ASSERT(false);
			return;
		}
		bool bvaliddmac = false;
		tinyxml2::XMLDocument xml_doc;
		if (xml_doc.LoadFile("maclist.xml") == XML_SUCCESS)
		{
			XMLElement* xml_root = xml_doc.FirstChildElement("maclist");
			if (xml_root)
			{
				for (const XMLElement* xml_child = xml_root->FirstChildElement(); xml_child; xml_child = xml_child->NextSiblingElement())
				{
					const char* pItem = xml_child->Attribute("mac");
					if (pItem&&strcmp(pMsg->MachineCode, pItem) == 0)
					{
						bvaliddmac = true;
						break;
					}
				}
			}
		}

		LC_Cmd_CheckClientInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Control, LC_CheckClientInfo), sizeof(LC_Cmd_CheckClientInfo));
		msg.Result = (pMsg->RankValue == m_ControlRankValue&&bvaliddmac);//登陆成功了 可以进入
		SendNetCmdToClient(pClient, &msg);
		if (!msg.Result)
		{
			pClient->Removed = true;
		}
		else
		{
			pClient->dwChecked = 1;
		}
		return;
	}
	if (pClient->dwChecked == 0)
	{
		return;
	}

	switch (pCmd->SubCmdType)
	{
		
	case CL_SendMsgToAllGame:
	{
								CL_Cmd_SendMsgToAllGame* pMsg = (CL_Cmd_SendMsgToAllGame*)pCmd;
								if (!pMsg)
								{
									ASSERT(false);
									return;
								}
								SendNetCmdToCenter(pMsg);
								return;
	}
		break;
	case CL_SendSystemEmail:
	{
							   CL_Cmd_SendSystemEmail* pMsg = (CL_Cmd_SendSystemEmail*)pCmd;
							   if (!pMsg)
							   {
								   ASSERT(false);
								   return;
							   }
							   SendNetCmdToCenter(pMsg);
							   return;
	}
		break;
	case CL_ChangeaNickName:
	{
							   CL_Cmd_ChangeaNickName* pMsg = (CL_Cmd_ChangeaNickName*)pCmd;
							   if (!pMsg)
							   {
								   ASSERT(false);
								   return;
							   }
							   SendNetCmdToCenter(pMsg);
	}
		break;
	
	case CL_QueryOnlineUserInfo:
	{
								   CL_Cmd_QueryOnlineUserInfo* pMsg = (CL_Cmd_QueryOnlineUserInfo*)pCmd;
								   if (!pMsg)
								   {
									   ASSERT(false);
									   return;
								   }
								   pMsg->ClientID = pClient->OutsideExtraData;//记录下客户端ID
								   SendNetCmdToCenter(pMsg);
	}
		break;
	case CL_KickUserByID:
	{
							CL_Cmd_KickUserByID* pMsg = (CL_Cmd_KickUserByID*)pCmd;
							pMsg->ClientID = pClient->OutsideExtraData;//记录下客户端ID
							SendNetCmdToCenter(pMsg);
							//SendNetCmdToAllGame(pMsg);
							return;
	}
		break;
	case CL_ReloadConfig:
	{
							OnReloadConfig();
							SendNetCmdToAllServer(pCmd);
							return;
	}
		break;
	case CL_HandleEntityItem:
	{
								CL_Cmd_HandleEntityItem* pMsg = (CL_Cmd_HandleEntityItem*)pCmd;
								pMsg->ClientID = pClient->OutsideExtraData;
								pMsg->ClientIP = pClient->IP;
								SendNetCmdToCenter(pMsg);
								return;
	}
		break;
	case CL_ResetRolePassword:
	{
								 CL_Cmd_ResetRolePassword* pMsg = (CL_Cmd_ResetRolePassword*)pCmd;

								 DBR_Cmd_ResetUserPassword msg;
								 SetMsgInfo(msg, DBR_ResetUserPassword, sizeof(DBR_Cmd_ResetUserPassword));
								 msg.dwUserID = pMsg->dwUserID;
								 msg.Password1 = pMsg->Password1;
								 msg.Password2 = pMsg->Password2;
								 msg.Password3 = pMsg->Password3;
								 SendNetCmdToControlDB(&msg);

								 SendNetCmdToAllLogon(pMsg);
								 return;
	}
		break;
	case CL_FreezeAccount:
	{
							 CL_Cmd_FreezeAccount*pMsg = (CL_Cmd_FreezeAccount*)pCmd;

							 DBR_Cmd_FreezeUser msg;
							 SetMsgInfo(msg, DBR_FreezeUser, sizeof(DBR_Cmd_FreezeUser));
							 msg.dwUserID = pMsg->dwUserID;
							 msg.FreezeMin = pMsg->FreezeMin;
							 SendNetCmdToControlDB(&msg);

							 SendNetCmdToCenter(pMsg);
							 SendNetCmdToAllLogon(pMsg);
							 return;
	}
	case CL_ChangeParticularStates:
	{
									  CL_Cmd_ChangeParticularStates*pMsg = (CL_Cmd_ChangeParticularStates*)pCmd;
									  pMsg->ClientID = pClient->OutsideExtraData;//记录下客户端ID
									  SendNetCmdToCenter(pMsg);
									  return;
	}
	case CL_QueryFishPool:
	{
							 if (pCmd->GetCmdSize() == sizeof(CL_CMD_QueryFishPool))
							 {
								 ((CL_CMD_QueryFishPool*)pCmd)->ClientID = pClient->OutsideExtraData;
								 SendNetCmdToAllGame(pCmd);
							 }
							 return;
	}
	case CL_QueryBlackList:
	{

							  if (pCmd->GetCmdSize() == sizeof(LC_CMD_QueryFishBlackList))
							  {
								  ((LC_CMD_QueryFishBlackList*)pCmd)->ClientID = pClient->OutsideExtraData;
								  SendNetCmdToAllGame(pCmd);
							  }
							  return;
	}
	case CL_SetBlackList:
	{
							if (pCmd->GetCmdSize() > sizeof(LC_CMD_SetFishBlackList))
							{
								((LC_CMD_SetFishBlackList*)pCmd)->ClientID = pClient->OutsideExtraData;
								SendNetCmdToAllGame(pCmd);
							}
							return;
	}
	case CL_OxAdmin:
	{
					   if (pCmd->GetCmdSize() == sizeof(CL_CMD_OxAdminReq))
					   {
						   ((CL_CMD_OxAdminReq*)pCmd)->ClientID = pClient->OutsideExtraData;
						   SendNetCmdToMiniGame(pCmd);
					   }
					   break;
	}
	case CL_AnimalAdmin:
	{
						   if (pCmd->GetCmdSize() == sizeof(CL_CMD_AnimalAdminReq))
						   {
							   ((CL_CMD_AnimalAdminReq*)pCmd)->ClientID = pClient->OutsideExtraData;
							   SendNetCmdToMiniGame(pCmd);
						   }
						   break;
	}
	case CL_CarAdmin:
	{
						if (pCmd->GetCmdSize() == sizeof(CL_CMD_CarAdminReq))
						{
							((CL_CMD_CarAdminReq*)pCmd)->ClientID = pClient->OutsideExtraData;
							SendNetCmdToMiniGame(pCmd);
						}
						break;
	}

	}
	return;
}


void FishServer::HandleGMToolMsg(ServerClientData* pClient, NetCmd* pCmd)
{
	//处理外部发送来的处理命令
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return;
	}
	if (pCmd->CmdType != Main_Control)
	{
		ASSERT(false);
		return;
	}

	if (pCmd->SubCmdType == GM_CL_CHECK_PASSWORD_REQ)
	{
		bool check = false;
		GM_CL_Cmd_CheckPassWordReq* pMsg = (GM_CL_Cmd_CheckPassWordReq*)pCmd;
		if (strcmp(g_FishServer.GetFishConfig().GetGMToolConfig().Account_.c_str(), pMsg->Account) && 
			strcmp(g_FishServer.GetFishConfig().GetGMToolConfig().PassWord_.c_str(), pMsg->PassWord))
		{

			check = true;
		}
		if (!check)
		{
			pClient->Removed = true;
		}
		else
		{
			pClient->dwChecked = 1;
		}
		CL_GM_Cmd_CheckPassWordACK msg;
		SetMsgInfo(msg, GetMsgType(Main_Control, CL_GM_CHECK_PASSWORD_ACK), sizeof(CL_GM_Cmd_CheckPassWordACK));
		msg.ret = check;
		SendNetCmdToClient(pClient, &msg);
		
	}
	if (pCmd->SubCmdType == CL_CheckClientInfo)//先验证
	{
		CL_Cmd_CheckClientInfo* pMsg = (CL_Cmd_CheckClientInfo*)pCmd;
		if (!pMsg)
		{
			ASSERT(false);
			return;
		}
		bool bvaliddmac = false;
		tinyxml2::XMLDocument xml_doc;
		if (xml_doc.LoadFile("maclist.xml") == XML_SUCCESS)
		{
			XMLElement* xml_root = xml_doc.FirstChildElement("maclist");
			if (xml_root)
			{
				for (const XMLElement* xml_child = xml_root->FirstChildElement(); xml_child; xml_child = xml_child->NextSiblingElement())
				{
					const char* pItem = xml_child->Attribute("mac");
					if (pItem&&strcmp(pMsg->MachineCode, pItem) == 0)
					{
						bvaliddmac = true;
						break;
					}
				}
			}
		}

		LC_Cmd_CheckClientInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Control, LC_CheckClientInfo), sizeof(LC_Cmd_CheckClientInfo));
		msg.Result = (pMsg->RankValue == m_ControlRankValue&&bvaliddmac);//登陆成功了 可以进入
		SendNetCmdToClient(pClient, &msg);
		if (!msg.Result)
		{
			pClient->Removed = true;
		}
		else
		{
			pClient->dwChecked = 1;
		}
		return;
	}
	if (pClient->dwChecked == 0)
	{
		return;
	}

	switch (pCmd->SubCmdType)
	{
	case GM_CL_QUERY_USER_INFO:
	{
		GM_CL_QueryUserInfoReq* pMsg = (GM_CL_QueryUserInfoReq*)pCmd;
		if (!pMsg)
		{
			ASSERT(false);
			return;
		}
		SendNetCmdToCenter(pMsg);
	}
	break;
	case GM_ADD_NEW_OPERATOR_MAIL:
	{
		SendNetCmdToCenter(pCmd);
	}
	break;
	case GM_CL_REWARD_CONFIGS_REQ:
	{
		tagRewardMap& rewardConfig = m_FishConfig.GetFishRewardConfig();
		
		HashMap<WORD, tagRewardOnce>::iterator it = rewardConfig.RewardMap.begin();
		Cl_GM_RewardConfigACK msg;
		SetMsgInfo(msg, GetMsgType(Main_Control, CL_GM_REWARD_CONFIGS_ACK), sizeof(Cl_GM_RewardConfigACK));

		msg.count = 0;
		msg.end = false;
		for (; it != rewardConfig.RewardMap.end(); ++ it)
		{
			if (msg.count >= MAX_REWARDS)
			{
				SendNetCmdToClient(pClient, &msg);
				msg.count = 0;
			}
			const tagRewardOnce& entry = it->second;
			msg.rewards[msg.count].RewardID = entry.RewardID;

			vector<tagItemOnce>::const_iterator it_item = entry.RewardItemVec.begin();
			msg.rewards[msg.count].ItemCount = 0;
			for (; it_item != it->second.RewardItemVec.end(); ++ it_item)
			{
				msg.rewards[msg.count].item[msg.rewards[msg.count].ItemCount] = (*it_item);
				msg.rewards[msg.count].ItemCount++;
				if (msg.rewards[msg.count].ItemCount >= MAX_GM_ITEMS)
				{
					break;
				}
			}
			msg.count++;
		}
		msg.end = true;
		SendNetCmdToClient(pClient, &msg);
	}
	break;
		//case CL_CheckClientInfo:
		//	{
		//		CL_Cmd_CheckClientInfo* pMsg = (CL_Cmd_CheckClientInfo*)pCmd;
		//		if (!pMsg)
		//		{
		//			ASSERT(false);
		//			return;
		//		}
		//		LC_Cmd_CheckClientInfo msg;
		//		SetMsgInfo(msg, GetMsgType(Main_Control, LC_CheckClientInfo), sizeof(LC_Cmd_CheckClientInfo));
		//		msg.Result = (pMsg->RankValue == m_ControlRankValue);//登陆成功了 可以进入
		//		SendNetCmdToClient(pClient, &msg);
		//		if (!msg.Result)
		//		{
		//			pClient->Removed = true;
		//		}
		//		return;
		//	}
	case CL_SendMsgToAllGame:
	{
		CL_Cmd_SendMsgToAllGame* pMsg = (CL_Cmd_SendMsgToAllGame*)pCmd;
		if (!pMsg)
		{
			ASSERT(false);
			return;
		}
		SendNetCmdToCenter(pMsg);
		return;
	}
	break;
	case CL_SendSystemEmail:
	{
		CL_Cmd_SendSystemEmail* pMsg = (CL_Cmd_SendSystemEmail*)pCmd;
		if (!pMsg)
		{
			ASSERT(false);
			return;
		}
		SendNetCmdToCenter(pMsg);
		return;
	}
	break;
	case CL_ChangeaNickName:
	{
		CL_Cmd_ChangeaNickName* pMsg = (CL_Cmd_ChangeaNickName*)pCmd;
		if (!pMsg)
		{
			ASSERT(false);
			return;
		}
		SendNetCmdToCenter(pMsg);
	}
	break;
	case CL_QueryOnlineUserInfo:
	{
		CL_Cmd_QueryOnlineUserInfo* pMsg = (CL_Cmd_QueryOnlineUserInfo*)pCmd;
		if (!pMsg)
		{
			ASSERT(false);
			return;
		}
		pMsg->ClientID = pClient->OutsideExtraData;//记录下客户端ID
		SendNetCmdToCenter(pMsg);
	}
	break;
	case CL_KickUserByID:
	{
		CL_Cmd_KickUserByID* pMsg = (CL_Cmd_KickUserByID*)pCmd;
		pMsg->ClientID = pClient->OutsideExtraData;//记录下客户端ID
		SendNetCmdToCenter(pMsg);
		//SendNetCmdToAllGame(pMsg);
		return;
	}
	break;
	case CL_ReloadConfig:
	{
		OnReloadConfig();
		SendNetCmdToAllServer(pCmd);
		return;
	}
	break;
	case CL_HandleEntityItem:
	{
		CL_Cmd_HandleEntityItem* pMsg = (CL_Cmd_HandleEntityItem*)pCmd;
		pMsg->ClientID = pClient->OutsideExtraData;
		pMsg->ClientIP = pClient->IP;
		SendNetCmdToCenter(pMsg);
		return;
	}
	break;
	case CL_ResetRolePassword:
	{
		CL_Cmd_ResetRolePassword* pMsg = (CL_Cmd_ResetRolePassword*)pCmd;

		DBR_Cmd_ResetUserPassword msg;
		SetMsgInfo(msg, DBR_ResetUserPassword, sizeof(DBR_Cmd_ResetUserPassword));
		msg.dwUserID = pMsg->dwUserID;
		msg.Password1 = pMsg->Password1;
		msg.Password2 = pMsg->Password2;
		msg.Password3 = pMsg->Password3;
		SendNetCmdToControlDB(&msg);

		SendNetCmdToAllLogon(pMsg);
		return;
	}
	break;
	case CL_FreezeAccount:
	{
		CL_Cmd_FreezeAccount*pMsg = (CL_Cmd_FreezeAccount*)pCmd;

		DBR_Cmd_FreezeUser msg;
		SetMsgInfo(msg, DBR_FreezeUser, sizeof(DBR_Cmd_FreezeUser));
		msg.dwUserID = pMsg->dwUserID;
		msg.FreezeMin = pMsg->FreezeMin;
		SendNetCmdToControlDB(&msg);

		SendNetCmdToCenter(pMsg);
		SendNetCmdToAllLogon(pMsg);
		return;
	}
	case CL_ChangeParticularStates:
	{
		CL_Cmd_ChangeParticularStates*pMsg = (CL_Cmd_ChangeParticularStates*)pCmd;
		pMsg->ClientID = pClient->OutsideExtraData;//记录下客户端ID
		SendNetCmdToCenter(pMsg);
		return;
	}
	case CL_QueryFishPool:
	{
		if (pCmd->GetCmdSize() == sizeof(CL_CMD_QueryFishPool))
		{
			((CL_CMD_QueryFishPool*)pCmd)->ClientID = pClient->OutsideExtraData;
			SendNetCmdToAllGame(pCmd);
		}
		return;
	}
	case CL_QueryBlackList:
	{

		if (pCmd->GetCmdSize() == sizeof(LC_CMD_QueryFishBlackList))
		{
			((LC_CMD_QueryFishBlackList*)pCmd)->ClientID = pClient->OutsideExtraData;
			SendNetCmdToAllGame(pCmd);
		}
		return;
	}
	case CL_SetBlackList:
	{
		if (pCmd->GetCmdSize() > sizeof(LC_CMD_SetFishBlackList))
		{
			((LC_CMD_SetFishBlackList*)pCmd)->ClientID = pClient->OutsideExtraData;
			SendNetCmdToAllGame(pCmd);
		}
		return;
	}
	case CL_OxAdmin:
	{
		if (pCmd->GetCmdSize() == sizeof(CL_CMD_OxAdminReq))
		{
			((CL_CMD_OxAdminReq*)pCmd)->ClientID = pClient->OutsideExtraData;
			SendNetCmdToMiniGame(pCmd);
		}
		break;
	}
	case CL_AnimalAdmin:
	{
		if (pCmd->GetCmdSize() == sizeof(CL_CMD_AnimalAdminReq))
		{
			((CL_CMD_AnimalAdminReq*)pCmd)->ClientID = pClient->OutsideExtraData;
			SendNetCmdToMiniGame(pCmd);
		}
		break;
	}
	case CL_CarAdmin:
	{
		if (pCmd->GetCmdSize() == sizeof(CL_CMD_CarAdminReq))
		{
			((CL_CMD_CarAdminReq*)pCmd)->ClientID = pClient->OutsideExtraData;
			SendNetCmdToMiniGame(pCmd);
		}
		break;
	}

	}
	return;
}
void FishServer::HandleServerMsg(ServerClientData* pClient, NetCmd* pCmd)
{
	//清理内部各个服务器发送来的命令
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return;
	}
	//各个服务器注册的命令
	if (pCmd->CmdType != Main_Control)
	{
		ASSERT(false);
		return;
	}
	int GMNetID = GetFishConfig().GetGMToolConfig().NetID_;
	switch (pCmd->SubCmdType)
	{
		//接收到其他服务器的数据 不保存 直接进行转发出去
	case LC_CenterInfo:
	case LC_GameInfo:
	case LC_LogonInfo:
	case LC_DBInfo:
	case LC_RankInfo:
	case LC_FTPInfo:
	case LC_OperateInfo:
	case LC_MiNiGameInfo:
	{
							SendNetCmdToAllClient(pCmd);
							break;
	}
	case CE_GM_QUERY_USER_ACK:
	{		
		SendNtCmdToGMTool(pCmd);
	}
	break;
	case LC_KickUserResult:
	{
							  //LC_Cmd_KickUserResult* pMsg = (LC_Cmd_KickUserResult*)pCmd;
							  //HashMap<DWORD, ServerClientData* >::iterator Iter = m_ControlClientList.find(pMsg->ClientID);
							  //if (Iter == m_ControlClientList.end())
							  //{
								 // ASSERT(false);
								 // return;
							  //}

		SendNtCmdToGMTool(pCmd);
							  //SendNetCmdToClient(Iter->second, pMsg);
							  break;
	}
	case LC_HandleEntityItem:
	{
		SendNtCmdToGMTool(pCmd);
		break;
	}

	case LC_QueryOnlineUserInfo:
	{
		SendNtCmdToGMTool(pCmd);
		break;
	}
	case LC_ChangeParticularStates:
	{
		SendNtCmdToGMTool(pCmd);
	}
		break;

	case LC_QueryFishPoolResult:
	{
		SendNtCmdToGMTool(pCmd);
		break;
	}
	case LC_QueryBlackListResult:
	{
		SendNtCmdToGMTool(pCmd);
		break;
	}
	case LC_SetBlackListResult:
	{
		SendNtCmdToGMTool(pCmd);
		 break;
	}
	case LC_OxAdmin:
	{
		SendNtCmdToGMTool(pCmd);
		break;
	}
	case LC_OxAdminQueryName:
	{
		SendNtCmdToGMTool(pCmd);
		break;
	}
	case LC_OxAdminQueryAllJetton:
	{
		SendNtCmdToGMTool(pCmd);
		 break;
	}
	case LC_OxAdminQueryPlayerJetton:
	{
		SendNtCmdToGMTool(pCmd);
										
		break;
	}
	case LC_AnimalAdmin:
	{
		SendNtCmdToGMTool(pCmd);
						  
		break;
	}
	case LC_AnimalAdminQueryName:
	{
		SendNtCmdToGMTool(pCmd);
								
		break;
	}
	case LC_AnimalAdminQueryAllJetton:
	{
		SendNtCmdToGMTool(pCmd);
										
		break;
	}
	case LC_AnimalAdminQueryPlayerJetton:
	{
		SendNtCmdToGMTool(pCmd);
		break;
	}

	case LC_CarAdmin:
	{
		SendNtCmdToGMTool(pCmd);
		break;
	}
	case LC_CarAdminQueryName:
	{
		SendNtCmdToGMTool(pCmd);
		 break;
	}
	case LC_CarAdminQueryAllJetton:
	{
		SendNtCmdToGMTool(pCmd);
		break;
	}
	case LC_CarAdminQueryPlayerJetton:
	{
		SendNtCmdToGMTool(pCmd);
										 
		break;
	}
	}
	return;
}
void FishServer::HandleDataBaseMsg(NetCmd* pCmd)
{
	//处理数据库发送来的命令
	if (!pCmd)
	{
		ASSERT(false);
		return;
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
	if (!m_ControlClientList.empty())
	{
		HashMap<DWORD, ServerClientData*>::iterator Iter = m_ControlClientList.begin();
		for (; Iter != m_ControlClientList.end();)
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
	if (!m_ControlServerList.empty())
	{
		HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.begin();
		for (; Iter != m_ControlServerList.end();)
		{
			if (!Iter->second)
				continue;
			while (Iter->second->RecvList.HasItem())
			{
				NetCmd* pCmd = Iter->second->RecvList.GetItem();
				if (Iter->first == g_FishServer.GetFishConfig().GetGMToolConfig().NetID_)
				{
					HandleGMToolMsg(Iter->second, pCmd);
				}
				else
				{
					HandleServerMsg(Iter->second, pCmd);
				}
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
	if (!pData || (SeverID != m_ControlServerNetworkID && SeverID != m_ControlClientNetworkID) || (SeverID == m_ControlServerNetworkID && recvSize != sizeof(BYTE)) || (SeverID == m_ControlClientNetworkID && recvSize != 0))
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
		if (pOnce->SeverID == m_ControlServerNetworkID)
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
			HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(ClientID);
			if (Iter != m_ControlServerList.end())
			{
				ASSERT(false);
				m_ServerTcp.Kick(pOnce->pClient);
				if (pOnce->pPoint)
					free(pOnce->pPoint);
				delete pOnce;
				continue;
			}
			m_ControlServerList.insert(HashMap<BYTE, ServerClientData*>::value_type(ClientID, pOnce->pClient));
			//触发玩家加入的事件
			OnTcpServerJoin(pOnce->SeverID, pOnce->pClient);
			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
			continue;
		}
		else if (pOnce->SeverID == m_ControlClientNetworkID)
		{
			if (pOnce->pPoint || pOnce->Length != 0)
			{
				m_ClientTcp.Kick(pOnce->pClient);
				delete pOnce;
				continue;
			}
			pOnce->pClient->OutsideExtraData = ++m_ControlClientIndex;
			pOnce->pClient->dwChecked = 0;
			HashMap<DWORD, ServerClientData*>::iterator Iter = m_ControlClientList.find(pOnce->pClient->OutsideExtraData);
			if (Iter != m_ControlClientList.end())
			{
				ASSERT(false);
				m_ClientTcp.Kick(pOnce->pClient);
				delete pOnce;
				continue;
			}
			m_ControlClientList.insert(HashMap<DWORD, ServerClientData*>::value_type(pOnce->pClient->OutsideExtraData, pOnce->pClient));
			//触发玩家加入的事件
			OnTcpServerJoin(pOnce->SeverID, pOnce->pClient);
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
	HashMap<DWORD, ServerClientData*>::iterator Iter = m_ControlClientList.begin();
	for (; Iter != m_ControlClientList.end(); ++Iter)
	{
		if (!m_ClientTcp.Send(Iter->second, pCmd))
		{
			ASSERT(false);
		}
	}
}
void  FishServer::SendNetCmdToControlDB(NetCmd* pCmd)
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

void FishServer::SendNtCmdToGMTool(NetCmd* pCmd)
{
	int NetID = GetFishConfig().GetGMToolConfig().NetID_;
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(NetID);
	if (Iter == m_ControlServerList.end())
	{
		ASSERT(false);
		return;
	}
	if (!m_ServerTcp.Send(Iter->second, pCmd))
	{
		ASSERT(false);
	}
}
void  FishServer::SendNetCmdToDB(BYTE DBID, NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(DBID);
	if (Iter == m_ControlServerList.end())
	{
		ASSERT(false);
		return;
	}
	if (!m_ServerTcp.Send(Iter->second, pCmd))
	{
		ASSERT(false);
	}
}
void  FishServer::SendNetCmdToAllDB(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.begin();
	for (; Iter != m_ControlServerList.end(); ++Iter)
	{
		if (g_FishServerConfig.GetDBServerConfig(Iter->first))
		{
			if (!m_ServerTcp.Send(Iter->second, pCmd))
			{
				ASSERT(false);
			}
		}
	}
}
void  FishServer::SendNetCmdToFtp(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServerConfig.GetFTPServerConfig())
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(g_FishServerConfig.GetFTPServerConfig()->NetworkID);
	if (Iter == m_ControlServerList.end())
	{
		ASSERT(false);
		return;
	}
	if (!m_ServerTcp.Send(Iter->second, pCmd))
	{
		ASSERT(false);
	}
}
void  FishServer::SendNetCmdToRank(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServerConfig.GetRankServerConfig())
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(g_FishServerConfig.GetRankServerConfig()->NetworkID);
	if (Iter == m_ControlServerList.end())
	{
		ASSERT(false);
		return;
	}
	if (!m_ServerTcp.Send(Iter->second, pCmd))
	{
		ASSERT(false);
	}
}
void  FishServer::SendNetCmdToLogon(BYTE LogonID, NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(LogonID);
	if (Iter == m_ControlServerList.end())
	{
		ASSERT(false);
		return;
	}
	if (!m_ServerTcp.Send(Iter->second, pCmd))
	{
		ASSERT(false);
	}
}
void  FishServer::SendNetCmdToAllLogon(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.begin();
	for (; Iter != m_ControlServerList.end(); ++Iter)
	{
		if (g_FishServerConfig.GetLogonServerConfig(Iter->first))
		{
			if (!m_ServerTcp.Send(Iter->second, pCmd))
			{
				ASSERT(false);
			}
		}
	}
}
void  FishServer::SendNetCmdToGame(BYTE GameID, NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(GameID);
	if (Iter == m_ControlServerList.end())
	{
		ASSERT(false);
		return;
	}
	if (!m_ServerTcp.Send(Iter->second, pCmd))
	{
		ASSERT(false);
	}
}
void  FishServer::SendNetCmdToAllGame(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.begin();
	for (; Iter != m_ControlServerList.end(); ++Iter)
	{
		if (g_FishServerConfig.GetGameServerConfig(Iter->first))
		{
			if (!m_ServerTcp.Send(Iter->second, pCmd))
			{
				ASSERT(false);
			}
		}
	}
}
void FishServer::SendNetCmdToAllServer(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.begin();
	for (; Iter != m_ControlServerList.end(); ++Iter)
	{
		if (!m_ServerTcp.Send(Iter->second, pCmd))
		{
			ASSERT(false);
		}
	}
}
void  FishServer::SendNetCmdToCenter(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServerConfig.GetCenterServerConfig())
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(g_FishServerConfig.GetCenterServerConfig()->NetworkID);
	if (Iter == m_ControlServerList.end())
	{
		ASSERT(false);
		return;
	}
	if (!m_ServerTcp.Send(Iter->second, pCmd))
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
	if (!g_FishServerConfig.GetMiniGameServerConfig())
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(g_FishServerConfig.GetMiniGameServerConfig()->NetworkID);
	if (Iter == m_ControlServerList.end())
	{
		ASSERT(false);
		return;
	}
	if (!m_ServerTcp.Send(Iter->second, pCmd))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToOperate(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServerConfig.GetOperateConfig())
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ControlServerList.find(g_FishServerConfig.GetOperateConfig()->NetworkID);
	if (Iter == m_ControlServerList.end())
	{
		ASSERT(false);
		return;
	}
	if (!m_ServerTcp.Send(Iter->second, pCmd))
	{
		ASSERT(false);
	}
}
FishServer g_FishServer;