#include "Stdafx.h"
#include "RoleTitleManager.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleTitleManager::RoleTitleManager()
{
	m_IsSendToClient = false;
	m_IsLoadDB = false;
	m_TitltMap.clear();
}
RoleTitleManager::~RoleTitleManager()
{

}
bool RoleTitleManager::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	return OnLoadRoleTitle();
}
bool RoleTitleManager::OnLoadRoleTitle()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	/*DBR_Cmd_LoadRoleTitle msg;
	SetMsgInfo(msg,DBR_LoadRoleTitle, sizeof(DBR_Cmd_LoadRoleTitle));
	msg.dwUserID = m_pRole->GetUserID();
	g_FishServer.SendNetCmdToDB(&msg);*/
	return true;
}
void RoleTitleManager::OnLoadRoleTitleResult(DBO_Cmd_LoadRoleTitle* pDB)
{
	if (!pDB || !m_pRole)
	{
		ASSERT(false);
		return;
	}
	if ((pDB->States & MsgBegin) != 0)
	{
		m_TitltMap.clear();
	}
	for (WORD i = 0; i < pDB->Sum; ++i)
	{
		if (pDB->Array[i] >= Max_Title)
		{
			ASSERT(false);
			continue;
		}
		if (!g_FishServer.GetFishConfig().TitleIsExists(pDB->Array[i]))
			continue;
		m_TitltMap.insert(HashMap<BYTE, bool>::value_type(pDB->Array[i], true));
	}
	if ((pDB->States & MsgEnd) != 0)
	{
		m_IsLoadDB = true;
		m_pRole->IsLoadFinish();
	}
}
bool RoleTitleManager::AddRoleTitle(BYTE TitleID)
{
	if (!m_pRole || TitleID >= Max_Title)
	{
		ASSERT(false);
		return false;
	}
	if (!g_FishServer.GetFishConfig().TitleIsExists(TitleID))
		return false;
	HashMap<BYTE, bool>::iterator Iter = m_TitltMap.find(TitleID);
	if (Iter != m_TitltMap.end())
		return false;
	m_TitltMap.insert(HashMap<BYTE, bool>::value_type(TitleID, true));
	DBR_Cmd_AddRoleTitle msg;
	SetMsgInfo(msg,DBR_AddRoleTitle, sizeof(DBR_Cmd_AddRoleTitle));
	msg.dwUserID = m_pRole->GetUserID();
	msg.TitleID = TitleID;
	g_FishServer.SendNetCmdToSaveDB(&msg);
	if (m_IsSendToClient)
	{
		LC_Cmd_AddRoleTitle msgClient;
		SetMsgInfo(msgClient,GetMsgType(Main_Title, LC_AddRoleTitle), sizeof(LC_Cmd_AddRoleTitle));
		msgClient.AddTitleID = TitleID;
		m_pRole->SendDataToClient(&msgClient);
	}
	return true;
}
bool RoleTitleManager::DelRoleTitle(BYTE TitleID)
{
	if (!m_pRole || TitleID >= Max_Title)
	{
		ASSERT(false);
		return false;
	}
	if (!g_FishServer.GetFishConfig().TitleIsExists(TitleID))
		return false;
	HashMap<BYTE, bool>::iterator Iter = m_TitltMap.find(TitleID);
	if (Iter == m_TitltMap.end())
		return false;
	m_TitltMap.erase(Iter);
	DBR_Cmd_DelRoleTitle msg;
	SetMsgInfo(msg,DBR_DelRoleTitle, sizeof(DBR_Cmd_DelRoleTitle));
	msg.dwUserID = m_pRole->GetUserID();
	msg.TitleID = TitleID;
	g_FishServer.SendNetCmdToSaveDB(&msg);

	if (m_IsSendToClient)
	{
		LC_Cmd_DelRoleTitle msgClient;
		SetMsgInfo(msgClient,GetMsgType(Main_Title, LC_DelRoleTitle), sizeof(LC_Cmd_DelRoleTitle));
		msgClient.DelTitleID = TitleID;
		m_pRole->SendDataToClient(&msgClient);
	}
	return true;
}
void RoleTitleManager::GetRoleTitleToClient()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	DWORD PageSize = sizeof(LC_Cmd_LoadRoleTitle)+sizeof(BYTE)*(m_TitltMap.size() - 1);
	LC_Cmd_LoadRoleTitle * msg = (LC_Cmd_LoadRoleTitle*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Title, LC_LoadRoleTitle));
	HashMap<BYTE, bool>::iterator Iter = m_TitltMap.begin();
	for (WORD i = 0; Iter != m_TitltMap.end(); ++Iter,++i)//便利玩家身上的全部的称号
	{
		msg->Array[i] = Iter->first;
	}
	std::vector<LC_Cmd_LoadRoleTitle*> pVec;
	SqlitMsg(msg, PageSize, m_TitltMap.size(),false, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_LoadRoleTitle*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			m_pRole->SendDataToClient(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
	m_IsSendToClient = true;
}
void RoleTitleManager::SetRoleCurrTitleID(BYTE TitleID)
{
	//设置玩家的称号ID 
	//1.判断玩家是否有称号
	if (TitleID >= Max_Title || !m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (!g_FishServer.GetFishConfig().TitleIsExists(TitleID))
		return;
	HashMap<BYTE, bool>::iterator Iter = m_TitltMap.find(TitleID);
	if (Iter == m_TitltMap.end())
		return;
	m_pRole->ChangeRoleTitle(TitleID);
}