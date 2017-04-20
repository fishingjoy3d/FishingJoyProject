using UnityEngine;
using System.Collections.Generic;


public enum EmailChildType
{
    EmailList_Wnd,
    Gift_Wnd,
}

public class EmailWndUI : BaseWnd
{
    GameObject[]        m_ChildObj = new GameObject[2];
    GameObject[]        m_WarrningObj = new GameObject[2];
    UIButton[]          m_BtnObj = new UIButton[2];
    GameObject          m_EmailItemObj = null;
    GameObject          m_EmailListWnd = null;
    UIGrid              m_Grid = null;
    UIScrollView        m_ScrollView = null;
    UILabel             m_EmailNum = null;
    EmailChildType       m_ChildType;
    List<EmailItemUI>   m_EmailList = new List<EmailItemUI>();
   // ReadEmailUI         m_EmailPlayerWnd ;
    ReadEmailUI         m_EmailSysWnd = new ReadEmailUI();
    uint                m_CurMail;          //当前读取的邮件ID

    public void Init(Transform parent)
    {
        m_BaseWndObject = parent.GetChild(4).gameObject;
        m_BaseTrans = m_BaseWndObject.transform;

        m_EmailItemObj = m_BaseTrans.GetChild(2).GetChild(2).GetChild(0).gameObject;
        m_Grid = m_BaseTrans.GetChild(2).GetChild(2).GetChild(1).GetComponent<UIGrid>();
        m_ScrollView = m_BaseTrans.GetChild(2).GetChild(2).GetComponent<UIScrollView>();
        UIEventListener.Get(m_BaseTrans.GetChild(2).GetChild(0).gameObject).onClick = OnBatchDelete;

        for (int i = 0; i < 2; i++)
        {
            m_BtnObj[i] = m_BaseTrans.GetChild(i).GetComponent<UIButton>();
            m_ChildObj[i] = m_BaseTrans.GetChild(i + 2).gameObject;
            m_WarrningObj[i] = m_BaseTrans.GetChild(i).GetChild(1).gameObject;
            switch (i)
            {
                case 0:      //邮件
                    UIEventListener.Get(m_BtnObj[i].gameObject).onClick = OnMailMsg;
                    break;
                case 1:   //玩家赠礼
                    UIEventListener.Get(m_BtnObj[i].gameObject).onClick = OnGiftMsg;
                    break;
            }

        }
        m_EmailNum = m_BaseTrans.GetChild(2).GetChild(1).GetComponent<UILabel>();
        //InitReadEmailWndUI();


        m_ChildType = EmailChildType.EmailList_Wnd;

    }
    void InitNormalTitleItem(tagNormalMailTitle NormalMailTitle)
    {
        EmailItemUI item = null;
        item = new EmailItemUI(NormalMailTitle, null);
        item.Init(m_EmailItemObj);
        m_EmailList.Add(item);
    }
    void InitSysTitleItem(tagSystemMailTitle SysMailTitle)
    {
        EmailItemUI item = null;
        item = new EmailItemUI( null, SysMailTitle);
        item.Init(m_EmailItemObj);
        m_EmailList.Add(item);
    }
    public void Update(float delta)
    {
        switch (m_ChildType)
        {
            case EmailChildType.EmailList_Wnd:
                {
                    break;
                }
            case EmailChildType.Gift_Wnd:
                break;
        }
    }
    public void UpdateWarrningSate()
    {
        m_WarrningObj[0].SetActive(PlayerRole.Instance.RoleStatesMessage.GetMailStates());
        m_WarrningObj[1].SetActive(PlayerRole.Instance.RoleStatesMessage.GetGiffStates());
    }
    public void UpdateEmailListDate()
    {
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (m_ChildObj[0].activeSelf != true)
            return;

        ClearEmailGridChild();
        m_EmailList.Clear();
        List<tagNormalMailTitle> pNormalList = null;
        List<tagSystemMailTitle> pSystemList = null;
        PlayerRole.Instance.MailManager.GetAllUserMail(ref pNormalList, ref pSystemList);
        if (pNormalList != null)
        {
            for (byte i = 0; i < pNormalList.Count; ++i)
            {
                InitNormalTitleItem(pNormalList[i]);
            }
        }
        if (pSystemList != null)
        {
            for (byte j = 0; j < pSystemList.Count; ++j)
            {
                InitSysTitleItem(pSystemList[j]);

            }
        }

        SortEmailList();
        SetEamilListDate();

    }
    public void DeleteEmailListDate(uint mailID)
    {
        int lastID = 0;
        for (int i = 0; i < m_EmailList.Count; )
        {
            if (m_EmailList[i].MailItmeInfo.MailID == mailID)
            {
                EmailItemUI item = m_EmailList[i];
                m_Grid.RemoveChild(item.m_BaseTrans);
                item.ShutDown();
                Utility.ListRemoveAt(m_EmailList, i);
                lastID = i;
                break;
            }
            else
                ++i;
        }
        if (m_EmailList.Count > FishConfig.Instance.m_MailConfig.ClientShowMailSum)
            m_EmailNum.text = string.Format("邮件数量：{0}/{1}", m_EmailList.Count, m_EmailList.Count);
        else
            m_EmailNum.text = string.Format("邮件数量：{0}/{1}", m_EmailList.Count, FishConfig.Instance.m_MailConfig.ClientShowMailSum);

    }
    public void SetEamilListDate()
    {
        //m_Grid.transform.DetachChildren();
        for (byte i = 0; i < m_EmailList.Count; ++i)
        {
            m_Grid.AddChild(m_EmailList[i].m_BaseTrans, false);
            m_EmailList[i].ResetLocalScale();
        }
        if (m_EmailList.Count > FishConfig.Instance.m_MailConfig.ClientShowMailSum)
            m_EmailNum.text = string.Format("邮件数量：{0}/{1}", m_EmailList.Count, m_EmailList.Count);
        else
            m_EmailNum.text = string.Format("邮件数量：{0}/{1}", m_EmailList.Count, FishConfig.Instance.m_MailConfig.ClientShowMailSum);

    }

