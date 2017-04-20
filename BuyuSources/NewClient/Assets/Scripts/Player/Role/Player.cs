using UnityEngine;
using System.Collections.Generic;
using System;


//public class Player
//{
//    PlayerExtraData  m_PlayerData;
//    public PlayerExtraData PlayerData
//    {
//        get { return m_PlayerData; }
//        set { m_PlayerData = value; }
//    }

//}
//用户的基本类
public class RoleBase //玩家的基类
{
    public virtual UInt32 GetUserID()
    {
        return 0;
    }
    public virtual string GetNickName()
    { 
        return "";
    }
    public virtual UInt16 GetLevel()
    {
        return 0;
    }
    public virtual UInt32 GetExp()
    {
        return 0;
    }
    public virtual UInt32 GetFaceID()
    {
        return 0;
    }
    public virtual bool GetGender()
    {
        return false;
    }
    public virtual UInt32 GetGlobel()
    {
        return 0;
    }
    public virtual UInt32 GetMedal()
    {
        return 0;
    }
    public virtual UInt32 GetCurrency()
    {
        return 0;
    }
    public virtual UInt32 GetAchievementPoint()
    {
        return 0;
    }
    public virtual Byte GetTitleID()
    {
        return 0;
    }
    public virtual UInt32[] GetCharmInfo()
    {
        return null;
    }
    public virtual UInt32 GetProduction()
    {
        return 0;
    }
    public virtual UInt32 GetGameTime()
    {
        return 0;
    }
    public virtual Byte GetSendGiffSum()
    {
        return 0;
    }
    public virtual Byte GetAcceptGiffSum()
    {
        return 0;
    }
    public virtual Byte GetMonthID()
    {
        return 0;
    }
    public virtual UInt32 GetMonthIndex()
    {
        return 0;
    }
    public virtual UInt32 GetMonthGlobel()
    {
        return 0;
    }
    public virtual UInt32 GetMonthScore()
    {
        return 0;
    }
    public virtual Byte GetMonthAddGlobelNum()
    {
        return 0;
    }
    public virtual Byte GetLeaveOnlineDay()
    {
        return 0;
    }
    public virtual Byte GetRelationType()
    {
        return 0;
    }
    public virtual bool IsOnline()
    {
        return false;
    }
    public virtual Byte GetSeat()
    {
        return 0;
    }
    public virtual UInt32 GetMonthUpperSocre()
    {
        return 0;
    }
    public virtual void SetUserID(UInt32 dwUserID)
    {

    }
    public virtual void SetNickName(string NickName)
    {

    }
    public virtual void SetLevel(UInt16 wLevel)
    {

    }
    public virtual void SetExp(UInt32 dwExp)
    {

    }
    public virtual void SetFaceID(UInt32 FaceID)
    {

    }
    public virtual void SetGender(bool bGender)
    {

    }
    public virtual void SetGlobel(UInt32 dwGlobel)
    {

    }
    public virtual void SetMedal(UInt32 dwMedal)
    {

    }
    public virtual void SetCurrency(UInt32 Currency)
    {

    }
    public virtual void SetAchievementPoint(UInt32 dwAchievementPoint)
    {

    }
    public virtual void SetTitleID(Byte bTitleID)
    {

    }
    public virtual void SetCharmInfo(UInt32[] CharmArray)
    {

    }
  
    public virtual void SetProduction(UInt32 dwProduction)
    {

    }
    public virtual void SetGameTime(UInt32 GameTime)
    {

    }
    public virtual void SetSendGiffSum(Byte SendSum)
    {

    }
    public virtual void SetAcceptGiffSum(Byte AccpetSum)
    {

    }
    public virtual void SetMonthID(Byte MonthID)
    {

    }
    public virtual void SetMonthIndex(UInt32 MonthIndex)
    {

    }
    public virtual void SetMonthGlobel(UInt32 MonthGlobel)
    {

    }
    public virtual void SetMonthScore(UInt32 MonthScore)
    {

    }
    public virtual void SetMonthAddGlobelNum(Byte MonthAddGlobelNum)
    {

    }
    public virtual void SetLeaveOnlineDay(Byte DiffDay)
    {
    }
    public  virtual void SetRelationType(Byte Type)
    {

    }
    public virtual void SetIsOnline(bool IsOnline)
    {

    }
    public virtual void SetMonthUpperSocre(UInt32 UpperSocre)
    {

    }
    public virtual void SetSeat(Byte SeatID)
    {

    }
    //public virtual UInt32 GetClientIP()
    //{
    //    return 0;
    //}
    //public virtual void SetClientIP(UInt32 ClientIP)
    //{

    //}

    public virtual string GetIPAddress()
    {
        return "";
    }
    public virtual void SetIPAddress(string Add)
    {

    }


    public virtual Byte GetVipLevel()
    {
        return 0;
    }
    public virtual void SetVipLevel(Byte VipLevel)
    {

    }
    public virtual bool GetIsInMonthCard()
    {
        return false;
    }
    public virtual void SetIsInMonthCard(bool IsInMonthCard)
    {

    }

    public virtual UInt32 GetGameID()
    {
        return 0;
    }

}
class RoleMe : RoleBase //本人 自己的数据类
{
    private tagRoleInfo RoleInfo = null;//玩家的基本数据
    private tagRoleMonthInfo RoleMonthInfo = null;//玩家的比赛数据
    private UInt32 MonthUpperSocre = 0;
    private Byte Seat = 0xff;
    private Byte TableTypeID = 0;

