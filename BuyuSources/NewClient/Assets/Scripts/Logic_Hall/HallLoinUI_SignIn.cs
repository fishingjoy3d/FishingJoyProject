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

class SignInPropReward
{
    public UISprite         m_PropReward;           //某天签到奖励
    public UISprite         m_PropRewardBg;         
    public UILabel          m_PropNum;
    public UILabel          m_DateTime;
    public GameObject       m_AlreadyGetFlag;
    public GameObject       m_PropHight;
    public byte             m_IndxDay;

    public void Init(Transform trans)
    {
        m_PropReward = trans.GetComponent<UISprite>();
        m_PropRewardBg = trans.GetChild(2).GetComponent<UISprite>();
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
    UIButton[]          m_SignInBtn = new UIButton[2];          //签到、补签按纽 
    UISprite            m_SignInBtnBg;                          //签到背景
    UILabel[]           m_SignInDayTime = new UILabel[2];       //签到的天数、剩余补签天数
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
        for (byte i = 0; i < m_SignInBtn.Length; ++i)
        {
            m_SignInBtn[i] = m_BaseTrans.GetChild(8 + i).GetComponent<UIButton>();
            
            m_SignInDayTime[i] = m_BaseTrans.GetChild(6 + i).GetComponent<UILabel>();
            if (i == 0)
            {
                m_AgainSignInBtnBg[0] = m_BaseTrans.GetChild(8).GetChild(0).gameObject;
                m_AgainSignInBtnBg[1] = m_BaseTrans.GetChild(8).GetChild(2).gameObject;
                UIEventListener.Get(m_SignInBtn[i].gameObject).onClick = OnClickRepeatSignIn;
            }
            else
            {
                m_SignInBtnBg = m_BaseTrans.GetChild(8 + i).GetChild(1).GetComponent<UISprite>();
                UIEventListener.Get(m_SignInBtn[i].gameObject).onClick = OnClickSignIn;
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
                    m_SignInBtn[0].isEnabled = true;
            }
            else
            {
                if (m_SignInBtn[0].isEnabled != false)
                    m_SignInBtn[0].isEnabled = false;
            }
        }
    }
    public void UpdateSignInData()
    {
        if (m_SignInDayTime == null || m_SignInDayTime[0] == null || m_SignInDayTime[1] == null || m_RetroactivePrice == null) 
            return;

        m_SignInDayTime[0].text = PlayerRole.Instance.CheckManager.GetMonthCheckDaySum().ToString();
        m_SignInDayTime[1].text = PlayerRole.Instance.CheckManager.GetCanCheckOtherSum().ToString();
        m_RetroactivePrice.text = FishConfig.Instance.m_CheckInfo.CheckOtherUser.ToString();
        
        //补签按纽状态刷新
        if (PlayerRole.Instance.CheckManager.GetCanCheckOtherSum() == 0)
        {
            m_RetroactivePrice.gameObject.SetActive(false);
            m_SignInBtn[0].isEnabled = false;
            m_AgainSignInBtnBg[0].SetActive(false);
            m_AgainSignInBtnBg[1].SetActive(true);
            ChangeBtnBg(m_SignInBtn[0], "Btn_Bg_2");
        }
        else
        {
            m_RetroactivePrice.gameObject.SetActive(true);
            m_SignInBtn[0].isEnabled = true;
            m_AgainSignInBtnBg[0].SetActive(true);
            m_AgainSignInBtnBg[1].SetActive(false);
            ChangeBtnBg(m_SignInBtn[0], "Btn_Bg_0");
        }
        //签到按纽状态刷新
        if (!PlayerRole.Instance.CheckManager.IsCanCheckNowDay())
        {
            m_SignInBtn[1].isEnabled = false;
            m_SignInBtnBg.spriteName = "Activity_SigInBtn1";
            m_SignInBtnBg.width = 84;
            ChangeBtnBg(m_SignInBtn[1], "Btn_Bg_2");
        }
        else
        {
            m_SignInBtn[1].isEnabled = true;
            m_SignInBtnBg.spriteName = "Activity_SigInBtn0";
            m_SignInBtnBg.width = 64;
            ChangeBtnBg(m_SignInBtn[1], "Btn_Bg_0");
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
                m_SignInChestsInfo[Index].m_SignInLevel.text = map.Key.ToString() + "天";
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
                m_SignInChestsInfo[Index-2].m_SignInLevel.text = map.Key.ToString() + "天";
                m_SignInChestsInfo[Index-2].m_RewardID = map.Value;

                if (SignInSumDay >= map.Key)
                {
                    m_SignInChestsInfo[Index-2].m_SignInChestsBtn.isEnabled = false;
                    m_SignInChestsInfo[Index-2].m_GetFlagObj.SetActive(true);
                    m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.normalSprite = GetDisableSpriteName(Index);
                    m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.disabledSprite = GetDisableSpriteName(Index);
                }
                else
                {
                    m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.isEnabled = true;
                    m_SignInChestsInfo[Index - 2].m_GetFlagObj.SetActive(false);
                    m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.normalSprite = GetNormalSpriteName(Index);
                    m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.hoverSprite = GetNormalSpriteName(Index);
                    m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.pressedSprite = GetNormalSpriteName(Index);
                    m_SignInChestsInfo[Index - 2].m_SignInChestsBtn.disabledSprite = GetNormalSpriteName(Index);
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
            m_ScrollTrans.localPosition = new Vector3(0, 270, 0);
            m_UIPanel.clipOffset = new Vector2(0, -270);
        }
        else if (day > 20 && day <= 30)
        {
            m_ScrollTrans.localPosition = new Vector3(0, 500, 0);
            m_UIPanel.clipOffset = new Vector2(0, -500);
        }
        else if (day > 30)
        {
            m_ScrollTrans.localPosition = new Vector3(0, 600, 0);
            m_UIPanel.clipOffset = new Vector2(0, -600);
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
            prop.m_PropRewardBg.color = new Color(0.5f, 0.5f, 0.5f);
            prop.m_PropNum.text = "";
            prop.m_AlreadyGetFlag.SetActive(true);
            prop.m_PropHight.SetActive(false);
        }
        else
        {
            prop.m_PropNum.text = pReward.RewardItemVec[0].ItemSum.ToString();

            //int NowDay= SystemTime.Instance.GetSystemDateTime.Day;//1-31
            int NowDay = PlayerRole.Instance.CheckManager.GetNowDay();
            if (day == NowDay)
                prop.m_PropHight.SetActive(true);
            else if (day < NowDay)
            {
                prop.m_PropReward.color = new Color(0.5f, 0.5f, 0.5f);
                prop.m_PropRewardBg.color = new Color(0.5f, 0.5f, 0.5f);
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
        m_SignInBtn[0].isEnabled = false;
    }
    void OnClickSignIn(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        PlayerRole.Instance.CheckManager.CheckNowDay();
    }
    string GetDisableSpriteName(byte Index)
    {
        if (Index < 3)
            return "Activity_SignIn_Chests0";
        else if (Index >= 3 && Index < 6)
            return "Activity_SignIn_Chests2";
        else
            return "Activity_SignIn_Chests4";

    }
    string GetNormalSpriteName(byte Index)
    {
        if (Index < 3)
            return "Activity_SignIn_Chests1";
        else if (Index >= 3 && Index < 6)
            return "Activity_SignIn_Chests3";
        else
            return "Activity_SignIn_Chests5";
    }
    void ChangeBtnBg(UIButton btn, string str)
    {
        btn.normalSprite = str;
        btn.disabledSprite = str;
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
}
