using UnityEngine;
using System;
using System.Collections.Generic;

public enum MatchType
{
      SignUp_ = 0,  //报名中
      HotMatch_,
      EndMatch_,
}

public class HallLoginUI_Contest : HallLoginUI_BaseWind 
{
//    TweenPosition                                       m_TopBtnsBGAnim;            //比赛报名按纽的移动背景
    TweenPosition                                       m_DetailTopBtnBGAnim;       //比赛详情按钮的移动背景
    TweenScale                                          m_DetailAnim;               //比赛祥情弹出动画
    GameObject                                          m_MatchDetail = null;           //用来显示和隐藏比赛的详细信息
    GameObject[]                                        m_MatchDetailChildUI = new GameObject[3];
 //   WindBtn[]                                           m_TopBtnsList = new WindBtn[3];
    UILabel                                             m_TopMatchTimeLabel;
    UILabel                                             m_TopMatchTitleLable;
    UILabel                                             m_TopMatchSysTimeLabel;
    WindBtn []                                          m_MatchDetailTopBtn = new WindBtn[3];//比赛详细信息topbtns
    Transform[]                                         m_TopBtnsListTrans = new Transform[3];
    Transform[]                                         m_MatchDetailTopBtnsTrans = new Transform[3];
    MatchType                                           m_MatchType = MatchType.HotMatch_;
    ViewInf                                             m_MatchScrollView = new ViewInf();
    ViewInf                                             m_MatchRankScrollView = new ViewInf();//比赛排行的view
    ViewInf                                             m_MatchAwardScrollView = new ViewInf();//比赛奖励的view
    UILabel                                             m_MatchDetialLabel;         //比赛详情的label
    UILabel                                             m_MatchRankTime;            //比赛排名的时间显示
    UIScrollView                                        m_UIScrollView;
    Transform                                           m_ScrollViewTans;
    UIPanel                                             m_ScrollPanel;
    List<MatchViewItem>                                 m_MatchViewList = new List<MatchViewItem>();
    List<MatchRankView>                                 m_MatchRankList = new List<MatchRankView>();
    Dictionary<Byte, tagMonthConfig>                    m_SignUpMap = new Dictionary<Byte, tagMonthConfig>();       //报名中列表
    Dictionary<Byte, tagMonthConfig>                    m_GoingMap = new Dictionary<Byte, tagMonthConfig>();        //进行中列表
    int                                                 m_CurMatchDetailID; //当前显示的比赛祥情的比赛ID
    float                                               m_Elapsed = 0;

    internal Dictionary<Byte, tagMonthConfig> GoingMap
    {
        get { return m_GoingMap; }
    }
    internal Dictionary<Byte, tagMonthConfig> SignUpMap
    {
        get { return m_SignUpMap; }
    }

