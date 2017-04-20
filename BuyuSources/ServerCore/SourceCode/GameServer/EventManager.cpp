#include "Stdafx.h"
#include "EventManager.h"
#include "RoleEx.h"
#include "FishServer.h"
EventManager::EventManager()
{
}
EventManager::~EventManager()
{
	OnDestroy();
}
void EventManager::OnInit()
{
}
void EventManager::OnDestroy()
{
}
void EventManager::OnLoadGlobelTask(CG_Cmd_GetGlobelTaskInfo* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	if ((pMsg->States & MsgBegin) != 0)
	{
		m_TaskGlobelMap.clear();
	}
	if (pMsg->Sum > 0)
	{
		for (WORD i = 0; i < pMsg->Sum; ++i)
		{
			HashMap<BYTE, tagTaskConfig>::iterator Iter = g_FishServer.GetFishConfig().GetTaskConfig().m_TaskMap.find(pMsg->Array[i]);
			if (Iter == g_FishServer.GetFishConfig().GetTaskConfig().m_TaskMap.end())
			{
				ASSERT(false);
				continue;
			}
			m_TaskGlobelMap.insert(std::map<BYTE, BYTE>::value_type(Iter->second.GroupID, pMsg->Array[i]));
		}
	}
}
bool EventManager::IsGlobleStop(BYTE TaskID)
{
	HashMap<BYTE, tagTaskConfig>::iterator Iter = g_FishServer.GetFishConfig().GetTaskConfig().m_TaskMap.find(TaskID);
	if (Iter == g_FishServer.GetFishConfig().GetTaskConfig().m_TaskMap.end())
		return false;
	std::map<BYTE, BYTE>::iterator IterFind = m_TaskGlobelMap.find(Iter->second.GroupID);
	if (IterFind == m_TaskGlobelMap.end())
		return false;
	return IterFind->second != TaskID;
}