#pragma once
class CRoleEx;
class RoleVip
{
public:
	RoleVip();
	virtual ~RoleVip();
	bool OnInit(CRoleEx* pRole);
	void OnRechargeRMBChange();

	BYTE GetLauncherReBoundNum();//获取子弹反弹的次数
	//bool  IsCanLauncherLocking();//是否可以子弹锁定
	BYTE AddAlmsSum();//可以领取救济金次数
	float AddMonthScoreRate();//比赛积分提升比例
	float AddReChargeRate();//充值提升比例
	float AddAlmsRate();//救济金比例提升
	BYTE GetUseMedalSum();//兑换次数提升
	float AddCatchFishRate();//捕鱼概率提升
private:
	bool OnChangeRoleVipLevel(BYTE VipLevel);
private:
	CRoleEx*			m_pRole;
};