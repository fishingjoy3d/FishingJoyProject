#include "stdafx.h"
#include "FishDesk.h"
#include "FishCallbak.h"
#include "NetCmd.h"
#include "FishScene.h"
#include "FishScene\BulletManager.h"
#include "FishScene\FishManager.h"
#include "FishScene\FishCollider.h"
#include "..\Role.h"
#include "FishUtility.h"
FishDesk::FishDesk()
{
	g_BulletId = 0;
	g_DeltaTime = 0;
	m_pScene = NULL;
	m_PlayerManager = NULL;
	m_bResetScene = true;
}
FishDesk::~FishDesk()
{

}
bool FishDesk::Init(int index, NetSendInterface *pNetSend, const WCHAR *pcMapName, CTableRoleManager* pManager)
{
	m_pScene = new FishScene;
	if (!m_pScene->Init(pManager, this))
	{
		SAFE_DELETE(m_pScene);
		return false;
	}
	m_pScene->SetMap(pcMapName);	//设置地图
	m_pNetSend = pNetSend;
	m_nDeskIndex = index;
	m_bPause = false;
	m_PlayerManager = pManager;
	if (!m_PlayerManager)
	{
		ASSERT(false);
		return false;
	}
	return true;
}
void FishDesk::Shutdown()
{
	m_pScene->Shutdown();
	SAFE_DELETE(m_pScene);
	m_PlayerManager->OnDelAllRole();
}
CRole* FishDesk::GetPlayerFromID(PlayerID id)
{
	return m_PlayerManager->GetRoleByUserID(id);
}

void  FishDesk::Update(float deltaTime)
{
	if (m_PlayerManager->GetRoleSum() > 0)
	{
		if (m_bPause)
			m_pScene->Update(0);
		else
			m_pScene->Update(deltaTime);
	}
}
void FishDesk::RemovePlayer(PlayerID id)
{
	m_PlayerManager->OnDelRole(id);
	if (m_PlayerManager->GetRoleSum() == 0)
	{
		m_pScene->Clear();
	}
}

bool FishDesk::HasFreeSeat()
{
	return m_PlayerManager->GetRoleSum() < m_PlayerManager->GetMaxPlayerSum();
}
bool FishDesk::FindPlayerByName(const WCHAR *pcName)
{
	for (BYTE i = 0; i < m_PlayerManager->GetMaxPlayerSum(); ++i)
	{
		CRole* pRole = m_PlayerManager->GetRoleBySeatID(i);
		if (pRole && pRole->IsActionUser() && wcscmp(pcName, pRole->GetNickName()) == 0)
		{
			return true;
		}
	}
	
	return false;
}
void FishDesk::OnGameStar()
{
	//玩家进入归玩家进入 游戏开始归游戏开始
	m_pScene->Reset();//重置场景
}
void FishDesk::OnGameEnd()
{
	m_pScene->Clear();
}
CRole* FishDesk::GetPlayer(byte seat)
{
	return m_PlayerManager->GetRoleBySeatID(seat);
}
byte FishDesk::GetSceneBackground()
{ 
	if (m_pScene) 
		return m_pScene->GetSceneBackground(); 
	else 
		return 0; 
}
bool FishDesk::PlayerJoin(PlayerID id, byte&seat)
{
	CRole* pRole = m_PlayerManager->GetRoleByUserID(id);
	if (!pRole)
		return false;
	seat = pRole->GetSeatID();
	AsyncPlayerJoin(pRole);
	return true;
}
bool FishDesk::PlayerLeave(PlayerID id)
{
	CRole* pRole = m_PlayerManager->GetRoleByUserID(id);
	if (pRole == NULL || !pRole->IsActionUser())
		return false;
	RemovePlayer(pRole->GetID());
	NetCmdPlayerLeave ncp;
	SetMsgInfo(ncp,CMD_PLAYER_LEAVE, sizeof(NetCmdPlayerLeave));//发送命令 指定玩家离开
	ncp.Seat = pRole->GetSeatID();
	SendAll(&ncp);
	pRole->Clear();
	m_pScene->GetBulletMgr()->RemoveBySeat(pRole->GetSeatID());
	return true;
}
void FishDesk::SendFrom(PlayerID id, NetCmd *pCmd)
{
	for (BYTE i = 0; i < m_PlayerManager->GetMaxPlayerSum(); ++i)
	{
		CRole* pRole = m_PlayerManager->GetRoleBySeatID(i);
		if (pRole && pRole->IsActionUser() && pRole->GetID() != id && pRole->IsCanSendTableMsg())
		{
			if (m_pNetSend->IsUseSeat())
				m_pNetSend->SendBySeat(pRole->GetSeatID(), pCmd);
			else
				m_pNetSend->Send(pRole->GetID(), pCmd);
		}
	}
}

void FishDesk::SendTo(PlayerID id, NetCmd *pCmd)
{
	CRole* pRole = m_PlayerManager->GetRoleByUserID(id);
	if (pRole == NULL || !pRole->IsActionUser() && pRole->IsCanSendTableMsg())
	{
		free(pCmd);
		return;
	}
	if (m_pNetSend->IsUseSeat())
		m_pNetSend->SendBySeat(pRole->GetSeatID(), pCmd);
	else
		m_pNetSend->Send(pRole->GetID(), pCmd);
}
void FishDesk::SendAll(NetCmd *pCmd)
{
	for (BYTE i = 0; i < m_PlayerManager->GetMaxPlayerSum(); ++i)
	{
		CRole* pRole = m_PlayerManager->GetRoleBySeatID(i);
		if (pRole && pRole->IsActionUser() && pRole->IsCanSendTableMsg())
		{
			if (m_pNetSend->IsUseSeat())
				m_pNetSend->SendBySeat(pRole->GetSeatID(), pCmd);
			else
				m_pNetSend->Send(pRole->GetID(), pCmd);
		}
	}
}
void FishDesk::SendAll(USHORT groupIndex, USHORT pathIndex, USHORT startID)
{
	NetCmdFish fish;
	SetMsgInfo(fish,CMD_FISH, sizeof(NetCmdFish));
	fish.GroupID = groupIndex;
	fish.PathID = pathIndex;
	fish.StartID = startID;
	SendAll(&fish);
}
void FishDesk::SendAll(USHORT groupIndex, USHORT startID)
{
	NetCmdFish fish;
	SetMsgInfo(fish,CMD_FISH, sizeof(NetCmdFish));
	fish.GroupID = groupIndex;
	fish.PathID = USHRT_MAX;
	fish.StartID = startID;
	SendAll(&fish);
}