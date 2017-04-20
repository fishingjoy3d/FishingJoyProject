#include "stdafx.h"
#include "OrderOnlyManager.h"
#include "FishServer.h"
OrderOnlyManager::OrderOnlyManager()
{

}
OrderOnlyManager::~OrderOnlyManager()
{

}
void OrderOnlyManager::OnInit()
{
	DBR_Cmd_LoadRechageOrderID msg;
	SetMsgInfo(msg, DBR_LoadRechageOrderID, sizeof(DBR_Cmd_LoadRechageOrderID));
	msg.DiffDay = 2;
	g_FishServer.SendNetCmdToDB(&msg);
}
void OrderOnlyManager::OnLoadAllOrderID(DBO_Cmd_LoadRechageOrderID* pMsg)
{
	if (!pMsg)
	{
		return;
	}
	if (pMsg->States & MsgBegin)
	{
		for (BYTE i = 0; i < OT_Max; ++i)
			m_OrderMap[i].clear();
	}
	for (WORD i = 0; i < pMsg->Sum; ++i)
	{
		if (pMsg->Array[i].OrderType >= OT_Max)
			continue;
		m_OrderMap[pMsg->Array[i].OrderType].insert(set<UINT64>::value_type(pMsg->Array[i].OrderID));
	}
	if (pMsg->States & MsgEnd)
	{
		g_FishServer.OnFinish();//Operate 可以开启了
	}
}
void OrderOnlyManager::OnAddOrderInfo(DWORD dwUserID,UINT64 OrderID, BYTE OrderType)
{
	if (OrderType >= OT_Max)
		return;
	set<UINT64>::iterator Iter =  m_OrderMap[OrderType].find(OrderID);
	if (Iter != m_OrderMap[OrderType].end())
		return;
	m_OrderMap[OrderType].insert(set<UINT64>::value_type(OrderID));


	DBR_Cmd_AddRechageOrderID msgDB;
	SetMsgInfo(msgDB, DBR_AddRechageOrderID, sizeof(DBR_Cmd_AddRechageOrderID));
	msgDB.dwUserID = dwUserID;
	msgDB.OrderOnlyID.OrderID = OrderID;
	msgDB.OrderOnlyID.OrderType = OrderType;
	g_FishServer.SendNetCmdToDB(&msgDB);
}
bool OrderOnlyManager::IsExists(UINT64 OrderID, BYTE OrderType)
{
	if (OrderType >= OT_Max)
		return false;
	set<UINT64>::iterator Iter = m_OrderMap[OrderType].find(OrderID);
	if (Iter != m_OrderMap[OrderType].end())
		return true;
	else
		return false;
}