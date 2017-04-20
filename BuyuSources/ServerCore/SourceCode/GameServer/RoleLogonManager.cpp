#include "Stdafx.h"
#include "RoleLogonManager.h"
RoleLogonManager::RoleLogonManager()
{
	m_RoleOnlyMap.clear();
}
RoleLogonManager::~RoleLogonManager()
{

}
DWORD RoleLogonManager::OnAddRoleOnlyInfo(DWORD UserID)
{
	HashMap<DWORD, DWORD>::iterator Iter = m_RoleOnlyMap.find(UserID);
	DWORD RandID = RandUInt();
	if (Iter == m_RoleOnlyMap.end())
	{
		m_RoleOnlyMap.insert(HashMap<DWORD, DWORD>::value_type(UserID, RandID));
		return RandID;
	}
	else
	{
		Iter->second = RandID;
		return RandID;
	}
}
void RoleLogonManager::OnDleRoleOnlyInfo(DWORD UserID)
{
	m_RoleOnlyMap.erase(UserID);
}
bool RoleLogonManager::CheckRoleOnlyID(DWORD UserID, DWORD RandID)
{
	HashMap<DWORD, DWORD>::iterator Iter = m_RoleOnlyMap.find(UserID);
	if (Iter == m_RoleOnlyMap.end())
	{
		return false;
	}
	else
	{
		 return Iter->second == RandID;
	}
}