using UnityEngine;
using System.Collections;

public class HallLoginUI_Btns : HallLoginUI_BaseWind
{
    UserRichInf[]   m_RichInf = new UserRichInf[3]; 
    WindBtn[]       m_BottomBtn = new WindBtn[6];
    GameObject[]    m_Warning = new GameObject[4];      //红点警示
    GameObject[]    m_BtnWindObj = new GameObject[3]; //0  Rich  1 Bottom 2 back
    GameObject[]    M_EffectObj = new GameObject[3];
    GameObject[]    m_VipBtn = new GameObject[3];       //VIP 和 月卡按纽 充值
    GameObject      m_MonthCardEftObj;
    Animator        m_MonthCardEftAnim;
    GameObject      m_RelaxBtn;
    public void Init()
    {
        WindID.m_state = HallLogicUIStatue.Hall_State.Hall_Btns;
        WindID.m_SmailWind = 0;
        Object obj = Initobj("HallWindBtns");
    }
    public void InitGameOnject()
    {
        Init_GameObj();
        ResManager.Instance.UnloadObject(Obj);
        InitRiches();
        InitBottomBtn();
        InitShopAndBackBtn();
        InitRelaxBtn();
    }
    void InitRiches()
    {
        Transform bseaT = BaseTranF.GetChild(0);
        m_BtnWindObj[0] = bseaT.gameObject;
        string[] EffectName = { "UIEfMedal", "UIEfDiamond", "UIEfGlodLight" };
        Object[] effect = GlobalEffectMgr.Instance.UIGoldEffect;
        for (byte i = 0; i < m_RichInf.Length; ++i)
        {
            if (i != 0)
            {
                m_RichInf[i].m_Btn.m_BtnObj = bseaT.GetChild(i).GetChild(2).gameObject;
                m_RichInf[i].m_Btn.m_Btn = m_RichInf[i].m_Btn.m_BtnObj.GetComponent<UIButton>();
                m_RichInf[i].m_Btn.m_IsChecked = false;
                m_RichInf[i].m_Btn.m_Tag = i;
                GetBtnLister(m_RichInf[i].m_Btn.m_BtnObj).onClick = OnClickRichBtns;
            }
            m_RichInf[i].m_GoldLabel = bseaT.GetChild(i).GetChild(1).GetComponent<UILabel>();
            Transform Effectparent = bseaT.GetChild(i).GetChild(0);
            if (i < 1)
                M_EffectObj[i] = (GameObject)GameObject.Instantiate(effect[2]);
            else if (i == 1)
            {
                M_EffectObj[i] = (GameObject)GameObject.Instantiate(effect[1]);
            }
            else
            {
                M_EffectObj[i] = (GameObject)GameObject.Instantiate(effect[0]);
            }
            if (M_EffectObj[i].transform.localPosition.x != 0) M_EffectObj[i].transform.localPosition = new Vector3(0, 0, 0);
            M_EffectObj[i].transform.SetParent(Effectparent, false);

        }
        UpdateUserGold();
    }
    void InitBottomBtn()
    {
        Transform baseT = BaseTranF.GetChild(1);
        m_BtnWindObj[1] = baseT.gameObject;
        for (byte i = 0; i < m_BottomBtn.Length; ++i)
        {
            if (i > 0 && i < 5)
                m_Warning[i - 1] = baseT.GetChild(i).GetChild(0).gameObject;
            m_BottomBtn[i].m_BtnObj = baseT.GetChild(i).gameObject;
            m_BottomBtn[i].m_Btn = m_BottomBtn[i].m_BtnObj.GetComponent<UIButton>();
            m_BottomBtn[i].m_IsChecked = false;
            m_BottomBtn[i].m_Tag = i;
            GetBtnLister(m_BottomBtn[i].m_BtnObj).onClick = OnChlickBottomBtns;
        }
       // IOS屏蔽兑换按纽
        if (ServerSetting.ShowExchange == false)
        {
            m_BottomBtn[0].m_BtnObj.transform.localPosition = m_BottomBtn[4].m_BtnObj.transform.localPosition;
            m_BottomBtn[4].m_BtnObj.transform.localPosition = m_BottomBtn[3].m_BtnObj.transform.localPosition;
            m_BottomBtn[3].m_BtnObj.SetActive(false);
        }
    }
    void InitShopAndBackBtn()
    {
        m_BtnWindObj[2] = BaseTranF.GetChild(3).gameObject;
        for (byte i = 0; i < m_VipBtn.Length; ++i )
        {
            m_VipBtn[i] = BaseTranF.GetChild(i + 4).gameObject;
        }
        m_MonthCardEftObj = BaseTranF.GetChild(5).GetChild(0).gameObject;
        m_MonthCardEftAnim = BaseTranF.GetChild(5).GetComponent<Animator>();

        GetBtnLister(BaseTranF.GetChild(2).gameObject).onClick = OnClickShop;
        GetBtnLister(BaseTranF.GetChild(3).gameObject).onClick = OnClickBack;
        GetBtnLister(m_VipBtn[0]).onClick = OnVip;
        GetBtnLister(m_VipBtn[1]).onClick = OnMonthCard;
        GetBtnLister(m_VipBtn[2]).onClick = OnPay;

    }
    void InitRelaxBtn()
    {
        m_RelaxBtn = BaseTranF.GetChild(7).gameObject;
        UIEventListener.Get(m_RelaxBtn).onClick = OnClickRelax;

        if (!ServerSetting.ShowGame)
        {
            m_RelaxBtn.SetActive(false);
        }
    }
    public void HideMonthCardEft()
    {
        if (WindObj == null)
            return;
       if (!PlayerRole.Instance.RoleMonthCard.IsCanGetRoleMonthCardReward())
       {
           m_MonthCardEftAnim.enabled = false;
           m_MonthCardEftObj.SetActive(false);
       }
    }
    void HideMonthCardBtnAndVIPBtn(bool bShow)
    {
        for (byte i = 0; i < m_VipBtn.Length; ++i)
        {
            m_VipBtn[i].SetActive(bShow);
        }
        if (ServerSetting.ShowGame)
            m_RelaxBtn.SetActive(bShow);
    }
    void OnClickRichBtns(GameObject go)
    {
        PlayBtnMusic();
        byte clickTag = 0;
        for (byte i = 0; i < m_RichInf.Length; ++i)
        {
            if (m_RichInf[i].m_Btn.m_BtnObj == go)
            {
                m_RichInf[i].m_Btn.m_IsChecked = true;
                clickTag = m_RichInf[i].m_Btn.m_Tag;
            }
            else
                m_RichInf[i].m_Btn.m_IsChecked = false;
        }
        switch (clickTag)
        { 
            case 0:
                break;
            case 1://点击钻石处的“+”
                {
                   // HallRunTimeInfo.Login_UI.ShowBtnWindEffect(false);
                  //  GlobalHallUIMgr.Instance.SetGlobalWindBtnCanTouch(false, GlobalBtnTouchBoxPool.GlobalWindStatue.Global_haed);
                    GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
                    break;
                }
            case 2:
                {
                   // HallRunTimeInfo.Login_UI.ShowBtnWindEffect(false);
               //     GlobalHallUIMgr.Instance.SetGlobalWindBtnCanTouch(false, GlobalBtnTouchBoxPool.GlobalWindStatue.Global_haed);
                    //点击GoLd处的“+”
                    GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Gold);
                    break;
                }
            default:
                break;
        }
    }
    void OnChlickBottomBtns(GameObject go)
    {
        PlayBtnMusic();
        byte clickTag = 0;
        for (byte i = 0; i < m_BottomBtn.Length; ++i)
        {
            if (m_BottomBtn[i].m_BtnObj == go)
            { 
                m_BottomBtn[i].m_IsChecked = true;
                clickTag = m_BottomBtn[i].m_Tag;
            }
            else
                m_BottomBtn[i].m_IsChecked = false;
        }
        switch(clickTag)
        {
            case 0://设置
                {
                    HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(false);
                    GlobalHallUIMgr.Instance.ShowSettingWind();
                }
                break;
            case 1://好友
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(false);
                    }
                    GlobalHallUIMgr.Instance.ShowFriendSysWnd();
                }
                break;
            case 2://排行
                {
                    HallRunTimeInfo.Instance_UI.m_loginUi.ChangeInfmationWindType(InfmationWind.Wind_Rank);
                    HallRunTimeInfo.Instance_UI.m_loginUi.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Information);
                }
                break;
            case 3://兑换
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(false);
                    }
                    GlobalHallUIMgr.Instance.ShowShopSysWnd(Shop_Type.Shop_Material);
                }
                break;
            case 4://成就
                {
                    HallRunTimeInfo.Instance_UI.m_loginUi.ChangeInfmationWindType(InfmationWind.Wind_Achievement);
                    HallRunTimeInfo.Instance_UI.m_loginUi.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Information);
                }
                break;
            case 5://敬请期待
                break;
            default:
                break;

        }
    }
    void OnCheckTempBackBtn(GameObject go)
    {
        PlayBtnMusic();
        //if (HallRunTimeInfo.Login_UI.GetWindStatue().m_state == HallLogicUIStatue.Hall_State.Hall_Mian)
        //{
        //    LogicManager.Instance.LogOff();
        //}
        //else if (HallRunTimeInfo.Login_UI.GetWindStatue().m_state == HallLogicUIStatue.Hall_State.Hall_SelectRoom)
        //{
        //    HallRunTimeInfo.Instance_UI.m_loginUi.SetWindSattue(HallLogicUIStatue.Hall_State.Hall_Mian, 0);
        //}
    }
    void OnClickShop(GameObject go)
    {
        if (HallRunTimeInfo.Instance_UI != null)
        {
            HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(false);
        }
        GlobalHallUIMgr.Instance.ShowShopSysWnd(Shop_Type.Shop_Property);

        PlayBtnMusic();
    }
    void OnClickBack(GameObject go)
    {
        PlayBtnMusic();
        if (HallRunTimeInfo.Login_UI.GetWindStatue().m_state == HallLogicUIStatue.Hall_State.Hall_SelectRoom||
            HallRunTimeInfo.Login_UI.GetWindStatue().m_state == HallLogicUIStatue.Hall_State.Hall_Information||
            HallRunTimeInfo.Login_UI.GetWindStatue().m_state == HallLogicUIStatue.Hall_State.Hall_Contest)
        {
            HallRunTimeInfo.Instance_UI.m_loginUi.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Mian);
            SetBtnUIStatue(WindBtnStatue.HallBtn_Mian);
        }
    }
    void OnClickRelax(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        GlobalHallUIMgr.Instance.ShowGameEnterWnd();
    }
    void OnVip(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        GlobalHallUIMgr.Instance.ShowVipWnd();
    }
    void OnMonthCard(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        GlobalHallUIMgr.Instance.ShowMonthCardWnd();
    }
    void OnPay(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
    }
    public void Update(float dTime)
    {
        //UpdateWarningState(1);
        //UpdateWarningState(2);
        //UpdateWarningState(4);
    }
    public void UpdateUserGold()
    {
        if (m_RichInf == null || m_RichInf.Length != 3)
            return;
        if (m_RichInf[0].m_GoldLabel == null || m_RichInf[1].m_GoldLabel == null || m_RichInf[2].m_GoldLabel == null)
            return;
        m_RichInf[0].m_GoldLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetMedal().ToString();
        m_RichInf[1].m_GoldLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency().ToString();
        m_RichInf[2].m_GoldLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel().ToString();
    }
    public void UpdateWarningState(byte type)   //'1' 邮件赠礼 '2' 排名  ‘4’成就
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        if (m_BtnWindObj[1].activeSelf != true)
            return;
        switch (type)
        {
            case 1://邮件,好友
                {
                    if ((PlayerRole.Instance.RoleChar.GetCharList() != null && PlayerRole.Instance.RoleChar.GetCharList().Count > 0) ||
                        PlayerRole.Instance.RoleStatesMessage.GetRelationStates()||
                        PlayerRole.Instance.RoleStatesMessage.GetMailStates() ||
                        PlayerRole.Instance.RoleStatesMessage.GetGiffStates())
                    {
                         m_Warning[0].SetActive(true);
                    }
                    else
                    {
                         m_Warning[0].SetActive(false);
                    }                                                    
                    break;
                }                
            case 2://排行
                {
                    if (PlayerRole.Instance.RoleStatesMessage.GetWeekRankStates())
                        m_Warning[1].SetActive(true);
                    else
                        m_Warning[1].SetActive(false);
                    break;
                }
              
            case 4://成就
                {
                    if (PlayerRole.Instance.RoleStatesMessage.GetAchievementStates())
                        m_Warning[3].SetActive(true);
                    else
                        m_Warning[3].SetActive(false);
                    break;
                }               
            default:
                break;

        }
    }
    public void SetBtnUIStatue(HallLoginUI_Btns.WindBtnStatue statue)
    {
        if (WindObj == null)
            InitGameOnject();
        switch (statue)
        {
            case WindBtnStatue.HallBtn_Mian:
                {
                    m_BtnWindObj[0].transform.localPosition = new Vector3(0,260,0);
                    if (!m_BtnWindObj[1].activeSelf) 
                        m_BtnWindObj[1].SetActive(true);
                    if (m_BtnWindObj[2].activeSelf) 
                        m_BtnWindObj[2].SetActive(false);
                    HideMonthCardEft();
                    HideMonthCardBtnAndVIPBtn(true);
                }
                break;
            case WindBtnStatue.HallBtn_SelectScene:
                {
                    m_BtnWindObj[0].transform.localPosition = new Vector3(0, 260, 0);
                    if (m_BtnWindObj[1].activeSelf)
                        m_BtnWindObj[1].SetActive(false);
                    if (!m_BtnWindObj[2].activeSelf) 
                        m_BtnWindObj[2].SetActive(true);
                    HideMonthCardBtnAndVIPBtn(true);
                }
                break;
            case WindBtnStatue.HallBtn_Information:
                {
                    m_BtnWindObj[0].transform.localPosition = new Vector3(-180, -265, 0);
                    if (m_BtnWindObj[1].activeSelf) 
                        m_BtnWindObj[1].SetActive(false);
                    if (!m_BtnWindObj[2].activeSelf) 
                        m_BtnWindObj[2].SetActive(true);
                    HideMonthCardBtnAndVIPBtn(false);

                }
                break;
            case WindBtnStatue.HallBtn_Contest:
                {
                    m_BtnWindObj[0].transform.localPosition = new Vector3(0, 260, 0);
                    if (m_BtnWindObj[1].activeSelf) 
                        m_BtnWindObj[1].SetActive(false);
                    if (!m_BtnWindObj[2].activeSelf)
                        m_BtnWindObj[2].SetActive(true);
                    HideMonthCardBtnAndVIPBtn(false);

                }break;
            default:
                break;
        }
    }
    struct UserRichInf
    {
        public UILabel m_GoldLabel;
        public WindBtn m_Btn;
    }
    public enum WindBtnStatue
    {
        HallBtn_Mian = 0,//主界面显示的按钮
        HallBtn_SelectScene,//选关界面显示的按钮
        HallBtn_Information,//个人信息界面显示的按钮
        HallBtn_Contest,//竞赛界面
    }

}
