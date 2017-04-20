#include "stdafx.h"
#include "UseRMBManager.h"
#include "FishServer.h"
#include "..\CommonFile\FishServerConfig.h"
#include "..\CommonFile\DBLogManager.h"
extern void SendLogDB(NetCmd* pCmd);
UseRMBManager::UseRMBManager()
{
}
UseRMBManager::~UseRMBManager()
{

}
void UseRMBManager::OnInit()
{
	/*DBR_Cmd_LoadRechageOrderID msg;
	SetMsgInfo(msg, DBR_LoadRechageOrderID, sizeof(DBR_Cmd_LoadRechageOrderID));
	msg.DiffDay = 2;
	g_FishServer.SendNetCmdToDB(&msg);*/
}
//void UseRMBManager::OnLoadAllOrderID(DBO_Cmd_LoadRechageOrderID* pMsg)
//{
//	if (!pMsg)
//	{
//		ASSERT(false);
//		return;
//	}
//	if (pMsg->States & MsgBegin)
//	{
//		m_OrderIDSet.clear();
//	}
//	for (WORD i = 0; i < pMsg->Sum; ++i)
//	{
//		m_OrderIDSet.insert(std::set<unsigned __int64>::value_type(pMsg->Array[i]));
//	}
//	if (pMsg->States & MsgEnd)
//	{
//		//Operate 可以登陆了
//		g_FishServer.OnFinish();//Operate 可以开启了
//	}
//}
bool UseRMBManager::OnHandleHttpInfoByUserRecharge(string orderid, string price, string ChannelCode, string channelOrderid, string channelLabel, string callbackInfo, string sign)
{
	//旧版本
	LogInfoToFile("LogRechargeInfo.txt", "充值异步回调: orderid=%s, price=%s,ChannelCode=%s, channelOrderid =%s, channelLabel =%s, callbackInfo=%s, sign=%s", orderid.c_str(), price.c_str(), ChannelCode.c_str(), channelOrderid.c_str(), channelLabel.c_str(), callbackInfo.c_str(), sign.c_str());

	//命令应该可以做到玩家所在的Center所在
	//CustParam 应该包含  UserID  /ShopID 
	DWORD dPrice = strtoul(price.c_str(), null, 10);
	string BackInfo = Base64Decode(callbackInfo.c_str(), callbackInfo.length());
	UINT64 Param = _strtoui64(BackInfo.c_str(), null, 10);//_atoi64(BackInfo.c_str());
	DWORD UserID = static_cast<DWORD>(Param >> 32);
	DWORD ShopItemID = (DWORD)Param;

	HashMap<DWORD, tagFishRechargeInfo>::iterator Iter = g_FishServer.GetFishConfig().GetFishRechargesConfig().m_FishRechargeMap.find(ShopItemID);
	if (Iter == g_FishServer.GetFishConfig().GetFishRechargesConfig().m_FishRechargeMap.end())//物品不存在
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:物品不存在", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, 0, 0, 0, 0, 0, 0, SendLogDB);
		return false;
	}
	if (Iter->second.dDisCountPrice * 100 != dPrice)//价格不正确
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:价格不正确", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, 0, 0, 0, 0, 0, 0, SendLogDB);
		return false;
	}
	//对当前的兑换数据进行判断处理  验证 兑换码是否是有效的 
	//验证签名
	OperateConfig* pConfig = g_FishServerConfig.GetOperateConfig();
	if (!pConfig)
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:系统错误", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, 0, 0, 0, 0, 0, 0, SendLogDB);
		return false;
	}
	//g_FishServer.ShowInfoToWin("渠道服务器进行充值3");
	string Product = pConfig->ProductSecret;
	string CheckStr = orderid + price + callbackInfo + Product;
	string Md5Str = md5(CheckStr);//加密后的签名
	if (Md5Str.compare(sign) != 0)//验证签名是否正确
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:MD5验证失败", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, 0, 0, 0, 0, 0, 0, SendLogDB);
		return false;
	}

	//g_FishServer.ShowInfoToWin("渠道服务器进行充值4");
	//验证兑换码 是否存在
	//1.订单号转为3个DWORD 3个DWORD 一组 进行处理
	AE_CRC_PAIRS pThree;
	AECrc32(pThree, orderid.c_str(), orderid.length(), 0, 0x73573);
	//订单转化的CRC 数据
	DWORD Crc1 = pThree.Crc1;
	DWORD Crc2 = pThree.Crc2;
	unsigned __int64 i64Value = Crc1;
	i64Value = (i64Value << 32);
	i64Value += Crc2;
	//获得订单的唯一编号 我们写入数据库 或者进行判断
	if (g_FishServer.GetOrderOnlyManager().IsExists(i64Value, OT_SDK))
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:重复的订单", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, 0, 0, 0, 0, 0, 0, SendLogDB);
		//LogInfoToFile("LogRechargeInfo.txt", "充值异步回调 订单重复执行: orderid=%s", orderid.c_str());
		return false;//订单号存在
	}
	//g_FishServer.ShowInfoToWin("渠道服务器进行充值5");
	//因为兑换码可能重复发送 我们想要对验证码进行验证 处理
	//1.将验证码保存到数据库去
	g_FishServer.GetOrderOnlyManager().OnAddOrderInfo(UserID, i64Value, OT_SDK);
	//DBR_Cmd_AddRechageOrderID msgDB;
	//SetMsgInfo(msgDB, DBR_AddRechageOrderID, sizeof(DBR_Cmd_AddRechageOrderID));
	//msgDB.dwUserID = UserID;
	//msgDB.OrderOnlyID = i64Value;
	//g_FishServer.SendNetCmdToDB(&msgDB);
	////2.将验证码保存到内存中去
	//m_OrderIDSet.insert(std::set<unsigned __int64>::value_type(i64Value));
	//处理网络命令 
	//我们想要组织命令到客户端去
	//将这些字符串 转化为TCHAR  并且 放入到对象里面去
	UINT Count = 0;
	TCHAR* pOrderID = CharToWChar(orderid.c_str(), Count);
	TCHAR* pChannelCode = CharToWChar(ChannelCode.c_str(), Count);
	TCHAR* pChannelOrderid = CharToWChar(channelOrderid.c_str(), Count);
	TCHAR* pChannelLabel = CharToWChar(channelLabel.c_str(), Count);

	DWORD Length = sizeof(StringArrayData)+(orderid.length() - 1)*sizeof(TCHAR)+
		sizeof(StringArrayData)+(ChannelCode.length() - 1)*sizeof(TCHAR)+
		sizeof(StringArrayData)+(channelOrderid.length() - 1)*sizeof(TCHAR)+
		sizeof(StringArrayData)+(channelLabel.length() - 1)*sizeof(TCHAR);


	DWORD PageSize = sizeof(OC_Cmd_UseRMB)-sizeof(BYTE)+Length;
	OC_Cmd_UseRMB* msg = (OC_Cmd_UseRMB*)malloc(PageSize);
	CheckMsgSizeReturn(PageSize);
	msg->SetCmdType(GetMsgType(Main_Operate, OC_UseRMB));
	msg->SetCmdSize(static_cast<WORD>(PageSize));
	msg->rechargeInfo.Price = dPrice;
	msg->rechargeInfo.UserID = UserID;
	msg->rechargeInfo.ShopItemID = ShopItemID;
	msg->rechargeInfo.HandleSum = 4;
	msg->rechargeInfo.Sum = Length;
	msg->rechargeInfo.FreePrice = 0;
	DWORD BeginIndex = 0;
	{
		StringArrayData* pString = (StringArrayData*)&msg->rechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(orderid.length()*sizeof(TCHAR));
		memcpy_s(pString->Array, orderid.length()*sizeof(TCHAR), pOrderID, orderid.length()*sizeof(TCHAR));
		BeginIndex += sizeof(StringArrayData)+(orderid.length() - 1)*sizeof(TCHAR);
	}
	{
		StringArrayData* pString = (StringArrayData*)&msg->rechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(ChannelCode.length()*sizeof(TCHAR));
		memcpy_s(pString->Array, ChannelCode.length() * sizeof(TCHAR), pChannelCode, ChannelCode.length() * sizeof(TCHAR));
		BeginIndex += sizeof(StringArrayData)+(ChannelCode.length() - 1)*sizeof(TCHAR);
	}
	{
		StringArrayData* pString = (StringArrayData*)&msg->rechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(channelOrderid.length()*sizeof(TCHAR));
		memcpy_s(pString->Array, channelOrderid.length() * sizeof(TCHAR), pChannelOrderid, channelOrderid.length() * sizeof(TCHAR));
		BeginIndex += sizeof(StringArrayData)+(channelOrderid.length() - 1)*sizeof(TCHAR);
	}
	{
		StringArrayData* pString = (StringArrayData*)&msg->rechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(channelLabel.length()*sizeof(TCHAR));
		memcpy_s(pString->Array, channelLabel.length() * sizeof(TCHAR), pChannelLabel, channelLabel.length() * sizeof(TCHAR));
		BeginIndex += sizeof(StringArrayData)+(channelLabel.length() - 1)*sizeof(TCHAR);
	}

	g_FishServer.SendNetCmdToCenterServer(msg);
	free(msg);//将玩家充值的数据发送到 GameServer去
	return true;
}
bool UseRMBManager::OnHandleHttpInfoByUserRecharge(string orderid, string price, string ChannelCode, string channelOrderid, string channelLabel, string callbackInfo, string sign, string sign2, string version, string freePrice, string sdkCode)
{
	// string sign2, string version, string freePrice, string sdkCode
	LogInfoToFile("LogRechargeInfo.txt", "充值异步回调: orderid=%s, price=%s,freeprice=%s,version=%s,sdkCode=%s, ChannelCode=%s, channelOrderid =%s, channelLabel =%s, callbackInfo=%s, sign=%s ,sign2=%s", orderid.c_str(), price.c_str(), freePrice.c_str(), version.c_str(), sdkCode.c_str(), ChannelCode.c_str(), channelOrderid.c_str(), channelLabel.c_str(), callbackInfo.c_str(), sign.c_str(), sign2.c_str());

	//命令应该可以做到玩家所在的Center所在
	//CustParam 应该包含  UserID  /ShopID 
	DWORD dPrice = strtoul(price.c_str(), null, 10);
	DWORD FreePrice = strtoul(freePrice.c_str(), null, 10);
	string BackInfo = Base64Decode(callbackInfo.c_str(), callbackInfo.length());
	UINT64 Param = _strtoui64(BackInfo.c_str(), null, 10);//_atoi64(BackInfo.c_str());
	DWORD UserID = static_cast<DWORD>(Param >> 32);
	DWORD ShopItemID = (DWORD)Param;
	/*if (dPrice + FreePrice == 2800)//临时的
	{
	UserID = (DWORD)Param;
	ShopItemID = 13;
	}*/
	/*if (sdkCode.length() == 0 || strlen(sdkCode.c_str()) == 0)
	sdkCode = channelLabel;
	if (freePrice.length() == 0 || strlen(freePrice.c_str()) == 0)
	freePrice = "0";
	if (version.compare("v2.0") != 0)
	{
	g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:版本错误", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, FreePrice, 0, 0, 0, 0, 0, SendLogDB);
	return false;
	}
	if (FreePrice != 0 && sdkCode.compare("anzhi") != 0)
	{
	g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:非安智渠道无礼金功能", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, FreePrice, 0, 0, 0, 0, 0, SendLogDB);
	return false;
	}*/
	HashMap<DWORD, tagFishRechargeInfo>::iterator Iter = g_FishServer.GetFishConfig().GetFishRechargesConfig().m_FishRechargeMap.find(ShopItemID);
	if (Iter == g_FishServer.GetFishConfig().GetFishRechargesConfig().m_FishRechargeMap.end())//物品不存在
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:物品不存在", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, FreePrice, 0, 0, 0, 0, 0, SendLogDB);
		return false;
	}
	if (Iter->second.dDisCountPrice * 100 != dPrice)//价格不正确
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:价格不正确", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, FreePrice, 0, 0, 0, 0, 0, SendLogDB);
		return false;
	}
	//对当前的兑换数据进行判断处理  验证 兑换码是否是有效的 
	//验证签名
	OperateConfig* pConfig = g_FishServerConfig.GetOperateConfig();
	if (!pConfig)
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:系统错误", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, FreePrice, 0, 0, 0, 0, 0, SendLogDB);
		return false;
	}
	//g_FishServer.ShowInfoToWin("渠道服务器进行充值3");
	string Product = pConfig->ProductSecret;
	string CheckStr = orderid + price + callbackInfo + Product;
	string Md5Str = md5(CheckStr);//加密后的签名
	if (Md5Str.compare(sign) != 0)//验证签名是否正确
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:MD5验证失败", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, FreePrice, 0, 0, 0, 0, 0, SendLogDB);
		return false;
	}

	string CheckSign2Str = "callbackInfo=" + callbackInfo + "&channelCode=" + ChannelCode + "&channelLabel=" + channelLabel + "&channelOrderId=" + channelOrderid + "&freePrice=" + freePrice + "&orderId=" + orderid + "&price=" + price + "&sdkCode=" + sdkCode + "&version=" + version + "&" + Product;
	string Md5Str2 = md5(CheckSign2Str);//加密后的签名
	if (Md5Str2.compare(sign2) != 0)//验证签名是否正确
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:MD5验证2失败", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, FreePrice, 0, 0, 0, 0, 0, SendLogDB);
		//LogInfoToFile("LogRechargeInfo.txt", "充值异步回调 订单失败 MD5校验失败: orderid=%s ChannelMD5=%s,ConfigMD5=%s", orderid.c_str(), sign.c_str(), Md5Str.c_str());
		return false;
	}

	//g_FishServer.ShowInfoToWin("渠道服务器进行充值4");
	//验证兑换码 是否存在
	//1.订单号转为3个DWORD 3个DWORD 一组 进行处理
	AE_CRC_PAIRS pThree;
	AECrc32(pThree, orderid.c_str(), orderid.length(), 0, 0x73573);
	//订单转化的CRC 数据
	DWORD Crc1 = pThree.Crc1;
	DWORD Crc2 = pThree.Crc2;
	unsigned __int64 i64Value = Crc1;
	i64Value = (i64Value << 32);
	i64Value += Crc2;
	//获得订单的唯一编号 我们写入数据库 或者进行判断
	//std::set<unsigned __int64>::iterator IterFind = m_OrderIDSet.find(i64Value);
	if (g_FishServer.GetOrderOnlyManager().IsExists(i64Value, OT_SDK))
	{
		g_DBLogManager.LogUserRechargeLogToDB("运营服务器验证失败:重复的订单", orderid, UserID, ChannelCode, channelOrderid, channelLabel, ShopItemID, dPrice, FreePrice, 0, 0, 0, 0, 0, SendLogDB);
		//LogInfoToFile("LogRechargeInfo.txt", "充值异步回调 订单重复执行: orderid=%s", orderid.c_str());
		return false;//订单号存在
	}
	//g_FishServer.ShowInfoToWin("渠道服务器进行充值5");
	//因为兑换码可能重复发送 我们想要对验证码进行验证 处理
	//1.将验证码保存到数据库去
	//DBR_Cmd_AddRechageOrderID msgDB;
	//SetMsgInfo(msgDB, DBR_AddRechageOrderID, sizeof(DBR_Cmd_AddRechageOrderID));
	//msgDB.dwUserID = UserID;
	//msgDB.OrderOnlyID = i64Value;
	//g_FishServer.SendNetCmdToDB(&msgDB);
	////2.将验证码保存到内存中去
	//m_OrderIDSet.insert(std::set<unsigned __int64>::value_type(i64Value));
	g_FishServer.GetOrderOnlyManager().OnAddOrderInfo(UserID, i64Value, OT_SDK);
	//处理网络命令 
	//我们想要组织命令到客户端去
	//将这些字符串 转化为TCHAR  并且 放入到对象里面去
	UINT Count = 0;
	TCHAR* pOrderID = CharToWChar(orderid.c_str(), Count);
	TCHAR* pChannelCode = CharToWChar(ChannelCode.c_str(), Count);
	TCHAR* pChannelOrderid = CharToWChar(channelOrderid.c_str(), Count);
	TCHAR* pChannelLabel = CharToWChar(channelLabel.c_str(), Count);

	DWORD Length = sizeof(StringArrayData)+(orderid.length() - 1)*sizeof(TCHAR)+
		sizeof(StringArrayData)+(ChannelCode.length() - 1)*sizeof(TCHAR)+
		sizeof(StringArrayData)+(channelOrderid.length() - 1)*sizeof(TCHAR)+
		sizeof(StringArrayData)+(channelLabel.length() - 1)*sizeof(TCHAR);


	DWORD PageSize = sizeof(OC_Cmd_UseRMB)-sizeof(BYTE)+Length;
	OC_Cmd_UseRMB* msg = (OC_Cmd_UseRMB*)malloc(PageSize);
	CheckMsgSizeReturn(PageSize);
	msg->SetCmdType(GetMsgType(Main_Operate, OC_UseRMB));
	msg->SetCmdSize(static_cast<WORD>(PageSize));
	msg->rechargeInfo.Price = dPrice;
	msg->rechargeInfo.UserID = UserID;
	msg->rechargeInfo.ShopItemID = ShopItemID;
	msg->rechargeInfo.HandleSum = 4;
	msg->rechargeInfo.Sum = Length;
	msg->rechargeInfo.FreePrice = FreePrice;
	DWORD BeginIndex = 0;
	{
		StringArrayData* pString = (StringArrayData*)&msg->rechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(orderid.length()*sizeof(TCHAR));
		memcpy_s(pString->Array, orderid.length()*sizeof(TCHAR), pOrderID, orderid.length()*sizeof(TCHAR));
		BeginIndex += sizeof(StringArrayData)+(orderid.length() - 1)*sizeof(TCHAR);
	}
	{
		StringArrayData* pString = (StringArrayData*)&msg->rechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(ChannelCode.length()*sizeof(TCHAR));
		memcpy_s(pString->Array, ChannelCode.length() * sizeof(TCHAR), pChannelCode, ChannelCode.length() * sizeof(TCHAR));
		BeginIndex += sizeof(StringArrayData)+(ChannelCode.length() - 1)*sizeof(TCHAR);
	}
	{
		StringArrayData* pString = (StringArrayData*)&msg->rechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(channelOrderid.length()*sizeof(TCHAR));
		memcpy_s(pString->Array, channelOrderid.length() * sizeof(TCHAR), pChannelOrderid, channelOrderid.length() * sizeof(TCHAR));
		BeginIndex += sizeof(StringArrayData)+(channelOrderid.length() - 1)*sizeof(TCHAR);
	}
	{
		StringArrayData* pString = (StringArrayData*)&msg->rechargeInfo.Array[BeginIndex];
		pString->Length = ConvertDWORDToBYTE(channelLabel.length()*sizeof(TCHAR));
		memcpy_s(pString->Array, channelLabel.length() * sizeof(TCHAR), pChannelLabel, channelLabel.length() * sizeof(TCHAR));
		BeginIndex += sizeof(StringArrayData)+(channelLabel.length() - 1)*sizeof(TCHAR);
	}

	g_FishServer.SendNetCmdToCenterServer(msg);
	free(msg);//将玩家充值的数据发送到 GameServer去
	return true;
}
