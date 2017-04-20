//转盘管理器 
#pragma once
#include"niuniu/carrobot.h"
enum CarTableStates
{
	CTS_Begin = 1,	//转盘下注阶段
	CTS_End = 2,	//转盘开始阶段
	CTS_WriteBegin = 3,	//等待开始
};

//12 个下注区 我们进行处理 
// 12个下注区 

struct tagCarRoleInfo  //转盘玩家
{
	DWORD			UserID;
	DWORD			BetGlobel[MAX_CAR_ClientSum];
	bool			IsInWriteBanakerVec;
	bool			IsBankerUser;
	bool			IsInVipSeat;
	BYTE			VipSeat;
	bool			IsRobot;
};
struct tagCarVipSeatInfo
{
	DWORD		dwUserID;
	BYTE		NonBetGlobelSum;
};
struct CarResultLog  //Log
{
	BYTE		LogSum;
	DWORD		ResultLog;
	CarResultLog()
	{
		LogSum = 0;
		ResultLog = 0;
	}
	void SetResult(BYTE ResultIndex)
	{
		//12种   F 就够了   8ge  FFFF FFFF
		if (ResultIndex > 0xf)
			return;
		if (LogSum >= 8)
		{
			LogSum = 7;
			//清理数据
			for (BYTE i = 0; i < 28; ++i)
			{
				BYTE Value = ((ResultLog & (1 << (i + 4))) != 0 ? 1 : 0);
				if (Value == 1)
					ResultLog |= (Value << i);
				else
				{
					if ((ResultLog & (1 << i)) != 0)
						ResultLog ^= (1 << i);
				}
			}
			for (BYTE i = 28; i < 32; ++i)
			{
				if ((ResultLog & (1 << i)) != 0)
					ResultLog ^= (1 << i);
			}
		}

		DWORD BeginIndex = LogSum * 4;
		DWORD EndIndex = LogSum * 4 + 3;

		for (DWORD i = BeginIndex, j = 1; i <= EndIndex; ++i, ++j)
		{
			//
			bool Result = ((ResultIndex & (1 << (i - BeginIndex))) != 0);
			if (Result)
				ResultLog = ResultLog | (1 << i);
		}

		++LogSum;
	}
};

class CarManager
{
public:
	CarManager();
	virtual ~CarManager();

	bool		OnInit();//初始化
	void		OnUpdate(DWORD dwTimer);//更新函数
	void		OnDestroy();//销毁函数
	//玩家操作
	void		OnRoleJoinCarTable(DWORD dwUserID);
	void		OnRoleLeaveCarTable(DWORD dwUserID);
	void		OnRoleBetGlobel(DWORD dwUserID, BYTE BetIndex, DWORD AddGlobel);
	void		OnRoleBetGlobelByLog(DWORD dwUserID, DWORD AddGlobelArray[MAX_CAR_ClientSum]);
	void		OnRoleGetWriteBankerList(DWORD dwUserID);//玩家获取排队列表
	void		OnRoleJoinWriteBankerList(DWORD dwUserID);//玩家上庄
	void		OnRoleGetWriteBankerFirstSeat(DWORD dwUserID);//玩家抢庄
	void		OnRoleLeaveWriteBankerList(DWORD dwUserID);//玩家离开庄家排队列表
	void		OnRoleCanelBanker(DWORD dwUserID);//玩家下庄
	void		OnRoleJoinVipSeat(DWORD dwUserID, BYTE VipSeatIndex);//玩家做到贵宾席
	void		OnRoleLeaveVipSeat(DWORD dwUserID);//玩家离开贵宾席
	void		OnRoleGetNormalInfoByPage(DWORD dwUserID, DWORD Page);//玩家获取大众席的数据
	//外部函数
	void		OnRoleLeaveMiniGameServer(DWORD dwUserID);//玩家离开MiniGame服务器了
	void		OnRoleGlobelChange(DWORD dwUserID, int ChangeSum);
	UINT64      GetTableSystemGlobel(){ return m_SystemGlobelSum; }
private:
	void		SetTableStates(CarTableStates States);//设置桌子状态
	void		OnClearTableInfo();//每局之间清理下桌子数据
	//void		SendDataToAllTableRole(NetCmd* pCmd);
	void		SetNextBankerUserInfo();
	tagCarRoleInfo* GetRoleInfo(DWORD dwUserID);
	//事件
	void		OnWriteBankerChange();//当等待列表前8位发生变化的时候 
	void		OnBankerUserChange();//当当前专家发生变化的时候
	void		OnTableJoinBegin();//当桌子进入下注阶段的时候
	void		OnUpdateTableInfo();//更新桌子上的数据到客户端
	void		OnTableJoinEnd();//桌子进入开牌阶段
	//更新函数
	void		UpdateTableIsCanBegin(DWORD dwTimer);//更新桌子 是否可以进入下注阶段
	void		UpdateTableIsCanEnd(DWORD dwTimer);//是否可以进入开牌阶段
	void		UpdateTableInfo(DWORD dwTimer);//更新桌子数据
	//私有函数
	INT64		GetSystemGlobelChange();
	INT64		GetBrandUserGlobelChange();
	INT64		ChangeResultBySystem();
	void		CreateNewCar();
	BYTE		GetTableResult(BYTE& Index);
	SHORT		GetBrandRateValue(BYTE Index);
	bool		IsCanAddGlobel(DWORD AddGlobel, BYTE Index);
	BYTE		GetResultID();
	BYTE		GetResultRate();

public:
	DWORD		BankerId();
	DWORD		BankerGold();
	WORD		ApplyListLength();
	bool		IsGameEnd();
	bool		HaveApply(DWORD dwUserid);
	void		QueryNames(DWORD ClientID);
	void		QueryAllJetton(DWORD ClientID);
	void		QueryPlayerJetton(TCHAR *pNickame, DWORD ClientID);
	void		CalPlayerJetton(UINT64 uJetton[MAX_CAR_ClientSum]);
private:
	CarTableStates				m_TableStates;//桌子的状态
	DWORD						m_TableStatesLog;//桌子进入当前状态的记录时间
	bool						m_IsNeedClearBankerInfo;
	//桌子上的一些数据
	INT64						m_SystemGlobelSum;//系统库存金币数
	UINT64						m_TableBetAreaGlobel[MAX_CAR_ClientSum];//8个下注区的金币总数
	BYTE						m_TableResult;//轮盘的结果
	BYTE						m_TableResultIndex;//轮盘的位置
	//庄家数据
	DWORD						m_TableBankerUserID;//庄家的ID
	BYTE						m_TableBankerUseGameSum;//当前庄家已经进行游戏的局数
	//等待庄家列表的数据
	vector<DWORD>				m_TableWriteBankerVec;//等待列表
	//基本玩家的数据
	HashMap<DWORD, tagCarRoleInfo> m_TableUserMap;//玩家集合
	//贵宾席
	tagCarVipSeatInfo				m_VipSeatVec[MAX_CARVIPSEAT_Sum];//贵宾席的集合 8个贵宾席
	//
	CarResultLog					m_CarResultLog;
	CCarRobotManager				m_robotmanager;
};