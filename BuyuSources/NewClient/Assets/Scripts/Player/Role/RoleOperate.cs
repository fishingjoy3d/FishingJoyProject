using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

enum RoleOperateError //运营服务器的错误枚举
{
    //实名验证
    ROE_RealNameVerification_Sucess   =  1,//实名验证成功
    ROE_RealNameVerification_IDENTITY_IsBeUse,//身份证被使用了
    ROE_RealNameVerification_IDENTITY_Error,//身份证格式错误
    ROE_RealNameVerification_IDENTITY_NotName,//身份证与实际名称不符合
    ROE_RealNameVerification_TimeOut,//实名验证超时
    ROE_RealNameVerification_IsExists,//实名验证重复提交


    //手机绑定
    ROE_PhoneVerificationNum_Sucess =50,//获取验证码成功
    ROE_PhoneVerificationNum_PhoneError,//电话号码错误
    ROE_PhoneVerificationNum_PhoneBeUse,//电话号码被使用了
    ROE_PhoneVerificationNum_IsExists,//重复提交
    ROE_PhoneVerificationNum_WriteSend,//提交绑定手机成功 等待短信发送
    ROE_BindPhone_Sucess,//绑定手机成功
    ROE_BindPhone_BindNumError,//验证码错误
    ROE_BindPhone_BindTimeOut,//验证码超时
    ROE_BindPhone_SendSmsInfoError,//发送验证码错误

    //邮箱绑定
    ROE_BindMail_Sucess = 100,//绑定邮箱成功
    ROE_BindMail_IsBeUse,//邮箱呗使用了 无法绑定
    ROE_BindPhone_MailError,//邮箱错误无法绑定

    //购买实体物品
    ROE_BuyEntityItem_Sucess = 150,
    ROE_BuyEntityItem_Failed,

    ROE_PhonePay_Sucess = 200,
    ROE_PhonePay_System,//内部错误
    ROE_PhonePay_HttpError,//外部错误
}

class RoleOperate
{
    public RoleOperate()
    {
        OnClear();
    }
    public void OnClear()
    {

    }

    //连接到运营服务器相关的功能
    public bool OnRealNameVerification(string Name,string IDCard)
    {
        //进行身份证绑定 
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MIN_NAME_LENTH, FishDataInfo.MAX_NAME_LENTH,Name,StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_RealNameVerification_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MIN_IDENTITY_LENGTH, FishDataInfo.MAX_IDENTITY_LENGTH, IDCard, StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_RealNameVerification_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //发送命令到服务器
        CL_Cmd_RealNameVerification ncb = new CL_Cmd_RealNameVerification();
        ncb.SetCmdType(NetCmdType.CMD_CL_RealNameVerification);
        ncb.Name = Name;
        ncb.IDCard = IDCard;
        NetServices.Instance.Send<CL_Cmd_RealNameVerification>(ncb);
        return true;
    }
    public bool OnResetGetPhoneVerificationNum() //重新绑定已经的手机 重置二级密码
    {
        return OnGetPhoneVerificationNum(PlayerRole.Instance.EntityManager.GetRoleEntityInfo().Phone);
    }
    public bool OnGetPhoneVerificationNum(UInt64 PhoneNum)//获取手机验证码
    {
        //if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MIN_PHONE_LENGTH, FishDataInfo.MAX_PHONE_LENGTH, PhoneNum, StringCheckType.SCT_Normal))
        //{
        //    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_GetPhoneVerificationNum_Failed_1);
        //    MsgEventHandle.HandleMsg(pUOM);
        //    return false;
        //}
        if (PlayerRole.Instance.EntityManager.GetRoleEntityInfo().Phone != 0 && PlayerRole.Instance.EntityManager.GetRoleEntityInfo().Phone != PhoneNum)
        {
            return false;
        }
        if(!FishDataInfo.IsCanUsePhone(PhoneNum))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_GetPhoneVerificationNum_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_GetPhoneVerificationNum ncb = new CL_Cmd_GetPhoneVerificationNum();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetPhoneVerificationNum);
        ncb.PhoneNumber = PhoneNum;
        NetServices.Instance.Send<CL_Cmd_GetPhoneVerificationNum>(ncb);

