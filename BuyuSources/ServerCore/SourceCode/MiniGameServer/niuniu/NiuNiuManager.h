#pragma once
#include"oxrobot.h"
enum BrandNiuNiuLevel
{
	BNNL_Non	= 1,		//无牛
	BNNL_1		= 2,		//牛一 
	BNNL_2		= 3,
	BNNL_3		= 4,
	BNNL_4		= 5,
	BNNL_5		= 6,
	BNNL_6		= 7,
	BNNL_7		= 8,
	BNNL_8		= 9,
	BNNL_9		= 10,		//牛9
	BNNL_NN		= 11,		//牛牛
	BNNL_WHN	= 12,		//五花牛
	BNNL_ZD		= 13,		//炸弹
	BNNL_WXN	= 14,		//五小牛
};
enum NiuNiuTableStates
{
	NNTS_Begin			= 1,	//下注阶段
	NNTS_End			= 2,	//开牌阶段
	NNTS_WriteBegin		= 3,	//等待开始
};
struct tagNiuNiuRoleInfo
{
	DWORD			UserID;
	DWORD			BetGlobel[MAX_NIUNIU_ClientSum];
	bool			IsInWriteBanakerVec;
	bool			IsBankerUser;
	bool			IsInVipSeat;
	bool			IsRobot;
	BYTE			VipSeat;
};
struct OnceBrandList//一副牌
{
	std::vector<BYTE> BrandVec;//牌的集合

	OnceBrandList()
	{
		for (BYTE i = 1; i <= 52; ++i)
			BrandVec.push_back(i);
	}

	BYTE    GetNewBrand()
	{
		//获取一张新牌
		BYTE Index = RandUInt() % BrandVec.size();
		BYTE BrandValue = BrandVec[Index];//获取指定下表的牌
		BrandVec.erase(BrandVec.begin() + Index);
		return BrandValue;
	}
	void  AddBrand(BYTE BrandValue)
	{
		BrandVec.push_back(BrandValue);
	}
	void    OnClear()
	{
		BrandVec.clear();
		for (BYTE i = 1; i <= 52; ++i)
			BrandVec.push_back(i);
	}
};
struct VipSeatInfo
{
	DWORD		dwUserID;
	BYTE		NonBetGlobelSum;
};
struct NiuNiuResultLog  //Log
{
	BYTE		LogSum;
	DWORD		ResultLog;
	NiuNiuResultLog()
	{
		LogSum = 0;
		ResultLog = 0;
	}
	void SetResult(WORD TableResult[MAX_NIUNIU_ClientSum + 1])
	{
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
		
		for (DWORD i = BeginIndex, j = 1; i <= EndIndex; ++i,++j)
		{
			bool Result = (TableResult[0] >= TableResult[j]);//庄家赢
			if (Result)
				ResultLog = ResultLog | (1 << i);
		}

		++LogSum;
	}
};
class NiuNiuManager
{
public:
	NiuNiuManager();
	virtual ~NiuNiuManager();
	//基本函数
	bool		OnInit();//初始化函数
	void		OnUpdate(DWORD dwTimer);//更新函数
	void		OnDestroy();//销毁函数
	//玩家的操作
	void		OnRoleJoinNiuNiuTable(DWORD dwUserID);//玩家进入牛牛桌子
	void		OnRoleLeaveNiuNiuTable(DWORD dwUserID);//玩家离开牛牛桌子
	void		OnRoleBetGlobel(DWORD dwUserID, BYTE BetIndex, DWORD AddGlobel);//玩家下注
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
	void		OnRoleGlobelChange(DWORD dwUserID,int ChangeSum);

	UINT64      GetTableSystemGlobel(){ return m_SystemGlobelSum; }
private:
	//内部函数
	void		SetTableStates(NiuNiuTableStates States);//设置桌子状态
	void		OnClearTableInfo();//每局之间清理下桌子数据
	//void		SendDataToAllTableRole(NetCmd* pCmd);
	void		SetNextBankerUserInfo();
	tagNiuNiuRoleInfo* GetRoleInfo(DWORD dwUserID);
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
	INT64		ChangeBrandValueBySystem(OnceBrandList& pBrand);
	SHORT		GetBrandRateValue(BYTE Index);
	WORD		HandleBrandValue(BYTE* BrandValue);
	BYTE		GetBrandHandleValue(BYTE Value);
	void		ChangeValue(BYTE& A, BYTE& B);

	//void		OnChangeSystemGlobel(DWORD dwUserID, INT64 ChangeGlobel);
public:
	void		QueryNames(DWORD ClientID);
	void		QueryAllJetton(DWORD ClientID);
	void		QueryPlayerJetton(TCHAR *pNickame, DWORD ClientID);
	DWORD		BankerId();
	DWORD		BankerGold();
	WORD		ApplyListLength();
	bool		IsGameEnd();
	bool		HaveApply(DWORD dwUserid);
	void		CalPlayerJetton(UINT64 uJetton[MAX_NIUNIU_ClientSum]);
private:
	NiuNiuTableStates			m_TableStates;//桌子的状态
	DWORD						m_TableStatesLog;//桌子进入当前状态的记录时间
	bool						m_IsNeedClearBankerInfo;
	//桌子上的一些数据
	INT64						m_SystemGlobelSum;//系统库存金币数
	UINT64						m_TableBetAreaGlobel[MAX_NIUNIU_ClientSum];//4个下注区的金币总数
	BYTE						m_TableBrandArray[MAX_NIUNIU_ClientSum + 1][MAX_NIUNIU_BrandSum];//桌子上 5x5 牌
	WORD						m_TableBrandResult[MAX_NIUNIU_ClientSum + 1];//牌的结果
	//庄家数据
	DWORD						m_TableBankerUserID;//庄家的ID
	BYTE						m_TableBankerUseGameSum;//当前庄家已经进行游戏的局数
	//等待庄家列表的数据
	vector<DWORD>				m_TableWriteBankerVec;//等待列表
	//基本玩家的数据
	HashMap<DWORD, tagNiuNiuRoleInfo> m_TableUserMap;//玩家集合
	//贵宾席
	VipSeatInfo					m_VipSeatVec[MAX_VIPSEAT_Sum];//贵宾席的集合 8个贵宾席
	//比赛趋势
	NiuNiuResultLog				m_NiuNiuResultLog;
	COxRobotManager				m_oxrobotmanager;
};