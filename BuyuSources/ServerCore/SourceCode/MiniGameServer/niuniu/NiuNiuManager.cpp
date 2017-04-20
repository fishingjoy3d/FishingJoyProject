#include "Stdafx.h"
#include "NiuNiuManager.h"
#include "../CommonFile/DBLogManager.h"
#include "..\FishServer.h"
extern void SendLogDB(NetCmd* pCmd);
#define IsOnceBrandValue(a,b,c,d) a == b && a==c && a==d
NiuNiuManager::NiuNiuManager()
{

}
NiuNiuManager::~NiuNiuManager()
{
	OnDestroy();
}
void NiuNiuManager::SetTableStates(NiuNiuTableStates States)
{
	m_TableStates = States;
	m_TableStatesLog = timeGetTime();
}
bool NiuNiuManager::OnInit()
{
	SetTableStates(NNTS_WriteBegin);//设置初始化的时候 桌子状态为等待开始
	m_SystemGlobelSum = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.InitGlobelSum;//系统金币
	//初始化一些数据
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum; ++i)
		m_TableBetAreaGlobel[i] = 0;
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum + 1; ++i)
	{
		m_TableBrandResult[i] = 0;
		for (BYTE j = 0; j < MAX_NIUNIU_BrandSum; ++j)
			m_TableBrandArray[i][j] = 0;
	}
	for (BYTE i = 0; i < MAX_VIPSEAT_Sum; ++i)
	{
		m_VipSeatVec[i].dwUserID = 0;
		m_VipSeatVec[i].NonBetGlobelSum = 0;
	}
	m_TableBankerUserID = 0;
	m_TableBankerUseGameSum = 0;
	m_TableWriteBankerVec.clear();
	m_TableUserMap.clear();
	m_IsNeedClearBankerInfo = false;
	return true;
}
void NiuNiuManager::OnUpdate(DWORD dwTimer)
{
	static DWORD NiuNiuUpdateTime = 0;
	if (NiuNiuUpdateTime == 0 || dwTimer - NiuNiuUpdateTime >= 1000) //1s更新一次
	{
		NiuNiuUpdateTime = dwTimer;
		UpdateTableIsCanBegin(timeGetTime());
		UpdateTableIsCanEnd(timeGetTime());
		UpdateTableInfo(timeGetTime());
		m_oxrobotmanager.Update();
	}
}
void NiuNiuManager::OnDestroy()
{
}
void NiuNiuManager::OnRoleJoinNiuNiuTable(DWORD dwUserID)
{
	//玩家进入试图进入牛牛桌子
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_RoleJoinTable msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_RoleJoinTable), sizeof(MG_Cmd_RoleJoinTable));
	msg.dwUserID = dwUserID;
	msg.Result = false;
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter != m_TableUserMap.end())
	{
		/*ASSERT(false);
		pRole->SendDataToGameServer(&msg);*/

		//立刻让玩家离开
		OnRoleLeaveNiuNiuTable(dwUserID);

		//m_TableUserMap.erase(Iter);
	}
	if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinGlobelSum)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (m_TableStates == NNTS_WriteBegin)
	{
		//因为有人进入了 
		SetTableStates(NNTS_Begin);
		OnClearTableInfo();//清理桌子上的数据
	}

	msg.Result = true;
	tagNiuNiuRoleInfo pInfo;
	pInfo.UserID = dwUserID;
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum;++i)
		pInfo.BetGlobel[i] = 0;
	pInfo.IsBankerUser = false;
	pInfo.IsInWriteBanakerVec = false;
	pInfo.IsInVipSeat = false;
	pInfo.VipSeat = 0;
	pInfo.IsRobot = g_FishServer.GetRobotManager().IsRobot(dwUserID);

	m_TableUserMap.insert(HashMap<DWORD, tagNiuNiuRoleInfo>::value_type(dwUserID,pInfo));
	//继续设置数据到命令里面
	msg.TableStates = m_TableStates;
	DWORD NowTime = timeGetTime();
	if (m_TableStates == NNTS_Begin)
		msg.TableStatesUpdateSec = static_cast<BYTE>((g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.BeginWriteSec * 1000 - (NowTime - m_TableStatesLog)) / 1000);
	else if (m_TableStates == NNTS_End)
		msg.TableStatesUpdateSec = static_cast<BYTE>((g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.EndWriteSec * 1000 - (NowTime - m_TableStatesLog)) / 1000);
	else
		msg.TableStatesUpdateSec = 0;
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum; ++i)
		msg.TableBetAreaGlobel[i] = m_TableBetAreaGlobel[i];
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum + 1; ++i)
	{
		msg.TableBrandResult[i] = m_TableBrandResult[i];
		for (BYTE j = 0; j < MAX_NIUNIU_BrandSum; ++j)
			msg.TableBrandArray[i][j] = m_TableBrandArray[i][j];
	}
	msg.TableBankerInfo.dwUserID = m_TableBankerUserID;
	msg.TableBankerUseGameSum = m_TableBankerUseGameSum;
	if (msg.TableBankerInfo.dwUserID != 0)
	{
		Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(msg.TableBankerInfo.dwUserID);
		if (!pBankerRole)
		{
			msg.TableBankerInfo.dwUserID = 0;
			msg.TableBankerInfo.dwFaceID = 0;
			msg.TableBankerUseGameSum = 0;
			msg.TableBankerInfo.NickName[0] = 0;
		}
		else
		{
			msg.TableBankerInfo.GlobelSum = pBankerRole->GetGlobelSum();
			msg.TableBankerInfo.dwFaceID = pBankerRole->GetFaceID();
			TCHARCopy(msg.TableBankerInfo.NickName, CountArray(msg.TableBankerInfo.NickName), pBankerRole->GetNickName(), _tcslen(pBankerRole->GetNickName()));
		}
	}
	for (BYTE i = 0; i < MAX_SHOWBRAND_Sum; ++i)
	{
		msg.TableWriteBankerList[i].dwUserID = 0;
		msg.TableWriteBankerList[i].GlobelSum = 0;
		msg.TableWriteBankerList[i].NickName[0] = 0;
		msg.TableWriteBankerList[i].dwFaceID = 0;
	}
	if (!m_TableWriteBankerVec.empty())
	{
		vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
		for (BYTE i = 0; Iter != m_TableWriteBankerVec.end() && i < MAX_SHOWBRAND_Sum;)
		{
			Role* pDestRole = g_FishServer.GetRoleManager().QueryRole(*Iter);
			if (!pDestRole)
			{
				//++i;
				++Iter;
				continue;
			}
			msg.TableWriteBankerList[i].dwUserID = *Iter;
			msg.TableWriteBankerList[i].GlobelSum = pDestRole->GetGlobelSum();
			msg.TableWriteBankerList[i].dwFaceID = pDestRole->GetFaceID();
			TCHARCopy(msg.TableWriteBankerList[i].NickName, CountArray(msg.TableWriteBankerList[i].NickName), pDestRole->GetNickName(), _tcslen(pDestRole->GetNickName()));
			++i;
			++Iter;
		}
	}
	//贵宾席的数据
	for (BYTE i = 0; i < MAX_VIPSEAT_Sum; ++i)
	{
		if (m_VipSeatVec[i].dwUserID == 0)
		{
			msg.VipSeatList[i].dwUserID = 0;
			msg.VipSeatList[i].GlobelSum = 0;
			msg.VipSeatList[i].NickName[0] = 0;
			msg.VipSeatList[i].dwFaceID = 0;
		}
		else
		{
			Role* pDestRole = g_FishServer.GetRoleManager().QueryRole(m_VipSeatVec[i].dwUserID);
			if (pDestRole)
			{
				msg.VipSeatList[i].dwUserID = m_VipSeatVec[i].dwUserID;
				msg.VipSeatList[i].GlobelSum = pDestRole->GetGlobelSum();
				msg.VipSeatList[i].dwFaceID = pDestRole->GetFaceID();
				TCHARCopy(msg.VipSeatList[i].NickName, CountArray(msg.VipSeatList[i].NickName), pDestRole->GetNickName(), _tcslen(pDestRole->GetNickName()));
			}
			else
			{
				msg.VipSeatList[i].dwUserID = 0;
				msg.VipSeatList[i].GlobelSum = 0;
				msg.VipSeatList[i].NickName[0] = 0;
				msg.VipSeatList[i].dwFaceID = 0;
			}
		}
	}
	msg.TableGameSum = m_NiuNiuResultLog.LogSum;
	msg.TableResultLog = m_NiuNiuResultLog.ResultLog;

	pRole->SendDataToGameServer(&msg);
}
void NiuNiuManager::OnRoleLeaveNiuNiuTable(DWORD dwUserID)
{
	//当玩家离开牛牛桌子的时候 
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		//ASSERT(false);
		return;
	}
	if (Iter->second.IsInWriteBanakerVec)
	{
		vector<DWORD>::iterator IterFind = find(m_TableWriteBankerVec.begin(), m_TableWriteBankerVec.end(), dwUserID);
		if (IterFind != m_TableWriteBankerVec.end())
		{
			m_TableWriteBankerVec.erase(IterFind);
		}
	}
	if (Iter->second.IsInVipSeat && m_VipSeatVec[Iter->second.VipSeat].dwUserID == Iter->first)
	{
		/*m_VipSeatVec[Iter->second.VipSeat].dwUserID = 0;
		m_VipSeatVec[Iter->second.VipSeat].NonBetGlobelSum = 0;*/
		OnRoleLeaveVipSeat(dwUserID);//指定玩家离开VIP席
	}
	if (Iter->second.IsBankerUser && dwUserID == m_TableBankerUserID)
	{
		if (m_TableStates == NNTS_Begin)//如果为下注阶段
		{
			//庄家下注了 立刻由系统接手
			m_TableBankerUserID = 0;
			m_TableBankerUseGameSum = 0;
			OnBankerUserChange();//庄家发生变化了
		}
		else if (m_TableStates == NNTS_End)
		{
			m_IsNeedClearBankerInfo = true;//在开牌阶段 清理下庄家
		}
	}
	m_TableUserMap.erase(Iter);
}
void NiuNiuManager::OnRoleBetGlobel(DWORD dwUserID, BYTE BetIndex, DWORD AddGlobel)
{
	//玩家下注
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_RoleBetGlobel msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_RoleBetGlobel), sizeof(MG_Cmd_RoleBetGlobel));
	msg.dwUserID = dwUserID;
	msg.AddGlobel = AddGlobel;
	msg.Index = BetIndex;
	msg.Result = false;
	if (BetIndex >= MAX_NIUNIU_ClientSum)
	{
		pRole->SendDataToGameServer(&msg);
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (m_TableStates != NNTS_Begin)
	{
		//开牌阶段 无法下注
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (m_TableBankerUserID == dwUserID)
	{
		//庄家不可以下注
		pRole->SendDataToGameServer(&msg);
		return;
	}
	bool IsCanBetGlobel = true;

	DWORD RoleBetAllGlobel = 0;
	UINT64 TableBetAllGbel = 0;
	for (BYTE i = 0; i<MAX_NIUNIU_ClientSum; ++i)
	{
		RoleBetAllGlobel += Iter->second.BetGlobel[i];
		TableBetAllGbel += m_TableBetAreaGlobel[i];
	}
	if ((RoleBetAllGlobel + AddGlobel) * g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.MaxRate >(pRole->GetGlobelSum() + RoleBetAllGlobel))//金币超出最大限制了
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (RoleBetAllGlobel + AddGlobel > g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.MaxAddGlobel)//当前下注区的金币太多
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//不可以操出庄家的限制 
	if (m_TableBankerUserID != 0)
	{
		Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
		if (pBankerRole && (TableBetAllGbel + AddGlobel)* g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.MaxRate > pBankerRole->GetGlobelSum())
		{
			pRole->SendDataToGameServer(&msg);
			return;
		}
	}

	//可以下注了
	msg.Result = true;
	//1.扣除玩家金币
	pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() - AddGlobel);
	//2.添加下注区数据
	Iter->second.BetGlobel[BetIndex] += AddGlobel;
	m_TableBetAreaGlobel[BetIndex] += AddGlobel;
	if (!Iter->second.IsRobot)
		g_DBLogManager.LogToDB(dwUserID, LogType::LT_NiuNiu, AddGlobel, BetIndex, TEXT("玩家牛牛下注"), SendLogDB);//Param 表示 下注区
	pRole->SendDataToGameServer(&msg);

	TableBetAllGbel += AddGlobel;
	if (m_TableBankerUserID != 0)
	{
		Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
		if (pBankerRole && (TableBetAllGbel + 1000)* g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.MaxRate > pBankerRole->GetGlobelSum())
		{
			IsCanBetGlobel = false;
		}
	}

	if (Iter->second.IsInVipSeat)
	{
		MG_Cmd_VipSeatGlobelChange msg;
		SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_VipSeatGlobelChange), sizeof(MG_Cmd_VipSeatGlobelChange));
		msg.VipSeat = Iter->second.VipSeat;
		msg.GlobelSum = AddGlobel;
		msg.Index = BetIndex;	
		HashMap<DWORD, tagNiuNiuRoleInfo>::iterator IterRole = m_TableUserMap.begin();
		for (; IterRole != m_TableUserMap.end(); ++IterRole)
		{
			Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterRole->first);
			if (!pRole)
				continue;
			msg.dwUserID = IterRole->first;
			pRole->SendDataToGameServer(&msg);
		}
	}
	if (!IsCanBetGlobel)
	{
		MG_Cmd_TableStopUpdate msgTable;
		SetMsgInfo(msgTable, GetMsgType(Main_NiuNiu, MG_TableStopUpdate), sizeof(MG_Cmd_TableStopUpdate));
		HashMap<DWORD, tagNiuNiuRoleInfo>::iterator IterRole = m_TableUserMap.begin();
		for (; IterRole != m_TableUserMap.end(); ++IterRole)
		{
			Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterRole->first);
			if (!pRole)
				continue;
			msgTable.dwUserID = IterRole->first;

			pRole->SendDataToGameServer(&msgTable);
		}
	}
}

