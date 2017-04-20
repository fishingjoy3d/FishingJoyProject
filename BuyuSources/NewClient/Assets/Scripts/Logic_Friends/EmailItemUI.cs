using UnityEngine;
using System.Collections.Generic;

public enum EmailType
{
    Normal_Email,
    Sys_Email
}

//邮件背景条
public struct EmailBgType
{
    public UISprite m_BgTypeSprite;
    public GameObject m_BgObj;
}

public struct tagMailTitle
{
    public uint     MailID;
    public uint     SrcUserID;          //发送方的ID
    public uint     SrcFaceID;
    public string   SrcNickName;
    public byte     bDiffTime;          //在线天数差
    public byte     bType;              //玩家邮件为1、系统邮件为2
    public bool     bIsRead;            //true已经阅读 fasle 未读
    public bool     bIsExistsItem;      //是否存在附件
}

public class EmailItemUI : BaseWnd
{

    UISprite            m_StateSprite;
    UITexture           m_FaceTextrue;
    UILabel             m_TitleLable;
    UILabel             m_VaildTime;
    tagMailTitle        m_MailItmeInfo;
    public EmailItemUI(tagNormalMailTitle NormalMailTitle, tagSystemMailTitle SysMailTitle)
    {

        if (NormalMailTitle != null)
        {
            m_MailItmeInfo.MailID = NormalMailTitle.MailID;
            m_MailItmeInfo.bType = (byte)EmailType.Normal_Email;
            m_MailItmeInfo.SrcFaceID = NormalMailTitle.SrcFaceID;
            m_MailItmeInfo.SrcNickName = NormalMailTitle.SrcNickName;
            m_MailItmeInfo.SrcUserID = NormalMailTitle.SrcUserID;
            m_MailItmeInfo.bIsRead = NormalMailTitle.bIsRead;
            m_MailItmeInfo.bDiffTime = NormalMailTitle.bDiffTime;
        }
        else
        {
            m_MailItmeInfo.bType = (byte)EmailType.Sys_Email;
            m_MailItmeInfo.bDiffTime = SysMailTitle.bDiffTime;
            m_MailItmeInfo.bIsExistsItem = SysMailTitle.bIsExistsItem;
            m_MailItmeInfo.bIsRead = SysMailTitle.bIsRead;
            m_MailItmeInfo.MailID = SysMailTitle.MailID;
        }
    }
    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);
        m_StateSprite = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        UIEventListener.Get(m_BaseWndObject).onClick = OnClickMsg;


        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = OnDeleteClickMsg;


 
        m_TitleLable = m_BaseTrans.GetChild(3).gameObject.GetComponent<UILabel>();
        m_VaildTime = m_BaseTrans.GetChild(4).gameObject.GetComponent<UILabel>();
        m_FaceTextrue = m_BaseTrans.GetChild(2).GetChild(0).GetComponent<UITexture>();


        if (m_MailItmeInfo.bType == (byte)EmailType.Normal_Email)
        {
            if (m_MailItmeInfo.SrcFaceID < ConstValue.CUSTOM_HEADER)
                m_FaceTextrue.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[m_MailItmeInfo.SrcFaceID];
            else
                HeaderManager.Instance.GetPlayerHeader(m_MailItmeInfo.SrcUserID, m_MailItmeInfo.SrcFaceID, HeaderOptType.HEADER_FRIEND, (byte)WndType.Emanil_Wnd);
            UIEventListener.Get(m_BaseTrans.GetChild(3).gameObject).onClick = OnClickAvatarMsg;
            m_TitleLable.text = m_MailItmeInfo.SrcNickName;
        }
        else
        {
            m_FaceTextrue.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[6];
            m_TitleLable.text = "系统邮件";
        }

        if (m_MailItmeInfo.bDiffTime < 7)
            m_VaildTime.text = string.Format("剩余时间：{0}天", (7 - m_MailItmeInfo.bDiffTime));
        else
            m_VaildTime.text = "一个星期以前";

        UpdateState(m_MailItmeInfo.bIsRead);
    }
    public void SetMyHeader(Texture2D tex)
    {
        if (tex == null)
            m_FaceTextrue.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        else
            m_FaceTextrue.mainTexture = tex;
    }
    public void ResetLocalScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void Update(float delta)
    {
      
    }
  
    public void UpdateState(bool state)
    {
        int read = state ? 1 : 0;
        m_StateSprite.spriteName = string.Format("state{0}", read);
        m_MailItmeInfo.bIsRead = state;
    }
    //更新邮件携带附件状态
    public void UpdateMailAttachmentState(bool state)
    {
        m_MailItmeInfo.bIsExistsItem = state;
    }

    //点击玩家头像
    void OnClickAvatarMsg(GameObject go)
    {
        if (PlayerRole.Instance.RelationManager.GetFriendMap().ContainsKey(m_MailItmeInfo.SrcUserID))
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Friend_Head, m_MailItmeInfo.SrcUserID,
                (Texture2D)m_FaceTextrue.mainTexture, false, true);
        }
        else
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Other_Head, m_MailItmeInfo.SrcUserID,
                (Texture2D)m_FaceTextrue.mainTexture, false, false);
            PlayerRole.Instance.QueryManager.QueryUserByUserID(m_MailItmeInfo.SrcUserID);
        }

        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    //删除邮件
    void OnDeleteClickMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        //如果玩家还没领取邮件物品则不让其删除邮件
        if (m_MailItmeInfo.bIsExistsItem)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Mail_Del_Field1"), 1.5f);
            //m_bPressDel = false;
            //ChangSysMode(false, SysMode.SYS_NORMAL);
            return;
        }
        PlayerRole.Instance.MailManager.DelMail(m_MailItmeInfo.MailID);

    }

    //发送读取邮件请求消息
    void OnClickMsg(GameObject go)
    {
        //if (!FriendRuntime.FriendSysMgr.bMask)
        //    return;

        FriendRuntime.EmailUI.SendReadMail(m_MailItmeInfo.MailID, m_MailItmeInfo.bType);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
            GameObject.Destroy(m_BaseWndObject);
    }
    public tagMailTitle MailItmeInfo
    {
        get { return m_MailItmeInfo; }
        set { m_MailItmeInfo = value; }
    }
}
