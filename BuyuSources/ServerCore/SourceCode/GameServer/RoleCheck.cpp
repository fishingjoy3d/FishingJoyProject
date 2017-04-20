#include "StdAfx.h"
#include "RoleCheck.h"
#include "FishServer.h"
#include "RoleEx.h"
RoleCheck::RoleCheck()
{
}
RoleCheck::~RoleCheck()
{

}
bool RoleCheck::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_Role = pRole;
	//return LoadRoleCheckInfo();
	return true;
}
//bool RoleCheck::LoadRoleCheckInfo()
//{
//	//向数据库发送请求  请求加载全部的玩家的数据
//	if (!m_Role)
//	{
//		ASSERT(false);
//		return false;
//	}
//	DBR_Cmd_LoadRoleCheckInfo msg;
//	SetMsgInfo(msg,DBR_LoadRoleCheckInfo, sizeof(DBR_Cmd_LoadRoleCheckInfo));
//	msg.dwUserID = m_Role->GetUserID();
//	g_FishServer.SendNetCmdToDB(&msg);
//	return true;
//}
//void RoleCheck::LoadRoleCheckInfoResult(DBO_Cmd_LoadRoleCheckInfo* pDB)
//{
//	if (!pDB || !m_Role)
//	{
//		ASSERT(false);
//		return;
//	}
//	m_RoleCheckInfo = pDB->CheckInfo;
//	HandleCheckCheckInfo();
//	m_IsLoadDB = true;
//	m_Role->IsLoadFinish();
//}
//bool RoleCheck::HandleCheckCheckInfo()
//{
//	//判断今天 和 最后签到天数 是否为相邻的天数 或者是同一天 
//	time_t timeNow = time(NULL);
//	tm* pTm = localtime(&timeNow);
//	if (!pTm)
//	{
//		ASSERT(false);
//		return false;
//	}
//	pTm->tm_hour = 0;
//	pTm->tm_min = 0;
//	pTm->tm_sec = 0;
//	timeNow = mktime(pTm);//今天的Data数据
//	BYTE CheckMonth = pTm->tm_mon;
//
//	time_t timeCheck = time(NULL);
//	tm* pTmCheck = localtime(&timeCheck);
//	if (!pTmCheck)
//	{
//		ASSERT(false);
//		return false;
//	}
//	pTmCheck->tm_year = m_RoleCheckInfo.bLastLogYear + 100;
//	pTmCheck->tm_mon = m_RoleCheckInfo.bLastLogMonth - 1;
//	pTmCheck->tm_mday = m_RoleCheckInfo.bLastLogDay;
//	pTmCheck->tm_hour = 0;
//	pTmCheck->tm_min = 0;
//	pTmCheck->tm_sec = 0;
//	timeCheck = mktime(pTmCheck);
//
//	bool IsChange = false;
//	//if (timeNow - timeCheck < 0 || timeNow - timeCheck > 24 * 60 * 60)
//	//{
//	//	m_RoleCheckInfo.bCheckSum = 0;//连续签到次数修改为0
//	//	IsChange = true;
//	//}
//	if (CheckMonth != m_RoleCheckInfo.bLastLogMonth - 1)
//	{
//		//月份变化了 
//		m_RoleCheckInfo.MonthCheckInfo = 0;//当月份变化的时候 清空
//		IsChange = true;
//	}
//	return IsChange;
//}
//bool RoleCheck::GetRoleCheckInfo()
//{
//	if (!m_Role)
//	{
//		ASSERT(false);
//		return false;
//	}
//	LC_Cmd_GetRoleCheckInfo msg;
//	SetMsgInfo(msg,GetMsgType(Main_Check, LC_GetRoleCheckInfo), sizeof(LC_Cmd_GetRoleCheckInfo));
//	msg.CheckInfo = m_RoleCheckInfo;
//	m_Role->SendDataToClient(&msg);
//	return true;
//}

