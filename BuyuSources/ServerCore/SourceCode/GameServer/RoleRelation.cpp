#include "Stdafx.h"
#include "RoleRelation.h"
#include "RoleEx.h"
#include "RoleManager.h"
#include "FishServer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
RoleRelationManager::RoleRelationManager()
{
	m_pUser = NULL;
	m_RoleManager = NULL;
	m_BeRelationMap.clear();
	m_RelationMap.clear();
	m_IsLoadToClient = false;
	m_IsLoadDB = false;
}
RoleRelationManager::~RoleRelationManager()
{

}
bool RoleRelationManager::OnInit(CRoleEx* pUser, RoleManager* pManager)
{
	if (!pUser ||  !pManager)
	{
		ASSERT(false);
		return false;
	}
	m_RoleManager = pManager;
	m_pUser = pUser;
	return OnLoadUserRelation();
}
bool RoleRelationManager::OnLoadUserRelation()
{
	//发送DBR命令加载全部的数据
	if (!m_pUser)
	{
		ASSERT(false);
		return false;
	}
	m_BeRelationMap.clear();
	m_RelationMap.clear();
	/*DBR_Cmd_LoadUserRelation msg;
	SetMsgInfo(msg,DBR_LoadUserRelation, sizeof(DBR_Cmd_LoadUserRelation));
	msg.dwUserID = m_pUser->GetUserID();
	g_FishServer.SendNetCmdToDB(&msg);*/
	return true;
}
void RoleRelationManager::OnLoadBeUserRelationResult(DBO_Cmd_LoadBeUserRelation* pDB)//添加被动关系
{
	if (!pDB || !m_pUser)
	{
		ASSERT(false);
		return;
	}
	if ((pDB->States & MsgBegin) != 0)
	{
		m_BeRelationMap.clear();
	}
	for (size_t i = 0; i < pDB->Sum; ++i)
	{
		tagBeRoleRelation pType;
		pType.dwUserID = pDB->Array[i].dwUserID;
		pType.bRelationType = (RelationType)pDB->Array[i].bRelationType;
		m_BeRelationMap.insert(HashMap<DWORD, tagBeRoleRelation>::value_type(pDB->Array[i].dwUserID, pType));
	}
	if ((pDB->States & MsgEnd) != 0)
	{
		m_IsLoadDB = true;
		m_pUser->IsLoadFinish();
	}
}
void RoleRelationManager::OnLoadUserRelationResult(DBO_Cmd_LoadUserRelation* pDB)
{
	//将加载的关系全部加载起来
	if (!pDB)
	{
		ASSERT(false);
		return;
	}
	if ((pDB->States & MsgBegin) != 0)
	{
		m_RelationMap.clear();
	}
	for (size_t i = 0; i < pDB->Sum; ++i)
	{
		//将关系加入到集合里面去
		m_RelationMap.insert(HashMap<DWORD, tagRoleRelation>::value_type(pDB->Array[i].dwDestRoleID, pDB->Array[i]));
	}
}
//void RoleRelationManager::OnLoadBeUserRelationFinish()
//{
//	if (!m_pUser)
//	{
//		ASSERT(false);
//		return;
//	}
//
//}
bool RoleRelationManager::OnAddUserRelation(CL_Cmd_AddUserRelation* pMsg)
{
	//玩家添加一个关系
	if (!pMsg || !m_pUser)
	{
		ASSERT(false);
		return false;
	}
	if (pMsg->dwDestUserID == m_pUser->GetUserID())//不可以添加自己为关系
	{
		return false;
	}
	//有可能是修改 或者是添加

	if (m_RelationMap.size() >= g_FishServer.GetFishConfig().GetRelation().MaxRelationSum && m_RelationMap.count(pMsg->dwDestUserID) == 0)
	{
		//ASSERT(false);//超出限制 无法继续添加关系数据 防止玩家的关系数据过大
		return false;
	}

	HashMap<DWORD, tagRoleRelation>::iterator Iter= m_RelationMap.find(pMsg->dwDestUserID);
	if (Iter != m_RelationMap.end() && Iter->second.bRelationType == pMsg->bRelationType)
	{
		ASSERT(false);
		LC_Cmd_AddUserRelation msg;
		SetMsgInfo(msg,GetMsgType(Main_Relation, LC_AddUserRelation), sizeof(LC_Cmd_AddUserRelation));
		msg.RelationInfo.dwDestRoleID = pMsg->dwDestUserID;
		msg.RelationInfo.bRelationType = pMsg->bRelationType;
		msg.Result = false;
		m_pUser->SendDataToClient(&msg);
		return false;
	}
	//执行存储过程
	DBR_Cmd_AddUserRelation msg;
	SetMsgInfo(msg,DBR_AddUserRelation, sizeof(DBR_Cmd_AddUserRelation));
	msg.dwSrcUserID = m_pUser->GetUserID();
	msg.dwDestUserID = pMsg->dwDestUserID;
	msg.bRelationType = pMsg->bRelationType;
	g_FishServer.SendNetCmdToDB(&msg);
	return true;
}
void RoleRelationManager::OnAddUserRelationResult(DBO_Cmd_AddUserRelation* pDB)
{
	if (!pDB || !m_pUser || !m_RoleManager)
	{
		ASSERT(false);
		return;
	}
	//有可能是修改 有可能是添加
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(pDB->RelationInfo.dwDestRoleID);
	if (Iter == m_RelationMap.end())
	{
		//添加
		LC_Cmd_AddUserRelation msg;
		SetMsgInfo(msg, GetMsgType(Main_Relation, LC_AddUserRelation), sizeof(LC_Cmd_AddUserRelation));
		msg.Result = pDB->Result;
		if (msg.Result)
		{
			m_RelationMap.insert(HashMap<DWORD, tagRoleRelation>::value_type(pDB->RelationInfo.dwDestRoleID, pDB->RelationInfo));

			msg.RelationInfo.dwDestRoleID = pDB->RelationInfo.dwDestRoleID;
			msg.RelationInfo.bGender = pDB->RelationInfo.bGender;
			msg.RelationInfo.bRelationType = pDB->RelationInfo.bRelationType;
			msg.RelationInfo.dwFaceID = pDB->RelationInfo.dwFaceID;
			msg.RelationInfo.wLevel = pDB->RelationInfo.wLevel;
			msg.RelationInfo.TitleID = pDB->RelationInfo.TitleID;
			msg.RelationInfo.dwAchievementPoint = pDB->RelationInfo.dwAchievementPoint;
			msg.RelationInfo.VipLevel = pDB->RelationInfo.VipLevel;
			msg.RelationInfo.IsInMonthCard = pDB->RelationInfo.IsInMonthCard;
			msg.RelationInfo.GameID = pDB->RelationInfo.GameID;
			if (pDB->RelationInfo.IsShowIpAddress)
				g_FishServer.GetAddressByIP(pDB->RelationInfo.ClientIP, msg.RelationInfo.IPAddress, CountArray(msg.RelationInfo.IPAddress));
			else
				TCHARCopy(msg.RelationInfo.IPAddress, CountArray(msg.RelationInfo.IPAddress), Defalue_Ip_Address, _tcslen(Defalue_Ip_Address));
			for (int i = 0; i < MAX_CHARM_ITEMSUM; ++i)
				msg.RelationInfo.CharmArray[i] = pDB->RelationInfo.CharmArray[i];

			//计算玩家上次登陆的时间
			if (pDB->RelationInfo.IsOnline)
				msg.RelationInfo.bDiffDay = 0xff;//玩家玩家在线的情况 当前在线 为-1
			else
			{
				time_t Now = time(NULL);
				__int64 diffDay = GetDiffDay(pDB->RelationInfo.LastLogonTime,g_FishServer.GetFishConfig().GetWriteSec());
				if (diffDay > 250)
					diffDay = 250;
				msg.RelationInfo.bDiffDay = (BYTE)diffDay;
			}

			TCHARCopy(msg.RelationInfo.DestNickName, CountArray(msg.RelationInfo.DestNickName), pDB->RelationInfo.DestNickName, _tcslen(pDB->RelationInfo.DestNickName));
			if (m_IsLoadToClient)
				m_pUser->SendDataToClient(&msg);
		}
		else
		{
			m_pUser->SendDataToClient(&msg);
		}
		//添加关系成功 我们想要让被添加的玩家进行添加
		if (pDB->Result)
		{
			//将当前添加成功的数据发送到Center去
			CC_Cmd_AddUserRelation msg;
			SetMsgInfo(msg, GetMsgType(Main_Relation, CC_AddUserRelation), sizeof(CC_Cmd_AddUserRelation));
			msg.dwUserID = m_pUser->GetUserID();
			msg.RelationInfo = pDB->RelationInfo;
			m_pUser->SendDataToCenter(&msg);//将数据发送到中央服务器去
		}
		m_pUser->GetRoleRelationRequest().OnAddRelationResult(m_pUser->GetUserID(), pDB->RelationInfo.dwDestRoleID, pDB->RelationInfo.bRelationType, msg.Result);
	}
	else
	{
		//修改
		if (pDB->Result)
		{
			Iter->second.bRelationType = pDB->RelationInfo.bRelationType;
			//发送到中央服务器去
			CC_Cmd_ChangeUserRelation msgCenter;
			SetMsgInfo(msgCenter, GetMsgType(Main_Relation, CC_ChangeUserRelation), sizeof(CC_Cmd_ChangeUserRelation));
			msgCenter.dwUserID = m_pUser->GetUserID();
			msgCenter.dwDestUserID = pDB->RelationInfo.dwDestRoleID;
			msgCenter.RelationType = pDB->RelationInfo.bRelationType;
			m_pUser->SendDataToCenter(&msgCenter);
			//发送到客户端去
			LC_Cmd_ChangeUserRelation msgClient;
			SetMsgInfo(msgClient, GetMsgType(Main_Relation, LC_ChangeUserRelation), sizeof(LC_Cmd_ChangeUserRelation));
			msgClient.dwDestUserID = pDB->RelationInfo.dwDestRoleID;
			msgClient.RelationType = pDB->RelationInfo.bRelationType;
			msgClient.Result = true;
			m_pUser->SendDataToClient(&msgClient);
		}
		else
		{
			LC_Cmd_ChangeUserRelation msgClient;
			SetMsgInfo(msgClient, GetMsgType(Main_Relation, LC_ChangeUserRelation), sizeof(LC_Cmd_ChangeUserRelation));
			msgClient.dwDestUserID = pDB->RelationInfo.dwDestRoleID;
			msgClient.RelationType = pDB->RelationInfo.bRelationType;
			msgClient.Result = false;
			m_pUser->SendDataToClient(&msgClient);
		}
	}
}
bool RoleRelationManager::OnDelUserRelation(CL_Cmd_DelUserRelation* pDB)
{
	//玩家删除一个关系
	if (!pDB || !m_pUser || !m_IsLoadToClient)
	{
		ASSERT(false);
		return false;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(pDB->dwDestUserID);
	if (Iter != m_RelationMap.end())
	{
		//发送DBR请求
		DBR_Cmd_DelUserRelation msg;
		SetMsgInfo(msg,DBR_DelUserRelation, sizeof(DBR_Cmd_DelUserRelation));
		msg.dwSrcUserID = m_pUser->GetUserID();
		msg.dwDestUserID = pDB->dwDestUserID;
		g_FishServer.SendNetCmdToSaveDB(&msg);
		//发送到中央服务器去
		CC_Cmd_DelUserRelation msgCenter;
		SetMsgInfo(msgCenter,GetMsgType(Main_Relation, CC_DelUserRelation), sizeof(CC_Cmd_DelUserRelation));
		msgCenter.dwUserID = m_pUser->GetUserID();
		msgCenter.dwDestUserID = pDB->dwDestUserID;
		m_pUser->SendDataToCenter(&msgCenter);
		//发送到客户端去
		LC_Cmd_DelUserRelation msgClient;
		SetMsgInfo(msgClient,GetMsgType(Main_Relation, LC_DelUserRelation), sizeof(LC_Cmd_DelUserRelation));
		msgClient.dwDestUserID = pDB->dwDestUserID;
		msgClient.Result = true;
		m_pUser->SendDataToClient(&msgClient);


		BYTE RelationType = Iter->second.bRelationType;
		m_RelationMap.erase(Iter);

		
		m_pUser->GetRoleCharManager().OnDelRelation(pDB->dwDestUserID);

		m_pUser->GetRoleRelationRequest().OnDelRelation(pDB->dwDestUserID, RelationType);
		return true;
	}
	LC_Cmd_DelUserRelation msg;
	SetMsgInfo(msg,GetMsgType(Main_Relation, LC_DelUserRelation), sizeof(LC_Cmd_DelUserRelation));
	msg.dwDestUserID = pDB->dwDestUserID;
	msg.Result = false;
	m_pUser->SendDataToClient(&msg);
	return true;
}
//bool RoleRelationManager::OnChangeUserRelation(CL_Cmd_ChangeUserRelation* pMsg)
//{
//	//修改一个已经存在的关系
//	if (!pMsg || !m_pUser)
//	{
//		ASSERT(false);
//		return false;
//	}
//	if (!m_IsLoadToClient)
//	{
//		ASSERT(false);
//		return false;
//	}
//	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(pMsg->dwDestUserID);
//	if (Iter != m_RelationMap.end())
//	{
//		
//		return true;
//	}
//	
//	return true;
//}
void RoleRelationManager::OnAddBeUserRelation(tagBeRoleRelation* pInfo)
{
	//添加一个被动关系
	if (!pInfo)
	{
		ASSERT(false);
		return;
	}
	tagBeRoleRelation pType;
	pType.dwUserID = pInfo->dwUserID;
	pType.bRelationType = (RelationType)pInfo->bRelationType;
	m_BeRelationMap.insert(HashMap<DWORD, tagBeRoleRelation>::value_type(pInfo->dwUserID, pType));
}
void RoleRelationManager::OnDelBeUserRelation(DWORD dwDestUserID)
{
	m_BeRelationMap.erase(dwDestUserID);
}
void RoleRelationManager::OnChagneBeUserRelation(DWORD dwDestUserID, BYTE bRelationType)
{
	HashMap<DWORD, tagBeRoleRelation>::iterator Iter = m_BeRelationMap.find(dwDestUserID);
	if (Iter != m_BeRelationMap.end())
	{
		Iter->second.bRelationType = (RelationType)bRelationType;
	}
}
bool RoleRelationManager::OnGetUserRelation()
{
	//客户端获取全部玩家的关系数据 存储起来
	if (!m_pUser)
	{
		ASSERT(false);
		return false;
	}
	DWORD PageSize = sizeof(LC_Cmd_GetUserRelation)+sizeof(tagRoleRelationClient)*(m_RelationMap.size() - 1);
	LC_Cmd_GetUserRelation * msg = (LC_Cmd_GetUserRelation*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return false;
	}
	msg->SetCmdType(GetMsgType(Main_Relation, LC_GetUserRelation));
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.begin();
	for (int i = 0; Iter != m_RelationMap.end(); ++Iter, ++i)
	{
		msg->Array[i].dwDestRoleID = Iter->second.dwDestRoleID;
		msg->Array[i].bGender = Iter->second.bGender;
		msg->Array[i].bRelationType = Iter->second.bRelationType;
		msg->Array[i].dwFaceID = Iter->second.dwFaceID;
		msg->Array[i].wLevel = Iter->second.wLevel;
		msg->Array[i].TitleID = Iter->second.TitleID;
		msg->Array[i].dwAchievementPoint = Iter->second.dwAchievementPoint;
		msg->Array[i].VipLevel = Iter->second.VipLevel;
		msg->Array[i].IsInMonthCard = Iter->second.IsInMonthCard;
		msg->Array[i].GameID = Iter->second.GameID;
		if (Iter->second.IsShowIpAddress)
			g_FishServer.GetAddressByIP(Iter->second.ClientIP, msg->Array[i].IPAddress, CountArray(msg->Array[i].IPAddress));
		else
			TCHARCopy(msg->Array[i].IPAddress, CountArray(msg->Array[i].IPAddress), Defalue_Ip_Address, _tcslen(Defalue_Ip_Address));
		for (int j = 0; j < MAX_CHARM_ITEMSUM; ++j)
			msg->Array[i].CharmArray[j] = Iter->second.CharmArray[j];
		TCHARCopy(msg->Array[i].DestNickName, CountArray(msg->Array[i].DestNickName), Iter->second.DestNickName, _tcslen(Iter->second.DestNickName));
		if (Iter->second.IsOnline)
			msg->Array[i].bDiffDay = 0xff;//玩家玩家在线的情况 当前在线 为-1
		else
		{
			time_t Now = time(NULL);
			__int64 diffDay = GetDiffDay(Iter->second.LastLogonTime, g_FishServer.GetFishConfig().GetWriteSec());
			if (diffDay > 250)
				diffDay = 250;
			msg->Array[i].bDiffDay = (BYTE)diffDay;
		}
	}
	std::vector<LC_Cmd_GetUserRelation*> pVec;
	SqlitMsg(msg, PageSize, m_RelationMap.size(), true, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_GetUserRelation*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			m_pUser->SendDataToClient(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
	//发送完成命令
	m_IsLoadToClient = true;
	return true;
}
void RoleRelationManager::SendRoleRelationToCenter()
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	//将玩家的关系数据发送到Center去 
	{
		DWORD PageSize = sizeof(CC_Cmd_GetUserRelation)+sizeof(tagRoleRelation)*(m_RelationMap.size() - 1);
		CC_Cmd_GetUserRelation * msg = (CC_Cmd_GetUserRelation*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return;
		}
		msg->SetCmdType(GetMsgType(Main_Relation, CC_LoadUserRelation));
		msg->dwUserID = m_pUser->GetUserID();// 数据
		HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.begin();
		for (int i = 0; Iter != m_RelationMap.end(); ++Iter, ++i)
		{
			msg->Array[i] = Iter->second;
		}
		std::vector<CC_Cmd_GetUserRelation*> pVec;
		SqlitMsg(msg, PageSize, m_RelationMap.size(), false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<CC_Cmd_GetUserRelation*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				m_pUser->SendDataToCenter(*Iter);
				free(*Iter);
			}
			pVec.clear();
		}
	}
	//被动关系 
	{
		DWORD PageSize = sizeof(CC_Cmd_LoadBeUserRelation)+sizeof(tagBeRoleRelation)*(m_BeRelationMap.size() - 1);
		CC_Cmd_LoadBeUserRelation * msg = (CC_Cmd_LoadBeUserRelation*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return;
		}
		msg->SetCmdType(GetMsgType(Main_Relation, CC_LoadBeUserRelation));
		msg->dwUserID = m_pUser->GetUserID();// 数据
		HashMap<DWORD, tagBeRoleRelation>::iterator Iter = m_BeRelationMap.begin();
		for (int i = 0; Iter != m_BeRelationMap.end(); ++Iter, ++i)
		{
			msg->Array[i] = Iter->second;
		}
		std::vector<CC_Cmd_LoadBeUserRelation*> pVec;
		SqlitMsg(msg, PageSize, m_BeRelationMap.size(), false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<CC_Cmd_LoadBeUserRelation*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				m_pUser->SendDataToCenter(*Iter);
				free(*Iter);
			}
			pVec.clear();
		}
	}
	//发送完成命令
	/*CC_Cmd_GetUserRelationFinish msgFinish;
	SetMsgInfo(msgFinish,GetMsgType(Main_Relation, CC_LoadUserRelationFinish), sizeof(CC_Cmd_GetUserRelationFinish));
	msgFinish.dwUserID = m_pUser->GetUserID();
	m_pUser->SendDataToCenter(&msgFinish);*/
}
void RoleRelationManager::OnChangeUserOnline(DWORD dwUserID, bool IsOnline)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		//ASSERT(false);
		return;
	}
	else
	{
		Iter->second.IsOnline = IsOnline;
		if (IsOnline)
		{
			Iter->second.LastLogonTime = time(NULL);
		}
		if (m_IsLoadToClient)
		{
			LC_Cmd_ChangeUserOline msg;
			SetMsgInfo(msg,GetMsgType(Main_Relation, LC_RoleChangeOnline), sizeof(LC_Cmd_ChangeUserOline));
			//msg.dwSrcUserID = m_pUser->GetUserID();
			msg.dwDestUserID = dwUserID;
			msg.IsOnline = IsOnline;
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationLevel(DWORD dwUserID, WORD wLevel)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		if (Iter->second.wLevel == wLevel)
			return;
		Iter->second.wLevel = wLevel;
		//发送命令到客户端去
		if (m_IsLoadToClient)
		{
			LC_Cmd_ChangeRelationLevel msg;
			SetMsgInfo(msg,GetMsgType(Main_Relation, LC_ChangeRelationLevel), sizeof(LC_Cmd_ChangeRelationLevel));
			msg.dwDestUserID = dwUserID;
			msg.wLevel = wLevel;
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationFaceID(DWORD dwUserID, DWORD FaceID)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		if (Iter->second.dwFaceID == FaceID)
			return;
		Iter->second.dwFaceID = FaceID;
		//发送命令到客户端去
		if (m_IsLoadToClient)
		{
			LC_Cmd_ChangeRelationFaceID msg;
			SetMsgInfo(msg,GetMsgType(Main_Relation, LC_ChangeRelationFaceID), sizeof(LC_Cmd_ChangeRelationFaceID));
			msg.dwDestUserID = dwUserID;
			msg.dwFaceID = FaceID;
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationGender(DWORD dwUserID, bool bGender)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		if (Iter->second.bGender == bGender)
			return;
		Iter->second.bGender = bGender;
		//发送命令到客户端去
		if (m_IsLoadToClient)
		{
			LC_Cmd_ChangeRelationGender msg;
			SetMsgInfo(msg,GetMsgType(Main_Relation, LC_ChangeRelationGender), sizeof(LC_Cmd_ChangeRelationGender));
			msg.dwDestUserID = dwUserID;
			msg.bGender = bGender;
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationNickName(DWORD dwUserID, LPTSTR pNickName)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		if (_tcscmp(pNickName, Iter->second.DestNickName) == 0)
			return;
		TCHARCopy(Iter->second.DestNickName, CountArray(Iter->second.DestNickName), pNickName, _tcslen(pNickName));
		//发送命令到客户端去
		if (m_IsLoadToClient)
		{
			LC_Cmd_ChangeRelationNickName msg;
			SetMsgInfo(msg,GetMsgType(Main_Relation, LC_ChangeRelationNickName), sizeof(LC_Cmd_ChangeRelationNickName));
			msg.dwDestUserID = dwUserID;
			TCHARCopy(msg.cNickName, CountArray(msg.cNickName), pNickName, _tcslen(pNickName));
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationTitle(DWORD dwUserID, BYTE TitleID)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		if (Iter->second.TitleID == TitleID)
			return;
		Iter->second.TitleID = TitleID;
		//发送命令到客户端去
		if (m_IsLoadToClient)
		{
			LC_Cmd_ChangeRelationTitle msg;
			SetMsgInfo(msg,GetMsgType(Main_Relation, LC_ChangeRelationTitle), sizeof(LC_Cmd_ChangeRelationTitle));
			msg.dwDestUserID = dwUserID;
			msg.TitleID = TitleID;
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationAchievementPoint(DWORD dwUserID, DWORD dwAchievementPoint)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	//修改玩家的成就点数
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		if (Iter->second.dwAchievementPoint == dwAchievementPoint)
			return;
		Iter->second.dwAchievementPoint = dwAchievementPoint;
		//发送命令到客户端去
		if (m_IsLoadToClient)
		{
			LC_Cmd_ChangeRelationAchievementPoint msg;
			SetMsgInfo(msg,GetMsgType(Main_Relation, LC_ChangeRelationAchievementPoint), sizeof(LC_Cmd_ChangeRelationAchievementPoint));
			msg.dwDestUserID = dwUserID;
			msg.dwAchievementPoint = dwAchievementPoint;
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationCharmValue(DWORD dwUserID, DWORD pCharm[MAX_CHARM_ITEMSUM])
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		for (int i = 0; i < MAX_CHARM_ITEMSUM;++i)
			Iter->second.CharmArray[i] = pCharm[i];
		//发送命令到客户端去
		if (m_IsLoadToClient)
		{
			LC_Cmd_ChangeRelationCharmValue msg;
			SetMsgInfo(msg,GetMsgType(Main_Relation, LC_ChangeRelationCharmValue), sizeof(LC_Cmd_ChangeRelationCharmValue));
			msg.dwDestUserID = dwUserID;
			for (int i = 0; i < MAX_CHARM_ITEMSUM; ++i)
				msg.CharmInfo[i] = pCharm[i];
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationCLientIP(DWORD dwUserID, DWORD ClientIP)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		Iter->second.ClientIP = ClientIP;
		//发送命令到客户端去
		if (m_IsLoadToClient)
		{
			LC_Cmd_ChangeRelationIP msg;
			SetMsgInfo(msg, GetMsgType(Main_Relation, LC_ChangeRelationIP), sizeof(LC_Cmd_ChangeRelationIP));
			msg.dwDestUserID = dwUserID;
			g_FishServer.GetAddressByIP(ClientIP, msg.IPAddress, CountArray(msg.IPAddress));
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationIsShowIpAddress(DWORD dwUserID, bool States)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		Iter->second.IsShowIpAddress = States;
		if (States)
		{
			//修改地址
			LC_Cmd_ChangeRelationIP msg;
			SetMsgInfo(msg, GetMsgType(Main_Relation, LC_ChangeRelationIP), sizeof(LC_Cmd_ChangeRelationIP));
			msg.dwDestUserID = dwUserID;
			g_FishServer.GetAddressByIP(Iter->second.ClientIP, msg.IPAddress, CountArray(msg.IPAddress));
			m_pUser->SendDataToClient(&msg);
		}
		else
		{
			LC_Cmd_ChangeRelationIP msg;
			SetMsgInfo(msg, GetMsgType(Main_Relation, LC_ChangeRelationIP), sizeof(LC_Cmd_ChangeRelationIP));
			msg.dwDestUserID = dwUserID;
			TCHARCopy(msg.IPAddress, CountArray(msg.IPAddress), Defalue_Ip_Address, _tcslen(Defalue_Ip_Address));//目标的地址为隐藏
			m_pUser->SendDataToClient(&msg);
		}
	}
}
void RoleRelationManager::OnChangeRelationVipLevel(DWORD dwUserID, BYTE VipLevel)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		Iter->second.VipLevel = VipLevel;
		LC_Cmd_ChangeRelationVipLevel msg;
		SetMsgInfo(msg, GetMsgType(Main_Relation, LC_ChangeRelationVipLevel), sizeof(LC_Cmd_ChangeRelationVipLevel));
		msg.dwDestUserID = dwUserID;
		msg.VipLevel = VipLevel;
		m_pUser->SendDataToClient(&msg);
	}
}
void RoleRelationManager::OnChangeRelationIsInMonthCard(DWORD dwUserID, bool IsInMonthCard)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return;
	}
	else
	{
		Iter->second.IsInMonthCard = IsInMonthCard;
		LC_Cmd_ChangeRelationIsInMonthCard msg;
		SetMsgInfo(msg, GetMsgType(Main_Relation, LC_ChangeRelationIsInMonthCard), sizeof(LC_Cmd_ChangeRelationIsInMonthCard));
		msg.dwDestUserID = dwUserID;
		msg.IsInMonthCard = IsInMonthCard;
		m_pUser->SendDataToClient(&msg);
	}
}
tagRoleRelation* RoleRelationManager::QueryRelationInfo(DWORD dwDestUserID)
{
	HashMap<DWORD, tagRoleRelation>::iterator Iter = m_RelationMap.find(dwDestUserID);
	if (Iter == m_RelationMap.end())
	{
		ASSERT(false);
		return null;
	}
	else
		return &Iter->second;
}