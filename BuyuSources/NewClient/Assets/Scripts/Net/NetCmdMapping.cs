using System;
using System.Collections.Generic;

//====================================================
//           网络命令ID
//====================================================
public enum MainCmdType
{
    CMD_MAIN_TYPE = 0,//游戏内部的命令

    CMD_MAIN_Logon = 1,//玩家登陆命令整理
    CMD_MAIN_Table = 2,//桌子
    CMD_MAIN_Game = 3,//游戏
    CMD_MAIN_Center = 4, //中央
    CMD_MAIN_Balanced = 5,//负载
    CMD_MAIN_Item = 6,//物品
    CMD_MAIN_Relation = 7,//关系
    CMD_MAIN_Mail = 8,//邮件
    CMD_MAIN_Role = 9,//玩家
    CMD_MAIN_Query = 10,//查询
    CMD_MAIN_Check = 11,//签到
    CMD_MAIN_Task = 12,//任务
    CMD_MAIN_Achievement = 13,//玩家成就
    CMD_MAIN_Title = 14,//玩家称号
    CMD_MAIN_Month = 15,//比赛
    CMD_MAIN_Rank = 16,//排行榜
    CMD_MAIN_Chest = 17,//宝箱
    CMD_MAIN_Charm = 18,//魅力
    CMD_MAIN_Shop = 19,//商店
    CMD_MAIN_Entity = 20,//实体
    CMD_MAIN_Action = 21,//活动
    CMD_MAIN_Giff = 22,//赠送
   // CMD_MAIN_GlobelShop=23,//金币商店
    CMD_MAIN_OnlineReward = 24,
    CMD_MAIN_FTP = 25,
    CMD_MAIN_GameData  = 26,
    CMD_MAIN_Package  = 27,
    CMD_Main_Launcher = 28,
    CMD_Main_Message  = 29,
    CMD_Main_Recharge = 30,
    CMD_Main_Announcement = 31,
    CMD_Main_Operate = 32,
    CMD_Main_RoleMessage = 33,
    CMD_Main_Exchange = 34,
    CMD_Main_Lottery = 35,
    CMD_EXTRA_DOWN = 36,
    CMD_Main_MiniGame = 37,
    CMD_Main_NiuNiu  = 38,
    CMD_Main_Char = 39,
    CMD_Main_RelationRequest = 40,
    CMD_Main_Dial = 41,
    CMD_Main_Car = 42,
}
//public enum LOGON_SubCmd
//{
//    CL_Sub_Account_Logon = 1,
//    CL_Sub_Account_Rsg = 2,
//    CL_Sub_Account_Query = 3,
//    LC_Sub_Account_LogonSucess = 10,//登陆 注册成功
//    LC_Sub_Account_LogonError = 11,//登陆失败
//    LC_Sub_Account_RsgError = 12,//注册失败
//    LC_Sub_Account_QueryError = 13,//快速登陆失败
//    LC_Sub_Account_VersionError = 14,//客户端需要版本更新
//    LC_Sub_QueryAccount = 15,
//    CL_Sub_QueryRsg = 16,
//    LC_Sub_QueryRsgError = 17,
//    LC_Sub_DayChange = 18,

//    CL_Ping = 20,
//    LC_Ping = 21,
//}
public enum LOGON_SubCmd
{
    CL_Sub_AccountLogon     = 1,
    CL_Sub_AccountRsg       = 2,
    //CL_Sub_AccountQueryRsg  = 3,
    LC_Sub_AccountOnlyID    = 5,
    CL_Sub_AccountOnlyID        = 6,
    LC_Sub_AccountOnlyIDFailed  = 7,
    LC_Sub_AccountOnlyIDSuccess = 8,

    CL_Sub_ResetLogonGameServer = 12,
    LC_Sub_ResetLogonGameServer = 13,
    LC_Sub_AccountLogonToScreen = 14,

    LC_Sub_ServerChangeSocket = 15,//玩家在游戏中被顶掉了

    CL_Sub_ResetPassword = 16,
    LC_Sub_ResetPassword = 17,

    CL_Sub_AccountResetAccount = 18,
    LC_Sub_AccountResetAccount = 19,

    LC_Sub_LogonQueueWrite = 20,

    CL_Sub_GetNewAccount = 23,
    LC_Sub_GetNewAccount = 24,

    LC_Sub_CheckVersionError = 25,

    LC_Sub_AccountIsFreeze = 26,

    CL_Sub_ChannelLogon = 30,
    LC_Sub_ChannelLogon = 33,


    CL_Sub_PhoneLogon       = 50,
    LC_Sub_PhoneLogon       = 53,
    CL_Sub_PhoneLogonCheck  = 54,
    LC_Sub_PhoneLogonCheck  = 57,
    LC_Sub_SaveAccountInfo  = 58,

    CL_Sub_PhoneSecPwd      = 70,

    CL_Sub_WeiXinLogon      = 80,
    LC_Sub_WeiXinLogon      = 83,

    CL_Sub_QQLogon          = 84,
    LC_Sub_QQLogon          = 87,

    LC_Sub_RsgNewAccount    = 88,
}
public enum TABLE_SubCmd
{
    CL_Sub_JoinTable = 1,//进入桌子
    LC_Sub_JoinTable = 2,//进入桌子的返回
    CL_Sub_LeaveTable = 3,
    //桌子上消息同步
    LC_Sub_OtherUserInfo = 4,//其他玩家的消息
    LC_Sub_UserLeave = 5,//玩家离开桌子
    LC_Sub_MeSeatID = 6,//设置玩家自己的位置

    LC_Sub_TableChangeRoleLevel = 10,
    LC_Sub_TableChangeRoleGender = 11,
    LC_Sub_TableChangeRoleNickName = 12,
    LC_Sub_TableChangeRoleFaceID = 13,
    LC_Sub_TableChangeRoleGlobe = 14,
    //LC_Sub_TableChangeRoleExp = 15,
    LC_Sub_TableChangeRoleTitle = 16,
    LC_Sub_TableChangeRoleAchievementPoint = 17,
    LC_Sub_TableChangeRoleCharmValue = 18,
    LC_Sub_TableChangeRoleIpAddress = 19,
    LC_Sub_TableChangeRoleVipLevel      = 20,
    LC_Sub_TableChangeRoleIsInMonthCard = 21,


    LC_Sub_ResetOtherUserInfo = 30,

    LC_Sub_LeaveTableByServer = 31,
}
public enum SubMainCmdType
{
    SUB_HEARBEAT,

    //登陆相关
    SUB_FAST_LOGON,
    SUB_NORMAL_LOGON,
    SUB_REGISTER_LOGON,

    SUB_LOGON_RESULT,
    SUB_LOGON_RESULT_SIMPLE,
    SUB_LOGON_HALL,
    SUB_LOGON_HALL_RESULT,

    SUB_JOIN_ROOM,
    SUB_JOIN_ROOM_RESULT,

    SUB_PLAYER_READY,

    SUB_BULLET                          ,
    SUB_FISH                            ,
    SUB_CATCHED                         ,
    SUB_PLAYER_JOIN                     ,
    SUB_PLAYER_LEAVE                    ,
    SUB_CHANGE_LAUNCHER                 ,
    SUB_CHANGE_LAUNCHER_TYPE            ,
    SUB_CHANGE_CLIENT_RATE              ,
    SUB_CHANGE_RATE                     ,
    SUB_CHANGE_RATE_TYPE                ,
    SUB_SYNC_FISH                       ,
    SUB_ROOM_INFO                       ,
    SUB_SYNC_BULLET                     ,
    SUB_PING                            ,
    SUB_REDUCTION                       ,
                                 
    SUB_USE_SKILL                       ,
            
    //技能相关命令                      
    SUB_SKILL_TORNADO_REQUEST           ,
    SUB_SKILL_TORNADO_RESPONSE          ,
    SUB_SKILL_FREEZE_REQUEST            ,
    SUB_SKILL_FREEZE_RESPONSE           ,
    SUB_SKILL_LIGHTING_REQUEST          ,
    SUB_SKILL_LIGHTING_RESPONSE         ,
    SUB_SKILL_LASER_REQUEST             ,
    SUB_SKILL_LASER_RESPONSE            ,
    SUB_SKILL_DISASTER_REQUEST          ,
    SUB_SKILL_DISASTER_RESPONSE         ,
    SUB_SKILL_LOCK_REQUEST              ,
    SUB_SKILL_LOCK_RESPONSE             ,

    SUB_BULLET_REDUCTION                ,
    SUB_UPLOADING_REQUEST               ,
    SUB_UPLOADING_RESPONSE              ,
    SUB_UPLOADING_CHUNK                 ,
    SUB_UPLOADING_CHUNK_RESPONSE        ,
    SUB_UPLOADING_COMPLETE              ,
    SUB_CHANGE_IMG                      ,
    SUB_FISH_PACKAGE                    ,
                
    SUB_CHECK_FISH_POS                  ,
    SUB_CHECK_BULLET_POS                ,
    SUB_CLEAR_SCENE                     ,
    SUB_LAUNCH_LASER_FAILED             ,
    SUB_SKILL_FAILED,                                    
                                        
    CMD_SUB_DOUBLEHIT_EFFECT            ,
}
public enum ITEM_SubCmd
{
    CL_Sub_GetUserItem = 1,
    LC_Sub_GetUserItem = 2,
    //LC_Sub_GetUserItemFinish = 3,
    LC_Sub_AddUserItem = 4,
    LC_Sub_DelUserItem = 5,
    LC_Sub_ChangeUserItem = 6,

    CL_Sub_OnUseItem = 7,
    LC_Sub_OnUseItem = 8,

    CL_Sub_OnAcceptItem = 9,
    LC_Sub_OnAcceptItem = 10,
}
public enum Relation_SubCmd
{
    CL_Sub_GetUserRelation = 1,
    LC_Sub_GetUserRelation = 2,
    //LC_Sub_GetUserRelationFinish = 3,

    CL_Sub_AddUserRelation = 4,
    LC_Sub_AddUserRelation = 5,
    CL_Sub_DelUserRelation = 6,
    LC_Sub_DelUserRelation = 7,
    //CL_Sub_ChangeUserRelation = 8,
    LC_Sub_ChangeUserRelation = 9,

    LC_Sub_RoleChangeOnline = 20,

    //继续添加
    LC_Sub_ChangeRelationLevel = 30,
    LC_Sub_ChangeRelationFaceID = 31,
    LC_Sub_ChangeRelationNickName = 32,
    LC_Sub_ChangeRelationGender = 33,
    LC_Sub_ChangeRelationTitle = 34,
    LC_Sub_ChangeRelationAchievementPoint = 35,
    LC_Sub_ChangeRelationCharmValue = 36,
    LC_Sub_ChangeRelationIP = 37,

    LC_Sub_ChangeRelationVipLevel           = 42,
    LC_Sub_ChangeRelationIsInMonthCard      = 44,
}
public enum Mail_SubCmd
{
    CL_Sub_GetUserMail = 1,
    LC_Sub_GetUserSystemMail = 2,
    LC_Sub_GetUserNormalMail = 3,
    //LC_Sub_GetUserMailFinish = 4,
    //CL_Sub_SendUserMail = 5,
    //LC_Sub_SendUserMail = 6,
    CL_Sub_DelUserMail = 8,
    LC_Sub_DelUserMail = 9,
    CL_Sub_GetMailItem = 10,
    LC_Sub_GetMailItem = 11,
    //LC_Sub_ChangeMailIsRead = 12,
    CL_Sub_ReadMail = 13,
    LC_Sub_ReadNormalMail = 14,
    LC_Sub_ReadSystemMail = 15,
    LC_Sub_ReadMailError = 16,
    LC_Sub_AddNormalTitle = 19,
    LC_Sub_AddSystemTitle = 20,

    CL_Sub_SendUserMail         = 23,
    LC_Sub_SendUserMailResult   = 28,
}
public enum Role_SubCmd
{
    LC_Sub_ChangeRoleExp = 1,
    LC_Sub_ChangeRoleLevel = 2,
    LC_Sub_ChangeRoleGender = 3,
    LC_Sub_ChangeRoleNickName = 4,
    LC_Sub_ChangeRoleFaceID = 5,
    LC_Sub_ChangeRoleGlobe = 6,
    LC_Sub_ChangeRoleMedal = 7,
    LC_Sub_ChangeRoleCurrency = 8,
    LC_Sub_ChangeRoleTitle = 9,
    LC_Sub_ChangeRoleAchievementPoint = 10,
    LC_Sub_ChangeRoleCharmValue = 11,
    LC_Sub_ChangeRoleSendGiffSum = 12,
    LC_Sub_ChangeRoleAcceptGiffSum = 13,

    LC_Sub_ChangeRoleTaskStates = 14,
    LC_Sub_ChangeRoleAchievementStates = 15,
    LC_Sub_ChangeRoleActionStates = 16,

    CL_Sub_ChangeRoleGender = 40,
    CL_Sub_ChangeRoleNickName = 41,
    CL_Sub_ChangeRoleNormalFaceID = 42,
    //CL_Sub_RequestUserFaceID = 43,//请求更换头像
    //LC_Sub_ResponseUserFaceID = 44,
    //CL_Sub_UpLoadingchunk = 45,
    //LC_Sub_UpLoadingResponse = 46,
    //LC_Sub_UpLoadingFinish = 47,
    //LC_Sub_UpLoadFtpError = 48,

    LC_Sub_DayChange = 49,

    LC_Sub_ChangeRoleAchievementIndex = 52,

    LC_ResetRoleGlobel = 53,

    CL_Sub_ResetRoleInfo = 54,
    LC_Sub_ResetRoleInfo = 55,

    LC_Sub_ChangeRoleCheckData = 56,

    CL_Sub_ChangeRoleIsShowIpAddress = 57,
    LC_Sub_ChangeRoleIsShowIpAddress = 58,
    LC_Sub_NewDay = 60,

    LC_Sub_ChangeRoleExChangeStates = 62,
    LC_Sub_ChangeRoleTotalRecharge = 63,
    LC_Sub_RoleProtect = 64,

    LC_Sub_ChangeRoleIsFirstPayGlobel = 65,
    LC_Sub_ChangeRoleIsFirstPayCurrcey = 66,

    LC_Sub_ChangeRoleMonthCardInfo  = 67,
    LC_Sub_ChangeRoleRateValue      = 68,
    CL_Sub_ChangeRoleRateValue      = 69,

    CL_Sub_GetMonthCardReward = 70,
    LC_Sub_GetMonthCardReward = 71,

    LC_Sub_ChangeRoleVipLevel = 72,

    LC_Sub_ChangeRoleCashSum  = 75,
    CL_SUB_RoleProtect = 76,

    LC_Sub_ChangeRoleParticularStates = 79,

    LC_Sub_SetSecondPassword    = 80,
    CL_Sub_ChangeSecondPassword = 81,
    LC_Sub_ChangeSecondPassword = 82,

    CL_Sub_ChangeRoleShareStates = 83,
    LC_Sub_ChangeRoleShareStates = 84,

    LC_Sub_OpenShareUI = 85,

    CL_Sub_UpdateAccount = 86,
}
public enum Query_SubCmd
{
    CL_Sub_QueryUserByNickName = 1,
    CL_Sub_QueryUserByUserID = 2,
    CL_Sub_QueryUserByGameID = 3,

    LC_Sub_QueryUserInfo = 10,
    //LC_Sub_QueryUserInfoFinish = 11,
    LC_Sub_QueryUserOnce = 12,
}
public enum Check_SubCmd
{
    //CL_Sub_GetRoleCheckInfo = 1,
   // LC_Sub_GetRoleCheckInfo = 2,
    CL_Sub_CheckNowDay = 4,
    LC_Sub_CheckNowDay = 5,
    CL_Sub_CheckOtherDay = 6,
    LC_Sub_CheckOtherDay = 7,
   // LC_Sub_CheckChange = 8,
}
public enum Task_SubCmd
{
    CL_Sub_GetRoleTaskInfo = 1,
    LC_Sub_GetRoleTaskInfo = 2,
    //LC_Sub_GetRoleTaskInfoFinish = 3,
    LC_Sub_TaskAllEventFinish = 5,
    CL_Sub_GetTaskReward = 6,
    LC_Sub_GetTaskReward = 7,
    //LC_Sub_ResetTask = 8,
    CL_Sub_GetOnceTaskInfo = 9,
    LC_Sub_GetOnceTaskInfo = 10,

    LC_Sub_JoinTask = 12,
    LC_Sub_ClearTask = 13,
    LC_Sub_DelTask = 14,
}
public enum Achievement_SubCmd
{
    CL_Sub_GetRoleAchievementInfo = 1,
    LC_Sub_GetRoleAchievementInfo = 2,
   // LC_Sub_GetRoleAchievementInfoFinish = 3,
    LC_Sub_AchievementAllEventFinish = 5,
    CL_Sub_GetAchievementReward = 6,
    LC_Sub_GetAchievementReward = 7,
    CL_Sub_GetOnceAchievementInfo = 9,
    LC_Sub_GetOnceAchievementInfo = 10,

    LC_Sub_JoinAchievement = 11,
    LC_Sub_DelAchievement = 12,
}
public enum Title_SubCmd
{
    CL_Sub_LoadRoleTitle = 1,
    LC_Sub_LoadRoleTitle = 2,
    //LC_Sub_LoadRoleTitleFinish = 3,
    LC_Sub_AddRoleTitle = 4,
    LC_Sub_DelRoleTitle = 5,
    CL_Sub_ChangeRoleCurrTitle = 6,
}
public enum Month_SubCmd
{
    CL_Sub_SignUpMonth = 1,//玩家请求报名比赛
    LC_Sub_SignUpMonthSucess = 6,
    LC_Sub_SignUpMonthFiled = 7,
    CL_Sub_JoinMonth = 8,
    LC_Sub_JoinMonthSucess = 11,
    LC_Sub_JoinMonthFiled = 12,
    CL_Sub_ChangeUserAddMonthGlobelNum = 15,//玩家申请续币
    LC_Sub_ChangeUserAddMonthGlobelNum = 17,//发送到中央服务器去   
    LC_Sub_UserChangeIndex = 19,
    LC_Sub_UserMonthEnd = 21,

    CL_Sub_LoadRoleSignUpInfo = 32,
    LC_Sub_LoadRoleSignUpInfo = 33,


   // CL_Sub_GetMonthRoleSum = 37,
    LC_Sub_GetMonthRoleSum = 38,
   // LC_Sub_GetMonthRoleSumFinish = 39,

    CL_Sub_ResetMonthInfo = 40,
    LC_Sub_ResetMonthInfo = 43,
    LC_Sub_SetRoleMonthInfo = 44,

    LC_Sub_ResetRoleMonthInfo = 45,

