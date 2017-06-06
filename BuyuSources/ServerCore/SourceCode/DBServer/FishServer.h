//DB 服务器的核心类
#pragma once
#include "..\CommonFile\FishConfig.h"
typedef HashMap<UINT, SqlTable*> TableHashMap;
int SplitStringA(const std::string& strIn, const std::string& strDelimiter, std::vector<std::string>& ret);
struct RcveMsg
{
	NetCmd* pCmd;
	BYTE   ClientID;
};
class FishServer : public INetHandler
{
public:
	FishServer();
	virtual ~FishServer();

	bool InitServer(BYTE DBNetworkID);
	bool MainUpdate();
	bool OnDestroy();

	void SetServerClose(){ m_bRun = true; }
	void SetReloadConfig(){ m_IsReloadConfig = true; }
	void ShowInfoToWin(const char *pcStr, ...);

	virtual uint CanConnected(BYTE SeverID, uint ip, short port, void *pData, uint recvSize);
	virtual bool NewClient(BYTE SeverID, ServerClientData *pClient, void *pData, uint recvSize);
	virtual void Disconnect(BYTE ServerID, ServerClientData *pClient, RemoveType rt);
	void OnAddClient();

	//void SendNetCmdToClient(ServerClientData* pClient, NetCmd* pCmd);
	void SendNetCmdToControl(NetCmd*pCmd);

	FishConfig& GetFishConfig(){ return m_Config; }


	ServerClientData* GetClientData(BYTE ClientID);
	void HandleThreadMsg(BYTE ThreadIndex);//线程处理命令
private:
	bool ConnectClient();
	bool ConnectControl();

	void OnHandleAllMsg();
	void OnAddDBResult(BYTE Index, BYTE ClientID, NetCmd*pCmd);

	void OnTcpServerLeave(BYTE ServerID, ServerClientData* pClient);
	void OnTcpServerJoin(BYTE ServerID, ServerClientData* pClient);
	void OnTcpClientConnect(TCPClient* pClient);
	void OnTcpClientLeave(TCPClient* pClient);