    public UnityEngine.Object Init()
    {
        UnityEngine.Object obj = Initobj("ContestWind");
        WindID.m_state = HallLogicUIStatue.Hall_State.Hall_Contest;
        WindID.m_SmailWind = 0;
        return obj;
    }
    public void InitGameobject()
    {
        Init_GameObj();
        InitMatch();
        InitMatchDetailUI();
    }
    void InitMatch()//初始化竞赛一级界面
    { 
        Transform BaseTopBtn = BaseTranF.GetChild(0);
        //m_TopBtnsBGAnim = BaseTopBtn.GetChild(0).GetComponent<TweenPosition>();
        //for (byte i = 0; i < 3; i++)
        //{
        //    m_TopBtnsList[i].m_BtnObj = BaseTopBtn.GetChild(i + 1).gameObject;
        //    m_TopBtnsListTrans[i] = BaseTopBtn.GetChild(i + 1);
        //    m_TopBtnsList[i].m_Btn = m_TopBtnsList[i].m_BtnObj.GetComponent<UIButton>();
        //    if (i == 0)
        //        m_TopBtnsList[i].m_IsChecked = true;
        //    else
        //        m_TopBtnsList[i].m_IsChecked = false;
        //    m_TopBtnsList[i].m_Tag = i;
        //  //  GetBtnLister(m_TopBtnsList[i].m_BtnObj).onClick = OnTopBtnChlick;
        //}
        m_TopMatchTimeLabel = BaseTopBtn.GetChild(0).GetComponent<UILabel>();
        m_TopMatchTitleLable = BaseTopBtn.GetChild(1).GetComponent<UILabel>(); 
        m_TopMatchSysTimeLabel = BaseTopBtn.GetChild(2).GetComponent<UILabel>();
        m_MatchScrollView.m_BaseChild = BaseTranF.GetChild(1).GetChild(0).gameObject;
        m_MatchScrollView.m_grid = BaseTranF.GetChild(1).GetChild(1).GetComponent<UIGrid>();
        m_UIScrollView = BaseTranF.GetChild(1).GetComponent<UIScrollView>();
        m_ScrollViewTans = BaseTranF.GetChild(1);
        m_ScrollPanel = BaseTranF.GetChild(1).GetComponent<UIPanel>();
        SetMatchTitleName();
    }
    void InitMatchDetailUI()//初始化竞赛二级界面
    {
        m_MatchDetail = BaseTranF.GetChild(2).gameObject;
        Transform MatchTrans = m_MatchDetail.transform;
        m_DetailAnim = m_MatchDetail.GetComponent<TweenScale>();

        for (byte i = 0; i < 3; ++i)
        {
            m_MatchDetailChildUI[i] = MatchTrans.GetChild(i + 1).gameObject;

            m_MatchDetailTopBtn[i].m_BtnObj = MatchTrans.GetChild(0).GetChild(i).gameObject;
            m_MatchDetailTopBtnsTrans[i] = MatchTrans.GetChild(0).GetChild(i);
            if (i == 1)
            {
                m_MatchDetailTopBtn[i].m_IsChecked = true;
                m_MatchDetailChildUI[i].SetActive(true);
            }
            else
                m_MatchDetailTopBtn[i].m_IsChecked = false;
            m_MatchDetailTopBtn[i].m_Tag = i;
            GetBtnLister(m_MatchDetailTopBtn[i].m_BtnObj, false).onClick = OnClickMatchDetailTopBtn;
        }
        m_DetailTopBtnBGAnim = MatchTrans.GetChild(0).GetChild(3).GetComponent<TweenPosition>();
        ChangeDetailTopBtsBg();
        GetBtnLister(MatchTrans.GetChild(4).gameObject, false).onClick = OnClickCloseBtn;        //OnClickSignBtn;
        GetBtnLister(MatchTrans.GetChild(5).gameObject, false).onClick = OnClickCloseBtn;

        m_MatchDetialLabel = MatchTrans.GetChild(2).GetChild(0).GetComponent<UILabel>();
        //比赛奖励
        m_MatchAwardScrollView.m_BaseChild = MatchTrans.GetChild(1).GetChild(0).GetChild(0).gameObject;
        m_MatchAwardScrollView.m_grid = MatchTrans.GetChild(1).GetChild(0).GetChild(1).GetComponent<UIGrid>();

        //比赛排名
        m_MatchRankScrollView.m_BaseChild = MatchTrans.GetChild(3).GetChild(0).GetChild(0).gameObject;
        m_MatchRankScrollView.m_grid = MatchTrans.GetChild(3).GetChild(0).GetChild(1).GetComponent<UIGrid>();
        m_MatchRankTime = MatchTrans.GetChild(3).GetChild(1).GetComponent<UILabel>();
    }
    void SetMatchTitleName()
    {
        m_TopMatchTitleLable.text = LocalSetting.GetMatchTitle(SDKMgr.PACKAGE_NAME_CRC);
    }
    void ChangeDetailTopBtsBg()
    {
        Vector3 FromPos = m_DetailTopBtnBGAnim.transform.localPosition;
        Vector3 ToPos = Vector3.zero;
 
        for (int i = 0; i < 3; ++i)
        {
            if (m_MatchDetailTopBtn[i].m_IsChecked == true)
            {
                UISprite sp = m_MatchDetailTopBtn[i].m_BtnObj.GetComponent<UISprite>();
                string str = sp.spriteName;
                str = str.Replace("0", "1");
                m_MatchDetailTopBtn[i].m_BtnObj.GetComponent<UISprite>().spriteName = str;
                ToPos = m_MatchDetailTopBtnsTrans[i].localPosition;
                if (i == 0)
                    ToPos.x += 10;
                else if (i == 2)
                    ToPos.x -= 10;
            }
            else
            {
                UISprite sp = m_MatchDetailTopBtn[i].m_BtnObj.GetComponent<UISprite>();
                string str = sp.spriteName;
                str = str.Replace("1", "0");
                m_MatchDetailTopBtn[i].m_BtnObj.GetComponent<UISprite>().spriteName = str;
            }
        }
        m_DetailTopBtnBGAnim.ResetToBeginning();
        m_DetailTopBtnBGAnim.from = FromPos;
        m_DetailTopBtnBGAnim.to = ToPos;
        m_DetailTopBtnBGAnim.PlayForward();
    }
    
