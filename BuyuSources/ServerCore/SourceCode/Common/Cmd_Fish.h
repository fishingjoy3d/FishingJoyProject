//当前项目的  核心的结构类 
#pragma once
#pragma pack(push)
#pragma pack(1)
#include <time.h>
#include <set>
//一些常量的定义
#define MAX_NICKNAME 8   //玩家昵称的长度
#define MIN_NICKNAME 2

#define MAX_ERRORSTR_LENGTH 16

//#define MAX_MAIL_ITEM 3	 //邮件最多携带的物品数量

#define MAX_MAIL_CONTEXT 128  // 邮件内容最大长度
#define MIN_MAIL_CONTEXT 1

#define ACCOUNT_LENGTH 32  //账户的长度
#define ACCOUNT_MIN_LENGTH 4

#define	MAC_ADDRESS_LENGTH 56
#define LOGON_EXTERNAL_LENGTH 512

#define MAX_CHARM_ITEMSUM 8  //魅力数组的长度

#define MAX_NAME_LENTH 10  //玩家现实名称的长度
#define MIN_NAME_LENTH 2

#define MAX_ADDRESS_LENGTH 128  //玩家现实地址的长度
#define MIN_ADDRESS_LENGTH 1

#define MAX_PHONE_LENGTH 32 //玩家手机的长度
#define MIN_PHONE_LENGTH 8 //玩家手机的长度

#define MAX_IDENTITY_LENGTH 32 //玩家身份证的长度
#define MIN_IDENTITY_LENGTH 15 //玩家身份证的长度

#define MAX_EMAIL_LENGTH 64 //玩家邮箱地址的长度
#define MIN_EMAIL_LENGTH 12

#define MAX_MAP_NAME	9 //游戏场景地图名称长度
#define QUERY_ROLE_SUM 6 //一次查询玩家分页的数量
//#define SOCKET_TCP_PACKET 1000 //对外网络最大的长度限制
#define THREAD_NUM 8  //更新桌子的线程数量
#define DB_ThreadSum 8 //DB的线程数量
//#define DB_Msg_PageSum 60000
#define Diff_Update_Sec 600  //排行榜提前时间秒
#define CHECKMSG(NowMSG,LENGTH,TYPE)		\
	(NowMSG == NULL || sizeof(TYPE) != LENGTH) ? NULL : static_cast<TYPE*>(NowMSG);
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
#define SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 
#define GetMsgType(Main,Sub) (Main<<8)+Sub
#define SetMsgInfo(Object,Type,Size) Object.SetCmdSize(Size); Object.SetCmdType(Type);
#define CheckMsgSize(Size) if(Size >= 0xffff) {ASSERT(false);return;}
#define CheckMsgSizeReturn(Size) if(Size >= 0xffff) {ASSERT(false);return false;}
#define Msg_OnceSum 200  //内部网络的
#define Client_Msg_OnceSum 1
#define GameServer_Max_UserSum 1000

#define MAX_ADDRESS_LENGTH_IP 16
#define Defalue_Ip_Address TEXT("隐藏地址")

#define MAX_MESSAGE_LENGTH 128
#define MIN_MESSAGE_LENGTH 1

//#define MAX_LOGINFO_LENGTH  128
#define MAX_Announcement_Length 64

#define IP_MAX_LENGTH 16

#define  DB_Log_Str_Length 256

#define  RMB_OnceOnlyID_Length 32

#define  ExChangeCode_Length 16

#define PhoneIsCanUse(PhoneNumber) (PhoneNumber <=99999999999 && PhoneNumber>=10000000000)


#define MAX_NIUNIU_ClientSum 4 //客户端下注区的数量
#define MAX_NIUNIU_BrandSum 5  //一次发牌的数量
#define MAX_VIPSEAT_Sum 8 //贵宾席的数量
#define MAX_SHOWBRAND_Sum 8
#define MAX_NORMAL_PAGESUM 9


#define MAX_DIAL_GameSum 24 //客户端转盘动物的数量
#define MAX_DIAL_ClientSum 12 //客户端下注区的数量
#define MAX_DIALVIPSEAT_Sum 8 //贵宾席的数量
#define MAX_DIALSHOWBRAND_Sum 8
#define MAX_DIALNORMAL_PAGESUM 9



#define MAX_CAR_ClientSum 8 //客户端下注区的数量
#define MAX_CARVIPSEAT_Sum 8 //贵宾席的数量
#define MAX_CARSHOWBRAND_Sum 8
#define MAX_CARNORMAL_PAGESUM 9


#define MAX_MonthList_Length 4

#define MAX_RelationRequestLength 32

#define MAX_REQUIRED_OPERATOR_SHOP_ITEM_COUT 3
#define MAX_PayNOLength 64
#define MAX_IconLength 64
#define MAX_DisCountPicNameLength 64
#define MAX_OperatorTitleLength 24


#define MAX_SIGN_CODE 1024
#define MAX_UrlLength 512


const int MAC_LENGTH = 18;

struct AfxNetworkClientOnce
{
	BYTE SeverID;
	ServerClientData *pClient;
	void* pPoint;
	DWORD Length;
};





//static void LogTaskToFile(DWORD AddGlobelSum,DWORD AllGlobelSum)
//{
//	//写入到文件里面去 Log.txt
//	FILE* pFile = NULL;
//	errno_t Error = fopen_s(&pFile, "LogTask.txt", "a");
//	if (!pFile)
//	{
//		Error = fopen_s(&pFile, "LogTask.txt", "w+");
//	}
//	if (pFile)
//	{
//		SYSTEMTIME time;
//		GetLocalTime(&time);
//		fprintf_s(pFile, "%d %d\n", AddGlobelSum, AllGlobelSum);
//		fclose(pFile);
//	}
//}


struct int256  //特殊的结构 
{
	//BYTE 8 位
	BYTE Value[32];
};
struct int256Handle
{
	static void Clear(int256& p)
	{
		for (int i = 0; i < CountArray(p.Value); ++i)
			p.Value[i] = 0;
	}
	static void SetValue(int256& p, BYTE pArray[], BYTE Size)
	{
		//将数据设置到里面去
		for (int i = 0; i < Size && i<CountArray(p.Value); ++i)
		{
			p.Value[i] = pArray[i];
		}
	}
	static void SetBitStates(int256& p, BYTE BitIndex, bool States)
	{
		BYTE ArrayIndex = static_cast<BYTE>(BitIndex / 8);
		BYTE ChangeIndex = static_cast<BYTE>(BitIndex % 8);
		BYTE BitValue = static_cast<BYTE>(1 << ChangeIndex);
		if (States)
			p.Value[ArrayIndex] |= BitValue;
		else
			p.Value[ArrayIndex] ^= BitValue;
	}
	static bool GetBitStates(int256& p, BYTE BitIndex)
	{
		BYTE ArrayIndex = static_cast<BYTE>(BitIndex / 8);
		BYTE ChangeIndex = static_cast<BYTE>(BitIndex % 8);
		return ((p.Value[ArrayIndex] & (1 << ChangeIndex)) == (1 << ChangeIndex));
	}
};

//enum ErrorCode
//{
//	EC_Init = 0,
//	EC_Sucess = 0,
//	//错误编码 与服务器端相同
//	//GlobelShop
//	EC_GlobelShop_ItemNotExites,//指定的物品在货币商店不存在
//	EC_GlobelShop_CurreyError,//货币不够购买货币商店的指定物品
//	EC_Globel_Shop_Sucess,//购买物品成功了
//	//RoleAchievement
//	EC_RoleAchievement_NotExites,//成就不存在
//	EC_RoleAchievement_NotFinih,//成就条件未完成 无法提交完成
//	//RoleAction
//	EC_RoleAction_NotExites,//活动不存在
//	EC_RoleAction_NotFinih,//活动条件未完成 无法提交完成
//	//RoleCharm
//	EC_RoleCharm_NonMe,//不可以给自己添加魅力物品
//	EC_RoleCharm_NonItem,//指定的魅力物品不存在
//	EC_RoleCharm_GlobelError,//指定魅力物品的数量不够
//	EC_RoleCharm_Sucess,//赠送魅力物品成功了
//	EC_RoleCharm_SystemError,//系统错误
//	EC_RoleCharm_CenterFiled,//未与中央服务器连接
//	//RoleCheck
//	EC_RoleCheck_CheckNowDayError,//今天已经签到过了
//	EC_RoleCheck_CheckOtherMoneyError,//货币不够 无法补签
//	EC_RoleCheck_CheckOtherError,//无法补签 因为没有需要补签的日期
//	//RoleEntity
//	EC_RoleEntity_NameError,//实际名称长度不正确 无法修改
//	EC_RoleEntity_Phone,//实际手机号码不正确 无法修改
//	EC_RoleEntity_EMail,//实际邮箱地址不正确 无法修改
//	EC_RoleEntity_Address,//实际用户地址不正确 无法修改
//	EC_RoleEntity_EntityID,//实际用户的身份证号码不正确 无法修改
//	//RoleGiff
//	EC_RoleGiff_NotMe,//不可以给自己发送赠送
//	EC_RoleGiff_SendMaxSum,//今天已经到达最大赠送次数 无法继续赠送
//	EC_RoleGiff_SendOnceUserSum,//今天对当前玩家的赠送次数已经到达最大 无法继续赠送
//	EC_RoleGiff_RecvMaxSum,//今天已经到达最大的接收赠送次数 无法继续接收
//	EC_RoleGiff_GiffNotExites,//当前赠送不存在 无法接收
//	EC_RoleGiff_GiffNull,//不存在可以接收的赠送
//	EC_RoleGiff_TargetGiffMax,//目标的赠送列表已经满了 无法继续接收赠送
//	EC_RoleGiff_CenterFiled,//为连接中央服务器
//	//RoleItem
//	//RoleMail
//	EC_RoleMail_NotMe,//不可以给自己发送邮件
//	EC_RoleMail_MailNull,//目标邮件不存在
//	EC_RoleMail_ItemNonExites,//当前邮件无附件 无法领取
//	EC_RoleMail_ContextError,//邮件主体长度不正确 无法发送
//	EC_RoleMail_SendTargetMailFull,//邮件发送失败 目标邮箱已经满了 
//	EC_RoleMail_DelMailError,//目标邮件不存在 无法删除
//	EC_RoleMail_CenterField,//无法连接中央服务器
//	EC_RoleMail_SendSucess,//邮件发送成功
//	//RoleMonth
//	EC_RoleMonth_SigUpMonthNotExists,//当前比赛不存在 无法报名
//	EC_RoleMonth_SigUpMonthTimeError,//当前比赛不在报名时间 无法报名
//	EC_RoleMonth_SigUpMonthError,//已经报名当前比赛了 无法继续报名
//	EC_RoleMonth_SigUpMonthTableError,//无法加入放弃比赛的桌子的限制 无法报名比赛
//	EC_RoleMonth_SigUpMonthMoneyError,//货币不够 无法报名比赛
//	EC_RoleMonth_SigUpMonthItemError,//物品不够 无法报名比赛
//	EC_RoleMonth_JoinMonthNotExists,//当前比赛不存在 无法创建
//	EC_RoleMonth_JoinMonthTimeError,//当前比赛不在参加时间 无法参加
//	EC_RoleMonth_JoinMonthError,//未报名当前比赛了 无法继续参加
//	EC_RoleMonth_JoinMonthTableError,//无法加入放弃比赛的桌子的限制 无法参加比赛
//	EC_RoleMonth_AddMonthGlobelMonthError,//当前比赛不存在 无法续币
//	EC_RoleMonth_AddMonthGlobelSumError,//无法继续续币了 到达最大次数了
//	EC_RoleMonth_AddMonthGlobelMoneyError,//无法续币 货币不够
//	EC_RoleMonth_ResetMonthMonthError,//重置比赛失败 当前比赛不存在
//	EC_RoleMonth_ResetMonthMoneyError,//货币不够 无法重置比赛
//	EC_RoleMonth_ResetMonthItemError,//物品不够 无法重置比赛
//	//RoleOnlirneReward
//	EC_RoleOnlineReward_RewardIDError,//当前在线奖励不存在 无法领取
//	EC_RoleOnlineReward_OnlineMinError,//无法领取在线奖励 在线时间不够
//	EC_RoleOnlineReward_Error,//已经领取过在线奖励了 无法继续领取
//	EC_RoleOnlineReward_Sucess,//领取在线奖励成功
//	//RoleQuery
//	EC_QueryRole_RoleNameError,//查询名称长度不正确 无法查询
//	EC_QueryRole_QueryMeError,//玩家不可以查询自己
//	//RoleRank
//	EC_RoleRank_WeekReward_RankNotExites,//指定排行榜不存在
//	EC_RoleRank_WeekReward_NotInRank,//上周为上榜 无法领取奖励
//	EC_RoleRank_WeekReward_Error,//已经领取奖励了 无法继续领取
//	//RoleRelation
//	EC_RoleRelation_Add_NotMe,//不可以添加自己
//	EC_RoleRelation_Add_RelationExists,//已经存在关系 无法继续添加
//	EC_RoleRelation_Add_RelationIsFull,//关系列表已经满了 无法继续添加
//	EC_RoleRelation_Del_RelationNotExists,//关系不存在 无法删除
//	EC_RoleRelation_Change_RelationNotExists,//指定关系不存在 无法修改
//	EC_RoleRelation_Change_Error,//当前关系为发生变化 无法修改
//	//RoleShop
//	EC_RoleShop_ShopError,//指定商店不存在 无法购买
//	EC_RoleShop_ItemNotExists,//指定商店的物品不在 无法购买
//	EC_RoleShop_ItemNotinTime,//指定物品为激活 无法购买
//	EC_RoleShop_EntityItemError,//购买实体物品 必须完善显示地址和数据 
//	EC_RoleShop_MoneyError,//购买物品失败 货币不够
//	//RoleTable
//	EC_RoleTable_TableError,//指定桌子不存在 无法进入
//	EC_RoleTable_GlobelMin,//金币数量太少 无法进入桌子
//	EC_RoleTable_GlobelMax,//金币数量太多 无法进入桌子
//	EC_RoleTable_CurreyMin,//乐币数量太少 无法进入桌子
//	EC_RoleTable_CurreyMax,//乐币数量太多 无法进入桌子
//	EC_RoleTable_ItemError,//进入桌子需要的物品不符合 无法进入桌子
//	//RoleTask
//	EC_RoleTask_NotExites,//活动不存在
//	EC_RoleTask_NotFinih,//任务条件未完成 无法提交完成
//	//RoleTitle
//	EC_RoleTitle_NonTitle,//称号不存在 无法切换称号
//	//Package
//	EC_Package_NonItem,//礼包不存在
//	EC_Package_BagNonItem,//背包里没有礼包存在
//	EC_Package_Null,//礼包为空
//	EC_Package_Succes,//礼包打开成功
//	//Message
//	EC_Message_Length,//发送消息长度不正确 无法发送
//	EC_Message_Type,//消息类型错误
//	//实名验证
//	EC_Verification_Sucess,//验证成功
//	EC_Verification_IsExists,//重复提交订单
//	EC_Verification_Filed,//验证失败
//	//手机绑定
//	EC_BindPhone_PhoneNumberSucess,//手机验证码已经发送
//	EC_BindPhone_PhoneNumberFailed,//手机验证码发送失败
//	EC_BindPhone_PhoneNumberIsExists,//手机已经注册过了
//	EC_BindPhone_Sucess,
//	EC_BindPhone_Failed,
//	//实体物品购买
//	EC_EntityItem_Failed,
//	EC_EntityItem_Sucess,
//	//绑定邮箱
//	EC_BindEMail_Failed,
//	EC_BindEMail_Sucess,
//	EC_BindEmail_IsUse,
//	//充值
//	EC_Recharge_Sucess,
//	EC_Recharge_Failed,
//	//修改密码
//	EC_ResetPassword_Sucess,
//	EC_ResetPassword_Failed,
//	//使用物品
//	EC_UseItem_Sucess,
//	EC_UseItem_Failed,
//	//使用RMB
//	EC_UseRMB_Sucess,
//	EC_UseRMB_Failed,
//	//
//	EC_NickName_Error,
//	EC_AccountName_Error,
//	EC_Account_IsExists,
//	EC_Password_Error,
//	EC_Logon_Error,
//
//	//登陆的最新的提示
//	EC_Logon_AccountNotExists,//账号不存在
//	EC_Logon_PasswordError,//输入的密码错误
//	EC_Rsg_AccountExists,//账号已经存在 无法注册
//	//字符串错误提示
//	EC_AccountName_StringError,//账号字符串错误
//	EC_Password_StringError,//密码错误
//	EC_NickName_StringError,//用户名错误
//	EC_NickName_IsExists,//用户名已经存在
//	EC_MailContext_StringError,//邮件内容错误
//
//	EC_ResetAccount_Error,//非游客账号 无法重置账号
//	EC_ResetAccount_ServerError,//服务器告诉客户端 无法重置账号
//
//	EC_Max = 0xff,
//};

struct StringArrayData
{
	//多个变长的string的处理
	USHORT		Length;
	TCHAR		Array[1];//字符串
};
struct StringArrayDataByChar
{
	USHORT		Length;
	char		Array[1];//字符串
};
struct ChannelUserInfo
{
	//TCHAR			uid[32];
	//TCHAR			channelLabel[32];//渠道的名称
	//TCHAR			channeluid[32];//渠道用户的ID
	//TCHAR			productCode[32];
	//TCHAR			channelCode[32];
	//TCHAR			UserName[32];//用户名称
	//TCHAR			token[32];//渠道令牌
	BYTE			HandleSum;
	DWORD			Sum;
	BYTE			Array[1];
};

template<typename T>
void GetStringArrayVecByData(vector<StringArrayData*>& pVec, T* pData)
{
	pVec.clear();
	if (pData->Sum == 0)
		return;
	DWORD BeginIndex = 0;
	while (pData->Sum - BeginIndex >= (sizeof(StringArrayData)-sizeof(TCHAR)) && pVec.size() < pData->HandleSum)
	{
		StringArrayData* p = (StringArrayData*)&pData->Array[BeginIndex];
		USHORT Length = p->Length;
		pVec.push_back(p);
		BeginIndex += (Length + sizeof(USHORT));
	}
};
template<typename T>
void GetStringArrayVecByData(vector<TCHAR*>& pVec, T* pData)
{
	pVec.clear();
	if (pData->Sum == 0)
		return;
	DWORD BeginIndex = 0;
	while (pData->Sum - BeginIndex >= (sizeof(StringArrayData)-sizeof(TCHAR)) && pVec.size() < pData->HandleSum)
	{
		StringArrayData* p = (StringArrayData*)&pData->Array[BeginIndex];
		USHORT Length = p->Length;

		TCHAR* pStr = (TCHAR*)malloc(Length+sizeof(TCHAR));
		memcpy_s(pStr, Length, p->Array, Length);
		pStr[Length / sizeof(TCHAR)] = 0;
		pVec.push_back(pStr);
		BeginIndex += (Length + sizeof(USHORT));
	}
};
template<typename T>
void GetStringArrayVecByData(vector<char*>& pVec, T* pData)
{
	pVec.clear();
	if (pData->Sum == 0)
		return;
	DWORD BeginIndex = 0;
	while (pData->Sum - BeginIndex >= (sizeof(StringArrayDataByChar)-sizeof(char)) && pVec.size() < pData->HandleSum)
	{
		StringArrayDataByChar* p = (StringArrayDataByChar*)&pData->Array[BeginIndex];
		USHORT Length = p->Length;

		char* pStr = (char*)malloc(Length + sizeof(char));
		memcpy_s(pStr, Length, p->Array, Length);
		pStr[Length / sizeof(char)] = 0;
		pVec.push_back(pStr);
		BeginIndex += (Length + sizeof(USHORT));
	}
};
template<typename T>
void FreeVec(vector<T>& pVec)
{
	if (pVec.empty())
		return; 
	vector<T>::iterator Iter = pVec.begin();
	for (; Iter != pVec.end(); ++Iter)
	{
		free(*Iter);
	}
	pVec.clear();
}


struct tagRoleServerInfo
{
	//玩家身上的属性 但是不发送到客户端去
	//总捕鱼金币收益
	int64			TotalFishGlobelSum;

	//新手保护功能
	BYTE			RoleProtectSum;//保护次数
	__int64			RoleProtectLogTime;//保护时间

	//玩家二级密码
	DWORD			SecPasswordCrc1;
	DWORD			SecPasswordCrc2;
	DWORD			SecPasswordCrc3;
};

struct tagSystemMailRecord
{
	int MailID;
	DWORD EndTime;
};

#define MAX_SYSTEM_RECORD_COUT 100
struct tagRoleInfo
{
	DWORD			dwUserID;
	TCHAR			NickName[MAX_NICKNAME + 1];
	WORD			wLevel;//玩家点击
	DWORD			dwExp;//玩家经验
	DWORD			dwFaceID;//玩家头像ID
	bool			bGender; //玩家性别
	DWORD			dwGlobeNum;
	DWORD           dwMedalNum;
	DWORD			dwCurrencyNum;
	DWORD			dwAchievementPoint;//玩家的成就点数
	BYTE			TitleID;
	//玩家每天的数据
	DWORD			dwProduction;//当天获得的金币数量
	DWORD			dwGameTime;//当天的游戏时间
	BYTE			SendGiffSum;//发送赠送的次数 当天
	BYTE			AcceptGiffSum;//接收赠送的次数 当天
	//玩家的 一些状态
	int256			TaskStates;
	int256			AchievementStates;
	int256			ActionStates;
	//每天在线的分钟数
	WORD			OnlineMin;
	DWORD			OnlineRewardStates;//玩家当前领取奖励的记录 最大 32个 
	//玩家的魅力数据
	DWORD			CharmArray[MAX_CHARM_ITEMSUM];//魅力值对应的物品
	//判断账号是否可以修改
	bool			IsCanResetAccount;
	//玩家排名
	DWORD			AchievementPointIndex;//玩家成就点数排名
	//玩家IP地址
	DWORD			ClientIP;//玩家IP
	TCHAR			IPAddress[MAX_ADDRESS_LENGTH_IP+1];
	//签到数据
	DWORD			CheckData;//签到的数据 1-31
	bool			IsShowIPAddress;//默认值为true
	//兑换码状态
	DWORD			ExChangeStates;
	//总充值数
	DWORD			TotalRechargeSum;
	//首冲
	bool			bIsFirstPayGlobel;//是否首冲金币
	bool			bIsFirstPayCurrcey;//是否首冲乐币
	//新的
	DWORD			LotteryScore;//奖金
	BYTE			LotteryFishSum;//奖金鱼的数量
	//Vip数据
	BYTE			VipLevel;//不涉及数据库的
	//月卡数据
	BYTE			MonthCardID;
	__int64			MonthCardEndTime;
	__int64			GetMonthCardRewardTime;//最后次领取月卡物品的时间
	//倍率
	int256			RateValue;//开启的倍率的数值
	//
	BYTE			CashSum;//兑换次数
	//
	BYTE			benefitCount;//领取数量
	DWORD			benefitTime;  //最近一次时间
	//bool			bpoor;
	//总兑换的奖牌
	DWORD			TotalUseMedal;
	//锁定数据
	//DWORD			LockStates;
	//__int64			LockEndTime;

	//
	DWORD			ParticularStates;//玩家的一些特殊状态
	//
	DWORD			GameID;//玩家在注册的时候 生成的不变的GameID 

	//是否已经分享
	bool			bShareStates;
	//兑换次数
	DWORD			TotalCashSum;//总兑换次数
	tagSystemMailRecord SystemMailRecord[MAX_SYSTEM_RECORD_COUT];
	DWORD			ChannelID;
};
//玩家现实地址
struct tagRoleAddressInfo
{
	TCHAR			Name[MAX_NAME_LENTH + 1];//姓名
	unsigned __int64			Phone;//手机号码
	TCHAR			Email[MAX_EMAIL_LENGTH + 1];//邮箱
	TCHAR			IdentityID[MAX_IDENTITY_LENGTH + 1];//身份证号码
	TCHAR			EntityItemUseName[MAX_NAME_LENTH + 1];//收货人的名称
	unsigned __int64 EntityItemUsePhone;//收货人的手机号码
	TCHAR			EntityItemUseAddres[MAX_ADDRESS_LENGTH + 1];//收货人的现实地址
};

struct tagRemoteRoleInfo
{
	BYTE			SeatID;
	DWORD			dwUserID;
	TCHAR			NickName[MAX_NICKNAME + 1];
	WORD			wLevel;//玩家点击
	//DWORD			dwExp;//玩家经验
	DWORD			dwFaceID;//玩家头像ID
	bool			bGender; //玩家性别
	DWORD			dwGlobeNum;
	DWORD			CharmArray[MAX_CHARM_ITEMSUM];//魅力值对应的物品

	DWORD			dwAchievementPoint;//玩家的成就点数
	BYTE			TitleID;
	//DWORD			ClientIP;//xuda
	TCHAR			IPAddress[MAX_ADDRESS_LENGTH_IP+1];
	BYTE			VipLevel;
	bool			IsInMonthCard;
	DWORD			GameID;
};
struct tagCenterRoleInfo
{
	DWORD			dwUserID;
	TCHAR			NickName[MAX_NICKNAME + 1];
	WORD			wLevel;//玩家点击
	//DWORD			dwExp;//玩家经验
	DWORD			dwFaceID;//玩家头像ID
	BYTE			TitleID;
	DWORD			dwAchievementPoint;
	DWORD			CharmArray[MAX_CHARM_ITEMSUM];//魅力值对应的物品
	bool			bGender; //玩家性别
	DWORD			ClientIP;//玩家IP
	bool			IsShowIpAddress;
	BYTE			VipLevel;
	bool			IsInMonthCard;
	bool			IsOnline;
	DWORD			ParticularStates;
	DWORD			GameID;
};


struct tagQueryRoleInfo
{
	DWORD			dwUserID;
	TCHAR			NickName[MAX_NICKNAME + 1];
	WORD			wLevel;//玩家点击
	DWORD			dwFaceID;//玩家头像ID
	bool			bGender; //玩家性别 n
	bool			bIsOnline;
	DWORD			dwAchievementPoint;//玩家的成就点数
	BYTE			TitleID;
	DWORD			CharmArray[MAX_CHARM_ITEMSUM];//魅力值对应的物品
	DWORD			ClientIP;//玩家IP
	bool			IsShowIpAddress;
	BYTE			VipLevel;
	bool			IsInMonthCard;
	DWORD			GameID;
};
struct tagQueryRoleInfoClient
{
	DWORD			dwUserID;
	TCHAR			NickName[MAX_NICKNAME + 1];
	WORD			wLevel;//玩家点击
	DWORD			dwFaceID;//玩家头像ID
	bool			bGender; //玩家性别 n
	bool			bIsOnline;
	DWORD			dwAchievementPoint;//玩家的成就点数
	BYTE			TitleID;
	DWORD			CharmArray[MAX_CHARM_ITEMSUM];//魅力值对应的物品
	TCHAR			IPAddress[MAX_ADDRESS_LENGTH_IP+1];
	BYTE			VipLevel;
	bool			IsInMonthCard;
	DWORD			GameID;
};

enum ParticularStates
{
	ParticularState_NiuNiuVip = 1,
	ParticularState_DialVip   = 2,
	ParticularState_CarVip    = 4,
};

struct tagMiniGameRoleInfo
{
	DWORD		dwUserID;
	TCHAR		NickName[MAX_NICKNAME + 1];
	DWORD       FaceID;
	DWORD		dwGlobelSum;
	DWORD       dwMadleSum;
	DWORD		dwCurrceySum;
	BYTE		VipLevel;
	BYTE		GameConfigID;
	DWORD		ParticularStates;
	DWORD		GameID;
};

struct RechageInfo
{
	DWORD			UserID;//玩家ID
	DWORD			Price;//价格 单位分
	DWORD			FreePrice;
	DWORD			ShopItemID;//参数 当前代表 购买的商品的ID
	//TCHAR			orderid[32];
	//TCHAR			ChannelCode[32];//渠道的名称
	//TCHAR			channelOrderid[32];//渠道用户的ID
	//TCHAR			channelLabel[32];
	BYTE			HandleSum;
	DWORD			Sum;
	BYTE			Array[1];
};

enum EItemType
{
	IT_Globel   = 1,//金币 
	IT_Medal    = 2,//奖牌
	IT_AchievementPoint = 3,//成就点
	IT_Title    = 4,//称号
	IT_Currey   = 5,//乐币
	IT_MonthSocre = 6,//比赛积分
	IT_MonthGlobel=7,//比赛金币
	IT_Normal	  =8,//普通物品
	IT_Skill      =9,//技能物品
	IT_Package    =10,//礼包物品
	IT_Charm      =11,//魅力物品
	IT_Giff		  =12,//赠送物品
	IT_Cannon     =13,//炮台物品
	IT_OsCar	  =14,//潜艇物品
	IT_Entity	  =15,//实体物品 现实的物品
	IT_MonthCard  =16,//月卡物品
	IT_GlobelBag  =17,//聚宝盆物品
	IT_Horn		  =18,//喇叭

};
struct tagItemConfig
{
	DWORD	ItemID;//物品ID
	BYTE    ItemType;//物品类型
	DWORD   ItemParam;//物品的数值  特殊处理用的 可以没有 没有为0
	TCHAR	ItemName[256];
	TCHAR	ItemDesc[256];
};

struct tagItemOnce  //配置文件使用的数据 
{
	DWORD		ItemID;
	DWORD		ItemSum;//物品数量修改
	DWORD		LastMin;//物品程序的分钟
};
struct tagItemInfo//玩家具体的数据
{
	DWORD		ItemOnlyID;//物品的唯一ID  主键ID
	DWORD		ItemID;//物品ID
	DWORD		ItemSum;//物品数量
	__int64     EndTime;//物品结束时间
};
enum RelationType
{
	RT_Friend = 1,
	RT_Black = 2,
};
struct tagRoleRelation
{
	//一条关系数据
	DWORD			dwDestRoleID;
	DWORD			dwFaceID;
	WORD			wLevel;
	bool			bGender;
	//bool			bIsBe;
	//BYTE			bLastLogonTime;//最后次在线时间
	__int64			LastLogonTime;
	bool			IsOnline;
	TCHAR			DestNickName[MAX_NICKNAME + 1];
	BYTE			bRelationType;//关系类型
	BYTE			TitleID;
	DWORD			dwAchievementPoint;
	DWORD			CharmArray[MAX_CHARM_ITEMSUM];//魅力值对应的物品
	DWORD			ClientIP;//玩家IP
	bool			IsShowIpAddress;
	BYTE			VipLevel;
	bool			IsInMonthCard;
	DWORD			GameID;
};
struct tagBeRoleRelation//被动关系数据
{
	DWORD			dwUserID;
	BYTE			bRelationType;
};
struct tagRoleRelationClient
{ 
	DWORD			dwDestRoleID;
	DWORD			dwFaceID;
	WORD			wLevel;
	bool			bGender;
	BYTE			bDiffDay;//在线天数差
	TCHAR			DestNickName[MAX_NICKNAME + 1];
	BYTE			bRelationType;//关系类型
	BYTE			TitleID;
	DWORD			dwAchievementPoint;
	DWORD			CharmArray[MAX_CHARM_ITEMSUM];//魅力值对应的物品
	//DWORD			ClientIP;//玩家IP //xuda
	TCHAR			IPAddress[MAX_ADDRESS_LENGTH_IP+1];
	BYTE			VipLevel;
	bool			IsInMonthCard;
	DWORD			GameID;
};
//服务器端需要一个统一的邮件格式
struct tagRoleMail   //从数据库读取出来的邮件信息
{
	DWORD		MailID;
	DWORD		SrcUserID;//发送方的ID
	DWORD		SrcFaceID;
	TCHAR		SrcNickName[MAX_NICKNAME + 1];
	__int64		SendTimeLog;//发送时间
	TCHAR		Context[MAX_MAIL_CONTEXT + 1];//邮件内容
	WORD		RewardID;//奖励 奖励
	DWORD		RewardSum;//奖励的数量
	bool		bIsRead;//是否已经阅读
	bool		bIsExistsReward;//是否存在奖励
};




struct tagNormalMailTitle
{
	DWORD		MailID;
	DWORD		SrcUserID;//发送方的ID
	DWORD		SrcFaceID;
	TCHAR		SrcNickName[MAX_NICKNAME + 1];
	BYTE		bDiffTime;//在线天数差
	bool		bIsRead;
};
struct tagNormalMail
{
	DWORD		MailID;
	DWORD		SrcUserID;//发送方的ID
	DWORD		SrcFaceID;
	TCHAR		SrcNickName[MAX_NICKNAME + 1];
	TCHAR		Context[MAX_MAIL_CONTEXT + 1];//邮件内容
	BYTE		bDiffTime;//在线天数差
	bool		bIsRead;
};
struct tagSystemMailTitle
{
	DWORD		MailID;
	BYTE		bDiffTime;//在线天数差
	bool		bIsRead;//是否已经阅读
	bool		bIsExistsItem;//是否存在附件
};
struct tagSystemMail
{
	DWORD		MailID;
	TCHAR		Context[MAX_MAIL_CONTEXT + 1];//邮件内容
	WORD		RewardID;
	DWORD		RewardSum;//奖励的数量
	BYTE		bDiffTime;//在线天数差
	bool		bIsRead;//是否已经阅读
	bool		bIsExistsReward;//是否存在奖励
};


struct tagOperatorSystemMail
{
	TCHAR		Context[MAX_MAIL_CONTEXT + 1];//邮件内容
	WORD		RewardID;
	DWORD		RewardSum;//奖励的数量
	
	DWORD	BeginTime;
	DWORD   EndTime;
	DWORD   ID;
};

//签到
//struct tagRoleCheckInfo
//{
//	//BYTE		bMontheCheckSum;//本月签到的信息记录 0-30天  每天是否签到
//	//BYTE		bCheckSum;//连续签到的次数 不随本月的信息修改
//	DWORD		MonthCheckInfo;//本月签到的数据
//	BYTE		bLastLogYear;//最后次签到的 年 月 日
//	BYTE		bLastLogMonth;
//	BYTE		bLastLogDay;
//};
//任务的事件类型
enum EventTargetType  //被其他地方调用的钩子 必须与玩家相关的数据
{
	ET_GetGlobel = 1,//获得金币 累加的
	ET_GetMadel = 2,//获得奖牌 累加
	ET_GetCurren = 3,//获得乐币 累加
	ET_UpperLevel = 4,//提升等级 累加等级
	ET_CatchFish = 5,//捕获鱼   绑定参数0 表示任意鱼
	ET_SendGiff = 6,//发送赠送 
	ET_UseSkill = 7,//使用技能
	ET_LauncherGlobel = 8,//炮台获得金币
	ET_MaxGlobelSum = 9,//拥有的最大金币数量 需要修改 最大金币数量 应该带进度的
	ET_ToLevel = 10,//到指定等级
	ET_MaxCurren = 11,//最大拥有乐币  
	ET_Recharge  = 12,//充值人民币事件
	ET_Max,
};

struct EventInfo  //事件 已经事件带的参赛  用于 任务 和 成就
{
	BYTE	OnlyID;//唯一ID 不是事件ID 在同一个节点下不可以重复
	BYTE	EventID;
	DWORD	BindParam;//当前事件绑定的参赛 可以为空
	DWORD	FinishValue;//当前事件的完成参数
};
struct tagRoleTaskInfo//任务的结构数据
{
	BYTE		TaskID;//任务ID 从0到 256  可以直接建立数组
	DWORD		TaskValue;
};
//成就的类型
struct tagRoleAchievementInfo
{
	BYTE		AchievementID;
	DWORD		AchievementValue;
};

//活动的类型
struct tagRoleActionInfo
{
	BYTE		ActionID;
	DWORD		ActionValue;
	DWORD		ActionStates;
};
//比赛
struct tagRoleMonthInfo //玩家的比赛信息
{
	DWORD		dwUserID;
	DWORD		dwIndex;
	BYTE		bMonthID;
	DWORD		dwMonthGlobel;
	DWORD		dwMonthScore;
	BYTE		wUserAddGlobelNum;//续币的次数
	DWORD		SkillSum;//使用技能的次数
};
struct tagRoleMonthCenterInfo
{
	tagRoleMonthInfo		MonthInfo;
	DWORD					FaceID;
	float					VipAddScore;
	TCHAR					NickName[MAX_NICKNAME + 1];	
};
//Rank
struct tagRankInfo
{
	DWORD		dwUserID;
	BYTE		bIndex;
	DWORD		dwFaceID;
	BYTE		TitleID;
	bool		Gender;
	TCHAR		NickName[MAX_NICKNAME + 1];
	__int64		Param;
};

//赠送 giff
struct tagGiffInfo
{
	DWORD		OnlyID;
	DWORD		dwUserID;
	DWORD		dwFaceID;
	TCHAR		NickName[MAX_NICKNAME + 1];
	__int64		SendTime;
};
struct tagGiffClientInfo
{
	DWORD		OnlyID;
	DWORD		dwUserID;
	DWORD		dwFaceID;
	TCHAR		NickName[MAX_NICKNAME + 1];
	BYTE		DiffTime;//0-250
};


//配置文件结构体
//签到
struct RoleCheckConfig
{
	DWORD			CheckOtherUser;//补签需要的花费
	DWORD			MaxCheckDaySum;
	HashMap<DWORD, WORD> CheckDayReward;
	HashMap<DWORD, WORD>  MonthCheckReward;
};

//任务的配置文件属性
struct tagTaskConfig
{
	BYTE		TaskID;	//任务ID 
	//任务条件是从分组中提取出 TASK_MAXEVENTSUM 个随机的条件 
	EventInfo	EventInfo;
	BYTE		UpperTaskID; //上级任务ID
	bool		IsDayClear; //是否为每日任务
	//DWORD		RewardGlobel;//奖励的金钱
	//DWORD		RewardMabel;//奖励的奖牌
	//DWORD		RewardCurreny;//奖励的乐币
	WORD		RewardID;
	BYTE		GroupID;//同一分组下的任务互斥 并且 只有每日任务才可以进行分组
	BYTE		JoinLevel;
	vector<BYTE> LowerTaskVec;//下级的任务ID列表
};
struct tagTaskMap
{
	BYTE							m_MaxJoinTaskSum;
	HashMap<BYTE, tagTaskConfig>  m_TaskMap;
	HashMap<BYTE, std::vector<BYTE>> m_TaskGroup;
};
//成就配置
struct tagAchievementConfig
{
	BYTE		AchievementID;
	EventInfo	AchievementEventInfo;
	BYTE		UpperAchievementID;
	WORD		RewardID;
	BYTE		JoinLevel;
	vector<BYTE> LowerAchievementVec;
};
struct tagAchievementMap
{
	BYTE								m_MaxJoinAchievementSum;
	HashMap<BYTE, tagAchievementConfig>	m_AchievementMap;
};
//活动
struct tagActionEventConfig
{
	DWORD			ID;//阶段的唯一ID
	//DWORD			BindParam;
	DWORD			FinishValue;
	WORD			RewardID;
};
struct tagActionConfig //一个系列的活动  包括多个事件 已经不同的奖励
{
	BYTE		ActionID;
	BYTE		EventID;//一个活动  一个事件
	DWORD		BindParam;
	DWORD		FinishValue;//当获得完成的总的值
	HashMap<DWORD, tagActionEventConfig> EventMap;

	__int64		BeginTime;
	WORD		LastHour;

	bool IsInTime()
	{
		if (LastHour == 0)//持续小时为0的时候 永远保持激活状态
			return true;
		time_t now = time(NULL);
		if (now >= BeginTime && now <= BeginTime + LastHour * 3600)
			return true;
		else
			return false;
	}
};
struct tagActionGroupMap
{
	BYTE									m_MaxJoinActionSum;
	HashMap<BYTE, tagActionConfig>			m_ActionMap;
};
//比赛配置
struct tagMonthConvert
{
	BYTE		ConvertSum;
	DWORD		LostGlobel;
	DWORD		LostMadle;
	DWORD		LostCurrey;
	DWORD		AddMonthGlobel;
};
struct tagMonthTime
{
	BYTE		ID;//标记当前的阶段
	//开始阶段
	BYTE		BeginWeek;
	BYTE		BeginHour;
	BYTE		BeginMin;
	//WORD		LastMin;
	//阶段的持续时间
	WORD		AllLastMin;//这个节点持续时间
	//阶段的等待时间
	WORD		WriteLastMin;//下一个阶段的开始时间
	//报名阶段
	WORD		SignWriteMin;
	WORD		SignLastMin;
	//开始阶段
	WORD		StarWriteMin;
	WORD		SratLastMin;
	//
	bool		IsContinue;
	WORD		RunSum;//循环的次数

	__int64     GetBeginTime()
	{
		//获取开始时间
		time_t Now = time(NULL);
		tm NowTime;
		errno_t Err = localtime_s(&NowTime, &Now);
		if (Err != 0)
		{
			ASSERT(false);
			return 0;
		}
		if (BeginWeek == 8)
		{
			//8 表示每一天 都可以 
			int DiffHour = NowTime.tm_hour - BeginHour;
			int DiffMin = NowTime.tm_min - BeginMin;
			int DiffSec = DiffHour * 3600 + DiffMin * 60;
			if (DiffSec >= 0)
				return (Now - DiffSec);//获取上次的开始时间
			else
			{
				//昨天的 
				DiffSec += 24 * 3600;
				return (Now - DiffSec);//获取上次的开始时间
			}
		}
		else
		{
			int DiffDay = NowTime.tm_wday - BeginWeek;//( < 0) ? (7 + NowTime.tm_wday - BeginWeek) : (NowTime.tm_wday - BeginWeek);//差距的天数
			int DiffHour = NowTime.tm_hour - BeginHour;
			int DiffMin = NowTime.tm_min - BeginMin;
			int DiffSec = DiffDay * 24 * 3600 + DiffHour * 3600 + DiffMin * 60;
			//修改下 
			if (DiffSec < 0)
			{
				DiffSec = 7 * 3600 * 24 + DiffSec;
			}
			return (Now - DiffSec);//获取上次的开始时间
		}
		
	}
	DWORD		IsInMonthTime()
	{
		time_t Now = time(NULL);//当前时间
		time_t BeginTime = GetBeginTime();//比赛的开始时间
		if (BeginTime > Now)
		{
			ASSERT(false);
			return 0;
		}
		//有些比赛不是循环的 我们想要判断
		if (!IsContinue)
		{
			//一次性的时间判断函数
			if (Now < (BeginTime + AllLastMin*60))
			{
				return static_cast<DWORD>(ID << 16);
			}
			else
				return 0u;
		}
		DWORD DiffSec = static_cast<DWORD>(Now - BeginTime);
		//一个完整阶段的时间 AllLastMin + WriteLastMin
		DWORD nValue = (DiffSec/60) % (AllLastMin + WriteLastMin);

		//DWORD ContinueSum = (nValue == 0 ? ((DiffSec / 60) / (AllLastMin + WriteLastMin)) : ((DiffSec / 60) / (AllLastMin + WriteLastMin) + 1));//循环的次数
		DWORD ContinueSum = (DiffSec / 60) / (AllLastMin + WriteLastMin);
		if (ContinueSum >= RunSum)
		{
			//已经超过了 不在循环
			return false;
		}
		else if (nValue < AllLastMin)
		{
			WORD Sum = static_cast<WORD>((DiffSec / 60) / (AllLastMin + WriteLastMin));
			return static_cast<DWORD>((ID << 16) + Sum);
		}
		else if (nValue >= static_cast<DWORD>(AllLastMin) && nValue < static_cast<DWORD>(AllLastMin + WriteLastMin))
			return 0;
		else
		{
			ASSERT(false);
			return 0;
		}
	}
	bool		IsInSignTime()
	{
		//比赛时间 为 [) 不包括 最后一分钟
		//是否在比赛阶段
		time_t Now = time(NULL);//当前时间
		time_t BeginTime = GetBeginTime();//比赛的开始时间
		if (BeginTime > Now)
		{
			ASSERT(false);
			return false;
		}
		if (!IsContinue)
		{
			//一次性的时间判断函数
			if (Now < (BeginTime + AllLastMin * 60))
			{
				if (Now >= (BeginTime + SignWriteMin * 60) && Now < (BeginTime + SignWriteMin * 60 + SignLastMin * 60))
					return true;
				else
					return false;
			}
			else
				return false;
		}
		DWORD DiffSec = static_cast<DWORD>(Now - BeginTime);
		//一个完整阶段的时间 AllLastMin + WriteLastMin
		DWORD nValue = (DiffSec / 60) % (AllLastMin + WriteLastMin);
		//DWORD ContinueSum = (nValue == 0 ? ((DiffSec / 60) / (AllLastMin + WriteLastMin)) : ((DiffSec / 60) / (AllLastMin + WriteLastMin) + 1));//循环的次数
		DWORD ContinueSum = (DiffSec / 60) / (AllLastMin + WriteLastMin);
		if (ContinueSum >= RunSum)
		{
			//已经超过了 不在循环
			return false;
		}
		else if (nValue < AllLastMin)
		{
			//在比赛阶段 继续判断
			if (nValue >= static_cast<DWORD>(SignWriteMin) && nValue < static_cast<DWORD>(SignWriteMin + SignLastMin))
				return true;
			else
				return false;
		}
		else if (nValue >= static_cast<DWORD>(AllLastMin) && nValue < static_cast<DWORD>(AllLastMin + WriteLastMin))
			return false;
		else
		{
			ASSERT(false);
			return false;
		}
	}
	bool		IsInStarTime()
	{
		//是否在比赛阶段
		time_t Now = time(NULL);//当前时间
		time_t BeginTime = GetBeginTime();//比赛的开始时间
		if (BeginTime > Now)
		{
			ASSERT(false);
			return false;
		}
		if (!IsContinue)
		{
			//一次性的时间判断函数
			if (Now < (BeginTime + AllLastMin * 60))
			{
				if (Now >= (BeginTime + StarWriteMin * 60) && Now < (BeginTime + StarWriteMin * 60 + SratLastMin * 60))
					return true;
				else
					return false;
			}
			else
				return false;
		}
		DWORD DiffSec = static_cast<DWORD>(Now - BeginTime);
		//一个完整阶段的时间 AllLastMin + WriteLastMin
		DWORD nValue = (DiffSec / 60) % (AllLastMin + WriteLastMin);
		//DWORD ContinueSum = (nValue == 0 ? ((DiffSec / 60) / (AllLastMin + WriteLastMin)) : ((DiffSec / 60) / (AllLastMin + WriteLastMin) + 1));//循环的次数
		DWORD ContinueSum = (DiffSec / 60) / (AllLastMin + WriteLastMin);
		if (ContinueSum >= RunSum)
		{
			//已经超过了 不在循环
			return false;
		}
		else if (nValue < AllLastMin)
		{
			//在比赛阶段 继续判断
			if (nValue >= static_cast<DWORD>(StarWriteMin) && nValue < static_cast<DWORD>(StarWriteMin + SratLastMin))
				return true;
			else
				return false;
		}
		else if (nValue >= static_cast<DWORD>(AllLastMin) && nValue < static_cast<DWORD>(AllLastMin + WriteLastMin))
			return false;
		else
		{
			ASSERT(false);
			return false;
		}
	}
};
struct tagMonthReward
{
	WORD		Index;
	WORD		RewardID;
};
struct tagMonthRobot
{
	DWORD		Level;
	DWORD		RobotSum;
	DWORD		AddScore;
	DWORD		Rate;
	DWORD		MaxScore;
	DWORD		SumRate;
	//技能的配置
	BYTE		SkillSum;//技能使用次数 
	DWORD		SkillUseRate;//每次使用技能的概率
	DWORD		SkillScore;//每次技能增加的积分
	DWORD		SkillScoreRate;//技能积分波动比例
};
struct tagMonthConfig
{
	BYTE		MonthID;
	DWORD		SignGlobel;
	DWORD       SignMadel;
	DWORD		SignCurrey;
	tagItemOnce SignItem;
	DWORD		InitMonthGlobel;
	BYTE		TableTypeID;
	BYTE		RankSum;
	BYTE		MinRateLimit;
	BYTE		TypeID;
	HashMap<DWORD, tagMonthRobot> m_RobotMap;
	HashMap<BYTE, tagMonthConvert> m_ConvertMap;//续币的集合
	//std::vector<tagMonthTime> m_MonthTime;
	HashMap<BYTE, tagMonthTime> m_MonthTime;
	HashMap<WORD, tagMonthReward> m_MonthReward;

	DWORD GetMonthTimeID()
	{
		//获取当前正在进行的比赛时间的唯一ID
		if (m_MonthTime.empty())
			return 0;
		HashMap<BYTE, tagMonthTime>::iterator Iter = m_MonthTime.begin();
		for (; Iter != m_MonthTime.end(); ++Iter)
		{
			DWORD nValue = Iter->second.IsInMonthTime();
			if (nValue != 0)
				return nValue;
		}
		return 0;
	}
};
struct tagMonthMap
{
	BYTE								m_MonthBeginLevel;
	HashMap<BYTE, tagMonthConfig>		m_MonthMap;
};

//排行榜
struct tagRankReward
{
	BYTE		Index;
	WORD		RewardID;
	//tagItemOnce ItemInfo;
};

struct tagRankConfig
{
	BYTE		RankID;
	BYTE		RowCount;
	HashMap<BYTE, tagRankReward> RewardMap;
};
struct tagRankMap
{
	BYTE						   RankWeekDay;
	HashMap<BYTE, tagRankConfig>  m_RankMap;
};
//宝箱
struct tagChestsCost
{
	BYTE					MaxCostNum;
	HashMap<BYTE, DWORD>	CostMap;//抽宝箱的花费
};
struct tagChestsRewardOnce
{
	BYTE					OnceID;
	DWORD					Chances;
	WORD					RewardID;
};
struct tagChestsReward
{
	BYTE					RewardID;
	DWORD					Chances;
	DWORD					MaxOnceChances;
	std::vector<tagChestsRewardOnce> RewardMap;
};
struct tagChestsAllReward
{
	BYTE				 SpecialRewardTypeID;
	BYTE				 SpecialRewardUseRp;
	BYTE				 OtherAddRp;
	DWORD				 MaxRewardChances;
	//std::vector<tagChestsReward> RewardVec;
	HashMap<BYTE, tagChestsReward> RewardMap;
};
struct tagChestConfig
{
	BYTE				ChestID;
	WORD				CatchChance;
	BYTE				ExisteSec;
	WORD				ImmediateRewardid;
	tagChestsCost		CostInfo;
	tagChestsAllReward	RewardInfo;
};
struct tagChestMap
{
	HashMap<BYTE, tagChestConfig>  ChestMap;
};
//魅力配置文件
struct tagCharmOnce
{
	BYTE			Index;
	DWORD			UserGlobel;//消耗的金币
	DWORD			UserMedal;
	DWORD			UserCurrcey;
	DWORD			ItemID;
	DWORD			ItemSum;
	int				ChangeCharmValue;
};
struct tagCharmConfig
{
	BYTE				 CharmMaxItem;
	HashMap<BYTE, tagCharmOnce> CharmIndexMap;
};
//商店的配置文件
struct tagShopItemTimeConfig
{
	__int64 BeginTime;
	WORD	LastHour;
};

enum ShopItemType
{
	SIT_Normal = 0,//普通物品
	SIT_Entity = 1,//实体物品 
	SIT_PhonePay = 2,//手机话费
	SIT_MonthCard=3,//月卡物品
	SIT_GlobelBag = 4,//聚宝盆
};

struct tagShopItemConfig//商品的名称
{
	BYTE		ShopItemIndex;
	tagItemOnce ItemInfo;
	DWORD		PriceGlobel;
	DWORD		PriceMabel;
	DWORD		PriceCurrey;
	//bool		IsEntityItem;//是否为实体的物品
	BYTE		ShopType;//物品的特殊类型
	bool		IsCanPile;//是否可以一次购买多个
	std::vector<tagShopItemTimeConfig> TimeVec;//物品的时间限制 无的话表示无限制
};
struct tagShopConfig
{
	BYTE		ShopID;
	HashMap<BYTE, tagShopItemConfig>  ShopItemMap;
	std::vector<tagShopItemConfig> ShopItemVec;
};


struct tagShopConfigMap
{
	HashMap<BYTE, tagShopConfig>		ShopMap;
	std::vector<tagShopConfig>			ShopVec;
};
//Game
struct tagShopItemRunTimeData
{
	BYTE									ShopID;
	std::vector<DWORD>						ItemVec;
	HashMap<DWORD, bool>					ShopItem;
};
struct tagShopRunTimeData
{
	std::vector<tagShopItemRunTimeData*>		ShopVec;
	HashMap<BYTE, tagShopItemRunTimeData*>		ShopMap;
};
//Giff
struct tagGiffConfig
{
	BYTE			LimitDay;
	BYTE			SendSumLimitByDay;
	BYTE			AcceptSubLimitByDay;
	BYTE			SendOnceUserLimitByDay;
	//DWORD			RewardGlobel;
	//tagItemOnce		ItemInfo;
	WORD			RewardID;
	BYTE			MaxUserGiffSum;
};
//TableConfig
struct tagTableJoinItem
{
	DWORD		ItemID;
	DWORD		ItemSum;
};
struct TableInfo
{
	BYTE		TableTypeID;//座子的ID
	DWORD		MinGlobelSum;
	DWORD		MaxGlobelSum;
	DWORD		MinCurreySum;
	DWORD		MaxCurreySum;
	DWORD		MinLevel;
	DWORD	    MaxLevel;
	BYTE		MinRate;
	BYTE		MaxRate;
	USHORT      TableRate;
	BYTE		wMaxPlayerSum;//桌子最大人数
	TCHAR		cMapName[MAX_MAP_NAME + 1];//地图名称
	vector<tagTableJoinItem> JoinItemVec;//进入房间需要的数据
};
struct tagTableConfig
{
	HashMap<BYTE, TableInfo> m_TableConfig;
};
//Relation
struct tagRelationConfig
{
	BYTE		MaxRelationSum;
	BYTE		MaxRelationRequestSum;
	BYTE		RelationRequestLimitDay;
};
//物品与货币的直接转换
struct tagItemConvert
{
	DWORD		 ItemID;
	DWORD		 Globel;
	DWORD		 Medal;
	DWORD		 Currey;
};
struct tagItemConvertMap
{
	HashMap<DWORD, tagItemConvert>		m_ConvertMap;
};
//服务器关系设置
struct tagFishServerUpdate
{
	BYTE		UpdateHour;
	BYTE		UpdateMin;

	bool  IsChangeUpdate(__int64 LogTime)
	{
		DWORD WriteSec = static_cast<DWORD>(UpdateHour * 3600 + UpdateMin * 60);
		//将时间进行处理
		time_t Now = time(NULL);
		Now -= WriteSec;
		if (Now < 0)
			Now = 0;
		LogTime -= WriteSec;
		if (LogTime < 0)
			LogTime = 0;

		tm NowTime;
		errno_t Err = localtime_s(&NowTime, &Now);
		if (Err != 0)
		{
			ASSERT(false);
			return false; 
		}
		tm LogTm;
		Err = localtime_s(&LogTm, &LogTime);
		if (Err != 0)
		{
			ASSERT(false);
			return false;
		}
		return (LogTm.tm_mday != NowTime.tm_mday || LogTm.tm_mon != NowTime.tm_mon || LogTm.tm_year != NowTime.tm_year);
	}
	bool IsChangeUpdate(__int64 LogTime, __int64 OnLineTime)
	{
		//判断当前时间 和 记录时间 是否穿过了更新时间 判断记录和 当前是否为2天
		DWORD WriteSec = static_cast<DWORD>(UpdateHour * 3600 + UpdateMin * 60);
		//判断修改后的日期是否为同一天 
		OnLineTime -= WriteSec;
		if (OnLineTime < 0)
			OnLineTime = 0;
		LogTime -= WriteSec;
		if (LogTime < 0)
			LogTime = 0;

		tm NowTime;
		errno_t Err = localtime_s(&NowTime, &OnLineTime);
		if (Err != 0)
		{
			ASSERT(false);
			return false;
		}
		tm LogTm;
		Err = localtime_s(&LogTm, &LogTime);
		if (Err != 0)
		{
			ASSERT(false);
			return false;
		}
		return (LogTm.tm_mday != NowTime.tm_mday || LogTm.tm_mon != NowTime.tm_mon || LogTm.tm_year != NowTime.tm_year);
	}
};
struct tagMailConfig
{
	BYTE MailLimitDay;
	//BYTE MailMaxJoinSum;
	BYTE MaxUserMailSum;
	BYTE ClientShowMailSum;
};
//金币商店
//struct tagGlobelShopItem
//{
//	BYTE	ID;
//	DWORD   LostCurrey;
//	DWORD	AddGlobel;
//};
//struct tagGlobelShop
//{
//	HashMap<BYTE, tagGlobelShopItem> m_ShopItem;
//};
//在线奖励
struct tagOnceOnlienReward
{
	BYTE	ID;
	WORD	OnlineMin;
	WORD	RewardID;
	//tagItemOnce ItemInfo;
};
struct tagOnlineReward
{
	HashMap<BYTE, tagOnceOnlienReward> m_OnlineRewardMap;//在线奖励的列表
};

//RoleGameData
struct tagRoleGameData
{
	DWORD			RoleMonthSigupSum;//玩家报名比赛的次数
	DWORD			RoleMonthRewardSum;//玩家获得比赛奖励的次数
	DWORD			RoleMonthFirstSum;//获得比赛第一名的次数
	DWORD			RoleMonthSecondSum;//获得比赛第二名的次数
	DWORD			RoleMonthThreeSum;//获得比赛第三名的次数
	DWORD			RoleCatchFishTotalSum;//玩家捕获鱼的总数
	DWORD			RoleGetGlobelSum;//玩家获得总金币数

	DWORD			NonMonthGameSec;//玩家进行非比赛游戏时间的秒杀
	DWORD			TotalGameSec;//玩家进行总游戏时间的秒数

	//DWORD			CatchFishOneSum;//玩家捕获指定类型鱼的数量
	//DWORD			CatchFishTwoSum;
	//DWORD			CatchFishThreeSum;

	DWORD			CatchFishSum_9;
	DWORD			CatchFishSum_18;
	DWORD			CatchFishSum_20;
	DWORD			CatchFishSum_1;
	DWORD			CatchFishSum_3;
	DWORD			CatchFishSum_19;

	DWORD			MaxCombSum;//最大连击数

	tagRoleGameData()
	{
		RoleMonthSigupSum = 0;
		RoleMonthRewardSum = 0;
		RoleMonthFirstSum = 0;
		RoleMonthSecondSum = 0;
		RoleMonthThreeSum = 0;
		RoleCatchFishTotalSum = 0;
		RoleGetGlobelSum = 0;
		NonMonthGameSec = 0;
		TotalGameSec = 0;
		/*CatchFishOneSum = 0;
		CatchFishTwoSum = 0;
		CatchFishThreeSum = 0;*/
		MaxCombSum = 0;

		CatchFishSum_9 = 0;
		CatchFishSum_18 = 0;
		CatchFishSum_20 = 0;
		CatchFishSum_1 = 0;
		CatchFishSum_3 = 0;
		CatchFishSum_19 = 0;
	}
};

//礼包 
struct tagFishPackageItem
{
	WORD		RewardID;
	DWORD		RankValue;

	tagFishPackageItem()
	{
		RewardID = 0;
		RankValue = 0;
	}
};
struct tagFishPackageGroup
{
	BYTE			GroupID;
	DWORD			MaxRankValue;
	//礼包的项  金币 奖牌 宝石 物品 4种类型
	vector<tagFishPackageItem> PackageItemVec;//只可以从中产生一个物品
};
struct tagFishPackage
{
	DWORD			ItemID;
	bool			IsAutoOpen;
	vector<tagFishPackageGroup>	ItemVec;//宝箱里包含的物品
};
struct tagFishPckageMap
{
	HashMap<DWORD, tagFishPackage>	m_PackageMap;//全部宝箱的集合
};
//全局奖励控制
struct tagRewardOnce
{
	WORD		RewardID;
	vector<tagItemOnce> RewardItemVec;
};
struct tagRewardMap
{
	HashMap<WORD, tagRewardOnce> RewardMap;
};
struct tagFishSystemInfo//系统信息
{
	//服务器系统配置
	DWORD		VersionID;//版本ID
	DWORD		PathCrc;//路径CRC
	DWORD		CacheUserMin;//缓存的分钟
	DWORD		AchievementRankSum;//成就排行榜的数量
	DWORD		RsgInitGlobelSum;//初始化玩家金币数
	DWORD		AnnouncementSum;//公告最多的条数
	WORD		FirstBindPhoneRewardID;//首次绑定手机的奖励
	DWORD		UserQueueHandleSumBySec;//服务器每秒处理的玩家登陆的个数
	DWORD		MaxGobelSum;//42Y 玩家最大金币上限
	DWORD		ChangeNickNameNeedItemID;//修改名称物品
	DWORD		ChangeNickNameNeedItemSum;
	DWORD		ChangeGenderNeedItemID;//修改性别物品
	DWORD		ChangeGenderNeedItemSum;
	BYTE		LeaveTableNonLauncherMin;//踢出桌子的未操作时间
	DWORD		InitRateValue;//初始化的时候 玩家默认的倍率状态
	WORD		EmailGlobelRewardID;
	WORD		EmailCurrceyRewardID;
	WORD		EmailMedalRewradID;
	BYTE		RoleSaveMin;
	__int64		RateInitRewardTime;
	DWORD		SendMessageItemID;
	WORD		SendMessageItemSum;
};
enum RechargeType
{
	RT_FirstCurrcey = 1,//首冲钻石
	RT_FirstGlobel = 2,//首冲金币
	RT_Currcey = 3,//充值钻石
	RT_Globel = 4,//充值金币
	RT_Reward = 5,//充值奖励ID
};
//人民币转 乐币操作
struct tagFishRechargeInfo
{
	DWORD		ID;
	DWORD		dDisCountPrice;//充值 单位元
	DWORD		dPreDisCountPrice;//折扣前的价格 单位元
	WORD		RewardID;
	BYTE		RechargeType;//充值类型
	DWORD		AddMoney;//添加货币类型
	WCHAR       Name[MAX_OperatorTitleLength];
	WCHAR       PayNO[MAX_PayNOLength];

	WCHAR		Icon[MAX_IconLength];
	WCHAR		DisCountPicName[MAX_DisCountPicNameLength];
	
	/*bool		IsCurreyOrGlobel;
	bool		IsFirstPay;*/

	bool IsAddGlobel()
	{
		if (RechargeType == RT_FirstGlobel || RechargeType == RT_Globel)
			return true;
		else
			return false;
	}
	bool IsAddCurrcey()
	{
		if (RechargeType == RT_FirstCurrcey || RechargeType == RT_Currcey)
			return true;
		else
			return false;
	}
	bool IsAddReward()
	{
		if (RechargeType == RT_Reward)
			return true;
		else
			return false;
	}
	bool IsFirstAdd()
	{
		if (RechargeType == RT_FirstCurrcey || RechargeType == RT_FirstGlobel)
			return true;
		else
			return false;
	}
};
struct tagFishRechargesMap
{
	HashMap<DWORD, tagFishRechargeInfo> m_FishRechargeMap;
};
struct tagErrorStr
{
	TCHAR		ErrorStr[MAX_ERRORSTR_LENGTH + 1];
};
struct tagErrorMap
{
	vector<tagErrorStr>	ErrorStrVec;
};
//Announcement
struct AnnouncementOnce //公告分很多类型
{
	//一条公告
	//TCHAR    AnnouncementStr[MAX_Announcement_Length]; //公告的字符串
	TCHAR		NickName[MAX_NAME_LENTH + 1];
	BYTE		ShopID;
	BYTE		ShopOnlyID;
};

//LevelReward
struct LevelRewardMap
{
	HashMap<WORD, WORD>  m_LevelRewardMap;
};

//ExChange
struct tagExChange
{
	BYTE			ID;
	BYTE			TypeID;
	DWORD			ChannelID;
	WORD			RewardID;
};

struct tagOperatorShopConfig
{
	BYTE			ID;
	WCHAR           PayNO[MAX_PayNOLength];
	WCHAR           Title[MAX_OperatorTitleLength];
	WCHAR			IconResource[MAX_IconLength];
	int				ItemID;
	int				ItemSum;
	int				Price;
	int				Grid;
	int				Channel;
};

struct tagOperatorShopCongfigMap
{
	HashMap<int, HashMap<int, tagOperatorShopConfig>> ChannelOperatorShopMap;
};


struct tagExChangeMap
{
	HashMap<BYTE, tagExChange> m_ExChangeMap;
};

struct tagRoleProtectConfig
{
	BYTE			ProtectSum;
	BYTE			ProtectCDMin;
	DWORD			AddGlobel;
	DWORD			MaxGlobelSum;
	DWORD			OpenProtectGlobelSum;
	HashMap<BYTE, BYTE> TableMap;//允许的桌子的类型的集合
};

struct tagNotice
{
	DWORD			ID;
	time_t			BeginTime;
	DWORD			StepNum;
	DWORD			StepMin;
	BYTE			OnceStepNum;
	BYTE			OnceStepSec;
	DWORD			MessageInfoColor;
	TCHAR			MessageInfo[MAX_MESSAGE_LENGTH+1];
};

struct tagLotteryReward
{
	DWORD		Rate;
	WORD		RewardID;
};
struct tagLotteryOnce
{
	BYTE  LotteryID;
	BYTE  UpperLotteryID;
	DWORD NeedUseScore;
	vector<tagLotteryReward> RewardVec;
	DWORD TotalRate;
};
struct tagLotteryConfig
{
	BYTE						  MaxLotteryFishSum;//最大次数
	HashMap<BYTE, tagLotteryOnce> LotteryMap;
	HashMap<BYTE, DWORD>	      FishScore;

	DWORD GetFishLotteryScore(BYTE FishTypeID,BYTE LotteryFishSum)
	{
		if (LotteryFishSum >= MaxLotteryFishSum)
			return 0;
		HashMap<BYTE, DWORD>::iterator Iter= FishScore.find(FishTypeID);
		if (Iter == FishScore.end())
			return 0;
		else
			return Iter->second;
	}
	
};

struct tagVipOnce
{
	BYTE VipLevel;//VIP等级
	BYTE UpperVipLevel;//下一级的VIP等级索引
	DWORD NeedRechatgeRMBSum;//当前等级需要的人民币数字
	BYTE LauncherReBoundNum;//增加子弹反弹次数
	//bool IsCanLauncherLocking;//是否可以使用炮台锁定
	BYTE AddAlmsSum;//增加救济金的次数
	DWORD AddAlmsRate;//增加救济金的百分比例
	DWORD AddMonthScoreRate;//增加比赛积分的百分比例
	DWORD AddReChargeRate;//充值提升的百分比
	DWORD AddCatchFishRate;//捕鱼概率提升
	BYTE  AddUseMedalSum;//兑换次数提升
	std::set<BYTE> CanUseLauncherMap;//VIP可以使用的炮的集合
};
struct tagVipConfig
{
	//默认值
	BYTE DefaultLauncherReBoundNum;//增加子弹反弹次数
	BYTE DefaultAlmsSum;//增加救济金的次数
	BYTE DefaultUseMedalSum;//默认兑换次数
	HashMap<BYTE, tagVipOnce> VipMap;

	BYTE GetLauncherReBoundNum(BYTE VipLevel)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = VipMap.find(VipLevel);
		if (Iter == VipMap.end())
		{
			return DefaultLauncherReBoundNum;
		}
		return DefaultLauncherReBoundNum + Iter->second.LauncherReBoundNum;
	}
	BYTE GetUseMedalSum(BYTE VipLevel)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = VipMap.find(VipLevel);
		if (Iter == VipMap.end())
		{
			return DefaultUseMedalSum;
		}
		return DefaultUseMedalSum + Iter->second.AddUseMedalSum;
	}
	/*bool  IsCanLauncherLocking(BYTE VipLevel)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = VipMap.find(VipLevel);
		if (Iter == VipMap.end())
		{
			return false;
		}
		return Iter->second.IsCanLauncherLocking;
	}*/
	BYTE AddAlmsSum(BYTE VipLevel)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = VipMap.find(VipLevel);
		if (Iter == VipMap.end())
		{
			return DefaultAlmsSum;
		}
		return DefaultAlmsSum + Iter->second.AddAlmsSum;
	}
	float AddAlmsRate(BYTE VipLevel)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = VipMap.find(VipLevel);
		if (Iter == VipMap.end())
		{
			return 1.0f;
		}
		return (Iter->second.AddAlmsRate + 100) / 100.0f;
	}
	float AddMonthScoreRate(BYTE VipLevel)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = VipMap.find(VipLevel);
		if (Iter == VipMap.end())
		{
			return 1.0f;
		}
		return (Iter->second.AddMonthScoreRate + 100) / 100.0f;
	}
	float AddReChargeRate(BYTE VipLevel)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = VipMap.find(VipLevel);
		if (Iter == VipMap.end())
		{
			return 1.0f;
		}
		return (Iter->second.AddReChargeRate + 100) / 100.0f;
	}
	float AddCatchFishRate(BYTE VipLevel)
	{
		HashMap<BYTE, tagVipOnce>::iterator Iter = VipMap.find(VipLevel);
		if (Iter == VipMap.end())
		{
			return 1.0f;
		}
		return (Iter->second.AddCatchFishRate + 100) / 100.0f;
	}
};

struct tagMonthCardOnce
{
	BYTE	MonthCardID;
	DWORD	UseLastMin;
	WORD	OnceDayRewardID;
	bool	IsCanAutoFire;//是否可以自动开火
	DWORD	AddLotteryRate;//抽奖概率提升的百分比
};
struct tagMonthCardConfig
{
	HashMap<BYTE, tagMonthCardOnce> MonthCardMap;
};

struct DropInfo
{
	WORD	RewardID;
	DWORD	RateValue;
};
struct tagFishDropOnce
{
	BYTE		FishTypeID;
	DWORD		TotalRateValue;
	vector<DropInfo> DroupVec;

	WORD GetFishDrop()
	{
		if (DroupVec.empty())
			return 0;
		DWORD RandValue = RandUInt() % TotalRateValue;
		vector<DropInfo>::iterator Iter = DroupVec.begin();
		//WORD UpperRewardID = 0;
		for (; Iter != DroupVec.end(); ++Iter)
		{
			if (RandValue < Iter->RateValue)
			{
				return Iter->RewardID;
			}
		}
		return 0;
	}
};
struct tagFishDropConfig
{
	HashMap<BYTE, tagFishDropOnce> FishDropMap;
};

//MiNiGame
//struct tagDialLevelInfo
//{
//	BYTE		ID;
//	DWORD		MinRate;
//	DWORD		MaxRate;
//};
struct RobotAnimalCountControl
{
	BYTE StartTime;
	BYTE EndTime;
	BYTE RobotCountMin;
	BYTE RobotCountMax;
};
struct tagDialRate
{
	BYTE			ID;
	DWORD			MaxRateSum;
	BYTE			RateArray[MAX_DIAL_ClientSum];
	BYTE			SumArray[MAX_DIAL_ClientSum];
};
struct tagDialConfig
{
	DWORD  BeginWriteSec;
	DWORD  EndWriteSec;
	DWORD  UpdateSec;
	DWORD  InitGlobelSum;
	DWORD  JoinGlobelSum;
	DWORD  MaxAddGlobel;
	BYTE   ChangeGlobelRate;//衰减值
	DWORD  JoinBankerGlobelSum;
	DWORD  GetNextBankerNeedGlobel;
	DWORD  BankerGameSum;
	DWORD  VipNonBetSum;
	BYTE	MaxRate;
	vector<BYTE> DialAreaData;//等级倍率
	vector<tagDialRate> RateMap;

	vector<RobotAnimalCountControl>VecRobotCount;
	BYTE  RobotCountInterval;
	BYTE  RobotMinGameTime;
	BYTE  RobotMaxGameTime;
	BYTE  RobotActionRate;
	BYTE  RobotBankerRate;
	BYTE RobotWaitBankerMin;
	BYTE RobotWaitBankerMax;
	BYTE RobotBankerCountMin;
	BYTE RobotBankerCountMax;
	BYTE RobotListCountMin;
	BYTE RobotListCountMax;
	BYTE RobotListCountInterval;

	BYTE RobotMinBetTime;
	BYTE RobotMaxBetTime;

	DWORD RobotMinJetton;
	DWORD RobotMaxJetton;
	DWORD RobotAreaLimit;


	DWORD RobotBankerScoreMin;
	DWORD RobotBankerScoreMax;
	DWORD RobotPlayerScoreMin;
	DWORD RobotPlayerScoreMax;

	float GetChangeGlobelRateValue()
	{
		return (1000 - ChangeGlobelRate) / 1000.0f;
	}
};
struct tagCarValue
{
	BYTE		ID;
	BYTE		TypeID;
	//根据配置文件计算出来的全局数据
	DWORD		MinRate;
	DWORD		MaxRate;//当前的权重数据 
};
struct tagCarType
{
	BYTE		TypeID;//类型ID
	BYTE		RateValue;//类型倍率
	DWORD		RateF;//随机到的权重
};

struct RobotCarCountControl
{
	BYTE StartTime;
	BYTE EndTime;
	BYTE RobotCountMin;
	BYTE RobotCountMax;
};

struct tagCarConfig
{
	DWORD  BeginWriteSec;
	DWORD  EndWriteSec;
	DWORD  UpdateSec;
	DWORD  InitGlobelSum;
	DWORD  JoinGlobelSum;
	DWORD  MaxAddGlobel;
	BYTE   ChangeGlobelRate;//衰减值
	DWORD  JoinBankerGlobelSum;
	DWORD  GetNextBankerNeedGlobel;
	DWORD  BankerGameSum;
	DWORD  VipNonBetSum;
	DWORD  MaxRate;

	vector<RobotCarCountControl>VecRobotCount;
	BYTE  RobotCountInterval;
	BYTE  RobotMinGameTime;
	BYTE  RobotMaxGameTime;
	BYTE  RobotActionRate;
	BYTE  RobotBankerRate;
	BYTE RobotWaitBankerMin;
	BYTE RobotWaitBankerMax;
	BYTE RobotBankerCountMin;
	BYTE RobotBankerCountMax;
	BYTE RobotListCountMin;
	BYTE RobotListCountMax;
	BYTE RobotListCountInterval;

	BYTE RobotMinBetTime;
	BYTE RobotMaxBetTime;

	DWORD RobotMinJetton;
	DWORD RobotMaxJetton;
	DWORD RobotAreaLimit;


	DWORD RobotBankerScoreMin;
	DWORD RobotBankerScoreMax;
	DWORD RobotPlayerScoreMin;
	DWORD RobotPlayerScoreMax;



	//汽车轮盘数据  2个  类型  和 具体的数据
	HashMap<BYTE, tagCarType>   CarTypeMap;//类型的数据
	vector<tagCarValue>  CarValueVec;//轮盘数据

	float GetChangeGlobelRateValue()
	{
		return (1000 - ChangeGlobelRate) / 1000.0f;
	}

	BYTE GetRateValue(BYTE Index)
	{
		if (Index >= CarValueVec.size())
			return 0;
		HashMap<BYTE, tagCarType>::iterator Iter = CarTypeMap.find(CarValueVec[Index].TypeID);
		if (Iter == CarTypeMap.end())
			return 0;
		else
			return Iter->second.RateValue;
	}
};
struct RobotOxCountControl
{
	BYTE StartTime;
	BYTE EndTime;
	BYTE RobotCountMin;
	BYTE RobotCountMax;
};
struct tagNiuNiuConfig
{
	DWORD  BeginWriteSec;
	DWORD  EndWriteSec;
	DWORD  UpdateSec;
	DWORD  InitGlobelSum;
	DWORD  JoinGlobelSum;
	DWORD  MaxAddGlobel;
	BYTE   MaxRate;
	BYTE   ChangeGlobelRate;//衰减值
	DWORD  JoinBankerGlobelSum;
	DWORD  GetNextBankerNeedGlobel;
	DWORD  BankerGameSum;
	DWORD  VipNonBetSum;
	HashMap<BYTE, BYTE> NiuNiuLevelRate;//等级倍率

	vector<RobotOxCountControl>VecRobotCount;
	BYTE  RobotCountInterval;
	BYTE  RobotMinGameTime;
	BYTE  RobotMaxGameTime;
	BYTE  RobotActionRate;
	BYTE  RobotBankerRate;
	BYTE RobotWaitBankerMin;
	BYTE RobotWaitBankerMax;
	BYTE RobotBankerCountMin;
	BYTE RobotBankerCountMax;
	BYTE RobotListCountMin;
	BYTE RobotListCountMax;
	BYTE RobotListCountInterval;

	BYTE RobotMinBetTime;
	BYTE RobotMaxBetTime;
	
	DWORD RobotMinJetton;
	DWORD RobotMaxJetton;
	DWORD RobotAreaLimit;
	

	DWORD RobotBankerScoreMin;
	DWORD RobotBankerScoreMax;
	DWORD RobotPlayerScoreMin;
	DWORD RobotPlayerScoreMax;


	

	float GetChangeGlobelRateValue()
	{
		return (1000 - ChangeGlobelRate) / 1000.0f;
	}

	BYTE GetRateByBrandValue(WORD Value)
	{
		BYTE Level = (BYTE)(Value >> 8);
		HashMap<BYTE, BYTE>::iterator Iter= NiuNiuLevelRate.find(Level);
		if (Iter != NiuNiuLevelRate.end())
			return Iter->second;
		else
			return 1;
	}
};
struct tagMiNiGameConfig
{
	tagNiuNiuConfig		niuniuConfig;
	tagDialConfig		dialConfig;
	tagCarConfig		carConfig;
};

struct tagRobotControlTime
{
	DWORD		OpenMinSec;
	DWORD		OpenMaxSec;
	DWORD		StopMinSec;
	DWORD		StopMaxSec;
};
struct tagRobotOpenFireInfo
{
	bool					IsOpen;
	tagRobotControlTime		FireInfo;

};
struct tagRobotLauncherType
{
	BYTE				LauncherType;
	DWORD				RateF;
};
struct tagRobotLauncherInfo
{
	bool					IsOpen;
	tagRobotControlTime    LauncherInfo;
	vector<tagRobotLauncherType> LauncherVec;
	DWORD						 MaxRateF;
};
struct tagRobotRateType
{
	BYTE		RateType;
	DWORD		RateF;
};
struct tagRobotRateInfo
{
	bool					IsOpen;
	tagRobotControlTime		RateInfo;
	vector<tagRobotRateType> RateVec;
	DWORD					 MaxRateF;
};
struct tagRobotSkillInfo
{
	bool					IsOpen;
	tagRobotControlTime   SkillInfo;
};
struct tagRobotRoomInfo
{
	bool					IsOpen;
	tagRobotControlTime	  RoomInfo;
};

struct tagRobotSendCharmInfo
{
	BYTE			CharmIndexID;
	WORD			MinItemSum;
	WORD			MaxItemSum;
	DWORD			RateF;
};
struct tagRobotSendCharm
{
	std::vector<tagRobotSendCharmInfo> vecSendCharmArray;
	DWORD				dwSendCharmArrayMaxRateF;
};
struct tagRobotBeSendCharm
{
	BYTE			CharmIndexID;
	BYTE			NonRateF;
	tagRobotSendCharm SendInfo;
};
struct tagRobotCharmChangeEvent
{
	DWORD			LeaveRoomRateF;
	BYTE			NonRateF;
	tagRobotSendCharm AddSendInfo;
	tagRobotSendCharm DelSendInfo;
};
struct tagRobotCharmInfo
{
	bool				IsOpen;
	DWORD				SendCharmMinCDSec;//主动赠送魅力物品的CD
	DWORD				SendCharmMaxCDSec;//主动赠送魅力物品的CD
	DWORD				BeChangeCharmValue;
	DWORD				BeChangeCharmSec;
	BYTE				NonRateF;
	tagRobotSendCharm	SendInfo;
	HashMap<BYTE, tagRobotBeSendCharm>	BeSendInfoMap;
	tagRobotCharmChangeEvent CharmChangeEvent;
};
struct tagGameRobotConfig
{
	DWORD				RobotID;
	DWORD				MonthRobotSum;
	BYTE				TableTypeID;
	BYTE				MonthID;
	DWORD				InitGlobelSum;
	DWORD				AddGlobelSum;
	DWORD				InitCurrceySum;
	DWORD				AddCurrceySum;
	DWORD				MinLevel;
	DWORD				MaxLevel;
	BYTE				GlobelRateF;
	BYTE				CurrceyRateF;
	tagRobotOpenFireInfo OpenFireInfo;
	tagRobotLauncherInfo LauncherInfo;
	tagRobotRateInfo     RateInfo;
	tagRobotSkillInfo	 SkillInfo;
	tagRobotRoomInfo     RoomInfo;
	tagRobotCharmInfo    CharmInfo;
};
struct tagGameRobotInfo
{
	HashMap<DWORD, tagGameRobotConfig>	RobotMap;
	multimap<DWORD, DWORD>				RobotIndexMap;
};

//具体的网络命令了 
//数据库命令
enum FishDBCmd
{
	//LogOn
	DB_Logon_Init = 0,

	DBR_AccountLogon = DB_Logon_Init + 1,
	DBO_AccountLogon = DB_Logon_Init + 1,

	DBR_AccountRsg = DB_Logon_Init + 2,
	DBO_AccountRsg = DB_Logon_Init + 2,

	DBR_ResetAccount = DB_Logon_Init + 3,
	DBO_ResetAccount = DB_Logon_Init + 3,

	DBR_ChangeAccountPassword = DB_Logon_Init + 4,
	DBO_ChangeAccountPassword = DB_Logon_Init + 4,

	DBR_GetAccountInfoByUserID = DB_Logon_Init + 5,
	DBO_GetAccountInfoByUserID = DB_Logon_Init + 5,

	DBR_SetOnline = DB_Logon_Init + 6,
	DBO_SetOnline = DB_Logon_Init + 6,

	//DBR_LoadAllInfo = DB_Logon_Init + 7,

	DBR_SetRoleClientInfo = DB_Logon_Init + 7,

	DBR_GetNewAccount = DB_Logon_Init + 8,
	DBO_GetNewAccount = DB_Logon_Init + 8,

	DBR_QueryLogon = DB_Logon_Init+9,
	DBO_QueryLogon = DB_Logon_Init+9,

	DBR_SaveChannelInfo = DB_Logon_Init +10,
	
	DBR_SetAccountFreeze = DB_Logon_Init + 11,

	DBR_PhoneLogon = DB_Logon_Init + 12,
	DBO_PhoneLogon = DB_Logon_Init + 12,

	DBR_PhoneSecPwd = DB_Logon_Init + 13,
	DBO_PhoneSecPwd = DB_Logon_Init + 13,

	//Item
	DB_Item_Init,

	DBR_LoadUserItem = DB_Item_Init + 1,
	DBO_LoadUserItem = DB_Item_Init + 1,

	//DBO_LoadUserItemFinish = DB_Item_Init + 2,

	DBR_AddUserItem = DB_Item_Init + 3,
	DBO_AddUserItem = DB_Item_Init + 3,

	DBR_DelUserItem = DB_Item_Init + 4,

	DBR_ChangeUserItem = DB_Item_Init + 5,
	//Relation
	DB_Relation_Init,

	DBR_LoadUserRelation = DB_Relation_Init + 1,
	DBO_LoadUserRelation = DB_Relation_Init + 1,

	DBO_LoadBeUserRelation = DB_Relation_Init + 2,

	//DBO_LoadBeUserRelationFinish = DB_Relation_Init + 3,

	DBR_AddUserRelation = DB_Relation_Init + 4,
	DBO_AddUserRelation = DB_Relation_Init + 4,

	DBR_DelUserRelation = DB_Relation_Init + 5,
	//Mail
	DB_Main_Init,
	DBR_LoadUserMail = DB_Main_Init + 1,
	DBO_LoadUserMail = DB_Main_Init + 1,

	//DBO_LoadUserMailFinish = DB_Main_Init + 2,

	DBR_AddUserMail = DB_Main_Init + 3,
	DBO_AddUserMail = DB_Main_Init + 3,

	DBR_DelUserMail = DB_Main_Init + 4,

	DBR_GetUserMailItem = DB_Main_Init + 5,

	DBR_ChangeUserMailIsRead = DB_Main_Init + 6,

	//Role
	DB_Role_Init,

	DBR_SaveRoleExInfo = DB_Role_Init + 1,
	DBR_SaveRoleNickName = DB_Role_Init + 2,
	DBO_SaveRoleNickName = DB_Role_Init + 2,
	DBR_SaveRoleLevel = DB_Role_Init + 3,
	DBR_SaveRoleGender = DB_Role_Init + 4,
	DBR_SaveRoleAchievementPoint = DB_Role_Init + 5,
	DBR_SaveRoleCurTitle = DB_Role_Init + 6,
	DBR_SaveRoleCharmArray = DB_Role_Init + 7,
	DBR_SaveRoleCurrency = DB_Role_Init + 8,
	DBR_SaveRoleFaceID = DB_Role_Init + 9,
	DBR_SaveRoleMedal = DB_Role_Init + 10,
	DBR_SaveRoleSendGiffSum = DB_Role_Init + 11,
	DBR_SaveRoleAcceptGiffSum = DB_Role_Init + 12,
	DBR_SaveRoleTaskStates = DB_Role_Init + 13,
	DBR_SaveRoleAchievementStates = DB_Role_Init + 14,
	DBR_SaveRoleActionStates = DB_Role_Init + 15,
	DBR_SaveRoleOnlineStates = DB_Role_Init + 16,
	DBR_SaveRoleCheckData = DB_Role_Init + 17,
	DBR_SaveRoleIsShowIpAddress = DB_Role_Init + 18,


	DBR_SaveRoleAllInfo = DB_Role_Init + 19,
	DBO_SaveRoleAllInfo = DB_Role_Init + 20,

	DBR_SaveRoleGlobel = DB_Role_Init + 21,
	DBR_SaveRoleOnlineMin = DB_Role_Init + 22,

	DBR_SaveRoleExChangeStates = DB_Role_Init + 23,
	DBR_SaveRoleTotalRecharge = DB_Role_Init + 24,
	DBR_SaveRoleIsFirstPayGlobel = DB_Role_Init + 25,
	DBR_SaveRoleIsFirstPayCurrcey = DB_Role_Init + 26,

	DBR_ChangeRoleMoney = DB_Role_Init + 27,

	DBR_TableChange = DB_Role_Init + 28,

	DBR_RoleEntityItem = DB_Role_Init + 29,
	DBO_RoleEntityItem = DB_Role_Init + 29,

	DBR_DelRoleEntityItem = DB_Role_Init + 30,

	DBR_SaveRoleMonthCardInfo = DB_Role_Init + 31,
	DBR_SaveRoleRateValue = DB_Role_Init + 32,
	DBR_SaveRoleGetMonthCardRewardTime = DB_Role_Init + 33,
	DBR_SaveRoleVipLevel = DB_Role_Init + 34,
	DBR_SaveRoleCashSum = DB_Role_Init + 35,
	DBR_DelRoleCashSum = DB_Role_Init + 36,

	DBR_AddRoleTotalRecharge = DB_Role_Init + 37,

	DBR_SaveRoleMaxRateValue = DB_Role_Init + 38,

	//DBR_SaveRoleLockInfo        = DB_Role_Init + 38,

	DBR_SaveRoleParticularStates = DB_Role_Init + 39,

	DBR_SaveRoleSecPassword = DB_Role_Init + 40,

	DBR_ChangeRoleSecPassword = DB_Role_Init + 41,
	DBO_ChangeRoleSecPassword = DB_Role_Init + 41,

	DBR_ChangeRoleShareStates = DB_Role_Init + 42,

	DBR_GameIDConvertToUserID = DB_Role_Init + 43,
	DBO_GameIDConvertToUserID = DB_Role_Init + 43,
	//Query
	DB_Query_Init,

	DBR_Query_RoleInfoByNickName = DB_Query_Init+1,
	DBO_Query_RoleInfo = DB_Query_Init+1,

	DBR_Query_RoleInfoByUserID = DB_Query_Init+2,
	DBO_Query_RoleInfoByUserID = DB_Query_Init+2,

	DBR_Query_RoleInfoByGameID = DB_Query_Init + 3,
	DBO_Query_RoleInfoByGameID = DB_Query_Init + 3,

	//TASK
	DB_Task_Init,

	DBR_LoadRoleTask = DB_Task_Init+1,
	DBO_LoadRoleTask = DB_Task_Init + 1,

	DBR_SaveRoleTask = DB_Task_Init + 2,//保存玩家的一个任务

	DBR_DelRoleTask = DB_Task_Init + 3,

	DBR_ClearRoleTask = DB_Task_Init + 4,//清空一个玩家全部的任务

	DBR_SaveAllTask = DB_Task_Init+5,

	//成就
	DB_Achievement_Init,
	DBR_LoadRoleAchievement = DB_Achievement_Init + 1,
	DBO_LoadRoleAchievement = DB_Achievement_Init + 1,

	DBR_SaveRoleAchievement = DB_Achievement_Init + 2,//保存玩家的一个任务

	DBR_DelRoleAchievement = DB_Achievement_Init + 3,

	DBR_SaveAllAchievement = DB_Achievement_Init + 4,
	//称号
	DB_Title_Init,
	DBR_LoadRoleTitle = DB_Title_Init+1,
	DBO_LoadRoleTitle = DB_Title_Init + 1,

	DBR_AddRoleTitle = DB_Title_Init + 2,

	DBR_DelRoleTitle = DB_Title_Init + 3,

	//排行榜
	DB_Rank_Init,

	DBR_LoadRankInfo = DB_Rank_Init+1,
	DBO_LoadRankInfo = DB_Rank_Init + 1,

	DBR_LoadWeekRankInfo = DB_Rank_Init + 2,
	DBO_LoadWeekRankInfo = DB_Rank_Init + 2,

	DBR_SaveWeekRankInfo = DB_Rank_Init + 3,

	DBR_ChangeWeekInfo = DB_Rank_Init +4,
	//魅力 目前不在使用 
	DB_Charm_Init,
	DBR_SaveRoleCharm = DB_Charm_Init+1,
	DBO_SaveRoleCharm = DB_Charm_Init+1,

	//商店
	DB_Shop_Init,
	DBR_AddRoleEntityItem = DB_Shop_Init+1,

	//现实地址
	DB_Enetity_Init,
	DBR_LoadRoleEntity = DB_Enetity_Init+1,
	DBO_LoadRoleEntity = DB_Enetity_Init + 1,

	DBR_SaveRoleEntityName = DB_Enetity_Init + 2,
	DBR_SaveRoleEntityPhone = DB_Enetity_Init + 3,
	DBR_SaveRoleEntityEmail = DB_Enetity_Init + 4,
	DBR_SaveRoleEntityIdentityID = DB_Enetity_Init + 5,
	DBR_SaveRoleEntityItemUseName = DB_Enetity_Init + 6,
	DBR_SaveRoleEntityItemUsePhone = DB_Enetity_Init + 7,
	DBR_SaveRoleEntityItemUseAddress = DB_Enetity_Init + 8,

	//DBR_SaveRoleEntity = 94,
	//活动
	DB_Acion_Init,
	DBR_LoadRoleAction = DB_Acion_Init+1,
	DBO_LoadRoleAction = DB_Acion_Init + 1,

	DBR_SaveRoleAction = DB_Acion_Init + 2,

	DBR_DelRoleAction = DB_Acion_Init + 3,

	DBR_SaveAllAction = DB_Acion_Init +4,

	//赠送
	DB_Giff_Init,
	DBR_LoadRoleGiff = DB_Giff_Init+1,
	DBO_LoadRoleGiff = DB_Giff_Init + 1,

	DBR_AddRoleGiff = DB_Giff_Init + 2,
	DBO_AddRoleGiff = DB_Giff_Init + 2,

	DBR_DelRoleGiff = DB_Giff_Init + 3,

	DBR_GetNowDayGiff = DB_Giff_Init+4,
	DBO_GetNowDayGiff = DB_Giff_Init + 4,//获取今天的赠送数据

	DBR_ClearNowDayGiff = DB_Giff_Init+ 5,
	//GameData
	DB_GameData_Init,
	DBR_LoadGameData = DB_GameData_Init+1,
	DBO_LoadGameData = DB_GameData_Init + 1,

	DBR_SaveGameData = DB_GameData_Init + 2,

	DBR_ChangeGameDataByMonth = DB_GameData_Init + 3,

	DBR_LoadAllUserAchievementPointList = DB_GameData_Init + 4,//读取全部的成就点排行
	DBO_LoadAllUserAchievementPointList = DB_GameData_Init + 4,//向服务器发送排行


	DBR_LoadAllAccountInfo = DB_GameData_Init + 5,
	DBO_LoadAllAcoountInfo = DB_GameData_Init + 5,

	//DBO_LoadAllAccountInfoFinish = DB_GameData_Init + 6,

	//Log
	DB_Log_Init,
	DBR_LogInfo = DB_Log_Init+1,
	DBR_LogRecharge = DB_Log_Init + 2,
	DBR_LogOnlineInfo = DB_Log_Init +3,
	DBR_LogPhonePay = DB_Log_Init+4,
	DBR_LogRoleTableInfo = DB_Log_Init +5,
	DBR_LogEntityItemInfo = DB_Log_Init+6,
	DBR_CreateLogTable  = DB_Log_Init + 7,
	DBR_LogNiuNiuTableInfo = DB_Log_Init + 8,
	DBR_LogExChangeInfo = DB_Log_Init+9,
	DBR_LogLotteryInfo = DB_Log_Init+10,
	DBR_LogMonthInfo = DB_Log_Init + 11,
	DBR_LogDialInfo = DB_Log_Init + 12,
	DBR_LogCarInfo = DB_Log_Init + 13,

	//Announcement
	DB_Announcement_Init,
	DBR_LoadAllAnnouncement = DB_Announcement_Init+1,
	DBO_LoadALlAnnouncement = DB_Announcement_Init + 1,
	//DBO_LoadAllAnnouncementFinish = DB_Announcement_Init + 2,

	DBR_AddAnnouncement = DB_Announcement_Init + 3,

	//Operate
	DB_Operate_Init,
	DBR_SaveRoleEmail = DB_Operate_Init+1,

	//Center
	DB_Center_Init,
	DBR_ClearFishDB = DB_Center_Init+1,
	DBO_ClearFishDB = DB_Center_Init+1,

	//ExChange
	DBR_ExChange_Init,

	DBR_QueryExChange = DBR_ExChange_Init + 1,
	DBO_QueryExChange = DBR_ExChange_Init + 1,
	DBR_DelExChange   = DBR_ExChange_Init + 2,

	//Recharge
	DBR_Recharge_Init,
	DBR_AddRecharge = DBR_Recharge_Init +1,

	DBR_LoadRechageOrderID = DBR_Recharge_Init +2,
	DBO_LoadRechageOrderID = DBR_Recharge_Init +2,

	DBR_AddRechageOrderID = DBR_Recharge_Init+3,

	DBR_CheckEntityID = DBR_Recharge_Init+4,
	DBO_CheckEntityID = DBR_Recharge_Init+4,

	DBR_CheckPhone = DBR_Recharge_Init + 5,
	DBO_CheckPhone = DBR_Recharge_Init + 5,

	DBR_LoadPhonePayOrderID = DBR_Recharge_Init+6,
	DBO_LoadPhonePayOrderID = DBR_Recharge_Init+6,
	DBR_AddPhonePayOrderID = DBR_Recharge_Init+7,

	DBR_GetRechargeOrderID = DBR_Recharge_Init+8,
	DBO_GetRechargeOrderID = DBR_Recharge_Init+8,

	DBR_QueryRechargeOrderInfo = DBR_Recharge_Init + 9,
	DBO_QueryRechargeOrderInfo = DBR_Recharge_Init + 9,

	DBR_DelRechargeOrderInfo = DBR_Recharge_Init + 10,

	//Control
	DBR_Control_init,
	DBR_ResetUserPassword	= DBR_Control_init +1,
	DBR_FreezeUser			= DBR_Control_init+2,

	//Robot
	DBR_Robot_Init,
	DBR_LoadRobotInfo = DBR_Robot_Init + 1,
	DBO_LoadRobotInfo = DBR_Robot_Init + 1,

	//Char
	DBR_Char_Init,
	DBR_LoadCharInfo = DBR_Char_Init + 1,
	DBO_LoadCharInfo = DBR_Char_Init + 1,
	DBR_DelCharInfo = DBR_Char_Init + 2,
	DBR_AddCharInfo = DBR_Char_Init+3,

	//Relationrequest
	DBR_RelationRequest_Init,

	DBR_LoadRelationRequest = DBR_RelationRequest_Init + 1,
	DBO_LoadRelationRequest = DBR_RelationRequest_Init + 1,

	DBR_AddRelationRequest = DBR_RelationRequest_Init + 2,
	DBO_AddRelationRequest = DBR_RelationRequest_Init + 2,

	DBR_DelRelationRequest = DBR_RelationRequest_Init + 3,

	DBR_OPERATOR_SYSTEM_MAIL_INIT,
    DBR_ADD_OPERATOR_SYSTEM_MAIL = DBR_OPERATOR_SYSTEM_MAIL_INIT + 1,
	DBO_ADD_OPERATOR_SYSTEM_MAIL = DBR_OPERATOR_SYSTEM_MAIL_INIT + 2,
	DBR_SAVE_ROLE_SYSTEM_MAIL_RECORD = DBR_OPERATOR_SYSTEM_MAIL_INIT + 3,
	DBR_LOAD_OPERATOR_SYSTEM_MAIL = DBR_OPERATOR_SYSTEM_MAIL_INIT + 4,
	DBO_LOAD_OPERATOR_SYSTEM_MAIL = DBR_OPERATOR_SYSTEM_MAIL_INIT + 5,

	DBR_Operator_Logon_Init,
	DBR_Operator_Logon = DBR_Operator_Logon_Init +1,
	DBO_Operator_Logon = DBR_Operator_Logon_Init + 2,

		DBR_Deal_Init,
		DBR_Deal_Create,
		DBO_Deal_Create,
		DBR_Deal_Third_Platform_Verify,
		DBO_Deal_Third_Platform_Verify,
		DBR_Deal_Apply_Create_Log,
		DBR_Deal_Create_Log,
		DBR_Deal_ThirdPlatform_Verify_Log,
		DBR_Deal_Pay_Log
};


struct DBR_Cmd_Deal_Apply_Create_Log : public NetCmd
{
	int ItemID;
	int ChannelID;
	DWORD UserID;
};

struct DBR_Cmd_Deal_Create_Log : public NetCmd
{
	int ItemID;
	DWORD UserID;
	DWORD OrderID;
	DWORD ChannelID;
	WCHAR ProductID[MAX_PayNOLength];
};

struct DBR_Cmd_Deal_ThirdPlatform_Verify_Log : public NetCmd
{
	DWORD SDKFlowID;
	DWORD OrderID;
	int ChannelID;
	char ExternalData[256];
};

struct DBR_Cmd_Deal_Pay_Log : public NetCmd
{
	DWORD user_id;
	DWORD OrderID;
	int ChannelID;
	int Price;
	WCHAR good_id[MAX_PayNOLength];
	int ShopItem;
};



struct DBR_Cmd_Deal_Third_Platform_Verify : public NetCmd
{
	DWORD Order_id;
	//WCHAR good_id[MAX_PayNOLength];
};

struct tagDeal
{
	DWORD user_id;
	DWORD order_id;
	DWORD channel_id;
	DWORD shop_id;
	WCHAR good_id[MAX_PayNOLength];
};
struct DBO_Cmd_Deal_Third_Platform_Verify : public NetCmd
{
	tagDeal info;
	bool result;
};

struct DBR_Cmd_Deal_Create : public NetCmd
{
	WCHAR       good_id[MAX_PayNOLength];
	DWORD		shop_id;
	DWORD		user_id;
	DWORD		channel_id;
};


struct DBO_Cmd_Deal_Create : public NetCmd
{
	DWORD	   order_id;
	DWORD	   shop_id;
	DWORD	   user_id;
	WCHAR       good_id[MAX_PayNOLength];
};


struct tagLogon
{
	TCHAR	AccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
	TCHAR	MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	BYTE	PlatformID;//平台的ID
	int		ChannelID;
	DWORD   VersionID;
	DWORD	PathCrc;
	TCHAR	External[LOGON_EXTERNAL_LENGTH + 1];//Mac地址
	DWORD	ClientIP;
};
struct DBR_Cmd_Operator_Logon : public NetCmd
{	
	DWORD ClientID;
	tagLogon logon;
};

struct DBO_Cmd_Operator_Logon : public NetCmd
{
	DWORD ClientID;	
	DWORD dwUserID;
};


struct DBR_Cmd_AccountLogon : public NetCmd
{
	//玩家进行登陆处理
	DWORD ClientID;
	TCHAR	AccountName[ACCOUNT_LENGTH+1];
	TCHAR	MacAddress[MAC_ADDRESS_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
};
struct DBO_Cmd_AccountLogon : public NetCmd
{
	DWORD ClientID;
	DWORD	dwUserID;
	bool	IsFreeze;
	__int64 FreezeEndTime;
};
struct DBR_Cmd_AccountRsg : public NetCmd
{
	DWORD	ClientID;
	TCHAR	AccountName[ACCOUNT_LENGTH + 1];
	TCHAR	MacAddress[MAC_ADDRESS_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
	//注册数据
	DWORD	ClientIP;
};
struct DBO_Cmd_AccountRsg : public NetCmd
{
	DWORD ClientID;
	DWORD	dwUserID;
};

struct DBR_Cmd_PhoneSecPwd : public NetCmd
{
	DWORD	ClientID;
	UINT64	PhoneNumber;
	TCHAR	MacAddress[MAC_ADDRESS_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
};
struct DBO_Cmd_PhoneSecPwd : public NetCmd
{
	DWORD	ClientID;
	TCHAR	AccountName[ACCOUNT_LENGTH + 1];
	DWORD	dwUserID;
	bool	IsFreeze;
	__int64 FreezeEndTime;
};
struct DBR_Cmd_ResetAccount : public NetCmd
{
	DWORD	dwUserID;
	TCHAR	AccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
};
struct DBO_Cmd_ResetAccount : public NetCmd
{
	DWORD	dwUserID;
	bool	Result;
	TCHAR	AccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
};
struct DBR_Cmd_ChangeAccountPassword : public NetCmd
{
	DWORD	dwUserID;
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
	DWORD	OldPasswordCrc1;
	DWORD	OldPasswordCrc2;
	DWORD	OldPasswordCrc3;
};
struct DBO_Cmd_ChangeAccountPassword : public NetCmd
{
	DWORD	dwUserID;
	bool	Result;
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
};
struct DBR_Cmd_GetAccountInfoByUserID : public NetCmd
{
	DWORD	dwUserID;
	DWORD	ClientID;
	bool	LogonByGameServer;
};
struct DBO_Cmd_GetAccountInfoByUserID : public NetCmd
{
	DWORD		ClientID;
	bool		Result;
	bool		IsFreeze;
	__int64     FreezeEndTime;
	tagRoleInfo RoleInfo;
	tagRoleServerInfo RoleServerInfo;
	__int64		LastOnlineTime;//最后登录的时间
	bool		LogonByGameServer;
	bool		IsRobot;
};
struct DBR_Cmd_RoleOnline : public NetCmd
{
	DWORD			dwUserID;
	bool			IsOnline;
};
struct DBO_Cmd_RoleOnline : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
	bool			IsOnline;
};
struct DBR_Cmd_GetNewAccount : public NetCmd
{
	DWORD			ClientID;
	TCHAR			MacAddress[MAC_ADDRESS_LENGTH + 1];
	DWORD			PasswordCrc1;
	DWORD			PasswordCrc2;
	DWORD			PasswordCrc3;
	DWORD			ClientIP;
};
struct DBO_Cmd_GetNewAccount : public NetCmd
{
	bool			Result;
	DWORD			ClientID;
	DWORD			dwUserID;
	TCHAR			AccountName[ACCOUNT_LENGTH + 1];
	DWORD			PasswordCrc1;
	DWORD			PasswordCrc2;
	DWORD			PasswordCrc3;
};
struct DBR_Cmd_QueryLogon : public NetCmd
{
	DWORD			ClientID;
	TCHAR			AccountName[ACCOUNT_LENGTH + 1];
	TCHAR			MacAddress[MAC_ADDRESS_LENGTH + 1];
	DWORD			PasswordCrc1;
	DWORD			PasswordCrc2;
	DWORD			PasswordCrc3;
	DWORD			ClientIP;
	BYTE			LogonType;
};
struct DBO_Cmd_QueryLogon : public NetCmd
{
	DWORD			ClientID;
	DWORD			dwUserID;
	bool			IsFreeze;
	__int64			FreezeEndTime;
	bool			IsNewRsg;
	BYTE			LogonType;
};
struct DBR_Cmd_SaveChannelInfo : public NetCmd
{
	DWORD			dwUserID;
	ChannelUserInfo pInfo;
};
struct DBR_Cmd_SetAccountFreeze : public NetCmd
{
	DWORD			dwUserID;
	DWORD			FreezeMin;
};

struct DBR_Cmd_PhoneLogon : public NetCmd
{
	UINT64			PhoneNumber;
	DWORD			ClientID;
	TCHAR			MacAddress[MAC_ADDRESS_LENGTH + 1];
	DWORD			ClientIP;
	DWORD			PasswordCrc1;
	DWORD			PasswordCrc2;
	DWORD			PasswordCrc3;
	bool			IsFirOrSec;
};
struct DBO_Cmd_PhoneLogon : public NetCmd
{
	DWORD			ClientID;
	DWORD			dwUserID;
	//新生成的账号 我们让玩家登陆
	TCHAR			AccountName[ACCOUNT_LENGTH + 1];
	DWORD			PasswordCrc1;
	DWORD			PasswordCrc2;
	DWORD			PasswordCrc3;
	bool			IsFreeze;
	__int64			FreezeEndTime;
};
//struct DBR_Cmd_LoadAllInfo : public NetCmd
//{
//	DWORD			dwUserID;
//};
//struct DBR_Cmd_GetRoleAchievementIndex : public NetCmd
//{
//	DWORD			dwUserID;
//};
//struct DBO_Cmd_GetRoleAchievementIndex : public NetCmd
//{
//	DWORD			dwUserID;
//	DWORD			AchievementIndex;
//};
struct DBR_Cmd_SetRoleClientInfo : public NetCmd
{
	DWORD			dwUserID;
	BYTE			PlateFormID;
	DWORD			ScreenPoint;
};

//Item
struct DBR_Cmd_LoadUserItem : public NetCmd
{
	DWORD			dwUserID;
};
struct DBO_Cmd_LoadUserItem : public NetCmd
{
	DWORD			dwUserID; 
	BYTE			States;
	WORD			Sum;
	tagItemInfo		Array[1];
};
//struct DBO_Cmd_LoadUserItemFinish : public NetCmd
//{
//	DWORD			dwUserID;
//};
struct DBR_Cmd_AddUserItem : public NetCmd
{
	DWORD			dwUserID;
	tagItemOnce		ItemInfo;
};
struct DBO_Cmd_AddUserItem : public NetCmd
{
	DWORD			dwUserID;
	tagItemInfo		ItemInfo;
};
struct DBR_Cmd_DelUserItem : public NetCmd
{
	DWORD			ItemOnlyID;
};
struct DBR_Cmd_ChangeUserItem : public NetCmd
{
	DWORD			ItemOnlyID;
	DWORD			ItemSum;
};
//Relation
struct DBR_Cmd_LoadUserRelation : public NetCmd
{
	DWORD			dwUserID;
};
struct DBO_Cmd_LoadUserRelation : public NetCmd
{
	DWORD			dwUserID;
	BYTE			States;
	WORD			Sum;
	tagRoleRelation Array[1];
};
struct DBO_Cmd_LoadBeUserRelation : public NetCmd
{
	DWORD				dwUserID;
	BYTE				States;
	WORD				Sum;
	tagBeRoleRelation	Array[1];
};
//struct DBO_Cmd_LoadBeUserRelationFinish : public NetCmd
//{
//	DWORD			dwUserID;
//};

struct DBR_Cmd_AddUserRelation : public NetCmd
{
	DWORD			dwSrcUserID;
	DWORD			dwDestUserID;
	BYTE			bRelationType;
};
struct DBO_Cmd_AddUserRelation : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
	tagRoleRelation	RelationInfo;
};
struct DBR_Cmd_DelUserRelation : public NetCmd
{
	DWORD			dwSrcUserID;
	DWORD			dwDestUserID;
};
//struct DBR_Cmd_ChangeUserRelation : public NetCmd
//{
//	DWORD			dwSrcUserID;
//	DWORD			dwDestUserID;
//	BYTE			bRelationType;
//};
//Mail
struct DBR_Cmd_LoadUserMail : public NetCmd
{
	DWORD			dwUserID;
};
struct DBO_Cmd_LoadUserMail : public NetCmd
{
	DWORD			dwUserID;
	BYTE			States;
	WORD			Sum;
	tagRoleMail     Array[1];
};
//struct DBO_Cmd_LoadUserMailFinish : public NetCmd
//{
//	DWORD			dwUserID;
//};
struct DBR_Cmd_AddUserMail : public NetCmd
{
	DWORD			dwDestUserID;//目标玩家的ID
	tagRoleMail		MailInfo;//待添加的邮件
};

struct DBR_Cmd_SaveSystemMailRecord : public NetCmd
{
	DWORD				UserID;
	tagSystemMailRecord MailRecord[MAX_SYSTEM_RECORD_COUT];
};
struct DBO_Cmd_AddUserMail : public NetCmd
{
	DWORD			dwDestUserID;
	bool			Result;
	tagRoleMail		MailInfo;
};
struct DBR_Cmd_DelUserMail : public NetCmd
{
	DWORD			dwMailID;
};
struct DBR_Cmd_GetUserMailItem : public NetCmd
{
	DWORD			dwMailID;
	//tagItemOnce		ItemArray[MAX_MAIL_ITEM];//物品的数据 更改后的物品的数据
};
struct DBR_Cmd_ChangeUserMailIsRead : public NetCmd
{
	DWORD			dwMailID;
	bool			bIsRead;
};
//Role
struct DBR_Cmd_SaveRoleExInfo : public NetCmd
{
	int			 RoleCharmValue;
	tagRoleInfo  RoleInfo;
};
//struct DBR_Cmd_SaveRoleQueryAtt : public NetCmd
//{
//	//部分需要立即保存数据库的属性进行保存
//	DWORD			dwUserID;
//	TCHAR			NickName[MAX_NICKNAME + 1];
//	WORD			wLevel;//玩家点击
//	DWORD			dwFaceID;//玩家头像ID
//	bool			bGender; //玩家性别
//	DWORD			dwAchievementPoint;//玩家的成就点数
//	BYTE			TitleID;
//	bool			IsShowIpAddress;
//};

struct DBR_Cmd_SaveRoleNickName : public NetCmd
{
	DWORD			dwUserID;
	TCHAR			NickName[MAX_NICKNAME + 1];
};
struct DBO_Cmd_SaveRoleNickName : public NetCmd
{
	bool			Result;
	DWORD			dwUserID;
	TCHAR			NickName[MAX_NICKNAME + 1];
};
struct DBR_Cmd_SaveRoleLevel : public NetCmd
{
	DWORD			dwUserID;
	WORD			wLevel;
};
struct DBR_Cmd_SaveRoleGender : public NetCmd
{
	DWORD			dwUserID;
	bool			bGender;
};
struct DBR_Cmd_SaveRoleAchievementPoint : public NetCmd
{
	DWORD			dwUserID;
	DWORD			dwAchievementPoint;
};
struct DBR_Cmd_SaveRoleCurTitle : public NetCmd
{
	DWORD			dwUserID;
	BYTE			TitleID;
};
struct DBR_Cmd_SaveRoleCharmArray : public NetCmd
{
	DWORD			dwUserID;
	DWORD			CharmArray[MAX_CHARM_ITEMSUM];//魅力值对应的物品
};
struct DBR_Cmd_SaveRoleCurrency : public NetCmd
{
	DWORD			dwUserID;
	DWORD			dwCurrencyNum;
};
struct DBR_Cmd_SaveRoleFaceID : public NetCmd
{
	DWORD			dwUserID;
	DWORD			dwFaceID;
};
struct DBR_Cmd_SaveRoleMedal : public NetCmd
{
	DWORD			dwUserID;
	DWORD			dwMedalSum;
	DWORD			TotalUseMedal;
};
struct DBR_Cmd_SaveRoleSendGiffSum : public NetCmd
{
	DWORD			dwUserID;
	BYTE			SendSum;
};
struct DBR_Cmd_SaveRoleAcceptGiffSum : public NetCmd
{
	DWORD			dwUserID;
	BYTE			AcceptSum;
};
struct DBR_Cmd_SaveRoleTaskStates : public NetCmd
{
	DWORD			dwUserID;
	int256			States;
};
struct DBR_Cmd_SaveRoleAchievementStates : public NetCmd
{
	DWORD			dwUserID;
	int256			States;
};
struct DBR_Cmd_SaveRoleActionStates : public NetCmd
{
	DWORD			dwUserID;
	int256			States;
};
struct DBR_Cmd_SaveRoleOnlineStates : public NetCmd
{
	DWORD			dwUserID;
	DWORD			OnlineStates;
};
struct DBR_Cmd_SaveRoleCheckData : public NetCmd
{
	DWORD			dwUserID;
	DWORD			CheckData;
};
struct DBR_Cmd_SaveRoleIsShowIpAddress : public NetCmd
{
	DWORD			dwUserID;
	bool			IsShowIP;
};
struct DBR_Cmd_SaveRoleExChangeStates : public NetCmd
{
	DWORD			dwUserID;
	DWORD			States;
};
struct DBR_Cmd_SaveRoleTotalRecharge : public NetCmd
{
	DWORD			dwUserID;
	DWORD			Sum;
};
struct DBR_Cmd_AddRoleTotalRecharge : public NetCmd
{
	DWORD			dwUserID;
	DWORD			Sum;
};
struct DBR_Cmd_SaveRoleMaxRateValue : public NetCmd
{
	DWORD			dwUserID;
	BYTE			MaxRate;
};
struct DBR_Cmd_SaveRoleParticularStates : public NetCmd
{
	DWORD			dwUserID;
	DWORD			ParticularStates;
};
struct DBR_Cmd_SaveRoleSecPassword : public NetCmd
{
	DWORD			dwUserID;
	DWORD			SecPasswordCrc1;
	DWORD			SecPasswordCrc2;
	DWORD			SecPasswordCrc3;
};
struct DBR_Cmd_ChangeRoleSecPassword : public NetCmd
{
	DWORD			dwUserID;

	DWORD			dwOldSecPasswordCrc1;
	DWORD			dwOldSecPasswordCrc2;
	DWORD			dwOldSecPasswordCrc3;

	DWORD			dwNewSecPasswordCrc1;
	DWORD			dwNewSecPasswordCrc2;
	DWORD			dwNewSecPasswordCrc3;
};
struct DBO_Cmd_ChangeRoleSecPassword : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
	DWORD			dwNewSecPasswordCrc1;
	DWORD			dwNewSecPasswordCrc2;
	DWORD			dwNewSecPasswordCrc3;
};
/*struct DBR_Cmd_SaveRoleLockInfo : public NetCmd
{
	DWORD			dwUserID;
	DWORD			LockStates;
	time_t			LockEndTime;
};*/
struct DBR_Cmd_SaveRoleIsFirstPayGlobel : public NetCmd
{
	DWORD			dwUserID;
};
struct DBR_Cmd_SaveRoleIsFirstPayCurrcey : public NetCmd
{
	DWORD			dwUserID;
};
struct DBR_Cmd_ChangeRoleMoney : public NetCmd
{
	DWORD			dwUserID;
	int				GlobelSum;
	int				MedalSum;
	int				CurrceySum;
};
struct DBR_Cmd_TableChange : public NetCmd
{
	DWORD			dwUserID;
	BYTE			TableTypeID;
	BYTE			TableMonthID;
	int				GlobelSum;
	int				MedalSum;
	int				CurrceySum;
	time_t			LogTime;
	bool			JoinOrLeave;
};
struct DBR_Cmd_RoleEntityItem : public NetCmd
{
	DWORD			ClientID;
	DWORD			ClientIP;
	DWORD			ID;
	char			OrderNumber[32];//快递单号
};
struct DBO_Cmd_RoleEntityItem : public NetCmd
{
	DWORD			ClientID;
	DWORD			ClientIP;
	char			OrderNumber[32];//快递单号
	bool			Result;
	//订单的数据
	DWORD			ID;
	DWORD			ItemID;
	DWORD			ItemSum;
	DWORD			MedalNum;
	DWORD			NowMedalNum;
	DWORD			UserID;
	UINT64			Phone;
	time_t			ShopTime;
	TCHAR			Address[MAX_ADDRESS_LENGTH + 1];
	TCHAR			Name[MAX_NAME_LENTH + 1];
	TCHAR			IDEntity[MAX_IDENTITY_LENGTH + 1];
};
struct DBR_Cmd_DelRoleEntityItem : public NetCmd
{
	DWORD			ID;
};
struct DBR_Cmd_SaveRoleMonthCardInfo : public NetCmd
{
	DWORD			UserID;
	BYTE			MonthCardID;
	__int64			MonthCardEndTime;
};
struct DBR_Cmd_SaveRoleRateValue : public NetCmd
{
	DWORD			UserID;
	int256			RateValue;//玩家倍率属性
	DWORD			MaxRateValue;
};
struct DBR_Cmd_SaveRoleGetMonthCardRewardTime : public NetCmd
{
	DWORD			UserID;
	__int64			LogTime;
};
struct DBR_Cmd_SaveRoleVipLevel : public NetCmd
{
	DWORD			UserID;
	BYTE			VipLevel;
};
struct DBR_Cmd_SaveRoleCashSum : public NetCmd
{
	DWORD			UserID;
	BYTE			CashSum;
	DWORD			TotalCashSum;
};
struct DBR_Cmd_ChangeRoleShareStates : public NetCmd
{
	DWORD			dwUserID;
	bool			States;
};

struct DBR_Cmd_GameIDConvertToUserID : public NetCmd
{
	DWORD			SrcUserID;
	DWORD			dwGameID;
	BYTE			NetCmd;
};
struct DBO_Cmd_GameIDConvertToUserID : public NetCmd
{
	DWORD			SrcUserID;
	DWORD			dwGameID;
	DWORD			dwUserID;
	BYTE			NetCmd;
};

struct DBR_Cmd_DelRoleCashSum : public NetCmd
{
	DWORD			UserID;
	BYTE			DelCashSum;
};
union UnionEventInfo
{
	tagRoleTaskInfo	TaskInfo;
	tagRoleAchievementInfo AchievementInfo;
	tagRoleActionInfo ActionInfo;
};
struct SaveEventInfo
{
	BYTE			InfoStates;//1 任务  2 成就 3 活动
	UnionEventInfo	EventInfo;
};
struct DBR_Cmd_SaveRoleAllInfo : public NetCmd//玩家需要保存的全部的数据
{
	DWORD			dwUserID;
	bool			IsNeedResult;//是否需要返回命令
	//根据待保存的数据 
	//1.RoleEx 中的
	DWORD			dwExp;//玩家经验
	DWORD			dwGlobeNum;
	DWORD			dwProduction;//当天获得的金币数量
	DWORD			dwGameTime;//当天的游戏时间
	WORD			OnlineMin;
	DWORD			ClientIP;//玩家IP
	DWORD			dwLotteryScore;
	BYTE			bLotteryFishSum;
	//int64			TotalFishGlobelSum;
	tagRoleServerInfo RoleServerInfo;
	//2.GameData
	tagRoleGameData GameData;
	//3.
	BYTE			States;
	WORD			Sum;
	SaveEventInfo   Array[1];
};
struct DBO_Cmd_SaveRoleAllInfo : public NetCmd
{
	DWORD			dwUserID;//玩家在保存完毕后的回发数据
};
struct DBR_Cmd_SaveRoleGlobel : public NetCmd
{
	DWORD			dwUserID;
	DWORD			dwGlobel;
};
struct DBR_Cmd_SaveRoleOnlineMin : public NetCmd
{
	DWORD			dwUserID;
	WORD			OnLineMin;
};
//Query
struct DBR_Cmd_QueryRoleInfoByNickName : public NetCmd
{
	DWORD		dwUserID;
	TCHAR		QueryNickName[MAX_NICKNAME + 1];
	BYTE		Page;
	bool		IsOnline;
};
struct DBO_Cmd_QueryRoleInfo : public NetCmd
{
	DWORD		dwUserID;
	WORD		Page;
	BYTE		States;
	WORD		Sum;
	tagQueryRoleInfo Array[1];
};
struct DBR_Cmd_Query_RoleInfoByUserID : public NetCmd
{
	DWORD		dwUserID;
	DWORD		dwDestUserID;
};
struct DBO_Cmd_Query_RoleInfoByUserID : public NetCmd
{
	DWORD		dwUserID;
	tagQueryRoleInfo RoleInfo;
};

struct DBR_Cmd_Query_RoleInfoByGameID : public NetCmd
{
	DWORD		dwUserID;
	DWORD		dwDestGameID;
};
struct DBO_Cmd_Query_RoleInfoByGameID : public NetCmd
{
	DWORD		dwUserID;
	tagQueryRoleInfo RoleInfo;
};
//Check
//struct DBR_Cmd_LoadRoleCheckInfo : public NetCmd
//{
//	DWORD	dwUserID;
//};
//struct DBO_Cmd_LoadRoleCheckInfo : public NetCmd
//{
//	DWORD	dwUserID;
//	tagRoleCheckInfo	CheckInfo;
//};
//struct DBR_Cmd_ChangeRoleCheckInfo : public NetCmd
//{
//	DWORD	dwUserID;
//	tagRoleCheckInfo	CheckInfo;
//};
//Task
struct DBR_Cmd_LoadRoleTask : public NetCmd
{
	DWORD  dwUserID;
};
struct DBO_Cmd_LoadRoleTask : public NetCmd
{
	DWORD  dwUserID;
	BYTE   States;
	WORD   Sum;
	tagRoleTaskInfo Array[1];
};
struct DBR_Cmd_SaveRoleTask : public NetCmd
{
	DWORD  dwUserID;
	tagRoleTaskInfo TaskInfo;
};
struct DBR_Cmd_DelRoleTask : public NetCmd
{
	DWORD	dwUserID;
	BYTE	bTaskID;
};
struct DBR_Cmd_ClearRoleTask : public NetCmd
{
	DWORD	dwUserID;
};
struct DBR_Cmd_SaveAllTask : public NetCmd
{
	DWORD			dwUserID;
	BYTE			States;
	WORD			Sum;
	tagRoleTaskInfo Array[1];
};
//Achievement
struct DBR_Cmd_LoadRoleAchievement : public NetCmd
{
	DWORD  dwUserID;
};
struct DBO_Cmd_LoadRoleAchievement : public NetCmd
{
	DWORD  dwUserID;
	BYTE   States;
	WORD   Sum;
	tagRoleAchievementInfo Array[1];
};
struct DBR_Cmd_SaveRoleAchievement : public NetCmd
{
	DWORD  dwUserID;
	tagRoleAchievementInfo AchievementInfo;
};
struct DBR_Cmd_DelRoleAchievement : public NetCmd
{
	DWORD	dwUserID;
	BYTE	bAchievementID;
};
struct DBR_Cmd_SaveAllAchievement : public NetCmd
{
	DWORD			dwUserID;
	BYTE			States;
	WORD			Sum;
	tagRoleAchievementInfo Array[1];
};
//称号
struct DBR_Cmd_LoadRoleTitle : public NetCmd
{
	DWORD  dwUserID;
};
struct DBO_Cmd_LoadRoleTitle : public NetCmd
{
	DWORD  dwUserID;
	BYTE   States;
	WORD   Sum;
	BYTE   Array[1];
};
struct DBR_Cmd_AddRoleTitle : public NetCmd
{
	DWORD  dwUserID;
	BYTE   TitleID;
};
struct DBR_Cmd_DelRoleTitle : public NetCmd
{
	DWORD  dwUserID;
	BYTE   TitleID;
};
//Rank
struct DBR_Cmd_LoadRankInfo : public NetCmd
{
	BYTE  RankID;
	BYTE  RandRowCount;
};
struct DBO_Cmd_LoadRankInfo : public NetCmd
{
	BYTE  RankID;
	BYTE  States;
	WORD  Sum;
	tagRankInfo Array[1];
};
//struct DBR_Cmd_LoadWeekRankInfo : public NetCmd
//{
//	DWORD	VersionID;
//};
//struct RankIndexInfo
//{
//	BYTE		RankID;
//	BYTE		RankIndex;
//};
//struct tagRankWeekReward
//{
//	RankIndexInfo   IndexInfo;
//	bool			IsReward;//是否已经领取奖励了
//};
//struct tagDBWeekReward
//{
//	DWORD				dwUserID;
//	tagRankWeekReward	WeekRank;
//};
//struct DBO_Cmd_LoadWeekRankInfo : public NetCmd
//{
//	WORD				WeekRankSum;
//	tagDBWeekReward		WeekRankArray[1];
//};
//struct DBR_Cmd_SaveWeekRankInfo : public NetCmd//待保存的数据
//{
//	DWORD				VersionID;
//	tagDBWeekReward		SaveInfo;
//};
struct tagRankWeekRankInfo
{
	DWORD			dwUserID;
	DWORD			VersionID;
	BYTE			RankID;
	BYTE			RankIndex;
	bool			IsReward;
};
struct tagRankWeekRankClient
{
	BYTE			RankID;
	BYTE			RankIndex;
	bool			IsReward;
};
struct DBR_Cmd_SaveWeekRankInfo : public NetCmd
{
	BYTE					States;
	WORD					Sum;
	tagRankWeekRankInfo		Array[1];
};
struct DBR_Cmd_LoadWeekRankInfo : public NetCmd
{
	DWORD					dwUserID;
	bool					IsInit;
};
struct DBO_Cmd_LoadWeekRankInfo : public NetCmd
{
	DWORD					dwUserID;
	bool					IsInit;
	BYTE					States;
	WORD					Sum;
	tagRankWeekRankClient	Array[1];
};
struct DBR_Cmd_ChangeWeekInfo : public NetCmd
{
	DWORD					dwUserID;
	tagRankWeekRankClient	RankInfo;
};
//魅力
struct DBR_Cmd_SaveRoleCharm : public NetCmd
{
	DWORD	dwUserID;
	DWORD   dwDestUserID;
	BYTE	bIndex;
	int		AddCharmValue;
	DWORD	ItemSum;
};
struct DBO_Cmd_SaveRoleCharm : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	BYTE	bIndex;
	DWORD	ItemSum;
	bool	Result;
	int		dwDestUserCharmValue;
};
struct DBR_Cmd_AddRoleEntityItem : public NetCmd
{
	DWORD	dwUserID;
	DWORD	ItemID;
	DWORD	ItemSum;
	DWORD	UseMedal;
	DWORD	NowMedalSum;
	TCHAR	Name[MAX_NAME_LENTH + 1];//姓名
	unsigned __int64	Phone;//手机号码
	TCHAR	Addres[MAX_ADDRESS_LENGTH + 1];//现实地址
	TCHAR	IDEntity[MAX_IDENTITY_LENGTH+1];
};
//Entity
struct DBR_Cmd_LoadRoleEntity : public NetCmd
{
	DWORD	dwUserID;
};
struct DBO_Cmd_LoadRoleEntity : public NetCmd
{
	DWORD	dwUserID;
	tagRoleAddressInfo RoleInfo;
};
struct DBR_Cmd_SaveRoleEntityName : public NetCmd
{
	DWORD			dwUserID;
	TCHAR			Name[MAX_NAME_LENTH + 1];//姓名
};
struct DBR_Cmd_SaveRoleEntityPhone : public NetCmd
{
	DWORD			dwUserID;
	unsigned __int64	Phone;//手机号码
};
struct DBR_Cmd_SaveRoleEntityEmail : public NetCmd
{
	DWORD			dwUserID;
	TCHAR			Email[MAX_EMAIL_LENGTH + 1];//邮箱
};
struct DBR_Cmd_SaveRoleEntityIdentityID : public NetCmd
{
	DWORD	dwUserID;
	TCHAR			IdentityID[MAX_IDENTITY_LENGTH + 1];//身份证号码
	
};
struct DBR_Cmd_SaveRoleEntityItemUseName : public NetCmd
{
	DWORD	dwUserID;
	TCHAR			EntityItemUseName[MAX_NAME_LENTH + 1];//收货人的名称
	
};
struct DBR_Cmd_SaveRoleEntityItemUsePhone : public NetCmd
{
	DWORD	dwUserID;
	unsigned __int64 EntityItemUsePhone;//收货人的手机号码
	
};
struct DBR_Cmd_SaveRoleEntityItemUseAddress : public NetCmd
{
	DWORD	dwUserID;
	TCHAR			EntityItemUseAddres[MAX_ADDRESS_LENGTH + 1];//收货人的现实地址
};
//struct DBR_Cmd_SaveRoleEntity : public NetCmd
//{
//	DWORD	dwUserID;
//	tagRoleAddressInfo RoleInfo;
//};
//Action
struct DBR_Cmd_LoadRoleAction : public NetCmd
{
	DWORD  dwUserID;
};
struct DBO_Cmd_LoadRoleAction : public NetCmd
{
	DWORD  dwUserID;
	BYTE	States;
	WORD   Sum;
	tagRoleActionInfo Array[1];
};
struct DBR_Cmd_SaveRoleAction : public NetCmd
{
	DWORD  dwUserID;
	tagRoleActionInfo ActionInfo;
};
struct DBR_Cmd_DelRoleAction : public NetCmd
{
	DWORD	dwUserID;
	BYTE	bActionID;
};
struct DBR_Cmd_SaveAllAction : public NetCmd
{
	DWORD			dwUserID;
	BYTE			States;
	WORD			Sum;
	tagRoleActionInfo Array[1];
};
//赠送
struct DBR_Cmd_LoadRoleGiff : public NetCmd
{
	DWORD	dwUserID;
	DWORD	LimitDay;
};
struct DBO_Cmd_LoadRoleGiff : public NetCmd
{
	DWORD	dwUserID;
	BYTE	States;
	WORD    Sum;
	tagGiffInfo Array[1];
};
struct DBR_Cmd_AddRoleGiff : public NetCmd
{
	DWORD	SrcUserID;
	DWORD	DestUserID;
};
struct DBO_Cmd_AddRoleGiff : public NetCmd
{
	DWORD		dwDestUserID;
	tagGiffInfo GiffInfo;
	bool	Result;
};
struct DBR_Cmd_DelRoleGiff : public NetCmd
{
	DWORD	GiffID;
};
struct tagNowDaySendGiffInfo
{
	DWORD		dwUserID;
	BYTE		SendSum;
};
struct DBR_Cmd_GetNowDayGiff : public NetCmd
{
	DWORD		dwUserID;
};
struct DBO_Cmd_GetNowDayGiff : public NetCmd
{
	//获取今天玩家发送的赠送的数据 玩家ID 次数
	DWORD   dwUserID;
	BYTE	States;
	WORD	Sum;
	tagNowDaySendGiffInfo Array[1];
};
struct DBR_Cmd_ClearNowDayGiff : public NetCmd
{
	DWORD		dwUserID;
};
struct DBR_Cmd_LoadGameData : public NetCmd
{
	DWORD	dwUserID;
};
struct DBO_Cmd_LoadGameData : public NetCmd
{
	DWORD	dwUserID;
	tagRoleGameData GameData;
};
struct DBR_Cmd_SaveGameData : public NetCmd
{
	DWORD	dwUserID;
	tagRoleGameData GameData;
};
struct DBR_Cmd_ChangeGameDataByMonth :public NetCmd
{
	DWORD	dwUserID;
	BYTE	MonthIndex;
};
struct DBR_Cmd_LoadAllUserAchievementPointList :public NetCmd
{
	DWORD   RankSum;
};
struct DBO_Cmd_LoadAllUserAchievementPointList :public NetCmd
{
	BYTE		States;
	WORD		Sum;
	DWORD		Array[1];
};
//登陆
struct AccountCacheInfo
{
	DWORD	dwUserID;
	TCHAR	AccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
	TCHAR   MacAddress[MAC_ADDRESS_LENGTH + 1];
	bool	IsFreeze;
	__int64 FreezeEndTime;
};
struct TempAccountCacheInfo
{
	DWORD			 ClientID;
	AccountCacheInfo AccountInfo;
};
struct DBR_Cmd_LoadAllAccountInfo :public NetCmd
{

};
struct DBO_Cmd_LoadAllAccountInfo :public NetCmd//变长分包命令
{
	BYTE				States;
	WORD				Sum;
	AccountCacheInfo	Array[1];
};
//struct DBO_Cmd_LoadAllAccountInfoFinish :public NetCmd
//{
//
//};
struct DBR_Cmd_LogInfo :public NetCmd
{
	DWORD			UserID;
	BYTE			Type;
	int				TypeSum;
	DWORD			Param;
	TCHAR			Info[DB_Log_Str_Length+1];
};
struct LogRechargeInfo
{
	//char			OrderString
	//char			OrderID
	//char			OrderCode
	//char			ChannelOrderID
	//char			channelLabel
	BYTE			HandleSum;
	DWORD			Sum;
	BYTE			Array[1];
};
struct DBR_Cmd_LogRecharge :public NetCmd
{
	DWORD				UserID;
	DWORD				ShopItemID;
	DWORD				Price;
	DWORD				FreePrice;
	DWORD				OldGlobelNum;
	DWORD				OldCurrceyNum;
	DWORD				AddGlobel;
	DWORD				AddCurrcey;
	WORD				AddRewardID;
	LogRechargeInfo		RechargeInfo;
};
struct DBR_Cmd_LogOnlineInfo :public NetCmd
{
	//记录玩家在线的数据
	DWORD			dwUserID;
	bool			IsOnline;
	DWORD			GlobelSum;
	DWORD			CurrceySum;
	DWORD			MadelSum;
	char			MacAddress[MAC_ADDRESS_LENGTH+1];
	char			IpAddress[IP_MAX_LENGTH + 1];
};
struct DBR_Cmd_LogRoleTableInfo :public NetCmd
{
	DWORD			dwUserID;
	DWORD			GlobelSum;
	DWORD			MedalSum;
	DWORD			CurrceySum;
	BYTE			TableID;
	BYTE			TableMonthID;
	bool			JionOrLeave;
};
struct LogEntityItemInfo
{
	//char				OrderStates
	//char				Address
	//char				IDEntity
	//char				Name
	BYTE			HandleSum;
	DWORD			Sum;
	BYTE			Array[1];
};
struct DBR_Cmd_LogEntityItemInfo :public NetCmd
{
	DWORD ID;
	DWORD ItemID;
	DWORD ItemSum;
	time_t ShopLogTime;
	DWORD UserID;
	UINT64 Phone;
	DWORD Medal;
	DWORD NowMedal;
	DWORD HandleIP;
	LogEntityItemInfo EntityInfo;
};
struct LogPhonePayInfo
{
	//char			OrderStates
	BYTE			HandleSum;
	DWORD			Sum;
	BYTE			Array[1];
};
struct DBR_Cmd_LogPhonePay :public NetCmd
{
	UINT64 OrderID;
	DWORD UserID;
	UINT64 PhoneNumber;
	DWORD FacePrice;
	LogPhonePayInfo PhonePayInfo;
};
struct DBR_Cmd_CreateLogTable :public NetCmd
{
	WORD		Year;
	BYTE		Month;
	BYTE		Day;
};
struct DBR_Cmd_LogNiuNiuTableInfo :public NetCmd
{
	BYTE		BrandValue[MAX_NIUNIU_ClientSum + 1][MAX_NIUNIU_BrandSum];//25张牌的数据
	INT64		BrandGlobelSum;//每局输赢的结果
	DWORD		UserSum;
	INT64		SystemGlobelSum;
	time_t		NowTime;
};
struct DBR_Cmd_LogExChangeInfo :public NetCmd
{
	DWORD		dwUserID;
	BYTE		ExChangeTypeID;
	TCHAR		ExChangeCode[ExChangeCode_Length + 1];
};
struct DBR_Cmd_LogLotteryInfo :public NetCmd
{
	DWORD		dwUserID;
	BYTE		LotteryID;
	WORD		RewardID;
};
struct DBR_Cmd_LogMonthInfo :public NetCmd
{
	DWORD		dwUserID;
	BYTE		MonthID;
	DWORD		MonthIndex;
	DWORD		MonthScore;
	DWORD		SkillSum;
	BYTE		AddGlobelSum;
	WORD		RewardID;
};
struct DBR_Cmd_LogDialInfo :public NetCmd
{
	DWORD BanderUserID;
	BYTE AreaData[MAX_DIAL_GameSum];
	UINT64 AreaGlobel[MAX_DIAL_ClientSum];
	WORD ResultIndex;
	INT64 BrandGlobel;
	INT64 SystemGlobel;
	DWORD RoleSum;
};
struct DBR_Cmd_LogCarInfo :public NetCmd
{
	DWORD BanderUserID;
	UINT64 AreaGlobel[MAX_CAR_ClientSum];
	BYTE ResultIndex;
	INT64 BrandGlobel;
	INT64 SystemGlobel;
	DWORD RoleSum;
};

struct DBR_Cmd_LoadAllSystemMail :public NetCmd
{
	
};

#define MAX_SYSTEM_MAIL_COUNT 100
struct DBO_Cmd_LoadAllSystemMail :public NetCmd
{
	tagOperatorSystemMail mail[MAX_SYSTEM_MAIL_COUNT];
	BYTE			States;
	WORD			Sum;
};

struct DBR_Cmd_LoadAllAnnouncement :public NetCmd
{

};
struct DBO_Cmd_LoadALlAnnouncement :public NetCmd
{
	BYTE			States;
	WORD			Sum;
	AnnouncementOnce Array[1];
};
//struct DBO_Cmd_LoadAllAnnouncementFinish :public NetCmd
//{
//
//};
struct DBR_Cmd_AddAnnouncement :public NetCmd
{
	AnnouncementOnce AnnouncementInfo;
};
struct DBR_Cmd_SaveRoleEmail :public NetCmd
{
	DWORD		dwUserID;
	TCHAR		Email[MAX_EMAIL_LENGTH+1];
};
struct DBR_Cmd_ClearFishDB : public NetCmd
{
	bool		IsInit;
};
struct DBO_Cmd_ClearFishDB : public NetCmd
{
	
};

//ExChange
struct DBR_Cmd_QueryExChange : public NetCmd
{
	DWORD		dwUserID;
	TCHAR		ExChangeCode[ExChangeCode_Length+1];
};
struct DBO_Cmd_QueryExChange : public NetCmd
{
	DWORD		dwUserID;
	TCHAR		ExChangeCode[ExChangeCode_Length + 1];
	BYTE		ID;
};
struct DBR_Cmd_DelExChange : public NetCmd
{
	DWORD		dwUserID;
	TCHAR		ExChangeCode[ExChangeCode_Length + 1];
};

//Recharge
struct DBR_Cmd_AddRecharge : public NetCmd
{
	//玩家进行操作  玩家离线的情况下进行
	DWORD		AddGlobelSum;
	DWORD		AddCurrceySum;
	bool		IsFirst;
	//Log
	RechageInfo	rechargeInfo;//充值的操作
};
struct DBR_Cmd_LoadRechageOrderID : public NetCmd
{
	//加载全部的订单号
	BYTE			DiffDay;
};
struct OrderSetInfo
{
	UINT64   OrderID;
	BYTE	 OrderType;
};
struct DBO_Cmd_LoadRechageOrderID : public NetCmd
{
	BYTE						States;
	WORD						Sum;
	OrderSetInfo				Array[1];
};
struct DBR_Cmd_AddRechageOrderID : public NetCmd
{
	DWORD						dwUserID;
	OrderSetInfo				OrderOnlyID;
};
struct DBR_Cmd_CheckEntityID : public NetCmd
{
	//检测身份证
	DWORD						dwUserID;
	DWORD						EntityCrcID;
	TCHAR						EntityID[MAX_IDENTITY_LENGTH+1];
};
struct DBO_Cmd_CheckEntityID : public NetCmd
{
	DWORD						dwUserID;
	DWORD						EntityCrcID;
	bool						Result;
};
struct DBR_Cmd_CheckPhone : public NetCmd
{
	bool						IsBindOrLogon;
	DWORD						dwUserID;
	unsigned __int64			Phone;
	DWORD						ClientID;
};
struct DBO_Cmd_CheckPhone : public NetCmd
{
	bool						IsBindOrLogon;
	DWORD						dwUserID;
	bool						Result;
	unsigned __int64			Phone;
	DWORD						ClientID;
};
struct DBR_Cmd_LoadPhonePayOrderID : public NetCmd
{
	BYTE			DiffDay;
};
struct DBO_Cmd_LoadPhonePayOrderID : public NetCmd
{
	BYTE						States;
	WORD						Sum;
	unsigned __int64			Array[1];
};
struct DBR_Cmd_AddPhonePayOrderID : public NetCmd
{
	DWORD						dwUserID;
	unsigned __int64			OrderOnlyID;
};
struct DBR_Cmd_GetRechargeOrderID : public NetCmd
{
	DWORD						dwUserID;
	DWORD						ShopID;
	bool						IsPC;
};
struct DBO_Cmd_GetRechargeOrderID : public NetCmd
{
	DWORD						OrderID;
	DWORD						dwUserID;
	DWORD						ShopID;
	bool						IsPC;
};
struct DBR_Cmd_QueryRechargeOrderInfo : public NetCmd
{
	DWORD						OrderID;
	DWORD						Price;
	DWORD						CheckUserID;
	DWORD						CheckShopID;
};
struct DBO_Cmd_QueryRechargeOrderInfo : public NetCmd
{
	DWORD						OrderID;
	DWORD						dwUserID;
	DWORD						ShopID;
	DWORD						Price;
	DWORD						CheckUserID;
	DWORD						CheckShopID;
};
struct DBR_Cmd_DelRechargeOrderInfo : public NetCmd
{
	DWORD						OrderID;
};
//Control
struct DBR_Cmd_ResetUserPassword : public NetCmd
{
	DWORD						dwUserID;
	DWORD						Password1;
	DWORD						Password2;
	DWORD						Password3;
};
struct DBR_Cmd_FreezeUser : public NetCmd
{
	DWORD						dwUserID;
	DWORD						FreezeMin;
};

struct RobotUserInfo  //机器人的基本数据
{
	DWORD			dwUserID;
	TCHAR			NickName[MAX_NICKNAME + 1];
	WORD			wLevel;//玩家点击
	DWORD			dwExp;//玩家经验
	DWORD			dwFaceID;//玩家头像ID
	bool			bGender; //玩家性别
	DWORD			dwGlobeNum;
	DWORD           dwMedalNum;
	DWORD			dwCurrencyNum;
	DWORD			dwAchievementPoint;//玩家的成就点数
	BYTE			TitleID;
	DWORD			CharmArray[MAX_CHARM_ITEMSUM];//魅力值对应的物品
	BYTE			VipLevel;//不涉及数据库的
	bool			IsInMonthCard;//是否为月卡用户
};

struct DBR_Cmd_LoadRobotInfo : public NetCmd
{
	DWORD						BeginUserID;
	DWORD						EndUserID;
};
struct DBO_Cmd_LoadRobotInfo : public NetCmd
{
	BYTE		States;
	WORD		Sum;
	RobotUserInfo Array[1];
};
//char
#define MAX_ROLECHAR_LENGTH 32
struct tagRoleCharInfo  //一条聊天信息 好友的数据 所以 无须携带其他玩家相关的数据
{
	DWORD		SrcUserID;//发送方
	DWORD		DestUserID;//接收方
	TCHAR		MessageInfo[MAX_ROLECHAR_LENGTH + 1];//消息的主体
	__int64     LogTime;//发送的时间
};

struct DBR_Cmd_LoadCharInfo : public NetCmd
{
	DWORD		dwUserID;
};
struct DBO_Cmd_LoadCharInfo : public NetCmd
{
	DWORD				dwUserID;
	BYTE				States;
	WORD				Sum;
	tagRoleCharInfo		Array[1];
};
struct DBR_Cmd_DelCharInfo : public NetCmd
{
	DWORD				dwSrcUserID;
	DWORD				dwDestUserID;
};
struct DBR_Cmd_AddCharInfo : public NetCmd
{
	tagRoleCharInfo		MessageInfo;
};


struct tagRelationRequest
{
	DWORD		ID;
	DWORD		SrcUserID;
	DWORD		SrcFaceID;
	TCHAR		SrcNickName[MAX_NICKNAME + 1];
	WORD		SrcLevel;
	BYTE		SrcVipLevel;
	bool		SrcGender;
	TCHAR		SrcIPAddress[MAX_ADDRESS_LENGTH_IP + 1];
	DWORD		DestUserID;
	TCHAR		MessageInfo[MAX_RelationRequestLength + 1];
	BYTE		RelationType;
	__int64     SendTime;
};
struct DBR_Cmd_LoadRelationRequest : public NetCmd
{
	DWORD				dwUserID;
};
struct DBO_Cmd_LoadRelationRequest : public NetCmd
{
	DWORD				dwUserID;
	BYTE				States;
	WORD				Sum;
	tagRelationRequest	Array[1];
};
struct DBR_Cmd_AddRelationRequest : public NetCmd
{
	DWORD				dwUserID;
	tagRelationRequest	Info;
};
struct DBO_Cmd_AddRelationRequest : public NetCmd
{
	DWORD				dwUserID;
	bool				Result;
	BYTE				ErrorID;
	tagRelationRequest	Info;
};
struct DBR_Cmd_DelRelationRequest : public NetCmd
{
	DWORD				dwUserID;
	DWORD				ID;
};

//网络命令模块 
//主命令
enum MainCmd
{
	Main_Logon = 1,//登陆命令
	Main_Table = 2,//桌子上的命令
	Main_Game = 3,//游戏ID
	Main_Center = 4,//与中央服务器的通讯
	Main_Balanced = 5,//Gate与负载均衡之间的通讯

	Main_Item = 6,//玩家物品相关的操作
	Main_Relation = 7,
	Main_Mail = 8,//邮件 系统邮件 或者 玩家私信
	Main_Role = 9,//玩家的基本数据修改
	Main_Query = 10,//查询玩家数据
	Main_Check = 11,//玩家签到
	Main_Task = 12,//玩家任务
	Main_Achievement = 13,//玩家成就
	Main_Title = 14,//玩家称号
	Main_Month = 15,//比赛
	Main_Rank = 16,//排行榜
	Main_Chest = 17,//宝箱
	Main_Charm = 18,//魅力
	Main_Shop = 19,//商店
	Main_Entity = 20,//玩家现实数据
	Main_Action = 21,//活动
	Main_Giff = 22,//赠送
	//Main_GlobelShop = 23,
	Main_OnlineReward = 24,
	Main_Ftp = 25,
	Main_GameData = 26,
	Main_Package = 27,//礼包
	Main_Launcher = 28,
	Main_Message = 29,
	Main_Recharge = 30,
	Main_Announcement = 31,
	Main_Operate = 32,//运营 对外的服务
	Main_RoleMessage = 33,
	Main_Exchange = 34,//兑换功能
	Main_Lottery = 35,//抽奖
	Main_Extra_Down = 36,//FTP下载命令
	Main_MiniGame = 37,
	Main_NiuNiu = 38,
	Main_Char = 39,
	Main_RelationRequest = 40,
	Main_Dial = 41,
	Main_Car = 42,
	Main_OperatorSystemMail = 43,

	Main_Server		  = 200,//服务器构架的流程
	Main_Control	  = 201,//控制器
};

enum OperatorSystemMailCmd
{
	CG_AddOperatorSystemMail,
	GC_GetAllSystemMail,
	CG_GetAllSystemMail

};
struct CG_Cmd_AddNewOperatorMail : public NetCmd
{
	tagOperatorSystemMail mail;
};

struct GC_Cmd_GetAllOperatorSystemMail : public NetCmd
{

};


struct CG_Cmd_GetAllOperatorSystemMail : public NetCmd
{
	tagOperatorSystemMail mail[MAX_SYSTEM_MAIL_COUNT];
	int sum;
};

enum LogonCmd
{
	CL_AccountLogon = 1,
	CL_AccountRsg = 2,
	//CL_AccountQueryRsg = 3,

	LG_AccountOnlyID = 4,
	LC_AccountOnlyID = 5,

	CL_AccountOnlyID = 6,
	LC_AccountOnlyIDFailed = 7,
	LC_AccountOnlyIDSuccess = 8,

	/*CL_AccountChangePassword = 6,
	LC_AccountChangePassword = 7,

	CL_AccountResetAccount = 8,
	LC_AccountResetAccount = 9,*/

	//GameServer 之间人数同步
	GL_RsgGameServer = 9,
	//GL_OnlinePlayerSum = 10,

	GL_AccountOnlyID = 11,

	CL_ResetLogonGameServer = 12,
	LC_ResetLogonGameServer = 13,

	LC_AccountLogonToScreen = 14,//玩家直接登陆到场景

	LC_ServerChangeSocket = 15,//玩家在游戏中被顶掉了

	CL_ResetPassword = 16,
	LC_ResetPassword = 17,

	CL_AccountResetAccount = 18,
	LC_AccountResetAccount = 19,

	LC_LogonQueueWrite = 20,

	GL_ResetPassword = 21,
	GL_ResetAccount = 22,


	CL_GetNewAccount = 23,
	LC_GetNewAccount = 24,

	LC_CheckVersionError = 25,
	LC_AccountIsFreeze = 26,

	//渠道的登陆
	CL_ChannelLogon = 30,//客户端发起登陆
	LO_ChannelLogon = 31,
	OL_ChannelLogon = 32,
	LC_ChannelLogon = 33,

	//快速登陆
	CL_QueryLogon = 40,
	LG_ChannelInfo = 41,

	//手机号码登陆
	CL_PhoneLogon = 50,
	LO_PhoneLogon = 51,
	OL_PhoneLogon = 52,
	LC_PhoneLogon = 53,
	CL_PhoneLogonCheck = 54,
	LO_PhoneLogonCheck = 55,
	OL_PhoneLogonCheck = 56,
	LC_PhoneLogonCheck = 57,
	LC_SaveAccountInfo = 58,

	//手机二级密码登陆
	CL_PhoneSecPwd = 70,

	//WeiXin
	CL_WeiXinLogon = 80,
	LO_WeiXinLogon = 81,
	OL_WeiXinLogon = 82,
	LC_WeiXinLogon = 83,

	//QQ
	CL_QQLogon = 84,
	LO_QQLogon = 85,
	OL_QQLogon = 86,
	LC_QQLogon = 87,

	//
	LC_RsgNewAccount = 88,

	CL_OperatorLogon = 89,
	LC_OperatorLogon = 90,
	LO_OperatorLogon = 91,
	OL_OperatorLogon = 92,
};

enum ChannelType
{
	Self_ChannelType,
	Dome_ChannelType, //冰穹渠道

};

struct LC_Cmd_OperatorLogon : public NetCmd
{
	int channel;
	bool result;
};
struct CL_Cmd_OperatorLogon : public NetCmd
{
	tagLogon logon;
};

struct LO_Cmd_OperatorLogon : public NetCmd
{
	tagLogon logon;
	BYTE     client_id;
};

struct OL_Cmd_OperatorLogon : public NetCmd
{
	tagLogon logon;
	BYTE     client_id;
	bool	 result;
};

struct CL_Cmd_QueryLogon : public NetCmd
{
	TCHAR	AccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
	TCHAR	MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	BYTE	PlateFormID;//平台的ID
	DWORD   VersionID;
	DWORD	PathCrc;
	BYTE	LogonType;//1:Channel  2:WeiXin  3:QQ
};
struct CL_Cmd_AccountLogon : public NetCmd
{
	TCHAR	AccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
	TCHAR	MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	BYTE	PlateFormID;//平台的ID
	DWORD   VersionID;
	DWORD	PathCrc;
};
struct CL_Cmd_PhoneSecPwd : public NetCmd
{
	UINT64				PhoneNumber;
	DWORD				PasswordCrc1;
	DWORD				PasswordCrc2;
	DWORD				PasswordCrc3;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	BYTE				PlateFormID;//平台的ID
	DWORD				VersionID;
	DWORD				PathCrc;
};
struct CL_Cmd_AccountRsg : public NetCmd
{
	TCHAR	AccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
	/*TCHAR	NickName[MAX_NICKNAME + 1];
	bool	Gender;*/
	TCHAR	MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	BYTE	PlateFormID;//平台的ID
	DWORD   VersionID;
	DWORD	PathCrc;
};
//struct CL_Cmd_AccountQueryRsg : public NetCmd
//{
//	DWORD	PasswordCrc1;
//	DWORD	PasswordCrc2;
//	DWORD	PasswordCrc3;
//	/*TCHAR	NickName[MAX_NICKNAME + 1];
//	bool	Gender;*/
//	BYTE	PlateFormID;//平台的ID
//	DWORD   VersionID;
//	DWORD	PathCrc;
//};
struct LG_Cmd_AccountOnlyID : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwOnlyID;//生成的唯一ID验证码
	DWORD	ClientID;
	BYTE	GameServerID;
	BYTE	LogonID;
	BYTE	LogonTypeID;//登陆1 注册2
};
struct GL_Cmd_AccountOnlyID : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwOnlyID;//生成的唯一ID验证码
	DWORD	ClientID;
	BYTE	GameServerID;
	BYTE	LogonTypeID;//登陆1 注册2
};
struct LC_Cmd_AccountOnlyID : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwOnlyID;
	DWORD	GateIp;
	WORD	GatePort;
	DWORD	GameIp;
	WORD	GamePort;
	DWORD	HallTimeOut;
	BYTE	LogonType;//登陆1  注册2
};
struct CL_Cmd_AccountOnlyID : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwOnlyID;
	BYTE	PlateFormID;//玩家平台ID
	DWORD	ScreenPoint;//玩家屏幕分辨率
	TCHAR	MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
};
struct LC_Cmd_AccountOnlyIDFailed : public NetCmd
{
};
struct LC_Cmd_AccountOnlyIDSuccess : public NetCmd
{
	DWORD			RandID;//客户端唯一的ID
	DWORD			OperateIp;//携带运营服务器的IP地址
	tagRoleInfo		RoleInfo;//玩家数据
};
struct LC_Cmd_AccountLogonToScreen : public NetCmd
{
	DWORD			RandID;//客户端唯一的ID
	//tagRoleInfo		RoleInfo;//玩家数据
	BYTE			bTableTypeID;
	BYTE			SeatID;
	BYTE			LauncherType;
	BYTE			BackgroundImage;
	BYTE			RateIndex;
	int				Energy;
};
struct CL_Cmd_ResetLogonGameServer : public NetCmd
{
	DWORD			UserID;
	DWORD			RandID;
	DWORD			VersionID;
	DWORD			PathCrc;
};
struct LC_Cmd_ResetLogonGameServer : public NetCmd
{
};
//struct CL_Cmd_AccountChangePassword : public NetCmd
//{
//	DWORD	dwUserID;
//	DWORD	PasswordCrc1;
//	DWORD	PasswordCrc2;
//	DWORD	PasswordCrc3;
//	DWORD	OldPasswordCrc1;
//	DWORD	OldPasswordCrc2;
//	DWORD	OldPasswordCrc3;
//};
//struct LC_Cmd_AccountChangePassword : public NetCmd
//{
//	DWORD	dwUserID;
//	bool	Result;
//};
struct GL_Cmd_RsgGameServer : public NetCmd
{
	//DWORD	Ip;//注册当前GameServer去Logon上面
	//WORD	Port;
	DWORD GameConfigID;
};
//struct GL_Cmd_OnlinePlayerSum : public NetCmd
//{
//	WORD	PlayerSum;//在线玩家人数
//};
struct CL_Cmd_ResetPassword : public NetCmd//修改密码
{
	DWORD		OldPasswordCrc1;
	DWORD		OldPasswordCrc2;
	DWORD		OldPasswordCrc3;
	DWORD		NewPasswordCrc1;
	DWORD		NewPasswordCrc2;
	DWORD		NewPasswordCrc3;
};
struct LC_Cmd_ResetPassword : public NetCmd//修改密码的结果
{
	bool		Result;
	DWORD		NewPasswordCrc1;
	DWORD		NewPasswordCrc2;
	DWORD		NewPasswordCrc3;
};
struct CL_Cmd_AccountResetAccount : public NetCmd
{
	TCHAR	NewAccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
};
struct LC_Cmd_AccountResetAccount : public NetCmd
{
	bool	Result;
};
struct LC_Cmd_LogonQueueWrite : public NetCmd
{
	DWORD	WriteIndex;
	DWORD	WriteSec;
};
struct GL_Cmd_ResetPassword : public NetCmd
{
	DWORD	dwUserID;
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
};
struct GL_Cmd_ResetAccount : public NetCmd
{
	DWORD	dwUserID;
	TCHAR	NewAccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
};
struct CL_Cmd_GetNewAccount : public NetCmd
{
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
	TCHAR	MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	BYTE	PlateFormID;//平台的ID
	DWORD   VersionID;
	DWORD	PathCrc;
};
struct LC_Cmd_GetNewAccount : public NetCmd
{
	bool	Result;
	TCHAR	NewAccountName[ACCOUNT_LENGTH + 1];
	DWORD	PasswordCrc1;
	DWORD	PasswordCrc2;
	DWORD	PasswordCrc3;
};
struct LC_Cmd_CheckVersionError : public NetCmd
{
	DWORD   VersionID;
	DWORD	PathCrc;
};
struct LC_Cmd_AccountIsFreeze : public NetCmd
{
	__int64 EndTime;
};
struct CL_Cmd_ChannelLogon : public NetCmd
{
	BYTE				PlateFormID;//平台的ID
	DWORD				VersionID;
	DWORD				PathCrc;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	ChannelUserInfo		channelInfo;
};
struct LO_Cmd_ChannelLogon : public NetCmd
{
	DWORD				ClientID;
	BYTE				PlateFormID;//平台的ID
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	ChannelUserInfo		channelInfo;
};
struct OL_Cmd_ChannelLogon : public NetCmd
{
	//在运营服务器上 对外打交道 
	DWORD				ClientID;
	BYTE				PlateFormID;//平台的ID
	bool				Result;//登陆的结果
	DWORD				ChannelID;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	ChannelUserInfo		channelInfo;
};
struct LC_Cmd_ChannelLogon : public NetCmd
{
	bool				Result;
};

struct WeiXinUserCodeInfo
{
	//CHAR			Code[xx];//或者为 OpenID
	BYTE			HandleSum;
	DWORD			Sum;
	BYTE			Array[1];
};
struct CL_Cmd_WeiXinLogon : public NetCmd
{
	BYTE				PlateFormID;//平台的ID
	DWORD				VersionID;
	DWORD				PathCrc;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	WeiXinUserCodeInfo  CodeInfo;
};
struct LO_Cmd_WeiXinLogon : public NetCmd
{
	BYTE				PlateFormID;//平台的ID
	DWORD				ClientID;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	WeiXinUserCodeInfo  CodeInfo;
};
struct OL_Cmd_WeiXinLogon : public NetCmd
{
	BYTE				PlateFormID;//平台的ID
	DWORD				ClientID;
	bool				Result;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	WeiXinUserCodeInfo  OpenInfo;
};
struct LC_Cmd_WeiXinLogon : public NetCmd
{
	bool				Result;
};

struct QQUserCodeInfo
{
	//CHAR			Token[xx];//或者为 OpenID
	BYTE			HandleSum;
	DWORD			Sum;
	BYTE			Array[1];
};
struct CL_Cmd_QQLogon : public NetCmd
{
	BYTE				PlateFormID;//平台的ID
	DWORD				VersionID;
	DWORD				PathCrc;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	QQUserCodeInfo		TokenInfo;
};
struct LO_Cmd_QQLogon : public NetCmd
{
	BYTE				PlateFormID;//平台的ID
	DWORD				ClientID;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	QQUserCodeInfo		TokenInfo;
};
struct OL_Cmd_QQLogon : public NetCmd
{
	BYTE				PlateFormID;//平台的ID
	DWORD				ClientID;
	bool				Result;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
	QQUserCodeInfo		OpenInfo;
};
struct LC_Cmd_QQLogon : public NetCmd
{
	bool				Result;
};

struct LC_Cmd_RsgNewAccount : public NetCmd
{
	//当当前账号为新注册的时候
	BYTE				LogonType;//1:Channel 2:WeiXin 3:QQ
};

enum PhoneLogonError
{
	PLE_Success = 0,//
	PLE_LogonExists = 1,//手机号码登陆已经存在 
	PLE_PasswordFailed = 2,//手机验证码发送失败
	PLE_PasswordError = 3,//手机验证码错误
	PLE_SystemError = 4,//系统错误
};

struct CL_Cmd_PhoneLogon : public NetCmd
{
	UINT64				PhoneNumber;
};
struct LO_Cmd_PhoneLogon : public NetCmd
{
	UINT64				PhoneNumber;
	DWORD				ClientID;
};
struct OL_Cmd_PhoneLogon : public NetCmd
{
	UINT64				PhoneNumber;
	DWORD				ClientID;
	BYTE				ErrorID;
};
struct LC_Cmd_PhoneLogon : public NetCmd
{
	UINT64				PhoneNumber;
	BYTE				ErrorID;
};
struct CL_Cmd_PhoneLogonCheck : public NetCmd
{
	UINT64				PhoneNumber;
	DWORD				Password;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];
	DWORD				Crc1;
	DWORD				Crc2;
	DWORD				Crc3;
	bool				IsFirOrSec;
};
struct LO_Cmd_PhoneLogonCheck : public NetCmd
{
	UINT64				PhoneNumber;
	DWORD				Password;
	DWORD				ClientID;
	DWORD				ClientIP;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];
	DWORD				Crc1;
	DWORD				Crc2;
	DWORD				Crc3;
	bool				IsFirOrSec;
};
struct OL_Cmd_PhoneLogonCheck : public NetCmd
{
	UINT64				PhoneNumber;
	DWORD				ClientID;
	bool				Result;
	BYTE				ErrorID;
	DWORD				ClientIP;
	TCHAR				MacAddress[MAC_ADDRESS_LENGTH + 1];
	DWORD				Crc1;
	DWORD				Crc2;
	DWORD				Crc3;
	bool				IsFirOrSec;
};
struct LC_Cmd_PhoneLogonCheck : public NetCmd
{
	bool				Result;
	DWORD				dwUserID;
	BYTE				ErrorID;
};
struct LC_Cmd_SaveAccountInfo : public NetCmd
{
	TCHAR			AccountName[ACCOUNT_LENGTH + 1];
	DWORD			PasswordCrc1;
	DWORD			PasswordCrc2;
	DWORD			PasswordCrc3;
};
enum TableCmd
{
	//玩家登陆成功后 可以选择进出桌子
	CL_Sub_JoinTable = 1,//进入桌子
	LC_Sub_JoinTable = 2,//进入桌子的返回
	CL_Sub_LeaveTable = 3,
	//桌子上消息同步
	LC_Sub_OtherUserInfo = 4,//其他玩家的消息
	LC_Sub_UserLeave = 5,//玩家离开桌子

	LC_Sub_MeSeatID = 6,

	//桌子上其他玩家的数据修改
	LC_TableChangeRoleLevel = 10,
	LC_TableChangeRoleGender = 11,
	LC_TableChangeRoleNickName = 12,
	LC_TableChangeRoleFaceID = 13,
	LC_TableChangeRoleGlobe = 14,
	LC_TableChangeRoleExp = 15,
	LC_TableChangeRoleTitle = 16,
	LC_TableChangeRoleAchievementPoint = 17,
	LC_TableChangeRoleCharmValue = 18,
	LC_TableChangeRoleIpAddress = 19,
	LC_TableChangeRoleVipLevel = 20,
	LC_TableChangeRoleIsInMonthCard = 21,


	LC_ResetOtherUserInfo = 30,

	LC_LeaveTableByServer = 31,//服务器让玩家离开桌子

};
struct CL_JoinTable : public NetCmd
{
	BYTE		bTableTypeID;//选择进入桌子的类型
};
struct LC_JoinTableResult : public NetCmd
{
	bool		Result;
	BYTE		bTableTypeID;
	BYTE		SeatID;
	BYTE		LauncherType;
	BYTE		BackgroundImage;
	BYTE		RateIndex;
	int			Energy;
};
struct CL_LeaveTable : public NetCmd
{
};
struct LC_OtherUserInfo : public NetCmd
{
	tagRemoteRoleInfo UserInfo;
};
struct LC_OtherUserLeave : public NetCmd
{
	DWORD		dwUserID;
};
struct LC_Cmd_MeSeatID : public NetCmd
{
	BYTE		SeatID;
};
struct LC_Cmd_TableChangeRoleLevel : public NetCmd
{
	DWORD		dwDestUserID;
	WORD		wLevel;
};
struct LC_Cmd_TableChangeRoleGender : public NetCmd
{
	DWORD		dwDestUserID;
	bool		bGender;
};
struct LC_Cmd_TableChangeRoleNickName : public NetCmd
{
	DWORD       dwDestUserID;
	TCHAR		NickName[MAX_NICKNAME + 1];
};
struct LC_Cmd_TableChangeRoleFaceID : public NetCmd
{
	DWORD		dwDestUserID;
	DWORD		dwDestFaceID;
};
struct LC_Cmd_TableChangeRoleGlobe : public NetCmd
{
	DWORD		dwDestUserID;
	int			dwGlobelSum;
};
struct LC_Cmd_TableChangeRoleExp : public NetCmd
{
	DWORD		dwDestUserID;
	DWORD		dwExp;
};
struct LC_Cmd_TableChangeRoleTitle : public NetCmd
{
	DWORD		dwDestUserID;
	BYTE		TitleID;
};
struct LC_Cmd_TableChangeRoleAchievementPoint : public NetCmd
{
	DWORD		dwDestUserID;
	DWORD		dwAchievementPoint;
};
struct LC_Cmd_TableChangeRoleCharmValue : public NetCmd
{
	DWORD		dwDestUserID;
	DWORD		CharmInfo[MAX_CHARM_ITEMSUM];
};
struct LC_Cmd_TableChangeRoleIpAddress : public NetCmd
{
	DWORD		dwDestUserID;
	TCHAR		IpAddress[MAX_ADDRESS_LENGTH_IP+1];
};
struct LC_Cmd_ResetOtherUserInfo : public NetCmd
{
	tagRemoteRoleInfo UserInfo;
};
struct LG_Cmd_ChannelInfo : public NetCmd
{
	DWORD				dwUserID;
	DWORD				ClientID;
	ChannelUserInfo		channelUserInfo;
};
struct LC_Cmd_LeaveTableByServer : public NetCmd
{
	bool				IsReturnLogon;
};
struct LC_Cmd_TableChangeRoleVipLevel : public NetCmd
{
	DWORD				dwDestUserID;
	BYTE				VipLevel;
};
struct LC_Cmd_TableChangeRoleIsInMonthCard : public NetCmd
{
	DWORD				dwDestUserID;
	bool				IsInMonthCard;
};
//---------------------------------------------------------------game
enum GameSub
{
	GAME_SUB_COMBO = 50,
};

struct Game_Combo : public NetCmd
{
	BYTE seat;
	BYTE byOrder;
	bool bbuff;
};
//---------------------------------------------------------------中央服务器
enum CenterSub
{
	CL_Sub_UserEnter = 1,

	CL_Sub_UserEnterFinish = 5,

	CL_Sub_RsgFinish = 6,

	CL_Sub_UserLeave = 10,

	CC_UserLeaveGame = 11,

	//CG_DayChange = 12,

	CG_CenterTimeChange = 12,
};
struct CL_UserEnter : public NetCmd
{
	//发送用户登陆Center的消息 消息包含玩家的远程数据 
	tagCenterRoleInfo RoleInfo;
	bool			  IsRobot;
};
struct CL_UserEnterFinish : public NetCmd
{
	DWORD			dwUserID;
};
struct CL_UserLeave : public NetCmd
{
	DWORD				dwUserID;
};

enum BalancedSub
{
	CL_Sub_GateInfo = 1,

	CL_Sub_GetGate = 2,
	LC_Sub_GetGate = 3,
};
struct CL_GateInfo : public NetCmd
{
	__int64		GroupID;
	DWORD		Ip;
	WORD		Port;
	WORD		UserSum;
	WORD		UserMaxSum;
};
struct LC_GetGate : public NetCmd
{
	bool		Result;
	DWORD		Ip;
	WORD		Port;
};
struct CC_Cmd_UserLeaveGame : public NetCmd
{
	//清理玩家 离线
	DWORD		dwUserID;
};
//struct CG_Cmd_DayChange
//{
//	bool		MonthIsChange;
//};
struct CG_Cmd_CenterTimeChange : public NetCmd
{
	//有中央服务器控制全部的服务器进行更新操作 1分钟进行一次更新
	__int64	TimeLog;//中央服务器的时间
};
//-----------------------------------------------------物品	

enum ItemSub
{
	CL_GetUserItem = 1,
	LC_GetUserItem = 2,
	//LC_GetUserItemFinish = 3,

	LC_AddUserItem = 4,
	LC_DelUserItem = 5,
	LC_ChangeUserItem = 6,

	CL_OnUseItem = 7,
	LC_OnUseItem = 8,

	CL_OnAcceptItem = 9,
	LC_OnAcceptItem = 10,
};
struct CL_Cmd_GetUserItem : public NetCmd
{

};
struct LC_Cmd_GetUserItem : public NetCmd
{
	BYTE		States;
	WORD		Sum;
	tagItemInfo Array[1];
};
//struct LC_Cmd_GetUserItemFinish : public NetCmd
//{
//};
struct LC_Cmd_AddUserItem : public NetCmd
{
	tagItemInfo ItemInfo;
};
struct LC_Cmd_DelUserItem : public NetCmd
{
	DWORD		ItemOnlyID;
};
struct LC_Cmd_ChangeUserItem : public NetCmd
{
	DWORD		ItemOnlyID;
	DWORD		ItemSum;
};
struct CL_Cmd_OnUseItem : public NetCmd
{
	DWORD		ItemOnlyID;
	DWORD		ItemID;
	DWORD		ItemSum;
};
struct LC_Cmd_OnUseItem : public NetCmd
{
	bool		Result;
	DWORD		ItemOnlyID;
	DWORD		ItemID;
	DWORD		ItemSum;
};
struct CL_Cmd_OnAcceptItem : public NetCmd
{
	DWORD		dwID;
	DWORD		ItemOnlyID;
	DWORD		ItemID;
	DWORD		ItemSum;
	bool		GameIDorUserID;
};
struct LC_Cmd_OnAcceptItem : public NetCmd
{
	bool		Result;
	DWORD		dwDestUserID;
	DWORD		ItemOnlyID;
	DWORD		ItemID;
	DWORD		ItemSum;
};
//------------------------------------------------Relation
enum RelationSub
{
	CL_GetUserRelation = 1,
	LC_GetUserRelation = 2,
	//LC_GetUserRelationFinish = 3,

	CL_AddUserRelation = 4,
	LC_AddUserRelation = 5,
	CL_DelUserRelation = 6,
	LC_DelUserRelation = 7,
	//CL_ChangeUserRelation = 8,
	LC_ChangeUserRelation = 9,

	//Center
	CC_LoadUserRelation = 10,
	CC_LoadBeUserRelation = 11,
	//CC_LoadUserRelationFinish = 12,
	CC_AddUserRelation = 13,
	CC_DelUserRelation = 14,
	CC_ChangeUserRelation = 15,

	CC_AddBeUserRelation = 16,
	CC_DelBeUserRelation = 17,
	CC_ChangeBeUserRelation = 18,

	CC_RoleChangeOnline = 19,
	LC_RoleChangeOnline = 20,

	//
	CC_ChangeRelationLevel = 22,
	CC_ChangeRelationFaceID = 23,
	CC_ChangeRelationNickName = 24,
	CC_ChangeRelationGender = 25,
	CC_ChangeRelationTitle = 26,
	CC_ChangeRelationAchievementPoint = 27,
	CC_ChangeRelationCharmValue = 28,
	CC_ChangeRelationIP	 = 29,
	 
	//
	LC_ChangeRelationLevel = 30,
	LC_ChangeRelationFaceID = 31,
	LC_ChangeRelationNickName = 32,
	LC_ChangeRelationGender = 33,
	LC_ChangeRelationTitle = 34,
	LC_ChangeRelationAchievementPoint = 35,
	LC_ChangeRelationCharmValue = 36,
	LC_ChangeRelationIP = 37,

	CC_ChangeRelationIsShowIpAddress = 40,

	CC_ChangeRelationVipLevel		= 41,
	LC_ChangeRelationVipLevel		= 42,
	CC_ChangeRelationIsInMonthCard	= 43,
	LC_ChangeRelationIsInMonthCard	= 44,
};
struct CL_Cmd_GetUserRelation : public NetCmd
{
};
struct LC_Cmd_GetUserRelation : public NetCmd
{
	BYTE	States;
	WORD	Sum;
	tagRoleRelationClient Array[1];
};
//struct LC_Cmd_GetUserRelationFinish : public NetCmd
//{
//};
struct CL_Cmd_AddUserRelation : public NetCmd
{
	DWORD  dwDestUserID;
	BYTE	bRelationType;
};
struct LC_Cmd_AddUserRelation : public NetCmd
{
	bool	Result;
	tagRoleRelationClient RelationInfo;
};
struct CL_Cmd_DelUserRelation : public NetCmd
{
	DWORD	dwDestUserID;
};
struct LC_Cmd_DelUserRelation : public NetCmd
{
	DWORD	dwDestUserID;
	bool	Result;
};
//struct CL_Cmd_ChangeUserRelation : public NetCmd
//{
//	DWORD	dwDestUserID;
//	BYTE	RelationType;
//};
struct LC_Cmd_ChangeUserRelation : public NetCmd
{
	DWORD   dwDestUserID;
	bool	Result;
	BYTE	RelationType;
};
struct CC_Cmd_GetUserRelation : public NetCmd
{
	DWORD	dwUserID;
	BYTE	States;
	WORD	Sum;
	tagRoleRelation Array[1];
};
struct CC_Cmd_LoadBeUserRelation : public NetCmd
{
	DWORD	dwUserID;
	BYTE	States;
	WORD	Sum;
	tagBeRoleRelation Array[1];
};
//struct CC_Cmd_GetUserRelationFinish : public NetCmd
//{
//	DWORD	dwUserID;
//};
struct CC_Cmd_AddUserRelation : public NetCmd
{
	DWORD  dwUserID;
	tagRoleRelation RelationInfo;
};
struct CC_Cmd_DelUserRelation : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
};
struct CC_Cmd_ChangeUserRelation : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	BYTE	RelationType;
};
struct CC_Cmd_AddBeUserRelation : public NetCmd
{
	DWORD	dwUserID;
	tagBeRoleRelation RelationInfo;
};
struct CC_Cmd_DelBeUserRelation : public NetCmd
{
	DWORD  dwUserID;
	DWORD  dwDestUserID;
};
struct CC_Cmd_ChangeBeUserRelation : public NetCmd
{
	DWORD dwUserID;
	DWORD dwDestUserID;
	BYTE  RelationType;
};
struct CC_Cmd_ChangeUserOline : public NetCmd
{
	DWORD dwSrcUserID;
	DWORD dwDestUserID;
	bool  IsOnline;
};
struct LC_Cmd_ChangeUserOline : public NetCmd
{
	//DWORD dwSrcUserID;
	DWORD dwDestUserID;
	bool  IsOnline;
};
struct CC_Cmd_ChangeRelationLevel : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	WORD	wLevel;
};
struct CC_Cmd_ChangeRelationFaceID : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	DWORD	dwFaceID;
};
struct CC_Cmd_ChangeRelationNickName : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	TCHAR	cNickName[MAX_NICKNAME + 1];
};
struct CC_Cmd_ChangeRelationGender : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	bool	bGender;
};
struct CC_Cmd_ChangeRelationTitle : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	BYTE	TitleID;
};
struct CC_Cmd_ChangeRelationAchievementPoint : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	DWORD   dwAchievementPoint;
};
struct CC_Cmd_ChangeRelationCharmValue : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	DWORD	CharmInfo[MAX_CHARM_ITEMSUM];
};
struct CC_Cmd_ChangeRelationIP : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	DWORD	ClientIP;
};
struct LC_Cmd_ChangeRelationLevel : public NetCmd
{
	DWORD	dwDestUserID;
	WORD	wLevel;
};
struct LC_Cmd_ChangeRelationFaceID : public NetCmd
{
	DWORD	dwDestUserID;
	DWORD	dwFaceID;
};
struct LC_Cmd_ChangeRelationNickName : public NetCmd
{
	DWORD	dwDestUserID;
	TCHAR	cNickName[MAX_NICKNAME + 1];
};
struct LC_Cmd_ChangeRelationGender : public NetCmd
{
	DWORD	dwDestUserID;
	bool	bGender;
};
struct LC_Cmd_ChangeRelationTitle : public NetCmd
{
	DWORD	dwDestUserID;
	BYTE	TitleID;
};
struct LC_Cmd_ChangeRelationAchievementPoint : public NetCmd
{
	DWORD	dwDestUserID;
	DWORD   dwAchievementPoint;
};
struct LC_Cmd_ChangeRelationCharmValue : public NetCmd
{
	DWORD	dwDestUserID;
	DWORD	CharmInfo[MAX_CHARM_ITEMSUM];
};
struct LC_Cmd_ChangeRelationIP : public NetCmd
{
	DWORD	dwDestUserID;
	//DWORD	ClientIP; //xuda
	TCHAR			IPAddress[MAX_ADDRESS_LENGTH_IP+1];
};
struct CC_Cmd_ChangeRelationIsShowIpAddress: public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	bool	IsShowIpAddress;
};
struct CC_Cmd_ChangeRelationVipLevel : public NetCmd
{
	DWORD  dwUserID;
	DWORD  dwDestUserID;
	BYTE   VipLevel;
};
struct LC_Cmd_ChangeRelationVipLevel : public NetCmd
{
	DWORD  dwDestUserID;
	BYTE   VipLevel;
};
struct CC_Cmd_ChangeRelationIsInMonthCard : public NetCmd
{
	DWORD dwUserID;
	DWORD  dwDestUserID;
	bool  IsInMonthCard;
};
struct LC_Cmd_ChangeRelationIsInMonthCard : public NetCmd
{
	DWORD  dwDestUserID;
	bool  IsInMonthCard;
};
//-----------------------------------------------------mail
enum MailSub //系统邮件应该是由中央服务器进行发送的 然后同步到GameServer去
{
	CL_GetUserMail = 1,
	LC_GetUserSystemMail = 2,
	LC_GetUserNormalMail = 3,
	//LC_GetUserMailFinish = 4,

	//CL_SendUserMail = 5,//客户端向服务器端发送一个邮件
	//LC_SendUserMail = 6,//客户端接收到一个邮件消息 表示 发送失败或者成功
	//LC_SendBeUserMail = 7,

	CL_DelUserMail = 8,
	LC_DelUserMail = 9,

	CL_GetMailItem = 10,
	LC_GetMailItem = 11,

	//LC_ChangeMailIsRead = 12,

	CL_ReadMail = 13,
	LC_ReadNormalMail = 14,
	LC_ReadSystemMail = 15,
	LC_ReadMailError = 16,

	//发送邮件到中央服务器进行中转
	//CC_AddUserMail = 17,//玩家发送邮件 或者 系统发送邮件的时候 触发
	//CC_AddBeUserMain = 18,// 在中央服务器接收到发送邮件的时候 如果被接收者在线就直接触发 转发过去

	LC_AddNormalTitle = 19,
	LC_AddSystemTitle = 20,

	CC_SendSystemMail = 21,
	CC_SendBeSystemMail = 22,

	//发送邮件的完整的流程 7个命令
	CL_SendUserMail = 23,
	GC_SendUserMail = 24,
	CG_SendUserMail = 25,
	GC_SendUserMailResult = 26,
	CG_SendUserMailResult = 27,
	LC_SendUserMailResult = 28,
};

struct CL_Cmd_GetUserMail : public NetCmd
{
	//DWORD  dwUserID;
};
struct LC_Cmd_NormalMail : public NetCmd
{
	BYTE				States;
	WORD				Sum;
	tagNormalMailTitle	Array[1];
};
struct LC_Cmd_SystemMail : public NetCmd
{
	BYTE				States;
	WORD				Sum;
	tagSystemMailTitle  Array[1];
};
//struct LC_Cmd_GetMailFinish : public NetCmd
//{
//};


struct CL_Cmd_SendUserMail : public NetCmd
{
	DWORD				DestUserID;//发送方的ID
	TCHAR				Context[MAX_MAIL_CONTEXT + 1];//邮件内容
};
struct GC_Cmd_SendUserMail : public NetCmd
{
	DWORD				DestUserID;
	tagRoleMail			MailInfo;
};
struct CG_Cmd_SendUserMail : public NetCmd
{
	DWORD				DestUserID;
	tagRoleMail			MailInfo;
};
struct GC_Cmd_SendUserMailResult : public NetCmd
{
	DWORD				SrcUserID;
	DWORD				DestUserID;
	bool				Result;
};
struct CG_Cmd_SendUserMailResult : public NetCmd
{
	DWORD				SrcUserID;
	DWORD				DestUserID;
	bool				Result;
};
struct LC_Cmd_SendUserMailResult : public NetCmd
{
	DWORD				DestUserID;
	bool				Result;
};


//struct LC_Cmd_SendBeNormalMail
//{
//	DWORD				dwUserID;
//	tagNormalMailTitle	MailInfo;
//};

struct CL_Cmd_DelMail : public NetCmd
{
	DWORD				dwMailID;
};
struct LC_Cmd_DelMail : public NetCmd
{
	DWORD				dwMailID;
	bool				Result;
};
struct CL_Cmd_GetMailItem : public NetCmd
{
	DWORD				dwMailID;
};
struct LC_Cmd_GetMailItem : public NetCmd
{
	DWORD				dwMailID;
	bool				Result;
};
//struct LC_Cmd_ChangeMailIsRead
//{
//	DWORD				dwUserID;
//	DWORD				dwMailID;
//};
struct CL_Cmd_GetMailContext : public NetCmd
{
	DWORD				dwMailID;
};
struct LC_Cmd_GetNormalMail : public NetCmd
{
	tagNormalMail		MailInfo;
};
struct LC_Cmd_GetSystemMail : public NetCmd
{
	//DWORD				dwUserID;
	tagSystemMail		MailInfo;
};
struct LC_Cmd_GetMailError : public NetCmd
{
	//DWORD				dwUserID;
	DWORD				MailID;
};
//struct CC_Cmd_AddUserMail : public NetCmd
//{
//	DWORD				dwUserID;
//	tagRoleMail			MailInfo;
//};
//struct CC_Cmd_AddBeUserMail : public NetCmd
//{
//	DWORD				dwDestRoleID;
//	tagRoleMail			MailInfo;
//};
struct LC_Cmd_AddNormalMail : public NetCmd
{
	tagNormalMailTitle	MailInfo;
};
struct LC_Cmd_AddSystemMail : public NetCmd
{
	tagSystemMailTitle	MailInfo;
};
struct CC_Cmd_SendSystemMail : public NetCmd
{
	DWORD				dwDestUserID;
	tagRoleMail			MailInfo;
};
struct CC_Cmd_SendBeSystemMail : public NetCmd
{
	DWORD				dwDestUserID;
	tagRoleMail			MailInfo;
};
//Role---------------------------------------------------------------
enum RoleSub
{
	LC_ChangeRoleExp = 1,
	LC_ChangeRoleLevel = 2,
	LC_ChangeRoleGender = 3,
	LC_ChangeRoleNickName = 4,
	LC_ChangeRoleFaceID = 5,
	LC_ChangeRoleGlobe = 6,
	LC_ChangeRoleMedal = 7,
	LC_ChangeRoleCurrency = 8,
	LC_ChangeRoleTitle = 9,
	LC_ChangeRoleAchievementPoint = 10,
	LC_ChangeRoleCharmValue = 11,
	LC_ChangeRoleSendGiffSum = 12,
	LC_ChangeRoleAcceptGiffSum = 13,
	LC_ChangeRoleTaskStates = 14,
	LC_ChangeRoleAchievementStates = 15,
	LC_ChangeRoleActionStates = 16,

	//向中央 服务器发送的
	//CC_ChangeRoleExp = 20,
	CC_ChangeRoleLevel = 21,
	CC_ChangeRoleGender = 22,
	CC_ChangeRoleNickName = 23,
	CC_ChangeRoleFaceID = 24,
	CC_ChangeRoleTitle = 25,
	CC_ChangeRoleAchievementPoint = 26,
	CC_ChangeRoleCharmValue = 27,
	CC_ChangeRoleClientIP = 28,
	/*CC_ChangeRoleGlobe = 25,
	CC_ChangeRoleMedal = 26,
	CC_ChangeRoleCurrency = 27,*/

	//客户端向服务器端 发送修改的命令
	CL_ChangeRoleGender = 40,
	CL_ChangeRoleNickName = 41,
	CL_ChangeRoleNormalFaceID = 42,
	//头像的修改 需要注意点 
	//CL_RequestUserFaceID = 43,//请求更换头像
	//LC_ResponseUserFaceID = 44,
	//CL_UpLoadingchunk = 45,
	//LC_UpLoadingResponse = 46,
	//LC_UpLoadingFinish = 47,
	//LC_UpLoadFtpError = 48,//修改头像失败了

	LC_DayChange = 49,

	GF_SaveImageData = 50,
	FG_SaveImageData = 51,

	LC_ChangeRoleAchievementIndex = 52,

	LC_ResetRoleGlobel = 53,


	CL_ResetRoleInfo = 54,
	LC_ResetRoleInfo = 55,

	LC_ChangeRoleCheckData = 56,

	CL_ChangeRoleIsShowIpAddress = 57,
	LC_ChangeRoleIsShowIpAddress = 58,
	CC_ChangeRoleIsShowIpAddress = 59,

	LC_NewDay = 60,

	CC_ChangeRoleIsOnline = 61,

	LC_ChangeRoleExChangeStates = 62,
	LC_ChangeRoleTotalRecharge = 63,

	LC_RoleProtect  = 64,

	LC_ChangeRoleIsFirstPayGlobel = 65,
	LC_ChangeRoleIsFirstPayCurrcey = 66,

	LC_ChangeRoleMonthCardInfo	= 67,
	LC_ChangeRoleRateValue		= 68,
	CL_ChangeRoleRateValue	    = 69,

	CL_GetMonthCardReward	    = 70,
	LC_GetMonthCardReward       = 71,

	LC_ChangeRoleVipLevel       = 72,

	CC_ChangeRoleVipLevel       = 73,
	CC_ChangeRoleIsInMonthCard  = 74,

	LC_ChangeRoleCashSum		= 75,
	CL_RoleProtect = 76,
	LC_RoleProtectStatus = 77,

	CC_ChangeRoleParticularStates = 78,
	LC_ChangeRoleParticularStates = 79,

	//LC_ChangeRoleLockInfo = 78,
	LC_SetSecondPassword		= 80,

	CL_ChangeSecondPassword     = 81,
	LC_ChangeSecondPassword     = 82,

	CL_ChangeRoleShareStates	= 83,
	LC_ChangeRoleShareStates    = 84,

	LC_OpenShareUI				= 85,//开启分享

	//更新账号
	CL_UpdateAccount			= 86,
	LO_UpdateAccount			= 87,
	OL_UpdateAccount			= 88,
};
struct LC_Cmd_ChangRoleExp : public NetCmd
{
	//DWORD		dwUserID;
	DWORD		dwExp;
};
struct LC_Cmd_ChangeRoleLevel : public NetCmd
{
	//DWORD		dwUserID;
	WORD		wLevel;
};
struct LC_Cmd_ChangeRoleGender : public NetCmd
{
	//DWORD		dwUserID;
	bool		bGender;
};
struct LC_Cmd_ChangeRoleNickName : public NetCmd
{
	//DWORD		dwUserID;
	bool		Result;
	TCHAR		NickName[MAX_NICKNAME + 1];
};
struct LC_Cmd_ChangeRoleFaceID : public NetCmd
{
	//DWORD		dwUserID;
	DWORD		dwFaceID;
};
struct LC_Cmd_ChangeRoleGlobe : public NetCmd
{
	//DWORD		dwUserID;
	int		dwGlobeNum;
};
struct LC_Cmd_ChangeRoleMedal : public NetCmd
{
	//DWORD		dwUserID;
	DWORD       dwMedalNum;
	DWORD		TotalUseMedal;
};
struct LC_Cmd_ChangeRoleCurrency : public NetCmd
{
	//DWORD		dwUserID;
	DWORD		dwCurrencyNum;
};
struct LC_Cmd_ChangeRoleTitle : public NetCmd
{
	BYTE		TitleID;
};
struct LC_Cmd_ChangeRoleAchievementPoint : public NetCmd
{
	DWORD		dwAchievementPoint;
};
struct LC_Cmd_ChangeRoleCharmValue : public NetCmd
{
	DWORD		CharmInfo[MAX_CHARM_ITEMSUM];
};
struct LC_Cmd_ChangeRoleSendGiffSum : public NetCmd
{
	BYTE	SendGiffSum;
};
struct LC_Cmd_ChangeRoleAcceptGiffSum : public NetCmd
{
	BYTE	AcceptGiffSum;
};
struct LC_Cmd_ChangeRoleTaskStates : public NetCmd
{
	BYTE	Index;
	bool	States;
};
struct LC_Cmd_ChangeRoleAchievementStates : public NetCmd
{
	BYTE	Index;
	bool	States;
};
struct LC_Cmd_ChangeRoleActionStates : public NetCmd
{
	BYTE	Index;
	bool	States;
};

//struct CC_Cmd_ChangRoleExp : public NetCmd
//{
//	DWORD		dwUserID;
//	DWORD		dwExp;
//};
struct CC_Cmd_ChangeRoleLevel : public NetCmd
{
	DWORD		dwUserID;
	WORD		wLevel;
};
struct CC_Cmd_ChangeRoleGender : public NetCmd
{
	DWORD		dwUserID;
	bool		bGender;
};
struct CC_Cmd_ChangeRoleNickName : public NetCmd
{
	DWORD		dwUserID;
	TCHAR		NickName[MAX_NICKNAME + 1];
};
struct CC_Cmd_ChangeRoleFaceID : public NetCmd
{
	DWORD		dwUserID;
	DWORD		dwFaceID;
};
struct CC_Cmd_ChangeRoleTitle : public NetCmd
{
	DWORD		dwUserID;
	BYTE		TitleID;
};
struct CC_Cmd_ChangeRoleAchievementPoint : public NetCmd
{
	DWORD		dwUserID;
	DWORD		dwAchievementPoint;
};
struct CC_Cmd_ChangeRoleCharmValue : public NetCmd
{
	DWORD		dwUserID;
	DWORD		CharmInfo[MAX_CHARM_ITEMSUM];
};
struct CC_Cmd_ChangeRoleClientIP : public NetCmd
{
	DWORD		dwUserID;
	DWORD		ClientIP;
};

struct LC_Cmd_ChangeRoleAchievementIndex : public NetCmd
{
	DWORD		AchievementIndex;
};
//struct CC_Cmd_ChangeRoleGlobe
//{
//	DWORD		dwUserID;
//	DWORD		dwGlobeNum;
//};
//struct CC_Cmd_ChangeRoleMedal
//{
//	DWORD		dwUserID;
//	DWORD       dwMedalNum;
//};
//struct CC_Cmd_ChangeRoleCurrency
//{
//	DWORD		dwUserID;
//	DWORD		dwCurrencyNum;
//};
struct CL_Cmd_ChangeRoleGender : public NetCmd
{
	//玩家请求修改性别
	//DWORD		dwUserID;
	bool		bGender;
};
struct CL_Cmd_ChangeRoleNickName : public NetCmd
{
	//DWORD		dwUserID;
	TCHAR		NickName[MAX_NICKNAME + 1];
};
struct CL_Cmd_ChangeRoleNormalFaceID : public NetCmd
{
	//DWORD		dwUserID;
	DWORD		dwFaceID;
};

//struct CL_Cmd_RequestUserFaceID : public NetCmd
//{
//	//客户端玩家请求 上传头像
//	//DWORD		dwUserID;
//	WORD		Size;//请求上传的头像大小
//};
//struct LC_Cmd_ResponseUserFaceID : public NetCmd
//{
//	//回应客户端的请求
//	BYTE		AllowUpload;//是否可以上传
//};
//struct CL_Cmd_UpLoadingchunk : public NetCmd//变长命令
//{
//	WORD		StartIndex;
//	WORD		Size;
//	BYTE		ImgData[1];
//};
//struct LC_Cmd_UpLoadingResponse : public NetCmd
//{
//	BYTE		UpdateResult;
//};
//struct LC_Cmd_UpLoadingFinish : public NetCmd
//{
//	//DWORD		dwUserID;
//	DWORD		dwFaceID;
//};
struct LC_Cmd_DayChange : public NetCmd
{
	BYTE			Year;
	BYTE			Month;
	BYTE			Day;
	BYTE			Hour;
	BYTE			Min;
	BYTE			Sec;
	bool			IsDayUpdate;
};
struct GF_Cmd_SaveImageData : public NetCmd
{
	USHORT		Size;
	UINT_PTR	ID;
	UINT		ImgCrc;
	USHORT		ImgSize;
	char		ImgName[50];
	BYTE		ImgData[1];
};
struct FG_Cmd_SaveImageData : public NetCmd
{
	USHORT		Size;
	UINT_PTR	ID;
	UINT		Crc;
	UINT		Result;
};
struct LC_Cmd_ResetRoleGlobel : public NetCmd
{
	DWORD		dwTotalGlobel;
	DWORD		dwLotteryScore;
};
struct LC_Cmd_ResetRoleInfo : public NetCmd
{
	DWORD		  OperateIP;
	tagRoleInfo   RoleInfo;
};
struct LC_Cmd_ChangeRoleCheckData : public NetCmd
{
	DWORD			CheckData;
};
struct CL_Cmd_ChangeRoleIsShowIpAddress : public NetCmd
{
	bool			IsShowIpAddress;
};
struct LC_Cmd_ChangeRoleIsShowIpAddress : public NetCmd
{
	bool			IsShowIpAddress;
};
struct CC_Cmd_ChangeRoleIsShowIpAddress : public NetCmd
{
	DWORD			dwUserID;
	bool			IsShowIpAddress;
};
struct CC_Cmd_ChangeRoleIsOnline : public NetCmd
{
	DWORD			dwUserID;
	bool			IsOnline;
};
struct LC_Cmd_ChangeRoleExChangeStates : public NetCmd
{
	DWORD			States;
};
struct LC_Cmd_ChangeRoleTotalRecharge : public NetCmd
{
	DWORD			Sum;
};
struct LC_Cmd_ChangeRoleIsFirstPayGlobel : public NetCmd
{

};
struct LC_Cmd_ChangeRoleIsFirstPayCurrcey : public NetCmd
{

};
struct LC_Cmd_RoleProtect : public NetCmd
{

};

struct LC_Cmd_RoleProtectStatus : public NetCmd
{
	//BYTE setat;
	//bool poor;
};
struct CC_Cmd_ChangeRoleParticularStates : public NetCmd
{
	DWORD		dwUserID;
	DWORD		ParticularStates;
};
struct LC_Cmd_ChangeRoleParticularStates : public NetCmd
{
	DWORD		ParticularStates;
};
struct CL_Cmd_RoleProtect : public NetCmd
{

};

struct LC_Cmd_ChangeRoleMonthCardInfo : public NetCmd
{
	BYTE		MonthCardID;
	__int64     MonthCardEndTime;
};
struct CL_Cmd_ChangeRoleRateValue : public NetCmd
{
	BYTE		RateIndex;//需要解锁的倍率
};
struct LC_Cmd_ChangeRoleRateValue : public NetCmd
{
	int256		RateValue;
	BYTE		OpenRateIndex;
};
struct CL_Cmd_GetMonthCardReward : public NetCmd
{

};
struct LC_Cmd_GetMonthCardReward : public NetCmd
{
	bool	Result;
};
struct LC_Cmd_ChangeRoleVipLevel : public NetCmd
{
	BYTE	VipLevel;
};
struct CC_Cmd_ChangeRoleVipLevel : public NetCmd
{
	DWORD  dwUserID;
	BYTE   VipLevel;
};
struct CC_Cmd_ChangeRoleIsInMonthCard : public NetCmd
{
	DWORD  dwUserID;
	bool   IsInMonthCard;
};
struct LC_Cmd_ChangeRoleCashSum : public NetCmd
{
	BYTE	CashSum;
	DWORD   TotalCashSum;
};

struct CL_Cmd_ChangeRoleShareStates : public NetCmd
{
	bool  ShareStates;
};
struct LC_Cmd_ChangeRoleShareStates : public NetCmd
{
	bool  ShareStates;
};

struct LC_Cmd_OpenShareUI : public NetCmd
{

};


struct AccountSDKInfo
{
	//CHAR			Token/OpenID
	BYTE			HandleSum;
	DWORD			Sum;
	BYTE			Array[1];
};

struct CL_Cmd_UpdateAccount	 : public NetCmd
{
	BYTE			bType;
	AccountSDKInfo  Info;
};
struct LO_Cmd_UpdateAccount	 : public NetCmd
{
	DWORD			dwUserID;
	BYTE			bType;
	AccountSDKInfo  Info;
};
struct OL_Cmd_UpdateAccount	 : public NetCmd
{
	DWORD			dwUserID;
	bool			bGender;
	TCHAR			NickName[MAX_NICKNAME + 1];
	DWORD			dwFaceID;
};

/*struct LC_Cmd_ChangeRoleLockInfo : public NetCmd
{
	DWORD	LockStates;
	__int64 LockEndTime;
};*/
struct LC_Cmd_SetSecondPassword : public NetCmd
{
	//让玩家查询设置二级密码
};
struct CL_Cmd_ChangeSecondPassword : public NetCmd
{
	DWORD	dwOldCrc1;
	DWORD	dwOldCrc2;
	DWORD	dwOldCrc3;

	DWORD	dwNewCrc1;
	DWORD	dwNewCrc2;
	DWORD	dwNewCrc3;
};
struct LC_Cmd_ChangeSecondPassword : public NetCmd
{
	bool	Result;
};
//-------------------------------------------------------------------------玩家查询
enum QuerySub
{
	//查询集合
	CL_QueryUserByNickName = 1,//根据名称进行模糊查询 分页查询
	CL_QueryUserByUserID = 2,
	CL_QueryUserByGameID = 3,

	LC_QueryUserInfo = 10,
	//LC_QueryUserInfoFinish = 11,

	LC_QueryUserOnce = 12,
};
struct CL_Cmd_QueryUserByNickName : public NetCmd
{
	TCHAR		NickName[MAX_NICKNAME + 1];
	BYTE		Page;//页面数
	bool		IsOnline;//是否查询不在线的玩家 (可以进行控制的)
};
struct CL_Cmd_QueryUserByUserID : public NetCmd
{
	DWORD		dwDestUserID;
};
struct CL_Cmd_QueryUserByGameID : public NetCmd
{
	DWORD		GameID;
};
struct LC_Cmd_QueryUserInfo : public NetCmd
{
	WORD		Page;
	BYTE		States;
	WORD		Sum;
	tagQueryRoleInfoClient  Array[1];
};
struct LC_Cmd_QueryUserOnce : public NetCmd
{
	tagQueryRoleInfoClient  RoleInfo;
};
//struct LC_Cmd_QueryUserInfoFinish
//{
//
//};

//-------------------------------------------------------------------check
enum CheckSub
{
	/*CL_GetRoleCheckInfo = 1,
	LC_GetRoleCheckInfo = 2,*/

	CL_CheckNowDay = 4,
	LC_CheckNowDay = 5,

	CL_CheckOtherDay = 6,
	LC_CheckOtherDay = 7,

	//LC_CheckChange = 8,
};
struct LC_Cmd_CheckNowDay : public NetCmd
{
	bool		Result;
	WORD		DayRewardID;
	WORD		MonthRewardID;
};
struct CL_Cmd_CheckOtherDay : public NetCmd
{
	BYTE	   Day;
};
struct LC_Cmd_CheckOtherDay : public NetCmd
{
	bool       Result;
	BYTE	   Day;
	WORD	   DayRewardID;
	WORD	   MonthRewardID;
};
//--------------------------------------------------------------------Task
enum TaskSub
{
	CL_GetRoleTaskInfo = 1,
	LC_GetRoleTaskInfo = 2,
	//LC_GetRoleTaskInfoFinish = 3,

	LC_TaskAllEventFinish = 5,

	CL_GetTaskReward = 6,
	LC_GetTaskReward = 7,

	//LC_ResetTask  = 8,

	CL_GetOnceTaskInfo = 9,
	LC_GetOnceTaskInfo = 10,


	CG_GetGlobelTaskInfo = 11,

	LC_JoinTask = 12,
	LC_ClearTask = 13,
	LC_DelTask = 14,

};
struct CL_Cmd_GetRoleTaskInfo : public NetCmd
{
	
};
struct LC_Cmd_GetRoleTaskInfo : public NetCmd
{
	BYTE			States;
	WORD			Sum;
	tagRoleTaskInfo Array[1];
};
//struct LC_Cmd_GetRoleTaskInfoFinish : public NetCmd
//{
//
//};
struct LC_Cmd_TaskAllEventFinish : public NetCmd
{
	BYTE			TaskID;
};
struct CL_Cmd_GetTaskReward : public NetCmd
{
	BYTE			TaskID;
};
struct LC_Cmd_GetTaskReward : public NetCmd
{
	BYTE	TaskID;
};
struct CL_Cmd_GetOnceTaskInfo : public NetCmd
{
	BYTE	TaskID;
};
struct LC_Cmd_GetOnceTaskInfo : public NetCmd
{
	tagRoleTaskInfo		TaskInfo;
};
struct CG_Cmd_GetGlobelTaskInfo : public NetCmd
{
	BYTE		States;
	WORD		Sum;
	BYTE		Array[1];
};
struct LC_Cmd_JoinTask : public NetCmd
{
	BYTE		TaskID;//接取的新任务
};
struct LC_Cmd_DelTask : public NetCmd
{
	BYTE		TaskID;//待删除的任务
};
//Achievement --------------------------------------------------------------------
enum AchievementSub
{
	CL_GetRoleAchievementInfo = 1,
	LC_GetRoleAchievementInfo = 2,
	//LC_GetRoleAchievementInfoFinish = 3,

	LC_AchievementAllEventFinish = 5,

	CL_GetAchievementReward = 6,
	LC_GetAchievementReward = 7,

	CL_GetOnceAchievementInfo = 9,
	LC_GetOnceAchievementInfo = 10,

	LC_JoinAchievement = 11,
	LC_DelAchievement = 12,

	LC_AchievementList = 13,
};
struct CL_Cmd_GetRoleAchievementInfo : public NetCmd
{

};
struct LC_Cmd_GetRoleAchievementInfo : public NetCmd
{
	BYTE			States;
	WORD			Sum;
	tagRoleAchievementInfo Array[1];
};
struct LC_Cmd_GetRoleAchievementInfoFinish : public NetCmd
{

};
struct LC_Cmd_AchievementAllEventFinish : public NetCmd
{
	BYTE			AchievementID;
};
struct CL_Cmd_GetAchievementReward : public NetCmd
{
	BYTE			AchievementID;
};
struct LC_Cmd_GetAchievementReward : public NetCmd
{
	BYTE			AchievementID;
};
struct CL_Cmd_GetOnceAchievementInfo : public NetCmd
{
	BYTE	AchievementID;
};
struct LC_Cmd_GetOnceAchievementInfo : public NetCmd
{
	tagRoleAchievementInfo		AchievementInfo;
};
struct LC_Cmd_JoinAchievement : public NetCmd
{
	BYTE		AchievementID;
};
struct LC_Cmd_DelAchievement : public NetCmd
{
	BYTE		AchievementID;
};
struct LC_Cmd_AchievementList : public NetCmd
{
	BYTE		States;
	WORD		Sum;
	DWORD		Array[1];
};
//称号--------------------------------------------
enum TitleSub
{
	CL_LoadRoleTitle = 1,
	LC_LoadRoleTitle = 2,
	//LC_LoadRoleTitleFinish = 3,

	LC_AddRoleTitle = 4,
	LC_DelRoleTitle = 5,

	CL_ChangeRoleCurrTitle = 6,
};
struct CL_Cmd_LoadRoleTitle : public NetCmd
{

};
struct LC_Cmd_LoadRoleTitle : public NetCmd
{
	BYTE		States;
	WORD		Sum;
	BYTE		Array[1];
};
struct LC_Cmd_LoadRoleTitleFinish : public NetCmd
{

};
struct LC_Cmd_AddRoleTitle : public NetCmd
{
	BYTE		AddTitleID;
};
struct LC_Cmd_DelRoleTitle : public NetCmd
{
	BYTE		DelTitleID;
};
struct CL_Cmd_ChangeRoleCurrTitle : public NetCmd
{
	BYTE		TitleID;
};
//---------------------------Month-----------------
enum MonthSub
{
	//报名
	CL_SignUpMonth = 1,//玩家请求报名比赛
	GC_SignUpMonth = 2,//转发到中央服务器
	CG_SignUpMonth = 3,//中央服务器询问游戏服务前期
	GC_SignUpMonthResult = 4,//游戏服务器做物品删除的操作 并且成功结果返回中央服务器
	CG_SignUpMonthSucess = 5,
	LC_SignUpMonthSucess = 6,
	LC_SignUpMonthFiled = 7,
	//参加比赛
	CL_JoinMonth = 8,
	GC_JoinMonth = 9,
	CG_JoinMonth = 10,
	LC_JoinMonthSucess = 11,
	LC_JoinMonthFiled = 12,
	//玩家积分 比赛金币的变化
	/*GC_ChangeUserPoint = 13,
	GC_ChangeUserGlobel = 14,*/
	GC_ChangeUserMonthInfo = 13,

	CL_ChangeUserAddMonthGlobelNum = 15,//玩家申请续币
	GC_ChangeUserAddMonthGlobelNum = 16,//玩家进行续币操作
	LC_ChangeUserAddMonthGlobelNum = 17,//

	//玩家在Center上排名变化的时候 我们如何处理?
	CG_UserChangeIndex = 18,
	LC_UserChangeIndex = 19,

	CG_UserMonthEnd = 20,
	LC_UserMonthEnd = 21,

	//
	GC_LoadRoleSignUp = 30,
	CG_LoadRoleSignUp = 31,
	CL_LoadRoleSignUpInfo = 32,
	LC_LoadRoleSignUpInfo = 33,

	//CG_SendMonthRoleSumBegin = 34,//弃用
	CG_SendMonthRoleSum = 35,
	//CG_SendMonthRoleSumFinish = 36,//弃用
	//CL_GetMonthRoleSum = 37,
	LC_GetMonthRoleSum = 38,
	//LC_GetMonthRoleSumFinish = 39,

	CL_ResetMonthInfo = 40,
	GC_ResetMonthInfo = 41,
	CG_ResetMonthInfo = 42,
	LC_ResetMonthInfo = 43,

	LC_SetRoleMonthInfo = 44,

	LC_ResetRoleMonthInfo = 45,

	CG_GetUserMonthInfo = 46,
	GC_GetUserMonthInfoFinish = 47,

	/*GC_LogonToMonthTable = 50,
	CG_LogonToMonthTable = 51,*/

	CG_MonthList		= 48,
	LC_MonthList		= 49,

	CG_MonthStates      = 50,
};
struct CL_Cmd_SignUpMonth : public NetCmd
{
	BYTE		MonthID;
};
struct GC_Cmd_SignUpMonth : public NetCmd
{
	DWORD		dwUserID;
	BYTE		MonthID;
};
struct CG_Cmd_SignUpMonth : public NetCmd
{
	DWORD		dwUserID;
	BYTE		MonthID;
	bool		Result;
};
struct LC_Cmd_SignUpMonthFiled : public NetCmd
{
	BYTE		MonthID;
};
struct GC_Cmd_SignUpMonthResult : public NetCmd
{
	DWORD		dwUserID;
	BYTE		MonthID;
};
struct CG_Cmd_SignUpMonthSucess : public NetCmd
{
	DWORD		dwUserID;
	BYTE		MonthID;
};
struct LC_Cmd_SignUpMonthSucess : public NetCmd
{
	BYTE		MonthID;
};
struct CL_Cmd_JoinMonth : public NetCmd
{
	BYTE		MonthID;
};
struct GC_Cmd_JoinMonth : public NetCmd
{
	DWORD		dwUserID;
	//BYTE		TableTypeID;
	BYTE		MonthID;
	WORD		TableID;
};
struct CG_Cmd_JoinMonth : public NetCmd
{
	bool		Result;
	tagRoleMonthInfo MonthInfo;
	WORD		TableID;
};
struct LC_Cmd_JoinMonthSucess : public NetCmd
{
	tagRoleMonthInfo MonthInfo;
};
struct LC_Cmd_ChangeUserAddMonthGlobelNum : public NetCmd
{
	DWORD			dwUserID;
};
//struct LC_Cmd_JoinMonthFiled
//{
//};
//struct GC_Cmd_ChangeUserPoint : public NetCmd
//{
//	BYTE		MonthID;
//	DWORD		dwUserID;
//	DWORD		dwPoint;
//};
//struct GC_Cmd_ChangeUserGlobel : public NetCmd
//{
//	BYTE		MonthID;
//	DWORD		dwUserID;
//	DWORD		dwMonthGlobel;
//};
struct GC_Cmd_ChangeUserMonthInfo : public NetCmd
{
	bool		IsEnd;
	BYTE		MonthID;
	DWORD		dwUserID;
	DWORD		dwMonthGlobel;
	DWORD		dwPoint;
	DWORD		SkillSum;
};
struct CG_Cmd_GetUserMonthInfo : public NetCmd
{
	BYTE		MonthID;
};
struct GC_Cmd_GetUserMonthInfoFinish : public NetCmd
{
	BYTE		MonthID;
};
struct tagMonthRoleInfo
{
	DWORD		dwUserID;
	DWORD		Index;
	TCHAR		NickName[MAX_NICKNAME+1];
	DWORD		Score;
	DWORD		VipScore;
	DWORD		FaceID;
};
struct CG_Cmd_MonthList : public NetCmd
{
	BYTE					MonthID;
	//MAX_MonthList_Length
	tagMonthRoleInfo		MonthInfo[MAX_MonthList_Length];
};
struct LC_Cmd_MonthList : public NetCmd
{
	BYTE					MonthID;
	//MAX_MonthList_Length
	tagMonthRoleInfo		MonthInfo[MAX_MonthList_Length];
};

//struct CL_Cmd_ChangeUserAddMonthGlobelNum
//{
//	
//};
struct GC_Cmd_ChangeUserAddMonthGlobelNum : public NetCmd
{
	DWORD			dwUserID;
	BYTE			Month;
};
//struct LC_Cmd_ChangeUserAddMonthGlobelNum
//{
//
//};
struct CG_Cmd_UserChangeIndex : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwIndex;
	DWORD	dwUpperSocre;
};
struct LC_Cmd_UserChangeIndex : public NetCmd
{
	DWORD   dwUserID;
	DWORD	dwIndex;
	DWORD	dwUpperSocre;
};
struct CG_Cmd_UserMonthEnd : public NetCmd
{
	DWORD		dwUserID;
	BYTE		MonthID;
	DWORD		MonthIndex;
	DWORD		MonthScores;
	DWORD		VipScores;
};
struct LC_Cmd_UserMonthEnd : public NetCmd
{
	BYTE		MonthID;
	DWORD		MonthIndex;
	DWORD		MonthScores;
	DWORD		VipScores;
};
struct GC_Cmd_LoadRoleSignUp : public NetCmd
{
	DWORD dwUserID;
};
struct CG_Cmd_LoadRoleSignUp : public NetCmd
{
	DWORD dwUserID;
	BYTE   States;
	WORD   Sum;
	BYTE   Array[1];
};
//struct CL_Cmd_LoadRoleSignUpInfo
//{
//
//};
struct LC_Cmd_LoadRoleSignUpInfo : public NetCmd
{
	BYTE   States;
	WORD   Sum;
	BYTE   Array[1];
};
struct tagMonthRoleSum
{
	BYTE  MonthID;
	WORD  MonthUserSum;
};
struct CG_Cmd_SendMonthRoleSum : public NetCmd
{
	BYTE	States;
	WORD	Sum;
	tagMonthRoleSum Array[1];
};
struct LC_Cmd_GetMonthRoleSum : public NetCmd
{
	BYTE	States;
	WORD	Sum;
	tagMonthRoleSum Array[1];
};
struct CL_Cmd_ResetMonthInfo : public NetCmd
{
	BYTE	MonthID;
};
struct GC_Cmd_ResetMonthInfo : public NetCmd
{
	DWORD	dwUserID;
	BYTE	MonthID;
};
struct CG_Cmd_ResetMonthInfo : public NetCmd
{
	DWORD	dwUserID;
	BYTE	MonthID;
	bool	Result;
};
struct LC_Cmd_ResetMonthInfo : public NetCmd
{
	DWORD	dwUserID;
	BYTE	MonthID;
	bool	Result;
};
struct LC_Cmd_SetRoleMonthInfo : public NetCmd
{
	DWORD dwUserID;
	BYTE  MonthID;
	int MonthSocre;
	int MonthGlobel;
};
struct LC_Cmd_ResetRoleMonthInfo : public NetCmd
{
	tagRoleMonthInfo MonthInfo;
};
struct CG_Cmd_MonthStates : public NetCmd
{
	BYTE		MonthID;
	BYTE		MonthStates;//1 比赛开始  2比赛未开始  4报名开始 8 报名未开始
};
//struct GC_Cmd_LogonToMonthTable : public NetCmd
//{
//	DWORD			dwUserID;
//	BYTE			MonthID;
//};
//struct CG_Cmd_LogonToMonthTable : public NetCmd
//{
//	DWORD			dwUserID;
//	BYTE			MonthID;
//	bool			Result;
//	tagRoleMonthInfo MonthInfo;
//};
//排行榜 Rank --------------------------------------------
enum RankSub
{
	CL_GetRankReward = 1,
	/*LR_GetRankReward = 2,
	RL_GetRankReward = 3,*/
	LC_GetRankReward = 4,

	//获取排行榜领取奖励的姿态
	CL_GetRankInfo = 5,
	/*LR_GetRankInfo = 6,
	RL_GetWeekRankInfo = 7,
	RL_GetRankInfoFinish = 8,*/
	LC_GetWeekRankInfo = 9,
	//LC_GetRankInfoFinish = 10,

};
struct CL_Cmd_GetRankReward : public NetCmd
{
	BYTE		RankID;
};
//struct LR_Cmd_GetRankReward : public NetCmd
//{
//	DWORD		dwUserID;
//	BYTE		RankID;
//};
//struct RL_Cmd_GetRankReward : public NetCmd
//{
//	DWORD		dwUserID;
//	BYTE		RankID;
//	BYTE		Index;
//};
struct LC_Cmd_GetRankReward : public NetCmd
{
	BYTE		RankID;
};
//struct LR_Cmd_GetRankInfo : public NetCmd
//{
//	DWORD		dwUserID;
//};
//struct RL_Cmd_GetWeekRankInfo : public NetCmd
//{
//	//获取玩家的上周排名
//	DWORD		dwUserID;
//	BYTE		RankSum;
//	tagRankWeekReward RankArray[1];
//};
//struct RL_Cmd_GetRankInfoFinish : public NetCmd
//{
//	DWORD		dwUserID;
//};
struct LC_Cmd_GetWeekRankInfo : public NetCmd
{
	BYTE					States;
	WORD					Sum;
	tagRankWeekRankClient	Array[1];
};
//Chest ----------------------------------------------------------------------
enum ChestSub
{
	//LC_AddChest = 1,
	LC_ActionChest = 2,
	LC_ChestEnd = 3,

	CL_GetChestReward = 4,
	LC_GetChestReward = 5,

	CL_CloseChest = 6,

	LC_ResetChest = 7,
};
struct ChestOnceStates
{
	BYTE	Index;
	BYTE	RewardID;
	BYTE	RewardOnlyID;
};
struct LC_Cmd_ActionChest : public NetCmd
{
	bool			IsReset;
	BYTE			ChestOnlyID;
	BYTE			ChestTypeID;
	BYTE			ChestSum;
	BYTE			OpenSum;
	DWORD			EndSec;
	ChestOnceStates	ChestArray[1];
};
struct LC_Cmd_ChestEnd : public NetCmd
{
	BYTE			ChestOnlyID;
};
struct CL_Cmd_GetChestReward : public NetCmd
{
	BYTE			ChestOnlyID;
	BYTE			ChestIndex;//开启的位置索引 
};
struct LC_Cmd_GetChestReward : public NetCmd
{
	bool		Result;
	BYTE		ChestOnlyID;
	ChestOnceStates ChestStates;
};
struct CL_Cmd_CloseChest : public NetCmd
{
	BYTE			ChestOnlyID;
};
struct LC_Cmd_ResetChest : public NetCmd
{
};
//Charm
enum CharmSub
{
	CL_ChangeOtherUserCharm = 1,
	CC_ChangeOtherUserCharm = 2,
	CC_ChangeRoleCharmResult = 3,
	CC_ChangeUserCharm = 4,
	LC_ChangeRoleCharmResult = 5,

	LC_TableRoleCharmInfo = 6,
};
struct CL_Cmd_ChangeOtherUserCharm : public NetCmd
{
	DWORD		dwDestUserID;
	BYTE		CharmIndex;
	DWORD		ItemSum;
};
struct CC_Cmd_ChangeOtherUserCharm : public NetCmd
{
	DWORD		dwUserID;
	DWORD		dwDestUserID;
	BYTE		CharmIndex;
	DWORD		ItemSum;
};
struct CC_Cmd_ChangeRoleCharmResult : public NetCmd
{
	DWORD		dwUserID;
	BYTE		CharmIndex;
	DWORD		ItemSum;
	bool		Result;
	DWORD		dwDestUserID;
	int			dwDestUserCharmValue;
};
struct CC_Cmd_ChangeUserCharm : public NetCmd
{
	DWORD		dwUserID;
	BYTE		CharmIndex;
	DWORD		ItemSum;
};
struct LC_Cmd_ChangeRoleCharmResult : public NetCmd
{
	bool		Result;
	DWORD		dwDestUserID;
	int			dwDestUserCharmValue;
};
struct LC_Cmd_TableRoleCharmInfo : public NetCmd
{
	DWORD		SrcUserID;
	DWORD		DestUserID;
	BYTE		CharmIndex;
	DWORD		ItemSum;
};
//商店
enum ShopSub
{
	CL_ShopItem = 1,
	LC_ShopItemResult = 2,

	CG_RequiredOperatorShopList = 10,
	GC_RequiredOperatorShopList = 11,
};
struct CL_Cmd_ShopItem : public NetCmd
{
	BYTE		ShopID;
	//tagItemOnce pItem;
	BYTE		ShopItemIndex;
	DWORD		ItemSum;
};
struct LC_Cmd_ShopItemResult : public NetCmd
{
	bool		Result;
};
//Entity
enum EntitySub
{
	CL_LoadUserEntity = 1,
	LC_LoadUserEntity = 2,

	//CL_ChangeRoleName = 3,
	LC_ChangeRoleName = 4,

	//CL_ChangeRolePhone = 5,
	LC_ChangeRolePhone = 6,

	//CL_ChangeRoleEmail = 7,
	LC_ChangeRoleEmail = 8,

	/*CL_ChangeRoleAddress = 9,
	LC_ChangeRoleAddress = 10,*/

	//CL_ChangeRoleEntityID = 11,
	LC_ChangeRoleEntityID = 12,

	CL_ChangeRoleEntityItemUseName = 15,
	LC_ChangeRoleEntityItemUseName = 16,

	CL_ChagneRoleEntityItemUsePhone = 17,
	LC_ChangeRoleEntityItemUsePhone = 18,

	CL_ChangeRoleEntityItemUseAddress = 19,
	LC_ChangeRoleEntityItemUseAddress = 20,
};
struct LC_Cmd_LoadUserEntity : public NetCmd
{
	tagRoleAddressInfo		UserEntityInfo;
};
//struct CL_Cmd_ChangeRoleName : public NetCmd
//{
//	TCHAR			Name[MAX_NAME_LENTH + 1];//姓名
//};
struct LC_Cmd_ChangeRoleName : public NetCmd
{
	TCHAR			Name[MAX_NAME_LENTH + 1];//姓名
};
//
//struct CL_Cmd_ChangeRolePhone : public NetCmd
//{
//	TCHAR		Phone[MAX_PHONE_LENGTH + 1];
//};
struct LC_Cmd_ChangeRolePhone : public NetCmd
{
	unsigned __int64		Phone;
};

//struct CL_Cmd_ChangeRoleEmail : public NetCmd
//{
//	TCHAR			Email[MAX_EMAIL_LENGTH + 1];
//};
struct LC_Cmd_ChangeRoleEmail : public NetCmd
{
	TCHAR			Email[MAX_EMAIL_LENGTH + 1];
};

//struct CL_Cmd_ChangeRoleAddress : public NetCmd
//{
//	TCHAR			Addres[MAX_ADDRESS_LENGTH + 1];
//};
//struct LC_Cmd_ChangeRoleAddress : public NetCmd
//{
//	TCHAR			Addres[MAX_ADDRESS_LENGTH + 1];
//};

//struct CL_Cmd_ChangeRoleEntityID : public NetCmd
//{
//	TCHAR			IdentityID[MAX_IDENTITY_LENGTH + 1];//身份证号码
//};
struct LC_Cmd_ChangeRoleEntityID : public NetCmd
{
	TCHAR			IdentityID[MAX_IDENTITY_LENGTH + 1];//身份证号码
};

struct CL_Cmd_ChangeRoleEntityItemUseName : public NetCmd
{
	TCHAR			EntityItemUseName[MAX_NAME_LENTH + 1];
};
struct LC_Cmd_ChangeRoleEntityItemUseName: public NetCmd
{
	TCHAR			EntityItemUseName[MAX_NAME_LENTH + 1];
};
struct CL_Cmd_ChagneRoleEntityItemUsePhone : public NetCmd
{
	unsigned __int64		EntityItemUsePhone;
};
struct LC_Cmd_ChangeRoleEntityItemUsePhone : public NetCmd
{
	unsigned __int64		EntityItemUsePhone;
};
struct CL_Cmd_ChangeRoleEntityItemUseAddress : public NetCmd
{
	TCHAR			EntityItemUseAddres[MAX_ADDRESS_LENGTH + 1];
};
struct LC_Cmd_ChangeRoleEntityItemUseAddress : public NetCmd
{
	TCHAR			EntityItemUseAddres[MAX_ADDRESS_LENGTH + 1];
};
//活动

enum ActionSub
{
	CL_GetRoleActionInfo = 1,
	LC_GetRoleActionInfo = 2,
	//LC_GetRoleActionInfoFinish = 3,

	LC_ActionAllEventFinish = 5,

	CL_GetActionReward = 6,
	LC_GetActionReward = 7,

	CL_GetOnceActionInfo = 9,
	LC_GetOnceActionInfo = 10,

	LC_JoinAction = 11,
	LC_DelAction = 12,
};
struct CL_Cmd_GetRoleActionInfo : public NetCmd
{

};
struct LC_Cmd_GetRoleActionInfo : public NetCmd
{
	BYTE			States;
	WORD			Sum;
	tagRoleActionInfo Array[1];
};
//struct LC_Cmd_GetRoleActionInfoFinish : public NetCmd
//{
//
//};
struct LC_Cmd_ActionAllEventFinish : public NetCmd
{
	BYTE			ActionID;
};
struct CL_Cmd_GetActionReward : public NetCmd
{
	BYTE			ActionID;
	DWORD			OnceID;
};
struct LC_Cmd_GetActionReward : public NetCmd
{
	BYTE			ActionID;
};
struct CL_Cmd_GetOnceActionInfo : public NetCmd
{
	BYTE	ActionID;
};
struct LC_Cmd_GetOnceActionInfo : public NetCmd
{
	tagRoleActionInfo		ActionInfo;
};
struct LC_Cmd_DelAction : public NetCmd
{
	BYTE	ActionID;
};
struct LC_Cmd_JoinAction : public NetCmd
{
	BYTE	ActionID;
};
//Giff --------------------------------------------------------------------------
enum GiffSub
{
	CL_GetRoleGiff = 1,
	LC_GetRoleGiff = 2,
	//LC_GetRoleGiffFinish = 3,

	/*CL_AddRoleGiff = 4,
	LC_AddRoleGiffResult = 5,
	CC_AddRoleGiff = 6,
	CC_AddBeRoleGiff = 7,
	LC_AddBeRoleGiff = 8,*/

	CL_GetRoleGiffReward = 9,
	LC_GetRoleGiffReward = 10,

	CL_GetAllRoleGiffReward = 11,
	LC_GetAllRoleGiffReward = 12,

	CL_GetRoleNowDaySendGiff = 13,
	LC_GetRoleNowDaySendGiff = 14,
	//LC_GetRoleNowDaySendGiffFinish = 15,

	LC_DelRoleGiff			=16,

	CL_AddRoleGiff			= 17,
	GC_AddRoleGiff			= 18,
	CG_AddRoleGiff			= 19,
	GC_AddRoleGiffResult	= 20,
	CG_AddRoleGiffResult	= 21,
	LC_AddRoleGiffResult	= 22,
	LC_AddBeRoleGiff		= 23,
};
struct LC_Cmd_GetRoleGiff : public NetCmd
{
	BYTE			  States;
	WORD			  Sum;
	tagGiffClientInfo Array[1];
};

struct CL_Cmd_AddRoleGiff : public NetCmd
{
	DWORD		dwDestUserID;
};
struct GC_Cmd_AddRoleGiff : public NetCmd
{
	DWORD		dwSrcUserID;
	DWORD		dwDestUserID;
};
struct CG_Cmd_AddRoleGiff : public NetCmd
{
	DWORD		dwSrcUserID;
	DWORD		dwDestUserID;
};
struct GC_Cmd_AddRoleGiffResult : public NetCmd
{
	DWORD		dwSrcUserID;
	DWORD		dwDestUserID;
	bool		Result;
};
struct CG_Cmd_AddRoleGiffResult : public NetCmd
{
	DWORD		dwSrcUserID;
	DWORD		dwDestUserID;
	bool		Result;
};
struct LC_Cmd_AddRoleGiffResult : public NetCmd
{
	DWORD		dwDestUserID;
	bool		Result;
};
struct LC_Cmd_AddBeRoleGiff : public NetCmd
{
	tagGiffClientInfo GiffInfo;
};
//struct CL_Cmd_AddRoleGiff : public NetCmd
//{
//	DWORD		dwDestUserID;
//};
struct LC_Cmd_GetRoleNowDaySendGiff : public NetCmd
{
	BYTE				States;
	WORD				Sum;
	tagNowDaySendGiffInfo Array[1];
};
//struct LC_Cmd_AddRoleGiffResult : public NetCmd
//{
//	DWORD		dwDestUserID;
//	BYTE		Result;
//};
//struct CC_Cmd_AddRoleGiff : public NetCmd
//{
//	/*DWORD		dwSrcUserID;
//	DWORD		dwDestUserID;
//	DWORD		OnlyID;*/
//	DWORD		dwDestUserID;
//	tagGiffInfo GiffInfo;
//};
//struct CC_Cmd_AddBeRoleGiff : public NetCmd
//{
//	/*DWORD		dwSrcUserID;
//	DWORD		dwDestUserID;
//	DWORD		OnlyID;*/
//	DWORD		dwDestUserID;
//	tagGiffInfo GiffInfo;
//};
//struct LC_Cmd_AddBeRoleGiff : public NetCmd
//{
//	/*DWORD		dwSrcUserID;
//	DWORD		OnlyID;*/
//	tagGiffClientInfo GiffInfo;
//};
struct CL_Cmd_GetRoleGiffReward : public NetCmd
{
	DWORD		GiffID;
};
struct LC_Cmd_GetRoleGiffReward : public NetCmd
{
	DWORD		GiffID;
};
struct LC_Cmd_GetAllRoleGiffReward : public NetCmd
{
	BYTE		States;
	WORD		Sum;
	DWORD		Array[1];
};
struct LC_Cmd_DelRoleGiff : public NetCmd
{
	BYTE		States;
	WORD		Sum;
	DWORD		Array[1];
};

//GlobelShop
//enum GlobelShopSub
//{
//	CL_GlobelShopItem = 1,
//	LC_GlobelShopItem = 2,
//};
//struct CL_Cmd_GlobelShopItem : public NetCmd
//{
//	BYTE		ID;
//	WORD		Sum;
//};
//struct LC_Cmd_GlobelShopItem : public NetCmd
//{
//	BYTE		Result;
//};
//OnlineReward 
enum OnlineRewardSub
{
	CL_GetOnlineReward = 1,
	LC_GetOnlineReward = 2,
};
struct CL_Cmd_GetOnlineReward : public NetCmd
{
	BYTE		ID;
};
struct LC_Cmd_GetOnlineReward : public NetCmd
{
	BYTE		RewardID;
	DWORD		States;
	bool		Result;//是否成功
};

enum FtpShopSub
{
	CF_UpLoadFile = 1,
};
struct CF_Cmd_UpLoadFile : public NetCmd
{
	char			FileName[64+1];
	DWORD			FileSum;
	BYTE			States;
	WORD			Sum;
	BYTE			Array[1];
};

//Main_GameData
enum GameDataSub
{
	CL_GetGameData  = 1,
	LC_GendGameData = 2,
};
struct CL_Cmd_GetGameData : public NetCmd
{
};
struct LC_Cmd_GendGameData : public NetCmd
{
	tagRoleGameData GameData;
};


//Main_Package
enum PackageSub
{
	//CL_OpenPackage = 1,
	LC_OpenPackage = 2,
};
//struct CL_Cmd_OpenPackage : public NetCmd
//{
//	DWORD		PackageItemID;
//};
struct LC_Cmd_OpenPackage : public NetCmd
{
	bool		Result;
	BYTE		RewardSum;
	WORD		RewardID[1];
};

//Main_Server
enum ServerSub
{
	//服务器构架 之间相互注册的流程处理
	//Logon->Center
	LC_RsgLogon   = 1,//Logon上Center注册自己 表示自己可以登陆了 携带自己的ID Logon在配置文件FishServer中的唯一标记Logon的ID
	CL_RsgLogon	  = 2,//Center 将Logon需要的 当前在线玩家的列表返回给Logon 变长的 UserID:GameID
	CG_RsgLogon	  = 3,//告诉GameServer 一个新的Logon进来了
	CG_UnRsgLogon = 4,//告诉全部的GameServer 指定的Logon离开了


	GC_RsgGame	  = 5,//Game上Center 注册自己 并且携带当前已经登陆在GameServer上的全部玩家 进行上 中央服务器注册
	CG_RsgGame	  = 6,//Center将全部的Logon的列表发送给刚加入的Game 告诉他 去连接这些的Logon 
	CL_UnRsgGame  = 7,//GameServer离开Center了

	CL_RsgUser    = 8,
	CL_UnRsgUser  = 9,

	CL_GameInfo   = 10,

	//CC_ClientToServer = 10,

	//各个服务器之间的转发
	SS_LogonToGame = 20,//LogonToGameServer
	SS_GameToLogon = 21,//GameServerToLogon
	SS_GameToGame  = 22,//GameToGame
	SS_LogonToLogon= 23,//LogonToLogon
};
struct LC_Cmd_RsgLogon : public NetCmd
{
	BYTE		LogonConfigID;//当前Logon的配置ID
};
struct tagPlayerInfo
{
	DWORD		dwUserID;
	BYTE		GameConfigID;
};
struct CL_Cmd_RsgLogon : public NetCmd//数据会很长
{
	BYTE			States;
	WORD			Sum;//玩家的人数
	//玩家的列表
	tagPlayerInfo   Array[1];//玩家的列表
};
struct CG_Cmd_RsgLogon : public NetCmd
{
	BYTE		LogonConfigID;
};
struct CG_Cmd_UnRsgLogon : public NetCmd
{
	BYTE		LogonConfigID;
};
struct GC_Cmd_RsgGame : public NetCmd
{
	BYTE		GameConfigID;
};
struct CG_Cmd_RsgGame : public NetCmd
{
	BYTE		States;
	WORD        Sum;//Logon服务器的数量
	BYTE		Array[1];
};
struct CL_Cmd_UnRsgGame : public NetCmd
{
	BYTE		GameConfigID;
};
struct CL_Cmd_RsgUser : public NetCmd
{
	tagPlayerInfo	PlayerInfo;
};
struct CL_Cmd_UnRsgUser : public NetCmd
{
	DWORD			dwUserID;
};
struct CL_Cmd_GameInfo : public NetCmd
{
	BYTE		States;
	WORD        Sum;//Game服务器的数量
	BYTE		Array[1];
};
//struct CC_Cmd_ClientToServer : public NetCmd
//{
//	DWORD			Length;
//	BYTE			Data[1];//数据流 
//};
struct SS_Cmd_LogonToGame : public NetCmd
{
	BYTE		GameID;
	WORD		Length;
	BYTE		Array[1];
};
struct SS_Cmd_GameToLogon : public NetCmd
{
	BYTE		LogonID;
	WORD		Length;
	BYTE		Array[1];
};
struct SS_Cmd_GameToGame : public NetCmd
{
	BYTE		GameID;
	WORD		Length;
	BYTE		Array[1];
};
struct SS_Cmd_LogonToLogon : public NetCmd
{
	BYTE		LogonID;
	WORD		Length;
	BYTE		Array[1];
};

//Launcher Main_Launcher
enum LauncherSub
{
	LC_LauncherData = 1,
};
struct LC_Cmd_LauncherData : public NetCmd
{
	DWORD	LauncherData;//炮台数据
};

//Message Main_Message
enum MessageSub
{
	GC_SendMessage = 1,//GameServer->Center
	CG_SendMessage = 2,//Center->GameServer
	LC_SendMessage = 3,//GameServer->Client
	CL_SendMessage = 4,//Client->GameServer
};
enum MessageType
{
	MT_Table  =1,//当前桌子
	MT_TableMessage = 2,//当前桌子 弹框

	MT_Game = 3,//当前GameServer
	MT_GameMessage = 4,

	MT_Center = 5,//当前服务器
	MT_CenterMessage=6,

	MT_User = 7,//单个玩家
	MT_UserMessage = 8,
};
struct GC_Cmd_SendMessage : public NetCmd
{
	DWORD		Param;
	DWORD		MessageColor;
	BYTE		StepNum;
	BYTE		StepSec;
	BYTE		MessageType;//消息类型
	WORD		MessageSize;
	TCHAR		Message[1];//变长的命令
	//TCHAR		Message[MAX_MESSAGE_LENGTH+1];//消息
};
struct CG_Cmd_SendMessage : public NetCmd
{
	DWORD		Param;
	DWORD		MessageColor;
	BYTE		StepNum;
	BYTE		StepSec;
	BYTE		MessageType;//消息类型
	//TCHAR		Message[MAX_MESSAGE_LENGTH + 1];//消息
	WORD		MessageSize;
	TCHAR		Message[1];//变长的命令
};
struct LC_Cmd_SendMessage : public NetCmd
{
	DWORD		Param;
	DWORD		MessageColor;
	BYTE		StepNum;
	BYTE		StepSec;
	BYTE		MessageType;//消息类型
	//TCHAR		Message[MAX_MESSAGE_LENGTH + 1];//消息
	WORD		MessageSize;
	TCHAR		Message[1];//变长的命令
};
struct CL_Cmd_SendMessage : public NetCmd
{
	DWORD		Param;
	//客户端向服务器端发送消息
	DWORD		MessageColor;
	BYTE		StepNum;
	BYTE		StepSec;
	BYTE		MessageType;//消息类型
	//TCHAR		Message[MAX_MESSAGE_LENGTH + 1];//消息
	WORD		MessageSize;
	TCHAR		Message[1];//变长的命令
};
//Main_Recharge
enum RechargeSub
{
	CL_Recharge = 1,
	LC_Recharge = 2,

	LC_GetOrderID = 3,

	CL_IOSRecharge = 4,//IOS平台充值
	GO_IOSRecharge = 5,//IOS平台充值

	CG_CreateOrder = 10,
	GC_CreateOrder = 11
};
struct CL_Cmd_Recharge : public NetCmd
{
	DWORD	ID;
	bool    IsPC;
};
struct LC_Cmd_Recharge : public NetCmd
{
	DWORD	ID;
	bool	Result;
};
struct LC_Cmd_GetOrderID : public NetCmd
{
	DWORD	OrderID;
	DWORD	ShopID;
	bool	Result;
};
struct IOSRechargeInfo
{
	//char			OrderString
	BYTE			HandleSum;
	DWORD			Sum;
	BYTE			Array[1];
};
struct CL_Cmd_IOSRecharge : public NetCmd
{
	IOSRechargeInfo		OrderInfo;
};
struct GO_Cmd_IOSRecharge : public NetCmd
{
	DWORD				dwUserID;
	IOSRechargeInfo		OrderInfo;
};
//Main_Announcement
enum AnnouncementSub
{
	GC_GetAllAnnouncement = 1,
	CG_GetAllAnnouncement = 2,
	//CG_GetAllAnnouncementFinish = 3,
	GC_SendNewAnnouncementOnce = 4,
	CG_SendNewAnnouncementOnce = 5,
	CL_GetAllAnnouncement = 6,
	LC_GetAllAnnouncement = 7,
	//LC_GetAllAnnouncementFinish = 8,
	LC_SendNewAnnouncementOnce =9,
};


struct GC_Cmd_GetAllAnnouncement : public NetCmd
{

};
struct CG_Cmd_GetAllAnnouncement : public NetCmd
{
	BYTE		States;
	WORD		Sum;
	AnnouncementOnce Array[1];
};
//struct CG_Cmd_GetAllAnnouncementFinish : public NetCmd
//{
//
//};
struct GC_Cmd_SendNewAnnouncementOnce : public NetCmd
{
	AnnouncementOnce		pOnce;
};
struct CG_Cmd_SendNewAnnouncementOnce : public NetCmd
{
	AnnouncementOnce		pOnce;
};
struct CL_Cmd_GetAllAnnouncement : public NetCmd
{

};
struct LC_Cmd_GetAllAnnouncement : public NetCmd
{
	BYTE		States;
	WORD		Sum;
	AnnouncementOnce Array[1];
};
//struct LC_Cmd_GetAllAnnouncementFinish : public NetCmd
//{
//
//};
struct LC_Cmd_SendNewAnnouncementOnce : public NetCmd
{
	AnnouncementOnce		pOnce;
};
//运营 Main_Operate
enum OperateSub
{
	//1.实名验证  名称 和 身份证号码
	CL_RealNameVerification = 1,//将  名称  身份证号码 发送给 GameServer
	GO_RealNameVerification = 2,//将玩家ID 名称 身份证号码 发送给运营服务器
	OG_RealNameVerification = 3,//运营服务器将验证的结果返回给GameServer GameServer进行处理 存储数据库 
	LC_RealNameVerification = 4,//将验证的结果进行显示 携带ErrorID 

	//2.绑定手机
	CL_GetPhoneVerificationNum = 11,//发送给GameServer 携带手机号码 
	GO_GetPhoneVerificationNum = 12,//将玩家ID 手机号码 发送给运营服务器
	OG_GetPhoneVerificationNum = 13,//运营服务器将返回的结果 返回给GameServer
	LC_GetPhoneVerificationNum = 14,//通知客户端 发送手机验证码的结果 ErrorID 
	CL_BindPhone = 15,//将验证码 和 手机号码 发送到GameServer
	GO_BindPhone = 16,//将玩家ID 验证码 和 手机号码 发送到运营服务器
	OG_BindPhone = 17,//运营服务器将绑定操作的结果 返回GameServer
	LC_BindPhone = 18,//将绑定手机操作的结果返回客户端

	//3.BuyEntityItem
	GO_BuyEntityItem = 21,//向运营服务器 发送命令 玩家购买指定的实体物品
	OG_BuyEntityItem = 22,//运营服务器将购买结果返回GameServer
	LC_BuyEntityItem = 23,//将购买实体物品的结果返回给客户端 ErrorID

	//4.绑定邮箱
	CL_BindEmail = 31,//发送服务器端 绑定邮件
	GO_BindEmail = 32,//发送到运营服务器 绑定邮箱
	OC_BindEmail = 33,//运营服务器返回 绑定邮箱的结果
	CG_BindEmail = 34,//
	LC_BindEmail = 35,//告诉客户端 绑定邮箱的结果

	//5.扣除rmb
	//GO_UseRMB					= 36,
	OC_UseRMB = 36,
	CG_UseRMB = 37,

	//6.花费充值
	/*CL_PrepaidRecharge		    =40,
	GO_PrepaidRecharge			=41,
	OG_PrepaidRecharge			=42,
	LC_PrepaidRecharge			=43,*/
	GO_PhonePay = 40,
	OC_PhonePay = 41,
	CG_PhonePay = 42,
	LC_PhonePay = 43,

	//7.普通充值
	GO_AddNormalOrderID = 50,
	OC_AddNormalOrderID = 51,
	CG_AddNormalOrderID = 52,
	LC_AddNormalOrderID = 53,

	OC_Deal_Third_Platform_Verify = 60,
	CG_Deal_NotifyPay = 61,
	CG_Deal_NotifyClient = 62
};

struct CG_Cmd_Deal_NotifyClient : public NetCmd
{

};

struct CG_Cmd_Deal_NotifyPay : public NetCmd
{
	DWORD dwUserid;
	DWORD dwShopItem;
	//DWORD dwAddRechargeSum;
};

struct OC_Cmd_Third_Platform_Verify : public NetCmd
{
	tagDeal info;
};

struct CG_Cmd_RequiredOperatorShopList : public NetCmd
{

};


struct GC_Cmd_RequiredOperatorShopList : public NetCmd
{
	tagFishRechargeInfo config[MAX_REQUIRED_OPERATOR_SHOP_ITEM_COUT];
	int sum;
	BYTE States;
};


//实名验证
struct CL_Cmd_RealNameVerification : public NetCmd
{
	//玩家名称 和 身份证
	TCHAR			Name[MAX_NAME_LENTH + 1];
	TCHAR			IDCard[MAX_IDENTITY_LENGTH+1];
};
struct GO_Cmd_RealNameVerification : public NetCmd
{
	DWORD			dwUserID;
	TCHAR			Name[MAX_NAME_LENTH + 1];
	TCHAR			IDCard[MAX_IDENTITY_LENGTH + 1];
};
struct OG_Cmd_RealNameVerification : public NetCmd
{
	bool			Result;
	BYTE			ErrorID;//错误ID
	DWORD			dwUserID;
	TCHAR			Name[MAX_NAME_LENTH + 1];
	TCHAR			IDCard[MAX_IDENTITY_LENGTH + 1];
};
struct LC_Cmd_RealNameVerification : public NetCmd
{
	BYTE			ErrorID;//错误ID
	TCHAR			Name[MAX_NAME_LENTH + 1];
	TCHAR			IDCard[MAX_IDENTITY_LENGTH + 1];
};
//手机绑定
struct CL_Cmd_GetPhoneVerificationNum : public NetCmd
{
	//TCHAR			PhoneNumber[MAX_PHONE_LENGTH + 1];
	unsigned __int64  PhoneNumber;
};
struct GO_Cmd_GetPhoneVerificationNum : public NetCmd
{
	DWORD			dwUserID;
	unsigned __int64  PhoneNumber;
	//TCHAR			PhoneNumber[MAX_PHONE_LENGTH + 1];
};
struct OG_Cmd_GetPhoneVerificationNum : public NetCmd
{
	BYTE			ErrorID;
	DWORD			dwUserID;
	unsigned __int64  PhoneNumber;
	//TCHAR			PhoneNumber[MAX_PHONE_LENGTH + 1];
};
struct LC_Cmd_GetPhoneVerificationNum : public NetCmd
{
	BYTE			ErrorID;
	//TCHAR			PhoneNumber[MAX_PHONE_LENGTH + 1];
	unsigned __int64  PhoneNumber;
};
struct CL_Cmd_BindPhone : public NetCmd
{
	//TCHAR			PhoneNumber[MAX_PHONE_LENGTH + 1];
	//unsigned __int64  PhoneNumber;
	DWORD			BindNumber;
	DWORD			SecPasswordCrc1;
	DWORD			SecPasswordCrc2;
	DWORD			SecPasswordCrc3;
};
struct GO_Cmd_BindPhone : public NetCmd
{
	DWORD			dwUserID;
	//TCHAR			PhoneNumber[MAX_PHONE_LENGTH + 1];
	//unsigned __int64  PhoneNumber;
	DWORD			BindNumber;
	DWORD			SecPasswordCrc1;
	DWORD			SecPasswordCrc2;
	DWORD			SecPasswordCrc3;
};
struct OG_Cmd_BindPhone : public NetCmd
{
	bool			Result;
	BYTE			ErrorID;
	DWORD			dwUserID;
	//TCHAR			PhoneNumber[MAX_PHONE_LENGTH+1];
	unsigned __int64  PhoneNumber;
	DWORD			SecPasswordCrc1;
	DWORD			SecPasswordCrc2;
	DWORD			SecPasswordCrc3;
};
struct LC_Cmd_BindPhone : public NetCmd
{
	BYTE			ErrorID;
	bool			IsFirstBind;
	//TCHAR			PhoneNumber[MAX_PHONE_LENGTH+1];
	unsigned __int64  PhoneNumber;
};

struct CG_Cmd_CreateOrder : public NetCmd
{
	int ShopIndex;
	int Price;
};

struct GC_Cmd_CreateOrder : public NetCmd
{
	int OrderID;
	int ItemID;
	WCHAR ProductID[MAX_PayNOLength];
//	WCHAR sign_code[MAX_SIGN_CODE];
	WCHAR notify_url[MAX_UrlLength];
};

//3.实体物品购买 有充值类型的 或者是 邮递类型的 外部控制 
struct GO_Cmd_BuyEntityItem : public NetCmd
{
	//告诉运营服务器 玩家购买实体物品
	DWORD			dwUserID;
	//地址 手机 名称 
	TCHAR			Name[MAX_NAME_LENTH + 1];//姓名
	unsigned __int64	Phone;//手机号码
	TCHAR			Addres[MAX_ADDRESS_LENGTH + 1];//现实地址
	//物品数据
	BYTE			ShopID;
	BYTE			ShopOnlyID;
	DWORD			ItemID;
	DWORD			ItemSum;
};
struct OG_Cmd_BuyEntityItem : public NetCmd
{
	bool			Result;
	BYTE			ErrorID;
	DWORD			dwUserID;
	BYTE			ShopID;
	BYTE			ShopOnlyID;
	DWORD			ItemSum;
};
struct LC_Cmd_BuyEntityItem : public NetCmd
{
	BYTE			ErrorID;//提示玩家购买成功 还是 失败
};
//绑定邮箱
struct CL_Cmd_BindEmail : public NetCmd
{
	TCHAR			EMail[MAX_EMAIL_LENGTH + 1];
};
struct GO_Cmd_BindEmail : public NetCmd
{
	DWORD			dwUserID;
	TCHAR			EMail[MAX_EMAIL_LENGTH + 1];
};
struct OC_Cmd_BindEmail : public NetCmd
{
	bool			Result;
	BYTE			ErrorID;
	DWORD			dwUserID;
	TCHAR			EMail[MAX_EMAIL_LENGTH + 1];
};
struct CG_Cmd_BindEmail : public NetCmd
{
	bool			Result;
	BYTE			ErrorID;
	DWORD			dwUserID;
	TCHAR			EMail[MAX_EMAIL_LENGTH + 1];
};
struct LC_Cmd_BindEmail : public NetCmd
{
	BYTE			ErrorID;
	TCHAR			EMail[MAX_EMAIL_LENGTH + 1];
};
//
//struct GO_Cmd_UseRMB : public NetCmd
//{
//	DWORD			dwUserID;
//	DWORD			OnlyID;
//	float			LostRMB;
//	TCHAR			OnceOnlyID[RMB_OnceOnlyID_Length];//订单唯一编号
//};

struct OC_Cmd_UseRMB : public NetCmd
{
	RechageInfo		rechargeInfo;
};
struct CG_Cmd_UseRMB : public NetCmd
{
	RechageInfo		rechargeInfo;
};
//struct CL_Cmd_PrepaidRecharge : public NetCmd
//{
//	BYTE		ShopID;
//	BYTE		ShopOnlyID;
//	TCHAR		PhoneNumber[MAX_PHONE_LENGTH + 1];//手机号码
//};
//struct GO_Cmd_PrepaidRecharge : public NetCmd
//{
//	TCHAR		PhoneNumber[MAX_PHONE_LENGTH + 1];//手机号码
//	BYTE		ShopID;
//	BYTE		ShopOnlyID;
//	DWORD		RechargeMoney;
//};
//struct OG_Cmd_PrepaidRecharge : public NetCmd
//{
//	bool		Result;//成功或者失败了
//	BYTE		ErrorID;
//	TCHAR		PhoneNumber[MAX_PHONE_LENGTH + 1];//手机号码
//	BYTE		ShopID;
//	BYTE		ShopOnlyID;
//	DWORD		RechargeMoney;
//};
//struct LC_Cmd_PrepaidRecharge : public NetCmd
//{
//	bool		Result;//成功或者失败了
//	BYTE		ErrorID;
//};

struct GO_Cmd_PhonePay : public NetCmd
{
	DWORD		dwUserID;
	BYTE		ShopID;
	BYTE		ShopOnlyID;
	DWORD		ShopSum;
	unsigned __int64 Phone;
};
struct OC_Cmd_PhonePay : public NetCmd
{
	unsigned __int64 OrderID;
	DWORD		dwUserID;
	unsigned __int64 Phone;
	bool		Result;
	BYTE		ErrorID;
	BYTE		ShopID;
	BYTE		ShopOnlyID;
	DWORD		ShopSum;
};
struct CG_Cmd_PhonePay : public NetCmd
{
	unsigned __int64 OrderID;
	DWORD		dwUserID;
	unsigned __int64 Phone;
	bool		Result;
	BYTE		ErrorID;
	BYTE		ShopID;
	BYTE		ShopOnlyID;
	DWORD		ShopSum;
};
struct LC_Cmd_PhonePay : public NetCmd
{
	BYTE		ErrorID;
};

struct GO_Cmd_AddNormalOrderID : public NetCmd
{
	DWORD	OrderID;
	DWORD	ShopID;
	DWORD	dwUserID;
};
struct OC_Cmd_AddNormalOrderID : public NetCmd
{
	DWORD	dwUserID;
	bool	Result;
	char	Transid[33];//订单流水号
	char    Sign[256];
	DWORD	ShopID;
	DWORD	OrderID;
};
struct CG_Cmd_AddNormalOrderID : public NetCmd
{
	DWORD	dwUserID;
	bool	Result;
	char	Transid[33];//订单流水号
	char    Sign[256];
	DWORD	ShopID;
	DWORD	OrderID;
};
struct LC_Cmd_AddNormalOrderID : public NetCmd
{
	bool	Result;
	char	Transid[33];//订单流水号
	char    Sign[256];
	DWORD	ShopID;
	DWORD	OrderID;
};


//Main_RoleMessage
enum RoleMessageSub
{
	LC_RoleMessageChange  = 1,
};
struct LC_Cmd_RoleMessageChange : public NetCmd
{
	DWORD			RoleMessageData;
};

//Main_Exchange
enum ExChangeSub
{
	CL_RoleUseExChangeCode = 1,//使用兑换码
	LC_RoleUseExChangeCode = 2,
};
struct CL_Cmd_RoleUseExChangeCode : public NetCmd
{
	TCHAR			ExChangeCode[ExChangeCode_Length+1];
};
struct  LC_Cmd_RoleUseExChangeCode : public NetCmd
{
	bool			Result;//使用是否成功
	BYTE			ID;
};

//Main_Control
enum ControlSub
{
	CL_CheckClientInfo = 1,//外部登陆到Control
	LC_CheckClientInfo = 2,

	//CL_GetCenterPlayerSum = 3,//获取服务器端在线人数
	//LC_GetCenterPlayerSum = 4,

	CL_SendMsgToAllGame = 5,//发送全服公告

	//CL_ResetNoticeConfig= 6,

	CL_KickUserByID = 7,//剔除玩家
	LC_KickUserResult = 8,

	CL_ReloadConfig = 9,//重新加载配置文件

	CL_HandleEntityItem = 10,//实体物品发货
	LC_HandleEntityItem = 11,

	CL_ResetRolePassword = 12,//重置用户的密码
	CL_FreezeAccount = 13,//冻结玩家 

	CL_SendSystemEmail = 14,//发送系统邮件

	CL_ChangeaNickName = 15,//修改名称

	CL_QueryOnlineUserInfo = 16,
	LC_QueryOnlineUserInfo = 17,

	CL_ChangeParticularStates = 18,
	LC_ChangeParticularStates = 19,

	//控制服务器数据统计
	LC_CenterInfo = 20,
	LC_GameInfo = 21,
	LC_LogonInfo = 22,
	LC_DBInfo = 23,
	LC_RankInfo = 24,
	LC_FTPInfo = 25,
	LC_OperateInfo = 26,
	LC_MiNiGameInfo = 27,
	CL_QueryFishPool = 28,//查询库存
	LC_QueryFishPoolResult = 29,//查询库存结果
	CL_QueryBlackList = 30,//查询黑名单
	LC_QueryBlackListResult = 31,//查询黑名单结果
	CL_SetBlackList = 32,//设置黑名单
	LC_SetBlackListResult = 33,//设置黑名单结果
	CL_OxAdmin = 40,//牛牛控制
	LC_OxAdmin = 41,//牛牛控制
	LC_OxAdminQueryName = 42,//查询名字
	LC_OxAdminQueryAllJetton = 43,//查询所有
	LC_OxAdminQueryPlayerJetton = 44,//

	CL_AnimalAdmin = 45,////舞会控制
	LC_AnimalAdmin = 46,//
	LC_AnimalAdminQueryName = 47,//查询名字
	LC_AnimalAdminQueryAllJetton = 48,//查询所有
	LC_AnimalAdminQueryPlayerJetton = 49,//

	CL_CarAdmin = 50,////车控制
	LC_CarAdmin = 51,//
	LC_CarAdminQueryName = 52,//查询名字
	LC_CarAdminQueryAllJetton = 53,//查询所有
	LC_CarAdminQueryPlayerJetton = 54,//

	///new
	GM_CL_CHECK_PASSWORD_REQ = 100,
	CL_GM_CHECK_PASSWORD_ACK = 101,
	GM_CL_QUERY_USER_INFO = 102,
	CE_GM_QUERY_USER_ACK = 103,

	GM_CL_REWARD_CONFIGS_REQ = 104,
	CL_GM_REWARD_CONFIGS_ACK = 105,

	GM_CL_ITEM_CONFIGS_REQ = 106,
	CL_GM_ITEM_CONFIGS_ACK = 107,

	GM_ADD_NEW_OPERATOR_MAIL = 108,
	
	CENTRAL_GS_ADD_NEW_OPERATOR_MAIL = 109,
	GS_CENTRAL_ALL_OPERATOR_MAIL_REQ = 110,
	CENTRAL_GS_ALL_OPERATOR_MAIL_ACK = 111,

};
//牛牛,舞会,碰碰车通用
enum  CONTROL_COMMON_INNER//通用控制代码,,返回状态
{
	CS_BANKER_LOSE = 1,
	CS_BANKER_WIN = 2,
	CS_BET_AREA = 3,

	RQ_SET_WIN_AREA = 4,
	RQ_RESET_CONTROL = 5,
	RQ_PRINT_SYN = 6,
	RQ_QUERY_NAMES = 7,  //查id
	RQ_QUERY_PLAYERJETTON = 8,  //下注
	RQ_QUERY_ALLJETTON = 9, //总下注 

	ACK_SET_WIN_AREA = 10,
	ACK_PRINT_SYN = 11,
	ACK_RESET_CONTROL = 12,

	CR_ACCEPT = 13,			//接受
	CR_REFUSAL = 14,			//拒
};

///GMYOOL
struct GM_CL_ItemConfigsReq : public NetCmd
{

};

#define MAX_ITEMS 30

struct CL_GM_ItemConfigsACK : public NetCmd
{
	int			  ItemSum;
	bool		  End;
	tagItemConfig Items[MAX_ITEMS];
};

#define MAX_REWARDS 100
struct GM_Cl_RewardConfigReq : public NetCmd
{

};
#define MAX_GM_ITEMS 30
struct TAG_GM_Reward
{
	WORD		RewardID;
	tagItemOnce item[MAX_GM_ITEMS] ;
	int			ItemCount;
};

struct Cl_GM_RewardConfigACK : public NetCmd
{
	TAG_GM_Reward rewards[MAX_REWARDS];
	BYTE count;
	bool end;
};


#define MAXQUERYALLUSERINFO 40
struct CE_GM_QueryUserACK : public NetCmd
{
	CE_GM_QueryUserACK()
	{
		SubCmdType = CE_GM_QUERY_USER_ACK;
		CmdType = Main_Control;
	}
	tagRoleInfo Role[MAXQUERYALLUSERINFO];
	BYTE count;
	bool end;	
};

enum QueryUserType
{
	QueryUserType_All,
	QueryUserType_Online,
	QueryUserType_Offline,
	QueryUserType_ByNickName
};

struct GM_CL_QueryUserInfoReq : public NetCmd
{
	GM_CL_QueryUserInfoReq()
	{
		SubCmdType = GM_CL_QUERY_USER_INFO;
		CmdType = Main_Control;
	}

	TCHAR	NickName[MAX_NICKNAME + 1];
	BYTE   Type;
};

struct GM_CL_Cmd_CheckPassWordReq : public NetCmd
{
	GM_CL_Cmd_CheckPassWordReq()
	{
		SubCmdType = GM_CL_CHECK_PASSWORD_REQ;
		CmdType = Main_Control;
	}
	char			PassWord[255];
	char			Account[255];
};



struct CL_GM_Cmd_CheckPassWordACK : public NetCmd
{
	CL_GM_Cmd_CheckPassWordACK()
	{
		SubCmdType = CL_GM_CHECK_PASSWORD_ACK;
		CmdType = Main_Control;
	}
	bool ret;
};
///GMYOOL
struct CL_Cmd_CheckClientInfo : public NetCmd
{
	DWORD			RankValue;
	char			MachineCode[MAC_LENGTH];
};


struct LC_Cmd_CheckClientInfo : public NetCmd
{
	bool			Result;
};

//struct CL_Cmd_GetCenterPlayerSum : public NetCmd
//{
//
//};
//struct LC_Cmd_GetCenterPlayerSum : public NetCmd
//{
//	DWORD		PlayerSum;
//};
#define MAXALLMSG 512
struct CL_Cmd_SendMsgToAllGame : public NetCmd
{
	DWORD		Param;
	DWORD		MessageColor;
	BYTE		StepNum;
	BYTE		StepSec;
	WORD		MessageSize;
	TCHAR		CenterMessage[512];
};

//struct CL_Cmd_ResetNoticeConfig : public NetCmd
//{
//};


struct CL_Cmd_KickUserByID : public NetCmd
{
	DWORD			dwUserID;
	DWORD			ClientID;
	DWORD			FreezeMin;
};
struct LC_Cmd_KickUserResult : public NetCmd
{
	DWORD			dwUserID;
	DWORD			ClientID;
	BYTE			Result;
};
struct CL_Cmd_ReloadConfig : public NetCmd
{

};
struct CL_Cmd_ChangeParticularStates : public NetCmd
{
	DWORD		dwUserID;
	DWORD		ParticularStates;
	DWORD		ClientID;
};
struct LC_Cmd_ChangeParticularStates : public NetCmd
{
	DWORD		dwUserID;
	DWORD		ParticularStates;
	DWORD		ClientID;
	bool		Result;
};


struct CL_Cmd_HandleEntityItem : public NetCmd
{
	DWORD			ID;
	DWORD			ClientID;
	DWORD			ClientIP;
	char			OrderNumber[32];//快递订单号
};
struct LC_Cmd_HandleEntityItem : public NetCmd
{
	DWORD			ClientID;
	bool			Result;
};

struct CL_Cmd_ResetRolePassword : public NetCmd
{
	//重置用户的密码
	DWORD			dwUserID;
	DWORD			Password1;
	DWORD			Password2;
	DWORD			Password3;
};
struct CL_Cmd_FreezeAccount : public NetCmd
{
	DWORD			dwUserID;
	DWORD			FreezeMin;//冻结的秒数 
};

struct CL_Cmd_SendSystemEmail : public NetCmd
{
	DWORD		dwUserID;
	DWORD		RewardID;
	DWORD		RewardSum;
	WORD		ContextSize;
	TCHAR		EmailContext[MAX_MAIL_CONTEXT + 1];
};
struct CL_Cmd_ChangeaNickName : public NetCmd
{
	DWORD		dwUserID;
	TCHAR		NickName[MAX_NICKNAME + 1];
};
struct CL_Cmd_QueryOnlineUserInfo : public NetCmd
{
	DWORD		dwUserID;
	DWORD		ClientID;
};
struct LC_Cmd_QueryOnlineUserInfo : public NetCmd
{
	DWORD		ClientID;
	bool		Result;
	tagRoleInfo RoleInfo;
};

struct LC_Cmd_CenterInfo : public NetCmd
{
	//
	BYTE		CenterID;
	DWORD		AllGamePlayerSum;//总在线人数
	DWORD		GameServerSum;
	DWORD		LogonServerSum;
};
struct LC_Cmd_GameInfo : public NetCmd
{
	BYTE		GameID;
	DWORD		OnlinePlayerSum;
	DWORD		TableSum;
	DWORD		WriteLogonPlayerSum;
};
struct LC_Cmd_LogonInfo : public NetCmd
{
	BYTE		LogonID;
	DWORD		LogonTablePlayerSum;
};
struct LC_Cmd_DBInfo : public NetCmd
{
	BYTE		DBID;
};
struct LC_Cmd_RankInfo : public NetCmd
{
	DWORD		RankID;
};
struct LC_Cmd_FTPInfo : public NetCmd
{
	DWORD		FTPID;
};
struct LC_Cmd_OperateInfo : public NetCmd
{
	DWORD		OperateID;
};
struct LC_Cmd_MiNiGameInfo :public NetCmd
{
	BYTE		MiniGameID;

	//NiuNiu的数据
	INT64		NowNiuNiuSystemGlobel;//当前牛牛的金币
};
//查询库存
struct CL_CMD_QueryFishPool :public NetCmd
{
	DWORD ClientID;
};

struct TableTypePool
{
	bool bopen;
	int64 npool;
};

struct LC_CMD_QueryFishPoolResult :public NetCmd
{
	DWORD ClientID;
	BYTE byServerid;	
	TableTypePool  table[1];
};
//查询黑名单
struct LC_CMD_QueryFishBlackList :public NetCmd
{
	DWORD ClientID;
};

struct LC_CMD_QueryFishBlackListResult :public NetCmd
{
	DWORD ClientID;
	BYTE byServerid;	
	DWORD dwUserID[1];
};
//设置黑名单
struct LC_CMD_SetFishBlackList :public NetCmd
{
	DWORD ClientID;
	DWORD dwUserID[1];
};
struct LC_CMD_SetFishBlackListResult :public NetCmd
{
	DWORD ClientID;
	BYTE byServerid;
	BYTE byCount;
};
//ox
struct tagOxAdminReq
{
	BYTE							m_cbExcuteTimes;					//执行次数
	BYTE							m_cbControlStyle;					//控制方式
	bool							m_bWinArea[MAX_NIUNIU_ClientSum];						//赢家区域
};
struct CL_CMD_OxAdminReq :public NetCmd
{
	DWORD ClientID;
	BYTE cbReqType;
	BYTE cbExtendData[20];			//附加数据
};

struct LC_CMD_OxAdminReq :public NetCmd
{
	DWORD ClientID;
	BYTE cbAckType;					//回复类型
	BYTE cbResult;
	BYTE cbExtendData[20];			//附加数据
};
struct LC_CMD_OxAdmin_AllJetton :public NetCmd
{
	DWORD ClientID;
	UINT64 nStock;
	UINT64 nJetton[MAX_NIUNIU_ClientSum];
};
struct LC_CMD_OxAdmin_PlayerJetton :public NetCmd
{
	DWORD ClientID;
	TCHAR name[MAX_NICKNAME+1];
	DWORD dwJetton[MAX_NIUNIU_ClientSum];
};

struct LC_CMD_OxAdmin_PlayerNames :public NetCmd
{
	DWORD ClientID;
	TCHAR names[1];
};

//舞会
struct tagAnimalAdminReq
{
	BYTE							m_cbExcuteTimes;					//执行次数
	BYTE							m_cbControlStyle;					//控制方式
	BYTE							m_cbarea;
};
struct CL_CMD_AnimalAdminReq :public NetCmd
{
	DWORD ClientID;
	BYTE cbReqType;
	BYTE cbExtendData[20];			//附加数据
};

struct LC_CMD_AnimalAdminReq :public NetCmd
{
	DWORD ClientID;
	BYTE cbAckType;					//回复类型
	BYTE cbResult;
	BYTE cbExtendData[20];			//附加数据
};

struct LC_CMD_AnimalAdmin_AllJetton :public NetCmd
{
	DWORD ClientID;
	UINT64 nStock;
	UINT64 nJetton[MAX_DIAL_ClientSum];
};
struct LC_CMD_AnimalAdmin_PlayerJetton :public NetCmd
{
	DWORD ClientID;
	TCHAR name[MAX_NICKNAME + 1];
	DWORD dwJetton[MAX_DIAL_ClientSum];
};

struct LC_CMD_AnimalAdmin_PlayerNames :public NetCmd
{
	DWORD ClientID;
	TCHAR names[1];
};

//车
struct tagCarAdminReq
{
	BYTE							m_cbExcuteTimes;					//执行次数
	BYTE							m_cbControlStyle;					//控制方式
	BYTE							m_cbarea;
};
struct CL_CMD_CarAdminReq :public NetCmd
{
	DWORD ClientID;
	BYTE cbReqType;
	BYTE cbExtendData[20];			//附加数据
};

struct LC_CMD_CarAdminReq :public NetCmd
{
	DWORD ClientID;
	BYTE cbAckType;					//回复类型
	BYTE cbResult;
	BYTE cbExtendData[20];			//附加数据
};

struct LC_CMD_CarAdmin_AllJetton :public NetCmd
{
	DWORD ClientID;
	UINT64 nStock;
	UINT64 nJetton[MAX_CAR_ClientSum];
};
struct LC_CMD_CarAdmin_PlayerJetton :public NetCmd
{
	DWORD ClientID;
	TCHAR name[MAX_NICKNAME + 1];
	DWORD dwJetton[MAX_CAR_ClientSum];
};

struct LC_CMD_CarAdmin_PlayerNames :public NetCmd
{
	DWORD ClientID;
	TCHAR names[1];
};


//Main_Lottery
enum LotterySub
{
	CL_GetLotteryReward = 1,
	LC_GetLotteryReward = 2,

	CL_LotteryUIStates   = 3,
	LC_LotteryUIStates   = 4,
};
struct CL_Cmd_GetLotteryReward : public NetCmd
{
	BYTE		LotteryID;
};
struct LC_Cmd_GetLotteryReward : public NetCmd
{
	BYTE		LotteryID;
	WORD		RewardID;
	bool		Result;
};
struct CL_Cmd_LotteryUIStates : public NetCmd
{
	DWORD		dwUserID;
	bool		IsOpen;
};
struct LC_Cmd_LotteryUIStates : public NetCmd
{
	DWORD		dwUserID;
	bool		IsOpen;
};
enum ExtraDownCmd
{
	EXTRA_DOWN_CHECK_VER = 1,
	EXTRA_DOWN_FILE		 = 2,
	EXTRA_DOWN_FILE_EX	 = 3,
};

//Main_MiniGame
//牛牛的一些基本结构

enum MiniGameSub
{
	////小游戏的处理
	////牛牛
	//CL_JoinRoom = 1,//进入牛牛房间
	//GM_JoinRoom = 2,
	//MG_JoinRoom = 3,//将桌子上的最新的数据发送给玩家
	//LC_JoinRoom = 4,

	//MG_TableUpdate = 5,//在下注阶段 不停的发送最新的奖池数据到客户端去
	//LC_TableUpdate = 6,

	//CL_AddGlobel = 7,//下注
	//GM_AddGlobel = 8,
	//MG_AddGlobel = 9,
	//LC_AddGlobel = 10,//下注的结果

	//MG_TableResult = 11,//结果
	//LC_TableResult = 12,

	//MG_TableBegin = 13,//桌子重新进入下注阶段
	//LC_TableBegin = 14,

	//CL_LeaveNiuNiuTable = 15,
	//GM_LeaveNiuNiuTable = 16,

	//MG_BankerChange = 17,
	//LC_BankerChange = 18,

	///*MG_WirteBankerListChange = 19,
	//LC_WirteBankerListChange = 20,*/

	//CL_AddBankerList = 21, //上庄
	//GM_AddBankerList = 22, 
	//MG_AddBankerList = 23,
	//LC_AddBankerList = 24,

	//CL_NextBankerSeat = 25,//抢庄
	//GM_NextBankerSeat = 26,
	//MG_NextBankerSeat = 27,
	//LC_NextBankerSeat = 28,

	//CL_GetBankerList  = 29,//获取排队列表
	//GM_GetBankerList  = 30,
	//MG_GetBankerList  = 31,
	//LC_GetBankerList  = 32,

	//CL_LeaveBankerList = 33,//离开排队列表
	//GM_LeaveBankerList = 34,
	//MG_LeaveBankerList = 35,
	//LC_LeaveBankerList = 36,

	//CL_CanelBankerSeat = 37,
	//GM_CanelBankerSeat = 38,
	//MG_CanelBankerSeat = 39,
	//LC_CanelBankerSeat = 40,


	//MiniGame服务器基本控制
	//CL_RoleJoinMiniGame    = 30,
	GM_RoleJoinMiniGame    = 31,
	//CL_RoleLeaveMiniGame   = 32,
	GM_RoleLeaveMiniGame   = 33,
	GM_RoleChangeNickName  = 34,
	GM_RoleChangeFaceID    = 35,
	GM_RoleChangeGlobel    = 36,
	GM_RoleChangeMadel     = 37,
	GM_RoleChangeCurrcey   = 38,
	GM_RoleChangeVipLevel  = 39,
	GM_ChangeRoleParticularStates = 40,
};
////玩家进入桌子
//struct CL_Cmd_JoinRoom : public NetCmd
//{
//	//玩家进入桌子 牛牛
//};
//struct GM_Cmd_JoinRoom : public NetCmd
//{
//	DWORD				dwUserID;
//};
//struct MG_Cmd_JoinRoom : public NetCmd
//{
//	DWORD				dwUserID;
//	BYTE				TableStates;
//	BYTE				TableUpdateSec;//桌子上剩余的秒数
//	//将桌子上的数据发送到客户端去
//	TableUpdateInfo		TableInfo;//桌子上奖池的数据
//	TableBrandResult	TableBrand;//桌子上牌的数据
//	BankerInfo			TableBanker;//庄家数据
//	BYTE				BankerGameSum;//庄家进行游戏的次数
//	WriteBankerInfo		ListInfo;//排队的信息
//};
//struct LC_Cmd_JoinRoom : public NetCmd
//{
//	BYTE				TableUpdateSec;//桌子上剩余的秒数
//	BYTE				TableStates;//桌子状态
//	TableUpdateInfo		TableInfo;//桌子上奖池的数据
//	TableBrandResult	TableBrand;//桌子上牌的数据
//	BankerInfo			TableBanker;//庄家数据
//	BYTE				BankerGameSum;//庄家进行游戏的次数
//	WriteBankerInfo		ListInfo;//排队的信息
//};
////桌子刷新
//struct MG_Cmd_TableUpdate : public NetCmd
//{
//	DWORD				dwUserID;
//	TableUpdateInfo		TableInfo;
//};
//struct LC_Cmd_TableUpdate : public NetCmd
//{
//	TableUpdateInfo		TableInfo;
//};
////玩家下注
//struct CL_Cmd_AddGlobel : public NetCmd
//{
//	//玩家下注
//	BYTE				Index;//下注区
//	DWORD				GlobelSum;
//};
//struct GM_Cmd_AddGlobel : public NetCmd
//{
//	DWORD				dwUserID;
//	BYTE				Index;//下注区
//	DWORD				GlobelSum;
//	DWORD				RoleGlobelSum;
//};
//struct MG_Cmd_AddGlobel : public NetCmd
//{
//	DWORD				dwUserID;
//	BYTE				Index;//下注区
//	DWORD				GlobelSum;
//	bool				Result;
//};
//struct LC_Cmd_AddGlobel : public NetCmd
//{
//	BYTE				Index;//下注区
//	DWORD				GlobelSum;//金币数量
//	bool				Result;//结果
//};
////桌子开牌阶段
//struct MG_Cmd_TableResult : public NetCmd
//{
//	DWORD				dwUserID;
//	int					AddGlobel;
//	TableBrandResult	TableBrand;//桌子上牌的数据
//	TableUpdateInfo		TableInfo;
//	DWORD				BankerUserID;
//	DWORD				BankerGlobelSum;
//};
//struct LC_Cmd_TableResult : public NetCmd
//{
//	int					AddGlobel;
//	TableBrandResult	TableBrand;//桌子上牌的数据
//	TableUpdateInfo		TableInfo;
//	DWORD				BankerUserID;
//	DWORD				BankerGlobelSum;
//};
////桌子重新进入下注阶段
//struct MG_Cmd_TableBegin : public NetCmd
//{
//	DWORD				dwUserID;				
//};
//struct LC_Cmd_TableBegin : public NetCmd
//{
//
//};
////离开
//struct CL_Cmd_LeaveNiuNiuTable : public NetCmd
//{
//
//};
//struct GM_Cmd_LeaveNiuNiuTable : public NetCmd
//{
//	DWORD				dwUserID;
//};
////庄家变化的时候
//struct MG_Cmd_BankerChange : public NetCmd
//{
//	//当庄家发生变化的时候
//	DWORD				dwUserID;
//	BankerInfo			TableBanker;//庄家数据
//};
//struct LC_Cmd_BankerChange : public NetCmd
//{
//	BankerInfo			TableBanker;//庄家数据
//};
//
//struct MG_Cmd_WirteBankerListChange : public NetCmd
//{
//	DWORD				dwUserID;
//};
//struct LC_Cmd_WirteBankerListChange : public NetCmd
//{
//
//};
//
//struct CL_Cmd_AddBankerList : public NetCmd
//{
//
//};
//struct GM_Cmd_AddBankerList : public NetCmd
//{
//	DWORD		dwUserID;
//};
//struct MG_Cmd_AddBankerList : public NetCmd
//{
//	DWORD		dwUserID;
//	bool		Result;
//	DWORD		SeatIndex;//位置
//};
//struct LC_Cmd_AddBankerList : public NetCmd
//{
//	bool		Result;
//	DWORD		SeatIndex;//位置
//};
//
//struct CL_Cmd_NextBankerSeat : public NetCmd
//{
//
//};
//struct GM_Cmd_NextBankerSeat : public NetCmd
//{
//	DWORD		dwUserID;
//};
//struct MG_Cmd_NextBankerSeat : public NetCmd
//{
//	DWORD		dwUserID;
//	bool		Result;
//};
//struct LC_Cmd_NextBankerSeat : public NetCmd
//{
//	bool		Result;
//};
//
//struct CL_Cmd_GetBankerList : public NetCmd
//{
//
//};
//struct GM_Cmd_GetBankerList : public NetCmd
//{
//	DWORD		dwUserID;
//};
//struct MG_Cmd_GetBankerList : public NetCmd
//{
//	DWORD		dwUserID;
//	DWORD		SeatIndex;
//	WriteBankerInfo		ListInfo;//排队的信息
//};
//struct LC_Cmd_GetBankerList : public NetCmd
//{
//	DWORD		SeatIndex;
//	WriteBankerInfo		ListInfo;//排队的信息
//};
//struct CL_Cmd_LeaveBankerList : public NetCmd
//{
//
//};
//struct GM_Cmd_LeaveBankerList : public NetCmd
//{
//	DWORD		dwUserID;
//};
//struct MG_Cmd_LeaveBankerList : public NetCmd
//{
//	DWORD		dwUserID;
//	bool		Result;
//};
//struct LC_Cmd_LeaveBankerList : public NetCmd
//{
//	bool		Result;
//};
//struct CL_Cmd_CanelBankerSeat : public NetCmd
//{
//
//};
//struct GM_Cmd_CanelBankerSeat : public NetCmd
//{
//	DWORD		dwUserID;
//};
//struct MG_Cmd_CanelBankerSeat : public NetCmd
//{
//	DWORD		dwUserID;
//	bool		Result;
//};
//struct LC_Cmd_CanelBankerSeat : public NetCmd
//{
//	bool		Result;
//};
//基本控制流程
//struct CL_Cmd_RoleJoinMiniGame : public NetCmd
//{
//	
//};
struct GM_AddOperatorSystemMail : public NetCmd
{	
	tagOperatorSystemMail mail;
};

struct GM_Cmd_RoleJoinMiniGame : public NetCmd
{
	tagMiniGameRoleInfo   RoleInfo;
};
//struct CL_Cmd_RoleLeaveMiniGame : public NetCmd
//{
//
//};
struct GM_Cmd_RoleLeaveMiniGame : public NetCmd
{
	DWORD					dwUserID;
};
struct GM_Cmd_RoleChangeNickName : public NetCmd
{
	DWORD					dwUserID;
	TCHAR					NickName[MAX_NICKNAME + 1];
};
struct GM_Cmd_RoleChangeFaceID : public NetCmd
{
	DWORD					dwUserID;
	DWORD					FaceID;
};
struct GM_Cmd_RoleChangeGlobel : public NetCmd
{
	DWORD					dwUserID;
	DWORD					dwGlobel;
};
struct GM_Cmd_RoleChangeMadel : public NetCmd
{
	DWORD					dwUserID;
	DWORD					dwMadel;
};
struct GM_Cmd_RoleChangeCurrcey : public NetCmd
{
	DWORD					dwUserID;
	DWORD					dwCurrcey;
};
struct GM_Cmd_RoleChangeVipLevel : public NetCmd
{
	DWORD					dwUserID;
	BYTE					VipLevel;
};
struct GM_Cmd_ChangeRoleParticularStates : public NetCmd
{
	DWORD					dwUserID;
	DWORD					ParticularStates;
};
//Main_Dial
enum DialSub
{
	//玩家进入牛牛的桌子
	CL_DialRoleJoinTable = 1,
	GM_DialRoleJoinTable = 2,
	MG_DialRoleJoinTable = 3,
	LC_DialRoleJoinTable = 4,

	//玩家离开牛牛桌子
	CL_DialRoleLeaveTable = 5,
	GM_DialRoleLeaveTable = 6,

	//玩家下注
	CL_DialRoleBetGlobel = 7,
	GM_DialRoleBetGlobel = 8,
	MG_DialRoleBetGlobel = 9,
	LC_DialRoleBetGlobel = 10,

	//玩家获取庄家排队列表
	CL_DialGetBankerList = 11,
	GM_DialGetBankerList = 12,
	MG_DialGetBankerList = 13,
	LC_DialGetBankerList = 14,

	//庄家队列变化的时候
	MG_DialBankerListChange = 15,
	LC_DialBankerListChange = 16,

	//庄家发生变化的时候
	MG_DialBankerUserChange = 17,
	LC_DialBankerUserChange = 18,

	//桌子进入下注阶段
	MG_DialTableJoinBegin = 19,
	LC_DialTableJoinBegin = 20,

	//桌子在下注阶段的更新命令
	MG_DialTableUpdate = 21,
	LC_DialTableUpdate = 22,

	//桌子进入开牌阶段
	MG_DialTableJoinEnd = 23,
	LC_DialTableJoinEnd = 24,

	//玩家上庄
	CL_DialRoleJoinBankerList = 25,
	GM_DialRoleJoinBankerList = 26,
	MG_DialRoleJoinBankerList = 27,
	LC_DialRoleJoinBankerList = 28,

	//玩家抢庄
	CL_DialRoleGetBankerFirstSeat = 29,
	GM_DialRoleGetBankerFirstSeat = 30,
	MG_DialRoleGetBankerFirstSeat = 31,
	LC_DialRoleGetBankerFirstSeat = 32,

	//玩家离开队列
	CL_DialRoleLeaveBankerList = 33,
	GM_DialRoleLeaveBankerList = 34,
	MG_DialRoleLeaveBankerList = 35,
	LC_DialRoleLeaveBankerList = 36,

	//玩家取消做庄
	CL_DialRoleCanelBankerSeat = 37,
	GM_DialRoleCanelBankerSeat = 38,
	MG_DialRoleCanelBankerSeat = 39,
	LC_DialRoleCanelBankerSeat = 40,

	//庄家金币变化的时候
	MG_DialBankerUserGlobelChange = 41,
	LC_DialBankerUserGlobelChange = 42,

	//
	MG_DialWriteBankerUserGlobelMsg = 43,
	LC_DialWriteBankerUserGlobelMsg = 44,

	//玩家进入贵宾席
	CL_DialRoleJoinVipSeat = 45,
	GM_DialRoleJoinVipSeat = 46,
	MG_DialRoleJoinVipSeat = 47,
	LC_DialRoleJoinVipSeat = 48,

	CL_DialRoleLeaveVipSeat = 49,
	GM_DialRoleLeaveVipSeat = 50,
	MG_DialRoleLeaveVipSeat = 51,
	LC_DialRoleLeaveVipSeat = 52,

	MG_DialRoleBeLeaveVipSeat = 53,
	LC_DialRoleBeLeaveVipSeat = 54,

	//大众席数据
	CL_DialGetNormalSeatInfo = 55,
	GM_DialGetNormalSeatInfo = 56,
	MG_DialGetNormalSeatInfo = 57,
	LC_DialGetNormalSeatInfo = 58,

	//
	MG_DialVipSeatGlobelChange = 59,
	LC_DialVipSeatGlobelChange = 60,

	MG_DialTableStopUpdate = 61,
	LC_DialTableStopUpdate = 62,

	MG_DialVipGlobelChange = 63,
	LC_DialVipGlobelChange = 64,

	//继续下注
	CL_DialRoleBetGlobelByLog = 65,
	GM_DialRoleBetGlobelByLog = 66,
	MG_DialRoleBetGlobelByLog = 67,
	LC_DialRoleBetGlobelByLog = 68,
};
struct DialRoleInfo
{
	DWORD	dwUserID;
	DWORD	GlobelSum;
	DWORD	dwFaceID;
	TCHAR   NickName[MAX_NICKNAME + 1];
};
struct CL_Cmd_DialRoleJoinTable : public NetCmd
{

};
struct GM_Cmd_DialRoleJoinTable : public NetCmd
{
	DWORD			dwUserID;
};
struct MG_Cmd_DialRoleJoinTable : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
	//具体数据 桌子上的数据
	BYTE			TableStates;//桌子状态
	BYTE			TableStatesUpdateSec;//桌子当前状态的秒数
	UINT64			TableBetAreaGlobel[MAX_DIAL_ClientSum];//下注区金币数量
	BYTE			TableAreaData[MAX_DIAL_GameSum];//轮盘数据
	BYTE			TableAreaDataRate[MAX_DIAL_ClientSum];//轮盘倍率
	DialRoleInfo	TableBankerInfo;
	BYTE			TableBankerUseGameSum;//当前庄家已经进行游戏的局数
	DialRoleInfo	TableWriteBankerList[MAX_DIALSHOWBRAND_Sum];
	DialRoleInfo	VipSeatList[MAX_DIALVIPSEAT_Sum];//贵宾席的玩家
	BYTE			TableGameSum;//记录游戏局数
	DWORD			TableResultLog;//游戏结果记录
	WORD			TableResultIndex;
};
struct LC_Cmd_DialRoleJoinTable : public NetCmd
{
	bool			Result;
	//具体数据 桌子上的数据
	BYTE			TableStates;//桌子状态
	BYTE			TableStatesUpdateSec;//桌子当前状态的秒数
	UINT64			TableBetAreaGlobel[MAX_DIAL_ClientSum];//下注区金币数量
	BYTE			TableAreaData[MAX_DIAL_GameSum];//轮盘数据
	BYTE			TableAreaDataRate[MAX_DIAL_ClientSum];//轮盘倍率
	DialRoleInfo	TableBankerInfo;
	BYTE			TableBankerUseGameSum;//当前庄家已经进行游戏的局数
	DialRoleInfo	TableWriteBankerList[MAX_DIALSHOWBRAND_Sum];
	DialRoleInfo	VipSeatList[MAX_DIALVIPSEAT_Sum];//贵宾席的玩家
	BYTE			TableGameSum;//记录游戏局数
	DWORD			TableResultLog;//游戏结果记录
	WORD			TableResultIndex;
};

struct CL_Cmd_DialRoleBetGlobel : public NetCmd
{
	BYTE			Index;
	DWORD			AddGlobel;
};
struct GM_Cmd_DialRoleBetGlobel : public NetCmd
{
	DWORD			dwUserID;
	BYTE			Index;
	DWORD			AddGlobel;
};
struct MG_Cmd_DialRoleBetGlobel : public NetCmd
{
	DWORD			dwUserID;
	DWORD			AddGlobel;
	BYTE            Index;
	bool			Result;
};
struct LC_Cmd_DialRoleBetGlobel : public NetCmd
{
	DWORD			AddGlobel;
	BYTE            Index;
	bool			Result;
};

struct CL_Cmd_DialGetBankerList : public NetCmd
{

};
struct GM_Cmd_DialGetBankerList : public NetCmd
{
	DWORD			dwUserID;
};
struct MG_Cmd_DialGetBankerList : public NetCmd
{
	DWORD			dwUserID;
	DWORD			dwMySeatIndex;
	DialRoleInfo	TableWriteBankerList[MAX_DIALSHOWBRAND_Sum];
};
struct LC_Cmd_DialGetBankerList : public NetCmd
{
	DWORD			dwMySeatIndex;
	DialRoleInfo	TableWriteBankerList[MAX_DIALSHOWBRAND_Sum];
};

struct CL_Cmd_DialRoleJoinBankerList : public NetCmd
{

};
struct GM_Cmd_DialRoleJoinBankerList : public NetCmd
{
	DWORD		dwUserID;
};
struct MG_Cmd_DialRoleJoinBankerList : public NetCmd
{
	DWORD		dwUserID;
	bool		Result;
	DWORD		SeatIndex;
};
struct LC_Cmd_DialRoleJoinBankerList : public NetCmd
{
	bool		Result;
	DWORD		SeatIndex;
};

struct CL_Cmd_DialRoleGetBankerFirstSeat : public NetCmd
{

};
struct GM_Cmd_DialRoleGetBankerFirstSeat : public NetCmd
{
	DWORD		dwUserID;
};
struct MG_Cmd_DialRoleGetBankerFirstSeat : public NetCmd
{
	DWORD		dwUserID;
	bool		Result;
};
struct LC_Cmd_DialRoleGetBankerFirstSeat : public NetCmd
{
	bool		Result;
};


struct CL_Cmd_DialRoleLeaveBankerList : public NetCmd
{

};
struct GM_Cmd_DialRoleLeaveBankerList : public NetCmd
{
	DWORD			dwUserID;
};
struct MG_Cmd_DialRoleLeaveBankerList : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
};
struct LC_Cmd_DialRoleLeaveBankerList : public NetCmd
{
	bool			Result;
};

struct CL_Cmd_DialRoleCanelBankerSeat : public NetCmd
{

};
struct GM_Cmd_DialRoleCanelBankerSeat : public NetCmd
{
	DWORD		dwUserID;
};
struct MG_Cmd_DialRoleCanelBankerSeat : public NetCmd
{
	DWORD		dwUserID;
	bool		Result;
};
struct LC_Cmd_DialRoleCanelBankerSeat : public NetCmd
{
	bool		Result;
};

struct MG_Cmd_DialBankerListChange : public NetCmd
{
	DWORD		dwUserID;
};
struct LC_Cmd_DialBankerListChange : public NetCmd
{

};

struct MG_Cmd_DialBankerUserChange : public NetCmd  //GameServer处理的时候 需要归还玩家的金币数量
{
	DWORD			dwUserID;
	DialRoleInfo	BankerUserInfo;
	BYTE			GameSum;
};
struct LC_Cmd_DialBankerUserChange : public NetCmd
{
	DialRoleInfo	BankerUserInfo;
	BYTE			GameSum;
};

struct MG_Cmd_DialTableJoinBegin : public NetCmd
{
	DWORD			dwUserID;
	BYTE			TableAreaData[MAX_DIAL_GameSum];//轮盘数据
	BYTE			TableAreaDataRate[MAX_DIAL_ClientSum];//轮盘倍率
};
struct LC_Cmd_DialTableJoinBegin : public NetCmd
{
	BYTE			TableAreaData[MAX_DIAL_GameSum];//轮盘数据
	BYTE			TableAreaDataRate[MAX_DIAL_ClientSum];//轮盘倍率
};

struct MG_Cmd_DialTableUpdate : public NetCmd
{
	DWORD		dwUserID;
	UINT64		TableBetAreaGlobel[MAX_DIAL_ClientSum];
};
struct LC_Cmd_DialTableUpdate : public NetCmd
{
	UINT64		TableBetAreaGlobel[MAX_DIAL_ClientSum];
};
struct DialUserGlobelInfo
{
	DWORD	dwUserID;
	DWORD	dwGlobelSum;
};
struct MG_Cmd_DialTableJoinEnd : public NetCmd
{
	DWORD		dwUserID;
	INT64		AddGlobelSum;
	UINT64		TableBetAreaGlobel[MAX_DIAL_ClientSum];
	WORD		TableResultIndex;
	BYTE		TableGameSum;//记录游戏局数
	DWORD		TableResultLog;//游戏结果记录
	//金币变化
	DialUserGlobelInfo BankerUserGlobelInfo;
	DialUserGlobelInfo BankerListGlobelInfo[MAX_DIALSHOWBRAND_Sum];
	DialUserGlobelInfo VipGlobelInfo[MAX_DIALVIPSEAT_Sum];
};
struct LC_Cmd_DialTableJoinEnd : public NetCmd
{
	INT64		AddGlobelSum;
	UINT64		TableBetAreaGlobel[MAX_DIAL_ClientSum];
	WORD		TableResultIndex;
	BYTE		TableGameSum;//记录游戏局数
	DWORD		TableResultLog;//游戏结果记录
	//金币变化
	DialUserGlobelInfo BankerUserGlobelInfo;
	DialUserGlobelInfo BankerListGlobelInfo[MAX_DIALSHOWBRAND_Sum];
	DialUserGlobelInfo VipGlobelInfo[MAX_DIALVIPSEAT_Sum];
};

struct CL_Cmd_DialRoleLeaveTable : public NetCmd
{

};
struct GM_Cmd_DialRoleLeaveTable : public NetCmd
{
	DWORD		dwUserID;
};

struct MG_Cmd_DialBankerUserGlobelChange : public NetCmd
{
	DWORD		dwUserID;
	DWORD		dwBankerUserID;
	DWORD		dwBankerGlobelSum;
};
struct LC_Cmd_DialBankerUserGlobelChange : public NetCmd
{
	DWORD		dwBankerUserID;
	DWORD		dwBankerGlobelSum;
};

struct MG_Cmd_DialWriteBankerUserGlobelMsg : public NetCmd
{
	DWORD		dwUserID;
};
struct LC_Cmd_DialWriteBankerUserGlobelMsg : public NetCmd
{

};

struct CL_Cmd_DialRoleJoinVipSeat : public NetCmd
{
	BYTE	VipSeatIndex;
};
struct GM_Cmd_DialRoleJoinVipSeat : public NetCmd
{
	DWORD	dwUserID;
	BYTE	VipSeatIndex;
};
struct MG_Cmd_DialRoleJoinVipSeat : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
	BYTE			VipSeatIndex;
	DialRoleInfo	DestUserInfo;

};
struct LC_Cmd_DialRoleJoinVipSeat : public NetCmd
{
	bool			Result;
	BYTE			VipSeatIndex;
	DialRoleInfo	DestUserInfo;
};

struct CL_Cmd_DialRoleLeaveVipSeat : public NetCmd
{

};
struct GM_Cmd_DialRoleLeaveVipSeat : public NetCmd
{
	DWORD	dwUserID;
};
struct MG_Cmd_DialRoleLeaveVipSeat : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	BYTE	VipSeatIndex;
};
struct LC_Cmd_DialRoleLeaveVipSeat : public NetCmd
{
	DWORD	dwDestUserID;
	BYTE	VipSeatIndex;
};

struct MG_Cmd_DialRoleBeLeaveVipSeat : public NetCmd
{
	DWORD		   dwUserID;
	DialRoleInfo DestRoleInfo;
};
struct LC_Cmd_DialRoleBeLeaveVipSeat : public NetCmd
{
	DialRoleInfo DestRoleInfo;
};
struct CL_Cmd_DialGetNormalSeatInfo : public NetCmd
{
	DWORD			Page;
};
struct GM_Cmd_DialGetNormalSeatInfo : public NetCmd
{
	DWORD			dwUserID;
	DWORD			Page;
};
struct MG_Cmd_DialGetNormalSeatInfo : public NetCmd
{
	DWORD			dwUserID;
	DWORD			Page;
	DWORD			TotalRoleSum;
	DialRoleInfo	Array[MAX_DIALNORMAL_PAGESUM];
};
struct LC_Cmd_DialGetNormalSeatInfo : public NetCmd
{
	DWORD			Page;
	DWORD			TotalRoleSum;
	DialRoleInfo	Array[MAX_DIALNORMAL_PAGESUM];
};
struct MG_Cmd_DialVipSeatGlobelChange : public NetCmd
{
	DWORD			dwUserID;
	BYTE			VipSeat;
	BYTE			Index;
	DWORD				GlobelSum;
};
struct LC_Cmd_DialVipSeatGlobelChange : public NetCmd
{
	BYTE			VipSeat;
	BYTE			Index;
	DWORD				GlobelSum;
};
struct MG_Cmd_DialTableStopUpdate : public NetCmd
{
	DWORD			dwUserID;
};
struct LC_Cmd_DialTableStopUpdate : public NetCmd
{

};
struct MG_Cmd_DialVipGlobelChange : public NetCmd
{
	DWORD			dwUserID;
	DWORD			VipUserID;
	DWORD			VipGlobelSum;
};
struct LC_Cmd_DialVipGlobelChange : public NetCmd
{
	DWORD			VipUserID;
	DWORD			VipGlobelSum;
};
struct CL_Cmd_DialRoleBetGlobelByLog : public NetCmd
{
	DWORD			betGlobel[MAX_DIAL_ClientSum];
};
struct GM_Cmd_DialRoleBetGlobelByLog : public NetCmd
{
	DWORD			dwUserID;
	DWORD			betGlobel[MAX_DIAL_ClientSum];
};
struct MG_Cmd_DialRoleBetGlobelByLog : public NetCmd
{
	DWORD			dwUserID;
	DWORD			AllGlobel;
	bool			Result;
	DWORD			betGlobel[MAX_DIAL_ClientSum];
};
struct LC_Cmd_DialRoleBetGlobelByLog : public NetCmd
{
	bool			Result;
	DWORD			betGlobel[MAX_DIAL_ClientSum];
};
//Main_NiuNiu
enum NiuNiuSub
{
	//玩家进入牛牛的桌子
	CL_RoleJoinTable   = 1,
	GM_RoleJoinTable   = 2,
	MG_RoleJoinTable   = 3,
	LC_RoleJoinTable   = 4,

	//玩家离开牛牛桌子
	CL_RoleLeaveTable  = 5,
	GM_RoleLeaveTable  = 6,

	//玩家下注
	CL_RoleBetGlobel   = 7,
	GM_RoleBetGlobel   = 8,
	MG_RoleBetGlobel   = 9,
	LC_RoleBetGlobel   = 10,

	//玩家获取庄家排队列表
	CL_GetBankerList   = 11,
	GM_GetBankerList   = 12,
	MG_GetBankerList   = 13,
	LC_GetBankerList   = 14,

	//庄家队列变化的时候
	MG_BankerListChange = 15,
	LC_BankerListChange = 16,

	//庄家发生变化的时候
	MG_BankerUserChange = 17,
	LC_BankerUserChange = 18,

	//桌子进入下注阶段
	MG_TableJoinBegin   = 19,
	LC_TableJoinBegin   = 20,

	//桌子在下注阶段的更新命令
	MG_TableUpdate      = 21,
	LC_TableUpdate		= 22,

	//桌子进入开牌阶段
	MG_TableJoinEnd		= 23,
	LC_TableJoinEnd		= 24,

	//玩家上庄
	CL_RoleJoinBankerList = 25,
	GM_RoleJoinBankerList = 26,
	MG_RoleJoinBankerList = 27,
	LC_RoleJoinBankerList = 28,

	//玩家抢庄
	CL_RoleGetBankerFirstSeat = 29,
	GM_RoleGetBankerFirstSeat = 30,
	MG_RoleGetBankerFirstSeat = 31,
	LC_RoleGetBankerFirstSeat = 32,

	//玩家离开队列
	CL_RoleLeaveBankerList    = 33,
	GM_RoleLeaveBankerList    = 34,
	MG_RoleLeaveBankerList	  = 35,
	LC_RoleLeaveBankerList    = 36,

	//玩家取消做庄
	CL_RoleCanelBankerSeat    = 37,
	GM_RoleCanelBankerSeat	  = 38,
	MG_RoleCanelBankerSeat    = 39,
	LC_RoleCanelBankerSeat    = 40,

	//庄家金币变化的时候
	MG_BankerUserGlobelChange = 41,
	LC_BankerUserGlobelChange = 42,

	//
	MG_WriteBankerUserGlobelMsg = 43,
	LC_WriteBankerUserGlobelMsg = 44,

	//玩家进入贵宾席
	CL_RoleJoinVipSeat = 45,
	GM_RoleJoinVipSeat = 46,
	MG_RoleJoinVipSeat = 47,
	LC_RoleJoinVipSeat = 48,

	CL_RoleLeaveVipSeat = 49,
	GM_RoleLeaveVipSeat = 50,
	MG_RoleLeaveVipSeat = 51,
	LC_RoleLeaveVipSeat = 52,

	MG_RoleBeLeaveVipSeat = 53,
	LC_RoleBeLeaveVipSeat = 54,

	//大众席数据
	CL_GetNormalSeatInfo = 55,
	GM_GetNormalSeatInfo = 56,
	MG_GetNormalSeatInfo = 57,
	LC_GetNormalSeatInfo = 58,

	//
	MG_VipSeatGlobelChange = 59,
	LC_VipSeatGlobelChange = 60,

	MG_TableStopUpdate = 61,
	LC_TableStopUpdate = 62,

	MG_VipGlobelChange = 63,
	LC_VipGlobelChange = 64,
};
struct NiuNiuRoleInfo
{
	DWORD	dwUserID;
	DWORD	GlobelSum;
	DWORD	dwFaceID;
	TCHAR   NickName[MAX_NICKNAME +1];
};
struct CL_Cmd_RoleJoinTable : public NetCmd
{

};
struct GM_Cmd_RoleJoinTable : public NetCmd
{
	DWORD			dwUserID;
};
struct MG_Cmd_RoleJoinTable : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
	//具体数据 桌子上的数据
	BYTE			TableStates;//桌子状态
	BYTE			TableStatesUpdateSec;//桌子当前状态的秒数
	UINT64			TableBetAreaGlobel[MAX_NIUNIU_ClientSum];//下注区金币数量
	BYTE			TableBrandArray[MAX_NIUNIU_ClientSum + 1][MAX_NIUNIU_BrandSum];//桌子上 5x5 牌
	WORD			TableBrandResult[MAX_NIUNIU_ClientSum + 1];//牌的结果
	NiuNiuRoleInfo  TableBankerInfo;
	BYTE			TableBankerUseGameSum;//当前庄家已经进行游戏的局数
	NiuNiuRoleInfo	TableWriteBankerList[MAX_SHOWBRAND_Sum];
	NiuNiuRoleInfo	VipSeatList[MAX_VIPSEAT_Sum];//贵宾席的玩家
	BYTE			TableGameSum;//记录游戏局数
	DWORD			TableResultLog;//游戏结果记录
};
struct LC_Cmd_RoleJoinTable : public NetCmd
{
	bool			Result;
	//具体数据 桌子上的数据
	BYTE			TableStates;//桌子状态
	BYTE			TableStatesUpdateSec;//桌子当前状态的秒数
	UINT64			TableBetAreaGlobel[MAX_NIUNIU_ClientSum];//下注区金币数量
	BYTE			TableBrandArray[MAX_NIUNIU_ClientSum + 1][MAX_NIUNIU_BrandSum];//桌子上 5x5 牌
	WORD			TableBrandResult[MAX_NIUNIU_ClientSum + 1];//牌的结果
	NiuNiuRoleInfo  TableBankerInfo;
	BYTE			TableBankerUseGameSum;//当前庄家已经进行游戏的局数
	NiuNiuRoleInfo	TableWriteBankerList[MAX_SHOWBRAND_Sum];
	NiuNiuRoleInfo	VipSeatList[MAX_VIPSEAT_Sum];//贵宾席的玩家
	BYTE			TableGameSum;//记录游戏局数
	DWORD			TableResultLog;//游戏结果记录
};

struct CL_Cmd_RoleBetGlobel : public NetCmd
{
	BYTE			Index;
	DWORD			AddGlobel;
};
struct GM_Cmd_RoleBetGlobel : public NetCmd
{
	DWORD			dwUserID;
	BYTE			Index;
	DWORD			AddGlobel;
};
struct MG_Cmd_RoleBetGlobel : public NetCmd
{
	DWORD			dwUserID;
	DWORD			AddGlobel;
	BYTE            Index;
	bool			Result;
};
struct LC_Cmd_RoleBetGlobel : public NetCmd
{
	DWORD			AddGlobel;
	BYTE            Index;
	bool			Result;
};

struct CL_Cmd_GetBankerList : public NetCmd
{

};
struct GM_Cmd_GetBankerList : public NetCmd
{
	DWORD			dwUserID;
};
struct MG_Cmd_GetBankerList : public NetCmd
{
	DWORD			dwUserID;
	DWORD			dwMySeatIndex;
	NiuNiuRoleInfo	TableWriteBankerList[MAX_SHOWBRAND_Sum];
};
struct LC_Cmd_GetBankerList : public NetCmd
{
	DWORD			dwMySeatIndex;
	NiuNiuRoleInfo	TableWriteBankerList[MAX_SHOWBRAND_Sum];
};

struct CL_Cmd_RoleJoinBankerList : public NetCmd
{

};
struct GM_Cmd_RoleJoinBankerList : public NetCmd
{
	DWORD		dwUserID;
};
struct MG_Cmd_RoleJoinBankerList : public NetCmd
{
	DWORD		dwUserID;
	bool		Result;
	DWORD		SeatIndex;
};
struct LC_Cmd_RoleJoinBankerList : public NetCmd
{
	bool		Result;
	DWORD		SeatIndex;
};

struct CL_Cmd_RoleGetBankerFirstSeat : public NetCmd
{

};
struct GM_Cmd_RoleGetBankerFirstSeat : public NetCmd
{
	DWORD		dwUserID;
};
struct MG_Cmd_RoleGetBankerFirstSeat : public NetCmd
{
	DWORD		dwUserID;
	bool		Result;
};
struct LC_Cmd_RoleGetBankerFirstSeat : public NetCmd
{
	bool		Result;
};


struct CL_Cmd_RoleLeaveBankerList : public NetCmd
{

};
struct GM_Cmd_RoleLeaveBankerList : public NetCmd
{
	DWORD			dwUserID;
};
struct MG_Cmd_RoleLeaveBankerList : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
};
struct LC_Cmd_RoleLeaveBankerList : public NetCmd
{
	bool			Result;
};

struct CL_Cmd_RoleCanelBankerSeat : public NetCmd
{

};
struct GM_Cmd_RoleCanelBankerSeat : public NetCmd
{
	DWORD		dwUserID;
};
struct MG_Cmd_RoleCanelBankerSeat : public NetCmd
{
	DWORD		dwUserID;
	bool		Result;
};
struct LC_Cmd_RoleCanelBankerSeat : public NetCmd
{
	bool		Result;
};

struct MG_Cmd_BankerListChange : public NetCmd
{
	DWORD		dwUserID;
};
struct LC_Cmd_BankerListChange : public NetCmd
{

};

struct MG_Cmd_BankerUserChange : public NetCmd  //GameServer处理的时候 需要归还玩家的金币数量
{
	DWORD			dwUserID;
	NiuNiuRoleInfo	BankerUserInfo;
	BYTE			GameSum;
};
struct LC_Cmd_BankerUserChange : public NetCmd
{
	NiuNiuRoleInfo	BankerUserInfo;
	BYTE			GameSum;
};

struct MG_Cmd_TableJoinBegin : public NetCmd
{
	DWORD			dwUserID;
};
struct LC_Cmd_TableJoinBegin : public NetCmd
{

};

struct MG_Cmd_TableUpdate : public NetCmd
{
	DWORD		dwUserID;
	UINT64		TableBetAreaGlobel[MAX_NIUNIU_ClientSum];
};
struct LC_Cmd_TableUpdate : public NetCmd
{
	UINT64		TableBetAreaGlobel[MAX_NIUNIU_ClientSum];
};
struct NiuNiuUserGlobelInfo
{
	DWORD	dwUserID;
	DWORD	dwGlobelSum;
};
struct MG_Cmd_TableJoinEnd : public NetCmd
{
	DWORD		dwUserID;
	INT64		AddGlobelSum;
	UINT64		TableBetAreaGlobel[MAX_NIUNIU_ClientSum];
	BYTE		TableBrandArray[MAX_NIUNIU_ClientSum + 1][MAX_NIUNIU_BrandSum];//桌子上 5x5 牌
	WORD		TableBrandResult[MAX_NIUNIU_ClientSum + 1];//牌的结果
	BYTE		TableGameSum;//记录游戏局数
	DWORD		TableResultLog;//游戏结果记录
	//金币变化
	NiuNiuUserGlobelInfo BankerUserGlobelInfo;
	NiuNiuUserGlobelInfo BankerListGlobelInfo[MAX_SHOWBRAND_Sum];
	NiuNiuUserGlobelInfo VipGlobelInfo[MAX_VIPSEAT_Sum];
};
struct LC_Cmd_TableJoinEnd : public NetCmd
{
	INT64		AddGlobelSum;
	UINT64		TableBetAreaGlobel[MAX_NIUNIU_ClientSum];
	BYTE		TableBrandArray[MAX_NIUNIU_ClientSum + 1][MAX_NIUNIU_BrandSum];//桌子上 5x5 牌
	WORD		TableBrandResult[MAX_NIUNIU_ClientSum + 1];//牌的结果
	BYTE		TableGameSum;//记录游戏局数
	DWORD		TableResultLog;//游戏结果记录
	//金币变化
	NiuNiuUserGlobelInfo BankerUserGlobelInfo;
	NiuNiuUserGlobelInfo BankerListGlobelInfo[MAX_SHOWBRAND_Sum];
	NiuNiuUserGlobelInfo VipGlobelInfo[MAX_VIPSEAT_Sum];
};

struct CL_Cmd_RoleLeaveTable : public NetCmd
{

};
struct GM_Cmd_RoleLeaveTable : public NetCmd
{
	DWORD		dwUserID;
};

struct MG_Cmd_BankerUserGlobelChange : public NetCmd
{
	DWORD		dwUserID;
	DWORD		dwBankerUserID;
	DWORD		dwBankerGlobelSum;
};
struct LC_Cmd_BankerUserGlobelChange : public NetCmd
{
	DWORD		dwBankerUserID;
	DWORD		dwBankerGlobelSum;
};

struct MG_Cmd_WriteBankerUserGlobelMsg : public NetCmd
{
	DWORD		dwUserID;
};
struct LC_Cmd_WriteBankerUserGlobelMsg : public NetCmd
{

};

struct CL_Cmd_RoleJoinVipSeat : public NetCmd
{
	BYTE	VipSeatIndex;
};
struct GM_Cmd_RoleJoinVipSeat : public NetCmd
{
	DWORD	dwUserID;
	BYTE	VipSeatIndex;
};
struct MG_Cmd_RoleJoinVipSeat : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
	BYTE			VipSeatIndex;
	NiuNiuRoleInfo	DestUserInfo;
	
};
struct LC_Cmd_RoleJoinVipSeat : public NetCmd
{
	bool			Result;
	BYTE			VipSeatIndex;
	NiuNiuRoleInfo	DestUserInfo;
};

struct CL_Cmd_RoleLeaveVipSeat : public NetCmd
{

};
struct GM_Cmd_RoleLeaveVipSeat : public NetCmd
{
	DWORD	dwUserID;
};
struct MG_Cmd_RoleLeaveVipSeat : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	BYTE	VipSeatIndex;
};
struct LC_Cmd_RoleLeaveVipSeat : public NetCmd
{
	DWORD	dwDestUserID;
	BYTE	VipSeatIndex;
};

struct MG_Cmd_RoleBeLeaveVipSeat : public NetCmd
{
	DWORD		   dwUserID;
	NiuNiuRoleInfo DestRoleInfo;
};
struct LC_Cmd_RoleBeLeaveVipSeat : public NetCmd
{
	NiuNiuRoleInfo DestRoleInfo;
};
struct CL_Cmd_GetNormalSeatInfo : public NetCmd
{
	DWORD			Page;
};
struct GM_Cmd_GetNormalSeatInfo : public NetCmd
{
	DWORD			dwUserID;
	DWORD			Page;
};
struct MG_Cmd_GetNormalSeatInfo : public NetCmd
{
	DWORD			dwUserID;
	DWORD			Page;
	DWORD			TotalRoleSum;
	NiuNiuRoleInfo  Array[MAX_NORMAL_PAGESUM];
};
struct LC_Cmd_GetNormalSeatInfo : public NetCmd
{
	DWORD			Page;
	DWORD			TotalRoleSum;
	NiuNiuRoleInfo  Array[MAX_NORMAL_PAGESUM];
};
struct MG_Cmd_VipSeatGlobelChange : public NetCmd
{
	DWORD			dwUserID;
	BYTE			VipSeat;
	BYTE			Index;
	DWORD				GlobelSum;
};
struct LC_Cmd_VipSeatGlobelChange : public NetCmd
{
	BYTE			VipSeat;
	BYTE			Index;
	DWORD				GlobelSum;
};
struct MG_Cmd_TableStopUpdate : public NetCmd
{
	DWORD			dwUserID;
};
struct LC_Cmd_TableStopUpdate : public NetCmd
{

};
struct MG_Cmd_VipGlobelChange : public NetCmd
{
	DWORD			dwUserID;
	DWORD			VipUserID;
	DWORD			VipGlobelSum;
};
struct LC_Cmd_VipGlobelChange : public NetCmd
{
	DWORD			VipUserID;
	DWORD			VipGlobelSum;
};
//Main_Char

enum CharSub
{
	CL_LoadAllCharInfo = 1,
	LC_LoadAllCharInfo = 2,

	CL_LoadCharListByUserID = 3,
	LC_LoadCharListByUserID = 4,

	CL_SendCharInfo = 5,
	GC_SendCharInfo = 6,
	CG_SendCharInfo = 7,
	LC_SendCharInfo = 8,
};
struct CL_Cmd_LoadAllCharInfo : public NetCmd
{
	
};
struct LC_Cmd_LoadAllCharInfo : public NetCmd //分包变长命令
{
	BYTE				States;
	WORD				Sum;
	tagRoleCharInfo		Array[1];
};
struct CL_Cmd_LoadCharListByUserID : public NetCmd
{
	DWORD				SrcUserID;
};
struct LC_Cmd_LoadCharListByUserID : public NetCmd
{
	DWORD				SrcUserID;
	BYTE				States;
	WORD				Sum;
	tagRoleCharInfo		Array[1];
};
struct CL_Cmd_SendCharInfo : public NetCmd
{
	tagRoleCharInfo		MessageInfo;
};
struct GC_Cmd_SendCharInfo : public NetCmd
{
	tagRoleCharInfo		MessageInfo;
};
struct CG_Cmd_SendCharInfo : public NetCmd
{
	tagRoleCharInfo		MessageInfo;
};
struct LC_Cmd_SendCharInfo : public NetCmd
{
	tagRoleCharInfo		MessageInfo;
};

//Main_RelationRequest
enum RelationRequestSub
{
	//加载数据
	CL_LoadRelationRequest = 1,
	LC_LoadRelationRequest = 2,

	//
	CL_SendRelationRequest = 3,
	GC_SendRelationRequest = 4,
	CG_SendRelationRequest = 5,
	LC_SendRelationRequest = 6,
	LC_BeAddRelationRequest = 7,

	//
	CL_HandleRelationRequest = 8,
	GC_HandleRelationRequest = 9,
	CG_HandleRelationRequest = 10,
	LC_HandleRelationRequest = 11,
	GC_DestDelRelation = 12,
	CG_DestDelRelation = 13,

	//
	LC_DelRelationRequest = 14,
};
struct CL_Cmd_LoadRelationRequest : public NetCmd
{

};
struct LC_Cmd_LoadRelationRequest : public NetCmd
{
	BYTE				States;
	WORD				Sum;
	tagRelationRequest	Array[1];
};

struct CL_Cmd_SendRelationRequest : public NetCmd
{
	tagRelationRequest		Info;
};
struct GC_Cmd_SendRelationRequest : public NetCmd
{
	tagRelationRequest		Info;
};
struct CG_Cmd_SendRelationRequest : public NetCmd
{
	tagRelationRequest		Info;
};
struct LC_Cmd_SendRelationRequest : public NetCmd
{
	bool					Result;
	BYTE					ErrorID;
	tagRelationRequest		Info;
};
struct LC_Cmd_BeAddRelationRequest : public NetCmd
{
	tagRelationRequest		Info;
};

struct CL_Cmd_HandleRelationRequest : public NetCmd
{
	DWORD					ID;
	bool					Result;
};
struct GC_Cmd_HandleRelationRequest : public NetCmd
{
	tagRelationRequest      Info;
	bool					Result;
};
struct CG_Cmd_HandleRelationRequest : public NetCmd
{
	tagRelationRequest      Info;
	bool					Result;
};
struct LC_Cmd_HandleRelationRequest : public NetCmd
{
	tagRelationRequest      Info;
	bool					Result;
};
struct GC_Cmd_DestDelRelation : public NetCmd
{
	tagRelationRequest      Info;
};
struct CG_Cmd_DestDelRelation : public NetCmd
{
	tagRelationRequest      Info;
};
struct LC_Cmd_DelRelationRequest : public NetCmd
{
	DWORD					ID;
};



//Main_Car
enum CarSub
{
	//玩家进入牛牛的桌子
	CL_CarRoleJoinTable = 1,
	GM_CarRoleJoinTable = 2,
	MG_CarRoleJoinTable = 3,
	LC_CarRoleJoinTable = 4,

	//玩家离开牛牛桌子
	CL_CarRoleLeaveTable = 5,
	GM_CarRoleLeaveTable = 6,

	//玩家下注
	CL_CarRoleBetGlobel = 7,
	GM_CarRoleBetGlobel = 8,
	MG_CarRoleBetGlobel = 9,
	LC_CarRoleBetGlobel = 10,

	//玩家获取庄家排队列表
	CL_CarGetBankerList = 11,
	GM_CarGetBankerList = 12,
	MG_CarGetBankerList = 13,
	LC_CarGetBankerList = 14,

	//庄家队列变化的时候
	MG_CarBankerListChange = 15,
	LC_CarBankerListChange = 16,

	//庄家发生变化的时候
	MG_CarBankerUserChange = 17,
	LC_CarBankerUserChange = 18,

	//桌子进入下注阶段
	MG_CarTableJoinBegin = 19,
	LC_CarTableJoinBegin = 20,

	//桌子在下注阶段的更新命令
	MG_CarTableUpdate = 21,
	LC_CarTableUpdate = 22,

	//桌子进入开牌阶段
	MG_CarTableJoinEnd = 23,
	LC_CarTableJoinEnd = 24,

	//玩家上庄
	CL_CarRoleJoinBankerList = 25,
	GM_CarRoleJoinBankerList = 26,
	MG_CarRoleJoinBankerList = 27,
	LC_CarRoleJoinBankerList = 28,

	//玩家抢庄
	CL_CarRoleGetBankerFirstSeat = 29,
	GM_CarRoleGetBankerFirstSeat = 30,
	MG_CarRoleGetBankerFirstSeat = 31,
	LC_CarRoleGetBankerFirstSeat = 32,

	//玩家离开队列
	CL_CarRoleLeaveBankerList = 33,
	GM_CarRoleLeaveBankerList = 34,
	MG_CarRoleLeaveBankerList = 35,
	LC_CarRoleLeaveBankerList = 36,

	//玩家取消做庄
	CL_CarRoleCanelBankerSeat = 37,
	GM_CarRoleCanelBankerSeat = 38,
	MG_CarRoleCanelBankerSeat = 39,
	LC_CarRoleCanelBankerSeat = 40,

	//庄家金币变化的时候
	MG_CarBankerUserGlobelChange = 41,
	LC_CarBankerUserGlobelChange = 42,

	//
	MG_CarWriteBankerUserGlobelMsg = 43,
	LC_CarWriteBankerUserGlobelMsg = 44,

	//玩家进入贵宾席
	CL_CarRoleJoinVipSeat = 45,
	GM_CarRoleJoinVipSeat = 46,
	MG_CarRoleJoinVipSeat = 47,
	LC_CarRoleJoinVipSeat = 48,

	CL_CarRoleLeaveVipSeat = 49,
	GM_CarRoleLeaveVipSeat = 50,
	MG_CarRoleLeaveVipSeat = 51,
	LC_CarRoleLeaveVipSeat = 52,

	MG_CarRoleBeLeaveVipSeat = 53,
	LC_CarRoleBeLeaveVipSeat = 54,

	//大众席数据
	CL_CarGetNormalSeatInfo = 55,
	GM_CarGetNormalSeatInfo = 56,
	MG_CarGetNormalSeatInfo = 57,
	LC_CarGetNormalSeatInfo = 58,

	//
	MG_CarVipSeatGlobelChange = 59,
	LC_CarVipSeatGlobelChange = 60,

	MG_CarTableStopUpdate = 61,
	LC_CarTableStopUpdate = 62,

	MG_CarVipGlobelChange = 63,
	LC_CarVipGlobelChange = 64,

	//继续下注
	CL_CarRoleBetGlobelByLog = 65,
	GM_CarRoleBetGlobelByLog = 66,
	MG_CarRoleBetGlobelByLog = 67,
	LC_CarRoleBetGlobelByLog = 68,
};
struct CarRoleInfo
{
	DWORD	dwUserID;
	DWORD	GlobelSum;
	DWORD	dwFaceID;
	TCHAR   NickName[MAX_NICKNAME + 1];
};
struct CL_Cmd_CarRoleJoinTable : public NetCmd
{

};
struct GM_Cmd_CarRoleJoinTable : public NetCmd
{
	DWORD			dwUserID;
};
struct MG_Cmd_CarRoleJoinTable : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
	//具体数据 桌子上的数据
	BYTE			TableStates;//桌子状态
	BYTE			TableStatesUpdateSec;//桌子当前状态的秒数
	UINT64			TableBetAreaGlobel[MAX_CAR_ClientSum];//下注区金币数量
	CarRoleInfo	TableBankerInfo;
	BYTE			TableBankerUseGameSum;//当前庄家已经进行游戏的局数
	CarRoleInfo	TableWriteBankerList[MAX_CARSHOWBRAND_Sum];
	CarRoleInfo	VipSeatList[MAX_CARVIPSEAT_Sum];//贵宾席的玩家
	BYTE			TableGameSum;//记录游戏局数
	DWORD			TableResultLog;//游戏结果记录
	BYTE			TableResultIndex;
};
struct LC_Cmd_CarRoleJoinTable : public NetCmd
{
	bool			Result;
	//具体数据 桌子上的数据
	BYTE			TableStates;//桌子状态
	BYTE			TableStatesUpdateSec;//桌子当前状态的秒数
	UINT64			TableBetAreaGlobel[MAX_CAR_ClientSum];//下注区金币数量
	CarRoleInfo	TableBankerInfo;
	BYTE			TableBankerUseGameSum;//当前庄家已经进行游戏的局数
	CarRoleInfo	TableWriteBankerList[MAX_CARSHOWBRAND_Sum];
	CarRoleInfo	VipSeatList[MAX_CARVIPSEAT_Sum];//贵宾席的玩家
	BYTE			TableGameSum;//记录游戏局数
	DWORD			TableResultLog;//游戏结果记录
	BYTE			TableResultIndex;
};

struct CL_Cmd_CarRoleBetGlobel : public NetCmd
{
	BYTE			Index;
	DWORD			AddGlobel;
};
struct GM_Cmd_CarRoleBetGlobel : public NetCmd
{
	DWORD			dwUserID;
	BYTE			Index;
	DWORD			AddGlobel;
};
struct MG_Cmd_CarRoleBetGlobel : public NetCmd
{
	DWORD			dwUserID;
	DWORD			AddGlobel;
	BYTE            Index;
	bool			Result;
};
struct LC_Cmd_CarRoleBetGlobel : public NetCmd
{
	DWORD			AddGlobel;
	BYTE            Index;
	bool			Result;
};

struct CL_Cmd_CarGetBankerList : public NetCmd
{

};
struct GM_Cmd_CarGetBankerList : public NetCmd
{
	DWORD			dwUserID;
};
struct MG_Cmd_CarGetBankerList : public NetCmd
{
	DWORD			dwUserID;
	DWORD			dwMySeatIndex;
	CarRoleInfo	TableWriteBankerList[MAX_CARSHOWBRAND_Sum];
};
struct LC_Cmd_CarGetBankerList : public NetCmd
{
	DWORD			dwMySeatIndex;
	CarRoleInfo	TableWriteBankerList[MAX_CARSHOWBRAND_Sum];
};

struct CL_Cmd_CarRoleJoinBankerList : public NetCmd
{

};
struct GM_Cmd_CarRoleJoinBankerList : public NetCmd
{
	DWORD		dwUserID;
};
struct MG_Cmd_CarRoleJoinBankerList : public NetCmd
{
	DWORD		dwUserID;
	bool		Result;
	DWORD		SeatIndex;
};
struct LC_Cmd_CarRoleJoinBankerList : public NetCmd
{
	bool		Result;
	DWORD		SeatIndex;
};

struct CL_Cmd_CarRoleGetBankerFirstSeat : public NetCmd
{

};
struct GM_Cmd_CarRoleGetBankerFirstSeat : public NetCmd
{
	DWORD		dwUserID;
};
struct MG_Cmd_CarRoleGetBankerFirstSeat : public NetCmd
{
	DWORD		dwUserID;
	bool		Result;
};
struct LC_Cmd_CarRoleGetBankerFirstSeat : public NetCmd
{
	bool		Result;
};


struct CL_Cmd_CarRoleLeaveBankerList : public NetCmd
{

};
struct GM_Cmd_CarRoleLeaveBankerList : public NetCmd
{
	DWORD			dwUserID;
};
struct MG_Cmd_CarRoleLeaveBankerList : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
};
struct LC_Cmd_CarRoleLeaveBankerList : public NetCmd
{
	bool			Result;
};

struct CL_Cmd_CarRoleCanelBankerSeat : public NetCmd
{

};
struct GM_Cmd_CarRoleCanelBankerSeat : public NetCmd
{
	DWORD		dwUserID;
};
struct MG_Cmd_CarRoleCanelBankerSeat : public NetCmd
{
	DWORD		dwUserID;
	bool		Result;
};
struct LC_Cmd_CarRoleCanelBankerSeat : public NetCmd
{
	bool		Result;
};

struct MG_Cmd_CarBankerListChange : public NetCmd
{
	DWORD		dwUserID;
};
struct LC_Cmd_CarBankerListChange : public NetCmd
{

};

struct MG_Cmd_CarBankerUserChange : public NetCmd  //GameServer处理的时候 需要归还玩家的金币数量
{
	DWORD			dwUserID;
	CarRoleInfo	BankerUserInfo;
	BYTE			GameSum;
};
struct LC_Cmd_CarBankerUserChange : public NetCmd
{
	CarRoleInfo	BankerUserInfo;
	BYTE			GameSum;
};

struct MG_Cmd_CarTableJoinBegin : public NetCmd
{
	DWORD			dwUserID;
	//BYTE			TableAreaData[MAX_CAR_GameSum];//轮盘数据
	BYTE			TableAreaDataRate[MAX_CAR_ClientSum];//轮盘倍率
};
struct LC_Cmd_CarTableJoinBegin : public NetCmd
{
	//BYTE			TableAreaData[MAX_CAR_GameSum];//轮盘数据
	BYTE			TableAreaDataRate[MAX_CAR_ClientSum];//轮盘倍率
};

struct MG_Cmd_CarTableUpdate : public NetCmd
{
	DWORD		dwUserID;
	UINT64		TableBetAreaGlobel[MAX_CAR_ClientSum];
};
struct LC_Cmd_CarTableUpdate : public NetCmd
{
	UINT64		TableBetAreaGlobel[MAX_CAR_ClientSum];
};
struct CarUserGlobelInfo
{
	DWORD	dwUserID;
	DWORD	dwGlobelSum;
};
struct MG_Cmd_CarTableJoinEnd : public NetCmd
{
	DWORD		dwUserID;
	INT64		AddGlobelSum;
	UINT64		TableBetAreaGlobel[MAX_CAR_ClientSum];
	BYTE		TableResultIndex;
	BYTE		TableGameSum;//记录游戏局数
	DWORD		TableResultLog;//游戏结果记录
	//金币变化
	CarUserGlobelInfo BankerUserGlobelInfo;
	CarUserGlobelInfo BankerListGlobelInfo[MAX_CARSHOWBRAND_Sum];
	CarUserGlobelInfo VipGlobelInfo[MAX_CARVIPSEAT_Sum];
};
struct LC_Cmd_CarTableJoinEnd : public NetCmd
{
	INT64		AddGlobelSum;
	UINT64		TableBetAreaGlobel[MAX_CAR_ClientSum];
	BYTE		TableResultIndex;
	BYTE		TableGameSum;//记录游戏局数
	DWORD		TableResultLog;//游戏结果记录
	//金币变化
	CarUserGlobelInfo BankerUserGlobelInfo;
	CarUserGlobelInfo BankerListGlobelInfo[MAX_CARSHOWBRAND_Sum];
	CarUserGlobelInfo VipGlobelInfo[MAX_CARVIPSEAT_Sum];
};

struct CL_Cmd_CarRoleLeaveTable : public NetCmd
{

};
struct GM_Cmd_CarRoleLeaveTable : public NetCmd
{
	DWORD		dwUserID;
};

struct MG_Cmd_CarBankerUserGlobelChange : public NetCmd
{
	DWORD		dwUserID;
	DWORD		dwBankerUserID;
	DWORD		dwBankerGlobelSum;
};
struct LC_Cmd_CarBankerUserGlobelChange : public NetCmd
{
	DWORD		dwBankerUserID;
	DWORD		dwBankerGlobelSum;
};

struct MG_Cmd_CarWriteBankerUserGlobelMsg : public NetCmd
{
	DWORD		dwUserID;
};
struct LC_Cmd_CarWriteBankerUserGlobelMsg : public NetCmd
{

};

struct CL_Cmd_CarRoleJoinVipSeat : public NetCmd
{
	BYTE	VipSeatIndex;
};
struct GM_Cmd_CarRoleJoinVipSeat : public NetCmd
{
	DWORD	dwUserID;
	BYTE	VipSeatIndex;
};
struct MG_Cmd_CarRoleJoinVipSeat : public NetCmd
{
	DWORD			dwUserID;
	bool			Result;
	BYTE			VipSeatIndex;
	CarRoleInfo	DestUserInfo;

};
struct LC_Cmd_CarRoleJoinVipSeat : public NetCmd
{
	bool			Result;
	BYTE			VipSeatIndex;
	CarRoleInfo	DestUserInfo;
};

struct CL_Cmd_CarRoleLeaveVipSeat : public NetCmd
{

};
struct GM_Cmd_CarRoleLeaveVipSeat : public NetCmd
{
	DWORD	dwUserID;
};
struct MG_Cmd_CarRoleLeaveVipSeat : public NetCmd
{
	DWORD	dwUserID;
	DWORD	dwDestUserID;
	BYTE	VipSeatIndex;
};
struct LC_Cmd_CarRoleLeaveVipSeat : public NetCmd
{
	DWORD	dwDestUserID;
	BYTE	VipSeatIndex;
};

struct MG_Cmd_CarRoleBeLeaveVipSeat : public NetCmd
{
	DWORD		   dwUserID;
	CarRoleInfo DestRoleInfo;
};
struct LC_Cmd_CarRoleBeLeaveVipSeat : public NetCmd
{
	CarRoleInfo DestRoleInfo;
};
struct CL_Cmd_CarGetNormalSeatInfo : public NetCmd
{
	DWORD			Page;
};
struct GM_Cmd_CarGetNormalSeatInfo : public NetCmd
{
	DWORD			dwUserID;
	DWORD			Page;
};
struct MG_Cmd_CarGetNormalSeatInfo : public NetCmd
{
	DWORD			dwUserID;
	DWORD			Page;
	DWORD			TotalRoleSum;
	CarRoleInfo	Array[MAX_CARNORMAL_PAGESUM];
};
struct LC_Cmd_CarGetNormalSeatInfo : public NetCmd
{
	DWORD			Page;
	DWORD			TotalRoleSum;
	CarRoleInfo	Array[MAX_CARNORMAL_PAGESUM];
};
struct MG_Cmd_CarVipSeatGlobelChange : public NetCmd
{
	DWORD			dwUserID;
	BYTE			VipSeat;
	BYTE			Index;
	DWORD				GlobelSum;
};
struct LC_Cmd_CarVipSeatGlobelChange : public NetCmd
{
	BYTE			VipSeat;
	BYTE			Index;
	DWORD				GlobelSum;
};
struct MG_Cmd_CarTableStopUpdate : public NetCmd
{
	DWORD			dwUserID;
};
struct LC_Cmd_CarTableStopUpdate : public NetCmd
{

};
struct MG_Cmd_CarVipGlobelChange : public NetCmd
{
	DWORD			dwUserID;
	DWORD			VipUserID;
	DWORD			VipGlobelSum;
};
struct LC_Cmd_CarVipGlobelChange : public NetCmd
{
	DWORD			VipUserID;
	DWORD			VipGlobelSum;
};
struct CL_Cmd_CarRoleBetGlobelByLog : public NetCmd
{
	DWORD			betGlobel[MAX_CAR_ClientSum];
};
struct GM_Cmd_CarRoleBetGlobelByLog : public NetCmd
{
	DWORD			dwUserID;
	DWORD			betGlobel[MAX_CAR_ClientSum];
};
struct MG_Cmd_CarRoleBetGlobelByLog : public NetCmd
{
	DWORD			dwUserID;
	DWORD			AllGlobel;
	bool			Result;
	DWORD			betGlobel[MAX_CAR_ClientSum];
};
struct LC_Cmd_CarRoleBetGlobelByLog : public NetCmd
{
	bool			Result;
	DWORD			betGlobel[MAX_CAR_ClientSum];
};



#pragma pack(pop)