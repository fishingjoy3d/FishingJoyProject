#include "stdafx.h"
#include "GameServerManager.h"
#include "FishServer.h"
#include "..\CommonFile\FishServerConfig.h"
GameServerManager::GameServerManager()
{

}
GameServerManager::~GameServerManager()
{
	OnDestroy();
}
void GameServerManager::SendNetCmdToLogon(BYTE LogonID, NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, TCPClientOnce>::iterator Iter = m_LogonMap.find(LogonID);
	if (Iter == m_LogonMap.end())
	{
		ASSERT(false);
		return;
	}
	if (!Iter->second.m_pClient->IsConnected())
	{
		ASSERT(false);
		return;
	}
	Iter->second.m_pClient->Send(pCmd, false);
}
void GameServerManager::SendNetCmdToAllLogon(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, TCPClientOnce>::iterator Iter = m_LogonMap.begin();
	for (; Iter != m_LogonMap.end(); ++Iter)
	{
		if (!Iter->second.m_pClient->IsConnected())
			continue;
		Iter->second.m_pClient->Send(pCmd, false);
	}
}
//void GameServerManager::OnRsgGameToCenter()
//{
//	//将自己注册到GameServer去
//	//GC_Cmd_RsgGame msg;
//	//SetMsgInfo(msg, GetMsgType(Main_Server, GC_RsgGame), sizeof(GC_Cmd_RsgGame));
//	//msg.GameConfigID = g_FishServer.GetGameIndex();
//	//g_FishServer.SendNetCmdToCenter(&msg);//将命令发送到中央服务器去
//}
//void GameServerManager::OnRsgGameToCenterResult(CG_Cmd_RsgGame* pMsg)
//{
//	//GameServer  注册中央服务器的返回值
//	if (!pMsg)
//	{
//		ASSERT(false);
//		return;
//	}
//	if ((pMsg->States & MsgBegin) != 0)
//	{
//		m_LogonMap.clear();
//	}
//	for (WORD i = 0; i < pMsg->Sum; ++i)
//	{
//		BYTE LogonConfigID = pMsg->Array[i];//Logon对应的配置ID
//		//我们发送连接 连接到Logon去
//		TCPClientOnce pOnce;
//		LogonServerConfig* pLogonConfig = g_FishServerConfig.GetLogonServerConfig(LogonConfigID);
//		if (!pLogonConfig)
//		{
//			ASSERT(false);
//			continue;
//		}
//		pOnce.m_pClient = new TCPClient();
//		ClientInitData pLogonData;
//		pLogonData.BuffSize = pLogonConfig->LogonServerBuffSize;
//		pLogonData.Port = pLogonConfig->LogonServerListenPort;
//		pLogonData.SleepTime = pLogonConfig->LogonServerSleepTime;
//		pLogonData.SocketRecvSize = pLogonConfig->LogonServerRecvBuffSize;
//		pLogonData.SocketSendSize = pLogonConfig->LogonServerSendBuffSize;
//		pLogonData.ThreadNum = 1;
//		pLogonData.Timeout = pLogonConfig->LogonServerTimeOut;
//		pLogonData.CmdHearbeat = 0;
//		pLogonData.SendArraySize = pLogonConfig->LogonServerMaxSendCmdCount;
//		pLogonData.RecvArraySize = pLogonConfig->LogonServerMaxRecvCmdCount;
//
//		m_LogonMap.insert(HashMap<BYTE, TCPClientOnce>::value_type(LogonConfigID, pOnce));
//
//		BYTE GameID = g_FishServer.GetGameIndex();
//
//		if (!pOnce.m_pClient->Init(pLogonConfig->LogonServerListenIP, pLogonData, (void*)&GameID, sizeof(BYTE)))
//		{
//			g_FishServer.ShowInfoToWin("登陆服务器连接失败");
//			pOnce.m_ClientStates = false;
//		}
//		else
//		{
//			pOnce.m_ClientStates = true;
//
//			GameServerConfig* pGameConfig = g_FishServerConfig.GetGameServerConfig(g_FishServer.GetGameIndex());
//			if (!pGameConfig)
//				return;
//
//			//GL_Cmd_RsgGameServer msg;
//			//SetMsgInfo(msg, GetMsgType(Main_Logon, GL_RsgGameServer), sizeof(GL_Cmd_RsgGameServer));
//			////获取本机的IP地址 发送给LogonServer
//			//msg.GameConfigID = g_FishServer.GetGameIndex();
//			//SendNetCmdToLogon(LogonConfigID,&msg);
//
//			g_FishServer.ShowInfoToWin("已经与登陆服务器连接成功了");
//		}
//		
//	}
//}
void GameServerManager::OnGameLeavCenter()
{
	//当GameServer与中央服务器断开;连接的时候 我们断开全部的Logon的连接 在GameServer重新连接的时候 会重新连接的
	OnDestroy();
}
//void GameServerManager::OnLogonLeaveCenter(BYTE LogonID)
//{
//	HashMap<BYTE, TCPClientOnce>::iterator Iter = m_LogonMap.find(LogonID);
//	if (Iter == m_LogonMap.end())
//	{
//		ASSERT(false);
//		return;
//	}
//	Iter->second.m_pClient->Shutdown();
//	delete Iter->second.m_pClient;
//	m_LogonMap.erase(Iter);
//}
//void GameServerManager::OnLogonRsgCenter(BYTE LogonID)
//{
//	HashMap<BYTE, TCPClientOnce>::iterator Iter = m_LogonMap.find(LogonID);
//	if (Iter != m_LogonMap.end())
//	{
//		ASSERT(false);
//		return;
//	}
//	//一个新的Logon加入
//	TCPClientOnce pOnce;
//	LogonServerConfig* pLogonConfig = g_FishServerConfig.GetLogonServerConfig(LogonID);
//	if (!pLogonConfig)
//	{
//		ASSERT(false);
//		return;
//	}
//	pOnce.m_pClient = new TCPClient();
//	ClientInitData pLogonData;
//	pLogonData.BuffSize = pLogonConfig->LogonServerBuffSize;
//	pLogonData.Port = pLogonConfig->LogonServerListenPort;
//	pLogonData.SleepTime = pLogonConfig->LogonServerSleepTime;
//	pLogonData.SocketRecvSize = pLogonConfig->LogonServerRecvBuffSize;
//	pLogonData.SocketSendSize = pLogonConfig->LogonServerSendBuffSize;
//	pLogonData.ThreadNum = 1;
//	pLogonData.Timeout = pLogonConfig->LogonServerTimeOut;
//	pLogonData.CmdHearbeat = 0;
//	pLogonData.SendArraySize = pLogonConfig->LogonServerMaxSendCmdCount;
//	pLogonData.RecvArraySize = pLogonConfig->LogonServerMaxRecvCmdCount;
//
//	m_LogonMap.insert(HashMap<BYTE, TCPClientOnce>::value_type(LogonID, pOnce));
//
//	BYTE GameID = g_FishServer.GetGameIndex();
//	if (!pOnce.m_pClient->Init(pLogonConfig->LogonServerListenIP, pLogonData, (void*)&GameID, sizeof(BYTE)))
//	{
//		g_FishServer.ShowInfoToWin("登陆服务器连接失败");
//		pOnce.m_ClientStates = false;
//	}
//	else
//	{
//		pOnce.m_ClientStates = true;
//
//		//GL_Cmd_RsgGameServer msg;
//		//SetMsgInfo(msg, GetMsgType(Main_Logon, GL_RsgGameServer), sizeof(GL_Cmd_RsgGameServer));
//		////获取本机的IP地址 发送给LogonServer
//		//GameServerConfig* pGameConfig = g_FishServerConfig.GetGameServerConfig(g_FishServer.GetGameIndex());
//		//if (!pGameConfig)
//		//	return;
//		//msg.GameConfigID = g_FishServer.GetGameIndex();
//		//SendNetCmdToLogon(LogonID, &msg);
//
//
//		g_FishServer.ShowInfoToWin("已经与登陆服务器连接成功了");
//	}
//	
//}
//void GameServerManager::OnDestroy()
//{
//	if (m_LogonMap.empty())
//	{
//		return;
//	}
//	HashMap<BYTE, TCPClientOnce>::iterator Iter = m_LogonMap.begin();
//	for (; Iter != m_LogonMap.end(); ++Iter)
//	{
//		Iter->second.m_pClient->Shutdown();
//		delete Iter->second.m_pClient;
//	}
//	m_LogonMap.clear();
//}
//void GameServerManager::OnHandleLogonMsg()
//{
//	if (m_LogonMap.empty())
//	{
//		return;
//	}
//	HashMap<BYTE, TCPClientOnce>::iterator Iter = m_LogonMap.begin();
//	for (; Iter != m_LogonMap.end(); ++Iter)
//	{
//		if (Iter->second.m_pClient->IsConnected())
//		{
//			if (!Iter->second.m_ClientStates)
//			{
//				Iter->second.m_ClientStates = true;
//				g_FishServer.ShowInfoToWin("与登陆服务器连接成功");
//				//发送一个注册的命令
//				//GL_Cmd_RsgGameServer msg;
//				//SetMsgInfo(msg, GetMsgType(Main_Logon, GL_RsgGameServer), sizeof(GL_Cmd_RsgGameServer));
//				////获取本机的IP地址 发送给LogonServer
//				//GameServerConfig* pGameConfig = g_FishServerConfig.GetGameServerConfig(g_FishServer.GetGameIndex());
//				//if (!pGameConfig)
//				//	return;
//				//msg.GameConfigID = g_FishServer.GetGameIndex();
//				//SendNetCmdToLogon(Iter->first, &msg);//将GameServer的注册发送到Logon去
//			}
//			NetCmd* pCmd = Iter->second.m_pClient->GetCmd();
//			int Sum = 0;
//			while (pCmd && Sum < Msg_OnceSum)
//			{
//				g_FishServer.HandleLogonMsg(Iter->first, pCmd);//将命令进行处理
//				free(pCmd);
//				++Sum;
//				pCmd = Iter->second.m_pClient->GetCmd();
//			}
//			if (pCmd)
//			{
//				g_FishServer.HandleLogonMsg(Iter->first, pCmd);//将命令进行处理
//				free(pCmd);
//			}
//		}
//		else
//		{
//			if (Iter->second.m_ClientStates)
//			{
//				g_FishServer.ShowInfoToWin("与登陆服务器断开连接");
//				Iter->second.m_ClientStates = false;
//			}
//			//重新连接DB
//			//根据配置文件ID 进行处理连接
//			BYTE LogonConfigID = Iter->first;
//
//			LogonServerConfig* pLogonConfig = g_FishServerConfig.GetLogonServerConfig(LogonConfigID);
//			if (!pLogonConfig)
//			{
//				ASSERT(false);
//				continue;
//			}
//			ClientInitData pLogonData;
//			pLogonData.BuffSize = pLogonConfig->LogonServerBuffSize;
//			pLogonData.Port = pLogonConfig->LogonServerListenPort;
//			pLogonData.SleepTime = pLogonConfig->LogonServerSleepTime;
//			pLogonData.SocketRecvSize = pLogonConfig->LogonServerRecvBuffSize;
//			pLogonData.SocketSendSize = pLogonConfig->LogonServerSendBuffSize;
//			pLogonData.ThreadNum = 1;
//			pLogonData.Timeout = pLogonConfig->LogonServerTimeOut;
//			pLogonData.CmdHearbeat = 0;
//			pLogonData.SendArraySize = pLogonConfig->LogonServerMaxSendCmdCount;
//			pLogonData.RecvArraySize = pLogonConfig->LogonServerMaxRecvCmdCount;
//
//			BYTE GameID = g_FishServer.GetGameIndex();
//			if (!Iter->second.m_pClient->Init(pLogonConfig->LogonServerListenIP, pLogonData, (void*)&GameID, sizeof(BYTE)))
//			{
//				g_FishServer.ShowInfoToWin("登陆服务器连接失败");
//				Iter->second.m_ClientStates = false;
//			}
//		}
//	}
//}