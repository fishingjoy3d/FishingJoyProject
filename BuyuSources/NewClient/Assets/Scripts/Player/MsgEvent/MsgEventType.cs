using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
public enum MsgEventType
{
    MET_Null = 0,

    //MET_Role_ShowInfo,

   // MET_ErrorCode,
    MET_UserOperation,

    //物品
    MET_Item_Change,  //玩家背包里的物品发送变化了  可能是有物品 添加 物品删除 物品改变 物品到期等
    MET_Item_Use,
    MET_Item_Accept,//赠送物品

    //邮件
    MET_MailTitle_Change,  //玩家有邮件标题发生变化了 如邮件里的物品被领取了  邮件修改为已读 收到新的邮件 等
    MET_MailTitle_Add   ,
    MET_MailTitle_Del    ,
    Met_MailGet_Item,   //系统邮件获取物品

    MET_MailContext_Change ,//邮件主体变化了 物品被领取  状态为已读
    MET_MailContext_Add ,//有新的邮件主体添加了
    MET_MailContext_Del ,//有新的邮件主体删除了

    MET_MailContext_AllChange,
    MET_MailTitle_NonRead,

    //关系
    MET_Friend_Load,
    MET_Friend_Change, //好友列表关系发生变化了 如有好友删除了 好友添加 好友数据变化了
    MET_Friend_Add,
    MET_Friend_Del,

    MET_Black_Load,
    MET_Black_Change,//黑名单 列表变化了 如黑名单 添加 删除 修改了
    MET_Black_Add,
    MET_Black_Del,

    //玩家
    MET_Role_Change,//玩家自己的属性变化了 如名称 等级 金币 奖牌 乐币
    MET_Role_LevelChange,
    MET_Role_Protect,
    MET_Role_MonthCardChange,
    MET_Role_RateChange,
    MET_MonthCard_GetReward,
    MET_Role_VipChange,
    MET_BindPhone_Reset,
    MET_Role_ChangeSecPwd,

    MET_Role_OpenShareUI,

    //查询
    MET_Query_Change,//查询时间 当玩家进行查询后 异步进行通知 查询完成了
    //
    MET_Query_Role, //查询某个玩家

    //签到
    MET_Check_Change ,//签到数据 变化了 
    MET_Check_CheckNowDay,
    MET_Check_CheckOtherDay,

    //任务
    MET_Task_Change ,//任务数据变化了
    MET_Task_OnceChange,//单任务变化了
    MET_Task_EventFinish,
    MET_Task_FinihStates,
    //成就
    MET_Achievement_OnceChange,
    MET_Achievement_Change , //成就数据变化了
    MET_Achievement_EventFinish,
    MET_Achievement_FinihStates,

    //活动
    MET_Action_OnceChange,
    MET_Action_Change ,//活动数据变化了
    MET_Action_FinihStates,
    MET_Action_OnceReward,

    //桌子
    MET_Table_Add ,//有其他玩家进入桌子的事件
    MET_Table_Del ,//其他玩家离开桌子事件
    MET_Table_Change ,//其他桌子上玩家属性变化的事件
    MET_Table_LevelChange,

    //比赛
    MET_Month_Add , //接收到一个新的玩家的比赛属性
    MET_Month_Change ,//比赛属性变化了
    MET_Match_ScoreAdd,     //比赛中通过打鱼金币增加
    MET_Month_SignUpChange ,//玩家比赛报名列表变化了
    MET_Month_SignOnceUpChange,
    MET_Month_SignUpSumChange ,//比赛报名 人数变化了
    MET_Month_RankXmlChange ,//比赛的排行榜数据变化了
    MET_Month_End,
    MET_Match_ContinueGold,  //玩家续币成功
    MET_Match_ListChange,//列表变化事件

    //实体数据
    MET_Entity_Change ,//玩家实体数据变化了
    MET_Entity_BindPhone,

    //称号
    MET_Title_Change ,//玩家获得新的称号的时候 或者 失去称号

    //排行榜
    MET_Rank_Change ,//玩家获得服务器端玩家上周排行榜奖励的事件
    MET_Rank_GetReward ,//玩家领取排行榜奖励的变化
    MET_Rank_XmlChange ,//排行榜的排行榜文件变化了
    MET_Rank_ExistsReward,

    //宝箱
    MET_Chest_Action ,//宝箱被激活了
    MET_Chest_End ,//宝箱到期了
    MET_Chest_Reward ,//获取宝箱奖励了
    MET_Chest_Reset,

    //赠送
    MET_Giff_Add ,//获得一个赠送
    MET_Giff_Del ,//删除一个赠送
    MET_Giff_Change ,//赠送改变了  表示赠送列表发送变化了 可能涉及多个赠送
    MET_Giff_SendInfoChange ,//玩家今天发生赠送的列表 发送变化了 或者是获取了
    MET_Giff_SendInfoOnceChange,
    MET_Giff_ExistsChange,

    //商店
    MET_Shop_ShopResult ,//玩家购买物品的结果

    //在线奖励
    MET_OnlineReward_Change,
    MET_OnlineReward_Once,

    //GameData
    MET_GameData_Change,

    //charm
    MET_Charm_RewardOnce,

    MET_Charm_UserChange,

    //screen
    MET_Game_RateChange,

    //Launcher
    MET_Launcher_DataChange,

    //Recharge
    MET_Recharge_Result,
    MET_Recharge_GetOrderID,
    MET_Recharge_AddOrderInfo,

    //Announcement
    MET_Announcement_Change,

    //Package
    MET_Package_Result,

    //Table
    MET_Table_Join,

    //RoleStatesMessage
    MET_RoleStates_Change,

    //Login
    MET_Logon_ReturnLogon,
    MET_Logon_Freeze,

    //Queue
    MET_Queue_Logon,

    //ExChange
    MET_Use_ExChange,

    //Notice
    MET_Notice_Once,

    //Lottery
    MET_Lottery_Result,
    MET_Lottery_ScoreChange,
    MET_Table_LotteryUIStates,

    //NiuNiu
    MET_NiuNiu_Join,
    MET_NiuNiu_AddGlobel,
    MET_NiuNiu_Update,
    MET_NiuNiu_StatesChange,

    MET_NiuNiu_UpdateWriteBankerList,
    MET_NiuNiu_BankerUserChange,
    MET_NiuNiu_WriteBankerGlobelMsg,

    MET_NiuNiu_VipSeatChange,

    MET_NiuNiu_BeLeaveVip,

    MET_NiuNiu_NormalSeatInfo,
    MET_NiuNiu_VipGetGlobel,

    MET_NiuNiu_TableStopUpdate,

    MET_NiuNiu_VipGlobelChange,
    //Char
    MET_Char_ListChange,
    MET_Char_ContectChange,
    MET_Char_AddChar,

    //RelationRequest
    MET_RelationRequest_Change,
    MET_RelationRequest_SendResult,
    MET_RelationRequest_HandleResult,

    //Dial
    MET_Dial_Join,
    MET_Dial_AddGlobel,
    MET_Dial_Update,
    MET_Dial_StatesChange,
        
    MET_Dial_UpdateWriteBankerList,
    MET_Dial_BankerUserChange,
    MET_Dial_WriteBankerGlobelMsg,
        
    MET_Dial_VipSeatChange,
        
    MET_Dial_BeLeaveVip,
        
    MET_Dial_NormalSeatInfo,
    MET_Dial_VipGetGlobel,
        
    MET_Dial_TableStopUpdate,
        
    MET_Dial_VipGlobelChange,

    //Car
    MET_Car_Join,
    MET_Car_AddGlobel,
    MET_Car_Update,
    MET_Car_StatesChange,

