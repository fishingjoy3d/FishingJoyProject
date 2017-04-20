using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//客户端的全部的消息类
//public enum ErrorCode
//{
//    EC_Init = 0,
//    EC_Sucess= 0,
//    //错误编码 与服务器端相同
//    //GlobelShop
//    EC_GlobelShop_ItemNotExites,    //指定的物品在货币商店不存在
//    EC_GlobelShop_CurreyError,      //货币不够购买货币商店的指定物品
//    EC_Globel_Shop_Sucess,          //购买物品成功了
//    //RoleAchievement
//    EC_RoleAchievement_NotExites,//成就不存在
//    EC_RoleAchievement_NotFinih,//成就条件未完成 无法提交完成
//    //RoleAction
//    EC_RoleAction_NotExites,//活动不存在
//    EC_RoleAction_NotFinih,//活动条件未完成 无法提交完成
//    //RoleCharm
//    EC_RoleCharm_NonMe,//不可以给自己添加魅力物品
//    EC_RoleCharm_NonItem,//指定的魅力物品不存在
//    EC_RoleCharm_GlobelError,//指定魅力物品的数量不够
//    EC_RoleCharm_Sucess,//赠送魅力物品成功了
//    EC_RoleCharm_SystemError,//系统错误 
//    EC_RoleCharm_CenterFiled,//未与中央服务器连接
//    //RoleCheck
//    EC_RoleCheck_CheckNowDayError,//今天已经签到过了
//    EC_RoleCheck_CheckOtherMoneyError,//货币不够 无法补签
//    EC_RoleCheck_CheckOtherError,//无法补签 因为没有需要补签的日期
//    //RoleEntity
//    EC_RoleEntity_NameError,//实际名称长度不正确 无法修改
//    EC_RoleEntity_Phone,//实际手机号码不正确 无法修改
//    EC_RoleEntity_EMail,//实际邮箱地址不正确 无法修改
//    EC_RoleEntity_Address,//实际用户地址不正确 无法修改
//    EC_RoleEntity_EntityID,//实际用户的身份证号码不正确 无法修改
//    //RoleGiff
//    EC_RoleGiff_NotMe,//不可以给自己发送赠送
//    EC_RoleGiff_SendMaxSum,//今天已经到达最大赠送次数 无法继续赠送
//    EC_RoleGiff_SendOnceUserSum,//今天对当前玩家的赠送次数已经到达最大 无法继续赠送
//    EC_RoleGiff_RecvMaxSum,//今天已经到达最大的接收赠送次数 无法继续接收
//    EC_RoleGiff_GiffNotExites,//当前赠送不存在 无法接收
//    EC_RoleGiff_GiffNull,//不存在可以接收的赠送
//    EC_RoleGiff_TargetGiffMax,//目标的赠送列表已经满了 无法继续接收赠送
//    EC_RoleGiff_CenterFiled,//为连接中央服务器
//    //RoleItem
//    //RoleMail
//    EC_RoleMail_NotMe,//不可以给自己发送邮件
//    EC_RoleMail_MailNull,//目标邮件不存在
//    EC_RoleMail_ItemNonExites,//当前邮件无附件 无法领取
//    EC_RoleMail_ContextError,//邮件主体长度不正确 无法发送
//    EC_RoleMail_SendTargetMailFull,//邮件发送失败 目标邮箱已经满了 
//    EC_RoleMail_DelMailError,//目标邮件不存在 无法删除
//    EC_RoleMail_CenterField,//无法连接中央服务器
//    EC_RoleMail_SendSucess,//邮件发送成功
//    //RoleMonth
//    EC_RoleMonth_SigUpMonthNotExists,//当前比赛不存在 无法报名
//    EC_RoleMonth_SigUpMonthTimeError,//当前比赛不在报名时间 无法报名
//    EC_RoleMonth_SigUpMonthError,//已经报名当前比赛了 无法继续报名
//    EC_RoleMonth_SigUpMonthTableError,//无法加入放弃比赛的桌子的限制 无法报名比赛
//    EC_RoleMonth_SigUpMonthMoneyError,//货币不够 无法报名比赛
//    EC_RoleMonth_SigUpMonthItemError,//物品不够 无法报名比赛
//    EC_RoleMonth_JoinMonthNotExists,//当前比赛不存在 无法创建
//    EC_RoleMonth_JoinMonthTimeError,//当前比赛不在参加时间 无法参加
//    EC_RoleMonth_JoinMonthError,//未报名当前比赛了 无法继续参加
//    EC_RoleMonth_JoinMonthTableError,//无法加入放弃比赛的桌子的限制 无法参加比赛
//    EC_RoleMonth_AddMonthGlobelMonthError,//当前比赛不存在 无法续币
//    EC_RoleMonth_AddMonthGlobelSumError,//无法继续续币了 到达最大次数了
//    EC_RoleMonth_AddMonthGlobelMoneyError,//无法续币 货币不够
//    EC_RoleMonth_ResetMonthMonthError,//重置比赛失败 当前比赛不存在
//    EC_RoleMonth_ResetMonthMoneyError,//货币不够 无法重置比赛
//    EC_RoleMonth_ResetMonthItemError,//物品不够 无法重置比赛
//    //RoleOnlirneReward
//    EC_RoleOnlineReward_RewardIDError,//当前在线奖励不存在 无法领取
//    EC_RoleOnlineReward_OnlineMinError,//无法领取在线奖励 在线时间不够
//    EC_RoleOnlineReward_Error,//已经领取过在线奖励了 无法继续领取
//    EC_RoleOnlineReward_Sucess,//领取在线奖励成功
//    //RoleQuery
//    EC_QueryRole_RoleNameError,//查询名称长度不正确 无法查询
//    EC_QueryRole_QueryMeError,//不可以查询自己
//    //RoleRank
//    EC_RoleRank_WeekReward_RankNotExites,//指定排行榜不存在
//    EC_RoleRank_WeekReward_NotInRank,//上周为上榜 无法领取奖励
//    EC_RoleRank_WeekReward_Error,//已经领取奖励了 无法继续领取
//    //RoleRelation
//    EC_RoleRelation_Add_NotMe,//不可以添加自己为好友
//    EC_RoleRelation_Add_RelationExists,//已经存在关系 无法继续添加
//    EC_RoleRelation_Add_RelationIsFull,//关系列表已经满了 无法继续添加
//    EC_RoleRelation_Del_RelationNotExists,//关系不存在 无法删除
//    EC_RoleRelation_Change_RelationNotExists,//指定关系不存在 无法修改
//    EC_RoleRelation_Change_Error,//当前关系为发生变化 无法修改
//    //RoleShop
//    EC_RoleShop_ShopError,//指定商店不存在 无法购买
//    EC_RoleShop_ItemNotExists,//指定商店的物品不在 无法购买
//    EC_RoleShop_ItemNotinTime,//指定物品为激活 无法购买
//    EC_RoleShop_EntityItemError,//购买实体物品 必须完善显示地址和数据 
//    EC_RoleShop_MoneyError,//购买物品失败 货币不够
//    //RoleTable
//    EC_RoleTable_TableError,//指定桌子不存在 无法进入
//    EC_RoleTable_GlobelMin,//金币数量太少 无法进入桌子
//    EC_RoleTable_GlobelMax,//金币数量太多 无法进入桌子
//    EC_RoleTable_CurreyMin,//乐币数量太少 无法进入桌子
//    EC_RoleTable_CurreyMax,//乐币数量太多 无法进入桌子
//    EC_RoleTable_ItemError,//进入桌子需要的物品不符合 无法进入桌子
//    //RoleTask
//    EC_RoleTask_NotExites,//活动不存在
//    EC_RoleTask_NotFinih,//任务条件未完成 无法提交完成
//    //RoleTitle
//    EC_RoleTitle_NonTitle,//称号不存在 无法切换称号
//    //Package
//    EC_Package_NonItem,//礼包不存在
//    EC_Package_BagNonItem,//背包里没有礼包存在
//    EC_Package_Null,//礼包为空
//    EC_Package_Succes,//礼包打开成功
//    //Message
//    EC_Message_Length,//发送消息长度不正确 无法发送
//    EC_Message_Type,//消息类型错误

//    //实名验证
//    EC_Verification_Sucess,//验证成功
//    EC_Verification_IsExists,//重复提交订单
//    EC_Verification_Filed,//验证失败
//    //手机绑定
//    EC_BindPhone_PhoneNumberSucess,//手机验证码已经发送
//    EC_BindPhone_PhoneNumberFailed,//手机验证码发送失败
//    EC_BindPhone_PhoneNumberIsExists,//手机已经注册过了
//    EC_BindPhone_Sucess,
//    EC_BindPhone_Failed,
//    //实体物品购买
//    EC_EntityItem_Failed,
//    EC_EntityItem_Sucess,
//    //绑定邮箱
//    EC_BindEMail_Failed,
//    EC_BindEMail_Sucess,
//    EC_BindEmail_IsUse,
//    //充值
//    EC_Recharge_Sucess,
//    EC_Recharge_Failed,
//    //修改密码
//    EC_ResetPassword_Sucess,
//    EC_ResetPassword_Failed,
//    //物品使用
//    EC_UseItem_Sucess,
//    EC_UseItem_Failed,
//    //使用RMB
//    EC_UseRMB_Sucess,
//    EC_UseRMB_Failed,
//    //
//    EC_NickName_Error,//当前用户名不可以使用
//    EC_AccountName_Error,//账号名称错误
//    EC_Account_IsExists,//账号已经存在
//    EC_Password_Error,//密码不可以使用
//    EC_Logon_Error,

//    //登陆的最新的提示
//    EC_Logon_AccountNotExists,//账号不存在
//    EC_Logon_PasswordError,//输入的密码错误
//    EC_Rsg_AccountExists,//账号已经存在 无法注册
//    //字符串错误提示
//    EC_AccountName_StringError,//账号字符串错误
//    EC_Password_StringError,//密码错误
//    EC_NickName_StringError,//用户名错误
//    EC_NickName_IsExists,//用户名已经存在
//    EC_MailContext_StringError,//邮件内容错误

//    EC_ResetAccount_Error,//非游客账号 无法重置账号
//    EC_ResetAccount_ServerError,//服务器告诉客户端 无法重置账号

//    EC_Max = 0xff,
//}
public enum UserOperateMessage
{
    UOM_Init  = 0,

    //用户操作的返回结果 客户端 自行定义 无须通过服务器端的
    //用户
    UOM_Role_ChangeFace_Sucess,
    UOM_Role_ChangeFace_Failed,
    UOM_Role_ChangeFace_Failed_1,//前后一样的头像

    UOM_Role_Gender_Sucess,
    UOM_Role_Gender_Failed,
    UOM_Role_Gender_Failed_1,//前后一致
    UOM_Role_Gender_Failed_2,//改性别物品不够

    UOM_Role_NickName_Sucess,
    UOM_Role_NickName_Failed,
    UOM_Role_NickName_Failed_1,//名称无法使用
    UOM_Role_NickName_Failed_2,//前后一致
    UOM_Role_NickName_Failed_3,//名称已经被使用
    UOM_Role_NickName_Failed_4,//改名物品不够

    UOM_Role_IsShowIpAddress_Sucess,
    UOM_Role_IsShowIpAddress_Failed,
    UOM_Role_IsShowIpAddress_Failed_1,//新旧状态一样

    UOM_Role_ChangePassword_Sucess,
    UOM_Role_ChangePassword_Failed,
    UOM_Role_ChangePassword_Failed_1,//旧密码无法使用
    UOM_Role_ChangePassword_Failed_2,//新密码无法使用
    UOM_Role_ChangePassword_Failed_3,//新旧密码一样
    UOM_Role_ChangePassword_Failed_4,//旧密码转化失败
    UOM_Role_ChangePassword_Failed_5,//新密码转化失败
    UOM_Role_ChangePassword_Failed_6,//旧密码错误

    UOM_Role_ResetAccount_Sucess,
    UOM_Role_ResetAccount_Failed,
    UOM_Role_ResetAccount_Failed_1,//非游客
    UOM_Role_ResetAccount_Failed_2,//账号无法使用
    UOM_Role_ResetAccount_Failed_3,//密码无法使用
    UOM_Role_ResetAccount_Failed_4,//转化密码失败
    UOM_Role_ResetAccount_Failed_5,//账号已经被使用

    UOM_Role_CustFace_Sucess,
    UOM_Role_CustFace_Failed,

    UOM_Role_MonthCardReward_Sucess,
    UOM_Role_MonthCardReward_Failed,
    UOM_Role_MonthCardReward_Failed_1,//不是月卡用户
    UOM_Role_MonthCardReward_Failed_2,//今天已经领取过了
    UOM_Role_MonthCardReward_Failed_3,//系统错误

    //成就
    UOM_Achievement_GetReward_Sucess,
    UOM_Achievement_GetReward_Failed,
    UOM_Achievement_GetReward_Failed_1,//成就不存在
    UOM_Achievement_GetReward_Failed_2,//未完成

    //活动
    UOM_Action_GetReward_Sucess,
    UOM_Action_GetReward_Failed,
    UOM_Action_GetReward_Failed_1,//活动不存在
    UOM_Action_GetReward_Failed_2,//阶段不存在
    UOM_Action_GetReward_Failed_3,//阶段未完成
    UOM_Action_GetReward_Failed_4,//阶段已经完成
    UOM_Action_GetReward_Failed_5,//活动过期

    //魅力
    UOM_Charm_SendCharm_Sucess,
    UOM_Charm_SendCharm_Failed,
    UOM_Charm_SendCharm_Failed_1,//不可以给自己赠送
    UOM_Charm_SendCharm_Failed_2,//不存在的魅力物品
    UOM_Charm_SendCharm_Failed_3,//无物品 或者货币不足
    UOM_Charm_SendCharm_Failed_4,//目标玩家不存在

    //签到
    UOM_Check_NowDay_Sucess,
    UOM_Check_NowDay_Failed,
    UOM_Check_NowDay_Failed_1,//当前已经签到了

    UOM_Check_OtherDay_Sucess,
    UOM_Check_OtherDay_Failed,
    UOM_Check_OtherDay_Failed_1,//金钱不够
    UOM_Check_OtherDay_Failed_2,//当前已经签到了
    UOM_Check_OtherDay_Failed_3,//系统错误

    //宝箱
    UOM_Chest_OpenChest_Sucess,
    UOM_Chest_OpenChest_Failed,
    UOM_Chest_OpenChest_Failed_1,//未激活的宝箱
    UOM_Chest_OpenChest_Failed_2,//指定位置已经开启了
    UOM_Chest_OpenChest_Failed_3,//指定宝箱不存在
    UOM_Chest_OpenChest_Failed_4,//宝箱开启已经到达最大次数
    UOM_Chest_OpenChest_Failed_5,//开启宝箱货币不够
    UOM_Chest_OpenChest_Failed_6,//系统错误

    UOM_Chest_CloseChest_Sucess,
    UOM_Chest_CloseChest_Failed,
    UOM_Chest_CloseChest_Failed_1,//指定宝箱不存在
    UOM_Chest_CloseChest_Failed_2,//系统错误

    //实体数据
    UOM_Entity_EntityItemUseAddress_Sucess,
    UOM_Entity_EntityItemUseAddress_Failed,
    UOM_Entity_EntityItemUseAddress_Failed_1,//长度不正确

    UOM_Entity_EntityItemUsePhone_Sucess,
    UOM_Entity_EntityItemUsePhone_Failed,
    UOM_Entity_EntityItemUsePhone_Failed_1,//长度不正确

    UOM_Entity_EntityItemUseName_Sucess,
    UOM_Entity_EntityItemUseName_Failed,
    UOM_Entity_EntityItemUseName_Failed_1,//长度不正确

    //赠送
    UOM_Giff_GetAllGiff_Sucess,
    UOM_Giff_GetAllGiff_Failed,
    UOM_Giff_GetAllGiff_Failed_1,//最大次数
    UOM_Giff_GetAllGiff_Failed_2,//无赠送

    UOM_Giff_SendGiff_Sucess,
    UOM_Giff_SendGiff_Failed,
    UOM_Giff_SendGiff_Failed_1,//最大次数
    UOM_Giff_SendGiff_Failed_2,//单个玩家最大次数
    UOM_Giff_SendGiff_Failed_3,//不可以给自己赠送
    UOM_Giff_SendGiff_Failed_4,//目标玩家列表满了

    UOM_Giff_AcceptGiff_Sucess,
    UOM_Giff_AcceptGiff_Failed,
    UOM_Giff_AcceptGiff_Failed_1,//最大次数
    UOM_Giff_AcceptGiff_Failed_2,//指定赠送不存在

    //物品
    UOM_Item_UseItem_Sucess,
    UOM_Item_UseItem_Failed,
    UOM_Item_UseItem_Failed_1,//使用数量不可以为0
    UOM_Item_UseItem_Failed_2,//物品不存在
    UOM_Item_UseItem_Failed_3,//使用数量不够
    UOM_Item_UseItem_Failed_4,//物品类型不允许使用
    UOM_Item_UseItem_Failed_5,//系统错误
    UOM_Item_UseItem_Failed_6,//当前物品不可以同时使用多个
    UOM_Item_UseItem_Failed_7,//当前物品不可以重复使用

    UOM_Item_AcceptItem_Sucess,
    UOM_Item_AcceptItem_Failed,
    UOM_Item_AcceptItem_Failed_1,//赠送数量不可以为0
    UOM_Item_AcceptItem_Failed_2,//赠送物品不存在
    UOM_Item_AcceptItem_Failed_3,//赠送数量不够
    UOM_Item_AcceptItem_Failed_4,//物品类型不允许赠送
    UOM_Item_AcceptItem_Failed_5,//系统错误
    UOM_Item_AcceptItem_Failed_6,//指定好友不存在

    //邮件
    UOM_Mail_GetNormalContext_Sucess,
    UOM_Mail_GetNormalContext_Failed,

    UOM_Mail_GetSystemContext_Sucess,
    UOM_Mail_GetSystemContext_Failed,

    UOM_Mail_SendMail_Sucess,
    UOM_Mail_SendMail_Failed,
    UOM_Mail_SendMail_Failed_1,//邮件内容长度问题
    UOM_Mail_SendMail_Failed_2,//不可以给自己发送
    UOM_Mail_SendMail_Failed_3,//目标邮箱满了

    UOM_Mail_DelMail_Sucess,
    UOM_Mail_DelMail_Failed,
    UOM_Mail_DelMail_Failed_1,//邮件删除失败 目标邮件不存在

    UOM_Mail_GetSystemMailItem_Sucess,
    UOM_Mail_GetSystemMailItem_Failed,
    UOM_Mail_GetSystemMailItem_Failed_1, //邮件不存在
    UOM_Mail_GetSystemMailItem_Failed_2,//邮件物品不存在
    UOM_Mail_GetSystemMailItem_Failed_3,//邮件物品已经被领取
    UOM_Mail_GetSystemMailItem_Failed_4,//系统错误

