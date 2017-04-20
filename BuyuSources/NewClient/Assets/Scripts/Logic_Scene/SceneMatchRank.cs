using UnityEngine;
using System.Collections.Generic;

public class SceneMatchRank : BaseWnd
{
    Object                  m_Obj = null;
    UILabel                 m_TitleLabel;
    UILabel                 m_TipsLabel;
    SceneMatchRankItem[]    m_RankItem = new SceneMatchRankItem[5];

	public void InitObj()
    {
        m_Obj = ResManager.Instance.LoadObject(("Scene_Match_Rank"), "SceneRes/Prefab/UI/MatchRank/", ResType.SceneRes);
    }
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(m_Obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        m_TitleLabel = m_BaseTrans.GetChild(0).GetComponent<UILabel>();
        m_TipsLabel = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        UIEventListener.Get(m_BaseTrans.GetChild(2).gameObject).onClick = OnCloseWnd;
        for (byte i = 0; i < m_RankItem.Length; ++i)
        {
            SceneMatchRankItem item = new SceneMatchRankItem();
            item.m_BaseWndObject = m_BaseTrans.GetChild(i + 3).gameObject;
            item.m_BaseTrans = m_BaseTrans.GetChild(i + 3);
            item.Init();
            m_RankItem[i] = item;
        }
    }
    public void ShowMatchRankWnd( )
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseTrans);
        }
        UpdateCurMatchRank();
        if (FishConfig.Instance.m_MatchInfo.m_MonthStr.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()))
            m_TitleLabel.text = FishConfig.Instance.m_MatchInfo.m_MonthStr[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].MonthName +"元话费赛";
        m_TipsLabel.text = StringTable.GetString("Match_Scene_Rank_Tips");
    }
    public void UpdateCurMatchRank()
    {
        if (m_BaseWndObject == null)
            return;
        m_RankItem[0].SetMyInfo();
        tagMonthRoleInfo[] pMatchRankInfo = PlayerRole.Instance.MonthManager.GetMonthList();
        if (pMatchRankInfo != null)
        {
            for (byte i = 0; i < pMatchRankInfo.Length; ++i)
            {
                if (pMatchRankInfo[i].dwUserID == 0)
                    return;
                m_RankItem[i + 1].SetInfo(pMatchRankInfo[i]);
            }
        }
    }
    public void SetMySelfHeader(Texture2D tex)
    {
        if (m_BaseWndObject == null)
            return;
        m_RankItem[0].SetHeader(tex);
    }
    public void SetSceneMatchRankHeader(Texture2D tex, uint UserID)
    {
        for (byte i = 1; i < m_RankItem.Length; ++i)
        {
            if (m_RankItem[i].m_UserID == UserID)
            {
                m_RankItem[i].SetHeader(tex);
            }
        }
    }
    void OnCloseWnd(GameObject go)
    {
        DestorySelf();
    }
    void DestorySelf()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseTrans);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
    public void ShutDown()
    {
        DestorySelf();
        ResManager.Instance.UnloadObject(m_Obj);
    }
}

class SceneMatchRankItem : BaseWnd
{
    UILabel         m_RankLabel;
    UILabel         m_NameLabel;
    UILabel         m_ScoreLabel;
    UISprite        m_Crow;
    UITexture       m_HeadTexture;

    public uint     m_UserID;
    bool            m_bMySelf;

    public void Init()
    {
        m_RankLabel = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        m_HeadTexture = m_BaseTrans.GetChild(2).GetComponent<UITexture>();
        m_NameLabel = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
        m_ScoreLabel = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        m_Crow = m_BaseTrans.GetChild(5).GetComponent<UISprite>();
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnClickNameCards;

    }
    public void SetInfo(tagMonthRoleInfo pRankInfo)
    {
        if (m_BaseWndObject.activeSelf == false)
            m_BaseWndObject.SetActive(true);
        if (pRankInfo.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            m_bMySelf = true;
        else
            m_bMySelf = false;

        if (pRankInfo.Index == 4)
            m_RankLabel.text = pRankInfo.Index.ToString();
        m_NameLabel.text = pRankInfo.NickName;
        m_ScoreLabel.text = string.Format("分数: {0}", pRankInfo.Score);
        m_UserID = pRankInfo.dwUserID;
        if (pRankInfo.FaceID < ConstValue.CUSTOM_HEADER)
            m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[pRankInfo.FaceID];
        else
            HeaderManager.Instance.GetPlayerHeader(pRankInfo.dwUserID, pRankInfo.FaceID, HeaderOptType.HEADER_MATCHRANK, null);

    }
    public void SetMyInfo()
    {
        m_bMySelf = true;
        uint ficeID = PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID();
        if (ficeID < ConstValue.CUSTOM_HEADER)
        {
            m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
        }
        else
        {
            //获取头像
            HeaderManager.Instance.GetPlayerHeader(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), ficeID, HeaderOptType.HEADER_ME, null);
        }
        m_NameLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();
        m_RankLabel.text = (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthIndex() + 1).ToString();
        m_ScoreLabel.text = string.Format("分数: {0}", PlayerRole.Instance.RoleInfo.RoleMe.GetMonthScore());
    }
    public void SetHeader(Texture2D tex)
    {
        if (tex == null)
            m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        else
            m_HeadTexture.mainTexture = tex;
    }
    void OnClickNameCards(GameObject go)
    {
        if (m_bMySelf)
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Other_Head, PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), (Texture2D)m_HeadTexture.mainTexture, true);
        }
        else
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Other_Head, m_UserID,
                (Texture2D)m_HeadTexture.mainTexture, false, false);
            PlayerRole.Instance.QueryManager.QueryUserByUserID(m_UserID);
        }
    }
}
