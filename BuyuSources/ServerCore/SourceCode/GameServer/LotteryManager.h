//±¶œ‰π‹¿Ì∆˜
#pragma once
class LotteryManager
{
public:
	LotteryManager();
	virtual ~LotteryManager();

	void OnRoleGetLotteryReward(DWORD dwUserID,BYTE LotteryID);
};