using UnityEngine;
using System.Collections.Generic;

public class TipBox_0 : GlobalBaseWind
{
    const float m_fBgplay = 0.01f;
    const float m_fItemPlay = 0.2f;

    float m_TipBoxLifeTime;
    float m_fElapsedTime = 0;
    Object m_objItem;
    GameObject[] m_goItem = new GameObject[3];
    GameObject m_goOk;
    public Object Init()
    {
        m_objItem = ResManager.Instance.LoadObject("TipBox_1", "ExtraRes/UI/Predab/", ResType.ExtraRes, false);//Item
        // m_WindId = GlobalBtnTouchBoxPool.GlobalWindStatue.Global_TipBox_0;
        Object obj = Initobj("TipBox_0");
        return obj;
    }
    public void InitGameObject(TipBox_0.TipBox_0_ZiTi ZiTiType, float LifeTime, tagItemOnce[] pItems, byte byCount)
    {
        Init_GameObj(true);
        BaseTranF.GetChild(1).GetComponent<UISprite>().spriteName = ZiTiType.ToString();
        m_goOk = BaseTranF.GetChild(2).gameObject;
        UIEventListener.Get(m_goOk).onClick = OnOK;
        m_goOk.SetActive(false);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_GoldJump);
        WndManager.Instance.Push(BaseTranF);
        for (int i = 0; i < byCount; i++)
        {
            m_goItem[i] = (GameObject)GameObject.Instantiate(m_objItem);
            m_goItem[i].SetActive(false);

            UISprite sp = m_goItem[i].transform.GetChild(3).GetComponent<UISprite>();
            UILabel label = m_goItem[i].transform.GetChild(1).GetComponent<UILabel>();

            sp.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[pItems[i].ItemID].ItemIcon;
            label.text = pItems[i].ItemSum.ToString();


            if (byCount == 0)
            {

            }
            else if (byCount == 1)
            {
                m_goItem[i].transform.position = new Vector3(0f, -6.3f, 0f);
            }
            else if (byCount == 2)
            {
                if (i == 0)
                {
                    m_goItem[i].transform.position = new Vector3(-77f, -6.3f, 0f);
                }
                else if (i == 1)
                {
                    m_goItem[i].transform.position = new Vector3(77f, -6.3f, 0f);
                }
            }
            else if (byCount == 3)
            {
                if (i == 0)
                {
                    m_goItem[i].transform.position = new Vector3(-127f, -6.3f, 0f);
                }
                else if (i == 1)
                {
                    m_goItem[i].transform.position = new Vector3(0f, -6.3f, 0f);
                }
                else if (i == 2)
                {
                    m_goItem[i].transform.position = new Vector3(127f, -6.3f, 0f);
                }
            }
            m_goItem[i].transform.SetParent(BaseTranF.transform, false);
        }
        m_fElapsedTime = 0;
        m_TipBoxLifeTime = LifeTime;// LifeTime;// m_fBgplay + f_fItemPlay * 3 + LifeTime;

    }
    void InitTipBox()
    {

    }
    public bool Update(float dTime)
    {
        if (WindObj == null)
            return false;
        m_fElapsedTime += dTime;
        if (m_fElapsedTime >= m_fBgplay && m_goItem[0] != null && !m_goItem[0].activeSelf)
        {
            m_goItem[0].SetActive(true);
        }
        if (m_fElapsedTime >= m_fBgplay + m_fItemPlay && m_goItem[1] != null && !m_goItem[1].activeSelf)
        {
            m_goItem[1].SetActive(true);
        }
        if (m_fElapsedTime >= m_fBgplay + 2 * m_fItemPlay && m_goItem[2] != null && !m_goItem[2].activeSelf)
        {
            m_goItem[2].SetActive(true);
        }
        if (!m_goOk.activeSelf && m_fElapsedTime > m_fBgplay + 3 * m_fItemPlay)
        {
            m_goOk.SetActive(true);
        }
        return true;
    }
    void OnOK(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        DestorySelf();
    }
    public void DestorySelf()
    {
        WndManager.Instance.Pop(BaseTranF);
        for (int i = 0; i < m_goItem.Length; i++)
        {
            if (m_goItem[i] != null)
            {
                GameObject.Destroy(m_goItem[i]);
                m_goItem[i] = null;
            }
        }
        if (WindObj)
            GameObject.Destroy(WindObj);
        WindObj = null;

        //m_TipBoxLifeTime = 1f;
    }
    public enum TipBox_0_ZiTi
    {
        Exchange = 0,
        Pay,
        Buy,
        Match,
        Rank,
        Sign,
        Online,
        Mission,
        Activity
    }
}
public class GlobalBaseWind
{
    string m_path = "ExtraRes/UI/Predab/";
    GameObject m_WindObj;
    Transform m_BaseTranF;
    Object m_Obj;

    public void Init()
    {
    }
    public Object Initobj(string objName)
    {
        m_Obj = ResManager.Instance.LoadObject(objName, Path, ResType.ExtraRes, false);
        return m_Obj;
    }
    public void Init_GameObj(bool bShow = true)
    {
        SetParent(((GameObject)GameObject.Instantiate(m_Obj)).transform, bShow);
        // ResManager.Instance.UnloadObject(Obj);
    }
    public void SetParent(Transform objtr, bool bShow = true)
    {
        objtr.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (objtr.localScale.x != 1) objtr.localScale = new Vector3(1, 1, 1);
        m_BaseTranF = objtr;
        m_WindObj = m_BaseTranF.gameObject;
        if (m_WindObj.activeSelf != bShow) m_WindObj.SetActive(bShow);
    }
    public void CloseWind()
    {
        if (m_WindObj != null)
        {
            GameObject.Destroy(m_WindObj);
        }
        m_WindObj = null;
        m_BaseTranF = null;
        //  GlobalHallUIMgr.Instance.ClearGlobalTouchPool(m_WindId);
    }
    public void ShowWind(int depth = 9)
    {
        if (!m_WindObj.activeSelf)
        {
            m_WindObj.SetActive(true);
        }

    }
    public void Update(float dTime)
    { }
    public GameObject GridInstantiateObj(UIGrid grid, GameObject obj)
    {
        GameObject obj__ = GameObject.Instantiate(obj) as GameObject;
        if (!obj.activeSelf)
            obj.SetActive(true);
        grid.AddChild(obj__.transform);
        if (obj__.transform.localScale.x != 1)
            obj__.transform.localScale = new Vector3(1, 1, 1);
        return obj__;
    }
    public void ChangeBtnName(UIButton btn, string name)
    {
        btn.normalSprite = name;
        btn.hoverSprite = name;
        btn.pressedSprite = name;
        btn.disabledSprite = name;
    }
    public UIEventListener GetBtnLister(GameObject BtnObj, bool bAddTouchPool = true)
    {
        return UIEventListener.Get(BtnObj);
    }
    public void PlayBtnMusic()
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
    }
    public Transform BaseTranF
    {
        get { return m_BaseTranF; }
        set { m_BaseTranF = value; }
    }
    public string Path
    {
        get { return m_path; }
        set { m_path = value; }
    }
    public struct WindBtn
    {
        public GameObject m_BtnObj;
        public UIButton m_Btn;
        public byte m_Tag;
        public bool m_IsChecked;
    }
    public struct WindTwreen
    {
        public TweenAlpha m_alpha;
        public TweenPosition m_Pos;
    }
    public struct ViewInf
    {
        public UIGrid m_grid;
        public GameObject m_BaseChild;
    }
    public GameObject WindObj
    {
        get { return m_WindObj; }
        set { m_WindObj = value; }
    }
    public Object Obj
    {
        get { return m_Obj; }
        set { m_Obj = value; }
    }
}
class Mask : GlobalBaseWind//蒙版
{

