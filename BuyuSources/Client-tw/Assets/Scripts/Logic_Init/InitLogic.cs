using UnityEngine;
using System.Collections.Generic;

public class InitLogic:ILogic
{
    float m_Progress = 0;
    static bool m_State = true;
    InitLogicUI m_LogicUI;
    static short m_InitEndCount = 0;
    static short m_InitStartCount = 0;
    public bool Init(ILogicUI logicUI, object obj)
    {
        m_LogicUI = (InitLogicUI)logicUI;
        SceneMain.Instance.StartInnerCoroutine(MainInitProcedure(null));
        return true;
    }
    public float Progress
    {
        get { return m_Progress; }
        set { m_Progress = value; }
    }
    public static void SetInitFailed()
    {
        m_State = false;
    }
    public static void StartInit()
    {
        ++m_InitStartCount;
    }
    public static void EndInit()
    {
        ++m_InitEndCount;
    }
    public static bool IsCompletion
    {
        get
        {
            return m_InitStartCount == m_InitEndCount;
        }
    }
    public void Update(float delta)
    {
        if(!m_State)
        {
            NativeInterface.ShowMsgAndExit("res_load_err", 120);
            return;
        }
    }
    public void Shutdown()
    {

    }
    //主要初始化函数
    System.Collections.IEnumerator MainInitProcedure(object obj)
    {
        //SceneObjMgr.Instance.LoadSceneObj();
        yield return new WaitForEndOfFrame();
        m_State = ServerSetting.Init();
        if (!m_State)
        {
            LogMgr.Log("资源加载失败:ServerSetting");
        }
        yield return new WaitForEndOfFrame();
        m_State = LocalSetting.Init();
        if (!m_State)
        {
            LogMgr.Log("资源加载失败:LocalSetting");
        }
        yield return new WaitForEndOfFrame();
        m_State = FishResManager.Instance.Init();
        if (!m_State)
        {
            LogMgr.Log("资源加载失败:FishResManager");
        }
        yield return new WaitForEndOfFrame();
        m_State = PathManager.Instance.Init();
        if (!m_State)
        {
            LogMgr.Log("资源加载失败:PathManager");
        }
        yield return new WaitForEndOfFrame();
        m_State = GlobalEffectMgr.Instance.Init();
        if (!m_State)
        {
            LogMgr.Log("资源加载失败:GlobalEffectMgr");
        }
        yield return new WaitForEndOfFrame();
        m_State = GlobalAudioMgr.Instance.Init();
        if (!m_State)
        {
            LogMgr.Log("资源加载失败:GlobalAudioMgr");
        }
        yield return new WaitForEndOfFrame();
        m_State = GlobalHallUIMgr.Instance.Init();
        if (!m_State)
        {
            LogMgr.Log("资源加载失败:GlobalHallUIMgr");
        }
        while(IsCompletion == false)
        {
            if(m_State == false)
            {
                NativeInterface.ShowMsgAndExit("res_load_err", 212);
                SceneMain.Instance.Shutdown();
                yield break;
            }
            yield return new WaitForEndOfFrame();
        }
       
        LogicManager.Instance.InitCompletion();
        LogicManager.Instance.Forward(null);
        yield break;
    }
  
}
