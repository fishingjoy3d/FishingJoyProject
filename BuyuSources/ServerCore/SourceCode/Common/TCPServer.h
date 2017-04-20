//#pragma once
//class TCPServer :public NetServer
//{
//public:
//	TCPServer();
//	~TCPServer();
//
//	virtual bool Init(const ServerInitData &data);
//	virtual void Shutdown();
//	virtual bool Send(ServerClientData *pClient, NetCmd *pCmd);
//	virtual void SetCmdHandler(INetHandler *pHandler)
//	{
//		m_CmdHandler = pHandler;
//	}
//	virtual bool Kick(ServerClientData *pClient, RemoveType rt = REMOVE_NORMAL)
//	{
//		RemoveClient((MyClientData*)pClient, rt, true);
//		return true;
//	}
//	virtual UINT JoinNum()const
//	{
//		return (UINT)m_JoinNum;
//	}
//	virtual void SwitchAccept(bool bEnable)
//	{
//
//	}
//	void _RecvThread();
//	void _DispatchThread();
//	void _RecoverThread();
//	const ServerInitData& GetInitData()
//	{
//		return m_InitData;
//	}
//protected:
//	void	_HandleAcceptBack(AcceptBuffData *pdata, UINT recvSize);
//	void	_HandleRecvBack(MyClientData *pc, ClientIOData *pd, UINT recvSize);
//	void	_HandleSendBack(MyClientData *pc, ClientIOData *pd, UINT sendSize);
//	void	_HandleSendFirstData(MyClientData *pc, ClientIOData* pd);
//	void	_PostAccept(AcceptBuffData *pData);
//	void	_PostRecv(MyClientData *pc);
//	void	_PostSendPing(MyClientData *pc);
//	void	_PostSend(MyClientData *pc, bool bResend);
//	void	_PostSendBack(MyClientData *pc, UINT id);
//	void	_PostSendFirstData(MyClientData *pc);
//	void    _PostSendHearbeat(MyClientData *pc);
//	void	_PostSendVersionError(AcceptBuffData *pc);
//	int 	_GetDispIdx();
//	void	RemoveClient(MyClientData *pc, RemoveType rt, bool subRefCount);
//	void	RemoveClientImmediate(MyClientData *pc);
//	MyClientData* _NewClient(NewClientData &ncd);
//	CACHELINE(1);
//	volatile long				m_JoinNum;
//	CACHELINE(2);
//	volatile bool				m_bRun;
//	CACHELINE(3);
//	volatile long				m_ExitCount;
//	CACHELINE(4);
//	volatile long				m_DispIndex;
//	CACHELINE(5);
//	volatile long				m_RecvIndex;
//	CACHELINE(6);
//	SafeNewClientList			m_NewClientList;
//	CACHELINE(7);
//
//	LPFN_GETACCEPTEXSOCKADDRS	m_pGetAcceptEx;
//	LPFN_ACCEPTEX				m_pAcceptEx;
//	int							m_DispNum;
//	MyDispatchData				*m_pDispatchData;
//	AcceptBuffData				*m_pAcceptData;
//	HANDLE						m_CompletionPort;
//	INetHandler					*m_CmdHandler;
//	SOCKET						m_Socket;
//	ServerInitData				m_InitData;
//};
