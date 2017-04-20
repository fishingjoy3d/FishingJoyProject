#pragma once
#include "..\CommonFile\FishConfig.h"
#include "RankManager.h"
class FishServer/* : public INetHandler*/
{
public:
	FishServer();
	virtual ~FishServer();

	bool InitServer(bool IsNeedCreateRankXml);
	bool MainUpdate();
	bool OnDestroy();

	void UpdateByMin(DWORD dwTimer);

	void SetServerClose(){ m_IsClose = true; }
	void ShowInfoToWin(const char *pcStr, ...);

	/*virtual bool CanConnected(BYTE SeverID, uint ip, short port);
	virtual bool NewClient(BYTE SeverID, ServerClientData *pClient);*/

	void SendNetCmdToDB(NetCmd* pCmd);
	void SendNetCmdToControl(NetCmd*pCmd);
	//void SendNetCmdToGameServer(ServerClientData* pClient, NetCmd* pCmd);
	//void SendNetCmdToAllGameServer(NetCmd* pCmd);
	void SendNetCmdToFtpServer(NetCmd* pCmd);
	void SendNetCmdToLogDB(NetCmd* pCmd);

	FishConfig& GetFishConfig(){ return m_FishConfig; }

	ServerClientData* GetUserClientDataByIndex(DWORD IndexID);


private:
	bool ConnectFTP();
	bool ConnectDB();
	bool ConnectControl();
	bool ConnectLogDB();
	//bool ConnectClient();

	void HandleAllMsg();

	bool HandleControlMsg(NetCmd* pCmd);

	/*void OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient);
	void OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient);*/
	void OnTcpClientConnect(TCPClient* pClient);
	void OnTcpClientLeave(TCPClient* pClient);


	//bool HandleGameServerMsg(ServerClientData* pClient, NetCmd* pCmd);
	bool HandleDataBaseMsg(NetCmd* pCmd);
	bool HandleFtpMsg(NetCmd* pCmd);

	bool HandleDataBaseLoadRankInfo(NetCmd* pCmd);

	void UpdateInfoToControl(DWORD dwTimer);
private:
	Dump						m_pDump;
	//服务器控制属性
	volatile bool	 					m_IsClose;
	//TCPServer							m_ClientTcp;//对GameServer的网络库
	TCPClient							m_DBTcp;//对数据库的网络库
	TCPClient							m_FtpTcp;//对FTP的网络库
	//HashMap<DWORD, ServerClientData*>	m_ClintList;//GameServer的列表
	//DWORD								m_ClientIndex;

	bool								m_DBIsConnect;
	bool								m_FtpIsConnect;

	//管理器
	FishConfig							m_FishConfig;

	RankManager							m_RankManager;

	BYTE								m_RankNetworkID;

	TCPClient									m_ControlTcp;
	bool										m_ControlIsConnect;

	TCPClient									m_LogDBTcp;
	bool										m_LogDBIsConnect;
};
extern FishServer g_FishServer;