    LC_Sub_MonthList =49,
}
public enum Rank_SubCmd
{
    CL_Sub_GetRankReward = 1,
    LC_Sub_GetRankReward = 4,
    CL_Sub_GetRankInfo = 5,
    LC_Sub_GetWeekRankInfo = 9,
    //LC_Sub_GetRankInfoFinish = 10,
}
public enum Chest_SubCmd
{
    //LC_Sub_AddChest = 1,
    LC_Sub_ActionChest = 2,
    LC_Sub_ChestEnd = 3,
    CL_Sub_GetChestReward = 4,
    LC_Sub_GetChestReward = 5,

    CL_Sub_CloseChest = 6,

    LC_Sub_ResetChest = 7,
}
public enum Charm_SubCmd
{
    CL_Sub_ChangeOtherUserCharm = 1,
    LC_Sub_ChangeRoleCharmResult = 5,

    LC_Sub_TableRoleCharmInfo = 6,
}
public enum Shop_SubCmd
{
    CL_Sub_ShopItem = 1,
    LC_Sub_ShopItemResult = 2,
}
public enum Entity_SubCmd
{
    CL_Sub_LoadUserEntity = 1,
    LC_Sub_LoadUserEntity = 2,
   // CL_Sub_ChangeRoleName = 3,
    LC_Sub_ChangeRoleName = 4,
  //  CL_Sub_ChangeRolePhone = 5,
    LC_Sub_ChangeRolePhone = 6,
    //CL_Sub_ChangeRoleEmail = 7,
    LC_Sub_ChangeRoleEmail = 8,
   // CL_Sub_ChangeRoleAddress = 9,
   // LC_Sub_ChangeRoleAddress = 10,
   // CL_Sub_ChangeRoleEntityID = 11,
    LC_Sub_ChangeRoleEntityID = 12,

    CL_Sub_ChangeRoleEntityItemUseName = 15,
    LC_Sub_ChangeRoleEntityItemUseName = 16,
    CL_Sub_ChagneRoleEntityItemUsePhone = 17,
    LC_Sub_ChangeRoleEntityItemUsePhone = 18,
    CL_Sub_ChangeRoleEntityItemUseAddress = 19,
    LC_Sub_ChangeRoleEntityItemUseAddress = 20,
}

public enum Action_SubCmd
{
    CL_Sub_GetRoleActionInfo = 1,
    LC_Sub_GetRoleActionInfo = 2,
   // LC_Sub_GetRoleActionInfoFinish = 3,
   // LC_Sub_ActionAllEventFinish = 5,
    CL_Sub_GetActionReward = 6,
    LC_Sub_GetActionReward = 7,
    CL_Sub_GetOnceActionInfo = 9,
    LC_Sub_GetOnceActionInfo = 10,

    LC_Sub_JoinAction = 11,
    LC_Sub_DelAction = 12,
}
public enum Giff_SubCmd
{
    CL_Sub_GetRoleGiff = 1,
    LC_Sub_GetRoleGiff = 2,
    //LC_Sub_GetRoleGiffFinish = 3,
    //CL_Sub_AddRoleGiff = 4,
    //LC_Sub_AddRoleGiffResult = 5,
    //LC_Sub_AddBeRoleGiff = 8,
    CL_Sub_GetRoleGiffReward = 9,
    LC_Sub_GetRoleGiffReward = 10,
    CL_Sub_GetAllRoleGiffReward = 11,
    LC_Sub_GetAllRoleGiffReward = 12,


    CL_Sub_GetRoleNowDaySendGiff = 13,
    LC_Sub_GetRoleNowDaySendGiff = 14,
    //LC_Sub_GetRoleNowDaySendGiffFinish = 15,
    LC_Sub_DelRoleGiff = 16,

    CL_Sub_AddRoleGiff              = 17,
    LC_Sub_AddRoleGiffResult        = 22,
    LC_Sub_AddBeRoleGiff            = 23,
}
//public enum GlobelShop_SubCmd
//{
//    CL_Sub_GlobelShopItem = 1,
//    LC_Sub_GlobelShopItem = 2,
//}
public enum OnlineReward_SubCmd
{
    CL_Sub_GetOnlineReward = 1,
    LC_Sub_GetOnlineReward = 2,
}
public enum GameData_SubCmd
{
    CL_Sub_GetGameData  = 1,
    LC_Sub_GendGameData = 2,
}
public enum Package_SubCmd
{
   // CL_Sub_OpenPackage = 1,
    LC_Sub_OpenPackage = 2,
}
public enum Launcher_SubCmd
{
    LC_Sub_LauncherData = 1,
}
public enum Message_SubCmd
{
    LC_Sub_SendMessage = 3,//GameServer->Client
    CL_Sub_SendMessage = 4,//Client->GameServer
}
public enum Announcement_SubCmd
{
    CL_Sub_GetAllAnnouncement           = 6,
    LC_Sub_GetAllAnnouncement           = 7,
    //LC_Sub_GetAllAnnouncementFinish     = 8,
    LC_Sub_SendNewAnnouncementOnce      = 9,
}
public enum Recharge_SubCmd
{
    CL_Sub_Recharge = 1,
    LC_Sub_Recharge = 2,
    LC_Sub_GetOrderID = 3,
    CL_Sub_IOSRecharge = 4,//IOS平台充值
}
public enum Operate_SubCmd
{
    CL_Sub_RealNameVerification = 1,//将  名称  身份证号码 发送给 GameServer
    LC_Sub_RealNameVerification = 4,//将验证的结果进行显示 携带ErrorID 
    CL_Sub_GetPhoneVerificationNum = 11,//发送给GameServer 携带手机号码 
    LC_Sub_GetPhoneVerificationNum = 14,//通知客户端 发送手机验证码的结果 ErrorID 
    CL_Sub_BindPhone = 15,//将验证码 和 手机号码 发送到GameServer
    LC_Sub_BindPhone = 18,//将绑定手机操作的结果返回客户端
    LC_Sub_BuyEntityItem = 23,//将购买实体物品的结果返回给客户端 ErrorID

    CL_Sub_BindEmail = 31,//发送服务器端 绑定邮件
    LC_Sub_BindEmail = 35,//告诉客户端 绑定邮箱的结果

    LC_Sub_PhonePay = 43,

    LC_Sub_AddNormalOrderID = 53,
}
public enum RoleMessage_SubCmd
{
    LC_Sub_RoleMessageChange = 1,
}
public enum ExChange_SubCmd
{
    CL_Sub_RoleUseExChangeCode = 1,
    LC_Sub_RoleUseExChangeCode = 2,
}
public enum Lottery_SubCmd
{
    CL_Sub_GetLotteryReward = 1,
    LC_Sub_GetLotteryReward = 2,

    CL_Sub_LotteryUIStates = 3,
    LC_Sub_LotteryUIStates = 4,
}
public enum EXTRA_DOWN
{
    EXTRA_DOWN_CHECK_VER = 1,
    EXTRA_DOWN_FILE = 2,
    EXTRA_DOWN_FILE_EX = 3,
}
//public enum MiniGame_SubCmd
//{
//    CL_Sub_JoinRoom             = 1,//进入牛牛房间
//    LC_Sub_JoinRoom             = 4,
//    LC_Sub_TableUpdate          = 6,
//    CL_Sub_AddGlobel            = 7,//下注
//    LC_Sub_AddGlobel            = 10,//下注的结果
//    LC_Sub_TableResult          = 12,
//    LC_Sub_TableBegin           = 14,
//    CL_Sub_LeaveNiuNiuTable     = 15,

//    LC_Sub_BankerChange             = 18,
//    LC_Sub_WirteBankerListChange    = 20,
//    CL_Sub_AddBankerList            = 21,
//    LC_Sub_AddBankerList            = 24,
//}
public enum NiuNiu_SubCmd
{
    CL_Sub_RoleJoinTable            = 1,
    LC_Sub_RoleJoinTable            = 4,
    CL_Sub_RoleLeaveTable           = 5,
    CL_Sub_RoleBetGlobel            = 7,
    LC_Sub_RoleBetGlobel            = 10,
    CL_Sub_GetBankerList            = 11,
    LC_Sub_GetBankerList            = 14,
    LC_Sub_BankerListChange         = 16,
    LC_Sub_BankerUserChange         = 18,
    LC_Sub_TableJoinBegin           = 20,
    LC_Sub_TableUpdate              = 22,
    LC_Sub_TableJoinEnd             = 24,
    CL_Sub_RoleJoinBankerList       = 25,
    LC_Sub_RoleJoinBankerList       = 28,
    CL_Sub_RoleGetBankerFirstSeat   = 29,
    LC_Sub_RoleGetBankerFirstSeat   = 32,
    CL_Sub_RoleLeaveBankerList      = 33,
    LC_Sub_RoleLeaveBankerList      = 36,
    CL_Sub_RoleCanelBankerSeat      = 37,
    LC_Sub_RoleCanelBankerSeat      = 40,
    LC_Sub_BankerUserGlobelChange   = 42,
    LC_Sub_WriteBankerUserGlobelMsg = 44,

    //玩家进入贵宾席
    CL_Sub_RoleJoinVipSeat          = 45,
    LC_Sub_RoleJoinVipSeat          = 48,
    CL_Sub_RoleLeaveVipSeat         = 49,
    LC_Sub_RoleLeaveVipSeat         = 52,
    LC_Sub_RoleBeLeaveVipSeat       = 54,

    CL_Sub_GetNormalSeatInfo        = 55,
    LC_Sub_GetNormalSeatInfo        = 58,

    LC_Sub_VipSeatGlobelChange      = 60,

    LC_Sub_TableStopUpdate          = 62,

    LC_Sub_VipGlobelChange          = 64,
}
public enum Dial_SubCmd
{
    CL_Sub_DialRoleJoinTable = 1,
    LC_Sub_DialRoleJoinTable = 4,
    CL_Sub_DialRoleLeaveTable = 5,
    CL_Sub_DialRoleBetGlobel = 7,
    LC_Sub_DialRoleBetGlobel = 10,
    CL_Sub_DialGetBankerList = 11,
    LC_Sub_DialGetBankerList = 14,
    LC_Sub_DialBankerListChange = 16,
    LC_Sub_DialBankerUserChange = 18,
    LC_Sub_DialTableJoinBegin = 20,
    LC_Sub_DialTableUpdate = 22,
    LC_Sub_DialTableJoinEnd = 24,
    CL_Sub_DialRoleJoinBankerList = 25,
    LC_Sub_DialRoleJoinBankerList = 28,
    CL_Sub_DialRoleGetBankerFirstSeat = 29,
    LC_Sub_DialRoleGetBankerFirstSeat = 32,
    CL_Sub_DialRoleLeaveBankerList = 33,
    LC_Sub_DialRoleLeaveBankerList = 36,
    CL_Sub_DialRoleCanelBankerSeat = 37,
    LC_Sub_DialRoleCanelBankerSeat = 40,
    LC_Sub_DialBankerUserGlobelChange = 42,
    LC_Sub_DialWriteBankerUserGlobelMsg = 44,

    //玩家进入贵宾席
    CL_Sub_DialRoleJoinVipSeat = 45,
    LC_Sub_DialRoleJoinVipSeat = 48,
    CL_Sub_DialRoleLeaveVipSeat = 49,
    LC_Sub_DialRoleLeaveVipSeat = 52,
    LC_Sub_DialRoleBeLeaveVipSeat = 54,

    CL_Sub_DialGetNormalSeatInfo = 55,
    LC_Sub_DialGetNormalSeatInfo = 58,

    LC_Sub_DialVipSeatGlobelChange = 60,

    LC_Sub_DialTableStopUpdate = 62,

    LC_Sub_DialVipGlobelChange = 64,

    CL_Sub_DialRoleBetGlobelByLog = 65,
    LC_Sub_DialRoleBetGlobelByLog = 68,
}
public enum Char_SubCmd
{
    CL_Sub_LoadAllCharInfo          = 1,
    LC_Sub_LoadAllCharInfo          = 2,
    CL_Sub_LoadCharListByUserID     = 3,
    LC_Sub_LoadCharListByUserID     = 4,
    CL_Sub_SendCharInfo             = 5,
    LC_Sub_SendCharInfo             = 8,
}

public enum RelationRequest_SubCmd
{
    CL_Sub_LoadRelationRequest      = 1,
    LC_Sub_LoadRelationRequest      = 2,
    CL_Sub_SendRelationRequest      = 3,
    LC_Sub_SendRelationRequest      = 6,
    LC_Sub_BeAddRelationRequest     = 7,
    CL_Sub_HandleRelationRequest    = 8,
    LC_Sub_HandleRelationRequest    = 11,
    LC_Sub_DelRelationRequest       = 14,
}

public enum Car_SubCmd
{
    CL_Sub_CarRoleJoinTable = 1,
    LC_Sub_CarRoleJoinTable = 4,
    CL_Sub_CarRoleLeaveTable = 5,
    CL_Sub_CarRoleBetGlobel = 7,
    LC_Sub_CarRoleBetGlobel = 10,
    CL_Sub_CarGetBankerList = 11,
    LC_Sub_CarGetBankerList = 14,
    LC_Sub_CarBankerListChange = 16,
    LC_Sub_CarBankerUserChange = 18,
    LC_Sub_CarTableJoinBegin = 20,
    LC_Sub_CarTableUpdate = 22,
    LC_Sub_CarTableJoinEnd = 24,
    CL_Sub_CarRoleJoinBankerList = 25,
    LC_Sub_CarRoleJoinBankerList = 28,
    CL_Sub_CarRoleGetBankerFirstSeat = 29,
    LC_Sub_CarRoleGetBankerFirstSeat = 32,
    CL_Sub_CarRoleLeaveBankerList = 33,
    LC_Sub_CarRoleLeaveBankerList = 36,
    CL_Sub_CarRoleCanelBankerSeat = 37,
    LC_Sub_CarRoleCanelBankerSeat = 40,
    LC_Sub_CarBankerUserGlobelChange = 42,
    LC_Sub_CarWriteBankerUserGlobelMsg = 44,

    //玩家进入贵宾席
    CL_Sub_CarRoleJoinVipSeat = 45,
    LC_Sub_CarRoleJoinVipSeat = 48,
    CL_Sub_CarRoleLeaveVipSeat = 49,
    LC_Sub_CarRoleLeaveVipSeat = 52,
    LC_Sub_CarRoleBeLeaveVipSeat = 54,

    CL_Sub_CarGetNormalSeatInfo = 55,
    LC_Sub_CarGetNormalSeatInfo = 58,

    LC_Sub_CarVipSeatGlobelChange = 60,

    LC_Sub_CarTableStopUpdate = 62,

    LC_Sub_CarVipGlobelChange = 64,

    CL_Sub_CarRoleBetGlobelByLog = 65,
    LC_Sub_CarRoleBetGlobelByLog = 68,
}
public enum NetCmdType
{
    CMD_HEARTBEAT                   = MainCmdType.CMD_MAIN_TYPE << 8 | SubMainCmdType.SUB_HEARBEAT,
    //登陆相关
    CMD_FAST_LOGON                  = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_FAST_LOGON,
    CMD_NORMAL_LOGON                = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_NORMAL_LOGON,
    CMD_REGISTER_LOGON              = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_REGISTER_LOGON,
    CMD_LOGON_RESULT                = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_LOGON_RESULT,
    CMD_LOGON_RESULT_SIMPLE         = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_LOGON_RESULT_SIMPLE,
    CMD_LOGON_HALL                  = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_LOGON_HALL,
    CMD_LOGON_HALL_RESULT           = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_LOGON_HALL_RESULT,
    CMD_JOIN_ROOM                   = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_JOIN_ROOM,
    CMD_JOIN_ROOM_RESULT            = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_JOIN_ROOM_RESULT,
    CMD_PLAYER_READY                = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_PLAYER_READY,
    CMD_BULLET                      = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_BULLET,
    CMD_FISH                        = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_FISH,
    CMD_CATCHED                     = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CATCHED,
    CMD_PLAYER_JOIN                 = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_PLAYER_JOIN,
    CMD_PLAYER_LEAVE                = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_PLAYER_LEAVE,
    CMD_CHANGE_LAUNCHER             = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CHANGE_LAUNCHER,
    CMD_CHANGE_LAUNCHER_TYPE        = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CHANGE_LAUNCHER_TYPE,
    CMD_CHANGE_CLIENT_RATE          = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CHANGE_CLIENT_RATE,
    CMD_CHANGE_RATE                 = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CHANGE_RATE,
    CMD_CHANGE_RATE_TYPE            = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CHANGE_RATE_TYPE,
    CMD_SYNC_FISH                   = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SYNC_FISH,
    CMD_ROOM_INFO                   = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_ROOM_INFO,
    CMD_SYNC_BULLET                 = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SYNC_BULLET,
    CMD_PING                        = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_PING,
    CMD_REDUCTION                   = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_REDUCTION,
    CMD_USE_SKILL                   = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_USE_SKILL,
    //技能相关命令                              
    CMD_SKILL_TORNADO_REQUEST       = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_TORNADO_REQUEST,
    CMD_SKILL_TORNADO_RESPONSE      = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_TORNADO_RESPONSE,
    CMD_SKILL_FREEZE_REQUEST        = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_FREEZE_REQUEST,
    CMD_SKILL_FREEZE_RESPONSE       = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_FREEZE_RESPONSE,
    CMD_SKILL_LIGHTING_REQUEST      = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_LIGHTING_REQUEST,
    CMD_SKILL_LIGHTING_RESPONSE     = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_LIGHTING_RESPONSE,
    CMD_SKILL_LASER_REQUEST         = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_LASER_REQUEST,
    CMD_SKILL_LASER_RESPONSE        = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_LASER_RESPONSE,
    CMD_SKILL_DISASTER_REQUEST      = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_DISASTER_REQUEST,
    CMD_SKILL_DISASTER_RESPONSE     = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_DISASTER_RESPONSE,

    CMD_SKILL_LOCK_RESPONSE         = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_LOCK_RESPONSE,
    CMD_SKILL_LOCK_REQUEST          = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_LOCK_REQUEST,

    CMD_BULLET_REDUCTION            = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_BULLET_REDUCTION,
    CMD_UPLOADING_REQUEST           = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_UPLOADING_REQUEST,
    CMD_UPLOADING_RESPONSE          = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_UPLOADING_RESPONSE,
    CMD_UPLOADING_CHUNK             = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_UPLOADING_CHUNK,
    CMD_UPLOADING_CHUNK_RESPONSE    = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_UPLOADING_CHUNK_RESPONSE,
    CMD_UPLOADING_COMPLETE          = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_UPLOADING_COMPLETE,
    CMD_CHANGE_IMG                  = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CHANGE_IMG,
    CMD_FISH_PACKAGE                = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_FISH_PACKAGE,
    CMD_CHECK_FISH_POS              = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CHECK_FISH_POS,
    CMD_CHECK_BULLET_POS            = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CHECK_BULLET_POS,
    CMD_CLEAR_SCENE                 = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_CLEAR_SCENE,
    CMD_LAUNCH_LASER_FAILED         = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_LAUNCH_LASER_FAILED,
    CMD_SKILL_FAILLED               = MainCmdType.CMD_MAIN_Game << 8 | SubMainCmdType.SUB_SKILL_FAILED,

