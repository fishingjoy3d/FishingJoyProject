using UnityEngine;
using System.Collections.Generic;

public class HallLogicUIStatue
{
    Hall_STATE m_HallState;
    public void Init()
    {
        m_HallState = new Hall_STATE();
        m_HallState.m_state = Hall_State.Hall_Mian;
        m_HallState.m_SmailWind = 0;
    }
    
    public  HallLogicUIStatue.Hall_STATE GetHallStatue()
    {
        return m_HallState;
    }
    public void ChangeWindStatue(HallLogicUIStatue.Hall_State statue)
    {
        m_HallState.m_state = statue;
        m_HallState.m_SmailWind = 0;
        
    }
    public class Hall_STATE
    {
        public HallLogicUIStatue.Hall_State m_state;
        public byte m_SmailWind;
    }
    public enum Hall_State //大厅状态
    {
        Hall_Btns = 0,//按钮界面
        Hall_Mian,//主大厅
        Hall_SelectRoom,//选着房间
        Hall_Information,//个人信息界面
        Hall_Exchange,//兑换界面
        Hall_Contest,//竞赛界面
        Hall_Activity,//活动界面
    }
}

public class HallLogicUI:ILogicUI
{
    HallLogic                       m_Logic;
    HallLogicUIStatue               m_UIStatue = new HallLogicUIStatue();
    List<Object>                    m_WindObjList = new List<Object>();//用作界面最后的销毁            
    HallLogicUI_Main                m_MainWind = new HallLogicUI_Main();
    HallLoginUI_Btns                m_BtnWind = new HallLoginUI_Btns();
    HallLoginUI_Room                m_RoomWind = new HallLoginUI_Room();
    HallLoginUI_Infomation          m_InfomationWind = new HallLoginUI_Infomation();
    HallLoginUI_Contest             m_MatchWind = new HallLoginUI_Contest();
    HallLoginUI_Activity            m_ActivityWind = new HallLoginUI_Activity();
    HallLoginUI_MatchGuide          m_MatchGuide = new HallLoginUI_MatchGuide();
    HallLoginUI_CDKey m_CDKeyWnd = new HallLoginUI_CDKey();       //兑换码窗口

