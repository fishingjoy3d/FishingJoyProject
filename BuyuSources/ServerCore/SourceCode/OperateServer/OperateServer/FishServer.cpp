#include "stdafx.h"
#include "FishServer.h"
#include "..\CommonFile\FishServerConfig.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
FishServer g_FishServer;
void SendLogDB(NetCmd* pCmd)
{
	g_FishServer.SendNetCmdToLogDB(pCmd);
}
FishServer::FishServer()
{
	m_IsClose = false;
	m_OperateNetworkID = 0;
	m_CenterTcpStates = false;
	m_DBTcpStates = false;
	m_IsReloadConfig = false;
	m_ControlIsConnect = false;

	m_LogDBIsConnect = false;
}
FishServer::~FishServer()
{

}
bool FishServer::InitServer()
{
	m_pDump.OnInit();
	if (!g_FishServerConfig.LoadServerConfigFile())
	{
		ShowInfoToWin("服务器配置文件读取失败");
		return false;
	}
	if (!m_FishConfig.LoadConfigFilePath())
	{
		ASSERT(false);
		return false;
	}


	OperateConfig* pOperateConfig = g_FishServerConfig.GetOperateConfig();
	if (!pOperateConfig)
	{
		ASSERT(false);
		return false;
	}
	if (!pOperateConfig)
	{
		ASSERT(false);
		return false;
	}
	m_OperateNetworkID = pOperateConfig->NetworkID;
	m_IOSPayManager.OnInit();
	if (!ConnectControl())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectControl())
				break;
		}
	}
	if (!ConnectDB())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectDB())
				break;
		}
	}
	if (!ConnectLogDB())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectLogDB())
				break;
		}
	}
	if (!ConnectCenter())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectCenter())
				break;
		}
	}
	if (!ConnectFTP())
	{
		while (true)
		{
			Sleep(5);
			if (ConnectFTP())
				break;
		}
	}

	m_PhonePayManager.OnInit();
	m_UseRMBManager.OnInit();
	
	m_OrderOnlyManager.OnInit();
	return true;
}
void FishServer::OnFinish()
{
	if (!ConnectHttp())
	{
		ASSERT(false);
		return;
	}
	if (!ConnectHttpClient())
	{
		ASSERT(false);
		return;
	}
	ShowInfoToWin("Http服务启动成功");
	if (!ConnectClient())
	{
		ASSERT(false);
		return;
	}
	return;
}
bool FishServer::ConnectLogDB()
{
	OperateConfig* pOperateConfig = g_FishServerConfig.GetOperateConfig();
	if (!pOperateConfig)
	{
		return false;
	}
	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pOperateConfig->LogDBID);
	if (!pDBConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pDBData;
	pDBData.BuffSize = pDBConfig->BuffSize;
	pDBData.Port = pDBConfig->DBListenPort;
	pDBData.SleepTime = pDBConfig->SleepTime;
	pDBData.SocketRecvSize = pDBConfig->RecvBuffSize;
	pDBData.SocketSendSize = pDBConfig->SendBuffSize;
	pDBData.ThreadNum = 1;
	pDBData.Timeout = pDBConfig->TimeOut;
	pDBData.CmdHearbeat = 0;
	pDBData.SendArraySize = pDBConfig->MaxSendCmdCount;
	pDBData.RecvArraySize = pDBConfig->MaxRecvCmdCount;
	if (!m_LogDBTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_OperateNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("Log数据库连接失败");
		return false;
	}
	OnTcpClientConnect(&m_LogDBTcp);
	return true;
}
bool FishServer::ConnectFTP()
{
	FTPServerConfig* pFtpConfig = g_FishServerConfig.GetFTPServerConfig();
	if (!pFtpConfig)
	{
		return false;
	}
	ClientInitData pFtpData;
	pFtpData.BuffSize = pFtpConfig->BuffSize;
	pFtpData.Port = pFtpConfig->FTPListenPort;
	pFtpData.SleepTime = pFtpConfig->SleepTime;
	pFtpData.SocketRecvSize = pFtpConfig->RecvBuffSize;
	pFtpData.SocketSendSize = pFtpConfig->SendBuffSize;
	pFtpData.ThreadNum = 1;
	pFtpData.Timeout = pFtpConfig->TimeOut;
	pFtpData.CmdHearbeat = 0;
	pFtpData.SendArraySize = pFtpConfig->MaxSendCmdCount;
	pFtpData.RecvArraySize = pFtpConfig->MaxRecvCmdCount;
	if (!m_FTPTcp.Init(pFtpConfig->FTPListenIP, pFtpData, (void*)&m_OperateNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("FTP连接失败");
		return false;
	}
	OnTcpClientConnect(&m_FTPTcp);
	return true;

}
bool FishServer::MainUpdate()
{
	while (!m_IsClose)
	{
		DWORD dwTimer = timeGetTime();

		if (m_IsReloadConfig)
		{
			m_IsReloadConfig = false;
			OnReloadConfig();//重新加载配置文件
		}

		UpdateInfoToControl(dwTimer);

		OnAddClient();

		m_RealNameVerificationManager.OnUpdate(dwTimer);
		//m_BindPhoneManager.OnUpdateByMin(dwTimer);
		m_PhoneSMSManager.OnUpdate(dwTimer);
		m_BindPhoneManager.OnUpdateSendPhoneSMS(dwTimer);
		m_PhoneLogon.OnUpdate(dwTimer);
		m_WeiXinLogon.Update();
		m_QQLogon.Update();
		
		//1.处理GameServer 的命令
		if (!m_ClintList.empty())
		{
			HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClintList.begin();
			for (; Iter != m_ClintList.end();)
			{
				if (!Iter->second)
				{
					++Iter;
					continue;
				}
				if (Iter->second->Removed)
				{
					OnTcpServerLeave(m_OperateNetworkID, Iter->second);
					m_ClientTcp.Kick(Iter->second);
					Iter = m_ClintList.erase(Iter);
					continue;
				}
				int Sum = 0;
				while (Iter->second->RecvList.HasItem() && Sum < Msg_OnceSum)
				{
					NetCmd* pCmd = Iter->second->RecvList.GetItem();
					//处理网络命令 客户端发送来的登陆 注册 等命令
					HandleGameServerMsg(Iter->second, pCmd);
					free(pCmd);
					++Sum;
				}
				++Iter;
			}
		}
		//2.中央服务器
		if (m_CenterTcp.IsConnected())
		{
			if (!m_CenterTcpStates)
				OnTcpClientConnect(&m_CenterTcp);
			NetCmd* pCmd = m_CenterTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleCenterMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_CenterTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleCenterMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_CenterTcpStates)
				OnTcpClientLeave(&m_CenterTcp);

			ConnectCenter();
		}
		//3.中央服务器
		if (m_DBTcp.IsConnected())
		{
			if (!m_DBTcpStates)
				OnTcpClientConnect(&m_DBTcp);
			NetCmd* pCmd = m_DBTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleDBMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_DBTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleDBMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_DBTcpStates)
				OnTcpClientLeave(&m_DBTcp);

			ConnectDB();
		}
		//4.FTP
		if (m_FTPTcp.IsConnected())
		{
			if (!m_FTPTcpStates)
				OnTcpClientConnect(&m_FTPTcp);
			NetCmd* pCmd = m_FTPTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleFtpMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_FTPTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleFtpMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_FTPTcpStates)
				OnTcpClientLeave(&m_FTPTcp);

			ConnectFTP();
		}

		
		//Control
		if (m_ControlTcp.IsConnected())
		{
			if (!m_ControlIsConnect)
				OnTcpClientConnect(&m_ControlTcp);
			NetCmd* pCmd = m_ControlTcp.GetCmd();
			int Sum = 0;
			while (pCmd && Sum < Msg_OnceSum)
			{
				HandleControlMsg(pCmd);
				free(pCmd);
				++Sum;
				pCmd = m_ControlTcp.GetCmd();
			}
			if (pCmd)
			{
				HandleControlMsg(pCmd);
				free(pCmd);
			}
		}
		else
		{
			if (m_ControlIsConnect)
				OnTcpClientLeave(&m_ControlTcp);

			//重新连接DB
			ConnectControl();
		}

		if (m_LogDBTcp.IsConnected())//LogDB 重新连接
		{
			if (!m_LogDBIsConnect)
				OnTcpClientConnect(&m_LogDBTcp);
		}
		else
		{
			if (m_LogDBIsConnect)
				OnTcpClientLeave(&m_LogDBTcp);

			//重新连接DB
			ConnectLogDB();
		}

		//Http
		while (m_HttpServer.HasRecvData())
		{
			HttpRecvData pData = m_HttpServer.GetRecvList().GetItem();
			HandleHttpMsg(&pData);
		}

		//HttpClient
		HttpClientResult* pResult;
		while (m_HttpClient.GetResult(pResult) && pResult)
		{
			HandleHttpClientMsg(pResult);
			m_HttpClient.FreeResult(pResult);
		}

		Sleep(1);
	}
	return OnDestroy();

}
bool FishServer::OnDestroy()
{
	HandleAllMsg();
	return true;
}
void FishServer::ShowInfoToWin(const char *pcStr, ...)
{
	if (!pcStr)
	{
		ASSERT(false);
		return;
	}
	va_list var;
	va_start(var, pcStr);
	UINT nCount = _vscprintf(pcStr, var);
	char *pBuffer = new char[nCount + 1];
	vsprintf_s(pBuffer, nCount + 1, pcStr, var);
	printf("%s\n", pBuffer);
	//std::cout << pBuffer;
	//std::cout << "\n";
	SAFE_DELETE_ARR(pBuffer);
	va_end(var);
}
void FishServer::Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	//输出 客户端离开的状态
	switch (rt)
	{
	case REMOVE_NONE:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为None",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_NORMAL:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为普通",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_RECV_ERROR:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为接收错误",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_SEND_ERROR:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为发送错误",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_TIMEOUT:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为超时",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_CMD_SEND_OVERFLOW:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为发送太多",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_CMD_RECV_OVERFLOW:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为接收太多",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_CMD_SIZE_ERROR:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为命令大小",pClient->OutsideExtraData);
		}
		break;
	case REMOVE_RECVBACK_NOT_SPACE:
		{
			ShowInfoToWin("客户端 ID:%d 离开原因为接收缓冲区溢出",pClient->OutsideExtraData);
		}
		break;
	}
}
uint FishServer::CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize)
{
	//中央服务器只有在FTP 和 DB都连接成功后才工作
	if (m_CenterTcp.IsConnected() && m_DBTcp.IsConnected())
		return CONNECT_CHECK_OK;
	else
		return CONNECT_CHECK_FAILED;
}
bool FishServer::NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize)
{
	if (!pClient)
	{
		ASSERT(false);
		return false;
	}
	if (!pData || recvSize != sizeof(BYTE))
	{
		m_ClientTcp.Kick(pClient);
		ASSERT(false);
		return false;
	}
	AfxNetworkClientOnce* pOnce = new AfxNetworkClientOnce();
	if (!pOnce)
	{
		m_ClientTcp.Kick(pClient);
		ASSERT(false);
		return false;
	}
	pOnce->SeverID = SeverID;
	pOnce->pClient = pClient;
	pOnce->pPoint = (void*)malloc(recvSize);
	if (!pOnce->pPoint)
	{
		m_ClientTcp.Kick(pClient);
		ASSERT(false);
		free(pOnce->pPoint);
		delete pOnce;
		return false;
	}
	memcpy_s(pOnce->pPoint, recvSize, pData, recvSize);
	pOnce->Length = recvSize;
	m_AfxAddClient.AddItem(pOnce);
	return true;
}
void FishServer::OnAddClient()
{
	while (m_AfxAddClient.HasItem())
	{
		AfxNetworkClientOnce* pOnce = m_AfxAddClient.GetItem();
		if (pOnce->SeverID == m_OperateNetworkID)
		{
			if (!pOnce->pPoint || pOnce->Length != sizeof(BYTE))
			{
				m_ClientTcp.Kick(pOnce->pClient);
				ASSERT(false);
				if (pOnce->pPoint)
					free(pOnce->pPoint);
				delete pOnce;
				continue;
			}
			BYTE ClientID = *((BYTE*)pOnce->pPoint);
			if (ClientID == 0)
			{
				m_ClientTcp.Kick(pOnce->pClient);
				//ASSERT(false);
				if (pOnce->pPoint)
					free(pOnce->pPoint);
				delete pOnce;
				continue;
			}
			pOnce->pClient->OutsideExtraData = ClientID;//玩家的ID
			m_ClintList.insert(HashMap<BYTE, ServerClientData*>::value_type(ClientID, pOnce->pClient));
			//触发玩家加入的事件
			OnTcpServerJoin(pOnce->SeverID, pOnce->pClient);
			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
		}
		else
		{
			//ASSERT(false);
			if (pOnce->pPoint)
				free(pOnce->pPoint);
			delete pOnce;
		}
	}
}


