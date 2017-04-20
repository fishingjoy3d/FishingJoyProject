#pragma once
#include "Stdafx.h"
#define MAX_TASK_SUM 255
#define MAX_EVENT_SUM 255
#define MAX_BINDPARAM_SUM 255
class CRoleEx;
class EventManager //核心的事件触发机制
{
public:
	EventManager();
	~EventManager();
	void OnInit();
	void OnLoadGlobelTask(CG_Cmd_GetGlobelTaskInfo* pMsg);//加载全局的数据
	bool IsGlobleStop(BYTE TaskID);
private:
	void OnDestroy();
private:
	std::map<BYTE, BYTE>							m_TaskGlobelMap;//全局的任务列表
};