    public void ShowMatchDetail(bool bShow,int MatchID)//显示和关闭比赛详细信息
    {
        if (m_MatchDetail.activeSelf != bShow)
            m_MatchDetail.SetActive(bShow);
        if (bShow)
        {
            m_CurMatchDetailID = MatchID;
            OnClickMatchDetailTopBtn(m_MatchDetailTopBtn[1].m_BtnObj);
            WndManager.Instance.Push(m_MatchDetail);
           // m_DetailAnim.ResetToBeginning();
            if (m_DetailAnim != null)
            {
                m_DetailAnim.ResetToBeginning();
                m_DetailAnim.PlayForward();
            }
        }
        else
        {
            m_MatchRankList.Clear();
        }
       
    }
    public void Update(float dTime)
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;
        m_Elapsed += dTime;
        if (m_Elapsed >= 1)
        {
            m_Elapsed = 0;
            UpdateMatchData();
           // PlayerRole.Instance.MonthManager.OnUpdate();
        }
        m_TopMatchSysTimeLabel.text = SystemTime.Instance.GetSystemDateTime.Hour.ToString("D2") + ":" +
            SystemTime.Instance.GetSystemDateTime.Minute.ToString("D2") + ":" +
            SystemTime.Instance.GetSystemDateTime.Second.ToString("D2");
    }
    public void UpdateMatchPeopleNum()
    {
        for (byte i = 0; i < m_MatchViewList.Count; ++i )
        {
            m_MatchViewList[i].UpdateMatchPeopleNum();
        }
    }
    public void UpdateMatchSingleData(byte matchID)
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;

     //   m_MatchViewList[matchID].UpdateMatchStatus();
        UpdateMatchData();
    }

    public void UpdateMatchData()
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;
        if (PlayerRole.Instance.MonthManager.GetSignUpList() == null)
            return;
        m_GoingMap.Clear();
        m_SignUpMap.Clear();
        //查找热门赛事
        foreach (KeyValuePair<byte, tagMonthConfig> map in FishConfig.Instance.m_MatchInfo.m_MonthMap)
        {
            if (PlayerRole.Instance.MonthManager.MonthIsInSignUp(map.Key))
            {
                m_SignUpMap.Add(map.Key, map.Value);
            }

            if (PlayerRole.Instance.MonthManager.MonthIsInStar(map.Key))
            {
                m_GoingMap.Add(map.Key, map.Value);
            }
        }
        ChangeMatchTopWndDate();
    }
    void ChangeMatchTopWndDate()
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;
        if (PlayerRole.Instance.MonthManager.GetSignUpList() == null)
            return;

        ClearMatchViewGrid();
        m_MatchViewList.Clear();
        m_ScrollViewTans.localPosition = new Vector3(-340, 68, 0);
        m_ScrollPanel.clipOffset = new Vector2(57, -110);
        m_UIScrollView.ResetPosition();
        //查找热门赛事
        foreach (KeyValuePair<byte, tagMonthConfig> map in FishConfig.Instance.m_MatchInfo.m_MonthMap)
        {

            if (m_SignUpMap.ContainsKey(map.Key) == false &&
                PlayerRole.Instance.MonthManager.GetSignUpList().ContainsKey(map.Key) == false)
                continue;
            MatchViewItem item = new MatchViewItem();
            item.Init(m_MatchScrollView.m_BaseChild);
            item.SetMatchInfo(map.Key, m_MatchType);
            m_MatchViewList.Add(item);
        }
        m_MatchViewList.Sort(CompareItemByTime);
        for (byte i = 0; i < m_MatchViewList.Count; ++i )
        {
            m_MatchScrollView.m_grid.AddChild(m_MatchViewList[i].m_BaseTrans);
            m_MatchViewList[i].ResetScale();
        }
    }
    static int CompareItemByTime(MatchViewItem item1, MatchViewItem item2)
    {
        if (item1.DayTime > item2.DayTime)
            return 1;
        else if (item1.DayTime < item2.DayTime)
            return -1;
        else
            return 0;
    }
    void ShowMatchRewardDetail()
    {
        CleanScrollView(m_MatchAwardScrollView.m_grid);
        foreach (KeyValuePair<Byte, tagMonthReward> map in FishConfig.Instance.m_MatchInfo.m_MonthMap[(byte)m_CurMatchDetailID].m_MonthReward)
        {
            MatchRewardItem rewardItem = new MatchRewardItem();
            rewardItem.Init(m_MatchAwardScrollView.m_BaseChild);
            rewardItem.SetRewardInfo(map.Value.Index, map.Value.RewardID);
            m_MatchAwardScrollView.m_grid.AddChild(rewardItem.m_BaseTrans);
            rewardItem.m_BaseTrans.localScale = Vector3.one;
        }
    }
    
    void OnClickCloseBtn(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        WndManager.Instance.Pop(m_MatchDetail);

        m_MatchDetail.SetActive(false);
    }
    void OnClickMatchDetailTopBtn(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        bool haveTag = false;
        byte MyTag = 0;
        for (byte i = 0; i < m_MatchDetailTopBtn.Length; ++i)
        {
            if (m_MatchDetailTopBtn[i].m_BtnObj == go)
            {
                haveTag = true;
                MyTag = m_MatchDetailTopBtn[i].m_Tag;
                m_MatchDetailTopBtn[i].m_IsChecked = true;
                m_MatchDetailChildUI[i].SetActive(true);

            }
            else
            {
                m_MatchDetailTopBtn[i].m_IsChecked = false;
                m_MatchDetailChildUI[i].SetActive(false);
            }
        }
        ChangeDetailTopBtsBg();

        if (!haveTag)
            return;

        switch(MyTag)
        {
            case 0:     //奖励

                ShowMatchRewardDetail();
                break;
            case 1:     //祥情
                if (FishConfig.Instance.m_MatchInfo.m_MonthStr.ContainsKey((byte)m_CurMatchDetailID))
                {
                    string strDesc = FishConfig.Instance.m_MatchInfo.m_MonthStr[(byte)m_CurMatchDetailID].MonthDec;
                    strDesc = strDesc.Replace("/n", "\n");
                    m_MatchDetialLabel.text = strDesc;
                }
                break;
            case 2:     //比赛排行
                UpdateMatchRankDate();
                break;
        }
    }
    public void UpdateMatchRankDate()
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true || m_MatchDetailChildUI[2].activeSelf != true)
            return;

        DateTime BeginTime = new DateTime();
        DateTime EndTime = new DateTime();
        PlayerRole.Instance.MonthManager.GetMonthRankTime((byte)m_CurMatchDetailID, ref BeginTime, ref EndTime);

        string beginStr = BeginTime.Year.ToString() + "年" + BeginTime.Month.ToString() + "月" + BeginTime.Day.ToString() + "日" +
             BeginTime.Hour.ToString("D2") + ":" + BeginTime.Minute.ToString("D2");
        string endStr = EndTime.Year.ToString() + "年" + EndTime.Month.ToString() + "月" + EndTime.Day.ToString() + "日" +
            EndTime.Hour.ToString("D2") + ":" + EndTime.Minute.ToString("D2");
        m_MatchRankTime.text = ("比赛开始结束时间为") + beginStr + "-" + endStr;
        //  m_ContestId
        //需要个根据上个界面点击的比赛的id来获得相应的数据
        CleanScrollView(m_MatchRankScrollView.m_grid);
        m_MatchRankList.Clear();
        if (PlayerRole.Instance.MonthManager.GetMonthRankInfo((byte)m_CurMatchDetailID) == null || PlayerRole.Instance.MonthManager.GetMonthRankInfo((byte)m_CurMatchDetailID).m_RankInfo == null)
            return;
        tagMonthRankOnce pRank = PlayerRole.Instance.MonthManager.GetMonthRankInfo((byte)m_CurMatchDetailID);
        foreach(KeyValuePair<Byte, tagMonthRank>map in pRank.m_RankInfo)
        {
            MatchRankView rankView = new MatchRankView();
            rankView.Init(m_MatchRankScrollView.m_BaseChild);
            rankView.SetMatchRankingInfo(map.Value);
            m_MatchRankScrollView.m_grid.AddChild(rankView.m_BaseTrans);
            rankView.m_BaseTrans.localScale = Vector3.one;
            m_MatchRankList.Add(rankView);
        }
    }
    public void SetHeader(uint UserID, Texture2D tex)
    {
        if (m_MatchDetail == null || m_MatchDetail.activeSelf != true)
            return ;
        for (byte i = 0; i < m_MatchRankList.Count; )
        {
            if (m_MatchRankList[i].UserID == UserID)
                m_MatchRankList[i].SetHeader(tex);
            ++i;
        }
    }


    void ClearMatchViewGrid()
    {
        List<Transform> gridChid = m_MatchScrollView.m_grid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_MatchScrollView.m_grid.transform.DetachChildren();
    }
    public void ShutDownMatch()
    {
        if (WindObj != null)
        {
            WndManager.Instance.Pop(m_MatchDetail);
            GameObject.Destroy(WindObj);
        }
        WindObj = null;
        BaseTranF = null;  
    }
}