    UIPanel m_Panle = null;
    public void Init()
    {
        Initobj("MaskTextTure");
        Init_GameObj(false);
        ResManager.Instance.UnloadObject(Obj);
        m_Panle = BaseTranF.GetComponent<UIPanel>();
    }
    public void CloseWind()
    {
        if (WindObj.activeSelf)
        {
            WindObj.SetActive(false);
        }
    }
    public void ShowWind()
    {
        if (!WindObj.activeSelf)
        {
            WindObj.SetActive(true);
        }

    }
    public void SetDepth(int depth)
    {
        m_Panle.depth = depth;
    }
}
public class HeadWind : GlobalBaseWind
{
    GameObject[]    m_HeadWindGameObj = new GameObject[3];
    GameObject      m_WarningObj;
    UILabel         m_NormalHeadExp_label = null;
    UISprite        m_NormalHeadExp_Sprite = null;
    UITexture       m_HeadTexture = null;
    UILabel[]       m_MatchInfo_Label = new UILabel[5];
    Transform       m_tfMatchScore;
    UISprite        m_GameDelayed_Sprite;
    UILabel         m_GameDelayed_Label;
    GameObject      m_VipObj;
    UISprite        m_VipLevelIcon;

    public Object Init()
    {
        //m_WindId = GlobalBtnTouchBoxPool.GlobalWindStatue.Global_haed;
        Object obj = Initobj("HeadWind");
        return obj;
    }
    public void SetMyHeader(Texture2D tex)
    {
        if (tex == null)
            m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        else
            m_HeadTexture.mainTexture = tex;
    }
    public void InitGameObject()
    {
        Init_GameObj(false);
        for (byte i = 0; i < m_HeadWindGameObj.Length; ++i)
            m_HeadWindGameObj[i] = BaseTranF.GetChild(i).gameObject;
        InitNormalHead();
        SetHeadInf();
        InitMatchInfo();
        m_GameDelayed_Sprite = BaseTranF.GetChild(2).GetChild(0).GetComponent<UISprite>();
        m_GameDelayed_Label = BaseTranF.GetChild(2).GetChild(1).GetComponent<UILabel>();
    }
    void InitNormalHead()
    {
        m_HeadTexture = BaseTranF.GetChild(0).GetChild(0).GetComponent<UITexture>();
        m_NormalHeadExp_Sprite = BaseTranF.GetChild(0).GetChild(1).GetComponent<UISprite>();
        m_NormalHeadExp_label = BaseTranF.GetChild(0).GetChild(2).GetComponent<UILabel>();
        m_WarningObj = BaseTranF.GetChild(0).GetChild(3).gameObject;
        m_VipObj = BaseTranF.GetChild(0).GetChild(4).gameObject;
        m_VipLevelIcon = BaseTranF.GetChild(0).GetChild(4).GetComponent<UISprite>();
        GetBtnLister(BaseTranF.GetChild(0).gameObject).onClick = OnClickHeadBtn;
        UIEventListener.Get(BaseTranF.GetChild(0).gameObject).onPress = OnPressMsg;
    }
    void InitMatchInfo()
    {
        for (byte i = 0; i < m_MatchInfo_Label.Length; ++i)
        {
            m_MatchInfo_Label[i] = BaseTranF.GetChild(1).GetChild(i).GetComponent<UILabel>();
        }
        m_tfMatchScore = BaseTranF.GetChild(1).GetChild(5).transform;
        UIEventListener.Get(BaseTranF.GetChild(1).gameObject).onClick = OnClickMatchRank;
    }
    public void ShowHeadWind(HeadWind.HeadStatue statue)
    {
        if (!BaseTranF.gameObject.activeSelf) BaseTranF.gameObject.SetActive(true);
        if (statue == HeadStatue.Head_Normal)
        {
            if (!m_HeadWindGameObj[0].activeSelf)
            {
                m_HeadWindGameObj[0].SetActive(true);
                if (SceneRuntime.Instance != null)
                {
                    m_HeadWindGameObj[2].SetActive(true);
                    UpdateWarningSate();
                }
            }
            if (m_HeadWindGameObj[1].activeSelf)
                m_HeadWindGameObj[1].SetActive(false);
        }
        else
        {
            if (!m_HeadWindGameObj[1].activeSelf)
                m_HeadWindGameObj[1].SetActive(true);
            if (m_HeadWindGameObj[0].activeSelf)
                m_HeadWindGameObj[0].SetActive(false);
        }
    }
    public void SetHeadInf()
    {
        uint ficeID = PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID();
        if (ficeID < ConstValue.CUSTOM_HEADER)
        {
            m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
        }
        else
        {
            //获取头像
            //  m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[0];
            HeaderManager.Instance.GetPlayerHeader(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), ficeID, HeaderOptType.HEADER_ME, null);
        }
        //m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
        m_NormalHeadExp_label.text = PlayerRole.Instance.RoleInfo.RoleMe.GetLevel().ToString();
        m_NormalHeadExp_Sprite.fillAmount = GetCurExperience();
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() > 0)
        {
            m_VipObj.SetActive(true);
            m_VipLevelIcon.spriteName = string.Format("Vip_Icon_{0}", PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel());
        }
    }
    public void UpdateWarningSate()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        if (SceneRuntime.Instance == null)
            return;
        m_WarningObj.SetActive(PlayerRole.Instance.RoleStatesMessage.GetTaskStates());
    }
    public void UpdateGameDelayedData()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        if (m_HeadWindGameObj[2].activeSelf == false)
            return;
        uint d = SceneMain.Instance.bulletTime.CurTime;
        if (d > 999)
            d = 999;
        if (d < 70)
        {
            m_GameDelayed_Label.color = Color.green;
            m_GameDelayed_Sprite.spriteName = "Game_Delayed0";
        }
        else if (d < 130)
        {
            m_GameDelayed_Label.color = new Color(1.0f, 0.8f, 0);
            m_GameDelayed_Sprite.spriteName = "Game_Delayed1";
        }
        else
        {
            m_GameDelayed_Label.color = Color.red;
            m_GameDelayed_Sprite.spriteName = "Game_Delayed2";
        }
        m_GameDelayed_Label.text = d + "ms";
    }
    public void UpdateExperienceData()
    {
        if (WindObj == null)
            return;
        if (WindObj.activeSelf != true)
            return;
        m_NormalHeadExp_label.text = PlayerRole.Instance.RoleInfo.RoleMe.GetLevel().ToString();
        m_NormalHeadExp_Sprite.fillAmount = GetCurExperience();
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() > 0)
        {
            m_VipObj.SetActive(true);
            m_VipLevelIcon.spriteName = string.Format("Vip_Icon_{0}", PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel());

        }
    }
    public void UpdateMyMatchScoreInfo()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        if (m_HeadWindGameObj[1].activeSelf != true)
            return;
        m_MatchInfo_Label[0].text = PlayerRole.Instance.RoleInfo.RoleMe.GetMonthScore().ToString();
    }
    public void UpdateMyMatchInfoDate()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        if (m_HeadWindGameObj[1].activeSelf != true)
            return;
        m_MatchInfo_Label[1].text = (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthIndex() + 1).ToString();
        m_MatchInfo_Label[0].text = PlayerRole.Instance.RoleInfo.RoleMe.GetMonthScore().ToString();
        if (FishConfig.Instance.m_MatchInfo.m_MonthStr.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()))
            m_MatchInfo_Label[2].text = FishConfig.Instance.m_MatchInfo.m_MonthStr[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].MonthName + "元话费赛";
        m_MatchInfo_Label[3].text = PlayerRole.Instance.MonthManager.GetMonthSigUpPlayerSum(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()).ToString();
        UpdateMatchTime();
    }
    public void UpdateMatchTime()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        if (m_HeadWindGameObj[1].activeSelf != true)
            return;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() == 0)
            return;
        m_MatchInfo_Label[4].text = GetEndTime(PlayerRole.Instance.MonthManager.GetMonthGameEndTime(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()));
    }
    string GetEndTime(System.DateTime date)
    {
        int mill = date.Millisecond * -1;
        date = date.AddMilliseconds(mill);
        int sec = date.Second * -1;
        date = date.AddSeconds(sec);
        System.TimeSpan Span = date - SystemTime.Instance.GetSystemDateTime;
        int hour = (int)Span.TotalSeconds / 3600;
        int min = (int)(Span.TotalSeconds % 3600) / 60;
        int second = (int)(Span.TotalSeconds % 3600) % 60;
        string strTime = hour.ToString("D2") + ":" + min.ToString("D2") + ":" + second.ToString("D2");
        //  Debug.Log(strTime);
        return strTime;
    }
    float GetCurExperience()
    {
        uint experineceSum = 0;
        ushort level = PlayerRole.Instance.RoleInfo.RoleMe.GetLevel();
        if (level == Experience.Exp.Length-1)    //经验满级
            return 1.0f;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetLevel() < Experience.Exp.Length)
        {
            experineceSum = Experience.Exp[level + 1] - Experience.Exp[level];
            float exp = (float)(PlayerRole.Instance.RoleInfo.RoleMe.GetExp() * 1.0f) / (float)(experineceSum * 1.0f);
            return (0.1f + (exp * 0.8f));
        }
        else
            return 1.0f;

    }
    void OnClickHeadBtn(GameObject go)
    {
        if (HallRunTimeInfo.Instance_UI != null)
        {
            HallRunTimeInfo.Instance_UI.m_loginUi.ChangeInfmationWindType(InfmationWind.Wind_UserInF);
            HallRunTimeInfo.Instance_UI.m_loginUi.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Information);
        }
        else if (SceneRuntime.Instance != null)
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Other_Head, PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), (Texture2D)m_HeadTexture.mainTexture, true);
        }
    }
    void OnClickMatchRank(GameObject go)
    {
        SceneRuntime.LogicUI.ShowMatchRankWnd();
    }
    void OnPressMsg(GameObject obj, bool state)
    {
        if (SceneRuntime.Instance == null)
            return;
    }
    public void ShutDown()
    {
        // GlobalHallUIMgr.Instance.ClearGlobalTouchPool(m_WindId);
        for (byte i = 0; i < 2; ++i)
            if (m_HeadWindGameObj[i].activeSelf)
                m_HeadWindGameObj[i].SetActive(false);
    }

    public Vector3 ScorePos()
    {
        return m_tfMatchScore.position;
    }
    public enum HeadStatue
    {
        Head_Normal = 0,//平时的头像，
        Head_Contest,//比赛时的头像，
    }
}

