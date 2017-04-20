//玩家物品管理类
#pragma once
#include "Stdafx.h"
class CRoleEx;
class RoleManager;
class RoleItemManger;
struct tagItemType //玩家一个类型的物品的集合数据
{
	DWORD							AllItemSum;
	tagItemInfo						NonTimeItem;//非限时物品
	HashMap<DWORD,tagItemInfo>		TimeItem;

	CRoleEx*						m_pRole;
	RoleItemManger*					m_pItemManager;
	tagItemType()
	{
		AllItemSum = 0;
		ZeroMemory(&NonTimeItem, sizeof(NonTimeItem));
		//TimeItem.Clear();
		TimeItem.clear();
	}
	~tagItemType()
	{
		Destroy();
	}
	void OnInit(CRoleEx* pRole, RoleItemManger* pManager);
	void LoadItem(tagItemInfo& pInfo);
	void Destroy();
	bool AddItem(tagItemOnce& pItem);
	void OnAddItemResult(DBO_Cmd_AddUserItem* pMsg);
	bool DelItem(DWORD ItemID, DWORD ItemSum);
	bool DelItem(DWORD ItemOnlyID, DWORD ItemID, DWORD ItemSum);
	void OnUpdateByMin();
};
class RoleItemManger//玩家在Logon上的物品类
{
public:
	RoleItemManger();
	virtual ~RoleItemManger();
	bool OnInit(CRoleEx* pUser, RoleManager* pManager);
	bool OnLoadUserItem();
	void OnLoadUserItemResult(DBO_Cmd_LoadUserItem* pDB);
	//void OnLoadUserItemFinish();
	bool OnGetUserItem();
	bool OnAddUserItem(tagItemOnce& pItem);
	void OnAddUserItemResult(DBO_Cmd_AddUserItem* pDB);
	bool OnDelUserItem(DWORD ItemID, DWORD ItemSum);
	bool OnDelUserItem(DWORD ItemOnlyID, DWORD ItemID, DWORD ItemSum);
	bool OnDelUserItemList(vector<tagItemOnce>& pVec);//删除多种物品的时候  一次性进行判断 
	void OnUpdateByMin(bool IsHourChange, bool IsDayChange, bool IsMonthChange, bool IsYearChange);

	DWORD QueryItemCount(DWORD ItemID);
	DWORD QueryItemAllTimeCount(DWORD ItemID);
	bool IsLoadDB(){ return m_IsLoadDB; }
	bool IsSendClient(){ return m_IsLoadToClient; }

	void ResetClientInfo(){ m_IsLoadToClient = false; }

	bool OnTryUseItem(DWORD ItemOnlyID, DWORD ItemID, DWORD ItemSum);
	bool OnTryAcceptItemToFriend(DWORD DestUserID,DWORD ItemOnlyID, DWORD ItemID, DWORD ItemSum);

	bool GetItemIsAllExists(DWORD ItemID, DWORD ItemSum);
private:
	bool IsExistsItem(DWORD ItemID);
	bool IsCanUseItem(DWORD ItemID);
	bool IsCanAcceptItem(DWORD ItemID);
	bool OnUseItem(DWORD ItemOnlyID,DWORD ItemID, DWORD ItemSum);
private:
	bool							m_IsLoadToClient;
	//指针
	RoleManager*					m_RoleManager;
	CRoleEx*						m_pUser;
	bool							m_IsLoadDB;
	//背包数据
	HashMap<DWORD, tagItemType*>	m_ItemMap;
};