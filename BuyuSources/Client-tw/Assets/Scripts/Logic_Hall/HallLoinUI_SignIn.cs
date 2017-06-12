using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

struct SignInChestsInfo
{
    public GameObject       m_GetFlagObj;           //签到宝箱已经领取标志
    public UIButton         m_SignInChestsBtn;      //签到宝箱
    public UILabel          m_SignInLevel;         //签到等级
    public ushort           m_RewardID;
}
class SignInButtonInfo
{
    public GameObject       m_Obj;
    public UIButton         m_Button;
    public UISprite[]       m_BG = new UISprite[2];   //签到，补签背景
    public GameObject       m_CanSign;                //可签到，可补签
    public GameObject       m_AlreadySign;            //已签到，剩余补签次数为0
    public UILabel          m_DayTime;                //签到的天数、剩余补签天数
    public SignInButtonInfo()
    {
        m_BG = new UISprite[2];
    }
}
class SignInPropReward
{
    public UISprite         m_PropReward;           //某天签到奖励
    public UISprite[]       m_PropRewardBg = new UISprite[2];         
    public UILabel          m_PropNum;
    public UILabel          m_DateTime;
    public GameObject       m_AlreadyGetFlag;
    public GameObject       m_PropHight;
    public byte             m_IndxDay;

    public void Init(Transform trans)
    {
        m_PropReward = trans.GetComponent<UISprite>();
        m_PropRewardBg[0] = trans.GetChild(2).GetComponent<UISprite>();
        m_PropRewardBg[1] = m_PropRewardBg[0].transform.GetChild(0).GetComponent<UISprite>();
        m_PropNum = trans.GetChild(3).GetComponent<UILabel>();
        m_DateTime = trans.GetChild(4).GetComponent<UILabel>();
        m_AlreadyGetFlag = trans.GetChild(0).gameObject;
        m_PropHight = trans.GetChild(1).gameObject;
        UIEventListener.Get(trans.gameObject).onClick = OnClickSignUp;
    }
    void OnClickSignUp(GameObject go)
    {
        byte day = PlayerRole.Instance.CheckManager.GetNowDay();
        if (!PlayerRole.Instance.CheckManager.IsCanCheckNowDay() || m_IndxDay != day)
            return;
        PlayerRole.Instance.CheckManager.CheckNowDay();

        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
}
public class HallLoinUI_SignIn : BaseWnd
{
    SignInChestsInfo[]  m_SignInChestsInfo = new SignInChestsInfo[5];
    SignInButtonInfo[]  m_SignInBtns = new SignInButtonInfo[2]; //签到、补签按纽

    UILabel             m_RetroactivePrice;                     //补签所需钻石数
    UIPanel             m_UIPanel;
    Transform           m_ScrollTrans;
    UIGrid[]            m_UIGrid = new UIGrid[5];
    GameObject[]        m_AgainSignInBtnBg = new GameObject[2];
    GameObject          m_BasePropItem;
    float               m_BtnCDTime;                        //补签按纽CD时间
    float               m_BtnCDInterval = 2;                    
    bool                m_bPlayBtnCD = false;
   // HallLoginUI_Activity m_Parent;
	public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;

