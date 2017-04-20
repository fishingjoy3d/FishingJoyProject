using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
public enum NiuNiuTableStates
{
    NNTS_Begin = 1,//下注阶段
    NNTS_End = 2,//开牌阶段
    NNTS_WriteBegin = 3,//等待开始
};
public class TableUpdateInfo
{
    public UInt64[] GlobelNum;
    public TableUpdateInfo()
    {
        GlobelNum = new UInt64[FishDataInfo.MAX_NIUNIU_ClientSum];
    }
    public void SetInfo(UInt64[] GlobelNum)
    {
        for(Byte i=0;i<FishDataInfo.MAX_NIUNIU_ClientSum;++i)
        {
            this.GlobelNum[i] = GlobelNum[i];
        }
    }
}
public class TableBrandResult
{
     public Byte[] BrandArray;
     public UInt16[] BrandResult;
    public TableBrandResult()
     {
         BrandArray = new Byte[(FishDataInfo.MAX_NIUNIU_ClientSum + 1) * FishDataInfo.MAX_NIUNIU_BrandSum];
         BrandResult = new UInt16[FishDataInfo.MAX_NIUNIU_ClientSum + 1];
     }
     public void SetInfo(Byte[] BrandArray, UInt16[] BrandResult)
     {
         for(Byte i=0;i<FishDataInfo.MAX_NIUNIU_ClientSum +1;++i)
         {
             this.BrandResult[i] = BrandResult[i];
             for(Byte j=0;j<FishDataInfo.MAX_NIUNIU_BrandSum;++j)
             {
                 this.BrandArray[i * FishDataInfo.MAX_NIUNIU_BrandSum + j] = BrandArray[i * FishDataInfo.MAX_NIUNIU_BrandSum + j];
             }
         }
     }
}
public class BankerUserInfo
{
    public NiuNiuRoleInfo UserInfo;//庄家数据
    public Byte GameSum;//已经进行的游戏盘数
    public BankerUserInfo()
    {
        UserInfo = new NiuNiuRoleInfo();
        GameSum = 0;
    }
    public void SetInfo(NiuNiuRoleInfo UserInfo,Byte GameSum)
    {
        this.UserInfo = UserInfo;
        this.GameSum = GameSum;
    }
}
public class WriteBankerList
{
    public UInt32 MySeatIndex;
    public NiuNiuRoleInfo[] List;
    public WriteBankerList()
    {
        MySeatIndex = 0xffffffff;
        List = new NiuNiuRoleInfo[FishDataInfo.MAX_NIUNIU_BankerShowSum];
    }
    public void SetInfo(UInt32 MySeatIndex,NiuNiuRoleInfo[] List)
    {
        this.MySeatIndex = MySeatIndex;
        for (Byte i = 0; i < FishDataInfo.MAX_NIUNIU_BankerShowSum;++i )
            this.List[i] = List[i];
    }
}
public class VipSeatList
{
    public NiuNiuRoleInfo[] List;
    public VipSeatList()
    {
        List = new NiuNiuRoleInfo[FishDataInfo.MAX_NIUNIU_VipSeat];
    }
    public void SetInfo(NiuNiuRoleInfo[] List)
    {
        for (Byte i = 0; i < FishDataInfo.MAX_NIUNIU_VipSeat; ++i)
            this.List[i] = List[i];
    }
}
public class NiuNiuGameLog
{
    public Byte GameSum;
    public UInt32 GameLog;
    
    public NiuNiuGameLog()
    {
        GameSum = 0;
        GameLog = 0;
    }
}
class RoleNiuNiu
{
    //牛牛客户端基本数据
    private NiuNiuTableStates TableStates;
    private TableUpdateInfo TableUpdateInfo = new TableUpdateInfo();
    private TableBrandResult BrandResult = new TableBrandResult();
    private BankerUserInfo TableBankerUserInfo = new BankerUserInfo();
    private WriteBankerList TableWriteBankerList = new WriteBankerList();
    private UInt32[] TableGlobelSum = new UInt32[FishDataInfo.MAX_NIUNIU_ClientSum];
    private VipSeatList TableVipSeatInfo = new VipSeatList();
    private bool IsInBankerList = false;
    private NiuNiuGameLog TableGameLog = new NiuNiuGameLog();
    public RoleNiuNiu()
    {
        for (int i = 0; i < FishDataInfo.MAX_NIUNIU_ClientSum; ++i)
            TableGlobelSum[i] = 0;
    }
    public UInt32 GetRoleAddAllGlobelSum()
    {
        UInt32 All = 0;
        for (Byte i = 0; i < FishDataInfo.MAX_NIUNIU_ClientSum; ++i)
            All += TableGlobelSum[i];
        return All;
    }