//void RoleCheck::OnDayChange()
//{
//	if (!m_Role)
//	{
//		ASSERT(false);
//		return;
//	}
//	if (HandleCheckCheckInfo())
//	{
//		DBR_Cmd_ChangeRoleCheckInfo msgDB;
//		SetMsgInfo(msgDB,DBR_ChangeRoleCheckInfo, sizeof(DBR_Cmd_ChangeRoleCheckInfo));
//		msgDB.dwUserID = m_Role->GetUserID();
//		msgDB.CheckInfo = m_RoleCheckInfo;
//		g_FishServer.SendNetCmdToDB(&msgDB);
//		//通知客户端去
//		LC_Cmd_CheckChange msg;
//		SetMsgInfo(msg,GetMsgType(Main_Check, LC_CheckChange), sizeof(LC_Cmd_CheckChange));
//		msg.CheckInfo = m_RoleCheckInfo;
//		m_Role->SendDataToClient(&msg);
//	}
//}

bool RoleCheck::IsCanCheckNowDay()
{
	//判断今天是否可以签到
	//SYSTEMTIME pNowTime;
	//GetLocalTime(&pNowTime);

	time_t NowTime = time(null);
	NowTime -= g_FishServer.GetFishConfig().GetWriteSec();
	tm Now;
	errno_t Err = localtime_s(&Now, &NowTime);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	DWORD CheckData = m_Role->GetRoleInfo().CheckData;
	DWORD MonthDay = Now.tm_mday;//1-31
	return ((CheckData& (1 << MonthDay)) == 0);
}
BYTE RoleCheck::GetMonthCheckSum()
{
	BYTE MonthSum = 0;
	DWORD CheckData = m_Role->GetRoleInfo().CheckData;
	for (int i = 1; i <= 31; ++i)
	{
		if ((CheckData & (1 << i)) != 0)
			++MonthSum;
	}
	return MonthSum;
}
bool RoleCheck::RoleChecking()
{
	if (!m_Role)
	{
		ASSERT(false);
		return false;
	}
	time_t NowTime = time(null);
	NowTime -= g_FishServer.GetFishConfig().GetWriteSec();
	tm Now;
	errno_t Err = localtime_s(&Now, &NowTime);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	if (!IsCanCheckNowDay())
	{
		//今天不可以签到
		LC_Cmd_CheckNowDay msg;
		SetMsgInfo(msg, GetMsgType(Main_Check, LC_CheckNowDay), sizeof(LC_Cmd_CheckNowDay));
		msg.Result = false;
		m_Role->SendDataToClient(&msg);
		return true;
	}
	//今天可以签到 我们进行设置
	DWORD CheckData = (m_Role->GetRoleInfo().CheckData | (1 << Now.tm_mday));//进行赋值
	m_Role->ChangeRoleCheckData(CheckData);//修改签到数据
	//我们根据数据 给予奖励 
	LC_Cmd_CheckNowDay msgCheck;//签到的结果
	SetMsgInfo(msgCheck, GetMsgType(Main_Check, LC_CheckNowDay), sizeof(LC_Cmd_CheckNowDay));
	msgCheck.Result = true;
	msgCheck.DayRewardID = 0;
	msgCheck.MonthRewardID = 0;
	//1.今天的奖励 
	HashMap<DWORD, WORD>::iterator Iter = g_FishServer.GetFishConfig().GetCheckConfig().CheckDayReward.find(Now.tm_mday);
	if (Iter != g_FishServer.GetFishConfig().GetCheckConfig().CheckDayReward.end())
	{
		m_Role->OnAddRoleRewardByRewardID(Iter->second,TEXT("领取签到当天奖励"));
		msgCheck.DayRewardID = Iter->second;
	}
	//2.连续签到的奖励
	Iter = g_FishServer.GetFishConfig().GetCheckConfig().MonthCheckReward.find(GetMonthCheckSum());
	if (Iter != g_FishServer.GetFishConfig().GetCheckConfig().MonthCheckReward.end())
	{
		m_Role->OnAddRoleRewardByRewardID(Iter->second, TEXT("领取签到月份连续天数奖励"));
		msgCheck.MonthRewardID = Iter->second;
	}
	m_Role->SendDataToClient(&msgCheck);

	m_Role->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Check, true);

	return true;
}
bool RoleCheck::RoleCheckeOnther(BYTE DaySum)
{
	//玩家进行当月的补签
	//1.判断玩家乐币数
	if (!m_Role)
	{
		ASSERT(false);
		return false;
	}
	//补签指定的天数
	time_t NowTime = time(null);
	NowTime -= g_FishServer.GetFishConfig().GetWriteSec();
	tm Now;
	errno_t Err = localtime_s(&Now, &NowTime);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	if (m_Role->GetRoleInfo().dwCurrencyNum < g_FishServer.GetFishConfig().GetCheckConfig().CheckOtherUser)
	{
		LC_Cmd_CheckOtherDay msg;
		SetMsgInfo(msg, GetMsgType(Main_Check, LC_CheckOtherDay), sizeof(LC_Cmd_CheckOtherDay));
		msg.Result = false;
		m_Role->SendDataToClient(&msg);
		return true;
	}
	BYTE MonthCheckSum = GetMonthCheckSum();//本月签到的次数
	if (IsCanCheckNowDay())
		MonthCheckSum += 1;
	if (Now.tm_mday == MonthCheckSum)
	{
		LC_Cmd_CheckOtherDay msg;
		SetMsgInfo(msg, GetMsgType(Main_Check, LC_CheckOtherDay), sizeof(LC_Cmd_CheckOtherDay));
		msg.Result = false;
		m_Role->SendDataToClient(&msg);
		return true;
	}
	else if (MonthCheckSum > Now.tm_mday)
	{
		ASSERT(false);
		return false;
	}
	else if (DaySum >= Now.tm_mday)
	{
		ASSERT(false);
		return false;
	}
	else
	{
		//判断指定天数是否可以补签
		DWORD CheckData = m_Role->GetRoleInfo().CheckData;
		if (DaySum == Now.tm_mday || (CheckData & (1 << DaySum)) != 0)
		{
			LC_Cmd_CheckOtherDay msg;
			SetMsgInfo(msg, GetMsgType(Main_Check, LC_CheckOtherDay), sizeof(LC_Cmd_CheckOtherDay));
			msg.Result = false;
			m_Role->SendDataToClient(&msg);
			return true;
		}
		//进行补签
		//1.扣钱
		m_Role->ChangeRoleCurrency(g_FishServer.GetFishConfig().GetCheckConfig().CheckOtherUser*-1,TEXT("进行补签 扣除乐币"));

		//当前没签到 
		CheckData |= (1 << DaySum);
		m_Role->ChangeRoleCheckData(CheckData);//修改签到数据
		//给予奖励
		LC_Cmd_CheckOtherDay msg;
		SetMsgInfo(msg, GetMsgType(Main_Check, LC_CheckOtherDay), sizeof(LC_Cmd_CheckOtherDay));
		msg.Result = true;
		msg.Day = DaySum;
		msg.DayRewardID = 0;
		msg.MonthRewardID = 0;

		HashMap<DWORD, WORD>::iterator Iter = g_FishServer.GetFishConfig().GetCheckConfig().CheckDayReward.find(DaySum);
		if (Iter != g_FishServer.GetFishConfig().GetCheckConfig().CheckDayReward.end())
		{
			m_Role->OnAddRoleRewardByRewardID(Iter->second, TEXT("领取补签当天奖励"));
			msg.DayRewardID = Iter->second;
		}
		Iter = g_FishServer.GetFishConfig().GetCheckConfig().MonthCheckReward.find(GetMonthCheckSum());
		if (Iter != g_FishServer.GetFishConfig().GetCheckConfig().MonthCheckReward.end())
		{
			m_Role->OnAddRoleRewardByRewardID(Iter->second, TEXT("领取补签月份连续天数奖励"));
			msg.MonthRewardID = Iter->second;
		}
		m_Role->SendDataToClient(&msg);
		return true;
	}
}
bool RoleCheck::GetCheckMessageStates()
{
	return IsCanCheckNowDay();
}