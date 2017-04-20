#pragma once

class CServerControlOx
{
	//玩家控制						
protected:
	BYTE							m_cbExcuteTimes;				//执行次数
	BYTE							m_cbControlStyle;				//控制方式
	bool							m_bWinArea[MAX_NIUNIU_ClientSum];		//赢家区域	
public:
	CServerControlOx(void);
	virtual ~CServerControlOx(void);

public:

public:
	//服务器控制
	virtual bool  Control(const void * pDataBuffer);

	//需要控制
	virtual bool  NeedControl();

	//完成控制
	virtual bool  CompleteControl();

	
	//开始控制
	virtual void  GetSuitResult(BYTE cbTableCardArray[MAX_NIUNIU_ClientSum + 1][MAX_NIUNIU_BrandSum], WORD wTableBrandResult[MAX_NIUNIU_ClientSum + 1],UINT64 nAllJettonScore[MAX_NIUNIU_ClientSum]);

	//输赢控制
protected:

	//区域牌型
	bool AreaWinCard(BYTE cbStack[], int nTableMutiple[][MAX_NIUNIU_ClientSum + 1]);

	//计算分配
	bool GetSuitCardCombine(BYTE cbStack[], int nTableMutiple[][MAX_NIUNIU_ClientSum + 1], UINT64 nAllJettonScore[]);

	//庄家输赢
	LONGLONG GetBankerWinScore(int nWinMultiple[], UINT64 nAllJettonScore[]);

	//庄家赢牌
	void BankerWinCard(bool bIsWin, BYTE cbStack[], int nTableMutiple[][MAX_NIUNIU_ClientSum + 1], UINT64 nAllJettonScore[]);
};

class CServerControlAnimal
{
protected:
	BYTE							m_cbExcuteTimes;				//执行次数
	BYTE							m_cbControlStyle;				//控制方式
	BYTE							m_cbarea;
public:
	CServerControlAnimal();
	virtual bool  Control(const void * pDataBuffer);
	virtual bool  NeedControl();//完成控制
	virtual bool  CompleteControl();
	BYTE GetSuitResult();
};

class CServerControlCar
{
protected:
	BYTE							m_cbExcuteTimes;				//执行次数
	BYTE							m_cbControlStyle;				//控制方式
	BYTE							m_cbarea;
public:
	CServerControlCar();
	virtual bool  Control(const void * pDataBuffer);
	virtual bool  NeedControl();//完成控制
	virtual bool  CompleteControl();
	BYTE GetSuitResult();
};