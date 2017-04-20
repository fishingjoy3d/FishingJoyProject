#include "stdafx.h"
#include "PhoneLogon.h"
#include "FishServer.h"
#include "..\CommonFile\FishServerConfig.h"

const static char PhoneLogonSMSInfo[] = "您的手机登陆密码为 %u 【乐多游戏中心】";

PhoneLogon::PhoneLogon()
{
	m_LogonMap.clear();
}
PhoneLogon::~PhoneLogon()
{
	m_LogonMap.clear();
}
bool PhoneLogon::OnAddPhoneLogon(ServerClientData* pData, UINT64 PhoneNumber, BYTE GameConfigID, DWORD ClientID)
{
	if (!pData)
	{
		ASSERT(false);
		return false;
	}

	DBR_Cmd_CheckPhone msg;
	SetMsgInfo(msg, DBR_CheckPhone, sizeof(DBR_Cmd_CheckPhone));
	msg.ClientID = ClientID;
	msg.IsBindOrLogon = false;
	msg.dwUserID = GameConfigID;
	msg.Phone = PhoneNumber;
	g_FishServer.SendNetCmdToDB(&msg);
	return true;
}
void PhoneLogon::OnCheckPhoneResult(DBO_Cmd_CheckPhone* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}
	
	ServerClientData* pData = g_FishServer.GetUserClientDataByIndex(static_cast<BYTE>(pMsg->dwUserID));
	if (!pData)
	{
		ASSERT(false);
		return;
	}

	if (pMsg->Result) //需要取反 
	{
		OL_Cmd_PhoneLogon msg;
		SetMsgInfo(msg, GetMsgType(Main_Logon, OL_PhoneLogon), sizeof(OL_Cmd_PhoneLogon));
		msg.PhoneNumber = pMsg->Phone;
		msg.ClientID = pMsg->ClientID;
		msg.ErrorID = PLE_LogonExists;
		g_FishServer.SendNetCmdToClient(pData, &msg);
		return;
	}

	HashMap<UINT64, PhoneLogonOnceInfo>::iterator Iter = m_LogonMap.find(pMsg->Phone);
	if (Iter != m_LogonMap.end())
	{
		//客户端重新获取验证码数据
		m_LogonMap.erase(Iter);
	}
	PhoneLogonOnceInfo pInfo;
	pInfo.PhoneNumber = pMsg->Phone;
	pInfo.GameConfigID = static_cast<BYTE>(pMsg->dwUserID);
	pInfo.ClientID = pMsg->ClientID;
	pInfo.LogonPassword = 0;
	m_LogonMap.insert(HashMap<UINT64, PhoneLogonOnceInfo>::value_type(pInfo.PhoneNumber,pInfo));
}
void PhoneLogon::OnUpdate(DWORD dwTimer)
{
	if (m_LogonMap.empty())
		return;
	HashMap<UINT64, PhoneLogonOnceInfo>::iterator Iter = m_LogonMap.begin();
	for (; Iter != m_LogonMap.end(); ++Iter)
	{
		if (Iter->second.LogonPassword == 0)
		{
			//未处理的 我们进行处理
			OnHandlePhoneLogon(Iter->second);
		}
	}
}
void PhoneLogon::OnHandlePhoneLogon(PhoneLogonOnceInfo& pInfo)
{
	PhoneSMSOnceInfo pSMS;
	pSMS.OnlyID = 0;
	pSMS.PhoneNumberVec.push_back(pInfo.PhoneNumber);
	pSMS.Type = ST_PhoneLogon;
	DWORD RankValue = 123456;//如果测试 表示 验证码为 123456
	if (!g_FishServerConfig.GetIsOperateTest())
		RankValue = RandUInt() % 899999 + 100000;//随机码
	sprintf_s(pSMS.SMSInfo, CountArray(pSMS.SMSInfo), PhoneLogonSMSInfo, RankValue);
	pInfo.HandleOnlyID = g_FishServer.GetPhoneSMSMananger().OnAddSMSEvent(pSMS);
	pInfo.LogonPassword = RankValue;
}
void PhoneLogon::OnHandleSMSEvent(PhoneSMSOnceInfo pEvent, bool Result)
{
	//异步调用
	//pEvent.OnlyID
	//通过OnlyID 获得 手机ID
	HashMap<UINT64, PhoneLogonOnceInfo>::iterator Iter = m_LogonMap.begin();
	for (; Iter != m_LogonMap.end(); ++Iter)
	{
		if (Iter->second.HandleOnlyID == pEvent.OnlyID)
		{
			ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(Iter->second.GameConfigID);
			if (!pClient)
			{
				m_LogonMap.erase(Iter);
				ASSERT(false);
				return;
			}
			if (Result)
			{
				//手机验证码发送成功 进行登陆
				OL_Cmd_PhoneLogon msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, OL_PhoneLogon), sizeof(OL_Cmd_PhoneLogon));
				msg.PhoneNumber = Iter->second.PhoneNumber;
				msg.ClientID = Iter->second.ClientID;
				msg.ErrorID = PLE_Success;
				g_FishServer.SendNetCmdToClient(pClient, &msg);
			}
			else
			{
				//手机验证码发送失败 无法登陆
				OL_Cmd_PhoneLogon msg;
				SetMsgInfo(msg, GetMsgType(Main_Logon, OL_PhoneLogon), sizeof(OL_Cmd_PhoneLogon));
				msg.PhoneNumber = Iter->second.PhoneNumber;
				msg.ClientID = Iter->second.ClientID;
				msg.ErrorID = PLE_LogonExists;
				g_FishServer.SendNetCmdToClient(pClient, &msg);

				m_LogonMap.erase(Iter);
			}
			return;
		}
	}
}
void PhoneLogon::OnCheckPhoneLogonPassword(LO_Cmd_PhoneLogonCheck* pMsg)
{
	if (!pMsg)
	{
		ASSERT(false);
		return;
	}

	HashMap<UINT64, PhoneLogonOnceInfo>::iterator Iter = m_LogonMap.find(pMsg->PhoneNumber);
	if (Iter == m_LogonMap.end())
	{
		ASSERT(false);
		return;
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(Iter->second.GameConfigID);
	if (!pClient)
	{
		m_LogonMap.erase(Iter);
		ASSERT(false);
		return;
	}
	if (Iter->second.LogonPassword != pMsg->Password || Iter->first != pMsg->PhoneNumber)
	{
		ASSERT(false);
		//提示客户端 验证码错误
		OL_Cmd_PhoneLogonCheck msg;
		SetMsgInfo(msg, GetMsgType(Main_Logon, OL_PhoneLogonCheck), sizeof(OL_Cmd_PhoneLogonCheck));
		msg.ClientID = pMsg->ClientID;
		msg.Result = false;
		msg.ErrorID = PLE_PasswordError;
		msg.ClientIP = pMsg->ClientIP;
		msg.PhoneNumber = pMsg->PhoneNumber;
		TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
		msg.Crc1 = pMsg->Crc1;
		msg.Crc2 = pMsg->Crc2;
		msg.Crc3 = pMsg->Crc3;
		msg.IsFirOrSec = pMsg->IsFirOrSec;
		g_FishServer.SendNetCmdToClient(pClient, &msg);
		return;
	}
	//客户端验证成功 发送命令到GameServer去
	OL_Cmd_PhoneLogonCheck msg;
	SetMsgInfo(msg, GetMsgType(Main_Logon, OL_PhoneLogonCheck), sizeof(OL_Cmd_PhoneLogonCheck));
	msg.ClientID = pMsg->ClientID;//使用新的ID
	msg.Result = true;
	msg.ErrorID = PLE_Success;
	msg.ClientIP = pMsg->ClientIP;
	msg.PhoneNumber = pMsg->PhoneNumber;
	TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
	msg.Crc1 = pMsg->Crc1;
	msg.Crc2 = pMsg->Crc2;
	msg.Crc3 = pMsg->Crc3;
	msg.IsFirOrSec = pMsg->IsFirOrSec;
	g_FishServer.SendNetCmdToClient(pClient, &msg);

	m_LogonMap.erase(Iter);
	return;
}