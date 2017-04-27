#include "StdAfx.h"
#include "RoleEx.h"
#include "Role.h"
#include "RoleManager.h"
#include "FishServer.h"
#include "..\CommonFile\DBLogManager.h"
#include "FishLogic\NetCmd.h"
extern void SendLogDB(NetCmd* pCmd);
CRoleEx::CRoleEx()
{
	m_IsNeedSave = false;
	m_IsChangeClientIP = false;
	m_LogonTimeByDay = 0;
	m_LogobByGameServer = false;
	m_ChannelID = 0;//渠道ID 默认为0

	m_IsRobot = false;//是否为机器人
	m_IsAfk = false;//是否离线
	m_IsExit = false;
	m_IsOnline = false;//是否在线
}
CRoleEx::~CRoleEx()
{
	//WORD OnLineMin = static_cast<WORD>(GetRoleOnlineSec() / 60);//获取玩家在线的分钟
	//if (m_RoleInfo.OnlineMin != OnLineMin)
	//{
	//	m_RoleInfo.OnlineMin = OnLineMin;
	//	m_IsNeedSave = true;
	//}
	//SaveRoleExInfo();//玩家离线的时候 如果需要保持数据 就保存次
	if (!m_ChannelUserInfo.empty())
	{
		std::vector<TCHAR*>::iterator Iter = m_ChannelUserInfo.begin();
		for (; Iter != m_ChannelUserInfo.end(); ++Iter)
		{
			free(*Iter);
		}
		m_ChannelUserInfo.clear();
	}

	m_IsAfk = false;
	m_IsExit = false;
	m_IsOnline = false;
}

DWORD CRoleEx::GetOperatorChannelID()
{
	return m_RoleInfo.ChannelID;
}
bool CRoleEx::OnInit(tagRoleInfo* pUserInfo, tagRoleServerInfo* pRoleServerInfo, RoleManager* pManager, DWORD dwSocketID, time_t pTime, bool LogobByGameServer, bool IsRobot)//玩家登陆成功的时候 dwSocketID 对应的Gate的ID
{
	if (!pUserInfo || !pRoleServerInfo)
	{
		ASSERT(false);
		return false;
	}
	ServerClientData * pClient = g_FishServer.GetUserClientDataByIndex(dwSocketID);
	if (dwSocketID != 0 && !pClient)
	{
		ASSERT(false);
		return false;
	}
	m_IsRobot = IsRobot;
	m_dwGameSocketID = dwSocketID;
	m_RoleInfo = *pUserInfo;
	m_RoleServerInfo = *pRoleServerInfo;

	{
		m_RoleInfo.benefitCount = m_RoleServerInfo.RoleProtectSum;
		m_RoleInfo.benefitTime = (DWORD)m_RoleServerInfo.RoleProtectLogTime;
	}

	m_RoleInfo.AchievementPointIndex = g_FishServer.GetAchievementIndex(m_RoleInfo.dwUserID);//设置玩家的排名

	if (pClient && m_RoleInfo.ClientIP != pClient->IP)
	{
		m_RoleInfo.ClientIP = pClient->IP;//玩家上线成功获取玩家的IP地址

		//玩家的IP地址发生变化了 我们需要进行处理 玩家切换IP登陆了
		m_IsNeedSave = true;
		m_IsChangeClientIP = true;
	}
	else
	{
		m_IsChangeClientIP = false;
	}

	g_FishServer.GetAddressByIP(m_RoleInfo.ClientIP, m_RoleInfo.IPAddress, CountArray(m_RoleInfo.IPAddress));//设置玩家的IP位置

	m_RoleManager = pManager;
	m_LastOnLineTime = pTime;
	m_LogonTime = time(NULL);
	m_LogonTimeByDay = m_LogonTime;
	if (!IsOnceDayOnline())
	{
		//是否为同一天登陆 每日数据清空
		m_RoleInfo.dwProduction = 0;
		m_RoleInfo.dwGameTime = 0;
		m_RoleInfo.OnlineMin = 0;
		ResetPerDay();

		ChangeRoleSendGiffSum(m_RoleInfo.SendGiffSum * -1);
		ChangeRoleCashSum(m_RoleInfo.CashSum * -1);
		ChangeRoleAcceptGiffSum(m_RoleInfo.AcceptGiffSum * -1);
		ClearRoleTaskStates();
		ChangeRoleOnlineRewardStates(0);
		m_IsNeedSave = true;
		if (!IsOnceMonthOnline())
		{
			ChangeRoleCheckData(0);//即可保存
		}

	}
	m_LogobByGameServer = LogobByGameServer;

	ChannelUserInfo* pInfo = g_FishServer.GetChannelUserInfo(GetUserID());
	if (pInfo)
	{
		//保存一下玩家的渠道数据
		DWORD PageSize = sizeof(DBR_Cmd_SaveChannelInfo)+sizeof(BYTE)*(pInfo->Sum - 1);
		DWORD InfoSize = sizeof(ChannelUserInfo)+sizeof(BYTE)*(pInfo->Sum - 1);
		DBR_Cmd_SaveChannelInfo* msgDB = (DBR_Cmd_SaveChannelInfo*)malloc(PageSize);
		if (!msgDB)
		{
			ASSERT(false);
			return false;
		}
		msgDB->SetCmdType(DBR_SaveChannelInfo);
		msgDB->SetCmdSize(static_cast<WORD>(PageSize));
		msgDB->dwUserID = GetUserID();
		memcpy_s(&msgDB->pInfo, InfoSize, pInfo, InfoSize);
		g_FishServer.SendNetCmdToSaveDB(msgDB);
		free(msgDB);

		GetStringArrayVecByData(m_ChannelUserInfo, pInfo);
		if (m_ChannelUserInfo.size() != pInfo->HandleSum)
		{
			ASSERT(false);
			m_ChannelUserInfo.clear();
			m_ChannelID = 0;
		}
		//获取渠道名称
		m_ChannelID = GetCrc32(m_ChannelUserInfo[1]);

		g_FishServer.OnDelChannelInfo(GetUserID());//将玩家的渠道数据取走
	}
	else
	{
		m_ChannelUserInfo.clear();
		m_ChannelID = 0;
	}

	string MacAddress = g_FishServer.GetUserMacAddress(GetUserID());
	string IPAddress = g_FishServer.GetUserIpAddress(GetUserID());
	g_DBLogManager.LogRoleOnlineInfo(m_RoleInfo.dwUserID, true, MacAddress, IPAddress, m_RoleInfo.dwGlobeNum, m_RoleInfo.dwCurrencyNum, m_RoleInfo.dwMedalNum, SendLogDB);

	return m_RelationManager.OnInit(this, pManager) && m_ItemManager.OnInit(this, pManager) && m_MailManager.OnInit(this, pManager) /*&& m_RoleFtpFaceManager.OnInit(this)*/
		&& m_RoleCheck.OnInit(this) && m_RoleTask.OnInit(this) && m_RoleAchievement.OnInit(this) && m_RoleMonth.OnInit(this) && m_RoleTitleManager.OnInit(this) &&
		m_RoleIDEntity.OnInit(this) && m_RoleActionManager.OnInit(this) && m_RoleGiffManager.OnInit(this) && m_RoleFtpManager.OnInit(this) && m_RoleGameData.OnInit(this) && m_RoleRank.OnInit(this)
		&& m_RoleProtect.OnInit(this) && m_RoleVip.OnInit(this) && m_RoleMonthCard.OnInit(this) && m_RoleRate.OnInit(this) && m_RoleCharManger.OnInit(this) && m_RoleRelationRequest.OnInit(this);
}
bool CRoleEx::IsLoadFinish()
{
	if (
		m_RelationManager.IsLoadDB() &&
		m_ItemManager.IsLoadDB() &&
		m_MailManager.IsLoadDB() &&
		m_RoleTask.IsLoadDB() &&
		m_RoleAchievement.IsLoadDB() &&
		m_RoleTitleManager.IsLoadDB() &&
		m_RoleIDEntity.IsLoadDB() &&
		m_RoleActionManager.IsLoadDB() &&
		m_RoleGiffManager.IsLoadDB() &&
		m_RoleGameData.IsLoadDB() &&
		m_RoleRank.IsLoadDB() &&
		m_RoleCharManger.IsLoadDB() && 
		m_RoleRelationRequest.IsLoadDB()
		)
	{
		//这些特殊的数据 无须玩家上线设置 每个事件接取的时候都已经设置了
		UpdateRoleEvent();

		SendUserInfoToCenter();//将玩家数据发送到中央服务器去

		OnHandleRoleVersionChange();

		OnUserLoadFinish(m_LogobByGameServer);

		m_IsOnline = true;

		
		return true;
	}
	else
	{
		return false;
	}
}
void CRoleEx::OnHandleRoleVersionChange()
{
	//因为版本做的一些事情
	//1302 1303 1304
     //10   100  300
	DWORD ChangeRate = 1;
	if (m_RoleInfo.TotalRechargeSum >= 2)
		ChangeRate = 2;

	DWORD Sum = GetItemManager().QueryItemAllTimeCount(1302);
	if (Sum > 0)
	{
		//玩家拥有3号炮 
		DWORD AddCurrcey = Sum * 10 * ChangeRate;
		if (GetItemManager().OnDelUserItem(1302, GetItemManager().QueryItemCount(1302)))
		{
			//发送邮件
			tagRoleMail	MailInfo;
			MailInfo.bIsRead = false;
			if (ChangeRate == 2)
				_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("{ItemName:ItemID=%u}炮 获取机制已经被修改 特退还您购买炮的双倍补偿款"),1302);
			else
				_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("{ItemName:ItemID=%u}炮 获取机制已经被修改 特退还您购买炮的补偿款"), 1302);
			MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailCurrceyRewardID;
			MailInfo.RewardSum = AddCurrcey;
			MailInfo.MailID = 0;
			MailInfo.SendTimeLog = time(NULL);
			MailInfo.SrcFaceID = 0;
			TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
			MailInfo.SrcUserID = 0;//系统发送
			MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
			DBR_Cmd_AddUserMail msg;
			SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
			msg.dwDestUserID = GetUserID();
			msg.MailInfo = MailInfo;
			g_FishServer.SendNetCmdToDB(&msg);
		}
	}

	Sum = GetItemManager().QueryItemAllTimeCount(1303);
	if (Sum > 0)
	{
		//玩家拥有3号炮 
		DWORD AddCurrcey = Sum * 100 * ChangeRate;
		if (GetItemManager().OnDelUserItem(1303, GetItemManager().QueryItemCount(1303)))
		{
			//发送邮件
			tagRoleMail	MailInfo;
			MailInfo.bIsRead = false;
			if (ChangeRate == 2)
				_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("{ItemName:ItemID=%u}炮 获取机制已经被修改 特退还您购买炮的双倍补偿款"), 1303);
			else
				_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("{ItemName:ItemID=%u}炮 获取机制已经被修改 特退还您购买炮的补偿款"), 1303);
			MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailCurrceyRewardID;
			MailInfo.RewardSum = AddCurrcey;
			MailInfo.MailID = 0;
			MailInfo.SendTimeLog = time(NULL);
			MailInfo.SrcFaceID = 0;
			TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
			MailInfo.SrcUserID = 0;//系统发送
			MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
			DBR_Cmd_AddUserMail msg;
			SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
			msg.dwDestUserID = GetUserID();
			msg.MailInfo = MailInfo;
			g_FishServer.SendNetCmdToDB(&msg);
		}
	}

	Sum = GetItemManager().QueryItemAllTimeCount(1304);
	if (Sum > 0)
	{
		//玩家拥有3号炮 
		DWORD AddCurrcey = Sum * 300 * ChangeRate;
		if (GetItemManager().OnDelUserItem(1304, GetItemManager().QueryItemCount(1304)))
		{
			//发送邮件
			tagRoleMail	MailInfo;
			MailInfo.bIsRead = false;
			if (ChangeRate == 2)
				_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("{ItemName:ItemID=%u}炮 获取机制已经被修改 特退还您购买炮的双倍补偿款"), 1304);
			else
				_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("{ItemName:ItemID=%u}炮 获取机制已经被修改 特退还您购买炮的补偿款"), 1304);
			MailInfo.RewardID = g_FishServer.GetFishConfig().GetSystemConfig().EmailCurrceyRewardID;
			MailInfo.RewardSum = AddCurrcey;
			MailInfo.MailID = 0;
			MailInfo.SendTimeLog = time(NULL);
			MailInfo.SrcFaceID = 0;
			TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
			MailInfo.SrcUserID = 0;//系统发送
			MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
			DBR_Cmd_AddUserMail msg;
			SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
			msg.dwDestUserID = GetUserID();
			msg.MailInfo = MailInfo;
			g_FishServer.SendNetCmdToDB(&msg);
		}
	}
}
void CRoleEx::UpdateRoleEvent()
{
	//便利 所有的任务 成就 活动 的 Join 函数
	m_RoleTask.OnResetJoinAllTask();
	m_RoleActionManager.OnResetJoinAllAction();
	m_RoleAchievement.OnResetJoinAllAchievement();
}
DWORD CRoleEx::GetRoleOnlineSec()
{
	return ConvertInt64ToDWORD(m_RoleInfo.OnlineMin * 60 + (time(NULL) - m_LogonTimeByDay));//获取当天在线的秒杀
}
void CRoleEx::ChangeRoleSocketID(DWORD SocketID)
{
	if (!m_RoleManager)
	{
		ASSERT(false);
		return;
	}
	if (m_dwGameSocketID == 0)//机器人不允许修改
	{
		ASSERT(false);
		return;
	}
	m_RoleManager->ChangeRoleSocket(this, SocketID);
	m_dwGameSocketID = SocketID;
}


