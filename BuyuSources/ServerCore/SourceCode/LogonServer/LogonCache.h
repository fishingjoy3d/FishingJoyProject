#pragma once
class LogonCache
{
public:
	LogonCache();
	virtual ~LogonCache();

	void OnLoadAllAccountData();//从数据库读取全部的数据
	void OnLoadAllAccountDataResult(DBO_Cmd_LoadAllAccountInfo* msg);//从数据库加载的结果
	//void OnLoadAllAccountDataResultFinish();
	bool CheckAccountData(const TCHAR* AccountName, DWORD PasswordCrc1, DWORD PasswordCrc2, DWORD PasswordCrc3, DWORD& UserID, bool& IsFreeze,__int64& FreezeEndTime);
	bool IsExistsAccount(const TCHAR* AccountName);
	void OnAccountRsg(AccountCacheInfo& pData);//当Logon接收到一个玩家注册的时候 

	//缓存数据
	void OnAddTempAccountInfo(TempAccountCacheInfo& pData);//将未验证的数据放入缓存之中
	void OnCheckTempAccountInfo(DWORD ClientID, DWORD dwUserID, bool  Result);//删除还是转化为正式的缓存数据
	void OnCheckTempAccountInfo(DWORD ClientID,const TCHAR* AccountName, DWORD dwUserID, bool Result);

	void OnChangeAccountPassword(DWORD dwUserID, DWORD Pass1, DWORD Pass2, DWORD Pass3);
	void OnChangeAccountFreezeInfo(DWORD dwUserID, time_t FreezeEndTime);
	void OnResetAccount(DWORD dwUserID, const TCHAR* AccountName, DWORD Pass1, DWORD Pass2, DWORD Pass3);
	void OnAddAccountInfo(DWORD dwUserID, const TCHAR* AccountName, DWORD Pass1, DWORD Pass2, DWORD Pass3);
private :
	DWORD GetAccountNameCrc(const TCHAR* pAccountName);
	void OnDestroy();
private:
	HashMap<DWORD, AccountCacheInfo*>	m_AccountMap;
	HashMap<DWORD, AccountCacheInfo*>   m_UserMap;

	HashMap<DWORD, TempAccountCacheInfo>	m_TempAccountMap;//缓存的数据 //客户端端的CLientID
};