//ÅÅÐÐ°ñÐÞ¸Ä
#pragma once
class CRoleEx;
class RoleRank
{
public:
	RoleRank();
	virtual ~RoleRank();

	bool	OnInit(CRoleEx* pRole);
	void	OnLoadWeekRankInfoResult(DBO_Cmd_LoadWeekRankInfo* pMsg);
	void	SendAllRankInfoToClient();
	void	UpdateByDay();
	bool	GetRankReward(BYTE RankID);

	bool	IsLoadDB(){ return m_IsLoadDB; }

	bool	GetRankMessageStates();
private:
	CRoleEx*								m_pRole;
	bool									m_IsLoadDB;
	HashMap<BYTE, tagRankWeekRankClient>	m_RankInfo;
	bool									m_IsSendToClient;
};