    //提供大量的具体的功能的操作对象 进行核心的功能处理
    public RoleMe(tagRoleInfo pRoleInfo)
    {
        RoleInfo = pRoleInfo;
        RoleMonthInfo = null;
    }
    public RoleMe()
    {
        RoleInfo = null;
        RoleMonthInfo = null;
    }

    public virtual Byte GetTableTypeID()
    {
        return TableTypeID;
    }
    public virtual void SetTableTypeID(Byte TableTypeID)
    {
        this.TableTypeID = TableTypeID;
    }


    public virtual void SetRoleInfo(tagRoleInfo pRoleInfo)
    {
        RoleInfo = pRoleInfo;
    }
    public virtual void SetMonthInfo(tagRoleMonthInfo pMonthInfo)
    {
        RoleMonthInfo = pMonthInfo;
    }
    public override UInt32 GetUserID()
    {
        if (RoleInfo == null)
            return base.GetUserID();
        return RoleInfo.dwUserID;
    }
    public override string GetNickName()
    {
        if (RoleInfo == null)
            return base.GetNickName();
        return RoleInfo.NickName;
    }
    public override UInt16 GetLevel()
    {
        if (RoleInfo == null)
            return base.GetLevel();
        return RoleInfo.wLevel;
    }
    public override UInt32 GetExp()
    {
        if (RoleInfo == null)
            return base.GetExp();
        return RoleInfo.dwExp;
    }
    public override UInt32 GetFaceID()
    {
        if (RoleInfo == null)
            return base.GetFaceID();
        return RoleInfo.dwFaceID;
    }
    public override bool GetGender()
    {
        if (RoleInfo == null)
            return base.GetGender();
        return RoleInfo.bGender;
    }
    public override UInt32 GetGlobel()
    {
        if (RoleInfo == null)
            return base.GetGlobel();
        return RoleInfo.dwGlobeNum;
    }
    public override UInt32 GetMedal()
    {
        if (RoleInfo == null)
            return base.GetMedal();
        return RoleInfo.dwMedalNum;
    }
    public override UInt32 GetCurrency()
    {
        if (RoleInfo == null)
            return base.GetCurrency();
        return RoleInfo.dwCurrencyNum;
    }
    public override UInt32 GetAchievementPoint()
    {
        if (RoleInfo == null)
            return base.GetAchievementPoint();
        return RoleInfo.dwAchievementPoint;
    }
    public override Byte GetTitleID()
    {
        if (RoleInfo == null)
            return base.GetTitleID();
        return RoleInfo.TitleID;
    }
    public override UInt32[] GetCharmInfo()
    {
        if (RoleInfo == null)
            return base.GetCharmInfo();
        return RoleInfo.CharmArray;
    }
    public override UInt32 GetProduction()
    {
        if (RoleInfo == null)
            return base.GetProduction();
        return RoleInfo.dwProduction;
    }
    public override UInt32 GetGameTime()
    {
        if (RoleInfo == null)
            return base.GetGameTime();
        return 0;
    }
    public override Byte GetSendGiffSum()
    {
        if (RoleInfo == null)
            return base.GetSendGiffSum();
        return RoleInfo.SendGiffSum;
    }
    public override Byte GetAcceptGiffSum()
    {
        if (RoleInfo == null)
            return base.GetAcceptGiffSum();
        return RoleInfo.AcceptGiffSum;
    }
    public virtual int256 GetTaskStates()
    {
        if (RoleInfo == null)
            return null;
        else
            return RoleInfo.TaskStates;
    }
    public virtual int256 GetAchievementStates()
    {
        if (RoleInfo == null)
            return null;
        else
            return RoleInfo.AchievementStates;
    }
    public virtual int256 GetActionStates()
    {
        if (RoleInfo == null)
            return null;
        else
            return RoleInfo.ActionStates;
    }
    public virtual UInt32 GetCheckData()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.CheckData;
    }
    public virtual UInt16 GetOnlineMin()
    {
        if (RoleInfo == null)
            return 0;
        else
        {
            return Convert.ToUInt16(SystemTime.Instance.GetOnlineSecByDay() / 60 + RoleInfo.OnlineMin);//获取玩家当天在线的分钟数
        }
    }
    public virtual UInt32 GetOnlineSec()
    {
        if (RoleInfo == null)
            return 0;
        else
        {
            return Convert.ToUInt32(SystemTime.Instance.GetOnlineSecByDay() + RoleInfo.OnlineMin*60);
        }
    }
    public virtual UInt32 GetOnlineRewardStates()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.OnlineRewardStates;
    }
    public override Byte GetMonthID()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthID();
        return RoleMonthInfo.bMonthID;
    }
    public override UInt32 GetMonthIndex()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthIndex();
        return RoleMonthInfo.dwIndex;
    }
    public override UInt32 GetMonthGlobel()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthGlobel();
        return RoleMonthInfo.dwMonthGlobel;
    }
    public override UInt32 GetMonthScore()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthScore();
        return RoleMonthInfo.dwMonthScore;
    }
    public override Byte GetMonthAddGlobelNum()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthAddGlobelNum();
        return RoleMonthInfo.wUserAddGlobelNum;
    }
    public override UInt32 GetMonthUpperSocre()
    {
        if (RoleMonthInfo == null || RoleMonthInfo.bMonthID == 0)
            return base.GetMonthUpperSocre();
        return MonthUpperSocre;
    }
    public override Byte GetSeat()
    {
        return Seat;
    }
    public virtual bool GetIsCanResetAccount()
    {
        return RoleInfo.IsCanResetAccount;
    }
    public override void SetUserID(UInt32 dwUserID)
    {
        if (RoleInfo == null)
            base.SetUserID(dwUserID);
        else
            RoleInfo.dwUserID = dwUserID;
    }
    public override void SetNickName(string NickName)
    {
        if (RoleInfo == null)
            base.SetNickName(NickName);
        else
            RoleInfo.NickName = NickName;
    }
    public override void SetLevel(UInt16 wLevel)
    {
        if (RoleInfo == null)
            base.SetLevel(wLevel);
        else
        {
            if(wLevel > RoleInfo.wLevel)
            {
                PlayerRole.Instance.HandleEvent(EventTargetType.ET_UpperLevel, 0, Convert.ToUInt32(wLevel - RoleInfo.wLevel));
            }
            RoleInfo.wLevel = wLevel;

            PlayerRole.Instance.HandleEvent(EventTargetType.ET_ToLevel, 0,RoleInfo.wLevel);
        }
    }
    public override void SetExp(UInt32 dwExp)
    {
        if (RoleInfo == null)
            base.SetExp(dwExp);
        else
            RoleInfo.dwExp = dwExp;
    }
    public override void SetFaceID(UInt32 FaceID)
    {
        if (RoleInfo == null)
            base.SetFaceID(FaceID);
        else
            RoleInfo.dwFaceID = FaceID;
    }
    public override void SetGender(bool bGender)
    {
        if (RoleInfo == null)
            base.SetGender(bGender);
        else
            RoleInfo.bGender = bGender;
    }
    public override void SetGlobel(UInt32 dwGlobel)
    {
        if (RoleInfo == null)
            base.SetGlobel(dwGlobel);
        else
        {
            if(dwGlobel > RoleInfo.dwGlobeNum)
            {
                PlayerRole.Instance.HandleEvent(EventTargetType.ET_GetGlobel, 0, dwGlobel - RoleInfo.dwGlobeNum); //触发玩家获得金币的事件

                PlayerRole.Instance.RoleGameData.OnHandleRoleGetGlobel(dwGlobel - RoleInfo.dwGlobeNum);
            }
            RoleInfo.dwGlobeNum = dwGlobel;

            PlayerRole.Instance.HandleEvent(EventTargetType.ET_MaxGlobelSum, 0,RoleInfo.dwGlobeNum);
        } 
    }
    public override void SetMedal(UInt32 dwMedal)
    {
        if (RoleInfo == null)
            base.SetMedal(dwMedal);
        else
        {
            if (dwMedal > RoleInfo.dwMedalNum)
            {
                PlayerRole.Instance.HandleEvent(EventTargetType.ET_GetMadel, 0, dwMedal - RoleInfo.dwMedalNum); //触发玩家获得金币的事件
            }
            RoleInfo.dwMedalNum = dwMedal;
        }
    }
    public override void SetCurrency(UInt32 Currency)
    {
        if (RoleInfo == null)
            base.SetCurrency(Currency);
        else
        {
            if (Currency > RoleInfo.dwCurrencyNum)
            {
                PlayerRole.Instance.HandleEvent(EventTargetType.ET_GetCurren, 0, Currency - RoleInfo.dwCurrencyNum); //触发玩家获得金币的事件
            }
            RoleInfo.dwCurrencyNum = Currency;

            PlayerRole.Instance.HandleEvent(EventTargetType.ET_MaxCurren, 0,RoleInfo.dwCurrencyNum);
        }
    }
    public override void SetAchievementPoint(UInt32 dwAchievementPoint)
    {
        if (RoleInfo == null)
            base.SetAchievementPoint(dwAchievementPoint);
        else
            RoleInfo.dwAchievementPoint = dwAchievementPoint;
    }
    public override void SetTitleID(Byte bTitleID)
    {
        if (RoleInfo == null)
            base.SetTitleID(bTitleID);
        else
            RoleInfo.TitleID = bTitleID;
    }
    public override void SetCharmInfo(UInt32[] CharmArray)
    {
        if (RoleInfo == null)
            base.SetCharmInfo(CharmArray);
        else
            RoleInfo.CharmArray = CharmArray;
    }
    public override void SetProduction(UInt32 dwProduction)
    {
        if (RoleInfo == null)
            base.SetProduction(dwProduction);
        else
            RoleInfo.dwProduction = dwProduction;
    }
    public override void SetGameTime(UInt32 GameTime)
    {
        if (RoleInfo == null)
            base.SetGameTime(GameTime);
        else
            RoleInfo.dwGameTime = GameTime;
    }
    public override void SetSendGiffSum(Byte SendSum)
    {
        if (RoleInfo == null)
            base.SetSendGiffSum(SendSum);
        else
            RoleInfo.SendGiffSum = SendSum;
    }
    public override void SetAcceptGiffSum(Byte AccpetSum)
    {
        if (RoleInfo == null)
            base.SetAcceptGiffSum(AccpetSum);
        else
            RoleInfo.AcceptGiffSum = AccpetSum;
    }
    public override void SetMonthID(Byte MonthID)
    {
        if (RoleMonthInfo == null)
            base.SetMonthID(MonthID);
        else
            RoleMonthInfo.bMonthID = MonthID;
    }
    public override void SetMonthIndex(UInt32 MonthIndex)
    {
        if (RoleMonthInfo == null)
            base.SetMonthIndex(MonthIndex);
        else
            RoleMonthInfo.dwIndex = MonthIndex;
    }
    public override void SetMonthGlobel(UInt32 MonthGlobel)
    {
        if (RoleMonthInfo == null)
            base.SetMonthGlobel(MonthGlobel);
        else
            RoleMonthInfo.dwMonthGlobel = MonthGlobel;
    }
    public override void SetMonthScore(UInt32 MonthScore)
    {
        if (RoleMonthInfo == null)
            base.SetMonthScore(MonthScore);
        else
            RoleMonthInfo.dwMonthScore = MonthScore;
    }
    public override void SetMonthAddGlobelNum(Byte MonthAddGlobelNum)
    {
        if (RoleMonthInfo == null)
            base.SetMonthAddGlobelNum(MonthAddGlobelNum);
        else
            RoleMonthInfo.wUserAddGlobelNum = MonthAddGlobelNum;
    }
    public override bool IsOnline() //玩家自己肯定在线
    {
        return true;
    }
    public override void SetMonthUpperSocre(UInt32 UpperSocre)
    {
        MonthUpperSocre = UpperSocre;
    }
    public override void SetSeat(Byte SeatID)
    {
        Seat = SeatID;
    }

    public virtual void SetTaskStates(int256 States)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.TaskStates = States;
    }
    public virtual void SetAchievementStates(int256 States)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.AchievementStates = States;
    }
    public virtual void SetActionStates(int256 States)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.ActionStates = States;
    }

    public virtual void SetOnlineMin(UInt16 OnlineMin)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.OnlineMin = OnlineMin;
    }
    public virtual void  SetOnlineRewardStates(UInt32 States)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.OnlineRewardStates = States;
    }
    public virtual void SetAccountIsCanReset(bool States)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.IsCanResetAccount = States;
    }

    public virtual UInt32 GetAchievementIndex()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.AchievementPointIndex;
    }
    public virtual void SetAchievementIndex(UInt32 AchievementIndex)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.AchievementPointIndex = AchievementIndex;
    }

    public virtual UInt32 GetClientIP()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.ClientIP;
    }
    public virtual void SetClientIP(UInt32 ClientIP)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.ClientIP = ClientIP;
    }

    public override string GetIPAddress()
    {
        if (RoleInfo == null)
            return "";
        else
            return RoleInfo.IPAddress;
    }
    public override void SetIPAddress(string Add)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.IPAddress = Add;
    }

    public virtual void SetCheckData(UInt32 CheckData)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.CheckData = CheckData;
    }

    public virtual bool IsShowIpAddress()
    {
        if (RoleInfo == null)
            return true;
        else
            return RoleInfo.IsShowIPAddress;
    }
    public virtual void SetIsShowIpAddress(bool IsShowAddress)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.IsShowIPAddress = IsShowAddress;
    }

    public virtual UInt32 GetExChangeStates()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.ExChangeStates;
    }
    public virtual void SetExChangeStates(UInt32 States)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.ExChangeStates = States;
    }

    public virtual UInt32 GetTotalRechargeSum()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.TotalRechargeSum;
    }
    public virtual void SetTotalRechargeSum(UInt32 TotalSum)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.TotalRechargeSum = TotalSum;
    }

    public virtual bool GetIsFirstPayGlobel()
    {
        if (RoleInfo == null)
            return true;
        else
            return RoleInfo.bIsFirstPayGlobel;
    }
    public virtual void SetIsFirstPayGlobel(bool States)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.bIsFirstPayGlobel = States;
    }

    public virtual bool GetIsFirstPayCurrcey()
    {
        if (RoleInfo == null)
            return true;
        else
            return RoleInfo.bIsFirstPayCurrcey;
    }
    public virtual void SetIsFirstPayCurrcey(bool States)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.bIsFirstPayCurrcey = States;
    }


    public virtual UInt32 GetLotteryScore()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.LotteryScore;
    }
    public virtual void SetLotteryScore(UInt32 nValue)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.LotteryScore = nValue;
    }

    public virtual Byte GetLotteryFishSum()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.LotteryFishSum;
    }
    public virtual void SetLotteryFishSum(Byte nValue)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.LotteryFishSum = nValue;
    }

    

    public override Byte GetVipLevel()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.VipLevel;
    }
    public override void SetVipLevel(Byte VipLevel)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.VipLevel = VipLevel;
    }
    public virtual Byte GetMonthCardID()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.MonthCardID;
    }
    public virtual void SetMonthCardID(Byte MonthCardID)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.MonthCardID = MonthCardID;
    }



    public virtual DateTime GetMonthCardEndTime()
    {
        if (RoleInfo == null)
            return TimeZone.CurrentTimeZone.ToLocalTime(new DateTime(1970, 1, 1));
        else
            return SystemTime.Instance.GetDateTimeByTotalSec(RoleInfo.MonthCardEndTime);//获取VIP的结束时间
    }
    public virtual void SetMonthCardEndTime(Int64 EndTime)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.MonthCardEndTime = EndTime;
    }

    public virtual int256 GetRateValue()
    {
        if (RoleInfo == null)
            return null;
        else
            return RoleInfo.RateValue;
    }
    public virtual void SetRateValue(int256 RateValue)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.RateValue = RateValue;
    }

    public virtual DateTime GetMonthCardRewardTime()
    {
        if (RoleInfo == null)
            return TimeZone.CurrentTimeZone.ToLocalTime(new DateTime(1970, 1, 1));
        else
            return SystemTime.Instance.GetDateTimeByTotalSec(RoleInfo.GetMonthCardRewardTime);//获取VIP的结束时间
    }
    public virtual void SetMonthCardRewardTime(Int64 LogTime)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.GetMonthCardRewardTime = LogTime;
    }


    public virtual Byte GetCashSum()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.CashSum;
    }
    public virtual void SetCashSum(Byte CashSum)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.CashSum = CashSum;
    }
    public Byte BeneFitCount
    {
        set { RoleInfo.benefitCount = value; }
        get { return RoleInfo.benefitCount; }
    }
    public UInt32 BenefitTime
    {
        set { RoleInfo.benefitTime = value; }
        get { return RoleInfo.benefitTime;}
    }
    //public bool Poor
    //{
    //    set { RoleInfo.bpoor = value; }
    //    get { return RoleInfo.bpoor;}
    //}

    public virtual UInt32 GetTotalUseMedal()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.TotalUseMedal;
    }
    public virtual void SetTotalUseMedal(UInt32 TotalUseMedal)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.TotalUseMedal = TotalUseMedal;
    }

    public virtual UInt32 GetParticularStates()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.ParticularStates;
    }
    public virtual void SetParticularStates(UInt32 ParticularStates)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.ParticularStates = ParticularStates;
    }

    public override UInt32 GetGameID()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.GameID;
    }


    public virtual bool GetShareStates()
    {
        if (RoleInfo == null)
            return false;
        else
            return RoleInfo.bShareStates;
    }
    public virtual void SetShareStates(bool States)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.bShareStates = States;
    }

    public virtual UInt32 GetTotalCashSum()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.TotalCashSum;
    }
    public virtual void SetTotalCashSum(UInt32 TotalCashSum)
    {
        if (RoleInfo == null)
            return;
        else
            RoleInfo.TotalCashSum = TotalCashSum;
    }
}
class TableRole : RoleBase //桌子上的玩家数据类
{
    private tagRemoteRoleInfo RoleInfo = null;//玩家的基本数据
    private tagRoleMonthInfo RoleMonthInfo = null;//玩家的比赛数据
    private UInt32 MonthUpperSocre = 0;
    public TableRole(tagRemoteRoleInfo pRoleInfo)
    {
        RoleInfo = pRoleInfo;
        RoleMonthInfo = null;
    }

