using UnityEngine;
using System.Collections.Generic;

public enum HallState
{
    HALL_WAIT,
    HALL_JOIN_ROOM,
    HALL_JOIN_FAILED,
}
public class HallLogic:ILogic, ICmdHandler
{
    HallLogicUI m_LogicUI;
    byte m_State;
    //bool b
    public bool Init(ILogicUI logicUI, object obj)
    {
        SceneObjMgr.Instance.SetShowType(ShowFlagType.SHOW_NONE);
        m_LogicUI = (HallLogicUI)logicUI;
        HallRunTimeInfo.Init(this);
        NetServices.Instance.CmdHandler = this;
        State = HallState.HALL_WAIT;
        return true;
    }
    public void ResetHall()
    {
        NetServices.Instance.CmdHandler = this;
    }
    public HallState State
    {
        get { return (HallState)m_State; }
        set { m_State = (byte)value; }
    }
    public void Update(float delta)
    {
        GlobalHallUIMgr.Instance.GameShare.Update();
    }
    public bool Handle(NetCmdPack pack)
    {
        if (pack.cmd.GetCmdType() == NetCmdType.CMD_JOIN_ROOM_RESULT)
        {
            NetCmdJoinRoomResult ncj = (NetCmdJoinRoomResult)pack.cmd;
            JoinRoomData ncg = new JoinRoomData();
            ncg.RoomID = ncj.RoomID;
            ncg.BackgroundImage = ncj.BackgroundImage;
            ncg.LauncherType = ncj.LauncherType;
            ncg.Seat = ncj.Seat;
            ncg.RateIndex = 0;
            if (ncj.Result == 0)
            {
                State = HallState.HALL_JOIN_FAILED;
                GlobalEffectMgr.Instance.CloseLoadingMessage();
            }
            else
            {
                NetServices.Instance.CmdHandler = null;
                LogicManager.Instance.Forward(ncg);
            }
            return true;
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_JoinTable)
        {
            LC_Cmd_JoinTableResult ncb = (LC_Cmd_JoinTableResult)pack.cmd;

            tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Table_JoinTable_Sucess : UserOperateMessage.UOM_Table_JoinTable_Failed_7));
            MsgEventHandle.HandleMsg(pUOM);

            if(ncb.Result)
            {
                //在玩家身上设置玩家桌子ID
                PlayerRole.Instance.RoleInfo.RoleMe.SetTableTypeID(ncb.bTableTypeID);

                NetServices.Instance.CmdHandler = null;

                PlayerRole.Instance.RoleGameData.OnHandleRoleJoinTable();//表示玩家已经进入房间了
                JoinRoomData ncg = new JoinRoomData();
                ncg.RoomID = ncb.bTableTypeID;
                ncg.BackgroundImage = ncb.BackgroundImage;
                ncg.LauncherType = ncb.LauncherType;
                ncg.Seat = ncb.SeatID;
                ncg.RateIndex = ncb.RateIndex;
                ncg.Energy = ncb.Energy;
                LogicManager.Instance.Forward(ncg);
            }
            else
            {
                State = HallState.HALL_JOIN_FAILED;
                GlobalEffectMgr.Instance.CloseLoadingMessage();
            }
            return true;
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_CHANGE_IMG)
        {
            NetCmdImgChange cmd = (NetCmdImgChange)pack.cmd;
            HeaderManager.Instance.GetPlayerHeader(cmd.PlayerID, cmd.ImgCrcValue, HeaderOptType.HEADER_FRIEND, null);
            return true;
        }
        return false;
    }
    public bool CanProcessCmd()
    {
        return true;
    }
    public void JoinRoom(byte roomid)
    {
        TableError pError = PlayerRole.Instance.TableManager.IsCanJoinTable(roomid, false);
        if (pError != TableError.TE_Sucess)
        {
            //进入房间失败了 我们进行处理 
            tagJoinTableEvent pEvent= new tagJoinTableEvent(roomid,pError);
            MsgEventHandle.HandleMsg(pEvent);//无法进入房间的事件
            return;
        }
        GlobalEffectMgr.Instance.ShowLoadingMessage();

        //发送进入房间的命令到服务端去
        if (SceneMain.IsUserOldServer)
        {
            NetCmdJoinRoom ncr = new NetCmdJoinRoom();
            ncr.SetCmdType(NetCmdType.CMD_JOIN_ROOM);
            ncr.RoomType = roomid;
            NetServices.Instance.Send<NetCmdJoinRoom>(ncr);
        }
        else
        {
            CL_Cmd_JoinTable ncb = new CL_Cmd_JoinTable();
            ncb.SetCmdType(NetCmdType.CMD_CL_JoinTable);
            ncb.bTableType = roomid;
            NetServices.Instance.Send<CL_Cmd_JoinTable>(ncb);
        }
        State = HallState.HALL_JOIN_ROOM;
    }
    public void JoinMonth(byte MonthID)
    {
        //进入比赛的桌子
        if (!PlayerRole.Instance.MonthManager.JoinMonth(MonthID))
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("jointableerror"), 2, false);
            return;
        }
    }

    public void StateChanged(NetState state)
    {
        if(state == NetState.NET_DISCONNECT)
        {
            GlobalLogon.Instance.ReconnectServer();
            //GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("disconnect"), 2, false);
            //LogicManager.Instance.LogOff();
        }
    }
    public void Shutdown()
    {
     
        HallRunTimeInfo.Instance.ShutDown(0);
    }
}
