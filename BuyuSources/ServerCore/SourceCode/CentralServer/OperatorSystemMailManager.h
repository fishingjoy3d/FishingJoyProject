#pragma once
class OperatorSystemMailManager
{
public:
	typedef std::map<DWORD, tagOperatorSystemMail> MAILS;
public:
	OperatorSystemMailManager();
	virtual ~OperatorSystemMailManager();
	void AddNewMail(tagOperatorSystemMail& mail);
	void OnLoadSystemMailByDB();
	void OnLoadSystemMail(DBO_Cmd_LoadAllSystemMail* pMsg);
	const MAILS* GetMails();
protected:
	MAILS _OperatorMails;
};