class MatchViewItem : BaseWnd
{
    UISprite            m_MatchHead;
    GameObject          m_HotFlag;          //是否为热门赛标志
    UILabel             m_MatchName;
    UILabel             m_MatchPeopleNum;   //比赛人数
    UILabel             m_DayLabel;         //比赛日期
    UILabel             m_Timelabel;        //比赛时间
    UILabel             m_EntryFee;         //报名费用
    UILabel             m_AwardGold;        //比赛奖金
    UISprite            m_EnterFeeIcon;     //报名费用图标
    UISprite            m_AwardIcon;        //比赛奖励图标
    UIButton            m_EntryBtn;         //报名按纽
    MatchType           m_MatchType;        //当前显示的比赛类型
    uint                m_DayTime;
    byte                m_MatchID;         //比赛ID

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseWndObject.SetActive(true);
        m_BaseTrans = m_BaseWndObject.transform;

        m_MatchHead = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_HotFlag = m_BaseTrans.GetChild(0).GetChild(0).gameObject;
        m_MatchName = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        m_MatchPeopleNum = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        m_DayLabel = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
        m_Timelabel = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        m_EntryFee = m_BaseTrans.GetChild(5).GetChild(0).GetComponent<UILabel>();
        m_AwardGold = m_BaseTrans.GetChild(6).GetChild(0).GetComponent<UILabel>();
        m_EnterFeeIcon = m_BaseTrans.GetChild(5).GetComponent<UISprite>();
        m_AwardIcon = m_BaseTrans.GetChild(6).GetComponent<UISprite>();
        m_EntryBtn = m_BaseTrans.GetChild(7).GetComponent<UIButton>();
        UIEventListener.Get(m_EntryBtn.gameObject).onClick = OnEntryMatch;
        UIEventListener.Get(m_BaseWndObject).onClick = OnMatchDetail;
    }
    public void ResetScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void SetMatchInfo(byte matchID, MatchType matchType)
    {
        m_MatchID = matchID;
    //    m_MatchType = matchType;

        if (FishConfig.Instance.m_MatchInfo.m_MonthStr.ContainsKey(m_MatchID))
        {
            m_MatchName.text = FishConfig.Instance.m_MatchInfo.m_MonthStr[m_MatchID].MonthName;
            //比赛头像 
            m_MatchHead.spriteName = FishConfig.Instance.m_MatchInfo.m_MonthStr[m_MatchID].MonthIcon;  //暂时没有头像选择先屏蔽
        }
        UpdateMatchPeopleNum();

        DateTime beginTime;
        DateTime endTime;
       // if (MatchType.HotMatch_ == m_MatchType)
          //  beginTime = PlayerRole.Instance.MonthManager.GetMonthGameEndTime(m_MatchID);
       // else if (MatchType.SignUp_ == m_MatchType)
            PlayerRole.Instance.MonthManager.GetMonthNowOrNextStarTime(m_MatchID, out beginTime, out endTime);
        //else
           // PlayerRole.Instance.MonthManager.GetMonthNowOrNextSignTime(m_MatchID, out beginTime, out endTime);
        m_DayLabel.text = GetMatchTimeStr(beginTime);
        m_Timelabel.text = beginTime.Hour.ToString("D2") + ":" + beginTime.Minute.ToString("D2");
        ShowEntryFee();
        ShowAward();
        UpdateMatchStatus();
        m_DayTime = SystemTime.Instance.GetTimeTotalSecond(beginTime);
    }
    public void UpdateMatchPeopleNum()
    {
        if (m_MatchPeopleNum == null || m_MatchPeopleNum.text == null)
            return;
        if (PlayerRole.Instance.MonthManager.GetMonthSigUpPlayerSum(m_MatchID) != 0)
            m_MatchPeopleNum.text = PlayerRole.Instance.MonthManager.GetMonthSigUpPlayerSum(m_MatchID).ToString();
        else
            m_MatchPeopleNum.text = "0";
    }
    public void UpdateMatchStatus()
    {
        if (PlayerRole.Instance.MonthManager.GetSignUpList().ContainsKey(m_MatchID) == true)
        {
            if (HallRunTimeInfo.Login_UI.MatchWind.GoingMap.ContainsKey(m_MatchID) == true)
            {
                ChangeButtonStatus(m_EntryBtn, "Entry_Match_Btn0", true);
                m_EntryBtn.isEnabled = true;

            }
            else
            {
                ChangeButtonStatus(m_EntryBtn, "Entry_Match_Btn1", true);
                m_EntryBtn.isEnabled = false;
            }

        }
        else
        {
                ChangeButtonStatus(m_EntryBtn, "Match_SingUp_Btn0", true);
                m_EntryBtn.isEnabled = true;

        }
    }
    void ChangeButtonStatus(UIButton uiBtn, string strName, bool enabled = true)
    {
        uiBtn.normalSprite = strName;
        uiBtn.hoverSprite = strName;
        uiBtn.pressedSprite = strName;
        uiBtn.disabledSprite = strName;

        uiBtn.isEnabled = enabled;
    }
    void ShowEntryFee()
    {
        //报名费的图标
        tagMonthConfig pMatchMap = FishConfig.Instance.m_MatchInfo.m_MonthMap[m_MatchID];
        if (pMatchMap.SignGlobel > 0)
        {
            m_EnterFeeIcon.spriteName = "100";   //接口留用
            m_EntryFee.text = pMatchMap.SignGlobel.ToString();
        }
        else if (pMatchMap.SignCurrey > 0)
        {
            m_EnterFeeIcon.spriteName = "500";   //接口留用
            m_EntryFee.text = pMatchMap.SignCurrey.ToString();            
        }
        else if (pMatchMap.SignMadel > 0)
        {
            m_EnterFeeIcon.spriteName = "200";
            m_EntryFee.text = pMatchMap.SignMadel.ToString();            
        }
        else if (pMatchMap.SignItem.ItemSum > 0)
        {
            uint itemID = pMatchMap.SignItem.ItemID;
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(itemID) == true)
            {
                m_EnterFeeIcon.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[itemID].ItemIcon;   //接口留用
                m_EntryFee.text = pMatchMap.SignItem.ItemSum.ToString();
            }
        }
        else
        {
            m_EntryFee.text = "免费";
        }
    }
    void ShowAward()
    {
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap[m_MatchID].m_MonthReward.ContainsKey(1))
            return ;

        //奖励图标
        ushort awardID = (ushort)FishConfig.Instance.m_MatchInfo.m_MonthMap[m_MatchID].m_MonthReward[1].RewardID;
        if (!FishConfig.Instance.m_RewardConfig.RewardMap.ContainsKey(awardID))
            return ;
        tagRewardOnce pRewardMap = FishConfig.Instance.m_RewardConfig.RewardMap[awardID];
        uint ItemID = pRewardMap.RewardItemVec[0].ItemID;
        if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
            return;
        m_AwardIcon.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon;
        m_AwardGold.text = pRewardMap.RewardItemVec[0].ItemSum.ToString();
    }
    string GetMatchTimeStr(DateTime beginTime)
    {
        string desStr = "";
        if (SystemTime.Instance.GetSystemDateTime.Day == beginTime.Day)
            return "今天";
        switch(beginTime.DayOfWeek)
        {
            case DayOfWeek.Monday:
                desStr =  "星期-";
                break;
            case DayOfWeek.Tuesday:
                desStr = "星期二";
                break;
            case DayOfWeek.Wednesday:
                desStr = "星期三";
                break;
            case DayOfWeek.Thursday:
                desStr = "星期四";
                break;
            case DayOfWeek.Friday:
                desStr = "星期五";
                break;
            case DayOfWeek.Saturday:
                desStr = "星期六";
                break;
            case DayOfWeek.Sunday:
                desStr = "星期日";
                break;
        }

        return desStr;
    }
    void OnEntryMatch(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (m_EntryBtn.normalSprite == "Match_SingUp_Btn0")
            PlayerRole.Instance.MonthManager.SignUpMonth(m_MatchID);
   
        if (m_EntryBtn.normalSprite == "Entry_Match_Btn0")
            PlayerRole.Instance.MonthManager.JoinMonth(m_MatchID);
     
    }
    void OnMatchDetail(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        HallRunTimeInfo.Instance_UI.m_loginUi.MatchWind.ShowMatchDetail(true, (int)m_MatchID);
    }
    public uint DayTime
    {
        get { return m_DayTime; }
    }
}