	bool HandleDataBaseMsg(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool HandleControlMsg(NetCmd* pCmd);

	void OnReloadConfig();


	//MySQl
	bool SelectTable(const SqlCondition &d, SqlTable *pMainTable, SqlTable &table){
		return pMainTable->Select(d, table);
	}
	bool CommitTable(SqlTable &table);
	bool CommitTable(SqlTable &mainTable, BYTE *pTableData, UINT size){
		return mainTable.CombineFromBytes(pTableData, size);
	}
private:
	//数据库具体的操作
	//具体数据库操作的函数
	//1.登陆相关
	bool OnHandleOperatorLogon(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAccountLogon(BYTE Index, BYTE ClientID, NetCmd* pCmd);//账号登陆	
	bool OnHandleQueryLogon(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAccountRsg(BYTE Index, BYTE ClientID, NetCmd* pCmd);//账号注册
	bool OnHandlePhoneSecPwdLogon(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAccountReset(BYTE Index, BYTE ClientID, NetCmd* pCmd);//游客账号转正式账号
	bool OnHandleChangeAccountPassword(BYTE Index, BYTE ClientID, NetCmd* pCmd);//修改账号密码
	bool OnHandleChangeAccountOnline(BYTE Index, BYTE ClientID, NetCmd* pCmd);//修改账号是否在线信息
	//bool OnHandleRoleAchievementIndex(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleRoleClientInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleGetNewAccount(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveChannelInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSetAccountFreeze(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandlePhoneLogon(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//2.玩家相关
	bool OnHandleGetRoleInfoByUserID(BYTE Index, BYTE ClientID, NetCmd* pCmd);//获取玩家数据根据玩家ID
	bool OnHandleLoadAllUserInfo(bool IsOnceDay, DWORD dwUserID, BYTE Index, BYTE ClientID);
	bool OnHandleSaveRoleInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);//保存玩家数据
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
	bool OnHandleAddRoleTotalRecharge(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//bool OnHandleSaveRoleLockInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleIsFirstPayGlobel(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleIsFirstPayCurrcey(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeRoleMoney(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleTableChange(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleRoleEntityItem(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleEntityItem(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleMonthCardInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleRateValue(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleMaxRate(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleGetMonthCardRewardTime(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleVipLevel(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleCashSum(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleCashSum(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleParticularStates(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleSecPassword(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeRoleSecPassword(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeRoleShareStates(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleGameIDConvertUserID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//3.玩家物品
	bool OnHandleGetUserItem(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddUserItem(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelUserItem(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeUserItem(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//4.玩家关系
	bool OnHandleGetUserRelation(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddUserRelation(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelUserRelation(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//bool OnHandleChangeUserRelation(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	//5.邮件
	bool OnHandleGetUserMail(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddUserMail(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelUserMail(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleGetUserMailItem(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleChangeUserMailIsRead(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddOperatorSystemMail(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSystemMailRecordStateModify(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//6.查询
	bool OnHandleQueryRoleInfoByNickName(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleQueryRoleInfoByUserID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleQueryRoleInfoByGameID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//7.签到
	/*bool OnHandleLoadRoleCheckInfo(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleUpdateRoleCheckInfo(BYTE Index,BYTE ClientID, NetCmd* pCmd);*/
	//8.任务
	bool OnHandleLoadRoleTask(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleTask(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleTask(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleClearRoleTask(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveAllTask(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//9.成就
	bool OnHandleLoadRoleAchievement(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleAchievement(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleAchievement(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveAllAchievement(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//10.活动
	bool OnHandleLoadRoleAction(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleAction(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleAction(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveAllAction(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//11.称号
	bool OnHandleLoadRoleTitle(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddRoleTitle(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleTitle(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//12.商店
	bool OnHandleSaveRoleEntityItem(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//13.实体
	bool OnHandleLoadRoleEntity(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//bool OnHandleSaveRoleEntity(BYTE Index,BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityName(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityPhone(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityEmail(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityIdentityID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityItemUseName(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityItemUsePhone(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleSaveRoleEntityItemUseAddress(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//14.赠送
	bool OnHandleLoadRoleGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddRoleGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRoleGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLoadNowDayGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleClearNowDayGiff(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//15.魅力
	bool OnHandleChangeRoleCharm(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//16.排行榜
	bool OnHandleLoadRankInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
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
	bool OnHandleLogRoleTableInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogRoleEntityItemInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleCreateLogTable(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogNiuNiuTableInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogExChangeInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogLotteryInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogMonthInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogDialInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLogCarInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDealApplyCreateLog(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDealCreateLog(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDealThirdPlatformVerifyLog(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDealPayLog(BYTE Index, BYTE ClientID, NetCmd* pCmd);




	//Announcement
	bool OnHandleLoadAllAnnouncementInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddAnnouncementInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	
	//邮箱
	bool OnHandleChangeRoleEmail(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleLoadAllSystemMail(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//Center
	bool OnHandleClearFishDB(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//ExChange
	bool OnHandleQueryExChange(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnDelExChange(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//Recharge
	bool OnThirdPlatformVerify(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnThirdPlatformCreateAndVerify(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnAddRecharge(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnLoadRechargeOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnAddRechargeOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnCheckPhone(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnCheckEntityID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnLoadPhonePayOrderList(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnAddPhonePayOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnGetRechargeOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnQueryRechargeOrderInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnDleRechargeOrderInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);

	bool OnCreateDealOrderID(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//Control
	bool OnResetUserPassword(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleFreezeUser(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//Robot
	bool OnHandelLoadRobotInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//Char
	bool OnHandleLoadCharInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelCharInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddCharInfo(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	//RelationRequest
	bool OnHandleLoadRelationRequest(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleAddRelationRequest(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	bool OnHandleDelRelationRequest(BYTE Index, BYTE ClientID, NetCmd* pCmd);
	void UpdateInfoToControl(DWORD dwTimer);
	std::string GetRateValueString(int256& RateValue);
	int256 GetRateValueFromString(const char* s);
private:
	//dump
	Dump										m_pDump;
	BYTE										m_DBNetworkID;
	//服务器控制属性
	volatile bool								m_IsReloadConfig;
	volatile bool								m_bRun;
	volatile long								m_ExitCount;

	NewServer									m_Server;//对GameServer的网络库
	SafeList<AfxNetworkClientOnce*>				m_AfxAddClient;
	HashMap<BYTE, ServerClientData*>			m_ClintList;
	//SQL
	SQL											m_Sql[DB_ThreadSum];
	SafeList<RcveMsg*>							m_RecvMsgList[DB_ThreadSum];
	SafeList<RcveMsg*>							m_SendMsgList[DB_ThreadSum];
	SafeList<SqlUpdateStr*>						m_SqlList;
	//ControlServer
	TCPClient									m_ControlTcp;
	bool										m_ControlIsConnect;
	//Config
	FishConfig									m_Config;
};
extern FishServer g_FishServer;