#pragma once
#include "..\CommonFile\FishConfig.h"
#include "BuyEntityItemManager.h"
#include "RealNameVerificationManager.h"
#include "BindPhoneManager.h"
#include "BindEMailManager.h"
#include "UseRMBManager.h"
#include "ChannelLogonManager.h"
#include "PhonePayManager.h"
#include "PhoneLogon.h"
#include "NormalRechargeManager.h"
#include "IOSPayManager.h"
#include "OrderOnlyManager.h"
#include "WeiXinLogon.h"
#include "QQLogon.h"
#include "WeiXinUserInfoManager.h"
#include "QQUserInfoManager.h"
#include "HttpNewServer.h"
enum HttpType
{
	HT_Recharge = 1,//请求注册
	HT_PhonePay = 2,//手机话费
	HT_NormalPay = 3,//普通充值
};
enum HttpClientType
{
	HCT_Logon		= 1,//渠道登陆
	HCT_PhoneSMS	= 2,//手机短信
	HCT_PhonePay	= 3,//手机话费
};

class FishServer : public INetHandler
{
public:
	FishServer();
	virtual ~FishServer();

	bool InitServer();
	bool MainUpdate();
	bool OnDestroy();

	void SetServerClose(){ m_IsClose = true; }
	void SetReloadConfig(){ m_IsReloadConfig = true; }
	void ShowInfoToWin(const char *pcStr, ...);

	virtual uint CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize);
	virtual bool NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize);
	virtual void Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt);

	void SendNetCmdToClient(ServerClientData* pClient, NetCmd* pCmd);
	void SendNetCmdToCenterServer(NetCmd* pCmd);
	void SendNetCmdToDB(NetCmd* pCmd);
	void SendNetCmdToControl(NetCmd*pCmd);
	void SendNetCmdToLogDB(NetCmd* pCmd);
	void SendNetCmdToFtp(NetCmd* pCmd);

	ServerClientData* GetUserClientDataByIndex(BYTE IndexID);
	FishConfig&		GetFishConfig(){ return m_FishConfig; }
	HttpClient&		GetHttpClient(){ return m_HttpClient; }
	PhoneSMSManager& GetPhoneSMSMananger(){ return m_PhoneSMSManager; }
	BindPhoneManager& GetBindPhoneManager(){ return m_BindPhoneManager; }
	PhoneLogon& GetPhoneLogon(){ return m_PhoneLogon; }
	NormalRechargeManager& GetNormalRechargeManager(){ return m_NormalRechargeManager; }
	IOSPayManager& GetIOSPayManager(){ return m_IOSPayManager; }
	OrderOnlyManager& GetOrderOnlyManager(){ return m_OrderOnlyManager; }

	void			OnFinish();

	WORD			GetPhonePayID(){ return m_PhonePayID; }
	WORD			GetPhoneSMSID(){ return m_PhoneSMSID; }
	WORD			GetChannelLogonID(){ return m_ChannelLogonID; }
	WORD			GetNormalRechargeID(){ return m_NormalRechargeID; }
	WORD			GetIOSPayID(){ return m_IOSPayID; }
	WORD			GetIOSPayTestID(){ return m_IOSPayTestID; }
	WORD			GetWeiXinTokenID(){ return m_WeiXinTokenID; }
	WORD			GetQQTokenID(){ return m_QQTokenID; }

	WORD			GetWeiXinUserInfoID(){ return m_WeiXinUserInfoID; }
	WORD			GetWeiXinFaceImgID(){ return m_WeiXinFaceImgID; }
	//WORD			GetWeiXinTokenID(){ return m_WeiXinUserTokenID; }

	WORD			GetQQUserInfoID(){ return m_QQUserInfoID; }
	WORD			GetQQFaceImgID(){ return m_QQFaceImgID; }
private:
	bool ConnectClient();
	bool ConnectCenter();
	bool ConnectHttp();
	bool ConnectHttpClient();
	bool ConnectDB();
	bool ConnectControl();
	bool ConnectLogDB();
	bool ConnectFTP();

	void HandleAllMsg();
	bool HandleCenterMsg(NetCmd* pCmd);
	bool HandleHttpMsg(HttpRecvData* pHttpData);
	bool HandleDBMsg(NetCmd* pCmd);
	bool HandleFtpMsg(NetCmd* pCmd);
	bool HandleHttpClientMsg(HttpClientResult* pResult);

	bool HandleControlMsg(NetCmd* pCmd);

	void OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient);
	void OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient);
	void OnTcpClientConnect(TCPClient* pClient);
	void OnTcpClientLeave(TCPClient* pClient);

	bool HandleGameServerMsg(ServerClientData* pClient, NetCmd* pCmd);

	void OnAddClient();

	void OnReloadConfig();

	void UpdateInfoToControl(DWORD dwTimer);
private:
	Dump								m_pDump;
	//服务器控制属性
	volatile bool	 					m_IsClose;
	volatile bool						m_IsReloadConfig;

	FishConfig							m_FishConfig;
	

	NewServer							m_ClientTcp;//对GameServer的网络库
	HashMap<BYTE, ServerClientData*>	m_ClintList;//GameServer的列表

	BYTE								m_OperateNetworkID;

	//中央服务器		运营服务器想要连接到中央服务器去
	TCPClient							m_CenterTcp;//连接到中央服务器的连接
	bool								m_CenterTcpStates;

	TCPClient							m_DBTcp;
	bool								m_DBTcpStates;

	TCPClient							m_FTPTcp;
	bool								m_FTPTcpStates;

	//功能
	BindPhoneManager					m_BindPhoneManager;//手机验证
	RealNameVerificationManager			m_RealNameVerificationManager;//身份证绑定
	BuyEntityManager					m_BuyEntityManager;//实体物品购买
	BindEmailManager					m_BindEmailManager;
	UseRMBManager						m_UseRMBManager;

	SafeList<AfxNetworkClientOnce*>      m_AfxAddClient;

	ChannelLogonManager					m_ChannelLogon;
	PhonePayManager						m_PhonePayManager;

	WeiXinLogon							m_WeiXinLogon;
	QQLogon								m_QQLogon;

	WeiXinUserInfoManager				m_WeiXinUserInfo;
	QQUserInfoManager					m_QQUserInfo;

	//Http
	HttpNewServer							m_HttpServer;
	HttpClient							m_HttpClient;

	//HashMap
	WORD								m_ChannelLogonID;
	WORD								m_PhoneSMSID;
	WORD								m_PhonePayID;
	WORD								m_NormalRechargeID;
	WORD								m_IOSPayID;
	WORD								m_IOSPayTestID;

	WORD								m_WeiXinTokenID;
	WORD								m_QQTokenID;

	WORD								m_WeiXinUserInfoID;
	WORD								m_WeiXinFaceImgID;
	//WORD								m_WeiXinUserTokenID;

	WORD								m_QQUserInfoID;
	WORD								m_QQFaceImgID;

	TCPClient									m_ControlTcp;
	bool										m_ControlIsConnect;

	TCPClient									m_LogDBTcp;
	bool										m_LogDBIsConnect;

	//短信管理器
	PhoneSMSManager								m_PhoneSMSManager;
	PhoneLogon									m_PhoneLogon;

	//普通充值操作
	NormalRechargeManager						m_NormalRechargeManager;

	//IOS充值
	IOSPayManager								m_IOSPayManager;

	//唯一索引管理器
	OrderOnlyManager							m_OrderOnlyManager;
};
extern FishServer g_FishServer;