using UnityEngine;
using System.Collections.Generic;


public enum ButtonType
{
    Close_Btn,
    Friend_Btn,
    Email_Btn,
    Present_Btn
}

public enum WndType
{
    Friend_Wnd,
    Emanil_Wnd,
    Present_Wnd,
    SendMsg_Wnd,
    Search_Wnd,
    Blacklist_Wnd
}

public struct TopButton
{
    public GameObject m_BtnObj;
    public UIButton m_UIButton;
    public UISprite m_UISprite;
    public Transform m_BtnTrans;
}

public struct ButtonMask
{
    public GameObject       m_Mask;
    public Transform        m_MaskTrans;
    public TweenPosition    m_TweenAnim;
}

public class BaseWnd
{
    public GameObject m_BaseWndObject;
    public Transform m_BaseTrans;
}

public class FriendSysMgr : BaseWnd
{
 //   TweenScale      m_TweenAnim;
    TopButton[]     m_TopBtnObj = new TopButton[3];
    GameObject[]    m_Warning = new GameObject[2];
  //  ButtonMask      m_MaskObj;
    WndType         m_ShowWndType;
    FriendWndUI     m_FriendWndUI = new FriendWndUI();
    EmailWndUI      m_EmailWndUI = new EmailWndUI();
    PresentWndUI    m_PresentWndUI = new PresentWndUI();
    SearchWndUI     m_SearchWndUI = new SearchWndUI();
    SendMsgWndUI    m_SendMsgWndUI = new SendMsgWndUI();
    BlacklistWndUI  m_BlacklistWndUI = new BlacklistWndUI();
    ApplyCodeWnd    m_ApplyCodeWnd = new ApplyCodeWnd();

    public FriendSysMgr()
    {
        m_ShowWndType = WndType.Friend_Wnd;
    }

    public void Init()
    {
        FriendRuntime.Init(this);
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.FriendSysObj[0]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        for (int i = 0; i < m_TopBtnObj.Length; i++)
        {
            m_TopBtnObj[i].m_BtnObj = m_BaseTrans.GetChild(i).gameObject;

            if (i > 0)
            {
                m_TopBtnObj[i].m_UISprite = m_TopBtnObj[i].m_BtnObj.transform.GetChild(0).GetComponent<UISprite>();
                m_Warning[i - 1] = m_BaseTrans.GetChild(i).GetChild(1).gameObject;
            }
            switch (i)
            {
                case (int)ButtonType.Close_Btn:
                    UIEventListener.Get(m_TopBtnObj[i].m_BtnObj).onClick = OnCloseMsg;
                    break;
                case (int)ButtonType.Friend_Btn:
                    UIEventListener.Get(m_TopBtnObj[i].m_BtnObj).onClick = OnFriendMsg;
                    break;
                case (int)ButtonType.Email_Btn:
                    UIEventListener.Get(m_TopBtnObj[i].m_BtnObj).onClick = OnEmailMsg;
                    break;
            }
        }

        m_FriendWndUI.Init(m_BaseTrans);
        m_EmailWndUI.Init(m_BaseTrans);
        m_PresentWndUI.Init(m_BaseTrans);

    }
    