    //聊天
    UOM_Message_SendMessage_Sucess,
    UOM_Message_SendMessage_Failed,
    UOM_Message_SendMessage_Failed_1,//内容错误
    UOM_Message_SendMessage_Failed_2,//类型错误

    //比赛
    UOM_Month_SignUpMonth_Sucess,
    UOM_Month_SignUpMonth_Failed,
    UOM_Month_SignUpMonth_Failed_1,//指定比赛不存在
    UOM_Month_SignUpMonth_Failed_2,//比赛不在报名时间
    UOM_Month_SignUpMonth_Failed_3,//比赛已经报名了
    UOM_Month_SignUpMonth_Failed_4,//比赛 货币不够
    UOM_Month_SignUpMonth_Failed_5,//比赛 门票不够
    UOM_Month_SignUpMonth_Failed_6,//比赛 桌子不存在
    UOM_Month_SignUpMonth_Failed_7,//比赛 系统错误
    UOM_Month_SignUpMonth_Failed_8,//比赛 等级不够 无法开始比赛
    UOM_Month_SignUpMonth_Failed_9,//比赛 最低倍率 不符合

    UOM_Month_JoinMonth_Sucess,
    UOM_Month_JoinMonth_Failed,
    UOM_Month_JoinMonth_Failed_1,//比赛不存在
    UOM_Month_JoinMonth_Failed_2,//比赛不在进入时间
    UOM_Month_JoinMonth_Failed_3,//比赛未报名
    UOM_Month_JoinMonth_Failed_4,//不符合比赛桌子条件
    UOM_Month_JoinMonth_Failed_5,//比赛 系统错误
    UOM_Month_JoinMonth_Failed_6,//比赛 等级不够

    UOM_Month_AddMonthGlobel_Sucess,
    UOM_Month_AddMonthGlobel_Failed,
    UOM_Month_AddMonthGlobel_Failed_1,//比赛不存在
    UOM_Month_AddMonthGlobel_Failed_2,//续币次数限制
    UOM_Month_AddMonthGlobel_Failed_3,//续币资金不够

    UOM_Month_ResetMonth_Sucess,
    UOM_Month_ResetMonth_Failed,
    UOM_Month_ResetMonth_Failed_1,//比赛不存在
    UOM_Month_ResetMonth_Failed_2,//比赛未报名
    UOM_Month_ResetMonth_Failed_3,//比赛未开始
    UOM_Month_ResetMonth_Failed_4,//比赛 货币不够
    UOM_Month_ResetMonth_Failed_5,//比赛 门票不够
    UOM_Month_ResetMonth_Failed_6,//系统错误

    //在线奖励
    UOM_OnlineReward_GetReward_Sucess,
    UOM_OnlineReward_GetReward_Failed,
    UOM_OnlineReward_GetReward_Failed_1,//在线奖励不存在
    UOM_OnlineReward_GetReward_Failed_2,//奖励不存在
    UOM_OnlineReward_GetReward_Failed_3,//在线时间不够
    UOM_OnlineReward_GetReward_Failed_4,//已经领取了
    UOM_OnlineReward_GetReward_Failed_5,//系统错误

    //运营
    UOM_Operate_RealNameVerification_Sucess,
    UOM_Operate_RealNameVerification_Failed,
    UOM_Operate_RealNameVerification_Failed_1,//名称错误
    UOM_Operate_RealNameVerification_Failed_2,//身份证错误
    UOM_Operate_RealNameVerification_Failed_3,//身份证格式错误
    UOM_Operate_RealNameVerification_Failed_4,//身份证被使用
    UOM_Operate_RealNameVerification_Failed_5,//身份证 与名称不符合
    UOM_Operate_RealNameVerification_Failed_6,//实名验证超时
    UOM_Operate_RealNameVerification_Failed_7,//重复提交

    UOM_Operate_GetPhoneVerificationNum_Sucess,
    UOM_Operate_GetPhoneVerificationNum_Sucess_1,//等待发送验证短信
    UOM_Operate_GetPhoneVerificationNum_Failed,
    UOM_Operate_GetPhoneVerificationNum_Failed_1,//手机号码错误
    UOM_Operate_GetPhoneVerificationNum_Failed_2,//手机号码不存在
    UOM_Operate_GetPhoneVerificationNum_Failed_3,//手机号码被使用了
    UOM_Operate_GetPhoneVerificationNum_Failed_4,//重复提交

    UOM_Operate_BindPhone_Sucess,
    UOM_Operate_BindPhone_Failed,
    UOM_Operate_BindPhone_Failed_1,//验证码错误
    UOM_Operate_BindPhone_Failed_2,//验证码超时
    UOM_Operate_BindPhone_Failed_3,

    UOM_Operate_BindEmail_Sucess,
    UOM_Operate_BindEmail_Failed,
    UOM_Operate_BindEmail_Failed_1, //邮箱格式不正确
    UOM_Operate_BindEmail_Failed_2, //邮箱被使用了
    UOM_Operate_BindEmail_Failed_3, //邮箱不存在

    UOM_Operate_EntityItem_Sucess,
    UOM_Operate_EntityItem_Failed,
    UOM_Operate_EntityItem_Failed_1,//实体物品购买失败 物品不存在

    UOM_Operate_PhonePay_Sucess,
    UOM_Operate_PhonePay_Failed,
    UOM_Operate_PhonePay_Failed_1,
    UOM_Operate_PhonePay_Failed_2,

    //查询
    UOM_Query_QueryName_Sucess,
    UOM_Query_QueryName_Failed,
    UOM_Query_QueryName_Failed_1,//名称错误

    UOM_Query_QueryID_Sucess,
    UOM_Query_QueryID_Failed,
    UOM_Query_QueryID_Failed_1,//不可以查询自己

    //排行榜
    UOM_Rank_GetWeekReward_Sucess,
    UOM_Rank_GetWeekReward_Failed,
    UOM_Rank_GetWeekReward_Failed_1,//排行榜不存在
    UOM_Rank_GetWeekReward_Failed_2,//玩家未上榜
    UOM_Rank_GetWeekReward_Failed_3,//已经领取奖励了

    //充值
    UOM_Recharge_SendRecharge_Sucess,
    UOM_Recharge_SendRecharge_Failed,
    UOM_Recharge_SendRecharge_Failed_1,//商品不存在
    UOM_Recharge_SendRecharge_Failed_2,//RMB不够
    UOM_Recharge_SendRecharge_Failed_3,//已经首冲过了
    UOM_Recharge_SendRecharge_Failed_4,//金币已经到达最大上限

    UOM_Recharge_GetOrderID_Success,
    UOM_Recharge_GetOrderID_Failed,
    UOM_Recharge_GetOrderID_Failed_1,

    //关系
    UOM_Relation_AddRelation_Sucess,
    UOM_Relation_AddRelation_Failed,
    UOM_Relation_AddRelation_Failed_1,//不可以添加自己
    UOM_Relation_AddRelation_Failed_2,//指定关系已经存在
    UOM_Relation_AddRelation_Failed_3,//关系列表满了
    UOM_Relation_AddRelation_Failed_4,//系统错误

    UOM_Relation_DelRelation_Sucess,
    UOM_Relation_DelRelation_Failed,
    UOM_Relation_DelRelation_Failed_1,//指定关系不存在
    UOM_Relation_DelRelation_Failed_2,//系统错误

    UOM_Relation_ChangeRelation_Sucess,
    UOM_Relation_ChangeRelation_Failed,
    UOM_Relation_ChangeRelation_Failed_1,//系统错误

    //商店
    UOM_Shop_BuyItem_Sucess,
    UOM_Shop_BuyItem_Failed,
    UOM_Shop_BuyItem_Failed_1,//指定商品不存在
    UOM_Shop_BuyItem_Failed_2,//指定物品不存在
    UOM_Shop_BuyItem_Failed_3,//商品下架了
    UOM_Shop_BuyItem_Failed_4,//未完善实体信息 不可以购买实体物品
    UOM_Shop_BuyItem_Failed_5,//货币不够
    UOM_Shop_BuyItem_Failed_6,//系统错误
    UOM_Shop_BuyItem_Failed_7,//未绑定手机号码 无法充值话费
    UOM_Shop_BuyItem_Failed_8,//不可以一次购买多个当前物品
    UOM_Shop_BuyItem_Failed_9,//实体物品和手机号码兑换次数超出当天限制
    UOM_Shop_BuyItem_Failed_10,//购买前必须先分享

    //桌子
    UOM_Table_JoinTable_Sucess,
    UOM_Table_JoinTable_Failed,
    UOM_Table_JoinTable_Failed_1,//桌子不存在
    UOM_Table_JoinTable_Failed_2,//金币太少
    UOM_Table_JoinTable_Failed_3,//金币太多
    UOM_Table_JoinTable_Failed_4,//钻石太少
    UOM_Table_JoinTable_Failed_5,//钻石太多
    UOM_Table_JoinTable_Failed_6,//进入桌子门票不够
    UOM_Table_JoinTable_Failed_7,//系统错误
    UOM_Table_JoinTable_Failed_8,//为开启房间最低倍率 
    UOM_Table_JoinTable_Failed_9,//等级太低
    UOM_Table_JoinTable_Failed_10,//等级太高

    //任务
    UOM_Task_GetReward_Sucess,
    UOM_Task_GetReward_Failed,
    UOM_Task_GetReward_Failed_1,//任务不存在
    UOM_Task_GetReward_Failed_2,//任务未完成

    //登陆
    UOM_Logon_NormalLogon_Sucess,
    UOM_Logon_NormalLogon_Failed,
    UOM_Logon_NormalLogon_Failed_1,//账号无法使用
    UOM_Logon_NormalLogon_Failed_2,//密码无法使用
    UOM_Logon_NormalLogon_Failed_3,//密码转化失败
    UOM_Logon_NormalLogon_Failed_4,//密码错误

    UOM_Logon_Register_Sucess,
    UOM_Logon_Register_Failed,
    UOM_Logon_Register_Failed_1,//账号无法使用
    UOM_Logon_Register_Failed_2,//密码无法使用
    UOM_Logon_Register_Failed_3,//密码转化失败
    UOM_Logon_Register_Failed_4,//账号已经存在了

    UOM_Logon_FastRegister_Sucess,
    UOM_Logon_FastRegister_Failed,
    UOM_Logon_FastRegister_Failed_1,//密码无法使用
    UOM_Logon_FastRegister_Failed_2,//密码转化失败
    UOM_Logon_FastRegister_Failed_3,//系统错误

    UOM_Logon_Channel_Sucess,
    UOM_Logon_Channel_Failed,
    UOM_Logon_Channel_Failed_1,//运营平台验证失败

    UOM_PhoneLogon_Check_Success,//手机登陆 获取验证码成功
    UOM_PhoneLogon_Check_Failed,//手机登陆 获取验证码失败
    UOM_PhoneLogon_Check_Failed_1,//手机号码错误
    UOM_PhoneLogon_Check_Failed_2,//手机号码 不存在
    UOM_PhoneLogon_Check_Failed_3,//验证码发送失败
    UOM_PhoneLogon_Check_Failed_4,//系统错误

    UOM_PhoneLog_Success,//登陆成功
    UOM_PhoneLog_Failed,//登陆失败
    UOM_PhoneLog_Failed_1,//手机号码错误
    UOM_PhoneLog_Failed_2,//密码无法使用
    UOM_PhoneLog_Failed_3,//密码转化失败
    UOM_PhoneLog_Failed_4,//验证码错误
    UOM_PhoneLog_Failed_5,//无登陆信息

    UOM_SecPass_Success,
    UOM_SecPass_Failed,
    UOM_SecPass_Failed_1,
    UOM_SecPass_Failed_2,
    UOM_SecPass_Failed_3,

    UOM_ChangeSecPass_Success,
    UOM_ChangeSecPass_Faile,
    UOM_ChangeSecPass_Faile_1,
    UOM_ChangeSecPass_Faile_2,
    UOM_ChangeSecPass_Faile_3,
    UOM_ChangeSecPass_Faile_4,
    UOM_ChangeSecPass_Faile_5,
    UOM_ChangeSecPass_Faile_6,

    UOM_WeiXinLogon_Success,
    UOM_WeiXinLogon_Faile,
    UOM_WeiXinLogon_Faile_1,

    UOM_QQLogon_Success,
    UOM_QQLogon_Faile,
    UOM_QQLogon_Faile_1,

    //登陆里面的特殊的处理
    UOM_Logon_Version_Error,//版本错误
    UOM_Logon_Reset_Error,//重新连接失败
    UOM_Logon_Account_Freeze,//账号被冻结了
    UOM_Logon_Account_Freeze_2,//账号被冻结了

    //称号
    UOM_Title_Sucess,
    UOM_Title_Failed,
    UOM_Title_Failed_1,//称号不存在

    //兑换码
    UOM_ExChange_Sucess,
    UOM_ExChange_Failed,
    UOM_ExChange_Failed_1,//兑换码长度不正确 或者 包含非法字符串
    UOM_ExChange_Failed_2,//兑换码不存在
    UOM_ExChange_Failed_3,//已经使用同类型兑换码
    UOM_ExChange_Failed_4,//当前平台不可以使用兑换码

    //抽奖
    UOM_Lottery_Sucess,
    UOM_Lottery_Failed,
    UOM_Lottery_Failed_1,//指定抽奖不存在
    UOM_Lottery_Failed_2,//抽奖积分不够
    UOM_Lottery_Failed_3,//抽奖 系统错误
    UOM_Lottery_Failed_4,//抽奖 奖金鱼数量不够

    //minigame
    UOM_NiuNiu_JoinRoom_Sucess,
    UOM_NiuNiu_JoinRoom_Failed,
    UOM_NiuNiu_JoinRoom_Failed_1,
    UOM_NiuNiu_JoinRoom_Failed_2,//系统错误

    UOM_NiuNiu_AddGlobel_Sucess,
    UOM_NiuNiu_AddGlobel_Failed,
    UOM_NiuNiu_AddGlobel_Failed_1,
    UOM_NiuNiu_AddGlobel_Failed_2,
    UOM_NiuNiu_AddGlobel_Failed_3,
    UOM_NiuNiu_AddGlobel_Failed_4,//押注金币超出庄家上限
    UOM_NiuNiu_AddGlobel_Failed_5,//系统错误
    UOM_NiuNiu_AddGlobel_Failed_6,//庄家无法下注

    UOM_NiuNiu_AddBankerList_Sucess,
    UOM_NiuNiu_AddBankerList_Failed,
    UOM_NiuNiu_AddBankerList_Failed_1,//金币不够 无法上庄
    UOM_NiuNiu_AddBankerList_Failed_2,//系统错误
    UOM_NiuNiu_AddBankerList_Failed_3,//已经在列表里面了

    UOM_NiuNiu_NextBankerSeat_Sucess,
    UOM_NiuNiu_NextBankerSeat_Failed,
    UOM_NiuNiu_NextBankerSeat_Failed_1,//不在列表里 无法抢庄
    UOM_NiuNiu_NextBankerSeat_Failed_2,//金币不够 无法抢庄
    UOM_NiuNiu_NextBankerSeat_Failed_3,//系统错误
    UOM_NiuNiu_NextBankerSeat_Failed_4,//已经是第一民 

    UOM_NiuNiu_LeaveBankerList_Sucess,
    UOM_NiuNiu_LeaveBankerList_Failed,
    UOM_NiuNiu_LeaveBankerList_Failed_1,//离开失败 不在队列里面
    UOM_NiuNiu_LeaveBankerList_Failed_2,//系统错误

    UOM_NiuNiu_CanelBanker_Sucess,
    UOM_NiuNiu_CanelBanker_Failed,
    UOM_NiuNiu_CanelBanker_Failed_1,//不是庄家
    UOM_NiuNiu_CanelBanker_Failed_2,//系统错误

    UOM_NiuNiu_JoinVip_Sucess,
    UOM_NiuNiu_JoinVip_Failed,
    UOM_NiuNiu_JoinVip_Failed_1,//位置错误
    UOM_NiuNiu_JoinVip_Failed_2,//非VIP玩家
    UOM_NiuNiu_JoinVip_Failed_3,//目标VIP等级高于你
    UOM_NiuNiu_JoinVip_Failed_4,//已经在VIP席位了
    UOM_NiuNiu_JoinVip_Failed_5,//庄家不可以做VIP

    UOM_NiuNiu_LeaveVip_Sucess,
    UOM_NiuNiu_LeaveVip_Failed,
    UOM_NiuNiu_LeaveVip_Failed_1,//不在VIP座位上
    UOM_NiuNiu_LeaveVip_Failed_2,//系统错误

    //Dial

    UOM_Dial_JoinRoom_Sucess,
    UOM_Dial_JoinRoom_Failed,
    UOM_Dial_JoinRoom_Failed_1,//进入金币不够
    UOM_Dial_JoinRoom_Failed_2,//系统错误
        
    UOM_Dial_AddGlobel_Sucess,
    UOM_Dial_AddGlobel_Failed,
    UOM_Dial_AddGlobel_Failed_1,//下注区错误
    //UOM_Dial_AddGlobel_Failed_2,
    UOM_Dial_AddGlobel_Failed_3,//下注金币达到上限
    UOM_Dial_AddGlobel_Failed_4,//押注金币超出庄家上限
    UOM_Dial_AddGlobel_Failed_5,//系统错误
    UOM_Dial_AddGlobel_Failed_6,//庄家无法下注
    UOM_Dial_AddGlobel_Failed_7,//不是下注时间
    UOM_Dial_AddGlobel_Failed_8,//下注金币不可以为空
    UOM_Dial_AddGlobel_Failed_9,//玩家金币不够无法下注
    UOM_Dial_AddGlobel_Failed_10,//非月卡用户不可以续压
        
    UOM_Dial_AddBankerList_Sucess,
    UOM_Dial_AddBankerList_Failed,
    UOM_Dial_AddBankerList_Failed_1,//金币不够 无法上庄
    UOM_Dial_AddBankerList_Failed_2,//系统错误
    UOM_Dial_AddBankerList_Failed_3,//已经在列表里面了
        
    UOM_Dial_NextBankerSeat_Sucess,
    UOM_Dial_NextBankerSeat_Failed,
    UOM_Dial_NextBankerSeat_Failed_1,//不在列表里 无法抢庄
    UOM_Dial_NextBankerSeat_Failed_2,//金币不够 无法抢庄
    UOM_Dial_NextBankerSeat_Failed_3,//系统错误
    UOM_Dial_NextBankerSeat_Failed_4,//已经是第一民 
        
    UOM_Dial_LeaveBankerList_Sucess,
    UOM_Dial_LeaveBankerList_Failed,
    UOM_Dial_LeaveBankerList_Failed_1,//离开失败 不在队列里面
    UOM_Dial_LeaveBankerList_Failed_2,//系统错误
        
    UOM_Dial_CanelBanker_Sucess,
    UOM_Dial_CanelBanker_Failed,
    UOM_Dial_CanelBanker_Failed_1,//不是庄家
    UOM_Dial_CanelBanker_Failed_2,//系统错误
        
