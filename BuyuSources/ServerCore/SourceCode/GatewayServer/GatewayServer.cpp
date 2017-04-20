// DBServer.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
class MyHandler : public INetHandler
{
	virtual uint CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize)
	{
		return CONNECT_CHECK_OK;
	}
	//新的客户端加入，以及初始接收到的连接数据。
	virtual bool NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize)
	{
		return true;
	}
	//客户端断开连接及断开连接的类型。GetRemoveTypeString(rt)获取字符串描述。
	virtual void Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt)
	{
		UNREFERENCED_PARAMETER(ServerID);
		UNREFERENCED_PARAMETER(pClient);
		UNREFERENCED_PARAMETER(rt);
	};
};
MyHandler g_handler;
int _tmain(int argc, _TCHAR* argv[])
{
	GWInitData data;
	
	data.ServerID = 0;
	data.Timeout = 5000;
	data.Port = 41056;
	data.ListenCount = 100;
	data.BuffSize = 4096;
	data.MaxSendCountPerFrame = 1;
	data.MaxAcceptNumPerFrame = 100;
	data.SocketRecvSize = 4096;
	data.SocketSendSize = 4096;
	data.MaxRecvCmdCount = 256;
	data.MaxSendCmdCount = 256;
	data.SleepTime = 1;
	data.AcceptSleepTime = 1;
	data.CmdHearbeat = 0;
	data.RecvThreadNum = 8;
	data.SendThreadNum = 8;
	data.AcceptRecvData = false;
	data.SceneHearbeatTick = 100;

	strcpy_s(data.LogonServerIP, sizeof(data.LogonServerIP), "139.196.192.84");
	strcpy_s(data.OperationServerIP, sizeof(data.OperationServerIP), "139.196.192.84");
	strcpy_s(data.LocalServerIP, sizeof(data.LocalServerIP), "139.196.192.139");
	data.LogonClientPort = 40056;
	data.LogonServerPort = 40056;
	data.LogonThreadNum = 4;
	data.LogonAcceptPerFrame = 10;
	data.LogonTimeout = 5000;

	data.OperationAcceptPerFrame = 10;
	data.OperationClientPort = 1680;
	data.OperationServerPort = 1680;
	data.OperationTimeout    = 3000;
	NewGateServer ggg;
	ggg.SetCmdHandler(&g_handler);
	ggg.Init(data);
	while (1)
	{
		Sleep(100);
	}

	return 0;
}

