using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//玩家任务类
class RoleAchievementManager
{
    //任务管理类 无须处理具体的逻辑 只需要进行简单的处理就OK了
    private bool m_IsLoadInfo = false;
    private Dictionary<Byte, RoleAchievementBase> m_AchievementMap = new Dictionary<Byte, RoleAchievementBase>();//当前正在进行的任务列表
    private bool m_IsLoadByServer = false;
   // private bool m_IsExistsFinishAchievement = false;

    public void ResetInfo()
    {
        OnClear();
        tagAchievementChangeEvent pEvent = new tagAchievementChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }
    //public bool IsExistsFinishItem()
    //{
    //    //判断当前成就了是否有已经完成 待领取的成就
    //    return m_IsExistsFinishAchievement;
    //}
    //public void SetExistsFinishItem(bool States)
    //{
    //    if (m_IsExistsFinishAchievement != States)
    //    {
    //        m_IsExistsFinishAchievement = States;
    //        //发送事件
    //        tagAchievementFinishStatesChagneEvent pEvent = new tagAchievementFinishStatesChagneEvent();
    //        MsgEventHandle.HandleMsg(pEvent);
    //    }
    //    else
    //        m_IsExistsFinishAchievement = States;
    //}
    //private void CheckIsExistsFinishItem()
    //{
    //    if (m_AchievementMap.Count == 0)
    //        SetExistsFinishItem(false);
    //    else
    //    {
    //        foreach (RoleAchievementBase var in m_AchievementMap.Values)
    //        {
    //            if (var.m_AchievementEventIsFinish)
    //            {
    //                SetExistsFinishItem(true);
    //                return;
    //            }
    //        }
    //        SetExistsFinishItem(false);
    //    }
    //}

