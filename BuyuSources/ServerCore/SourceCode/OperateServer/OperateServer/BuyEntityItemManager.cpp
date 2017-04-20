#include "stdafx.h"
#include "FishServer.h"
#include "BuyEntityItemManager.h"
BuyEntityManager::BuyEntityManager()
{
	m_OnlyID = 0;
	m_BuyEntityMap.clear();
}
BuyEntityManager::~BuyEntityManager()
{
	m_BuyEntityMap.clear();
}
bool BuyEntityManager::OnBuyEntityItem(ServerClientData* pClient, GO_Cmd_BuyEntityItem*pMsg)
{
	if (!pClient || !pMsg)
	{
		ASSERT(false);
		return false;
	}
	DWORD OnlyID = m_OnlyID;
	++m_OnlyID;
	BuyEntityOnce pOnce;
	pOnce.OnlyID = OnlyID;
	pOnce.UserID = pMsg->dwUserID;
	pOnce.ItemID = pMsg->ItemID;
	pOnce.ItemSum = pMsg->ItemSum;
	pOnce.ShopID = pMsg->ShopID;
	pOnce.ShopOnlyID = pMsg->ShopOnlyID;
	pOnce.GameServerClientID = pClient->OutsideExtraData;
	TCHARCopy(pOnce.Addres, CountArray(pOnce.Addres), pMsg->Addres, _tcslen(pMsg->Addres));
	TCHARCopy(pOnce.Name, CountArray(pOnce.Name), pMsg->Name, _tcslen(pMsg->Name));
	pOnce.Phone = pMsg->Phone;
	m_BuyEntityMap.insert(HashMap<DWORD, BuyEntityOnce>::value_type(OnlyID,pOnce));//添加进入集合
	//发送命令到外部服务器去
	//SendLink
	//无须外部处理  后续可能进行添加的 如 直接发送到商店去 上面的

	//xuda
	OnBuyEntityItemResult(ROE_BuyEntityItem_Sucess,true, pMsg->dwUserID, OnlyID);//表示玩家购买物品成功了 实体物品购买成功了
	return true;
}
void BuyEntityManager::OnBuyEntityItemResult(BYTE ErrorID, bool Result, DWORD dwUserID, DWORD OnlyID)
{
	HashMap<DWORD, BuyEntityOnce>::iterator Iter = m_BuyEntityMap.find(OnlyID);//玩家已经提交订单了
	if (Iter == m_BuyEntityMap.end())
	{
		ASSERT(false);
		return;//不存在的订单 无须处理
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(ConvertDWORDToBYTE(Iter->second.GameServerClientID));
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//获取到订单后 我们开始处理
	OG_Cmd_BuyEntityItem msg;
	SetMsgInfo(msg, GetMsgType(Main_Operate, OG_BuyEntityItem), sizeof(OG_Cmd_BuyEntityItem));
	msg.dwUserID = dwUserID;
	msg.ErrorID = ErrorID;
	msg.ShopID = Iter->second.ShopID;
	msg.ShopOnlyID = Iter->second.ShopOnlyID;
	msg.ItemSum = Iter->second.ItemSum;
	msg.Result = Result;
	g_FishServer.SendNetCmdToClient(pClient, &msg);
	m_BuyEntityMap.erase(Iter);//移除订单
}