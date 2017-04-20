//任务管理器
#pragma once
#include "Stdafx.h"
class CRoleEx;
class RoleTaskBase;
class RoleTaskManager  //用于管理玩家身上的全部的任务
{
public:
	RoleTaskManager();
	virtual ~RoleTaskManager();
	bool OnInit(CRoleEx* pRole);//初始化
	void OnLoadAllTaskInfoByDB(DBO_Cmd_LoadRoleTask* pDB);//从数据库加载全部的任务
	void OnDayChange();//当天数变化的时候
	void SendAllTaskToClient();//将当前接取的任务发送到客户端去
	bool IsSendClient(){ return m_IsSendClient; }//是否发生到客户端去了
	bool OnFinishTask(BYTE TaskID);//完成指定任务
	bool OnDelTask(BYTE TaskID);//删除指定任务
	void OnRoleLevelChange();//当玩家等级变化的时候
	void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	//void OnSaveByUpdate();

	bool IsLoadDB(){ return m_IsLoadDB; }

	RoleTaskBase* QueryTask(BYTE TaskID);

	void ResetClientInfo(){ m_IsSendClient = false; }

	void GetAllNeedSaveTask(vector<tagRoleTaskInfo>& pVec);

	bool GetTaskMessageStates();
	void OnResetJoinAllTask();
private:
	void OnDestroy();//销毁对象
	void SendLoadAllTaskInfoToDB();//从数据库加载数据
	bool OnClearAllTask();//清空所有接取的任务
	bool OnJoinTask(BYTE TaskID,bool IsNeedSave);//接取指定任务
	bool IsCanJoinTask(BYTE TaskID);//判断指定任务是否可以接取
	void OnJoinTaskByConfig(bool IsNeedSave);//尝试接取任务
	RoleTaskBase* CreateTaskByEventID(BYTE EventID);
	//void SaveTask();
private:
	bool						m_IsLoadDB;
	bool						m_IsSendClient;
	CRoleEx*					m_pRole;//玩家
	std::vector<RoleTaskBase*>  m_TaskVec;
	int256						m_JoinTaskLog;//接取任务的记录
};
class RoleTaskBase //玩家任务的基类
{
public:
	RoleTaskBase();
	virtual ~RoleTaskBase();
	virtual bool OnInit(CRoleEx* pRole, RoleTaskManager* pManager, tagRoleTaskInfo* pInfo);
	virtual bool OnInit(CRoleEx* pRole, RoleTaskManager* pManager, BYTE TaskID,bool IsSave);
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual bool IsNeedSave(){ return m_IsNeedSave; }
	virtual void OnSave(){ m_IsNeedSave = false; }
	virtual bool IsEventFinish(){ return m_EventIsFinish; }
	virtual BYTE GetTaskID(){ return m_TaskInfo.TaskID; }
	virtual tagRoleTaskInfo& GetTaskInfo(){ return m_TaskInfo; }
	virtual void OnJoinTask();
protected:
	CRoleEx*					m_pRole;//玩家
	RoleTaskManager*			m_pTaskManager;
	bool						m_EventIsFinish;//条件是否已经完成了
	bool						m_IsNeedSave;
	//tagTaskConfig*				m_TaskConfig;//任务的配置数据
	tagRoleTaskInfo				m_TaskInfo;//任务的数据库数据
};
//任务的具体实现类
class GetGlobelRoleTask : public RoleTaskBase
{
public:
	GetGlobelRoleTask() : RoleTaskBase(){}
	virtual ~GetGlobelRoleTask(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class GetMadelRoleTask : public RoleTaskBase
{
public:
	GetMadelRoleTask() : RoleTaskBase(){}
	virtual ~GetMadelRoleTask() {}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class GetCurrenRoleTask : public RoleTaskBase
{
public:
	GetCurrenRoleTask() : RoleTaskBase(){}
	virtual ~GetCurrenRoleTask(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class UpperLevelRoleTask : public RoleTaskBase
{
public:
	UpperLevelRoleTask() : RoleTaskBase(){}
	virtual ~UpperLevelRoleTask() {}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class CatchFishRoleTask : public RoleTaskBase
{
public:
	CatchFishRoleTask() : RoleTaskBase(){}
	virtual ~CatchFishRoleTask() {}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class SendGiffRoleTask : public RoleTaskBase
{
public:
	SendGiffRoleTask() : RoleTaskBase(){}
	virtual ~SendGiffRoleTask(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class UseSkillRoleTask : public RoleTaskBase
{
public:
	UseSkillRoleTask() : RoleTaskBase(){}
	virtual ~UseSkillRoleTask(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class LauncherGlobelRoleTask : public RoleTaskBase
{
public:
	LauncherGlobelRoleTask() : RoleTaskBase(){}
	virtual ~LauncherGlobelRoleTask(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
};
class MaxGlobelSumRoleTask : public RoleTaskBase
{
public:
	MaxGlobelSumRoleTask() : RoleTaskBase(){}
	virtual ~MaxGlobelSumRoleTask(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinTask();
};
class ToLevelRoleTask : public RoleTaskBase
{
public:
	ToLevelRoleTask() : RoleTaskBase(){}
	virtual ~ToLevelRoleTask(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinTask();
};
class MaxCurrenRoleTask : public RoleTaskBase
{
public:
	MaxCurrenRoleTask() : RoleTaskBase(){}
	virtual ~MaxCurrenRoleTask(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinTask();
};
class RechargeRoleTask : public RoleTaskBase
{
public:
	RechargeRoleTask() : RoleTaskBase(){}
	virtual ~RechargeRoleTask(){}
	virtual void OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param);
	virtual void OnJoinTask();
};

