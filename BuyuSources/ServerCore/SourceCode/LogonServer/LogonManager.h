#pragma once
#include <set>
struct UserLogonTempInfo
{
	HashMap<DWORD, BYTE>  m_UserTempSum;
	HashMap<BYTE, WORD>  m_TampGameServerSum;

	void OnUserTempLogon(DWORD dwUserID, BYTE GameConfigID)
	{
		HashMap<DWORD, BYTE>::iterator Iter = m_UserTempSum.find(dwUserID);
		if (Iter != m_UserTempSum.end())
		{
			//ASSERT(false);
			return;
		}
		m_UserTempSum.insert(HashMap<DWORD, BYTE>::value_type(dwUserID, GameConfigID));
		HashMap<BYTE, WORD>::iterator  IterGame = m_TampGameServerSum.find(GameConfigID);
		if (IterGame == m_TampGameServerSum.end())
		{
			m_TampGameServerSum.insert(HashMap<BYTE, WORD>::value_type(GameConfigID, 1));
		}
		else
		{
			IterGame->second += 1;
		}
	}
	void OnUserLogon(DWORD dwUserID)
	{
		//因为玩家不一定是都走Logon 可能走Game登陆
		HashMap<DWORD, BYTE>::iterator Iter = m_UserTempSum.find(dwUserID);
		if (Iter == m_UserTempSum.end())
		{
			//ASSERT(false);
			return;
		}
		HashMap<BYTE, WORD>::iterator Iter2 = m_TampGameServerSum.find(Iter->second);
		if (Iter2 == m_TampGameServerSum.end())
		{
			ASSERT(false);
			m_UserTempSum.erase(Iter);
			return;
		}
		else
		{
			if (Iter2->second < 1)
			{
				ASSERT(false);
				m_UserTempSum.erase(Iter);
				return;
			}
			Iter2->second -= 1;
		}
		m_UserTempSum.erase(Iter);
	}
	WORD GetGameTempLogonUserSum(BYTE GameConfigID)
	{
		HashMap<BYTE, WORD>::iterator Iter = m_TampGameServerSum.find(GameConfigID);
		if (Iter == m_TampGameServerSum.end())
			return 0;
		else
			return Iter->second;
	}
	BYTE GetUserTempGameConfigID(DWORD dwUserID)
	{
		HashMap<DWORD, BYTE>::iterator Iter = m_UserTempSum.find(dwUserID);
		if (Iter == m_UserTempSum.end())
			return 0;
		else
			return Iter->second;
	}
 };
class LogonManager
{
public:
	LogonManager();
	virtual ~LogonManager();

	void OnLogonRsgToCenter();
	void OnLogonRsgResult(CL_Cmd_RsgLogon* pMsg);
	void OnLogonLeaveCenter();
	
	void OnGameRsgLogon(BYTE GameConfigID);
	void OnGameLeaveCenter(BYTE GameConfigID);
	void OnGameLeaveLogon(BYTE GameSocketID);
	void OnGameJoinLogon(BYTE GameSocketID);

	void OnUserJoinCenter(DWORD dwUserID, BYTE GameConfigID);
	void OnUserLeaveCenter(DWORD dwUserID);

	BYTE GetGameServerConfigIDByUserID(DWORD dwUserID);

	DWORD LogonRoleSum(){ return m_UserMap.size(); }
private:
	HashMap<DWORD, BYTE>		m_UserMap;//UserID->GameConfigID;
	HashMap<BYTE, WORD>			m_GamePlayerSum;//GameConfigID->PlayerSum

	HashMap<BYTE,DWORD>			m_GameServerMap;//GameServer 的集合

	UserLogonTempInfo			m_TempLogonInfo;

	DWORD						m_GameIndex;
};