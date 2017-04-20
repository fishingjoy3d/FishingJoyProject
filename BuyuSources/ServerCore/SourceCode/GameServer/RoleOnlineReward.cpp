#include "Stdafx.h"
#include "RoleOnlineReward.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleOnlineReward::RoleOnlineReward()
{

}
RoleOnlineReward::~RoleOnlineReward()
{

}
void RoleOnlineReward::OnGetOnlineReward(CRoleEx* pRole,BYTE ID)
{
	LC_Cmd_GetOnlineReward msg;
	SetMsgInfo(msg,GetMsgType(Main_OnlineReward, LC_GetOnlineReward), sizeof(LC_Cmd_GetOnlineReward));
	msg.RewardID = ID;
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	WORD OnLineMin = ConvertDWORDToWORD(pRole->GetRoleOnlineSec() / 60);
	//msg.OnLineSec = pRole->GetRoleOnlineSec() - (pRole->GetRoleInfo().OnlineMin * 60);//获取今天在线的秒数
	//msg.DBOnLineMin = pRole->GetRoleInfo().OnlineMin;
	HashMap<BYTE, tagOnceOnlienReward>::iterator Iter = g_FishServer.GetFishConfig().GetOnlineRewardConfig().m_OnlineRewardMap.find(ID);
	if (Iter == g_FishServer.GetFishConfig().GetOnlineRewardConfig().m_OnlineRewardMap.end())
	{
		ASSERT(false);
		msg.Result = false;
		pRole->SendDataToClient(&msg);
		return;
	}
	if (pRole->GetRoleInfo().OnlineRewardStates & (1 << (ID - 1)) || Iter->second.OnlineMin > OnLineMin)
	{
		//ASSERT(false);
		msg.Result = false;
		pRole->SendDataToClient(&msg);
		return;
	}
	//需要记录玩家是否领取奖励了 配置数量不限制的情况下 我们需要循环变量玩家的奖励 32 DWORD  
	//在线时间足够  并且 未领取过奖励
	pRole->OnAddRoleRewardByRewardID(Iter->second.RewardID,TEXT("领取在线奖励"));
	//2.奖励发送完毕后设置标记
	pRole->ChangeRoleOnlineRewardStates(pRole->GetRoleInfo().OnlineRewardStates | (1 << (ID - 1)));
	//3.发送命令到客户端去 玩家领取在线奖励成功了
	msg.Result = true;
	msg.States = pRole->GetRoleInfo().OnlineRewardStates;
	pRole->SendDataToClient(&msg);
	return;
}