void CRoleEx::CheckSystemOperatorMail()
{
	g_FishServer.GetSystemMailManager().TryGetSystemMail(this);
	/*
	time_t cur_time = time(NULL);
	
	std::vector<int> VC_EmptyIndexs;
	bool modify = false;
	//const SystemMailManager::MAILS& SystemMails = g_FishServer.GetSystemMailManager().
	//if (SystemMails.size() == 0)
	//{
	//	return;
	//}	
	MAILS::const_iterator it = g_FishServer.GetSystemMailManager()._OperatorMails.begin();
	for (; it != g_FishServer.GetSystemMailManager()._OperatorMails.end(); ++it)
	{
		VC_EmptyIndexs.clear();
		const tagOperatorSystemMail& MailEntry = it->second;
		bool can_send_mail = true;
		if (cur_time >= MailEntry.BeginTime && cur_time <= MailEntry.EndTime)
		{

			for (size_t i = 0; i < MAX_SYSTEM_RECORD_COUT; i++)
			{
				if (m_RoleInfo.SystemMailRecord[i].MailID == MailEntry.ID)
				{
					can_send_mail = false;
					break;
				}
				if (m_RoleInfo.SystemMailRecord[i].MailID == 0 || m_RoleInfo.SystemMailRecord[i].EndTime < cur_time)
				{
					m_RoleInfo.SystemMailRecord[i].MailID = 0;
					VC_EmptyIndexs.push_back(i);

				}
			}
			if (VC_EmptyIndexs.size() == 0)
			{
				break;
			}
			else if (can_send_mail)
			{
				m_RoleInfo.SystemMailRecord[VC_EmptyIndexs[0]].MailID = MailEntry.ID;
				m_RoleInfo.SystemMailRecord[VC_EmptyIndexs[0]].EndTime = MailEntry.EndTime;
				tagRoleMail	MailInfo;
				MailInfo.bIsRead = false;
				TCHARCopy(MailInfo.Context, CountArray(MailInfo.Context), MailEntry.Context,  CountArray(MailEntry.Context));
				MailInfo.RewardID = MailEntry.RewardID;
				MailInfo.RewardSum = MailEntry.RewardSum;
				MailInfo.MailID = 0;
				MailInfo.SendTimeLog = cur_time;
				MailInfo.SrcFaceID = 0;
				TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
				MailInfo.SrcUserID = 0;//系统发送
				MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum != 0);
				DBR_Cmd_AddUserMail msg;
				SetMsgInfo(msg, DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
				msg.dwDestUserID = GetUserID();
				msg.MailInfo = MailInfo;
				g_FishServer.SendNetCmdToDB(&msg);
				modify = true;
			}
		}
	}

	if (modify)
	{
		DBR_Cmd_SaveSystemMailRecord msg;
		for (size_t i = 0; i < MAX_SYSTEM_RECORD_COUT; i++)
		{
			msg.MailRecord[i] = m_RoleInfo.SystemMailRecord[i];
		}
		msg.UserID = GetUserID();
		SetMsgInfo(msg, DBR_SAVE_ROLE_SYSTEM_MAIL_RECORD, sizeof(DBR_Cmd_SaveSystemMailRecord));
		g_FishServer.SendNetCmdToDB(&msg);		
	}
	*/
	
}


