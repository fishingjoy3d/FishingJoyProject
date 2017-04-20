using UnityEngine;
using System.Collections.Generic;
public class LogicManager:Singleton<LogicManager>
{
    
    enum Action
    {
        ACTION_NONE,
        ACTION_SHUTDOWN,
        ACTION_FORWARD,
        ACTION_BACK,
        ACTION_TOSCENE,
    }
    LogicRuntimeData m_LogicData;
    byte m_eAction = (byte)Action.ACTION_NONE;
    object m_Params = null;
    public bool IsPlaying
    {
        get
        {
            if (m_LogicData != null)
                return m_LogicData.Type == LogicType.LOGIC_SCENE;
            else
                return false;
        }
    }
    public void GlobalInit()
    {
//#if UNITY_EDITOR
        LogicRuntimeData ld = LogicFlowManager.GetLogic(ResManager.ENABLE_RESOURCES ? LogicType.LOGIC_INIT : LogicType.LOGIC_UPDATE);
//#else
//        LogicRuntimeData ld = LogicFlowManager.GetLogic(LogicType.LOGIC_UPDATE);
//#endif
        ActiveLogic(ld);
    }
    void CloseLogic()
    {
        if(m_LogicData != null)
        {
            m_LogicData.LogicUIObj.Shutdown();
            m_LogicData.LogicObj.Shutdown();
            //释放资源
            ResManager.Instance.UnloadManagerObjects();
            m_LogicData = null;
        }
    }
    public LogicType ActiveLogicType
    {
        get
        {
            if (m_LogicData == null)
                return LogicType.LOGIC_MAX;
            return m_LogicData.Type;
        }
    }
    System.Collections.IEnumerator AsyncLoadRes(LogicRuntimeData ld)
    {
        if (ld.Type != LogicType.LOGIC_INIT)
        {
            if(GlobalLoading.Instance.IsShow == false)
                GlobalEffectMgr.Instance.ShowLoadingMessage();
        }

        if (ResType.GlobalRes == ld.LogicRes)
        {
            bool bret = ResManager.Instance.BeginLoadRes(ResType.GlobalRes, true);
            yield return new WaitForEndOfFrame();
            bret &= ResManager.Instance.BeginLoadRes(ResType.FishRes, true);
            yield return new WaitForEndOfFrame();
            bret &= ResManager.Instance.BeginLoadRes(ResType.PathRes, true);
            yield return new WaitForEndOfFrame();
            bret &= ResManager.Instance.BeginLoadRes(ResType.ExtraRes, true);
            yield return new WaitForEndOfFrame();
            bret &= ResManager.Instance.BeginLoadRes(ResType.LogonRes, true);
            yield return new WaitForEndOfFrame();
            bret &= ResManager.Instance.BeginLoadRes(ResType.SceneRes, true);
            yield return new WaitForEndOfFrame();
            bret &= ResManager.Instance.BeginLoadRes(ResType.GameRes, true);
            if(!bret)
            {
                NativeInterface.ShowMsgAndExit("res_load_err", 200);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                yield break;
            }
        }
        else if (ld.LogicRes != ResType.MAX)
        {
            yield return new WaitForEndOfFrame();
            bool bret = ResManager.Instance.BeginLoadRes(ld.LogicRes, true);
            if(!bret)
            {
                NativeInterface.ShowMsgAndExit("res_load_err", 201);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                yield break;
            }
        }
        yield return new WaitForEndOfFrame();
        bool bRet = ld.LogicObj.Init(ld.LogicUIObj, m_Params);
        yield return new WaitForEndOfFrame();
        bRet &= ld.LogicUIObj.Init(ld.LogicObj, m_Params);
        if (!bRet)
        {
            //逻辑和UI初始化失败
            NativeInterface.ShowMsgAndExit("res_load_err", 202);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            InitCompletion();
            yield break;
        }
        m_LogicData = ld;
        m_Params = null;
        yield break;
    }
    public void InitCompletion()
    {
        ResManager.Instance.EndLoadRes();
    }
    void ActiveLogic(LogicRuntimeData ld)
    {
        GlobalUpdate.OnLogicChange();
        CloseLogic();
        if (ld.Type != LogicType.LOGIC_UPDATE)
        {
            SceneMain.Instance.StartInnerCoroutine(AsyncLoadRes(ld));
            return;
        }
        m_LogicData = ld;
        if (ResType.GlobalRes == ld.LogicRes)
        {
            if (!ResManager.Instance.BeginLoadRes(ResType.GlobalRes, true) ||
                !ResManager.Instance.BeginLoadRes(ResType.FishRes, false) ||
                !ResManager.Instance.BeginLoadRes(ResType.PathRes, true) ||
                !ResManager.Instance.BeginLoadRes(ResType.ExtraRes, true))
            {
                NativeInterface.ShowMsgAndExit("res_load_err", 200);
                return;
            }
        }
        else if(ld.LogicRes != ResType.MAX)
        {
            if (!ResManager.Instance.BeginLoadRes(m_LogicData.LogicRes, true))
            {
                NativeInterface.ShowMsgAndExit("res_load_err", 203);
                return;
            }
        }
        bool bRet = m_LogicData.LogicObj.Init(m_LogicData.LogicUIObj, m_Params);
        bRet &= m_LogicData.LogicUIObj.Init(m_LogicData.LogicObj, m_Params);
        m_Params = null;
        if (!bRet)
        {
            //逻辑和UI初始化失败
            NativeInterface.ShowMsgAndExit("res_load_err", 202);
            return;
        }
        
        ResManager.Instance.EndLoadRes();
    }
    public void LogOff()
    {
        if (m_Params != null)
            return;
        m_Params = true;
        try
        {
            WndManager.Instance.Clear();
        }
        catch{
        }
        try
        {
            NetServices.Instance.ClearCmdAndHandler();
            NetServices.Instance.Disconnect();
        }
        catch{
        }

        LogicRuntimeData ld = LogicFlowManager.GetLogic(LogicType.LOGIC_LOGON);
        ActiveLogic(ld);
        try
        {
            tagReturnLogonEvent pEvent = new tagReturnLogonEvent();
            MsgEventHandle.HandleMsg(pEvent);
            SDKMgr.Instance.Logout();
        }
        catch{
        }
    }
    public void Shutdown()
    {
        LogicAction = Action.ACTION_SHUTDOWN;
    }
    public void Forward(object obj)
    {
        m_Params = obj;
        LogicAction = Action.ACTION_FORWARD;
    }
    public void SkipToHall()
    {
        if (ActiveLogicType == LogicType.LOGIC_SCENE)
        {
            NetServices.Instance.CmdHandler = null;
            Back(null);
        }
        else
        {
            HallRunTimeInfo.Login_.ResetHall();
        }
    }
    public void SkipToScene(object obj)
    {
        if(ActiveLogicType != LogicType.LOGIC_SCENE)
        {
            LogMgr.Log("SkipToScene，当前不在场景中。");
        }
        else
        {
            SceneRuntime.SceneLogic.ResetScene((JoinRoomData)obj);
        }
    }
    public void Back(object obj)
    {
        if(IsPlaying)
            NetServices.Instance.ClearCmdAndHandler();
        m_Params = obj;
        LogicAction = Action.ACTION_BACK;
    }
    void CheckAction()
    {
        if (LogicAction == Action.ACTION_NONE)
            return;

        if (LogicAction == Action.ACTION_FORWARD)
        {
            LogicRuntimeData ld = LogicFlowManager.GetForwardLogic(m_LogicData.Type);
            if (ld == null)
            {
                LogMgr.Log("Logic forward failed.");
            }
            ActiveLogic(ld);
        }
        else if (LogicAction == Action.ACTION_BACK)
        {
            LogicRuntimeData ld = LogicFlowManager.GetBackLogic(m_LogicData.Type);
            if (ld == null)
            {
                LogMgr.Log("Logic back failed.");
            }
            ActiveLogic(ld);
        }
        else if (LogicAction == Action.ACTION_SHUTDOWN)
        {
            SceneMain.Instance.Shutdown();
        }
        else if(LogicAction == Action.ACTION_TOSCENE)
        {
            LogicRuntimeData ld = LogicFlowManager.GetLogic(LogicType.LOGIC_SCENE);
            if (ld == null)
            {
                LogMgr.Log("Skip to scene failed.");
            }
            ActiveLogic(ld);
        }
        LogicAction = Action.ACTION_NONE;
    }
    public void Update(float delta)
    {
        GlobalUpdate.PreUpdate(delta);
        if(m_LogicData != null)
        { 
            m_LogicData.LogicObj.Update(delta);
            m_LogicData.LogicUIObj.Update(delta);
        }
        GlobalUpdate.LateUpdate(delta);
        if(m_LogicData != null)
            CheckAction();
    }
    public void OnGUI(float delta)
    {
        if(m_LogicData != null)
            m_LogicData.LogicUIObj.OnGUI(delta);
    }
    Action LogicAction
    {
        get
        {
            return (Action)m_eAction;
        }
        set
        {
            m_eAction = (byte)value;
        }
    }
}
