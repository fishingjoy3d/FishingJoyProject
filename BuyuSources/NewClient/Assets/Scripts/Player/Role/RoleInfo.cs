using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家自身的数据管理器 用于修改玩家的本事的数据
class RoleInfo
{
    private RoleMe m_RoleInfo = new RoleMe();

    public RoleInfo()
    {
        OnClear();

        //RoleInfoTest();
    }
    public void OnClear()
    {
        m_RoleInfo.SetRoleInfo(null);
        m_RoleInfo.SetMonthInfo(null);
        m_RoleInfo.SetSeat(0xff);
    }

    internal RoleMe RoleMe
    {
      get { return m_RoleInfo; }
    }

    //提供公用函数
    public bool ChangeGender()
    {
        return SendChangeRoleGender(!m_RoleInfo.GetGender());
    }
   
    public bool ChangeNickName(string Name)
    {
        return SendChangeRoleNickName(Name);
    }
    public Byte GetRoleUnLockRateIndex()
    {
        //获取用户未解锁的倍率范围
        int256 RateValue = PlayerRole.Instance.RoleInfo.RoleMe.GetRateValue();
        for(Byte i =0;i< BulletSetting.BulletRate.Length;++i)
        {
            if (!int256Function.GetBitStates(RateValue, i))
                return i;
        }
        return 0;
    }
    public Byte GetCanUseCashSum()
    {
        Byte AllCashSum = PlayerRole.Instance.RoleVip.GetUseMedalSum();
        Byte UseCashSum = PlayerRole.Instance.RoleInfo.RoleMe.GetCashSum();
        if (AllCashSum >= UseCashSum)
            return Convert.ToByte(AllCashSum - UseCashSum);
        else
            return 0;
    }
    public bool IsCanUseRateIndex(Byte RateIndex)
    {
        return int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetRateValue(), RateIndex);
    }
    public UInt32 GetUnLockRateNeedCurrcey(Byte AddRateIndex)
    {
        UInt32 CurrceySum = 0;
        for (Byte i = 0; i <= AddRateIndex; ++i)
        {
            if (IsCanUseRateIndex(i))
                continue;
            CurrceySum += Convert.ToUInt32(BulletSetting.RateUnlock[i]);
        }
        return CurrceySum;
    }
    public bool ChangeRoleShareSatate(bool States)
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetShareStates() == States)
            return false;
        CL_Cmd_ChangeRoleShareStates ncb = new CL_Cmd_ChangeRoleShareStates();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleShareStates);
        ncb.ShareStates = States;
        NetServices.Instance.Send<CL_Cmd_ChangeRoleShareStates>(ncb);
        //LogMgr.Log("已经分享");
        return true;
    }
    public bool ChangeRoleRateValue(Byte AddRateIndex)
    {
        //判断倍率是否可以开启
        UInt32 CurrceySum = GetUnLockRateNeedCurrcey(AddRateIndex);
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < CurrceySum)
            return false;

        CL_Cmd_ChangeRoleRateValue ncb = new CL_Cmd_ChangeRoleRateValue();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleRateValue);
        ncb.RateIndex = AddRateIndex;
        NetServices.Instance.Send<CL_Cmd_ChangeRoleRateValue>(ncb);
        return true;
    }
    public bool ChangeSystemFaceID(UInt32 FaceID)
    {
        return SendChangeRoleNormalFaceID(FaceID);
    }
    public void SendIsShowIpAddress(bool States)
    {
        if (States == RoleMe.IsShowIpAddress())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_IsShowIpAddress_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return;
        }
            
        CL_Cmd_ChangeRoleIsShowIpAddress ncb = new CL_Cmd_ChangeRoleIsShowIpAddress();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleIsShowIpAddress);
        ncb.IsShowIpAddress = States;
        NetServices.Instance.Send<CL_Cmd_ChangeRoleIsShowIpAddress>(ncb);
    }
    public bool ResetAccountInfo(string AccountInfo,string Password)//无须旧密码 因为游客根本不找到旧密码
    {
        //必须为游客才可以重置账号
        if(!PlayerRole.Instance.RoleInfo.RoleMe.GetIsCanResetAccount())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ResetAccount_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.AccountNameMinLength, FishDataInfo.AccountNameLength, AccountInfo, StringCheckType.SCT_AccountName))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ResetAccount_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength, FishDataInfo.PasswordLength, Password, StringCheckType.SCT_Password))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ResetAccount_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        UInt32 NewCrc1, NewCrc2, NewCrc3;
        if (!NativeInterface.ComputeCrc(AccountInfo, Password, out NewCrc1, out NewCrc2, out NewCrc3))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ResetAccount_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //发送命令到服务器端去 需要判断账号是否存在
        CL_Cmd_AccountResetAccount ncb = new CL_Cmd_AccountResetAccount();
        ncb.SetCmdType(NetCmdType.CMD_CL_AccountResetAccount);
        ncb.NewAccountName = AccountInfo;
        ncb.PasswordCrc1 = NewCrc1;
        ncb.PasswordCrc2 = NewCrc2;
        ncb.PasswordCrc3 = NewCrc3;
        NetServices.Instance.Send<CL_Cmd_AccountResetAccount>(ncb);

        GlobalLogon.Instance.AccountData.TempAccountInfo.Clear();
        GlobalLogon.Instance.AccountData.TempPhoneInfo.Clear();
        GlobalLogon.Instance.AccountData.TempAccountInfo.UID = AccountInfo;
        GlobalLogon.Instance.AccountData.TempAccountInfo.CRC1 = NewCrc1;
        GlobalLogon.Instance.AccountData.TempAccountInfo.CRC2 = NewCrc2;
        GlobalLogon.Instance.AccountData.TempAccountInfo.CRC3 = NewCrc3;

        return true;
    }
    public bool ResetAccountPassword(string OldPassword,string NewPassword)
    {
        if (GlobalLogon.Instance.AccountData == null)
            return false;
        //进行验证 
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength, FishDataInfo.PasswordLength, OldPassword, StringCheckType.SCT_Password))  
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ChangePassword_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength, FishDataInfo.PasswordLength, NewPassword, StringCheckType.SCT_Password))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ChangePassword_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
        }
        if (NewPassword.CompareTo(OldPassword) == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ChangePassword_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        string AccountName = GlobalLogon.Instance.AccountData.AccountInfo.UID;
        //发送命令到服务器去
        UInt32 OldCrc1, OldCrc2, OldCrc3;
        if (!NativeInterface.ComputeCrc(AccountName, OldPassword, out OldCrc1, out OldCrc2, out OldCrc3))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ChangePassword_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        UInt32 NewCrc1, NewCrc2, NewCrc3;
        if (!NativeInterface.ComputeCrc(AccountName, NewPassword, out NewCrc1, out NewCrc2, out NewCrc3))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ChangePassword_Failed_5);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_ResetPassword ncb = new CL_Cmd_ResetPassword();
        ncb.SetCmdType(NetCmdType.CMD_CL_ResetPassword);
        ncb.OldPasswordCrc1 = OldCrc1;
        ncb.OldPasswordCrc2 = OldCrc2;
        ncb.OldPasswordCrc3 = OldCrc3;
        ncb.NewPasswordCrc1 = NewCrc1;
        ncb.NewPasswordCrc2 = NewCrc2;
        ncb.NewPasswordCrc3 = NewCrc3;
        NetServices.Instance.Send<CL_Cmd_ResetPassword>(ncb);
        return true;
    }
    public bool ChangeSecPassword(string OldPassword,string NewPassword)
    {
        //修改二级密码
        CL_Cmd_ChangeSecondPassword ncb = new CL_Cmd_ChangeSecondPassword();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeSecondPassword);

        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength, FishDataInfo.PasswordLength, OldPassword, StringCheckType.SCT_Password))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ChangeSecPass_Faile_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength, FishDataInfo.PasswordLength, NewPassword, StringCheckType.SCT_Password))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ChangeSecPass_Faile_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (NewPassword.CompareTo(OldPassword) == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ChangeSecPass_Faile_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }

        NormalAccountInfo ad1 = new NormalAccountInfo();
        if (!NativeInterface.ComputeCrc("", OldPassword, out ad1.CRC1, out ad1.CRC2, out ad1.CRC3))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ChangeSecPass_Faile_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false ;
        }

        NormalAccountInfo ad2 = new NormalAccountInfo();
        if (!NativeInterface.ComputeCrc("", NewPassword, out ad2.CRC1, out ad2.CRC2, out ad2.CRC3))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ChangeSecPass_Faile_5);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }

        GlobalLogon.Instance.AccountData.SaveAccountData();

        ncb.dwOldCrc1 = ad1.CRC1;
        ncb.dwOldCrc2 = ad1.CRC2;
        ncb.dwOldCrc3 = ad1.CRC3;

        ncb.dwNewCrc1 = ad2.CRC1;
        ncb.dwNewCrc2 = ad2.CRC2;
        ncb.dwNewCrc3 = ad2.CRC3;

        GlobalLogon.Instance.AccountData.TempPhoneInfo.Clear();
        GlobalLogon.Instance.AccountData.TempAccountInfo.Clear();
        GlobalLogon.Instance.AccountData.TempAccountInfo.UID = GlobalLogon.Instance.AccountData.AccountInfo.UID;
        GlobalLogon.Instance.AccountData.TempAccountInfo.CRC1 = ad2.CRC1;
        GlobalLogon.Instance.AccountData.TempAccountInfo.CRC2 = ad2.CRC2;
        GlobalLogon.Instance.AccountData.TempAccountInfo.CRC3 = ad2.CRC3;

        NetServices.Instance.Send<CL_Cmd_ChangeSecondPassword>(ncb);

        return true;
    }
    //public void ChangeRoleCustFaceID(UInt32 FaceID) //修改玩家的自定义头像的结果
    //{
    //    return;
    //    m_RoleInfo.SetFaceID(FaceID);

    //    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_CustFace_Sucess);//使用用户头像成功 
    //    MsgEventHandle.HandleMsg(pUOM);

    //    tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);
    //}
    public void ResetSceneInfo()
    {
        CL_Cmd_ResetRoleInfo ncb = new CL_Cmd_ResetRoleInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_ResetRoleInfo);
        NetServices.Instance.Send<CL_Cmd_ResetRoleInfo>(ncb);
    }
    public void UpdateDayByServer() //按服务器进行每天的更新处理
    {
        //服务器进行天数更新的时候
        m_RoleInfo.SetGameTime(0);
        m_RoleInfo.SetProduction(0);
        m_RoleInfo.SetSendGiffSum(0);
        m_RoleInfo.SetAcceptGiffSum(0);
        m_RoleInfo.BeneFitCount = 0;
        m_RoleInfo.BenefitTime = 0;

        int256Function.Clear(m_RoleInfo.GetTaskStates());

        m_RoleInfo.SetOnlineMin(0);
        m_RoleInfo.SetOnlineRewardStates(0);
        SystemTime.Instance.ClearLogTimeByDay();//记录时间设置到当前

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagOnlineRewardChangeEvent pEventOnline = new tagOnlineRewardChangeEvent();
        MsgEventHandle.HandleMsg(pEventOnline);
    }
    //public bool ChangeUserFaceID(UInt16 PicSize)
    //{
    //    return SendRequestUserFaceID(PicSize);
    //}
    //public bool UpperLoadPicChunk(UInt16 StarIndex,UInt16 Size,Byte[] pArray)
    //{
    //    return SendUserFacePicChunk(StarIndex, Size, pArray);
    //}
    //玩家属性改变
    private bool SendChangeRoleGender(bool bGender)
    {
        //玩家修改性别
        if (m_RoleInfo.GetGender() == bGender)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_Gender_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }     
        //判断物品是否足够
        UInt32 ItemID = FishConfig.Instance.m_SystemConfig.ChangeGenderNeedItemID;
        UInt32 ItemSum = FishConfig.Instance.m_SystemConfig.ChangeGenderNeedItemSum;
        if (ItemID != 0 && ItemSum > 0 && PlayerRole.Instance.ItemManager.GetItemSum(ItemID) < ItemSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_Gender_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_ChangeRoleGender ncb = new CL_Cmd_ChangeRoleGender();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleGender);
        ncb.bGender = bGender;
        NetServices.Instance.Send<CL_Cmd_ChangeRoleGender>(ncb);
        return true;
    }
    private bool SendChangeRoleNickName(string NickName)
    {
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.NickNameMinLength, FishDataInfo.NickNameLength, NickName, StringCheckType.SCT_Normal))
        {
            //提示当前用户名称 无法使用
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_NickName_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (NickName.CompareTo(m_RoleInfo.GetNickName()) == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_NickName_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(m_RoleInfo.GetNickName().IndexOf("游客") != 0 && m_RoleInfo.GetNickName().IndexOf("用户") != 0)//系统生成的名称 第一次是可以修改的 
        {
            UInt32 ItemID = FishConfig.Instance.m_SystemConfig.ChangeNickNameNeedItemID;
            UInt32 ItemSum = FishConfig.Instance.m_SystemConfig.ChangeNickNameNeedItemSum;
            if (ItemID !=0 && ItemSum >0 && PlayerRole.Instance.ItemManager.GetItemSum(ItemID) < ItemSum)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_NickName_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }
        CL_Cmd_ChangeRoleNickName ncb = new CL_Cmd_ChangeRoleNickName();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleNickName);
        ncb.NickName = NickName;
        NetServices.Instance.Send<CL_Cmd_ChangeRoleNickName>(ncb);
        return true;
    }
    private bool SendChangeRoleNormalFaceID(UInt32 FaceID)
    {
        //玩家切换普通的头像ID
        if (FaceID == m_RoleInfo.GetFaceID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ChangeFace_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        CL_Cmd_ChangeRoleNormalFaceID ncb = new CL_Cmd_ChangeRoleNormalFaceID();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleNormalFaceID);
        ncb.dwFaceID = FaceID;
        NetServices.Instance.Send<CL_Cmd_ChangeRoleNormalFaceID>(ncb);
        return true;
    }
    //private bool SendRequestUserFaceID(UInt16 PicSize)//用户向服务器端请求上传指定大小的图片
    //{
    //    if (PicSize <= 0)
    //        return false;
    //    CL_Cmd_RequestUserFaceID ncb = new CL_Cmd_RequestUserFaceID();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_RequestUserFaceID);
    //    ncb.PicSize = PicSize;
    //    NetServices.Instance.Send<CL_Cmd_RequestUserFaceID>(ncb);
    //    return true;
    //}
    //private bool SendUserFacePicChunk(UInt16 StarIndex, UInt16 Size, Byte[] pArray)
    //{
        
    //    if (StarIndex < 0 || Size < 0 || pArray == null || pArray.Length > Size)
    //        return false;
    //    CL_Cmd_UpLoadingchunk ncb = new CL_Cmd_UpLoadingchunk();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_UpLoadingchunk);
    //    ncb.StartIndex = StarIndex;
    //    ncb.Size = Size;
    //    ncb.ImgData = pArray;
    //    NetServices.Instance.Send<CL_Cmd_UpLoadingchunk>(ncb);
    //    return true;
    //}
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_ChangeRoleExp:
                return HandleLCChangeRoleExp(obj);
            case NetCmdType.CMD_LC_ChangeRoleLevel:
                return HandleLCChangeRoleLevel(obj);
            case NetCmdType.CMD_LC_ChangeRoleGender:
                return HandleLCChangeRoleGender(obj);
            case NetCmdType.CMD_LC_ChangeRoleNickName:
                return HandleLCChangeRoleNickName(obj);
            case NetCmdType.CMD_LC_ChangeRoleFaceID:
                return HandleLCChangeRoleFaceID(obj);
            case NetCmdType.CMD_LC_ChangeRoleGlobe:
                return HandleLCChangeRoleGlobe(obj);
            case NetCmdType.CMD_LC_ChangeRoleMedal:
                return HandleLCChangeRoleMedal(obj);
            case NetCmdType.CMD_LC_ChangeRoleCurrency:
                return HandleLCChangeRoleCurrency(obj);
            //case NetCmdType.CMD_LC_ResponseUserFaceID:
            //    return HandleLCResponseUserFaceID(obj);
            //case NetCmdType.CMD_LC_UpLoadingResponse:
            //    return HandleLCUpLoadChunckResult(obj);
            //case NetCmdType.CMD_LC_UpLoadingFinish:
            //    return HandleLCUpLoadFinish(obj);
            //case NetCmdType.CMD_LC_UpLoadFtpError:
            //    return HandleUpLoadFtpError(obj);
            case NetCmdType.CMD_LC_ChangeRoleTitle:
                return HandleChangeRoleTitle(obj);
            case NetCmdType.CMD_LC_ChangeRoleAchievementPoint:
                return HandleChangeRoleAchievementPoint(obj);
            case NetCmdType.CMD_LC_ChangeRoleCharmValue:
                return HandleChangeRoleCharm(obj);
            case NetCmdType.CMD_LC_ChangeRoleSendGiffSum:
                return HandleChangeRoleSendGiffSum(obj);
            case NetCmdType.CMD_LC_ChangeRoleAcceptGiffSum:
                return HandleChangeRoleAcceptGiffSum(obj);
            case NetCmdType.CMD_LC_DayChange:
                return HandleSystemDay(obj);
            case NetCmdType.CMD_LC_ChangeRoleTaskStates:
                return HandleRoleTaskStates(obj);
            case NetCmdType.CMD_LC_ChangeRoleAchievementStates:
                return HandleRoleAchievementStates(obj);
            case NetCmdType.CMD_LC_ChangeRoleActionStates:
                return HandleRoleActionStates(obj);
            case NetCmdType.CMD_LC_ChangeRoleAchievementIndex:
                return HandleRoleAchievementIndex(obj);
            case NetCmdType.CMD_LC_ResetRoleGlobel:
                return HandleResetRoleGlobel(obj);
            case NetCmdType.CMD_LC_ResetRoleInfo:
                return HandleResetUserInfo(obj);
            case NetCmdType.CMD_LC_ChangeRoleCheckData:
                return HandleChangeRoleCheckData(obj);
            case NetCmdType.CMD_LC_ChangeRoleIsShowIpAddress:
                return HandleChangeRoleIsShowIpAddress(obj);
            case NetCmdType.CMD_LC_NewDay:
                return HandleChangeClientNewDay(obj);
            case NetCmdType.CMD_LC_AccountResetAccount:
                return HandleResetAccountName(obj);
            case NetCmdType.CMD_LC_ResetPassword:
                return HandleChangePassword(obj);
            case NetCmdType.CMD_LC_ChangeRoleExChangeStates:
                return HandleChangeExChangeStates(obj);
            case NetCmdType.CMD_LC_ChangeRoleTotalRecharge:
                return HandleChangeRoleTotalRecharge(obj);
            case NetCmdType.CMD_LC_RoleProtect:
                return HandleRoleProtect(obj);
            case NetCmdType.CMD_LC_ChangeRoleParticularStates:
                return HandleChangeRoleParticularStates(obj);
            case NetCmdType.CMD_LC_ChangeRoleIsFirstPayGlobel:
                return HandleRoleIsFirstPayGlobel(obj);
            case NetCmdType.CMD_LC_ChangeRoleIsFirstPayCurrcey:
                return HandleRoleIsFirstPayCurrcey(obj);
            case NetCmdType.CMD_LC_ChangeRoleMonthCardInfo:
                return HandleRoleMonthCardInfo(obj);
            case NetCmdType.CMD_LC_ChangeRoleRateValue:
                return HandleRoleRateValue(obj);
            case NetCmdType.CMD_LC_GetMonthCardReward:
                return HandleRoleGetMonthCardReward(obj);
            case NetCmdType.CMD_LC_ChangeRoleVipLevel:
                return HandleChangeRoleVipLevel(obj);
            case NetCmdType.CMD_LC_ChangeRoleCashSum:
                return HandleChangeRoleCashSum(obj);
            case NetCmdType.CMD_LC_SetSecondPassword:
                return HandleResetBindPhone(obj);
            case NetCmdType.CMD_LC_ChangeSecondPassword:
                return HandleChangeRoleSecondPassword(obj);
            case NetCmdType.CMD_LC_ChangeRoleShareStates:
                return HandleChangeRoleShareStates(obj);
            case NetCmdType.CMD_LC_OpenShareUI:
                return HandleOpenShareUI(obj);
        }
        return true;
    }
    bool HandleLCChangeRoleExp(NetCmdBase obj)
    {
        //当用户经验发生变化的时候
        LC_Cmd_ChangRoleExp ncb = (LC_Cmd_ChangRoleExp)obj;
        m_RoleInfo.SetExp(ncb.dwExp);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleLCChangeRoleLevel(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleLevel ncb = (LC_Cmd_ChangeRoleLevel)obj;
        UInt16 OldLevel = m_RoleInfo.GetLevel();
        m_RoleInfo.SetLevel(ncb.wLevel);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        UInt16 RewardID = 0;
        if (FishConfig.Instance.m_LevelRewardMap.m_LevelRewardMap.ContainsKey(ncb.wLevel))
            RewardID = FishConfig.Instance.m_LevelRewardMap.m_LevelRewardMap[ncb.wLevel];
        tagRoleLevelChangeEvent pLevelEvent = new tagRoleLevelChangeEvent(OldLevel, ncb.wLevel, RewardID);
        MsgEventHandle.HandleMsg(pLevelEvent);

        return true;
    }
    bool HandleLCChangeRoleGender(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleGender ncb = (LC_Cmd_ChangeRoleGender)obj;
        m_RoleInfo.SetGender(ncb.bGender);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_Gender_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        return true;
    }
    bool HandleLCChangeRoleNickName(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleNickName ncb = (LC_Cmd_ChangeRoleNickName)obj;
        if (ncb.Result)
        {
            m_RoleInfo.SetNickName(ncb.NickName);

            tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Role_NickName_Sucess : UserOperateMessage.UOM_Role_NickName_Failed_3));
        MsgEventHandle.HandleMsg(pUOM);
        return true;
    }
    bool HandleLCChangeRoleFaceID(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleFaceID ncb = (LC_Cmd_ChangeRoleFaceID)obj;
        m_RoleInfo.SetFaceID(ncb.dwFaceID);

        if(ncb.dwFaceID <= 100)
        {
            //系统头像
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_ChangeFace_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            //自定义头像
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_CustFace_Sucess);//使用用户头像成功 
            MsgEventHandle.HandleMsg(pUOM);
        }
        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleLCChangeRoleGlobe(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleGlobe ncb = (LC_Cmd_ChangeRoleGlobe)obj;
        m_RoleInfo.SetGlobel(Convert.ToUInt32(ncb.dwGlobeNum + m_RoleInfo.GetGlobel()));

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleLCChangeRoleMedal(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleMedal ncb = (LC_Cmd_ChangeRoleMedal)obj;
        m_RoleInfo.SetMedal(ncb.dwMedalNum);
        m_RoleInfo.SetTotalUseMedal(ncb.TotalUseMedal);//总使用的奖牌数量

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleLCChangeRoleCurrency(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleCurrency ncb = (LC_Cmd_ChangeRoleCurrency)obj;
        m_RoleInfo.SetCurrency(ncb.dwCurrencyNum);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeRoleTitle(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleTitle ncb = (LC_Cmd_ChangeRoleTitle)obj;
        m_RoleInfo.SetTitleID(ncb.TitleID);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Title_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeRoleAchievementPoint(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleAchievementPoint ncb = (LC_Cmd_ChangeRoleAchievementPoint)obj;
        m_RoleInfo.SetAchievementPoint(ncb.dwAchievementPoint);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeRoleCharm(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleCharmValue ncb = (LC_Cmd_ChangeRoleCharmValue)obj;
        m_RoleInfo.SetCharmInfo(ncb.CharmArray);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeRoleSendGiffSum(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleSendGiffSum ncb = (LC_Cmd_ChangeRoleSendGiffSum)obj;
        m_RoleInfo.SetSendGiffSum(Convert.ToByte(ncb.SendGiffSum));

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeRoleAcceptGiffSum(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleAcceptGiffSum ncb = (LC_Cmd_ChangeRoleAcceptGiffSum)obj;
        m_RoleInfo.SetAcceptGiffSum(Convert.ToByte(ncb.AcceptGiffSum));

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleSystemDay(NetCmdBase obj)
    {
        LC_Cmd_DayChange ncb = (LC_Cmd_DayChange)obj;

        if(ncb.IsDayUpdate)
        {
            PlayerRole.Instance.UpdateDayByServer();
            return true;
        }
        else
        {
            SystemTime.Instance.SetSystemTime(ncb.Year, ncb.Month, ncb.Day, ncb.Hour, ncb.Min, ncb.Sec);
            return true;
        }
        return false;
    }

    bool HandleRoleTaskStates(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleTaskStates ncb = (LC_Cmd_ChangeRoleTaskStates)obj;
        int256Function.SetBitStates(m_RoleInfo.GetTaskStates(),ncb.Index,ncb.States);
        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleRoleAchievementStates(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleAchievementStates ncb = (LC_Cmd_ChangeRoleAchievementStates)obj;
        int256Function.SetBitStates(m_RoleInfo.GetAchievementStates(), ncb.Index, ncb.States);
        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleRoleActionStates(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleActionStates ncb = (LC_Cmd_ChangeRoleActionStates)obj;
        int256Function.SetBitStates(m_RoleInfo.GetActionStates(), ncb.Index, ncb.States);
        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleRoleAchievementIndex(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleAchievementIndex ncb = (LC_Cmd_ChangeRoleAchievementIndex)obj;
        RoleMe.SetAchievementIndex(ncb.AchievementIndex);
        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleResetRoleGlobel(NetCmdBase obj)
    {
        LC_Cmd_ResetRoleGlobel ncb = (LC_Cmd_ResetRoleGlobel)obj;
        RoleMe.SetGlobel(ncb.TotalGlobelSum);

        RoleMe.SetLotteryScore(ncb.LotteryScore);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleResetUserInfo(NetCmdBase obj)
    {
        LC_Cmd_ResetRoleInfo ncb = (LC_Cmd_ResetRoleInfo)obj;
        RoleMe.SetRoleInfo(ncb.RoleInfo);
        ServerSetting.SetCallbckUrl(Utility.IPToString(ncb.OperateIP));
        PlayerRole.Instance.RoleExChange.SetPlayerRoleIsShowExChange();

        PlayerRole.Instance.TableManager.ClearTableInfo();
        PlayerRole.Instance.OnRoleResetOtherInfo();//清空数据并且提供事件

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleChangeRoleCheckData(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleCheckData ncb = (LC_Cmd_ChangeRoleCheckData)obj;
        RoleMe.SetCheckData(ncb.CheckData);

        tagCheckChangeEvent pEvent = new tagCheckChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleChangeRoleIsShowIpAddress(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleIsShowIpAddress ncb = (LC_Cmd_ChangeRoleIsShowIpAddress)obj;
        RoleMe.SetIsShowIpAddress(ncb.IsShowIpAddress);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Role_IsShowIpAddress_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeClientNewDay(NetCmdBase obj)
    {
        LC_Cmd_NewDay ncb = (LC_Cmd_NewDay)obj;
        PlayerRole.Instance.MailManager.UpdateByNewDay();
        PlayerRole.Instance.RelationManager.UpdateByDay();
        PlayerRole.Instance.GiffManager.OnNewDay();
        return true;
    }
    bool HandleResetAccountName(NetCmdBase obj)
    {
        LC_Cmd_AccountResetAccount ncb = (LC_Cmd_AccountResetAccount)obj;
        if (ncb.Result)
        {
            //重置账号成功了
            PlayerRole.Instance.RoleInfo.RoleMe.SetAccountIsCanReset(false);

            GlobalLogon.Instance.AccountData.LoadTempAccountInfo();
            GlobalLogon.Instance.AccountData.SaveAccountData();


            GlobalHallUIMgr.Instance.ChangeAccountSeccess();            
        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Role_ResetAccount_Sucess : UserOperateMessage.UOM_Role_ResetAccount_Failed_5));
        MsgEventHandle.HandleMsg(pUOM);
        return true;
    }
    bool HandleChangePassword(NetCmdBase obj)
    {
        LC_Cmd_ResetPassword ncb = (LC_Cmd_ResetPassword)obj;
        if (ncb.Result)
        {
            GlobalLogon.Instance.SaveAccountPassWord(ncb.NewPasswordCrc1, ncb.NewPasswordCrc2, ncb.NewPasswordCrc3);        
        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Role_ChangePassword_Sucess : UserOperateMessage.UOM_Role_ChangePassword_Failed_6));
        MsgEventHandle.HandleMsg(pUOM);
        return true;
    }
    bool HandleChangeExChangeStates(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleExChangeStates ncb = (LC_Cmd_ChangeRoleExChangeStates)obj;
        PlayerRole.Instance.RoleInfo.RoleMe.SetExChangeStates(ncb.States);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeRoleTotalRecharge(NetCmdBase obj)
    {
        //修改玩家总充值的数量
        LC_Cmd_ChangeRoleTotalRecharge ncb = (LC_Cmd_ChangeRoleTotalRecharge)obj;
        PlayerRole.Instance.RoleInfo.RoleMe.SetTotalRechargeSum(ncb.Sum);
        
        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeRoleParticularStates(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleParticularStates ncb = (LC_Cmd_ChangeRoleParticularStates)obj;
        PlayerRole.Instance.RoleInfo.RoleMe.SetParticularStates(ncb.ParticularStates);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleRoleProtect(NetCmdBase obj)
    {
        //当触发新手保护的时候
        LC_Cmd_RoleProtect ncb = (LC_Cmd_RoleProtect)obj;

        tagRoleProtectEvent pEvent = new tagRoleProtectEvent();
        //pEvent.nCount = ncb.nCount;
        //pEvent.ncd = ncb.ncd;

        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleRoleIsFirstPayGlobel(NetCmdBase obj)
    {
        PlayerRole.Instance.RoleInfo.RoleMe.SetIsFirstPayGlobel(false);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleRoleIsFirstPayCurrcey(NetCmdBase obj)
    {
        PlayerRole.Instance.RoleInfo.RoleMe.SetIsFirstPayCurrcey(false);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleRoleMonthCardInfo(NetCmdBase obj)
    {
        //处理月卡数据
        LC_Cmd_ChangeRoleMonthCardInfo ncb = (LC_Cmd_ChangeRoleMonthCardInfo)obj;
        PlayerRole.Instance.RoleInfo.RoleMe.SetMonthCardID(ncb.MonthCardID);
        PlayerRole.Instance.RoleInfo.RoleMe.SetMonthCardEndTime(ncb.MonthCardEndTime);
        //设置完毕后 我们触发事件 
        tagRoleMonthCardChangeEvent pEvent = new tagRoleMonthCardChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleRoleRateValue(NetCmdBase obj)
    {
        //玩家倍率发生变化了 
        LC_Cmd_ChangeRoleRateValue ncb = (LC_Cmd_ChangeRoleRateValue)obj;

        PlayerRole.Instance.RoleInfo.RoleMe.SetRateValue(ncb.RateValue);
       
        //触发事件
        tagRoleRateValueChangeEvent pEvent = new tagRoleRateValueChangeEvent(ncb.OpenRateIndex);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleRoleGetMonthCardReward(NetCmdBase obj)
    {
        LC_Cmd_GetMonthCardReward ncb = (LC_Cmd_GetMonthCardReward)obj;
        if(ncb.Result)
        {
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthCardRewardTime(SystemTime.Instance.GetSystemTimeTotalSecond());//领取时间记录为当前时间
        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent(ncb.Result ? UserOperateMessage.UOM_Role_MonthCardReward_Sucess : UserOperateMessage.UOM_Role_MonthCardReward_Failed_3);
        MsgEventHandle.HandleMsg(pUOM);

        //发送事件
        tagRoleMonthCardRewardEvent pEvent = new tagRoleMonthCardRewardEvent(ncb.Result);//参数玩家获得月卡奖励的事件
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleChangeRoleVipLevel(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleVipLevel ncb = (LC_Cmd_ChangeRoleVipLevel)obj;
        PlayerRole.Instance.RoleInfo.RoleMe.SetVipLevel(ncb.VipLevel);

        tagRoleVipChangeEvent pEvent = new tagRoleVipChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeRoleCashSum(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleCashSum ncb = (LC_Cmd_ChangeRoleCashSum)obj;
        PlayerRole.Instance.RoleInfo.RoleMe.SetCashSum(ncb.CashSum);
        PlayerRole.Instance.RoleInfo.RoleMe.SetTotalCashSum(ncb.TotalCashSum);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleChangeRoleShareStates(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleShareStates ncb = (LC_Cmd_ChangeRoleShareStates)obj;
        PlayerRole.Instance.RoleInfo.RoleMe.SetShareStates(ncb.ShareStates);

        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleOpenShareUI(NetCmdBase obj)
    {
        tagRoleOpenShareUIEvent pEvent = new tagRoleOpenShareUIEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleResetBindPhone(NetCmdBase obj)
    {
        //让玩家重新绑定手机
        tagRoleResetBindPhoneEvent pEvent = new tagRoleResetBindPhoneEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleChangeRoleSecondPassword(NetCmdBase obj)
    {
        //玩家修改二级密码的结果
        LC_Cmd_ChangeSecondPassword ncb = (LC_Cmd_ChangeSecondPassword)obj;
        bool Result = ncb.Result;

        if(Result)
        {
            GlobalLogon.Instance.AccountData.SaveAccountData();
        }
        else
        {
            GlobalLogon.Instance.AccountData.LoadAccountData();
        }

        //修改密码的结果 进行消息提示
        //
        tagUserOperationEvent pUOM = new tagUserOperationEvent(Result ? UserOperateMessage.UOM_ChangeSecPass_Success : UserOperateMessage.UOM_ChangeSecPass_Faile_6);
        MsgEventHandle.HandleMsg(pUOM);

        //触发事件
        tagRoleChangeSecPwdEvent pEvent = new tagRoleChangeSecPwdEvent(ncb.Result);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    //bool HandleLCResponseUserFaceID(NetCmdBase obj)//玩家FTP头像上的修改放入到FTP的处理类里面去
    //{
    //    LC_Cmd_ResponseUserFaceID ncb = (LC_Cmd_ResponseUserFaceID)obj;
    //    HeadPhotoUpload.Instance.m_UploadCmdList.Add(obj);
    //    return true;
    //}
    //bool HandleLCUpLoadChunckResult(NetCmdBase obj)
    //{
    //    LC_Cmd_UpLoadingResponse ncb = (LC_Cmd_UpLoadingResponse)obj;
    //    HeadPhotoUpload.Instance.m_UploadCmdList.Add(obj);
    //    return true;
    //}
    //bool HandleLCUpLoadFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_UpLoadingFinish ncb = (LC_Cmd_UpLoadingFinish)obj;

    //    m_RoleInfo.SetFaceID(ncb.dwFaceID);
    //    tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);

    //    HeadPhotoUpload.Instance.m_UploadCmdList.Add(obj);
    //    return true;
    //}
    //bool HandleUpLoadFtpError(NetCmdBase obj)
    //{
    //    LC_Cmd_UpLoadFtpError ncb = (LC_Cmd_UpLoadFtpError)obj;
    //    HeadPhotoUpload.Instance.m_UploadCmdList.Add(obj);
    //    return true;
    //}
}