#pragma once
#include "..\CommonFile\FishConfig.h"
typedef HashMap<UINT, SqlTable*> TableHashMap;
struct RcveMsg
{
	NetCmd* pCmd;
	BYTE   ClientID;
	//ServerClientData* pClient;
};
class DBServer: public INetHandler
{
public:
	DBServer();
	~DBServer();

	void SetServerClose(){ m_bRun = false; }


	//bool Init(const SQLInitData &sid);
	bool Init(DWORD Index);
	void DBThread();
	void NetThread();
	void HandleThreadMsg(BYTE ThreadIndex);

	void ShowInfoToWin(const char *pcStr, ...);
	//void SendDBMsg();
	void Shutdown();
	virtual uint CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize){
		return CONNECT_CHECK_OK;
	}
	virtual void Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt);
	virtual bool NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize)
	{
		if (!pClient)
		{
			ASSERT(false);
			return false;
		}
		if (!pData || recvSize != sizeof(BYTE))
		{
			m_Server.Kick(pClient);
			ASSERT(false);
			return false;
		}
		AfxNetworkClientOnce* pOnce = new AfxNetworkClientOnce();
		if (!pOnce)
		{
			m_Server.Kick(pClient);
			ASSERT(false);
			return false;
		}
		pOnce->SeverID = SeverID;
		pOnce->pClient = pClient;
		pOnce->pPoint = (void*)malloc(recvSize);
		if (!pOnce->pPoint)
		{
			m_Server.Kick(pClient);
			ASSERT(false);
			delete pOnce;
			return false;
		}
		memcpy_s(pOnce->pPoint, recvSize, pData, recvSize);//复制进来
		pOnce->Length = recvSize;
		m_AfxAddClient.AddItem(pOnce);
		return true;
	}

	void OnAddClient();

	bool HandleDataBaseMsg(BYTE Index, BYTE ClientID, NetCmd* pCmd);

	ServerClientData* GetClientData(BYTE ClientID);

	void SendNetCmdToControl(NetCmd*pCmd);
protected:
	bool SelectTable(const SqlCondition &d, SqlTable *pMainTable, SqlTable &table){
		return pMainTable->Select(d, table);
	}
	bool CommitTable(SqlTable &table);
	bool CommitTable(SqlTable &mainTable, BYTE *pTableData, UINT size){
		return mainTable.CombineFromBytes(pTableData, size);
	}
	Dump				m_pDump;
	SQL					m_Sql[DB_ThreadSum];
	volatile bool	m_bRun;
	volatile long	m_ExitCount;

	NewServer					m_Server;
	HashMap<BYTE,ServerClientData*>	m_ClintList;
	//DWORD								m_ClientIndex;
	SafeList<SqlUpdateStr*>		m_SqlList;

	//数据库处理
	SafeList<RcveMsg*>			m_RecvMsgList[DB_ThreadSum];
	SafeList<RcveMsg*>			m_SendMsgList[DB_ThreadSum];

	FishConfig					m_Config;

	SafeList<AfxNetworkClientOnce*>      m_AfxAddClient;

	TCPClient									m_ControlTcp;
	bool										m_ControlIsConnect;
	BYTE										m_DBNetworkID;
