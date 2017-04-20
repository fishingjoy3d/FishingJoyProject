#include "Stdafx.h"
#include "FishServerConfig.h"
FishServerConfig g_FishServerConfig;
FishServerConfig::FishServerConfig()
{

}
FishServerConfig::~FishServerConfig()
{
	m_DBConfigMap.clear();
	m_GameConfigMap.clear();
}
bool FishServerConfig::LoadServerConfigFile()
{
	//加载全部的配置文件
	WHXml pXml;
	if (!pXml.LoadXMLFilePath(TEXT("FishServer.xml")))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishServer = pXml.GetChildNodeByName(TEXT("FishServer"));
	if (!pFishServer)
	{
		ASSERT(false);
		return false;
	}
	//2.DB
	if (!LoadDBConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}
	//1.Center
	if (!LoadCenterConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}
	//3.GameServer
	if (!LoadGameServerConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}

	if (!LoadLogonConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}

	if (!LoadRankConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}

	if (!LoadFTPConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}

	if (!LoadRobotConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}

	if (!LoadOperateConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}
	/*if (!LoadGateConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}*/
	if (!LoadGateWayConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadControlServerConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadMiniGameServerConfig(pFishServer))
	{
		ASSERT(false);
		return false;
	}
	return true;
}
bool FishServerConfig::LoadCenterConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pCenterServer = pFishConfig->GetChildNodeByName(TEXT("CenterServer"));
	if (!pCenterServer)
	{
		ASSERT(false);
		return false;
	}
	if (!pCenterServer->GetAttribute(TEXT("NetworkID"), m_CenterConfig.NetworkID))
		return false;

	{
		TCHAR IP[IP_MAX_LENGTH] = { 0 };
		if (!pCenterServer->GetAttribute(TEXT("IP"), IP, IP_MAX_LENGTH))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(IP, Count);
		strncpy_s(m_CenterConfig.CenterListenIP, Count + 1, pChar, Count);
		free(pChar);
	}
	if (!pCenterServer->GetAttribute(TEXT("DBNetworkID"), m_CenterConfig.DBNetworkID))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("LogDBID"), m_CenterConfig.LogDBID))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("Port"), m_CenterConfig.CenterListenPort))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("TimeOut"), m_CenterConfig.TimeOut))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("BuffSize"), m_CenterConfig.BuffSize))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("RecvBuffSize"), m_CenterConfig.RecvBuffSize))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("SendBuffSize"), m_CenterConfig.SendBuffSize))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("RecvThreadSum"), m_CenterConfig.RecvThreadSum))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("SendThreadSum"), m_CenterConfig.SendThreadSum))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("SleepTime"), m_CenterConfig.SleepTime))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("AcceptSleepTime"), m_CenterConfig.AcceptSleepTime))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ListenCount"), m_CenterConfig.ListenCount))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("MaxSendCountPerFrame"), m_CenterConfig.MaxSendCountPerFrame))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("MaxAcceptNumPerFrame"), m_CenterConfig.MaxAcceptNumPerFrame))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("MaxRecvCmdCount"), m_CenterConfig.MaxRecvCmdCount))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("MaxSendCmdCount"), m_CenterConfig.MaxSendCmdCount))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("SceneHearbeatTick"), m_CenterConfig.SceneHearbeatTick))
		return false;

	if (!pCenterServer->GetAttribute(TEXT("BeginRobotUserID"), m_CenterConfig.BeginUserID))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("EndRobotUserID"), m_CenterConfig.EndUserID))
		return false;
	/*{
		TCHAR IP[IP_MAX_LENGTH] = { 0 };
		if (!pCenterServer->GetAttribute(TEXT("ControlIP"), IP, IP_MAX_LENGTH))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(IP, Count);
		strncpy_s(m_CenterConfig.ControlListenIP, Count + 1, pChar, Count);
		free(pChar);
	}
	
	if (!pCenterServer->GetAttribute(TEXT("ControlNetworkID"), m_CenterConfig.ControlNetworkID))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlPassword"), m_CenterConfig.ControlPassword))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlPort"), m_CenterConfig.ControlListenPort))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlTimeOut"), m_CenterConfig.ControlTimeOut))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlBuffSize"), m_CenterConfig.ControlBuffSize))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlRecvBuffSize"), m_CenterConfig.ControlRecvBuffSize))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlSendBuffSize"), m_CenterConfig.ControlSendBuffSize))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlRecvThreadSum"), m_CenterConfig.ControlRecvThreadSum))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlSendThreadSum"), m_CenterConfig.ControlSendThreadSum))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlSleepTime"), m_CenterConfig.ControlSleepTime))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlAcceptSleepTime"), m_CenterConfig.ControlAcceptSleepTime))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlListenCount"), m_CenterConfig.ControlListenCount))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlMaxSendCountPerFrame"), m_CenterConfig.ControlMaxSendCountPerFrame))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlMaxAcceptNumPerFrame"), m_CenterConfig.ControlMaxAcceptNumPerFrame))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlMaxRecvCmdCount"), m_CenterConfig.ControlMaxRecvCmdCount))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlMaxSendCmdCount"), m_CenterConfig.ControlMaxSendCmdCount))
		return false;
	if (!pCenterServer->GetAttribute(TEXT("ControlSceneHearbeatTick"), m_CenterConfig.ControlSceneHearbeatTick))
		return false;*/

	return true;
}



