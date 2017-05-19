using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家的宝箱的管理器
class RoleChest
{
    //private List<tagChestInfo> ChestList = new List<tagChestInfo>();

    private tagChestInfo ActionChestInfo;
    public RoleChest()
    {
        OnClear();
    }
    public void OnClear()
    {
        ActionChestInfo = null;
    }
    //公共函数
    public tagChestInfo GetNowActionChest()
    {
        return ActionChestInfo;
    }
    public bool GetChestRewardByIndex(Byte ChestIndex)
    {
        return SendGetChestReward(ChestIndex);
    }
    public bool CloseNowChest()
    {
        return SendCloseActionChest();
    }
    private bool IsCanOpenChestIndex(Byte ChestIndex)
    {
        if (ActionChestInfo == null)
            return false;
        foreach(ChestOnceStates var in ActionChestInfo.ChestArray)
        {
            if (var.Index == ChestIndex)
                return false;
        }
        return true;
    }
    private bool SendGetChestReward(Byte ChestIndex)
    {
        //获取宝箱的奖励 当前激活的宝箱
        //1.判断宝箱数量
        if (ActionChestInfo == null)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_OpenChest_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        tagChestInfo pActionChest = ActionChestInfo;
        if (pActionChest.ActionTime == 0 || pActionChest.ChestTypeID == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_OpenChest_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!IsCanOpenChestIndex(ChestIndex))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_OpenChest_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_ChestInfo.ChestMap.ContainsKey(pActionChest.ChestTypeID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_OpenChest_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //当前集合的宝箱存在 判断宝箱的打开次数
        if (pActionChest.OpenSum >= FishConfig.Instance.m_ChestInfo.ChestMap[pActionChest.ChestTypeID].CostInfo.MaxCostNum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_OpenChest_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_ChestInfo.ChestMap[pActionChest.ChestTypeID].CostInfo.CostMap.ContainsKey((Byte)(pActionChest.OpenSum + 1)))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_OpenChest_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_ChestInfo.ChestMap[pActionChest.ChestTypeID].CostInfo.CostMap[(Byte)(pActionChest.OpenSum + 1)])
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_OpenChest_Failed_5);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_GetChestReward ncb = new CL_Cmd_GetChestReward();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetChestReward);
        ncb.ChestIndex = ChestIndex;//无实际作用
        ncb.ChestOnlyID = pActionChest.ChestOnlyID;
        NetServices.Instance.Send<CL_Cmd_GetChestReward>(ncb);
        return true;
    }
    private bool SendCloseActionChest()//发送关闭当前宝箱的操作
    {
        if (ActionChestInfo == null)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_CloseChest_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        tagChestInfo pActionChest = ActionChestInfo;
        if (pActionChest.ActionTime == 0 || pActionChest.ChestTypeID == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_CloseChest_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_ChestInfo.ChestMap.ContainsKey(pActionChest.ChestTypeID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_CloseChest_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //关闭当前激活的宝箱
        CL_Cmd_CloseChest ncb = new CL_Cmd_CloseChest();
        ncb.SetCmdType(NetCmdType.CMD_CL_CloseChest);
        ncb.ChestOnlyID = pActionChest.ChestOnlyID;
        NetServices.Instance.Send<CL_Cmd_CloseChest>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            //case NetCmdType.CMD_LC_AddChest:
            //    return HandleAddChest(obj);
            case NetCmdType.CMD_LC_ActionChest:
                return HandleActionChest(obj);
            case NetCmdType.CMD_LC_ChestEnd:
                return HandleChestEnd(obj);
            case NetCmdType.CMD_LC_GetChestReward:
                return HandleGetChestReward(obj);
            case NetCmdType.CMD_LC_ResetChest:
                return HandleResetChest(obj);
        }
        return true;
    }
    //bool HandleAddChest(NetCmdBase obj)
    //{
    //    //客户端添加一个新的宝箱
    //    LC_Cmd_AddChest ncb = (LC_Cmd_AddChest)obj;
    //    if (!FishConfig.Instance.m_ChestInfo.ChestMap.ContainsKey(ncb.ChestTypeID))
    //        return false;
    //    tagChestInfo pChest = new tagChestInfo();
    //    pChest.ChestTypeID = ncb.ChestTypeID;
    //    pChest.ActionTime = 0;
    //    pChest.OpenSum = 0;
    //    pChest.ChestOnlyID = ncb.ChestOnlyID;
    //    ChestList.Add(pChest);

    //    return true;
    //}
    bool HandleActionChest(NetCmdBase obj)
    {
        LC_Cmd_ActionChest ncb = (LC_Cmd_ActionChest)obj;
        if (ActionChestInfo != null)
            return false;
        if (!FishConfig.Instance.m_ChestInfo.ChestMap.ContainsKey(ncb.ChestTypeID))
            return false;
        ActionChestInfo = new tagChestInfo();
        ActionChestInfo.ChestOnlyID = ncb.ChestOnlyID;
        ActionChestInfo.ChestTypeID = ncb.ChestTypeID;
        ActionChestInfo.OpenSum = ncb.OpenSum;
        ActionChestInfo.ActionTime = Utility.GetTickCount() - Convert.ToUInt32(FishConfig.Instance.m_ChestInfo.ChestMap[ncb.ChestTypeID].ExisteSec * 1000 - ncb.EndSec);
        //ActionChestInfo.ActionOpemStates = ncb.ActionOpenStates;
        //ActionChestInfo.ActionRewardStates = ncb.ActionRewardStates;
        if (ncb.ChestArray != null && ncb.ChestArray.Length > 0)
        {
            for (int i = 0; i < ncb.ChestArray.Length; ++i)
            {
                ActionChestInfo.ChestArray.Add(ncb.ChestArray[i]);
            }
        }
        tagChestActionEvent pEvent = new tagChestActionEvent(ActionChestInfo, ncb.ChestSum, ncb.IsReset);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleChestEnd(NetCmdBase obj)
    {
        LC_Cmd_ChestEnd ncb = (LC_Cmd_ChestEnd)obj;
        if (ActionChestInfo == null)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_CloseChest_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (ActionChestInfo.ChestOnlyID != ncb.ChestOnlyID)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_CloseChest_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        ActionChestInfo = null;
        tagChestEndEvent pEvent = new tagChestEndEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM2 = new tagUserOperationEvent(UserOperateMessage.UOM_Chest_CloseChest_Sucess);
        MsgEventHandle.HandleMsg(pUOM2);

        return true;
    }
    bool HandleGetChestReward(NetCmdBase obj)
    {
        LC_Cmd_GetChestReward ncb = (LC_Cmd_GetChestReward)obj;

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Chest_OpenChest_Sucess : UserOperateMessage.UOM_Chest_OpenChest_Failed_6));
        MsgEventHandle.HandleMsg(pUOM);

        if (ActionChestInfo == null)
            return false;
        tagChestInfo pChest = ActionChestInfo;
        if (pChest.ChestOnlyID != ncb.ChestOnlyID)
            return false;
        if (!FishConfig.Instance.m_ChestInfo.ChestMap[pChest.ChestTypeID].RewardInfo.RewardMap.ContainsKey(ncb.ChestStates.RewardID))
            return false;
        if (!FishConfig.Instance.m_ChestInfo.ChestMap[pChest.ChestTypeID].RewardInfo.RewardMap[ncb.ChestStates.RewardID].RewardLog.ContainsKey(ncb.ChestStates.RewardOnlyID))
            return false;
        tagChestsRewardOnce pOnce = FishConfig.Instance.m_ChestInfo.ChestMap[pChest.ChestTypeID].RewardInfo.RewardMap[ncb.ChestStates.RewardID].RewardLog[ncb.ChestStates.RewardOnlyID];
        if (ncb.Result)
        {
            pChest.OpenSum += 1;
            ChestOnceStates pChestOnce = new ChestOnceStates();
            pChestOnce = ncb.ChestStates;
            tagChestRewardEvent pEvent = new tagChestRewardEvent(pChestOnce);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleResetChest(NetCmdBase obj)
    {
        LC_Cmd_ResetChest ncb = (LC_Cmd_ResetChest)obj;
        ActionChestInfo = null;
        tagChestResetEvent pEvent = new tagChestResetEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}
