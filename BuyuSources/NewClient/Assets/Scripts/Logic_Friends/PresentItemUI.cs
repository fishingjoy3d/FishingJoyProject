using UnityEngine;
using System.Collections.Generic;

public class PresentItemUI : BaseWnd
{

    GameObject          m_PresentBtn;
    UITexture           m_FaceTexture;
    UILabel             m_NickName;
    UILabel             m_VaildTime;
    public tagGiffClientInfo   m_PresentItemInfo;

    public PresentItemUI(tagGiffClientInfo tagItemInfo)
    {
        m_PresentItemInfo = tagItemInfo;
    }

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);
        m_PresentBtn = m_BaseTrans.GetChild(0).gameObject;
        UIEventListener.Get(m_PresentBtn).onClick = OnReceivePresentMsg;

        m_FaceTexture = m_BaseTrans.GetChild(1).GetChild(0).GetComponent<UITexture>();
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = OnClickAvatarMsg;

        m_NickName = m_BaseTrans.GetChild(2).gameObject.GetComponent<UILabel>();
        m_VaildTime = m_BaseTrans.GetChild(3).gameObject.GetComponent<UILabel>();
        ShowItemInfo();
    }
    public void ResetLocalScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void ShowItemInfo()
    {
        if (m_PresentItemInfo.dwFaceID < ConstValue.CUSTOM_HEADER)
            m_FaceTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[m_PresentItemInfo.dwFaceID];
        else
            HeaderManager.Instance.GetPlayerHeader(m_PresentItemInfo.dwUserID, m_PresentItemInfo.dwFaceID, HeaderOptType.HEADER_FRIEND, (byte)WndType.Present_Wnd);

        m_NickName.text = string.Format("[f8ea28]{0}[-]  赠送了您", m_PresentItemInfo.NickName);
        int time = 7 - m_PresentItemInfo.DiffTime;
        if (time < 0)
            time = 0;
        m_VaildTime.text = string.Format("剩余时间：{0}天", time);

    }
    public void SetHeader(Texture2D tex)
    {
        if (tex == null)
            m_FaceTexture.mainTexture = tex;
        else
            m_FaceTexture.mainTexture = tex;

    }
    void OnReceivePresentMsg(GameObject go)
    {
        PlayerRole.Instance.GiffManager.AcceptGiff(m_PresentItemInfo.OnlyID);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void OnClickAvatarMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (PlayerRole.Instance.RelationManager.GetFriendMap().ContainsKey(m_PresentItemInfo.dwUserID))
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Friend_Head, m_PresentItemInfo.dwUserID,
                (Texture2D)m_FaceTexture.mainTexture, false, true);
        }
        else
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Other_Head, m_PresentItemInfo.dwUserID,
                (Texture2D)m_FaceTexture.mainTexture, false, false);
            PlayerRole.Instance.QueryManager.QueryUserByUserID(m_PresentItemInfo.dwUserID);
        }

    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
            GameObject.Destroy(m_BaseWndObject);
    }
    public tagGiffClientInfo PresentItemInfo
    {
        get { return m_PresentItemInfo; }
        set { m_PresentItemInfo = value; }
    }
}
