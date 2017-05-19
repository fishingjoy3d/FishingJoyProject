using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//玩家任务类
class RoleTaskManager
{
    //任务管理类 无须处理具体的逻辑 只需要进行简单的处理就OK了
    private bool m_IsLoadInfo = false;
    private Dictionary<Byte, RoleTaskBase> m_TaskMap = new Dictionary<Byte, RoleTaskBase>();//当前正在进行的任务列表
    private bool m_IsLoadByServer = false;

    private bool m_IsExistsFinishTask = false;
    public bool IsExistsFinishItem()
    {
        //判断当前成就了是否有已经完成 待领取的成就
        return m_IsExistsFinishTask;
    }
    public void SetExistsFinishItem(bool States)
    {
        if (m_IsExistsFinishTask != States)
        {
            //发送事件
            tagTaskFinishStatesChange pEvent = new tagTaskFinishStatesChange();
            MsgEventHandle.HandleMsg(pEvent);
        }
        m_IsExistsFinishTask = States;
    }
    private void CheckIsExistsFinishItem()
    {
        if (m_TaskMap.Count == 0)
            SetExistsFinishItem(false);
        else
        {
            foreach (RoleTaskBase var in m_TaskMap.Values)
            {
                if (var.m_TaskEventIsFinish)
                {
                    SetExistsFinishItem(true);
                    return;
                }
            }
            SetExistsFinishItem(false);
        }
    }
    public RoleTaskManager()
    {
        OnClear();
    }
    public void ResetInfo()
    {
        OnClear();
        //清理排行榜的数据 在排行榜打开的情况下 
        tagTaskChangeEvent pEvent = new tagTaskChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }
    public void OnClear()
    {
        m_IsLoadInfo = false;
        m_IsLoadByServer = false;
        m_IsExistsFinishTask = false;
        m_TaskMap.Clear();
    }
    //public void TaskTest()
    //{
    //    for (Byte i = 1; i <= 10; ++i)
    //    {

    //        tagTaskConfig pConfig = null;
    //        if (!FishConfig.Instance.m_TaskInfo.m_TaskMap.TryGetValue(i, out pConfig))
    //            continue;
    //        RoleTaskBase pBase = CreateTask((EventTargetType)pConfig.TaskEventInfo.EventID);

    //        tagRoleTaskInfo pInfo = new tagRoleTaskInfo();
    //        pInfo.TaskID = i;
    //        pInfo.TaskValue = 0;

    //        if (pBase == null || !pBase.OnInit(pInfo, this))
    //            continue;
    //        m_TaskMap.Add(i, pBase);
    //    }

