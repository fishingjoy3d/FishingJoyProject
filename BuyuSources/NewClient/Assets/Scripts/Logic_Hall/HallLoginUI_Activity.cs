using System;
using UnityEngine;
using System.Collections.Generic;

public enum ActivityType
{
    SignIn_UI = 0,          //签到UI
    LevelActivity_UI,       //等级活动UI
    PayActivity_UI,         //充值送礼UI
    TimeLimitActivity_UI,   //限时活动
    ActivityMax_Num = 4,    //活动最大数
}

struct NodeItem
{
    public UIButton     m_LevelNodeBtn;
    public UISprite     m_LevelNodeBg;
    public UISprite     m_LevelBg;
    public UILabel      m_LevelLabel;
    public GameObject   m_BaseObj;
    public GameObject   m_LightObj;
    public GameObject   m_GetAnimObj;
    public UISprite     m_LightBg;
    public Animator     m_RewardAnim;
    public uint         m_NodeID;
    public uint         m_RewardID;
    public bool         m_bGetReward;       //能否领取奖励
}

struct ActivityDetailInfo
{
    public GameObject       m_DetailObj;
    public UILabel          m_DetailLabel;  //活动祥情说明
}

struct ActivityBtn
{
    public GameObject   m_BtnObj;
    public UISprite     m_BtnBg;
    public byte         m_Tag;
    public bool         m_IsChecked;
}

public class HallLoginUI_Activity : HallLoginUI_BaseWind
{
    GameObject[]                m_ActivityTitle = new GameObject[(byte)ActivityType.ActivityMax_Num]; 
    GameObject[]                m_ActivityChildWndObj = new GameObject[(byte)ActivityType.ActivityMax_Num];
    ActivityBtn[]               m_ActivityBtn = new ActivityBtn[(byte)ActivityType.ActivityMax_Num];
    ActivityDetailInfo          m_ActivityDetail = new ActivityDetailInfo();
    ActivityLevel               m_ActivityLevelWnd = new ActivityLevel();
    ActivityPayWnd              m_ActivityPayWnd = new ActivityPayWnd();
    ActivityTimeLimitWnd        m_ActivityTimeLimitWnd = new ActivityTimeLimitWnd();    //限时通用活动
    HallLoinUI_SignIn           m_SignInWnd = new HallLoinUI_SignIn();
    GameObject[]                m_WarningObj = new GameObject[4];           //提示红点

    ActivityType                m_ActivityType;
    bool                        m_bTimeLimitExist = false;   //限时活动是否存在

    public UnityEngine.Object Init()
    {
        UnityEngine.Object obj = ResManager.Instance.LoadObject("ActivityWnd", "GlobalRes/Activity/", ResType.GlobalRes);
        Obj = obj;
        WindID.m_state = HallLogicUIStatue.Hall_State.Hall_Activity;
        WindID.m_SmailWind = 0;
        return obj;
    }
    public void InitGameObject()
    {
        if (WindObj == null)
        {
            WindObj = GameObject.Instantiate(Obj) as GameObject;
            BaseTranF = WindObj.transform;
            BaseTranF.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
            WindObj.SetActive(true);
            WndManager.Instance.Push(WindObj);
        }
        m_ActivityDetail.m_DetailObj = BaseTranF.GetChild(2).gameObject;
        m_ActivityDetail.m_DetailLabel = BaseTranF.GetChild(2).GetComponent<UILabel>();
        GetBtnLister(BaseTranF.GetChild(1).gameObject).onClick = OnClickActivityDetail;
        GetBtnLister(BaseTranF.GetChild(8).gameObject).onClick = OnClickCloseWind;

        for (byte i = 0; i < (byte)ActivityType.ActivityMax_Num; ++i )
        {
            m_ActivityTitle[i] = BaseTranF.GetChild(0).GetChild(i).gameObject;

            m_ActivityBtn[i].m_BtnObj = BaseTranF.GetChild(3).GetChild(i).gameObject;
            m_ActivityBtn[i].m_BtnBg = BaseTranF.GetChild(3).GetChild(i).GetChild(0).GetComponent<UISprite>();
            m_ActivityBtn[i].m_Tag = i;
            if (i == (byte)ActivityType.LevelActivity_UI)
                m_ActivityBtn[i].m_IsChecked = true;
            else
                m_ActivityBtn[i].m_IsChecked = false;
            GetBtnLister(m_ActivityBtn[i].m_BtnObj).onClick = OnChangeActivityWnd;

            m_WarningObj[i] = BaseTranF.GetChild(3).GetChild(4 + i).gameObject;
        }
        m_bTimeLimitExist = m_ActivityTimeLimitWnd.CheckActivityIsExist() ;
        SetInitalWnd();
        //ExchangeLeftBtnBg();
        InitActivity();
       // m_ActivityRewardWnd.Init(BaseTranF.GetChild(8).gameObject);
    }

