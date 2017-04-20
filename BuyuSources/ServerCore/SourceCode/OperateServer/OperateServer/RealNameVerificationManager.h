//运营服务器上 实名验证的功能管理
//实名验证订单 GameServerConfigID
#pragma once
struct RealNameVerificationOnce
{
	DWORD			UserID;
	DWORD			GameServerClientID;
	TCHAR			Name[MAX_NAME_LENTH + 1];//实际名称
	TCHAR			IDCard[MAX_IDENTITY_LENGTH + 1];//身份证号码

	//订单的验证
	DWORD			beginTime;//订单处理 1分钟内无处理 表示订单过期了
};
class RealNameVerificationManager
{
public:
	RealNameVerificationManager();
	virtual ~RealNameVerificationManager();

	bool OnAddRealNameVerificationOnce(ServerClientData* pClient, GO_Cmd_RealNameVerification* pMsg);//添加一个订单
	//void OnCheckEntityResult(DBO_Cmd_CheckEntityID* pMsg);
	void OnAddRealNameVerificationOnceResult(BYTE ErrorID,bool Result, DWORD dwUserID);//从外部服务器返回的结果
	void OnUpdate(DWORD dwTimer);//进行更新处理

	bool CheckEntityID(LPCTSTR EntityID);
private:
	HashMap<DWORD, RealNameVerificationOnce>			m_RealNameVerificationMap;//全部等待实明验证的订单
};