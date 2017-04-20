using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家赠送管理器
class RoleGiff
{
    private bool m_IsLoadInfo = false;
    private Dictionary<UInt32, tagGiffClientInfo> GiffMap = new Dictionary<UInt32, tagGiffClientInfo>();
    private bool m_IsLoadNowSend = false;
    private Dictionary<UInt32, Byte> GissNowDaySendMap = new Dictionary<UInt32, Byte>();
    private bool m_IsLoadByServer = false;
    private bool m_IsLoadDayByServer = false;
   // private bool m_IsExistsGiff = false;
    public RoleGiff()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_IsLoadInfo = false;
        m_IsLoadNowSend = false;
        m_IsLoadByServer = false;
        m_IsLoadDayByServer = false;
        GiffMap.Clear();
        GissNowDaySendMap.Clear();
    }
    public void ResetInfo()
    {
        OnClear();
        tagGiffChangeEvent pEvent = new tagGiffChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }
    public void OnNewDay()
    {
        if (GiffMap.Count > 0)
        {
            for (int i = 0; i < GiffMap.Count; ++i)
            {
                if (GiffMap.Values.ElementAt<tagGiffClientInfo>(i).DiffTime != 0xff && GiffMap.Values.ElementAt<tagGiffClientInfo>(i).DiffTime < 250)
                {
                    GiffMap.Values.ElementAt<tagGiffClientInfo>(i).DiffTime += 1;
                }
            }

            tagGiffChangeEvent pEvent = new tagGiffChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        //tagGiffSendInfoChangeEvent pSendEvent = new tagGiffSendInfoChangeEvent();
        //MsgEventHandle.HandleMsg(pSendEvent);
    }
    public void OnDayChange()
    {
        GissNowDaySendMap.Clear();//发送过赠送的数据

        tagGiffSendInfoChangeEvent pSendEvent = new tagGiffSendInfoChangeEvent();
        MsgEventHandle.HandleMsg(pSendEvent);
    }

    public Dictionary<UInt32, tagGiffClientInfo> GetAllGiffMap()
    {
        if(m_IsLoadInfo)
        {
            return GiffMap;
        }
        else
        {
            //GiffTest();
            //应该只需要发送一次
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                GiffMap.Clear();
                SendGetRoleGiff();
            }
            return null;
        }
    }
    public Dictionary<UInt32, Byte> GetNowDaySendGiffInfo()
    {
        if (m_IsLoadNowSend)
        {
            return GissNowDaySendMap;
        }
        else
        {
            if (!m_IsLoadDayByServer)
            {
                m_IsLoadDayByServer = true;
                GissNowDaySendMap.Clear();
                SendGetRoleNowDaySendGiffInfo();
            }
            return null;
        }
    }
    public bool SendGiffToTargetUser(UInt32 dwUserID)
    {
        return SendGiffToOtherUser(dwUserID);
    }
    public bool AcceptGiff(UInt32 GiffID)
    {
        return SendAcceptGiff(GiffID);
    }
    public bool AcceptAllGiff()
    {
        return SendAcceptAllGiff();
    }


    private void SendGetRoleGiff()
    {
        CL_Cmd_GetRoleGiff ncb = new CL_Cmd_GetRoleGiff();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetRoleGiff);
        NetServices.Instance.Send<CL_Cmd_GetRoleGiff>(ncb);
    }
    private void SendGetRoleNowDaySendGiffInfo()
    {
        CL_Cmd_GetRoleNowDaySendGiff ncb = new CL_Cmd_GetRoleNowDaySendGiff();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetRoleNowDaySendGiff);
        NetServices.Instance.Send<CL_Cmd_GetRoleNowDaySendGiff>(ncb);
    }
    private bool SendGiffToOtherUser(UInt32 dwUserID)
    {
        //1.判断发送次数
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetSendGiffSum() >= FishConfig.Instance.m_GiffInfo.SendSumLimitByDay)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Giff_SendGiff_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (GissNowDaySendMap.ContainsKey(dwUserID) && GissNowDaySendMap[dwUserID] >= FishConfig.Instance.m_GiffInfo.SendOnceUserLimitByDay)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Giff_SendGiff_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Giff_SendGiff_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_AddRoleGiff ncb = new CL_Cmd_AddRoleGiff();
        ncb.SetCmdType(NetCmdType.CMD_CL_AddRoleGiff);
        ncb.dwDestUserID = dwUserID;
        NetServices.Instance.Send<CL_Cmd_AddRoleGiff>(ncb);
        return true;
    }
    private bool SendAcceptGiff(UInt32 GiffID)
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetAcceptGiffSum() >= FishConfig.Instance.m_GiffInfo.AcceptSubLimitByDay)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Giff_AcceptGiff_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!GiffMap.ContainsKey(GiffID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Giff_AcceptGiff_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_GetRoleGiffReward ncb = new CL_Cmd_GetRoleGiffReward();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetRoleGiffReward);
        ncb.GiffID = GiffID;
        NetServices.Instance.Send<CL_Cmd_GetRoleGiffReward>(ncb);
        return true;
    }
    private bool SendAcceptAllGiff()
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetAcceptGiffSum() >= FishConfig.Instance.m_GiffInfo.AcceptSubLimitByDay)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Giff_GetAllGiff_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (GiffMap.Count <= 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Giff_GetAllGiff_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_GetAllRoleGiffReward ncb = new CL_Cmd_GetAllRoleGiffReward();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetAllRoleGiffReward);
        NetServices.Instance.Send<CL_Cmd_GetAllRoleGiffReward>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetRoleGiff:
                return HandleLoadRoleGiff(obj);
            //case NetCmdType.CMD_LC_GetRoleGiffFinish:
            //    return HandleLoadRoleGiffFinish(obj);
            case NetCmdType.CMD_LC_AddRoleGiffResult:
                return HandleAddRoleGiffResult(obj);
            case NetCmdType.CMD_LC_AddBeRoleGiff:
                return HandleAddBeRoleGiff(obj);
            case NetCmdType.CMD_LC_GetRoleGiffReward:
                return HandleAcceptRoleGiffResult(obj);
            case NetCmdType.CMD_LC_GetAllRoleGiffReward:
                return HandleAcceptAllGiffResult(obj);
            case NetCmdType.CMD_LC_GetRoleNowDaySendGiff:
                return HandleLoadRoleNowDaySendGiffInfo(obj);
            //case NetCmdType.CMD_LC_GetRoleNowDaySendGiffFinish:
            //    return HandleLoadRoleNowDaySendGiffInfoFinish(obj);
            case NetCmdType.CMD_LC_DelRoleGiff:
                return HandleDelRoleGiff(obj);
        }
        return true;
    }
    bool HandleLoadRoleGiff(NetCmdBase obj)
    {
        LC_Cmd_GetRoleGiff ncb = (LC_Cmd_GetRoleGiff)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            GiffMap.Clear();
        }
        for (int i = 0; i < ncb.Sum; ++i)
        {
            GiffMap.Add(ncb.Array[i].OnlyID, ncb.Array[i]);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadInfo = true;
            tagGiffChangeEvent pEvent = new tagGiffChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //bool HandleLoadRoleGiffFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_GetRoleGiffFinish ncb = (LC_Cmd_GetRoleGiffFinish)obj;
    //    //FriendSysManager.Instance.UpdatePresentList();
       

    //    return true;
    //}

    bool HandleAddRoleGiffResult(NetCmdBase obj)
    {
        LC_Cmd_AddRoleGiffResult ncb = (LC_Cmd_AddRoleGiffResult)obj;
        if (ncb.Result)
        {
            //赠送成功显示提示消息
            if (GissNowDaySendMap.ContainsKey(ncb.dwDestUserID))
            {
                GissNowDaySendMap[ncb.dwDestUserID] += 1;
            }
            else
            {
                GissNowDaySendMap.Add(ncb.dwDestUserID, 1);
            }
            PlayerRole.Instance.HandleEvent(EventTargetType.ET_SendGiff, 0, 1);

            tagGiffSendInfoOnceChangeEvent pEvent = new tagGiffSendInfoOnceChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Giff_SendGiff_Sucess : UserOperateMessage.UOM_Giff_SendGiff_Failed_4));
        MsgEventHandle.HandleMsg(pUOM);
        return true;
    }
    bool HandleAddBeRoleGiff(NetCmdBase obj)
    {
        LC_Cmd_AddBeRoleGiff ncb = (LC_Cmd_AddBeRoleGiff)obj;
        GiffMap.Add(ncb.GiffInfo.OnlyID, ncb.GiffInfo);

        //CheckIsExistsGiff();
        //接收别人赠送礼物消息
       // FriendSysManager.Instance.AddPresentMsg(ncb.GiffInfo.OnlyID, ncb.GiffInfo);
        tagGiffAddEvent pEvent = new tagGiffAddEvent(ncb.GiffInfo.OnlyID);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleAcceptRoleGiffResult(NetCmdBase obj)
    {
        LC_Cmd_GetRoleGiffReward ncb = (LC_Cmd_GetRoleGiffReward)obj;

        //领取成功消息
        GiffMap.Remove(ncb.GiffID);

       // CheckIsExistsGiff();

        tagGiffDelEvent pEvent = new tagGiffDelEvent(ncb.GiffID);
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Giff_AcceptGiff_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        return true;
    }
    bool HandleAcceptAllGiffResult(NetCmdBase obj)
    {
        LC_Cmd_GetAllRoleGiffReward ncb = (LC_Cmd_GetAllRoleGiffReward)obj;
        for (UInt16 i = 0; i < ncb.Sum; ++i)
        {
            GiffMap.Remove(ncb.Array[i]);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            tagGiffChangeEvent pEvent = new tagGiffChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Giff_GetAllGiff_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    bool HandleLoadRoleNowDaySendGiffInfo(NetCmdBase obj)
    {
        LC_Cmd_GetRoleNowDaySendGiff ncb = (LC_Cmd_GetRoleNowDaySendGiff)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            GissNowDaySendMap.Clear();
        }
        //获取全部玩家今天发生的数据
        for (UInt16 i = 0; i < ncb.Sum; ++i)
        {
            GissNowDaySendMap.Add(ncb.Array[i].dwUserID, ncb.Array[i].SendSum);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadNowSend = true;
            tagGiffSendInfoChangeEvent pEvent = new tagGiffSendInfoChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //bool HandleLoadRoleNowDaySendGiffInfoFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_GetRoleNowDaySendGiffFinish ncb = (LC_Cmd_GetRoleNowDaySendGiffFinish)obj;
        
    //    return true;
    //}
    bool HandleDelRoleGiff(NetCmdBase obj)
    {
        LC_Cmd_DelRoleGiff ncb = (LC_Cmd_DelRoleGiff)obj;
        for(int i =0;i<ncb.Sum;++i)
        {
            GiffMap.Remove(ncb.Array[i]);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            GissNowDaySendMap.Clear();//将今天目前记录的全部清理掉

            tagGiffChangeEvent pEvent = new tagGiffChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);

            tagGiffSendInfoChangeEvent pSendEvent = new tagGiffSendInfoChangeEvent();
            MsgEventHandle.HandleMsg(pSendEvent);
        }
        return true;
    }
}