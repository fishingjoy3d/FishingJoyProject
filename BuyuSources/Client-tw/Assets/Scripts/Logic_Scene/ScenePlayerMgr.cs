using UnityEngine;
using System.Collections.Generic;

//场景中的玩家集合类，玩家扩展信息和炮台相关
public class ScenePlayer
{
    public PlayerExtraData  Player = new PlayerExtraData();
    public Launcher         Launcher;
    public byte             ClientSeat;  //反转之后的客户端位置
    public byte             RateIndex;
    public SceneComboEft    ComboEft = null;     //连击效果显示
}
public class ScenePlayerMgr
{
    ScenePlayer[]   m_PlayerList = new ScenePlayer[ConstValue.PLAYER_MAX_NUM];
    Object[]        m_LauncherObj = new  Object[ConstValue.PLAYER_MAX_NUM];
    Object[]        m_GunBarrelObj = new Object[ConstValue.MAX_LAUNCHER_NUM * 2];
    Object          m_ComboObj = null;
    Object          m_ComboEftObj = null;
    Object          m_LockedObj = null;

    byte            m_MyClientSeat;  //自己的座位
    ushort          m_LockedFishID = 0;//锁定的鱼ID，0未锁定
    bool            m_bAutoShot = false;
    bool            m_bAutoLocked = false;//自动锁定
    uint            m_LockInterval = 0;
    public void Init()
    {
        for (byte i = 0; i < ConstValue.PLAYER_MAX_NUM; ++i)
        {
            m_LauncherObj[i] = ResManager.Instance.LoadObject(string.Format("BulletLauncher{0}", i), "SceneRes/Prefab/UI/Launcher/", ResType.SceneRes);
        }
        for (byte j = 0; j < m_GunBarrelObj.Length; ++j)
        {
            m_GunBarrelObj[j] = ResManager.Instance.LoadObject(string.Format("GunBarrel{0}", j), "SceneRes/Prefab/UI/Launcher/", ResType.SceneRes);
        }
        m_ComboObj = ResManager.Instance.LoadObject("Combo_UI", "SceneRes/Prefab/UI/DoubleHit/", ResType.SceneRes);
        m_ComboEftObj = ResManager.Instance.LoadObject("UIEf_Combo", "SceneRes/Prefab/UI/Combo/", ResType.SceneRes);
        m_LockedObj = ResManager.Instance.LoadObject(("LockedFishUI"), "SceneRes/Prefab/UI/LockedFish/", ResType.SceneRes);

    }
    public bool AutoShotOrLocked
    {
        get
        {
            return m_bAutoShot | (m_LockedFishID != 0);
        }
    }
    public bool AutoLocked
    {
        get
        {
            return m_bAutoLocked;
        }
    }
    public bool AutoShot
    {
        get
        {
            return m_bAutoShot;
        }
    }
    public void SetAutoShot(bool bAutoShot)
    {
        m_bAutoShot = bAutoShot;
    }
    public void SetLocked(bool bLocked)
    {
        m_bAutoLocked = bLocked;
        if (bLocked == false)
            m_LockedFishID = 0;
    }
    public void StopAutoShotAndLocked()
    {
        m_bAutoShot = false;
        m_bAutoLocked = false;
        m_LockedFishID = 0;
    }
    public byte MyClientSeat
    {
        get { return m_MyClientSeat; }
        set { m_MyClientSeat = value; }
    }
    public ScenePlayer MySelf
    {
        get
        {
            return m_PlayerList[m_MyClientSeat];
        }
    }
    public ScenePlayer GetPlayer(byte clientSeat)
    {
        if (clientSeat >= m_PlayerList.Length)
            return null;
        return m_PlayerList[clientSeat];
    }
    public Object LauncherObject(byte seat)
    {
        return m_LauncherObj[seat];
    }
    public void UpdatePlayerGold(byte clientSeat, int gold)
    {
        if (GetPlayer(clientSeat) == null)
            return;
        ScenePlayer sp = SceneRuntime.PlayerMgr.GetPlayer(clientSeat);
        PlayerRole.Instance.OnAddUserGlobelByCatchedData(clientSeat, gold);
        if (sp != null && sp.Launcher != null)
            sp.Launcher.UpdateUserGold(clientSeat);
    }
    public void UpdateVipLevelData(uint UserID)
    {
        if (UserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            return;
        byte clientSeat = PlayerRole.Instance.TableManager.GetTableRole(UserID).GetSeat();
        if (m_PlayerList[clientSeat] == null)
            return;
        if (m_PlayerList[clientSeat].Launcher == null)
            return;
        m_PlayerList[clientSeat].Launcher.UpdateVipLevelData(UserID);
    }
    public Object GetGunBarrelObj(byte type)
    {
        return m_GunBarrelObj[type];
    }
    public bool PlayerJoin(PlayerExtraData player, byte clientSeat, byte rateIdx, byte launcherType, bool valid)
    {
        if (m_PlayerList[clientSeat] != null)
        {
            LogMgr.Log("Seat palyer already exists:" + clientSeat.ToString());
            return false;
        }
        ScenePlayer sp = new ScenePlayer();
        sp.Player = player;
        sp.ClientSeat = clientSeat;
        sp.RateIndex = rateIdx;
        sp.Launcher = new Launcher(launcherType, valid, clientSeat, rateIdx);
        sp.Launcher.Init();
        m_PlayerList[clientSeat] = sp;

        if (clientSeat != MyClientSeat)
        {
            //获取头像
            if (player.playerData.ImgCrc < ConstValue.CUSTOM_HEADER)
                sp.Launcher.SetPlayerImg(null, player.playerData.ImgCrc);
            else
                HeaderManager.Instance.GetPlayerHeader(player.playerData.ID, player.playerData.ImgCrc, HeaderOptType.HEADER_SCENE, clientSeat);
        }
        else
        {
            SceneRuntime.LogicUI.GetSkillBar.UpdateData();
            SceneRuntime.LogicUI.UpdateSkillState();
        }
        //获取金币
        m_PlayerList[clientSeat].Launcher.UpdateUserGold(clientSeat);
        
        
        
        return true;
    }
    public void SetPlayerHeader(uint playerID, byte seat, Texture2D tex)
    {
        if (GetPlayer(seat) == null)
            return;
        if (playerID == GetPlayer(seat).Player.playerData.ID)
        {
            GetPlayer(seat).Launcher.SetPlayerImg(tex, GetPlayer(seat).Player.playerData.ImgCrc);
        }
    }
    public void PlayerLeave(byte clientSeat)
    {
        if (m_PlayerList[clientSeat] == null)
        {
            LogMgr.Log("Seat not found player:" + clientSeat.ToString());
            return;
        }
       
        m_PlayerList[clientSeat].Launcher.Shutdown();
        if (m_PlayerList[clientSeat].ComboEft != null)
        {
            m_PlayerList[clientSeat].ComboEft.ShutDown();
            m_PlayerList[clientSeat].ComboEft = null;
        }
        m_PlayerList[clientSeat] = null;
        SceneRuntime.BulletMgr.PlayerLeave(clientSeat);
        if (clientSeat == MyClientSeat)
        {
            SceneRuntime.LauncherEftMgr.RemoveAtEffect();
            SceneRuntime.LauncherEftMgr.StopXPSkillActiveEft();
            SceneRuntime.LauncherEftMgr.StopXPSKillLiveEft();
        }

    }
    public void UpdateEnergy(int energy)
    {
        if (GetPlayer(m_MyClientSeat) == null)
            return;
        m_PlayerList[m_MyClientSeat].Launcher.UpdateEnergy(energy);
    }
    public void Update(float delta)
    {
       // -------------------------------------
        if (Input.GetMouseButtonDown(0) && SceneRuntime.HandleClickEvent == false && m_bAutoLocked)
        {
            ushort fishid = SceneRuntime.FishMgr.GetFishIDByScreenPoint(Input.mousePosition);
            //m_bAutoLocked = fishid != 0;
            m_LockedFishID = fishid;
        }
       // -------------------------------------
        if (m_LockedFishID != 0)
        {
            Vector3 startScrPoint = SceneRuntime.GetLauncherScrStartPos(MyClientSeat);
            Vector3 startViewPoint = SceneRuntime.GetLauncherViewStartPos(MyClientSeat);
            Fish fish = SceneRuntime.FishMgr.FindFishByID(m_LockedFishID);
            
            if (fish == null || fish.IsDelay || fish.Catched || Utility.IsInScreen(fish.ScreenPos) == false || Utility.CheckLauncherAngle(fish, startScrPoint, startViewPoint) == false)
            {
                m_LockedFishID = 0;
            }
        }
        else if(m_bAutoLocked)
        {
            //if(Utility.GetTickCount() - m_LockInterval > 0)
            {
                Vector3 startScrPoint = SceneRuntime.GetLauncherScrStartPos(MyClientSeat);
                Vector3 startViewPoint = SceneRuntime.GetLauncherViewStartPos(MyClientSeat);
                m_LockInterval = Utility.GetTickCount();
                m_LockedFishID = SceneRuntime.FishMgr.GetFishBySortGold(startScrPoint, startViewPoint);
            }
        }

        SceneRuntime.LogicUI.UpdateLockedUI();

        for (int i = 0; i < ConstValue.PLAYER_MAX_NUM; ++i)
        {
            if (m_PlayerList[i] == null)
                continue;
            ScenePlayer sp = m_PlayerList[i];
            sp.Launcher.Update(delta);
            if (sp.ComboEft != null && sp.ComboEft.m_BaseWndObject != null)
            {
                if (sp.ComboEft.m_BaseWndObject.activeSelf != true)
                    continue;
                if (sp.ComboEft.Updata(delta) == false)
                    sp.ComboEft.m_BaseWndObject.SetActive(false);
            }
        }
        
    }
    public ushort LockedFishID
    {
        get
        {
            return m_LockedFishID;
        }
        set
        {
            m_LockedFishID = value;
        }
    }
    public void UpdatePlayerGold(uint UserID)
    {
        byte clientSeat = 0;
        if (UserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            clientSeat = PlayerRole.Instance.TableManager.GetTableRole(UserID).GetSeat();
        else
            clientSeat = m_MyClientSeat;
        if (m_PlayerList[clientSeat] == null)
            return;
        m_PlayerList[clientSeat].Launcher.UpdateUserGold(clientSeat);
    }
    public void UpdateMyLauncherChange()
    {
        m_PlayerList[m_MyClientSeat].Launcher.UpdateLauncherChange();
    }
    public void ChangeLauncher(NetCmdPack pack)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_ChangeGun, false, true);
        NetCmdChangeLauncher ncc = (NetCmdChangeLauncher)pack.cmd;
        bool launcherValid;
        byte clientLauncherType;
        byte clientSeat = SceneRuntime.ServerToClientSeat(ncc.Seat);
        if (GetPlayer(clientSeat) == null)
            return;
        SceneRuntime.CheckLauncherValid(ncc.LauncherType, out clientLauncherType, out launcherValid);
        m_PlayerList[clientSeat].Launcher.ChangeLauncher(clientLauncherType, launcherValid);

        //change launcherType;
        if (m_MyClientSeat == clientSeat)
        {
            SceneRuntime.LogicUI.UpateLaunchData();
            SceneRuntime.LogicUI.UpdateSkillState();
        }
    }
    public void NewPlayerProtect()
    {
        SceneRuntime.SceneLogic.ChangeDestLauncher(0);
     //   byte rateIndex = (byte)(BulletSetting.BulletRate.Length + 1);
        byte rateIndex = 0;
        SceneRuntime.SceneLogic.ChangeDestRate(rateIndex);       
    }
    public void ChangeRate(NetCmdPack pack)
    {
        NetCmdChangeRate cmd = (NetCmdChangeRate)pack.cmd;
        byte clientSeat = SceneRuntime.ServerToClientSeat(cmd.Seat);
        if (GetPlayer(clientSeat) == null)
            return;
        byte OldRate = GetPlayer(clientSeat).RateIndex;
        GetPlayer(clientSeat).RateIndex = cmd.RateIndex;
        //Debug.Log(cmd.RateIndex);
        GetPlayer(clientSeat).Launcher.ChangeRate(cmd.RateIndex, cmd.IsCanUseRate);
        
        //如果是自己切换倍率我们就清掉连击状态
        if (clientSeat == m_MyClientSeat)
        {
            if (m_PlayerList[clientSeat].ComboEft != null)
            {  
                if (m_PlayerList[clientSeat].ComboEft.LifeTime >= 0.1f)
                    m_PlayerList[clientSeat].ComboEft.LifeTime = 0;
            }

        }
        //因为玩家切换倍率 我们触发一个事件
        tagGameRateChangeEvent pEvent = new tagGameRateChangeEvent(GetPlayer(clientSeat).Player.playerData.ID, OldRate,cmd.RateIndex);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public void LaunchBullet(NetCmdPack pack)
    {
        NetCmdBullet cmd = (NetCmdBullet)pack.cmd;
        byte clientSeat, id;
        SceneRuntime.BuuletIDToSeat(cmd.BulletID, out clientSeat, out id);
        if (GetPlayer(clientSeat) == null)
            return;
        ScenePlayer sp = GetPlayer(clientSeat);
        if (sp.Launcher.LauncherType != cmd.LauncherType)
        {
            sp.Launcher.ChangeLauncher(cmd.LauncherType, true);
        }
        if(sp == null)
        {
            LogMgr.Log("不存在的玩家座位:" + clientSeat);
            return;
        }
        float elapsedTime = (Utility.GetTickCount() - pack.tick) * 0.001f + SceneRuntime.NetDelayTime;
        short degree = SceneRuntime.AngleInversion(cmd.Degree);
        SceneRuntime.BulletMgr.LaunchBullet(cmd.BulletID, sp.Launcher.LauncherType, sp.RateIndex, degree, elapsedTime, cmd.ReboundCount, cmd.LockFishID);
        sp.Launcher.LauncherBullet(cmd.Energy);
        
       // SceneRuntime.SceneLogic.LaunchBullet(clientSeat, sp.Launcher.LauncherType, sp.RateIndex);
        int gold = LauncherSetting.LauncherDataList[sp.Launcher.LauncherType].Consume 
            * BulletSetting.BulletRate[sp.RateIndex];
        ConsumeGold(clientSeat, gold, sp);
       
    }
    public void LaunchSyncBullet(NetCmdPack pack)
    {
        NetCmdSyncBullet cmd = (NetCmdSyncBullet)pack.cmd;
        float elapsedTime = (Utility.GetTickCount() - pack.tick) * 0.001f + SceneRuntime.NetDelayTime;
        for(int i = 0; i < cmd.Bullets.Length; ++i)
        {
            SyncBulletData data = cmd.Bullets[i];
            byte clientSeat, id;
            SceneRuntime.BuuletIDToSeat(data.BulletID, out clientSeat, out id);
            ScenePlayer sp = GetPlayer(clientSeat);
            if (sp == null)
            {
                //LogMgr.Log("不存在的玩家座位:" + clientSeat);
                return;
            }
            short degree = SceneRuntime.AngleInversion(data.Degree);
            SceneRuntime.BulletMgr.LaunchBullet(data.BulletID, data.BulletType, data.RateIdx, degree, data.Time * 0.001f + elapsedTime, data.ReboundCount, data.LockFishID/*, data.CollideCount, data.PauseTime * 0.001f, data.SpeedScaling * 0.001f*/);
            //int gold = LauncherSetting.LauncherDataList[data.BulletType].Consume * BulletSetting.BulletRate[data.RateIdx];
            //ConsumeGold(clientSeat, gold, sp);
        }
    }
    public void SetMatchGoldBg(uint UserID)
    {
        byte clientSeat = 0;
        if (UserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            clientSeat = PlayerRole.Instance.TableManager.GetTableRole(UserID).GetSeat();
        else
            clientSeat = m_MyClientSeat;
        if (m_PlayerList[clientSeat] == null)
            return;
        m_PlayerList[clientSeat].Launcher.SetMatchGoldBg();
    }
    public void ShowLotteryState(uint userID, bool bState)
    {
        Debug.Log("Lottery0: " + bState.ToString());

        for (int i = 0; i < m_PlayerList.Length; ++i)
        {
            if (m_PlayerList[i] == null || m_PlayerList[i].ClientSeat == MyClientSeat ||
                userID != m_PlayerList[i].Player.playerData.ID)
                continue;
            m_PlayerList[i].Launcher.ShowLotteryState(bState);
        }
    }
    public void ShowOtherUserLocked(byte clientSeat)
    {
        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) != null)
        {
            SceneRuntime.PlayerMgr.GetPlayer(clientSeat).Launcher.ShowOtherUserLocked();
        }
    }
    public void ClearPlayer()
    {
        for (int i = 0; i < m_PlayerList.Length; ++i)
        {
            if (m_PlayerList[i] == null || m_PlayerList[i].ClientSeat == MyClientSeat)
                continue;

            m_PlayerList[i].Launcher.Shutdown();
            if (m_PlayerList[i].ComboEft != null)
                m_PlayerList[i].ComboEft.ShutDown();
            m_PlayerList[i] = null;
        }
    }
    public void ClearAllPlayer()
    {
        for (int i = 0; i < m_PlayerList.Length; ++i)
        {
            if (m_PlayerList[i] == null)
                continue;

            m_PlayerList[i].Launcher.Shutdown();
            if (m_PlayerList[i].ComboEft != null)
                m_PlayerList[i].ComboEft.ShutDown();
            m_PlayerList[i] = null;
        }
    }
    public void Shutdown()
    {
        for(int i = 0; i < m_PlayerList.Length; ++i)
        {
            if(m_PlayerList[i] != null)
            {
                m_PlayerList[i].Launcher.Shutdown();
                if (m_PlayerList[i].ComboEft != null)
                    m_PlayerList[i].ComboEft.ShutDown();
                m_PlayerList[i] = null;
            }

            if (m_LauncherObj[i] != null)
            {
                ResManager.Instance.UnloadObject(m_LauncherObj[i]);
            }
            
        }

        for (byte j = 0; j < m_GunBarrelObj.Length; ++j)
        {
            ResManager.Instance.UnloadObject(m_GunBarrelObj[j]);
        }
        
        ResManager.Instance.UnloadObject(m_ComboObj);
        ResManager.Instance.UnloadObject(m_LockedObj);

    }
    public void ConsumeGold(byte clientSeat, int gold, ScenePlayer sp)
    {
        if(m_PlayerList[clientSeat] == null)
        {
            Debug.Log("client Player is null.");
            return;
        }
        if (!MatchJudge.IsMatch()&&clientSeat==m_MyClientSeat)
        {
            GlobalHallUIMgr.Instance.GameShare.AddGlod(-gold);        
        }
        PlayerRole.Instance.OnAddUserGlobelByCatchedData(clientSeat, -gold);
        m_PlayerList[clientSeat].Launcher.UpdateUserGold(m_PlayerList[clientSeat].ClientSeat);
    }
    public void ChangeLauncherAngle(Vector3 dir, byte seat)
    {
        m_PlayerList[seat].Launcher.Direction = dir;
        m_PlayerList[seat].Launcher.UpdatOtherAngle();
    }
    public void PlayCD( float CDTime)
    {
        for (byte i = 0; i < ConstValue.PLAYER_MAX_NUM; ++i )
        {
            if (m_PlayerList[i] != null)
                m_PlayerList[i].Launcher.PlayCD(CDTime);
        }
    }
    public void LaunchLaserFailed(NetCmdLaunchFailed lf)
    {
        GetPlayer(m_MyClientSeat).Launcher.LaunchLaserFailed(lf);
    }
    public void LaunchLaser(byte clientSeat, byte launcherType, short degree)
    {
        m_PlayerList[clientSeat].Launcher.LaunchLaser(clientSeat, degree);
    }
    public void FishCatch(byte clientSeat, ushort combo)
    {
        ScenePlayer player = m_PlayerList[clientSeat];
        ushort ComboCount = (ushort)(combo & 0x7FFF);
        bool IsComboCRI = ((combo & 0x8000) != 0) ? true : false;
        if (ComboCount > 0)
            ShowCombo(clientSeat, ComboCount, IsComboCRI);
    }
    //播放头像上的特效
    public void PlaySkillAvatarEft(byte clientSeat)
    {
        GetPlayer(clientSeat).Launcher.PlaySkillAvatarEft();
    }
   
    void ShowCombo(byte clientSeat, ushort ComboCount, bool bCRI /*是否必杀*/)
    {
        if (m_PlayerList[clientSeat].ComboEft == null)
        {
            SceneComboEft combo = new SceneComboEft();
            combo.Pos =  SceneRuntime.GetLauncherGoldIconPos(clientSeat);
            if (m_MyClientSeat == clientSeat)
                combo.IsMySelf = true;
            else
                combo.IsMySelf = false;

            combo.Init(m_ComboObj);

            m_PlayerList[clientSeat].ComboEft = combo;
        }
        m_PlayerList[clientSeat].ComboEft.ShowCombo(ComboCount, bCRI);
        

        if (clientSeat == m_MyClientSeat)
        {
            PlayerRole.Instance.RoleGameData.OnHandleRoleCombo(ComboCount);
        }
    }
    //获取连击必杀特效Object
    public Object ComboEftObj
    {
        get { return m_ComboEftObj; }
    }
    public Object LockedObj
    {
        get
        {
            return m_LockedObj;
        }
    }
}

