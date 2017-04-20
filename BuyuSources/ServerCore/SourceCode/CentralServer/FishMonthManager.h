//中央服务器上的比赛管理器 管理来自全部GameServer上的全部数据
//比赛的管理器  对比赛进行管理 玩家的报名 玩家的加入 等操作
#pragma once
#include "Stdafx.h"
#include <vector>
struct tagMonthConfig;
struct LastMonthRewardInfo
{
	DWORD				LoadSum;
	DWORD				m_NowMonthTimeID;
	//EndTime
	SYSTEMTIME pTime;
	//上次比赛的记录
	std::vector<tagRoleMonthCenterInfo*>		Lastm_RoleVec;//按积分继续排序
	HashMap<DWORD, tagRoleMonthCenterInfo*>		Lastm_MonthRoleMap;//玩家的集合
	LastMonthRewardInfo()
	{
		LoadSum = 0;
		m_NowMonthTimeID = 0;
		Lastm_RoleVec.clear();
		Lastm_MonthRoleMap.clear();
	}
	void OnAddInfo(DWORD TimeID,std::vector<tagRoleMonthCenterInfo*>& pVec)
	{
		m_NowMonthTimeID = TimeID;
		GetLocalTime(&pTime);
		LoadSum = 0;
		if (pVec.empty())
			return;
		std::vector<tagRoleMonthCenterInfo*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			if (!(*Iter))
				continue;
			Lastm_RoleVec.push_back(*Iter);
			Lastm_MonthRoleMap.insert(HashMap<DWORD, tagRoleMonthCenterInfo*>::value_type((*Iter)->MonthInfo.dwUserID, *Iter));
		}
	}
	void UpdateRoleInfo(DWORD dwUserID, DWORD Point, DWORD Globel, DWORD SkillSum)
	{
		HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator Iter = Lastm_MonthRoleMap.find(dwUserID);
		if (Iter == Lastm_MonthRoleMap.end() || !Iter->second)
		{
			ASSERT(false);
			return;
		}
		Iter->second->MonthInfo.dwMonthGlobel = Globel;
		Iter->second->MonthInfo.dwMonthScore = Point;
		Iter->second->MonthInfo.SkillSum = SkillSum;
	}
	bool OnAddGameServerFinish();
	void OnDestroy()
	{
		if (Lastm_RoleVec.empty())
			return;
		std::vector<tagRoleMonthCenterInfo*>::iterator Iter = Lastm_RoleVec.begin();
		for (; Iter != Lastm_RoleVec.end(); ++Iter)
		{
			if (!(*Iter))
				continue;
			delete (*Iter);
		}
		Lastm_RoleVec.clear();
		Lastm_MonthRoleMap.clear();

		LoadSum = 0;
		m_NowMonthTimeID = 0;
	}
};
struct tagRobotInfo
{
	DWORD	dwUserID;
	DWORD	Level;
	BYTE	SkillSum;
};
class FishMonth //一场比赛
{
public:
	FishMonth();
	virtual ~FishMonth();

	bool OnInit(BYTE MontID);
	void OnDestroy();
	//比赛的处理函数
	void  UpdateByMin();//关系函数 1分钟更新一次 关系比赛的状态
	void  UpdateMonthIndex();
	void  SignUp(DWORD dwUserID);//是否可以报名比赛
	void  SignUpResult(GC_Cmd_SignUpMonthResult* pMsg);
	void  JoinMonth(DWORD dwUserID, WORD TableID);//是否可以参加比赛

	void OnChangeRolePoint(DWORD dwUserID, DWORD Point);
	void OnChangeRoleGlobel(DWORD dwUserID, DWORD Globel);
	void OnChangeRoleInfo(DWORD dwUserID, DWORD Point, DWORD Globel,DWORD SkillSum);
	void OnAddMonthGlobelNum(DWORD dwUserID);

	void OnLoadAllGameUserMonthInfo();

	bool RoleIsInMonth(DWORD dwUserID){ return m_MonthRoleMap.count(dwUserID) == 1; }
	BYTE GetMonthID();
	WORD GetSigupSum(){ return ConvertDWORDToWORD(m_RoleVec.size() /*+ m_MonthRankSum*/); }

	BYTE IsInSignUp();
	BYTE IsInStart();

	void ResetUserMonthInfo(DWORD dwUserID);//用户将自己的比赛重置


	bool MonthIsNeedSendPlayerSum(){ return m_IsInSignUp  && m_IsStart; }


	bool IsCanJoinMonth(DWORD dwUserID, tagRoleMonthInfo& MonthInfo);

	LastMonthRewardInfo& GetLastMonthInfo(){ return m_LastMonthInfo; }

	void OnUpdateRobotInfo();
	void OnLoadRobotFinish();

	void SendMonthStatesToGameServer(BYTE GameConfigID,BYTE MonthStates);
private:
	void  OnJoinSignUp();//比赛进入报名阶段
	void  OnLeaveSignUp();//比赛离开报名阶段
	void  OnJoinStart();//比赛进入比赛阶段
	void  OnLeaveStart();//比赛结束
	void  OnSaveMonthXml(SYSTEMTIME& pTime,std::vector<tagRoleMonthCenterInfo*>& pVec);

	void SaveFileToFtp(const char* FilePath,const char* FileName);

	void OnRoleSumChange();

	void OnLoadRobotInfo();
	
	void OnDestroyRobotInfo();
private:
	//tagMonthConfig*					m_FishMonthConfig;//当前比赛的配置信息
	BYTE							m_MonthID;
	bool							m_IsInSignUp;//是否在报名阶段
	bool							m_IsStart;//是否在开始阶段
	//当前比赛的集合 还需要进行调整排名数据 需要进行排名 无须使用MapVec
	std::vector<tagRoleMonthCenterInfo*>		m_RoleVec;//按积分继续排序
	HashMap<DWORD,tagRoleMonthCenterInfo*>		m_MonthRoleMap;//玩家的集合

	DWORD										m_NowMonthTimeID;

	LastMonthRewardInfo							m_LastMonthInfo;

	//DWORD										m_MonthRankSum;

	HashMap<DWORD, tagRobotInfo>				m_RobotMap;//机器人的集合 
};
class FishMonthManager //管理器
{
public:
	FishMonthManager();
	virtual ~FishMonthManager();

	bool	OnInit();
	void    Update(DWORD dwTimer);
	FishMonth* QueryMonth(BYTE MonthID);

	void SendRoleSignUpInfoToGameServer(DWORD dwUserID);
	void ResetUserMonthInfo(DWORD dwUserID, BYTE MonthID);

	void OnSendMonthDataToGame(BYTE GameSocketID);

	void OnLoadRobotFinish();

	bool RobotIsFinish(){ return m_RobotIsFinish; }

	void OnGameRsg(BYTE GameID);
	//void OnLogonToMonth(ServerClientData* pClient,DWORD dwUserID, BYTE MonthID);
private:
	void	UpdateByMin(DWORD dwTimer);
	void	UpdateMonthIndex(DWORD dwTimer);
	void	UpdateRobot(DWORD dwTimer);
	//void    UpdateMonthRoleSunToGameServer(DWORD dwTimer);
private:
	HashMap<BYTE, FishMonth*>			m_FishMonthMap;
	bool								m_RobotIsFinish;
};