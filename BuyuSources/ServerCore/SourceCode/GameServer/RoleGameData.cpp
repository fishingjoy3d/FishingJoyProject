#include "Stdafx.h"
#include "RoleGameData.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleGameData::RoleGameData()
{
	m_IsLoadDB = false;
}
RoleGameData::~RoleGameData()
{
	//SaveRoleGameData();
}
bool RoleGameData::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	//发送数据库
	m_IsLoadDB = false;
	OnLoadRoleGameDataByDB();
	return true;
}
void RoleGameData::OnLoadRoleGameDataByDB()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	/*DBR_Cmd_LoadGameData msg;
	SetMsgInfo(msg, DBR_LoadGameData, sizeof(DBR_Cmd_LoadGameData));
	msg.dwUserID = m_pRole->GetUserID();
	g_FishServer.SendNetCmdToDB(&msg);*/
}
void RoleGameData::OnLoadRoleGameDataResult(DBO_Cmd_LoadGameData* pMsg)
{
	if (!m_pRole || !pMsg)
	{
		ASSERT(false);
		return;
	}
	//将数据库和结构绑定起来
	m_RoleGameData = pMsg->GameData;
	m_IsLoadDB = true;
	m_pRole->IsLoadFinish();
}
void RoleGameData::SendRoleGameDataToClient()
{
	//将数据发送到客户端去
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	LC_Cmd_GendGameData msg;
	SetMsgInfo(msg, GetMsgType(Main_GameData, LC_GendGameData), sizeof(LC_Cmd_GendGameData));
	msg.GameData = m_RoleGameData;
	m_pRole->SendDataToClient(&msg);
}
//void RoleGameData::SaveRoleGameData()
//{
//	//发送数据库命令 将数据保存起来
//	if (!m_IsNeedSave || !m_pRole)
//		return;
//	//发送DBR命令 保存数据
//	DBR_Cmd_SaveGameData msg;
//	SetMsgInfo(msg, DBR_SaveGameData, sizeof(DBR_Cmd_SaveGameData));
//	msg.dwUserID = m_pRole->GetUserID();
//	msg.GameData = m_RoleGameData;
//	g_FishServer.SendNetCmdToDB(&msg);
//	m_IsNeedSave = false;
//}
void RoleGameData::OnHandleCatchFish(BYTE FishID)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//玩家捕获指定ID的鱼.
	m_RoleGameData.RoleCatchFishTotalSum += 1;	
	if (FishID == 9)
	{
		m_RoleGameData.CatchFishSum_9 += 1;
	}
	else if (FishID == 18)
	{
		m_RoleGameData.CatchFishSum_18 += 1;
	}
	else if (FishID == 20)
	{
		m_RoleGameData.CatchFishSum_20 += 1;
	}
	else if (FishID == 1)
	{
		m_RoleGameData.CatchFishSum_1 += 1;
	}
	else if (FishID == 3)
	{
		m_RoleGameData.CatchFishSum_3 += 1;
	}
	else if (FishID == 19)
	{
		m_RoleGameData.CatchFishSum_19 += 1;
	}
	m_pRole->SetRoleIsNeedSave();
}
void RoleGameData::OnHandleRoleGetGlobel(DWORD AddGlobel)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	m_RoleGameData.RoleGetGlobelSum += AddGlobel;
	m_pRole->SetRoleIsNeedSave();
}
void RoleGameData::OnHandleRoleMonthReward(int RewardIndex)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	m_RoleGameData.RoleMonthRewardSum += 1;
	if (RewardIndex == 1)
		m_RoleGameData.RoleMonthFirstSum += 1;
	else if (RewardIndex == 2)
		m_RoleGameData.RoleMonthSecondSum += 1;
	else if (RewardIndex == 3)
		m_RoleGameData.RoleMonthThreeSum += 1;
	m_pRole->SetRoleIsNeedSave();
}
void RoleGameData::OnHandleRoleJoinTable(bool IsMonth)
{
	m_IsInMonth = IsMonth;
	m_LogJoinTableTime = timeGetTime();
}
void RoleGameData::OnHandleRoleLeaveTable()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//当用户离开桌子的时候 
	//1.判断玩家是否离开比赛的桌子 并且 判断玩家在桌子上的游戏时间 用于统计数据
	DWORD GameSec = (timeGetTime() - m_LogJoinTableTime) / 1000;
	m_RoleGameData.TotalGameSec += GameSec;
	if (!m_IsInMonth)
		m_RoleGameData.NonMonthGameSec += GameSec;
	m_pRole->SetRoleIsNeedSave();
}
void RoleGameData::OnHandleRoleSignUpMonth()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//玩家报名参加比赛
	m_RoleGameData.RoleMonthSigupSum += 1;
	m_pRole->SetRoleIsNeedSave();
}
void RoleGameData::OnHandleRoleComb()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());
	if (!pRole)
	{
		return;
	}
	WORD ComnoSum = pRole->CombCount();
	if (m_RoleGameData.MaxCombSum >= ComnoSum)
		return;
	m_RoleGameData.MaxCombSum = ComnoSum;//记录玩家最大的连击数
	m_pRole->SetRoleIsNeedSave();
}