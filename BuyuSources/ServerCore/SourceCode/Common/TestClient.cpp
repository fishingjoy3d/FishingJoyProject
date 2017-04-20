#include "stdafx.h"
#include "TestClient.h"

void RemoveClient(ClientData*pc)
{
	pc->InsideRemoved = true;
	if (InterlockedDecrement(&pc->RefCount) <= 0)
	{
		while (pc->RecvList.HasItem())
			free(pc->RecvList.GetItem());
		pc->RecvList.~SafeList();
		closesocket(pc->Socket);
		free(pc);
	}
}

static UINT WINAPI ThreadFunc1(void *p)
{
	((TestClient*)p)->RecvThread();
	return 0;
}
static UINT WINAPI ThreadFunc2(void *p)
{
	((TestClient*)p)->SendThread();
	return 0;
}
static UINT WINAPI ThreadFunc3(void *p)
{
	((TestClient*)p)->ConnectThread();
	return 0;
}

void TestClient::ConnectThread()
{
	fd_set  *pSet = CreateFDSet();
	timeval time = { 0, 0 };
	int index = 0;
	vector<ClientData*> clientList;
	const int stepCount = 300;
	UINT connectCount = 0;
	UINT disCount = 0;
	UINT createNum = 0;
	while (m_bRun)
	{
		for (int i = 0; index < m_MaxNum && i < stepCount; ++index, ++i)
		{
			SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (s == INVALID_SOCKET)
			{
				Log(L"invalid socket.");
				continue;
			}

			SOCKADDR_IN addr;
			memset(&addr, 0, sizeof(addr));
			inet_pton(AF_INET, m_IP, &addr.sin_addr.s_addr);
			addr.sin_family = AF_INET;
			addr.sin_port = htons(m_InitData.Port);

			setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const char*)&m_InitData.SocketRecvSize, sizeof(uint));
			setsockopt(s, SOL_SOCKET, SO_SNDBUF, (const char*)&m_InitData.SocketSendSize, sizeof(uint));

			unsigned long arg = 1;
			ioctlsocket(s, FIONBIO, &arg);

			long noDelay = 1;
			int len = sizeof(long);
			setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&noDelay, len);
			int ret = connect(s, (const struct sockaddr *) &addr, sizeof(addr));

			ClientData *pc = new(malloc(sizeof(ClientData)+m_InitData.BuffSize))ClientData;
			pc->Socket = s;
			pc->Offset = 0;
			pc->RecvSize = 0;
			pc->RefCount = 0;
			pc->SendTick = 0;
			pc->InsideRemoved = false;
			pc->RecvTick = GetTickCount();
			++createNum;
			if (ret == 0)
			{
				++connectCount;
				int idx = connectCount % m_InitData.ThreadNum;
				pc->RefCount = 2;
				m_NewRecvList[idx].AddItem(pc);
				m_NewSendList[idx].AddItem(pc);
				Log(L"直接连接成功:%d", connectCount);
			}
			else if (ret == -1)
			{
				clientList.push_back(pc);
			//	Log(L"connect failed.");
			}
		}
		UINT tick = GetTickCount();
		//检查连接是否已经成功
		if (clientList.size() > 0)
		{
			FD_ZERO(pSet);
			for (UINT i = 0; i < clientList.size(); ++i)
			{
				FD_ADD(clientList[i]->Socket, pSet);
			}
			int nRet = select(0, NULL, pSet, NULL, &time);
			
			for (uint i = 0; i < clientList.size();)
			{
				ClientData *pscd = clientList[i];
				if (tick - pscd->RecvTick > m_InitData.Timeout)
				{
					ListRemoveAt(clientList, i);
					RemoveClient(pscd);
					++disCount;
					Log(L"连接超时:%d", disCount);
					continue;
				}
				if (!FD_ISSET(pscd->Socket, pSet))
				{
					++i;
					continue;
				}
				int error = -1, len = sizeof(int);
				getsockopt(pscd->Socket, SOL_SOCKET, SO_ERROR, (char*)&error, (socklen_t*)&len);
				if (error == 0)
				{
					pscd->RecvTick = GetTickCount();
					++connectCount;
					pscd->RefCount = 2;
					int idx = connectCount % m_InitData.ThreadNum;
					m_NewRecvList[idx].AddItem(pscd);
					m_NewSendList[idx].AddItem(pscd);
					//Log(L"连接成功:%d", connectCount);
					ListRemoveAt(clientList, i);
					continue;
				}
				else
				{
					closesocket(pscd->Socket);
					ListRemoveAt(clientList, i);
					RemoveClient(pscd);
					++disCount;
					Log(L"连接失败:%d", disCount);
					continue;
				}
			}
		}
		Sleep(10);
	}
	
	for (UINT i = 0; i < clientList.size(); ++i)
		RemoveClient(clientList[i]);
	DeleteFDSet(pSet);
	::InterlockedIncrement(&m_ExitCount);
}