    void InitActivity()
    {
        for (byte i = 0; i < (byte)ActivityType.ActivityMax_Num; ++i)
        {
            m_ActivityChildWndObj[i] = BaseTranF.GetChild(4 + i).gameObject;
        }
        //m_ActivityChildWndObj[(byte)m_ActivityType].SetActive(true);
        //初始化签到活动
        m_SignInWnd.Init(m_ActivityChildWndObj[(byte)ActivityType.SignIn_UI]);
        //初始化等级礼包活动
        m_ActivityLevelWnd.Init(m_ActivityChildWndObj[(byte)ActivityType.LevelActivity_UI]);
        //初始化充值好礼活动
        m_ActivityPayWnd.Init(m_ActivityChildWndObj[(byte)ActivityType.PayActivity_UI]);
        //初始化通用活动
        if (m_bTimeLimitExist)
        {
            m_ActivityBtn[(byte)ActivityType.TimeLimitActivity_UI].m_BtnObj.SetActive(true);
            m_ActivityTimeLimitWnd.Init(m_ActivityChildWndObj[(byte)ActivityType.TimeLimitActivity_UI]);
        }
        else
            m_ActivityBtn[(byte)ActivityType.TimeLimitActivity_UI].m_BtnObj.SetActive(false);

        m_ActivityTitle[(byte)m_ActivityType].SetActive(true);
        OnChangeActivityWnd(m_ActivityBtn[(byte)m_ActivityType].m_BtnObj);
    }
    void SetInitalWnd()
    {
        bool bCheck = IsCanCheck();
        bool bLevelReward = IsLevelReward();
        bool bPayReward = IsPayReward();
        bool bLimitTimeReward = IsTimeLimitReward();
        if (bCheck)
            m_ActivityType = ActivityType.SignIn_UI;
        else if (bLevelReward)
            m_ActivityType = ActivityType.LevelActivity_UI;
        else if (bPayReward)
            m_ActivityType = ActivityType.PayActivity_UI;
        else if (bLimitTimeReward)
            m_ActivityType = ActivityType.TimeLimitActivity_UI;
        else
            m_ActivityType = ActivityType.SignIn_UI;
    }
    public void Update(float dTime)
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;
        switch (m_ActivityType)
        {
            case ActivityType.SignIn_UI:
                m_SignInWnd.Updtae(dTime);
                break;
            case ActivityType.LevelActivity_UI:
                m_ActivityLevelWnd.Update(dTime);
                break;
            case ActivityType.PayActivity_UI:
                m_ActivityPayWnd.Update(dTime);
                break;
            case ActivityType.TimeLimitActivity_UI:
                break;
        }
    }
    public void UpdateActivityData()
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;
        UpdateWndUIData(m_ActivityType);

    }
    public void UpdateSingleActivityData(byte ActivityID)
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        if (ActivityID == m_ActivityLevelWnd.ActivityID)
        {
            m_ActivityLevelWnd.UpdateActivityLevelData();
        }
        else if (ActivityID == m_ActivityPayWnd.ActivityID)
        {
            IsPayReward();
            m_ActivityPayWnd.UpdatePayActivityData();
        }
        else if (ActivityID == m_ActivityTimeLimitWnd.ActivityID)
        {
            IsTimeLimitReward();
            m_ActivityTimeLimitWnd.UpdateLimitTimeActivityData();
        }
    }
    public void SetActivityRewardEft(byte ActivityID, byte NodeID, ushort RewardID)
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        if (ActivityID == m_ActivityLevelWnd.ActivityID)
        {
            m_ActivityLevelWnd.SetActivityRewardEft(ActivityID, NodeID);
            IsLevelReward();
        }
        else if (ActivityID == m_ActivityPayWnd.ActivityID)
        {
            IsPayReward();
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Activity_Reward_Success"), 1, false);
        }
        else if (ActivityID == m_ActivityTimeLimitWnd.ActivityID)
        {
            IsTimeLimitReward();
            GlobalHallUIMgr.Instance.ShowTipBox_0(TipBox_0.TipBox_0_ZiTi.Activity, 2, RewardID);
        }
    }
    void OnClickActivityDetail(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void OnClickCloseWind(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        WndManager.Instance.Pop(WindObj);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        HallRunTimeInfo.Login_UI.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Mian);
        m_ActivityLevelWnd.SaveSubmrainePos();
    }
    void OnChangeActivityWnd(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        byte Tag = 0;
        for (byte i = 0; i < (byte)ActivityType.ActivityMax_Num; ++i)
        {
            if (go == m_ActivityBtn[i].m_BtnObj)
            {
                Tag = m_ActivityBtn[i].m_Tag;
                m_ActivityBtn[i].m_IsChecked = true;
                m_ActivityChildWndObj[i].SetActive(true);
                m_ActivityTitle[i].SetActive(true);
            }
            else
            {
                m_ActivityBtn[i].m_IsChecked = false;
                m_ActivityChildWndObj[i].SetActive(false);
                m_ActivityTitle[i].SetActive(false);
            }
        }
        ExchangeLeftBtnBg();
        switch (Tag)
        {
            case (byte)ActivityType.SignIn_UI:         //签到按纽   
                m_ActivityType = ActivityType.SignIn_UI;
               // m_SignInWnd.SetParent(this);
                m_SignInWnd.UpdateSignInData();
                break;
            case (byte)ActivityType.LevelActivity_UI:         //等级礼包
                m_ActivityType = ActivityType.LevelActivity_UI;
                m_ActivityLevelWnd.ActivityID = 1;
                m_ActivityLevelWnd.ShowActivityLevelWnd();
                break;
            case (byte)ActivityType.PayActivity_UI:         //充值礼包
                m_ActivityType = ActivityType.PayActivity_UI;
                m_ActivityPayWnd.ActivityID = 2;
                m_ActivityPayWnd.UpdatePayActivityData();
                break;
            case (byte)ActivityType.TimeLimitActivity_UI:
                {
                    m_ActivityType = ActivityType.PayActivity_UI;
                    m_ActivityTimeLimitWnd.UpdateLimitTimeActivityData();
                    break;
                }
        }

    }
    void ExchangeLeftBtnBg()
    {
        for (byte i = 0; i < (byte)ActivityType.ActivityMax_Num; ++i)
        {
            if (m_ActivityBtn[i].m_IsChecked)
                m_ActivityBtn[i].m_BtnBg.spriteName = "Activity_SelectBtn_Bg0";
            else
                m_ActivityBtn[i].m_BtnBg.spriteName = "Activity_SelectBtn_Bg1";
        }
    }
    void UpdateWndUIData(ActivityType statue)
    {
        //IsLevelReward();
        //IsPayReward();
        m_bTimeLimitExist = m_ActivityTimeLimitWnd.CheckActivityIsExist();
        if (m_bTimeLimitExist)
        {
            if (m_ActivityBtn[(byte)ActivityType.TimeLimitActivity_UI].m_BtnObj.activeSelf == false)
            {
                m_ActivityBtn[(byte)ActivityType.TimeLimitActivity_UI].m_BtnObj.SetActive(true);
                m_ActivityTimeLimitWnd.Init(m_ActivityChildWndObj[(byte)ActivityType.TimeLimitActivity_UI]);
            }
        }
        
        SetInitalWnd();
        m_ActivityTitle[(byte)m_ActivityType].SetActive(true);
        OnChangeActivityWnd(m_ActivityBtn[(byte)m_ActivityType].m_BtnObj);

        switch (statue)
        {
            case ActivityType.SignIn_UI:
                break;
            case ActivityType.LevelActivity_UI:
                m_ActivityLevelWnd.UpdateActivityLevelData();
                break;
            case ActivityType.PayActivity_UI:
                m_ActivityPayWnd.UpdatePayActivityData();
                break;
            case ActivityType.TimeLimitActivity_UI:
                m_ActivityTimeLimitWnd.UpdateLimitTimeActivityData();
                break;
        }
    }
    public void UpdateSignInData()
    {
        m_SignInWnd.UpdateSignInData();
        if (PlayerRole.Instance.CheckManager.IsCanCheckNowDay())
        {
            m_WarningObj[0].SetActive(true);
        }
        else
            m_WarningObj[0].SetActive(false);
    }
    bool IsCanCheck()
    {
        bool bSignIn = false;

        if (PlayerRole.Instance.CheckManager.IsCanCheckNowDay())
        {
            m_WarningObj[0].SetActive(true);
            return true;
        }
        return false;
    }
    bool IsLevelReward()
    {
        if (PlayerRole.Instance.ActionManager.GetActionArray() != null)
        {
                if (PlayerRole.Instance.ActionManager.GetAction(1) != null)
                {
                    RoleActionBase pActivityMap = PlayerRole.Instance.ActionManager.GetAction(1);
                    foreach (KeyValuePair<uint, tagActionEventConfig> map in pActivityMap.pConfig.EventMap)
                    {
                        
                        if ((map.Value.ID & pActivityMap.pActionInfo.ActionStates) == 0)
                        {
                            //该活动结点达到完成条件可以领取奖励了
                            if (pActivityMap.pActionInfo.ActionValue >= map.Value.FinishValue)
                            {
                                m_WarningObj[1].SetActive(true);
                                return true;
                            }
                        }
                    }
                }
        }
        m_WarningObj[1].SetActive(false);
        return false;
    }
    bool IsPayReward()
    {
        if (PlayerRole.Instance.ActionManager.GetActionArray() != null)
        {
            if (PlayerRole.Instance.ActionManager.GetAction(2) != null)
            {
                RoleActionBase pActivityMap = PlayerRole.Instance.ActionManager.GetAction(2);
                foreach (KeyValuePair<uint, tagActionEventConfig> map in pActivityMap.pConfig.EventMap)
                {

                    if ((map.Value.ID & pActivityMap.pActionInfo.ActionStates) == 0)
                    {
                        //该活动结点达到完成条件可以领取奖励了
                        if (pActivityMap.pActionInfo.ActionValue >= map.Value.FinishValue)
                        {
                            m_WarningObj[2].SetActive(true);
                            return true;
                        }
                    }
                }
            }
        }
        m_WarningObj[2].SetActive(false);
        return false;
    }

    bool IsTimeLimitReward()
    {
        if (!m_bTimeLimitExist)
            return false;
        if (PlayerRole.Instance.ActionManager.GetActionArray() != null)
        {
            if (PlayerRole.Instance.ActionManager.GetAction(m_ActivityTimeLimitWnd.ActivityID) != null)
            {
                RoleActionBase pActivityMap = PlayerRole.Instance.ActionManager.GetAction(m_ActivityTimeLimitWnd.ActivityID);
                foreach (KeyValuePair<uint, tagActionEventConfig> map in pActivityMap.pConfig.EventMap)
                {

                    if ((map.Value.ID & pActivityMap.pActionInfo.ActionStates) == 0)
                    {
                        //该活动结点达到完成条件可以领取奖励了
                        if (pActivityMap.pActionInfo.ActionValue >= map.Value.FinishValue)
                        {
                            m_WarningObj[(byte)ActivityType.TimeLimitActivity_UI].SetActive(true);
                            return true;
                        }
                    }
                }
            }
        }
        m_WarningObj[(byte)ActivityType.TimeLimitActivity_UI].SetActive(false);
        return false;
    }
    public void ShutDownActivity()
    {
        if (WindObj != null)
        {
            WndManager.Instance.Pop(WindObj);
            GameObject.Destroy(WindObj);
        }
        WindObj = null;
        BaseTranF = null;
    }
}

