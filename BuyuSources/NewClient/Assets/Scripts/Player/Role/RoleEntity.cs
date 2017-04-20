using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家的实体数据管理器
class RoleEntity
{
    private bool m_IsLoadInfo = false;
    private tagRoleAddressInfo RoleEntityInfo = new tagRoleAddressInfo();
    private bool m_IsLoadByServer = false;
    public RoleEntity()
    {
        OnClear();
    }
    public void OnClear()
    {
        //清理数据
        m_IsLoadInfo = false;
        RoleEntityInfo.Email = "";
        RoleEntityInfo.IdentityID = "";
        RoleEntityInfo.Name = "";
        RoleEntityInfo.Phone = 0;
        RoleEntityInfo.EntityItemUseAddres = "";
        RoleEntityInfo.EntityItemUseName = "";
        RoleEntityInfo.EntityItemUsePhone = 0;
        m_IsLoadByServer = false;
    }
    public void ResetInfo()
    {
        OnClear();
        tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }
    //public void EntityTest()
    //{
    //    RoleEntityInfo.Addres = "测试数据";
    //    RoleEntityInfo.Email = "测试数据";
    //    RoleEntityInfo.IdentityID = "测试数据";
    //    RoleEntityInfo.Name = "测试数据";
    //    RoleEntityInfo.Phone = "测试数据";
    //    m_IsLoadInfo = true;

