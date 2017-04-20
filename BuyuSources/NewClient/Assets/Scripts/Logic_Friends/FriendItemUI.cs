using UnityEngine;
using System.Collections.Generic;


public enum ItemStae
{
    STATE_NORMAL,
    STATE_DELETE
}

public class FriendItemUI  : BaseWnd
{
    GameObject          m_NewMsgObj;
    UILabel             m_NickName;
    UILabel             m_Level;
    UILabel             m_OnLineLast;
    UILabel             m_IPAddress;
    UISprite            m_GenderSprite;
    UISprite            m_VipIcon;
    UIButton            m_UIPresentBtn;
    UITexture           m_FaceTexture;
    RelationRole        m_FriendItemInfo;
    uint                m_PlayerID;
    bool                m_bNewMsg;


    public FriendItemUI(ItemStae mode, uint PlayerID, RelationRole tagFriend)
    {
        m_bNewMsg = false;
        m_PlayerID = PlayerID;
        m_FriendItemInfo = tagFriend;
    }

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_NickName = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
        m_Level = m_BaseTrans.GetChild(5).GetComponent<UILabel>();
        m_OnLineLast = m_BaseTrans.GetChild(6).GetComponent<UILabel>();
        m_IPAddress = m_BaseTrans.GetChild(8).GetComponent<UILabel>();
        m_GenderSprite = m_BaseTrans.GetChild(7).GetComponent<UISprite>();
        m_FaceTexture = m_BaseTrans.GetChild(4).GetChild(0).GetComponent<UITexture>();
        m_NewMsgObj = m_BaseTrans.GetChild(9).gameObject;
        m_VipIcon = m_BaseTrans.GetChild(10).GetComponent<UISprite>();
        UIEventListener.Get(m_BaseTrans.GetChild(4).gameObject).onClick = OnClickAvatarMsg;


        for (int i = 0; i < 3; i++)
        {

            switch (i)
            {
                case 0:     //私信按纽  
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnSendMsg;
                    break;
                case 1:     //赠送按纽
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnPresentMsg;
                    m_UIPresentBtn = m_BaseTrans.GetChild(i).GetComponent<UIButton>();
                    break;
                case 2:     //删除按纽
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnDeleteMsg;
                    break;
            }
        }


        ShowListItemInfo();
    }
    public void RestLocalScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
	
	public void Update (float deltaTime) 
    {
        
	}
    void ShowListItemInfo()
    {
        //头像显示
        if (m_FriendItemInfo.GetFaceID() < ConstValue.CUSTOM_HEADER)
            m_FaceTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[m_FriendItemInfo.GetFaceID()];
        else 
            HeaderManager.Instance.GetPlayerHeader(m_FriendItemInfo.GetUserID(), m_FriendItemInfo.GetFaceID(), HeaderOptType.HEADER_FRIEND, 
                (byte)WndType.Friend_Wnd);

        m_NickName.text = m_FriendItemInfo.GetNickName();
        m_Level.text = m_FriendItemInfo.GetLevel().ToString();
        
        if (m_FriendItemInfo.GetLeaveOnlineDay() == 255)
            m_OnLineLast.text = "在线";
        else
        {
            m_OnLineLast.text = "离线";
        }

        if (m_FriendItemInfo.GetLeaveOnlineDay() != 255)
        {
            m_GenderSprite.spriteName = "offline";
        }
        //性别
        else if (!m_FriendItemInfo.GetGender())
            m_GenderSprite.spriteName = "male";
        else
            m_GenderSprite.spriteName = "female";

        //IP地址
        m_IPAddress.text = m_FriendItemInfo.GetIPAddress();

        //VIP
        if (m_FriendItemInfo.GetVipLevel() > 0)
        {
            m_VipIcon.gameObject.SetActive(true);
            m_VipIcon.spriteName = string.Format("Vip_Icon_0{0}", m_FriendItemInfo.GetVipLevel());
        }
        
        //检查该好友是否已经送赠送过了
        if (PlayerRole.Instance.GiffManager.GetNowDaySendGiffInfo() != null)
        {
            if (PlayerRole.Instance.GiffManager.GetNowDaySendGiffInfo().ContainsKey(m_FriendItemInfo.GetUserID()))
                SetPressentBtn();
                //m_UIPresentBtn.enabled = false;
        }
        //检查是否有新消息
        if (PlayerRole.Instance.RoleChar.GetCharList() != null)
        {
            if (PlayerRole.Instance.RoleChar.GetCharList().ContainsKey(m_FriendItemInfo.GetUserID()))
            {
                SetNewMsgSate();
            }
        }

    }
    public void SetMyHeader(Texture2D tex)
    {
        if (tex == null)
            m_FaceTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        else
            m_FaceTexture.mainTexture = tex;
    }
    public void SetPressentBtn()
    {
        m_UIPresentBtn.normalSprite = "Send_Gift_Btn1";
        m_UIPresentBtn.disabledSprite = "Send_Gift_Btn1";
        m_UIPresentBtn.isEnabled = false;
    }
    public void SetNewMsgSate(bool bSate = true)
    {
        m_bNewMsg = true;
        m_NewMsgObj.SetActive(true);
    }
    //点击头像
    void OnClickAvatarMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Friend_Head, m_PlayerID, (Texture2D)m_FaceTexture.mainTexture, false, true);
    }
    //发送私信消息
    void OnSendMsg(GameObject go)
    {

        FriendRuntime.SendMsgUI.ShowWnd(m_PlayerID, m_FriendItemInfo.GetNickName(), (Texture2D)m_FaceTexture.mainTexture, m_FriendItemInfo.GetGender());
        
        //FriendRuntime.FriendSysMgr.SetWndMask(false);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_NewMsgObj.SetActive(false);
    }
    //赠礼按纽消息
    void OnPresentMsg(GameObject go)
    {
       
        PlayerRole.Instance.GiffManager.SendGiffToTargetUser(m_FriendItemInfo.GetUserID());
       // m_UIPresentBtn.enabled = false;
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    //删除好友按纽消息
    void OnDeleteMsg(GameObject go)
    {
        GlobalEffectMgr.Instance.ShowMessageBox(string.Format(StringTable.GetString("Del_Friend_Tips"), m_FriendItemInfo.GetNickName()), MssageType.Del_Friend_Msg,
            0, m_FriendItemInfo.GetUserID());
        //PlayerRole.Instance.RelationManager.DelRelation(m_FriendItemInfo.GetUserID());
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }


    public RelationRole FriendItemInfo
    {
        get { return m_FriendItemInfo; }
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
            GameObject.Destroy(m_BaseWndObject);
    }
}
