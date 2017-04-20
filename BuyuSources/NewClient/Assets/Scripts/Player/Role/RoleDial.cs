using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public enum DialTableStates
{
    DTS_Begin = 1,//下注阶段
    DTS_End = 2,//
    DTS_WriteBegin = 3,//等待开始
};


public class DialTableUpdateInfo
{
    public UInt64[] GlobelNum;
    public DialTableUpdateInfo()
    {
        GlobelNum = new UInt64[FishDataInfo.MAX_Dial_ClientSum];
    }
    public void SetInfo(UInt64[] GlobelNum)
    {
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
        {
            this.GlobelNum[i] = GlobelNum[i];
        }
    }
}
public class DialTableAreaData //转盘数据
{
    public Byte[] TableAreaData;//转盘的数据
    public Byte[] TableAreaDataRate;//转盘的倍率
   // public UInt64[] TableBetAreaGlobel;//转盘的下注金币
    public DialTableAreaData()
    {
        TableAreaData = new Byte[FishDataInfo.MAX_Dial_GameSum];
        //TableBetAreaGlobel = new UInt64[FishDataInfo.MAX_Dial_ClientSum];
        TableAreaDataRate = new Byte[FishDataInfo.MAX_Dial_ClientSum];
    }
    public void SetInfo(Byte[] TableAreaData, Byte[] TableAreaDataRate)
    {
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
        {
            //this.TableBetAreaGlobel[i] = TableBetAreaGlobel[i];
            this.TableAreaDataRate[i] = TableAreaDataRate[i];
        }
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_GameSum; ++i)
        {
            this.TableAreaData[i] = TableAreaData[i];
        }
    }
}
public class DialBankerUserInfo
{
    public DialRoleInfo UserInfo;//庄家数据
    public Byte GameSum;//已经进行的游戏盘数
    public DialBankerUserInfo()
    {
        UserInfo = new DialRoleInfo();
        GameSum = 0;
    }
    public void SetInfo(DialRoleInfo UserInfo, Byte GameSum)
    {
        this.UserInfo = UserInfo;
        this.GameSum = GameSum;
    }
}
public class DialWriteBankerList
{
    public UInt32 MySeatIndex;
    public DialRoleInfo[] List;
    public DialWriteBankerList()
    {
        MySeatIndex = 0xffffffff;
        List = new DialRoleInfo[FishDataInfo.MAX_Dial_BankerShowSum];
    }
    public void SetInfo(UInt32 MySeatIndex, DialRoleInfo[] List)
    {
        this.MySeatIndex = MySeatIndex;
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_BankerShowSum; ++i)
            this.List[i] = List[i];
    }
}
public class DialVipSeatList
{
    public DialRoleInfo[] List;
    public DialVipSeatList()
    {
        List = new DialRoleInfo[FishDataInfo.MAX_Dial_VipSeat];
    }
    public void SetInfo(DialRoleInfo[] List)
    {
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_VipSeat; ++i)
            this.List[i] = List[i];
    }
}
public class DialGameLog
{
    public Byte GameSum;
    public UInt32 GameLog;

    public DialGameLog()
    {
        GameSum = 0;
        GameLog = 0;
    }
}

class RoleDial
{
    //Dial客户端基本数据
    private DialTableStates TableStates;//桌子状态
    private DialTableUpdateInfo TableUpdateInfo = new DialTableUpdateInfo();//桌子上押注金币数
    private DialTableAreaData AreaData = new DialTableAreaData();//桌子上轮盘数据
    private DialBankerUserInfo TableBankerUserInfo = new DialBankerUserInfo();//桌子上庄家数据
    private DialWriteBankerList TableWriteBankerList = new DialWriteBankerList();//桌子上 排队列表数据
    private UInt32[] TableGlobelSum = new UInt32[FishDataInfo.MAX_Dial_ClientSum];//桌子上自己下注区的金币数量
    private UInt32[] LogTableGlobelSum = new UInt32[FishDataInfo.MAX_Dial_ClientSum];//桌子上自己下注区的金币数量
    private DialVipSeatList TableVipSeatInfo = new DialVipSeatList();//桌子上VIP的数据
    private bool IsInBankerList = false;//是否在列表里面
    private DialGameLog TableGameLog = new DialGameLog();//桌子记录
    private UInt16 TableResult = 0;//桌子结果

