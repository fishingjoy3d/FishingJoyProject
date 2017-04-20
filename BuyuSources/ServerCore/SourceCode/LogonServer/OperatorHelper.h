#pragma once
typedef std::map<ChannelType, BaseOperator*> OPERATORHELPERS;
class OperatorHelper
{
public:
	OperatorHelper();
	virtual ~OperatorHelper();
protected:
	OPERATORHELPERS _operators;
	

};

class BaseOperator
{
public:
	BaseOperator();
	~BaseOperator();
	virtual void Update();
	ChannelType GetType() { return _Type; }
protected:
	ChannelType _Type;
};

class DomeOperator : public BaseOperator
{
	struct tagDomeLogon
	{
		tagLogon Logon_;
		DWORD ClientLogonTime_;
	};
	
	typedef std::map<std::wstring, DWORD> USERS;
	typedef std::map<std::wstring, tagDomeLogon> CLIENTUSERS;
public:
	DomeOperator();
	~DomeOperator();
	void AcceptOperatorServer(TCHAR* user);
	void AcceptClient(tagLogon& logon);
	void Update();
	void DomeLogonSucessful(tagDomeLogon info);
protected:	
	USERS _server_wait_users;
	CLIENTUSERS _client_wait_users;

};