private:
	bool ConnectControl();
	bool HandleControlMsg(NetCmd* pCmd);

	void OnHandleAllMsg();

	void OnAddDBResult(BYTE Index, BYTE ClientID, NetCmd*pCmd);
	//具体数据库操作的函数
	//1.登陆相关
	bool OnHandleAccountLogon(BYTE Index, BYTE ClientID, NetCmd* pCmd);//账号登陆
	bool OnHandleQueryLogon(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAccountRsg(BYTE Index, BYTE ClientID, NetCmd* pCmd);//账号注册
	bool OnHandleAccountReset(BYTE Index, BYTE ClientID, NetCmd* pCmd);//游客账号转正式账号
	bool OnHandleChangeAccountPassword(BYTE Index, BYTE ClientID, NetCmd* pCmd);//修改账号密码
	bool OnHandleChangeAccountOnline(BYTE Index, BYTE ClientID, NetCmd* pCmd);//修改账号是否在线信息
	//bool OnHandleRoleAchievementIndex(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleRoleClientInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleGetNewAccount(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveChannelInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//2.玩家相关
	bool OnHandleGetRoleInfoByUserID(BYTE Index,BYTE ClientID, NetCmd* pCmd);//获取玩家数据根据玩家ID
	bool OnHandleLoadAllUserInfo(bool IsOnceDay,DWORD dwUserID,BYTE Index, BYTE ClientID);
	bool OnHandleSaveRoleInfo(BYTE Index,BYTE ClientID, NetCmd* pCmd);//保存玩家数据
	//bool OnHandleSaveRoleQueryAtt(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleNickName(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleLevel(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleGender(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleAchievementPoint(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleCurTitle(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleCharmArray(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleCurency(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleFaceID(BYTE Index, BYTE ClientID, NetCmd* pCmd);

	bool OnHandleSaveRoleMedal(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleSendGiffSum(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleAcceptGiffSum(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleTaskStates(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleAchievementStates(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleActionStates(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleOnlineStates(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleCheckData(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleIsShopIpAddress(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleAllInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleGlobel(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleOnLineMin(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleExChangeStates(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleTotalRecharge(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleIsFirstPayGlobel(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleIsFirstPayCurrcey(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeRoleMoney(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleTableChange(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//3.玩家物品
	bool OnHandleGetUserItem(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddUserItem(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelUserItem(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeUserItem(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//4.玩家关系
	bool OnHandleGetUserRelation(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddUserRelation(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelUserRelation(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//bool OnHandleChangeUserRelation(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//5.邮件
	bool OnHandleGetUserMail(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddUserMail(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelUserMail(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleGetUserMailItem(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeUserMailIsRead(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//6.查询
	bool OnHandleQueryRoleInfoByNickName(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleQueryRoleInfoByUserID(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//7.签到
	/*bool OnHandleLoadRoleCheckInfo(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleUpdateRoleCheckInfo(BYTE Index,BYTE ClientID, NetCmd* pCmd);*/
	//8.任务
	bool OnHandleLoadRoleTask(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleTask(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleTask(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleClearRoleTask(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveAllTask(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//9.成就
	bool OnHandleLoadRoleAchievement(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleAchievement(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleAchievement(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveAllAchievement(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//10.活动
	bool OnHandleLoadRoleAction(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleAction(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleAction(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveAllAction(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//11.称号
	bool OnHandleLoadRoleTitle(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddRoleTitle(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleTitle(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//12.商店
	bool OnHandleSaveRoleEntityItem(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//13.实体
	bool OnHandleLoadRoleEntity(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//bool OnHandleSaveRoleEntity(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityName(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityPhone(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityEmail(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityIdentityID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityItemUseName(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityItemUsePhone(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityItemUseAddress(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//14.赠送
	bool OnHandleLoadRoleGiff(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddRoleGiff(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleGiff(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLoadNowDayGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleClearNowDayGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//15.魅力
	bool OnHandleChangeRoleCharm(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//16.排行榜
	bool OnHandleLoadRankInfo(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLoadWeekRankInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveWeekRankInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeWeekInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//17.GameData
	bool OnHandleLoadRoleGameData(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleGameData(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeRoleGameDataByMonth(BYTE Index, BYTE ClientID, NetCmd* pCmd);

	bool OnHandleAchievementPointList(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//
	bool OnHandleLoadAllAccountInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//
	bool OnHandleLogInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogRechargeInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogRoleOnlineInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogPhonePayInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//Announcement
	bool OnHandleLoadAllAnnouncementInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddAnnouncementInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//邮箱
	bool OnHandleChangeRoleEmail(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//Center
	bool OnHandleClearFishDB(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//ExChange
	bool OnHandleQueryExChange(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnDelExChange(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//Recharge
	bool OnAddRecharge(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnLoadRechargeOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnAddRechargeOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnCheckPhone(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnCheckEntityID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnLoadPhonePayOrderList(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnAddPhonePayOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
};
extern DBServer g_DBServer;