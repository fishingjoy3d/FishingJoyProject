using System;
using System.Collections.Generic;
public class SkillConsume  //道具消耗,
{
   public byte byorder;//次数
   public byte byCount;//数量
};
public class SkillData
{
    public byte ID;
    public string name;
    public float CDTime;
    public ushort ItemId;//消耗物品id
    public ushort multiple;
    public List<SkillConsume> NumRequire = new List<SkillConsume>();//
    public ReductionData Reduction = new ReductionData();
}
public class SkillSetting
{
    public static SkillData[] SkillDataList;
}
