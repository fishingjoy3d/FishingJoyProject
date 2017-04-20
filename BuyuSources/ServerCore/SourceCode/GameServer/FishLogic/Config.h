#ifndef CONFIG_HEAD
#define CONFIG_HEAD
#pragma once
#include<vector>
#include<algorithm>
//#include"Role.h"
#include "FishCallbak.h"
#include "FishUtility.h"


struct Con_Combo
{
	static int s_sustaintime;
	static int s_buff_cycle;
	static float s_bufferchance;
};
struct Con_Energy //大招 配置
{
	int  nid;
	TCHAR chname;
	float ncdtime;
	int nmincatch;
	int nmaxcatch;
	int nrevise;
	int nincome;//收益
	float denergyratio;
	int nthreshold;
	int nradius;

	float npseed;
	float ntime1;//减速阶段
	float ntime2;//保持匀速
	float ntime3;//恢复阶段	
};

struct SkillConsume  //道具消耗,
{
	byte byorder;//次数
	byte byCount;//数量
};

struct Con_Skill //技能
{

	int  nid;
	TCHAR chname;
	float ncdtime;
	int nmincatch;
	int nmaxcatch;
	int nrevise;
	int nincome;//收益

	int ngoodsid;
	int multiple;//收益倍率
	std::vector<SkillConsume>vecConsume;
	//int ngoodsconsume;
	//只有某些道具特有
	int nplaytime;
	//针对减速
	float npseed;
	float ntime1;//减速阶段
	float ntime2;//保持匀速
	float ntime3;//恢复阶段



};

struct  Con_Rate //倍率
{
	int  nid;
	int nValue;
	int nUnlock;
	int unlockreward;
	float drop;
};

struct Con_Cannon //炮 
{
	int  nid;
	char chname;
	float dlauncherinterval;
	int bulletspeed;
	int nconsume;
	float nradius;
	float ncatchradius;
	int nmaxcatch;
	std::vector<float>vCollision;


	int   nItemId;   //物品id
	int   nItemCount;//物品数量
	int   nskill;

	float npseed;
	float ntime1;//减速阶段
	float ntime2;//保持匀速
	float ntime3;//恢复阶段


	static float s_ratiofactor;
	static float s_maxratio;
	static float s_finalratio;
	static float s_lockratio;
};

struct Con_Fish //鱼 
{
	int nrank;
	int  nid;
	int nvalue;
	int maxcount;
	//float reviseratio;
	float chance; //掉落概率
	int   type;  //掉落类型
	float flashchance; //闪电影响 
	vector<float>fishrate;//
	static float s_revies_Activity;
	static int s_flashfishid;
	static int s_flashfishCatch;
	static int s_flashfishradius;
};

struct Con_Rp //概率 付出与回报
{
	int  nid;
	float rp;
	float reviseratio1;
	float reviseratio2;
	static float s_cycle;
	static float s_effecttime;
};

struct Randomtime //鱼的概率
{
	static int s_cycle;
	static float s_reviseratio1;
	static float s_reviseratio2;
};

struct Con_Production //产量决定概率
{
	int  nid;
	int nincome;
	float reviseratio1;
	float reviseratio2;
};

struct Con_Rank //玩家等级 的概率
{
	int  nid;
	int level;
	int experience;
	float reviseratio;
};

struct Con_Playtime //有效时间 当天的
{
	int  nid;
	int ntime;
	float reviseratio1;
	float reviseratio2;
	float drop;
};

struct Con_Money //玩家的金钱树
{
	int  nid;
	int money;
	float reviseratio1;
	float reviseratio2;
};

struct Con_LuckItem
{
	static int s_threshold;//阀值
	static float s_base;   //保底
	static float s_part;//

	int nid;
	int nluck;
	float reviseratio1;
	float reviseratio2;

};

//奖池
struct Con_Pool
{
	int nid;
	int pool;
	float reviseratio1;
	float reviseratio2;
};


//template<class T>
//class Vec :public vector<T>
//{
//public:
//	typename Vec<T>::iterator find(int id)
//	{
//		int nIndex = 0;
//		Vec<T>::iterator it = begin();
//		for (; it != end(); it++, nIndex++)
//		{
//			if (nIndex == id)
//			{
//				break;
//			}
//		}
//		return it;
//	}
//};

template<class T>
bool SortByid(T&v1, T& v2)
{
	return v1.nid < v2.nid;
}

typedef vector<Con_Energy>Energy_Vec;
typedef vector<Con_Skill>Skill_Vec;
typedef vector<Con_Rate>Rate_Vec;
typedef vector<Con_Cannon>Cannon_Vec;
typedef vector<Con_Fish>Fish_Vec;
typedef vector<Con_Rp>Rp_Vec;
typedef vector<Con_Production>Production_Vec;
typedef vector<Con_Rank>Rank_Vec;
typedef vector<Con_Playtime>Playtime_Vec;
typedef vector<Con_Money>Money_Vec;
typedef vector<Con_LuckItem>Luck_Vec;
typedef vector<Con_Pool>Pool_Vec;

