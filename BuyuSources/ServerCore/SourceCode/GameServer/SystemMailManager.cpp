#include "stdafx.h"
#include "SystemMailManager.h"
#include "FishServer.h"

SystemMailManager::SystemMailManager()
{
}


SystemMailManager::~SystemMailManager()
{
}


void SystemMailManager::AddNewMail(tagOperatorSystemMail& mail)
{
	_OperatorMails[mail.ID] = mail;
}

const SystemMailManager::MAILS* SystemMailManager::GetMails()
{
	return &_OperatorMails;
}

void SystemMailManager::OnConnectCentralServer()
{
	_OperatorMails.clear();
	GC_Cmd_GetAllOperatorSystemMail msg;
	SetMsgInfo(msg, GetMsgType(Main_OperatorSystemMail, GC_GetAllSystemMail), sizeof(msg));
	g_FishServer.SendNetCmdToCenter(&msg);
}

void SystemMailManager::OnLoadAllOperatorSystemMailByCentral(CG_Cmd_GetAllOperatorSystemMail* msg)
{
	_OperatorMails.clear();
	for (int i = 0; i < msg->sum; i++)
	{
		_OperatorMails[msg->mail[i].ID] = msg->mail[i];
	}
}