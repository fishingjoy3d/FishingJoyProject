//玩家之间的相互赠送功能
//玩家每天可以发送多少次赠送 
//玩家每天可以接受多少次赠送 
//每次赠送携带的金币数
//记录玩家每天发送的数据的次数 每日数据
//记录玩家每天接收的次数  每日数据 
#pragma once
#include "Stdafx.h"
class CRoleEx;
class RoleGiffManager
{
public:
	RoleGiffManager();
	virtual ~RoleGiffManager();
	bool OnInit(CRoleEx* pRole);
	bool OnLoadRoleGiff();
	void OnLoadRoleGiffResult(DBO_Cmd_LoadRoleGiff* pDB);
	void OnLoadRoleSendGiffInfo(DBO_Cmd_GetNowDayGiff* pDB);
	void GetRoleGiffToClient();
	void SendGiffToOtherRole(DWORD dwUserID);
	void SendGiffToOtherRoleResult(DWORD dwDestUserID);
	void AcceptRoleGiff(DWORD GiffID);
	void AcceptAllGiff();
	void OnDayChange();
	void AddBeGiff(DWORD SrcUserID);
	void AddBeGiffResult(DBO_Cmd_AddRoleGiff* pMsg);

	void SendNowDaySendGiffToClient();
	bool IsLoadDB(){ return m_IsLoadDB; }

	void ResetClientInfo(){ m_IsSendClient = false; }

	bool GetGiffMessageStates();
private:
	CRoleEx*					m_pRole;
	std::vector<tagGiffInfo>	m_GiffVec;//所有的赠送的集合
	bool						m_IsLoadDB;
	bool						m_IsSendClient;
 
	HashMap<DWORD, tagNowDaySendGiffInfo> m_SendInfo;//今天我发送的赠送的集合
	//HashMap<DWORD, BYTE>		m_SendInfo;
};