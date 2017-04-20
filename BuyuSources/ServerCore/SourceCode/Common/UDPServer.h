#pragma once
#define MAX_THREAD_NUM		16
#define ACCEPT_BUFF_SIZE	128
struct UDPClientData : ServerClientData
{
	UDPClientData(UINT nSendCount, UINT nRecvCount) :SendList(nSendCount), ServerClientData(nRecvCount)
	{
		GWIsInScene = false;
	}
	CACHELINE(0);
	volatile long		RefCount;
	CACHELINE(4);
	SafeCmdList			SendList;
	CACHELINE(6);

	UINT	ResendCount;
	UINT	SendID;
	UINT	SendTick;
	UINT	SendCmdTick;
	CACHELINE(X);
	UINT	RecvID;
	UINT	RecvTick;
	UINT	SendSize;
	UINT    *pSendBuff;
	BYTE	SendError;
	bool	IsGateway;
	bool	GWIsInScene;
	bool	bSendBackID;
	char	Buff[1];
};
struct UDPRecvThreadData
{
	UDPRecvThreadData(UINT nSize) :NewClientList(nSize){
		OnlineNum = 0;
	}
	volatile	long		OnlineNum;
	SafeArray<UDPClientData*>	NewClientList;

};

class NewUDPServer : public NetServer
{
public:
	NewUDPServer();
	~NewUDPServer();

	bool		Init(const ServerInitData &data, bool btcp = true);
	bool		Kick(ServerClientData *pClient, RemoveType rt = REMOVE_NORMAL);
	void		Shutdown();
	void		SetCmdHandler(INetHandler *pHandler);
	bool		Send(ServerClientData *pClient, NetCmd *pCmd);
	UINT		JoinNum()const;
	void		SwitchAccept(bool bEnable);

	void		_ThreadRecv();
	void		_ThreadAccept();
protected:
	void		RemoveClient(UDPClientData *pc, RemoveType rt);
	bool		RecvDataByUDP(UDPClientData *pc, char *Buff, int nSize, UINT tick);
	bool		RecvDataByTCP(UDPClientData *pc, int nSize);
	bool		AddNewClient(void *pData);
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
	volatile long				m_ExitIndex;
	CACHELINE(5);
	volatile bool				m_bAccept;
	CACHELINE(7);

	INetHandler					*m_pHandler;
	SOCKET						m_Socket;
	UDPRecvThreadData			*m_RecvThreadData[MAX_THREAD_NUM];
	ServerInitData				m_InitData;

};