void CRoleEx::OnUserLoadFinish(bool IsLogonGameServer)//当当前对象加载完毕的时候
{
	g_FishServer.GetRoleLogonManager().OnDleRoleOnlyInfo(m_RoleInfo.dwUserID);//玩家登陆成功的时候 删除玩家在Logon上保存的唯一码

	//g_DBLogManager.LogRoleOnlineInfo(m_RoleInfo.dwUserID, true, SendLogDB);

	//需要先确定 玩家需要前往 大厅 或者 是比赛
	//根据状态确定玩家
	CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_RoleInfo.dwUserID);
	bool IsToScrene = false;
	if (!IsLogonGameServer)
	{
		IsToScrene = false;
		if (pRole)
		{
			g_FishServer.GetTableManager()->OnPlayerLeaveTable(this->GetUserID());//如果玩家在桌子里面 离开桌子
		}
	}
	else
	{
		IsToScrene = (pRole != null);
	}
	SetAfkStates(false);
	if (IsToScrene && pRole)
	{
		//前往场景
		GameTable* pTable = g_FishServer.GetTableManager()->GetTable(pRole->GetTableID());
		if (pTable)
		{
			//将当前场景数据发送到客户端去
			//1.玩家在桌子里面 我们需要发送命令到客户端去 
			LC_Cmd_AccountLogonToScreen msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountLogonToScreen), sizeof(LC_Cmd_AccountLogonToScreen));
			msg.RandID = g_FishServer.GetRoleLogonManager().OnAddRoleOnlyInfo(m_RoleInfo.dwUserID);//将玩家添加到唯一编码里面去
			msg.bTableTypeID = pTable->GetTableTypeID();
			msg.BackgroundImage = pTable->GetFishDesk()->GetSceneBackground();
			if (m_RoleLauncherManager.IsCanUserLauncherByID(pRole->GetLauncherType()))//可以使用当前炮塔
				msg.LauncherType = pRole->GetLauncherType() | 128;
			else
				msg.LauncherType = pRole->GetLauncherType();
			msg.SeatID = pRole->GetSeatID();
			msg.RateIndex = pRole->GetRateIndex();
			msg.Energy = pRole->GetEnergy();
			SendDataToClient(&msg);
			//发送桌子上的数据到客户端去
			ResetRoleInfoToClient();//刷新桌子上的命令
		}
		else
		{
			//玩家不在桌子里面
			if (IsLogonGameServer)
				ResetClientInfo();

			LC_Cmd_AccountOnlyIDSuccess msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountOnlyIDSuccess), sizeof(LC_Cmd_AccountOnlyIDSuccess));
			msg.RandID = g_FishServer.GetRoleLogonManager().OnAddRoleOnlyInfo(m_RoleInfo.dwUserID);//将玩家添加到唯一编码里面去
			msg.RoleInfo = m_RoleInfo;
			msg.OperateIp = g_FishServer.GetOperateIP();
			SendDataToClient(&msg);
		}
	}
	else
	{
		if (IsLogonGameServer)
			ResetClientInfo();
		//前往大厅
		LC_Cmd_AccountOnlyIDSuccess msg;
		SetMsgInfo(msg, GetMsgType(Main_Logon, LC_AccountOnlyIDSuccess), sizeof(LC_Cmd_AccountOnlyIDSuccess));
		msg.RandID = g_FishServer.GetRoleLogonManager().OnAddRoleOnlyInfo(m_RoleInfo.dwUserID);//将玩家添加到唯一编码里面去
		msg.RoleInfo = m_RoleInfo;
		msg.OperateIp = g_FishServer.GetOperateIP();
		SendDataToClient(&msg);
	}
	//外部的其他命令
	SYSTEMTIME time;
	GetLocalTime(&time);
	LC_Cmd_DayChange msgSystemTime;
	SetMsgInfo(msgSystemTime, GetMsgType(Main_Role, LC_DayChange), sizeof(LC_Cmd_DayChange));
	msgSystemTime.Year = (BYTE)(time.wYear - 2000);
	msgSystemTime.Month = (BYTE)time.wMonth;
	msgSystemTime.Day = (BYTE)time.wDay;
	msgSystemTime.Hour = (BYTE)time.wHour;
	msgSystemTime.Min = (BYTE)time.wMinute;
	msgSystemTime.Sec = (BYTE)time.wSecond;
	msgSystemTime.IsDayUpdate = false;
	//msgSystemTime.IsNewDay = false;
	SendDataToClient(&msgSystemTime);

	//将部分特殊的数据 发送到客户端去
	g_FishServer.SendAllMonthPlayerSumToClient(m_RoleInfo.dwUserID);
	m_RoleLauncherManager.OnInit(this);
	m_RoleMessageStates.OnInit(this);//玩家登陆成功后发送命令


	//判断玩家是否需要重新设置二级密码
	if (GetRoleEntity().GetEntityInfo().Phone != 0 && m_RoleServerInfo.SecPasswordCrc1 == 0 && m_RoleServerInfo.SecPasswordCrc2 == 0 && m_RoleServerInfo.SecPasswordCrc3 == 0)
	{
		//玩家已经设置手机 但是未设置 手机的二级密码 我们想要让玩家设置手机密码
		LC_Cmd_SetSecondPassword msg;
		SetMsgInfo(msg, GetMsgType(Main_Role, LC_SetSecondPassword), sizeof(LC_Cmd_SetSecondPassword));
		SendDataToClient(&msg);
	}


	if (m_IsRobot)
		g_FishServer.GetRobotManager().OnAddRobot(GetUserID());//如果当前玩家为机器人 加入到机器人的集合里面去

	//char IpStr[32] = { 0 };
	//sprintf_s(IpStr, sizeof(IpStr), "玩家 %d 登陆成功\n", m_RoleInfo.dwUserID);
	//std::cout << IpStr;

	return;
}
//bool CRoleEx::SetRoleIsOnline(bool IsOnline)
//{
//	//发送数据库命令 设置当前玩家已经上线了
//	DBR_Cmd_RoleOnline msg;
//	SetMsgInfo(msg,DBR_SetOnline, sizeof(DBR_Cmd_RoleOnline));
//	msg.dwUserID = m_RoleInfo.dwUserID;
//	msg.IsOnline = IsOnline;
//	g_FishServer.SendNetCmdToSaveDB(&msg);
//	return true;
//}
void CRoleEx::SendDataToClient(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (m_IsRobot)
		return;
	if (m_IsAfk)//玩家下线了 无须发送命令到客户端去
		return;
	//将玩家的命令发送到客户端去 并且将数据进行处理
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(m_dwGameSocketID);
	if (!pClient)
		return;
	g_FishServer.SendNewCmdToClient(pClient, pCmd);
}
void CRoleEx::SendUserInfoToCenter()
{
	//将当前玩家的数据发送到Center去
	//1.发送玩家的基本数据
	CL_UserEnter msg;
	SetMsgInfo(msg,GetMsgType(Main_Center, CL_Sub_UserEnter), sizeof(CL_UserEnter));
	msg.RoleInfo.bGender = m_RoleInfo.bGender;
	//msg.RoleInfo.dwExp = m_RoleInfo.dwExp;
	msg.RoleInfo.dwFaceID = m_RoleInfo.dwFaceID;
	msg.RoleInfo.dwUserID = m_RoleInfo.dwUserID;
	msg.RoleInfo.wLevel = m_RoleInfo.wLevel;
	msg.RoleInfo.dwAchievementPoint = m_RoleInfo.dwAchievementPoint;
	msg.RoleInfo.TitleID = m_RoleInfo.TitleID;
	msg.RoleInfo.ClientIP = m_RoleInfo.ClientIP;//客户端的IP地址
	msg.IsRobot = m_IsRobot;
	msg.RoleInfo.IsShowIpAddress = m_RoleInfo.IsShowIPAddress;
	msg.RoleInfo.VipLevel = m_RoleInfo.VipLevel;
	msg.RoleInfo.IsInMonthCard = (m_RoleInfo.MonthCardID != 0 && time(null) <= m_RoleInfo.MonthCardEndTime);
	msg.RoleInfo.ParticularStates = m_RoleInfo.ParticularStates;//特殊状态
	msg.RoleInfo.GameID = m_RoleInfo.GameID;
	for (int i = 0; i < MAX_CHARM_ITEMSUM;++i)
		msg.RoleInfo.CharmArray[i] = m_RoleInfo.CharmArray[i];
	TCHARCopy(msg.RoleInfo.NickName, CountArray(msg.RoleInfo.NickName), m_RoleInfo.NickName, _tcslen(m_RoleInfo.NickName));
	msg.RoleInfo.IsOnline = true;//设置玩家在中央服务器状态为在线状态
	SendDataToCenter(&msg);
	//2.发送玩家的关系数据
	m_RelationManager.SendRoleRelationToCenter();//将数据发送到Center去
	//3.
	CL_UserEnterFinish msgFinish;
	SetMsgInfo(msgFinish,GetMsgType(Main_Center, CL_Sub_UserEnterFinish), sizeof(CL_UserEnterFinish));
	msgFinish.dwUserID = m_RoleInfo.dwUserID;
	SendDataToCenter(&msgFinish);
	//4.
	if (m_IsChangeClientIP)
	{
		//玩家的IP地址变化了 我们想要进行修改玩家的IP地址 并且通知其他玩家 在游戏过程中IP地址是不会发生变化的
		CC_Cmd_ChangeRoleClientIP msg;
		SetMsgInfo(msg, GetMsgType(Main_Role, CC_ChangeRoleClientIP), sizeof(CC_Cmd_ChangeRoleClientIP));
		msg.dwUserID = m_RoleInfo.dwUserID;
		msg.ClientIP = m_RoleInfo.ClientIP;//玩家IP变化了
		m_IsChangeClientIP = false;
	}
}
void CRoleEx::SendUserLeaveToCenter()
{
	//玩家离开中央服务器
	//g_FishServer.DelRoleOnlineInfo(m_RoleInfo.dwUserID);
	CL_UserLeave msg;
	SetMsgInfo(msg,GetMsgType(Main_Center, CL_Sub_UserLeave), sizeof(CL_UserLeave));
	msg.dwUserID = m_RoleInfo.dwUserID;
	SendDataToCenter(&msg);
}
void CRoleEx::SendDataToCenter(NetCmd* pCmd)
{
	g_FishServer.SendNetCmdToCenter(pCmd);
}
//void CRoleEx::SendDataToRank(NetCmd* pCmd)
//{
//	g_FishServer.SendNetCmdToRank(pCmd);
//}
//修改函数 其他玩家引用自己的 有关系(想要立刻同步) 有桌子 (桌子 想要立刻同步)
bool CRoleEx::ChangeRoleExp(int AddExp, bool IsSendToClient)
{
	if (!g_FishServer.GetTableManager() || !g_FishServer.GetTableManager()->GetGameConfig())
	{
		ASSERT(false);
		return false;
	}
	if (AddExp == 0)
		return true;
	if (!CheckChangeDWORDValue(m_RoleInfo.dwExp, AddExp))
		return false;
	m_RoleInfo.dwExp += AddExp;
	//玩家的经验增加了  我们想要 判断玩家是否发生等级修改的状态  如果等级也变化了 我们想要将经验清空  等级提升
	DWORD ChangeExp = 0;
	WORD  ChangeLevel = 0;
	g_FishServer.GetTableManager()->GetGameConfig()->LevelUp(m_RoleInfo.wLevel, m_RoleInfo.dwExp, ChangeLevel, ChangeExp);
	if (ChangeExp != m_RoleInfo.dwExp)
		m_RoleInfo.dwExp = ChangeExp;
	if (IsSendToClient || (m_RoleInfo.wLevel != ChangeLevel))//当等级发送变化的时候 发送经验到客户端去
	{
		LC_Cmd_ChangRoleExp msg;
		SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleExp), sizeof(LC_Cmd_ChangRoleExp));
		msg.dwExp = ChangeExp;
		SendDataToClient(&msg);
	}
	m_IsNeedSave = true;
	if (m_RoleInfo.wLevel != ChangeLevel)
	{
		ChangeRoleLevel(ChangeLevel - m_RoleInfo.wLevel);
	}
	
	return true;
}
bool CRoleEx::ChangeRoleLevel(short AddLevel)
{
	//当玩家点击改变的时候
	if (AddLevel == 0)
		return true;
	if (!CheckChangeDWORDValue(m_RoleInfo.wLevel, AddLevel))
		return false;
	m_RoleInfo.wLevel += AddLevel;

	OnHandleEvent(true,true,true,ET_UpperLevel, 0, AddLevel);
	OnHandleEvent(true, true, true, ET_ToLevel, 0,m_RoleInfo.wLevel);
	//当玩家等级变化的时候
	m_RoleTask.OnRoleLevelChange();
	m_RoleAchievement.OnRoleLevelChange();
	m_RoleActionManager.OnRoleLevelChange();

	HashMap<WORD, WORD>::iterator Iter = g_FishServer.GetFishConfig().GetFishLevelRewardConfig().m_LevelRewardMap.find(m_RoleInfo.wLevel);
	if (Iter != g_FishServer.GetFishConfig().GetFishLevelRewardConfig().m_LevelRewardMap.end())
	{
		WORD RewardID = Iter->second;
		OnAddRoleRewardByRewardID(RewardID, TEXT("升级获得奖励"));
	}

	LC_Cmd_ChangeRoleLevel msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleLevel), sizeof(LC_Cmd_ChangeRoleLevel));
	msg.wLevel = m_RoleInfo.wLevel;
	//发生出去 好友 已经 全部的桌子上的玩家
	SendDataToClient(&msg);

	LC_Cmd_TableChangeRoleLevel msgTable;
	SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleLevel), sizeof(LC_Cmd_TableChangeRoleLevel));
	msgTable.dwDestUserID = m_RoleInfo.dwUserID;
	msgTable.wLevel = m_RoleInfo.wLevel;
	SendDataToTable(&msgTable);

	CC_Cmd_ChangeRoleLevel msgCenter;
	SetMsgInfo(msgCenter,GetMsgType(Main_Role, CC_ChangeRoleLevel), sizeof(CC_Cmd_ChangeRoleLevel));
	msgCenter.dwUserID = m_RoleInfo.dwUserID;
	msgCenter.wLevel = m_RoleInfo.wLevel;
	SendDataToCenter(&msgCenter);


	DBR_Cmd_SaveRoleLevel msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleLevel, sizeof(DBR_Cmd_SaveRoleLevel));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.wLevel = m_RoleInfo.wLevel;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
	return true;
}
bool CRoleEx::ChangeRoleGender(bool bGender)
{
	if (m_RoleInfo.bGender == bGender)
		return true;
	m_RoleInfo.bGender = bGender;
	LC_Cmd_ChangeRoleGender msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleGender), sizeof(LC_Cmd_ChangeRoleGender));
	msg.bGender = bGender;
	SendDataToClient(&msg);

	LC_Cmd_TableChangeRoleGender msgTable;
	SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleGender), sizeof(LC_Cmd_TableChangeRoleGender));
	msgTable.dwDestUserID = m_RoleInfo.dwUserID;
	msgTable.bGender = bGender;
	SendDataToTable(&msgTable);

	CC_Cmd_ChangeRoleGender msgCenter;
	SetMsgInfo(msgCenter,GetMsgType(Main_Role, CC_ChangeRoleGender), sizeof(CC_Cmd_ChangeRoleGender));
	msgCenter.dwUserID = m_RoleInfo.dwUserID;
	msgCenter.bGender = bGender;
	SendDataToCenter(&msgCenter);

	DBR_Cmd_SaveRoleGender msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleGender, sizeof(DBR_Cmd_SaveRoleGender));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.bGender = m_RoleInfo.bGender;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);


	return true;
}
bool CRoleEx::ChangeRoleFaceID(DWORD FaceID)
{
	if (m_RoleInfo.dwFaceID == FaceID)
		return true;
	m_RoleInfo.dwFaceID = FaceID;
	LC_Cmd_ChangeRoleFaceID msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleFaceID), sizeof(LC_Cmd_ChangeRoleFaceID));
	msg.dwFaceID = m_RoleInfo.dwFaceID;
	SendDataToClient(&msg);

	LC_Cmd_TableChangeRoleFaceID msgTable;
	SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleFaceID), sizeof(LC_Cmd_TableChangeRoleFaceID));
	msgTable.dwDestUserID = m_RoleInfo.dwUserID;
	msgTable.dwDestFaceID = m_RoleInfo.dwFaceID;
	SendDataToTable(&msgTable);

	CC_Cmd_ChangeRoleFaceID msgCenter;
	SetMsgInfo(msgCenter,GetMsgType(Main_Role, CC_ChangeRoleFaceID), sizeof(CC_Cmd_ChangeRoleFaceID));
	msgCenter.dwUserID = m_RoleInfo.dwUserID;
	msgCenter.dwFaceID = m_RoleInfo.dwFaceID;
	SendDataToCenter(&msgCenter);

	GM_Cmd_RoleChangeFaceID msgMini;
	SetMsgInfo(msgMini, GetMsgType(Main_MiniGame, GM_RoleChangeFaceID), sizeof(GM_Cmd_RoleChangeFaceID));
	msgMini.dwUserID = m_RoleInfo.dwUserID;
	msgMini.FaceID = m_RoleInfo.dwFaceID;
	g_FishServer.SendNetCmdToMiniGame(&msgMini);

	DBR_Cmd_SaveRoleFaceID msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleFaceID, sizeof(DBR_Cmd_SaveRoleFaceID));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.dwFaceID = m_RoleInfo.dwFaceID;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleNickName(LPTSTR pNickName)
{
	if (_tcscmp(pNickName, m_RoleInfo.NickName) == 0)
		return true;
	if (!g_FishServer.GetFishConfig().CheckStringIsError(pNickName, MIN_NICKNAME, MAX_NICKNAME, SCT_Normal))
	{
		ASSERT(false);
		return false;
	}
	//玩家进行名称 修改 需要先询问数据库 
	DBR_Cmd_SaveRoleNickName msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleNickName, sizeof(DBR_Cmd_SaveRoleNickName));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	TCHARCopy(msgDB.NickName, CountArray(msgDB.NickName), pNickName, _tcslen(pNickName));
	g_FishServer.SendNetCmdToDB(&msgDB);
	return true;
}
void CRoleEx::ChangeRoleNickNameResult(DBO_Cmd_SaveRoleNickName* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	if (pMsg->Result)
	{
		TCHARCopy(m_RoleInfo.NickName, CountArray(m_RoleInfo.NickName), pMsg->NickName, _tcslen(pMsg->NickName));

		LC_Cmd_ChangeRoleNickName msg;
		msg.Result = true;
		SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleNickName), sizeof(LC_Cmd_ChangeRoleNickName));
		TCHARCopy(msg.NickName, CountArray(msg.NickName), pMsg->NickName, _tcslen(pMsg->NickName));
		SendDataToClient(&msg);

		LC_Cmd_TableChangeRoleNickName msgTable;
		SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleNickName), sizeof(LC_Cmd_TableChangeRoleNickName));
		msgTable.dwDestUserID = m_RoleInfo.dwUserID;
		TCHARCopy(msgTable.NickName, CountArray(msgTable.NickName), pMsg->NickName, _tcslen(pMsg->NickName));
		SendDataToTable(&msgTable);

		CC_Cmd_ChangeRoleNickName msgCenter;
		SetMsgInfo(msgCenter, GetMsgType(Main_Role, CC_ChangeRoleNickName), sizeof(CC_Cmd_ChangeRoleNickName));
		msgCenter.dwUserID = m_RoleInfo.dwUserID;
		TCHARCopy(msgCenter.NickName, CountArray(msgCenter.NickName), pMsg->NickName, _tcslen(pMsg->NickName));
		SendDataToCenter(&msgCenter);

		GM_Cmd_RoleChangeNickName msgMini;
		SetMsgInfo(msgMini, GetMsgType(Main_MiniGame, GM_RoleChangeNickName), sizeof(GM_Cmd_RoleChangeNickName));
		msgMini.dwUserID = m_RoleInfo.dwUserID;
		TCHARCopy(msgMini.NickName, CountArray(msgMini.NickName), pMsg->NickName, _tcslen(pMsg->NickName));
		g_FishServer.SendNetCmdToMiniGame(&msgMini);
	}
	else
	{
		//修改失败了
		LC_Cmd_ChangeRoleNickName msg;
		msg.Result = false;
		SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleNickName), sizeof(LC_Cmd_ChangeRoleNickName));
		TCHARCopy(msg.NickName, CountArray(msg.NickName), pMsg->NickName, _tcslen(pMsg->NickName));
		SendDataToClient(&msg);
	}
}
bool CRoleEx::ChangeRoleGlobe(int AddGlobe, bool IsSendToClient, bool IsSaveToDB,bool IsSendToMiniGame)
{
	if (AddGlobe == 0)
		return true;
	if (!CheckChangeDWORDValue(m_RoleInfo.dwGlobeNum, AddGlobe))
		return false;

	if (AddGlobe >0 && m_RoleInfo.dwGlobeNum + AddGlobe >= g_FishServer.GetFishConfig().GetSystemConfig().MaxGobelSum)//金币到达上限了 无法添加金币
		return false;

	m_RoleInfo.dwGlobeNum += AddGlobe;

	if (AddGlobe > 0)
		OnHandleEvent(true, true, true, ET_GetGlobel, 0, AddGlobe);

	OnHandleEvent(true, true, true, ET_MaxGlobelSum, 0,m_RoleInfo.dwGlobeNum);

	if (IsSendToClient)
	{
		LC_Cmd_ChangeRoleGlobe msg;
		SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleGlobe), sizeof(LC_Cmd_ChangeRoleGlobe));
		msg.dwGlobeNum = AddGlobe;//m_RoleInfo.dwGlobeNum;
		SendDataToClient(&msg);//只发送客户端去

		LC_Cmd_TableChangeRoleGlobe msgTable;
		SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleGlobe), sizeof(LC_Cmd_TableChangeRoleGlobe));
		msgTable.dwDestUserID = m_RoleInfo.dwUserID;
		msgTable.dwGlobelSum = AddGlobe;//m_RoleInfo.dwGlobeNum;
		SendDataToTable(&msgTable);
	}

	if (IsSaveToDB)
	{
		//保存到数据库去
		DBR_Cmd_SaveRoleGlobel msgDB;
		SetMsgInfo(msgDB, DBR_SaveRoleGlobel, sizeof(DBR_Cmd_SaveRoleGlobel));
		msgDB.dwUserID = m_RoleInfo.dwUserID;
		msgDB.dwGlobel = m_RoleInfo.dwGlobeNum;
		g_FishServer.SendNetCmdToSaveDB(&msgDB);
	}
	else
		m_IsNeedSave = true;

	if (IsSendToMiniGame)
	{
		GM_Cmd_RoleChangeGlobel msg;
		SetMsgInfo(msg, GetMsgType(Main_MiniGame, GM_RoleChangeGlobel), sizeof(GM_Cmd_RoleChangeGlobel));
		msg.dwUserID = m_RoleInfo.dwUserID;
		msg.dwGlobel = m_RoleInfo.dwGlobeNum;
		g_FishServer.SendNetCmdToMiniGame(&msg);
	}

	if (AddGlobe > 0)
	{
		m_RoleGameData.OnHandleRoleGetGlobel(AddGlobe);
	}

	return true;
}
bool CRoleEx::ChangeRoleMedal(int AddMedal, const TCHAR *pcStr)
{
	if (AddMedal == 0)
		return true;
	if (!CheckChangeDWORDValue(m_RoleInfo.dwMedalNum, AddMedal))
		return false;
	m_RoleInfo.dwMedalNum += AddMedal;
	if (AddMedal > 0)
		OnHandleEvent(true, true, true, ET_GetMadel, 0, AddMedal);

	if (AddMedal < 0)
		m_RoleInfo.TotalUseMedal += (AddMedal*-1);//记录玩家总使用的奖牌数

	LC_Cmd_ChangeRoleMedal msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleMedal), sizeof(LC_Cmd_ChangeRoleMedal));
	msg.dwMedalNum = m_RoleInfo.dwMedalNum;
	msg.TotalUseMedal = m_RoleInfo.TotalUseMedal;
	SendDataToClient(&msg);//只发送客户端去

	GM_Cmd_RoleChangeMadel msgMini;
	SetMsgInfo(msgMini, GetMsgType(Main_MiniGame, GM_RoleChangeMadel), sizeof(GM_Cmd_RoleChangeMadel));
	msgMini.dwUserID = m_RoleInfo.dwUserID;
	msgMini.dwMadel = m_RoleInfo.dwMedalNum;
	g_FishServer.SendNetCmdToMiniGame(&msgMini);

	//保存到数据库去
	DBR_Cmd_SaveRoleMedal msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleMedal, sizeof(DBR_Cmd_SaveRoleMedal));
	msgDB.dwMedalSum = m_RoleInfo.dwMedalNum;
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.TotalUseMedal = m_RoleInfo.TotalUseMedal;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	//记录Log到数据库去
	g_DBLogManager.LogToDB(GetUserID(), LT_Medal, AddMedal,0, pcStr, SendLogDB);
	return true;
}
bool CRoleEx::ChangeRoleCurrency(int AddCurrency, const TCHAR *pcStr)
{
	if (AddCurrency == 0)
		return true;
	if (!CheckChangeDWORDValue(m_RoleInfo.dwCurrencyNum, AddCurrency))
		return false;
	m_RoleInfo.dwCurrencyNum += AddCurrency;
	if (AddCurrency > 0)
		OnHandleEvent(true, true, true, ET_GetCurren, 0, AddCurrency);

	OnHandleEvent(true, true, true, ET_MaxCurren, 0, m_RoleInfo.dwCurrencyNum);

	LC_Cmd_ChangeRoleCurrency msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleCurrency), sizeof(LC_Cmd_ChangeRoleCurrency));
	msg.dwCurrencyNum = m_RoleInfo.dwCurrencyNum;
	SendDataToClient(&msg);//只发送客户端去

	GM_Cmd_RoleChangeCurrcey msgMini;
	SetMsgInfo(msgMini, GetMsgType(Main_MiniGame, GM_RoleChangeCurrcey), sizeof(GM_Cmd_RoleChangeCurrcey));
	msgMini.dwUserID = m_RoleInfo.dwUserID;
	msgMini.dwCurrcey = m_RoleInfo.dwCurrencyNum;
	g_FishServer.SendNetCmdToMiniGame(&msgMini);

	DBR_Cmd_SaveRoleCurrency msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleCurrency, sizeof(DBR_Cmd_SaveRoleCurrency));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.dwCurrencyNum = m_RoleInfo.dwCurrencyNum;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	//记录Log到数据库去
	g_DBLogManager.LogToDB(GetUserID(), LT_Currcey, AddCurrency,0, pcStr, SendLogDB);

	return true;
}
bool CRoleEx::LostUserMoney(DWORD Globel, DWORD Medal, DWORD Currey, const TCHAR *pcStr)
{
	if (Globel >= MAXINT32 || Medal >= MAXINT32 || Currey >= MAXINT32)
	{
		return false;
	}
	if (m_RoleInfo.dwGlobeNum < Globel || m_RoleInfo.dwMedalNum < Medal || m_RoleInfo.dwCurrencyNum < Currey)
		return false;
	if (!ChangeRoleGlobe(Globel*-1, true))
		return false;

	if (!ChangeRoleMedal(Medal*-1, pcStr))
	{
		ChangeRoleGlobe(Globel, true);
		return false;
	}
	if (!ChangeRoleCurrency(Currey*-1, pcStr))
	{
		ChangeRoleGlobe(Globel, true);

		//归还扣除的乐币
		TCHAR NewChar[DB_Log_Str_Length];
		_stprintf_s(NewChar, CountArray(NewChar), TEXT("归还扣除的奖牌 数量:%d"),Medal);

		ChangeRoleMedal(Medal, NewChar);
		return false;
	}
	return true;
}
void CRoleEx::SendDataToTable(NetCmd* pCmd)
{
	if (!g_FishServer.GetTableManager())
	{
		free(pCmd);
		ASSERT(false);
		return;
	}
	g_FishServer.GetTableManager()->SendDataToTable(m_RoleInfo.dwUserID, pCmd);
}
//void CRoleEx::SaveRoleExInfo()
//{
//	if (!m_IsNeedSave)
//		return;
//	将玩家整体的数据保存到数据库去
//	DBR_Cmd_SaveRoleExInfo msg;
//	SetMsgInfo(msg,DBR_SaveRoleExInfo, sizeof(DBR_Cmd_SaveRoleExInfo));
//	msg.RoleInfo = m_RoleInfo;
//	msg.RoleCharmValue = g_FishServer.GetFishConfig().GetCharmValue(m_RoleInfo.CharmArray);
//	g_FishServer.SendNetCmdToDB(&msg);
//	m_IsNeedSave = false;
//}

