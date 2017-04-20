//用于发送Log记录的文件
#pragma once
enum LogType
{
	LT_Currcey = 1,//钻石
	LT_Medal = 2,//奖牌
	LT_Reward = 3,//奖励
	LT_NiuNiu = 4,//牛牛
	LT_GlobelBag=5,//聚宝盆
	LT_Lottery=6,//抽奖
	LT_Skill = 7,//技能
	LT_Dial = 8,//森林舞会
	LT_Car = 9,
};
typedef void(SendMsgFun)(NetCmd* pCmd);

class DBLogManager
{
public:
	DBLogManager();
	virtual ~DBLogManager();

	//普通的Log记录
	void LogToDB(DWORD dwUserID, LogType Type, int TypeSum,DWORD Param, const TCHAR *pcStr, SendMsgFun pSend);//玩家在游戏中的行为 最普遍的行为
	//充值Log记录
	void LogUserRechargeLogToDB(string OrderStates, string OrderID, DWORD UserID, string ChannelCode, string ChannelOrderID, string ChannelLabel, DWORD ShopItemID, DWORD Price, DWORD FreePrice, DWORD OldGlobelNum, DWORD OldCurrceyNum, DWORD AddGlobel, DWORD AddCurrcey, WORD RewardID, SendMsgFun pSend);
	//实体物品Log记录
	void LogUserEntityItemLogToDB(string OrderStates, DWORD ID, DWORD ItemID, DWORD ItemSum, time_t ShopLogTime, DWORD UserID, string Address, UINT64 Phone, string IDEntity, string Name, string OrderNumber, DWORD MedalSum, DWORD NowMedalSum,DWORD HandleIP, SendMsgFun pSend);
	//玩家上下线Log记录
	void LogRoleOnlineInfo(DWORD dwUserID, bool IsOnlineOrLeaveOnline, string MacAddress,string IpAddress, DWORD GlobelNum, DWORD CurrceyNum, DWORD MedalNum,  SendMsgFun pSend);//玩家上线下线记录
	//玩家进出桌子Log记录
	void LogRoleJoinOrLeaveTableInfo(DWORD dwUserID, DWORD GlobelNum, DWORD CurrceyNum, DWORD MedalNum, BYTE TableID, BYTE MonthID, bool IsJoinOrLeave, SendMsgFun pSend);//玩家进程桌子的记录
	//玩家充值手机话费Log记录
	void LogRolePhonePayLogToDB(string OrderStates, UINT64 OrderID, DWORD UserID, UINT64 PhoneNumber, DWORD FacePrice, SendMsgFun pSend);
	//记录系统每局的游戏状态 
	void LogNiuNiuTableInfoToDB(BYTE BrandValue[MAX_NIUNIU_ClientSum + 1][MAX_NIUNIU_BrandSum], INT64 BrandGlobel, INT64 SystemGlobel, DWORD RoleSum, SendMsgFun pSend);
	//记录玩家使用兑换码
	void LogExChangeInfoToDB(DWORD dwUserID, BYTE ExChangeTypeID, TCHAR ExChange[ExChangeCode_Length + 1], SendMsgFun pSend);
	//记录玩家使用抽奖记录
	void LogLotteryInfoToDB(DWORD dwUserID, BYTE LotteryID, WORD RewardID, SendMsgFun pSend);
	//记录玩家比赛记录
	void LogMonthInfoToDB(DWORD dwUserID, BYTE MonthID, DWORD MonthScore, DWORD MonthIndex, BYTE AddMonthGlobelSum, DWORD MonthSkillSum, WORD MonthRewardID, SendMsgFun pSend);
	//记录玩家森林舞会记录
	void LogDialInfoToDB(DWORD BanderUserID, BYTE AreaData[MAX_DIAL_GameSum], UINT64 AreaGlobel[MAX_DIAL_ClientSum], WORD ResultIndex, INT64 BrandGlobel, INT64 SystemGlobel, DWORD RoleSum, SendMsgFun pSend);
	//记录转盘数据
	void LogCarInfoToDB(DWORD BanderUserID, UINT64 AreaGlobel[MAX_CAR_ClientSum], BYTE ResultIndex, INT64 BrandGlobel, INT64 SystemGlobel, DWORD RoleSum, SendMsgFun pSend);
};
extern DBLogManager g_DBLogManager;