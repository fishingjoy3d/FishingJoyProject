using UnityEngine;
using System.Collections.Generic;

public class SearchWndUI : BaseWnd
{

    GameObject[]    m_BtnObj = new GameObject[2];
    TweenScale      m_TweenAnim;
    Transform       m_ScrollTrans;
    GameObject      m_ScrollItem;
    UIGrid[]        m_Gird = new UIGrid[3];
    UIInput         m_InputName;
    UIButton        m_UISearchButton;
    UISprite        m_UISearchBtnBg;
    UIPanel         m_UIPanel;
    List<ViewWndUI> m_SearchList = new List<ViewWndUI>();
    bool            m_bCDState;          //是否处于CD状态
    float           m_CDSearchTime;
    float           m_CDCurTime;

    public SearchWndUI()
    {
        m_bCDState = false;
        m_CDSearchTime = 5;
        m_CDCurTime = 0;
    }

    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.FriendSysObj[1]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_TweenAnim = m_BaseWndObject.GetComponent<TweenScale>();
        for (int i = 0; i < 2; i++)
        {
            m_BtnObj[i] = m_BaseTrans.GetChild(i).gameObject;
            if (i == 0)
                UIEventListener.Get(m_BtnObj[i]).onClick = OnCloseBtnMsg;
            else
            {
                m_UISearchButton = m_BtnObj[i].GetComponent<UIButton>();
                m_UISearchBtnBg = m_BaseTrans.GetChild(i).GetChild(1).GetComponent<UISprite>();
                UIEventListener.Get(m_BtnObj[i]).onClick = OnSearchBtnMsg;
            }
        }
        m_ScrollTrans = m_BaseTrans.GetChild(3);
        m_ScrollItem = m_ScrollTrans.GetChild(0).gameObject;
        m_UIPanel = m_ScrollTrans.gameObject.GetComponent<UIPanel>();
        m_InputName = m_BaseTrans.GetChild(2).gameObject.GetComponent<UIInput>();
        for (int j = 1; j < 4; j++)
        {
            m_Gird[j - 1] = m_ScrollTrans.GetChild(j).gameObject.GetComponent<UIGrid>();
        }

    }
    public void UpateQueryListDate()
    {
        if (m_BaseWndObject.activeSelf != true)
            return;
        ClearGird();
        m_SearchList.Clear();
        m_ScrollTrans.localPosition = new Vector3(0, -45f, 0);
        m_UIPanel.clipOffset = new Vector2(0, -4.5f);
        for (byte i = 0; i < PlayerRole.Instance.QueryManager.GetQueryList().Count; ++i)
        {
            ViewWndUI item = new ViewWndUI(ItemType.Search_Item, PlayerRole.Instance.QueryManager.GetQueryList()[i], null);
            item.Init(m_ScrollItem);
            m_Gird[i%3].AddChild(item.m_BaseTrans);
            item.ResetLocalScale();
            m_SearchList.Add(item);
        }

        //m_CDCurTime = 0;
        //m_bCDState = false;
        //m_UISearchButton.isEnabled = true;
    }
    public void Update(float delta)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;

        if (m_bCDState)
        {
            m_CDCurTime += delta;
            if (m_CDCurTime >= m_CDSearchTime)
            {
                m_CDCurTime = 0;
                m_bCDState = false;
                m_UISearchButton.isEnabled = true;
                m_UISearchBtnBg.spriteName = "Search_btn0";
            }
        }
    }
    public void ShowWnd(bool bShow)
    {
        if (m_BaseWndObject != null)
            return;

        Init();
        m_CDCurTime = 0;
        m_bCDState = true;
        m_UISearchButton.isEnabled = false;
        m_UISearchBtnBg.spriteName = "Search_btn2";
        m_TweenAnim.ResetToBeginning();
        m_TweenAnim.PlayForward();

        ClearGird();
        m_InputName.value = "";
        PlayerRole.Instance.QueryManager.QueryUserListByNickName(m_InputName.value, 0, false);
       // QueryMsg.Instance.SendQueryRoleByNickName(m_InputName.value, 0, false);
    }
    public void SetHeader(uint userID, Texture2D tex)
    {
        for (int i = 0; i < m_SearchList.Count; ++i)
        {
            if (userID == m_SearchList[i].PlayerID)
            {
                m_SearchList[i].SetHeader(tex);
                break;
            }
        }
    }
    void OnSearchBtnMsg(GameObject go)
    {
        m_CDCurTime = 0;
        m_bCDState = true;
        m_UISearchButton.isEnabled = false;
        m_UISearchBtnBg.spriteName = "Search_btn2";
        PlayerRole.Instance.QueryManager.QueryUserListByNickName(m_InputName.value, 0, false);
       
    }
    void OnCloseBtnMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        ShutDown();
       // FriendRuntime.FriendSysMgr.SetWndMask(true);
    }
    void ClearGird()
    {
        for (byte i = 0; i < 3; ++i)
        {
            List<Transform> gridChid = m_Gird[i].GetChildList();

            foreach (Transform tr in gridChid)
            {
                GameObject.Destroy(tr.gameObject);
            }
            m_Gird[i].transform.DetachChildren();
        }

    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            ClearGird();
            m_BaseWndObject = null;
        }
    }
}



public class ApplyCodeWnd : BaseWnd
{
    uint m_UserID;
    UIInput m_UIInput;
    void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.FriendSysObj[4]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_UIInput = m_BaseTrans.GetChild(1).GetComponent<UIInput>();
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = CloseMsg;
        UIEventListener.Get(m_BaseTrans.GetChild(2).gameObject).onClick = OnOK;
    }
    public void ShowApplyCodeWnd(uint UserID)
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseWndObject);
        }
        m_UserID = UserID;

    }
    void OnOK(GameObject go)
    {
        //PlayerRole.Instance.RelationManager.AddOtherRoleRelation(m_UserID, 1);
        PlayerRole.Instance.RoleRelationRequest.SendRelationRequest(m_UserID, 1, m_UIInput.value);
        ShutDown();
    }
    void CloseMsg(GameObject go)
    {
        ShutDown();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
}