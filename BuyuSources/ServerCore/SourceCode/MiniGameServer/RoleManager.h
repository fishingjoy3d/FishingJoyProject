#pragma once
class Role
{
public:
	Role(tagMiniGameRoleInfo* pInfo);
	virtual ~Role();

	DWORD	GetRoleID();
	DWORD	GetFaceID();
	DWORD   GetGlobelSum();
	DWORD   GetMadleSum();
	DWORD   GetCurrceySum();
	BYTE    GetVipLevel();
	LPTSTR  GetNickName();
	BYTE	GetGameConfigID();
	DWORD   GetParticularStates();
	void	SendDataToGameServer(NetCmd* pCmd);

	tagMiniGameRoleInfo& GetRoleInfo(){ return m_RoleInfo; };
	//1.
	void    OnChangeRoleGlobelSum(DWORD GlobelSum,bool IsServer = false);
	void	OnChangeRoleMadleSum(DWORD MadleSum);
	void    OnChangeRoleCurrceySum(DWORD CurrceySum);
	void    OnChangeRoleVipLevel(BYTE VipLevel);
	void    OnChangeRoleFaceID(DWORD FaceID);
	void	OnChangeRoleNickName(LPTSTR pNickName);
	void    OnChangeRoleParticularStates(DWORD States);
private:
	tagMiniGameRoleInfo			m_RoleInfo;
};
class RoleManager
{
public:
	RoleManager();
	virtual ~RoleManager();

	Role* QueryRole(DWORD dwUserID);

	void  OnRsgRole(tagMiniGameRoleInfo* pInfo,BYTE GameConfigID);
	void  OnUnRsgRole(DWORD dwUserID);
private:
	HashMap<DWORD, Role*>		m_RoleMap;
};