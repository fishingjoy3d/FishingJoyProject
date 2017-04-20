#pragma once
struct FTPRequestData
{
	FTPRequestData()
	{
	}
	FTPRequestData(int n)
	{
	}

	bool	Version;
	char	File[256];
	UINT	FileSize;
};
struct FTPClientInfo
{
	FTPClientInfo()
	{
		Socket = NULL;
		Tick = timeGetTime();
		RecvSize = 0;
		BuffLength = 0;
		Buff = 0;
		QueueIdx = 0;
		SendTick = 0;
		Port = 0;
		Cmd = NULL;
		FileCount = 0;
		RecvFileCount = 0;
	}
	SOCKET	Socket;
	USHORT	Port;
	UINT	OnlineTick;
	UINT	Tick;
	UINT	SendTick;
	UINT	RecvSize;
	UINT	QueueIdx;
	UINT	BuffLength;
	UINT	FileCount;
	UINT	RecvFileCount;
	NetCmd	*Cmd;
	char	*Buff;
};
struct FTPClientThreadData
{
	FTPClientThreadData() :NewClients(1024)
	{
	}
	SafeArray<FTPRequestData> NewClients;
};
class FTPClient
{
public:
	FTPClient()
	{
	}
	bool	Init(const char *IP, USHORT port, UINT threadNum);
	bool	GetFile(const char *pcFile);
	bool	GetVersion( const char *pcVer);

	void	_Thread();
protected:
	CACHELINE(0);
	volatile UINT		m_Run;
	CACHELINE(1);
	volatile UINT		m_RecvCompletion;
	CACHELINE(2);
	volatile UINT		m_ThreadIdx;
	CACHELINE(3);
	UINT				m_ThreadNum;
	UINT				m_RequestNum;
	SOCKADDR_IN			m_Addr;
	FTPClientThreadData	m_ThreadData[MAX_THREAD_NUM];
};