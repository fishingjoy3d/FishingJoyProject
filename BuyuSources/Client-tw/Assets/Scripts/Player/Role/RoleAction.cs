using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
class RoleActionManager
{
    //任务管理类 无须处理具体的逻辑 只需要进行简单的处理就OK了
    private bool m_IsLoadInfo = false;
    private Dictionary<Byte, RoleActionBase> m_ActionMap = new Dictionary<Byte, RoleActionBase>();//当前正在进行的任务列表
   // private bool m_IsExistsFinishAction = false;
    private bool m_IsLoadByServer = false;
    //public bool IsExistsFinishItem()
    //{
    //    //判断当前成就了是否有已经完成 待领取的成就
    //    return m_IsExistsFinishAction;
    //}
    //public void SetExistsFinishItem(bool States)
    //{
    //    if (m_IsExistsFinishAction != States)
    //    {
    //        m_IsExistsFinishAction = States;
    //        //发送事件
    //        tagActionFinishStatesChagneEvent pEvent = new tagActionFinishStatesChagneEvent();
    //        MsgEventHandle.HandleMsg(pEvent);
    //    }
    //    else
    //        m_IsExistsFinishAction = States;
    //}
    //private void CheckIsExistsFinishItem()
    //{
    //    if (m_ActionMap.Count == 0)
    //        SetExistsFinishItem(false);
    //    else
    //    {
    //        foreach (RoleActionBase var in m_ActionMap.Values)
    //        {
    //            foreach (tagActionEventConfig var2 in var.pConfig.EventMap.Values)
    //            {
    //                if (IsCanFinishAction(var.pConfig.ActionID, var2.ID))
    //                {
    //                    SetExistsFinishItem(true);
    //                    return;
    //                }
    //            }
    //        }
    //        SetExistsFinishItem(false);
    //    }
    //}
    public void ResetInfo()
    {
        OnClear();
        tagActionChangeEvent pEvent = new tagActionChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }
    public RoleActionManager()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_IsLoadInfo = false;
        //m_IsExistsFinishAction = false;
        m_IsLoadByServer = false;
        m_ActionMap.Clear();
    }
    //public void ActionTest()
    //{
    //    //添加成就的测试数据
    //    for (Byte i = 1; i <= 10; ++i)
    //    {

    //        tagActionConfig pConfig = null;
    //        if (!FishConfig.Instance.m_ActionInfo.m_ActionMap.TryGetValue(i, out pConfig))
    //            continue;
    //        RoleActionBase pBase = CreateAction((EventTargetType)pConfig.ActionEventInfo.EventID);

    //        tagRoleActionInfo pInfo = new tagRoleActionInfo();
    //        pInfo.ActionID = i;
    //        pInfo.ActionValue = 0;

    //        if (pBase == null || !pBase.OnInit(pInfo, this))
    //            continue;
    //        m_ActionMap.Add(i, pBase);
    //    }

    //    m_IsLoadInfo = true;
    //    tagActionChangeEvent pEvent = new tagActionChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);
    //}
    public bool GetActionReward(Byte ActionID, UInt32 OnceID)
    {
        return SendGetActionReward(ActionID, OnceID);
    }
    public bool GetActionIsFinish(Byte ActionID)
    {
        return int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetActionStates(), ActionID);
    }
    public bool IsAction(Byte ActionID)
    {
        return m_ActionMap.ContainsKey(ActionID);
    }
    public RoleActionBase GetAction(Byte ActionID)
    {
        RoleActionBase pBase;
        if (!m_ActionMap.TryGetValue(ActionID, out pBase))
            return null;
        return pBase;
    }
    public Dictionary<Byte, RoleActionBase> GetActionArray()
    {
        if (m_IsLoadInfo)
        {
            return m_ActionMap;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                m_ActionMap.Clear();
                SendGetRoleActionInfo();
            }
            // ActionTest();
            return null;
        }
    }
    public void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (m_ActionMap.Count == 0)
            return;
        foreach (RoleActionBase var in m_ActionMap.Values)
        {
            var.OnHandleEvent(EventID, BindParam, Param);
        }
    }
    //命令处理 基本的命令处理
    private void SendGetRoleActionInfo()//获取全部的任务数据
    {
        CL_Cmd_GetRoleActionInfo ncb = new CL_Cmd_GetRoleActionInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetRoleActionInfo);
        NetServices.Instance.Send<CL_Cmd_GetRoleActionInfo>(ncb);
    }
    private bool SendGetActionReward(Byte ActionID, UInt32 OnceID)
    {
        //判断任务是否完成了
        RoleActionBase pAction = GetAction(ActionID);
        if (pAction == null)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Action_GetReward_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!pAction.pConfig.EventMap.ContainsKey(OnceID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Action_GetReward_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (pAction.pConfig.EventMap[OnceID].FinishValue > pAction.pActionInfo.ActionValue)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Action_GetReward_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if ((pAction.pActionInfo.ActionStates & OnceID) != 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Action_GetReward_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!pAction.pConfig.IsInTime())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Action_GetReward_Failed_5);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_GetActionReward ncb = new CL_Cmd_GetActionReward();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetActionReward);
        ncb.ActionID = ActionID;
        ncb.OnceID = OnceID;
        NetServices.Instance.Send<CL_Cmd_GetActionReward>(ncb);
        return true;
    }
    private void SendGetActionInfoByServer(Byte ActionID)
    {
        CL_Cmd_GetOnceActionInfo ncb = new CL_Cmd_GetOnceActionInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetOnceActionInfo);
        ncb.ActionID = ActionID;
        NetServices.Instance.Send<CL_Cmd_GetOnceActionInfo>(ncb);
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetRoleActionInfo:
                return HandleGetRoleActionInfo(obj);
            //case NetCmdType.CMD_LC_GetRoleActionInfoFinish:
            //    return HandleGetRoleActionInfoFinish(obj);
            //case NetCmdType.CMD_LC_ActionAllEventFinish:
            //    return HandleActionAllEventFinish(obj);
            case NetCmdType.CMD_LC_GetActionReward:
                return HandleGetActionReward(obj);
            case NetCmdType.CMD_LC_GetOnceActionInfo:
                return HandleChangeActionInfo(obj);
            case NetCmdType.CMD_LC_JoinAction:
                return HandleJoinActionInfo(obj);
            case NetCmdType.CMD_LC_DelAction:
                return HandleDelActionInfo(obj);
        }
        return true;
    }
    bool HandleGetRoleActionInfo(NetCmdBase obj)
    {
        LC_Cmd_GetRoleActionInfo ncb = (LC_Cmd_GetRoleActionInfo)obj;
        if ((ncb.States & FishDataInfo.MsgBegin)!=0)
        {
            m_ActionMap.Clear();
        }
        for (UInt16 i = 0; i < ncb.Sum; ++i)
        {
            tagActionConfig pConfig = null;
            if (!FishConfig.Instance.m_ActionInfo.m_ActionMap.TryGetValue(ncb.Array[i].ActionID, out pConfig))
                continue;
            RoleActionBase pBase = CreateAction((EventTargetType)pConfig.EventID);
            if (pBase == null || !pBase.OnInit(ncb.Array[i], this))
                continue;
            m_ActionMap.Add(ncb.Array[i].ActionID, pBase);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadInfo = true;
            tagActionChangeEvent pEvent = new tagActionChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //bool HandleGetRoleActionInfoFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_GetRoleActionInfoFinish ncb = (LC_Cmd_GetRoleActionInfoFinish)obj;
        
    //    //CheckIsExistsFinishItem();
       
    //    return true;
    //}
    //bool HandleActionAllEventFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_ActionAllEventFinish ncb = (LC_Cmd_ActionAllEventFinish)obj;
    //    RoleActionBase pAction = GetAction(ncb.ActionID);
    //    if (pAction == null)
    //    {
    //        SendGetActionInfoByServer(ncb.ActionID);
    //        return true;
    //    }
    //    pAction.SetActionIsFinish();
    //    return true;
    //}
    bool HandleGetActionReward(NetCmdBase obj)
    {
        LC_Cmd_GetActionReward ncb = (LC_Cmd_GetActionReward)obj;

        RoleActionBase pBase = GetAction(ncb.ActionID);
        tagActionOnceRewardEvent pReward = null;
        if (pBase != null)
        {
            foreach (tagActionEventConfig var in pBase.pConfig.EventMap.Values)
            {
                if ((var.ID & pBase.pActionInfo.ActionStates) == 0)
                {
                    //为完成的条件
                    pReward = new tagActionOnceRewardEvent(ncb.ActionID,var.ID, var.RewardID);
                    break;
                }
            }
        }
       
        m_ActionMap.Remove(ncb.ActionID);//将任务移除掉
       // CheckIsExistsFinishItem();
        tagOnceActionChangeEvent pEvent = new tagOnceActionChangeEvent(ncb.ActionID);
        MsgEventHandle.HandleMsg(pEvent);

        if (pReward != null)
        {
            MsgEventHandle.HandleMsg(pReward);

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Action_GetReward_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    bool HandleChangeActionInfo(NetCmdBase obj)
    {
        LC_Cmd_GetOnceActionInfo ncb = (LC_Cmd_GetOnceActionInfo)obj;
        RoleActionBase pAction = GetAction(ncb.ActionInfo.ActionID);
        if (pAction == null)
        {
            //直接添加
            tagActionConfig pConfig = null;
            if (!FishConfig.Instance.m_ActionInfo.m_ActionMap.TryGetValue(ncb.ActionInfo.ActionID, out pConfig))
                return false;
            pAction = CreateAction((EventTargetType)pConfig.EventID);
            if (pAction == null || !pAction.OnInit(ncb.ActionInfo, this))
            {
                return false;
            }
            m_ActionMap.Add(ncb.ActionInfo.ActionID, pAction);
          //  CheckIsExistsFinishItem();
            tagOnceActionChangeEvent pEvent = new tagOnceActionChangeEvent(ncb.ActionInfo.ActionID);
            MsgEventHandle.HandleMsg(pEvent);
            return true;
        }
        else
        {
            UInt32 ID = (ncb.ActionInfo.ActionStates ^ pAction.pActionInfo.ActionStates);

            pAction.pActionInfo = ncb.ActionInfo;

           // CheckIsExistsFinishItem();
            tagOnceActionChangeEvent pEvent = new tagOnceActionChangeEvent(ncb.ActionInfo.ActionID);
            MsgEventHandle.HandleMsg(pEvent);

            RoleActionBase pBase = GetAction(ncb.ActionInfo.ActionID);
            if (pBase != null && pBase.pConfig.EventMap.ContainsKey(ID))
            {
                tagActionEventConfig pEventConfig = pBase.pConfig.EventMap[ID];
                tagActionOnceRewardEvent pReward = new tagActionOnceRewardEvent(ncb.ActionInfo.ActionID, ID,pEventConfig.RewardID);
                MsgEventHandle.HandleMsg(pReward);

                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Action_GetReward_Sucess);
                MsgEventHandle.HandleMsg(pUOM);
            }
            //pAction.ChangeActionErrorInfo(ncb.ActionInfo);
        }
        return true;
    }
    bool HandleJoinActionInfo(NetCmdBase obj)
    {
        LC_Cmd_JoinAction ncb = (LC_Cmd_JoinAction)obj;
        tagActionConfig pConfig = null;
        if (!FishConfig.Instance.m_ActionInfo.m_ActionMap.TryGetValue(ncb.ActionID, out pConfig))
            return false;
        RoleActionBase pBase = CreateAction((EventTargetType)pConfig.EventID);
        tagRoleActionInfo pInfo = new tagRoleActionInfo();
        pInfo.ActionID = ncb.ActionID;
        pInfo.ActionValue = 0;
        if (pBase == null || !pBase.OnInit(pInfo, this))
            return false;
        m_ActionMap.Remove(ncb.ActionID);
        m_ActionMap.Add(ncb.ActionID, pBase);
        PlayerRole.Instance.UpdateRoleEventHandle();
       // CheckIsExistsFinishItem();
        tagOnceActionChangeEvent pEvent = new tagOnceActionChangeEvent(ncb.ActionID);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleDelActionInfo(NetCmdBase obj)
    {
        LC_Cmd_DelAction ncb = (LC_Cmd_DelAction)obj;
        m_ActionMap.Remove(ncb.ActionID);
      //  CheckIsExistsFinishItem();
        tagOnceActionChangeEvent pEvent = new tagOnceActionChangeEvent(ncb.ActionID);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    //bool HandleClearActionInfo(NetCmdBase obj)
    //{
    //    m_ActionMap.Clear();
    //   // CheckIsExistsFinishItem();
    //    tagActionChangeEvent pEvent = new tagActionChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);
    //    return true;
    //}
    private RoleActionBase CreateAction(EventTargetType EventType)
    {
        switch (EventType)
        {
            case EventTargetType.ET_GetGlobel:
                return new GetGlobelRoleAction();
            case EventTargetType.ET_GetMadel:
                return new GetMadelRoleAction();
            case EventTargetType.ET_GetCurren:
                return new GetCurrenRoleAction();
            case EventTargetType.ET_UpperLevel:
                return new UpperLevelRoleAction();
            case EventTargetType.ET_CatchFish:
                return new CatchFishRoleAction();
            case EventTargetType.ET_SendGiff:
                return new SendGiffRoleAction();
            case EventTargetType.ET_UseSkill:
                return new UseSkillRoleAction();
            case EventTargetType.ET_LauncherGlobel:
                return new LauncherGlobelRoleAction();
            case EventTargetType.ET_MaxGlobelSum:
                return new MaxGlobelSumRoleAction();
            case EventTargetType.ET_ToLevel:
                return new ToLevelRoleAction();
            case EventTargetType.ET_MaxCurren:
                return new MaxCurrenRoleAction();
            case EventTargetType.ET_Recharge:
                return new RechargeRoleAction();
        }
        return null;
    }
}
class RoleActionBase
{
    public tagRoleActionInfo pActionInfo = new tagRoleActionInfo();//当前任务的信息
    public RoleActionManager m_pManger = null;
    //public bool m_ActionEventIsFinish = false;//条件是否完成
    public tagActionConfig pConfig = null;

    public RoleActionBase()
    {

    }
    public virtual bool OnInit(tagRoleActionInfo pInfo, RoleActionManager pManager)
    {
        //初始化任务 的数据
        m_pManger = pManager;
        pActionInfo = pInfo;//客户端在加载的时候 无须判断是否需要进行每日清空
        pConfig = FishConfig.Instance.m_ActionInfo.m_ActionMap[pActionInfo.ActionID];
        //m_ActionEventIsFinish = (pConfig..FinishValue <= pInfo.ActionValue);
        OnJoinAction();
        return true;
    }
    public virtual void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        //if (m_ActionEventIsFinish)
        //    return;
        if (Param == 0)
            return;
        pActionInfo.ActionValue += Param;
        //if (pActionInfo.ActionValue > FishConfig.Instance.m_ActionInfo.m_ActionMap[pActionInfo.ActionID].ActionEventInfo.FinishValue)
        //    pActionInfo.ActionValue = FishConfig.Instance.m_ActionInfo.m_ActionMap[pActionInfo.ActionID].ActionEventInfo.FinishValue;
        //m_ActionEventIsFinish = (pConfig.ActionEventInfo.FinishValue <= pActionInfo.ActionValue);
        tagOnceActionChangeEvent pEvent = new tagOnceActionChangeEvent(pActionInfo.ActionID);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public virtual void ChangeActionErrorInfo(tagRoleActionInfo pInfo)
    {
        //客户端向服务器端发送错误的请求的时候 我们重置任务的数据
        pActionInfo = pInfo;
        //m_ActionEventIsFinish = (pConfig.ActionEventInfo.FinishValue <= pActionInfo.ActionValue);
        tagOnceActionChangeEvent pEvent = new tagOnceActionChangeEvent(pActionInfo.ActionID);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public virtual void OnJoinAction()
    {
        return;
    }
    //public virtual void SetActionIsFinish()
    //{
    //    pActionInfo.ActionValue = pConfig.ActionEventInfo.FinishValue;
    //    m_ActionEventIsFinish = true;
    //    tagOnceActionChangeEvent pEvent = new tagOnceActionChangeEvent(pActionInfo.ActionID);
    //    MsgEventHandle.HandleMsg(pEvent);
    //}
}
class GetGlobelRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_GetGlobel)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class GetMadelRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_GetMadel)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class GetCurrenRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_GetCurren)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class UpperLevelRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_UpperLevel)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class CatchFishRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_CatchFish || (pConfig.BindParam != BindParam && pConfig.BindParam != 0xff))
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class SendGiffRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_SendGiff)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class UseSkillRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_UseSkill || (pConfig.BindParam != BindParam && pConfig.BindParam != 0xff))
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class LauncherGlobelRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_LauncherGlobel || pConfig.BindParam > BindParam)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class MaxGlobelSumRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_MaxGlobelSum || Param <= pActionInfo.ActionValue)
            return;
        if ( Param != 0)
            pActionInfo.ActionValue = 0;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
    public override void OnJoinAction()
    {
	    //设置属性
	    if (pConfig.EventID != (Byte)EventTargetType.ET_MaxGlobelSum)
		    return;
	    OnHandleEvent(EventTargetType.ET_MaxGlobelSum, 0, PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel());
    }
}
class ToLevelRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_ToLevel || Param <= pActionInfo.ActionValue)
            return;
        if (Param != 0)
            pActionInfo.ActionValue = 0;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
    public override void OnJoinAction()
    {
	    //设置属性
	    if (pConfig.EventID != (Byte)EventTargetType.ET_ToLevel)
		    return;
	    OnHandleEvent(EventTargetType.ET_ToLevel, 0, PlayerRole.Instance.RoleInfo.RoleMe.GetLevel());
    }
}
class MaxCurrenRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_MaxCurren || Param <= pActionInfo.ActionValue)
            return;
        if (Param != 0)
            pActionInfo.ActionValue = 0;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
    public override void OnJoinAction()
    {
	    //设置属性
	    if (pConfig.EventID != (Byte)EventTargetType.ET_MaxCurren)
		    return;
	    OnHandleEvent(EventTargetType.ET_MaxCurren, 0, PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency());
    }
}
class RechargeRoleAction : RoleActionBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_Recharge)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}