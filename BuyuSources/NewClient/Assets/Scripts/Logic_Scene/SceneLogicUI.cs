using UnityEngine;
using System.Collections.Generic;

public class SceneLogicUI: ILogicUI
{
    SceneLogic          m_Logic;
    SkillBar            m_SkillBar = new SkillBar();
    SceneWonderful      m_Wonderful = new SceneWonderful();
    SendGiftEff         m_Gifteff=new SendGiftEff();
    DropItemManager     m_DropItem = new DropItemManager();
    SceneMatchGuide     m_MatchGuide = new SceneMatchGuide();
    SceneLockedUI       m_LockedFishUI = new SceneLockedUI();
    SceneChangeLauncher m_ChangeLauncherUI = new SceneChangeLauncher();
    SceneLotteryDraw    m_LotteryDraw = new SceneLotteryDraw();
    SceneMatchRank      m_SceneMatchRank = new SceneMatchRank();
    public bool Init(ILogic logic, object obj)
    {
        m_Logic = (SceneLogic)logic;
        m_Wonderful.Init();
        m_Gifteff.Init();
        m_DropItem.Init();
        m_MatchGuide.InitObj();
        m_ChangeLauncherUI.InitObj();
        m_LotteryDraw.Init();
        m_SceneMatchRank.InitObj();
        GlobalHallUIMgr.Instance.GameShare.EnterGame();
        return true;
    }
    public void InitByScene()
    {
        m_SkillBar.Init();
    }
    public void UpdateUnLockDataInfo()
    {
        m_SkillBar.UpdateUnLockDataInfo();
    }
    public void UpdateLotteryInfo()
    {
        m_SkillBar.UpdateLotteryInfo();
    }
    public void UpdateUnLockRateReward(uint RewardID)
    {
        m_SkillBar.UpdateUnLockRateReward(RewardID);
    }
    public void UpateLaunchData()
    {
        m_ChangeLauncherUI.UpdateLaunchData();
    }
    public void UpdateLockedSkillSate()
    {
        m_SkillBar.UpdateLockedSkillSate();
    }
    public void UpdateSkillState()
    {
        m_SkillBar.UpdateSkillState();
    }
    public void OnGUI(float delta)
    {

    }
    public void ShowClearSceneMsg()
    {
        //正在清场状态，不能发子弹和技能。
        if (ShopRuntime.Instance != null)
            return;
        GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("clearmsg"), ConstValue.CLEAR_TIME, false);
    }
    public void ShowSceneMatchGuide()
    {
        m_MatchGuide.ShowSceneMatchGuide();
    }
    public void ShowChangeLauncherWnd()
    {
        m_ChangeLauncherUI.ShowChangeLauncherWnd();
    }
    public void HideLeftBtn()
    {
        m_SkillBar.HideLeftBtn();
    }
    public void Update(float delta)
    {
        if (m_Logic.InitCompletion == false)
            return;
        m_SkillBar.Update(delta);
        m_Wonderful.UpDataUIScroe(delta);
        m_Gifteff.Update(delta);
        m_DropItem.Update(delta);
        m_LotteryDraw.Update(delta);
    }
    public void Shutdown()
    {
        if (m_SkillBar != null)
        {
            m_SkillBar.ShutDown();
            m_SkillBar = null;
        }
        m_Wonderful.ShutDown();
        m_Gifteff.ShutDown();
        m_DropItem.ShutDown();
        m_MatchGuide.ShutDown();
        m_LockedFishUI.ShutDown();
        m_ChangeLauncherUI.ShutDown();
        m_LotteryDraw.ShutDown();
        m_SceneMatchRank.ShutDown();
        GlobalHallUIMgr.Instance.GameShare.LveaveGame();
    }
    public void PlayCD(float time, SkillType type)
    {
        if (type < SkillType.SKILL_LOCK)
            m_SkillBar.PlayCD(time, 0);
        else
        {
            m_SkillBar.PlayCD(time, 1);
        }
    }
    public void RevertLockedFish()
    {
        m_SkillBar.RevertLockedFish();
    }
    public void ShowWonderfulUI(CatchedData cd)
    {
        if (SceneRuntime.PlayerMgr.GetPlayer(cd.ClientSeat) == SceneRuntime.PlayerMgr.MySelf && cd.CatchType > 0)
        {
            if (cd.CatchType == (byte)CatchedType.CATCHED_SKILL)
            {
                if (cd.GoldNum > (SkillSetting.SkillDataList[cd.SubType].multiple * ExtraSetting.WonderFulBase))
                    m_Wonderful.ShowWonderfulUI(cd);
            }
            else
            {
                if (cd.GoldNum > (BulletSetting.BulletRate[SceneRuntime.PlayerMgr.GetPlayer(cd.ClientSeat).RateIndex] * ExtraSetting.WonderFulBase))
                    m_Wonderful.ShowWonderfulUI(cd);
            }

        }   
    }
    public void SetMySelfHeader(Texture2D tex)
    {
        m_SceneMatchRank.SetMySelfHeader(tex);
    }
    public void SetSceneMatchRankHeader(Texture2D tex, uint UserID)
    {
        m_SceneMatchRank.SetSceneMatchRankHeader(tex, UserID);
    }
    public void ShowMatchRankWnd( )
    {
        m_SceneMatchRank.ShowMatchRankWnd();
    }
    public void ShowGiftEff(byte bySource,byte byDest,byte  byGiftIndex)
    {
        m_Gifteff.ShowGiftEff(bySource, byDest, byGiftIndex);
    }
    public void UpdateCurMatchRank()
    {
        m_SceneMatchRank.UpdateCurMatchRank();
    }
    public void UpdateLockedUI()
    {
        m_LockedFishUI.UpdateLockedUI();
    }
    public SkillBar GetSkillBar
    {
        get { return m_SkillBar; }
    }
    public void MoveCard(GameObject goCard,Vector3 vecEnd)
    {
        m_DropItem.MoveCard(goCard, vecEnd);
    }
    public void MoveDropCard(Vector3 vecStart, Vector3 vecEnd, ushort nReward)
    {
        m_DropItem.MoveDropCard(vecStart, vecEnd,nReward);
    }

    public void DestroyLottertDraw()
    {
        m_LotteryDraw.DestroySelf();
    }
    public void ShowLotteryDraw()
    {
        m_LotteryDraw.ShowLottertDraw();
    }
    public void ShowCardChoose(ushort RewardID)
    {
        m_LotteryDraw.m_cardChoose.ShowChooseCard(RewardID);
    }
    public bool UnLockIsShow()
    {
        return m_SkillBar.UnLockIsShow();
    }
}
