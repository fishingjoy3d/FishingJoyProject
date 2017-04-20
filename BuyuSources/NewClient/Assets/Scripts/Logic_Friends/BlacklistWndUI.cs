using UnityEngine;
using System.Collections.Generic;

public class BlacklistWndUI : BaseWnd
{
    GameObject          m_CloseBtn;
    TweenScale          m_TweenAnim;
    Transform           m_ScrollTrans;
    GameObject          m_ScrollItem;
    UIGrid[]            m_Gird = new UIGrid[3];
    List<ViewWndUI>     m_BlackList = new List<ViewWndUI>();
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.FriendSysObj[3]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_CloseBtn = m_BaseTrans.GetChild(0).gameObject;
        UIEventListener.Get(m_CloseBtn).onClick = OnCloseMsg;

        m_TweenAnim = m_BaseWndObject.GetComponent<TweenScale>();

        m_ScrollTrans = m_BaseTrans.GetChild(1);
        m_ScrollItem = m_ScrollTrans.GetChild(0).gameObject;
        for (int j = 1; j < 4; j++)
        {
            m_Gird[j - 1] = m_ScrollTrans.GetChild(j).gameObject.GetComponent<UIGrid>();
        }

    }
    public void UpdateBlacklistDate()
    {
        if (m_BaseWndObject == null)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (PlayerRole.Instance.RelationManager.GetBlackMap() == null)
            return;
        ClearGridChild();
        m_BlackList.Clear();
        byte Idx = 0;
        foreach (KeyValuePair<uint, RelationRole> map in PlayerRole.Instance.RelationManager.GetBlackMap())
        {
            ViewWndUI item = new ViewWndUI(ItemType.Blacklist_Item, null, map.Value);
            item.Init(m_ScrollItem);
            m_Gird[Idx].AddChild(item.m_BaseTrans);
            item.ResetLocalScale();
            m_BlackList.Add(item);
            if ((++Idx) > 2)
                Idx = 0;
        }
    }
    public void ShowWnd(bool bShow)
    {
        if (m_BaseWndObject != null)
            return;

        Init();
        WndManager.Instance.Push(m_BaseWndObject);
        m_TweenAnim.ResetToBeginning();
        m_TweenAnim.PlayForward();
        UpdateBlacklistDate();
    }
    public void SetHeader(uint userID, Texture2D tex)
    {
        for (int i = 0; i < m_BlackList.Count; ++i)
        {
            if (userID == m_BlackList[i].PlayerID)
            {
                m_BlackList[i].SetHeader(tex);
                break;
            }
        }
    }
    void OnCloseMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        ShutDown();
        //FriendRuntime.FriendSysMgr.SetWndMask(true);
    }
    void ClearGridChild()
    {
        for (byte i = 0; i < 3; i++)
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
            ClearGridChild();
            GameObject.Destroy(m_BaseWndObject);
        }
    }
}
