#pragma once

class TCPClient: public NetClient
{
public:
	enum State
	{
		STATE_DISCONNECT,
		STATE_CONNECTED,
	};
	TCPClient();
	virtual ~TCPClient();
	virtual bool Init(const char *ip, const ClientInitData &data, void *pData = NULL, int sendSize = 0);
	virtual bool Send(NetCmd *pCmd, bool bNoneUse);
	virtual void Shutdown();
	virtual bool IsConnected()const
	{
		return m_bConnected;
	}

	virtual NetCmd *GetCmd()
	{
		return m_RecvList->GetItem();
	}
	void SendThread();
	void RecvThread();
protected:
	void ReleaseData();
	virtual bool SendImmediate(void *pData, UINT size, bool &b10035);
	CACHELINE(0);
	volatile bool		m_bConnected;
	CACHELINE(1);
	volatile long		m_ExitCount;
	CACHELINE(2);
	volatile UINT		m_SendTick;
	CACHELINE(3);

	UINT				m_RecvTick;
	UINT				m_Offset;
	UINT				m_RecvSize;
	BYTE				*m_Buff;
	SOCKET				m_Socket;
	SafeCmdList			*m_RecvList;
	SafeCmdList			*m_SendList;
	ClientInitData		m_InitData;

};