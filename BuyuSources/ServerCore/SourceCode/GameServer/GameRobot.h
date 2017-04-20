//游戏机器人
#pragma once
#include "GameTable.h"
class GameRobotManager;

struct tagGameRobotCharm
{
	DWORD		dwUserID;
	BYTE		CharmIndexID;
	WORD		CharmItemSum;
	DWORD		BeginTimeLog;
};
struct tagGameRobotCharmEvent
{
	DWORD		SrcUserID;
	int			CharmValue;
	DWORD		TimeLog;
};
struct tagGameRobotCharmEventList
{
	std::list<tagGameRobotCharmEvent>	 EventList;
	int									 NowChangeCharmValue;
	tagGameRobotCharmEventList()
	{
		EventList.clear();
		NowChangeCharmValue = 0;
	}
	void Clear()
	{
		EventList.clear();
		NowChangeCharmValue = 0;
	}
	int AddCharmEvent(DWORD SrcUserID, int ChanrmValue,DWORD DiffTime)
	{
		DWORD NowTime = timeGetTime();
		std::list<tagGameRobotCharmEvent>::iterator Iter = EventList.begin();
		for (; Iter != EventList.end();)
		{
			if (NowTime - Iter->TimeLog >= DiffTime)
			{
				NowChangeCharmValue -= Iter->CharmValue;
				Iter = EventList.erase(Iter);
			}
			else
				++Iter;
		}
		tagGameRobotCharmEvent pEvent;
		pEvent.CharmValue = ChanrmValue;
		pEvent.SrcUserID = SrcUserID;
		pEvent.TimeLog = NowTime;
		EventList.push_back(pEvent);
		NowChangeCharmValue += pEvent.CharmValue;
		return NowChangeCharmValue;
	}
};
struct tagGameRobotCharmChangeManager
{
	HashMap<DWORD, tagGameRobotCharmEventList>	EventMap;
	void Clear()
	{
		EventMap.clear();
	}
	void Clear(DWORD dwUserID)
	{
		EventMap.erase(dwUserID);
	}
	int AddCharmEvent(DWORD SrcUserID, int ChanrmValue, DWORD DiffTime)
	{
		HashMap<DWORD, tagGameRobotCharmEventList>::iterator Iter = EventMap.find(SrcUserID);
		if (Iter == EventMap.end())
		{
			tagGameRobotCharmEventList pInfo;
			int Value=pInfo.AddCharmEvent(SrcUserID, ChanrmValue, DiffTime);
			EventMap.insert(HashMap<DWORD, tagGameRobotCharmEventList>::value_type(SrcUserID,pInfo));
			return Value;
		}
		else
		{
			return Iter->second.AddCharmEvent(SrcUserID, ChanrmValue, DiffTime);
		}
	}
};
class GameRobot
{
public:
	GameRobot(GameRobotManager* pManager,CRoleEx* pRole);
	virtual ~GameRobot();

	void Update();
	void SetRobotUse(DWORD RobotID);
	void ResetRobot();
	DWORD GetRobotUserID();
	CRoleEx* GetRoleInfo();

	void OnChangeCharmValue(DWORD SrcUserID, BYTE CharmIndexID, DWORD CharmItemSum, int ChangeCharmValue);
private:
	void UpdateRobotRoom();
	void UpdateRobotRate();
	void UpdateRobotSkill();
	void UpdateRobotLauncher();
	void UpdateRobotOpenFire();
	void UpdateRobotInfo();
	void UpdateRobotIsCanOpenFire();
	void UpdateRobotCharm();
	void UpdateRobotSendCharm();
private:
	GameRobotManager*   m_pManager;
	CRoleEx*			m_pRole;
	DWORD				m_RobotID;
	bool				m_IsUse;
	//1.进出房间的记录
	DWORD				m_RoomTimeLog;
	DWORD				m_LeaveTableTimeLog;
	//2.机器人切换倍率
	DWORD				m_RateTimeLog;
	BYTE				m_NowRate;
	DWORD				m_ChangeRateTimeLog;
	//3.机器人使用技能
	DWORD				m_SkillTimeLog;
	DWORD				m_UseSkillTimeLog;
	//4.机器人切换炮台
	DWORD				m_LauncherTimeLog;
	DWORD				m_ChangeLauncherTimeLog;
	//5.机器人开炮
	DWORD				m_OpenFireTimeLog;
	WORD				m_LockFishID;
	DWORD				m_Angle;
	//
	DWORD				m_StopOpenFireTimeLog;
	//
	DWORD				m_RobotInfoTimeLog;
	//
	DWORD				m_RobotOpenFireTimeLog;
	bool				m_IsOpenFire;

	//
	DWORD				m_RobotSendCharmItemTimeLog;
	DWORD				m_dwRobotSendCharmTimeLog;
	list<tagGameRobotCharm> m_vecSendCharmArray;//待发送的魅力集合
	tagGameRobotCharmChangeManager m_RobotCharmEvent;
	//
	DWORD				m_dwSendCharmTineLog;

};
struct tagRobotWrite
{
	WORD		TableID;
	DWORD		TimeLog;
};

class GameRobotManager//游戏机器人管理器
{
public:
	GameRobotManager();
	virtual ~GameRobotManager();

	void  OnLoadAllGameRobot(DWORD BeginUserID,DWORD EndUserID);//加载全部的机器人
	void  OnAddRobot(DWORD dwUserID);
	
	GameRobot* GetFreeRobot(DWORD RobotID);//获取一个空闲的机器人
	void ResetGameRobot(GameRobot* pRobot);//归还一个机器人
	void ResetGameRobot(DWORD dwUserID);
	//
	bool GameRobotIsCanJoinTable(GameTable* pTable);//判断机器人是否可以进入当前的桌子
	//
	void  OnRoleCreateNormalRoom(GameTable* pTable);//当普通玩家创建一个非比赛房间的时候
	void  OnRoleLeaveNormalRoom(GameTable* pTable);//当普通玩家离开一个非比赛房间的时候
	void  OnRoleJoinNormalRoom(GameTable* pTable);//当普通玩家进入一个非比赛房间的时候
	//
	void  OnHandleMonthStates(BYTE MonthID, BYTE MonthStates);
	//
	void  Update();
	//
	void  AdddWriteRobot(WORD TableID, DWORD WriteTime);
	void  UpdateWriteList();
	//
	void  AddResetRobot(DWORD dwUserID);
	void  UpdateResetVec();

	void OnChangeCharmValue(DWORD SrcUserID, DWORD DestUserID, BYTE CharmIndexID, DWORD CharmItemSum, int ChangeCharmValue);
private:
	void  OnHandleAllMonthStates();
	void  OnMonthBeginSign(BYTE MonthID);
	void  OnMonthEndSign(BYTE MonthID);
	void  OnMonthBeginStar(BYTE MonthID);
	void  OnMonthEndStar(BYTE MonthID);
private:
	DWORD						m_RobotConfigSum;
	bool						m_IsLoadFinish;
	HashMap<BYTE, BYTE>          m_MonthStatesMap;

	HashMap<DWORD, GameRobot*>	m_RobotMap;//已经使用的机器人
	vector<GameRobot*>			m_FreeRobot;//空闲机器人
	//
	HashMap<BYTE, vector<GameRobot*>> m_MonthSignMap;//比赛机器人的记录
	//tableID Time  WORD DWORD
	list<tagRobotWrite>			m_WriteList;
	//
	vector<DWORD>				m_ResetUserVec;
};