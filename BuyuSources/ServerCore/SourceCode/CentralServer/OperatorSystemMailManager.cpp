#include "stdafx.h"
#include "OperatorSystemMailManager.h"
#include "FishServer.h"

OperatorSystemMailManager::OperatorSystemMailManager()
{
}


OperatorSystemMailManager::~OperatorSystemMailManager()
{
}


void OperatorSystemMailManager::AddNewMail(tagOperatorSystemMail& mail)
{
	if (mail.ID != 0)
	{
		MAILS::iterator it = _OperatorMails.find(mail.ID);
		if (it != _OperatorMails.end())
		{
			_OperatorMails[mail.ID] = mail;
			CG_Cmd_AddNewOperatorMail msg;
			msg.mail = mail;
			SetMsgInfo(msg, GetMsgType(Main_OperatorSystemMail, CG_AddOperatorSystemMail), sizeof(msg));
			g_FishServer.SendNetCmdToAllGameServer(&msg);
		}
	}
}

void OperatorSystemMailManager::OnLoadSystemMailByDB()
{
	DBR_Cmd_LoadAllSystemMail msg;
	SetMsgInfo(msg, DBR_LOAD_OPERATOR_SYSTEM_MAIL, sizeof(DBR_Cmd_LoadAllSystemMail));
	g_FishServer.SendNetCmdToDB(&msg);

}


void OperatorSystemMailManager::OnLoadSystemMail(DBO_Cmd_LoadAllSystemMail* pMsg)
{
	_OperatorMails.clear();
	for (size_t i = 0; i < pMsg->Sum; i++)
	{
		_OperatorMails[pMsg->mail[i].ID] = pMsg->mail[i];
	}
}

const OperatorSystemMailManager::MAILS* OperatorSystemMailManager::GetMails()
{
	return &_OperatorMails;
}