class ActivityLevel : BaseWnd
{
    NodeItem[]              m_LevelNodeItem = new NodeItem[4];      //等级结点
    Vector3[]               m_PathPos = new Vector3[20];                //路径位置点
    Animator                m_SubmarineAnim;                            //潜艇动画
    GameObject              m_SubmarineIdelAnim;                        //潜艇待机动画
    GameObject              m_RewardEftObj;
    GameObject[]            m_LevelNodeEft = new GameObject[4];
    UISprite[]              m_LevelBgEft = new UISprite[4];             //
    byte                    m_ActivityID;
    byte                    m_SubmarinePos;                             //潜艇位置
    float                   m_NormalizedTime;                           //潜艇动画起始时间
    float                   m_DelayedInterval;                          //延时频率
    float                   m_Time;
    float                   m_AnimTime;
    float                   m_AnimLastTime;
    bool                    m_bDelayed;                                 //是否延时播放潜艇动画
    bool                    m_bPlayAnim;
    bool                    m_bError = false;
    byte                    m_EftIdx;                                   //特效下标
    float                   m_EftTime;
    float                   m_EftFillTime;                              //背景变化时间

    public void Init(GameObject go)
    {
        m_ActivityID = (byte)ActivityType.LevelActivity_UI;

        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;
        m_SubmarineAnim = m_BaseTrans.GetChild(4).GetComponent<Animator>();
        m_SubmarineIdelAnim = m_BaseTrans.GetChild(4).GetChild(0).gameObject;
        for (byte i = 0; i < m_LevelNodeItem.Length; ++i)
        {
            m_LevelNodeItem[i].m_BaseObj = m_BaseTrans.GetChild(i).gameObject;
            m_LevelNodeItem[i].m_LevelNodeBtn = m_BaseTrans.GetChild(i).GetComponent<UIButton>();
            m_LevelNodeItem[i].m_LevelLabel = m_BaseTrans.GetChild(i).GetChild(0).GetChild(0).GetComponent<UILabel>();
            m_LevelNodeItem[i].m_LevelBg = m_BaseTrans.GetChild(i).GetChild(0).GetComponent<UISprite>();
            m_LevelNodeItem[i].m_RewardAnim = m_BaseTrans.GetChild(i).GetChild(2).GetComponent<Animator>();
            m_LevelNodeItem[i].m_LightObj = m_BaseTrans.GetChild(i).GetChild(1).gameObject;
            UIEventListener.Get(m_LevelNodeItem[i].m_BaseObj).onClick = OnClickLevelNode;
        }

        m_RewardEftObj = m_BaseTrans.GetChild(5).gameObject;
        for (byte i = 0; i < m_LevelNodeEft.Length; ++i )
        {
            m_LevelNodeEft[i] = m_BaseTrans.GetChild(5).GetChild(i).gameObject;
            m_LevelBgEft[i] = m_LevelNodeEft[i].transform.GetChild(1).GetComponent<UISprite>();
        }
            //for (byte i = 0; i < m_PathPos.Length; ++i )
            //{
            //    m_PathPos[i] = m_BaseTrans.GetChild(5).GetChild(i).localPosition;
            //}

            //  ShowActivityLevelWnd();
            m_DelayedInterval = 1.0f;
        m_bDelayed = false;
        m_bPlayAnim = false;

        if (LoadSubmarinePos() == false)
            m_SubmarinePos = 0;
    }
    void PlaySubmarineAnim(float speed)
    {
        if (speed > 0)
            m_SubmarineIdelAnim.SetActive(false);
        else
            m_SubmarineIdelAnim.SetActive(true);

        int state = Animator.StringToHash("activeBoat_Anim");
        m_SubmarineAnim.Play(state, 0, m_NormalizedTime);
        m_SubmarineAnim.speed = speed;
    }
    public void ShowActivityLevelWnd()
    {
        m_Time = 0;
        
        m_NormalizedTime = m_SubmarinePos * 0.05f;       //潜艇起始位置时间

        if (m_SubmarinePos >= 0)
        {
            PlaySubmarineAnim(0.0f);
        }
        if (  (m_SubmarinePos < PlayerRole.Instance.RoleInfo.RoleMe.GetLevel() ) && (m_SubmarinePos < 20) )
        {
            m_bPlayAnim = true;
            m_AnimTime = 0;
            m_AnimLastTime = (PlayerRole.Instance.RoleInfo.RoleMe.GetLevel() - m_SubmarinePos) * 0.2f;
            m_bDelayed = true;
            m_SubmarinePos = (byte)PlayerRole.Instance.RoleInfo.RoleMe.GetLevel();
        }
        UpdateActivityLevelData();
    }
    public void Update(float deltaTime)
    { 
        if (m_bDelayed)
        {
            m_Time += deltaTime;
            if (m_Time >= m_DelayedInterval)
            {
                PlaySubmarineAnim(1.0f);
                m_bDelayed = false;
                m_Time = 0;
            }
            return;
        }
        if (m_bPlayAnim)
        {
            m_AnimTime += deltaTime;
            if (m_AnimTime >= m_AnimLastTime)
            {
                m_bPlayAnim = false;
                m_SubmarineAnim.speed = 0;
                m_SubmarineIdelAnim.SetActive(true);
            }
        }
        if (m_EftTime > 0)
        {
            m_EftTime -= deltaTime;
            if (m_EftTime <= 0)
            {
                m_EftTime = 0;
                //m_LevelNodeItem[m_EftIdx].m_LevelNodeBtn.gameObject.SetActive(true);
                m_RewardEftObj.SetActive(false);
                m_LevelNodeEft[m_EftIdx].SetActive(false);
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Activity_Reward_Success"), 1.0f, false);
                WndManager.Instance.Pop(m_RewardEftObj);

            }
            m_EftFillTime += deltaTime;
            if (m_EftFillTime > 0.2f && m_EftFillTime <= 0.6f)
            {
                m_LevelBgEft[m_EftIdx].fillAmount = (m_EftFillTime - 0.2f) / 0.4f;
            }
            else
            {
                m_LevelNodeItem[m_EftIdx].m_LevelNodeBtn.gameObject.SetActive(true);
                m_LevelBgEft[m_EftIdx].fillAmount = 1;
            }

            if (m_EftFillTime > 1.7f)
                m_LevelNodeItem[m_EftIdx].m_LightObj.SetActive(true);

        }
    }
    public void UpdateActivityLevelData()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        if (int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetActionStates(), m_ActivityID))
        {
            //当前活动已经完成了
           tagActionConfig pActivityMap =  FishConfig.Instance.m_ActionInfo.m_ActionMap[m_ActivityID];
           //活动结束了
            if (!pActivityMap.IsInTime())
           {
               m_BaseWndObject.SetActive(false);
               return;
           }
            ShowFinshActivity(pActivityMap);
            m_bError = false;
        }
        else  if (PlayerRole.Instance.ActionManager.GetActionArray() != null)
        {
            if (PlayerRole.Instance.ActionManager.GetAction(m_ActivityID) != null)
            {
                RoleActionBase pActivityMap = PlayerRole.Instance.ActionManager.GetAction(m_ActivityID);
                byte Indx = 0;
                m_bError = false;
                foreach (KeyValuePair<uint, tagActionEventConfig> map in pActivityMap.pConfig.EventMap)
                {
                    //该结点任务完成既领取了
                    if ((map.Value.ID & pActivityMap.pActionInfo.ActionStates) != 0)
                    {
                        m_LevelNodeItem[Indx].m_LightObj.SetActive(true);
                        m_LevelNodeItem[Indx].m_LevelBg.spriteName = "Level_Bg1";
                        m_LevelNodeItem[Indx].m_LevelLabel.text = map.Value.FinishValue.ToString() + "  级";
                        m_LevelNodeItem[Indx].m_LevelNodeBtn.isEnabled = false;
                        string str = "Activity_Level" + map.Value.FinishValue.ToString() + "_Btn0";
                        ChangeButtonBg(m_LevelNodeItem[Indx].m_LevelNodeBtn, str);
                        m_LevelNodeItem[Indx].m_bGetReward = false;
                        m_LevelNodeItem[Indx].m_NodeID = map.Value.ID;
                        m_LevelNodeItem[Indx].m_RewardAnim.SetBool("Reward", false);
                    }
                    else
                    {
                        //该活动结点达到完成条件可以领取奖励了
                        if (pActivityMap.pActionInfo.ActionValue >= map.Value.FinishValue)
                        {
                            string str = "Activity_Level" + map.Value.FinishValue.ToString() + "_Btn0";
                            ChangeButtonBg(m_LevelNodeItem[Indx].m_LevelNodeBtn, str);
                            m_LevelNodeItem[Indx].m_NodeID = map.Value.ID;
                            m_LevelNodeItem[Indx].m_bGetReward = true;
                            m_LevelNodeItem[Indx].m_RewardID = map.Value.RewardID;
                            m_LevelNodeItem[Indx].m_LevelLabel.text = map.Value.FinishValue.ToString() + "  级";
                            m_LevelNodeItem[Indx].m_RewardAnim.SetBool("Reward", true);
                        }
                        else
                        {
                            m_LevelNodeItem[Indx].m_bGetReward = false;
                            m_LevelNodeItem[Indx].m_RewardID = map.Value.RewardID;
                            m_LevelNodeItem[Indx].m_LevelLabel.text = map.Value.FinishValue.ToString() + "  级";
                        }
                    }
                    ++Indx;
                }
            }
            else
                m_bError = true;

        }
        else
        {
            //错误情况
            m_bError = true;
           // m_BaseWndObject.SetActive(false);

        }
    }
    public void SetActivityRewardEft(byte ActivityID, byte NodeID)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        for (byte i = 0; i < m_LevelNodeItem.Length; ++i)
        {
            if(m_LevelNodeItem[i].m_NodeID == NodeID)
            {
                m_LevelNodeItem[i].m_LightObj.SetActive(false);
                m_LevelNodeItem[i].m_LevelNodeBtn.gameObject.SetActive(false);
                m_RewardEftObj.SetActive(true);
                m_LevelNodeEft[i].SetActive(true);
                m_EftIdx = i;
                m_EftTime = 2.0f;
                m_EftFillTime = 0;
                WndManager.Instance.Push(m_RewardEftObj);
                break;
            }
        }
    }
    void OnClickLevelNode(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (m_bError)
            return;
        for (byte i = 0; i < m_LevelNodeItem.Length; ++i)
        {
            if (m_LevelNodeItem[i].m_BaseObj == go)
            {
                //调用可以领取窗口
                if (m_LevelNodeItem[i].m_bGetReward)
                {
                    GlobalHallUIMgr.Instance.GetRewardWnd(m_LevelNodeItem[i].m_RewardID, m_ActivityID, m_LevelNodeItem[i].m_NodeID, ActivityType.LevelActivity_UI);
                }
                else
                {
                    GlobalHallUIMgr.Instance.PreviewRewardWnd(m_LevelNodeItem[i].m_RewardID, ActivityType.LevelActivity_UI);
                }
            }
        }
    }
    void ShowFinshActivity(tagActionConfig pMap)
    {
        byte Indx = 0;
        foreach(KeyValuePair<uint , tagActionEventConfig> map in pMap.EventMap)
        {
            m_LevelNodeItem[Indx].m_LevelNodeBtn.isEnabled = false;
            m_LevelNodeItem[Indx].m_LevelBg.spriteName = "Level_Bg1";
            m_LevelNodeItem[Indx].m_LightObj.SetActive(true);
            m_LevelNodeItem[Indx].m_LevelLabel.text = map.Value.FinishValue.ToString()+"  级";
            switch (Indx)
            {
                case 0:
                    ChangeButtonBg(m_LevelNodeItem[Indx].m_LevelNodeBtn, "Activity_Level5_Btn0");
                    break;
                case 1:
                    ChangeButtonBg(m_LevelNodeItem[Indx].m_LevelNodeBtn, "Activity_Level10_Btn0");
                    break;
                case 2:
                    ChangeButtonBg(m_LevelNodeItem[Indx].m_LevelNodeBtn, "Activity_Level15_Btn0");
                    break;
                case 3:
                    ChangeButtonBg(m_LevelNodeItem[Indx].m_LevelNodeBtn, "Activity_Level20_Btn0");
                    break;
            }
            ++Indx;
        }
    }
    void ChangeButtonBg(UIButton Btn, string spriteName)
    {
        Btn.normalSprite = spriteName;
        Btn.hoverSprite = spriteName;
        Btn.pressedSprite = spriteName;
        Btn.disabledSprite = spriteName;
    }
    public bool LoadSubmarinePos()
    {
        //读取SD卡的潜艇位置
        byte[] data = RuntimeInfo.GetLocalFile(RuntimeInfo.GetSubmarinePosFileName());
        if (data == null)
            return false;

        System.IO.MemoryStream ms = new System.IO.MemoryStream(data);
        System.IO.BinaryReader br = new System.IO.BinaryReader(ms);
        m_SubmarinePos = br.ReadByte();
        uint UserID = br.ReadUInt32();
        //m_SubmarinePos = data[0];

        if (UserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            return false;
        return true;
    }
    //位置保存到SDK里
    public void SaveSubmrainePos()
    {
        //byte data;
        System.IO.MemoryStream ms = new System.IO.MemoryStream();
        System.IO.BinaryWriter bw = new System.IO.BinaryWriter(ms);
        bw.Write(m_SubmarinePos);
        bw.Write(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
        RuntimeInfo.SaveLocalFile(RuntimeInfo.GetSubmarinePosFileName(), ms.ToArray());
        bw.Close();
    }

    public byte ActivityID
    {
        get { return m_ActivityID; }
        set { m_ActivityID = value; }
    }
}

class ActivityPayWnd : BaseWnd
{
    NodeItem[]              m_PayNodeItem = new NodeItem[5];
    GameObject              m_GetRewardEftObj;
    GameObject[]            m_RewardEft = new GameObject[5];
    byte                    m_ActivityID;
    byte                    m_NodeIndx;         //点击时记录结点下标
    float                   m_EftTime = 0;
    float                   m_EftEndTime = 0;

    public void Init(GameObject go)
    {
        m_ActivityID = (byte)ActivityType.PayActivity_UI;

        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;

        for (byte i = 0; i < m_PayNodeItem.Length; ++i)
        {
            m_PayNodeItem[i].m_BaseObj = m_BaseTrans.GetChild(i).gameObject;
            m_PayNodeItem[i].m_LevelNodeBtn = m_BaseTrans.GetChild(i).GetComponent<UIButton>();
            m_PayNodeItem[i].m_LevelNodeBg = m_BaseTrans.GetChild(i).GetComponent<UISprite>();
            m_PayNodeItem[i].m_LevelLabel = m_BaseTrans.GetChild(i).GetChild(0).GetComponent<UILabel>();
            m_PayNodeItem[i].m_LevelBg = m_BaseTrans.GetChild(i).GetChild(0).GetChild(0).GetComponent<UISprite>();
            m_PayNodeItem[i].m_LightBg = m_BaseTrans.GetChild(i).GetChild(1).GetComponent<UISprite>();
            m_PayNodeItem[i].m_GetAnimObj = m_BaseTrans.GetChild(i).GetChild(2).gameObject;
            UIEventListener.Get(m_PayNodeItem[i].m_BaseObj).onClick = OnClickReward;

            m_RewardEft[i] = m_BaseTrans.GetChild(6).GetChild(i).gameObject;
        }
        UIEventListener.Get(m_BaseTrans.GetChild(5).gameObject).onClick = OnClickGotoPay;

        m_GetRewardEftObj = m_BaseTrans.GetChild(6).gameObject;
    }
    public void Update(float deltaTime)
    {
        if (m_EftTime > 0)
        {
            m_EftTime -= deltaTime;
            if (m_EftTime <= 0)
            {
                m_EftTime = 0;
                m_GetRewardEftObj.SetActive(false);
                m_RewardEft[m_NodeIndx].SetActive(false);
            }
        }
        if (m_EftEndTime > 0)
        {
            m_EftEndTime -= deltaTime;

            if (m_EftEndTime <= 0)
            {
                m_EftEndTime = 0;
                ChangeButtonBg(m_PayNodeItem[m_NodeIndx].m_LevelNodeBtn, "Activity_Pay_Chests1");
                WndManager.Instance.Pop(m_GetRewardEftObj);
                GlobalHallUIMgr.Instance.GetRewardWnd(m_PayNodeItem[m_NodeIndx].m_RewardID, m_ActivityID,
                    m_PayNodeItem[m_NodeIndx].m_NodeID, ActivityType.PayActivity_UI);
            }
        }
    }
    public void UpdatePayActivityData()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetActionStates(), m_ActivityID))
        {
            //当前活动已经完成了
            tagActionConfig pActivityMap = FishConfig.Instance.m_ActionInfo.m_ActionMap[m_ActivityID];
            //活动结束了
            if (!pActivityMap.IsInTime())
            {
                m_BaseWndObject.SetActive(false);
                return;
            }
            byte Indx = 0;
            foreach(KeyValuePair<uint, tagActionEventConfig>map in pActivityMap.EventMap)
            {
                SetNodeState(Indx, map.Value.RewardID, map.Value.ID, map.Value.FinishValue);
                Indx++;
            }
        }
        else if (PlayerRole.Instance.ActionManager.GetActionArray() != null)
        {
            if (PlayerRole.Instance.ActionManager.GetAction(m_ActivityID) != null)
            {
                RoleActionBase pActivityMap = PlayerRole.Instance.ActionManager.GetAction(m_ActivityID);
                byte Indx = 0;
                bool bGetReward = false;
                foreach (KeyValuePair<uint, tagActionEventConfig> map in pActivityMap.pConfig.EventMap)
                {
                    //该结点任务完成既领取了
                    if ((map.Value.ID & pActivityMap.pActionInfo.ActionStates) != 0)
                    {
                        if (m_PayNodeItem[Indx].m_GetAnimObj.activeSelf)
                            m_PayNodeItem[Indx].m_GetAnimObj.SetActive(false);
                        SetNodeState(Indx, map.Value.RewardID, map.Value.ID, map.Value.FinishValue);
                    }
                    else
                    {
                        //该活动结点达到完成条件可以领取奖励了
                        if (pActivityMap.pActionInfo.ActionValue >= map.Value.FinishValue)
                        {
                            m_PayNodeItem[Indx].m_LevelLabel.text = map.Value.FinishValue.ToString() + " RMB";
                            m_PayNodeItem[Indx].m_LevelBg.spriteName = "Activity_Pay_NumBg0";
                            m_PayNodeItem[Indx].m_LightBg.spriteName = "Activity_Pay_Nod1";
                            ChangeButtonBg(m_PayNodeItem[Indx].m_LevelNodeBtn, "123");
                            m_PayNodeItem[Indx].m_NodeID = map.Value.ID;
                            m_PayNodeItem[Indx].m_RewardID = map.Value.RewardID;
                            m_PayNodeItem[Indx].m_bGetReward = true;
                            bGetReward = true;
                          //  m_PayNodeItem[Indx].m_LevelNodeBg.spriteName = "123";
                            m_PayNodeItem[Indx].m_GetAnimObj.SetActive(true);
                        }
                        else
                        {
                            m_PayNodeItem[Indx].m_LevelLabel.text = map.Value.FinishValue.ToString() + " RMB";
                            m_PayNodeItem[Indx].m_LevelBg.spriteName = "Activity_Pay_NumBg0";
                            ChangeButtonBg(m_PayNodeItem[Indx].m_LevelNodeBtn, "Activity_Pay_Chests0");
                            m_PayNodeItem[Indx].m_NodeID = map.Value.ID;
                            m_PayNodeItem[Indx].m_RewardID = map.Value.RewardID;
                            m_PayNodeItem[Indx].m_bGetReward = false;

                            //if (!bGetReward)
                            //{
                            //    bGetReward = true;
                            //    m_PayNodeItem[Indx].m_LightBg.spriteName = "Activity_Pay_Nod1";

                            //}
                            //else
                            //    m_PayNodeItem[Indx].m_LightBg.spriteName = "Activity_Pay_Nod0";

                        }
                    }
                    ++Indx;
                }
            }
        }
        else
        {
            //错误情况
           // m_BaseWndObject.SetActive(false);
        }
    }
    void SetNodeState(byte Indx, uint RewardID, uint NodeID, uint FinishValue)
    {
        m_PayNodeItem[Indx].m_LevelNodeBtn.isEnabled = false;
        m_PayNodeItem[Indx].m_LevelLabel.text = FinishValue.ToString() + " RMB";
        m_PayNodeItem[Indx].m_LevelBg.spriteName = "Activity_Pay_NumBg1";
        m_PayNodeItem[Indx].m_LightBg.spriteName = "Activity_Pay_Nod0";
        ChangeButtonBg(m_PayNodeItem[Indx].m_LevelNodeBtn, "Activity_Pay_Chests2");
        m_PayNodeItem[Indx].m_NodeID = NodeID;
        m_PayNodeItem[Indx].m_RewardID = RewardID;
        m_PayNodeItem[Indx].m_bGetReward = false;
    }
    void OnClickReward(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        for (byte i = 0; i < m_PayNodeItem.Length; ++i)
        {
            if (m_PayNodeItem[i].m_BaseObj == go)
            {
                //调用可以领取窗口
                if (m_PayNodeItem[i].m_bGetReward)
                {
                    m_NodeIndx = i;
                    m_GetRewardEftObj.SetActive(true);
                    m_RewardEft[i].SetActive(true);
                    m_EftTime = 2.3f;
                    m_EftEndTime = 2f;
                   // m_PayNodeItem[i].m_LevelNodeBg.spriteName = "Activity_Pay_Chests0";
                   // ChangeButtonBg(m_PayNodeItem[m_NodeIndx].m_LevelNodeBtn, "Activity_Pay_Chests0");
                    m_PayNodeItem[i].m_GetAnimObj.SetActive(false);

                    WndManager.Instance.Push(m_GetRewardEftObj);

                   // GlobalHallUIMgr.Instance.GetRewardWnd(m_PayNodeItem[i].m_RewardID, m_ActivityID, m_PayNodeItem[i].m_NodeID, ActivityType.PayActivity_UI);
                }
                else
                {
                    GlobalHallUIMgr.Instance.PreviewRewardWnd(m_PayNodeItem[i].m_RewardID, ActivityType.PayActivity_UI);
                }
            }
        }
    }
    void OnClickGotoPay(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
    }
    void ChangeButtonBg(UIButton Btn, string spriteName)
    {
        Btn.normalSprite = spriteName;
        Btn.hoverSprite = spriteName;
        Btn.pressedSprite = spriteName;
        Btn.disabledSprite = spriteName;
    }
    public byte ActivityID
    {
        get { return m_ActivityID; }
        set { m_ActivityID = value; }
    }
}

