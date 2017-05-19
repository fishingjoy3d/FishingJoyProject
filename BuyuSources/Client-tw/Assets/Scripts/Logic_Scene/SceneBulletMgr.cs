using UnityEngine;
using System.Collections.Generic;

public struct DestroyBulletData
{
    public byte LauncherType;
    public byte RateIndex;
}
public class PlayerBullets
{
    public Dictionary<byte, Bullet> BulletList = new Dictionary<byte, Bullet>();
    public Dictionary<byte, DestroyBulletData> DestroyBulletList = new Dictionary<byte, DestroyBulletData>();
}

public class SceneBulletMgr
{
    PlayerBullets[] m_PlayerBullets;
    Object[] m_BulletObj = new Object[ConstValue.MAX_LAUNCHER_NUM];
    Object[]    m_BulletParObj = new Object[ConstValue.MAX_LAUNCHER_NUM];
 //   uint        m_BulletCount = 0;
    public void Init()
    {
        m_PlayerBullets = new PlayerBullets[ConstValue.PLAYER_MAX_NUM];
        for(int i = 0; i < m_PlayerBullets.Length; ++i)
        {
            m_PlayerBullets[i] = new PlayerBullets();
        }

        for (int j = 0; j < m_BulletObj.Length; ++j)
        {
            string str = string.Format("LauncherBullet{0}", j);
            m_BulletObj[j] = ResManager.Instance.LoadObject(str, "SceneRes/Prefab/Effect/Launcher/Bullet/", ResType.SceneRes);

            str = string.Format("LauncherPar{0}", j);
            m_BulletParObj[j] = ResManager.Instance.LoadObject(str, "SceneRes/Prefab/Effect/Launcher/Par/", ResType.SceneRes);
        }
    }
    public Object[] BulletObj
    {
        get { return m_BulletObj; }
        set { m_BulletObj = value; }
    }
    public void RemoveBullet(byte clientSeat, byte id)
    {
        PlayerBullets pb = m_PlayerBullets[clientSeat];
        Bullet bullet;
        if(pb.BulletList.TryGetValue(id, out bullet))
        {
            InnerRemoveBullet(clientSeat, bullet);
            bullet.Destroy();
            pb.BulletList.Remove(id);
        }
    }
    public void InnerRemoveBullet(byte clientSeat, Bullet bullet)
    {
        DestroyBulletData dbd;
        dbd.LauncherType = bullet.LauncherType;
        dbd.RateIndex = bullet.RateIndex;
        try
        {
            if (clientSeat < m_PlayerBullets.Length)
                m_PlayerBullets[clientSeat].DestroyBulletList[bullet.ID] = dbd;
        }
        catch(System.Exception e)
        {
            ReportException.Instance.AddException("InnerRemoveBullet err:" + e.ToString());
        }
    }
    public CatchBulletData GetBullet(byte clientSeat, byte id)
    {
        PlayerBullets pb = m_PlayerBullets[clientSeat];
        CatchBulletData cbd = new CatchBulletData();
        if (pb.BulletList.TryGetValue(id, out cbd.BulletObj) == false)
        {
            DestroyBulletData dbd;
            if(pb.DestroyBulletList.TryGetValue(id, out dbd))
            {
                cbd.LauncherType = dbd.LauncherType;
                cbd.RateIndex = dbd.RateIndex;
            }
            else
            {
                cbd.LauncherType = 255;
                LogMgr.Log("不存在的子弹, Seat:" + clientSeat + ", id:" + id);
            }
        }
        return cbd;
    }
    public Object GetBulletPar(byte type)
    {
        return m_BulletParObj[type];
    }
    static float lastTime = 0;
    public void LaunchBullet(ushort bulletID, byte launcherType, byte rateIndex, short angle, float time, byte reboundCount, ushort lockfishid/*, byte collidCount = 0, float pauseTime = 0, float speedScaling = 0*/)
    {
        byte clientSeat, id;
        SceneRuntime.BuuletIDToSeat(bulletID, out clientSeat, out id);
        Bullet bullet = new Bullet();
        Vector3 startpos;
        Vector3 dir;
        float volume = 1;
        float curtime = Time.time;
        byte[] type = { 9, 9, 10, 11, 12 };
        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) == null)
            return;

        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) == SceneRuntime.PlayerMgr.MySelf)
        {
            volume = 1;
            lastTime = curtime;
            GlobalAudioMgr.Instance.PlayOrdianryMusic((Audio.OrdianryMusic)type[launcherType], false, false, volume);
        }
        else
        {
            volume = 0.30f;
            if ((lastTime + 0.1f) < curtime)
            {
                GlobalAudioMgr.Instance.PlayOrdianryMusic((Audio.OrdianryMusic)type[launcherType], false, false, volume);
            }
        }
        
        SceneRuntime.GetBulletPosAndDir(clientSeat, angle, out dir, out startpos);
        SceneRuntime.PlayerMgr.ChangeLauncherAngle(dir, clientSeat);        //改变炮台角度
        bullet.Init(clientSeat, id, launcherType, rateIndex, startpos, dir, reboundCount, lockfishid);
        bullet.InitAngle( Utility.ShortToFlaot(angle) );
        //bullet.SetCollidData(collidCount, pauseTime, speedScaling);
        PlayerBullets pb = m_PlayerBullets[clientSeat];
        if (pb != null)
        {
            Bullet findBullet;
            if (pb.BulletList.TryGetValue(id, out findBullet))
            {
                Debug.Log("相同的子弹ID:" + id);
                findBullet.Destroy();
                pb.BulletList.Remove(id);
            }
            if (time > 0)
                bullet.Update(time);
            pb.BulletList.Add(id, bullet);
        }
        
    }
    public void ClearAllBullet()
    {
        for (int i = 0; i < m_PlayerBullets.Length; ++i)
        {
            PlayerBullets pb = m_PlayerBullets[i];
            foreach (Bullet bullet in pb.BulletList.Values)
                bullet.Destroy();
            pb.BulletList.Clear();
        }
    }
    public void Update(float delta)
    {
        for(int i = 0; i < m_PlayerBullets.Length; ++i)
        {
            PlayerBullets pb = m_PlayerBullets[i];
            List<Bullet> bullets = new List<Bullet>(pb.BulletList.Values);
            for (int j = 0; j < bullets.Count; ++j )
            {
                Bullet bullet = bullets[j];
                if (!bullet.Update(delta))
                {
                    InnerRemoveBullet((byte)i, bullet);
                    bullet.Destroy();
                    pb.BulletList.Remove(bullet.ID);
                }
            }
        }
        //SendCheckPos();
    }
    public void PlayerLeave(byte seat)
    {
        PlayerBullets pb = m_PlayerBullets[seat];
        foreach(Bullet bullet in pb.BulletList.Values)
            bullet.Destroy();
        pb.BulletList.Clear();
    }
    public void Shutdown()
    {
        for (byte i = 0; i < m_PlayerBullets.Length; ++i)
        {
            PlayerLeave(i);
        }

        for (byte j = 0; j < m_BulletObj.Length; ++j)
        {
            ResManager.Instance.UnloadObject(m_BulletObj[j]);
        }
    }
    int SendInterval = 0;
    public void SendCheckPos()
    {
        if (++SendInterval < 60 * 6)
            return;
        SendInterval = 0;

        NetCmdCheckBulletPos pp = new NetCmdCheckBulletPos();
        pp.SetCmdType(NetCmdType.CMD_CHECK_BULLET_POS);
        List<CheckBulletPos> pl = new List<CheckBulletPos>();
        for (int i = 0; i < m_PlayerBullets.Length; ++i)
        {
            PlayerBullets pb = m_PlayerBullets[i];
            foreach(Bullet bullet in pb.BulletList.Values)
            {
                CheckBulletPos bb = new CheckBulletPos();
                bb.Pos = new NetCmdVector3(bullet.Position.x, bullet.Position.y, bullet.Position.z);
                bb.ID = bullet.ServerID;
                pl.Add(bb);
            }
        }
        pp.Count = (ushort)pl.Count;
        pp.Bullets = pl.ToArray();
        SceneRuntime.Send<NetCmdCheckBulletPos>(pp);
    }
    public bool HaveBullet(byte byseat)
    {
          PlayerBullets pb = m_PlayerBullets[byseat];
          return pb != null && pb.BulletList.Count != 0 || SceneRuntime.EffectMgr.HaveFlyGold(byseat);      
    }
}
