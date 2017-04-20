#pragma once



class GMToolManager
{
public:
	GMToolManager();
	virtual ~GMToolManager();
public:
	bool Init();
	void SendLoginReq(const char* Account, const char* PassWord);
	void KickUser(DWORD UserID, int FrozenTime);
	void SendQueryUser(QueryUserType en, CString NickName);
	void Update();
	bool HandleControlMsg(NetCmd* pCmd);
	void ConnectControl();
	void SendNetCmdToControl(NetCmd*pCmd);
	void StringToChar(const CString s, char *Destination);
	void Notice(CString n);
	void SendSystemMail(DWORD UserID, CString Context, int RewardID, int RewardSum);
	void SendSystemOperatorMail(CString Context, int RewardID, int RewardSum, DWORD BeginTime, DWORD EndTime);
	void SendMsgToAllGame(CString Context, int Sum, int Sec, int param, DWORD color);
	const tagItemConfig* GetItemConfig(DWORD ItemID);
	char *w2c(char *pcstr, const wchar_t *pwstr, size_t len);



	void utf8ToUnicode(const string& src, wstring& result);
	std::map<DWORD, tagRoleInfo>* GetRoleList();
	std::map<DWORD, tagRewardOnce>* GetRewards();
protected:
	void ReqLoadRewardConfig();
	void ReqLoadItemConfig();
protected:
	void OnTcpClientConnect(TCPClient* pClient);
	void OnTcpClientLeave(TCPClient* pClient);
public:
	TCPClient _ControlTcp;
	int _GMToolNetWorkID;
	bool m_ControlIsConnect;
	ControlServerConfig m_ControlServerConfig;
	bool _login_successful;
	std::map<DWORD, tagRoleInfo> _RoleList;
	std::map<DWORD, tagRewardOnce> _Rewards;
	std::map<DWORD, tagItemConfig> _ItemConfigs;
};




