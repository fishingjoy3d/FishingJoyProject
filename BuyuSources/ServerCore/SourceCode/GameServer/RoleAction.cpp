#include "Stdafx.h"
#include "RoleAction.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleActionManager::RoleActionManager()
{
	m_IsLoadDB = false;
	m_IsSendClient = false;
	m_pRole = NULL;
	m_ActionVec.clear();
	int256Handle::Clear(m_JoinActionLog);
}
RoleActionManager::~RoleActionManager()
{
	//SaveAction();//先将当前正在进行的任务全部保存一次
	OnDestroy();//删除对象
}
//void RoleActionManager::OnSaveByUpdate()
//{
//	//服务器端没15分钟保存一次
//	SaveAction();
//}
RoleActionBase* RoleActionManager::QueryAction(BYTE ActionID)
{
	//查找
	if (m_ActionVec.empty())
		return NULL;
	std::vector<RoleActionBase*>::iterator Iter = m_ActionVec.begin();
	for (; Iter != m_ActionVec.end(); ++Iter)
	{
		if (!(*Iter))
		{
			ASSERT(false);
			continue;
		}
		if ((*Iter)->GetActionID() == ActionID)
		{
			return *Iter;
		}
	}
	return NULL;
}
void RoleActionManager::UpdateByHour()//每小时 0分钟开始精准更新
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//按小时对当前的获得进行更新
	if (m_ActionVec.size() == 0)
		return;
	std::vector<RoleActionBase*>::iterator Iter = m_ActionVec.begin();
	for (; Iter != m_ActionVec.end();)
	{
		//判断活动是否在时间内 不在的话 将获得删除掉
		if (!(*Iter))
		{
			ASSERT(false);
			++Iter;
			continue;
		}
		if ((*Iter)->IsInTime())
		{
			++Iter;
			continue;
		}
		DBR_Cmd_DelRoleAction msg;
		SetMsgInfo(msg, DBR_DelRoleAction, sizeof(DBR_Cmd_DelRoleAction));
		msg.dwUserID = m_pRole->GetUserID();
		msg.bActionID = (*Iter)->GetActionID();
		g_FishServer.SendNetCmdToSaveDB(&msg);
		//4.将对象删除掉
		int256Handle::SetBitStates(m_JoinActionLog, (*Iter)->GetActionID(), false);

		if (m_IsSendClient)
		{
			LC_Cmd_DelAction msgDel;
			SetMsgInfo(msgDel, GetMsgType(Main_Action, LC_DelAction), sizeof(LC_Cmd_DelAction));
			msgDel.ActionID = (*Iter)->GetActionID();
			m_pRole->SendDataToClient(&msgDel);
		}
		delete *Iter;
		Iter = m_ActionVec.erase(Iter);
	}
	//处理完毕后 我们想要让玩家重新接取活动
	OnJoinActionByConfig(false);
}
bool RoleActionManager::IsCanJoinAction(BYTE ActionID)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	//是否可以接取指定的任务
	//0.任务数量的要求
	if (m_ActionVec.size() >= g_FishServer.GetFishConfig().GetActionConfig().m_MaxJoinActionSum)
	{
		return false;
	}
	//判断 任务是否已经接取了  就是当前Vector里面是否有当前的ID的存在
	if (int256Handle::GetBitStates(m_JoinActionLog, ActionID))
	{
		return false;
	}
	//1.判断任务是否已经完成了
	if (int256Handle::GetBitStates(m_pRole->GetRoleInfo().ActionStates, ActionID))
	{
		return false;//任务已经完成了
	}
	//2.等级符合要求
	HashMap<BYTE, tagActionConfig>::iterator Iter = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(ActionID);
	if (Iter == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return false;
	}
	if (!Iter->second.IsInTime())
		return false;
	return true;
}
void RoleActionManager::OnDestroy()
{
	if (m_ActionVec.size() == 0)
		return;
	std::vector<RoleActionBase*>::iterator Iter = m_ActionVec.begin();
	for (; Iter != m_ActionVec.end(); ++Iter)
	{
		delete (*Iter);
	}
	m_ActionVec.clear();
	int256Handle::Clear(m_JoinActionLog);
}
//void RoleActionManager::SaveAction()
//{
//	//保持当前全部的任务
//	if (!m_pRole)
//	{
//		ASSERT(false);
//		return;
//	}
//	if (m_ActionVec.size() == 0)
//		return;
//	std::vector<RoleActionBase*>::iterator Iter = m_ActionVec.begin();
//	for (; Iter != m_ActionVec.end(); ++Iter)
//	{
//		if (!(*Iter)->IsNeedSave())
//			continue;
//		DBR_Cmd_SaveRoleAction msg;
//		SetMsgInfo(msg, DBR_SaveRoleAction, sizeof(DBR_Cmd_SaveRoleAction));
//		msg.dwUserID = m_pRole->GetUserID();
//		msg.ActionInfo = (*Iter)->GetActionInfo();
//		g_FishServer.SendNetCmdToDB(&msg);
//	}
//}
void RoleActionManager::GetAllNeedSaveAction(vector<tagRoleActionInfo>& pVec)
{
	pVec.clear();
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (m_ActionVec.size() == 0)
		return;
	std::vector<RoleActionBase*>::iterator Iter = m_ActionVec.begin();
	for (; Iter != m_ActionVec.end(); ++Iter)
	{
		if (!(*Iter))
			continue;
		if (!(*Iter)->IsNeedSave())
			continue;
		pVec.push_back((*Iter)->GetActionInfo());
		(*Iter)->OnSave();
	}
}
bool RoleActionManager::GetActionMessageStates()
{
	std::vector<RoleActionBase*>::iterator Iter = m_ActionVec.begin();
	for (; Iter != m_ActionVec.end(); ++Iter)
	{
		if (!(*Iter))
			continue;
		if ((*Iter)->IsExistsFinishEvent())
			return true;
	}
	return false;
}
void RoleActionManager::OnResetJoinAllAction()
{
	std::vector<RoleActionBase*>::iterator Iter = m_ActionVec.begin();
	for (; Iter != m_ActionVec.end(); ++Iter)
	{
		if (!(*Iter))
			continue;
		(*Iter)->OnJoinAction();
	}
}
bool RoleActionManager::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	SendLoadAllActionInfoToDB();
	return true;
}
void RoleActionManager::SendLoadAllActionInfoToDB()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//DBR_Cmd_LoadRoleAction msg;
	//SetMsgInfo(msg, DBR_LoadRoleAction, sizeof(DBR_Cmd_LoadRoleAction));
	//msg.dwUserID = m_pRole->GetUserID();
	//g_FishServer.SendNetCmdToDB(&msg);
}
void RoleActionManager::OnLoadAllActionInfoByDB(DBO_Cmd_LoadRoleAction* pDB)
{
	if (!pDB || !m_pRole)
	{
		ASSERT(false);
		return;
	}
	if ((pDB->States & MsgBegin) != 0)
	{
		m_ActionVec.clear();
	}
	for (WORD i = 0; i < pDB->Sum; ++i)
	{
		//UserID ActionID Param 任务的数据库表 3个参数就可以了
		HashMap<BYTE, tagActionConfig>::iterator Iter = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(pDB->Array[i].ActionID);
		if (Iter == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end() || !IsCanJoinAction(pDB->Array[i].ActionID))
		{
			//将任务删除掉
			DBR_Cmd_DelRoleAction msg;
			SetMsgInfo(msg, DBR_DelRoleAction, sizeof(DBR_Cmd_DelRoleAction));
			msg.dwUserID = m_pRole->GetUserID();
			msg.bActionID = pDB->Array[i].ActionID;
			g_FishServer.SendNetCmdToSaveDB(&msg);
			continue;
		}
		else
		{
			//生成一个对象
			RoleActionBase* pBase = CreateActionByEventID(Iter->second.EventID);//根据事件ID 创建一个任务对象
			if (!pBase || !pBase->OnInit(m_pRole, this, &pDB->Array[i]))
			{
				ASSERT(false);
				continue;
			}
			//存入集合里面去
			m_ActionVec.push_back(pBase);
			int256Handle::SetBitStates(m_JoinActionLog, pDB->Array[i].ActionID, true);
			continue;
		}
	}
	if ((pDB->States & MsgEnd) != 0)
	{
		OnJoinActionByConfig(false);
		m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Action);
		m_IsLoadDB = true;
		m_pRole->IsLoadFinish();
	}
}
void RoleActionManager::SendAllActionToClient()
{
	//将当前正在进行的任务发送到客户端去
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//客户端向服务器端询问全部的任务信息 
	DWORD PageSize = sizeof(LC_Cmd_GetRoleActionInfo)+(m_ActionVec.size() - 1)*sizeof(tagRoleActionInfo);
	LC_Cmd_GetRoleActionInfo * msg = (LC_Cmd_GetRoleActionInfo*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Action, LC_GetRoleActionInfo));
	std::vector<RoleActionBase*>::iterator Iter = m_ActionVec.begin();
	for (WORD i = 0; Iter != m_ActionVec.end(); ++Iter, ++i)
	{
		if (!(*Iter))
			continue;
		msg->Array[i] = (*Iter)->GetActionInfo();
	}
	std::vector<LC_Cmd_GetRoleActionInfo*> pVec;
	SqlitMsg(msg, PageSize, m_ActionVec.size(),true, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_GetRoleActionInfo*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			m_pRole->SendDataToClient(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
	m_IsSendClient = true;
}
void RoleActionManager::OnRoleLevelChange()
{
	OnJoinActionByConfig(false);
}
void RoleActionManager::OnJoinActionByConfig(bool IsNeedSave)
{
	if (m_ActionVec.size() >= g_FishServer.GetFishConfig().GetActionConfig().m_MaxJoinActionSum)//任务已满无法接取
		return;
	//尝试接取任务
	HashMap<BYTE, tagActionConfig>::iterator Iter = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.begin();
	for (; Iter != g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end(); ++Iter)
	{
		if (m_ActionVec.size() < g_FishServer.GetFishConfig().GetActionConfig().m_MaxJoinActionSum)
			OnJoinAction(Iter->second.ActionID, IsNeedSave);
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
		vector<tagRoleActionInfo> pVec;
		GetAllNeedSaveAction(pVec);
		if (pVec.empty())
			return;
		//创建一个命令将全部的任务保存起来
		DWORD PageSize = sizeof(DBR_Cmd_SaveAllAction)+(pVec.size() - 1)*sizeof(tagRoleActionInfo);
		DBR_Cmd_SaveAllAction* msg = (DBR_Cmd_SaveAllAction*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return;
		}
		msg->SetCmdType(DBR_SaveAllAction);
		msg->dwUserID = m_pRole->GetUserID();
		for (DWORD i = 0; i < pVec.size(); ++i)
		{
			msg->Array[i] = pVec[i];
		}

		std::vector<DBR_Cmd_SaveAllAction*> pVec2;
		SqlitMsg(msg, PageSize, pVec.size(),false, pVec2);
		free(msg);
		if (!pVec2.empty())
		{
			std::vector<DBR_Cmd_SaveAllAction*>::iterator Iter = pVec2.begin();
			for (; Iter != pVec2.end(); ++Iter)
			{
				g_FishServer.SendNetCmdToSaveDB(*Iter);
				free(*Iter);
			}
			pVec.clear();
		}
	}
}
bool RoleActionManager::OnFinishAction(BYTE ActionID, DWORD ActionOnceID)//领取任务奖励的时候
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	//当一个任务完成的时候 我们进行处理
	//1.判断任务是否存在
	if (!int256Handle::GetBitStates(m_JoinActionLog, ActionID))
	{
		return false;
	}
	HashMap<BYTE, tagActionConfig>::iterator Iter = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(ActionID);
	if (Iter == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		return false;
	}
	std::vector<RoleActionBase*>::iterator IterAction = m_ActionVec.begin();
	for (; IterAction != m_ActionVec.end(); ++IterAction)
	{
		if (!(*IterAction))
			continue;
		if ((*IterAction)->GetActionID() == ActionID)
		{
			//领取全部的当前可以领取的活动的奖励
			HashMap<DWORD, tagActionEventConfig>::iterator IterReward = Iter->second.EventMap.find(ActionOnceID);
			if (IterReward == Iter->second.EventMap.end())
				return false;
			if ((*IterAction)->GetActionInfo().ActionValue < IterReward->second.FinishValue || ((*IterAction)->GetActionInfo().ActionStates & IterReward->first) != 0)
				return false;
			m_pRole->OnAddRoleRewardByRewardID(IterReward->second.RewardID,TEXT("完成活动阶段奖励"));//玩家获得奖励
			(*IterAction)->GetActionInfo().ActionStates |= IterReward->first;//设置玩家已经领取了当前的奖励
			if (Iter->second.FinishValue != (*IterAction)->GetActionInfo().ActionStates)
			{
				//不需要删除活动 
				//保存活动 的 当前数据 阶段性的保存 非完成状态
				DBR_Cmd_SaveRoleAction msg;
				SetMsgInfo(msg, DBR_SaveRoleAction, sizeof(DBR_Cmd_SaveRoleAction));
				msg.dwUserID = m_pRole->GetUserID();
				msg.ActionInfo = (*IterAction)->GetActionInfo();
				g_FishServer.SendNetCmdToSaveDB(&msg);

				if (m_IsSendClient)
				{
					//告诉客户端 指定的活动的姿态发生变化了 领取部分奖励了
					LC_Cmd_GetOnceActionInfo msg;
					SetMsgInfo(msg, GetMsgType(Main_Action, LC_GetOnceActionInfo), sizeof(LC_Cmd_GetOnceActionInfo));
					msg.ActionInfo = (*IterAction)->GetActionInfo();
					m_pRole->SendDataToClient(&msg);
				}
			}
			else
			{
				int256Handle::SetBitStates(m_JoinActionLog, ActionID, false);//
				m_pRole->ChangeRoleActionStates(ActionID, true);//指定活动已经完成了 可以取消了
				if (m_IsSendClient)
				{
					LC_Cmd_GetActionReward msg;//告诉客户端指定活动已经完成了
					SetMsgInfo(msg, GetMsgType(Main_Action, LC_GetActionReward), sizeof(LC_Cmd_GetActionReward));
					msg.ActionID = ActionID;
					m_pRole->SendDataToClient(&msg);
				}

				DBR_Cmd_DelRoleAction msgDel;
				SetMsgInfo(msgDel, DBR_DelRoleAction, sizeof(DBR_Cmd_DelRoleAction));
				msgDel.dwUserID = m_pRole->GetUserID();
				msgDel.bActionID = ActionID;
				g_FishServer.SendNetCmdToSaveDB(&msgDel);

				delete *IterAction;
				m_ActionVec.erase(IterAction);

				//7.因为玩家完成了一个任务 
				OnJoinActionByConfig(false);//玩家需要重新接取一个任务 补充缺失
				
			}
			m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Action);
			return true;
		}
	}
	return false;
}
bool RoleActionManager::OnJoinAction(BYTE ActionID, bool IsNeedSave)//接取一个任务的时候
{
	if (!m_pRole)
	{
		ASSERT(false);
		return false;
	}
	//当玩家接取一个新的任务的时候 我们进行处理
	//1.判断任务是否存在
	HashMap<BYTE, tagActionConfig>::iterator Iter = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(ActionID);
	if (Iter == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		return false;
	}
	if (!IsCanJoinAction(ActionID))
		return false;
	RoleActionBase* pAction = CreateActionByEventID(Iter->second.EventID);
	if (!pAction)
	{
		ASSERT(false);
		return false;
	}
	m_ActionVec.push_back(pAction);
	pAction->OnInit(m_pRole, this, ActionID, IsNeedSave);//初始化任务
	int256Handle::SetBitStates(m_JoinActionLog, ActionID, true);
	//4.发送命令到客户端去
	if (m_IsSendClient)
	{
		LC_Cmd_JoinAction msg;
		SetMsgInfo(msg, GetMsgType(Main_Action, LC_JoinAction), sizeof(LC_Cmd_JoinAction));
		msg.ActionID = ActionID;
		m_pRole->SendDataToClient(&msg);
	}
	if (IsNeedSave)
	{
		DBR_Cmd_SaveRoleAction msg;
		SetMsgInfo(msg, DBR_SaveRoleAction, sizeof(DBR_Cmd_SaveRoleAction));
		msg.dwUserID = m_pRole->GetUserID();
		msg.ActionInfo = pAction->GetActionInfo();
		g_FishServer.SendNetCmdToSaveDB(&msg);
	}
	return true;
}
RoleActionBase* RoleActionManager::CreateActionByEventID(BYTE EventID)
{
	switch (EventID)
	{
	case ET_GetGlobel:
		return new GetGlobelRoleAction();
	case ET_GetMadel:
		return new GetMadelRoleAction();
	case ET_GetCurren:
		return new GetCurrenRoleAction();
	case ET_UpperLevel:
		return new UpperLevelRoleAction();
	case ET_CatchFish:
		return new CatchFishRoleAction();
	case ET_SendGiff:
		return new SendGiffRoleAction();
	case ET_UseSkill:
		return new UseSkillRoleAction();
	case ET_LauncherGlobel:
		return new LauncherGlobelRoleAction();
	case ET_MaxGlobelSum:
		return new MaxGlobelSumRoleAction();
	case ET_ToLevel:
		return new ToLevelRoleAction();
	case ET_MaxCurren:
		return new MaxCurrenRoleAction();
	case ET_Recharge:
		return new RechargeRoleAction();
	default:
		return NULL;
	}
}
void RoleActionManager::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (m_ActionVec.empty())
		return;
	std::vector<RoleActionBase*>::iterator Iter = m_ActionVec.begin();
	for (; Iter != m_ActionVec.end(); ++Iter)
	{
		(*Iter)->OnHandleEvent(EventID, BindParam, Param);
	}
}