        GlobalLogon.Instance.AccountData.SaveAccountData();
        GlobalLogon.Instance.AccountData.TempPhoneInfo.PhoneNumber = PhoneNum;

        return true;
    }
    public bool OnBindPhone(UInt32 BindValue, string SecPassword)
    {
        //if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MIN_PHONE_LENGTH, FishDataInfo.MAX_PHONE_LENGTH, PhoneName, StringCheckType.SCT_Normal))
        //{
        //    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_GetPhoneVerificationNum_Failed_1);
        //    MsgEventHandle.HandleMsg(pUOM);
        //    return false;
        //}
        //if (PhoneName <= 1000000000)
        //{
        //    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_GetPhoneVerificationNum_Failed_1);
        //    MsgEventHandle.HandleMsg(pUOM);
        //    return false;
        //}

        //绑定二级密码
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.PasswordMinLength, FishDataInfo.PasswordLength, SecPassword, StringCheckType.SCT_Password))
        {
            return false;
        }
        PhoneAccountInfo ad = new PhoneAccountInfo();
        if (!NativeInterface.ComputeCrc("", SecPassword, out ad.phoneCRC1, out ad.phoneCRC2, out ad.phoneCRC3))
        {
            return false;
        }

        CL_Cmd_BindPhone ncb = new CL_Cmd_BindPhone();
        ncb.SetCmdType(NetCmdType.CMD_CL_BindPhone);
        //ncb.PhoneNumber = PhoneName;
        ncb.BindNumber = BindValue;

        ncb.SecPasswordCrc1 = ad.phoneCRC1;
        ncb.SecPasswordCrc2 = ad.phoneCRC2;
        ncb.SecPasswordCrc3 = ad.phoneCRC3;

        GlobalLogon.Instance.AccountData.TempPhoneInfo.phoneCRC1 = ad.phoneCRC1;
        GlobalLogon.Instance.AccountData.TempPhoneInfo.phoneCRC2 = ad.phoneCRC2;
        GlobalLogon.Instance.AccountData.TempPhoneInfo.phoneCRC3 = ad.phoneCRC3; 

        NetServices.Instance.Send<CL_Cmd_BindPhone>(ncb);
        return true;
    }
    public bool OnBindEmail(string Email)
    {
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MIN_EMAIL_LENGTH, FishDataInfo.MAX_EMAIL_LENGTH, Email, StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_BindEmail_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_BindEmail ncb = new CL_Cmd_BindEmail();
        ncb.SetCmdType(NetCmdType.CMD_CL_BindEmail);
        ncb.EMail = Email;
        NetServices.Instance.Send<CL_Cmd_BindEmail>(ncb);
        return true;
    }

    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_RealNameVerification:
                return HandleRealNameVerification(obj);
            case NetCmdType.CMD_LC_GetPhoneVerificationNum:
                return HandleGetPhoneVerificationNum(obj);
            case NetCmdType.CMD_LC_BindPhone:
                return HandleBindPhone(obj);
            case NetCmdType.CMD_LC_BuyEntityItem:
                return HandleBuyEntityItem(obj);
            case NetCmdType.CMD_LC_BindEmail:
                return HandleBindEmail(obj);
            case NetCmdType.CMD_LC_PhonePay:
                return HandlePhonePay(obj);
            case NetCmdType.CMD_LC_AddNormalOrderID:
                return HandleAddNormalRechargeOrderID(obj);
        }
        return true;
    }
    bool HandleRealNameVerification(NetCmdBase obj)
    {
        LC_Cmd_RealNameVerification ncb = (LC_Cmd_RealNameVerification)obj;
        //发送消息

        switch(ncb.ErrorID)
        {
            case (Byte)RoleOperateError.ROE_RealNameVerification_Sucess:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_RealNameVerification_Sucess);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_RealNameVerification_IDENTITY_IsBeUse:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_RealNameVerification_Failed_4);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_RealNameVerification_IDENTITY_Error:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_RealNameVerification_Failed_3);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_RealNameVerification_IDENTITY_NotName:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_RealNameVerification_Failed_5);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_RealNameVerification_TimeOut:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_RealNameVerification_Failed_6);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_RealNameVerification_IsExists:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_RealNameVerification_Failed_7);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
        }
        return true;
    }
    bool HandleGetPhoneVerificationNum(NetCmdBase obj)
    {
        LC_Cmd_GetPhoneVerificationNum ncb = (LC_Cmd_GetPhoneVerificationNum)obj;

        switch (ncb.ErrorID)
        {
            case (Byte)RoleOperateError.ROE_PhoneVerificationNum_Sucess:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_GetPhoneVerificationNum_Sucess);
                    MsgEventHandle.HandleMsg(pUOM);

                    tagEntityBindPhoneEvent pEvent = new tagEntityBindPhoneEvent(true, false, (Byte)RoleOperateError.ROE_PhoneVerificationNum_Sucess);
                    MsgEventHandle.HandleMsg(pEvent);
                }
                break;
            case (Byte)RoleOperateError.ROE_PhoneVerificationNum_PhoneError:
                {

                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_GetPhoneVerificationNum_Failed_2);
                    MsgEventHandle.HandleMsg(pUOM);

                    tagEntityBindPhoneEvent pEvent = new tagEntityBindPhoneEvent(false, false, (Byte)RoleOperateError.ROE_PhoneVerificationNum_PhoneError);
                    MsgEventHandle.HandleMsg(pEvent);
                }
                break;
            case (Byte)RoleOperateError.ROE_PhoneVerificationNum_PhoneBeUse:
                {
  
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_GetPhoneVerificationNum_Failed_3);
                    MsgEventHandle.HandleMsg(pUOM);

                    tagEntityBindPhoneEvent pEvent = new tagEntityBindPhoneEvent(false, false, (Byte)RoleOperateError.ROE_PhoneVerificationNum_PhoneBeUse);
                    MsgEventHandle.HandleMsg(pEvent);
                }
                break;
            case (Byte)RoleOperateError.ROE_PhoneVerificationNum_IsExists:
                {

                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_GetPhoneVerificationNum_Failed_4);
                    MsgEventHandle.HandleMsg(pUOM);

                    tagEntityBindPhoneEvent pEvent = new tagEntityBindPhoneEvent(false, false, (Byte)RoleOperateError.ROE_PhoneVerificationNum_IsExists);
                    MsgEventHandle.HandleMsg(pEvent);

                }
                break;
            case (Byte)RoleOperateError.ROE_PhoneVerificationNum_WriteSend:
                {

                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_GetPhoneVerificationNum_Sucess_1);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
        }
        return true;
    }
    bool HandleBindPhone(NetCmdBase obj)
    {
        LC_Cmd_BindPhone ncb = (LC_Cmd_BindPhone)obj;

        switch (ncb.ErrorID)
        {
            case (Byte)RoleOperateError.ROE_BindPhone_Sucess:
                {
                    GlobalLogon.Instance.AccountData.LoadTempPhoneInfo();
                    GlobalLogon.Instance.AccountData.SaveAccountData();

                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_BindPhone_Sucess);
                    MsgEventHandle.HandleMsg(pUOM);

                    tagEntityBindPhoneEvent pEvent = new tagEntityBindPhoneEvent(true, ncb.IsFirstBind, (Byte)RoleOperateError.ROE_BindPhone_Sucess);
                    MsgEventHandle.HandleMsg(pEvent);
                }
                break;
            case (Byte)RoleOperateError.ROE_BindPhone_BindNumError:
                {

                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_BindPhone_Failed_1);
                    MsgEventHandle.HandleMsg(pUOM);

                    tagEntityBindPhoneEvent pEvent = new tagEntityBindPhoneEvent(false, false, (Byte)RoleOperateError.ROE_BindPhone_BindNumError);
                    MsgEventHandle.HandleMsg(pEvent);
                }
                break;
            case (Byte)RoleOperateError.ROE_BindPhone_BindTimeOut:
                {

                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_BindPhone_Failed_2);
                    MsgEventHandle.HandleMsg(pUOM);

                    tagEntityBindPhoneEvent pEvent = new tagEntityBindPhoneEvent(false, false, (Byte)RoleOperateError.ROE_BindPhone_BindTimeOut);
                    MsgEventHandle.HandleMsg(pEvent);
                }
                break;
            case (Byte)RoleOperateError.ROE_BindPhone_SendSmsInfoError:
                {

                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_BindPhone_Failed_3);
                    MsgEventHandle.HandleMsg(pUOM);

                    tagEntityBindPhoneEvent pEvent = new tagEntityBindPhoneEvent(false, false, (Byte)RoleOperateError.ROE_BindPhone_SendSmsInfoError);
                    MsgEventHandle.HandleMsg(pEvent);
                }
                break;
        }
        return true;
    }
    bool HandleBuyEntityItem(NetCmdBase obj)
    {
        LC_Cmd_BuyEntityItem ncb = (LC_Cmd_BuyEntityItem)obj;

        switch (ncb.ErrorID)
        {
            case (Byte)RoleOperateError.ROE_BuyEntityItem_Sucess:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_EntityItem_Sucess);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_BuyEntityItem_Failed:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_EntityItem_Failed_1);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
        }
        return true;
    }
    bool HandleBindEmail(NetCmdBase obj)
    {
        LC_Cmd_BindEmail ncb = (LC_Cmd_BindEmail)obj;

        switch (ncb.ErrorID)
        {
            case (Byte)RoleOperateError.ROE_BindMail_Sucess:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_BindEmail_Sucess);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_BindMail_IsBeUse:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_BindEmail_Failed_2);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_BindPhone_MailError:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_BindEmail_Failed_3);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
        }
        return true;
    }
    bool HandlePhonePay(NetCmdBase obj)
    {
        LC_Cmd_PhonePay ncb = (LC_Cmd_PhonePay)obj;
        //玩家充值话费的结果
        switch (ncb.ErrorID)
        {
            case (Byte)RoleOperateError.ROE_PhonePay_Sucess:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_PhonePay_Sucess);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_PhonePay_System:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_PhonePay_Failed_1);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
            case (Byte)RoleOperateError.ROE_PhonePay_HttpError:
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Operate_PhonePay_Failed_2);
                    MsgEventHandle.HandleMsg(pUOM);
                }
                break;
        }
        return true;
    }
    bool HandleAddNormalRechargeOrderID(NetCmdBase obj)
    {
        LC_Cmd_AddNormalOrderID ncb = (LC_Cmd_AddNormalOrderID)obj;
        //客户端接收到命令了
        //触发事件
        string TransID = "";
        for(int i = 0; i < ncb.Transid.Length; ++i)
        {
            if (ncb.Transid[i] != 0)
                TransID += (char)ncb.Transid[i];
            else
                break;
        }
        string Sign = "";
        for(int i = 0; i < ncb.Sign.Length; ++i)
        {
            if(ncb.Sign[i] != 0)
                Sign += (char)ncb.Sign[i];
            else
                break;
        }
        //UnityEngine.Debug.Log("ID:" + TransID);
        //UnityEngine.Debug.Log("Sign:" + Sign);
        tagAddRechargeOrderInfoEvent pEvent = new tagAddRechargeOrderInfoEvent(ncb.Result, ncb.ShopID, ncb.OrderID, TransID, Sign);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
}