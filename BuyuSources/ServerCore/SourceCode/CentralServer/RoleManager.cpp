#include "Stdafx.h"
#include "RoleManager.h"
#include "FishServer.h"
CenterRoleManager::CenterRoleManager()
{
	m_LeaveOnlineUser = 0;
}
CenterRoleManager::~CenterRoleManager()
{
	Destroy();
}
void CenterRoleManager::OnInit()
{
	m_RoleMap.clear();
}
void CenterRoleManager::Destroy()
{
	if (m_RoleMap.empty())
		return;
	HashMap<DWORD, CenterRole*>::iterator Iter = m_RoleMap.begin();
	for (; Iter != m_RoleMap.end(); ++Iter)
	{
		if (!Iter->second)
			continue;
		delete Iter->second;
	}
	m_RoleMap.clear();
}
bool CenterRoleManager::OnCreateCenterRole(BYTE dwSocketID, tagCenterRoleInfo* pInfo, bool IsRobot)
{
	if (!pInfo)
	{
		ASSERT(false);
		return false;
	}
	HashMap<DWORD, CenterRole*>::iterator Iter = m_RoleMap.find(pInfo->dwUserID);
	if (Iter != m_RoleMap.end())
	{
		return true;
	}
	else
	{
		CenterRole* pRole = new CenterRole();
		if (!pRole)
		{
			ASSERT(false);
			return false;
		}
		pRole->OnInit(dwSocketID, pInfo, this,IsRobot);
		//加入集合
		m_RoleMap.insert(std::map<DWORD, CenterRole*>::value_type(pInfo->dwUserID, pRole));
		return true;
	}
}
bool CenterRoleManager::OnDelCenterRole(DWORD dwUserID)
{
	HashMap<DWORD, CenterRole*>::iterator Iter = m_RoleMap.find(dwUserID);
	if (Iter != m_RoleMap.end() && Iter->second)
	{
		Iter->second->OnRoleLeave();
		delete Iter->second;
		m_RoleMap.erase(Iter);
		return true;
	}
	else
	{
		return true;
	}
}
bool CenterRoleManager::OnPlazaLeave(BYTE dwSocketID)
{
	HashMap<DWORD, CenterRole*>::iterator Iter = m_RoleMap.begin();
	for (; Iter != m_RoleMap.end();)
	{
		if (Iter->second && Iter->second->GetSocketID() == dwSocketID)
		{
			//Iter->second->ClearSocketID();
			Iter->second->OnRoleLeave();
			g_FishServer.GetCenterManager().UnRsgUser(Iter->first);//让指定玩家离线 当GameServer断开连接的时候 进行处理

			delete Iter->second;
			Iter = m_RoleMap.erase(Iter);
		}
		else
			++Iter;
	}
	return true;
}
void CenterRoleManager::SetCenterUserFinish(DWORD dwUserID)
{
	HashMap<DWORD, CenterRole*>::iterator Iter = m_RoleMap.find(dwUserID);
	if (Iter != m_RoleMap.end() && Iter->second)
	{
		Iter->second->OnRoleEnterFinish();
	}
}
CenterRole* CenterRoleManager::QueryCenterUser(DWORD dwUserID)
{
	HashMap<DWORD, CenterRole*>::iterator Iter = m_RoleMap.find(dwUserID);
	if (Iter != m_RoleMap.end() && Iter->second)
		return Iter->second;
	else
		return NULL;
}

