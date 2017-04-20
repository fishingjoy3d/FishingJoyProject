//发送手机短信的工具类
#pragma once
#define MAX_SMS_Length 128
#define SMS_TimeOut 60000
enum SMSType
{
	ST_Init = 0,
	ST_PhoneBind  = 1,
	ST_PhoneLogon = 2,
};
struct  PhoneSMSOnceInfo //一次发送事件
{
	DWORD				OnlyID;
	SMSType				Type;
	vector<UINT64>		PhoneNumberVec;//手机列表
	DWORD				LogTime;
	char				SMSInfo[MAX_SMS_Length+1];//短信信息

	PhoneSMSOnceInfo()
	{
		OnlyID = 0;
		Type = ST_Init;
		PhoneNumberVec.clear();
		LogTime = 0;
		ZeroMemory(&SMSInfo, sizeof(char)*(MAX_SMS_Length + 1));
	}
};
class PhoneSMSManager
{
public:
	PhoneSMSManager();
	virtual ~PhoneSMSManager();

	DWORD OnAddSMSEvent(PhoneSMSOnceInfo pEvent);
	void OnUpdate(DWORD dwTimer);

	void OnSendPhoneSMSResult(UINT64 ID, char* pData, DWORD Length);//异步的结果 由外部进行调用
private:
	bool HandleSMSEvent(PhoneSMSOnceInfo pEvent);
	void HandleSMSResult(PhoneSMSOnceInfo pEvent, bool Result);

private:
	DWORD									m_HandleIndex;

	HashMap<DWORD, PhoneSMSOnceInfo>		m_SMSHandleMap;//待处理的短信事件
};