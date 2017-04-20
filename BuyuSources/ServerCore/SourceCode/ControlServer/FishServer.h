#pragma once
#include "..\CommonFile\FishConfig.h"
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
	void OnAddClient();

	void SendNetCmdToClient(ServerClientData* pClient, NetCmd* pCmd);
	void SendNetCmdToAllClient(NetCmd* pCmd);

	void SendNetCmdToControlDB(NetCmd* pCmd);//发送命令给DB

	void SendNetCmdToDB(BYTE DBID,NetCmd* pCmd);
	void SendNetCmdToAllDB(NetCmd* pCmd);
	void SendNetCmdToFtp(NetCmd* pCmd);
	void SendNetCmdToRank(NetCmd* pCmd);
	void SendNetCmdToOperate(NetCmd* pCmd);
	void SendNetCmdToLogon(BYTE LogonID,NetCmd* pCmd);
	void SendNetCmdToAllLogon(NetCmd* pCmd);
	void SendNetCmdToGame(BYTE GameID, NetCmd* pCmd);
	void SendNetCmdToAllGame(NetCmd* pCmd);
	void SendNetCmdToCenter(NetCmd* pCmd);
	void SendNetCmdToAllServer(NetCmd* pCmd);
	void SendNetCmdToLogDB(NetCmd* pCmd);
	void SendNetCmdToMiniGame(NetCmd* pCmd);
	void SendNtCmdToGMTool(NetCmd* pCmd);
	

	FishConfig& GetFishConfig(){ return m_FishConfig; }
private:
	bool ConnectControlClient();
	bool ConnectControlServer();
	bool ConnectDB();
	bool ConnectLogDB();

	void OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient);
	void OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient);
	void OnTcpClientConnect(TCPClient* pClient);
	void OnTcpClientLeave(TCPClient* pClient);

	void OnReloadConfig();

	void HandleClientMsg(ServerClientData* pClient, NetCmd* pCmd);
	void HandleServerMsg(ServerClientData* pClient, NetCmd* pCmd);
	void HandleGMToolMsg(ServerClientData* pClient, NetCmd* pCmd);
	void HandleDataBaseMsg(NetCmd* pCmd);

	void HandleAllMsg();
private:
	Dump										m_pDump;
	volatile bool	 							m_IsClose;
	volatile bool								m_IsReloadConfig;

	NewServer									m_ClientTcp;
	NewServer									m_ServerTcp;

	SafeList<AfxNetworkClientOnce*>				m_AfxAddClient;

	//Client  所有客户端的列表
	HashMap<DWORD, ServerClientData*>			m_ControlClientList;//Control的列表
	HashMap<BYTE, ServerClientData*>			m_ControlServerList;//Control的列表	

	TCPClient									m_DBTcp;//对数据库的网络库
	bool										m_DBIsConnect;

	TCPClient									m_LogDBTcp;
	bool										m_LogDBIsConnect;


	BYTE										m_ControlServerNetworkID;
	BYTE										m_ControlClientNetworkID;
	BYTE										m_ControlID;
	//配置文件
	FishConfig									m_FishConfig;

	DWORD										m_ControlClientIndex;
	DWORD										m_ControlRankValue;

};

extern FishServer g_FishServer;