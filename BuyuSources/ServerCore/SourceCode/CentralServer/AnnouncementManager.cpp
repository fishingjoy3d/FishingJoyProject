#include "Stdafx.h"
#include "AnnouncementManager.h"
#include "FishServer.h"
AnnouncementManager::AnnouncementManager()
{
	m_IsFinish = false;
	m_AnnouncementList.clear();
	m_GameServerVec.clear();
}
AnnouncementManager::~AnnouncementManager()
{
	m_IsFinish = false;
	m_AnnouncementList.clear();
	m_GameServerVec.clear();
}
void AnnouncementManager::OnLoadAllAnnouncementByDB()
{
	//发送命令到数据库去
	m_IsFinish = false;
	m_AnnouncementList.clear();
	
	DBR_Cmd_LoadAllAnnouncement msg;
	SetMsgInfo(msg, DBR_LoadAllAnnouncement, sizeof(DBR_Cmd_LoadAllAnnouncement));
	g_FishServer.SendNetCmdToDB(&msg);
}
void AnnouncementManager::OnLoadAllAnnouncementByDBResult(DBO_Cmd_LoadALlAnnouncement* pMsg)
{
	//接收数据库返回的消息
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	if ((pMsg->States & MsgBegin) != 0)
	{
		m_AnnouncementList.clear();
	}
	for (BYTE i = 0; i < pMsg->Sum; ++i)
	{
		m_AnnouncementList.push_front(pMsg->Array[i]);
	}
	if ((pMsg->States & MsgEnd) != 0)
	{
		m_IsFinish = true;
		while (m_AnnouncementList.size() > g_FishServer.GetFishConfig().GetSystemConfig().AnnouncementSum)
		{
			m_AnnouncementList.pop_back();//移除最后面的一个公告
		}
		if (!m_GameServerVec.empty())
		{
			std::vector<BYTE>::iterator Iter = m_GameServerVec.begin();
			for (; Iter != m_GameServerVec.end(); ++Iter)
			{
				OnSendAllAnnouncementToGameServer(*Iter);
			}
			m_GameServerVec.clear();
		}
	}
}
void AnnouncementManager::OnAddNewAnnouncementOnce(AnnouncementOnce& pOnce)
{
	//接收到GameServer发送来的新的公告
	m_AnnouncementList.push_front(pOnce);
	while (m_AnnouncementList.size() > g_FishServer.GetFishConfig().GetSystemConfig().AnnouncementSum)
	{
		m_AnnouncementList.pop_back();//移除最后面的一个公告
	}
	CG_Cmd_SendNewAnnouncementOnce msg;
	SetMsgInfo(msg, GetMsgType(Main_Announcement, CG_SendNewAnnouncementOnce), sizeof(CG_Cmd_SendNewAnnouncementOnce));
	msg.pOnce = pOnce;
	g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(&msg);//发送给全部的GameServer去

	DBR_Cmd_AddAnnouncement msgDB;
	SetMsgInfo(msgDB, DBR_AddAnnouncement, sizeof(DBR_Cmd_AddAnnouncement));
	msgDB.AnnouncementInfo = pOnce;
	g_FishServer.SendNetCmdToDB(&msgDB);
}
void AnnouncementManager::OnSendAllAnnouncementToGameServer(BYTE GameID)
{
	//发送全部的公告到GameServer去
	if (!m_IsFinish)
	{
		m_GameServerVec.push_back(GameID);
		return;
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(GameID);
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	DWORD ArraySum = min(g_FishServer.GetFishConfig().GetSystemConfig().AnnouncementSum, m_AnnouncementList.size());
	DWORD PageSize = sizeof(CG_Cmd_GetAllAnnouncement)+sizeof(AnnouncementOnce)*(ArraySum - 1);
	CG_Cmd_GetAllAnnouncement * msg = (CG_Cmd_GetAllAnnouncement*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Announcement, CG_GetAllAnnouncement));
	std::deque<AnnouncementOnce>::iterator Iter = m_AnnouncementList.begin();
	for (DWORD i = 0; i<g_FishServer.GetFishConfig().GetSystemConfig().AnnouncementSum&& Iter != m_AnnouncementList.end(); ++Iter, ++i)
	{
		msg->Array[i] = *Iter;
	}
	std::vector<CG_Cmd_GetAllAnnouncement*> pVec;
	SqlitMsg(msg, PageSize, ArraySum,false, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<CG_Cmd_GetAllAnnouncement*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			g_FishServer.SendNetCmdToClient(pClient, *Iter);
			free(*Iter);
		}
		pVec.clear();
	}
}