    UOM_Dial_JoinVip_Sucess,
    UOM_Dial_JoinVip_Failed,
    UOM_Dial_JoinVip_Failed_1,//位置错误
    UOM_Dial_JoinVip_Failed_2,//非VIP玩家
    UOM_Dial_JoinVip_Failed_3,//目标VIP等级高于你
    UOM_Dial_JoinVip_Failed_4,//已经在VIP席位了
    UOM_Dial_JoinVip_Failed_5,//庄家不可以做VIP
        
    UOM_Dial_LeaveVip_Sucess,
    UOM_Dial_LeaveVip_Failed,
    UOM_Dial_LeaveVip_Failed_1,//不在VIP座位上
    UOM_Dial_LeaveVip_Failed_2,//系统错误

    //RelationRequest
    UOM_RelationRequest_SendRequest_Sucess,
    UOM_RelationRequest_SendRequest_Failed,
    UOM_RelationRequest_SendRequest_Failed_1,//关系类型不正确
    UOM_RelationRequest_SendRequest_Failed_2,//字符串包含非法字符 或者 长度不正确
    UOM_RelationRequest_SendRequest_Failed_3,//和目标玩家已经存在关系
    UOM_RelationRequest_SendRequest_Failed_4,//重复发送申请
    UOM_RelationRequest_SendRequest_Failed_5,//目标已经向你发送相同申请了
    UOM_RelationRequest_SendRequest_Failed_6,//已经和目标玩家是好友了
    UOM_RelationRequest_SendRequest_Failed_7,//目标玩家和你已经是好友了
    UOM_RelationRequest_SendRequest_Failed_8,//目标的好友列表已经满了
    UOM_RelationRequest_SendRequest_Failed_9,//你的好友列表已经满了
    UOM_RelationRequest_SendRequest_Failed_10,//目标玩家申请列表已经满了

    UOM_RelationRequest_HandleRequest_Sucess,
    UOM_RelationRequest_HandleRequest_Failed,
    UOM_RelationRequest_HandleRequest_Failed_1,//ID 不存在

    UOM_RoleChar_SendChar_Sucess,
    UOM_RoleChar_SendChar_Failed,
    UOM_RoleChar_SendChar_Failed_1,//目标不是好友
    UOM_RoleChar_SendChar_Failed_2,//包含非法字符



    //Car

    UOM_Car_JoinRoom_Sucess,
    UOM_Car_JoinRoom_Failed,
    UOM_Car_JoinRoom_Failed_1,//进入金币不够
    UOM_Car_JoinRoom_Failed_2,//系统错误

    UOM_Car_AddGlobel_Sucess,
    UOM_Car_AddGlobel_Failed,
    UOM_Car_AddGlobel_Failed_1,//下注区错误
    //UOM_Car_AddGlobel_Failed_2,
    UOM_Car_AddGlobel_Failed_3,//下注金币达到上限
    UOM_Car_AddGlobel_Failed_4,//押注金币超出庄家上限
    UOM_Car_AddGlobel_Failed_5,//系统错误
    UOM_Car_AddGlobel_Failed_6,//庄家无法下注
    UOM_Car_AddGlobel_Failed_7,//不是下注时间
    UOM_Car_AddGlobel_Failed_8,//下注金币不可以为空
    UOM_Car_AddGlobel_Failed_9,//玩家金币不够无法下注

    UOM_Car_AddBankerList_Sucess,
    UOM_Car_AddBankerList_Failed,
    UOM_Car_AddBankerList_Failed_1,//金币不够 无法上庄
    UOM_Car_AddBankerList_Failed_2,//系统错误
    UOM_Car_AddBankerList_Failed_3,//已经在列表里面了

    UOM_Car_NextBankerSeat_Sucess,
    UOM_Car_NextBankerSeat_Failed,
    UOM_Car_NextBankerSeat_Failed_1,//不在列表里 无法抢庄
    UOM_Car_NextBankerSeat_Failed_2,//金币不够 无法抢庄
    UOM_Car_NextBankerSeat_Failed_3,//系统错误
    UOM_Car_NextBankerSeat_Failed_4,//已经是第一民 

    UOM_Car_LeaveBankerList_Sucess,
    UOM_Car_LeaveBankerList_Failed,
    UOM_Car_LeaveBankerList_Failed_1,//离开失败 不在队列里面
    UOM_Car_LeaveBankerList_Failed_2,//系统错误

    UOM_Car_CanelBanker_Sucess,
    UOM_Car_CanelBanker_Failed,
    UOM_Car_CanelBanker_Failed_1,//不是庄家
    UOM_Car_CanelBanker_Failed_2,//系统错误

    UOM_Car_JoinVip_Sucess,
    UOM_Car_JoinVip_Failed,
    UOM_Car_JoinVip_Failed_1,//位置错误
    UOM_Car_JoinVip_Failed_2,//非VIP玩家
    UOM_Car_JoinVip_Failed_3,//目标VIP等级高于你
    UOM_Car_JoinVip_Failed_4,//已经在VIP席位了
    UOM_Car_JoinVip_Failed_5,//庄家不可以做VIP

    UOM_Car_LeaveVip_Sucess,
    UOM_Car_LeaveVip_Failed,
    UOM_Car_LeaveVip_Failed_1,//不在VIP座位上
    UOM_Car_LeaveVip_Failed_2,//系统错误
}

public class FishDataInfo
{
    public const int AccountNameLength = 32;
    public const int AccountNameMinLength = 4;

    public const int PasswordLength = 32;
    public const int PasswordMinLength = 3;

    public const int NickNameLength = 8;
    public const int NickNameMinLength = 2;

    //public const int MaxAdderLength = 32;
    //public const int MinAdderLength = 1;

    public const int MaxMailContexeLength = 128;
    public const int MinMailContexeLength = 1;

   // public const int MaxMailItemSum = 3;
    public const int MAX_CHARM_ITEMSUM = 8;

    public const int MAX_NAME_LENTH = 10;
    public const int MIN_NAME_LENTH = 2;

    public const int MAX_ADDRESS_LENGTH = 128;
    public const int MIN_ADDRESS_LENGTH = 1;

    //public const int MAX_PHONE_LENGTH = 32;
    //public const int MIN_PHONE_LENGTH = 8;

    public const int MAX_IDENTITY_LENGTH = 32;
    public const int MIN_IDENTITY_LENGTH = 15;

    public const int MAX_EMAIL_LENGTH = 64;
    public const int MIN_EMAIL_LENGTH = 12;

    public const int MAX_ADDRESS_LENGTH_IP = 16;

    public const int MAX_MESSAGE_LENGTH = 128;
    public const int MIN_MESSAGE_LENGTH = 1;
    //public const int MAX_Announcement_Length = 64;
    //public const int VersionID = 100001;

    public const Byte MsgBegin = 1;
    public const Byte MsgEnd = 2;

    public const int RMB_OnceOnlyID_Length =32;

    public const int ExChangeCode_Length = 16;
    public const int ExChangeCode_MinLength = 16;

    public const int MacAddressLength = 56;

    public const int  MAX_NIUNIU_ClientSum =4; //客户端下注区的数量
    public const int MAX_NIUNIU_BrandSum = 5; //一次发牌的数量
    public const int MAX_NIUNIU_BankerShowSum = 8;
    public const int MAX_NIUNIU_VipSeat = 8;
    public const int MAX_NORMAL_PAGESUM = 9;

    public const int MAX_Dial_ClientSum = 12; //客户端下注区的数量
    public const int MAX_Dial_GameSum = 24;
    public const int MAX_Dial_BankerShowSum = 8;
    public const int MAX_Dial_VipSeat = 8;
    public const int MAX_DialNORMAL_PAGESUM = 9;

    public const int MAX_Car_ClientSum = 8; //客户端下注区的数量
    //public const int MAX_Car_GameSum = 16;
    public const int MAX_Car_BankerShowSum = 8;
    public const int MAX_Car_VipSeat = 8;
    public const int MAX_CarNORMAL_PAGESUM = 9;
    

    public const int MAX_MonthList_Length = 4;

    public const int MAX_ROLECHAR_LENGTH = 32;

    public const int MAX_RelationRequest_MsgLength = 32;

    public static bool IsCanUsePhone(UInt64 PhoneNumber)
    {
        // 11位
        //10000000000
        //99999999999
        if (PhoneNumber >= 10000000000 && PhoneNumber <= 99999999999)
            return true;
        else
            return false;
    }
}

public class NetCmdKNDetateSocket : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwSendTickCount;
    [TypeInfo(1)]
    public UInt32 dwRecvTickCount;//IP
}


//Balanced
public class NetCmdBalancedSend : NetCmdBase
{
}
public class NetCmdBalancedRecv : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public UInt32 Ip;
    [TypeInfo(2)]
    public UInt16 Port;
}
//Logon
public class CL_Cmd_AccountLogon : NetCmdBase
{
    [TypeInfo(0,FishDataInfo.AccountNameLength+1)]
	public string	AccountName;
	[TypeInfo(1)]
    public UInt32	PasswordCrc1;
	[TypeInfo(2)]
    public UInt32	PasswordCrc2;
	[TypeInfo(3)]
    public UInt32	PasswordCrc3;
    [TypeInfo(4,FishDataInfo.MacAddressLength+1)]
    public string   MacAddress;
    [TypeInfo(5)]
    public Byte PlateFormID;//平台的ID
    [TypeInfo(6)]
    public UInt32 VersionID;
    [TypeInfo(7)]
    public UInt32 PathCrc;
}
public class CL_Cmd_AccountRsg : NetCmdBase
{
	[TypeInfo(0,FishDataInfo.AccountNameLength+1)]
	public string	AccountName;
	[TypeInfo(1)]
    public UInt32	PasswordCrc1;
	[TypeInfo(2)]
    public UInt32	PasswordCrc2;
	[TypeInfo(3)]
    public UInt32	PasswordCrc3;
    //[TypeInfo(4,FishDataInfo.NickNameLength+1)]
    //public string	NickName;
    //[TypeInfo(5)]
    //public bool     Gender;
    [TypeInfo(4, FishDataInfo.MacAddressLength + 1)]
    public string MacAddress;
    [TypeInfo(5)]
    public Byte PlateFormID;//平台的ID
    [TypeInfo(6)]
    public UInt32 VersionID;
    [TypeInfo(7)]
    public UInt32 PathCrc;
}
//public class CL_Cmd_AccountQueryRsg : NetCmdBase
//{
//    [TypeInfo(0)]
//    public UInt32 PasswordCrc1;
//    [TypeInfo(1)]
//    public UInt32 PasswordCrc2;
//    [TypeInfo(2)]
//    public UInt32 PasswordCrc3;
//    //[TypeInfo(4,FishDataInfo.NickNameLength+1)]
//    //public string	NickName;
//    //[TypeInfo(5)]
//    //public bool     Gender;
//    [TypeInfo(3)]
//    public Byte PlateFormID;//平台的ID
//    [TypeInfo(4)]
//    public UInt32 VersionID;
//    [TypeInfo(5)]
//    public UInt32 PathCrc;
//}
public class CL_Cmd_GetNewAccount : NetCmdBase
{
	[TypeInfo(0)]
    public UInt32	PasswordCrc1;
	[TypeInfo(1)]
    public UInt32	PasswordCrc2;
	[TypeInfo(2)]
    public UInt32	PasswordCrc3;
    [TypeInfo(3, FishDataInfo.MacAddressLength + 1)]
    public string MacAddress;
	[TypeInfo(4)]
    public Byte	PlateFormID;//平台的ID
	[TypeInfo(5)]
    public UInt32   VersionID;
	[TypeInfo(6)]
    public UInt32	PathCrc;
};
public class LC_Cmd_GetNewAccount : NetCmdBase
{
	[TypeInfo(0)]
    public bool	Result;
    [TypeInfo(1, FishDataInfo.AccountNameLength + 1)]
    public string	NewAccountName;
	[TypeInfo(2)]
    public UInt32	PasswordCrc1;
	[TypeInfo(3)]
    public UInt32	PasswordCrc2;
	[TypeInfo(4)]
    public UInt32	PasswordCrc3;
};
public class LC_Cmd_CheckVersionError : NetCmdBase
{
	[TypeInfo(0)]
    public UInt32 VersionID;
	[TypeInfo(1)]
    public UInt32 PathCrc;
};
public class LC_Cmd_AccountIsFreeze: NetCmdBase
{
    [TypeInfo(0)]
    public Int64 EndTime;
}

public class LC_Cmd_AccountOnlyID : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public UInt32 dwOnlyID;
    [TypeInfo(2)]
    public UInt32 GateIp;
    [TypeInfo(3)]
    public UInt16 GatePort;
    [TypeInfo(4)]
    public UInt32 GameIp;
    [TypeInfo(5)]
    public UInt16 GamePort;
    [TypeInfo(6)]
    public UInt32 HallTimeOut;
    [TypeInfo(7)]
    public Byte   LogonTypeID;//1为登陆 2为注册
}
public class CL_Cmd_AccountOnlyID : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public UInt32 dwOnlyID;
    [TypeInfo(2)]
    public Byte PlateFormID;//玩家平台ID
    [TypeInfo(3)]
    public UInt32 ScreenPoint;//玩家屏幕分辨率
    [TypeInfo(4, FishDataInfo.MacAddressLength + 1)]
    public string MacAddress;
}
public class LC_Cmd_AccountOnlyIDFailed : NetCmdBase
{
}
public class LC_Cmd_AccountOnlyIDSuccess : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 RandID;//客户端唯一的ID
    [TypeInfo(1)]
    public UInt32 OperateIP;
    [TypeInfo(2)]
    public tagRoleInfo RoleInfo;//玩家数据
}
public class LC_Cmd_AccountLogonToScreen: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 RandID;//客户端唯一的ID
    //[TypeInfo(1)]
    //public tagRoleInfo RoleInfo;//玩家数据
    [TypeInfo(1)]
    public Byte bTableTypeID;
    [TypeInfo(2)]
    public Byte SeatID;
    [TypeInfo(3)]
    public Byte LauncherType;
    [TypeInfo(4)]
    public Byte BackgroundImage;
    [TypeInfo(5)]
    public Byte RateIndex;
    [TypeInfo(6)]
    public Int32 Energy;
}
public class LC_Cmd_ServerChangeSocket : NetCmdBase
{
}
public class CL_Cmd_ResetPassword: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 OldPasswordCrc1;
    [TypeInfo(1)]
    public UInt32 OldPasswordCrc2;
    [TypeInfo(2)]
    public UInt32 OldPasswordCrc3;
    [TypeInfo(3)]
    public UInt32 NewPasswordCrc1;
    [TypeInfo(4)]
    public UInt32 NewPasswordCrc2;
    [TypeInfo(5)]
    public UInt32 NewPasswordCrc3;
}
public class LC_Cmd_ResetPassword : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public UInt32 NewPasswordCrc1;
    [TypeInfo(2)]
    public UInt32 NewPasswordCrc2;
    [TypeInfo(3)]
    public UInt32 NewPasswordCrc3;
}
public class  CL_Cmd_AccountResetAccount : NetCmdBase
{
	[TypeInfo(0,FishDataInfo.AccountNameLength+1)]
    public string	NewAccountName;
	[TypeInfo(1)]
    public UInt32	PasswordCrc1;
	[TypeInfo(2)]
    public UInt32	PasswordCrc2;
	[TypeInfo(3)]
    public UInt32	PasswordCrc3;
};
public class  LC_Cmd_AccountResetAccount : NetCmdBase
{
    [TypeInfo(0)]
    public bool     Result;
};
public class LC_Cmd_LogonQueueWrite : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 WriteIndex;
    [TypeInfo(1)]
    public UInt32 WriteSec;
};


public class CL_Cmd_ResetLogonGameServer: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 UserID;
    [TypeInfo(1)]
    public UInt32 RandID;
    [TypeInfo(2)]
    public UInt32 VersionID;
    [TypeInfo(3)]
    public UInt32 PathCrc;
}
public class LC_Cmd_ResetLogonGameServer : NetCmdBase
{
} 
    
public class int256
{
    [TypeInfo(0, 32)]
    public Byte[] Value;
}
public class int256Function
{
    public static void Clear(int256 p)
    {
        for (int i = 0; i < p.Value.Length; ++i)
            p.Value[i] = 0;
    }
    public static void SetBitStates(int256 p, Byte BitIndex, bool States)
    {
        Byte ArrayIndex = Convert.ToByte(BitIndex / 8);
        Byte ChangeIndex = Convert.ToByte(BitIndex %8);
        Byte BitValue = Convert.ToByte(1 << ChangeIndex);
        if (States)
            p.Value[ArrayIndex] |= BitValue;
        else
            p.Value[ArrayIndex] ^= BitValue;
    }
    public static bool GetBitStates(int256 p, Byte BitIndex)
    {
        Byte ArrayIndex = Convert.ToByte(BitIndex / 8);
        Byte ChangeIndex = Convert.ToByte(BitIndex % 8);
        return ((p.Value[ArrayIndex] & (1 << ChangeIndex)) == (1 << ChangeIndex));
    }
}

