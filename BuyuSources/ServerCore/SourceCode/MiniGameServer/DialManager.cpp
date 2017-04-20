#include "Stdafx.h"
#include "DialManager.h"
#include "../CommonFile/DBLogManager.h"
#include "FishServer.h"
extern void SendLogDB(NetCmd* pCmd);
DialManager::DialManager()
{

}
DialManager::~DialManager()
{
	OnDestroy();
}
void DialManager::SetTableStates(DialTableStates States)
{
	m_TableStates = States;
	m_TableStatesLog = timeGetTime();
}
bool DialManager::OnInit()
{
	SetTableStates(DTS_WriteBegin);//设置初始化的时候 桌子状态为等待开始
	m_SystemGlobelSum = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.InitGlobelSum;//系统金币
	//初始化一些数据
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		m_TableBetAreaGlobel[i] = 0;
		m_TableAreaRate[i] = 0;
	}
	for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
	{
		m_TableAreaData[i] = 0;
	}
	for (BYTE i = 0; i < MAX_DIALVIPSEAT_Sum; ++i)
	{
		m_VipSeatVec[i].dwUserID = 0;
		m_VipSeatVec[i].NonBetGlobelSum = 0;
	}
	m_TableBankerUserID = 0;
	m_TableBankerUseGameSum = 0;
	m_TableResult = 0;
	m_TableWriteBankerVec.clear();
	m_TableUserMap.clear();
	m_IsNeedClearBankerInfo = false;
	return true;
}
void DialManager::OnUpdate(DWORD dwTimer)
{
	static DWORD DialUpdateTime = 0;
	if (DialUpdateTime == 0 || dwTimer - DialUpdateTime >= 1000) //1s更新一次
	{
		DialUpdateTime = dwTimer;
		UpdateTableIsCanBegin(timeGetTime());
		UpdateTableIsCanEnd(timeGetTime());
		UpdateTableInfo(timeGetTime());
		m_animalrobotmanager.Update();
	}
}
void DialManager::OnDestroy()
{
}
void DialManager::OnRoleJoinDialTable(DWORD dwUserID)
{
	//玩家进入试图进入牛牛桌子
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_DialRoleJoinTable msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialRoleJoinTable), sizeof(MG_Cmd_DialRoleJoinTable));
	msg.dwUserID = dwUserID;
	msg.Result = false;
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter != m_TableUserMap.end())
	{
		//立刻让玩家离开
		OnRoleLeaveDialTable(dwUserID);
	}
	if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.JoinGlobelSum)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (m_TableStates == DTS_WriteBegin)
	{
		//因为有人进入了 
		SetTableStates(DTS_Begin);
		OnClearTableInfo();//清理桌子上的数据
		//CreateNewDial();//创建新的轮盘
		OnInitDial();//如果是第一次的话 初始化轮盘数据
	}
	msg.Result = true;
	tagDialRoleInfo pInfo;
	pInfo.UserID = dwUserID;
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		pInfo.BetGlobel[i] = 0;
	pInfo.IsBankerUser = false;
	pInfo.IsInWriteBanakerVec = false;
	pInfo.IsInVipSeat = false;
	pInfo.VipSeat = 0;
	pInfo.IsRobot = g_FishServer.GetRobotManager().IsRobot(dwUserID);

	m_TableUserMap.insert(HashMap<DWORD, tagDialRoleInfo>::value_type(dwUserID, pInfo));
	//继续设置数据到命令里面
	msg.TableStates = m_TableStates;
	DWORD NowTime = timeGetTime();
	if (m_TableStates == DTS_Begin)
		msg.TableStatesUpdateSec = static_cast<BYTE>((g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.BeginWriteSec * 1000 - (NowTime - m_TableStatesLog)) / 1000);
	else if (m_TableStates == DTS_End)
		msg.TableStatesUpdateSec = static_cast<BYTE>((g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.EndWriteSec * 1000 - (NowTime - m_TableStatesLog)) / 1000);
	else
		msg.TableStatesUpdateSec = 0;
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		msg.TableAreaDataRate[i] = m_TableAreaRate[i];
		msg.TableBetAreaGlobel[i] = m_TableBetAreaGlobel[i];
	}
	for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
	{
		msg.TableAreaData[i] = m_TableAreaData[i];
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
	for (BYTE i = 0; i < MAX_DIALSHOWBRAND_Sum; ++i)
	{
		msg.TableWriteBankerList[i].dwUserID = 0;
		msg.TableWriteBankerList[i].GlobelSum = 0;
		msg.TableWriteBankerList[i].NickName[0] = 0;
		msg.TableWriteBankerList[i].dwFaceID = 0;
	}
	if (!m_TableWriteBankerVec.empty())
	{
		vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
		for (BYTE i = 0; Iter != m_TableWriteBankerVec.end() && i < MAX_DIALSHOWBRAND_Sum;)
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
	for (BYTE i = 0; i < MAX_DIALVIPSEAT_Sum; ++i)
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
	msg.TableGameSum = m_DialResultLog.LogSum;
	msg.TableResultLog = m_DialResultLog.ResultLog;
	msg.TableResultIndex = m_TableResult;
	pRole->SendDataToGameServer(&msg);
}
void DialManager::OnRoleLeaveDialTable(DWORD dwUserID)
{
	//当玩家离开牛牛桌子的时候 
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
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
		OnRoleLeaveVipSeat(dwUserID);//指定玩家离开VIP席
	}
	if (Iter->second.IsBankerUser && dwUserID == m_TableBankerUserID)
	{
		if (m_TableStates == DTS_Begin)//如果为下注阶段
		{
			//庄家下注了 立刻由系统接手
			m_TableBankerUserID = 0;
			m_TableBankerUseGameSum = 0;
			OnBankerUserChange();//庄家发生变化了
		}
		else if (m_TableStates == DTS_End)
		{
			m_IsNeedClearBankerInfo = true;//在开牌阶段 清理下庄家
		}
	}
	m_TableUserMap.erase(Iter);
}
bool DialManager::IsCanAddGlobel(DWORD AddGlobel, BYTE Index)
{
	UINT64 MaxGlobel = 0;
	for (BYTE i = 0; i <MAX_DIAL_ClientSum; ++i)
	{
		UINT64 Globel = m_TableBetAreaGlobel[i];
		if (i == Index)
		{
			Globel += AddGlobel;
		}
		Globel = m_TableAreaRate[i] * Globel;
		if (Globel >= MaxGlobel)
			MaxGlobel = Globel;
	}
	if (m_TableBankerUserID != 0)
	{
		Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
		if (!pBankerRole)
			return true;
		if (MaxGlobel > pBankerRole->GetGlobelSum())
			return false;
		else
			return true;
	}
	else
		return true;
}
void DialManager::OnRoleBetGlobelByLog(DWORD dwUserID, DWORD AddGlobelArray[MAX_DIAL_ClientSum])
{
	//玩家进行继续押注
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	

	MG_Cmd_DialRoleBetGlobelByLog msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialRoleBetGlobelByLog), sizeof(MG_Cmd_DialRoleBetGlobelByLog));
	msg.dwUserID = dwUserID;

	DWORD AllGlobel = 0; // 下注的总金币数量
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		AllGlobel += AddGlobelArray[i];
		msg.betGlobel[i] = AddGlobelArray[i];
	}

	msg.AllGlobel = AllGlobel;
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (pRole->GetGlobelSum() < AllGlobel)
	{
		ASSERT(false);
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (m_TableStates != DTS_Begin)
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

	DWORD RoleBetAllGlobel = 0;
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		RoleBetAllGlobel += Iter->second.BetGlobel[i];
	}

	if (RoleBetAllGlobel + AllGlobel> g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.MaxAddGlobel)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}

	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		if (!IsCanAddGlobel(AddGlobelArray[i], i))
		{
			pRole->SendDataToGameServer(&msg);
			return;
		}
	}

	//可以下注了
	msg.Result = true;
	//1.扣除玩家金币
	pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() - AllGlobel);
	//2.添加下注区数据
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		Iter->second.BetGlobel[i] += AddGlobelArray[i];
		m_TableBetAreaGlobel[i] += AddGlobelArray[i];

		if (!Iter->second.IsRobot)
			g_DBLogManager.LogToDB(dwUserID, LogType::LT_Dial, AddGlobelArray[i], i, TEXT("玩家转盘下注"), SendLogDB);//Param 表示 下注区
	}
	pRole->SendDataToGameServer(&msg);

	if (Iter->second.IsInVipSeat)
	{
		for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		{
			if (AddGlobelArray[i] == 0)
				continue;
			MG_Cmd_DialVipSeatGlobelChange msg;
			SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialVipSeatGlobelChange), sizeof(MG_Cmd_DialVipSeatGlobelChange));
			msg.VipSeat = Iter->second.VipSeat;
			msg.GlobelSum = AddGlobelArray[i];
			msg.Index = i;
			HashMap<DWORD, tagDialRoleInfo>::iterator IterRole = m_TableUserMap.begin();
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
}
void DialManager::OnRoleBetGlobel(DWORD dwUserID, BYTE BetIndex, DWORD AddGlobel)
{
	//玩家下注
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_DialRoleBetGlobel msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialRoleBetGlobel), sizeof(MG_Cmd_DialRoleBetGlobel));
	msg.dwUserID = dwUserID;
	msg.AddGlobel = AddGlobel;
	msg.Index = BetIndex;
	msg.Result = false;
	if (BetIndex >= MAX_DIAL_ClientSum)
	{
		pRole->SendDataToGameServer(&msg);
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (pRole->GetGlobelSum() < AddGlobel)
	{
		ASSERT(false);
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (m_TableStates != DTS_Begin)
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
	for (BYTE i = 0; i<MAX_DIAL_ClientSum; ++i)
	{
		RoleBetAllGlobel += Iter->second.BetGlobel[i];
		TableBetAllGbel += m_TableBetAreaGlobel[i];
	}
	if (RoleBetAllGlobel + AddGlobel > g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.MaxAddGlobel)//当前下注区的金币太多
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//不可以操出庄家的限制 
	if (!IsCanAddGlobel(AddGlobel, BetIndex))
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}

	//可以下注了
	msg.Result = true;
	//1.扣除玩家金币
	pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() - AddGlobel);
	//2.添加下注区数据
	Iter->second.BetGlobel[BetIndex] += AddGlobel;
	m_TableBetAreaGlobel[BetIndex] += AddGlobel;
	if (!Iter->second.IsRobot)
		g_DBLogManager.LogToDB(dwUserID, LogType::LT_Dial, AddGlobel, BetIndex, TEXT("玩家转盘下注"), SendLogDB);//Param 表示 下注区
	pRole->SendDataToGameServer(&msg);


	/*TableBetAllGbel += AddGlobel;
	if (m_TableBankerUserID != 0)
	{
		Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
		if (pBankerRole && (TableBetAllGbel + 1000)* m_NowMaxRate > pBankerRole->GetGlobelSum())
		{
			IsCanBetGlobel = false;
		}
	}*/

	if (Iter->second.IsInVipSeat)
	{
		MG_Cmd_DialVipSeatGlobelChange msg;
		SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialVipSeatGlobelChange), sizeof(MG_Cmd_DialVipSeatGlobelChange));
		msg.VipSeat = Iter->second.VipSeat;
		msg.GlobelSum = AddGlobel;
		msg.Index = BetIndex;
		HashMap<DWORD, tagDialRoleInfo>::iterator IterRole = m_TableUserMap.begin();
		for (; IterRole != m_TableUserMap.end(); ++IterRole)
		{
			Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterRole->first);
			if (!pRole)
				continue;
			msg.dwUserID = IterRole->first;
			pRole->SendDataToGameServer(&msg);
		}
	}
	/*if (!IsCanBetGlobel)
	{
		MG_Cmd_DialTableStopUpdate msgTable;
		SetMsgInfo(msgTable, GetMsgType(Main_Dial, MG_DialTableStopUpdate), sizeof(MG_Cmd_DialTableStopUpdate));
		HashMap<DWORD, tagDialRoleInfo>::iterator IterRole = m_TableUserMap.begin();
		for (; IterRole != m_TableUserMap.end(); ++IterRole)
		{
			Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterRole->first);
			if (!pRole)
				continue;
			msgTable.dwUserID = IterRole->first;

			pRole->SendDataToGameServer(&msgTable);
		}
	}*/
}
void DialManager::OnRoleGetWriteBankerList(DWORD dwUserID)
{
	//玩家进入庄家排队列表
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	//发生命令到客户端去
	MG_Cmd_DialGetBankerList msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialGetBankerList), sizeof(MG_Cmd_DialGetBankerList));
	msg.dwUserID = dwUserID;
	msg.dwMySeatIndex = 0xffffffff;
	for (BYTE i = 0; i < MAX_DIALSHOWBRAND_Sum; ++i)
	{
		msg.TableWriteBankerList[i].dwUserID = 0;
		msg.TableWriteBankerList[i].GlobelSum = 0;
		msg.TableWriteBankerList[i].NickName[0] = 0;
		msg.TableWriteBankerList[i].dwFaceID = 0;
	}
	if (!m_TableWriteBankerVec.empty())
	{
		vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
		for (BYTE i = 0; Iter != m_TableWriteBankerVec.end() && i < MAX_DIALSHOWBRAND_Sum;)
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
void DialManager::OnRoleJoinWriteBankerList(DWORD dwUserID)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	//玩家试图进行上庄 
	MG_Cmd_DialRoleJoinBankerList msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialRoleJoinBankerList), sizeof(MG_Cmd_DialRoleJoinBankerList));
	msg.dwUserID = dwUserID;
	msg.Result = false;
	msg.SeatIndex = 0xffffffff;
	//1.已经在列表里面了
	if (Iter->second.IsInWriteBanakerVec)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//2.判断玩家金币是否足够
	if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.JoinBankerGlobelSum)
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
	if (msg.SeatIndex < MAX_DIALSHOWBRAND_Sum)
	{
		OnWriteBankerChange();
	}
	else
	{
		pRole->SendDataToGameServer(&msg);
	}
}
void DialManager::OnRoleGetWriteBankerFirstSeat(DWORD dwUserID)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_DialRoleGetBankerFirstSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialRoleGetBankerFirstSeat), sizeof(MG_Cmd_DialRoleGetBankerFirstSeat));
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
	if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.GetNextBankerNeedGlobel)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//可以抢庄
	msg.Result = true;
	//1.扣除金币
	pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() - g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.GetNextBankerNeedGlobel);
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
void DialManager::OnRoleLeaveWriteBankerList(DWORD dwUserID)
{
	//玩家离开上庄排队列表 
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_DialRoleLeaveBankerList msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialRoleLeaveBankerList), sizeof(MG_Cmd_DialRoleLeaveBankerList));
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
	if (Index < MAX_DIALSHOWBRAND_Sum)
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
void DialManager::OnRoleCanelBanker(DWORD dwUserID)
{
	//玩家下庄
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_DialRoleCanelBankerSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialRoleCanelBankerSeat), sizeof(MG_Cmd_DialRoleCanelBankerSeat));
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
	if (m_TableStates == DTS_Begin)
	{
		//应该让玩家的金币 暂时冻结1分钟 


		//下注阶段 玩家下庄 立刻又系统补上
		m_TableBankerUserID = 0;
		m_TableBankerUseGameSum = 0;
		OnBankerUserChange();
	}
	else if (m_TableStates == DTS_End)
	{
		//开牌阶段 玩家下庄 直接下注 设置
		//设置玩家下庄成功 
		m_IsNeedClearBankerInfo = true;
	}
	if (pRole)
		pRole->SendDataToGameServer(&msg);
}
void DialManager::OnRoleLeaveMiniGameServer(DWORD dwUserID)
{
	OnRoleLeaveDialTable(dwUserID);
}
void DialManager::OnRoleGlobelChange(DWORD dwUserID, int ChangeSum)//当发生用户充值的时候
{
	if (m_TableBankerUserID != 0 && m_TableBankerUserID == dwUserID)
	{
		Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
		if (!pBankerRole)
			return;

		MG_Cmd_DialBankerUserGlobelChange msg;
		SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialBankerUserGlobelChange), sizeof(MG_Cmd_DialBankerUserGlobelChange));
		msg.dwBankerGlobelSum = pBankerRole->GetGlobelSum();
		msg.dwBankerUserID = dwUserID;

		HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.begin();
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
		HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
		if (Iter == m_TableUserMap.end())
			return;
		if (!Iter->second.IsInVipSeat)
			return;
		//如果玩家在VIP位置上的时候 
		MG_Cmd_DialVipGlobelChange msg;
		SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialVipGlobelChange), sizeof(MG_Cmd_DialVipGlobelChange));
		msg.VipUserID = dwUserID;
		msg.VipGlobelSum = pRole->GetGlobelSum();
		HashMap<DWORD, tagDialRoleInfo>::iterator IterRole = m_TableUserMap.begin();
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
void DialManager::OnClearTableInfo()
{
	//每局开始下注之前 先清理下数据
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		m_TableBetAreaGlobel[i] = 0;
		//m_TableAreaRate[i] = 0;
	}
	/*for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
	{
		m_TableAreaData[i] = 0;
	}*/
}
void DialManager::OnWriteBankerChange()
{
	MG_Cmd_DialBankerListChange msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialBankerListChange), sizeof(MG_Cmd_DialBankerListChange));
	//SendDataToAllTableRole(&msg);
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void DialManager::OnBankerUserChange()
{
	MG_Cmd_DialBankerUserChange msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialBankerUserChange), sizeof(MG_Cmd_DialBankerUserChange));
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
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void DialManager::UpdateTableIsCanBegin(DWORD dwTimer)
{
	if (m_TableStates == DTS_End)
	{
		//桌子当前状态为 开牌阶段
		if (m_TableStatesLog != 0 && dwTimer - m_TableStatesLog < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.EndWriteSec * 1000)
			return;
		//进入开牌阶段已经满30秒了
		if (m_TableUserMap.empty())
		{
			SetTableStates(DTS_WriteBegin);//桌子进入等待开始阶段 因为没人 无须继续处理
			OnClearTableInfo();
		}
		else
		{
			OnTableJoinBegin();
		}
		return;
	}
	else if (m_TableStates == DTS_WriteBegin)
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
void DialManager::UpdateTableIsCanEnd(DWORD dwTimer)
{
	if (m_TableStates == DTS_Begin)
	{
		if (dwTimer - m_TableStatesLog < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.BeginWriteSec * 1000)
			return;
		//进入下注阶段30秒满了 可以进入开牌阶段了 
		OnTableJoinEnd();
		SetTableStates(DTS_End);//桌子进入开牌阶段
		return;
	}
	else
	{
		return;//其他阶段不做处理
	}
}
void DialManager::UpdateTableInfo(DWORD dwTimer)
{
	if (m_TableStates != DTS_Begin)
		return;
	static DWORD DialUpdateTableInfo = 0;
	if (DialUpdateTableInfo == 0 || dwTimer - DialUpdateTableInfo >= g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.UpdateSec * 1000)
	{
		DialUpdateTableInfo = dwTimer;
		OnUpdateTableInfo();
	}
}
tagDialRoleInfo* DialManager::GetRoleInfo(DWORD dwUserID)
{
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
		return NULL;
	else
		return &Iter->second;
}
void DialManager::SetNextBankerUserInfo()
{
	//在游戏开始下注的时候 重新设置下庄家的数据
	bool BankerIsChange = false;
	if (m_IsNeedClearBankerInfo)
	{
		tagDialRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
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
			if (m_TableBankerUseGameSum >= g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.BankerGameSum)
			{
				tagDialRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
				if (pUserInfo)
				{
					pUserInfo->IsBankerUser = false;
				}

				m_TableBankerUserID = 0;
				m_TableBankerUseGameSum = 0;
				BankerIsChange = true;
			}
			else if (pBankerRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.JoinBankerGlobelSum)
			{
				tagDialRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
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
			tagDialRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
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
				if (!pDestRole || pDestRole->GetGlobelSum() <  g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.JoinBankerGlobelSum)
				{
					tagDialRoleInfo* pUserInfo = GetRoleInfo(*Iter);
					if (pUserInfo)
					{
						pUserInfo->IsInWriteBanakerVec = false;
					}

					Iter = m_TableWriteBankerVec.erase(Iter);
				}
				else
				{
					tagDialRoleInfo* pUserInfo = GetRoleInfo(*Iter);
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
		OnBankerUserChange();
		BankerIsChange = false;
	}
}
void DialManager::OnInitDial()
{
	if (m_TableResult == 0)
	{
		for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		{
			m_TableAreaRate[i] = 0;
		}
		//第一次生成轮盘
		for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
		{
			BYTE Value = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[i];//0-3 外圈数据
			BYTE Value2 = RandUInt() % 3;//0-2 内圈数据
			BYTE Result = Value2 * 4 + Value;
			m_TableAreaData[i] = Result;
			m_TableAreaRate[Result] += 1;
		}

		for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		{
			if (m_TableAreaRate[i] == 0)
			{
				//当前选项需要重新随机
				BYTE Value = i / 4;//0-2 颜色
				BYTE Rand = i % 4;//0-3 动物
				for (BYTE j = 0; j < MAX_DIAL_ClientSum; ++j)
				{
					if (m_TableAreaRate[j] > 1 && j % 4 == Rand)
					{
						//修改数据
						for (BYTE k = 0; k < MAX_DIAL_GameSum; ++k)
						{
							if (m_TableAreaData[k] == j)
							{
								m_TableAreaData[k] = i;
								m_TableAreaRate[j] -= 1;
								m_TableAreaRate[i] += 1;
								break;
							}
						}
						break;
					}
				}
			}
		}

		BYTE ID = static_cast<BYTE>(RandUInt() % g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.RateMap.size());
		for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		{
			m_TableAreaRate[i] = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.RateMap[ID].RateArray[i];
		}

		/*for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		{
			BYTE Rate = MAX_DIAL_GameSum / m_TableAreaRate[i];
			m_TableAreaRate[i] = Rate;
		}*/

		/*g_FishServer.ShowInfoToWin("初始化轮盘");

		{
			string str = "";
			for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
			{
				BYTE Result = m_TableAreaData[i] / 4;
				BYTE Value = m_TableAreaData[i] % 4;
				string strValue = (Value == 0 ? "熊猫" : (Value == 1 ? "猴子" : (Value == 2 ? "兔子" : "狮子")));
				str += strValue;
				if (i != MAX_DIAL_GameSum - 1)
					str += ",";
			}
			g_FishServer.ShowInfoToWin(str.c_str());
		}*/
	}
}
void DialManager::OnMoveDialByResult()
{
	//每局结束的时候 刷新轮盘数据
	BYTE Index1 = (m_TableResult >> 8);
	BYTE Index2 = static_cast<BYTE>(m_TableResult);
	BYTE Array1[MAX_DIAL_GameSum] = { 0 };
	if (Index2 == Index1)
	{
		for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
		{
			Array1[i] = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[i];
		}
	}
	else if (Index1 > Index2)
	{
		BYTE Change = Index1 - Index2;
		for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
		{
			BYTE Index = (i + Change) % MAX_DIAL_GameSum;
			Array1[Index] = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[i];
		}
	}
	else
	{
		BYTE Change = Index2 - Index1;
		for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
		{
			BYTE Index = (MAX_DIAL_GameSum + i - Change) % MAX_DIAL_GameSum;
			Array1[Index] = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[i];
		}
	}
	for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)//设置轮盘的数据
	{
		m_TableAreaData[i] = (m_TableAreaData[i] / 4) * 4 + Array1[i];
	}
}
BYTE DialManager::GetIndexByParam(BYTE Param,bool States)
{
	if (States)
	{
		//外圈
		vector<BYTE> pVec;
		for (BYTE j = 0; j < MAX_DIAL_GameSum; ++j)
		{
			if (g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[j] % 4 == Param % 4)
			{
				pVec.push_back(j);
			}
		}
		size_t Index = RandUInt() % pVec.size();
		return pVec[Index];
	}
	else
	{
		//内圈
		vector<BYTE> pVec;
		for (BYTE j = 0; j < MAX_DIAL_GameSum; ++j)
		{
			if (m_TableAreaData[j] / 4 == Param / 4)
			{
				pVec.push_back(j);
			}
		}
		size_t Index = RandUInt() % pVec.size();
		return pVec[Index];
	}
}
void DialManager::CreateNewDial() //设置轮盘上的数值
{
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		m_TableAreaRate[i] = 0;
	}
	//2.确定内圈数据  内圈 保证 0-1-2 并且保证 全部的 12种选择 全部有值
	//外圈固定了 我们生成内圈的数据
	BYTE Result[MAX_DIAL_GameSum] = { 0 };
	BYTE ID = static_cast<BYTE>(RandUInt() % g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.RateMap.size());
	
	BYTE TempArray[MAX_DIAL_ClientSum] = { 0 };
	for (BYTE x = 0; x < MAX_DIAL_ClientSum; ++x)
	{
		BYTE Value = RandUInt() % 12;//0-11
		while (TempArray[Value] == 1)
		{
			Value = RandUInt() % 12;
		}
		TempArray[Value] = 1;
		//具体数据
		BYTE i = Value;
		for (BYTE j = 0; j < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.RateMap[ID].SumArray[i]; ++j)
		{
			bool IsResult = false;
			//不一定按 顺序的来 按倒序的来
			for (BYTE k = 0; k < MAX_DIAL_GameSum; ++k)
			{
				if (/*m_TableAreaData[k] % 4 */g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[k] % 4 == i % 4 && Result[k] == 0)
				{
					m_TableAreaData[k] = i;
					Result[k] = 1;
					m_TableAreaRate[i] += 1;
					IsResult = true;
					break;
				}
			}
			//
			if (!IsResult)
				g_FishServer.ShowInfoToWin("轮盘的数据 错误 无法生成颜色数据 %d", ID);
		}
	}

	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		if (m_TableAreaRate[i] == 0)
		{
			//当前选项需要重新随机
			BYTE Value = i / 4;//0-2 颜色
			BYTE Rand = i % 4;//0-3 动物
			for (BYTE j = 0; j < MAX_DIAL_ClientSum; ++j)
			{
				if (m_TableAreaRate[j] > 1 && j % 4 == Rand)
				{
					//修改数据
					for (BYTE k = 0; k < MAX_DIAL_GameSum; ++k)
					{
						if (m_TableAreaData[k] == j)
						{
							m_TableAreaData[k] = i;
							m_TableAreaRate[j] -= 1;
							m_TableAreaRate[i] += 1;
							break;
						}
					}
					break;
				}
			}
		}
	}
	
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		m_TableAreaRate[i] = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.RateMap[ID].RateArray[i];
	}
}
void DialManager::OnTableJoinBegin()
{
	//当桌子进入下注阶段的时候
	//1.判断庄家是否需要进行变化
	SetNextBankerUserInfo();
	//2.清空桌子上的数据
	OnClearTableInfo();
	//3.通知玩家 进入新的阶段了
	SetTableStates(DTS_Begin);//桌子进入下注阶段
	//4.生成轮盘数据
	CreateNewDial();
	//5.
	MG_Cmd_DialTableJoinBegin msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialTableJoinBegin), sizeof(MG_Cmd_DialTableJoinBegin));
	for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
		msg.TableAreaData[i] = m_TableAreaData[i];
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		msg.TableAreaDataRate[i] = m_TableAreaRate[i];
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
	m_animalrobotmanager.StartGame();
}
void DialManager::OnUpdateTableInfo()
{
	//更新桌子上的数据
	MG_Cmd_DialTableUpdate msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialTableUpdate), sizeof(MG_Cmd_DialTableUpdate));
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		msg.TableBetAreaGlobel[i] = m_TableBetAreaGlobel[i];
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
}
BYTE DialManager::GetAreaID()
{
	BYTE Index1 = (m_TableResult >> 8);
	BYTE Index2 = static_cast<BYTE>(m_TableResult);
	//0-11
	BYTE Color = m_TableAreaData[Index1] / 4;
	BYTE Item = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[Index2];//m_TableAreaData[Index2] % 4;
	BYTE Result = Color * 4 + Item;
	return Result;
}
void DialManager::ChangeAreaDataByResult()
{
	//修改轮盘的数据 
	BYTE Index1 = (m_TableResult >> 8);
	BYTE Index2 = static_cast<BYTE>(m_TableResult);

	BYTE AreaData1[MAX_DIAL_GameSum] = { 0 };
	BYTE AreaData2[MAX_DIAL_GameSum] = { 0 };
	//内圈数据 设置 
	for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
	{
		BYTE Index = (i >= Index1 ? i - Index1 : (MAX_DIAL_GameSum + i - Index1));
		AreaData1[Index] = m_TableAreaData[i] / 4;
	}
	//外圈数据设置
	for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
	{
		BYTE Index = (i >= Index2 ? i - Index2 : (MAX_DIAL_GameSum + i - Index2));
		AreaData2[Index] = m_TableAreaData[i] % 4;
	}
	//
	for (BYTE i = 0; i < MAX_DIAL_GameSum; ++i)
	{
		m_TableAreaData[i] = AreaData1[i] * 4 + AreaData2[i];
	}
}
INT64 DialManager::GetBrandUserGlobelChange()
{
	INT64 GlobelChange = 0;
	BYTE ID = GetAreaID();
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		//内圈决定颜色 完全决定 动物 
		if (ID == i)
		{
			GlobelChange -= (m_TableAreaRate[i] * m_TableBetAreaGlobel[i]);
		}
		else
		{
			GlobelChange += m_TableBetAreaGlobel[i];
		}
	}
	return GlobelChange;
}
INT64 DialManager::GetSystemGlobelChange()
{
	bool IsSystemBranker = false;
	if (m_TableBankerUserID == 0)//庄家为系统的话 或者 庄家为机器人的时候
	{
		IsSystemBranker = true;
	}
	else
	{
		HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(m_TableBankerUserID);
		if (Iter != m_TableUserMap.end() && Iter->second.IsRobot)
		{
			IsSystemBranker = true;
		}
	}

	INT64 GlobelChange = 0;
	BYTE ID = GetAreaID();
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		{
			if (Iter->second.BetGlobel[i] == 0)
				continue;
			if (i == ID)
			{
				if (Iter->second.IsRobot)
				{
					if (!IsSystemBranker)
						GlobelChange += (Iter->second.BetGlobel[i] * m_TableAreaRate[i]);
				}
				else
				{
					if (IsSystemBranker)
						GlobelChange -= (Iter->second.BetGlobel[i] * m_TableAreaRate[i]);
				}
			}
			else
			{
				if (Iter->second.IsRobot)
				{
					if (!IsSystemBranker)
						GlobelChange -= (Iter->second.BetGlobel[i]);
				}
				else
				{
					if (IsSystemBranker)
						GlobelChange += (Iter->second.BetGlobel[i]);
				}
			}
		}
	}
	return GlobelChange;
}
INT64 DialManager::ChangeResultBySystem(BYTE OldIndex2)
{
	//1.重新随机系统的牌型 直到系统不亏钱
	BYTE Index = 0;
	INT64 SystemGlobel = 0;
	do
	{
		//随机参数一个数据
		//m_TableResult = RandUInt() % MAX_DIAL_GameSum;
		BYTE Index1 = 0;// RandUInt() % MAX_DIAL_GameSum;
		BYTE Index2 = 0;// RandUInt() % MAX_DIAL_GameSum;
		CreateNewResult(Index1, Index2);//产生结果
		Index2 += OldIndex2;
		Index2 %= MAX_DIAL_GameSum;
		m_TableResult = (Index1 << 8) | Index2;
		//ChangeAreaDataByResult();
		SystemGlobel = GetSystemGlobelChange();
		++Index;
	} while (SystemGlobel < 0 && Index < 200);//必须在200次内
	return SystemGlobel;
}
void DialManager::CreateNewResult(BYTE& Index1, BYTE& Index2)
{
	Index1 = 0;
	Index2 = 0;
	//根据轮盘的倍率 参数一个具体的结果 符合概率限制 数量大的概率小 需要被一个数除
	DWORD MaxSum = 0;
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		if (MaxSum == 0)
			MaxSum = m_TableAreaRate[i];
		else if (MaxSum % m_TableAreaRate[i] != 0)
			MaxSum *= m_TableAreaRate[i];
	}
	DWORD TempArray[MAX_DIAL_ClientSum];
	DWORD MaxRateValue = 0;
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		TempArray[i] = MaxSum / m_TableAreaRate[i];//权重
		MaxRateValue += TempArray[i];
	}
	DWORD Value = RandUInt() % MaxRateValue;
	DWORD BeAdddValue = 0;
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
	{
		if (Value < BeAdddValue + TempArray[i])
		{
			//结果 为 i  我们进行处理
			//根据结果 我们确定 转盘的转到数据
			//1.内圈
			/*for (BYTE j = 0; j < MAX_DIAL_GameSum; ++j)
			{
				if (m_TableAreaData[j] / 4 == i / 4)
				{
					Index1 = j;
					break;
				}
			}*/
			Index1 = GetIndexByParam(i, false);
			//2.外圈
			Index2 = GetIndexByParam(i, true);
			/*for (BYTE j = 0; j < MAX_DIAL_GameSum; ++j)
			{
				if (g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[j] % 4 == i % 4)
				{
					Index2 = j;
					break;
				}
			}*/
			return;
		}
		else
		{
			BeAdddValue += TempArray[i];
		}
	}
}
void DialManager::OnTableJoinEnd()
{
	//当比赛开牌的时候 
	//进行结算的时候 
	UINT64 TotalAllAddGlobel = 0;
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		TotalAllAddGlobel += m_TableBetAreaGlobel[i];

	//根据概率 生成一个 结果
	BYTE Index1 = 0; //RandUInt() % MAX_DIAL_GameSum;
	BYTE Index2 = 0;// RandUInt() % MAX_DIAL_GameSum;
	INT64 SystemGlobel=0;
	INT64 BrandUserGlobel = 0;
	if (g_FishServer.GetAnimalControl().NeedControl())	
	{
		DWORD Value = g_FishServer.GetAnimalControl().GetSuitResult();		
		//1.外圈
		for (BYTE j = 0; j < MAX_DIAL_GameSum; ++j)
		{
			if (m_TableAreaData[j] / 4 == Value / 4)
			{
				Index1 = j;
				break;
			}
		}
		//2.外圈
		for (BYTE j = 0; j < MAX_DIAL_GameSum; ++j)
		{
			if (g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[j] % 4 == Value % 4)
			{
				Index2 = j;
				break;
			}
		}
		m_TableResult = (Index1 << 8) | Index2;
		SystemGlobel = GetSystemGlobelChange();
		BrandUserGlobel = GetBrandUserGlobelChange();
		g_FishServer.GetAnimalControl().CompleteControl();
	}
	else
	{
		CreateNewResult(Index1, Index2);//产生结果
		BYTE OldIndex2 = (BYTE)m_TableResult;
		Index2 += OldIndex2;
		Index2 %= MAX_DIAL_GameSum;

		m_TableResult = (Index1 << 8) | Index2;
		//ChangeAreaDataByResult();//修改轮盘
		//m_TableResult = RandUInt() % MAX_DIAL_GameSum;
		//控制优先
		 SystemGlobel = GetSystemGlobelChange();
		 BrandUserGlobel = GetBrandUserGlobelChange();
		if (m_SystemGlobelSum < 0 || (m_SystemGlobelSum + SystemGlobel) < 0)
		{
			SystemGlobel = ChangeResultBySystem(OldIndex2);
			BrandUserGlobel = GetBrandUserGlobelChange();//根据结果计算庄家金币的变化
		}
	}

	{
		BYTE Index1 = (m_TableResult >> 8);
		BYTE Index2 = static_cast<BYTE>(m_TableResult);
		BYTE Rank = m_TableAreaData[Index1] / 4;
		BYTE Value = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.DialAreaData[Index2];
		string strRank = (Rank == 0 ? "红" : (Rank == 1 ? "黄" : "绿"));
		string strValue = (Value == 0 ? "熊猫" : (Value == 1 ? "猴子" : (Value ==2?"兔子":"狮子")));
		string strInfo = strValue + strRank;
		g_FishServer.ShowInfoToWin(strInfo.c_str());
	}

	/*{
		BYTE Rank = GetAreaID() / 4;
		BYTE Value = GetAreaID()%4;
		string strRank = (Rank == 0 ? "红" : (Rank == 1 ? "黄" : "绿"));
		string strValue = (Value == 0 ? "熊猫" : (Value == 1 ? "猴子" : (Value == 2 ? "兔子" : "狮子")));
		string strInfo = "确认结果:" +strValue + strRank;
		g_FishServer.ShowInfoToWin(strInfo.c_str());
	}*/
	m_DialResultLog.SetResult(GetAreaID());//根据当局的结果设置数据
	//CreateNewDial();//根据最新的结果 刷新下轮盘数据
	OnMoveDialByResult();

	//3.干涉完毕后 我们进行处理
	MG_Cmd_DialTableJoinEnd msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialTableJoinEnd), sizeof(MG_Cmd_DialTableJoinEnd));
	for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
		msg.TableBetAreaGlobel[i] = m_TableBetAreaGlobel[i];
	msg.TableResultIndex = m_TableResult;
	
	msg.TableGameSum = m_DialResultLog.LogSum;
	msg.TableResultLog = m_DialResultLog.ResultLog;
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
		for (BYTE i = 0; i < MAX_DIALSHOWBRAND_Sum; ++i)
		{
			msg.BankerListGlobelInfo[i].dwUserID = 0;
			msg.BankerListGlobelInfo[i].dwGlobelSum = 0;
		}
	}
	else
	{
		vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
		for (BYTE i = 0; Iter != m_TableWriteBankerVec.end() && i < MAX_DIALSHOWBRAND_Sum;)
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
	for (BYTE i = 0; i < MAX_DIALVIPSEAT_Sum; ++i)
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


	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		if (m_TableBankerUserID != 0 && msg.dwUserID == m_TableBankerUserID)
		{
			msg.AddGlobelSum = BrandUserGlobel;
		}
		else
		{
			//处理当前玩家的数据
			int AddGlobel = 0;
			for (BYTE i = 0; i < MAX_DIAL_ClientSum; ++i)
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

			if (AddGlobel == 0 && Iter->second.IsInVipSeat && m_VipSeatVec[Iter->second.VipSeat].dwUserID == Iter->first)
			{
				//当前玩家未下注
				++m_VipSeatVec[Iter->second.VipSeat].NonBetGlobelSum;
				if (m_VipSeatVec[Iter->second.VipSeat].NonBetGlobelSum >= g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.VipNonBetSum)
				{
					if ((pRole->GetRoleInfo().ParticularStates & ParticularState_DialVip) != 0)
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
		m_SystemGlobelSum = static_cast<INT64>(m_SystemGlobelSum * g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.GetChangeGlobelRateValue());

	if (m_TableBankerUserID != 0)
	{
		if (BrandUserGlobel >= 0)
			g_DBLogManager.LogToDB(m_TableBankerUserID, LT_Dial, 1, static_cast<DWORD>(BrandUserGlobel), TEXT("玩家坐庄森林舞会获得金币"), SendLogDB);
		else
			g_DBLogManager.LogToDB(m_TableBankerUserID, LT_Dial, 0, static_cast<DWORD>(BrandUserGlobel*-1), TEXT("玩家坐庄森林舞会失去金币"), SendLogDB);
	}
	//存储到数据库里面去
	g_DBLogManager.LogDialInfoToDB(m_TableBankerUserID, m_TableAreaData, m_TableBetAreaGlobel, m_TableResult, SystemGlobel, m_SystemGlobelSum, m_TableUserMap.size(), SendLogDB);
	//判断当前在排队列表里的玩家 
	MG_Cmd_DialWriteBankerUserGlobelMsg msgSend;
	SetMsgInfo(msgSend, GetMsgType(Main_Dial, MG_DialWriteBankerUserGlobelMsg), sizeof(MG_Cmd_DialWriteBankerUserGlobelMsg));
	vector<DWORD>::iterator IterVec = m_TableWriteBankerVec.begin();
	for (; IterVec != m_TableWriteBankerVec.end(); ++IterVec)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(*IterVec);
		if (!pRole)
			continue;
		if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().dialConfig.JoinBankerGlobelSum)
		{
			msgSend.dwUserID = *IterVec;
			pRole->SendDataToGameServer(&msgSend);
		}
	}
	m_animalrobotmanager.EndGame();
}
SHORT DialManager::GetBrandRateValue(BYTE Index)
{
	if (Index >= MAX_DIAL_ClientSum)
		return 1;
	if (GetAreaID() == Index)
	{
		//比庄家大的时候
		BYTE Rate = m_TableAreaRate[Index];
		//玩家赢庄家 退还本金 在算倍率
		return static_cast<SHORT>(Rate);
	}
	else
	{
		return 0;
	}
}
void DialManager::OnRoleJoinVipSeat(DWORD dwUserID, BYTE VipSeatIndex)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_DialRoleJoinVipSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialRoleJoinVipSeat), sizeof(MG_Cmd_DialRoleJoinVipSeat));
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
	if (VipSeatIndex >= MAX_DIALVIPSEAT_Sum)
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
				MG_Cmd_DialRoleBeLeaveVipSeat msgLeave;
				SetMsgInfo(msgLeave, GetMsgType(Main_Dial, MG_DialRoleBeLeaveVipSeat), sizeof(MG_Cmd_DialRoleBeLeaveVipSeat));
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
	HashMap<DWORD, tagDialRoleInfo>::iterator IterSend = m_TableUserMap.begin();//Vip席位的改变发送给全部玩家
	for (; IterSend != m_TableUserMap.end(); ++IterSend)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterSend->first);
		if (!pRole)
			continue;
		msg.dwUserID = IterSend->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void DialManager::OnRoleLeaveVipSeat(DWORD dwUserID)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
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

	MG_Cmd_DialRoleLeaveVipSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialRoleLeaveVipSeat), sizeof(MG_Cmd_DialRoleLeaveVipSeat));
	msg.dwDestUserID = dwUserID;
	msg.VipSeatIndex = Iter->second.VipSeat;
	HashMap<DWORD, tagDialRoleInfo>::iterator IterSend = m_TableUserMap.begin();
	for (; IterSend != m_TableUserMap.end(); ++IterSend)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterSend->first);
		if (!pRole)
			continue;
		msg.dwUserID = IterSend->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void DialManager::OnRoleGetNormalInfoByPage(DWORD dwUserID, DWORD Page)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagDialRoleInfo>::iterator IterRole = m_TableUserMap.find(dwUserID);
	if (IterRole == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}

	DWORD BeginIndex = Page * MAX_DIALNORMAL_PAGESUM;
	DWORD EndIndex = (Page + 1)* MAX_DIALNORMAL_PAGESUM - 1;


	MG_Cmd_DialGetNormalSeatInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_Dial, MG_DialGetNormalSeatInfo), sizeof(MG_Cmd_DialGetNormalSeatInfo));
	msg.dwUserID = dwUserID;
	msg.Page = Page;
	msg.TotalRoleSum = m_TableUserMap.size();

	for (BYTE i = 0; i < MAX_DIALVIPSEAT_Sum; ++i)
	{
		if (m_VipSeatVec[i].dwUserID != 0)
			--msg.TotalRoleSum;
	}
	if (m_TableBankerUserID != 0)
		--msg.TotalRoleSum;

	for (BYTE i = 0; i < MAX_DIALNORMAL_PAGESUM; ++i)
	{
		msg.Array[i].dwFaceID = 0;
		msg.Array[i].dwUserID = 0;
		msg.Array[i].GlobelSum = 0;
		msg.Array[i].NickName[0] = 0;
	}
	HashMap<DWORD, tagDialRoleInfo>::iterator Iter = m_TableUserMap.begin();
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

