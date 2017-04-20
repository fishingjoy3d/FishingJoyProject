//当前游戏的核心类 
//一个桌子代表 几个玩家的一场游戏
//桌子在房间开始的时候就已经创建玩家 并且设置好了相关数据
#pragma once
#include "Stdafx.h"
#include".\FishLogic\fishdesk.h"
#include "GameTable.h"
#include "RoleManager.h"
#include "RoleEx.h"
#include"FishLogic\Config.h"


struct  GoldPool
{
	GoldPool(int64 ngold)
	{
		gold = ngold;
		open = false;
		byGive = 0;
	}
	int64 gold;
	bool open;
	byte byGive;
};
class TableManager //管理桌子 需要一个桌子的配置文件控制器 网络命令与更新器的线程冲突
{
	//桌子的管理器
public:
	TableManager();
	virtual ~TableManager();

	void OnInit();//初始化
	void Destroy();
	void OnStopService();
	void Update(DWORD dwTimeStep);
	bool OnHandleTableMsg(DWORD dwUserID,NetCmd* pData);
	void OnPlayerJoinTable(BYTE TableTypeID, CRoleEx* pRoleEx,BYTE MonthID = 0,bool IsSendToClient = true);
	bool OnPlayerJoinTable(WORD TableID, CRoleEx* pRoleEx, bool IsSendToClient = true);
	void OnPlayerLeaveTable(DWORD dwUserID);
	void ResetTableInfo(DWORD dwUserID);
	//发送桌子消息
	void	SendDataToTable(DWORD dwUserID, NetCmd* pData);
	

	CRole* SearchUser(DWORD dwUserid);
	CConfig *GetGameConfig();
	GameTable* GetTable(WORD TableID);
	DWORD GetTableSum(){ return m_TableVec.size(); }
	//TableConfig&	GetTableConfig(){ return m_Config; }

	void OnChangeTableGlobel(WORD TableID, int AddGlobel, USHORT uTableRate);
	void OnResetTableGlobel(WORD TableID,int64 nValue);
	int64  GetTableGlobel(WORD TableID);

	void OnChangeTableTypePlayerSum(BYTE TableTypeID,bool IsAddOrDel);
	bool QueryPool(WORD TableID, int64 & nPoolGold);
	void QueryPool(BYTE TableTypeID, bool &bopen, int64&nPoolGold);
	std::list<DWORD> GetBlackList();
	bool SetBlackList(DWORD *pid,BYTE byCount);
	bool Isabhor(DWORD dwUserid);

private:
	DWORD									m_LastUpdate;
	//TableConfig								m_Config;
	
	WORD						 			m_MaxTableID;
	//桌子具体的对象处理
	HashMap<DWORD, WORD>					m_RoleTableMap;
	std::vector<GameTable*>					m_TableVec;

	CConfig                     *m_pGameConfig;

	CFishTimer                  m_TimerRanomCatch;
	CFishTimer                  m_TimerRp;
	CFishTimer                  m_TimerGameTime;
	CFishTimer                  m_TimerRpAdd;

	bool						m_bUpdateTime;
	bool						m_bUpdateRp;
	bool						m_bUpdateRpEffect;

	HashMap<BYTE, GoldPool>		m_TableGlobeMap;//桌子类型的金币池
	HashMap<BYTE, DWORD>        m_TableTypeSum;//当前类型玩家的数量
	std::list<DWORD>			m_blacklist;
};