//通用限时活动
public class ActivityTimeLimitWnd : BaseWnd
{
    GameObject              m_BaseItem;
    UIGrid                  m_UIGrid;
    List<LimitTimeItem>     m_NodeList = new List<LimitTimeItem>();
    byte                    m_ActivityID;

    public void Init(GameObject go)
    {
        if (m_BaseWndObject != null)
            return;

        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseItem = m_BaseTrans.GetChild(0).GetChild(0).gameObject;
        m_UIGrid = m_BaseTrans.GetChild(0).GetChild(1).GetComponent<UIGrid>();

    }
    public bool CheckActivityIsExist()
    {
        if (PlayerRole.Instance.ActionManager.GetActionArray() != null)
        {
            foreach(KeyValuePair<Byte, RoleActionBase>map in PlayerRole.Instance.ActionManager.GetActionArray())
            {
                if (map.Key > (byte)ActivityType.PayActivity_UI)
                {
                    m_ActivityID = map.Key;
                    return true;
                }
            }
        }
        foreach (KeyValuePair<byte, tagActionConfig> map in FishConfig.Instance.m_ActionInfo.m_ActionMap)
        {
            if (map.Key < (byte)ActivityType.TimeLimitActivity_UI)
                continue;
            if (int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetActionStates(), map.Key))
            {
                //当前活动已经完成了
                //活动结束了
                if (map.Value.IsInTime())
                {
                    m_ActivityID = map.Key;
                    return true;
                }
            }
        }

