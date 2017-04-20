#include "Stdafx.h"
#include "LotteryManager.h"
#include "FishServer.h"
#include "..\CommonFile\DBLogManager.h"
extern void SendLogDB(NetCmd* pCmd);
LotteryManager::LotteryManager()
{

}
LotteryManager::~LotteryManager()
{

}
void LotteryManager::OnRoleGetLotteryReward(DWORD dwUserID, BYTE LotteryID)
{
	//玩家获得奖励
	CRoleEx* pRole = g_FishServer.GetRoleManager()->QueryUser(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, tagLotteryOnce>::iterator Iter = g_FishServer.GetFishConfig().GetLotteryConfig().LotteryMap.find(LotteryID);
	if (Iter == g_FishServer.GetFishConfig().GetLotteryConfig().LotteryMap.end())
	{
		ASSERT(false);
		return;
	}
	if (pRole->GetRoleInfo().LotteryFishSum < g_FishServer.GetFishConfig().GetLotteryConfig().MaxLotteryFishSum)
	{
		//ASSERT(false);
		LC_Cmd_GetLotteryReward msg;
		SetMsgInfo(msg, GetMsgType(Main_Lottery, LC_GetLotteryReward), sizeof(LC_Cmd_GetLotteryReward));
		msg.LotteryID = LotteryID;
		msg.RewardID = 0;
		msg.Result = false;
		pRole->SendDataToClient(&msg);
		return;
	}
	if (pRole->GetRoleInfo().LotteryScore < Iter->second.NeedUseScore)
	{
		LC_Cmd_GetLotteryReward msg;
		SetMsgInfo(msg, GetMsgType(Main_Lottery, LC_GetLotteryReward), sizeof(LC_Cmd_GetLotteryReward));
		msg.LotteryID = LotteryID;
		msg.RewardID = 0;
		msg.Result = false;
		pRole->SendDataToClient(&msg);
		return;
	}
	//都够的情况下 我们进行处理
	WORD RewardID = 0;
	DWORD RankValue = (RandUInt() % Iter->second.TotalRate);
	RankValue = min((DWORD)pRole->GetRoleMonthCard().AddLotteryRate() * RankValue * RankValue, Iter->second.TotalRate - 1);
	vector<tagLotteryReward>::iterator IterReward = Iter->second.RewardVec.begin();
	for (; IterReward != Iter->second.RewardVec.end(); ++IterReward)
	{
		if (RankValue <= IterReward->Rate)
		{
			//当前奖励了
			RewardID = IterReward->RewardID;
			break;
		}
	}
	if (RewardID == 0)
	{
		ASSERT(false);
		LC_Cmd_GetLotteryReward msg;
		SetMsgInfo(msg, GetMsgType(Main_Lottery, LC_GetLotteryReward), sizeof(LC_Cmd_GetLotteryReward));
		msg.LotteryID = LotteryID;
		msg.RewardID = RewardID;
		msg.Result = false;
		pRole->SendDataToClient(&msg);
	}
	else
	{
		//扣除玩家积分
		pRole->OnClearRoleLotteryInfo();//清空玩家抽奖的数据
		//添加玩家奖励
		pRole->OnAddRoleRewardByRewardID(RewardID, TEXT("抽奖获得奖励"));

		g_DBLogManager.LogToDB(dwUserID, LT_Lottery, LotteryID, RewardID, TEXT("玩家进行抽奖"), SendLogDB);

		g_DBLogManager.LogLotteryInfoToDB(dwUserID, LotteryID, RewardID, SendLogDB);

		//发送命令到客户端
		LC_Cmd_GetLotteryReward msg;
		SetMsgInfo(msg, GetMsgType(Main_Lottery, LC_GetLotteryReward), sizeof(LC_Cmd_GetLotteryReward));
		msg.LotteryID = LotteryID;
		msg.RewardID = RewardID;
		msg.Result = true;
		pRole->SendDataToClient(&msg);
	}
}
