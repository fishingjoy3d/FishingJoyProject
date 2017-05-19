using System;
using System.Collections.Generic;
public class RoomData
{
    public byte RoomID;
    public byte RoomRateIdx;
}
public class ExtraSetting
{
    public static RoomData[] RoomDataList;
    public static byte MultipleEffect;
    public static ushort WonderFulBase;
}
public class DoubleHit
{
    public static float     IntervalTime;   //连击的间隔时间(秒)
    public static ushort    MaxTimes;       //连击的最大次数
}
