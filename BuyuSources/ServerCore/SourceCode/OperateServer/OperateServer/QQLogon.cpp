#include "stdafx.h"
#include "QQLogon.h"
#include "FishServer.h"
#include "..\CommonFile\FishServerConfig.h"
QQLogon::QQLogon()
{
}
QQLogon::~QQLogon()
{

}
void QQLogon::HandleCode(ServerClientData* pClient, LO_Cmd_QQLogon* pMsg)
{
	if (!pClient || !pMsg)
	{
		ASSERT(false);
		return;
	}
	UINT64 OnlyID = (static_cast<UINT64>(pMsg->ClientID) << 32);
	OnlyID += pClient->OutsideExtraData;
	HashMap<UINT64, tagQQUser>::iterator Iter = m_UserMap.find(OnlyID);
	if (Iter != m_UserMap.end())
	{
		ASSERT(false);
		return;
	}

	//对命令进行解析
	std::vector<char*> pVec;
	GetStringArrayVecByData(pVec, &pMsg->TokenInfo);
	if (pVec.size() != pMsg->TokenInfo.HandleSum)
	{
		ASSERT(false);
		return;
	}

	//处理客户端发送来的数据 转发到微信去 我们获取登陆凭证
	OperateConfig* pConfig = g_FishServerConfig.GetOperateConfig();
	if (!pConfig)
	{
		ASSERT(false);
		return;
	}
	//string QQAppID = pConfig->QQAppID;
	//string QQSecret = pConfig->QQSecret;

	char SignStr[2048];
	sprintf_s(SignStr, CountArray(SignStr), "{3}?access_token=%s",
		pVec[0]);//字符串获取完毕
	//发送出去

	WORD RequestID = g_FishServer.GetQQTokenID();
	if (g_FishServerConfig.GetIsOperateTest())
	{
		//如果为测试
		tagQQUser pUser;
		pUser.OnlyID = OnlyID;
		pUser.ClientID = pMsg->ClientID;
		pUser.LogonClientID = pClient->OutsideExtraData;
		pUser.TimeLog = timeGetTime();
		pUser.PlateFormID = pMsg->PlateFormID;
		TCHARCopy(pUser.MacAddress, CountArray(pUser.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
		m_UserMap.insert(HashMap<UINT64, tagQQUser>::value_type(OnlyID, pUser));
		FreeVec(pVec);

		string strTestUrlResult = "{\"client_id\":\"YOUR_APPID\",\"openid\":\"YOUR_OPENID\"}";
		OnHandleQQTokenResult(OnlyID, (char*)strTestUrlResult.c_str(), strTestUrlResult.length());
		return;
	}
	else
	{
		UINT64* pID = new UINT64(OnlyID);
		if (!g_FishServer.GetHttpClient().AddRequest((UINT_PTR)pID, RequestID, SignStr,true))//直接发送Get请求
		{
			delete(pID);
			return;
		}
	}

	tagQQUser pUser;
	pUser.OnlyID = OnlyID;
	pUser.ClientID = pMsg->ClientID;
	pUser.LogonClientID = pClient->OutsideExtraData;
	pUser.TimeLog = timeGetTime();
	pUser.PlateFormID = pMsg->PlateFormID;
	TCHARCopy(pUser.MacAddress, CountArray(pUser.MacAddress), pMsg->MacAddress, _tcslen(pMsg->MacAddress));
	m_UserMap.insert(HashMap<UINT64, tagQQUser>::value_type(OnlyID, pUser));
	FreeVec(pVec);
	return;
}
void QQLogon::Update()
{
	if (m_UserMap.empty())
		return;
	DWORD NowTime = timeGetTime();
	HashMap<UINT64, tagQQUser>::iterator Iter = m_UserMap.begin();
	for (; Iter != m_UserMap.end();)
	{
		if (NowTime >= Iter->second.TimeLog + 30000)
		{
			ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(Iter->second.LogonClientID);
			if (!pClient)
			{
				Iter = m_UserMap.erase(Iter);
				continue;
			}

			OL_Cmd_QQLogon msg;
			SetMsgInfo(msg, GetMsgType(Main_Logon, OL_QQLogon), sizeof(OL_Cmd_QQLogon));
			msg.PlateFormID = Iter->second.PlateFormID;
			msg.ClientID = Iter->second.ClientID;
			TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), Iter->second.MacAddress, _tcslen(Iter->second.MacAddress));
			msg.Result = false;
			g_FishServer.SendNetCmdToClient(pClient, &msg);

			Iter = m_UserMap.erase(Iter);
		}
		else
			++Iter;
	}
}
void QQLogon::OnHandleQQTokenResult(unsigned __int64 OnlyID, char* pData, DWORD Length)
{
	//返回消息 玩家通过code 获取 token的结果
	if (pData == NULL || Length == 0)
	{
		ASSERT(false);
		return;
	}
	LogInfoToFile("LogonError.txt", "QQ登陆 接收到数据 %s", pData);
	HashMap<UINT64, tagQQUser>::iterator Iter = m_UserMap.find(OnlyID);
	if (Iter == m_UserMap.end())
	{
		ASSERT(false);
		return;
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(Iter->second.LogonClientID);
	if (!pClient)
	{
		ASSERT(false);
		return;
	}

	bool Result = false;
	string OpenID = "";
	if (strstr(pData, "\"error\"") != null)
	{
		//微信登陆失败
		Result = false;
	}
	else if (strstr(pData, "\"openid\"") != null)
	{
		//请求登陆成功了 我们进行处理
		CHAR* pDestChar = strstr(pData, "\"openid\"");
		pDestChar += sizeof("\"openid\"");
		pDestChar = strstr(pDestChar, "\"");
		pDestChar += 1;
		char* pEndChar = strstr(pDestChar, "\"");
		pEndChar[0] = 0;
		OpenID = pDestChar;
		Result = true;
	}
	else
	{
		Result = false;
	}
	if (Result)
	{
		DWORD Length = sizeof(StringArrayDataByChar)+(OpenID.length() - 1)*sizeof(char);
		DWORD PageSize = sizeof(OL_Cmd_QQLogon)-sizeof(BYTE)+Length;
		OL_Cmd_QQLogon* msg = (OL_Cmd_QQLogon*)malloc(PageSize);
		CheckMsgSize(PageSize);
		msg->SetCmdType(GetMsgType(Main_Logon, OL_QQLogon));
		msg->SetCmdSize(static_cast<WORD>(PageSize));
		msg->PlateFormID = Iter->second.PlateFormID;
		msg->ClientID = Iter->second.ClientID;
		TCHARCopy(msg->MacAddress, CountArray(msg->MacAddress), Iter->second.MacAddress, _tcslen(Iter->second.MacAddress));
		DWORD BeginIndex = 0;
		{
			StringArrayDataByChar* pString = (StringArrayDataByChar*)&msg->OpenInfo.Array[BeginIndex];
			pString->Length = ConvertDWORDToBYTE(OpenID.length()*sizeof(char));
			memcpy_s(pString->Array, OpenID.length()*sizeof(char), OpenID.c_str(), OpenID.length()*sizeof(char));
			BeginIndex += sizeof(StringArrayDataByChar)+(OpenID.length() - 1)*sizeof(char);
		}
		msg->Result = true;
		g_FishServer.SendNetCmdToClient(pClient, msg);
		free(msg);
	}
	else
	{
		OL_Cmd_QQLogon msg;
		SetMsgInfo(msg, GetMsgType(Main_Logon, OL_QQLogon), sizeof(OL_Cmd_QQLogon));
		msg.PlateFormID = Iter->second.PlateFormID;
		msg.ClientID = Iter->second.ClientID;
		TCHARCopy(msg.MacAddress, CountArray(msg.MacAddress), Iter->second.MacAddress, _tcslen(Iter->second.MacAddress));
		msg.Result = false;
		g_FishServer.SendNetCmdToClient(pClient, &msg);
	}
}