    MET_Car_UpdateWriteBankerList,
    MET_Car_BankerUserChange,
    MET_Car_WriteBankerGlobelMsg,

    MET_Car_VipSeatChange,

    MET_Car_BeLeaveVip,

    MET_Car_NormalSeatInfo,
    MET_Car_VipGetGlobel,

    MET_Car_TableStopUpdate,

    MET_Car_VipGlobelChange,

    MET_Max,
}
public class tagMsgEventBase
{
    private MsgEventType Type;
    internal MsgEventType GetMsgEventType
    {
        get { return Type; }
    }
    public tagMsgEventBase(MsgEventType nType)
    {
        Type = nType;
    }

    public void Handle()
    {
        MsgEventHandle.HandleMsg(this);
    }
}
//public class tagErrorCodeEvent : tagMsgEventBase
//{
//    public ErrorCode pCode;
//    public tagErrorCodeEvent(ErrorCode Code)
//        : base(MsgEventType.MET_ErrorCode)
//    {
//        pCode = Code;
//    }
//}

public class tagUserOperationEvent : tagMsgEventBase
{
    public UserOperateMessage pCode;
    public string[] StringParam = null;
    public tagUserOperationEvent(UserOperateMessage Code,params string[] ParamArray)
        : base(MsgEventType.MET_UserOperation)
    {
        pCode = Code;
        StringParam = ParamArray;
    }
}


//物品的事件
class tagItemChangeEvent : tagMsgEventBase
{
    public tagItemChangeEvent()
        : base(MsgEventType.MET_Item_Change)
    {
        //无须携带参数
    }
}
class tagItemUseEvent : tagMsgEventBase
{
    public bool Result;
    public UInt32 ItemOnlyID;
    public UInt32 ItemID;
    public UInt32 ItemSum;
    public tagItemUseEvent(bool Result, UInt32 ItemOnlyID, UInt32 ItemID, UInt32 ItemSum)
        : base(MsgEventType.MET_Item_Use)
    {
       this.Result= Result;
       this.ItemOnlyID = ItemOnlyID;
       this.ItemID = ItemID;
       this.ItemSum = ItemSum;
    }
}
class tagItemAcceptEvent : tagMsgEventBase
{
    public UInt32 DestUserID;
    public bool Result;
    public UInt32 ItemOnlyID;
    public UInt32 ItemID;
    public UInt32 ItemSum;
    public tagItemAcceptEvent(UInt32 DestUserID, bool Result, UInt32 ItemOnlyID, UInt32 ItemID, UInt32 ItemSum)
        : base(MsgEventType.MET_Item_Accept)
    {
       this.DestUserID = DestUserID;
       this.Result= Result;
       this.ItemOnlyID = ItemOnlyID;
       this.ItemID = ItemID;
       this.ItemSum = ItemSum;
    }
}


//邮件事件
class tagMailTitleChangeEvent : tagMsgEventBase
{
    public tagMailTitleChangeEvent()
        : base(MsgEventType.MET_MailTitle_Change)
    {
        //无须携带参数
    }
}
class tagMailTitleAddEvent : tagMsgEventBase
{
    public UInt32 pMailID;
    public bool IsSystemMail;
    public tagMailTitleAddEvent(UInt32 MailID,bool bIsSystem)
        : base(MsgEventType.MET_MailTitle_Add)
    {
        //无须携带参数
        pMailID = MailID;
        IsSystemMail = bIsSystem;
    }
}
//邮件领取物品成功
class tagMailGetItemEvent: tagMsgEventBase
{
    public tagMailGetItemEvent() : base(MsgEventType.Met_MailGet_Item)
    {

    }
}

class tagMailTitleDelEvent : tagMsgEventBase
{
    public UInt32 pMailID;
    public tagMailTitleDelEvent(UInt32 MailID)
        : base(MsgEventType.MET_MailTitle_Del)
    {
        //无须携带参数
        pMailID = MailID;
    }
}
class tagMailContextChangeEvent : tagMsgEventBase
{
    public UInt32 pMailID;
    public tagMailContextChangeEvent(UInt32 MailID)
        : base(MsgEventType.MET_MailContext_Change)
    {
        //无须携带参数
        pMailID = MailID;
    }
}
class tagMailContextAddEvent : tagMsgEventBase
{
    public UInt32 pMailID;
    public tagMailContextAddEvent(UInt32 MailID)
        : base(MsgEventType.MET_MailContext_Add)
    {
        //无须携带参数
        pMailID = MailID;
    }
}
class tagMailContextDelEvent : tagMsgEventBase
{
    public UInt32 pMailID;
    public tagMailContextDelEvent(UInt32 MailID)
        : base(MsgEventType.MET_MailContext_Del)
    {
        //无须携带参数
        pMailID = MailID;
    }
}
class tagMailAllContextChangeEvent : tagMsgEventBase
{
    public tagMailAllContextChangeEvent()
        : base(MsgEventType.MET_MailContext_AllChange)
    {
    }
}

class tagMailNonReadEvent : tagMsgEventBase
{
    public tagMailNonReadEvent()
        : base(MsgEventType.MET_MailTitle_NonRead)
    {
    }
}


//好友关系
class tagFriendLoadEvent : tagMsgEventBase
{
    public tagFriendLoadEvent()
        : base(MsgEventType.MET_Friend_Load)
    {
        //无须携带参数
    }
}
class tagFriendChangeEvent : tagMsgEventBase
{
    public UInt32 UserID;
    public tagFriendChangeEvent(UInt32 pUserID)
        : base(MsgEventType.MET_Friend_Change)
    {
        //无须携带参数
        UserID = pUserID;
    }
}
class tagFriendAddEvent : tagMsgEventBase
{
    public UInt32 UserID;
    public tagFriendAddEvent(UInt32 pUserID)
        : base(MsgEventType.MET_Friend_Add)
    {
        //无须携带参数
        UserID = pUserID;
    }
}
class tagFriendDelEvent : tagMsgEventBase
{
    public UInt32 UserID;
    public tagFriendDelEvent(UInt32 pUserID)
        : base(MsgEventType.MET_Friend_Del)
    {
        //无须携带参数
        UserID = pUserID;
    }
}
class tagBlackLoadEvent : tagMsgEventBase
{
    public tagBlackLoadEvent()
        : base(MsgEventType.MET_Black_Load)
    {
        //无须携带参数
    }
}
class tagBlackChangeEvent : tagMsgEventBase
{
    public UInt32 UserID;
    public tagBlackChangeEvent(UInt32 pUserID)
        : base(MsgEventType.MET_Black_Change)
    {
        //无须携带参数
        UserID = pUserID;
    }
}
class tagBlackAddEvent : tagMsgEventBase
{
    public UInt32 UserID;
    public tagBlackAddEvent(UInt32 pUserID)
        : base(MsgEventType.MET_Black_Add)
    {
        //无须携带参数
        UserID = pUserID;
    }
}
class tagBlackDelEvent : tagMsgEventBase
{
    public UInt32 UserID;
    public tagBlackDelEvent(UInt32 pUserID)
        : base(MsgEventType.MET_Black_Del)
    {
        //无须携带参数
        UserID = pUserID;
    }
}

//玩家
class tagRoleChangeEvent : tagMsgEventBase
{
    public tagRoleChangeEvent()
        : base(MsgEventType.MET_Role_Change)
    {
        //无须携带参数
    }
}
class tagRoleLevelChangeEvent : tagMsgEventBase
{
    public UInt16 OldLevel;
    public UInt16 NowLevel;
    public UInt16 RewardID;
    public tagRoleLevelChangeEvent(UInt16 OldLevel,UInt16 NowLevel,UInt16 RewardID)
        : base(MsgEventType.MET_Role_LevelChange)
    {
        this.OldLevel = OldLevel;
        this.NowLevel = NowLevel;
        this.RewardID = RewardID;
    }
}

