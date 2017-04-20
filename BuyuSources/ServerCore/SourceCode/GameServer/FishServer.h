//GameServer核心类
#pragma once
#include "UserLogonManager.h"
#include "..\CommonFile\FishConfig.h"
#include "RoleManager.h"
#include "TableManager.h"
#include "ShopManager.h"
#include "EventManager.h"
#include "RoleOnlineReward.h"
#include "FishPackageManager.h"
//#include "GameServerManager.h"
#include "RoleCache.h"
#include "RoleLogonManager.h"
#include "TcpClientList.h"
#include "AnnouncementManager.h"
//#include "RobotManager.h"
#include "RoleQueueManager.h"
#include "ExChangeManager.h"
#include "SystemMailManager.h"
#include "LotteryManager.h"
#include "GameRobot.h"
struct DelClient
{
	DWORD		LogTime;
	DWORD		SocketID;
};

class FishServer : public INetHandler
{
public:
	FishServer();
	virtual ~FishServer();

	bool InitServer(int Index);
	void OnLoadFinish();
	bool MainUpdate();
	bool OnDestroy();

	void SetServerClose(){ m_IsClose = true; }
	void SetReloadConfig(){ m_IsReloadConfig = true; }
	void ShowInfoToWin(const char *pcStr, ...);

	virtual uint CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize);
	virtual bool NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize);
	virtual void Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt);
	void OnAddClient();

	void SendNetCmdToDB(NetCmd* pCmd);
	void SendNetCmdToCenter(NetCmd* pCmd);
	//void SendNetCmdToRank(NetCmd* pCmd);
	void SendNetCmdToFTP(NetCmd* pCmd);
	void SendNetCmdToOperate(NetCmd* pCmd);
	void SendNetCmdToSaveDB(NetCmd* pCmd);
	void SendNetCmdToLogDB(NetCmd* pCmd);
	//void SendNetCmdToLogon(NetCmd* pCmd);
	void SendNewCmdToClient(ServerClientData* pClient, NetCmd* pCmd);
	void SendNewCmdToAllClient(NetCmd* pCmd);
	void SendNetCmdToControl(NetCmd*pCmd);
	void SendNetCmdToLogon(BYTE LogonID, NetCmd* pCmd);
	void SendNetCmdToMiniGame(NetCmd* pCmd);

	ServerClientData* GetUserClientDataByIndex(DWORD IndexID);

	DWORD			GetOnlinePlayerSum(){ return m_ClintList.size(); }

	RoleManager*	GetRoleManager(){ return &m_RoleManager; }
	TableManager*	GetTableManager(){ return &m_TableManager; }
	EventManager&	GetEventManager(){ return m_EventManager; }
	FishConfig&		GetFishConfig(){ return m_FishConfig; }
	FishPackageManager& GetPackageManager(){ return m_PackageManager; }
	AnnouncementManager& GetAnnouncementManager(){ return m_AnnouncementManager; }
	GameRobotManager& GetRobotManager(){ return m_RobotManager; }
	RoleCache& GetRoleCache(){ return m_RoleCache; }
	SystemMailManager                       GetSystemMailManager() { return _SystemMailManager; }

	void UpdateByMin(DWORD dwTimer);
	void OnSaveInfoToDB(DWORD dwTimer);

	void CloseClientSocket(DWORD SocketID);

	BYTE  GetGameIndex(){ return m_GameNetworkID; }

	//bool HandleLogonMsg(BYTE LogonID, NetCmd* pCmd);

	//bool IsConnectCenterServer(){ return m_CenterTcp.IsConnected(); }

	void GetAddressByIP(DWORD IP, TCHAR* pAddress,DWORD ArrayCount);

	void SendAllMonthPlayerSumToClient(DWORD dwUserID);
	DWORD GetAchievementIndex(DWORD dwUserID);

	RoleLogonManager& GetRoleLogonManager(){ return m_RoleLogonManager; }

	//bool RoleIsOnlineByCenter(DWORD dwUserID);
	//void DelRoleOnlineInfo(DWORD dwUserID);
	void AddDelRoleSocket(DelClient delCLient){ m_DelSocketVec.push_back(delCLient); }

	bool HandleDataBaseMsg(NetCmd* pCmd);

	ChannelUserInfo* GetChannelUserInfo(DWORD UserID);
	void OnDelChannelInfo(DWORD UserID);
	DWORD GetOperateIP(){ return m_OperateIP; }

	BYTE KickUserByID(DWORD dwUserID,DWORD FreezeMin);

	string GetUserMacAddress(DWORD dwUserID);
	string GetUserIpAddress(DWORD dwUserID);
