//任务管理器
#pragma once
#include "Stdafx.h"
class CRoleEx;
class RoleAchievementBase;
class RoleAchievementManager  //用于管理玩家身上的全部的任务
{
public:
	RoleAchievementManager();
	virtual ~RoleAchievementManager();
	bool OnInit(CRoleEx* pRole);//初始化
	void OnLoadAllAchievementInfoByDB(DBO_Cmd_LoadRoleAchievement* pDB);//从数据库加载全部的任务
	void SendAllAchievementToClient();//将当前接取的任务发送到客户端去
	bool IsSendClient(){ return m_IsSendClient; }//是否发生到客户端去了
	bool OnFinishAchievement(BYTE AchievementID);//完成指定任务
	//bool OnDelAchievement(BYTE AchievementID);//删除指定任务
	void OnRoleLevelChange();//当玩家等级变化的时候
	void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	//void OnSaveByUpdate();

	bool IsLoadDB(){ return m_IsLoadDB; }

	RoleAchievementBase* QueryAchievement(BYTE AchievementID);

	void ResetClientInfo(){ m_IsSendClient = false; }

	void GetAllNeedSaveAchievement(vector<tagRoleAchievementInfo>& pVec);

	bool GetAchievementMessageStates();

	void OnResetJoinAllAchievement();
private:
	void OnDestroy();//销毁对象
	void SendLoadAllAchievementInfoToDB();//从数据库加载数据
	bool OnJoinAchievement(BYTE AchievementID,bool IsNeedSave);//接取指定任务
	bool IsCanJoinAchievement(BYTE AchievementID);//判断指定任务是否可以接取
	void OnJoinAchievementByConfig(bool IsNeedSave);//尝试接取任务
	RoleAchievementBase* CreateAchievementByEventID(BYTE EventID);
	//void SaveAchievement();
private:
	bool						m_IsLoadDB;
	bool						m_IsSendClient;
	CRoleEx*					m_pRole;//玩家
	std::vector<RoleAchievementBase*>  m_AchievementVec;
	int256						m_JoinAchievementLog;//接取任务的记录
};
class RoleAchievementBase //玩家任务的基类
{
public:
	RoleAchievementBase();
	virtual ~RoleAchievementBase();
	virtual bool OnInit(CRoleEx* pRole, RoleAchievementManager* pManager, tagRoleAchievementInfo* pInfo);
	virtual bool OnInit(CRoleEx* pRole, RoleAchievementManager* pManager, BYTE AchievementID,bool IsNeedSave);
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual bool IsNeedSave(){ return m_IsNeedSave; }
	virtual void OnSave(){ m_IsNeedSave = false; }
	virtual bool IsEventFinish(){ return m_EventIsFinish; }
	virtual BYTE GetAchievementID(){ return m_AchievementInfo.AchievementID; }
	virtual tagRoleAchievementInfo& GetAchievementInfo(){ return m_AchievementInfo; }
	virtual void OnJoinAchievement();
protected:
	CRoleEx*					m_pRole;//玩家
	RoleAchievementManager*			m_pAchievementManager;
	bool						m_EventIsFinish;//条件是否已经完成了
	bool						m_IsNeedSave;
	//tagAchievementConfig*				m_AchievementConfig;//任务的配置数据
	tagRoleAchievementInfo				m_AchievementInfo;//任务的数据库数据
};
//任务的具体实现类
class GetGlobelRoleAchievement : public RoleAchievementBase
{
public:
	GetGlobelRoleAchievement() : RoleAchievementBase(){}
	virtual ~GetGlobelRoleAchievement(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class GetMadelRoleAchievement : public RoleAchievementBase
{
public:
	GetMadelRoleAchievement() : RoleAchievementBase(){}
	virtual ~GetMadelRoleAchievement() {}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class GetCurrenRoleAchievement : public RoleAchievementBase
{
public:
	GetCurrenRoleAchievement() : RoleAchievementBase(){}
	virtual ~GetCurrenRoleAchievement(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class UpperLevelRoleAchievement : public RoleAchievementBase
{
public:
	UpperLevelRoleAchievement() : RoleAchievementBase(){}
	virtual ~UpperLevelRoleAchievement() {}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class CatchFishRoleAchievement : public RoleAchievementBase
{
public:
	CatchFishRoleAchievement() : RoleAchievementBase(){}
	virtual ~CatchFishRoleAchievement() {}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class SendGiffRoleAchievement : public RoleAchievementBase
{
public:
	SendGiffRoleAchievement() : RoleAchievementBase(){}
	virtual ~SendGiffRoleAchievement(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class UseSkillRoleAchievement : public RoleAchievementBase
{
public:
	UseSkillRoleAchievement() : RoleAchievementBase(){}
	virtual ~UseSkillRoleAchievement(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class LauncherGlobelRoleAchievement : public RoleAchievementBase
{
public:
	LauncherGlobelRoleAchievement() : RoleAchievementBase(){}
	virtual ~LauncherGlobelRoleAchievement(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class MaxGlobelSumRoleAchievement : public RoleAchievementBase
{
public:
	MaxGlobelSumRoleAchievement() : RoleAchievementBase(){}
	virtual ~MaxGlobelSumRoleAchievement(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinAchievement();
};
class ToLevelRoleAchievement : public RoleAchievementBase
{
public:
	ToLevelRoleAchievement() : RoleAchievementBase(){}
	virtual ~ToLevelRoleAchievement(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinAchievement();
};
class MaxCurrenRoleAchievement : public RoleAchievementBase
{
public:
	MaxCurrenRoleAchievement() : RoleAchievementBase(){}
	virtual ~MaxCurrenRoleAchievement(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinAchievement();
};
class RechargeRoleAchievement : public RoleAchievementBase
{
public:
	RechargeRoleAchievement() : RoleAchievementBase(){}
	virtual ~RechargeRoleAchievement(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinAchievement();
};