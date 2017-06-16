using UnityEngine;
using System.Collections.Generic;

public class HallLogicUI_Main:HallLoginUI_BaseWind
{
    CenterRightBtn[]    m_CenterRightBtn ;
    CenterLiftBtn       m_CenterLiftBtn = new CenterLiftBtn();
    GameObject          m_CenterObj;
    GameObject[]        m_INfObj = new GameObject[3];
    GameObject[]        m_LeftWarning = new GameObject[2];     //有任务完成或活动完成界面红点提示
    WindBtn[]           m_LiftBtn = new WindBtn[4];
    Benefit m_benefit = new Benefit();
    Object[]            m_EffectRigthBtn = new Object[3];
    Object[]            m_EffectLiftBtn = new Object[3];
   // Object              m_effectReward = new Object();
    Transform[]         m_LeftTrans = new Transform[2];
    Vector3             m_CDKeyOldPos;
    HallLogicUI_RotateBtns m_RotateBtns = new HallLogicUI_RotateBtns();
    GameObject[]        m_RealRightBtn;

    public Object Init()
    {
        Object obj = Initobj("MianHallWind");
        WindID.m_state = HallLogicUIStatue.Hall_State.Hall_Mian;
        WindID.m_SmailWind = 0;
        string[] EffectRName = { "Fish_MainUI_Lianji ", "Fish_MainUI_Jingsai", "Fish_MainUI_Chuangguan" };
        string[] EffectLName = { "FishCrownLight","FishYachiLight", "FishSmokeEffect"  };
        for (byte i = 0; i < 3; ++i)
        {
            m_EffectLiftBtn[i] = ResManager.Instance.LoadObject(EffectLName[i], "HallRes/Prefab/Effect/UIFishEffect/", ResType.HallRes);
            m_EffectRigthBtn[i] = ResManager.Instance.LoadObject(EffectRName[i], "HallRes/Prefab/Effect/UIModeEffect/", ResType.HallRes);
        }
        //m_effectReward = ResManager.Instance.LoadObject("Ef_TimeBox_01", "HallRes/Prefab/Effect/UITimeBox/", ResType.HallRes);
        return obj;
    }
    public void InitGameObj()
    {
        Init_GameObj();
        InitCenterBtn();
        InitLiftBtn();
    }
    void InitCenterBtn()
    {
        //Right
        Transform baseTr_R = BaseTranF.GetChild(0);
        m_INfObj[0] = baseTr_R.gameObject;

        m_RotateBtns.Init(baseTr_R);
        m_CenterRightBtn = new CenterRightBtn[baseTr_R.childCount];
        
        for (byte i = 0; i < baseTr_R.childCount; ++i)
        {
            m_CenterRightBtn[i].m_Btn.m_BtnObj = baseTr_R.GetChild(i).gameObject;
            m_CenterRightBtn[i].m_Btn.m_Btn = m_CenterRightBtn[i].m_Btn.m_BtnObj.GetComponent<UIButton>();
            m_CenterRightBtn[i].m_Lock = baseTr_R.GetChild(i).GetChild(1).gameObject;
            if (i == 1)
            {
                m_CenterRightBtn[i].m_Btn.m_IsChecked = true;
                //m_CenterRightBtn[i].m_Btn.m_Btn.isEnabled = true;
                m_CenterRightBtn[i].m_bLock = false;
            }
            else
            {
                m_CenterRightBtn[i].m_bLock = true;
                m_CenterRightBtn[i].m_Btn.m_IsChecked = false;
                if (i != 2)
                    m_CenterRightBtn[i].m_Lock.SetActive(true);
                // m_CenterRightBtn[i].m_Btn.m_Btn.isEnabled = false;
            }
            m_CenterRightBtn[i].m_Btn.m_Tag = i;
            //UIEventListener listener = GetBtnLister(m_CenterRightBtn[i].m_Btn.m_BtnObj);
            //listener.onClick = OnCheckCenterRightBtn;
            //listener.onDrag = m_RotateBtns.OnDrag;
            //listener.onDragEnd = m_RotateBtns.OnDragEnd;

            m_CenterRightBtn[i].m_Tween.m_alpha = m_CenterRightBtn[i].m_Btn.m_Btn.gameObject.GetComponent<TweenAlpha>();
            m_CenterRightBtn[i].m_Tween.m_Pos = m_CenterRightBtn[i].m_Btn.m_Btn.gameObject.GetComponent<TweenPosition>();
            //GameObject GEobj = GameObject.Instantiate(m_EffectRigthBtn[i]) as GameObject;
            //GEobj.transform.SetParent(m_CenterRightBtn[i].m_Btn.m_BtnObj.transform,false);
        }
        m_CenterRightBtn[2].m_Btn.m_BtnObj.SetActive(ServerSetting.ShowHallThirdBtn);

        Transform rotateBG = BaseTranF.GetChild(3);
        GetBtnLister(rotateBG.gameObject).onDrag = m_RotateBtns.OnDrag;
        GetBtnLister(rotateBG.gameObject).onDragEnd = m_RotateBtns.OnDragEnd;

        m_RealRightBtn = new GameObject[baseTr_R.childCount];
        for(int i=0;i< baseTr_R.childCount;i++)
        {
            m_RealRightBtn[i] = BaseTranF.GetChild(4 + i).gameObject;
            GetBtnLister(m_RealRightBtn[i]).onClick = OnClickRealRightBtn;
            GetBtnLister(m_RealRightBtn[i]).onDrag = m_RotateBtns.OnDrag;
            GetBtnLister(m_RealRightBtn[i]).onDragEnd = m_RotateBtns.OnDragEnd;
        }

        //left
        Transform baseTr_L = BaseTranF.GetChild(1);
        m_CenterObj = baseTr_L.gameObject;
        m_INfObj[1] = baseTr_L.gameObject;
        m_CenterLiftBtn.m_BGSp = baseTr_L.GetChild(0).GetComponent<UISprite>();
        m_CenterLiftBtn.m_Btn.m_Btn = baseTr_L.GetComponent<UIButton>();
        m_CenterLiftBtn.m_Btn.m_IsChecked = false;
        m_CenterLiftBtn.m_Btn.m_Tag = 0;
        GetBtnLister(m_CenterLiftBtn.m_Btn.m_Btn.gameObject).onClick = OnClickCenterLiftBtn;
        m_CenterLiftBtn.m_Lock = baseTr_L.GetChild(1).gameObject;
        m_CenterLiftBtn.m_Title = baseTr_L.GetChild(2).GetComponent<UISprite>();
        m_CenterLiftBtn.m_Effect = new GameObject[3];
        for (byte i = 0; i < 3; ++i)
        {
            m_CenterLiftBtn.m_Effect[i] =  GameObject.Instantiate(
                
                m_EffectLiftBtn[i]) as GameObject;
            m_CenterLiftBtn.m_Effect[i].transform.SetParent(m_CenterLiftBtn.m_BGSp.transform, false);
            GlobalEffectMgr.SetEffectOnUI(m_CenterLiftBtn.m_Effect[i]);
        }
        GlobalEffectMgr.SetEffectOnUI(baseTr_L.GetChild(0).GetChild(0).gameObject);
        m_CenterLiftBtn.m_BtnTag = 1;
        
        Center_L_BtnBgChange(1);
        //m_CenterRightBtn[2].m_Btn.m_BtnObj.SetActive(ServerSetting.ShowMatch);
    }
    void InitLiftBtn()
    {
        Transform baseT = BaseTranF.GetChild(2);
        m_INfObj[2] = baseT.gameObject;
        for (byte i = 0; i < m_LiftBtn.Length; ++i)
        {
            if (i == 2)//时间
            {
                m_LiftBtn[i].m_BtnObj = baseT.GetChild(i).GetChild(1).gameObject;
                m_LeftTrans[i - 2] = baseT.GetChild(i);
            }
            else if (i == 3)
            {
                m_LiftBtn[i].m_BtnObj = baseT.GetChild(i).gameObject;
                m_LeftTrans[i - 2] = baseT.GetChild(i);
                m_CDKeyOldPos = m_LeftTrans[i - 2].localPosition;


            }
            else
            {
                m_LiftBtn[i].m_BtnObj = baseT.GetChild(i).gameObject;
                m_LeftWarning[i] = baseT.GetChild(i).GetChild(0).gameObject;
            }
            m_LiftBtn[i].m_Btn = m_LiftBtn[i].m_BtnObj.GetComponent<UIButton>();
            m_LiftBtn[i].m_IsChecked = false;
            m_LiftBtn[i].m_Tag = i;
            GetBtnLister(m_LiftBtn[i].m_BtnObj).onClick = ONClickLiftBtn;
        }

        m_benefit.Init(baseT.GetChild(2),false);

        //m_LeftTrans[1].localPosition = m_LeftTrans[0].localPosition;
        if (PlayerRole.Instance.RoleExChange.IsShowExChangeUI)
            m_LiftBtn[3].m_BtnObj.SetActive(true);
        else
            m_LiftBtn[3].m_BtnObj.SetActive(false);
        if(SDKMgr.PACKAGE_NAME == "com.leduo.buyu3d")
            m_LiftBtn[3].m_BtnObj.SetActive(false);

    }
    public void Update(float dTime)
    {
        if (m_benefit!=null)
        {
            m_benefit.Update(dTime);
            //if (PlayerRole.Instance.RoleExChange.IsShowExChangeUI)
            //{
            //    if (m_benefit.m_bNeedChangePos)
            //    {
            //        tagOnceOnlienReward newReward = PlayerRole.Instance.RoleOnlineReward.FindValiedReward();
            //        if (newReward != null)
            //        {
            //            m_LeftTrans[1].localPosition = m_CDKeyOldPos;
            //        }
            //        else
            //        {
            //            m_LeftTrans[1].localPosition = m_LeftTrans[0].localPosition;

            //        }
            //        m_benefit.m_bNeedChangePos = false;

            //    }
            //}
        }
    }
    public void UpdateActivityTaskWarningState(byte type)
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        switch(type)
        {
                
            case 0:     //任务
                m_LeftWarning[0].SetActive(PlayerRole.Instance.RoleStatesMessage.GetTaskStates());                        
                break;
            case 1:     //活动
                if (PlayerRole.Instance.RoleStatesMessage.GetActionStates() || PlayerRole.Instance.RoleStatesMessage.GetCheckStates())
                    m_LeftWarning[1].SetActive(true);   
                else
                    m_LeftWarning[1].SetActive(false);   
     
                break;
        }
    }
    void Center_L_BtnBgChange(byte tag)
    {
        //string[] BgSpName = {"MainHal_ManyPeople1","MainHal_Match1","MainHal_Coustoms1"};
        //string[] TitleName = {"MainHal_ManyPeople2", "MainHal_Match2" , "MainHal_Coustoms2" };
        //int[] SpWidth = { 220, 252,222 };
        //int[] spHeight = { 174, 226, 230 };
        m_CenterLiftBtn.m_BtnTag = tag;
        //m_CenterLiftBtn.m_BGSp.spriteName = BgSpName[tag];
        //m_CenterLiftBtn.m_BGSp.width = SpWidth[tag];
        //m_CenterLiftBtn.m_BGSp.height = spHeight[tag];
        //m_CenterLiftBtn.m_Title.spriteName = TitleName[tag];
        //if (tag == 0)
        //{
        //    if (m_CenterLiftBtn.m_Lock.activeSelf)
        //        m_CenterLiftBtn.m_Lock.SetActive(false);
        //    m_CenterLiftBtn.m_Btn.m_Btn.isEnabled = true;
        //}
        //else 
        //{
        //    if (m_CenterRightBtn[tag].m_Lock.activeSelf)
        //    {
        //        m_CenterLiftBtn.m_Lock.SetActive(true);
        //        m_CenterLiftBtn.m_Btn.m_Btn.isEnabled = false;
        //    }
        //    else
        //    {
        //        m_CenterLiftBtn.m_Lock.SetActive(false);
        //        m_CenterLiftBtn.m_Btn.m_Btn.isEnabled = true;
        //    }
        //}

        //for (byte i = 0; i < m_CenterLiftBtn.m_Effect.Length; ++i)
        //{
        //    if (i == tag)
        //    {
        //        if (!m_CenterLiftBtn.m_Effect[i].activeSelf)
        //            m_CenterLiftBtn.m_Effect[i].SetActive(true);
        //    }
        //    else
        //    {
        //        if (m_CenterLiftBtn.m_Effect[i].activeSelf)
        //            m_CenterLiftBtn.m_Effect[i].SetActive(false);
        //    }
                
        //}

        int count = m_INfObj[1].transform.childCount;
        for (int i = 0; i < count; i++)
        {
            Transform child = m_INfObj[1].transform.GetChild(i);
            if (child != null && child.gameObject.activeSelf)
            {
                child.gameObject.SetActive(false);
            }
        }
    }
    void OnCheckCenterRightBtn(GameObject go)
    {
        PlayBtnMusic();
        byte checktag = 0;
        for(byte i = 0 ; i < m_CenterRightBtn.Length;++i)
        {
            if (m_CenterRightBtn[i].m_Btn.m_Btn.gameObject == go)
            {
                if (m_CenterRightBtn[i].m_bLock && i ==0)
                {
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Select_Match_Failed"), 1.5f);
                    return;
                }
                if (i==2)//(m_CenterRightBtn[i].m_bLock && i == 2)
                {
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Select_Match_Failed1"), 1.5f);
                    return;
                }
                checktag = m_CenterRightBtn[i].m_Btn.m_Tag;
            }
        }
        Center_L_BtnBgChange(checktag);

        // Modify 2017/04/14
        OnClickCenterLiftBtn(null);
    }
    void ONClickLiftBtn(GameObject go)
    {
        PlayBtnMusic();
        byte ClickTag = 0;
        bool HaveClick = false;
        for (byte i = 0; i < m_LiftBtn.Length; ++i)
        {
            if (m_LiftBtn[i].m_BtnObj == go)
            {
                ClickTag = m_LiftBtn[i].m_Tag;
                m_LiftBtn[i].m_IsChecked = true;
                HaveClick = true;
            }
            else
                m_LiftBtn[i].m_IsChecked = false;
        }
        if (HaveClick)
        {
            switch (ClickTag)
            {
                case 0:        //任务
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.ChangeInfmationWindType(InfmationWind.Wind_Task);
                        HallRunTimeInfo.Instance_UI.m_loginUi.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Information);
                    }
                    break;
                case 1:     //活动
                    {
                        HallRunTimeInfo.Login_UI.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Activity);
                    }
                    break;
                case 2:     //宝箱
                    {
                        m_benefit.OnClick();                     
                        break;
                    }
                case 3:         //对兑码
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.CDKeyWnd.ShowCDKeyWnd();
                        break;
                    }
                default:
                    break;
            }
        }
    }
    void OnClickCenterLiftBtn(GameObject go)
    {
        PlayBtnMusic();
        if (m_CenterLiftBtn.m_BtnTag == 1)
        {
            HallRunTimeInfo.Instance_UI.m_loginUi.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_SelectRoom);
        }
        else if (m_CenterLiftBtn.m_BtnTag == 0)
        {//比赛 
            HallRunTimeInfo.Login_UI.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Contest);
            HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(false);
        }
        else 
        {
        }

    }
    void OnClickRealRightBtn(GameObject go)
    {
        int index = -1;
        for (int i = 0; i < m_RealRightBtn.Length; i++)
        {
            if (m_RealRightBtn[i] == go)
            {
                index = i;
                break;
            }
        }

        if (index >= 0)
        {
            int rotateIndex = Period(m_RotateBtns.CurrentIndex, m_RealRightBtn.Length);
            Debug.Log("index " + index + " CurrentIndex " + m_RotateBtns.CurrentIndex + " rotateIndex " + rotateIndex);

            index -= rotateIndex;
            index = Period(index, m_RealRightBtn.Length);
            OnCheckCenterRightBtn(m_CenterRightBtn[index].m_Btn.m_Btn.gameObject);
        }
    }

    private int Period(int amount, int period)
    {
        int v = amount;
        if (v > 0)
        {
            v = v % period;
        }
        else if (v < 0)
        {
            int times = Mathf.CeilToInt((-v) * 1.0f / period);
            v = v + times * period;
        }
        return v;
    }
    public void UpdateMatchSate()
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetLevel() >= FishConfig.Instance.m_MatchInfo.BeginMonthLevel)
        {
            if (!HallRunTimeInfo.Login_UI.MatchGuide.GetMathGuide())
            {
                m_CenterRightBtn[0].m_bLock = false;
                m_CenterRightBtn[0].m_Lock.SetActive(false);
                // m_CenterRightBtn[1].m_Btn.m_Btn.isEnabled = true;
            }
            else
            {
                m_RotateBtns.ShowMatchGuide();
            }
        }

    }
    public void OpenRightLock()
    {
        m_CenterRightBtn[0].m_Lock.SetActive(false);
        m_CenterRightBtn[0].m_bLock = false;

      //  m_CenterRightBtn[1].m_Btn.m_Btn.isEnabled = true;

    }
    public void SetLeftCenterBtn(bool bShow)
    {
        m_CenterObj.SetActive(bShow);
    }
    public void UpdateTimeBox(byte byId/*以分钟计算*/)
    {       
        tagOnceOnlienReward pReward;
        if (FishConfig.Instance.m_OnlineRewardConfig.m_OnlineRewardMap.TryGetValue(byId, out pReward))
        {
            GlobalHallUIMgr.Instance.ShowTipBox_0(TipBox_0.TipBox_0_ZiTi.Online, 1.0f, pReward.RewardID);         
        }       
    }
    public void ShowMainWindInf(bool bShow)//是否显示主界面的信息
    {
        if (BaseTranF == null)
            return;
        for (int i = 0; i < m_INfObj.Length; ++i)
            if (m_INfObj[i].activeSelf != bShow)
                m_INfObj[i].SetActive(bShow);
    }
    struct CenterRightBtn
    {
        public WindBtn      m_Btn;
        public GameObject   m_Lock;
        public WindTwreen   m_Tween;
        public bool         m_bLock;
    }
    struct CenterLiftBtn
    { 
        public WindBtn m_Btn;
        public GameObject m_Lock;
        public UISprite m_BGSp;
        public UISprite m_Title;
        public GameObject[] m_Effect;
        public byte m_BtnTag;
        public bool m_bLock;

    }
}