class tagRoleProtectEvent : tagMsgEventBase
{
    public tagRoleProtectEvent()
        : base(MsgEventType.MET_Role_Protect)
    {
       
        //无须携带参数
    }
    public byte nCount;
    public ushort ncd;
}
class tagRoleMonthCardChangeEvent : tagMsgEventBase
{
    public tagRoleMonthCardChangeEvent()
        : base(MsgEventType.MET_Role_MonthCardChange)
    {
        //无须携带参数
    }
}
class tagRoleRateValueChangeEvent : tagMsgEventBase
{
    public Byte RateIndex;
    public tagRoleRateValueChangeEvent(Byte RateIndex)
        : base(MsgEventType.MET_Role_RateChange)
    {
        //无须携带参数
        this.RateIndex = RateIndex;
    }
}

class tagRoleMonthCardRewardEvent : tagMsgEventBase
{
    bool Result;
    public tagRoleMonthCardRewardEvent( bool Result)
        : base(MsgEventType.MET_MonthCard_GetReward)
    {
        this.Result = Result;
    }
}

class tagRoleVipChangeEvent : tagMsgEventBase
{
    public tagRoleVipChangeEvent()
        : base(MsgEventType.MET_Role_VipChange)
    {
    }
}

class tagRoleResetBindPhoneEvent : tagMsgEventBase
{
    public tagRoleResetBindPhoneEvent()
        : base(MsgEventType.MET_BindPhone_Reset)
    {
    }
}

class tagRoleChangeSecPwdEvent : tagMsgEventBase
{
    public bool Result;
    public tagRoleChangeSecPwdEvent(bool Result)
        : base(MsgEventType.MET_Role_ChangeSecPwd)
    {
        this.Result = Result;
    }
}
class tagRoleOpenShareUIEvent : tagMsgEventBase
{
    public tagRoleOpenShareUIEvent()
        : base(MsgEventType.MET_Role_OpenShareUI)
    {
    }
}


//查询
class tagQueryChangeEvent : tagMsgEventBase
{
    public tagQueryChangeEvent()
        : base(MsgEventType.MET_Query_Change)
    {
        //无须携带参数
    }
}
class tagQueryRoleEvent : tagMsgEventBase
{
    public QueryRole m_Role;
    public tagQueryRoleEvent(QueryRole pRole)
        : base (MsgEventType.MET_Query_Role)
    {
        m_Role = pRole;
    }
}
//签到
class tagCheckChangeEvent : tagMsgEventBase
{
    public tagCheckChangeEvent()
        : base(MsgEventType.MET_Check_Change)
    {
        //无须携带参数
    }
}
class tagCheckCheckNowDayEvent : tagMsgEventBase
{
    public UInt16 DayReawrdID;
    public UInt16 MonthRewardID;
    public tagCheckCheckNowDayEvent(UInt16 DayReawrdID,UInt16 MonthRewardID)
        : base(MsgEventType.MET_Check_CheckNowDay)
    {
        this.DayReawrdID = DayReawrdID;
        this.MonthRewardID = MonthRewardID;
    }
}
class tagCheckCheckOtherDayEvent : tagMsgEventBase
{
    public Byte Day;
    public UInt16 DayReawrdID;
    public UInt16 MonthRewardID;
    public tagCheckCheckOtherDayEvent(Byte Day,UInt16 DayReawrdID,UInt16 MonthRewardID)
        : base(MsgEventType.MET_Check_CheckOtherDay)
    {
        this.Day = Day;
        this.DayReawrdID = DayReawrdID;
        this.MonthRewardID = MonthRewardID;
    }
}
//任务
class tagOnceTaskChangeEvent : tagMsgEventBase
{
    public Byte pTaskID;
    public tagOnceTaskChangeEvent(Byte TaskID)
        : base(MsgEventType.MET_Task_OnceChange)
    {
        pTaskID = TaskID;
    }
}
class tagOnceTaskEventFinishEvent : tagMsgEventBase
{
    public Byte TaskID;
    public tagOnceTaskEventFinishEvent(Byte TaskID)
        : base(MsgEventType.MET_Task_EventFinish)
    {
        this.TaskID = TaskID;
    }    
}
class tagTaskFinishStatesChange : tagMsgEventBase
{
    public tagTaskFinishStatesChange()
        : base(MsgEventType.MET_Task_FinihStates)
    {
    }    
}

class tagTaskChangeEvent : tagMsgEventBase
{
    public tagTaskChangeEvent()
        : base(MsgEventType.MET_Task_Change)
    {
        //无须携带参数
    }
}
//成就
class tagOnceAchievementChangeEvent : tagMsgEventBase
{
    public Byte pAchievement;
    public tagOnceAchievementChangeEvent(Byte AchievementID)
        : base(MsgEventType.MET_Achievement_OnceChange)
    {
        pAchievement = AchievementID;
    }
}
class tagAchievementChangeEvent : tagMsgEventBase
{
    public tagAchievementChangeEvent()
        : base(MsgEventType.MET_Achievement_Change)
    {
        //无须携带参数
    }
}
class tagAchievementEventFinishEvent : tagMsgEventBase
{
    public Byte pAchievement;
    public tagAchievementEventFinishEvent(Byte AchievementID)
        : base(MsgEventType.MET_Achievement_EventFinish)
    {
        pAchievement = AchievementID;
    }
}
class tagAchievementFinishStatesChagneEvent : tagMsgEventBase
{
    public tagAchievementFinishStatesChagneEvent()
        : base(MsgEventType.MET_Achievement_FinihStates)
    {
    }
}


//活动
class tagOnceActionChangeEvent : tagMsgEventBase
{
    public Byte pAction;
    public tagOnceActionChangeEvent(Byte ActionID)
        : base(MsgEventType.MET_Action_OnceChange)
    {
        pAction = ActionID;
    }
}
class tagActionChangeEvent : tagMsgEventBase
{
    public tagActionChangeEvent()
        : base(MsgEventType.MET_Action_Change)
    {
        //无须携带参数
    }
}
class tagActionFinishStatesChagneEvent : tagMsgEventBase
{
    public tagActionFinishStatesChagneEvent()
        : base(MsgEventType.MET_Action_FinihStates)
    {
    }
}
class tagActionOnceRewardEvent : tagMsgEventBase
{
    public Byte ActionID;
    public UInt32 EventID;
    public UInt16 RewardID;
    public tagActionOnceRewardEvent(Byte ActionID,  UInt32 EventID,UInt16 RewardID)
        : base(MsgEventType.MET_Action_OnceReward)
    {
        this.ActionID = ActionID;
        this.EventID = EventID;
        this.RewardID = RewardID;
    }
}

//桌子
class tagTableAddEvent : tagMsgEventBase
{
    private UInt32 dwUserID = 0;
    public tagTableAddEvent(UInt32 UserID)
        : base(MsgEventType.MET_Table_Add)
    {
        dwUserID = UserID;
    }
}
class tagTableDelEvent : tagMsgEventBase
{
    public UInt32 dwUserID = 0;
    public tagTableDelEvent(UInt32 UserID)
        : base(MsgEventType.MET_Table_Del)
    {
        dwUserID = UserID;
    }
}
class tagTableChangeEvent : tagMsgEventBase
{
    public UInt32 dwUserID = 0;
    public tagTableChangeEvent(UInt32 UserID)
        : base(MsgEventType.MET_Table_Change)
    {
        dwUserID = UserID;
    }
}
class tagTableLevelChangeEvent : tagMsgEventBase
{
    public UInt32 dwUserID = 0;
    public UInt16 OldLevel;
    public UInt16 NewLevel;
    public tagTableLevelChangeEvent(UInt32 UserID,UInt16 OldLevel,UInt16 NewLevel)
        : base(MsgEventType.MET_Table_LevelChange)
    {
        dwUserID = UserID;
        this.OldLevel = OldLevel;
        this.NewLevel = NewLevel;
    }
}

