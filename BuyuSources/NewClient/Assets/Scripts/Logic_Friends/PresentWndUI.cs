using UnityEngine;
using System.Collections.Generic;

public class PresentWndUI : BaseWnd
{
    GameObject              m_BtnObj;
    GameObject              m_PresentItem;
    UIGrid                  m_Grid;
    UILabel                 m_ReceiveLable = null;
    UIScrollView            m_ScrollView = null;
    List<PresentItemUI>     m_PresentList = new List<PresentItemUI>();


    public void Init(Transform parent)
    {
        m_BaseWndObject = parent.GetChild(4).GetChild(3).gameObject;
        m_BaseTrans = m_BaseWndObject.transform;

        m_ScrollView = m_BaseTrans.GetChild(0).GetComponent<UIScrollView>();
        m_PresentItem = m_BaseTrans.GetChild(0).GetChild(0).gameObject;
        m_Grid = m_BaseTrans.GetChild(0).GetChild(1).gameObject.GetComponent<UIGrid>();

        m_BtnObj = m_BaseTrans.GetChild(1).gameObject;
        UIEventListener.Get(m_BtnObj).onClick = OnAllReceiveMsg;

        m_ReceiveLable = m_BaseTrans.GetChild(2).gameObject.GetComponent<UILabel>();
    }
    void InitPresentItem(uint onlyID, tagGiffClientInfo tagItemInfo)
    {
        PresentItemUI item = new PresentItemUI(tagItemInfo);
        item.Init(m_PresentItem);
        m_PresentList.Add(item);

    }
    public void UpdatePresentListDate()
    {
        if (m_BaseWndObject.activeSelf != true)
            return;
        ClearPressentGridChild();
        m_PresentList.Clear();
        if (PlayerRole.Instance.GiffManager.GetAllGiffMap() == null)
            return;
        foreach (KeyValuePair<uint, tagGiffClientInfo> map in PlayerRole.Instance.GiffManager.GetAllGiffMap())
        {
            InitPresentItem(map.Key, map.Value);
        }
        SetPresentListDate();
        m_ReceiveLable.text = string.Format("已领取：{0}/{1}", PlayerRole.Instance.RoleInfo.RoleMe.GetAcceptGiffSum(), FishConfig.Instance.m_GiffInfo.AcceptSubLimitByDay);
    }
    public void UpdateAddPresentDate(uint onlyID)
    {
        if (PlayerRole.Instance.GiffManager.GetAllGiffMap() == null)
            return;
        if (PlayerRole.Instance.GiffManager.GetAllGiffMap().ContainsKey(onlyID))
        {
            InitPresentItem(onlyID, PlayerRole.Instance.GiffManager.GetAllGiffMap()[onlyID]);
            SetPresentListDate();
        }

    }
    void SetPresentListDate()
    {
        SortList();
        for (byte i = 0; i < m_PresentList.Count; ++i)
        {
            m_Grid.AddChild(m_PresentList[i].m_BaseTrans);
            m_PresentList[i].ResetLocalScale();
        }
    }
   
    void SortList()
    {
        m_PresentList.Sort(CompareListByTime);
    }
    static int CompareListByTime(PresentItemUI item1, PresentItemUI item2)
    {
        if (item1.PresentItemInfo.DiffTime < item2.PresentItemInfo.DiffTime)
            return 1;
        else if (item1.PresentItemInfo.DiffTime > item2.PresentItemInfo.DiffTime)
            return -1;
        else
            return 0;
    }
	public void ShowWnd(bool bShow)
    {
        if (m_BaseWndObject.activeSelf != bShow)
            m_BaseWndObject.SetActive(bShow);

        //
        if (bShow)
            UpdatePresentListDate();
    }
    public void SetPressentHeader(uint id , Texture2D tex)
    {
        for (byte i = 0; i < m_PresentList.Count; ++i)
        {
            if (m_PresentList[i].m_PresentItemInfo.dwUserID == id)
                m_PresentList[i].SetHeader(tex);
        }
    }
    void OnAllReceiveMsg(GameObject go)
    {
        //if (!FriendRuntime.FriendSysMgr.bMask)
        //    return;
        PlayerRole.Instance.GiffManager.AcceptAllGiff();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    public void UpdateReceiverPressent(uint onlyID)
    {
        if (m_BaseWndObject.activeSelf != true)
            return;

        for (byte i = 0; i < m_PresentList.Count; )
        {
            if (m_PresentList[i].PresentItemInfo.OnlyID == onlyID)
            {
                PresentItemUI item = m_PresentList[i];
                m_Grid.RemoveChild(item.m_BaseTrans);
                item.ShutDown();
                Utility.ListRemoveAt(m_PresentList, i);
                break;
            }
            else
                i++;
        }
    }
    public void UpdateAcceptGiffSum()
    {
        if (m_BaseWndObject.activeSelf != true)
            return;

        m_ReceiveLable.text = string.Format("今日已领取：{0}/{1}", PlayerRole.Instance.RoleInfo.RoleMe.GetAcceptGiffSum(), FishConfig.Instance.m_GiffInfo.AcceptSubLimitByDay);
    }
    void ClearPressentGridChild()
    {
        List<Transform> gridChid = m_Grid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_Grid.transform.DetachChildren();
    }
    public void SetMask(bool bMask)
    {
        m_ScrollView.enabled = bMask;
    }
    public void ShutDown()
    {
        m_PresentList.Clear();
        ClearPressentGridChild();
    }
}
