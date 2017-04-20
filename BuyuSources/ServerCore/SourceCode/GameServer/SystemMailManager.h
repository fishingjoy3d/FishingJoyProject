#pragma once
class SystemMailManager
{
public:
	typedef std::map<DWORD, tagOperatorSystemMail> MAILS;
public:
	SystemMailManager();
	virtual ~SystemMailManager();
	void AddNewMail(tagOperatorSystemMail& mail);
	void OnConnectCentralServer();
	void OnLoadAllOperatorSystemMailByCentral(CG_Cmd_GetAllOperatorSystemMail* msg);
	const MAILS* GetMails();
protected:
	MAILS _OperatorMails;
};

