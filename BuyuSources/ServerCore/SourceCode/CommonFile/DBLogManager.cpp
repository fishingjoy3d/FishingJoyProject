#include "Stdafx.h"
#include "DBLogManager.h"
DBLogManager::DBLogManager()
{

}
DBLogManager::~DBLogManager()
{

}
void DBLogManager::LogToDB(DWORD dwUserID, LogType Type, int TypeSum, DWORD Param, const TCHAR *pcStr, SendMsgFun pSend)//记录到LOG去
{
	DBR_Cmd_LogInfo msg;
	SetMsgInfo(msg, DBR_LogInfo, sizeof(DBR_Cmd_LogInfo));	
	TCHARCopy(msg.Info, CountArray(msg.Info), pcStr, _tcslen(pcStr));
	msg.UserID = dwUserID;
	msg.Type = Type;
	msg.TypeSum = TypeSum;
	msg.Param = Param;
	pSend(&msg);
}
void DBLogManager::LogUserRechargeLogToDB(string OrderStates, string OrderID, DWORD UserID, string ChannelCode, string ChannelOrderID, string ChannelLabel, 
				DWORD ShopItemID, DWORD Price,DWORD FreePrice, DWORD OldGlobelNum, DWORD OldCurrceyNum, DWORD AddGlobel, DWORD AddCurrcey,WORD RewardID, SendMsgFun pSend)
{
	//发送命令到数据库 记录下 玩家充值方面的记录 
	//char			OrderString
	//char			OrderID
	//char			OrderCode
	//char			ChannelOrderID
	//char			channelLabel
	DWORD Length = sizeof(StringArrayDataByChar)+(OrderStates.length() - 1)*sizeof(char)+
		sizeof(StringArrayDataByChar)+(OrderID.length() - 1)*sizeof(char)+
		sizeof(StringArrayDataByChar)+(ChannelCode.length() - 1)*sizeof(char)+
		sizeof(StringArrayDataByChar)+(ChannelOrderID.length() - 1)*sizeof(char)+
		sizeof(StringArrayDataByChar)+(ChannelLabel.length() - 1)*sizeof(char);

	DWORD PageSize = sizeof(DBR_Cmd_LogRecharge)-sizeof(BYTE)+Length;
	DBR_Cmd_LogRecharge* msg = (DBR_Cmd_LogRecharge*)malloc(PageSize);
	CheckMsgSize(PageSize);
	msg->SetCmdType(DBR_LogRecharge);
	msg->SetCmdSize(static_cast<WORD>(PageSize));
	msg->RechargeInfo.HandleSum = 5;
	msg->RechargeInfo.Sum = Length;
	msg->UserID = UserID;
	msg->ShopItemID = ShopItemID;
	msg->Price = Price;
	msg->OldGlobelNum = OldGlobelNum;
	msg->OldCurrceyNum = OldCurrceyNum;
	msg->AddGlobel = AddGlobel;
	msg->AddCurrcey = AddCurrcey;
	msg->AddRewardID = RewardID;
	msg->FreePrice = FreePrice;
	DWORD BeginIndex = 0;
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->RechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(OrderStates.length()*sizeof(char));
		memcpy_s(pString->Array, OrderStates.length()*sizeof(char), OrderStates.c_str(), OrderStates.length()*sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(OrderStates.length() - 1)*sizeof(char);
	}
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->RechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(OrderID.length()*sizeof(char));
		memcpy_s(pString->Array, OrderID.length() * sizeof(char), OrderID.c_str(), OrderID.length() * sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(OrderID.length() - 1)*sizeof(char);
	}
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->RechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(ChannelCode.length()*sizeof(char));
		memcpy_s(pString->Array, ChannelCode.length() * sizeof(char), ChannelCode.c_str(), ChannelCode.length() * sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(ChannelCode.length() - 1)*sizeof(char);
	}
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->RechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(ChannelOrderID.length()*sizeof(char));
		memcpy_s(pString->Array, ChannelOrderID.length() * sizeof(char), ChannelOrderID.c_str(), ChannelOrderID.length() * sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(ChannelOrderID.length() - 1)*sizeof(char);
	}
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->RechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(ChannelLabel.length()*sizeof(char));
		memcpy_s(pString->Array, ChannelLabel.length() * sizeof(char), ChannelLabel.c_str(), ChannelLabel.length() * sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(ChannelLabel.length() - 1)*sizeof(char);
	}

	pSend(msg);
	free(msg);//将玩家充值的数据发送到 GameServer去
}
void DBLogManager::LogUserEntityItemLogToDB(string OrderStates, DWORD ID, DWORD ItemID, DWORD ItemSum, time_t ShopLogTime, DWORD UserID, 
	string Address, UINT64 Phone, string IDEntity, string Name, string OrderNumber, DWORD MedalSum, DWORD NowMedalSum, DWORD HandleIP, SendMsgFun pSend)
{
	//char				OrderStates
	//char				Address
	//char				IDEntity
	//char				Name
	//char				OrderNumber

	DWORD Length = sizeof(StringArrayDataByChar)+(OrderStates.length() - 1)*sizeof(char)+
		sizeof(StringArrayDataByChar)+(Address.length() - 1)*sizeof(char)+
		sizeof(StringArrayDataByChar)+(IDEntity.length() - 1)*sizeof(char)+
		sizeof(StringArrayDataByChar)+(Name.length() - 1)*sizeof(char)+
		sizeof(StringArrayDataByChar)+(OrderNumber.length() - 1)*sizeof(char);

	DWORD PageSize = sizeof(DBR_Cmd_LogEntityItemInfo)-sizeof(BYTE)+Length;
	DBR_Cmd_LogEntityItemInfo* msg = (DBR_Cmd_LogEntityItemInfo*)malloc(PageSize);
	CheckMsgSize(PageSize);
	msg->SetCmdType(DBR_LogEntityItemInfo);
	msg->SetCmdSize(static_cast<WORD>(PageSize));
	msg->EntityInfo.HandleSum = 5;
	msg->EntityInfo.Sum = Length;
	msg->ID = ID;
	msg->ItemID = ItemID;
	msg->ItemSum = ItemSum;
	msg->ShopLogTime = ShopLogTime;
	msg->UserID = UserID;
	msg->Phone = Phone;
	msg->Medal = MedalSum;
	msg->NowMedal = NowMedalSum;
	msg->HandleIP = HandleIP;

	DWORD BeginIndex = 0;
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->EntityInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(OrderStates.length()*sizeof(char));
		memcpy_s(pString->Array, OrderStates.length()*sizeof(char), OrderStates.c_str(), OrderStates.length()*sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(OrderStates.length() - 1)*sizeof(char);
	}
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->EntityInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(Address.length()*sizeof(char));
		memcpy_s(pString->Array, Address.length() * sizeof(char), Address.c_str(), Address.length() * sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(Address.length() - 1)*sizeof(char);
	}
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->EntityInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(IDEntity.length()*sizeof(char));
		memcpy_s(pString->Array, IDEntity.length() * sizeof(char), IDEntity.c_str(), IDEntity.length() * sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(IDEntity.length() - 1)*sizeof(char);
	}
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->EntityInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(Name.length()*sizeof(char));
		memcpy_s(pString->Array, Name.length() * sizeof(char), Name.c_str(), Name.length() * sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(Name.length() - 1)*sizeof(char);
	}
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->EntityInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(OrderNumber.length()*sizeof(char));
		memcpy_s(pString->Array, OrderNumber.length() * sizeof(char), OrderNumber.c_str(), OrderNumber.length() * sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(OrderNumber.length() - 1)*sizeof(char);
	}
	pSend(msg);
	free(msg);
}
void DBLogManager::LogRoleOnlineInfo(DWORD dwUserID, bool IsOnlineOrLeaveOnline, string MacAddress, string IpAddress, DWORD GlobelNum, DWORD CurrceyNum, DWORD MedalNum, SendMsgFun pSend)
{
	DBR_Cmd_LogOnlineInfo msg;
	SetMsgInfo(msg, DBR_LogOnlineInfo, sizeof(DBR_Cmd_LogOnlineInfo));
	msg.dwUserID = dwUserID;
	msg.IsOnline = IsOnlineOrLeaveOnline;
	msg.MadelSum = MedalNum;
	msg.CurrceySum = CurrceyNum;
	msg.GlobelSum = GlobelNum;
	strncpy_s(msg.IpAddress, CountArray(msg.IpAddress), IpAddress.c_str(), IpAddress.length());
	strncpy_s(msg.MacAddress, CountArray(msg.MacAddress), MacAddress.c_str(), MacAddress.length());
	pSend(&msg);
}
void DBLogManager::LogRoleJoinOrLeaveTableInfo(DWORD dwUserID, DWORD GlobelNum, DWORD CurrceyNum, DWORD MedalNum, BYTE TableID, BYTE MonthID, bool IsJoinOrLeave, SendMsgFun pSend)
{
	DBR_Cmd_LogRoleTableInfo msg;
	SetMsgInfo(msg, DBR_LogRoleTableInfo, sizeof(DBR_Cmd_LogRoleTableInfo));
	msg.dwUserID = dwUserID;
	msg.GlobelSum = GlobelNum;
	msg.MedalSum = MedalNum;
	msg.CurrceySum = CurrceyNum;
	msg.TableID = TableID;
	msg.TableMonthID = MonthID;
	msg.JionOrLeave = IsJoinOrLeave;
	pSend(&msg);
}
void DBLogManager::LogRolePhonePayLogToDB(string OrderStates, UINT64 OrderID, DWORD UserID, UINT64 PhoneNumber, DWORD FacePrice, SendMsgFun pSend)
{
	//char OrderStates
	DWORD Length = sizeof(StringArrayDataByChar)+(OrderStates.length() - 1)*sizeof(char);

	DWORD PageSize = sizeof(DBR_Cmd_LogPhonePay)-sizeof(BYTE)+Length;
	DBR_Cmd_LogPhonePay* msg = (DBR_Cmd_LogPhonePay*)malloc(PageSize);
	CheckMsgSize(PageSize);
	msg->SetCmdType(DBR_LogPhonePay);
	msg->SetCmdSize(static_cast<WORD>(PageSize));
	msg->PhonePayInfo.HandleSum = 1;
	msg->PhonePayInfo.Sum = Length;
	msg->OrderID = OrderID;
	msg->UserID = UserID;
	msg->PhoneNumber = PhoneNumber;
	msg->FacePrice = FacePrice;

	DWORD BeginIndex = 0;
	{
		StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->PhonePayInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(OrderStates.length()*sizeof(char));
		memcpy_s(pString->Array, OrderStates.length()*sizeof(char), OrderStates.c_str(), OrderStates.length()*sizeof(char));
		BeginIndex += sizeof(StringArrayDataByChar)+(OrderStates.length() - 1)*sizeof(char);
	}
	pSend(msg);
	free(msg);
}
void DBLogManager::LogNiuNiuTableInfoToDB(BYTE BrandValue[MAX_NIUNIU_ClientSum + 1][MAX_NIUNIU_BrandSum], INT64 BrandGlobel, INT64 SystemGlobel, DWORD RoleSum, SendMsgFun pSend)
{
	DBR_Cmd_LogNiuNiuTableInfo msg;
	SetMsgInfo(msg, DBR_LogNiuNiuTableInfo, sizeof(DBR_Cmd_LogNiuNiuTableInfo));
	msg.SystemGlobelSum = SystemGlobel;
	msg.UserSum = RoleSum;
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum + 1; ++i)
	{
		for (BYTE j = 0; j < MAX_NIUNIU_BrandSum; ++j)
		{
			msg.BrandValue[i][j] = BrandValue[i][j];
		}
	}
	msg.BrandGlobelSum = BrandGlobel;
	msg.NowTime = time(null);
	pSend(&msg);
}
void DBLogManager::LogExChangeInfoToDB(DWORD dwUserID, BYTE ExChangeTypeID, TCHAR ExChange[ExChangeCode_Length + 1], SendMsgFun pSend)
{
	DBR_Cmd_LogExChangeInfo msg;
	SetMsgInfo(msg, DBR_LogExChangeInfo, sizeof(DBR_Cmd_LogExChangeInfo));
	msg.dwUserID = dwUserID;
	msg.ExChangeTypeID = ExChangeTypeID;
	TCHARCopy(msg.ExChangeCode, CountArray(msg.ExChangeCode), ExChange, _tcslen(ExChange));
	pSend(&msg);
}
void DBLogManager::LogLotteryInfoToDB(DWORD dwUserID, BYTE LotteryID, WORD RewardID, SendMsgFun pSend)
{
	DBR_Cmd_LogLotteryInfo msg;
	SetMsgInfo(msg, DBR_LogLotteryInfo, sizeof(DBR_Cmd_LogLotteryInfo));
	msg.dwUserID = dwUserID;
	msg.LotteryID = LotteryID;
	msg.RewardID = RewardID;
	pSend(&msg);
}
void DBLogManager::LogMonthInfoToDB(DWORD dwUserID, BYTE MonthID, DWORD MonthScore, DWORD MonthIndex, BYTE AddMonthGlobelSum, DWORD MonthSkillSum, WORD MonthRewardID, SendMsgFun pSend)
{
	DBR_Cmd_LogMonthInfo msg;
	SetMsgInfo(msg, DBR_LogMonthInfo, sizeof(DBR_Cmd_LogMonthInfo));
	msg.dwUserID = dwUserID;
	msg.MonthID = MonthID;
	msg.MonthIndex = MonthIndex;
	msg.AddGlobelSum = AddMonthGlobelSum;
	msg.SkillSum = MonthSkillSum;
	msg.RewardID = MonthRewardID;
	msg.MonthScore = MonthScore;
	pSend(&msg);
}
void DBLogManager::LogDialInfoToDB(DWORD BanderUserID, BYTE AreaData[MAX_DIAL_GameSum], UINT64 AreaGlobel[MAX_DIAL_ClientSum], 
	WORD ResultIndex, INT64 BrandGlobel, INT64 SystemGlobel, DWORD RoleSum, SendMsgFun pSend)
{
	//记录每一局 森林舞会的记录
	//庄家是谁
	//转盘数据
	//下注区金币数量
	//系统输赢 
	//系统库存
	//玩家数量
	//系统结果
	DBR_Cmd_LogDialInfo msg;
	SetMsgInfo(msg, DBR_LogDialInfo, sizeof(DBR_Cmd_LogDialInfo));
	msg.BanderUserID = BanderUserID;
	for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
		msg.AreaData[i] = AreaData[i];
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		msg.AreaGlobel[i] = AreaGlobel[i];
	msg.ResultIndex = ResultIndex;
	msg.BrandGlobel = BrandGlobel;
	msg.SystemGlobel = SystemGlobel;
	msg.RoleSum = RoleSum;
	pSend(&msg);
}
void DBLogManager::LogCarInfoToDB(DWORD BanderUserID, UINT64 AreaGlobel[MAX_CAR_ClientSum], BYTE ResultIndex, INT64 BrandGlobel, INT64 SystemGlobel, DWORD RoleSum, SendMsgFun pSend)
{
	//记录转盘的结果
	DBR_Cmd_LogCarInfo msg;
	SetMsgInfo(msg, DBR_LogCarInfo, sizeof(DBR_Cmd_LogCarInfo));
	msg.BanderUserID = BanderUserID;
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
		msg.AreaGlobel[i] = AreaGlobel[i];
	msg.ResultIndex = ResultIndex;
	msg.BrandGlobel = BrandGlobel;
	msg.SystemGlobel = SystemGlobel;
	msg.RoleSum = RoleSum;
	pSend(&msg);
}
DBLogManager g_DBLogManager;