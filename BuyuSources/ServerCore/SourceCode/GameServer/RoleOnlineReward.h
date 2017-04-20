//玩家在线奖励数据
#pragma once
class CRoleEx;
class RoleOnlineReward
{
public:
	RoleOnlineReward();
	virtual ~RoleOnlineReward();

	void OnGetOnlineReward(CRoleEx* pRole, BYTE ID);//获取在线奖励
};