    public HallLogicUI_Main MainWind
    {
        get { return m_MainWind; }
    }
    public HallLoginUI_Btns BtnWind
    {
        get { return m_BtnWind; }
    }
    public HallLoginUI_MatchGuide MatchGuide
    {
        get { return m_MatchGuide; }
    }
    public HallLoginUI_Infomation InfomationWind
    {
        get { return m_InfomationWind; }
    }
    public HallLoginUI_Contest MatchWind
    {
        get { return m_MatchWind; }
    }
    public HallLoginUI_CDKey CDKeyWnd
    {
        get { return m_CDKeyWnd; }
    }
    public bool Init(ILogic logic, object obj)
    {
        m_Logic = (HallLogic)logic;
        SceneMain.Instance.StartInnerCoroutine(MainInitProcedure(null));
        return true;
    }
    public System.Collections.IEnumerator MainInitProcedure(object obj)
    {
        m_UIStatue.Init();
     //   m_HallBtnTouchBoxPool.Init();
        HallRunTimeInfo.Init_UI(this);
        yield return new WaitForEndOfFrame();
        m_BtnWind.Init();
        m_WindObjList.Add(m_MainWind.Init()); yield return new WaitForEndOfFrame();
        m_WindObjList.Add(m_RoomWind.Init());
        m_WindObjList.Add(m_InfomationWind.Init()); yield return new WaitForEndOfFrame();
        m_WindObjList.Add(m_MatchWind.Init());
        m_WindObjList.Add(m_ActivityWind.Init()); yield return new WaitForEndOfFrame();
        m_MatchGuide.InitObj(); yield return new WaitForEndOfFrame();
        m_CDKeyWnd.InitObj(); yield return new WaitForEndOfFrame();
        GlobalHallUIMgr.Instance.GameLogicMgr.Init();
        HallInformationInit(false);
        while (HallRunTimeInfo.IsHallInfoInitCompletion() == false)
            yield return new WaitForEndOfFrame();

        ChangeWind();
        LogicManager.Instance.InitCompletion();
        GlobalEffectMgr.Instance.CloseLoadingMessage();
        GlobalHallUIMgr.Instance.AdjustNoticePos();
        if (PlayerRole.Instance.RoleStatesMessage.GetCheckStates())
        {
            ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Activity);
        }
        yield break;
    }
    public void SetMyHeader(Texture2D tex)
    {
        m_InfomationWind.SetMyHeader(tex);
    }
    public void ChangeHallWind(HallLogicUIStatue.Hall_State statue)
    {
        if (statue == HallLogicUIStatue.Hall_State.Hall_Contest||statue == HallLogicUIStatue.Hall_State.Hall_Activity)
            MyShutDown(statue);
        else
            MyShutDown(GetWindStatue().m_state);
        m_UIStatue.ChangeWindStatue(statue);
        ChangeWind();
    }
    public HallLogicUIStatue.Hall_STATE GetWindStatue()
    {
        return m_UIStatue.GetHallStatue();
    }
  
    void ChangeWind()
    {
        if (UI_IsNotInScene(m_UIStatue.GetHallStatue().m_state))
        {
            switch (m_UIStatue.GetHallStatue().m_state)
            {
                case HallLogicUIStatue.Hall_State.Hall_Mian:
                    HallMainInit(); break;
                case HallLogicUIStatue.Hall_State.Hall_SelectRoom:
                    HallSelectRoomInit(); break;
                case HallLogicUIStatue.Hall_State.Hall_Information:
                    HallInformationInit(true); break;
                case HallLogicUIStatue.Hall_State.Hall_Exchange:
                    HallExchangeInit(); break;
                case HallLogicUIStatue.Hall_State.Hall_Btns:
                    break;
                case HallLogicUIStatue.Hall_State.Hall_Contest:
                    HallContestInit(); break;
                case HallLogicUIStatue.Hall_State.Hall_Activity:
                    HallActivity();break;
                default:
                    break;
            }
        }
        else
        {
            if (m_UIStatue.GetHallStatue().m_state == HallLogicUIStatue.Hall_State.Hall_Mian)
                ShowMainWindCenterInf();
        }
        //修改比赛界面进入个人信息界面返回后不显头像Bug
        if (m_UIStatue.GetHallStatue().m_state != HallLogicUIStatue.Hall_State.Hall_Information)
        {
            GlobalHallUIMgr.Instance.ShowHead(HeadWind.HeadStatue.Head_Normal);
        }
    }
    public void ChangeInfmationWindType(InfmationWind InfmationWindType)
    { 
        m_InfmationWindType = InfmationWindType;
    }
    void HallContestInit()
    {
        if(m_MatchWind.BaseTranF ==null)
            m_MatchWind.InitGameobject();
        m_BtnWind.SetBtnUIStatue(HallLoginUI_Btns.WindBtnStatue.HallBtn_Contest);

        UpdateMatchData();
    }
    void HallMainInit()
    {
        if (m_MainWind.BaseTranF == null)
            m_MainWind.InitGameObj();
        ShowMainWindCenterInf();
        m_MainWind.UpdateMatchSate();
        m_BtnWind.SetBtnUIStatue(HallLoginUI_Btns.WindBtnStatue.HallBtn_Mian);
        for (byte i = 0; i < 5; ++i)
        {
            m_MainWind.UpdateActivityTaskWarningState(i);
            m_BtnWind.UpdateWarningState(i);
        }     
    }
    void HallSelectRoomInit()
    {
        m_RoomWind.InitGameObj();
       m_BtnWind.SetBtnUIStatue(HallLoginUI_Btns.WindBtnStatue.HallBtn_SelectScene);
    }
    InfmationWind m_InfmationWindType = InfmationWind.Wind_UserInF;

    System.Collections.IEnumerator HallInformationInit_Procedure(object obj)
    {
        GlobalHallUIMgr.Instance.CloseHead();

        HallRunTimeInfo.SetHallInfoBeginInit();
        SceneMain.Instance.StartInnerCoroutine(m_InfomationWind.InitGameObj(m_InfmationWindType));
        while(m_InfomationWind.IsInit)
            yield return new WaitForEndOfFrame();

        if((bool)obj)
            m_BtnWind.SetBtnUIStatue(HallLoginUI_Btns.WindBtnStatue.HallBtn_Information);

        if (PlayerRole.Instance.RoleInfo.RoleMe != null)
            UpDateInfomationUserInfContent();

        yield return new WaitForEndOfFrame();
        if (PlayerRole.Instance.RankManager.GetRankInfo() != null)
            UpdateInfomationRankContent();
        yield return new WaitForEndOfFrame();
        if (PlayerRole.Instance.AchievementManager.GetAchievementArray() != null)
            UpdateAchievementDate();
        yield return new WaitForEndOfFrame();
        if (PlayerRole.Instance.TaskManager.GetTaskArray() != null)
            UpdateInfomationTaskContent();

        GlobalLoading.Instance.Hide();
        HallRunTimeInfo.SetHallInfoInitCompletion();
        yield break;
    }
    void HallInformationInit(bool showbtn)
    {
        SceneMain.Instance.StartInnerCoroutine(HallInformationInit_Procedure(showbtn));
    }
    void HallExchangeInit()
    { }
    void HallActivity()
    {
        if (m_ActivityWind.BaseTranF == null)
            m_ActivityWind.InitGameObject();
        ShowMainWindCenterInf(false);
    }
    public void Update(float delta)
    {
        switch (m_UIStatue.GetHallStatue().m_state)
        {
            case HallLogicUIStatue.Hall_State.Hall_Mian:
                    m_MainWind.Update(delta); break;
            case HallLogicUIStatue.Hall_State.Hall_SelectRoom:
                { } break;
            case HallLogicUIStatue.Hall_State.Hall_Contest:
                m_MatchWind.Update(delta);break;
            case HallLogicUIStatue.Hall_State.Hall_Activity:
                m_ActivityWind.Update(delta);break;
            default:
                break;
        }
        m_BtnWind.Update(delta);
        m_MatchGuide.Update(delta);
    }
    public void UpDateInfomationUserInfContent()
    {
        m_InfomationWind.UpDateUserInfContent();
    }
    public void UpdatePlayerGameDate()
    {
        m_InfomationWind.UpdatePlayerGameDate();
    }
    public void UpdateUserTitle()
    {
        m_InfomationWind.UpdateUserTitle();
    }
    public void UpdateInfomationRankContent()
    { 
        m_InfomationWind.UpdateRankContent();
    }
    public void UpdateRankFriendState(uint userID)
    {
        m_InfomationWind.UpdateRankFriendState(userID);
    }
    public void UpdateAchievementDate()
    {
        m_InfomationWind.UpdateAchievementDate();
    }
    public void UpdateMyAchievemetnData()
    {
        m_InfomationWind.UpdateMyAchievemetnData();
    }
    public void UpdateOnlyAchievementData(byte achieveID)
    {
        m_InfomationWind.UpdateOnlyAchievementData(achieveID);
    }
    public void UpdateFinishAchievementData()
    {
        m_InfomationWind.UpdateFinishAchievementDate();
    }
    public void UpdateInfomationTaskContent()
    {
        m_InfomationWind.UpdateTaskContent();
    }
    public void UpdateTaskProgress(byte taskID)
    {
        m_InfomationWind.UpdateTaskProgress(taskID);
    }
    public void UpdateInfomationMyRankContent()
    { 
        m_InfomationWind.UpdateMyRankContent(); 
    }
    public void UpdateMatchData()
    {
        m_MatchWind.UpdateMatchData(); 
    }
    public void UpdateMatchSingleData(byte matchID)
    {
        m_MatchWind.UpdateMatchSingleData(matchID);
    }
    public void UpdateMatchPeopleNum()
    {
        m_MatchWind.UpdateMatchPeopleNum();
    }
    public void UpdateMatchRankData()
    {
        m_MatchWind.UpdateMatchRankDate();
    }
    public void SetHeader(uint UserID, Texture2D tex)
    {
        m_MatchWind.SetHeader(UserID, tex);
    }
    public void UpdateActivitySignData()
    { 
        m_ActivityWind.UpdateSignInData(); 
    }
    public void UpdateActivityWelfareContent()
    {
        m_ActivityWind.UpdateActivityData();
    }
    //更新单个活动数据
    public void UpdateSingleActivityData(byte ActivityID)
    {
        m_ActivityWind.UpdateSingleActivityData(ActivityID);
    }
    //设置领取活动奖励UI特效
    public void SetActivityRewardEft(byte ActivityID, byte NodeID, ushort RewardID)
    {
        m_ActivityWind.SetActivityRewardEft(ActivityID, NodeID, RewardID);
    }
    public void UpdateMainWindTimeBox(byte byId)
    {
        m_MainWind.UpdateTimeBox(byId);
    }
    public void UpdateUserGold()
    {
        if (m_BtnWind != null)
            m_BtnWind.UpdateUserGold();
    }
    public void UpdateWarningState(byte type)
    {
        if (m_BtnWind != null)
            m_BtnWind.UpdateWarningState(type);
    }
    public void UpdateActivityTaskWarningState(byte type)
    {
        if (m_MainWind != null)
            m_MainWind.UpdateActivityTaskWarningState(type);
    }
    public void SetInformationHeader(uint id, byte flag, Texture2D tex)
    {
        m_InfomationWind.SetRankHeader(id, flag, tex);
    }
    bool UI_IsNotInScene(HallLogicUIStatue.Hall_State uiID)//需要显示的界面是否已经在界面中
    {
        switch (m_UIStatue.GetHallStatue().m_state)
        {
            case HallLogicUIStatue.Hall_State.Hall_Mian:
                {
                    if (m_MainWind.WindObj == null)
                        return true;
                } break;
            case HallLogicUIStatue.Hall_State.Hall_SelectRoom:
                {
                    if (m_RoomWind.WindObj == null)
                        return true;
                }; break;
            case HallLogicUIStatue.Hall_State.Hall_Information:
                {
                    if (m_InfomationWind.WindObj != null)
                    {
                        if (m_InfomationWind.WindObj.activeSelf == false)
                            return true;
                    }
                } break;
            case HallLogicUIStatue.Hall_State.Hall_Exchange:
                {} break;
            case HallLogicUIStatue.Hall_State.Hall_Btns:
                break;
            case HallLogicUIStatue.Hall_State.Hall_Contest:
                {
                    if (m_MatchWind.WindObj == null)
                        return true;
                }
                break;
            case HallLogicUIStatue.Hall_State.Hall_Activity:
                {
                    if (m_ActivityWind.WindObj == null)
                        return true;
                }break;
            default:
                break;
        }
        return false;
    }
    public void Shutdown()
    {
        GlobalHallUIMgr.Instance.GameLogicMgr.ShutDown();
        GlobalHallUIMgr.Instance.CloseHead();//关闭头像，此处的shutDown只关闭没有销毁，
        HallRunTimeInfo.Instance_UI.ShutDown(1);
        m_BtnWind.ShutDown();
        if (m_UIStatue.GetHallStatue().m_state == HallLogicUIStatue.Hall_State.Hall_Contest || 
            m_UIStatue.GetHallStatue().m_state == HallLogicUIStatue.Hall_State.Hall_Activity)
        {
            m_MainWind.ShutDown();
        }
        MyShutDown(m_UIStatue.GetHallStatue().m_state);
        for (byte i = 0; i < m_WindObjList.Count; ++i)
            ResManager.Instance.UnloadObject(m_WindObjList[i]);
        //m_HallBtnTouchBoxPool.ShutDown();
        m_InfomationWind.ShutDown();
        m_MatchGuide.ShutDown();
        m_CDKeyWnd.ShutDown();

    }
    public void ShowMainWindCenterInf(bool bShow = true /*屏蔽特效*/)
    {
        m_MainWind.ShowMainWindInf(bShow);
        //HallRunTimeInfo.Login_UI.SetHallWindBtnCanTouch(bShow, HallLogicUIStatue.Hall_State.Hall_Mian);
    }

    void MyShutDown(HallLogicUIStatue.Hall_State uiID)
    {
        switch( uiID)
        {
            case HallLogicUIStatue.Hall_State.Hall_Mian:
                m_MainWind.ShutDown();  break;
            case HallLogicUIStatue.Hall_State.Hall_SelectRoom:
                m_RoomWind.ShutDown(); break;
            case HallLogicUIStatue.Hall_State.Hall_Information:
                m_InfomationWind.ShutHide();break;
            case HallLogicUIStatue.Hall_State.Hall_Exchange:
                { } break;
            case HallLogicUIStatue.Hall_State.Hall_Btns:
                break;
            case HallLogicUIStatue.Hall_State.Hall_Contest:
                {
                    m_MatchWind.ShutDownMatch();
                    //WndManager.Instance.Pop();
                   // SetHallWindBtnCanTouch(false, HallLogicUIStatue.Hall_State.Hall_Mian);
                }
                break;
            case HallLogicUIStatue.Hall_State.Hall_Activity:
                {
                    m_ActivityWind.ShutDownActivity();
                }break;
            default:
                break;
        }
    }
    public void OnGUI(float delta)
    {
    }
    
}