    public void SetMailHeader(uint mailID, Texture2D tex)
    {
        

    }
    public void ShowWnd(bool bShow)
    {
        if (m_BaseWndObject.activeSelf != bShow)
            m_BaseWndObject.SetActive(bShow);

        //显示时回到初始状态
        m_ChildType = EmailChildType.EmailList_Wnd;
        SetChildObjActive();
        UpdateEmailListDate();
        UpdateWarrningSate();
    }
    public void SendReadMail(uint mailID, byte type)
    {
        if (type == (byte)EmailType.Sys_Email)
        {
            if (PlayerRole.Instance.MailManager.GetSystemMailContext(mailID) == null)
                return;
            else
            {
                //ShowChildWnd(m_ChildType, false);
               // m_ChildType = EmailChildTyp.EmailSys_Wnd;
               // ShowChildWnd(m_ChildType, true, mailID);
                m_EmailSysWnd.ShowReadWndUI(true, mailID);
                m_CurMail = mailID;
            }
        }
        else if (type == (byte)EmailType.Normal_Email)
        {
            if (PlayerRole.Instance.MailManager.GetNormalMailContext(mailID) == null)
                return;
            else
            {
                //ShowChildWnd(m_ChildType, false);
                //m_ChildType = EmailChildTyp.EmailPlayer_Wnd;
                //ShowChildWnd(m_ChildType, true, mailID);
                m_CurMail = mailID;
            }
        }
    }
    public void UpdateReadMail(uint mailID)
    {
         if (PlayerRole.Instance.MailManager.IsNormalMail(mailID))
         {
             //ShowChildWnd(m_ChildType, false);
             //m_ChildType = EmailChildTyp.EmailPlayer_Wnd;
             //ShowChildWnd(m_ChildType, true, mailID);
             m_CurMail = mailID;
         }
         else if (PlayerRole.Instance.MailManager.IsSystemMail(mailID))
         {
             //ShowChildWnd(m_ChildType, false);
             //m_ChildType = EmailChildTyp.EmailSys_Wnd;
             //ShowChildWnd(m_ChildType, true, mailID);
             m_EmailSysWnd.ShowReadWndUI(true, mailID);
             m_CurMail = mailID;
         }
    }
    void OnMailMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_ChildType = EmailChildType.EmailList_Wnd;
        SetChildObjActive();
        UpdateEmailListDate();
    }
    void OnGiftMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_ChildType = EmailChildType.Gift_Wnd;
        SetChildObjActive();
        FriendRuntime.PresentUI.ShowWnd(true);
    }
    //邮件批量删除功能
    void OnBatchDelete(GameObject go)
    {

    }
    //领取物品后更新列表中带附件状态
   public void UpdatebSysMailItemState(uint mailID)
    {
        if (m_EmailSysWnd.m_BaseWndObject.activeSelf != true)
            return;
        m_EmailSysWnd.UpdateReceiveState();
        for (byte i = 0; i < m_EmailList.Count; ++i)
        {
            if (m_EmailList[i].MailItmeInfo.MailID == mailID)
            {
                m_EmailList[i].UpdateMailAttachmentState(false);
            }
        }
    }
    void SortEmailList()
    {
        m_EmailList.Sort(CompareDinsByItem);
    }
    static int CompareDinsByItem(EmailItemUI item1, EmailItemUI item2)
    {
        //优先级第一的未读和已经读
        if (!item1.MailItmeInfo.bIsRead && item2.MailItmeInfo.bIsRead)
            return -1;
        else if (item1.MailItmeInfo.bIsRead && !item2.MailItmeInfo.bIsRead)
            return 1;
        else //(!item1.MailItmeInfo.bIsRead && !item2.MailItmeInfo.bIsRead)    //都未读 或已经读
        {
            //是否带附件
            if (item1.MailItmeInfo.bIsExistsItem && !item2.MailItmeInfo.bIsExistsItem)
                return -1;
            else if (!item1.MailItmeInfo.bIsExistsItem && item2.MailItmeInfo.bIsExistsItem)
                return 1;
            else
            {
                if (item1.MailItmeInfo.bType > item2.MailItmeInfo.bType)
                    return -1;
                else if (item1.MailItmeInfo.bType < item2.MailItmeInfo.bType)
                    return 1;
                else
                {
                    if (item1.MailItmeInfo.bDiffTime > item2.MailItmeInfo.bDiffTime)
                        return -1;
                    else if (item1.MailItmeInfo.bDiffTime < item2.MailItmeInfo.bDiffTime)
                        return 1;
                    else
                        return 0;
                }
            }
        }
    }
    void ClearEmailGridChild()
    {
        List<Transform> gridChid = m_Grid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_Grid.transform.DetachChildren();
    }
    void SetChildObjActive()
    {
        if (m_ChildType == EmailChildType.EmailList_Wnd)
        {
            m_ChildObj[0].SetActive(true);
            m_ChildObj[1].SetActive(false);
            ChangeButtonSprite(0, "Sys_Mail_Btn02");
            ChangeButtonSprite(1, "Friend_Gig_Btn01");
        }
        else
        {
            m_ChildObj[1].SetActive(true);
            m_ChildObj[0].SetActive(false);
            ChangeButtonSprite(0, "Sys_Mail_Btn01");
            ChangeButtonSprite(1, "Friend_Gig_Btn02");
        }
    }
    void ChangeButtonSprite(byte Idx, string str)
    {
        m_BtnObj[Idx].pressedSprite = str;
        m_BtnObj[Idx].normalSprite = str;
        m_BtnObj[Idx].hoverSprite = str;
    }

    public void ShutDown()
    {
        m_EmailList.Clear();
        ClearEmailGridChild();
    }
}
