#include "stdafx.h"
#include "OperatorHelper.h"
#include "FishServer.h"

OperatorHelper::OperatorHelper()
{
	DomeOperator* dome = new DomeOperator();
	_operators[dome->GetType()] = dome;

}


OperatorHelper::~OperatorHelper()
{
}


DomeOperator* OperatorHelper::GetDomeOperator()
{
	DomeOperator* Operator = NULL;
	OPERATORHELPERS::iterator it = _operators.find(Dome_ChannelType);
	if (it != _operators.end())
	{
		Operator = (DomeOperator*)it->second;
	}
	return Operator;
}

void OperatorHelper::AcceptDomeServerLogon(TCHAR* user)
{
	DomeOperator* Operator = GetDomeOperator();
	if (Operator != NULL)
	{
		Operator->AcceptOperatorServerLogon(user);
	}
}
void OperatorHelper::AcceptDomeClientLogon(tagLogon& logon, BYTE LogonClientID, ServerClientData* pClient)
{
	DomeOperator* Operator = GetDomeOperator();
	if (Operator != NULL)
	{
		Operator->AcceptClientLogon(logon, LogonClientID, pClient);
	}
}

void OperatorHelper::Update()
{
	OPERATORHELPERS::iterator it = _operators.begin();
	for (; it != _operators.end(); ++ it)
	{
		it->second->Update();
	}
}


BaseOperator::BaseOperator()
{

}
BaseOperator::~BaseOperator()
{

}

void BaseOperator::Update()
{

}




DomeOperator::DomeOperator()
{
	_Type = Dome_ChannelType;
	_wait_sec = 20;
}


DomeOperator::~DomeOperator()
{

}


void DomeOperator::AcceptOperatorServerLogon(TCHAR* user)
{

	LogInfoToFile("Log", "http AcceptOperatorServerLogon 接收到数据 %s", user);
	USERS::iterator it = _server_wait_users.find(user);
	if (it != _server_wait_users.end())
	{
		_server_wait_users.erase(it);
	}

	time_t cur_time = time(0);
	_server_wait_users[user] = cur_time;
	
}


//void AcceptClient(tagLogon& logon, BYTE	LogonClientID);
void DomeOperator::AcceptClientLogon(tagLogon& logon, BYTE	LogonClientID, ServerClientData* pClient)
{
	OL_Cmd_OperatorLogon msg;
	msg.logon = logon;
	msg.client_id = LogonClientID;
	msg.result = false;
	USERS::iterator it = _server_wait_users.find(logon.AccountName);
	if (it != _server_wait_users.end())
	{
		msg.result = true;
		_server_wait_users.erase(it);
	}
	//msg.result = true;
	SetMsgInfo(msg, GetMsgType(Main_Logon, OL_OperatorLogon), sizeof(msg));
	g_FishServer.SendNetCmdToClient(pClient, &msg);


}

void DomeOperator::Update()
{
	time_t cur_time = time(0);
	USERS::iterator it = _server_wait_users.begin();
	while (it != _server_wait_users.end())
	{
		long time_diff = cur_time - it->second;
		if (time_diff > 30 || time_diff < 0)
		{
			it = _server_wait_users.erase(it);
		}
		else
		{
			++it;
		}
	}

}