bool CRoleEx::ChangeRoleProduction(DWORD dwProduction)
{
	m_RoleInfo.dwProduction += dwProduction;
	return true;
}

bool CRoleEx::ChangeRoleGameTime(WORD wGameTime)
{
	m_RoleInfo.dwGameTime += wGameTime;
	return true;
}
bool CRoleEx::ChangeRoleTitle(BYTE TitleID)
{
	//改变玩家的称号ID 设置玩家的称号 被称号系统调用
	if (m_RoleInfo.TitleID == TitleID)
		return true;
	m_RoleInfo.TitleID = TitleID;
	//1.发送到客户端去 
	LC_Cmd_ChangeRoleTitle msgClient;
	SetMsgInfo(msgClient,GetMsgType(Main_Role, LC_ChangeRoleTitle), sizeof(LC_Cmd_ChangeRoleTitle));
	msgClient.TitleID = TitleID;
	SendDataToClient(&msgClient);
	//2.发送到中央服务器去
	CC_Cmd_ChangeRoleTitle msgCenter;
	SetMsgInfo(msgCenter,GetMsgType(Main_Role, CC_ChangeRoleTitle), sizeof(CC_Cmd_ChangeRoleTitle));
	msgCenter.dwUserID = GetUserID();
	msgCenter.TitleID = TitleID;
	SendDataToCenter(&msgCenter);
	//3.发送到桌子上面去
	LC_Cmd_TableChangeRoleTitle msgTable;
	SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleTitle), sizeof(LC_Cmd_TableChangeRoleTitle));
	msgTable.dwDestUserID = GetUserID();
	msgTable.TitleID = TitleID;
	SendDataToTable(&msgTable);

	DBR_Cmd_SaveRoleCurTitle msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleCurTitle, sizeof(DBR_Cmd_SaveRoleCurTitle));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.TitleID = m_RoleInfo.TitleID;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);


	return true;
}
bool CRoleEx::ChangeRoleAchievementPoint(DWORD dwAchievementPoint)
{
	//修改玩家的成就点数
	if (dwAchievementPoint == 0)
		return true;
	m_RoleInfo.dwAchievementPoint += dwAchievementPoint;

	LC_Cmd_ChangeRoleAchievementPoint msgClient;
	SetMsgInfo(msgClient,GetMsgType(Main_Role, LC_ChangeRoleAchievementPoint), sizeof(LC_Cmd_ChangeRoleAchievementPoint));
	msgClient.dwAchievementPoint = m_RoleInfo.dwAchievementPoint;
	SendDataToClient(&msgClient);

	LC_Cmd_TableChangeRoleAchievementPoint msgTable;
	SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleAchievementPoint), sizeof(LC_Cmd_TableChangeRoleAchievementPoint));
	msgTable.dwDestUserID = m_RoleInfo.dwUserID;
	msgTable.dwAchievementPoint = m_RoleInfo.dwAchievementPoint;
	SendDataToTable(&msgTable);

	CC_Cmd_ChangeRoleAchievementPoint msgCenter;
	SetMsgInfo(msgCenter,GetMsgType(Main_Role, CC_ChangeRoleAchievementPoint), sizeof(CC_Cmd_ChangeRoleAchievementPoint));
	msgCenter.dwUserID = m_RoleInfo.dwUserID;
	msgCenter.dwAchievementPoint = m_RoleInfo.dwAchievementPoint;
	SendDataToCenter(&msgCenter);

	DBR_Cmd_SaveRoleAchievementPoint msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleAchievementPoint, sizeof(DBR_Cmd_SaveRoleAchievementPoint));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.dwAchievementPoint = m_RoleInfo.dwAchievementPoint;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleCharmValue(BYTE Index, int AddSum)
{
	//修改玩家的魅力数据
	if (Index >= MAX_CHARM_ITEMSUM || AddSum == 0)
		return true;
	if (!CheckChangeDWORDValue(m_RoleInfo.CharmArray[Index], AddSum))
		return false;
	m_RoleInfo.CharmArray[Index] += AddSum;
	//1.发送命令到客户端去
	LC_Cmd_ChangeRoleCharmValue msgClient;
	SetMsgInfo(msgClient,GetMsgType(Main_Role, LC_ChangeRoleCharmValue), sizeof(LC_Cmd_ChangeRoleCharmValue));
	for (int i = 0; i < MAX_CHARM_ITEMSUM;++i)
		msgClient.CharmInfo[i] = m_RoleInfo.CharmArray[i];
	SendDataToClient(&msgClient);
	//2.发送同桌子上的玩家
	LC_Cmd_TableChangeRoleCharmValue msgTable;
	SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleCharmValue), sizeof(LC_Cmd_TableChangeRoleCharmValue));
	msgTable.dwDestUserID = m_RoleInfo.dwUserID;
	for (int i = 0; i < MAX_CHARM_ITEMSUM; ++i)
		msgTable.CharmInfo[i] = m_RoleInfo.CharmArray[i];
	SendDataToTable(&msgTable);
	//3.发送到中央服务器去
	CC_Cmd_ChangeRoleCharmValue msgCenter;
	SetMsgInfo(msgCenter,GetMsgType(Main_Role, CC_ChangeRoleCharmValue), sizeof(CC_Cmd_ChangeRoleCharmValue));
	msgCenter.dwUserID = m_RoleInfo.dwUserID;
	for (int i = 0; i < MAX_CHARM_ITEMSUM; ++i)
		msgCenter.CharmInfo[i] = m_RoleInfo.CharmArray[i];
	SendDataToCenter(&msgCenter);

	DBR_Cmd_SaveRoleCharmArray msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleCharmArray, sizeof(DBR_Cmd_SaveRoleCharmArray));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	for (int i = 0; i < MAX_CHARM_ITEMSUM; ++i)
		msgDB.CharmArray[i] = m_RoleInfo.CharmArray[i];
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleSendGiffSum(int AddSum)
{
	if (AddSum == 0)
		return true;
	m_RoleInfo.SendGiffSum = ConvertIntToBYTE(m_RoleInfo.SendGiffSum + AddSum);
	//发送到客户度去
	LC_Cmd_ChangeRoleSendGiffSum msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleSendGiffSum), sizeof(LC_Cmd_ChangeRoleSendGiffSum));
	msg.SendGiffSum = m_RoleInfo.SendGiffSum;
	SendDataToClient(&msg);

	DBR_Cmd_SaveRoleSendGiffSum msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleSendGiffSum, sizeof(DBR_Cmd_SaveRoleSendGiffSum));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.SendSum = m_RoleInfo.SendGiffSum;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleAcceptGiffSum(int AddSum)
{
	if (AddSum == 0)
		return true;
	m_RoleInfo.AcceptGiffSum = ConvertIntToBYTE(m_RoleInfo.AcceptGiffSum + AddSum);
	LC_Cmd_ChangeRoleAcceptGiffSum msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleAcceptGiffSum), sizeof(LC_Cmd_ChangeRoleAcceptGiffSum));
	msg.AcceptGiffSum = m_RoleInfo.AcceptGiffSum;
	SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleAcceptGiffSum msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleAcceptGiffSum, sizeof(DBR_Cmd_SaveRoleAcceptGiffSum));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.AcceptSum = m_RoleInfo.AcceptGiffSum;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ClearRoleTaskStates()
{
	int256Handle::Clear(m_RoleInfo.TaskStates);

	DBR_Cmd_SaveRoleTaskStates msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleTaskStates, sizeof(DBR_Cmd_SaveRoleTaskStates));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.States = m_RoleInfo.TaskStates;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleTaskStates(BYTE Index, bool States)
{
	//设置任务的状态
	if (int256Handle::GetBitStates(m_RoleInfo.TaskStates, Index) == States)
		return true;
	int256Handle::SetBitStates(m_RoleInfo.TaskStates, Index,States);
	//发送命令到客户端去
	LC_Cmd_ChangeRoleTaskStates msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleTaskStates), sizeof(LC_Cmd_ChangeRoleTaskStates));
	msg.Index = Index;
	msg.States = States;
	SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleTaskStates msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleTaskStates, sizeof(DBR_Cmd_SaveRoleTaskStates));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.States = m_RoleInfo.TaskStates;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleAchievementStates(BYTE Index, bool States)
{
	if (int256Handle::GetBitStates(m_RoleInfo.AchievementStates, Index) == States)
		return true;
	int256Handle::SetBitStates(m_RoleInfo.AchievementStates, Index, States);
	//发送命令到客户端去
	LC_Cmd_ChangeRoleAchievementStates msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleAchievementStates), sizeof(LC_Cmd_ChangeRoleAchievementStates));
	msg.Index = Index;
	msg.States = States;
	SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleAchievementStates msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleAchievementStates, sizeof(DBR_Cmd_SaveRoleAchievementStates));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.States = m_RoleInfo.AchievementStates;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleActionStates(BYTE Index, bool States)
{
	if (int256Handle::GetBitStates(m_RoleInfo.ActionStates, Index) == States)
		return true;
	int256Handle::SetBitStates(m_RoleInfo.ActionStates, Index, States);
	//发送命令到客户端去
	LC_Cmd_ChangeRoleActionStates msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ChangeRoleActionStates), sizeof(LC_Cmd_ChangeRoleActionStates));
	msg.Index = Index;
	msg.States = States;
	SendDataToClient(&msg);
	
	DBR_Cmd_SaveRoleActionStates msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleActionStates, sizeof(DBR_Cmd_SaveRoleActionStates));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.States = m_RoleInfo.ActionStates;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleOnlineRewardStates(DWORD States)
{
	if (m_RoleInfo.OnlineRewardStates == States)
		return true;
	m_RoleInfo.OnlineRewardStates = States;

	DBR_Cmd_SaveRoleOnlineStates msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleOnlineStates, sizeof(DBR_Cmd_SaveRoleOnlineStates));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.OnlineStates = m_RoleInfo.OnlineRewardStates;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleCheckData(DWORD CheckData)
{
	//签到数据按月更新

	if (m_RoleInfo.CheckData == CheckData)
		return true;
	m_RoleInfo.CheckData = CheckData;
	//发送命令到客户端去
	LC_Cmd_ChangeRoleCheckData msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleCheckData), sizeof(LC_Cmd_ChangeRoleCheckData));
	msg.CheckData = CheckData;
	SendDataToClient(&msg);

	DBR_Cmd_SaveRoleCheckData msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleCheckData, sizeof(DBR_Cmd_SaveRoleCheckData));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.CheckData = m_RoleInfo.CheckData;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleIsShowIpAddress(bool States)
{
	//玩家进行修改 是否需要修改 是否显示IP地址
	if (m_RoleInfo.IsShowIPAddress == States)
		return true;
	m_RoleInfo.IsShowIPAddress = States;
	//将命令发送到客户端 和 桌子上其他玩家去 玩家的IP地址变化了 和 中央服务器去
	//1.玩家自己 告诉客户端 玩家属性变化了 bool
	LC_Cmd_ChangeRoleIsShowIpAddress msgClient;
	SetMsgInfo(msgClient, GetMsgType(Main_Role, LC_ChangeRoleIsShowIpAddress), sizeof(LC_Cmd_ChangeRoleIsShowIpAddress));
	msgClient.IsShowIpAddress = States;
	SendDataToClient(&msgClient);
	//2.中央服务器
	CC_Cmd_ChangeRoleIsShowIpAddress msgCenter;
	SetMsgInfo(msgCenter, GetMsgType(Main_Role, CC_ChangeRoleIsShowIpAddress), sizeof(CC_Cmd_ChangeRoleIsShowIpAddress));
	msgCenter.IsShowIpAddress = States;
	msgCenter.dwUserID = m_RoleInfo.dwUserID;
	SendDataToCenter(&msgCenter);
	//3.桌子 告诉桌子上其他玩家 玩家的IP地址变化了
	LC_Cmd_TableChangeRoleIpAddress msgTable;
	SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleIpAddress), sizeof(LC_Cmd_TableChangeRoleIpAddress));
	msgTable.dwDestUserID = m_RoleInfo.dwUserID;
	if (m_RoleInfo.IsShowIPAddress)
	{
		//展示自己的属性
		TCHARCopy(msgTable.IpAddress, CountArray(msgTable.IpAddress), m_RoleInfo.IPAddress, _tcslen(m_RoleInfo.IPAddress));
	}
	else
	{
		TCHARCopy(msgTable.IpAddress, CountArray(msgTable.IpAddress), Defalue_Ip_Address, _tcslen(Defalue_Ip_Address));
	}
	SendDataToTable(&msgTable);
	//4.设置当前数据立刻存储到数据库去
	
	//OnSaveRoleQueryAtt();

	DBR_Cmd_SaveRoleIsShowIpAddress msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleIsShowIpAddress, sizeof(DBR_Cmd_SaveRoleIsShowIpAddress));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.IsShowIP = m_RoleInfo.IsShowIPAddress;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	return true;
}
bool CRoleEx::ChangeRoleIsOnline(bool States)
{
	//告诉玩家是否在线的状态 玩家进入AFK 状态 无须处理 直接将数据发送到中央服务器 与 数据库去
	if (m_IsOnline == States)
		return true;
	m_IsOnline = States;

	DBR_Cmd_RoleOnline msg;
	SetMsgInfo(msg, DBR_SetOnline, sizeof(DBR_Cmd_RoleOnline));
	msg.dwUserID = m_RoleInfo.dwUserID;
	msg.IsOnline = States;
	g_FishServer.SendNetCmdToSaveDB(&msg);//需要快速保存的

	//中央服务器
	CC_Cmd_ChangeRoleIsOnline msgCenter;	
	SetMsgInfo(msgCenter, GetMsgType(Main_Role, CC_ChangeRoleIsOnline), sizeof(CC_Cmd_ChangeRoleIsOnline));
	msgCenter.dwUserID = m_RoleInfo.dwUserID;
	msgCenter.IsOnline = States;
	SendDataToCenter(&msgCenter);

	//发送命令到迷你服务器去 玩家下线了
	if (!States)
	{
		/*GM_Cmd_LeaveNiuNiuTable msgMini;
		SetMsgInfo(msgMini, GetMsgType(Main_MiniGame, GM_LeaveNiuNiuTable), sizeof(GM_Cmd_LeaveNiuNiuTable));
		msgMini.dwUserID = GetUserID();
		g_FishServer.SendNetCmdToMiniGame(&msgMini);*/

		GM_Cmd_RoleLeaveMiniGame msgLeave;
		SetMsgInfo(msgLeave, GetMsgType(Main_MiniGame, GM_RoleLeaveMiniGame), sizeof(GM_Cmd_RoleLeaveMiniGame));
		msgLeave.dwUserID = GetUserID();
		g_FishServer.SendNetCmdToMiniGame(&msgLeave);

		DBR_Cmd_TableChange msgDB;//记录玩家进入
		SetMsgInfo(msgDB, DBR_TableChange, sizeof(DBR_Cmd_TableChange));
		msgDB.dwUserID = GetUserID();
		msgDB.CurrceySum = GetRoleInfo().dwCurrencyNum;
		msgDB.GlobelSum = GetRoleInfo().dwGlobeNum;
		msgDB.MedalSum = GetRoleInfo().dwMedalNum;
		msgDB.JoinOrLeave = false;
		msgDB.LogTime = time(null);
		msgDB.TableTypeID = 250;
		msgDB.TableMonthID = 0;
		g_FishServer.SendNetCmdToSaveDB(&msgDB);
		g_DBLogManager.LogRoleJoinOrLeaveTableInfo(msgDB.dwUserID, msgDB.GlobelSum, msgDB.CurrceySum, msgDB.MedalSum, msgDB.TableTypeID, msgDB.TableMonthID, false, SendLogDB);
	}
	

	if (States)
	{
		string MacAddress = g_FishServer.GetUserMacAddress(GetUserID());
		string IPAddress = g_FishServer.GetUserIpAddress(GetUserID());
		g_DBLogManager.LogRoleOnlineInfo(m_RoleInfo.dwUserID, true, MacAddress, IPAddress, m_RoleInfo.dwGlobeNum, m_RoleInfo.dwCurrencyNum, m_RoleInfo.dwMedalNum, SendLogDB);
		//g_FishServer.ShowInfoToWin("玩家上线Log");
	}
	else
	{
		string MacAddress = g_FishServer.GetUserMacAddress(GetUserID());
		string IPAddress = g_FishServer.GetUserIpAddress(GetUserID());
		g_DBLogManager.LogRoleOnlineInfo(m_RoleInfo.dwUserID, false, MacAddress, IPAddress, m_RoleInfo.dwGlobeNum, m_RoleInfo.dwCurrencyNum, m_RoleInfo.dwMedalNum, SendLogDB);
		//g_FishServer.ShowInfoToWin("玩家下线Log");
	}
	return true;
}
bool CRoleEx::ChangeRoleExChangeStates(DWORD States)
{
	if (m_RoleInfo.ExChangeStates == States)
		return true;
	m_RoleInfo.ExChangeStates = States;
	//发送命令到客户端
	LC_Cmd_ChangeRoleExChangeStates msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleExChangeStates), sizeof(LC_Cmd_ChangeRoleExChangeStates));
	msg.States = States;
	SendDataToClient(&msg);
	//到数据库
	DBR_Cmd_SaveRoleExChangeStates msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleExChangeStates, sizeof(DBR_Cmd_SaveRoleExChangeStates));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.States = States;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
	return true;
}
bool CRoleEx::ChangeRoleTotalRechargeSum(DWORD AddSum)
{
	if (AddSum == 0)
		return true;
	m_RoleInfo.TotalRechargeSum += AddSum;//玩家总充值的数量 单位元 不是分

	//立刻保存到数据库去
	DBR_Cmd_SaveRoleTotalRecharge msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleTotalRecharge, sizeof(DBR_Cmd_SaveRoleTotalRecharge));
	msgDB.dwUserID = GetUserID();
	msgDB.Sum = m_RoleInfo.TotalRechargeSum;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
	//立刻发生到客户端去
	LC_Cmd_ChangeRoleTotalRecharge msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleTotalRecharge), sizeof(LC_Cmd_ChangeRoleTotalRecharge));
	msg.Sum = m_RoleInfo.TotalRechargeSum;
	SendDataToClient(&msg);

	GetRoleVip().OnRechargeRMBChange();//当玩家充值变化的时候 进行处理
	return true;
}
bool CRoleEx::ChangeRoleIsFirstPayGlobel()
{
	if (!m_RoleInfo.bIsFirstPayGlobel)
		return false;
	m_RoleInfo.bIsFirstPayGlobel = false;
	//发送到数据库去 
	DBR_Cmd_SaveRoleIsFirstPayGlobel msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleIsFirstPayGlobel, sizeof(DBR_Cmd_SaveRoleIsFirstPayGlobel));
	msgDB.dwUserID = GetUserID();
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
	//发送到客户端去
	LC_Cmd_ChangeRoleIsFirstPayGlobel msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleIsFirstPayGlobel), sizeof(LC_Cmd_ChangeRoleIsFirstPayGlobel));
	SendDataToClient(&msg);
	return true;
}
bool CRoleEx::ChangeRoleIsFirstPayCurrcey()
{
	if (!m_RoleInfo.bIsFirstPayCurrcey)
		return false;
	m_RoleInfo.bIsFirstPayCurrcey = false;
	//发送到数据库去
	DBR_Cmd_SaveRoleIsFirstPayCurrcey msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleIsFirstPayCurrcey, sizeof(DBR_Cmd_SaveRoleIsFirstPayCurrcey));
	msgDB.dwUserID = GetUserID();
	g_FishServer.SendNetCmdToSaveDB(&msgDB);
	//发送到客户端去
	LC_Cmd_ChangeRoleIsFirstPayCurrcey msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleIsFirstPayCurrcey), sizeof(LC_Cmd_ChangeRoleIsFirstPayCurrcey));
	SendDataToClient(&msg);
	return true;
}
bool CRoleEx::ChangeRoleParticularStates(DWORD States)
{
	if (m_RoleInfo.ParticularStates == States)
		return false;
	m_RoleInfo.ParticularStates = States;

	//1.通知数据库
	DBR_Cmd_SaveRoleParticularStates msg;
	SetMsgInfo(msg, DBR_SaveRoleParticularStates, sizeof(msg));
	msg.dwUserID = m_RoleInfo.dwUserID;
	msg.ParticularStates = m_RoleInfo.ParticularStates;
	g_FishServer.SendNetCmdToSaveDB(&msg);
	//2.通知中央服务器 
	CC_Cmd_ChangeRoleParticularStates msgCenter;
	SetMsgInfo(msgCenter, GetMsgType(Main_Role, CC_ChangeRoleParticularStates), sizeof(CC_Cmd_ChangeRoleParticularStates));
	msgCenter.dwUserID = m_RoleInfo.dwUserID;
	msgCenter.ParticularStates = m_RoleInfo.ParticularStates;
	SendDataToCenter(&msgCenter);
	//客户端
	LC_Cmd_ChangeRoleParticularStates msgClient;
	SetMsgInfo(msgClient, GetMsgType(Main_Role, LC_ChangeRoleParticularStates), sizeof(LC_Cmd_ChangeRoleParticularStates));
	msgClient.ParticularStates = m_RoleInfo.ParticularStates;
	SendDataToClient(&msgClient);
	//3.通知miniGame
	GM_Cmd_ChangeRoleParticularStates msgMini;
	SetMsgInfo(msgMini, GetMsgType(Main_MiniGame, GM_ChangeRoleParticularStates), sizeof(GM_Cmd_ChangeRoleParticularStates));
	msgMini.dwUserID = m_RoleInfo.dwUserID;
	msgMini.ParticularStates = m_RoleInfo.ParticularStates;
	g_FishServer.SendNetCmdToMiniGame(&msgMini);
	return true;
}
void CRoleEx::OnRoleCatchFishByLottery(BYTE FishTypeID, CatchType pType, byte subType)
{
	/*if (m_RoleInfo.LotteryFishSum >= g_FishServer.GetFishConfig().GetLotteryConfig().MaxLotteryFishSum)
		return;*/
	HashMap<BYTE, DWORD>::iterator Iter= g_FishServer.GetFishConfig().GetLotteryConfig().FishScore.find(FishTypeID);
	if (Iter == g_FishServer.GetFishConfig().GetLotteryConfig().FishScore.end())
		return;
	else
	{
		CRole* pRole = g_FishServer.GetTableManager()->SearchUser(GetUserID());
		if (pRole)
		{
			if (m_RoleInfo.LotteryFishSum < 0xff)
				m_RoleInfo.LotteryFishSum++;
			WORD RateValue = 1;
			if (pType == CatchType::CATCH_LASER || pType == CatchType::CATCH_BULLET)
				RateValue = g_FishServer.GetTableManager()->GetGameConfig()->BulletMultiple(pRole->GetRateIndex());
			else if (pType == CatchType::CATCH_SKILL)
			{
				RateValue = g_FishServer.GetTableManager()->GetGameConfig()->SkillMultiple(subType);
			}
			m_RoleInfo.LotteryScore += (Iter->second * RateValue);
			m_IsNeedSave = true;
		}
		
	}
}
void CRoleEx::OnClearRoleLotteryInfo()
{
	m_RoleInfo.LotteryFishSum = 0;
	m_RoleInfo.LotteryScore = 0;
	m_IsNeedSave = true;
}
bool CRoleEx::ChangeRoleTotalFishGlobelSum(int AddSum)
{
	if (AddSum == 0)
		return true;
	m_RoleServerInfo.TotalFishGlobelSum += AddSum;
	m_IsNeedSave = true;
	return true;
}
void CRoleEx::OnChangeRoleSecPassword(DWORD Crc1, DWORD Crc2, DWORD Crc3, bool IsSaveToDB)
{
	if (
		m_RoleServerInfo.SecPasswordCrc1 == Crc1 &&
		m_RoleServerInfo.SecPasswordCrc2 == Crc2 &&
		m_RoleServerInfo.SecPasswordCrc3 == Crc3
		)
		return;

	m_RoleServerInfo.SecPasswordCrc1 = Crc1;
	m_RoleServerInfo.SecPasswordCrc2 = Crc2;
	m_RoleServerInfo.SecPasswordCrc3 = Crc3;
	if (IsSaveToDB)
	{
		DBR_Cmd_SaveRoleSecPassword msg;
		SetMsgInfo(msg, DBR_SaveRoleSecPassword, sizeof(DBR_Cmd_SaveRoleSecPassword));
		msg.dwUserID = GetUserID();
		msg.SecPasswordCrc1 = Crc1;
		msg.SecPasswordCrc2 = Crc2;
		msg.SecPasswordCrc3 = Crc3;
		g_FishServer.SendNetCmdToSaveDB(&msg);
	}
}
//bool CRoleEx::SetRoleMonthCardInfo(BYTE MonthCardID)
//{
//	time_t pNow = time(null);
//	if (MonthCardID == 0)
//	{
//		if (m_RoleInfo.MonthCardID == 0)
//			return true;
//		//清空玩家月卡数据
//		m_RoleInfo.MonthCardID = 0;
//		m_RoleInfo.MonthCardEndTime = 0;
//
//		DBR_Cmd_SaveRoleMonthCardInfo msgDB;
//		SetMsgInfo(msgDB, DBR_SaveRoleMonthCardInfo, sizeof(DBR_Cmd_SaveRoleMonthCardInfo));
//		msgDB.UserID = GetUserID();
//		msgDB.MonthCardID = 0;
//		msgDB.MonthCardEndTime = 0;
//		g_FishServer.SendNetCmdToSaveDB(&msgDB);
//		//发送命令到客户端
//		LC_Cmd_ChangeRoleMonthCardInfo msg;
//		SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleMonthCardInfo), sizeof(LC_Cmd_ChangeRoleMonthCardInfo));
//		msg.MonthCardID = 0;
//		msg.MonthCardEndTime = 0;
//		SendDataToClient(&msg);
//
//		LC_Cmd_TableChangeRoleIsInMonthCard msgTable;
//		SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleIsInMonthCard), sizeof(LC_Cmd_TableChangeRoleIsInMonthCard));
//		msgTable.dwDestUserID = GetUserID();
//		msgTable.IsInMonthCard = false;
//		SendDataToTable(&msgTable);
//
//		CC_Cmd_ChangeRoleIsInMonthCard msgCenter;
//		SetMsgInfo(msgCenter, GetMsgType(Main_Role, CC_ChangeRoleIsInMonthCard), sizeof(CC_Cmd_ChangeRoleIsInMonthCard));
//		msgCenter.dwUserID = GetUserID();
//		msgCenter.IsInMonthCard = false;
//		SendDataToCenter(&msgCenter);
//
//		return true;
//	}
//	else if (m_RoleInfo.MonthCardID != MonthCardID)
//	{
//		if (m_RoleInfo.MonthCardID != 0 && pNow < m_RoleInfo.MonthCardEndTime)
//		{
//			//玩家处于月卡状态 并且 当前月卡不是 使用的月卡
//			return false;
//		}
//		//替换新的ID
//		//设置月卡的信息
//		HashMap<BYTE, tagMonthCardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(MonthCardID);
//		if (Iter == g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
//		{
//			ASSERT(false);
//			return false;
//		}
//		//可以设置玩家月卡数据了
//		m_RoleInfo.MonthCardID = MonthCardID;
//		m_RoleInfo.MonthCardEndTime = pNow + Iter->second.UseLastMin * 60;//月卡结束的时间
//		//GetRoleLauncherManager().OnMonthCardChange(0, MonthCardID);//获得新的月卡
//		//发送命令到数据库
//		DBR_Cmd_SaveRoleMonthCardInfo msgDB;
//		SetMsgInfo(msgDB, DBR_SaveRoleMonthCardInfo, sizeof(DBR_Cmd_SaveRoleMonthCardInfo));
//		msgDB.UserID = GetUserID();
//		msgDB.MonthCardID = MonthCardID;
//		msgDB.MonthCardEndTime = m_RoleInfo.MonthCardEndTime;
//		g_FishServer.SendNetCmdToSaveDB(&msgDB);
//		//发送命令到客户端
//		LC_Cmd_ChangeRoleMonthCardInfo msg;
//		SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleMonthCardInfo), sizeof(LC_Cmd_ChangeRoleMonthCardInfo));
//		msg.MonthCardID = MonthCardID;
//		msg.MonthCardEndTime = m_RoleInfo.MonthCardEndTime;
//		SendDataToClient(&msg);
//
//		LC_Cmd_TableChangeRoleIsInMonthCard msgTable;
//		SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleIsInMonthCard), sizeof(LC_Cmd_TableChangeRoleIsInMonthCard));
//		msgTable.dwDestUserID = GetUserID();
//		msgTable.IsInMonthCard = true;
//		SendDataToTable(&msgTable);
//
//		CC_Cmd_ChangeRoleIsInMonthCard msgCenter;
//		SetMsgInfo(msgCenter, GetMsgType(Main_Role, CC_ChangeRoleIsInMonthCard), sizeof(CC_Cmd_ChangeRoleIsInMonthCard));
//		msgCenter.dwUserID = GetUserID();
//		msgCenter.IsInMonthCard = true;
//		SendDataToCenter(&msgCenter);
//
//		return true;
//	}
//	else if (m_RoleInfo.MonthCardID == MonthCardID)
//	{
//		//叠加 
//		HashMap<BYTE, tagMonthCardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(MonthCardID);
//		if (Iter == g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
//		{
//			ASSERT(false);
//			return false;
//		}
//		//可以设置玩家月卡数据了
//		m_RoleInfo.MonthCardID = MonthCardID;
//		m_RoleInfo.MonthCardEndTime = max(pNow, m_RoleInfo.MonthCardEndTime) + Iter->second.UseLastMin * 60;//月卡结束的时间
//		//GetRoleLauncherManager().OnMonthCardChange(0, MonthCardID);//获得新的月卡
//		//发送命令到数据库
//		DBR_Cmd_SaveRoleMonthCardInfo msgDB;
//		SetMsgInfo(msgDB, DBR_SaveRoleMonthCardInfo, sizeof(DBR_Cmd_SaveRoleMonthCardInfo));
//		msgDB.UserID = GetUserID();
//		msgDB.MonthCardID = MonthCardID;
//		msgDB.MonthCardEndTime = m_RoleInfo.MonthCardEndTime;
//		g_FishServer.SendNetCmdToSaveDB(&msgDB);
//		//发送命令到客户端
//		LC_Cmd_ChangeRoleMonthCardInfo msg;
//		SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleMonthCardInfo), sizeof(LC_Cmd_ChangeRoleMonthCardInfo));
//		msg.MonthCardID = MonthCardID;
//		msg.MonthCardEndTime = m_RoleInfo.MonthCardEndTime;
//		SendDataToClient(&msg);
//
//		LC_Cmd_TableChangeRoleIsInMonthCard msgTable;
//		SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleIsInMonthCard), sizeof(LC_Cmd_TableChangeRoleIsInMonthCard));
//		msgTable.dwDestUserID = GetUserID();
//		msgTable.IsInMonthCard = true;
//		SendDataToTable(&msgTable);
//
//		CC_Cmd_ChangeRoleIsInMonthCard msgCenter;
//		SetMsgInfo(msgCenter, GetMsgType(Main_Role, CC_ChangeRoleIsInMonthCard), sizeof(CC_Cmd_ChangeRoleIsInMonthCard));
//		msgCenter.dwUserID = GetUserID();
//		msgCenter.IsInMonthCard = true;
//		SendDataToCenter(&msgCenter);
//
//		return true;
//	}
//	else
//	{
//		ASSERT(false);
//		return false;
//	}
//	return false;
//}
//bool CRoleEx::GetRoleMonthCardReward()
//{
//	//玩家试图领取今天的月卡物品的奖励
//	time_t pNow = time(null);
//	if (m_RoleInfo.MonthCardID == 0 || pNow > m_RoleInfo.MonthCardEndTime)//非月卡用于 无法领取月卡物品
//		return false;
//	if (g_FishServer.GetFishConfig().GetFishUpdateConfig().IsChangeUpdate(m_RoleInfo.GetMonthCardRewardTime, pNow))
//	{
//		HashMap<BYTE, tagMonthCardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.find(m_RoleInfo.MonthCardID);
//		if (Iter == g_FishServer.GetFishConfig().GetMonthCardConfig().MonthCardMap.end())
//			return false;
//		WORD RewardID = Iter->second.OnceDayRewardID;
//		OnAddRoleRewardByRewardID(RewardID, TEXT("领取月卡礼包奖励"));
//
//		DBR_Cmd_SaveRoleGetMonthCardRewardTime msg;
//		SetMsgInfo(msg, DBR_SaveRoleGetMonthCardRewardTime, sizeof(DBR_Cmd_SaveRoleGetMonthCardRewardTime));
//		msg.UserID = GetUserID();
//		msg.LogTime = pNow;
//		g_FishServer.SendNetCmdToSaveDB(&msg);
//		return true;
//	}
//	else
//	{
//		return false;//今天已经领取过了 无法在领取了
//	}
//}
//bool CRoleEx::ChangeRoleRateValue(BYTE AddRateIndex)
//{
//	//玩家试图开启一个新的倍率
//	BYTE UpperRate = min(0, AddRateIndex - 1);
//	if (!int256Handle::GetBitStates(m_RoleInfo.RateValue, UpperRate))
//		return false;
//	if (int256Handle::GetBitStates(m_RoleInfo.RateValue, AddRateIndex))//已经开启无须再次开启
//		return true;
//	//开启新的
//	int256Handle::SetBitStates(m_RoleInfo.RateValue, AddRateIndex, true);
//	LC_Cmd_ChangeRoleRateValue msg;
//	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleRateValue), sizeof(LC_Cmd_ChangeRoleRateValue));
//	msg.RateValue = m_RoleInfo.RateValue;
//	SendDataToClient(&msg);
//	//发送命令到数据库去
//	DBR_Cmd_SaveRoleRateValue msgDB;
//	SetMsgInfo(msgDB, DBR_SaveRoleRateValue, sizeof(DBR_Cmd_SaveRoleRateValue));
//	msgDB.UserID = GetUserID();
//	msgDB.RateValue = m_RoleInfo.RateValue;
//	g_FishServer.SendNetCmdToSaveDB(&msgDB);
//	return true;
//}
//bool CRoleEx::ChangeRoleVipLevel(BYTE VipLevel, bool IsInit)
//{
//	if (VipLevel == 0)
//	{
//		if (m_RoleInfo.VipLevel == 0)
//			return true;
//	}
//	else
//	{
//		if (VipLevel == m_RoleInfo.VipLevel)
//			return true;
//		HashMap<BYTE, tagVipOnce>::iterator Iter= g_FishServer.GetFishConfig().GetVipConfig().VipMap.find(VipLevel);
//		if (Iter == g_FishServer.GetFishConfig().GetVipConfig().VipMap.end())
//			return false;
//		if (m_RoleInfo.TotalRechargeSum < Iter->second.NeedRechatgeRMBSum)
//			return false;
//	}
//	if (!IsInit)
//		GetRoleLauncherManager().OnVipLevelChange(m_RoleInfo.VipLevel, VipLevel);//初始化的时候 无须修改
//	m_RoleInfo.VipLevel = VipLevel;
//
//	DBR_Cmd_SaveRoleVipLevel msgDB;
//	SetMsgInfo(msgDB, DBR_SaveRoleVipLevel, sizeof(DBR_Cmd_SaveRoleVipLevel));
//	msgDB.VipLevel = m_RoleInfo.VipLevel;
//	msgDB.UserID = GetUserID();
//	g_FishServer.SendNetCmdToSaveDB(&msgDB);
//
//	if (!IsInit)
//	{
//		//发送命令到中央服务器告诉玩家 VIP等级变化了
//		CC_Cmd_ChangeRoleVipLevel msgCenter;
//		SetMsgInfo(msgCenter, GetMsgType(Main_Role, CC_ChangeRoleVipLevel), sizeof(CC_Cmd_ChangeRoleVipLevel));
//		msgCenter.dwUserID = GetUserID();
//		msgCenter.VipLevel = m_RoleInfo.VipLevel;
//		SendDataToCenter(&msgCenter);
//
//		LC_Cmd_ChangeRoleVipLevel msg;
//		SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleVipLevel), sizeof(LC_Cmd_ChangeRoleVipLevel));
//		msg.VipLevel = m_RoleInfo.VipLevel;
//		SendDataToClient(&msg);
//
//		//发送同桌子上的玩家
//		LC_Cmd_TableChangeRoleVipLevel msgTable;
//		SetMsgInfo(msgTable, GetMsgType(Main_Table, LC_TableChangeRoleVipLevel), sizeof(LC_Cmd_TableChangeRoleVipLevel));
//		msgTable.dwDestUserID = GetUserID();
//		msgTable.VipLevel = m_RoleInfo.VipLevel;
//		SendDataToTable(&msgTable);
//	}
//	return true;
//}
//bool CRoleEx::IsCanUseRateIndex(BYTE RateIndex)
//{
//	return int256Handle::GetBitStates(m_RoleInfo.RateValue, RateIndex);//判断指定倍率是否可以使用
//}
bool CRoleEx::ChangeRoleCashSum(int AddSum)
{
	//添加兑换次数
	if (AddSum == 0)
		return true;
	if (AddSum < 0 && (m_RoleInfo.CashSum + AddSum) < 0)
		return false;
	m_RoleInfo.CashSum += AddSum;
	if (AddSum > 0)
		m_RoleInfo.TotalCashSum += AddSum;

	//发送命令到客户端 已经 数据库 无须发送到中央和 桌子上
	DBR_Cmd_SaveRoleCashSum msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleCashSum, sizeof(DBR_Cmd_SaveRoleCashSum));
	msgDB.UserID = m_RoleInfo.dwUserID;
	msgDB.CashSum = m_RoleInfo.CashSum;
	msgDB.TotalCashSum = m_RoleInfo.TotalCashSum;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	//发送到客户端去
	LC_Cmd_ChangeRoleCashSum msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleCashSum), sizeof(LC_Cmd_ChangeRoleCashSum));
	msg.CashSum = m_RoleInfo.CashSum;
	msg.TotalCashSum = m_RoleInfo.TotalCashSum;
	SendDataToClient(&msg);

	return true;
}
bool CRoleEx::ChangeRoleShareStates(bool States)
{
	if (m_RoleInfo.bShareStates == States)
		return false;
	m_RoleInfo.bShareStates = States;

	DBR_Cmd_ChangeRoleShareStates msgDB;
	SetMsgInfo(msgDB, DBR_ChangeRoleShareStates, sizeof(DBR_Cmd_ChangeRoleShareStates));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.States = m_RoleInfo.bShareStates;
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	LC_Cmd_ChangeRoleShareStates msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_ChangeRoleShareStates), sizeof(LC_Cmd_ChangeRoleShareStates));
	msg.ShareStates = m_RoleInfo.bShareStates;
	SendDataToClient(&msg);

	return true;
}
void CRoleEx::AddRoleProtectSum()
{
	time_t pNow = time(null);
	m_RoleServerInfo.RoleProtectLogTime = pNow;
	m_RoleServerInfo.RoleProtectSum += 1;
	m_IsNeedSave = true;
	{
		m_RoleInfo.benefitCount = m_RoleServerInfo.RoleProtectSum;
		m_RoleInfo.benefitTime = (DWORD)m_RoleServerInfo.RoleProtectLogTime;
	}
}
DWORD CRoleEx::GetScore()
{
	return m_RoleInfo.dwGlobeNum;
}