        return false;
    }
    void InitTimeLimitItem(tagActionEventConfig tagInfo, byte state, uint curValue = 0)
    {
        LimitTimeItem item = new LimitTimeItem();
        item.Init(m_BaseItem);
        item.SetItemInfo(m_ActivityID, tagInfo, state, curValue);
        m_NodeList.Add(item);
    }
    void AddGrid()
    {
        for (byte i = 0; i < m_NodeList.Count; ++i)
        {
            m_UIGrid.AddChild(m_NodeList[i].m_BaseTrans);
            m_NodeList[i].m_BaseTrans.localScale = Vector3.one;
        }
    }
    public void UpdateLimitTimeActivityData()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        ClearGrid();
        m_NodeList.Clear();
        if (int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetActionStates(), m_ActivityID))
        {
            //当前活动已经完成了
            tagActionConfig pActivityMap = FishConfig.Instance.m_ActionInfo.m_ActionMap[m_ActivityID];
            //活动结束了
            if (!pActivityMap.IsInTime())
            {
                m_BaseWndObject.SetActive(false);
                return;
            }
            foreach (KeyValuePair<uint, tagActionEventConfig> map in pActivityMap.EventMap)
            {
                InitTimeLimitItem(map.Value, 3);
            }
            AddGrid();
        }
        else if (PlayerRole.Instance.ActionManager.GetActionArray() != null)
        {
            if (PlayerRole.Instance.ActionManager.GetAction(m_ActivityID) != null)
            {
                RoleActionBase pActivityMap = PlayerRole.Instance.ActionManager.GetAction(m_ActivityID);
                foreach (KeyValuePair<uint, tagActionEventConfig> map in pActivityMap.pConfig.EventMap)
                {
                    //该结点任务完成既领取了
                    if ((map.Value.ID & pActivityMap.pActionInfo.ActionStates) != 0)
                    {
                        InitTimeLimitItem(map.Value, 3);
                    }
                    else
                    {
                        //该活动结点达到完成条件可以领取奖励了
                        if (pActivityMap.pActionInfo.ActionValue >= map.Value.FinishValue)
                        {
                            InitTimeLimitItem(map.Value, 1, map.Value.FinishValue);
                        }
                        else
                        {
                            InitTimeLimitItem(map.Value, 2, pActivityMap.pActionInfo.ActionValue);
                        }
                    }
                }
                m_NodeList.Sort(CompareItemBySate);
                AddGrid();
            }
        }
    }
    static int CompareItemBySate(LimitTimeItem item1, LimitTimeItem item2)
    {
        if (item1.NodeState < item2.NodeState)
            return -1;
        else if (item1.NodeState > item2.NodeState)
            return 1;
        else
        {
            if (item1.NodeID < item2.NodeID)
                return -1;
            else if (item1.NodeID > item2.NodeID)
                return 1;

            return 0;
        }
            

    }
    void ClearGrid()
    {
        List<Transform> gridChid = m_UIGrid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_UIGrid.transform.DetachChildren();
    }
    public byte ActivityID
    {
        get { return m_ActivityID; }
        set { m_ActivityID = value; }
    }

}
struct NodeReward
{
    public GameObject   m_Obj;
    public UILabel      m_RewardSum;
    public UISprite     m_RewardIcon;
}
class LimitTimeItem : BaseWnd
{
    UILabel             m_NodeName;
    UILabel             m_NodeProgress;
    UIButton            m_NodeReceiveBtn;
    NodeReward[]        m_NodeReward = new NodeReward[3];
    byte                m_NodeState;    //活动结点状态未完成‘2’完成未领取‘1’完成已经领取的‘3’
    byte                m_ActivityID;
    uint                m_NodeID;

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseWndObject.SetActive(true);

