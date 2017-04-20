#include "stdafx.h"
#include "GameRobot.h"
#include "FishServer.h"
#include "FishLogic\NetCmd.h"
GameRobotManager::GameRobotManager()
{
	m_RobotMap.clear();
	m_FreeRobot.clear();
	m_MonthSignMap.clear();
	m_WriteList.clear();
	m_IsLoadFinish = false;
}
GameRobotManager::~GameRobotManager()
{
	HashMap<DWORD, GameRobot*>::iterator IterMap = m_RobotMap.begin();
	for (; IterMap != m_RobotMap.end(); ++IterMap)
	{
		delete IterMap->second;
	}
	m_RobotMap.clear();
	vector<GameRobot*>::iterator IterVec = m_FreeRobot.begin();
	for (; IterVec != m_FreeRobot.end(); ++IterVec)
	{
		delete *IterVec;
	}
	m_FreeRobot.clear();
	m_MonthSignMap.clear();
	m_WriteList.clear();
}
void GameRobotManager::OnLoadAllGameRobot(DWORD BeginUserID, DWORD EndUserID)
{
	m_FreeRobot.clear();
	m_RobotMap.clear();
	if (BeginUserID == 0 || EndUserID == 0)
		return;//无须机器人
	//机器人的管理器 管理当前GameServer上全部的机器人 
	//初始化  需要知道机器人的容器 我们创建大量的机器人 进行处理 从数据库读取出来  
	DBR_Cmd_GetAccountInfoByUserID msg;
	SetMsgInfo(msg, DBR_GetAccountInfoByUserID, sizeof(DBR_Cmd_GetAccountInfoByUserID));
	msg.ClientID = 0;
	msg.LogonByGameServer = false;
	for (size_t i = BeginUserID; i <= EndUserID; ++i)
	{
		//我们让玩家上线
		msg.dwUserID = i;
		g_FishServer.SendNetCmdToDB(&msg);//发送命令
	}
	m_RobotConfigSum = EndUserID - BeginUserID + 1;
}
void GameRobotManager::OnAddRobot(DWORD dwUserID)
{
	//机器人加载完毕
	CRoleEx* pRole = g_FishServer.GetRoleManager()->QueryUser(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	GameRobot* pRobot = new GameRobot(this,pRole);
	if (!pRobot)
	{
		ASSERT(false);
		return;
	}
	m_FreeRobot.push_back(pRobot);
	if (m_FreeRobot.size() >= m_RobotConfigSum*0.8 && !m_IsLoadFinish)
	{
		//告诉GameServer加载完毕了
		//g_FishServer.ShowInfoToWin("机器人加载大致完成");
		m_IsLoadFinish = true;
		OnHandleAllMonthStates();
		g_FishServer.OnLoadFinish();
	}
	return;
}
GameRobot* GameRobotManager::GetFreeRobot(DWORD RobotID)
{
	//获取一个空闲的机器人
	if (m_FreeRobot.empty())
		return NULL;
	size_t Index = RandUInt() % m_FreeRobot.size();
	GameRobot* pRobot = m_FreeRobot[Index];
	if (!pRobot)
		return NULL;

	HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(RobotID);
	if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
		return NULL;
	{
		//最大金币数量修改
		if (pRobot->GetRoleInfo()->GetRoleInfo().dwGlobeNum > Iter->second.InitGlobelSum)
		{
			DWORD MaxGlobel = static_cast<DWORD>(((100 + (RandUInt() % (Iter->second.GlobelRateF * 2)) - Iter->second.GlobelRateF) / 100.0) * Iter->second.InitGlobelSum);
			pRobot->GetRoleInfo()->ChangeRoleGlobe(MaxGlobel - pRobot->GetRoleInfo()->GetRoleInfo().dwGlobeNum);
		}
	}
	{

		if (pRobot->GetRoleInfo()->GetRoleInfo().dwCurrencyNum >  Iter->second.InitCurrceySum)
		{
			DWORD MaxCurrcey = static_cast<DWORD>(((100 + (RandUInt() % (Iter->second.CurrceyRateF * 2)) - Iter->second.CurrceyRateF) / 100.0) * Iter->second.InitCurrceySum);
			pRobot->GetRoleInfo()->ChangeRoleCurrency(MaxCurrcey - pRobot->GetRoleInfo()->GetRoleInfo().dwCurrencyNum, TEXT("机器人补充钻石"));
		}
	}

	{
		if (pRobot->GetRoleInfo()->GetRoleInfo().dwGlobeNum < Iter->second.AddGlobelSum)
		{
			DWORD AddGlobel = static_cast<DWORD>(((100 + (RandUInt() % (Iter->second.GlobelRateF * 2)) - Iter->second.GlobelRateF) / 100.0) * Iter->second.AddGlobelSum);
			pRobot->GetRoleInfo()->ChangeRoleGlobe(AddGlobel);
		}
	}

	{
		if (pRobot->GetRoleInfo()->GetRoleInfo().dwCurrencyNum < Iter->second.AddCurrceySum)
		{
			DWORD AddCurrcey = static_cast<DWORD>(((100 + (RandUInt() % (Iter->second.CurrceyRateF * 2)) - Iter->second.CurrceyRateF) / 100.0) * Iter->second.AddCurrceySum);
			pRobot->GetRoleInfo()->ChangeRoleCurrency(AddCurrcey, TEXT("机器人补充钻石"));
		}
	}


	m_FreeRobot[Index] = m_FreeRobot[m_FreeRobot.size() - 1];
	m_FreeRobot[m_FreeRobot.size() - 1] = pRobot;
	m_FreeRobot.pop_back();//移除
	pRobot->SetRobotUse(RobotID);//设置机器人已经使用了
	m_RobotMap.insert(HashMap<DWORD, GameRobot*>::value_type(pRobot->GetRobotUserID(), pRobot));
	//g_FishServer.ShowInfoToWin("申请新机器人 剩余机器人数量:%d", m_FreeRobot.size());
	return pRobot;
}
void GameRobotManager::ResetGameRobot(DWORD dwUserID)
{
	HashMap<DWORD, GameRobot*>::iterator Iter = m_RobotMap.find(dwUserID);
	if (Iter == m_RobotMap.end())
	{
		ASSERT(false);
		return;
	}
	GameRobot* pRobot = Iter->second;
	m_RobotMap.erase(Iter);
	pRobot->ResetRobot();
	m_FreeRobot.push_back(pRobot);
	//g_FishServer.ShowInfoToWin("归还机器人 剩余机器人数量:%d", m_FreeRobot.size());
	//g_FishServer.ShowInfoToWin("归还机器人");

	return;
}
void GameRobotManager::ResetGameRobot(GameRobot* pRobot)
{
	if (!pRobot)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, GameRobot*>::iterator Iter = m_RobotMap.find(pRobot->GetRobotUserID());
	if (Iter == m_RobotMap.end())
	{
		ASSERT(false);
		return;
	}
	m_RobotMap.erase(Iter);
	pRobot->ResetRobot();
	m_FreeRobot.push_back(pRobot);
	//g_FishServer.ShowInfoToWin("归还机器人 剩余机器人数量:%d", m_FreeRobot.size());
	//g_FishServer.ShowInfoToWin("归还机器人");
	return;
}
bool GameRobotManager::GameRobotIsCanJoinTable(GameTable* pTable)
{
	//判断机器人是否可以加入当前桌子
	if (!pTable)
	{
		ASSERT(false);
		return false;
	}
	if (pTable->GetTableMonthID() == 0)
	{
		//非比赛房间
		//当普通玩家进入一个已经存在的房间的时候 如果玩家人数 小于等于2人 并且 全部为普通玩家的时候 添加一个机器人进来
		/*if (pTable->GetRoleManager().GetRoleSum() > 2)
			return false;
		for (BYTE i = 0; i < pTable->GetRoleManager().GetMaxPlayerSum(); ++i)
		{
			CRole* pRole = pTable->GetRoleManager().GetRoleBySeatID(i);
			if (!pRole || !pRole->IsActionUser())
				continue;
			if (pRole->GetRoleExInfo()->IsRobot())
			{
				return false;
			}
		}*/

		//修改规则 最后一个位置 机器人不可以进入
		if (pTable->GetRoleManager().GetRoleSum() >= 3)
			return false;

		return true;
	}
	else
	{
		//比赛房间 一个房间最多2个机器人
		BYTE RobotSum = 0;
		for (BYTE i = 0; i < pTable->GetRoleManager().GetMaxPlayerSum(); ++i)
		{
			CRole* pRole = pTable->GetRoleManager().GetRoleBySeatID(i);
			if (!pRole || !pRole->IsActionUser())
				continue;
			if (pRole->GetRoleExInfo()->IsRobot())
				++RobotSum;
		}
		if (RobotSum >= 2)
			return false;
		else
			return true;
	}
}
void GameRobotManager::OnRoleCreateNormalRoom(GameTable* pTable)
{
	if (!pTable || pTable->GetTableMonthID() != 0)
		return;
	if (pTable->GetRoleManager().GetRoleSum() != 1)
		return;
	for (BYTE i = 0; i < pTable->GetRoleManager().GetMaxPlayerSum(); ++i)
	{
		CRole* pRole = pTable->GetRoleManager().GetRoleBySeatID(i);
		if (!pRole || !pRole->IsActionUser())
			continue;
		if (pRole->GetRoleExInfo()->IsRobot())
			return;
	}
	//创建一个机器人 加入到集合里面去
	/*DWORD AddSec = RandUInt() % 10000 + 5000;

	AdddWriteRobot(pTable->GetTableID(), timeGetTime() + AddSec);*/

	//桌子ID 和 比赛ID 我们生成机器人的类型ID
	DWORD Key = (pTable->GetTableTypeID() << 16) + pTable->GetTableMonthID();
	multimap<DWORD, DWORD>::iterator Iter= g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotIndexMap.find(Key);
	if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotIndexMap.end())
		return;
	
	DWORD RobotNum = (RandUInt() % 99999) % 3;
	for (DWORD i = 0; i < RobotNum; ++i)
	{
		GameRobot* pRobot = GetFreeRobot(Iter->second);//获取一个空闲的机器人
		if (!pRobot)
			return;
		CRoleEx* pRole = pRobot->GetRoleInfo();
		if (!pRole)
			return;
		if (!g_FishServer.GetTableManager()->OnPlayerJoinTable(pTable->GetTableID(), pRole))
			AddResetRobot(pRole->GetUserID());
	}
}
void GameRobotManager::OnRoleLeaveNormalRoom(GameTable* pTable)
{
	//当普通玩家离开一个房间的时候 判断如果房间里没有普通玩家了 全部的机器人离开
	if (!pTable || pTable->GetTableMonthID() != 0)
		return;
	BYTE RoleSum = 0;
	for (BYTE i = 0; i < pTable->GetRoleManager().GetMaxPlayerSum(); ++i)
	{
		CRole* pRole = pTable->GetRoleManager().GetRoleBySeatID(i);
		if (!pRole || !pRole->IsActionUser())
			continue;
		if (!pRole->GetRoleExInfo()->IsRobot())
			++RoleSum;
	}
	if (RoleSum == 0)
	{
		for (BYTE i = 0; i < pTable->GetRoleManager().GetMaxPlayerSum(); ++i)
		{
			CRole* pRole = pTable->GetRoleManager().GetRoleBySeatID(i);
			if (!pRole || !pRole->IsActionUser())
				continue;
			if (pRole->GetRoleExInfo()->IsRobot())
			{
				//归还机器人
				DWORD dwUserID = pRole->GetID();
				g_FishServer.GetTableManager()->OnPlayerLeaveTable(dwUserID);
				ResetGameRobot(dwUserID);//归还机器人
			}
		}
	}
}
void GameRobotManager::OnRoleJoinNormalRoom(GameTable* pTable)
{
	//当普通玩家进入一个已经存在的房间的时候 如果玩家人数 小于等于2人 并且 全部为普通玩家的时候 添加一个机器人进来
	if (!pTable || pTable->GetTableMonthID() != 0)
		return;
	if (pTable->GetRoleManager().GetRoleSum() > 2)
		return;
	BYTE RobotSum = 0;
	for (BYTE i = 0; i < pTable->GetRoleManager().GetMaxPlayerSum(); ++i)
	{
		CRole* pRole = pTable->GetRoleManager().GetRoleBySeatID(i);
		if (!pRole || !pRole->IsActionUser())
			continue;
		if (pRole->GetRoleExInfo()->IsRobot())
			++RobotSum;
	}
	if (RobotSum > 0)
		return;

	//DWORD AddSec = RandUInt() % 10000 + 5000;

	//AdddWriteRobot(pTable->GetTableID(), timeGetTime() + AddSec);

	DWORD Key = (pTable->GetTableTypeID() << 16) + pTable->GetTableMonthID();
	multimap<DWORD, DWORD>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotIndexMap.find(Key);
	if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotIndexMap.end())
		return;
	DWORD RobotID = Iter->second;

	DWORD RobotNum = (RandUInt() % 99999) % 3;
	for (DWORD i = 0; i < RobotNum; ++i)
	{
		GameRobot* pRobot = GetFreeRobot(RobotID);//获取一个空闲的机器人
		if (!pRobot)
			return;
		CRoleEx* pRole = pRobot->GetRoleInfo();
		if (!pRole)
			return;
		if (!g_FishServer.GetTableManager()->OnPlayerJoinTable(pTable->GetTableID(), pRole))
			AddResetRobot(pRole->GetUserID());
	}
}
void GameRobotManager::OnMonthBeginSign(BYTE MonthID)
{
	//g_FishServer.ShowInfoToWin("%d 比赛 开始报名", MonthID);
	//当比赛开始报名的时候
	//我们获取指定数量的机器人 去报名 
	HashMap<BYTE, tagMonthConfig>::iterator Iter = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(MonthID);
	if (Iter == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
		return;
	BYTE TableID = Iter->second.TableTypeID;
	DWORD Key = (TableID << 16) + MonthID;

	multimap<DWORD, DWORD>::_Pairii pair = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotIndexMap.equal_range(Key);
	multimap<DWORD, DWORD>::iterator IterFind = pair.first;
	for (; IterFind != pair.second; ++IterFind)
	{
		DWORD RobotID = IterFind->second;

		HashMap<DWORD, tagGameRobotConfig>::iterator IterRobot = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(RobotID);
		if (IterRobot == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
			return;
		vector<GameRobot*> pVec;
		for (DWORD i = 0; i < IterRobot->second.MonthRobotSum; ++i)
		{
			GameRobot* pRobot = GetFreeRobot(RobotID);
			if (!pRobot)
			{
				continue;
			}
			//报名比赛
			GC_Cmd_SignUpMonth msg;
			SetMsgInfo(msg, GetMsgType(Main_Month, GC_SignUpMonth), sizeof(GC_Cmd_SignUpMonth));
			msg.dwUserID = pRobot->GetRobotUserID();
			msg.MonthID = MonthID;
			pRobot->GetRoleInfo()->SendDataToCenter(&msg);
			//将机器人存储到集合里面去
			pVec.push_back(pRobot);
		}
		//将集合 和 MonthID 绑定起来
		m_MonthSignMap.insert(HashMap<BYTE, vector<GameRobot*>>::value_type(MonthID, pVec));
	}
}
void GameRobotManager::OnMonthEndSign(BYTE MonthID)
{
	//无须做操作
	//g_FishServer.ShowInfoToWin("%d 比赛 结束报名", MonthID);
}
void GameRobotManager::OnMonthBeginStar(BYTE MonthID)
{
	//g_FishServer.ShowInfoToWin("%d 比赛 开始比赛", MonthID);

	//让指定的机器人直接进入比赛 不进行报名处理
	//开始报名 结束比赛
	//获取指定数量的机器人去参加比赛
	HashMap<BYTE, vector<GameRobot*>>::iterator Iter= m_MonthSignMap.find(MonthID);
	if (Iter == m_MonthSignMap.end())
		return;
	HashMap<BYTE, tagMonthConfig>::iterator IterMonth = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(MonthID);
	if (IterMonth == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
		return;
	vector<GameRobot*>::iterator IterVec = Iter->second.begin();
	for (; IterVec != Iter->second.end(); ++IterVec)
	{
		CRoleEx* pRole = (*IterVec)->GetRoleInfo();
		if (!pRole)
			continue;

		GC_Cmd_JoinMonth msg;
		SetMsgInfo(msg, GetMsgType(Main_Month, GC_JoinMonth), sizeof(GC_Cmd_JoinMonth));
		msg.dwUserID = pRole->GetUserID();
		msg.TableID = 0xffff;//无须指定桌子
		msg.MonthID = MonthID;
		pRole->SendDataToCenter(&msg);
		//g_FishServer.GetTableManager()->OnPlayerJoinTable(IterMonth->second.TableTypeID, pRole, MonthID, true);
	}
}
void GameRobotManager::OnMonthEndStar(BYTE MonthID)
{	
	//将在比赛里面的机器人 全部回收回来
	//g_FishServer.ShowInfoToWin("%d 比赛 结束比赛", MonthID);
	HashMap<BYTE, vector<GameRobot*>>::iterator Iter = m_MonthSignMap.find(MonthID);
	if (Iter == m_MonthSignMap.end())
		return;
	
	vector<GameRobot*>::iterator IterVec = Iter->second.begin();
	for (; IterVec != Iter->second.end(); ++IterVec)
	{
		g_FishServer.GetTableManager()->OnPlayerLeaveTable((*IterVec)->GetRobotUserID());//玩家离开比赛桌子
		ResetGameRobot(*IterVec);//归还机器人
	}
	m_MonthSignMap.erase(Iter);//移除机器人
}
void GameRobotManager::Update()
{
	UpdateResetVec();
	UpdateWriteList();
	HashMap<DWORD, GameRobot*>::iterator Iter = m_RobotMap.begin();
	for (; Iter != m_RobotMap.end(); ++Iter)
	{
		Iter->second->Update();
	}
}
void GameRobotManager::AdddWriteRobot(WORD TableID, DWORD WriteTime)
{
	tagRobotWrite pInfo;
	pInfo.TableID = TableID;
	pInfo.TimeLog = WriteTime;
	m_WriteList.push_back(pInfo);
}
void GameRobotManager::UpdateWriteList()
{
	if (m_WriteList.empty())
		return;
	DWORD Time = timeGetTime();
	list<tagRobotWrite>::iterator Iter = m_WriteList.begin();
	for (; Iter != m_WriteList.end();)
	{
		if (Time > Iter->TimeLog)
		{
			GameTable* pTable = g_FishServer.GetTableManager()->GetTable(Iter->TableID);
			if (!pTable)
				continue;
			BYTE TableTypeID = pTable->GetTableTypeID();
			BYTE MonthID = pTable->GetTableMonthID();
			
			DWORD Key = (TableTypeID << 16) + MonthID;
			multimap<DWORD, DWORD>::iterator IterFind = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotIndexMap.find(Key);
			if (IterFind == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotIndexMap.end())
				return;
			DWORD RobotID = IterFind->second;

			GameRobot* pRobot = GetFreeRobot(RobotID);
			if (!pRobot)
			{
				++Iter;
				continue;
			}
			if(!g_FishServer.GetTableManager()->OnPlayerJoinTable(Iter->TableID, pRobot->GetRoleInfo()))
				AddResetRobot(pRobot->GetRobotUserID());

			Iter = m_WriteList.erase(Iter);
		}
		else
		{
			return;
		}
	}
}
void GameRobotManager::AddResetRobot(DWORD dwUserID)
{
	m_ResetUserVec.push_back(dwUserID);
}
void GameRobotManager::UpdateResetVec()
{
	if (m_ResetUserVec.empty())
		return;
	vector<DWORD>::iterator Iter = m_ResetUserVec.begin();
	for (; Iter != m_ResetUserVec.end(); ++Iter)
	{
		ResetGameRobot(*Iter);
	}
	m_ResetUserVec.clear();
}
void GameRobotManager::OnHandleMonthStates(BYTE MonthID, BYTE MonthStates)
{
	if (!m_IsLoadFinish)
	{
		HashMap<BYTE, BYTE>::iterator Iter= m_MonthStatesMap.find(MonthID);
		if (Iter == m_MonthStatesMap.end())
		{
			m_MonthStatesMap.insert(HashMap<BYTE, BYTE>::value_type(MonthID,MonthStates));
		}
		else
		{
			Iter->second = MonthStates;
		}
		return;
	}
	else
	{
		if (MonthStates == 1)
		{
			OnMonthBeginSign(MonthID);
		}
		else if (MonthStates == 2)
		{
			OnMonthEndSign(MonthID);
		}
		else if (MonthStates == 4)
		{
			OnMonthBeginStar(MonthID);
		}
		else if (MonthStates == 8)
		{
			OnMonthEndStar(MonthID);
		}
		else
		{
			//复合操作
			//已经加载完毕了 直接处理
			bool IsInSign = false;
			if ((MonthStates & 1) != 0)
				IsInSign = true;
			else  if ((MonthStates & 2) != 0)
				IsInSign = false;

			bool IsInStar = false;
			if ((MonthStates & 4) != 0)
				IsInStar = true;
			else  if ((MonthStates & 8) != 0)
				IsInStar = false;


			//开始报名 开始比赛
			//结束报名 结束比赛
			//结束报名 开始比赛
			//结束比赛 开始报名 
			if (IsInStar == false && IsInSign == true)
			{
				if (IsInStar)
					OnMonthBeginStar(MonthID);
				else
					OnMonthEndStar(MonthID);

				if (IsInSign)
					OnMonthBeginSign(MonthID);
				else
					OnMonthEndSign(MonthID);
			}
			else
			{
				if (IsInSign)
					OnMonthBeginSign(MonthID);
				else
					OnMonthEndSign(MonthID);

				if (IsInStar)
					OnMonthBeginStar(MonthID);
				else
					OnMonthEndStar(MonthID);
			}
		}
	}
}
void GameRobotManager::OnHandleAllMonthStates()
{
	if (m_MonthStatesMap.empty())
		return;
	HashMap<BYTE, BYTE>::iterator Iter = m_MonthStatesMap.begin();
	for (; Iter != m_MonthStatesMap.end(); ++Iter)
	{
		OnHandleMonthStates(Iter->first, Iter->second);
	}
	m_MonthStatesMap.clear();
}
void GameRobotManager::OnChangeCharmValue(DWORD SrcUserID, DWORD DestUserID, BYTE CharmIndexID, DWORD CharmItemSum, int ChangeCharmValue)
{
	HashMap<DWORD, GameRobot*>::iterator Iter = m_RobotMap.find(DestUserID);
	if (Iter == m_RobotMap.end())
		return;
	Iter->second->OnChangeCharmValue(SrcUserID, CharmIndexID, CharmItemSum, ChangeCharmValue);
}
//单个机器人处理
GameRobot::GameRobot(GameRobotManager* pManager, CRoleEx* pRole)
{
	if (!pRole || !m_pManager)
	{
		ASSERT(false);
		return;
	}
	m_pManager = pManager;
	m_pRole = pRole;
	m_RobotID = 0;
	m_IsUse = false;
	/*m_TableID = 0;
	m_TableTypeID = 0;
	m_MonthID = 0;
	m_IsInTable = false;*/
	//
	m_LeaveTableTimeLog = 0;
	m_RoomTimeLog = 0;
	//
	m_NowRate = 0xff;
	m_ChangeRateTimeLog = 0;
	m_RateTimeLog = 0;
	//
	m_UseSkillTimeLog = 0;
	m_SkillTimeLog = 0;
	//
	m_ChangeLauncherTimeLog = 0;
	m_LauncherTimeLog = 0;
	//
	m_OpenFireTimeLog = 0;
	//
	m_StopOpenFireTimeLog = 0;
	//
	m_RobotInfoTimeLog = 0;
	//
	m_IsOpenFire = false;
	m_RobotOpenFireTimeLog = 0;
	//
	m_RobotSendCharmItemTimeLog = 0;
	m_dwRobotSendCharmTimeLog = 0;
	m_vecSendCharmArray.clear();
	//
	m_dwSendCharmTineLog = 0;
	//
	m_RobotCharmEvent.Clear();
}
GameRobot::~GameRobot()
{

}
void GameRobot::SetRobotUse(DWORD RobotID)
{
	if (m_IsUse)
	{
		ASSERT(false);
		return;
	}
	m_RobotID = RobotID;
	m_IsUse = true;
	/*m_TableTypeID = static_cast<BYTE>(RobotID >> 16);
	m_MonthID = (BYTE)RobotID;
	m_TableID = 0;*/
}
void GameRobot::ResetRobot()
{
	if (!m_IsUse)
	{
		ASSERT(false);
		return;
	}
	m_RobotID = 0;
	m_IsUse = false;
	//
	m_LeaveTableTimeLog = 0;
	m_RoomTimeLog = 0;
	//
	m_NowRate = 0xff;
	m_ChangeRateTimeLog = 0;
	m_RateTimeLog = 0;
	//
	m_UseSkillTimeLog = 0;
	m_SkillTimeLog = 0;
	//
	m_ChangeLauncherTimeLog = 0;
	m_LauncherTimeLog = 0;
	//
	m_OpenFireTimeLog = 0;
	//
	m_StopOpenFireTimeLog = 0;
	//
	m_RobotInfoTimeLog = 0;
	//
	m_IsOpenFire = false;
	m_RobotOpenFireTimeLog = 0;

	//
	m_RobotSendCharmItemTimeLog = 0;
	m_dwRobotSendCharmTimeLog = 0;
	m_vecSendCharmArray.clear();
	//
	m_dwSendCharmTineLog = 0;
	//
	m_RobotCharmEvent.Clear();
}
DWORD GameRobot::GetRobotUserID()
{
	if (!m_pRole)
		return 0;
	return m_pRole->GetUserID();
}
CRoleEx* GameRobot::GetRoleInfo()
{
	return m_pRole;
}
void GameRobot::Update()
{
	UpdateRobotInfo();
	UpdateRobotRoom();
	UpdateRobotRate();
	UpdateRobotSkill();
	UpdateRobotLauncher();
	UpdateRobotOpenFire();
	UpdateRobotIsCanOpenFire();
	UpdateRobotCharm();
	UpdateRobotSendCharm();
}
void GameRobot::UpdateRobotRoom()
{
	if (m_RoomTimeLog != 0 && timeGetTime() - m_RoomTimeLog < 500)
		return;
	m_RoomTimeLog = timeGetTime();
	//更新机器人进出房间
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (m_pRole->GetRoleMonth().IsInMonthTable())//比赛无须进进出出
	{
		//如果玩家处于破产状态 并且 无法使用技能离开
		HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
		if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
			return;
		if (Iter->second.SkillInfo.IsOpen)
			return;
		if (m_pRole->GetRoleMonth().GetMonthInfo().dwMonthGlobel == 0 && !m_pRole->GetRoleMonth().IsCanAddMonthGlobel())
		{
			g_FishServer.GetTableManager()->OnPlayerLeaveTable(GetRobotUserID());//让机器人离开桌子
			m_pManager->AddResetRobot(GetRobotUserID());
			return;
		}
		return;
	}
	//m_IsInTable = false;
	CRole* pRole= g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());
	if (!pRole)//玩家不在桌子上
		return;
	HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
	if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
		return;
	if (!Iter->second.RoomInfo.IsOpen)//为开启的换 表示玩家一直在桌子里面
		return;
	if (m_LeaveTableTimeLog == 0)
	{
		DWORD JoinSec = RandUInt() % (Iter->second.RoomInfo.RoomInfo.OpenMaxSec - Iter->second.RoomInfo.RoomInfo.OpenMinSec) + Iter->second.RoomInfo.RoomInfo.OpenMinSec;//在房间待的秒数
		m_LeaveTableTimeLog = timeGetTime() + JoinSec * 1000;
		//m_TableID = pRole->GetTableID();
		//m_IsInTable = true;
	}
	if (timeGetTime() > m_LeaveTableTimeLog)
	{
		//机器人需要离开桌子了
		g_FishServer.GetTableManager()->OnPlayerLeaveTable(GetRobotUserID());//让机器人离开桌子
		//设置缓存
		DWORD LeaveSec = RandUInt() % (Iter->second.RoomInfo.RoomInfo.StopMaxSec - Iter->second.RoomInfo.RoomInfo.StopMinSec) + Iter->second.RoomInfo.RoomInfo.StopMinSec;
		m_pManager->AdddWriteRobot(pRole->GetTableID(), timeGetTime() + LeaveSec * 1000);
		//回收机器人
		//m_pManager->ResetGameRobot(this);//不可以直接移除 需要在Update里进行移除 操作
		m_pManager->AddResetRobot(GetRobotUserID());
		//
		//m_IsInTable = false;
	}
}
void GameRobot::UpdateRobotRate()
{
	if (m_RateTimeLog != 0 && timeGetTime() - m_RateTimeLog < 300)
		return;
	m_RateTimeLog = timeGetTime();
	//更新机器人切换倍率
	CRole* pUser = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());//玩家进入桌子 异步的
	if (!pUser /*|| m_pRole->GetRoleMonth().IsInMonthTable()*/)
		return;
	GameTable* pTable = g_FishServer.GetTableManager()->GetTable(pUser->GetTableID());
	if (!pTable)
		return;
	if (m_NowRate == 0xff)
	{
		if (m_ChangeRateTimeLog == 0)
		{
			HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
			if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
				return;

			DWORD RateF = RandUInt() % Iter->second.RateInfo.MaxRateF;
			vector<tagRobotRateType>::iterator IterVec = Iter->second.RateInfo.RateVec.begin();
			for (; IterVec != Iter->second.RateInfo.RateVec.end(); ++IterVec)
			{
				if (RateF < IterVec->RateF)
				{
					//切换到当前倍率
					//不可以一次切换到指定倍率 必须 一级一级的切 中间间隔
					m_NowRate = IterVec->RateType;
					break;
				}
			}
			HashMap<BYTE, TableInfo>::iterator IterTable = g_FishServer.GetFishConfig().GetTableConfig().m_TableConfig.find(pTable->GetTableTypeID());
			if (IterTable == g_FishServer.GetFishConfig().GetTableConfig().m_TableConfig.end())
				return;
			if (m_NowRate < IterTable->second.MinRate)
				m_NowRate = IterTable->second.MinRate;
			if (m_NowRate > IterTable->second.MaxRate)
				m_NowRate = IterTable->second.MaxRate;
			NetCmdChangeRateType msg;
			SetMsgInfo(msg, CMD_CHANGE_RATE_TYPE, sizeof(msg));
			msg.Seat = pUser->GetSeatID();
			msg.RateIndex = m_NowRate;
			g_FishServer.GetTableManager()->OnHandleTableMsg(m_pRole->GetUserID(), &msg);
			m_NowRate = 0xff;


			DWORD JoinSec = RandUInt() % (Iter->second.RateInfo.RateInfo.OpenMaxSec - Iter->second.RateInfo.RateInfo.OpenMinSec) + Iter->second.RateInfo.RateInfo.OpenMinSec;//切换倍率的CD
			m_ChangeRateTimeLog = timeGetTime() + JoinSec*1000;
		}
		else if (timeGetTime() >= m_ChangeRateTimeLog)
		{
			HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
			if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
				return;

			if (!Iter->second.RateInfo.IsOpen)
				return;

			DWORD RateF = RandUInt() % Iter->second.RateInfo.MaxRateF;
			vector<tagRobotRateType>::iterator IterVec = Iter->second.RateInfo.RateVec.begin();
			for (; IterVec != Iter->second.RateInfo.RateVec.end(); ++IterVec)
			{
				if (RateF < IterVec->RateF)
				{
					//切换到当前倍率
					//不可以一次切换到指定倍率 必须 一级一级的切 中间间隔
					m_NowRate = IterVec->RateType;
					break;
				}
			}

			//修改当前倍率
			HashMap<BYTE, TableInfo>::iterator IterTable = g_FishServer.GetFishConfig().GetTableConfig().m_TableConfig.find(pTable->GetTableTypeID());
			if (IterTable == g_FishServer.GetFishConfig().GetTableConfig().m_TableConfig.end())
				return;
			if (m_NowRate < IterTable->second.MinRate)
				m_NowRate = IterTable->second.MinRate;
			if (m_NowRate > IterTable->second.MaxRate)
				m_NowRate = IterTable->second.MaxRate;

			DWORD JoinSec = RandUInt() % (Iter->second.RateInfo.RateInfo.OpenMaxSec - Iter->second.RateInfo.RateInfo.OpenMinSec) + Iter->second.RateInfo.RateInfo.OpenMinSec;//切换倍率的CD
			m_ChangeRateTimeLog = timeGetTime() + JoinSec * 1000;
		}
	}
	else
	{
		if (pUser->GetRateIndex() == m_NowRate)
		{
			m_NowRate = 0xff;
			return;
		}
		//将玩家当前倍率 往 指定倍率进行切换
		BYTE RateIndex = (m_NowRate < pUser->GetRateIndex() ? pUser->GetRateIndex() - 1 : pUser->GetRateIndex() + 1);
		NetCmdChangeRateType msg;
		SetMsgInfo(msg, CMD_CHANGE_RATE_TYPE, sizeof(msg));
		msg.Seat = pUser->GetSeatID();
		msg.RateIndex = RateIndex;
		g_FishServer.GetTableManager()->OnHandleTableMsg(m_pRole->GetUserID(), &msg);
		return;
	}
}
void GameRobot::UpdateRobotSkill()
{
	if (m_SkillTimeLog != 0 && timeGetTime() - m_SkillTimeLog < 1000)
		return;
	m_SkillTimeLog = timeGetTime();
	//机器人使用技能 普通的
	CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());//玩家进入桌子 异步的
	if (!pRole)
		return;
	if (m_UseSkillTimeLog == 0)
	{
		HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
		if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
			return;
		DWORD JoinSec = RandUInt() % (Iter->second.SkillInfo.SkillInfo.OpenMaxSec - Iter->second.SkillInfo.SkillInfo.OpenMinSec) + Iter->second.SkillInfo.SkillInfo.OpenMinSec;//切换基恩能够的CD
		m_UseSkillTimeLog = timeGetTime() + JoinSec * 1000;//下次使用技能的CD
	}
	else if (timeGetTime() >= m_UseSkillTimeLog)
	{
		GameTable* pTable = g_FishServer.GetTableManager()->GetTable(pRole->GetTableID());
		if (!pTable)
			return;
		if (!m_pRole->GetRoleMonth().IsInMonthTable() && pTable->GetFishDesk()->GetFishSum() < 80)
			return;
		if (m_pRole->GetRoleMonth().IsInMonthTable() && pTable->GetFishDesk()->GetFishSum() < 20)
			return;

		HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
		if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
			return;

		if (!Iter->second.SkillInfo.IsOpen)
			return;

		//使用技能
		BYTE SkillID = g_FishServer.GetTableManager()->GetGameConfig()->CannonOwnSkill(pRole->GetLauncherType());

		NetCmdUseSkill msg;
		SetMsgInfo(msg, CMD_USE_SKILL, sizeof(msg));
		msg.SkillID = SkillID;
		g_FishServer.GetTableManager()->OnHandleTableMsg(m_pRole->GetUserID(), &msg);

		

		DWORD JoinSec = RandUInt() % (Iter->second.SkillInfo.SkillInfo.OpenMaxSec - Iter->second.SkillInfo.SkillInfo.OpenMinSec) + Iter->second.SkillInfo.SkillInfo.OpenMinSec;//切换基恩能够的CD
		m_UseSkillTimeLog = timeGetTime() + JoinSec * 1000;//下次使用技能的CD

		m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog,timeGetTime() + 3000);
	}
}
void GameRobot::UpdateRobotLauncher()
{
	if (m_LauncherTimeLog != 0 && timeGetTime() - m_LauncherTimeLog < 1000)
		return;
	m_LauncherTimeLog = timeGetTime();
	CRole* pUser = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());//玩家进入桌子 异步的
	if (!pUser)
		return;
	//切换炮的CD
	if (m_ChangeLauncherTimeLog == 0)
	{
		HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
		if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
			return;
		DWORD LauncherCDSec = RandUInt() % (Iter->second.LauncherInfo.LauncherInfo.OpenMaxSec - Iter->second.LauncherInfo.LauncherInfo.OpenMinSec) + Iter->second.LauncherInfo.LauncherInfo.OpenMinSec;
		m_ChangeLauncherTimeLog = timeGetTime() + LauncherCDSec * 1000;
		//玩家进入的随机产生一种可以使用的炮
		DWORD RateF = RandUInt() % Iter->second.LauncherInfo.MaxRateF;
		vector<tagRobotLauncherType>::iterator IterVec = Iter->second.LauncherInfo.LauncherVec.begin();
		for (; IterVec != Iter->second.LauncherInfo.LauncherVec.end(); ++IterVec)
		{
			if (RateF < IterVec->RateF)
			{
				BYTE LauncherType = IterVec->LauncherType;
				while (LauncherType >0 && !m_pRole->GetRoleLauncherManager().IsCanUserLauncherByID(LauncherType))
				{
					--LauncherType;
				}

				//切换到当前炮台
				NetCmdChangeLauncherType change;//切炮前2s 不可以发送子弹
				SetMsgInfo(change, CMD_CHANGE_LAUNCHER_TYPE, sizeof(change));
				change.Seat = pUser->GetSeatID();
				change.LauncherType = LauncherType;
				g_FishServer.GetTableManager()->OnHandleTableMsg(m_pRole->GetUserID(), &change);

				return;
			}
		}
	}
	else if (timeGetTime() >= m_ChangeLauncherTimeLog)
	{
		HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
		if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
			return;
		if (!Iter->second.LauncherInfo.IsOpen)
			return;
		//可以切换炮台了
		DWORD LauncherCDSec = RandUInt() % (Iter->second.LauncherInfo.LauncherInfo.OpenMaxSec - Iter->second.LauncherInfo.LauncherInfo.OpenMinSec) + Iter->second.LauncherInfo.LauncherInfo.OpenMinSec;
		m_ChangeLauncherTimeLog = timeGetTime() + LauncherCDSec * 1000;

		DWORD RateF = RandUInt() % Iter->second.LauncherInfo.MaxRateF;
		vector<tagRobotLauncherType>::iterator IterVec = Iter->second.LauncherInfo.LauncherVec.begin();
		for (; IterVec != Iter->second.LauncherInfo.LauncherVec.end(); ++IterVec)
		{
			if (RateF < IterVec->RateF)
			{
				BYTE LauncherType = IterVec->LauncherType;
				while (LauncherType >0 && !m_pRole->GetRoleLauncherManager().IsCanUserLauncherByID(LauncherType))
				{
					--LauncherType;
				}

				//切换到当前炮台
				NetCmdChangeLauncherType change;//切炮前2s 不可以发送子弹
				SetMsgInfo(change, CMD_CHANGE_LAUNCHER_TYPE, sizeof(change));
				change.Seat = pUser->GetSeatID();
				change.LauncherType = LauncherType;
				g_FishServer.GetTableManager()->OnHandleTableMsg(m_pRole->GetUserID(), &change);

				m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog, timeGetTime() + 1000);
				return;
			}
		}
	}
}
void GameRobot::UpdateRobotOpenFire()
{
	if (m_OpenFireTimeLog == 0 || timeGetTime() - m_OpenFireTimeLog > 300)
	{
		m_OpenFireTimeLog = timeGetTime();

		CRole* pUser = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());//玩家进入桌子 异步的
		if (!pUser)
			return;
		
		//判断玩家是否可以开炮
		if (m_NowRate != 0xff)
			return;
		if (!m_IsOpenFire)
			return;
		if (m_OpenFireTimeLog < m_StopOpenFireTimeLog)
			return;
		GameTable* pTable = g_FishServer.GetTableManager()->GetTable(pUser->GetTableID());
		if (!pTable)
			return; 
		m_Angle = pTable->GetFishDesk()->GetAngleByFish(m_LockFishID, pUser->GetSeatID());
		if (m_LockFishID == 0 && m_Angle == 0xffff)
			return;
		//玩家开炮
		if (!pUser->IsFullEnergy())//发射子弹
		{
			if (pUser->CheckFire(0))
			{
				//1.发射子弹
				NetCmdBullet msg;
				SetMsgInfo(msg, CMD_BULLET, sizeof(NetCmdBullet));
				msg.BulletID = 0;
				msg.Angle = m_Angle % 65535;
				msg.LockFishID = 0;
				msg.LauncherType = pUser->GetLauncherType();
				msg.Energy = (uint)pUser->GetEnergy();
				msg.ReboundCount = 0; //pUser->GetRoleExInfo()->GetRoleVip().GetLauncherReBoundNum();
				if (msg.LauncherType == 4)
					msg.ReboundCount = 0;
				//将命令设置到GameServer去
				g_FishServer.GetTableManager()->OnHandleTableMsg(m_pRole->GetUserID(), &msg);
			}
			else
			{
				//如果玩家在比赛模式
				if (m_NowRate == 0xff && m_pRole->GetRoleMonth().IsInMonthTable() && m_pRole->GetRoleMonth().GetMonthInfo().dwMonthGlobel > 0 && !m_pRole->GetRoleMonth().IsCanAddMonthGlobel())
				{
					//条件玩家的倍率 到 1倍率  从0开始
					m_NowRate = 0;
					HashMap<BYTE, TableInfo>::iterator IterTable = g_FishServer.GetFishConfig().GetTableConfig().m_TableConfig.find(pTable->GetTableTypeID());
					if (IterTable == g_FishServer.GetFishConfig().GetTableConfig().m_TableConfig.end())
						return;
					if (m_NowRate < IterTable->second.MinRate)
						m_NowRate = IterTable->second.MinRate;
					if (m_NowRate > IterTable->second.MaxRate)
						m_NowRate = IterTable->second.MaxRate;

					HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
					if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
						return;
					DWORD JoinSec = RandUInt() % (Iter->second.RateInfo.RateInfo.OpenMaxSec - Iter->second.RateInfo.RateInfo.OpenMinSec) + Iter->second.RateInfo.RateInfo.OpenMinSec;//切换倍率的CD
					m_ChangeRateTimeLog = timeGetTime() + JoinSec * 1000;
				}
			}
		}
		else//发射大招
		{
			NetCmdSkillLaser msgSkill;
			SetMsgInfo(msgSkill, CMD_SKILL_LASER_REQUEST, sizeof(msgSkill));
			msgSkill.Degree = m_Angle % 65535;
			g_FishServer.GetTableManager()->OnHandleTableMsg(m_pRole->GetUserID(), &msgSkill);

			m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog, timeGetTime() + 2000);
		}
	}
}
void GameRobot::UpdateRobotInfo()
{
	if (m_RobotInfoTimeLog != 0 && timeGetTime() - m_RobotInfoTimeLog < 1000)
		return;
	m_RobotInfoTimeLog = timeGetTime();

	CRole* pUser = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());//玩家进入桌子 异步的
	if (!pUser)
		return;

	//更新玩家的金币 钻石  或者 进行续币 等操作
	HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
	if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
		return;
	
	{
		if (m_pRole->GetRoleInfo().dwGlobeNum < Iter->second.AddGlobelSum)
		{
			DWORD AddGlobel = static_cast<DWORD>(((100 + (RandUInt() % (Iter->second.GlobelRateF * 2)) - Iter->second.GlobelRateF) / 100.0) * Iter->second.AddGlobelSum);
			m_pRole->ChangeRoleGlobe(AddGlobel);
			m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog, timeGetTime() + 2000);
		}
	}

	{
		if (m_pRole->GetRoleInfo().dwCurrencyNum < Iter->second.AddCurrceySum)
		{
			DWORD AddCurrcey = static_cast<DWORD>(((100 + (RandUInt() % (Iter->second.CurrceyRateF * 2)) - Iter->second.CurrceyRateF) / 100.0) * Iter->second.AddCurrceySum);
			m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog, timeGetTime() + 2000);
			m_pRole->ChangeRoleCurrency(AddCurrcey, TEXT("机器人补充钻石"));
		}
	}

	if (m_pRole->GetRoleInfo().wLevel > Iter->second.MaxLevel)
	{
		m_pRole->ChangeRoleLevel(static_cast<short>(Iter->second.MaxLevel - m_pRole->GetRoleInfo().wLevel));
		m_pRole->ChangeRoleExp(-1 * m_pRole->GetRoleInfo().dwExp,false);
	}
	if (m_pRole->GetRoleInfo().wLevel < Iter->second.MinLevel)
	{
		m_pRole->ChangeRoleLevel(static_cast<short>(m_pRole->GetRoleInfo().wLevel - Iter->second.MinLevel));
		m_pRole->ChangeRoleExp(-1 * m_pRole->GetRoleInfo().dwExp, false);
	}

	if (m_pRole->GetRoleMonth().IsInMonthTable())
	{
		if (m_pRole->GetRoleMonth().GetMonthInfo().dwMonthGlobel <= 100)
		{
			//玩家进行续币
			if (m_pRole->GetRoleMonth().IsCanAddMonthGlobel())
			{
				m_pRole->GetRoleMonth().OnRoleAddMonthGlobel();
				m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog, timeGetTime() + 1000);
			}
		}
	}
}
void GameRobot::UpdateRobotIsCanOpenFire()
{
	//更新机器人是否可以开火
	//机器人连续开炮后 需要停止一段时间
	if (timeGetTime() <= m_RobotOpenFireTimeLog)
		return;
	HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
	if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
		return;
	if (!Iter->second.OpenFireInfo.IsOpen)
	{
		m_IsOpenFire = true;
		return;
	}
	if (m_IsOpenFire)
	{
		m_IsOpenFire = false;
		//设置停火 等待下次开始
		DWORD StarSec = RandUInt() % (Iter->second.OpenFireInfo.FireInfo.StopMaxSec - Iter->second.OpenFireInfo.FireInfo.StopMinSec) + Iter->second.OpenFireInfo.FireInfo.StopMinSec;
		m_RobotOpenFireTimeLog = timeGetTime() + StarSec * 1000;
	}
	else
	{
		m_IsOpenFire = true;
		DWORD StarSec = RandUInt() % (Iter->second.OpenFireInfo.FireInfo.OpenMaxSec - Iter->second.OpenFireInfo.FireInfo.OpenMinSec) + Iter->second.OpenFireInfo.FireInfo.OpenMinSec;
		m_RobotOpenFireTimeLog = timeGetTime() + StarSec * 1000;
	}
}
void GameRobot::UpdateRobotCharm()
{
	//关系机器人使用魅力物品
	if (m_RobotSendCharmItemTimeLog != 0 && timeGetTime() - m_RobotSendCharmItemTimeLog < 2000)
		return;
	m_RobotSendCharmItemTimeLog = timeGetTime();

	HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
	if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
		return;
	if (!Iter->second.CharmInfo.IsOpen)
		return;
	if (m_dwRobotSendCharmTimeLog == 0)
	{
		m_dwRobotSendCharmTimeLog = timeGetTime() + (RandUInt() % (Iter->second.CharmInfo.SendCharmMaxCDSec - Iter->second.CharmInfo.SendCharmMinCDSec) + Iter->second.CharmInfo.SendCharmMinCDSec) * 1000;
		return;
	}
	if (timeGetTime() < m_dwRobotSendCharmTimeLog)
		return;

	BYTE RandF = static_cast<BYTE>(RandUInt() % 100);
	if (RandF < Iter->second.CharmInfo.NonRateF)
	{
		m_dwRobotSendCharmTimeLog = timeGetTime() + (RandUInt() % (Iter->second.CharmInfo.SendCharmMaxCDSec - Iter->second.CharmInfo.SendCharmMinCDSec) + Iter->second.CharmInfo.SendCharmMinCDSec) * 1000;
		return;
	}
	//每2秒更新一次
	//1.主动 处理 
	CRole* pUser = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());//玩家进入桌子 异步的
	if (!pUser /*|| m_pRole->GetRoleMonth().IsInMonthTable()*/)
		return;
	GameTable* pTable = g_FishServer.GetTableManager()->GetTable(pUser->GetTableID());
	if (!pTable)
		return;
	if (pTable->GetRoleManager().GetRoleSum() <= 1)
		return;
	BYTE SeatIndex = RandUInt() % (pTable->GetRoleManager().GetRoleSum() -1);
	DWORD dwUserID = 0;
	BYTE NowIndex = 0;
	for (BYTE i = 0; i <= SeatIndex;)
	{
		CRole* pRole = pTable->GetRoleManager().GetRoleBySeatID(i);
		if (!pRole || !pRole->IsActionUser())
		{
			++i;
			continue;
		}
		else if (pRole->GetID() == GetRobotUserID())
		{
			++i;
			++NowIndex;
			continue;
		}
		else
		{
			if (NowIndex == SeatIndex)
			{
				dwUserID = pRole->GetID();
				break;
			}
			++i;
			++NowIndex;
		}
	}
	if (dwUserID == 0)
		return;
	//随机从桌子里挑选一位玩家 随机参数一种魅力 物品 随机产生魅力物品的数量 我们进行记录到集合里面去	
	DWORD RateF = RandUInt() % Iter->second.CharmInfo.SendInfo.dwSendCharmArrayMaxRateF;
	std::vector<tagRobotSendCharmInfo>::iterator IterVec = Iter->second.CharmInfo.SendInfo.vecSendCharmArray.begin();
	for (; IterVec != Iter->second.CharmInfo.SendInfo.vecSendCharmArray.end(); ++IterVec)
	{
		if (RateF < IterVec->RateF)
		{
			BYTE ItemID = IterVec->CharmIndexID;
			WORD Sum = RandUInt() % (IterVec->MaxItemSum - IterVec->MinItemSum) + IterVec->MinItemSum;
			if (Sum > 0)
			{
				tagGameRobotCharm pEvent;
				pEvent.dwUserID = dwUserID;
				pEvent.CharmIndexID = ItemID;
				pEvent.CharmItemSum = Sum;
				pEvent.BeginTimeLog = timeGetTime() + 2000;
				m_vecSendCharmArray.push_back(pEvent);
			}
			//加入到发送集合里面去
			m_dwRobotSendCharmTimeLog = timeGetTime() + (RandUInt() % (Iter->second.CharmInfo.SendCharmMaxCDSec - Iter->second.CharmInfo.SendCharmMinCDSec) + Iter->second.CharmInfo.SendCharmMinCDSec) * 1000;
			break;
		}
	}
}
void GameRobot::UpdateRobotSendCharm()
{
	//1s更新一次
	if (m_dwSendCharmTineLog != 0 && timeGetTime() - m_dwSendCharmTineLog < 500)
		return;
	m_dwSendCharmTineLog = timeGetTime();
	if (m_vecSendCharmArray.empty())
		return;

	HashMap<DWORD, tagGameRobotConfig>::iterator IterRobot = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
	if (IterRobot == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
		return;
	if (!IterRobot->second.CharmInfo.IsOpen)
		return;

	list<tagGameRobotCharm>::iterator Iter = m_vecSendCharmArray.begin();
	for (; Iter != m_vecSendCharmArray.end();)
	{
		//向指定玩家发送魅力物品
		CRoleEx* pDestRole = g_FishServer.GetRoleManager()->QueryUser(Iter->dwUserID);
		if (!pDestRole)
		{
			Iter = m_vecSendCharmArray.erase(Iter);
			continue;
		}
		if (Iter->BeginTimeLog > m_dwSendCharmTineLog)
			return;
		if (Iter->CharmItemSum == 0)
		{
			Iter = m_vecSendCharmArray.erase(Iter);
			continue;
		}

		//扣除玩家金币 或者 钻石
		HashMap<BYTE, tagCharmOnce>::iterator IterCharm = g_FishServer.GetFishConfig().GetCharmConfig().CharmIndexMap.find(Iter->CharmIndexID);
		if (IterCharm == g_FishServer.GetFishConfig().GetCharmConfig().CharmIndexMap.end())
		{
			Iter = m_vecSendCharmArray.erase(Iter);
			continue;
		}
		if (!m_pRole->LostUserMoney(IterCharm->second.UserGlobel, IterCharm->second.UserMedal, IterCharm->second.UserCurrcey , TEXT("机器人赠送魅力物品 扣除货币")))
		{
			Iter = m_vecSendCharmArray.erase(Iter);
			continue;
		}
		pDestRole->ChangeRoleCharmValue(Iter->CharmIndexID,1);
		LC_Cmd_ChangeRoleCharmResult pResult;
		SetMsgInfo(pResult, GetMsgType(Main_Charm, LC_ChangeRoleCharmResult), sizeof(LC_Cmd_ChangeRoleCharmResult));
		pResult.Result = true;
		pResult.dwDestUserID = pDestRole->GetUserID();
		pResult.dwDestUserCharmValue = g_FishServer.GetFishConfig().GetCharmValue(pDestRole->GetRoleInfo().CharmArray);
		m_pRole->SendDataToClient(&pResult);
		LC_Cmd_TableRoleCharmInfo msgTable;
		SetMsgInfo(msgTable, GetMsgType(Main_Charm, LC_TableRoleCharmInfo), sizeof(LC_Cmd_TableRoleCharmInfo));
		msgTable.SrcUserID = m_pRole->GetUserID();
		msgTable.DestUserID = Iter->dwUserID;
		msgTable.CharmIndex = Iter->CharmIndexID;
		msgTable.ItemSum =1;
		m_pRole->SendDataToClient(&msgTable);
		m_pRole->SendDataToTable(&msgTable);


		Iter->CharmItemSum -= 1;
		if (Iter->CharmItemSum == 0)
			m_vecSendCharmArray.erase(Iter);

		m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog, timeGetTime() + 3000);
		return;
	}
}
void GameRobot::OnChangeCharmValue(DWORD SrcUserID, BYTE CharmIndexID, DWORD CharmItemSum, int ChangeCharmValue)
{
	HashMap<DWORD, tagGameRobotConfig>::iterator Iter = g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.find(m_RobotID);
	if (Iter == g_FishServer.GetFishConfig().GetFishGameRobotConfig().RobotMap.end())
		return;
	//1.触发被动效果
	HashMap<BYTE, tagRobotBeSendCharm>::iterator IterFind = Iter->second.CharmInfo.BeSendInfoMap.find(CharmIndexID);
	if (IterFind != Iter->second.CharmInfo.BeSendInfoMap.end())
	{
		BYTE NonRandF = static_cast<BYTE>(RandUInt() % 100);
		if (NonRandF >= IterFind->second.NonRateF)
		{
			DWORD RateF = RandUInt() % IterFind->second.SendInfo.dwSendCharmArrayMaxRateF;
			std::vector<tagRobotSendCharmInfo>::iterator IterVec = IterFind->second.SendInfo.vecSendCharmArray.begin();
			for (; IterVec != IterFind->second.SendInfo.vecSendCharmArray.end(); ++IterVec)
			{
				if (RateF < IterVec->RateF)
				{
					BYTE ItemID = IterVec->CharmIndexID;
					WORD Sum = RandUInt() % (IterVec->MaxItemSum - IterVec->MinItemSum) + IterVec->MinItemSum;
					//加入到发送集合里面去
					if (Sum > 0)
					{
						tagGameRobotCharm pEvent;
						pEvent.dwUserID = SrcUserID;
						pEvent.CharmIndexID = ItemID;
						pEvent.CharmItemSum = Sum;
						pEvent.BeginTimeLog = timeGetTime() + 2000;
						m_vecSendCharmArray.push_back(pEvent);
						m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog, timeGetTime() + 3000);
					}
					break;
				}
			}
		}
	}
	//2.将时间加入到集合里面去 判断是否需要触发事件
	int CharmValue = m_RobotCharmEvent.AddCharmEvent(SrcUserID, ChangeCharmValue, Iter->second.CharmInfo.BeChangeCharmSec * 1000);
	if (static_cast<DWORD>(abs(CharmValue)) >= Iter->second.CharmInfo.BeChangeCharmValue)
	{
		//触发事件
		m_RobotCharmEvent.Clear(SrcUserID);

		BYTE NonRandF = static_cast<BYTE>(RandUInt() % 100);
		if (NonRandF >= Iter->second.CharmInfo.CharmChangeEvent.NonRateF)
		{
			DWORD RateF = RandUInt() % 100;
			if (RateF < Iter->second.CharmInfo.CharmChangeEvent.LeaveRoomRateF && !m_pRole->GetRoleMonth().IsInMonthTable())
			{
				CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());
				if (!pRole)//玩家不在桌子上
					return;
				DWORD LeaveSec = RandUInt() % (Iter->second.RoomInfo.RoomInfo.StopMaxSec - Iter->second.RoomInfo.RoomInfo.StopMinSec) + Iter->second.RoomInfo.RoomInfo.StopMinSec;
				m_pManager->AdddWriteRobot(pRole->GetTableID(), timeGetTime() + LeaveSec * 1000);
				//机器人需要离开桌子了
				g_FishServer.GetTableManager()->OnPlayerLeaveTable(GetRobotUserID());//让机器人离开桌子
				//回收机器人
				m_pManager->AddResetRobot(GetRobotUserID());
				return;
			}
			else
			{
				if (CharmValue >=0)
				{
					//触发反击
					DWORD RateF = RandUInt() % Iter->second.CharmInfo.CharmChangeEvent.AddSendInfo.dwSendCharmArrayMaxRateF;
					std::vector<tagRobotSendCharmInfo>::iterator IterVec = Iter->second.CharmInfo.CharmChangeEvent.AddSendInfo.vecSendCharmArray.begin();
					for (; IterVec != Iter->second.CharmInfo.CharmChangeEvent.AddSendInfo.vecSendCharmArray.end(); ++IterVec)
					{
						if (RateF < IterVec->RateF)
						{
							BYTE ItemID = IterVec->CharmIndexID;
							WORD Sum = RandUInt() % (IterVec->MaxItemSum - IterVec->MinItemSum) + IterVec->MinItemSum;
							//加入到发送集合里面去
							if (Sum > 0)
							{
								tagGameRobotCharm pEvent;
								pEvent.dwUserID = SrcUserID;
								pEvent.CharmIndexID = ItemID;
								pEvent.CharmItemSum = Sum;
								pEvent.BeginTimeLog = timeGetTime() + 2000;
								m_vecSendCharmArray.push_back(pEvent);
								m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog, timeGetTime() + 3000);
							}
							break;
						}
					}
				}
				else
				{
					//触发反击
					DWORD RateF = RandUInt() % Iter->second.CharmInfo.CharmChangeEvent.DelSendInfo.dwSendCharmArrayMaxRateF;
					std::vector<tagRobotSendCharmInfo>::iterator IterVec = Iter->second.CharmInfo.CharmChangeEvent.DelSendInfo.vecSendCharmArray.begin();
					for (; IterVec != Iter->second.CharmInfo.CharmChangeEvent.DelSendInfo.vecSendCharmArray.end(); ++IterVec)
					{
						if (RateF < IterVec->RateF)
						{
							BYTE ItemID = IterVec->CharmIndexID;
							WORD Sum = RandUInt() % (IterVec->MaxItemSum - IterVec->MinItemSum) + IterVec->MinItemSum;
							//加入到发送集合里面去
							if (Sum > 0)
							{
								tagGameRobotCharm pEvent;
								pEvent.dwUserID = SrcUserID;
								pEvent.CharmIndexID = ItemID;
								pEvent.CharmItemSum = Sum;
								pEvent.BeginTimeLog = timeGetTime() + 2000;
								m_vecSendCharmArray.push_back(pEvent);
								m_StopOpenFireTimeLog = max(m_StopOpenFireTimeLog, timeGetTime() + 3000);
							}
							break;
						}
					}
				}
			}
		}
	}
}

