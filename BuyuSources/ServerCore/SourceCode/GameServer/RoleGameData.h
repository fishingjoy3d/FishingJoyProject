//玩家在游戏中的 一些数据的统计
//玩家参加比赛 获得名次的次数
//玩家参加比赛 获得前三名的次数 (分别123)
//玩家捕获特殊鱼 的次数 (特殊鱼)
//玩家获得金币的总数量
//玩家开炮消耗金币的数量等数据统计
#pragma once
#include "Stdafx.h"
class CRoleEx;
struct tagRoleGameData;
class RoleGameData
{
public:
	RoleGameData();
	virtual ~RoleGameData();

	bool OnInit(CRoleEx* pRole);

	void OnLoadRoleGameDataByDB();//从数据库加载玩家的数据
	void OnLoadRoleGameDataResult(DBO_Cmd_LoadGameData* pMsg);//从数据库返回的玩家游戏数据的资料
	void SendRoleGameDataToClient();//玩家记录数据发送到客户端去
	//void SaveRoleGameData();//将玩家记录数据保存到数据库去
	bool IsLoadDB(){ return m_IsLoadDB; }
	//关于玩家记录数据的具体处理函数 根据功能写死 无须特殊处理
	void OnHandleCatchFish(BYTE FishID);
	void OnHandleRoleGetGlobel(DWORD AddGlobel);//玩家获得金币
	void OnHandleRoleMonthReward(int RewardIndex);//玩家获得比赛的名称

	void OnHandleRoleJoinTable(bool IsMonth);
	void OnHandleRoleLeaveTable();
	void OnHandleRoleSignUpMonth();

	void OnHandleRoleComb();

	tagRoleGameData& GetGameData(){ return m_RoleGameData; }
private:
	bool					m_IsLoadDB;
	CRoleEx*				m_pRole;
	//具体的结构 需要根据策划具体的数据 来进行添加 一个列一个函数 并且将功能直接写死 无须获得配置
	tagRoleGameData			m_RoleGameData;

	//临时记录数据
	bool					m_IsInMonth;
	DWORD					m_LogJoinTableTime;
};