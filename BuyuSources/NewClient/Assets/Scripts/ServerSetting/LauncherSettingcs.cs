using System;
using System.Collections.Generic;

public enum LauncherType
{
    LAUNCHER_1 = 0,     //第1号炮
    LAUNCHER_2,         //第2号炮
    LAUNCHER_3,
    LAUNCHER_4,
    LAUNCHER_5,
    LAUNCHER_MAX,
}

public class LauncherData
{
    public byte             ID;
    public string           name;
    public float            Interval;
    public ushort           Energy;     //能量值
    public byte             Consume;
    public float            LaserCDTime;
    public float            Speed;
    public ushort           nItemid;       //绑定的物品id
    public uint             nItemcount;   //绑定数量
    public byte             nSkillBind;//绑定的技能
    public ReductionData    LaserReduction = new ReductionData();
}
public class LauncherSetting
{
    public static LauncherData[] LauncherDataList;

}