DWORD DialManager::BankerId()
{
	return m_TableBankerUserID;
}

WORD DialManager::ApplyListLength()
{
	return m_TableWriteBankerVec.size();
}

DWORD DialManager::BankerGold()
{
	Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
	if (pBankerRole)
	{
		return pBankerRole->GetGlobelSum();
	}
	return 0;
}
bool	DialManager::IsGameEnd()
{
	return m_TableStates == DTS_End;
}

bool	DialManager::HaveApply(DWORD dwUserid)
{
	return find(m_TableWriteBankerVec.begin(), m_TableWriteBankerVec.end(), dwUserid) != m_TableWriteBankerVec.end();
}

void DialManager::QueryNames(DWORD ClientID)
{
	int nNamelength = (MAX_NICKNAME + 1);
	int nSize = sizeof(LC_CMD_AnimalAdmin_PlayerNames)+sizeof(TCHAR)*nNamelength*(m_TableUserMap.size() - m_animalrobotmanager.RobotCount());
	LC_CMD_AnimalAdmin_PlayerNames *pResult = (LC_CMD_AnimalAdmin_PlayerNames*)malloc(nSize);
	pResult->SetCmdSize(nSize);
	pResult->SetCmdType((Main_Control << 8) | LC_AnimalAdminQueryName);
	pResult->ClientID = ClientID;
	int nIndex = 0;
	for (HashMap<DWORD, tagDialRoleInfo>::iterator it = m_TableUserMap.begin(); it != m_TableUserMap.end(); it++)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(it->first);
		if (pRole&&!m_animalrobotmanager.IsRobot(pRole->GetRoleID()))
		{
			TCHARCopy(&pResult->names[1 + nIndex*nNamelength], nNamelength, pRole->GetNickName(), MAX_NICKNAME);
			nIndex++;
		}
	}
	g_FishServer.SendNetCmdToControl(pResult);
	free(pResult);
}

