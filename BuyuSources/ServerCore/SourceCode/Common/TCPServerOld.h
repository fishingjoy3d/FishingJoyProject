//#pragma once
//#include "NetServer.h"
//
////快速响应收发数据有几个参数会影响比较大
////1.SleepTime,收发线程的睡眠间隔时间。
////2.RecvThreadNum 接收线程数量  
////3.SendThreadNum 发送线程数量 
////4.当连接数量很大时，线程越多，每个线程处理的连接数就越少，每帧耗费的时间就越少。当耗费的时间大于睡眠时间时，线程几乎就一直在循环处理，导致CPU占用率上升。
//
//struct TCPClientData : public ServerClientData
//{
//	SafeArray<NetCmd*, 512>	SendList; //发送的队列
//	volatile long RefCount;
//	UINT	SendTick;
//	UINT	RecvTick;
//	UINT	Offset;
//	UINT	RecvSize;
//	BYTE    Buff[1];
//};
//struct ThreadRuntimeData
//{
//	volatile long	Num;
//	volatile float	Delta1;
//	volatile float  Delta2;
//};
//
//class TCPServerOld :public NetServer
//{
//public:
//	typedef vector<TCPClientData*> ClientVector;
//	typedef SafeArray<TCPClientData*>  ClientList;
//	TCPServerOld();
//	~TCPServerOld();
//
//	virtual bool Init(const ServerInitData &data);
//	virtual void Shutdown();
//	virtual bool Send(ServerClientData *pClient, NetCmd *pCmd);
//	virtual void SetCmdHandler(INetHandler *pHandler)
//	{
//		m_CmdHandler = pHandler;
//	}
//	virtual bool Kick(ServerClientData *pClient);
//	virtual UINT JoinNum()const
//	{
//		return (UINT)m_JoinNum;
//	}
//	virtual void SwitchAccept(bool bEnable)
//	{
//		m_bEnableAccept = bEnable;
//	}
//	void RecvThread();
//	void SendThread();
//	void AcceptThread();
//	void RecvThread2();
//	const ServerInitData& GetInitData()
//	{
//		return m_InitData;
//	}
//	const ThreadRuntimeData& GetRecvThreadData(ushort idx)
//	{
//		return m_RecvThreadData[idx];
//	}
//	const ThreadRuntimeData& GetSendThreadData(ushort idx)
//	{
//		return m_SendThreadData[idx];
//	}
//protected:
//	void AddRecvThreadNum(int idx)
//	{
//		::InterlockedIncrement(&m_RecvThreadData[idx].Num);
//	}
//	void RemoveRecvThreadNum(int idx)
//	{
//		::InterlockedDecrement(&m_RecvThreadData[idx].Num);
//	}
//	void AddSendThreadNum(int idx)
//	{
//		::InterlockedIncrement(&m_SendThreadData[idx].Num);
//	}
//	void RemoveSendThreadNum(int idx)
//	{
//		::InterlockedDecrement(&m_SendThreadData[idx].Num);
//	}
//	void	AddJoinNum()
//	{
//		::InterlockedIncrement(&m_JoinNum);
//	}
//	void	RemoveJoinNum()
//	{
//		::InterlockedDecrement(&m_JoinNum);
//	}
//	bool	RecvData(TCPClientData *pscd, int nRecvSize);
//	void	RemoveClient(TCPClientData *pscd);
//	void	RemoveClientImmediate(TCPClientData *pscd);
//	ushort	GetRecvThreadMin();
//	ushort	GetSendThreadMin();
//
//	volatile bool			m_bEnableAccept;
//	volatile long			m_RecvIndex;
//	volatile long			m_SendIndex;
//	volatile long			m_ExitCount;
//	volatile bool			m_bRun;
//	volatile long			m_JoinNum;
//
//	ThreadRuntimeData		m_RecvThreadData[MAX_SOCKET_THREAD];
//	ThreadRuntimeData		m_SendThreadData[MAX_SOCKET_THREAD];
//	ClientList				m_NewRecvList[MAX_SOCKET_THREAD];
//	ClientList				m_NewSendList[MAX_SOCKET_THREAD];
//
//	INetHandler				*m_CmdHandler;
//	SOCKET					 m_Socket;
//	ServerInitData			 m_InitData;
//	NetCmd					 m_HeartCmd;
//};