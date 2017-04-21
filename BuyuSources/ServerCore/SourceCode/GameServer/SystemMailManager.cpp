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
	g_FishServer.GetRoleManager()->TryGetSystemMail();
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
	g_FishServer.GetRoleManager()->TryGetSystemMail();
}

void SystemMailManager::TryGetSystemMail(CRoleEx* role)
{
	time_t cur_time = time(NULL);

	std::vector<int> VC_EmptyIndexs;
	bool modify = false;
	//const SystemMailManager::MAILS& SystemMails = g_FishServer.GetSystemMailManager().
	//if (SystemMails.size() == 0)
	//{
	//	return;
	//}	
	tagRoleInfo& role_info = role->GetRoleInfo();
	DWORD user_id = role->GetUserID();
	MAILS::const_iterator it = _OperatorMails.begin();
	for (; it != _OperatorMails.end(); ++it)
	{
		VC_EmptyIndexs.clear();
		const tagOperatorSystemMail& MailEntry = it->second;
		bool can_send_mail = true;
		if (cur_time >= MailEntry.BeginTime && cur_time <= MailEntry.EndTime)
		{

			for (size_t i = 0; i < MAX_SYSTEM_RECORD_COUT; i++)
			{
				if (role_info.SystemMailRecord[i].MailID == MailEntry.ID)
				{
					can_send_mail = false;
					break;
				}
				if (role_info.SystemMailRecord[i].MailID == 0 || role_info.SystemMailRecord[i].EndTime < cur_time)
				{
					role_info.SystemMailRecord[i].MailID = 0;
					VC_EmptyIndexs.push_back(i);

				}
			}
			if (can_send_mail == true && VC_EmptyIndexs.size() == 0)
			{
				break;
			}
			else if (can_send_mail)
			{
				role_info.SystemMailRecord[VC_EmptyIndexs[0]].MailID = MailEntry.ID;
				role_info.SystemMailRecord[VC_EmptyIndexs[0]].EndTime = MailEntry.EndTime;
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), MailEntry.Context, _tcslen(MailEntry.Context));
				MailInfo.RewardID = MailEntry.RewardID;
				MailInfo.RewardSum = MailEntry.RewardSum;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = cur_time;
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//ÏµÍ³·¢ËÍ
				MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
				DBR_Cmd_AddUserMail msg;
				SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
				msg.dwDestUserID = user_id;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToDB(&msg);
				modify = true;
			}
		}
	}

	if (modify)
	{
		DBR_Cmd_SaveSystemMailRecord msg;
		for (size_t i = 0; i < MAX_SYSTEM_RECORD_COUT; i++)
		{
			msg.MailRecord[i] = role_info.SystemMailRecord[i];
		}
		msg.UserID = user_id;
		SetMsgInfo(msg, DBR_SAVE_ROLE_SYSTEM_MAIL_RECORD, sizeof(DBR_Cmd_SaveSystemMailRecord));
		g_FishServer.SendNetCmdToDB(&msg);
	}

}