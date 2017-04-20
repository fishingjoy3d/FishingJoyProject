#include "Stdafx.h"
#include "CarManager.h"
#include "../CommonFile/DBLogManager.h"
#include "FishServer.h"
extern void SendLogDB(NetCmd* pCmd);
CarManager::CarManager()
{

}
CarManager::~CarManager()
{
	OnDestroy();
}
void CarManager::SetTableStates(CarTableStates States)
{
	m_TableStates = States;
	m_TableStatesLog = timeGetTime();
}
bool CarManager::OnInit()
{
	SetTableStates(CTS_WriteBegin);//设置初始化的时候 桌子状态为等待开始
	m_SystemGlobelSum = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.InitGlobelSum;//系统金币
	//初始化一些数据
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
	{
		m_TableBetAreaGlobel[i] = 0;
	}
	for (BYTE i = 0; i < MAX_CARVIPSEAT_Sum; ++i)
	{
		m_VipSeatVec[i].dwUserID = 0;
		m_VipSeatVec[i].NonBetGlobelSum = 0;
	}
	m_TableBankerUserID = 0;
	m_TableBankerUseGameSum = 0;
	m_TableResult = 0;
	m_TableResultIndex = 0;
	m_TableWriteBankerVec.clear();
	m_TableUserMap.clear();
	m_IsNeedClearBankerInfo = false;
	return true;
}
void CarManager::OnUpdate(DWORD dwTimer)
{
	static DWORD CarUpdateTime = 0;
	if (CarUpdateTime == 0 || dwTimer - CarUpdateTime >= 1000) //1s更新一次
	{
		CarUpdateTime = dwTimer;
		UpdateTableIsCanBegin(timeGetTime());
		UpdateTableIsCanEnd(timeGetTime());
		UpdateTableInfo(timeGetTime());
		m_robotmanager.Update();
	}
}
void CarManager::OnDestroy()
{
}
void CarManager::OnRoleJoinCarTable(DWORD dwUserID)
{
	//玩家进入试图进入牛牛桌子
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_CarRoleJoinTable msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarRoleJoinTable), sizeof(MG_Cmd_CarRoleJoinTable));
	msg.dwUserID = dwUserID;
	msg.Result = false;
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter != m_TableUserMap.end())
	{
		//立刻让玩家离开
		OnRoleLeaveCarTable(dwUserID);
	}
	if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.JoinGlobelSum)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	if (m_TableStates == CTS_WriteBegin)
	{
		//因为有人进入了 
		SetTableStates(CTS_Begin);
		OnClearTableInfo();//清理桌子上的数据
	}
	msg.Result = true;
	tagCarRoleInfo pInfo;
	pInfo.UserID = dwUserID;
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
		pInfo.BetGlobel[i] = 0;
	pInfo.IsBankerUser = false;
	pInfo.IsInWriteBanakerVec = false;
	pInfo.IsInVipSeat = false;
	pInfo.VipSeat = 0;
	pInfo.IsRobot = g_FishServer.GetRobotManager().IsRobot(dwUserID);

	m_TableUserMap.insert(HashMap<DWORD, tagCarRoleInfo>::value_type(dwUserID, pInfo));
	//继续设置数据到命令里面
	msg.TableStates = m_TableStates;
	DWORD NowTime = timeGetTime();
	if (m_TableStates == CTS_Begin)
		msg.TableStatesUpdateSec = static_cast<BYTE>((g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.BeginWriteSec * 1000 - (NowTime - m_TableStatesLog)) / 1000);
	else if (m_TableStates == CTS_End)
		msg.TableStatesUpdateSec = static_cast<BYTE>((g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.EndWriteSec * 1000 - (NowTime - m_TableStatesLog)) / 1000);
	else
		msg.TableStatesUpdateSec = 0;
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
	{
		msg.TableBetAreaGlobel[i] = m_TableBetAreaGlobel[i];
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
	for (BYTE i = 0; i < MAX_CARSHOWBRAND_Sum; ++i)
	{
		msg.TableWriteBankerList[i].dwUserID = 0;
		msg.TableWriteBankerList[i].GlobelSum = 0;
		msg.TableWriteBankerList[i].NickName[0] = 0;
		msg.TableWriteBankerList[i].dwFaceID = 0;
	}
	if (!m_TableWriteBankerVec.empty())
	{
		vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
		for (BYTE i = 0; Iter != m_TableWriteBankerVec.end() && i < MAX_CARSHOWBRAND_Sum;)
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
	for (BYTE i = 0; i < MAX_CARVIPSEAT_Sum; ++i)
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
	msg.TableGameSum = m_CarResultLog.LogSum;
	msg.TableResultLog = m_CarResultLog.ResultLog;
	msg.TableResultIndex = m_TableResultIndex;
	pRole->SendDataToGameServer(&msg);
}
void CarManager::OnRoleLeaveCarTable(DWORD dwUserID)
{
	//当玩家离开牛牛桌子的时候 
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
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
		if (m_TableStates == CTS_Begin)//如果为下注阶段
		{
			//庄家下注了 立刻由系统接手
			m_TableBankerUserID = 0;
			m_TableBankerUseGameSum = 0;
			OnBankerUserChange();//庄家发生变化了
		}
		else if (m_TableStates == CTS_End)
		{
			m_IsNeedClearBankerInfo = true;//在开牌阶段 清理下庄家
		}
	}
	m_TableUserMap.erase(Iter);
}
bool CarManager::IsCanAddGlobel(DWORD AddGlobel, BYTE Index)
{
	UINT64 MaxGlobel = 0;
	for (BYTE i = 0; i <MAX_CAR_ClientSum; ++i)
	{
		UINT64 Globel = m_TableBetAreaGlobel[i];
		if (i == Index)
		{
			Globel += AddGlobel;
		}
		Globel = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.GetRateValue(i) * Globel;
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
void CarManager::OnRoleBetGlobelByLog(DWORD dwUserID, DWORD AddGlobelArray[MAX_CAR_ClientSum])
{
	//玩家进行继续押注
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}

	MG_Cmd_CarRoleBetGlobelByLog msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarRoleBetGlobelByLog), sizeof(MG_Cmd_CarRoleBetGlobelByLog));
	msg.dwUserID = dwUserID;

	DWORD AllGlobel = 0; // 下注的总金币数量
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
	{
		AllGlobel += AddGlobelArray[i];
		msg.betGlobel[i] = AddGlobelArray[i];
	}

	msg.AllGlobel = AllGlobel;
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
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
	if (m_TableStates != CTS_Begin)
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
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
	{
		RoleBetAllGlobel += Iter->second.BetGlobel[i];
	}

	if (RoleBetAllGlobel + AllGlobel> g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.MaxAddGlobel)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}

	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
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
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
	{
		Iter->second.BetGlobel[i] += AddGlobelArray[i];
		m_TableBetAreaGlobel[i] += AddGlobelArray[i];

		if (!Iter->second.IsRobot)
			g_DBLogManager.LogToDB(dwUserID, LogType::LT_Car, AddGlobelArray[i], i, TEXT("玩家转盘下注"), SendLogDB);//Param 表示 下注区
	}
	pRole->SendDataToGameServer(&msg);

	if (Iter->second.IsInVipSeat)
	{
		for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
		{
			if (AddGlobelArray[i] == 0)
				continue;
			MG_Cmd_CarVipSeatGlobelChange msg;
			SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarVipSeatGlobelChange), sizeof(MG_Cmd_CarVipSeatGlobelChange));
			msg.VipSeat = Iter->second.VipSeat;
			msg.GlobelSum = AddGlobelArray[i];
			msg.Index = i;
			HashMap<DWORD, tagCarRoleInfo>::iterator IterRole = m_TableUserMap.begin();
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
void CarManager::OnRoleBetGlobel(DWORD dwUserID, BYTE BetIndex, DWORD AddGlobel)
{
	//玩家下注
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_CarRoleBetGlobel msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarRoleBetGlobel), sizeof(MG_Cmd_CarRoleBetGlobel));
	msg.dwUserID = dwUserID;
	msg.AddGlobel = AddGlobel;
	msg.Index = BetIndex;
	msg.Result = false;
	if (BetIndex >= MAX_CAR_ClientSum)
	{
		pRole->SendDataToGameServer(&msg);
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
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
	if (m_TableStates != CTS_Begin)
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
	for (BYTE i = 0; i<MAX_CAR_ClientSum; ++i)
	{
		RoleBetAllGlobel += Iter->second.BetGlobel[i];
		TableBetAllGbel += m_TableBetAreaGlobel[i];
	}
	if (RoleBetAllGlobel + AddGlobel > g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.MaxAddGlobel)//当前下注区的金币太多
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
		g_DBLogManager.LogToDB(dwUserID, LogType::LT_Car, AddGlobel, BetIndex, TEXT("玩家转盘下注"), SendLogDB);//Param 表示 下注区
	pRole->SendDataToGameServer(&msg);

	if (Iter->second.IsInVipSeat)
	{
		MG_Cmd_CarVipSeatGlobelChange msg;
		SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarVipSeatGlobelChange), sizeof(MG_Cmd_CarVipSeatGlobelChange));
		msg.VipSeat = Iter->second.VipSeat;
		msg.GlobelSum = AddGlobel;
		msg.Index = BetIndex;
		HashMap<DWORD, tagCarRoleInfo>::iterator IterRole = m_TableUserMap.begin();
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
void CarManager::OnRoleGetWriteBankerList(DWORD dwUserID)
{
	//玩家进入庄家排队列表
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	//发生命令到客户端去
	MG_Cmd_CarGetBankerList msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarGetBankerList), sizeof(MG_Cmd_CarGetBankerList));
	msg.dwUserID = dwUserID;
	msg.dwMySeatIndex = 0xffffffff;
	for (BYTE i = 0; i < MAX_CARSHOWBRAND_Sum; ++i)
	{
		msg.TableWriteBankerList[i].dwUserID = 0;
		msg.TableWriteBankerList[i].GlobelSum = 0;
		msg.TableWriteBankerList[i].NickName[0] = 0;
		msg.TableWriteBankerList[i].dwFaceID = 0;
	}
	if (!m_TableWriteBankerVec.empty())
	{
		vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
		for (BYTE i = 0; Iter != m_TableWriteBankerVec.end() && i < MAX_CARSHOWBRAND_Sum;)
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
void CarManager::OnRoleJoinWriteBankerList(DWORD dwUserID)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	//玩家试图进行上庄 
	MG_Cmd_CarRoleJoinBankerList msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarRoleJoinBankerList), sizeof(MG_Cmd_CarRoleJoinBankerList));
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
	if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.JoinBankerGlobelSum)
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
	if (msg.SeatIndex < MAX_CARSHOWBRAND_Sum)
	{
		OnWriteBankerChange();
	}
	else
	{
		pRole->SendDataToGameServer(&msg);
	}
}
void CarManager::OnRoleGetWriteBankerFirstSeat(DWORD dwUserID)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_CarRoleGetBankerFirstSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarRoleGetBankerFirstSeat), sizeof(MG_Cmd_CarRoleGetBankerFirstSeat));
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
	if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.GetNextBankerNeedGlobel)
	{
		pRole->SendDataToGameServer(&msg);
		return;
	}
	//可以抢庄
	msg.Result = true;
	//1.扣除金币
	pRole->OnChangeRoleGlobelSum(pRole->GetGlobelSum() - g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.GetNextBankerNeedGlobel);
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
void CarManager::OnRoleLeaveWriteBankerList(DWORD dwUserID)
{
	//玩家离开上庄排队列表 
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_CarRoleLeaveBankerList msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarRoleLeaveBankerList), sizeof(MG_Cmd_CarRoleLeaveBankerList));
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
	if (Index < MAX_CARSHOWBRAND_Sum)
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
void CarManager::OnRoleCanelBanker(DWORD dwUserID)
{
	//玩家下庄
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_CarRoleCanelBankerSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarRoleCanelBankerSeat), sizeof(MG_Cmd_CarRoleCanelBankerSeat));
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
	if (m_TableStates == CTS_Begin)
	{
		//应该让玩家的金币 暂时冻结1分钟 


		//下注阶段 玩家下庄 立刻又系统补上
		m_TableBankerUserID = 0;
		m_TableBankerUseGameSum = 0;
		OnBankerUserChange();
	}
	else if (m_TableStates == CTS_End)
	{
		//开牌阶段 玩家下庄 直接下注 设置
		//设置玩家下庄成功 
		m_IsNeedClearBankerInfo = true;
	}
	if (pRole)
		pRole->SendDataToGameServer(&msg);
}
void CarManager::OnRoleLeaveMiniGameServer(DWORD dwUserID)
{
	OnRoleLeaveCarTable(dwUserID);
}
void CarManager::OnRoleGlobelChange(DWORD dwUserID, int ChangeSum)//当发生用户充值的时候
{
	if (m_TableBankerUserID != 0 && m_TableBankerUserID == dwUserID)
	{
		Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
		if (!pBankerRole)
			return;

		MG_Cmd_CarBankerUserGlobelChange msg;
		SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarBankerUserGlobelChange), sizeof(MG_Cmd_CarBankerUserGlobelChange));
		msg.dwBankerGlobelSum = pBankerRole->GetGlobelSum();
		msg.dwBankerUserID = dwUserID;

		HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.begin();
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
		HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
		if (Iter == m_TableUserMap.end())
			return;
		if (!Iter->second.IsInVipSeat)
			return;
		//如果玩家在VIP位置上的时候 
		MG_Cmd_CarVipGlobelChange msg;
		SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarVipGlobelChange), sizeof(MG_Cmd_CarVipGlobelChange));
		msg.VipUserID = dwUserID;
		msg.VipGlobelSum = pRole->GetGlobelSum();
		HashMap<DWORD, tagCarRoleInfo>::iterator IterRole = m_TableUserMap.begin();
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
void CarManager::OnClearTableInfo()
{
	//每局开始下注之前 先清理下数据
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
	{
		m_TableBetAreaGlobel[i] = 0;
	}
}
void CarManager::OnWriteBankerChange()
{
	MG_Cmd_CarBankerListChange msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarBankerListChange), sizeof(MG_Cmd_CarBankerListChange));
	//SendDataToAllTableRole(&msg);
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void CarManager::OnBankerUserChange()
{
	MG_Cmd_CarBankerUserChange msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarBankerUserChange), sizeof(MG_Cmd_CarBankerUserChange));
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
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void CarManager::UpdateTableIsCanBegin(DWORD dwTimer)
{
	if (m_TableStates == CTS_End)
	{
		//桌子当前状态为 开牌阶段
		if (m_TableStatesLog != 0 && dwTimer - m_TableStatesLog < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.EndWriteSec * 1000)
			return;
		//进入开牌阶段已经满30秒了
		if (m_TableUserMap.empty())
		{
			SetTableStates(CTS_WriteBegin);//桌子进入等待开始阶段 因为没人 无须继续处理
			OnClearTableInfo();
		}
		else
		{
			OnTableJoinBegin();
		}
		return;
	}
	else if (m_TableStates == CTS_WriteBegin)
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
void CarManager::UpdateTableIsCanEnd(DWORD dwTimer)
{
	if (m_TableStates == CTS_Begin)
	{
		if (dwTimer - m_TableStatesLog < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.BeginWriteSec * 1000)
			return;
		//进入下注阶段30秒满了 可以进入开牌阶段了 
		OnTableJoinEnd();
		SetTableStates(CTS_End);//桌子进入开牌阶段
		return;
	}
	else
	{
		return;//其他阶段不做处理
	}
}
void CarManager::UpdateTableInfo(DWORD dwTimer)
{
	if (m_TableStates != CTS_Begin)
		return;
	static DWORD CarUpdateTableInfo = 0;
	if (CarUpdateTableInfo == 0 || dwTimer - CarUpdateTableInfo >= g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.UpdateSec * 1000)
	{
		CarUpdateTableInfo = dwTimer;
		OnUpdateTableInfo();
	}
}
tagCarRoleInfo* CarManager::GetRoleInfo(DWORD dwUserID)
{
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
		return NULL;
	else
		return &Iter->second;
}
void CarManager::SetNextBankerUserInfo()
{
	//在游戏开始下注的时候 重新设置下庄家的数据
	bool BankerIsChange = false;
	if (m_IsNeedClearBankerInfo)
	{
		tagCarRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
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
			if (m_TableBankerUseGameSum >= g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.BankerGameSum)
			{
				tagCarRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
				if (pUserInfo)
				{
					pUserInfo->IsBankerUser = false;
				}

				m_TableBankerUserID = 0;
				m_TableBankerUseGameSum = 0;
				BankerIsChange = true;
			}
			else if (pBankerRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.JoinBankerGlobelSum)
			{
				tagCarRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
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
			tagCarRoleInfo* pUserInfo = GetRoleInfo(m_TableBankerUserID);
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
				if (!pDestRole || pDestRole->GetGlobelSum() <  g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.JoinBankerGlobelSum)
				{
					tagCarRoleInfo* pUserInfo = GetRoleInfo(*Iter);
					if (pUserInfo)
					{
						pUserInfo->IsInWriteBanakerVec = false;
					}

					Iter = m_TableWriteBankerVec.erase(Iter);
				}
				else
				{
					tagCarRoleInfo* pUserInfo = GetRoleInfo(*Iter);
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
void CarManager::CreateNewCar()
{
	m_TableResult = 0;
	m_TableResultIndex = 0;
}
BYTE CarManager::GetTableResult(BYTE& Index)//随机产生一个结果  
{
	Index = 0;
	DWORD Value = RandUInt() % g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.MaxRate;
	vector<tagCarValue>::iterator Iter = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.CarValueVec.begin();
	for (; Iter != g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.CarValueVec.end(); ++Iter)
	{
		if (Value >= Iter->MinRate && Value < Iter->MaxRate)
		{
			Index = Iter->ID;
			return Iter->TypeID;
		}
	}
	return 0;
}
void CarManager::OnTableJoinBegin()
{
	//当桌子进入下注阶段的时候
	//1.判断庄家是否需要进行变化
	SetNextBankerUserInfo();
	//2.清空桌子上的数据
	OnClearTableInfo();
	//3.通知玩家 进入新的阶段了
	SetTableStates(CTS_Begin);//桌子进入下注阶段
	//4.生成轮盘数据
	CreateNewCar();
	//5.
	MG_Cmd_CarTableJoinBegin msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarTableJoinBegin), sizeof(MG_Cmd_CarTableJoinBegin));
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
	m_robotmanager.StartGame();
}
void CarManager::OnUpdateTableInfo()
{
	//更新桌子上的数据
	MG_Cmd_CarTableUpdate msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarTableUpdate), sizeof(MG_Cmd_CarTableUpdate));
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
		msg.TableBetAreaGlobel[i] = m_TableBetAreaGlobel[i];
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(Iter->first);
		if (!pRole)
			continue;
		msg.dwUserID = Iter->first;
		pRole->SendDataToGameServer(&msg);
	}
}
INT64 CarManager::GetBrandUserGlobelChange()
{
	INT64 GlobelChange = 0;
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
	{
		if (GetResultID() == i)
		{
			GlobelChange -= (GetResultRate() * m_TableBetAreaGlobel[i]);
		}
		else
		{
			GlobelChange += m_TableBetAreaGlobel[i];
		}
	}
	return GlobelChange;
}
INT64 CarManager::GetSystemGlobelChange()
{
	bool IsSystemBranker = false;
	if (m_TableBankerUserID == 0)//庄家为系统的话 或者 庄家为机器人的时候
	{
		IsSystemBranker = true;
	}
	else
	{
		HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(m_TableBankerUserID);
		if (Iter != m_TableUserMap.end() && Iter->second.IsRobot)
		{
			IsSystemBranker = true;
		}
	}

	INT64 GlobelChange = 0;
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.begin();
	for (; Iter != m_TableUserMap.end(); ++Iter)
	{
		for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
		{
			if (Iter->second.BetGlobel[i] == 0)
				continue;
			if (i == GetResultID())
			{
				if (Iter->second.IsRobot)
				{
					if (!IsSystemBranker)
						GlobelChange += (Iter->second.BetGlobel[i] * GetResultRate());
				}
				else
				{
					if (IsSystemBranker)
						GlobelChange -= (Iter->second.BetGlobel[i] * GetResultRate());
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
INT64 CarManager::ChangeResultBySystem()
{
	//1.重新随机系统的牌型 直到系统不亏钱
	BYTE Index = 0;
	INT64 SystemGlobel = 0;
	do
	{
		//随机参数一个数据
		m_TableResult = GetTableResult(m_TableResultIndex);
		SystemGlobel = GetSystemGlobelChange();
		++Index;
	} while (SystemGlobel < 0 && Index < 200);//必须在200次内
	return SystemGlobel;
}
void CarManager::OnTableJoinEnd()
{
	//当比赛开牌的时候 
	//进行结算的时候 
	UINT64 TotalAllAddGlobel = 0;
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
		TotalAllAddGlobel += m_TableBetAreaGlobel[i];


	//根据概率 生成一个 结果
	m_TableResult = GetTableResult(m_TableResultIndex);
	//控制优先
	INT64 SystemGlobel = GetSystemGlobelChange();
	INT64 BrandUserGlobel = GetBrandUserGlobelChange();

	if (g_FishServer.GetCarControl().NeedControl())
	{
		//m_TableResult=
		for (int i = 0; i < 20; i++)
		{
			BYTE byPos = rand() % g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.CarValueVec.size();
			tagCarValue pItem = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.CarValueVec[byPos];
			if (pItem.TypeID == g_FishServer.GetCarControl().GetSuitResult())
			{
				m_TableResult = pItem.TypeID;
				m_TableResultIndex = byPos;
				break;
			}
		}	
		SystemGlobel = GetSystemGlobelChange();
		BrandUserGlobel = GetBrandUserGlobelChange();
		g_FishServer.GetCarControl().CompleteControl();
	}
	else
	{

		if (m_SystemGlobelSum < 0 || (m_SystemGlobelSum + SystemGlobel) < 0)
		{
			SystemGlobel = ChangeResultBySystem();
			BrandUserGlobel = GetBrandUserGlobelChange();//根据结果计算庄家金币的变化
		}
	}

	//3.干涉完毕后 我们进行处理
	MG_Cmd_CarTableJoinEnd msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarTableJoinEnd), sizeof(MG_Cmd_CarTableJoinEnd));
	for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
		msg.TableBetAreaGlobel[i] = m_TableBetAreaGlobel[i];
	msg.TableResultIndex = m_TableResultIndex;
	//m_CarResultLog.SetResult(m_TableAreaData[m_TableResult]);//根据当局的结果设置数据

	//设置结构
	m_CarResultLog.SetResult(m_TableResult);

	msg.TableGameSum = m_CarResultLog.LogSum;
	msg.TableResultLog = m_CarResultLog.ResultLog;
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
		for (BYTE i = 0; i < MAX_CARSHOWBRAND_Sum; ++i)
		{
			msg.BankerListGlobelInfo[i].dwUserID = 0;
			msg.BankerListGlobelInfo[i].dwGlobelSum = 0;
		}
	}
	else
	{
		vector<DWORD>::iterator Iter = m_TableWriteBankerVec.begin();
		for (BYTE i = 0; Iter != m_TableWriteBankerVec.end() && i < MAX_CARSHOWBRAND_Sum;)
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
	for (BYTE i = 0; i < MAX_CARVIPSEAT_Sum; ++i)
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


	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.begin();
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
			for (BYTE i = 0; i < MAX_CAR_ClientSum; ++i)
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
				if (m_VipSeatVec[Iter->second.VipSeat].NonBetGlobelSum >= g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.VipNonBetSum)
				{
					if ((pRole->GetRoleInfo().ParticularStates & ParticularState_CarVip) != 0)
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
		m_SystemGlobelSum = static_cast<INT64>(m_SystemGlobelSum * g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.GetChangeGlobelRateValue());

	if (m_TableBankerUserID != 0)
	{
		if (BrandUserGlobel >= 0)
			g_DBLogManager.LogToDB(m_TableBankerUserID, LT_Car, 1, static_cast<DWORD>(BrandUserGlobel), TEXT("玩家坐庄宝马奔驰获得金币"), SendLogDB);
		else
			g_DBLogManager.LogToDB(m_TableBankerUserID, LT_Car, 0, static_cast<DWORD>(BrandUserGlobel*-1), TEXT("玩家坐庄宝马奔驰失去金币"), SendLogDB);
	}
	//存储到数据库里面去
	g_DBLogManager.LogCarInfoToDB(m_TableBankerUserID, m_TableBetAreaGlobel, m_TableResult, SystemGlobel, m_SystemGlobelSum, m_TableUserMap.size(), SendLogDB);
	//判断当前在排队列表里的玩家 
	MG_Cmd_CarWriteBankerUserGlobelMsg msgSend;
	SetMsgInfo(msgSend, GetMsgType(Main_Car, MG_CarWriteBankerUserGlobelMsg), sizeof(MG_Cmd_CarWriteBankerUserGlobelMsg));
	vector<DWORD>::iterator IterVec = m_TableWriteBankerVec.begin();
	for (; IterVec != m_TableWriteBankerVec.end(); ++IterVec)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(*IterVec);
		if (!pRole)
			continue;
		if (pRole->GetGlobelSum() < g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.JoinBankerGlobelSum)
		{
			msgSend.dwUserID = *IterVec;
			pRole->SendDataToGameServer(&msgSend);
		}
	}
	m_robotmanager.EndGame();
}
SHORT CarManager::GetBrandRateValue(BYTE Index)
{
	if (Index >= MAX_CAR_ClientSum)
		return 1;
	if (GetResultID() == Index)
	{
		//比庄家大的时候
		BYTE Rate = GetResultRate();
		return static_cast<SHORT>(Rate);
	}
	else
	{
		return 0;
	}
}
void CarManager::OnRoleJoinVipSeat(DWORD dwUserID, BYTE VipSeatIndex)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
	if (Iter == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}
	MG_Cmd_CarRoleJoinVipSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarRoleJoinVipSeat), sizeof(MG_Cmd_CarRoleJoinVipSeat));
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
	if (VipSeatIndex >= MAX_CARVIPSEAT_Sum)
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
				MG_Cmd_CarRoleBeLeaveVipSeat msgLeave;
				SetMsgInfo(msgLeave, GetMsgType(Main_Car, MG_CarRoleBeLeaveVipSeat), sizeof(MG_Cmd_CarRoleBeLeaveVipSeat));
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
	HashMap<DWORD, tagCarRoleInfo>::iterator IterSend = m_TableUserMap.begin();//Vip席位的改变发送给全部玩家
	for (; IterSend != m_TableUserMap.end(); ++IterSend)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterSend->first);
		if (!pRole)
			continue;
		msg.dwUserID = IterSend->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void CarManager::OnRoleLeaveVipSeat(DWORD dwUserID)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.find(dwUserID);
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

	MG_Cmd_CarRoleLeaveVipSeat msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarRoleLeaveVipSeat), sizeof(MG_Cmd_CarRoleLeaveVipSeat));
	msg.dwDestUserID = dwUserID;
	msg.VipSeatIndex = Iter->second.VipSeat;
	HashMap<DWORD, tagCarRoleInfo>::iterator IterSend = m_TableUserMap.begin();
	for (; IterSend != m_TableUserMap.end(); ++IterSend)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(IterSend->first);
		if (!pRole)
			continue;
		msg.dwUserID = IterSend->first;
		pRole->SendDataToGameServer(&msg);
	}
}
void CarManager::OnRoleGetNormalInfoByPage(DWORD dwUserID, DWORD Page)
{
	Role* pRole = g_FishServer.GetRoleManager().QueryRole(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagCarRoleInfo>::iterator IterRole = m_TableUserMap.find(dwUserID);
	if (IterRole == m_TableUserMap.end())
	{
		ASSERT(false);
		return;
	}

	DWORD BeginIndex = Page * MAX_CARNORMAL_PAGESUM;
	DWORD EndIndex = (Page + 1)* MAX_CARNORMAL_PAGESUM - 1;


	MG_Cmd_CarGetNormalSeatInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_Car, MG_CarGetNormalSeatInfo), sizeof(MG_Cmd_CarGetNormalSeatInfo));
	msg.dwUserID = dwUserID;
	msg.Page = Page;
	msg.TotalRoleSum = m_TableUserMap.size();

	for (BYTE i = 0; i < MAX_CARVIPSEAT_Sum; ++i)
	{
		if (m_VipSeatVec[i].dwUserID != 0)
			--msg.TotalRoleSum;
	}
	if (m_TableBankerUserID != 0)
		--msg.TotalRoleSum;

	for (BYTE i = 0; i < MAX_CARNORMAL_PAGESUM; ++i)
	{
		msg.Array[i].dwFaceID = 0;
		msg.Array[i].dwUserID = 0;
		msg.Array[i].GlobelSum = 0;
		msg.Array[i].NickName[0] = 0;
	}
	HashMap<DWORD, tagCarRoleInfo>::iterator Iter = m_TableUserMap.begin();
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
BYTE CarManager::GetResultID()
{
	//下注区 从 0开始 到 7  一共8个区域
	//获取下注区的类型ID
	return m_TableResult;
}
BYTE CarManager::GetResultRate()
{
	//根据结果 获取倍率
	HashMap<BYTE, tagCarType>::iterator Iter =  g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.CarTypeMap.find(GetResultID());
	if (Iter == g_FishServer.GetFishConfig().GetFishMiNiGameConfig().carConfig.CarTypeMap.end())
	{
		return 0;
	}
	else
	{
		return  Iter->second.RateValue;
	}
}