DWORD CRoleEx::GetExp()
{
	return m_RoleInfo.dwExp;
}
WORD  CRoleEx::GetLevel()
{
	return m_RoleInfo.wLevel;
}
DWORD CRoleEx::GetProduction()
{
	return m_RoleInfo.dwProduction;
}
DWORD CRoleEx::GetGameTime()
{
	return m_RoleInfo.dwGameTime;
}
bool CRoleEx::IsOnceDayOnline()
{
	if (g_FishServer.GetFishConfig().GetFishUpdateConfig().IsChangeUpdate(m_LastOnLineTime, m_LogonTime))
		return false;
	else
		return true;
}
bool CRoleEx::IsOnceMonthOnline()
{
	time_t DestOnLineTime = m_LastOnLineTime - g_FishServer.GetFishConfig().GetWriteSec();
	if (DestOnLineTime < 0)
		DestOnLineTime = 0;
	time_t DestLogonTime = m_LogonTime - g_FishServer.GetFishConfig().GetWriteSec();
	if (DestLogonTime < 0)
		DestLogonTime = 0;
	tm pLogonTime;
	errno_t Error = localtime_s(&pLogonTime, &DestOnLineTime);
	if (Error != 0)
	{
		ASSERT(false);
		return false;
	}

	tm pNowTime;
	Error = localtime_s(&pNowTime, &DestLogonTime);
	if (Error != 0)
	{
		ASSERT(false);
		return false;
	}
	return pNowTime.tm_mon == pLogonTime.tm_mon;
}
void CRoleEx::OnSaveInfoToDB()
{
	SaveAllRoleInfo(false);//平时的普通的保存 15分钟进行一次的
}
void CRoleEx::UpdateByMin(bool IsHourChange, bool IsDayChange, bool IsMonthChange, bool IsYearChange)
{
	//每分钟更新次 玩家是否需要进行保护 (金币保护
	if (!m_IsAfk && !m_IsExit && !m_IsRobot)
	{
		//GetaRoleProtect().OnUserNonGlobel();//新手保护
	}

	//判断月卡是否过期
	GetRoleMonthCard().UpdateMonthCard();//更新月卡数据

	//判断玩家最后发送子弹 或者使用技能的时间 如果大于多少分钟 未操作 让玩家离开桌子
	CRole* pTableRole = g_FishServer.GetTableManager()->SearchUser(GetUserID());
	if (pTableRole && pTableRole->IsNeedLeaveTable())
	{
		//让玩家离开桌子
		g_FishServer.GetTableManager()->OnPlayerLeaveTable(GetUserID());
		//发送命令让客户端玩家离开桌子
		LC_Cmd_LeaveTableByServer msg;
		SetMsgInfo(msg, GetMsgType(Main_Table, LC_LeaveTableByServer), sizeof(LC_Cmd_LeaveTableByServer));
		msg.IsReturnLogon = false;
		SendDataToClient(&msg);
	}

	//每分钟 保存次玩家在线的时间数据 
	if (IsDayChange)
	{
		//g_FishServer.ShowInfoToWin("玩家 天数变化 进行更新");
		ChangeRoleSendGiffSum(m_RoleInfo.SendGiffSum * -1);
		ChangeRoleCashSum(m_RoleInfo.CashSum * -1);
		ChangeRoleAcceptGiffSum(m_RoleInfo.AcceptGiffSum * -1);
		ClearRoleTaskStates();
		ChangeRoleOnlineRewardStates(0);

		m_RoleInfo.dwProduction = 0;
		m_RoleInfo.dwGameTime = 0;
		m_RoleInfo.OnlineMin = 0;//玩家每天在线的奖励
		m_LogonTimeByDay = time(NULL);//每天隔天的时候 记录时间清空

		m_IsNeedSave = true;

		m_RoleTask.OnDayChange();	//任务无须按天进行更新 特殊情况 
		m_MailManager.OnDayChange();//邮件数据
		m_RoleGiffManager.OnDayChange();//赠送数据进行更新
		m_RoleRank.UpdateByDay();//按天进行更新
		m_RoleRelationRequest.OnUpdateByDay();

		ResetPerDay();

		//当天发生变化的时候
		SYSTEMTIME time;
		GetLocalTime(&time);
		LC_Cmd_DayChange msg;
		SetMsgInfo(msg,GetMsgType(Main_Role, LC_DayChange), sizeof(LC_Cmd_DayChange));
		msg.Year = (BYTE)(time.wYear - 2000);
		msg.Month = (BYTE)time.wMonth;
		msg.Day = (BYTE)time.wDay;
		msg.Hour = (BYTE)time.wHour;
		msg.Min = (BYTE)time.wMinute;
		msg.Sec = (BYTE)time.wSecond;
		msg.IsDayUpdate = IsDayChange;
		SendDataToClient(&msg);
	}
	if (IsMonthChange)
	{
		//g_FishServer.ShowInfoToWin("玩家 月份变化 进行更新");

		ChangeRoleCheckData(0);//清空签到数据
	}
	m_ItemManager.OnUpdateByMin(IsHourChange, IsDayChange, IsMonthChange, IsYearChange);
}
void CRoleEx::OnHandleEvent(bool IsUpdateTask, bool IsUpdateAction, bool IsUpdateAchievement,BYTE EventID, DWORD BindParam, DWORD Param)
{
	if (IsUpdateTask)
		m_RoleTask.OnHandleEvent(EventID, BindParam, Param);
	if (IsUpdateAction)
		m_RoleAchievement.OnHandleEvent(EventID, BindParam, Param);
	if (IsUpdateAchievement)
		m_RoleActionManager.OnHandleEvent(EventID, BindParam, Param);
}
void CRoleEx::OnAddRoleRewardByRewardID(WORD RewardID, const TCHAR* pStr,DWORD RewardSum)
{
	//直接给予玩家奖励
	HashMap<WORD, tagRewardOnce>::iterator Iter = g_FishServer.GetFishConfig().GetFishRewardConfig().RewardMap.find(RewardID);
	if (Iter == g_FishServer.GetFishConfig().GetFishRewardConfig().RewardMap.end())
		return;
	//获得奖励的配置	
	if (!Iter->second.RewardItemVec.empty())
	{
		vector<tagItemOnce>::iterator IterItem = Iter->second.RewardItemVec.begin();
		for (; IterItem != Iter->second.RewardItemVec.end(); ++IterItem)
		{
			tagItemOnce pOnce = *IterItem;
			pOnce.ItemSum *= RewardSum;
			GetItemManager().OnAddUserItem(pOnce);
		}
	}
	//将玩家领取奖励 的事件存储到LOG里面去
	g_DBLogManager.LogToDB(GetUserID(), LT_Reward, RewardID, RewardSum, pStr, SendLogDB);//奖励数量
}
void CRoleEx::SetRoleExLeaveServer()
{
	//1.玩家进入暂离状态
	SetAfkStates(true);
	//2.如果玩家在房间里面 将玩家大招的能量清空
	CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_RoleInfo.dwUserID);
	if (pRole)
	{
		pRole->SetRoleIsCanSendTableMsg(false);//不可以向客户端发送命令
	}
}