bool FishServerConfig::LoadDBConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pDBServer = pFishConfig->GetChildNodeByName(TEXT("DBServer"));
	while (pDBServer)
	{
		DBServerConfig pDBConfig;

		if (!pDBServer->GetAttribute(TEXT("NetworkID"), pDBConfig.NetworkID))
			return false;

		TCHAR IP[IP_MAX_LENGTH] = { 0 };
		if (!pDBServer->GetAttribute(TEXT("IP"), IP, IP_MAX_LENGTH))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(IP, Count);
		strncpy_s(pDBConfig.DBListenIP, Count+1, pChar,Count);
		free(pChar);

		if (!pDBServer->GetAttribute(TEXT("Port"), pDBConfig.DBListenPort))
			return false;
		/*if (!pDBServer->GetAttribute(TEXT("RecvMaxSize"), pDBConfig.RecvMaxSize))
			return false;
		if (!pDBServer->GetAttribute(TEXT("SendMaxSize"), pDBConfig.SendMaxSize))
			return false;*/
		if (!pDBServer->GetAttribute(TEXT("TimeOut"), pDBConfig.TimeOut))
			return false;
		if (!pDBServer->GetAttribute(TEXT("BuffSize"), pDBConfig.BuffSize))
			return false;
		if (!pDBServer->GetAttribute(TEXT("RecvBuffSize"), pDBConfig.RecvBuffSize))
			return false;
		if (!pDBServer->GetAttribute(TEXT("SendBuffSize"), pDBConfig.SendBuffSize))
			return false;
		if (!pDBServer->GetAttribute(TEXT("RecvThreadSum"), pDBConfig.RecvThreadSum))
			return false;
		if (!pDBServer->GetAttribute(TEXT("SendThreadSum"), pDBConfig.SendThreadSum))
			return false;
		if (!pDBServer->GetAttribute(TEXT("SleepTime"), pDBConfig.SleepTime))
			return false;
		if (!pDBServer->GetAttribute(TEXT("AcceptSleepTime"), pDBConfig.AcceptSleepTime))
			return false;
		if (!pDBServer->GetAttribute(TEXT("ListenCount"), pDBConfig.ListenCount))
			return false;
		if (!pDBServer->GetAttribute(TEXT("MaxSendCountPerFrame"), pDBConfig.MaxSendCountPerFrame))
			return false;
		if (!pDBServer->GetAttribute(TEXT("MaxAcceptNumPerFrame"), pDBConfig.MaxAcceptNumPerFrame))
			return false;
		if (!pDBServer->GetAttribute(TEXT("MaxRecvCmdCount"), pDBConfig.MaxRecvCmdCount))
			return false;
		if (!pDBServer->GetAttribute(TEXT("MaxSendCmdCount"), pDBConfig.MaxSendCmdCount))
			return false;
		if (!pDBServer->GetAttribute(TEXT("SceneHearbeatTick"), pDBConfig.SceneHearbeatTick))
			return false;

		TCHAR TempStr[MySQLLength] = { 0 };
		UINT TempCount = 0;

		if (!pDBServer->GetAttribute(TEXT("DBName"), TempStr, MySQLLength))
			return false;

		pChar = WCharToChar(TempStr, TempCount);
		strncpy_s(pDBConfig.MySQlDBName, TempCount + 1, pChar, TempCount);
		free(pChar);


		if (!pDBServer->GetAttribute(TEXT("DBIP"), TempStr, MySQLLength))
			return false;
		pChar = WCharToChar(TempStr, TempCount);
		strncpy_s(pDBConfig.MySQlIP, TempCount + 1, pChar, TempCount);
		free(pChar);


		if (!pDBServer->GetAttribute(TEXT("DBUserName"), TempStr, MySQLLength))
			return false;
		pChar = WCharToChar(TempStr, TempCount);
		strncpy_s(pDBConfig.MySQlUserName, TempCount + 1, pChar, TempCount);
		free(pChar);

		if (!pDBServer->GetAttribute(TEXT("DBUserPassword"), TempStr, MySQLLength))
			return false;
		pChar = WCharToChar(TempStr, TempCount);
		
		strncpy_s(pDBConfig.MySQlUserPassword, TempCount + 1, pChar, TempCount);
		free(pChar);
		if (!pDBServer->GetAttribute(TEXT("DBPort"), pDBConfig.MySqlPort))
			return false;
		

		m_DBConfigMap.insert(HashMap<BYTE, DBServerConfig>::value_type(pDBConfig.NetworkID, pDBConfig));

		pDBServer = pDBServer->GetNextSignelNode();
	}
	return true;
}
bool FishServerConfig::LoadMiniGameServerConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pGameServer = pFishConfig->GetChildNodeByName(TEXT("MiniGameServer"));
	if (!pGameServer)
	{
		ASSERT(false);
		return false;
	}

	if (!pGameServer->GetAttribute(TEXT("NetworkID"), m_MiniGameServerConfig.NetworkID))
		return false;

	TCHAR IP[IP_MAX_LENGTH] = { 0 };
	if (!pGameServer->GetAttribute(TEXT("IP"), IP, IP_MAX_LENGTH))
		return false;
	UINT Count = 0;
	char* pChar = WCharToChar(IP, Count);
	strncpy_s(m_MiniGameServerConfig.GameListenIP, Count + 1, pChar, Count);
	free(pChar);

	if (!pGameServer->GetAttribute(TEXT("Port"), m_MiniGameServerConfig.GameListenPort))
		return false;
	if (!pGameServer->GetAttribute(TEXT("TimeOut"), m_MiniGameServerConfig.TimeOut))
		return false;
	if (!pGameServer->GetAttribute(TEXT("BuffSize"), m_MiniGameServerConfig.BuffSize))
		return false;
	if (!pGameServer->GetAttribute(TEXT("DBNetworkID"), m_MiniGameServerConfig.DBNetworkID))
		return false;
	if (!pGameServer->GetAttribute(TEXT("RecvBuffSize"), m_MiniGameServerConfig.RecvBuffSize))
		return false;
	if (!pGameServer->GetAttribute(TEXT("SendBuffSize"), m_MiniGameServerConfig.SendBuffSize))
		return false;
	if (!pGameServer->GetAttribute(TEXT("RecvThreadSum"), m_MiniGameServerConfig.RecvThreadSum))
		return false;
	if (!pGameServer->GetAttribute(TEXT("SendThreadSum"), m_MiniGameServerConfig.SendThreadSum))
		return false;
	if (!pGameServer->GetAttribute(TEXT("SleepTime"), m_MiniGameServerConfig.SleepTime))
		return false;
	if (!pGameServer->GetAttribute(TEXT("AcceptSleepTime"), m_MiniGameServerConfig.AcceptSleepTime))
		return false;
	if (!pGameServer->GetAttribute(TEXT("LogDBNetworkID"), m_MiniGameServerConfig.LogDBNetworkID))
		return false;
	if (!pGameServer->GetAttribute(TEXT("ListenCount"), m_MiniGameServerConfig.ListenCount))
		return false;
	if (!pGameServer->GetAttribute(TEXT("MaxSendCountPerFrame"), m_MiniGameServerConfig.MaxSendCountPerFrame))
		return false;
	if (!pGameServer->GetAttribute(TEXT("MaxAcceptNumPerFrame"), m_MiniGameServerConfig.MaxAcceptNumPerFrame))
		return false;
	if (!pGameServer->GetAttribute(TEXT("MaxRecvCmdCount"), m_MiniGameServerConfig.MaxRecvCmdCount))
		return false;
	if (!pGameServer->GetAttribute(TEXT("MaxSendCmdCount"), m_MiniGameServerConfig.MaxSendCmdCount))
		return false;
	if (!pGameServer->GetAttribute(TEXT("SceneHearbeatTick"), m_MiniGameServerConfig.SceneHearbeatTick))
		return false;
	if (!pGameServer->GetAttribute(TEXT("BeginRobotUserID"), m_MiniGameServerConfig.BeginUserID))
		return false;
	if (!pGameServer->GetAttribute(TEXT("EndRobotUserID"), m_MiniGameServerConfig.EndUserID))
		return false;
	if (m_DBConfigMap.count(m_MiniGameServerConfig.LogDBNetworkID) != 1)
		return false;
	if (m_DBConfigMap.count(m_MiniGameServerConfig.DBNetworkID) != 1)
		return false;
	if (m_MiniGameServerConfig.LogDBNetworkID == m_MiniGameServerConfig.DBNetworkID)
		return false;
	return true;
}
bool FishServerConfig::LoadGameServerConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pGameServer = pFishConfig->GetChildNodeByName(TEXT("GameServer"));
	while (pGameServer)
	{
		GameServerConfig pGameConfig;

		if (!pGameServer->GetAttribute(TEXT("NetworkID"), pGameConfig.NetworkID))
			return false;

		TCHAR IP[IP_MAX_LENGTH] = { 0 };
		if (!pGameServer->GetAttribute(TEXT("IP"), IP, IP_MAX_LENGTH))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(IP, Count);
		strncpy_s(pGameConfig.GameListenIP, Count + 1, pChar, Count);
		free(pChar);

		if (!pGameServer->GetAttribute(TEXT("Port"), pGameConfig.GameListenPort))
			return false;
		/*if (!pGameServer->GetAttribute(TEXT("RecvMaxSize"), pGameConfig.RecvMaxSize))
			return false;
		if (!pGameServer->GetAttribute(TEXT("SendMaxSize"), pGameConfig.SendMaxSize))
			return false;*/
		if (!pGameServer->GetAttribute(TEXT("TimeOut"), pGameConfig.TimeOut))
			return false;
		if (!pGameServer->GetAttribute(TEXT("BuffSize"), pGameConfig.BuffSize))
			return false;
		if (!pGameServer->GetAttribute(TEXT("DBNetworkID"), pGameConfig.DBNetworkID))
			return false;
		if (!pGameServer->GetAttribute(TEXT("RecvBuffSize"), pGameConfig.RecvBuffSize))
			return false;
		if (!pGameServer->GetAttribute(TEXT("SendBuffSize"), pGameConfig.SendBuffSize))
			return false;
		if (!pGameServer->GetAttribute(TEXT("RecvThreadSum"), pGameConfig.RecvThreadSum))
			return false;
		if (!pGameServer->GetAttribute(TEXT("SendThreadSum"), pGameConfig.SendThreadSum))
			return false;
		if (!pGameServer->GetAttribute(TEXT("SleepTime"), pGameConfig.SleepTime))
			return false;
		if (!pGameServer->GetAttribute(TEXT("AcceptSleepTime"), pGameConfig.AcceptSleepTime))
			return false;
		if (!pGameServer->GetAttribute(TEXT("BeginRobotUserID"), pGameConfig.BeginRobotUserID))
			return false;
		if (!pGameServer->GetAttribute(TEXT("EndRobotUserID"), pGameConfig.EndRobotUserID))
			return false;
		if (!pGameServer->GetAttribute(TEXT("SaveDBNetworkID"), pGameConfig.SaveDBNetworkID))
			return false;
		if (!pGameServer->GetAttribute(TEXT("LogDBNetworkID"), pGameConfig.LogDBNetworkID))
			return false;
		if (!pGameServer->GetAttribute(TEXT("ListenCount"), pGameConfig.ListenCount))
			return false;
		if (!pGameServer->GetAttribute(TEXT("MaxSendCountPerFrame"), pGameConfig.MaxSendCountPerFrame))
			return false;
		if (!pGameServer->GetAttribute(TEXT("MaxAcceptNumPerFrame"), pGameConfig.MaxAcceptNumPerFrame))
			return false;
		if (!pGameServer->GetAttribute(TEXT("MaxRecvCmdCount"), pGameConfig.MaxRecvCmdCount))
			return false;
		if (!pGameServer->GetAttribute(TEXT("MaxSendCmdCount"), pGameConfig.MaxSendCmdCount))
			return false;
		if (!pGameServer->GetAttribute(TEXT("SceneHearbeatTick"), pGameConfig.SceneHearbeatTick))
			return false;

		if (m_DBConfigMap.count(pGameConfig.SaveDBNetworkID) != 1)
			return false;
		if (m_DBConfigMap.count(pGameConfig.LogDBNetworkID) != 1)
			return false;
		if (m_DBConfigMap.count(pGameConfig.DBNetworkID) != 1)
			return false;
		if (pGameConfig.SaveDBNetworkID == pGameConfig.DBNetworkID || pGameConfig.LogDBNetworkID == pGameConfig.DBNetworkID || pGameConfig.LogDBNetworkID == pGameConfig.SaveDBNetworkID) //一个Game必须有3个数据库
			return false;
		/*WHXmlNode* pDB = pGameServer->GetChildNodeByName(TEXT("DB"));
		while (pDB)
		{
			DWORD DBID = 0;
			if (!pDB->GetAttribute(TEXT("NetworkID"), DBID))
				return false;
			if (m_DBConfigMap.count(DBID) != 1)
				return false;
			pGameConfig.DBNetworkVec.push_back(DBID);
			pDB = pDB->GetNextSignelNode();
		}
		if (pGameConfig.DBNetworkVec.empty())
			return false;*/

		m_GameConfigMap.insert(HashMap<BYTE, GameServerConfig>::value_type(pGameConfig.NetworkID, pGameConfig));

		pGameServer = pGameServer->GetNextSignelNode();
	}
	return true;
}
bool FishServerConfig::LoadLogonConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pLogonServer = pFishConfig->GetChildNodeByName(TEXT("LogonServer"));
	while (pLogonServer)
	{
		LogonServerConfig m_LogonConfig;

		if (!pLogonServer->GetAttribute(TEXT("LogonID"), m_LogonConfig.LogonID))
			return false;
		/*if (!pLogonServer->GetAttribute(TEXT("ServerNetworkID"), m_LogonConfig.ServerNetworkID))
			return false;*/
		if (!pLogonServer->GetAttribute(TEXT("ClientNetworkID"), m_LogonConfig.ClientNetworkID))
			return false;
		/*if (!pLogonServer->GetAttribute(TEXT("TimeOut"), m_LogonConfig.TimeOut))
			return false;*/

		TCHAR ClientIP[IP_MAX_LENGTH] = { 0 };
		if (!pLogonServer->GetAttribute(TEXT("ClientIP"), ClientIP, IP_MAX_LENGTH))
			return false;
		UINT ClientCount = 0;
		char* pClientChar = WCharToChar(ClientIP, ClientCount);
		strncpy_s(m_LogonConfig.LogonClientListenIP, ClientCount + 1, pClientChar, ClientCount);
		free(pClientChar);

		if (!pLogonServer->GetAttribute(TEXT("ClientPort"), m_LogonConfig.LogonClientListenPort))
			return false;
		/*if (!pLogonServer->GetAttribute(TEXT("ClientRecvMaxSize"), m_LogonConfig.LogonClientRecvMaxSize))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientSendMaxSize"), m_LogonConfig.LogonClientSendMaxSize))
			return false;*/
		if (!pLogonServer->GetAttribute(TEXT("ClientBuffSize"), m_LogonConfig.LogonClientBuffSize))
			return false;

		if (!pLogonServer->GetAttribute(TEXT("ClientRecvBuffSize"), m_LogonConfig.LogonClientRecvBuffSize))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientSendBuffSize"), m_LogonConfig.LogonClientSendBuffSize))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientRecvThreadSum"), m_LogonConfig.LogonClientRecvThreadSum))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientSendThreadSum"), m_LogonConfig.LogonClientSendThreadSum))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientSleepTime"), m_LogonConfig.LogonClientSleepTime))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientAcceptSleepTime"), m_LogonConfig.LogonClientAcceptSleepTime))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientTimeOut"), m_LogonConfig.LogonClientTimeOut))
			return false;


		if (!pLogonServer->GetAttribute(TEXT("ClientListenCount"), m_LogonConfig.LogonClientListenCount))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientMaxSendCountPerFrame"), m_LogonConfig.LogonClientMaxSendCountPerFrame))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientMaxAcceptNumPerFrame"), m_LogonConfig.LogonClientMaxAcceptNumPerFrame))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientMaxRecvCmdCount"), m_LogonConfig.LogonClientMaxRecvCmdCount))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientMaxSendCmdCount"), m_LogonConfig.LogonClientMaxSendCmdCount))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("ClientSceneHearbeatTick"), m_LogonConfig.LogonClientSceneHearbeatTick))
			return false;



		//TCHAR ServerIP[IP_MAX_LENGTH] = { 0 };
		//if (!pLogonServer->GetAttribute(TEXT("ServerIP"), ServerIP, IP_MAX_LENGTH))
		//	return false;
		//UINT ServerCount = 0;
		//char* pServerChar = WCharToChar(ServerIP, ServerCount);
		//strncpy_s(m_LogonConfig.LogonServerListenIP, ServerCount + 1, pServerChar, ServerCount);
		//free(pServerChar);


		//if (!pLogonServer->GetAttribute(TEXT("ServerPort"), m_LogonConfig.LogonServerListenPort))
		//	return false;
		///*if (!pLogonServer->GetAttribute(TEXT("ServerRecvMaxSize"), m_LogonConfig.LogonServerRecvMaxSize))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerSendMaxSize"), m_LogonConfig.LogonServerSendMaxSize))
		//	return false;*/
		//if (!pLogonServer->GetAttribute(TEXT("ServerBuffSize"), m_LogonConfig.LogonServerBuffSize))
		//	return false;

		//if (!pLogonServer->GetAttribute(TEXT("ServerRecvBuffSize"), m_LogonConfig.LogonServerRecvBuffSize))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerSendBuffSize"), m_LogonConfig.LogonServerSendBuffSize))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerRecvThreadSum"), m_LogonConfig.LogonServerRecvThreadSum))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerSendThreadSum"), m_LogonConfig.LogonServerSendThreadSum))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerSleepTime"), m_LogonConfig.LogonServerSleepTime))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerAcceptSleepTime"), m_LogonConfig.LogonServerAcceptSleepTime))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerTimeOut"), m_LogonConfig.LogonServerTimeOut))
		//	return false;

		//if (!pLogonServer->GetAttribute(TEXT("ServerListenCount"), m_LogonConfig.LogonServerListenCount))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerMaxSendCountPerFrame"), m_LogonConfig.LogonServerMaxSendCountPerFrame))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerMaxAcceptNumPerFrame"), m_LogonConfig.LogonServerMaxAcceptNumPerFrame))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerMaxRecvCmdCount"), m_LogonConfig.LogonServerMaxRecvCmdCount))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerMaxSendCmdCount"), m_LogonConfig.LogonServerMaxSendCmdCount))
		//	return false;
		//if (!pLogonServer->GetAttribute(TEXT("ServerSceneHearbeatTick"), m_LogonConfig.LogonServerSceneHearbeatTick))
		//	return false;

		if (!pLogonServer->GetAttribute(TEXT("LogDBID"), m_LogonConfig.LogDBID))
			return false;
		if (!pLogonServer->GetAttribute(TEXT("DBNetworkID"), m_LogonConfig.DBNetworkID))
			return false;

		if (!GetDBServerConfig(m_LogonConfig.LogDBID) || !GetDBServerConfig(m_LogonConfig.DBNetworkID))
			return false;


		m_LogonConfigMap.insert(HashMap<BYTE, LogonServerConfig>::value_type(m_LogonConfig.LogonID, m_LogonConfig));

		pLogonServer = pLogonServer->GetNextSignelNode();
	}
	
	return true;
}
bool FishServerConfig::LoadRankConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pRankServer = pFishConfig->GetChildNodeByName(TEXT("RankServer"));
	if (!pRankServer)
	{
		ASSERT(false);
		return false;
	}
	if (!pRankServer->GetAttribute(TEXT("NetworkID"), m_RankConfig.NetworkID))
		return false;

	TCHAR IP[IP_MAX_LENGTH] = { 0 };
	if (!pRankServer->GetAttribute(TEXT("IP"), IP, IP_MAX_LENGTH))
		return false;
	UINT Count = 0;
	char* pChar = WCharToChar(IP, Count);
	strncpy_s(m_RankConfig.RankListenIP, Count + 1, pChar, Count);
	free(pChar);

	if (!pRankServer->GetAttribute(TEXT("Port"), m_RankConfig.RankListenPort))
		return false;
	/*if (!pRankServer->GetAttribute(TEXT("RecvMaxSize"), m_RankConfig.RecvMaxSize))
		return false;
	if (!pRankServer->GetAttribute(TEXT("SendMaxSize"), m_RankConfig.SendMaxSize))
		return false;*/
	if (!pRankServer->GetAttribute(TEXT("TimeOut"), m_RankConfig.TimeOut))
		return false;
	if (!pRankServer->GetAttribute(TEXT("BuffSize"), m_RankConfig.BuffSize))
		return false;
	if (!pRankServer->GetAttribute(TEXT("DBNetworkID"), m_RankConfig.DBNetworkID))
		return false;
	if (!pRankServer->GetAttribute(TEXT("RecvBuffSize"), m_RankConfig.RecvBuffSize))
		return false;
	if (!pRankServer->GetAttribute(TEXT("SendBuffSize"), m_RankConfig.SendBuffSize))
		return false;
	if (!pRankServer->GetAttribute(TEXT("RecvThreadSum"), m_RankConfig.RecvThreadSum))
		return false;
	if (!pRankServer->GetAttribute(TEXT("SendThreadSum"), m_RankConfig.SendThreadSum))
		return false;
	if (!pRankServer->GetAttribute(TEXT("SleepTime"), m_RankConfig.SleepTime))
		return false;
	if (!pRankServer->GetAttribute(TEXT("AcceptSleepTime"), m_RankConfig.AcceptSleepTime))
		return false;
	if (!pRankServer->GetAttribute(TEXT("ListenCount"), m_RankConfig.ListenCount))
		return false;
	if (!pRankServer->GetAttribute(TEXT("MaxSendCountPerFrame"), m_RankConfig.MaxSendCountPerFrame))
		return false;
	if (!pRankServer->GetAttribute(TEXT("MaxAcceptNumPerFrame"), m_RankConfig.MaxAcceptNumPerFrame))
		return false;
	if (!pRankServer->GetAttribute(TEXT("MaxRecvCmdCount"), m_RankConfig.MaxRecvCmdCount))
		return false;
	if (!pRankServer->GetAttribute(TEXT("MaxSendCmdCount"), m_RankConfig.MaxSendCmdCount))
		return false;
	if (!pRankServer->GetAttribute(TEXT("SceneHearbeatTick"), m_RankConfig.SceneHearbeatTick))
		return false;
	if (!pRankServer->GetAttribute(TEXT("LogDBID"), m_RankConfig.LogDBID))
		return false;
	if (!GetDBServerConfig(m_RankConfig.LogDBID))
		return false;
	return true;
}
bool FishServerConfig::LoadFTPConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFTPServer = pFishConfig->GetChildNodeByName(TEXT("FTPServer"));
	if (!pFTPServer)
	{
		ASSERT(false);
		return false;
	}
	if (!pFTPServer->GetAttribute(TEXT("NetworkID"), m_FtpConfig.NetworkID))
		return false;

	TCHAR IP[IP_MAX_LENGTH] = { 0 };
	if (!pFTPServer->GetAttribute(TEXT("IP"), IP, IP_MAX_LENGTH))
		return false;
	UINT Count = 0;
	char* pChar = WCharToChar(IP, Count);
	strncpy_s(m_FtpConfig.FTPListenIP, Count + 1, pChar, Count);
	free(pChar);

	TCHAR FilePath[FTP_PATH] = { 0 };
	if (!pFTPServer->GetAttribute(TEXT("FTPFilePath"), FilePath, FTP_PATH))
		return false;
	Count = 0;
	pChar = WCharToChar(FilePath, Count);
	strncpy_s(m_FtpConfig.FTPFilePath, Count + 1, pChar, Count);
	free(pChar);

	if (!pFTPServer->GetAttribute(TEXT("Port"), m_FtpConfig.FTPListenPort))
		return false;
	/*if (!pFTPServer->GetAttribute(TEXT("RecvMaxSize"), m_FtpConfig.RecvMaxSize))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("SendMaxSize"), m_FtpConfig.SendMaxSize))
		return false;*/
	if (!pFTPServer->GetAttribute(TEXT("TimeOut"), m_FtpConfig.TimeOut))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("BuffSize"), m_FtpConfig.BuffSize))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("RecvBuffSize"), m_FtpConfig.RecvBuffSize))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("SendBuffSize"), m_FtpConfig.SendBuffSize))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("RecvThreadSum"), m_FtpConfig.RecvThreadSum))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("SendThreadSum"), m_FtpConfig.SendThreadSum))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("SleepTime"), m_FtpConfig.SleepTime))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("AcceptSleepTime"), m_FtpConfig.AcceptSleepTime))
		return false;

	if (!pFTPServer->GetAttribute(TEXT("ListenCount"), m_FtpConfig.ListenCount))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("MaxSendCountPerFrame"), m_FtpConfig.MaxSendCountPerFrame))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("MaxAcceptNumPerFrame"), m_FtpConfig.MaxAcceptNumPerFrame))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("MaxRecvCmdCount"), m_FtpConfig.MaxRecvCmdCount))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("MaxSendCmdCount"), m_FtpConfig.MaxSendCmdCount))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("SceneHearbeatTick"), m_FtpConfig.SceneHearbeatTick))
		return false;
	if (!pFTPServer->GetAttribute(TEXT("LogDBID"), m_FtpConfig.LogDBID))
		return false;
	if (!GetDBServerConfig(m_FtpConfig.LogDBID))
		return false;
	return true;
}
bool FishServerConfig::LoadRobotConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pRobotServer = pFishConfig->GetChildNodeByName(TEXT("RobotServer"));
	if (!pRobotServer)
	{
		ASSERT(false);
		return false;
	}
	if (!pRobotServer->GetAttribute(TEXT("UserLogonID"), m_RobotConfig.UserLogonID))
		return false;
	if (m_LogonConfigMap.count(m_RobotConfig.UserLogonID) != 1)
	{
		ASSERT(false);
		return false;
	}
	if (!pRobotServer->GetAttribute(TEXT("OpenRobotSum"), m_RobotConfig.OpenRobotSum))
		return false;
	if (!pRobotServer->GetAttribute(TEXT("BeginRobotSum"), m_RobotConfig.BeginRobotSum))
		return false;
	return true;
}
bool FishServerConfig::LoadOperateConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pOperateServer = pFishConfig->GetChildNodeByName(TEXT("OperateServer"));
	if (!pOperateServer)
	{
		ASSERT(false);
		return false;
	}
	if (!pOperateServer->GetAttribute(TEXT("NetworkID"), m_OperateConfig.NetworkID))
		return false;

	{
		TCHAR IP[IP_MAX_LENGTH] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("IP"), IP, IP_MAX_LENGTH))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(IP, Count);
		strncpy_s(m_OperateConfig.OperateListenIP, Count + 1, pChar, Count);
		free(pChar);
	}

	{
		TCHAR strProductSecret[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("ProductSecret"), strProductSecret, CountArray(strProductSecret)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strProductSecret, Count);
		strncpy_s(m_OperateConfig.ProductSecret, Count + 1, pChar, Count);
		free(pChar);
	}


	{
		TCHAR strSMSUserName[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("SMSUserName"), strSMSUserName, CountArray(strSMSUserName)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strSMSUserName, Count);
		strncpy_s(m_OperateConfig.SMSUserName, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strSMSPassword[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("SMSPassword"), strSMSPassword, CountArray(strSMSPassword)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strSMSPassword, Count);
		strncpy_s(m_OperateConfig.SMSPassword, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strSMSInfo[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("SMSInfo"), strSMSInfo, CountArray(strSMSInfo)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strSMSInfo, Count);
		strncpy_s(m_OperateConfig.SMSInfo, Count + 1, pChar, Count);
		free(pChar);
	}

	//充值话费
	{
		TCHAR strPhonePayPartNerID[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("PhonePayPartNerID"), strPhonePayPartNerID, CountArray(strPhonePayPartNerID)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strPhonePayPartNerID, Count);
		strncpy_s(m_OperateConfig.PhonePayPartNerID, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strPhonePayOrgcode[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("PhonePayOrgcode"), strPhonePayOrgcode, CountArray(strPhonePayOrgcode)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strPhonePayOrgcode, Count);
		strncpy_s(m_OperateConfig.PhonePayOrgcode, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strPhonePayAgentId[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("PhonePayAgentId"), strPhonePayAgentId, CountArray(strPhonePayAgentId)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strPhonePayAgentId, Count);
		strncpy_s(m_OperateConfig.PhonePayAgentId, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strPhonePayPassword[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("PhonePayPassword"), strPhonePayPassword, CountArray(strPhonePayPassword)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strPhonePayPassword, Count);
		strncpy_s(m_OperateConfig.PhonePayPassword, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strPhonePayPayPassword[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("PhonePayPayPassword"), strPhonePayPayPassword, CountArray(strPhonePayPayPassword)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strPhonePayPayPassword, Count);
		strncpy_s(m_OperateConfig.PhonePayPayPassword, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strPhonePayIP[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("PhonePayIP"), strPhonePayIP, CountArray(strPhonePayIP)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strPhonePayIP, Count);
		strncpy_s(m_OperateConfig.PhonePayIP, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strPhonePayKey[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("PhonePayKey"), strPhonePayKey, CountArray(strPhonePayKey)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strPhonePayKey, Count);
		strncpy_s(m_OperateConfig.PhonePayKey, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strApppayID[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("ApppayID"), strApppayID, CountArray(strApppayID)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strApppayID, Count);
		strncpy_s(m_OperateConfig.ApppayID, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strApppayKey[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("ApppayKey"), strApppayKey, CountArray(strApppayKey)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strApppayKey, Count);
		strncpy_s(m_OperateConfig.ApppayKey, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strApppayPrvKey[IApppayPrivateKeyLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("ApppayPrvKey"), strApppayPrvKey, CountArray(strApppayPrvKey)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strApppayPrvKey, Count);
		strncpy_s(m_OperateConfig.ApppayPrvKey, Count + 1, pChar, Count);
		free(pChar);
	}

	//WeiXin
	{
		TCHAR strApppayKey[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("WeiXinAppID"), strApppayKey, CountArray(strApppayKey)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strApppayKey, Count);
		strncpy_s(m_OperateConfig.WeiXinAppID, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strApppayKey[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("WeiXinSecret"), strApppayKey, CountArray(strApppayKey)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strApppayKey, Count);
		strncpy_s(m_OperateConfig.WeiXinSecret, Count + 1, pChar, Count);
		free(pChar);
	}

	//QQ
	{
		TCHAR strApppayKey[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("QQAppID"), strApppayKey, CountArray(strApppayKey)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strApppayKey, Count);
		strncpy_s(m_OperateConfig.QQAppID, Count + 1, pChar, Count);
		free(pChar);
	}
	{
		TCHAR strApppayKey[FileXmlStrLength] = { 0 };
		if (!pOperateServer->GetAttribute(TEXT("QQAppKey"), strApppayKey, CountArray(strApppayKey)))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(strApppayKey, Count);
		strncpy_s(m_OperateConfig.QQKey, Count + 1, pChar, Count);
		free(pChar);
	}
	
	if (!pOperateServer->GetAttribute(TEXT("PhonePayPort"), m_OperateConfig.PhonePayPort))
		return false;

	if (!pOperateServer->GetAttribute(TEXT("Port"), m_OperateConfig.OperateListenPort))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("TimeOut"), m_OperateConfig.TimeOut))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("BuffSize"), m_OperateConfig.BuffSize))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("RecvBuffSize"), m_OperateConfig.RecvBuffSize))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("SendBuffSize"), m_OperateConfig.SendBuffSize))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("RecvThreadSum"), m_OperateConfig.RecvThreadSum))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("SendThreadSum"), m_OperateConfig.SendThreadSum))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("SleepTime"), m_OperateConfig.SleepTime))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("AcceptSleepTime"), m_OperateConfig.AcceptSleepTime))
		return false;

	if (!pOperateServer->GetAttribute(TEXT("ListenCount"), m_OperateConfig.ListenCount))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("MaxSendCountPerFrame"), m_OperateConfig.MaxSendCountPerFrame))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("MaxAcceptNumPerFrame"), m_OperateConfig.MaxAcceptNumPerFrame))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("MaxRecvCmdCount"), m_OperateConfig.MaxRecvCmdCount))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("MaxSendCmdCount"), m_OperateConfig.MaxSendCmdCount))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("SceneHearbeatTick"), m_OperateConfig.SceneHearbeatTick))
		return false;

	if (!pOperateServer->GetAttribute(TEXT("DBNetworkID"), m_OperateConfig.DBNetworkID))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("HttpPort"), m_OperateConfig.HttpPort))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("HttpSleepTime"), m_OperateConfig.HttpSleepTime))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("HttpThreadSum"), m_OperateConfig.HttpThreadSum))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("HttpTimeOut"), m_OperateConfig.HttpTimeOut))
		return false;

	//HttpClient
	if (!pOperateServer->GetAttribute(TEXT("HttpClientThreadNum"), m_OperateConfig.HttpClientThreadNum))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("HttpClientTimeout"), m_OperateConfig.HttpClientTimeout))
		return false;
	if (!pOperateServer->GetAttribute(TEXT("HttpClientSleepTime"), m_OperateConfig.HttpClientSleepTime))
		return false;

	BYTE IsTest = 0;
	if (!pOperateServer->GetAttribute(TEXT("OpenTest"), IsTest))
		return false;
	m_OperateConfig.OpenTest = (IsTest == 1 ? true : false);

	if (!pOperateServer->GetAttribute(TEXT("LogDBID"), m_OperateConfig.LogDBID))
		return false;
	if (!GetDBServerConfig(m_OperateConfig.LogDBID))
		return false;

	return true;
}
//bool FishServerConfig::LoadGateConfig(WHXmlNode* pFishConfig)
//{
//	//加载全部的网关的配置文件
//	WHXmlNode* pGateServer = pFishConfig->GetChildNodeByName(TEXT("GateServer"));
//	while (pGateServer)
//	{
//		GateConfig pGateConfig;
//
//		if (!pGateServer->GetAttribute(TEXT("NetworkID"), pGateConfig.NetworkID))
//			return false;
//
//		{
//			TCHAR IP[IP_MAX_LENGTH] = { 0 };
//			if (!pGateServer->GetAttribute(TEXT("LogonListenIP"), IP, IP_MAX_LENGTH))
//				return false;
//			UINT Count = 0;
//			char* pChar = WCharToChar(IP, Count);
//			strncpy_s(pGateConfig.LogonListenIP, Count + 1, pChar, Count);
//			free(pChar);
//		}
//		{
//			TCHAR IP[IP_MAX_LENGTH] = { 0 };
//			if (!pGateServer->GetAttribute(TEXT("GameListenIP"), IP, IP_MAX_LENGTH))
//				return false;
//			UINT Count = 0;
//			char* pChar = WCharToChar(IP, Count);
//			strncpy_s(pGateConfig.GameListenIP, Count + 1, pChar, Count);
//			free(pChar);
//		}
//
//		if (!pGateServer->GetAttribute(TEXT("PingPort"), pGateConfig.PingPort))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("SleepTime"), pGateConfig.SleepTime))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("RecvBuffSize"), pGateConfig.RecvBuffSize))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("SendBuffSize"), pGateConfig.SendBuffSize))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("LogonNetworkID"), pGateConfig.LogonNetworkID))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("LogonBuffSize"), pGateConfig.LogonBuffSize))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("LogonThreadSum"), pGateConfig.LogonThreadSum))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("LogonServerPort"), pGateConfig.LogonServerPort))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("LogonClientPort"), pGateConfig.LogonClientPort))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("LogonTimeOut"), pGateConfig.LogonTimeOut))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("LogonAcceptPerSec"), pGateConfig.LogonAcceptPerSec))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("GameNetworkID"), pGateConfig.GameNetworkID))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("GameBuffSize"), pGateConfig.GameBuffSize))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("GameThreadSum"), pGateConfig.GameThreadSum))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("GameServerPort"), pGateConfig.GameServerPort))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("GameClientPort"), pGateConfig.GameClientPort))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("GameTimeOut"), pGateConfig.GameTimeOut))
//			return false;
//		if (!pGateServer->GetAttribute(TEXT("GameAcceptPerSec"), pGateConfig.GameAcceptPerSec))
//			return false;
//
//		m_GateConfigMap.insert(HashMap<BYTE, GateConfig>::value_type(pGateConfig.NetworkID, pGateConfig));
//
//		pGateServer = pGateServer->GetNextSignelNode();
//	}
//	return true;
//}
bool FishServerConfig::LoadGateWayConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pGateWays = pFishConfig->GetChildNodeByName(TEXT("GateWays"));
	if (!pGateWays)
	{
		ASSERT(false);
		return false;
	}
	m_GateWayMap.clear();
	WHXmlNode* pGateWay = pGateWays->GetChildNodeByName(TEXT("GateWay"));
	while (pGateWay)
	{
		DWORD ChannelID = 0;
		{
			TCHAR ChannelStr[512] = { 0 };
			if (!pGateWay->GetAttribute(TEXT("ChannelID"), ChannelStr, CountArray(ChannelStr)))
				return false;
			if (_tcslen(ChannelStr) == 0)
			{
				ChannelID = 0;
			}
			else
			{
				ChannelID = GetCrc32(ChannelStr);
			}
		}
		DWORD GateIP = 0;
		{
			TCHAR IP[IP_MAX_LENGTH] = { 0 };
			if (!pGateWay->GetAttribute(TEXT("GateIp"), IP, IP_MAX_LENGTH))
				return false;
			if (_tcslen(IP) == 0)
			{
				GateIP = 0;
			}
			else
			{
				UINT Count = 0;
				char* pChar = WCharToChar(IP, Count);
				//GateIP = inet_addr(pChar);
				inet_pton(AF_INET, pChar, (void *)&GateIP);
				free(pChar);
			}
		}
		WORD GatePort = 0;
		if (!pGateWay->GetAttribute(TEXT("GatePort"), GatePort))
			return false;

		GateWay pWay;
		pWay.ChannelID = ChannelID;
		pWay.Ip = GateIP;
		pWay.Port = GatePort;
		m_GateWayMap.insert(HashMap<DWORD, GateWay>::value_type(ChannelID,pWay));

		pGateWay = pGateWay->GetNextSignelNode();
	}
	return true;
}
bool FishServerConfig::LoadControlServerConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pControlServer = pFishConfig->GetChildNodeByName(TEXT("ControlServer"));
	if (!pControlServer)
	{
		ASSERT(false);
		return false;
	}
	if (!pControlServer->GetAttribute(TEXT("ControlID"), m_ControlServerConfig.ControlID))
		return false;


	if (!pControlServer->GetAttribute(TEXT("DBNetworkID"), m_ControlServerConfig.DBNetworkID))
		return false;

	{
		TCHAR IP[IP_MAX_LENGTH] = { 0 };
		if (!pControlServer->GetAttribute(TEXT("ControlServerIP"), IP, IP_MAX_LENGTH))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(IP, Count);
		strncpy_s(m_ControlServerConfig.ControlServerListenIP, Count + 1, pChar, Count);
		free(pChar);
	}
	if (!pControlServer->GetAttribute(TEXT("ControlServerNetworkID"), m_ControlServerConfig.ControlServerNetworkID))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerPort"), m_ControlServerConfig.ControlServerListenPort))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerTimeOut"), m_ControlServerConfig.ControlServerTimeOut))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerBuffSize"), m_ControlServerConfig.ControlServerBuffSize))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerRecvBuffSize"), m_ControlServerConfig.ControlServerRecvBuffSize))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerSendBuffSize"), m_ControlServerConfig.ControlServerSendBuffSize))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerRecvThreadSum"), m_ControlServerConfig.ControlServerRecvThreadSum))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerSendThreadSum"), m_ControlServerConfig.ControlServerSendThreadSum))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerSleepTime"), m_ControlServerConfig.ControlServerSleepTime))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerAcceptSleepTime"), m_ControlServerConfig.ControlServerAcceptSleepTime))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerListenCount"), m_ControlServerConfig.ControlServerListenCount))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerMaxSendCountPerFrame"), m_ControlServerConfig.ControlServerMaxSendCountPerFrame))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerMaxAcceptNumPerFrame"), m_ControlServerConfig.ControlServerMaxAcceptNumPerFrame))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerMaxRecvCmdCount"), m_ControlServerConfig.ControlServerMaxRecvCmdCount))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerMaxSendCmdCount"), m_ControlServerConfig.ControlServerMaxSendCmdCount))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlServerSceneHearbeatTick"), m_ControlServerConfig.ControlServerSceneHearbeatTick))
		return false;


	{
		TCHAR IP[IP_MAX_LENGTH] = { 0 };
		if (!pControlServer->GetAttribute(TEXT("ControlClientIP"), IP, IP_MAX_LENGTH))
			return false;
		UINT Count = 0;
		char* pChar = WCharToChar(IP, Count);
		strncpy_s(m_ControlServerConfig.ControlClientListenIP, Count + 1, pChar, Count);
		free(pChar);
	}
	if (!pControlServer->GetAttribute(TEXT("ControlClientNetworkID"), m_ControlServerConfig.ControlClientNetworkID))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientPassword"), m_ControlServerConfig.ControlClientPassword))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientPort"), m_ControlServerConfig.ControlClientListenPort))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientTimeOut"), m_ControlServerConfig.ControlClientTimeOut))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientBuffSize"), m_ControlServerConfig.ControlClientBuffSize))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientRecvBuffSize"), m_ControlServerConfig.ControlClientRecvBuffSize))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientSendBuffSize"), m_ControlServerConfig.ControlClientSendBuffSize))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientRecvThreadSum"), m_ControlServerConfig.ControlClientRecvThreadSum))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientSendThreadSum"), m_ControlServerConfig.ControlClientSendThreadSum))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientSleepTime"), m_ControlServerConfig.ControlClientSleepTime))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientAcceptSleepTime"), m_ControlServerConfig.ControlClientAcceptSleepTime))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientListenCount"), m_ControlServerConfig.ControlClientListenCount))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientMaxSendCountPerFrame"), m_ControlServerConfig.ControlClientMaxSendCountPerFrame))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientMaxAcceptNumPerFrame"), m_ControlServerConfig.ControlClientMaxAcceptNumPerFrame))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientMaxRecvCmdCount"), m_ControlServerConfig.ControlClientMaxRecvCmdCount))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientMaxSendCmdCount"), m_ControlServerConfig.ControlClientMaxSendCmdCount))
		return false;
	if (!pControlServer->GetAttribute(TEXT("ControlClientSceneHearbeatTick"), m_ControlServerConfig.ControlClientSceneHearbeatTick))
		return false;
	if (!pControlServer->GetAttribute(TEXT("LogDBID"), m_ControlServerConfig.LogDBID))
		return false;
	if (!GetDBServerConfig(m_ControlServerConfig.LogDBID))
		return false;
	return true;
}
DBServerConfig*	FishServerConfig::GetDBServerConfig(BYTE DBNetworkID)
{
	HashMap<BYTE, DBServerConfig>::iterator Iter = m_DBConfigMap.find(DBNetworkID);
	if (Iter == m_DBConfigMap.end())
		return NULL;
	else
		return &Iter->second;
}
GameServerConfig* FishServerConfig::GetGameServerConfig(BYTE GameNetworkID)
{
	HashMap<BYTE, GameServerConfig>::iterator Iter = m_GameConfigMap.find(GameNetworkID);
	if (Iter == m_GameConfigMap.end())
		return NULL;
	else
		return &Iter->second;
}
//GateConfig* FishServerConfig::GetGateConfig(BYTE GateConfigID)
//{
//	HashMap<BYTE, GateConfig>::iterator Iter = m_GateConfigMap.find(GateConfigID);
//	if (Iter == m_GateConfigMap.end())
//		return NULL;
//	else
//		return &Iter->second;
//}
LogonServerConfig* FishServerConfig::GetLogonServerConfig(BYTE LogonID)
{
	HashMap<BYTE, LogonServerConfig>::iterator Iter = m_LogonConfigMap.find(LogonID);
	if (Iter == m_LogonConfigMap.end())
		return NULL;
	else
		return &Iter->second;
}
bool FishServerConfig::IsLogonServerConfigID(BYTE ConfigID)
{
	HashMap<BYTE, LogonServerConfig>::iterator Iter = m_LogonConfigMap.find(ConfigID);
	if (Iter == m_LogonConfigMap.end())
		return false;
	else
		return true;
}
bool FishServerConfig::IsGameServerConfigID(BYTE ConfigID)
{
	HashMap<BYTE, GameServerConfig>::iterator Iter = m_GameConfigMap.find(ConfigID);
	if (Iter == m_GameConfigMap.end())
		return false;
	else
		return true;
}
GateWay* FishServerConfig::GetGateWay(DWORD ChannelID)
{
	HashMap<DWORD, GateWay>::iterator Iter = m_GateWayMap.find(ChannelID);
	if (Iter == m_GateWayMap.end())
		return NULL;
	else
		return &Iter->second;
}
bool FishServerConfig::GetIsOperateTest()
{
	return m_OperateConfig.OpenTest;
}