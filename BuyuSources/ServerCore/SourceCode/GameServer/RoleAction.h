//任务管理器
#pragma once
#include "Stdafx.h"
class CRoleEx;
class RoleActionBase;
//活动相似与成就 有时间限制
//活动 有 链条关系 并且 有时间限制 我们应该限制下 活动有时间限制但是 无每天清空的限制 客户端接取后 我们可以提取出全部活动链条
class RoleActionManager  //用于管理玩家身上的全部的任务
{
public:
	RoleActionManager();
	virtual ~RoleActionManager();
	bool OnInit(CRoleEx* pRole);//初始化
	void OnLoadAllActionInfoByDB(DBO_Cmd_LoadRoleAction* pDB);//从数据库加载全部的任务
	void SendAllActionToClient();//将当前接取的任务发送到客户端去
	bool IsSendClient(){ return m_IsSendClient; }//是否发生到客户端去了
	bool OnFinishAction(BYTE ActionID, DWORD ActionOnceID);//完成指定任务
	void OnRoleLevelChange();//当玩家等级变化的时候
	void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	//void OnSaveByUpdate();
	void UpdateByHour();
	bool IsLoadDB(){ return m_IsLoadDB; }

	RoleActionBase* QueryAction(BYTE ActionID);

	void ResetClientInfo(){ m_IsSendClient = false; }

	void GetAllNeedSaveAction(vector<tagRoleActionInfo>& pVec);

	bool GetActionMessageStates();
	void OnResetJoinAllAction();
private:
	void OnDestroy();//销毁对象
	void SendLoadAllActionInfoToDB();//从数据库加载数据
	bool OnJoinAction(BYTE ActionID, bool IsNeedSave);//接取指定任务
	bool IsCanJoinAction(BYTE ActionID);//判断指定任务是否可以接取
	void OnJoinActionByConfig(bool IsNeedSave);//尝试接取任务
	RoleActionBase* CreateActionByEventID(BYTE EventID);
	//void SaveAction();
private:
	bool						m_IsLoadDB;
	bool						m_IsSendClient;
	CRoleEx*					m_pRole;//玩家
	std::vector<RoleActionBase*>  m_ActionVec;
	int256						m_JoinActionLog;//接取任务的记录
};
class RoleActionBase //玩家任务的基类
{
public:
	RoleActionBase();
	virtual ~RoleActionBase();
	virtual bool OnInit(CRoleEx* pRole, RoleActionManager* pManager, tagRoleActionInfo* pInfo);
	virtual bool OnInit(CRoleEx* pRole, RoleActionManager* pManager, BYTE ActionID, bool IsNeedSave);
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual bool IsNeedSave(){ return m_IsNeedSave; }
	virtual void OnSave(){ m_IsNeedSave = false; }
	//virtual bool IsEventFinish(){ return m_EventIsFinish; }
	virtual BYTE GetActionID(){ return m_ActionInfo.ActionID; }
	virtual tagRoleActionInfo& GetActionInfo(){ return m_ActionInfo; }
	virtual bool  IsInTime();
	virtual void  SetIsNeedSave(){ m_IsNeedSave = true; }
	virtual void OnJoinAction();
	virtual bool IsExistsFinishEvent();
protected:
	CRoleEx*					m_pRole;//玩家
	RoleActionManager*			m_pActionManager;
	//bool						m_EventIsFinish;//条件是否已经完成了
	bool						m_IsNeedSave;
	//tagActionConfig*				m_ActionConfig;//任务的配置数据
	tagRoleActionInfo				m_ActionInfo;//任务的数据库数据
};
//任务的具体实现类
class GetGlobelRoleAction : public RoleActionBase
{
public:
	GetGlobelRoleAction() : RoleActionBase(){}
	virtual ~GetGlobelRoleAction(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class GetMadelRoleAction : public RoleActionBase
{
public:
	GetMadelRoleAction() : RoleActionBase(){}
	virtual ~GetMadelRoleAction() {}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class GetCurrenRoleAction : public RoleActionBase
{
public:
	GetCurrenRoleAction() : RoleActionBase(){}
	virtual ~GetCurrenRoleAction(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class UpperLevelRoleAction : public RoleActionBase
{
public:
	UpperLevelRoleAction() : RoleActionBase(){}
	virtual ~UpperLevelRoleAction() {}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class CatchFishRoleAction : public RoleActionBase
{
public:
	CatchFishRoleAction() : RoleActionBase(){}
	virtual ~CatchFishRoleAction() {}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class SendGiffRoleAction : public RoleActionBase
{
public:
	SendGiffRoleAction() : RoleActionBase(){}
	virtual ~SendGiffRoleAction(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class UseSkillRoleAction : public RoleActionBase
{
public:
	UseSkillRoleAction() : RoleActionBase(){}
	virtual ~UseSkillRoleAction(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class LauncherGlobelRoleAction : public RoleActionBase
{
public:
	LauncherGlobelRoleAction() : RoleActionBase(){}
	virtual ~LauncherGlobelRoleAction(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class MaxGlobelSumRoleAction : public RoleActionBase
{
public:
	MaxGlobelSumRoleAction() : RoleActionBase(){}
	virtual ~MaxGlobelSumRoleAction(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinAction();
};
class ToLevelRoleAction : public RoleActionBase
{
public:
	ToLevelRoleAction() : RoleActionBase(){}
	virtual ~ToLevelRoleAction(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinAction();
};
class MaxCurrenRoleAction : public RoleActionBase
{
public:
	MaxCurrenRoleAction() : RoleActionBase(){}
	virtual ~MaxCurrenRoleAction(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinAction();
};
class RechargeRoleAction : public RoleActionBase
{
public:
	RechargeRoleAction() : RoleActionBase(){}
	virtual ~RechargeRoleAction(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinAction();
};

