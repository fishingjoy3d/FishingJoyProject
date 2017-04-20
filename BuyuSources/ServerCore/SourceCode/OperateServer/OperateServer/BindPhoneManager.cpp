#include "stdafx.h"
#include "BindPhoneManager.h"
#include "FishServer.h"
#include "..\CommonFile\FishServerConfig.h"

const static char PhoneSMSInfo[] = "您的手机验证码为 %u 【乐多游戏中心】";
BindPhoneManager::BindPhoneManager()
{
	m_BindPhoneMap.clear();
}
BindPhoneManager::~BindPhoneManager()
{
	m_BindPhoneMap.clear();
}
bool BindPhoneManager::CheckPhoneNumber(unsigned __int64  PhoneNumber)
{
	//检查手机号码是否正确
	return true;
}
bool BindPhoneManager::OnGetPhoneVerificationNumber(ServerClientData* pClient, GO_Cmd_GetPhoneVerificationNum* pMsg)
{
	if (!pClient || !pMsg)
	{
		ASSERT(false);
		return false;
	}
	if (!CheckPhoneNumber(pMsg->PhoneNumber))
	{
		OG_Cmd_BindPhone msg;
		SetMsgInfo(msg, GetMsgType(Main_Operate, OG_BindPhone), sizeof(OG_Cmd_BindPhone));
		msg.dwUserID = pMsg->dwUserID;
		msg.Result = false;
		msg.ErrorID = ROE_PhoneVerificationNum_PhoneError;
		msg.PhoneNumber = pMsg->PhoneNumber;
		g_FishServer.SendNetCmdToClient(pClient, &msg);
		ASSERT(false);
		return false;
	}
	//检查手机号码
	DBR_Cmd_CheckPhone msgDB;
	SetMsgInfo(msgDB, DBR_CheckPhone, sizeof(DBR_Cmd_CheckPhone));
	msgDB.dwUserID = pMsg->dwUserID;
	msgDB.Phone = pMsg->PhoneNumber;
	msgDB.ClientID = pClient->OutsideExtraData;
	msgDB.IsBindOrLogon = true;
	g_FishServer.SendNetCmdToDB(&msgDB);
	return true;
}
void BindPhoneManager::OnCheckPhoneResult(DBO_Cmd_CheckPhone* pMsg)
{
	//检查手机是否可以使用
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(ConvertDWORDToBYTE(pMsg->ClientID));
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	if (pMsg->Result)
	{
		//添加玩家的订单
		HashMap<DWORD, BindPhoneOnce>::iterator Iter = m_BindPhoneMap.find(pMsg->dwUserID);
		if (Iter != m_BindPhoneMap.end())
		{
			//订单已经存在了 我们进行处理 
			//1.如果订单 是已经发送的 并且 还在等待短信的 并且  时间大于3分钟的 过期订单 直接删除掉
			if (Iter->second.SendPhoneNumberOnlyID != 0 /*&& timeGetTime() - Iter->second.beginTime >= 120000*/)
			{
				m_BindPhoneMap.erase(Iter);
			}
			else
			{
				//已经存在的订单 无法继续提交
				OG_Cmd_BindPhone msg;
				SetMsgInfo(msg, GetMsgType(Main_Operate, OG_BindPhone), sizeof(OG_Cmd_BindPhone));
				msg.dwUserID = pMsg->dwUserID;
				msg.Result = false;
				msg.ErrorID = ROE_PhoneVerificationNum_IsExists;
				msg.PhoneNumber = pMsg->Phone;
				g_FishServer.SendNetCmdToClient(pClient, &msg);
				return;
			}
		}
		//向外部服务器提交订单
		BindPhoneOnce pOnce;
		pOnce.UserID = pMsg->dwUserID;
		pOnce.GameServerClientID = pMsg->ClientID;
		pOnce.VerificationNumber = 0;
		pOnce.PhoneNumber = pMsg->Phone;
		m_BindPhoneMap.insert(HashMap<DWORD, BindPhoneOnce>::value_type(pMsg->dwUserID, pOnce));

		//离开发送命令到客户端去
		OG_Cmd_GetPhoneVerificationNum msg;
		SetMsgInfo(msg, GetMsgType(Main_Operate, OG_GetPhoneVerificationNum), sizeof(OG_Cmd_GetPhoneVerificationNum));
		msg.dwUserID = pMsg->dwUserID;
		msg.ErrorID = ROE_PhoneVerificationNum_WriteSend;
		msg.PhoneNumber = pMsg->Phone;
		g_FishServer.SendNetCmdToClient(pClient, &msg);
		//手机号码验证完毕了 我们发送短信给玩家 短信是集合发送的
		m_SendPhoneList.UserVec.push_back(pMsg->dwUserID);
	}
	else
	{
		//手机已经被使用了
		OG_Cmd_GetPhoneVerificationNum msg;
		SetMsgInfo(msg, GetMsgType(Main_Operate, OG_GetPhoneVerificationNum), sizeof(OG_Cmd_GetPhoneVerificationNum));
		msg.dwUserID = pMsg->dwUserID;
		msg.ErrorID = ROE_PhoneVerificationNum_PhoneBeUse;
		msg.PhoneNumber = pMsg->Phone;
		g_FishServer.SendNetCmdToClient(pClient, &msg);
	}
}
void BindPhoneManager::OnGetPhoneVerificationNumberResult(bool Result,BYTE ErrorID, DWORD dwUserID)
{
	//外部服务器发送来的 获取手机验证码的结果
	HashMap<DWORD, BindPhoneOnce>::iterator Iter = m_BindPhoneMap.find(dwUserID);
	if (Iter == m_BindPhoneMap.end())
	{
		ASSERT(false);
		return;
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(ConvertDWORDToBYTE(Iter->second.GameServerClientID));
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//发送命令到GameServer去
	OG_Cmd_GetPhoneVerificationNum msg;
	SetMsgInfo(msg, GetMsgType(Main_Operate, OG_GetPhoneVerificationNum), sizeof(OG_Cmd_GetPhoneVerificationNum));
	msg.dwUserID = Iter->second.UserID;
	msg.ErrorID = ErrorID;
	msg.PhoneNumber = Iter->second.PhoneNumber;
	g_FishServer.SendNetCmdToClient(pClient, &msg);
	//if (!Result)
	//{
	//	//表示验证失败了我们移除掉数据
	//	m_BindPhoneMap.erase(Iter);
	//}
}
bool BindPhoneManager::OnBindPhontByVerificationNumber(ServerClientData* pClient, GO_Cmd_BindPhone* pMsg)
{
	if (!pClient || !pMsg)
	{
		ASSERT(false);
		return false;
	}
	HashMap<DWORD, BindPhoneOnce>::iterator Iter = m_BindPhoneMap.find(pMsg->dwUserID);
	if (Iter == m_BindPhoneMap.end())
	{
		//ASSERT(false);
		OG_Cmd_BindPhone msg;
		SetMsgInfo(msg, GetMsgType(Main_Operate, OG_BindPhone), sizeof(OG_Cmd_BindPhone));
		msg.dwUserID = pMsg->dwUserID;
		msg.ErrorID = ROE_BindPhone_BindTimeOut;
		msg.Result = false;
		msg.PhoneNumber = 0;// pMsg->PhoneNumber;
		msg.SecPasswordCrc1 = pMsg->SecPasswordCrc1;
		msg.SecPasswordCrc2 = pMsg->SecPasswordCrc2;
		msg.SecPasswordCrc3 = pMsg->SecPasswordCrc3;
		g_FishServer.SendNetCmdToClient(pClient, &msg);
		return false;
	}
	if (Iter->second.SendPhoneNumberOnlyID == 0 || Iter->second.VerificationNumber != pMsg->BindNumber)
	{
		//ASSERT(false);
		OG_Cmd_BindPhone msg;
		SetMsgInfo(msg, GetMsgType(Main_Operate, OG_BindPhone), sizeof(OG_Cmd_BindPhone));
		msg.dwUserID = pMsg->dwUserID;
		msg.ErrorID = ROE_BindPhone_BindNumError;
		msg.Result = false;
		msg.PhoneNumber = 0;// pMsg->PhoneNumber;
		msg.SecPasswordCrc1 = pMsg->SecPasswordCrc1;
		msg.SecPasswordCrc2 = pMsg->SecPasswordCrc2;
		msg.SecPasswordCrc3 = pMsg->SecPasswordCrc3;
		g_FishServer.SendNetCmdToClient(pClient, &msg);
		return false;
	}
	//验证完毕后 我们继续处理
	OG_Cmd_BindPhone msg;
	SetMsgInfo(msg, GetMsgType(Main_Operate, OG_BindPhone), sizeof(OG_Cmd_BindPhone));
	msg.dwUserID = pMsg->dwUserID;
	msg.ErrorID = ROE_BindPhone_Sucess;
	msg.Result = true;
	msg.PhoneNumber = Iter->second.PhoneNumber;//使用订单上记录的手机号码
	msg.SecPasswordCrc1 = pMsg->SecPasswordCrc1;
	msg.SecPasswordCrc2 = pMsg->SecPasswordCrc2;
	msg.SecPasswordCrc3 = pMsg->SecPasswordCrc3;
	g_FishServer.SendNetCmdToClient(pClient, &msg);
	m_BindPhoneMap.erase(pMsg->dwUserID);
	return true;
}
void BindPhoneManager::OnUpdateSendPhoneSMS(DWORD dwTimer)
{
	//5秒检查一次
	static DWORD SendPhoneSMSLogTime = 0;
	if (SendPhoneSMSLogTime == 0 || dwTimer - SendPhoneSMSLogTime >= 5000)
	{
		SendPhoneSMSLogTime = dwTimer;
		if (m_SendPhoneList.UserVec.empty())
			return;
		DWORD RankValue = 123456;//如果测试 表示 验证码为 123456
		if (!g_FishServerConfig.GetIsOperateTest())
			RankValue = RandUInt() % 899999 + 100000;//随机码
		PhoneSMSOnceInfo pEvent;
		sprintf_s(pEvent.SMSInfo, CountArray(pEvent.SMSInfo), PhoneSMSInfo, RankValue);
		pEvent.Type = ST_PhoneBind;
		vector<DWORD>::iterator Iter = m_SendPhoneList.UserVec.begin();
		for (; Iter != m_SendPhoneList.UserVec.end(); ++Iter)
		{
			DWORD dwUserID = *Iter;
			HashMap<DWORD, BindPhoneOnce>::iterator IterFind = m_BindPhoneMap.find(dwUserID);
			if (IterFind == m_BindPhoneMap.end())
				continue;
			pEvent.PhoneNumberVec.push_back(IterFind->second.PhoneNumber);
		}
		DWORD OnlyID = g_FishServer.GetPhoneSMSMananger().OnAddSMSEvent(pEvent);
		Iter = m_SendPhoneList.UserVec.begin();
		for (; Iter != m_SendPhoneList.UserVec.end(); ++Iter)
		{
			DWORD dwUserID = *Iter;
			HashMap<DWORD, BindPhoneOnce>::iterator IterFind = m_BindPhoneMap.find(dwUserID);
			if (IterFind == m_BindPhoneMap.end())
				continue;
			IterFind->second.SendPhoneNumberOnlyID = OnlyID;
			IterFind->second.VerificationNumber = RankValue;//不一定代表成功
		}
		m_SendPhoneList.UserVec.clear();
		return;
	}
}
void BindPhoneManager::OnHandleSMSEvent(PhoneSMSOnceInfo pEvent, bool Result)
{
	DWORD OnlyID = pEvent.OnlyID;
	HashMap<DWORD, BindPhoneOnce>::iterator Iter = m_BindPhoneMap.begin();
	for (; Iter != m_BindPhoneMap.end();)
	{
		if (Iter->second.SendPhoneNumberOnlyID == OnlyID)
		{
			if (Result)
			{
				OnGetPhoneVerificationNumberResult(true, ROE_PhoneVerificationNum_Sucess, Iter->first);
				++Iter;
			}
			else
			{
				OnGetPhoneVerificationNumberResult(false, ROE_BindPhone_SendSmsInfoError, Iter->first);
				Iter = m_BindPhoneMap.erase(Iter);
			}
		}
		else
			++Iter;
	}
}