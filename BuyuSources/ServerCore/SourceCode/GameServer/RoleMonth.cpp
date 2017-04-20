#include "Stdafx.h"
#include "FishServer.h"
#include "RoleMonth.h"
RoleMont::RoleMont()
{
	ClearMonthInfo();
	m_SignUpMonthVec.clear();
	m_IsLoadSignUpFinish = false;
	m_IsNeedSendToClient = false;
}
RoleMont::~RoleMont()
{
	OnPlayerLeaveTable();//玩家离开桌子的时候 上传玩家最新的比赛数据到中央服务器去
	ClearMonthInfo();
}
bool RoleMont::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	return true;
}
void RoleMont::OnLoadMonthInfo(tagRoleMonthInfo* pInfo)
{
	if (!pInfo || !m_pRole)
	{
		ASSERT(false);
		return;
	}
	m_MonthInfo = *pInfo;
	m_MonthID = pInfo->bMonthID;
	HashMap<BYTE, tagMonthConfig>::iterator Iter= g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (Iter == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
}
void RoleMont::ClearMonthInfo()
{
	//m_MonthConfig = NULL;
	//m_pRole = NULL;
	m_MonthID = 0;
	m_IsCenterChange = false;
	m_CenterUpdateTime = 0;
	ZeroMemory(&m_MonthInfo, sizeof(m_MonthInfo));
}
bool RoleMont::IsInMonthTable()
{
	return (m_MonthID != 0);
}
bool RoleMont::OnChangeRoleMonthGlobel(int MonthGlobel, bool IsSendToClient)
{
	if (MonthGlobel == 0)
		return true;
	//修改玩家的游戏金币
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	if (!IsInMonthTable())
		return false;
	/*if (m_MonthInfo.dwMonthGlobel + MonthGlobel < 0)
		return false;*/
	if (!CheckChangeDWORDValue(m_MonthInfo.dwMonthGlobel, MonthGlobel))
		return false;
	m_MonthInfo.dwMonthGlobel += MonthGlobel;

	//GC_Cmd_ChangeUserGlobel msg;
	//SetMsgInfo(msg,GetMsgType(Main_Month, GC_ChangeUserGlobel), sizeof(GC_Cmd_ChangeUserGlobel));
	//msg.dwUserID = m_pRole->GetUserID();
	//msg.MonthID = m_MonthID;
	//msg.dwMonthGlobel = m_MonthInfo.dwMonthGlobel;
	//m_pRole->SendDataToCenter(&msg);

	m_IsCenterChange = true;

	if (IsSendToClient)
	{
		LC_Cmd_SetRoleMonthInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Month, LC_SetRoleMonthInfo), sizeof(LC_Cmd_SetRoleMonthInfo));
		msg.dwUserID = m_pRole->GetUserID();
		msg.MonthID = m_MonthID;
		msg.MonthGlobel = MonthGlobel;//m_MonthInfo.dwMonthGlobel;
		msg.MonthSocre = 0;//m_MonthInfo.dwMonthScore;
		m_pRole->SendDataToClient(&msg);
		m_pRole->SendDataToTable(&msg);
	}

	return true;
}
bool RoleMont::OnChangeRoleMonthPoint(int Point, bool IsSendToClient)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	if(!IsInMonthTable())
		return false;
	if (Point == 0)
		return true;
	if (m_MonthInfo.dwMonthScore + Point < 0)
		return false;
	m_MonthInfo.dwMonthScore += Point;

	m_IsCenterChange = true;

	if (IsSendToClient)
	{
		LC_Cmd_SetRoleMonthInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Month, LC_SetRoleMonthInfo), sizeof(LC_Cmd_SetRoleMonthInfo));
		msg.dwUserID = m_pRole->GetUserID();
		msg.MonthID = m_MonthID;
		msg.MonthGlobel = 0;//m_MonthInfo.dwMonthGlobel;
		msg.MonthSocre = Point;//m_MonthInfo.dwMonthScore;
		m_pRole->SendDataToClient(&msg);
		m_pRole->SendDataToTable(&msg);
	}
	return true;
}
void RoleMont::OnChangeRoleMonthIndex(DWORD dwIndex,DWORD UpperSocre)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!IsInMonthTable())
		return;
	if (m_MonthInfo.dwIndex == dwIndex)
	{
		ASSERT(false);
		return;
	}
	m_MonthInfo.dwIndex = dwIndex;

	LC_Cmd_UserChangeIndex msg;
	SetMsgInfo(msg,GetMsgType(Main_Month, LC_UserChangeIndex), sizeof(LC_Cmd_UserChangeIndex));
	msg.dwUserID = m_pRole->GetUserID();
	msg.dwIndex = dwIndex;
	msg.dwUpperSocre = UpperSocre;
	m_pRole->SendDataToClient(&msg);
	m_pRole->SendDataToTable(&msg);
}
bool RoleMont::IsCanAddMonthGlobel()
{
	if (!m_pRole /*|| !m_MonthConfig*/ || !IsInMonthTable())
	{
		return false;
	}
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		return false;
	}

	HashMap<BYTE, tagMonthConvert>::iterator Iter = IterConfig->second.m_ConvertMap.find(m_MonthInfo.wUserAddGlobelNum + 1);
	if (Iter == IterConfig->second.m_ConvertMap.end())
	{
		return false;
	}
	if (!m_pRole->IsRobot())
	{
		if (static_cast<UINT64>(Iter->second.LostGlobel)  > MAXUINT32 ||
			static_cast<UINT64>(Iter->second.LostMadle)  > MAXUINT32 ||
			static_cast<UINT64>(Iter->second.LostCurrey)  > MAXUINT32
			)
		{
			return false;
		}

		if (!m_pRole->LostUserMoney(Iter->second.LostGlobel, Iter->second.LostMadle, Iter->second.LostCurrey, TEXT("比赛续币 扣除钻石")))
		{
			return false;
		}
	}
	return true;
}
void RoleMont::OnRoleAddMonthGlobel()
{
	if (!m_pRole /*|| !m_MonthConfig*/ || !IsInMonthTable())
	{
		ASSERT(false);
		return;
	}

	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}

	HashMap<BYTE, tagMonthConvert>::iterator Iter = IterConfig->second.m_ConvertMap.find(m_MonthInfo.wUserAddGlobelNum + 1);
	if (Iter == IterConfig->second.m_ConvertMap.end())
	{
		ASSERT(false);
		return;
	}
	//扣除续币的花费
	if (!m_pRole->IsRobot())
	{
		if (static_cast<UINT64>(Iter->second.LostGlobel)  > MAXUINT32 ||
			static_cast<UINT64>(Iter->second.LostMadle)  > MAXUINT32 ||
			static_cast<UINT64>(Iter->second.LostCurrey)  > MAXUINT32
			)
		{
			ASSERT(false);
			return;
		}

		if (!m_pRole->LostUserMoney(Iter->second.LostGlobel, Iter->second.LostMadle, Iter->second.LostCurrey, TEXT("比赛续币 扣除钻石")))
		{
			ASSERT(false);
			return;
		}
	}

	m_MonthInfo.wUserAddGlobelNum += 1;
	m_MonthInfo.dwMonthGlobel += Iter->second.AddMonthGlobel;

	GC_Cmd_ChangeUserAddMonthGlobelNum msg;
	SetMsgInfo(msg,GetMsgType(Main_Month, GC_ChangeUserAddMonthGlobelNum), sizeof(GC_Cmd_ChangeUserAddMonthGlobelNum));
	msg.dwUserID = m_pRole->GetUserID();
	msg.Month = m_MonthID;
	m_pRole->SendDataToCenter(&msg);

	LC_Cmd_ChangeUserAddMonthGlobelNum msgClient;
	SetMsgInfo(msgClient,GetMsgType(Main_Month, LC_ChangeUserAddMonthGlobelNum), sizeof(LC_Cmd_ChangeUserAddMonthGlobelNum));
	msgClient.dwUserID = m_pRole->GetUserID();
	m_pRole->SendDataToClient(&msgClient);
	m_pRole->SendDataToTable(&msgClient);
}
void RoleMont::OnMonthEnd(BYTE Month, DWORD Index, DWORD MonthScores, DWORD VipMonthScores)
{
	//当中央服务器发送 当前比赛结束的时候 玩家直接退出当前的桌子 返回大厅
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}

	std::vector<BYTE>::iterator Iter=find(m_SignUpMonthVec.begin(), m_SignUpMonthVec.end(), Month);//从报名列表里面移除掉
	if (Iter != m_SignUpMonthVec.end())
		m_SignUpMonthVec.erase(Iter);

	LC_Cmd_UserMonthEnd msg;
	SetMsgInfo(msg,GetMsgType(Main_Month, LC_UserMonthEnd), sizeof(LC_Cmd_UserMonthEnd));
	msg.MonthID = Month;
	msg.MonthIndex = Index;
	msg.MonthScores = MonthScores;
	msg.VipScores = VipMonthScores;
	m_pRole->SendDataToClient(&msg);
	if (m_MonthInfo.bMonthID != Month)
		return;
	g_FishServer.GetTableManager()->OnPlayerLeaveTable(m_pRole->GetUserID());
	ClearMonthInfo();
}
void RoleMont::OnPlayerLeaveTable()
{
	if (!IsInMonthTable())
		return;
	if (m_IsCenterChange)
	{
		m_IsCenterChange = false;
		//玩家离开桌子之前 立刻上传次 自己的积分 金币
		GC_Cmd_ChangeUserMonthInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Month, GC_ChangeUserMonthInfo), sizeof(GC_Cmd_ChangeUserMonthInfo));
		msg.IsEnd = false;
		msg.dwUserID = m_pRole->GetUserID();
		msg.MonthID = m_MonthID;
		msg.dwMonthGlobel = m_MonthInfo.dwMonthGlobel;
		msg.dwPoint = m_MonthInfo.dwMonthScore;
		msg.SkillSum = m_MonthInfo.SkillSum;
		m_pRole->SendDataToCenter(&msg);
	}
	ClearMonthInfo();//当玩家离开桌子的时候 清空比赛信息
}
void RoleMont::OnUpdate(DWORD dwTimer)
{
	if (!m_IsCenterChange)
		return;
	if (!IsInMonthTable())
		return;
	if (m_CenterUpdateTime != 0 && dwTimer - m_CenterUpdateTime < 5000)
		return;//10秒向服务器同步一次数据
	m_IsCenterChange = false;
	m_CenterUpdateTime = dwTimer;
	//将当前玩家的积分 金币上传到服务器端去
	GC_Cmd_ChangeUserMonthInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_Month, GC_ChangeUserMonthInfo), sizeof(GC_Cmd_ChangeUserMonthInfo));
	msg.IsEnd = false;
	msg.dwUserID = m_pRole->GetUserID();
	msg.MonthID = m_MonthID;
	msg.dwMonthGlobel = m_MonthInfo.dwMonthGlobel;
	msg.dwPoint = m_MonthInfo.dwMonthScore;
	msg.SkillSum = m_MonthInfo.SkillSum;
	m_pRole->SendDataToCenter(&msg);
}
void RoleMont::OnUpMonthInfo(BYTE MonthID)
{
	if (!m_IsCenterChange)
		return;
	if (m_MonthID != MonthID)
		return;
	GC_Cmd_ChangeUserMonthInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_Month, GC_ChangeUserMonthInfo), sizeof(GC_Cmd_ChangeUserMonthInfo));
	msg.IsEnd = true;
	msg.dwUserID = m_pRole->GetUserID();
	msg.MonthID = m_MonthID;
	msg.dwMonthGlobel = m_MonthInfo.dwMonthGlobel;
	msg.dwPoint = m_MonthInfo.dwMonthScore;
	msg.SkillSum = m_MonthInfo.SkillSum;
	m_pRole->SendDataToCenter(&msg);

	m_IsCenterChange = false;
	m_CenterUpdateTime = timeGetTime();
}
void RoleMont::OnLoadSignUpMonthInfo(CG_Cmd_LoadRoleSignUp* pMsg)
{
	//加载玩家比赛报名数据
	if (pMsg->dwUserID != m_pRole->GetUserID())
	{
		ASSERT(false);
		return;
	}
	if ((pMsg->States & MsgBegin) != 0)
	{
		m_SignUpMonthVec.clear();
	}
	if (pMsg->Sum != 0)
	{
		for (BYTE i = 0; i < pMsg->Sum; ++i)
		{
			m_SignUpMonthVec.push_back(pMsg->Array[i]);
		}
	}
	if ((pMsg->States & MsgEnd) != 0)
	{
		//加载完毕 
		m_IsLoadSignUpFinish = true;
		if (m_IsNeedSendToClient)
		{
			m_IsNeedSendToClient = false;
			SendAllSignUpInfoToClient();
		}
	}
}
void RoleMont::OnSignUpSucess(BYTE Month)
{
	m_SignUpMonthVec.push_back(Month);
}
void RoleMont::OnResetMonth(BYTE Month)
{
	//重置比赛
	if (m_MonthID != Month)
		return;
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	m_MonthInfo.dwMonthScore = 0;
	m_MonthInfo.dwMonthGlobel = IterConfig->second.InitMonthGlobel;
	m_MonthInfo.wUserAddGlobelNum = 0;
	m_MonthInfo.SkillSum = 0;
}
void RoleMont::OnUseMonthSkill(BYTE Month)
{
	if (m_MonthID != Month)
		return;
	m_MonthInfo.SkillSum += 1;
}
void RoleMont::SendAllSignUpInfoToClient()
{
	if (!m_IsLoadSignUpFinish)
	{
		m_IsNeedSendToClient = true;
		return;
	}
	//将玩家报名的数据发送到客户端去
	DWORD PageSize = sizeof(LC_Cmd_LoadRoleSignUpInfo)+sizeof(BYTE)*(m_SignUpMonthVec.size() - 1);
	LC_Cmd_LoadRoleSignUpInfo* msg = (LC_Cmd_LoadRoleSignUpInfo*)malloc(PageSize);
	msg->SetCmdType(GetMsgType(Main_Month, LC_LoadRoleSignUpInfo));
	std::vector<BYTE>::iterator Iter = m_SignUpMonthVec.begin();
	for (WORD i = 0; Iter != m_SignUpMonthVec.end(); ++Iter, ++i)
	{
		msg->Array[i] = (*Iter);
	}
	std::vector<LC_Cmd_LoadRoleSignUpInfo*> pVec;
	SqlitMsg(msg, PageSize, m_SignUpMonthVec.size(),true, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_LoadRoleSignUpInfo*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			m_pRole->SendDataToClient(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
}