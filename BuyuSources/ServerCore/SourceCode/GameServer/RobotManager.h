//机器人的管理选项
#pragma once
class CRoleEx;
class Robot//单个机器人的处理
{
public:
	Robot();
	virtual ~Robot();
	void OnInit(CRoleEx* pRole);
	void OnUpdate(DWORD dwTimer);
	void OnUseRobot(WORD TableID, BYTE MonthID);
	void OnDelRobot();

	bool IsUse(){ return m_IsUse; }
private:
	void		OnUpdateRoleChangeLauncher();//玩家切炮控制
	void		OnUpdateRoleOpenFire();//玩家开炮控制
	void		OnUpdateRoleLauncherFace();//控制玩家炮台方向
	void		OnUpdateRoleUseSkill();//控制玩家使用技能
private:
	CRoleEx*			m_pRole;
	bool				m_IsUse;
	DWORD				m_UpdateTimeLog;

	DWORD				m_ChangeLauncherTimeLog;

	DWORD				m_ChangeAngleTimeLog;
	DWORD				m_Angle;//炮台方向

	WORD				m_LockFishID;
};
class RobotManager
{
public:
	RobotManager();
	virtual ~RobotManager();

	void OnInit(DWORD BeginRobotUserID,DWORD EndRobotUserID);

	void OnAddRobot(DWORD dwUserID);
	void OnUseRobot(WORD TableID, BYTE MonthID);
	void OnDelRobot(DWORD dwUserID);

	void OnUpdate(DWORD dwTimer);
private:
	Robot* GetFreeRobot();//获取 一个空闲的机器人
private:
	HashMap<DWORD, Robot*>		m_RobotMap;//机器人的集合
};
