//月卡
#pragma once
class CRoleEx;
class RoleMonthCard
{
public:
	RoleMonthCard();
	virtual ~RoleMonthCard();

	bool OnInit(CRoleEx* pRole);

	bool	IsInMonthCard();

	bool	SetRoleMonthCardInfo(BYTE MonthCardID);//给玩家添加月卡并且保存到数据库
	bool	GetRoleMonthCardReward();

	void	UpdateMonthCard();

	bool IsCanAutoFire();
	float AddLotteryRate();
private:
	CRoleEx*			m_pRole;
};