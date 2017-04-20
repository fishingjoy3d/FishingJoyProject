#include "Stdafx.h"
#include "RoleRank.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleRank::RoleRank()
{
	m_pRole = null;
	m_RankInfo.clear();
	m_IsLoadDB = false;
	m_IsSendToClient = false;
}
RoleRank::~RoleRank()
{

}
bool RoleRank::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	m_RankInfo.clear();
	m_IsLoadDB = false;
	return true;
}
void RoleRank::OnLoadWeekRankInfoResult(DBO_Cmd_LoadWeekRankInfo* pMsg)
{
	if (!pMsg || !m_pRole)
	{
		ASSERT(false);
		return;
	}
	if ((pMsg->States & MsgBegin) != 0)
	{
		m_RankInfo.clear();
	}
	for (WORD i = 0; i < pMsg->Sum; ++i)
	{
		m_RankInfo.insert(HashMap<BYTE, tagRankWeekRankClient>::value_type(pMsg->Array[i].RankID,pMsg->Array[i]));
	}
	if ((pMsg->States & MsgEnd) != 0)
	{
		m_IsLoadDB = true;
		if (m_IsSendToClient)
		{
			SendAllRankInfoToClient();//在加载期间 客户端请求了的
			m_IsSendToClient = false;
		}

		m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_WeekRank);

		if (pMsg->IsInit)
			m_pRole->IsLoadFinish();
	}
}
void RoleRank::SendAllRankInfoToClient()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!m_IsLoadDB)
	{
		m_IsSendToClient = true;
		return;
	}
	//将玩家排行榜的数据发送到客户端去
	DWORD PageSize = sizeof(LC_Cmd_GetWeekRankInfo)+(m_RankInfo.size() - 1)*sizeof(tagRankWeekRankClient);
	LC_Cmd_GetWeekRankInfo* msg = (LC_Cmd_GetWeekRankInfo*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Rank, LC_GetWeekRankInfo));
	HashMap<BYTE, tagRankWeekRankClient>::iterator Iter = m_RankInfo.begin();
	for (WORD i = 0; Iter != m_RankInfo.end(); ++Iter,++i)
	{
		msg->Array[i] = Iter->second;
	}
	std::vector<LC_Cmd_GetWeekRankInfo*> pVec;
	SqlitMsg(msg, PageSize, m_RankInfo.size(),true, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_GetWeekRankInfo*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			m_pRole->SendDataToClient(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
}
bool RoleRank::GetRankReward(BYTE RankID)
{
	if (!m_pRole || !m_IsLoadDB)
	{
		ASSERT(false);
		return false;
	}
	//获取排行榜的奖励
	HashMap<BYTE, tagRankWeekRankClient>::iterator Iter = m_RankInfo.find(RankID);
	if (Iter == m_RankInfo.end())
	{
		ASSERT(false);
		return false;
	}
	if (Iter->second.IsReward || Iter->second.RankID != RankID)
	{
		ASSERT(false);
		return false;
	}
	HashMap<BYTE, tagRankConfig>::iterator IterRank = g_FishServer.GetFishConfig().GetRankConfig().m_RankMap.find(RankID);
	if (IterRank == g_FishServer.GetFishConfig().GetRankConfig().m_RankMap.end())
	{
		ASSERT(false);
		return false;
	}
	HashMap<BYTE, tagRankReward>::iterator IterReward = IterRank->second.RewardMap.find(Iter->second.RankIndex);
	if (IterReward == IterRank->second.RewardMap.end())
	{
		ASSERT(false);
		return false;
	}
	Iter->second.IsReward = true;
	m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_WeekRank);
	//发送命令到数据库去 立即保存
	DBR_Cmd_ChangeWeekInfo msgDB;
	SetMsgInfo(msgDB, DBR_ChangeWeekInfo, sizeof(DBR_Cmd_ChangeWeekInfo));
	msgDB.dwUserID = m_pRole->GetUserID();
	msgDB.RankInfo = Iter->second;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
	m_pRole->OnAddRoleRewardByRewardID(IterReward->second.RewardID,TEXT("领取排行榜奖励"));

	LC_Cmd_GetRankReward msg;
	SetMsgInfo(msg, GetMsgType(Main_Rank, LC_GetRankReward), sizeof(LC_Cmd_GetRankReward));
	msg.RankID = Iter->first;
	m_pRole->SendDataToClient(&msg);

	return true;
}
void RoleRank::UpdateByDay()
{
	//4点钟 更新
	time_t Nowtime = time(NULL) - g_FishServer.GetFishConfig().GetWriteSec();
	tm pNowTime;
	errno_t Error = localtime_s(&pNowTime, &Nowtime);
	if (Error != 0)
	{
		ASSERT(false);
		return;
	}
	if (pNowTime.tm_mday == g_FishServer.GetFishConfig().GetRankConfig().RankWeekDay)
	{
		//月排行榜更新了
		m_RankInfo.clear();
		m_IsLoadDB = false;

		DBR_Cmd_LoadWeekRankInfo msg;
		SetMsgInfo(msg, DBO_LoadWeekRankInfo, sizeof(DBO_Cmd_LoadWeekRankInfo));
		msg.dwUserID = m_pRole->GetUserID();
		msg.IsInit = false;
		g_FishServer.SendNetCmdToDB(&msg);
	}
}
bool RoleRank::GetRankMessageStates()
{
	HashMap<BYTE, tagRankWeekRankClient>::iterator Iter = m_RankInfo.begin();
	for (; Iter != m_RankInfo.end(); ++Iter)
	{
		if (!Iter->second.IsReward)
			return true;
	}
	return false;
}