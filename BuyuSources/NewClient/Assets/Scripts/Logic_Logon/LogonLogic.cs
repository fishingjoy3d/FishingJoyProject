using UnityEngine;
using System.Collections.Generic;
using System;
using System.Text;

public enum LogonState
{
    LOGON_INIT,                 //等待用户选择登陆方式
    LOGON_FAST_LOGINNING,       //快速登陆
    LOGON_REGISTER_LOGINNING,   //注册登陆
    LOGON_NORMAL_LOGINNING,     //普通登陆
    LOGON_PHONE_GETCHECK,       //手机登陆获取验证码
    LOGON_PHONE_LOGINNING,      //手机登陆 
    LOGON_CONNECTING,           //正在连接服务器
    LOGON_WAIT_RESULT,          //正在等待结果
    LOGON_WAIT_HALL_RESULT,     //正在等待结果
    LOGON_ACCOUNT_OR_PWD_ERROR, //帐户或者密码错误
    LOGON_NET_ERROR,            //网络连接错误
    LOGON_ERROR,                //登陆失败，帐户原因
    LOGON_CONNECT_HALL,         //进入游戏大厅
    LOGON_HALL_ERROR,           //进入游戏大厅失败
    LOGON_LOGINED,              //登陆成功
    LOGON_WRITE,                //排队状态
    LOGON_SDK_LOGON_WAIT,       //等待SDK登陆
    LOGON_SDK_LOGINNING,        //SDK登陆成功，连接我们的服务器
    LOGON_PHONE_SECPWD,         //手机二级密码登陆
    LOGON_WEIXIN_LOGINNING,
    LOGON_QQ_LOGINNING,
}
struct ByteData
{
    public ByteData(byte[] d)
    {
        strData = d;
    }
    public byte[] strData;
}
public class LogonLogic : ILogic, ICmdHandler
{
    LogonState      m_State;
    //AccountData     m_AccountData;
    LogonLogicUI    m_LogicUI;
    volatile bool   m_bConnecting;
    UInt32          m_UserID;
    UInt32          m_OnlyID;
    float           m_Timeout;
    bool            m_QQOrWeiXinLogon;
    string          m_OpenID;
    string          m_Tonken;
    bool            m_IsNewRsg;
    byte            m_LogonType;
    public bool Init(ILogicUI logicUI, object obj)
    {
        SceneObjMgr.Instance.SetShowType(ShowFlagType.SHOW_VERSION);
        m_LogicUI = (LogonLogicUI)logicUI;
        SetState(LogonState.LOGON_INIT);
        NetServices.Instance.CmdHandler = this;
        m_bConnecting = false;
        m_IsNewRsg = false;
        m_OpenID = "";
        m_Tonken = "";
        m_LogonType = 0;
        LogonRuntime.Init(this);
        GlobalHallUIMgr.Instance.ShutDowNotic();

        if (SDKMgr.IS_SDK_CHANNEL)
        {
            //渠道包
            if(obj != null)
            {
                //返回到登陆界面，点击登陆
            }
            else
            {
                //启动时直接登陆
                if(SDKMgr.Instance.InitOK)
                    SDKLogin();
            }
        }
        else
        {
            if (obj != null)
            {
                GlobalLogon.Instance.AccountData.LoadAccountData();//从文件中将数据读取出来
            }
            else
            {
                //AccountData ad = new AccountData();
                //ad.LoadAccountData();
                GlobalLogon.Instance.AccountData.LoadAccountData();
                if (GlobalLogon.Instance.AccountData.IsSavePassword(true))//如果文件里面保存了密码就登陆 没有密码就不登陆
                    _Logon(GlobalLogon.Instance.AccountData.AccountInfo);
                else if (GlobalLogon.Instance.AccountData.IsSavePassword(false))
                    _SecPassLogon(GlobalLogon.Instance.AccountData.PhoneInfo);
            }
        }
        return true;
    }
    public LogonLogicUI LogicUI
    {
        get { return m_LogicUI; }
    }
    public LogonState State
    {
        get { return m_State; }
        set { m_State = value; }
    }
    void SetState(LogonState state)
    {
        m_State = state;
        //m_LogicUI.OnStateChanged(state);
    }
    //快速登陆
    public void GuestLogon()
    {
        if(m_State != LogonState.LOGON_INIT)
        {
            return;
        }
        if (GlobalLogon.Instance.AccountData != null)
            _Logon(GlobalLogon.Instance.AccountData.AccountInfo);
        else
            _FastLogon();
    }
    //SDK登录
    public System.Collections.IEnumerator WaitSDKLogon(System.Object obj)
    {
        uint tick = Utility.GetTickCount();
        while (SDKMgr.Instance.InitOK == false)
        {
            uint t = Utility.GetTickCount() - tick;
            if(t > 2000)
            {
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("sdkiniterr"), 3, false);
                m_State = LogonState.LOGON_INIT;
                yield break;
            }
            yield return new WaitForEndOfFrame();
        }
        m_State = LogonState.LOGON_SDK_LOGON_WAIT;
        SDKMgr.Instance.SDK.Login("");
        yield break;
    }
    public void SDKLogin()
    {
        if (m_State != LogonState.LOGON_INIT && m_State != LogonState.LOGON_ERROR)
            return;
        if (SDKMgr.Instance.InitOK == false)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("sdkiniterr"), 3, false);
            m_State = LogonState.LOGON_INIT;
            return;
        }
        m_State = LogonState.LOGON_SDK_LOGON_WAIT;
        GlobalEffectMgr.Instance.ShowLoadingMessage();
        SDKMgr.Instance.SDKLogin();
        //GlobalEffectMgr.Instance.ShowLoadingMessage();
        //SDKMgr.Instance.ResetLogonState();
       // SceneMain.Instance.StartInnerCoroutine(WaitSDKLogon(null));
    }
    //注册
    public void RegisterLogon(AccountInfo rd)
    {
        if (m_State != LogonState.LOGON_INIT)
        {
            return;
        }
        NormalAccountInfo ad = new NormalAccountInfo();
        ad.UID = rd.UID;
        //AccountData ad = new AccountData();
        //ad.UID = rd.UID;
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.AccountNameMinLength, FishDataInfo.AccountNameLength, ad.UID, StringCheckType.SCT_AccountName))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_Register_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearAccountInfo();
            }
            return;
        }
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength, FishDataInfo.PasswordLength, rd.PWD, StringCheckType.SCT_Password))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_Register_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearAccountInfo();
            }
            return;
        }
        if (!NativeInterface.ComputeCrc(ad.UID, rd.PWD, out ad.CRC1, out ad.CRC2, out ad.CRC3))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_Register_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearAccountInfo();
            }
            return;
        }
        _Register(ad);
    }
    //是否正在等待登录
    public bool IsWaitLogon
    {
        get
        {
            return (State == LogonState.LOGON_INIT || State == LogonState.LOGON_ERROR || State == LogonState.LOGON_NET_ERROR);
        }
    }
    //手机二级密码登陆
    public void SecPasswordLogon(PhoneAccountInfo rd)
    {
        if (rd.PhoneNumber == 0 || !FishDataInfo.IsCanUsePhone(rd.PhoneNumber))
        {
            //当前随机号码无法使用
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_SecPass_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearPhoneInfo();
            }
            return;
        }
        _SecPassLogon(rd);
    }
    public void SecPasswordLogon(AccountInfo rd)
    {
        if (m_State != LogonState.LOGON_INIT)
        {
            return;
        }
        UInt64 PhoneNumber = Convert.ToUInt64(rd.UID);
        if(PhoneNumber ==0 || !FishDataInfo.IsCanUsePhone(PhoneNumber))
        {
            //当前随机号码无法使用
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_SecPass_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearPhoneInfo();
            }
            return;
        }
        PhoneAccountInfo ad = new PhoneAccountInfo();
        ad.PhoneNumber = Convert.ToUInt64(rd.UID);

        //AccountData ad = new AccountData();
        //ad.PhoneNumber = Convert.ToUInt64(rd.UID);
        //xuda  空密码为 123
        if (rd.PWD == null || rd.PWD.Length == 0 || rd.PWD == "")
        {
            rd.PWD = "123";
        }
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength, FishDataInfo.PasswordLength, rd.PWD, StringCheckType.SCT_Password))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_SecPass_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearPhoneInfo();
            }
            return;
        }
        if (!NativeInterface.ComputeCrc(ad.PhoneNumber.ToString(), rd.PWD, out ad.phoneCRC1, out ad.phoneCRC2, out ad.phoneCRC3))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_SecPass_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearPhoneInfo();
            }
            return;
        }
        //基本验证完毕
        _SecPassLogon(ad);
    }
    //正常登陆
    public void Logon(AccountInfo rd)
    {
        if (m_State != LogonState.LOGON_INIT)
        { 
            return;
        }
        //AccountData ad = new AccountData();
        //ad.UID = rd.UID;

        NormalAccountInfo ad = new NormalAccountInfo();
        ad.UID = rd.UID;

        //xuda  空密码为 123
        if (rd.PWD == null || rd.PWD.Length == 0 || rd.PWD == "")
        {
            rd.PWD = "123";
        }
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.AccountNameMinLength, FishDataInfo.AccountNameLength, ad.UID, StringCheckType.SCT_AccountName))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_NormalLogon_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearAccountInfo();
            }
            return;
        }
        if(!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength,FishDataInfo.PasswordLength,rd.PWD,StringCheckType.SCT_Password))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_NormalLogon_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearAccountInfo();
            }
            return;
        }
        if(!NativeInterface.ComputeCrc(ad.UID, rd.PWD, out ad.CRC1, out ad.CRC2, out ad.CRC3))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_NormalLogon_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SetState(LogonState.LOGON_ERROR);
            if (GlobalLogon.Instance.AccountData != null)
            {
                GlobalLogon.Instance.AccountData.ClearAccountInfo();
            }
            return;
        }
        _Logon(ad);
    }
    void _FastLogon()
    {
        SetState(LogonState.LOGON_FAST_LOGINNING);
        BeginConnect();
    }
    void _Logon(NormalAccountInfo ad)
    {
        GlobalLogon.Instance.AccountData.TempAccountInfo.Clear();
        GlobalLogon.Instance.AccountData.TempPhoneInfo.Clear();
        GlobalLogon.Instance.AccountData.TempAccountInfo = ad;
        SetState(LogonState.LOGON_NORMAL_LOGINNING);
        BeginConnect();
    }
    void _Register(NormalAccountInfo ad)
    {
        GlobalLogon.Instance.AccountData.TempAccountInfo.Clear();
        GlobalLogon.Instance.AccountData.TempPhoneInfo.Clear();
        GlobalLogon.Instance.AccountData.TempAccountInfo = ad;
        SetState(LogonState.LOGON_REGISTER_LOGINNING);
        BeginConnect();
    }
    void _SecPassLogon(PhoneAccountInfo ad)
    {
        GlobalLogon.Instance.AccountData.TempAccountInfo.Clear();
        GlobalLogon.Instance.AccountData.TempPhoneInfo.Clear();
        GlobalLogon.Instance.AccountData.TempPhoneInfo = ad;
        SetState(LogonState.LOGON_PHONE_SECPWD);
        BeginConnect();
    }

    void _WeiXinLogon(string Str, string Token)
    {
        m_OpenID = Str;
        m_Tonken = Token;
        SetState(LogonState.LOGON_WEIXIN_LOGINNING);//开始微信登陆
        BeginConnect();
    }
    void _QQLogon(string Str, string Token)
    {
        m_OpenID = Str;
        m_Tonken = Token;
        SetState(LogonState.LOGON_QQ_LOGINNING);//开始微信登陆
        BeginConnect();
    }

    public bool _GetPhoneLogonCheck(UInt64 PhoneNumber)
    {
        if (!FishDataInfo.IsCanUsePhone(PhoneNumber))
            return false;
        if (GlobalLogon.Instance.PhoneLogonData == null)
            GlobalLogon.Instance.PhoneLogonData = new PhoneLogonData();
        GlobalLogon.Instance.PhoneLogonData.PhoneNumber = PhoneNumber;
        SetState(LogonState.LOGON_PHONE_GETCHECK);
        BeginConnect();
        return true;
    }
   public void _PhoneLogon(UInt64 PhoneNumber,UInt32 Password,string NewPassword,bool IsFirOrSec)
    {
        //手机登陆
        GlobalLogon.Instance.PhoneLogonData.Password = Password;
        GlobalLogon.Instance.PhoneLogonData.IsFirOrSec = IsFirOrSec;
        GlobalLogon.Instance.PhoneLogonData.NewPassword = NewPassword;
        SetState(LogonState.LOGON_PHONE_LOGINNING);
        BeginConnect();
    }
    string RankString(int MinLength,int MaxLength,StringCheckType Type)
    {
        int Count = 10;
        while (Count>0)
        {
            StringBuilder sb = new StringBuilder();
            int RankLength = UnityEngine.Random.Range(MinLength, MaxLength);
            for (int i = 0; i < RankLength; ++i)
            {
                //a-z 65 - 90 97-122
                sb.Append(Convert.ToChar(UnityEngine.Random.Range(65, 90)));
            }
            string str = sb.ToString();
            if (FishConfig.Instance.m_ErrorString.CheckStringIsError(MinLength,MaxLength,str, Type))
            {
                return str;
            }
            --Count;
        }
        //10次后还随机不出来字符串 
        return null;
    }
    void SendLogonData()
    {
        switch (m_State)
        {
            case LogonState.LOGON_FAST_LOGINNING:
                if (SceneMain.IsUserOldServer)
                {
                    NetCmdFastLogon ncf = new NetCmdFastLogon();
                    ncf.SetCmdType(NetCmdType.CMD_FAST_LOGON);
                    NetServices.Instance.Send<NetCmdFastLogon>(ncf);
                }
                else
                {
                    //玩家进行快速登陆 当前操作一定是注册一个账号 快速的账号注册
                    CL_Cmd_GetNewAccount ncb = new CL_Cmd_GetNewAccount();
                    ncb.SetCmdType(NetCmdType.CMD_CL_GetNewAccount);
                    string Password = RankString(FishDataInfo.PasswordMinLength, FishDataInfo.PasswordLength, StringCheckType.SCT_Password);
                    if (Password == null)
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_FastRegister_Failed_1);
                        MsgEventHandle.HandleMsg(pUOM);

                        SetState(LogonState.LOGON_ERROR);
                        GlobalEffectMgr.Instance.CloseLoadingMessage();
                        return;
                    }
                    UInt32 Crc1, Crc2, Crc3;
                    if(!NativeInterface.ComputeCrc("Fast", Password, out Crc1, out Crc2, out Crc3))
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_FastRegister_Failed_2);
                        MsgEventHandle.HandleMsg(pUOM);

                        SetState(LogonState.LOGON_ERROR);
                        GlobalEffectMgr.Instance.CloseLoadingMessage();
                        return;
                    }
                    ncb.PasswordCrc1 = Crc1;
                    ncb.PasswordCrc2 = Crc2;
                    ncb.PasswordCrc3 = Crc3;
                    ncb.MacAddress = Utility.GetMacAddress();
                    ncb.VersionID = ServerSetting.ClientVer;
                    ncb.PlateFormID = (Byte)RuntimeInfo.GetPlatform();
                    ncb.PathCrc = ServerSetting.RES_VERSION;

                    //4.组织起来发送到服务器去
                    NetServices.Instance.Send<CL_Cmd_GetNewAccount>(ncb);
                }
                break;
            case LogonState.LOGON_NORMAL_LOGINNING:
                if (SceneMain.IsUserOldServer)
                {
                    NetCmdNormalLogon nnl = new NetCmdNormalLogon();
                    nnl.SetCmdType(NetCmdType.CMD_NORMAL_LOGON);
                    NetServices.Instance.Send<NetCmdNormalLogon>(nnl);
                }
                else
                {
                    CL_Cmd_AccountLogon ncb = new CL_Cmd_AccountLogon();
                    ncb.SetCmdType(NetCmdType.CMD_CL_AccountLogon);
                    ncb.AccountName = GlobalLogon.Instance.AccountData.TempAccountInfo.UID;
                    if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.AccountNameMinLength, FishDataInfo.AccountNameLength, ncb.AccountName, StringCheckType.SCT_AccountName))
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_NormalLogon_Failed_1);
                        MsgEventHandle.HandleMsg(pUOM);
                        SetState(LogonState.LOGON_ERROR);
                        GlobalEffectMgr.Instance.CloseLoadingMessage();
                        return;
                    }
                    ncb.PasswordCrc1 = GlobalLogon.Instance.AccountData.TempAccountInfo.CRC1;
                    ncb.PasswordCrc2 = GlobalLogon.Instance.AccountData.TempAccountInfo.CRC2;
                    ncb.PasswordCrc3 = GlobalLogon.Instance.AccountData.TempAccountInfo.CRC3;

                    ncb.VersionID = ServerSetting.ClientVer;
                    ncb.PlateFormID = (Byte)RuntimeInfo.GetPlatform();
                    ncb.PathCrc = ServerSetting.RES_VERSION;
                    ncb.MacAddress = Utility.GetMacAddress();

                    //发送命令
                    NetServices.Instance.Send<CL_Cmd_AccountLogon>(ncb);
                    return;
                }
                break;
            case LogonState.LOGON_PHONE_GETCHECK:
                {
                    UInt64 PhoneNumber = GlobalLogon.Instance.PhoneLogonData.PhoneNumber;
                    if (!FishDataInfo.IsCanUsePhone(PhoneNumber))
                    {
                        //提示玩家手机号码不正确 无法登陆
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLogon_Check_Failed_1);
                        MsgEventHandle.HandleMsg(pUOM);

                        SetState(LogonState.LOGON_ERROR);
                        GlobalEffectMgr.Instance.CloseLoadingMessage();
                        return;
                    }
                    CL_Cmd_PhoneLogon ncb = new CL_Cmd_PhoneLogon();
                    ncb.SetCmdType(NetCmdType.CMD_CL_PhoneLogon);
                    ncb.PhoneNumber = PhoneNumber;
                    NetServices.Instance.Send<CL_Cmd_PhoneLogon>(ncb);
                    return;
                }                
            case LogonState.LOGON_PHONE_LOGINNING:
                { 
                    //最新修改 手机登陆修改为 修改玩家密码 或者 二级密码 进行登陆
                    //手机+ 验证码 进行 取回密码 可以修改一级密码 或者 是二级密码

                    //玩家进行手机登陆 
                    UInt64 PhoneNumber = GlobalLogon.Instance.PhoneLogonData.PhoneNumber;
                    UInt32 Password = GlobalLogon.Instance.PhoneLogonData.Password;
                    string NewPassword = GlobalLogon.Instance.PhoneLogonData.NewPassword;
                    if(!FishDataInfo.IsCanUsePhone(PhoneNumber))
                    {
                        //提示玩家手机号码不正确 无法登陆
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLog_Failed_1);
                        MsgEventHandle.HandleMsg(pUOM);

                        SetState(LogonState.LOGON_ERROR);
                        GlobalEffectMgr.Instance.CloseLoadingMessage();
                        return;
                    }
                    if(!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength,FishDataInfo.PasswordLength,NewPassword,StringCheckType.SCT_Password))
                    {
                        //检测密码是否正确
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLog_Failed_2);
                        MsgEventHandle.HandleMsg(pUOM);

                        SetState(LogonState.LOGON_ERROR);
                        GlobalEffectMgr.Instance.CloseLoadingMessage();
                        return;
                    }

                    UInt32 Crc1, Crc2, Crc3;
                    if (!NativeInterface.ComputeCrc("PhoneLogon", NewPassword, out Crc1, out Crc2, out Crc3))
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLog_Failed_3);
                        MsgEventHandle.HandleMsg(pUOM);

                        SetState(LogonState.LOGON_ERROR);
                        GlobalEffectMgr.Instance.CloseLoadingMessage();
                        return;
                    }

                    CL_Cmd_PhoneLogonCheck ncb = new CL_Cmd_PhoneLogonCheck();
                    ncb.SetCmdType(NetCmdType.CMD_CL_PhoneLogonCheck);
                    ncb.PhoneNumber = PhoneNumber;
                    ncb.Password = Password;
                    ncb.MacAddress = Utility.GetMacAddress();
                    ncb.PasswordCrc1 = Crc1;
                    ncb.PasswordCrc2 = Crc2;
                    ncb.PasswordCrc3 = Crc3;
                    ncb.IsFirOrSecPass = GlobalLogon.Instance.PhoneLogonData.IsFirOrSec;
                    NetServices.Instance.Send<CL_Cmd_PhoneLogonCheck>(ncb);
                    return;
                }               
            case LogonState.LOGON_REGISTER_LOGINNING:
                if (SceneMain.IsUserOldServer)
                {
                    NetCmdRegisterLogon ncr = new NetCmdRegisterLogon();
                    ncr.SetCmdType(NetCmdType.CMD_REGISTER_LOGON);
                    NetServices.Instance.Send<NetCmdRegisterLogon>(ncr);
                }
                else
                {
                    //玩家进行注册命令
                    CL_Cmd_AccountRsg ncb = new CL_Cmd_AccountRsg();
                    ncb.SetCmdType(NetCmdType.CMD_CL_AccountRsg);
                    ncb.AccountName = GlobalLogon.Instance.AccountData.TempAccountInfo.UID;
                    if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.AccountNameMinLength, FishDataInfo.AccountNameLength, ncb.AccountName, StringCheckType.SCT_AccountName))
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_Register_Failed_1);
                        MsgEventHandle.HandleMsg(pUOM);
                        GlobalEffectMgr.Instance.CloseLoadingMessage();
                        SetState(LogonState.LOGON_ERROR);
                        return;
                    }
                    ncb.PasswordCrc1 = GlobalLogon.Instance.AccountData.TempAccountInfo.CRC1;
                    ncb.PasswordCrc2 = GlobalLogon.Instance.AccountData.TempAccountInfo.CRC2;
                    ncb.PasswordCrc3 = GlobalLogon.Instance.AccountData.TempAccountInfo.CRC3;
                    ncb.MacAddress = Utility.GetMacAddress();

                    ncb.VersionID = ServerSetting.ClientVer;
                    ncb.PlateFormID = (Byte)RuntimeInfo.GetPlatform();
                    ncb.PathCrc = ServerSetting.RES_VERSION;

                    //发送命令
                    NetServices.Instance.Send<CL_Cmd_AccountRsg>(ncb);
                }
                break;
            case LogonState.LOGON_PHONE_SECPWD:
                { 
                    //玩家使用手机号码 和 二级密码登陆
                    UInt64 PhoneNumber = GlobalLogon.Instance.AccountData.TempPhoneInfo.PhoneNumber;
                    if (PhoneNumber == 0 || !FishDataInfo.IsCanUsePhone(PhoneNumber))
                    {
                        //提示玩家手机号码不正确 无法登陆
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_SecPass_Failed_1);
                        MsgEventHandle.HandleMsg(pUOM);

                        SetState(LogonState.LOGON_ERROR);
                        GlobalEffectMgr.Instance.CloseLoadingMessage();
                        return;
                    }
                    //使用二级密码登陆+
                    CL_Cmd_PhoneSecPwd ncb = new CL_Cmd_PhoneSecPwd();
                    ncb.SetCmdType(NetCmdType.CMD_CL_PhoneSecPwd);
                    ncb.PhoneNumber = PhoneNumber;

                    ncb.PasswordCrc1 = GlobalLogon.Instance.AccountData.TempPhoneInfo.phoneCRC1;
                    ncb.PasswordCrc2 = GlobalLogon.Instance.AccountData.TempPhoneInfo.phoneCRC2;
                    ncb.PasswordCrc3 = GlobalLogon.Instance.AccountData.TempPhoneInfo.phoneCRC3;
                    ncb.MacAddress = Utility.GetMacAddress();

                    ncb.VersionID = ServerSetting.ClientVer;
                    ncb.PlateFormID = (Byte)RuntimeInfo.GetPlatform();
                    ncb.PathCrc = ServerSetting.RES_VERSION;

                    NetServices.Instance.Send<CL_Cmd_PhoneSecPwd>(ncb);
                }
                break;
            case LogonState.LOGON_WEIXIN_LOGINNING:
                { 
                    SendWeiXinLogon(m_OpenID);
                }
                break;
            case LogonState.LOGON_QQ_LOGINNING:
                {
                    SendQQLogon(m_OpenID);
                }
                break;
            default:
                return;
        }
        m_State = LogonState.LOGON_WAIT_RESULT;
    }
    public static void ShowLogonErrMsg()
    {
        string msg;
        {
            msg = StringTable.GetString("connect_server_err");
        }
        GlobalHallUIMgr.Instance.ShowSystemTipsUI(msg, 3, false);
    }
    System.Collections.IEnumerator WaitConnect(object obj)
    {
        while (m_bConnecting)
        {
            yield return new WaitForSeconds(0.001f);
        }
        if(!NetServices.Instance.IsConnected)
        {
            ShowLogonErrMsg();
            SetState(LogonState.LOGON_ERROR);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            yield break;
        }
        if (m_State == LogonState.LOGON_SDK_LOGINNING)
        {
            //登录成功设置IP状态
            SendSDKLogonData();
        }
        else if (m_State == LogonState.LOGON_CONNECT_HALL)
        {
            SendLogonHallData();
        }
        else
        {
            //登录成功设置IP状态
            SendLogonData();
        }
        
    }
    void SendSDKLogonData()
    {
        SDKLoginResult logonData = SDKMgr.Instance.LoginData; 
        CL_Cmd_ChannelLogon cmdLogon = new CL_Cmd_ChannelLogon();
        cmdLogon.channelInfo = new ChannelUserInfo();
        cmdLogon.SetCmdType(NetCmdType.CMD_CL_ChannelLogon);
        //cmdLogon.channelInfo.uid = uint.Parse(logonData.UID);
        cmdLogon.MacAddress = Utility.GetMacAddress();
        ByteData[] bytedata =new ByteData[]
        {
            new ByteData(System.Text.Encoding.Unicode.GetBytes(logonData.UID)),
            new ByteData(System.Text.Encoding.Unicode.GetBytes(logonData.ChannelLabel)),
            new ByteData(System.Text.Encoding.Unicode.GetBytes(logonData.ChannelUid)),
            new ByteData(System.Text.Encoding.Unicode.GetBytes(logonData.ProductCode)),
            new ByteData(System.Text.Encoding.Unicode.GetBytes(logonData.ChannelCode)),
            new ByteData(System.Text.Encoding.Unicode.GetBytes(logonData.UserName)),
            new ByteData(System.Text.Encoding.Unicode.GetBytes(logonData.Token))
        };
        cmdLogon.channelInfo.HandleSum = 7;
        cmdLogon.channelInfo.Sum = 0;
        for (int i = 0; i < bytedata.Length; ++i)
            cmdLogon.channelInfo.Sum += (uint)(bytedata[i].strData.Length + 2);

        cmdLogon.channelInfo.Array = new byte[cmdLogon.channelInfo.Sum];
        int idx = 0;
        for (int i = 0; i < bytedata.Length; ++i)
        {
            byte[] len = System.BitConverter.GetBytes((ushort)bytedata[i].strData.Length);
            cmdLogon.channelInfo.Array[idx++] = len[0];
            cmdLogon.channelInfo.Array[idx++] = len[1];
            System.Array.Copy(bytedata[i].strData, 0, cmdLogon.channelInfo.Array, idx, bytedata[i].strData.Length);
            idx += bytedata[i].strData.Length;
        }

        cmdLogon.VersionID = ServerSetting.ClientVer;
        cmdLogon.PlateFormID = (Byte)RuntimeInfo.GetPlatform();
        cmdLogon.PathCrc = ServerSetting.RES_VERSION;
        
        NetServices.Instance.Send<CL_Cmd_ChannelLogon>(cmdLogon);
    }
    void StartConnect()
    {
        try
        {
            if (m_State == LogonState.LOGON_CONNECT_HALL)
            {
                NetServices.Instance.Connect(false, ServerSetting.HallServerIP, ServerSetting.HallServerPort, ServerSetting.NewIP, ServerSetting.NewPort);
            }
            else
            {
                ServerIPData sid = ServerSetting.GetLogonServer();
                NetServices.Instance.Connect(true, sid.IP, sid.Port);
            }
        }
        catch
        {

        }
        m_bConnecting = false;
    }
    void BeginConnect()
    {
        m_Timeout = 0;
        if (!NetServices.Instance.IsConnected)
        {
            m_bConnecting = true;
            System.Threading.Thread t = new System.Threading.Thread( new System.Threading.ThreadStart(StartConnect));
            t.Start();
        }
        SceneMain.Instance.StartInnerCoroutine(WaitConnect(null));
    }
    public void SDKLogoned(bool bresult)
    {
        if (bresult == false)
        {
            m_State = LogonState.LOGON_INIT;
        }
        else
        {
            //SDK登陆成功
            //开始连接服务器。
            SetState(LogonState.LOGON_SDK_LOGINNING);
            GlobalEffectMgr.Instance.ShowLoadingMessage();
            BeginConnect();
        }
    }
    public void Update(float delta)
    {
       if (m_State == LogonState.LOGON_SDK_LOGON_WAIT)
       {
           //渠道登录，等待中。
       }
       else if (m_State != LogonState.LOGON_ERROR && m_State != LogonState.LOGON_INIT && m_State != LogonState.LOGON_WRITE)
        {
            m_Timeout += delta;
            if(m_Timeout >= 20.0f)
            {
                m_Timeout = 0;
                //超时
                if (m_State == LogonState.LOGON_CONNECT_HALL || m_State == LogonState.LOGON_WAIT_HALL_RESULT)
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("join_hall_error"), 3, false);
                else
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("connect_server_err"), 3, false);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                NetServices.Instance.Disconnect();
                SetState(LogonState.LOGON_ERROR);
                SDKMgr.Instance.Logout();
            }
        }
    }
    public void Shutdown()
    {
        LogonRuntime.Shutdown();
    }
    public void StateChanged(NetState state)
    {
        if(state == NetState.NET_DISCONNECT)
        {
            NetServices.Instance.ClearCmd();
            SetState(LogonState.LOGON_ERROR);
        }
    }
    void SendLogonHallData()
    {
        if(SceneMain.IsUserOldServer)
        {
            NetCmdLogonHall ncl = new NetCmdLogonHall();
            ncl.SetCmdType(NetCmdType.CMD_LOGON_HALL);
            ncl.CRC1 = GlobalLogon.Instance.AccountData.AccountInfo.CRC1;
            ncl.CRC2 = GlobalLogon.Instance.AccountData.AccountInfo.CRC2;
            ncl.CRC3 = GlobalLogon.Instance.AccountData.AccountInfo.CRC3;
            ncl.UID = GlobalLogon.Instance.AccountData.AccountInfo.UID;
            NetServices.Instance.Send<NetCmdLogonHall>(ncl);
        }
        else
        {
            CL_Cmd_AccountOnlyID msgHall = new CL_Cmd_AccountOnlyID();
            msgHall.SetCmdType(NetCmdType.CMD_CL_AccountOnlyID);
            msgHall.dwOnlyID = m_OnlyID;
            msgHall.dwUserID = m_UserID;
            msgHall.MacAddress = Utility.GetMacAddress();
            //携带数据
            msgHall.PlateFormID = (Byte)RuntimeInfo.GetPlatform();
            int Witdh = Resolution.GetScreenWidth();
            msgHall.ScreenPoint = Convert.ToUInt32((Witdh << 16) + Resolution.GetScreenHeight());
                
            NetServices.Instance.Send<CL_Cmd_AccountOnlyID>(msgHall);
        }
        SetState(LogonState.LOGON_WAIT_HALL_RESULT);
    }
    public void JoinHall()
    {
        NetServices.Instance.Disconnect();
        SetState(LogonState.LOGON_CONNECT_HALL);
        BeginConnect();
    }
    public bool CanProcessCmd()
    {
        return true;
    }
    public bool Handle(NetCmdPack pack)
    {
        if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_AccountOnlyID)
        {
            //玩家登陆的结果 或者是注册的结果
            LC_Cmd_AccountOnlyID ncb = (LC_Cmd_AccountOnlyID)pack.cmd;

            switch(ncb.LogonTypeID)
            {
                case 1:
                    { 
                        //正常登陆
                        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.dwUserID == 0 ? UserOperateMessage.UOM_Logon_NormalLogon_Failed_4 : UserOperateMessage.UOM_Logon_NormalLogon_Sucess));
                        MsgEventHandle.HandleMsg(pUOM);
                    }
                    break;
                case 2:
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.dwUserID == 0 ? UserOperateMessage.UOM_Logon_Register_Failed_4 : UserOperateMessage.UOM_Logon_Register_Sucess));
                        MsgEventHandle.HandleMsg(pUOM);
                    }
                    break;
            }

            if(ncb.dwUserID == 0)
            {
                SetState(LogonState.LOGON_ERROR);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                if (GlobalLogon.Instance.AccountData != null)
                {
                    GlobalLogon.Instance.AccountData.LoadTempAccountInfo();
                    GlobalLogon.Instance.AccountData.LoadTempPhoneInfo();
                    GlobalLogon.Instance.AccountData.SaveAccountData();
                }
            }
            else
            {
                //断开Socket连接 组织新的命令到GameServer去
               // LogMgr.Log("已经与LogonServer断开连接了\r\n");
               // ServerSetting.HallTimeout = Convert.ToInt32(ncb.HallTimeOut);
                NetServices.Instance.Disconnect();
                //将IP转化为String 
                m_UserID = ncb.dwUserID;
                m_OnlyID = ncb.dwOnlyID;

                if (GlobalLogon.Instance.AccountData != null)
                {
                    GlobalLogon.Instance.AccountData.LoadTempAccountInfo();
                    GlobalLogon.Instance.AccountData.LoadTempPhoneInfo();
                    GlobalLogon.Instance.AccountData.SaveAccountData();//将登陆成功的账号数据保存起来
                }

                ServerSetting.HallServerIP = Utility.IPToString(ncb.GateIp);
                ServerSetting.HallServerPort = ncb.GatePort;
                ServerSetting.NewIP = ncb.GameIp;
                ServerSetting.NewPort = ncb.GamePort;
                JoinHall();//加入到大厅并且发送命令 设置好IP 和 Port 并且 设置好需要发送的命令的参赛
                return true;
            }
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_CheckVersionError)
        {
            LC_Cmd_CheckVersionError ncb = (LC_Cmd_CheckVersionError)pack.cmd;

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_Version_Error);
            MsgEventHandle.HandleMsg(pUOM);

            //客户端版本错误 提示 断开
            SetState(LogonState.LOGON_ERROR);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SDKMgr.Instance.Logout();

            //if (GlobalLogon.Instance.AccountData != null)
            //{
            //    GlobalLogon.Instance.AccountData.UID = null;
            //    GlobalLogon.Instance.AccountData.CRC1 = 0;
            //    GlobalLogon.Instance.AccountData.CRC2 = 0;
            //    GlobalLogon.Instance.AccountData.CRC3 = 0;

            //}
            return true;
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_AccountIsFreeze)
        {
            LC_Cmd_AccountIsFreeze ncb = (LC_Cmd_AccountIsFreeze)pack.cmd;

            DateTime starTime = TimeZone.CurrentTimeZone.ToLocalTime(new DateTime(1970, 1, 1));
            try
            {
                DateTime pTime = starTime.AddSeconds(Convert.ToDouble(ncb.EndTime));
                tagLogonAccountFreezeEvent pEvent = new tagLogonAccountFreezeEvent(pTime);
                MsgEventHandle.HandleMsg(pEvent);
            }
            catch
            {

            }





            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_Account_Freeze_2);
            MsgEventHandle.HandleMsg(pUOM);

            //客户端版本错误 提示 断开
            SetState(LogonState.LOGON_ERROR);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            SDKMgr.Instance.Logout();
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_GetNewAccount) //快速登陆
        {
            //快速登陆的时候 先获取新的账号
            LC_Cmd_GetNewAccount ncbGet = (LC_Cmd_GetNewAccount)pack.cmd;

            tagUserOperationEvent pUOM = new tagUserOperationEvent((ncbGet.Result ? UserOperateMessage.UOM_Logon_FastRegister_Sucess : UserOperateMessage.UOM_Logon_FastRegister_Failed_3));
            MsgEventHandle.HandleMsg(pUOM);

            if (ncbGet.Result)
            {
                //玩家登陆成功 我们进行普通登陆
                //保存账号数据
                if (GlobalLogon.Instance.AccountData == null)
                    GlobalLogon.Instance.AccountData = new AccountData();
                GlobalLogon.Instance.AccountData.AccountInfo.UID = ncbGet.NewAccountName;
                GlobalLogon.Instance.AccountData.AccountInfo.CRC1 = ncbGet.PasswordCrc1;
                GlobalLogon.Instance.AccountData.AccountInfo.CRC2 = ncbGet.PasswordCrc2;
                GlobalLogon.Instance.AccountData.AccountInfo.CRC3 = ncbGet.PasswordCrc3;
                GlobalLogon.Instance.AccountData.SaveAccountData();

                //CL_Cmd_AccountLogon ncb = new CL_Cmd_AccountLogon();
                //ncb.SetCmdType(NetCmdType.CMD_CL_AccountLogon);
                //ncb.AccountName = ncbGet.NewAccountName;
                //if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.AccountNameMinLength, FishDataInfo.AccountNameLength, ncb.AccountName, StringCheckType.SCT_AccountName))
                //{
                //    tagUserOperationEvent pUOM2 = new tagUserOperationEvent(UserOperateMessage.UOM_Logon_NormalLogon_Failed_1);
                //    MsgEventHandle.HandleMsg(pUOM2);
                    
                //    SetState(LogonState.LOGON_ERROR);
                //    return false;
                //}
                //ncb.PasswordCrc1 = ncbGet.PasswordCrc1;
                //ncb.PasswordCrc2 = ncbGet.PasswordCrc2;
                //ncb.PasswordCrc3 = ncbGet.PasswordCrc3;
                //ncb.VersionID = ResManager.Instance.VersionMgr.ClientVer;
                //ncb.PlateFormID = (Byte)RuntimeInfo.GetPlatform();
                //ncb.PathCrc = PathManager.Instance.PathCRC;
                ////发送命令
                //NetServices.Instance.Send<CL_Cmd_AccountLogon>(ncb);
                return true;
            }
            else
            {
                //进行快速登陆失败了
                SetState(LogonState.LOGON_ERROR);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                return false;
            }
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_AccountOnlyIDFailed)
        {
            SetState(LogonState.LOGON_ERROR);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            NetServices.Instance.Disconnect();
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("join_failed"), 3, false); 
            // NativeInterface.ShowMsgAndExit("join_hall_error", 130);
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_AccountOnlyIDSuccess)
        {
            LC_Cmd_AccountOnlyIDSuccess ncb = (LC_Cmd_AccountOnlyIDSuccess)pack.cmd;
            //保存玩家账号数据 登陆成功的时候
            PlayerRole.Instance.RoleInfo.RoleMe.SetRoleInfo(ncb.RoleInfo);//设置玩家的数据
            ServerSetting.SetCallbckUrl(Utility.IPToString(ncb.OperateIP));
            PlayerRole.Instance.RoleExChange.SetPlayerRoleIsShowExChange();
            GlobalLogon.Instance.OnlyID = ncb.RandID;

            if (GlobalLogon.Instance.AccountData != null)
            { 
                //设置玩家身份标志
                if (PlayerRole.Instance.RoleInfo.RoleMe.GetIsCanResetAccount())
                    GlobalLogon.Instance.AccountData.TAG = 1;
                else
                    GlobalLogon.Instance.AccountData.TAG = 0;
                GlobalLogon.Instance.AccountData.SaveAccountData();
                //GlobalLogon.Instance.SaveAccountData(GlobalLogon.Instance.AccountData);
            }

            NetServices.Instance.CmdHandler = null;
            //RuntimeData.MyAccount = GlobalLogon.Instance.AccountData;
           // string str = string.Format(StringTable.GetString("sdkwelcome"), SDKMgr.Instance.SDK.GetChannelName(), SDKMgr.Instance.LoginData.ChannelUid);

            if(SDKMgr.IS_SDK_CHANNEL)
            {
                const int zoneID =1;
               // SDKMgr.Instance.SDK.ShowAndroidToast(str);
                //玩家登录
                SDKMgr.Instance.SDK.SetExtraData(
                    SDK.ENTER_SERVER,
                    SDKMgr.Instance.LoginData.UID,
                    ncb.RoleInfo.NickName,
                    (int)ncb.RoleInfo.wLevel,
                    zoneID,
                    "乐多捕鱼" + zoneID + "区",
                    (int)ncb.RoleInfo.dwGlobeNum,
                    1,
                    "无帮派"
                    );
                //玩家角色
                SDKMgr.Instance.SDK.SetExtraData(
                    SDK.CREATE_ROLE,
                    SDKMgr.Instance.LoginData.UID,
                    ncb.RoleInfo.NickName,
                    (int)ncb.RoleInfo.wLevel,
                    zoneID,
                    "乐多捕鱼" + zoneID + "区",
                    (int)ncb.RoleInfo.dwGlobeNum,
                    1,
                    "无帮派"
                    );
                
            }
            //PlayerRole.Instance.IsNeedResetTable = false;
            ServerSetting.SetLogonServerState(true);
            LogicManager.Instance.Forward(null);

            if(m_IsNewRsg && (m_LogonType == 1 || m_LogonType==2))
            {
                //是注册的账号
                 UpdateAccountInfo(m_LogonType, m_Tonken, m_OpenID);
            }
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LOGON_RESULT)
        {
            
            NetCmdLogonResult nlr = (NetCmdLogonResult)pack.cmd;
            if (nlr.Result == 0)
            {
                SetState(LogonState.LOGON_ERROR);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                NetServices.Instance.Disconnect();
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("join_failed"), 3, false);
                SDKMgr.Instance.Logout();
            }
            else
            {
                SetState(LogonState.LOGON_LOGINED);
                if (GlobalLogon.Instance.AccountData == null)
                    GlobalLogon.Instance.AccountData = new AccountData();
                GlobalLogon.Instance.AccountData.TAG = 0;
                GlobalLogon.Instance.AccountData.AccountInfo.UID = nlr.UID;
                GlobalLogon.Instance.AccountData.AccountInfo.CRC1 = nlr.CRC1;
                GlobalLogon.Instance.AccountData.AccountInfo.CRC2 = nlr.CRC2;
                GlobalLogon.Instance.AccountData.AccountInfo.CRC3 = nlr.CRC3;
                //GlobalLogon.Instance.SaveAccountData(ad);
                //GlobalLogon.Instance.AccountData = ad;
                GlobalLogon.Instance.AccountData.SaveAccountData();

                ServerSetting.HallServerIP = Utility.IPToString(nlr.ServerIP);
                ServerSetting.HallServerPort = nlr.ServerPort;
                JoinHall();
            }
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LOGON_RESULT_SIMPLE)
        {
            NetCmdLogonResultSimple nlr = (NetCmdLogonResultSimple)pack.cmd;
            if (nlr.Result == 0)
            {
                SetState(LogonState.LOGON_ERROR);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                NetServices.Instance.Disconnect();
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("join_failed"), 3, false); 
            }
            else
            {
                SetState(LogonState.LOGON_LOGINED);
                ServerSetting.HallServerIP = Utility.IPToString(nlr.ServerIP);
                ServerSetting.HallServerPort = nlr.ServerPort;

                //加载资源
                JoinHall();
            }
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LOGON_HALL_RESULT)
        {
            NetCmdLogonHallResult ncl = (NetCmdLogonHallResult)pack.cmd;
            if (ncl.Result == 0)
            {
                SetState(LogonState.LOGON_ERROR);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                NetServices.Instance.Disconnect();
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("join_hall_error"), 3, false);
                SDKMgr.Instance.Logout();
            }
            else
            {
                NetServices.Instance.CmdHandler = null;
                //RuntimeData.MyAccount = GlobalLogon.Instance.AccountData;
                //临时，设置玩家ID
                PlayerRole.Instance.RoleInfo.RoleMe.SetUserID(ncl.PlayerID);
                ServerSetting.SetLogonServerState(true);
                LogicManager.Instance.Forward(null);
            }
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_LogonQueueWrite)
        {
            //排队的消息  我们让客户端继续等待
            LC_Cmd_LogonQueueWrite ncb = (LC_Cmd_LogonQueueWrite)pack.cmd;
            SetState(LogonState.LOGON_WRITE);//设置当前进入等待状态 一直进行等待登陆的处理 
            //
            tagQueueWriteEvent pEvent = new tagQueueWriteEvent(ncb.WriteIndex, ncb.WriteSec, true);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_ChannelLogon)
        {
            //通过平台登录 失败了
            LC_Cmd_ChannelLogon ncb = (LC_Cmd_ChannelLogon)pack.cmd;
            //提示玩家登陆失败了
            if(ncb.Result == false)
            {
                string str = StringTable.GetString("UOM_Logon_Channel_Failed_1");
                str = string.Format(str, SDKMgr.Instance.SDK.GetChannelName());
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(str, 3, false);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                SetState(LogonState.LOGON_ERROR);
                NetServices.Instance.Disconnect();
                SDKMgr.Instance.Logout();
                return false;
            }
            return true;
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_PhoneLogon)
        {
            //手机登陆获取验证码
            LC_Cmd_PhoneLogon ncb = (LC_Cmd_PhoneLogon)pack.cmd;
            if (ncb.ErrorID == (Byte)PhoneLogonError.PLE_Success)
            {
                //手机登陆获取验证码成功
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLogon_Check_Success);
                MsgEventHandle.HandleMsg(pUOM);
            }
            else
            {
                //提示玩家 错误数据
                switch(ncb.ErrorID)
                {
                    case (Byte)PhoneLogonError.PLE_LogonExists:
                        {
                            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLogon_Check_Failed_2);
                            MsgEventHandle.HandleMsg(pUOM);
                        }
                        break;
                    case (Byte)PhoneLogonError.PLE_PasswordFailed:
                        {
                            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLogon_Check_Failed_3);
                            MsgEventHandle.HandleMsg(pUOM);
                        }
                        break;
                    case (Byte)PhoneLogonError.PLE_SystemError:
                        {
                            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLogon_Check_Failed_4);
                            MsgEventHandle.HandleMsg(pUOM);
                        }
                        break;
                }
                SetState(LogonState.LOGON_ERROR);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                NetServices.Instance.Disconnect();
            }
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_PhoneLogonCheck)
        {
            LC_Cmd_PhoneLogonCheck ncb = (LC_Cmd_PhoneLogonCheck)pack.cmd;
            if (ncb.Result && ncb.dwUserID != 0)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLog_Success);
                MsgEventHandle.HandleMsg(pUOM);
                return true;//无须做继续处理
            }
            else
            {
                //登陆失败了
                switch (ncb.ErrorID)
                {
                    case (Byte)PhoneLogonError.PLE_PasswordError:
                        {
                            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLog_Failed_4);
                            MsgEventHandle.HandleMsg(pUOM);
                        }
                        break;
                    case (Byte)PhoneLogonError.PLE_SystemError:
                        {
                            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_PhoneLog_Failed_5);
                            MsgEventHandle.HandleMsg(pUOM);
                        }
                        break;
                }
                SetState(LogonState.LOGON_ERROR);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                NetServices.Instance.Disconnect();
            }
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_WeiXinLogon)
        {
            LC_Cmd_WeiXinLogon ncb = (LC_Cmd_WeiXinLogon)pack.cmd;
            if(ncb.Result)
            {
                 tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_WeiXinLogon_Success);
                 MsgEventHandle.HandleMsg(pUOM);
            }
            else
            {
                 tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_WeiXinLogon_Faile_1);
                 MsgEventHandle.HandleMsg(pUOM);

                 SetState(LogonState.LOGON_ERROR);
                 GlobalEffectMgr.Instance.CloseLoadingMessage();
                 NetServices.Instance.Disconnect();
            }
        }
        else if (pack.cmd.GetCmdType() == NetCmdType.CMD_LC_QQLogon)
        {
            LC_Cmd_QQLogon ncb = (LC_Cmd_QQLogon)pack.cmd;
            if (ncb.Result)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_QQLogon_Success);
                MsgEventHandle.HandleMsg(pUOM);
            }
            else
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_QQLogon_Faile_1);
                MsgEventHandle.HandleMsg(pUOM);

                SetState(LogonState.LOGON_ERROR);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                NetServices.Instance.Disconnect();
            }
        }
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_SaveAccountInfo)
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
        else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_RsgNewAccount)
        {
            //当前玩家登陆的账号未新注册的
            LC_Cmd_RsgNewAccount ncb = (LC_Cmd_RsgNewAccount)pack.cmd;
            m_IsNewRsg = true;
            m_LogonType = ncb.LogonType;
            if (ncb.LogonType == 2)
            {
                //微信玩家注册
            }
            else if (ncb.LogonType == 3)
            {
                //QQ玩家注册
            }

            return true;
        }
        else
            return false;

        return true;
    }

    void TestChannelLogon()
    {
        SDKLoginResult pResult = new SDKLoginResult();
        pResult.ChannelCode = "testChannelCode";
        pResult.ChannelLabel = "testChannelLabel";
        pResult.ChannelUid = "testOnlyUserID";
        pResult.ProductCode = "testProductCode";
        pResult.Token = "testToken";
        pResult.UserName = "testName";
        pResult.UID = "testUID";
        pResult.CustomParams = "testCustomParams";
        pResult.Result = LoginState.LOGIN_OK;
        SDKMgr.Instance.SDKCallback.LoginCallback(pResult);
        //SendSDKLogonData();
    }

   public void OnBeginQQLogon()
    {
        m_QQOrWeiXinLogon = true;
        SDKMgr.Instance.ThirdLogin(true);//QQ登陆
    }
   public void OnBeginWeiXinLogon()
    {
        m_QQOrWeiXinLogon = false;
        SDKMgr.Instance.ThirdLogin(false);//微信登陆
    }
    public void OnHandleQQOrWeiXinLogonResult(string Str,string Token)
    {
        m_LogicUI.CloseLogonInitWnd();
        GlobalEffectMgr.Instance.ShowLoadingMessage();
        if(m_QQOrWeiXinLogon)
        {
            //SendQQLogon(Str);
            _QQLogon(Str, Token);
        }
        else
        {
            _WeiXinLogon(Str, Token);
            //SendWeiXinLogon(Str);
        }
    }
    void UpdateAccountInfo(Byte bType,string Token,string OpenID)
    {
        CL_Cmd_UpdateAccount cmd = new CL_Cmd_UpdateAccount();
        cmd.Info = new AccountSDKInfo();
        cmd.SetCmdType(NetCmdType.CMD_CL_UpdateAccount);
        cmd.bType = bType;

        ByteData[] bytedata = new ByteData[]
        {
            new ByteData(System.Text.Encoding.ASCII.GetBytes(Token)),
            new ByteData(System.Text.Encoding.ASCII.GetBytes(OpenID)),
        };
        cmd.Info.HandleSum = 2;
        cmd.Info.Sum = 0;
        for (int i = 0; i < bytedata.Length; ++i)
            cmd.Info.Sum += (uint)(bytedata[i].strData.Length + 2);

        cmd.Info.Array = new byte[cmd.Info.Sum];
        int idx = 0;
        for (int i = 0; i < bytedata.Length; ++i)
        {
            byte[] len = System.BitConverter.GetBytes((ushort)bytedata[i].strData.Length);
            cmd.Info.Array[idx++] = len[0];
            cmd.Info.Array[idx++] = len[1];
            System.Array.Copy(bytedata[i].strData, 0, cmd.Info.Array, idx, bytedata[i].strData.Length);
            idx += bytedata[i].strData.Length;
        }
        NetServices.Instance.Send<CL_Cmd_UpdateAccount>(cmd);
    }
    //QQ 和微信登陆的回调函数
    void SendWeiXinLogon(string Code)
    {
        m_LogonType = 2;
        //玩家获取微信的数据后 试图进行微信登陆
        CL_Cmd_WeiXinLogon cmd = new CL_Cmd_WeiXinLogon();
        cmd.CodeInfo = new WeiXinUserCodeInfo();
        cmd.SetCmdType(NetCmdType.CMD_CL_WeiXinLogon);
        cmd.MacAddress = Utility.GetMacAddress();
        cmd.VersionID = ServerSetting.ClientVer;
        cmd.PlateFormID = (Byte)RuntimeInfo.GetPlatform();
        cmd.PathCrc = ServerSetting.RES_VERSION;

        ByteData[] bytedata = new ByteData[]
        {
            new ByteData(System.Text.Encoding.ASCII.GetBytes(Code)),
        };
        cmd.CodeInfo.HandleSum = 1;
        cmd.CodeInfo.Sum = 0;
        for (int i = 0; i < bytedata.Length; ++i)
            cmd.CodeInfo.Sum += (uint)(bytedata[i].strData.Length + 2);

        cmd.CodeInfo.Array = new byte[cmd.CodeInfo.Sum];
        int idx = 0;
        for (int i = 0; i < bytedata.Length; ++i)
        {
            byte[] len = System.BitConverter.GetBytes((ushort)bytedata[i].strData.Length);
            cmd.CodeInfo.Array[idx++] = len[0];
            cmd.CodeInfo.Array[idx++] = len[1];
            System.Array.Copy(bytedata[i].strData, 0, cmd.CodeInfo.Array, idx, bytedata[i].strData.Length);
            idx += bytedata[i].strData.Length;
        }
        NetServices.Instance.Send<CL_Cmd_WeiXinLogon>(cmd);
    }

    void SendQQLogon(string Token)
    {
        m_LogonType = 3;
        //玩家获取微信的数据后 试图进行微信登陆
        CL_Cmd_QQLogon cmd = new CL_Cmd_QQLogon();
        cmd.TokenInfo = new QQUserCodeInfo();
        cmd.SetCmdType(NetCmdType.CMD_CL_QQLogon);
        cmd.MacAddress = Utility.GetMacAddress();
        cmd.VersionID = ServerSetting.ClientVer;
        cmd.PlateFormID = (Byte)RuntimeInfo.GetPlatform();
        cmd.PathCrc = ServerSetting.RES_VERSION;

        ByteData[] bytedata = new ByteData[]
        {
            new ByteData(System.Text.Encoding.ASCII.GetBytes(Token)),
        };
        cmd.TokenInfo.HandleSum = 1;
        cmd.TokenInfo.Sum = 0;
        for (int i = 0; i < bytedata.Length; ++i)
            cmd.TokenInfo.Sum += (uint)(bytedata[i].strData.Length + 2);

        cmd.TokenInfo.Array = new byte[cmd.TokenInfo.Sum];
        int idx = 0;
        for (int i = 0; i < bytedata.Length; ++i)
        {
            byte[] len = System.BitConverter.GetBytes((ushort)bytedata[i].strData.Length);
            cmd.TokenInfo.Array[idx++] = len[0];
            cmd.TokenInfo.Array[idx++] = len[1];
            System.Array.Copy(bytedata[i].strData, 0, cmd.TokenInfo.Array, idx, bytedata[i].strData.Length);
            idx += bytedata[i].strData.Length;
        }
        NetServices.Instance.Send<CL_Cmd_QQLogon>(cmd);
    }
}
