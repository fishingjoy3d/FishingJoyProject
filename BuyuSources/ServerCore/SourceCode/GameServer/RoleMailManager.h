//玩家邮件处理
#pragma once
#include "Stdafx.h"
#include <list>
class CRoleEx;
class RoleManager;
class RoleMailManager  //管理玩家全部的邮件系统 我们可以开始处理了
{
public:
	RoleMailManager();
	virtual ~RoleMailManager();

	bool OnInit(CRoleEx* pUser, RoleManager* pManager);//初始化
	bool OnLoadUserMailByPage();//读取指定页数的邮件
	bool OnLoadUserMailByPageResult(DBO_Cmd_LoadUserMail* pDB);
	//void OnLoadUserMailFinish();
	bool OnGetAllUserMail();
	bool OnGetUserMailContext(DWORD MailID);
	bool OnGetUserMailItem(DWORD MailID);
	bool OnDelUserMail(DWORD MailID);
	bool OnBeAddUserMail(tagRoleMail* pMail);
	bool OnBeAddUserMailResult(DBO_Cmd_AddUserMail* pMsg);
	bool OnSendUserMail(CL_Cmd_SendUserMail* pMsg);

	//玩家邮件管理器 只保存七天以内的邮件
	void OnDayChange();

	bool IsLoadDB(){ return m_IsLoadDB; }

	void ResetClientInfo(){ m_IsLoadToClient = false; }

	bool  GetMailMessageStates();
private:
	void OnSendAddMailToClient(tagRoleMail* pMail);
private:
	//std::list<tagRoleMail>		m_RoleMailVec;//玩家邮件的集合 使用List集合  将全部的邮件按顺序读取进来
	std::list<tagRoleMail*>		m_RoleMailVec;//邮件的集合
	//属性
	bool						m_IsLoadToClient;
	RoleManager*				m_RoleManager;
	CRoleEx*					m_pUser;
	bool						m_IsLoadDB;

	BYTE						m_ClientMailSum;
};