public class GlobalHallUIMgr : Singleton<GlobalHallUIMgr>
{
    List<Object> m_GlobalHallUIObject = new List<Object>();
    public Texture2D[] m_HeadTextureUI = new Texture2D[7];//头像的5个贴图加一个系统头像
    //  GlobalBtnTouchBoxPool               m_GlobalBtnTouchBoxPool = new GlobalBtnTouchBoxPool();
    HeadWind m_HeadWind = new HeadWind();
    SettingWind m_SettingWind = new SettingWind();
    FishShow m_FishSHow = new FishShow();
    AboutOur m_AboutOur = new AboutOur();
    Mask m_Mask = new Mask();
    GlobalNotice m_Notice = new GlobalNotice();
    NoticMgr m_NoticeMgr = new NoticMgr();
    TweenScale m_scale = null;
    TipBox_0 m_TipBox_0 = new TipBox_0();
    Object[] m_FriendSysObj = new Object[5];
    Object[] m_ShopSysObj = new Object[9];
    Object[] m_GlobalUIObject = new Object[6];  //'0' 账号管理 ‘1’系统提示 ‘2’玩家小名片 '3' 比赛提示框'4'奖励标框 5分享
    Object m_objGiftBtneff;//礼物按钮效果
    FriendSysMgr m_FriendSysMgr = new FriendSysMgr();
    ShopLogicMgr m_ShopLogicMgr = new ShopLogicMgr();
    PayLogicUI m_PayLogicUI = new PayLogicUI();
    Account_MgrUI m_AccountMgrUI = new Account_MgrUI();       //账号管理窗口
    GlobalSystemTipsUI m_SysTipsUI = new GlobalSystemTipsUI();
    GlobalNameCards m_NameCardsUI = new GlobalNameCards();      //玩家小名片
    GlobalMatchMsgBox m_MatchMsgBox = new GlobalMatchMsgBox();    //比赛提示框
    PayBuyConfirmUI m_PayBuyConfirmUI = new PayBuyConfirmUI();
    ActivityRewardWnd m_ActivityRewardWnd = new ActivityRewardWnd();
    VipMgrLogic m_VipMgrLogic = new VipMgrLogic();
    GameLogicMgr m_GameLogicMgr = new GameLogicMgr();
    GameShare m_GameShare=new GameShare();
    public GameLogicMgr GameLogicMgr
    {
        get { return m_GameLogicMgr; }
        set { m_GameLogicMgr = value; }
    }

