using UnityEngine;
using System.Collections.Generic;
public class FishData
{
    public byte     ID;
    public ushort   Gold;
    public string   Name;
    public string   Decl;
}


public class FishDeadTimeData
{
    public float TianZai_DouDong_Time;           //天灾抖动时间

    public float TianZai_Stay_Time1;             //天灾等待陨石降落停留最短时间
    public float TianZai_Stay_Time2;             //天灾等待陨石降落停留最长时间

    public float TianZai_Dead_Time1;             //天灾死亡停留最短时间
    public float TianZai_Dead_Time2;             //天灾死亡停留最长时间

    public float Bullet_BingDong_Dead_Time1;     //冰冻炮击中死亡停留最短时间
    public float Bullet_BingDong_Dead_Time2;     //冰冻炮击中死亡停留长短时间

    public float BingDong_Dead_Time1;            //冰冻技能死亡最短时间
    public float BingDong_Dead_Time2;            //冰冻技能死亡最长时间

    public float ShanDian_Dead_Time1;            //闪电技能死亡最短时间
    public float ShanDian_Dead_Time2;            //闪电技能死亡最长时间

    public float LongJuanFeng_Dead_Time1;        //龙卷风技能死亡最短时间
    public float LongJuanFeng_Dead_Time2;        //龙卷风技能死亡最长时间

    public float JiGuang_Stay_Time;              //激光死亡鱼减速到0的时间
    public float JiGuang_Dead_Time1;             //激光大招死亡最短时间
    public float JiGuang_Dead_Time2;             //激光大招死亡最长时间

    public float ShandDian_Speed;
    public float JiGuang_Speed;
    public float JiGuang_BingDong_Speed;
    public float TianZai_Speed;
    public float BingDong_Speed;
}
public class FishSetting
{
    public struct OrderFishByGold : System.IComparable
    {
        public byte ArrIndex;
        public byte FishID;
        public int Gold;
        public int CompareTo(object obj)
        {
            OrderFishByGold ff = (OrderFishByGold)obj;
            if (Gold > ff.Gold)
                return 1;
            else if (Gold < ff.Gold)
                return -1;
            else
                return 0;
        }
    }
    public static List<OrderFishByGold> SortFishList;
    public static FishDeadTimeData FishDeadTime;
    public static FishData[] FishDataList;
    public static ushort FISH_MAX_NUM = ushort.MaxValue;
    public static void SortByGold()
    {
        SortFishList = new List<OrderFishByGold>(FishDataList.Length);
        for(byte i = 0; i < FishDataList.Length; ++i)
        {
            if (FishDataList[i].Gold != 0)
            {
                OrderFishByGold ll;
                ll.ArrIndex = i;
                ll.FishID = FishDataList[i].ID;
                ll.Gold = FishDataList[i].Gold;
                SortFishList.Add(ll);
            }
        }
        SortFishList.Sort();
    }
}