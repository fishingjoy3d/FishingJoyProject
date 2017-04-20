using UnityEngine;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class GlobalLogon : Singleton<GlobalLogon>
{
    private AccountData m_AccountData = new AccountData();
    private PhoneLogonData m_PhoneLogonData;

    volatile bool m_bConnecting = false;
    public AccountData AccountData
    {
        get { return m_AccountData; }
        set { m_AccountData = value; }
    }

    public PhoneLogonData PhoneLogonData
    {
        get { return m_PhoneLogonData; }
        set { m_PhoneLogonData = value; }
    }

    private uint m_OnlyID;

    public uint OnlyID
    {
        get { return m_OnlyID; }
        set { m_OnlyID = value; }
    }

    System.Collections.IEnumerator WaitConnect(object obj)
    {
        while (m_bConnecting)
        {
            yield return new WaitForSeconds(0.001f);
        }
        if (!NetServices.Instance.IsConnected)
        {
            LogicManager.Instance.LogOff();
            LogonLogic.ShowLogonErrMsg();
            yield break;
        }
        else
        {
            //连接成功
            ResetLogonGameServer();
            yield break;
        }
    }
    void StartConnect()
    {
        try
        {
            NetServices.Instance.Connect(false, ServerSetting.HallServerIP, ServerSetting.HallServerPort, ServerSetting.NewIP, ServerSetting.NewPort);
        }
        catch
        {

        }
        m_bConnecting = false;
    }
    public void ReconnectServer()
    {

        GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("ReConnect"), 15, false);
        m_bConnecting = true;
        System.Threading.Thread t = new System.Threading.Thread(new System.Threading.ThreadStart(StartConnect));
        t.Start();
        SceneMain.Instance.StartInnerCoroutine(WaitConnect(null));
    }
    //用于全局登陆处理的类
    public void ResetLogonGameServer()
    {
        //2.在连接成功后 发送命令到GameServer去 
        CL_Cmd_ResetLogonGameServer ncb = new CL_Cmd_ResetLogonGameServer();
        ncb.SetCmdType(NetCmdType.CMD_CL_ResetLogonGameServer);
        ncb.UserID = PlayerRole.Instance.RoleInfo.RoleMe.GetUserID();
        ncb.RandID = m_OnlyID;
        ncb.VersionID = ServerSetting.ClientVer;
        ncb.PathCrc = ServerSetting.RES_VERSION;
        NetServices.Instance.Send<CL_Cmd_ResetLogonGameServer>(ncb);//将命令发送到GameServer
    }
    public bool Handle(NetCmdPack pack)
    {
        //登陆的后续命令处理
        if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_AccountOnlyIDSuccess)
        {
            //玩家登陆GameServer成功了
            LC_Cmd_AccountOnlyIDSuccess ncb = (LC_Cmd_AccountOnlyIDSuccess)pack.cmd;
            //保存玩家账号数据 登陆成功的时候
            PlayerRole.Instance.RoleInfo.RoleMe.SetRoleInfo(ncb.RoleInfo);//设置玩家的数据.
            ServerSetting.SetCallbckUrl(Utility.IPToString(ncb.OperateIP));
            //设置玩家身份标志
            if (m_AccountData != null)
            {
                if (PlayerRole.Instance.RoleInfo.RoleMe.GetIsCanResetAccount())
                    m_AccountData.TAG = 1;
                else
                    m_AccountData.TAG = 0;
                m_AccountData.SaveAccountData();
                //SaveAccountData(m_AccountData);//将账号数据保存起来
            }
            // NetServices.Instance.CmdHandler = null;
           // RuntimeData.MyAccount = m_AccountData;
            m_OnlyID = ncb.RandID;//用于登陆的唯一的ID
            PlayerRole.Instance.RoleExChange.SetPlayerRoleIsShowExChange();
            //PlayerRole.Instance.IsNeedResetTable = false;
            LogicManager.Instance.SkipToHall();
            PlayerRole.Instance.OnRoleResetOtherInfo();//清空数据并且提供事件

        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_AccountLogonToScreen)
        {
            //玩家登陆GameServer成功了
            LC_Cmd_AccountLogonToScreen ncb = (LC_Cmd_AccountLogonToScreen)pack.cmd;
            //保存玩家账号数据 登陆成功的时候
            //PlayerRole.Instance.RoleInfo.RoleMe.SetRoleInfo(ncb.RoleInfo);//设置玩家的数据.
            //设置玩家身份标志
            if (m_AccountData != null)
            {
                if (PlayerRole.Instance.RoleInfo.RoleMe.GetIsCanResetAccount())
                    GlobalLogon.Instance.AccountData.TAG = 1;
                else
                    GlobalLogon.Instance.AccountData.TAG = 0;
                m_AccountData.SaveAccountData();
                //SaveAccountData(m_AccountData);//将账号数据保存起来
                // NetServices.Instance.CmdHandler = null;
            }
			//RuntimeData.MyAccount = m_AccountData;
            m_OnlyID = ncb.RandID;//用于登陆的唯一的ID

           // PlayerRole.Instance.IsNeedResetTable = true;
            //直接进入场景
            JoinRoomData ncg    = new JoinRoomData();
            ncg.RoomID          = ncb.bTableTypeID;
            ncg.BackgroundImage = ncb.BackgroundImage;
            ncg.LauncherType    = ncb.LauncherType;
            ncg.Seat            = ncb.SeatID;
            ncg.RateIndex       = ncb.RateIndex;
            ncg.Energy          = ncb.Energy;
            //场景
            LogicManager.Instance.SkipToScene(ncg);
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_ResetLogonGameServer)
        {
            //玩家重新连接服务器失败了 让玩家返回登陆界面去
            LogicManager.Instance.LogOff();
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_Reset_Error);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_JoinTable)
        {
            LC_Cmd_JoinTableResult ncb = (LC_Cmd_JoinTableResult)pack.cmd;
            if (ncb.Result)
            {
                PlayerRole.Instance.RoleInfo.RoleMe.SetTableTypeID(ncb.bTableTypeID);

                JoinRoomData ncg = new JoinRoomData();
                ncg.RoomID = ncb.bTableTypeID;
                ncg.BackgroundImage = ncb.BackgroundImage;
                ncg.LauncherType = ncb.LauncherType;
                ncg.Seat = ncb.SeatID;
                ncg.RateIndex = ncb.RateIndex;
                ncg.Energy = ncb.Energy;
                SceneRuntime.SceneLogic.ResetPlayerData(ncg, false);
                //LogicManager.Instance.Forward(ncg);
            }
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_ServerChangeSocket)
        {
            //提示玩家已经在其他地方登陆了 被踢掉了
            LogicManager.Instance.LogOff();
            string str = StringTable.GetString("Account_OtherLogon");
            if(str != null)
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(str, 5, false);
        }
        //else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_ResetPassword)
        //{
        //    LC_Cmd_ResetPassword ncb = (LC_Cmd_ResetPassword)pack.cmd;
        //    if(ncb.Result)
        //    {
        //        GlobalLogon.Instance.AccountData.CRC1 = ncb.NewPasswordCrc1;
        //        GlobalLogon.Instance.AccountData.CRC2 = ncb.NewPasswordCrc2;
        //        GlobalLogon.Instance.AccountData.CRC3 = ncb.NewPasswordCrc3;

        //        tagErrorCodeEvent pEvent = new tagErrorCodeEvent(ErrorCode.EC_ResetPassword_Sucess);
        //        MsgEventHandle.HandleMsg(pEvent);
        //    }
        //    else
        //    {
        //        tagErrorCodeEvent pEvent = new tagErrorCodeEvent(ErrorCode.EC_ResetPassword_Failed);
        //        MsgEventHandle.HandleMsg(pEvent);
        //    }
        //    return true;
        //}
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_CheckVersionError)
        {
            LogicManager.Instance.LogOff();
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_Version_Error);
            MsgEventHandle.HandleMsg(pUOM);
            return true;
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_LogonQueueWrite)
        {
            //排队的消息  我们让客户端继续等待
            LC_Cmd_LogonQueueWrite ncb = (LC_Cmd_LogonQueueWrite)pack.cmd;
            //触发排队等待的事件
            tagQueueWriteEvent pEvent = new tagQueueWriteEvent(ncb.WriteIndex, ncb.WriteSec, true);
            MsgEventHandle.HandleMsg(pEvent);
            
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_AccountIsFreeze)
        {
            LC_Cmd_AccountIsFreeze ncb = (LC_Cmd_AccountIsFreeze)pack.cmd;

            System.DateTime starTime = System.TimeZone.CurrentTimeZone.ToLocalTime(new System.DateTime(1970, 1, 1));
            System.DateTime pTime = starTime.AddSeconds(System.Convert.ToDouble(ncb.EndTime));

            tagLogonAccountFreezeEvent pEvent = new tagLogonAccountFreezeEvent(pTime);
            MsgEventHandle.HandleMsg(pEvent);

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_Account_Freeze_2);
            MsgEventHandle.HandleMsg(pUOM);

            LogicManager.Instance.LogOff();
            return true;
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_SaveAccountInfo)
        {
            LC_Cmd_SaveAccountInfo ncb = (LC_Cmd_SaveAccountInfo)pack.cmd;
            //将账号 和 3个 密码保持起来 供使用
            if (GlobalLogon.Instance.AccountData == null)
                GlobalLogon.Instance.AccountData = new AccountData();
            GlobalLogon.Instance.AccountData.AccountInfo.UID = ncb.AccountName;
            GlobalLogon.Instance.AccountData.AccountInfo.CRC1 = ncb.PasswordCrc1;
            GlobalLogon.Instance.AccountData.AccountInfo.CRC2 = ncb.PasswordCrc2;
            GlobalLogon.Instance.AccountData.AccountInfo.CRC3 = ncb.PasswordCrc3;
            GlobalLogon.Instance.AccountData.SaveAccountData();
            //GlobalLogon.Instance.SaveAccountData(GlobalLogon.Instance.AccountData);//立刻保存一次
            return true;
        }
        else
            return false;
        GlobalHallUIMgr.Instance.UpdateSystemTipsUITime(0.5f);
        return true;
    }

    public void SaveAccountName(string name)
    {
        if (GlobalLogon.Instance.AccountData!=null)
        {
            GlobalLogon.Instance.AccountData.AccountInfo.UID = name;
            GlobalLogon.Instance.AccountData.SaveAccountData();
            //SaveAccountData(GlobalLogon.Instance.AccountData);
        }
    }
      public void SaveAccountPassWord(uint n1,uint n2,uint n3)
    {
        if (GlobalLogon.Instance.AccountData != null&&GlobalLogon.Instance.AccountData.bIsSavePassword)
          {
              GlobalLogon.Instance.AccountData.AccountInfo.CRC1 = n1;
              GlobalLogon.Instance.AccountData.AccountInfo.CRC2 = n2;
              GlobalLogon.Instance.AccountData.AccountInfo.CRC3 = n3;
              GlobalLogon.Instance.AccountData.SaveAccountData();
              //SaveAccountData(GlobalLogon.Instance.AccountData);
          }
    }
    public void SaveAccountPassWord(string strpass)
    {
        if (GlobalLogon.Instance.AccountData != null && GlobalLogon.Instance.AccountData.bIsSavePassword)
        {
            NativeInterface.ComputeCrc("", strpass, out GlobalLogon.Instance.AccountData.AccountInfo.CRC1, out GlobalLogon.Instance.AccountData.AccountInfo.CRC2, out GlobalLogon.Instance.AccountData.AccountInfo.CRC3);           
            //SaveAccountData(GlobalLogon.Instance.AccountData);
            GlobalLogon.Instance.AccountData.SaveAccountData();
        }       
    }
    public void SavePhoneNum(System.UInt64 num)  
    {
        if (GlobalLogon.Instance.AccountData != null)
        {
            GlobalLogon.Instance.AccountData.PhoneInfo.PhoneNumber=num;
            //SaveAccountData(GlobalLogon.Instance.AccountData);
            GlobalLogon.Instance.AccountData.SaveAccountData();
        }       
    }
    public void SavePhonePassWord(string strpass)
    {
        if (GlobalLogon.Instance.AccountData != null&&GlobalLogon.Instance.AccountData.bIsSavePhonePassword)
        {
            NativeInterface.ComputeCrc("", strpass, out GlobalLogon.Instance.AccountData.PhoneInfo.phoneCRC1, out GlobalLogon.Instance.AccountData.PhoneInfo.phoneCRC2, out GlobalLogon.Instance.AccountData.PhoneInfo.phoneCRC3);
           // SaveAccountData(GlobalLogon.Instance.AccountData);
            GlobalLogon.Instance.AccountData.SaveAccountData();
        }       
    }
    //public void 
    //public void SaveAccountData(AccountData ad)
    //{
    //    if (ad == null)
    //        return;
    //    GlobalLogon.Instance.AccountData = ad;
    //    ad.KEY = ConstValue.FILE_END_MAGIC;
    //    ad.VER = ConstValue.FILE_VER;
    //    System.IO.MemoryStream ms = new System.IO.MemoryStream();
    //    System.IO.BinaryWriter bw = new System.IO.BinaryWriter(ms);
    //    bw.Write(ad.KEY);
    //    bw.Write(ad.VER);
    //    bw.Write(ad.TAG);
    //    bw.Write(ad.UID);
    //    bw.Write(ad.CRC1);
    //    bw.Write(ad.CRC2);
    //    bw.Write(ad.CRC3);
    //    bw.Write(ad.SavePassword);
    //    bw.Write(ad.PhoneNumber);
    //    bw.Write(ad.phoneCRC1);
    //    bw.Write(ad.phoneCRC2);
    //    bw.Write(ad.phoneCRC3);

    //    RuntimeInfo.SaveLocalFile(RuntimeInfo.GetAccountFileName(), ms.ToArray());
    //    bw.Close();
    //}
}