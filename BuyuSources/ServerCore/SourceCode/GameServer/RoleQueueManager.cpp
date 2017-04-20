#include "Stdafx.h"
#include "RoleQueueManager.h"
#include "FishServer.h"
#include "RoleEx.h"
RoleQueueManager::RoleQueueManager()
{
	m_QueueList.clear();
	m_UpdateTimeLog = 0;
	m_QueueMsgUpate = 0;
}
RoleQueueManager::~RoleQueueManager()
{

}
bool RoleQueueManager::OnAddRoleToQueue(DWORD dwUserID, DWORD ClientID, bool LogonByGameServer)
{
	CRoleEx* pRole = g_FishServer.GetRoleManager()->QueryUser(dwUserID);
	if (pRole)
	{
		ASSERT(false);
		return false;
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(ClientID);
	if (!pClient)
	{
		ASSERT(false);
		return false;
	}
	tagRoleQueueOnce pOnce;
	pOnce.ClientID = ClientID;
	pOnce.dwUserID = dwUserID;
	pOnce.LogonByGameServer = LogonByGameServer;
	m_QueueList.push_back(pOnce);
	//计算客户端 需要排队的时间 每10秒减少10个 
	DWORD HandleSum = g_FishServer.GetFishConfig().GetSystemConfig().UserQueueHandleSumBySec;
	DWORD WriteSec = ((m_QueueList.size() % HandleSum == 0) ? (m_QueueList.size() / HandleSum) : (m_QueueList.size() / HandleSum + 1)) * 1;//需要等待的秒数
	//发送到客户端去
	LC_Cmd_LogonQueueWrite msg;
	SetMsgInfo(msg, GetMsgType(Main_Logon, LC_LogonQueueWrite), sizeof(LC_Cmd_LogonQueueWrite));
	msg.WriteIndex = m_QueueList.size();
	msg.WriteSec = WriteSec;
	g_FishServer.SendNewCmdToClient(pClient, &msg);
	return true;
}
void RoleQueueManager::OnUpdateQueue(DWORD dwTimer)
{
	if (m_UpdateTimeLog == 0 || dwTimer - m_UpdateTimeLog >= 1000)
	{
		m_UpdateTimeLog = dwTimer;


		//继续更新
		if (m_QueueList.empty())
			return;

		DWORD HandleSum = g_FishServer.GetFishConfig().GetSystemConfig().UserQueueHandleSumBySec;

		std::list<tagRoleQueueOnce>::iterator Iter = m_QueueList.begin();
		for (size_t i = 0; Iter != m_QueueList.end() && i< HandleSum;)
		{
			ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(Iter->ClientID);
			if (!pClient)
			{
				//玩家不排队了 下一个
				Iter = m_QueueList.erase(Iter);
				continue;
			}
			//一秒进入 10个玩家
			DWORD dwUserID = Iter->dwUserID;
			//让玩家进行登陆 并且是进行数据库的登陆
			CRoleEx* pRole = g_FishServer.GetRoleManager()->QueryUser(dwUserID);
			if (pRole)
			{
				ASSERT(false);
				if (pRole->IsRobot())
				{
					ASSERT(false);
				}
				else
				{
					if (!pRole->IsAFK())
					{
						NetCmd msg;
						SetMsgInfo(msg, GetMsgType(Main_Logon, LC_ServerChangeSocket), sizeof(NetCmd));
						pRole->SendDataToClient(&msg);

						DelClient pDel;
						pDel.LogTime = timeGetTime();
						pDel.SocketID = pRole->GetGameSocketID();
						g_FishServer.AddDelRoleSocket(pDel);

						//玩家Socket进行切换
						DWORD SocketID = pRole->GetGameSocketID();
						pRole->ChangeRoleSocketID(Iter->ClientID);

						//告诉客户端玩家登陆成功了
						pRole->OnUserLoadFinish(Iter->LogonByGameServer);//玩家登陆 不在桌子里面
					}
					else
					{
						g_FishServer.GetTableManager()->OnPlayerLeaveTable(pRole->GetUserID());//先离开桌子
						g_FishServer.GetRoleCache().OnDleRoleCache(pRole->GetUserID());
						pRole->ChangeRoleSocketID(Iter->ClientID);//修改缓存玩家的Socket
						pRole->OnUserLoadFinish(Iter->LogonByGameServer);//离线 进入 可能在桌子里面 其中 可以设置玩家上线操作
					}
					if (pRole->IsExit())
					{
						pRole->SetIsExit(false);//设置玩家重新上线
						//g_FishServer.ShowInfoToWin("玩家进行离线保存 在回发确认命令之前 重新登陆了  回发确认命令 失效 ");
					}
					
				}
			}
			else
			{
				//让玩家进行数据库查询 登陆
				DBR_Cmd_GetAccountInfoByUserID msg;
				SetMsgInfo(msg, DBR_GetAccountInfoByUserID, sizeof(DBR_Cmd_GetAccountInfoByUserID));
				msg.dwUserID = dwUserID;
				msg.ClientID = Iter->ClientID;
				msg.LogonByGameServer = true;
				g_FishServer.SendNetCmdToDB(&msg);

				++i;
			}
			Iter = m_QueueList.erase(Iter);
		}
	}
}