    public virtual void SetRoleInfo(tagRemoteRoleInfo pRoleInfo)
    {
        RoleInfo = pRoleInfo;
    }
    public virtual void SetMonthInfo(tagRoleMonthInfo pMonthInfo)
    {
        RoleMonthInfo = pMonthInfo;
    }
    public override UInt32 GetUserID()
    {
        if (RoleInfo == null)
            return base.GetUserID();
        return RoleInfo.dwUserID;
    }
    public override string GetNickName()
    {
        if (RoleInfo == null)
            return base.GetNickName();
        return RoleInfo.NickName;
    }
    public override UInt16 GetLevel()
    {
        if (RoleInfo == null)
            return base.GetLevel();
        return RoleInfo.wLevel;
    }
    //public override UInt32 GetExp()
    //{
    //    if (RoleInfo == null)
    //        return base.GetExp();
    //    return RoleInfo.dwExp;
    //}
    public override UInt32 GetFaceID()
    {
        if (RoleInfo == null)
            return base.GetFaceID();
        return RoleInfo.dwFaceID;
    }
    public override bool GetGender()
    {
        if (RoleInfo == null)
            return base.GetGender();
        return RoleInfo.bGender;
    }
    public override UInt32 GetGlobel()
    {
        if (RoleInfo == null)
            return base.GetGlobel();
        return RoleInfo.dwGlobeNum;
    }
    public override UInt32 GetAchievementPoint()
    {
        if (RoleInfo == null)
            return base.GetAchievementPoint();
        return RoleInfo.dwAchievementPoint;
    }
    public override Byte GetTitleID()
    {
        if (RoleInfo == null)
            return base.GetTitleID();
        return RoleInfo.TitleID;
    }
    public override UInt32[] GetCharmInfo()
    {
        if (RoleInfo == null)
            return base.GetCharmInfo();
        return RoleInfo.CharmArray;
    }
    public override Byte GetMonthID()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthID();
        return RoleMonthInfo.bMonthID;
    }
    public override UInt32 GetMonthIndex()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthIndex();
        return RoleMonthInfo.dwIndex;
    }
    public override UInt32 GetMonthGlobel()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthGlobel();
        return RoleMonthInfo.dwMonthGlobel;
    }
    public override UInt32 GetMonthScore()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthScore();
        return RoleMonthInfo.dwMonthScore;
    }
    public override Byte GetMonthAddGlobelNum()
    {
        if (RoleMonthInfo == null)
            return base.GetMonthAddGlobelNum();
        return RoleMonthInfo.wUserAddGlobelNum;
    }
    public override UInt32 GetMonthUpperSocre()
    {
        if (RoleMonthInfo == null || RoleMonthInfo.bMonthID == 0)
            return base.GetMonthUpperSocre();
        return MonthUpperSocre;
    }
    public override bool IsOnline()
    {
        return true;
    }
    public override Byte GetSeat()
    {
        if (RoleInfo == null)
            return base.GetSeat();
        else
            return RoleInfo.SeatID;
    }
    public override void SetUserID(UInt32 dwUserID)
    {
        if (RoleInfo == null)
            base.SetUserID(dwUserID);
        else
            RoleInfo.dwUserID = dwUserID;
    }
    public override void SetNickName(string NickName)
    {
        if (RoleInfo == null)
            base.SetNickName(NickName);
        else
            RoleInfo.NickName = NickName;
    }
    public override void SetLevel(UInt16 wLevel)
    {
        if (RoleInfo == null)
            base.SetLevel(wLevel);
        else
            RoleInfo.wLevel = wLevel;
    }
    //public override void SetExp(UInt32 dwExp)
    //{
    //    if (RoleInfo == null)
    //        base.SetExp(dwExp);
    //    else
    //        RoleInfo.dwExp = dwExp;
    //}
    public override void SetFaceID(UInt32 FaceID)
    {
        if (RoleInfo == null)
            base.SetFaceID(FaceID);
        else
            RoleInfo.dwFaceID = FaceID;
    }
    public override void SetGender(bool bGender)
    {
        if (RoleInfo == null)
            base.SetGender(bGender);
        else
            RoleInfo.bGender = bGender;
    }
    public override void SetGlobel(UInt32 dwGlobel)
    {
        if (RoleInfo == null)
            base.SetGlobel(dwGlobel);
        else
            RoleInfo.dwGlobeNum = dwGlobel;
    }
    public override void SetAchievementPoint(UInt32 dwAchievementPoint)
    {
        if (RoleInfo == null)
            base.SetAchievementPoint(dwAchievementPoint);
        else
            RoleInfo.dwAchievementPoint = dwAchievementPoint;
    }
    public override void SetTitleID(Byte bTitleID)
    {
        if (RoleInfo == null)
            base.SetTitleID(bTitleID);
        else
            RoleInfo.TitleID = bTitleID;
    }
    public override void SetCharmInfo(UInt32[] CharmArray)
    {
        if (RoleInfo == null)
            base.SetCharmInfo(CharmArray);
        else
            RoleInfo.CharmArray = CharmArray;
    }
    public override void SetMonthID(Byte MonthID)
    {
        if (RoleMonthInfo == null)
            base.SetMonthID(MonthID);
        else
            RoleMonthInfo.bMonthID = MonthID;
    }
    public override void SetMonthIndex(UInt32 MonthIndex)
    {
        if (RoleMonthInfo == null)
            base.SetMonthIndex(MonthIndex);
        else
            RoleMonthInfo.dwIndex = MonthIndex;
    }
    public override void SetMonthGlobel(UInt32 MonthGlobel)
    {
        if (RoleMonthInfo == null)
            base.SetMonthGlobel(MonthGlobel);
        else
            RoleMonthInfo.dwMonthGlobel = MonthGlobel;
    }
    public override void SetMonthScore(UInt32 MonthScore)
    {
        if (RoleMonthInfo == null)
            base.SetMonthScore(MonthScore);
        else
            RoleMonthInfo.dwMonthScore = MonthScore;
    }
    public override void SetMonthAddGlobelNum(Byte MonthAddGlobelNum)
    {
        if (RoleMonthInfo == null)
            base.SetMonthAddGlobelNum(MonthAddGlobelNum);
        else
            RoleMonthInfo.wUserAddGlobelNum = MonthAddGlobelNum;
    }
    public override void SetMonthUpperSocre(UInt32 UpperSocre)
    {
        MonthUpperSocre = UpperSocre;
    }
    public override void SetSeat(Byte SeatID)
    {
        RoleInfo.SeatID = SeatID;
    }

    //public override string GetClientIP()
    //{
    //    if (RoleInfo == null)
    //        return 0;
    //    else
    //        return RoleInfo.ClientIP;
    //}
    //public override void SetClientIP(UInt32 ClientIP)
    //{
    //    if (RoleInfo == null)
    //        return;
    //    else
    //        RoleInfo.ClientIP = ClientIP;
    //}
    public override string GetIPAddress()
    {
        if (RoleInfo == null)
            return "";
        else
            return RoleInfo.IPAddress;
    }
    public override void SetIPAddress(string Add)
    {
        RoleInfo.IPAddress = Add;
    }


    public override Byte GetVipLevel()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.VipLevel;
    }
    public override void SetVipLevel(Byte VipLevel)
    {
        if (RoleInfo == null)
            return;
        RoleInfo.VipLevel = VipLevel;
    }
    public override bool GetIsInMonthCard()
    {
        if (RoleInfo == null)
            return false;
        else
            return RoleInfo.IsInMonthCard;
    }
    public override void SetIsInMonthCard(bool IsInMonthCard)
    {
        if (RoleInfo == null)
            return;
        RoleInfo.IsInMonthCard = IsInMonthCard;
    }

    public override uint GetGameID()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.GameID;
    }
}
public class RelationRole : RoleBase  //关系好友数据
{
    private tagRoleRelationClient RoleInfo = null;//玩家的基本数据
    public RelationRole(tagRoleRelationClient pRoleInfo)
    {
        RoleInfo = pRoleInfo;
    }
    public override Byte GetLeaveOnlineDay()
    {
        if (RoleInfo == null)
            return base.GetLeaveOnlineDay();
        return RoleInfo.bDiffDay;
    }
    public override void SetLeaveOnlineDay(Byte DiffDay)
    {
        if (RoleInfo == null)
            base.SetLeaveOnlineDay(DiffDay);
        else
            RoleInfo.bDiffDay = DiffDay;
    }
    public override Byte GetRelationType()
    {
        if (RoleInfo == null)
            return base.GetRelationType();
        return RoleInfo.bRelationType;
    }
    public override void SetRelationType(Byte Type)
    {
        if (RoleInfo == null)
            base.SetRelationType(Type);
        else
            RoleInfo.bRelationType = Type;
    }
    public override UInt32 GetUserID()
    {
        if (RoleInfo == null)
            return base.GetUserID();
        return RoleInfo.dwDestRoleID;
    }
    public override string GetNickName()
    {
        if (RoleInfo == null)
            return base.GetNickName();
        return RoleInfo.DestNickName;
    }
    public override UInt16 GetLevel()
    {
        if (RoleInfo == null)
            return base.GetLevel();
        return RoleInfo.wLevel;
    }
    public override UInt32 GetFaceID()
    {
        if (RoleInfo == null)
            return base.GetFaceID();
        return RoleInfo.dwFaceID;
    }
    public override bool GetGender()
    {
        if (RoleInfo == null)
            return base.GetGender();
        return RoleInfo.bGender;
    }
    public override UInt32 GetAchievementPoint()
    {
        if (RoleInfo == null)
            return base.GetAchievementPoint();
        return RoleInfo.dwAchievementPoint;
    }
    public override Byte GetTitleID()
    {
        if (RoleInfo == null)
            return base.GetTitleID();
        return RoleInfo.TitleID;
    }
    public override UInt32[] GetCharmInfo()
    {
        if (RoleInfo == null)
            return base.GetCharmInfo();
        return RoleInfo.CharmArray;
    }
    public override void SetUserID(UInt32 dwUserID)
    {
        if (RoleInfo == null)
            base.SetUserID(dwUserID);
        else
            RoleInfo.dwDestRoleID = dwUserID;
    }
    public override void SetNickName(string NickName)
    {
        if (RoleInfo == null)
            base.SetNickName(NickName);
        else
            RoleInfo.DestNickName = NickName;
    }
    public override void SetLevel(UInt16 wLevel)
    {
        if (RoleInfo == null)
            base.SetLevel(wLevel);
        else
            RoleInfo.wLevel = wLevel;
    }
    public override void SetFaceID(UInt32 FaceID)
    {
        if (RoleInfo == null)
            base.SetFaceID(FaceID);
        else
            RoleInfo.dwFaceID = FaceID;
    }
    public override void SetGender(bool bGender)
    {
        if (RoleInfo == null)
            base.SetGender(bGender);
        else
            RoleInfo.bGender = bGender;
    }
    public override void SetAchievementPoint(UInt32 dwAchievementPoint)
    {
        if (RoleInfo == null)
            base.SetAchievementPoint(dwAchievementPoint);
        else
            RoleInfo.dwAchievementPoint = dwAchievementPoint;
    }
    public override void SetTitleID(Byte bTitleID)
    {
        if (RoleInfo == null)
            base.SetTitleID(bTitleID);
        else
            RoleInfo.TitleID = bTitleID;
    }
    public override void SetCharmInfo(UInt32[] CharmArray)
    {
        if (RoleInfo == null)
            base.SetCharmInfo(CharmArray);
        else
            RoleInfo.CharmArray = CharmArray;
    }
    public override bool IsOnline()
    {
        if (RoleInfo == null)
            return IsOnline();
        return RoleInfo.bDiffDay == 255;//判断玩家是否在线
    }

