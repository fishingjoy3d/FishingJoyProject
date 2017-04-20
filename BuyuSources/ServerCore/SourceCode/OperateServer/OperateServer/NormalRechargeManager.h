//非渠道充值处理
#pragma once

#define NORMALRECHARGE_Sign					"sign"
#define NORMALRECHARGE_SignType				"signtype"
#define NORMALRECHARGE_TransType			"transtype"
#define NORMALRECHARGE_Cporderid			"cporderid"
#define NORMALRECHARGE_Transid				"transid"
#define NORMALRECHARGE_AppUserID			"appuserid"
#define NORMALRECHARGE_AppID				"appid"
#define NORMALRECHARGE_Warwsid				"waresid"
#define NORMALRECHARGE_FeeType				"feetype"
#define NORMALRECHARGE_Money				"money"
#define NORMALRECHARGE_Currency				"currency"
#define NORMALRECHARGE_Result				"result"
#define NORMALRECHARGE_TransTime			"transtime"
#define NORMALRECHARGE_Cpprivate			"cpprivate"
#define NORMALRECHARGE_PayType				"paytype"

#define NORMALRECHARGE_ADDORDER_Sign        "sign"
#define NORMALRECHARGE_ADDORDER_Code        "code"
#define NORMALRECHARGE_ADDORDER_Transid     "transid"

struct NormalRechargeCrcInfo
{
	DWORD		signCrc;
	DWORD		signtypeCrc;
	DWORD		transtypeCrc;
	DWORD		cporderidCrc;
	DWORD		transidCrc;
	DWORD		appuseridCrc;
	DWORD		appidCrc;
	DWORD		waresidCrc;
	DWORD       feetypeCrc;
	DWORD		moneyCrc;
	DWORD       currencyCrc;
	DWORD		resultCrc;
	DWORD       transtimeCrc;
	DWORD		cpprivateCrc;
	DWORD       paytypeCrc;

	NormalRechargeCrcInfo()
	{
		signCrc = AECrc32(NORMALRECHARGE_Sign, strlen(NORMALRECHARGE_Sign), 0);
		signtypeCrc = AECrc32(NORMALRECHARGE_SignType, strlen(NORMALRECHARGE_SignType), 0);
		transtypeCrc = AECrc32(NORMALRECHARGE_TransType, strlen(NORMALRECHARGE_TransType), 0);
		cporderidCrc = AECrc32(NORMALRECHARGE_Cporderid, strlen(NORMALRECHARGE_Cporderid), 0);
		transidCrc = AECrc32(NORMALRECHARGE_Transid, strlen(NORMALRECHARGE_Transid), 0);
		appuseridCrc = AECrc32(NORMALRECHARGE_AppUserID, strlen(NORMALRECHARGE_AppUserID), 0);
		appidCrc = AECrc32(NORMALRECHARGE_AppID, strlen(NORMALRECHARGE_AppID), 0);
		waresidCrc = AECrc32(NORMALRECHARGE_Warwsid, strlen(NORMALRECHARGE_Warwsid), 0);
		feetypeCrc = AECrc32(NORMALRECHARGE_FeeType, strlen(NORMALRECHARGE_FeeType), 0);
		moneyCrc = AECrc32(NORMALRECHARGE_Money, strlen(NORMALRECHARGE_Money), 0);
		currencyCrc = AECrc32(NORMALRECHARGE_Currency, strlen(NORMALRECHARGE_Currency), 0);
		resultCrc = AECrc32(NORMALRECHARGE_Result, strlen(NORMALRECHARGE_Result), 0);
		transtimeCrc = AECrc32(NORMALRECHARGE_TransTime, strlen(NORMALRECHARGE_TransTime), 0);
		cpprivateCrc = AECrc32(NORMALRECHARGE_Cpprivate, strlen(NORMALRECHARGE_Cpprivate), 0);
		paytypeCrc = AECrc32(NORMALRECHARGE_PayType, strlen(NORMALRECHARGE_PayType), 0);
	}
};

struct NormalRechargeAddOrderCrcInfo
{
	DWORD		signCrc;
	DWORD		codeCrc;
	DWORD		transidCrc;

	NormalRechargeAddOrderCrcInfo()
	{
		signCrc = AECrc32(NORMALRECHARGE_ADDORDER_Sign, strlen(NORMALRECHARGE_ADDORDER_Sign), 0);
		codeCrc = AECrc32(NORMALRECHARGE_ADDORDER_Code, strlen(NORMALRECHARGE_ADDORDER_Code), 0);
		transidCrc = AECrc32(NORMALRECHARGE_ADDORDER_Transid, strlen(NORMALRECHARGE_ADDORDER_Transid), 0);
	}
};

struct OrderInfo
{
	UINT64      OnlyID;
	DWORD		dwUserID;
	DWORD		ShopID;
	DWORD		OrderID;
};

class NormalRechargeManager
{
public:
	NormalRechargeManager();
	virtual ~NormalRechargeManager();

	bool OnHandleAddOrder(DWORD dwUserID, DWORD OrderID, DWORD ShopID);
	void OnHandleAddOrderResult(UINT64 ID, char* pData, DWORD Length);

	void OnHandleNormalRechargeInfo(string UrlStr);
	void OnHandleNormalRechargeInfoResult(DBO_Cmd_QueryRechargeOrderInfo* pMsg);
private:
	void OnHandleRechargeByParam(string transtype, string cporderid, string transid, string appuserid, string appid, string waresid, string feetype, string money, string currency, string result, string transtime, string cpprivate, string paytype);
private:
	NormalRechargeCrcInfo			m_NormalRechargeCrcInfo;
	NormalRechargeAddOrderCrcInfo	m_NormalRechargeAddOrderCrcInfo;
	HashMap<UINT64, OrderInfo>		m_OrderMap;
};