//比赛
class tagMonthAddEvent : tagMsgEventBase
{
    public UInt32 dwUserID = 0;
    public tagMonthAddEvent(UInt32 UserID)
        : base(MsgEventType.MET_Month_Add)
    {
        dwUserID = UserID;
    }
}
class tagMonthChangeEvent : tagMsgEventBase
{
    public UInt32 dwUserID = 0;
    public tagMonthChangeEvent(UInt32 UserID)
        : base(MsgEventType.MET_Month_Change)
    {
        dwUserID = UserID;
    }
}
class tagMatchScoreAddEvent : tagMsgEventBase
{
    public UInt32 dwUserID = 0;
    public tagMatchScoreAddEvent(UInt32 UserID)
        : base(MsgEventType.MET_Match_ScoreAdd)
    {
        dwUserID = UserID;
    }
}
class tagMatchContiueGoldEvent : tagMsgEventBase
{
    public tagMatchContiueGoldEvent()
        : base(MsgEventType.MET_Match_ContinueGold)
    {

    }
}
class tagMatchListChange :tagMsgEventBase
{
    public tagMatchListChange()
        : base(MsgEventType.MET_Match_ListChange)
    {

    }
}

class tagMonthSignUpChangeEvent : tagMsgEventBase
{
    public tagMonthSignUpChangeEvent()
        : base(MsgEventType.MET_Month_SignUpChange)
    {
    }
}
class tagMonthSignUpOnceChangeEvent : tagMsgEventBase
{
    public Byte MonthID;
    public tagMonthSignUpOnceChangeEvent(Byte MonthID)
        : base(MsgEventType.MET_Month_SignOnceUpChange)
    {
        this.MonthID = MonthID;
    }
}

class tagMonthSignUpSumChangeEvent : tagMsgEventBase
{
    public tagMonthSignUpSumChangeEvent()
        : base(MsgEventType.MET_Month_SignUpSumChange)
    {
    }
}
class tagMonthRankXmlChangeEvent : tagMsgEventBase
{
    private Byte MonthID;
    public tagMonthRankXmlChangeEvent(Byte bMonthID)
        : base(MsgEventType.MET_Month_RankXmlChange)
    {
        MonthID = bMonthID;
    }
}
class tagMonthEndEvent : tagMsgEventBase
{
    public Byte MonthID;
    public UInt32 MonthIndex;
    public UInt32 MonthScores;
    public UInt32 VipScores;
    public tagMonthEndEvent(Byte bMonthID, UInt32 MonthIndex, UInt32 MonthScores, UInt32 VipScores)
        : base(MsgEventType.MET_Month_End)
    {
        MonthID = bMonthID;
        this.MonthIndex = MonthIndex;
        this.MonthScores = MonthScores;
        this.VipScores = VipScores;
    }
}

//实体数据
class tagEntityChangeEvent : tagMsgEventBase
{
    public tagEntityChangeEvent()
        : base(MsgEventType.MET_Entity_Change)
    {
    }
}
class tagEntityBindPhoneEvent : tagMsgEventBase
{
    public bool Result;
    public bool IsFirst;
    public Byte EventType;
    public tagEntityBindPhoneEvent(bool Result, bool IsFirst, Byte EventType)
        : base(MsgEventType.MET_Entity_BindPhone)
    {
        this.Result = Result;
        this.IsFirst = IsFirst;
        this.EventType = EventType;
    }
}

//称号
class tagTitleChangeEvent : tagMsgEventBase
{
    public tagTitleChangeEvent()
        : base(MsgEventType.MET_Title_Change)
    {
    }
}
//排行榜
class tagRankChangeEvent : tagMsgEventBase
{
    public tagRankChangeEvent()
        : base(MsgEventType.MET_Rank_Change)
    {
    }
}
class tagRankGetRewardEvent : tagMsgEventBase
{
    public  Byte RankID;
    public tagRankGetRewardEvent(Byte Rank)
        : base(MsgEventType.MET_Rank_GetReward)
    {
        RankID = Rank;
    }
}
class tagRankXmlChangeEvent : tagMsgEventBase
{
    public tagRankXmlChangeEvent()
        : base(MsgEventType.MET_Rank_XmlChange)
    {
    }
}


class tagRankExistsRewardEvent : tagMsgEventBase
{
    public tagRankExistsRewardEvent()
        : base(MsgEventType.MET_Rank_ExistsReward)
    {
    }
}


//宝箱
public class tagChestActionEvent : tagMsgEventBase
{
    public Byte ChestSum;
    public tagChestInfo ActionChest;
    public bool IsReset;
    public tagChestActionEvent(tagChestInfo ChestObj, Byte ChestSum, bool IsReset)
        : base(MsgEventType.MET_Chest_Action)
    {
        this.ChestSum = ChestSum;
        ActionChest = ChestObj;
        this.IsReset = IsReset;
    }
}
class tagChestEndEvent : tagMsgEventBase
{
    public tagChestEndEvent()
        : base(MsgEventType.MET_Chest_End)
    {
    }
}
class tagChestRewardEvent : tagMsgEventBase
{
    public ChestOnceStates ChestOnceStates;
    public tagChestRewardEvent(ChestOnceStates ChestOnceStates)
        : base(MsgEventType.MET_Chest_Reward)
    {
        this.ChestOnceStates = ChestOnceStates;
    }
}

class tagChestResetEvent : tagMsgEventBase
{
    public tagChestResetEvent()
        : base(MsgEventType.MET_Chest_Reset)
    {
    }
}
//赠送
class tagGiffAddEvent : tagMsgEventBase
{
    public UInt32 GiffID;
    public tagGiffAddEvent(UInt32 ID)
        : base(MsgEventType.MET_Giff_Add)
    {
        GiffID = ID;
    }
}
class tagGiffDelEvent : tagMsgEventBase
{
    public UInt32 GiffID;
    public tagGiffDelEvent(UInt32 ID)
        : base(MsgEventType.MET_Giff_Del)
    {
        GiffID = ID;
    }
}
class tagGiffChangeEvent : tagMsgEventBase
{
    public tagGiffChangeEvent()
        : base(MsgEventType.MET_Giff_Change)
    {
    }
}
class tagGiffSendInfoChangeEvent : tagMsgEventBase
{
    public tagGiffSendInfoChangeEvent()
        : base(MsgEventType.MET_Giff_SendInfoChange)
    {
    }
}
class tagGiffSendInfoOnceChangeEvent : tagMsgEventBase
{
    public UInt32 UserID;
    public tagGiffSendInfoOnceChangeEvent(UInt32 pUserID)
        : base(MsgEventType.MET_Giff_SendInfoOnceChange)
    {
        UserID = pUserID;
    }
}

class tagGiffExistsChangeEvent : tagMsgEventBase
{
    public tagGiffExistsChangeEvent()
        : base(MsgEventType.MET_Giff_ExistsChange)
    {
    }
}