class MatchRewardItem : BaseWnd
{
    GameObject          m_CrowObj;
    UISprite            m_MatchRewardBg;
    UISprite            m_CrowSprite; 
    UILabel             m_Rankinglabel;     //名次
    UILabel             m_AwardLabel;       //奖励
    UILabel             m_MadelLabel;       //奖牌

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseWndObject.SetActive(true);
        m_BaseTrans = m_BaseWndObject.transform;

        m_CrowObj = m_BaseTrans.GetChild(0).GetChild(0).gameObject;
        m_MatchRewardBg = m_BaseTrans.GetChild(3).GetComponent<UISprite>();
        m_CrowSprite = m_CrowObj.GetComponent<UISprite>();
        m_Rankinglabel = m_BaseTrans.GetChild(0).GetComponent<UILabel>();
        m_AwardLabel = m_BaseTrans.GetChild(1).GetChild(0).GetComponent<UILabel>();
        m_MadelLabel = m_BaseTrans.GetChild(2).GetChild(0).GetComponent<UILabel>();
    }
    public void SetRewardInfo(ushort ranking, ushort RewardID)
    {
        tagRewardOnce pRewardMap = null;
        if (!FishConfig.Instance.m_RewardConfig.RewardMap.TryGetValue(RewardID, out pRewardMap) || pRewardMap == null)
        {
            LogMgr.Log(string.Format("不存在的奖励ID {0}", RewardID));
            return;
        }
        //前三名特殊处理
        if (ranking < 4)
        {
            m_MatchRewardBg.spriteName = "MatchDetail_Awardbg1";
            m_CrowObj.SetActive(true);

            m_CrowSprite.spriteName = "Information_Rank_Crown" + ranking.ToString();

        }
        m_Rankinglabel.text = ranking.ToString();
        for (byte i = 0; i < pRewardMap.RewardItemVec.Count; ++i )
        {
            uint ItemID = pRewardMap.RewardItemVec[i].ItemID;
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                continue;

            if (FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemTypeID == EItemType.IT_Globel)
                m_AwardLabel.text = pRewardMap.RewardItemVec[i].ItemSum.ToString();
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemTypeID == EItemType.IT_Medal)
                m_MadelLabel.text = pRewardMap.RewardItemVec[i].ItemSum.ToString();
        }
            

    }
}