    //public override UInt32 GetClientIP()
    //{
    //    if (RoleInfo == null)
    //        return 0;
    //    else
    //        return RoleInfo.ClientIP;
    //}
    //public override void SetClientIP(UInt32 ClientIP)
    //{
    //    if (RoleInfo == null)
    //        return;
    //    else
    //        RoleInfo.ClientIP = ClientIP;
    //}

    public override string GetIPAddress()
    {
        if (RoleInfo == null)
            return "";
        else
            return RoleInfo.IPAddress;
    }
    public override void SetIPAddress(string Add)
    {
        RoleInfo.IPAddress = Add;
    }

    public override Byte GetVipLevel()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.VipLevel;
    }
    public override void SetVipLevel(Byte VipLevel)
    {
        if (RoleInfo == null)
            return;
        RoleInfo.VipLevel = VipLevel;
    }
    public override bool GetIsInMonthCard()
    {
        if (RoleInfo == null)
            return false;
        else
            return RoleInfo.IsInMonthCard;
    }
    public override void SetIsInMonthCard(bool IsInMonthCard)
    {
        if (RoleInfo == null)
            return;
        RoleInfo.IsInMonthCard = IsInMonthCard;
    }

    public override uint GetGameID()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.GameID;
    }
}
public class QueryRole : RoleBase //通过查询获得的用户数据
{
    private tagQueryRoleInfoClient RoleInfo = null;//玩家的基本数据
    public QueryRole(tagQueryRoleInfoClient pRoleInfo)
    {
        RoleInfo = pRoleInfo;
    }
    public override bool IsOnline()
    {
        if (RoleInfo == null)
            return base.IsOnline();
        return RoleInfo.bIsOnline;
    }
    public override void SetIsOnline(bool IsOnline)
    {
        if (RoleInfo == null)
            base.SetIsOnline(IsOnline);
        else
            RoleInfo.bIsOnline = IsOnline;
    }
    public override UInt32 GetUserID()
    {
        if (RoleInfo == null)
            return base.GetUserID();
        return RoleInfo.dwUserID;
    }
    public override string GetNickName()
    {
        if (RoleInfo == null)
            return base.GetNickName();
        return RoleInfo.NickName;
    }
    public override UInt16 GetLevel()
    {
        if (RoleInfo == null)
            return base.GetLevel();
        return RoleInfo.wLevel;
    }
    public override UInt32 GetFaceID()
    {
        if (RoleInfo == null)
            return base.GetFaceID();
        return RoleInfo.dwFaceID;
    }
    public override bool GetGender()
    {
        if (RoleInfo == null)
            return base.GetGender();
        return RoleInfo.bGender;
    }
    public override UInt32 GetAchievementPoint()
    {
        if (RoleInfo == null)
            return base.GetAchievementPoint();
        return RoleInfo.dwAchievementPoint;
    }
    public override Byte GetTitleID()
    {
        if (RoleInfo == null)
            return base.GetTitleID();
        return RoleInfo.TitleID;
    }
    public override UInt32[] GetCharmInfo()
    {
        if (RoleInfo == null)
            return base.GetCharmInfo();
        return RoleInfo.CharmArray;
    }
    public override void SetUserID(UInt32 dwUserID)
    {
        if (RoleInfo == null)
            base.SetUserID(dwUserID);
        else
            RoleInfo.dwUserID = dwUserID;
    }
    public override void SetNickName(string NickName)
    {
        if (RoleInfo == null)
            base.SetNickName(NickName);
        else
            RoleInfo.NickName = NickName;
    }
    public override void SetLevel(UInt16 wLevel)
    {
        if (RoleInfo == null)
            base.SetLevel(wLevel);
        else
            RoleInfo.wLevel = wLevel;
    }
    public override void SetFaceID(UInt32 FaceID)
    {
        if (RoleInfo == null)
            base.SetFaceID(FaceID);
        else
            RoleInfo.dwFaceID = FaceID;
    }
    public override void SetGender(bool bGender)
    {
        if (RoleInfo == null)
            base.SetGender(bGender);
        else
            RoleInfo.bGender = bGender;
    }
    public override void SetAchievementPoint(UInt32 dwAchievementPoint)
    {
        if (RoleInfo == null)
            base.SetAchievementPoint(dwAchievementPoint);
        else
            RoleInfo.dwAchievementPoint = dwAchievementPoint;
    }
    public override void SetTitleID(Byte bTitleID)
    {
        if (RoleInfo == null)
            base.SetTitleID(bTitleID);
        else
            RoleInfo.TitleID = bTitleID;
    }
    public override void SetCharmInfo(UInt32[] CharmArray)
    {
        if (RoleInfo == null)
            base.SetCharmInfo(CharmArray);
        else
            RoleInfo.CharmArray = CharmArray;
    }

