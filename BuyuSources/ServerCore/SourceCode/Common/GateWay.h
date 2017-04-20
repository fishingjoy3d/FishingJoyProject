#pragma once
#define MAX_THREAD_NUM		16
#define ACCEPT_BUFF_SIZE	128
enum GWUDPState
{
	GWS_NONE,
	GWS_CONNECTING,
	GWS_SEND_VALID_DATA,
	GWS_SEND_HEARBEAT,
	GWS_OK,
	GWS_REMOVED,
};
struct GWInitData : public ServerInitData
{
	char	LogonServerIP[30];		//登录服务器的IP
	char	OperationServerIP[30];	//运营服务器的IP
	char	LocalServerIP[30];		//本服务器的外网IP
	USHORT	LogonClientPort;
	USHORT	LogonServerPort;
	UINT	LogonThreadNum;
	UINT	LogonAcceptPerFrame;
	UINT	LogonTimeout;

	UINT	OperationAcceptPerFrame;
	USHORT	OperationClientPort;
	USHORT	OperationServerPort;
	UINT	OperationTimeout;
	
};
struct GWClientData : ServerClientData
{
	GWClientData(UINT nRecvCount, UINT ClientArrayCount, UINT ServerArrayCount) :
	ServerClientData(nRecvCount), 
	RecvClientList(ClientArrayCount), 
	RecvServerList(ServerArrayCount)
	{
		State = GWS_NONE;
		InnerIsInScene = false;
	}

	MyArray<UINT*>	RecvClientList;
	MyArray<UINT*>	RecvServerList;
	SOCKET			ServerSocket;
	UINT			ConnectServerIP;
	USHORT			ConnectServerPort;
	UINT			ValidBuff[5];
	BYTE			State;

	//客户端
	UINT			ResendCount;
	UINT			SendID;
	UINT			SendTick;
	UINT			SendCmdTick;
	UINT			RecvID;
	UINT			RecvTick;
	UINT			SendSize;
	UINT			*pSendBuff;
	BYTE			SendError;
	bool			bSendBackID;
	//服务器端
	UINT			ServerResendCount;
	UINT			ServerSendID;
	UINT			ServerSendTick;
	UINT			ServerSendCmdTick;
	UINT			ServerRecvID;
	UINT			ServerRecvTick;
	UINT			ServerSendSize;
	BYTE			ServerSendError;
	bool			bServerSendBackID;
	UINT			*pServerSendBuff;
	bool			InnerIsInScene;

	char			Buff[1];
};
struct GWRecvThreadData
{
	GWRecvThreadData(UINT nSize) :NewClientList(nSize){
		OnlineNum = 0;
	}
	volatile	long		OnlineNum;
	SafeArray<GWClientData*>	NewClientList;

};

struct GWNewLogonClient
{
	GWNewLogonClient(UINT buffSize)
	{
		ClientSocket = NULL;
		ServerSocket = NULL;
		RecvTick = 0;
		SendTick = 0;
		pServerBuff = (char*)malloc(buffSize * 2);
		pClientBuff = pServerBuff + buffSize;
		ServerRecvSize = 0;
		ClientRecvSize = 0;
		ServerRecvOffset = 0;
	}
	~GWNewLogonClient()
	{
		SAFE_FREE(pServerBuff);
	}
	SOCKET	ClientSocket;
	SOCKET  ServerSocket;
	UINT	RecvTick;
	UINT	SendTick;
	BYTE    State;
	char    *pServerBuff;
	char    *pClientBuff;
	UINT	ServerRecvSize;
	UINT	ClientRecvSize;
	UINT	ServerRecvOffset;
	list<NetCmd*> ServerCmdList;
};
struct GWLogonThreadData
{
	GWLogonThreadData(UINT nSize) :NewClientList(nSize)
	{
		OnlineNum = 0;
	}
	volatile	long		OnlineNum;
	SafeArray<GWNewLogonClient*>	NewClientList;

};


struct GWOperationClient
{
	GWOperationClient(UINT buffSize)
	{
		ClientSocket = NULL;
		ServerSocket = NULL;
		RecvTick = 0;
		SendTick = 0;
		pServerBuff = (char*)malloc(buffSize * 2);
		pClientBuff = pServerBuff + buffSize;
		ServerRecvSize = 0;
		ClientRecvSize = 0;
	}
	~GWOperationClient()
	{
		SAFE_FREE(pServerBuff);
	}
	SOCKET	ClientSocket;
	SOCKET  ServerSocket;
	UINT	RecvTick;
	UINT	SendTick;
	BYTE    State;
	char    *pServerBuff;
	char    *pClientBuff;
	UINT	ServerRecvSize;
	UINT	ClientRecvSize;
};
struct GWOperationThreadData
{
	GWOperationThreadData(UINT nSize) :NewClientList(nSize)
	{
		OnlineNum = 0;
	}
	volatile	long		OnlineNum;
	SafeArray<GWOperationClient*>	NewClientList;

};

class NewGateServer : public NetServer
{
public:
	NewGateServer();
	~NewGateServer();

	bool		Init(const GWInitData &data, bool btcp = true);
	bool		Init(const ServerInitData &data, bool btcp = true)
	{
		return false;
	}
	bool		Kick(ServerClientData *pClient, RemoveType rt = REMOVE_NORMAL);
	void		Shutdown();
	void		SetCmdHandler(INetHandler *pHandler);
	bool		Send(ServerClientData *pClient, NetCmd *pCmd);
	UINT		JoinNum()const;
	void		SwitchAccept(bool bEnable);

	void		_ThreadRecv();
	void		_ThreadAccept();
	void		_ThreadLogonAccept();
	void		_ThreadLogonRecv();
	void		_ThreadOperationAccept();
	void		_ThreadOperationRecv();
protected:
	bool		TCPRecvServerCmd(GWNewLogonClient *pc);
	void		RemoveClient(GWClientData *pc, RemoveType rt);
	bool		RecvDataByUDP(GWClientData *pc, char *Buff, int nSize, UINT tick);
	bool		RecvDataByUDPServer(GWClientData *pc, char *Buff, int nSize, UINT tick);
	bool		RecvDataByTCP(GWClientData *pc, int nSize);
	bool		AddNewClient(void *pData);
	bool		CheckNewClient(AcceptClientData &data);
	void		GetSendAndRecvIndex(ushort &sendIdx, ushort &recvIdx);
	void		GetRecvIndex(ushort &recvIdx);
	bool		InitClientUDP(GWClientData *pc, bool bTCP);
	void		CheckConnectList(GWRecvThreadData *pRecvData, vector<GWClientData*> &tempClientList, vector<GWClientData*> &clientList, char *pBuff);
	CACHELINE(0);
	volatile UINT				m_OnlineNum;
	CACHELINE(1);
	volatile bool				m_bRun;
	CACHELINE(2);
	volatile long				m_RecvIndex;
	CACHELINE(3);
	volatile long				m_ExitIndex;
	CACHELINE(5);
	volatile bool				m_bAccept;
	CACHELINE(7);
	volatile UINT				m_LogonIndex;
	CACHELINE(8);


	UINT						m_LocalIP;
	SOCKET						m_Socket;
	SOCKET						m_LogonSocket;
	SOCKET						m_OperationSocket;
	INetHandler					*m_pHandler;
	GWRecvThreadData			*m_RecvThreadData[MAX_THREAD_NUM];
	GWLogonThreadData			*m_LogonThreadData[MAX_THREAD_NUM];
	GWOperationThreadData		m_OperationThreadData;
	GWInitData					m_InitData;

};