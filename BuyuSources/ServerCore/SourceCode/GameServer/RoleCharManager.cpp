#include "Stdafx.h"
#include "RoleCharManager.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleCharManager::RoleCharManager()
{
	m_IsLoadDB = false;
}
RoleCharManager::~RoleCharManager()
{

}
bool RoleCharManager::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	return true;
}
void RoleCharManager::OnLoadAllCharInfoByDB(DBO_Cmd_LoadCharInfo* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	if (pMsg->States & MsgBegin)
	{
		m_CharMap.clear();
	}
	for (WORD i = 0; i < pMsg->Sum; ++i)
	{
		tagRoleCharInfo& pInfo = pMsg->Array[i];
		HashMap<DWORD, tagRoleCharArray>::iterator Iter = m_CharMap.find(pInfo.SrcUserID);
		if (Iter == m_CharMap.end())
		{
			tagRoleCharArray pArray;
			pArray.SrcUserID = pInfo.SrcUserID;
			pArray.Array.push_back(pInfo);
			m_CharMap.insert(HashMap<DWORD, tagRoleCharArray>::value_type(pInfo.SrcUserID, pArray));
		}
		else
		{
			Iter->second.Array.push_back(pInfo);
		}
	}
	if (pMsg->States & MsgEnd)
	{
		m_IsLoadDB = true;
		m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Char);
		m_pRole->IsLoadFinish();
	}
}
void RoleCharManager::OnLoadCharMapList()
{
	DWORD PageSize = sizeof(LC_Cmd_LoadAllCharInfo)+sizeof(tagRoleCharInfo)* (m_CharMap.size() - 1);
	CheckMsgSize(PageSize);
	LC_Cmd_LoadAllCharInfo* msg = (LC_Cmd_LoadAllCharInfo*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdSize(static_cast<WORD>(PageSize));
	msg->SetCmdType(GetMsgType(Main_Char, LC_LoadAllCharInfo));
	HashMap<DWORD, tagRoleCharArray>::iterator Iter = m_CharMap.begin();
	for (WORD i =0; Iter != m_CharMap.end();)
	{
		if (Iter->second.Array.empty())
		{
			Iter = m_CharMap.erase(Iter);
			PageSize -= sizeof(tagRoleCharInfo);
			continue;
		}
		msg->Array[i] = Iter->second.Array[Iter->second.Array.size()-1];
		++Iter;
		++i;
	}
	std::vector<LC_Cmd_LoadAllCharInfo*> pVec;
	SqlitMsg(msg, PageSize, m_CharMap.size(), true, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_LoadAllCharInfo*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			m_pRole->SendDataToClient(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
}
void RoleCharManager::OnLoadAllCharInfoByUserID(DWORD dwSrcUserID)
{
	HashMap<DWORD, tagRoleCharArray>::iterator Iter = m_CharMap.find(dwSrcUserID);
	if (Iter == m_CharMap.end())
	{
		ASSERT(false);
		return;
	}
	DWORD PageSize = sizeof(LC_Cmd_LoadCharListByUserID)+sizeof(tagRoleCharInfo)* (Iter->second.Array.size()- 1);
	CheckMsgSize(PageSize);
	LC_Cmd_LoadCharListByUserID* msg = (LC_Cmd_LoadCharListByUserID*)malloc(PageSize);
	msg->SetCmdSize(static_cast<WORD>(PageSize));
	msg->SetCmdType(GetMsgType(Main_Char, LC_LoadCharListByUserID));
	msg->SrcUserID = dwSrcUserID;
	vector<tagRoleCharInfo>::iterator IterVec = Iter->second.Array.begin();
	for (WORD i = 0; IterVec != Iter->second.Array.end(); ++IterVec, ++i)
	{
		msg->Array[i] = *IterVec;
	}
	std::vector<LC_Cmd_LoadCharListByUserID*> pVec;
	SqlitMsg(msg, PageSize, Iter->second.Array.size(), true, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_LoadCharListByUserID*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			m_pRole->SendDataToClient(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
	Iter->second.Array.clear();//已经拿到客户端的 我们清理掉
	m_CharMap.erase(Iter);
	//客户端获取聊天数据后 删除掉
	DBR_Cmd_DelCharInfo msgDB;
	SetMsgInfo(msgDB, DBR_DelCharInfo, sizeof(DBR_Cmd_DelCharInfo));
	msgDB.dwSrcUserID = dwSrcUserID;
	msgDB.dwDestUserID = m_pRole->GetUserID();
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Char);
}
void RoleCharManager::OnSendCharInfo(tagRoleCharInfo& pInfo)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	pInfo.SrcUserID = m_pRole->GetUserID();
	CRoleEx* pDestRole = g_FishServer.GetRoleManager()->QueryUser(pInfo.DestUserID);
	if (pDestRole)
	{
		pDestRole->GetRoleCharManager().OnAddCharInfo(pInfo);
	}
	else
	{
		GC_Cmd_SendCharInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Char, GC_SendCharInfo), sizeof(GC_Cmd_SendCharInfo));
		msg.MessageInfo = pInfo;
		g_FishServer.SendNetCmdToCenter(&msg);
	}
}
void RoleCharManager::OnDelRelation(DWORD dwDestUserID)
{
	HashMap<DWORD, tagRoleCharArray>::iterator Iter = m_CharMap.find(dwDestUserID);
	if (Iter == m_CharMap.end())
		return;
	m_CharMap.erase(Iter);
	m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Char);
}
void RoleCharManager::OnAddCharInfo(tagRoleCharInfo& pInfo)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleCharArray>::iterator Iter = m_CharMap.find(pInfo.SrcUserID);
	if (Iter == m_CharMap.end())
	{
		tagRoleCharArray pArray;
		pArray.SrcUserID = pInfo.SrcUserID;
		pArray.Array.push_back(pInfo);
		m_CharMap.insert(HashMap<DWORD, tagRoleCharArray>::value_type(pInfo.SrcUserID, pArray));
	}
	else
	{
		Iter->second.Array.push_back(pInfo);
	}
	//发送命令
	LC_Cmd_SendCharInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_Char, LC_SendCharInfo), sizeof(LC_Cmd_SendCharInfo));
	msg.MessageInfo = pInfo;
	m_pRole->SendDataToClient(&msg);

	m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Char);
}
bool RoleCharManager::GetCharMessageStates()
{
	//判断是否有新的聊天数据
	if (m_CharMap.empty())
		return false;
	return true;
}