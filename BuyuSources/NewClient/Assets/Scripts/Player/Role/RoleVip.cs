using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RoleVip
{
    public Byte GetLauncherReBoundNum()//获取子弹反弹的次数
    {
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() ==0 || !FishConfig.Instance.m_VipConfig.VipMap.ContainsKey( PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()))
            return FishConfig.Instance.m_VipConfig.DefaultLauncherReBoundNum;
        return Convert.ToByte(FishConfig.Instance.m_VipConfig.DefaultLauncherReBoundNum + FishConfig.Instance.m_VipConfig.VipMap[PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()].TotalLevel.LauncherReBoundNum);
    }
    public Byte GetUseMedalSum()//获取可以兑换的次数
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0 || !FishConfig.Instance.m_VipConfig.VipMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()))
            return FishConfig.Instance.m_VipConfig.DefaultUseMedalSum;
        return Convert.ToByte(FishConfig.Instance.m_VipConfig.DefaultUseMedalSum + FishConfig.Instance.m_VipConfig.VipMap[PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()].TotalLevel.AddUseMedalSum); 
    }
    //public bool IsCanLauncherLocking()//是否可以子弹锁定
    //{
    //    if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0 || !FishConfig.Instance.m_VipConfig.VipMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()))
    //        return false;
    //    return FishConfig.Instance.m_VipConfig.VipMap[PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()].TotalLevel.IsCanLauncherLocking;
    //}
    public UInt32 AddAlmsSum()//可以领取救济金次数
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0 || !FishConfig.Instance.m_VipConfig.VipMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()))
            return FishConfig.Instance.m_VipConfig.DefaultAlmsSum;
        return Convert.ToByte(FishConfig.Instance.m_VipConfig.DefaultAlmsSum + FishConfig.Instance.m_VipConfig.VipMap[PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()].TotalLevel.AddAlmsSum);
    }
    public float AddMonthScoreRate()//比赛积分提升比例
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0 || !FishConfig.Instance.m_VipConfig.VipMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()))
            return 1.0f;
        return Convert.ToSingle((FishConfig.Instance.m_VipConfig.VipMap[PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()].TotalLevel.AddMonthScoreRate +100.0f)/100.0f);
    }
    public float AddReChargeRate()//充值提升比例
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0 || !FishConfig.Instance.m_VipConfig.VipMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()))
            return 1.0f;
        return Convert.ToSingle((FishConfig.Instance.m_VipConfig.VipMap[PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()].TotalLevel.AddReChargeRate + 100.0f) / 100.0f);
    }
    public float AddCatchFishRate()//捕鱼概率提升
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0 || !FishConfig.Instance.m_VipConfig.VipMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()))
            return 1.0f;
        return Convert.ToSingle((FishConfig.Instance.m_VipConfig.VipMap[PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()].TotalLevel.AddCatchFishRate + 100.0f) / 100.0f);
    }
    public float AddAlmsRate()//救济金的提升比例
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0 || !FishConfig.Instance.m_VipConfig.VipMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()))
            return 1.0f;
        return Convert.ToSingle((FishConfig.Instance.m_VipConfig.VipMap[PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()].TotalLevel.AddAlmsRate + 100.0f) / 100.0f);
    }
}