    public void AnimationEffect(GameObject obj)
    {
        TweenScale scale = obj.GetComponent<TweenScale>();
        if (scale == null)
        {
            obj.AddComponent<TweenScale>();
            scale = obj.GetComponent<TweenScale>();
            scale.animationCurve = m_scale.animationCurve;
            scale.from = m_scale.from;
            scale.to = m_scale.to;
            scale.duration = m_scale.duration;
        }
        if (scale.enabled != true)
            scale.enabled = true;
        scale.ResetToBeginning();
        scale.PlayForward();
    }
    System.Collections.IEnumerator IniProcedure(object obj)
    {
        LoadHeadTextTure();
        yield return new WaitForEndOfFrame();
        m_Mask.Init();
        yield return new WaitForEndOfFrame();
        m_GlobalHallUIObject.Add(m_HeadWind.Init());
        m_GlobalHallUIObject.Add(m_SettingWind.Init());
        m_GlobalHallUIObject.Add(m_FishSHow.Init());
        m_GlobalHallUIObject.Add(m_AboutOur.Init());
        m_GlobalHallUIObject.Add(m_TipBox_0.Init());
        m_Notice.InitObj();
        m_VipMgrLogic.Init();
        m_scale = m_Mask.BaseTranF.GetComponent<TweenScale>();
        if (m_scale.enabled) m_scale.enabled = false;
        yield return new WaitForEndOfFrame();
        for (byte i = 0; i < FriendSysObj.Length; ++i)
        {
            m_FriendSysObj[i] = ResManager.Instance.LoadObject(string.Format("FriendSysWidget{0}", i), "ExtraRes/UI/Predab/FriendsSys/", ResType.ExtraRes, false);
        }
        for (byte j = 0; j < ShopSysObj.Length; ++j)
        {
            m_ShopSysObj[j] = ResManager.Instance.LoadObject(string.Format("ShopSysWndUI{0}", j), "ExtraRes/UI/Predab/ShopSys/", ResType.ExtraRes, false);
        }
        yield return new WaitForEndOfFrame();
        m_GlobalUIObject[0] = ResManager.Instance.LoadObject("Account_Mgr_Wnd", "ExtraRes/UI/Predab/AccountMgr/", ResType.ExtraRes, false);
        m_GlobalUIObject[1] = ResManager.Instance.LoadObject("SystemTips_Dlg", "ExtraRes/UI/Predab/", ResType.ExtraRes, false);
        m_GlobalUIObject[2] = ResManager.Instance.LoadObject("Name_Card_Dlg", "ExtraRes/UI/Predab/NameCards/", ResType.ExtraRes, false);
        m_GlobalUIObject[3] = ResManager.Instance.LoadObject("Match_MsgBox", "ExtraRes/UI/Predab/", ResType.ExtraRes, false);
        m_GlobalUIObject[4] = ResManager.Instance.LoadObject("Show_RewardWnd", "ExtraRes/UI/Predab/", ResType.ExtraRes, false);

        m_GlobalUIObject[4] = ResManager.Instance.LoadObject("Show_RewardWnd", "ExtraRes/UI/Predab/", ResType.ExtraRes, false);

        m_GlobalUIObject[5] = ResManager.Instance.LoadObject("game_share", "ExtraRes/UI/Predab/", ResType.ExtraRes, false);

        m_objGiftBtneff = ResManager.Instance.LoadObject("UIEF_CharmClick", "ExtraRes/Effect/Prefabs/UIEFCharmClick/", ResType.ExtraRes, false);
        InitLogic.EndInit();
        yield break;
    }
    public bool Init()
    {
        InitLogic.StartInit();
        SceneMain.Instance.StartInnerCoroutine(IniProcedure(null));
        return true;
    }
    void LoadHeadTextTure()
    {
        for (byte i = 0; i < m_HeadTextureUI.Length; ++i)
        {
            string str = "GlobalWind_Head_" + i.ToString();
            m_HeadTextureUI[i] = (Texture2D)ResManager.Instance.LoadObject(str, "ExtraRes/UI/Image/HeadImage/", ResType.ExtraRes, typeof(Texture2D), false);
        }
    }

