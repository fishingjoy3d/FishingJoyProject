#pragma once
class UDPClient : public NetClient
{
public:
	enum State
	{
		STATE_DISCONNECT,
		STATE_CONNECTED,
	};
	UDPClient();
	~UDPClient();
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
	void RecvThread();
protected:
	void ReleaseData();
	void CloseSocket();
	bool RecvData(char *pbuff, int size);
	virtual bool SendImmediate(NetCmd *pCmd, bool &b10035);
	CACHELINE(0);
	volatile bool		m_bConnected;

	UINT				m_RecvID;
	UINT				m_SendID;
	bool				m_bSendBackID;
	UINT				*m_pSendBuff;
	SOCKET				m_Socket;
	SOCKADDR_IN			m_ServerAddr;
	SafeCmdList			*m_RecvList;
	SafeCmdList			*m_SendList;
	ClientInitData		m_InitData;

};