    //    tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);
    //}
    public tagRoleAddressInfo GetRoleEntityInfo()
    {
        if(m_IsLoadInfo)
        {
            return RoleEntityInfo;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                SendLoadRoleEntityInfo();
            }
            //EntityTest();
            return null;
        }
    }
    //public bool ChangeRoleEntityName(string Name)
    //{
    //    return SendChangeRoleName(Name);
    //}
    //public bool ChangeRoleEntityPhone(string Phone)
    //{
    //    return SendChangeRolePhone(Phone);
    //}
    //public bool ChangeRoleEntityEmail(string Email)
    //{
    //    return SendChangeRoleEmail(Email);
    //}
    //public bool ChangeRoleEntityID(string EntityID)
    //{
    //    return SendChangeRoleEntityID(EntityID);
    //}
    public bool ChangeRoleEntityEntityItemUseAddress(string Adder)
    {
        return SendChangeRoleEntityItemUseAddress(Adder);
    }
    public bool ChangeRoleEntityEntityItemUseName(string Name)
    {
        return SendChangeRoleEntityItemUseName(Name);
    }
    public bool ChangeRoleEntityEntityItemUsePhone(UInt64 Phone)
    {
        return SendChangeRoleEntityItemUsePhone(Phone);
    }
    public bool CheckEntityInfoIsCanUser()
    {
        if (FishDataInfo.IsCanUsePhone(RoleEntityInfo.EntityItemUsePhone) &&
            RoleEntityInfo.EntityItemUseName != "" &&
            RoleEntityInfo.IdentityID != "" &&
            RoleEntityInfo.EntityItemUseAddres != "")
            return true;
        else
            return false;
    }
    public bool CheckIsCanPhonePay()
    {
        if (FishDataInfo.IsCanUsePhone(RoleEntityInfo.EntityItemUsePhone))
            return true;
        else
            return false;
    }

    private void SendLoadRoleEntityInfo()
    {
        CL_Cmd_LoadUserEntity ncb = new CL_Cmd_LoadUserEntity();
        ncb.SetCmdType(NetCmdType.CMD_CL_LoadUserEntity);
        NetServices.Instance.Send<CL_Cmd_LoadUserEntity>(ncb);
    }
    //private bool SendChangeRoleName(string Name)
    //{
    //    if (Name == null || Name.Length > FishDataInfo.MAX_NAME_LENTH)
    //    {
    //        tagErrorCodeEvent pCode = new tagErrorCodeEvent(ErrorCode.EC_RoleEntity_NameError);
    //        MsgEventHandle.HandleMsg(pCode);
    //        return false;
    //    }
    //    CL_Cmd_ChangeRoleName ncb = new CL_Cmd_ChangeRoleName();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleName);
    //    ncb.Name = Name;
    //    NetServices.Instance.Send<CL_Cmd_ChangeRoleName>(ncb);
    //    return true;
    //}
    //private bool SendChangeRolePhone(string Phone)
    //{
    //    if (Phone == null || Phone.Length > FishDataInfo.MAX_PHONE_LENGTH)
    //    {
    //        tagErrorCodeEvent pCode = new tagErrorCodeEvent(ErrorCode.EC_RoleEntity_Phone);
    //        MsgEventHandle.HandleMsg(pCode);
    //        return false;
    //    }
    //    CL_Cmd_ChangeRolePhone ncb = new CL_Cmd_ChangeRolePhone();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRolePhone);
    //    ncb.Phone = Phone;
    //    NetServices.Instance.Send<CL_Cmd_ChangeRolePhone>(ncb);
    //    return true;
    //}
    //private bool SendChangeRoleEmail(string Email)
    //{
    //    if (Email == null || Email.Length > FishDataInfo.MAX_EMAIL_LENGTH)
    //    {
    //        tagErrorCodeEvent pCode = new tagErrorCodeEvent(ErrorCode.EC_RoleEntity_EMail);
    //        MsgEventHandle.HandleMsg(pCode);
    //        return false;
    //    }
    //    CL_Cmd_ChangeRoleEmail ncb = new CL_Cmd_ChangeRoleEmail();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleEmail);
    //    ncb.Email = Email;
    //    NetServices.Instance.Send<CL_Cmd_ChangeRoleEmail>(ncb);
    //    return true;
    //}
    private bool SendChangeRoleEntityItemUseAddress(string Address)
    {
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MIN_ADDRESS_LENGTH, FishDataInfo.MAX_ADDRESS_LENGTH,Address,StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Entity_EntityItemUseAddress_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_ChangeRoleEntityItemUseAddress ncb = new CL_Cmd_ChangeRoleEntityItemUseAddress();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleEntityItemUseAddress);
        ncb.EntityItemUseAddress = Address;
        NetServices.Instance.Send<CL_Cmd_ChangeRoleEntityItemUseAddress>(ncb);
        return true;
    }
    private bool SendChangeRoleEntityItemUseName(string Name)
    {
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MIN_NAME_LENTH, FishDataInfo.MAX_NAME_LENTH, Name, StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Entity_EntityItemUseName_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_ChangeRoleEntityItemUseName ncb = new CL_Cmd_ChangeRoleEntityItemUseName();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleEntityItemUseName);
        ncb.EntityItemUseName = Name;
        NetServices.Instance.Send<CL_Cmd_ChangeRoleEntityItemUseName>(ncb);
        return true;
    }
    private bool SendChangeRoleEntityItemUsePhone(UInt64 Phone)
    {
        //if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MIN_PHONE_LENGTH, FishDataInfo.MAX_PHONE_LENGTH, Phone, StringCheckType.SCT_Normal))
        //{
        //    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Entity_EntityItemUsePhone_Failed_1);
        //    MsgEventHandle.HandleMsg(pUOM);
        //    return false;
        //}
        if(!FishDataInfo.IsCanUsePhone(Phone))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Entity_EntityItemUsePhone_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_ChagneRoleEntityItemUsePhone ncb = new CL_Cmd_ChagneRoleEntityItemUsePhone();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChagneRoleEntityItemUsePhone);
        ncb.EntityItemUsePhone = Phone;
        NetServices.Instance.Send<CL_Cmd_ChagneRoleEntityItemUsePhone>(ncb);
        return true;
    }
    //private bool SendChangeRoleEntityID(string EntityID)
    //{
    //    if (EntityID == null || EntityID.Length > FishDataInfo.MAX_IDENTITY_LENGTH)
    //    {
    //        tagErrorCodeEvent pCode = new tagErrorCodeEvent(ErrorCode.EC_RoleEntity_EntityID);
    //        MsgEventHandle.HandleMsg(pCode);
    //        return false;
    //    }
    //    CL_Cmd_ChangeRoleEntityID ncb = new CL_Cmd_ChangeRoleEntityID();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleEntityID);
    //    ncb.IdentityID = EntityID;
    //    NetServices.Instance.Send<CL_Cmd_ChangeRoleEntityID>(ncb);
    //    return true;
    //}

    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_LoadUserEntity:
                return HandleLoadRoleEntityInfo(obj);
            case NetCmdType.CMD_LC_ChangeRoleName:
                return HandleChangeRoleName(obj);
            case NetCmdType.CMD_LC_ChangeRolePhone:
                return HandleChangeRolePhone(obj);
            case NetCmdType.CMD_LC_ChangeRoleEmail:
                return HandleChangeRoleEmail(obj);
            //case NetCmdType.CMD_LC_ChangeRoleAddress:
            //    return HandleChangeRoleAddress(obj);
            case NetCmdType.CMD_LC_ChangeRoleEntityID:
                return HandleChangeRoleEntityID(obj);
            case NetCmdType.CMD_LC_ChangeRoleEntityItemUseName:
                return HandleChangeRoleEntityItemUseName(obj);
            case NetCmdType.CMD_LC_ChangeRoleEntityItemUsePhone:
                return HandleChangeRoleEntityItemUsePhone(obj);
            case NetCmdType.CMD_LC_ChangeRoleEntityItemUseAddress:
                return HandleChangeRoleEntityItemUseAddress(obj);
        }
        return true;
    }
    bool HandleLoadRoleEntityInfo(NetCmdBase obj)
    {
        LC_Cmd_LoadUserEntity ncb = (LC_Cmd_LoadUserEntity)obj;
        RoleEntityInfo = ncb.UserEntityInfo;
        m_IsLoadInfo = true;

        tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleChangeRoleName(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleName ncb = (LC_Cmd_ChangeRoleName)obj;
        RoleEntityInfo.Name = ncb.Name;

        tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleChangeRolePhone(NetCmdBase obj)
    {
        LC_Cmd_ChangeRolePhone ncb = (LC_Cmd_ChangeRolePhone)obj;
        RoleEntityInfo.Phone = ncb.Phone;

        tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChangeRoleEmail(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleEmail ncb = (LC_Cmd_ChangeRoleEmail)obj;
        RoleEntityInfo.Email = ncb.Email;

        tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    //bool HandleChangeRoleAddress(NetCmdBase obj)
    //{
    //    LC_Cmd_ChangeRoleAddress ncb = (LC_Cmd_ChangeRoleAddress)obj;
    //    RoleEntityInfo.Addres = ncb.Addres;

    //    tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);

    //    return true;
    //}
    bool HandleChangeRoleEntityID(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleEntityID ncb = (LC_Cmd_ChangeRoleEntityID)obj;
        RoleEntityInfo.IdentityID = ncb.IdentityID;

        tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleChangeRoleEntityItemUseName(NetCmdBase obj)
    {
        LC_Cmd_CHangeRoleEntityItemUseName ncb = (LC_Cmd_CHangeRoleEntityItemUseName)obj;
        RoleEntityInfo.EntityItemUseName = ncb.EntityItemUseName;
        tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Entity_EntityItemUseName_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        return true;
    }
    bool HandleChangeRoleEntityItemUsePhone(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleEntityItemUsePhone ncb = (LC_Cmd_ChangeRoleEntityItemUsePhone)obj;
        RoleEntityInfo.EntityItemUsePhone = ncb.EntityItemUsePhone;
        tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Entity_EntityItemUsePhone_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        return true;
    }
    bool HandleChangeRoleEntityItemUseAddress(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleEntityItemUseAddress ncb = (LC_Cmd_ChangeRoleEntityItemUseAddress)obj;
        RoleEntityInfo.EntityItemUseAddres = ncb.EntityItemUseAddress;
        tagEntityChangeEvent pEvent = new tagEntityChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Entity_EntityItemUseAddress_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        return true;
    }
}