#pragma once
//接入第三方的 平台 进行使用指定的订单号的协议处理
#include <set>
#define ORDER_ID_STR			"orderid"
#define PRICE_STR				"price"
#define CHANNEL_CODE_STR		"channelcode"
#define CHANNEL_ORDER_ID_STR	"channelorderid"
#define CHANNEL_LABEL_STR		"channellabel"
#define CALLBACK_STR			"callbackinfo"
#define SIGN_STR				"sign"
#define SIGN_STR_2				"sign2"
#define VERSION_STR				"version"
#define FREEPRICE_STR			"freeprice"
#define SDKCode_STR				"sdkcode"
struct RechargeCrcInfo
{
	DWORD		orderidCrc;
	DWORD		priceCrc;
	DWORD		ChannelCodeCrc;
	DWORD		channelOrderidCrc;
	DWORD		channelLabelCrc;
	DWORD		callbackInfoCrc;
	DWORD		signCrc;
	DWORD		sign2Crc;
	DWORD       versionCrc;
	DWORD		freePriceCrc;
	DWORD       sdkCodeCrc;

	RechargeCrcInfo()
	{
		orderidCrc			= AECrc32(ORDER_ID_STR, strlen(ORDER_ID_STR), 0);
		priceCrc			= AECrc32(PRICE_STR, strlen(PRICE_STR), 0);
		ChannelCodeCrc		= AECrc32(CHANNEL_CODE_STR, strlen(CHANNEL_CODE_STR), 0);
		channelOrderidCrc	= AECrc32(CHANNEL_ORDER_ID_STR, strlen(CHANNEL_ORDER_ID_STR), 0);
		channelLabelCrc		= AECrc32(CHANNEL_LABEL_STR, strlen(CHANNEL_LABEL_STR), 0);
		callbackInfoCrc		= AECrc32(CALLBACK_STR, strlen(CALLBACK_STR), 0);
		signCrc				= AECrc32(SIGN_STR, strlen(SIGN_STR), 0);
		sign2Crc			= AECrc32(SIGN_STR_2, strlen(SIGN_STR_2), 0);
		versionCrc			= AECrc32(VERSION_STR, strlen(VERSION_STR), 0);
		freePriceCrc		= AECrc32(FREEPRICE_STR, strlen(FREEPRICE_STR), 0);
		sdkCodeCrc			= AECrc32(SDKCode_STR, strlen(SDKCode_STR), 0);

		Log("OrderCrc:%u",			orderidCrc);
		Log("priceCrc:%u",			priceCrc);
		Log("ChannelCodeCrc:%u",	ChannelCodeCrc);
		Log("channelOrderidCrc:%u", channelOrderidCrc);
		Log("channelLabelCrc:%u",	channelLabelCrc);
		Log("callbackInfoCrc:%u",	callbackInfoCrc);
		Log("signCrc:%u",			signCrc);
		Log("sign2:%u",				sign2Crc);
		Log("version:%u",			versionCrc);
		Log("freePrice:%u",			freePriceCrc);
		Log("sdkCode:%u",			sdkCodeCrc);
	}
};

class UseRMBManager
{
public:
	UseRMBManager();
	virtual ~UseRMBManager();

	RechargeCrcInfo& GetRechargeCrcInfo(){ return m_RechargeCrcInfo; }

	void OnInit();

	//void OnLoadAllOrderID(DBO_Cmd_LoadRechageOrderID* pMsg);//加载全部的7天内的订单号 并且进行处理 (订单号一直存储到数据库中)

	bool OnHandleHttpInfoByUserRecharge(string orderid, string price, string ChannelCode, string channelOrderid, string channelLabel, string callbackInfo, string sign, string sign2, string version, string freePrice, string sdkCode);
	bool OnHandleHttpInfoByUserRecharge(string orderid, string price, string ChannelCode, string channelOrderid, string channelLabel, string callbackInfo, string sign);

private:
	//std::set<unsigned __int64>			m_OrderIDSet;//已经处理的订单的集合 我们只读取7天内的订单号码存储起来
	RechargeCrcInfo						m_RechargeCrcInfo;

};