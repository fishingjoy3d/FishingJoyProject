using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
class MsgEventHandle  
{
    public static void HandleMsg(tagMsgEventBase pBase)
    {
        if (pBase == null)
            return;

        switch(pBase.GetMsgEventType)
        {
            case MsgEventType.MET_UserOperation:
                {
                    tagUserOperationEvent pEvent = (tagUserOperationEvent)pBase;

                    //除开提示的一些特殊处理
                    switch (pEvent.pCode)
                    {
                        case UserOperateMessage.UOM_Mail_SendMail_Sucess:
                            {
                                if (FriendRuntime.Instance != null)
                                {
                                    FriendRuntime.SendMsgUI.ShutDown();
                                }
                            }
                            break;
                    }
                    string ShowInfo;
                    byte ShowTime = StringTable.GetStringShowTime(pEvent.pCode.ToString(), out ShowInfo);
                    if (ShowTime == 0)
                        return;
                    if (pEvent.StringParam != null)
                        ShowInfo = string.Format(ShowInfo, pEvent.StringParam);
                    //GlobalHallUIMgr.Instance.ShowSystemTipsUI(ShowInfo, 0, false);
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(ShowInfo, (float)ShowTime, false);
                }
                break;
            case MsgEventType.MET_Item_Change:
                {
                    if (ShopRuntime.Instance != null)
                    {
                        ShopRuntime.ShopUI.UpdateKnapsackDate();
                        ShopRuntime.DaPaoShop.UpdateDaPaoDate();
                    }
                    if (SceneRuntime.Instance != null && SceneRuntime.SceneLogic != null)
                    {
                        SceneRuntime.SceneLogic.LogicUI.GetSkillBar.UpdateData();
                       // SceneRuntime.SceneLogic.NewGide.ShowLaunchCountDown(true);
                    }
                    if (HallRunTimeInfo.Instance_UI != null && HallRunTimeInfo.Instance_UI.m_loginUi != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.InfomationWind.UpdateChangeInfoBtnState();
                    }
                    GlobalHallUIMgr.Instance.UpdateOtherCardsInfoDate();
                }
                break;
            case MsgEventType.MET_Item_Use:
                { 
                
                }
                break;
            case MsgEventType.MET_Item_Accept:
                {
                    tagItemAcceptEvent pEvent = (tagItemAcceptEvent)pBase;
                    if(pEvent.Result)
                    {
                        ShopRuntime.ShopLogicMgr.TreasurebowlWnd.ShowRectipt(pEvent.DestUserID, pEvent.ItemID, pEvent.ItemSum);
                    }                    
                }
                break;
            case MsgEventType.MET_MailTitle_Change:
                {
                      if (FriendRuntime.Instance != null)
                          FriendRuntime.EmailUI.UpdateEmailListDate();
                }
                break;
            case MsgEventType.MET_MailTitle_Add:
                {
                    if (FriendRuntime.Instance != null)
                        FriendRuntime.EmailUI.UpdateEmailListDate();
                }
                break;
            case MsgEventType.MET_MailTitle_Del:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        tagMailTitleDelEvent pRole = (tagMailTitleDelEvent)pBase;
                        FriendRuntime.EmailUI.DeleteEmailListDate(pRole.pMailID);
                    }
                }
                break;
            case MsgEventType.Met_MailGet_Item:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Mail_GetItem_Sucess"), 1, false);
                    }
                }
                break;
            case MsgEventType.MET_MailContext_Change:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        tagMailContextChangeEvent pMail = (tagMailContextChangeEvent)pBase;
                        FriendRuntime.EmailUI.UpdatebSysMailItemState(pMail.pMailID);
                    }
                }
                break;
            case MsgEventType.MET_MailContext_Add:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        tagMailContextAddEvent pMail = (tagMailContextAddEvent)pBase;
                        FriendRuntime.EmailUI.UpdateReadMail(pMail.pMailID);
                    }
                }
                break;
            case MsgEventType.MET_MailContext_Del:
                {
                
                }
                break;
            case MsgEventType.MET_MailContext_AllChange:
                { 
                
                }
                break;
            case MsgEventType.MET_Friend_Load:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.FriendUI.UpdateListDate();
                    }
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateInfomationRankContent();
                    }
                    GlobalHallUIMgr.Instance.UpateCardsFriendBtnStatus();
                }
                break;
            case MsgEventType.MET_Friend_Change:
                {
                    tagFriendChangeEvent pEvent = (tagFriendChangeEvent)pBase;
                    if (HallRunTimeInfo.Instance_UI != null) 
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateInfomationRankContent();
                    if (FriendRuntime.Instance != null)
                        FriendRuntime.FriendUI.UpdateListDate();
                    GlobalHallUIMgr.Instance.UpdateCardsInfoDate(pEvent.UserID);
                  //  GlobalHallUIMgr.Instance.UpateCardsFriendBtnStatus();
                }
                break;
            case MsgEventType.MET_Friend_Add:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.FriendUI.UpdateListDate();

                    }
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        tagFriendAddEvent pAdd = (tagFriendAddEvent)pBase;
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateRankFriendState(pAdd.UserID);
                    }
                    GlobalHallUIMgr.Instance.UpateCardsFriendBtnStatus();
                }
                break;
            case MsgEventType.MET_Friend_Del:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        tagFriendDelEvent pRole = (tagFriendDelEvent)pBase;
                        FriendRuntime.FriendUI.DeleteFriend(pRole.UserID);
                    }
                }
                break;
            case MsgEventType.MET_Black_Load:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.BlacklistUI.UpdateBlacklistDate();
                    }
                }
                break;
            case MsgEventType.MET_Black_Change:
                {
                    tagBlackChangeEvent pEvent = (tagBlackChangeEvent)pBase;
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.BlacklistUI.UpdateBlacklistDate();
                    }
                    GlobalHallUIMgr.Instance.UpdateCardsInfoDate(pEvent.UserID);
                }
                break;
            case MsgEventType.MET_Black_Add:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.BlacklistUI.UpdateBlacklistDate();

                    }
                }
                break;
            case MsgEventType.MET_Black_Del:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.BlacklistUI.UpdateBlacklistDate();
                    }
                }
                break;
            case MsgEventType.MET_Role_Change:
                {
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
                        SceneRuntime.LogicUI.UpdateUnLockDataInfo();

                    }
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.PresentUI.UpdateAcceptGiffSum();
                    }
                    if (HallRunTimeInfo.Instance_UI != null )
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateUserGold();
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateFinishAchievementData();
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateMyAchievemetnData();
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpDateInfomationUserInfContent();
                    }
                    if (ShopRuntime.Instance != null && ShopRuntime.ShopLogicMgr != null)
                    {
                        ShopRuntime.ShopLogicMgr.UpdateUserGold();
                    }
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.SetNiuniuMyGold();
                        GlobalHallUIMgr.Instance.SetNiuniuWeagerBtn();
                    }
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr())
                    {
                        GlobalHallUIMgr.Instance.ForestDanceSetPlayerGold();
                    }
                    if (GlobalHallUIMgr.Instance.GetCarLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.SetCarPlayerGold();
                        GlobalHallUIMgr.Instance.SetCarBetBtn();
                    }
                    GlobalHallUIMgr.Instance.UpdateExperienceDate();
                    GlobalHallUIMgr.Instance.UpdateCardsInfoDate(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
                    GlobalHallUIMgr.Instance.UpdateAccountMgrDate();
                    GlobalHallUIMgr.Instance.UpdateVipPayProgressInfo();

                }
                break;
            case MsgEventType.MET_Role_Protect:
                {
                    PlayerRole.Instance.RoleInfo.RoleMe.BenefitTime = SystemTime.Instance.GetSystemTimeTotalSecond();
                    PlayerRole.Instance.RoleInfo.RoleMe.BeneFitCount += 1;

                    {
                        string str = StringTable.GetString("BenefitFetch");
                        str = string.Format(str, PlayerRole.Instance.RoleVip.AddAlmsSum() - PlayerRole.Instance.RoleInfo.RoleMe.BeneFitCount);
                        GlobalEffectMgr.Instance.ShowMessageBox(str, MssageType.BeneFit_Msg_6);

                    }                         
                }
                break;
            case MsgEventType.MET_Role_MonthCardChange:
                { 
                    //玩家月卡发生变化了
                    GlobalHallUIMgr.Instance.UpdateMonthCardData();
                    if(HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.BtnWind.HideMonthCardEft();
                    }
                }
                break;
            case MsgEventType.MET_Role_RateChange:
                {
                    tagRoleRateValueChangeEvent pRete = (tagRoleRateValueChangeEvent)pBase;
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.LogicUI.UpdateUnLockDataInfo();
                        SceneRuntime.SceneLogic.ChangeDestRate(pRete.RateIndex);
                        SceneRuntime.LogicUI.UpdateUnLockRateReward(BulletSetting.RateReward[pRete.RateIndex]);
                    }
                    else
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("join_room_unlock_sussess"), 2.5f);
                }
                break;
            case MsgEventType.MET_MonthCard_GetReward:
                {
                    GlobalHallUIMgr.Instance.UpdateMonthCardData();
                    if(HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.BtnWind.HideMonthCardEft();
                    }
                    ushort[] rewardID = new ushort[1];
                    if (FishConfig.Instance.m_MonthCardConfig.MonthCardMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardID()))
                        rewardID[0] = FishConfig.Instance.m_MonthCardConfig.MonthCardMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthCardID()].OnceDayRewardID;
                    else
                        return;

                    GlobalHallUIMgr.Instance.ShowRewardWnd(rewardID);

                }
                break;
            case MsgEventType.MET_Role_VipChange:
                {
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.LogicUI.UpdateLockedSkillSate();
                        SceneRuntime.LogicUI.UpdateSkillState();
                    }
                    if (ShopRuntime.Instance != null)
                    {
                        ShopRuntime.ShopUI.UpdateKnapsackLauncherDate();
                    }
                    GlobalHallUIMgr.Instance.UpdateVipPayProgressInfo();
                    GlobalHallUIMgr.Instance.UpdateExperienceDate();

                }
                break;
            case MsgEventType.MET_Role_LevelChange:
                { 
                    //玩家自己等级变化的时候
                    tagRoleLevelChangeEvent pLevel = (tagRoleLevelChangeEvent)pBase;
                    if (pLevel.NowLevel == 3)
                    {
                        //检查是否需要引导
                        if (SceneRuntime.Instance != null)
                        {
                             SceneRuntime.LogicUI.ShowSceneMatchGuide();
                        }
                    }
                    else
                    {
                        GlobalHallUIMgr.Instance.ShowToUpgrade((byte)pLevel.NowLevel, 3.5f, pLevel.RewardID);
                    }
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.SceneLogic.NewGide.SetLauncherGuide(pLevel.RewardID);
                    }
                    //玩家等级
                    SDKMgr.Instance.SDK.SetExtraData(
                       SDK.LEVEL_UP,
                       SDKMgr.Instance.LoginData.UID,
                       PlayerRole.Instance.RoleInfo.RoleMe.GetNickName(),
                       pLevel.NowLevel,
                       1,
                       "乐多捕鱼1区",
                       (int)PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel(),
                       1,
                       "无帮派"
                       );
                }
                break;
            case MsgEventType.MET_Query_Change:
                {
                        if (FriendRuntime.Instance != null)
                        {
                            FriendRuntime.SearchUI.UpateQueryListDate();
                        }
                }
                break;
            case MsgEventType.MET_Query_Role:
                {
                    tagQueryRoleEvent pQuery = (tagQueryRoleEvent)pBase;
                    GlobalHallUIMgr.Instance.UpdateOtherQueryUserCardsInfo(pQuery.m_Role);
                }
                break;
            case MsgEventType.MET_Check_Change:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateActivitySignData(); 
                    }
                }
                break;
            case MsgEventType.MET_Check_CheckNowDay:
                {
                    tagCheckCheckNowDayEvent pSignIn = (tagCheckCheckNowDayEvent)pBase;
                    bool bReward = false;
                    if (pSignIn.DayReawrdID != 0)
                        bReward = true;
                    if (pSignIn.MonthRewardID != 0)
                        bReward = true;
                    if (bReward)
                    {
                        GlobalHallUIMgr.Instance.ShowTipBox_0(TipBox_0.TipBox_0_ZiTi.Sign, 2, pSignIn.DayReawrdID, pSignIn.MonthRewardID);
                    }
                    
                }
                break;
            case MsgEventType.MET_Check_CheckOtherDay:
                {
                    tagCheckCheckOtherDayEvent pSignIn = (tagCheckCheckOtherDayEvent)pBase;
                    bool bReward = false;
                    if (pSignIn.DayReawrdID != 0)
                        bReward = true;
                    if (pSignIn.MonthRewardID != 0)
                        bReward = true;
                    if (bReward)
                    {
                        GlobalHallUIMgr.Instance.ShowTipBox_0(TipBox_0.TipBox_0_ZiTi.Sign, 2, pSignIn.DayReawrdID, pSignIn.MonthRewardID);
                    }
                }
                break;
            case MsgEventType.MET_Task_Change:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateInfomationTaskContent();
                    }
                    if (SceneRuntime.Instance != null)
                    {
                        GlobalHallUIMgr.Instance.UpdateTaskDate();
                    }
                }
                break;
            case MsgEventType.MET_Task_OnceChange:
                {
                    tagOnceTaskChangeEvent ptask = (tagOnceTaskChangeEvent)pBase;
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateTaskProgress(ptask.pTaskID);
                    }
                    GlobalHallUIMgr.Instance.UpdateTaskProgress(ptask.pTaskID);
                }
                break;
            case MsgEventType.MET_Task_EventFinish:
                {
                    //tagOnceTaskEventFinishEvent pTask = (tagOnceTaskEventFinishEvent)pBase;
                    //GlobalHallUIMgr.Instance.ShowFinishTips(pTask.TaskID, 2, 2);
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateInfomationTaskContent();
                    }
                }
                break;
            case MsgEventType.MET_Achievement_Change:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    { HallRunTimeInfo.Instance_UI.m_loginUi.UpdateAchievementDate(); }
                }
                break;
            case MsgEventType.MET_Achievement_EventFinish:
                {
                    tagAchievementEventFinishEvent pAcieve = (tagAchievementEventFinishEvent)pBase;
                    if (FishConfig.Instance.m_AchievementInfo.m_AchievementStr[pAcieve.pAchievement].AchievementStar == 2)
                    {
                        GlobalHallUIMgr.Instance.ShowFinishTips(pAcieve.pAchievement, 3.2f, 1);
                    }
                }
                break;
            case MsgEventType.MET_Achievement_OnceChange:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        tagOnceAchievementChangeEvent pAchieve = (tagOnceAchievementChangeEvent)pBase;
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateOnlyAchievementData(pAchieve.pAchievement);
                    }
                }
                break;
            case MsgEventType.MET_Action_Change:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateActivityWelfareContent();
                    }
                }
                break;
            case MsgEventType.MET_Action_OnceChange:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        tagOnceActionChangeEvent pActivity = (tagOnceActionChangeEvent)pBase;
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateSingleActivityData(pActivity.pAction);
                    }
                }
                break;
            case MsgEventType.MET_Table_Add:
                {
                    
                }
                break;
            case MsgEventType.MET_Table_Del:
                {
                    tagTableDelEvent pDel = (tagTableDelEvent)pBase;
                    GlobalHallUIMgr.Instance.UpdateCardsInfoDel(pDel.dwUserID);
                }
                break;
            case MsgEventType.MET_Table_Change:
                {
                    tagTableChangeEvent pTable = (tagTableChangeEvent)pBase;
                    GlobalHallUIMgr.Instance.UpdateCardsInfoDate(pTable.dwUserID);
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(pTable.dwUserID);
                        SceneRuntime.PlayerMgr.UpdateVipLevelData(pTable.dwUserID);
                    }
                }
                break;
            case MsgEventType.MET_Table_LevelChange:
                { 
                
                }
                break;
            case MsgEventType.MET_Month_Add:
                {
                    tagMonthAddEvent pAdd = (tagMonthAddEvent)pBase;
                    if (pAdd.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
                    {
                        GlobalHallUIMgr.Instance.ShowHead(HeadWind.HeadStatue.Head_Contest);
                        if (SceneRuntime.Instance != null)
                            SceneRuntime.LogicUI.HideLeftBtn();
                    }
                    GlobalHallUIMgr.Instance.UpdateMyMatchInfoDate();
                    //比赛的金币变
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.LogicUI.DestroyLottertDraw();
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(pAdd.dwUserID);
                        SceneRuntime.PlayerMgr.SetMatchGoldBg(pAdd.dwUserID);
                    }
                }
                break;
            case MsgEventType.MET_Month_Change:
                {
                    tagMonthChangeEvent pChange = (tagMonthChangeEvent)pBase;
                    if (pChange.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
                    {
                        GlobalHallUIMgr.Instance.ShowHead(HeadWind.HeadStatue.Head_Contest);
                    }
                    GlobalHallUIMgr.Instance.UpdateMyMatchInfoDate();
                    //比赛的金币变
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(pChange.dwUserID);
                    }
                }
                break;
            case MsgEventType.MET_Match_ScoreAdd:
                {
                    tagMatchScoreAddEvent pAdd = (tagMatchScoreAddEvent)pBase;
                    GlobalHallUIMgr.Instance.UpdateMyMatchScoreInfo();
                }
                break;
            case MsgEventType.MET_Match_ContinueGold:
                    {
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Match_ContinueGold_Success"), 1, false);
                    }
                break;
            case MsgEventType.MET_Match_ListChange:
                { 
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.LogicUI.UpdateCurMatchRank();
                    }
                }
                break;
            case MsgEventType.MET_Month_SignUpChange:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateMatchData();
                    }
                }
                break;
            case MsgEventType.MET_Month_SignOnceUpChange:
                { 
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        tagMonthSignUpOnceChangeEvent pSignUp = (tagMonthSignUpOnceChangeEvent)pBase;
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateMatchSingleData(pSignUp.MonthID);
                    }
                            
                }
                break;
            case MsgEventType.MET_Month_SignUpSumChange:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateMatchPeopleNum();
                    }
                    GlobalHallUIMgr.Instance.UpdateMyMatchInfoDate();

                }
                break;
            case MsgEventType.MET_Month_RankXmlChange:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateMatchRankData();
                    }
                }
                break;
            case MsgEventType.MET_Month_End:
                {
                    tagMonthEndEvent pMatch = (tagMonthEndEvent)pBase;
                    if (pMatch.MonthIndex == UInt32.MaxValue)
                        return;
                    if (pMatch.MonthIndex <= Byte.MaxValue && FishConfig.Instance.m_MatchInfo.m_MonthMap[pMatch.MonthID].m_MonthReward.ContainsKey((byte)pMatch.MonthIndex))
                    {
                        string strDec = string.Format(StringTable.GetString("Match_End_Warning"), pMatch.MonthScores, pMatch.VipScores, pMatch.MonthIndex);
                        GlobalHallUIMgr.Instance.ShowMatchMsgBox(strDec, pMatch.MonthID, MatchMsgBoxType.Match_Good);
                    }
                    else
                    {
                        string strDec = string.Format(StringTable.GetString("Match_End_Warning2"), pMatch.MonthScores, pMatch.VipScores);
                        GlobalHallUIMgr.Instance.ShowMatchMsgBox(strDec, pMatch.MonthID, MatchMsgBoxType.Match_End);
                    }
                }
                break;
            case MsgEventType.MET_Entity_Change:
                {
                    GlobalHallUIMgr.Instance.UpdateAccountMgrDate();
                    if (HallRunTimeInfo.Instance_UI != null && HallRunTimeInfo.Instance_UI.m_loginUi != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.InfomationWind.AccountmgrBtnPos();
                    }
                }
                break;
            case MsgEventType.MET_Entity_BindPhone:
                { 
                    //绑定手机的提示
                    tagEntityBindPhoneEvent pBind = (tagEntityBindPhoneEvent)pBase;
                    GlobalHallUIMgr.Instance.UpdateAccountMindState(pBind.Result, pBind.IsFirst, pBind.EventType);
                     if (HallRunTimeInfo.Instance_UI != null && HallRunTimeInfo.Instance_UI.m_loginUi != null)
                     {
                         HallRunTimeInfo.Instance_UI.m_loginUi.InfomationWind.AccountmgrBtnPos();
                     }
                }
                break;
            case MsgEventType.MET_Title_Change:
                {
                     if (HallRunTimeInfo.Instance_UI != null)
                     {
                         HallRunTimeInfo.Instance_UI.m_loginUi.UpdateUserTitle();
                     }
                }
                break;
            case MsgEventType.MET_Rank_Change:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateInfomationMyRankContent(); 
                    }
                }
                break;
            case MsgEventType.MET_Rank_GetReward:
                {
                    tagRankGetRewardEvent pRankReward = (tagRankGetRewardEvent)pBase;
                    byte idex = PlayerRole.Instance.RankManager.GetWeekRankInfo()[pRankReward.RankID].RankIndex;
                    ushort rewadID = FishConfig.Instance.m_RankInfo.m_RankMap[pRankReward.RankID].RewardMap[idex].RewardID;
                    GlobalHallUIMgr.Instance.ShowTipBox_0(TipBox_0.TipBox_0_ZiTi.Rank, 1, rewadID);
                    if (HallRunTimeInfo.Instance_UI != null)
                    { 
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateInfomationMyRankContent(); 
                    }
                }
                break;
            case MsgEventType.MET_Rank_XmlChange:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    { HallRunTimeInfo.Instance_UI.m_loginUi.UpdateInfomationTaskContent(); }
                }
                break;
            case MsgEventType.MET_Chest_Action:
                {
                      if(SceneRuntime.SceneLogic!=null)
                      {
                          tagChestActionEvent pChest = (tagChestActionEvent)pBase;
                          SceneRuntime.SceneLogic.ChestMgr.ShowChestWindow(pChest);                         
                      }
                  
                }
                break;
            case MsgEventType.MET_Chest_End:
                {
                    if (SceneRuntime.SceneLogic != null)
                      {
                          SceneRuntime.SceneLogic.ChestMgr.HideChestWindow();                          
                      }
                }
                break;
            case MsgEventType.MET_Chest_Reward:
                {
                    if (SceneRuntime.SceneLogic != null)
                    {
                        tagChestRewardEvent pChest = (tagChestRewardEvent)pBase;
                        SceneRuntime.SceneLogic.ChestMgr.OnOpenBox(false,pChest.ChestOnceStates.Index, pChest.ChestOnceStates.RewardID, pChest.ChestOnceStates.RewardOnlyID,true);
                    }
                }
                break;
            case MsgEventType.MET_Chest_Reset:
                {
                     if (SceneRuntime.SceneLogic != null)
                     {
                         if(SceneRuntime.SceneLogic.ChestMgr != null)
                            SceneRuntime.SceneLogic.ChestMgr.OnReset();
                     }

                }
                break;
            case MsgEventType.MET_Giff_Add:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        tagGiffAddEvent pGif = (tagGiffAddEvent)pBase;
                        FriendRuntime.PresentUI.UpdateAddPresentDate(pGif.GiffID);
                    }
                }
                break;
            case MsgEventType.MET_Giff_Del:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        tagGiffDelEvent pGif = (tagGiffDelEvent)pBase;
                        FriendRuntime.PresentUI.UpdateReceiverPressent(pGif.GiffID);
                    }
                }
                break;
            case MsgEventType.MET_Giff_Change:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.PresentUI.UpdatePresentListDate();
                    }
                }
                break;
            case MsgEventType.MET_Giff_SendInfoChange:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.FriendUI.UpdateListDate();
                    }
                }
                break;
            case MsgEventType.MET_Giff_SendInfoOnceChange:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        tagGiffSendInfoOnceChangeEvent pGiff = (tagGiffSendInfoOnceChangeEvent)pBase;
                        FriendRuntime.FriendUI.PressentScuess(pGiff.UserID);
                    }
                }
                break;
            case MsgEventType.MET_Shop_ShopResult:
                {
                    if (ShopRuntime.Instance != null)
                    {
                        tagShopResultEvent pShop = (tagShopResultEvent)pBase;
                        if (pShop.Result)
                        {
                            if (ShopRuntime.Instance != null)
                                ShopRuntime.ConfirmBuyUI.ShutDown();
                            GlobalHallUIMgr.Instance.ShowSystemTipsUI("恭喜您购买成功", 0.5f, false);
                        }
                        else
                            GlobalHallUIMgr.Instance.ShowSystemTipsUI("很可惜您的购买失败", 0.5f, false);
                        ShopRuntime.ShopUI.UpdateKnapsackDate();
                    }
                }
                break;
            case MsgEventType.MET_OnlineReward_Change:
                {
                    //if (HallRunTimeInfo.Instance_UI != null && HallRunTimeInfo.Login_UI != null)
                    //    HallRunTimeInfo.Login_UI.UpdateMainWindTimeBox(0);
                }
                break;
            case MsgEventType.MET_OnlineReward_Once:
                { 
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        tagOnlineRewardOnceChangeEvent pReward = (tagOnlineRewardOnceChangeEvent)pBase;
                        HallRunTimeInfo.Login_UI.UpdateMainWindTimeBox(pReward.RewardID);
                    }
                }
                break;
            case MsgEventType.MET_GameData_Change:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdatePlayerGameDate();
                    }
                }
                break;
            case MsgEventType.MET_Charm_RewardOnce:
                { 
                    if(SceneRuntime.Instance!=null)
                    {
                        tagCharmRewardOnce pCharm = (tagCharmRewardOnce)pBase;
                        if (pCharm == null)
                            return;
                        if (SceneRuntime.Instance != null && SceneRuntime.Instance.Scene != null)
                        {
                            byte bySource=0;
                            byte byDest = 0;
                            RoleMe pMySelf= PlayerRole.Instance.RoleInfo.RoleMe;
                            if (pMySelf == null)
                                return;
                            if (pCharm.SrcUserID==pMySelf.GetUserID())
                            {
                                bySource=pMySelf.GetSeat();
                            }
                            else
                            {
                                if (PlayerRole.Instance.TableManager.GetTableRole(pCharm.SrcUserID) == null)
                                    return;
                                bySource=PlayerRole.Instance.TableManager.GetTableRole(pCharm.SrcUserID).GetSeat();// 
                            }
                            bySource = SceneRuntime.ServerToClientSeat(bySource);

                            if (pCharm.DestUserID == pMySelf.GetUserID())
                            {
                                byDest = pMySelf.GetSeat();
                            }
                            else
                            {
                                if (PlayerRole.Instance.TableManager.GetTableRole(pCharm.DestUserID) == null)
                                    return;
                                byDest = PlayerRole.Instance.TableManager.GetTableRole(pCharm.DestUserID).GetSeat();
                            }
                            byDest = SceneRuntime.ServerToClientSeat(byDest);

                            SceneRuntime.Instance.Scene.LogicUI.ShowGiftEff(bySource, byDest, pCharm.CharmIndex);
                        }
                        return;
                    }
                    //魅力赠送的动画播放 

                }
                break;
            case MsgEventType.MET_Charm_UserChange:
                { 
                    //一个玩家的魅力变化了 更新到合适的UI上面去 绝对不是自己
                    tagUserChangeEvent pEvent = (tagUserChangeEvent)pBase;
                    GlobalHallUIMgr.Instance.UpdateCardsInfoCharmDate(pEvent.DestUserID, pEvent.CharmValue);
                }
                break;
            case MsgEventType.MET_Game_RateChange:
                { 
                    //当玩家倍率变化的时候 不 一定是自己
                    tagGameRateChangeEvent pRate = (tagGameRateChangeEvent)pBase;
                    if (SceneRuntime.Instance != null)
                    {
                        if (pRate.UserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
                        {
                            SceneRuntime.LogicUI.GetSkillBar.UpdateData();

                        }
                    }

                }
                break;
            case MsgEventType.MET_Launcher_DataChange:
                { 
                    //当玩家炮台数据发生变化的时候 我们处理下
                    if (ShopRuntime.Instance != null)
                    {
                        ShopRuntime.DaPaoShop.UpdateDaPaoDate();
                        ShopRuntime.ShopUI.UpdateKnapsackLauncherDate();
                    }

                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.PlayerMgr.UpdateMyLauncherChange();
                       // SceneRuntime.SceneLogic.NewGide.ShowLaunchCountDown();
                    }

                }
                break;
            case MsgEventType.MET_Recharge_Result:
                {
                    tagRechargeResultEvent pPayResult = (tagRechargeResultEvent)pBase;
                    if (pPayResult.Result)
                    {
                        GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("EC_Recharge_Sucess"), MssageType.PayScuess_Mag_0);
                        //GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_Recharge_Sucess"), 0.5f, false);
                        GlobalHallUIMgr.Instance.ShutDownPayBuyConfirm();
                        GlobalHallUIMgr.Instance.ShutDownPayWnd();
                    }
                    else
                    {
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_Recharge_Failed"), 0.5f, false);

                    }
                }
                break;
            case MsgEventType.MET_Recharge_GetOrderID:
                { 
                    tagRechargeGetOrderIDEvent pEvent = (tagRechargeGetOrderIDEvent)pBase;
                    if (pEvent.Result == true)
                    {
                        if (FishConfig.Instance.m_FishRecharge.m_FishRechargeMap.ContainsKey(pEvent.ShopID) == true)
                        {
                             UInt64 orderID = pEvent.OrderID;
                             orderID = (orderID << 32) | pEvent.ShopID;
                            tagFishRechargeInfo pInfo = FishConfig.Instance.m_FishRecharge.m_FishRechargeMap[pEvent.ShopID];

                            int price = (int)pInfo.dDisCountPrice;
                            byte type = 1;
                            if ((byte)pInfo.RechargeType == 1 || (byte)pInfo.RechargeType == 3)
                                type = 2;
                            else if ((byte) pInfo.RechargeType == 2 || (byte)pInfo.RechargeType == 4)
                                type = 1;
                            else
                                type = 3;
                            
                            SDKMgr.Instance.SDK.Pay(price, type.ToString(), 1, pEvent.OrderID.ToString(), orderID.ToString(), (int)pEvent.ShopID);
                        }
                    }
                }
                break;
            case MsgEventType.MET_Recharge_AddOrderInfo:
                {
                    tagAddRechargeOrderInfoEvent pEvent = (tagAddRechargeOrderInfoEvent)pBase;
                    //接收到订单了 进行处理
                    if (pEvent.Result == true)
                    {
                        if (FishConfig.Instance.m_FishRecharge.m_FishRechargeMap.ContainsKey(pEvent.ShopID) == true)
                        {
                            
                            SDKMgr.Instance.SDK.Pay(1, "1", 1, pEvent.Transid, pEvent.Sign, (int)pEvent.ShopID);
                        }
                    }


                }
                break;
            case MsgEventType.MET_Announcement_Change:
                {
                    if (ShopRuntime.Instance != null)
                    {
                        ShopRuntime.MaterailShop.UpdateNoticeDate();
                    }
                }
                break;
            case MsgEventType.MET_Package_Result:
                {
                    tagPackageResultEvent pResult = (tagPackageResultEvent)pBase;
                    if (!pResult.Result)
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_Package_NonItem"), 1, false);
                    else
                        GlobalHallUIMgr.Instance.ShowRewardWnd(pResult.RewardID);
                }
                break;
            case MsgEventType.MET_Table_Join:
                { 
                    //进入房间失败后  我们进行处理 获取合适的房间 或者让玩家进行充值
                    //根据错误的码  我们进行处理
                    tagJoinTableEvent pJoin = (tagJoinTableEvent)pBase;
                    uint gold = PlayerRole.Instance.TableManager.GetTableErrorParam(pJoin.TableID, pJoin.Error);
                    if (pJoin.Error == TableError.TE_MaxGlobel || pJoin.Error == TableError.TE_MaxCurrcey)
                    {
                        string str  = string.Format(StringTable.GetString("join_room_failed2"), gold);
                        GlobalEffectMgr.Instance.ShowMessageBox(str, MssageType.GameRoom_Msg_1, pJoin.TableID);
                    }
                    else if (pJoin.Error == TableError.TE_MinCurrcey || pJoin.Error == TableError.TE_MinGlobel)
                    {
                        string str = string.Format(StringTable.GetString("join_room_failed1"), gold);
                        GlobalEffectMgr.Instance.ShowMessageBox(str, MssageType.GameRoom_Msg_2, pJoin.TableID);
                    }
                    else if (pJoin.Error == TableError.TE_MinLevel || pJoin.Error == TableError.TE_MaxLevel)
                    {
                        string str = string.Format(StringTable.GetString("join_room_failed4"), gold);
                        GlobalEffectMgr.Instance.ShowMessageBox(str, MssageType.GameRoom_Msg_2, pJoin.TableID);
                    }

                    if (pJoin.Error == TableError.TE_RateError)
                    {
                        GlobalEffectMgr.Instance.ShowMessageBox(gold.ToString(), MssageType.GameRoom_Msg_3, pJoin.TableID);
                    }
                }
                break;
            case MsgEventType.MET_Task_FinihStates:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateActivityTaskWarningState(0);
                }
                break;
            case MsgEventType.MET_Achievement_FinihStates:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateWarningState(4);
                }
                break;
            case MsgEventType.MET_Action_FinihStates:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateActivityTaskWarningState(1);
                }
                break;
            case MsgEventType.MET_Action_OnceReward:
                {
                    tagActionOnceRewardEvent pEvent = (tagActionOnceRewardEvent)pBase;
                   // GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Activity_Reward_Success"), 1.0f, false);

                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.SetActivityRewardEft(pEvent.ActionID, (byte)pEvent.EventID, pEvent.RewardID);
                    }
                    //LogMgr.Log(string.Format("获取指定活动 ID: {0}  的 奖励ID {1}\n", pEvent.ActionID, pEvent.RewardID));
                }
                break;
            case MsgEventType.MET_MailTitle_NonRead:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateWarningState(1);
                    if (FriendRuntime.Instance != null)
                        FriendRuntime.Instance.FriendSys.UpdateWarningState(WndType.Emanil_Wnd);
                    if (SceneRuntime.Instance != null)
                        SceneRuntime.SceneLogic.BtnsMgr.UpdateWarningState();
                }
                break;
            case MsgEventType.MET_Rank_ExistsReward:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateWarningState(2);
                    if (SceneRuntime.Instance != null)
                        SceneRuntime.SceneLogic.BtnsMgr.UpdateWarningState();
                }
                break;
            case MsgEventType.MET_Giff_ExistsChange:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateWarningState(1);
                    if (FriendRuntime.Instance != null)
                        FriendRuntime.Instance.FriendSys.UpdateWarningState(WndType.Present_Wnd);
                    if (SceneRuntime.Instance != null)
                        SceneRuntime.SceneLogic.BtnsMgr.UpdateWarningState();
                }
                break;
            case MsgEventType.MET_RoleStates_Change:
                {
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateActivityTaskWarningState(0);
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateWarningState(4);
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateActivityTaskWarningState(1);
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateWarningState(2);
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateWarningState(1);
                    }

                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.Instance.FriendSys.UpdateWarningState(WndType.Emanil_Wnd);
                        FriendRuntime.Instance.FriendSys.UpdateWarningState(WndType.Present_Wnd);
                    }
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.SceneLogic.BtnsMgr.UpdateWarningState();
                        GlobalHallUIMgr.Instance.UpdateWarningSate();
                    }
                
                }
                break;
            case MsgEventType.MET_Logon_ReturnLogon:
                {
                    PlayerRole.Instance.OnUserLogout();//将部分数据清空掉
                    //关闭一切需要关闭的UI
                    //
                    GlobalHallUIMgr.Instance.ShutDown();

                }
                break;
            case MsgEventType.MET_Queue_Logon:
                {
                    tagQueueWriteEvent pEvent = (tagQueueWriteEvent)pBase;

                    //if(pEvent.IsLogon)
                    //    LogMgr.Log(string.Format("玩家连接成功 排队人数为 {0}  排队等待时间为 {1} 秒", pEvent.WriteIndex, pEvent.WriteSec));
                    //else
                    //    LogMgr.Log(string.Format("玩家重新连接成功 排队人数为 {0}  排队等待时间为 {1} 秒", pEvent.WriteIndex, pEvent.WriteSec));
                }
                break;
            case MsgEventType.MET_Use_ExChange:
                {
                    tagUseExChangeEvent pCDKey = (tagUseExChangeEvent)pBase;
                    ushort[] RewardID = new ushort[1];
                    RewardID[0] = pCDKey.RewardID;
                    GlobalHallUIMgr.Instance.ShowRewardWnd(RewardID);
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Instance_UI.m_loginUi.CDKeyWnd.DestoryWnd();
                    }
                }
                break;
            case MsgEventType.MET_Notice_Once:
                {
                    if (LogicManager.Instance.ActiveLogicType == LogicType.LOGIC_LOGON)
                        return;
                    //公告的命令
                    tagNoticeOnceEvent pNotic = (tagNoticeOnceEvent)pBase;
                    if(pNotic.MessageType == MessageType.MT_CenterMessage || pNotic.MessageType == MessageType.MT_GameMessage || pNotic.MessageType == MessageType.MT_TableMessage || pNotic.MessageType == MessageType.MT_UserMessage)
                    {
                        //发送命令 弹框
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI(pNotic.Message, 3, false);
                    }
                    else
                    {
                        NoticeInfo info = new NoticeInfo();
                        info.StrNotic = pNotic.Message;
                        info.TextColor = pNotic.MessageColor;
                        info.RollCount = pNotic.StepNum;
                        info.RollInterval = pNotic.StepSec;
                        GlobalHallUIMgr.Instance.ShowSystemNotice(info);
                    }
                }
                break;
            case MsgEventType.MET_Logon_Freeze:
                {
                    tagLogonAccountFreezeEvent pMsg = (tagLogonAccountFreezeEvent)pBase;
                    //LogMgr.Log(string.Format("账号被冻结 冻结结束时间为{0}", pMsg.EndTime.ToString()));
                }
                break;
            case MsgEventType.MET_Lottery_Result:
                {
                    tagLotteryResultEvent pMsg = (tagLotteryResultEvent)pBase;
                    //tagRewardOnce pReward = FishConfig.Instance.m_RewardConfig.RewardMap[pMsg.RewardID];
                    if (SceneRuntime.Instance != null)
                    {
                            SceneRuntime.LogicUI.ShowCardChoose(pMsg.RewardID);
                            SceneRuntime.LogicUI.UpdateLotteryInfo();
                    }

                }
                break;
            case MsgEventType.MET_Lottery_ScoreChange:
                {
                    if (SceneRuntime.Instance != null)
                    {
                        SceneRuntime.LogicUI.UpdateLotteryInfo();
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_Join:
                { 
                    //玩家进入牛牛房间的时候
                    tagNiuNiuJoin pEvent = (tagNiuNiuJoin)pBase;
                    GlobalHallUIMgr.Instance.ShowNiuniuWnd();
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.GetNiuniuTableState(
                            pEvent.TableGameLog, pEvent.TableVipSeatInfo,
                            pEvent.TableBankerInfo, pEvent.TableWriteBankerList,
                            pEvent.TableStates, pEvent.TableUpdateSec,
                            pEvent.TableBrandresult.BrandArray, pEvent.TableBrandresult.BrandResult, pEvent.TableGlobelInfo.GlobelNum);
                        GlobalHallUIMgr.Instance.SetNiuniuWeagerBtn();
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_UpdateWriteBankerList:
                { 
                    //获取和更新上庄链表
                    tagNiuNiuUpdateWriteBankerList pEvent = (tagNiuNiuUpdateWriteBankerList)pBase;
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.GetNiuniuHostline(pEvent.TableWriteBankerList);
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_BankerUserChange:
                {
                    tagNiuNiuBankerUserChange pEvent = (tagNiuNiuBankerUserChange)pBase;
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.UpdateNiuniuHost(pEvent.TableBankerInfo);
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_AddGlobel:
                {
                    tagNiuNiuAddGlobel pEvent = (tagNiuNiuAddGlobel)pBase;
                    //玩家往牛牛上进行下注的结果
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr() && pEvent.Result)
                    {
                        GlobalHallUIMgr.Instance.UpdateNiuniuMyWeagerNum();
                        GlobalHallUIMgr.Instance.SetNiuniuWeagerBtn();
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_Update:
                {
                    //牛牛的桌子上数据发生变化的时候
                    tagNiuNiuTableInfoChange pEvent = (tagNiuNiuTableInfoChange)pBase;
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.SetNiuniuWeagerBtn();
                        GlobalHallUIMgr.Instance.UpdateNiuniuDate(pEvent.TableGlobelInfo.GlobelNum);
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_StatesChange:
                {
                    tagNiuNiuStatesChange pEvent = (tagNiuNiuStatesChange)pBase;
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.GetNiuniuResult(
                            pEvent.TableBankerUserInfo,
                            pEvent.OldVipSeatInfo, pEvent.TableVipSeatInfo,
                            pEvent.TableGameLog, (int)pEvent.TableStates, 
                            pEvent.TableBrandresult);
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_WriteBankerGlobelMsg:
                { 
                    
                }
                break;
            case MsgEventType.MET_NiuNiu_VipSeatChange:
                {
                    tagNiuNiuVipSeatChangeEvent pEvent = (tagNiuNiuVipSeatChangeEvent)pBase;
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.NiuniuVipSeatChange(pEvent.Result, pEvent.TableVipList);
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_BeLeaveVip:
                {
                    tagNiuNiuBeLeaveVipSeatEvent pEvent = (tagNiuNiuBeLeaveVipSeatEvent)pBase;
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.NiuniuBeLeaveVip(pEvent.DestRoleInfo);
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_NormalSeatInfo:
                {
                    tagNiuNiuGetNormalSeatInfoEvent pEvent = (tagNiuNiuGetNormalSeatInfoEvent)pBase;
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.NiuniuGetNormalPeople(pEvent.Page, pEvent.TotalRoleSum, pEvent.Array);
                    }
                }
                break;
            case MsgEventType.MET_Table_LotteryUIStates:
                {
                    tagTableLotteryUIChangeEvent pEvent = (tagTableLotteryUIChangeEvent)pBase;

                    if (SceneRuntime.Instance != null && pEvent.UserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
                    {

                        SceneRuntime.PlayerMgr.ShowLotteryState(pEvent.UserID, pEvent.IsOpen);
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_VipGetGlobel:
                {
                    tagNiuNiuVipGetGlobelEvent pEvent = (tagNiuNiuVipGetGlobelEvent)pBase;

                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.NiuniuVipWeager(pEvent.VipSeat, pEvent.AddGlobel, pEvent.Index, pEvent.AllGlobel);
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_TableStopUpdate:
                { 
                    //桌子停止更新金币
                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.NiuniuStopWeager();
                    }
                }
                break;
            case MsgEventType.MET_NiuNiu_VipGlobelChange:
                { 
                    //VIP玩家金币变化 非下注的时候
                    tagNiuNiuVipGlobelChange pEvent = (tagNiuNiuVipGlobelChange)pBase;

                    if (GlobalHallUIMgr.Instance.GetGameLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.NiuniuVipGlobalChange(pEvent.VipUserID, pEvent.VipSeat, pEvent.VipGlobelSum);
                    }
                }
                break;
            case MsgEventType.MET_Char_ListChange:
                { 
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.FriendUI.UpdateListDate();
                        FriendRuntime.FriendUI.UpdateAllNewMsg();
                    }

                    if (HallRunTimeInfo.Instance_UI != null)
                    {         
                        HallRunTimeInfo.Instance_UI.m_loginUi.UpdateWarningState(1);
                    }
                }
                break;
            case MsgEventType.MET_Char_ContectChange:
                {
                    if (FriendRuntime.Instance != null)
                    {
                        FriendRuntime.SendMsgUI.UpdateChateInfo();
                    }
                }
                break;
            case MsgEventType.MET_Char_AddChar:
                {
                    tagCharAddCharEvent pMsg = (tagCharAddCharEvent)pBase;
                    if (FriendRuntime.Instance != null)
                    {
                        if (FriendRuntime.SendMsgUI.AddNewMsg(pMsg.pInfo) == false)
                            FriendRuntime.FriendUI.UpdateNewMsgState(pMsg.pInfo.SrcUserID);
                    }
                }
                break;
            case MsgEventType.MET_RelationRequest_Change:
                {
                   if (FriendRuntime.Instance != null)
                   {
                       FriendRuntime.FriendUI.UpdateApplyListData();
                   }
                }
                break;
            case MsgEventType.MET_RelationRequest_SendResult:
                {
                    LogMgr.Log("关系请求结果");
                }
                break;
            case MsgEventType.MET_RelationRequest_HandleResult:
                {
                    
                }
                break;
            case MsgEventType.MET_Dial_Join:
                {
                    tagDialJoin pEvent = (tagDialJoin)pBase;
                    GlobalHallUIMgr.Instance.ForestDanceShowWnd();
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr())
                    {
                        GlobalHallUIMgr.Instance.ForestDanceGetTableState(
                            pEvent.TableUpdateSec, pEvent.TableStates, pEvent.TableGlobelInfo, pEvent.AreaData,
                            pEvent.TableBankerInfo, pEvent.TableWriteBankerList, pEvent.TableVipSeatInfo,
                            pEvent.TableGameLog, pEvent.TableResult);
                    }
                }
                break;
            case MsgEventType.MET_Dial_AddGlobel:
                {
                    tagDialAddGlobel pEvent = (tagDialAddGlobel)pBase;
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr() && pEvent.Result)
                    {
                        GlobalHallUIMgr.Instance.ForestDanceUpdateMyWeager(pEvent.TableGlobelSum);
                        GlobalHallUIMgr.Instance.ForestDanceSetWeagerBtn();
                    }
                }
                break;
            case MsgEventType.MET_Dial_Update:
                {
                    tagDialTableInfoChange pEvent = (tagDialTableInfoChange)pBase;
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr())
                    {
                        GlobalHallUIMgr.Instance.ForestDanceUpdateTotalWeager(
                           pEvent.TableGlobelInfo, pEvent.TableStates);
                    }
                }
                break;
            case MsgEventType.MET_Dial_StatesChange:
                {
                    tagDialStatesChange pEvent = (tagDialStatesChange)pBase;
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr())
                    {
                        GlobalHallUIMgr.Instance.ForestDanceStateChange(
                            pEvent.TableStates, pEvent.TableGlobelInfo, pEvent.AreaData, pEvent.OldVipSeatInfo,
                            pEvent.TableVipSeatInfo, pEvent.TableWriteBankerList, pEvent.TableBankerUserInfo, 
                            pEvent.TableGameLog, pEvent.TableResult);
                    }
                }
                break;
            case MsgEventType.MET_Dial_UpdateWriteBankerList:
                {
                    tagDialUpdateWriteBankerList pEvent = (tagDialUpdateWriteBankerList)pBase;
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr())
                    {
                        GlobalHallUIMgr.Instance.ForestDanceGetHostLine(
                            pEvent.TableWriteBankerList);
                    }
                }
                break;
            case MsgEventType.MET_Dial_BankerUserChange:
                {
                    tagDialBankerUserChange pEvent = (tagDialBankerUserChange)pBase;
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr())
                    {
                        GlobalHallUIMgr.Instance.ForestDanceUpdateHost(pEvent.TableBankerInfo);
                    }
                }
                break;
            case MsgEventType.MET_Dial_WriteBankerGlobelMsg:
                {

                }
                break;
            case MsgEventType.MET_Dial_VipSeatChange:
                {
                    tagDialVipSeatChangeEvent pEvent = (tagDialVipSeatChangeEvent)pBase;
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr())
                    {
                        GlobalHallUIMgr.Instance.ForestDanceVipChange(
                            pEvent.Result, pEvent.TableVipList);
                    }
                }
                break;
            case MsgEventType.MET_Dial_BeLeaveVip:
                {
                    tagDialBeLeaveVipSeatEvent pEvent = (tagDialBeLeaveVipSeatEvent)pBase;
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr())
                    {
                        GlobalHallUIMgr.Instance.ForestDanceLeaveVip(
                            pEvent.DestRoleInfo);
                    }
                }
                break;
            case MsgEventType.MET_Dial_NormalSeatInfo:
                {
                    tagDialGetNormalSeatInfoEvent pEvent = (tagDialGetNormalSeatInfoEvent)pBase;
                    if (GlobalHallUIMgr.Instance.GetForestDanceMgr())
                    {
                        GlobalHallUIMgr.Instance.ForestDanceGetNormal(
                            pEvent.Page, pEvent.TotalRoleSum, pEvent.Array);
                    }
                }
                break;
            case MsgEventType.MET_Dial_VipGetGlobel:
                {

                }
                break;
            case MsgEventType.MET_Dial_TableStopUpdate:
                {

                }
                break;
            case MsgEventType.MET_Dial_VipGlobelChange:
                {

                }
                break;
            case MsgEventType.MET_Car_Join:
                {
                    tagCarJoin pEvent = (tagCarJoin)pBase;
                    GlobalHallUIMgr.Instance.ShowCarWnd();
                    if(GlobalHallUIMgr.Instance.GetCarLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.EnterGameCar(pEvent.TableUpdateSec,
       pEvent.TableStates,
       pEvent.TableGlobelInfo,
       pEvent.TableBankerInfo,
       pEvent.TableWriteBankerList,
    pEvent.TableVipSeatInfo,
    pEvent.TableGameLog,
    pEvent.TableResult);
                        GlobalHallUIMgr.Instance.SetCarBetBtn();
                    }
                }
                break;
            case MsgEventType.MET_Car_AddGlobel:
                {
                    tagCarAddGlobel pEvent = (tagCarAddGlobel)pBase;
                     if(GlobalHallUIMgr.Instance.GetCarLogicMgr()&&pEvent.Result)
                     {
                         GlobalHallUIMgr.Instance.UpdateCarMyWeagerNum();
                         GlobalHallUIMgr.Instance.SetCarBetBtn();
                     }
                }
                break;
            case MsgEventType.MET_Car_Update:
                {
                    tagCarTableInfoChange pEvent = (tagCarTableInfoChange)pBase;
                    if(GlobalHallUIMgr.Instance.GetCarLogicMgr())
                    {                      
                        GlobalHallUIMgr.Instance.UpdateCarDate(pEvent.TableGlobelInfo.GlobelNum);
                        GlobalHallUIMgr.Instance.SetCarBetBtn();
                    }
                }
                break;
            case MsgEventType.MET_Car_StatesChange:
                {
                    tagCarStatesChange pEvent = (tagCarStatesChange)pBase;
                     if(GlobalHallUIMgr.Instance.GetCarLogicMgr())
                     {
                         GlobalHallUIMgr.Instance.CarStatusChange( pEvent.TableStates,
     pEvent.TableGlobelInfo,
     pEvent.OldVipSeatInfo,
     pEvent.TableVipSeatInfo,
     pEvent.TableWriteBankerList,
     pEvent.TableBankerUserInfo,
     pEvent.TableGameLog,
     pEvent.TableResult);
                     }

                }
                break;
            case MsgEventType.MET_Car_UpdateWriteBankerList:
                {
                    tagCarUpdateWriteBankerList pEvent = (tagCarUpdateWriteBankerList)pBase;
                     if(GlobalHallUIMgr.Instance.GetCarLogicMgr())
                     {
                         GlobalHallUIMgr.Instance.GetCarHostline(pEvent.TableWriteBankerList);
                     }
                }
                break;
            case MsgEventType.MET_Car_BankerUserChange:
                {
                    tagCarBankerUserChange pEvent=(tagCarBankerUserChange)pBase;
                    if(GlobalHallUIMgr.Instance.GetCarLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.UpdateCarHost(pEvent.TableBankerInfo);
                    }
                }
                break;
            case MsgEventType.MET_Car_WriteBankerGlobelMsg:
                {

                }
                break;
            case MsgEventType.MET_Car_VipSeatChange:
                {

                }
                break;
            case MsgEventType.MET_Car_BeLeaveVip:
                {

                }
                break;
            case MsgEventType.MET_Car_NormalSeatInfo:
                {
                    tagCarGetNormalSeatInfoEvent pEvent = (tagCarGetNormalSeatInfoEvent)pBase;
                    if(GlobalHallUIMgr.Instance.GetCarLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.CarGetNormalPeople(pEvent.Page, pEvent.TotalRoleSum, pEvent.Array);
                    }
                }
                break;
            case MsgEventType.MET_Car_VipGetGlobel:
                {

                }
                break;
            case MsgEventType.MET_Car_TableStopUpdate:
                {
                    if(GlobalHallUIMgr.Instance.GetCarLogicMgr())
                    {
                        GlobalHallUIMgr.Instance.CarStopWeager();
                    }
                }
                break;
            case MsgEventType.MET_Car_VipGlobelChange:
                {

                }
                break;
            case MsgEventType.MET_BindPhone_Reset:
                {
                    //触发玩家需要重新绑定手机的事件
                    LogMgr.Log("接收到重新绑定手机的事件");

                    //修改密码
                   // PlayerRole.Instance.RoleInfo.ChangeSecPassword("123", "234");
                }
                break;
            case MsgEventType.MET_Role_ChangeSecPwd:
                {
                    tagRoleChangeSecPwdEvent pEvent = (tagRoleChangeSecPwdEvent)pBase;
                    if (pEvent.Result)
                    {
                        GlobalHallUIMgr.Instance.PhonePsChanged();
                    }                             
                    //修改二级密码返回值
                }
                break;
            case MsgEventType.MET_Role_OpenShareUI:
                { 
                    //打开客户端分享UI
                }
                break;
        }
    }
}