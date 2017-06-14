#include "stdafx.h"
#include "HttpNewServer.h"
#include "..\CommonFile\json/json.h"
#include "FishServer.h"

HttpNewServer::HttpNewServer()
{

	
	//DomeLogin?loginNo=38862603772888081806&userId=bq_000080143&appCode=D0000356
	_HttpCallBacks["DomeLogin"] = &HttpNewServer::DomeLogin;
	_HttpCallBacks["DomeTestLogin"] = &HttpNewServer::DomeTestLogin;
	_HttpCallBacks["DomePay"] = &HttpNewServer::DomePay;
	_HttpCallBacks["DomeTestPay"] = &HttpNewServer::DomeTestPay;
	_HttpCallBacks["FacebookPay"] = &HttpNewServer::FacebookPay;
	//Call("DomePay", "responseCode=1000&errorCode=&errorMsg=&data=%7B%22orderNo%22%3A%2256%22%2C%22sdkflowId%22%3A%22201705031539515146290%22%7D&signCode=qxD9lzq8tJn%2BN%2FJ6TnAbfAbT1e5gbySFoOt1RVhgmx6IbUu8uDsVwwR7zDPozMmgVjBMaBTg9AEffRWy7GpFe19tuPsT1SGQrIlqUs7nrOCeoHRBKvdeinj21RNHGnxvcEew138MG4QtkbWcdBqqapmPdXbiMITvEufQlYT9jlM%3D", NULL);
	//std::string source = "loginNo = 81646809760111260474 & userId = bq_000080143&appCode = D0000356";
	//std::map<std::string, std::string> map_entry;
	//DomeArguHelp(source.c_str(), map_entry);


	//int c = 0;
	//c++;


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


void HttpNewServer::Call(const char* fun, const char* data, HttpClientData* c)
{
	MAP_CALL_BACKS::iterator it = _HttpCallBacks.find(fun);
	if (it != _HttpCallBacks.end())
	{
		Fun call = it->second;
		(this->*call)(data, c);
	}
	else
	{
		char strTime[128] = { 0 };

		GetGMTTimeStr(strTime, sizeof(strTime));
		SendResponse(c, strTime, "Failed");
	}
}

void HttpNewServer::Call(const char* data, HttpClientData* c)
{

	LogInfoToFile("Log.txt", "http 接收到数据 %s", data);
	std::string entry = data;
	int index = entry.find('?');
	int count = entry.size();
	if (index != std::string::npos)
	{
		count = index;
	}
	std::string call_fuc = entry.substr(0, count);
	std::string argument = "";
	if (count != entry.size())
	{
		argument = entry.substr(count + 1, entry.size());
	}
	Call(call_fuc.c_str(), argument.c_str(), c);
}

void HttpNewServer::NormalCall(HttpClientData *pc, const char* Time, bool post)
{

	if (post)
	{
		Call(pc->RequestUrl, pc->Buff, pc);
		
	}
	else
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
		else
		{
			char strTime[128] = { 0 };

			GetGMTTimeStr(strTime, sizeof(strTime));
			LogInfoToFile("LogError.txt", "http NormalCall 接收到数据 %s 解析失败", resource.c_str());
			SendResponse(pc, strTime, "Failed");
		}
	}

}


