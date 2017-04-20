#include "stdafx.h"
#include "PhoneSMSManager.h"
#include "..\CommonFile\FishServerConfig.h"
#include "FishServer.h"

const static char sSendPhoneVerificationNumberSucess[] = "sms&stat=100";
const static BYTE bSendPhoneVerificationNUmberSucessLength = strlen(sSendPhoneVerificationNumberSucess);

PhoneSMSManager::PhoneSMSManager()
{
	m_HandleIndex = 0;
}
PhoneSMSManager::~PhoneSMSManager()
{

}
DWORD PhoneSMSManager::OnAddSMSEvent(PhoneSMSOnceInfo pEvent)
{
	pEvent.OnlyID = (++m_HandleIndex);
	m_SMSHandleMap.insert(HashMap<DWORD, PhoneSMSOnceInfo>::value_type(pEvent.OnlyID, pEvent));
	return pEvent.OnlyID;
}
void PhoneSMSManager::OnUpdate(DWORD dwTimer)
{
	//更新函数 核心的函数
	HashMap<DWORD, PhoneSMSOnceInfo>::iterator Iter = m_SMSHandleMap.begin();
	for (; Iter != m_SMSHandleMap.end();)
	{
		if (Iter->second.LogTime == 0)
		{
			//未处理的订单
			if (g_FishServerConfig.GetIsOperateTest())
			{
				HandleSMSResult(Iter->second, true);
				Iter = m_SMSHandleMap.erase(Iter);
				continue;
			}

			bool Result = HandleSMSEvent(Iter->second);
			if (Result)
			{
				Iter->second.LogTime = dwTimer;
				++Iter;
				continue;
			}
			else
			{
				//处理订单失败了
				HandleSMSResult(Iter->second, false);
				Iter = m_SMSHandleMap.erase(Iter);
				continue;
			}
		}
		else
		{
			//已经处理的订单
			if (dwTimer - Iter->second.LogTime >= SMS_TimeOut)
			{
				//已经超时了 我们进行处理
				HandleSMSResult(Iter->second, false);
				Iter = m_SMSHandleMap.erase(Iter);
				continue;
			}
			else
			{
				++Iter;
				continue;
			}
		}
	}
}
bool PhoneSMSManager::HandleSMSEvent(PhoneSMSOnceInfo pEvent)
{
	//处理一个事件 将它即可发送出去
	OperateConfig* pConfig = g_FishServerConfig.GetOperateConfig();
	if (!pConfig)
	{
		ASSERT(false);
		return false;
	}

	string DestPass = pConfig->SMSPassword;
	DestPass += pConfig->SMSUserName;
	string Md5Str = md5(DestPass);//加密后的签名

	stringstream pStream;
	pStream << "mt/?uid=" << pConfig->SMSUserName << "&pwd=" << Md5Str.c_str() << "&mobile=";
	vector<UINT64>::iterator Iter = pEvent.PhoneNumberVec.begin();
	for (; Iter != pEvent.PhoneNumberVec.end(); ++Iter)
	{
		if (Iter != pEvent.PhoneNumberVec.begin())
			pStream << ",";
		pStream << (*Iter);
	}
	char SmsInfo[256];
	strncpy_s(SmsInfo, CountArray(SmsInfo), pEvent.SMSInfo, CountArray(pEvent.SMSInfo));
	pStream << "&content=" << UrlEncode(SmsInfo).c_str();
	WORD RequestID = g_FishServer.GetPhoneSMSID();
	UINT64* pInt = new UINT64(pEvent.OnlyID);
	if (!g_FishServer.GetHttpClient().AddRequest((UINT_PTR)pInt, RequestID, pStream.str().c_str()))
	{
		delete(pInt);
		return false;
	}
	//成功后
	return true;//不是一定成功 有可能 异步失败 
}
void PhoneSMSManager::OnSendPhoneSMSResult(UINT64 ID, char* pData, DWORD Length)
{
	if (!pData)
	{
		ASSERT(false);
		return;
	}
	DWORD OnlyID = static_cast<DWORD>(ID);//唯一ID
	HashMap<DWORD, PhoneSMSOnceInfo>::iterator Iter = m_SMSHandleMap.find(OnlyID);
	if (Iter == m_SMSHandleMap.end())
	{
		ASSERT(false);
		return;
	}
	if (Length != 0 && strlen(pData) >= bSendPhoneVerificationNUmberSucessLength && _strnicmp(pData, sSendPhoneVerificationNumberSucess, bSendPhoneVerificationNUmberSucessLength) == 0)
	{
		HandleSMSResult(Iter->second, true);
		m_SMSHandleMap.erase(Iter);
	}
	else
	{
		HandleSMSResult(Iter->second, false);
		m_SMSHandleMap.erase(Iter);
	}
}
void PhoneSMSManager::HandleSMSResult(PhoneSMSOnceInfo pEvent, bool Result)
{
	//最终订单的结果处理 
	//调用
	if (pEvent.Type == ST_PhoneBind)
	{
		//绑定手机号码
		g_FishServer.GetBindPhoneManager().OnHandleSMSEvent(pEvent, Result);
	}
	else if (pEvent.Type == ST_PhoneLogon)
	{
		//使用手机进行登陆
		g_FishServer.GetPhoneLogon().OnHandleSMSEvent(pEvent, Result);
	}
	else
	{
		ASSERT(false);
		return;
	}
}
