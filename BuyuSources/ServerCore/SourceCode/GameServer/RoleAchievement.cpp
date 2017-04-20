#include "Stdafx.h"
#include "RoleAchievement.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleAchievementManager::RoleAchievementManager()
{
	m_IsLoadDB = false;
	m_IsSendClient = false;
	m_pRole = NULL;
	m_AchievementVec.clear();
	int256Handle::Clear(m_JoinAchievementLog);
}
RoleAchievementManager::~RoleAchievementManager()
{
	//SaveAchievement();//先将当前正在进行的任务全部保存一次
	OnDestroy();//删除对象
}
//void RoleAchievementManager::OnSaveByUpdate()
//{
//	//服务器端没15分钟保存一次
//	SaveAchievement();
//}
RoleAchievementBase* RoleAchievementManager::QueryAchievement(BYTE AchievementID)
{
	//查找
	if (m_AchievementVec.empty())
		return NULL;
	std::vector<RoleAchievementBase*>::iterator Iter = m_AchievementVec.begin();
	for (; Iter != m_AchievementVec.end(); ++Iter)
	{
		if (!(*Iter))
		{
			ASSERT(false);
			continue;
		}
		if ((*Iter)->GetAchievementID() == AchievementID)
		{
			return *Iter;
		}
	}
	return NULL;
}
bool RoleAchievementManager::IsCanJoinAchievement(BYTE AchievementID)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	//是否可以接取指定的任务
	//0.任务数量的要求
	if (m_AchievementVec.size() >= g_FishServer.GetFishConfig().GetAchievementConfig().m_MaxJoinAchievementSum)
	{
		return false;
	}
	//判断 任务是否已经接取了  就是当前Vector里面是否有当前的ID的存在
	if (int256Handle::GetBitStates(m_JoinAchievementLog, AchievementID))
	{
		return false;
	}
	//1.判断任务是否已经完成了
	if (int256Handle::GetBitStates(m_pRole->GetRoleInfo().AchievementStates, AchievementID))
	{
		return false;//任务已经完成了
	}
	//2.等级符合要求
	HashMap<BYTE, tagAchievementConfig>::iterator Iter = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(AchievementID);
	if (Iter == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return false;
	}
	if (m_pRole->GetRoleInfo().wLevel < Iter->second.JoinLevel)
	{
		return false;
	}
	//3.前置任务是否已经完成了
	if (Iter->second.LowerAchievementVec.empty())
		return true;
	else
	{
		vector<BYTE>::iterator IterLower = Iter->second.LowerAchievementVec.begin();
		for (; IterLower != Iter->second.LowerAchievementVec.end(); ++IterLower)
		{
			if (!int256Handle::GetBitStates(m_pRole->GetRoleInfo().AchievementStates, *IterLower))
				return false;
		}
		return true;
	}
}
void RoleAchievementManager::OnDestroy()
{
	if (m_AchievementVec.size() == 0)
		return;
	std::vector<RoleAchievementBase*>::iterator Iter = m_AchievementVec.begin();
	for (; Iter != m_AchievementVec.end(); ++Iter)
	{
		delete (*Iter);
	}
	m_AchievementVec.clear();
	int256Handle::Clear(m_JoinAchievementLog);
}
void RoleAchievementManager::GetAllNeedSaveAchievement(vector<tagRoleAchievementInfo>& pVec)
{
	pVec.clear();
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//保持当前全部的任务
	if (m_AchievementVec.size() == 0)
		return;
	std::vector<RoleAchievementBase*>::iterator Iter = m_AchievementVec.begin();
	for (; Iter != m_AchievementVec.end(); ++Iter)
	{
		if (!(*Iter))
		{
			ASSERT(false);
			continue;
		}
		if (!(*Iter)->IsNeedSave())
			continue;
		pVec.push_back((*Iter)->GetAchievementInfo());
		(*Iter)->OnSave();
	}
}
bool RoleAchievementManager::GetAchievementMessageStates()
{
	std::vector<RoleAchievementBase*>::iterator Iter = m_AchievementVec.begin();
	for (; Iter != m_AchievementVec.end(); ++Iter)
	{
		if (!(*Iter))
			continue;
		if ((*Iter)->IsEventFinish())
			return true;
	}
	return false;
}
void RoleAchievementManager::OnResetJoinAllAchievement()
{
	std::vector<RoleAchievementBase*>::iterator Iter = m_AchievementVec.begin();
	for (; Iter != m_AchievementVec.end(); ++Iter)
	{
		if (!(*Iter))
			continue;
		(*Iter)->OnJoinAchievement();
	}
}
//void RoleAchievementManager::SaveAchievement()
//{
//	if (!m_pRole)
//	{
//		ASSERT(false);
//		return; 
//	}
//	//保持当前全部的任务
//	if (m_AchievementVec.size() == 0)
//		return;
//	std::vector<RoleAchievementBase*>::iterator Iter = m_AchievementVec.begin();
//	for (; Iter != m_AchievementVec.end(); ++Iter)
//	{
//		if (!(*Iter))
//		{
//			ASSERT(false);
//			continue;
//		}
//		if (!(*Iter)->IsNeedSave())
//			continue;
//		DBR_Cmd_SaveRoleAchievement msg;
//		SetMsgInfo(msg,DBR_SaveRoleAchievement, sizeof(DBR_Cmd_SaveRoleAchievement));
//		msg.dwUserID = m_pRole->GetUserID();
//		msg.AchievementInfo = (*Iter)->GetAchievementInfo();
//		g_FishServer.SendNetCmdToDB(&msg);
//	}
//}
bool RoleAchievementManager::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	SendLoadAllAchievementInfoToDB();
	return true;
}
void RoleAchievementManager::SendLoadAllAchievementInfoToDB()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//DBR_Cmd_LoadRoleAchievement msg;
	//SetMsgInfo(msg,DBR_LoadRoleAchievement, sizeof(DBR_Cmd_LoadRoleAchievement));
	//msg.dwUserID = m_pRole->GetUserID();
	//g_FishServer.SendNetCmdToDB(&msg);
}
void RoleAchievementManager::OnLoadAllAchievementInfoByDB(DBO_Cmd_LoadRoleAchievement* pDB)
{
	if (!pDB || !m_pRole)
	{
		ASSERT(false);
		return;
	}
	if ((pDB->States & MsgBegin) != 0)
	{
		m_AchievementVec.clear();
	}
	for (WORD i = 0; i < pDB->Sum; ++i)
	{
		//UserID AchievementID Param 任务的数据库表 3个参数就可以了
		HashMap<BYTE, tagAchievementConfig>::iterator Iter = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(pDB->Array[i].AchievementID);
		if (Iter == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end() || !IsCanJoinAchievement(pDB->Array[i].AchievementID))
		{
			//将任务删除掉
			DBR_Cmd_DelRoleAchievement msg;
			SetMsgInfo(msg,DBR_DelRoleAchievement, sizeof(DBR_Cmd_DelRoleAchievement));
			msg.dwUserID = m_pRole->GetUserID();
			msg.bAchievementID = pDB->Array[i].AchievementID;
			g_FishServer.SendNetCmdToSaveDB(&msg);
			continue;
		}
		else
		{
			//生成一个对象
			RoleAchievementBase* pBase = CreateAchievementByEventID(Iter->second.AchievementEventInfo.EventID);//根据事件ID 创建一个任务对象
			if (!pBase || !pBase->OnInit(m_pRole, this, &pDB->Array[i]))
			{
				ASSERT(false);
				continue;
			}
			//存入集合里面去
			m_AchievementVec.push_back(pBase);
			int256Handle::SetBitStates(m_JoinAchievementLog, pDB->Array[i].AchievementID, true);
			continue;
		}
	}
	if ((pDB->States & MsgEnd) != 0)
	{
		OnJoinAchievementByConfig(false);
		m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Achievement);
		m_IsLoadDB = true;
		m_pRole->IsLoadFinish();
	}
}
void RoleAchievementManager::SendAllAchievementToClient()
{
	//将当前正在进行的任务发送到客户端去
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//客户端向服务器端询问全部的任务信息 
	DWORD PageSize = sizeof(LC_Cmd_GetRoleAchievementInfo)+(m_AchievementVec.size() - 1)*sizeof(tagRoleAchievementInfo);
	LC_Cmd_GetRoleAchievementInfo * msg = (LC_Cmd_GetRoleAchievementInfo*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Achievement, LC_GetRoleAchievementInfo));
	if (!m_AchievementVec.empty())
	{
		std::vector<RoleAchievementBase*>::iterator Iter = m_AchievementVec.begin();
		for (WORD i = 0; Iter != m_AchievementVec.end(); ++Iter, ++i)
		{
			msg->Array[i] = (*Iter)->GetAchievementInfo();
		}
	}
	std::vector<LC_Cmd_GetRoleAchievementInfo*> pVec;
	SqlitMsg(msg, PageSize, m_AchievementVec.size(),true, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_GetRoleAchievementInfo*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			m_pRole->SendDataToClient(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
	m_IsSendClient = true;
}
void RoleAchievementManager::OnRoleLevelChange()
{
	OnJoinAchievementByConfig(false);
}
void RoleAchievementManager::OnJoinAchievementByConfig(bool IsNeedSave)
{
	if (m_AchievementVec.size() >= g_FishServer.GetFishConfig().GetAchievementConfig().m_MaxJoinAchievementSum)//任务已满无法接取
		return;
	//尝试接取任务
	HashMap<BYTE, tagAchievementConfig>::iterator Iter = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.begin();
	for (; Iter != g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end(); ++Iter)
	{
		if (m_AchievementVec.size() < g_FishServer.GetFishConfig().GetAchievementConfig().m_MaxJoinAchievementSum)
			OnJoinAchievement(Iter->second.AchievementID,IsNeedSave);
		else
			break;
	}
	if (!IsNeedSave)
	{
		//获取全部任务里待保存的 我们直接进行保存起来
		if (!m_pRole)
		{
			ASSERT(false);
			return;
		}
		vector<tagRoleAchievementInfo> pVec;
		GetAllNeedSaveAchievement(pVec);
		if (pVec.empty())
			return;
		//创建一个命令将全部的任务保存起来
		DWORD PageSize = sizeof(DBR_Cmd_SaveAllAchievement)+(pVec.size() - 1)*sizeof(tagRoleAchievementInfo);
		DBR_Cmd_SaveAllAchievement* msg = (DBR_Cmd_SaveAllAchievement*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return;
		}
		msg->SetCmdType(DBR_SaveAllAchievement);
		msg->dwUserID = m_pRole->GetUserID();
		for (BYTE i = 0; i < pVec.size(); ++i)
		{
			msg->Array[i] = pVec[i];
		}

		std::vector<DBR_Cmd_SaveAllAchievement*> pVec2;
		SqlitMsg(msg, PageSize, pVec.size(),false, pVec2);
		free(msg);
		if (!pVec2.empty())
		{
			std::vector<DBR_Cmd_SaveAllAchievement*>::iterator Iter = pVec2.begin();
			for (; Iter != pVec2.end(); ++Iter)
			{
				g_FishServer.SendNetCmdToSaveDB(*Iter);
				free(*Iter);
			}
			pVec.clear();
		}
	}
}
bool RoleAchievementManager::OnFinishAchievement(BYTE AchievementID)//领取任务奖励的时候
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	//当一个任务完成的时候 我们进行处理
	//1.判断任务是否存在
	if (!int256Handle::GetBitStates(m_JoinAchievementLog, AchievementID))
	{
		return false;
	}
	HashMap<BYTE, tagAchievementConfig>::iterator Iter = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(AchievementID);
	if (Iter == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		return false;
	}
	std::vector<RoleAchievementBase*>::iterator IterAchievement = m_AchievementVec.begin();
	for (; IterAchievement != m_AchievementVec.end(); ++IterAchievement)
	{
		if (!(*IterAchievement))
		{
			ASSERT(false);
			continue;
		}
		if ((*IterAchievement)->GetAchievementID() == AchievementID)
		{
			if (!(*IterAchievement)->IsEventFinish())
			{
				ASSERT(false);
				if (m_IsSendClient)
				{
					LC_Cmd_GetOnceAchievementInfo msg;
					SetMsgInfo(msg,GetMsgType(Main_Achievement, LC_GetOnceAchievementInfo), sizeof(LC_Cmd_GetOnceAchievementInfo));
					msg.AchievementInfo = (*IterAchievement)->GetAchievementInfo();
					m_pRole->SendDataToClient(&msg);
				}
				return false;
			}
			int256Handle::SetBitStates(m_JoinAchievementLog, AchievementID, false);
			//3.给予任务的奖励
			m_pRole->OnAddRoleRewardByRewardID(Iter->second.RewardID,TEXT("完成成就奖励"));
			m_pRole->ChangeRoleAchievementStates(AchievementID, true);
			if (m_IsSendClient)
			{
				LC_Cmd_GetAchievementReward msg;
				SetMsgInfo(msg,GetMsgType(Main_Achievement, LC_GetAchievementReward), sizeof(LC_Cmd_GetAchievementReward));
				msg.AchievementID = AchievementID;
				m_pRole->SendDataToClient(&msg);
			}

			DBR_Cmd_DelRoleAchievement msgDel;
			SetMsgInfo(msgDel,DBR_DelRoleAchievement, sizeof(DBR_Cmd_DelRoleAchievement));
			msgDel.dwUserID = m_pRole->GetUserID();
			msgDel.bAchievementID = AchievementID;
			g_FishServer.SendNetCmdToSaveDB(&msgDel);

			delete *IterAchievement;
			m_AchievementVec.erase(IterAchievement);
			//6.发送命令到客户端去 玩家指定任务已经领取奖励了 将任务删除掉
			/*LC_Cmd_DelAchievement msg;
			msg.AchievementID = AchievementID;
			m_pRole->SendDataToClient(Main_Achievement, LC_DelAchievement, &msg, sizeof(msg));*/
			//7.因为玩家完成了一个任务 
			if (Iter->second.UpperAchievementID == 0)
				OnJoinAchievementByConfig(false);//玩家需要重新接取一个任务 补充缺失
			else
			{
				if (!OnJoinAchievement(Iter->second.UpperAchievementID,true))
					OnJoinAchievementByConfig(false);//玩家需要重新接取一个任务 补充缺失
			}
			m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Achievement);
			return true;
		}
	}
	return false;
}
bool RoleAchievementManager::OnJoinAchievement(BYTE AchievementID, bool IsNeedSave)//接取一个任务的时候
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	//当玩家接取一个新的任务的时候 我们进行处理
	//1.判断任务是否存在
	HashMap<BYTE, tagAchievementConfig>::iterator Iter = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(AchievementID);
	if (Iter == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		return false;
	}
	if (!IsCanJoinAchievement(AchievementID))
		return false;
	//3.任务是否可以接取 前置函数已经判断过了 无须继续判断 生成任务对象
	RoleAchievementBase* pAchievement = CreateAchievementByEventID(Iter->second.AchievementEventInfo.EventID);
	if (!pAchievement)
	{
		ASSERT(false);
		return false;
	}
	m_AchievementVec.push_back(pAchievement);//先放入到集合里面去
	pAchievement->OnInit(m_pRole, this, AchievementID, IsNeedSave);//初始化任务
	int256Handle::SetBitStates(m_JoinAchievementLog, AchievementID, true);
	//4.发送命令到客户端去
	if (m_IsSendClient)
	{
		LC_Cmd_JoinAchievement msg;
		SetMsgInfo(msg,GetMsgType(Main_Achievement, LC_JoinAchievement), sizeof(LC_Cmd_JoinAchievement));
		msg.AchievementID = AchievementID;

		m_pRole->SendDataToClient(&msg);
	}
	if (IsNeedSave)
	{
		DBR_Cmd_SaveRoleAchievement msg;
		SetMsgInfo(msg, DBR_SaveRoleAchievement, sizeof(DBR_Cmd_SaveRoleAchievement));
		msg.dwUserID = m_pRole->GetUserID();
		msg.AchievementInfo = pAchievement->GetAchievementInfo();
		g_FishServer.SendNetCmdToSaveDB(&msg);
	}
	return true;
}
//bool RoleAchievementManager::OnDelAchievement(BYTE AchievementID)
//{
//	if (!m_pRole)
//	{
//		ASSERT(false);
//		return false;
//	}
//	//删除一个任务
//	//1.任务配置是否存在
//	HashMap<BYTE, tagAchievementConfig*>::iterator Iter = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(AchievementID);
//	if (Iter == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end() || !Iter->second)
//	{
//		return false;
//	}
//	//2.任务是否已经接取了
//	if (!int256Handle::GetBitStates(m_JoinAchievementLog, AchievementID))
//	{
//		return false;
//	}
//	//3.将任务删除掉 
//	DBR_Cmd_DelRoleAchievement msg;
//	SetMsgInfo(msg,DBR_DelRoleAchievement, sizeof(DBR_Cmd_DelRoleAchievement));
//	msg.dwUserID = m_pRole->GetUserID();
//	msg.bAchievementID = AchievementID;
//	g_FishServer.SendNetCmdToDB(&msg);
//	//4.将对象删除掉
//	int256Handle::SetBitStates(m_JoinAchievementLog, AchievementID, false);
//	std::vector<RoleAchievementBase*>::iterator IterAchievement = m_AchievementVec.begin();
//	for (; IterAchievement != m_AchievementVec.end(); ++IterAchievement)
//	{
//		if ((*IterAchievement)->GetAchievementID() == AchievementID)
//		{
//			delete *IterAchievement;
//			m_AchievementVec.erase(IterAchievement);
//			break;
//		}
//	}
//	//5.通知客户端 玩家放弃任务
//	if (m_IsSendClient)
//	{
//		LC_Cmd_DelAchievement msgDel;
//		SetMsgInfo(msgDel,GetMsgType(Main_Achievement, LC_DelAchievement), sizeof(LC_Cmd_DelAchievement));
//		msgDel.AchievementID = AchievementID;
//		m_pRole->SendDataToClient(&msgDel);
//	}
//	return true;
//}
RoleAchievementBase* RoleAchievementManager::CreateAchievementByEventID(BYTE EventID)
{
	switch (EventID)
	{
	case ET_GetGlobel:
		return new GetGlobelRoleAchievement();
	case ET_GetMadel:
		return new GetMadelRoleAchievement();
	case ET_GetCurren:
		return new GetCurrenRoleAchievement();
	case ET_UpperLevel:
		return new UpperLevelRoleAchievement();
	case ET_CatchFish:
		return new CatchFishRoleAchievement();
	case ET_SendGiff:
		return new SendGiffRoleAchievement();
	case ET_UseSkill:
		return new UseSkillRoleAchievement();
	case ET_LauncherGlobel:
		return new LauncherGlobelRoleAchievement();
	case ET_MaxGlobelSum:
		return new MaxGlobelSumRoleAchievement();
	case ET_ToLevel:
		return new ToLevelRoleAchievement();
	case ET_MaxCurren:
		return new MaxCurrenRoleAchievement();
	case ET_Recharge:
		return new RechargeRoleAchievement();
	default:
		return NULL;
	}
}
void RoleAchievementManager::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (m_AchievementVec.empty())
		return;
	std::vector<RoleAchievementBase*>::iterator Iter = m_AchievementVec.begin();
	for (; Iter != m_AchievementVec.end(); ++Iter)
	{
		if (!(*Iter))
		{
			ASSERT(false);
			continue;
		}
		(*Iter)->OnHandleEvent(EventID, BindParam, Param);
	}
}

