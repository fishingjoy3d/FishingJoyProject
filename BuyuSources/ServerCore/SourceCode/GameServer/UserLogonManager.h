#pragma once
//管理玩家登陆的控制
struct UserLogonInfo
{
	DWORD	dwUserID;
	DWORD	LogonTimeLog;
	DWORD	LogonOnlyID;
};
class UserLogonManager
{
public:
	UserLogonManager();
	virtual ~UserLogonManager();

	void OnUpdate(DWORD dwTimer);

	void OnAddUserLogonInfo(DWORD dwUserID, DWORD OnlyID);
	bool CheckUserLogonInfo(DWORD dwUserID, DWORD OnlyID);
private:
	HashMap<DWORD, UserLogonInfo*>	m_UserLogonMap;
};