    public RoleAchievementManager()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_IsLoadInfo = false;
        m_IsLoadByServer = false;
       // m_IsExistsFinishAchievement = false;
        m_AchievementMap.Clear();
    }
    //public void AchievementTest()
    //{
    //    //添加成就的测试数据
    //    for (Byte i = 1; i <= 10; ++i)
    //    {
           
    //        tagAchievementConfig pConfig = null;
    //        if (!FishConfig.Instance.m_AchievementInfo.m_AchievementMap.TryGetValue(i, out pConfig))
    //            continue;
    //        RoleAchievementBase pBase = CreateAchievement((EventTargetType)pConfig.AchievementEventInfo.EventID);

    //        tagRoleAchievementInfo pInfo = new tagRoleAchievementInfo();
    //        pInfo.AchievementID = i;
    //        pInfo.AchievementValue = 0;

    //        if (pBase == null || !pBase.OnInit(pInfo, this))
    //            continue;
    //        m_AchievementMap.Add(i, pBase);
    //    }

    //    m_IsLoadInfo = true;
    //    tagAchievementChangeEvent pEvent = new tagAchievementChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);
    //}
    public bool GetAchievementReward(Byte AchievementID)
    {
        return SendGetAchievementReward(AchievementID);
    }
    public RoleAchievementBase GetAchievement(Byte AchievementID)
    {
        RoleAchievementBase pBase = null;
        if (!m_AchievementMap.TryGetValue(AchievementID, out pBase))
            return null;
        return pBase;
    }
    public bool GetAchievementIsFinish(Byte AchievementID)
    {
        return int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementStates(), AchievementID);
    }
    public bool IsAction(Byte AchievementID)
    {
        return m_AchievementMap.ContainsKey(AchievementID);
    }
    public Dictionary<Byte, RoleAchievementBase> GetAchievementArray()
    {
        if (m_IsLoadInfo)
        {
            return m_AchievementMap;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                m_AchievementMap.Clear();
                SendGetRoleAchievementInfo();
            }
            //AchievementTest();
            return null;
        }
    }
    public void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (m_AchievementMap.Count == 0)
            return;
        foreach (RoleAchievementBase var in m_AchievementMap.Values)
        {
            var.OnHandleEvent(EventID, BindParam, Param);
        }
    }

    //命令处理 基本的命令处理
    private void SendGetRoleAchievementInfo()//获取全部的任务数据
    {
        CL_Cmd_GetRoleAchievementInfo ncb = new CL_Cmd_GetRoleAchievementInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetRoleAchievementInfo);
        NetServices.Instance.Send<CL_Cmd_GetRoleAchievementInfo>(ncb);
    }
    private bool SendGetAchievementReward(Byte AchievementID)
    {
        //判断任务是否完成了
        RoleAchievementBase pAchievement = GetAchievement(AchievementID);
        if (pAchievement == null)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Achievement_GetReward_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!pAchievement.m_AchievementEventIsFinish)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Achievement_GetReward_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_GetAchievementReward ncb = new CL_Cmd_GetAchievementReward();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetAchievementReward);
        ncb.AchievementID = AchievementID;
        NetServices.Instance.Send<CL_Cmd_GetAchievementReward>(ncb);
        return true;
    }
    private void SendGetAchievementInfoByServer(Byte AchievementID)
    {
        CL_Cmd_GetOnceAchievementInfo ncb = new CL_Cmd_GetOnceAchievementInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetOnceAchievementInfo);
        ncb.AchievementID = AchievementID;
        NetServices.Instance.Send<CL_Cmd_GetOnceAchievementInfo>(ncb);
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetRoleAchievementInfo:
                return HandleGetRoleAchievementInfo(obj);
            //case NetCmdType.CMD_LC_GetRoleAchievementInfoFinish:
            //    return HandleGetRoleAchievementInfoFinish(obj);
            case NetCmdType.CMD_LC_AchievementAllEventFinish:
                return HandleAchievementAllEventFinish(obj);
            case NetCmdType.CMD_LC_GetAchievementReward:
                return HandleGetAchievementReward(obj);
            case NetCmdType.CMD_LC_GetOnceAchievementInfo:
                return HandleChangeAchievementInfo(obj);
            case NetCmdType.CMD_LC_JoinAchievement:
                return HandleJoinAchievementInfo(obj);
            case NetCmdType.CMD_LC_DelAchievement:
                return HandleDelAchievementInfo(obj);
        }
        return true;
    }
    bool HandleGetRoleAchievementInfo(NetCmdBase obj)
    {
        LC_Cmd_GetRoleAchievementInfo ncb = (LC_Cmd_GetRoleAchievementInfo)obj;
        if ((ncb.States & FishDataInfo.MsgBegin)!= 0)
        {
            m_AchievementMap.Clear();
        }
        for (UInt16 i = 0; i < ncb.Sum; ++i)
        {
            tagAchievementConfig pConfig = null;
            if (!FishConfig.Instance.m_AchievementInfo.m_AchievementMap.TryGetValue(ncb.Array[i].AchievementID, out pConfig))
                continue;
            RoleAchievementBase pBase = CreateAchievement((EventTargetType)pConfig.AchievementEventInfo.EventID);
            if (pBase == null || !pBase.OnInit(ncb.Array[i], this))
                continue;
            m_AchievementMap.Add(ncb.Array[i].AchievementID, pBase);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadInfo = true;
            tagAchievementChangeEvent pEvent = new tagAchievementChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //bool HandleGetRoleAchievementInfoFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_GetRoleAchievementInfoFinish ncb = (LC_Cmd_GetRoleAchievementInfoFinish)obj;
       
    //    return true;
    //}
    bool HandleAchievementAllEventFinish(NetCmdBase obj)
    {
        LC_Cmd_AchievementAllEventFinish ncb = (LC_Cmd_AchievementAllEventFinish)obj;
        RoleAchievementBase pAchievement = GetAchievement(ncb.AchievementID);
        if (pAchievement == null)
        {
            SendGetAchievementInfoByServer(ncb.AchievementID);
            return true;
        }
        pAchievement.SetAchievementIsFinish();
        //m_IsExistsFinishAchievement = true;
        tagOnceAchievementChangeEvent pEvent = new tagOnceAchievementChangeEvent(ncb.AchievementID);
        MsgEventHandle.HandleMsg(pEvent);

        tagAchievementEventFinishEvent pFinish = new tagAchievementEventFinishEvent(ncb.AchievementID);
        MsgEventHandle.HandleMsg(pFinish);
        return true;
    }
    bool HandleGetAchievementReward(NetCmdBase obj)
    {
        LC_Cmd_GetAchievementReward ncb = (LC_Cmd_GetAchievementReward)obj;
        m_AchievementMap.Remove(ncb.AchievementID);//将任务移除掉
      //  CheckIsExistsFinishItem();//判断成就是否有完成的

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Achievement_GetReward_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        tagOnceAchievementChangeEvent pEvent = new tagOnceAchievementChangeEvent(ncb.AchievementID);
        MsgEventHandle.HandleMsg(pEvent);
        //UIWidgetManager.Instance.UpdateUserInfAchievement();
        return true;
    }
    bool HandleChangeAchievementInfo(NetCmdBase obj)
    {
        LC_Cmd_GetOnceAchievementInfo ncb = (LC_Cmd_GetOnceAchievementInfo)obj;
        RoleAchievementBase pAchievement = GetAchievement(ncb.AchievementInfo.AchievementID);
        if (pAchievement == null)
        {
            //直接添加
            tagAchievementConfig pConfig = null;
            if (!FishConfig.Instance.m_AchievementInfo.m_AchievementMap.TryGetValue(ncb.AchievementInfo.AchievementID, out pConfig))
                return false;
            pAchievement = CreateAchievement((EventTargetType)pConfig.AchievementEventInfo.EventID);
            if (pAchievement == null || !pAchievement.OnInit(ncb.AchievementInfo, this))
            {
                return false;
            }
            m_AchievementMap.Add(ncb.AchievementInfo.AchievementID, pAchievement);
           // CheckIsExistsFinishItem();//判断成就是否有完成的
            tagOnceAchievementChangeEvent pEvent = new tagOnceAchievementChangeEvent(ncb.AchievementInfo.AchievementID);
            MsgEventHandle.HandleMsg(pEvent);

            return true;
        }
        else
        {
          //  CheckIsExistsFinishItem();//判断成就是否有完成的
            pAchievement.ChangeAchievementErrorInfo(ncb.AchievementInfo);
        }
        return true;
    }
    bool HandleJoinAchievementInfo(NetCmdBase obj)
    {
        LC_Cmd_JoinAchievement ncb = (LC_Cmd_JoinAchievement)obj;
        tagAchievementConfig pConfig = null;
        if (!FishConfig.Instance.m_AchievementInfo.m_AchievementMap.TryGetValue(ncb.AchievementID, out pConfig))
            return false;
        RoleAchievementBase pBase = CreateAchievement((EventTargetType)pConfig.AchievementEventInfo.EventID);
        tagRoleAchievementInfo pInfo = new tagRoleAchievementInfo();
        pInfo.AchievementID = ncb.AchievementID;
        pInfo.AchievementValue = 0;
        if (pBase == null || !pBase.OnInit(pInfo, this))
            return false;
        m_AchievementMap.Add(ncb.AchievementID, pBase);

        //因为接到新的成就了 我们刷新下状态
        PlayerRole.Instance.UpdateRoleEventHandle();
       // CheckIsExistsFinishItem();//判断成就是否有完成的

        tagOnceAchievementChangeEvent pEvent = new tagOnceAchievementChangeEvent(ncb.AchievementID);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleDelAchievementInfo(NetCmdBase obj)
    {
        LC_Cmd_DelAchievement ncb = (LC_Cmd_DelAchievement)obj;
        m_AchievementMap.Remove(ncb.AchievementID);
       // CheckIsExistsFinishItem();//判断成就是否有完成的
        tagOnceAchievementChangeEvent pEvent = new tagOnceAchievementChangeEvent(ncb.AchievementID);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleClearAchievementInfo(NetCmdBase obj)
    {
        m_AchievementMap.Clear();
       // CheckIsExistsFinishItem();//判断成就是否有完成的
        tagAchievementChangeEvent pEvent = new tagAchievementChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    private RoleAchievementBase CreateAchievement(EventTargetType EventType)
    {
        switch (EventType)
        {
            case EventTargetType.ET_GetGlobel:
                return new GetGlobelRoleAchievement();
            case EventTargetType.ET_GetMadel:
                return new GetMadelRoleAchievement();
            case EventTargetType.ET_GetCurren:
                return new GetCurrenRoleAchievement();
            case EventTargetType.ET_UpperLevel:
                return new UpperLevelRoleAchievement();
            case EventTargetType.ET_CatchFish:
                return new CatchFishRoleAchievement();
            case EventTargetType.ET_SendGiff:
                return new SendGiffRoleAchievement();
            case EventTargetType.ET_UseSkill:
                return new UseSkillRoleAchievement();
            case EventTargetType.ET_LauncherGlobel:
                return new LauncherGlobelRoleAchievement();
            case EventTargetType.ET_MaxGlobelSum:
                return new MaxGlobelSumRoleAchievement();
            case EventTargetType.ET_ToLevel:
                return new ToLevelRoleAchievement();
            case EventTargetType.ET_MaxCurren:
                return new MaxCurrenRoleAchievement();
            case EventTargetType.ET_Recharge:
                return new RechargeRoleAchievement();
        }
        return null;
    }
}
class RoleAchievementBase
{
    public tagRoleAchievementInfo pAchievementInfo = new tagRoleAchievementInfo();//当前任务的信息
    public RoleAchievementManager m_pManger = null;
    public bool m_AchievementEventIsFinish = false;//条件是否完成
    public tagAchievementConfig pConfig = null;

    public RoleAchievementBase()
    {

    }
    public virtual bool OnInit(tagRoleAchievementInfo pInfo, RoleAchievementManager pManager)
    {
        //初始化任务 的数据
        m_pManger = pManager;
        pAchievementInfo = pInfo;//客户端在加载的时候 无须判断是否需要进行每日清空
        pConfig = FishConfig.Instance.m_AchievementInfo.m_AchievementMap[pAchievementInfo.AchievementID];
        m_AchievementEventIsFinish = (pConfig.AchievementEventInfo.FinishValue <= pInfo.AchievementValue);
        OnJoinAchievement();
        return true;
    }
    public virtual void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (m_AchievementEventIsFinish)
            return;
        if (Param == 0)
            return;
        pAchievementInfo.AchievementValue += Param;
        if (pAchievementInfo.AchievementValue > FishConfig.Instance.m_AchievementInfo.m_AchievementMap[pAchievementInfo.AchievementID].AchievementEventInfo.FinishValue)
            pAchievementInfo.AchievementValue = FishConfig.Instance.m_AchievementInfo.m_AchievementMap[pAchievementInfo.AchievementID].AchievementEventInfo.FinishValue;
        m_AchievementEventIsFinish = (pConfig.AchievementEventInfo.FinishValue <= pAchievementInfo.AchievementValue);
        tagOnceAchievementChangeEvent pEvent = new tagOnceAchievementChangeEvent(pAchievementInfo.AchievementID);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public virtual void ChangeAchievementErrorInfo(tagRoleAchievementInfo pInfo)
    {
        //客户端向服务器端发送错误的请求的时候 我们重置任务的数据
        pAchievementInfo = pInfo;
        m_AchievementEventIsFinish = (pConfig.AchievementEventInfo.FinishValue <= pAchievementInfo.AchievementValue);
        tagOnceAchievementChangeEvent pEvent = new tagOnceAchievementChangeEvent(pAchievementInfo.AchievementID);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public virtual void SetAchievementIsFinish()
    {
        pAchievementInfo.AchievementValue = pConfig.AchievementEventInfo.FinishValue;
        m_AchievementEventIsFinish = true;
        tagOnceAchievementChangeEvent pEvent = new tagOnceAchievementChangeEvent(pAchievementInfo.AchievementID);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public virtual void OnJoinAchievement()
    {
        return;
    }
}
class GetGlobelRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_GetGlobel)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class GetMadelRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_GetMadel)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class GetCurrenRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_GetCurren)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class UpperLevelRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_UpperLevel)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class CatchFishRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_CatchFish || (pConfig.AchievementEventInfo.BindParam != BindParam && pConfig.AchievementEventInfo.BindParam!=0xff))
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class SendGiffRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_SendGiff)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class UseSkillRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_UseSkill || (pConfig.AchievementEventInfo.BindParam != BindParam && pConfig.AchievementEventInfo.BindParam != 0xff))
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class LauncherGlobelRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_LauncherGlobel || pConfig.AchievementEventInfo.BindParam > BindParam)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class MaxGlobelSumRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_MaxGlobelSum || Param <= pAchievementInfo.AchievementValue)
            return;
        if (!m_AchievementEventIsFinish && Param != 0)
            pAchievementInfo.AchievementValue = 0;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
    public override void OnJoinAchievement()
    {
        //设置属性
        if (pConfig.AchievementEventInfo.EventID != (Byte)EventTargetType.ET_MaxGlobelSum)
            return;
        OnHandleEvent(EventTargetType.ET_MaxGlobelSum, 0, PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel());
    }
}
class ToLevelRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_ToLevel || Param <= pAchievementInfo.AchievementValue)
            return;
        if (!m_AchievementEventIsFinish && Param != 0)
            pAchievementInfo.AchievementValue = 0;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
    public override void OnJoinAchievement()
    {
        //设置属性
        if (pConfig.AchievementEventInfo.EventID != (Byte)EventTargetType.ET_ToLevel)
            return;
        OnHandleEvent(EventTargetType.ET_ToLevel, 0, PlayerRole.Instance.RoleInfo.RoleMe.GetLevel());
    }
}
class MaxCurrenRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_MaxCurren || Param <= pAchievementInfo.AchievementValue)
            return;
        if (!m_AchievementEventIsFinish && Param != 0)
            pAchievementInfo.AchievementValue = 0;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
    public override void OnJoinAchievement()
    {
        //设置属性
        if (pConfig.AchievementEventInfo.EventID != (Byte)EventTargetType.ET_MaxCurren)
            return;
        OnHandleEvent(EventTargetType.ET_MaxCurren, 0, PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency());
    }
}
class RechargeRoleAchievement : RoleAchievementBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_Recharge)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
