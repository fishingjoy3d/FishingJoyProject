using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
public class SystemTime : Singleton<SystemTime>//系统时间记录 修改时间为 24小时制度
{
    private DateTime LogClientTime = DateTime.Now;
    private DateTime LogSystemTime = DateTime.Now;
    private UInt32 LogTimeStep = Utility.GetTickCount();
    private UInt32 LogTimeStepByDay = Utility.GetTickCount();
    private double ServerDiffSec = 0.0;
    public DateTime GetSystemDateTime
    {
        get
        {
            return DateTime.Now.AddSeconds(ServerDiffSec);
            //UInt32 temp = Utility.GetTickCount() - LogTimeStep;
            //DateTime Time = LogSystemTime.AddMilliseconds(temp);
            //return Time;
        }
    }
    public UInt32 GetSystemTimeTotalSecond()
    {
        //获取时间戳
        DateTime starTime = TimeZone.CurrentTimeZone.ToLocalTime(new DateTime(1970, 1, 1));
        return (UInt32)(GetSystemDateTime - starTime).TotalSeconds;
    }
    public UInt32 GetTimeTotalSecond(DateTime pTime)
    {
        DateTime starTime = TimeZone.CurrentTimeZone.ToLocalTime(new DateTime(1970, 1, 1));
        return (UInt32)(pTime - starTime).TotalSeconds;
    }
    public DateTime GetDateTimeByTotalSec(Int64 TotalSec)
    {
        Int64 DiffSec = TotalSec - GetSystemTimeTotalSecond();
        return GetSystemDateTime.AddSeconds(DiffSec);
    }
    public DateTime GetOnlineTime()
    {
        return LogSystemTime;
    }
    public UInt16 GetOnlineSec()
    {
        return Convert.ToUInt16((Utility.GetTickCount() - LogTimeStep) / 1000);//获取当前游戏在线的秒数
    }
    public UInt32 GetOnlineSecByDay()
    {
        return Convert.ToUInt32((Utility.GetTickCount() - LogTimeStepByDay) / 1000);//获取当前游戏在线的秒数
    }
    public void SetOnlineSecByDay(UInt32 OnLineSec)
    {
        if (Utility.GetTickCount() < (OnLineSec * 1000))
            LogTimeStepByDay = 0;
        else
            LogTimeStepByDay =   Convert.ToUInt16(Utility.GetTickCount() - (OnLineSec*1000));
    }
    public void ClearLogTimeByDay()
    {
        LogTimeStepByDay = Utility.GetTickCount();
    }
    public void SetSystemTime(Byte Year, Byte Month, Byte Day, Byte Hour, Byte Min, Byte Sec)
    {
        LogSystemTime = new DateTime(Year + 2000, Month, Day, Hour, Min, Sec);
        LogClientTime = DateTime.Now;
        ServerDiffSec = (LogSystemTime - LogClientTime).TotalSeconds;//服务器端和客户端差距的秒数 记录起来

        //记录下当前的timegettime 的数据
        LogTimeStep = Utility.GetTickCount();
        LogTimeStepByDay = LogTimeStep;
    }
}