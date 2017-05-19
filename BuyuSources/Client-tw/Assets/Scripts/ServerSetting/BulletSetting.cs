using System;
using System.Collections.Generic;

public class BulletSetting
{
    public static ushort[] BulletRate;
    public static ushort[] RateUnlock;
    public static ushort[] RateReward;
    public static ReductionData FreezeBulletReduction = new ReductionData();
    public static byte BULLET_MAX_NUM = byte.MaxValue;
}