    //    m_IsLoadInfo = true;
    //    tagTaskChangeEvent pEvent = new tagTaskChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);
    //}
    public bool GetTaskReward(Byte TaskID)
    {
        return SendGetTaskReward(TaskID);
    }
    public RoleTaskBase GetTask(Byte TaskID)
    {
        RoleTaskBase pBase = null;
        if (!m_TaskMap.TryGetValue(TaskID, out pBase))
            return null;
        return pBase;
    }
    public Dictionary<Byte, RoleTaskBase> GetTaskArray()
    {
        if (m_IsLoadInfo)
        {
            return m_TaskMap;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                SendGetRoleTaskInfo();
            }
            //TaskTest();
            return null;
        }
    }
    public bool GetTaskIsFinish(Byte TaskID)
    {
        return int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetTaskStates(), TaskID);
    }
    public bool IsAction(Byte TaskID)
    {
        return m_TaskMap.ContainsKey(TaskID);
    }
    public void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (m_TaskMap.Count == 0)
            return;
        foreach(RoleTaskBase var in m_TaskMap.Values)
        {
            var.OnHandleEvent(EventID, BindParam, Param);
        }
    }

    //命令处理 基本的命令处理
    private void SendGetRoleTaskInfo()//获取全部的任务数据
    {
        CL_Cmd_GetRoleTaskInfo ncb = new CL_Cmd_GetRoleTaskInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetRoleTaskInfo);
        NetServices.Instance.Send<CL_Cmd_GetRoleTaskInfo>(ncb);
        m_TaskMap.Clear();
    }
    private bool SendGetTaskReward(Byte TaskID)
    {
        //判断任务是否完成了
        RoleTaskBase pTask = GetTask(TaskID);
        if (pTask == null)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Task_GetReward_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!pTask.m_TaskEventIsFinish)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Task_GetReward_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_GetTaskReward ncb = new CL_Cmd_GetTaskReward();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetTaskReward);
        ncb.TaskID = TaskID;
        NetServices.Instance.Send<CL_Cmd_GetTaskReward>(ncb);

        
        return true;
    }
    private void SendGetTaskInfoByServer(Byte TaskID)
    {
        CL_Cmd_GetOnceTaskInfo ncb = new CL_Cmd_GetOnceTaskInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetOnceTaskInfo);
        ncb.TaskID = TaskID;
        NetServices.Instance.Send<CL_Cmd_GetOnceTaskInfo>(ncb);
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetRoleTaskInfo:
                return HandleGetRoleTaskInfo(obj);
            //case NetCmdType.CMD_LC_GetRoleTaskInfoFinish:
            //    return HandleGetRoleTaskInfoFinish(obj);
            case NetCmdType.CMD_LC_TaskAllEventFinish:
                return HandleTaskAllEventFinish(obj);
            case NetCmdType.CMD_LC_GetTaskReward:
                return HandleGetTaskReward(obj);
            case NetCmdType.CMD_LC_GetOnceTaskInfo:
                return HandleChangeTaskInfo(obj);
            case NetCmdType.CMD_LC_JoinTask:
                return HandleJoinTaskInfo(obj);
            case NetCmdType.CMD_LC_DelTask:
                return HandleDelTaskInfo(obj);
            case NetCmdType.CMD_LC_ClearTask:
                return HandleClearTaskInfo(obj);

        }
        return true;
    }
    bool HandleGetRoleTaskInfo(NetCmdBase obj)
    {
        LC_Cmd_GetRoleTaskInfo ncb = (LC_Cmd_GetRoleTaskInfo)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            m_TaskMap.Clear();
        }
        for (UInt16 i = 0; i < ncb.Sum;++i )
        {
            tagTaskConfig pConfig = null;
            if (!FishConfig.Instance.m_TaskInfo.m_TaskMap.TryGetValue(ncb.Array[i].TaskID, out pConfig))
                continue;
            RoleTaskBase pBase = CreateTask((EventTargetType)pConfig.TaskEventInfo.EventID);
            if (pBase == null || !pBase.OnInit(ncb.Array[i], this))
                continue;
            m_TaskMap.Add(ncb.Array[i].TaskID, pBase);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadInfo = true;
            CheckIsExistsFinishItem();
            tagTaskChangeEvent pEvent = new tagTaskChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //bool HandleGetRoleTaskInfoFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_GetRoleTaskInfoFinish ncb = (LC_Cmd_GetRoleTaskInfoFinish)obj;
        
    //    return true;
    //}
    bool HandleTaskAllEventFinish(NetCmdBase obj)
    {
        LC_Cmd_TaskAllEventFinish ncb = (LC_Cmd_TaskAllEventFinish)obj;
        RoleTaskBase pTask = GetTask(ncb.TaskID);
        if (pTask == null)
        {
            SendGetTaskInfoByServer(ncb.TaskID);
            return true;
        }
        pTask.SetTaskIsFinish();
        m_IsExistsFinishTask = true;
        tagOnceTaskChangeEvent pEvent = new tagOnceTaskChangeEvent(ncb.TaskID);
        MsgEventHandle.HandleMsg(pEvent);

        tagOnceTaskEventFinishEvent pFinish = new tagOnceTaskEventFinishEvent(ncb.TaskID);
        MsgEventHandle.HandleMsg(pFinish);
        return true;
    }
    bool HandleGetTaskReward(NetCmdBase obj)
    {
        LC_Cmd_GetTaskReward ncb = (LC_Cmd_GetTaskReward)obj;
        m_TaskMap.Remove(ncb.TaskID);//将任务移除掉
        CheckIsExistsFinishItem();
        tagOnceTaskChangeEvent pEvent = new tagOnceTaskChangeEvent(ncb.TaskID);
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Task_GetReward_Sucess);
        MsgEventHandle.HandleMsg(pUOM);
        //UIWidgetManager.Instance.UpdateUserInfTask();
        return true;
    }
    bool HandleChangeTaskInfo(NetCmdBase obj)
    {
        LC_Cmd_GetOnceTaskInfo ncb = (LC_Cmd_GetOnceTaskInfo)obj;
        RoleTaskBase pTask = GetTask(ncb.TaskInfo.TaskID);
        if (pTask == null)
        {
            //直接添加
            tagTaskConfig pConfig = null;
            if (!FishConfig.Instance.m_TaskInfo.m_TaskMap.TryGetValue(ncb.TaskInfo.TaskID, out pConfig))
                return false;
            pTask = CreateTask((EventTargetType)pConfig.TaskEventInfo.EventID);
            if (pTask == null || !pTask.OnInit(ncb.TaskInfo, this))
            {
                return false;
            }
            m_TaskMap.Add(ncb.TaskInfo.TaskID, pTask);
            CheckIsExistsFinishItem();
            tagOnceTaskChangeEvent pEvent = new tagOnceTaskChangeEvent(ncb.TaskInfo.TaskID);
            MsgEventHandle.HandleMsg(pEvent);

            return true;
        }
        else
        {
            CheckIsExistsFinishItem();
            pTask.ChangeTaskErrorInfo(ncb.TaskInfo);
            
        }
        return true;
    }
    bool HandleJoinTaskInfo(NetCmdBase obj)
    {
        LC_Cmd_JoinTask ncb = (LC_Cmd_JoinTask)obj;
        tagTaskConfig pConfig = null;
        if (!FishConfig.Instance.m_TaskInfo.m_TaskMap.TryGetValue(ncb.TaskID, out pConfig))
            return false;
        RoleTaskBase pBase = CreateTask((EventTargetType)pConfig.TaskEventInfo.EventID);
        tagRoleTaskInfo pInfo = new tagRoleTaskInfo();
        pInfo.TaskID = ncb.TaskID;
        pInfo.TaskValue = 0;
        if (pBase == null || !pBase.OnInit(pInfo,this))
            return false;
        m_TaskMap.Add(ncb.TaskID, pBase);
        PlayerRole.Instance.UpdateRoleEventHandle();
        CheckIsExistsFinishItem();
        tagOnceTaskChangeEvent pEvent = new tagOnceTaskChangeEvent(ncb.TaskID);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleDelTaskInfo(NetCmdBase obj)
    {
        LC_Cmd_DelTask ncb = (LC_Cmd_DelTask)obj;
        m_TaskMap.Remove(ncb.TaskID);
        CheckIsExistsFinishItem();
        tagOnceTaskChangeEvent pEvent = new tagOnceTaskChangeEvent(ncb.TaskID);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleClearTaskInfo(NetCmdBase obj)
    {
        m_TaskMap.Clear();
        CheckIsExistsFinishItem();
        tagTaskChangeEvent pEvent = new tagTaskChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    private RoleTaskBase CreateTask(EventTargetType EventType)
    {
        switch (EventType)
        {
            case EventTargetType.ET_GetGlobel:
                return new GetGlobelRoleTask();
            case EventTargetType.ET_GetMadel:
                return new GetMadelRoleTask();
            case EventTargetType.ET_GetCurren:
                return new GetCurrenRoleTask();
            case EventTargetType.ET_UpperLevel:
                return new UpperLevelRoleTask();
            case EventTargetType.ET_CatchFish:
                return new CatchFishRoleTask();
            case EventTargetType.ET_SendGiff:
                return new SendGiffRoleTask();
            case EventTargetType.ET_UseSkill:
                return new UseSkillRoleTask();
            case EventTargetType.ET_LauncherGlobel:
                return new LauncherGlobelRoleTask();
            case EventTargetType.ET_MaxGlobelSum:
                return new MaxGlobelSumRoleTask();
            case EventTargetType.ET_ToLevel:
                return new ToLevelRoleTask();
            case EventTargetType.ET_MaxCurren:
                return new MaxCurrenRoleTask();
            case EventTargetType.ET_Recharge:
                return new RechargeRoleTask();
        }
        return null;
    }
}
class RoleTaskBase
{
    public tagRoleTaskInfo pTaskInfo = new tagRoleTaskInfo();//当前任务的信息
    public RoleTaskManager m_pManger = null;
    public bool m_TaskEventIsFinish = false;//条件是否完成
    public tagTaskConfig pConfig = null;
    
    public RoleTaskBase()
    {

    }
    public virtual bool OnInit(tagRoleTaskInfo pInfo, RoleTaskManager pManager)
    {
        //初始化任务 的数据
        m_pManger = pManager;
        pTaskInfo = pInfo;//客户端在加载的时候 无须判断是否需要进行每日清空
        pConfig = FishConfig.Instance.m_TaskInfo.m_TaskMap[pTaskInfo.TaskID];
        m_TaskEventIsFinish = (pConfig.TaskEventInfo.FinishValue <= pInfo.TaskValue);
        OnJoinTask();
        return true;
    }
    public virtual void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (m_TaskEventIsFinish)
            return;
        if (Param == 0)
            return;
        pTaskInfo.TaskValue += Param;
        if (pTaskInfo.TaskValue > FishConfig.Instance.m_TaskInfo.m_TaskMap[pTaskInfo.TaskID].TaskEventInfo.FinishValue)
            pTaskInfo.TaskValue = FishConfig.Instance.m_TaskInfo.m_TaskMap[pTaskInfo.TaskID].TaskEventInfo.FinishValue;
        m_TaskEventIsFinish = (pConfig.TaskEventInfo.FinishValue <= pTaskInfo.TaskValue);
        tagOnceTaskChangeEvent pEvent = new tagOnceTaskChangeEvent(pTaskInfo.TaskID);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public virtual void ChangeTaskErrorInfo(tagRoleTaskInfo pInfo)
    {
        //客户端向服务器端发送错误的请求的时候 我们重置任务的数据
        pTaskInfo = pInfo;
        m_TaskEventIsFinish = (pConfig.TaskEventInfo.FinishValue <= pTaskInfo.TaskValue);
        tagOnceTaskChangeEvent pEvent = new tagOnceTaskChangeEvent(pTaskInfo.TaskID);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public virtual void SetTaskIsFinish()
    {
        pTaskInfo.TaskValue = pConfig.TaskEventInfo.FinishValue;
        m_TaskEventIsFinish = true;
        tagOnceTaskChangeEvent pEvent = new tagOnceTaskChangeEvent(pTaskInfo.TaskID);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public virtual void OnJoinTask()
    {
        return;
    }
}
class GetGlobelRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_GetGlobel)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class GetMadelRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_GetMadel)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class GetCurrenRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_GetCurren)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class UpperLevelRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_UpperLevel)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class CatchFishRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_CatchFish || (pConfig.TaskEventInfo.BindParam != BindParam && pConfig.TaskEventInfo.BindParam != 0xff))
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class SendGiffRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_SendGiff)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class UseSkillRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_UseSkill || (pConfig.TaskEventInfo.BindParam != BindParam && pConfig.TaskEventInfo.BindParam != 0xff))
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class LauncherGlobelRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_LauncherGlobel || pConfig.TaskEventInfo.BindParam > BindParam)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}
class MaxGlobelSumRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_MaxGlobelSum || Param <= pTaskInfo.TaskValue)
            return;
        if (!m_TaskEventIsFinish && Param != 0)
            pTaskInfo.TaskValue = 0;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
    public override void OnJoinTask()
    {
        //设置属性
        if (pConfig.TaskEventInfo.EventID != (Byte)EventTargetType.ET_MaxGlobelSum)
            return;
        OnHandleEvent(EventTargetType.ET_MaxGlobelSum, 0, PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel());
    }
}
class ToLevelRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_ToLevel || Param <= pTaskInfo.TaskValue)
            return;
        if (!m_TaskEventIsFinish && Param != 0)
            pTaskInfo.TaskValue = 0;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
    public override void OnJoinTask()
    {
        //设置属性
        if (pConfig.TaskEventInfo.EventID != (Byte)EventTargetType.ET_ToLevel)
            return;
        OnHandleEvent(EventTargetType.ET_ToLevel, 0, PlayerRole.Instance.RoleInfo.RoleMe.GetLevel());
    }
}
class MaxCurrenRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_MaxCurren || Param <= pTaskInfo.TaskValue)
            return;
        if (!m_TaskEventIsFinish && Param != 0)
            pTaskInfo.TaskValue = 0;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
    public override void OnJoinTask()
    {
        //设置属性
        if (pConfig.TaskEventInfo.EventID != (Byte)EventTargetType.ET_MaxCurren)
            return;
        OnHandleEvent(EventTargetType.ET_MaxCurren, 0, PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency());
    }
}
class RechargeRoleTask : RoleTaskBase
{
    public override void OnHandleEvent(EventTargetType EventID, UInt32 BindParam, UInt32 Param)
    {
        if (Param == 0 || EventID != EventTargetType.ET_Recharge)
            return;
        base.OnHandleEvent(EventID, BindParam, Param);
    }
}