    //public override UInt32 GetClientIP()
    //{
    //    if (RoleInfo == null)
    //        return 0;
    //    else
    //        return RoleInfo.ClientIP;
    //}
    //public override void SetClientIP(UInt32 ClientIP)
    //{
    //    if (RoleInfo == null)
    //        return;
    //    else
    //        RoleInfo.ClientIP = ClientIP;
    //}

    public override string GetIPAddress()
    {
        if (RoleInfo == null)
            return "";
        else
            return RoleInfo.IPAddress;
    }
    public override void SetIPAddress(string Add)
    {
        RoleInfo.IPAddress = Add;
    }

    public override Byte GetVipLevel()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.VipLevel;
    }
    public override void SetVipLevel(Byte VipLevel)
    {
        if (RoleInfo == null)
            return;
        RoleInfo.VipLevel = VipLevel;
    }
    public override bool GetIsInMonthCard()
    {
        if (RoleInfo == null)
            return false;
        else
            return RoleInfo.IsInMonthCard;
    }
    public override void SetIsInMonthCard(bool IsInMonthCard)
    {
        if (RoleInfo == null)
            return;
        RoleInfo.IsInMonthCard = IsInMonthCard;
    }

    public override uint GetGameID()
    {
        if (RoleInfo == null)
            return 0;
        else
            return RoleInfo.GameID;
    }
}
