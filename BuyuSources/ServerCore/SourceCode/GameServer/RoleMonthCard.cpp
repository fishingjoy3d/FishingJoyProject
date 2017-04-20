#include "Stdafx.h"
#include "RoleMonthCard.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleMonthCard::RoleMonthCard()
{

}
RoleMonthCard::~RoleMonthCard()
{

}
bool RoleMonthCard::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;

	//判断玩家月卡是否过期了
	if (!IsInMonthCard())
	{
		if (m_pRole->GetRoleInfo().MonthCardID != 0)
		{
			m_pRole->GetRoleInfo().MonthCardID = 0;
			m_pRole->GetRoleInfo().MonthCardEndTime = 0;

			//告诉数据库 月卡已经过期了
			DBR_Cmd_SaveRoleMonthCardInfo msgDB;
			SetMsgInfo(msgDB, DBR_SaveRoleMonthCardInfo, sizeof(DBR_Cmd_SaveRoleMonthCardInfo));
			msgDB.UserID = m_pRole->GetUserID();
			msgDB.MonthCardID = m_pRole->GetRoleInfo().MonthCardID;
			msgDB.MonthCardEndTime = m_pRole->GetRoleInfo().MonthCardEndTime;
			g_FishServer.SendNetCmdToSaveDB(&msgDB);
		}
	}


	return true;
}
void RoleMonthCard::UpdateMonthCard()
{
	//每分钟更新月卡的数据 判断月卡是否过期
	if (m_pRole->GetRoleInfo().MonthCardID != 0 && !IsInMonthCard())
	{
		SetRoleMonthCardInfo(0);
	}
}
bool RoleMonthCard::IsInMonthCard()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	time_t pNow = time(null);
	if (m_pRole->GetRoleInfo().MonthCardID != 0 && pNow < m_pRole->GetRoleInfo().MonthCardEndTime)
		return true;
	else
		return false;
}
bool RoleMonthCard::IsCanAutoFire()
{
	if (!IsInMonthCard())
		return false;
	HashMap<BYTE, tagMonthCardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(m_pRole->GetRoleInfo().MonthCardID);
	if (Iter == g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
	{
		ASSERT(false);
		return false;
	}
	return Iter->second.IsCanAutoFire;
}
float RoleMonthCard::AddLotteryRate()
{
	if (!IsInMonthCard())
		return 1.0f;
	HashMap<BYTE, tagMonthCardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(m_pRole->GetRoleInfo().MonthCardID);
	if (Iter == g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
	{
		ASSERT(false);
		return 1.0f;
	}
	return (Iter->second.AddLotteryRate + 100) / 100 * 1.0f;
}
bool RoleMonthCard::SetRoleMonthCardInfo(BYTE MonthCardID)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	//给玩家添加一张新的月卡
	time_t pNow = time(null);
	if (MonthCardID == 0)
	{
		if (m_pRole->GetRoleInfo().MonthCardID == 0)
			return true;
		//清空玩家月卡数据
		m_pRole->GetRoleInfo().MonthCardID = 0;
		m_pRole->GetRoleInfo().MonthCardEndTime = 0;
	}
	else if (m_pRole->GetRoleInfo().MonthCardID != MonthCardID)
	{
		if (IsInMonthCard())
			return false;
		//替换新的ID
		//设置月卡的信息
		HashMap<BYTE, tagMonthCardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(MonthCardID);
		if (Iter == g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
		{
			ASSERT(false);
			return false;
		}
		//可以设置玩家月卡数据了
		m_pRole->GetRoleInfo().MonthCardID = MonthCardID;
		m_pRole->GetRoleInfo().MonthCardEndTime = pNow + Iter->second.UseLastMin * 60;//月卡结束的时间
	}
	else if (m_pRole->GetRoleInfo().MonthCardID == MonthCardID)
	{
		//叠加 
		HashMap<BYTE, tagMonthCardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(MonthCardID);
		if (Iter == g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
		{
			ASSERT(false);
			return false;
		}
		//可以设置玩家月卡数据了
		m_pRole->GetRoleInfo().MonthCardID = MonthCardID;
		m_pRole->GetRoleInfo().MonthCardEndTime = max(pNow, m_pRole->GetRoleInfo().MonthCardEndTime) + Iter->second.UseLastMin * 60;//月卡结束的时间
	}
	else
	{
		ASSERT(false);
		return false;
	}

	//发送命令到数据库
	DBR_Cmd_SaveRoleMonthCardInfo msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleMonthCardInfo, sizeof(DBR_Cmd_SaveRoleMonthCardInfo));
	msgDB.UserID = m_pRole->GetUserID();
	msgDB.MonthCardID = MonthCardID;
	msgDB.MonthCardEndTime = m_pRole->GetRoleInfo().MonthCardEndTime;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
	//发送命令到客户端
	LC_Cmd_ChangeRoleMonthCardInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleMonthCardInfo), sizeof(LC_Cmd_ChangeRoleMonthCardInfo));
	msg.MonthCardID = MonthCardID;
	msg.MonthCardEndTime = m_pRole->GetRoleInfo().MonthCardEndTime;
	m_pRole->SendDataToClient(&msg);

	LC_Cmd_TableChangeRoleIsInMonthCard msgTable;
	SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleIsInMonthCard), sizeof(LC_Cmd_TableChangeRoleIsInMonthCard));
	msgTable.dwDestUserID = m_pRole->GetUserID();
	msgTable.IsInMonthCard = true;
	m_pRole->SendDataToTable(&msgTable);

	CC_Cmd_ChangeRoleIsInMonthCard msgCenter;
	SetMsgInfo(msgCenter, GetMsgType(Main_Role, CC_ChangeRoleIsInMonthCard), sizeof(CC_Cmd_ChangeRoleIsInMonthCard));
	msgCenter.dwUserID = m_pRole->GetUserID();
	msgCenter.IsInMonthCard = true;
	m_pRole->SendDataToCenter(&msgCenter);

	return true;
}
bool RoleMonthCard::GetRoleMonthCardReward()
{
	if (!IsInMonthCard())
		return false;
	//获取当前月卡的奖励
	time_t pNow = time(null);
	if (g_FishServer.GetFishConfig().GetFishUpdateConfig().IsChangeUpdate(m_pRole->GetRoleInfo().GetMonthCardRewardTime, pNow))
	{
		HashMap<BYTE, tagMonthCardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(m_pRole->GetRoleInfo().MonthCardID);
		if (Iter == g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
			return false;
		WORD RewardID = Iter->second.OnceDayRewardID;
		m_pRole->OnAddRoleRewardByRewardID(RewardID, TEXT("领取月卡礼包奖励"));

		m_pRole->GetRoleInfo().GetMonthCardRewardTime = pNow;//设置最后次领取时间为当前

		DBR_Cmd_SaveRoleGetMonthCardRewardTime msg;
		SetMsgInfo(msg, DBR_SaveRoleGetMonthCardRewardTime, sizeof(DBR_Cmd_SaveRoleGetMonthCardRewardTime));
		msg.UserID = m_pRole->GetUserID();
		msg.LogTime = pNow;
		g_FishServer.SendNetCmdToSaveDB(&msg);
		return true;
	}
	else
	{
		return false;//今天已经领取过了 无法在领取了
	}
}