//任务基类
RoleActionBase::RoleActionBase()
{
	m_pActionManager = NULL;
	m_IsNeedSave = false;
	//m_ActionConfig = NULL;
	m_ActionInfo.ActionID = 0;
	m_ActionInfo.ActionValue = 0;
	m_ActionInfo.ActionStates = 0;
}
RoleActionBase::~RoleActionBase()
{
}
bool RoleActionBase::OnInit(CRoleEx* pRole, RoleActionManager* pManager, tagRoleActionInfo* pInfo)
{
	if (!pInfo || !pManager || !pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	m_pActionManager = pManager;
	m_ActionInfo.ActionID = pInfo->ActionID;
	m_ActionInfo.ActionValue = pInfo->ActionValue;
	m_ActionInfo.ActionStates = pInfo->ActionStates;//设置活动的状态的BUG
	m_IsNeedSave = false;
	HashMap<BYTE, tagActionConfig>::iterator Iter = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (Iter == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return false;
	}
	//m_ActionConfig = Iter->second;
	return true;
}
bool RoleActionBase::OnInit(CRoleEx* pRole, RoleActionManager* pManager, BYTE ActionID, bool IsNeedSave)
{
	if (!pManager || !pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	m_pActionManager = pManager;
	m_ActionInfo.ActionID = ActionID;
	m_ActionInfo.ActionValue = 0;
	m_ActionInfo.ActionStates = 0;
	m_IsNeedSave = !IsNeedSave;
	HashMap<BYTE, tagActionConfig>::iterator Iter = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (Iter == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return false;
	}
	//m_ActionConfig = Iter->second;
	OnJoinAction();
	m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Action);
	return true;
}
bool RoleActionBase::IsInTime()
{
	/*if (!m_ActionConfig)
		return false;*/
	HashMap<BYTE, tagActionConfig>::iterator IterGroup = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterGroup == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return false;
	}
	return IterGroup->second.IsInTime();
}
void RoleActionBase::OnJoinAction()
{

}
bool RoleActionBase::IsExistsFinishEvent()
{
	//是否存在已经完成的阶段 但是未领取的
	HashMap<BYTE, tagActionConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterConfig == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return false;
	}

	HashMap<DWORD, tagActionEventConfig>::iterator Iter = IterConfig->second.EventMap.begin();
	for (; Iter != IterConfig->second.EventMap.end(); ++Iter)
	{
		if (m_ActionInfo.ActionValue >= Iter->second.FinishValue && (m_ActionInfo.ActionStates & Iter->first) == 0)
			return true;
	}
	return false;
}
void RoleActionBase::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	//处理逻辑
	if (/*!m_ActionConfig || */!m_pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, tagActionConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterConfig == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.EventID != EventID)
	{
		return;
	}
	m_ActionInfo.ActionValue += Param;
	m_IsNeedSave = true;
	m_pRole->SetRoleIsNeedSave();
	m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Action);
}


