//#pragma once
//
//#include <MSWSock.h>  
//struct MyClientData;
//struct ClientIOData;
//#ifndef MAX_SEND_CMD_COUNT			//每客户端最大发送命令的数量
//#define MAX_SEND_CMD_COUNT  256
//#endif
//#ifndef MAX_RECV_CMD_COUNT			//每客户端最大接收的命令数量
//#define MAX_RECV_CMD_COUNT  128
//#endif
//#ifndef ACCEPT_BUFF_SIZE			//接收连接的BUFF大小。
//#define ACCEPT_BUFF_SIZE	512
//#endif
//#ifndef ACCEPT_ALLOW_BUFF_SIZE		//初始连接允许接收的最大字节数
//#define ACCEPT_ALLOW_BUFF_SIZE 32
//#endif
//enum IOType
//{
//	IO_NONE,
//	IO_RECV,
//	IO_SEND,
//	IO_HEARBEAT,
//	IO_ACCEPT,
//	IO_SEND_FIRST_DATA,
//	IO_SEND_BACK,
//	IO_PING,
//	IO_SEND_VERSION,
//};
//enum SendState
//{
//	CLIENT_SEND_NONE,
//	CLIENT_SEND_POST,		//发送投递中
//	CLIENT_SEND_WAIT,		//发送等待中
//};
//
//struct ClientIOData
//{
//	OVERLAPPED		Overlapped;
//	WSABUF			WsaBuff;
//	BYTE			OptType;
//	UINT_PTR		AttachData;
//};
//struct AcceptBuffData
//{
//	SOCKADDR_IN		Addr;
//	int				AddrSize;
//	ClientIOData	PerIO;
//	char			Buff[ACCEPT_BUFF_SIZE];
//};
//struct NewClientData
//{
//	NewClientData(){
//	}
//	NewClientData(int n){
//	}
//	SOCKADDR_IN Addr;
//	char		Buff[ACCEPT_ALLOW_BUFF_SIZE];
//	ushort		RecvSize;
//	UINT_PTR	AttachData;
//};
//typedef SafeArray<NewClientData, 512> SafeNewClientList;
//typedef SafeArray<UINT, 64> SafeRecvBackList;
//struct MyDispatchData
//{
//	UINT				OnlineNum;
//	SafeAddClientList	NewClients;
//};
//struct MyClientData :public ServerClientData
//{
//	ClientIOData		RecvIO;
//	ClientIOData		SendIO;
//	ClientIOData		SendBackIO;
//	ClientIOData		HearbeatIO;
//	CACHELINE(1);
//	volatile UINT		RecvTick;
//	CACHELINE(2);
//	volatile UINT		SendTick;
//	CACHELINE(3);
//	volatile long		RefCount;
//	CACHELINE(4);
//	volatile bool		SendFirstData;
//	CACHELINE(6);
//	SafeSendList		SendList;
//	CACHELINE(7);
//	volatile UINT		RecvID;
//	CACHELINE(8);
//	CACHELINE(10);
//	volatile long		SendState;
//	CACHELINE(11);
//	volatile UINT		SendCmdTick;
//	CACHELINE(12);
//	UINT				Offset;
//	UINT				RecvSize;
//	UINT				SendID;
//	USHORT				ServerPort;
//	SOCKADDR_IN			ClientAddr;
//	bool				WaitRemove;
//	BYTE				DispIdx;
//	SafeRecvBackList	RecvIDList;
//	bool				bWaitAnswer;
//	char				Buff[1];
//};