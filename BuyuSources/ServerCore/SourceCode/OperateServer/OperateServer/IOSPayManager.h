#pragma once

//苹果手机充值
struct IOSOrderInfo
{
	UINT64		OrderID;
	DWORD		dwUserID;
	DWORD		ItemID;
	char		OrderInfo[10240];
	//bool		IsTest;
};
class IOSPayManager
{
public:
	IOSPayManager();
	virtual ~IOSPayManager();

	void OnInit();

	void OnSendOrderToIOSServer(DWORD dwUserID, string OrderInfo/*, bool IsTest = false*/);
	void OnSendOrderToIOSResult(unsigned __int64 Order64ID, char* pData, DWORD Length);
private:
	DWORD GetItemID(string str);
	UINT64 GetOrderSetID(string str);
	bool  OnHandleRechargeResult(UINT64 OrderID,DWORD dwUserID, DWORD ItemID);
private:
	HashMap<UINT64, IOSOrderInfo> m_Map;
};