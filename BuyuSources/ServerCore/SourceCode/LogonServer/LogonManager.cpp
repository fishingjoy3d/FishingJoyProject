#include "Stdafx.h"
#include "LogonManager.h"
#include "FishServer.h"
LogonManager::LogonManager()
{
	m_GameIndex =1;
	m_GameServerMap.clear();
}
LogonManager::~LogonManager()
{
	
}
void LogonManager::OnLogonRsgToCenter()
{
	//Logon 连接到Center
	/*LC_Cmd_RsgLogon msg;
	SetMsgInfo(msg, GetMsgType(Main_Server, LC_RsgLogon), sizeof(LC_Cmd_RsgLogon));
	msg.LogonConfigID = g_FishServer.GetLogonConfigID();
	g_FishServer.SendNetCmdToCenter(&msg);*/
}
void LogonManager::OnLogonRsgResult(CL_Cmd_RsgLogon* pMsg)
{
	if (!pMsg)
		return;
	if ((pMsg->States & MsgBegin) != 0)
		m_UserMap.clear();
	for (size_t i = 0; i < pMsg->Sum; ++i)//创建玩家的列表
	{
		m_UserMap.insert(HashMap<DWORD, BYTE>::value_type(pMsg->Array[i].dwUserID, pMsg->Array[i].GameConfigID));

		HashMap<BYTE, WORD>::iterator Iter = m_GamePlayerSum.find(pMsg->Array[i].GameConfigID);
		if (Iter == m_GamePlayerSum.end())
		{
			m_GamePlayerSum.insert(HashMap<BYTE, WORD>::value_type(pMsg->Array[i].GameConfigID, 1));
		}
		else
		{
			Iter->second += 1;
		}
	}
}
void LogonManager::OnLogonLeaveCenter()
{
	//当Logon离开中央服务器的时候
	//当自己与Logon断开连接的时候 表示当前Logon暂停服务 不在接收接入

	return;
}
void LogonManager::OnUserJoinCenter(DWORD dwUserID, BYTE GameConfigID)
{
	//将SocketID 转化为 ConfigureID
	HashMap<DWORD, BYTE>::iterator Iter = m_UserMap.find(dwUserID);
	if (Iter != m_UserMap.end())
	{
		ASSERT(false);
		return;
	}
	m_UserMap.insert(HashMap<DWORD, BYTE>::value_type(dwUserID, GameConfigID));

	HashMap<BYTE, WORD>::iterator IterSum = m_GamePlayerSum.find(GameConfigID);
	if (IterSum == m_GamePlayerSum.end())
	{
		m_GamePlayerSum.insert(HashMap<BYTE, WORD>::value_type(GameConfigID, 1));
	}
	else
	{
		IterSum->second += 1;
	}

	m_TempLogonInfo.OnUserLogon(dwUserID);//用户正式登陆 删除临时记录的数据
}
void LogonManager::OnUserLeaveCenter(DWORD dwUserID)
{
	HashMap<DWORD, BYTE>::iterator Iter = m_UserMap.find(dwUserID);
	if (Iter == m_UserMap.end())
	{
		//ASSERT(false);
		return;
	}
	BYTE GameConfigID = Iter->second;
	m_UserMap.erase(Iter);

	HashMap<BYTE, WORD>::iterator IterSum = m_GamePlayerSum.find(GameConfigID);
	if (IterSum == m_GamePlayerSum.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		IterSum->second -= 1;
		if (IterSum->second == 0)
		{
			m_GamePlayerSum.erase(IterSum);
		}
	}
}
void LogonManager::OnGameRsgLogon(BYTE GameConfigID)
{
	//一个GameServer连接到Logon上面来了
	HashMap<BYTE,DWORD>::iterator Iter = m_GameServerMap.find(GameConfigID);
	if (Iter != m_GameServerMap.end())
	{
		ASSERT(false);
		return;
	}
	m_GameServerMap.insert(HashMap<BYTE, DWORD>::value_type(GameConfigID,0));
	printf("GameServer 加入 Logon\n");
	//std::cout << "GameServer 加入 Logon\n";
}
void LogonManager::OnGameLeaveCenter(BYTE GameConfigID)
{
	HashMap<BYTE, DWORD>::iterator Iter = m_GameServerMap.find(GameConfigID);
	if (Iter == m_GameServerMap.end())
	{
		//ASSERT(false);
		return;
	}
	//因为一个GameServer的离开 我们让属于这个GameServer的玩家全部离开玩家集合
	m_GameServerMap.erase(Iter);

	HashMap<DWORD, BYTE>::iterator IterRole = m_UserMap.begin();
	for (; IterRole != m_UserMap.end();)
	{
		if (IterRole->second == GameConfigID)
		{
			IterRole = m_UserMap.erase(IterRole);
		}
		else
			++IterRole;
	}
	m_GamePlayerSum.erase(GameConfigID);

	g_FishServer.ShowInfoToWin("GameServer 离开Center 在Logon上进行删除");
}
void LogonManager::OnGameLeaveLogon(BYTE GameConfigID)
{
	HashMap<BYTE, DWORD>::iterator Iter = m_GameServerMap.find(GameConfigID);
	if (Iter == m_GameServerMap.end())
	{
		//ASSERT(false);
		return;
	}
	m_GameServerMap.erase(Iter);

	g_FishServer.ShowInfoToWin("GameServer 离开Logon");
}
void LogonManager::OnGameJoinLogon(BYTE GameConfigID)
{
}
BYTE LogonManager::GetGameServerConfigIDByUserID(DWORD dwUserID)//找到最合适玩家登陆的GameServer
{
	if (m_GameServerMap.empty())
	{
		ASSERT(false);
		return 0;
	}
	//根据玩家ID 获取玩家最合算的GameServer的ID
	HashMap<DWORD, BYTE>::iterator Iter = m_UserMap.find(dwUserID);
	if (Iter != m_UserMap.end())
		return Iter->second;
	else
	{
		if (m_GamePlayerSum.empty())
		{
			BYTE GameConfigID = m_GameServerMap.begin()->first;
			m_TempLogonInfo.OnUserTempLogon(dwUserID, GameConfigID);//用于进行临时的记录下
			return GameConfigID;
		}
		else
		{
			//我们先判断玩家是否有上次临时登陆的数据
			BYTE GameConfigID = m_TempLogonInfo.GetUserTempGameConfigID(dwUserID);
			if (GameConfigID != 0)
				return GameConfigID;
			//我们想要记录个 人数最少的 
			//测试 
			//多个GameServer 我们从多个GameServer里选择一个进行登陆 
			DWORD Index = m_GameIndex % m_GameServerMap.size();
			++m_GameIndex;

			HashMap<BYTE, DWORD>::iterator IterSum = m_GameServerMap.begin();
			for (DWORD i = 0; i < Index; ++i)
				++IterSum;
			return IterSum->first;
		}
	}
	return 0;
}