//中央服务器上的客户端管理
//保存当前连接上中央服务器的Logon 和 Game的基本数据  Logon 包括 ID  Game 包括ID
#pragma once
#include <set>
class ClientListManager
{
public:
	ClientListManager();
	virtual ~ClientListManager();

	void SendNetCmdToGameServer(BYTE GameID,NetCmd* pCmd);
	void SendNetCmdToLogonServer(BYTE LogonID, NetCmd* pCmd);
	void SendNetCmdToAllGameServer(NetCmd* pCmd);
	DWORD GetGameServerSum();
	DWORD GetLogonServerSum();
	void SendNetCmdToAllLogonServer(NetCmd* pCmd);

	void RsgLogon(BYTE LogonID);
	void UnRsgLogon(BYTE LogonID);

	void RsgGame(BYTE GameID);
	void UnRsgGame(BYTE GameID);

	void RsgUser(DWORD UserID, BYTE GameID);
	void UnRsgUser(DWORD UserID);

	void OnClientLeave(BYTE CLientID);
private:
	HashMap<BYTE,DWORD>					m_GameServerMap;
	HashMap<BYTE,DWORD>					m_LogonServerMap;
	//玩家的列表
	HashMap<DWORD, BYTE>				m_PlayerList;//UserID->Config						
};