public class tagRoleInfo
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1, FishDataInfo.NickNameLength + 1)]
    public string NickName;
    [TypeInfo(2)]
    public UInt16 wLevel;//玩家点击
    [TypeInfo(3)]
    public UInt32 dwExp;//玩家经验
    [TypeInfo(4)]
    public UInt32 dwFaceID;//玩家头像ID
    [TypeInfo(5)]
    public bool bGender; //玩家性别
    [TypeInfo(6)]
    public UInt32 dwGlobeNum;
    [TypeInfo(7)]
    public UInt32 dwMedalNum;
    [TypeInfo(8)]
    public UInt32 dwCurrencyNum;  //钻石
    [TypeInfo(9)]
    public UInt32 dwAchievementPoint;
    [TypeInfo(10)]
    public Byte TitleID;
    [TypeInfo(11)]
    public UInt32 dwProduction;//当天获得的金币数量
    [TypeInfo(12)]
    public UInt32 dwGameTime;//当天的游戏时间
    [TypeInfo(13)]
    public Byte SendGiffSum;//发送赠送的次数 当天
    [TypeInfo(14)]
    public Byte AcceptGiffSum;//接收赠送的次数 当天
    [TypeInfo(15)]
    public int256 TaskStates;
    [TypeInfo(16)]
    public int256 AchievementStates;
    [TypeInfo(17)]
    public int256 ActionStates;
    [TypeInfo(18)]
    public UInt16 OnlineMin;
    [TypeInfo(19)]
    public UInt32 OnlineRewardStates;
    [TypeInfo(20, FishDataInfo.MAX_CHARM_ITEMSUM)]
    public UInt32[] CharmArray;
    [TypeInfo(21)]
    public bool IsCanResetAccount;
    [TypeInfo(22)]
    public UInt32 AchievementPointIndex;
    [TypeInfo(23)]
    public UInt32 ClientIP;
    [TypeInfo(24, FishDataInfo.MAX_ADDRESS_LENGTH_IP+1)]
    public string IPAddress;
    [TypeInfo(25)]
    public UInt32 CheckData;
    [TypeInfo(26)]
    public bool IsShowIPAddress;//默认值为true
    [TypeInfo(27)]
    public UInt32 ExChangeStates;
    [TypeInfo(28)]
    public UInt32 TotalRechargeSum;
    [TypeInfo(29)]
    public bool bIsFirstPayGlobel;
    [TypeInfo(30)]
    public bool bIsFirstPayCurrcey;
    [TypeInfo(31)]
    public UInt32 LotteryScore;
    [TypeInfo(32)]
    public Byte LotteryFishSum;
    //Vip数据
    [TypeInfo(33)]
    public Byte VipLevel;//不涉及数据库的
    //月卡数据
    [TypeInfo(34)]
    public Byte MonthCardID;
    [TypeInfo(35)]
    public Int64 MonthCardEndTime;
    [TypeInfo(36)]
    public Int64 GetMonthCardRewardTime;
    //倍率
    [TypeInfo(37)]
    public int256 RateValue;//开启的倍率的数值
    [TypeInfo(38)]
    public byte CashSum;
    [TypeInfo(39)]
    public Byte benefitCount;//
    [TypeInfo(40)]
    public UInt32 benefitTime;//
    [TypeInfo(41)]
    public UInt32 TotalUseMedal;//
    [TypeInfo(42)]
    public UInt32 ParticularStates;//
    [TypeInfo(43)]
    public UInt32 GameID;//
    [TypeInfo(44)]
    public bool bShareStates;//
    [TypeInfo(45)]
    public UInt32 TotalCashSum;//
    // [TypeInfo(41)]
    //public bool bpoor;//
}
//public class NetCmdLogonAccount : NetCmdBase
//{
//    [TypeInfo(0)]
//    public UInt32 VesionID;
//    [TypeInfo(1, FishDataInfo.AccountNameLength + 1)]
//    public string AccountName;
//    [TypeInfo(2, FishDataInfo.PasswordLength + 1)]
//    public string Password;
//}
//public class NetCmdLogonAccountResult : NetCmdBase
//{
//    [TypeInfo(0)]
//    public tagRoleInfo RoleInfo;
//}
//public class NetCmdLogonAccountError : NetCmdBase
//{

//}
//public class NetCmdLogonVersionError : NetCmdBase
//{

//}
//public class NetCmdLogonRsgAccount : NetCmdBase
//{
//    [TypeInfo(0)]
//    public UInt32 VesionID;
//    [TypeInfo(1, FishDataInfo.AccountNameLength + 1)]
//    public string AccountName;
//    [TypeInfo(2, FishDataInfo.PasswordLength + 1)]
//    public string Password;
//    [TypeInfo(3, FishDataInfo.MacAdderLength + 1)]
//    public string MacAdder;
//    [TypeInfo(4, FishDataInfo.NickNameLength + 1)]
//    public string NickName;
//    [TypeInfo(5)]
//    public bool bGender;
//}
//public class NetCmdLogonRsgAccountError : NetCmdBase
//{

//}
//public class NetCmdLogonQuery : NetCmdBase
//{
//    [TypeInfo(0)]
//    public UInt32 VesionID;
//    //[TypeInfo(1, FishDataInfo.MacAdderLength+1)]
//    //public string MacAdder;
//}
//public class NetCmdLogonQueryError : NetCmdBase
//{

//}
//public class LC_Cmd_QueryAccount : NetCmdBase
//{
//    [TypeInfo(0, FishDataInfo.AccountNameLength + 1)]
//    public string Account;//用户名
//    [TypeInfo(1, FishDataInfo.PasswordLength + 1)]
//    public string Password;//密码
//}
//public class CL_Cmd_QueryRsg : NetCmdBase
//{
//    [TypeInfo(0)]
//    public UInt32 VersionID;
//    [TypeInfo(1, FishDataInfo.AccountNameLength + 1)]
//    public string Account;//用户名
//    [TypeInfo(2, FishDataInfo.PasswordLength + 1)]
//    public string Password;//密码
//    [TypeInfo(3, FishDataInfo.AccountNameLength + 1)]
//    public string QueryAccount;//用户名
//    [TypeInfo(4, FishDataInfo.PasswordLength + 1)]
//    public string QueryPassword;//密码
//}
//public class LC_Cmd_QueryRsgError : NetCmdBase
//{

//}
public class LC_Cmd_DayChange : NetCmdBase
{
    [TypeInfo(0)]
    public Byte Year;
    [TypeInfo(1)]
    public Byte Month;
    [TypeInfo(2)]
    public Byte Day;
    [TypeInfo(3)]
    public Byte Hour;
    [TypeInfo(4)]
    public Byte Min;
    [TypeInfo(5)]
    public Byte Sec;
    [TypeInfo(6)]
    public bool IsDayUpdate;
}
public class LC_Cmd_ChangeRoleAchievementIndex : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 AchievementIndex;
};
public class LC_Cmd_ResetRoleGlobel : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 TotalGlobelSum;
    [TypeInfo(1)]
    public UInt32 LotteryScore;
}
public class CL_Cmd_ResetRoleInfo: NetCmdBase
{

}
public class LC_Cmd_ResetRoleInfo: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 OperateIP;
    [TypeInfo(1)]
    public tagRoleInfo RoleInfo;
}
public class LC_Cmd_ChangeRoleCheckData: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 CheckData;
}
public class CL_Cmd_ChangeRoleIsShowIpAddress: NetCmdBase
{
    [TypeInfo(0)]
    public bool IsShowIpAddress;
}
public class LC_Cmd_ChangeRoleIsShowIpAddress: NetCmdBase
{
    [TypeInfo(0)]
    public bool IsShowIpAddress;
}
public class LC_Cmd_NewDay : NetCmdBase
{

}
public class LC_Cmd_ChangeRoleExChangeStates : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 States;
}
public class LC_Cmd_ChangeRoleTotalRecharge : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 Sum;
}
public class LC_Cmd_RoleProtect : NetCmdBase
{
    //[TypeInfo(0)]
    //public byte nCount;
    //[TypeInfo(1)]
    //public ushort ncd;
}
public class  CL_Cmd_RoleProtect :  NetCmd
{
};
public class LC_Cmd_ChangeRoleIsFirstPayGlobel : NetCmdBase
{

}
public class LC_Cmd_ChangeRoleIsFirstPayCurrcey : NetCmdBase
{

}
public class LC_Cmd_ChangeRoleParticularStates : NetCmdBase
{
	[TypeInfo(0)]
    public UInt32		ParticularStates;
};
public class LC_Cmd_ChangeRoleMonthCardInfo: NetCmdBase
{
    [TypeInfo(0)]
    public Byte MonthCardID;
    [TypeInfo(1)]
    public Int64 MonthCardEndTime;
}
public class LC_Cmd_ChangeRoleRateValue : NetCmdBase
{
    [TypeInfo(0)]
    public int256 RateValue;
    [TypeInfo(1)]
    public Byte OpenRateIndex;
}
public class CL_Cmd_ChangeRoleRateValue : NetCmdBase
{
    [TypeInfo(0)]
    public Byte RateIndex;
}
public class CL_Cmd_GetMonthCardReward: NetCmdBase
{

}
public class LC_Cmd_GetMonthCardReward : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class LC_Cmd_ChangeRoleVipLevel : NetCmdBase
{
    [TypeInfo(0)]
    public Byte VipLevel;
}
public class LC_Cmd_ChangeRoleCashSum: NetCmdBase
{
    [TypeInfo(0)]
    public Byte CashSum;
    [TypeInfo(1)]
    public UInt32 TotalCashSum;
}

public class LC_Cmd_SetSecondPassword :NetCmdBase
{
	//让玩家查询设置二级密码
};
public class CL_Cmd_ChangeSecondPassword : NetCmdBase
{
	[TypeInfo(0)]
    public UInt32	dwOldCrc1;
    [TypeInfo(1)]
    public UInt32 dwOldCrc2;
    [TypeInfo(2)]
    public UInt32 dwOldCrc3;
    [TypeInfo(3)]
    public UInt32 dwNewCrc1;
    [TypeInfo(4)]
    public UInt32 dwNewCrc2;
    [TypeInfo(5)]
    public UInt32 dwNewCrc3;
};
public class LC_Cmd_ChangeSecondPassword : NetCmdBase
{
	[TypeInfo(0)]
    public bool	Result;
};

public class CL_Cmd_ChangeRoleShareStates : NetCmdBase
{
    [TypeInfo(0)]
    public bool ShareStates;
};

public class LC_Cmd_ChangeRoleShareStates : NetCmdBase
{
    [TypeInfo(0)]
    public bool ShareStates;
};

public class LC_Cmd_OpenShareUI : NetCmdBase
{
};


public class AccountSDKInfo
{
	//CHAR			Token[xx];//或者为 OpenID
	[TypeInfo(0)]
    public Byte				    HandleSum;
	[TypeInfo(1)]
    public UInt32				Sum;
	[TypeInfo(2,true)]
    public Byte[]				Array;
};

public class CL_Cmd_UpdateAccount : NetCmdBase
{
    [TypeInfo(0)]
    public Byte			    bType;
    [TypeInfo(1)]
    public AccountSDKInfo   Info;
};


//桌子
public class tagRemoteRoleInfo
{
    [TypeInfo(0)]
    public Byte SeatID;
    [TypeInfo(1)]
    public UInt32 dwUserID;
    [TypeInfo(2, FishDataInfo.NickNameLength + 1)]
    public string NickName;
    [TypeInfo(3)]
    public UInt16 wLevel;//玩家点击
    //[TypeInfo(4)]
    //public UInt32 dwExp;//玩家经验
    [TypeInfo(4)]
    public UInt32 dwFaceID;//玩家头像ID
    [TypeInfo(5)]
    public bool bGender; //玩家性别
    [TypeInfo(6)]
    public UInt32 dwGlobeNum; //玩家金币
    [TypeInfo(7, FishDataInfo.MAX_CHARM_ITEMSUM)]
    public UInt32[] CharmArray;
    [TypeInfo(8)]
    public UInt32 dwAchievementPoint;
    [TypeInfo(9)]
    public Byte TitleID;
    [TypeInfo(10, FishDataInfo.MAX_ADDRESS_LENGTH_IP+1)]
    public string	IPAddress;
    [TypeInfo(11)]
    public Byte VipLevel;
    [TypeInfo(12)]
    public bool IsInMonthCard;
    [TypeInfo(13)]
    public UInt32 GameID;
};
public class CL_Cmd_JoinTable : NetCmdBase
{
    [TypeInfo(0)]
    public Byte bTableType;
}
public class LC_Cmd_JoinTableResult : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public Byte bTableTypeID;
    [TypeInfo(2)]
    public Byte SeatID;
    [TypeInfo(3)]
    public Byte LauncherType;
    [TypeInfo(4)]
    public Byte BackgroundImage;
    [TypeInfo(5)]
    public Byte RateIndex;
    [TypeInfo(6)]
    public int Energy;
}
public class CL_Cmd_LeaveTable : NetCmdBase
{
}
public class LC_Cmd_OtherUserInfo : NetCmdBase
{
    [TypeInfo(0)]
    public tagRemoteRoleInfo UserInfo;
}
public class LC_Cmd_MeSeatID : NetCmdBase
{
    [TypeInfo(0)]
    public Byte SeatID;
};
public class LC_Cmd_OtherUserLeave : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
}
public class LC_Cmd_TableChangeRoleLevel : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public UInt16 wLevel;
}
public class LC_Cmd_TableChangeRoleGender : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public bool bGender;
}
public class LC_Cmd_TableChangeRoleNickName : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1, FishDataInfo.NickNameLength + 1)]
    public string NickName;
}
public class LC_Cmd_TableChangeRoleFaceID : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public UInt32 dwDestFaceID;
}
public class LC_Cmd_TableChangeRoleGlobe : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Int32 dwGlobelSum;
}
public class LC_Cmd_TableChangeRoleTitle : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Byte TitleID;
}
public class LC_Cmd_TableChangeRoleAchievementPoint : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public UInt32 dwAchievementPoint;
}
//public class LC_Cmd_TableChangeRoleExp : NetCmdBase
//{
//    [TypeInfo(0)]
//    public UInt32 dwDestUserID;
//    [TypeInfo(1)]
//    public UInt32 dwExp;
//}
//public class tagRoleCharmInfo
//{
//    [TypeInfo(0, FishDataInfo.MAX_CHARM_ITEMSUM)]
//    public Byte[] CharmArray;
//};
public class LC_Cmd_TableChangeRoleCharmValue : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1, FishDataInfo.MAX_CHARM_ITEMSUM)]
    public UInt32[] CharmArray;
}
public class LC_Cmd_TableChangeRoleIpAddress: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32   dwDestUserID;
	[TypeInfo(1, FishDataInfo.MAX_ADDRESS_LENGTH_IP+1)]
    public string IpAddress;
}
public class LC_Cmd_TableChangeRoleVipLevel: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Byte VipLevel;
}
public class LC_Cmd_TableChangeRoleIsInMonthCard: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public bool IsInMonthCard;
}
public class LC_Cmd_ResetOtherUserInfo: NetCmdBase
{
    [TypeInfo(0)]
    public tagRemoteRoleInfo UserInfo;
}
public class LC_Cmd_LeaveTableByServer: NetCmdBase
{
    [TypeInfo(0)]
    public bool IsReturnLogon;
}
//物品
public class tagItemOnce
{
    [TypeInfo(0)]
    public UInt32 ItemID;   //物品ID
    [TypeInfo(1)]
    public UInt32 ItemSum;  //物品数量
    [TypeInfo(2)]
    public UInt32 LastMin;	//物品持续分钟 0表示无限
}
public class tagItemInfo//玩家具体的数据
{
    [TypeInfo(0)]
    public UInt32 ItemOnlyID;//物品的唯一ID  主键ID
    [TypeInfo(1)]
    public UInt32 ItemID;//物品ID
    [TypeInfo(2)]
    public UInt32 ItemSum;//物品数量
    [TypeInfo(3)]
    public Int64 EndTime;//物品结束时间
};
public class CL_Cmd_GetUserItem : NetCmdBase
{
}
public class LC_Cmd_GetUserItem : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagItemInfo[] Array;
}
//public class LC_Cmd_GetUserItemFinish : NetCmdBase
//{
//}
public class LC_Cmd_AddUserItem : NetCmdBase
{
    [TypeInfo(0)]
    public tagItemInfo ItemInfo;
}
public class LC_Cmd_DelUserItem : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 ItemOnlyID;
}
public class LC_Cmd_ChangeUserItem : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 ItemOnlyID;
    [TypeInfo(1)]
    public UInt32 ItemSum;
};
public class CL_Cmd_OnUseItem: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 ItemOnlyID;
    [TypeInfo(1)]
    public UInt32 ItemID;
    [TypeInfo(2)]
    public UInt32 ItemSum;
}
public class LC_Cmd_OnUseItem: NetCmdBase
{
    [TypeInfo(0)]
    public bool   Result;
    [TypeInfo(1)]
    public UInt32 ItemOnlyID;
    [TypeInfo(2)]
    public UInt32 ItemID;
    [TypeInfo(3)]
    public UInt32 ItemSum;
}
public class CL_Cmd_OnAcceptItem: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwID;
    [TypeInfo(1)]
    public UInt32 ItemOnlyID;
    [TypeInfo(2)]
    public UInt32 ItemID;
    [TypeInfo(3)]
    public UInt32 ItemSum;
    [TypeInfo(4)]
    public bool GameIDorUserID;
}
public class LC_Cmd_OnAcceptItem : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public UInt32 dwDestUserID;
    [TypeInfo(2)]
    public UInt32 ItemOnlyID;
    [TypeInfo(3)]
    public UInt32 ItemID;
    [TypeInfo(4)]
    public UInt32 ItemSum;
}
//Relation--------------------------------------------------------------------
public class tagRoleRelationClient//用户客户端关系
{
    [TypeInfo(0)]
    public UInt32 dwDestRoleID;
    [TypeInfo(1)]
    public UInt32 dwFaceID;
    [TypeInfo(2)]
    public UInt16 wLevel;
    [TypeInfo(3)]
    public bool bGender;
    [TypeInfo(4)]
    public Byte bDiffDay;//在线天数差 255表示玩家在线  其他都表示不在线的天数 绝对小于250
    [TypeInfo(5, FishDataInfo.NickNameLength + 1)]
    public string DestNickName;
    [TypeInfo(6)]
    public Byte bRelationType;//关系类型
    [TypeInfo(7)]
    public Byte TitleID;
    [TypeInfo(8)]
    public UInt32 dwAchievementPoint;
    [TypeInfo(9, FishDataInfo.MAX_CHARM_ITEMSUM)]
    public UInt32[] CharmArray;
    [TypeInfo(10, FishDataInfo.MAX_ADDRESS_LENGTH_IP+1)]
    public string IPAddress;
    [TypeInfo(11)]
    public Byte VipLevel;
    [TypeInfo(12)]
    public bool IsInMonthCard;
    [TypeInfo(13)]
    public UInt32 GameID;
}
public class CL_Cmd_GetUserRelation : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwRoleID;
}
public class LC_Cmd_GetUserRelation : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagRoleRelationClient[] Array;
}
//public class LC_Cmd_GetUserRelationFinish : NetCmdBase
//{
//    //[TypeInfo(0)]
//    //public UInt32 dwRoleID;
//}
public class CL_Cmd_AddUserRelation : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwSrcRoleID;
    [TypeInfo(0)]
    public UInt32 dwDestRoleID;
    [TypeInfo(1)]
    public Byte bRelationType;
}
public class LC_Cmd_AddUserRelation : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwRoleID;
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public tagRoleRelationClient RelationInfo;
}
public class CL_Cmd_DelUserRelation : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwSrcRoleID;
    [TypeInfo(0)]
    public UInt32 dwDestRoleID;
}
public class LC_Cmd_DelUserRelation : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwSrcRoleID;
    [TypeInfo(0)]
    public UInt32 dwDestRoleID;
    [TypeInfo(1)]
    public bool Result;
}
//public class CL_Cmd_ChangeUserRelation : NetCmdBase
//{
//    //[TypeInfo(0)]
//    //public UInt32 dwSrcRoleID;
//    [TypeInfo(0)]
//    public UInt32 dwDestRoleID;
//    [TypeInfo(1)]
//    public Byte bRelationType;
//}
public class LC_Cmd_ChangeUserRelation : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwSrcRoleID;
    [TypeInfo(0)]
    public UInt32 dwDestRoleID;
    [TypeInfo(1)]
    public bool Result;
    [TypeInfo(2)]
    public Byte bRelationType;
}
public class LC_Cmd_ChangeUserOline : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwSrcRoleID;
    [TypeInfo(0)]
    public UInt32 dwDestRoleID;
    [TypeInfo(1)]
    public bool IsOnline;
}
public class LC_Cmd_ChangeRelationLevel : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public UInt16 wLevel;
}
public class LC_Cmd_ChangeRelationFaceID : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public UInt32 dwFaceID;
}
public class LC_Cmd_ChangeRelationNickName : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1, FishDataInfo.NickNameLength + 1)]
    public string cNickName;
}
public class LC_Cmd_ChangeRelationGender : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public bool bGender;
}
public class LC_Cmd_ChangeRelationTitle : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Byte TitleID;
}
public class LC_Cmd_ChangeRelationAchievementPoint : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public UInt32 dwAchievementPoint;
}
public class LC_Cmd_ChangeRelationCharmValue : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1, FishDataInfo.MAX_CHARM_ITEMSUM)]
    public UInt32[] CharmArray;
}
public class LC_Cmd_ChangeRelationIP : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1, FishDataInfo.MAX_ADDRESS_LENGTH_IP+1)]
    public string IPAddress;
}
public class LC_Cmd_ChangeRelationVipLevel : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Byte VipLevel;
}
public class LC_Cmd_ChangeRelationIsInMonthCard : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public bool IsInMonthCard;
}
//Mail--------------------------------------------------------------------------------------------
public class tagNormalMailTitle
{
    [TypeInfo(0)]
    public UInt32 MailID;
    [TypeInfo(1)]
    public UInt32 SrcUserID;//发送方的ID
    [TypeInfo(2)]
    public UInt32 SrcFaceID;
    [TypeInfo(3, FishDataInfo.NickNameLength + 1)]
    public string SrcNickName;
    [TypeInfo(4)]
    public Byte bDiffTime;//在线天数差
    [TypeInfo(5)]
    public bool bIsRead;
};
public class tagNormalMail
{
    [TypeInfo(0)]
    public UInt32 MailID;
    [TypeInfo(1)]
    public UInt32 SrcUserID;//发送方的ID
    [TypeInfo(2)]
    public UInt32 SrcFaceID;
    [TypeInfo(3, FishDataInfo.NickNameLength + 1)]
    public string SrcNickName;
    [TypeInfo(4, FishDataInfo.MaxMailContexeLength + 1)]
    public string Context;//邮件内容
    [TypeInfo(5)]
    public Byte bDiffTime;//在线天数差
    [TypeInfo(6)]
    public bool bIsRead;
};
public class tagSystemMailTitle
{
    [TypeInfo(0)]
    public UInt32 MailID;
    [TypeInfo(1)]
    public Byte bDiffTime;//在线天数差
    [TypeInfo(2)]
    public bool bIsRead;//是否已经阅读
    [TypeInfo(3)]
    public bool bIsExistsItem;//是否存在附件
};
public class tagSystemMail
{
    [TypeInfo(0)]
    public UInt32 MailID;
    [TypeInfo(1, FishDataInfo.MaxMailContexeLength + 1)]
    public string Context;//邮件内容
    [TypeInfo(2)]
    public UInt16 RewardID;//携带的金币数量
    [TypeInfo(3)]
    public UInt32 RewardSum;//携带的金币数量
    [TypeInfo(4)]
    public Byte bDiffTime;//在线天数差
    [TypeInfo(5)]
    public bool bIsRead;//是否已经阅读
    [TypeInfo(6)]
    public bool bIsExistsReward;//是否存在奖励
};
public class CL_Cmd_GetUserMail : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
}
public class LC_Cmd_NormalMail : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagNormalMailTitle[] Array;
}
public class LC_Cmd_SystemMail : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagSystemMailTitle[] Array;
}
//public class LC_Cmd_GetMailFinish : NetCmdBase
//{
//    //[TypeInfo(0)]
//    //public UInt32 dwUserID;
//}
public class CL_Cmd_SendUserMail : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1, FishDataInfo.MaxMailContexeLength + 1)]
    public string Context;
}
public class LC_Cmd_SendNormalMail : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Byte Result;
}
public class CL_Cmd_DelMail : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwMailID;
}
public class LC_Cmd_DelMail : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwMailID;
    [TypeInfo(1)]
    public bool Result;
}
public class CL_Cmd_GetMailItem : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwMailID;
}
public class LC_Cmd_GetMailItem : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwMailID;
    [TypeInfo(1)]
    public bool Result;
}
//public class NetCmdMailLCGetUserMailIsRead : NetCmdBase
//{
//    [TypeInfo(0)]
//    public UInt32 dwUserID;
//    [TypeInfo(1)]
//    public UInt32 dwMailID;
//}
public class CL_Cmd_GetMailContext : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwMailID;
}
public class LC_Cmd_GetNormalMail : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public tagNormalMail MailInfo;
}
public class LC_Cmd_GetSystemMail : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public tagSystemMail MailInfo;
}
public class LC_Cmd_GetMailError : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwMailID;
}
public class LC_Cmd_AddNormalMail : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public tagNormalMailTitle MailInfo;
}
public class LC_Cmd_AddSystemMail : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public tagSystemMailTitle MailInfo;
}
public class LC_Cmd_SendUserMailResult: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32				DestUserID;
	[TypeInfo(1)]
    public bool				    Result;
}
//Role-------------------------------------------------------------------------------
public class LC_Cmd_ChangRoleExp : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwExp;
}
public class LC_Cmd_ChangeRoleLevel : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt16 wLevel;
}
public class LC_Cmd_ChangeRoleGender : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public bool bGender;
}
public class LC_Cmd_ChangeRoleNickName : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1, FishDataInfo.NickNameLength + 1)]
    public string NickName;
}
public class LC_Cmd_ChangeRoleFaceID : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwFaceID;
}
public class LC_Cmd_ChangeRoleGlobe : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public Int32 dwGlobeNum;
}
public class LC_Cmd_ChangeRoleMedal : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwMedalNum;
    [TypeInfo(1)]
    public UInt32 TotalUseMedal;
}
public class LC_Cmd_ChangeRoleCurrency : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwCurrencyNum;
}
public class LC_Cmd_ChangeRoleTitle : NetCmdBase
{
    [TypeInfo(0)]
    public Byte TitleID;
};
public class LC_Cmd_ChangeRoleAchievementPoint : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwAchievementPoint;
};
public class LC_Cmd_ChangeRoleCharmValue : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_CHARM_ITEMSUM)]
    public UInt32[] CharmArray;
};
public class LC_Cmd_ChangeRoleSendGiffSum : NetCmdBase
{
    [TypeInfo(0)]
    public Byte SendGiffSum;
};
public class LC_Cmd_ChangeRoleAcceptGiffSum : NetCmdBase
{
    [TypeInfo(0)]
    public Byte AcceptGiffSum;
};
public class LC_Cmd_ChangeRoleTaskStates : NetCmdBase
{
    [TypeInfo(0)]
    public Byte Index;
    [TypeInfo(1)]
    public bool States;
};
public class LC_Cmd_ChangeRoleAchievementStates : NetCmdBase
{
    [TypeInfo(0)]
    public Byte Index;
    [TypeInfo(1)]
    public bool States;
};
public class LC_Cmd_ChangeRoleActionStates : NetCmdBase
{
    [TypeInfo(0)]
    public Byte Index;
    [TypeInfo(1)]
    public bool States;
};
public class CL_Cmd_ChangeRoleGender : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public bool bGender;
}
public class CL_Cmd_ChangeRoleNickName : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0, FishDataInfo.NickNameLength + 1)]
    public string NickName;
}
public class CL_Cmd_ChangeRoleNormalFaceID : NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt32 dwUserID;
    [TypeInfo(0)]
    public UInt32 dwFaceID;
}
//public class CL_Cmd_RequestUserFaceID : NetCmdBase
//{
//    //[TypeInfo(0)]
//    //public UInt32 dwUserID;
//    [TypeInfo(0)]
//    public UInt16 PicSize;
//}
//public class LC_Cmd_ResponseUserFaceID : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Byte AllowUpload;
//}
//public class CL_Cmd_UpLoadingchunk : NetCmdBase
//{
//    [TypeInfo(0)]
//    public UInt16 StartIndex;
//    [TypeInfo(1)]
//    public UInt16 Size;
//    [TypeInfo(2, true)]
//    public Byte[] ImgData;
//}
//public class LC_Cmd_UpLoadingResponse : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Byte UpdateResult;
//}
//public class LC_Cmd_UpLoadingFinish : NetCmdBase
//{
//    //[TypeInfo(0)]
//    //public UInt32 dwUserID;
//    [TypeInfo(0)]
//    public UInt32 dwFaceID;
//}
public class LC_Cmd_UpLoadFtpError : NetCmdBase
{
}

