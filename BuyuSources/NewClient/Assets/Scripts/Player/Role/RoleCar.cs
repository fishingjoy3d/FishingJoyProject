using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
public enum CarTableStates
{
    CTS_Begin = 1,//下注阶段
    CTS_End = 2,//
    CTS_WriteBegin = 3,//等待开始
};

public class CarTableUpdateInfo
{
    public UInt64[] GlobelNum;
    public CarTableUpdateInfo()
    {
        GlobelNum = new UInt64[FishDataInfo.MAX_Car_ClientSum];
    }
    public void SetInfo(UInt64[] GlobelNum)
    {
        for (Byte i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
        {
            this.GlobelNum[i] = GlobelNum[i];
        }
    }
}
public class CarBankerUserInfo
{
    public CarRoleInfo UserInfo;//庄家数据
    public Byte GameSum;//已经进行的游戏盘数
    public CarBankerUserInfo()
    {
        UserInfo = new CarRoleInfo();
        GameSum = 0;
    }
    public void SetInfo(CarRoleInfo UserInfo, Byte GameSum)
    {
        this.UserInfo = UserInfo;
        this.GameSum = GameSum;
    }
}
public class CarWriteBankerList
{
    public UInt32 MySeatIndex;
    public CarRoleInfo[] List;
    public CarWriteBankerList()
    {
        MySeatIndex = 0xffffffff;
        List = new CarRoleInfo[FishDataInfo.MAX_Car_BankerShowSum];
    }
    public void SetInfo(UInt32 MySeatIndex, CarRoleInfo[] List)
    {
        this.MySeatIndex = MySeatIndex;
        for (Byte i = 0; i < FishDataInfo.MAX_Car_BankerShowSum; ++i)
            this.List[i] = List[i];
    }
}
public class CarVipSeatList
{
    public CarRoleInfo[] List;
    public CarVipSeatList()
    {
        List = new CarRoleInfo[FishDataInfo.MAX_Car_VipSeat];
    }
    public void SetInfo(CarRoleInfo[] List)
    {
        for (Byte i = 0; i < FishDataInfo.MAX_Car_VipSeat; ++i)
            this.List[i] = List[i];
    }
}
public class CarGameLog
{
    public Byte GameSum;
    public UInt32 GameLog;

    public CarGameLog()
    {
        GameSum = 0;
        GameLog = 0;
    }
}

class RoleCar
{
    //Car客户端基本数据
    private CarTableStates TableStates;//桌子状态
    private CarTableUpdateInfo TableUpdateInfo = new CarTableUpdateInfo();//桌子上押注金币数
    //private CarTableAreaData AreaData = new CarTableAreaData();//桌子上轮盘数据
    private CarBankerUserInfo TableBankerUserInfo = new CarBankerUserInfo();//桌子上庄家数据
    private CarWriteBankerList TableWriteBankerList = new CarWriteBankerList();//桌子上 排队列表数据
    private UInt32[] TableGlobelSum = new UInt32[FishDataInfo.MAX_Car_ClientSum];//桌子上自己下注区的金币数量
    private UInt32[] LogTableGlobelSum = new UInt32[FishDataInfo.MAX_Car_ClientSum];//桌子上自己下注区的金币数量
    private CarVipSeatList TableVipSeatInfo = new CarVipSeatList();//桌子上VIP的数据
    private bool IsInBankerList = false;//是否在列表里面
    private CarGameLog TableGameLog = new CarGameLog();//桌子记录
    private Byte TableResult = 0;//桌子结果

