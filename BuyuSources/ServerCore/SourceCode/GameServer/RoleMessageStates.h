//玩家的 红点状态 由服务器端控制
#pragma once
class CRoleEx;
enum RoleMessageType
{
	RMT_Mail		= 1,//未读的邮件状态
	RMT_WeekRank    = 2,//未领取的排行榜奖励
	RMT_Giff		= 4,//未领取的赠送 
	RMT_Task		= 8,
	RMT_Achievement = 16,
	RMT_Action		= 32,
	RMT_Check		= 64,//判断玩家今天是否可以签到
	RMT_Char		= 128,
	RMT_Relation	= 256,
};
class RoleMessageStates
{
public:
	RoleMessageStates();
	virtual ~RoleMessageStates();

	void	OnInit(CRoleEx* pRole);

	void	OnChangeRoleMessageStates(RoleMessageType Type,bool IsSendToClient = true);
private:
	CRoleEx*				m_pRole;
	bool					m_IsInit;
	DWORD					m_StatesValue;
};