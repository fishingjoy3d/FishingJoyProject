#include "Stdafx.h"
#include "RoleEx.h"
#include "FishServer.h"
#include "RoleLauncherManager.h"
RoleLauncherManager::RoleLauncherManager()
{
	m_LauncherStates = 0;
	m_pConfig = null;
	m_pRole = null;
	m_ItemToLauncherMap.clear();
}
RoleLauncherManager::~RoleLauncherManager()
{
	m_LauncherStates = 0;
	m_pConfig = null;
	m_pRole = null;
	m_ItemToLauncherMap.clear();
}
void RoleLauncherManager::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	m_LauncherStates = 0;
	m_pConfig = g_FishServer.GetTableManager()->GetGameConfig();
	if (!m_pConfig)
	{
		ASSERT(false);
		return;
	}
	time_t pNow = time(null);
	m_pRole = pRole;
	m_ItemToLauncherMap.clear();
	for (int i = 0; i < MAX_LAUNCHER_NUM; ++i)
	{
		DWORD ItemID = 0;
		DWORD ItemSum = 0;
		m_pConfig->GoodsInfo(i, (int&)ItemID, (int&)ItemSum);
		if (ItemID == 0 || ItemSum == 0)
		{
			//当前炮台无须物品 直接设置为 可用
			m_LauncherStates |= (1 << (i + 1));//设置炮台的状态可用
			continue;
		}
		m_ItemToLauncherMap.insert(HashMap<DWORD, BYTE>::value_type(ItemID, i));

		DWORD AllItemSum = m_pRole->GetItemManager().QueryItemCount(ItemID);
		if (AllItemSum >= ItemSum)
		{
			m_LauncherStates |= (1 << (i + 1));//设置炮台的状态可用
			continue;
		}

		//判断玩家VIP等级
		if (m_pRole->GetRoleInfo().VipLevel != 0)
		{
			HashMap<BYTE, tagVipOnce>::iterator Iter = g_FishServer.GetFishConfig().GetVipConfig().VipMap.find(m_pRole->GetRoleInfo().VipLevel);
			if (Iter != g_FishServer.GetFishConfig().GetVipConfig().VipMap.end())
			{
				if (Iter->second.CanUseLauncherMap.count(i) == 1)
				{
					//可以使用炮
					m_LauncherStates |= (1 << (i + 1));//设置炮台的状态可用
					continue;
				}
			}
		}

		//判断玩家月卡
		//if (m_pRole->GetRoleInfo().MonthCardID != 0 && pNow < m_pRole->GetRoleInfo().MonthCardEndTime)
		//{
		//	HashMap<BYTE, tagMonthCardOnce>::iterator Iter= g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(m_pRole->GetRoleInfo().MonthCardID);
		//	if (Iter != g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
		//	{
		//		if (Iter->second.CanUseLauncherMap.count(i) == 1)
		//		{
		//			//可以使用炮
		//			m_LauncherStates |= (1 << (i + 1));//设置炮台的状态可用
		//			continue;
		//		}
		//	}
		//}
	}
	//将数据发送到客户端去.
	LC_Cmd_LauncherData msg;
	SetMsgInfo(msg, GetMsgType(Main_Launcher, LC_LauncherData), sizeof(LC_Cmd_LauncherData));
	msg.LauncherData = m_LauncherStates;
	m_pRole->SendDataToClient(&msg);
}
void RoleLauncherManager::OnAddItem(DWORD ItemID)
{
	if (!m_pConfig || !m_pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, BYTE>::iterator Iter = m_ItemToLauncherMap.find(ItemID);
	if (Iter == m_ItemToLauncherMap.end())
		return;
	//有物品
	if (IsCanUserLauncherByID(Iter->second))
		return;
	//不可用变为可用
	int nItemID = 0;
	int nItemSum = 0;
	int Launcher = Iter->second;
	m_pConfig->GoodsInfo(Launcher, nItemID, nItemSum);
	DWORD AllItemSum = m_pRole->GetItemManager().QueryItemCount(nItemID);
	if (AllItemSum >= ConvertIntToDWORD(nItemSum))
	{
		m_LauncherStates |= (1 << (Launcher + 1));
		//发送命令到客户端去 
		LC_Cmd_LauncherData msg;
		SetMsgInfo(msg, GetMsgType(Main_Launcher, LC_LauncherData), sizeof(LC_Cmd_LauncherData));
		msg.LauncherData = m_LauncherStates;
		m_pRole->SendDataToClient(&msg);

		//判断玩家当前炮是否变化了 我们想要判断
		CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());
		if (pRole && pRole->GetLauncherType() == Launcher)
		{
			pRole->OnResetRoleLauncher();
		}
	}
}
void RoleLauncherManager::OnDelItem(DWORD ItemID)
{
	if (!m_pConfig || !m_pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, BYTE>::iterator Iter = m_ItemToLauncherMap.find(ItemID);
	if (Iter == m_ItemToLauncherMap.end())
		return;
	if (!IsCanUserLauncherByID(Iter->second))
		return;
	//因为可以使用 我们想要判断是否为VIP给予的 永久的
	if (m_pRole->GetRoleInfo().VipLevel != 0)
	{
		HashMap<BYTE, tagVipOnce>::iterator IterVip = g_FishServer.GetFishConfig().GetVipConfig().VipMap.find(m_pRole->GetRoleInfo().VipLevel);
		if (IterVip != g_FishServer.GetFishConfig().GetVipConfig().VipMap.end())
		{
			if (IterVip->second.CanUseLauncherMap.count(Iter->second) == 1)
			{
				//当前炮为VIP赋予的炮 不要删除
				return;
			}
		}
	}

	int nItemID = 0;
	int nItemSum = 0;
	int Launcher = Iter->second;
	m_pConfig->GoodsInfo(Launcher, nItemID, nItemSum);
	DWORD AllItemSum = m_pRole->GetItemManager().QueryItemCount(nItemID);
	if (AllItemSum < ConvertIntToDWORD(nItemSum))
	{
		//当前炮台无法使用
		m_LauncherStates ^= (1 << (Launcher + 1));
		//发送命令到客户端去
		LC_Cmd_LauncherData msg;
		SetMsgInfo(msg, GetMsgType(Main_Launcher, LC_LauncherData), sizeof(LC_Cmd_LauncherData));
		msg.LauncherData = m_LauncherStates;
		m_pRole->SendDataToClient(&msg);

		CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());
		if (pRole && pRole->GetLauncherType() == Launcher)
			pRole->OnResetRoleLauncher();//因为失去炮 我们处理下玩家炮台状态当前的
	}
}
bool RoleLauncherManager::IsCanUserLauncherByID(BYTE LauncherID)
{
	if (LauncherID >= 32 || LauncherID >= MAX_LAUNCHER_NUM)
	{
		ASSERT(false);
		return false;
	}
	return  ((m_LauncherStates & (1 << (LauncherID + 1))) != 0);
}
void RoleLauncherManager::OnVipLevelChange(BYTE OldVipLevel, BYTE NewVipLevel)
{
	//当VIP发生变化的时候 我们先移除旧的VIP炮台数据 在 添加进行的炮台数据
	if (OldVipLevel != 0)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = g_FishServer.GetFishConfig().GetVipConfig().VipMap.find(OldVipLevel);
		if (Iter != g_FishServer.GetFishConfig().GetVipConfig().VipMap.end())
		{
			set<BYTE>::iterator IterLauncher = Iter->second.CanUseLauncherMap.begin();
			for (; IterLauncher != Iter->second.CanUseLauncherMap.end(); ++IterLauncher)
			{
				m_LauncherStates ^= (1 << (*IterLauncher + 1));
			}
		}
	}
	if (NewVipLevel != 0)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = g_FishServer.GetFishConfig().GetVipConfig().VipMap.find(NewVipLevel);
		if (Iter != g_FishServer.GetFishConfig().GetVipConfig().VipMap.end())
		{
			set<BYTE>::iterator IterLauncher = Iter->second.CanUseLauncherMap.begin();
			for (; IterLauncher != Iter->second.CanUseLauncherMap.end(); ++IterLauncher)
			{
				m_LauncherStates |= (1 << (*IterLauncher + 1));//设置炮台的状态可用
			}
		}
	}
	LC_Cmd_LauncherData msg;
	SetMsgInfo(msg, GetMsgType(Main_Launcher, LC_LauncherData), sizeof(LC_Cmd_LauncherData));
	msg.LauncherData = m_LauncherStates;
	m_pRole->SendDataToClient(&msg);

	CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());
	if (pRole)
		pRole->OnResetRoleLauncher();
}
bool RoleLauncherManager::IsCanUseLauncherAllTime(BYTE LauncherID)
{
	if (!IsCanUserLauncherByID(LauncherID))
		return false;
	//拥有的炮 判断是否为VIP的
	if (m_pRole->GetRoleInfo().VipLevel != 0)
	{
		HashMap<BYTE, tagVipOnce>::iterator IterVip = g_FishServer.GetFishConfig().GetVipConfig().VipMap.find(m_pRole->GetRoleInfo().VipLevel);
		if (IterVip != g_FishServer.GetFishConfig().GetVipConfig().VipMap.end())
		{
			if (IterVip->second.CanUseLauncherMap.count(LauncherID) == 1)
				return true;
		}
	}
	//炮不是VIP的炮 我们继续判断 是否永久拥有物品
	int nItemID = 0;
	int nItemSum = 0;
	int Launcher = LauncherID;
	m_pConfig->GoodsInfo(Launcher, nItemID, nItemSum);
	return m_pRole->GetItemManager().GetItemIsAllExists(nItemID, nItemSum);
}
//void RoleLauncherManager::OnMonthCardChange(BYTE OldMonthCardID, BYTE NewMonthCardID)
//{
//	if (OldMonthCardID != 0)
//	{
//		HashMap<BYTE, tagMonthCardOnce>::iterator Iter= g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(OldMonthCardID);
//		if (Iter != g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
//		{
//			set<BYTE>::iterator IterLauncher = Iter->second.CanUseLauncherMap.begin();
//			for (; IterLauncher != Iter->second.CanUseLauncherMap.end(); ++IterLauncher)
//			{
//				m_LauncherStates ^= (1 << (*IterLauncher + 1));
//			}
//		}
//	}
//	if (NewMonthCardID != 0)
//	{
//		HashMap<BYTE, tagMonthCardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(NewMonthCardID);
//		if (Iter != g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
//		{
//			set<BYTE>::iterator IterLauncher = Iter->second.CanUseLauncherMap.begin();
//			for (; IterLauncher != Iter->second.CanUseLauncherMap.end(); ++IterLauncher)
//			{
//				m_LauncherStates += (1 << (*IterLauncher + 1));//设置炮台的状态可用
//			}
//		}
//	}
//	LC_Cmd_LauncherData msg;
//	SetMsgInfo(msg, GetMsgType(Main_Launcher, LC_LauncherData), sizeof(LC_Cmd_LauncherData));
//	msg.LauncherData = m_LauncherStates;
//	m_pRole->SendDataToClient(&msg);
//
//	CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());
//	if (pRole)
//		pRole->OnResetRoleLauncher();
//}