//任务具体的对象处理流程
void GetGlobelRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_GetGlobel || Param == 0)
		return;
	//对数据进行
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void GetMadelRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_GetMadel || Param == 0)
		return;
	//对数据进行
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void GetCurrenRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_GetCurren || Param == 0)
		return;
	//对数据进行
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void UpperLevelRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_UpperLevel || Param == 0)
		return;
	//对数据进行
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void CatchFishRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	HashMap<BYTE, tagActionConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterConfig == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return;
	}
	if (EventID != ET_CatchFish || Param == 0 || (BindParam != IterConfig->second.BindParam  && IterConfig->second.BindParam != 0xff))
		return;
	//对数据进行
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void SendGiffRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_SendGiff || Param == 0)
		return;
	//对数据进行
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void UseSkillRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	HashMap<BYTE, tagActionConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterConfig == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return;
	}
	if (EventID != ET_UseSkill || Param == 0 || (BindParam != IterConfig->second.BindParam  && IterConfig->second.BindParam != 0xff))
		return;
	//对数据进行
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void LauncherGlobelRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	HashMap<BYTE, tagActionConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterConfig == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return;
	}
	if (EventID != ET_LauncherGlobel || Param == 0 || BindParam < IterConfig->second.BindParam)
		return;
	//对数据进行
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void MaxGlobelSumRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_MaxGlobelSum || Param == 0 || Param <= m_ActionInfo.ActionValue)
		return;
	//对数据进行
	m_ActionInfo.ActionValue = 0;//先清空 在设置
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void ToLevelRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_ToLevel || Param == 0 || Param <= m_ActionInfo.ActionValue)
		return;
	//对数据进行
	m_ActionInfo.ActionValue = 0;//先清空 在设置
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void MaxCurrenRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_MaxCurren || Param == 0 || Param <= m_ActionInfo.ActionValue)
		return;
	//对数据进行
	m_ActionInfo.ActionValue = 0;//先清空 在设置
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void RechargeRoleAction::OnHandleEvent(BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (EventID != ET_Recharge || Param == 0 || Param <= m_ActionInfo.ActionValue)
		return;
	m_ActionInfo.ActionValue = 0;//先清空 在设置
	RoleActionBase::OnHandleEvent(EventID, BindParam, Param);
}
void MaxGlobelSumRoleAction::OnJoinAction()
{
	//设置属性
	HashMap<BYTE, tagActionConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterConfig == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.EventID != ET_MaxGlobelSum)
		return;
	OnHandleEvent(ET_MaxGlobelSum, 0, m_pRole->GetRoleInfo().dwGlobeNum);
}
void ToLevelRoleAction::OnJoinAction()
{
	//设置属性
	HashMap<BYTE, tagActionConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterConfig == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.EventID != ET_ToLevel)
		return;
	OnHandleEvent(ET_ToLevel, 0,m_pRole->GetRoleInfo().wLevel);
}
void MaxCurrenRoleAction::OnJoinAction()
{
	//设置属性
	HashMap<BYTE, tagActionConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterConfig == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.EventID != ET_MaxCurren)
		return;
	OnHandleEvent(ET_MaxCurren, 0,m_pRole->GetRoleInfo().dwCurrencyNum);
}
void RechargeRoleAction::OnJoinAction()
{
	//设置属性
	HashMap<BYTE, tagActionConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.find(m_ActionInfo.ActionID);
	if (IterConfig == g_FishServer.GetFishConfig().GetActionConfig().m_ActionMap.end())
	{
		ASSERT(false);
		return;
	}
	if (IterConfig->second.EventID != ET_Recharge)
		return;
	OnHandleEvent(ET_Recharge, 0, m_pRole->GetRoleInfo().TotalRechargeSum);
}