        for (byte i = 0; i < m_SignInChestsInfo.Length; ++i)
        {
            m_SignInChestsInfo[i].m_SignInChestsBtn = m_BaseTrans.GetChild(i).GetComponent<UIButton>();
            m_SignInChestsInfo[i].m_GetFlagObj = m_BaseTrans.GetChild(i).GetChild(1).gameObject;
            m_SignInChestsInfo[i].m_SignInLevel = m_BaseTrans.GetChild(i).GetChild(0).GetComponent<UILabel>();
            UIEventListener.Get(m_SignInChestsInfo[i].m_SignInChestsBtn.gameObject).onClick = OnClickChestsReward;
        }
        for (byte i = 0; i < m_SignInBtns.Length; ++i)
        {
            m_SignInBtns[i] = new SignInButtonInfo();

            Transform tran = m_BaseTrans.GetChild(8 + i);
            m_SignInBtns[i].m_Button = tran.GetComponent<UIButton>();
            m_SignInBtns[i].m_Obj = tran.gameObject;
            m_SignInBtns[i].m_DayTime = m_BaseTrans.GetChild(6 + i).GetComponent<UILabel>();
            m_SignInBtns[i].m_BG[0] = tran.GetChild(1).GetComponent<UISprite>();
            m_SignInBtns[i].m_BG[1] = m_SignInBtns[i].m_BG[0].transform.GetChild(0).GetComponent<UISprite>();
            m_SignInBtns[i].m_CanSign = tran.GetChild(0).gameObject;
            m_SignInBtns[i].m_AlreadySign = tran.GetChild(2).gameObject;
            if (i == 0)
            {
                UIEventListener.Get(m_SignInBtns[i].m_Obj).onClick = OnClickRepeatSignIn;
                UIEventListener.Get(m_SignInBtns[i].m_Obj).onPress = OnPressRepeatSignIn;
                UIEventListener.Get(m_SignInBtns[i].m_Obj).onHover = OnPressRepeatSignIn;
            }
            else
            {
                UIEventListener.Get(m_SignInBtns[i].m_Obj).onClick = OnClickSignIn;
                UIEventListener.Get(m_SignInBtns[i].m_Obj).onPress = OnPressSignIn;
                UIEventListener.Get(m_SignInBtns[i].m_Obj).onHover = OnPressSignIn;
            }
        }
        m_RetroactivePrice = m_BaseTrans.GetChild(8).GetChild(0).GetComponent<UILabel>();
        m_ScrollTrans = m_BaseTrans.GetChild(5).GetChild(0);
        m_UIPanel = m_BaseTrans.GetChild(5).GetChild(0).GetComponent<UIPanel>();
        m_BasePropItem = m_BaseTrans.GetChild(5).GetChild(0).GetChild(0).gameObject;
        for (byte i = 0; i < m_UIGrid.Length; ++i)
            m_UIGrid[i] = m_BaseTrans.GetChild(5).GetChild(0).GetChild(1 + i).GetComponent<UIGrid>();
    }
    public void Updtae(float deltaTime)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        if (m_bPlayBtnCD)
        {
            m_BtnCDTime += deltaTime;
            if (m_BtnCDTime >= m_BtnCDInterval)
            {
                m_bPlayBtnCD = false;
                if (PlayerRole.Instance.CheckManager.GetCanCheckOtherSum() != 0)
                {
                    SetButton(m_SignInBtns[0], true);
                }
            }
            else
            {
                SetButton(m_SignInBtns[0], false);
            }
        }
    }
    public void UpdateSignInData()
    {
        if (m_SignInBtns == null || m_SignInBtns[0].m_DayTime == null || m_SignInBtns[1].m_DayTime == null || m_RetroactivePrice == null) 
            return;

        m_SignInBtns[0].m_DayTime.text = PlayerRole.Instance.CheckManager.GetMonthCheckDaySum().ToString();
        m_SignInBtns[1].m_DayTime.text = PlayerRole.Instance.CheckManager.GetCanCheckOtherSum().ToString();
        m_RetroactivePrice.text = FishConfig.Instance.m_CheckInfo.CheckOtherUser.ToString();
        
        //补签按纽状态刷新
        if (PlayerRole.Instance.CheckManager.GetCanCheckOtherSum() == 0)
        {
            m_RetroactivePrice.gameObject.SetActive(false);
            m_SignInBtns[0].m_AlreadySign.SetActive(true);
            m_SignInBtns[0].m_CanSign.SetActive(false);
            SetButton(m_SignInBtns[0], false);
        }
        else
        {
            m_RetroactivePrice.gameObject.SetActive(true);
            m_SignInBtns[0].m_AlreadySign.SetActive(false);
            m_SignInBtns[0].m_CanSign.SetActive(true);
            SetButton(m_SignInBtns[0], true);
        }
        //签到按纽状态刷新
        if (!PlayerRole.Instance.CheckManager.IsCanCheckNowDay())
        {
            m_SignInBtns[1].m_AlreadySign.SetActive(true);
            m_SignInBtns[1].m_CanSign.SetActive(false);
            SetButton(m_SignInBtns[1], false);
        }
        else
        {
            m_SignInBtns[1].m_AlreadySign.SetActive(false);
            m_SignInBtns[1].m_CanSign.SetActive(true);
            SetButton(m_SignInBtns[1], true);
        }
        UpdateSignInPropState();
        UpdateSignInChestsState();
    }
    void UpdateSignInChestsState()
    {
        byte SignInSumDay = PlayerRole.Instance.CheckManager.GetMonthCheckDaySum();
        //判断是否需要向左移动两个
        bool bMove = false;
        byte Index = 0;
        foreach(KeyValuePair<byte, ushort> map in FishConfig.Instance.m_CheckInfo.MonthCheckReward)
        {
            if (SignInSumDay >= map.Key && Index == 2)
            {
                bMove = true;
                break;
            }
            ++Index;
        }
        //从第一个开始显示
        Index = 0;
        if (!bMove)
        {
            foreach (KeyValuePair<byte, ushort> map in FishConfig.Instance.m_CheckInfo.MonthCheckReward)
            {
                if (Index == 5)
                    break;
                m_SignInChestsInfo[Index].m_SignInLevel.text = map.Key.ToString() + StringTable.GetString("Global_Day_2");
                m_SignInChestsInfo[Index].m_RewardID = map.Value;

                if (SignInSumDay >= map.Key)
                {
                    m_SignInChestsInfo[Index].m_SignInChestsBtn.isEnabled = false;
                    m_SignInChestsInfo[Index].m_GetFlagObj.SetActive(true);
                    m_SignInChestsInfo[Index].m_SignInChestsBtn.normalSprite = "Activity_SignIn_Chests0";
                    m_SignInChestsInfo[Index].m_SignInChestsBtn.disabledSprite = "Activity_SignIn_Chests0";
                }
                Index++;
            }
        }
        else
        {
            foreach (KeyValuePair<byte, ushort> map in FishConfig.Instance.m_CheckInfo.MonthCheckReward)
            {
                if (Index < 2)
                {
                    Index++;
                    continue;
                }
                m_SignInChestsInfo[Index-2].m_SignInLevel.text = map.Key.ToString() + StringTable.GetString("Global_Day_2");
                m_SignInChestsInfo[Index-2].m_RewardID = map.Value;

                if (SignInSumDay >= map.Key)
                {
                    m_SignInChestsInfo[Index-2].m_SignInChestsBtn.isEnabled = false;
                    m_SignInChestsInfo[Index-2].m_GetFlagObj.SetActive(true);
                    //m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.normalSprite = GetDisableSpriteName(Index);
                    //m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.disabledSprite = GetDisableSpriteName(Index);
                }
                else
                {
                    m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.isEnabled = true;
                    m_SignInChestsInfo[Index - 2].m_GetFlagObj.SetActive(false);
                    //m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.normalSprite = GetNormalSpriteName(Index);
                    //m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.hoverSprite = GetNormalSpriteName(Index);
                    //m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.pressedSprite = GetNormalSpriteName(Index);
                    //m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.disabledSprite = GetNormalSpriteName(Index);
                }
                Index++;

            }
        }
    }
    void UpdateSignInPropState()
    {
        ClearGird();
        byte Indx = 0;
        int day = PlayerRole.Instance.CheckManager.GetNowDay();
        foreach (KeyValuePair<byte, ushort> map in FishConfig.Instance.m_CheckInfo.CheckDayReward)
        {
            SignInPropReward prop = new SignInPropReward();
            GameObject go = GameObject.Instantiate(m_BasePropItem) as GameObject;
            prop.Init(go.transform);
            SetPropInfo(prop, map.Key, map.Value);
            go.SetActive(true);
            m_UIGrid[Indx % 5].AddChild(go.transform);
            go.transform.localScale = Vector3.one;
            Indx++;
        }
        if (day > 10 && day <= 20)
        {
            m_ScrollTrans.localPosition = new Vector3(0, 260, 0);
            m_UIPanel.clipOffset = new Vector2(0, -260);
        }
        else if (day > 20 && day <= 30)
        {
            m_ScrollTrans.localPosition = new Vector3(0, 450, 0);
            m_UIPanel.clipOffset = new Vector2(0, -450);
        }
        else if (day > 30)
        {
            m_ScrollTrans.localPosition = new Vector3(0, 520, 0);
            m_UIPanel.clipOffset = new Vector2(0, -520);
        }

    }
    void SetPropInfo(SignInPropReward prop, byte day, ushort rewardID)
    {
        tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID(rewardID);
        if (pReward == null)
            return;
        uint ItemID = pReward.RewardItemVec[0].ItemID;
        if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
            return;
        prop.m_PropReward.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon;
        prop.m_DateTime.text = day.ToString();
        prop.m_IndxDay = day;

        //当前已经签 到
        if (PlayerRole.Instance.CheckManager.IsCheckByDay(day) )
        {
            prop.m_PropReward.color = new Color(0.5f, 0.5f, 0.5f);
            prop.m_PropRewardBg[0].color = new Color(0.5f, 0.5f, 0.5f);
            prop.m_PropRewardBg[1].color = new Color(0.5f, 0.5f, 0.5f);
            prop.m_PropNum.text = "";
            prop.m_AlreadyGetFlag.SetActive(true);
            prop.m_PropHight.SetActive(false);

            string bgName = GetRewardBgName(0);
            prop.m_PropRewardBg[0].spriteName = bgName;
            prop.m_PropRewardBg[1].spriteName = bgName;
        }
        else
        {
            prop.m_PropNum.text = pReward.RewardItemVec[0].ItemSum.ToString();

            //int NowDay= SystemTime.Instance.GetSystemDateTime.Day;//1-31
            int NowDay = PlayerRole.Instance.CheckManager.GetNowDay();
            if (day == NowDay)
            {
                prop.m_PropHight.SetActive(true);
                string bgName = GetRewardBgName(1);
                prop.m_PropRewardBg[0].spriteName = bgName;
                prop.m_PropRewardBg[1].spriteName = bgName;
            }
            else if (day < NowDay)
            {
                prop.m_PropReward.color = new Color(0.5f, 0.5f, 0.5f);
                prop.m_PropRewardBg[0].color = new Color(0.5f, 0.5f, 0.5f);
                prop.m_PropRewardBg[1].color = new Color(0.5f, 0.5f, 0.5f);

                string bgName = GetRewardBgName(2);
                prop.m_PropRewardBg[0].spriteName = bgName;
                prop.m_PropRewardBg[1].spriteName = bgName;
            }

        }

    }
    void OnClickChestsReward(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        for (byte i = 0; i < m_SignInChestsInfo.Length; ++i)
        {
            if (go == m_SignInChestsInfo[i].m_SignInChestsBtn.gameObject)
                GlobalHallUIMgr.Instance.PreviewRewardWnd(m_SignInChestsInfo[i].m_RewardID, ActivityType.SignIn_UI);
        }
    }
    void OnClickRepeatSignIn(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (FishConfig.Instance.m_CheckInfo.CheckOtherUser > PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency())
        {
            GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
            return;
        }
        PlayerRole.Instance.CheckManager.CheckOtherDay();
        m_bPlayBtnCD = true;
        m_BtnCDTime = 0;
        SetButton(m_SignInBtns[0], false);
    }
    void OnClickSignIn(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        PlayerRole.Instance.CheckManager.CheckNowDay();
    }
    void OnPressRepeatSignIn(GameObject go, bool state)
    {
        string spriteName = state ? GetSpriteName(UIButtonColor.State.Pressed) : GetSpriteName(UIButtonColor.State.Normal);
        m_SignInBtns[0].m_BG[0].spriteName = spriteName;
        m_SignInBtns[0].m_BG[1].spriteName = spriteName;
    }
    void OnPressSignIn(GameObject go, bool state)
    {
        string spriteName = state ? GetSpriteName(UIButtonColor.State.Pressed) : GetSpriteName(UIButtonColor.State.Normal);
        m_SignInBtns[1].m_BG[0].spriteName = spriteName;
        m_SignInBtns[1].m_BG[1].spriteName = spriteName;
    }
    string GetSpriteName(UIButtonColor.State state)
    {
        string spriteName = string.Empty;
        switch (state)
        {
            case UIButtonColor.State.Normal:
                spriteName = "yellowBtn";
                break;
            case UIButtonColor.State.Hover:
            case UIButtonColor.State.Pressed:
                spriteName = "yellowBtn-click";
                break;
            case UIButtonColor.State.Disabled:
                spriteName = "yellowBtn-click";
                break;
        }
        return spriteName;
    }
    void SetButton(SignInButtonInfo buttonInfo, bool enable)
    {
        if (buttonInfo == null || buttonInfo.m_Button == null) return;

        buttonInfo.m_Button.isEnabled = enable;
        UIButtonColor.State state = enable ? UIButtonColor.State.Normal : UIButtonColor.State.Disabled;
        string spriteName = GetSpriteName(state);
        buttonInfo.m_BG[0].spriteName = spriteName;
        buttonInfo.m_BG[1].spriteName = spriteName;
    }
    void ClearGird()
    {
        for (byte i = 0; i < 5; ++i)
        {
            List<Transform> gridChid = m_UIGrid[i].GetChildList();

            foreach (Transform tr in gridChid)
            {
                GameObject.Destroy(tr.gameObject);
            }
            m_UIGrid[i].transform.DetachChildren();
        }

    }
    // 0之前 1今天 2以后
    string GetRewardBgName(int index)
    {
        string str = "unsign";
        if (index == 0)
        {
            str = "signed";
        }
        else if (index == 1)
        {
            str = "sign-today";
        }
        return str;
    }
}
