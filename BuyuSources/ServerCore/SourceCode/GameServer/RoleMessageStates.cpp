#include "StdAfx.h"
#include "RoleMessageStates.h"
#include "RoleEx.h"
RoleMessageStates::RoleMessageStates()
{
	m_IsInit = false;
}
RoleMessageStates::~RoleMessageStates()
{

}
void RoleMessageStates::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	m_pRole = pRole;
	m_StatesValue = 0;
	m_IsInit = true;
	//根据各个管理器的设置最终的值
	OnChangeRoleMessageStates(RMT_Mail, false);
	OnChangeRoleMessageStates(RMT_WeekRank, false);
	OnChangeRoleMessageStates(RMT_Giff, false);
	OnChangeRoleMessageStates(RMT_Task, false);
	OnChangeRoleMessageStates(RMT_Achievement, false);
	OnChangeRoleMessageStates(RMT_Action, false);
	OnChangeRoleMessageStates(RMT_Check, false);
	OnChangeRoleMessageStates(RMT_Char, false);
	OnChangeRoleMessageStates(RMT_Relation, false);
	//将数据自动发送到客户端去
	LC_Cmd_RoleMessageChange msg;
	SetMsgInfo(msg, GetMsgType(Main_RoleMessage, LC_RoleMessageChange), sizeof(LC_Cmd_RoleMessageChange));
	msg.RoleMessageData = m_StatesValue;
	m_pRole->SendDataToClient(&msg);
}
void RoleMessageStates::OnChangeRoleMessageStates(RoleMessageType Type, bool IsSendToClient)
{
	if (!m_pRole)
	{
		//ASSERT(false);
		return;
	}
	if (!m_IsInit)
		return;//未初始化 
	DWORD OldStates = m_StatesValue;
	//当指定类型变化的时候 我们重新获取类型的数据下
	switch (Type)
	{
	case RMT_Mail:
		{
			bool States = m_pRole->GetMailManager().GetMailMessageStates();
			if ((m_StatesValue & RMT_Mail) !=0)
				m_StatesValue ^= RMT_Mail;
			if (States)
				m_StatesValue |= RMT_Mail;
		}
		break;
	case RMT_WeekRank:
		{
			bool States = m_pRole->GetRoleRank().GetRankMessageStates();
			if ((m_StatesValue & RMT_WeekRank) != 0)
				m_StatesValue ^= RMT_WeekRank;
			if (States)
				m_StatesValue |= RMT_WeekRank;
		}
		break;
	case RMT_Giff:
		{
			bool States = m_pRole->GetRoleGiffManager().GetGiffMessageStates();
			if ((m_StatesValue & RMT_Giff) != 0)
				m_StatesValue ^= RMT_Giff;
			if (States)
				m_StatesValue |= RMT_Giff;
		}
		break;
	case RMT_Task:
		{
			bool States = m_pRole->GetRoleTaskManager().GetTaskMessageStates();
			if ((m_StatesValue & RMT_Task) != 0)
				m_StatesValue ^= RMT_Task;
			if (States)
				m_StatesValue |= RMT_Task;
		}
		break;
	case RMT_Achievement:
		{
			bool States = m_pRole->GetRoleAchievementManager().GetAchievementMessageStates();
			if ((m_StatesValue & RMT_Achievement) != 0)
				m_StatesValue ^= RMT_Achievement;
			if (States)
				m_StatesValue |= RMT_Achievement;
		}
		break;
	case RMT_Action:
		{
			bool States = m_pRole->GetRoleActionManager().GetActionMessageStates();
			if ((m_StatesValue & RMT_Action) != 0)
				m_StatesValue ^= RMT_Action;
			if (States)
				m_StatesValue |= RMT_Action;
		}
		break;
	case RMT_Check:
		{
			bool States = m_pRole->GetRoleCheckManager().GetCheckMessageStates();
			if ((m_StatesValue & RMT_Check) != 0)
				m_StatesValue ^= RMT_Check;
			if (States)
				m_StatesValue |= RMT_Check;
		}
		break;
	case RMT_Char:
		{
			bool States = m_pRole->GetRoleCharManager().GetCharMessageStates();
			if ((m_StatesValue & RMT_Char) != 0)
				m_StatesValue ^= RMT_Char;
			if (States)
				m_StatesValue |= RMT_Char;
		}
		break;
	case RMT_Relation:
		{
			bool States = m_pRole->GetRoleRelationRequest().GetRelationRequestMessageStates();
			if ((m_StatesValue & RMT_Relation) != 0)
				m_StatesValue ^= RMT_Relation;
			if (States)
				m_StatesValue |= RMT_Relation;
		}
	}
	//如果值变化了 发送命令到客户端去 
	if (OldStates == m_StatesValue)
		return;
	//发送到客户端去
	if (IsSendToClient)
	{
		LC_Cmd_RoleMessageChange msg;
		SetMsgInfo(msg, GetMsgType(Main_RoleMessage, LC_RoleMessageChange), sizeof(LC_Cmd_RoleMessageChange));
		msg.RoleMessageData = m_StatesValue;
		m_pRole->SendDataToClient(&msg);
	}
}