//中央服务器上的任务管理器
#pragma once
#include "Stdafx.h"
class TaskManager//中央服务器用于控制全局的Task的生成
{
public:
	TaskManager();
	virtual ~TaskManager();
	void OnInit();
	void OnGameServerRsg(BYTE SocketID);
	void UpdateByMin(bool IsHourChange, bool IsDayChange, bool IsMonthChange, bool IsYearChange);
private:
	void CreateGlobelTaskInfo();
	void SendTaskInfoToGameServer(BYTE SocketID);
private:
	std::vector<BYTE>			m_GlobelTaskVec;
};