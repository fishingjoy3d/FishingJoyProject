#include "stdafx.h"
#include "RealNameVerificationManager.h"
#include "FishServer.h"
RealNameVerificationManager::RealNameVerificationManager()
{

}
RealNameVerificationManager::~RealNameVerificationManager()
{

}
bool RealNameVerificationManager::CheckEntityID(LPCTSTR EntityID)
{
	SYSTEMTIME pTime;
	GetLocalTime(&pTime);
	DWORD NowYear = pTime.wYear;
	DWORD Length = _tcslen(EntityID);
	if (Length != 15 && Length != 18)
		return false;
	BYTE Array[18];//身份证数组
	for (BYTE i = 0; i < Length; ++i)
		Array[((Length == 15) ? ((i < 6) ? i : (i + 2)) : i)] = ((EntityID[i] == 88 || EntityID[i] == 120) ? 10 : (EntityID[i] - 48));
	//如果为15位  我们想要补充前面的数据
	if (Length == 15)
	{		
		Array[6] = 1u;
		Array[7] = ((Array[11] * 100u + Array[12] * 10u + Array[13]) >= 996u && (Array[8] * 10u + Array[9]) >= ((NowYear - 100u) % 100u) ? 8u : 9u);
		Array[17] = 11u;//11表示无须计算
	}
	//年
	bool CheckYear = ((Array[6] * 10 + Array[7]) < 18 || (Array[6] * 10 + Array[7])>20 || Array[8] >= 10 || Array[9] >= 10);
	bool CheckMonth = (Array[10]>1 || Array[11] >= 10 || (Array[10] * 10 + Array[11]) > 12);
	BYTE Month = (Array[10] * 10 + Array[11]) == 2 ? ((Array[6] * 1000 + Array[7] * 100 + Array[8] * 10 + Array[9] % 4 == 0 || (Array[6] * 1000 + Array[7] * 100 + Array[8] * 10 + Array[9] % 400) == 0) ? 29 : 28)
		: (((Array[10] * 10 + Array[11]) == 4 || (Array[10] * 10 + Array[11]) == 6 || (Array[10] * 10 + Array[11]) == 9 || (Array[10] * 10 + Array[11]) == 11) ? 30 : 31);
	bool CheckDay = (Array[12] > 3 || Array[13] >= 10 || (Array[12] * 10 + Array[13]) > Month);
	bool CheckStates = (Array[11] * 100 + Array[12] * 10 + Array[13] >= 996 && (NowYear - Array[6] * 1000 - Array[7] * 100 - Array[8] * 10 - Array[9]) < 99);
	if (CheckYear || CheckMonth || CheckDay || CheckStates)
		return false;
	if (Array[17] == 11)
		return true;
	DWORD ArrayDest[17] = { 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };
	DWORD TotalSum = 0;
	for (int i = 0; i < 17;++i)
		TotalSum += (Array[i] * ArrayDest[i]);
	DWORD ResultValue[11] = { 1, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2 };
	return (Array[17] == ResultValue[TotalSum % 11]);
}
bool RealNameVerificationManager::OnAddRealNameVerificationOnce(ServerClientData* pClient, GO_Cmd_RealNameVerification* pMsg)
{
	if (!pClient || !pMsg)
	{
		ASSERT(false);
		return false;
	}
	HashMap<DWORD, RealNameVerificationOnce>::iterator Iter = m_RealNameVerificationMap.find(pMsg->dwUserID);//玩家已经提交订单了
	if (Iter != m_RealNameVerificationMap.end())
	{
		ASSERT(false);
		OG_Cmd_RealNameVerification msg;
		SetMsgInfo(msg, GetMsgType(Main_Operate, OG_RealNameVerification), sizeof(OG_Cmd_RealNameVerification));
		msg.dwUserID = pMsg->dwUserID;
		msg.ErrorID = ROE_RealNameVerification_IsExists;
		msg.Result = false;
		TCHARCopy(msg.IDCard, CountArray(msg.IDCard), pMsg->IDCard, _tcslen(pMsg->IDCard));
		TCHARCopy(msg.Name, CountArray(msg.Name), pMsg->Name, _tcslen(pMsg->Name));
		g_FishServer.SendNetCmdToClient(pClient, &msg);
		return false;
	}
	if (!CheckEntityID(pMsg->IDCard))
	{
		ASSERT(false);
		OG_Cmd_RealNameVerification msg;
		SetMsgInfo(msg, GetMsgType(Main_Operate, OG_RealNameVerification), sizeof(OG_Cmd_RealNameVerification));
		msg.dwUserID = pMsg->dwUserID;
		msg.ErrorID = ROE_RealNameVerification_IDENTITY_Error;//身份证格式错误
		msg.Result = false;
		TCHARCopy(msg.IDCard, CountArray(msg.IDCard), pMsg->IDCard, _tcslen(pMsg->IDCard));
		TCHARCopy(msg.Name, CountArray(msg.Name), pMsg->Name, _tcslen(pMsg->Name));
		g_FishServer.SendNetCmdToClient(pClient, &msg);
		return false;
	}
	RealNameVerificationOnce pOnce;
	pOnce.beginTime = timeGetTime();
	pOnce.GameServerClientID = pClient->OutsideExtraData;
	pOnce.UserID = pMsg->dwUserID;
	TCHARCopy(pOnce.IDCard, CountArray(pOnce.IDCard), pMsg->IDCard, _tcslen(pMsg->IDCard));
	TCHARCopy(pOnce.Name, CountArray(pOnce.Name), pMsg->Name, _tcslen(pMsg->Name));
	m_RealNameVerificationMap.insert(HashMap<DWORD, RealNameVerificationOnce>::value_type(pOnce.UserID, pOnce));

	OnAddRealNameVerificationOnceResult(ROE_RealNameVerification_Sucess, true, pMsg->dwUserID);//直接让玩家验证完成了

	//发送命令到数据库去 检测身份证是否被使用了
	/*DBR_Cmd_CheckEntityID msgDB;
	SetMsgInfo(msgDB, DBR_CheckEntityID, sizeof(DBR_Cmd_CheckEntityID));
	msgDB.dwUserID = pMsg->dwUserID;
	TCHARCopy(msgDB.EntityID, CountArray(msgDB.EntityID), pMsg->IDCard, _tcslen(pMsg->IDCard));
	msgDB.EntityCrcID = GetCrc32(msgDB.EntityID);
	g_FishServer.SendNetCmdToDB(&msgDB);*/
	return true;
}
//void RealNameVerificationManager::OnCheckEntityResult(DBO_Cmd_CheckEntityID* pMsg)
//{
//	if (!pMsg)
//	{
//		ASSERT(false);
//		return;
//	}
//	HashMap<DWORD, RealNameVerificationOnce>::iterator Iter = m_RealNameVerificationMap.find(pMsg->dwUserID);
//	if (Iter == m_RealNameVerificationMap.end())
//	{
//		ASSERT(false);
//		return;
//	}
//	DWORD PhoneCrc = GetCrc32(Iter->second.IDCard);
//	if (PhoneCrc != pMsg->EntityCrcID)
//	{
//		ASSERT(false);
//		return;
//	}
//	if (pMsg->Result)
//	{
//		//1.手机号码检测成功 可以使用 我们让玩家进行处理
//		//1.组织URL 命令 通过HTTP 进行发送处理
//
//		OnAddRealNameVerificationOnceResult(ROE_RealNameVerification_Sucess, true, pMsg->dwUserID);//直接让玩家验证完成了
//	}
//	else
//	{
//		//手机已经被使用了
//		OnAddRealNameVerificationOnceResult(ROE_RealNameVerification_IDENTITY_IsBeUse, false, pMsg->dwUserID);//直接让玩家验证完成了
//	}
//}
void RealNameVerificationManager::OnAddRealNameVerificationOnceResult(BYTE ErrorID, bool Result, DWORD dwUserID)
{
	HashMap<DWORD, RealNameVerificationOnce>::iterator Iter = m_RealNameVerificationMap.find(dwUserID);//玩家已经提交订单了
	if (Iter == m_RealNameVerificationMap.end())
	{
		ASSERT(false);
		return;//不存在的订单 无须处理
	}
	ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(ConvertDWORDToBYTE(Iter->second.GameServerClientID));
	if (!pClient)
	{
		ASSERT(false);
		return;
	}
	OG_Cmd_RealNameVerification msg;
	SetMsgInfo(msg, GetMsgType(Main_Operate, OG_RealNameVerification), sizeof(OG_Cmd_RealNameVerification));
	msg.dwUserID = dwUserID;
	msg.Result = Result;
	msg.ErrorID = ErrorID;
	TCHARCopy(msg.IDCard, CountArray(msg.IDCard), Iter->second.IDCard, _tcslen(Iter->second.IDCard));
	TCHARCopy(msg.Name, CountArray(msg.Name), Iter->second.Name, _tcslen(Iter->second.Name));
	g_FishServer.SendNetCmdToClient(pClient, &msg);
	m_RealNameVerificationMap.erase(Iter);
}
void RealNameVerificationManager::OnUpdate(DWORD dwTimer)
{
	static DWORD RealNameVerificationLogTime = 0;
	if (RealNameVerificationLogTime == 0 || dwTimer - RealNameVerificationLogTime >= 30000)
	{
		RealNameVerificationLogTime = dwTimer;
		if (m_RealNameVerificationMap.empty())
			return;
		HashMap<DWORD, RealNameVerificationOnce>::iterator Iter = m_RealNameVerificationMap.begin();
		for (; Iter != m_RealNameVerificationMap.end();)
		{
			if (dwTimer - Iter->second.beginTime >= 30000)
			{
				//已经过期了
				ServerClientData* pClient = g_FishServer.GetUserClientDataByIndex(ConvertDWORDToBYTE(Iter->second.GameServerClientID));
				if (!pClient)
				{
					ASSERT(false);
					Iter = m_RealNameVerificationMap.erase(Iter);
					continue;
				}
				OG_Cmd_RealNameVerification msg;
				SetMsgInfo(msg, GetMsgType(Main_Operate, OG_RealNameVerification), sizeof(OG_Cmd_RealNameVerification));
				msg.dwUserID = Iter->second.UserID;
				msg.ErrorID = ROE_RealNameVerification_TimeOut;
				msg.Result = false;
				TCHARCopy(msg.IDCard, CountArray(msg.IDCard), Iter->second.IDCard, _tcslen(Iter->second.IDCard));
				TCHARCopy(msg.Name, CountArray(msg.Name), Iter->second.Name, _tcslen(Iter->second.Name));
				g_FishServer.SendNetCmdToClient(pClient, &msg);
				Iter = m_RealNameVerificationMap.erase(Iter);
			}
			else
			{
				++Iter;
			}
		}
	}	
}