//”√ªßª∫¥Ê 
#pragma once
class RoleCache
{
public:
	RoleCache();
	virtual ~RoleCache();

	void OnAddRoleCache(DWORD dwUserID);
	void OnDleRoleCache(DWORD dwUserID);

	void UpdateByMin();

	bool IsOpenRoleCache();
private:
	HashMap<DWORD, DWORD> m_CacheRoleMap;
};