#pragma once
#include "NewServer.h"
#include <map>



class HttpNewServer :
	public HttpServer
{
public:
	typedef void (HttpNewServer::*Fun)(const char*, HttpClientData*);
	typedef std::map<std::string, Fun> MAP_CALL_BACKS;
	
	HttpNewServer();
	virtual ~HttpNewServer();
protected:
	void NormalCall(HttpClientData *pc, const char* Time);
	void Call(const char* data, HttpClientData* c);
protected:
	void DomeTestLogin(const char* data, HttpClientData* c);
	void DomeLogin(const char* data, HttpClientData* c);
	void DomePay(const char* data, HttpClientData* c);
	void DomeTestPay(const char* data, HttpClientData* c);

	void DomeArguHelp(const char* data, std::map<std::string, std::string>& map);

	MAP_CALL_BACKS _HttpCallBacks;
};