//-------------------------Qauery
public class tagQueryRoleInfoClient
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1, FishDataInfo.NickNameLength + 1)]
    public string NickName;
    [TypeInfo(2)]
    public UInt16 wLevel;//玩家点击
    [TypeInfo(3)]
    public UInt32 dwFaceID;//玩家头像ID
    [TypeInfo(4)]
    public bool bGender; //玩家性别
    [TypeInfo(5)]
    public bool bIsOnline;
    [TypeInfo(6)]
    public UInt32 dwAchievementPoint;
    [TypeInfo(7)]
    public Byte TitleID;
    [TypeInfo(8, FishDataInfo.MAX_CHARM_ITEMSUM)]
    public UInt32[] CharmArray;
    [TypeInfo(9, FishDataInfo.MAX_ADDRESS_LENGTH_IP+1)]
    public string IPAddress;
    [TypeInfo(10)]
    public Byte VipLevel;
    [TypeInfo(11)]
    public bool IsInMonthCard;
    [TypeInfo(12)]
    public UInt32 GameID;
}
public class CL_Cmd_QueryUserByNickName : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.NickNameLength + 1)]
    public string NickName;
    [TypeInfo(1)]
    public Byte Page;//页面数
    [TypeInfo(2)]
    public bool IsOnline;//是否查询不在线的玩家 (可以进行控制的)
}
public class LC_Cmd_QueryUserInfo : NetCmdBase
{
    [TypeInfo(0)]
    public UInt16 Page;
    [TypeInfo(1)]
    public Byte States;
    [TypeInfo(2)]
    public UInt16 Sum;
    [TypeInfo(3, true)]
    public tagQueryRoleInfoClient[] Array;
}
//public class LC_Cmd_QueryUserInfoFinish : NetCmdBase
//{

//}
public class CL_Cmd_QueryUserByUserID : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
}
public class CL_Cmd_QueryUserByGameID : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 GameID;
}
public class LC_Cmd_QueryUserOnce : NetCmdBase
{
    [TypeInfo(0)]
    public tagQueryRoleInfoClient RoleInfo;
}
//Check-------------------------------------------------------------
//public class tagRoleCheckInfo
//{
//    [TypeInfo(0)]
//    public Byte bMontheCheckSum;//本月签到的信息记录 0-30天  每天是否签到
//    [TypeInfo(1)]
//    public Byte bCheckSum;//连续签到的次数 不随本月的信息修改8
//    [TypeInfo(2)]
//    public Byte bLastLogYear;//最后次签到的 年 月 日
//    [TypeInfo(3)]
//    public Byte bLastLogMonth;
//    [TypeInfo(4)]
//    public Byte bLastLogDay;
//};
//public class CL_Cmd_GetRoleCheckInfo : NetCmdBase
//{

//};
//public class LC_Cmd_GetRoleCheckInfo : NetCmdBase
//{
//    [TypeInfo(0)]
//    public tagRoleCheckInfo CheckInfo;
//};
public class CL_Cmd_CheckNowDay : NetCmdBase
{
};
public class LC_Cmd_CheckNowDay : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public UInt16 DayRewardID;
    [TypeInfo(2)]
    public UInt16 MonthRewardID;
};
public class CL_Cmd_CheckOtherDay : NetCmdBase
{
    [TypeInfo(0)]
    public Byte Day;
};
public class LC_Cmd_CheckOtherDay : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public Byte Day;
    [TypeInfo(2)]
    public UInt16 DayRewardID;
    [TypeInfo(3)]
    public UInt16 MonthRewardID;
};
//public class LC_Cmd_CheckChange : NetCmdBase
//{
//    [TypeInfo(0)]
//    public tagRoleCheckInfo CheckInfo;
//};

//Task------------------------------------------------------------------------
public enum EventTargetType  //被其他地方调用的钩子 必须与玩家相关的数据
{
    ET_NULL = 0,
    ET_GetGlobel = 1,//获得金币 累加的
    ET_GetMadel = 2,//获得奖牌 累加
    ET_GetCurren = 3,//获得乐币 累加
    ET_UpperLevel = 4,//提升等级 累加等级
    ET_CatchFish = 5,//捕获鱼   绑定参数0 表示任意鱼
    ET_SendGiff = 6,//发送赠送 
    ET_UseSkill = 7,//使用技能
    ET_LauncherGlobel = 8,//炮台获得金币
    ET_MaxGlobelSum = 9,//拥有的最大金币数量
    ET_ToLevel = 10,//到指定等级
    ET_MaxCurren = 11,//最大拥有乐币
    ET_Recharge  = 12,
    ET_Max,
}
public class EventInfo  //事件 已经事件带的参赛  用于 任务 和 成就
{
    public Byte OnlyID;//唯一ID 不是事件ID 在同一个节点下不可以重复
    public Byte EventID;
    public UInt32 BindParam;
    public UInt32 FinishValue;
};
public class tagRoleTaskInfo//任务的结构数据 玩家的任务结构
{
    [TypeInfo(0)]
    public Byte TaskID;//任务ID 从0到 256  可以直接建立数组
    [TypeInfo(1)]
    public UInt32 TaskValue;
};
//任务的命令
public class CL_Cmd_GetRoleTaskInfo : NetCmdBase
{

};
public class LC_Cmd_GetRoleTaskInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagRoleTaskInfo[] Array;
};
//public class LC_Cmd_GetRoleTaskInfoFinish : NetCmdBase
//{

//};
public class LC_Cmd_TaskAllEventFinish : NetCmdBase
{
    [TypeInfo(0)]
    public Byte TaskID;
};
public class CL_Cmd_GetTaskReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte TaskID;
};
public class LC_Cmd_GetTaskReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte TaskID;
};
//public class LC_Cmd_ResetTask : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Byte TaskID;
//    [TypeInfo(1)]
//    public bool IsReset;
//    [TypeInfo(2)]
//    public bool IsAction;
//    [TypeInfo(3)]
//    public bool TaskGroupStop;
//};
public class CL_Cmd_GetOnceTaskInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte TaskID;
};
public class LC_Cmd_GetOnceTaskInfo : NetCmdBase
{
    [TypeInfo(0)]
    public tagRoleTaskInfo TaskInfo;
};
public class LC_Cmd_JoinTask : NetCmdBase
{
    [TypeInfo(0)]
    public Byte TaskID;
};
public class LC_Cmd_ClearTask : NetCmdBase
{
};
public class LC_Cmd_DelTask : NetCmdBase
{
    [TypeInfo(0)]
    public Byte TaskID;
};
//成就
public class tagRoleAchievementInfo
{
    [TypeInfo(0)]
    public Byte AchievementID;
    [TypeInfo(1)]
    public UInt32 AchievementValue;
};
public class CL_Cmd_GetRoleAchievementInfo : NetCmdBase
{

};
public class LC_Cmd_GetRoleAchievementInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagRoleAchievementInfo[] Array;
};
//public class LC_Cmd_GetRoleAchievementInfoFinish : NetCmdBase
//{

//};
public class LC_Cmd_AchievementAllEventFinish : NetCmdBase
{
    [TypeInfo(0)]
    public Byte AchievementID;
};
public class CL_Cmd_GetAchievementReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte AchievementID;
};
public class LC_Cmd_GetAchievementReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte AchievementID;
};
public class CL_Cmd_GetOnceAchievementInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte AchievementID;
};
public class LC_Cmd_GetOnceAchievementInfo : NetCmdBase
{
    [TypeInfo(0)]
    public tagRoleAchievementInfo AchievementInfo;
};
public class LC_Cmd_JoinAchievement : NetCmdBase
{
    [TypeInfo(0)]
    public Byte AchievementID;
};
public class LC_Cmd_DelAchievement : NetCmdBase
{
    [TypeInfo(0)]
    public Byte AchievementID;
};
//称号
public class CL_Cmd_LoadRoleTitle : NetCmdBase
{

};
public class LC_Cmd_LoadRoleTitle : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public Byte[] Array;
};
//public class LC_Cmd_LoadRoleTitleFinish : NetCmdBase
//{

//};
public class LC_Cmd_AddRoleTitle : NetCmdBase
{
    [TypeInfo(0)]
    public Byte AddTitleID;
};
public class LC_Cmd_DelRoleTitle : NetCmdBase
{
    [TypeInfo(0)]
    public Byte DelTitleID;
};
public class CL_Cmd_ChangeRoleCurrTitle : NetCmdBase
{
    [TypeInfo(0)]
    public Byte TitleID;
};

//比赛
public class tagRoleMonthInfo //玩家的比赛信息
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public UInt32 dwIndex;
    [TypeInfo(2)]
    public Byte bMonthID;
    [TypeInfo(3)]
    public UInt32 dwMonthGlobel;
    [TypeInfo(4)]
    public UInt32 dwMonthScore;
    [TypeInfo(5)]
    public Byte wUserAddGlobelNum;//续币的次数
    [TypeInfo(6)]
    public UInt32 SkillSum; //客户端无效的
}

public class CL_Cmd_SignUpMonth : NetCmdBase
{
    [TypeInfo(0)]
    public Byte MonthID;
}
public class LC_Cmd_SignUpMonthFiled : NetCmdBase
{
    [TypeInfo(0)]
    public Byte MonthID;
}
public class LC_Cmd_SignUpMonthSucess : NetCmdBase
{
    [TypeInfo(0)]
    public Byte MonthID;
}
public class CL_Cmd_JoinMonth : NetCmdBase
{
    [TypeInfo(0)]
    public Byte MonthID;
}
public class LC_Cmd_JoinMonthSucess : NetCmdBase
{
    [TypeInfo(0)]
    public tagRoleMonthInfo MonthInfo;
}
public class LC_Cmd_JoinMonthFiled : NetCmdBase
{
}
public class CL_Cmd_ChangeUserAddMonthGlobelNum : NetCmdBase
{

}
public class LC_Cmd_ChangeUserAddMonthGlobelNum : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
}
public class LC_Cmd_UserChangeIndex : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public UInt32 dwIndex;
    [TypeInfo(2)]
    public UInt32 dwUpperSocre;
}
public class LC_Cmd_UserMonthEnd : NetCmdBase
{
    [TypeInfo(0)]
    public Byte MonthID;
    [TypeInfo(1)]
    public UInt32 MonthIndex;
    [TypeInfo(2)]
    public UInt32 MonthScores;
    [TypeInfo(3)]
    public UInt32 VipScores;
}
public class CL_Cmd_LoadRoleSignUpInfo : NetCmdBase
{

};
public class LC_Cmd_LoadRoleSignUpInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public Byte[] Array;
};
public class tagMonthRoleSum
{
    [TypeInfo(0)]
    public Byte MonthID;
    [TypeInfo(1)]
    public UInt16 MonthUserSum;
};
//public class CL_Cmd_GetMonthRoleSum : NetCmdBase
//{

