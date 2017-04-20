#pragma once
#include "Stdafx.h"
class CRoleEx;

//玩家进行签到
//1.玩家每天的签到
//2.玩家可以领取当前的签到奖励 也可以 继续领取 本月签到奖励


//玩家签到数据到RoleInfo里面 这里为 签到的外部处理函数



class RoleCheck //玩家签到 查询 玩家级别的查询
{
public:
	RoleCheck();
	virtual ~RoleCheck();

	bool  OnInit(CRoleEx* pRole);//签到数据进行签到的初始化

	bool  RoleChecking();//玩家进行签到
	bool  RoleCheckeOnther(BYTE DaySum);//玩家进行补签

	//bool  LoadRoleCheckInfo();//加载玩家的签到数据
	//void  LoadRoleCheckInfoResult(DBO_Cmd_LoadRoleCheckInfo* pDB);//数据库返回值
	//bool  GetRoleCheckInfo();//客户端加载签到数据
	//void OnDayChange()
	//bool IsLoadDB(){ return m_IsLoadDB; }

	bool GetCheckMessageStates();
private:
	//bool HandleCheckCheckInfo();

	//对当前函数进行处理
	bool IsCanCheckNowDay();
	BYTE GetMonthCheckSum();

private:
	//tagRoleCheckInfo		m_RoleCheckInfo;
	CRoleEx*				m_Role;
	//bool					m_IsLoadDB;
};