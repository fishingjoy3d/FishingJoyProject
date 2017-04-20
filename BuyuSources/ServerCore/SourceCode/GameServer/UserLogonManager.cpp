#include "stdafx.h"
#include "UserLogonManager.h"
UserLogonManager::UserLogonManager()
{
	m_UserLogonMap.clear();
}
UserLogonManager::~UserLogonManager()
{
	if (!m_UserLogonMap.empty())
	{
		HashMap<DWORD, UserLogonInfo*>::iterator Iter = m_UserLogonMap.begin();
		for (; Iter != m_UserLogonMap.end();)
		{
			delete Iter->second;
		}
		m_UserLogonMap.clear();
	}
}
void UserLogonManager::OnUpdate(DWORD dwTimer)
{
	if (m_UserLogonMap.empty())
		return;
	HashMap<DWORD, UserLogonInfo*>::iterator Iter = m_UserLogonMap.begin();
	for (; Iter != m_UserLogonMap.end(); )
	{
		if (dwTimer >= Iter->second->LogonTimeLog && dwTimer - Iter->second->LogonTimeLog >= 60000)
		{
			delete Iter->second;
			Iter = m_UserLogonMap.erase(Iter);
		}
		else
			++Iter;
	}
}
void UserLogonManager::OnAddUserLogonInfo(DWORD dwUserID, DWORD OnlyID)
{
	HashMap<DWORD, UserLogonInfo*>::iterator Iter = m_UserLogonMap.find(dwUserID);
	if (Iter == m_UserLogonMap.end())
	{
		UserLogonInfo* pInfo = new UserLogonInfo;
		pInfo->dwUserID = dwUserID;
		pInfo->LogonOnlyID = OnlyID;
		pInfo->LogonTimeLog = timeGetTime();
		m_UserLogonMap.insert(HashMap<DWORD, UserLogonInfo*>::value_type(dwUserID, pInfo));
	}
	else
	{
		Iter->second->LogonTimeLog = timeGetTime();
		Iter->second->LogonOnlyID = OnlyID;
	}
}
bool UserLogonManager::CheckUserLogonInfo(DWORD dwUserID, DWORD OnlyID)
{
	HashMap<DWORD, UserLogonInfo*>::iterator Iter = m_UserLogonMap.find(dwUserID);
	if (Iter == m_UserLogonMap.end())
	{
		ASSERT(false);
		return false;
	}
	if (Iter->second->LogonOnlyID != OnlyID || timeGetTime() - Iter->second->LogonTimeLog >= 60000)
	{
		ASSERT(false);
		return false;
	}
	delete Iter->second;
	m_UserLogonMap.erase(Iter);
	return true;
}