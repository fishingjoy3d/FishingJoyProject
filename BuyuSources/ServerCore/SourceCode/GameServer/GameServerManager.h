#pragma once
//struct TCPClientOnce//单个项
//{
//	TCPClient*		m_pClient;
//	bool			m_ClientStates;
//};
class GameServerManager //连接到全部的Logon上 从中央服务器获取到全部的Logon列表后
{
public:
	GameServerManager();
	virtual ~GameServerManager();
	void OnDestroy();

	void SendNetCmdToLogon(BYTE LogonID, NetCmd* pCmd);
	void SendNetCmdToAllLogon(NetCmd* pCmd);

	//void OnRsgGameToCenter();//将自己注册到Center去
	//void OnRsgGameToCenterResult(CG_Cmd_RsgGame* pMsg);
	void OnGameLeavCenter();//当自己断开与Center的连接的时候
	void OnLogonLeaveCenter(BYTE LogonID);//中央服务器告诉gameServer 一个Logon离开中央服务器了 gameServer将 它剔除掉
	void OnLogonRsgCenter(BYTE LogonID);

	//关于Logon的命令的处理 因为有多个 所以进行处理
	void OnHandleLogonMsg();
private:
	//HashMap<BYTE, TCPClientOnce>		m_LogonMap;//logon的集合 
};