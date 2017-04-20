//登陆服务器的核心类
#pragma once
#include "..\CommonFile\FishConfig.h"
#include "LogonManager.h"
#include "LogonCache.h"
struct DelClient
{
	DWORD		LogTime;
	DWORD		SocketID;
};
struct GameServerInfo
{
	DWORD	ID;
	DWORD	IP;
	WORD	Port;
	WORD	OnlineSum;

	GameServerInfo()
	{
		ID = 0;
		IP = 0;
		Port = 0;
		OnlineSum = 0;
	}
};
struct WriteGameServerUserInfo
{
	DWORD		dwUserID;
	DWORD		ClientID;
	DWORD		LogTime;
};
class FishServer : public INetHandler
{
public:
	FishServer();
	virtual ~FishServer();

	bool InitServer(DWORD LogonID);
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
	void SendNetCmdToGameServer(BYTE GameID,NetCmd* pCmd);
	void SendNewCmdToClient(ServerClientData* pClient, NetCmd* pCmd);
	void SendNetCmdToCenter(NetCmd* pCmd);
	void SendNetCmdToOperate(NetCmd* pCmd);
	void SendNetCmdToControl(NetCmd*pCmd);
	void SendNetCmdToLogDB(NetCmd* pCmd);

	ServerClientData* GetClientServerData(DWORD ClientID);
	//ServerClientData* GetServerServerData(BYTE ServerID);
	//GameServerInfo* GetGameServerInfo(DWORD GameServerID);

	DWORD GetLogonConfigID(){ return m_LogonConfigID; }

	void OnLogonDBDataSucess();

	void AddDelRoleSocket(DelClient delCLient){ m_DelSocketVec.push_back(delCLient); }
private:
	bool ConnectDB();
	bool ConnectCenter();
	bool ConnectOperate();
	bool ConnectClient();
	bool ConnectControl();
	bool ConnectLogDB();

	void HandleAllMsg();

	bool HandleControlMsg(NetCmd* pCmd);

	void OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient);
	void OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient);
	void OnTcpClientConnect(TCPClient* pClient);
	void OnTcpClientLeave(TCPClient* pClient);


	void HandleClientMsg(ServerClientData* pClient,NetCmd* pCmd);
	void HandleGameServerMsg(NetCmd* pCmd);
	void HandleDataBaseMsg(NetCmd* pCmd);
	void HandleCenterMsg(NetCmd* pCmd);
	void HandleOperateMsg(NetCmd* pCmd);

	//ServerClientData* GetMaxFreeGameServer();

	void CheckDelSocket(DWORD dwTimer);
	void CloseClientSocket(DWORD SocketID);

	void OnAddChannelInfo(DWORD ClientID, ChannelUserInfo* pInfo);
	void OnDelChannelInfo(DWORD ClientID);
	void OnSendChannelInfoToGameServer(DWORD dwUserID, DWORD ClientID, BYTE GameServerID);
	void OnClearAllChannelInfo();

	bool OnAddUserWriteGameServer(DWORD dwUserID,DWORD ClientID);
	bool OnCheckUserWriteGameServerIsExists(DWORD dwUserID);
	void OnUpdateWriteGameServerUserMap(DWORD dwTimer);

	void OnReloadConfig();

	void UpdateInfoToControl(DWORD dwTimer);
private:
	Dump						m_pDump;
	volatile bool	 	m_IsClose;
	volatile bool								m_IsReloadConfig;
	//网络库
	NewServer			m_ClientTcp;//对客户端的网络库
	//NewServer			m_ServerTcp;//对服务器的网络库
	TCPClient			m_DBTcp;//对数据库的网络库
	TCPClient			m_CenterTcp;//对中央服务器的网络库
	TCPClient			m_OperateTcp;//运营服务器
	//vector<ServerClientData*>	m_ClintList;//客户端的连接
	//vector<ServerClientData*>	m_ServerList;//GameServer的连接

	SafeList<AfxNetworkClientOnce*>      m_AfxAddClient;

	HashMap<DWORD, ServerClientData*>	m_ClintList;
	DWORD								m_ClientIndex;

	//HashMap<BYTE, ServerClientData*>	m_ServerList;
	//DWORD								m_ServerIndex;

	bool											m_DBIsConnect;
	bool											m_CenterIsConnect;
	bool											m_OperateIsConnect;
	//配置数据 
	BYTE								m_ClientNetworkID;
	//BYTE								m_ServerNetworkID;

	BYTE								m_LogonConfigID;

	LogonManager						m_LogonManager;
	FishConfig							m_FishConfig;

	LogonCache							m_LogonCacheManager;

	vector<DelClient>					m_DelSocketVec;//待删除的客户端

	HashMap<DWORD, ChannelUserInfo*>	m_ChannelInfo;

	HashMap<DWORD, DWORD>				m_UserChannelMap;

	//向GameServer 发送的玩家的记录 
	HashMap<DWORD, WriteGameServerUserInfo>		m_SendGameUserMap;//等待GameServer返回的玩家的集合

	TCPClient									m_ControlTcp;
	bool										m_ControlIsConnect;


	TCPClient									m_LogDBTcp;
	bool										m_LogDBIsConnect;
};
extern FishServer g_FishServer;