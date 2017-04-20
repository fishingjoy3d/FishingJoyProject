#include "stdafx.h"
#include "UDPClient.h"
#define SLEEP_TIME 10

static UINT WINAPI ThreadFunc1(void *p)
{
	((UDPClient*)p)->RecvThread();
	return 0;
}

UDPClient::UDPClient()
{
	m_bConnected = false;
	m_Socket = NULL;
	m_RecvList = NULL;
	m_SendList = NULL;
	
}
UDPClient::~UDPClient()
{

}
UINT StrToIP(const char *pc)
{
	UINT a, b, c, d;
	const char *p1= strchr(pc, '.') + 1;
	const char *p2 = strchr(p1, '.') + 1;
	const char *p3 = strchr(p2, '.') + 1;
	a = atoi(pc);
	b = atoi(p1);
	c = atoi(p2);
	d = atoi(p3);
	return (a << 24) | (b << 16) | (c << 8) | d;
}
bool UDPClient::Init(const char *ip, const ClientInitData &data, void *pData, int sendSize)
{
	const char *pcGameServerIP = NULL;
	USHORT gameServerPort = 0;
	const UINT CONNECT_TIMEOUT = 3000;
	m_InitData = data;
	SOCKADDR_IN addr;
	int addrSize = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, (void *)&addr.sin_addr);
	addr.sin_port = htons(data.Port);
	CreateSocketData csd;
	if (CreateSocket(CST_TCP, 0, m_InitData.SocketRecvSize, m_InitData.SocketSendSize, csd) == false)
		return false;
	unsigned long arg = 0;
	ioctlsocket(csd.Socket, FIONBIO, &arg);
	//连接到TCP
	int ret = connect(csd.Socket, (sockaddr*)&addr, addrSize);
	if (ret != 0)
	{
		closesocket(csd.Socket);
		return false;
	}
	char buff[64];
	UINT tick = timeGetTime();
	bool bok = false;
	while (timeGetTime() - tick < CONNECT_TIMEOUT)
	{
		ret = recv(csd.Socket, buff, sizeof(buff), 0);
		if (ret == 16 && *(UINT*)buff == SERVER_CONNECT_MAGIC)
		{
			bok = true;
			break;
		}
		Sleep(50);
	}
	closesocket(csd.Socket);
	if (!bok)
		return false;
	USHORT newPort = *((UINT*)(buff + 12));
	//连接到UDP
	if (CreateSocket(CST_UDP, 0, m_InitData.SocketRecvSize, m_InitData.SocketSendSize, csd) == false)
		return false;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, (void *)&addr.sin_addr);
	addr.sin_port = htons(newPort);
	ret = connect(csd.Socket, (sockaddr*)&addr, addrSize);
	if (ret != 0)
	{
		closesocket(csd.Socket);
		return false;
	}
	tick = timeGetTime();
	char newbuff[64];
	UINT* pSendData = (UINT*)(buff + 0);
	*(pSendData + 0) = 0x8fffffff;
	*(pSendData + 1) |= 0xc0000000;
	*(pSendData + 2) |= 0xc0000000;
	UINT hearbeat = HEARBEAT_ID;
	bok = false;
	while (timeGetTime() - tick < CONNECT_TIMEOUT)
	{
		send(csd.Socket, (char*)pSendData, 12, 0);
		ret = recv(csd.Socket, newbuff, sizeof(newbuff), 0);
		if (ret == 4 && *(UINT*)newbuff == HEARBEAT_ID)
		{
			//发送3次心跳
			for (int i = 0; i < 3; ++i)
			{
				send(csd.Socket, (char*)&hearbeat, 4, 0);
				Sleep(50);
			}
			bok = true;
			break;
		}
		Sleep(50);
	}
	if (!bok)
	{
		closesocket(csd.Socket);
		return false;
	}

	m_Socket = csd.Socket;
	m_SendList = new SafeCmdList(data.SendArraySize);
	m_RecvList = new SafeCmdList(data.RecvArraySize);

	m_RecvID = 1;
	m_SendID = 1;
	m_bSendBackID = false;
	m_pSendBuff = NULL;

	m_ServerAddr = addr;
	m_bConnected = true;
	::_beginthreadex(0, 0, (ThreadFunc1), this, 0, 0);
	return true;
}

