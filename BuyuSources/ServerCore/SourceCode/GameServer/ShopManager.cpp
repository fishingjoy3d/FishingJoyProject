#include "Stdafx.h"
#include "ShopManager.h"
#include "FishServer.h"
#include "..\CommonFile\DBLogManager.h"
extern void SendLogDB(NetCmd* pCmd);
ShopManager::ShopManager()
{
}
ShopManager::~ShopManager()
{
	Destroy();
}
void ShopManager::OnInit()
{
	HandleShopItem();
}
void ShopManager::UpdateByMin()
{
	//精确更新 只有在0秒的时候进行更新处理  或者初始化的时候进行更新 
	SYSTEMTIME time;
	GetLocalTime(&time);
	static BYTE LogUpdateMin = 0xff;//上次更新记录的分钟数
	if (LogUpdateMin != 0xff && time.wSecond != 0 && time.wMinute == LogUpdateMin)
		return;//不进行更新
	LogUpdateMin = (BYTE)time.wMinute;//记录上次更新
	//处理所有商店物品的状态
	HandleShopItem();
}
void ShopManager::HandleShopItem()
{
	m_ShopItemStates.clear();
	HashMap<BYTE, tagShopConfig>::iterator Iter = g_FishServer.GetFishConfig().GetShopConfig().ShopMap.begin();
	for (; Iter != g_FishServer.GetFishConfig().GetShopConfig().ShopMap.end(); ++Iter)
	{
		HashMap<BYTE, tagShopItemConfig>::iterator IterItem = Iter->second.ShopItemMap.begin();
		for (; IterItem != Iter->second.ShopItemMap.end(); ++IterItem)
		{
			tagShopItemConfig* pItemConfig = &IterItem->second;
			if (ShopItemIsInTime(pItemConfig))
			{
				m_ShopItemStates.insert(HashMap<WORD, bool>::value_type((Iter->first << 8) + IterItem->first, true));
			}
			else
			{
				m_ShopItemStates.insert(HashMap<WORD, bool>::value_type((Iter->first << 8) + IterItem->first, false));
			}
		}
	}
}
bool ShopManager::ShopItemIsInTime(tagShopItemConfig* pItemConfig)
{
	if (!pItemConfig)
	{
		ASSERT(false);
		return false;
	}
	if (pItemConfig->TimeVec.empty())
		return true;
	time_t Now = time(NULL);
	std::vector<tagShopItemTimeConfig>::iterator Iter = pItemConfig->TimeVec.begin();
	for (; Iter != pItemConfig->TimeVec.end(); ++Iter)
	{
		if (Now >= Iter->BeginTime && Now <= (Iter->BeginTime + Iter->LastHour * 3600))
			return true;
	}
	return false;
}
void ShopManager::OnShellShopItem(CRoleEx* pRole, BYTE ShopID, BYTE ItemIndex, DWORD ItemSum)
{
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	if (ItemSum > 999)
	{
		ASSERT(false);
		return;
	}
	LC_Cmd_ShopItemResult msg;
	SetMsgInfo(msg,GetMsgType(Main_Shop, LC_ShopItemResult), sizeof(LC_Cmd_ShopItemResult));
	msg.Result = false;
	//购买物品 判断配置文件
	HashMap<BYTE, tagShopConfig>::iterator Iter = g_FishServer.GetFishConfig().GetShopConfig().ShopMap.find(ShopID);
	if (Iter == g_FishServer.GetFishConfig().GetShopConfig().ShopMap.end())
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}

	if (Iter->first != ShopID)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, tagShopItemConfig>::iterator IterItem = Iter->second.ShopItemMap.find(ItemIndex);
	if (IterItem == Iter->second.ShopItemMap.end())
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	if (IterItem->second.ShopItemIndex != ItemIndex)
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	//判断物品是否在时间内
	WORD Key = (Iter->first << 8) + IterItem->first;
	HashMap<WORD, bool>::iterator IterTime = m_ShopItemStates.find(Key);
	if (IterTime == m_ShopItemStates.end() || !IterTime->second)
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	if (ItemSum > 1 && !IterItem->second.IsCanPile)//不可以堆叠的物品 一次只可以购买一个
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	if (IterItem->second.ShopType == SIT_Entity && !pRole->GetRoleEntity().IsCanUserEntity())//为实体物品 但是未配置好玩家的现实地址 无法进行收取物品
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	if (IterItem->second.ShopType == SIT_PhonePay && !PhoneIsCanUse(pRole->GetRoleEntity().GetEntityInfo().Phone))//为实体物品 但是未配置好玩家的现实地址 无法进行收取物品
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	if (IterItem->second.ShopType == SIT_MonthCard && pRole->GetRoleInfo().MonthCardID != 0 && time(null) < pRole->GetRoleInfo().MonthCardEndTime)
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	//如果为兑换类型 
	if ((IterItem->second.ShopType == SIT_Entity || IterItem->second.ShopType == SIT_PhonePay) && pRole->GetRoleInfo().CashSum >= pRole->GetRoleVip().GetUseMedalSum())//玩家每天可以兑换的次数
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	
	if (static_cast<UINT64>(IterItem->second.PriceGlobel) * ItemSum  > MAXUINT32 ||
		static_cast<UINT64>(IterItem->second.PriceMabel) * ItemSum  > MAXUINT32 ||
		static_cast<UINT64>(IterItem->second.PriceCurrey) * ItemSum  > MAXUINT32
		)
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}

	//判断玩家的金钱 和 物品的价格
	if (!pRole->LostUserMoney(IterItem->second.PriceGlobel*ItemSum, IterItem->second.PriceMabel*ItemSum, IterItem->second.PriceCurrey*ItemSum,TEXT("商店购买物品扣除玩家货币")))
	{
		pRole->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	if (IterItem->second.ShopType == SIT_Entity)
	{
		//购买实体物品 发送命令
		//GO_Cmd_BuyEntityItem msg;
		//SetMsgInfo(msg, GetMsgType(Main_Operate, GO_BuyEntityItem), sizeof(GO_Cmd_BuyEntityItem));
		//TCHARCopy(msg.Addres, CountArray(msg.Addres), pRole->GetRoleEntity().GetEntityInfo().EntityItemUseAddres, _tcslen(pRole->GetRoleEntity().GetEntityInfo().EntityItemUseAddres));
		//TCHARCopy(msg.Name, CountArray(msg.Name), pRole->GetRoleEntity().GetEntityInfo().EntityItemUseName, _tcslen(pRole->GetRoleEntity().GetEntityInfo().EntityItemUseName));
		//msg.Phone = pRole->GetRoleEntity().GetEntityInfo().EntityItemUsePhone;
		//msg.ShopID = Iter->second.ShopID;
		//msg.ShopOnlyID = IterItem->second.ShopItemIndex;
		//msg.ItemID = IterItem->second.ItemInfo.ItemID;
		//msg.ItemSum = ItemSum;//购买的数量
		//msg.dwUserID = pRole->GetUserID();
		//g_FishServer.SendNetCmdToOperate(&msg);

		//购买实体物品 无须走运营服务器 直接发送命令到数据库去
		DBR_Cmd_AddRoleEntityItem msgDB;//发送到数据库去
		SetMsgInfo(msgDB, DBR_AddRoleEntityItem, sizeof(DBR_Cmd_AddRoleEntityItem));
		msgDB.dwUserID = pRole->GetUserID();
		msgDB.ItemID = IterItem->second.ItemInfo.ItemID;
		msgDB.ItemSum = IterItem->second.ItemInfo.ItemSum * ItemSum;
		msgDB.UseMedal = IterItem->second.PriceMabel * ItemSum;//花费的奖牌数量
		msgDB.NowMedalSum = pRole->GetRoleInfo().dwMedalNum;
		//设置现实的地址数据
		tagRoleAddressInfo& pInfo = pRole->GetRoleEntity().GetEntityInfo();
		TCHARCopy(msgDB.Name, CountArray(msgDB.Name), pInfo.EntityItemUseName, _tcslen(pInfo.EntityItemUseName));
		TCHARCopy(msgDB.IDEntity, CountArray(msgDB.IDEntity), pInfo.IdentityID, _tcslen(pInfo.IdentityID));
		msgDB.Phone = pInfo.EntityItemUsePhone;
		TCHARCopy(msgDB.Addres, CountArray(msgDB.Addres), pInfo.EntityItemUseAddres, _tcslen(pInfo.EntityItemUseAddres));
		g_FishServer.SendNetCmdToSaveDB(&msgDB);
		g_FishServer.GetAnnouncementManager().OnAddNewAnnouncementOnce(pRole->GetRoleInfo().NickName, Iter->second.ShopID, IterItem->second.ShopItemIndex);//添加一条公告

		pRole->ChangeRoleCashSum(1);//添加次数

		pRole->ChangeRoleShareStates(false);
	}
	else if (IterItem->second.ShopType == SIT_PhonePay)
	{
		//花费充值
		GO_Cmd_PhonePay msg;
		SetMsgInfo(msg, GetMsgType(Main_Operate, GO_PhonePay), sizeof(GO_Cmd_PhonePay));
		msg.dwUserID = pRole->GetUserID();
		msg.ShopID = Iter->second.ShopID;
		msg.ShopOnlyID = IterItem->second.ShopItemIndex;
		msg.ShopSum = ItemSum;//物品的数量
		msg.Phone = pRole->GetRoleEntity().GetEntityInfo().Phone;
		g_FishServer.SendNetCmdToOperate(&msg);

		pRole->ChangeRoleCashSum(1);//添加次数
		return;
	}
	else if (IterItem->second.ShopType == SIT_MonthCard)
	{
		//玩家购买月卡物品
		DWORD ItemParam = g_FishServer.GetFishConfig().GetItemParam(IterItem->second.ItemInfo.ItemID);
		if (ItemParam == 0 || !pRole->GetRoleMonthCard().SetRoleMonthCardInfo(ConvertDWORDToBYTE(ItemParam)))
		{
			pRole->ChangeRoleGlobe(IterItem->second.PriceCurrey*ItemSum, true);
			pRole->ChangeRoleMedal(IterItem->second.PriceMabel*ItemSum, TEXT("商店购买物品 月卡不存在 归还玩家奖牌"));
			pRole->ChangeRoleCurrency(IterItem->second.PriceCurrey*ItemSum, TEXT("商店购买物品 月卡不存在 归还玩家钻石"));
			pRole->SendDataToClient(&msg);
			ASSERT(false);
			pRole->SendDataToClient(&msg);
			return;
		}
	}
	else
	{
		//给玩家添加物品 物品数量
		if (IterItem->second.ShopType == SIT_GlobelBag)
		{
			g_DBLogManager.LogToDB(pRole->GetUserID(), LT_GlobelBag, static_cast<int>(ItemSum),0, TEXT("购买聚宝盆"), SendLogDB);
		}

		tagItemOnce pOnce = IterItem->second.ItemInfo;
		pOnce.ItemSum *= ItemSum;
		if (!pRole->GetItemManager().OnAddUserItem(pOnce))
		{
			pRole->ChangeRoleGlobe(IterItem->second.PriceCurrey*ItemSum,true);
			pRole->ChangeRoleMedal(IterItem->second.PriceMabel*ItemSum,TEXT("商店购买物品 添加物品失败 归还玩家奖牌"));
			pRole->ChangeRoleCurrency(IterItem->second.PriceCurrey*ItemSum, TEXT("商店购买物品 添加物品失败 归还玩家钻石"));
			pRole->SendDataToClient(&msg);
			ASSERT(false);
			pRole->SendDataToClient(&msg);
			return;
		}
	}
	//玩家购买成功了
	//发送命令到客户端去
	msg.Result = true;
	pRole->SendDataToClient(&msg);
	return;
}
void ShopManager::Destroy()
{
}