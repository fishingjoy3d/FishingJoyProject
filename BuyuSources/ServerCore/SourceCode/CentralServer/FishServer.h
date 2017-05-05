#pragma once
#include "..\CommonFile\FishConfig.h"
#include "RoleManager.h"
#include "RoleRelationManager.h"
#include "FishMonthManager.h"
#include "TaskManager.h"
#include "ClientListManager.h"
#include "AnnouncementManager.h"
#include "FishNoticeManager.h"
#include "OperatorSystemMailManager.h"
#include "..\CommonFile\RobotManager.h"

class FishServer : public INetHandler
{
public:
	FishServer();
	virtual ~FishServer();

	bool InitServer(bool IsInit);
	bool MainUpdate();
	bool OnDestroy();

	void UpdateByMin(DWORD dwTimer);

	void SetServerClose(){ m_IsClose = true; }
	void SetReloadConfig(){ m_IsReloadConfig = true; }
	void ShowInfoToWin(const char *pcStr, ...);

	virtual uint CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize);
	virtual bool NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize);
	virtual void Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt);
	void OnAddClient();

	void SendNetCmdToDB(NetCmd* pCmd);
	void SendNetCmdToClient(ServerClientData* pClient, NetCmd* pCmd);
	void SendNetCmdToFtp(NetCmd* pCmd);
	void SendNetCmdToControl(NetCmd*pCmd);
	void SendNetCmdToLogDB(NetCmd* pCmd);
	void SendNetCmdToAllGameServer(NetCmd* pCmd);

	FishConfig& GetFishConfig(){ return m_FishConfig; }
	CenterRoleManager& GetRoleManager(){ return m_RoleManager; }
	FishMonthManager&	GetMonthManager(){ return m_MonthManager; }
	ClientListManager& GetCenterManager(){ return m_CenterServerManager; }
	RobotManager& GetRobotManager(){ return m_RobotManager; }

	ServerClientData* GetUserClientDataByIndex(BYTE IndexID);

	void SendUserAchievementToGameServer(BYTE GameID);

	void SendMessageByType(TCHAR* pMessage,WORD MessageSize, BYTE MessageType, DWORD MessageColor, BYTE StepNum, BYTE StepSec, DWORD Param);
private:
	void OnCenterFinish();

	bool ConnectFTP();
	bool ConnectDB();
	bool ConnectClient();
	bool ConnectControl();
	bool ConnectLogDB();
	void ThirdPlatformBuy(int shopID, DWORD user_id, int channel_id, int order_id);
	void HandleAllMsg();

	void OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient);
	void OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient);
	void OnTcpClientConnect(TCPClient* pClient);
	void OnTcpClientLeave(TCPClient* pClient);

	bool HandleGameServerMsg(ServerClientData* pClient, NetCmd* pCmd);
	bool HandleDataBaseMsg(NetCmd* pCmd);
	bool HandleFtpMsg(NetCmd* pCmd);
	bool HandleControlMsg(NetCmd* pCmd);

	bool HandleAddUserMailResult(NetCmd* pCmd);
	bool HandleSaveRoleCharmResult(NetCmd* pCmd);
	bool HandleSendAchievementPointList(NetCmd* pCmd);

	void SendGetAchievementList();

	void OnReloadConfig();
	void UpdateInfoToControl(DWORD dwTimer);
private:
	//dump
	Dump										m_pDump;
	//服务器控制属性
	volatile bool	 							m_IsClose;
	volatile bool								m_IsReloadConfig;
	NewServer									m_ClientTcp;//对GameServer的网络库
	TCPClient									m_DBTcp;//对数据库的网络库
	TCPClient									m_LogDBTcp;//Log 数据库
	TCPClient									m_FtpTcp;//对FTP的网络库

	//NewServer									m_ControlTcp;//外部控制的TCP 对象 
	//DWORD										m_ControlRankValue;
	//HashMap<DWORD, ServerClientData*>			m_ControlList;//Control的列表
	//BYTE										m_ControlworkID;
	//DWORD										m_ControlIndex;

	BYTE										m_CenterworkID;
	HashMap<BYTE, ServerClientData*>			m_ClintList;//GameServer的列表
	SafeList<AfxNetworkClientOnce*>				m_AfxAddClient;
	bool										m_DBIsConnect;
	bool										m_FtpIsConnect;
	bool										m_LogDBIsConnect;
	//玩家管理器
	CenterRoleManager							m_RoleManager;
	//配置文件
	FishConfig									m_FishConfig;
	//比赛管理区
	FishMonthManager							m_MonthManager;
	//任务管理器
	TaskManager									m_GlobelTaskManager;
	//GameServer 和 LogonServer 的 管理器 
	ClientListManager							m_CenterServerManager;
	//成就点排名
	HashMap<DWORD, WORD>						m_AchievementList;
	//公告
	AnnouncementManager							m_AnnouncementManager;
	OperatorSystemMailManager                   m_OperatorMailManager;

	FishNoticeManager							m_FishNoticeManager;
	

	//连接到控制器
	TCPClient									m_ControlTcp;
	bool										m_ControlIsConnect;

	//机器人
	RobotManager								m_RobotManager;
	std::string									m_account;
	std::string									m_password;
};
extern FishServer g_FishServer;