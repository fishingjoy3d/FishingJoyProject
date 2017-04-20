#include "Stdafx.h"
#include "RoleMailManager.h"
#include "RoleEx.h"
#include "RoleManager.h"
#include "FishServer.h"
RoleMailManager::RoleMailManager()
{
	m_IsLoadToClient = false;
	m_IsLoadDB = false;
	m_RoleManager = NULL;
	m_pUser = NULL;
	m_ClientMailSum = 0;
}
RoleMailManager::~RoleMailManager()
{
	if (!m_RoleMailVec.empty())
	{
		std::list<tagRoleMail*>::iterator Iter = m_RoleMailVec.begin();
		for (; Iter != m_RoleMailVec.end(); ++Iter)
		{
			delete *Iter;
		}
		m_RoleMailVec.clear();
	}
}
bool RoleMailManager::OnInit(CRoleEx* pUser, RoleManager* pManager)
{
	//读取邮件数据
	if (!pUser ||  !pManager)
	{
		ASSERT(false);
		return false;
	}
	m_RoleManager = pManager;
	m_pUser = pUser;
	m_RoleMailVec.clear();
	return OnLoadUserMailByPage();//加载地0页的邮件
}
bool RoleMailManager::OnLoadUserMailByPage()
{
	//不加载全部的玩家数据
	//加载指定页面的邮件数据 发送DBR请求
	if ( !m_pUser)
	{
		ASSERT(false);
		return false;
	} 
	m_IsLoadDB = false;
	m_RoleMailVec.clear();
	//DBR_Cmd_LoadUserMail msg;
	//SetMsgInfo(msg,DBR_LoadUserMail, sizeof(DBR_Cmd_LoadUserMail));
	//msg.dwUserID = m_pUser->GetUserID();
	//g_FishServer.SendNetCmdToDB(&msg);
	return true;
}
bool RoleMailManager::OnLoadUserMailByPageResult(DBO_Cmd_LoadUserMail* pDB)
{
	//加载邮件的数据
	if (!pDB || !m_pUser)
	{
		ASSERT(false);
		return false;
	}
	if ((pDB->States & MsgBegin) != 0)
	{
		m_RoleMailVec.clear();
	}
	DBR_Cmd_DelUserMail msg;
	SetMsgInfo(msg, DBR_DelUserMail, sizeof(DBR_Cmd_DelUserMail));
	for (size_t i = 0; i < pDB->Sum; ++i)
	{
		//1.判断邮件是否过期了 过期了的话 立刻删除掉
		int DiffDay = GetDiffDay(pDB->Array[i].SendTimeLog, g_FishServer.GetFishConfig().GetWriteSec());//发送邮件到今天的天数差 
		if (DiffDay > g_FishServer.GetFishConfig().GetFishMailConfig().MailLimitDay && pDB->Array[i].SrcUserID != 0) //玩家邮件才进行删除判断 系统邮件不进行过期判断
		{
			msg.dwMailID = pDB->Array[i].MailID;
			g_FishServer.SendNetCmdToSaveDB(&msg);
			continue;
		}
		tagRoleMail* pMail = new tagRoleMail;
		memcpy_s(pMail, sizeof(tagRoleMail), &pDB->Array[i], sizeof(tagRoleMail));
		m_RoleMailVec.push_back(pMail);
	}
	if ((pDB->States & MsgEnd) != 0)
	{
		m_IsLoadDB = true;
		m_pUser->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Mail);
		m_pUser->IsLoadFinish();//判断玩家是否上线完毕
	}
	return true;
}
//void RoleMailManager::OnLoadUserMailFinish()
//{
//	if (!m_pUser)
//	{
//		ASSERT(false);
//		return;
//	}
//	
//}
bool RoleMailManager::OnGetAllUserMail()
{
	if (!m_pUser)
	{
		ASSERT(false);
		return false;
	}
	//玩家获取邮件信息
	//向客户端发送数据 把当前邮件的前多少发送到客户端去
	//必须 先发送普通邮件 最后发送系统邮件 系统邮件发送完毕 在客户端标记所有邮件已经发送完毕
	m_ClientMailSum = 0;
	//1.先发普通的邮件
	DWORD MailSum = g_FishServer.GetFishConfig().GetFishMailConfig().ClientShowMailSum;

	DWORD NormalPageSize = sizeof(LC_Cmd_NormalMail)+sizeof(tagNormalMailTitle)*(MailSum - 1);
	LC_Cmd_NormalMail * msgNormal = (LC_Cmd_NormalMail*)malloc(NormalPageSize);
	if (!msgNormal)
	{
		ASSERT(false);
		return false;
	}
	msgNormal->SetCmdType(GetMsgType(Main_Mail, LC_GetUserNormalMail));
	msgNormal->Sum = 0;

	DWORD SystemPageSize = sizeof(LC_Cmd_SystemMail)+sizeof(tagSystemMailTitle)*(MailSum - 1);
	LC_Cmd_SystemMail * msgSystem = (LC_Cmd_SystemMail*)malloc(SystemPageSize);
	if (!msgSystem)
	{
		ASSERT(false);
		return false;
	}
	msgSystem->SetCmdType(GetMsgType(Main_Mail, LC_GetUserSystemMail));
	msgSystem->Sum = 0;

	std::list<tagRoleMail*>::iterator Iter = m_RoleMailVec.begin();
	for (BYTE i = 0, j = 0, k = 0; Iter != m_RoleMailVec.end() && i < g_FishServer.GetFishConfig().GetFishMailConfig().ClientShowMailSum; ++Iter, ++i)
	{
		if ((*Iter)->SrcUserID == 0)
		{
			msgSystem->Array[k].MailID = (*Iter)->MailID;
			msgSystem->Array[k].bIsRead = (*Iter)->bIsRead;
			__int64 diffDay = GetDiffDay((*Iter)->SendTimeLog, g_FishServer.GetFishConfig().GetWriteSec());
			if (diffDay > 250)
				diffDay = 250;
			msgSystem->Array[k].bDiffTime = (BYTE)diffDay;
			if (((*Iter)->RewardID != 0 && (*Iter)->RewardSum != 0) && (*Iter)->bIsExistsReward)
				msgSystem->Array[k].bIsExistsItem = true;
			else
				msgSystem->Array[k].bIsExistsItem = false;
			++k;
			msgSystem->Sum = k;
		}
		else
		{
			//玩家邮件
			msgNormal->Array[j].MailID = (*Iter)->MailID;
			msgNormal->Array[j].bIsRead = (*Iter)->bIsRead;
			msgNormal->Array[j].SrcFaceID = (*Iter)->SrcFaceID;
			msgNormal->Array[j].SrcUserID = (*Iter)->SrcUserID;
			TCHARCopy(msgNormal->Array[j].SrcNickName, CountArray(msgNormal->Array[j].SrcNickName), (*Iter)->SrcNickName, _tcslen((*Iter)->SrcNickName));

			__int64 diffDay = GetDiffDay((*Iter)->SendTimeLog, g_FishServer.GetFishConfig().GetWriteSec());
			if (diffDay > 250)
				diffDay = 250;
			msgNormal->Array[j].bDiffTime = (BYTE)diffDay;
			++j;
			msgNormal->Sum = j;
		}
		++m_ClientMailSum;//客户端邮件数量
	}
	//命令已经准备完毕后
	{
		std::vector<LC_Cmd_NormalMail*> pVec;
		SqlitMsg(msgNormal, sizeof(LC_Cmd_NormalMail)+sizeof(tagNormalMailTitle)*(msgNormal->Sum - 1), msgNormal->Sum, true, pVec);
		free(msgNormal);
		if (!pVec.empty())
		{
			std::vector<LC_Cmd_NormalMail*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				m_pUser->SendDataToClient(*Iter);
				free(*Iter);
			}
			pVec.clear();
		}
	}
	{
		std::vector<LC_Cmd_SystemMail*> pVec;
		SqlitMsg(msgSystem, sizeof(LC_Cmd_SystemMail)+sizeof(tagSystemMailTitle)*(msgSystem->Sum - 1), msgSystem->Sum, true, pVec);
		free(msgSystem);
		if (!pVec.empty())
		{
			std::vector<LC_Cmd_SystemMail*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				m_pUser->SendDataToClient(*Iter);
				free(*Iter);
			}
			pVec.clear();
		}
	}
	/*LC_Cmd_GetMailFinish msgFinish;
	SetMsgInfo(msgFinish,GetMsgType(Main_Mail, LC_GetUserMailFinish), sizeof(LC_Cmd_GetMailFinish));
	m_pUser->SendDataToClient(&msgFinish);*/
	m_IsLoadToClient = true;
	return true;
}
bool RoleMailManager::OnGetUserMailContext(DWORD MailID)
{
	if ( !m_pUser)
	{
		ASSERT(false);
		return false;
	}
	//将一个邮件的详细信息发送到客户端去 
	std::list<tagRoleMail*>::iterator Iter = m_RoleMailVec.begin();
	for (; Iter != m_RoleMailVec.end(); ++Iter)
	{
		if ((*Iter)->MailID == MailID)
		{
			//发送DBR请求
			if ((*Iter)->bIsRead == false)
			{
				(*Iter)->bIsRead = true;

				m_pUser->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Mail);

				DBR_Cmd_ChangeUserMailIsRead msg;
				SetMsgInfo(msg,DBR_ChangeUserMailIsRead, sizeof(DBR_Cmd_ChangeUserMailIsRead));
				msg.dwMailID = (*Iter)->MailID;
				msg.bIsRead = true;
				g_FishServer.SendNetCmdToSaveDB(&msg);
			}
			//获取到邮件了  我们进行设置 
			if ((*Iter)->SrcUserID == 0)
			{
				//系统邮件 
				LC_Cmd_GetSystemMail msg;
				SetMsgInfo(msg,GetMsgType(Main_Mail, LC_ReadSystemMail), sizeof(LC_Cmd_GetSystemMail));
				//msg.dwUserID = m_pUser->GetUserID();
				msg.MailInfo.MailID = (*Iter)->MailID;
				msg.MailInfo.bIsRead = (*Iter)->bIsRead;
				__int64 diffDay = GetDiffDay((*Iter)->SendTimeLog, g_FishServer.GetFishConfig().GetWriteSec());
				if (diffDay > 250)
					diffDay = 250;
				msg.MailInfo.bDiffTime = (BYTE)diffDay;
				TCHARCopy(msg.MailInfo.Context, CountArray(msg.MailInfo.Context), (*Iter)->Context, _tcslen((*Iter)->Context));
				msg.MailInfo.RewardID = (*Iter)->RewardID;
				msg.MailInfo.RewardSum = (*Iter)->RewardSum;
				msg.MailInfo.bIsExistsReward = (*Iter)->bIsExistsReward;
				m_pUser->SendDataToClient(&msg);
				return true;
			}
			else
			{
				//普通邮件
				LC_Cmd_GetNormalMail msg;
				SetMsgInfo(msg,GetMsgType(Main_Mail,LC_ReadNormalMail), sizeof(LC_Cmd_GetNormalMail));
				msg.MailInfo.MailID = (*Iter)->MailID;
				msg.MailInfo.bIsRead = (*Iter)->bIsRead;
				msg.MailInfo.SrcFaceID = (*Iter)->SrcFaceID;
				msg.MailInfo.SrcUserID = (*Iter)->SrcUserID;
				TCHARCopy(msg.MailInfo.SrcNickName, CountArray(msg.MailInfo.SrcNickName), (*Iter)->SrcNickName, _tcslen((*Iter)->SrcNickName));
				TCHARCopy(msg.MailInfo.Context, CountArray(msg.MailInfo.Context), (*Iter)->Context, _tcslen((*Iter)->Context));
				__int64 diffDay = GetDiffDay((*Iter)->SendTimeLog, g_FishServer.GetFishConfig().GetWriteSec());
				if (diffDay > 250)
					diffDay = 250;
				msg.MailInfo.bDiffTime = (BYTE)diffDay;
				m_pUser->SendDataToClient(&msg);
				return true;
			}
		}
	}
	//找不到邮件 我们也需要回应玩家的操作
	LC_Cmd_GetMailError msg;
	SetMsgInfo(msg,GetMsgType(Main_Mail, LC_ReadMailError), sizeof(LC_Cmd_GetMailError));
	//msg.dwUserID = m_pUser->GetUserID();
	msg.MailID = MailID;
	m_pUser->SendDataToClient(&msg);
	return true;
}
bool RoleMailManager::OnGetUserMailItem(DWORD MailID)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return false;
	}
	//从邮件里获取物品
	std::list<tagRoleMail*>::iterator Iter = m_RoleMailVec.begin();
	for (; Iter != m_RoleMailVec.end(); ++Iter)
	{
		if ((*Iter)->MailID == MailID && (*Iter)->SrcUserID == 0)
		{
			if ((*Iter)->bIsExistsReward == false || (*Iter)->RewardID == 0 || (*Iter)->RewardSum == 0)
			{
				LC_Cmd_GetMailItem msgClient;
				SetMsgInfo(msgClient, GetMsgType(Main_Mail, LC_GetMailItem), sizeof(LC_Cmd_GetMailItem));
				msgClient.dwMailID = MailID;
				msgClient.Result = false;
				m_pUser->SendDataToClient(&msgClient);
				m_pUser->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Mail);
				return true;
			}
			//只有系统邮件才有 物品 和 金钱数据
			//1.向玩家结构里面添加金钱
			m_pUser->OnAddRoleRewardByRewardID((*Iter)->RewardID,TEXT("领取邮件物品记录"),(*Iter)->RewardSum);
			//(*Iter)->RewardID = 0;
			(*Iter)->bIsExistsReward = false;//
			//3.向数据库请求操作
			DBR_Cmd_GetUserMailItem msg;
			SetMsgInfo(msg,DBR_GetUserMailItem, sizeof(DBR_Cmd_GetUserMailItem));
			msg.dwMailID = MailID;
			g_FishServer.SendNetCmdToSaveDB(&msg);
			//4.发送命令到客户端去
			LC_Cmd_GetMailItem msgClient;
			SetMsgInfo(msgClient,GetMsgType(Main_Mail, LC_GetMailItem), sizeof(LC_Cmd_GetMailItem));
			msgClient.dwMailID = MailID;
			msgClient.Result = true;
			m_pUser->SendDataToClient(&msgClient);
			return true;
		}
	}
	LC_Cmd_GetMailItem msgClient;
	SetMsgInfo(msgClient,GetMsgType(Main_Mail, LC_GetMailItem), sizeof(LC_Cmd_GetMailItem));
	msgClient.dwMailID = MailID;
	msgClient.Result = false;
	m_pUser->SendDataToClient(&msgClient);
	m_pUser->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Mail);
	return true;
}
bool RoleMailManager::OnDelUserMail(DWORD MailID)
{
	if (!m_pUser)
	{
		ASSERT(false);
		return false;
	}
	//删除一封邮件
	std::list<tagRoleMail*>::iterator Iter = m_RoleMailVec.begin();
	bool IsNeedSendNewMail = false;
	for (int i = 0; Iter != m_RoleMailVec.end(); ++Iter,++i)
	{
		if ((*Iter)->MailID == MailID)
		{
			//1.发送到数据库
			DBR_Cmd_DelUserMail msg;
			SetMsgInfo(msg, DBR_DelUserMail, sizeof(DBR_Cmd_DelUserMail));
			msg.dwMailID = MailID;
			g_FishServer.SendNetCmdToSaveDB(&msg);
			//2.发送命令到客户端去
			LC_Cmd_DelMail msgClient;
			SetMsgInfo(msgClient, GetMsgType(Main_Mail, LC_DelUserMail), sizeof(LC_Cmd_DelMail));
			msgClient.dwMailID = MailID;
			msgClient.Result = true;
			m_pUser->SendDataToClient(&msgClient);
			//3.移除数据
			delete *Iter;//因为邮件是New 出来的 是需要销毁内存的
			//ListRemoveAt(m_RoleMailVec, i);//移除指定位置的数据
			Iter = m_RoleMailVec.erase(Iter);//为保证顺序 无须进行交换删除
			//4.我们想要补发一封邮件到客户端去 保证客户端保持最新的20封邮件
			IsNeedSendNewMail = true;

			--m_ClientMailSum;//删除客户端的一封邮件

			if (m_RoleMailVec.size() >= g_FishServer.GetFishConfig().GetFishMailConfig().ClientShowMailSum && m_ClientMailSum<g_FishServer.GetFishConfig().GetFishMailConfig().ClientShowMailSum)//邮件不够 不补发
			{
				Iter = m_RoleMailVec.begin();
				for (int i = 0; i < g_FishServer.GetFishConfig().GetFishMailConfig().ClientShowMailSum - 1; ++i)
					++Iter;
				tagRoleMail* pMail = *Iter;
				OnSendAddMailToClient(pMail);//补充一封邮件
			}
			m_pUser->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Mail);
			return true;
		}
	}
	LC_Cmd_DelMail msgClient;
	SetMsgInfo(msgClient,GetMsgType(Main_Mail, LC_DelUserMail), sizeof(LC_Cmd_DelMail));
	msgClient.dwMailID = MailID;
	msgClient.Result = false;
	m_pUser->SendDataToClient(&msgClient);
	return true;
}
bool RoleMailManager::OnBeAddUserMail(tagRoleMail* pMail)
{
	//由中央服务器发送来的 当接收到一个邮件 或者 系统邮件的时候 我们先存储到集合里面去
	if (!pMail)
	{
		ASSERT(false);
		return false;
	}
	if (pMail->SrcUserID !=0 && m_RoleMailVec.size() >= g_FishServer.GetFishConfig().GetFishMailConfig().MaxUserMailSum)
	{
		//邮箱已经满了
		//发送回发命令给原玩家
		CRoleEx* pSrcRole = g_FishServer.GetRoleManager()->QueryUser(pMail->SrcUserID);
		if (pSrcRole)
		{
			LC_Cmd_SendUserMailResult msg;
			SetMsgInfo(msg, GetMsgType(Main_Mail, LC_SendUserMailResult), sizeof(LC_Cmd_SendUserMailResult));
			msg.DestUserID = m_pUser->GetUserID();
			msg.Result = false;
			pSrcRole->SendDataToClient(&msg);
			return false;
		}
		else
		{
			//发送邮件结果到中央服务器去
			GC_Cmd_SendUserMailResult msg;
			SetMsgInfo(msg, GetMsgType(Main_Mail, GC_SendUserMailResult), sizeof(GC_Cmd_SendUserMailResult));
			msg.DestUserID = m_pUser->GetUserID();
			msg.SrcUserID = pMail->SrcUserID;
			msg.Result = false;
			m_pUser->SendDataToCenter(&msg);
			return false;
		}
	}
	else if (pMail->SrcUserID == 0)
	{
		//系统邮件 无须 在保存处理 
		//系统邮件由中央服务器进行保存的 
		tagRoleMail* pNewMail = new tagRoleMail;
		if (!pNewMail)
		{
			ASSERT(false);
			return false;
		}
		memcpy_s(pNewMail, sizeof(tagRoleMail), pMail, sizeof(tagRoleMail));
		m_RoleMailVec.push_front(pNewMail);//放到最前面
		OnSendAddMailToClient(pNewMail);
		
		m_pUser->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Mail);
		return true;
		//DBR_Cmd_AddUserMail msg;
		//SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
		//msg.dwDestUserID = m_pUser->GetUserID();//当前玩家
		//msg.MailInfo = *pMail;
		//g_FishServer.SendNetCmdToDB(&msg);
	}
	else
	{
		DBR_Cmd_AddUserMail msg;
		SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
		msg.dwDestUserID = m_pUser->GetUserID();//当前玩家
		msg.MailInfo = *pMail;
		g_FishServer.SendNetCmdToDB(&msg);
	}
	return true;
}
bool RoleMailManager::OnBeAddUserMailResult(DBO_Cmd_AddUserMail* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	if (pMsg->Result)
	{
		/*if (pMsg->MailInfo.SrcUserID == 0)
			return true;*/
		//发送邮件成功了
		tagRoleMail* pNewMail = new tagRoleMail;
		memcpy_s(pNewMail, sizeof(tagRoleMail), &pMsg->MailInfo, sizeof(tagRoleMail));
		m_RoleMailVec.push_front(pNewMail);//放到最前面
		OnSendAddMailToClient(pNewMail);
		m_pUser->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Mail);
		//将结果回发回去
		CRoleEx* pSrcRole = g_FishServer.GetRoleManager()->QueryUser(pNewMail->SrcUserID);
		if (pSrcRole)
		{	
			LC_Cmd_SendUserMailResult msg;
			SetMsgInfo(msg, GetMsgType(Main_Mail, LC_SendUserMailResult), sizeof(LC_Cmd_SendUserMailResult));
			msg.DestUserID = m_pUser->GetUserID();
			msg.Result = true;
			pSrcRole->SendDataToClient(&msg);
		}
		else
		{
			GC_Cmd_SendUserMailResult msg;
			SetMsgInfo(msg, GetMsgType(Main_Mail, GC_SendUserMailResult), sizeof(GC_Cmd_SendUserMailResult));
			msg.DestUserID = m_pUser->GetUserID();
			msg.SrcUserID = pMsg->MailInfo.SrcUserID;
			msg.Result = true;
			m_pUser->SendDataToCenter(&msg);
		}
	}
	else
	{
		if (pMsg->MailInfo.SrcUserID == 0)
			return false;
		CRoleEx* pSrcRole = g_FishServer.GetRoleManager()->QueryUser(pMsg->MailInfo.SrcUserID);
		if (pSrcRole)
		{
			LC_Cmd_SendUserMailResult msg;
			SetMsgInfo(msg, GetMsgType(Main_Mail, LC_SendUserMailResult), sizeof(LC_Cmd_SendUserMailResult));
			msg.DestUserID = m_pUser->GetUserID();
			msg.Result = false;
			pSrcRole->SendDataToClient(&msg);
		}
		else
		{
			GC_Cmd_SendUserMailResult msg;
			SetMsgInfo(msg, GetMsgType(Main_Mail, GC_SendUserMailResult), sizeof(GC_Cmd_SendUserMailResult));
			msg.DestUserID = m_pUser->GetUserID();
			msg.SrcUserID = pMsg->MailInfo.SrcUserID;
			msg.Result = false;
			m_pUser->SendDataToCenter(&msg);
		}
	}
	return true;
}
void RoleMailManager::OnSendAddMailToClient(tagRoleMail* pMail)
{
	if (!pMail || !m_pUser)
	{
		ASSERT(false);
		return;
	}
	//发送命令到客户端去
	if (pMail->SrcUserID == 0)
	{
		LC_Cmd_AddSystemMail msg;
		SetMsgInfo(msg,GetMsgType(Main_Mail, LC_AddSystemTitle), sizeof(LC_Cmd_AddSystemMail));
		msg.MailInfo.MailID = pMail->MailID;
		msg.MailInfo.bIsRead = pMail->bIsRead;
		__int64 diffDay = GetDiffDay(pMail->SendTimeLog, g_FishServer.GetFishConfig().GetWriteSec());
		if (diffDay > 250)
			diffDay = 250;
		msg.MailInfo.bDiffTime = (BYTE)diffDay;
		if (pMail->RewardID != 0 && pMail->RewardSum != 0 && pMail->bIsExistsReward)
			msg.MailInfo.bIsExistsItem = true;
		else
			msg.MailInfo.bIsExistsItem = false;
		m_pUser->SendDataToClient(&msg);
		++m_ClientMailSum;

		return;
	}
	else
	{
		if (pMail->SrcUserID == m_pUser->GetUserID())//不可以给自己发送邮件
			return;
		LC_Cmd_AddNormalMail msg;
		SetMsgInfo(msg,GetMsgType(Main_Mail, LC_AddNormalTitle), sizeof(LC_Cmd_AddNormalMail));
		msg.MailInfo.MailID = pMail->MailID;
		msg.MailInfo.bIsRead = pMail->bIsRead;
		msg.MailInfo.SrcFaceID = pMail->SrcFaceID;
		msg.MailInfo.SrcUserID = pMail->SrcUserID;
		TCHARCopy(msg.MailInfo.SrcNickName, CountArray(msg.MailInfo.SrcNickName), pMail->SrcNickName, _tcslen(pMail->SrcNickName));
		__int64 diffDay = GetDiffDay(pMail->SendTimeLog, g_FishServer.GetFishConfig().GetWriteSec());
		if (diffDay > 250)
			diffDay = 250;
		msg.MailInfo.bDiffTime = (BYTE)diffDay;
		m_pUser->SendDataToClient(&msg);
		++m_ClientMailSum;
		return;
	}
}
bool RoleMailManager::OnSendUserMail(CL_Cmd_SendUserMail* pMsg)
{
	//发送发送一封邮件 玩家的邮件
	if (!pMsg || !m_pUser)
	{
		ASSERT(false);
		return false;
	}
	if (!g_FishServer.GetFishConfig().CheckStringIsError(pMsg->Context, MIN_MAIL_CONTEXT, MAX_MAIL_CONTEXT,SCT_Normal))//邮件内容包含非法字符
	{
		LC_Cmd_SendUserMailResult msg;
		SetMsgInfo(msg, GetMsgType(Main_Mail, LC_SendUserMailResult), sizeof(LC_Cmd_SendUserMailResult));
		msg.DestUserID =pMsg->DestUserID;
		msg.Result = false;
		m_pUser->SendDataToClient(&msg);
		return false;
	}
	//发送封邮件给其他玩家
	//构建邮件内容
	tagRoleMail pMail;
	TCHARCopy(pMail.Context, CountArray(pMail.Context), pMsg->Context, _tcslen(pMsg->Context));
	TCHARCopy(pMail.SrcNickName, CountArray(pMail.SrcNickName), m_pUser->GetRoleInfo().NickName, _tcslen(m_pUser->GetRoleInfo().NickName));
	pMail.SrcUserID = m_pUser->GetUserID();
	pMail.SrcFaceID = m_pUser->GetFaceID();
	pMail.bIsRead = false;
	pMail.RewardID = 0;
	pMail.MailID = 0;
	pMail.bIsExistsReward = false;//玩家邮件不存在物品
	pMail.SendTimeLog = time(NULL);

	CRoleEx* pDestRole = g_FishServer.GetRoleManager()->QueryUser(pMsg->DestUserID);
	if (pDestRole)//玩家在同一个GameServer上面
	{
		//玩家在线 我们进行处理
		pDestRole->GetMailManager().OnBeAddUserMail(&pMail);//给玩家添加邮件
	}
	else
	{
		//发送到中央服务器去
		GC_Cmd_SendUserMail msg;
		SetMsgInfo(msg, GetMsgType(Main_Mail, GC_SendUserMail), sizeof(GC_Cmd_SendUserMail));
		msg.DestUserID = pMsg->DestUserID;
		msg.MailInfo = pMail;
		g_FishServer.SendNetCmdToCenter(&msg);
	}
	return true;
}
void RoleMailManager::OnDayChange()
{
	//删除七天外的邮件 客户端自行处理
	time_t Now = time(NULL);
	BYTE NowClientSum = m_ClientMailSum;

	DBR_Cmd_DelUserMail msg;
	SetMsgInfo(msg, DBR_DelUserMail, sizeof(DBR_Cmd_DelUserMail));

	LC_Cmd_DelMail msgClient;
	SetMsgInfo(msgClient, GetMsgType(Main_Mail, LC_DelUserMail), sizeof(LC_Cmd_DelMail));
	msgClient.Result = true;

	std::list<tagRoleMail*>::iterator Iter = m_RoleMailVec.begin();
	for (size_t i = 0; Iter != m_RoleMailVec.end(); ++i)
	{
		int DiffDay = GetDiffDay((*Iter)->SendTimeLog,g_FishServer.GetFishConfig().GetWriteSec());//发送邮件到今天的天数差 
		if (DiffDay > g_FishServer.GetFishConfig().GetFishMailConfig().MailLimitDay && (*Iter)->SrcUserID != 0)
		{
			//发送命令到客户端去删除数据
			msg.dwMailID = (*Iter)->MailID;
			g_FishServer.SendNetCmdToSaveDB(&msg);
			//2.发送命令到客户端去
			msgClient.dwMailID = (*Iter)->MailID;
			m_pUser->SendDataToClient(&msgClient);
			delete *Iter;
			Iter = m_RoleMailVec.erase(Iter);//无法变化顺序 必须一个接一个删除 
			if (i < NowClientSum)
			{
				--m_ClientMailSum;
			}
		}
		else
		{
			++Iter;
		}
	}
	if (m_ClientMailSum<0)
		m_ClientMailSum = 0;//客户端邮件数
	if (m_ClientMailSum < g_FishServer.GetFishConfig().GetFishMailConfig().ClientShowMailSum && m_RoleMailVec.size() > m_ClientMailSum)
	{
		//服务器端还有剩余的邮件 我们将补充部分邮件到客户端去
		//前面 m_ClientMailSum 封邮件无须发送 只发送后面的 并且补充到MAIL_CLIENTSUM 就可以了
		int BeginIndex = m_ClientMailSum;
		int SendSum = g_FishServer.GetFishConfig().GetFishMailConfig().ClientShowMailSum - m_ClientMailSum;
		for (int i = 0; Iter != m_RoleMailVec.end(); ++i,++Iter)
		{
			if (i < BeginIndex)
				continue;
			if (i >= SendSum + BeginIndex)
				break;
			//发送邮件
			OnSendAddMailToClient(*Iter);
			++m_ClientMailSum;//客户端邮件数量添加
		}
	}
	m_pUser->GetRoleMessageStates().OnChangeRoleMessageStates(RMT_Mail);
}
bool RoleMailManager::GetMailMessageStates()
{
	//是否有未读邮件 或者是未拿去物品的邮件
	std::list<tagRoleMail*>::iterator Iter = m_RoleMailVec.begin();
	for (; Iter != m_RoleMailVec.end(); ++Iter)
	{
		if (!(*Iter)->bIsRead)
			return true;
	}
	return false;
}