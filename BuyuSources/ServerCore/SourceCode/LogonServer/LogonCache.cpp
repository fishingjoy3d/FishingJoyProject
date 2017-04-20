#include "stdafx.h"
#include "LogonCache.h"
#include "FishServer.h"
LogonCache::LogonCache()
{
	m_AccountMap.clear();
	m_TempAccountMap.clear();
	m_UserMap.clear();
}
LogonCache::~LogonCache()
{
	m_TempAccountMap.clear();
	OnDestroy();
}
void LogonCache::OnLoadAllAccountData()
{
	//发送命令到数据库去
	DBR_Cmd_LoadAllAccountInfo msg;
	SetMsgInfo(msg, DBR_LoadAllAccountInfo, sizeof(DBR_Cmd_LoadAllAccountInfo));
	g_FishServer.SendNetCmdToDB(&msg);
	m_AccountMap.clear();
}
void LogonCache::OnLoadAllAccountDataResult(DBO_Cmd_LoadAllAccountInfo* msg)
{
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	if ((msg->States & MsgBegin) != 0)
	{
		m_AccountMap.clear();
		m_UserMap.clear();
	}
	for (WORD i = 0; i < msg->Sum; ++i)
	{
		DWORD AccountCrc = GetAccountNameCrc(msg->Array[i].AccountName);
		AccountCacheInfo* pInfo = new AccountCacheInfo(msg->Array[i]);
		m_AccountMap.insert(HashMap<DWORD, AccountCacheInfo*>::value_type(AccountCrc, pInfo));
		m_UserMap.insert(HashMap<DWORD, AccountCacheInfo*>::value_type(pInfo->dwUserID, pInfo));
	}
	if ((msg->States & MsgEnd) != 0)
	{
		g_FishServer.OnLogonDBDataSucess();
	}
}
DWORD LogonCache::GetAccountNameCrc(const TCHAR* pAccountName)
{
	if (!pAccountName)
		return 0;
	TCHAR AccountName[ACCOUNT_LENGTH +1];
	int len = _tcslen(pAccountName);
	for (int i = 0; i < len; ++i)
	{
		AccountName[i] = ::towlower(pAccountName[i]);
	}
	//memcpy_s(AccountName, (ACCOUNT_LENGTH + 1)* sizeof(TCHAR), pAccountName, len * sizeof(TCHAR));
	return AECrc32(AccountName, sizeof(TCHAR) * len, 0);
}
//void LogonCache::OnLoadAllAccountDataResultFinish()
//{
//	//提示 所有的账号数据全部加载完毕了
//	
//}
bool LogonCache::IsExistsAccount(const TCHAR* AccountName)
{
	if (!AccountName)
	{
		ASSERT(false);
		return true;
	}
	DWORD AccountCrc = GetAccountNameCrc(AccountName);
	HashMap<DWORD, AccountCacheInfo*>::iterator Iter = m_AccountMap.find(AccountCrc);
	if (Iter == m_AccountMap.end())
		return false;
	else
	{
		Log(L"存在相同的账号名:%s", AccountName);
		return true;
	}
}
bool LogonCache::CheckAccountData(const TCHAR* AccountName, DWORD PasswordCrc1, DWORD PasswordCrc2, DWORD PasswordCrc3, DWORD& UserID, bool& IsFreeze, __int64& FreezeEndTime)
{
	DWORD AccountCrc = GetAccountNameCrc(AccountName);
	HashMap<DWORD, AccountCacheInfo*>::iterator Iter = m_AccountMap.find(AccountCrc);
	if (Iter == m_AccountMap.end())
	{
		FreezeEndTime = 0;
		IsFreeze = false;
		UserID = 0;
		return false;
	}
	if (Iter->second->PasswordCrc1 == PasswordCrc1 && Iter->second->PasswordCrc2 == PasswordCrc2 && Iter->second->PasswordCrc3 == PasswordCrc3)
	{
		time_t pNow = time(null);
		IsFreeze = (Iter->second->IsFreeze && Iter->second->FreezeEndTime > pNow);
		UserID = Iter->second->dwUserID;
		FreezeEndTime = Iter->second->FreezeEndTime;
		return true;
	}
	else
	{
		FreezeEndTime = 0;
		IsFreeze = false;
		UserID = 0;
		return true;
	}
}
void LogonCache::OnAccountRsg(AccountCacheInfo& pData)
{
	DWORD AccountCrc = GetAccountNameCrc(pData.AccountName);
	HashMap<DWORD, AccountCacheInfo*>::iterator Iter = m_AccountMap.find(AccountCrc);
	if (Iter != m_AccountMap.end())
	{
		ASSERT(false);
		return;
	}
	AccountCacheInfo* pInfo = new AccountCacheInfo(pData);
	m_AccountMap.insert(HashMap<DWORD, AccountCacheInfo*>::value_type(AccountCrc, pInfo));
	m_UserMap.insert(HashMap<DWORD, AccountCacheInfo*>::value_type(pInfo->dwUserID, pInfo));
}
void LogonCache::OnAddTempAccountInfo(TempAccountCacheInfo& pData)
{
	HashMap<DWORD, TempAccountCacheInfo>::iterator Iter= m_TempAccountMap.find(pData.ClientID);
	if (Iter != m_TempAccountMap.end())
	{
		ASSERT(false);
		return;
	}
	m_TempAccountMap.insert(HashMap<DWORD, TempAccountCacheInfo>::value_type(pData.ClientID,pData));
}
void LogonCache::OnCheckTempAccountInfo(DWORD ClientID, DWORD dwUserID, bool Result)
{
	HashMap<DWORD, TempAccountCacheInfo>::iterator Iter = m_TempAccountMap.find(ClientID);
	if (Iter == m_TempAccountMap.end())
	{
		ASSERT(false);
		return;
	}
	if (Result)
	{
		Iter->second.AccountInfo.dwUserID = dwUserID;
		OnAccountRsg(Iter->second.AccountInfo);//添加
		m_TempAccountMap.erase(Iter);
	}
	else
	{
		m_TempAccountMap.erase(Iter);
	}
}
void LogonCache::OnCheckTempAccountInfo(DWORD ClientID, const TCHAR* AccountName, DWORD dwUserID, bool Result)
{
	HashMap<DWORD, TempAccountCacheInfo>::iterator Iter = m_TempAccountMap.find(ClientID);
	if (Iter == m_TempAccountMap.end())
	{
		ASSERT(false);
		return;
	}
	if (Result)
	{
		TCHARCopy(Iter->second.AccountInfo.AccountName, CountArray(Iter->second.AccountInfo.AccountName), AccountName, _tcslen(AccountName));
		Iter->second.AccountInfo.dwUserID = dwUserID;
		OnAccountRsg(Iter->second.AccountInfo);//添加
		m_TempAccountMap.erase(Iter);
	}
	else
	{
		m_TempAccountMap.erase(Iter);
	}

}
void LogonCache::OnChangeAccountPassword(DWORD dwUserID, DWORD Pass1, DWORD Pass2, DWORD Pass3)
{
	HashMap<DWORD, AccountCacheInfo*>::iterator Iter = m_UserMap.find(dwUserID);
	if (Iter == m_UserMap.end())
	{
		ASSERT(false);
		return;
	}
	Iter->second->PasswordCrc1 = Pass1;
	Iter->second->PasswordCrc2 = Pass2;
	Iter->second->PasswordCrc3 = Pass3;
}
void LogonCache::OnChangeAccountFreezeInfo(DWORD dwUserID, time_t FreezeEndTime)
{
	HashMap<DWORD, AccountCacheInfo*>::iterator Iter = m_UserMap.find(dwUserID);
	if (Iter == m_UserMap.end())
	{
		ASSERT(false);
		return;
	}
	Iter->second->FreezeEndTime = FreezeEndTime;
	Iter->second->IsFreeze = true;
}
void LogonCache::OnResetAccount(DWORD dwUserID, const TCHAR* AccountName, DWORD Pass1, DWORD Pass2, DWORD Pass3)
{
	if (!AccountName)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, AccountCacheInfo*>::iterator Iter = m_UserMap.find(dwUserID);
	if (Iter == m_UserMap.end())
	{
		ASSERT(false);
		return;
	}
	DWORD OldAccountCrc = GetAccountNameCrc(Iter->second->AccountName);
	m_AccountMap.erase(OldAccountCrc);

	TCHARCopy(Iter->second->AccountName, CountArray(Iter->second->AccountName), AccountName, _tcslen(AccountName));
	Iter->second->PasswordCrc1 = Pass1;
	Iter->second->PasswordCrc2 = Pass2;
	Iter->second->PasswordCrc3 = Pass3;
	//修改账号索引
	DWORD AccountCrc = GetAccountNameCrc(Iter->second->AccountName);
	m_AccountMap.insert(HashMap<DWORD, AccountCacheInfo*>::value_type(AccountCrc, Iter->second));
}
void LogonCache::OnAddAccountInfo(DWORD dwUserID, const TCHAR* AccountName, DWORD Pass1, DWORD Pass2, DWORD Pass3)
{
	DWORD AccountCrc = GetAccountNameCrc(AccountName);
	HashMap<DWORD, AccountCacheInfo*>::iterator Iter = m_AccountMap.find(AccountCrc);
	if (Iter != m_AccountMap.end())
	{
		ASSERT(false);
		return;
	}
	Iter = m_UserMap.find(dwUserID);
	if (Iter != m_UserMap.end())
	{
		ASSERT(false);
		return;
	}
	AccountCacheInfo* pInfo = new AccountCacheInfo();
	TCHARCopy(pInfo->AccountName, CountArray(pInfo->AccountName), AccountName, _tcslen(AccountName));
	pInfo->dwUserID = dwUserID;
	pInfo->PasswordCrc1 = Pass1;
	pInfo->PasswordCrc2 = Pass2;
	pInfo->PasswordCrc3 = Pass3;
	m_AccountMap.insert(HashMap<DWORD, AccountCacheInfo*>::value_type(AccountCrc, pInfo));
	m_UserMap.insert(HashMap<DWORD, AccountCacheInfo*>::value_type(pInfo->dwUserID, pInfo));
}
void LogonCache::OnDestroy()
{
	HashMap<DWORD, AccountCacheInfo*>::iterator Iter = m_AccountMap.begin();
	for (; Iter != m_AccountMap.end(); ++Iter)
	{
		delete Iter->second;
	}
	m_AccountMap.clear();
	m_UserMap.clear();
}