void CRoleEx::ResetRoleInfoToClient()
{
	ResetClientInfo();//将一些管理器的数据刷新掉

	LC_Cmd_ResetRoleInfo msg;
	SetMsgInfo(msg,GetMsgType(Main_Role, LC_ResetRoleInfo), sizeof(LC_Cmd_ResetRoleInfo));
	msg.RoleInfo = m_RoleInfo;
	msg.OperateIP = g_FishServer.GetOperateIP();
	SendDataToClient(&msg);

	CRole* pRole= g_FishServer.GetTableManager()->SearchUser(m_RoleInfo.dwUserID);
	if (pRole)
	{
		//重连清场
		NetCmdClearScene cmd;
		cmd.SetCmdSize(sizeof(cmd));
		cmd.SetCmdType(CMD_CLEAR_SCENE);
		cmd.ClearType = 2;
		SendDataToClient(&cmd);

		pRole->SetRoleIsCanSendTableMsg(true);//可以发送命令
		//玩家在桌子里
		pRole->GetChestManager().SendAllChestToClient();//将当前在桌子里的宝箱发送到客户端去
		ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(GetGameSocketID());
		if (pClient)
		{
			pClient->IsInScene = true;
		}
		g_FishServer.GetTableManager()->ResetTableInfo(m_RoleInfo.dwUserID);
	}
}
void CRoleEx::ResetClientInfo()
{
	m_RelationManager.ResetClientInfo();
	m_ItemManager.ResetClientInfo();
	m_MailManager.ResetClientInfo();
	m_RoleTask.ResetClientInfo();
	m_RoleAchievement.ResetClientInfo();
	m_RoleTitleManager.ResetClientInfo();
	m_RoleIDEntity.ResetClientInfo();
	m_RoleActionManager.ResetClientInfo();
	m_RoleGiffManager.ResetClientInfo();
	//m_RoleGameData.ResetClientInfo();
}
bool CRoleEx::SaveAllRoleInfo(bool IsExit)
{
	//WORD OnLineMin = ConvertDWORDToWORD(GetRoleOnlineSec() / 60);//获取玩家在线的分钟
	//if (m_RoleInfo.OnlineMin != OnLineMin)
	//{
	//	m_RoleInfo.OnlineMin = OnLineMin;
	//	//m_IsNeedSave = true;
	//	DBR_Cmd_SaveRoleOnlineMin msgDB;
	//	SetMsgInfo(msgDB, DBR_SaveRoleOnlineMin, sizeof(DBR_Cmd_SaveRoleOnlineMin));
	//	msgDB.dwUserID = m_RoleInfo.dwUserID;
	//	msgDB.OnLineMin = m_RoleInfo.OnlineMin;
	//	g_FishServer.SendNetCmdToSaveDB(&msgDB);
	//}

	//每次保存的时候 保存下玩家的在线时间
	DBR_Cmd_SaveRoleOnlineMin msgDB;
	SetMsgInfo(msgDB, DBR_SaveRoleOnlineMin, sizeof(DBR_Cmd_SaveRoleOnlineMin));
	msgDB.dwUserID = m_RoleInfo.dwUserID;
	msgDB.OnLineMin = ConvertDWORDToWORD(GetRoleOnlineSec() / 60);//保存玩家的在线分钟
	g_FishServer.SendNetCmdToSaveDB(&msgDB);

	if (!m_IsNeedSave && !IsExit)//无须保存
	{
		SetIsExit(IsExit);
		//m_IsExit = IsExit;
		return false;
	}
	if (IsExit)
	{
		//g_FishServer.ShowInfoToWin("玩家进行离线保存");
	}
	//std::cout << "玩家 数据进行保存\n";
	//保存玩家的全部的数据
	SetIsExit(IsExit);
	vector<tagRoleTaskInfo> pTaskVec;
	GetRoleTaskManager().GetAllNeedSaveTask(pTaskVec);

	vector<tagRoleAchievementInfo> pAchievementVec;
	GetRoleAchievementManager().GetAllNeedSaveAchievement(pAchievementVec);

	vector<tagRoleActionInfo> pActionVec;
	GetRoleActionManager().GetAllNeedSaveAction(pActionVec);

	DWORD EventSize = pTaskVec.size() + pAchievementVec.size() + pActionVec.size();
	DWORD PageSize = sizeof(DBR_Cmd_SaveRoleAllInfo)+sizeof(SaveEventInfo)*(EventSize - 1);
	//创建命令 10.1
	DBR_Cmd_SaveRoleAllInfo * msg = (DBR_Cmd_SaveRoleAllInfo*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return false;
	}
	msg->SetCmdType(DBR_SaveRoleAllInfo);
	//往命令里面填充数据
	msg->dwUserID = m_RoleInfo.dwUserID;
	msg->dwExp = m_RoleInfo.dwExp;
	msg->dwGlobeNum = m_RoleInfo.dwGlobeNum;
	msg->dwProduction = m_RoleInfo.dwProduction;
	msg->dwGameTime = m_RoleInfo.dwGameTime;
	msg->IsNeedResult = IsExit;
	msg->GameData = GetRoleGameData().GetGameData();
	msg->OnlineMin = ConvertDWORDToWORD(GetRoleOnlineSec() / 60);//保存玩家的在线分钟
	msg->ClientIP = m_RoleInfo.ClientIP;
	msg->dwLotteryScore = m_RoleInfo.LotteryScore;//抽奖积分
	msg->bLotteryFishSum = m_RoleInfo.LotteryFishSum;//捕获的奖金鱼的数量
	msg->RoleServerInfo = m_RoleServerInfo;//保存玩家服务器属性 这些属性不发送客户端的 也无须即可保存

	int i = 0;
	vector<tagRoleTaskInfo>::iterator IterTask = pTaskVec.begin();
	for (; IterTask != pTaskVec.end(); ++IterTask, ++i)
	{
		msg->Array[i].InfoStates = 1;
		msg->Array[i].EventInfo.TaskInfo = *IterTask;
	}
	vector<tagRoleAchievementInfo>::iterator IterAchievement = pAchievementVec.begin();
	for (; IterAchievement != pAchievementVec.end(); ++IterAchievement, ++i)
	{
		msg->Array[i].InfoStates = 2;
		msg->Array[i].EventInfo.AchievementInfo = *IterAchievement;
	}
	vector<tagRoleActionInfo>::iterator IterAction = pActionVec.begin();
	for (; IterAction != pActionVec.end(); ++IterAction, ++i)
	{
		msg->Array[i].InfoStates = 3;
		msg->Array[i].EventInfo.ActionInfo = *IterAction;
	}
	//分包发送
	std::vector<DBR_Cmd_SaveRoleAllInfo*> pVec;
	SqlitMsg(msg, PageSize, EventSize,false, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<DBR_Cmd_SaveRoleAllInfo*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			g_FishServer.SendNetCmdToSaveDB(*Iter);//发送到保存数据库去 最终保存数据
			free(*Iter);
		}
		pVec.clear();
	}
	//命令发送完毕后 
	m_IsNeedSave = false;
	return true;
}
void CRoleEx::SetAfkStates(bool States) 
{ 
	if (m_IsAfk == States)
		return;
	m_IsAfk = States; 
	ChangeRoleIsOnline(!States); 
}
void CRoleEx::SetIsExit(bool States)
{ 
	if (m_IsExit == States)
		return;
	m_IsExit = States; 
	ChangeRoleIsOnline(!States);
}
void CRoleEx::ResetPerDay()
{
	m_RoleInfo.benefitCount = m_RoleServerInfo.RoleProtectSum = 0;
	m_RoleInfo.benefitTime = 0;
	m_RoleServerInfo.RoleProtectLogTime = 0;
}
void CRoleEx::SendClientOpenShareUI()
{
	LC_Cmd_OpenShareUI msg;
	SetMsgInfo(msg, GetMsgType(Main_Role, LC_OpenShareUI), sizeof(LC_Cmd_OpenShareUI));
	SendDataToClient(&msg);
}