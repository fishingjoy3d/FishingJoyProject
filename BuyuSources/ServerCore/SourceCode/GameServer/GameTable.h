#pragma once
#include "Stdafx.h"
#include".\FishLogic\fishdesk.h"
#include "Role.h"
#include"FishTimer.h"
#include"SkillInfo.h"

const DWORD REPEATTIMER = (DWORD)0xFFFFFFFF;
const int GAME_TIME_SPACE = 5;//record game time;
const int MINUTE2SECOND = 60;

//桌子控制
class GameTable : public NetSendInterface
{
public:
	GameTable();
	virtual ~GameTable();

	virtual bool				OnInit(WORD TableID, BYTE TableTypeID,BYTE MonthID = 0);
	virtual void				OnDestroy();
	virtual bool				IsCanJoinTable(CRoleEx* pRoleEx, BYTE MonthID);
	virtual bool				OnRoleJoinTable(CRoleEx* pRoleEx,BYTE MonthID,bool IsSendToClient);
	virtual bool				OnRoleLeaveTable(DWORD dwUserID);
	virtual bool				OnHandleTableMsg(DWORD dwUserID,NetCmd* pCmd);
	virtual void				Update(bool bUpdateTime, bool bUpdateRp, bool bUpdateRpEffect);
	virtual void				OnGameStart();
	virtual void				OnGameStop();
	virtual WORD				GetTableID(){ return m_TableID; }
	virtual bool				IsFull();
	virtual WORD				GetTablePlayerSum();
	virtual BYTE				GetTableTypeID(){ return m_TableTypeID; }
	virtual BYTE				GetTableMonthID(){ return m_MonthID; }
	virtual CRole*              SearchUser(DWORD dwUserID);
	virtual void				SendDataToTable(DWORD dwUserID, NetCmd* pData);
	virtual void				SendDataToTableAllUser(NetCmd* pData);
	virtual void				SendRoleJoinInfo(DWORD dwUserID);
	virtual void				SendRoleMonthInfo(DWORD dwUserID);

	virtual void				Send(PlayerID RoleID, NetCmd*);
	virtual void				SendBySeat(byte seatID, NetCmd*);
	virtual bool				IsUseSeat()const;
	//桌子上的切换函数
	virtual bool				ChangeRoleRate(BYTE Rate);//玩家修改倍率
	virtual bool				IsCanJoinRoom(CRoleEx* pRole);
	//机器人
	virtual bool				IsCanAddRobot();
	//
	virtual CTableRoleManager&	GetRoleManager(){ return m_RoleManager; }

	FishDesk*					GetFishDesk(){ return &m_fishdesk; }

	void SendTableRoleInfoToClient(DWORD dwUserID);

private:
	DWORD						m_LastUpdateTime;
	WORD						m_TableID;
	BYTE						m_MonthID;//比赛的ID  0表示不是比赛的桌子
	CTableRoleManager			m_RoleManager;
	FishDesk					m_fishdesk;//具体的游戏类
	BYTE						m_TableTypeID;
};