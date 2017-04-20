#include "Stdafx.h"
#include "ExChangeManager.h"
#include "FishServer.h"
#include "..\CommonFile\DBLogManager.h"
extern void SendLogDB(NetCmd* pCmd);
ExChangeManager::ExChangeManager()
{

}
ExChangeManager::~ExChangeManager()
{

}
void ExChangeManager::OnUseExChangeCode(CRoleEx* pRole,CL_Cmd_RoleUseExChangeCode* pMsg)
{
	if (!pMsg || !pRole)
	{
		ASSERT(false);
		return;
	}
	//向数据库进行查询 兑换码对应的ID 
	DBR_Cmd_QueryExChange msg;
	SetMsgInfo(msg, DBR_QueryExChange, sizeof(DBR_Cmd_QueryExChange));
	msg.dwUserID = pRole->GetUserID();
	TCHARCopy(msg.ExChangeCode, CountArray(msg.ExChangeCode), pMsg->ExChangeCode, _tcslen(pMsg->ExChangeCode));
	g_FishServer.SendNetCmdToDB(&msg);
}
void ExChangeManager::OnUseExChangeCodeDBResult(DBO_Cmd_QueryExChange* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	CRoleEx* pRole = g_FishServer.GetRoleManager()->QueryUser(pMsg->dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	LC_Cmd_RoleUseExChangeCode msgRole;
	SetMsgInfo(msgRole, GetMsgType(Main_Exchange, LC_RoleUseExChangeCode), sizeof(LC_Cmd_RoleUseExChangeCode));
	msgRole.Result = false;
	msgRole.ID = 0;
	if (pMsg->ID != 0)
	{
		BYTE ID = pMsg->ID;
		HashMap<BYTE, tagExChange>::iterator Iter = g_FishServer.GetFishConfig().GetExChangeMap().m_ExChangeMap.find(ID);
		if (Iter != g_FishServer.GetFishConfig().GetExChangeMap().m_ExChangeMap.end())
		{
			if ((pRole->GetChannelID() == Iter->second.ChannelID || Iter->second.ChannelID == 0) && (Iter->second.TypeID == 0 || ((pRole->GetRoleInfo().ExChangeStates & Iter->second.TypeID) == 0)))
			{
				DWORD States = (pRole->GetRoleInfo().ExChangeStates | Iter->second.TypeID);
				pRole->ChangeRoleExChangeStates(States);

				//发送奖励
				pRole->OnAddRoleRewardByRewardID(Iter->second.RewardID, TEXT("兑换码获取奖励"));

				//删除兑换码
				DBR_Cmd_DelExChange msg;
				SetMsgInfo(msg, DBR_DelExChange, sizeof(DBR_Cmd_DelExChange));
				msg.dwUserID = pRole->GetUserID();
				TCHARCopy(msg.ExChangeCode, CountArray(msg.ExChangeCode), pMsg->ExChangeCode, _tcslen(pMsg->ExChangeCode));
				g_FishServer.SendNetCmdToSaveDB(&msg);

				//记录到Log数据库去
				g_DBLogManager.LogExChangeInfoToDB(pMsg->dwUserID, pMsg->ID, pMsg->ExChangeCode, SendLogDB);


				msgRole.Result = true;
				msgRole.ID = ID;
			}
			else
			{
				msgRole.Result = false;
				msgRole.ID = ID;
			}
		}
	}
	pRole->SendDataToClient(&msgRole);
}

