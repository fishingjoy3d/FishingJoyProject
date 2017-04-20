using UnityEngine;
using System.Collections.Generic;

public class ApplyItemUI  : BaseWnd
{

    UILabel         m_NickName;
    UILabel         m_Level;
    UILabel         m_Message;
    UISprite        m_GenderSprite;
    UITexture       m_FaceTexture;
    UILabel         m_IPAddress;
    UISprite        m_VipIcon;
    uint            m_UserID;
    uint            m_ID;

    public uint UserID
    {
        get { return m_UserID; }
    }
    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_NickName = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        m_Level = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        m_Message = m_BaseTrans.GetChild(5).GetComponent<UILabel>();
        m_IPAddress = m_BaseTrans.GetChild(7).GetComponent<UILabel>();
        m_GenderSprite = m_BaseTrans.GetChild(6).GetComponent<UISprite>();
        m_FaceTexture = m_BaseTrans.GetChild(3).GetChild(0).GetComponent<UITexture>();
        m_VipIcon = m_BaseTrans.GetChild(8).GetComponent<UISprite>();

        UIEventListener.Get(m_BaseTrans.GetChild(3).gameObject).onClick = OnClickAvatarMsg;


        for (int i = 0; i < 2; i++)
        {

            switch (i)
            {
                case 0:     //同意申请
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnAgreeApplyMsg;
                    break;
                case 1:     //拒绝申请
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnRefuseApplyMsg;
                    break;
            }
        }
    }
    public void ResetScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void ShowApplayInfo(tagRelationRequest pInfo)
    {
        m_ID = pInfo.ID;
        m_UserID = pInfo.SrcUserID;
        //头像显示
        if (pInfo.SrcFaceID < ConstValue.CUSTOM_HEADER)
            m_FaceTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[pInfo.SrcFaceID];
        else
            HeaderManager.Instance.GetPlayerHeader(pInfo.SrcUserID, pInfo.SrcFaceID, HeaderOptType.HEADER_FRIEND,
                (byte)WndType.Friend_Wnd);

        m_NickName.text = pInfo.SrcNickName;
        m_IPAddress.text = pInfo.SrcIPAddress;
        
        m_Level.text = pInfo.SrcLevel.ToString();
        if (!pInfo.SrcGender)
            m_GenderSprite.spriteName = "male";
        else
            m_GenderSprite.spriteName = "female";

        m_Message.text = "留言:"+pInfo.MessageInfo;
    }
    public void SetMyHeader(Texture2D tex)
    {
        if (tex == null)
            m_FaceTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        else
            m_FaceTexture.mainTexture = tex;
    }
    void OnAgreeApplyMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        PlayerRole.Instance.RoleRelationRequest.HandleRelationRequest(m_ID, true);
    }
    void OnRefuseApplyMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        PlayerRole.Instance.RoleRelationRequest.HandleRelationRequest(m_ID, false);

    }
    void OnClickAvatarMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Other_Head, m_UserID, (Texture2D)m_FaceTexture.mainTexture);
    }

}