    //处理函数
    public RoleCar()
    {
        for (int i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
        {
            TableGlobelSum[i] = 0;
            LogTableGlobelSum[i] = 0;
        }
    }
    public UInt32 GetRoleAddAllGlobelSum()
    {
        UInt32 All = 0;
        for (Byte i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
            All += TableGlobelSum[i];
        return All;
    }

    //牛牛 管理类
    //进入牛牛房间
    public bool OnJoinRoom()
    {
        //1.玩家金币大于多少才可以进入
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MiNiGameConfig.carConfig.JoinGlobelSum)
        {
            //提示玩家进入 牛牛 金币过少 无法进入
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_JoinRoom_Failed_1, FishConfig.Instance.m_MiNiGameConfig.carConfig.JoinGlobelSum.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_CarRoleJoinTable msg = new CL_Cmd_CarRoleJoinTable();
        msg.SetCmdType(NetCmdType.CMD_CL_CarRoleJoinTable);
        NetServices.Instance.Send<CL_Cmd_CarRoleJoinTable>(msg);
        return true;
    }
    //离开牛牛房间
    public void OnLeaveCarTable()
    {
        TableUpdateInfo = new CarTableUpdateInfo();
        //AreaData = new CarTableAreaData();
        TableBankerUserInfo = new CarBankerUserInfo();
        TableWriteBankerList = new CarWriteBankerList();
        TableVipSeatInfo = new CarVipSeatList();
        TableGameLog = new CarGameLog();
        IsInBankerList = false;
        for (int i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
        {
            TableGlobelSum[i] = 0;
            LogTableGlobelSum[i] = 0;
        }
        //玩家离开桌子 
        CL_Cmd_CarRoleLeaveTable ncb = new CL_Cmd_CarRoleLeaveTable();
        ncb.SetCmdType(NetCmdType.CMD_CL_CarRoleLeaveTable);
        NetServices.Instance.Send<CL_Cmd_CarRoleLeaveTable>(ncb);
    }
    private bool IsCanAddGlobel(UInt32 AddGlobel, Byte Index)
    {
        UInt64 MaxGlobel = 0;
        for (Byte i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
        {
            UInt64 Globel = TableUpdateInfo.GlobelNum[i];
            if (i == Index)
            {
                Globel += AddGlobel;
            }
            Globel = FishConfig.Instance.m_MiNiGameConfig.carConfig.GetResultRate(i) * Globel;
            if (Globel >= MaxGlobel)
                MaxGlobel = Globel;
        }
        if (TableBankerUserInfo.UserInfo.dwUserID != 0 && MaxGlobel > TableBankerUserInfo.UserInfo.GlobelSum)
            return false;
        else
            return true;
    }
    //下注
    public bool OnAddGlobel(UInt32 AddGlobel, Byte Index)
    {
        if (TableStates != CarTableStates.CTS_Begin)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_7);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (AddGlobel == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_8);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < AddGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_9);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //下注金币
        if (Index >= FishDataInfo.MAX_Car_ClientSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        UInt32 AllGlobelSum = GetRoleAddAllGlobelSum();

        if (AllGlobelSum + AddGlobel > FishConfig.Instance.m_MiNiGameConfig.carConfig.MaxAddGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_3, FishConfig.Instance.m_MiNiGameConfig.carConfig.MaxAddGlobel.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetUserID() == TableBankerUserInfo.UserInfo.dwUserID)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_6);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!IsCanAddGlobel(AddGlobel, Index))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }

        CL_Cmd_CarRoleBetGlobel ncb = new CL_Cmd_CarRoleBetGlobel();
        ncb.SetCmdType(NetCmdType.CMD_CL_CarRoleBetGlobel);
        ncb.Index = Index;
        ncb.AddGlobel = AddGlobel;
        NetServices.Instance.Send<CL_Cmd_CarRoleBetGlobel>(ncb);
        return true;
    }
    public bool OnAddGlobelByLog()
    {
        //根据上次的金币进行押注
        if (TableStates != CarTableStates.CTS_Begin)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_7);
            MsgEventHandle.HandleMsg(pUOM);