//商店
class tagShopResultEvent : tagMsgEventBase
{
    public bool Result;
    public tagShopResultEvent(bool bResult)
        : base(MsgEventType.MET_Shop_ShopResult)
    {
        Result = bResult;
    }
}
//在线奖励
class tagOnlineRewardChangeEvent : tagMsgEventBase
{
    public tagOnlineRewardChangeEvent()
        : base(MsgEventType.MET_OnlineReward_Change)
    {
    }
}
class tagOnlineRewardOnceChangeEvent : tagMsgEventBase
{
    public Byte RewardID;
    public tagOnlineRewardOnceChangeEvent(Byte ID)
        : base(MsgEventType.MET_OnlineReward_Once)
    {
        RewardID = ID;
    }
}

//gameData
class tagGameDataChangeEvent : tagMsgEventBase
{
    public tagGameDataChangeEvent()
        : base(MsgEventType.MET_GameData_Change)
    {
    }
}
//Charm 
class tagCharmRewardOnce : tagMsgEventBase
{
    public UInt32 SrcUserID;
    public UInt32 DestUserID;
    public Byte  CharmIndex;
    public UInt32 ItemSum;
    public tagCharmRewardOnce(UInt32 SrcUserID, UInt32 DestUserID, Byte CharmIndex, UInt32 ItemSum)
        : base(MsgEventType.MET_Charm_RewardOnce)
    {
        this.SrcUserID = SrcUserID;
        this.DestUserID = DestUserID;
        this.CharmIndex = CharmIndex;
        this.ItemSum = ItemSum;
    }
}
class tagUserChangeEvent : tagMsgEventBase
{
    public UInt32 DestUserID;
    public Int32 CharmValue;
    public tagUserChangeEvent(UInt32 DestUserID, Int32 CharmValue)
        : base(MsgEventType.MET_Charm_UserChange)
    {
        this.DestUserID = DestUserID;
        this.CharmValue = CharmValue;
    }
}

//Game
class tagGameRateChangeEvent : tagMsgEventBase
{
    public UInt32 UserID;
    public Byte OldRateIndex;
    public Byte NewRateIndex;
    public tagGameRateChangeEvent(UInt32 UserID, Byte OldRateIndex, Byte NewRateIndex)
        : base(MsgEventType.MET_Game_RateChange)
    {
        this.UserID = UserID;
        this.OldRateIndex = OldRateIndex;
        this.NewRateIndex = NewRateIndex;
    }
}
//Launcher
class tagLauncherDataChangeEvent : tagMsgEventBase
{
    public UInt32 LauncherData;
     public tagLauncherDataChangeEvent(UInt32 LauncherData)  
         : base(MsgEventType.MET_Launcher_DataChange)
     {
         this.LauncherData = LauncherData;
     }
}

//MET_Recharge_Result
class tagRechargeResultEvent : tagMsgEventBase
{
    public UInt32 ID;
    public bool Result;
     public tagRechargeResultEvent(UInt32 ID,bool Result)
        : base(MsgEventType.MET_Recharge_Result)
     {
         this.ID = ID;
         this.Result = Result;
     }
}

class tagRechargeGetOrderIDEvent : tagMsgEventBase
{
    public UInt32 OrderID;
    public UInt32 ShopID;
    public bool Result;
    public tagRechargeGetOrderIDEvent(UInt32 OrderID,UInt32 ShopID, bool Result)
        : base(MsgEventType.MET_Recharge_GetOrderID)
     {
         this.OrderID = OrderID;
         this.Result = Result;
         this.ShopID = ShopID;
     }
}

class tagAddRechargeOrderInfoEvent : tagMsgEventBase
{
    public bool Result;
    public UInt32 ShopID;
    public UInt32 OrderID;
    public string Transid;
    public string Sign;
    public tagAddRechargeOrderInfoEvent(bool Result, UInt32 ShopID, UInt32 OrderID, string Transid, string Sign)
        : base(MsgEventType.MET_Recharge_AddOrderInfo)
     {
         this.Result = Result;
         this.OrderID = OrderID;
         this.ShopID = ShopID;
         this.Transid = Transid;
         this.Sign = Sign;
     }
}


//MET_Announcement_Change
class tagAnnouncementChangeEvent : tagMsgEventBase
{
    public tagAnnouncementChangeEvent()
        : base(MsgEventType.MET_Announcement_Change)
    {
    }
}

class tagPackageResultEvent : tagMsgEventBase
{
    public bool Result;
    public UInt16[] RewardID;
    public tagPackageResultEvent(bool Result, UInt16[] RewardID)
        : base(MsgEventType.MET_Package_Result)
    {
        this.Result = Result;
        this.RewardID = RewardID;
    }
}

class tagJoinTableEvent : tagMsgEventBase
{
    public Byte TableID;
    public TableError Error;
    public tagJoinTableEvent(Byte TableID,TableError Error)
        : base(MsgEventType.MET_Table_Join)
    {
        this.TableID = TableID;
        this.Error = Error;
    }
}

class tagRoleStatesChangeEvent : tagMsgEventBase
{
    public UInt32 StatesData;
    public tagRoleStatesChangeEvent(UInt32 StatesData)
        : base(MsgEventType.MET_RoleStates_Change)
    {
        this.StatesData = StatesData;
    }
}

class tagReturnLogonEvent : tagMsgEventBase
{
    public tagReturnLogonEvent()
        : base(MsgEventType.MET_Logon_ReturnLogon)
    {
    }
}

class tagLogonAccountFreezeEvent : tagMsgEventBase
{
    public DateTime EndTime;
    public tagLogonAccountFreezeEvent(DateTime Time)
        : base(MsgEventType.MET_Logon_Freeze)
    {
        EndTime = Time;
    }
}


class tagQueueWriteEvent : tagMsgEventBase
{
    public UInt32 WriteIndex;
    public UInt32 WriteSec;
    public bool IsLogon;
    public tagQueueWriteEvent(UInt32 WriteIndex, UInt32 WriteSec, bool IsLogon)
        : base(MsgEventType.MET_Queue_Logon)
    {
        this.WriteIndex = WriteIndex;
        this.WriteSec = WriteSec;
        this.IsLogon = IsLogon;
    }
}


class tagUseExChangeEvent : tagMsgEventBase
{
    public UInt16 RewardID;
    public tagUseExChangeEvent(UInt16 RewardID)
        : base(MsgEventType.MET_Use_ExChange)
    {
        this.RewardID = RewardID;
    }
}

class tagNoticeOnceEvent : tagMsgEventBase
{
    public MessageType MessageType;
    public UInt32 MessageColor;
    public string Message;
    public Byte StepNum;
    public Byte StepSec;
    public tagNoticeOnceEvent(MessageType Type,UInt32 MessageColor, string Message, Byte StepNum, Byte StepSec)
        : base(MsgEventType.MET_Notice_Once)
    {
        this.MessageColor = MessageColor;
        this.Message = Message;
        this.StepNum = StepNum;
        this.StepSec = StepSec;
        this.MessageType = Type;
    }
}


class tagLotteryResultEvent : tagMsgEventBase
{
    public Byte LotteryID;
    public bool Result;
    public UInt16 RewardID;
    public tagLotteryResultEvent(Byte LotteryID, bool Result, UInt16 RewardID)
        : base(MsgEventType.MET_Lottery_Result)
    {
        this.LotteryID = LotteryID;
        this.Result = Result;
        this.RewardID = RewardID;
    }
}
class tagLotteryScoreChangeEvent : tagMsgEventBase
{
    public tagLotteryScoreChangeEvent()
        : base(MsgEventType.MET_Lottery_ScoreChange)
    {
    }
}

