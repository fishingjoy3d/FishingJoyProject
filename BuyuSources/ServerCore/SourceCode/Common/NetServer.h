#pragma once
#define SERVER_CONNECT_MAGIC 0x1343316f
#define MAX_SOCKET_THREAD 16
#define IS_ANSWER(X) ((X & 0x80000000) != 0)
#define IS_GW_ANSWER(X) ((X & 0xc0000000) == 0xc0000000)
#define RESET_ANSWER(X) ((X) &= 0x7fffffff)
#define RESET_GW_ANSWER(X) ((X) &= 0x3fffffff)
#define SET_ANSWER(X) ((X) | 0x80000000)
#define SET_GW_ANSWER(X) ((X) | 0xc0000000)
#define CMD_ID_SIZE (sizeof(UINT))
#define HEARBEAT_ID		0xffffffff
#define PING_ID			0x8fffffff

#define UDP_RESEND_TICK		35	//UDP重发超时ms

inline bool IsDisconnect(INT ret, UINT code)
{
	return (ret == SOCKET_ERROR && (code == WSAECONNRESET || code == WSAECONNABORTED));
}
enum ServerClientState
{
	CLIENT_STATE_ACCEPT,
	CLIENT_STATE_CHECKED,
	CLIENT_STATE_JOINED,
	CLIENT_STATE_REMOVE,
};
enum RemoveType
{
	REMOVE_NONE,
	REMOVE_NORMAL,				//上层移除
	REMOVE_RECV_ERROR,			//接收错误
	REMOVE_SEND_ERROR,			//发送错误
	REMOVE_TIMEOUT,				//超时
	REMOVE_CMD_SEND_OVERFLOW,	//发送队列溢出，发送命令太多
	REMOVE_CMD_RECV_OVERFLOW,	//接收队列溢出，接收命令太多
	REMOVE_CMD_SIZE_ERROR,		//接收的命令大小错误
	REMOVE_RECVBACK_NOT_SPACE,	//
	REMOVE_RELOGON,				//重登陆
};
inline const char *GetRemoveTypeString(RemoveType rt)
{
	switch (rt)
	{
	case REMOVE_NONE:
		return "REMOVE_NONE";
	case REMOVE_NORMAL:
		return "REMOVE_NORMAL";
	case REMOVE_RECV_ERROR:
		return "REMOVE_RECV_ERROR";
	case REMOVE_SEND_ERROR:
		return "REMOVE_SEND_ERROR";
	case REMOVE_TIMEOUT:
		return "REMOVE_TIMEOUT";
	case REMOVE_CMD_SEND_OVERFLOW:
		return "REMOVE_CMD_SEND_OVERFLOW";
	case REMOVE_CMD_RECV_OVERFLOW:
		return "REMOVE_CMD_RECV_OVERFLOW";
	case REMOVE_CMD_SIZE_ERROR:
		return "REMOVE_CMD_SIZE_ERROR";
	case REMOVE_RECVBACK_NOT_SPACE:
		return "REMOVE_RECVBACK_NOT_SPACE";
	case REMOVE_RELOGON:
		return "REMOVE_RELOGON";
	default:
		return "REMOVE_OTHER";
	}
}
inline void GetIPString(UINT ip, char xx[30])
{
	sprintf_s(xx, 30, "%d.%d.%d.%d",
		(ip >> 0) & 0xff,
		(ip >> 8) & 0xff,
		(ip >> 16) & 0xff,
		(ip >> 24) & 0xff
		);
}
inline void GetIPString(UINT ip, USHORT port, char xx[30])
{
	sprintf_s(xx, 30, "%d.%d.%d.%d:%d",
		(ip >>  0) & 0xff,
		(ip >>  8) & 0xff,
		(ip >> 16) & 0xff,
		(ip >> 24) & 0xff,
		port
		);
}
//客户端连接信息
struct ServerClientData
{
	ServerClientData(UINT recvCount) :RecvList(recvCount)
	{

	}
	//基本信息
	CACHELINE(1);
	volatile bool		Removed;			//是否标记为移除
	CACHELINE(2);
	volatile RemoveType	RemoveCode;			//客户端断开的标记
	CACHELINE(3);
	volatile bool		IsInScene;			//在场景中为TRUE，大厅为FALSE
	CACHELINE(4);
	SOCKET				Socket;
	UINT				IP;
	USHORT				Port;
	SafeArray<NetCmd*>	RecvList;			//接收的队列
	UINT_PTR			OutsideExtraData;	//外部附加的数据
	DWORD				dwChecked;			//验证
};

struct ServerInitData
{
	BYTE		ServerID;				//服务器识别码
	INT			Timeout;				//接收超时(毫秒)
	ushort		Port;					//端口
	ushort		ListenCount;			//监听排队的数量	//1 - 64
	uint		BuffSize;				//接收缓冲区大小, 最大能接收的命令长度
	uint		MaxSendCountPerFrame;	//每帧最多发送多少个命令
	uint		MaxAcceptNumPerFrame;	//每次Accept间隔能接受多少个连接;
	uint		SocketRecvSize;			//底层缓冲区大小
	uint		SocketSendSize;			//底层缓冲区大小
	uint		MaxRecvCmdCount;		//最多可同时接收多少个命令，多了就踢除
	uint		MaxSendCmdCount;		//最多可同时发送多少个命令，多了就踢除
	uint		SleepTime;				//发送和接收线程睡眠的间隔
	uint		AcceptSleepTime;		//Accept睡眠时间
	ushort		CmdHearbeat;			//心跳的命令
	ushort		RecvThreadNum;			//接收线程的数量
	ushort		SendThreadNum;			//发送线程的数量
	bool		AcceptRecvData;			//Accept接收数据才触发。
	ushort		SceneHearbeatTick;		//在场景中的心跳TICK，此参数重要，对客户端的DelayACK机制有屏蔽功能。取值范围:(50 - 100)
	ServerInitData()
	{
		memset(this, 0, sizeof(*this));
		SceneHearbeatTick = 100;
	}
};
#define CONNECT_CHECK_OK	 (0u)
#define CONNECT_CHECK_FAILED (0xffffffffu)
class INetHandler
{
public:
	//这些回调都是线程触发
	//是否允许新的客户端与服务器连接:
	//返回CONNECT_CHECK_OK表示允许;
	//返回CONNECT_CHECK_FAILED表示不允许;
	//返回>0表示不允许，并将pData中的数据发送给客户端。
	virtual uint CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize) = 0;
	//新的客户端加入，以及初始接收到的连接数据。
	virtual bool NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize) = 0;
	//客户端断开连接及断开连接的类型。GetRemoveTypeString(rt)获取字符串描述。
	virtual void Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt)
	{
		UNREFERENCED_PARAMETER(ServerID);
		UNREFERENCED_PARAMETER(pClient);
		UNREFERENCED_PARAMETER(rt);
	};
};

class NetServer
{
public:
	virtual ~NetServer(){};
	virtual bool Init(const ServerInitData &data, bool bTCP = true) = 0;
	virtual bool Kick(ServerClientData *pClient, RemoveType rt = REMOVE_NORMAL) = 0;
	virtual void Shutdown() = 0;
	virtual void SetCmdHandler(INetHandler *pHandler) = 0;
	virtual bool Send(ServerClientData *pClient, NetCmd *pCmd) = 0;
	virtual UINT JoinNum()const = 0;
	virtual void SwitchAccept(bool bEnable) = 0;
};