//}
public class LC_Cmd_GetMonthRoleSum : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagMonthRoleSum[] Array;
}
//public class LC_Cmd_GetMonthRoleSumFinish : NetCmdBase
//{
//}
public class CL_Cmd_ResetMonthInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte MonthID;
}
public class LC_Cmd_ResetMonthInfo : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public Byte MonthID;
    [TypeInfo(2)]
    public bool Result;
}
public class LC_Cmd_SetRoleMonthInfo : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public Byte MonthID;
    [TypeInfo(2)]
    public Int32 MonthSocre;
    [TypeInfo(3)]
    public Int32 MonthGlobel;
};
public class LC_Cmd_ResetRoleMonthInfo: NetCmdBase
{
    [TypeInfo(0)]
    public tagRoleMonthInfo MonthInfo;
}
public class tagMonthRoleInfo
{
	[TypeInfo(0)]
    public UInt32		dwUserID;
	[TypeInfo(1)]
    public UInt32       Index;
	[TypeInfo(2,FishDataInfo.NickNameLength+1)]
    public string		NickName;
	[TypeInfo(3)]
    public UInt32		Score;
	[TypeInfo(4)]
    public UInt32		VipScore;
	[TypeInfo(5)]
    public UInt32		FaceID;
};
public class LC_Cmd_MonthList: NetCmdBase
{
    [TypeInfo(0)]
    public Byte					MonthID;
	[TypeInfo(1,FishDataInfo.MAX_MonthList_Length)]
    public tagMonthRoleInfo[]	MonthInfo;
}
//Rank -------------------------------------------------------------------------------------
//public class RankIndexInfo
//{
//    [TypeInfo(0)]
//    public Byte RankID;
//    [TypeInfo(1)]
//    public Byte RankIndex;
//};
//public class tagRankWeekReward
//{
//    [TypeInfo(0)]
//    public RankIndexInfo IndexInfo;
//    [TypeInfo(1)]
//    public bool IsReward;//是否已经领取奖励了
//};
public class tagRankWeekRankClient
{
    [TypeInfo(0)]
    public Byte RankID;
    [TypeInfo(1)]
    public Byte RankIndex;
    [TypeInfo(2)]
    public bool IsReward;
};
public class CL_Cmd_GetRankReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte RankID;
}
public class LC_Cmd_GetRankReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte RankID;
}
public class CL_Cmd_GetRankInfo : NetCmdBase
{
}
public class LC_Cmd_GetWeekRankInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagRankWeekRankClient[] Array;
}
//public class LC_Cmd_GetRankInfoFinish : NetCmdBase
//{
//}
//Chest ----------------------------------------------------------------
public class ChestOnceStates
{
    [TypeInfo(0)]
    public Byte Index;
    [TypeInfo(1)]
    public Byte RewardID;
    [TypeInfo(2)]
    public Byte RewardOnlyID;
};
public class tagChestInfo
{
    public Byte ChestOnlyID;
    public Byte ChestTypeID;//宝箱的ID
    public Byte OpenSum;//已经开启的次数
    public UInt32 ActionTime;//激活的时间
    public List<ChestOnceStates> ChestArray = new List<ChestOnceStates>();
    public tagChestInfo()
    {
        ChestOnlyID = 0;
        ChestTypeID = 0;
        OpenSum = 0;
        ActionTime = 0;
        //ActionOpemStates = 0;
        //ActionRewardStates = 0;
        ChestArray.Clear();
    }
}
//public class LC_Cmd_AddChest : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Byte ChestOnlyID;
//    [TypeInfo(1)]
//    public Byte ChestTypeID;//宝箱的类型ID
//}
public class LC_Cmd_ActionChest : NetCmdBase
{
    [TypeInfo(0)]
    public bool IsReset;
    [TypeInfo(1)]
    public Byte ChestOnlyID;
    [TypeInfo(2)]
    public Byte ChestTypeID;
    //[TypeInfo(2)]
    //public UInt32 ActionOpenStates;
    //[TypeInfo(3)]
    //public UInt32 ActionRewardStates;
    [TypeInfo(3)]
    public Byte ChestSum;
    [TypeInfo(4)]
    public Byte OpenSum;
    [TypeInfo(5)]
    public UInt32 EndSec;
    [TypeInfo(6,true)]
    public ChestOnceStates[] ChestArray;
}
public class LC_Cmd_ChestEnd : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ChestOnlyID;
}
public class CL_Cmd_GetChestReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ChestOnlyID;
    [TypeInfo(1)]
    public Byte ChestIndex;
}
public class LC_Cmd_GetChestReward : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public Byte ChestOnlyID;
    [TypeInfo(2)]
    public ChestOnceStates ChestStates;
}
public class CL_Cmd_CloseChest : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ChestOnlyID;
}
public class LC_Cmd_ResetChest: NetCmdBase
{
}
//Charm----------------------------------------------------------------------
public class CL_Cmd_ChangeOtherUserCharm : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Byte CharmIndex;
    [TypeInfo(2)]
    public UInt32 ItemSum;
}
public class LC_Cmd_ChangeRoleCharmResult : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public UInt32 dwDestUserID;
    [TypeInfo(2)]
    public Int32  dwDestUserCharmValue;
}
public class LC_Cmd_TableRoleCharmInfo: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 SrcUserID;
    [TypeInfo(1)]
    public UInt32 DestUserID;
    [TypeInfo(2)]
    public Byte CharmIndex;
    [TypeInfo(3)]
    public UInt32 ItemSum;
}
//shop---------------------------------------------------------
public class CL_Cmd_ShopItem : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ShopID;
    [TypeInfo(1)]
    public Byte ShopItemIndex;
    [TypeInfo(2)]
    public UInt32 ItemSum;
};
public class LC_Cmd_ShopItemResult : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
};

//Entity---------------------------------------------------------------
public class tagRoleAddressInfo
{
    [TypeInfo(0, FishDataInfo.MAX_NAME_LENTH + 1)]
    public string Name;
    [TypeInfo(1)]
    public UInt64 Phone;
    [TypeInfo(2, FishDataInfo.MAX_EMAIL_LENGTH + 1)]
    public string Email;
    [TypeInfo(3, FishDataInfo.MAX_IDENTITY_LENGTH + 1)]
    public string IdentityID;
    [TypeInfo(4, FishDataInfo.MAX_NAME_LENTH + 1)]
    public string EntityItemUseName;
    [TypeInfo(5)]
    public UInt64 EntityItemUsePhone;
    [TypeInfo(6, FishDataInfo.MAX_ADDRESS_LENGTH + 1)]
    public string EntityItemUseAddres;
}
public class CL_Cmd_LoadUserEntity : NetCmdBase
{
}
public class LC_Cmd_LoadUserEntity : NetCmdBase
{
    [TypeInfo(0)]
    public tagRoleAddressInfo UserEntityInfo;
}
//public class CL_Cmd_ChangeRoleName : NetCmdBase
//{
//    [TypeInfo(0, FishDataInfo.MAX_NAME_LENTH + 1)]
//    public string Name;//姓名
//}
public class LC_Cmd_ChangeRoleName : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_NAME_LENTH + 1)]
    public string Name;
}
//public class CL_Cmd_ChangeRolePhone : NetCmdBase
//{
//    [TypeInfo(0, FishDataInfo.MAX_PHONE_LENGTH + 1)]
//    public string Phone;
//}
public class LC_Cmd_ChangeRolePhone : NetCmdBase
{
    [TypeInfo(0)]
    public UInt64 Phone;
}

//public class CL_Cmd_ChangeRoleEmail : NetCmdBase
//{
//    [TypeInfo(0, FishDataInfo.MAX_EMAIL_LENGTH + 1)]
//    public string Email;
//}
public class LC_Cmd_ChangeRoleEmail : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_EMAIL_LENGTH + 1)]
    public string Email;
}

//public class CL_Cmd_ChangeRoleAddress : NetCmdBase
//{
//    [TypeInfo(0, FishDataInfo.MAX_ADDRESS_LENGTH + 1)]
//    public string Addres;
//}
//public class LC_Cmd_ChangeRoleAddress : NetCmdBase
//{
//    [TypeInfo(0, FishDataInfo.MAX_ADDRESS_LENGTH + 1)]
//    public string Addres;
//}

//public class CL_Cmd_ChangeRoleEntityID : NetCmdBase
//{
//    [TypeInfo(0, FishDataInfo.MAX_IDENTITY_LENGTH + 1)]
//    public string IdentityID;
//}
public class LC_Cmd_ChangeRoleEntityID : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_IDENTITY_LENGTH + 1)]
    public string IdentityID;
}
public class CL_Cmd_ChangeRoleEntityItemUseName   : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_NAME_LENTH + 1)]
    public string EntityItemUseName;
}
public class LC_Cmd_CHangeRoleEntityItemUseName   : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_NAME_LENTH + 1)]
    public string EntityItemUseName;
}
public class CL_Cmd_ChagneRoleEntityItemUsePhone  : NetCmdBase
{
    [TypeInfo(0)]
    public UInt64 EntityItemUsePhone;
}
public class LC_Cmd_ChangeRoleEntityItemUsePhone  : NetCmdBase
{
    [TypeInfo(0)]
    public UInt64 EntityItemUsePhone;
}
public class CL_Cmd_ChangeRoleEntityItemUseAddress: NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_ADDRESS_LENGTH + 1)]
    public string EntityItemUseAddress;
}
public class LC_Cmd_ChangeRoleEntityItemUseAddress : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_ADDRESS_LENGTH + 1)]
    public string EntityItemUseAddress;
}
//活动
public class tagRoleActionInfo
{
    [TypeInfo(0)]
    public Byte ActionID;
    [TypeInfo(1)]
    public UInt32 ActionValue;
    [TypeInfo(2)]
    public UInt32 ActionStates;//用于控制活动的进行进度
};
public class CL_Cmd_GetRoleActionInfo : NetCmdBase
{

};
public class LC_Cmd_GetRoleActionInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagRoleActionInfo[] Array;
};
//public class LC_Cmd_GetRoleActionInfoFinish : NetCmdBase
//{

//};
public class LC_Cmd_ActionAllEventFinish : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ActionID;
};
public class CL_Cmd_GetActionReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ActionID;
    [TypeInfo(1)]
    public UInt32 OnceID;
};
public class LC_Cmd_GetActionReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ActionID;
};
public class CL_Cmd_GetOnceActionInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ActionID;
};
public class LC_Cmd_GetOnceActionInfo : NetCmdBase
{
    [TypeInfo(0)]
    public tagRoleActionInfo ActionInfo;
};
public class LC_Cmd_DelAction : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ActionID;
}
public class LC_Cmd_JoinAction : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ActionID;
}
//Giff
public class tagGiffClientInfo
{
    [TypeInfo(0)]
    public UInt32 OnlyID;
    [TypeInfo(1)]
    public UInt32 dwUserID;
    [TypeInfo(2)]
    public UInt32 dwFaceID;
    [TypeInfo(3, FishDataInfo.NickNameLength + 1)]
    public string NickName;
    [TypeInfo(4)]
    public Byte DiffTime;//0-250
}
public class CL_Cmd_GetRoleGiff : NetCmdBase
{

}
public class LC_Cmd_GetRoleGiff : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagGiffClientInfo[] Array;
}
//public class LC_Cmd_GetRoleGiffFinish : NetCmdBase
//{

//}
public class CL_Cmd_AddRoleGiff : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
}
public class LC_Cmd_AddRoleGiffResult : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public bool Result;
}
public class LC_Cmd_AddBeRoleGiff : NetCmdBase
{
    [TypeInfo(0)]
    public tagGiffClientInfo GiffInfo;
}
public class CL_Cmd_GetRoleGiffReward : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 GiffID;
}
public class LC_Cmd_GetRoleGiffReward : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 GiffID;
}
public class CL_Cmd_GetAllRoleGiffReward : NetCmdBase
{

}
public class LC_Cmd_GetAllRoleGiffReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public UInt32[] Array;
}
public class tagNowDaySendGiffInfo
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public Byte SendSum;
}
public class CL_Cmd_GetRoleNowDaySendGiff : NetCmdBase
{

}
public class LC_Cmd_GetRoleNowDaySendGiff : NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public tagNowDaySendGiffInfo[] Array;
}
//public class LC_Cmd_GetRoleNowDaySendGiffFinish : NetCmdBase
//{
//}
public class LC_Cmd_DelRoleGiff: NetCmdBase
{
    [TypeInfo(0)]
    public Byte States;
    [TypeInfo(1)]
    public UInt16 Sum;
    [TypeInfo(2, true)]
    public UInt32[] Array;
}
//GlobelShop
//public class CL_Cmd_GlobelShopItem : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Byte ID;
//    [TypeInfo(1)]
//    public UInt16 Sum;
//};
//public class LC_Cmd_GlobelShopItem : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Byte Result;
//};
//在线奖励
public class CL_Cmd_GetOnlineReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ID;
}
public class LC_Cmd_GetOnlineReward : NetCmdBase
{
    [TypeInfo(0)]
    public Byte RewardID;
    [TypeInfo(1)]
    public UInt32 States;
    [TypeInfo(2)]
    public bool Result;//是否成功
}

//玩家数据统计处理
public class tagRoleGameData
{
    [TypeInfo(0)]
    public UInt32 RoleMonthSigupSum;//玩家报名比赛的次数
    [TypeInfo(1)]
    public UInt32 RoleMonthRewardSum;//
    [TypeInfo(2)]
    public UInt32 RoleMonthFirstSum;
    [TypeInfo(3)]
    public UInt32 RoleMonthSecondSum;
    [TypeInfo(4)]
    public UInt32 RoleMonthThreeSum;
    [TypeInfo(5)]
    public UInt32 RoleCatchFishTotalSum;
    [TypeInfo(6)]
    public UInt32 RoleGetGlobelSum;
    [TypeInfo(7)]
    public UInt32 NonMonthGameSec;//玩家进行非比赛游戏时间的秒杀
    [TypeInfo(8)]
    public UInt32 TotalGameSec;//玩家进行总游戏时间的秒数
    [TypeInfo(9)]
    public UInt32 CatchFishSum_9;//玩家捕获指定类型鱼的数量
    [TypeInfo(10)]
    public UInt32 CatchFishSum_18;
    [TypeInfo(11)]
    public UInt32 CatchFishSum_20;
    [TypeInfo(12)]
    public UInt32 CatchFishSum_1;
    [TypeInfo(13)]
    public UInt32 CatchFishSum_3;
    [TypeInfo(14)]
    public UInt32 CatchFishSum_19;
    [TypeInfo(15)]
    public UInt32 MaxCombSum;
}
public class CL_Cmd_GetGameData : NetCmdBase
{

}
public class LC_Cmd_GendGameData : NetCmdBase
{
    [TypeInfo(0)]
    public tagRoleGameData GameData;
}
//package
//public class  CL_Cmd_OpenPackage  : NetCmdBase
//{
//    [TypeInfo(0)]
//    public UInt32 PackageItemID;
//};
public class LC_Cmd_OpenPackage : NetCmdBase
{
    [TypeInfo(0)]
    public bool     Result;
    [TypeInfo(1)]
    public Byte     RewardSum;
    [TypeInfo(2,true)]
    public UInt16[] RewardID;
};
//Launcher
public class LC_Cmd_LauncherData : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 LauncherData;
};
//Message
enum MessageType
{
    MT_Table = 1,//当前桌子
    MT_TableMessage = 2,//当前桌子 弹框

    MT_Game = 3,//当前GameServer
    MT_GameMessage = 4,

    MT_Center = 5,//当前服务器
    MT_CenterMessage = 6,

    MT_User = 7,//单个玩家
    MT_UserMessage = 8,
};
public class LC_Cmd_SendMessage: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 Param;
    [TypeInfo(1)]
    public UInt32 MessageColor;
    [TypeInfo(2)]
    public Byte StepNum;
    [TypeInfo(3)]
    public Byte StepSec;
    [TypeInfo(4)]
    public Byte		    MessageType;//消息类型
    [TypeInfo(5)]
    public UInt16       MessageSize;//消息类型
    [TypeInfo(6,true)]
	public string		Message;//消息
}
public class CL_Cmd_SendMessage: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 Param;
    [TypeInfo(1)]
    public UInt32 MessageColor;
    [TypeInfo(2)]
    public Byte StepNum;
    [TypeInfo(3)]
    public Byte StepSec;
    [TypeInfo(4)]
    public Byte MessageType;//消息类型
    [TypeInfo(5)]
    public UInt16 MessageSize;//消息类型
    [TypeInfo(6, true)]
    public string Message;//消息
}

//Recharge
public class CL_Cmd_Recharge: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 ID;
    [TypeInfo(1)]
    public bool IsPC;
}
public class LC_Cmd_Recharge : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 ID;
    [TypeInfo(1)]
    public bool Result;
}

public class LC_Cmd_GetOrderID : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 OrderID;
    [TypeInfo(1)]
    public UInt32 ShopID;
    [TypeInfo(2)]
    public bool Result;
}
public class IOSRechargeInfo
{
    //string			OrderString
    [TypeInfo(0)]
    public Byte HandleSum;
    [TypeInfo(1)]
    public UInt32 Sum;
    [TypeInfo(2, true)]
    public Byte[] Array;//平台的唯一ID
};
public class CL_Cmd_IOSRecharge : NetCmdBase
{
    [TypeInfo(0)]
    public IOSRechargeInfo OrderInfo;
}
//Announcement
public class AnnouncementOnce
{
	//一条公告
    [TypeInfo(0, FishDataInfo.MAX_NAME_LENTH+1)]
	public string    NickName; //公告的字符串
    [TypeInfo(1)]
    public Byte     ShopID;
    [TypeInfo(2)]
    public Byte     ShopOnlyID;
};
//public class AnnouncementClientOnce
//{
//    //一条公告
//    public string AnnouncementStr; //公告的字符串
//};
public class CL_Cmd_GetAllAnnouncement: NetCmdBase
{

}
public class LC_Cmd_GetAllAnnouncement: NetCmdBase
{
    [TypeInfo(0)]
    public Byte		States;
    [TypeInfo(1)]
    public UInt16  Sum;
    [TypeInfo(2,true)]
	public AnnouncementOnce[] Array;
}
//public class LC_Cmd_GetAllAnnouncementFinish: NetCmdBase
//{

