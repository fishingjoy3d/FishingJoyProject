#include "Stdafx.h"
#include "RoleRate.h"
#include "FishServer.h"
RoleRate::RoleRate()
{

}
RoleRate::~RoleRate()
{

}
bool RoleRate::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;

	//初始化玩家的倍率
	DWORD InitRateValue = g_FishServer.GetFishConfig().GetSystemConfig().InitRateValue;
	bool IsChange = false;
	for (BYTE i = 0; i < 32; ++i)//玩家的默认倍率 只可以是前32个
	{
		if ((InitRateValue& (1 << i)) != 0 && !int256Handle::GetBitStates(m_pRole->GetRoleInfo().RateValue, i))
		{
			IsChange = true;
			int256Handle::SetBitStates(m_pRole->GetRoleInfo().RateValue, i, true);
		}
		else if ((InitRateValue& (1 << i)) == 0)
			break;
	}
	
	//倍率计算完毕后 进行处理
	m_MinRateIndex = 0;
	m_MaxRateIndex = 0;
	for (BYTE i = 0; i < g_FishServer.GetTableManager()->GetGameConfig()->RateCount(); ++i)
	{
		if (IsCanUseRateIndex(i))
		{
			if (i >= m_MaxRateIndex)
				m_MaxRateIndex = i;

			if (m_MinRateIndex >= i)
				m_MinRateIndex = i;
		}
		else
		{
			break;
		}
	}
	GetInitRateReward();//判断玩家是否需要补偿倍率
	//将int256 数据清空
	for (BYTE i = m_MaxRateIndex + 1; i < 255; ++i)//将后面的情况 防止异常出现
	{
		if (IsCanUseRateIndex(i))
		{
			int256Handle::SetBitStates(m_pRole->GetRoleInfo().RateValue, i, false);
			IsChange = true;
		}
	}
	if (IsChange)
	{
		//保存玩家的倍率到数据库去
		DBR_Cmd_SaveRoleRateValue msgDB;
		SetMsgInfo(msgDB, DBR_SaveRoleRateValue, sizeof(DBR_Cmd_SaveRoleRateValue));
		msgDB.UserID = m_pRole->GetUserID();
		msgDB.RateValue = m_pRole->GetRoleInfo().RateValue;
		msgDB.MaxRateValue = m_MaxRateIndex;
		g_FishServer.SendNetCmdToSaveDB(&msgDB);
	}
	else
	{
		//保存玩家最大解锁倍率 防止玩家以前未登陆过的 处理
		DBR_Cmd_SaveRoleMaxRateValue msgDB;
		SetMsgInfo(msgDB, DBR_SaveRoleMaxRateValue, sizeof(DBR_Cmd_SaveRoleMaxRateValue));
		msgDB.dwUserID = m_pRole->GetUserID();
		msgDB.MaxRate = m_MaxRateIndex;
		g_FishServer.SendNetCmdToSaveDB(&msgDB);
	}
	return true;
}
void RoleRate::GetInitRateReward()
{
	time_t Nowtime = m_pRole->GetLastOnLineTime();
	time_t LogTime = g_FishServer.GetFishConfig().GetSystemConfig().RateInitRewardTime;
	if (Nowtime < LogTime)
	{
		tagRoleMail	MailInfo;
		MailInfo.bIsRead = false;
		//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的
		MailInfo.MailID = 0;
		MailInfo.SendTimeLog = time(NULL);
		MailInfo.SrcFaceID = 0;
		TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
		MailInfo.SrcUserID = 0;//系统发送
		MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
		DBR_Cmd_AddUserMail msg;
		SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
		msg.dwDestUserID = m_pRole->GetUserID();
		//玩家是之前登陆的 进行处理
		for (BYTE i = m_MinRateIndex; i <= m_MaxRateIndex; ++i)
		{
			WORD RewardID = g_FishServer.GetTableManager()->GetGameConfig()->UnlockRateReward(i);
			if (RewardID == 0)
				continue;
			WORD RateSum = g_FishServer.GetTableManager()->GetGameConfig()->BulletMultiple(i);
			//发送邮件
			_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("解锁%u倍倍率奖励补偿"), RateSum);
			MailInfo.RewardID = RewardID;
			MailInfo.RewardSum = 1;
			msg.MailInfo = MailInfo;
			g_FishServer.SendNetCmdToDB(&msg);
		}
	}
}
bool RoleRate::OnChangeRoleRate(BYTE AddRateIndex)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	//修改玩家的倍率
	if (AddRateIndex <= m_MaxRateIndex)
	{
		ASSERT(false);
		return false;
	}
	//判断是否可以使用
	if (IsCanUseRateIndex(AddRateIndex))
	{
		ASSERT(false);
		return false;
	}
	//开启新的倍率
	for (BYTE i = m_MaxRateIndex + 1; i <= AddRateIndex; ++i)
	{
		int256Handle::SetBitStates(m_pRole->GetRoleInfo().RateValue, i, 1);

		WORD RewardID = g_FishServer.GetTableManager()->GetGameConfig()->UnlockRateReward(i);
		m_pRole->OnAddRoleRewardByRewardID(RewardID, TEXT("解锁倍率获得奖励"));
	}

	m_MaxRateIndex = AddRateIndex;//可以使用的最高倍率
		
	LC_Cmd_ChangeRoleRateValue msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleRateValue), sizeof(LC_Cmd_ChangeRoleRateValue));
	msg.RateValue = m_pRole->GetRoleInfo().RateValue;
	msg.OpenRateIndex = m_MaxRateIndex;
	m_pRole->SendDataToClient(&msg);
	//发送命令到数据库去
	DBR_Cmd_SaveRoleRateValue msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleRateValue, sizeof(DBR_Cmd_SaveRoleRateValue));
	msgDB.UserID = m_pRole->GetUserID();
	msgDB.RateValue = m_pRole->GetRoleInfo().RateValue;
	msgDB.MaxRateValue = m_MaxRateIndex;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool RoleRate::IsCanUseRateIndex(BYTE RateIndex)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	if (m_pRole->IsRobot())
		return true;
	return int256Handle::GetBitStates(m_pRole->GetRoleInfo().RateValue, RateIndex);//判断指定倍率是否可以使用
}
BYTE RoleRate::GetCanShowMaxRate()
{
	//获取可以显示的最高倍率
	BYTE MaxRate = g_FishServer.GetTableManager()->GetGameConfig()->RateCount() - 1;
	BYTE ShowRate = m_MaxRateIndex + 1;
	return min(MaxRate, ShowRate);
}
void RoleRate::ResetRateInfo()
{
	//倍率数据已经乱了重新设置下
	m_MinRateIndex = 0;
	m_MaxRateIndex = 0;
	for (BYTE i = 0; i < g_FishServer.GetTableManager()->GetGameConfig()->RateCount(); ++i)
	{
		if (IsCanUseRateIndex(i))
		{
			if (i >= m_MaxRateIndex)
				m_MaxRateIndex = i;

			if (m_MinRateIndex >= i)
				m_MinRateIndex = i;
		}
		else
		{
			break;
		}
	}
}