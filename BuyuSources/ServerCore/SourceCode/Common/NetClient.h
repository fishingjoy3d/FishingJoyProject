#pragma once

struct ClientInitData
{
	uint		Timeout;	//超时(毫秒)
	ushort		Port;		//端口
	uint		BuffSize;	//缓冲区大小
	uint		SocketRecvSize;
	uint		SocketSendSize;
	uint		CmdHearbeat;
	uint		SleepTime;
	UINT		RecvArraySize;
	UINT		SendArraySize;
	uint		ThreadNum;	//测试使用
};

class NetClient
{
public:
	virtual bool Init(const char *ip, const ClientInitData &data, void *pData = NULL, int sendSize = 0) = 0;
	virtual bool Send(NetCmd *pCmd, bool bNoneUse) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsConnected()const = 0;
	virtual NetCmd *GetCmd() = 0;
};