class tagTableLotteryUIChangeEvent : tagMsgEventBase
{
    public UInt32 UserID;
    public bool IsOpen;
    public tagTableLotteryUIChangeEvent(UInt32 UserID, bool IsOpen)
        : base(MsgEventType.MET_Table_LotteryUIStates)
    {
        this.UserID = UserID;
        this.IsOpen = IsOpen;
    }
}


class tagNiuNiuJoin : tagMsgEventBase
{
    public Byte TableUpdateSec;
    public NiuNiuTableStates TableStates;
    public TableUpdateInfo TableGlobelInfo;
    public TableBrandResult TableBrandresult;
    public BankerUserInfo TableBankerInfo;
    public WriteBankerList TableWriteBankerList;
    public VipSeatList TableVipSeatInfo;
    public NiuNiuGameLog TableGameLog;

    public tagNiuNiuJoin(Byte TableUpdateSec, NiuNiuTableStates TableStates, TableUpdateInfo TableGlobelInfo, TableBrandResult TableBrandresult, BankerUserInfo TableBankerInfo, WriteBankerList TableWriteBankerList, VipSeatList TableVipSeatInfo, NiuNiuGameLog TableGameLog)
        : base(MsgEventType.MET_NiuNiu_Join)
    {
        this.TableUpdateSec = TableUpdateSec;
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
        this.TableBrandresult = TableBrandresult;
        this.TableBankerInfo = TableBankerInfo;//庄家数据
        this.TableWriteBankerList = TableWriteBankerList;
        this.TableVipSeatInfo = TableVipSeatInfo;
        this.TableGameLog = TableGameLog;
    }
}
class tagNiuNiuBankerUserChange : tagMsgEventBase
{
     public BankerUserInfo TableBankerInfo;
     public tagNiuNiuBankerUserChange(BankerUserInfo TableBankerInfo)
         : base(MsgEventType.MET_NiuNiu_BankerUserChange)
     {
         this.TableBankerInfo = TableBankerInfo;
     }
}

class tagNiuNiuUpdateWriteBankerList : tagMsgEventBase
{
     public WriteBankerList TableWriteBankerList;
     public tagNiuNiuUpdateWriteBankerList(WriteBankerList TableWriteBankerList)
        : base(MsgEventType.MET_NiuNiu_UpdateWriteBankerList)
     {
         this.TableWriteBankerList = TableWriteBankerList;
     }
    
}
class tagNiuNiuAddGlobel : tagMsgEventBase
{
    public NiuNiuTableStates TableStates;
    public TableUpdateInfo TableGlobelInfo;
    public TableBrandResult TableBrandresult;
    public bool Result;
    public tagNiuNiuAddGlobel(NiuNiuTableStates TableStates, TableUpdateInfo TableGlobelInfo, TableBrandResult TableBrandresult, bool Result)
        : base(MsgEventType.MET_NiuNiu_AddGlobel)
    {
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
        this.TableBrandresult = TableBrandresult;
        this.Result = Result;
    }
}
class tagNiuNiuTableInfoChange : tagMsgEventBase
{
    public NiuNiuTableStates TableStates;
    public TableUpdateInfo TableGlobelInfo;
    public tagNiuNiuTableInfoChange(NiuNiuTableStates TableStates, TableUpdateInfo TableGlobelInfo)
        : base(MsgEventType.MET_NiuNiu_Update)
    {
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
    }
}

class tagNiuNiuStatesChange : tagMsgEventBase
{
    public NiuNiuTableStates TableStates;
    public TableUpdateInfo TableGlobelInfo;
    public TableBrandResult TableBrandresult;
    public VipSeatList OldVipSeatInfo;
    public VipSeatList TableVipSeatInfo;
    public WriteBankerList TableWriteBankerList;
    public BankerUserInfo TableBankerUserInfo;
    public NiuNiuGameLog TableGameLog;
    public tagNiuNiuStatesChange(NiuNiuTableStates TableStates, TableUpdateInfo TableGlobelInfo, TableBrandResult TableBrandresult, VipSeatList OldVipSeatInfo,VipSeatList TableVipSeatInfo, WriteBankerList TableWriteBankerList, BankerUserInfo TableBankerUserInfo, NiuNiuGameLog TableGameLog)
        : base(MsgEventType.MET_NiuNiu_StatesChange)
    {
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
        this.TableBrandresult = TableBrandresult;
        this.TableVipSeatInfo = TableVipSeatInfo;
        this.TableWriteBankerList = TableWriteBankerList;
        this.TableBankerUserInfo = TableBankerUserInfo;
        this.TableGameLog = TableGameLog;
        this.OldVipSeatInfo = OldVipSeatInfo;
    }
}


class tagNiuNiuWriteBankerGlobelMsg : tagMsgEventBase
{
    public tagNiuNiuWriteBankerGlobelMsg()
        : base(MsgEventType.MET_NiuNiu_WriteBankerGlobelMsg)
    {
    }
}

class tagNiuNiuVipSeatChangeEvent : tagMsgEventBase
{
    public bool Result;
    public VipSeatList TableVipList;
    public tagNiuNiuVipSeatChangeEvent(bool Result,VipSeatList TableVipList)
        : base(MsgEventType.MET_NiuNiu_VipSeatChange)
    {
        this.Result = Result;
        this.TableVipList = TableVipList;
    }
}
class tagNiuNiuBeLeaveVipSeatEvent : tagMsgEventBase
{
    public NiuNiuRoleInfo DestRoleInfo;
    public tagNiuNiuBeLeaveVipSeatEvent(NiuNiuRoleInfo DestRoleInfo)
        : base(MsgEventType.MET_NiuNiu_BeLeaveVip)
    {
        this.DestRoleInfo = DestRoleInfo;
    }
}


class tagNiuNiuGetNormalSeatInfoEvent : tagMsgEventBase
{
    public UInt32 Page;
    public UInt32 TotalRoleSum;
    public NiuNiuRoleInfo[] Array;
    public tagNiuNiuGetNormalSeatInfoEvent(UInt32 Page,UInt32 TotalRoleSum, NiuNiuRoleInfo[] Array)
        : base(MsgEventType.MET_NiuNiu_NormalSeatInfo)
    {
        this.Page = Page;
        this.Array = Array;
        this.TotalRoleSum = TotalRoleSum;
    }
}

class tagNiuNiuVipGetGlobelEvent : tagMsgEventBase
{
    public Byte VipSeat;
    public Byte Index;
    public UInt32 AddGlobel;
    public UInt32 AllGlobel;
    public tagNiuNiuVipGetGlobelEvent(Byte VipSeat, Byte Index, UInt32 AddGlobel, UInt32 AllGlobel)
        : base(MsgEventType.MET_NiuNiu_VipGetGlobel)
    {
        this.VipSeat = VipSeat;
        this.AddGlobel = AddGlobel;
        this.Index = Index;
        this.AllGlobel = AllGlobel;
    }
}
class tagNiuNiuTableStopUpdate : tagMsgEventBase
{
    public tagNiuNiuTableStopUpdate()
        : base(MsgEventType.MET_NiuNiu_TableStopUpdate)
    {
    }
}

class tagNiuNiuVipGlobelChange : tagMsgEventBase
{
    public UInt32 VipUserID;
    public Byte VipSeat;
    public UInt32 VipGlobelSum;
    public tagNiuNiuVipGlobelChange(UInt32 VipUserID, Byte VipSeat, UInt32 VipGlobelSum)
        : base(MsgEventType.MET_NiuNiu_VipGlobelChange)
    {
        this.VipUserID = VipUserID;
        this.VipSeat = VipSeat;
        this.VipGlobelSum = VipGlobelSum;
    }
}

