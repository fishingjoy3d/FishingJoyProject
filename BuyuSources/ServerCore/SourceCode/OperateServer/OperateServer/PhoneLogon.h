#pragma once
#include "PhoneSMSManager.h"

struct PhoneLogonOnceInfo
{
	UINT64  PhoneNumber;//手机号
	DWORD   LogonPassword;//登陆的密码 随机产生的
	DWORD	HandleOnlyID;
	BYTE    GameConfigID;
	DWORD   ClientID;

	PhoneLogonOnceInfo()
	{
		PhoneNumber = 0;
		LogonPassword = 0;
		GameConfigID = 0;
		ClientID = 0;
		HandleOnlyID = 0;
	}
};

class PhoneLogon //使用手机号码进行登陆处理
{
public:
	PhoneLogon();
	virtual ~PhoneLogon();

	bool OnAddPhoneLogon(ServerClientData* pData,UINT64 PhoneNumber, BYTE GameConfigID, DWORD ClientID);//处理指定玩家登陆的状态
	void OnCheckPhoneResult(DBO_Cmd_CheckPhone* pMsg);

	void OnUpdate(DWORD dwTimer);

	void OnHandleSMSEvent(PhoneSMSOnceInfo pEvent, bool Result);

	void OnCheckPhoneLogonPassword(LO_Cmd_PhoneLogonCheck* pMsg);
private:
	void OnHandlePhoneLogon(PhoneLogonOnceInfo& pInfo);
private:
	HashMap<UINT64, PhoneLogonOnceInfo>  m_LogonMap;//待处理的登陆数据
};