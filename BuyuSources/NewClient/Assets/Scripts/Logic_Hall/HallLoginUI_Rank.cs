using UnityEngine;
using System.Collections.Generic;

public enum RankType
{
    Level_Rank = 1,         //等级榜
    Gold_Rank,              //财富榜
    Achievement_Rank,       //成就榜
    Medal_Rank,             //奖牌榜
    Enchantment_Rank,       //魅力榜
    Rand_Max,
}
public struct RankNumber
{
     public GameObject m_BaseObj;
     public UILabel m_Label;
}

public class HallLoginUI_Rank : HallLoginUI_BaseWind
{
    WindBtn[]           m_RankLeftBtns = new WindBtn[5];
    GameObject[]        m_LeftWarning = new GameObject[5];
    UIGrid              m_Grid;
    GameObject          m_BaseChild;
    List<RankItemUI>    m_RankList = new List<RankItemUI>();
    MyRankItemUI        m_MyRankItem = new MyRankItemUI();
    RankType            m_RankType = RankType.Level_Rank;
    UIPanel             m_UIPanel;
    UIScrollView        m_UIScrollView;
    Transform           m_ScrollTrans;


    public void Init(GameObject go)
    {
        BaseTranF = go.transform;
        WindObj = go;
        Transform leftBaseBtn = BaseTranF.GetChild(0);
        for (byte i = 0; i < m_RankLeftBtns.Length; ++i)
        {
            m_RankLeftBtns[i].m_BtnObj = leftBaseBtn.GetChild(i).gameObject;
            m_RankLeftBtns[i].m_Btn = m_RankLeftBtns[i].m_BtnObj.GetComponent<UIButton>();
            if (i == 0)
                m_RankLeftBtns[i].m_IsChecked = true;
            else
                m_RankLeftBtns[i].m_IsChecked = false;
            m_RankLeftBtns[i].m_Tag = i;
            UIEventListener.Get(m_RankLeftBtns[i].m_BtnObj).onClick = OnCheckRankLeftBtns;

            m_LeftWarning[i] = leftBaseBtn.GetChild(i + 5).gameObject;
        }
        ExChangeRankLeftBtnsBg();
        m_Grid = BaseTranF.GetChild(1).GetChild(1).GetComponent<UIGrid>();
        m_UIPanel = BaseTranF.GetChild(1).GetComponent<UIPanel>();
        m_UIScrollView = BaseTranF.GetChild(1).GetComponent<UIScrollView>();
        m_ScrollTrans = BaseTranF.GetChild(1);
        //m_grid.transform.localPosition = new Vector3(-52f, 0f, 0f);
        m_BaseChild = BaseTranF.GetChild(1).GetChild(0).gameObject;

        InitMyRankItem();
    }
    void InitMyRankItem()
    {
        m_MyRankItem.m_Number = new RankNumber[5];
        m_MyRankItem.m_RankingLabel = new UILabel[2];
        for (byte j = 0; j < m_MyRankItem.m_RankingLabel.Length; ++j)
        {
           m_MyRankItem.m_RankingLabel[j]  = BaseTranF.GetChild(2).GetChild(0).GetChild(j).GetComponent<UILabel>();
        }
        m_MyRankItem.m_HeadTexture = BaseTranF.GetChild(2).GetChild(1).GetComponent<UITexture>();
        m_MyRankItem.m_HistoryRank = BaseTranF.GetChild(2).GetChild(2).GetComponent<UILabel>();
        m_MyRankItem.m_GetRewards = BaseTranF.GetChild(2).GetChild(4).GetComponent<UIButton>();
        UIEventListener.Get(m_MyRankItem.m_GetRewards.gameObject).onClick = OnClickGetRewards;
        for (byte i = 0; i < m_MyRankItem.m_Number.Length; ++i)
        {
            m_MyRankItem.m_Number[i].m_BaseObj = BaseTranF.GetChild(2).GetChild(3).GetChild(i).gameObject;
            m_MyRankItem.m_Number[i].m_Label = BaseTranF.GetChild(2).GetChild(3).GetChild(i).GetChild(0).GetComponent<UILabel>();
        }
    }
    public void UpdateRankContent()
    {
        if (WindObj == null || !WindObj.activeSelf)
            return;
        
        if (PlayerRole.Instance.RankManager.GetRankInfo() == null || PlayerRole.Instance.RelationManager.GetFriendMap() == null)
            return;
        m_RankList.Clear();
        ClearGird();
        if (PlayerRole.Instance.RankManager.GetRankInfo().ContainsKey((byte)m_RankType) == false)
            return;
        m_ScrollTrans.localPosition = new Vector3(64, 104, 0);
        m_UIPanel.clipOffset = new Vector2(55.27f, -150);
        m_UIScrollView.ResetPosition();
        foreach (KeyValuePair<uint, tagRankInfo> map in PlayerRole.Instance.RankManager.GetRankInfo()[(byte)m_RankType].RankMap)
        {
            RankItemUI rank = new RankItemUI(m_RankType, map.Value);
            rank.Init(m_BaseChild);
            m_RankList.Add(rank);
        }
        m_RankList.Sort(CompareItemByIndex);
        for (byte i = 0; i < m_RankList.Count; ++i)
        {
            m_Grid.AddChild(m_RankList[i].m_BaseTrans, false);
            m_RankList[i].ResetScale();
        }

    }
    public void UpdateRankFriendState(uint userID)
    {
        for (byte i = 0; i < m_RankList.Count; ++i)
        {
            if(userID == m_RankList[i].RankInfo.dwUserID)
            {
                m_RankList[i].UpdateRankFriendState();
                break;
            }
        }
    }
    public void UpdateMyRankContent()
    {
        if (!BaseTranF.gameObject.activeSelf)
            return;

        if (PlayerRole.Instance.RankManager.GetWeekRankInfo() == null)
            return;
        if (PlayerRole.Instance.RankManager.GetWeekRankInfo().ContainsKey((byte)m_RankType) == true)
        {
            tagRankWeekRankClient pWeekRankReward = PlayerRole.Instance.RankManager.GetWeekRankInfo()[(byte)m_RankType];
            m_MyRankItem.m_HistoryRank.text = "上周排名：" + pWeekRankReward.RankIndex.ToString();
            if (PlayerRole.Instance.RankManager.GetWeekRankInfo()[(byte)m_RankType].IsReward)
            {
                m_MyRankItem.m_GetRewards.isEnabled = false;
                m_MyRankItem.m_GetRewards.normalSprite = "Information_Rank_GetAward1";
                m_MyRankItem.m_GetRewards.pressedSprite = "Information_Rank_GetAward1";
                m_MyRankItem.m_GetRewards.hoverSprite = "Information_Rank_GetAward1";
                m_MyRankItem.m_GetRewards.disabledSprite = "Information_Rank_GetAward1";

            }
            else
            {
                byte rankIndx = pWeekRankReward.RankIndex;
                if (FishConfig.Instance.m_RankInfo.m_RankMap.ContainsKey((byte)m_RankType) && 
                    FishConfig.Instance.m_RankInfo.m_RankMap[(byte)m_RankType].RewardMap.ContainsKey(rankIndx))
                {
                    m_MyRankItem.m_GetRewards.isEnabled = true;
                    m_MyRankItem.m_GetRewards.normalSprite = "Information_Rank_GetAward0";
                    m_MyRankItem.m_GetRewards.pressedSprite = "Information_Rank_GetAward0";
                    m_MyRankItem.m_GetRewards.hoverSprite = "Information_Rank_GetAward0";
                    m_MyRankItem.m_GetRewards.disabledSprite = "Information_Rank_GetAward0";
                }
                else
                {
                    m_MyRankItem.m_GetRewards.isEnabled = false;
                    m_MyRankItem.m_GetRewards.normalSprite = "Information_Rank_GetAward2";
                    m_MyRankItem.m_GetRewards.disabledSprite = "Information_Rank_GetAward2";
                }
            }
        }
        else
        {
            m_MyRankItem.m_HistoryRank.text = "上周未上榜";
            m_MyRankItem.m_GetRewards.isEnabled = false;
            m_MyRankItem.m_GetRewards.normalSprite = "Information_Rank_GetAward2";
            m_MyRankItem.m_GetRewards.disabledSprite = "Information_Rank_GetAward2";
        }
        for (byte i = 1; i <= m_MyRankItem.m_Number.Length; ++i)
        {
            if (i == (byte)m_RankType)
                m_MyRankItem.m_Number[i-1].m_BaseObj.SetActive(true);
            else
                m_MyRankItem.m_Number[i-1].m_BaseObj.SetActive(false);
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID() < ConstValue.CUSTOM_HEADER)
            m_MyRankItem.m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID()];
        else
            HeaderManager.Instance.GetPlayerHeader(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID(),
                HeaderOptType.HEADER_RANK, (byte)1);
        QueryMyRank();
        SetMyRankNumber();
        CheckIsHaveReward();
    }
    public void SetHeader(uint userID, byte flag, Texture2D tex)
    {
        if (flag == 2)
        {
            for (byte i = 0; i < m_RankList.Count; ++i)
            {
                if (m_RankList[i].RankInfo.dwUserID == userID)
                {
                    m_RankList[i].SetHeader(tex);
                    break;
                }
            }
        }
        else
        {
            m_MyRankItem.m_HeadTexture.mainTexture = tex;
        }

    }
    static int CompareItemByIndex(RankItemUI item1, RankItemUI item2)
    {
        if (item1.RankInfo.bIndex > item2.RankInfo.bIndex)
            return 1;
        else
           return -1;
    }
    void QueryMyRank()
    {
        for (byte j = 0; j < m_MyRankItem.m_RankingLabel.Length; ++j)
            m_MyRankItem.m_RankingLabel[j].gameObject.SetActive(false);
        for (byte i = 0; i < m_RankList.Count; ++i)
        {
            if (m_RankList[i].RankInfo.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            {
                m_MyRankItem.m_RankingLabel[0].gameObject.SetActive(true);
                m_MyRankItem.m_RankingLabel[0].text = "当前排名:" + m_RankList[i].RankInfo.bIndex.ToString();
                return;
            }
        }
        m_MyRankItem.m_RankingLabel[1].gameObject.SetActive(true);
        m_MyRankItem.m_RankingLabel[1].text = "当前未上榜";
        return;
    }
    void OnCheckRankLeftBtns(GameObject go)//点击排行榜左侧切换按钮
    {
        PlayBtnMusic();
        int checkTag = 0;
        for (int i = 0; i < m_RankLeftBtns.Length; ++i)
        {
            if (m_RankLeftBtns[i].m_BtnObj == go)
            {
                checkTag = m_RankLeftBtns[i].m_Tag + 1;
                m_RankLeftBtns[i].m_IsChecked = true;
                
            }
            else
                m_RankLeftBtns[i].m_IsChecked = false;
        }

        if ((int)m_RankType == checkTag)
            return;
        m_RankType = (RankType)checkTag;
        UpdateRankContent();
        UpdateMyRankContent();
        ExChangeRankLeftBtnsBg();
    }
    void ExChangeRankLeftBtnsBg()
    {
        for (int i = 0; i < m_RankLeftBtns.Length; ++i)
        {
            if (m_RankLeftBtns[i].m_IsChecked == true)
            {

                string str = m_RankLeftBtns[i].m_Btn.hoverSprite;
                if (str != "Information_BtnChecked")
                    ChangeBtnName(m_RankLeftBtns[i].m_Btn, "Information_BtnChecked");
            }
            else
            {
                string str = m_RankLeftBtns[i].m_Btn.hoverSprite;
                if (str != "Information_BtnUnChecked")
                    ChangeBtnName(m_RankLeftBtns[i].m_Btn, "Information_BtnUnChecked");
            }
        }
    }
    void SetMyRankNumber()
    {

        switch (m_RankType)
        {
            case RankType.Achievement_Rank:
                m_MyRankItem.m_Number[(int)m_RankType - 1].m_Label.text = PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementPoint().ToString();
                break;
            case RankType.Gold_Rank:
                m_MyRankItem.m_Number[(int)m_RankType - 1].m_Label.text = PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel().ToString();
                break;
            case RankType.Level_Rank:
                m_MyRankItem.m_Number[(int)m_RankType - 1].m_Label.text = PlayerRole.Instance.RoleInfo.RoleMe.GetLevel().ToString();
                break;
            case RankType.Medal_Rank:
                m_MyRankItem.m_Number[(int)m_RankType - 1].m_Label.text = PlayerRole.Instance.RoleInfo.RoleMe.GetMedal().ToString();
                break;
            case RankType.Enchantment_Rank:     //魅力榜
                {

                    int charmSun = 0;
                    for (byte i = 0; i < PlayerRole.Instance.RoleInfo.RoleMe.GetCharmInfo().Length; ++i)
                    {
                        tagCharmOnce charmOnce;
                        if (FishConfig.Instance.m_CharmInfo.CharmIndexMap.TryGetValue(i, out charmOnce))
                            charmSun += (int)PlayerRole.Instance.RoleInfo.RoleMe.GetCharmInfo()[i] * charmOnce.ChangeCharmValue;
                    }
                    m_MyRankItem.m_Number[(int)m_RankType - 1].m_Label.text = charmSun.ToString();
                }
                break;
            default:
                break;
        }
       
    }
    void CheckIsHaveReward()
    {
        if (PlayerRole.Instance.RoleStatesMessage.GetWeekRankStates())
        {
            for (byte i = 1; i < (byte)RankType.Rand_Max; ++i)
            {
                if (PlayerRole.Instance.RankManager.GetWeekRankInfo().ContainsKey(i) == true)
                {
                    tagRankWeekRankClient pWeekRankReward = PlayerRole.Instance.RankManager.GetWeekRankInfo()[i];
                    if (pWeekRankReward.IsReward)
                    {
                        m_LeftWarning[i-1].SetActive(false);
                    }
                    else
                    {
                        byte rankIndx = pWeekRankReward.RankIndex;
                        if (FishConfig.Instance.m_RankInfo.m_RankMap.ContainsKey(i) &&
                            FishConfig.Instance.m_RankInfo.m_RankMap[i].RewardMap.ContainsKey(rankIndx))
                        {
                            m_LeftWarning[i-1].SetActive(true);
                        }
                        else
                        {
                            m_LeftWarning[i-1].SetActive(false);
                        }
                    }
                }
                else
                    m_LeftWarning[i-1].SetActive(false);
            }
        }
        else
        {
            for (byte i = 0; i < m_LeftWarning.Length; ++i)
            {
                m_LeftWarning[i].SetActive(false);
            }
        }
    }
    void OnClickGetRewards(GameObject go)
    {
        PlayBtnMusic();
        if (PlayerRole.Instance.RankManager.GetWeekRankReward((byte)m_RankType))
        { 
        }
        else
        {
        }
    }
    void ClearGird()
    {
        List<Transform> gridChid = m_Grid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_Grid.transform.DetachChildren();
    }
}

