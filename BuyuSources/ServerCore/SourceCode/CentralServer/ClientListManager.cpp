#include "Stdafx.h"
#include "ClientListManager.h"
#include "FishServer.h"
ClientListManager::ClientListManager()
{
	m_PlayerList.clear();
	m_GameServerMap.clear();
	m_LogonServerMap.clear();
}
ClientListManager::~ClientListManager()
{

}
void ClientListManager::SendNetCmdToGameServer(BYTE GameID, NetCmd* pCmd)
{
	//将命令发送到GameServer去
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(GameID);
	if (!pClient)
	{
		//ASSERT(false);
		return;
	}
	g_FishServer.SendNetCmdToClient(pClient, pCmd);
}
void ClientListManager::SendNetCmdToLogonServer(BYTE LogonID, NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(LogonID);
	if (!pClient)
	{
		//ASSERT(false);
		return;
	}
	g_FishServer.SendNetCmdToClient(pClient, pCmd);
}
void ClientListManager::SendNetCmdToAllGameServer(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (m_GameServerMap.empty())
		return;
	HashMap<BYTE,DWORD>::iterator Iter = m_GameServerMap.begin();
	for (; Iter != m_GameServerMap.end(); ++Iter)
	{
		SendNetCmdToGameServer(Iter->first, pCmd);
	}
}
DWORD ClientListManager::GetGameServerSum()
{
	return m_GameServerMap.size();
}
DWORD ClientListManager::GetLogonServerSum()
{
	return m_LogonServerMap.size();
}
void ClientListManager::SendNetCmdToAllLogonServer(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (m_LogonServerMap.empty())
	{
		return;
	}
	HashMap<BYTE, DWORD>::iterator Iter = m_LogonServerMap.begin();
	for (; Iter != m_LogonServerMap.end(); ++Iter)
	{
		SendNetCmdToLogonServer(Iter->first, pCmd);
	}
}
void ClientListManager::RsgLogon(BYTE LogonID)
{
	//当一个Logon注册进来的时候
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(LogonID);
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, DWORD>::iterator Iter = m_LogonServerMap.find(LogonID);
	if (Iter != m_LogonServerMap.end())
	{
		ASSERT(false);
		return;
	}
	m_LogonServerMap.insert(HashMap<BYTE, DWORD>::value_type(LogonID,0));
	//因为一个Logon进来了 我们想要将全部玩家的列表发送给Logon
	DWORD MsgPage = sizeof(CL_Cmd_RsgLogon)+(m_PlayerList.size() - 1) * sizeof(tagPlayerInfo);//5 个长度  应给进行分包发送
	CL_Cmd_RsgLogon * msgCL = (CL_Cmd_RsgLogon*)malloc(MsgPage);
	if (!msgCL)
	{
		ASSERT(false);
		return;
	}
	msgCL->SetCmdType(GetMsgType(Main_Server, CL_RsgLogon));
	HashMap<DWORD, BYTE>::iterator IterRole = m_PlayerList.begin();
	for (int i = 0; IterRole != m_PlayerList.end(); ++IterRole, ++i)
	{
		msgCL->Array[i].dwUserID = IterRole->first;
		msgCL->Array[i].GameConfigID = IterRole->second;
	}

	std::vector<CL_Cmd_RsgLogon*> pVec;
	SqlitMsg(msgCL, MsgPage, m_PlayerList.size(),false, pVec);
	free(msgCL);
	if (!pVec.empty())
	{
		std::vector<CL_Cmd_RsgLogon*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			SendNetCmdToLogonServer(LogonID, *Iter);
			free(*Iter);
		}
		pVec.clear();
	}

	//将全部的GameServer的数据 发送到Logon上面去
	{
		DWORD MsgPage = sizeof(CL_Cmd_GameInfo)+(m_GameServerMap.size() - 1) * sizeof(BYTE);//5 个长度  应给进行分包发送
		CL_Cmd_GameInfo * msgCL = (CL_Cmd_GameInfo*)malloc(MsgPage);
		if (!msgCL)
		{
			ASSERT(false);
			return;
		}
		msgCL->SetCmdType(GetMsgType(Main_Server, CL_GameInfo));
		HashMap<BYTE, DWORD>::iterator IterRole = m_GameServerMap.begin();
		for (int i = 0; IterRole != m_GameServerMap.end(); ++IterRole, ++i)
		{
			msgCL->Array[i] = IterRole->first;
		}

		std::vector<CL_Cmd_GameInfo*> pVec;
		SqlitMsg(msgCL, MsgPage, m_GameServerMap.size(), false, pVec);
		free(msgCL);
		if (!pVec.empty())
		{
			std::vector<CL_Cmd_GameInfo*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				SendNetCmdToLogonServer(LogonID, *Iter);
				free(*Iter);
			}
			pVec.clear();
		}
	}

	//将Logon 的数据 发送给 其他全部的GameServer去
	/*CG_Cmd_RsgLogon msgCG;
	SetMsgInfo(msgCG, GetMsgType(Main_Server, CG_RsgLogon), sizeof(CG_Cmd_RsgLogon));
	msgCG.LogonConfigID = LogonID;
	SendNetCmdToAllGameServer(&msgCG);*/
	g_FishServer.ShowInfoToWin("Logon注册进来 ID:%d",LogonID);
}
void ClientListManager::UnRsgLogon(BYTE LogonID)
{
	//当一个Logon离开Center的时候 我们如何处理呢?
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(LogonID);
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, DWORD>::iterator Iter= m_LogonServerMap.find(LogonID);
	if (Iter == m_LogonServerMap.end())
	{
		ASSERT(false);
		return;
	}

	//通知全部的GameServer Logon离开了Center GameServer断开与Logon的连接
	/*CG_Cmd_UnRsgLogon msg;
	SetMsgInfo(msg, GetMsgType(Main_Server, CG_UnRsgLogon), sizeof(CG_Cmd_UnRsgLogon));
	msg.LogonConfigID = LogonID;
	SendNetCmdToAllGameServer(&msg);*/

	m_LogonServerMap.erase(Iter);

	g_FishServer.ShowInfoToWin("Logon离开 ID:%d",LogonID);
}
void ClientListManager::RsgGame(BYTE GameID)
{
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(GameID);
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, DWORD>::iterator Iter = m_GameServerMap.find(GameID);
	if (Iter != m_GameServerMap.end())
	{
		ASSERT(false);
		return;
	}
	m_GameServerMap.insert(HashMap<BYTE,DWORD>::value_type(GameID,0));
	//当一个GameServer注册进来的时候 我们想要将全部的Logon的数据发送给GameServer
	g_FishServer.GetMonthManager().OnSendMonthDataToGame(GameID);//将比赛的数据同步给Game

	//因为一个Game注册进入 需要将当前Game注册到Logon上面去
	CL_Cmd_GameInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_Server, CL_GameInfo), sizeof(CL_Cmd_GameInfo));
	msg.Sum = 1;
	msg.Array[0] = GameID;
	msg.States = MsgBegin | MsgEnd;
	SendNetCmdToAllLogonServer(&msg);//发送给全部的Logon去

	g_FishServer.GetMonthManager().OnGameRsg(GameID);

	/*DWORD PageSize = sizeof(CG_Cmd_RsgGame)+(m_LogonServerMap.size() - 1)*sizeof(BYTE);
	CG_Cmd_RsgGame* msg = (CG_Cmd_RsgGame*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Server, CG_RsgGame));
	Iter = m_LogonServerMap.begin();
	for (DWORD i = 0; Iter != m_LogonServerMap.end(); ++Iter, ++i)
	{
		msg->Array[i] = Iter->first;
	}
	std::vector<CG_Cmd_RsgGame*> pVec;
	SqlitMsg(msg, PageSize, m_LogonServerMap.size(), false, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<CG_Cmd_RsgGame*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			SendNetCmdToGameServer(GameID, *Iter);
			free(*Iter);
		}
		pVec.clear();
	}*/
	g_FishServer.ShowInfoToWin("Game注册进来 ID:%d",GameID);
}
void ClientListManager::UnRsgGame(BYTE GameID)
{
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(GameID);
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, DWORD>::iterator Iter = m_GameServerMap.find(GameID);
	if (Iter == m_GameServerMap.end())
	{
		ASSERT(false);
		return;
	}
	//当一个GameServer离开Center的时候 将当前GameServer 发送给全部的Logon 让Logon删除掉
	CL_Cmd_UnRsgGame msg;
	SetMsgInfo(msg, GetMsgType(Main_Server, CL_UnRsgGame), sizeof(CL_Cmd_UnRsgGame));
	msg.GameConfigID = GameID;
	SendNetCmdToAllLogonServer(&msg);

	m_GameServerMap.erase(Iter);

	g_FishServer.ShowInfoToWin("Game离开 ID:%d",GameID);
}
void ClientListManager::RsgUser(DWORD UserID, BYTE GameID)
{
	//当一个玩家进来的时候 我们进行处理
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(GameID);
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//一个玩家进入了中央服务器 插入到集合
	HashMap<DWORD,BYTE>::iterator IterRole = m_PlayerList.find(UserID);
	if (IterRole != m_PlayerList.end())
	{
		ASSERT(false);
		return;
	}
	m_PlayerList.insert(HashMap<DWORD, BYTE>::value_type(UserID, GameID));//玩家对于的gameServer
	//告诉全部的Logon 玩家进入 的命令
	CL_Cmd_RsgUser msg;
	SetMsgInfo(msg, GetMsgType(Main_Server, CL_RsgUser), sizeof(CL_Cmd_RsgUser));
	msg.PlayerInfo.dwUserID = UserID;
	msg.PlayerInfo.GameConfigID = GameID;
	SendNetCmdToAllLogonServer(&msg);//告诉全部的Logon服务器
	//SendNetCmdToAllGameServer(&msg);

	//g_FishServer.ShowInfoToWin("玩家 ID: %d  进入中央服务器 绑定GameServerID为: %d",UserID,GameID);
}
void ClientListManager::UnRsgUser(DWORD UserID)
{
	HashMap<DWORD, BYTE>::iterator IterRole = m_PlayerList.find(UserID);
	if (IterRole == m_PlayerList.end())
	{
		ASSERT(false);
		return;
	}
	//玩家离开的时候
	CL_Cmd_UnRsgUser msg;
	SetMsgInfo(msg, GetMsgType(Main_Server, CL_UnRsgUser), sizeof(CL_Cmd_UnRsgUser));
	msg.dwUserID = UserID;
	SendNetCmdToAllLogonServer(&msg);//告诉全部的Logon服务器
	m_PlayerList.erase(IterRole);

	//g_FishServer.ShowInfoToWin("玩家 ID: %d  离开中央服务器", UserID);
}
void ClientListManager::OnClientLeave(BYTE CLientID)//当登陆服务器 或者 中央服务器离开的时候
{
	//当一个平台离开的时候 
	HashMap<BYTE,DWORD>::iterator Iter = m_GameServerMap.find(CLientID);
	if (Iter != m_GameServerMap.end())
	{
		//GameServer 离开
		UnRsgGame(CLientID);
		//让当前平台上的玩
		g_FishServer.GetRoleManager().OnPlazaLeave(CLientID);
	}
	else
	{
		Iter = m_LogonServerMap.find(CLientID);
		if (Iter != m_LogonServerMap.end())
		{
			UnRsgLogon(CLientID);
		}
		else
		{
			//ASSERT(false);
			return;
		}
	}
}