            return false;
        }
        UInt32 AddGlobel = 0;
        for (int i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
            AddGlobel += LogTableGlobelSum[i];
        if (AddGlobel == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_8);
            MsgEventHandle.HandleMsg(pUOM);

            return false;
        }
        UInt32 AllGlobelSum = GetRoleAddAllGlobelSum();
        if (AllGlobelSum + AddGlobel > FishConfig.Instance.m_MiNiGameConfig.carConfig.MaxAddGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_3, FishConfig.Instance.m_MiNiGameConfig.carConfig.MaxAddGlobel.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < AddGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_9);
            MsgEventHandle.HandleMsg(pUOM);

            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetUserID() == TableBankerUserInfo.UserInfo.dwUserID)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_6);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        for (Byte i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
        {
            if (!IsCanAddGlobel(LogTableGlobelSum[i], i))
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }
        CL_Cmd_CarRoleBetGlobelByLog ncb = new CL_Cmd_CarRoleBetGlobelByLog();
        ncb.SetCmdType(NetCmdType.CMD_CL_CarRoleBetGlobelByLog);
        ncb.betGlobel = new UInt32[FishDataInfo.MAX_Car_ClientSum];
        ncb.betGlobel = LogTableGlobelSum;
        NetServices.Instance.Send<CL_Cmd_CarRoleBetGlobelByLog>(ncb);
        return true;
    }
    //获取庄家排队列表
    public void OnGetWriteBankerList()
    {
        CL_Cmd_CarGetBankerList ncb = new CL_Cmd_CarGetBankerList();
        ncb.SetCmdType(NetCmdType.CMD_CL_CarGetBankerList);
        NetServices.Instance.Send<CL_Cmd_CarGetBankerList>(ncb);
    }
    //玩家上庄
    public bool OnRoleJoinWriteBankerList()
    {
        //玩家上庄
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MiNiGameConfig.carConfig.JoinBankerGlobelSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddBankerList_Failed_1, FishConfig.Instance.m_MiNiGameConfig.carConfig.JoinBankerGlobelSum.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (TableWriteBankerList.MySeatIndex != 0xffffffff)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddBankerList_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }

        CL_Cmd_CarRoleJoinBankerList ncb = new CL_Cmd_CarRoleJoinBankerList();
        ncb.SetCmdType(NetCmdType.CMD_CL_CarRoleJoinBankerList);
        NetServices.Instance.Send<CL_Cmd_CarRoleJoinBankerList>(ncb);
        return true;
    }
    //玩家抢庄
    public bool OnRoleGetWriteBankerFirstSeat()
    {
        //玩家抢庄
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MiNiGameConfig.carConfig.GetNextBankerNeedGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_NextBankerSeat_Failed_2, FishConfig.Instance.m_MiNiGameConfig.carConfig.GetNextBankerNeedGlobel.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (TableWriteBankerList.MySeatIndex == 0xffffffff)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_NextBankerSeat_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (TableWriteBankerList.List[0].dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_NextBankerSeat_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_CarRoleGetBankerFirstSeat ncb = new CL_Cmd_CarRoleGetBankerFirstSeat();
        ncb.SetCmdType(NetCmdType.CMD_CL_CarRoleGetBankerFirstSeat);
        NetServices.Instance.Send<CL_Cmd_CarRoleGetBankerFirstSeat>(ncb);
        return true;
    }
    //玩家离开上庄队列
    public bool OnRoleLeaveWriteBankerList()
    {
        if (TableWriteBankerList.MySeatIndex == 0xffffffff)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_LeaveBankerList_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_CarRoleLeaveBankerList ncb = new CL_Cmd_CarRoleLeaveBankerList();
        ncb.SetCmdType(NetCmdType.CMD_CL_CarRoleLeaveBankerList);
        NetServices.Instance.Send<CL_Cmd_CarRoleLeaveBankerList>(ncb);
        return true;
    }
    //玩家下庄
    public bool OnRoleCanelBanker()
    {
        if (TableBankerUserInfo.UserInfo.dwUserID == 0 || TableBankerUserInfo.UserInfo.dwUserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_CanelBanker_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_CarRoleCanelBankerSeat ncb = new CL_Cmd_CarRoleCanelBankerSeat();
        ncb.SetCmdType(NetCmdType.CMD_CL_CarRoleCanelBankerSeat);
        NetServices.Instance.Send<CL_Cmd_CarRoleCanelBankerSeat>(ncb);
        return true;
    }
    //玩家在VIP的位置坐下
    public bool OnRoleJoinVipSeat(Byte VipSeatID)
    {
        if (VipSeatID >= FishDataInfo.MAX_Car_VipSeat)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_JoinVip_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_JoinVip_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (TableBankerUserInfo.UserInfo.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_JoinVip_Failed_5);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        for (Byte i = 0; i < FishDataInfo.MAX_Car_VipSeat; ++i)
        {
            if (TableVipSeatInfo.List[i] != null && TableVipSeatInfo.List[i].dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_JoinVip_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }

        CL_Cmd_CarRoleJoinVipSeat msg = new CL_Cmd_CarRoleJoinVipSeat();
        msg.SetCmdType(NetCmdType.CMD_CL_CarRoleJoinVipSeat);
        msg.VipSeatIndex = VipSeatID;
        NetServices.Instance.Send<CL_Cmd_CarRoleJoinVipSeat>(msg);
        return true;
    }
    public bool OnRoleLeaveVipSeat()
    {
        bool IsInVipSeat = false;
        //玩家从VIP席位移动到 普通席位
        for (Byte i = 0; i < FishDataInfo.MAX_Car_VipSeat; ++i)
        {
            if (TableVipSeatInfo.List[i] != null && TableVipSeatInfo.List[i].dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            {
                IsInVipSeat = true;
                break;
            }
        }
        if (!IsInVipSeat)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_LeaveVip_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_CarRoleLeaveVipSeat ncb = new CL_Cmd_CarRoleLeaveVipSeat();
        ncb.SetCmdType(NetCmdType.CMD_LC_CarRoleLeaveVipSeat);
        NetServices.Instance.Send<CL_Cmd_CarRoleLeaveVipSeat>(ncb);
        return true;
    }
    public CarGameLog GetGameLog()
    {
        return TableGameLog;
    }
    public void OnGetNormalInfo(UInt32 Page)
    {
        CL_Cmd_CarGetNormalSeatInfo msg = new CL_Cmd_CarGetNormalSeatInfo();
        msg.SetCmdType(NetCmdType.CMD_CL_CarGetNormalSeatInfo);
        msg.Page = Page;
        NetServices.Instance.Send<CL_Cmd_CarGetNormalSeatInfo>(msg);
    }

    //处理函数
    public bool OnHandleRoleJoinTable(NetCmdBase obj)
    {
        LC_Cmd_CarRoleJoinTable ncb = (LC_Cmd_CarRoleJoinTable)obj;
        if (ncb.Result)
        {
            //赋值 将消息里携带的数据 赋值到结构里面去
            TableStates = (CarTableStates)ncb.TableStates;
            TableUpdateInfo.SetInfo(ncb.TableBetAreaGlobel);
            TableBankerUserInfo.SetInfo(ncb.TableBankerInfo, ncb.TableBankerUseGameSum);
            TableWriteBankerList.SetInfo(0xffffffff, ncb.TableWriteBankerList);
            TableVipSeatInfo.SetInfo(ncb.VipSeatList);
            for (Byte i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
            {
                TableGlobelSum[i] = 0;
                LogTableGlobelSum[i] = 0;
            }
            IsInBankerList = false;
            TableResult = ncb.TableResultIndex;
            TableGameLog.GameLog = ncb.TableResultLog;
            TableGameLog.GameSum = ncb.TableGameSum;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_JoinRoom_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            tagCarJoin pEvent = new tagCarJoin(ncb.TableStatesUpdateSec, TableStates, TableUpdateInfo, TableBankerUserInfo, TableWriteBankerList, TableVipSeatInfo, TableGameLog, TableResult);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            //进入房间失败了
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_JoinRoom_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleAddGlobel(NetCmdBase obj)
    {
        LC_Cmd_CarRoleBetGlobel ncb = (LC_Cmd_CarRoleBetGlobel)obj;
        if (ncb.Result)
        {
            TableUpdateInfo.GlobelNum[ncb.Index] += ncb.AddGlobel;
            TableGlobelSum[ncb.Index] += ncb.AddGlobel;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            if (TableBankerUserInfo.UserInfo.dwUserID == 0)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_3);
                MsgEventHandle.HandleMsg(pUOM);
            }
            else
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
            }
        }
        tagCarAddGlobel pEvent = new tagCarAddGlobel(TableStates, TableUpdateInfo, ncb.Result);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    public bool OnHandleGetWritebankerList(NetCmdBase obj)
    {
        LC_Cmd_CarGetBankerList ncb = (LC_Cmd_CarGetBankerList)obj;
        TableWriteBankerList.SetInfo(ncb.dwMySeatIndex, ncb.TableWriteBankerList);

        //触发事件
        tagCarUpdateWriteBankerList pEvent = new tagCarUpdateWriteBankerList(TableWriteBankerList);
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
        LC_Cmd_CarBankerUserChange ncb = (LC_Cmd_CarBankerUserChange)obj;
        TableBankerUserInfo.SetInfo(ncb.BankerUserInfo, ncb.GameSum);
        //触发庄家变化的事件
        tagCarBankerUserChange pEvent = new tagCarBankerUserChange(TableBankerUserInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleTableJoinBegin(NetCmdBase obj)
    {
        LC_Cmd_CarTableJoinBegin ncb = (LC_Cmd_CarTableJoinBegin)obj;

        TableStates = CarTableStates.CTS_Begin;
        TableUpdateInfo = new CarTableUpdateInfo();
        UInt64[] BetGlobel = new UInt64[FishDataInfo.MAX_Car_ClientSum];
        LogTableGlobelSum = TableGlobelSum;//记录上次押注的情况
        for (int i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
        {
            TableGlobelSum[i] = 0;
            BetGlobel[i] = 0;
        }
        TableUpdateInfo.SetInfo(BetGlobel);
        TableBankerUserInfo.GameSum++;
        //触发事件
        tagCarStatesChange pEvent = new tagCarStatesChange(TableStates, TableUpdateInfo, null, TableVipSeatInfo, TableWriteBankerList, TableBankerUserInfo, TableGameLog, 0);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleUpdateTable(NetCmdBase obj)
    {
        LC_Cmd_CarTableUpdate ncb = (LC_Cmd_CarTableUpdate)obj;

        TableStates = CarTableStates.CTS_Begin;//只有在开始阶段 才会进行更新
        TableUpdateInfo.SetInfo(ncb.TableBetAreaGlobel);
        //事件
        tagCarTableInfoChange pEvent = new tagCarTableInfoChange(TableStates, TableUpdateInfo);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    public bool OnHandleTableJoinEnd(NetCmdBase obj)
    {
        LC_Cmd_CarTableJoinEnd ncb = (LC_Cmd_CarTableJoinEnd)obj;
        TableResult = ncb.TableResultIndex;
        TableUpdateInfo.SetInfo(ncb.TableBetAreaGlobel);
        TableStates = CarTableStates.CTS_End;//等待关闭状态

        for (int i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
            TableGlobelSum[i] = 0;

        //金币变化
        if (ncb.BankerUserGlobelInfo.dwUserID == TableBankerUserInfo.UserInfo.dwUserID)
        {
            TableBankerUserInfo.UserInfo.GlobelSum = ncb.BankerUserGlobelInfo.dwGlobelSum;
        }

        for (Byte i = 0; i < FishDataInfo.MAX_Car_BankerShowSum; ++i)
        {
            if (TableWriteBankerList.List[i] != null && ncb.BankerListGlobelInfo[i].dwUserID == TableWriteBankerList.List[i].dwUserID)
            {
                TableWriteBankerList.List[i].GlobelSum = ncb.BankerListGlobelInfo[i].dwGlobelSum;
            }
        }
        CarVipSeatList pOldlist = new CarVipSeatList();
        for (Byte i = 0; i < FishDataInfo.MAX_Car_VipSeat; ++i)
        {
            if (TableVipSeatInfo.List[i] != null && ncb.VipGlobelInfo[i].dwUserID == TableVipSeatInfo.List[i].dwUserID)
            {
                pOldlist.List[i] = TableVipSeatInfo.List[i];

                TableVipSeatInfo.List[i].GlobelSum = ncb.VipGlobelInfo[i].dwGlobelSum;
            }
        }

        TableGameLog.GameLog = ncb.TableResultLog;
        TableGameLog.GameSum = ncb.TableGameSum;

        LogMgr.Log(string.Format("玩家金币变化 {0}", ncb.AddGlobelSum));

        //触发事件
        tagCarStatesChange pEvent = new tagCarStatesChange(TableStates, TableUpdateInfo, pOldlist, TableVipSeatInfo, TableWriteBankerList, TableBankerUserInfo, TableGameLog, TableResult);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleJoinWriteBankerList(NetCmdBase obj)
    {
        LC_Cmd_CarRoleJoinBankerList ncb = (LC_Cmd_CarRoleJoinBankerList)obj;
        if (ncb.Result)
        {
            TableWriteBankerList.MySeatIndex = ncb.SeatIndex;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddBankerList_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            tagCarUpdateWriteBankerList pEvent = new tagCarUpdateWriteBankerList(TableWriteBankerList);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddBankerList_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleRoleGetWriteBankerFirstSeat(NetCmdBase obj)
    {
        LC_Cmd_CarRoleGetBankerFirstSeat ncb = (LC_Cmd_CarRoleGetBankerFirstSeat)obj;
        if (ncb.Result)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_NextBankerSeat_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_NextBankerSeat_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleRoleLeaveWriteBankerList(NetCmdBase obj)
    {
        LC_Cmd_CarRoleLeaveBankerList ncb = (LC_Cmd_CarRoleLeaveBankerList)obj;
        if (ncb.Result)
        {
            TableWriteBankerList.MySeatIndex = 0xffffffff;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_LeaveBankerList_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            tagCarUpdateWriteBankerList pEvent = new tagCarUpdateWriteBankerList(TableWriteBankerList);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_LeaveBankerList_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleRoleCanelBanker(NetCmdBase obj)
    {
        LC_Cmd_CarRoleCanelBankerSeat ncb = (LC_Cmd_CarRoleCanelBankerSeat)obj;
        if (ncb.Result)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_CanelBanker_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_CanelBanker_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleBankerUserGlobelChange(NetCmdBase obj)
    {
        LC_Cmd_CarBankerUserGlobelChange ncb = (LC_Cmd_CarBankerUserGlobelChange)obj;
        if (TableBankerUserInfo.UserInfo.dwUserID != ncb.dwBankerUserID)
            return false;
        TableBankerUserInfo.UserInfo.GlobelSum = ncb.dwBankerGlobelSum;
        //触发事件
        tagCarBankerUserChange pEvent = new tagCarBankerUserChange(TableBankerUserInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleWriteBankerUserGlobelMsg(NetCmdBase obj)
    {
        //触发事件 玩家上单后金币不够 的提示
        tagCarWriteBankerGlobelMsg pEvent = new tagCarWriteBankerGlobelMsg();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleRoleJoinVipSeat(NetCmdBase obj)
    {
        LC_Cmd_CarRoleJoinVipSeat ncb = (LC_Cmd_CarRoleJoinVipSeat)obj;
        if (ncb.Result)
        {
            TableVipSeatInfo.List[ncb.VipSeatIndex] = ncb.DestUserInfo;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_JoinVip_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_JoinVip_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
        }

        tagCarVipSeatChangeEvent pEvent = new tagCarVipSeatChangeEvent(ncb.Result, TableVipSeatInfo);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    public bool OnHandleRoleLeaveVipSeat(NetCmdBase obj)
    {
        LC_Cmd_CarRoleLeaveVipSeat ncb = (LC_Cmd_CarRoleLeaveVipSeat)obj;
        for (Byte i = 0; i < FishDataInfo.MAX_Car_VipSeat; ++i)
        {
            if (TableVipSeatInfo.List[i] != null && TableVipSeatInfo.List[i].dwUserID == ncb.dwDestUserID)
            {
                TableVipSeatInfo.List[i].dwUserID = 0;
                TableVipSeatInfo.List[i].GlobelSum = 0;
                TableVipSeatInfo.List[i].NickName = "";
                TableVipSeatInfo.List[i].dwFaceID = 0;

                if (ncb.dwDestUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_LeaveVip_Sucess);
                    MsgEventHandle.HandleMsg(pUOM);
                }

                tagCarVipSeatChangeEvent pEvent = new tagCarVipSeatChangeEvent(true, TableVipSeatInfo);
                MsgEventHandle.HandleMsg(pEvent);

                return true;
            }
        }
        return true;
    }
    public bool OnHandleRoleBeLeaveVipSeat(NetCmdBase obj)
    {
        LC_Cmd_CarRoleBeLeaveVipSeat ncb = (LC_Cmd_CarRoleBeLeaveVipSeat)obj;
        tagCarBeLeaveVipSeatEvent pEvent = new tagCarBeLeaveVipSeatEvent(ncb.DestRoleInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }

    public bool OnHandleGetNormalSeatInfo(NetCmdBase obj)
    {
        LC_Cmd_CarGetNormalSeatInfo ncb = (LC_Cmd_CarGetNormalSeatInfo)obj;

        tagCarGetNormalSeatInfoEvent pEvent = new tagCarGetNormalSeatInfoEvent(ncb.Page, ncb.TotalRoleSum, ncb.Array);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleVipGetGlobel(NetCmdBase obj)
    {
        LC_Cmd_CarVipSeatGlobelChange ncb = (LC_Cmd_CarVipSeatGlobelChange)obj;
        TableVipSeatInfo.List[ncb.VipSeat].GlobelSum -= ncb.GlobelSum;

        tagCarVipGetGlobelEvent pEvent = new tagCarVipGetGlobelEvent(ncb.VipSeat, ncb.Index, ncb.GlobelSum, TableVipSeatInfo.List[ncb.VipSeat].GlobelSum);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleTableStopUpdate(NetCmdBase obj)
    {
        LC_Cmd_CarTableStopUpdate ncb = (LC_Cmd_CarTableStopUpdate)obj;

        tagCarTableStopUpdate pEvent = new tagCarTableStopUpdate();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }

    public bool OnHandleVipGlobelChange(NetCmdBase obj)
    {
        LC_Cmd_CarVipGlobelChange ncb = (LC_Cmd_CarVipGlobelChange)obj;
        for (Byte i = 0; i < TableVipSeatInfo.List.Length; ++i)
        {
            if (TableVipSeatInfo.List[i].dwUserID == ncb.VipUserID)
            {
                TableVipSeatInfo.List[i].GlobelSum = ncb.VipGlobelSum;

                tagCarVipGlobelChange pEvent = new tagCarVipGlobelChange(ncb.VipUserID, i, ncb.VipGlobelSum);
                MsgEventHandle.HandleMsg(pEvent);

                return true;
            }
        }
        return true;
    }
    public bool OnHandleRoleBetGlobelByLog(NetCmdBase obj)
    {
        LC_Cmd_CarRoleBetGlobelByLog ncb = (LC_Cmd_CarRoleBetGlobelByLog)obj;
        if (ncb.Result)
        {
            //继续下注成功了
            for (Byte i = 0; i < FishDataInfo.MAX_Car_ClientSum; ++i)
            {
                TableGlobelSum[i] += ncb.betGlobel[i];
                TableUpdateInfo.GlobelNum[i] += ncb.betGlobel[i];
                LogTableGlobelSum[i] = 0;
            }

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            //玩家继续下注失败 无法下注
            if (TableBankerUserInfo.UserInfo.dwUserID == 0)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_3);
                MsgEventHandle.HandleMsg(pUOM);
            }
            else
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Car_AddGlobel_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
            }
        }
        tagCarAddGlobel pEvent = new tagCarAddGlobel(TableStates, TableUpdateInfo, ncb.Result);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}