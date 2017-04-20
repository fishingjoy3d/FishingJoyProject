#pragma once
#include <set>
enum OrderType
{
	OT_SDK  = 1,
	OT_Normal = 2,
	OT_IOS = 3,
	OT_Max,
};
class OrderOnlyManager
{
public: 
	OrderOnlyManager();
	virtual ~OrderOnlyManager();
	void OnInit();
	void OnLoadAllOrderID(DBO_Cmd_LoadRechageOrderID* pMsg);
	void OnAddOrderInfo(DWORD dwUserID, UINT64 OrderID, BYTE OrderType);
	bool IsExists(UINT64 OrderID, BYTE OrderType);
private:
	set<UINT64>			m_OrderMap[OT_Max];
};