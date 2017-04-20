#pragma once
#include "..\CommonFile\FishConfig.h"
#include "niuniu\NiuNiuManager.h"
#include "RoleManager.h"
#include "..\CommonFile\RobotManager.h"
#include "DialManager.h"
#include"ServerControlItemSink.h"
#include "CarManager.h"
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
	void SendNetCmdToClient(BYTE GameID, NetCmd* pCmd);
	void SendNetCmdToAllClient(NetCmd* pCmd);
	void SendNetCmdToControl(NetCmd* pCmd);
	void SendNetCmdToCenter(NetCmd* pCmd);

	void SendNetCmdToDB(NetCmd* pCmd);
	void SendNetCmdToLogDB(NetCmd* pCmd);

	FishConfig& GetFishConfig(){ return m_FishConfig; }
	RoleManager& GetRoleManager(){ return m_RoleManager; }
	NiuNiuManager& GetNiuNiuManager(){ return m_NiuNiuManager; }
	DialManager& GetDialManager(){ return m_DialManager; }
	RobotManager& GetRobotManager(){ return m_RobotManager; }
	CServerControlOx& GetOxControl(){ return m_Oxcontrol; }
	CServerControlAnimal& GetAnimalControl(){ return m_AninalControl; }
	CServerControlCar& GetCarControl(){ return m_CarControl; }
	CarManager& GetCarManager(){ return m_CarManager; }
private:
	bool ConnectClient();
	bool ConnectDB();
	bool ConnectLogDB();
	bool ConnectControl();
	bool ConnectCenter();

	void OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient);
	void OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient);
	void OnTcpClientConnect(TCPClient* pClient);
	void OnTcpClientLeave(TCPClient* pClient);

	void OnReloadConfig();

	void HandleClientMsg(ServerClientData* pClient, NetCmd* pCmd);
	void HandleDataBaseMsg(NetCmd* pCmd);
	void HandleControlMsg(NetCmd* pCmd);
	void HandleCenterMsg(NetCmd* pCmd);

	void HandleAllMsg();

	void UpdateInfoToControl(DWORD dwTimer);

	void OnServerIsFinish();

	void HandleNiuNiuMsg(NetCmd* pCmd);
	void HandleDialMsg(NetCmd* pCmd);
	void HandleCarMsg(NetCmd* pCmd);
private:
	Dump										m_pDump;
	volatile bool	 							m_IsClose;
	volatile bool								m_IsReloadConfig;

	NewServer									m_ClientTcp;
	NewServer									m_ServerTcp;

	SafeList<AfxNetworkClientOnce*>				m_AfxAddClient;

	//Client  所有客户端的列表
	HashMap<BYTE, ServerClientData*>			m_ClientList;//Control的列表

	TCPClient									m_DBTcp;//对数据库的网络库
	bool										m_DBIsConnect;

	TCPClient									m_LogDBTcp;
	bool										m_LogDBIsConnect;

	TCPClient									m_ControlTcp;
	bool										m_ControlIsConnect;

	TCPClient									m_CenterTcp;
	bool										m_CenterIsConnect;

	BYTE										m_MiniGameServerID;
	//配置文件
	FishConfig									m_FishConfig;

	RoleManager									m_RoleManager;
	//
	NiuNiuManager								m_NiuNiuManager;
	DialManager									m_DialManager;
	CarManager									m_CarManager;
	//
	RobotManager								m_RobotManager;
	CServerControlOx							m_Oxcontrol;
	CServerControlAnimal						m_AninalControl;
	CServerControlCar							m_CarControl;
};

extern FishServer g_FishServer;