    public void Update(float deltaTime)
    {
        if ( m_BaseWndObject== null || m_BaseWndObject.activeSelf != true)
            return;
        if (m_SearchWndUI.m_BaseWndObject != null)
        {
            m_SearchWndUI.Update(deltaTime);
            return;
        }
        else if (m_SendMsgWndUI.m_BaseWndObject != null)
        {
            m_SendMsgWndUI.Update(deltaTime);
            return;
        }
        else if (m_BlacklistWndUI.m_BaseWndObject != null)
        {
            
            return;
        }
        else
        {
            switch (m_ShowWndType)
            {
                case WndType.Friend_Wnd:
                   // m_FriendWndUI.Update(deltaTime);
                    break;
                case WndType.Emanil_Wnd:
                    m_EmailWndUI.Update(deltaTime);
                    break;
                case WndType.Present_Wnd:
                    break;
            }
        }
        UpdateWarningState(WndType.Emanil_Wnd);

    }
    public void UpdateWarningState(WndType type)
    {
        m_Warning[0].SetActive((PlayerRole.Instance.RoleChar.GetCharList() != null && PlayerRole.Instance.RoleChar.GetCharList().Count > 0) || PlayerRole.Instance.RoleStatesMessage.GetRelationStates());
        m_Warning[1].SetActive(PlayerRole.Instance.RoleStatesMessage.GetMailStates() || PlayerRole.Instance.RoleStatesMessage.GetGiffStates());
        m_FriendWndUI.UpdateWarningState();
        m_EmailWndUI.UpdateWarrningSate();
        //if (type == WndType.Emanil_Wnd)
        //{
        //    m_Warning[1].SetActive(PlayerRole.Instance.RoleStatesMessage.GetMailStates());
        //}
        //else if (type == WndType.Present_Wnd)
        //{
        //    m_Warning[2].SetActive(PlayerRole.Instance.RoleStatesMessage.GetGiffStates());
        //}
        //m_EmailWndUI.UpdateWarrningSate();
    }
    public void ShowWnd()
    {
        Init();
        m_BaseWndObject.SetActive(true);
        OnFriendMsg(m_TopBtnObj[1].m_BtnObj);
        m_Warning[1].SetActive(PlayerRole.Instance.RoleStatesMessage.GetMailStates());
      //  m_Warning[2].SetActive(PlayerRole.Instance.RoleStatesMessage.GetGiffStates());
        WndManager.Instance.Push(m_BaseWndObject);
    }
    void ShowChildWnd(WndType type, bool bshow)
    {
        switch (type)
        {
            case WndType.Friend_Wnd:
                m_FriendWndUI.ShowWnd(bshow);
                break;
            case WndType.Emanil_Wnd:
                m_EmailWndUI.ShowWnd(bshow);
                break;
            case WndType.Present_Wnd:

               // m_PresentWndUI.ShowWnd(bshow);
                break;
        }
    }
    void CheckHighLight(WndType mode)
    {
        int Idx = (int)m_ShowWndType + 1;
        m_TopBtnObj[Idx].m_UISprite.spriteName = GetSpriteName(m_ShowWndType, 1);

        m_ShowWndType = mode;
        Idx = (int)mode + 1;
        m_TopBtnObj[Idx].m_UISprite.spriteName = GetSpriteName(m_ShowWndType, 2);

    }
    string GetSpriteName(WndType mode, int state)
    {
        string str = string.Format("");

        switch (mode)
        {
            case WndType.Friend_Wnd:
                str = string.Format("Friend_Btn0{0}", state);
                return str;
            case WndType.Emanil_Wnd:
                str = string.Format("Email_Btn0{0}", state);
                return str;
            case WndType.Present_Wnd:
                str = string.Format("Present_Btn0{0}", state);
                return str;
        }
        return null;
    }
    void OnCloseMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        ShutDown();
        
    }
    void OnFriendMsg(GameObject go)
    {
        if (m_ShowWndType == WndType.Friend_Wnd)
        {
            m_FriendWndUI.UpdateListDate();
            return;
        }
        ShowChildWnd(m_ShowWndType, false);
        CheckHighLight(WndType.Friend_Wnd);

        ShowChildWnd(m_ShowWndType, true);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void OnEmailMsg(GameObject go)
    {
        if (m_ShowWndType == WndType.Emanil_Wnd)
            return;
        ShowChildWnd(m_ShowWndType, false);
        CheckHighLight(WndType.Emanil_Wnd);

        ShowChildWnd(m_ShowWndType, true);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
  
    public void SetHeader(uint id, byte type, Texture2D tex)
    {
        if (type == (byte)WndType.Friend_Wnd)
            FriendRuntime.FriendUI.SetFriendHeader(id, tex);
        else if (type == (byte)WndType.Emanil_Wnd)
            FriendRuntime.EmailUI.SetMailHeader(id, tex);
        else
            FriendRuntime.PresentUI.SetPressentHeader(id, tex);
    }

    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
            FriendRuntime.Shutdown();
            if (HallRunTimeInfo.Instance_UI != null)
            {
                HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(true);
            }
            m_FriendWndUI.ShutDown();
            m_EmailWndUI.ShutDown();
            m_PresentWndUI.ShutDown();
            m_SendMsgWndUI.ShutDown();
            m_SearchWndUI.ShutDown();
          //  m_BlacklistWndUI.ShutDown();
        }
    }
    public FriendWndUI FriendWndUI
    {
        get { return m_FriendWndUI; }
    }
    public EmailWndUI EmailWndUI
    {
        get { return m_EmailWndUI; }
    }
    public SendMsgWndUI SendMsgWndUI
    {
        get { return m_SendMsgWndUI; }
    }
    public PresentWndUI PresentWndUI
    {
        get { return m_PresentWndUI; }
    }
    public SearchWndUI SearchWndUI
    {
        get { return m_SearchWndUI; }
    }
    public BlacklistWndUI BlacklistWndUI
    {
        get { return m_BlacklistWndUI; }
    }
    public ApplyCodeWnd ApplyCodeWnd
    {
        get { return m_ApplyCodeWnd; }
    }
}
