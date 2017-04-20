using UnityEngine;
using System.Collections.Generic;

public enum Achieve_State
{
    Achieve_UnFinish = 0,
    Achieve_Finsih,
}

public class HallLoginUI_Achievement : HallLoginUI_BaseWind
{
    WindBtn[]           m_AchieveBtns = new WindBtn[3];
    UILabel[]           m_AchieveInfo = new UILabel[2];     //成就点数 、 成就排名
    UIGrid[]            m_UIGrid = new UIGrid[2];
    UIPanel             m_UIPanel;
    UIScrollView        m_UIScrollView;
    Transform           m_ScrollTrans;
    GameObject          m_BaseItem;
    Achieve_State       m_AchieveSate = Achieve_State.Achieve_UnFinish;
    List<AchieveItem>   m_AchieveList = new List<AchieveItem>();


	public void Init(GameObject go)
    {
        WindObj = go;
        BaseTranF = WindObj.transform;

        for (byte i = 0; i < 2; ++i)
            m_AchieveInfo[i] = BaseTranF.GetChild(0).GetChild(i).GetComponent<UILabel>();

        for (byte j = 0; j < 3; ++j)
        {
            m_AchieveBtns[j].m_BtnObj = BaseTranF.GetChild(1).GetChild(j).gameObject;
            m_AchieveBtns[j].m_Btn = m_AchieveBtns[j].m_BtnObj.GetComponent<UIButton>();
            if (j == 0)
            {
                m_AchieveBtns[j].m_IsChecked = true;
            }
            else
            {
                m_AchieveBtns[j].m_IsChecked = false;
            }

            m_AchieveBtns[j].m_Tag = j;
            GetBtnLister(m_AchieveBtns[j].m_BtnObj).onClick = OnAchieveTopBtnClick;
        }
        ChangeAchievementBtnBg();

        m_UIScrollView = BaseTranF.GetChild(2).GetComponent<UIScrollView>();
        m_ScrollTrans = BaseTranF.GetChild(2);
        m_UIPanel = BaseTranF.GetChild(2).GetComponent<UIPanel>();
        m_BaseItem = BaseTranF.GetChild(2).GetChild(0).gameObject;
        for (byte i = 0; i < 2; ++i)
        {
            m_UIGrid[i] = BaseTranF.GetChild(2).GetChild(i + 1).GetComponent<UIGrid>();
        }
    }
    public void InitAchieveBtnState()
    {
        if (m_AchieveBtns[0].m_IsChecked)
            return;
        m_AchieveBtns[0].m_IsChecked = true;
        m_AchieveBtns[1].m_IsChecked = false;
        ChangeAchievementBtnBg();
    }
    void InitAchieveItem(RoleAchievementBase Value, int indx)
    {
        AchieveItem item = new AchieveItem(true);
        item.Init(m_BaseItem);
        tagAchievementStrInfo achievementStrInfo = FishConfig.Instance.m_AchievementInfo.m_AchievementStr[Value.pConfig.AchievementID];
        item.AchieveID = Value.pConfig.AchievementID;
        string strDesc = achievementStrInfo.AchievementDec;
        string strName = achievementStrInfo.AchievementName;
        int starNum = achievementStrInfo.AchievementStar;
        uint curAchieveValue = PlayerRole.Instance.AchievementManager.GetAchievement(Value.pConfig.AchievementID).pAchievementInfo.AchievementValue;
        uint AchieveValueMax = Value.pConfig.AchievementEventInfo.FinishValue;

        tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID(Value.pConfig.RewardID);
        if (pReward == null)
            return;
        item.ShowAchievementInfo(strDesc, strName,  achievementStrInfo.AchievementIcon, pReward, starNum);
        item.UpdateAchievementProgress(curAchieveValue, AchieveValueMax, Value.m_AchievementEventIsFinish);
        m_AchieveList.Add(item);
    }
    public void UpdateMyAchievemetnData()
    {
        m_AchieveInfo[0].text = "成就点：" + PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementPoint().ToString();
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementIndex() == 0xffffffff)
            m_AchieveInfo[1].text = "成就排名：" + FishConfig.Instance.m_SystemConfig.AchievementRankSum.ToString() + "名以后";
        else
            m_AchieveInfo[1].text = "成就排名：" + (PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementIndex()+1).ToString();
    }
    public void UpdateOnlyAchievementDate(byte achieveID)
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;
        if (PlayerRole.Instance.AchievementManager.GetAchievementArray() == null)
            return;
        for (byte i = 0; i < m_AchieveList.Count; )
        {
            if (m_AchieveList[i].AchieveID == achieveID)
            {
                if (PlayerRole.Instance.AchievementManager.GetAchievementArray().ContainsKey(achieveID) == false)
                {
                    if (int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementStates(), achieveID))
                    {

                        string strTips = string.Format(StringTable.GetString("Finish_Achievement"), FishConfig.Instance.m_AchievementInfo.m_AchievementStr[achieveID].AchievementName);
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI(strTips, 1, true);
                    }
                    AchieveItem item = m_AchieveList[i];
                    m_UIGrid[i % 2].RemoveChild(item.m_BaseTrans);
                    item.ShutDown();
                    Utility.ListRemoveAt(m_AchieveList, i);
                }
                else
                {
                    uint curAchieveValue = PlayerRole.Instance.AchievementManager.GetAchievement(achieveID).pAchievementInfo.AchievementValue;
                    uint AchieveValueMax = PlayerRole.Instance.AchievementManager.GetAchievementArray()[achieveID].pConfig.AchievementEventInfo.FinishValue;

                    tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID(PlayerRole.Instance.AchievementManager.GetAchievementArray()[achieveID].pConfig.RewardID);
                    if (pReward == null)
                        continue; ;

                    //if (pReward.RewardTitleID == 0)
                    //    bHaveName = false;
                    //if (FishConfig.Instance.m_TitleInfo.m_TileMap.ContainsKey( pReward.RewardTitleID) == false)
                    //    bHaveName = false;
                    if (curAchieveValue >= AchieveValueMax)
                        m_AchieveList[i].UpdateAchievementProgress(curAchieveValue, AchieveValueMax, true);
                    else
                        m_AchieveList[i].UpdateAchievementProgress(curAchieveValue, AchieveValueMax);
                }
                break;
            }
            else
            {
                ++i;
                //新增一个成就
                if ( (i == m_AchieveList.Count) && (PlayerRole.Instance.AchievementManager.GetAchievementArray().ContainsKey(achieveID) == true) )
                {
                   //  InitAchieveItem(PlayerRole.Instance.AchievementManager.GetAchievementArray()[achieveID], m_AchieveList.Count);
                    UpdateAchievementDate();
                }
            }
        }
        
    }
    public void UpdateAchievementDate()
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;
        if (PlayerRole.Instance.AchievementManager.GetAchievementArray() == null)
            return;
        ClearGrid();
        m_AchieveList.Clear();
        InitAchieveBtnState();

        int indx = 0;
        foreach (KeyValuePair<byte, RoleAchievementBase> map in PlayerRole.Instance.AchievementManager.GetAchievementArray())
        {
            InitAchieveItem(map.Value, indx);
            ++indx;
        }
        m_AchieveList.Sort(CompareItemBySate);
        for (int i = 0; i < m_AchieveList.Count; ++i )
        {
            m_UIGrid[i % 2].AddChild(m_AchieveList[i].m_BaseTrans);
            m_AchieveList[i].ResetScale();
        }
    }
    public void UpdateFinishAchievementDate()
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;
        if (m_AchieveSate == Achieve_State.Achieve_UnFinish)
            return;
        ClearGrid();
        m_AchieveList.Clear();

        m_ScrollTrans.localPosition = new Vector3(0, 55, 0);
        m_UIPanel.clipOffset = new Vector2(0, -110);
        m_UIScrollView.ResetPosition();

        int indx = 0;
        foreach (KeyValuePair<byte, tagAchievementConfig> map in FishConfig.Instance.m_AchievementInfo.m_AchievementMap)
        {
            if (!int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementStates(), map.Key))
                continue;
            if (map.Value.UpperAchievementID != 0 &&
                int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementStates(), map.Value.UpperAchievementID))
                continue;

            AchieveItem item = new AchieveItem(false);
            item.Init(m_BaseItem);
            tagAchievementStrInfo achievementStrInfo = FishConfig.Instance.m_AchievementInfo.m_AchievementStr[map.Value.AchievementID];
            string strDesc = achievementStrInfo.AchievementDec;
            string strName = achievementStrInfo.AchievementName;
            int starNum = achievementStrInfo.AchievementStar;
            uint AchieveValueMax = map.Value.AchievementEventInfo.FinishValue;
            item.AchieveID = map.Value.AchievementID;

            tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID(map.Value.RewardID);
            if (pReward == null)
                continue;

            item.ShowAchievementInfo(strDesc, strName, achievementStrInfo.AchievementIcon, pReward, starNum + 1);
            item.UpdateAchievementProgress(AchieveValueMax, AchieveValueMax);
            m_UIGrid[indx % 2].AddChild(item.m_BaseTrans);
            item.ResetScale();
            m_AchieveList.Add(item);
            ++indx;
        }

    }
    static int CompareItemBySate(AchieveItem item1, AchieveItem item2)
    {
        if (item1.bFinish && !item2.bFinish)
            return -1;
        else if (!item1.bFinish && item2.bFinish)
            return 1;
        else
            return 0;

    }
    void OnAchieveTopBtnClick(GameObject go)
    {
        PlayBtnMusic();
        int CilckIndex = 0;
        for (int i = 0; i < 3; ++i)
        {
            if (m_AchieveBtns[i].m_BtnObj == go)
            {
                CilckIndex = m_AchieveBtns[i].m_Tag;
                m_AchieveBtns[i].m_IsChecked = true;
            }
            else
                m_AchieveBtns[i].m_IsChecked = false;
        }
        if (CilckIndex == 0)
        {
            
            ChangeAchievementBtnBg();
            m_AchieveSate = (Achieve_State)CilckIndex;
            UpdateAchievementDate();
        }
        else if (CilckIndex == 1)
        {
            ChangeAchievementBtnBg();
            m_AchieveSate = (Achieve_State)CilckIndex;
            UpdateFinishAchievementDate();
        }

    }

    //切换成就中完成未完成按纽背景
    void ChangeAchievementBtnBg()
    {
        for (int i = 0; i < 2; ++i)
        {
            string str = m_AchieveBtns[i].m_Btn.normalSprite;
            if (m_AchieveBtns[i].m_IsChecked == true)
            {
                str = str.Replace("1", "0");
                ChangeBtnName(m_AchieveBtns[i].m_Btn, str);
            }
            else
            {
                str = str.Replace("0", "1");
                ChangeBtnName(m_AchieveBtns[i].m_Btn, str);
            }
        }
    }
    void ClearGrid()
    {
        for (byte i = 0; i < 2; i ++)
        {
            List<Transform> gridChid = m_UIGrid[i].GetChildList();

            foreach (Transform tr in gridChid)
            {
                GameObject.Destroy(tr.gameObject);
            }
            m_UIGrid[i].transform.DetachChildren();
        }

    }
}

