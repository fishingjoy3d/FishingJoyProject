#include "Stdafx.h"
#include "TaskManager.h"
#include "FishServer.h"
TaskManager::TaskManager()
{

}
TaskManager::~TaskManager()
{

}
void TaskManager::OnInit()
{
	CreateGlobelTaskInfo();
}
void TaskManager::OnGameServerRsg(BYTE SocketID)
{
	SendTaskInfoToGameServer(SocketID);
}
void TaskManager::UpdateByMin(bool IsHourChange, bool IsDayChange, bool IsMonthChange, bool IsYearChange)
{
	if (IsDayChange)//每
	{
		CreateGlobelTaskInfo();
		SendTaskInfoToGameServer(0);
	}
}
void TaskManager::CreateGlobelTaskInfo()
{
	//处理一个任务的全局事件
	m_GlobelTaskVec.clear();
	HashMap<BYTE, std::vector<BYTE>>::iterator Iter = g_FishServer.GetFishConfig().GetTaskConfig().m_TaskGroup.begin();
	for (; Iter != g_FishServer.GetFishConfig().GetTaskConfig().m_TaskGroup.end(); ++Iter)
	{
		BYTE TaskID = Iter->second[RandUInt() % Iter->second.size()];
		m_GlobelTaskVec.push_back(TaskID);
	}
	//g_FishServer.ShowInfoToWin("全局任务管理器 生成新的每日任务 互斥列表");
}
void TaskManager::SendTaskInfoToGameServer(BYTE SocketID)
{
	DWORD PageSize = sizeof(CG_Cmd_GetGlobelTaskInfo)+(g_FishServer.GetFishConfig().GetTaskConfig().m_TaskGroup.size() - 1)*sizeof(BYTE);
	//CheckMsgSize(PageSize);
	CG_Cmd_GetGlobelTaskInfo* msg = (CG_Cmd_GetGlobelTaskInfo*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Task, CG_GetGlobelTaskInfo));
	std::vector<BYTE>::iterator Iter = m_GlobelTaskVec.begin();
	for (WORD i = 0; Iter != m_GlobelTaskVec.end(); ++Iter, ++i)
	{
		msg->Array[i] = *Iter;
	}
	std::vector<CG_Cmd_GetGlobelTaskInfo*> pVec;
	SqlitMsg(msg, PageSize, g_FishServer.GetFishConfig().GetTaskConfig().m_TaskGroup.size(),false, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<CG_Cmd_GetGlobelTaskInfo*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			if (SocketID != 0)
			{
				g_FishServer.GetCenterManager().SendNetCmdToGameServer(SocketID, *Iter);
			}
			else
			{
				g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(*Iter);
			}
			free(*Iter);
		}
		pVec.clear();
	}
}