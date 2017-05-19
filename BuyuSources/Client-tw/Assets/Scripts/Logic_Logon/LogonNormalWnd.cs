using UnityEngine;
using System.Collections.Generic;


public class LogonNormalWnd
{
   protected GameObject m_gownd;
   protected UIInput m_inputAccount;
   protected UIInput m_inputps;
   protected  UISprite m_spriteremember;
    public void Init(Object obj)
    {
        m_gownd=GameObject.Instantiate(obj) as GameObject;
        m_gownd.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_inputAccount = m_gownd.transform.GetChild(5).GetChild(0).GetComponent<UIInput>();
        m_inputps = m_gownd.transform.GetChild(5).GetChild(1).GetComponent<UIInput>();

        UIEventListener.Get(m_gownd.transform.GetChild(0).gameObject).onClick = EventClose;
        UIEventListener.Get(m_gownd.transform.GetChild(1).gameObject).onClick = EventRemember;
        UIEventListener.Get(m_gownd.transform.GetChild(2).gameObject).onClick = EventForget;
        UIEventListener.Get(m_gownd.transform.GetChild(3).gameObject).onClick = EventLogon;
        UIEventListener.Get(m_gownd.transform.GetChild(4).gameObject).onClick = EventRegister;                
        m_spriteremember = m_gownd.transform.GetChild(1).GetChild(2).GetComponent<UISprite>();   
        SetSprite();
        FillLogonInfo();

    }
    public virtual void FillLogonInfo()
    {
        if (GlobalLogon.Instance.AccountData != null && GlobalLogon.Instance.AccountData.AccountInfo.UID != null && GlobalLogon.Instance.AccountData.AccountInfo.UID.Length > 0)
        {
            m_inputAccount.value = GlobalLogon.Instance.AccountData.AccountInfo.UID;                      
            if(GlobalLogon.Instance.AccountData.bIsSavePassword&&
                GlobalLogon.Instance.AccountData.AccountInfo.CRC1 != 0 &&
                GlobalLogon.Instance.AccountData.AccountInfo.CRC2 != 0 &&
                GlobalLogon.Instance.AccountData.AccountInfo.CRC3 != 0)
            {
                m_inputps.value = "********";
            }
        }
    }

   public virtual void SetSprite()
    {
        if (GlobalLogon.Instance.AccountData != null)
        {
            m_spriteremember.gameObject.SetActive(GlobalLogon.Instance.AccountData.bIsSavePassword); 
        }         
    }
    void EventClose(GameObject go)
    {
        ShutDown();
        LogonRuntime.LogonLogicUI.ChangeLogonWnd(LogonWnd_Status.LogonWnd_Init);
    }
    public virtual void ChangeState()
    {
        if (GlobalLogon.Instance.AccountData == null)
        {
            GlobalLogon.Instance.AccountData = new AccountData();       
        }
        GlobalLogon.Instance.AccountData.bIsSavePassword = !GlobalLogon.Instance.AccountData.bIsSavePassword;
        GlobalLogon.Instance.AccountData.SaveAccountData();
        SetSprite();
    }
    void EventRemember(GameObject go)
    {
        ChangeState();                
    }
    void EventForget(GameObject go)
    {
        ShutDown();
        LogonRuntime.LogonLogicUI.ChangeLogonWnd(LogonWnd_Status.LogonWnd_FindPS);       
    }
    void EventLogon(GameObject go)
    {
        Logon(go);
    }
    public    virtual void Logon(GameObject go) 
    {
        if (m_inputAccount.value.Length != 0 && m_inputps.value.Length != 0)
        {
            ShutDown();
            GlobalEffectMgr.Instance.ShowLoadingMessage();
            AccountInfo rd = new AccountInfo();
            rd.UID = m_inputAccount.value;
            rd.PWD = m_inputps.value;

            if (GlobalLogon.Instance.AccountData != null && m_inputps.value == "********" && rd.UID == GlobalLogon.Instance.AccountData.AccountInfo.UID)
                LogonRuntime.LogonLogic.GuestLogon();
            else
                LogonRuntime.LogonLogic.Logon(rd);           
        }      
    }
    void EventRegister(GameObject go)
    {
        ShutDown();
        LogonRuntime.LogonLogicUI.ChangeLogonWnd(LogonWnd_Status.LogonWnd_Resgiter);        
    }

     public void ShutDown()
    {
        if (m_gownd != null)
         {
             GameObject.Destroy(m_gownd);
             m_gownd = null;
         }
    }
}