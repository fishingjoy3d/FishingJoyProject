#pragma once
#include "Stdafx.h"
class CRoleEx;
class RoleManager
{
public:
	RoleManager();
	virtual ~RoleManager();
	void Destroy();

	bool OnInit();
	void OnUpdate(DWORD dwTimer);
	void OnUpdateRoleTask();
	CRoleEx* QueryUser(DWORD dwUserID);
	CRoleEx* QueryUserByGameID(DWORD GameID);
	CRoleEx* QuertUserBySocketID(DWORD dwSocketID);
	void  OnDelUser(DWORD dwUserID,bool IsWriteSaveInfo,bool IsLeaveCenter);
	void OnDelUserResult(DBO_Cmd_SaveRoleAllInfo* pResult);
	bool CreateRole(tagRoleInfo* pUserInfo, tagRoleServerInfo* pUserServerInfo, DWORD dwSocketID, time_t pTime, bool LogobByGameServer, bool IsRobot);

	HashMap<DWORD, CRoleEx*>& GetAllRole(){ return m_RoleMap; }

	void OnUpdateByMin(bool IsHourChange, bool IsDayChange, bool IsMonthChange, bool IsYearChange);//每分钟更新
	void ChangeRoleSocket(CRoleEx* pRole, DWORD SocketID);
	//void SetRoleLeave(CRoleEx* pRole);
	//void SetRoleOnLine(CRoleEx* pRole, DWORD SocketID);

	void SendSystemMailToUser(DWORD dwUserID, LPTSTR pContext, WORD RewardID);

	void OnSaveInfoToDB();
	//void OnGetRoleAchievementIndex();

	void OnKickAllUser();
private:
	HashMap<DWORD, CRoleEx*>	m_RoleMap;
	HashMap<DWORD, CRoleEx*>	m_RoleSocketMap;
	HashMap<DWORD, CRoleEx*>    m_RoleGameMap;
};