void FishServer::SendNetCmdToClient(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pCmd || !pClient)
	{
		ASSERT(false);
		return;
	}
	if (!m_ClientTcp.Send(pClient, pCmd))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToCenterServer(NetCmd* pCmd)
{
	if (!m_CenterTcp.IsConnected() || !pCmd)
		return;
	if (!m_CenterTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToDB(NetCmd* pCmd)
{
	if (!m_DBTcp.IsConnected() || !pCmd)
		return;
	if (!m_DBTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
ServerClientData* FishServer::GetUserClientDataByIndex(BYTE IndexID)
{
	HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClintList.find(IndexID);
	if (Iter == m_ClintList.end() || !Iter->second)
		return NULL;
	else
		return Iter->second;
}
bool FishServer::ConnectHttp()
{
	OperateConfig* pOperateConfig = g_FishServerConfig.GetOperateConfig();
	if (!pOperateConfig)
	{
		ASSERT(false);
		return false;
	}
	//处理HTTP的连接
	HttpServerInitData pHttpInitData;
	pHttpInitData.Port = pOperateConfig->HttpPort;
	pHttpInitData.SleepTime = pOperateConfig->HttpSleepTime;
	pHttpInitData.ThreadNum = pOperateConfig->HttpThreadSum;
	pHttpInitData.Timeout = pOperateConfig->TimeOut;

	//添加处理命令
	pHttpInitData.AddWebAddr(HT_Recharge, "pay_callback_lj.clkj?", "success", true, "orderid,price,ChannelCode,channelOrderid,channelLabel,sign,callbackinfo");//,sign2,version,freePrice,sdkCode 可选参数
	pHttpInitData.AddWebAddr(HT_PhonePay, "phone_pay_callback.clkj?", "success", true, "account_no,face_price,order_no,order_time,order_type,pay_money,profit,status,sign_type,stream_id");
	pHttpInitData.AddWebAddr(HT_NormalPay, "pay_callback_self.clkj?", "SUCCESS", true);
	if (!m_HttpServer.Init(pHttpInitData))
	{
		ShowInfoToWin("Http服务器启动失败");
		return false;
	}
	return true;
}
bool FishServer::ConnectHttpClient()
{
	OperateConfig* pOperateConfig = g_FishServerConfig.GetOperateConfig();
	if (!pOperateConfig)
	{
		ASSERT(false);
		return false;
	}
	//1.初始化HttpClient
	HttpClientInitData pInit;
	pInit.SleepTime = pOperateConfig->HttpClientSleepTime;
	pInit.ThreadNum = pOperateConfig->HttpClientThreadNum;
	pInit.Timeout = pOperateConfig->HttpClientTimeout;
	if (!m_HttpClient.Init(pInit))
	{
		ASSERT(false);
		return false;
	}
	/*if (g_FishServerConfig.GetIsOperateTest())
	{
		m_ChannelLogonID = 0;
		m_PhoneSMSID = 0;
		m_PhonePayID = 0;
		m_NormalRechargeID = 0;
		m_IOSPayID = 0;
		m_IOSPayTestID = 0;
		m_WeiXinTokenID = 0;
		m_QQTokenID = 0;

		m_WeiXinUserInfoID = 0;
		m_WeiXinFaceImgID = 0;

		m_QQUserInfoID = 0;
		m_QQFaceImgID = 0;
	}
	else*/
	{
		//启动HTTP 客户端
		//1.登陆 HCT_Logon
		m_ChannelLogonID = m_HttpClient.AddHost("gameproxy.xinmei365.com");
		//3.短信 HCT_PhoneSMS
		m_PhoneSMSID = m_HttpClient.AddHost("api.sms.cn");
		//4.话费 HCT_PhonePay
		m_PhonePayID = m_HttpClient.AddIP(pOperateConfig->PhonePayIP, pOperateConfig->PhonePayPort);//IP Port 配置文件配置
		//5.普通充值
		m_NormalRechargeID = m_HttpClient.AddHost("ipay.iapppay.com", 9999);
		//6.IOS充值
		//m_IOSPayID = m_HttpClient.AddHost("buy.itunes.apple.com");
		//m_IOSPayTestID = m_HttpClient.AddHost("sandbox.iTunes.Apple.com");
		m_IOSPayID = m_HttpClient.AddIP("127.0.0.1", 45435);
		//7.
		m_WeiXinTokenID = m_HttpClient.AddIP("127.0.0.1", 45435);//https api.weixin.qq.com
		//8.
		m_QQTokenID = m_HttpClient.AddIP("127.0.0.1", 45435);//https  graph.qq.com


		m_WeiXinUserInfoID = m_HttpClient.AddIP("127.0.0.1", 45435);
		m_WeiXinFaceImgID = m_HttpClient.AddIP("127.0.0.1", 45435);
		//m_WeiXinUserTokenID = m_HttpClient.AddIP("127.0.0.1", 45435);

		m_QQUserInfoID = m_HttpClient.AddIP("127.0.0.1", 45435);
		m_QQFaceImgID = m_HttpClient.AddIP("127.0.0.1", 45435);

	}
	return true;
}
bool FishServer::ConnectCenter()
{
	CenterServerConfig* pCenterConfig = g_FishServerConfig.GetCenterServerConfig();
	if (!pCenterConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pCenterData;
	pCenterData.BuffSize = pCenterConfig->BuffSize;
	pCenterData.Port = pCenterConfig->CenterListenPort;
	pCenterData.SleepTime = pCenterConfig->SleepTime;
	pCenterData.SocketRecvSize = pCenterConfig->RecvBuffSize;
	pCenterData.SocketSendSize = pCenterConfig->SendBuffSize;
	pCenterData.ThreadNum = 1;
	pCenterData.Timeout = pCenterConfig->TimeOut;
	pCenterData.CmdHearbeat = 0;

	pCenterData.SendArraySize = pCenterConfig->MaxSendCmdCount;
	pCenterData.RecvArraySize = pCenterConfig->MaxRecvCmdCount;


	if (!m_CenterTcp.Init(pCenterConfig->CenterListenIP, pCenterData, (void*)&m_OperateNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("中央服务器连接失败");
		return false;
	}
	OnTcpClientConnect(&m_CenterTcp);
	return true;
}
bool FishServer::ConnectDB()
{
	OperateConfig* pOperateConfig = g_FishServerConfig.GetOperateConfig();
	if (!pOperateConfig)
	{
		ASSERT(false);
		return false;
	}
	DBServerConfig* pDBConfig = g_FishServerConfig.GetDBServerConfig(pOperateConfig->DBNetworkID);
	if (!pDBConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pDBData;
	pDBData.BuffSize = pDBConfig->BuffSize;
	pDBData.Port = pDBConfig->DBListenPort;
	pDBData.SleepTime = pDBConfig->SleepTime;
	pDBData.SocketRecvSize = pDBConfig->RecvBuffSize;
	pDBData.SocketSendSize = pDBConfig->SendBuffSize;
	pDBData.ThreadNum = 1;
	pDBData.Timeout = pDBConfig->TimeOut;
	pDBData.CmdHearbeat = 0;
	pDBData.SendArraySize = pDBConfig->MaxSendCmdCount;
	pDBData.RecvArraySize = pDBConfig->MaxRecvCmdCount;


	if (!m_DBTcp.Init(pDBConfig->DBListenIP, pDBData, (void*)&m_OperateNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("数据库连接失败");
		return false;
	}
	OnTcpClientConnect(&m_DBTcp);
	return true;
}
bool FishServer::ConnectClient()
{
	OperateConfig* pOperateConfig = g_FishServerConfig.GetOperateConfig();
	if (!pOperateConfig)
	{
		ASSERT(false);
		return false;
	}
	ServerInitData pCLientData;
	pCLientData.ServerID = pOperateConfig->NetworkID;
	pCLientData.BuffSize = pOperateConfig->BuffSize;
	pCLientData.CmdHearbeat = 0;
	pCLientData.Port = pOperateConfig->OperateListenPort;
	pCLientData.SocketRecvSize = pOperateConfig->RecvBuffSize;
	pCLientData.SocketSendSize = pOperateConfig->SendBuffSize;
	pCLientData.Timeout = pOperateConfig->TimeOut;
	pCLientData.RecvThreadNum = pOperateConfig->RecvThreadSum;
	pCLientData.SendThreadNum = pOperateConfig->SendThreadSum;
	pCLientData.SleepTime = pOperateConfig->SleepTime;
	pCLientData.AcceptSleepTime = pOperateConfig->AcceptSleepTime;
	pCLientData.AcceptRecvData = true;

	pCLientData.SceneHearbeatTick = pOperateConfig->SceneHearbeatTick;
	pCLientData.MaxSendCmdCount = pOperateConfig->MaxSendCmdCount;
	pCLientData.MaxAcceptNumPerFrame = pOperateConfig->MaxAcceptNumPerFrame;
	pCLientData.MaxRecvCmdCount = pOperateConfig->MaxRecvCmdCount;
	pCLientData.MaxSendCountPerFrame = pOperateConfig->MaxSendCountPerFrame;
	pCLientData.ListenCount = pOperateConfig->ListenCount;
	m_ClientTcp.SetCmdHandler(this);
	if (!m_ClientTcp.Init(pCLientData))
	{
		ShowInfoToWin("对内网络库启动失败");
		return false;
	}
	
	return true;
}
void FishServer::HandleAllMsg()
{
	if (!m_ClintList.empty())
	{
		HashMap<BYTE, ServerClientData*>::iterator Iter = m_ClintList.begin();
		for (; Iter != m_ClintList.end();)
		{
			if (!Iter->second)
				continue;
			while (Iter->second->RecvList.HasItem())
			{
				NetCmd* pCmd = Iter->second->RecvList.GetItem();
				//处理网络命令 客户端发送来的登陆 注册 等命令
				HandleGameServerMsg(Iter->second, pCmd);
				free(pCmd);
			}
			++Iter;
		}
	}

	NetCmd* pCmd = m_CenterTcp.GetCmd();
	while (pCmd)
	{
		HandleCenterMsg(pCmd);
		free(pCmd);
		pCmd = m_CenterTcp.GetCmd();
	}

	pCmd = m_DBTcp.GetCmd();
	while (pCmd)
	{
		HandleDBMsg(pCmd);
		free(pCmd);
		pCmd = m_DBTcp.GetCmd();
	}

	pCmd = m_ControlTcp.GetCmd();
	while (pCmd)
	{
		HandleControlMsg(pCmd);
		free(pCmd);
		pCmd = m_ControlTcp.GetCmd();
	}

	int HttpHandle = 0;
	while (m_HttpServer.HasRecvData() && HttpHandle<Msg_OnceSum)
	{
		HttpRecvData pData = m_HttpServer.GetRecvList().GetItem();
		HandleHttpMsg(&pData);

		++HttpHandle;
	}
}
void FishServer::OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	if (ServerID == m_OperateNetworkID)
	{
		ShowInfoToWin("一个GameServer离开了OperateServer");
	}
	else
	{
		ASSERT(false);
		return;
	}
	return;
}
void FishServer::OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	if (ServerID == m_OperateNetworkID)
	{
		ShowInfoToWin("一个GameServer加入了OperateServer");
	}
	else
	{
		ASSERT(false);
		return;
	}
	return;
}
void FishServer::OnTcpClientConnect(TCPClient* pClient)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	if (pClient == &m_CenterTcp)
	{
		m_CenterTcpStates = true;
		ShowInfoToWin("已经与中央服务器连接成功了");
	}
	else if (pClient == &m_DBTcp)
	{
		m_DBTcpStates = true;
		ShowInfoToWin("已经与数据库连接成功了");
	}
	else if (pClient == &m_ControlTcp)
	{
		m_ControlIsConnect = true;
		ShowInfoToWin("已经与ControlServer连接成功了");
	}
	else if (pClient == &m_LogDBTcp)
	{
		m_LogDBIsConnect = true;
		ShowInfoToWin("已经与Log数据库连接成功了");
	}
	else if (pClient == &m_FTPTcp)
	{
		m_FTPTcpStates = true;
		ShowInfoToWin("已经与Ftp连接成功了");
	}
	else
	{
		ASSERT(false);
		return;
	}
}
void FishServer::OnTcpClientLeave(TCPClient* pClient)
{
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	if (pClient == &m_CenterTcp)
	{
		m_CenterTcpStates = false;
		m_CenterTcp.Shutdown();
		ShowInfoToWin("已经与中央服务器断开连接了");
	}
	else if (pClient == &m_DBTcp)
	{
		m_DBTcpStates = false;
		m_DBTcp.Shutdown();
		ShowInfoToWin("已经与数据库断开连接了");
	}
	else if (pClient == &m_ControlTcp)
	{
		m_ControlIsConnect = false;
		m_ControlTcp.Shutdown();
		ShowInfoToWin("已经与ControlServer断开连接了");
	}
	else if (pClient == &m_LogDBTcp)
	{
		ShowInfoToWin("已经与Log数据库断开连接了");
		m_LogDBIsConnect = false;
		m_LogDBTcp.Shutdown();
	}
	else if (pClient == &m_FTPTcp)
	{
		m_FTPTcpStates = false;
		ShowInfoToWin("已经与Ftp断开连接了");
		m_FTPTcp.Shutdown();
	}
	else
	{
		ASSERT(false);
		return;
	}
}
bool FishServer::HandleGameServerMsg(ServerClientData* pClient, NetCmd* pCmd)
{
	if (!pClient || !pCmd)
	{
		ASSERT(false);
		return false;
	}
	if (pCmd->CmdType == Main_Operate)
	{
		//处理来自GameServer的命令
		switch (pCmd->SubCmdType)
		{
		case GO_BuyEntityItem:
			{
				GO_Cmd_BuyEntityItem* pMsg = (GO_Cmd_BuyEntityItem*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				return m_BuyEntityManager.OnBuyEntityItem(pClient,pMsg);
			}
			break;
		case GO_RealNameVerification:
			{
				GO_Cmd_RealNameVerification* pMsg = (GO_Cmd_RealNameVerification*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				return m_RealNameVerificationManager.OnAddRealNameVerificationOnce(pClient, pMsg);
			}
			break;
		case GO_GetPhoneVerificationNum:
			{
				GO_Cmd_GetPhoneVerificationNum* pMsg = (GO_Cmd_GetPhoneVerificationNum*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				return m_BindPhoneManager.OnGetPhoneVerificationNumber(pClient, pMsg);
			}
			break;
		case GO_BindPhone:
			{
				GO_Cmd_BindPhone* pMsg = (GO_Cmd_BindPhone*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				return m_BindPhoneManager.OnBindPhontByVerificationNumber(pClient, pMsg);
			}
			break;
		case GO_BindEmail:
			{
				GO_Cmd_BindEmail* pMsg = (GO_Cmd_BindEmail*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				return m_BindEmailManager.OnBindEmail(pClient, pMsg);
			}
			break;
		case GO_PhonePay:
			{
				GO_Cmd_PhonePay* pMsg = (GO_Cmd_PhonePay*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				m_PhonePayManager.OnHandleRolePhonePay(pClient,pMsg);
				return true;
			}
			break;
		case GO_AddNormalOrderID:
			{
				GO_Cmd_AddNormalOrderID* pMsg = (GO_Cmd_AddNormalOrderID*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				if (!m_NormalRechargeManager.OnHandleAddOrder(pMsg->dwUserID, pMsg->OrderID, pMsg->ShopID))
				{
					OC_Cmd_AddNormalOrderID msg;
					SetMsgInfo(msg, GetMsgType(Main_Operate, OC_AddNormalOrderID), sizeof(OC_Cmd_AddNormalOrderID));
					msg.dwUserID = pMsg->dwUserID;
					msg.OrderID = pMsg->OrderID;
					msg.ShopID = pMsg->ShopID;
					msg.Result = false;
					msg.Sign[0] = 0;
					msg.Transid[0] = 0;
					SendNetCmdToCenterServer(&msg);
				}
				return true;
			}
			break;
		/*case GO_UseRMB:
			{
				GO_Cmd_UseRMB* pMsg = (GO_Cmd_UseRMB*)pCmd;
				return m_UseRMBManager.OnUseRMBMsg(pClient, pMsg);
			}
			break;*/
		}
	}
	else if (pCmd->CmdType == Main_Logon)
	{
		switch (pCmd->SubCmdType)
		{
		case LO_ChannelLogon:
			{
				LO_Cmd_ChannelLogon* pMsg = (LO_Cmd_ChannelLogon*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				m_ChannelLogon.OnHandleLogonInfo(pClient, pMsg);
				return true;
			}
			break;
		case LO_WeiXinLogon:
			{
				LO_Cmd_WeiXinLogon* pMsg = (LO_Cmd_WeiXinLogon*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				m_WeiXinLogon.HandleCode(pClient, pMsg);
				return true;
			}
			break;
		case LO_QQLogon:
			{
				LO_Cmd_QQLogon* pMsg = (LO_Cmd_QQLogon*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				m_QQLogon.HandleCode(pClient, pMsg);
				return true;
			}
			break;
		case LO_PhoneLogon:
			{
				//手机登陆
				LO_Cmd_PhoneLogon* pMsg = (LO_Cmd_PhoneLogon*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				m_PhoneLogon.OnAddPhoneLogon(pClient, pMsg->PhoneNumber, static_cast<BYTE>(pClient->OutsideExtraData), pMsg->ClientID);
				return true;
			}
			break;
		case LO_PhoneLogonCheck:
			{
				LO_Cmd_PhoneLogonCheck* pMsg = (LO_Cmd_PhoneLogonCheck*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				m_PhoneLogon.OnCheckPhoneLogonPassword(pMsg);
				return true;
			}
			break;
		}
	}
	else if (pCmd->CmdType == Main_Recharge)
	{
		switch (pCmd->SubCmdType)
		{
		case GO_IOSRecharge:
			{
				GO_Cmd_IOSRecharge* pMsg = (GO_Cmd_IOSRecharge*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				vector<char*> pVec;
				GetStringArrayVecByData(pVec, &pMsg->OrderInfo);
				if (pVec.size() != pMsg->OrderInfo.HandleSum)
				{
					ASSERT(false);
					return false;
				}
				string OrderInfo = pVec[0];
				GetIOSPayManager().OnSendOrderToIOSServer(pMsg->dwUserID,OrderInfo);
				return true;
			}
			break;
		}
		return false;
	}
	else if (pCmd->CmdType == Main_Role)
	{
		switch (pCmd->SubCmdType)
		{
		case LO_UpdateAccount:
			{
				LO_Cmd_UpdateAccount* pMsg = (LO_Cmd_UpdateAccount*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				//将命令进行分解处理
				std::vector<char*> pVec;
				GetStringArrayVecByData(pVec, &pMsg->Info);
				if (pVec.size() != pMsg->Info.HandleSum)
				{
					FreeVec(pVec);
					ASSERT(false);
					return false;
				}
				if (pMsg->bType == 3)
				{
					m_QQUserInfo.HandleGetQQUserInfo(pClient, pMsg->dwUserID, pVec[0], pVec[1]);
				}
				else if (pMsg->bType == 2)
				{
					m_WeiXinUserInfo.HandleGetWeiXinUserInfo(pClient, pMsg->dwUserID, pVec[0], pVec[1]);
					//m_WeiXinUserInfo.HandleGetWeiXinUserToken(pClient, pMsg->dwUserID, pVec[1]);
				}
				return true;
			}
			break;
		}
	}
	else
	{
		ASSERT(false);
		return false;
	}
	return true;
}
bool FishServer::HandleCenterMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	return false;
}
bool FishServer::HandleHttpClientMsg(HttpClientResult* pResult)
{
	if (!pResult)
	{
		ASSERT(false);
		return false;
	}
	if (pResult->HostIdx == m_ChannelLogonID)
	{
		ShowInfoToWin("接收到登陆返回数据:%s", pResult->RecvBuff);
		//渠道登陆的返回处理
		m_ChannelLogon.OnHandleLogonResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}
	else if (pResult->HostIdx == m_PhonePayID)
	{
		//渠道登陆的返回处理
		m_PhonePayManager.OnSendPhonePayResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}
	else if (pResult->HostIdx == m_PhoneSMSID)
	{
		//渠道登陆的返回处理
		m_PhoneSMSManager.OnSendPhoneSMSResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		//m_BindPhoneManager.OnSendPhoneSMSResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}
	else if (pResult->HostIdx == m_NormalRechargeID)
	{
		m_NormalRechargeManager.OnHandleAddOrderResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}
	else if (pResult->HostIdx == m_IOSPayID)
	{
		m_IOSPayManager.OnSendOrderToIOSResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}
	/*else if (pResult->HostIdx == m_WeiXinTokenID)
	{
		m_WeiXinLogon.OnHandleWeiXinTokenResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}
	else if (pResult->HostIdx == m_QQTokenID)
	{
		m_QQLogon.OnHandleQQTokenResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}*/
	else if (pResult->HostIdx == m_WeiXinUserInfoID)
	{
		UINT Count = 0;
		char* pStr = WCharToChar((wchar_t*)pResult->RecvBuff, Count);
		m_WeiXinUserInfo.HandleWeiXinUserInfoResult(*(UINT64*)pResult->ID, pStr, Count);
		delete((UINT64*)pResult->ID);
		free(pStr);
		return true;
	}
	else if (pResult->HostIdx == m_WeiXinFaceImgID)
	{
		m_WeiXinUserInfo.HandleWeiXinUserImgResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}
	/*else if (pResult->HostIdx == m_WeiXinUserTokenID)
	{
		m_WeiXinUserInfo.HandleWeiXinUserTokenResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}*/
	else if (pResult->HostIdx == m_QQUserInfoID)
	{
		UINT Count = 0;
		char* pStr = WCharToChar((wchar_t*)pResult->RecvBuff, Count);
		m_QQUserInfo.HandleQQUserInfoResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		free(pStr);
		return true;
	}
	else if (pResult->HostIdx == m_QQFaceImgID)
	{
		m_QQUserInfo.HandleQQUserImgResult(*(UINT64*)pResult->ID, pResult->RecvBuff, pResult->RecvSize);
		delete((UINT64*)pResult->ID);
		return true;
	}
	else
	{
		ASSERT(false);
		delete((UINT64*)pResult->ID);
		return false;
	}
	return false;
}
bool FishServer::HandleFtpMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	//处理FTP发送来的命令
	if (pCmd->CmdType == Main_Role)
	{
		switch (pCmd->SubCmdType)
		{
		case FG_SaveImageData:
			{
				FG_Cmd_SaveImageData* pMsg = (FG_Cmd_SaveImageData*)pCmd;
				if (!pMsg)
				{
					ASSERT(false);
					return false;
				}
				m_WeiXinUserInfo.HandleSaveRoleFaceImgResult(pMsg->ID, pMsg->Result==1?true:false, pMsg->Crc);
				m_QQUserInfo.HandleSaveRoleFaceImgResult(pMsg->ID, pMsg->Result == 1 ? true : false, pMsg->Crc);
				return true;
			}
		}
	}
	return true;
}
bool FishServer::HandleDBMsg(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->GetCmdType())
	{
	case DBO_LoadRechageOrderID:
		{
			DBO_Cmd_LoadRechageOrderID* pMsg = (DBO_Cmd_LoadRechageOrderID*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			m_OrderOnlyManager.OnLoadAllOrderID(pMsg);
			//m_UseRMBManager.OnLoadAllOrderID(pMsg);
			return true;
		}
		break;
	case DBO_CheckPhone:
		{
			DBO_Cmd_CheckPhone* pMsg = (DBO_Cmd_CheckPhone*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			if (pMsg->IsBindOrLogon)
			{
				m_BindPhoneManager.OnCheckPhoneResult(pMsg);
			}
			else
			{
				m_PhoneLogon.OnCheckPhoneResult(pMsg);
			}
			return true;
		}
		break;
	case DBO_QueryRechargeOrderInfo:
		{
			DBO_Cmd_QueryRechargeOrderInfo* pMsg = (DBO_Cmd_QueryRechargeOrderInfo*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			m_NormalRechargeManager.OnHandleNormalRechargeInfoResult(pMsg);
			return true;
		}
		break;
	/*case DBO_CheckEntityID:
		{
			DBO_Cmd_CheckEntityID* pMsg = (DBO_Cmd_CheckEntityID*)pCmd;
			if (!pMsg)
			{
				ASSERT(false);
				return false;
			}
			m_RealNameVerificationManager.OnCheckEntityResult(pMsg);
			return true;
		}*/
	}
	return false;
}
bool FishServer::HandleHttpMsg(HttpRecvData* pHttpData)
{
	if (!pHttpData)
	{
		ASSERT(false);
		return false;
	}
	//处理数据
	switch (pHttpData->RequestCode)
	{
	case HT_Recharge:
		{
			if (pHttpData->KeyValues.count(m_UseRMBManager.GetRechargeCrcInfo().sign2Crc) == 1 &&
				pHttpData->KeyValues.count(m_UseRMBManager.GetRechargeCrcInfo().versionCrc) == 1 &&
				pHttpData->KeyValues.count(m_UseRMBManager.GetRechargeCrcInfo().freePriceCrc) == 1 &&
				pHttpData->KeyValues.count(m_UseRMBManager.GetRechargeCrcInfo().sdkCodeCrc) == 1)
			{
				bool Result = m_UseRMBManager.OnHandleHttpInfoByUserRecharge(pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().orderidCrc], pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().priceCrc],
					pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().ChannelCodeCrc], pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().channelOrderidCrc], pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().channelLabelCrc]
					, pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().callbackInfoCrc], pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().signCrc]
					, pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().sign2Crc]
					, pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().versionCrc]
					, pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().freePriceCrc]
					, pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().sdkCodeCrc]);
				return Result;
			}
			else
			{
				bool Result = m_UseRMBManager.OnHandleHttpInfoByUserRecharge(pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().orderidCrc], pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().priceCrc],
					pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().ChannelCodeCrc], pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().channelOrderidCrc], pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().channelLabelCrc]
					, pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().callbackInfoCrc], pHttpData->KeyValues[m_UseRMBManager.GetRechargeCrcInfo().signCrc]);
				return Result;
			}
		}
		break;
	case HT_PhonePay:
		{
			m_PhonePayManager.OnHandleHttpResult(
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().account_noCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().face_priceCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().order_noCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().order_timeCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().order_typeCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().partner_idCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().pay_moneyCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().profitCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().sign_typeCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().statusCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().stream_idCrc],
				pHttpData->KeyValues[m_PhonePayManager.GetPhonePayCrcInfo().signCrc]);
			return true;
		}
		break;
	case HT_NormalPay:
		{
			m_NormalRechargeManager.OnHandleNormalRechargeInfo(pHttpData->PostData);
			return true;
		}
		break;
	}
	return false;
}
void FishServer::OnReloadConfig()
{
	FishConfig pNewConfig;
	if (!pNewConfig.LoadConfigFilePath())
	{
		ShowInfoToWin("重新加载FishConfig.xml失败");
		return;
	}
	m_FishConfig = pNewConfig;
	ShowInfoToWin("重新加载FishConfig.xml成功");
}

void FishServer::SendNetCmdToControl(NetCmd*pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_ControlTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
bool FishServer::ConnectControl()
{
	//对控制方开启的端口 允许外部链接到中央服务器来进行登陆处理 携带DWORD 的唯一ID
	ControlServerConfig* pConfig = g_FishServerConfig.GetControlServerConfig();
	if (!pConfig)
	{
		ASSERT(false);
		return false;
	}
	ClientInitData pControlData;
	pControlData.BuffSize = pConfig->ControlServerBuffSize;
	pControlData.Port = pConfig->ControlServerListenPort;
	pControlData.SleepTime = pConfig->ControlServerSleepTime;
	pControlData.SocketRecvSize = pConfig->ControlServerRecvBuffSize;
	pControlData.SocketSendSize = pConfig->ControlServerSendBuffSize;
	pControlData.ThreadNum = 1;
	pControlData.Timeout = pConfig->ControlServerTimeOut;
	pControlData.CmdHearbeat = 0;
	pControlData.SendArraySize = pConfig->ControlServerMaxSendCmdCount;
	pControlData.RecvArraySize = pConfig->ControlServerMaxRecvCmdCount;

	if (!m_ControlTcp.Init(pConfig->ControlServerListenIP, pControlData, (void*)&m_OperateNetworkID, sizeof(BYTE)))
	{
		ShowInfoToWin("ControlServer连接失败");
		return false;
	}
	OnTcpClientConnect(&m_ControlTcp);
	return true;
}
bool FishServer::HandleControlMsg(NetCmd* pCmd)
{
	//处理运营服务器发送来的数据
	if (!pCmd)
		return false;
	if (pCmd->CmdType != Main_Control)
	{
		ASSERT(false);
		return false;
	}
	switch (pCmd->SubCmdType)
	{
	case CL_ReloadConfig:
		{
			OnReloadConfig();
			return true;
		}
		break;
	}

	return true;
}
void FishServer::SendNetCmdToLogDB(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_LogDBTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::SendNetCmdToFtp(NetCmd* pCmd)
{
	if (!pCmd)
	{
		ASSERT(false);
		return;
	}
	if (!m_FTPTcp.Send(pCmd, false))
	{
		ASSERT(false);
	}
}
void FishServer::UpdateInfoToControl(DWORD dwTimer)
{
	static DWORD LogUpdateCenterTime = 0;
	if (LogUpdateCenterTime == 0 || dwTimer - LogUpdateCenterTime >= 10000)
	{
		LogUpdateCenterTime = dwTimer;
		LC_Cmd_OperateInfo msg;
		SetMsgInfo(msg, GetMsgType(Main_Control, LC_OperateInfo), sizeof(LC_Cmd_OperateInfo));
		msg.OperateID = m_OperateNetworkID;
		SendNetCmdToControl(&msg);
	}
}