using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家签到管理器
class RoleCheck
{
    //签到的数据 需要进行处理  需要符合服务器端时间

    public RoleCheck()
    {
       
    }
    //检查当月的某天是否签到
    public bool IsCheckByDay(Byte MonthDay)
    {
        UInt32 CheckData = PlayerRole.Instance.RoleInfo.RoleMe.GetCheckData();
        return (CheckData & (1 << MonthDay)) != 0;
    }
    public Byte GetNowDay()//获取今天的天数
    {
        DateTime time = SystemTime.Instance.GetSystemDateTime;
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        time = time.AddSeconds(WriteSec * -1);
        return Convert.ToByte(time.Day);
    }
    public Byte GetCanCheckOtherSum()
    {
        //剩余补签的次数
        Byte MonthCheckSum = GetMonthCheckDaySum();//本月签到的次数
        if (IsCanCheckNowDay())
            MonthCheckSum += 1;
        //DateTime time = SystemTime.Instance.GetSystemDateTime;

        DateTime time = SystemTime.Instance.GetSystemDateTime;
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        time = time.AddSeconds(WriteSec * -1);

        int DaySum = time.Day;//1-31
        if (MonthCheckSum > DaySum)
            return 0;
        return Convert.ToByte(DaySum - MonthCheckSum);
    }
    public bool IsCanCheckNowDay()
    {
        //判断今天是否已经签到了
        DateTime time = SystemTime.Instance.GetSystemDateTime;
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        time = time.AddSeconds(WriteSec * -1);

        int DaySum = time.Day;//1-31
        UInt32 CheckData = PlayerRole.Instance.RoleInfo.RoleMe.GetCheckData();
        return (CheckData & (1 << DaySum)) == 0;
    }
    //本月总共签到次数
    public Byte GetMonthCheckDaySum()
    {
        UInt32 CheckData = PlayerRole.Instance.RoleInfo.RoleMe.GetCheckData();
        Byte MonthSum = 0;
        for(int i =1;i<=32;++i)
        {
            if((CheckData & (1 << i)) != 0)
            {
                ++MonthSum;
            }
        }
        return MonthSum;
    }
    //签到
    public bool CheckNowDay()
    {
        return SendCheckNowDay();
    }
    //补签
    public bool CheckOtherDay()
    {
        //获取 最后一天的
        DateTime time = SystemTime.Instance.GetSystemDateTime;
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        time = time.AddSeconds(WriteSec * -1);
        int DaySum = time.Day;//1-31
        for (Byte i = (Byte)(DaySum-1); i>=1 ;--i )
        {
            if (IsCheckByDay(i))
                continue;
            return SendCheckOtherDay(i);
        }
        return false;
    }
    private bool SendCheckNowDay()
    {
        CL_Cmd_CheckNowDay ncb = new CL_Cmd_CheckNowDay();
        ncb.SetCmdType(NetCmdType.CMD_CL_CheckNowDay);
        if (!IsCanCheckNowDay())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Check_NowDay_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        NetServices.Instance.Send<CL_Cmd_CheckNowDay>(ncb);
        return true;
    }
    private bool SendCheckOtherDay(Byte DaySum)
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_CheckInfo.CheckOtherUser)
        {
            //乐币数量不够
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Check_OtherDay_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(IsCheckByDay(DaySum))//指定天是否签到了
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Check_OtherDay_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_CheckOtherDay ncb = new CL_Cmd_CheckOtherDay();
        ncb.SetCmdType(NetCmdType.CMD_CL_CheckOtherDay);
        ncb.Day = DaySum;
        NetServices.Instance.Send<CL_Cmd_CheckOtherDay>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_CheckNowDay:
                return HandleCheckNowDayResult(obj);
            case NetCmdType.CMD_LC_CheckOtherDay:
                return HandleCheckOtherDayResult(obj);
        }
        return true;
    }
    bool HandleCheckNowDayResult(NetCmdBase obj)
    {
        LC_Cmd_CheckNowDay ncb = (LC_Cmd_CheckNowDay)obj;
        if (ncb.Result)
        {
            tagCheckCheckNowDayEvent pEvent = new tagCheckCheckNowDayEvent(ncb.DayRewardID, ncb.MonthRewardID);
            MsgEventHandle.HandleMsg(pEvent);

        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Check_NowDay_Sucess : UserOperateMessage.UOM_Check_NowDay_Failed_1));
        MsgEventHandle.HandleMsg(pUOM);

        return true;
    }
    bool HandleCheckOtherDayResult(NetCmdBase obj)
    {
        LC_Cmd_CheckOtherDay ncb = (LC_Cmd_CheckOtherDay)obj;
        if (ncb.Result)
        {
            tagCheckCheckOtherDayEvent pEvent = new tagCheckCheckOtherDayEvent(ncb.Day, ncb.DayRewardID, ncb.MonthRewardID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Check_OtherDay_Sucess : UserOperateMessage.UOM_Check_OtherDay_Failed_3));
        MsgEventHandle.HandleMsg(pUOM);
        return true;
    }
}