public struct MyRankItemUI
{
    public UITexture        m_HeadTexture;
    public UILabel []       m_RankingLabel;
    public UILabel          m_HistoryRank;
    public RankNumber[]     m_Number;
    public UIButton         m_GetRewards;
}

public class RankItemUI : BaseWnd
{
    UISprite        m_Bg;
    UISprite        m_Crown;//皇冠
    UITexture       m_HeadTextrue;
    UILabel         m_NameLabel;
    UILabel         m_RankLabel;     //排名
    RankNumber[]    m_Number = new RankNumber[5];
    UIButton        m_AddFriendBtn;
    tagRankInfo     m_RankInfo = new tagRankInfo();
    RankType        m_RankType;
    bool            m_bFriends;

    public tagRankInfo RankInfo
    {
        get { return m_RankInfo; }
    }
    public RankItemUI(RankType type, tagRankInfo rankInfo)
    {
        m_RankType = type;
        m_RankInfo = rankInfo;
        m_bFriends = false;
    }
    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseWndObject.SetActive(true);
        m_Bg = m_BaseWndObject.GetComponent<UISprite>();
        m_RankLabel = m_BaseTrans.GetChild(0).GetComponent<UILabel>();
        m_Crown = m_RankLabel.transform.GetChild(0).GetComponent<UISprite>();
        m_NameLabel = m_BaseTrans.GetChild(1).GetChild(1).GetComponent<UILabel>();