class tagCharListChange : tagMsgEventBase
{
    public tagCharListChange()
        : base(MsgEventType.MET_Char_ListChange)
    {
    }
}
class tagCharContextChange : tagMsgEventBase
{
    public UInt32 SrcUserID;
    public tagCharContextChange(UInt32 SrcUserID)
        : base(MsgEventType.MET_Char_ContectChange)
    {
        this.SrcUserID = SrcUserID;
    }
}
class tagCharAddCharEvent : tagMsgEventBase
{
    public tagRoleCharInfo pInfo;
    public tagCharAddCharEvent(tagRoleCharInfo pInfo)
        : base(MsgEventType.MET_Char_AddChar)
    {
        this.pInfo = pInfo;
    }
}


class tagRelationRequestChangeEvent : tagMsgEventBase
{
    public tagRelationRequestChangeEvent()
        : base(MsgEventType.MET_RelationRequest_Change)
    {
    }
}
class tagRelationRequestSendResultEvent : tagMsgEventBase
{
    public tagRelationRequest Info;
    public bool Result;
    public tagRelationRequestSendResultEvent(tagRelationRequest Info,bool Result)
        : base(MsgEventType.MET_RelationRequest_SendResult)
    {
        this.Info = Info;
        this.Result = Result;
    }
}

class tagRelationRequestHandleResultEvent : tagMsgEventBase
{
     public tagRelationRequest Info;
    public bool Result;
    public tagRelationRequestHandleResultEvent(tagRelationRequest Info,bool Result)
        : base(MsgEventType.MET_RelationRequest_HandleResult)
    {
         this.Info = Info;
        this.Result = Result;
    }
}


//Dial

class tagDialJoin : tagMsgEventBase
{
    public Byte TableUpdateSec;
    public DialTableStates TableStates;
    public DialTableUpdateInfo TableGlobelInfo;
    public DialTableAreaData AreaData;
    public DialBankerUserInfo TableBankerInfo;
    public DialWriteBankerList TableWriteBankerList;
    public DialVipSeatList TableVipSeatInfo;
    public DialGameLog TableGameLog;
    public UInt16 TableResult;

    public tagDialJoin(Byte TableUpdateSec, DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo, DialTableAreaData AreaData, DialBankerUserInfo TableBankerInfo,
        DialWriteBankerList TableWriteBankerList, DialVipSeatList TableVipSeatInfo, DialGameLog TableGameLog, UInt16 TableResult)
        : base(MsgEventType.MET_Dial_Join)
    {
        this.TableUpdateSec = TableUpdateSec;
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
        this.AreaData = AreaData;
        this.TableBankerInfo = TableBankerInfo;//庄家数据
        this.TableWriteBankerList = TableWriteBankerList;
        this.TableVipSeatInfo = TableVipSeatInfo;
        this.TableGameLog = TableGameLog;
        this.TableResult = TableResult;
    }
}
class tagDialAddGlobel : tagMsgEventBase
{
    public DialTableStates TableStates;
    public DialTableUpdateInfo TableGlobelInfo;
    public DialTableAreaData AreaData;
    public UInt32[] TableGlobelSum;
    public bool Result;
    public tagDialAddGlobel(DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo, DialTableAreaData AreaData, bool Result, UInt32[] TableGlobelSum)
        : base(MsgEventType.MET_Dial_AddGlobel)
    {
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
        this.AreaData = AreaData;
        this.Result = Result;
        this.TableGlobelSum = TableGlobelSum;
    }
}
class tagDialUpdateWriteBankerList : tagMsgEventBase
{
    public DialWriteBankerList TableWriteBankerList;
    public tagDialUpdateWriteBankerList(DialWriteBankerList TableWriteBankerList)
        : base(MsgEventType.MET_Dial_UpdateWriteBankerList)
    {
        this.TableWriteBankerList = TableWriteBankerList;
    }

}
class tagDialBankerUserChange : tagMsgEventBase
{
    public DialBankerUserInfo TableBankerInfo;
    public tagDialBankerUserChange(DialBankerUserInfo TableBankerInfo)
        : base(MsgEventType.MET_Dial_BankerUserChange)
    {
        this.TableBankerInfo = TableBankerInfo;
    }
}
class tagDialStatesChange : tagMsgEventBase
{
    public DialTableStates TableStates;
    public DialTableUpdateInfo TableGlobelInfo;
    public DialTableAreaData AreaData;
    public DialVipSeatList OldVipSeatInfo;
    public DialVipSeatList TableVipSeatInfo;
    public DialWriteBankerList TableWriteBankerList;
    public DialBankerUserInfo TableBankerUserInfo;
    public DialGameLog TableGameLog;
    public UInt16 TableResult;
    public tagDialStatesChange(DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo, DialTableAreaData AreaData,
        DialVipSeatList OldVipSeatInfo, DialVipSeatList TableVipSeatInfo, DialWriteBankerList TableWriteBankerList, DialBankerUserInfo TableBankerUserInfo, DialGameLog TableGameLog, UInt16 TableResult)
        : base(MsgEventType.MET_Dial_StatesChange)
    {
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
        this.AreaData = AreaData;
        this.TableVipSeatInfo = TableVipSeatInfo;
        this.TableWriteBankerList = TableWriteBankerList;
        this.TableBankerUserInfo = TableBankerUserInfo;
        this.TableGameLog = TableGameLog;
        this.OldVipSeatInfo = OldVipSeatInfo;
        this.TableResult = TableResult;
    }
}


class tagDialTableInfoChange : tagMsgEventBase
{
    public DialTableStates TableStates;
    public DialTableUpdateInfo TableGlobelInfo;
    public tagDialTableInfoChange(DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo)
        : base(MsgEventType.MET_Dial_Update)
    {
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
    }
}
class tagDialWriteBankerGlobelMsg : tagMsgEventBase
{
    public tagDialWriteBankerGlobelMsg()
        : base(MsgEventType.MET_Dial_WriteBankerGlobelMsg)
    {
    }
}

class tagDialVipSeatChangeEvent : tagMsgEventBase
{
    public bool Result;
    public DialVipSeatList TableVipList;
    public tagDialVipSeatChangeEvent(bool Result, DialVipSeatList TableVipList)
        : base(MsgEventType.MET_Dial_VipSeatChange)
    {
        this.Result = Result;
        this.TableVipList = TableVipList;
    }
}
class tagDialBeLeaveVipSeatEvent : tagMsgEventBase
{
    public DialRoleInfo DestRoleInfo;
    public tagDialBeLeaveVipSeatEvent(DialRoleInfo DestRoleInfo)
        : base(MsgEventType.MET_Dial_BeLeaveVip)
    {
        this.DestRoleInfo = DestRoleInfo;
    }
}
class tagDialGetNormalSeatInfoEvent : tagMsgEventBase
{
    public UInt32 Page;
    public UInt32 TotalRoleSum;
    public DialRoleInfo[] Array;
    public tagDialGetNormalSeatInfoEvent(UInt32 Page, UInt32 TotalRoleSum, DialRoleInfo[] Array)
        : base(MsgEventType.MET_Dial_NormalSeatInfo)
    {
        this.Page = Page;
        this.Array = Array;
        this.TotalRoleSum = TotalRoleSum;
    }
}

class tagDialVipGetGlobelEvent : tagMsgEventBase
{
    public Byte VipSeat;
    public Byte Index;
    public UInt32 AddGlobel;
    public UInt32 AllGlobel;
    public tagDialVipGetGlobelEvent(Byte VipSeat, Byte Index, UInt32 AddGlobel, UInt32 AllGlobel)
        : base(MsgEventType.MET_Dial_VipGetGlobel)
    {
        this.VipSeat = VipSeat;
        this.AddGlobel = AddGlobel;
        this.Index = Index;
        this.AllGlobel = AllGlobel;
    }
}
class tagDialTableStopUpdate : tagMsgEventBase
{
    public tagDialTableStopUpdate()
        : base(MsgEventType.MET_Dial_TableStopUpdate)
    {
    }
}

