using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using UnityEngine;
class tagRoleRelation
{
    public Byte MaxRelationSum;//最大关系的数量
}
class RoleCheckConfig //签到的配置信息
{
    public UInt32 CheckOtherUser;//补签需要的花费
    public Byte MaxCheckDaySum;
    public Dictionary<Byte, UInt16> CheckDayReward = new Dictionary<Byte, UInt16>();
    public Dictionary<Byte, UInt16> MonthCheckReward = new Dictionary<Byte, UInt16>();
};
class tagTaskConfig//单条任务配置
{
    public Byte TaskID;
    public EventInfo TaskEventInfo = new EventInfo();
    public Byte UpperTaskID;
    public bool IsDayClear;
    public UInt16 RewardID;
    public Byte GroupID;//同一分组下的任务互斥 并且 只有每日任务才可以进行分组
    public Byte JoinLevel;
    public List<Byte> LowerTaskVec = new List<Byte>();
}
class tagTaskStrInfo
{
    public Byte TaskID;
    public string TaskName;
    public string TaskDec;
    public string TaskIcon;
}
class tagTaskMap //任务的数据
{
    public Byte m_MaxJoinTaskSum;
    public List<tagTaskConfig> m_TaskVec = new List<tagTaskConfig>();
    public Dictionary<Byte, tagTaskConfig> m_TaskMap = new Dictionary<Byte, tagTaskConfig>();
    public Dictionary<Byte, tagTaskStrInfo> m_TaskStr = new Dictionary<Byte, tagTaskStrInfo>();
}

//成就配置
class tagAchievementConfig
{
    public Byte AchievementID;
    public EventInfo AchievementEventInfo = new EventInfo();
    public Byte UpperAchievementID;
    public UInt16 RewardID;
    public Byte JoinLevel;
    public List<Byte> LowerAchievementVec = new List<Byte>();
};
class tagAchievementStrInfo
{
    public Byte AchievementID;
    public string AchievementName;
    public string AchievementDec;
    public int AchievementStar;//1-3
    public string AchievementIcon;
}
class tagAchievementMap
{
    public Byte m_MaxJoinAchievementSum;
    public List<tagAchievementConfig> m_AchievementVec = new List<tagAchievementConfig>();
    public Dictionary<Byte, tagAchievementConfig> m_AchievementMap = new Dictionary<Byte, tagAchievementConfig>();
    public Dictionary<Byte, tagAchievementStrInfo> m_AchievementStr = new Dictionary<Byte, tagAchievementStrInfo>();
}
//比赛数据
class tagMonthConvert
{
    public Byte ConvertSum;
    public UInt32 LostGlobel;
    public UInt32 LostMedal;
    public UInt32 LostCurrey;
    public UInt32 AddMonthGlobel;
};



class tagMonthTime
{

