using UnityEngine;
using System.Collections.Generic;
public class SceneLogic:ILogic, ICmdHandler
{
    SceneLogicUI        m_LogicUI;
    SceneBulletMgr      m_BulletMgr;
    SceneFishMgr        m_FishMgr;
    SceneEffectMgr      m_EffectMgr;
    ScenePlayerMgr      m_PlayerMgr;
    SceneSkillMgr       m_SkillMgr;
    LauncherEffectMgr   m_LauncherEffectMgr;
    SceneChestMgr       m_ChestMgr;
    SceneBtnsMrg        m_BtnsMgr;
    SceneNewGuide       m_NewGide;

    byte                m_RoomType;
    byte                m_RoomRateIndex;
    int                 m_InitStartCount = 0;
    bool                m_bClearScene;
    float               m_fClearTime = 0;
    bool                m_bRefreshScene = false;
    public bool Init(ILogicUI logicUI, object obj)
    {
        SceneObjMgr.Instance.SetShowType(ShowFlagType.SHOW_PING);
        m_LogicUI = (SceneLogicUI)logicUI;
        AddStartCount();
        SceneMain.Instance.StartInnerCoroutine(MainInitProcedure(obj));
        return true;
    }
    public System.Collections.IEnumerator MainInitProcedure(object obj)
    {
        //异步加载
        m_FishMgr = new SceneFishMgr();
        m_BulletMgr = new SceneBulletMgr();
        m_PlayerMgr = new ScenePlayerMgr();
        m_SkillMgr = new SceneSkillMgr();
        m_EffectMgr = new SceneEffectMgr();
        m_LauncherEffectMgr = new LauncherEffectMgr();
        m_ChestMgr = new SceneChestMgr();
        m_BtnsMgr = new SceneBtnsMrg();
        m_NewGide = new SceneNewGuide();

        m_FishMgr.Init(); yield return new WaitForEndOfFrame();
        m_BulletMgr.Init(); yield return new WaitForEndOfFrame();
        m_LauncherEffectMgr.Init(); yield return new WaitForEndOfFrame();
        m_PlayerMgr.Init(); yield return new WaitForEndOfFrame();
        m_SkillMgr.Init(); yield return new WaitForEndOfFrame();
        m_EffectMgr.Init(); yield return new WaitForEndOfFrame();
        m_ChestMgr.Init(); yield return new WaitForEndOfFrame();
        m_BtnsMgr.Init(); yield return new WaitForEndOfFrame();
        m_NewGide.InitObj(); yield return new WaitForEndOfFrame();
        //加载技能栏
        m_LogicUI.InitByScene();

        SceneRuntime.Init(this);
        ResetScene((JoinRoomData)obj, true);

        while(PathManager.Instance.IsInitOK == false)
        {
            yield return new WaitForEndOfFrame();
        }
        GlobalLoading.Instance.Hide();
        GlobalHallUIMgr.Instance.ShowHead(HeadWind.HeadStatue.Head_Normal);
        GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound1);
       // m_NewGide.CheckIsLauncherGuide();
        //调整系公告位置
        GlobalHallUIMgr.Instance.AdjustNoticePos();
        m_LogicUI.UpdateUnLockDataInfo();
        m_LogicUI.UpdateLotteryInfo();
        SubStartCount();
        yield break;
    }
    public void ResetScene(JoinRoomData jrd, bool bFirst = false)
    {
        NetServices.Instance.CmdHandler = this;
        ResetPlayerData(jrd, bFirst);
        if(bFirst == false)
            RefreshScene();
    }
    public bool CanProcessCmd()
    {
        return InitCompletion;
    }
    public bool InitCompletion
    {
        get
        {
            return m_InitStartCount == 0;
        }
    }
    public  void AddStartCount()
    {
        ++m_InitStartCount;
    }
    public void SubStartCount()
    {
        --m_InitStartCount;
        if(m_InitStartCount == 0)
        {
            GlobalEffectMgr.Instance.CloseLoadingMessage();
            LogicManager.Instance.InitCompletion();
        }
    }
    public void Shutdown()
    {
        GlobalHallUIMgr.Instance.CloseHead();
        m_FishMgr.Shutdown();
        m_BulletMgr.Shutdown();
        m_PlayerMgr.Shutdown();
        m_SkillMgr.Shutdown();
        m_EffectMgr.Shutdown();
        m_LauncherEffectMgr.ShutDown();
        m_ChestMgr.ShutDown();
        m_BtnsMgr.ShutDown();
        m_NewGide.ShutDown();
       // GlobalHallUIMgr.Instance.ShutDowNotic();
        GlobalEffectMgr.Instance.Clear();
        SceneRuntime.Shutdown();
        PlayerRole.Instance.OnUserLeaveTable();
    }
    public bool bClearScene
    {
        get { return m_bClearScene; }
    }
    public SceneBulletMgr BulletMgr
    {
        get { return m_BulletMgr; }
    }
    public SceneBtnsMrg BtnsMgr
    {
        get { return m_BtnsMgr; }
    }
    public SceneFishMgr FishMgr
    {
        get { return m_FishMgr; }
    }

    public SceneEffectMgr EffectMgr
    {
        get { return m_EffectMgr; }
    }

    public ScenePlayerMgr PlayerMgr
    {
        get { return m_PlayerMgr; }
    }
    public SceneNewGuide NewGide
    {
        get { return m_NewGide; }
    }
    public LauncherEffectMgr LauncherEftMgr
    {
        get { return m_LauncherEffectMgr; }
    }
    public SceneChestMgr ChestMgr
    {
        get { return m_ChestMgr; }
        set { m_ChestMgr = value; }
    }
    public SceneLogicUI LogicUI
    {
        get { return m_LogicUI; }
    }
    public void Send<T>(NetCmdBase ncb)
    {
        NetServices.Instance.Send<T>(ncb);
    }
    public SceneSkillMgr SkillMgr
    {
        get { return m_SkillMgr; }
    }
    public void StateChanged(NetState state)
    {
        if (state == NetState.NET_DISCONNECT)
        {
            GlobalLogon.Instance.ReconnectServer();
            //NativeInterface.ShowMsg("disconnect", 150);
            //GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("disconnect"), "", "", false);
            //LogicManager.Instance.LogOff();
        }
    }
    public void LaunchBullet(byte ClientSeat, byte bulletType, byte rateIndex)
    {
        //当玩家开炮的时候 我们扣除金币
        PlayerRole.Instance.HandleLaunchBullet(ClientSeat, bulletType, rateIndex);
    }
    public void CatchFish(CatchedData cd)
    {
        PlayerRole.Instance.HandeCatchFishData(cd);
        PlayMusic(cd);
        m_LogicUI.ShowWonderfulUI(cd);
    }
    void PlayMusic(CatchedData cd)
    {
        
        float volume = 1;
        if (SceneRuntime.PlayerMgr.GetPlayer(cd.ClientSeat) == SceneRuntime.PlayerMgr.MySelf)
            volume = 1;
        else
            volume = 0.30f;
        if (cd.CatchType == 2)
        {           
            GlobalAudioMgr.Instance.PlaySkillMusic(Audio.SkillMusic.m_Skill_0 + cd.SubType, false, false, volume);
        }
        else if (cd.CatchType == 0)
        {
            byte[] type = { 5,5, 6, 7, 8 };
            GlobalAudioMgr.Instance.PlayOrdianryMusic((Audio.OrdianryMusic)type[cd.SubType],false,false,volume);
        }
        else if (cd.CatchType == 1)
        {
            byte[] type = { 6, 6, 7, 8, 9 };
            GlobalAudioMgr.Instance.PlaySkillMusic((Audio.SkillMusic)type[cd.SubType], false, true, volume);
        }
    }
    public void SetClearScene()
    {
        m_LogicUI.ShowClearSceneMsg();
        m_bClearScene = true;
        m_fClearTime = 0;
    }
    public void Update(float delta)
    {
        if(m_bClearScene)
        {
            m_fClearTime += delta;
            if (m_fClearTime >= ConstValue.CLEAR_TIME)
                m_bClearScene = false;
        }
        //初始化已经完成了
        if (InitCompletion == false)
            return;
        m_FishMgr.Update(delta);
        m_BulletMgr.Update(delta);
        m_PlayerMgr.Update(delta);
        m_SkillMgr.Update(delta);
        m_EffectMgr.Update(delta);
        m_LauncherEffectMgr.Update(delta);
        m_ChestMgr.Update(delta);
        m_BtnsMgr.Update(delta);
        m_NewGide.Update(delta);
    }
    public void LateUpdate(float delta)
    {
        if (InitCompletion == false)
            return;
        //FioshMgr,BulletMgr,必须在网络命令之后进行更新。
        
    }
    //命令处理
    //======================================================================
    public void PlayerJoin(NetCmdPack pack)
    {
        NetCmdPlayerJoin ncp    = (NetCmdPlayerJoin)pack.cmd;
        PlayerExtraData pd      = new PlayerExtraData();
        pd.playerData.ID        = ncp.PlayerInfo.ID;
        pd.playerData.GoldNum   = ncp.PlayerInfo.GoldNum;
        pd.playerData.ImgCrc    = ncp.PlayerInfo.ImgCrc;
        pd.playerData.Level     = ncp.PlayerInfo.Lvl;
        pd.playerData.Name      = ncp.PlayerInfo.Name;
        byte clientSeat         = SceneRuntime.ServerToClientSeat(ncp.Seat);

        bool launcherValid;
        byte clientLauncherType;
        SceneRuntime.CheckLauncherValid(ncp.LauncherType, out clientLauncherType, out launcherValid);
        m_PlayerMgr.PlayerJoin(pd, clientSeat, ncp.rateIndex, clientLauncherType, launcherValid);//m_RoomRateIndex
    }
    public void ResetPlayerData(JoinRoomData jrd, bool bFirst)
    {
        byte serverSeat = jrd.Seat;
        byte serverLauncherType = jrd.LauncherType;
        SceneObjMgr.Instance.SwapBackgroundImage(jrd.BackgroundImage);
        m_RoomType = jrd.RoomID;
        m_RoomRateIndex = FishConfig.Instance.m_TableInfo.m_TableConfig[m_RoomType].MinRate;// ExtraSetting.RoomDataList[m_RoomType].RoomRateIdx;
        //管理器初始化
        if (bFirst || SceneRuntime.BackgroundIndex != jrd.BackgroundImage)
        {
            m_EffectMgr.ClearBackEffect();
            m_EffectMgr.LoadBackEffect(jrd.BackgroundImage);
        }
        SceneRuntime.Inversion = serverSeat > 1;
        SceneRuntime.BackgroundIndex = jrd.BackgroundImage;
        m_PlayerMgr.MyClientSeat = SceneRuntime.ServerToClientSeat(serverSeat);
        PlayerMgr.ClearAllPlayer();
        //加入自己
        bool launcherValid;
        byte clientLauncherType;
        SceneRuntime.CheckLauncherValid(
            serverLauncherType,
            out clientLauncherType,
            out launcherValid);
        //获取自己的消息
        RoleMe pMe = PlayerRole.Instance.RoleInfo.RoleMe;
        pMe.SetSeat(serverSeat);
        PlayerExtraData pPlayer = new PlayerExtraData();
        pPlayer.playerData.GoldNum =
            (int)PlayerRole.Instance.GetPlayerGlobelBySeat(pMe.GetSeat());
        pPlayer.playerData.ID = pMe.GetUserID();
        pPlayer.playerData.ImgCrc = pMe.GetFaceID();
        pPlayer.playerData.Level = (byte)pMe.GetLevel();
        pPlayer.playerData.Name = pMe.GetNickName();
        m_PlayerMgr.PlayerJoin(pPlayer,
            m_PlayerMgr.MyClientSeat,
            jrd.RateIndex,
            clientLauncherType,
            launcherValid);
        m_PlayerMgr.UpdateEnergy(jrd.Energy);
    }
    public void RefreshScene(bool bSendResetCmd = false)
    {
        m_PlayerMgr.ClearPlayer();
        m_FishMgr.ClearAllFish();
        m_BulletMgr.ClearAllBullet();
        m_EffectMgr.Clear();
        NetCmdLaunchFailed dd = new NetCmdLaunchFailed();
        dd.Energy = 0;
        dd.FailedType = (byte)LaunchFailedType.LFT_CD;
        m_PlayerMgr.LaunchLaserFailed(dd);
        m_bRefreshScene = true;
        if (bSendResetCmd)
            PlayerRole.Instance.RoleInfo.ResetSceneInfo();
    }
    public void PlayerLeave(NetCmdPack pack)
    {
        byte clientSeat = SceneRuntime.ServerToClientSeat(((NetCmdPlayerLeave)pack.cmd).Seat);
        m_PlayerMgr.PlayerLeave(clientSeat);
    }
    public void ChangeLauncher(byte launcherType)
    {
        NetCmdChangeLauncher ncb = new NetCmdChangeLauncher();
        ncb.SetCmdType(NetCmdType.CMD_CHANGE_LAUNCHER);
        ncb.Seat = SceneRuntime.ClientToServerSeat(m_PlayerMgr.MyClientSeat);
        ncb.LauncherType = launcherType;
        Send<NetCmdChangeLauncher>(ncb);
    }
    //切换到指定类型 的炮
    public void ChangeDestLauncher(byte launcherType)
    {
        NetCmdChangeLauncherType ncb = new NetCmdChangeLauncherType();
        ncb.SetCmdType(NetCmdType.CMD_CHANGE_LAUNCHER_TYPE);
        ncb.Seat = SceneRuntime.ClientToServerSeat(m_PlayerMgr.MyClientSeat);
        ncb.LauncherType = launcherType;
        Send<NetCmdChangeLauncherType>(ncb);
    }
    public void ChangeRate(byte rateIndex)
    {
        NetCmdChangeRate ncb = new NetCmdChangeRate();
        ncb.SetCmdType(NetCmdType.CMD_CHANGE_RATE);
        ncb.Seat = SceneRuntime.ClientToServerSeat(m_PlayerMgr.MyClientSeat);
        ncb.RateIndex = rateIndex;
        Send<NetCmdChangeRate>(ncb);
    }
    public void ChangeRate(bool UpOrDown)
    {
        NetCmdClientChangeRate ncb = new NetCmdClientChangeRate();
        ncb.SetCmdType(NetCmdType.CMD_CHANGE_CLIENT_RATE);
        ncb.UpRoDownRate = UpOrDown;
        Send<NetCmdClientChangeRate>(ncb);
    }
    //切换到指定倍率
    public void ChangeDestRate(byte rateIndex)
    {
        NetCmdChangeRateType ncb = new NetCmdChangeRateType();
        ncb.SetCmdType(NetCmdType.CMD_CHANGE_RATE_TYPE);
        ncb.Seat = SceneRuntime.ClientToServerSeat(m_PlayerMgr.MyClientSeat);
        ncb.RateIndex = rateIndex;
        Send<NetCmdChangeRateType>(ncb);
    }
    uint bulletTick = 0;
    public void LaunchBullet(short angle)
    {
        if (m_bClearScene)
        {
            //清场时不能发子弹。
            return;
        }
        NetCmdBullet ncb = new NetCmdBullet();
        ncb.SetCmdType(NetCmdType.CMD_BULLET);
        ncb.Degree = angle;
        ncb.LockFishID = m_PlayerMgr.LockedFishID;
        ncb.LauncherType = SceneRuntime.SceneLogic.PlayerMgr.MySelf.Launcher.LauncherType;
        Send<NetCmdBullet>(ncb);
        bulletTick = Utility.GetTickCount();
    }
    public void UseSkill(SkillType skill)
    {
        if (m_bClearScene)
        {
            //清场时不能发技能。
            return;
        }      
        NetCmdUseSkill cmd = new NetCmdUseSkill();
        cmd.SetCmdType(NetCmdType.CMD_USE_SKILL);
        cmd.SkillID = (byte)skill;
        Send<NetCmdUseSkill>(cmd);
    }
    public bool UseLaser(short degree)
    {
        if (m_bClearScene)
        {
            //清场时不能发大招。
            return false;
        }
        NetCmdSkillLaser nc = new NetCmdSkillLaser();
        nc.SetCmdType(NetCmdType.CMD_SKILL_LASER_REQUEST);
        nc.Degree = degree;
        Send<NetCmdSkillLaser>(nc);
        return true;
    }
    //返回大厅
    public void BackToHall()
    {
        if(SceneMain.IsUserOldServer)
        {
            LogicManager.Instance.LogOff();
            //LogicManager.Instance.Back(null);
        }
        else
        {
            //发送玩家离开桌子的命令
            CL_Cmd_LeaveTable ncb = new CL_Cmd_LeaveTable();
            ncb.SetCmdType(NetCmdType.CMD_CL_LeaveTable);
            NetServices.Instance.Send<CL_Cmd_LeaveTable>(ncb);

            PlayerRole.Instance.RoleGameData.OnHandleRoleLeaveTable();
           
            //让玩家直接离开场景
            LogicManager.Instance.Back(null);//返回大厅就Ok了
            int bg = 1;
            GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound2);        
        }
       
    }
    //处理网络命令
    public bool Handle(NetCmdPack pack)
    {
        if(m_bRefreshScene)
        {
            //刷新场景中，过期的命令不处理
            if(pack.cmd.CmdType == (byte)MainCmdType.CMD_MAIN_Game)
                return true;
            else if(pack.cmd.GetCmdType() == NetCmdType.CMD_LC_ResetRoleInfo)
                m_bRefreshScene = false;
        }
        switch (pack.cmd.GetCmdType())
        {
            case NetCmdType.CMD_FISH:
                m_FishMgr.LaunchFish(pack);
                break;
            case NetCmdType.CMD_SYNC_FISH:
                m_FishMgr.LaunchFishByAsycScene(pack);
                break;
            case NetCmdType.CMD_BULLET:
                NetCmdBullet cmd = (NetCmdBullet)pack.cmd;
                byte clientSeat, id;
                SceneRuntime.BuuletIDToSeat(cmd.BulletID, out clientSeat, out id);
                if (clientSeat == SceneRuntime.MyClientSeat)
                {
                    uint time = Utility.GetTickCount() - bulletTick;
                    SceneMain.Instance.bulletTime.AddTime(time);
                }
                m_PlayerMgr.LaunchBullet(pack);
                break;
            case NetCmdType.CMD_SYNC_BULLET:
                m_PlayerMgr.LaunchSyncBullet(pack);
                break;
            case NetCmdType.CMD_PLAYER_JOIN:
                PlayerJoin(pack);
                break;
            case NetCmdType.CMD_PLAYER_LEAVE:
                PlayerLeave(pack);
                break;
            case NetCmdType.CMD_CHANGE_LAUNCHER:
                m_PlayerMgr.ChangeLauncher(pack);
                break;
            case NetCmdType.CMD_CATCHED:
                m_SkillMgr.FishCatched(pack);
                break;
            case NetCmdType.CMD_CHANGE_RATE:
                m_PlayerMgr.ChangeRate(pack);
                break;
            case NetCmdType.CMD_SKILL_DISASTER_RESPONSE:
                m_SkillMgr.UseSkillDisaster(pack);
                break;
            case NetCmdType.CMD_SKILL_LOCK_RESPONSE:
                m_SkillMgr.UseSkillLock(pack);
                break;
            case NetCmdType.CMD_SKILL_FREEZE_RESPONSE:
                m_SkillMgr.UseSkillFreeze(pack);
                break;
            case NetCmdType.CMD_SKILL_LASER_RESPONSE:
                m_SkillMgr.LaunchLaser(pack);
                break;
            case NetCmdType.CMD_SKILL_LIGHTING_RESPONSE:
                m_SkillMgr.UseSkillLighting(pack);
                break;
            case NetCmdType.CMD_SKILL_TORNADO_RESPONSE:
                m_SkillMgr.UseSkillTornado(pack);
                break;
            case NetCmdType.CMD_FISH_PACKAGE:
                m_FishMgr.SetFishPackage(pack); 
                break;
            case NetCmdType.CMD_BULLET_REDUCTION:
                m_SkillMgr.FishCatchedByFreeze(pack);
                break;
            case NetCmdType.CMD_REDUCTION:
                m_SkillMgr.Reduction(pack);
                break;
            case NetCmdType.CMD_CLEAR_SCENE:
                m_SkillMgr.ClearScene(pack);
                break;
            case NetCmdType.CMD_LAUNCH_LASER_FAILED:
                m_PlayerMgr.LaunchLaserFailed((NetCmdLaunchFailed)pack.cmd);
                break;
            case NetCmdType.CMD_SKILL_FAILLED:
                {
                    m_LogicUI.GetSkillBar.SkillFail((NetCmdSkillFailed)pack.cmd);
                    break;
                }
            default:
                return false;
        }
        return true;
    }
}
