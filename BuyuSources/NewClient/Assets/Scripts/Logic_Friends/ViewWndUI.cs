using UnityEngine;
using System.Collections.Generic;

public enum ItemType
{
    Search_Item,
    Blacklist_Item
}

public class ViewWndUI : BaseWnd
{
    GameObject              m_BtnObj;
    UIButton                m_UIButton;
    UILabel                 m_LevelLabel;
    UILabel                 m_NickName;
    UILabel                 m_Address;
    UISprite                m_Gender;
    UITexture               m_FaceTexture;
    uint                    m_PlayerID;
    ItemType                m_ItemType;
    QueryRole               m_QueryInfo;
    RelationRole            m_BlacklistInfo;

    public uint PlayerID
    {
        get { return m_PlayerID; }
    }
    public ViewWndUI(ItemType type, QueryRole ItemInfo, RelationRole ItemInfo1)
    {
        m_ItemType = type;
        m_QueryInfo = ItemInfo;
        m_BlacklistInfo = ItemInfo1;
    }

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_BtnObj = m_BaseTrans.GetChild(0).gameObject;
        m_UIButton = m_BtnObj.GetComponent<UIButton>();
        UIEventListener.Get(m_BtnObj).onClick = OnButtonMsg;

        m_FaceTexture = m_BaseTrans.GetChild(1).GetChild(0).GetComponent<UITexture>();
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = OnClickAvatarMsg;

        m_LevelLabel = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        m_NickName = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
        m_Address = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        m_Gender = m_BaseTrans.GetChild(5).GetComponent<UISprite>();
        ShowItemInfo();
    }
    public void ResetLocalScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void SetHeader(Texture2D tex)
    {
        if (tex == null)
            m_FaceTexture.mainTexture = tex;
        else
            m_FaceTexture.mainTexture = tex;
    }
    void ShowItemInfo()
    {
        if (m_QueryInfo != null)
        {
            m_PlayerID = m_QueryInfo.GetUserID();
            m_NickName.text = m_QueryInfo.GetNickName();
            m_LevelLabel.text = string.Format("Lv." + m_QueryInfo.GetLevel().ToString());
            m_Address.text = m_QueryInfo.GetIPAddress(); ;
            ShowQueryGender();
            //显示头像
            if (m_QueryInfo.GetFaceID() < ConstValue.CUSTOM_HEADER)
                m_FaceTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[m_QueryInfo.GetFaceID()];
            else
                HeaderManager.Instance.GetPlayerHeader(m_QueryInfo.GetUserID(), m_QueryInfo.GetFaceID(), HeaderOptType.HEADER_FIND, null);
           
        }
        else if (m_BlacklistInfo != null)
        {
            m_PlayerID = m_BlacklistInfo.GetUserID();
            m_NickName.text = m_BlacklistInfo.GetNickName();
            m_LevelLabel.text = string.Format("Lv." + m_BlacklistInfo.GetLevel().ToString());
            m_Address.text = m_BlacklistInfo.GetIPAddress();
            ShowBlacklistGender();
            //显示头像
            if (m_BlacklistInfo.GetFaceID() < ConstValue.CUSTOM_HEADER)
                m_FaceTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[m_BlacklistInfo.GetFaceID()];
            else
                HeaderManager.Instance.GetPlayerHeader(m_BlacklistInfo.GetUserID(), m_BlacklistInfo.GetFaceID(), HeaderOptType.HEADER_BLACK, null);
        }
        

        //如果该玩家已经是好友了则添加好友按纽变灰
        if (PlayerRole.Instance.RelationManager.GetFriendMap() != null && 
            PlayerRole.Instance.RelationManager.GetFriendMap().ContainsKey(m_PlayerID) == true)
        {
            m_UIButton.isEnabled = false;
        }
    }
    void ShowQueryGender()
    {
        if (m_QueryInfo.IsOnline() != true)
        {
            m_Gender.spriteName = "offline";
            return;
        }

        if (!m_QueryInfo.GetGender())
            m_Gender.spriteName = "male";
        else
            m_Gender.spriteName = "female";
    }
    void ShowBlacklistGender()
    {
        if (m_BlacklistInfo.IsOnline() != true)
        {
            m_Gender.spriteName = "offline";
            return;
        }
        if (!m_BlacklistInfo.GetGender())
            m_Gender.spriteName = "male";
        else
            m_Gender.spriteName = "female";
    }
    void OnButtonMsg(GameObject go)
    {
        if (m_ItemType == ItemType.Search_Item)
        {
            if (PlayerRole.Instance.RelationManager.GetFriendMap().ContainsKey(m_PlayerID) == true)
            {
                //以后再处理弹出提示窗口消息
                GlobalHallUIMgr.Instance.ShowSystemTipsUI("他已经是你好友了", 1, false);
                return;
            }
            FriendRuntime.ApplyCodeUI.ShowApplyCodeWnd(m_PlayerID);
            //PlayerRole.Instance.RelationManager.AddOtherRoleRelation(m_PlayerID, 1);
        }
        else if (m_ItemType == ItemType.Blacklist_Item)
        {
           // RelationMsg.Instance.SendDelUserRelation(m_PlayerID);
            PlayerRole.Instance.RelationManager.DelRelation(m_PlayerID);
        }

      //  m_UIButton.isEnabled = false;
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void OnClickAvatarMsg(GameObject go)
    {
        Debug.Log("click avatar");
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
}