public class AchieveItem : BaseWnd
{
    GameObject          m_AchieveStateBg;
    UISprite            m_AchieveSlider;
    UISprite            m_AchieveIcon;
    UISprite[]          m_Star = new UISprite[3];
    UILabel             m_AchieveName;
    UILabel             m_AchievePoint;
    UILabel             m_AchieveDesc;  //成就描述
    UILabel             m_AchieveProgress;  //成就进度
    GameObject          m_bHaveName;
    byte                m_AchieveID;
    bool                m_bListener;
    bool m_bFinish;

    public bool bFinish
    {
        get { return m_bFinish; }
    }

    public AchieveItem(bool bListener)
    {
        m_bListener = bListener;
    }
    public byte AchieveID
    {
        get { return m_AchieveID; }
        set { m_AchieveID = value; }
    }

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseWndObject.SetActive(true);

        m_AchieveStateBg = m_BaseTrans.GetChild(6).gameObject;
        m_AchieveSlider = m_BaseTrans.GetChild(0).GetChild(0).GetComponent<UISprite>();
        m_AchieveProgress = m_BaseTrans.GetChild(0).GetChild(1).GetComponent<UILabel>();
        m_AchieveIcon = m_BaseTrans.GetChild(1).GetComponent<UISprite>();
        for (byte i = 0; i < 3; ++i)
        {
            m_Star[i] = m_BaseTrans.GetChild(1).GetChild(i).GetComponent<UISprite>();
        }
        m_AchievePoint = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        m_AchieveDesc = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
        m_AchieveName = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        m_bHaveName = m_BaseTrans.GetChild(5).gameObject;
        if (m_bListener)
            UIEventListener.Get(m_BaseWndObject).onClick = OnReceiveAchievement;
    }
    public void ResetScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void ShowAchievementInfo(string achieveDesc, string achieveName,  string achieveIcon, tagRewardOnce pReward, int starNum)
    {
        bool bHaveName = false;
        m_AchieveDesc.text = achieveDesc;
        m_AchieveIcon.spriteName = achieveIcon;
        for (int i = 0; i < starNum; ++i)
            m_Star[i].spriteName = "Information_Star";
        for (byte i = 0; i < pReward.RewardItemVec.Count; ++i )
        {
            uint ItemID = pReward.RewardItemVec[i].ItemID;
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                continue;
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemTypeID == EItemType.IT_AchievementPoint)
                m_AchievePoint.text = "成就点数：" + pReward.RewardItemVec[i].ItemSum.ToString();
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemTypeID == EItemType.IT_Title)
            {
                m_AchieveName.text = FishConfig.Instance.m_TitleInfo.m_TileMap[(byte)pReward.RewardItemVec[i].ItemSum].TitleName; 
                m_bHaveName.SetActive(true);
            }
        }
        
        if (!bHaveName)
            m_AchieveName.text = achieveName;
            
    }
    public void UpdateAchievementProgress(uint curAchieve, uint maxAchieve, bool bFinish = false)
    {
        if (curAchieve >= maxAchieve)
            curAchieve = maxAchieve;
        m_AchieveProgress.text = curAchieve.ToString() + "/" + maxAchieve.ToString();
        m_AchieveSlider.fillAmount = (float)(curAchieve * 1.0f) / (float)(maxAchieve * 1.0f);
        m_bFinish = bFinish;
        if (bFinish)
            m_AchieveStateBg.SetActive(true);
        else
            m_AchieveStateBg.SetActive(false);

    }
    void OnReceiveAchievement(GameObject go)
    {
        if (m_AchieveSlider.fillAmount < 1)
            return;
        PlayerRole.Instance.AchievementManager.GetAchievementReward(m_AchieveID);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
            GameObject.Destroy(m_BaseWndObject);
    }
}


