#include "stdafx.h"
#include "TcpClientList.h"
TcpClientList::TcpClientList()
{
	m_SendIndex = 0;
}
TcpClientList::~TcpClientList()
{
	if (m_TcpClientMap.empty())
		return;
	delete[] m_GetCmdArray;
	vector<TcpClientOnce*>::iterator Iter = m_TcpClientMap.begin();
	for (; Iter != m_TcpClientMap.end(); ++Iter)
	{
		delete *Iter;
	}
	m_TcpClientMap.clear();
}
bool TcpClientList::Init(std::vector<TcpClientConnectOnce>& pList)
{
	if (pList.empty())
	{
		ASSERT(false);
		return false;
	}
	std::vector<TcpClientConnectOnce>::iterator Iter = pList.begin();
	for (; Iter != pList.end(); ++Iter)
	{
		//创建一个节点
		TcpClientOnce* pOnce = new TcpClientOnce();
		pOnce->TcpID = Iter->TcpID;
		pOnce->m_TcpStates = false;
		if (!pOnce->m_Tcp.Init(Iter->IpStr, Iter->ClientData))
		{
			delete pOnce;
			return false;
		}
		pOnce->m_TcpStates = true;
		m_TcpClientMap.push_back(pOnce);//保存起来
		//OnTcpClientConnect(&pOnce->m_Tcp);//当连接成功的时候
	}
	if (!m_TcpClientMap.empty())
	{
		m_GetCmdArray = new NetCmd*[m_TcpClientMap.size()];
	}
		
	return true;
}
bool TcpClientList::Send(NetCmd *pCmd, bool bNoneUse)
{
	if (!IsConnected())
	{
		ASSERT(false);
		return false;
	}
	//发送命令出去 从全部已经连接了的里面选择一个进行发送
	//当前有个记录 我们按记录进行发送
	if (m_SendIndex >= m_TcpClientMap.size())
		m_SendIndex = m_SendIndex%m_TcpClientMap.size();//重新设置索引
	if (!m_TcpClientMap[m_SendIndex]->m_TcpStates || !m_TcpClientMap[m_SendIndex]->m_Tcp.IsConnected())
	{
		DWORD NowIndex = m_SendIndex;
		//进行循环变
		++m_SendIndex;
		if (m_SendIndex >= m_TcpClientMap.size())
			m_SendIndex = m_SendIndex%m_TcpClientMap.size();
		while (m_SendIndex != NowIndex)
		{
			if (!m_TcpClientMap[m_SendIndex]->m_TcpStates || !m_TcpClientMap[m_SendIndex]->m_Tcp.IsConnected())
			{
				++m_SendIndex;
				if (m_SendIndex >= m_TcpClientMap.size())
					m_SendIndex = m_SendIndex%m_TcpClientMap.size();
				continue;
			}
			//连接的 
			m_TcpClientMap[m_SendIndex]->m_Tcp.Send(pCmd, bNoneUse);
			return true;
		}
		ASSERT(false);
		return false;
	}
	else
	{
		m_TcpClientMap[m_SendIndex]->m_Tcp.Send(pCmd, bNoneUse);//将命令发送出去
		++m_SendIndex;
		return true;
	}
}
void TcpClientList::Shutdown()
{
	vector<TcpClientOnce*>::iterator Iter = m_TcpClientMap.begin();
	for (; Iter != m_TcpClientMap.end(); ++Iter)
	{
		(*Iter)->m_Tcp.Shutdown();
	}
}
bool TcpClientList::IsConnected()
{
	vector<TcpClientOnce*>::iterator Iter = m_TcpClientMap.begin();
	for (; Iter != m_TcpClientMap.end(); ++Iter)
	{
		if((*Iter)->m_TcpStates)
			return true;
	}
	return false;
}
NetCmd** TcpClientList::CheckAllTcpClient(bool CheckNetStates)//关系里面的状态
{
	//vector<NetCmd*> pVec;
	ZeroMemory(m_GetCmdArray, sizeof(NetCmd*)*m_TcpClientMap.size());
	vector<TcpClientOnce*>::iterator Iter = m_TcpClientMap.begin();
	for (int i =0; Iter != m_TcpClientMap.end(); ++Iter)
	{
		if (CheckNetStates)
		{
			if ((*Iter)->m_Tcp.IsConnected())
			{
				if (!(*Iter)->m_TcpStates)
				{
					//重新连接成功了
					(*Iter)->m_TcpStates = true;
					std::cout << "TCPClientList 一个连接连接成功了\n";
				}
				//获取命令
				NetCmd* pCmd = (*Iter)->m_Tcp.GetCmd();
				if (!pCmd)
					continue;
				//pVec.push_back(pCmd);
				m_GetCmdArray[i] = pCmd;
				++i;
			}
			else
			{
				if ((*Iter)->m_TcpStates)
				{
					//失去连接了
					(*Iter)->m_TcpStates = false;
					std::cout << "TCPClientList 一个连接断开连接了\n";
				}
			}
		}
		else
		{
			NetCmd* pCmd = (*Iter)->m_Tcp.GetCmd();
			if (!pCmd)
				continue;
			//pVec.push_back(pCmd);
			m_GetCmdArray[i] = pCmd;
			++i;
		}
	}
	return m_GetCmdArray;
}