//任务基类
RoleAchievementBase::RoleAchievementBase()
{
	m_pAchievementManager = NULL;
	m_EventIsFinish = false;
	m_IsNeedSave = false;
	//m_AchievementConfig = NULL;
	m_AchievementInfo.AchievementID = 0;
	m_AchievementInfo.AchievementValue = 0;
}
RoleAchievementBase::~RoleAchievementBase()
{
}
bool RoleAchievementBase::OnInit(CRoleEx* pRole, RoleAchievementManager* pManager, tagRoleAchievementInfo* pInfo)
{
	if (!pInfo || !pManager || !pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	m_pAchievementManager = pManager;
	m_AchievementInfo.AchievementID = pInfo->AchievementID;
	m_AchievementInfo.AchievementValue = pInfo->AchievementValue;
	m_IsNeedSave = false;
	HashMap<BYTE, tagAchievementConfig>::iterator Iter = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (Iter == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return false;
	}
	//m_AchievementConfig = Iter->second;
	m_EventIsFinish = (Iter->second.AchievementEventInfo.FinishValue <= m_AchievementInfo.AchievementValue);
	return true;
}
bool RoleAchievementBase::OnInit(CRoleEx* pRole, RoleAchievementManager* pManager, BYTE AchievementID, bool IsNeedSave)
{
	if (!pManager || !pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	m_pAchievementManager = pManager;
	m_AchievementInfo.AchievementID = AchievementID;
	m_AchievementInfo.AchievementValue = 0;
	m_IsNeedSave = !IsNeedSave;
	HashMap<BYTE, tagAchievementConfig>::iterator Iter = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (Iter == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return false;
	}
	//m_AchievementConfig = Iter->second;
	m_EventIsFinish = false;
	OnJoinAchievement();
	m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Achievement);
	return true;
}
void RoleAchievementBase::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//处理逻辑
	if (m_EventIsFinish)
		return;
	/*if (!m_AchievementConfig)
	{
		ASSERT(false);
		return;
	}*/
	HashMap<BYTE, tagAchievementConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (IterConfig == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end() )
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.AchievementEventInfo.EventID != EventID)
	{
		return;
	}
	m_AchievementInfo.AchievementValue += Param;
	m_IsNeedSave = true;
	m_pRole->SetRoleIsNeedSave();
	if (m_AchievementInfo.AchievementValue >= IterConfig->second.AchievementEventInfo.FinishValue)
	{
		//任务完成了 
		m_EventIsFinish = true;
		m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Achievement);

		LC_Cmd_AchievementAllEventFinish msg;
		SetMsgInfo(msg,GetMsgType(Main_Achievement, LC_AchievementAllEventFinish), sizeof(LC_Cmd_AchievementAllEventFinish));
		msg.AchievementID = m_AchievementInfo.AchievementID;
		m_pRole->SendDataToClient(&msg);
	}
}
void RoleAchievementBase::OnJoinAchievement()
{

}