//}
public class LC_Cmd_SendNewAnnouncementOnce : NetCmdBase
{
    [TypeInfo(0)]
    public AnnouncementOnce		pOnce;
}
//operate
public class CL_Cmd_RealNameVerification : NetCmdBase
{
    [TypeInfo(0,FishDataInfo.MAX_NAME_LENTH+1)]
    public string			Name;
    [TypeInfo(1,FishDataInfo.MAX_IDENTITY_LENGTH+1)]
	public string			IDCard;
}
public class LC_Cmd_RealNameVerification: NetCmdBase
{
    [TypeInfo(0)]
    public Byte			ErrorID;//错误ID
    [TypeInfo(1,FishDataInfo.MAX_NAME_LENTH+1)]
	public string			Name;
    [TypeInfo(2,FishDataInfo.MAX_IDENTITY_LENGTH+1)]
	public string			IDCard;
}
public class CL_Cmd_GetPhoneVerificationNum : NetCmdBase
{
    [TypeInfo(0)]
    public UInt64 PhoneNumber;
}
public class LC_Cmd_GetPhoneVerificationNum: NetCmdBase
{
    [TypeInfo(0)]
    public Byte ErrorID;
    [TypeInfo(1)]
    public UInt64 PhoneNumber;
}
public class CL_Cmd_BindPhone: NetCmdBase
{
    //[TypeInfo(0)]
    //public UInt64			PhoneNumber;
    [TypeInfo(0)]
    public UInt32           BindNumber;
    [TypeInfo(1)]
    public UInt32           SecPasswordCrc1;
    [TypeInfo(2)]
    public UInt32           SecPasswordCrc2;
    [TypeInfo(3)]
    public UInt32           SecPasswordCrc3;
}
public class LC_Cmd_BindPhone : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ErrorID;
    [TypeInfo(1)]
    public bool IsFirstBind;
    [TypeInfo(2)]
    public UInt64 PhoneNumber;
}
public class LC_Cmd_BuyEntityItem : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ErrorID;//提示玩家购买成功 还是 失败
}
public class CL_Cmd_BindEmail : NetCmdBase
{
    [TypeInfo(0,FishDataInfo.MAX_EMAIL_LENGTH +1)]
    public string			EMail;
}
public class LC_Cmd_BindEmail : NetCmdBase
{
    [TypeInfo(0)]
    public Byte			    ErrorID;
	[TypeInfo(1,FishDataInfo.MAX_EMAIL_LENGTH +1)]
    public string			EMail;
}
public class LC_Cmd_PhonePay : NetCmdBase
{
    [TypeInfo(0)]
    public Byte ErrorID;
}
public class LC_Cmd_AddNormalOrderID : NetCmdBase
{
    [TypeInfo(0)]
    public bool	    Result;
	[TypeInfo(1,33)]
    public Byte[]	Transid;//订单流水号
    [TypeInfo(2, 256)]
    public Byte[]   Sign;//订单流水号
	[TypeInfo(3)]
    public UInt32	ShopID;
	[TypeInfo(4)]
    public UInt32	OrderID;
}
public class LC_Cmd_RoleMessageChange : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 RoleMessageData;
}

//
public class ChannelUserInfo
{
    //[TypeInfo(0)]
    //public UInt32			uid;//平台的唯一ID
    [TypeInfo(0)]
    public Byte			    HandleSum;
    [TypeInfo(1)]
    public UInt32			Sum;
    [TypeInfo(2,true)]
    public Byte[]			Array;//平台的唯一ID

    //[TypeInfo(1,32)]
    //public string			uid;//渠道的名称
    //[TypeInfo(1,32)]
    //public string			channelLabel;//渠道的名称
    //[TypeInfo(2,32)]
    //public string			channeluid;//渠道用户的ID
    //[TypeInfo(3,32)]
    //public string			productCode;
    //[TypeInfo(4,32)]
    //public string			channelCode;
    //[TypeInfo(5,32)]
    //public string			UserName;//用户名称
    //[TypeInfo(6,32)]
    //public string			token;//渠道令牌
}
public class CL_Cmd_ChannelLogon : NetCmdBase
{
    [TypeInfo(0)]
    public Byte PlateFormID;//平台的ID
    [TypeInfo(1)]
    public UInt32 VersionID;
    [TypeInfo(2)]
    public UInt32 PathCrc;
    [TypeInfo(3,FishDataInfo.MacAddressLength+1)]
    public string MacAddress;
    [TypeInfo(4)]
    public ChannelUserInfo channelInfo;
}
public class LC_Cmd_ChannelLogon : NetCmdBase
{
	[TypeInfo(0)]
    public bool				Result;
}



public enum PhoneLogonError
{
	PLE_Success = 0,//
	PLE_LogonExists = 1,//手机号码登陆已经存在 
	PLE_PasswordFailed = 2,//手机验证码发送失败
	PLE_PasswordError = 3,//手机验证码错误
	PLE_SystemError = 4,//系统错误
};
public class CL_Cmd_PhoneLogon: NetCmdBase
{
	[TypeInfo(0)]
    public UInt64   PhoneNumber;
}
public class LC_Cmd_PhoneLogon: NetCmdBase
{
    [TypeInfo(0)]
    public UInt64 PhoneNumber;
    [TypeInfo(1)]
    public Byte ErrorID;
}
public class CL_Cmd_PhoneLogonCheck: NetCmdBase //修改密码
{
	[TypeInfo(0)]
    public UInt64				PhoneNumber;
	[TypeInfo(1)]
    public UInt32				Password;
	[TypeInfo(2,FishDataInfo.MacAddressLength+1)]
    public string				MacAddress;
    [TypeInfo(3)]
    public UInt32               PasswordCrc1;
    [TypeInfo(4)]
    public UInt32               PasswordCrc2;
    [TypeInfo(5)]
    public UInt32               PasswordCrc3;
    [TypeInfo(6)]
    public bool                 IsFirOrSecPass;
}
public class LC_Cmd_PhoneLogonCheck: NetCmdBase
{
	[TypeInfo(0)]
    public bool				Result;
	[TypeInfo(1)]
    public UInt32			dwUserID;
	[TypeInfo(2)]
    public Byte				ErrorID;
}
public class LC_Cmd_SaveAccountInfo : NetCmdBase
{
    [TypeInfo(0,FishDataInfo.AccountNameLength+1)]
    public string			AccountName;
	[TypeInfo(1)]
    public UInt32			PasswordCrc1;
	[TypeInfo(2)]
    public UInt32			PasswordCrc2;
	[TypeInfo(3)]
    public UInt32			PasswordCrc3;
}

public class CL_Cmd_PhoneSecPwd : NetCmdBase
{
    [TypeInfo(0)]
    public UInt64				PhoneNumber;
	[TypeInfo(1)]
    public UInt32				PasswordCrc1;
	[TypeInfo(2)]
    public UInt32				PasswordCrc2;
	[TypeInfo(3)]
    public UInt32				PasswordCrc3;
	[TypeInfo(4,FishDataInfo.MacAddressLength+1)]
    public string				MacAddress;//Mac地址
	[TypeInfo(5)]
    public Byte				    PlateFormID;//平台的ID
	[TypeInfo(6)]
    public UInt32				VersionID;
	[TypeInfo(7)]
    public UInt32				PathCrc;
}

public class WeiXinUserCodeInfo
{
	//CHAR			Code[xx];//或者为 OpenID
	[TypeInfo(0)]
    public Byte				    HandleSum;
	[TypeInfo(1)]
    public UInt32				Sum;
	[TypeInfo(2,true)]
    public Byte[]				Array;
};
public class CL_Cmd_WeiXinLogon: NetCmdBase
{
    [TypeInfo(0)]
    public Byte				    PlateFormID;//平台的ID
	[TypeInfo(1)]
    public UInt32				VersionID;
	[TypeInfo(2)]
    public UInt32				PathCrc;
	[TypeInfo(3,FishDataInfo.MacAddressLength+1)]
    public string				MacAddress;//Mac地址
	[TypeInfo(4)]
    public WeiXinUserCodeInfo   CodeInfo;
}
public class LC_Cmd_WeiXinLogon : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}

public class QQUserCodeInfo
{
	//CHAR			Token[xx];//或者为 OpenID
	[TypeInfo(0)]
    public Byte				    HandleSum;
	[TypeInfo(1)]
    public UInt32				Sum;
	[TypeInfo(2,true)]
    public Byte[]				Array;
};
public class CL_Cmd_QQLogon: NetCmdBase
{
    [TypeInfo(0)]
    public Byte				    PlateFormID;//平台的ID
	[TypeInfo(1)]
    public UInt32				VersionID;
	[TypeInfo(2)]
    public UInt32				PathCrc;
	[TypeInfo(3,FishDataInfo.MacAddressLength+1)]
    public string				MacAddress;//Mac地址
	[TypeInfo(4)]
    public QQUserCodeInfo       TokenInfo;
}
public class LC_Cmd_QQLogon : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class LC_Cmd_RsgNewAccount: NetCmdBase
{
    [TypeInfo(0)]
    public Byte LogonType;
}
//ExChange

public class CL_Cmd_RoleUseExChangeCode: NetCmdBase
{
    [TypeInfo(0,FishDataInfo.ExChangeCode_Length+1)]
    public string	ExChangeCode;
}
public class LC_Cmd_RoleUseExChangeCode: NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;//使用是否成功
    [TypeInfo(1)]
    public Byte ID;
}

//Lottery
public class CL_Cmd_GetLotteryReward: NetCmdBase
{
    [TypeInfo(0)]
    public Byte		LotteryID;
}
public class LC_Cmd_GetLotteryReward: NetCmdBase
{
    [TypeInfo(0)]
    public Byte LotteryID;
    [TypeInfo(1)]
    public UInt16 RewardID;
    [TypeInfo(2)]
    public bool Result;
}

public class CL_Cmd_LotteryUIStates: NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public bool IsOpen;
}
public class LC_Cmd_LotteryUIStates : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public bool IsOpen;
}

//minigame
//public class TableUpdateInfo //桌子上奖池的数据
//{
//    //下注阶段 桌子上的数据
//    [TypeInfo(0,FishDataInfo.MAX_NIUNIU_ClientSum)]
//    public UInt64[]  GlobelNum;//4个下注区的数据
//};
//public class TableBrandResult//桌子上 牌的数据
//{
//    //桌子牌的数据
//    [TypeInfo(0,(FishDataInfo.MAX_NIUNIU_ClientSum +1) * FishDataInfo.MAX_NIUNIU_BrandSum)]
//    public Byte[]	BrandArray;//25  5x5 牌的数据 牌包括了结果
//    [TypeInfo(1,FishDataInfo.MAX_NIUNIU_ClientSum +1)]
//    public UInt16[]  BrandResult;//牌的结果 Level<<8 + MaxBrand
//};
//public class  BankerInfo //桌子上庄家数据
//{
//    [TypeInfo(0)]
//    public UInt32	dwUserID;
//    [TypeInfo(1)]
//    public UInt32	FaceID;
//    [TypeInfo(2,FishDataInfo.NickNameLength +1)]
//    public string	NickName;
//    [TypeInfo(3)]
//    public UInt32	AllBankerGlobelSum;
//};
//public class CL_Cmd_JoinRoom : NetCmdBase
//{

//}
//public class LC_Cmd_JoinRoom : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Byte TableUpdateSec;//桌子上剩余的秒数
//    [TypeInfo(1)]
//    public Byte TableStates;//桌子状态
//    [TypeInfo(2)]
//    public TableUpdateInfo TableInfo;//桌子上奖池的数据
//    [TypeInfo(3)]
//    public TableBrandResult TableBrand;//桌子上牌的数据
//    [TypeInfo(4)]
//    public BankerInfo       TableBanker;//庄家数据
//}
//public class LC_Cmd_TableUpdate : NetCmdBase
//{
//    [TypeInfo(0)]
//    public TableUpdateInfo TableInfo;
//}
//public class CL_Cmd_AddGlobel : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Byte Index;//下注区
//    [TypeInfo(1)]
//    public UInt32 GlobelSum;
//}
//public class LC_Cmd_AddGlobel : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Byte Index;//下注区
//    [TypeInfo(1)]
//    public UInt32 GlobelSum;//金币数量
//    [TypeInfo(2)]
//    public bool Result;//结果
//}
//public class LC_Cmd_TableResult : NetCmdBase
//{
//    [TypeInfo(0)]
//    public Int32 AddGlobel;
//    [TypeInfo(1)]
//    public TableBrandResult TableBrand;//桌子上牌的数据
//    [TypeInfo(2)]
//    public TableUpdateInfo TableInfo;//桌子上牌的数据
//}
//public class LC_Cmd_TableBegin : NetCmdBase
//{

//}
//public class CL_Cmd_LeaveNiuNiuTable : NetCmdBase
//{

//}
//public class LC_Cmd_BankerChange : NetCmdBase
//{
//    [TypeInfo(0)]
//    public BankerInfo TableBanker;//庄家数据
//}
//public class LC_Cmd_WirteBankerListChange : NetCmdBase
//{

//}
//public class CL_Cmd_AddBankerList : NetCmdBase
//{

//}
//public class LC_Cmd_AddBankerList : NetCmdBase
//{
//    [TypeInfo(0)]
//    public bool Result;
//}


//NiuNiu
public class NiuNiuRoleInfo
{
    [TypeInfo(0)]
	public UInt32	dwUserID;
	[TypeInfo(1)]
	public UInt32	GlobelSum;
    [TypeInfo(2)]
    public UInt32   dwFaceID;
	[TypeInfo(3,FishDataInfo.NickNameLength + 1)]
	public string   NickName;
};
public class NiuNiuUserGlobelInfo
{
	[TypeInfo(0)]
	public UInt32	dwUserID;
	[TypeInfo(1)]
	public UInt32	dwGlobelSum;
};
public class CL_Cmd_RoleJoinTable : NetCmdBase
{

}
public class LC_Cmd_RoleJoinTable : NetCmdBase
{
    [TypeInfo(0)]
	public bool			Result;
	//具体数据 桌子上的数据
	[TypeInfo(1)]
	public Byte			TableStates;//桌子状态
	[TypeInfo(2)]
	public Byte			TableStatesUpdateSec;//桌子当前状态的秒数
	[TypeInfo(3,FishDataInfo.MAX_NIUNIU_ClientSum)]
	public UInt64[]		TableBetAreaGlobel;//下注区金币数量
	[TypeInfo(4,(FishDataInfo.MAX_NIUNIU_ClientSum +1) * FishDataInfo.MAX_NIUNIU_BrandSum)]
    public Byte[]       TableBrandArray;//桌子上 5x5 牌
	[TypeInfo(5,FishDataInfo.MAX_NIUNIU_ClientSum +1)]
	public UInt16[]		TableBrandResult;//牌的结果
	[TypeInfo(6)]
	public NiuNiuRoleInfo  TableBankerInfo;
	[TypeInfo(7)]
	public Byte			TableBankerUseGameSum;//当前庄家已经进行游戏的局数
	[TypeInfo(8,FishDataInfo.MAX_NIUNIU_BankerShowSum)]
    public NiuNiuRoleInfo[] TableWriteBankerList;
    [TypeInfo(9,FishDataInfo.MAX_NIUNIU_VipSeat)]
    public NiuNiuRoleInfo[]	VipSeatList;//贵宾席的玩家
	[TypeInfo(10)]
	public Byte			TableGameSum;//记录游戏局数
	[TypeInfo(11)]
	public UInt32		TableResultLog;//游戏结果记录
}
public class CL_Cmd_RoleLeaveTable : NetCmdBase
{

}
public class CL_Cmd_RoleBetGlobel : NetCmdBase
{
    [TypeInfo(0)]
	public Byte			Index;
	[TypeInfo(1)]
	public UInt32		AddGlobel;
}
public class LC_Cmd_RoleBetGlobel : NetCmdBase
{
    [TypeInfo(0)]
	public UInt32			AddGlobel;
    [TypeInfo(1)]
    public Byte             Index;
	[TypeInfo(2)]
	public bool			    Result;
}
public class CL_Cmd_GetBankerList : NetCmdBase
{

}
public class LC_Cmd_GetBankerList : NetCmdBase
{
    [TypeInfo(0)]
	public UInt32			dwMySeatIndex;
	[TypeInfo(1,FishDataInfo.MAX_NIUNIU_BankerShowSum)]
    public NiuNiuRoleInfo[] TableWriteBankerList;
}
public class LC_Cmd_BankerListChange : NetCmdBase
{

}
public class LC_Cmd_BankerUserChange : NetCmdBase
{
    [TypeInfo(0)]
	public NiuNiuRoleInfo	BankerUserInfo;
	[TypeInfo(1)]
	public Byte			    GameSum;
}
public class LC_Cmd_TableJoinBegin : NetCmdBase
{

}
public class LC_Cmd_TableUpdate : NetCmdBase
{
    [TypeInfo(0,FishDataInfo.MAX_NIUNIU_ClientSum)]
    public UInt64[] TableBetAreaGlobel;
}
public class LC_Cmd_TableJoinEnd : NetCmdBase
{
    [TypeInfo(0)]
	public Int64		AddGlobelSum;
	[TypeInfo(1,FishDataInfo.MAX_NIUNIU_ClientSum)]
    public UInt64[] TableBetAreaGlobel;
	[TypeInfo(2,(FishDataInfo.MAX_NIUNIU_ClientSum +1) * FishDataInfo.MAX_NIUNIU_BrandSum)]
    public Byte[] TableBrandArray;//桌子上 5x5 牌
	[TypeInfo(3,FishDataInfo.MAX_NIUNIU_ClientSum +1)]
    public UInt16[] TableBrandResult;//牌的结果
    [TypeInfo(4)]
    public Byte TableGameSum;//记录游戏局数
    [TypeInfo(5)]
    public UInt32 TableResultLog;//游戏结果记录
    [TypeInfo(6)]
    public NiuNiuUserGlobelInfo BankerUserGlobelInfo;
	[TypeInfo(7,FishDataInfo.MAX_NIUNIU_BankerShowSum)]
    public NiuNiuUserGlobelInfo[] BankerListGlobelInfo;
	[TypeInfo(8,FishDataInfo.MAX_NIUNIU_VipSeat)]
    public NiuNiuUserGlobelInfo[] VipGlobelInfo;
}

public class CL_Cmd_RoleJoinBankerList : NetCmdBase
{

}
public class LC_Cmd_RoleJoinBankerList : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
	[TypeInfo(1)]
	public UInt32	SeatIndex;
}
public class CL_Cmd_RoleGetBankerFirstSeat : NetCmdBase
{

}
public class LC_Cmd_RoleGetBankerFirstSeat : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class CL_Cmd_RoleLeaveBankerList : NetCmdBase
{

}
public class LC_Cmd_RoleLeaveBankerList : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class CL_Cmd_RoleCanelBankerSeat : NetCmdBase
{

}
public class LC_Cmd_RoleCanelBankerSeat : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class LC_Cmd_BankerUserGlobelChange : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwBankerUserID;
    [TypeInfo(1)]
    public UInt32 dwBankerGlobelSum;
}
public class LC_Cmd_WriteBankerUserGlobelMsg : NetCmdBase
{

}


public class CL_Cmd_RoleJoinVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public Byte VipSeatIndex;
}
public class LC_Cmd_RoleJoinVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public Byte VipSeatIndex;
    [TypeInfo(2)]
    public NiuNiuRoleInfo DestUserInfo;
}
public class CL_Cmd_RoleLeaveVipSeat : NetCmdBase
{

}
public class LC_Cmd_RoleLeaveVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Byte VipSeatIndex;
}
public class LC_Cmd_RoleBeLeaveVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public NiuNiuRoleInfo DestRoleInfo;
}