class tagDialVipGlobelChange : tagMsgEventBase
{
    public UInt32 VipUserID;
    public Byte VipSeat;
    public UInt32 VipGlobelSum;
    public tagDialVipGlobelChange(UInt32 VipUserID, Byte VipSeat, UInt32 VipGlobelSum)
        : base(MsgEventType.MET_Dial_VipGlobelChange)
    {
        this.VipUserID = VipUserID;
        this.VipSeat = VipSeat;
        this.VipGlobelSum = VipGlobelSum;
    }
}


//Car

class tagCarJoin : tagMsgEventBase
{
    public Byte TableUpdateSec;
    public CarTableStates TableStates;
    public CarTableUpdateInfo TableGlobelInfo;
    public CarBankerUserInfo TableBankerInfo;
    public CarWriteBankerList TableWriteBankerList;
    public CarVipSeatList TableVipSeatInfo;
    public CarGameLog TableGameLog;
    public Byte TableResult;

    public tagCarJoin(Byte TableUpdateSec, CarTableStates TableStates, CarTableUpdateInfo TableGlobelInfo,  CarBankerUserInfo TableBankerInfo,
        CarWriteBankerList TableWriteBankerList, CarVipSeatList TableVipSeatInfo, CarGameLog TableGameLog, Byte TableResult)
        : base(MsgEventType.MET_Car_Join)
    {
        this.TableUpdateSec = TableUpdateSec;
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
        this.TableBankerInfo = TableBankerInfo;//庄家数据
        this.TableWriteBankerList = TableWriteBankerList;
        this.TableVipSeatInfo = TableVipSeatInfo;
        this.TableGameLog = TableGameLog;
        this.TableResult = TableResult;
    }
}
class tagCarAddGlobel : tagMsgEventBase
{
    public CarTableStates TableStates;
    public CarTableUpdateInfo TableGlobelInfo;
    public bool Result;
    public tagCarAddGlobel(CarTableStates TableStates, CarTableUpdateInfo TableGlobelInfo, bool Result)
        : base(MsgEventType.MET_Car_AddGlobel)
    {
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
        this.Result = Result;
    }
}
class tagCarUpdateWriteBankerList : tagMsgEventBase
{
    public CarWriteBankerList TableWriteBankerList;
    public tagCarUpdateWriteBankerList(CarWriteBankerList TableWriteBankerList)
        : base(MsgEventType.MET_Car_UpdateWriteBankerList)
    {
        this.TableWriteBankerList = TableWriteBankerList;
    }

}
class tagCarBankerUserChange : tagMsgEventBase
{
    public CarBankerUserInfo TableBankerInfo;
    public tagCarBankerUserChange(CarBankerUserInfo TableBankerInfo)
        : base(MsgEventType.MET_Car_BankerUserChange)
    {
        this.TableBankerInfo = TableBankerInfo;
    }
}
class tagCarStatesChange : tagMsgEventBase
{
    public CarTableStates TableStates;
    public CarTableUpdateInfo TableGlobelInfo;
    public CarVipSeatList OldVipSeatInfo;
    public CarVipSeatList TableVipSeatInfo;
    public CarWriteBankerList TableWriteBankerList;
    public CarBankerUserInfo TableBankerUserInfo;
    public CarGameLog TableGameLog;
    public Byte TableResult;
    public tagCarStatesChange(CarTableStates TableStates, CarTableUpdateInfo TableGlobelInfo, 
        CarVipSeatList OldVipSeatInfo, CarVipSeatList TableVipSeatInfo, CarWriteBankerList TableWriteBankerList, CarBankerUserInfo TableBankerUserInfo, CarGameLog TableGameLog, Byte TableResult)
        : base(MsgEventType.MET_Car_StatesChange)
    {
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
        this.TableVipSeatInfo = TableVipSeatInfo;
        this.TableWriteBankerList = TableWriteBankerList;
        this.TableBankerUserInfo = TableBankerUserInfo;
        this.TableGameLog = TableGameLog;
        this.OldVipSeatInfo = OldVipSeatInfo;
        this.TableResult = TableResult;
    }
}


class tagCarTableInfoChange : tagMsgEventBase
{
    public CarTableStates TableStates;
    public CarTableUpdateInfo TableGlobelInfo;
    public tagCarTableInfoChange(CarTableStates TableStates, CarTableUpdateInfo TableGlobelInfo)
        : base(MsgEventType.MET_Car_Update)
    {
        this.TableStates = TableStates;
        this.TableGlobelInfo = TableGlobelInfo;
    }
}
class tagCarWriteBankerGlobelMsg : tagMsgEventBase
{
    public tagCarWriteBankerGlobelMsg()
        : base(MsgEventType.MET_Car_WriteBankerGlobelMsg)
    {
    }
}

class tagCarVipSeatChangeEvent : tagMsgEventBase
{
    public bool Result;
    public CarVipSeatList TableVipList;
    public tagCarVipSeatChangeEvent(bool Result, CarVipSeatList TableVipList)
        : base(MsgEventType.MET_Car_VipSeatChange)
    {
        this.Result = Result;
        this.TableVipList = TableVipList;
    }
}
class tagCarBeLeaveVipSeatEvent : tagMsgEventBase
{
    public CarRoleInfo DestRoleInfo;
    public tagCarBeLeaveVipSeatEvent(CarRoleInfo DestRoleInfo)
        : base(MsgEventType.MET_Car_BeLeaveVip)
    {
        this.DestRoleInfo = DestRoleInfo;
    }
}
class tagCarGetNormalSeatInfoEvent : tagMsgEventBase
{
    public UInt32 Page;
    public UInt32 TotalRoleSum;
    public CarRoleInfo[] Array;
    public tagCarGetNormalSeatInfoEvent(UInt32 Page, UInt32 TotalRoleSum, CarRoleInfo[] Array)
        : base(MsgEventType.MET_Car_NormalSeatInfo)
    {
        this.Page = Page;
        this.Array = Array;
        this.TotalRoleSum = TotalRoleSum;
    }
}

class tagCarVipGetGlobelEvent : tagMsgEventBase
{
    public Byte VipSeat;
    public Byte Index;
    public UInt32 AddGlobel;
    public UInt32 AllGlobel;
    public tagCarVipGetGlobelEvent(Byte VipSeat, Byte Index, UInt32 AddGlobel, UInt32 AllGlobel)
        : base(MsgEventType.MET_Car_VipGetGlobel)
    {
        this.VipSeat = VipSeat;
        this.AddGlobel = AddGlobel;
        this.Index = Index;
        this.AllGlobel = AllGlobel;
    }
}
class tagCarTableStopUpdate : tagMsgEventBase
{
    public tagCarTableStopUpdate()
        : base(MsgEventType.MET_Car_TableStopUpdate)
    {
    }
}

class tagCarVipGlobelChange : tagMsgEventBase
{
    public UInt32 VipUserID;
    public Byte VipSeat;
    public UInt32 VipGlobelSum;
    public tagCarVipGlobelChange(UInt32 VipUserID, Byte VipSeat, UInt32 VipGlobelSum)
        : base(MsgEventType.MET_Car_VipGlobelChange)
    {
        this.VipUserID = VipUserID;
        this.VipSeat = VipSeat;
        this.VipGlobelSum = VipGlobelSum;
    }
}