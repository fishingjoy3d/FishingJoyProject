//通用机器人管理类
//机器人基于基本玩家 IsRobot = 1
//数据库 创建机器人 我们进行 设置 处理 RsgIP 等操作
#pragma once
typedef void(SendMsgToDBFun)(NetCmd* pCmd);
class RobotManager
{
public:
	RobotManager();
	virtual ~RobotManager();
	
	void  OnLoadRobotInfoBySum(DWORD BeginUserID,DWORD EndUserID, SendMsgToDBFun pSend);//加载指定数量的机器人
	void  OnLoadRobotInfoResult(DBO_Cmd_LoadRobotInfo* msg);
	RobotUserInfo* GetNonUseRobot();
	void  OnNonUseRobot(DWORD dwUserID);//归还机器人
	vector<RobotUserInfo*>& GetAllNonRobotVec(){ return m_NonUseRobotInfo; }

	bool IsRobot(DWORD dwUserID);
private:
	HashMap<DWORD, RobotUserInfo*> m_UseRobotInfo;
	vector<RobotUserInfo*> m_NonUseRobotInfo;
};