using UnityEngine;
using System.Collections.Generic;


public enum SysMode
{
    SYS_NORMAL,
    SYS_MANAGER
}

public enum FriendListType
{
    MyFriendList,
    ApplyList,
}

public class FriendWndUI : BaseWnd
{
    GameObject[]        m_ChildObj = new GameObject[2];
    UIButton[]          m_BtnObj = new UIButton[2];
    GameObject[]        m_WarrningObj = new GameObject[2];
    GameObject[]        m_FriendItemObj = new GameObject[2];
    UIGrid[]            m_Grid = new UIGrid[2];
    UILabel             m_FriendsCout;
    Transform[]         m_ScrollTrans = new Transform[2];
    UIPanel[]           m_UIPanel = new UIPanel[2];
    List<FriendItemUI>  m_FriendList = new List<FriendItemUI>();
    List<ApplyItemUI>   m_ApplyList = new List<ApplyItemUI>();
    FriendListType      m_ListType = FriendListType.MyFriendList;

    public void Init(Transform parent)
    {
        m_BaseWndObject = parent.GetChild(3).gameObject;
        m_BaseTrans = m_BaseWndObject.transform;

        for (byte i = 0; i < 2; ++i )
        {
            m_ChildObj[i] = m_BaseTrans.GetChild(i).gameObject;
            m_FriendItemObj[i] = m_BaseTrans.GetChild(i).GetChild(0).gameObject;
            m_Grid[i] = m_BaseTrans.GetChild(i).GetChild(1).gameObject.GetComponent<UIGrid>();
            m_ScrollTrans[i] = m_BaseTrans.GetChild(i);
            m_UIPanel[i] = m_BaseTrans.GetChild(i).gameObject.GetComponent<UIPanel>();
        }

        m_FriendsCout = m_BaseTrans.GetChild(3).gameObject.GetComponent<UILabel>();

        for (int i = 0; i < 2; i++)
        {
            m_BtnObj[i] = m_BaseTrans.GetChild(i + 4).GetComponent<UIButton>();
            m_WarrningObj[i] = m_BtnObj[i].gameObject.transform.GetChild(1).gameObject;
            switch (i)
            {
                case 0:     //我的好友
                    UIEventListener.Get(m_BtnObj[i].gameObject).onClick = OnMyFriendMsg;
                    break;
                case 1:     //申请列表
                    UIEventListener.Get(m_BtnObj[i].gameObject).onClick = OnApplyMsg;
                    break;
            }
        }
        UIEventListener.Get(m_BaseTrans.GetChild(2).gameObject).onClick = OnAddFriendMsg;
       // m_SysMode = (byte)SysMode.SYS_NORMAL;
        PlayerRole.Instance.RelationManager.GetFriendMap();
    }
    void InitFriendItem(uint PlayerID, RelationRole Value)
    {
        FriendItemUI item = new FriendItemUI(ItemStae.STATE_NORMAL, PlayerID, Value);
        item.Init(m_FriendItemObj[0]);
        m_FriendList.Add(item);
    }
    void InitApplyItem(tagRelationRequest Value)
    {
        ApplyItemUI item = new ApplyItemUI();
        item.Init(m_FriendItemObj[1]);
        item.ShowApplayInfo(Value);
        m_ApplyList.Add(item);

    }
    public void UpdateListDate()
    {
        if (m_BaseWndObject.activeSelf != false && m_ListType == FriendListType.MyFriendList)
        {
            if (PlayerRole.Instance.RelationManager.GetFriendMap() == null ||
                PlayerRole.Instance.GiffManager.GetNowDaySendGiffInfo() == null ||
                PlayerRole.Instance.RoleChar.GetCharList() == null)        //获取今天已经赠数据
                return;
            m_FriendList.Clear();
            ClearGridChild(0);
            foreach (KeyValuePair<uint, RelationRole> map in PlayerRole.Instance.RelationManager.GetFriendMap())
            {
                InitFriendItem(map.Key, map.Value);
            }
            m_FriendList.Sort(CompareItemByTag);
            for (byte i = 0; i < m_FriendList.Count; i++)
            {
                m_Grid[0].AddChild(m_FriendList[i].m_BaseTrans, false);
                m_FriendList[i].RestLocalScale();
            }
            m_FriendsCout.text = string.Format("当前好友数：{0}", m_FriendList.Count);
        }
    }
    public void UpdateApplyListData()
    {
        if (m_BaseWndObject.activeSelf != false && m_ListType == FriendListType.ApplyList)
        {
            if (PlayerRole.Instance.RoleRelationRequest.GetAllInfo() == null)
                return;
            m_ApplyList.Clear();
            ClearGridChild(1);
            foreach (KeyValuePair<uint , tagRelationRequest>map in PlayerRole.Instance.RoleRelationRequest.GetAllInfo())
            {
                InitApplyItem(map.Value);
            }
            for (byte i = 0; i < m_ApplyList.Count; i++)
            {
                m_Grid[1].AddChild(m_ApplyList[i].m_BaseTrans, false);
                m_ApplyList[i].ResetScale();
            }
        }

    }
    static int CompareItemByTag(FriendItemUI item1, FriendItemUI item2)
    {
        if (item1.FriendItemInfo.GetLeaveOnlineDay() == 255 && item2.FriendItemInfo.GetLeaveOnlineDay() < 255)
            return -1;
        else if (item1.FriendItemInfo.GetLeaveOnlineDay() < 255 && item2.FriendItemInfo.GetLeaveOnlineDay() == 255)
            return 1;
        else
        {
            if (item1.FriendItemInfo.GetLevel() > item2.FriendItemInfo.GetLevel())
                return -1;
            else if (item1.FriendItemInfo.GetLevel() < item2.FriendItemInfo.GetLevel())
                return 1;
            else
                return 0;
        }
    }
    public void ShowWnd(bool bShow)
    {
        if (m_BaseWndObject.activeSelf != bShow)
            m_BaseWndObject.SetActive(bShow);
        if (bShow)
        {
            m_ListType = FriendListType.MyFriendList;
            SetChildObjActive();
            UpdateListDate();
        }

    }
    public void SetFriendHeader(uint userID, Texture2D tex)
    {
        if (m_ListType == FriendListType.MyFriendList)
        {
            for (byte i = 0; i < m_FriendList.Count; )
            {
                if (m_FriendList[i].FriendItemInfo.GetUserID() == userID)
                {
                    m_FriendList[i].SetMyHeader(tex);
                    break;
                }
                else
                    i++;
            }
        }
        else
        {
            for (byte i = 0; i < m_ApplyList.Count; )
            {
                if (m_ApplyList[i].UserID == userID)
                {
                    m_ApplyList[i].SetMyHeader(tex);
                    break;
                }
                else
                    i++;
            }
        }

    }
    public void UpdateNewMsgState(uint UserID, bool bSate = true)
    {
        for (byte i = 0; i < m_FriendList.Count; )
        {
            if (m_FriendList[i].FriendItemInfo.GetUserID() == UserID)
            {
                m_FriendList[i].SetNewMsgSate();
                break;
            }
            else
                i++;
        }
    }
    public void UpdateAllNewMsg()
    {
        if (PlayerRole.Instance.RoleChar.GetCharList() == null)
            return;
        foreach (KeyValuePair<uint, tagRoleCharInfo> map in PlayerRole.Instance.RoleChar.GetCharList())
        {
            for (byte i = 0; i < m_FriendList.Count; )
            {
                if (m_FriendList[i].FriendItemInfo.GetUserID() == map.Value.SrcUserID)
                {
                    m_FriendList[i].SetNewMsgSate();
                    break;
                }
                else
                    i++;
            }
        }
    }
    void OnMyFriendMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_ListType = FriendListType.MyFriendList;
        SetChildObjActive();
        UpdateListDate();
    }
    void OnApplyMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_ListType = FriendListType.ApplyList;
        SetChildObjActive();
        UpdateApplyListData();
    }
    void OnAddFriendMsg(GameObject go)
    {
        FriendRuntime.SearchUI.ShowWnd(true);
        WndManager.Instance.Push(FriendRuntime.SearchUI.m_BaseWndObject);

        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
   
    public void DeleteFriend(uint userID)
    {
        for (byte i = 0; i < m_FriendList.Count; )
        {
            if (m_FriendList[i].FriendItemInfo.GetUserID() == userID)
            {
                FriendItemUI item = m_FriendList[i];
                m_Grid[0].RemoveChild(item.m_BaseTrans);
                item.ShutDown();
                Utility.ListRemoveAt(m_FriendList, i);
                //删除完好友后将本地保聊天信息清除
                RuntimeInfo.DeleteLocalChatFile(RuntimeInfo.GetChatLogFileName(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), userID));
                break;
            }
            else
                i++;
        }
        m_ScrollTrans[0].localPosition = new Vector3(151, 46.8f, 0);
        m_UIPanel[0].clipOffset = new Vector2(0, -142);
        m_FriendsCout.text = string.Format("当前好友数：{0}", m_FriendList.Count);
    }
    public void PressentScuess(uint userID)
    {
        for (byte i = 0; i < m_FriendList.Count; )
        {
            if (m_FriendList[i].FriendItemInfo.GetUserID() == userID)
            {
                m_FriendList[i].SetPressentBtn();
                break;
            }
            else
                i++;
        }

    }
    void SetChildObjActive()
    {
        if (m_ListType == FriendListType.MyFriendList)
        {
            m_ChildObj[0].SetActive(true);
            m_ChildObj[1].SetActive(false);
            ChangeButtonSprite(0, "My_Friend_Btn02");
            ChangeButtonSprite(1, "Apply_Friend_Btn01");
        }
        else
        {
            m_ChildObj[1].SetActive(true);
            m_ChildObj[0].SetActive(false);
            ChangeButtonSprite(0, "My_Friend_Btn01");
            ChangeButtonSprite(1, "Apply_Friend_Btn02");
        }
    }
    void ChangeButtonSprite(byte Idx, string str)
    {
        m_BtnObj[Idx].pressedSprite = str;
        m_BtnObj[Idx].normalSprite = str;
        m_BtnObj[Idx].hoverSprite = str;
    }
    void ClearGridChild(byte Idx)
    {
        List<Transform> gridChid = m_Grid[Idx].GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_Grid[Idx].transform.DetachChildren();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            m_FriendList.Clear();
            m_ApplyList.Clear();
            ClearGridChild(0);
            ClearGridChild(1);
            GameObject.Destroy(m_BaseWndObject);
        }
    }
    public void UpdateWarningState()
    {
        m_WarrningObj[0].SetActive(PlayerRole.Instance.RoleChar.GetCharList() != null && PlayerRole.Instance.RoleChar.GetCharList().Count > 0);
        m_WarrningObj[1].SetActive(PlayerRole.Instance.RoleStatesMessage.GetRelationStates());
    }
}
