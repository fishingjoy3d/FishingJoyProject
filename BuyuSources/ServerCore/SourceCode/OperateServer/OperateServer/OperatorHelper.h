#pragma once
#include <map>

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


typedef std::map<ChannelType, BaseOperator*> OPERATORHELPERS;
class OperatorHelper
{
public:
	OperatorHelper();
	virtual ~OperatorHelper();
	DomeOperator* GetDomeOperator();
	void AcceptDomeServerLogon(TCHAR* user);
	void AcceptDomeClientLogon(tagLogon& logon, BYTE LogonClientID, ServerClientData* pClient);
	void Update();
protected:
	OPERATORHELPERS _operators;
	

};

class DomeOperator : public BaseOperator
{

	typedef std::map<std::wstring, DWORD> USERS;
	//typedef std::map<std::wstring, tagDomeLogon> CLIENTUSERS;
public:
	DomeOperator();
	~DomeOperator();
	void AcceptOperatorServerLogon(TCHAR* user);
	void AcceptClientLogon(tagLogon& logon, BYTE LogonClientID, ServerClientData* pClient);
	void Update();
	
protected:	
	USERS _server_wait_users;
	int _wait_sec;
	

};