void DialManager::QueryAllJetton(DWORD ClientID)
{
	LC_CMD_AnimalAdmin_AllJetton result;
	ZeroMemory(&result, sizeof(result));
	result.SetCmdSize(sizeof(result));
	result.SetCmdType((Main_Control << 8) | LC_AnimalAdminQueryAllJetton);
	result.ClientID = ClientID;
	result.nStock = m_SystemGlobelSum;
	CalPlayerJetton(result.nJetton);
	g_FishServer.SendNetCmdToControl(&result);

}

void DialManager::QueryPlayerJetton(TCHAR *pNickame, DWORD ClientID)
{
	for (HashMap<DWORD, tagDialRoleInfo>::iterator it = m_TableUserMap.begin(); it != m_TableUserMap.end(); it++)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(it->first);
		if (pRole)
		{
			if (_tcscmp(pNickame, pRole->GetNickName()) == 0)
			{
				//HashMap<DWORD, tagDialRoleInfo>::iterator itJetton = m_TableUserMap.find(pRole->GetRoleID());
				//if (itJetton != m_TableUserMap.end())
				{
					LC_CMD_AnimalAdmin_PlayerJetton result;
					result.SetCmdSize(sizeof(result));
					result.SetCmdType((Main_Control << 8) | LC_AnimalAdminQueryPlayerJetton);
					result.ClientID = ClientID;
					//TCHARCopy(result.name, MAX_NICKNAME + 1, pNickame, MAX_NICKNAME);
					memcpy(result.dwJetton, it->second.BetGlobel, sizeof(result.dwJetton));
					g_FishServer.SendNetCmdToControl(&result);
					break;
				}
			}
		}
	}
}

void DialManager::CalPlayerJetton(UINT64 uJetton[MAX_DIAL_ClientSum])
{
	for (HashMap<DWORD, tagDialRoleInfo>::iterator it = m_TableUserMap.begin(); it != m_TableUserMap.end(); it++)
	{
		if (!m_animalrobotmanager.IsRobot(it->first))
		{
			for (int i = 0; i < MAX_DIAL_ClientSum; i++)
			{
				uJetton[i] += it->second.BetGlobel[i];
			}
		}
	}
}