private:
	bool ConnectFTP();
	bool ConnectDB();
    bool ConnectSaveDB();
	bool ConnectLogDB();
	//bool ConnectRank();
	//bool ConnectLogon();
	bool ConnectCenter();
	bool ConnectClient();
	bool ConnectOperate();
	bool ConnectControl();
	bool ConnectMiniGame();

	void OnHandleAllMsg();

	void OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient);
	void OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient);
	void OnTcpClientConnect(TCPClient* pClient);
	void OnTcpClientLeave(TCPClient* pClient);

	void OnConnectionCenterServer();
	void OnLeaveCenterServer();

	bool HandleControlMsg(NetCmd* pCmd);
	bool HandleMiniGameMsg(NetCmd* pCmd);
	bool HandleNiuNiuMsg(NetCmd* pCmd);
	bool HandleDialMsg(NetCmd* pCmd);
	bool HandleCarMsg(NetCmd* pCmd);

	bool HandleClientMsg(ServerClientData* pClient, NetCmd* pCmd);
	
	//bool HandleRankMsg(NetCmd* pCmd);
	
	bool HandleCenterMsg(NetCmd* pCmd);
	bool HandleFtpMsg(NetCmd* pCmd);

	bool HandleOperateMsg(NetCmd* pCmd);

	//1.登陆模块
	bool OnHandLogonLogonMsg(NetCmd* pCmd);
	bool OnHandClientLogonMsg(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandDBLogonMsg(NetCmd* pCmd);
	bool OnHandleResetPasswordResult(NetCmd* pCmd);
	bool OnHandleRoleOnline(NetCmd* pCmd);
	bool OnHandleRoleAchievementIndex(NetCmd* pCmd);
	bool OnHandleResetAccountName(NetCmd* pCmd);
	//用户的命令
	//Center
	bool OnHandleSocketCenter(NetCmd* pCmd);
	//桌子进出
	bool OnHandleTCPNetworkTable(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleRoleJoinTable(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleRoleLeaveTable(ServerClientData* pClient, NetCmd* pCmd);
	//游戏内部消息
	bool OnHandleTCPNetworkGame(ServerClientData* pClient, NetCmd* pCmd);
	//网关处理
	bool OnGateJoinGameRoom(DWORD dwSocketID);//当一个网关加入进来的时候
	bool OnGateLeaveGameRoom(DWORD dwSocketID);
	//物品处理
	bool OnHandleTCPNetworkItem(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDateBaseLoadItem(NetCmd* pCmd);
	//bool OnHandleDataBaseLoadItemFinish(NetCmd* pCmd);
	bool OnHandleDataBaseAddItemResult(NetCmd* pCmd);
	//关系
	bool OnHandleTCPNetworkRelation(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleSocketRelation(NetCmd* pCmd);
	bool OnHandleDateBaseLoadRelation(NetCmd* pCmd);
	bool OnHandleDataBaseLoadBeRelation(NetCmd* pCmd);
	//bool OnHandleDataBaseLoadBeRelationFinish(NetCmd* pCmd);
	bool OnHandleDataBaseAddRelation(NetCmd* pCmd);
	//Mail
	bool OnHandleTCPNetworkMail(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDataBaseLoadUserMail(NetCmd* pCmd);
	//bool OnHandleDataBaseLoadUserMailFinish(NetCmd* pCmd);
	bool OnHandleDataBaseSendUserMail(NetCmd* pCmd);
	bool OnHandleSocketMail(NetCmd* pCmd);
	//Role
	bool OnHandleTCPNetworkRole(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleSaveRoleAllInfo(NetCmd* pCmd);
	bool OnHandleChangeRoleNickName(NetCmd* pCmd);
	bool OnHandleChangeRoleSecPassword(NetCmd* pCmd);
	bool OnHandleGameIDConvertUserID(NetCmd* pCmd);
	//查询
	bool OnHandleTCPNetworkQuery(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDataBaseLoadQueryUserInfo(NetCmd* pCmd);
	bool OnHandleDataBaseLoadQueryUserInfoByUserID(NetCmd* pCmd);
	bool OnHandleDataBaseLoadQueryUserInfoByGameID(NetCmd* pCmd);
	//签到
	bool OnHandleTCPNetworkCheck(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDataBaseLoadUserCheckInfo(NetCmd* pCmd);
	//任务
	bool OnHandleTCPNetworkTask(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleSocketTask(NetCmd* pCmd);
	bool OnHandleDataBaseLoadUserTaskInfo(NetCmd* pCmd);
	//成就
	bool OnHandleTCPNetworkAchievement(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDataBaseLoadUserAchievementInfo(NetCmd* pCmd);
	//比赛
	bool OnHandleTCPNetworkMonth(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleSocketMonth(NetCmd* pCmd);
	//称号
	bool OnHandleTCPNetworkTitle(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDataBaseLoadUserTitleInfo(NetCmd* pCmd);
	//排行榜
	bool OnHandleTCPNetworkRank(ServerClientData* pClient, NetCmd* pCmd);
	//宝箱
	bool OnHandleTCPNetworkChest(ServerClientData* pClient, NetCmd* pCmd);
	//魅力
	bool OnHandleTCPNetworkCharm(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleSocketCharm(NetCmd* pCmd);
	//商店
	bool OnHandleTCPNetworkShop(ServerClientData* pClient, NetCmd* pCmd);
	//Entity
	bool OnHandleTCPNetworlEntity(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDataBaseLoadUserEntityInfo(NetCmd* pCmd);
	//活动
	bool OnHandleTCPNetworkAction(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDataBaseLoadUserActionInfo(NetCmd* pCmd);
	//赠送
	bool OnHandleTCPNetworkGiff(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDataBaseLoadUserGiff(NetCmd* pCmd);
	bool OnHnaldeDataBaseLoadUserSendGiffInfo(NetCmd* pCmd);
	bool OnHandleDataBaseAddUserGiff(NetCmd* pCmd);
	bool OnHandleSocketGiff(NetCmd* pCmd);
	//金币商店
	//bool OnHandleTCPNetworkGlobelShop(ServerClientData* pClient, NetCmd* pCmd);
	//在线奖励
	bool OnHandleTCPNetworkOnlineReward(ServerClientData* pClient, NetCmd* pCmd);
	//玩家GameData
	bool OnHandleDataBaseLoadGameData( NetCmd* pCmd);
	//
	bool OnHandleTCPNetworkPackage(ServerClientData* pClient, NetCmd* pCmd);
	//GameData
	bool OnHandleTCPNetworkGameData(ServerClientData* pClient, NetCmd* pCmd);
	//Message
	bool OnHandleTCPNetworkMessage(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleSocketMessage(NetCmd* pCmd);
	//void HandlePlayerSum(DWORD dwTimer);
	void SendMessageByType(TCHAR* pMessage,WORD MessageSize, BYTE MessageType, DWORD MessageColor, BYTE StepNum, BYTE StepSec, DWORD Param,bool IsCenterMessage);
	//Recharge
	bool OnHandleTCPNetworkRecharge(ServerClientData* pClient, NetCmd* pCmd);
	//Announcement
	bool OnHandleTCPNetworkAnnouncement(ServerClientData* pClient, NetCmd* pCmd);
	//Operate
	bool OnHandleTCpNetworkOperate(ServerClientData* pClient, NetCmd* pCmd);

	void CheckDelSocket(DWORD dwTimer);

	void OnHandleUseRMB(CG_Cmd_UseRMB* pMsg);
	void OnHandlePhonePay(CG_Cmd_PhonePay* pMsg);

	//排行榜
	bool OnHandleLoadWeekRankInfo(NetCmd* pCmd);

	//ExChange
	bool OnHandleTCPNetworkExChange(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleDataBaseQueryExChange(NetCmd* pCmd);
	bool OnHandleGetRechargeOrderID(NetCmd* pCmd);

	//Lottery
	bool OnHandleTCPNetwordLottery(ServerClientData* pClient, NetCmd* pCmd);


	void OnAddChannelInfo(DWORD UserID, ChannelUserInfo* pInfo);
	
	void OnClearAllChannelInfo();
	
	void OnReloadConfig();

	//MiNiGame
	bool OnHandleTCPNetworkMiniGame(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleTCPNetworkNiuNiu(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleTCPNetworkDial(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleTCPNetworkCar(ServerClientData* pClient, NetCmd* pCmd);

	//Char
	bool OnHandleTCPNetworkChar(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleLoadCharInfo(NetCmd* pCmd);
	bool OnHandleSocketChar(NetCmd* pCmd);

	//RelationRequest
	bool OnHandleTCPNerwordRelationRequest(ServerClientData* pClient, NetCmd* pCmd);
	bool OnHandleSocketRelationRequest(NetCmd* pCmd);
	bool OnHandleDataBaseLoadRelationRequest(NetCmd* pCmd);
	bool OnHandleDataBaseAddRelationRequest(NetCmd* pCmd);

	void UpdateInfoToControl(DWORD dwTimer);
private:
	Dump						m_pDump;
	volatile bool	 			m_IsClose;
	volatile bool				m_IsReloadConfig;
	BYTE						m_GameNetworkID;
	//网络库
	NewUDPServer				m_ClientTcp;//对客户端的网络库
	HashMap<DWORD, ServerClientData*> m_ClintList;
	//vector<ServerClientData*>	m_ClintList;//客户端的连接

	TCPClient			m_CenterTcp;//连接到中央服务器的连接
	bool				m_CenterTcpStates;

	//DBServer
	TCPClient			m_DBTcp;//连接到数据库的连接
	bool				m_DBTcpStates;
	//DBSaveServer 专门用于保存的数据库连接
	TCPClient			m_DBSaveTcp;
	bool				m_DBSaveTcpStates;
	//Log
	TCPClient			m_DBLogTcp;
	bool				m_DBLogTcpStates;

	//TcpClientList		m_DBTcpList;

	//TCPClient			m_RankTcp;//连接到排行榜服务器的连接
	//bool				m_RankTcpStates;

	//TCPClient			m_LogonTcp;//连接到登录服务器的连接 因为Logon会有多个 
	//bool				m_LogonTcpStates;

	TCPClient			m_FtpTcp;//连接到FTP的连接
	bool				m_FtpTcpStates;

	TCPClient			m_OperatorTcp;
	bool				m_OperatorStates;

	//具体的管理器
	DWORD				m_UserIndex;
	UserLogonManager	m_LogonManager;

	//配置文件
	FishConfig			m_FishConfig;

	RoleManager			m_RoleManager;
	TableManager		m_TableManager;
	ShopManager			m_ShopManager;
	//GlobelShop			m_GlobelShopManager;
	RoleOnlineReward	m_OnlineRewardManager;
	FishPackageManager	m_PackageManager;
	EventManager		m_EventManager;
	bool				m_IsSendUserInfo;
	//FTP
	/*std::vector<ImgNetCmdResult>			m_MsgVec;*/

	
	

	/*DWORD									m_LogPlayerTime;
	DWORD									m_LogPlayerSum;*/

	//GameServerManager						m_GameServerManager;
	RoleCache								m_RoleCache;
	SafeList<AfxNetworkClientOnce*>			m_AfxAddClient;


	//中央服务器同步数据
	//比赛人数修改
	HashMap<BYTE, WORD>						m_MonthInfo;
	HashMap<DWORD, WORD>					m_AchjievementList;
	//全局汇总玩家集合
	//HashMap<DWORD, DWORD>					m_OnlineRoleMap;//UserID -> GameServerConfigID

	//std::vector<tagMonthRoleSum>			m_MonthInfo;
	//用户登陆唯一凭证
	RoleLogonManager						m_RoleLogonManager;

	AnnouncementManager						m_AnnouncementManager;
	SystemMailManager                       _SystemMailManager;

	vector<DelClient>						m_DelSocketVec;

	//RobotManager							m_RobotManager;

	RoleQueueManager						m_RoleQueueManager;//排队器

	//
	ExChangeManager							m_ExChangeManager;

	//
	HashMap<DWORD, ChannelUserInfo*>		m_ChannelInfo;

	//
	DWORD									m_OperateIP;


	//连接到控制器
	TCPClient									m_ControlTcp;
	bool										m_ControlIsConnect;

	//
	LotteryManager								m_LotteryManager;

	//连接到MiniGame
	TCPClient									m_MiniGameTcp;
	bool										m_MiniGameIsConnect;

	//记录登陆玩家 Mac地址
	HashMap<DWORD, string>						m_UserMacLog;

	GameRobotManager							m_RobotManager;
};
extern FishServer g_FishServer;