    public Byte ID;//标记当前的阶段
    //开始阶段
    public Byte BeginWeek;
    public Byte BeginHour;
    public Byte BeginMin;
    //WORD		LastMin;
    //阶段的持续时间
    public UInt16 AllLastMin;//这个节点持续时间
    //阶段的等待时间
    public UInt16 WriteLastMin;//下一个阶段的开始时间
    //报名阶段
    public UInt16 SignWriteMin;
    public UInt16 SignLastMin;
    //开始阶段
    public UInt16 StarWriteMin;
    public UInt16 SratLastMin;
    //
    public bool IsContinue;
    public UInt16 RunSum;
    public DateTime GetBeginTime()
    {
        //获取开始时间
        DateTime NowTime = SystemTime.Instance.GetSystemDateTime;
        if(BeginWeek == 8)
        {
            int DiffHour = NowTime.Hour - BeginHour;
            int DiffMin = NowTime.Minute - BeginMin;
            int DiffSec = DiffHour * 3600 + DiffMin * 60;
            if (DiffSec >= 0)
            {
                DateTime BeginTime = NowTime.AddSeconds(Convert.ToInt32(DiffSec * -1));
                return FishConfig.Instance.GetMinDateTime(BeginTime);
            }
            else
            {
                DiffSec += 24 * 3600;
                DateTime BeginTime = NowTime.AddSeconds(Convert.ToInt32(DiffSec * -1));
                return FishConfig.Instance.GetMinDateTime(BeginTime);
            }
        }
        else
        {
            int DiffDay = (Byte)NowTime.DayOfWeek - BeginWeek;
            int DiffHour = NowTime.Hour - BeginHour;
            int DiffMin = NowTime.Minute - BeginMin;
            int DiffSec = DiffDay * 24 * 3600 + DiffHour * 3600 + DiffMin * 60;
            if (DiffSec < 0)
            {
                DiffSec = 7 * 3600 * 24 + DiffSec;
            }
            DateTime BeginTime = NowTime.AddSeconds(Convert.ToInt32(DiffSec * -1));
            return FishConfig.Instance.GetMinDateTime(BeginTime);//获取上次的开始时间
        }
        
    }
    public UInt32 IsInMonthTime()
    {
        DateTime Now = SystemTime.Instance.GetSystemDateTime;
        DateTime BeginTime = GetBeginTime();//比赛的开始时间
        //有些比赛不是循环的 我们想要判断
        if (!IsContinue)
        {
            //一次性的时间判断函数
           if( (BeginTime.AddMinutes(AllLastMin) - Now).TotalSeconds > 0)//不可以为 =
           {
               return Convert.ToUInt32(ID << 16);
           }
           else
           {
               return 0;
           }
        }
        double DouSec = (Now - BeginTime).TotalSeconds;
        if (DouSec < 0)
            DouSec = 0;
        UInt32 DiffSec = Convert.ToUInt32(DouSec);//差距的秒数
        //一个完整阶段的时间 AllLastMin + WriteLastMin
        UInt32 nValue =Convert.ToUInt32((DiffSec / 60) % (AllLastMin + WriteLastMin));
       // UInt32 ContinueSum = Convert.ToUInt32(nValue == 0 ? ((DiffSec / 60) / (AllLastMin + WriteLastMin)) : ((DiffSec / 60) / (AllLastMin + WriteLastMin) + 1));//循环的次数
        UInt32 ContinueSum = Convert.ToUInt32((DiffSec / 60) / (AllLastMin + WriteLastMin));//循环的次数
        if (ContinueSum >= RunSum)
        {
            //已经超过了 不在循环
            return 0;
        }
        else if (nValue < AllLastMin)
        {
            UInt16 Sum = Convert.ToUInt16((DiffSec / 60) / (AllLastMin + WriteLastMin));
            return Convert.ToUInt32((ID << 16) + Sum);
        }
        else if (nValue >= AllLastMin && nValue < (AllLastMin + WriteLastMin))
            return 0;
        else
            return 0;
    }
    public bool IsInSignTime()
    {
        //是否在比赛阶段
        DateTime Now = SystemTime.Instance.GetSystemDateTime;
        DateTime BeginTime = GetBeginTime();//比赛的开始时间
        if (!IsContinue)
        {
            //一次性的时间判断函数
            if ((BeginTime.AddMinutes(AllLastMin) - Now).TotalSeconds > 0)
            {
                DateTime SignBegin = BeginTime.AddMinutes(SignWriteMin);
                DateTime SignEnd = BeginTime.AddMinutes(SignWriteMin + SignLastMin);
                if ((Now - SignBegin).TotalSeconds >= 0 && (SignEnd - Now).TotalSeconds > 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
                return false;  
        }
        double DouSec = (Now - BeginTime).TotalSeconds;
        if (DouSec < 0)
            DouSec = 0;
        UInt32 DiffSec = Convert.ToUInt32(DouSec);//差距的秒数
        //一个完整阶段的时间 AllLastMin + WriteLastMin
        UInt32 nValue = Convert.ToUInt32((DiffSec / 60) % (AllLastMin + WriteLastMin));
        //UInt32 ContinueSum = Convert.ToUInt32(nValue == 0 ? ((DiffSec / 60) / (AllLastMin + WriteLastMin)) : ((DiffSec / 60) / (AllLastMin + WriteLastMin) + 1));//循环的次数
        UInt32 ContinueSum = Convert.ToUInt32((DiffSec / 60) / (AllLastMin + WriteLastMin));//循环的次数
        if (ContinueSum >= RunSum)
        {
            //已经超过了 不在循环
            return false;
        }
        else if (nValue < AllLastMin)
        {
            //在比赛阶段 继续判断
            if (nValue >= SignWriteMin && nValue < (SignWriteMin + SignLastMin))
                return true;
            else
                return false;
        }
        else if (nValue >= AllLastMin && nValue < (AllLastMin + WriteLastMin))
            return false;
        else
        {
            return false;
        }
    }
    public bool IsInStarTime()
    {
        //是否在比赛阶段
        DateTime Now = SystemTime.Instance.GetSystemDateTime;
        DateTime BeginTime = GetBeginTime();//比赛的开始时间
        if (!IsContinue)
        {
            //一次性的时间判断函数
            if ((BeginTime.AddMinutes(AllLastMin) - Now).TotalSeconds > 0)
            {
                DateTime StarBegin = BeginTime.AddMinutes(StarWriteMin);
                DateTime StarEnd = BeginTime.AddMinutes(StarWriteMin + SratLastMin);
                if ((Now - StarBegin).TotalSeconds >= 0 && (StarEnd - Now).TotalSeconds > 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
                return false;  
        }
        double DouSec = (Now - BeginTime).TotalSeconds;
        if (DouSec < 0)
            DouSec = 0;
        UInt32 DiffSec = Convert.ToUInt32(DouSec);//差距的秒数
        //一个完整阶段的时间 AllLastMin + WriteLastMin
        UInt32 nValue = Convert.ToUInt32((DiffSec / 60) % (AllLastMin + WriteLastMin));
       // UInt32 ContinueSum = Convert.ToUInt32(nValue == 0 ? ((DiffSec / 60) / (AllLastMin + WriteLastMin)) : ((DiffSec / 60) / (AllLastMin + WriteLastMin) + 1));//循环的次数
        UInt32 ContinueSum = Convert.ToUInt32((DiffSec / 60) / (AllLastMin + WriteLastMin));//循环的次数
        if (ContinueSum >= RunSum)
        {
            //已经超过了 不在循环
            return false;
        }
        else if (nValue < AllLastMin)
        {
            //在比赛阶段 继续判断
            if (nValue >= StarWriteMin && nValue < (StarWriteMin + SratLastMin))
                return true;
            else
                return false;
        }
        else if (nValue >= AllLastMin && nValue < (AllLastMin + WriteLastMin))
            return false;
        else
        {
            return false;
        }
    }
    public bool IsBeginStar()
    {
        //刚刚开始
        DateTime Now = SystemTime.Instance.GetSystemDateTime;
        DateTime BeginTime = GetBeginTime();//比赛的开始时间
        if (!IsContinue)
        {
            if (Now.DayOfWeek == BeginTime.DayOfWeek && Now.Hour == BeginTime.Hour && Now.Minute == BeginTime.Minute)
                return true;
            else
                return false;
        }
        double DouSec = (Now - BeginTime).TotalSeconds;
        if (DouSec < 0)
            DouSec = 0;
        UInt32 DiffSec = Convert.ToUInt32(DouSec);//差距的秒数
        //一个完整阶段的时间 AllLastMin + WriteLastMin
        UInt32 nValue = Convert.ToUInt32((DiffSec / 60) % (AllLastMin + WriteLastMin));
        if (nValue == StarWriteMin)
            return true;
        else
            return false;
    }

    public DateTime GetNowOrNextSignTime()
    {
        //获取最近的报名时间
        DateTime Now = SystemTime.Instance.GetSystemDateTime;
        DateTime BeginTime = GetBeginTime();//比赛的开始时间
        if (!IsContinue)
        {
            //一次性的时间判断函数
            if ((BeginTime.AddMinutes(AllLastMin) - Now).TotalSeconds >= 0)
            {
                DateTime SignBegin = BeginTime.AddMinutes(SignWriteMin);
                DateTime SignEnd = BeginTime.AddMinutes(SignWriteMin + SignLastMin);
                if ((Now - SignBegin).TotalSeconds >= 0 && (SignEnd - Now).TotalSeconds > 0)
                {
                    return SignBegin;
                }
                else
                {
                    //不在报名期限类 下个星期的
                    return FishConfig.Instance.GetMinDateTime(SignBegin.AddDays((BeginWeek == 8 ? 1:7)));
                }
            }
            else
            {
                //已经过期了 我们返回下一个星期的时间.
                DateTime SignBegin = BeginTime.AddMinutes(SignWriteMin);
                return FishConfig.Instance.GetMinDateTime(SignBegin.AddDays((BeginWeek == 8 ? 1 : 7)));
            }
        }
        double DouSec = (Now - BeginTime).TotalSeconds;
        if (DouSec < 0)
            DouSec = 0;
        UInt32 DiffSec = Convert.ToUInt32(DouSec);//差距的秒数
        //一个完整阶段的时间 AllLastMin + WriteLastMin
        UInt32 nValue = Convert.ToUInt32((DiffSec / 60) % (AllLastMin + WriteLastMin));
       // UInt32 ContinueSum = Convert.ToUInt32(nValue == 0 ? ((DiffSec / 60) / (AllLastMin + WriteLastMin)) : ((DiffSec / 60) / (AllLastMin + WriteLastMin) + 1));//循环的次数
        //循环次数 的判断  最后次 无须添加等待时间
        UInt32 ContinueSum = Convert.ToUInt32((DiffSec / 60) / (AllLastMin + WriteLastMin));//循环的次数
        if (ContinueSum >= RunSum)
        {
            DateTime SignBegin = BeginTime.AddMinutes(SignWriteMin);
            return FishConfig.Instance.GetMinDateTime(SignBegin.AddDays((BeginWeek == 8 ? 1 : 7)));
        }
        else if (nValue < AllLastMin)
        {
            //在比赛阶段 继续判断
            if (nValue < SignWriteMin)
            {
                //在报名前的阶段
                return FishConfig.Instance.GetMinDateTime(Now.AddMinutes(Convert.ToInt32((int)SignWriteMin - (int)nValue)));//减掉时间
            }
            else if (nValue >= SignWriteMin && nValue < (SignWriteMin + SignLastMin))
            {
                //返回当前的比赛时间
                return FishConfig.Instance.GetMinDateTime(Now.AddMinutes(Convert.ToInt32((int)SignWriteMin - (int)nValue)));//减掉时间
            }
            else
            {
                //在报名后的阶段
                return FishConfig.Instance.GetMinDateTime(Now.AddMinutes(Convert.ToInt32((int)SignWriteMin - (int)nValue + (int)AllLastMin + (int)WriteLastMin)));
            }
        }
        else if (nValue >= AllLastMin && nValue < (AllLastMin + WriteLastMin))
        {
            if ((ContinueSum +1 ) >= RunSum)
            {
                DateTime SignBegin = BeginTime.AddMinutes(SignWriteMin);
                return FishConfig.Instance.GetMinDateTime(SignBegin.AddDays((BeginWeek == 8 ? 1 : 7)));
            }
            else
            {
                return FishConfig.Instance.GetMinDateTime(Now.AddMinutes(Convert.ToInt32((int)AllLastMin + (int)WriteLastMin - (int)nValue + (int)SignWriteMin)));
            }
        }
        else
        {
            DateTime ErrorTime = new DateTime();
            return ErrorTime;
        }
    }
    public DateTime GetNowOrNextStarTime()
    {
        DateTime Now = SystemTime.Instance.GetSystemDateTime;
        DateTime BeginTime = GetBeginTime();//比赛的开始时间
        if (!IsContinue)
        {
            //一次性的时间判断函数
            if ((BeginTime.AddMinutes(AllLastMin) - Now).TotalSeconds > 0)
            {
                DateTime StarBegin = BeginTime.AddMinutes(StarWriteMin);
                DateTime StarEnd = BeginTime.AddMinutes(StarWriteMin + SratLastMin);
                if ((Now - StarBegin).TotalSeconds >= 0 && (StarEnd - Now).TotalSeconds > 0)
                {
                    return FishConfig.Instance.GetMinDateTime(StarBegin);
                }
                else
                {
                    //不在报名期限类 下个星期的
                    return FishConfig.Instance.GetMinDateTime(StarBegin.AddDays((BeginWeek == 8 ? 1 : 7)));
                }
            }
            else
            {
                //已经过期了 我们返回下一个星期的时间.
                DateTime StarBegin = BeginTime.AddMinutes(StarWriteMin);
                return FishConfig.Instance.GetMinDateTime(StarBegin.AddDays((BeginWeek == 8 ? 1 : 7)));
            }
        }
        double DouSec = (Now - BeginTime).TotalSeconds;
        if (DouSec < 0)
            DouSec = 0;
        UInt32 DiffSec = Convert.ToUInt32(DouSec);//差距的秒数
        //一个完整阶段的时间 AllLastMin + WriteLastMin
        UInt32 nValue = Convert.ToUInt32((DiffSec / 60) % (AllLastMin + WriteLastMin));
       // UInt32 ContinueSum = Convert.ToUInt32(nValue == 0 ? ((DiffSec / 60) / (AllLastMin + WriteLastMin)) : ((DiffSec / 60) / (AllLastMin + WriteLastMin) + 1));//循环的次数
        UInt32 ContinueSum = Convert.ToUInt32((DiffSec / 60) / (AllLastMin + WriteLastMin));//循环的次数
        if (ContinueSum >= RunSum)
        {
            DateTime SignBegin = BeginTime.AddMinutes(StarWriteMin);
            return FishConfig.Instance.GetMinDateTime(SignBegin.AddDays((BeginWeek == 8 ? 1 : 7)));
        }
        else if (nValue < AllLastMin)
        {
            //在比赛阶段 继续判断
            if (nValue < StarWriteMin)
            {
                return FishConfig.Instance.GetMinDateTime(Now.AddMinutes(Convert.ToInt32((int)StarWriteMin - (int)nValue)));
            }
            else if (nValue >= StarWriteMin && nValue < (StarWriteMin + SratLastMin))
            {
                //返回当前的比赛时间
                return FishConfig.Instance.GetMinDateTime(Now.AddMinutes(Convert.ToInt32((int)StarWriteMin - (int)nValue)));
            }
            else
            {
                //在游戏阶段   返回下一次报名时间
                return FishConfig.Instance.GetMinDateTime(Now.AddMinutes(Convert.ToInt32((int)AllLastMin + (int)WriteLastMin - (int)nValue + (int)StarWriteMin)));
            }
        }
        else if (nValue >= AllLastMin && nValue < (AllLastMin + WriteLastMin))
        {
            //在等待阶段
            if ((ContinueSum + 1) >= RunSum)
            {
                DateTime SignBegin = BeginTime.AddMinutes(StarWriteMin);
                return FishConfig.Instance.GetMinDateTime(SignBegin.AddDays((BeginWeek == 8 ? 1 : 7)));
            }
            else
            {
                return FishConfig.Instance.GetMinDateTime(Now.AddMinutes(Convert.ToInt32((int)AllLastMin + (int)WriteLastMin - (int)nValue + (int)StarWriteMin)));
            }           
        }
        else
        {
            DateTime ErrorTime = new DateTime();
            return ErrorTime;
        }
    }

    public DateTime GetMaxNearEndMonthTime(UInt32 WriteTimeMin)
    {
        //有可能是获取上次的最后一个时间段 也可能是获得 当次最后一个时间段
        //当前时间 扣去时间
        DateTime NowTime = SystemTime.Instance.GetSystemDateTime.AddMinutes((Int32)WriteTimeMin * -1);
        //获取比赛的开始时间
        DateTime BeginTime = GetBeginTime();
        DateTime BeginStarTime = BeginTime.AddMinutes(StarWriteMin);//比赛的开始比赛的时间 
        DateTime BeginStarEndTime = BeginStarTime.AddMinutes(SratLastMin);//获取活动的开始和结束的时间
        UInt32 OnceMonthAllMin = Convert.ToUInt32(AllLastMin + WriteLastMin);//一个完整阶段的时间
        if(!IsContinue)
        {
            if((NowTime - BeginStarTime).TotalSeconds <= 0)
            {
                return FishConfig.Instance.GetMinDateTime(BeginStarTime.AddDays((BeginWeek == 8 ? -1:-7)));//返回7天前的阶段时间
            }
            else if((BeginStarEndTime - NowTime).TotalSeconds >= 0)
            {
                return FishConfig.Instance.GetMinDateTime(BeginStarTime.AddDays((BeginWeek == 8 ? -1:-7)));//返回7天前的阶段时间
            }
            else
            {
                //当前比赛已经结束了
                return FishConfig.Instance.GetMinDateTime(BeginStarTime);
            }
        }
        //处理循环的赛事
        //找到 比赛的时间段 和 当前时间
        DateTime EndStarTime = BeginStarTime.AddMinutes((RunSum-1) * OnceMonthAllMin);
        DateTime EndStarEndTime = BeginStarEndTime.AddMinutes((RunSum - 1) * OnceMonthAllMin);
        if((NowTime - BeginStarEndTime).TotalSeconds <= 0)
        {
            return FishConfig.Instance.GetMinDateTime(EndStarTime.AddDays((BeginWeek == 8 ? -1:-7)));
        }
        else if((NowTime - EndStarEndTime).TotalSeconds >= 0)
        {
            return FishConfig.Instance.GetMinDateTime(EndStarTime);
        }
        else
        {
            //在当天内 我们获取最近的
            UInt32 TotalMin = Convert.ToUInt32((NowTime - BeginTime).TotalMinutes);
            UInt32 nValue = Convert.ToUInt32(TotalMin % OnceMonthAllMin);//比赛开始多少分钟了
            UInt32 Num = Convert.ToUInt32(TotalMin / OnceMonthAllMin);//已经进行过的轮数
            DateTime LogBeginTime =  BeginTime.AddMinutes((Num-1)* OnceMonthAllMin);
            return FishConfig.Instance.GetMinDateTime(LogBeginTime.AddMinutes(StarWriteMin));
        }
    }
};
class tagMonthReward
{
    public UInt16 Index;
    public UInt16 RewardID;
};
class tagMonthConfig
{
    public Byte MonthID;
    public UInt32 SignGlobel;
    public UInt32 SignMadel;
    public UInt32 SignCurrey;
    public tagItemOnce SignItem = new tagItemOnce();//参加比赛需要扣的物品和数量
    public UInt32 InitMonthGlobel;//玩家进入比赛系统给的金币；此处比赛只能用系统给的金币，金币不足时可以续币
    public Byte TableTypeID;
    public Byte RankSum;
    public UInt32 MaxConvertSum;
    public Byte MinRateLimit;
    public Byte TypeID;
    public Dictionary<Byte, tagMonthConvert> m_ConvertMap = new Dictionary<Byte, tagMonthConvert>();//续币
    public Dictionary<Byte, tagMonthTime> m_MonthTime = new Dictionary<Byte,tagMonthTime>();//比赛时间
    public Dictionary<Byte, tagMonthReward> m_MonthReward = new Dictionary<Byte, tagMonthReward>();//比赛奖励
    public Dictionary<UInt32, string> m_MonthChannelName = new Dictionary<UInt32, string>();//比赛渠道名称

    public string GetMonthChannelName()
    {
        string ChannelName = "";
        if (SDKMgr.Instance.LoginData != null && SDKMgr.Instance.LoginData.ChannelLabel!= null)
            ChannelName = SDKMgr.Instance.LoginData.ChannelLabel.ToLower().Trim();
        UInt32 ChannelID = 0;
        if (ChannelName.Length != 0)
            ChannelID = Crc.Crc32(Encoding.Default.GetBytes(ChannelName.ToCharArray()), 0, ChannelName.Length);
        if (ChannelID == 0)
            return null;
        string strName = "";
        if (!m_MonthChannelName.TryGetValue(ChannelID, out strName))
            return null;
        else
            return strName;
    }
    public Byte NowIsInSignTime()
    {
        foreach (tagMonthTime var in m_MonthTime.Values)
        {
            if (var.IsInSignTime())
                return var.ID;
        }   
        return 0;
    }
    public Byte NowIsInStartTime()
    {
        foreach (tagMonthTime var in m_MonthTime.Values)
        {
            if (var.IsInStarTime())
                return var.ID;
        }
        return 0;
    }
    public bool NowIsBeginStar()
    {
        foreach (tagMonthTime var in m_MonthTime.Values)
        {
            if (var.IsBeginStar())
                return true;
        }
        return false;
    }
    public void GetMaxNearEndMonthStarTime(UInt32 WriteTimeMin,out DateTime BeginTime,out DateTime EndTime)
    {
        bool IsLog = false;
        BeginTime = new DateTime();
        EndTime = new DateTime();
        DateTime Now = SystemTime.Instance.GetSystemDateTime;
        //获取最近结束的比赛 的  比赛的开始时间
        foreach (tagMonthTime var in m_MonthTime.Values)
        {
            DateTime nBeginTime = var.GetMaxNearEndMonthTime(WriteTimeMin);
            DateTime nEndTime = nBeginTime.AddMinutes(var.SratLastMin);

            if (!IsLog)
            {
                BeginTime = nBeginTime;
                EndTime = nEndTime;
                IsLog = true;
            }
            else
            {
                if ((nEndTime - EndTime).TotalSeconds > 0)
                {
                    BeginTime = nBeginTime;
                    EndTime = nEndTime;
                }
            }
        }
    }
};
class tagMonthStr
{
    public Byte MonthID;
    public string MonthName;
    public string MonthDec;
    public string MonthIcon;
}
class tagMonthMap
{
    public Byte BeginMonthLevel;
    public List<tagMonthConfig> m_MonthVec = new List<tagMonthConfig>();
    public Dictionary<Byte, tagMonthStr> m_MonthStr = new Dictionary<Byte, tagMonthStr>();
    public Dictionary<Byte, tagMonthConfig> m_MonthMap = new Dictionary<Byte, tagMonthConfig>();
};
//称号
class tagTitleConfig
{
    public Byte TitleID;
    public string TitleName;
}
class tagTitleMap
{
    public Dictionary<Byte, tagTitleConfig> m_TileMap = new Dictionary<Byte, tagTitleConfig>();
}

//物品
public enum EItemType
{
    IT_Globel = 1,//金币 
    IT_Medal = 2,//奖牌
    IT_AchievementPoint = 3,//成就点
    IT_Title = 4,//称号
    IT_Currey = 5,//乐币
    IT_MonthSocre = 6,//比赛积分
    IT_MonthGlobel = 7,//比赛金币
    IT_Normal = 8,//普通物品
    IT_Skill = 9,//技能物品
    IT_Package = 10,//礼包物品
    IT_Charm = 11,//魅力物品
    IT_Giff = 12,//赠送物品
    IT_Cannon = 13,//炮台物品
    IT_OsCar = 14,//潜艇物品
    IT_Entity = 15,//实体物品 现实的物品
    IT_MonthCard = 16,//月卡物品
    IT_GlobelBag = 17,//聚宝盆物品
    IT_Horn      = 18,//喇叭
}
class tagItemConfig
{
    public UInt32 ItemID;
    public EItemType ItemTypeID;
    public UInt32 ItemParam;
    public string ItemName;
    public string ItemIcon;
    public string ItemDec;
}
class tagItemMap
{
    //Key 为ItemID
    public Dictionary<UInt32, tagItemConfig> m_ItemMap = new Dictionary<UInt32, tagItemConfig>();
}

//排行榜
class tagRankReward
{
    public Byte Index;
    public UInt16 RewardID;
};

class tagRankConfig
{
    public Byte RankID;
    public Byte RowCount;
    public Dictionary<Byte, tagRankReward> RewardMap = new Dictionary<Byte, tagRankReward>();
};
class tagRankMap
{
    public Byte RewardWeekDay;
    public Dictionary<Byte, tagRankConfig> m_RankMap = new Dictionary<Byte, tagRankConfig>();
};

//排行榜的具体信息
public class tagRankInfo
{
    public UInt32   dwUserID;
    public Byte     bIndex;
    public UInt32   dwFaceID;
    public Byte     TitleID;
    public bool     Gender;
    public string   NickName;
    public Int64    Param;
};
class RankInfo
{
    public Byte RankID;
    public Dictionary<UInt32, tagRankInfo> RankMap = new Dictionary<UInt32, tagRankInfo>();
}
class RankXmlInfo
{
    public Dictionary<Byte, RankInfo> RankXmlMap = new Dictionary<Byte, RankInfo>();
}

//宝箱的配置文件
class tagChestsCost
{
    public Byte MaxCostNum;
    public Dictionary<Byte, UInt32> CostMap = new Dictionary<Byte, UInt32>();
}
public class tagChestsRewardOnce
{
    public Byte OnceID;
    public UInt32 Chances;
    public UInt16 RewardID;
}
class tagChestsReward
{
    public Byte RewardID;
    public UInt32 Chances;
    public UInt32 MaxOnceChances;
    // public List<tagChestsRewardOnce> RewardMap = new List<tagChestsRewardOnce>();
    public Dictionary<Byte, tagChestsRewardOnce> RewardLog = new Dictionary<Byte, tagChestsRewardOnce>();
};
class tagChestsAllReward
{
    public Byte SpecialRewardTypeID;
    public Byte SpecialRewardUseRp;
    public Byte OtherAddRp;
    public UInt32 MaxRewardChances;
    //public List<tagChestsReward> RewardVec = new List<tagChestsReward>();
    public Dictionary<Byte, tagChestsReward> RewardMap = new Dictionary<Byte, tagChestsReward>();
};
class tagChestConfig
{
    public Byte ChestID;
    public Byte ExisteSec;
    public UInt16 ImmediateRewardid;
    public tagChestsCost CostInfo;
    public tagChestsAllReward RewardInfo;
};
class tagChestMap
{
    public Dictionary<Byte, tagChestConfig> ChestMap = new Dictionary<Byte, tagChestConfig>();
};

//魅力的配置文件
class tagCharmOnce
{
    public UInt32 UserGlobel;
    public UInt32 UserMedal;
    public UInt32 UserCurrcey;
    public Byte Index;
    public int ChangeCharmValue;
    public UInt32 ItemID;
    public UInt32 ItemSum;
    public string PicName;
};
class tagCharmConfig
{
    public Byte CharmMaxItem;
    public Dictionary<Byte, tagCharmOnce> CharmIndexMap = new Dictionary<Byte, tagCharmOnce>();//0-7
};

//商店的配置
public class tagShopItemTimeConfig//商店的时间
{
    //public Byte BeginWeek;//开始的星期
    //public Byte BeginHour;//开始的小时
    //public Byte BeginMin;//开始的分钟
    //public UInt16 LastMin;//持续的分钟
    public DateTime BeginTime = new DateTime();
    public UInt16 LastHour;
};
public enum ShopItemType
{
    SIT_Normal = 0,//普通物品
    SIT_Entity = 1,//实体物品 
    SIT_PhonePay = 2,//手机话费
    SIT_MonthCard = 3,//月卡物品
    SIT_GlobelBag = 4,//聚宝盆
}
public class tagShopItemConfig//商店里的单个物品
{
    public Byte ItemInde;//物品的唯一索引
    public tagItemOnce ItemInfo = new tagItemOnce();//物品具体信息

    public UInt32 PriceGlobel; //价格金币
    public UInt32 PriceMabel;//价格奖牌
    public UInt32 PriceCurrey;//价格 乐币
    public ShopItemType ShopType;//是否为实体物品  true为实体 false为虚拟
    public bool IsCanPile;//是否可以堆叠
    public List<tagShopItemTimeConfig> TimeVec = new List<tagShopItemTimeConfig>();//物品的时间限制 无的话表示无限制

    public bool IsInTime()
    {
        if (TimeVec.Count == 0)
            return true;
        DateTime time = SystemTime.Instance.GetSystemDateTime;
        for (int i = 0; i < TimeVec.Count; ++i)
        {
            if (time < TimeVec[i].BeginTime)
                continue;
            TimeSpan Time = time - TimeVec[i].BeginTime;
            if (Time.TotalSeconds > TimeVec[i].LastHour * 3600)
                continue;
            return true;
        }
        return false;
    }
};
public class tagShopItemStr
{
    public Byte ItemIndex;
    public string ItemName;
    public string ItemIcon;
    public string ItemDec;
}
class tagShopConfig//单个商店所有物口集合
{
    public Byte ShopID;//商店ID
    public Dictionary<Byte, tagShopItemConfig> ShopItemMap = new Dictionary<Byte, tagShopItemConfig>();//商店的物品集合
    public Dictionary<Byte, tagShopItemStr> ShopItemStrMap = new Dictionary<Byte, tagShopItemStr>();
    //public List<tagShopItemConfig> ShopItemVec = new List<tagShopItemConfig>();
};
class tagShopConfigMap//全部商店的集合
{
    public Dictionary<Byte, tagShopConfig> ShopMap = new Dictionary<Byte, tagShopConfig>();
    // public List<tagShopConfig> ShopVec = new List<tagShopConfig>();
};
//活动
public class tagActionEventConfig
{
    public UInt32 ID;         //用于判断结点活动完成情况做与 1、2、4、8
    public UInt32 FinishValue;
    public UInt16 RewardID;
    public string Common;
}
class tagActionConfig
{
    public Byte ActionID;

    public Byte EventID;
    public UInt32 BindParam;
    public Dictionary<UInt32, tagActionEventConfig> EventMap = new Dictionary<UInt32, tagActionEventConfig>();

    public DateTime BeginTime;
    public UInt16 LastHour;//持续的小时

    //public string GroupName;
    //public string GroupButtonPic;
    //public string GroupHeadPic;
    //public string GroupCommon;
    public bool IsInTime()
    {
        if (LastHour == 0)
            return true;
        DateTime now = SystemTime.Instance.GetSystemDateTime;
        TimeSpan time = now - BeginTime;
        if (time.TotalSeconds < 0 || time.TotalSeconds > LastHour * 3600)
            return false;
        else
            return true;
    }
}
class tagActionMap
{
    public Byte m_MaxJoinActionSum;
    public Dictionary<Byte, tagActionConfig> m_ActionMap = new Dictionary<Byte, tagActionConfig>();
}
//赠送的处理
class tagGiffConfig
{
    public Byte LimitDay;
    public Byte SendSumLimitByDay;
    public Byte AcceptSubLimitByDay;
    public Byte SendOnceUserLimitByDay;
   // public UInt32 RewardGlobel;
    public UInt16 RewardID;
    public Byte MaxUserGiffSum;
}

//桌子的配置
class TableInfo
{
    public Byte TableTypeID;//座子的ID
    public UInt32 MinGlobelSum;
    public UInt32 MaxGlobelSum;
    public UInt32 MinCurreySum;
    public UInt32 MaxCurreySum;
    public UInt32 MinLevel;
    public UInt32 MaxLevel;
    public Byte MinRate;
    public Byte MaxRate;
    public Byte wMaxPlayerSum;//桌子最大人数
    public string cMapName;//地图名称
    public string TableName;
    public Dictionary<UInt32, UInt32> JoinItemMap = new Dictionary<UInt32, UInt32>();
}
//桌子
class tagTableConfig
{
    public Dictionary<Byte, TableInfo> m_TableConfig = new Dictionary<Byte, TableInfo>();
}
//物品转换
class tagItemConvert
{
    public UInt32 ItemID;
    public UInt32 Globel;
    public UInt32 Medal;
    public UInt32 Currey;
}
class tagItemConvertConfig
{
    public Dictionary<UInt32, tagItemConvert> m_ConvertMap = new Dictionary<UInt32, tagItemConvert>();
}

//战场排行
class tagMonthRank
{
    //战场排行信息
    public Byte Index;
    public UInt32 UserID;
    public UInt32 FaceID;
    public string NickName;
    public UInt32 Socres;
    public UInt32 VipSocres;
}
class tagMonthRankOnce
{
    public Byte MonthID;//比赛的ID
    public string FileName;//排行榜文件名称
    public Dictionary<Byte, tagMonthRank> m_RankInfo = new Dictionary<Byte, tagMonthRank>();
}
class tagMonthRankMap
{
    public Dictionary<Byte, tagMonthRankOnce> m_RankInfo = new Dictionary<Byte, tagMonthRankOnce>();
}

class tagMailConfig
{
    public Byte MailLimitDay;
    //public Byte MailMaxJoinSum;
    public Byte MaxUserMailSum;
    public Byte ClientShowMailSum;
}

//金币商店
//class tagGlobelShopItem
//{
//    public Byte ID;
//    public UInt32 LostCurrey;   //扣除钻石
//    public UInt32 AddGlobel;    //增加金币
//    public string ItemName;
//    public string ItemIcon;
//    public string ItemDec;
//};
//class tagGlobelShop
//{
//    public Dictionary<Byte, tagGlobelShopItem> m_GlobelShopMap = new Dictionary<Byte, tagGlobelShopItem>();
//};
//在线奖励
class tagOnceOnlienReward
{
    public Byte ID;
    public UInt16 OnlineMin;
    public UInt16 RewardID;
};
class tagOnlineReward
{
    public Dictionary<Byte, tagOnceOnlienReward> m_OnlineRewardMap = new Dictionary<Byte, tagOnceOnlienReward>();
};

//礼包
class tagFishPckageMap
{
    public Dictionary<UInt32, bool> m_PackageMap = new Dictionary<UInt32, bool>();//全部宝箱的集合
};

//奖励控制
public class tagRewardOnce
{
    public UInt16 RewardID;
    //public UInt32 RewardGlobel; //奖励普通金币
    //public UInt32 RewradMedal;
    //public UInt32 RewardCurrey;
    //public Byte RewardTitleID;
    //public UInt32 RewardAchievementPoint;
    //public UInt32 RewardMonthSocre;
    //public UInt32 RewardMonthGlobel;
    public List<tagItemOnce> RewardItemVec = new List<tagItemOnce>();
};

class tagFishSystem
{
    public UInt32 AchievementRankSum;
    public UInt32 AnnouncementSum;
    public UInt16 FirstBindPhoneRewardID;
    public UInt32 MaxGobelSum;
    public UInt32 ChangeNickNameNeedItemID;//修改名称物品
    public UInt32 ChangeNickNameNeedItemSum;
    public UInt32 ChangeGenderNeedItemID;//修改性别物品
    public UInt32 ChangeGenderNeedItemSum;

    public UInt32 SendMessageItemID;//
    public UInt16 SendMessageItemSum;
}

class tagRewardMap
{
    public Dictionary<UInt16, tagRewardOnce> RewardMap = new Dictionary<UInt16, tagRewardOnce>();
};

class FishUpdateInfo
{
    public Byte UpdateHour;
    public Byte UpdateMin;
}

public enum RechargeType
{
    RT_FirstCurrcey = 1,//首冲钻石
    RT_FirstGlobel = 2,//首冲金币
    RT_Currcey = 3,//充值钻石
    RT_Globel = 4,//充值金币
    RT_Reward = 5,//充值奖励ID
};
public class tagFishRechargeInfo
{
    public UInt32 ID;
    public UInt32 dDisCountPrice;//单位元
    public UInt32 dPreDisCountPrice;//折扣前的价格
    public UInt32 AddMoney;//添加乐币的数
    public UInt16 RewardID;
    public RechargeType RechargeType;
    //public bool IsCurreyOrGlobel;//添加的是乐币还是 金币
    public string Name;//商品名称
    public string Icon;//商品图标
    public string sDisCountPicName;//折扣图标
   // public bool IsFirstPay;

    public bool IsAddGlobel()
	{
        if (RechargeType == RechargeType.RT_FirstGlobel || RechargeType == RechargeType.RT_Globel)
			return true;
		else
			return false;
	}
    public bool IsAddCurrcey()
	{
        if (RechargeType == RechargeType.RT_FirstCurrcey || RechargeType == RechargeType.RT_Currcey)
			return true;
		else
			return false;
	}
    public bool IsAddReward()
	{
        if (RechargeType == RechargeType.RT_Reward)
			return true;
		else
			return false;
	}
    public bool IsFirstAdd()
	{
        if (RechargeType == RechargeType.RT_FirstCurrcey || RechargeType == RechargeType.RT_FirstGlobel)
			return true;
		else
			return false;
	}
};
//花费RMB 可以购买特殊的东西
class tagFishRechargesMap
{
    public Dictionary<UInt32, tagFishRechargeInfo> m_FishRechargeMap = new Dictionary<UInt32, tagFishRechargeInfo>();
};

class FishLauncherToShop//炮台转化到商店的功能
{
    public Byte LauncherType;
    public Byte ShopID;
    public Byte ShopOnlyID;
}
class FishSkillToShop//炮台转化到商店的功能
{
    public UInt16 ID;
    public Byte SkillType;
    public Byte RateType;
    public Byte ShopID;
    public Byte ShopOnlyID;

    public UInt16 GetID()
    {
        return Convert.ToUInt16((SkillType << 8) + RateType);
    }
}
class FishTableGroup//桌子转化的功能 
{
    public Byte GroupID;
    public List<Byte> TableList = new List<Byte>();

    public bool IsInGroup(Byte TableID)
    {
        for(int i =0;i<TableList.Count;++i)
        {
            if (TableList[i] == TableID)
                return true;
        }
        return false;
    }
}
class FishScriptMap
{
    public Dictionary<Byte, FishLauncherToShop> m_LauncherToShopMap = new Dictionary<Byte, FishLauncherToShop>();
    public Dictionary<Byte, FishTableGroup> m_TableGroup = new Dictionary<Byte, FishTableGroup>();
    public Dictionary<UInt16, FishSkillToShop> m_SkillToShopMap = new Dictionary<UInt16, FishSkillToShop>();

    public List<Byte> GetTableGroupList(Byte TableID)
    {
        foreach(FishTableGroup var in m_TableGroup.Values)
        {
            if(var.IsInGroup(TableID))
            {
                return var.TableList;
            }
        }
        return null;
    }
    public FishSkillToShop GetSkillToShopInfoByID(Byte SkillTypeID,Byte RateType)
    {
        UInt16 ID = Convert.ToUInt16((SkillTypeID << 8) + RateType);
        FishSkillToShop pInfo = null;
        if (m_SkillToShopMap.TryGetValue(ID, out pInfo))
            return pInfo;
        else
            return null;
    }
}
enum StringCheckType
{
    SCT_AccountName = 1,
    SCT_Password = 2,
    SCT_Normal = 3,
};

//等级奖励
class LevelRewardMap
{
    public Dictionary<UInt16, UInt16> m_LevelRewardMap = new Dictionary<UInt16, UInt16>();
};

class ErrorString
{
    public List<string> StrList = new List<string>();

    public bool CheckStringIsError(int MinLength,int MaxLength,string Str, StringCheckType Type)
    {
        if(Str== null)
        {
            return false;
        }
        if(MinLength !=0 && (Str.Length == 0 || Str.Trim().Length == 0 || Str == "" || Str==" "))
        {
            return false;
        }
        if (Str.Length < MinLength || Str.Length > MaxLength)
        {
            return false;
        }
        foreach(string var in StrList)
        {
            if (Str.IndexOf(var) != -1)
                return false;
        }
        if (Type == StringCheckType.SCT_Normal)
            return true;
        else if (Type == StringCheckType.SCT_AccountName
            || Type == StringCheckType.SCT_Password)
        {
            foreach (char ch in Str)
            {
                if (
                    ch == 95 ||
                    (ch >= 48 && ch <= 57) ||
                    (ch >= 65 && ch <= 90) ||
                    (ch >= 97 && ch <= 122)
                    )
                {
                    continue;
                }
                return false;
            }
            return true;
        }
        else
            return false;
    }
}

class tagExChange
{
    public Byte ID;
    public UInt32 TypeID;
    public UInt32 ChannelID;
    public UInt16 RewardID;
};
class tagExChangeMap
{
    public Dictionary<Byte, tagExChange> m_ExChangeMap = new Dictionary<Byte, tagExChange>();
};
class tagRoleProtect
{
    public Byte ProtectSum;
    public Byte ProtectCDMin;
    public UInt32 AddGlobel;
    public UInt32 MaxGlobelSum;
    public UInt32 OpenProtectGlobelSum;
    public Dictionary<Byte, Byte> TableMap = new Dictionary<Byte, Byte>();//允许的桌子的类型的集合
}

class tagLotteryReward
{
    public UInt32 Rate;
    public UInt16 RewardID;
};
class tagLotteryOnce
{
    public Byte LotteryID;
    public Byte UpperLotteryID;
    public UInt32 NeedUseScore;
    public List<tagLotteryReward> RewardVec = new List<tagLotteryReward>();
    public UInt32 TotalRate;
}
class tagLottery
{
    public Byte MaxLotteryFishSum = 0;
    public Dictionary<Byte, tagLotteryOnce> LotteryMap = new Dictionary<Byte, tagLotteryOnce>();
    public Dictionary<Byte, UInt32> FishScore = new Dictionary<Byte, UInt32>();

    public UInt32 GetFishLotteryScore(Byte FishTypeID)
    {
        UInt32 Score=0;
        if (!FishScore.TryGetValue(FishTypeID, out Score))
            return 0;
        else
            return Score;
    }
}
public class tagVipEffer
{

    public Byte     LauncherReBoundNum;//增加子弹反弹次数
    //public bool     IsCanLauncherLocking;//是否可以使用炮台锁定
    public Byte     AddAlmsSum;//增加救济金的次数
    public UInt32   AddAlmsRate;//增加救济金的百分比例
    public UInt32   AddMonthScoreRate;//增加比赛积分的百分比例
    public UInt32   AddReChargeRate;//充值提升的百分比
    public UInt32   AddCatchFishRate;//捕鱼概率提升
    public Byte     AddUseMedalSum;//兑换次数提升
    public Dictionary<Byte, Byte> CanUseLauncher = new Dictionary<Byte, Byte>();
}
public class tagVipOnce
{
    public Byte VipLevel;//VIP等级
    public Byte UpperVipLevel;//下一级的VIP等级索引
    public string VipTitle;//VIp的标题
    public string VipName;//名称
    public string VipDec;//描述
    public UInt32 NeedRechatgeRMBSum;
    public tagVipEffer CurrceyLevel= new tagVipEffer();//当前等级的VIP效果
    public tagVipEffer TotalLevel = new tagVipEffer();//合并总VIP的效果 
}
class tagRoleVipConfig
{
    public Byte DefaultLauncherReBoundNum;
    public Byte DefaultAlmsSum;
    public Byte DefaultUseMedalSum;//默认兑换次数
    public Dictionary<Byte, tagVipOnce> VipMap = new Dictionary<Byte, tagVipOnce>();//VIP的集合

    public Byte GetVipLevelBuLauncherType(Byte LauncherType)
    {
        foreach(tagVipOnce var in VipMap.Values)
        {
            if (var.CurrceyLevel.CanUseLauncher.ContainsKey(LauncherType))
                return var.VipLevel;
        }
        return 0;
    }
}

class tagMonthCard
{
    public Byte     MonthCardID;
    public UInt32   UseLastMin;
    public UInt16   OnceDayRewardID;
    public bool     IsCanAutoFire;//是否可以自动开火
    public UInt32   AddLotteryRate;//抽奖概率提升的百分比
}
class tagMonthCardConfig
{
    public Dictionary<Byte, tagMonthCard> MonthCardMap = new Dictionary<Byte, tagMonthCard>();
}

public enum BrandNiuNiuLevel
{
	BNNL_Non = 1,		//无牛
	BNNL_1 = 2,		    //牛一 
	BNNL_2 = 3,
	BNNL_3 = 4,
	BNNL_4 = 5,
	BNNL_5 = 6,
	BNNL_6 = 7,
	BNNL_7 = 8,
	BNNL_8 = 9,
	BNNL_9 = 10,		//牛9
	BNNL_NN = 11,		//牛牛
	BNNL_WHN = 12,		//五花牛
	BNNL_ZD = 13,		//炸弹
	BNNL_WXN = 14,		//五小牛
};
//class tagDialLevelInfo
//{
//    public Byte ID;
//    public UInt32 MinRate;
//    public UInt32 MaxRate;
//};
class tagCarValue
{
    public Byte ID;
    public Byte TypeID;
};
class tagCarType
{
    public Byte TypeID;//类型ID
    public Byte RateValue;//类型倍率
    public UInt32 RateF;//随机到的权重
};
class tagCarConfig
{
    public UInt32 BeginWriteSec;
    public UInt32 EndWriteSec;
    public UInt32 UpdateSec;
    public UInt32 InitGlobelSum;
    public UInt32 JoinGlobelSum;
    public UInt32 MaxAddGlobel;
    public Byte ChangeGlobelRate;//衰减值
    public UInt32 JoinBankerGlobelSum;
    public UInt32 GetNextBankerNeedGlobel;
    public UInt32 BankerGameSum;
    public UInt32 VipNonBetSum;
    public UInt32 MaxRate;
    public Dictionary<Byte, tagCarType> CarTypeMap = new Dictionary<Byte,tagCarType>();//类型的数据
    public List<tagCarValue> CarValueVec = new List<tagCarValue>();//轮盘数据

    public Byte GetResultID(Byte Result)
    {
        return 0;
    }
    public Byte GetResultRate(Byte Result)
    {
        return 0;
    }
};
class tagDialConfig
{
    public UInt32 BeginWriteSec;
    public UInt32 EndWriteSec;
    public UInt32 UpdateSec;
    public UInt32 InitGlobelSum;
    public UInt32 JoinGlobelSum;
    public UInt32 MaxAddGlobel;
    public Byte ChangeGlobelRate;//衰减值
    public UInt32 JoinBankerGlobelSum;
    public UInt32 GetNextBankerNeedGlobel;
    public UInt32 BankerGameSum;
    public UInt32 VipNonBetSum;
    public UInt32 MaxRate;
    public List<Byte> DialAreaData = new List<Byte>();//等级倍率
};
class tagNiuNiuConfig
{
    public UInt32 BeginWriteSec;
    public UInt32 EndWriteSec;
    public UInt32 UpdateSec;
    public UInt32 InitGlobelSum;
    public UInt32 JoinGlobelSum;
    public Byte MaxRate;
    public UInt32 MaxAddGlobel;
    public UInt32 JoinBankerGlobelSum;
    public UInt32 GetNextBankerNeedGlobel;
    public UInt32 BankerGameSum;
    public UInt32 VipNonBetSum;

    public Dictionary<Byte, Byte> NiuNiuLevelRate = new Dictionary<Byte, Byte>();//等级倍率

    public Byte GetRateByBrandValue(UInt16 Value)
	{
        Byte Level = Convert.ToByte(Value >> 8);
        Byte Rate = 1;
        if (!NiuNiuLevelRate.TryGetValue(Level,out Rate))
            return 1;
        else
            return Rate;
	}
    public Byte  GetBrandType(Byte BrandValue)
    {
        return Convert.ToByte((BrandValue % 4) + 1);
    }
    public Byte GetBrandValue(Byte BrandValue)
    {
        return Convert.ToByte((BrandValue / 4) + 1);
    }
};
class tagMiNiGameConfig
{
    public tagNiuNiuConfig niuniuConfig = new tagNiuNiuConfig();
    public tagDialConfig dialConfig = new tagDialConfig();
    public tagCarConfig carConfig = new tagCarConfig();
};

class tagShareConfig
{
    public Dictionary<string, bool> PackageMap = new Dictionary<string, bool>();
}
class FishConfig : Singleton<FishConfig>
{
    public RoleCheckConfig m_CheckInfo = new RoleCheckConfig();
    public tagTaskMap m_TaskInfo = new tagTaskMap();
    public tagMonthMap m_MatchInfo = new tagMonthMap();
    public tagAchievementMap m_AchievementInfo = new tagAchievementMap();
    public tagTitleMap m_TitleInfo = new tagTitleMap();
    public tagItemMap m_ItemInfo = new tagItemMap();
    public tagRankMap m_RankInfo = new tagRankMap();
    public RankXmlInfo m_RankXmlInfo = new RankXmlInfo();
    public tagChestMap m_ChestInfo = new tagChestMap();
    public tagCharmConfig m_CharmInfo = new tagCharmConfig();
    public tagShopConfigMap m_ShopInfo = new tagShopConfigMap();
    public tagActionMap m_ActionInfo = new tagActionMap();
    public tagGiffConfig m_GiffInfo = new tagGiffConfig();
    public tagTableConfig m_TableInfo = new tagTableConfig();
    public tagRoleRelation m_RelationInfo = new tagRoleRelation();
    public tagItemConvertConfig m_ItemConvertInfo = new tagItemConvertConfig();
    public tagMonthRankMap m_MonthRankInfo = new tagMonthRankMap();
    public tagMailConfig m_MailConfig = new tagMailConfig();
    //public tagGlobelShop m_GlobelShopConfig = new tagGlobelShop();
    public tagOnlineReward m_OnlineRewardConfig = new tagOnlineReward();
    public tagFishPckageMap m_FishPackageConfig = new tagFishPckageMap();
    public tagRewardMap m_RewardConfig = new tagRewardMap();
    public tagFishSystem m_SystemConfig = new tagFishSystem();
    public FishUpdateInfo m_FishUpdate = new FishUpdateInfo();
    public tagFishRechargesMap m_FishRecharge = new tagFishRechargesMap();
    public FishScriptMap m_FishScriptMap = new FishScriptMap();
    public ErrorString m_ErrorString = new ErrorString();
    public LevelRewardMap m_LevelRewardMap = new LevelRewardMap();
    public tagExChangeMap m_ExChangeMap = new tagExChangeMap();
    public tagRoleProtect m_RoleProtect = new tagRoleProtect();
    public tagLottery m_LotteryConfig = new tagLottery();
    public tagRoleVipConfig m_VipConfig = new tagRoleVipConfig();
    public tagMonthCardConfig m_MonthCardConfig = new tagMonthCardConfig();
    public tagMiNiGameConfig m_MiNiGameConfig = new tagMiNiGameConfig();
    public tagShareConfig m_ShareConfig = new tagShareConfig();

    public DateTime GetMinDateTime(DateTime pTime)
    {
        return new DateTime(pTime.Year, pTime.Month, pTime.Day, pTime.Hour, pTime.Minute, 0);
    }
    public tagRewardOnce GetRewardOnceByID(UInt16 RewardID)
    {
        tagRewardOnce pOnce = null;
        if (m_RewardConfig.RewardMap.TryGetValue(RewardID, out pOnce))
            return pOnce;
        else
            return null;
    }
    public System.Collections.IEnumerator LoadFishConfig(UnityEngine.Object obj, UnityEngine.Object objErrorString)
    {
        TextAsset ta = obj as TextAsset;
        TextReader text = new StringReader(ta.text);
        
        XmlReader pRead = XmlReader.Create(text);
        if (pRead == null)
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        XmlDocument doc = new XmlDocument();
        doc.Load(pRead);
        XmlElement pFishConfig = doc.DocumentElement;
        // XmlNode pFishConfig = doc.DocumentElement.FirstChild;

        if (!LoadRoleCheckConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleTaskConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleAchievementConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleMonstConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleTitleConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleItemConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleRankConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleChestConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleCharmConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleShopConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleGiffConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleTableConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleRelationConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadItemConvertConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleActionConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if(!LoadMailConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        //yield return new WaitForEndOfFrame();
        //if (!LoadGlobelShopConfig(pFishConfig))
        //{
        //    InitLogic.SetInitFailed();
        //    yield break;
        //}
        yield return new WaitForEndOfFrame();
        if (!LoadOnlineRewardConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadFishRewardConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadFishSystemConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadFishUpdateConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadFishRechargeConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
         yield return new WaitForEndOfFrame();
         if (!LoadFishScriptConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
         yield return new WaitForEndOfFrame();
         if (!LoadFishLevelReward(pFishConfig))
         {
             InitLogic.SetInitFailed();
             yield break;
         }
        yield return new WaitForEndOfFrame();
        if (!LoadFishExChangeConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadRoleProtectConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadFishLotteryConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadFishMonthCardConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadFishVipConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadMiNiGameConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }   
        yield return new WaitForEndOfFrame();
        if (!LoadShareConfig(pFishConfig))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        yield return new WaitForEndOfFrame();
        if (!LoadFishErrorStringFile(objErrorString))
        {
            InitLogic.SetInitFailed();
            yield break;
        }
        ResManager.Instance.UnloadObject(obj);
        InitLogic.EndInit();
    }
    public bool LoadRoleCheckConfig(XmlElement pXml)
    {
        XmlNode pFishCheck = pXml.SelectSingleNode("FishCheck");
        if (pFishCheck == null)
        {
            return false;
        }
        m_CheckInfo.CheckOtherUser = Convert.ToUInt32(pFishCheck.Attributes["CheckOtherUser"].Value);//补签的花费
        XmlNodeList pCheckDayList = pFishCheck.SelectNodes("CheckDay");
        foreach (XmlNode pCheckDay in pCheckDayList)
        {
            m_CheckInfo.MaxCheckDaySum = Convert.ToByte(pCheckDay.Attributes["MaxDaySum"].Value);
            XmlNodeList pDaySumList = pCheckDay.SelectNodes("DaySum");
            foreach (XmlNode pDaySum in pDaySumList)
            {
                Byte Day = Convert.ToByte(pDaySum.Attributes["Day"].Value);
                UInt16 RewardID = Convert.ToUInt16(pDaySum.Attributes["RewardID"].Value);
                m_CheckInfo.CheckDayReward.Add(Day, RewardID);
            }
        }
        XmlNodeList pMonthCheckList = pFishCheck.SelectNodes("MonthCheck");
        foreach (XmlNode pMonthCheck in pMonthCheckList)
        {
            XmlNodeList pDaySumList = pMonthCheck.SelectNodes("DaySum");
            foreach (XmlNode pDaySum in pDaySumList)
            {
                Byte Day = Convert.ToByte(pDaySum.Attributes["Day"].Value);
                UInt16 RewardID = Convert.ToUInt16(pDaySum.Attributes["RewardID"].Value);
                m_CheckInfo.MonthCheckReward.Add(Day, RewardID);
            }
        }
        return true;
    }
    public bool LoadRoleTaskConfig(XmlElement pXml)
    {
        //加载全部的任务数据
        XmlNode pFishTasks = pXml.SelectSingleNode("FishTasks");
        if (pFishTasks == null)
            return false;
        m_TaskInfo.m_MaxJoinTaskSum = Convert.ToByte(pFishTasks.Attributes["MaxJoinTaskSum"].Value);
        XmlNodeList pTaskList = pFishTasks.SelectNodes("Task");
        foreach (XmlNode pTaskNode in pTaskList)
        {
            tagTaskConfig pTask = new tagTaskConfig();
            tagTaskStrInfo pStr = new tagTaskStrInfo();
            pTask.TaskID = Convert.ToByte(pTaskNode.Attributes["ID"].Value);
            pTask.UpperTaskID = Convert.ToByte(pTaskNode.Attributes["UpperTaskID"].Value);
            pTask.IsDayClear = Convert.ToByte(pTaskNode.Attributes["IsDayClear"].Value) == 1 ? true : false;
            pTask.RewardID = Convert.ToUInt16(pTaskNode.Attributes["RewardID"].Value);

            pTask.GroupID = Convert.ToByte(pTaskNode.Attributes["GroupID"].Value);
            pTask.JoinLevel = Convert.ToByte(pTaskNode.Attributes["JoinLevel"].Value);
            pStr.TaskID = pTask.TaskID;
            pStr.TaskName = pTaskNode.Attributes["TaskName"].Value.ToString();
            pStr.TaskDec = pTaskNode.Attributes["TaskDec"].Value.ToString();
            pStr.TaskIcon = pTaskNode.Attributes["TaskIcon"].Value.ToString();

            XmlNodeList pEventList = pTaskNode.SelectNodes("Event");
            foreach (XmlNode pEvent in pEventList)
            {
                pTask.TaskEventInfo = new EventInfo();
                pTask.TaskEventInfo.OnlyID = Convert.ToByte(pEvent.Attributes["OnlyID"].Value);
                pTask.TaskEventInfo.EventID = Convert.ToByte(pEvent.Attributes["ID"].Value);
                pTask.TaskEventInfo.BindParam = Convert.ToUInt32(pEvent.Attributes["BindParam"].Value);
                pTask.TaskEventInfo.FinishValue = Convert.ToUInt32(pEvent.Attributes["FinishParam"].Value);
                break;
            }
            //一个任务数据加入完毕 放入集合
            m_TaskInfo.m_TaskStr.Add(pStr.TaskID, pStr);
            m_TaskInfo.m_TaskMap.Add(pTask.TaskID, pTask);
            m_TaskInfo.m_TaskVec.Add(pTask);
        }
        for (int x = 0; x < m_TaskInfo.m_TaskVec.Count; ++x)
        {
            if (m_TaskInfo.m_TaskVec[x].UpperTaskID != 0)
            {
                tagTaskConfig pConfig = null;
                if (m_TaskInfo.m_TaskMap.TryGetValue(m_TaskInfo.m_TaskVec[x].UpperTaskID, out pConfig))
                {
                    pConfig.LowerTaskVec.Add(m_TaskInfo.m_TaskVec[x].TaskID);
                }
            }
        }
        return true;
    }
    public bool LoadRoleAchievementConfig(XmlElement pXml)
    {
        //加载全部的任务数据
        XmlNode pFishAchievements = pXml.SelectSingleNode("FishAchievements");
        if (pFishAchievements == null)
            return false;
        m_AchievementInfo.m_MaxJoinAchievementSum = Convert.ToByte(pFishAchievements.Attributes["MaxJoinAchievementSum"].Value);
        XmlNodeList pAchievementList = pFishAchievements.SelectNodes("Achievement");
        foreach (XmlNode pAchievementNode in pAchievementList)
        {
            tagAchievementConfig pAchievement = new tagAchievementConfig();
            tagAchievementStrInfo pStr = new tagAchievementStrInfo();
            pAchievement.AchievementID = Convert.ToByte(pAchievementNode.Attributes["ID"].Value);
            pAchievement.UpperAchievementID = Convert.ToByte(pAchievementNode.Attributes["UpperFishAchievement"].Value);
            pAchievement.RewardID = Convert.ToUInt16(pAchievementNode.Attributes["RewardID"].Value);
            pAchievement.JoinLevel = Convert.ToByte(pAchievementNode.Attributes["JoinLevel"].Value);
            pStr.AchievementID = pAchievement.AchievementID;
            pStr.AchievementName = pAchievementNode.Attributes["AchievementName"].Value.ToString();
            pStr.AchievementIcon = pAchievementNode.Attributes["AchievementIcon"].Value.ToString();
            pStr.AchievementDec = pAchievementNode.Attributes["AchievementDec"].Value.ToString();
            pStr.AchievementStar = Convert.ToInt32(pAchievementNode.Attributes["AchievementStar"].Value);
            //Group
            XmlNodeList pEventList = pAchievementNode.SelectNodes("Event");
            foreach(XmlNode pEvent in pEventList)
            {
                pAchievement.AchievementEventInfo.OnlyID = Convert.ToByte(pEvent.Attributes["OnlyID"].Value);
                pAchievement.AchievementEventInfo.EventID = Convert.ToByte(pEvent.Attributes["ID"].Value);
                pAchievement.AchievementEventInfo.BindParam = Convert.ToUInt32(pEvent.Attributes["BindParam"].Value);
                pAchievement.AchievementEventInfo.FinishValue = Convert.ToUInt32(pEvent.Attributes["FinishParam"].Value);
                break;
            }

            //一个任务数据加入完毕 放入集合
            m_AchievementInfo.m_AchievementStr.Add(pStr.AchievementID, pStr);
            m_AchievementInfo.m_AchievementMap.Add(pAchievement.AchievementID, pAchievement);
            m_AchievementInfo.m_AchievementVec.Add(pAchievement);
        }
        //任务全部加载完毕后 进行子任务的设置
        for (int x = 0; x < m_AchievementInfo.m_AchievementVec.Count; ++x)
        {
            if (m_AchievementInfo.m_AchievementVec[x].UpperAchievementID != 0)
            {
                tagAchievementConfig pConfig = null;
                if (m_AchievementInfo.m_AchievementMap.TryGetValue(m_AchievementInfo.m_AchievementVec[x].UpperAchievementID, out pConfig))
                {
                    pConfig.LowerAchievementVec.Add(m_AchievementInfo.m_AchievementVec[x].AchievementID);
                }
            }
        }
        return true;
    }
    public bool LoadRoleMonstConfig(XmlElement pXml)
    {
        XmlNode pFishMonths = pXml.SelectSingleNode("FishMonths");
        if (pFishMonths == null)
            return false;
        m_MatchInfo.BeginMonthLevel = Convert.ToByte(pFishMonths.Attributes["BeginMonthLevel"].Value);
        XmlNodeList pMonthList = pFishMonths.SelectNodes("Month");
        foreach (XmlNode pMonthNode in pMonthList)
        {
            tagMonthConfig pMonth = new tagMonthConfig();
            tagMonthStr pStr = new tagMonthStr();

            pMonth.MonthID = Convert.ToByte(pMonthNode.Attributes["ID"].Value);
            pMonth.TypeID = Convert.ToByte(pMonthNode.Attributes["TypeID"].Value);
            pMonth.SignGlobel = Convert.ToUInt32(pMonthNode.Attributes["SignGlobel"].Value);
            pMonth.SignMadel = Convert.ToUInt32(pMonthNode.Attributes["SignMedal"].Value);
            pMonth.SignCurrey = Convert.ToUInt32(pMonthNode.Attributes["SignCurrey"].Value);
            pMonth.SignItem.ItemID = Convert.ToUInt32(pMonthNode.Attributes["SignItemID"].Value);
            pMonth.SignItem.ItemSum = Convert.ToUInt32(pMonthNode.Attributes["SignItemSum"].Value);
            pMonth.InitMonthGlobel = Convert.ToUInt32(pMonthNode.Attributes["InitMatchGlobel"].Value);
            pMonth.RankSum = Convert.ToByte(pMonthNode.Attributes["RankSum"].Value);
            pMonth.TableTypeID = Convert.ToByte(pMonthNode.Attributes["TableTypeID"].Value);
            pMonth.MinRateLimit = Convert.ToByte(pMonthNode.Attributes["MonthMinRateLimit"].Value);
            pStr.MonthID = pMonth.MonthID;
            pStr.MonthName = pMonthNode.Attributes["MonthName"].Value.ToString();
            pStr.MonthDec = pMonthNode.Attributes["MonthDec"].Value.ToString();
            pStr.MonthIcon = pMonthNode.Attributes["MonthIcon"].Value.ToString();

            XmlNode pConverts = pMonthNode.SelectSingleNode("Converts");
            XmlNodeList pConvertList = pConverts.SelectNodes("Convert");

            pMonth.MaxConvertSum = 0;
            foreach (XmlNode pConvertNode in pConvertList)
            {
                tagMonthConvert pConvert = new tagMonthConvert();
                Byte BeginSum = Convert.ToByte(pConvertNode.Attributes["BeginSum"].Value);
                Byte EndSum = Convert.ToByte(pConvertNode.Attributes["EndSum"].Value);

                pConvert.LostGlobel = Convert.ToUInt32(pConvertNode.Attributes["LostGlobel"].Value);
                pConvert.LostMedal = Convert.ToUInt32(pConvertNode.Attributes["LostMedal"].Value);
                pConvert.LostCurrey = Convert.ToUInt32(pConvertNode.Attributes["LostCurrey"].Value);

                pConvert.AddMonthGlobel = Convert.ToUInt32(pConvertNode.Attributes["AddMonthGlobel"].Value);

                if (pMonth.MaxConvertSum == 0 || pMonth.MaxConvertSum < EndSum)
                    pMonth.MaxConvertSum = EndSum;

                for (Byte x = BeginSum; x <= EndSum; ++x)
                {
                    pConvert.ConvertSum = x;
                    pMonth.m_ConvertMap.Add(x, pConvert);
                }
            }

            XmlNodeList pTimeList = pMonthNode.SelectNodes("Time");
            foreach(XmlNode pTimeNode in pTimeList)
            {
                tagMonthTime pTime = new tagMonthTime();
                pTime.ID = Convert.ToByte(pTimeNode.Attributes["ID"].Value);
                pTime.WriteLastMin = Convert.ToUInt16(pTimeNode.Attributes["WriteNextMin"].Value);
                pTime.BeginWeek = Convert.ToByte(pTimeNode.Attributes["BeginWeek"].Value);
                pTime.BeginHour = Convert.ToByte(pTimeNode.Attributes["BeginHour"].Value);
                pTime.BeginMin = Convert.ToByte(pTimeNode.Attributes["BeginMin"].Value);
                pTime.AllLastMin = Convert.ToUInt16(pTimeNode.Attributes["MonthLastMin"].Value);
                pTime.IsContinue = (Convert.ToByte(pTimeNode.Attributes["IsContinue"].Value) == 1 ? true:false);
                pTime.RunSum = Convert.ToUInt16(pTimeNode.Attributes["RunSum"].Value);
                
                XmlNode pSignTimeNode =  pTimeNode.SelectSingleNode("SignTime");
                pTime.SignWriteMin = Convert.ToUInt16(pSignTimeNode.Attributes["BeginWriteMin"].Value);
                pTime.SignLastMin = Convert.ToUInt16(pSignTimeNode.Attributes["LastMin"].Value);

                XmlNode pStartTime =  pTimeNode.SelectSingleNode("StartTime");
                pTime.StarWriteMin = Convert.ToUInt16(pStartTime.Attributes["BeginWriteMin"].Value);
                pTime.SratLastMin = Convert.ToUInt16(pStartTime.Attributes["LastMin"].Value);
                pMonth.m_MonthTime.Add(pTime.ID,pTime);
            }

            XmlNode pRewards = pMonthNode.SelectSingleNode("Rewards");
            XmlNodeList pRewardList = pRewards.SelectNodes("Reward");
            foreach(XmlNode pRewardNode in pRewards)
            {
                Byte BeginIndex = Convert.ToByte(pRewardNode.Attributes["BeginIndex"].Value);
                Byte EndIndex = Convert.ToByte(pRewardNode.Attributes["EndIndex"].Value);
                UInt16 RewardID = Convert.ToUInt16(pRewardNode.Attributes["RewardID"].Value);

                for (Byte x = BeginIndex; x <= EndIndex; ++x)
                {
                    tagMonthReward pReward = new tagMonthReward();
                    pReward.Index = x;
                    pReward.RewardID = RewardID;
                    pMonth.m_MonthReward.Add(x, pReward);
                }
            }

            //比赛渠道名称
            XmlNode pMonthName = pMonthNode.SelectSingleNode("MonthName");
            if(pMonthName != null)
            {
                XmlNodeList pNameList = pMonthName.SelectNodes("Name");
                foreach (XmlNode pName in pNameList)
                {
                    string ChannelLable = pName.Attributes["ChannelLabel"].Value;
                    string MonthChannelName = pName.Attributes["MonthName"].Value;

                    UInt32 ChannelID = 0;
                    if (ChannelLable.Length != 0)
                        ChannelID = Crc.Crc32(Encoding.Default.GetBytes(ChannelLable.ToCharArray()), 0, ChannelLable.Length);
                    if (ChannelID == 0)
                        continue;

                    pMonth.m_MonthChannelName.Add(ChannelID, MonthChannelName);
                }
            }
            else
            {
                pMonth.m_MonthChannelName.Clear();
            }


            m_MatchInfo.m_MonthMap.Add(pMonth.MonthID, pMonth);
            m_MatchInfo.m_MonthVec.Add(pMonth);
            m_MatchInfo.m_MonthStr.Add(pStr.MonthID, pStr);
        }
        return true;
    }
    public bool LoadRoleTitleConfig(XmlElement pXml)
    {
        XmlNode pFishTitles = pXml.SelectSingleNode("FishTitles");
        if (pFishTitles == null)
            return false;
        XmlNodeList pTitleList = pFishTitles.SelectNodes("Title");
        foreach (XmlNode pTitleNode in pTitleList)
        {
            tagTitleConfig pTitle = new tagTitleConfig();
            pTitle.TitleID = Convert.ToByte(pTitleNode.Attributes["ID"].Value);
            pTitle.TitleName = pTitleNode.Attributes["Name"].Value.ToString();
            m_TitleInfo.m_TileMap.Add(pTitle.TitleID, pTitle);
        }
        return true;
    }
    public bool LoadRoleItemConfig(XmlElement pXml)
    {
        XmlNode pFishItems = pXml.SelectSingleNode("FishItems");
        if (pFishItems == null)
            return false;
        XmlNodeList pFishItemList = pFishItems.SelectNodes("Item");
        foreach (XmlNode pFishItem in pFishItemList)
        {
            tagItemConfig pItem = new tagItemConfig();
            pItem.ItemID = Convert.ToUInt32(pFishItem.Attributes["ID"].Value);
            pItem.ItemName = pFishItem.Attributes["Name"].Value.ToString();
            pItem.ItemIcon = pFishItem.Attributes["ItemIcon"].Value.ToString();

            if (pFishItem.Attributes["ItemParam"] != null)
                pItem.ItemParam = Convert.ToUInt32(pFishItem.Attributes["ItemParam"].Value);
            else
                pItem.ItemParam = 0;

            pItem.ItemDec = pFishItem.Attributes["ItemDec"].Value.ToString();
            pItem.ItemTypeID = (EItemType)Convert.ToByte(pFishItem.Attributes["TypeID"].Value);
            m_ItemInfo.m_ItemMap.Add(pItem.ItemID, pItem);
        }
        return true;
    }
    public bool LoadRoleRankConfig(XmlElement pXml)
    {
        //加载配置的基本信息
        XmlNode pFishRanks = pXml.SelectSingleNode("FishRanks");
        if (pFishRanks == null)
            return false;
        m_RankInfo.RewardWeekDay = Convert.ToByte(pFishRanks.Attributes["ReawrdWeekDay"].Value);
        XmlNodeList pRankList = pFishRanks.SelectNodes("Rank");
        foreach(XmlNode pRankNode in pRankList)
        {
            tagRankConfig pRank = new tagRankConfig();
            pRank.RankID = Convert.ToByte(pRankNode.Attributes["ID"].Value);
            pRank.RowCount = Convert.ToByte(pRankNode.Attributes["MaxRowSum"].Value);
            //读取奖励
            XmlNodeList pRewardList = pRankNode.SelectNodes("Reward");
            foreach (XmlNode pRewardNode in pRewardList)
            {
                Byte BeginIndex = 0, EndIndex = 0;
                BeginIndex = Convert.ToByte(pRewardNode.Attributes["BeginIndex"].Value);
                EndIndex = Convert.ToByte(pRewardNode.Attributes["EndIndex"].Value);

                UInt16 RewardID = Convert.ToUInt16(pRewardNode.Attributes["RewardID"].Value);

                for (Byte x = BeginIndex; x <= EndIndex; ++x)
                {
                    tagRankReward pReward = new tagRankReward();
                    pReward.Index = x;
                    pReward.RewardID = RewardID;
                    pRank.RewardMap.Add(x, pReward);
                }
            }
            m_RankInfo.m_RankMap.Add(pRank.RankID, pRank);
        }
        return true;
    }
    public bool LoadRoleChestConfig(XmlElement pXml)
    {
        //读取宝箱的配置文件
        XmlNode pFishChests = pXml.SelectSingleNode("FishChests");
        if (pFishChests == null)
            return false;
        XmlNodeList pChestList = pFishChests.SelectNodes("Chest");
        foreach (XmlNode pChestNode in pChestList)
        {
            //处理一个宝箱
            tagChestConfig pChestConfig = new tagChestConfig();
            pChestConfig.ChestID = Convert.ToByte(pChestNode.Attributes["ID"].Value);
            pChestConfig.ExisteSec = Convert.ToByte(pChestNode.Attributes["ExistsSec"].Value);
            pChestConfig.ImmediateRewardid = Convert.ToUInt16(pChestNode.Attributes["ImmediateRewardid"].Value);

            tagChestsCost pCost = new tagChestsCost();
            XmlNode pCostsNode = pChestNode.SelectSingleNode("Costs");
            pCost.MaxCostNum = Convert.ToByte(pCostsNode.Attributes["MaxCostSum"].Value);
            XmlNodeList pCostList = pCostsNode.SelectNodes("Cost");
            foreach (XmlNode pCostNode in pCostList)
            {
                Byte Num = Convert.ToByte(pCostNode.Attributes["Num"].Value);
                UInt32 UserCurrcey = Convert.ToUInt32(pCostNode.Attributes["UserCurrcey"].Value);
                pCost.CostMap.Add(Num, UserCurrcey);
            }
            pChestConfig.CostInfo = pCost;

            //奖励
            XmlNode pRewards = pChestNode.SelectSingleNode("Rewards");
            tagChestsAllReward pAllReward = new tagChestsAllReward();//全部的奖励体系
            pAllReward.MaxRewardChances = 0;
            pAllReward.SpecialRewardUseRp = Convert.ToByte(pRewards.Attributes["SpecialRewardUseRp"].Value);
            pAllReward.SpecialRewardTypeID = Convert.ToByte(pRewards.Attributes["SpecialRewardTypeID"].Value);
            pAllReward.OtherAddRp = Convert.ToByte(pRewards.Attributes["OtherAddRp"].Value);
            XmlNodeList pRewardList = pRewards.SelectNodes("Reward");
            foreach (XmlNode pRewardNode in pRewardList)
            {
                tagChestsReward pReward = new tagChestsReward();
                pReward.RewardID = Convert.ToByte(pRewardNode.Attributes["ID"].Value);
                pReward.Chances = Convert.ToUInt32(pRewardNode.Attributes["Chances"].Value);
                pAllReward.MaxRewardChances += pReward.Chances;
                pReward.Chances = pAllReward.MaxRewardChances;
                pReward.MaxOnceChances = 0;

                XmlNodeList pOnceList = pRewardNode.SelectNodes("Once");
                foreach(XmlNode pOnceNode in pOnceList)
                {
                    tagChestsRewardOnce pOnce = new tagChestsRewardOnce();
                    pOnce.OnceID = Convert.ToByte(pOnceNode.Attributes["ID"].Value);
                    pOnce.Chances = Convert.ToUInt32(pOnceNode.Attributes["Chances"].Value);
                    pReward.MaxOnceChances += pOnce.Chances;
                    pOnce.Chances = pReward.MaxOnceChances;

                    pOnce.RewardID = Convert.ToUInt16(pOnceNode.Attributes["RewardID"].Value);

                    pReward.RewardLog.Add(pOnce.OnceID, pOnce);
                }
                pAllReward.RewardMap.Add(pReward.RewardID, pReward); 
            }
            pChestConfig.RewardInfo = pAllReward;

            m_ChestInfo.ChestMap.Add(pChestConfig.ChestID, pChestConfig);
        }
        return true;
    }
    public bool LoadRoleCharmConfig(XmlElement pXml)
    {
        XmlNode pFishCharms = pXml.SelectSingleNode("FishCharms");
        if (pFishCharms == null)
            return false;
        m_CharmInfo.CharmMaxItem = Convert.ToByte(pFishCharms.Attributes["MaxSum"].Value);
        XmlNodeList pCharmList = pFishCharms.SelectNodes("Charm");
        foreach (XmlNode pCharmNode in pCharmList)
        {
            tagCharmOnce pOnce = new tagCharmOnce();
            pOnce.Index = Convert.ToByte(pCharmNode.Attributes["Index"].Value);
            pOnce.UserGlobel = Convert.ToUInt32(pCharmNode.Attributes["LostGlobel"].Value);
            pOnce.UserMedal = Convert.ToUInt32(pCharmNode.Attributes["LostMedal"].Value);
            pOnce.UserCurrcey = Convert.ToUInt32(pCharmNode.Attributes["LostCurrcey"].Value);
            pOnce.ChangeCharmValue = Convert.ToInt32(pCharmNode.Attributes["ChangeCharmValue"].Value);
            pOnce.ItemID = Convert.ToUInt32(pCharmNode.Attributes["ItemID"].Value);
            pOnce.ItemSum = Convert.ToUInt32(pCharmNode.Attributes["ItemSum"].Value);
            pOnce.PicName = pCharmNode.Attributes["PicName"].Value.ToString();
            m_CharmInfo.CharmIndexMap.Add(pOnce.Index, pOnce);
        }
        return true;
    }
    public bool LoadRoleShopConfig(XmlElement pXml)
    {
        XmlNode pShops = pXml.SelectSingleNode("FishShops");
        if (pShops == null)
            return false;
        XmlNodeList pShopList = pShops.SelectNodes("Shop");
        foreach (XmlNode pShopNode in pShopList)
        {
            tagShopConfig pShopConfig = new tagShopConfig();
            pShopConfig.ShopID = Convert.ToByte(pShopNode.Attributes["ID"].Value);

            XmlNodeList pItemList = pShopNode.SelectNodes("Item");
            foreach(XmlNode pItemNode in pItemList)
            {
                tagShopItemConfig pItemConfig = new tagShopItemConfig();
                tagShopItemStr pStr = new tagShopItemStr();
                pItemConfig.ItemInde = Convert.ToByte(pItemNode.Attributes["ID"].Value);
                pItemConfig.ItemInfo.ItemID = Convert.ToUInt32(pItemNode.Attributes["ItemID"].Value);
                pItemConfig.ItemInfo.ItemSum = Convert.ToUInt32(pItemNode.Attributes["ItemSum"].Value);
                pItemConfig.ItemInfo.LastMin = Convert.ToUInt32(pItemNode.Attributes["LastMin"].Value);
                pItemConfig.PriceGlobel = Convert.ToUInt32(pItemNode.Attributes["PriceGlobel"].Value);
                pItemConfig.PriceMabel = Convert.ToUInt32(pItemNode.Attributes["PriceMabel"].Value);
                pItemConfig.PriceCurrey = Convert.ToUInt32(pItemNode.Attributes["PriceCurrey"].Value);
                if (pItemNode.Attributes["ShopType"] != null)
                    pItemConfig.ShopType = (ShopItemType)Convert.ToByte(pItemNode.Attributes["ShopType"].Value);
                else
                    pItemConfig.ShopType = ShopItemType.SIT_Normal;

                if (pItemNode.Attributes["IsCanPile"] != null)
                    pItemConfig.IsCanPile = Convert.ToByte(pItemNode.Attributes["IsCanPile"].Value)==1?true:false;
                else
                     pItemConfig.IsCanPile = true;    

                pStr.ItemIndex = pItemConfig.ItemInde;
                pStr.ItemName = pItemNode.Attributes["ItemName"].Value.ToString();
                pStr.ItemIcon = pItemNode.Attributes["ItemIcon"].Value.ToString();
                pStr.ItemDec = pItemNode.Attributes["ItemDec"].Value.ToString();

                XmlNodeList pTimeList = pItemNode.SelectNodes("Time");
                foreach (XmlNode pTimeNode in pTimeList)
                {
                    tagShopItemTimeConfig pTimeConfig = new tagShopItemTimeConfig();

                    pTimeConfig.BeginTime = new DateTime(
                        Convert.ToInt32(pTimeNode.Attributes["BeginYear"].Value),
                        Convert.ToInt32(pTimeNode.Attributes["BeginMonth"].Value),
                        Convert.ToInt32(pTimeNode.Attributes["BeginDay"].Value),
                        Convert.ToInt32(pTimeNode.Attributes["BeginHour"].Value),
                        Convert.ToInt32(pTimeNode.Attributes["BeginMin"].Value), 0
                       );
                    pTimeConfig.LastHour = Convert.ToUInt16(pTimeNode.Attributes["LastHour"].Value);
                    pItemConfig.TimeVec.Add(pTimeConfig);
                }
                pShopConfig.ShopItemMap.Add(pItemConfig.ItemInde, pItemConfig);
                pShopConfig.ShopItemStrMap.Add(pStr.ItemIndex, pStr);
            }
            m_ShopInfo.ShopMap.Add(pShopConfig.ShopID, pShopConfig);
        }
        return true;
    }
    public bool LoadRoleActionConfig(XmlElement pXml)
    {
        //加载全部的活动数据
        XmlNode pFishActions = pXml.SelectSingleNode("FishActions");
        if (pFishActions == null)
            return false;
        m_ActionInfo.m_MaxJoinActionSum = Convert.ToByte(pFishActions.Attributes["MaxJoinActionSum"].Value);
        XmlNodeList pActionList = pFishActions.SelectNodes("Action");
        foreach (XmlNode pActionNode in pActionList)
        {
            tagActionConfig pAction = new tagActionConfig();
            pAction.ActionID = Convert.ToByte(pActionNode.Attributes["ID"].Value);
            pAction.BeginTime = new DateTime(
                        Convert.ToInt32(pActionNode.Attributes["BeginYear"].Value),
                        Convert.ToInt32(pActionNode.Attributes["BeginMonth"].Value),
                        Convert.ToInt32(pActionNode.Attributes["BeginDay"].Value),
                         Convert.ToInt32(pActionNode.Attributes["BeginHour"].Value),
                        0, 0
                       );
            // Convert.ToInt32(pActionNode.Attributes["BeginMin"].Value)
            pAction.LastHour = Convert.ToUInt16(pActionNode.Attributes["LastHour"].Value);
           // pAction.GroupName = pActionNode.Attributes["GroupName"].Value.ToString();
           // pAction.GroupCommon = pActionNode.Attributes["GroupCommon"].Value.ToString();
            //pAction.GroupHeadPic = pActionNode.Attributes["GroupHeadPic"].Value.ToString();
            //pAction.GroupButtonPic = pActionNode.Attributes["GroupButtonPic"].Value.ToString();

            XmlNodeList pEventList = pActionNode.SelectNodes("Event");
            foreach (XmlNode pEventNode in pEventList)
            {
                pAction.EventID = Convert.ToByte(pEventNode.Attributes["ID"].Value);
                pAction.BindParam = Convert.ToUInt32(pEventNode.Attributes["BindParam"].Value);

                XmlNodeList pOnceList = pEventNode.SelectNodes("Once");
                foreach (XmlNode pOnceNode in pOnceList)
                {
                    tagActionEventConfig pOnce = new tagActionEventConfig();
                    pOnce.ID = Convert.ToUInt32(pOnceNode.Attributes["ID"].Value);
                    pOnce.FinishValue = Convert.ToUInt32(pOnceNode.Attributes["FinishParam"].Value);
                    pOnce.RewardID = Convert.ToUInt16(pOnceNode.Attributes["RewardID"].Value);
                    pOnce.Common = pOnceNode.Attributes["Common"].Value.ToString();

                    pAction.EventMap.Add(pOnce.ID, pOnce);
                }
                break;
            }
            m_ActionInfo.m_ActionMap.Add(pAction.ActionID, pAction);
        }
        return true;
    }
    //public bool SaveAccountInfo(string Account, string Password)
    //{
    //    byte[] FileData = System.IO.File.ReadAllBytes("AccountInfo.xml");
    //    XmlDocument xmlDoc = new XmlDocument();
    //    xmlDoc.LoadXml(Encoding.Default.GetString(FileData));

    //    XmlNode root = xmlDoc.SelectSingleNode("AccountInfo");
    //    XmlElement pAccount = (XmlElement)root;
    //    pAccount.SetAttribute("Account", Account);
    //    pAccount.SetAttribute("Password", Password);
    //    xmlDoc.Save("AccountInfo.xml");
    //    return true;
    //}
    //public bool GetAccountInfo(ref string Account, ref string Password)
    //{
    //    byte[] FileData = System.IO.File.ReadAllBytes("AccountInfo.xml");
    //    XmlDocument xmlDoc = new XmlDocument();
    //    xmlDoc.LoadXml(Encoding.Default.GetString(FileData));
    //    // XmlDocument xmlDoc = new XmlDocument();
    //    //xmlDoc.Load("AccountInfo.xml");
    //    XmlNode root = xmlDoc.SelectSingleNode("AccountInfo");
    //    XmlElement pAccount = (XmlElement)root;
    //    Account = Convert.ToString(pAccount.GetAttribute("Account"));
    //    Password = Convert.ToString(pAccount.GetAttribute("Password"));
    //    return true;
    //}
    public bool LoadRoleGiffConfig(XmlElement pXml)
    {
        XmlNode pFishGiff = pXml.SelectSingleNode("FishGiff");
        if (pFishGiff == null)
            return false;
        m_GiffInfo.LimitDay = Convert.ToByte(pFishGiff.Attributes["LimitDay"].Value);
        m_GiffInfo.SendSumLimitByDay = Convert.ToByte(pFishGiff.Attributes["SendSumByDay"].Value);
        m_GiffInfo.AcceptSubLimitByDay = Convert.ToByte(pFishGiff.Attributes["AcceptSumByDay"].Value);
        m_GiffInfo.SendOnceUserLimitByDay = Convert.ToByte(pFishGiff.Attributes["SendOnceUserLimitByDay"].Value);
        m_GiffInfo.MaxUserGiffSum = Convert.ToByte(pFishGiff.Attributes["MaxUserGiffSum"].Value);
        m_GiffInfo.RewardID = Convert.ToUInt16(pFishGiff.Attributes["RewardID"].Value);
        return true;
    }
    public bool LoadRoleTableConfig(XmlElement pXml)
    {
        XmlNode pFishTables = pXml.SelectSingleNode("FishTables");
        if (pFishTables == null)
            return false;
        XmlNodeList pTableList = pFishTables.SelectNodes("Table");
        foreach (XmlNode pTableNode in pTableList)
        {
            TableInfo pInfo = new TableInfo();
            pInfo.TableTypeID = Convert.ToByte(pTableNode.Attributes["ID"].Value);

            if (pTableNode.Attributes["MinGlobelSum"].Value == "-1")
                pInfo.MinGlobelSum = 0xffffffff;
            else 
                pInfo.MinGlobelSum = Convert.ToUInt32(pTableNode.Attributes["MinGlobelSum"].Value);

            if (pTableNode.Attributes["MinCurrey"].Value == "-1")
                pInfo.MinCurreySum = 0xffffffff;
            else
                pInfo.MinCurreySum = Convert.ToUInt32(pTableNode.Attributes["MinCurrey"].Value);

            if (pTableNode.Attributes["MaxCurrey"].Value == "-1")
                pInfo.MaxCurreySum = 0xffffffff;
            else
                pInfo.MaxCurreySum = Convert.ToUInt32(pTableNode.Attributes["MaxCurrey"].Value);

            if (pTableNode.Attributes["MaxGlobelSum"].Value == "-1")
                pInfo.MaxGlobelSum = 0xffffffff;
            else
                pInfo.MaxGlobelSum = Convert.ToUInt32(pTableNode.Attributes["MaxGlobelSum"].Value);

            if (pTableNode.Attributes["MinLevel"].Value == "-1")
                pInfo.MinLevel = 0xffffffff;
            else
                pInfo.MinLevel = Convert.ToUInt32(pTableNode.Attributes["MinLevel"].Value);

            if (pTableNode.Attributes["MaxLevel"].Value == "-1")
                pInfo.MaxLevel = 0xffffffff;
            else
                pInfo.MaxLevel = Convert.ToUInt32(pTableNode.Attributes["MaxLevel"].Value);

            pInfo.TableName = Convert.ToString(pTableNode.Attributes["TableName"].Value);
            pInfo.wMaxPlayerSum = Convert.ToByte(pTableNode.Attributes["MaxPlayerSum"].Value);
            pInfo.MinRate = Convert.ToByte(pTableNode.Attributes["MinRate"].Value);
            pInfo.MaxRate = Convert.ToByte(pTableNode.Attributes["MaxRate"].Value);
            pInfo.cMapName = Convert.ToString(pTableNode.Attributes["MapName"].Value);

            XmlNodeList pJoinItemList = pTableNode.SelectNodes("JoinItem");
            foreach (XmlNode pJoinItemNode in pJoinItemList)
            {
                UInt32 ItemID = Convert.ToUInt32(pJoinItemNode.Attributes["ItemID"].Value);
                UInt32 ItemSum = Convert.ToUInt32(pJoinItemNode.Attributes["ItemSum"].Value);
                pInfo.JoinItemMap.Add(ItemID, ItemSum);
            }
            m_TableInfo.m_TableConfig.Add(pInfo.TableTypeID, pInfo);
        }
        return true;
    }
    public bool LoadRoleRelationConfig(XmlElement pXml)
    {
        XmlNode pFishRelations = pXml.SelectSingleNode("FishRelations");
        if (pFishRelations == null)
            return false;
        m_RelationInfo.MaxRelationSum = Convert.ToByte(pFishRelations.Attributes["MaxSum"].Value);
        return true;
    }
    public bool LoadItemConvertConfig(XmlElement pXml)
    {
        XmlNode pFishConvertItems = pXml.SelectSingleNode("FishConvertItems");
        if (pFishConvertItems == null)
            return false;
        XmlNodeList pFishItemList = pFishConvertItems.SelectNodes("FishItem");
        foreach (XmlNode pFishItemNode in pFishItemList)
        {
            tagItemConvert pItem = new tagItemConvert();
            pItem.ItemID = Convert.ToUInt32(pFishItemNode.Attributes["ItemID"].Value);
            pItem.Globel = Convert.ToUInt32(pFishItemNode.Attributes["Globel"].Value);
            pItem.Medal = Convert.ToUInt32(pFishItemNode.Attributes["Medal"].Value);
            pItem.Currey = Convert.ToUInt32(pFishItemNode.Attributes["Currey"].Value);

            m_ItemConvertInfo.m_ConvertMap.Add(pItem.ItemID, pItem);
        }
        return true;
    }
    public bool LoadMailConfig(XmlElement pXml)
    {
        XmlNode pFishMail = pXml.SelectSingleNode("FishMail");
        if (pFishMail == null)
            return false;
        m_MailConfig.MailLimitDay = Convert.ToByte(pFishMail.Attributes["LimitDay"].Value);
        m_MailConfig.MaxUserMailSum = Convert.ToByte(pFishMail.Attributes["UserMaxMailSum"].Value);
        m_MailConfig.ClientShowMailSum = Convert.ToByte(pFishMail.Attributes["ClientShowMailSum"].Value);
        return true;
    }
    //public bool LoadGlobelShopConfig(XmlElement pXml)
    //{
    //    XmlNode pFishGlobelShop = pXml.SelectSingleNode("FishGlobelShop");
    //    if (pFishGlobelShop == null)
    //        return false;
    //    XmlNodeList pItemList = pFishGlobelShop.SelectNodes("Item");
    //    foreach (XmlNode pItemNode in pItemList)
    //    {
    //        tagGlobelShopItem pItem = new tagGlobelShopItem();

    //        pItem.ID = Convert.ToByte(pItemNode.Attributes["ID"].Value);
    //        pItem.LostCurrey = Convert.ToUInt32(pItemNode.Attributes["LoseCurrey"].Value);
    //        pItem.AddGlobel = Convert.ToUInt32(pItemNode.Attributes["GlobelSum"].Value);
    //        pItem.ItemName = pItemNode.Attributes["Name"].Value.ToString();
    //        pItem.ItemDec = pItemNode.Attributes["Dec"].Value.ToString();
    //        pItem.ItemIcon = pItemNode.Attributes["Icon"].Value.ToString();

    //        m_GlobelShopConfig.m_GlobelShopMap.Add(pItem.ID, pItem);
    //    }
    //    return true;
    //}
    public bool LoadOnlineRewardConfig(XmlElement pXml)
    {
        XmlNode pFishOnlineReward = pXml.SelectSingleNode("FishOnlineReward");
        if (pFishOnlineReward == null)
            return false;
        XmlNodeList pOnceList = pFishOnlineReward.SelectNodes("Once");
        foreach (XmlNode pOnceNode in pOnceList)
        {
            tagOnceOnlienReward pOnce = new tagOnceOnlienReward();

            pOnce.ID = Convert.ToByte(pOnceNode.Attributes["ID"].Value);
            if (pOnce.ID < 1 || pOnce.ID > 32)
                return false;
            pOnce.OnlineMin = Convert.ToUInt16(pOnceNode.Attributes["OnlineMin"].Value);
            pOnce.RewardID = Convert.ToUInt16(pOnceNode.Attributes["RewardID"].Value);
            m_OnlineRewardConfig.m_OnlineRewardMap.Add(pOnce.ID, pOnce);
        }
        if (m_OnlineRewardConfig.m_OnlineRewardMap.Count > 32)
            return false;
        return true;
    }
    public bool LoadFishPackageConfig(XmlElement pXml)
    {
        XmlNode pFishPackages = pXml.SelectSingleNode("FishPackages");
        if (pFishPackages == null)
            return false;
        XmlNodeList pFishPackageList = pFishPackages.SelectNodes("FishPackage");
        foreach (XmlNode pFishPackageNode in pFishPackageList)
        {
            UInt32 ItemID = Convert.ToUInt32(pFishPackageNode.Attributes["ItemID"].Value);
            m_FishPackageConfig.m_PackageMap.Add(ItemID, true);
        }
        return true;
    }
    public bool LoadFishRewardConfig(XmlElement pXml)
    {
        XmlNode pFishRewardInfo = pXml.SelectSingleNode("FishRewardInfo");
        if (pFishRewardInfo == null)
            return false;
        XmlNodeList pRewardList = pFishRewardInfo.SelectNodes("Reward");
        foreach (XmlNode pReward in pRewardList)
        {
            tagRewardOnce pOnce = new tagRewardOnce();
            pOnce.RewardID = Convert.ToUInt16(pReward.Attributes["RewardID"].Value);

            XmlNodeList pItemList = pReward.SelectNodes("Item");
            foreach (XmlNode pItemNode in pItemList)
            {
                tagItemOnce pItem = new tagItemOnce();
                pItem.ItemID = Convert.ToUInt32(pItemNode.Attributes["ItemID"].Value);
                pItem.ItemSum = Convert.ToUInt32(pItemNode.Attributes["ItemSum"].Value);
                pItem.LastMin = Convert.ToUInt32(pItemNode.Attributes["LastMin"].Value);
                pOnce.RewardItemVec.Add(pItem);
            }
            if (m_RewardConfig.RewardMap.ContainsKey(pOnce.RewardID))
            {
                return false;
            }
            m_RewardConfig.RewardMap.Add(pOnce.RewardID, pOnce);
        }
        return true;
    }
    public bool LoadFishSystemConfig(XmlElement pXml)
    {
        XmlNode pFishSystem  = pXml.SelectSingleNode("FishSystem");
        if (pFishSystem == null)
            return false;
        tagFishSystem pSystem = new tagFishSystem();
        pSystem.AchievementRankSum = Convert.ToUInt32(pFishSystem.Attributes["MaxAchievementRankSum"].Value);
        pSystem.AnnouncementSum = Convert.ToUInt32(pFishSystem.Attributes["AnnouncementSum"].Value);
        pSystem.FirstBindPhoneRewardID = Convert.ToUInt16(pFishSystem.Attributes["FirstBindPhoneRewardID"].Value);
        if (pFishSystem.Attributes["MaxGobelSum"] != null)
        {
            pSystem.MaxGobelSum = Convert.ToUInt32(pFishSystem.Attributes["MaxGobelSum"].Value);
        }
        else
        {
            pSystem.MaxGobelSum = 4200000000;
        }
        pSystem.ChangeNickNameNeedItemID = Convert.ToUInt32(pFishSystem.Attributes["ChangeNickNameNeedItemID"].Value);
        pSystem.ChangeNickNameNeedItemSum = Convert.ToUInt32(pFishSystem.Attributes["ChangeNickNameNeedItemSum"].Value);
        pSystem.ChangeGenderNeedItemID = Convert.ToUInt32(pFishSystem.Attributes["ChangeGenderNeedItemID"].Value);
        pSystem.ChangeGenderNeedItemSum = Convert.ToUInt32(pFishSystem.Attributes["ChangeGenderNeedItemSum"].Value);
        pSystem.SendMessageItemID = Convert.ToUInt32(pFishSystem.Attributes["SendMessageItemID"].Value);
        pSystem.SendMessageItemSum = Convert.ToUInt16(pFishSystem.Attributes["SendMessageItemSum"].Value);
        m_SystemConfig = pSystem;
        return true;
    }
    public bool LoadFishUpdateConfig(XmlElement pXml)
    {
        XmlNode pFishSystem = pXml.SelectSingleNode("FishServerUpdateTime");
        if (pFishSystem == null)
            return false;
        m_FishUpdate.UpdateHour = Convert.ToByte(pFishSystem.Attributes["UpdateHour"].Value);
        m_FishUpdate.UpdateMin = Convert.ToByte(pFishSystem.Attributes["UpdateMin"].Value);
        return true;
    }
    public bool LoadFishRechargeConfig(XmlElement pXml)
    {
        XmlNode pFishRecharges = pXml.SelectSingleNode("FishRecharges");
        if (pFishRecharges == null)
            return false;
        XmlNodeList pList = pFishRecharges.SelectNodes("FishRecharge");
        foreach(XmlNode var in pList)
        {
            tagFishRechargeInfo pInfo = new tagFishRechargeInfo();

            pInfo.ID = Convert.ToUInt32(var.Attributes["ID"].Value);
            pInfo.dDisCountPrice = Convert.ToUInt32(var.Attributes["DisCountPrice"].Value);
            pInfo.dPreDisCountPrice = Convert.ToUInt32(var.Attributes["PreDisCountPrice"].Value);
            pInfo.AddMoney = Convert.ToUInt32(var.Attributes["AddMoney"].Value);
            pInfo.RechargeType = (RechargeType)Convert.ToByte(var.Attributes["RechargeType"].Value);
            pInfo.RewardID = Convert.ToUInt16(var.Attributes["AddRewardID"].Value);
            //pInfo.IsCurreyOrGlobel = (Convert.ToByte(var.Attributes["IsCurreyOrGlobel"].Value)==1?true:false);
            pInfo.Icon = Convert.ToString(var.Attributes["Icon"].Value);
            pInfo.Name = Convert.ToString(var.Attributes["Name"].Value);
            pInfo.sDisCountPicName = Convert.ToString(var.Attributes["DisCountPicName"].Value);
            //pInfo.IsFirstPay = (Convert.ToByte(var.Attributes["IsFirstPay"].Value) == 1 ? true : false);
            m_FishRecharge.m_FishRechargeMap.Add(pInfo.ID, pInfo);
        }
        return true;
    }
    public UInt32 GetWriteSec()
    {
        return Convert.ToUInt32(m_FishUpdate.UpdateHour * 3600 + m_FishUpdate.UpdateMin * 60);
    }

    public bool LoadFishScriptConfig(XmlElement pXml)
    {
        XmlNode pFishScript = pXml.SelectSingleNode("FishScript");
        if (pFishScript == null)
            return false;

        XmlNodeList pFishLauncherToShopList =  pFishScript.SelectNodes("LauncherToShop");
        foreach(XmlNode var in pFishLauncherToShopList)
        {
            FishLauncherToShop pInfo = new FishLauncherToShop();
            pInfo.LauncherType = Convert.ToByte(var.Attributes["LauncherType"].Value);
            pInfo.ShopID = Convert.ToByte(var.Attributes["ShopID"].Value);
            pInfo.ShopOnlyID = Convert.ToByte(var.Attributes["ShopOnlyID"].Value);

            m_FishScriptMap.m_LauncherToShopMap.Add(pInfo.LauncherType, pInfo);
        }

        XmlNodeList pTableGroupList =  pFishScript.SelectNodes("TableConvert");
        foreach(XmlNode var in pTableGroupList)
        {
            FishTableGroup pInfo = new FishTableGroup();
            pInfo.GroupID = Convert.ToByte(var.Attributes["GroupID"].Value);

            XmlNodeList pTableIDList = var.SelectNodes("Table");
            foreach(XmlNode var2 in pTableIDList)
            {
                Byte TableID = Convert.ToByte(var2.Attributes["TableID"].Value);
                pInfo.TableList.Add(TableID);
            }
            m_FishScriptMap.m_TableGroup.Add(pInfo.GroupID, pInfo);
        }

        XmlNodeList pFishSkillToShopList =  pFishScript.SelectNodes("SkillToShop");
        foreach(XmlNode var in pFishSkillToShopList)
        {
            FishSkillToShop pInfo = new FishSkillToShop();
            pInfo.SkillType = Convert.ToByte(var.Attributes["SkillTypeID"].Value);
            pInfo.RateType = Convert.ToByte(var.Attributes["RateType"].Value);
            pInfo.ID = pInfo.GetID();
            pInfo.ShopID = Convert.ToByte(var.Attributes["ShopID"].Value);
            pInfo.ShopOnlyID = Convert.ToByte(var.Attributes["ShopOnlyID"].Value);

            m_FishScriptMap.m_SkillToShopMap.Add(pInfo.ID, pInfo);
        }

        return true;
    }
    public bool LoadFishErrorStringFile(UnityEngine.Object obj)
    {
        TextAsset ta = obj as TextAsset;
        TextReader text = new StringReader(ta.text);
        m_ErrorString.StrList.Clear();
        while(true)
        {
            string Line = text.ReadLine();
            if (Line == null)
                break;
            m_ErrorString.StrList.Add(Line);
        }
        return true;
    }
    public bool LoadFishLevelReward(XmlElement pXml)
    {
        XmlNode pFishLevelReward = pXml.SelectSingleNode("FishLevelReward");
        if (pFishLevelReward == null)
            return false;

        XmlNodeList pLevelRewardList = pFishLevelReward.SelectNodes("LevelReward");
        foreach (XmlNode var in pLevelRewardList)
        {
            UInt16 Level = Convert.ToUInt16(var.Attributes["Level"].Value);
            UInt16 RewardID = Convert.ToUInt16(var.Attributes["RewardID"].Value);

            m_LevelRewardMap.m_LevelRewardMap.Add(Level, RewardID);
        }
        return true;
    }
    public bool LoadFishExChangeConfig(XmlElement pXml)
    {
        //兑换码
        XmlNode pFishExChanges = pXml.SelectSingleNode("FishExChanges");
        if (pFishExChanges == null)
            return false;

        XmlNodeList pExChangeList = pFishExChanges.SelectNodes("ExChange");
        foreach (XmlNode var in pExChangeList)
        {
            tagExChange pOnce = new tagExChange();

            pOnce.ID = Convert.ToByte(var.Attributes["ID"].Value);
            pOnce.TypeID = Convert.ToUInt32(var.Attributes["TypeID"].Value);
            pOnce.RewardID = Convert.ToUInt16(var.Attributes["RewardID"].Value);

            string ChannelName = var.Attributes["ChannelID"].Value;
            ChannelName = ChannelName.ToLower().Trim();
            if (ChannelName.Length == 0)
                pOnce.ChannelID = 0;
            else
                pOnce.ChannelID = Crc.Crc32(Encoding.Default.GetBytes(ChannelName.ToCharArray()), 0, ChannelName.Length);

            m_ExChangeMap.m_ExChangeMap.Add(pOnce.ID, pOnce);
        }
        return true;
    }
    public bool LoadRoleProtectConfig(XmlElement pXml)
    {
        XmlNode pFishRoleProtect = pXml.SelectSingleNode("FishRoleProtect");
        if (pFishRoleProtect == null)
            return false;

        m_RoleProtect.ProtectSum = Convert.ToByte(pFishRoleProtect.Attributes["ProtectSum"].Value);
        m_RoleProtect.ProtectCDMin = Convert.ToByte(pFishRoleProtect.Attributes["ProtectTimeMin"].Value);
        m_RoleProtect.AddGlobel = Convert.ToUInt32(pFishRoleProtect.Attributes["AddGlobel"].Value);
        m_RoleProtect.MaxGlobelSum = Convert.ToUInt32(pFishRoleProtect.Attributes["MaxGlobelSum"].Value);
        m_RoleProtect.OpenProtectGlobelSum = Convert.ToUInt32(pFishRoleProtect.Attributes["OpenProtectGlobelSum"].Value);

        XmlNodeList pExChangeList = pFishRoleProtect.SelectNodes("Table");
        foreach (XmlNode var in pExChangeList)
        {

            Byte ID = Convert.ToByte(var.Attributes["ID"].Value);
            m_RoleProtect.TableMap.Add(ID, 0);
        }
        return true;
    }
    public bool LoadFishLotteryConfig(XmlElement pXml)
    {
        XmlNode pFishLottery = pXml.SelectSingleNode("FishLottery");
        if (pFishLottery == null)
            return false;
        m_LotteryConfig.MaxLotteryFishSum = Convert.ToByte(pFishLottery.Attributes["MaxLotteryFishSum"].Value);
        //
        XmlNode pLotterys = pFishLottery.SelectSingleNode("Lotterys");
        if (pLotterys == null)
            return false;

        XmlNodeList pLotteryList = pLotterys.SelectNodes("Lottery");
        m_LotteryConfig.LotteryMap.Clear();
        foreach (XmlNode var in pLotteryList)
        {
            tagLotteryOnce pOnce = new tagLotteryOnce();
            pOnce.LotteryID = Convert.ToByte(var.Attributes["ID"].Value);
            pOnce.UpperLotteryID = Convert.ToByte(var.Attributes["UpperID"].Value); 
            pOnce.NeedUseScore = Convert.ToUInt32(var.Attributes["OpenNeedScore"].Value);

            XmlNodeList pRewardList = var.SelectNodes("Reward");
            pOnce.TotalRate = 0;
            foreach (XmlNode var2 in pRewardList)
            {
                tagLotteryReward pRewardOnce = new tagLotteryReward();
                pRewardOnce.RewardID = Convert.ToUInt16(var2.Attributes["RewardID"].Value);
                pOnce.TotalRate = Convert.ToUInt32(var2.Attributes["RewardRate"].Value);
                pRewardOnce.Rate = pOnce.TotalRate;

                pOnce.RewardVec.Add(pRewardOnce);
            }
            m_LotteryConfig.LotteryMap.Add(pOnce.LotteryID, pOnce);
        }

        XmlNode pFishScore = pFishLottery.SelectSingleNode("FishScore");
        if (pFishScore == null)
            return false;
        XmlNodeList pFish = pFishScore.SelectNodes("Fish");
        m_LotteryConfig.FishScore.Clear();
        foreach (XmlNode var in pFish)
        {
            Byte FishID = Convert.ToByte(var.Attributes["TypeID"].Value);
            UInt32 Score = Convert.ToUInt32(var.Attributes["Score"].Value);
            m_LotteryConfig.FishScore.Add(FishID, Score);
        }
        return true;
    }
    public bool LoadFishMonthCardConfig(XmlElement pXml)
    {
        XmlNode pFishMonthCards = pXml.SelectSingleNode("FishMonthCards");
        if (pFishMonthCards == null)
            return false;

        XmlNodeList pMonthCardList = pFishMonthCards.SelectNodes("MonthCard");
        foreach(XmlNode var in pMonthCardList)
        {
            tagMonthCard pOnce = new tagMonthCard();

            pOnce.MonthCardID = Convert.ToByte(var.Attributes["ID"].Value);
            pOnce.UseLastMin = Convert.ToUInt32(var.Attributes["CardLastMin"].Value);
            pOnce.OnceDayRewardID = Convert.ToUInt16(var.Attributes["OnceDayRewardID"].Value);
            pOnce.IsCanAutoFire = Convert.ToByte(var.Attributes["IsCanAutoFire"].Value) == 1 ? true : false;
            pOnce.AddLotteryRate = Convert.ToUInt32(var.Attributes["AddLotteryRate"].Value);

            m_MonthCardConfig.MonthCardMap.Add(pOnce.MonthCardID, pOnce);
        }

        return true;
    }
    public bool LoadFishVipConfig(XmlElement pXml)
    {
        XmlNode pFishVips = pXml.SelectSingleNode("FishVips");
        if (pFishVips == null)
            return false;

        m_VipConfig.DefaultLauncherReBoundNum = Convert.ToByte(pFishVips.Attributes["DefaultLauncherReBoundNum"].Value);
        m_VipConfig.DefaultAlmsSum = Convert.ToByte(pFishVips.Attributes["DefaultAlmsSum"].Value);
        m_VipConfig.DefaultUseMedalSum = Convert.ToByte(pFishVips.Attributes["DefaultUseMedalSum"].Value);

        XmlNodeList pVipList = pFishVips.SelectNodes("Vip");
        tagVipEffer pTotalEffer = new tagVipEffer();//保存总的数据
        foreach (XmlNode var in pVipList)
        {
            tagVipOnce pOnce = new tagVipOnce();
            //读取VIP数据
            pOnce.VipLevel = Convert.ToByte(var.Attributes["VipLevel"].Value);
            pOnce.UpperVipLevel = Convert.ToByte(var.Attributes["UpperVipLevel"].Value);
            pOnce.VipName = var.Attributes["VipName"].Value;
            pOnce.VipTitle = var.Attributes["VipTitle"].Value;
            pOnce.VipDec = var.Attributes["VipDec"].Value;
            pOnce.NeedRechatgeRMBSum = Convert.ToUInt32(var.Attributes["RechargeRMBSum"].Value);
            //当前等级的VIP数据
            pOnce.CurrceyLevel.LauncherReBoundNum = Convert.ToByte(var.Attributes["LauncherReBoundNum"].Value);
           // pOnce.CurrceyLevel.IsCanLauncherLocking = Convert.ToByte(var.Attributes["IsCanLauncherLocking"].Value) == 1 ? true : false;
            pOnce.CurrceyLevel.AddAlmsSum = Convert.ToByte(var.Attributes["AddAlmsSum"].Value);
            pOnce.CurrceyLevel.AddAlmsRate = Convert.ToUInt32(var.Attributes["AddAlmsRate"].Value);
            pOnce.CurrceyLevel.AddMonthScoreRate = Convert.ToUInt32(var.Attributes["AddMonthScoreRate"].Value);
            pOnce.CurrceyLevel.AddReChargeRate = Convert.ToUInt32(var.Attributes["AddReChargeRate"].Value);
            pOnce.CurrceyLevel.AddCatchFishRate = Convert.ToUInt32(var.Attributes["AddCatchFishRate"].Value);
            pOnce.CurrceyLevel.AddUseMedalSum = Convert.ToByte(var.Attributes["AddUseMedalSum"].Value);
            pOnce.CurrceyLevel.CanUseLauncher.Add(Convert.ToByte(var.Attributes["AddLauncherID"].Value), 1);

            //合并等级的VIP数据
            pTotalEffer.LauncherReBoundNum += pOnce.CurrceyLevel.LauncherReBoundNum;
            //pTotalEffer.IsCanLauncherLocking = (pTotalEffer.IsCanLauncherLocking || pOnce.CurrceyLevel.IsCanLauncherLocking);
            pTotalEffer.AddAlmsSum += pOnce.CurrceyLevel.AddAlmsSum;
            pTotalEffer.AddAlmsRate += pOnce.CurrceyLevel.AddAlmsRate;
            pTotalEffer.AddMonthScoreRate += pOnce.CurrceyLevel.AddMonthScoreRate;
            pTotalEffer.AddReChargeRate += pOnce.CurrceyLevel.AddReChargeRate;
            pTotalEffer.AddUseMedalSum += pOnce.CurrceyLevel.AddUseMedalSum;
            pTotalEffer.AddCatchFishRate += pOnce.CurrceyLevel.AddCatchFishRate;
            foreach (Byte LauncherType in pOnce.CurrceyLevel.CanUseLauncher.Keys)
                pTotalEffer.CanUseLauncher.Add(LauncherType, 1);

            pOnce.TotalLevel.LauncherReBoundNum = pTotalEffer.LauncherReBoundNum;
           // pOnce.TotalLevel.IsCanLauncherLocking = pTotalEffer.IsCanLauncherLocking;
            pOnce.TotalLevel.AddAlmsSum = pTotalEffer.AddAlmsSum;
            pOnce.TotalLevel.AddAlmsRate = pTotalEffer.AddAlmsRate;
            pOnce.TotalLevel.AddMonthScoreRate = pTotalEffer.AddMonthScoreRate;
            pOnce.TotalLevel.AddReChargeRate = pTotalEffer.AddReChargeRate;
            pOnce.TotalLevel.AddUseMedalSum = pTotalEffer.AddUseMedalSum;
            pOnce.TotalLevel.AddCatchFishRate = pTotalEffer.AddCatchFishRate;
            foreach (Byte LauncherType in pTotalEffer.CanUseLauncher.Keys)
                pOnce.TotalLevel.CanUseLauncher.Add(LauncherType, 1);

            m_VipConfig.VipMap.Add(pOnce.VipLevel, pOnce);
        }
        return true;
    }
    public bool LoadShareConfig(XmlElement pXml)
    {
        XmlNode pFishShare = pXml.SelectSingleNode("FishShare");
        if (pFishShare == null)
            return false;
        m_ShareConfig.PackageMap.Clear();
        XmlNodeList pShareOnce = pFishShare.SelectNodes("ShareOnce");
        foreach (XmlNode var in pShareOnce)
        {
            string PackageName = var.Attributes["PackageName"].Value;
            if (!m_ShareConfig.PackageMap.ContainsKey(PackageName))
                m_ShareConfig.PackageMap.Add(PackageName, true);
        }
        return true;
    }
    public bool LoadMiNiGameConfig(XmlElement pXml)
    {
        XmlNode pFishMiniGame = pXml.SelectSingleNode("FishMiniGame");
        if (pFishMiniGame == null)
            return false;

        //niuniu
        XmlNode pFishNiuNiu = pFishMiniGame.SelectSingleNode("niuniu");
        if (pFishNiuNiu == null)
            return false;
        m_MiNiGameConfig.niuniuConfig.BeginWriteSec = Convert.ToUInt32(pFishNiuNiu.Attributes["BeginWriteSec"].Value);
        m_MiNiGameConfig.niuniuConfig.EndWriteSec = Convert.ToUInt32(pFishNiuNiu.Attributes["EndWriteSec"].Value);
        m_MiNiGameConfig.niuniuConfig.InitGlobelSum = Convert.ToUInt32(pFishNiuNiu.Attributes["InitTableGlobel"].Value);
        m_MiNiGameConfig.niuniuConfig.JoinGlobelSum = Convert.ToUInt32(pFishNiuNiu.Attributes["JoinRoomGlobel"].Value);
        m_MiNiGameConfig.niuniuConfig.UpdateSec = Convert.ToUInt32(pFishNiuNiu.Attributes["TableUpdateSec"].Value);
        m_MiNiGameConfig.niuniuConfig.MaxAddGlobel = Convert.ToUInt32(pFishNiuNiu.Attributes["MaxAddGlobel"].Value);
        m_MiNiGameConfig.niuniuConfig.JoinBankerGlobelSum = Convert.ToUInt32(pFishNiuNiu.Attributes["JoinBankerGlobelSum"].Value);
        m_MiNiGameConfig.niuniuConfig.GetNextBankerNeedGlobel = Convert.ToUInt32(pFishNiuNiu.Attributes["GetNextBankerNeedGlobel"].Value);
        m_MiNiGameConfig.niuniuConfig.BankerGameSum = Convert.ToUInt32(pFishNiuNiu.Attributes["BankerGameSum"].Value);
        m_MiNiGameConfig.niuniuConfig.VipNonBetSum = Convert.ToUInt32(pFishNiuNiu.Attributes["VipNonBetSum"].Value);
        m_MiNiGameConfig.niuniuConfig.MaxRate = 0;
        XmlNodeList pNiuNiuLevel = pFishNiuNiu.SelectNodes("Level");
        foreach (XmlNode var in pNiuNiuLevel)
        {
            Byte ID = Convert.ToByte(var.Attributes["ID"].Value);
            Byte Rate = Convert.ToByte(var.Attributes["Rate"].Value);
            if (m_MiNiGameConfig.niuniuConfig.MaxRate == 0 || m_MiNiGameConfig.niuniuConfig.MaxRate < Rate)
                m_MiNiGameConfig.niuniuConfig.MaxRate = Rate;
            m_MiNiGameConfig.niuniuConfig.NiuNiuLevelRate.Add(ID, Rate);
        }
        //dial
        XmlNode pFishDial = pFishMiniGame.SelectSingleNode("dial");
        if (pFishDial == null)
            return false;
        m_MiNiGameConfig.dialConfig.BeginWriteSec = Convert.ToUInt32(pFishDial.Attributes["BeginWriteSec"].Value);
        m_MiNiGameConfig.dialConfig.EndWriteSec = Convert.ToUInt32(pFishDial.Attributes["EndWriteSec"].Value);
        m_MiNiGameConfig.dialConfig.InitGlobelSum = Convert.ToUInt32(pFishDial.Attributes["InitTableGlobel"].Value);
        m_MiNiGameConfig.dialConfig.JoinGlobelSum = Convert.ToUInt32(pFishDial.Attributes["JoinRoomGlobel"].Value);
        m_MiNiGameConfig.dialConfig.UpdateSec = Convert.ToUInt32(pFishDial.Attributes["TableUpdateSec"].Value);
        m_MiNiGameConfig.dialConfig.MaxAddGlobel = Convert.ToUInt32(pFishDial.Attributes["MaxAddGlobel"].Value);
        m_MiNiGameConfig.dialConfig.JoinBankerGlobelSum = Convert.ToUInt32(pFishDial.Attributes["JoinBankerGlobelSum"].Value);
        m_MiNiGameConfig.dialConfig.GetNextBankerNeedGlobel = Convert.ToUInt32(pFishDial.Attributes["GetNextBankerNeedGlobel"].Value);
        m_MiNiGameConfig.dialConfig.BankerGameSum = Convert.ToUInt32(pFishDial.Attributes["BankerGameSum"].Value);
        m_MiNiGameConfig.dialConfig.VipNonBetSum = Convert.ToUInt32(pFishDial.Attributes["VipNonBetSum"].Value);
        m_MiNiGameConfig.dialConfig.MaxRate = 0;
        XmlNodeList pDialLevel = pFishDial.SelectNodes("Area");
        foreach (XmlNode var in pDialLevel)
        {
            Byte ID = Convert.ToByte(var.Attributes["ID"].Value);
            Byte Value = Convert.ToByte(var.Attributes["Value"].Value);
            m_MiNiGameConfig.dialConfig.DialAreaData.Insert(ID, Value);
        }
        if (m_MiNiGameConfig.dialConfig.DialAreaData.Count != FishDataInfo.MAX_Dial_GameSum)
            return false;
        //car
        XmlNode pFishCar = pFishMiniGame.SelectSingleNode("car");
        if (pFishCar == null)
            return false;
        m_MiNiGameConfig.carConfig.BeginWriteSec = Convert.ToUInt32(pFishCar.Attributes["BeginWriteSec"].Value);
        m_MiNiGameConfig.carConfig.EndWriteSec = Convert.ToUInt32(pFishCar.Attributes["EndWriteSec"].Value);
        m_MiNiGameConfig.carConfig.InitGlobelSum = Convert.ToUInt32(pFishCar.Attributes["InitTableGlobel"].Value);
        m_MiNiGameConfig.carConfig.JoinGlobelSum = Convert.ToUInt32(pFishCar.Attributes["JoinRoomGlobel"].Value);
        m_MiNiGameConfig.carConfig.UpdateSec = Convert.ToUInt32(pFishCar.Attributes["TableUpdateSec"].Value);
        m_MiNiGameConfig.carConfig.MaxAddGlobel = Convert.ToUInt32(pFishCar.Attributes["MaxAddGlobel"].Value);
        m_MiNiGameConfig.carConfig.JoinBankerGlobelSum = Convert.ToUInt32(pFishCar.Attributes["JoinBankerGlobelSum"].Value);
        m_MiNiGameConfig.carConfig.GetNextBankerNeedGlobel = Convert.ToUInt32(pFishCar.Attributes["GetNextBankerNeedGlobel"].Value);
        m_MiNiGameConfig.carConfig.BankerGameSum = Convert.ToUInt32(pFishCar.Attributes["BankerGameSum"].Value);
        m_MiNiGameConfig.carConfig.VipNonBetSum = Convert.ToUInt32(pFishCar.Attributes["VipNonBetSum"].Value);
        m_MiNiGameConfig.carConfig.MaxRate = 0;
        //Types
        XmlNode pFishCarTypes = pFishCar.SelectSingleNode("Types");
        if (pFishCarTypes == null)
            return false;
        XmlNodeList pFishCarType = pFishCarTypes.SelectNodes("Type");
        foreach (XmlNode var in pFishCarType)
        {
            tagCarType pTyp = new tagCarType();
            pTyp.TypeID = Convert.ToByte(var.Attributes["ID"].Value);
            pTyp.RateValue = Convert.ToByte(var.Attributes["RateValue"].Value);
            pTyp.RateF = Convert.ToByte(var.Attributes["RateF"].Value);
            m_MiNiGameConfig.carConfig.CarTypeMap.Add(pTyp.TypeID, pTyp);
        }
        if (m_MiNiGameConfig.carConfig.CarTypeMap.Count != FishDataInfo.MAX_Car_ClientSum)
            return false;
        XmlNode pFishCarValues = pFishCar.SelectSingleNode("Values");
        if (pFishCarValues == null)
            return false;
        XmlNodeList pFishCarValue = pFishCarValues.SelectNodes("Value");
        foreach (XmlNode var in pFishCarValue)
        {
            tagCarValue pType = new tagCarValue();
            pType.ID = Convert.ToByte(var.Attributes["ID"].Value);
            pType.TypeID = Convert.ToByte(var.Attributes["TypeID"].Value);
            //计算概率
            if (!m_MiNiGameConfig.carConfig.CarTypeMap.ContainsKey(pType.TypeID))
                return false;
            m_MiNiGameConfig.carConfig.CarValueVec.Add(pType);
        }
        if (m_MiNiGameConfig.carConfig.CarValueVec.Count < FishDataInfo.MAX_Car_ClientSum)
            return false;
        return true;
    }
}