//任务具体的对象处理流程
void GetGlobelRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_GetGlobel || Param == 0)
		return;
	//对数据进行
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void GetMadelRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_GetMadel || Param == 0)
		return;
	//对数据进行
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void GetCurrenRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_GetCurren || Param == 0)
		return;
	//对数据进行
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void UpperLevelRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_UpperLevel || Param == 0)
		return;
	//对数据进行
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void CatchFishRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	HashMap<BYTE, tagAchievementConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (IterConfig == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return;
	}
	if (EventID != ET_CatchFish || Param == 0 || (BindParam != IterConfig->second.AchievementEventInfo.BindParam && IterConfig->second.AchievementEventInfo.BindParam != 0xff))
		return;
	//对数据进行
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void SendGiffRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_SendGiff || Param == 0)
		return;
	//对数据进行
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void UseSkillRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	HashMap<BYTE, tagAchievementConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (IterConfig == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return;
	}
	if (EventID != ET_UseSkill || Param == 0 || (BindParam != IterConfig->second.AchievementEventInfo.BindParam && IterConfig->second.AchievementEventInfo.BindParam != 0xff))
		return;
	//对数据进行
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void LauncherGlobelRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	HashMap<BYTE, tagAchievementConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (IterConfig == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return;
	}
	if (EventID != ET_LauncherGlobel || Param == 0 || BindParam < IterConfig->second.AchievementEventInfo.BindParam)
		return;
	//对数据进行
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void MaxGlobelSumRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	//玩家最大金币数 BindParam  
	if (EventID != ET_MaxGlobelSum || Param == 0 || Param <= m_AchievementInfo.AchievementValue)
		return;
	//对数据进行
	if (!m_EventIsFinish)
		m_AchievementInfo.AchievementValue = 0;//先清空 在设置
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param); 
}
void ToLevelRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_ToLevel || Param == 0 || Param <= m_AchievementInfo.AchievementValue)
		return;
	//对数据进行
	if (!m_EventIsFinish)
		m_AchievementInfo.AchievementValue = 0;//先清空 在设置
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void MaxCurrenRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_MaxCurren || Param == 0 || Param <= m_AchievementInfo.AchievementValue)
		return;
	//对数据进行
	if (!m_EventIsFinish)
		m_AchievementInfo.AchievementValue = 0;//先清空 在设置
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}
void RechargeRoleAchievement::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_Recharge || Param == 0 || Param <= m_AchievementInfo.AchievementValue)
		return;
	if (!m_EventIsFinish)
		m_AchievementInfo.AchievementValue = 0;//先清空 在设置
	RoleAchievementBase::OnHandleEvent(EventID, BindParam, Param);
}