    //处理函数
    public RoleDial()
    {
        for (int i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
        {
            TableGlobelSum[i] = 0;
            LogTableGlobelSum[i] = 0;
        }
    }
    public UInt32 GetRoleAddAllGlobelSum()
    {
        UInt32 All = 0;
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
            All += TableGlobelSum[i];
        return All;
    }

    //牛牛 管理类
    //进入牛牛房间
    public bool OnJoinRoom()
    {
        //1.玩家金币大于多少才可以进入
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MiNiGameConfig.dialConfig.JoinGlobelSum)
        {
            //提示玩家进入 牛牛 金币过少 无法进入
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_JoinRoom_Failed_1, FishConfig.Instance.m_MiNiGameConfig.dialConfig.JoinGlobelSum.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_DialRoleJoinTable msg = new CL_Cmd_DialRoleJoinTable();
        msg.SetCmdType(NetCmdType.CMD_CL_DialRoleJoinTable);
        NetServices.Instance.Send<CL_Cmd_DialRoleJoinTable>(msg);
        return true;
    }
    //离开牛牛房间
    public void OnLeaveDialTable()
    {
        TableUpdateInfo = new DialTableUpdateInfo();
        AreaData = new DialTableAreaData();
        TableBankerUserInfo = new DialBankerUserInfo();
        TableWriteBankerList = new DialWriteBankerList();
        TableVipSeatInfo = new DialVipSeatList();
        TableGameLog = new DialGameLog();
        IsInBankerList = false;
        for (int i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
        {
            TableGlobelSum[i] = 0;
            LogTableGlobelSum[i] = 0;
        }
        //玩家离开桌子 
        CL_Cmd_DialRoleLeaveTable ncb = new CL_Cmd_DialRoleLeaveTable();
        ncb.SetCmdType(NetCmdType.CMD_CL_DialRoleLeaveTable);
        NetServices.Instance.Send<CL_Cmd_DialRoleLeaveTable>(ncb);
    }
    private bool IsCanAddGlobel(UInt32 AddGlobel,Byte Index)
    {
        UInt64 MaxGlobel = 0;
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
        {
            UInt64 Globel = TableUpdateInfo.GlobelNum[i];
            if(i==Index)
            {
                Globel += AddGlobel;
            }
            Globel = AreaData.TableAreaDataRate[i] * Globel;
            if (Globel >= MaxGlobel)
                MaxGlobel = Globel;
        }
        if (TableBankerUserInfo.UserInfo.dwUserID != 0 && MaxGlobel > TableBankerUserInfo.UserInfo.GlobelSum)
            return false;
        else
            return true;
    }
    //下注
    public bool OnAddGlobel(UInt32 AddGlobel,Byte Index)
    {
        if (TableStates != DialTableStates.DTS_Begin)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_7);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (AddGlobel == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_8);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < AddGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_9);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //下注金币
        if(Index >= FishDataInfo.MAX_Dial_ClientSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        UInt32 AllGlobelSum = GetRoleAddAllGlobelSum();

        if (AllGlobelSum + AddGlobel > FishConfig.Instance.m_MiNiGameConfig.dialConfig.MaxAddGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_3, FishConfig.Instance.m_MiNiGameConfig.dialConfig.MaxAddGlobel.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID() == TableBankerUserInfo.UserInfo.dwUserID)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_6);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(!IsCanAddGlobel(AddGlobel,Index))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }

        CL_Cmd_DialRoleBetGlobel ncb = new CL_Cmd_DialRoleBetGlobel();
        ncb.SetCmdType(NetCmdType.CMD_CL_DialRoleBetGlobel);
        ncb.Index = Index;
        ncb.AddGlobel = AddGlobel;
        NetServices.Instance.Send<CL_Cmd_DialRoleBetGlobel>(ncb);
        return true;
    }
    public bool OnAddGlobelByLog()
    {
        //根据上次的金币进行押注
        if(TableStates !=  DialTableStates.DTS_Begin)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_7);
            MsgEventHandle.HandleMsg(pUOM);
            
            return false;
        }
        if(!PlayerRole.Instance.RoleMonthCard.IsInMonthCard())
        {
            //非月卡用户不可以续压

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_10);
            MsgEventHandle.HandleMsg(pUOM);
            
            return false;
        }
        UInt32 AddGlobel = 0;
        for (int i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
            AddGlobel += LogTableGlobelSum[i];
        if(AddGlobel == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_8);
            MsgEventHandle.HandleMsg(pUOM);
            
            return false;
        }
        UInt32 AllGlobelSum = GetRoleAddAllGlobelSum();
        if (AllGlobelSum + AddGlobel > FishConfig.Instance.m_MiNiGameConfig.dialConfig.MaxAddGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_3, FishConfig.Instance.m_MiNiGameConfig.dialConfig.MaxAddGlobel.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < AddGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_9);
            MsgEventHandle.HandleMsg(pUOM);
            
            return false;
        }
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID() == TableBankerUserInfo.UserInfo.dwUserID)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_6);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
        {
            if (!IsCanAddGlobel(LogTableGlobelSum[i], i))
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }
        CL_Cmd_DialRoleBetGlobelByLog ncb = new CL_Cmd_DialRoleBetGlobelByLog();
        ncb.SetCmdType(NetCmdType.CMD_CL_DialRoleBetGlobelByLog);
        ncb.betGlobel = new UInt32[FishDataInfo.MAX_Dial_ClientSum];
        ncb.betGlobel = LogTableGlobelSum;
        NetServices.Instance.Send<CL_Cmd_DialRoleBetGlobelByLog>(ncb);
        return true;
    }
    //获取庄家排队列表
    public void OnGetWriteBankerList()
    {
        CL_Cmd_DialGetBankerList ncb = new CL_Cmd_DialGetBankerList();
        ncb.SetCmdType(NetCmdType.CMD_CL_DialGetBankerList);
        NetServices.Instance.Send<CL_Cmd_DialGetBankerList>(ncb);
    }
    //玩家上庄
    public bool OnRoleJoinWriteBankerList()
    {
        //玩家上庄
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MiNiGameConfig.dialConfig.JoinBankerGlobelSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddBankerList_Failed_1, FishConfig.Instance.m_MiNiGameConfig.dialConfig.JoinBankerGlobelSum.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(TableWriteBankerList.MySeatIndex != 0xffffffff)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddBankerList_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }

        CL_Cmd_DialRoleJoinBankerList ncb = new CL_Cmd_DialRoleJoinBankerList();
        ncb.SetCmdType(NetCmdType.CMD_CL_DialRoleJoinBankerList);
        NetServices.Instance.Send<CL_Cmd_DialRoleJoinBankerList>(ncb);
        return true;
    }
    //玩家抢庄
    public bool OnRoleGetWriteBankerFirstSeat()
    {
        //玩家抢庄
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MiNiGameConfig.dialConfig.GetNextBankerNeedGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_NextBankerSeat_Failed_2, FishConfig.Instance.m_MiNiGameConfig.dialConfig.GetNextBankerNeedGlobel.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (TableWriteBankerList.MySeatIndex == 0xffffffff)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_NextBankerSeat_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(TableWriteBankerList.List[0].dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_NextBankerSeat_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_DialRoleGetBankerFirstSeat ncb = new CL_Cmd_DialRoleGetBankerFirstSeat();
        ncb.SetCmdType(NetCmdType.CMD_CL_DialRoleGetBankerFirstSeat);
        NetServices.Instance.Send<CL_Cmd_DialRoleGetBankerFirstSeat>(ncb);
        return true;
    }
    //玩家离开上庄队列
    public bool OnRoleLeaveWriteBankerList()
    {
        if (TableWriteBankerList.MySeatIndex == 0xffffffff)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_LeaveBankerList_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_DialRoleLeaveBankerList ncb = new CL_Cmd_DialRoleLeaveBankerList();
        ncb.SetCmdType(NetCmdType.CMD_CL_DialRoleLeaveBankerList);
        NetServices.Instance.Send<CL_Cmd_DialRoleLeaveBankerList>(ncb);
        return true;
    }
    //玩家下庄
    public bool OnRoleCanelBanker()
    {
        if(TableBankerUserInfo.UserInfo.dwUserID == 0 || TableBankerUserInfo.UserInfo.dwUserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_CanelBanker_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_DialRoleCanelBankerSeat ncb = new CL_Cmd_DialRoleCanelBankerSeat();
        ncb.SetCmdType(NetCmdType.CMD_CL_DialRoleCanelBankerSeat);
        NetServices.Instance.Send<CL_Cmd_DialRoleCanelBankerSeat>(ncb);
        return true;
    }
    //玩家在VIP的位置坐下
    public bool OnRoleJoinVipSeat(Byte VipSeatID)
    {
        if (VipSeatID >= FishDataInfo.MAX_Dial_VipSeat)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_JoinVip_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_JoinVip_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (TableBankerUserInfo.UserInfo.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_JoinVip_Failed_5);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_VipSeat; ++i)
        {
            if(TableVipSeatInfo.List[i] != null && TableVipSeatInfo.List[i].dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_JoinVip_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }

        CL_Cmd_DialRoleJoinVipSeat msg = new CL_Cmd_DialRoleJoinVipSeat();
        msg.SetCmdType(NetCmdType.CMD_CL_DialRoleJoinVipSeat);
        msg.VipSeatIndex = VipSeatID;
        NetServices.Instance.Send<CL_Cmd_DialRoleJoinVipSeat>(msg);
        return true;
    }
    public bool OnRoleLeaveVipSeat()
    {
        bool IsInVipSeat = false;
        //玩家从VIP席位移动到 普通席位
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_VipSeat; ++i)
        {
            if (TableVipSeatInfo.List[i] != null && TableVipSeatInfo.List[i].dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            {
                IsInVipSeat = true;
                break;
            }
        }
        if(!IsInVipSeat)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_LeaveVip_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_DialRoleLeaveVipSeat ncb = new CL_Cmd_DialRoleLeaveVipSeat();
        ncb.SetCmdType(NetCmdType.CMD_LC_DialRoleLeaveVipSeat);
        NetServices.Instance.Send<CL_Cmd_DialRoleLeaveVipSeat>(ncb);
        return true;
    }
    public DialGameLog GetGameLog()
    {
        return TableGameLog;
    }
    public void OnGetNormalInfo(UInt32 Page)
    {
        CL_Cmd_DialGetNormalSeatInfo msg = new CL_Cmd_DialGetNormalSeatInfo();
        msg.SetCmdType(NetCmdType.CMD_CL_DialGetNormalSeatInfo);
        msg.Page = Page;
        NetServices.Instance.Send<CL_Cmd_DialGetNormalSeatInfo>(msg);
    }

    //处理函数
    public bool OnHandleRoleJoinTable(NetCmdBase obj)
    {
        LC_Cmd_DialRoleJoinTable ncb = (LC_Cmd_DialRoleJoinTable)obj;
        if (ncb.Result)
        {
            //赋值 将消息里携带的数据 赋值到结构里面去
            TableStates = (DialTableStates)ncb.TableStates;
            TableUpdateInfo.SetInfo(ncb.TableBetAreaGlobel);
            AreaData.SetInfo(ncb.TableAreaData, ncb.TableAreaDataRate);
            TableBankerUserInfo.SetInfo(ncb.TableBankerInfo, ncb.TableBankerUseGameSum);
            TableWriteBankerList.SetInfo(0xffffffff, ncb.TableWriteBankerList);
            TableVipSeatInfo.SetInfo(ncb.VipSeatList);
            for (Byte i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
            {
                TableGlobelSum[i] = 0;
                LogTableGlobelSum[i] = 0;
            }
            IsInBankerList = false;
            TableResult = ncb.TableResultIndex;
            TableGameLog.GameLog = ncb.TableResultLog;
            TableGameLog.GameSum = ncb.TableGameSum;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_JoinRoom_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            tagDialJoin pEvent = new tagDialJoin(ncb.TableStatesUpdateSec, TableStates, TableUpdateInfo, AreaData, TableBankerUserInfo, TableWriteBankerList, TableVipSeatInfo, TableGameLog, TableResult);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            //进入房间失败了
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_JoinRoom_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleAddGlobel(NetCmdBase obj)
    {
        LC_Cmd_DialRoleBetGlobel ncb = (LC_Cmd_DialRoleBetGlobel)obj;
        if (ncb.Result)
        {
            TableUpdateInfo.GlobelNum[ncb.Index] += ncb.AddGlobel;
            TableGlobelSum[ncb.Index] += ncb.AddGlobel;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            if (TableBankerUserInfo.UserInfo.dwUserID == 0)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_3);
                MsgEventHandle.HandleMsg(pUOM);
            }
            else
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
            }
        }
        tagDialAddGlobel pEvent = new tagDialAddGlobel(TableStates, TableUpdateInfo, AreaData, ncb.Result, TableGlobelSum);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    public bool OnHandleGetWritebankerList(NetCmdBase obj)
    {
        LC_Cmd_DialGetBankerList ncb = (LC_Cmd_DialGetBankerList)obj;
        TableWriteBankerList.SetInfo(ncb.dwMySeatIndex, ncb.TableWriteBankerList);

        //触发事件
        tagDialUpdateWriteBankerList pEvent = new tagDialUpdateWriteBankerList(TableWriteBankerList);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleWriteBankerListChange(NetCmdBase obj)
    {
        OnGetWriteBankerList();//重新获取下数据
        return true;
    }
    public bool OnHandleBankerUserChange(NetCmdBase obj)
    {
        LC_Cmd_DialBankerUserChange ncb = (LC_Cmd_DialBankerUserChange)obj;
        TableBankerUserInfo.SetInfo(ncb.BankerUserInfo, ncb.GameSum);
        //触发庄家变化的事件
        tagDialBankerUserChange pEvent = new tagDialBankerUserChange(TableBankerUserInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleTableJoinBegin(NetCmdBase obj)
    {
        LC_Cmd_DialTableJoinBegin ncb = (LC_Cmd_DialTableJoinBegin)obj;

        TableStates = DialTableStates.DTS_Begin;
        TableUpdateInfo = new DialTableUpdateInfo();
        UInt64[] BetGlobel = new UInt64[FishDataInfo.MAX_Dial_ClientSum];
        //LogTableGlobelSum = TableGlobelSum;//记录上次押注的情况
        for (int i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
        {
            TableGlobelSum[i] = 0;
            BetGlobel[i]= 0;
        }
        TableUpdateInfo.SetInfo(BetGlobel);
        AreaData.SetInfo(ncb.TableAreaData, ncb.TableAreaDataRate);
        TableBankerUserInfo.GameSum++;
        //触发事件
        tagDialStatesChange pEvent = new tagDialStatesChange(TableStates, TableUpdateInfo, AreaData, null, TableVipSeatInfo, TableWriteBankerList, TableBankerUserInfo, TableGameLog,0);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleUpdateTable(NetCmdBase obj)
    {
        LC_Cmd_DialTableUpdate ncb = (LC_Cmd_DialTableUpdate)obj;

        TableStates = DialTableStates.DTS_Begin;//只有在开始阶段 才会进行更新
        TableUpdateInfo.SetInfo(ncb.TableBetAreaGlobel);
        //事件
        tagDialTableInfoChange pEvent = new tagDialTableInfoChange(TableStates, TableUpdateInfo);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    public bool OnHandleTableJoinEnd(NetCmdBase obj)
    {
        LC_Cmd_DialTableJoinEnd ncb = (LC_Cmd_DialTableJoinEnd)obj;
        TableResult = ncb.TableResultIndex;
        TableUpdateInfo.SetInfo(ncb.TableBetAreaGlobel);
        TableStates = DialTableStates.DTS_End;//等待关闭状态

        for (int i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
            LogTableGlobelSum[i] = TableGlobelSum[i];
        for (int i = 0; i < FishDataInfo.MAX_Dial_ClientSum; ++i)
            TableGlobelSum[i] = 0;

        //金币变化
        if (ncb.BankerUserGlobelInfo.dwUserID == TableBankerUserInfo.UserInfo.dwUserID)
        {
            TableBankerUserInfo.UserInfo.GlobelSum = ncb.BankerUserGlobelInfo.dwGlobelSum;
        }

        for (Byte i = 0; i < FishDataInfo.MAX_Dial_BankerShowSum; ++i)
        {
            if (TableWriteBankerList.List[i] != null && ncb.BankerListGlobelInfo[i].dwUserID == TableWriteBankerList.List[i].dwUserID)
            {
                TableWriteBankerList.List[i].GlobelSum = ncb.BankerListGlobelInfo[i].dwGlobelSum;
            }
        }
        DialVipSeatList pOldlist = new DialVipSeatList();
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_VipSeat; ++i)
        {
            if (TableVipSeatInfo.List[i] != null && ncb.VipGlobelInfo[i].dwUserID == TableVipSeatInfo.List[i].dwUserID)
            {
                pOldlist.List[i] = TableVipSeatInfo.List[i];

                TableVipSeatInfo.List[i].GlobelSum = ncb.VipGlobelInfo[i].dwGlobelSum;
            }
        }

        TableGameLog.GameLog = ncb.TableResultLog;
        TableGameLog.GameSum = ncb.TableGameSum;

        {
            Byte Index1 = Convert.ToByte(TableResult >> 8);
            Byte Index2 = (Byte)TableResult;
            Byte Rank = Convert.ToByte(AreaData.TableAreaData[Index1] / 4);
            Byte Value = Convert.ToByte(FishConfig.Instance.m_MiNiGameConfig.dialConfig.DialAreaData[Index2]);
            string strRank = (Rank == 0 ? "红" : (Rank == 1 ? "黄" : "绿"));
            string strValue = (Value == 0 ? "熊猫" : (Value == 1 ? "猴子" : (Value == 2 ? "兔子" : "狮子")));
            string strInfo = strValue + strRank;
            LogMgr.Log(strInfo);
        }

        //触发事件
        tagDialStatesChange pEvent = new tagDialStatesChange(TableStates, TableUpdateInfo, AreaData, pOldlist, TableVipSeatInfo, TableWriteBankerList, TableBankerUserInfo, TableGameLog, TableResult);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleJoinWriteBankerList(NetCmdBase obj)
    {
        LC_Cmd_DialRoleJoinBankerList ncb = (LC_Cmd_DialRoleJoinBankerList)obj;
        if (ncb.Result)
        {
            TableWriteBankerList.MySeatIndex = ncb.SeatIndex;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddBankerList_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            tagDialUpdateWriteBankerList pEvent = new tagDialUpdateWriteBankerList(TableWriteBankerList);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddBankerList_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleRoleGetWriteBankerFirstSeat(NetCmdBase obj)
    {
        LC_Cmd_DialRoleGetBankerFirstSeat ncb = (LC_Cmd_DialRoleGetBankerFirstSeat)obj;
        if (ncb.Result)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_NextBankerSeat_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_NextBankerSeat_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleRoleLeaveWriteBankerList(NetCmdBase obj)
    {
        LC_Cmd_DialRoleLeaveBankerList ncb = (LC_Cmd_DialRoleLeaveBankerList)obj;
        if (ncb.Result)
        {
            TableWriteBankerList.MySeatIndex = 0xffffffff;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_LeaveBankerList_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            tagDialUpdateWriteBankerList pEvent = new tagDialUpdateWriteBankerList(TableWriteBankerList);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_LeaveBankerList_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleRoleCanelBanker(NetCmdBase obj)
    {
        LC_Cmd_DialRoleCanelBankerSeat ncb = (LC_Cmd_DialRoleCanelBankerSeat)obj;
        if (ncb.Result)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_CanelBanker_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_CanelBanker_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleBankerUserGlobelChange(NetCmdBase obj)
    {
        LC_Cmd_DialBankerUserGlobelChange ncb = (LC_Cmd_DialBankerUserGlobelChange)obj;
        if (TableBankerUserInfo.UserInfo.dwUserID != ncb.dwBankerUserID)
            return false;
        TableBankerUserInfo.UserInfo.GlobelSum = ncb.dwBankerGlobelSum;
        //触发事件
        tagDialBankerUserChange pEvent = new tagDialBankerUserChange(TableBankerUserInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleWriteBankerUserGlobelMsg(NetCmdBase obj)
    {
        //触发事件 玩家上单后金币不够 的提示
        tagDialWriteBankerGlobelMsg pEvent = new tagDialWriteBankerGlobelMsg();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleRoleJoinVipSeat(NetCmdBase obj)
    {
        LC_Cmd_DialRoleJoinVipSeat ncb = (LC_Cmd_DialRoleJoinVipSeat)obj;
        if (ncb.Result)
        {
            TableVipSeatInfo.List[ncb.VipSeatIndex] = ncb.DestUserInfo;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_JoinVip_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_JoinVip_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
        }

        tagDialVipSeatChangeEvent pEvent = new tagDialVipSeatChangeEvent(ncb.Result, TableVipSeatInfo);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    public bool OnHandleRoleLeaveVipSeat(NetCmdBase obj)
    {
        LC_Cmd_DialRoleLeaveVipSeat ncb = (LC_Cmd_DialRoleLeaveVipSeat)obj;
        for (Byte i = 0; i < FishDataInfo.MAX_Dial_VipSeat; ++i)
        {
            if (TableVipSeatInfo.List[i] != null && TableVipSeatInfo.List[i].dwUserID == ncb.dwDestUserID)
            {
                TableVipSeatInfo.List[i].dwUserID = 0;
                TableVipSeatInfo.List[i].GlobelSum = 0;
                TableVipSeatInfo.List[i].NickName = "";
                TableVipSeatInfo.List[i].dwFaceID = 0;

                if (ncb.dwDestUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_LeaveVip_Sucess);
                    MsgEventHandle.HandleMsg(pUOM);
                }

                tagDialVipSeatChangeEvent pEvent = new tagDialVipSeatChangeEvent(true, TableVipSeatInfo);
                MsgEventHandle.HandleMsg(pEvent);

                return true;
            }
        }
        return true;
    }
    public bool OnHandleRoleBeLeaveVipSeat(NetCmdBase obj)
    {
        LC_Cmd_DialRoleBeLeaveVipSeat ncb = (LC_Cmd_DialRoleBeLeaveVipSeat)obj;
        tagDialBeLeaveVipSeatEvent pEvent = new tagDialBeLeaveVipSeatEvent(ncb.DestRoleInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }

    public bool OnHandleGetNormalSeatInfo(NetCmdBase obj)
    {
        LC_Cmd_DialGetNormalSeatInfo ncb = (LC_Cmd_DialGetNormalSeatInfo)obj;

        tagDialGetNormalSeatInfoEvent pEvent = new tagDialGetNormalSeatInfoEvent(ncb.Page, ncb.TotalRoleSum, ncb.Array);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleVipGetGlobel(NetCmdBase obj)
    {
        LC_Cmd_DialVipSeatGlobelChange ncb = (LC_Cmd_DialVipSeatGlobelChange)obj;
        TableVipSeatInfo.List[ncb.VipSeat].GlobelSum -= ncb.GlobelSum;

        tagDialVipGetGlobelEvent pEvent = new tagDialVipGetGlobelEvent(ncb.VipSeat, ncb.Index, ncb.GlobelSum, TableVipSeatInfo.List[ncb.VipSeat].GlobelSum);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleTableStopUpdate(NetCmdBase obj)
    {
        LC_Cmd_DialTableStopUpdate ncb = (LC_Cmd_DialTableStopUpdate)obj;

        tagDialTableStopUpdate pEvent = new tagDialTableStopUpdate();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }

    public bool OnHandleVipGlobelChange(NetCmdBase obj)
    {
        LC_Cmd_DialVipGlobelChange ncb = (LC_Cmd_DialVipGlobelChange)obj;
        for (Byte i = 0; i < TableVipSeatInfo.List.Length; ++i)
        {
            if (TableVipSeatInfo.List[i].dwUserID == ncb.VipUserID)
            {
                TableVipSeatInfo.List[i].GlobelSum = ncb.VipGlobelSum;

                tagDialVipGlobelChange pEvent = new tagDialVipGlobelChange(ncb.VipUserID, i, ncb.VipGlobelSum);
                MsgEventHandle.HandleMsg(pEvent);

                return true;
            }
        }
        return true;
    }
    public bool OnHandleRoleBetGlobelByLog(NetCmdBase obj)
    {
        LC_Cmd_DialRoleBetGlobelByLog ncb = (LC_Cmd_DialRoleBetGlobelByLog)obj;
        if(ncb.Result)
        {
            //继续下注成功了
            for(Byte i =0;i<FishDataInfo.MAX_Dial_ClientSum;++i)
            {
                TableGlobelSum[i] += ncb.betGlobel[i];
                TableUpdateInfo.GlobelNum[i] += ncb.betGlobel[i];
                LogTableGlobelSum[i] = 0;
            }

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            //玩家继续下注失败 无法下注
            if (TableBankerUserInfo.UserInfo.dwUserID == 0)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_3);
                MsgEventHandle.HandleMsg(pUOM);
            }
            else
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Dial_AddGlobel_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
            }
        }
        tagDialAddGlobel pEvent = new tagDialAddGlobel(TableStates, TableUpdateInfo, AreaData, ncb.Result, TableGlobelSum);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}