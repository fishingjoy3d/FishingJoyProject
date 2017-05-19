using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RoleMonthCard//玩家月卡
{
    public bool IsCanAutoFire()//是否可以自动开火
    {
        if (IsInMonthCard())
        {
            if (!FishConfig.Instance.m_MonthCardConfig.MonthCardMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardID()))
                return false;
            return FishConfig.Instance.m_MonthCardConfig.MonthCardMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardID()].IsCanAutoFire;
        }
        else
        {
            return false;
        }
    }
    public float AddLotteryRate()//抽奖概率提升
    {
        if (IsInMonthCard())
        {
            if (!FishConfig.Instance.m_MonthCardConfig.MonthCardMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardID()))
                return 1.0f;
            return (FishConfig.Instance.m_MonthCardConfig.MonthCardMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardID()].AddLotteryRate + 100.0f) / 100.0f;
        }
        else
        {
            return 1.0f;
        }
    }
    public bool IsInMonthCard()//玩家是否在月卡内
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardID() == 0 || SystemTime.Instance.GetSystemDateTime > PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardEndTime())
            return false;
        else
            return true;
    }
    public bool IsCanGetRoleMonthCardReward()
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardID() == 0 || SystemTime.Instance.GetSystemDateTime > PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardEndTime())
            return false;
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        DateTime LogTime = PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardRewardTime().AddSeconds(WriteSec * -1);//领取奖励的时间
        DateTime time = SystemTime.Instance.GetSystemDateTime.AddSeconds(WriteSec * -1);//系统时间
        if (time.Year == LogTime.Year && time.Month == LogTime.Month && time.Day == LogTime.Day)
        {
            return false;//今天已经领取过了
        }
        return true;
    }
    public bool GetRoleMonthCardReward() //获取玩家月卡的奖励
    {
        //1.判断玩家是否为月卡用户
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardID() == 0 || SystemTime.Instance.GetSystemDateTime > PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardEndTime())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_MonthCardReward_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        DateTime LogTime = PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardRewardTime().AddSeconds(WriteSec * -1);//领取奖励的时间
        DateTime time = SystemTime.Instance.GetSystemDateTime.AddSeconds(WriteSec * -1);//系统时间
        if (time.Year == LogTime.Year && time.Month == LogTime.Month && time.Day == LogTime.Day)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_MonthCardReward_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;//今天已经领取过了
        }
        CL_Cmd_GetMonthCardReward ncb = new CL_Cmd_GetMonthCardReward();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetMonthCardReward);
        NetServices.Instance.Send<CL_Cmd_GetMonthCardReward>(ncb);
        return true;
    }
}