class MatchRankView : BaseWnd     //比赛排名信息
{
     UITexture          m_HeadTexture;
     UISprite[]         m_HeadTask = new UISprite[2];
     UISprite           m_RankBg;
     UISprite           m_CrowineSp;
     UILabel            m_Ranking;
     UILabel            m_Name;
     UILabel            m_Score;
     uint               m_UserID;

     public uint UserID
     {
         get { return m_UserID; }
     }
    public void Init(GameObject go)
     {
         m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
         m_BaseWndObject.SetActive(true);
         m_BaseTrans = m_BaseWndObject.transform;
         m_HeadTexture = m_BaseTrans.GetChild(0).GetChild(0).GetComponent<UITexture>();
         m_RankBg = m_BaseTrans.GetChild(5).GetComponent<UISprite>();
         m_CrowineSp = m_BaseTrans.GetChild(1).GetComponent<UISprite>();
         m_Ranking = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
         m_Name = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
         m_Score = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        for (byte i = 0; i < m_HeadTask.Length; ++i)
        {
            m_HeadTask[i] = m_BaseTrans.GetChild(0).GetChild(1 + i).GetComponent<UISprite>();

        }

     }
    public void SetMatchRankingInfo(tagMonthRank pRankingInfo)
    {
        if (pRankingInfo.UserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            m_RankBg.spriteName = "Match_Ranking_Bg1";
            for (byte i = 0; i < m_HeadTask.Length; ++i )
                m_HeadTask[i].spriteName = "Avatar0";
            m_CrowineSp.spriteName = "Information_Rank_Crown" + pRankingInfo.Index.ToString();
        }
        else if (pRankingInfo.Index < 4)
        {
            m_RankBg.spriteName = "Match_Ranking_Bg0";
            for (byte i = 0; i < m_HeadTask.Length; ++i )
                m_HeadTask[i].spriteName = "Avatar1";
            m_CrowineSp.spriteName = "Information_Rank_Crown" + pRankingInfo.Index.ToString();
        }
        else
            m_CrowineSp.spriteName = "";
        if (pRankingInfo.FaceID< ConstValue.CUSTOM_HEADER)
            m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[pRankingInfo.FaceID];
        else
        {
            HeaderManager.Instance.GetPlayerHeader(pRankingInfo.UserID, pRankingInfo.FaceID, HeaderOptType.HEADER_MATCH, null);     //别人的附加数据为2 自己为1
        }

        m_Ranking.text = pRankingInfo.Index.ToString();
        m_Name.text = pRankingInfo.NickName;
        m_Score.text = "积分 ："+pRankingInfo.Socres.ToString()+"(VIP加成: "+pRankingInfo.VipSocres.ToString() + ")";
        m_UserID = pRankingInfo.UserID;
    }
    public void SetHeader(Texture2D tex)
    {
        if (tex == null)
            m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        else
            m_HeadTexture.mainTexture = tex;
    }
}