        m_NodeName = m_BaseTrans.GetChild(0).GetComponent<UILabel>();
        m_NodeProgress = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        m_NodeReceiveBtn = m_BaseTrans.GetChild(2).GetComponent<UIButton>();
        UIEventListener.Get(m_NodeReceiveBtn.gameObject).onClick = OnGetReward;
        for (byte i = 0; i < m_NodeReward.Length; ++i)
        {
            m_NodeReward[i].m_Obj = m_BaseTrans.GetChild(i + 3).gameObject;
            m_NodeReward[i].m_RewardSum = m_BaseTrans.GetChild(i + 3).GetChild(0).GetComponent<UILabel>();
            m_NodeReward[i].m_RewardIcon = m_BaseTrans.GetChild(i + 3).GetComponent<UISprite>();
        }
    }
    public void SetItemInfo(byte ActivityID, tagActionEventConfig tagInfo, byte state, uint curValue)
    {
        m_NodeState = state;
        m_ActivityID = ActivityID;
        m_NodeID = tagInfo.ID;
        m_NodeName.text = tagInfo.Common;
        if (state <= 2)
            m_NodeProgress.text = curValue.ToString() + "/" + tagInfo.FinishValue.ToString();
        else
            m_NodeProgress.text = tagInfo.FinishValue + "/" + tagInfo.FinishValue.ToString();
        tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID((ushort)tagInfo.RewardID);
        if (pReward == null)
            return;
        for (byte i = 0; i < pReward.RewardItemVec.Count; ++i)
        {
            uint ItemID = pReward.RewardItemVec[i].ItemID;
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                continue;
            if (i >= 3)
                break;
            m_NodeReward[i].m_RewardIcon.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon;
            m_NodeReward[i].m_RewardSum.text = "x" + pReward.RewardItemVec[i].ItemSum.ToString();
            m_NodeReward[i].m_Obj.SetActive(true);
        }
        ChangeBtnSate(state);
    }
    void ChangeBtnSate(byte state)
    {
        if (state == 1)
            m_NodeReceiveBtn.isEnabled = true;
        else 
            m_NodeReceiveBtn.isEnabled = false;

        string str = string.Format("Activity_TimeLimit_Btn{0}", state - 1);
        m_NodeReceiveBtn.normalSprite = str;
        m_NodeReceiveBtn.hoverSprite = str;
        m_NodeReceiveBtn.pressedSprite = str;
        m_NodeReceiveBtn.disabledSprite = str;
    }
    void OnGetReward(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        PlayerRole.Instance.ActionManager.GetActionReward((byte)m_ActivityID, m_NodeID);

    }
    public byte NodeState
    {
        get { return m_NodeState; }
    }
    public uint NodeID
    {
        get { return m_NodeID; }
    }
}

