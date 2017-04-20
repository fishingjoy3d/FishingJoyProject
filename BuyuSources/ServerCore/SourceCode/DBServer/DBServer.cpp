#include "stdafx.h"
#include "DBServer.h"
#include "..\CommonFile\FishServerConfig.h"

const static char DBErrorSqlFileName[] = "LogSqlError.txt";

DBServer g_DBServer;
struct Thread3Info
{
	void* pPoint;
	BYTE   Index;
};
struct Thread4nfo
{
	void* pPoint;
	BYTE   Index;
};
//发送到MySQL的字符串不可以用\结尾

inline UINT WINAPI ThreadFunc1(void *p)
{
	((DBServer*)p)->DBThread();
	return 0;
}
inline UINT WINAPI ThreadFunc2(void *p)
{
	((DBServer*)p)->NetThread();
	return 0;
}
inline UINT WINAPI ThreadFunc3(void *p)
{
	//线程3 用于处理队列里面的
	Thread3Info* pInfo = (Thread3Info*)p;
	((DBServer*)pInfo->pPoint)->HandleThreadMsg(pInfo->Index);
	delete pInfo;
	return 0;
}
//inline UINT WINAPI ThreadFunc4(void *p)
//{
//	//线程3 用于处理队列里面的
//	((DBServer*)p)->SendDBMsg();
//	return 0;
//}
bool DBServer::Init(DWORD Index)
{
	m_pDump.OnInit();
	if (!g_FishServerConfig.LoadServerConfigFile())
	{
		ShowInfoToWin("服务器配置文件读取失败");
		return false;
	}
	if (!m_Config.LoadConfigFilePath())
	{
		ShowInfoToWin("读取配置文件失败");
		return false;
	}
	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(ConvertDWORDToBYTE(Index));
	if (!pDBConfig)
	{
		ASSERT(false);
		return false;
	}
	m_DBNetworkID = ConvertDWORDToBYTE(Index);
	SQLInitData pMySQl;
	strncpy_s(pMySQl.IP, CountArray(pMySQl.IP), pDBConfig->MySQlIP, CountArray(pDBConfig->MySQlIP));
	strncpy_s(pMySQl.DB, CountArray(pMySQl.DB), pDBConfig->MySQlDBName, CountArray(pDBConfig->MySQlDBName));
	strncpy_s(pMySQl.UID, CountArray(pMySQl.UID), pDBConfig->MySQlUserName, CountArray(pDBConfig->MySQlUserName));
	strncpy_s(pMySQl.PWD, CountArray(pMySQl.PWD), pDBConfig->MySQlUserPassword, CountArray(pDBConfig->MySQlUserPassword));

	m_bRun = true;

	for (BYTE i = 0; i < DB_ThreadSum; ++i)
	{
		if (!m_Sql[i].Init(pMySQl))
		{
			Log("数据库连接失败:%s", pMySQl.IP);
			ShowInfoToWin("数据库连接失败");
			return false;
		}
	}

	//::_beginthreadex(0, 0, ThreadFunc1, this, 0, 0);
	::_beginthreadex(0, 0, ThreadFunc2, this, 0, 0);
	for (BYTE i = 0; i < DB_ThreadSum; ++i)
	{
		Thread3Info* pInfo = new Thread3Info;
		pInfo->Index = i;
		pInfo->pPoint = this;
		::_beginthreadex(0, 0, ThreadFunc3, pInfo, 0, 0);
	}
	//::_beginthreadex(0, 0, ThreadFunc4, this, 0, 0);

	ServerInitData siid;
	siid.ServerID = pDBConfig->NetworkID;
	siid.BuffSize = pDBConfig->BuffSize;
	siid.CmdHearbeat = DB_HEARBEAT;
	siid.Port = pDBConfig->DBListenPort;
	siid.SocketRecvSize = pDBConfig->RecvBuffSize;
	siid.SocketSendSize = pDBConfig->SendBuffSize;
	siid.Timeout = pDBConfig->TimeOut;
	siid.RecvThreadNum = pDBConfig->RecvThreadSum;
	siid.SendThreadNum = pDBConfig->SendThreadSum;
	siid.SleepTime = pDBConfig->SleepTime;
	siid.AcceptSleepTime = pDBConfig->AcceptSleepTime;
	siid.AcceptRecvData = true;

	siid.SceneHearbeatTick = pDBConfig->SceneHearbeatTick;
	siid.MaxSendCmdCount = pDBConfig->MaxSendCmdCount;
	siid.MaxAcceptNumPerFrame = pDBConfig->MaxAcceptNumPerFrame;
	siid.MaxRecvCmdCount = pDBConfig->MaxRecvCmdCount;
	siid.MaxSendCountPerFrame = pDBConfig->MaxSendCountPerFrame;
	siid.ListenCount = pDBConfig->ListenCount;


	m_Server.SetCmdHandler(this);
	m_Server.Init(siid);
	
	return true;
}
void DBServer::DBThread()
{
}
ServerClientData* DBServer::GetClientData(BYTE ClientID)
{
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClintList.find(ClientID);
	if (Iter == m_ClintList.end())
		return NULL;
	else
		return Iter->second;
}
void DBServer::NetThread()
{
	while (m_bRun)
	{
		OnAddClient();
		//接收命令
		__int64 MsgTotalSum = 0;
		HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClintList.begin();
		for (; Iter != m_ClintList.end();)
		{
			ServerClientData *pClient = Iter->second;
			if (!pClient)
			{
				++Iter;
				continue;
			}
			int Sum = 0;
			while (pClient->RecvList.HasItem() && (Sum<Msg_OnceSum || pClient->Removed))//如果客户端是需要移除的 就不限制数量了
			{
				/*if (pClient->RecvList.Count() >2000)
				{
					ShowInfoToWin("数据库待处理的命令为 %u", pClient->RecvList.Count());
				}*/
				
				NetCmd *pcmd = pClient->RecvList.GetItem();
				if (!pcmd)
				{
					ASSERT(false);
					continue;
				}
				//将命令放入到多线程里面去
				RcveMsg* pRecvInfo = new RcveMsg;
				pRecvInfo->pCmd = pcmd;
				pRecvInfo->ClientID = ConvertDWORDToBYTE(pClient->OutsideExtraData);
				m_RecvMsgList[MsgTotalSum% DB_ThreadSum].AddItem(pRecvInfo);
				++MsgTotalSum;
				++Sum;
			}
			if (pClient->Removed)//在离开前 先处理完当前客户的全部的数据
			{
				ShowInfoToWin("一个Client离开了DBServer");
				Iter = m_ClintList.erase(Iter);
				m_Server.Kick(pClient);
				continue;
			}
			++Iter;
		}
		//将网络命令发送出去
		for (BYTE i = 0; i < DB_ThreadSum; ++i)
		{
			while (m_SendMsgList[i].HasItem())
			{
				RcveMsg *pRecvInfo = m_SendMsgList[i].GetItem();
				if (!pRecvInfo)
					continue;
				ServerClientData* pClient = GetClientData(pRecvInfo->ClientID);
				if (!pClient)
				{
					delete pRecvInfo;
					continue;
				}
				m_Server.Send(pClient, pRecvInfo->pCmd);
				free(pRecvInfo->pCmd);//New出来的命令 需要free掉
				delete pRecvInfo;
			}
		}
		Sleep(1);
	}
	::InterlockedIncrement(&m_ExitCount);
}
//void DBServer::SendDBMsg()
//{
//	//将集合里的全部数据库命令发送出去
//	bool IsHandle = false;
//	while (m_bRun)
//	{
//		OnAddClient();
//
//		IsHandle = false;
//		for (BYTE i = 0; i < DB_ThreadSum; ++i)
//		{
//			RcveMsg *pRecvInfo = m_SendMsgList[i].GetItem();
//			if (pRecvInfo)//同一线程上只处理10个
//			{
//				if (pRecvInfo->pClient)
//				{
//					m_Server.Send(pRecvInfo->pClient, pRecvInfo->pCmd);
//				}
//				free(pRecvInfo->pCmd);//New出来的命令 需要free掉
//				delete pRecvInfo;
//				IsHandle = true;
//				//pRecvInfo = m_SendMsgList[i].GetItem();
//			}
//		}
//		if (!IsHandle)
//			Sleep(1);
//	}
//	::InterlockedIncrement(&m_ExitCount);
//}
void DBServer::HandleThreadMsg(BYTE ThreadIndex)//多个线程进程处理
{
	if (ThreadIndex >= DB_ThreadSum)
		return;
	while (m_bRun)
	{
		RcveMsg *pRecvInfo = m_RecvMsgList[ThreadIndex].GetItem();
		while (pRecvInfo)
		{
			HandleDataBaseMsg(ThreadIndex, pRecvInfo->ClientID, pRecvInfo->pCmd);
			free(pRecvInfo->pCmd);
			delete pRecvInfo;
			pRecvInfo = m_RecvMsgList[ThreadIndex].GetItem();
		}
		Sleep(1);
	}
	::InterlockedIncrement(&m_ExitCount);
}
void DBServer::Shutdown()
{
	m_ExitCount = 0;
	m_bRun = false;
	while (m_ExitCount != DB_ThreadSum + 1)
		Sleep(1);
	OnHandleAllMsg();//清理完全部的命令
}
bool DBServer::CommitTable(SqlTable &table)
{
	vector<SqlUpdateStr*> strList;
	table.CreateSqlStr(strList, true);
	for (UINT i = 0; i < strList.size(); ++i)
		m_SqlList.AddItem(strList[i]);

	return true;
}
DBServer::DBServer()
{
	m_ControlIsConnect = false;
	//m_ClientIndex = 1;
}
DBServer::~DBServer()
{
	while (m_AfxAddClient.HasItem())
	{
		AfxNetworkClientOnce* pOnce = m_AfxAddClient.GetItem();
		delete pOnce;
	}
}
void DBServer::ShowInfoToWin(const char *pcStr, ...)
{
	if (!pcStr)
	{
		ASSERT(false);
		return;
	}
	va_list var;
	va_start(var, pcStr);
	UINT nCount = _vscprintf(pcStr, var);
	char *pBuffer = new char[nCount + 1];
	vsprintf_s(pBuffer, nCount + 1, pcStr, var);
	std::cout << pBuffer;
	std::cout << "\n";
	SAFE_DELETE_ARR(pBuffer);
	va_end(var);
}
void DBServer::Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//输出 客户端离开的状态
	switch (rt)
	{
	case REMOVE_NONE:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为None",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_NORMAL:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为普通",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_RECV_ERROR:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为接收错误",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_SEND_ERROR:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为发送错误",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_TIMEOUT:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为超时",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_CMD_SEND_OVERFLOW:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为发送太多",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_CMD_RECV_OVERFLOW:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为接收太多",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_CMD_SIZE_ERROR:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为命令大小",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_RECVBACK_NOT_SPACE:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为接收缓冲区溢出",pClient->OutsideExtraData);
		}
		break;
	}
}
void DBServer::OnAddClient()
{
	while (m_AfxAddClient.HasItem())
	{
		AfxNetworkClientOnce* pOnce = m_AfxAddClient.GetItem();
		if (!pOnce)
		{
			//ASSERT(false);
			continue;
		}
		if (!pOnce->pPoint || pOnce->Length != sizeof(BYTE))
		{
			m_Server.Kick(pOnce->pClient);
			//ASSERT(false);
			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
			continue;
		}
		BYTE ClientID = *((BYTE*)pOnce->pPoint);
		if (ClientID == 0)
		{
			m_Server.Kick(pOnce->pClient);
			//ASSERT(false);
			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
			continue;
		}
		pOnce->pClient->OutsideExtraData = ClientID;
		
		HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClintList.find(ClientID);
		if (Iter != m_ClintList.end())
		{
			ASSERT(false);
			m_Server.Kick(pOnce->pClient);
			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
			continue;
		}
		m_ClintList.insert(HashMap<BYTE, ServerClientData*>::value_type(ClientID, pOnce->pClient));
		//ShowInfoToWin("一个Client加入了DBServer");

		if (pOnce->pPoint)
			free(pOnce->pPoint);
		delete pOnce;
	}
}
bool DBServer::HandleDataBaseMsg(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//处理发送来的数据库的命令 并且产生DBO命令回发回去
	if (!pCmd)//pClient 可以为空 当无须回发命令的时候 或者是DB退出的时候
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->GetCmdType())
	{
	//登陆   OK
	case DBR_AccountLogon:
		return OnHandleAccountLogon(Index, ClientID, pCmd);
	case DBR_QueryLogon:
		return OnHandleQueryLogon(Index, ClientID, pCmd);
	case DBR_AccountRsg:
		return OnHandleAccountRsg(Index, ClientID, pCmd);
	case DBR_ResetAccount:
		return OnHandleAccountReset(Index, ClientID, pCmd);
	case DBR_ChangeAccountPassword:
		return OnHandleChangeAccountPassword(Index, ClientID, pCmd);
	case DBR_GetAccountInfoByUserID:
		return OnHandleGetRoleInfoByUserID(Index, ClientID, pCmd);
	case DBR_SetOnline:
		return OnHandleChangeAccountOnline(Index, ClientID, pCmd);
	case DBR_SetRoleClientInfo:
		return OnHandleRoleClientInfo(Index, ClientID, pCmd);//无须进行无序话的处理
	case DBR_GetNewAccount:
		return OnHandleGetNewAccount(Index, ClientID, pCmd);
	case DBR_SaveChannelInfo:
		return OnHandleSaveChannelInfo(Index, ClientID, pCmd);
	//Item
	case DBR_LoadUserItem:
		return OnHandleGetUserItem(Index, ClientID, pCmd);//分包
	case DBR_AddUserItem:
		return OnHandleAddUserItem(Index, ClientID, pCmd);
	case DBR_DelUserItem:
		return OnHandleDelUserItem(Index, ClientID, pCmd);//无须进行无序话的处理
	case DBR_ChangeUserItem:
		return OnHandleChangeUserItem(Index, ClientID, pCmd);//须进行无序话的处理  添加修改前的数量 用于判断
	//Relation
	case DBR_LoadUserRelation:
		return OnHandleGetUserRelation(Index, ClientID, pCmd);//被动分包
	case DBR_AddUserRelation:
		return OnHandleAddUserRelation(Index, ClientID, pCmd);
	case DBR_DelUserRelation:
		return OnHandleDelUserRelation(Index, ClientID, pCmd);
	//Mail
	case DBR_LoadUserMail:
		return OnHandleGetUserMail(Index, ClientID, pCmd);//分包发送
	case DBR_AddUserMail:
		return OnHandleAddUserMail(Index, ClientID, pCmd);
	case DBR_DelUserMail:
		return OnHandleDelUserMail(Index, ClientID, pCmd);
	case DBR_GetUserMailItem:
		return OnHandleGetUserMailItem(Index, ClientID, pCmd);
	case DBR_ChangeUserMailIsRead:
		return OnHandleChangeUserMailIsRead(Index, ClientID, pCmd);
	//Role
	case DBR_SaveRoleExInfo:
		return OnHandleSaveRoleInfo(Index, ClientID, pCmd);
	case DBR_SaveRoleNickName:
		return OnHandleSaveRoleNickName(Index, ClientID, pCmd);
	case DBR_SaveRoleLevel:
		return OnHandleSaveRoleLevel(Index, ClientID, pCmd);
	case DBR_SaveRoleGender:
		return OnHandleSaveRoleGender(Index, ClientID, pCmd);
	case DBR_SaveRoleAchievementPoint:
		return OnHandleSaveRoleAchievementPoint(Index, ClientID, pCmd);
	case DBR_SaveRoleCurTitle:
		return OnHandleSaveRoleCurTitle(Index, ClientID, pCmd);
	case DBR_SaveRoleCharmArray:
		return OnHandleSaveRoleCharmArray(Index, ClientID, pCmd);
	case DBR_SaveRoleCurrency:
		return OnHandleSaveRoleCurency(Index, ClientID, pCmd);
	case DBR_SaveRoleFaceID:
		return OnHandleSaveRoleFaceID(Index, ClientID, pCmd);
	case DBR_SaveRoleMedal:
		return OnHandleSaveRoleMedal(Index, ClientID, pCmd);
	case DBR_SaveRoleSendGiffSum:
		return OnHandleSaveRoleSendGiffSum(Index, ClientID, pCmd);
	case DBR_SaveRoleAcceptGiffSum:
		return OnHandleSaveRoleAcceptGiffSum(Index, ClientID, pCmd);
	case DBR_SaveRoleTaskStates:
		return OnHandleSaveRoleTaskStates(Index, ClientID, pCmd);
	case DBR_SaveRoleAchievementStates:
		return OnHandleSaveRoleAchievementStates(Index, ClientID, pCmd);
	case DBR_SaveRoleActionStates:
		return OnHandleSaveRoleActionStates(Index, ClientID, pCmd);
	case DBR_SaveRoleOnlineStates:
		return OnHandleSaveRoleOnlineStates(Index, ClientID, pCmd);
	case DBR_SaveRoleCheckData:
		return OnHandleSaveRoleCheckData(Index, ClientID, pCmd);
	case DBR_SaveRoleIsShowIpAddress:
		return OnHandleSaveRoleIsShopIpAddress(Index, ClientID, pCmd);
	case DBR_SaveRoleAllInfo:
		return OnHandleSaveRoleAllInfo(Index, ClientID, pCmd);//保存玩家的全部数据
	case DBR_SaveRoleGlobel:
		return OnHandleSaveRoleGlobel(Index, ClientID, pCmd);
	case DBR_SaveRoleOnlineMin:
		return OnHandleSaveRoleOnLineMin(Index, ClientID, pCmd);
	case DBR_SaveRoleExChangeStates:
		return OnHandleSaveRoleExChangeStates(Index, ClientID, pCmd);
	case DBR_SaveRoleTotalRecharge:
		return OnHandleSaveRoleTotalRecharge(Index, ClientID, pCmd);
	case DBR_SaveRoleIsFirstPayGlobel:
		return OnHandleSaveRoleIsFirstPayGlobel(Index, ClientID, pCmd);
	case DBR_SaveRoleIsFirstPayCurrcey:
		return OnHandleSaveRoleIsFirstPayCurrcey(Index, ClientID, pCmd);
	case DBR_ChangeRoleMoney:
		return OnHandleChangeRoleMoney(Index, ClientID, pCmd);
	case DBR_TableChange:
		return OnHandleTableChange(Index, ClientID, pCmd);
	//Query
	case DBR_Query_RoleInfoByNickName:
		return OnHandleQueryRoleInfoByNickName(Index, ClientID, pCmd);//限制查询数量
	case DBR_Query_RoleInfoByUserID:
		return OnHandleQueryRoleInfoByUserID(Index, ClientID, pCmd);
	//任务
	case DBR_LoadRoleTask:
		return OnHandleLoadRoleTask(Index, ClientID, pCmd);
	case DBR_SaveRoleTask:
		return OnHandleSaveRoleTask(Index, ClientID, pCmd);
	case DBR_DelRoleTask:
		return OnHandleDelRoleTask(Index, ClientID, pCmd);
	case DBR_ClearRoleTask:
		return OnHandleClearRoleTask(Index, ClientID, pCmd);
	case DBR_SaveAllTask:
		return OnHandleSaveAllTask(Index, ClientID, pCmd);
	//成就
	case DBR_LoadRoleAchievement:
		return OnHandleLoadRoleAchievement(Index, ClientID, pCmd);
	case DBR_SaveRoleAchievement:
		return OnHandleSaveRoleAchievement(Index, ClientID, pCmd);
	case DBR_DelRoleAchievement:
		return OnHandleDelRoleAchievement(Index, ClientID, pCmd);
	case DBR_SaveAllAchievement:
		return OnHandleSaveAllAchievement(Index, ClientID, pCmd);
	//活动
	case DBR_LoadRoleAction:
		return OnHandleLoadRoleAction(Index, ClientID, pCmd);
	case DBR_SaveRoleAction:
		return OnHandleSaveRoleAction(Index, ClientID, pCmd);
	case DBR_DelRoleAction:
		return OnHandleDelRoleAction(Index, ClientID, pCmd);
	case DBR_SaveAllAction:
		return OnHandleSaveAllAction(Index, ClientID, pCmd);
	//称号
	case DBR_LoadRoleTitle:
		return OnHandleLoadRoleTitle(Index, ClientID, pCmd);
	case DBR_AddRoleTitle:
		return OnHandleAddRoleTitle(Index, ClientID, pCmd);
	case DBR_DelRoleTitle:
		return OnHandleDelRoleTitle(Index, ClientID, pCmd);
	//商店
	case DBR_AddRoleEntityItem:
		return OnHandleSaveRoleEntityItem(Index, ClientID, pCmd);
	//实体
	case DBR_LoadRoleEntity:
		return OnHandleLoadRoleEntity(Index, ClientID, pCmd);
	case DBR_SaveRoleEntityName:
		return OnHandleSaveRoleEntityName(Index, ClientID, pCmd);
	case DBR_SaveRoleEntityPhone:
		return OnHandleSaveRoleEntityPhone(Index, ClientID, pCmd);
	case DBR_SaveRoleEntityEmail:
		return OnHandleSaveRoleEntityEmail(Index, ClientID, pCmd);
	case DBR_SaveRoleEntityIdentityID:
		return OnHandleSaveRoleEntityIdentityID(Index, ClientID, pCmd);
	case DBR_SaveRoleEntityItemUseName:
		return OnHandleSaveRoleEntityItemUseName(Index, ClientID, pCmd);
	case DBR_SaveRoleEntityItemUsePhone:
		return OnHandleSaveRoleEntityItemUsePhone(Index, ClientID, pCmd);
	case DBR_SaveRoleEntityItemUseAddress:
		return OnHandleSaveRoleEntityItemUseAddress(Index, ClientID, pCmd);
	//赠送
	case DBR_LoadRoleGiff:
		return OnHandleLoadRoleGiff(Index, ClientID, pCmd);
	case DBR_AddRoleGiff:
		return OnHandleAddRoleGiff(Index, ClientID, pCmd);
	case DBR_DelRoleGiff:
		return OnHandleDelRoleGiff(Index, ClientID, pCmd);
	case DBR_GetNowDayGiff:
		return OnHandleLoadNowDayGiff(Index, ClientID, pCmd);
	case DBR_ClearNowDayGiff:
		return OnHandleClearNowDayGiff(Index, ClientID, pCmd);
	//魅力
	case DBO_SaveRoleCharm:
		return OnHandleChangeRoleCharm(Index, ClientID, pCmd);
	//排行榜
	case DBR_LoadRankInfo:
		return OnHandleLoadRankInfo(Index, ClientID, pCmd);
	case DBR_LoadWeekRankInfo:
		return OnHandleLoadWeekRankInfo(Index, ClientID, pCmd);
	case DBR_SaveWeekRankInfo:
		return OnHandleSaveWeekRankInfo(Index, ClientID, pCmd);
	case DBR_ChangeWeekInfo:
		return OnHandleChangeWeekInfo(Index, ClientID, pCmd);
	//玩家GameData
	case DBR_LoadGameData:
		return OnHandleLoadRoleGameData(Index, ClientID, pCmd);
	case DBR_SaveGameData:
		return OnHandleSaveRoleGameData(Index, ClientID, pCmd);
	case DBR_ChangeGameDataByMonth:
		return OnHandleChangeRoleGameDataByMonth(Index, ClientID, pCmd);
	//
	case DBR_LoadAllUserAchievementPointList:
		return OnHandleAchievementPointList(Index, ClientID, pCmd);
	//
	case DBR_LoadAllAccountInfo:
		return OnHandleLoadAllAccountInfo(Index, ClientID, pCmd);
	//Log
	case DBR_LogRecharge:
		return OnHandleLogRechargeInfo(Index, ClientID, pCmd);
	case DBR_LogInfo:
		return OnHandleLogInfo(Index, ClientID, pCmd);
	case DBR_LogOnlineInfo:
		return OnHandleLogRoleOnlineInfo(Index, ClientID, pCmd);
	case DBR_LogPhonePay:
		return OnHandleLogPhonePayInfo(Index, ClientID, pCmd);
	//Announcement
	case DBR_LoadAllAnnouncement:
		return OnHandleLoadAllAnnouncementInfo(Index, ClientID, pCmd);
	case DBR_AddAnnouncement:
		return OnHandleAddAnnouncementInfo(Index, ClientID, pCmd);
	//邮箱
	case DBR_SaveRoleEmail:
		return OnHandleChangeRoleEmail(Index, ClientID, pCmd);
	//Center
	case DBR_ClearFishDB:
		return OnHandleClearFishDB(Index, ClientID, pCmd);
	//ExChange
	case DBR_QueryExChange:
		return OnHandleQueryExChange(Index, ClientID, pCmd);
	case DBR_DelExChange:
		return OnDelExChange(Index, ClientID, pCmd);
	//Recharege
	case DBR_AddRecharge:
		return OnAddRecharge(Index, ClientID, pCmd);
	case DBR_LoadRechageOrderID:
		return OnLoadRechargeOrderID(Index, ClientID, pCmd);
	case DBR_AddRechageOrderID:
		return OnAddRechargeOrderID(Index, ClientID, pCmd);
	case DBR_CheckPhone:
		return OnCheckPhone(Index, ClientID, pCmd);
	case DBR_CheckEntityID:
		return OnCheckEntityID(Index, ClientID, pCmd);
	case DBR_LoadPhonePayOrderID:
		return OnLoadPhonePayOrderList(Index, ClientID, pCmd);
	case DBR_AddPhonePayOrderID:
		return OnAddPhonePayOrderID(Index, ClientID, pCmd);
	default:
		return false;
	}
	return true;
}
bool DBServer::OnHandleAccountLogon(BYTE Index,BYTE ClientID, NetCmd* pCmd)
{
	//处理命令
	DBR_Cmd_AccountLogon* pMsg = (DBR_Cmd_AccountLogon*)pCmd;
	//根据参数的类型执行存储过程 并且 接收存储过程返回的数据 
	//如果存储过程失败  就返回异常 并且设置失败
	//如果有数据 就设置DBO命令 并且返回给发送这个命令的服务器
	DBO_Cmd_AccountLogon* msg = (DBO_Cmd_AccountLogon*)CreateCmd(DBO_AccountLogon, sizeof(DBO_Cmd_AccountLogon));
	msg->ClientID = pMsg->ClientID;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;
	char* AccountName = WCharToChar(pMsg->AccountName, Size);

	char* DestAccountName = (char*)malloc((Size * 2  + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(AccountName, Size, DestAccountName);


	Size = 0;
	char* MacAddress = WCharToChar(pMsg->MacAddress, Size);

	char* DestMacAddress = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(MacAddress, Size, DestMacAddress);

	tm NowTime;
	time_t Now = time(null);
	errno_t Err = localtime_s(&NowTime, &Now);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAccountLogon('%s','%s','%u','%u','%u','%d-%d-%d %d:%d:%d');", DestAccountName, DestMacAddress, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3, NowTime.tm_year + 1900, NowTime.tm_mon + 1, NowTime.tm_mday, NowTime.tm_hour, NowTime.tm_min, NowTime.tm_sec);
	free(DestAccountName);
	free(AccountName);
	free(DestMacAddress);
	free(MacAddress);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		msg->dwUserID = pTable1.GetUint(0,0);
		msg->IsFreeze = pTable1.GetBit(0,1);
		msg->FreezeEndTime = pTable1.GetDateTime(0,2);

		time_t tNow = msg->FreezeEndTime;
		tm pNow;
		localtime_s(&pNow, &tNow);

		bool xx = true;
	}
	else
	{
		msg->dwUserID = 0;
		ASSERT(false);
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
	}
	//发送命令
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleQueryLogon(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_QueryLogon* pMsg = (DBR_Cmd_QueryLogon*)pCmd;
	DBO_Cmd_QueryLogon* msg = (DBO_Cmd_QueryLogon*)CreateCmd(DBO_QueryLogon, sizeof(DBO_Cmd_QueryLogon));
	msg->ClientID = pMsg->ClientID;

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;
	char* AccountName = WCharToChar(pMsg->AccountName, Size);

	char* DestAccountName = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(AccountName, Size, DestAccountName);

	Size = 0;
	char* MacAddress = WCharToChar(pMsg->MacAddress, Size);
	char* DestMacAddress = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(MacAddress, Size, DestMacAddress);

	//用户名称
	//参数一个随机长度的 名称 8个长度  游客+ '长度为6的 数据'
	string strNickName = "用户";
	for (size_t i = 0; i < MAX_NICKNAME - 2; ++i)
	{
		BYTE ID = RandUInt() % 62;//0-61
		char ch = 0;
		if (ID >= 0 && ID < 10)
		{
			ch = ID + 48;
			//48-57
		}
		else if (ID >= 10 && ID < 36)
		{
			ch = ID + 55;
			//65-90
		}
		else
		{
			ch = ID + 61;
			//97-122
		}
		strNickName = strNickName + ch;
	}

	tm NowTime;
	time_t Now = time(null);
	errno_t Err = localtime_s(&NowTime, &Now);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishQueryLogon('%s','%s','%u','%u','%u','%s',%d,'%u','%d.%d.%d.%d','%d-%d-%d %d:%d:%d');", DestAccountName, DestMacAddress, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3, strNickName.c_str(), 1, m_Config.GetSystemConfig().RsgInitGlobelSum, pMsg->ClientIP & 0xff, (pMsg->ClientIP >> 8) & 0xff, (pMsg->ClientIP >> 16) & 0xff, (pMsg->ClientIP >> 24) & 0xff, NowTime.tm_year + 1900, NowTime.tm_mon + 1, NowTime.tm_mday, NowTime.tm_hour, NowTime.tm_min, NowTime.tm_sec);
	free(DestAccountName);
	free(AccountName);
	free(DestMacAddress);
	free(MacAddress);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		msg->dwUserID = pTable1.GetUint(0, 0);
		msg->IsFreeze = pTable1.GetBit(0, 1);
		msg->FreezeEndTime = pTable1.GetDateTime(0, 2);
	}
	else
	{
		msg->dwUserID = 0;
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	//发送命令
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleAccountRsg(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//处理玩家账号注册
	DBR_Cmd_AccountRsg* pMsg = (DBR_Cmd_AccountRsg*)pCmd;
	DBO_Cmd_AccountRsg* msg = (DBO_Cmd_AccountRsg*)CreateCmd(DBO_AccountRsg, sizeof(DBO_Cmd_AccountRsg));
	msg->ClientID = pMsg->ClientID;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;
	char* AccountName = WCharToChar(pMsg->AccountName, Size);
	char* DestAccountName = (char*)malloc((Size * 2  + 1) * sizeof(char));
	if (!DestAccountName)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestAccountName, (Size * 2  + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(AccountName, Size, DestAccountName);

	Size = 0;
	char* MacAddress = WCharToChar(pMsg->MacAddress, Size);
	char* DestMacAddress = (char*)malloc((Size * 2 + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(MacAddress, Size, DestMacAddress);

	//参数一个随机长度的 名称 8个长度  游客+ '长度为6的 数据'
	string strNickName = "用户";
	for (size_t i = 0; i < MAX_NICKNAME - 2; ++i)
	{
		BYTE ID = RandUInt() % 62;//0-61
		char ch = 0;
		if (ID >= 0 && ID < 10)
		{
			ch = ID + 48;
			//48-57
		}
		else if (ID >= 10 && ID < 36)
		{
			ch = ID + 55;
			//65-90
		}
		else
		{
			ch = ID + 61;
			//97-122
		}
		strNickName = strNickName + ch;
	}

	tm NowTime;
	time_t Now = time(null);
	errno_t Err = localtime_s(&NowTime, &Now);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAccountRsg('%s','%s','%u','%u','%u','%s',%d,'%u','%d.%d.%d.%d','%d-%d-%d %d:%d:%d');", DestAccountName, DestMacAddress, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3, strNickName.c_str(), 1, m_Config.GetSystemConfig().RsgInitGlobelSum, pMsg->ClientIP & 0xff, (pMsg->ClientIP >> 8) & 0xff, (pMsg->ClientIP >> 16) & 0xff, (pMsg->ClientIP >> 24) & 0xff, NowTime.tm_year + 1900, NowTime.tm_mon + 1, NowTime.tm_mday, NowTime.tm_hour, NowTime.tm_min, NowTime.tm_sec);
	free(DestAccountName);
	free(AccountName);
	free(DestMacAddress);
	free(MacAddress);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		msg->dwUserID = pTable1.GetUint(0, 0);
	}
	else
	{
		msg->dwUserID = 0;
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleAccountReset(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//对于快速登陆的账号 我们可以提供一次玩家进行重新设置用户名和密码的机会 快速账号转化为普通账号
	DBR_Cmd_ResetAccount* pMsg = (DBR_Cmd_ResetAccount*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;
	char* AccountName = WCharToChar(pMsg->AccountName, Size);
	char* DestAccountName = (char*)malloc((Size * 2  + 1) * sizeof(char));
	if (!DestAccountName)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestAccountName, (Size * 2  + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(AccountName, Size, DestAccountName);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishChangeAccountName('%s','%u','%u','%u','%u');", DestAccountName, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3, pMsg->dwUserID);
	free(DestAccountName);
	free(AccountName);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 && pTable1.GetUint(0, 0) == 1);//返回值 为 1
	DBO_Cmd_ResetAccount* msg = (DBO_Cmd_ResetAccount*)CreateCmd(DBO_ResetAccount, sizeof(DBO_Cmd_ResetAccount));
	msg->dwUserID = pMsg->dwUserID;
	msg->Result = Result;
	msg->PasswordCrc1 = pMsg->PasswordCrc1;
	msg->PasswordCrc2 = pMsg->PasswordCrc2;
	msg->PasswordCrc3 = pMsg->PasswordCrc3;
	TCHARCopy(msg->AccountName, CountArray(msg->AccountName), pMsg->AccountName, _tcslen(pMsg->AccountName));
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleChangeAccountPassword(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//修改当前账号的密码
	DBR_Cmd_ChangeAccountPassword* pMsg = (DBR_Cmd_ChangeAccountPassword*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishChangeAccountPassword('%u','%u','%u','%u','%u','%u','%u');", pMsg->dwUserID, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3, pMsg->OldPasswordCrc1, pMsg->OldPasswordCrc2,pMsg->OldPasswordCrc3);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 && pTable1.GetUint(0, 0) == 1);
	DBO_Cmd_ChangeAccountPassword* msg = (DBO_Cmd_ChangeAccountPassword*)CreateCmd(DBO_ChangeAccountPassword, sizeof(DBO_Cmd_ChangeAccountPassword));
	msg->dwUserID = pMsg->dwUserID;
	msg->Result = Result;
	if (msg->Result)
	{
		msg->PasswordCrc1 = pMsg->PasswordCrc1;
		msg->PasswordCrc2 = pMsg->PasswordCrc2;
		msg->PasswordCrc3 = pMsg->PasswordCrc3;
	}
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleChangeAccountOnline(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//设置账号上线或者下线状态
	DBR_Cmd_RoleOnline* pMsg = (DBR_Cmd_RoleOnline*)pCmd;
	//DBO_Cmd_RoleOnline* msg = (DBO_Cmd_RoleOnline*)CreateCmd(DBO_RoleOnline, sizeof(DBO_Cmd_RoleOnline));
	//SqlTable pTable1;
	//char SqlStr[MAXSQL_LENGTH] = { 0 };
	//sprintf_s(SqlStr, sizeof(SqlStr), "call FishSetUserOnline('%u',%d);",pMsg->dwUserID,pMsg->IsOnline?1:0);
	//msg->dwUserID = pMsg->dwUserID;
	//msg->Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	//msg->IsOnline = pMsg->IsOnline;
	//if (!msg->Result)
	//	ASSERT(false);
	//OnAddDBResult(Index, ClientID, msg);
	//return true;

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSetUserOnline('%u',%d);", pMsg->dwUserID, pMsg->IsOnline ? 1 : 0);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		ASSERT(false);
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
	}
	return true;
}
//bool DBServer::OnHandleRoleAchievementIndex(BYTE Index, BYTE ClientID, NetCmd* pCmd)
//{
//	DBR_Cmd_GetRoleAchievementIndex* pMsg = (DBR_Cmd_GetRoleAchievementIndex*)pCmd;
//	SqlTable pTable1;
//	char SqlStr[MAXSQL_LENGTH] = { 0 };
//	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadRoleAcievementIndex('%u');", pMsg->dwUserID);
//	//获取存储过程的数据
//	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
//	{
//		DBO_Cmd_GetRoleAchievementIndex* msg = (DBO_Cmd_GetRoleAchievementIndex*)CreateCmd(DBO_GetRoleAchievementIndex, sizeof(DBO_Cmd_GetRoleAchievementIndex));
//		msg->dwUserID = pMsg->dwUserID;
//		msg->AchievementIndex = pTable1.GetUint(0, 0);
//		OnAddDBResult(Index, ClientID, msg);
//		return true;
//	}
//	else
//	{
//		ASSERT(false);
//		return false;
//	}
//}
bool DBServer::OnHandleRoleClientInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//保存玩家数据统计  
	DBR_Cmd_SetRoleClientInfo* pMsg = (DBR_Cmd_SetRoleClientInfo*)pCmd;
	//修改一组物品的数量
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveUserClientInfo('%u','%u','%u','%u');", pMsg->dwUserID, pMsg->PlateFormID, pMsg->ScreenPoint >> 16, (UINT16)pMsg->ScreenPoint);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		ASSERT(false);
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
	}
	return true;
}
bool DBServer::OnHandleSaveChannelInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveChannelInfo* pMsg = (DBR_Cmd_SaveChannelInfo*)pCmd;
	//保存数据
	vector<StringArrayData*> pVec;
	GetStringArrayVecByData(pVec, &pMsg->pInfo);
	if (pVec.size() != pMsg->pInfo.HandleSum)
	{
		ASSERT(false);
		return false;
	}

	UINT Size = 0;
	char* Info1 = WCharToChar(pVec[0]->Array, pVec[0]->Length/sizeof(TCHAR), Size);
	char* DestInfo1 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info1, Size, DestInfo1);

	char* Info2 = WCharToChar(pVec[1]->Array, pVec[1]->Length / sizeof(TCHAR), Size);
	char* DestInfo2 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info2, Size, DestInfo2);

	char* Info3 = WCharToChar(pVec[2]->Array, pVec[2]->Length / sizeof(TCHAR), Size);
	char* DestInfo3 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info3, Size, DestInfo3);

	char* Info4 = WCharToChar(pVec[3]->Array, pVec[3]->Length / sizeof(TCHAR), Size);
	char* DestInfo4 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info4, Size, DestInfo4);

	char* Info5 = WCharToChar(pVec[4]->Array, pVec[4]->Length / sizeof(TCHAR), Size);
	char* DestInfo5 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info5, Size, DestInfo5);

	char* Info6 = WCharToChar(pVec[5]->Array, pVec[5]->Length / sizeof(TCHAR), Size);
	char* DestInfo6 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info6, Size, DestInfo6);

	/*char* Info7 = WCharToChar(pVec[6], Size);
	char* DestInfo7 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info7, Size, DestInfo7);*/

	/*vector<TCHAR*>::iterator Iter = pVec.begin();
	for (; Iter != pVec.end(); ++Iter)
	{
		free(*Iter);
	}
	pVec.clear();*/

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveChannelUserInfo('%u','%s','%s','%s','%s','%s','%s');",
		pMsg->dwUserID, DestInfo1, DestInfo2, DestInfo3, DestInfo4, DestInfo5, DestInfo6);

	free(Info1);
	free(DestInfo1);

	free(Info2);
	free(DestInfo2);

	free(Info3);
	free(DestInfo3);

	free(Info4);
	free(DestInfo4);

	free(Info5);
	free(DestInfo5);

	free(Info6);
	free(DestInfo6);

	/*free(Info7);
	free(DestInfo7);*/

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		ASSERT(false);
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
	}
	return true;
}
bool DBServer::OnHandleGetNewAccount(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_GetNewAccount* pMsg = (DBR_Cmd_GetNewAccount*)pCmd;
	//生成一个账号名称
	string strAccountName = "";
	for (size_t i = 0; i < (ACCOUNT_LENGTH/2); ++i)
	{
		BYTE ID = RandUInt() % 62;//0-61
		char ch = 0;
		if (ID >= 0 && ID < 10)
		{
			ch = ID + 48;
			//48-57
		}
		else if (ID >= 10 && ID < 36)
		{
			ch = ID + 55;
			//65-90
		}
		else
		{
			ch = ID + 61;
			//97-122
		}
		strAccountName = strAccountName + ch;
	}
	//名称
	string strNickName = "游客";;
	for (size_t i = 0; i < MAX_NICKNAME - 2; ++i)
	{
		BYTE ID = RandUInt() % 62;//0-61
		char ch = 0;
		if (ID >= 0 && ID < 10)
		{
			ch = ID + 48;
			//48-57
		}
		else if (ID >= 10 && ID < 36)
		{
			ch = ID + 55;
			//65-90
		}
		else
		{
			ch = ID + 61;
			//97-122
		}
		strNickName = strNickName + ch;
	}

	UINT Size = 0;
	char* MacAddress = WCharToChar(pMsg->MacAddress, Size);
	char* DestMacAddress = (char*)malloc((Size * 2 + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(MacAddress, Size, DestMacAddress);

	//执行存储过程
	DBO_Cmd_GetNewAccount* msg = (DBO_Cmd_GetNewAccount*)CreateCmd(DBO_GetNewAccount, sizeof(DBO_Cmd_GetNewAccount));
	if (!msg)
	{
		ASSERT(false);
		return false;
	}
	msg->ClientID = pMsg->ClientID;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };

	tm NowTime;
	time_t Now = time(null);
	errno_t Err = localtime_s(&NowTime, &Now);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishGetNewAccount('%s','%s','%u','%u','%u','%s',%d ,'%u','%d.%d.%d.%d','%d-%d-%d %d:%d:%d');", strAccountName.c_str(), DestMacAddress, pMsg->PasswordCrc1, pMsg->PasswordCrc2, pMsg->PasswordCrc3, strNickName.c_str(), 1, m_Config.GetSystemConfig().RsgInitGlobelSum, pMsg->ClientIP & 0xff, (pMsg->ClientIP >> 8) & 0xff, (pMsg->ClientIP >> 16) & 0xff, (pMsg->ClientIP >> 24) & 0xff, NowTime.tm_year + 1900, NowTime.tm_mon + 1, NowTime.tm_mday, NowTime.tm_hour, NowTime.tm_min, NowTime.tm_sec);
	
	free(DestMacAddress);
	free(MacAddress);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		msg->Result = true;
		const TCHAR* pStr = pTable1.GetStr(0, 0);
		TCHARCopy(msg->AccountName, CountArray(msg->AccountName), pStr, _tcslen(pStr));
		msg->PasswordCrc1 = pTable1.GetUint(0, 1);
		msg->PasswordCrc2 = pTable1.GetUint(0, 2);
		msg->PasswordCrc3 = pTable1.GetUint(0, 3);
		msg->dwUserID = pTable1.GetUint(0, 4);
	}
	else
	{
		msg->Result = false;
		ASSERT(false);
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
	}
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleGetRoleInfoByUserID(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//获取玩家具体的数据 根据玩家的ID
	DBR_Cmd_GetAccountInfoByUserID* pMsg = (DBR_Cmd_GetAccountInfoByUserID*)pCmd;
	DBO_Cmd_GetAccountInfoByUserID* msg = (DBO_Cmd_GetAccountInfoByUserID*)CreateCmd(DBO_GetAccountInfoByUserID, sizeof(DBO_Cmd_GetAccountInfoByUserID));
	msg->ClientID = pMsg->ClientID;
	msg->RoleInfo.dwUserID = pMsg->dwUserID;
	msg->Result = false;
	msg->LogonByGameServer = pMsg->LogonByGameServer;
	//执行存储过程

	tm NowTime;
	time_t Now = time(null);
	errno_t Err = localtime_s(&NowTime, &Now);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadUserInfo('%u','%d-%d-%d %d:%d:%d');", pMsg->dwUserID, NowTime.tm_year + 1900, NowTime.tm_mon + 1, NowTime.tm_mday, NowTime.tm_hour, NowTime.tm_min, NowTime.tm_sec);
	//获取存储过程的数据
	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		//获取数据成功了 我们将数据设置到集合里面去
		msg->RoleInfo.dwUserID = pTable1.GetUint(0, 0);
		const TCHAR* pNickName = pTable1.GetStr(0, 1);
		TCHARCopy(msg->RoleInfo.NickName, CountArray(msg->RoleInfo.NickName), pNickName, _tcslen(pNickName));
		msg->RoleInfo.dwFaceID = pTable1.GetUint(0, 2);
		msg->RoleInfo.wLevel = pTable1.GetUshort(0, 3);
		msg->RoleInfo.dwExp = pTable1.GetUint(0, 4);
		msg->RoleInfo.bGender = pTable1.GetBit(0, 5);
		msg->RoleInfo.dwGlobeNum = pTable1.GetUint(0, 6);
		msg->RoleInfo.dwMedalNum = pTable1.GetUint(0, 7);
		msg->RoleInfo.dwCurrencyNum = pTable1.GetUint(0, 8);
		msg->RoleInfo.dwProduction = pTable1.GetUint(0, 9);
		msg->RoleInfo.dwGameTime = pTable1.GetUint(0, 10);
		msg->LastOnlineTime = pTable1.GetDateTime(0, 11);
		msg->RoleInfo.TitleID = pTable1.GetByte(0, 12);
		msg->RoleInfo.dwAchievementPoint = pTable1.GetUint(0, 13);
		msg->RoleInfo.SendGiffSum = pTable1.GetByte(0, 14);
		msg->RoleInfo.AcceptGiffSum = pTable1.GetByte(0, 15);
		//blob
		memcpy_s(&msg->RoleInfo.TaskStates, sizeof(msg->RoleInfo.TaskStates), pTable1.GetField((UINT)0, (UINT)16), sizeof(msg->RoleInfo.TaskStates));
		memcpy_s(&msg->RoleInfo.AchievementStates, sizeof(msg->RoleInfo.AchievementStates), pTable1.GetField((UINT)0, (UINT)17), sizeof(msg->RoleInfo.AchievementStates));
		memcpy_s(&msg->RoleInfo.ActionStates, sizeof(msg->RoleInfo.ActionStates), pTable1.GetField((UINT)0, (UINT)18), sizeof(msg->RoleInfo.ActionStates));
		msg->RoleInfo.OnlineMin = pTable1.GetUshort(0, 19);
		msg->RoleInfo.OnlineRewardStates = pTable1.GetUint(0, 20);
		memcpy_s(&msg->RoleInfo.CharmArray, sizeof(msg->RoleInfo.CharmArray), pTable1.GetField((UINT)0, (UINT)22), sizeof(msg->RoleInfo.CharmArray));
		msg->RoleInfo.IsCanResetAccount = pTable1.GetBit(0, 23);
		msg->RoleInfo.ClientIP = pTable1.GetUint(0, 24);
		msg->RoleInfo.CheckData = pTable1.GetUint(0, 25);
		msg->RoleInfo.IsShowIPAddress = pTable1.GetByte(0, 26) == 1?true:false;//是否显示IP地址
		msg->IsRobot = pTable1.GetByte(0, 27) == 1 ? true : false;//是否为机器人
		msg->RoleInfo.AchievementPointIndex = 0;
		msg->RoleInfo.ExChangeStates = pTable1.GetUint(0, 28);
		msg->RoleInfo.TotalRechargeSum = pTable1.GetUint(0, 29);
		//Server属性
		msg->RoleServerInfo.TotalFishGlobelSum = pTable1.GetInt64(0, 30);
		msg->RoleServerInfo.RoleProtectSum = pTable1.GetUint(0, 31);
		msg->RoleServerInfo.RoleProtectLogTime = pTable1.GetDateTime(0, 32);
		msg->RoleInfo.bIsFirstPayGlobel = pTable1.GetByte(0, 33)==1?true:false;
		msg->RoleInfo.bIsFirstPayCurrcey = pTable1.GetByte(0, 34) == 1 ? true : false;

		bool IsFreeze = pTable1.GetBit(0, 35);
		time_t FreezeEndTime = pTable1.GetDateTime(0, 36);
		if (IsFreeze && time(null) > FreezeEndTime)
		{
			IsFreeze = false;
		}
		msg->IsFreeze = IsFreeze;
		msg->FreezeEndTime = FreezeEndTime;
		if (IsFreeze)
		{
			msg->Result = false;
			OnAddDBResult(Index, ClientID, msg);
			return true;
		}
		/*msg->TableID = pTable1.GetUint(0, 25);
		msg->MonthID = pTable1.GetUint(0, 26);*/
		msg->Result = true;
		bool IsOnceDayLoad = !(m_Config.GetFishUpdateConfig().IsChangeUpdate(msg->LastOnlineTime, time(null)));
		DWORD dwUserID = msg->RoleInfo.dwUserID;
		OnAddDBResult(Index, ClientID, msg);
		return OnHandleLoadAllUserInfo(IsOnceDayLoad,dwUserID, Index, ClientID);
		//return true;
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
		OnAddDBResult(Index, ClientID, msg);
		return true;
	}
}
bool DBServer::OnHandleLoadAllUserInfo(bool IsOnceDay,DWORD dwUserID, BYTE Index, BYTE ClientID)
{
	//进行处理
	{
		DBR_Cmd_LoadUserRelation msg;
		SetMsgInfo(msg, DBR_LoadUserRelation, sizeof(DBR_Cmd_LoadUserRelation));
		msg.dwUserID = dwUserID;
		OnHandleGetUserRelation(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadUserItem msg;
		SetMsgInfo(msg, DBR_LoadUserItem, sizeof(DBR_Cmd_LoadUserItem));
		msg.dwUserID = dwUserID;
		OnHandleGetUserItem(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadUserMail msg;
		SetMsgInfo(msg, DBR_LoadUserMail, sizeof(DBR_Cmd_LoadUserMail));
		msg.dwUserID = dwUserID;
		OnHandleGetUserMail(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadRoleTask msg;
		SetMsgInfo(msg, DBR_LoadRoleTask, sizeof(DBR_Cmd_LoadRoleTask));
		msg.dwUserID = dwUserID;
		OnHandleLoadRoleTask(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadRoleAchievement msg;
		SetMsgInfo(msg, DBR_LoadRoleAchievement, sizeof(DBR_Cmd_LoadRoleAchievement));
		msg.dwUserID = dwUserID;
		OnHandleLoadRoleAchievement(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadRoleTitle msg;
		SetMsgInfo(msg, DBR_LoadRoleTitle, sizeof(DBR_Cmd_LoadRoleTitle));
		msg.dwUserID = dwUserID;
		OnHandleLoadRoleTitle(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadRoleEntity msg;
		SetMsgInfo(msg, DBR_LoadRoleEntity, sizeof(DBR_Cmd_LoadRoleEntity));
		msg.dwUserID = dwUserID;
		OnHandleLoadRoleEntity(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadRoleAction msg;
		SetMsgInfo(msg, DBR_LoadRoleAction, sizeof(DBR_Cmd_LoadRoleAction));
		msg.dwUserID = dwUserID;
		OnHandleLoadRoleAction(Index, ClientID, &msg);
	}
	if (IsOnceDay)
	{
		//同一天登陆 读取数据
		DBR_Cmd_GetNowDayGiff msg;
		SetMsgInfo(msg, DBR_GetNowDayGiff, sizeof(DBR_Cmd_GetNowDayGiff));
		msg.dwUserID = dwUserID;
		OnHandleLoadNowDayGiff(Index, ClientID, &msg);
	}
	else
	{
		DBR_Cmd_ClearNowDayGiff msg;
		SetMsgInfo(msg, DBR_ClearNowDayGiff, sizeof(DBR_Cmd_ClearNowDayGiff));
		msg.dwUserID = dwUserID;
		OnHandleClearNowDayGiff(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadRoleGiff msg;
		SetMsgInfo(msg, DBR_LoadRoleGiff, sizeof(DBR_Cmd_LoadRoleGiff));
		msg.LimitDay = m_Config.GetGiffConfig().LimitDay;
		msg.dwUserID = dwUserID;
		OnHandleLoadRoleGiff(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadGameData msg;
		SetMsgInfo(msg, DBR_LoadGameData, sizeof(DBR_Cmd_LoadGameData));
		msg.dwUserID = dwUserID;
		OnHandleLoadRoleGameData(Index, ClientID, &msg);
	}
	{
		DBR_Cmd_LoadWeekRankInfo msg;
		SetMsgInfo(msg, DBR_LoadWeekRankInfo, sizeof(DBR_Cmd_LoadWeekRankInfo));
		msg.dwUserID = dwUserID;
		msg.IsInit = true;
		OnHandleLoadWeekRankInfo(Index, ClientID, &msg);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//将玩家的数据保存起来
	DBR_Cmd_SaveRoleExInfo* pMsg = (DBR_Cmd_SaveRoleExInfo*)pCmd;
	//设置存储过程的参赛
	SqlTable pTable1;

	char SqlStr[MAXSQL_LENGTH] = { 0 };

	char pTaskStates[MAXBLOB_LENGTH] = { 0 };
	char pAchievementStates[MAXBLOB_LENGTH] = { 0 };
	char pActionStates[MAXBLOB_LENGTH] = { 0 };
	char pCharmArray[MAXBLOB_LENGTH] = { 0 };
	
	m_Sql[Index].GetMySqlEscapeString((char*)&pMsg->RoleInfo.TaskStates, sizeof(pMsg->RoleInfo.TaskStates), pTaskStates);
	m_Sql[Index].GetMySqlEscapeString((char*)&pMsg->RoleInfo.AchievementStates, sizeof(pMsg->RoleInfo.AchievementStates), pAchievementStates);
	m_Sql[Index].GetMySqlEscapeString((char*)&pMsg->RoleInfo.ActionStates, sizeof(pMsg->RoleInfo.ActionStates), pActionStates);
	m_Sql[Index].GetMySqlEscapeString((char*)&pMsg->RoleInfo.CharmArray, sizeof(pMsg->RoleInfo.CharmArray), pCharmArray);

	UINT Size = 0;
	char* NickName = WCharToChar(pMsg->RoleInfo.NickName, Size);
	char* DestNickName = (char*)malloc((Size * 2  + 1) * sizeof(char));
	if (!DestNickName)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestNickName, (Size * 2  + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(NickName, Size, DestNickName);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveUserInfo('%u','%s','%u','%u','%u',%u,'%u','%u','%u','%u','%u','%u','%u','%u','%u','%s','%s','%s','%u','%u','%d','%s','%u','%u','%u');", 
		pMsg->RoleInfo.dwUserID, DestNickName, pMsg->RoleInfo.dwFaceID, pMsg->RoleInfo.wLevel, pMsg->RoleInfo.dwExp,
		pMsg->RoleInfo.bGender, pMsg->RoleInfo.dwGlobeNum, pMsg->RoleInfo.dwMedalNum,pMsg->RoleInfo.dwCurrencyNum, pMsg->RoleInfo.dwProduction, 
		pMsg->RoleInfo.dwGameTime, pMsg->RoleInfo.TitleID, pMsg->RoleInfo.dwAchievementPoint, pMsg->RoleInfo.SendGiffSum, pMsg->RoleInfo.AcceptGiffSum, 
		pTaskStates, pAchievementStates, pActionStates, pMsg->RoleInfo.OnlineMin, pMsg->RoleInfo.OnlineRewardStates,
		pMsg->RoleCharmValue, pCharmArray, pMsg->RoleInfo.ClientIP, pMsg->RoleInfo.CheckData,pMsg->RoleInfo.IsShowIPAddress?1:0);
	free(DestNickName);
	free(NickName);
	

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleNickName(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//FishSaveRoleNickName
	DBR_Cmd_SaveRoleNickName* pMsg = (DBR_Cmd_SaveRoleNickName*)pCmd;
	DBO_Cmd_SaveRoleNickName* msg = (DBO_Cmd_SaveRoleNickName*)CreateCmd(DBO_SaveRoleNickName, sizeof(DBO_Cmd_SaveRoleNickName));
	msg->Result = false;
	msg->dwUserID = pMsg->dwUserID;
	TCHARCopy(msg->NickName, CountArray(msg->NickName), pMsg->NickName, _tcslen(pMsg->NickName));

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;
	char* NickName = WCharToChar(pMsg->NickName, Size);
	char* DestNickName = (char*)malloc((Size * 2 + 1) * sizeof(char));
	if (!DestNickName)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestNickName, (Size * 2 + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(NickName, Size, DestNickName);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleNickName('%u','%s');", pMsg->dwUserID, DestNickName);
	free(DestNickName);
	free(NickName);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 && pTable1.GetUint(0, 0) == 1);
	msg->Result = Result;

	OnAddDBResult(Index, ClientID, msg);//返回命令
	return true;
}
bool DBServer::OnHandleSaveRoleLevel(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//FishSaveRoleLevel
	DBR_Cmd_SaveRoleLevel* pMsg = (DBR_Cmd_SaveRoleLevel*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleLevel('%u','%u');", pMsg->dwUserID,pMsg->wLevel);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleGender(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//FishSaveRoleGender
	DBR_Cmd_SaveRoleGender* pMsg = (DBR_Cmd_SaveRoleGender*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleGender('%u',%d);", pMsg->dwUserID, pMsg->bGender?1:0);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleAchievementPoint(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//FishSaveRoleAchievementPoint
	DBR_Cmd_SaveRoleAchievementPoint* pMsg = (DBR_Cmd_SaveRoleAchievementPoint*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleAchievementPoint('%u','%u');", pMsg->dwUserID, pMsg->dwAchievementPoint);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleCurTitle(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//FishSaveRoleTitle
	DBR_Cmd_SaveRoleCurTitle* pMsg = (DBR_Cmd_SaveRoleCurTitle*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleTitle('%u','%u');", pMsg->dwUserID, pMsg->TitleID);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleCharmArray(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//FishSaveRoleCharmArray
	DBR_Cmd_SaveRoleCharmArray* pMsg = (DBR_Cmd_SaveRoleCharmArray*)pCmd;
	int CharmValue = m_Config.GetCharmValue(pMsg->CharmArray);
	char pCharmArray[MAXBLOB_LENGTH] = { 0 };
	m_Sql[Index].GetMySqlEscapeString((char*)&pMsg->CharmArray, sizeof(pMsg->CharmArray), pCharmArray);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleCharmArray('%u','%d','%s');", pMsg->dwUserID, CharmValue, pCharmArray);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleCurency(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//FishSaveRoleCurency
	DBR_Cmd_SaveRoleCurrency* pMsg = (DBR_Cmd_SaveRoleCurrency*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleCurency('%u','%u');", pMsg->dwUserID, pMsg->dwCurrencyNum);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleFaceID(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//FishSaveRoleFaceID
	DBR_Cmd_SaveRoleFaceID* pMsg = (DBR_Cmd_SaveRoleFaceID*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleFaceID('%u','%u');", pMsg->dwUserID, pMsg->dwFaceID);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleGlobel(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleGlobel* pMsg = (DBR_Cmd_SaveRoleGlobel*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleGlobel('%u','%u');", pMsg->dwUserID, pMsg->dwGlobel);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleOnLineMin(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleOnlineMin * pMsg = (DBR_Cmd_SaveRoleOnlineMin*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleOnLineMin('%u','%u');", pMsg->dwUserID, pMsg->OnLineMin);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleExChangeStates(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleExChangeStates * pMsg = (DBR_Cmd_SaveRoleExChangeStates*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleExChangeStates('%u','%u');", pMsg->dwUserID, pMsg->States);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleTotalRecharge(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleTotalRecharge * pMsg = (DBR_Cmd_SaveRoleTotalRecharge*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleTotalRecharge('%u','%u');", pMsg->dwUserID, pMsg->Sum);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleIsFirstPayGlobel(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//修改玩家是否首冲金币了
	DBR_Cmd_SaveRoleIsFirstPayGlobel * pMsg = (DBR_Cmd_SaveRoleIsFirstPayGlobel*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleIsFirstPayGlobel('%u');", pMsg->dwUserID);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
	
}
bool DBServer::OnHandleSaveRoleIsFirstPayCurrcey(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//修改玩家是否首冲钻石了
	DBR_Cmd_SaveRoleIsFirstPayCurrcey * pMsg = (DBR_Cmd_SaveRoleIsFirstPayCurrcey*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleIsFirstPayCurrcey('%u');", pMsg->dwUserID);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleChangeRoleMoney(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//保存玩家的金币修改
	DBR_Cmd_ChangeRoleMoney * pMsg = (DBR_Cmd_ChangeRoleMoney*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishChangeRoleMoney('%u','%d','%d','%d');", pMsg->dwUserID,pMsg->GlobelSum,pMsg->MedalSum,pMsg->CurrceySum);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleTableChange(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//保存玩家进入桌子的数据
	DBR_Cmd_TableChange * pMsg = (DBR_Cmd_TableChange*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };

	tm NowTime;
	time_t Now = time(null);
	errno_t Err = localtime_s(&NowTime, &Now);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishRoleTableInfo('%u','%u','%u','%u','%u','%u','%d-%d-%d %d:%d:%d',%d);", pMsg->dwUserID, pMsg->TableTypeID, pMsg->TableMonthID, pMsg->GlobelSum, pMsg->MedalSum, pMsg->CurrceySum
		, NowTime.tm_year + 1900, NowTime.tm_mon + 1, NowTime.tm_mday, NowTime.tm_hour, NowTime.tm_min, NowTime.tm_sec, pMsg->JoinOrLeave ? 1:0);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleMedal(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleMedal* pMsg = (DBR_Cmd_SaveRoleMedal*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleMedal('%u','%u');", pMsg->dwUserID, pMsg->dwMedalSum);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleSendGiffSum(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleSendGiffSum* pMsg = (DBR_Cmd_SaveRoleSendGiffSum*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleSendGiffSum('%u','%u');", pMsg->dwUserID, pMsg->SendSum);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleAcceptGiffSum(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleAcceptGiffSum* pMsg = (DBR_Cmd_SaveRoleAcceptGiffSum*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleAcceptGiffSum('%u','%u');", pMsg->dwUserID, pMsg->AcceptSum);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleTaskStates(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleTaskStates* pMsg = (DBR_Cmd_SaveRoleTaskStates*)pCmd;

	char pTaskStates[MAXBLOB_LENGTH] = { 0 };
	m_Sql[Index].GetMySqlEscapeString((char*)&pMsg->States, sizeof(pMsg->States), pTaskStates);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleTaskStates('%u','%s');", pMsg->dwUserID, pTaskStates);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleAchievementStates(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleAchievementStates* pMsg = (DBR_Cmd_SaveRoleAchievementStates*)pCmd;

	char pAchievementStates[MAXBLOB_LENGTH] = { 0 };
	m_Sql[Index].GetMySqlEscapeString((char*)&pMsg->States, sizeof(pMsg->States), pAchievementStates);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleAchievementStates('%u','%s');", pMsg->dwUserID, pAchievementStates);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleActionStates(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleActionStates* pMsg = (DBR_Cmd_SaveRoleActionStates*)pCmd;

	char pActionStates[MAXBLOB_LENGTH] = { 0 };
	m_Sql[Index].GetMySqlEscapeString((char*)&pMsg->States, sizeof(pMsg->States), pActionStates);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleActionStates('%u','%s');", pMsg->dwUserID, pActionStates);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleOnlineStates(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleOnlineStates* pMsg = (DBR_Cmd_SaveRoleOnlineStates*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleOnlineStates('%u','%u');", pMsg->dwUserID, pMsg->OnlineStates);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleCheckData(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleCheckData* pMsg = (DBR_Cmd_SaveRoleCheckData*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleCheckData('%u','%u');", pMsg->dwUserID, pMsg->CheckData);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleIsShopIpAddress(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleIsShowIpAddress* pMsg = (DBR_Cmd_SaveRoleIsShowIpAddress*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleIsShopIpAddress('%u','%u');", pMsg->dwUserID, pMsg->IsShowIP?1:0);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleAllInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleAllInfo* pMsg = (DBR_Cmd_SaveRoleAllInfo*)pCmd;
	//一次性保存玩家全部的数据
	//1.玩家本身 FishSaveRoleOtherInfo
	if ((pMsg->States & MsgBegin) != 0)
	{
		{
			//需要将玩家离线时间保存起来
			SYSTEMTIME pTime;
			GetLocalTime(&pTime);

			tm pLogTime;
			errno_t Error= localtime_s(&pLogTime, &pMsg->RoleServerInfo.RoleProtectLogTime);
			if (Error != 0)
			{
				ASSERT(false);
				return false;
			}

			SqlTable pTable1;
			char SqlStr[MAXSQL_LENGTH] = { 0 };
			sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleOtherInfo(%d,'%u','%u','%u','%u','%u','%u','%u','%lld','%u','%d-%d-%d %d:%d:%d','%d-%d-%d %d:%d:%d');", pMsg->IsNeedResult, pMsg->dwUserID, pMsg->dwGlobeNum, pMsg->OnlineMin, pMsg->dwExp, pMsg->dwProduction, pMsg->dwGameTime, pMsg->ClientIP,
				pMsg->RoleServerInfo.TotalFishGlobelSum, pMsg->RoleServerInfo.RoleProtectSum, 
				pLogTime.tm_year + 1900, pLogTime.tm_mon + 1, pLogTime.tm_mday, pLogTime.tm_hour, pLogTime.tm_min, pLogTime.tm_sec,
				pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
			bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
			if (!Result)
			{
				LogInfoToFile(DBErrorSqlFileName, SqlStr);
				ASSERT(false);
			}
		}
		//2.GameData
		{
			DBR_Cmd_SaveGameData msg;
			SetMsgInfo(msg, DBR_SaveGameData, sizeof(DBR_Cmd_SaveGameData));
			msg.dwUserID = pMsg->dwUserID;
			msg.GameData = pMsg->GameData;
			OnHandleSaveRoleGameData(Index, ClientID, &msg);
		}
	}
	for (int i = 0; i < pMsg->Sum; ++i)
	{
		SaveEventInfo pInfo = pMsg->Array[i];
		switch (pInfo.InfoStates)
		{
		case 1:
			{
				//任务
				  DBR_Cmd_SaveRoleTask msg;
				  SetMsgInfo(msg, DBR_SaveRoleTask, sizeof(DBR_Cmd_SaveRoleTask));
				  msg.dwUserID = pMsg->dwUserID;
				  msg.TaskInfo = pInfo.EventInfo.TaskInfo;
				  OnHandleSaveRoleTask(Index, ClientID, &msg);
			}
			break;
		case 2:
			{
				  DBR_Cmd_SaveRoleAchievement msg;
				  SetMsgInfo(msg, DBR_SaveRoleAchievement, sizeof(DBR_Cmd_SaveRoleAchievement));
				  msg.dwUserID = pMsg->dwUserID;
				  msg.AchievementInfo = pInfo.EventInfo.AchievementInfo;
				  OnHandleSaveRoleAchievement(Index, ClientID, &msg);
			}
			break;
		case 3:
			{
				  DBR_Cmd_SaveRoleAction msg;
				  SetMsgInfo(msg, DBR_SaveRoleAction, sizeof(DBR_Cmd_SaveRoleAction));
				  msg.dwUserID = pMsg->dwUserID;
				  msg.ActionInfo = pInfo.EventInfo.ActionInfo;
				  OnHandleSaveRoleAction(Index, ClientID, &msg);
			}
			break;
		}
	}
	if ((pMsg->States & MsgEnd) != 0)
	{
		if (pMsg->IsNeedResult)
		{
			DBO_Cmd_SaveRoleAllInfo* msg = (DBO_Cmd_SaveRoleAllInfo*)CreateCmd(DBO_SaveRoleAllInfo, sizeof(DBO_Cmd_SaveRoleAllInfo));
			msg->dwUserID = pMsg->dwUserID;
			OnAddDBResult(Index, ClientID, msg);
		}
	}
	return true;
}
bool DBServer::OnHandleGetUserItem(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//加载玩家物品 
	//玩家物品 没办法限制数量 必须进行分包发送
	DBR_Cmd_LoadUserItem* pMsg = (DBR_Cmd_LoadUserItem*)pCmd;

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);
	
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadItem('%u','%d-%d-%d %d:%d:%d');", pMsg->dwUserID, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Rows = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadUserItem)+(Rows - 1)*sizeof(tagItemInfo);
		DBO_Cmd_LoadUserItem * msg = (DBO_Cmd_LoadUserItem*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadUserItem);
		msg->dwUserID = pMsg->dwUserID;
		for (DWORD i = 0; i < Rows;++i)
		{
			msg->Array[i].ItemOnlyID = pTable1.GetUint(i, 0);
			msg->Array[i].ItemID = pTable1.GetUint(i, 1);
			msg->Array[i].ItemSum = pTable1.GetUint(i, 2);
			msg->Array[i].EndTime = pTable1.GetDateTime(i, 3);
		}	
		//命令准备完毕 我们分隔命令
		std::vector<DBO_Cmd_LoadUserItem*> pVec;
		SqlitMsg(msg, PageSize, Rows, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadUserItem*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		//直接发送命令
		DBO_Cmd_LoadUserItem * msg = (DBO_Cmd_LoadUserItem*)CreateCmd(DBO_LoadUserItem, sizeof(DBO_Cmd_LoadUserItem)-sizeof(tagItemInfo));
		if (!msg)
		{
			ASSERT(false);
			return false;
		}
		msg->Sum = 0;
		msg->dwUserID = pMsg->dwUserID;
		msg->States = (MsgBegin | MsgEnd);//开始和结束
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleAddUserItem(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_AddUserItem* pMsg = (DBR_Cmd_AddUserItem*)pCmd;
	DBO_Cmd_AddUserItem* msg = (DBO_Cmd_AddUserItem*)CreateCmd(DBO_AddUserItem, sizeof(DBO_Cmd_AddUserItem));
	msg->dwUserID = pMsg->dwUserID;
	msg->ItemInfo.ItemID = pMsg->ItemInfo.ItemID;
	msg->ItemInfo.ItemSum = pMsg->ItemInfo.ItemSum;
	if (pMsg->ItemInfo.LastMin != 0)
		msg->ItemInfo.EndTime = time(NULL) + pMsg->ItemInfo.LastMin * 60;
	else
		msg->ItemInfo.EndTime = 0;

	tm NowTime;
	errno_t Err = localtime_s(&NowTime, &msg->ItemInfo.EndTime);
	if (Err != 0)
	{
		ASSERT(false);
		return false;
	}
	
	//往玩家背包里添加一件物品
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	if (pMsg->ItemInfo.LastMin != 0)
	{
		sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddItem('%u','%u','%u','%d-%d-%d %d:%d:%d');", pMsg->dwUserID, pMsg->ItemInfo.ItemID, pMsg->ItemInfo.ItemSum,
			NowTime.tm_year + 1900, NowTime.tm_mon + 1, NowTime.tm_mday, NowTime.tm_hour, NowTime.tm_min, NowTime.tm_sec);
	}
	else
	{
		sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddItem('%u','%u','%u','%d-%d-%d %d:%d:%d');", pMsg->dwUserID, pMsg->ItemInfo.ItemID, pMsg->ItemInfo.ItemSum,0,0,0,0,0,0);
	}
	
	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		msg->ItemInfo.ItemOnlyID = pTable1.GetUint(0, 0);
		//添加物品成功 返回函数
		OnAddDBResult(Index, ClientID, msg);
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
		free(msg);
	}
	return true;
}
bool DBServer::OnHandleDelUserItem(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_DelUserItem* pMsg = (DBR_Cmd_DelUserItem*)pCmd;
	//玩家删除指定的物品
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishDelItem('%u');", pMsg->ItemOnlyID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleChangeUserItem(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_ChangeUserItem* pMsg = (DBR_Cmd_ChangeUserItem*)pCmd;
	//修改一组物品的数量
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishChangeUserItem('%u','%u');", pMsg->ItemOnlyID, pMsg->ItemSum);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleGetUserRelation(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LoadUserRelation* pMsg = (DBR_Cmd_LoadUserRelation*)pCmd;
	//加载玩家的关系 分2步  加载玩家主动关系 和 玩家的被动关系
	SqlTable pTable1, pTable2;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadUserRelation('%u','%u');", pMsg->dwUserID,m_Config.GetRelation().MaxRelationSum);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Rows = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadUserRelation)+(Rows - 1)*sizeof(tagRoleRelation);
		DBO_Cmd_LoadUserRelation * msg = (DBO_Cmd_LoadUserRelation*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadUserRelation);
		msg->dwUserID = pMsg->dwUserID;
		for (DWORD i = 0; i < Rows; ++i)
		{
			msg->Array[i].dwDestRoleID = pTable1.GetUint(i, 0);
			msg->Array[i].dwFaceID = pTable1.GetUint(i, 1);
			msg->Array[i].wLevel = pTable1.GetUshort(i, 2);
			msg->Array[i].bGender = pTable1.GetBit(i, 3);
			msg->Array[i].LastLogonTime = pTable1.GetInt64(i, 4);
			msg->Array[i].IsOnline = pTable1.GetBit(i, 5);
			const TCHAR* pNickName = pTable1.GetStr(i, 6);
			TCHARCopy(msg->Array[i].DestNickName, CountArray(msg->Array[i].DestNickName), pNickName, _tcslen(pNickName));
			msg->Array[i].bRelationType = pTable1.GetByte(i, 7);
			msg->Array[i].TitleID = pTable1.GetByte(i, 8);
			msg->Array[i].dwAchievementPoint = pTable1.GetUint(i, 9);
			memcpy_s(&msg->Array[i].CharmArray, sizeof(msg->Array[i].CharmArray), pTable1.GetField((UINT)i, (UINT)10), sizeof(msg->Array[i].CharmArray));
			msg->Array[i].ClientIP = pTable1.GetUint(i, 11);
			msg->Array[i].IsShowIpAddress = pTable1.GetByte(i, 12) == 1 ? true : false;
		}
		//命令创建完毕后 分包发送
		std::vector<DBO_Cmd_LoadUserRelation*> pVec;
		SqlitMsg(msg, PageSize, Rows, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadUserRelation*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadUserRelation* msg = (DBO_Cmd_LoadUserRelation*)CreateCmd(DBO_LoadUserRelation, sizeof(DBO_Cmd_LoadUserRelation) - sizeof(tagRoleRelation));
		msg->dwUserID = pMsg->dwUserID;
		msg->Sum = 0;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	//执行新的存储过程  被动关系的数据 无法限制数量 我们想要进行分包发送 控制好流程
	ZeroMemory(SqlStr, sizeof(SqlStr));
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadBeRelation('%u');", pMsg->dwUserID);

	if (m_Sql[Index].Select(SqlStr, 1, pTable2, true))
	{
		DWORD Rows = pTable2.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadBeUserRelation)+(Rows - 1)*sizeof(tagBeRoleRelation);
		DBO_Cmd_LoadBeUserRelation* msgBe = (DBO_Cmd_LoadBeUserRelation*)malloc(PageSize);
		if (!msgBe)
		{
			ASSERT(false);
			return false;
		}
		msgBe->SetCmdType(DBO_LoadBeUserRelation);
		msgBe->dwUserID = pMsg->dwUserID;
		for (DWORD i = 0; i < Rows;++i)
		{
			msgBe->Array[i].dwUserID = pTable2.GetUint(i, 0);
			msgBe->Array[i].bRelationType = pTable2.GetByte(i, 1);
		}
		std::vector<DBO_Cmd_LoadBeUserRelation*> pVec;
		SqlitMsg(msgBe, PageSize, Rows, false, pVec);
		free(msgBe);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadBeUserRelation*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadBeUserRelation* msgBeFinish = (DBO_Cmd_LoadBeUserRelation*)CreateCmd(DBO_LoadBeUserRelation, sizeof(DBO_Cmd_LoadBeUserRelation)-sizeof(tagBeRoleRelation));
		msgBeFinish->dwUserID = pMsg->dwUserID;
		msgBeFinish->Sum = 0;
		msgBeFinish->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msgBeFinish);

		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleAddUserRelation(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//添加玩家的关系数据
	DBR_Cmd_AddUserRelation* pMsg = (DBR_Cmd_AddUserRelation*)pCmd;
	DBO_Cmd_AddUserRelation* msg = (DBO_Cmd_AddUserRelation*)CreateCmd(DBO_AddUserRelation, sizeof(DBO_Cmd_AddUserRelation));
	msg->dwUserID = pMsg->dwSrcUserID;
	msg->RelationInfo.dwDestRoleID = pMsg->dwDestUserID;
	msg->RelationInfo.bRelationType = pMsg->bRelationType;
	msg->Result = false;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddUserRelation('%u','%u','%u');", pMsg->dwSrcUserID,pMsg->dwDestUserID,pMsg->bRelationType);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		msg->Result = true;
		msg->RelationInfo.dwDestRoleID = pTable1.GetUint(0, 0);
		msg->RelationInfo.dwFaceID = pTable1.GetUint(0, 1);
		msg->RelationInfo.wLevel = pTable1.GetUshort(0, 2);
		msg->RelationInfo.bGender = pTable1.GetBit(0, 3);
		msg->RelationInfo.LastLogonTime = pTable1.GetInt64(0, 4);
		msg->RelationInfo.IsOnline = pTable1.GetBit(0, 5);
		const TCHAR* pNickName = pTable1.GetStr(0, 6);
		TCHARCopy(msg->RelationInfo.DestNickName, CountArray(msg->RelationInfo.DestNickName), pNickName, _tcslen(pNickName));
		msg->RelationInfo.bRelationType = pTable1.GetByte(0, 7);
		msg->RelationInfo.TitleID = pTable1.GetByte(0, 8);
		msg->RelationInfo.dwAchievementPoint = pTable1.GetUint(0, 9);
		memcpy_s(&msg->RelationInfo.CharmArray, sizeof(msg->RelationInfo.CharmArray), pTable1.GetField((UINT)0, (UINT)10), sizeof(msg->RelationInfo.CharmArray));
		msg->RelationInfo.ClientIP = pTable1.GetUint(0, 11);
		msg->RelationInfo.IsShowIpAddress = pTable1.GetByte(0, 12)==1?true:false;
		OnAddDBResult(Index, ClientID, msg);
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleDelUserRelation(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_DelUserRelation* pMsg = (DBR_Cmd_DelUserRelation*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishDelUserRelation('%u','%u');", pMsg->dwSrcUserID, pMsg->dwDestUserID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
//bool DBServer::OnHandleChangeUserRelation(BYTE Index, BYTE ClientID, NetCmd* pCmd)
//{
//	//修改玩家的关系数据
//	DBR_Cmd_ChangeUserRelation* pMsg = (DBR_Cmd_ChangeUserRelation*)pCmd;
//	SqlTable pTable1;
//	char SqlStr[MAXSQL_LENGTH] = { 0 };
//	sprintf_s(SqlStr, sizeof(SqlStr), "call FishChangeUserRelation('%u','%u','%u');", pMsg->dwSrcUserID, pMsg->dwDestUserID, pMsg->bRelationType);
//	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*/*&& pTable1.GetUint(0, 0) == 1*/*/);
//	if (!Result)
//		ASSERT(false);
//	return true;
//}
bool DBServer::OnHandleGetUserMail(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//获取玩家全部的邮件
	DBR_Cmd_LoadUserMail* pMsg = (DBR_Cmd_LoadUserMail*)pCmd;

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);	

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadUserMail('%u','%u','%u', '%d-%d-%d %d:%d:%d');", pMsg->dwUserID, m_Config.GetFishMailConfig().MailLimitDay, m_Config.GetFishMailConfig().MaxUserMailSum
		, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
		
	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		//加载邮件
		DWORD Rows = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadUserMail)+(Rows - 1)*sizeof(tagRoleMail);
		DBO_Cmd_LoadUserMail* msg = (DBO_Cmd_LoadUserMail*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return false;
		}
		msg->SetCmdType(DBO_LoadUserMail);
		msg->dwUserID = pMsg->dwUserID;
		for (DWORD i = 0; i < Rows; ++i)
		{
			msg->Array[i].MailID = pTable1.GetUint(i, 0);
			msg->Array[i].SrcUserID = pTable1.GetUint(i, 1);
			const TCHAR* pContext = pTable1.GetStr(i, 2);
			TCHARCopy(msg->Array[i].Context, CountArray(msg->Array[i].Context), pContext, _tcslen(pContext));
			msg->Array[i].RewardID = pTable1.GetUshort(i, 3);
			msg->Array[i].bIsRead = pTable1.GetBit(i, 4);
			msg->Array[i].bIsExistsReward = pTable1.GetBit(i, 5);
			msg->Array[i].SendTimeLog = pTable1.GetDateTime(i, 6);
			msg->Array[i].SrcFaceID = pTable1.GetUint(i, 7);
			const TCHAR* pNickName = pTable1.GetStr(i, 8);
			TCHARCopy(msg->Array[i].SrcNickName, CountArray(msg->Array[i].SrcNickName), pNickName, _tcslen(pNickName));
		}
		
		std::vector<DBO_Cmd_LoadUserMail*> pVec;
		SqlitMsg(msg, PageSize, Rows, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadUserMail*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadUserMail * msgFinish = (DBO_Cmd_LoadUserMail*)CreateCmd(DBO_LoadUserMail, sizeof(DBO_Cmd_LoadUserMail)-sizeof(tagRoleMail));
		msgFinish->dwUserID = pMsg->dwUserID;
		msgFinish->Sum = 0;
		msgFinish->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msgFinish);

		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleAddUserMail(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//给指定玩家 添加一封邮件
	DBR_Cmd_AddUserMail * pMsg = (DBR_Cmd_AddUserMail*)pCmd;
	DBO_Cmd_AddUserMail* msg = (DBO_Cmd_AddUserMail*)CreateCmd(DBO_AddUserMail, sizeof(DBO_Cmd_AddUserMail));
	msg->dwDestUserID = pMsg->dwDestUserID;
	msg->Result = false;
	msg->MailInfo = pMsg->MailInfo;
	msg->MailInfo.MailID = 0;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };

	UINT Size = 0;
	char* Context = WCharToChar(pMsg->MailInfo.Context, Size);
	char* DestContext = (char*)malloc((Size * 2  + 1) * sizeof(char));
	if (!DestContext)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestContext, (Size * 2  + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Context, Size, DestContext);

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddUserMail('%u','%u','%s','%u','%u','%u','%d-%d-%d %d:%d:%d');", pMsg->MailInfo.SrcUserID, pMsg->dwDestUserID, DestContext,
		pMsg->MailInfo.RewardID, m_Config.GetFishMailConfig().MaxUserMailSum, m_Config.GetFishMailConfig().MailLimitDay
		, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
	free(DestContext);
	free(Context);
	
	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		if (pTable1.GetUint(0, 0) == 0)
		{
			msg->Result = false;
		}
		else
		{
			msg->Result = true;
			msg->MailInfo.MailID = pTable1.GetUint(0, 0);
			msg->MailInfo.SendTimeLog = time(NULL);
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleDelUserMail(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//移除掉一封邮件
	DBR_Cmd_DelUserMail* pMsg = (DBR_Cmd_DelUserMail*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishDelUserMail('%u');", pMsg->dwMailID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleGetUserMailItem(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//从邮件里拿去物品 
	DBR_Cmd_GetUserMailItem* pMsg = (DBR_Cmd_GetUserMailItem*)pCmd;

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishGetMailItem('%u');", pMsg->dwMailID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleChangeUserMailIsRead(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//将邮件设置为已读
	DBR_Cmd_ChangeUserMailIsRead* pMsg = (DBR_Cmd_ChangeUserMailIsRead*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishChangeUserMailIsRead('%u',%d);", pMsg->dwMailID, pMsg->bIsRead?1:0);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleQueryRoleInfoByNickName(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//玩家进行查询
	DBR_Cmd_QueryRoleInfoByNickName* pMsg = (DBR_Cmd_QueryRoleInfoByNickName*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;
	char* NickName = WCharToChar(pMsg->QueryNickName, Size);
	char* DestNickName = (char*)malloc((Size * 2  + 1) * sizeof(char));
	if (!DestNickName)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestNickName, (Size * 2  + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(NickName, Size, DestNickName);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishQueryUserInfoByNickName('%u','%s','%u','%u',%d);", pMsg->dwUserID, DestNickName, pMsg->Page*QUERY_ROLE_SUM, QUERY_ROLE_SUM, pMsg->IsOnline ? 1 : 0);
	free(DestNickName);
	free(NickName);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Row = pTable1.Rows();

		DWORD PageSize = sizeof(DBO_Cmd_QueryRoleInfo)+(Row - 1)*sizeof(tagQueryRoleInfo);
		DBO_Cmd_QueryRoleInfo* msg = (DBO_Cmd_QueryRoleInfo*)malloc(PageSize);
		msg->SetCmdType(DBO_Query_RoleInfo);
		msg->dwUserID = pMsg->dwUserID;
		msg->Page = pMsg->Page;
		for (DWORD i = 0; i < Row; ++i)
		{
			//将查询的数据加载进来
			msg->Array[i].dwUserID = pTable1.GetUint(i, 0);
			const TCHAR* pNickName = pTable1.GetStr(i, 1);
			TCHARCopy(msg->Array[i].NickName, CountArray(msg->Array[i].NickName), pNickName, _tcslen(pNickName));
			msg->Array[i].dwFaceID = pTable1.GetUint(i, 2);
			msg->Array[i].bGender = pTable1.GetBit(i, 3);
			msg->Array[i].wLevel = pTable1.GetUshort(i, 4);
			msg->Array[i].bIsOnline = pTable1.GetBit(i, 5);
			msg->Array[i].TitleID = pTable1.GetByte(i, 6);
			msg->Array[i].dwAchievementPoint = pTable1.GetUint(i, 7);
			memcpy_s(&msg->Array[i].CharmArray, sizeof(msg->Array[i].CharmArray), pTable1.GetField((UINT)i, (UINT)8), sizeof(msg->Array[i].CharmArray));
			msg->Array[i].ClientIP = pTable1.GetUint(i, 9);
		}

		std::vector<DBO_Cmd_QueryRoleInfo*> pVec;
		SqlitMsg(msg, PageSize, Row, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_QueryRoleInfo*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_QueryRoleInfo* msg = (DBO_Cmd_QueryRoleInfo*)CreateCmd(DBO_Query_RoleInfo, sizeof(DBO_Cmd_QueryRoleInfo)-sizeof(tagQueryRoleInfo));
		msg->dwUserID = pMsg->dwUserID;
		msg->Page = pMsg->Page;
		msg->Sum = 0;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleQueryRoleInfoByUserID(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//玩家按ID进行查询
	DBR_Cmd_Query_RoleInfoByUserID* pMsg = (DBR_Cmd_Query_RoleInfoByUserID*)pCmd;
	DBO_Cmd_Query_RoleInfoByUserID* msg = (DBO_Cmd_Query_RoleInfoByUserID*)CreateCmd(DBO_Query_RoleInfoByUserID, sizeof(DBO_Cmd_Query_RoleInfoByUserID));
	msg->dwUserID = pMsg->dwUserID;
	msg->RoleInfo.dwUserID = 0;

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishQueryUserInfoByUserID('%u');", pMsg->dwDestUserID);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		msg->RoleInfo.dwUserID = pTable1.GetUint(0, 0);
		const TCHAR* pNickName = pTable1.GetStr(0, 1);
		TCHARCopy(msg->RoleInfo.NickName, CountArray(msg->RoleInfo.NickName), pNickName, _tcslen(pNickName));
		msg->RoleInfo.dwFaceID = pTable1.GetUint(0, 2);
		msg->RoleInfo.bGender = pTable1.GetBit(0, 3);
		msg->RoleInfo.wLevel = pTable1.GetUshort(0, 4);
		msg->RoleInfo.bIsOnline = pTable1.GetBit(0, 5);
		msg->RoleInfo.TitleID = pTable1.GetByte(0, 6);
		msg->RoleInfo.dwAchievementPoint = pTable1.GetUint(0, 7);
		memcpy_s(&msg->RoleInfo.CharmArray, sizeof(msg->RoleInfo.CharmArray), pTable1.GetField((UINT)0, (UINT)8), sizeof(msg->RoleInfo.CharmArray));
		msg->RoleInfo.ClientIP = pTable1.GetUint(0, 9);
		msg->RoleInfo.IsShowIpAddress = pTable1.GetByte(0, 10)==1?true:false;
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
//bool DBServer::OnHandleLoadRoleCheckInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
//{
//	DBR_Cmd_LoadRoleCheckInfo* pMsg = (DBR_Cmd_LoadRoleCheckInfo*)pCmd;
//	DBO_Cmd_LoadRoleCheckInfo* msg = (DBO_Cmd_LoadRoleCheckInfo*)CreateCmd(DBO_LoadRoleCheckInfo, sizeof(DBO_Cmd_LoadRoleCheckInfo));
//	msg->dwUserID = pMsg->dwUserID;
//	//执行存储过程
//	SqlTable pTable1;
//	char SqlStr[MAXSQL_LENGTH] = { 0 };
//	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadRoleCheckInfo('%u');",pMsg->dwUserID);
//	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
//	{
//		if (pTable1.Rows() == 0)
//		{
//			msg->CheckInfo.bCheckSum = 0;
//			msg->CheckInfo.bLastLogDay = 0;
//			msg->CheckInfo.bLastLogMonth = 0;
//			msg->CheckInfo.bLastLogYear = 0;
//			msg->CheckInfo.bMontheCheckSum = 0;
//		}
//		else if (pTable1.Rows() == 1)
//		{
//			msg->CheckInfo.bCheckSum = pTable1.GetByte(0, 0);
//			msg->CheckInfo.bMontheCheckSum = pTable1.GetByte(0, 1);
//			__int64 LogTime = pTable1.GetDateTime(0, 2);
//			tm* pTime = localtime(&LogTime);
//			msg->CheckInfo.bLastLogDay = pTime->tm_mday;
//			msg->CheckInfo.bLastLogMonth = pTime->tm_mon + 1;
//			msg->CheckInfo.bLastLogYear = pTime->tm_year - 100;//1900 到2000
//		}
//		else
//		{
//			msg->CheckInfo.bCheckSum = 0;
//			msg->CheckInfo.bLastLogDay = 0;
//			msg->CheckInfo.bLastLogMonth = 0;
//			msg->CheckInfo.bLastLogYear = 0;
//			msg->CheckInfo.bMontheCheckSum = 0;
//			ASSERT(false);
//		}
//	}
//	else
//	{
//		msg->CheckInfo.bCheckSum = 0;
//		msg->CheckInfo.bLastLogDay = 0;
//		msg->CheckInfo.bLastLogMonth = 0;
//		msg->CheckInfo.bLastLogYear = 0;
//		msg->CheckInfo.bMontheCheckSum = 0;
//		ASSERT(false);
//	}
//	OnAddDBResult(Index, ClientID, msg);
//	return true;
//}
//bool DBServer::OnHandleUpdateRoleCheckInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
//{
//	//保存玩家的签到数据
//	DBR_Cmd_ChangeRoleCheckInfo * pMsg = (DBR_Cmd_ChangeRoleCheckInfo*)pCmd;
//	SqlTable pTable1;
//	char SqlStr[MAXSQL_LENGTH] = { 0 };
//	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleCheckInfo('%u','%u','%u','%04d-%02d-%02d %02d:%02d:%02d');", pMsg->dwUserID, pMsg->CheckInfo.bCheckSum, pMsg->CheckInfo.bMontheCheckSum, 
//		pMsg->CheckInfo.bLastLogYear + 2000, pMsg->CheckInfo.bLastLogMonth, pMsg->CheckInfo.bLastLogDay,0,0,0);
//	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 && pTable1.GetUint(0,0) == 1);
//	if (!Result)
//		ASSERT(false);
//	return true;
//}
bool DBServer::OnHandleLoadRoleTask(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LoadRoleTask* pMsg = (DBR_Cmd_LoadRoleTask*)pCmd;
	//DBO_Cmd_LoadRoleTask* msg = (DBO_Cmd_LoadRoleTask)
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadTask('%u');", pMsg->dwUserID);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Row = pTable1.Rows();

		DWORD PageSize = sizeof(DBO_Cmd_LoadRoleTask)+(Row - 1)*sizeof(tagRoleTaskInfo);
		DBO_Cmd_LoadRoleTask* msg = (DBO_Cmd_LoadRoleTask*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadRoleTask);
		msg->dwUserID = pMsg->dwUserID;
		for (DWORD i = 0; i < Row; ++i)
		{
			msg->Array[i].TaskID = pTable1.GetByte(i, 0);
			msg->Array[i].TaskValue = pTable1.GetUint(i, 1);
		}

		std::vector<DBO_Cmd_LoadRoleTask*> pVec;
		SqlitMsg(msg, PageSize, Row, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadRoleTask*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadRoleTask* msg = (DBO_Cmd_LoadRoleTask*)CreateCmd(DBO_LoadRoleTask, sizeof(DBO_Cmd_LoadRoleTask) - sizeof(tagRoleTaskInfo));
		msg->Sum = 0;
		msg->dwUserID = pMsg->dwUserID;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleTask(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//保存一个任务
	DBR_Cmd_SaveRoleTask* pMsg = (DBR_Cmd_SaveRoleTask*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveTask('%u','%u','%u');", pMsg->dwUserID, pMsg->TaskInfo.TaskID, pMsg->TaskInfo.TaskValue);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}

	/*char IpStr[64] = { 0 };
	sprintf_s(IpStr, sizeof(IpStr), "玩家 %d 正在保存任务\n", pMsg->dwUserID);
	std::cout << IpStr;*/

	return true;
}
bool DBServer::OnHandleSaveAllTask(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveAllTask* pMsg = (DBR_Cmd_SaveAllTask*)pCmd;
	DBR_Cmd_SaveRoleTask msg;
	SetMsgInfo(msg, DBR_SaveRoleTask, sizeof(DBR_Cmd_SaveRoleTask));
	for (BYTE i = 0; i < pMsg->Sum; ++i)
	{	
		msg.dwUserID = pMsg->dwUserID;
		msg.TaskInfo = pMsg->Array[i];
		OnHandleSaveRoleTask(Index, ClientID, &msg);
	}
	return true;
}
bool DBServer::OnHandleDelRoleTask(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_DelRoleTask* pMsg = (DBR_Cmd_DelRoleTask*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishDelTask('%u','%u');", pMsg->dwUserID,pMsg->bTaskID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleClearRoleTask(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//清理当前数据库的全部的任务 当前玩家的
	DBR_Cmd_ClearRoleTask* pMsg = (DBR_Cmd_ClearRoleTask*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishClearRoleTask('%u');", pMsg->dwUserID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleLoadRoleAchievement(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LoadRoleAchievement* pMsg = (DBR_Cmd_LoadRoleAchievement*)pCmd;
	//DBO_Cmd_LoadRoleAchievement* msg = (DBO_Cmd_LoadRoleAchievement)
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadAchievement('%u');", pMsg->dwUserID);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Row = pTable1.Rows();

		DWORD PageSize = sizeof(DBO_Cmd_LoadRoleAchievement)+(Row - 1)*sizeof(tagRoleAchievementInfo);

		DBO_Cmd_LoadRoleAchievement* msg = (DBO_Cmd_LoadRoleAchievement*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadRoleAchievement);
		msg->dwUserID = pMsg->dwUserID;
		for (DWORD i = 0; i < Row; ++i)
		{
			msg->Array[i].AchievementID = pTable1.GetByte(i, 0);
			msg->Array[i].AchievementValue = pTable1.GetUint(i, 1);
		}

		std::vector<DBO_Cmd_LoadRoleAchievement*> pVec;
		SqlitMsg(msg, PageSize, Row, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadRoleAchievement*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		
		DBO_Cmd_LoadRoleAchievement* msg = (DBO_Cmd_LoadRoleAchievement*)CreateCmd(DBO_LoadRoleAchievement, sizeof(DBO_Cmd_LoadRoleAchievement)-sizeof(tagRoleAchievementInfo));
		msg->Sum = 0;
		msg->dwUserID = pMsg->dwUserID;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleAchievement(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//保存一个任务
	DBR_Cmd_SaveRoleAchievement* pMsg = (DBR_Cmd_SaveRoleAchievement*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveAchievement('%u','%u','%u');", pMsg->dwUserID, pMsg->AchievementInfo.AchievementID, pMsg->AchievementInfo.AchievementValue);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveAllAchievement(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveAllAchievement* pMsg = (DBR_Cmd_SaveAllAchievement*)pCmd;
	DBR_Cmd_SaveRoleAchievement msg;
	SetMsgInfo(msg, DBR_SaveRoleAchievement, sizeof(DBR_Cmd_SaveRoleAchievement));
	for (BYTE i = 0; i < pMsg->Sum; ++i)
	{
		msg.dwUserID = pMsg->dwUserID;
		msg.AchievementInfo = pMsg->Array[i];
		OnHandleSaveRoleAchievement(Index, ClientID, &msg);
	}
	return true;
}
bool DBServer::OnHandleDelRoleAchievement(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_DelRoleAchievement* pMsg = (DBR_Cmd_DelRoleAchievement*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishDelAchievement('%u','%u');", pMsg->dwUserID, pMsg->bAchievementID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleLoadRoleAction(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LoadRoleAction* pMsg = (DBR_Cmd_LoadRoleAction*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadAction('%u');", pMsg->dwUserID);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Row = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadRoleAction)+(Row - 1)*sizeof(tagRoleActionInfo);
		DBO_Cmd_LoadRoleAction* msg = (DBO_Cmd_LoadRoleAction*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadRoleAction);
		msg->dwUserID = pMsg->dwUserID;
		for (DWORD i = 0; i < Row; ++i)
		{
			msg->Array[i].ActionID = pTable1.GetByte(i, 0);
			msg->Array[i].ActionValue = pTable1.GetUint(i, 1);
			msg->Array[i].ActionStates = pTable1.GetUint(i, 2);
		}

		std::vector<DBO_Cmd_LoadRoleAction*> pVec;
		SqlitMsg(msg, PageSize, Row, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadRoleAction*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		
		DBO_Cmd_LoadRoleAction* msg = (DBO_Cmd_LoadRoleAction*)CreateCmd(DBO_LoadRoleAction, sizeof(DBO_Cmd_LoadRoleAction)-sizeof(tagRoleActionInfo));
		msg->Sum = 0;
		msg->dwUserID = pMsg->dwUserID;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleAction(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//保存一个任务
	DBR_Cmd_SaveRoleAction* pMsg = (DBR_Cmd_SaveRoleAction*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveAction('%u','%u','%u','%u');", pMsg->dwUserID, pMsg->ActionInfo.ActionID, pMsg->ActionInfo.ActionValue, pMsg->ActionInfo.ActionStates);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveAllAction(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveAllAction* pMsg = (DBR_Cmd_SaveAllAction*)pCmd;
	DBR_Cmd_SaveRoleAction msg;
	SetMsgInfo(msg, DBR_SaveRoleAction, sizeof(DBR_Cmd_SaveRoleAction));
	for (BYTE i = 0; i < pMsg->Sum; ++i)
	{
		msg.dwUserID = pMsg->dwUserID;
		msg.ActionInfo = pMsg->Array[i];
		OnHandleSaveRoleAction(Index, ClientID, &msg);
	}
	return true;
}
bool DBServer::OnHandleDelRoleAction(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_DelRoleAction* pMsg = (DBR_Cmd_DelRoleAction*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishDelAction('%u','%u');", pMsg->dwUserID, pMsg->bActionID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleLoadRoleTitle(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//加载玩家全部的称号
	DBR_Cmd_LoadRoleTitle* pMsg = (DBR_Cmd_LoadRoleTitle*)pCmd;

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadTitle('%u');", pMsg->dwUserID);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		int Row = pTable1.Rows();

		DWORD PageSize = sizeof(DBO_Cmd_LoadRoleTitle)+(Row - 1)*sizeof(BYTE);
		DBO_Cmd_LoadRoleTitle* msg = (DBO_Cmd_LoadRoleTitle*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadRoleTitle);
		msg->dwUserID = pMsg->dwUserID;
		for (int i = 0; i < Row; ++i)
		{
			msg->Array[i] = pTable1.GetByte(i, 0);
		}

		std::vector<DBO_Cmd_LoadRoleTitle*> pVec;
		SqlitMsg(msg, PageSize, Row, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadRoleTitle*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		
		DBO_Cmd_LoadRoleTitle* msg = (DBO_Cmd_LoadRoleTitle*)CreateCmd(DBO_LoadRoleTitle, sizeof(DBO_Cmd_LoadRoleTitle)-sizeof(BYTE));
		msg->dwUserID = pMsg->dwUserID;
		msg->Sum = 0;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleAddRoleTitle(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_AddRoleTitle* pMsg = (DBR_Cmd_AddRoleTitle*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddTitle('%u','%u');", pMsg->dwUserID, pMsg->TitleID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleDelRoleTitle(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_DelRoleTitle* pMsg = (DBR_Cmd_DelRoleTitle*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishDelTitle('%u','%u');", pMsg->dwUserID, pMsg->TitleID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleEntityItem(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//插入数据
	DBR_Cmd_AddRoleEntityItem* pMsg = (DBR_Cmd_AddRoleEntityItem*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;
	char* pAddress = WCharToChar(pMsg->Addres, Size);
	char* DestAddress = (char*)malloc((Size * 2  + 1) * sizeof(char));
	if (!DestAddress)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestAddress, (Size * 2  + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(pAddress, Size, DestAddress);

	char* pName = WCharToChar(pMsg->Name, Size);
	char* DestName = (char*)malloc((Size * 2  + 1) * sizeof(char));
	if (!DestName)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestName, (Size * 2  + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(pName, Size, DestName);

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddRoleEntityItem('%u','%u','%u','%s','%llu','%s', '%d-%d-%d %d:%d:%d');", pMsg->dwUserID, pMsg->ItemID, pMsg->ItemSum, DestAddress,pMsg->Phone, DestName
			, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
	free(DestAddress);
	free(pAddress);
	free(DestName);
	free(pName);
	
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleLoadRoleEntity(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LoadRoleEntity* pMsg = (DBR_Cmd_LoadRoleEntity*)pCmd;
	DBO_Cmd_LoadRoleEntity* msg = (DBO_Cmd_LoadRoleEntity*)CreateCmd(DBO_LoadRoleEntity, sizeof(DBO_Cmd_LoadRoleEntity));
	msg->dwUserID = pMsg->dwUserID;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadUserEntity('%u');", pMsg->dwUserID);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		TCHARCopy(msg->RoleInfo.Name, CountArray(msg->RoleInfo.Name), pTable1.GetStr(0, 0), _tcslen(pTable1.GetStr(0, 0)));
		msg->RoleInfo.Phone = pTable1.GetUint64(0, 1);
		TCHARCopy(msg->RoleInfo.Email, CountArray(msg->RoleInfo.Email), pTable1.GetStr(0, 2), _tcslen(pTable1.GetStr(0, 2)));
		TCHARCopy(msg->RoleInfo.IdentityID, CountArray(msg->RoleInfo.IdentityID), pTable1.GetStr(0, 3), _tcslen(pTable1.GetStr(0, 3)));
		TCHARCopy(msg->RoleInfo.EntityItemUseName, CountArray(msg->RoleInfo.EntityItemUseName), pTable1.GetStr(0, 4), _tcslen(pTable1.GetStr(0, 4)));
		msg->RoleInfo.EntityItemUsePhone = pTable1.GetUint64(0, 5);
		TCHARCopy(msg->RoleInfo.EntityItemUseAddres, CountArray(msg->RoleInfo.EntityItemUseAddres), pTable1.GetStr(0, 6), _tcslen(pTable1.GetStr(0, 6)));
	}
	else
	{
		TCHARCopy(msg->RoleInfo.EntityItemUseName, CountArray(msg->RoleInfo.EntityItemUseName), TEXT(""), 0);
		msg->RoleInfo.Phone = 0;
		TCHARCopy(msg->RoleInfo.EntityItemUseAddres, CountArray(msg->RoleInfo.EntityItemUseAddres), TEXT(""), 0);
		TCHARCopy(msg->RoleInfo.Email, CountArray(msg->RoleInfo.Email), TEXT(""), 0);
		TCHARCopy(msg->RoleInfo.IdentityID, CountArray(msg->RoleInfo.IdentityID), TEXT(""), 0);
		TCHARCopy(msg->RoleInfo.Name, CountArray(msg->RoleInfo.Name), TEXT(""), 0);
		msg->RoleInfo.EntityItemUsePhone = 0;
	}
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleSaveRoleEntityName(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleEntityName* pMsg = (DBR_Cmd_SaveRoleEntityName*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;
	char* pName = WCharToChar(pMsg->Name, Size);
	char* DestName = (char*)malloc((Size * 2 + 1) * sizeof(char));
	if (!DestName)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestName, (Size * 2 + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(pName, Size, DestName);
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleEntityName('%u','%s');", pMsg->dwUserID, DestName);
	free(DestName);
	free(pName);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleEntityPhone(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleEntityPhone* pMsg = (DBR_Cmd_SaveRoleEntityPhone*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleEntityPhone('%u','%llu');", pMsg->dwUserID, pMsg->Phone);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleEntityEmail(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleEntityEmail* pMsg = (DBR_Cmd_SaveRoleEntityEmail*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;

	char* pEmail = WCharToChar(pMsg->Email, Size);
	char* DestEmail = (char*)malloc((Size * 2 + 1) * sizeof(char));
	if (!DestEmail)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestEmail, (Size * 2 + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(pEmail, Size, DestEmail);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleEntityEmail('%u','%s');", pMsg->dwUserID, DestEmail);
	free(DestEmail);
	free(pEmail);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleEntityIdentityID(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleEntityIdentityID* pMsg = (DBR_Cmd_SaveRoleEntityIdentityID*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;

	char* pIdentityID = WCharToChar(pMsg->IdentityID, Size);
	char* DestIdentityID = (char*)malloc((Size * 2 + 1) * sizeof(char));
	if (!DestIdentityID)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestIdentityID, (Size * 2 + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(pIdentityID, Size, DestIdentityID);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleEntityIdentityID('%u','%s');", pMsg->dwUserID, DestIdentityID);
	free(DestIdentityID);
	free(pIdentityID);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleEntityItemUseName(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleEntityItemUseName* pMsg = (DBR_Cmd_SaveRoleEntityItemUseName*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;

	char* pEntityItemUseName = WCharToChar(pMsg->EntityItemUseName, Size);
	char* DestEntityItemUseName = (char*)malloc((Size * 2 + 1) * sizeof(char));
	if (!DestEntityItemUseName)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestEntityItemUseName, (Size * 2 + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(pEntityItemUseName, Size, DestEntityItemUseName);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleEntityItemUseName('%u','%s');", pMsg->dwUserID, DestEntityItemUseName);
	free(DestEntityItemUseName);
	free(pEntityItemUseName);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleEntityItemUsePhone(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleEntityItemUsePhone* pMsg = (DBR_Cmd_SaveRoleEntityItemUsePhone*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleEntityItemUsePhone('%u','%llu');", pMsg->dwUserID, pMsg->EntityItemUsePhone);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveRoleEntityItemUseAddress(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleEntityItemUseAddress* pMsg = (DBR_Cmd_SaveRoleEntityItemUseAddress*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	UINT Size = 0;

	char* pEntityItemUseAddres = WCharToChar(pMsg->EntityItemUseAddres, Size);
	char* DestEntityItemUseAddres = (char*)malloc((Size * 2 + 1) * sizeof(char));
	if (!DestEntityItemUseAddres)
	{
		ASSERT(false);
		return false;
	}
	ZeroMemory(DestEntityItemUseAddres, (Size * 2 + 1) * sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(pEntityItemUseAddres, Size, DestEntityItemUseAddres);

	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleEntityItemUseAddress('%u','%s');", pMsg->dwUserID, DestEntityItemUseAddres);

	free(DestEntityItemUseAddres);
	free(pEntityItemUseAddres);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
//bool DBServer::OnHandleSaveRoleEntity(BYTE Index, BYTE ClientID, NetCmd* pCmd)
//{
//	//保存玩家的实体数据
//	DBR_Cmd_SaveRoleEntity* pMsg = (DBR_Cmd_SaveRoleEntity*)pCmd;
//	SqlTable pTable1;
//	char SqlStr[MAXSQL_LENGTH] = { 0 };
//	UINT Size = 0;
//	char* pName		= WCharToChar(pMsg->RoleInfo.Name, Size);
//	char* DestName = (char*)malloc((Size * 2  + 1) * sizeof(char));
//	ZeroMemory(DestName, (Size * 2  + 1) * sizeof(char));
//	m_Sql[Index].GetMySqlEscapeString(pName, Size, DestName);
//
//	char* pPhone	= WCharToChar(pMsg->RoleInfo.Phone, Size);
//	char* DestPhone = (char*)malloc((Size * 2  + 1) * sizeof(char));
//	ZeroMemory(DestPhone, (Size * 2  + 1) * sizeof(char));
//	m_Sql[Index].GetMySqlEscapeString(pPhone, Size, DestPhone);
//
//	char* pEmail	= WCharToChar(pMsg->RoleInfo.Email, Size);
//	char* DestEmail = (char*)malloc((Size * 2  + 1) * sizeof(char));
//	ZeroMemory(DestEmail, (Size * 2  + 1) * sizeof(char));
//	m_Sql[Index].GetMySqlEscapeString(pEmail, Size, DestEmail);
//
//	char* pIdentityID = WCharToChar(pMsg->RoleInfo.IdentityID, Size);
//	char* DestIdentityID = (char*)malloc((Size * 2  + 1) * sizeof(char));
//	ZeroMemory(DestIdentityID, (Size * 2  + 1) * sizeof(char));
//	m_Sql[Index].GetMySqlEscapeString(pIdentityID, Size, DestIdentityID);
//
//
//	char* pEntityItemUseAddres = WCharToChar(pMsg->RoleInfo.EntityItemUseAddres, Size);
//	char* DestEntityItemUseAddres = (char*)malloc((Size * 2 + 1) * sizeof(char));
//	ZeroMemory(DestEntityItemUseAddres, (Size * 2 + 1) * sizeof(char));
//	m_Sql[Index].GetMySqlEscapeString(pEntityItemUseAddres, Size, DestEntityItemUseAddres);
//
//	char* pEntityItemUseName = WCharToChar(pMsg->RoleInfo.EntityItemUseName, Size);
//	char* DestEntityItemUseName = (char*)malloc((Size * 2 + 1) * sizeof(char));
//	ZeroMemory(DestEntityItemUseName, (Size * 2 + 1) * sizeof(char));
//	m_Sql[Index].GetMySqlEscapeString(pEntityItemUseName, Size, DestEntityItemUseName);
//
//	char* pEntityItemUsePhone = WCharToChar(pMsg->RoleInfo.EntityItemUsePhone, Size);
//	char* DestEntityItemUsePhone = (char*)malloc((Size * 2 + 1) * sizeof(char));
//	ZeroMemory(DestEntityItemUsePhone, (Size * 2 + 1) * sizeof(char));
//	m_Sql[Index].GetMySqlEscapeString(pEntityItemUsePhone, Size, DestEntityItemUsePhone);
//
//	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleEntity('%u','%s','%s','%s','%s','%s','%s','%s');", pMsg->dwUserID, DestName, DestPhone, DestEmail, DestIdentityID, DestEntityItemUseName, DestEntityItemUsePhone, DestEntityItemUseAddres);
//	free(DestName);
//	free(pName);
//	free(DestPhone);
//	free(pPhone);
//	free(DestEmail);
//	free(pEmail);
//	free(DestIdentityID);
//	free(pIdentityID);
//	free(DestEntityItemUseAddres);
//	free(pEntityItemUseAddres);
//	free(DestEntityItemUseName);
//	free(pEntityItemUseName);
//	free(DestEntityItemUsePhone);
//	free(pEntityItemUsePhone);
//
//	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
//	if (!Result)
//		ASSERT(false);
//	return true;
//}
bool DBServer::OnHandleLoadRoleGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//加载 发送给直接的赠送 并且加载 玩家今天发生的赠送的数据 如赠送过哪些人
	//1.加载已经接收到的数据
	DBR_Cmd_LoadRoleGiff* pMsg = (DBR_Cmd_LoadRoleGiff*)pCmd;

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadGiff('%u','%u','%u', '%d-%d-%d %d:%d:%d');", pMsg->dwUserID, pMsg->LimitDay,m_Config.GetGiffConfig().MaxUserGiffSum
		, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Row = pTable1.Rows();

		DWORD PageSize = sizeof(DBO_Cmd_LoadRoleGiff)+(Row - 1)*sizeof(tagGiffInfo);
		DBO_Cmd_LoadRoleGiff* msg = (DBO_Cmd_LoadRoleGiff*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadRoleGiff);
		msg->dwUserID = pMsg->dwUserID;
		for (DWORD i = 0; i < Row; ++i)
		{
			msg->Array[i].OnlyID = pTable1.GetUint(i, 0);
			msg->Array[i].dwUserID = pTable1.GetUint(i, 1);
			msg->Array[i].SendTime = pTable1.GetInt64(i, 2);
			TCHARCopy(msg->Array[i].NickName, CountArray(msg->Array[i].NickName), pTable1.GetStr(i, 3), _tcslen(pTable1.GetStr(i, 3)));
			msg->Array[i].dwFaceID = pTable1.GetUint(i, 4);
		}

		std::vector<DBO_Cmd_LoadRoleGiff*> pVec;
		SqlitMsg(msg, PageSize, Row, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadRoleGiff*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadRoleGiff* msg = (DBO_Cmd_LoadRoleGiff*)CreateCmd(DBO_LoadRoleGiff, sizeof(DBO_Cmd_LoadRoleGiff) - sizeof(tagGiffInfo));
		msg->dwUserID = pMsg->dwUserID;
		msg->Sum = 0;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	////2.加载玩家今天发送过的列表数据
	//ZeroMemory(SqlStr, sizeof(SqlStr));
	//sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadNowDaySendGiff('%u','%u','%u');", pMsg->dwUserID, m_Config.GetGiffConfig().SendSumLimitByDay,m_Config.GetWriteSec());

	//if (m_Sql[Index].Select(SqlStr, 1, pTable2, true))
	//{
	//	int Row = pTable2.Rows();

	//	DWORD PageSize = sizeof(DBO_Cmd_GetNowDayGiff)+(Row - 1)*sizeof(tagNowDaySendGiffInfo);
	//	if (PageSize >= DB_Msg_PageSum)
	//	{
	//		ASSERT(false);
	//		return false;
	//	}

	//	DBO_Cmd_GetNowDayGiff* msg = (DBO_Cmd_GetNowDayGiff*)CreateCmd(DBO_GetNowDayGiff, ConvertDWORDToWORD(PageSize));
	//	msg->dwUserID = pMsg->dwUserID;
	//	msg->GiffSum = ConvertIntToBYTE(Row);
	//	for (int i = 0; i < Row; ++i)
	//	{
	//		msg->GiffArray[i].dwUserID = pTable2.GetUint(i, 0);
	//		msg->GiffArray[i].SendSum = pTable2.GetByte(i, 1);
	//	}
	//	OnAddDBResult(Index, ClientID, msg);
	//}
	//else
	//{
	//	DBO_Cmd_GetNowDayGiff* msg = (DBO_Cmd_GetNowDayGiff*)CreateCmd(DBO_GetNowDayGiff, sizeof(DBO_Cmd_GetNowDayGiff)-sizeof(tagNowDaySendGiffInfo));
	//	msg->dwUserID = pMsg->dwUserID;
	//	msg->GiffSum = 0;
	//	OnAddDBResult(Index, ClientID, msg);
	//	ASSERT(false);
	//}
	return true;
}
bool DBServer::OnHandleLoadNowDayGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_GetNowDayGiff* pMsg = (DBR_Cmd_GetNowDayGiff*)pCmd;

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadNowDaySendGiff('%u');", pMsg->dwUserID);

	if (m_Sql[Index].Select(SqlStr, 1, pTable1, true))
	{
		DWORD Row = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_GetNowDayGiff)+(Row - 1)*sizeof(tagNowDaySendGiffInfo);
		DBO_Cmd_GetNowDayGiff* msg = (DBO_Cmd_GetNowDayGiff*)malloc(PageSize);
		msg->SetCmdType(DBO_GetNowDayGiff);
		msg->dwUserID = pMsg->dwUserID;
		for (DWORD i = 0; i < Row; ++i)
		{
			msg->Array[i].dwUserID = pTable1.GetUint(i, 0);
			msg->Array[i].SendSum = pTable1.GetByte(i, 1);
		}

		std::vector<DBO_Cmd_GetNowDayGiff*> pVec;
		SqlitMsg(msg, PageSize, Row, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_GetNowDayGiff*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_GetNowDayGiff* msg = (DBO_Cmd_GetNowDayGiff*)CreateCmd(DBO_GetNowDayGiff, sizeof(DBO_Cmd_GetNowDayGiff)-sizeof(tagNowDaySendGiffInfo));
		msg->dwUserID = pMsg->dwUserID;
		msg->Sum = 0;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleClearNowDayGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_ClearNowDayGiff* pMsg = (DBR_Cmd_ClearNowDayGiff*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishClearNowDaySendGiff('%u');", pMsg->dwUserID);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleAddRoleGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//发送一个赠送给玩家 记录到数据库去
	DBR_Cmd_AddRoleGiff* pMsg = (DBR_Cmd_AddRoleGiff*)pCmd;
	DBO_Cmd_AddRoleGiff* msg = (DBO_Cmd_AddRoleGiff*)CreateCmd(DBO_AddRoleGiff, sizeof(DBO_Cmd_AddRoleGiff));
	msg->Result = false;
	msg->dwDestUserID = pMsg->DestUserID;
	msg->GiffInfo.dwUserID = pMsg->SrcUserID;

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddGiff('%u','%u','%u','%u','%u', '%d-%d-%d %d:%d:%d');", pMsg->SrcUserID, pMsg->DestUserID, m_Config.GetGiffConfig().MaxUserGiffSum, m_Config.GetGiffConfig().LimitDay, m_Config.GetGiffConfig().SendOnceUserLimitByDay
		, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1)
	{
		if (pTable1.GetUint(0, 0) == 0)
		{
			msg->Result = false;
			OnAddDBResult(Index, ClientID, msg);
		}
		else
		{
			msg->Result = true;
			msg->GiffInfo.OnlyID = pTable1.GetUint(0, 0);
			msg->GiffInfo.dwUserID = pTable1.GetUint(0, 1);
			msg->GiffInfo.SendTime = pTable1.GetInt64(0, 2);
			TCHARCopy(msg->GiffInfo.NickName, CountArray(msg->GiffInfo.NickName), pTable1.GetStr(0, 3), _tcslen(pTable1.GetStr(0, 3)));
			msg->GiffInfo.dwFaceID = pTable1.GetUint(0, 4);
			OnAddDBResult(Index, ClientID, msg);
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleDelRoleGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//删除赠送
	DBR_Cmd_DelRoleGiff * pMsg = (DBR_Cmd_DelRoleGiff*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishDelGiff('%u');", pMsg->GiffID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 /*&& pTable1.GetUint(0, 0) == 1*/);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleChangeRoleCharm(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleCharm* pMsg = (DBR_Cmd_SaveRoleCharm*)pCmd;
	DBO_Cmd_SaveRoleCharm * msg = (DBO_Cmd_SaveRoleCharm*)CreateCmd(DBO_SaveRoleCharm, sizeof(DBO_Cmd_SaveRoleCharm));
	msg->dwUserID = pMsg->dwUserID;
	msg->dwDestUserID = pMsg->dwDestUserID;
	msg->bIndex = pMsg->bIndex;
	msg->ItemSum = pMsg->ItemSum;
	msg->Result = false;
	msg->dwDestUserCharmValue = 0;
	//1.先读取玩家的魅力数据
	SqlTable pTable1, pTable2;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadUserCharm('%u');", pMsg->dwDestUserID);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD			CharmArray[MAX_CHARM_ITEMSUM] = { 0 };//玩家的魅力数据
		int				CharmValue = 0;
		CharmValue = pTable1.GetInt(0, 0);
		memcpy_s(&CharmArray, sizeof(CharmArray), pTable1.GetField((UINT)0, (UINT)1), sizeof(CharmArray));
		//数据获取成功后
		CharmArray[pMsg->bIndex] += 1;
		CharmValue += pMsg->AddCharmValue;
		//保存起来  FishSaveRoleCharm
		ZeroMemory(SqlStr, sizeof(SqlStr));
		char pCharmArray[MAXBLOB_LENGTH] = { 0 };
		m_Sql[Index].GetMySqlEscapeString((char*)&CharmArray, sizeof(CharmArray), pCharmArray);
		sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleCharm('%u','%d','%s');", pMsg->dwDestUserID, CharmValue, pCharmArray);
		if (m_Sql[Index].Select(SqlStr, 1, pTable2, true) && pTable2.Rows() == 1 && pTable2.GetUint(0, 0) == 1)
		{
			msg->Result = true;
			msg->dwDestUserCharmValue = CharmValue;
			OnAddDBResult(Index, ClientID, msg);
			return true;
		}
		else
		{
			LogInfoToFile(DBErrorSqlFileName, SqlStr);
			OnAddDBResult(Index, ClientID, msg);
			ASSERT(false);
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleLoadRankInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//加载排行榜数据
	DBR_Cmd_LoadRankInfo* pMsg = (DBR_Cmd_LoadRankInfo*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishGetRankInfo('%u','%u');", pMsg->RankID,pMsg->RandRowCount);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Row = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadRankInfo)+(Row - 1)*sizeof(tagRankInfo);
		DBO_Cmd_LoadRankInfo* msg = (DBO_Cmd_LoadRankInfo*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadRankInfo);
		msg->RankID = pMsg->RankID;
		for (DWORD i = 0; i < Row; ++i)
		{
			msg->Array[i].dwUserID = pTable1.GetUint(i, 0);
			msg->Array[i].dwFaceID = pTable1.GetUint(i, 1);
			msg->Array[i].TitleID = pTable1.GetByte(i, 2);
			TCHARCopy(msg->Array[i].NickName, CountArray(msg->Array[i].NickName), pTable1.GetStr(i, 3), _tcslen(pTable1.GetStr(i, 3)));
			msg->Array[i].Gender = pTable1.GetBit(i, 4);
			msg->Array[i].Param = pTable1.GetInt64(i, 5);
		}
		std::vector<DBO_Cmd_LoadRankInfo*> pVec;
		SqlitMsg(msg, PageSize, Row, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadRankInfo*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadRankInfo* msg = (DBO_Cmd_LoadRankInfo*)CreateCmd(DBO_LoadRankInfo, sizeof(DBO_Cmd_LoadRankInfo) - sizeof(tagRankInfo));
		msg->RankID = pMsg->RankID;
		msg->Sum = 0;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleLoadWeekRankInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//从数据库加载排行榜的数据
	DBR_Cmd_LoadWeekRankInfo* pMsg = (DBR_Cmd_LoadWeekRankInfo*)pCmd;
	//玩家加载自己的周排行榜的数据
	time_t NowTime = time(NULL) - m_Config.GetWriteSec();//获取当前应该显示的时间
	tm pNowTime;
	errno_t Error = localtime_s(&pNowTime, &NowTime);
	if (Error != 0)
	{
		ASSERT(false);
		return false;
	}
	//获取 本周 或者上周的日期
	BYTE DiffDay = ConvertIntToBYTE((pNowTime.tm_wday >= m_Config.GetRankConfig().RankWeekDay) ? (pNowTime.tm_wday - m_Config.GetRankConfig().RankWeekDay) : (7 - m_Config.GetRankConfig().RankWeekDay + pNowTime.tm_wday));
	time_t WeekTime = NowTime - DiffDay * 24 * 3600;
	tm pWeekTime;
	Error = localtime_s(&pWeekTime, &WeekTime);
	if (Error != 0)
	{
		ASSERT(false);
		return false;
	}
	DWORD VersionID = (pWeekTime.tm_year + 1900) * 10000 + (pWeekTime.tm_mon + 1) * 100 + pWeekTime.tm_mday;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadWeekRankInfo('%u','%u');", VersionID,pMsg->dwUserID);
	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		//加载完全部的数据 我们进行准备 大约100条  * (7)
		//无须分包发送
		int Row = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadWeekRankInfo)+(Row - 1)*sizeof(tagRankWeekRankClient);
		DBO_Cmd_LoadWeekRankInfo* msg = (DBO_Cmd_LoadWeekRankInfo*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadWeekRankInfo);
		msg->IsInit = pMsg->IsInit;
		msg->dwUserID = pMsg->dwUserID;
		for (int i = 0; i < Row; ++i)
		{
			//读取数据
			msg->Array[i].RankID = pTable1.GetByte(i, 0);
			msg->Array[i].RankIndex = pTable1.GetByte(i, 1);
			msg->Array[i].IsReward = pTable1.GetBit(i, 2)==1?true:false;
		}

		std::vector<DBO_Cmd_LoadWeekRankInfo*> pVec;
		SqlitMsg(msg, PageSize, Row, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadWeekRankInfo*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadWeekRankInfo* msg = (DBO_Cmd_LoadWeekRankInfo*)CreateCmd(DBO_LoadWeekRankInfo, sizeof(DBO_Cmd_LoadWeekRankInfo)-sizeof(tagRankWeekRankClient));
		msg->Sum = 0;
		msg->IsInit = pMsg->IsInit;
		msg->dwUserID = pMsg->dwUserID;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleSaveWeekRankInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//保存一个上周的排行数据
	DBR_Cmd_SaveWeekRankInfo* pMsg = (DBR_Cmd_SaveWeekRankInfo*)pCmd;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	for (WORD i = 0; i < pMsg->Sum; ++i)
	{
		sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveWeekRankInfo('%u','%u','%u','%u',%u);", pMsg->Array[i].VersionID, pMsg->Array[i].dwUserID, pMsg->Array[i].RankID, pMsg->Array[i].RankIndex, pMsg->Array[i].IsReward);
		SqlTable pTable1;
		bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
		if (!Result)
		{
			LogInfoToFile(DBErrorSqlFileName, SqlStr);
			ASSERT(false);
		}
	}
	return true;
}
bool DBServer::OnHandleChangeWeekInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_ChangeWeekInfo* pMsg = (DBR_Cmd_ChangeWeekInfo*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishChangeWeekRankInfo('%u','%u','%u',%u);",pMsg->dwUserID,pMsg->RankInfo.RankID,pMsg->RankInfo.RankIndex,pMsg->RankInfo.IsReward?1:0);
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleLoadRoleGameData(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LoadGameData* pMsg = (DBR_Cmd_LoadGameData*)pCmd;
	DBO_Cmd_LoadGameData * msg = (DBO_Cmd_LoadGameData*)CreateCmd(DBO_LoadGameData, sizeof(DBO_Cmd_LoadGameData));
	msg->dwUserID = pMsg->dwUserID;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadRoleGameData('%u');", pMsg->dwUserID);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		if (pTable1.Rows() == 1)
		{
			msg->GameData.RoleMonthRewardSum = pTable1.GetUint(0, 0);
			msg->GameData.RoleMonthFirstSum = pTable1.GetUint(0, 1);
			msg->GameData.RoleMonthSecondSum = pTable1.GetUint(0, 2);
			msg->GameData.RoleMonthThreeSum = pTable1.GetUint(0, 3);
			msg->GameData.RoleCatchFishTotalSum = pTable1.GetUint(0, 4);
			msg->GameData.RoleGetGlobelSum = pTable1.GetUint(0, 5);
			msg->GameData.RoleMonthSigupSum = pTable1.GetUint(0, 6);
			msg->GameData.NonMonthGameSec = pTable1.GetUint(0, 7);
			msg->GameData.TotalGameSec = pTable1.GetUint(0, 8);
			msg->GameData.CatchFishOneSum = pTable1.GetUint(0, 9);
			msg->GameData.CatchFishTwoSum = pTable1.GetUint(0, 10);
			msg->GameData.CatchFishThreeSum = pTable1.GetUint(0, 11);
			OnAddDBResult(Index, ClientID, msg);
		}
		else
		{
			msg->GameData.RoleMonthRewardSum = 0;
			msg->GameData.RoleMonthFirstSum = 0;
			msg->GameData.RoleMonthSecondSum = 0;
			msg->GameData.RoleMonthThreeSum = 0;
			msg->GameData.RoleCatchFishTotalSum = 0;
			msg->GameData.RoleGetGlobelSum = 0;
			msg->GameData.RoleMonthSigupSum = 0;
			msg->GameData.NonMonthGameSec = 0;
			msg->GameData.TotalGameSec = 0;
			msg->GameData.CatchFishOneSum = 0;
			msg->GameData.CatchFishTwoSum = 0;
			msg->GameData.CatchFishThreeSum = 0;
			OnAddDBResult(Index, ClientID, msg);
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
	}
	return true;

}
bool DBServer::OnHandleSaveRoleGameData(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveGameData* pMsg = (DBR_Cmd_SaveGameData*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleGameData('%u','%u','%u','%u','%u','%u','%u','%u','%u','%u','%u','%u','%u');", pMsg->dwUserID, pMsg->GameData.RoleMonthRewardSum, pMsg->GameData.RoleMonthFirstSum, pMsg->GameData.RoleMonthSecondSum, pMsg->GameData.RoleMonthThreeSum, pMsg->GameData.RoleCatchFishTotalSum, pMsg->GameData.RoleGetGlobelSum
		, pMsg->GameData.RoleMonthSigupSum, pMsg->GameData.NonMonthGameSec, pMsg->GameData.TotalGameSec, pMsg->GameData.CatchFishOneSum, pMsg->GameData.CatchFishTwoSum, pMsg->GameData.CatchFishThreeSum);
	
	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleChangeRoleGameDataByMonth(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_ChangeGameDataByMonth* pMsg = (DBR_Cmd_ChangeGameDataByMonth*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddRoleGameDataMonthReward('%u','%u');", pMsg->dwUserID, pMsg->MonthIndex);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleAchievementPointList(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LoadAllUserAchievementPointList* pMsg = (DBR_Cmd_LoadAllUserAchievementPointList*)pCmd;
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAchievementPointList('%u');", pMsg->RankSum);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Rows = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadAllUserAchievementPointList)+(pTable1.Rows() - 1)* sizeof(DWORD);
		DBO_Cmd_LoadAllUserAchievementPointList * msg = (DBO_Cmd_LoadAllUserAchievementPointList*)malloc(PageSize);
		msg->SetCmdType(DBO_LoadAllUserAchievementPointList);
		for (DWORD i = 0; i < Rows; ++i)
		{
			msg->Array[i] = pTable1.GetUint(i, 0);
		}

		std::vector<DBO_Cmd_LoadAllUserAchievementPointList*> pVec;
		SqlitMsg(msg, PageSize, Rows, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadAllUserAchievementPointList*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadAllUserAchievementPointList* msg = (DBO_Cmd_LoadAllUserAchievementPointList*)CreateCmd(DBO_LoadAllUserAchievementPointList, sizeof(DBO_Cmd_LoadAllUserAchievementPointList)-sizeof(DWORD));
		msg->Sum = 0;
		msg->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msg);
		ASSERT(false);
		return false;
	}
	return true;
}
bool DBServer::OnHandleLoadAllAccountInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//读取 FishLogonCache
	DBR_Cmd_LoadAllAccountInfo* pMsg = (DBR_Cmd_LoadAllAccountInfo*)pCmd;
	//加载玩家的关系 分2步  加载玩家主动关系 和 玩家的被动关系
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	string Sql = "call FishLogonCache();";
	strncpy_s(SqlStr, sizeof(SqlStr), Sql.c_str(), Sql.length());

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Rows = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadAllAccountInfo)+(Rows - 1)*sizeof(AccountCacheInfo);
		//玩家物品过多 我们分包发送
		DBO_Cmd_LoadAllAccountInfo * msg = (DBO_Cmd_LoadAllAccountInfo*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return false;
		}
		msg->SetCmdType(DBO_LoadAllAcoountInfo);
		for (DWORD i = 0; i < Rows;++i)
		{
			TCHARCopy(msg->Array[i].AccountName, CountArray(msg->Array[i].AccountName), pTable1.GetStr(i, 0), _tcslen(pTable1.GetStr(i, 0)));
			msg->Array[i].dwUserID = pTable1.GetUint(i, 1);
			msg->Array[i].PasswordCrc1 = pTable1.GetUint(i, 2);
			msg->Array[i].PasswordCrc2 = pTable1.GetUint(i, 3);
			msg->Array[i].PasswordCrc3 = pTable1.GetUint(i, 4);
			msg->Array[i].IsFreeze = pTable1.GetBit(i, 5) == 1 ? true : false;
			msg->Array[i].FreezeEndTime = pTable1.GetDateTime(i, 6);
			TCHARCopy(msg->Array[i].MacAddress, CountArray(msg->Array[i].MacAddress), pTable1.GetStr(i, 7), _tcslen(pTable1.GetStr(i, 7)));
		}
		std::vector<DBO_Cmd_LoadAllAccountInfo*> pVec;
		SqlitMsg(msg, PageSize, Rows, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadAllAccountInfo*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadAllAccountInfo* msgFinish = (DBO_Cmd_LoadAllAccountInfo*)CreateCmd(DBO_LoadAllAcoountInfo, sizeof(DBO_Cmd_LoadAllAccountInfo)-sizeof(AccountCacheInfo));
		msgFinish->States = (MsgBegin | MsgEnd);
		msgFinish->Sum = 0;
		OnAddDBResult(Index, ClientID, msgFinish);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleLogRechargeInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LogRecharge* pMsg = (DBR_Cmd_LogRecharge*)pCmd;

	//玩家进行充值的LOG记录 
	//将玩家的数据记录下来
	if (pMsg->rechargeInfo.Sum != 0)
	{
		//将命令里的数据切割出来
		vector<StringArrayData*> pVec;
		GetStringArrayVecByData(pVec, &pMsg->rechargeInfo);
		if (pVec.size() != pMsg->rechargeInfo.HandleSum)
		{
			ASSERT(false);
			return false;
		}

		UINT Size = 0;
		char* Info1 = WCharToChar(pVec[0]->Array, pVec[0]->Length / sizeof(TCHAR), Size);
		char* DestInfo1 = (char*)malloc((Size * 2 + 1)*sizeof(char));
		m_Sql[Index].GetMySqlEscapeString(Info1, Size, DestInfo1);

		char* Info2 = WCharToChar(pVec[1]->Array, pVec[1]->Length / sizeof(TCHAR), Size);
		char* DestInfo2 = (char*)malloc((Size * 2 + 1)*sizeof(char));
		m_Sql[Index].GetMySqlEscapeString(Info2, Size, DestInfo2);

		char* Info3 = WCharToChar(pVec[2]->Array, pVec[2]->Length / sizeof(TCHAR), Size);
		char* DestInfo3 = (char*)malloc((Size * 2 + 1)*sizeof(char));
		m_Sql[Index].GetMySqlEscapeString(Info3, Size, DestInfo3);

		char* Info4 = WCharToChar(pVec[3]->Array, pVec[3]->Length / sizeof(TCHAR), Size);
		char* DestInfo4 = (char*)malloc((Size * 2 + 1)*sizeof(char));
		m_Sql[Index].GetMySqlEscapeString(Info4, Size, DestInfo4);

		/*vector<TCHAR*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
		free(*Iter);
		}
		pVec.clear();*/

		SYSTEMTIME pTime;
		GetLocalTime(&pTime);

		SqlTable pTable1;
		char SqlStr[MAXSQL_LENGTH] = { 0 };
		sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddRechargeLog('%u','%u','%s','%s','%s','%s','%u','%u',,'%u','%u''%d-%d-%d %d:%d:%d');", pMsg->rechargeInfo.UserID, pMsg->rechargeInfo.Price, DestInfo1, DestInfo2,
			DestInfo3, DestInfo4, pMsg->AddGlobelSum, pMsg->AddCurrceySum, pMsg->OldGlobelSum, pMsg->OldCurrceySum,
			pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

		free(Info1);
		free(DestInfo1);

		free(Info2);
		free(DestInfo2);

		free(Info3);
		free(DestInfo3);

		free(Info4);
		free(DestInfo4);

		bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
		if (!Result)
		{
			LogInfoToFile(DBErrorSqlFileName, SqlStr);
			ASSERT(false);
		}
		return true;
	}
	else
	{
		//为0的情况下 
		SYSTEMTIME pTime;
		GetLocalTime(&pTime);

		SqlTable pTable1;
		char SqlStr[MAXSQL_LENGTH] = { 0 };
		sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddRechargeLog('%u','%u','','','','','%u','%u','%d-%d-%d %d:%d:%d');", pMsg->rechargeInfo.UserID, pMsg->rechargeInfo.Price,pMsg->AddGlobelSum, pMsg->AddCurrceySum,
			pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

		bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
		if (!Result)
		{
			LogInfoToFile(DBErrorSqlFileName, SqlStr);
			ASSERT(false);
		}
		return true;
	}
}
bool DBServer::OnHandleLogRoleOnlineInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//处理LOG记录
	DBR_Cmd_LogOnlineInfo* pMsg = (DBR_Cmd_LogOnlineInfo*)pCmd;
	//执行存储过程
	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	//保存玩家登陆的数据
	//玩家上线 或者离线的数据
	//FishAddRoleOnlineLog
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddRoleOnlineLog('%u',%d,'%d-%d-%d %d:%d:%d');", pMsg->dwUserID,pMsg->IsOnline?1:0,pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleLogPhonePayInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LogPhonePay* pMsg = (DBR_Cmd_LogPhonePay*)pCmd;
	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddPhonePayLog('%u','%llu','%u','%d-%d-%d %d:%d:%d');", pMsg->dwUserID, pMsg->PhoneNumber, pMsg->PhoneMoney, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnAddRecharge(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//玩家不在线 进行操作 我们进行记录下
	DBR_Cmd_AddRecharge* pMsg = (DBR_Cmd_AddRecharge*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	vector<StringArrayData*> pVec;
	GetStringArrayVecByData(pVec, &pMsg->rechargeInfo);
	if (pVec.size() != pMsg->rechargeInfo.HandleSum)
	{
		ASSERT(false);
		return false;
	}

	UINT Size = 0;
	char* Info1 = WCharToChar(pVec[0]->Array,pVec[0]->Length/sizeof(TCHAR), Size);
	char* DestInfo1 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info1, Size, DestInfo1);

	char* Info2 = WCharToChar(pVec[1]->Array, pVec[1]->Length / sizeof(TCHAR), Size);
	char* DestInfo2 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info2, Size, DestInfo2);

	char* Info3 = WCharToChar(pVec[2]->Array, pVec[2]->Length / sizeof(TCHAR), Size);
	char* DestInfo3 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info3, Size, DestInfo3);

	char* Info4 = WCharToChar(pVec[3]->Array, pVec[3]->Length / sizeof(TCHAR), Size);
	char* DestInfo4 = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info4, Size, DestInfo4);

	/*vector<TCHAR*>::iterator Iter = pVec.begin();
	for (; Iter != pVec.end(); ++Iter)
	{
		free(*Iter);
	}
	pVec.clear();*/

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddRecharge('%u','%u',%d,'%s','%s','%s','%s','%u','%u','%d-%d-%d %d:%d:%d');", pMsg->rechargeInfo.UserID, pMsg->rechargeInfo.Price,pMsg->IsFirst?1:0 ,DestInfo1, DestInfo2,
		DestInfo3, DestInfo4, pMsg->AddGlobelSum, pMsg->AddCurrceySum,
		pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	free(Info1);
	free(DestInfo1);

	free(Info2);
	free(DestInfo2);

	free(Info3);
	free(DestInfo3);

	free(Info4);
	free(DestInfo4);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnLoadPhonePayOrderList(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_LoadPhonePayOrderID* pMsg = (DBR_Cmd_LoadPhonePayOrderID*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	//
	BYTE DiffDay = pMsg->DiffDay;

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadPhonePayOrderId(%d,'%d-%d-%d %d:%d:%d');", pMsg->DiffDay, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Rows = pTable1.Rows();

		DWORD PageSize = sizeof(DBO_Cmd_LoadPhonePayOrderID)+(Rows - 1)*sizeof(unsigned __int64);
		//玩家物品过多 我们分包发送
		DBO_Cmd_LoadPhonePayOrderID * msg = (DBO_Cmd_LoadPhonePayOrderID*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return false;
		}
		msg->SetCmdType(DBO_LoadPhonePayOrderID);
		for (DWORD i = 0; i < Rows; ++i)
		{
			msg->Array[i] = pTable1.GetUint64(i, 0);
		}
		std::vector<DBO_Cmd_LoadPhonePayOrderID*> pVec;
		SqlitMsg(msg, PageSize, Rows, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadPhonePayOrderID*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadPhonePayOrderID* msgFinish = (DBO_Cmd_LoadPhonePayOrderID*)CreateCmd(DBO_LoadPhonePayOrderID, sizeof(DBO_Cmd_LoadPhonePayOrderID)-sizeof(__int64));
		msgFinish->States = (MsgBegin | MsgEnd);
		msgFinish->Sum = 0;
		OnAddDBResult(Index, ClientID, msgFinish);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnLoadRechargeOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//加载全部的订单号 到内存中去
	DBR_Cmd_LoadRechageOrderID* pMsg = (DBR_Cmd_LoadRechageOrderID*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	//
	BYTE DiffDay = pMsg->DiffDay;

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadRechargeOrderId(%d,'%d-%d-%d %d:%d:%d');", pMsg->DiffDay,pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	if (m_Sql[Index].Select(SqlStr, 0, pTable1, true))
	{
		DWORD Rows = pTable1.Rows();

		DWORD PageSize = sizeof(DBO_Cmd_LoadRechageOrderID)+(Rows - 1)*sizeof(unsigned __int64);
		//玩家物品过多 我们分包发送
		DBO_Cmd_LoadRechageOrderID * msg = (DBO_Cmd_LoadRechageOrderID*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return false;
		}
		msg->SetCmdType(DBO_LoadRechageOrderID);
		for (DWORD i = 0; i < Rows; ++i)
		{
			msg->Array[i] = pTable1.GetUint64(i, 0);
		}
		std::vector<DBO_Cmd_LoadRechageOrderID*> pVec;
		SqlitMsg(msg, PageSize, Rows, false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadRechageOrderID*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadRechageOrderID* msgFinish = (DBO_Cmd_LoadRechageOrderID*)CreateCmd(DBO_LoadRechageOrderID, sizeof(DBO_Cmd_LoadRechageOrderID)-sizeof(__int64));
		msgFinish->States = (MsgBegin | MsgEnd);
		msgFinish->Sum = 0;
		OnAddDBResult(Index, ClientID, msgFinish);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnAddRechargeOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//新处理了一个订单  记录到数据库去
	DBR_Cmd_AddRechageOrderID* pMsg = (DBR_Cmd_AddRechageOrderID*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddRechargeOrderID('%u','%llu','%d-%d-%d %d:%d:%d');", pMsg->dwUserID,pMsg->OrderOnlyID, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnAddPhonePayOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_AddPhonePayOrderID* pMsg = (DBR_Cmd_AddPhonePayOrderID*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}
	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddPhonePayOrderID('%u','%llu','%d-%d-%d %d:%d:%d');", pMsg->dwUserID, pMsg->OrderOnlyID, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnCheckPhone(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//检查手机是否可以使用
	DBR_Cmd_CheckPhone* pMsg = (DBR_Cmd_CheckPhone*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishCheckPhone('%llu');", pMsg->Phone);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 && pTable1.GetUint(0, 0) == 0);

	DBO_Cmd_CheckPhone* msg = (DBO_Cmd_CheckPhone*)CreateCmd(DBO_CheckPhone, sizeof(DBO_Cmd_CheckPhone));
	msg->dwUserID = pMsg->dwUserID;
	msg->Result = Result;
	msg->Phone = pMsg->Phone;
	msg->ClientID = pMsg->ClientID;
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnCheckEntityID(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_CheckEntityID* pMsg = (DBR_Cmd_CheckEntityID*)pCmd;
	if (!pMsg)
	{
		ASSERT(false);
		return false;
	}

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishCheckEntityID('%s');", pMsg->EntityID);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 && pTable1.GetUint(0, 0) == 0);
	DBO_Cmd_CheckEntityID* msg = (DBO_Cmd_CheckEntityID*)CreateCmd(DBO_CheckEntityID, sizeof(DBO_Cmd_CheckEntityID));
	msg->dwUserID = pMsg->dwUserID;
	msg->EntityCrcID = pMsg->EntityCrcID;
	msg->Result = Result;
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleLogInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//将发送来的Log数据记录下来  FishLogInfo
	DBR_Cmd_LogInfo* pMsg = (DBR_Cmd_LogInfo*)pCmd;

	UINT Size = 0;
	char* Info = WCharToChar(pMsg->Info, Size);
	char* DestInfo = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info, Size, DestInfo);

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddLogInfo('%u','%u','%d','%s','%d-%d-%d %d:%d:%d');", pMsg->UserID, pMsg->Type, pMsg->TypeSum, DestInfo, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
	free(Info);
	free(DestInfo);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1 );
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
//Announcement
bool DBServer::OnHandleLoadAllAnnouncementInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//加载全部的公告
	//分包发送
	//执行新的存储过程  被动关系的数据 无法限制数量 我们想要进行分包发送 控制好流程
	DBR_Cmd_LoadAllAnnouncement* pMsg = (DBR_Cmd_LoadAllAnnouncement*)pCmd;

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishLoadAllAnnouncementInfo('%u');", m_Config.GetSystemConfig().AnnouncementSum);

	if (m_Sql[Index].Select(SqlStr, 1, pTable1, true))
	{
		DWORD Rows = pTable1.Rows();
		DWORD PageSize = sizeof(DBO_Cmd_LoadALlAnnouncement)+(Rows - 1)*sizeof(AnnouncementOnce);
		DBO_Cmd_LoadALlAnnouncement* msgBe = (DBO_Cmd_LoadALlAnnouncement*)malloc(PageSize);
		if (!msgBe)
		{
			ASSERT(false);
			return false;
		}
		msgBe->SetCmdType(DBO_LoadALlAnnouncement);
		for (DWORD i = 0; i < Rows; ++i)
		{
			TCHARCopy(msgBe->Array[i].NickName, CountArray(msgBe->Array[i].NickName), pTable1.GetStr(i, 0), _tcslen(pTable1.GetStr(i, 0)));
			msgBe->Array[i].ShopID = pTable1.GetByte(i, 1);
			msgBe->Array[i].ShopOnlyID = pTable1.GetByte(i, 2);
		}
		std::vector<DBO_Cmd_LoadALlAnnouncement*> pVec;
		SqlitMsg(msgBe, PageSize, Rows, false, pVec);
		free(msgBe);
		if (!pVec.empty())
		{
			std::vector<DBO_Cmd_LoadALlAnnouncement*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				OnAddDBResult(Index, ClientID, *Iter);
			}
			pVec.clear();
		}
	}
	else
	{
		
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		DBO_Cmd_LoadALlAnnouncement* msgBeFinish = (DBO_Cmd_LoadALlAnnouncement*)CreateCmd(DBO_LoadALlAnnouncement, sizeof(DBO_Cmd_LoadALlAnnouncement)-sizeof(AnnouncementOnce));
		msgBeFinish->Sum = 0;
		msgBeFinish->States = (MsgBegin | MsgEnd);
		OnAddDBResult(Index, ClientID, msgBeFinish);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleAddAnnouncementInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_AddAnnouncement* pMsg = (DBR_Cmd_AddAnnouncement*)pCmd;

	UINT Size = 0;
	char* Info = WCharToChar(pMsg->AnnouncementInfo.NickName, Size);
	char* DestInfo = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info, Size, DestInfo);

	SYSTEMTIME pTime;
	GetLocalTime(&pTime);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishAddAnnouncementInfo('%s','%u','%u','%d-%d-%d %d:%d:%d');", DestInfo, pMsg->AnnouncementInfo.ShopID, pMsg->AnnouncementInfo.ShopOnlyID, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
	free(Info);
	free(DestInfo);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleChangeRoleEmail(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	DBR_Cmd_SaveRoleEmail* pMsg = (DBR_Cmd_SaveRoleEmail*)pCmd;

	UINT Size = 0;
	char* Info = WCharToChar(pMsg->Email, Size);
	char* DestInfo = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info, Size, DestInfo);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishSaveRoleEmail('%u','%s');",pMsg->dwUserID, DestInfo);
	free(Info);
	free(DestInfo);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
bool DBServer::OnHandleClearFishDB(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//在中央服务器启动的时候 清理下数据库
	DBR_Cmd_ClearFishDB* pMsg = (DBR_Cmd_ClearFishDB*)pCmd;
	SYSTEMTIME pTime;
	GetLocalTime(&pTime);
	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishServerInit(%d,'%u','%u','%u','%d-%d-%d %d:%d:%d');", pMsg->IsInit ? 1 : 0, 
		m_Config.GetSystemConfig().AnnouncementSum, m_Config.GetGiffConfig().LimitDay, m_Config.GetFishMailConfig().MailLimitDay, 
		pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
	bool Result = m_Sql[Index].Select(SqlStr, 0, pTable1, true);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	DBO_Cmd_ClearFishDB* msg = (DBO_Cmd_ClearFishDB*)CreateCmd(DBO_ClearFishDB, sizeof(DBO_Cmd_ClearFishDB));
	OnAddDBResult(Index, ClientID, msg);
	return true;
}
bool DBServer::OnHandleQueryExChange(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//查询一个兑换码是否存在 是否可以使用
	DBR_Cmd_QueryExChange* pMsg = (DBR_Cmd_QueryExChange*)pCmd;
	DBO_Cmd_QueryExChange* msg = (DBO_Cmd_QueryExChange*)CreateCmd(DBO_QueryExChange, sizeof(DBO_Cmd_QueryExChange));
	msg->dwUserID = pMsg->dwUserID;
	TCHARCopy(msg->ExChangeCode, CountArray(msg->ExChangeCode), pMsg->ExChangeCode, _tcslen(pMsg->ExChangeCode));
	msg->ID = 0;

	UINT Size = 0;
	char* Info = WCharToChar(pMsg->ExChangeCode, Size);
	char* DestInfo = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info, Size, DestInfo);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishQueryExChangeCode('%s');", DestInfo);
	free(Info);
	free(DestInfo);

	if (m_Sql[Index].Select(SqlStr, 1, pTable1, true))
	{
		DWORD Rows = pTable1.Rows();
		if (Rows == 1)
		{
			msg->ID = pTable1.GetByte(0, 0);
			OnAddDBResult(Index, ClientID, msg);
		}
		else
		{
			msg->ID = 0;
			OnAddDBResult(Index, ClientID, msg);
		}
	}
	else
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
		OnAddDBResult(Index, ClientID, msg);
	}
	return true;
}
bool DBServer::OnDelExChange(BYTE Index, BYTE ClientID, NetCmd* pCmd)
{
	//移除一个已经使用了的兑换码
	DBR_Cmd_DelExChange* pMsg = (DBR_Cmd_DelExChange*)pCmd;

	UINT Size = 0;
	char* Info = WCharToChar(pMsg->ExChangeCode, Size);
	char* DestInfo = (char*)malloc((Size * 2 + 1)*sizeof(char));
	m_Sql[Index].GetMySqlEscapeString(Info, Size, DestInfo);

	SqlTable pTable1;
	char SqlStr[MAXSQL_LENGTH] = { 0 };
	sprintf_s(SqlStr, sizeof(SqlStr), "call FishDelExChangeCode('%s');", DestInfo);
	free(Info);
	free(DestInfo);

	bool Result = (m_Sql[Index].Select(SqlStr, 0, pTable1, true) && pTable1.Rows() == 1);
	if (!Result)
	{
		LogInfoToFile(DBErrorSqlFileName, SqlStr);
		ASSERT(false);
	}
	return true;
}
void DBServer::OnAddDBResult(BYTE Index, BYTE ClientID, NetCmd*pCmd)
{
	if (Index >= DB_ThreadSum || !pCmd)
	{
		if (pCmd)
			free(pCmd);
		return;
	}
	RcveMsg* pRecvInfo = new RcveMsg;
	pRecvInfo->ClientID = ClientID;
	pRecvInfo->pCmd = pCmd;
	m_SendMsgList[Index].AddItem(pRecvInfo);
}
void DBServer::OnHandleAllMsg()
{
	//数据库在退出之前 先处理完全部的数据库命令 在退出
	for (BYTE i = 0; i < DB_ThreadSum; ++i)
	{
		RcveMsg *pRecvInfo = m_RecvMsgList[i].GetItem();
		while (pRecvInfo)
		{
			HandleDataBaseMsg(i, pRecvInfo->ClientID, pRecvInfo->pCmd);
			free(pRecvInfo->pCmd);
			delete pRecvInfo;
			pRecvInfo = m_RecvMsgList[i].GetItem();
		}
	}
}


void DBServer::SendNetCmdToControl(NetCmd*pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	m_ControlTcp.Send(pCmd, false);
}
bool DBServer::ConnectControl()
{
	//对控制方开启的端口 允许外部链接到中央服务器来进行登陆处理 携带DWORD 的唯一ID
	ControlServerConfig* pConfig = g_FishServerConfig.GetControlServerConfig();
	if (!pConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pControlData;
	pControlData.BuffSize = pConfig->ControlServerBuffSize;
	pControlData.Port = pConfig->ControlServerListenPort;
	pControlData.SleepTime = pConfig->ControlServerSleepTime;
	pControlData.SocketRecvSize = pConfig->ControlServerRecvBuffSize;
	pControlData.SocketSendSize = pConfig->ControlServerSendBuffSize;
	pControlData.ThreadNum = 1;
	pControlData.Timeout = pConfig->ControlServerTimeOut;
	pControlData.CmdHearbeat = 0;
	pControlData.SendArraySize = pConfig->ControlServerMaxSendCmdCount;
	pControlData.RecvArraySize = pConfig->ControlServerMaxRecvCmdCount;

	if (!m_ControlTcp.Init(pConfig->ControlServerListenIP, pControlData, (void*)&m_DBNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("ControlServer连接失败");
		return false;
	}
	m_ControlIsConnect = true;
	return true;
}