    bool bShowSetting = false;
    bool bShowTipBox_0 = false;
    public void Update(float dTime)
    {
        if (bShowSetting)
            m_SettingWind.Update(dTime);
        if (bShowTipBox_0)
        {
            if (!m_TipBox_0.Update(dTime))
            {
                bShowTipBox_0 = false;
                ShowMask(false, 9);
            }
        }
        if (m_Notice.m_BaseWndObject == null)
        {
            NoticeInfo info = m_NoticeMgr.Update(dTime);
            if (info != null)
            {
                m_Notice.ShowNotice(info);
            }
        }
        else
            m_Notice.Update(dTime);


        m_FriendSysMgr.Update(dTime);
        m_ShopLogicMgr.Update(dTime);
        m_MatchMsgBox.Update(dTime);
        if (m_SysTipsUI.m_BaseWndObject != null)
        {
            m_SysTipsUI.Update(dTime);
        }
        m_NameCardsUI.Update(dTime);
        m_ActivityRewardWnd.Upate(dTime);
        m_HeadWind.UpdateMatchTime();
        m_HeadWind.UpdateGameDelayedData();
        m_AccountMgrUI.Update(dTime);
        m_GameLogicMgr.Update(dTime);
    }
    public void ShowFishShow(bool isLottery)
    {
        //ShowMask();
        if (m_FishSHow.BaseTranF == null)
            m_FishSHow.InitGameObjcet(isLottery);
        m_FishSHow.ShowWind();
        if (HallRunTimeInfo.Instance_UI != null)
            HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(false);
        WndManager.Instance.Push(m_FishSHow.WindObj);
        GlobalHallUIMgr.Instance.AnimationEffect(m_FishSHow.WindObj);
    }
    public void ShowHead(HeadWind.HeadStatue statue)
    {
        if (m_HeadWind.BaseTranF == null)
            m_HeadWind.InitGameObject();
        m_HeadWind.ShowHeadWind(statue);
    }
    public void ShowSettingWind()
    {
        //ShowMask();
        bShowSetting = true;
        if (m_SettingWind.BaseTranF == null)
            m_SettingWind.InitGameObjcet();
        m_SettingWind.ShowWind();
        WndManager.Instance.Push(m_SettingWind.WindObj);
        GlobalHallUIMgr.Instance.AnimationEffect(m_SettingWind.WindObj);
    }
    public void ShowAboutOurWnd()
    {
        if (m_AboutOur.BaseTranF == null)
            m_AboutOur.InitGameObjcet();
        m_AboutOur.ShowWind();
        WndManager.Instance.Push(m_AboutOur.WindObj);

    }
    public void ShowMask(bool bSHow = true, int depth = 9)
    {
        if (bSHow)
            m_Mask.ShowWind();
        else
            m_Mask.CloseWind();
        m_Mask.SetDepth(depth);
    }
    public void ShowTipBox_0(TipBox_0.TipBox_0_ZiTi ZiTiType, float LifeTime, ushort uRewardid1, ushort uRewardid2 = 0)
    {
        if (bShowTipBox_0)
            return;
        bShowTipBox_0 = true;
        //解析奖励
        tagItemOnce[] pItems = new tagItemOnce[3];
        for (int i = 0; i < pItems.Length; i++)
        {
            pItems[i] = new tagItemOnce();
        }
        byte Index = 0;
        tagRewardOnce pReward;
        if (FishConfig.Instance.m_RewardConfig.RewardMap.TryGetValue(uRewardid1, out pReward))
        {
            for (byte i = 0; i < pReward.RewardItemVec.Count; i++)
            {
                if (Index < pItems.Length)
                {
                    pItems[Index++] = pReward.RewardItemVec[i];
                }
            }
        }
        if (FishConfig.Instance.m_RewardConfig.RewardMap.TryGetValue(uRewardid2, out pReward))
        {
            for (byte i = 0; i < pReward.RewardItemVec.Count; i++)
            {
                if (Index < pItems.Length)
                {
                    pItems[Index++] = pReward.RewardItemVec[i];
                }
            }
        }

        m_TipBox_0.InitGameObject(ZiTiType, LifeTime, pItems, Index);
        // ShowMask(true, 12);
    }
    public void CloseSettingWind()
    {
        if (m_SettingWind != null)
            WndManager.Instance.Pop(m_SettingWind.WindObj);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        // m_Mask.CloseWind();
        bShowSetting = false;
        m_SettingWind.CloseWind();
        if (HallRunTimeInfo.Instance_UI != null)
        {
            if (HallRunTimeInfo.Login_UI.GetWindStatue().m_state == HallLogicUIStatue.Hall_State.Hall_Mian)
            {
                HallRunTimeInfo.Login_UI.ShowMainWindCenterInf();
            }

        }
    }
    public void CloseAboutOurWnd()
    {
        if (m_AboutOur != null)
            WndManager.Instance.Pop(m_AboutOur.WindObj);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        m_AboutOur.CloseWind();
        ShowSettingWind();
    }
    public void CloseHead()
    {
        m_HeadWind.CloseWind();
    }
    public void CloseFishShow()
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        if (m_FishSHow.WindObj != null)
            WndManager.Instance.Pop(m_FishSHow.WindObj);