public class CL_Cmd_GetNormalSeatInfo: NetCmdBase
{
    [TypeInfo(0)]
    public  UInt32 Page;
}
public class LC_Cmd_GetNormalSeatInfo : NetCmdBase
{
    [TypeInfo(0)]
    public  UInt32			Page;
    [TypeInfo(1)]
    public UInt32           TotalRoleSum;
	[TypeInfo(2,FishDataInfo.MAX_NORMAL_PAGESUM)]
    public NiuNiuRoleInfo[]  Array;
}
public class LC_Cmd_VipSeatGlobelChange: NetCmdBase
{
    [TypeInfo(0)]
    public Byte VipSeat;
    [TypeInfo(1)]
    public Byte Index;
    [TypeInfo(2)]
    public UInt32 GlobelSum;
}
public class LC_Cmd_TableStopUpdate: NetCmdBase
{

}
public class LC_Cmd_VipGlobelChange : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 VipUserID;
    [TypeInfo(1)]
    public UInt32 VipGlobelSum;
}
//char
public class tagRoleCharInfo  //一条聊天信息 好友的数据 所以 无须携带其他玩家相关的数据
{
	[TypeInfo(0)]
    public  UInt32		SrcUserID;//发送方
	[TypeInfo(1)]
    public  UInt32		DestUserID;//接收方
	[TypeInfo(2,FishDataInfo.MAX_ROLECHAR_LENGTH+1)]
    public  string		MessageInfo;//消息的主体
	[TypeInfo(3)]
    public  Int64       LogTime;//发送的时间
};
public class CL_Cmd_LoadAllCharInfo : NetCmdBase
{

}
public class LC_Cmd_LoadAllCharInfo : NetCmdBase
{
    [TypeInfo(0)]
    public Byte				    States;
	[TypeInfo(1)]
    public UInt16				Sum;
	[TypeInfo(2,true)]
    public tagRoleCharInfo[]		Array;
}
public class CL_Cmd_LoadCharListByUserID : NetCmdBase
{
    [TypeInfo(0)]
    public  UInt32				SrcUserID;
}
public class LC_Cmd_LoadCharListByUserID : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32           SrcUserID;
    [TypeInfo(1)]
    public Byte				States;
	[TypeInfo(2)]
    public UInt16			Sum;
    [TypeInfo(3, true)]
    public tagRoleCharInfo[] Array;
}
public class CL_Cmd_SendCharInfo : NetCmdBase
{
    [TypeInfo(0)]
    public tagRoleCharInfo MessageInfo;
}
public class LC_Cmd_SendCharInfo : NetCmdBase
{
    [TypeInfo(0)]
    public tagRoleCharInfo MessageInfo;
}



public class tagRelationRequest
{
	[TypeInfo(0)]
    public  UInt32			ID;
	[TypeInfo(1)]
    public  UInt32			SrcUserID;
	[TypeInfo(2)]
    public  UInt32			SrcFaceID;
	[TypeInfo(3,FishDataInfo.NickNameLength + 1)]
    public string		    SrcNickName;
    [TypeInfo(4)]
    public UInt16           SrcLevel;
    [TypeInfo(5)]
    public Byte             SrcVipLevel;
    [TypeInfo(6)]
    public bool             SrcGender;
    [TypeInfo(7,FishDataInfo.MAX_ADDRESS_LENGTH_IP+1)]
    public string            SrcIPAddress;
	[TypeInfo(8)]
    public  UInt32			DestUserID;
	[TypeInfo(9,FishDataInfo.MAX_RelationRequest_MsgLength + 1)]
    public string		    MessageInfo; 
	[TypeInfo(10)]
    public  Byte			RelationType;
	[TypeInfo(11)]
    public  Int64	        SendTime;
};
public class CL_Cmd_LoadRelationRequest : NetCmdBase
{

}
public class LC_Cmd_LoadRelationRequest : NetCmdBase
{
    [TypeInfo(0)]
    public Byte				    States;
	[TypeInfo(1)]
    public UInt16				Sum;
	[TypeInfo(2,true)]
    public tagRelationRequest[]	Array;
}
public class CL_Cmd_SendRelationRequest : NetCmdBase
{
    [TypeInfo(0)]
    public tagRelationRequest Info;
}
public class LC_Cmd_SendRelationRequest : NetCmdBase
{
    [TypeInfo(0)]
    public  bool Result;
    [TypeInfo(1)]
    public Byte ErrorID;
    [TypeInfo(2)]
    public tagRelationRequest Info;
}
public class LC_Cmd_BeAddRelationRequest : NetCmdBase
{
    [TypeInfo(0)]
    public tagRelationRequest Info;
}
public class CL_Cmd_HandleRelationRequest : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32					ID;
    [TypeInfo(1)]
    public bool Result;
}
public class LC_Cmd_HandleRelationRequest : NetCmdBase
{
    [TypeInfo(0)]
    public tagRelationRequest Info;
    [TypeInfo(1)]
    public bool Result;
}

public class LC_Cmd_DelRelationRequest : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 ID;
}

//Dial

public class DialRoleInfo
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public UInt32 GlobelSum;
    [TypeInfo(2)]
    public UInt32 dwFaceID;
    [TypeInfo(3, FishDataInfo.NickNameLength + 1)]
    public string NickName;
};
public class DialUserGlobelInfo
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public UInt32 dwGlobelSum;
};
public class CL_Cmd_DialRoleJoinTable : NetCmdBase
{

}
public class LC_Cmd_DialRoleJoinTable : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    //具体数据 桌子上的数据
    [TypeInfo(1)]
    public Byte TableStates;//桌子状态
    [TypeInfo(2)]
    public Byte TableStatesUpdateSec;//桌子当前状态的秒数
    [TypeInfo(3, FishDataInfo.MAX_Dial_ClientSum)]
    public UInt64[] TableBetAreaGlobel;//下注区金币数量
    [TypeInfo(4, FishDataInfo.MAX_Dial_GameSum)]
    public Byte[] TableAreaData;
    [TypeInfo(5, FishDataInfo.MAX_Dial_ClientSum)]
    public Byte[] TableAreaDataRate;//下注区金币数量
    [TypeInfo(6)]
    public DialRoleInfo TableBankerInfo;
    [TypeInfo(7)]
    public Byte TableBankerUseGameSum;//当前庄家已经进行游戏的局数
    [TypeInfo(8, FishDataInfo.MAX_Dial_BankerShowSum)]
    public DialRoleInfo[] TableWriteBankerList;
    [TypeInfo(9, FishDataInfo.MAX_Dial_VipSeat)]
    public DialRoleInfo[] VipSeatList;//贵宾席的玩家
    [TypeInfo(10)]
    public Byte TableGameSum;//记录游戏局数
    [TypeInfo(11)]
    public UInt32 TableResultLog;//游戏结果记录
    [TypeInfo(12)]
    public UInt16 TableResultIndex;//游戏结果记录
}
public class CL_Cmd_DialRoleLeaveTable : NetCmdBase
{

}
public class CL_Cmd_DialRoleBetGlobel : NetCmdBase
{
    [TypeInfo(0)]
    public Byte Index;
    [TypeInfo(1)]
    public UInt32 AddGlobel;
}
public class LC_Cmd_DialRoleBetGlobel : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 AddGlobel;
    [TypeInfo(1)]
    public Byte Index;
    [TypeInfo(2)]
    public bool Result;
}
public class CL_Cmd_DialGetBankerList : NetCmdBase
{

}
public class LC_Cmd_DialGetBankerList : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwMySeatIndex;
    [TypeInfo(1, FishDataInfo.MAX_Dial_BankerShowSum)]
    public DialRoleInfo[] TableWriteBankerList;
}
public class LC_Cmd_DialBankerListChange : NetCmdBase
{

}
public class LC_Cmd_DialBankerUserChange : NetCmdBase
{
    [TypeInfo(0)]
    public DialRoleInfo BankerUserInfo;
    [TypeInfo(1)]
    public Byte GameSum;
}
public class LC_Cmd_DialTableJoinBegin : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_Dial_GameSum)]
    public Byte[] TableAreaData;
    [TypeInfo(1, FishDataInfo.MAX_Dial_ClientSum)]
    public Byte[] TableAreaDataRate;//下注区金币数量
}
public class LC_Cmd_DialTableUpdate : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_Dial_ClientSum)]
    public UInt64[] TableBetAreaGlobel;
}
public class LC_Cmd_DialTableJoinEnd : NetCmdBase
{
    [TypeInfo(0)]
    public Int64 AddGlobelSum;
    [TypeInfo(1, FishDataInfo.MAX_Dial_ClientSum)]
    public UInt64[] TableBetAreaGlobel;
    [TypeInfo(2)]
    public UInt16 TableResultIndex;
    [TypeInfo(3)]
    public Byte  TableGameSum;
    [TypeInfo(4)]
    public UInt32  TableResultLog;
    [TypeInfo(5)]
    public DialUserGlobelInfo BankerUserGlobelInfo;
    [TypeInfo(6, FishDataInfo.MAX_Dial_BankerShowSum)]
    public DialUserGlobelInfo[] BankerListGlobelInfo;
    [TypeInfo(7, FishDataInfo.MAX_Dial_VipSeat)]
    public DialUserGlobelInfo[] VipGlobelInfo;
}

public class CL_Cmd_DialRoleJoinBankerList : NetCmdBase
{

}
public class LC_Cmd_DialRoleJoinBankerList : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public UInt32 SeatIndex;
}
public class CL_Cmd_DialRoleGetBankerFirstSeat : NetCmdBase
{

}
public class LC_Cmd_DialRoleGetBankerFirstSeat : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class CL_Cmd_DialRoleLeaveBankerList : NetCmdBase
{

}
public class LC_Cmd_DialRoleLeaveBankerList : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class CL_Cmd_DialRoleCanelBankerSeat : NetCmdBase
{

}
public class LC_Cmd_DialRoleCanelBankerSeat : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class LC_Cmd_DialBankerUserGlobelChange : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwBankerUserID;
    [TypeInfo(1)]
    public UInt32 dwBankerGlobelSum;
}
public class LC_Cmd_DialWriteBankerUserGlobelMsg : NetCmdBase
{

}


public class CL_Cmd_DialRoleJoinVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public Byte VipSeatIndex;
}
public class LC_Cmd_DialRoleJoinVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public Byte VipSeatIndex;
    [TypeInfo(2)]
    public DialRoleInfo DestUserInfo;
}
public class CL_Cmd_DialRoleLeaveVipSeat : NetCmdBase
{

}
public class LC_Cmd_DialRoleLeaveVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Byte VipSeatIndex;
}
public class LC_Cmd_DialRoleBeLeaveVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public DialRoleInfo DestRoleInfo;
}

public class CL_Cmd_DialGetNormalSeatInfo : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 Page;
}
public class LC_Cmd_DialGetNormalSeatInfo : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 Page;
    [TypeInfo(1)]
    public UInt32 TotalRoleSum;
    [TypeInfo(2, FishDataInfo.MAX_DialNORMAL_PAGESUM)]
    public DialRoleInfo[] Array;
}
public class LC_Cmd_DialVipSeatGlobelChange : NetCmdBase
{
    [TypeInfo(0)]
    public Byte VipSeat;
    [TypeInfo(1)]
    public Byte Index;
    [TypeInfo(2)]
    public UInt32 GlobelSum;
}
public class LC_Cmd_DialTableStopUpdate : NetCmdBase
{

}
public class LC_Cmd_DialVipGlobelChange : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 VipUserID;
    [TypeInfo(1)]
    public UInt32 VipGlobelSum;
}

public class CL_Cmd_DialRoleBetGlobelByLog: NetCmdBase
{
    [TypeInfo(0,FishDataInfo.MAX_Dial_ClientSum)]
    public  UInt32[]			betGlobel;
}
public class LC_Cmd_DialRoleBetGlobelByLog : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1, FishDataInfo.MAX_Dial_ClientSum)]
    public UInt32[] betGlobel;
}


//Car

public class CarRoleInfo
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public UInt32 GlobelSum;
    [TypeInfo(2)]
    public UInt32 dwFaceID;
    [TypeInfo(3, FishDataInfo.NickNameLength + 1)]
    public string NickName;
};
public class CarUserGlobelInfo
{
    [TypeInfo(0)]
    public UInt32 dwUserID;
    [TypeInfo(1)]
    public UInt32 dwGlobelSum;
};
public class CL_Cmd_CarRoleJoinTable : NetCmdBase
{

}
public class LC_Cmd_CarRoleJoinTable : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    //具体数据 桌子上的数据
    [TypeInfo(1)]
    public Byte TableStates;//桌子状态
    [TypeInfo(2)]
    public Byte TableStatesUpdateSec;//桌子当前状态的秒数
    [TypeInfo(3, FishDataInfo.MAX_Car_ClientSum)]
    public UInt64[] TableBetAreaGlobel;//下注区金币数量
    [TypeInfo(4)]
    public CarRoleInfo TableBankerInfo;
    [TypeInfo(5)]
    public Byte TableBankerUseGameSum;//当前庄家已经进行游戏的局数
    [TypeInfo(6, FishDataInfo.MAX_Car_BankerShowSum)]
    public CarRoleInfo[] TableWriteBankerList;
    [TypeInfo(7, FishDataInfo.MAX_Car_VipSeat)]
    public CarRoleInfo[] VipSeatList;//贵宾席的玩家
    [TypeInfo(8)]
    public Byte TableGameSum;//记录游戏局数
    [TypeInfo(9)]
    public UInt32 TableResultLog;//游戏结果记录
    [TypeInfo(10)]
    public Byte TableResultIndex;//游戏结果记录
}
public class CL_Cmd_CarRoleLeaveTable : NetCmdBase
{

}
public class CL_Cmd_CarRoleBetGlobel : NetCmdBase
{
    [TypeInfo(0)]
    public Byte Index;
    [TypeInfo(1)]
    public UInt32 AddGlobel;
}
public class LC_Cmd_CarRoleBetGlobel : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 AddGlobel;
    [TypeInfo(1)]
    public Byte Index;
    [TypeInfo(2)]
    public bool Result;
}
public class CL_Cmd_CarGetBankerList : NetCmdBase
{

}
public class LC_Cmd_CarGetBankerList : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwMySeatIndex;
    [TypeInfo(1, FishDataInfo.MAX_Car_BankerShowSum)]
    public CarRoleInfo[] TableWriteBankerList;
}
public class LC_Cmd_CarBankerListChange : NetCmdBase
{

}
public class LC_Cmd_CarBankerUserChange : NetCmdBase
{
    [TypeInfo(0)]
    public CarRoleInfo BankerUserInfo;
    [TypeInfo(1)]
    public Byte GameSum;
}
public class LC_Cmd_CarTableJoinBegin : NetCmdBase
{
    //[TypeInfo(0, FishDataInfo.MAX_Car_GameSum)]
    //public Byte[] TableAreaData;
    [TypeInfo(0, FishDataInfo.MAX_Car_ClientSum)]
    public Byte[] TableAreaDataRate;//下注区金币数量
}
public class LC_Cmd_CarTableUpdate : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_Car_ClientSum)]
    public UInt64[] TableBetAreaGlobel;
}
public class LC_Cmd_CarTableJoinEnd : NetCmdBase
{
    [TypeInfo(0)]
    public Int64 AddGlobelSum;
    [TypeInfo(1, FishDataInfo.MAX_Car_ClientSum)]
    public UInt64[] TableBetAreaGlobel;
    [TypeInfo(2)]
    public Byte TableResultIndex;
    [TypeInfo(3)]
    public Byte TableGameSum;
    [TypeInfo(4)]
    public UInt32 TableResultLog;
    [TypeInfo(5)]
    public CarUserGlobelInfo BankerUserGlobelInfo;
    [TypeInfo(6, FishDataInfo.MAX_Car_BankerShowSum)]
    public CarUserGlobelInfo[] BankerListGlobelInfo;
    [TypeInfo(7, FishDataInfo.MAX_Car_VipSeat)]
    public CarUserGlobelInfo[] VipGlobelInfo;
}

public class CL_Cmd_CarRoleJoinBankerList : NetCmdBase
{

}
public class LC_Cmd_CarRoleJoinBankerList : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public UInt32 SeatIndex;
}
public class CL_Cmd_CarRoleGetBankerFirstSeat : NetCmdBase
{

}
public class LC_Cmd_CarRoleGetBankerFirstSeat : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class CL_Cmd_CarRoleLeaveBankerList : NetCmdBase
{

}
public class LC_Cmd_CarRoleLeaveBankerList : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class CL_Cmd_CarRoleCanelBankerSeat : NetCmdBase
{

}
public class LC_Cmd_CarRoleCanelBankerSeat : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
}
public class LC_Cmd_CarBankerUserGlobelChange : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwBankerUserID;
    [TypeInfo(1)]
    public UInt32 dwBankerGlobelSum;
}
public class LC_Cmd_CarWriteBankerUserGlobelMsg : NetCmdBase
{

}


public class CL_Cmd_CarRoleJoinVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public Byte VipSeatIndex;
}
public class LC_Cmd_CarRoleJoinVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1)]
    public Byte VipSeatIndex;
    [TypeInfo(2)]
    public CarRoleInfo DestUserInfo;
}
public class CL_Cmd_CarRoleLeaveVipSeat : NetCmdBase
{

}
public class LC_Cmd_CarRoleLeaveVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 dwDestUserID;
    [TypeInfo(1)]
    public Byte VipSeatIndex;
}
public class LC_Cmd_CarRoleBeLeaveVipSeat : NetCmdBase
{
    [TypeInfo(0)]
    public CarRoleInfo DestRoleInfo;
}

public class CL_Cmd_CarGetNormalSeatInfo : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 Page;
}
public class LC_Cmd_CarGetNormalSeatInfo : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 Page;
    [TypeInfo(1)]
    public UInt32 TotalRoleSum;
    [TypeInfo(2, FishDataInfo.MAX_CarNORMAL_PAGESUM)]
    public CarRoleInfo[] Array;
}
public class LC_Cmd_CarVipSeatGlobelChange : NetCmdBase
{
    [TypeInfo(0)]
    public Byte VipSeat;
    [TypeInfo(1)]
    public Byte Index;
    [TypeInfo(2)]
    public UInt32 GlobelSum;
}
public class LC_Cmd_CarTableStopUpdate : NetCmdBase
{

}
public class LC_Cmd_CarVipGlobelChange : NetCmdBase
{
    [TypeInfo(0)]
    public UInt32 VipUserID;
    [TypeInfo(1)]
    public UInt32 VipGlobelSum;
}

public class CL_Cmd_CarRoleBetGlobelByLog : NetCmdBase
{
    [TypeInfo(0, FishDataInfo.MAX_Car_ClientSum)]
    public UInt32[] betGlobel;
}
public class LC_Cmd_CarRoleBetGlobelByLog : NetCmdBase
{
    [TypeInfo(0)]
    public bool Result;
    [TypeInfo(1, FishDataInfo.MAX_Car_ClientSum)]
    public UInt32[] betGlobel;
}