    //其他的命令
    //CMD_CL_Account_Logon                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_Account_Logon),
    //CMD_CL_Account_Rsg                  = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_Account_Rsg),
    //CMD_CL_Account_Query                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_Account_Query),
    //CMD_LC_Account_LogonSucess          = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_Account_LogonSucess),
    //CMD_LC_Account_LogonError           = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_Account_LogonError),
    //CMD_LC_Account_RsgError             = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_Account_RsgError),
    //CMD_LC_Account_QueryError           = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_Account_QueryError),
    //CMD_LC_Account_VersionError         = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_Account_VersionError),
    //CMD_LC_DayChange                    = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_DayChange),
    //CMD_LC_QueryAccount                 = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_QueryAccount),
    //CMD_CL_QueryRsg                     = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_QueryRsg),
    //CMD_LC_QueryRsgError                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_QueryRsgError),
    ////桌子
    CMD_CL_JoinTable = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.CL_Sub_JoinTable),
    CMD_LC_JoinTable = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_JoinTable),
    CMD_CL_LeaveTable = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.CL_Sub_LeaveTable),
    //登陆
    CMD_CL_AccountLogon                 = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_AccountLogon),
    CMD_CL_AccountRsg                   = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_AccountRsg),
    //CMD_CL_AccountQueryRsg              = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_AccountQueryRsg),
    CMD_LC_AccountOnlyID                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_AccountOnlyID),
    CMD_CL_AccountOnlyID                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_AccountOnlyID),
    CMD_LC_AccountOnlyIDFailed          = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_AccountOnlyIDFailed),
    CMD_LC_AccountOnlyIDSuccess         = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_AccountOnlyIDSuccess),

    CMD_CL_ResetLogonGameServer         = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_ResetLogonGameServer),
    CMD_LC_ResetLogonGameServer         = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_ResetLogonGameServer),
    CMD_LC_AccountLogonToScreen         = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_AccountLogonToScreen),
    CMD_LC_ServerChangeSocket           = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_ServerChangeSocket),

    CMD_CL_ResetPassword                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_ResetPassword),
    CMD_LC_ResetPassword                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_ResetPassword),

    CMD_CL_AccountResetAccount          = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_AccountResetAccount),
    CMD_LC_AccountResetAccount          = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_AccountResetAccount),

    CMD_LC_LogonQueueWrite              = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_LogonQueueWrite),

    CMD_CL_GetNewAccount                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_GetNewAccount),
    CMD_LC_GetNewAccount                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_GetNewAccount),
    CMD_LC_CheckVersionError            = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_CheckVersionError),
    CMD_LC_AccountIsFreeze              = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_AccountIsFreeze),
    
    CMD_CL_ChannelLogon                 = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_ChannelLogon),
    CMD_LC_ChannelLogon                 = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_ChannelLogon),

    CMD_CL_PhoneLogon                   = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_PhoneLogon),
    CMD_LC_PhoneLogon                   = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_PhoneLogon),
    CMD_CL_PhoneLogonCheck              = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_PhoneLogonCheck),
    CMD_LC_PhoneLogonCheck              = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_PhoneLogonCheck),
    CMD_LC_SaveAccountInfo              = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_SaveAccountInfo),
    CMD_CL_PhoneSecPwd                  = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_PhoneSecPwd),
    CMD_CL_WeiXinLogon                  = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_WeiXinLogon),
    CMD_LC_WeiXinLogon                  = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_WeiXinLogon),
    CMD_CL_QQLogon                      = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.CL_Sub_QQLogon),
    CMD_LC_QQLogon                      = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_QQLogon),

    CMD_LC_RsgNewAccount                = (int)(MainCmdType.CMD_MAIN_Logon << 8) | (int)(LOGON_SubCmd.LC_Sub_RsgNewAccount),  
    
    

    CMD_LC_OtherUserInfo                = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_OtherUserInfo),
    CMD_LC_UserLeave                    = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_UserLeave),
    CMD_LC_MeSeatID                     = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_MeSeatID),
    CMD_LC_TableChangeRoleLevel         = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleLevel),
    CMD_LC_TableChangeRoleGender        = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleGender),
    CMD_LC_TableChangeRoleNickName      = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleNickName),
    CMD_LC_TableChangeRoleFaceID        = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleFaceID),
    CMD_LC_TableChangeRoleGlobe         = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleGlobe),
    //CMD_LC_TableChangeRoleExp           = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleExp),
    CMD_LC_TableChangeRoleTitle         = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleTitle),
    CMD_LC_TableChangeRoleAchievementPoint=(int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleAchievementPoint),
    CMD_LC_TableChangeRoleCharmValue    = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleCharmValue),
    CMD_LC_TableChangeRoleIpAddress     = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleIpAddress),

    CMD_LC_TableChangeRoleVipLevel      = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleVipLevel),
    CMD_LC_TableChangeRoleIsInMonthCard = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_TableChangeRoleIsInMonthCard),
         
    

    CMD_LC_ResetOtherUserInfo           = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_ResetOtherUserInfo),
    CMD_LC_LeaveTableByServer           = (int)(MainCmdType.CMD_MAIN_Table << 8) | (int)(TABLE_SubCmd.LC_Sub_LeaveTableByServer),
    
    

    //物品
    CMD_CL_GetUserItem                  = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.CL_Sub_GetUserItem),
    CMD_LC_GetUserItem                  = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.LC_Sub_GetUserItem),
   // CMD_LC_GetUserItemFinish            = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.LC_Sub_GetUserItemFinish),
    CMD_LC_AddUserItem                  = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.LC_Sub_AddUserItem),
    CMD_LC_DelUserItem                  = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.LC_Sub_DelUserItem),
    CMD_LC_ChangeUserItem               = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.LC_Sub_ChangeUserItem),
    CMD_CL_OnUseItem                    = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.CL_Sub_OnUseItem),
    CMD_LC_OnUseItem                    = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.LC_Sub_OnUseItem),

    CMD_CL_OnAcceptItem                 = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.CL_Sub_OnAcceptItem),
    CMD_LC_OnAcceptItem                 = (int)(MainCmdType.CMD_MAIN_Item << 8) | (int)(ITEM_SubCmd.LC_Sub_OnAcceptItem),
     
     
     
    //关系
    CMD_CL_GetUserRelation              = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.CL_Sub_GetUserRelation),
    CMD_LC_GetUserRelation              = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_GetUserRelation),
    //CMD_LC_GetUserRelationFinish        = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_GetUserRelationFinish),
    CMD_CL_AddUserRelation              = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.CL_Sub_AddUserRelation),
    CMD_LC_AddUserRelation              = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_AddUserRelation),
    CMD_CL_DelUserRelation              = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.CL_Sub_DelUserRelation),
    CMD_LC_DelUserRelation              = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_DelUserRelation),
    //CMD_CL_ChangeUserRelation           = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.CL_Sub_ChangeUserRelation),
    CMD_LC_ChangeUserRelation           = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeUserRelation),
    CMD_LC_RoleChangeOnline             = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_RoleChangeOnline),
    CMD_LC_ChangeRelationLevel          = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationLevel),
    CMD_LC_ChangeRelationFaceID         = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationFaceID),
    CMD_LC_ChangeRelationNickName       = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationNickName),
    CMD_LC_ChangeRelationGender         = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationGender),
    CMD_LC_ChangeRelationTitle          = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationTitle),
    CMD_LC_ChangeRelationAchievementPoint=(int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationAchievementPoint),
    CMD_LC_ChangeRelationCharmValue     = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationCharmValue),
    CMD_LC_ChangeRelationIP             = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationIP),

    CMD_LC_ChangeRelationVipLevel               = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationVipLevel),
    CMD_LC_ChangeRelationIsInMonthCard          = (int)(MainCmdType.CMD_MAIN_Relation << 8) | (int)(Relation_SubCmd.LC_Sub_ChangeRelationIsInMonthCard),
    
         
    
    //邮件
    CMD_CL_GetUserMail                  = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.CL_Sub_GetUserMail),
    CMD_LC_GetUserSystemMail            = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_GetUserSystemMail),
    CMD_LC_GetUserNormalMail            = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_GetUserNormalMail),
   //CMD_LC_GetUserMailFinish            = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_GetUserMailFinish),
    //CMD_CL_SendUserMail                 = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.CL_Sub_SendUserMail),
    //CMD_LC_SendUserMail                 = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_SendUserMail),
    CMD_CL_DelUserMail                  = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.CL_Sub_DelUserMail),
    CMD_LC_DelUserMail                  = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_DelUserMail),
    CMD_CL_GetMailItem                  = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.CL_Sub_GetMailItem),
    CMD_LC_GetMailItem                  = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_GetMailItem),
    CMD_CL_ReadMail                     = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.CL_Sub_ReadMail),
    CMD_LC_ReadNormalMail               = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_ReadNormalMail),
    CMD_LC_ReadSystemMail               = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_ReadSystemMail),
    CMD_LC_ReadMailError                = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_ReadMailError),
    CMD_LC_AddNormalTitle               = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_AddNormalTitle),
    CMD_LC_AddSystemTitle               = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_AddSystemTitle),

    CMD_CL_SendUserMail                 = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.CL_Sub_SendUserMail),
    CMD_LC_SendUserMailResult           = (int)(MainCmdType.CMD_MAIN_Mail << 8) | (int)(Mail_SubCmd.LC_Sub_SendUserMailResult),
           
     
    //Role
    CMD_LC_ChangeRoleExp                = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleExp),
    CMD_LC_ChangeRoleLevel              = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleLevel),
    CMD_LC_ChangeRoleGender             = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleGender),
    CMD_LC_ChangeRoleNickName           = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleNickName),
    CMD_LC_ChangeRoleFaceID             = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleFaceID),
    CMD_LC_ChangeRoleGlobe              = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleGlobe),
    CMD_LC_ChangeRoleMedal              = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleMedal),
    CMD_LC_ChangeRoleCurrency           = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleCurrency),
    CMD_LC_ChangeRoleTitle              = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleTitle),
    CMD_LC_ChangeRoleAchievementPoint   = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleAchievementPoint),
    CMD_LC_ChangeRoleCharmValue         = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleCharmValue),
    CMD_LC_ChangeRoleSendGiffSum        = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleSendGiffSum),
    CMD_LC_ChangeRoleAcceptGiffSum      = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleAcceptGiffSum),
    CMD_CL_ChangeRoleGender             = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_ChangeRoleGender),
    CMD_CL_ChangeRoleNickName           = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_ChangeRoleNickName),
    CMD_CL_ChangeRoleNormalFaceID       = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_ChangeRoleNormalFaceID),
    CMD_LC_ChangeRoleTaskStates         = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleTaskStates),
    CMD_LC_ChangeRoleAchievementStates  = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleAchievementStates),
    CMD_LC_ChangeRoleActionStates       = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleActionStates),
    //CMD_CL_RequestUserFaceID            = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_RequestUserFaceID),
    //CMD_LC_ResponseUserFaceID           = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ResponseUserFaceID),
    //CMD_CL_UpLoadingchunk               = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_UpLoadingchunk),
    //CMD_LC_UpLoadingResponse            = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_UpLoadingResponse),
    //CMD_LC_UpLoadingFinish              = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_UpLoadingFinish),
    //CMD_LC_UpLoadFtpError               = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_UpLoadFtpError),
    CMD_LC_DayChange                    = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_DayChange),
    CMD_LC_ChangeRoleAchievementIndex   = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleAchievementIndex),
    CMD_LC_ResetRoleGlobel              = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_ResetRoleGlobel),

    CMD_CL_ResetRoleInfo                = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_ResetRoleInfo),
    CMD_LC_ResetRoleInfo                = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ResetRoleInfo),
    CMD_LC_ChangeRoleCheckData          = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleCheckData),

    CMD_CL_ChangeRoleIsShowIpAddress    = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_ChangeRoleIsShowIpAddress),
    CMD_LC_ChangeRoleIsShowIpAddress    = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleIsShowIpAddress),
    CMD_LC_NewDay                       = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_NewDay),

    CMD_LC_ChangeRoleExChangeStates     = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleExChangeStates),
    CMD_LC_ChangeRoleTotalRecharge      = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleTotalRecharge),
    CMD_LC_RoleProtect                  = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_RoleProtect),
    CMD_CL_RoleProtect                  =(int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_SUB_RoleProtect),

    CMD_LC_ChangeRoleParticularStates   = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleParticularStates),

    CMD_LC_SetSecondPassword            = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_SetSecondPassword),
    CMD_CL_ChangeSecondPassword         = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_ChangeSecondPassword),
    CMD_LC_ChangeSecondPassword         = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeSecondPassword),

    CMD_CL_ChangeRoleShareStates        = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_ChangeRoleShareStates),
    CMD_LC_ChangeRoleShareStates        = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleShareStates),

    CMD_LC_OpenShareUI                  = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_OpenShareUI),

    CMD_CL_UpdateAccount                = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_UpdateAccount),
    

    CMD_LC_ChangeRoleIsFirstPayGlobel   = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleIsFirstPayGlobel),
    CMD_LC_ChangeRoleIsFirstPayCurrcey  = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleIsFirstPayCurrcey),
     
    CMD_LC_ChangeRoleMonthCardInfo      = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleMonthCardInfo),
    CMD_LC_ChangeRoleRateValue          = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleRateValue),
    CMD_CL_ChangeRoleRateValue          = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_ChangeRoleRateValue),

    CMD_CL_GetMonthCardReward           = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.CL_Sub_GetMonthCardReward),
    CMD_LC_GetMonthCardReward           = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_GetMonthCardReward),

    CMD_LC_ChangeRoleVipLevel           = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleVipLevel),
    CMD_LC_ChangeRoleCashSum            = (int)(MainCmdType.CMD_MAIN_Role << 8) | (int)(Role_SubCmd.LC_Sub_ChangeRoleCashSum),
    
    //Query
    CMD_CL_QueryUserByNickName          = (int)(MainCmdType.CMD_MAIN_Query << 8) | (int)(Query_SubCmd.CL_Sub_QueryUserByNickName),
    CMD_CL_QueryUserByUserID            = (int)(MainCmdType.CMD_MAIN_Query << 8) | (int)(Query_SubCmd.CL_Sub_QueryUserByUserID),
    CMD_CL_QueryUserByGameID            = (int)(MainCmdType.CMD_MAIN_Query << 8) | (int)(Query_SubCmd.CL_Sub_QueryUserByGameID),
    
    CMD_LC_QueryUserInfo                = (int)(MainCmdType.CMD_MAIN_Query << 8) | (int)(Query_SubCmd.LC_Sub_QueryUserInfo),
   // CMD_LC_QueryUserInfoFinish          = (int)(MainCmdType.CMD_MAIN_Query << 8) | (int)(Query_SubCmd.LC_Sub_QueryUserInfoFinish),
    CMD_LC_QueryUserOnce                = (int)(MainCmdType.CMD_MAIN_Query << 8) | (int)(Query_SubCmd.LC_Sub_QueryUserOnce),
    //Check
    //CMD_CL_GetRoleCheckInfo             = (int)(MainCmdType.CMD_MAIN_Check << 8) | (int)(Check_SubCmd.CL_Sub_GetRoleCheckInfo),
    //CMD_LC_GetRoleCheckInfo             = (int)(MainCmdType.CMD_MAIN_Check << 8) | (int)(Check_SubCmd.LC_Sub_GetRoleCheckInfo),
    CMD_CL_CheckNowDay                  = (int)(MainCmdType.CMD_MAIN_Check << 8) | (int)(Check_SubCmd.CL_Sub_CheckNowDay),
    CMD_LC_CheckNowDay                  = (int)(MainCmdType.CMD_MAIN_Check << 8) | (int)(Check_SubCmd.LC_Sub_CheckNowDay),
    CMD_CL_CheckOtherDay                = (int)(MainCmdType.CMD_MAIN_Check << 8) | (int)(Check_SubCmd.CL_Sub_CheckOtherDay),
    CMD_LC_CheckOtherDay                = (int)(MainCmdType.CMD_MAIN_Check << 8) | (int)(Check_SubCmd.LC_Sub_CheckOtherDay),
    //CMD_LC_CheckChange                  = (int)(MainCmdType.CMD_MAIN_Check << 8) | (int)(Check_SubCmd.LC_Sub_CheckChange),
    //任务
    CMD_CL_GetRoleTaskInfo              = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.CL_Sub_GetRoleTaskInfo),
    CMD_LC_GetRoleTaskInfo              = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.LC_Sub_GetRoleTaskInfo),
   // CMD_LC_GetRoleTaskInfoFinish        = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.LC_Sub_GetRoleTaskInfoFinish),
    CMD_LC_TaskAllEventFinish           = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.LC_Sub_TaskAllEventFinish),
    CMD_CL_GetTaskReward                = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.CL_Sub_GetTaskReward),
    CMD_LC_GetTaskReward                = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.LC_Sub_GetTaskReward),
    //CMD_LC_ResetTask                    = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.LC_Sub_ResetTask),
    CMD_CL_GetOnceTaskInfo              = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.CL_Sub_GetOnceTaskInfo),
    CMD_LC_GetOnceTaskInfo              = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.LC_Sub_GetOnceTaskInfo),
    CMD_LC_JoinTask                     = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.LC_Sub_JoinTask),
    CMD_LC_ClearTask                    = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.LC_Sub_ClearTask),
    CMD_LC_DelTask                      = (int)(MainCmdType.CMD_MAIN_Task << 8) | (int)(Task_SubCmd.LC_Sub_DelTask),
    //成就
    CMD_CL_GetRoleAchievementInfo       = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.CL_Sub_GetRoleAchievementInfo),
    CMD_LC_GetRoleAchievementInfo       = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.LC_Sub_GetRoleAchievementInfo),
    //CMD_LC_GetRoleAchievementInfoFinish = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.LC_Sub_GetRoleAchievementInfoFinish),
    CMD_LC_AchievementAllEventFinish    = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.LC_Sub_AchievementAllEventFinish),
    CMD_CL_GetAchievementReward         = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.CL_Sub_GetAchievementReward),
    CMD_LC_GetAchievementReward         = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.LC_Sub_GetAchievementReward),
    CMD_CL_GetOnceAchievementInfo       = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.CL_Sub_GetOnceAchievementInfo),
    CMD_LC_GetOnceAchievementInfo       = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.LC_Sub_GetOnceAchievementInfo),

    CMD_LC_JoinAchievement              = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.LC_Sub_JoinAchievement),
    CMD_LC_DelAchievement               = (int)(MainCmdType.CMD_MAIN_Achievement << 8) | (int)(Achievement_SubCmd.LC_Sub_DelAchievement),
    //称号
    CMD_CL_LoadRoleTitle                = (int)(MainCmdType.CMD_MAIN_Title << 8) | (int)(Title_SubCmd.CL_Sub_LoadRoleTitle),
    CMD_LC_LoadRoleTitle                = (int)(MainCmdType.CMD_MAIN_Title << 8) | (int)(Title_SubCmd.LC_Sub_LoadRoleTitle),
    //CMD_LC_LoadRoleTitleFinish          = (int)(MainCmdType.CMD_MAIN_Title << 8) | (int)(Title_SubCmd.LC_Sub_LoadRoleTitleFinish),
    CMD_LC_AddRoleTitle                 = (int)(MainCmdType.CMD_MAIN_Title << 8) | (int)(Title_SubCmd.LC_Sub_AddRoleTitle),
    CMD_LC_DelRoleTitle                 = (int)(MainCmdType.CMD_MAIN_Title << 8) | (int)(Title_SubCmd.LC_Sub_DelRoleTitle),
    CMD_CL_ChangeRoleCurrTitle          = (int)(MainCmdType.CMD_MAIN_Title << 8) | (int)(Title_SubCmd.CL_Sub_ChangeRoleCurrTitle),
    //比赛
    CMD_CL_SignUpMonth                  = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.CL_Sub_SignUpMonth),
    CMD_LC_SignUpMonthSucess            = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_SignUpMonthSucess),
    CMD_LC_SignUpMonthFiled             = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_SignUpMonthFiled),
    CMD_CL_JoinMonth                    = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.CL_Sub_JoinMonth),
    CMD_LC_JoinMonthSucess              = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_JoinMonthSucess),
    CMD_LC_JoinMonthFiled               = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_JoinMonthFiled),
    CMD_CL_ChangeUserAddMonthGlobelNum  = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.CL_Sub_ChangeUserAddMonthGlobelNum),
    CMD_LC_ChangeUserAddMonthGlobelNum  = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_ChangeUserAddMonthGlobelNum),
    CMD_LC_UserChangeIndex              = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_UserChangeIndex),
    CMD_LC_UserMonthEnd                 = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_UserMonthEnd),
    CMD_CL_LoadRoleSignUpInfo           = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.CL_Sub_LoadRoleSignUpInfo),
    CMD_LC_LoadRoleSignUpInfo           = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_LoadRoleSignUpInfo),
   // CMD_CL_GetMonthRoleSum              = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.CL_Sub_GetMonthRoleSum),
    CMD_LC_GetMonthRoleSum              = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_GetMonthRoleSum),
    //CMD_LC_GetMonthRoleSumFinish        = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_GetMonthRoleSumFinish),
    CMD_CL_ResetMonthInfo               = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.CL_Sub_ResetMonthInfo),
    CMD_LC_ResetMonthInfo               = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_ResetMonthInfo),
    CMD_LC_SetRoleMonthInfo               = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_SetRoleMonthInfo),
    CMD_LC_ResetRoleMonthInfo               = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_ResetRoleMonthInfo),
    CMD_LC_MonthList                    = (int)(MainCmdType.CMD_MAIN_Month << 8) | (int)(Month_SubCmd.LC_Sub_MonthList),
    
    //排行榜
    CMD_CL_GetRankReward                = (int)(MainCmdType.CMD_MAIN_Rank << 8) | (int)(Rank_SubCmd.CL_Sub_GetRankReward),
    CMD_LC_GetRankReward                = (int)(MainCmdType.CMD_MAIN_Rank << 8) | (int)(Rank_SubCmd.LC_Sub_GetRankReward),
    CMD_CL_GetRankInfo                  = (int)(MainCmdType.CMD_MAIN_Rank << 8) | (int)(Rank_SubCmd.CL_Sub_GetRankInfo),
    CMD_LC_GetWeekRankInfo              = (int)(MainCmdType.CMD_MAIN_Rank << 8) | (int)(Rank_SubCmd.LC_Sub_GetWeekRankInfo),
   // CMD_LC_GetRankInfoFinish            = (int)(MainCmdType.CMD_MAIN_Rank << 8) | (int)(Rank_SubCmd.LC_Sub_GetRankInfoFinish),
    //宝箱
    //CMD_LC_AddChest                     = (int)(MainCmdType.CMD_MAIN_Chest << 8) | (int)(Chest_SubCmd.LC_Sub_AddChest),
    CMD_LC_ActionChest                  = (int)(MainCmdType.CMD_MAIN_Chest << 8) | (int)(Chest_SubCmd.LC_Sub_ActionChest),
    CMD_LC_ChestEnd                     = (int)(MainCmdType.CMD_MAIN_Chest << 8) | (int)(Chest_SubCmd.LC_Sub_ChestEnd),
    CMD_CL_GetChestReward               = (int)(MainCmdType.CMD_MAIN_Chest << 8) | (int)(Chest_SubCmd.CL_Sub_GetChestReward),
    CMD_LC_GetChestReward               = (int)(MainCmdType.CMD_MAIN_Chest << 8) | (int)(Chest_SubCmd.LC_Sub_GetChestReward),
    CMD_CL_CloseChest                   = (int)(MainCmdType.CMD_MAIN_Chest << 8) | (int)(Chest_SubCmd.CL_Sub_CloseChest),
    CMD_LC_ResetChest                   = (int)(MainCmdType.CMD_MAIN_Chest << 8) | (int)(Chest_SubCmd.LC_Sub_ResetChest),
    
    //魅力
    CMD_CL_ChangeOtherUserCharm         = (int)(MainCmdType.CMD_MAIN_Charm << 8) | (int)(Charm_SubCmd.CL_Sub_ChangeOtherUserCharm),
    CMD_LC_ChangeRoleCharmResult        = (int)(MainCmdType.CMD_MAIN_Charm << 8) | (int)(Charm_SubCmd.LC_Sub_ChangeRoleCharmResult),
    CMD_LC_TableRoleCharmInfo           = (int)(MainCmdType.CMD_MAIN_Charm << 8) | (int)(Charm_SubCmd.LC_Sub_TableRoleCharmInfo),
    
    //商店
    CMD_CL_ShopItem                     = (int)(MainCmdType.CMD_MAIN_Shop << 8) | (int)(Shop_SubCmd.CL_Sub_ShopItem),
    CMD_LC_ShopItemResult               = (int)(MainCmdType.CMD_MAIN_Shop << 8) | (int)(Shop_SubCmd.LC_Sub_ShopItemResult),
    //实体
    CMD_CL_LoadUserEntity               = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.CL_Sub_LoadUserEntity),
    CMD_LC_LoadUserEntity               = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.LC_Sub_LoadUserEntity),
   // CMD_CL_ChangeRoleName               = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.CL_Sub_ChangeRoleName),
    CMD_LC_ChangeRoleName               = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.LC_Sub_ChangeRoleName),
    //CMD_CL_ChangeRolePhone              = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.CL_Sub_ChangeRolePhone),
    CMD_LC_ChangeRolePhone              = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.LC_Sub_ChangeRolePhone),
    //CMD_CL_ChangeRoleEmail              = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.CL_Sub_ChangeRoleEmail),
    CMD_LC_ChangeRoleEmail              = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.LC_Sub_ChangeRoleEmail),
    //CMD_CL_ChangeRoleAddress            = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.CL_Sub_ChangeRoleAddress),
    //CMD_LC_ChangeRoleAddress            = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.LC_Sub_ChangeRoleAddress),
   // CMD_CL_ChangeRoleEntityID           = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.CL_Sub_ChangeRoleEntityID),
    CMD_LC_ChangeRoleEntityID           = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.LC_Sub_ChangeRoleEntityID),

    CMD_CL_ChangeRoleEntityItemUseName      = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.CL_Sub_ChangeRoleEntityItemUseName),
    CMD_LC_ChangeRoleEntityItemUseName      = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.LC_Sub_ChangeRoleEntityItemUseName),
    CMD_CL_ChagneRoleEntityItemUsePhone     = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.CL_Sub_ChagneRoleEntityItemUsePhone),
    CMD_LC_ChangeRoleEntityItemUsePhone     = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.LC_Sub_ChangeRoleEntityItemUsePhone),
    CMD_CL_ChangeRoleEntityItemUseAddress   = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.CL_Sub_ChangeRoleEntityItemUseAddress),
    CMD_LC_ChangeRoleEntityItemUseAddress   = (int)(MainCmdType.CMD_MAIN_Entity << 8) | (int)(Entity_SubCmd.LC_Sub_ChangeRoleEntityItemUseAddress),
    //活动
    CMD_CL_GetRoleActionInfo            = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.CL_Sub_GetRoleActionInfo),
    CMD_LC_GetRoleActionInfo            = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.LC_Sub_GetRoleActionInfo),
    //CMD_LC_GetRoleActionInfoFinish      = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.LC_Sub_GetRoleActionInfoFinish),
    //CMD_LC_ActionAllEventFinish         = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.LC_Sub_ActionAllEventFinish),
    CMD_CL_GetActionReward              = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.CL_Sub_GetActionReward),
    CMD_LC_GetActionReward              = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.LC_Sub_GetActionReward),
    CMD_CL_GetOnceActionInfo            = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.CL_Sub_GetOnceActionInfo),
    CMD_LC_GetOnceActionInfo            = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.LC_Sub_GetOnceActionInfo),
    CMD_LC_JoinAction                   = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.LC_Sub_JoinAction),
    CMD_LC_DelAction                    = (int)(MainCmdType.CMD_MAIN_Action << 8) | (int)(Action_SubCmd.LC_Sub_DelAction),
    //Giff
    CMD_CL_GetRoleGiff                  = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.CL_Sub_GetRoleGiff),
    CMD_LC_GetRoleGiff                  = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.LC_Sub_GetRoleGiff),
    //CMD_LC_GetRoleGiffFinish            = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.LC_Sub_GetRoleGiffFinish),
    CMD_CL_AddRoleGiff                  = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.CL_Sub_AddRoleGiff),
    CMD_LC_AddRoleGiffResult            = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.LC_Sub_AddRoleGiffResult),
    CMD_LC_AddBeRoleGiff                = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.LC_Sub_AddBeRoleGiff),
    CMD_CL_GetRoleGiffReward            = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.CL_Sub_GetRoleGiffReward),
    CMD_LC_GetRoleGiffReward            = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.LC_Sub_GetRoleGiffReward),
    CMD_CL_GetAllRoleGiffReward         = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.CL_Sub_GetAllRoleGiffReward),
    CMD_LC_GetAllRoleGiffReward         = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.LC_Sub_GetAllRoleGiffReward),
    CMD_CL_GetRoleNowDaySendGiff        = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.CL_Sub_GetRoleNowDaySendGiff),
    CMD_LC_GetRoleNowDaySendGiff        = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.LC_Sub_GetRoleNowDaySendGiff),
    //CMD_LC_GetRoleNowDaySendGiffFinish  = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.LC_Sub_GetRoleNowDaySendGiffFinish),
    CMD_LC_DelRoleGiff                  = (int)(MainCmdType.CMD_MAIN_Giff << 8) | (int)(Giff_SubCmd.LC_Sub_DelRoleGiff),
    //GlobelShop
    //CMD_CL_GlobelShopItem               = (int)(MainCmdType.CMD_MAIN_GlobelShop << 8) | (int)(GlobelShop_SubCmd.CL_Sub_GlobelShopItem),
    //CMD_LC_GlobelShopItem               = (int)(MainCmdType.CMD_MAIN_GlobelShop << 8) | (int)(GlobelShop_SubCmd.LC_Sub_GlobelShopItem),
    //在线奖励
    CMD_CL_GetOnlineReward              = (int)(MainCmdType.CMD_MAIN_OnlineReward << 8) | (int)(OnlineReward_SubCmd.CL_Sub_GetOnlineReward),
    CMD_LC_GetOnlineReward              = (int)(MainCmdType.CMD_MAIN_OnlineReward << 8) | (int)(OnlineReward_SubCmd.LC_Sub_GetOnlineReward),
    //gamedata
    CMD_CL_GetGameData                  = (int)(MainCmdType.CMD_MAIN_GameData << 8) | (int)(GameData_SubCmd.CL_Sub_GetGameData),
    CMD_LC_GendGameData                 = (int)(MainCmdType.CMD_MAIN_GameData << 8) | (int)(GameData_SubCmd.LC_Sub_GendGameData),
    //package
    //CMD_CL_OpenPackage                  = (int)(MainCmdType.CMD_MAIN_Package << 8) | (int)(Package_SubCmd.CL_Sub_OpenPackage),
    CMD_LC_OpenPackage                  = (int)(MainCmdType.CMD_MAIN_Package << 8) | (int)(Package_SubCmd.LC_Sub_OpenPackage),
    //launcher
    CMD_LC_LauncherData                 = (int)(MainCmdType.CMD_Main_Launcher << 8) | (int)(Launcher_SubCmd.LC_Sub_LauncherData),
    //Message
    CMD_LC_SendMessage                  = (int)(MainCmdType.CMD_Main_Message << 8) | (int)(Message_SubCmd.LC_Sub_SendMessage),
    CMD_CL_SendMessage                  = (int)(MainCmdType.CMD_Main_Message << 8) | (int)(Message_SubCmd.CL_Sub_SendMessage),
    //充值
    CMD_CL_Recharge                     = (int)(MainCmdType.CMD_Main_Recharge << 8) | (int)(Recharge_SubCmd.CL_Sub_Recharge),
    CMD_LC_Recharge                     = (int)(MainCmdType.CMD_Main_Recharge << 8) | (int)(Recharge_SubCmd.LC_Sub_Recharge),
    CMD_LC_GetOrderID                   = (int)(MainCmdType.CMD_Main_Recharge << 8) | (int)(Recharge_SubCmd.LC_Sub_GetOrderID),
    CMD_CL_IOSRecharge                  = (int)(MainCmdType.CMD_Main_Recharge << 8) | (int)(Recharge_SubCmd.CL_Sub_IOSRecharge),

    //Announcement
    CMD_CL_GetAllAnnouncement                           = (int)(MainCmdType.CMD_Main_Announcement << 8) | (int)(Announcement_SubCmd.CL_Sub_GetAllAnnouncement      ),
    CMD_LC_GetAllAnnouncement                           = (int)(MainCmdType.CMD_Main_Announcement << 8) | (int)(Announcement_SubCmd.LC_Sub_GetAllAnnouncement      ),
   // CMD_LC_GetAllAnnouncementFinish                     = (int)(MainCmdType.CMD_Main_Announcement << 8) | (int)(Announcement_SubCmd.LC_Sub_GetAllAnnouncementFinish),
    CMD_LC_SendNewAnnouncementOnce                      = (int)(MainCmdType.CMD_Main_Announcement << 8) | (int)(Announcement_SubCmd.LC_Sub_SendNewAnnouncementOnce ),
    //Operate
   CMD_CL_RealNameVerification                            = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.CL_Sub_RealNameVerification       ),
   CMD_LC_RealNameVerification                           = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.LC_Sub_RealNameVerification      ),
   CMD_CL_GetPhoneVerificationNum                           = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.CL_Sub_GetPhoneVerificationNum      ),
   CMD_LC_GetPhoneVerificationNum                           = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.LC_Sub_GetPhoneVerificationNum      ),
   CMD_CL_BindPhone                           = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.CL_Sub_BindPhone      ),
   CMD_LC_BindPhone                            = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.LC_Sub_BindPhone       ),
   CMD_LC_BuyEntityItem                            = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.LC_Sub_BuyEntityItem       ),
   CMD_CL_BindEmail = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.CL_Sub_BindEmail),
   CMD_LC_BindEmail = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.LC_Sub_BindEmail),
   CMD_LC_PhonePay = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.LC_Sub_PhonePay),
   CMD_LC_AddNormalOrderID = (int)(MainCmdType.CMD_Main_Operate << 8) | (int)(Operate_SubCmd.LC_Sub_AddNormalOrderID),
    
    //RoleMessage
   CMD_LC_RoleMessageChange = (int)(MainCmdType.CMD_Main_RoleMessage << 8) | (int)(RoleMessage_SubCmd.LC_Sub_RoleMessageChange),
    //ExChange
   CMD_CL_RoleUseExChangeCode = (int)(MainCmdType.CMD_Main_Exchange << 8) | (int)(ExChange_SubCmd.CL_Sub_RoleUseExChangeCode),
   CMD_LC_RoleUseExChangeCode = (int)(MainCmdType.CMD_Main_Exchange << 8) | (int)(ExChange_SubCmd.LC_Sub_RoleUseExChangeCode),
    //Lottery
   CMD_CL_GetLotteryReward = (int)(MainCmdType.CMD_Main_Lottery << 8) | (int)(Lottery_SubCmd.CL_Sub_GetLotteryReward),
   CMD_LC_GetLotteryReward = (int)(MainCmdType.CMD_Main_Lottery << 8) | (int)(Lottery_SubCmd.LC_Sub_GetLotteryReward),

   CMD_CL_LotteryUIStates = (int)(MainCmdType.CMD_Main_Lottery << 8) | (int)(Lottery_SubCmd.CL_Sub_LotteryUIStates),
   CMD_LC_LotteryUIStates = (int)(MainCmdType.CMD_Main_Lottery << 8) | (int)(Lottery_SubCmd.LC_Sub_LotteryUIStates),
    
    
     
   CMD_EXTRA_CHECK_VER = (int)(MainCmdType.CMD_EXTRA_DOWN << 8) | (int)(EXTRA_DOWN.EXTRA_DOWN_CHECK_VER),
   CMD_EXTRA_FILE = (int)(MainCmdType.CMD_EXTRA_DOWN << 8) | (int)(EXTRA_DOWN.EXTRA_DOWN_FILE),
   CMD_EXTRA_FILE_EX = (int)(MainCmdType.CMD_EXTRA_DOWN << 8) | (int)(EXTRA_DOWN.EXTRA_DOWN_FILE_EX),

    //MiniGame
   //CMD_CL_JoinRoom      = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.CL_Sub_JoinRoom),
   //CMD_LC_JoinRoom      = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.LC_Sub_JoinRoom),
   //CMD_LC_TableUpdate   = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.LC_Sub_TableUpdate),
   //CMD_CL_AddGlobel     = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.CL_Sub_AddGlobel),
   //CMD_LC_AddGlobel     = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.LC_Sub_AddGlobel),
   //CMD_LC_TableResult   = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.LC_Sub_TableResult),
   //CMD_LC_TableBegin    = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.LC_Sub_TableBegin),
   //CMD_CL_LeaveNiuNiuTable = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.CL_Sub_LeaveNiuNiuTable),

   //CMD_LC_BankerChange              = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.LC_Sub_BankerChange),
   //CMD_LC_WirteBankerListChange     = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.LC_Sub_WirteBankerListChange),
   //CMD_CL_AddBankerList             = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.CL_Sub_AddBankerList),
   //CMD_LC_AddBankerList             = (int)(MainCmdType.CMD_Main_MiniGame << 8) | (int)(MiniGame_SubCmd.LC_Sub_AddBankerList),

    //NiuNiu
   CMD_CL_RoleJoinTable         = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_RoleJoinTable),
   CMD_LC_RoleJoinTable         = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_RoleJoinTable),
   CMD_CL_RoleLeaveTable        = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_RoleLeaveTable),
   CMD_CL_RoleBetGlobel         = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_RoleBetGlobel),
   CMD_LC_RoleBetGlobel         = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_RoleBetGlobel),
   CMD_CL_GetBankerList         = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_GetBankerList),
   CMD_LC_GetBankerList         = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_GetBankerList),
   CMD_LC_BankerListChange      = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_BankerListChange),
   CMD_LC_BankerUserChange      = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_BankerUserChange),
   CMD_LC_TableJoinBegin        = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_TableJoinBegin),
   CMD_LC_TableUpdate           = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_TableUpdate),
   CMD_LC_TableJoinEnd          = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_TableJoinEnd),
   CMD_CL_RoleJoinBankerList    = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_RoleJoinBankerList),
   CMD_LC_RoleJoinBankerList    = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_RoleJoinBankerList),
   CMD_CL_RoleGetBankerFirstSeat= (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_RoleGetBankerFirstSeat),
   CMD_LC_RoleGetBankerFirstSeat= (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_RoleGetBankerFirstSeat),
   CMD_CL_RoleLeaveBankerList   = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_RoleLeaveBankerList),
   CMD_LC_RoleLeaveBankerList   = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_RoleLeaveBankerList),
   CMD_CL_RoleCanelBankerSeat   = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_RoleCanelBankerSeat),
   CMD_LC_RoleCanelBankerSeat   = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_RoleCanelBankerSeat),
   CMD_LC_BankerUserGlobelChange= (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_BankerUserGlobelChange),
   CMD_LC_WriteBankerUserGlobelMsg = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_WriteBankerUserGlobelMsg),

   CMD_CL_RoleJoinVipSeat       = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_RoleJoinVipSeat),
   CMD_LC_RoleJoinVipSeat       = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_RoleJoinVipSeat),
   CMD_CL_RoleLeaveVipSeat      = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_RoleLeaveVipSeat),
   CMD_LC_RoleLeaveVipSeat      = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_RoleLeaveVipSeat),
   CMD_LC_RoleBeLeaveVipSeat    = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_RoleBeLeaveVipSeat),

    CMD_CL_GetNormalSeatInfo    = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.CL_Sub_GetNormalSeatInfo),
    CMD_LC_GetNormalSeatInfo    = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_GetNormalSeatInfo),
    CMD_LC_VipSeatGlobelChange  = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_VipSeatGlobelChange),
    CMD_LC_TableStopUpdate      = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_TableStopUpdate),
    CMD_LC_VipGlobelChange      = (int)(MainCmdType.CMD_Main_NiuNiu << 8) | (int)(NiuNiu_SubCmd.LC_Sub_VipGlobelChange),

    
    
    //char
    CMD_CL_LoadAllCharInfo          = (int)(MainCmdType.CMD_Main_Char << 8) | (int)(Char_SubCmd.CL_Sub_LoadAllCharInfo),
    CMD_LC_LoadAllCharInfo          = (int)(MainCmdType.CMD_Main_Char << 8) | (int)(Char_SubCmd.LC_Sub_LoadAllCharInfo),
    CMD_CL_LoadCharListByUserID     = (int)(MainCmdType.CMD_Main_Char << 8) | (int)(Char_SubCmd.CL_Sub_LoadCharListByUserID),
    CMD_LC_LoadCharListByUserID     = (int)(MainCmdType.CMD_Main_Char << 8) | (int)(Char_SubCmd.LC_Sub_LoadCharListByUserID),
    CMD_CL_SendCharInfo             = (int)(MainCmdType.CMD_Main_Char << 8) | (int)(Char_SubCmd.CL_Sub_SendCharInfo),
    CMD_LC_SendCharInfo             = (int)(MainCmdType.CMD_Main_Char << 8) | (int)(Char_SubCmd.LC_Sub_SendCharInfo),


    
    //
    CMD_CL_LoadRelationRequest               = (int)(MainCmdType.CMD_Main_RelationRequest << 8) | (int)(RelationRequest_SubCmd.CL_Sub_LoadRelationRequest  ),
    CMD_LC_LoadRelationRequest               = (int)(MainCmdType.CMD_Main_RelationRequest << 8) | (int)(RelationRequest_SubCmd.LC_Sub_LoadRelationRequest  ),
    CMD_CL_SendRelationRequest               = (int)(MainCmdType.CMD_Main_RelationRequest << 8) | (int)(RelationRequest_SubCmd.CL_Sub_SendRelationRequest  ),
    CMD_LC_SendRelationRequest               = (int)(MainCmdType.CMD_Main_RelationRequest << 8) | (int)(RelationRequest_SubCmd.LC_Sub_SendRelationRequest  ),
    CMD_LC_BeAddRelationRequest              = (int)(MainCmdType.CMD_Main_RelationRequest << 8) | (int)(RelationRequest_SubCmd.LC_Sub_BeAddRelationRequest ),
    CMD_CL_HandleRelationRequest             = (int)(MainCmdType.CMD_Main_RelationRequest << 8) | (int)(RelationRequest_SubCmd.CL_Sub_HandleRelationRequest),
    CMD_LC_HandleRelationRequest             = (int)(MainCmdType.CMD_Main_RelationRequest << 8) | (int)(RelationRequest_SubCmd.LC_Sub_HandleRelationRequest),
    CMD_LC_DelRelationRequest                = (int)(MainCmdType.CMD_Main_RelationRequest << 8) | (int)(RelationRequest_SubCmd.LC_Sub_DelRelationRequest),

    //Dial
    CMD_CL_DialRoleJoinTable = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleJoinTable),
    CMD_LC_DialRoleJoinTable = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleJoinTable),
    CMD_CL_DialRoleLeaveTable = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleLeaveTable),
    CMD_CL_DialRoleBetGlobel = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleBetGlobel),
    CMD_LC_DialRoleBetGlobel = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleBetGlobel),
    CMD_CL_DialGetBankerList = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialGetBankerList),
    CMD_LC_DialGetBankerList = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialGetBankerList),
    CMD_LC_DialBankerListChange = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialBankerListChange),
    CMD_LC_DialBankerUserChange = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialBankerUserChange),
    CMD_LC_DialTableJoinBegin = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialTableJoinBegin),
    CMD_LC_DialTableUpdate = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialTableUpdate),
    CMD_LC_DialTableJoinEnd = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialTableJoinEnd),
    CMD_CL_DialRoleJoinBankerList = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleJoinBankerList),
    CMD_LC_DialRoleJoinBankerList = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleJoinBankerList),
    CMD_CL_DialRoleGetBankerFirstSeat = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleGetBankerFirstSeat),
    CMD_LC_DialRoleGetBankerFirstSeat = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleGetBankerFirstSeat),
    CMD_CL_DialRoleLeaveBankerList = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleLeaveBankerList),
    CMD_LC_DialRoleLeaveBankerList = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleLeaveBankerList),
    CMD_CL_DialRoleCanelBankerSeat = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleCanelBankerSeat),
    CMD_LC_DialRoleCanelBankerSeat = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleCanelBankerSeat),
    CMD_LC_DialBankerUserGlobelChange = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialBankerUserGlobelChange),
    CMD_LC_DialWriteBankerUserGlobelMsg = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialWriteBankerUserGlobelMsg),

    CMD_CL_DialRoleJoinVipSeat = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleJoinVipSeat),
    CMD_LC_DialRoleJoinVipSeat = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleJoinVipSeat),
    CMD_CL_DialRoleLeaveVipSeat = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleLeaveVipSeat),
    CMD_LC_DialRoleLeaveVipSeat = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleLeaveVipSeat),
    CMD_LC_DialRoleBeLeaveVipSeat = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleBeLeaveVipSeat),

    CMD_CL_DialGetNormalSeatInfo = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialGetNormalSeatInfo),
    CMD_LC_DialGetNormalSeatInfo = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialGetNormalSeatInfo),
    CMD_LC_DialVipSeatGlobelChange = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialVipSeatGlobelChange),
    CMD_LC_DialTableStopUpdate = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialTableStopUpdate),
    CMD_LC_DialVipGlobelChange = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialVipGlobelChange),

    CMD_CL_DialRoleBetGlobelByLog = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.CL_Sub_DialRoleBetGlobelByLog),
    CMD_LC_DialRoleBetGlobelByLog = (int)(MainCmdType.CMD_Main_Dial << 8) | (int)(Dial_SubCmd.LC_Sub_DialRoleBetGlobelByLog),


    //Car
    CMD_CL_CarRoleJoinTable = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleJoinTable),
    CMD_LC_CarRoleJoinTable = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleJoinTable),
    CMD_CL_CarRoleLeaveTable = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleLeaveTable),
    CMD_CL_CarRoleBetGlobel = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleBetGlobel),
    CMD_LC_CarRoleBetGlobel = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleBetGlobel),
    CMD_CL_CarGetBankerList = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarGetBankerList),
    CMD_LC_CarGetBankerList = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarGetBankerList),
    CMD_LC_CarBankerListChange = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarBankerListChange),
    CMD_LC_CarBankerUserChange = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarBankerUserChange),
    CMD_LC_CarTableJoinBegin = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarTableJoinBegin),
    CMD_LC_CarTableUpdate = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarTableUpdate),
    CMD_LC_CarTableJoinEnd = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarTableJoinEnd),
    CMD_CL_CarRoleJoinBankerList = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleJoinBankerList),
    CMD_LC_CarRoleJoinBankerList = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleJoinBankerList),
    CMD_CL_CarRoleGetBankerFirstSeat = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleGetBankerFirstSeat),
    CMD_LC_CarRoleGetBankerFirstSeat = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleGetBankerFirstSeat),
    CMD_CL_CarRoleLeaveBankerList = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleLeaveBankerList),
    CMD_LC_CarRoleLeaveBankerList = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleLeaveBankerList),
    CMD_CL_CarRoleCanelBankerSeat = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleCanelBankerSeat),
    CMD_LC_CarRoleCanelBankerSeat = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleCanelBankerSeat),
    CMD_LC_CarBankerUserGlobelChange = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarBankerUserGlobelChange),
    CMD_LC_CarWriteBankerUserGlobelMsg = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarWriteBankerUserGlobelMsg),

    CMD_CL_CarRoleJoinVipSeat = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleJoinVipSeat),
    CMD_LC_CarRoleJoinVipSeat = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleJoinVipSeat),
    CMD_CL_CarRoleLeaveVipSeat = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleLeaveVipSeat),
    CMD_LC_CarRoleLeaveVipSeat = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleLeaveVipSeat),
    CMD_LC_CarRoleBeLeaveVipSeat = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleBeLeaveVipSeat),

    CMD_CL_CarGetNormalSeatInfo = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarGetNormalSeatInfo),
    CMD_LC_CarGetNormalSeatInfo = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarGetNormalSeatInfo),
    CMD_LC_CarVipSeatGlobelChange = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarVipSeatGlobelChange),
    CMD_LC_CarTableStopUpdate = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarTableStopUpdate),
    CMD_LC_CarVipGlobelChange = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarVipGlobelChange),

    CMD_CL_CarRoleBetGlobelByLog = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.CL_Sub_CarRoleBetGlobelByLog),
    CMD_LC_CarRoleBetGlobelByLog = (int)(MainCmdType.CMD_Main_Car << 8) | (int)(Car_SubCmd.LC_Sub_CarRoleBetGlobelByLog),
    

    CMD_MAX
}                                      

