#pragma once
#include "Stdafx.h"
class CRoleEx;
class RoleIDEntity
{
public:
	RoleIDEntity();
	virtual ~RoleIDEntity();
	bool OnInit(CRoleEx* pRole);
	bool OnLoadRoleAddressInfo();
	void OnLoadRoleAddressInfoResult(DBO_Cmd_LoadRoleEntity* pDB);
	void GetRoleAddressInfoToClient();
	//修改
	void OnChangeRoleEntityItemUseAddress(LPTSTR pAddress);
	void OnChangeRoleEntityItemUseName(LPTSTR pName);
	void OnChangeRoleEntityItemUsePhone(unsigned __int64 pPhone);
	void OnChangeRoleName(LPTSTR pName);
	void OnChangeRolePhone(unsigned __int64 pPhone);
	void OnChangeRoleEmail(LPTSTR pEmail);
	void OnChangeRoleEntityID(LPTSTR pEntityID);

	bool IsCanUserEntity();

	tagRoleAddressInfo& GetEntityInfo(){ return m_RoleInfo; }

	bool IsLoadDB(){ return m_IsLoadDB; }

	void OnSaveInfoToDB(){ OnSaveRoleAddress(); }

	void ResetClientInfo(){ m_IsSendClient = false; }
private:
	void OnSaveRoleAddress();
private:
	//bool					m_IsNeedSaveDB;
	CRoleEx*				m_pRole;
	tagRoleAddressInfo		m_RoleInfo;//玩家现实的信息
	bool					m_IsLoadDB;
	bool					m_IsSendClient;
};