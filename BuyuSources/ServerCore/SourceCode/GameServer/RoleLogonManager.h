//玩家重复登陆管理器
#pragma once
#include "Stdafx.h"
class RoleLogonManager
{
public:
	RoleLogonManager();
	virtual ~RoleLogonManager();

	DWORD OnAddRoleOnlyInfo(DWORD UserID);//添加玩家重复登陆的凭证
	bool CheckRoleOnlyID(DWORD UserID, DWORD RandID);
	void OnDleRoleOnlyInfo(DWORD UserID);
private:
	HashMap<DWORD, DWORD>  m_RoleOnlyMap;
};