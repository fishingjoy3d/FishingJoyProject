#include "Stdafx.h"
#include "RoleManager.h"
#include "FishServer.h"
#include "RoleEx.h"
RoleManager::RoleManager()
{

}
RoleManager::~RoleManager()
{
	Destroy();
}
void RoleManager::Destroy()
{
	if (m_RoleMap.empty())
		return;
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.begin();
	for (; Iter != m_RoleMap.end(); ++Iter)
	{
		if (!Iter->second)
			continue;
		Iter->second->SaveAllRoleInfo(false);
		Iter->second->SetIsExit(true);//设置玩家下线
		delete Iter->second;
	}
	m_RoleMap.clear();
	m_RoleSocketMap.clear();
	m_RoleGameMap.clear();
}
bool RoleManager::OnInit()
{
	return true;
}
CRoleEx* RoleManager::QueryUser(DWORD dwUserID)
{
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.find(dwUserID);
	if (Iter != m_RoleMap.end())
		return Iter->second;
	else
		return NULL;
}
CRoleEx* RoleManager::QueryUserByGameID(DWORD GameID)
{
	//查询玩家 根据GameID
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleGameMap.find(GameID);
	if (Iter != m_RoleMap.end())
		return Iter->second;
	else
		return NULL;
}
CRoleEx* RoleManager::QuertUserBySocketID(DWORD dwSocketID)
{
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleSocketMap.find(dwSocketID);
	if (Iter != m_RoleSocketMap.end())
		return Iter->second;
	else
		return NULL;
}
void  RoleManager::OnDelUser(DWORD dwUserID, bool IsWriteSaveInfo, bool IsLeaveCenter)
{
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.find(dwUserID);
	if (Iter != m_RoleMap.end())
	{
		//当一个玩家掉线 或者 退出GameServer的时候 我们应该缓存玩家的RoleEx对象 
		//RoleEx对象 不在删除 直接设置RoleEx 玩家离线了 设置SocketID 为0  无客户端数据了 在比赛离开比赛 在桌子 离开桌子等 
		//并且需要和Logon进行通讯 当玩家进入到GameServer的时候 我们想要让Logon找到 玩家 在多个gameServer 上的分布表
		if (IsWriteSaveInfo)
		{
			if (Iter->second->SaveAllRoleInfo(true))
			{
				//需要保持 并且 需要等待
				return;
			}
			else
			{
				ASSERT(false);
			}
		}
		//直接删除玩家 是否需要设置玩家下线呢?
		//被顶掉 和 创建玩家失败 都不需要设置玩家下线
		//玩家直接离线 
		//Iter->second->ChangeRoleIsOnline(false);
		if (IsLeaveCenter)
			Iter->second->SendUserLeaveToCenter();
		g_FishServer.GetTableManager()->OnPlayerLeaveTable(Iter->second->GetUserID());

		m_RoleSocketMap.erase(Iter->second->GetGameSocketID()/*GetGateSocketID*/);
		m_RoleGameMap.erase(Iter->second->GetRoleInfo().GameID);
		delete Iter->second;
		m_RoleMap.erase(Iter);
	}
}
void RoleManager::OnDelUserResult(DBO_Cmd_SaveRoleAllInfo* pResult)
{
	if (!pResult)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.find(pResult->dwUserID);
	if (Iter != m_RoleMap.end())
	{
		if (Iter->second->IsExit())//玩家在退出状态
		{
			//g_FishServer.ShowInfoToWin("玩家进行离线保存回发命令成功 玩家正式下线");
			//Iter->second->SetIsExit(false);
			//Iter->second->ChangeRoleIsOnline(false);
			Iter->second->SendUserLeaveToCenter();
			g_FishServer.GetTableManager()->OnPlayerLeaveTable(Iter->second->GetUserID());

			m_RoleSocketMap.erase(Iter->second->GetGameSocketID());
			m_RoleGameMap.erase(Iter->second->GetRoleInfo().GameID);
			delete Iter->second;
			m_RoleMap.erase(Iter);
		}
		else
		{
			//玩家查询上线了 不做处理
			//g_FishServer.ShowInfoToWin("玩家进行离线保存回发命令成功 回发命令失效 玩家重新上线了");
			return;
		}
	}
	else
	{
		ASSERT(false);
		return;
	}
}
void RoleManager::OnKickAllUser()
{
	//直接踢掉所有的玩家 
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.begin();
	for (; Iter != m_RoleMap.end(); ++Iter)
	{
		Iter->second->SendUserLeaveToCenter();
		g_FishServer.GetTableManager()->OnPlayerLeaveTable(Iter->second->GetUserID());
		Iter->second->SaveAllRoleInfo(false);//保存下玩家的数据
		Iter->second->SetIsExit(true);//设置玩家离线 特殊情况下的处理
		delete Iter->second;
	}
	m_RoleMap.clear();
	m_RoleSocketMap.clear();
	m_RoleGameMap.clear();
}
bool RoleManager::CreateRole(tagRoleInfo* pUserInfo,tagRoleServerInfo* pUserServerInfo, DWORD dwSocketID, time_t pTime, bool LogobByGameServer, bool IsRobot)
{
	if (!pUserInfo || !pUserServerInfo)
	{
		ASSERT(false);
		return false;
	}
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.find(pUserInfo->dwUserID);
	if (Iter != m_RoleMap.end())
	{
		return true;
	}
	else
	{
		CRoleEx * pUser = new CRoleEx();
		if (!pUser)
		{
			ASSERT(false);
			return false;
		}
		//玩家对象初始化完毕后 我们存储起来
		m_RoleMap.insert(HashMap<DWORD, CRoleEx*>::value_type(pUserInfo->dwUserID, pUser));
		m_RoleGameMap.insert(HashMap<DWORD, CRoleEx*>::value_type(pUserInfo->GameID, pUser));
		if (!IsRobot)//机器人无须添加到SocketMap里面
			m_RoleSocketMap.insert(HashMap<DWORD, CRoleEx*>::value_type(dwSocketID/*dwGateSocketID*/, pUser));
		if (!pUser->OnInit(pUserInfo, pUserServerInfo, this, dwSocketID, pTime, LogobByGameServer, IsRobot))
		{
			OnDelUser(pUserInfo->dwUserID,false,false);//不需要删除中央服务器
			delete pUser;
			return false;
		}
		return true;
	}
}
void RoleManager::SendSystemMailToUser(DWORD dwUserID, LPTSTR pContext, WORD RewardID)
{
	//发送一封系统邮件到中央服务器去
	CC_Cmd_SendSystemMail msg;
	SetMsgInfo(msg,GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
	msg.dwDestUserID = dwUserID;
	msg.MailInfo.RewardID = RewardID;
	TCHARCopy(msg.MailInfo.Context, CountArray(msg.MailInfo.Context), pContext, _tcslen(pContext));
	msg.MailInfo.bIsRead = false;
	msg.MailInfo.MailID = 0;
	msg.MailInfo.SendTimeLog = time(NULL);
	msg.MailInfo.SrcFaceID = 0;
	msg.MailInfo.SrcUserID = 0;
	msg.MailInfo.bIsExistsReward = (msg.MailInfo.RewardID != 0 && msg.MailInfo.RewardSum != 0);
	TCHARCopy(msg.MailInfo.SrcNickName, CountArray(msg.MailInfo.SrcNickName), TEXT(""), 0);
	g_FishServer.SendNetCmdToCenter(&msg);
}
void RoleManager::OnUpdate(DWORD dwTimer)
{
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.begin();
	for (; Iter != m_RoleMap.end(); ++Iter)
	{
		Iter->second->GetRoleMonth().OnUpdate(dwTimer);
	}
}
void RoleManager::OnUpdateRoleTask()
{
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.begin();
	for (; Iter != m_RoleMap.end(); ++Iter)
	{
		Iter->second->GetRoleTaskManager().OnDayChange();
	}
}
void RoleManager::OnUpdateByMin(bool IsHourChange, bool IsDayChange, bool IsMonthChange, bool IsYearChange)
{
	if (m_RoleMap.empty())
		return;
	SYSTEMTIME pTime;
	GetLocalTime(&pTime);
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.begin();
	for (; Iter != m_RoleMap.end(); ++Iter)
	{
		Iter->second->UpdateByMin(IsHourChange, IsDayChange, IsMonthChange, IsYearChange);
		if (pTime.wMinute == 0)//整点进行更新 
		{
			Iter->second->GetRoleActionManager().UpdateByHour();
		}
	}
}
void RoleManager::ChangeRoleSocket(CRoleEx* pRole, DWORD SocketID)
{
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	m_RoleSocketMap.erase(pRole->GetGameSocketID());
	if (SocketID != 0)
		m_RoleSocketMap.insert(HashMap<DWORD, CRoleEx*>::value_type(SocketID, pRole));
}
//void RoleManager::SetRoleLeave(CRoleEx* pRole)
//{
//	m_RoleSocketMap.erase(pRole->GetGameSocketID());
//}
//void RoleManager::SetRoleOnLine(CRoleEx* pRole, DWORD SocketID)
//{
//	m_RoleSocketMap.insert(HashMap<DWORD, CRoleEx*>::value_type(SocketID, pRole));
//}
void RoleManager::OnSaveInfoToDB()
{
	if (m_RoleMap.empty())
		return;
	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.begin();
	for (; Iter != m_RoleMap.end(); ++Iter)
	{
		Iter->second->OnSaveInfoToDB();
	}
}
//void RoleManager::OnGetRoleAchievementIndex()
//{
//	if (m_RoleMap.empty())
//		return;
//	HashMap<DWORD, CRoleEx*>::iterator Iter = m_RoleMap.begin();
//	for (; Iter != m_RoleMap.end(); ++Iter)
//	{
//		Iter->second->OnGetRoleAchievenmentIndexByDB();
//	}
//}