//====================================================
//           网络命令ID和结构体映射
//====================================================
public class NetCmdMapping
{
    public static Dictionary<NetCmdType, NetTypeInfo> ms_TypeMapList = new Dictionary<NetCmdType,NetTypeInfo>();
    static public int GetMapCount()
    {
        return ms_TypeMapList.Count;
    }
    static public bool IsRegisterCmd(NetCmdType cmdType)
    {
        return ms_TypeMapList.ContainsKey(cmdType);
    }
    static public NetTypeInfo GetTypeInfo(NetCmdType cmdType)
    {
        NetTypeInfo info ;
        if(ms_TypeMapList.TryGetValue(cmdType, out info))
            return info;
        else
            return null;
    }
    static public bool InitCmdTypeInfo<T>(NetCmdType cmdType) where T : NetCmdBase, new()
    {
        if(ms_TypeMapList.ContainsKey(cmdType))
            return false;
        TypeMapping.RegisterClassFromType<T>();
        NetTypeInfo nti = NetTypeCreater<T>.GetTypeInfo(cmdType);
        ms_TypeMapList.Add(cmdType, nti);
        return true;
    }
    static public bool GlobalInit()
    {
        bool bRet = TypeMapping.GlobalInit();
        
        //注册添加网络命令
        //----------------------------------------------------
        bRet &= InitCmdTypeInfo<NetCmd>(NetCmdType.CMD_HEARTBEAT);

        bRet &= InitCmdTypeInfo<NetCmdFastLogon>(NetCmdType.CMD_FAST_LOGON);
        bRet &= InitCmdTypeInfo<NetCmdRegisterLogon>(NetCmdType.CMD_REGISTER_LOGON);
        bRet &= InitCmdTypeInfo<NetCmdNormalLogon>(NetCmdType.CMD_NORMAL_LOGON);
        bRet &= InitCmdTypeInfo<NetCmdLogonResult>(NetCmdType.CMD_LOGON_RESULT);
        bRet &= InitCmdTypeInfo<NetCmdLogonResultSimple>(NetCmdType.CMD_LOGON_RESULT_SIMPLE);

        bRet &= InitCmdTypeInfo<NetCmdLogonHall>(NetCmdType.CMD_LOGON_HALL);
        bRet &= InitCmdTypeInfo<NetCmdLogonHallResult>(NetCmdType.CMD_LOGON_HALL_RESULT);

        bRet &= InitCmdTypeInfo<NetCmdJoinRoom>(NetCmdType.CMD_JOIN_ROOM);
        bRet &= InitCmdTypeInfo<NetCmdJoinRoomResult>(NetCmdType.CMD_JOIN_ROOM_RESULT);

        bRet &= InitCmdTypeInfo<NetCmd>(NetCmdType.CMD_PLAYER_READY);

        bRet &= InitCmdTypeInfo<NetCmdBullet>(NetCmdType.CMD_BULLET);
        bRet &= InitCmdTypeInfo<NetCmdFish>(NetCmdType.CMD_FISH);
        bRet &= InitCmdTypeInfo<NetCmdCatched>(NetCmdType.CMD_CATCHED);
        bRet &= InitCmdTypeInfo<NetCmdPlayerJoin>(NetCmdType.CMD_PLAYER_JOIN);
        bRet &= InitCmdTypeInfo<NetCmdPlayerLeave>(NetCmdType.CMD_PLAYER_LEAVE);
        bRet &= InitCmdTypeInfo<NetCmdChangeLauncher>(NetCmdType.CMD_CHANGE_LAUNCHER);
        bRet &= InitCmdTypeInfo<NetCmdChangeLauncherType>(NetCmdType.CMD_CHANGE_LAUNCHER_TYPE);
        bRet &= InitCmdTypeInfo<NetCmdClientChangeRate>(NetCmdType.CMD_CHANGE_CLIENT_RATE);
        bRet &= InitCmdTypeInfo<NetCmdChangeRate>(NetCmdType.CMD_CHANGE_RATE);
        bRet &= InitCmdTypeInfo<NetCmdChangeRateType>(NetCmdType.CMD_CHANGE_RATE_TYPE);
        bRet &= InitCmdTypeInfo<NetCmdSyncFish>(NetCmdType.CMD_SYNC_FISH);
        bRet &= InitCmdTypeInfo<NetCmdRoomInfo>(NetCmdType.CMD_ROOM_INFO);
        bRet &= InitCmdTypeInfo<NetCmdSyncBullet>(NetCmdType.CMD_SYNC_BULLET);
        bRet &= InitCmdTypeInfo<NetCmd>(NetCmdType.CMD_PING);
        bRet &= InitCmdTypeInfo<NetCmdReduction>(NetCmdType.CMD_REDUCTION);

        bRet &= InitCmdTypeInfo<NetCmdUseSkill>(NetCmdType.CMD_USE_SKILL);

        bRet &= InitCmdTypeInfo<NetCmd>(NetCmdType.CMD_SKILL_TORNADO_REQUEST);
        bRet &= InitCmdTypeInfo<NetCmdSkillTornado>(NetCmdType.CMD_SKILL_TORNADO_RESPONSE);
        bRet &= InitCmdTypeInfo<NetCmd>(NetCmdType.CMD_SKILL_FREEZE_REQUEST);
        bRet &= InitCmdTypeInfo<NetCmdSkillFreeze>(NetCmdType.CMD_SKILL_FREEZE_RESPONSE);
        bRet &= InitCmdTypeInfo<NetCmd>(NetCmdType.CMD_SKILL_LIGHTING_REQUEST);
        bRet &= InitCmdTypeInfo<NetCmdSkillLighting>(NetCmdType.CMD_SKILL_LIGHTING_RESPONSE);
        bRet &= InitCmdTypeInfo<NetCmdSkillLaser>(NetCmdType.CMD_SKILL_LASER_REQUEST);
        bRet &= InitCmdTypeInfo<NetCmdSkillLaserResponse>(NetCmdType.CMD_SKILL_LASER_RESPONSE);
        bRet &= InitCmdTypeInfo<NetCmd>(NetCmdType.CMD_SKILL_DISASTER_REQUEST);
        bRet &= InitCmdTypeInfo<NetCmdSkillDisaster>(NetCmdType.CMD_SKILL_DISASTER_RESPONSE);

        bRet &= InitCmdTypeInfo<NetCmd>(NetCmdType.CMD_SKILL_LOCK_REQUEST);
        bRet &= InitCmdTypeInfo<NetCmdSkillLock>(NetCmdType.CMD_SKILL_LOCK_RESPONSE);

        bRet &= InitCmdTypeInfo<NetCmdBulletReduction>(NetCmdType.CMD_BULLET_REDUCTION);

        //图片上传
        bRet &= InitCmdTypeInfo<NetCmdUploadImgRequest>(NetCmdType.CMD_UPLOADING_REQUEST);
        bRet &= InitCmdTypeInfo<NetCmdUploadImgResponse>(NetCmdType.CMD_UPLOADING_RESPONSE);
        bRet &= InitCmdTypeInfo<NetCmdUploadImgChunk>(NetCmdType.CMD_UPLOADING_CHUNK);
        bRet &= InitCmdTypeInfo<NetCmdUploadImgChunkResponse>(NetCmdType.CMD_UPLOADING_CHUNK_RESPONSE);
        bRet &= InitCmdTypeInfo<NetCmdUploadImgComplete>(NetCmdType.CMD_UPLOADING_COMPLETE);

        bRet &= InitCmdTypeInfo<NetCmdImgChange>(NetCmdType.CMD_CHANGE_IMG);
        bRet &= InitCmdTypeInfo<NetCmdFishPackage>(NetCmdType.CMD_FISH_PACKAGE);

        bRet &= InitCmdTypeInfo<NetCmdCheckFishPos>(NetCmdType.CMD_CHECK_FISH_POS);
        bRet &= InitCmdTypeInfo<NetCmdCheckBulletPos>(NetCmdType.CMD_CHECK_BULLET_POS);
        bRet &= InitCmdTypeInfo<NetCmdClearScene>(NetCmdType.CMD_CLEAR_SCENE);
        bRet &= InitCmdTypeInfo<NetCmdLaunchFailed>(NetCmdType.CMD_LAUNCH_LASER_FAILED);
        bRet &= InitCmdTypeInfo<NetCmdSkillFailed>(NetCmdType.CMD_SKILL_FAILLED);

        //----------------------------------------------------
        //登陆
        //bRet &= InitCmdTypeInfo<NetCmdLogonAccount>                 (NetCmdType.CMD_CL_Account_Logon            );
        //bRet &= InitCmdTypeInfo<NetCmdLogonRsgAccount>              (NetCmdType.CMD_CL_Account_Rsg              );
        //bRet &= InitCmdTypeInfo<NetCmdLogonQuery>                   (NetCmdType.CMD_CL_Account_Query            );
        //bRet &= InitCmdTypeInfo<NetCmdLogonAccountResult>           (NetCmdType.CMD_LC_Account_LogonSucess      );
        //bRet &= InitCmdTypeInfo<NetCmdLogonAccountError>            (NetCmdType.CMD_LC_Account_LogonError       );
        //bRet &= InitCmdTypeInfo<NetCmdLogonRsgAccountError>         (NetCmdType.CMD_LC_Account_RsgError         );
        //bRet &= InitCmdTypeInfo<NetCmdLogonQueryError>              (NetCmdType.CMD_LC_Account_QueryError       );
        //bRet &= InitCmdTypeInfo<NetCmdLogonVersionError>            (NetCmdType.CMD_LC_Account_VersionError     );
        //bRet &= InitCmdTypeInfo<LC_Cmd_QueryAccount>                (NetCmdType.CMD_LC_QueryAccount             );
        //bRet &= InitCmdTypeInfo<CL_Cmd_QueryRsg>                    (NetCmdType.CMD_CL_QueryRsg                 );
        //bRet &= InitCmdTypeInfo<LC_Cmd_QueryRsgError>               (NetCmdType.CMD_LC_QueryRsgError            );
        //bRet &= InitCmdTypeInfo<LC_Cmd_DayChange>                   (NetCmdType.CMD_LC_DayChange                );
        ////桌子
        bRet &= InitCmdTypeInfo<CL_Cmd_JoinTable>(NetCmdType.CMD_CL_JoinTable);
        bRet &= InitCmdTypeInfo<LC_Cmd_JoinTableResult>(NetCmdType.CMD_LC_JoinTable);
        bRet &= InitCmdTypeInfo<CL_Cmd_LeaveTable>(NetCmdType.CMD_CL_LeaveTable);
        bRet &= InitCmdTypeInfo<CL_Cmd_AccountLogon>(NetCmdType.CMD_CL_AccountLogon);
        bRet &= InitCmdTypeInfo<CL_Cmd_AccountRsg>(NetCmdType.CMD_CL_AccountRsg);
        //bRet &= InitCmdTypeInfo<CL_Cmd_AccountQueryRsg>(NetCmdType.CMD_CL_AccountQueryRsg);
        bRet &= InitCmdTypeInfo<LC_Cmd_AccountOnlyID>(NetCmdType.CMD_LC_AccountOnlyID);

        bRet &= InitCmdTypeInfo<CL_Cmd_AccountOnlyID>(NetCmdType.CMD_CL_AccountOnlyID);
        bRet &= InitCmdTypeInfo<LC_Cmd_AccountOnlyIDFailed>(NetCmdType.CMD_LC_AccountOnlyIDFailed);
        bRet &= InitCmdTypeInfo<LC_Cmd_AccountOnlyIDSuccess>(NetCmdType.CMD_LC_AccountOnlyIDSuccess);
        bRet &= InitCmdTypeInfo<CL_Cmd_ResetLogonGameServer>(NetCmdType.CMD_CL_ResetLogonGameServer);
        bRet &= InitCmdTypeInfo<LC_Cmd_ResetLogonGameServer>(NetCmdType.CMD_LC_ResetLogonGameServer);
        bRet &= InitCmdTypeInfo<LC_Cmd_AccountLogonToScreen>(NetCmdType.CMD_LC_AccountLogonToScreen);
        bRet &= InitCmdTypeInfo<LC_Cmd_ServerChangeSocket>(NetCmdType.CMD_LC_ServerChangeSocket);
        bRet &= InitCmdTypeInfo<CL_Cmd_ResetPassword>(NetCmdType.CMD_CL_ResetPassword);
        bRet &= InitCmdTypeInfo<LC_Cmd_ResetPassword>(NetCmdType.CMD_LC_ResetPassword);
        bRet &= InitCmdTypeInfo<CL_Cmd_AccountResetAccount>(NetCmdType.CMD_CL_AccountResetAccount);
        bRet &= InitCmdTypeInfo<LC_Cmd_AccountResetAccount>(NetCmdType.CMD_LC_AccountResetAccount);
        bRet &= InitCmdTypeInfo<LC_Cmd_LogonQueueWrite>(NetCmdType.CMD_LC_LogonQueueWrite);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetNewAccount>(NetCmdType.CMD_CL_GetNewAccount);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetNewAccount>(NetCmdType.CMD_LC_GetNewAccount);
        bRet &= InitCmdTypeInfo<LC_Cmd_CheckVersionError>(NetCmdType.CMD_LC_CheckVersionError);
        bRet &= InitCmdTypeInfo<LC_Cmd_AccountIsFreeze>(NetCmdType.CMD_LC_AccountIsFreeze);
        
        bRet &= InitCmdTypeInfo<CL_Cmd_ChannelLogon>(NetCmdType.CMD_CL_ChannelLogon);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChannelLogon>(NetCmdType.CMD_LC_ChannelLogon);

        bRet &= InitCmdTypeInfo<CL_Cmd_PhoneLogon>(NetCmdType.CMD_CL_PhoneLogon);
        bRet &= InitCmdTypeInfo<LC_Cmd_PhoneLogon>(NetCmdType.CMD_LC_PhoneLogon);
        bRet &= InitCmdTypeInfo<CL_Cmd_PhoneLogonCheck>(NetCmdType.CMD_CL_PhoneLogonCheck);
        bRet &= InitCmdTypeInfo<LC_Cmd_PhoneLogonCheck>(NetCmdType.CMD_LC_PhoneLogonCheck);
        bRet &= InitCmdTypeInfo<LC_Cmd_SaveAccountInfo>(NetCmdType.CMD_LC_SaveAccountInfo);
        bRet &= InitCmdTypeInfo<CL_Cmd_PhoneSecPwd>(NetCmdType.CMD_CL_PhoneSecPwd);
        bRet &= InitCmdTypeInfo<CL_Cmd_WeiXinLogon>(NetCmdType.CMD_CL_WeiXinLogon);
        bRet &= InitCmdTypeInfo<LC_Cmd_WeiXinLogon>(NetCmdType.CMD_LC_WeiXinLogon);
        bRet &= InitCmdTypeInfo<CL_Cmd_QQLogon>(NetCmdType.CMD_CL_QQLogon);
        bRet &= InitCmdTypeInfo<LC_Cmd_QQLogon>(NetCmdType.CMD_LC_QQLogon);
        bRet &= InitCmdTypeInfo<LC_Cmd_RsgNewAccount>(NetCmdType.CMD_LC_RsgNewAccount);
        
        
        //玩家
        bRet &= InitCmdTypeInfo<LC_Cmd_OtherUserInfo>               (NetCmdType.CMD_LC_OtherUserInfo            );
        bRet &= InitCmdTypeInfo<LC_Cmd_OtherUserLeave>              (NetCmdType.CMD_LC_UserLeave                );
        bRet &= InitCmdTypeInfo<LC_Cmd_MeSeatID>                    (NetCmdType.CMD_LC_MeSeatID                 );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleLevel>        (NetCmdType.CMD_LC_TableChangeRoleLevel     );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleGender>       (NetCmdType.CMD_LC_TableChangeRoleGender    );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleNickName>     (NetCmdType.CMD_LC_TableChangeRoleNickName  );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleFaceID>       (NetCmdType.CMD_LC_TableChangeRoleFaceID    );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleGlobe>        (NetCmdType.CMD_LC_TableChangeRoleGlobe     );
        //bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleExp>          (NetCmdType.CMD_LC_TableChangeRoleExp       );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleTitle>        (NetCmdType.CMD_LC_TableChangeRoleTitle     );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleAchievementPoint>(NetCmdType.CMD_LC_TableChangeRoleAchievementPoint);
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleCharmValue>   (NetCmdType.CMD_LC_TableChangeRoleCharmValue);
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleIpAddress>(NetCmdType.CMD_LC_TableChangeRoleIpAddress);
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleVipLevel>(NetCmdType.CMD_LC_TableChangeRoleVipLevel);
        bRet &= InitCmdTypeInfo<LC_Cmd_TableChangeRoleIsInMonthCard>(NetCmdType.CMD_LC_TableChangeRoleIsInMonthCard);
             
        

        bRet &= InitCmdTypeInfo<LC_Cmd_ResetOtherUserInfo>          (NetCmdType.CMD_LC_ResetOtherUserInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_LeaveTableByServer>          (NetCmdType.CMD_LC_LeaveTableByServer);
       
        
        //物品
        bRet &= InitCmdTypeInfo<CL_Cmd_GetUserItem>(NetCmdType.CMD_CL_GetUserItem);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetUserItem>(NetCmdType.CMD_LC_GetUserItem);
       // bRet &= InitCmdTypeInfo<LC_Cmd_GetUserItemFinish>(NetCmdType.CMD_LC_GetUserItemFinish);
        bRet &= InitCmdTypeInfo<LC_Cmd_AddUserItem>(NetCmdType.CMD_LC_AddUserItem);
        bRet &= InitCmdTypeInfo<LC_Cmd_DelUserItem>(NetCmdType.CMD_LC_DelUserItem);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeUserItem>(NetCmdType.CMD_LC_ChangeUserItem);
        bRet &= InitCmdTypeInfo<CL_Cmd_OnUseItem>(NetCmdType.CMD_CL_OnUseItem);
        bRet &= InitCmdTypeInfo<LC_Cmd_OnUseItem>(NetCmdType.CMD_LC_OnUseItem);

        bRet &= InitCmdTypeInfo<CL_Cmd_OnAcceptItem>(NetCmdType.CMD_CL_OnAcceptItem);
        bRet &= InitCmdTypeInfo<LC_Cmd_OnAcceptItem>(NetCmdType.CMD_LC_OnAcceptItem);
        
        
        
        //关系
        bRet &= InitCmdTypeInfo<CL_Cmd_GetUserRelation>(NetCmdType.CMD_CL_GetUserRelation);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetUserRelation>(NetCmdType.CMD_LC_GetUserRelation);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetUserRelationFinish>(NetCmdType.CMD_LC_GetUserRelationFinish);
        bRet &= InitCmdTypeInfo<CL_Cmd_AddUserRelation>(NetCmdType.CMD_CL_AddUserRelation);
        bRet &= InitCmdTypeInfo<LC_Cmd_AddUserRelation>(NetCmdType.CMD_LC_AddUserRelation);
        bRet &= InitCmdTypeInfo<CL_Cmd_DelUserRelation>(NetCmdType.CMD_CL_DelUserRelation);
        bRet &= InitCmdTypeInfo<LC_Cmd_DelUserRelation>(NetCmdType.CMD_LC_DelUserRelation);
       // bRet &= InitCmdTypeInfo<CL_Cmd_ChangeUserRelation>(NetCmdType.CMD_CL_ChangeUserRelation);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeUserRelation>(NetCmdType.CMD_LC_ChangeUserRelation);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeUserOline>(NetCmdType.CMD_LC_RoleChangeOnline);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationLevel>(NetCmdType.CMD_LC_ChangeRelationLevel);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationFaceID>(NetCmdType.CMD_LC_ChangeRelationFaceID);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationNickName>(NetCmdType.CMD_LC_ChangeRelationNickName);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationGender>(NetCmdType.CMD_LC_ChangeRelationGender);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationTitle>(NetCmdType.CMD_LC_ChangeRelationTitle);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationAchievementPoint>(NetCmdType.CMD_LC_ChangeRelationAchievementPoint);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationCharmValue>(NetCmdType.CMD_LC_ChangeRelationCharmValue);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationIP>(NetCmdType.CMD_LC_ChangeRelationIP);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationVipLevel>(NetCmdType.CMD_LC_ChangeRelationVipLevel);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRelationIsInMonthCard>(NetCmdType.CMD_LC_ChangeRelationIsInMonthCard);

        //邮件
        bRet &= InitCmdTypeInfo<CL_Cmd_GetUserMail>(NetCmdType.CMD_CL_GetUserMail);
        bRet &= InitCmdTypeInfo<LC_Cmd_SystemMail>(NetCmdType.CMD_LC_GetUserSystemMail);
        bRet &= InitCmdTypeInfo<LC_Cmd_NormalMail>(NetCmdType.CMD_LC_GetUserNormalMail);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetMailFinish>(NetCmdType.CMD_LC_GetUserMailFinish);
        //bRet &= InitCmdTypeInfo<CL_Cmd_SendNormalMail>(NetCmdType.CMD_CL_SendUserMail);
        //bRet &= InitCmdTypeInfo<LC_Cmd_SendNormalMail>(NetCmdType.CMD_LC_SendUserMail);
        bRet &= InitCmdTypeInfo<CL_Cmd_DelMail>(NetCmdType.CMD_CL_DelUserMail);
        bRet &= InitCmdTypeInfo<LC_Cmd_DelMail>(NetCmdType.CMD_LC_DelUserMail);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetMailItem>(NetCmdType.CMD_CL_GetMailItem);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetMailItem>(NetCmdType.CMD_LC_GetMailItem);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetMailContext>(NetCmdType.CMD_CL_ReadMail);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetNormalMail>(NetCmdType.CMD_LC_ReadNormalMail);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetSystemMail>(NetCmdType.CMD_LC_ReadSystemMail);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetMailError>(NetCmdType.CMD_LC_ReadMailError);
        bRet &= InitCmdTypeInfo<LC_Cmd_AddNormalMail>(NetCmdType.CMD_LC_AddNormalTitle);
        bRet &= InitCmdTypeInfo<LC_Cmd_AddSystemMail>(NetCmdType.CMD_LC_AddSystemTitle);
        bRet &= InitCmdTypeInfo<CL_Cmd_SendUserMail>(NetCmdType.CMD_CL_SendUserMail);
        bRet &= InitCmdTypeInfo<LC_Cmd_SendUserMailResult>(NetCmdType.CMD_LC_SendUserMailResult);
              
        
        //Role
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangRoleExp>(NetCmdType.CMD_LC_ChangeRoleExp);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleLevel>(NetCmdType.CMD_LC_ChangeRoleLevel);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleGender>(NetCmdType.CMD_LC_ChangeRoleGender);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleNickName>(NetCmdType.CMD_LC_ChangeRoleNickName);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleFaceID>(NetCmdType.CMD_LC_ChangeRoleFaceID);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleGlobe>(NetCmdType.CMD_LC_ChangeRoleGlobe);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleMedal>(NetCmdType.CMD_LC_ChangeRoleMedal);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleCurrency>(NetCmdType.CMD_LC_ChangeRoleCurrency);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleTitle>(NetCmdType.CMD_LC_ChangeRoleTitle);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleAchievementPoint>(NetCmdType.CMD_LC_ChangeRoleAchievementPoint);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleCharmValue>(NetCmdType.CMD_LC_ChangeRoleCharmValue);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleSendGiffSum>(NetCmdType.CMD_LC_ChangeRoleSendGiffSum);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleAcceptGiffSum>(NetCmdType.CMD_LC_ChangeRoleAcceptGiffSum);
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleGender>(NetCmdType.CMD_CL_ChangeRoleGender);
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleNickName>(NetCmdType.CMD_CL_ChangeRoleNickName);
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleNormalFaceID>(NetCmdType.CMD_CL_ChangeRoleNormalFaceID);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleTaskStates>(NetCmdType.CMD_LC_ChangeRoleTaskStates);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleAchievementStates>(NetCmdType.CMD_LC_ChangeRoleAchievementStates);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleActionStates>(NetCmdType.CMD_LC_ChangeRoleActionStates);
        //bRet &= InitCmdTypeInfo<CL_Cmd_RequestUserFaceID>(NetCmdType.CMD_CL_RequestUserFaceID);
        //bRet &= InitCmdTypeInfo<LC_Cmd_ResponseUserFaceID>(NetCmdType.CMD_LC_ResponseUserFaceID);
        //bRet &= InitCmdTypeInfo<CL_Cmd_UpLoadingchunk>(NetCmdType.CMD_CL_UpLoadingchunk);
        //bRet &= InitCmdTypeInfo<LC_Cmd_UpLoadingResponse>(NetCmdType.CMD_LC_UpLoadingResponse);
        //bRet &= InitCmdTypeInfo<LC_Cmd_UpLoadingFinish>(NetCmdType.CMD_LC_UpLoadingFinish);
        //bRet &= InitCmdTypeInfo<LC_Cmd_UpLoadFtpError>(NetCmdType.CMD_LC_UpLoadFtpError);
        bRet &= InitCmdTypeInfo<LC_Cmd_DayChange>(NetCmdType.CMD_LC_DayChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleAchievementIndex>(NetCmdType.CMD_LC_ChangeRoleAchievementIndex);
        bRet &= InitCmdTypeInfo<LC_Cmd_ResetRoleGlobel>(NetCmdType.CMD_LC_ResetRoleGlobel);
        bRet &= InitCmdTypeInfo<CL_Cmd_ResetRoleInfo>(NetCmdType.CMD_CL_ResetRoleInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_ResetRoleInfo>(NetCmdType.CMD_LC_ResetRoleInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleCheckData>(NetCmdType.CMD_LC_ChangeRoleCheckData);
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleIsShowIpAddress>(NetCmdType.CMD_CL_ChangeRoleIsShowIpAddress);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleIsShowIpAddress>(NetCmdType.CMD_LC_ChangeRoleIsShowIpAddress);
        bRet &= InitCmdTypeInfo<LC_Cmd_NewDay>(NetCmdType.CMD_LC_NewDay);

        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleExChangeStates>(NetCmdType.CMD_LC_ChangeRoleExChangeStates);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleTotalRecharge>(NetCmdType.CMD_LC_ChangeRoleTotalRecharge);
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleProtect>(NetCmdType.CMD_LC_RoleProtect);
        bRet &= InitCmdTypeInfo<CL_Cmd_RoleProtect>(NetCmdType.CMD_CL_RoleProtect);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleParticularStates>(NetCmdType.CMD_LC_ChangeRoleParticularStates);   
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleIsFirstPayGlobel>(NetCmdType.CMD_LC_ChangeRoleIsFirstPayGlobel);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleIsFirstPayCurrcey>(NetCmdType.CMD_LC_ChangeRoleIsFirstPayCurrcey);


        bRet &= InitCmdTypeInfo<LC_Cmd_SetSecondPassword>(NetCmdType.CMD_LC_SetSecondPassword);
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeSecondPassword>(NetCmdType.CMD_CL_ChangeSecondPassword);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeSecondPassword>(NetCmdType.CMD_LC_ChangeSecondPassword);
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleShareStates>(NetCmdType.CMD_CL_ChangeRoleShareStates);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleShareStates>(NetCmdType.CMD_LC_ChangeRoleShareStates);

        bRet &= InitCmdTypeInfo<LC_Cmd_OpenShareUI>(NetCmdType.CMD_LC_OpenShareUI);

        bRet &= InitCmdTypeInfo<CL_Cmd_UpdateAccount>(NetCmdType.CMD_CL_UpdateAccount);
        


        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleMonthCardInfo>(NetCmdType.CMD_LC_ChangeRoleMonthCardInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleRateValue>(NetCmdType.CMD_LC_ChangeRoleRateValue);
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleRateValue>(NetCmdType.CMD_CL_ChangeRoleRateValue);

        bRet &= InitCmdTypeInfo<CL_Cmd_GetMonthCardReward>(NetCmdType.CMD_CL_GetMonthCardReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetMonthCardReward>(NetCmdType.CMD_LC_GetMonthCardReward);

        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleVipLevel>(NetCmdType.CMD_LC_ChangeRoleVipLevel);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleCashSum>(NetCmdType.CMD_LC_ChangeRoleCashSum);
        

        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleExChangeStates>(NetCmdType.CMD_LC_ChangeRoleExChangeStates);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleTotalRecharge>(NetCmdType.CMD_LC_ChangeRoleTotalRecharge);
        
        //Query
        bRet &= InitCmdTypeInfo<CL_Cmd_QueryUserByNickName>(NetCmdType.CMD_CL_QueryUserByNickName);
        bRet &= InitCmdTypeInfo<CL_Cmd_QueryUserByUserID>(NetCmdType.CMD_CL_QueryUserByUserID);
        bRet &= InitCmdTypeInfo<CL_Cmd_QueryUserByGameID>(NetCmdType.CMD_CL_QueryUserByGameID);
        bRet &= InitCmdTypeInfo<LC_Cmd_QueryUserInfo>(NetCmdType.CMD_LC_QueryUserInfo);
        //bRet &= InitCmdTypeInfo<LC_Cmd_QueryUserInfoFinish>(NetCmdType.CMD_LC_QueryUserInfoFinish);
        bRet &= InitCmdTypeInfo<LC_Cmd_QueryUserOnce>(NetCmdType.CMD_LC_QueryUserOnce);
        //Check
        //bRet &= InitCmdTypeInfo<CL_Cmd_GetRoleCheckInfo>(NetCmdType.CMD_CL_GetRoleCheckInfo);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleCheckInfo>(NetCmdType.CMD_LC_GetRoleCheckInfo);
        bRet &= InitCmdTypeInfo<CL_Cmd_CheckNowDay>(NetCmdType.CMD_CL_CheckNowDay);
        bRet &= InitCmdTypeInfo<LC_Cmd_CheckNowDay>(NetCmdType.CMD_LC_CheckNowDay);
        bRet &= InitCmdTypeInfo<CL_Cmd_CheckOtherDay>(NetCmdType.CMD_CL_CheckOtherDay);
        bRet &= InitCmdTypeInfo<LC_Cmd_CheckOtherDay>(NetCmdType.CMD_LC_CheckOtherDay);
        //bRet &= InitCmdTypeInfo<LC_Cmd_CheckChange>(NetCmdType.CMD_LC_CheckChange);
        //Task
        bRet &= InitCmdTypeInfo<CL_Cmd_GetRoleTaskInfo>(NetCmdType.CMD_CL_GetRoleTaskInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleTaskInfo>(NetCmdType.CMD_LC_GetRoleTaskInfo);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleTaskInfoFinish>(NetCmdType.CMD_LC_GetRoleTaskInfoFinish);
        bRet &= InitCmdTypeInfo<LC_Cmd_TaskAllEventFinish>(NetCmdType.CMD_LC_TaskAllEventFinish);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetTaskReward>(NetCmdType.CMD_CL_GetTaskReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetTaskReward>(NetCmdType.CMD_LC_GetTaskReward);
        //bRet &= InitCmdTypeInfo<LC_Cmd_ResetTask>(NetCmdType.CMD_LC_ResetTask);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetOnceTaskInfo>(NetCmdType.CMD_CL_GetOnceTaskInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetOnceTaskInfo>(NetCmdType.CMD_LC_GetOnceTaskInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_JoinTask>(NetCmdType.CMD_LC_JoinTask);
        bRet &= InitCmdTypeInfo<LC_Cmd_ClearTask>(NetCmdType.CMD_LC_ClearTask);
        bRet &= InitCmdTypeInfo<LC_Cmd_DelTask>(NetCmdType.CMD_LC_DelTask);
        //成就
        bRet &= InitCmdTypeInfo<CL_Cmd_GetRoleAchievementInfo>(NetCmdType.CMD_CL_GetRoleAchievementInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleAchievementInfo>(NetCmdType.CMD_LC_GetRoleAchievementInfo);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleAchievementInfoFinish>(NetCmdType.CMD_LC_GetRoleAchievementInfoFinish);
        bRet &= InitCmdTypeInfo<LC_Cmd_AchievementAllEventFinish>(NetCmdType.CMD_LC_AchievementAllEventFinish);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetAchievementReward>(NetCmdType.CMD_CL_GetAchievementReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetAchievementReward>(NetCmdType.CMD_LC_GetAchievementReward);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetOnceAchievementInfo>(NetCmdType.CMD_CL_GetOnceAchievementInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetOnceAchievementInfo>(NetCmdType.CMD_LC_GetOnceAchievementInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_JoinAchievement>(NetCmdType.CMD_LC_JoinAchievement);
        bRet &= InitCmdTypeInfo<LC_Cmd_DelAchievement>(NetCmdType.CMD_LC_DelAchievement);
        //称号
        bRet &= InitCmdTypeInfo<CL_Cmd_LoadRoleTitle>(NetCmdType.CMD_CL_LoadRoleTitle);
        bRet &= InitCmdTypeInfo<LC_Cmd_LoadRoleTitle>(NetCmdType.CMD_LC_LoadRoleTitle);
        //bRet &= InitCmdTypeInfo<LC_Cmd_LoadRoleTitleFinish>(NetCmdType.CMD_LC_LoadRoleTitleFinish);
        bRet &= InitCmdTypeInfo<LC_Cmd_AddRoleTitle>(NetCmdType.CMD_LC_AddRoleTitle);
        bRet &= InitCmdTypeInfo<LC_Cmd_DelRoleTitle>(NetCmdType.CMD_LC_DelRoleTitle);
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleCurrTitle>(NetCmdType.CMD_CL_ChangeRoleCurrTitle);
        //比赛
        bRet &= InitCmdTypeInfo<CL_Cmd_SignUpMonth>(NetCmdType.CMD_CL_SignUpMonth);
        bRet &= InitCmdTypeInfo<LC_Cmd_SignUpMonthSucess>(NetCmdType.CMD_LC_SignUpMonthSucess);
        bRet &= InitCmdTypeInfo<LC_Cmd_SignUpMonthFiled>(NetCmdType.CMD_LC_SignUpMonthFiled);
        bRet &= InitCmdTypeInfo<CL_Cmd_JoinMonth>(NetCmdType.CMD_CL_JoinMonth);
        bRet &= InitCmdTypeInfo<LC_Cmd_JoinMonthSucess>(NetCmdType.CMD_LC_JoinMonthSucess);
        bRet &= InitCmdTypeInfo<LC_Cmd_JoinMonthFiled>(NetCmdType.CMD_LC_JoinMonthFiled);
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeUserAddMonthGlobelNum>(NetCmdType.CMD_CL_ChangeUserAddMonthGlobelNum);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeUserAddMonthGlobelNum>(NetCmdType.CMD_LC_ChangeUserAddMonthGlobelNum);
        bRet &= InitCmdTypeInfo<LC_Cmd_UserChangeIndex>(NetCmdType.CMD_LC_UserChangeIndex);
        bRet &= InitCmdTypeInfo<LC_Cmd_UserMonthEnd>(NetCmdType.CMD_LC_UserMonthEnd);
        bRet &= InitCmdTypeInfo<CL_Cmd_LoadRoleSignUpInfo>(NetCmdType.CMD_CL_LoadRoleSignUpInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_LoadRoleSignUpInfo>(NetCmdType.CMD_LC_LoadRoleSignUpInfo);
       // bRet &= InitCmdTypeInfo<CL_Cmd_GetMonthRoleSum>(NetCmdType.CMD_CL_GetMonthRoleSum);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetMonthRoleSum>(NetCmdType.CMD_LC_GetMonthRoleSum);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetMonthRoleSumFinish>(NetCmdType.CMD_LC_GetMonthRoleSumFinish);
        bRet &= InitCmdTypeInfo<CL_Cmd_ResetMonthInfo>(NetCmdType.CMD_CL_ResetMonthInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_ResetMonthInfo>(NetCmdType.CMD_LC_ResetMonthInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_SetRoleMonthInfo>(NetCmdType.CMD_LC_SetRoleMonthInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_ResetRoleMonthInfo>(NetCmdType.CMD_LC_ResetRoleMonthInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_MonthList>(NetCmdType.CMD_LC_MonthList);
        
        //排行榜
        bRet &= InitCmdTypeInfo<CL_Cmd_GetRankReward>(NetCmdType.CMD_CL_GetRankReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetRankReward>(NetCmdType.CMD_LC_GetRankReward);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetRankInfo>(NetCmdType.CMD_CL_GetRankInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetWeekRankInfo>(NetCmdType.CMD_LC_GetWeekRankInfo);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetRankInfoFinish>(NetCmdType.CMD_LC_GetRankInfoFinish);
        //Chest 宝箱
        //bRet &= InitCmdTypeInfo<LC_Cmd_AddChest>(NetCmdType.CMD_LC_AddChest);
        bRet &= InitCmdTypeInfo<LC_Cmd_ActionChest>(NetCmdType.CMD_LC_ActionChest);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChestEnd>(NetCmdType.CMD_LC_ChestEnd);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetChestReward>(NetCmdType.CMD_CL_GetChestReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetChestReward>(NetCmdType.CMD_LC_GetChestReward);
        bRet &= InitCmdTypeInfo<CL_Cmd_CloseChest>(NetCmdType.CMD_CL_CloseChest);
        bRet &= InitCmdTypeInfo<LC_Cmd_ResetChest>(NetCmdType.CMD_LC_ResetChest);
        
        //Charm 魅力
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeOtherUserCharm>(NetCmdType.CMD_CL_ChangeOtherUserCharm);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleCharmResult>(NetCmdType.CMD_LC_ChangeRoleCharmResult);
        bRet &= InitCmdTypeInfo<LC_Cmd_TableRoleCharmInfo>(NetCmdType.CMD_LC_TableRoleCharmInfo);
        
        //商店
        bRet &= InitCmdTypeInfo<CL_Cmd_ShopItem>(NetCmdType.CMD_CL_ShopItem);
        bRet &= InitCmdTypeInfo<LC_Cmd_ShopItemResult>(NetCmdType.CMD_LC_ShopItemResult);
        //Entity
        bRet &= InitCmdTypeInfo<CL_Cmd_LoadUserEntity>(NetCmdType.CMD_CL_LoadUserEntity);
        bRet &= InitCmdTypeInfo<LC_Cmd_LoadUserEntity>(NetCmdType.CMD_LC_LoadUserEntity);
       // bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleName>(NetCmdType.CMD_CL_ChangeRoleName);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleName>(NetCmdType.CMD_LC_ChangeRoleName);
       // bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRolePhone>(NetCmdType.CMD_CL_ChangeRolePhone);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRolePhone>(NetCmdType.CMD_LC_ChangeRolePhone);
       // bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleEmail>(NetCmdType.CMD_CL_ChangeRoleEmail);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleEmail>(NetCmdType.CMD_LC_ChangeRoleEmail);
       // bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleAddress>(NetCmdType.CMD_CL_ChangeRoleAddress);
       // bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleAddress>(NetCmdType.CMD_LC_ChangeRoleAddress);
       // bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleEntityID>(NetCmdType.CMD_CL_ChangeRoleEntityID);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleEntityID>(NetCmdType.CMD_LC_ChangeRoleEntityID);

        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleEntityItemUseName   >(NetCmdType.CMD_CL_ChangeRoleEntityItemUseName   );
        bRet &= InitCmdTypeInfo<LC_Cmd_CHangeRoleEntityItemUseName   >(NetCmdType.CMD_LC_ChangeRoleEntityItemUseName   );
        bRet &= InitCmdTypeInfo<CL_Cmd_ChagneRoleEntityItemUsePhone  >(NetCmdType.CMD_CL_ChagneRoleEntityItemUsePhone  );
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleEntityItemUsePhone  >(NetCmdType.CMD_LC_ChangeRoleEntityItemUsePhone  );
        bRet &= InitCmdTypeInfo<CL_Cmd_ChangeRoleEntityItemUseAddress>(NetCmdType.CMD_CL_ChangeRoleEntityItemUseAddress);
        bRet &= InitCmdTypeInfo<LC_Cmd_ChangeRoleEntityItemUseAddress>(NetCmdType.CMD_LC_ChangeRoleEntityItemUseAddress);
        //活动
        bRet &= InitCmdTypeInfo<CL_Cmd_GetRoleActionInfo>(NetCmdType.CMD_CL_GetRoleActionInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleActionInfo>(NetCmdType.CMD_LC_GetRoleActionInfo);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleActionInfoFinish>(NetCmdType.CMD_LC_GetRoleActionInfoFinish);
        //bRet &= InitCmdTypeInfo<LC_Cmd_ActionAllEventFinish>(NetCmdType.CMD_LC_ActionAllEventFinish);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetActionReward>(NetCmdType.CMD_CL_GetActionReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetActionReward>(NetCmdType.CMD_LC_GetActionReward);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetOnceActionInfo>(NetCmdType.CMD_CL_GetOnceActionInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetOnceActionInfo>(NetCmdType.CMD_LC_GetOnceActionInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_JoinAction>(NetCmdType.CMD_LC_JoinAction);
        bRet &= InitCmdTypeInfo<LC_Cmd_DelAction>(NetCmdType.CMD_LC_DelAction);
        //Giff
        bRet &= InitCmdTypeInfo<CL_Cmd_GetRoleGiff>(NetCmdType.CMD_CL_GetRoleGiff);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleGiff>(NetCmdType.CMD_LC_GetRoleGiff);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleGiffFinish>(NetCmdType.CMD_LC_GetRoleGiffFinish);
        bRet &= InitCmdTypeInfo<CL_Cmd_AddRoleGiff>(NetCmdType.CMD_CL_AddRoleGiff);
        bRet &= InitCmdTypeInfo<LC_Cmd_AddRoleGiffResult>(NetCmdType.CMD_LC_AddRoleGiffResult);
        bRet &= InitCmdTypeInfo<LC_Cmd_AddBeRoleGiff>(NetCmdType.CMD_LC_AddBeRoleGiff);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetRoleGiffReward>(NetCmdType.CMD_CL_GetRoleGiffReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleGiffReward>(NetCmdType.CMD_LC_GetRoleGiffReward);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetAllRoleGiffReward>(NetCmdType.CMD_CL_GetAllRoleGiffReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetAllRoleGiffReward>(NetCmdType.CMD_LC_GetAllRoleGiffReward);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetRoleNowDaySendGiff>(NetCmdType.CMD_CL_GetRoleNowDaySendGiff);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleNowDaySendGiff>(NetCmdType.CMD_LC_GetRoleNowDaySendGiff);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetRoleNowDaySendGiffFinish>(NetCmdType.CMD_LC_GetRoleNowDaySendGiffFinish);
        bRet &= InitCmdTypeInfo<LC_Cmd_DelRoleGiff>(NetCmdType.CMD_LC_DelRoleGiff);
        
        //GlobelShop
        // bRet &= InitCmdTypeInfo<CL_Cmd_GlobelShopItem>(NetCmdType.CMD_CL_GlobelShopItem);
        // bRet &= InitCmdTypeInfo<LC_Cmd_GlobelShopItem>(NetCmdType.CMD_LC_GlobelShopItem);
        //在线奖励
        bRet &= InitCmdTypeInfo<CL_Cmd_GetOnlineReward>(NetCmdType.CMD_CL_GetOnlineReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetOnlineReward>(NetCmdType.CMD_LC_GetOnlineReward);
        //gamedata
        bRet &= InitCmdTypeInfo<CL_Cmd_GetGameData>(NetCmdType.CMD_CL_GetGameData);
        bRet &= InitCmdTypeInfo<LC_Cmd_GendGameData>(NetCmdType.CMD_LC_GendGameData);
        //package
        //bRet &= InitCmdTypeInfo<CL_Cmd_OpenPackage>(NetCmdType.CMD_CL_OpenPackage);
        bRet &= InitCmdTypeInfo<LC_Cmd_OpenPackage>(NetCmdType.CMD_LC_OpenPackage);
        //Launcher
        bRet &= InitCmdTypeInfo<LC_Cmd_LauncherData>(NetCmdType.CMD_LC_LauncherData);
        //Message
        bRet &= InitCmdTypeInfo<LC_Cmd_SendMessage>(NetCmdType.CMD_LC_SendMessage);
        bRet &= InitCmdTypeInfo<CL_Cmd_SendMessage>(NetCmdType.CMD_CL_SendMessage);
        //Recharge
        bRet &= InitCmdTypeInfo<CL_Cmd_Recharge>(NetCmdType.CMD_CL_Recharge);
        bRet &= InitCmdTypeInfo<LC_Cmd_Recharge>(NetCmdType.CMD_LC_Recharge);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetOrderID>(NetCmdType.CMD_LC_GetOrderID);
        bRet &= InitCmdTypeInfo<CL_Cmd_IOSRecharge>(NetCmdType.CMD_CL_IOSRecharge);
        //Announcement
        bRet &= InitCmdTypeInfo<CL_Cmd_GetAllAnnouncement>(NetCmdType.CMD_CL_GetAllAnnouncement);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetAllAnnouncement>(NetCmdType.CMD_LC_GetAllAnnouncement);
        //bRet &= InitCmdTypeInfo<LC_Cmd_GetAllAnnouncementFinish>(NetCmdType.CMD_LC_GetAllAnnouncementFinish);
        bRet &= InitCmdTypeInfo<LC_Cmd_SendNewAnnouncementOnce>(NetCmdType.CMD_LC_SendNewAnnouncementOnce);
        //operate
        bRet &= InitCmdTypeInfo<CL_Cmd_RealNameVerification >(NetCmdType.CMD_CL_RealNameVerification );
        bRet &= InitCmdTypeInfo<LC_Cmd_RealNameVerification>(NetCmdType.CMD_LC_RealNameVerification);
        bRet &= InitCmdTypeInfo<CL_Cmd_GetPhoneVerificationNum>(NetCmdType.CMD_CL_GetPhoneVerificationNum);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetPhoneVerificationNum>(NetCmdType.CMD_LC_GetPhoneVerificationNum);
        bRet &= InitCmdTypeInfo<CL_Cmd_BindPhone>(NetCmdType.CMD_CL_BindPhone);
        bRet &= InitCmdTypeInfo<LC_Cmd_BindPhone >(NetCmdType.CMD_LC_BindPhone );
        bRet &= InitCmdTypeInfo<LC_Cmd_BuyEntityItem >(NetCmdType.CMD_LC_BuyEntityItem );
        bRet &= InitCmdTypeInfo<CL_Cmd_BindEmail>(NetCmdType.CMD_CL_BindEmail );
        bRet &= InitCmdTypeInfo<LC_Cmd_BindEmail>(NetCmdType.CMD_LC_BindEmail);
        bRet &= InitCmdTypeInfo<LC_Cmd_PhonePay>(NetCmdType.CMD_LC_PhonePay);
        bRet &= InitCmdTypeInfo<LC_Cmd_AddNormalOrderID>(NetCmdType.CMD_LC_AddNormalOrderID);
        
        //RoleMessage
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleMessageChange>(NetCmdType.CMD_LC_RoleMessageChange);
        //ExChange
        bRet &= InitCmdTypeInfo<CL_Cmd_RoleUseExChangeCode>(NetCmdType.CMD_CL_RoleUseExChangeCode);
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleUseExChangeCode>(NetCmdType.CMD_LC_RoleUseExChangeCode);
        //Lottery
        bRet &= InitCmdTypeInfo<CL_Cmd_GetLotteryReward>(NetCmdType.CMD_CL_GetLotteryReward);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetLotteryReward>(NetCmdType.CMD_LC_GetLotteryReward);

        bRet &= InitCmdTypeInfo<CL_Cmd_LotteryUIStates>(NetCmdType.CMD_CL_LotteryUIStates);
        bRet &= InitCmdTypeInfo<LC_Cmd_LotteryUIStates>(NetCmdType.CMD_LC_LotteryUIStates);
        
        

        bRet &= InitCmdTypeInfo<NetCmdCheckVer>(NetCmdType.CMD_EXTRA_CHECK_VER);
        bRet &= InitCmdTypeInfo<NetCmdFileRequest>(NetCmdType.CMD_EXTRA_FILE);
        bRet &= InitCmdTypeInfo<NetCmdFileRequest>(NetCmdType.CMD_EXTRA_FILE_EX);

        //minigame
        //bRet &= InitCmdTypeInfo<CL_Cmd_JoinRoom>(NetCmdType.CMD_CL_JoinRoom);
        //bRet &= InitCmdTypeInfo<LC_Cmd_JoinRoom>(NetCmdType.CMD_LC_JoinRoom);
        //bRet &= InitCmdTypeInfo<LC_Cmd_TableUpdate>(NetCmdType.CMD_LC_TableUpdate);
        //bRet &= InitCmdTypeInfo<CL_Cmd_AddGlobel>(NetCmdType.CMD_CL_AddGlobel);
        //bRet &= InitCmdTypeInfo<LC_Cmd_AddGlobel>(NetCmdType.CMD_LC_AddGlobel);
        //bRet &= InitCmdTypeInfo<LC_Cmd_TableResult>(NetCmdType.CMD_LC_TableResult);
        //bRet &= InitCmdTypeInfo<LC_Cmd_TableBegin>(NetCmdType.CMD_LC_TableBegin);
        //bRet &= InitCmdTypeInfo<CL_Cmd_LeaveNiuNiuTable>(NetCmdType.CMD_CL_LeaveNiuNiuTable);

        //bRet &= InitCmdTypeInfo<LC_Cmd_BankerChange>(NetCmdType.CMD_LC_BankerChange);
        //bRet &= InitCmdTypeInfo<LC_Cmd_WirteBankerListChange>(NetCmdType.CMD_LC_WirteBankerListChange);
        //bRet &= InitCmdTypeInfo<CL_Cmd_AddBankerList>(NetCmdType.CMD_CL_AddBankerList);
        //bRet &= InitCmdTypeInfo<LC_Cmd_AddBankerList>(NetCmdType.CMD_LC_AddBankerList);
        
        //NiuNiu

        bRet &= InitCmdTypeInfo<CL_Cmd_RoleJoinTable         >(NetCmdType.CMD_CL_RoleJoinTable         );
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleJoinTable         >(NetCmdType.CMD_LC_RoleJoinTable         );
        bRet &= InitCmdTypeInfo<CL_Cmd_RoleLeaveTable        >(NetCmdType.CMD_CL_RoleLeaveTable        );
        bRet &= InitCmdTypeInfo<CL_Cmd_RoleBetGlobel         >(NetCmdType.CMD_CL_RoleBetGlobel         );
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleBetGlobel         >(NetCmdType.CMD_LC_RoleBetGlobel         );
        bRet &= InitCmdTypeInfo<CL_Cmd_GetBankerList         >(NetCmdType.CMD_CL_GetBankerList         );
        bRet &= InitCmdTypeInfo<LC_Cmd_GetBankerList         >(NetCmdType.CMD_LC_GetBankerList         );
        bRet &= InitCmdTypeInfo<LC_Cmd_BankerListChange      >(NetCmdType.CMD_LC_BankerListChange      );
        bRet &= InitCmdTypeInfo<LC_Cmd_BankerUserChange      >(NetCmdType.CMD_LC_BankerUserChange      );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableJoinBegin        >(NetCmdType.CMD_LC_TableJoinBegin        );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableUpdate           >(NetCmdType.CMD_LC_TableUpdate           );
        bRet &= InitCmdTypeInfo<LC_Cmd_TableJoinEnd          >(NetCmdType.CMD_LC_TableJoinEnd          );
        bRet &= InitCmdTypeInfo<CL_Cmd_RoleJoinBankerList    >(NetCmdType.CMD_CL_RoleJoinBankerList    );
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleJoinBankerList    >(NetCmdType.CMD_LC_RoleJoinBankerList    );
        bRet &= InitCmdTypeInfo<CL_Cmd_RoleGetBankerFirstSeat>(NetCmdType.CMD_CL_RoleGetBankerFirstSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleGetBankerFirstSeat>(NetCmdType.CMD_LC_RoleGetBankerFirstSeat);
        bRet &= InitCmdTypeInfo<CL_Cmd_RoleLeaveBankerList   >(NetCmdType.CMD_CL_RoleLeaveBankerList   );
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleLeaveBankerList   >(NetCmdType.CMD_LC_RoleLeaveBankerList   );
        bRet &= InitCmdTypeInfo<CL_Cmd_RoleCanelBankerSeat   >(NetCmdType.CMD_CL_RoleCanelBankerSeat   );
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleCanelBankerSeat   >(NetCmdType.CMD_LC_RoleCanelBankerSeat   );
        bRet &= InitCmdTypeInfo<LC_Cmd_BankerUserGlobelChange>(NetCmdType.CMD_LC_BankerUserGlobelChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_WriteBankerUserGlobelMsg>(NetCmdType.CMD_LC_WriteBankerUserGlobelMsg);

        bRet &= InitCmdTypeInfo<CL_Cmd_RoleJoinVipSeat      >(NetCmdType.CMD_CL_RoleJoinVipSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleJoinVipSeat      >(NetCmdType.CMD_LC_RoleJoinVipSeat);
        bRet &= InitCmdTypeInfo<CL_Cmd_RoleLeaveVipSeat     >(NetCmdType.CMD_CL_RoleLeaveVipSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleLeaveVipSeat     >(NetCmdType.CMD_LC_RoleLeaveVipSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_RoleBeLeaveVipSeat   >(NetCmdType.CMD_LC_RoleBeLeaveVipSeat);

        bRet &= InitCmdTypeInfo<CL_Cmd_GetNormalSeatInfo>(NetCmdType.CMD_CL_GetNormalSeatInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_GetNormalSeatInfo>(NetCmdType.CMD_LC_GetNormalSeatInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_VipSeatGlobelChange>(NetCmdType.CMD_LC_VipSeatGlobelChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_TableStopUpdate>(NetCmdType.CMD_LC_TableStopUpdate);
        bRet &= InitCmdTypeInfo<LC_Cmd_VipGlobelChange>(NetCmdType.CMD_LC_VipGlobelChange);

        //Dial

        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleJoinTable>(NetCmdType.CMD_CL_DialRoleJoinTable);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleJoinTable>(NetCmdType.CMD_LC_DialRoleJoinTable);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleLeaveTable>(NetCmdType.CMD_CL_DialRoleLeaveTable);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleBetGlobel>(NetCmdType.CMD_CL_DialRoleBetGlobel);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleBetGlobel>(NetCmdType.CMD_LC_DialRoleBetGlobel);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialGetBankerList>(NetCmdType.CMD_CL_DialGetBankerList);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialGetBankerList>(NetCmdType.CMD_LC_DialGetBankerList);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialBankerListChange>(NetCmdType.CMD_LC_DialBankerListChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialBankerUserChange>(NetCmdType.CMD_LC_DialBankerUserChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialTableJoinBegin>(NetCmdType.CMD_LC_DialTableJoinBegin);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialTableUpdate>(NetCmdType.CMD_LC_DialTableUpdate);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialTableJoinEnd>(NetCmdType.CMD_LC_DialTableJoinEnd);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleJoinBankerList>(NetCmdType.CMD_CL_DialRoleJoinBankerList);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleJoinBankerList>(NetCmdType.CMD_LC_DialRoleJoinBankerList);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleGetBankerFirstSeat>(NetCmdType.CMD_CL_DialRoleGetBankerFirstSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleGetBankerFirstSeat>(NetCmdType.CMD_LC_DialRoleGetBankerFirstSeat);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleLeaveBankerList>(NetCmdType.CMD_CL_DialRoleLeaveBankerList);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleLeaveBankerList>(NetCmdType.CMD_LC_DialRoleLeaveBankerList);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleCanelBankerSeat>(NetCmdType.CMD_CL_DialRoleCanelBankerSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleCanelBankerSeat>(NetCmdType.CMD_LC_DialRoleCanelBankerSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialBankerUserGlobelChange>(NetCmdType.CMD_LC_DialBankerUserGlobelChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialWriteBankerUserGlobelMsg>(NetCmdType.CMD_LC_DialWriteBankerUserGlobelMsg);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleJoinVipSeat>(NetCmdType.CMD_CL_DialRoleJoinVipSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleJoinVipSeat>(NetCmdType.CMD_LC_DialRoleJoinVipSeat);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleLeaveVipSeat>(NetCmdType.CMD_CL_DialRoleLeaveVipSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleLeaveVipSeat>(NetCmdType.CMD_LC_DialRoleLeaveVipSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleBeLeaveVipSeat>(NetCmdType.CMD_LC_DialRoleBeLeaveVipSeat);
        bRet &= InitCmdTypeInfo<CL_Cmd_DialGetNormalSeatInfo>(NetCmdType.CMD_CL_DialGetNormalSeatInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialGetNormalSeatInfo>(NetCmdType.CMD_LC_DialGetNormalSeatInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialVipSeatGlobelChange>(NetCmdType.CMD_LC_DialVipSeatGlobelChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialTableStopUpdate>(NetCmdType.CMD_LC_DialTableStopUpdate);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialVipGlobelChange>(NetCmdType.CMD_LC_DialVipGlobelChange);

        bRet &= InitCmdTypeInfo<CL_Cmd_DialRoleBetGlobelByLog>(NetCmdType.CMD_CL_DialRoleBetGlobelByLog);
        bRet &= InitCmdTypeInfo<LC_Cmd_DialRoleBetGlobelByLog>(NetCmdType.CMD_LC_DialRoleBetGlobelByLog);
        
        
        //char
        bRet &= InitCmdTypeInfo<CL_Cmd_LoadAllCharInfo      >(NetCmdType.CMD_CL_LoadAllCharInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_LoadAllCharInfo      >(NetCmdType.CMD_LC_LoadAllCharInfo);
        bRet &= InitCmdTypeInfo<CL_Cmd_LoadCharListByUserID >(NetCmdType.CMD_CL_LoadCharListByUserID);
        bRet &= InitCmdTypeInfo<LC_Cmd_LoadCharListByUserID >(NetCmdType.CMD_LC_LoadCharListByUserID);
        bRet &= InitCmdTypeInfo<CL_Cmd_SendCharInfo         >(NetCmdType.CMD_CL_SendCharInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_SendCharInfo         >(NetCmdType.CMD_LC_SendCharInfo);
        
        
        //
        bRet &= InitCmdTypeInfo<CL_Cmd_LoadRelationRequest   >(NetCmdType.CMD_CL_LoadRelationRequest   );
        bRet &= InitCmdTypeInfo<LC_Cmd_LoadRelationRequest   >(NetCmdType.CMD_LC_LoadRelationRequest   );
        bRet &= InitCmdTypeInfo<CL_Cmd_SendRelationRequest   >(NetCmdType.CMD_CL_SendRelationRequest   );
        bRet &= InitCmdTypeInfo<LC_Cmd_SendRelationRequest   >(NetCmdType.CMD_LC_SendRelationRequest   );
        bRet &= InitCmdTypeInfo<LC_Cmd_BeAddRelationRequest  >(NetCmdType.CMD_LC_BeAddRelationRequest  );
        bRet &= InitCmdTypeInfo<CL_Cmd_HandleRelationRequest >(NetCmdType.CMD_CL_HandleRelationRequest );
        bRet &= InitCmdTypeInfo<LC_Cmd_HandleRelationRequest >(NetCmdType.CMD_LC_HandleRelationRequest );
        bRet &= InitCmdTypeInfo<LC_Cmd_DelRelationRequest>(NetCmdType.CMD_LC_DelRelationRequest);



        //Car

        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleJoinTable>(NetCmdType.CMD_CL_CarRoleJoinTable);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleJoinTable>(NetCmdType.CMD_LC_CarRoleJoinTable);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleLeaveTable>(NetCmdType.CMD_CL_CarRoleLeaveTable);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleBetGlobel>(NetCmdType.CMD_CL_CarRoleBetGlobel);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleBetGlobel>(NetCmdType.CMD_LC_CarRoleBetGlobel);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarGetBankerList>(NetCmdType.CMD_CL_CarGetBankerList);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarGetBankerList>(NetCmdType.CMD_LC_CarGetBankerList);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarBankerListChange>(NetCmdType.CMD_LC_CarBankerListChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarBankerUserChange>(NetCmdType.CMD_LC_CarBankerUserChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarTableJoinBegin>(NetCmdType.CMD_LC_CarTableJoinBegin);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarTableUpdate>(NetCmdType.CMD_LC_CarTableUpdate);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarTableJoinEnd>(NetCmdType.CMD_LC_CarTableJoinEnd);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleJoinBankerList>(NetCmdType.CMD_CL_CarRoleJoinBankerList);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleJoinBankerList>(NetCmdType.CMD_LC_CarRoleJoinBankerList);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleGetBankerFirstSeat>(NetCmdType.CMD_CL_CarRoleGetBankerFirstSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleGetBankerFirstSeat>(NetCmdType.CMD_LC_CarRoleGetBankerFirstSeat);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleLeaveBankerList>(NetCmdType.CMD_CL_CarRoleLeaveBankerList);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleLeaveBankerList>(NetCmdType.CMD_LC_CarRoleLeaveBankerList);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleCanelBankerSeat>(NetCmdType.CMD_CL_CarRoleCanelBankerSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleCanelBankerSeat>(NetCmdType.CMD_LC_CarRoleCanelBankerSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarBankerUserGlobelChange>(NetCmdType.CMD_LC_CarBankerUserGlobelChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarWriteBankerUserGlobelMsg>(NetCmdType.CMD_LC_CarWriteBankerUserGlobelMsg);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleJoinVipSeat>(NetCmdType.CMD_CL_CarRoleJoinVipSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleJoinVipSeat>(NetCmdType.CMD_LC_CarRoleJoinVipSeat);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleLeaveVipSeat>(NetCmdType.CMD_CL_CarRoleLeaveVipSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleLeaveVipSeat>(NetCmdType.CMD_LC_CarRoleLeaveVipSeat);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleBeLeaveVipSeat>(NetCmdType.CMD_LC_CarRoleBeLeaveVipSeat);
        bRet &= InitCmdTypeInfo<CL_Cmd_CarGetNormalSeatInfo>(NetCmdType.CMD_CL_CarGetNormalSeatInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarGetNormalSeatInfo>(NetCmdType.CMD_LC_CarGetNormalSeatInfo);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarVipSeatGlobelChange>(NetCmdType.CMD_LC_CarVipSeatGlobelChange);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarTableStopUpdate>(NetCmdType.CMD_LC_CarTableStopUpdate);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarVipGlobelChange>(NetCmdType.CMD_LC_CarVipGlobelChange);

        bRet &= InitCmdTypeInfo<CL_Cmd_CarRoleBetGlobelByLog>(NetCmdType.CMD_CL_CarRoleBetGlobelByLog);
        bRet &= InitCmdTypeInfo<LC_Cmd_CarRoleBetGlobelByLog>(NetCmdType.CMD_LC_CarRoleBetGlobelByLog);

        
        //结束
        return bRet;
    }

   
}
 
 
 
 







