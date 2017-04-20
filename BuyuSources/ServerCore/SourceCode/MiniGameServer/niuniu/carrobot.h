
#pragma once

//机器人信息
struct tagCarRobotInfo
{
	int nChip[5];
	int nAreaChance[MAX_CAR_ClientSum];										//区域几率
	//最大赔率

	tagCarRobotInfo()
	{
		int nTmpChip[5] = { 1000, 10000, 100000, 500000, 1000000 };
		int nTmpAreaChance[MAX_CAR_ClientSum] = { 1, 1, 1, 1, 1, 1, 1, 1 };
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
	}
};

struct CarTimer
{
	CarTimer(BYTE byTimerid, SHORT nTime)
	{
		m_byTimerid = byTimerid;
		m_nTime = nTime;
	}
	BYTE m_byTimerid;
	SHORT m_nTime;
};


class CCarRobot
{
	//游戏变量
protected:
	bool							m_bAction;
	bool							m_bBnkerRobot;						//庄家机器人
	LONGLONG						m_lMaxChipBanker;					//最大下注 (庄家)
	LONGLONG						m_lMaxChipUser;						//最大下注 (个人)
	LONGLONG						m_lAreaChip[MAX_CAR_ClientSum];			//区域下注 	

	int								m_nChipLimit[2];					//下注范围 (0-MAX_DIAL_ClientSum)
	int								m_nChipTime;						//下注次数 (本局)
	int								m_nChipTimeCount;					//已下次数 (本局)

	//上庄变量
protected:
	//bool							m_bMeApplyBanker;					//申请标识
	int								m_nBankerCount;						//本机器人的坐庄次数
	int								m_nWaitBanker;						//空几盘

	//配置变量  (全局配置)
protected:
	tagCarRobotInfo				m_RobotInfo;						//全局配置
	//配置变量	(游戏配置)
protected:
	LONGLONG						m_lAreaLimitScore;					//区域限制
	LONGLONG						m_lUserLimitScore;					//下注限制	

	//配置变量  (机器人配置)
protected:
	int								m_nRobotBankerCount;				//坐庄次数
	int								m_nRobotWaitBanker;					//空盘重申
	bool							m_bReduceJettonLimit;				//降低限制
	int								m_nMinRobotBankerCount;				//最小个数	

	//机器人存取款
	LONGLONG						m_lRobotScoreRange[2];				//最大范围
	LONGLONG						m_lRobotBankGetScore;				//提款数额
	LONGLONG						m_lRobotBankGetScoreBanker;			//提款数额 (庄家)
	int								m_nRobotBankStorageMul;				//存款倍数
	//
	std::vector<CarTimer*>			m_VecOxTimer;
	SHORT							m_nGameTime;
	DWORD							m_dwRobotid;
public:
	static DWORD					m_dwApplistChangeTime;
	static BYTE						m_byAppCountLimit;
	static DWORD					m_dwRobotCountChangeTime;
	static BYTE						m_byRobotCount;


	//控件变量
protected:

	//函数定义
public:
	//构造函数
	CCarRobot(DWORD dwRobotid);
	//析构函数
	virtual ~CCarRobot();
	//游戏事件
public:
	//时间消息
	virtual bool  OnEventTimer(UINT nTimerID);
public:
	//消息处理
public:
	DWORD URand()
	{
		return (rand() << 16) | (rand());
	}
	template<class T>
	T ClipNum(T t1, T t2)
	{
		if (t1 == t2)
		{
			return t1;
		}
		return t1 + URand() % (t2 - t1 + 1);
	}
	bool OnSubUserApplyBanker(const VOID * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const VOID * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const VOID * pBuffer, WORD wDataSize);

	//功能函数
public:
	//读取配置
	VOID ReadConfigInformation(TCHAR szFileName[], TCHAR szRoomName[], bool bReadFresh);
	//计算范围
	bool CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[]);
	bool Update();
	DWORD UserId();
	DWORD UserGold();
	DWORD BankerId();
	DWORD BankerGold();
	WORD  ApplyListLength();
	void StartGame();
	void EndGame();
	void AdjustGold(DWORD dwGold);
	tagCarConfig GameConfig();
};

class CCarRobotManager
{
public:
	CCarRobotManager();
	~CCarRobotManager();
	bool IsRobot(DWORD dwUserid);
	USHORT RobotCount();
	void Update();
	void StartGame();
	void EndGame();
private:
	std::vector<CCarRobot*>m_allrobot;
};