        for (byte i = 0; i < m_Number.Length; ++i )
        {
            m_Number[i].m_BaseObj = m_BaseTrans.GetChild(1).GetChild(2).GetChild(i).gameObject;
            m_Number[i].m_Label = m_Number[i].m_BaseObj.transform.GetChild(0).GetComponent<UILabel>();
        }

        m_HeadTextrue = m_BaseTrans.GetChild(1).GetChild(0).GetComponent<UITexture>();
        UIEventListener.Get(m_HeadTextrue.gameObject).onClick = OnClickHead;

        m_AddFriendBtn = m_BaseTrans.GetChild(1).GetChild(3).GetComponent<UIButton>();
        UIEventListener.Get(m_AddFriendBtn.gameObject).onClick = OnClickAddFriend;
        ShowRankInfo();
    }
    public void ResetScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void SetHeader(Texture2D tex)
    {
        if (tex == null)
            m_HeadTextrue.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        else
            m_HeadTextrue.mainTexture = tex;
    }
    public void UpdateRankFriendState()
    {
        //m_bFriends = true;
        //m_AddFriendBtn.isEnabled = "Information_Rank_AddFriend1";
        m_AddFriendBtn.isEnabled = false;
    }
    void ShowRankInfo()
    {
        if (PlayerRole.Instance.RelationManager.QueryRelationRole(m_RankInfo.dwUserID) != null ||
            m_RankInfo.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
          //  m_bFriends = true;
            m_AddFriendBtn.isEnabled = false;

         //   m_AddFriendBtn.spriteName = "Information_Rank_AddFriend1";
        }
        m_RankLabel.text = m_RankInfo.bIndex.ToString();
        if (m_RankInfo.bIndex < 4)
        {
            m_Crown.spriteName = string.Format("Information_Rank_Crown"+m_RankInfo.bIndex.ToString());
            m_Crown.gameObject.SetActive(true);
        }
        if (m_RankInfo.dwFaceID < ConstValue.CUSTOM_HEADER)
            m_HeadTextrue.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[m_RankInfo.dwFaceID];
        else
        {
            HeaderManager.Instance.GetPlayerHeader(m_RankInfo.dwUserID, m_RankInfo.dwFaceID, HeaderOptType.HEADER_RANK, (byte)2);     //别人的附加数据为2 自己为1
        }
        m_NameLabel.text = m_RankInfo.NickName;
        m_Number[(int)m_RankType-1].m_BaseObj.SetActive(true);
        m_Number[(int)m_RankType-1].m_Label.text = m_RankInfo.Param.ToString();
    }
    
    void OnClickHead(GameObject go)
    {
        if (m_RankInfo.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            return;
        if (PlayerRole.Instance.RelationManager.GetFriendMap().ContainsKey(m_RankInfo.dwUserID))
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Friend_Head, m_RankInfo.dwUserID,
                (Texture2D)m_HeadTextrue.mainTexture, false, true);
        }
        else
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Other_Head, m_RankInfo.dwUserID,
                (Texture2D)m_HeadTextrue.mainTexture, false, false);
            PlayerRole.Instance.QueryManager.QueryUserByUserID(m_RankInfo.dwUserID);
        }
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void OnClickAddFriend(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        //if (m_bFriends)
        //    return;
        GlobalHallUIMgr.Instance.ApplyAddFriend(m_RankInfo.dwUserID);

    }

}