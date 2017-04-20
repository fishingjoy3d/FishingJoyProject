#pragma once
#define OPEN_FILE_STATE			0xEFFFFFFF
#define NEW_CLIENT_COUNT		1024
#define MAX_FILE_COUNT			10	//最大不能超过127个
#include "Cmd_Fish.h"
#pragma pack(push)
#pragma pack(1)
struct NetCmdFileRequest :public NetCmd
{
	USHORT	Length;
	USHORT	FileCount;
	char	FileName[1];
};
struct NetCmdCheckVersion :public NetCmd
{
	UINT	Version;
	BYTE	Plateform;
	USHORT	ScreenWidth;
	USHORT	ScreenHeight;
	char	Mac[48];
	char	Package[48];
	USHORT	Length;
	char	FileName[1];
};
#pragma pack(pop)
enum FTPCmdType
{
	CMD_CHECK_VERSION = (Main_Extra_Down << 8) | (EXTRA_DOWN_CHECK_VER),
	CMD_FILE_REQUEST = (Main_Extra_Down << 8) | (EXTRA_DOWN_FILE),
	CMD_FILE_REQUEST_EX = (Main_Extra_Down << 8) | (EXTRA_DOWN_FILE_EX),
};
enum FTPOpenFileState
{
	FTP_OPEN_WAIT,
	FTP_OPEN_FAILED,
	FTP_OPEN_OK,
};
enum FTPRecvState
{
	FTP_RECV_OK,
	FTP_RECV_ERROR,
	FTP_RECV_FILE_ERROR,
	FTP_RECV_CONTINUE,
};
enum FTPRemoveType
{
	FTP_REMOVE_QUEUE_TIMEOUT,
	FTP_REMOVE_THREAD_NOTSPACE,
	FTP_REMOVE_RECVCMD_TIMEOUT,
	FTP_REMOVE_RECVCMD_ERROR,
	FTP_REMOVE_OPEN_FAILED,
	FTP_REMOVE_OPEN_TIMEOUT,
	FTP_REMOVE_TRANS_TIMEOUT,
	FTP_REMOVE_TRANS_COMPLETION,
	FTP_REMOVE_CREATE_UDP_FAILED,
	FTP_REMOVE_RECV_LOST_ERROR,
	FTP_REMOVE_UDP_CONNECT_TIMEOUT,
};
enum FTPFileChangeType
{
	FTP_FILE_NONE,
	FTP_FILE_DEL,
	FTP_FILE_RENAME,
	FTP_FILE_MODIFY
};
struct FTPFileData
{
	volatile UINT	UseCount;
	UINT			LastTick;
	UINT			FileNameCrc;
	UINT			FileLength;
	char			FileData[1];
};
struct FTPFileModify
{
	FTPFileModify()
	{
	}
	FTPFileModify(int n)
	{
	}
	BYTE			Opt;
	UINT			FileNameCrc;
	UINT			NewFileNameCrc;
};
struct FTPServerInitData
{
	FTPServerInitData()
	{
		BandwidthUsePercent = 0.7f;//
		SocketRecvBuffSize = 2048;
		SocketSendBuffSize = 8192;
		Timeout = 3000;
		SleepTime = 10;
		AcceptPerFrame = 64;
		FileCacheTime = 1000 * 60 * 10;//10分钟
		RecvThreadNum = 4;
		bFindISP = true;
		PerMaxSendSize = 65536 * 8;
	}
	UINT	SendSafeCount;			//安全的重发次数
	UINT	SendQueueInterval;		//ms, 发送排队的间隔
	UINT	SendErrorInterval;		//发送失败的间隔时间
	UINT	SendChunkInterval;		//ms 发送的数据最小间隔
	UINT	PerMaxSendSize;			//每次发送的最大字节
	UINT	RecvThreadNum;
	UINT	FileCacheTime;
	UINT	AcceptPerFrame;
	UINT	Timeout;
	UINT	SleepTime;
	UINT	SocketRecvBuffSize;
	UINT	SocketSendBuffSize;
	UINT	ClinetMinSpeed;			//Byte，每个客户端每秒最低传输速度
	UINT	Bandwidth;				//Byte, 服务器的总带宽，网络带宽是BIT要除以8，才是真正的字节带宽。
	float	BandwidthUsePercent;	//带宽的使用百分比，比如0.7f用于传输，0.3用于连接和排队的命令.
	UINT	BuffSize;
	USHORT	Port;
	UINT	PathLength;
	bool	bFindISP;				//是否查询宽带运营商。
	char	FTPPath[256];
};
struct FileNameData
{
	char	*pcName;
	USHORT	NameLength;
	UINT	Offset;
	UINT	Length;
};
enum ISPType
{
	ISP_DX = 0,		//电信
	ISP_LT,			//联通
	ISP_YD,			//移动
};
struct FTPClientData
{
	FTPClientData()
	{
	}
	UINT			MacCrc;
	bool			XorData;
	bool			CheckVer;
	BYTE			ISPType;
	SOCKET			Socket;
	UINT			IP;
	UINT			LocalIP;
	USHORT			Port;
	UINT			Tick;
	UINT			SendTick;
	UINT			OnlineTick;
	UINT			SendSize;
	UINT			TotalSendSize;
	UINT			RecvSize;
	UINT			SendErrorCount;
	BYTE			RemoveType;
	FTPFileData*	pFileHandler[MAX_FILE_COUNT];
	FileNameData	RequestFileList[MAX_FILE_COUNT];
	BYTE			RequestFileCount;
	BYTE			SendFileIndex;
	char			IPStr[30];
	UINT			IPRequestTick;
	volatile BYTE	OpenFileState;
	HashMap<uint, uint> *pResendList;
	char			Buff[1];
};