void NiuNiuManager::OnRoleGetWriteBankerList(DWORD dwUserID)
{
	//玩家进入庄家排队列表
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	//发生命令到客户端去
	MG_Cmd_GetBankerList msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_GetBankerList), sizeof(MG_Cmd_GetBankerList));
	msg.dwUserID = dwUserID;
	msg.dwMySeatIndex = 0xffffffff;
	for (BYTE i = 0; i < MAX_SHOWBRAND_Sum; ++i)
	{
		msg.TableWriteBankerList[i].dwUserID = 0;
		msg.TableWriteBankerList[i].GlobelSum = 0;
		msg.TableWriteBankerList[i].NickName[0] = 0;
		msg.TableWriteBankerList[i].dwFaceID = 0;
	}
	if (!m_TableWriteBankerVec.empty())
	{
		vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
		for (BYTE i = 0; Iter != m_TableWriteBankerVec.end() && i < MAX_SHOWBRAND_Sum;)
		{
			Role* pDestRole = g_FishServer.GetRoleManager().QueryRole(*Iter);
			if (!pDestRole)
			{
				//++i;
				++Iter;
				continue;
			}
			if (*Iter == dwUserID)
				msg.dwMySeatIndex = i;

			msg.TableWriteBankerList[i].dwUserID = *Iter;
			msg.TableWriteBankerList[i].GlobelSum = pDestRole->GetGlobelSum();
			msg.TableWriteBankerList[i].dwFaceID = pDestRole->GetFaceID();
			TCHARCopy(msg.TableWriteBankerList[i].NickName, CountArray(msg.TableWriteBankerList[i].NickName), pDestRole->GetNickName(), _tcslen(pDestRole->GetNickName()));
			++i;
			++Iter;
		}
	}
	pRole->SendDataToGameServer(&msg);
}
void NiuNiuManager::OnRoleJoinWriteBankerList(DWORD dwUserID)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	//玩家试图进行上庄 
	MG_Cmd_RoleJoinBankerList msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_RoleJoinBankerList), sizeof(MG_Cmd_RoleJoinBankerList));
	msg.dwUserID = dwUserID;
	msg.Result = false;
	msg.SeatIndex = 0xffffffff;
	//1.已经在列表里面了
	/*vector<DWORD>::iterator IterFind = find(m_TableWriteBankerVec.begin(), m_TableWriteBankerVec.end(), dwUserID);
	if (IterFind != m_TableWriteBankerVec.end())
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}*/
	if (Iter->second.IsInWriteBanakerVec)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//2.判断玩家金币是否足够
	if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//3.判断是否为庄家
	if (dwUserID == m_TableBankerUserID && !m_IsNeedClearBankerInfo)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//可以上庄了
	//
	msg.Result = true;
	msg.SeatIndex = m_TableWriteBankerVec.size();
	Iter->second.IsInWriteBanakerVec = true;
	//扣除金币
	//pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() - g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum);
	//加入到列表里面去
	m_TableWriteBankerVec.push_back(dwUserID);//加入到列表里面去
	//
	if (msg.SeatIndex < MAX_SHOWBRAND_Sum)
	{
		OnWriteBankerChange();
	}
	else
	{
		pRole->SendDataToGameServer(&msg);
	}
}
void NiuNiuManager::OnRoleGetWriteBankerFirstSeat(DWORD dwUserID)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_RoleGetBankerFirstSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_RoleGetBankerFirstSeat), sizeof(MG_Cmd_RoleGetBankerFirstSeat));
	msg.dwUserID = dwUserID;
	msg.Result = false;
	//1.不在列表里面
	if (!Iter->second.IsInWriteBanakerVec)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	vector<DWORD>::iterator IterFind = find(m_TableWriteBankerVec.begin(), m_TableWriteBankerVec.end(), dwUserID);
	if (IterFind == m_TableWriteBankerVec.end())
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//2.是否为头名
	if (*(m_TableWriteBankerVec.begin()) == dwUserID)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//3.金币是否足够
	if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetNextBankerNeedGlobel)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//可以抢庄
	msg.Result = true;
	//1.扣除金币
	pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() - g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetNextBankerNeedGlobel);
	//2.改变位置
	m_TableWriteBankerVec.erase(IterFind);
	/*vector<DWORD> tempVec;
	tempVec.push_back(dwUserID);
	copy(tempVec.begin() + 1, m_TableWriteBankerVec.begin(), m_TableWriteBankerVec.end());
	m_TableWriteBankerVec = tempVec;*/

	m_TableWriteBankerVec.insert(m_TableWriteBankerVec.begin(), dwUserID);

	//3.
	OnWriteBankerChange();//列表变化了
	pRole->SendDataToGameServer(&msg);
}
void NiuNiuManager::OnRoleLeaveWriteBankerList(DWORD dwUserID)
{
	//玩家离开上庄排队列表 
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_RoleLeaveBankerList msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_RoleLeaveBankerList), sizeof(MG_Cmd_RoleLeaveBankerList));
	msg.dwUserID = dwUserID;
	msg.Result = false;
	//1.不在队列里面
	if (!Iter->second.IsInWriteBanakerVec)
	{
		if (pRole)
			pRole->SendDataToGameServer(&msg);
		return;
	}
	vector<DWORD>::iterator IterFind = find(m_TableWriteBankerVec.begin(), m_TableWriteBankerVec.end(), dwUserID);
	if (IterFind == m_TableWriteBankerVec.end())
	{
		if (pRole)
			pRole->SendDataToGameServer(&msg);
		return;
	}
	//2.让玩家离开队列 并且退钱
	//因为玩家离开队列 我们触发更新命令
	size_t Index = IterFind - m_TableWriteBankerVec.begin();
	if (Index < MAX_SHOWBRAND_Sum)
	{
		OnWriteBankerChange();
	}
	else
	{
		msg.Result = true;
		if (pRole)
			pRole->SendDataToGameServer(&msg);
	}
	m_TableWriteBankerVec.erase(IterFind);//移除数据
	Iter->second.IsInWriteBanakerVec = false;
	
}
void NiuNiuManager::OnRoleCanelBanker(DWORD dwUserID)
{
	//玩家下庄
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_RoleCanelBankerSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_RoleCanelBankerSeat), sizeof(MG_Cmd_RoleCanelBankerSeat));
	msg.dwUserID = dwUserID;
	msg.Result = false;
	//不是庄家不可以下庄
	if (!Iter->second.IsBankerUser || dwUserID != m_TableBankerUserID)
	{
		if (pRole)
			pRole->SendDataToGameServer(&msg);
		return;
	}
	//让玩家下庄
	Iter->second.IsBankerUser = false;
	msg.Result = true;
	if (m_TableStates == NNTS_Begin)
	{
		//应该让玩家的金币 暂时冻结1分钟 


		//下注阶段 玩家下庄 立刻又系统补上
		m_TableBankerUserID = 0;
		m_TableBankerUseGameSum = 0;
		OnBankerUserChange();
	}
	else if (m_TableStates == NNTS_End)
	{
		//开牌阶段 玩家下庄 直接下注 设置
		//设置玩家下庄成功 
		m_IsNeedClearBankerInfo = true;
	}
	if (pRole)
		pRole->SendDataToGameServer(&msg);
}
void NiuNiuManager::OnRoleLeaveMiniGameServer(DWORD dwUserID)
{
	OnRoleLeaveNiuNiuTable(dwUserID);
}
void NiuNiuManager::OnRoleGlobelChange(DWORD dwUserID,int ChangeSum)//当发生用户充值的时候
{
	if (m_TableBankerUserID != 0 && m_TableBankerUserID == dwUserID)
	{
		Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
		if (!pBankerRole)
			return;

		MG_Cmd_BankerUserGlobelChange msg;
		SetMsgInfo(msg, GetMsgType(Main_MiniGame, MG_BankerUserGlobelChange), sizeof(MG_Cmd_BankerUserGlobelChange));
		msg.dwBankerGlobelSum = pBankerRole->GetGlobelSum();
		msg.dwBankerUserID = dwUserID;

		HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.begin();
		for (; Iter != m_TableUserMap.end(); ++Iter)
		{
			Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
			if (!pRole)
				continue;
			msg.dwUserID = Iter->first;
			pRole->SendDataToGameServer(&msg);
		}
	}
	else
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
		if (!pRole)
			return;
		//如果为VIP席位的话
		HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
		if (Iter == m_TableUserMap.end())
			return;
		if (!Iter->second.IsInVipSeat)
			return;
		//如果玩家在VIP位置上的时候 
		MG_Cmd_VipGlobelChange msg;
		SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_VipGlobelChange), sizeof(MG_Cmd_VipGlobelChange));
		msg.VipUserID = dwUserID;
		msg.VipGlobelSum = pRole->GetGlobelSum();
		HashMap<DWORD, tagNiuNiuRoleInfo>::iterator IterRole = m_TableUserMap.begin();
		for (; IterRole != m_TableUserMap.end(); ++IterRole)
		{
			Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterRole->first);
			if (!pRole)
				continue;
			msg.dwUserID = IterRole->first;
			pRole->SendDataToGameServer(&msg);
		}
	}
}
void NiuNiuManager::OnClearTableInfo()
{
	//每局开始下注之前 先清理下数据
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum; ++i)
		m_TableBetAreaGlobel[i] = 0;
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum + 1; ++i)
	{
		m_TableBrandResult[i] = 0;
		for (BYTE j = 0; j < MAX_NIUNIU_BrandSum; ++j)
			m_TableBrandArray[i][j] = 0;
	}
}
void NiuNiuManager::OnWriteBankerChange()
{
	MG_Cmd_BankerListChange msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_BankerListChange), sizeof(MG_Cmd_BankerListChange));
	//SendDataToAllTableRole(&msg);
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void NiuNiuManager::OnBankerUserChange()
{
	MG_Cmd_BankerUserChange msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_BankerUserChange), sizeof(MG_Cmd_BankerUserChange));
	if (m_TableBankerUserID == 0)
	{
		msg.BankerUserInfo.dwUserID = 0;
		msg.BankerUserInfo.GlobelSum = 0;
		msg.BankerUserInfo.NickName[0] = 0;
		msg.BankerUserInfo.dwFaceID = 0;
		msg.GameSum = 0;
	}
	else
	{
		Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
		if (!pBankerRole)
		{
			msg.BankerUserInfo.dwUserID = 0;
			msg.BankerUserInfo.GlobelSum = 0;
			msg.BankerUserInfo.NickName[0] = 0;
			msg.BankerUserInfo.dwFaceID = 0;
			msg.GameSum = 0;
		}
		else
		{
			msg.BankerUserInfo.dwUserID = m_TableBankerUserID;
			msg.BankerUserInfo.GlobelSum = pBankerRole->GetGlobelSum();
			TCHARCopy(msg.BankerUserInfo.NickName, CountArray(msg.BankerUserInfo.NickName), pBankerRole->GetNickName(), _tcslen(pBankerRole->GetNickName()));
			msg.BankerUserInfo.dwFaceID = pBankerRole->GetFaceID();
			msg.GameSum = m_TableBankerUseGameSum;
		}
	}
	//SendDataToAllTableRole(&msg);
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void NiuNiuManager::UpdateTableIsCanBegin(DWORD dwTimer)
{
	if (m_TableStates == NNTS_End)
	{
		//桌子当前状态为 开牌阶段
		if (m_TableStatesLog != 0 && dwTimer - m_TableStatesLog < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.EndWriteSec * 1000)
			return;
		//进入开牌阶段已经满30秒了
		if (m_TableUserMap.empty())
		{
			SetTableStates(NNTS_WriteBegin);//桌子进入等待开始阶段 因为没人 无须继续处理
			OnClearTableInfo();
		}
		else
		{
			OnTableJoinBegin();
		}
		return;
	}
	else if (m_TableStates == NNTS_WriteBegin)
	{
		if (m_TableUserMap.empty())
			return;
		OnTableJoinBegin();
		return;
	}
	else
	{
		return;
	}
}
void NiuNiuManager::UpdateTableIsCanEnd(DWORD dwTimer)
{
	if (m_TableStates == NNTS_Begin)
	{
		if (dwTimer - m_TableStatesLog < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.BeginWriteSec * 1000)
			return;
		//进入下注阶段30秒满了 可以进入开牌阶段了 
		OnTableJoinEnd();
		SetTableStates(NNTS_End);//桌子进入开牌阶段
		return;
	}
	else
	{
		return;//其他阶段不做处理
	}
}
void NiuNiuManager::UpdateTableInfo(DWORD dwTimer)
{
	if (m_TableStates != NNTS_Begin)
		return;
	static DWORD NiuNiuUpdateTableInfo = 0;
	if (NiuNiuUpdateTableInfo == 0 || dwTimer - NiuNiuUpdateTableInfo >= g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.UpdateSec * 1000)
	{
		NiuNiuUpdateTableInfo = dwTimer;
		OnUpdateTableInfo();
	}
}
tagNiuNiuRoleInfo* NiuNiuManager::GetRoleInfo(DWORD dwUserID)
{
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
		return NULL;
	else
		return &Iter->second;
}
void NiuNiuManager::SetNextBankerUserInfo()
{
	//在游戏开始下注的时候 重新设置下庄家的数据
	bool BankerIsChange = false;
	if (m_IsNeedClearBankerInfo)
	{
		tagNiuNiuRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
		if (pUserInfo)
		{
			pUserInfo->IsBankerUser = false;
		}

		m_TableBankerUserID = 0;
		m_TableBankerUseGameSum = 0;
		m_IsNeedClearBankerInfo = false;
		BankerIsChange = true;
	}
	if (m_TableBankerUserID != 0)
	{
		Role * pBankerRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
		if (pBankerRole)
		{
			++m_TableBankerUseGameSum;
			if (m_TableBankerUseGameSum >= g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.BankerGameSum)
			{
				tagNiuNiuRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
				if (pUserInfo)
				{
					pUserInfo->IsBankerUser = false;
				}

				m_TableBankerUserID = 0;
				m_TableBankerUseGameSum = 0;
				BankerIsChange = true;
			}
			else if (pBankerRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum)
			{
				tagNiuNiuRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
				if (pUserInfo)
				{
					pUserInfo->IsBankerUser = false;
				}

				m_TableBankerUserID = 0;
				m_TableBankerUseGameSum = 0;
				BankerIsChange = true;
			}
		}
		else
		{
			tagNiuNiuRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
			if (pUserInfo)
			{
				pUserInfo->IsBankerUser = false;
			}

			m_TableBankerUserID = 0;
			m_TableBankerUseGameSum = 0;
			BankerIsChange = true;
		}
	}
	if (m_TableBankerUserID == 0)
	{
		if (!m_TableWriteBankerVec.empty())
		{
			vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
			for (; Iter != m_TableWriteBankerVec.end();)
			{
				Role* pDestRole = g_FishServer.GetRoleManager().QueryRole(*Iter);
				if (!pDestRole || pDestRole->GetGlobelSum() <  g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum)
				{
					tagNiuNiuRoleInfo* pUserInfo = GetRoleInfo(*Iter);
					if (pUserInfo)
					{
						pUserInfo->IsInWriteBanakerVec = false;
					}

					Iter = m_TableWriteBankerVec.erase(Iter);
				}
				else
				{
					tagNiuNiuRoleInfo* pUserInfo = GetRoleInfo(*Iter);
					if (pUserInfo)
					{
						//因为玩家成为庄家 自动离开VIP席位
						if (pUserInfo->IsInVipSeat)
							OnRoleLeaveVipSeat(*Iter);

						pUserInfo->IsInWriteBanakerVec = false;
						pUserInfo->IsBankerUser = true;
					}

					//因为是从列表里拿取的
					OnWriteBankerChange();

					m_TableBankerUserID = *Iter;
					m_TableBankerUseGameSum = 0;
					m_TableWriteBankerVec.erase(Iter);
					BankerIsChange = true;
					break;
				}
			}
		}
	}
	if (BankerIsChange)
	{
		//开始下注阶段前  庄家发生变化了
		//if (m_TableBankerUserID != 0)
		//{
		//	Role* pDestRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
		//	if (pDestRole)
		//	{
		//		pDestRole->OnChangeRoleGlobelSum(pDestRole->GetGlobelSum() + g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum);//添加玩家的金币
		//	}
		//}
		OnBankerUserChange();
		BankerIsChange = false;
	}
}
void NiuNiuManager::OnTableJoinBegin()
{
	//当桌子进入下注阶段的时候
	//1.判断庄家是否需要进行变化
	SetNextBankerUserInfo();
	//2.清空桌子上的数据
	OnClearTableInfo();
	//3.通知玩家 进入新的阶段了
	SetTableStates(NNTS_Begin);//桌子进入下注阶段
	MG_Cmd_TableJoinBegin msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_TableJoinBegin), sizeof(MG_Cmd_TableJoinBegin));
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
	m_oxrobotmanager.StartGame();
}
void NiuNiuManager::OnUpdateTableInfo()
{
	//更新桌子上的数据
	MG_Cmd_TableUpdate msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_TableUpdate), sizeof(MG_Cmd_TableUpdate));
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum; ++i)
		msg.TableBetAreaGlobel[i] = m_TableBetAreaGlobel[i];
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
}
INT64 NiuNiuManager::GetBrandUserGlobelChange()
{
	INT64 GlobelChange = 0;
	for (BYTE i = 1; i < MAX_NIUNIU_ClientSum + 1; ++i)
	{
		if (m_TableBrandResult[i] > m_TableBrandResult[0])
		{
			BYTE Rate = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetRateByBrandValue(m_TableBrandResult[i]);
			GlobelChange -= (m_TableBetAreaGlobel[i - 1] * Rate);
		}
		else
		{
			BYTE Rate = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetRateByBrandValue(m_TableBrandResult[0]);
			GlobelChange += (m_TableBetAreaGlobel[i - 1] * Rate);
		}
	}
	return GlobelChange;
}
INT64 NiuNiuManager::GetSystemGlobelChange()
{
	bool IsSystemBranker = false;
	if (m_TableBankerUserID == 0)//庄家为系统的话 或者 庄家为机器人的时候
	{
		IsSystemBranker = true;
	}
	else
	{
		HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(m_TableBankerUserID);
		if (Iter != m_TableUserMap.end() && Iter->second.IsRobot)
		{
			IsSystemBranker = true;
		}
	}

	INT64 GlobelChange = 0;
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		for (BYTE i = 0; i < MAX_NIUNIU_ClientSum; ++i)
		{
			if (Iter->second.BetGlobel[i] == 0)
				continue;
			if (m_TableBrandResult[i+1] > m_TableBrandResult[0])
			{
				BYTE Rate = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetRateByBrandValue(m_TableBrandResult[i + 1]);

				if (Iter->second.IsRobot)
				{
					if (!IsSystemBranker)
						GlobelChange += (Iter->second.BetGlobel[i] * Rate);
				}
				else
				{
					if (IsSystemBranker)
						GlobelChange -= (Iter->second.BetGlobel[i] * Rate);
				}
			}
			else
			{
				BYTE Rate = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetRateByBrandValue(m_TableBrandResult[0]);

				if (Iter->second.IsRobot)
				{
					if (!IsSystemBranker)
						GlobelChange -= (Iter->second.BetGlobel[i] * Rate);
				}
				else
				{
					if (IsSystemBranker)
						GlobelChange += (Iter->second.BetGlobel[i] * Rate);
				}
			}
		}
	}
	return GlobelChange;
}
INT64 NiuNiuManager::ChangeBrandValueBySystem(OnceBrandList& pBrand)
{
	//1.重新随机系统的牌型 直到系统不亏钱
	BYTE Index = 0;
	INT64 SystemGlobel = 0;
	do
	{
		if (m_TableBrandResult[0] != 0)
		{
			for (BYTE j = 0; j < MAX_NIUNIU_BrandSum; ++j)
			{
				pBrand.AddBrand(m_TableBrandArray[0][j]);
				m_TableBrandArray[0][j] = 0;
			}
			m_TableBrandResult[0] = 0;
		}
		for (BYTE j = 0; j < MAX_NIUNIU_BrandSum; ++j)
		{
			m_TableBrandArray[0][j] = pBrand.GetNewBrand();//获取一张新牌
		}
		m_TableBrandResult[0] = HandleBrandValue(m_TableBrandArray[0]);
		SystemGlobel = GetSystemGlobelChange();
		++Index;
	} while (SystemGlobel < 0 && Index < 200);//必须在200次内
	return SystemGlobel;
}
void NiuNiuManager::OnTableJoinEnd()
{
	//当比赛开牌的时候 
	//进行结算的时候 
	UINT64 TotalAllAddGlobel = 0;
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum; ++i)
		TotalAllAddGlobel += m_TableBetAreaGlobel[i];

	OnceBrandList pBrand;//生成一副新的牌
	//处理结果 
	//1.我们先试图生成 全部的套牌 
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum + 1; ++i)
	{
		for (BYTE j = 0; j < MAX_NIUNIU_BrandSum; ++j)
		{
			m_TableBrandArray[i][j] = pBrand.GetNewBrand();//获取一张新牌
		}
		//5张牌放置完毕后 计算牌的值
		m_TableBrandResult[i] = HandleBrandValue(m_TableBrandArray[i]);
	}
	//控制优先
	INT64 SystemGlobel = 0;
	INT64 BrandUserGlobel = 0;
	if (g_FishServer.GetOxControl().NeedControl())
	{		
		//UINT64 nPlayerJettons[MAX_NIUNIU_ClientSum] = {0};
		g_FishServer.GetOxControl().GetSuitResult(m_TableBrandArray, m_TableBrandResult, m_TableBetAreaGlobel);

		g_FishServer.GetOxControl().CompleteControl();
		for (BYTE i = 0; i < MAX_NIUNIU_ClientSum + 1; ++i)
		{
			m_TableBrandResult[i] = HandleBrandValue(m_TableBrandArray[i]);
		}
		BrandUserGlobel = GetBrandUserGlobelChange();//根据结果计算庄家金币的变化
		SystemGlobel = GetSystemGlobelChange();//计算系统金币变化
	}
	else
	{
		//2.判断 庄家是否会破产 破产的话 我们改变 庄家的牌型  直到不破产为止
		BrandUserGlobel = GetBrandUserGlobelChange();//根据结果计算庄家金币的变化
		SystemGlobel = GetSystemGlobelChange();//计算系统金币变化

		if (m_SystemGlobelSum < 0 || (m_SystemGlobelSum + SystemGlobel) < 0)
		{
			//需要进行作弊
			SystemGlobel = ChangeBrandValueBySystem(pBrand);
			BrandUserGlobel = GetBrandUserGlobelChange();//根据结果计算庄家金币的变化
		}
		//if (m_TableBankerUserID == 0)//庄家为系统的话 或者 庄家为机器人的时候
		//{
		//	if (m_SystemGlobelSum < 0 || (m_SystemGlobelSum + SystemGlobel) < 0)
		//	{
		//		SystemGlobel = ChangeBrandValueBySystem(pBrand);
		//		BrandUserGlobel = GetBrandUserGlobelChange();//根据结果计算庄家金币的变化
		//	}
		//}
		//else
		//{
		//	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(m_TableBankerUserID);
		//	if (Iter != m_TableUserMap.end() && Iter->second.IsRobot)
		//	{
		//		if (m_SystemGlobelSum < 0 || (m_SystemGlobelSum + SystemGlobel) < 0)
		//		{
		//			SystemGlobel = ChangeBrandValueBySystem(pBrand);
		//			BrandUserGlobel = GetBrandUserGlobelChange();//根据结果计算庄家金币的变化
		//		}
		//	}
		//}
	}
	//3.干涉完毕后 我们进行处理
	MG_Cmd_TableJoinEnd msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_TableJoinEnd), sizeof(MG_Cmd_TableJoinEnd));
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum; ++i)
		msg.TableBetAreaGlobel[i] = m_TableBetAreaGlobel[i];
	for (BYTE i = 0; i < MAX_NIUNIU_ClientSum + 1; ++i)
	{
		msg.TableBrandResult[i] = m_TableBrandResult[i];
		for (BYTE j = 0; j < MAX_NIUNIU_BrandSum; ++j)
			msg.TableBrandArray[i][j] = m_TableBrandArray[i][j];
	}

	m_NiuNiuResultLog.SetResult(m_TableBrandResult);//根据当局的结果设置数据

	msg.TableGameSum = m_NiuNiuResultLog.LogSum;
	msg.TableResultLog = m_NiuNiuResultLog.ResultLog;

	//将当前玩家的金币发送到客户端去
	msg.BankerUserGlobelInfo.dwUserID = m_TableBankerUserID;
	if (m_TableBankerUserID != 0)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
		if (pRole)
		{
			//设置庄家的金币  应该是之后的金币数量
			if (pRole->GetGlobelSum() + BrandUserGlobel < 0)
				pRole->OnChangeRoleGlobelSum(0);
			else
				pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() + static_cast<DWORD>(BrandUserGlobel));

			msg.BankerUserGlobelInfo.dwGlobelSum = pRole->GetGlobelSum();
		}
		else
		{
			msg.BankerUserGlobelInfo.dwUserID = 0;
			msg.BankerUserGlobelInfo.dwGlobelSum = 0;
		}
	}
	if (m_TableWriteBankerVec.empty())
	{
		for (BYTE i = 0; i < MAX_SHOWBRAND_Sum; ++i)
		{
			msg.BankerListGlobelInfo[i].dwUserID = 0;
			msg.BankerListGlobelInfo[i].dwGlobelSum = 0;
		}
	}
	else
	{
		vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
		for (BYTE i = 0; Iter != m_TableWriteBankerVec.end() && i < MAX_SHOWBRAND_Sum;)
		{
			Role* pDestRole = g_FishServer.GetRoleManager().QueryRole(*Iter);
			if (!pDestRole)
			{
				//++i;
				++Iter;
				continue;
			}
			msg.BankerListGlobelInfo[i].dwUserID = *Iter;
			msg.BankerListGlobelInfo[i].dwGlobelSum = pDestRole->GetGlobelSum();
			++i;
			++Iter;
		}
	}
	for (BYTE i = 0; i < MAX_VIPSEAT_Sum; ++i)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(m_VipSeatVec[i].dwUserID);
		if (pRole)
		{
			msg.VipGlobelInfo[i].dwUserID = pRole->GetRoleID();
			msg.VipGlobelInfo[i].dwGlobelSum = pRole->GetGlobelSum();
		}
		else
		{
			msg.VipGlobelInfo[i].dwUserID = 0;
			msg.VipGlobelInfo[i].dwGlobelSum = 0;
		}
	}


	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		if (m_TableBankerUserID != 0 && msg.dwUserID == m_TableBankerUserID)
		{
			/*if (pRole->GetGlobelSum() + ResultGlobel < 0)
				pRole->OnChangeRoleGlobelSum(0);
			else
				pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() + static_cast<DWORD>(ResultGlobel));*/

			msg.AddGlobelSum = BrandUserGlobel;

			//OnChangeSystemGlobel(msg.dwUserID, msg.AddGlobelSum);
		}
		else
		{
			//处理当前玩家的数据
			int AddGlobel = 0;
			for (BYTE i = 0; i < MAX_NIUNIU_ClientSum; ++i)
			{
				if (Iter->second.BetGlobel[i] == 0)
					continue;
				//计算单个玩家的金币变化
				//玩家下过注
				SHORT Rate = GetBrandRateValue(i);
				AddGlobel += Iter->second.BetGlobel[i] * Rate;//计算玩家金币改变

				Iter->second.BetGlobel[i] = 0;//清空玩家下注的金币数量
			}
			if (pRole->GetGlobelSum() + AddGlobel < 0)
				pRole->OnChangeRoleGlobelSum(0);
			else
				pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() + AddGlobel);
			msg.AddGlobelSum = AddGlobel;//玩家输赢金币的数量

			//OnChangeSystemGlobel(msg.dwUserID, msg.AddGlobelSum);

			if (AddGlobel == 0 && Iter->second.IsInVipSeat && m_VipSeatVec[Iter->second.VipSeat].dwUserID == Iter->first)
			{
				//当前玩家未下注
				++m_VipSeatVec[Iter->second.VipSeat].NonBetGlobelSum;
				if (m_VipSeatVec[Iter->second.VipSeat].NonBetGlobelSum >= g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.VipNonBetSum)
				{
					//判断玩家是否为特殊玩家 我们进行处理
					if ((pRole->GetRoleInfo().ParticularStates & ParticularState_NiuNiuVip) != 0)
					{
						m_VipSeatVec[Iter->second.VipSeat].NonBetGlobelSum = 0;
					}
					else
						OnRoleLeaveVipSeat(Iter->first);//玩家离开VIP席位
				}
			}
		}
		pRole->SendDataToGameServer(&msg);
	}

	m_SystemGlobelSum += SystemGlobel;
	if (TotalAllAddGlobel != 0) //无人下注的时候 无须进行递减
		m_SystemGlobelSum = static_cast<INT64>(m_SystemGlobelSum * g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetChangeGlobelRateValue());

	if (m_TableBankerUserID != 0)
	{
		if (BrandUserGlobel >= 0)
			g_DBLogManager.LogToDB(m_TableBankerUserID, LT_NiuNiu, 1, static_cast<DWORD>(BrandUserGlobel), TEXT("玩家坐庄获得金币"), SendLogDB);
		else
			g_DBLogManager.LogToDB(m_TableBankerUserID, LT_NiuNiu, 0, static_cast<DWORD>(BrandUserGlobel*-1), TEXT("玩家坐庄失去金币"), SendLogDB);
	}
	//存储到数据库里面去
	g_DBLogManager.LogNiuNiuTableInfoToDB(m_TableBrandArray, SystemGlobel, m_SystemGlobelSum, m_TableUserMap.size(), SendLogDB);

	//判断当前在排队列表里的玩家 
	MG_Cmd_WriteBankerUserGlobelMsg msgSend;
	SetMsgInfo(msgSend, GetMsgType(Main_NiuNiu, MG_WriteBankerUserGlobelMsg), sizeof(MG_Cmd_WriteBankerUserGlobelMsg));
	vector<DWORD>::iterator IterVec = m_TableWriteBankerVec.begin();
	for (; IterVec != m_TableWriteBankerVec.end(); ++IterVec)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(*IterVec);
		if (!pRole)
			continue;
		if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.JoinBankerGlobelSum)
		{
			msgSend.dwUserID = *IterVec;
			pRole->SendDataToGameServer(&msgSend);
		}
	}
	m_oxrobotmanager.EndGame();
}
SHORT NiuNiuManager::GetBrandRateValue(BYTE Index)
{
	if (Index >= MAX_NIUNIU_ClientSum)
		return 1;
	if (m_TableBrandResult[Index + 1] > m_TableBrandResult[0])
	{
		//比庄家大的时候
		BYTE Rate = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetRateByBrandValue(m_TableBrandResult[Index + 1]);
		//玩家赢庄家 退还本金 在算倍率
		Rate += 1;
		return static_cast<SHORT>(Rate);
	}
	else
	{
		//比庄家小的时候
		BYTE Rate = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.GetRateByBrandValue(m_TableBrandResult[0]);
		//庄家赢 本金抵消一次倍率
		Rate -= 1;
		return static_cast<SHORT>(Rate*-1);
	}
}
WORD NiuNiuManager::HandleBrandValue(BYTE* BrandValue)
{
	if (MAX_NIUNIU_BrandSum != 5)
		return 0;
	//得到当前牌的值 
	// 牌型  最大牌点数 最大牌花色 
	BrandNiuNiuLevel bLevel = BrandNiuNiuLevel::BNNL_Non;//默认无牛
	BYTE bv = 0;//最大的牌的数字
	BYTE HandleBrandValue[MAX_NIUNIU_BrandSum] = { 0 };
	BYTE BrandCardValue[MAX_NIUNIU_BrandSum] = { 0 };//牌的面值
	for (BYTE i = 0; i < MAX_NIUNIU_BrandSum; ++i)
	{
		HandleBrandValue[i] = GetBrandHandleValue(BrandValue[i]);
		BrandCardValue[i] = static_cast<BYTE>(((BrandValue[i] - 1) / 4) + 1);
		if (bv == 0 || bv < BrandValue[i])
			bv = BrandValue[i];
	}
	//
	//1.五小牛
	if (BrandCardValue[0] + BrandCardValue[1] + BrandCardValue[2] + BrandCardValue[3] + BrandCardValue[4] < 10)
	{
		//&& BrandCardValue[0] < 5 && BrandCardValue[1] < 5 && BrandCardValue[2] < 5 && BrandCardValue[3] < 5 && BrandCardValue[4] < 5
		bLevel = BrandNiuNiuLevel::BNNL_WXN;//五小牛
		return ((bLevel << 8) + bv);
	}
	//2.炸弹
	if (
		IsOnceBrandValue(BrandCardValue[0], BrandCardValue[1], BrandCardValue[2], BrandCardValue[3]) ||
		IsOnceBrandValue(BrandCardValue[0], BrandCardValue[1], BrandCardValue[2], BrandCardValue[4]) ||
		IsOnceBrandValue(BrandCardValue[0], BrandCardValue[1], BrandCardValue[4], BrandCardValue[3]) ||
		IsOnceBrandValue(BrandCardValue[0], BrandCardValue[4], BrandCardValue[2], BrandCardValue[3]) ||
		IsOnceBrandValue(BrandCardValue[4], BrandCardValue[1], BrandCardValue[2], BrandCardValue[3])
		)
	{
		bLevel = BrandNiuNiuLevel::BNNL_ZD;//炸弹
		return ((bLevel << 8) + bv);
	}
	//处理牌的牛牛 BUG  应该全部进行判断 取最大的值
	//先将需要交换的数据记录下来
	HashMap<BYTE, BYTE> m_ChangeMap;
	BYTE OtherValue = 0;
	if ((HandleBrandValue[0] + HandleBrandValue[1] + HandleBrandValue[2]) % 10 == 0)
	{
		OtherValue = ((HandleBrandValue[3] + HandleBrandValue[4]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
	}
	if ((HandleBrandValue[0] + HandleBrandValue[1] + HandleBrandValue[3]) % 10 == 0 && ((HandleBrandValue[2] + HandleBrandValue[4]) % 10) >= OtherValue)
	{

		OtherValue = ((HandleBrandValue[2] + HandleBrandValue[4]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
		//交换牌
		m_ChangeMap.clear();
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(2, 3));
	}
	if ((HandleBrandValue[0] + HandleBrandValue[1] + HandleBrandValue[4]) % 10 == 0 && ((HandleBrandValue[2] + HandleBrandValue[3]) % 10) >= OtherValue)
	{
		OtherValue = ((HandleBrandValue[3] + HandleBrandValue[2]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
		m_ChangeMap.clear();
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(2, 4));
	}
	if ((HandleBrandValue[0] + HandleBrandValue[2] + HandleBrandValue[3]) % 10 == 0 && ((HandleBrandValue[1] + HandleBrandValue[4]) % 10) >= OtherValue)
	{
		OtherValue = ((HandleBrandValue[1] + HandleBrandValue[4]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
		m_ChangeMap.clear();
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(3, 1));
	}
	if ((HandleBrandValue[0] + HandleBrandValue[2] + HandleBrandValue[4]) % 10 == 0 && ((HandleBrandValue[3] + HandleBrandValue[1]) % 10) >= OtherValue)
	{
		OtherValue = ((HandleBrandValue[3] + HandleBrandValue[1]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
		m_ChangeMap.clear();
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(1, 4));
	}
	if ((HandleBrandValue[0] + HandleBrandValue[3] + HandleBrandValue[4]) % 10 == 0 && ((HandleBrandValue[2] + HandleBrandValue[1]) % 10) >= OtherValue)
	{
		OtherValue = ((HandleBrandValue[1] + HandleBrandValue[2]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
		m_ChangeMap.clear();
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(1, 3));
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(2, 4));
	}
	if ((HandleBrandValue[1] + HandleBrandValue[2] + HandleBrandValue[3]) % 10 == 0 && ((HandleBrandValue[0] + HandleBrandValue[4]) % 10) >= OtherValue)
	{
		OtherValue = ((HandleBrandValue[0] + HandleBrandValue[4]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
		m_ChangeMap.clear();
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(0, 3));
	}
	if ((HandleBrandValue[1] + HandleBrandValue[2] + HandleBrandValue[4]) % 10 == 0 && ((HandleBrandValue[0] + HandleBrandValue[3]) % 10) >= OtherValue)
	{
		OtherValue = ((HandleBrandValue[3] + HandleBrandValue[0]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
		m_ChangeMap.clear();
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(0, 4));
	}
	if ((HandleBrandValue[1] + HandleBrandValue[3] + HandleBrandValue[4]) % 10 == 0 && ((HandleBrandValue[2] + HandleBrandValue[0]) % 10) >= OtherValue)
	{
		OtherValue = ((HandleBrandValue[0] + HandleBrandValue[2]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
		m_ChangeMap.clear();
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(0, 3));
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(2, 4));
	}
	if ((HandleBrandValue[2] + HandleBrandValue[3] + HandleBrandValue[4]) % 10 == 0 && ((HandleBrandValue[1] + HandleBrandValue[0]) % 10) >= OtherValue)
	{
		OtherValue = ((HandleBrandValue[0] + HandleBrandValue[1]) % 10);//0-9
		if (OtherValue == 0)
			OtherValue = 10;
		m_ChangeMap.clear();
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(0, 3));
		m_ChangeMap.insert(HashMap<BYTE, BYTE>::value_type(1, 4));
	}
	if (OtherValue == 0)
	{
		bLevel = BrandNiuNiuLevel::BNNL_Non;//无牛
		return ((bLevel << 8) + bv);
	}

	//进行切换牌的操作
	if (!m_ChangeMap.empty())
	{
		HashMap<BYTE, BYTE>::iterator Iter = m_ChangeMap.begin();
		for (; Iter != m_ChangeMap.end(); ++Iter)
		{
			ChangeValue(BrandValue[Iter->first], BrandValue[Iter->second]);
		}
		m_ChangeMap.clear();
	}


	switch (OtherValue)
	{
	case 10:
		{
			//有可能为五花牛
			if (BrandCardValue[0] > 10 && BrandCardValue[1] > 10 && BrandCardValue[2] > 10 && BrandCardValue[3] > 10 && BrandCardValue[4] > 10 && BrandCardValue[5] > 10)
			{
				bLevel = BrandNiuNiuLevel::BNNL_WHN;
				return ((bLevel << 8) + bv);
			}
			else
			{
				bLevel = BrandNiuNiuLevel::BNNL_NN;
				return ((bLevel << 8) + bv);
			}
		}
		break;
	case 1:
		bLevel = BrandNiuNiuLevel::BNNL_1;
		return ((bLevel << 8) + bv);
		break;
	case 2:
		bLevel = BrandNiuNiuLevel::BNNL_2;
		return ((bLevel << 8) + bv);
		break;
	case 3:
		bLevel = BrandNiuNiuLevel::BNNL_3;
		return ((bLevel << 8) + bv);
		break;
	case 4:
		bLevel = BrandNiuNiuLevel::BNNL_4;
		return ((bLevel << 8) + bv);
		break;
	case 5:
		bLevel = BrandNiuNiuLevel::BNNL_5;
		return ((bLevel << 8) + bv);
		break;
	case 6:
		bLevel = BrandNiuNiuLevel::BNNL_6;
		return ((bLevel << 8) + bv);
		break;
	case 7:
		bLevel = BrandNiuNiuLevel::BNNL_7;
		return ((bLevel << 8) + bv);
		break;
	case 8:
		bLevel = BrandNiuNiuLevel::BNNL_8;
		return ((bLevel << 8) + bv);
		break;
	case 9:
		bLevel = BrandNiuNiuLevel::BNNL_9;
		return ((bLevel << 8) + bv);
		break;
	}
	return 0;
}
BYTE NiuNiuManager::GetBrandHandleValue(BYTE Value)
{
	// 1-52
	BYTE Type = (Value - 1) % 4;//0-3  
	BYTE BrandValue = ((Value - 1) / 4) + 1;//0-12 (A-10-K)
	if (BrandValue == 13 || BrandValue == 11 || BrandValue == 12)
		return 10;
	else
		return BrandValue;
}
void NiuNiuManager::ChangeValue(BYTE& A, BYTE& B)
{
	BYTE C = 0;
	C = A;
	A = B;
	B = C;
}
//void NiuNiuManager::OnChangeSystemGlobel(DWORD dwUserID, INT64 ChangeGlobel)
//{
//	if (ChangeGlobel == 0)
//		return;
//	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
//	if (Iter == m_TableUserMap.end())
//	{
//		ASSERT(false);
//		return;
//	}
//	if (Iter->second.IsRobot)
//	{
//		//机器人获得金币
//		m_SystemGlobelSum += ChangeGlobel;
//		//g_FishServer.ShowInfoToWin("机器人 金币改变 %d", ChangeGlobel);
//	}
//	else
//	{
//		m_SystemGlobelSum -= ChangeGlobel;
//
//		//g_FishServer.ShowInfoToWin("普通人 金币改变 %d", ChangeGlobel);
//	}
//}
void NiuNiuManager::OnRoleJoinVipSeat(DWORD dwUserID, BYTE VipSeatIndex)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_RoleJoinVipSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_RoleJoinVipSeat), sizeof(MG_Cmd_RoleJoinVipSeat));
	msg.Result = false;
	msg.DestUserInfo.dwUserID = dwUserID;
	msg.DestUserInfo.dwFaceID = pRole->GetFaceID();
	msg.DestUserInfo.GlobelSum = pRole->GetGlobelSum();
	TCHARCopy(msg.DestUserInfo.NickName, CountArray(msg.DestUserInfo.NickName), pRole->GetNickName(), _tcslen(pRole->GetNickName()));
	msg.dwUserID = dwUserID;
	msg.VipSeatIndex = VipSeatIndex;
	if (Iter->second.IsBankerUser)//庄家不可以做VIP席位
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (Iter->second.IsInVipSeat)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (pRole->GetVipLevel() == 0)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (VipSeatIndex >= MAX_VIPSEAT_Sum)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (m_VipSeatVec[VipSeatIndex].dwUserID != 0)
	{
		Role* pDestRole = g_FishServer.GetRoleManager().QueryRole(m_VipSeatVec[VipSeatIndex].dwUserID);
		if (pDestRole)
		{
			if (pDestRole->GetVipLevel() >= pRole->GetVipLevel())
			{
				pRole->SendDataToGameServer(&msg);
				return;
			}
			else
			{
				//告诉目标玩家 被踢了
				MG_Cmd_RoleBeLeaveVipSeat msgLeave;
				SetMsgInfo(msgLeave, GetMsgType(Main_NiuNiu, MG_RoleBeLeaveVipSeat), sizeof(MG_Cmd_RoleBeLeaveVipSeat));
				msgLeave.dwUserID = m_VipSeatVec[VipSeatIndex].dwUserID;
				msgLeave.DestRoleInfo.dwUserID = dwUserID;
				msgLeave.DestRoleInfo.dwFaceID = pRole->GetFaceID();
				msgLeave.DestRoleInfo.GlobelSum = pRole->GetGlobelSum();
				TCHARCopy(msgLeave.DestRoleInfo.NickName, CountArray(msgLeave.DestRoleInfo.NickName), pRole->GetNickName(), _tcslen(pRole->GetNickName()));
				pDestRole->SendDataToGameServer(&msgLeave);
			}
		}
	}
	//玩家可以做上Vip席位
	m_VipSeatVec[VipSeatIndex].dwUserID = dwUserID;
	m_VipSeatVec[VipSeatIndex].NonBetGlobelSum = 0;
	Iter->second.IsInVipSeat = true;
	Iter->second.VipSeat = VipSeatIndex;

	msg.Result = true;
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator IterSend = m_TableUserMap.begin();//Vip席位的改变发送给全部玩家
	for (; IterSend != m_TableUserMap.end(); ++IterSend)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterSend->first);
		if (!pRole)
			continue;
		msg.dwUserID = IterSend->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void NiuNiuManager::OnRoleLeaveVipSeat(DWORD dwUserID)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	if (Iter->second.IsBankerUser)
	{
		ASSERT(false);
		return;
	}
	if (!Iter->second.IsInVipSeat)
	{
		ASSERT(false);
		return;
	}
	if (m_VipSeatVec[Iter->second.VipSeat].dwUserID != dwUserID)
	{
		ASSERT(false);
		return;
	}

	m_VipSeatVec[Iter->second.VipSeat].dwUserID = 0;
	m_VipSeatVec[Iter->second.VipSeat].NonBetGlobelSum = 0;

	Iter->second.IsInVipSeat = false;
	Iter->second.VipSeat = 0;

	MG_Cmd_RoleLeaveVipSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_RoleLeaveVipSeat), sizeof(MG_Cmd_RoleLeaveVipSeat));
	msg.dwDestUserID = dwUserID;
	msg.VipSeatIndex = Iter->second.VipSeat;
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator IterSend = m_TableUserMap.begin();
	for (; IterSend != m_TableUserMap.end(); ++IterSend)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterSend->first);
		if (!pRole)
			continue;
		msg.dwUserID = IterSend->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void NiuNiuManager::OnRoleGetNormalInfoByPage(DWORD dwUserID, DWORD Page)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator IterRole = m_TableUserMap.find(dwUserID);
	if (IterRole == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}

	DWORD BeginIndex = Page * MAX_NORMAL_PAGESUM;
	DWORD EndIndex = (Page + 1)* MAX_NORMAL_PAGESUM - 1;


	MG_Cmd_GetNormalSeatInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_NiuNiu, MG_GetNormalSeatInfo), sizeof(MG_Cmd_GetNormalSeatInfo));
	msg.dwUserID = dwUserID;
	msg.Page = Page;
	msg.TotalRoleSum = m_TableUserMap.size();

	for (BYTE i = 0; i < MAX_VIPSEAT_Sum; ++i)
	{
		if (m_VipSeatVec[i].dwUserID != 0)
			--msg.TotalRoleSum;
	}
	if (m_TableBankerUserID != 0)
		--msg.TotalRoleSum;

	for (BYTE i = 0; i < MAX_NORMAL_PAGESUM; ++i)
	{
		msg.Array[i].dwFaceID = 0;
		msg.Array[i].dwUserID = 0;
		msg.Array[i].GlobelSum = 0;
		msg.Array[i].NickName[0] = 0;
	}
	HashMap<DWORD, tagNiuNiuRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (DWORD Index = 0; Iter != m_TableUserMap.end(); ++Iter)
	{
		if (Iter->second.IsInVipSeat || Iter->second.IsBankerUser)
			continue;
		Role* pDestRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pDestRole)
			continue;
		if (Index >= BeginIndex)
		{
			//将玩家记录到数据里面去
			msg.Array[Index - BeginIndex].dwUserID = pDestRole->GetRoleID();
			msg.Array[Index - BeginIndex].dwFaceID = pDestRole->GetFaceID();
			msg.Array[Index - BeginIndex].GlobelSum = pDestRole->GetGlobelSum();
			TCHARCopy(msg.Array[Index - BeginIndex].NickName, CountArray(msg.Array[Index - BeginIndex].NickName), pDestRole->GetNickName(), _tcslen(pDestRole->GetNickName()));

			if (Index == EndIndex)
			{
				pRole->SendDataToGameServer(&msg);
				return;
			}
		}
		++Index;
	}
	//目前还没有准备好 
	pRole->SendDataToGameServer(&msg);//人数不够 或者 人数为空的
	return;
}

