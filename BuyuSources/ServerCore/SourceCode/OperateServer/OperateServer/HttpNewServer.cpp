#include "stdafx.h"
#include "HttpNewServer.h"
#include "..\CommonFile\json/json.h"
#include "FishServer.h"

HttpNewServer::HttpNewServer()
{

	_HttpCallBacks["DomeLogin"] = &HttpNewServer::DomeLogin;
	_HttpCallBacks["DomeTestLogin"] = &HttpNewServer::DomeTestLogin;
	_HttpCallBacks["DomePay"] = &HttpNewServer::DomePay;
	_HttpCallBacks["DomeTestPay"] = &HttpNewServer::DomeTestPay;
}


HttpNewServer::~HttpNewServer()
{
}
void split(std::string& s, std::string& delim, std::vector< std::string >* ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		ret->push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last>0)
	{
		ret->push_back(s.substr(last, index - last));
	}
}

void HttpNewServer::Call(const char* data, HttpClientData* c)
{
	std::string entry = data;
	int index = entry.find('//');
	int count = entry.size();
	if (index != std::string::npos)
	{
		count = index;
	}
	std::string call_fuc = entry.substr(0, count);
	std::string argument = "";
	if (count != entry.size())
	{
		argument = entry.substr(1,count - 1);
	}
	MAP_CALL_BACKS::iterator it = _HttpCallBacks.find(call_fuc);
	if (it != _HttpCallBacks.end())
	{
		Fun call = it->second;		
		(this->*call)(argument.c_str(), c);
	}

}

void HttpNewServer::NormalCall(HttpClientData *pc, const char* Time)
{
	std::string  resource = pc->Buff;
	std::vector<std::string> strArray;
	std::string delim = "HTTP";
	std::string delim1 = " ";
	
	int index = resource.find("GET ");
	int index_end = resource.find(" HTTP");
	if (index != std::string::npos && index_end != std::string::npos)
	{
		std::string entry = resource.substr(index + 5, index_end - (index + 4));
		Call(entry.c_str(), pc);
	}	

}


void HttpNewServer::DomeArguHelp(const char* data, std::map<std::string, std::string>& map)
{
	std::string str_data = data;
	std::string delim = "&";
	std::string delim_1 = "=";
	std::vector<std::string> strArray;
	split(str_data, delim, &strArray);
	std::vector<std::string> strArray_1;
	std::vector<std::string>::iterator  it = strArray.begin();
	for (; it != strArray.end(); ++it)
	{
		std::string entry = (*it);

		split(entry, delim_1, &strArray_1);
		if (strArray_1.size() > 0)
		{
			map[strArray_1[0]] = "";
			if (strArray_1.size() == 2)
			{
				map[strArray_1[0]] = strArray_1[1];
			}

		}

	}

}

void HttpNewServer::DomePay(const char* data, HttpClientData* c)
{
	//userId=xxx&appCode=xxx&loginNo=xxx
	std::map<std::string, std::string> map_argu;
	DomeArguHelp(data, map_argu);	
	std::string responseCode = map_argu["responseCode"];
	std::string errorCode = map_argu["errorCode"];
	std::string errorMsg = map_argu["errorMsg"];
	std::string data_temp = map_argu["data"];
	std::string signCode = map_argu["signCode"];
}
void HttpNewServer::DomeTestPay(const char* data, HttpClientData* c)
{
	std::map<std::string, std::string> map_argu;
	DomeArguHelp(data, map_argu);
	std::string responseCode = map_argu["responseCode"];
	std::string errorCode = map_argu["errorCode"];
	std::string errorMsg = map_argu["errorMsg"];
	std::string data_temp = map_argu["data"];
	std::string signCode = map_argu["signCode"];
}
void HttpNewServer::DomeTestLogin(const char* data, HttpClientData* c)
{
	std::map<std::string, std::string> map_argu;
	DomeArguHelp(data, map_argu);
	std::string user_id = map_argu["userId"];
	std::string app_code = map_argu["appCode"];
	std::string login_No = map_argu["loginNo"];
	UINT count = 0;
	WCHAR* entry =  CharToWChar(user_id.c_str(), count);
	g_FishServer.GetOperatorHelper().AcceptDomeServerLogon(entry);
	delete[] entry;
}
void HttpNewServer::DomeLogin(const char* data, HttpClientData* c)
{
	std::map<std::string, std::string> map_argu;
	DomeArguHelp(data, map_argu);
	std::string user_id = map_argu["userId"];
	std::string app_code = map_argu["appCode"];
	std::string login_No = map_argu["loginNo"];
	UINT count = 0;
	WCHAR* entry = CharToWChar(user_id.c_str(), count);
	g_FishServer.GetOperatorHelper().AcceptDomeServerLogon(entry);
	delete[] entry;
}