        m_FishSHow.CloseWind();
    }
    public void ShutDown()
    {
        for (int i = 0; i < m_GlobalHallUIObject.Count; ++i)
            ResManager.Instance.UnloadObject(m_GlobalHallUIObject[i]);
        m_GlobalHallUIObject.Clear();
        ShutDownFriendSysWnd();
        ShutShopSysWnd();
        ShutDownPayWnd();
        ShutDownAccountMgr();
        ShutDownSystemTipsUI();
        ShutDownNameCardsUI();
        CloseFishShow();
        CloseSettingWind();
        m_MatchMsgBox.ShutDown();
        m_ActivityRewardWnd.ShutDown();
        m_VipMgrLogic.ShutDown();
        m_GameLogicMgr.ShutDown();
    }

    //好友系统
    public void ShowFriendSysWnd()
    {
        m_FriendSysMgr.ShowWnd();

    }
    public void ShutDownFriendSysWnd()
    {
        m_FriendSysMgr.ShutDown();
    }
    //显示商城
    public void ShowShopSysWnd(Shop_Type type)
    {
        m_ShopLogicMgr.ShowShopWndUI(type);
    }
    public void ShowConfirmBuyWnd(byte onlyID, byte shopID, uint itemSum = 1, bool bNewGuide = false)
    {
        m_ShopLogicMgr.ShowConfirmBuyWnd(onlyID, shopID, itemSum, bNewGuide);
    }
    public void ShutShopSysWnd()
    {
        m_ShopLogicMgr.ShutDown();
    }
    //显示充值界面
    public void ShowPayWnd(PayType type)
    {
        //if (HallRunTimeInfo.Instance_UI != null)
        //{
        //    HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(false);
        //    HallRunTimeInfo.Login_UI.SetHallWindBtnCanTouch(false, HallLogicUIStatue.Hall_State.Hall_Btns);
        //}
        m_PayLogicUI.ShowPayWndUI(type);
        //ShowMask(true);
    }
    public void ShutDownPayWnd()
    {
        m_PayLogicUI.CloseWnd();
    }
    //账号管理
    public void ShowAccountMgrWnd(Acoount_MgrUI_Type type)
    {
        m_AccountMgrUI.ShowAccount_MgrUI(type);
    }
    public void ChangeAccountSeccess()
    {
        m_AccountMgrUI.ChangeAccountSeccess();
    }
    public void PhonePsChanged()
    {
        m_AccountMgrUI.PhonePsChanged();
    }
    public void ShutDownAccountMgr()
    {
        m_AccountMgrUI.ShutDown();
    }
    //系统公告
    public void ShowSystemNotice(NoticeInfo info)
    {
        m_NoticeMgr.AddNotic(info);
        //  m_Notice.ShowNotice(str, times);
    }
    //调整系统公告位置
    public void AdjustNoticePos()
    {
        m_Notice.AdjustNoticePos();
    }
    public void ShutDowNotic()
    {
        m_Notice.ShutDown();
        m_NoticeMgr.ClearNotice();
    }
    //系统提示框
    public void UpdateSystemTipsUITime(float lifeTime)
    {
        m_SysTipsUI.UpdateSystemTime(lifeTime);
    }
    public void ShowSystemTipsUI(string describe, float lifetime, bool bScale = false)
    {
        m_SysTipsUI.ShowSystemTips(describe, lifetime, bScale);
    }
    public void ShowToUpgrade(byte Level, float lifetime, uint RewardID)
    {
        m_SysTipsUI.ShowToUpgrade(Level, lifetime, RewardID);
    }
    public void ShowFinishTips(uint ID, float lifetime, byte type)
    {
        m_SysTipsUI.ShowFinishTips(ID, lifetime, type);
    }
    public void ShutDownSystemTipsUI()
    {
        m_SysTipsUI.ShutDown();
    }
    //玩家小名片
    public void ShowNameCardsUI(NameCardsUIType type, CardsParentUIType parent, uint PlayerID, Texture2D tex, bool bMyself = false, bool bFriends = false)
    {
        m_NameCardsUI.ShowNameCardsUI(type, parent, PlayerID, tex, bMyself);
    }
    public void ShowMatchMsgBox(string strDec, byte MatchID, MatchMsgBoxType type)
    {
        m_MatchMsgBox.ShowMatchMsgBox(strDec, MatchID, type);
    }
    //商店充值确认框
    public void ShowmPayBuyConfirm(byte ID, uint amount, uint count, PayType type)
    {
        m_PayBuyConfirmUI.ShowPayBuyConfirm(ID, amount, count, type);
    }
    public void ShutDownPayBuyConfirm()
    {
        m_PayBuyConfirmUI.ShutDown();
    }
    //奖品预览
    public void PreviewRewardWnd(uint rewardID, ActivityType wndType)
    {
        m_ActivityRewardWnd.PreviewRewardWnd(rewardID, wndType);
    }
    //领取奖励物品
    public void GetRewardWnd(uint rewardID, uint activityID, uint nodeID, ActivityType wndType)
    {
        m_ActivityRewardWnd.GetRewardWnd(rewardID, activityID, nodeID, wndType);
    }
    public void ShowRewardWnd(ushort[] RewardID)
    {
        m_ActivityRewardWnd.ShowRewardWnd(RewardID);
    }
    public void ShowVipWnd()
    {
        m_VipMgrLogic.ShowVipWnd();
    }
    public void ShowVipDetailWnd()
    {
        m_VipMgrLogic.ShowVipDetailWnd();
    }
    public void ShowMonthCardWnd()
    {
        m_VipMgrLogic.ShowMonthCardWnd();
    }

    public void ShowGameEnterWnd()
    {
        m_GameLogicMgr.ShowGameEnterWnd();
    }

    //小游戏 牛牛
    public bool GetGameLogicMgr()
    {
        return m_GameLogicMgr.GetGameNiuNiuWnd();
    }
    public void ShowNiuniuWnd()
    {
        m_GameLogicMgr.ShowNiuniuWnd();
    }
    public void GetNiuniuTableState(NiuNiuGameLog log, VipSeatList list, BankerUserInfo TableBankerInfo, WriteBankerList TableWriteBankerList, 
        NiuNiuTableStates state, byte time, byte[] card, ushort[] result, ulong[] globalNum)
    {
        m_GameLogicMgr.GetNiuNiuTableState(log, list, TableBankerInfo, TableWriteBankerList, state, time, card, result, globalNum);
    }

    public void GetNiuniuHostline(WriteBankerList list)
    {
        m_GameLogicMgr.GetNiuniuHostline(list);
    }

    public void NiuniuSnatchHost()
    {
        m_GameLogicMgr.NiuniuSnatchHost();
    }

    public void NiuniuVipSeatChange(bool result, VipSeatList list)
    {
        m_GameLogicMgr.NiuniuVipSeatChange(result, list);
    }

    public void NiuniuSetVipHead(byte id, Texture2D tex)
    {
        m_GameLogicMgr.NiuniuSetVipHead(id, tex);
    }

    public void NiuniuSetNormalHead(Texture2D tex, uint id, byte index)
    {
        m_GameLogicMgr.NiuniuSetNormalHead(tex, id, index);
    }

    public void NiuniuBeLeaveVip(NiuNiuRoleInfo info)
    {
        m_GameLogicMgr.NiuniuBeLeaveVip(info);
    }

    public void UpdateNiuniuHost(BankerUserInfo info)
    {
        m_GameLogicMgr.UpdateNiuniuHost(info);
    }

    public void NiuniuGetNormalPeople(uint page, uint sum, NiuNiuRoleInfo[] array)
    {
        m_GameLogicMgr.NiuniuGetNormalPeople(page, sum, array);
    }

    public void NiuniuStopWeager()
    {
        m_GameLogicMgr.NiuniuStopWeager();
    }

    public void NiuniuVipWeager(byte seat, uint addglobal, byte index, uint allGlobal)
    {
        m_GameLogicMgr.NiuniuVipWeager(seat, addglobal, index, allGlobal);
    }

    public void NiuniuVipGlobalChange(uint id, byte seat, uint globalSum)
    {
        m_GameLogicMgr.NiuniuVipGlobalChange(id, seat, globalSum);
    }

    public void UpdateNiuniuDate(ulong[] date)
    {
        m_GameLogicMgr.UpdateNiuniuDate(date);
    }

    public void UpdateNiuniuMyWeagerNum()
    {
        m_GameLogicMgr.UpdateNiuniuMyWeagerNum();
    }

    public void GetNiuniuResult(BankerUserInfo tableBankUserInfo, VipSeatList oldVipInfo, VipSeatList newVipInfo, NiuNiuGameLog log, int state, TableBrandResult result)
    {
        m_GameLogicMgr.GetNiuniuResult(tableBankUserInfo ,oldVipInfo, newVipInfo, log, state, result);
    }

    public void SetNiuniuWeagerBtn()
    {
        m_GameLogicMgr.SetNiuniuWeagerBtn();
    }

    public void SetNiuniuMyGold()
    {
        m_GameLogicMgr.SetNiuniuMyGold();
    }
    public void NiuniuShutDown()
    {
        m_GameLogicMgr.ShutDown();
    }
    public void ShowBackToMain()
    {
        m_GameLogicMgr.ShowBackToMain();
    }
    public void SetPlayerHeader(Texture2D tex)
    {
        m_GameLogicMgr.SetPlayerHeader(tex);
    }
    public void SetNiuniuHostHeader(Texture2D tex)
    {
        m_GameLogicMgr.SetNiuniuHostHeader(tex);
    }
    public void UpdateTaskDate()
    {
        if (m_NameCardsUI.m_BaseWndObject == null)
            return;
        if (m_NameCardsUI.m_BaseWndObject.activeSelf != true)
            return;
        m_NameCardsUI.UpdateTaskDate();
    }
    public void UpdateTaskProgress(byte taskID)
    {
        if (m_NameCardsUI.m_BaseWndObject == null)
            return;
        if (m_NameCardsUI.m_BaseWndObject.activeSelf != true)
            return;
        m_NameCardsUI.UpdateTaskProgress(taskID);
    }
    public void ShutDownNameCardsUI()
    {
        m_NameCardsUI.ShutDown();
    }
    public Object[] FriendSysObj
    {
        get { return m_FriendSysObj; }
    }
    public Object[] ShopSysObj
    {
        get { return m_ShopSysObj; }
    }
    public Object[] GlobalUIObject
    {
        get { return m_GlobalUIObject; }
        set { m_GlobalUIObject = value; }
    }
    public Object GiftBtnEff
    {
        get { return m_objGiftBtneff; }
    }
    public void SetMyHeader(Texture2D tex)
    {
        m_HeadWind.SetMyHeader(tex);
    }
    public void UpdateExperienceDate()
    {
        m_HeadWind.UpdateExperienceData();
    }
    public void UpdateWarningSate()
    {
        m_HeadWind.UpdateWarningSate();
        m_NameCardsUI.UpdateCardsInfoWarningState();
    }
    public void UpdateMyMatchInfoDate()
    {
        m_HeadWind.UpdateMyMatchInfoDate();
    }
    public void UpdateMyMatchScoreInfo()
    {
        m_HeadWind.UpdateMyMatchScoreInfo();
    }
    public void UpdateOtherQueryUserCardsInfo(QueryRole pQuery)
    {
        m_NameCardsUI.UpdateOtherQueryUserCardsInfo(pQuery);
    }
    public void UpdateCardsInfoCharmDate(uint UserID, int CharmValue)
    {
        m_NameCardsUI.UpdateCardsInfoCharm(UserID, CharmValue);
    }
    public void UpdateCardsInfoDate(uint userID)
    {
        m_NameCardsUI.UpdateCardsInfoDate(userID);
    }
    public void UpdateOtherCardsInfoDate()
    {
        m_NameCardsUI.UpdateOtherCardsInfoData();
    }
    public void UpdateCardsInfoDel(uint userID)
    {
        m_NameCardsUI.UpdateCardsInfoDel(userID);
    }
    public void UpateCardsFriendBtnStatus()
    {
        m_NameCardsUI.UpateCardsFriendBtnStatus();
    }
    public void SetCardsHeader(uint UserID, Texture2D tex)
    {
        m_NameCardsUI.SetHeader(UserID, tex);
    }

    public void UpdateAccountMgrDate()
    {
        m_AccountMgrUI.UpdateAccountMgrDate();
    }
    public void UpdateAccountMindState(bool bResult, bool bFrist, byte type)
    {
        m_AccountMgrUI.UpdateAccountMindState(bResult, bFrist, type);
    }
    public Vector3 MatchScorePos()
    {
        return m_HeadWind.ScorePos();
    }
    public void UpdateMonthCardData()
    {
        m_VipMgrLogic.MonthCardLogicUI.UpdateMonthCardData();
    }
    public void UpdateVipPayProgressInfo()
    {
        m_VipMgrLogic.VipLogicUI.UpdateVipPayProgressInfo();
    }
    public void ApplyAddFriend(uint userID)
    {
        m_FriendSysMgr.ApplyCodeWnd.ShowApplyCodeWnd(userID);
    }

    //碰碰车
    public void ShowCarWnd()
    {
        m_GameLogicMgr.ShowCarWnd();
    }
     public bool GetCarLogicMgr()
    {
        return m_GameLogicMgr.GetCarLogicMgr();
    }
     public void SetCarBetBtn()
    {
        m_GameLogicMgr.SetCarBetBtn();
    }
     public void SetCarPlayerGold()
     {
         m_GameLogicMgr.SetCarPlayerGold();
     }
     public void UpdateCarMyWeagerNum()
     {
         m_GameLogicMgr.UpdateCarMyWeagerNum();
     }
     public void UpdateCarDate(ulong[] date)
     {
         m_GameLogicMgr.UpdateCarDate(date);
     }
     public void UpdateCarHost(CarBankerUserInfo info)
     {
         m_GameLogicMgr.UpdateCarHost(info);
     }
     public void GetCarHostline(CarWriteBankerList list)
     {
         m_GameLogicMgr.GetCarHostline(list);
     }
     public void CarGetNormalPeople(uint page, uint sum, CarRoleInfo[] array)
     {
         m_GameLogicMgr.CarGetNormalPeople(page, sum, array);
     }
     public void CarStopWeager()
     {
         m_GameLogicMgr.CarStopWeager();
     }
    public void CarStatusChange(     CarTableStates TableStates,
     CarTableUpdateInfo TableGlobelInfo,
     CarVipSeatList OldVipSeatInfo,
     CarVipSeatList TableVipSeatInfo,
     CarWriteBankerList TableWriteBankerList,
     CarBankerUserInfo TableBankerUserInfo,
     CarGameLog TableGameLog,
     byte TableResult)
     {
         m_GameLogicMgr.CarStatusChange(
         TableStates,
      TableGlobelInfo,
      OldVipSeatInfo,
      TableVipSeatInfo,
      TableWriteBankerList,
      TableBankerUserInfo,
      TableGameLog,
      TableResult);

     }
     public void EnterGameCar(byte TableUpdateSec,
         CarTableStates TableStates,
         CarTableUpdateInfo TableGlobelInfo,
         CarBankerUserInfo TableBankerInfo,
         CarWriteBankerList TableWriteBankerList,
         CarVipSeatList TableVipSeatInfo,
         CarGameLog TableGameLog,
         byte TableResult)
     {
         m_GameLogicMgr.EnterGameCar(TableUpdateSec,
       TableStates,
       TableGlobelInfo,
       TableBankerInfo,
       TableWriteBankerList,
    TableVipSeatInfo,
    TableGameLog,
    TableResult);
     }
    public void SetCarAvetarSelf(Texture2D tex)
     {
         m_GameLogicMgr.SetCarAvetarSelf(tex);
     }
    public void SetCarAvetarHost(Texture2D tex)
    {
        m_GameLogicMgr.SetCarAvetarHost(tex);
    }
    public void SetCarAvetarPlayList(Texture2D tex, uint id, byte data)
    {
        m_GameLogicMgr.SetCarAvetarPlayList(tex,id,data);
    }
    public void CarSnatchHost()
    {
        m_GameLogicMgr.CarSnatchHost ();
    }
    public void CarShutDown()
    {
        m_GameLogicMgr.ShutDown();
    }
    //森林舞会
    public bool GetForestDanceMgr()
    {
        return m_GameLogicMgr.GetGameForestDanceWnd();
    }

    public void ForestDanceShowWnd()
    {
        m_GameLogicMgr.ShowForestDanceWnd();
    }

    public void ForestDanceSetPlayerHead(Texture2D tex)
    {
        m_GameLogicMgr.ForestDanceSetPlayerHead(tex);
    }

    public void ForestDanceGetTableState(byte tableUpdateSec, DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo,
        DialTableAreaData AreaData, DialBankerUserInfo TableBankerInfo, DialWriteBankerList TableWriteBankerList,
        DialVipSeatList TableVipSeatInfo, DialGameLog TableGameLog, uint tableResult)
    {
        m_GameLogicMgr.ForestDanceGetTableState(tableUpdateSec, TableStates, TableGlobelInfo,
        AreaData, TableBankerInfo, TableWriteBankerList,
        TableVipSeatInfo, TableGameLog, tableResult);
    }

    public void ForestDanceStateChange(DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo,
        DialTableAreaData AreaData, DialVipSeatList OldVipSeatInfo, DialVipSeatList TableVipSeatInfo,
        DialWriteBankerList TableWriteBankerList, DialBankerUserInfo TableBankerUserInfo, 
        DialGameLog TableGameLog, uint tableResult)
    {
        m_GameLogicMgr.ForestDanceStateChange(TableStates, TableGlobelInfo,
        AreaData, OldVipSeatInfo, TableVipSeatInfo,
        TableWriteBankerList, TableBankerUserInfo, 
        TableGameLog, tableResult);
    }

    public void ForestDanceUpdateTotalWeager(DialTableUpdateInfo TableGlobelInfo, DialTableStates TableStates)
    {
        m_GameLogicMgr.ForestDanceUpdateTotalWeager(TableGlobelInfo, TableStates);
    }

    public void ForestDanceUpdateMyWeager(uint[] sum)
    {
        m_GameLogicMgr.ForestDanceUpdateMyWeager(sum);
    }

    public void ForestDanceVipChange(bool Result, DialVipSeatList TableVipList)
    {
        m_GameLogicMgr.ForestDanceVipChange(Result, TableVipList);
    }

    public void ForestDanceSetVipHead(byte index, Texture2D tex)
    {
        m_GameLogicMgr.ForestDanceSetVipHead(index, tex);
    }

    public void ForestDanceLeaveVip(DialRoleInfo info)
    {
        m_GameLogicMgr.ForestDanceLeaveVip(info);
    }

    public void ForestDanceGetHostLine(DialWriteBankerList list)
    {
        m_GameLogicMgr.ForestDanceGetHostLine(list);
    }

    public void ForestDanceUpdateHost(DialBankerUserInfo info)
    {
        m_GameLogicMgr.ForestDanceUpdateHost(info);
    }

    public void ForestDanceSetHostHead(Texture2D tex)
    {
        m_GameLogicMgr.ForestDanceSetHostHead(tex);
    }

    public void ForestDanceGetNormal(uint Page, uint TotalRoleSum, DialRoleInfo[] Array)
    {
        m_GameLogicMgr.ForestDanceGetNormal(Page, TotalRoleSum, Array);
    }

    public void ForestDanceSetNormalHead(Texture2D tex, uint id, byte data)
    {
        m_GameLogicMgr.ForestDanceSetNormalHead(tex, id, data);
    }

    public void ForestDanceSetPlayerGold()
    {
        m_GameLogicMgr.ForestDanceSetMyGold();
    }

    public void ForestDanceSetWeagerBtn()
    {
        m_GameLogicMgr.ForestDanceSetWeagerBtn();
    }         
    public void ShutForset()
    {
        m_GameLogicMgr.ShutDown();
    }

    public GameShare GameShare
    {
        get { return m_GameShare; }
    }
    public void SetGameShareAvatar(Texture2D texture )
    {
        m_GameShare.SetAvatar(texture);

    }
   
}
