//玩家身上的比赛的类
//只有玩家正式进入比赛桌子的时候这个类才会被填充
#pragma once
#include "Stdafx.h"
class CRoleEx;
struct tagMonthConfig;
class RoleMont
{
public:
	RoleMont();
	virtual ~RoleMont();

	bool OnInit(CRoleEx* pRole);

	void ClearMonthInfo();
	void OnLoadMonthInfo(tagRoleMonthInfo* pInfo);

	bool IsInMonthTable();

	bool OnChangeRoleMonthGlobel(int MonthGlobel,bool IsSendToClient = false);
	bool OnChangeRoleMonthPoint(int Point, bool IsSendToClient = false);

	void OnChangeRoleMonthIndex(DWORD dwIndex, DWORD UpperSocre);//修改玩家的排名
	void OnRoleAddMonthGlobel();//玩家进行续币操作
	bool IsCanAddMonthGlobel();

	void OnMonthEnd(BYTE Month,DWORD Index,DWORD MonthScores,DWORD VipMonthScores);//当比赛结束的时候

	void OnPlayerLeaveTable();

	BYTE GetMonthID(){ return m_MonthID; }
	
	tagRoleMonthInfo& GetMonthInfo(){ return m_MonthInfo; }

	void OnUpdate(DWORD dwTimer);
	void OnUpMonthInfo(BYTE MonthID);

	void OnLoadSignUpMonthInfo(CG_Cmd_LoadRoleSignUp* pMsg);
	void SendAllSignUpInfoToClient();
	void OnSignUpSucess(BYTE Month);

	void OnResetMonth(BYTE Month);

	void OnUseMonthSkill(BYTE Month);
private: 
	CRoleEx*					m_pRole;
	BYTE						m_MonthID;
	tagRoleMonthInfo			m_MonthInfo;//玩家的在GameServer上的比赛的信息
	//tagMonthConfig*				m_MonthConfig;
	bool						m_IsCenterChange;
	DWORD						m_CenterUpdateTime;

	//报名
	vector<BYTE>				m_SignUpMonthVec;
	bool						m_IsLoadSignUpFinish;
	bool						m_IsNeedSendToClient;
};