void MaxGlobelSumRoleAchievement::OnJoinAchievement()
{
	//设置属性
	HashMap<BYTE, tagAchievementConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (IterConfig == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.AchievementEventInfo.EventID != ET_MaxGlobelSum)
		return;
	OnHandleEvent(ET_MaxGlobelSum, 0, m_pRole->GetRoleInfo().dwGlobeNum);
}
void ToLevelRoleAchievement::OnJoinAchievement()
{
	//设置属性
	HashMap<BYTE, tagAchievementConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (IterConfig == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.AchievementEventInfo.EventID != ET_ToLevel)
		return;
	OnHandleEvent(ET_ToLevel, 0, m_pRole->GetRoleInfo().wLevel);
}
void MaxCurrenRoleAchievement::OnJoinAchievement()
{
	//设置属性
	HashMap<BYTE, tagAchievementConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (IterConfig == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.AchievementEventInfo.EventID != ET_MaxCurren)
		return;
	OnHandleEvent(ET_MaxCurren, 0, m_pRole->GetRoleInfo().dwCurrencyNum);
}
void RechargeRoleAchievement::OnJoinAchievement()
{
	//设置属性
	HashMap<BYTE, tagAchievementConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.find(m_AchievementInfo.AchievementID);
	if (IterConfig == g_FishServer.GetFishConfig().GetAchievementConfig().m_AchievementMap.end())
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.AchievementEventInfo.EventID != ET_Recharge)
		return;
	OnHandleEvent(ET_Recharge, 0, m_pRole->GetRoleInfo().TotalRechargeSum);
}