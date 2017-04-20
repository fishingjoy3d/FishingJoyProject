//封装性质的类
#pragma once
#include "TCPClient.h"
#include "NetClient.h"
//#define IP_MAX_LENGTH 16
struct TcpClientConnectOnce
{
	DWORD			TcpID;
	char			IpStr[IP_MAX_LENGTH];
	ClientInitData	ClientData;
};
struct TcpClientOnce
{
	DWORD				TcpID;
	TCPClient			m_Tcp;//连接到数据库的连接
	bool				m_TcpStates;
};
class TcpClientList
{
public:
	TcpClientList();
	virtual ~TcpClientList();
	//初始化
	bool Init(std::vector<TcpClientConnectOnce>& pList);//同时连接多个
	//发送命令
	bool Send(NetCmd *pCmd, bool bNoneUse);
	//关闭
	void Shutdown();
	//是否连接
	bool IsConnected();
	//内部自动进行 连接 
	NetCmd** CheckAllTcpClient(bool CheckNetStates);

	DWORD GetTcoClientSum(){ return m_TcpClientMap.size(); }
private:
	vector<TcpClientOnce*>					m_TcpClientMap;//对应的Map
	DWORD									m_SendIndex;
	NetCmd**								m_GetCmdArray;
};