void HttpNewServer::DomeArguHelp(const char* data, std::map<std::string, std::string>& map)
{
	std::string str_data = data;

	int begin = 0;

	begin = str_data.find(" ", begin);  //查找空格在str中第一次出现的位置

	while (begin != -1)  //表示字符串中存在空格

	{

		str_data.replace(begin, 1, "");  // 用空串替换str中从begin开始的1个字符

		begin = str_data.find(" ", begin);  //查找空格在替换后的str中第一次出现的位置

	}
	//str_data.erase(0, str_data.find_first_not_of(" "));
	//str_data.erase(str_data.find_last_not_of(" ") + 1);
	std::string delim = "&";
	std::string delim_1 = "=";
	std::vector<std::string> strArray;
	split(str_data, delim, &strArray);
	std::vector<std::string> strArray_1;
	std::vector<std::string>::iterator  it = strArray.begin();
	for (; it != strArray.end(); ++it)
	{
		std::string entry = (*it);
		strArray_1.clear();
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
	std::string UrlStr = UrlDecode(data);

	LogInfoToFile("DomePay.txt", "普通充值异步回调: %s", UrlStr.c_str());
	size_t BeginIndex = UrlStr.find_first_of("{");
	size_t EndIndex = UrlStr.find_first_of("}");
	if (BeginIndex == -1 || EndIndex == -1 || EndIndex <= BeginIndex)
	{
		ASSERT(false);
		LogInfoToFile("DomePay.txt", "普通充值异步回调 内容错误");
		return;
	}
	string transdata = UrlStr.substr(BeginIndex, EndIndex - BeginIndex + 1);
	if (transdata.length() == 0)
	{
		LogInfoToFile("DomePay.txt", "普通充值异步回调 具体数据为空");
		ASSERT(false);
		return;
	}
	std::string delim = "&";
	std::string delim1 = "=";
	std::vector<std::string> strArray;
	std::vector<std::string> strArray1;
	split(UrlStr, delim, &strArray);
	std::vector<std::string>::iterator it = strArray.begin();

	std::map<std::string, std::string> map_argu;
	for (; it != strArray.end(); ++ it)
	{
		strArray1.clear();
		split(*it, delim1, &strArray1);
		std::string name = strArray1[0];
		if (strArray1.size() > 0)
		{
			map_argu[name] = "";
		}
		if (strArray1.size() == 2)
		{
			map_argu[name] = strArray1[1];
		}
	}

	bool successful = false;
	
	if (map_argu["errorCode"] != "")
	{
		LogInfoToFile("DomePay.txt", "普通充值异步回调 支付失败 data数据[%s]  error code[%s]", data,   map_argu["errorCode"].c_str());
	}
	else
	{
		if (map_argu["data"] != "")
		{
			std::string strJosnInfo = map_argu["data"];
			LogInfoToFile("DomePay.txt", "普通充值异步回调 data数据[%s] ", strJosnInfo.c_str());
			Json::Reader jsonReader;
			Json::Value jsonRoot;

			if (!jsonReader.parse(strJosnInfo, jsonRoot))
			{
				ASSERT(false);
				return;
			}
			if (!jsonRoot["orderNo"].isNull() && !jsonRoot["sdkflowId"].isNull())
			{
				int order_id = atoi(jsonRoot["orderNo"].asString().c_str());
				int sdk_flow_id = atoi(jsonRoot["sdkflowId"].asString().c_str());// jsonRoot["sdkflowId"].asUInt();
				DBR_Cmd_Deal_Third_Platform_Verify msg;
				msg.Order_id = order_id;
				msg.channel_type = Dome_ChannelType;
				SetMsgInfo(msg, DBR_Deal_Third_Platform_Verify, sizeof(msg));
				g_FishServer.SendNetCmdToDB(&msg);
				successful = true;
				DBR_Cmd_Deal_ThirdPlatform_Verify_Log msg_log;
				msg_log.OrderID = order_id;
				msg_log.ChannelID = Dome_ChannelType;
				msg_log.SDKFlowID = sdk_flow_id;
				SetMsgInfo(msg_log, DBR_Deal_ThirdPlatform_Verify_Log, sizeof(msg_log));
				g_FishServer.SendNetCmdToLogDB(&msg_log);
			}
		}
	}


	char strTime[128] = { 0 };

	char sz_send[1024];
	std::string ret = "";
	if (successful)
	{
		ret = "{\"isSuccess\":\"true\"}";
	}
	else
	{
		ret = "{\"isSuccess\":\"false\"}";

	}
	GetGMTTimeStr(strTime, sizeof(strTime));

	int nret = sprintf_s(sz_send, sizeof(sz_send), "HTTP/1.1 200 OK\r\nServer: vata/0.0.1\r\n\r\n%s", ret.c_str());
	int count = send(c->Socket, sz_send, nret, 0);


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
	LogInfoToFile("HttpDomeLoginLog.txt", "http DomeLogin 接收到数据 %s", data);
	std::map<std::string, std::string> map_argu;
	DomeArguHelp(data, map_argu);
	std::string user_id = map_argu["userId"];
	std::string app_code = map_argu["appCode"];
	std::string login_No = map_argu["loginNo"];
	UINT count = 0;
	WCHAR* entry = CharToWChar(user_id.c_str(), count);
	g_FishServer.GetOperatorHelper().AcceptDomeServerLogon(entry);
	char strTime[128] = { 0 };
	GetGMTTimeStr(strTime, sizeof(strTime));
	//SendResponse(c, strTime, "isSuccess=true");
	delete[] entry;
}

struct FaceBookTag
{
	enum FaceBookType
	{
		FaceBookType_unknow,
		FaceBookType_charge,
		FaceBookType_refund
	};
	enum FaceBookState
	{
		FaceBookState_unknow,
		FaceBookState_completed,
		FaceBookState_initiated
	};
	enum FaceBookCurrencyType
	{
		FaceBookCurrencyType_unknow,
		FaceBookCurrencyType_USD
	};
	FaceBookType Type_;
	FaceBookState State_;
	FaceBookCurrencyType CurrencyType_;
	int amount;

	FaceBookTag()
	{
		Type_ = FaceBookType_unknow;
		State_ = FaceBookState_unknow;
		CurrencyType_ = FaceBookCurrencyType_unknow;
		amount = 0;
	}
};

struct FaceBookItemTag
{
	std::string product_;
	int quantity_;
};
void HttpNewServer::FacebookPay(const char* data, HttpClientData* c)
{
	LogInfoToFile("FacebookPayLog.txt", "http FacebookPay 接收到数据 %s", data);
	Json::Reader jsonReader;
	Json::Value jsonRoot;

	if (!jsonReader.parse(data, jsonRoot))
	{
		ASSERT(false);
		return;
	}
	if (!jsonRoot["id"].isNull() && !jsonRoot["user"].isNull())
	{
		long global_id = jsonRoot["id"].asUInt();
		long user_id = 0;
		Json::Value JsonValue = jsonRoot["user"];
		std::string name;
		if (!JsonValue["name"].isNull() && !JsonValue["id"].isNull())
		{
			user_id = JsonValue["id"].asUInt();
			name = JsonValue["name"].asString();
		}
		
		int length = 0;
		if (!jsonRoot["actions"].isNull() && !jsonRoot["items"].isNull())
		{
			std::vector<FaceBookItemTag> VcFaceBookItemTag;
			std::vector<FaceBookTag> VcFaceBookTag;
			Json::Value actionsJsonValue = jsonRoot["actions"];
			Json::Value itemsJsonValue = jsonRoot["items"];
			if (actionsJsonValue.isArray())
			{
				length = actionsJsonValue.size();
				for (size_t i = 0; i < length; i++)
				{				
					FaceBookTag tag_entry;
					Json::Value JsonAction = actionsJsonValue[i];
					if (JsonAction["type"].isString() && JsonAction["status"].isString() && JsonAction["currency"].isString() && JsonAction["amount"].isNull() && JsonAction["time_created"].isString()
						&& JsonAction["time_created"].isString())
					{
						if (JsonAction["type"].asString() == "charge")
						{
							tag_entry.Type_ = FaceBookTag::FaceBookType_charge;
						}
						else if (JsonAction["type"].asString() == "refund")
						{
							tag_entry.Type_ = FaceBookTag::FaceBookType_refund;
						}
						if (JsonAction["status"].asString() == "completed")
						{
							tag_entry.State_ = FaceBookTag::FaceBookState_completed;
						}
						else if (JsonAction["status"].asString() == "initiated")
						{
							tag_entry.State_ = FaceBookTag::FaceBookState_initiated;
						}

						if (JsonAction["currency"].asString() == "USD")
						{
							tag_entry.CurrencyType_ = FaceBookTag::FaceBookCurrencyType_USD;
						}

						float amount_entry =  atof(JsonAction["amount"].asString().c_str());
						tag_entry.amount = amount_entry * 100;
						VcFaceBookTag.push_back(tag_entry);
					}

				}
				if (itemsJsonValue.isArray())
				{
					length = itemsJsonValue.size();
					for (size_t i = 0; i < length; i++)
					{
						Json::Value JsonItem = itemsJsonValue[i];
						if (JsonItem["product"].isString() && JsonItem["quantity"].isInt())
						{
							FaceBookItemTag entry;
							entry.product_ = JsonItem["product"].asString();
							entry.quantity_ = JsonItem["quantity"].asInt();
							VcFaceBookItemTag.push_back(entry);
						}

					}
				}
				
				
				bool find_ret = false;
				std::vector<FaceBookTag>::iterator it = VcFaceBookTag.begin();
				for (; it != VcFaceBookTag.end(); ++ it)
				{
					FaceBookTag entryItem = (*it);
					if (entryItem.CurrencyType_ == FaceBookTag::FaceBookState_initiated)
					{
						std::vector<FaceBookItemTag>::iterator it_item = VcFaceBookItemTag.begin();
						for (; it_item != VcFaceBookItemTag.end(); ++ it_item)
						{
							FaceBookItemTag entry = (*it_item);
							find_ret = true;
							if (entry.product_.size() < MAX_PayNOLength)
							{

								WCHAR sz_temp_payNO[MAX_PayNOLength];
								UINT count_temp = 0;
								WCHAR* temp_PayNO =  CharToWChar(entry.product_.c_str(), count_temp);
								int shop_id = 0;
								
								TCHARCopy(sz_temp_payNO, CountArray(sz_temp_payNO), temp_PayNO, _tcslen(temp_PayNO));
								free(temp_PayNO);
								HashMap<DWORD, tagFishRechargeInfo>::iterator it = g_FishServer.GetFishConfig().GetFishRechargesConfig().m_FishRechargeMap.begin();
								for (; it != g_FishServer.GetFishConfig().GetFishRechargesConfig().m_FishRechargeMap.end(); ++ it)
								{
									const tagFishRechargeInfo& config_entry = it->second;
									HashMap<WORD, tagFishPayType>::const_iterator it_pay_type = config_entry.PayNO.find(FaceBook_ChannelPayType);
									if (it_pay_type != config_entry.PayNO.end())
									{
										if (wcscmp(it_pay_type->second.PayNO, sz_temp_payNO) == 0)
										{
											shop_id = config_entry.ID;
											break;
										}
									}
									
								}
								if (shop_id != 0)
								{
									DBR_Cmd_Deal_Third_Platform_Create_And_Verify msg;
									msg.channel_type = Facebook_ChannelType;
									msg.shop_id = shop_id;
									if (name.size() < 256)
									{
										strcpy_s(msg.account_name, name.c_str());
									}
									
									msg.pay_type = FaceBook_ChannelPayType;
									strcpy_s(msg.good_id, entry.product_.c_str());
									SetMsgInfo(msg, DBR_Deal_Third_Platform_Create_And_Verify, sizeof(msg));
									g_FishServer.SendNetCmdToDB(&msg);
								}
								else
								{

								}
								
							}
							else
							{

							}
							

							break;
						}
						break;
					}
				}
			}
		
		}

		if (!jsonRoot["disputes"].isNull())
		{
			if (!jsonRoot["disputes"].isArray())
			{
				Json::Value JsonDisputesArray = jsonRoot["disputes"];
				length = JsonDisputesArray.size();
				for (size_t i = 0; i < length; i++)
				{
					Json::Value JsonDisputes = JsonDisputesArray[i];
					if (JsonDisputes["user_comment"].isString() && JsonDisputes["time_created"].isString() && JsonDisputes["user_email"].isString()
						&& JsonDisputes["status"].isString() && JsonDisputes["reason"].isString())
					{
						std::string user_comment = JsonDisputes["user_comment"].asString();
						std::string time_created = JsonDisputes["time_created"].asString();
						std::string user_email = JsonDisputes["user_email"].asString();
						std::string status = JsonDisputes["status"].asString();
						std::string reason = JsonDisputes["reason"].asString();
					}					
				}
			}
		}
	}

	char strTime[128] = { 0 };

	char sz_send[1024];
	std::string ret = "";
	
	
	ret = "{\"isSuccess\":\"false\"}";


	GetGMTTimeStr(strTime, sizeof(strTime));

	int nret = sprintf_s(sz_send, sizeof(sz_send), "HTTP/1.1 200 OK\r\nServer: vata/0.0.1\r\n\r\n%s", ret.c_str());
	int count = send(c->Socket, sz_send, nret, 0);

}