void UDPClient::CloseSocket()
{
	m_bConnected = false;
	closesocket(m_Socket);
}
bool UDPClient::RecvData(char *pbuff, int recvSize)
{
	int offset = 0;
	while (offset < recvSize)
	{
		uint recvID = *(UINT*)(pbuff + offset);
		offset += 4;
		if (recvID == HEARBEAT_ID || recvID == PING_ID)	//心跳
		{
			// Debug.Log("recv hearbeat.");
		}
		else if (IS_ANSWER(recvID))
		{
			recvID = RESET_ANSWER(recvID);
			if (recvID == m_SendID && m_pSendBuff != null)
			{
				free(m_pSendBuff);
				m_pSendBuff = NULL;
			}
		}
		else
		{ 
			//static int IsShow = 0;
			//if (recvID != m_RecvID + 1 && IsShow < 20)
			//{
			//	//Log("RecvCMD:%d, %d", recvID, m_RecvID);
			//	++IsShow;
			//}
				
			NetCmd *pcmd = (NetCmd*)(pbuff + offset);
			if (pcmd->GetCmdSize() > recvSize - offset)
			{
				Log("命令未接收完全");
				return false;
			}
			if (recvID == m_RecvID + 1)
			{
				++m_RecvID;
				NetCmd *pnewcmd = CreateCmd(pcmd->GetCmdSize());
				memcpy_s(pnewcmd, pcmd->GetCmdSize(), pcmd, pcmd->GetCmdSize());
				if(m_RecvList->HasSpace())
					m_RecvList->AddItem(pnewcmd);
				else
				{
					Log("UDPClient接收空间已满!");
					return false;
				}
			}
			offset += pcmd->GetCmdSize();
			m_bSendBackID = true;
		}
	}
	return true;
}

void UDPClient::RecvThread()
{
	char	*pBuff = (char*)malloc(m_InitData.BuffSize);
	UINT	RecvTick = timeGetTime();
	UINT	timeout = m_InitData.Timeout >> 1;
	UINT	SendSize = 0;
	UINT	SendTick = 0;
	UINT	SendCmdTick = 0;
	while (m_bConnected)
	{
		UINT tick = timeGetTime();
		//for (int i = 0; i < 3; ++i)
		{
			int nRet = recv(m_Socket, (char*)pBuff, m_InitData.BuffSize, 0);
			if (nRet > 0)
			{
				if (RecvData(pBuff, nRet) == false)
				{
					Log("退出1");
					CloseSocket();
					return;
				}
				RecvTick = tick;
			}
			else 
			{
				if (tick - RecvTick > m_InitData.Timeout)
				{
					Log("退出2");
					CloseSocket();
					return;
				}
				//break;
			}

			if (m_pSendBuff != null)
			{
				//Log("处理1");
				if (tick - SendCmdTick > UDP_RESEND_TICK || m_bSendBackID)
				{
					m_pSendBuff[0] = SET_ANSWER(m_RecvID);
					if (send(m_Socket, (char*)m_pSendBuff, SendSize, 0) == SendSize)
					{
						m_bSendBackID = false;
						SendTick = SendCmdTick = tick;
					}
				}
			}
			else if (m_SendList->HasItem())
			{
				//Log("处理2");
				m_pSendBuff = (UINT*)m_SendList->GetItem();
				m_pSendBuff[0] = SET_ANSWER(m_RecvID);
				m_pSendBuff[1] = ++m_SendID;
				SendSize = ((NetCmd*)(m_pSendBuff + 2))->GetCmdSize() + sizeof(UINT)* 2;
				if (send(m_Socket, (char*)m_pSendBuff, SendSize, 0) == SendSize)
				{
					m_bSendBackID = false;
					SendTick = SendCmdTick = tick;
				}
				else
					SendCmdTick = 0;
			}
			else if (m_bSendBackID || tick - SendTick > timeout)
			{
				
				UINT backid = SET_ANSWER(m_RecvID);
				//Log("处理3 :%d", m_RecvID);
				//Log("回复或者心跳:%d, backID:%u", m_RecvID, backid);
				if (send(m_Socket, (char*)&backid, 4, 0) == 4)
				{
					m_bSendBackID = false;
					SendTick = tick;
				}
			}
			//Sleep(m_InitData.SleepTime);
		}
		Sleep(m_InitData.SleepTime);
	}
	free(pBuff);
}
bool UDPClient::Send(NetCmd *pCmd, bool bNoneUse)
{
	if (m_SendList->HasSpace() == false)
	{
		Log("发送队列已满:%d", m_SendList->ArraySize());
		return false;
	}
	BYTE *pData = (BYTE*)malloc(pCmd->GetCmdSize() + CMD_ID_SIZE * 2);
	if (!pData)
		return false;
	NetCmd *pNewCmd = (NetCmd*)(pData + CMD_ID_SIZE * 2);
	memcpy_s(pNewCmd, pCmd->GetCmdSize(), pCmd, pCmd->GetCmdSize());
	m_SendList->AddItem((NetCmd*)pData);
	return true;
}
bool UDPClient::SendImmediate(NetCmd *pCmd, bool &b10035)
{
	return false;
}
void UDPClient::Shutdown()
{
	if (m_bConnected)
	{
		CloseSocket();
	}
	ReleaseData();
}
void UDPClient::ReleaseData()
{
	while (m_RecvList->HasItem())
		free(m_RecvList->GetItem());
	while (m_SendList->HasItem())
		free(m_SendList->GetItem());
	delete (m_RecvList);
	delete(m_SendList);
}