DWORD CarManager::BankerId()
{
	return m_TableBankerUserID;
}

WORD CarManager::ApplyListLength()
{
	return m_TableWriteBankerVec.size();
}

DWORD CarManager::BankerGold()
{
	Role* pBankerRole = g_FishServer.GetRoleManager().QueryRole(m_TableBankerUserID);
	if (pBankerRole)
	{
		return pBankerRole->GetGlobelSum();
	}
	return 0;
}
bool	CarManager::IsGameEnd()
{
	return m_TableStates == CTS_End;
}

bool	CarManager::HaveApply(DWORD dwUserid)
{
	return find(m_TableWriteBankerVec.begin(), m_TableWriteBankerVec.end(), dwUserid) != m_TableWriteBankerVec.end();
}

void CarManager::QueryNames(DWORD ClientID)
{
	int nNamelength = (MAX_NICKNAME + 1);
	int nSize = sizeof(LC_CMD_CarAdmin_PlayerNames)+sizeof(TCHAR)*nNamelength*(m_TableUserMap.size() - m_robotmanager.RobotCount());
	LC_CMD_CarAdmin_PlayerNames *pResult = (LC_CMD_CarAdmin_PlayerNames*)malloc(nSize);
	pResult->SetCmdSize(nSize);
	pResult->SetCmdType((Main_Control << 8) | LC_CarAdminQueryName);
	pResult->ClientID = ClientID;
	int nIndex = 0;
	for (HashMap<DWORD, tagCarRoleInfo>::iterator it = m_TableUserMap.begin(); it != m_TableUserMap.end(); it++)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(it->first);
		if (pRole&&!m_robotmanager.IsRobot(pRole->GetRoleID()))
		{
			TCHARCopy(&pResult->names[1 + nIndex*nNamelength], nNamelength, pRole->GetNickName(), MAX_NICKNAME);
			nIndex++;
		}
	}
	g_FishServer.SendNetCmdToControl(pResult);
	free(pResult);
}

