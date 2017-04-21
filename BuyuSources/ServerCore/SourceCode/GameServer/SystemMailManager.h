#pragma once
class  CRoleEx;

typedef std::map<DWORD, tagOperatorSystemMail> MAILS;
class SystemMailManager
{
public:
public:
	SystemMailManager();
	virtual ~SystemMailManager();
	void AddNewMail(tagOperatorSystemMail& mail);
	void OnConnectCentralServer();
	void OnLoadAllOperatorSystemMailByCentral(CG_Cmd_GetAllOperatorSystemMail* msg);
	void TryGetSystemMail(CRoleEx* role);
protected:
	MAILS _OperatorMails;
	

	
};