void NiuNiuManager::QueryNames(DWORD ClientID)
{
	int nNamelength = (MAX_NICKNAME+1);
	int nSize = sizeof(LC_CMD_OxAdmin_PlayerNames)+sizeof(TCHAR)*nNamelength*(m_TableUserMap.size() - m_oxrobotmanager.RobotCount());
	LC_CMD_OxAdmin_PlayerNames *pResult = (LC_CMD_OxAdmin_PlayerNames*)malloc(nSize);
	pResult->SetCmdSize(nSize);
	pResult->SetCmdType((Main_Control << 8) | LC_OxAdminQueryName);
	pResult->ClientID = ClientID;
	int nIndex = 0;
	for (HashMap<DWORD, tagNiuNiuRoleInfo>::iterator it = m_TableUserMap.begin(); it != m_TableUserMap.end();it++)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(it->first);
		if (pRole&&!m_oxrobotmanager.IsRobot(pRole->GetRoleID()))
		{
			TCHARCopy(&pResult->names[1 + nIndex*nNamelength], nNamelength, pRole->GetNickName(), MAX_NICKNAME);
			nIndex++;
		}
	}
	g_FishServer.SendNetCmdToControl(pResult);
	free(pResult);	
}
void NiuNiuManager::QueryAllJetton(DWORD ClientID)
{
	LC_CMD_OxAdmin_AllJetton result;
	ZeroMemory(&result, sizeof(result));
	result.SetCmdSize(sizeof(result));
	result.SetCmdType((Main_Control << 8) | LC_OxAdminQueryAllJetton);
	result.ClientID = ClientID;
	result.nStock = m_SystemGlobelSum;
	CalPlayerJetton(result.nJetton);
	g_FishServer.SendNetCmdToControl(&result);

}
void NiuNiuManager::QueryPlayerJetton(TCHAR *pNickame,DWORD ClientID)
{
	for (HashMap<DWORD, tagNiuNiuRoleInfo>::iterator it = m_TableUserMap.begin(); it != m_TableUserMap.end(); it++)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(it->first);
		if (pRole)
		{
			if (_tcscmp(pNickame, pRole->GetNickName()) == 0)
			{
				HashMap<DWORD, tagNiuNiuRoleInfo>::iterator itJetton = m_TableUserMap.find(pRole->GetRoleID());
				if (itJetton != m_TableUserMap.end())
				{
					LC_CMD_OxAdmin_PlayerJetton result;
					result.SetCmdSize(sizeof(result));
					result.SetCmdType((Main_Control << 8) | LC_OxAdminQueryPlayerJetton);
					result.ClientID = ClientID;
					//TCHARCopy(result.name, MAX_NICKNAME + 1, pNickame, MAX_NICKNAME);
					memcpy(result.dwJetton, itJetton->second.BetGlobel, sizeof(result.dwJetton));				
					g_FishServer.SendNetCmdToControl(&result);
					break;
				}
			}
		}
	}

}

DWORD NiuNiuManager::BankerId()
{
	return m_TableBankerUserID;
}

WORD NiuNiuManager::ApplyListLength()
{
	return m_TableWriteBankerVec.size();
}

DWORD NiuNiuManager::BankerGold()
{
	Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
	if (pBankerRole)
	{
		return pBankerRole->GetGlobelSum();
	}
	return 0;
}
bool	NiuNiuManager::IsGameEnd()
{
	return m_TableStates == NNTS_End;
}

bool	NiuNiuManager::HaveApply(DWORD dwUserid)
{
	return find(m_TableWriteBankerVec.begin(), m_TableWriteBankerVec.end(), dwUserid) != m_TableWriteBankerVec.end();
}
void NiuNiuManager::CalPlayerJetton(UINT64 uJetton[MAX_NIUNIU_ClientSum])
{
	for (HashMap<DWORD, tagNiuNiuRoleInfo>::iterator it = m_TableUserMap.begin(); it != m_TableUserMap.end(); it++)
	{
		if (!m_oxrobotmanager.IsRobot(it->first))
		{
			for (int i = 0; i < MAX_NIUNIU_ClientSum; i++)
			{
				uJetton[i] += it->second.BetGlobel[i];
			}
		}
	}
}