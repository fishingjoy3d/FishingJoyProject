#include "stdafx.h"
#include "OperatorHelper.h"


OperatorHelper::OperatorHelper()
{
}


OperatorHelper::~OperatorHelper()
{
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
}


DomeOperator::~DomeOperator()
{

}


void DomeOperator::AcceptOperatorServer(TCHAR* user)
{
	CLIENTUSERS::iterator it_client = _client_wait_users.find(user);
	if (it_client != _client_wait_users.end())
	{


	}
	else
	{
		USERS::iterator it = _server_wait_users.find(user);
		if (it != _server_wait_users.end())
		{
			_server_wait_users.erase(it);
		}

		time_t cur_time = time(0);
		_server_wait_users[user] = cur_time;

	}
	
}
void DomeOperator::AcceptClient(tagLogon& logon)
{
	USERS::iterator it = _server_wait_users.find(logon.AccountName);

}

void DomeOperator::Update()
{

}

void DomeOperator::DomeLogonSucessful(tagDomeLogon info)
{

}