public class Benefit
{
    public GameObject m_ObjItem;
    public GameObject BoxObj;
    public UILabel m_TimeLabel;
    
    public TweenRotation m_tweenRot;
    public TweenScale m_tweenScale;
    public bool m_bNeedChangePos = false;
    private bool m_bInScene;

    private float m_fUpdate = 0;
    public void Init(Transform tf,bool bInscene)
    {
        m_ObjItem = tf.gameObject;
        BoxObj = m_ObjItem.transform.GetChild(1).gameObject;
        m_tweenRot = BoxObj.GetComponent<TweenRotation>();
        m_tweenScale = BoxObj.GetComponent<TweenScale>();
        m_tweenRot.enabled = false;
        m_tweenScale.enabled = false;

        m_TimeLabel = m_ObjItem.transform.GetChild(2).GetComponent<UILabel>();
        m_bInScene = bInscene;
    }
    bool IsPoor()
    {
        if (!m_bInScene)
        {
            return PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() <FishConfig.Instance.m_RoleProtect.OpenProtectGlobelSum;
        }
        else 
        {

            TableInfo pTableInfo;
            if (!FishConfig.Instance.m_TableInfo.m_TableConfig.TryGetValue(PlayerRole.Instance.RoleInfo.RoleMe.GetTableTypeID(), out pTableInfo))
            {
                return false;
            }

            return PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < LauncherSetting.LauncherDataList[0].Consume * BulletSetting.BulletRate[pTableInfo.MinRate] && !SceneRuntime.SceneLogic.BulletMgr.HaveBullet(SceneRuntime.PlayerMgr.MyClientSeat);
        }
    }
    public void OnClick()
    {
        if (!IsPoor())
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("BenefitGold"), MssageType.BeneFit_Msg_6);
            return;
        }  
        if (PlayerRole.Instance.RoleInfo.RoleMe.BeneFitCount >= PlayerRole.Instance.RoleVip.AddAlmsSum())
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("BenefitCount"), MssageType.BeneFit_Msg_6);
            return;
        }
        if (SystemTime.Instance.GetSystemTimeTotalSecond()<PlayerRole.Instance.RoleInfo.RoleMe.BenefitTime+FishConfig.Instance.m_RoleProtect.ProtectCDMin*60)  
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("BenefitTime"), MssageType.BeneFit_Msg_6);
            return;
        }
        CL_Cmd_RoleProtect msg = new CL_Cmd_RoleProtect();
        msg.SetCmdType(NetCmdType.CMD_CL_RoleProtect);
        NetServices.Instance.Send<CL_Cmd_RoleProtect>(msg);
    }
    void Rock(bool bRock)
    {
        if(bRock)
        {
            if (!m_tweenRot.enabled)
            {
                m_tweenRot.enabled = true;
                m_tweenScale.enabled = true;
            }
        }
        else
        {
            if (m_tweenRot.enabled)
            {
                m_tweenRot.enabled = false;
                m_tweenScale.enabled = false;
            }
        }
    }
    public bool Update(float dTime)
    {
        if (m_ObjItem == null)
        {
            return true;
        }

        m_fUpdate -= dTime;
        if (m_fUpdate > 0)
        {
            return true;
        }
        else
        {
            m_fUpdate = 0.2f;
        }

        int nTime = 0;
        if (PlayerRole.Instance.RoleInfo.RoleMe.BenefitTime + FishConfig.Instance.m_RoleProtect.ProtectCDMin * 60 >= SystemTime.Instance.GetSystemTimeTotalSecond())
        {
            nTime = (int)(PlayerRole.Instance.RoleInfo.RoleMe.BenefitTime + FishConfig.Instance.m_RoleProtect.ProtectCDMin * 60 - SystemTime.Instance.GetSystemTimeTotalSecond());
        }        

        if(!m_bInScene)
        {

            Rock(PlayerRole.Instance.RoleInfo.RoleMe.BeneFitCount < PlayerRole.Instance.RoleVip.AddAlmsSum() && IsPoor() && nTime==0);       
        }
        else
        {            
            m_ObjItem.SetActive(PlayerRole.Instance.RoleInfo.RoleMe.BeneFitCount < PlayerRole.Instance.RoleVip.AddAlmsSum() && IsPoor());           
            if (!m_ObjItem.activeSelf)
            {
                return true;
            }              
            Rock(nTime==0);
            m_TimeLabel.gameObject.SetActive(nTime!=0);
            m_TimeLabel.text = (nTime / 60).ToString("D2") + ":" + (nTime % 60).ToString("D2");
        }                   
        return true;
    }
}