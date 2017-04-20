#pragma once
#include "Stdafx.h"
#include"SkillInfo.h"
#include "RoleChest.h"
#include "FishLogic\FishUtility.h"
#include "FishLogic\FishCallbak.h"
class CTableRoleManager;
class CRoleEx;
class CConfig;

class CRole
{
	struct ComboInfo
	{
		USHORT m_ComboBulletID;
		USHORT m_crit;
		DWORD m_dwEndTime;
		std::vector<WORD>	m_vecBulletid;
	};
	struct LauncherInfo //炮信息
	{
		int nEnergy;
		//bool nEnable;
	};
	typedef std::vector<LauncherInfo>VecLauncherInfo;

public:
	CRole();
	virtual ~CRole();

	bool		OnInit(WORD TableID, BYTE SeatID, CTableRoleManager* pManager);//初始化桌子上的玩家的数据
	bool		IsActionUser() { return m_pRoleEx != NULL; }//判断当前位置是否为空的
	void		Clear();
	void		SetUser(CRoleEx* pUser);

	WORD		GetTableID(){ return m_TableID; }
	BYTE		GetSeatID(){ return m_SeatID; }

	DWORD		GetID();
	LPTSTR		GetNickName();
	DWORD		GetFaceID();
	CRoleEx*    GetRoleExInfo(){ return m_pRoleEx; }

	//自定义头像处理
	/*void		OnBeginUpLoadFaceData(WORD Size);
	void		OnUpLoadFaceData(WORD StarIndex, WORD Length, BYTE* pData);*/

	void Update(DWORD dwTimer);

	bool CheckBulletIdx(BYTE &bulletID);

	byte GetLauncherType()const
	{
		return m_LauncherType;
	}
	byte GetRateIndex()
	{
		return m_nMultipleIndex;
	}
	void AddLauncher(bool badd);
	void SetLauncher(BYTE LauncherType);
	void ResetData();

	void OnCatchFish(CatchType catchType, byte subType,WORD FishType, BYTE byPackageType, int nScore, int nExp);
	bool CheckFire(BYTE byLauncher);
	bool IsFullEnergy();
	SkillFailedType UseSkill(SkillType skill);
	LaunchFailedType UseLaser(byte launcherType);

	float GetRp();
	DWORD GetScore();
	DWORD GetExp();
	WORD  GetLevel();
	DWORD GetProduction();
	DWORD GetGameTime();
	void SetRpEffect(bool badd);
	void AddGameTime(int nSecond);
	USHORT Combo(WORD wCounter);
	void BulletGain(USHORT uBulletType, UINT uGain);
	bool IsComboBuff(USHORT BulletID);
	void OnSkillUsed(float nCdtime);
	void SetBulletRate(USHORT uPoolRate,BYTE byMin, BYTE byMax);
	void SetRoomLauncher();

	RoleChestManager& GetChestManager(){ return m_ChestManager; }

	BYTE GetBulletIdx(){ return m_BulletIdx; }
	ushort SkillInfo(SkillType skill);
	void SetRateIndex(byte idex);
	void ChangeRateIndex(byte idex);
	void OnChangeRateToIndex(bool UpOrDown, bool& IsCanUse);
	void OnChangeRateToIndex(byte RateIndex, bool& IsCanUse);
	int GetEnergy()
	{
		return m_vecLauncherInfo[m_LauncherType].nEnergy;
	}

	bool IsCanSendTableMsg(){ return m_IsSendTableMsg; }
	void SetRoleIsCanSendTableMsg(bool States){ m_IsSendTableMsg = States; }

	void OnResetRoleLauncher();
	
	bool HaveNormalLauncher(BYTE byType);

	void SetRoleLuckyValue(DWORD Value){ m_LuckyValue = Value; }
	void DelRoleLuckValue(DWORD Value);
	void AddRoleLuckValue(DWORD Value);
	DWORD GetRoleLuckyValue();

	USHORT TableRate();
	float RandTimeRate(float fmin, float fmax, BYTE byFishType);
	void ClearComb();
	void AddDropReward(ushort nReward);

	bool IsNeedLeaveTable();
	void SetLockEndTime();
	bool IsCanLock();
	ushort MinBulletCosume();	
	USHORT CombCount();
	void DelayCombo();
private:
	WORD		m_TableID;//桌子号
	BYTE		m_SeatID;//位置号

	CRoleEx*	m_pRoleEx;//外部的玩家的类
	byte		m_BulletIdx;//子弹 
	byte		m_LauncherType;//炮台
	byte        m_nMultipleIndex;//
	VecLauncherInfo				  m_vecLauncherInfo;

	int                         m_nPay;			         //付出,算r/p
	int                         m_nRevenue;				 //收入  r/p
	float                       m_fRp;
	DWORD						m_dwLastFireTime;
	BYTE						m_byMinRate;
	BYTE						m_byMaxRate;
	USHORT						m_nTableRate;

	CSkillInfo                  m_skill;
	ComboInfo                   m_Combo;
	CConfig                     *m_pConfig;
	CTableRoleManager			*m_pTableRolemanager;

	bool						m_IsSendTableMsg;

	RoleChestManager			m_ChestManager;//宝箱管理器
	DWORD						m_LuckyValue;//玩家的幸运值

	DWORD						m_dwUpdateRandomtime;
	vector<int> m_vecRandomByTime;;

	DWORD						m_dwLastUseSkillTime;

	DWORD						m_dwLockEndTime;//锁定结束时间
	BYTE						m_nBulletCount;
};
class CTableRoleManager
{
public:
	CTableRoleManager();
	virtual ~CTableRoleManager();

	void		Destroy();
	void		OnInit(WORD TableID, BYTE TableMaxPlayerSum);

	bool		IsFull(){ return GetRoleSum() >= GetMaxPlayerSum(); }
	int			GetRoleSum();//获取用户数量 获取激活的用户数量
	bool		OnDelRole(PlayerID RoleID);
	bool		OnDleRole(WORD ChairID);
	void		OnDelAllRole();
	bool		OnInitRole(CRoleEx* pEx);
	BYTE		GetMaxPlayerSum(){ return m_MaxTableUserSum; }
	CRole*		GetRoleBySeatID(BYTE ChairID);
	CRole*		GetRoleByUserID(PlayerID RoleID);
	void		OnUpdate(DWORD dwTimer);
	//桌子正在切换鱼阵
	void		SwitchFishTide();
private:
	WORD			m_TableID;
	BYTE			m_MaxTableUserSum;
	CRole*			m_TableRoleArray;
};

