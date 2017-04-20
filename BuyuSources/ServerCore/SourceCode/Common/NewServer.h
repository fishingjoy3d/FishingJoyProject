#pragma once
#define MAX_THREAD_NUM		16
#define ACCEPT_BUFF_SIZE	128
enum POST_TYPE
{
	POST_ACCEPT,
	POST_SEND_DATA,
};
enum WAIT_TYPE
{
	WAIT_0,
	WAIT_1,
};
struct AcceptClientData
{
	SOCKET	Socket;
	UINT	IP;
	USHORT	Port;
	char	Buff[ACCEPT_BUFF_SIZE];
	UINT	RecvSize;
	UINT	Tick;
	UINT	RandID;
	WAIT_TYPE WaitType;
};
struct ClientIO
{
	OVERLAPPED	Overlapped;
	WSABUF		WsaBuff;
	SOCKET		Socket;
	char		Buff[ACCEPT_BUFF_SIZE];
	BYTE		Opt;
};
struct AcceptData
{
	AcceptData(){
	}
	AcceptData(int n){
	}
	char		Buff[ACCEPT_BUFF_SIZE];
	int			RecvSize;
	SOCKET		Socket;
};
struct ClientData : ServerClientData
{
	ClientData(UINT nSendCount, UINT nRecvCount) :SendList(nSendCount), ServerClientData(nRecvCount)
	{

	}
	CACHELINE(0);
	volatile long		RefCount;
	CACHELINE(4);
	SafeCmdList			SendList;
	CACHELINE(6);

	UINT	SendTick;
	UINT	SendCmdTick;
	BYTE	SendError;
	CACHELINE(X);
	UINT	RecvTick;
	UINT	Offset;
	UINT	RecvSize;
	char	Buff[1];
};
struct RecvThreadData
{
	RecvThreadData(UINT nSize):NewClientList(nSize){
	}
	volatile	long		OnlineNum;
	SafeArray<ClientData*>	NewClientList;

};
struct SendThreadData
{
	SendThreadData(UINT nSize) :NewClientList(nSize){
	}
	volatile	long		OnlineNum;
	SafeArray<ClientData*>	NewClientList;
};

class NewServer : public NetServer
{
public:
	NewServer();
	~NewServer();

	bool		Init(const ServerInitData &data, bool bTCP = true /* False UDP */);
	bool		Kick(ServerClientData *pClient, RemoveType rt = REMOVE_NORMAL);
	void		Shutdown();
	void		SetCmdHandler(INetHandler *pHandler);
	bool		Send(ServerClientData *pClient, NetCmd *pCmd);
	UINT		JoinNum()const;
	void		SwitchAccept(bool bEnable);

	void		_ThreadRecvTCP();
	void		_ThreadSendTCP();
	void		_ThreadAccept();
protected:
	void		RemoveClient(ClientData *pc, RemoveType rt);
	bool		RecvDataByTCP(ClientData *pc, int nSize);
	void		AddNewClient(const AcceptClientData &acd);
	bool		CheckNewClient(AcceptClientData &data);
	void		GetSendAndRecvIndex(ushort &sendIdx, ushort &recvIdx);
	void		GetRecvIndex(ushort &recvIdx);

	CACHELINE(0);
	volatile UINT				m_OnlineNum;
	CACHELINE(1);
	volatile bool				m_bRun;
	CACHELINE(2);
	volatile long				m_RecvIndex;
	CACHELINE(3);
	volatile long				m_SendIndex;
	CACHELINE(4);
	volatile long				m_ExitIndex;
	CACHELINE(5);
	volatile long				m_SendDataIndex;
	CACHELINE(6);
	volatile bool				m_bAccept;
	CACHELINE(7);
	
	SafeArray<AcceptData>		m_NewClientList;
	INetHandler					*m_pHandler;
	SOCKET						m_Socket;
	RecvThreadData				*m_RecvThreadData[MAX_THREAD_NUM];
	SendThreadData				*m_SendThreadData[MAX_THREAD_NUM];
	ServerInitData				m_InitData;

};