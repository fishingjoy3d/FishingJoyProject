//商店管理器
#pragma once
#include "Stdafx.h"
class CRoleEx;
class ShopManager
{
public:
	ShopManager();
	virtual ~ShopManager();
	void OnInit();
	void Destroy();
	void OnShellShopItem(CRoleEx* pRole, BYTE ShopID, BYTE ItemIndex, DWORD ItemSum);//购买商店的自动物品

	void UpdateByMin();
private:
	bool ShopItemIsInTime(tagShopItemConfig* pItemConfig);
	void HandleShopItem();
private:
	HashMap<WORD, bool>		m_ShopItemStates;
};