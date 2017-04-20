#include "Stdafx.h"
#include "RoleIdEntity.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleIDEntity::RoleIDEntity()
{
	m_pRole = NULL;
	m_IsLoadDB = false;
	ZeroMemory(&m_RoleInfo, sizeof(m_RoleInfo));
	m_IsSendClient = false;
}
RoleIDEntity::~RoleIDEntity()
{
}
bool RoleIDEntity::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	return OnLoadRoleAddressInfo();
}
bool RoleIDEntity::OnLoadRoleAddressInfo()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	/*DBR_Cmd_LoadRoleEntity msg;
	SetMsgInfo(msg,DBR_LoadRoleEntity, sizeof(DBR_Cmd_LoadRoleEntity));
	msg.dwUserID = m_pRole->GetUserID();
	g_FishServer.SendNetCmdToDB(&msg);*/
	return true;
}
void RoleIDEntity::OnLoadRoleAddressInfoResult(DBO_Cmd_LoadRoleEntity* pDB)
{
	if (!pDB || !m_pRole || pDB->dwUserID != m_pRole->GetUserID())
	{
		ASSERT(false);
		return;
	}
	m_RoleInfo = pDB->RoleInfo;
	m_IsLoadDB = true;
	m_pRole->IsLoadFinish();
}
void RoleIDEntity::GetRoleAddressInfoToClient()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//发送命令到客户端去
	LC_Cmd_LoadUserEntity msg;
	SetMsgInfo(msg,GetMsgType(Main_Entity, LC_LoadUserEntity), sizeof(LC_Cmd_LoadUserEntity));
	msg.UserEntityInfo = m_RoleInfo;
	m_pRole->SendDataToClient(&msg);

	m_IsSendClient = true;
}
void RoleIDEntity::OnChangeRoleEntityItemUseAddress(LPTSTR pAddress)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServer.GetFishConfig().CheckStringIsError(pAddress, MIN_ADDRESS_LENGTH, MAX_ADDRESS_LENGTH, SCT_Normal))
	{
		ASSERT(false);
		return;
	}
	TCHARCopy(m_RoleInfo.EntityItemUseAddres, CountArray(m_RoleInfo.EntityItemUseAddres), pAddress, _tcslen(pAddress));

	LC_Cmd_ChangeRoleEntityItemUseAddress msg;
	SetMsgInfo(msg, GetMsgType(Main_Entity, LC_ChangeRoleEntityItemUseAddress), sizeof(LC_Cmd_ChangeRoleEntityItemUseAddress));
	TCHARCopy(msg.EntityItemUseAddres, CountArray(msg.EntityItemUseAddres), pAddress, _tcslen(pAddress));
	m_pRole->SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleEntityItemUseAddress msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleEntityItemUseAddress, sizeof(DBR_Cmd_SaveRoleEntityItemUseAddress));
	msgDB.dwUserID = m_pRole->GetUserID();
	TCHARCopy(msgDB.EntityItemUseAddres, CountArray(msgDB.EntityItemUseAddres), pAddress, _tcslen(pAddress));
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
}
void RoleIDEntity::OnChangeRoleEntityItemUseName(LPTSTR pName)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServer.GetFishConfig().CheckStringIsError(pName, MIN_NAME_LENTH, MAX_NAME_LENTH, SCT_Normal))
	{
		ASSERT(false);
		return;
	}
	TCHARCopy(m_RoleInfo.EntityItemUseName, CountArray(m_RoleInfo.EntityItemUseName), pName, _tcslen(pName));

	LC_Cmd_ChangeRoleEntityItemUseName msg;
	SetMsgInfo(msg, GetMsgType(Main_Entity, LC_ChangeRoleEntityItemUseName), sizeof(LC_Cmd_ChangeRoleEntityItemUseName));
	TCHARCopy(msg.EntityItemUseName, CountArray(msg.EntityItemUseName), pName, _tcslen(pName));
	m_pRole->SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleEntityItemUseName msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleEntityItemUseName, sizeof(DBR_Cmd_SaveRoleEntityItemUseName));
	msgDB.dwUserID = m_pRole->GetUserID();
	TCHARCopy(msgDB.EntityItemUseName, CountArray(msgDB.EntityItemUseName), pName, _tcslen(pName));
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
}
void RoleIDEntity::OnChangeRoleEntityItemUsePhone(unsigned __int64 pPhone)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!PhoneIsCanUse(pPhone))
	{
		ASSERT(false);
		return;
	}
	m_RoleInfo.EntityItemUsePhone = pPhone;

	LC_Cmd_ChangeRoleEntityItemUsePhone msg;
	SetMsgInfo(msg, GetMsgType(Main_Entity, LC_ChangeRoleEntityItemUsePhone), sizeof(LC_Cmd_ChangeRoleEntityItemUsePhone));
	msg.EntityItemUsePhone = pPhone;
	m_pRole->SendDataToClient(&msg);

	DBR_Cmd_SaveRoleEntityItemUsePhone msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleEntityItemUsePhone, sizeof(DBR_Cmd_SaveRoleEntityItemUsePhone));
	msgDB.dwUserID = m_pRole->GetUserID();
	msgDB.EntityItemUsePhone = pPhone;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
}
void RoleIDEntity::OnChangeRoleName(LPTSTR pName)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServer.GetFishConfig().CheckStringIsError(pName, MIN_NAME_LENTH, MAX_NAME_LENTH, SCT_Normal))
	{
		ASSERT(false);
		return;
	}

	TCHARCopy(m_RoleInfo.Name, CountArray(m_RoleInfo.Name), pName, _tcslen(pName));

	LC_Cmd_ChangeRoleName msg;
	SetMsgInfo(msg,GetMsgType(Main_Entity, LC_ChangeRoleName), sizeof(LC_Cmd_ChangeRoleName));
	TCHARCopy(msg.Name, CountArray(msg.Name), pName, _tcslen(pName));
	m_pRole->SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleEntityName msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleEntityName, sizeof(DBR_Cmd_SaveRoleEntityName));
	msgDB.dwUserID = m_pRole->GetUserID();
	TCHARCopy(msgDB.Name, CountArray(msgDB.Name), pName, _tcslen(pName));
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
}
void RoleIDEntity::OnChangeRolePhone(unsigned __int64 pPhone)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!PhoneIsCanUse(pPhone))
	{
		ASSERT(false);
		return;
	}
	m_RoleInfo.Phone = pPhone;

	LC_Cmd_ChangeRolePhone msg;
	SetMsgInfo(msg,GetMsgType(Main_Entity, LC_ChangeRolePhone), sizeof(LC_Cmd_ChangeRolePhone));
	msg.Phone = pPhone;
	m_pRole->SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleEntityPhone msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleEntityPhone, sizeof(DBR_Cmd_SaveRoleEntityPhone));
	msgDB.dwUserID = m_pRole->GetUserID();
	msgDB.Phone = pPhone;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
}
void RoleIDEntity::OnChangeRoleEmail(LPTSTR pEmail)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServer.GetFishConfig().CheckStringIsError(pEmail, MIN_EMAIL_LENGTH, MAX_EMAIL_LENGTH,SCT_Normal))
	{
		ASSERT(false);
		return;
	}
	TCHARCopy(m_RoleInfo.Email, CountArray(m_RoleInfo.Email), pEmail, _tcslen(pEmail));

	LC_Cmd_ChangeRoleEmail msg;
	SetMsgInfo(msg,GetMsgType(Main_Entity, LC_ChangeRoleEmail), sizeof(LC_Cmd_ChangeRoleEmail));
	TCHARCopy(msg.Email, CountArray(msg.Email), pEmail, _tcslen(pEmail));
	m_pRole->SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleEntityEmail msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleEntityEmail, sizeof(DBR_Cmd_SaveRoleEntityEmail));
	msgDB.dwUserID = m_pRole->GetUserID();
	TCHARCopy(msgDB.Email, CountArray(msgDB.Email), pEmail, _tcslen(pEmail));
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
}
void RoleIDEntity::OnChangeRoleEntityID(LPTSTR pEntityID)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServer.GetFishConfig().CheckStringIsError(pEntityID, MIN_IDENTITY_LENGTH, MAX_IDENTITY_LENGTH, SCT_Normal))
	{
		ASSERT(false);
		return;
	}
	TCHARCopy(m_RoleInfo.IdentityID, CountArray(m_RoleInfo.IdentityID), pEntityID, _tcslen(pEntityID));

	LC_Cmd_ChangeRoleEntityID msg;
	SetMsgInfo(msg,GetMsgType(Main_Entity, LC_ChangeRoleEntityID), sizeof(LC_Cmd_ChangeRoleEntityID));
	TCHARCopy(msg.IdentityID, CountArray(msg.IdentityID), pEntityID, _tcslen(pEntityID));
	m_pRole->SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleEntityIdentityID msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleEntityIdentityID, sizeof(DBR_Cmd_SaveRoleEntityIdentityID));
	msgDB.dwUserID = m_pRole->GetUserID();
	TCHARCopy(msgDB.IdentityID, CountArray(msgDB.IdentityID), pEntityID, _tcslen(pEntityID));
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
}
//void RoleIDEntity::OnSaveRoleAddress()
//{
//	if (!m_pRole)
//	{
//		ASSERT(false);
//		return;
//	}
//	if (!m_IsNeedSaveDB)
//		return;
//	m_IsNeedSaveDB = false;
//	DBR_Cmd_SaveRoleEntity msg;
//	SetMsgInfo(msg,DBR_SaveRoleEntity, sizeof(DBR_Cmd_SaveRoleEntity));
//	msg.dwUserID = m_pRole->GetUserID();
//	msg.RoleInfo = m_RoleInfo;
//	g_FishServer.SendNetCmdToDB(&msg);
//}
bool RoleIDEntity::IsCanUserEntity()
{
	if (CountArray(m_RoleInfo.EntityItemUseAddres) == 0 ||
		CountArray(m_RoleInfo.EntityItemUseName) == 0 ||
		CountArray(m_RoleInfo.IdentityID) == 0 ||
		!PhoneIsCanUse(m_RoleInfo.EntityItemUsePhone))
		return false;
	else
		return true;
}