CenterRole::CenterRole()
{
	//CTraceService::TraceString(TEXT("创建一个CenterRole对象"), TraceLevel_Normal);
	m_IsRobot = false;
}
CenterRole::~CenterRole()
{
	//CTraceService::TraceString(TEXT("删除一个CenterRole对象"), TraceLevel_Normal);
	//g_FishServer.GetCenterManager().UnRsgUser(m_RoleInfo.dwUserID);//指定玩家离开了
}
void CenterRole::OnInit(BYTE dwSocketID, tagCenterRoleInfo* pInfo, CenterRoleManager* pManager, bool IsRobot)
{
	//当对象创建完毕的时候
	if (!pInfo || !pManager)
	{
		ASSERT(false);
		return;
	}
	m_dwSocketID = dwSocketID;
	m_RoleInfo = *pInfo;
	m_pRoleManager = pManager;
	m_IsRobot = IsRobot;
	m_RelationManager.OnInit(this, m_pRoleManager);
}
void CenterRole::OnRoleLeave()
{
	//当 一个玩家离开Center的时候 我们想要通知玩家 的被好友列表的全部玩家  xx玩家下线了
	//发送命令 Relation...
	//CTraceService::TraceString(TEXT("CenterRole 触发离开事件"), TraceLevel_Normal);
	GetRelationManager().OnRoleOnlinChange(false);
}
void CenterRole::OnRoleEnterFinish()
{
	//当中央服务器玩家对象创建完毕的时候 
	//1.将玩家上线的消息发送给全部的被好友玩家
	//...
	//CTraceService::TraceString(TEXT("CenterRole 触发进入事件"), TraceLevel_Normal);
	GetRelationManager().OnRoleOnlinChange(true);
}
void CenterRole::SendDataToGameServer(NetCmd* pCmd)
{
	g_FishServer.GetCenterManager().SendNetCmdToGameServer(m_dwSocketID, pCmd);
}
void CenterRole::OnChangeRoleLevel(WORD wLevel)
{
	if (m_RoleInfo.wLevel == wLevel)
		return;
	m_RoleInfo.wLevel = wLevel;
	//玩家中央服务器点击修改完毕了 我们可以通知全部的好友 玩家点击改变了
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeRelationUserLevel(m_RoleInfo.dwUserID, wLevel);
		}
	}
}
//void CenterRole::OnChangeRoleExp(DWORD dwExp)
//{
//	if (m_RoleInfo.dwExp == dwExp)
//		return;
//	m_RoleInfo.dwExp = dwExp;
//	//经验的修改 无须通知其他的玩家
//}
void CenterRole::OnChangeRoleNickName(LPTSTR pNickName)
{
	if (_tcscmp(pNickName, m_RoleInfo.NickName) == 0)
		return;
	TCHARCopy(m_RoleInfo.NickName, CountArray(m_RoleInfo.NickName), pNickName, _tcslen(pNickName));
	//通知好友
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChanfeRelationUserNickName(m_RoleInfo.dwUserID, pNickName);
		}
	}
}
void CenterRole::OnChangeRoleFaceID(DWORD dwFaceID)
{
	if (m_RoleInfo.dwFaceID == dwFaceID)
		return;
	m_RoleInfo.dwFaceID = dwFaceID;
	//通知好友
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeReltaionUserFaceID(m_RoleInfo.dwUserID, dwFaceID);
		}
	}
}
void CenterRole::OnChangeRoleGender(bool bGender)
{
	if (m_RoleInfo.bGender == bGender)
		return;
	m_RoleInfo.bGender = bGender;
	//通知好友
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChanfeRelationUserGender(m_RoleInfo.dwUserID, bGender);
		}
	}
}
void CenterRole::OnChangeRoleTitle(BYTE TitleID)
{
	if (m_RoleInfo.TitleID == TitleID)
		return;
	m_RoleInfo.TitleID = TitleID;

	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeRelationUserTitle(m_RoleInfo.dwUserID, TitleID);
		}
	}
}
void CenterRole::OnChangeRoleAchievementPoint(DWORD dwAchievementPoint)
{
	if (m_RoleInfo.dwAchievementPoint == dwAchievementPoint)
		return;
	m_RoleInfo.dwAchievementPoint = dwAchievementPoint;

	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeRelationUserAchievementPoint(m_RoleInfo.dwUserID, dwAchievementPoint);
		}
	}
}
void CenterRole::OnChangeRoleCharmValue(DWORD CharmInfo[MAX_CHARM_ITEMSUM])
{
	for (int i = 0; i < MAX_CHARM_ITEMSUM;++i)
		m_RoleInfo.CharmArray[i] = CharmInfo[i];
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeRelationUserCharmValue(m_RoleInfo.dwUserID, CharmInfo);
		}
	}
}
void CenterRole::OnChangeRoleClientIP(DWORD ClientIP)
{
	m_RoleInfo.ClientIP = ClientIP;
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeRelationUserClientIP(m_RoleInfo.dwUserID, ClientIP);
		}
	}
}
void CenterRole::OnChangeRoleIsShowIpAddress(bool States)
{
	m_RoleInfo.IsShowIpAddress = States;
	//通知所有加我好友的玩家 我的状态变化了 应该修改下我的地址的显示
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeRelationUserIsShowIpAddress(m_RoleInfo.dwUserID, States);
		}
	}
}
void CenterRole::OnChangeRoleIsOnline(bool States)
{
	m_RoleInfo.IsOnline = States;
	m_pRoleManager->SetAddOrDelLeaveOnlineUser(States);//设置中央服务器离线人数
	//通知所有的好友玩家离线了
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeBeRoleOnlineStateus(m_RoleInfo.dwUserID, States);
		}
	}
}
void CenterRole::OnChangeRoleVipLevel(BYTE VipLevel)
{
	//当玩家的VIP等级变化的时候
	m_RoleInfo.VipLevel = VipLevel;
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeRelationUserVipLevel(m_RoleInfo.dwUserID, VipLevel);
		}
	}
}
void CenterRole::OnChangeRoleIsInMonthCard(bool IsInMonthCard)
{
	m_RoleInfo.IsInMonthCard = IsInMonthCard;
	HashMap<DWORD, tagBeRoleRelation>& pMap = GetRelationManager().GetAllBeRelationInfo();
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		CenterRole* pRole = m_pRoleManager->QueryCenterUser(Iter->second.dwUserID);
		if (pRole)
		{
			pRole->GetRelationManager().OnChangeRelatuonUserIsInMonthCard(m_RoleInfo.dwUserID, IsInMonthCard);
		}
	}
}
void CenterRole::OnChangeRoleParticularStates(DWORD States)
{
	m_RoleInfo.ParticularStates = States;
}