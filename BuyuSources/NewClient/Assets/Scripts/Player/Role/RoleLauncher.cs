using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RoleLauncher
{
    private UInt32 m_LauncherData = 0;//炮台的数据
    public RoleLauncher()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_LauncherData = 0;
    }
    public List<Byte> GetCanUseLauncher()
    {
        List<Byte> pList = new List<Byte>();
        //获取所有可以使用的炮台集合
        for(Byte i=0;i<(Byte)LauncherType.LAUNCHER_MAX;++i)
        {
            if(IsCanUseLauncher(i))
            {
                pList.Add(i);
            }
        }
        return pList;
    }
    public Byte LauncherIsVipUse(Byte LauncherType)
    {
        //获取炮台使用VIP等级 
       foreach(tagVipOnce var in FishConfig.Instance.m_VipConfig.VipMap.Values)
       {
           if(var.TotalLevel.CanUseLauncher.ContainsKey(LauncherType))
           {
               return var.VipLevel;
           }
       }
       return 0;
    }
    public bool IsCanUseLauncher(Byte LauncherType)//判断炮台是否可用
    {
        return Convert.ToUInt32(m_LauncherData & (1 << (LauncherType + 1))) != 0;
    }
    public bool GetLauncherEndTime(Byte LauncherType,out UInt32 EndTime)//先可以使用炮才可以判断炮的结束时间
    {
        //获取炮台需要的物品的ID 和数量 判断 物品结束的时间 直接使用函数
        //核心问题在于物品数据不一定发送到客户端去
        //获取指定类型的炮台的结束的时间  如果为null 表示无结束时间 可能 炮塔是永久的 或者 无法使用的炮台
        EndTime = 0;
        if (!IsCanUseLauncher(LauncherType))
            return false;
        else
        {
            UInt32 ItemID = LauncherSetting.LauncherDataList[LauncherType].nItemid;
            UInt32 ItemSum = LauncherSetting.LauncherDataList[LauncherType].nItemcount;
            if (ItemID == 0 || ItemSum == 0)
            {
                EndTime = 0;
                return true;
            }
            UInt32 EndSec = 0;
            if (!PlayerRole.Instance.ItemManager.GetItemEndTime(ItemID, ItemSum, out EndSec))
                return false;
            EndTime = EndSec;
            return true;
        }
    }
    public bool IsCanUseLauncherByAllTime(Byte Launcher)
    {
        //判断玩家是否永久用于指定的炮
        if(!IsCanUseLauncher(Launcher))
            return false;
        UInt32 EndSec = 0;
        if(!GetLauncherEndTime(Launcher,out EndSec))
            return false;
       if(EndSec == 0)
           return true;
       else
           return false;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_LauncherData:
                return HandleLoadRoleLauncherData(obj);
        }
        return true;
    }
    bool HandleLoadRoleLauncherData(NetCmdBase obj)
    {
        LC_Cmd_LauncherData ncb = (LC_Cmd_LauncherData)obj;
        m_LauncherData = ncb.LauncherData;
        //触发
        tagLauncherDataChangeEvent pEvent = new tagLauncherDataChangeEvent(ncb.LauncherData);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}