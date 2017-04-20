#include "Stdafx.h"
#include "RobotManager.h"
RobotManager::RobotManager()
{

}
RobotManager::~RobotManager()
{

}
void RobotManager::OnLoadRobotInfoBySum(DWORD BeginUserID, DWORD EndUserID, SendMsgToDBFun pSend)
{
	DBR_Cmd_LoadRobotInfo msg;
	SetMsgInfo(msg, DBR_LoadRobotInfo, sizeof(DBR_Cmd_LoadRobotInfo));
	msg.BeginUserID = BeginUserID;
	msg.EndUserID = EndUserID;
	pSend(&msg);
}
void RobotManager::OnLoadRobotInfoResult(DBO_Cmd_LoadRobotInfo* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	if (pMsg->States & MsgBegin)
	{
		m_UseRobotInfo.clear();
		m_NonUseRobotInfo.clear();
	}
	for (WORD i = 0; i < pMsg->Sum; ++i)
	{
		RobotUserInfo* pRobot = new RobotUserInfo(pMsg->Array[i]);
		m_NonUseRobotInfo.push_back(pRobot);
	}
	if (pMsg->States & MsgEnd)
	{
		Log("加载机器人数量:%u", m_NonUseRobotInfo.size());
	}
}
RobotUserInfo* RobotManager::GetNonUseRobot()
{
	if (m_NonUseRobotInfo.empty())
		return NULL;
	DWORD Index = RandUInt() % m_NonUseRobotInfo.size();
	RobotUserInfo* pInfo = m_NonUseRobotInfo[Index];
	m_NonUseRobotInfo[Index] = m_NonUseRobotInfo[m_NonUseRobotInfo.size() - 1];
	m_NonUseRobotInfo.pop_back();
	m_UseRobotInfo.insert(HashMap<DWORD, RobotUserInfo*>::value_type(pInfo->dwUserID,pInfo));
	return pInfo;
}
void RobotManager::OnNonUseRobot(DWORD dwUserID)
{
	HashMap<DWORD, RobotUserInfo*>::iterator Iter = m_UseRobotInfo.find(dwUserID);
	if (Iter == m_UseRobotInfo.end())
		return;
	RobotUserInfo* pInfo = Iter->second;
	m_UseRobotInfo.erase(Iter);
	m_NonUseRobotInfo.push_back(pInfo);
}
bool RobotManager::IsRobot(DWORD dwUserID)
{
	HashMap<DWORD, RobotUserInfo*>::iterator Iter = m_UseRobotInfo.find(dwUserID);
	if (Iter == m_UseRobotInfo.end())
		return false;
	else
		return true;
}