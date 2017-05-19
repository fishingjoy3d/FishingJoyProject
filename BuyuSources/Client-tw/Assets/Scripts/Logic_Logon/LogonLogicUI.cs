using UnityEngine;
using System.Collections.Generic;

public enum LogonWnd_Status
{
    LogonWnd_Init,          //先择登录  
    LogonWnd_Account,       //输账号登录
    LogonWnd_Phone,         //手机登陆
    LogonWnd_FindPS,        //找回密码
    LogonWnd_Resgiter,      //注册
}

public class LogonLogicUI : ILogicUI
{
    LogonLogic          m_Logic;
    Object[]            m_LogonObject = new Object[5];
    string              m_str;
    LogonInitWnd        m_LogonInitWnd = new LogonInitWnd();
    LogonNormalWnd      m_LogonNormalWnd = new LogonNormalWnd();
    PhoneLogon m_phonelogon = new PhoneLogon();
    ForgetPassWord m_forgetps = new ForgetPassWord();
    LogonRegisterWnd    m_LogonRegisterWnd = new LogonRegisterWnd();
    bool                m_bInit = false;
    LogonWnd_Status     m_CurLogonStatus;    

    public bool Init(ILogic logic, object obj)
    {
        m_Logic = (LogonLogic)logic;
        m_LogonObject[0] = ResManager.Instance.LoadObject("LogonWidget0", "LogonRes/Prefab/UI/", ResType.LogonRes);
        if (SDKMgr.IS_SDK_CHANNEL)
        {
            //渠道包，点击屏幕登陆。
            BeginChangeLogonWnd();
        }
        else
        {
            for (byte i = 1; i < m_LogonObject.Length; ++i)
            {
                m_LogonObject[i] = ResManager.Instance.LoadObject(string.Format("LogonWidget{0}", i), "LogonRes/Prefab/UI/", ResType.LogonRes);
            }
            if (m_Logic.State == LogonState.LOGON_INIT)
            {
                BeginChangeLogonWnd();
            }
        }
        SceneObjMgr.Instance.ResetBackTex();
        LogicManager.Instance.InitCompletion();
        return true;
    }
    public void UpdateNotice(List<AnnouncementData> annoucesList)
    {
        m_LogonInitWnd.UpdateNoticeDate(annoucesList);
    }
    void BeginChangeLogonWnd()
    {
        GlobalLoading.Instance.Hide();
        GlobalEffectMgr.Instance.CloseLoadingMessage();
        m_bInit = true;
        ChangeLogonWnd(LogonWnd_Status.LogonWnd_Init);
    }
    public void OnStateChanged(LogonState state)
    {
        switch(state)
        {
            case LogonState.LOGON_NET_ERROR:
                ChangeLogonWnd(m_CurLogonStatus);
                break;
            case LogonState.LOGON_ERROR:
                ChangeLogonWnd(m_CurLogonStatus);
                break;
        }
    }
    public void ChangeLogonWnd(LogonWnd_Status status)
    {
        switch (status)
        {
            case LogonWnd_Status.LogonWnd_Init:
                {
                    m_LogonInitWnd.Init(m_LogonObject[0]);
                    break;
                }
            case LogonWnd_Status.LogonWnd_Account:
                {
                    m_LogonNormalWnd.Init(m_LogonObject[1]);
                    break;
                }
            case LogonWnd_Status.LogonWnd_Phone:
                {
                    m_phonelogon.Init(m_LogonObject[2]);
                    break;
                }
            case LogonWnd_Status.LogonWnd_FindPS:
                {
                    m_forgetps.Init(m_LogonObject[3], m_CurLogonStatus == LogonWnd_Status.LogonWnd_Phone);
                    break;
                }
            case LogonWnd_Status.LogonWnd_Resgiter:
                {
                    m_LogonRegisterWnd.Init(m_LogonObject[4]);
                    break;
                }
        }
        m_CurLogonStatus = status;
    }
    public void OnGUI(float delta)
    {
       //if (SDKMgr.IS_SDK_CHANNEL)
       //{
       //    if(Input.GetMouseButtonDown(0))
       //    {
       //        if (m_Logic.IsWaitLogon)
       //        {
       //            //点击登录
       //            m_Logic.SDKLogin();
       //        }
       //    }
       //}
    }
    public void CloseLogonInitWnd()
    {
        if (m_LogonInitWnd != null)
            m_LogonInitWnd.ShutDown();
    }
    public void Update(float delta)
    {
        if (SDKMgr.IS_SDK_CHANNEL)
        {
            if(m_LogonInitWnd == null)
            {
                LogMgr.Log("<InitWnd Is Null>");
            }
            m_LogonInitWnd.IsShowLabel(m_Logic.IsWaitLogon);
            return;
        }
        if (m_bInit == false && m_Logic.IsWaitLogon)
        {
            BeginChangeLogonWnd();
        }
        else if(m_Logic.State == LogonState.LOGON_ERROR)
        {
            m_Logic.State = LogonState.LOGON_INIT;
            ChangeLogonWnd(m_CurLogonStatus);
            GlobalEffectMgr.Instance.CloseLoadingMessage();
        }
        //m_LogonNormalWnd.Update(delta);
        m_forgetps.Update(delta);
    }
    public void Shutdown()
    {
        //GlobalEffectMgr.Instance.CloseWindMessage();
        for (byte i = 0; i < m_LogonObject.Length; ++i )
            ResManager.Instance.UnloadObject(m_LogonObject[i]);

        m_LogonInitWnd.ShutDown();      
        m_LogonNormalWnd.ShutDown();
        m_phonelogon.ShutDown();
        m_forgetps.ShutDown();
        m_LogonRegisterWnd.ShutDown();       
    }   
}