    //牛牛 管理类
    //进入牛牛房间
    public bool OnJoinRoom()
    {
        //1.玩家金币大于多少才可以进入
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.JoinGlobelSum)
        {
            //提示玩家进入 牛牛 金币过少 无法进入
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_JoinRoom_Failed_1, FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.JoinGlobelSum.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_RoleJoinTable msg = new CL_Cmd_RoleJoinTable();
        msg.SetCmdType(NetCmdType.CMD_CL_RoleJoinTable);
        NetServices.Instance.Send<CL_Cmd_RoleJoinTable>(msg);
        return true;
    }
    //离开牛牛房间
    public void OnLeaveNiuNiuTable()
    {
        TableUpdateInfo = new TableUpdateInfo();
        BrandResult = new TableBrandResult();
        TableBankerUserInfo = new BankerUserInfo();
        TableWriteBankerList = new WriteBankerList();
        TableVipSeatInfo = new VipSeatList();
        TableGameLog = new NiuNiuGameLog();
        IsInBankerList = false;
        for (int i = 0; i < FishDataInfo.MAX_NIUNIU_ClientSum; ++i)
            TableGlobelSum[i] = 0;
        //玩家离开桌子 
        CL_Cmd_RoleLeaveTable ncb = new CL_Cmd_RoleLeaveTable();
        ncb.SetCmdType(NetCmdType.CMD_CL_RoleLeaveTable);
        NetServices.Instance.Send<CL_Cmd_RoleLeaveTable>(ncb);
    }
    //下注
    public bool OnAddGlobel(UInt32 AddGlobel,Byte Index)
    {
        //下注金币
        if(Index >= FishDataInfo.MAX_NIUNIU_ClientSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddGlobel_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        UInt32 AllGlobelSum = GetRoleAddAllGlobelSum();

        if (AllGlobelSum + AddGlobel > FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.MaxAddGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddGlobel_Failed_3, FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.MaxAddGlobel.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if ((AllGlobelSum + AddGlobel) * FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.MaxRate > (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() + AllGlobelSum))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddGlobel_Failed_2, FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.MaxRate.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID() == TableBankerUserInfo.UserInfo.dwUserID)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddGlobel_Failed_6);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        UInt64 TableAllGlobel = 0;
        for (Byte i = 0; i < FishDataInfo.MAX_NIUNIU_ClientSum; ++i)
            TableAllGlobel += TableUpdateInfo.GlobelNum[i];

        if (TableBankerUserInfo.UserInfo.dwUserID !=0 && (TableAllGlobel + AddGlobel) * FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.MaxRate > TableBankerUserInfo.UserInfo.GlobelSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddGlobel_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_RoleBetGlobel ncb = new CL_Cmd_RoleBetGlobel();
        ncb.SetCmdType(NetCmdType.CMD_CL_RoleBetGlobel);
        ncb.Index = Index;
        ncb.AddGlobel = AddGlobel;
        NetServices.Instance.Send<CL_Cmd_RoleBetGlobel>(ncb);
        return true;
    }
    //获取庄家排队列表
    public void OnGetWriteBankerList()
    {
        CL_Cmd_GetBankerList ncb = new CL_Cmd_GetBankerList();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetBankerList);
        NetServices.Instance.Send<CL_Cmd_GetBankerList>(ncb);
    }
    //玩家上庄
    public bool OnRoleJoinWriteBankerList()
    {
        //玩家上庄
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.JoinBankerGlobelSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddBankerList_Failed_1, FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.JoinBankerGlobelSum.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(TableWriteBankerList.MySeatIndex != 0xffffffff)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddBankerList_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }

        CL_Cmd_RoleJoinBankerList ncb = new CL_Cmd_RoleJoinBankerList();
        ncb.SetCmdType(NetCmdType.CMD_CL_RoleJoinBankerList);
        NetServices.Instance.Send<CL_Cmd_RoleJoinBankerList>(ncb);
        return true;
    }
    //玩家抢庄
    public bool OnRoleGetWriteBankerFirstSeat()
    {
        //玩家抢庄
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.GetNextBankerNeedGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_NextBankerSeat_Failed_2, FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.GetNextBankerNeedGlobel.ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (TableWriteBankerList.MySeatIndex == 0xffffffff)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_NextBankerSeat_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(TableWriteBankerList.List[0].dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_NextBankerSeat_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_RoleGetBankerFirstSeat ncb = new CL_Cmd_RoleGetBankerFirstSeat();
        ncb.SetCmdType(NetCmdType.CMD_CL_RoleGetBankerFirstSeat);
        NetServices.Instance.Send<CL_Cmd_RoleGetBankerFirstSeat>(ncb);
        return true;
    }
    //玩家离开上庄队列
    public bool OnRoleLeaveWriteBankerList()
    {
        if (TableWriteBankerList.MySeatIndex == 0xffffffff)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_LeaveBankerList_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_RoleLeaveBankerList ncb = new CL_Cmd_RoleLeaveBankerList();
        ncb.SetCmdType(NetCmdType.CMD_CL_RoleLeaveBankerList);
        NetServices.Instance.Send<CL_Cmd_RoleLeaveBankerList>(ncb);
        return true;
    }
    //玩家下庄
    public bool OnRoleCanelBanker()
    {
        if(TableBankerUserInfo.UserInfo.dwUserID == 0 || TableBankerUserInfo.UserInfo.dwUserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_CanelBanker_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_RoleCanelBankerSeat ncb = new CL_Cmd_RoleCanelBankerSeat();
        ncb.SetCmdType(NetCmdType.CMD_CL_RoleCanelBankerSeat);
        NetServices.Instance.Send<CL_Cmd_RoleCanelBankerSeat>(ncb);
        return true;
    }
    //玩家在VIP的位置坐下
    public bool OnRoleJoinVipSeat(Byte VipSeatID)
    {
        if (VipSeatID >= FishDataInfo.MAX_NIUNIU_VipSeat)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_JoinVip_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_JoinVip_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (TableBankerUserInfo.UserInfo.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_JoinVip_Failed_5);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        for(Byte i =0;i<FishDataInfo.MAX_NIUNIU_VipSeat;++i)
        {
            if(TableVipSeatInfo.List[i] != null && TableVipSeatInfo.List[i].dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_JoinVip_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }
      
        CL_Cmd_RoleJoinVipSeat msg = new CL_Cmd_RoleJoinVipSeat();
        msg.SetCmdType(NetCmdType.CMD_CL_RoleJoinVipSeat);
        msg.VipSeatIndex = VipSeatID;
        NetServices.Instance.Send<CL_Cmd_RoleJoinVipSeat>(msg);
        return true;
    }
    public bool OnRoleLeaveVipSeat()
    {
        bool IsInVipSeat = false;
        //玩家从VIP席位移动到 普通席位
        for(Byte i =0;i<FishDataInfo.MAX_NIUNIU_VipSeat;++i)
        {
            if (TableVipSeatInfo.List[i] != null && TableVipSeatInfo.List[i].dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            {
                IsInVipSeat = true;
                break;
            }
        }
        if(!IsInVipSeat)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_LeaveVip_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_RoleLeaveVipSeat ncb = new CL_Cmd_RoleLeaveVipSeat();
        ncb.SetCmdType(NetCmdType.CMD_LC_RoleLeaveVipSeat);
        NetServices.Instance.Send<CL_Cmd_RoleLeaveVipSeat>(ncb);
        return true;
    }
    public NiuNiuGameLog GetGameLog()
    {
        return TableGameLog;
    }

    public void OnGetNormalInfo(UInt32 Page)
    {
        CL_Cmd_GetNormalSeatInfo msg = new CL_Cmd_GetNormalSeatInfo();
        msg.SetCmdType(NetCmdType.CMD_CL_GetNormalSeatInfo);
        msg.Page = Page;
        NetServices.Instance.Send<CL_Cmd_GetNormalSeatInfo>(msg);
    }
    //服务器回发命令处理 
    public bool OnHandleRoleJoinTable(NetCmdBase obj)
    {
        LC_Cmd_RoleJoinTable ncb = (LC_Cmd_RoleJoinTable)obj;
        if (ncb.Result)
        {
            //赋值 将消息里携带的数据 赋值到结构里面去
            TableStates = (NiuNiuTableStates)ncb.TableStates;
            TableUpdateInfo.SetInfo(ncb.TableBetAreaGlobel);
            BrandResult.SetInfo(ncb.TableBrandArray, ncb.TableBrandResult);
            TableBankerUserInfo.SetInfo(ncb.TableBankerInfo, ncb.TableBankerUseGameSum);
            TableWriteBankerList.SetInfo(0xffffffff, ncb.TableWriteBankerList);
            TableVipSeatInfo.SetInfo(ncb.VipSeatList);
            for (Byte i = 0; i < FishDataInfo.MAX_NIUNIU_ClientSum; ++i)
                TableGlobelSum[i] = 0;
            IsInBankerList = false;

            TableGameLog.GameLog = ncb.TableResultLog;
            TableGameLog.GameSum = ncb.TableGameSum;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_JoinRoom_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            tagNiuNiuJoin pEvent = new tagNiuNiuJoin(ncb.TableStatesUpdateSec, TableStates, TableUpdateInfo, BrandResult, TableBankerUserInfo, TableWriteBankerList,TableVipSeatInfo,TableGameLog);
            MsgEventHandle.HandleMsg(pEvent);

            //string str = string.Format("排队列表变化 : ({0}:{1}) ({2}:{3}) ({4}:{5}) ({6}:{7}) ({8}:{9})",
            //  ncb.TableWriteBankerList[0].dwUserID, ncb.TableWriteBankerList[0].GlobelSum,
            //  ncb.TableWriteBankerList[1].dwUserID, ncb.TableWriteBankerList[1].GlobelSum,
            //  ncb.TableWriteBankerList[2].dwUserID, ncb.TableWriteBankerList[2].GlobelSum,
            //  ncb.TableWriteBankerList[3].dwUserID, ncb.TableWriteBankerList[3].GlobelSum,
            //  ncb.TableWriteBankerList[4].dwUserID, ncb.TableWriteBankerList[4].GlobelSum,
            //  ncb.TableWriteBankerList[5].dwUserID, ncb.TableWriteBankerList[5].GlobelSum,
            //  ncb.TableWriteBankerList[6].dwUserID, ncb.TableWriteBankerList[6].GlobelSum,
            //  ncb.TableWriteBankerList[7].dwUserID, ncb.TableWriteBankerList[7].GlobelSum);
            //LogMgr.Log(str);
            //string str = string.Format("游戏记录 局数:{0} 数据:{1}", TableGameLog.GameSum, TableGameLog.GameLog);
            //LogMgr.Log(str);
        }
        else
        {
            //进入房间失败了
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_JoinRoom_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleAddGlobel(NetCmdBase obj)
    {
        LC_Cmd_RoleBetGlobel ncb = (LC_Cmd_RoleBetGlobel)obj;
        if (ncb.Result)
        {
            TableUpdateInfo.GlobelNum[ncb.Index] += ncb.AddGlobel;
            TableGlobelSum[ncb.Index] += ncb.AddGlobel;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddGlobel_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            if(TableBankerUserInfo.UserInfo.dwUserID == 0)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddGlobel_Failed_3);
                MsgEventHandle.HandleMsg(pUOM);
            }
            else
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddGlobel_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
            }

            //tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddGlobel_Failed_5);
            //MsgEventHandle.HandleMsg(pUOM);
        }
        tagNiuNiuAddGlobel pEvent = new tagNiuNiuAddGlobel(TableStates, TableUpdateInfo, BrandResult, ncb.Result);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    public bool OnHandleGetWritebankerList(NetCmdBase obj)
    {
        LC_Cmd_GetBankerList ncb = (LC_Cmd_GetBankerList)obj;
        TableWriteBankerList.SetInfo(ncb.dwMySeatIndex, ncb.TableWriteBankerList);

        //string str = string.Format("排队列表变化 : ({0}:{1}) ({2}:{3}) ({4}:{5}) ({6}:{7}) ({8}:{9})", 
        //    ncb.TableWriteBankerList[0].dwUserID, ncb.TableWriteBankerList[0].GlobelSum,
        //    ncb.TableWriteBankerList[1].dwUserID,ncb.TableWriteBankerList[1].GlobelSum,
        //    ncb.TableWriteBankerList[2].dwUserID, ncb.TableWriteBankerList[2].GlobelSum,
        //    ncb.TableWriteBankerList[3].dwUserID,ncb.TableWriteBankerList[3].GlobelSum,
        //    ncb.TableWriteBankerList[4].dwUserID,ncb.TableWriteBankerList[4].GlobelSum,
        //    ncb.TableWriteBankerList[5].dwUserID, ncb.TableWriteBankerList[5].GlobelSum,
        //    ncb.TableWriteBankerList[6].dwUserID,ncb.TableWriteBankerList[6].GlobelSum,
        //    ncb.TableWriteBankerList[7].dwUserID, ncb.TableWriteBankerList[7].GlobelSum);
        //LogMgr.Log(str);

        //触发事件
        tagNiuNiuUpdateWriteBankerList pEvent = new tagNiuNiuUpdateWriteBankerList(TableWriteBankerList);
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
        LC_Cmd_BankerUserChange ncb = (LC_Cmd_BankerUserChange)obj;
        TableBankerUserInfo.SetInfo(ncb.BankerUserInfo, ncb.GameSum);
        //触发庄家变化的事件
        tagNiuNiuBankerUserChange pEvent = new tagNiuNiuBankerUserChange(TableBankerUserInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleTableJoinBegin(NetCmdBase obj)
    {
        TableStates = NiuNiuTableStates.NNTS_Begin;
        TableUpdateInfo = new TableUpdateInfo();
        BrandResult = new TableBrandResult();
        for (int i = 0; i < FishDataInfo.MAX_NIUNIU_ClientSum; ++i)
            TableGlobelSum[i] = 0;
        TableBankerUserInfo.GameSum++;
        //触发事件
        tagNiuNiuStatesChange pEvent = new tagNiuNiuStatesChange(TableStates, TableUpdateInfo, BrandResult,null, TableVipSeatInfo, TableWriteBankerList, TableBankerUserInfo, TableGameLog);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleUpdateTable(NetCmdBase obj)
    {
        LC_Cmd_TableUpdate ncb = (LC_Cmd_TableUpdate)obj;

        TableStates = NiuNiuTableStates.NNTS_Begin;//只有在开始阶段 才会进行更新
        TableUpdateInfo.SetInfo(ncb.TableBetAreaGlobel);
        //事件
        tagNiuNiuTableInfoChange pEvent = new tagNiuNiuTableInfoChange(TableStates, TableUpdateInfo);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    public bool OnHandleTableJoinEnd(NetCmdBase obj)
    {
        LC_Cmd_TableJoinEnd ncb = (LC_Cmd_TableJoinEnd)obj;

        BrandResult.SetInfo(ncb.TableBrandArray, ncb.TableBrandResult);
        TableUpdateInfo.SetInfo(ncb.TableBetAreaGlobel);
        TableStates = NiuNiuTableStates.NNTS_End;//等待关闭状态

        for (int i = 0; i < FishDataInfo.MAX_NIUNIU_ClientSum; ++i)
            TableGlobelSum[i] = 0;

        //金币变化
        if(ncb.BankerUserGlobelInfo.dwUserID == TableBankerUserInfo.UserInfo.dwUserID)
        {
            TableBankerUserInfo.UserInfo.GlobelSum = ncb.BankerUserGlobelInfo.dwGlobelSum;
        }

        for(Byte i =0;i<FishDataInfo.MAX_NIUNIU_BankerShowSum;++i)
        {
            if(TableWriteBankerList.List[i] != null && ncb.BankerListGlobelInfo[i].dwUserID == TableWriteBankerList.List[i].dwUserID)
            {
                TableWriteBankerList.List[i].GlobelSum = ncb.BankerListGlobelInfo[i].dwGlobelSum;
            }
        }
        VipSeatList pOldlist = new VipSeatList();
        for (Byte i = 0; i < FishDataInfo.MAX_NIUNIU_VipSeat; ++i)
        {
            if (TableVipSeatInfo.List[i] != null && ncb.VipGlobelInfo[i].dwUserID == TableVipSeatInfo.List[i].dwUserID)
            {
                pOldlist.List[i] = TableVipSeatInfo.List[i];

                TableVipSeatInfo.List[i].GlobelSum = ncb.VipGlobelInfo[i].dwGlobelSum;
            }
        }

        TableGameLog.GameLog = ncb.TableResultLog;
        TableGameLog.GameSum = ncb.TableGameSum;

        //触发事件
        tagNiuNiuStatesChange pEvent = new tagNiuNiuStatesChange(TableStates, TableUpdateInfo, BrandResult,pOldlist, TableVipSeatInfo, TableWriteBankerList, TableBankerUserInfo, TableGameLog);
        MsgEventHandle.HandleMsg(pEvent);

        //string str = string.Format("游戏记录 局数:{0} 数据:{1}", TableGameLog.GameSum, TableGameLog.GameLog);
        //LogMgr.Log(str);
        return true;
    }
    public bool OnHandleJoinWriteBankerList(NetCmdBase obj)
    {
        LC_Cmd_RoleJoinBankerList ncb = (LC_Cmd_RoleJoinBankerList)obj;
        if (ncb.Result)
        {
            TableWriteBankerList.MySeatIndex = ncb.SeatIndex;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddBankerList_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            tagNiuNiuUpdateWriteBankerList pEvent = new tagNiuNiuUpdateWriteBankerList(TableWriteBankerList);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_AddBankerList_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleRoleGetWriteBankerFirstSeat(NetCmdBase obj)
    {
        LC_Cmd_RoleGetBankerFirstSeat ncb = (LC_Cmd_RoleGetBankerFirstSeat)obj;
        if (ncb.Result)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_NextBankerSeat_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_NextBankerSeat_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleRoleLeaveWriteBankerList(NetCmdBase obj)
    {
        LC_Cmd_RoleLeaveBankerList ncb = (LC_Cmd_RoleLeaveBankerList)obj;
        if (ncb.Result)
        {
            TableWriteBankerList.MySeatIndex = 0xffffffff;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_LeaveBankerList_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            tagNiuNiuUpdateWriteBankerList pEvent = new tagNiuNiuUpdateWriteBankerList(TableWriteBankerList);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_LeaveBankerList_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleRoleCanelBanker(NetCmdBase obj)
    {
        LC_Cmd_RoleCanelBankerSeat ncb = (LC_Cmd_RoleCanelBankerSeat)obj;
        if(ncb.Result)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_CanelBanker_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_CanelBanker_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    public bool OnHandleBankerUserGlobelChange(NetCmdBase obj)
    {
        LC_Cmd_BankerUserGlobelChange ncb = (LC_Cmd_BankerUserGlobelChange)obj;
        if (TableBankerUserInfo.UserInfo.dwUserID != ncb.dwBankerUserID)
            return false;
        TableBankerUserInfo.UserInfo.GlobelSum = ncb.dwBankerGlobelSum;
        //触发事件
        tagNiuNiuBankerUserChange pEvent = new tagNiuNiuBankerUserChange(TableBankerUserInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleWriteBankerUserGlobelMsg(NetCmdBase obj)
    {
        //触发事件 玩家上单后金币不够 的提示
        tagNiuNiuWriteBankerGlobelMsg pEvent = new tagNiuNiuWriteBankerGlobelMsg();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleRoleJoinVipSeat(NetCmdBase obj)
    {
        LC_Cmd_RoleJoinVipSeat ncb = (LC_Cmd_RoleJoinVipSeat)obj;
        if(ncb.Result)
        {
            TableVipSeatInfo.List[ncb.VipSeatIndex] = ncb.DestUserInfo;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_JoinVip_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_JoinVip_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
        }

        tagNiuNiuVipSeatChangeEvent pEvent = new tagNiuNiuVipSeatChangeEvent(ncb.Result,TableVipSeatInfo);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    public bool OnHandleRoleLeaveVipSeat(NetCmdBase obj)
    {
        LC_Cmd_RoleLeaveVipSeat ncb = (LC_Cmd_RoleLeaveVipSeat)obj;
        for(Byte i =0;i<FishDataInfo.MAX_NIUNIU_VipSeat;++i)
        {
            if (TableVipSeatInfo.List[i] != null && TableVipSeatInfo.List[i].dwUserID == ncb.dwDestUserID)
            {
                TableVipSeatInfo.List[i].dwUserID = 0;
                TableVipSeatInfo.List[i].GlobelSum = 0;
                TableVipSeatInfo.List[i].NickName = "";
                TableVipSeatInfo.List[i].dwFaceID = 0;

                if (ncb.dwDestUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_LeaveVip_Sucess);
                    MsgEventHandle.HandleMsg(pUOM);
                }

                tagNiuNiuVipSeatChangeEvent pEvent = new tagNiuNiuVipSeatChangeEvent(true,TableVipSeatInfo);
                MsgEventHandle.HandleMsg(pEvent);

                return true;
            }
        }
        //if (ncb.dwDestUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        //{
        //    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_NiuNiu_LeaveVip_Failed_2);
        //    MsgEventHandle.HandleMsg(pUOM);
        //}
        return true;
    }
    public bool OnHandleRoleBeLeaveVipSeat(NetCmdBase obj)
    {
        LC_Cmd_RoleBeLeaveVipSeat ncb = (LC_Cmd_RoleBeLeaveVipSeat)obj;
        tagNiuNiuBeLeaveVipSeatEvent pEvent = new tagNiuNiuBeLeaveVipSeatEvent(ncb.DestRoleInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }

    public bool OnHandleGetNormalSeatInfo(NetCmdBase obj)
    {
        LC_Cmd_GetNormalSeatInfo ncb = (LC_Cmd_GetNormalSeatInfo)obj;

        tagNiuNiuGetNormalSeatInfoEvent pEvent = new tagNiuNiuGetNormalSeatInfoEvent(ncb.Page,ncb.TotalRoleSum, ncb.Array);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleVipGetGlobel(NetCmdBase obj)
    {
        LC_Cmd_VipSeatGlobelChange ncb = (LC_Cmd_VipSeatGlobelChange)obj;
        TableVipSeatInfo.List[ncb.VipSeat].GlobelSum -= ncb.GlobelSum;

        tagNiuNiuVipGetGlobelEvent pEvent = new tagNiuNiuVipGetGlobelEvent(ncb.VipSeat, ncb.Index, ncb.GlobelSum, TableVipSeatInfo.List[ncb.VipSeat].GlobelSum);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    public bool OnHandleTableStopUpdate(NetCmdBase obj)
    {
        LC_Cmd_TableStopUpdate ncb = (LC_Cmd_TableStopUpdate)obj;

        tagNiuNiuTableStopUpdate pEvent = new tagNiuNiuTableStopUpdate();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }

    public bool OnHandleVipGlobelChange(NetCmdBase obj)
    {
        LC_Cmd_VipGlobelChange ncb = (LC_Cmd_VipGlobelChange)obj;
        for(Byte i =0;i<TableVipSeatInfo.List.Length;++i)
        {
            if(TableVipSeatInfo.List[i].dwUserID == ncb.VipUserID)
            {
                TableVipSeatInfo.List[i].GlobelSum = ncb.VipGlobelSum;

                tagNiuNiuVipGlobelChange pEvent = new tagNiuNiuVipGlobelChange(ncb.VipUserID, i, ncb.VipGlobelSum);
                MsgEventHandle.HandleMsg(pEvent);

                return true;
            }
        }
        return true;
    }
}