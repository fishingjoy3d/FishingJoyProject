//#pragma once
//#include "NetClient.h"
//#define MAX_CLIENT_THREAD 8
//struct ClientThreadData
//{
//	volatile UINT Num;
//	volatile float Delta;
//};
//struct ClientData
//{
//	volatile bool InsideRemoved;
//	volatile long RefCount;
//	SafeList<NetCmd*> RecvList;
//	SOCKET	Socket;
//	UINT	RecvTick;
//	UINT	SendTick;
//	UINT	Offset;
//	UINT	RecvSize;
//	BYTE	Buff[1];
//};
//
//class TestClient
//{
//public:
//	void Init(const char *pIP, int maxNum, const ClientInitData &data);
//	void ConnectThread();
//	void SendThread();
//	void RecvThread();
//	void Shutdown();
//	UINT GetThreadNum()const
//	{
//		return m_InitData.ThreadNum;
//	}
//	const ClientThreadData& GetSendThreadData(int idx)
//	{
//		return m_SendData[idx];
//	}
//	const ClientThreadData& GetRecvThreadData(int idx)
//	{
//		return m_RecvData[idx];
//	}
//protected:
//	bool RecvData(ClientData *pscd, int nRecvSize);
//	volatile bool		m_bRun;
//	volatile long		m_ExitCount;
//	volatile long		m_SendIndex;
//	volatile long		m_RecvIndex;
//	int					m_MaxNum;
//	char				m_IP[20];
//	SafeList<ClientData*> m_NewRecvList[MAX_CLIENT_THREAD];
//	SafeList<ClientData*> m_NewSendList[MAX_CLIENT_THREAD];
//	ClientThreadData	m_SendData[MAX_CLIENT_THREAD];
//	ClientThreadData	m_RecvData[MAX_CLIENT_THREAD];
//	ClientInitData		m_InitData;
//	vector<ClientData*> m_ClientList;
//};