void TestClient::SendThread()
{
	vector<ClientData*> clientList;
	UINT halfTick = m_InitData.Timeout >> 1;
	NetCmd nc;
	nc.SetCmdSize(sizeof(nc));
	nc.SetCmdType(ConvertDWORDToWORD(m_InitData.CmdHearbeat));
	long threadIdx = ::InterlockedIncrement(&m_SendIndex) - 1;
	SafeList<ClientData*> &sendList = m_NewSendList[threadIdx];
	Timer t;
	t.Begin();
	while (m_bRun)
	{
		while (sendList.HasItem())
			clientList.push_back(sendList.GetItem());
		
		UINT tick = GetTickCount();
		for (UINT i = 0; i < clientList.size();)
		{
			ClientData *pc = clientList[i];
			if (pc->InsideRemoved)
			{
				RemoveClient(pc);
				ListRemoveAt(clientList, i);
				continue;
			}
			if (tick - pc->SendTick > halfTick)
			{
				int nret = send(pc->Socket, (const char*)&nc, nc.CmdSize, 0);
				if (nret != nc.CmdSize)
				{
					Log(L"send hearbeat err:%d, lastErr:%d", nret, WSAGetLastError());
				}
				pc->SendTick = tick;
			}
			++i;
		}
		m_SendData[threadIdx].Num = clientList.size();
		m_SendData[threadIdx].Delta = t.Intecept();
		Sleep(m_InitData.SleepTime);
	}
	for (UINT i = 0; i < clientList.size(); ++i)
		RemoveClient(clientList[i]);
	::InterlockedIncrement(&m_ExitCount);
}
bool TestClient::RecvData(ClientData *pscd, int nRecvSize)
{
	pscd->RecvSize += nRecvSize;
	UINT cmdSize = sizeof(NetCmd);
	if (pscd->RecvSize > m_InitData.BuffSize)
	{
		Log(L"Recv Size Error1:%d", pscd->RecvSize);
		return false;
	}
	while (pscd->RecvSize >= cmdSize)
	{
		BYTE *pBuff = pscd->Buff + pscd->Offset;
		cmdSize = *((USHORT*)&pBuff[0]);
		if (cmdSize > m_InitData.BuffSize)
		{
			Log(L"Recv Size Error2:%d", cmdSize);
			return false;
		}
		if (pscd->RecvSize >= cmdSize)
		{
			NetCmd *pCmd = (NetCmd*)pBuff;
			if (pCmd->GetCmdType() == m_InitData.CmdHearbeat)
			{

			}
			else
			{
				pscd->RecvList.AddItem(pCmd);
				pCmd = CreateCmd(ConvertDWORDToWORD(cmdSize), pBuff);
			}
			pscd->Offset += cmdSize;
			pscd->RecvSize -= cmdSize;
		}
		else
			break;
	}
	UINT freeBuffSize = m_InitData.BuffSize - (pscd->Offset + pscd->RecvSize);
	if (freeBuffSize < 32 || freeBuffSize < cmdSize - pscd->RecvSize)
	{
		memmove(pscd->Buff, pscd->Buff + pscd->Offset, pscd->RecvSize);
		pscd->Offset = 0;
	}

	return true;
}
void TestClient::RecvThread()
{
	fd_set  *pSet = CreateFDSet();
	timeval time = { 0, 0 };
	vector<ClientData*> recvList;
	long threadIdx = ::InterlockedIncrement(&m_RecvIndex) - 1;
	SafeList<ClientData*> &recvNewList = m_NewRecvList[threadIdx];
	Timer t;
	t.Begin();
	while (m_bRun)
	{
		while (recvNewList.HasItem())
		{
			recvList.push_back(recvNewList.GetItem());
		}
		FD_ZERO(pSet);
		UINT tick = GetTickCount();
		uint idx = 0;
		for (; idx < recvList.size();)
		{
			ClientData *pscd = recvList[idx];
			if (pscd->InsideRemoved || tick - pscd->RecvTick > m_InitData.Timeout)
			{
				if (tick - pscd->RecvTick > m_InitData.Timeout)
					Log(L"Timeout:%d", tick - pscd->RecvTick);
				RemoveClient(pscd);
				ListRemoveAt(recvList, idx);
				continue;
			}
			FD_ADD(pscd->Socket, pSet);
			++idx;
		}
		if (idx > 0)
		{
			int nRet = select(0, pSet, NULL, NULL, &time);
			if (nRet != 0)
			for (uint i = 0; i < recvList.size(); ++i)
			{
				ClientData *pscd = recvList[i];
				if (!FD_ISSET(pscd->Socket, pSet))
					continue;

				int curPos = pscd->Offset + pscd->RecvSize;
				nRet = recv(pscd->Socket, (char*)pscd->Buff + curPos, m_InitData.BuffSize - curPos, 0);
				if (nRet == 0 || (nRet == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
				{
					Log(L"recv error:%d, LastCode:%d", nRet, WSAGetLastError());
					pscd->InsideRemoved = true;
				}
				else if (!RecvData(pscd, nRet))
				{
					Log(L"recv size error:%d, LastCode:%d", nRet, WSAGetLastError());
					pscd->InsideRemoved = true;
				}
				else
				{
					while (pscd->RecvList.HasItem())
					{
						free(pscd->RecvList.GetItem());
					}
					pscd->RecvTick = tick;
				}
			}// end for
		}//enf if

		m_RecvData[threadIdx].Num = recvList.size();
		m_RecvData[threadIdx].Delta = t.Intecept();
		Sleep(m_InitData.SleepTime);
	}
	for (UINT i = 0; i < recvList.size(); ++i)
		RemoveClient(recvList[i]);
	DeleteFDSet(pSet);
	::InterlockedIncrement(&m_ExitCount);
}
void TestClient::Init(const char *pIP, int maxNum, const ClientInitData &data)
{
	m_SendIndex = 0;
	m_RecvIndex = 0;
	m_bRun = true;
	m_ExitCount = 0;
	m_InitData = data;
	m_MaxNum = maxNum;
	strcpy_s(m_IP, sizeof(m_IP), pIP);
	for (UINT i = 0; i < m_InitData.ThreadNum; ++i)
	{
		::_beginthreadex(0, 0, ThreadFunc1, this, 0, 0);
		::_beginthreadex(0, 0, ThreadFunc2, this, 0, 0);
	}
	::_beginthreadex(0, 0, ThreadFunc3, this, 0, 0);
	
}
void TestClient::Shutdown()
{
	m_bRun = false;
	while (ConvertIntToDWORD(m_ExitCount) != m_InitData.ThreadNum * 2 + 1)
		Sleep(10);
	for (int i = 0; i < MAX_CLIENT_THREAD; ++i)
	{
		while (m_NewRecvList[i].HasItem())
			RemoveClient(m_NewRecvList[i].GetItem());
		while (m_NewSendList[i].HasItem())
			RemoveClient(m_NewSendList[i].GetItem());
	}
}