#pragma once
#include "Stdafx.h"
#include "RoleRelationManager.h"
class CenterRole
{
public:
	CenterRole();
	virtual ~CenterRole();

	void     OnInit(BYTE dwSocketID, tagCenterRoleInfo* pInfo, CenterRoleManager* pManager,bool IsRobot);

	DWORD	 GetRoleID(){ return m_RoleInfo.dwUserID; }
	BYTE	 GetSocketID(){ return m_dwSocketID; }
	void     SetSocketID(BYTE dwSocketID){ m_dwSocketID = dwSocketID; }
	tagCenterRoleInfo& GetRoleInfo(){ return m_RoleInfo; }
	RoleRelationManager& GetRelationManager(){ return m_RelationManager; }

	void     OnRoleLeave();
	void	 OnRoleEnterFinish();

	void	SendDataToGameServer(NetCmd* pCmd);

	void    OnChangeRoleLevel(WORD wLevel);
	//void	OnChangeRoleExp(DWORD dwExp);
	void	OnChangeRoleNickName(LPTSTR pNickName);
	void	OnChangeRoleFaceID(DWORD dwFaceID);
	void	OnChangeRoleGender(bool bGender);
	void	OnChangeRoleTitle(BYTE TitleID);
	void    OnChangeRoleAchievementPoint(DWORD dwAchievementPoint);
	void	OnChangeRoleCharmValue(DWORD CharmInfo[MAX_CHARM_ITEMSUM]);
	void	OnChangeRoleClientIP(DWORD ClientIP);
	void	OnChangeRoleIsShowIpAddress(bool States);
	void	OnChangeRoleIsOnline(bool States);
	void	OnChangeRoleVipLevel(BYTE VipLevel);
	void	OnChangeRoleIsInMonthCard(bool IsInMonthCard);
	void    OnChangeRoleParticularStates(DWORD States);

	bool	IsRobot(){ return m_IsRobot; }

private:
	CenterRoleManager*			m_pRoleManager;
	BYTE						m_dwSocketID;//玩家对于的SocketID
	tagCenterRoleInfo			m_RoleInfo;//玩家的中央服务器上的信息
	RoleRelationManager			m_RelationManager;
	bool						m_IsRobot;
};
class CenterRoleManager
{
public:
	CenterRoleManager();
	virtual ~CenterRoleManager();

	void OnInit();
	void Destroy();
	bool OnCreateCenterRole(BYTE dwSocketID, tagCenterRoleInfo* pInfo, bool IsRobot);
	bool OnDelCenterRole(DWORD dwUserID);
	bool OnPlazaLeave(BYTE dwSocketID);
	void SetCenterUserFinish(DWORD dwUserID);
	const HashMap<DWORD, CenterRole*>* GetOnlineRole() { return &m_RoleMap; }
	CenterRole* QueryCenterUser(DWORD dwUserID);

	DWORD	GetOnLinePlayerSum(){ return m_RoleMap.size(); }

	DWORD	GetLeaveOnlinePlayerSum(){ return m_LeaveOnlineUser; }
	void	SetAddOrDelLeaveOnlineUser(bool States) { m_LeaveOnlineUser += (States ? 1 : -1); }
private:
	HashMap<DWORD, CenterRole*>		m_RoleMap;
	DWORD							m_LeaveOnlineUser;
};