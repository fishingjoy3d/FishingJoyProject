using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RoleExChange
{
    public bool IsShowExChangeUI = false;
    public RoleExChange()
    {
        OnClear();
    }
    public void OnClear()
    {
    }
    public void SetPlayerRoleIsShowExChange()
    {
        IsShowExChangeUI = IsCanShowExChangeUI();
    }
    private bool IsCanShowExChangeUI()
    {
        //是否需要显示 兑换码的UI 
        //1.获得玩家当前的兑换码的ID 进行测试
        string ChannelName = "";
        if (SDKMgr.Instance.LoginData != null && SDKMgr.Instance.LoginData.ChannelLabel!= null)
            ChannelName = SDKMgr.Instance.LoginData.ChannelLabel.ToLower().Trim();
        UInt32 ChannelID = 0;
        if (ChannelName.Length != 0)
            ChannelID = Crc.Crc32(Encoding.Default.GetBytes(ChannelName.ToCharArray()), 0, ChannelName.Length);
        //获得了平台的ID我们进行处理  便利全部的兑换码类型 
        //ID  TypeID  ChannelID  RewardID  
        //ChannelID  0表示全部平台通用
        foreach(tagExChange var in FishConfig.Instance.m_ExChangeMap.m_ExChangeMap.Values)
        {
            if (var.ChannelID == 0 || var.ChannelID == ChannelID)
                return true;
        }
        return false;
    }
    public bool OnUseExChangeCode(string code)
    {
        if(!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.ExChangeCode_MinLength,FishDataInfo.ExChangeCode_Length,code,StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ExChange_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_RoleUseExChangeCode msg =new CL_Cmd_RoleUseExChangeCode();
        msg.SetCmdType(NetCmdType.CMD_CL_RoleUseExChangeCode);
        msg.ExChangeCode = code;
        NetServices.Instance.Send<CL_Cmd_RoleUseExChangeCode>(msg);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_RoleUseExChangeCode:
                return HandleUseExChangeResult(obj);
        }
        return true;
    }
    bool HandleUseExChangeResult(NetCmdBase obj)
    {
        LC_Cmd_RoleUseExChangeCode ncb = (LC_Cmd_RoleUseExChangeCode)obj;
        if (ncb.Result)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ExChange_Sucess);
            MsgEventHandle.HandleMsg(pUOM);

            if(FishConfig.Instance.m_ExChangeMap.m_ExChangeMap.ContainsKey(ncb.ID))
            {
                tagUseExChangeEvent pEvent = new tagUseExChangeEvent(FishConfig.Instance.m_ExChangeMap.m_ExChangeMap[ncb.ID].RewardID);
                MsgEventHandle.HandleMsg(pEvent);
            }
            return true;
        }
        else
        {
            if(ncb.ID == 0)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ExChange_Failed_2);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
            else
            {
                if(!FishConfig.Instance.m_ExChangeMap.m_ExChangeMap.ContainsKey(ncb.ID))
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ExChange_Failed_2);
                    MsgEventHandle.HandleMsg(pUOM);
                    return false;
                }
                if((PlayerRole.Instance.RoleInfo.RoleMe.GetExChangeStates() & FishConfig.Instance.m_ExChangeMap.m_ExChangeMap[ncb.ID].TypeID) != 0)
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ExChange_Failed_3);
                    MsgEventHandle.HandleMsg(pUOM);
                    return false;
                }
                else
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_ExChange_Failed_4);
                    MsgEventHandle.HandleMsg(pUOM);
                    return false;
                }
            }
        }
    }
}