class TableManager;
class CConfig :public IFishSetting
{
public:
	CConfig(TableManager *pTableManager);
	~CConfig();
	virtual UINT  GetVersion();
	virtual float BulletRadius(USHORT bulletID);					//子弹基本半径
	virtual float BulletCatchRadius(USHORT bulletID);					//子弹碰撞之后的捕获半径
	virtual UINT  MaxCatch(USHORT bulletID);				//子弹的最大捕鱼数量
	virtual float CatchChance(PlayerID player, USHORT bulletID, byte BulletType, byte fishType, int catchNum, int maxCatch, BYTE byPackageType, bool bLocked);	//子弹的捕获机率(0 - 1)
	virtual SkillFailedType  UseSkill(PlayerID playerID, SkillType skill);
	virtual LaunchFailedType  UseLaser(PlayerID playerID, byte launcherType);
	virtual void  GetSkillRange(SkillType skill, int &minNum, int &maxNum);
	virtual void  GetLaserRange(byte laser, int &minNum, int &maxNum);
	virtual void  GetSkillFreezeReduction(byte &speedScaling, byte &duration1, byte& duration2, byte &duration3);
	virtual float GetSkillChance(SkillType skill, USHORT fishIndex, BYTE byPackageType);
	virtual float GetLaserChance(byte launcherType, USHORT fishIndex, BYTE byPackageType);
	virtual float GetLaserRadius(byte launcherType);
	//LauncherType炮台类型
	virtual BulletType GetBulletType(USHORT bulletID);
	virtual void  GetBulletFreezeReduction(byte &speedScaling, byte &duration1, byte& duration2, byte &duration3);
	//捕获鱼，返回金币数量
	virtual  UINT  CatchFish(PlayerID playerID, USHORT fishIndx, CatchType catchType, byte subType, byte packge, byte rate);
	virtual USHORT    GetCombo(PlayerID playerID, USHORT bulletID);
	//是否允许上传头像
	virtual bool CanUploadImg(PlayerID playerID, USHORT imgSize);
	//技能的cd时间
	virtual byte  GetSkillCDTime(SkillType st);
	//炮台大招的减速参数
	virtual void  GetLaserReduction(byte LauncherType, byte &speedScaling, byte &duration1, byte& duration2, byte &duration3);
	//炮台的子弹速度
	virtual byte  GetBulletSpeed(byte launcherType);
	//炮台的发射间隔
	virtual byte  GetLauncherInterval(byte LauncherType);
	//大招的CD时间
	virtual byte  GetLaserCDTime(byte LauncherType);
	virtual UINT  LaunchPackage(UINT fishid_packageid);
	virtual byte  GetRateIndex(byte seat, PlayerID id);
	virtual void  BulletGain(PlayerID id, byte BulletType, UINT goldNum);
	virtual USHORT FishRewardDrop(PlayerID id, BYTE byPackageType, USHORT byType);

	virtual bool IsLightingFish(USHORT fishIndex);
	virtual int  IsLightingFish(USHORT fishIndex, PlayerID id);
	//某一条鱼是否可以被闪电捕获，fishIndex鱼ID，玩家ID，dist:屏幕距离。
	virtual bool CanCatchLightingFish(USHORT fishIndex, PlayerID id, float dist);
	/////////////
	byte  BulletConsume(byte LauncherType);
	ushort  BulletMultiple(byte byIndex);

	float CalRandom(float d1, float d2);
	float CalBaseRatio(BYTE cbCannoIndex, BYTE cbFishIndex);
	bool LoadConfig(char szDir[]);	


	float RpRate(float nRp);
	float ProductionRate(int nProducntion);
	float RankRate(int nLevel);
	float GameTimeRate(int nMinutes);
	float MoneyRate(int nMoney);
	float LuckRate(int nLuck);
	float PoolRate(int nPool);

	int GetLevle(int nExperience);

	int RandomCatchCycle();
	int RpCycle();
	int RpEffectCycle();
	int FishCount();
	int CannonCount();
	int SkillCount();
	int RankCount();
	int RateCount();

	int  LaserThreshold(byte byIndex);
	float  LauncherInterval(byte byIndex);
	float  LaserCdTime(byte byIndex);
	float  SkillCdTime(byte byIndex);

	void GoodsInfo(int nSkill, int norder, int &nGoodsid, int &nGoodsConsume);
	void GoodsInfo(int nCannon, int &nGoodsid, int &nGoodsCount);
	bool FindCannon(byte nSkill, int &nCannon);
	bool LevelUp(WORD cbLevel, DWORD nExp, WORD& nNewLevel, DWORD&nNewExp);
	float ComboSustainTime();
	int ComboBuffCycle();	
	float PackageFactor(BYTE cbPackage);
	USHORT RateUnlock(BYTE byIndex);
	ushort SkillMultiple(byte byIndex);
	float GameTimeDrop(int nTime);
	USHORT UnlockRateReward(BYTE byIndex);
	BYTE CannonOwnSkill(BYTE byIndex);
private:
	int  m_nServerVersion;

	Energy_Vec m_Energy;
	Skill_Vec m_Skill;
	Rate_Vec m_Rate;
	Cannon_Vec m_Cannon;
	Fish_Vec m_Fish;
	Rp_Vec m_Rp;
	Randomtime m_RandomTime;
	Production_Vec m_production;
	Rank_Vec m_Rank;
	Playtime_Vec m_PlayTime;
	Money_Vec  m_Money;
	Luck_Vec   m_VceLuck;
	Pool_Vec   m_GoldPool;

	TableManager *m_pTableManager;
};
#endif