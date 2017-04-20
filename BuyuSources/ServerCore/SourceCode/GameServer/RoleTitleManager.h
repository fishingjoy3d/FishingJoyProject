//Íæ¼Ò³ÆºÅ¹ÜÀíÆ÷
#pragma once
#include "Stdafx.h"
#define Max_Title 255
class CRoleEx;
class RoleTitleManager
{
public:
	RoleTitleManager();
	virtual ~RoleTitleManager();

	bool OnInit(CRoleEx* pRole);
	bool OnLoadRoleTitle();
	void OnLoadRoleTitleResult(DBO_Cmd_LoadRoleTitle* pDB);

	void GetRoleTitleToClient();
	void SetRoleCurrTitleID(BYTE TitleID);

	bool AddRoleTitle(BYTE TitleID);
	bool DelRoleTitle(BYTE TitleID);

	bool IsLoadDB(){ return m_IsLoadDB; }

	void ResetClientInfo(){ m_IsSendToClient = false; }
private:
	bool				m_IsSendToClient;
	//BYTE				m_TitleArray[Max_Title];
	HashMap<BYTE, bool>  m_TitltMap;

	//vector<BYTE>		m_TitleVec;
	CRoleEx*			m_pRole;
	bool				m_IsLoadDB;
};