void CarManager::QueryAllJetton(DWORD ClientID)
{
	LC_CMD_CarAdmin_AllJetton result;
	ZeroMemory(&result, sizeof(result));
	result.SetCmdSize(sizeof(result));
	result.SetCmdType((Main_Control << 8) | LC_CarAdminQueryAllJetton);
	result.ClientID = ClientID;
	result.nStock = m_SystemGlobelSum;
	CalPlayerJetton(result.nJetton);
	g_FishServer.SendNetCmdToControl(&result);

}

void CarManager::QueryPlayerJetton(TCHAR *pNickame, DWORD ClientID)
{
	for (HashMap<DWORD, tagCarRoleInfo>::iterator it = m_TableUserMap.begin(); it != m_TableUserMap.end(); it++)
	{
		Role* pRole = g_FishServer.GetRoleManager().QueryRole(it->first);
		if (pRole)
		{
			if (_tcscmp(pNickame, pRole->GetNickName()) == 0)
			{
				//HashMap<DWORD, tagDialRoleInfo>::iterator itJetton = m_TableUserMap.find(pRole->GetRoleID());
				//if (itJetton != m_TableUserMap.end())
				{
					LC_CMD_CarAdmin_PlayerJetton result;
					result.SetCmdSize(sizeof(result));
					result.SetCmdType((Main_Control << 8) | LC_CarAdminQueryPlayerJetton);
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

void CarManager::CalPlayerJetton(UINT64 uJetton[MAX_CAR_ClientSum])
{
	for (HashMap<DWORD, tagCarRoleInfo>::iterator it = m_TableUserMap.begin(); it != m_TableUserMap.end(); it++)
	{
		if (!m_robotmanager.IsRobot(it->first))
		{
			for (int i = 0; i < MAX_CAR_ClientSum; i++)
			{
				uJetton[i] += it->second.BetGlobel[i];
			}
		}
	}
}