struct FTPThreadData
{
	FTPThreadData() :NewClients(NEW_CLIENT_COUNT){
	}
	SafeArray<FTPClientData*>	NewClients;
};

//FTP 使用1个Accept线程、N个RecvCMD线程、1个Queue线程、1个传输线程。
typedef HashMap<UINT, FTPFileData*> FTPFileMap;
typedef vector<FTPFileData*>		FTPFileList;
class FTPServer
{
public:
	FTPServer();
	~FTPServer();
	bool	Init(const FTPServerInitData &data);

	void	_ThreadAccept();
	void	_ThreadTrans();
	void	_ThreadQueue();
	void	_ThreadRecvCmd();
	void	_ThreadOpenFile();
	void	_ThreadFileWatcher();
protected:
	uint				InitFileData(FTPClientData *pc);
	void				CheckFileList(FTPFileMap *pfilemap, FTPFileList*pfileList);
	FTPFileData*		GetFileHandle(FTPFileMap *pfilemap, char *pname, UINT nameLength);
	FTPRecvState		RecvData(FTPClientData *pc);
	void				RemoveClient(FTPClientData *pclient, FTPRemoveType remove);
	UINT				GetSendInterval(UINT sendErrCount, UINT baseInterval);
	CACHELINE(0);
	volatile bool		m_bRun;
	CACHELINE(1);
	volatile UINT		m_TransNum;
	CACHELINE(2);
	volatile UINT		m_RecvCmdNum;
	CACHELINE(3);
	volatile UINT		m_SuccessNum;
	CACHELINE(4);
	volatile UINT		m_RecvThreadIdx;
	CACHELINE(5);
	volatile UINT		m_QueueNum;
	CACHELINE(6);
	FTPThreadData		m_QueueThreadData;
	CACHELINE(7);
	FTPThreadData		m_TransThreadData;
	CACHELINE(8);
	FTPFileMap				m_FileList;
	CACHELINE(9);
	FTPThreadData		   *m_pRecvThreadData;
	CACHELINE(10);
	FTPThreadData		   *m_pQueueThreadData;
	CACHELINE(11);
	FTPThreadData		   *m_pOpenFileThreadData;
	CACHELINE(12);
	volatile UINT		   m_MaxQueueNum;
	CACHELINE(13);
	volatile UINT		   m_MaxRecvCmdNum;
	CACHELINE(14);
	volatile UINT		   m_MaxTransNum;
	CACHELINE(15);
	SafeArray<FTPFileModify>  m_FileModify;
	CACHELINE(16);
	volatile UINT			m_TodayCompletionNum;
	CACHELINE(17);
	HttpClient				m_HTTPClient;
	struct tm				m_Today;
	FTPFileList				m_FreeFileList;
	HANDLE					m_DirHandle;
	UINT					m_TotalTransNum;	//可以同时传输的数量
	UINT					m_TransBandwidth;
	FTPServerInitData		m_InitData;
	SOCKET					m_Socket;

};