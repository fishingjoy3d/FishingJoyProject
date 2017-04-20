#include "Stdafx.h"
#include "RoleRelationRequest.h"
#include "RoleEx.h"
#include "FishServer.h"
RoleRelationRequest::RoleRelationRequest()
{
	m_WriteHandleMap.clear();
}
RoleRelationRequest::~RoleRelationRequest()
{

}
bool RoleRelationRequest::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	m_IsLoadDB = false;
	return true;
}
void RoleRelationRequest::OnLoadRelationRequestResult(DBO_Cmd_LoadRelationRequest* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (pMsg->States & MsgBegin)
	{
		m_RequestMap.clear();
	}
	DBR_Cmd_DelRelationRequest msgDB;
	SetMsgInfo(msgDB, DBR_DelRelationRequest, sizeof(DBR_Cmd_DelRelationRequest));
	msgDB.dwUserID = m_pRole->GetUserID();
	for (WORD i = 0; i < pMsg->Sum; ++i)
	{
		if (CheckRelationRequestIsTimeOut(pMsg->Array[i]))
		{
			msgDB.ID = pMsg->Array[i].ID;
			g_FishServer.SendNetCmdToSaveDB(&msgDB);
		}
		else
		{
			m_RequestMap.insert(HashMap<DWORD, tagRelationRequest>::value_type(pMsg->Array[i].ID, pMsg->Array[i]));
		}
	}
	if (pMsg->States & MsgEnd)
	{
		m_IsLoadDB = true;
		m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Relation);
		m_pRole->IsLoadFinish();
	}
}
bool RoleRelationRequest::CheckRelationRequestIsTimeOut(tagRelationRequest& pInfo)
{
	//检测当前请求是否已经过期了
	int DiffDay = GetDiffDay(pInfo.SendTime, g_FishServer.GetFishConfig().GetWriteSec());//发送邮件到今天的天数差 
	if (DiffDay > g_FishServer.GetFishConfig().GetRelation().RelationRequestLimitDay)
		return true;
	else
		return false;
}
void RoleRelationRequest::OnDelRelationRequest(DWORD ID)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRelationRequest>::iterator Iter = m_RequestMap.find(ID);
	if (Iter != m_RequestMap.end())
		m_RequestMap.erase(Iter);
	DBR_Cmd_DelRelationRequest msgDB;
	SetMsgInfo(msgDB, DBR_DelRelationRequest, sizeof(DBR_Cmd_DelRelationRequest));
	msgDB.dwUserID = m_pRole->GetUserID();
	msgDB.ID = ID;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	LC_Cmd_DelRelationRequest msg;
	SetMsgInfo(msg, GetMsgType(Main_RelationRequest, LC_DelRelationRequest), sizeof(LC_Cmd_DelRelationRequest));
	msg.ID = ID;
	m_pRole->SendDataToClient(&msg);

	m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Relation);
}
void RoleRelationRequest::SendInfoToClient()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//客户端请求 加载数据  
	//无须分包发送 
	DWORD PageSize = sizeof(LC_Cmd_LoadRelationRequest)+(m_RequestMap.size() - 1)* sizeof(tagRelationRequest);
	CheckMsgSize(PageSize);
	LC_Cmd_LoadRelationRequest* msg = (LC_Cmd_LoadRelationRequest*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_RelationRequest, LC_LoadRelationRequest));
	msg->Sum = m_RequestMap.size();
	HashMap<DWORD, tagRelationRequest>::iterator Iter = m_RequestMap.begin();
	for (size_t i = 0; Iter != m_RequestMap.end(); ++Iter, ++i)
	{
		msg->Array[i] = Iter->second;
	}

	std::vector<LC_Cmd_LoadRelationRequest*> pVec;
	SqlitMsg(msg, PageSize, m_RequestMap.size(), true, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<LC_Cmd_LoadRelationRequest*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			m_pRole->SendDataToClient(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
}
void RoleRelationRequest::OnSendNewRequest(BYTE RelationType, DWORD DestUserID, LPTSTR MessageInfo)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//客户端请求 添加一个 新的关系请求
	DBR_Cmd_AddRelationRequest msg;
	SetMsgInfo(msg, DBR_AddRelationRequest, sizeof(DBR_Cmd_AddRelationRequest));
	msg.dwUserID = m_pRole->GetUserID();
	msg.Info.SrcUserID = m_pRole->GetUserID();
	msg.Info.SrcFaceID = 0;
	msg.Info.DestUserID = DestUserID;
	msg.Info.RelationType = RelationType;
	msg.Info.SendTime = time(null);
	msg.Info.SrcGender = false;
	msg.Info.SrcLevel = 0;
	msg.Info.SrcVipLevel = 0;
	TCHARCopy(msg.Info.SrcIPAddress, CountArray(msg.Info.SrcIPAddress), m_pRole->GetRoleInfo().IPAddress, _tcslen(m_pRole->GetRoleInfo().IPAddress));
	TCHARCopy(msg.Info.MessageInfo, CountArray(msg.Info.MessageInfo), MessageInfo, _tcslen(MessageInfo));
	g_FishServer.SendNetCmdToDB(&msg);
}
void RoleRelationRequest::OnSendNewRequestDBResult(DBO_Cmd_AddRelationRequest* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (pMsg->Result)
	{
		//添加成功了
		CRoleEx* pDestRole = g_FishServer.GetRoleManager()->QueryUser(pMsg->Info.DestUserID);
		if (pDestRole)
		{
			pDestRole->GetRoleRelationRequest().OnBeAddNewrequest(pMsg->Info);
		}
		else
		{
			GC_Cmd_SendRelationRequest msg;
			SetMsgInfo(msg, GetMsgType(Main_RelationRequest, GC_SendRelationRequest), sizeof(GC_Cmd_SendRelationRequest));
			msg.Info = pMsg->Info;
			m_pRole->SendDataToCenter(&msg);
		}

		LC_Cmd_SendRelationRequest msg;
		SetMsgInfo(msg, GetMsgType(Main_RelationRequest, LC_SendRelationRequest), sizeof(LC_Cmd_SendRelationRequest));
		msg.Info = pMsg->Info;
		msg.Result = true;
		m_pRole->SendDataToClient(&msg);
	}
	else
	{
		LC_Cmd_SendRelationRequest msg;
		SetMsgInfo(msg, GetMsgType(Main_RelationRequest, LC_SendRelationRequest), sizeof(LC_Cmd_SendRelationRequest));
		msg.Info = pMsg->Info;
		msg.Result = false;
		msg.ErrorID = pMsg->ErrorID;
		m_pRole->SendDataToClient(&msg);
		return;
	}
}
void RoleRelationRequest::OnBeAddNewrequest(tagRelationRequest& pRequest)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	m_RequestMap.insert(HashMap<DWORD, tagRelationRequest>::value_type(pRequest.ID, pRequest));
	LC_Cmd_BeAddRelationRequest msg;
	SetMsgInfo(msg, GetMsgType(Main_RelationRequest, LC_BeAddRelationRequest), sizeof(LC_Cmd_BeAddRelationRequest));
	msg.Info = pRequest;
	m_pRole->SendDataToClient(&msg);

	m_pRole->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Relation);
}
void RoleRelationRequest::OnHandleRequest(DWORD ID, bool Result)
{
	//玩家同意或者是拒绝请求
	//处理请求 核心功能
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRelationRequest>::iterator Iter = m_RequestMap.find(ID);
	if (Iter == m_RequestMap.end())
	{
		ASSERT(false);
		return;
	}
	CRoleEx* pSrcUser = g_FishServer.GetRoleManager()->QueryUser(Iter->second.SrcUserID);
	if (Result)
	{
		//玩家同意添加关系 
		CL_Cmd_AddUserRelation msg;
		msg.bRelationType = Iter->second.RelationType;
		msg.dwDestUserID = Iter->second.SrcUserID;
		if (!m_pRole->GetRelation().OnAddUserRelation(&msg))
		{
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("%s因为关系已满 无法添加"), m_pRole->GetRoleInfo().NickName);
				MailInfo.RewardID = 0;
				MailInfo.RewardSum = 0;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = false;
				CC_Cmd_SendSystemMail msg;
				SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
				msg.dwDestUserID = Iter->second.SrcUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToCenter(&msg);
			}
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("因为关系已满 无法添加好友请求"), _tcslen(TEXT("因为关系已满 无法添加好友请求")));
				MailInfo.RewardID = 0;
				MailInfo.RewardSum = 0;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = false;
				CC_Cmd_SendSystemMail msg;
				SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
				msg.dwDestUserID = Iter->second.DestUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToCenter(&msg);
			}
			//添加关系失败了 我们应该进行处理下
			if (pSrcUser)
			{
				pSrcUser->GetRoleRelationRequest().OnBeHandleRequest(Iter->second, false);
			}
			else
			{
				GC_Cmd_HandleRelationRequest msg;
				SetMsgInfo(msg, GetMsgType(Main_RelationRequest, GC_HandleRelationRequest), sizeof(GC_Cmd_HandleRelationRequest));
				msg.Info = Iter->second;
				msg.Result = false;
				g_FishServer.SendNetCmdToCenter(&msg);
			}
		}
		else
		{
			//添加关系发送到数据库去 了 我们异步等待回调
			//将当前结果存储到集合里面去
			tagRelationResult pInfo;
			pInfo.Info = Iter->second;
			pInfo.IsSrcOrDest = false;
			m_WriteHandleMap.insert(HashMap<DWORD, tagRelationResult>::value_type(pInfo.Info.ID,pInfo));
		}
		OnDelRelationRequest(Iter->first);
	}
	else
	{
		//玩家拒绝添加关系
		//发送邮件给Src 玩家
		{
			tagRoleMail	MailInfo;
			MailInfo.bIsRead = false;
			_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("%s 拒绝了您的关系申请"), m_pRole->GetRoleInfo().NickName);
			MailInfo.RewardID = 0;
			MailInfo.RewardSum = 0;
			MailInfo.MailID = 0;
			MailInfo.SendTimeLog = time(NULL);
			MailInfo.SrcFaceID = 0;
			TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
			MailInfo.SrcUserID = 0;//系统发送
			MailInfo.bIsExistsReward = false;
			CC_Cmd_SendSystemMail msg;
			SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
			msg.dwDestUserID = Iter->second.SrcUserID;
			msg.MailInfo = MailInfo;
			g_FishServer.SendNetCmdToCenter(&msg);
		}


		if (pSrcUser)
		{
			pSrcUser->GetRoleRelationRequest().OnBeHandleRequest(Iter->second, false);
		}
		else
		{
			GC_Cmd_HandleRelationRequest msg;
			SetMsgInfo(msg, GetMsgType(Main_RelationRequest, GC_HandleRelationRequest), sizeof(GC_Cmd_HandleRelationRequest));
			msg.Info = Iter->second;
			msg.Result = false;
			g_FishServer.SendNetCmdToCenter(&msg);
		}
		OnDelRelationRequest(Iter->first);
	}
}
void RoleRelationRequest::OnAddRelationResult(DWORD SrcUserID, DWORD DestUserID, BYTE RelationType, bool Result)//Dest Src
{
	//玩家异步添加关系的结果
	HashMap<DWORD, tagRelationResult>::iterator Iter = m_WriteHandleMap.begin();
	for (; Iter != m_WriteHandleMap.end();)
	{
		if (((Iter->second.IsSrcOrDest && Iter->second.Info.SrcUserID == SrcUserID && Iter->second.Info.DestUserID == DestUserID)
			|| (!Iter->second.IsSrcOrDest && Iter->second.Info.SrcUserID == DestUserID && Iter->second.Info.DestUserID == SrcUserID)) && Iter->second.Info.RelationType == RelationType)
		{
			if (Result)
			{
				if (Iter->second.IsSrcOrDest)
				{
					CRoleEx* pSrcUser = g_FishServer.GetRoleManager()->QueryUser(Iter->second.Info.SrcUserID);
					if (!pSrcUser)
					{
						ASSERT(false);
						return;
					}
					//Src 添加关系成功了 
					LC_Cmd_HandleRelationRequest msg;
					SetMsgInfo(msg, GetMsgType(Main_RelationRequest, LC_HandleRelationRequest), sizeof(LC_Cmd_HandleRelationRequest));
					msg.Info = Iter->second.Info;
					msg.Result = true;
					pSrcUser->SendDataToClient(&msg);
				}
				else
				{
					CRoleEx* pSrcUser = g_FishServer.GetRoleManager()->QueryUser(Iter->second.Info.SrcUserID);
					if (pSrcUser)
					{
						pSrcUser->GetRoleRelationRequest().OnBeHandleRequest(Iter->second.Info, true);
					}
					else
					{
						GC_Cmd_HandleRelationRequest msg;
						SetMsgInfo(msg, GetMsgType(Main_RelationRequest, GC_HandleRelationRequest), sizeof(GC_Cmd_HandleRelationRequest));
						msg.Info = Iter->second.Info;
						msg.Result = true;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
				}
			}
			else
			{
				//添加关系失败了
				if (Iter->second.IsSrcOrDest)
				{
					{
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("%s同意关系请求 但因为关系已经满了 添加失败"), Iter->second.Info.SrcNickName);
						MailInfo.RewardID = 0;
						MailInfo.RewardSum = 0;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = false;
						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = Iter->second.Info.DestUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
					{
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("因为关系已满 无法添加好友请求"), _tcslen(TEXT("因为关系已满 无法添加好友请求")));
						MailInfo.RewardID = 0;
						MailInfo.RewardSum = 0;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = false;
						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = Iter->second.Info.SrcUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}

					//1.发送命令告诉 DestUser  删除已经添加了的关系
					OnDelDestRelation(Iter->second.Info);
					//2.发送SrcUser  失败了 同时告诉目标玩家 
					CRoleEx* pSrcUser = g_FishServer.GetRoleManager()->QueryUser(Iter->second.Info.SrcUserID);
					if (pSrcUser)
					{
						LC_Cmd_HandleRelationRequest msg;
						SetMsgInfo(msg, GetMsgType(Main_RelationRequest, LC_HandleRelationRequest), sizeof(LC_Cmd_HandleRelationRequest));
						msg.Info = Iter->second.Info;
						msg.Result = false;
						pSrcUser->SendDataToClient(&msg);
					}
				}
				else
				{
					//Dest 关系已经满了 无法添加 进行描述

					{
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("%s因为关系已满 无法添加"), m_pRole->GetRoleInfo().NickName);
						MailInfo.RewardID = 0;
						MailInfo.RewardSum = 0;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = false;
						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = Iter->second.Info.SrcUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
					{
						tagRoleMail	MailInfo;
						MailInfo.bIsRead = false;
						TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("因为关系已满 无法添加好友请求"), _tcslen(TEXT("因为关系已满 无法添加好友请求")));
						MailInfo.RewardID = 0;
						MailInfo.RewardSum = 0;
						MailInfo.MailID = 0;
						MailInfo.SendTimeLog = time(NULL);
						MailInfo.SrcFaceID = 0;
						TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
						MailInfo.SrcUserID = 0;//系统发送
						MailInfo.bIsExistsReward = false;
						CC_Cmd_SendSystemMail msg;
						SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
						msg.dwDestUserID = Iter->second.Info.DestUserID;
						msg.MailInfo = MailInfo;
						g_FishServer.SendNetCmdToCenter(&msg);
					}

					CRoleEx* pSrcUser = g_FishServer.GetRoleManager()->QueryUser(Iter->second.Info.SrcUserID);
					if (pSrcUser)
					{
						pSrcUser->GetRoleRelationRequest().OnBeHandleRequest(Iter->second.Info, false);
					}
					else
					{
						GC_Cmd_HandleRelationRequest msg;
						SetMsgInfo(msg, GetMsgType(Main_RelationRequest, GC_HandleRelationRequest), sizeof(GC_Cmd_HandleRelationRequest));
						msg.Info = Iter->second.Info;
						msg.Result = false;
						g_FishServer.SendNetCmdToCenter(&msg);
					}
				}
			}

			Iter = m_WriteHandleMap.erase(Iter);
		}
		else
			++Iter;
	}
}
void RoleRelationRequest::OnBeHandleRequest(tagRelationRequest& pRequest, bool Result)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (pRequest.SrcUserID != m_pRole->GetUserID())
	{
		ASSERT(false);
		return;
	}
	if (Result)
	{
		//添加关系
		CL_Cmd_AddUserRelation msg;
		msg.bRelationType = pRequest.RelationType;
		msg.dwDestUserID = pRequest.DestUserID;
		if (!m_pRole->GetRelation().OnAddUserRelation(&msg))
		{
			//回发命令到Dest上面去 告诉玩家删除关系
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("%s同意关系请求 但因为关系已经满了 添加失败"), pRequest.SrcNickName);
				MailInfo.RewardID = 0;
				MailInfo.RewardSum = 0;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = false;
				CC_Cmd_SendSystemMail msg;
				SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
				msg.dwDestUserID = pRequest.DestUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToCenter(&msg);
			}
			{
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), TEXT("因为关系已满 无法添加好友请求"), _tcslen(TEXT("因为关系已满 无法添加好友请求")));
				MailInfo.RewardID = 0;
				MailInfo.RewardSum = 0;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = time(NULL);
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = false;
				CC_Cmd_SendSystemMail msg;
				SetMsgInfo(msg, GetMsgType(Main_Mail, CC_SendSystemMail), sizeof(CC_Cmd_SendSystemMail));
				msg.dwDestUserID = pRequest.SrcUserID;
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToCenter(&msg);
			}
			OnDelDestRelation(pRequest);
		}
		else
		{
			tagRelationResult pInfo;
			pInfo.Info = pRequest;
			pInfo.IsSrcOrDest = true;
			m_WriteHandleMap.insert(HashMap<DWORD, tagRelationResult>::value_type(pInfo.Info.ID, pInfo));
		}
	}
	else
	{
		LC_Cmd_HandleRelationRequest msg;
		SetMsgInfo(msg, GetMsgType(Main_RelationRequest, LC_HandleRelationRequest), sizeof(LC_Cmd_HandleRelationRequest));
		msg.Info = pRequest;
		msg.Result = false;
		m_pRole->SendDataToClient(&msg);
	}
}
void RoleRelationRequest::OnDelDestRelation(tagRelationRequest& pInfo)
{
	CRoleEx* pDestRole = g_FishServer.GetRoleManager()->QueryUser(pInfo.DestUserID);
	if (pDestRole)
	{
		CL_Cmd_DelUserRelation msg;
		msg.dwDestUserID = pInfo.SrcUserID;
		pDestRole->GetRelation().OnDelUserRelation(&msg);
	}
	else
	{
		GC_Cmd_DestDelRelation msg;
		SetMsgInfo(msg, GetMsgType(Main_RelationRequest, GC_DestDelRelation), sizeof(GC_Cmd_DestDelRelation));
		msg.Info = pInfo;
		g_FishServer.SendNetCmdToCenter(&msg);
	}
}
void RoleRelationRequest::OnUpdateByDay()
{
	//每天4点进行更新处理 
	HashMap<DWORD, tagRelationRequest>::iterator Iter = m_RequestMap.begin();
	for (; Iter != m_RequestMap.end();)
	{
		if (CheckRelationRequestIsTimeOut(Iter->second))
		{
			//当前已经过期了 删除掉
			DBR_Cmd_DelRelationRequest msgDB;
			SetMsgInfo(msgDB, DBR_DelRelationRequest, sizeof(DBR_Cmd_DelRelationRequest));
			msgDB.dwUserID = m_pRole->GetUserID();
			msgDB.ID = Iter->first;
			g_FishServer.SendNetCmdToSaveDB(&msgDB);

			LC_Cmd_DelRelationRequest msg;
			SetMsgInfo(msg, GetMsgType(Main_RelationRequest, LC_DelRelationRequest), sizeof(LC_Cmd_DelRelationRequest));
			msg.ID = Iter->first;
			m_pRole->SendDataToClient(&msg);


			Iter = m_RequestMap.erase(Iter);
		}
		else
			++Iter;
	}


}
void RoleRelationRequest::OnDelRelation(DWORD DestUserID, BYTE RelationType)//关系相互删除
{
	//当玩家删除一个好友的时候
	CRoleEx* pDestRole = g_FishServer.GetRoleManager()->QueryUser(DestUserID);
	if (pDestRole)
	{
		CL_Cmd_DelUserRelation msg;
		msg.dwDestUserID = m_pRole->GetUserID();
		pDestRole->GetRelation().OnDelUserRelation(&msg);
	}
	else
	{
		GC_Cmd_DestDelRelation msg;
		SetMsgInfo(msg, GetMsgType(Main_RelationRequest, GC_DestDelRelation), sizeof(GC_Cmd_DestDelRelation));
		msg.Info.SrcUserID = m_pRole->GetUserID();
		msg.Info.DestUserID = DestUserID;
		g_FishServer.SendNetCmdToCenter(&msg);
	}
}

bool RoleRelationRequest::GetRelationRequestMessageStates()
{
	return !m_RequestMap.empty();
}