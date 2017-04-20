using UnityEngine;
using System.Collections.Generic;
public class Bullet:IGlobalEffect
{
    const float     LOCK_FISH_RADIUS = 35.0f;
    static float    LIFE_TIME = 1.5f;
    byte            m_LauncherType;
    byte            m_ClientSeat;
    byte            m_RateIndex;
    float           m_Time;
    byte            m_ID;
    byte            m_ReboundCount;
    float           m_Angle;
    Vector3         m_Pos;
    Vector3         m_Dir;
    GameObject      m_Object;
    Transform       m_Trans;
    GameObject      m_BulletParEft;
    Transform       m_ParEftTrans;
    int             m_GoldNum = 0;
    ushort          m_LockFishID = 0;
    Vector3         m_OrgDir;
    float           m_OrgAngle;
    public Transform ParEftTrans
    {
        get { return m_ParEftTrans; }
    }

    public void Init(byte clientSeat, byte id, byte launcherType, byte rateIndex, Vector3 startPos, Vector3 dir, byte reboundCount, ushort lockFishID)
    {
        m_Object        = (GameObject)GameObject.Instantiate(SceneRuntime.BulletMgr.BulletObj[launcherType]);
        m_Trans         = m_Object.transform;
        m_LockFishID    = lockFishID;
        m_ReboundCount  = reboundCount;
        m_ID            = id;
        m_ClientSeat    = clientSeat;
        m_RateIndex     = rateIndex;
        m_LauncherType  = launcherType;
        m_Pos           = startPos;
        m_Dir           = dir;
        Object obj      = SceneRuntime.BulletMgr.GetBulletPar(launcherType);
        m_BulletParEft  = (GameObject)GameObject.Instantiate(obj, startPos, Quaternion.identity);
        m_ParEftTrans   = m_BulletParEft.transform;
        m_ReboundCount  = reboundCount;
        m_LockFishID    = lockFishID;
        m_OrgDir        = m_Dir;//恢复原始的方向
    }
    public byte ID
    {
        get { return m_ID; }
    }
    //public uint ComboID
    //{
    //    get { return m_ComboID; }
    //    set { m_ComboID = value; }
    //}
    public ushort ServerID
    {
        get
        {
            int seat = SceneRuntime.ClientToServerSeat(m_ClientSeat) << 8;
            return (ushort)(seat | m_ID);
        }
    }
    public void InitAngle(float angle)
    {
        if (m_Object != null)
        {
            m_OrgAngle = angle;
            if (ClientSeat > 1)
                angle = 180 + angle;
            m_Angle = angle;
            m_Trans.localEulerAngles = new Vector3(0, 0, angle);
        }
    }
    public GameObject Object
    {
        get { return m_Object; }
        set { m_Object = value; }
    }
    public Vector3 Position
    {
        get
        {
            return m_Pos;
        }
    }
    public byte RateIndex
    {
        get
        {
            return m_RateIndex;
        }
    }
    public float Time
    {
        get
        {
            return m_Time;
        }
    }
    public byte LauncherType
    {
        get
        {
            return m_LauncherType;
        }
    }
    public int GoldNum
    {
        get
        {
            return m_GoldNum;
        }
        set
        {
            m_GoldNum = value;
        }
    }
    public byte ClientSeat
    {
        get
        {
            return m_ClientSeat;
        }
    }
    public void ShowGoldNum()
    {
        if (SceneRuntime.PlayerMgr.MyClientSeat == m_ClientSeat && m_GoldNum > 0)
            SceneRuntime.EffectMgr.ShowGetGoldNum(m_GoldNum, m_ClientSeat);
    }
    public void Destroy()
    {
        ShowGoldNum();
        if (m_Object != null)
        {
            GameObject.Destroy(m_Object);
        }
        GlobalEffectData efc = new GlobalEffectData(m_BulletParEft, 0, 2.0f);
        GlobalEffectMgr.Instance.AddEffect(efc);
    }
    void CheckBoundary()
    {
        //裁剪掉屏幕外的子弹
        if (m_ReboundCount == 0)
            return;
        if (m_Pos.x > Utility.NearRightBottomPoint.x || m_Pos.x < Utility.NearLeftTopPoint.x ||
            m_Pos.y > Utility.NearLeftTopPoint.y || m_Pos.y < Utility.NearRightBottomPoint.y)
        {
            Vector3 dir = m_Dir;
            if (m_Pos.x > Utility.NearRightBottomPoint.x || m_Pos.x < Utility.NearLeftTopPoint.x)
            {
                if (m_Pos.x > Utility.NearRightBottomPoint.x)
                    m_Pos.x = Utility.NearRightBottomPoint.x;
                else
                    m_Pos.x = Utility.NearLeftTopPoint.x;
                m_Dir.x = -m_Dir.x;
                m_Angle = -m_Angle;
                m_Trans.localEulerAngles = new Vector3(0, 0, m_Angle);
            }
            if (m_Pos.y > Utility.NearLeftTopPoint.y || m_Pos.y < Utility.NearRightBottomPoint.y)
            {
                if (m_Pos.y > Utility.NearLeftTopPoint.y)
                    m_Pos.y = Utility.NearLeftTopPoint.y;
                else
                    m_Pos.y = Utility.NearRightBottomPoint.y;
                m_Dir.y = -m_Dir.y;
                m_Angle = 180 - m_Angle;
                m_Trans.localEulerAngles = new Vector3(0, 0, m_Angle);
            }
            m_Time = 0;
            --m_ReboundCount;
        }
    }
    public ushort LockedFishID
    {
        get
        {
            return m_LockFishID;
        }
    }
    bool UpdateLockFish()
    {
        Fish fish = SceneRuntime.FishMgr.FindFishByID(m_LockFishID);
        if (fish == null || fish.IsDelay || fish.Catched)
            return false;
        Vector3 bulletScrPos = Camera.main.WorldToScreenPoint(m_Pos);
        Vector3 dir = fish.ScreenPos - bulletScrPos;
        dir.z = 0;
        float len = dir.magnitude;
        if (len > LOCK_FISH_RADIUS)
        {
            m_Dir = dir / len;
            Vector2 UpDir = new Vector2(0, ClientSeat > 1 ? -1 : 1);
            float dot = Vector2.Dot(UpDir, m_Dir);
            m_Angle = Mathf.Acos(Mathf.Clamp(dot, 0.0f, 1.0f)) * Mathf.Rad2Deg;
            if (m_Dir.x >= 0)
                m_Angle = -m_Angle;
            if (ClientSeat > 1)
                m_Angle = 180 - m_Angle;
            m_Trans.localEulerAngles = new Vector3(0, 0, m_Angle);
        }
        return true;
    }
    public void ClearLockFishID()
    {
        m_Dir = m_OrgDir;
        InitAngle(m_OrgAngle);
        m_LockFishID = 0;
    }
    public bool Update(float delta)
    {
        if(m_LockFishID != 0)
        {
            if (!UpdateLockFish())
            {
                ClearLockFishID();
            }
        }
        m_Time += delta;
        if (m_Time >= LIFE_TIME)//1.5f保证所有子弹出屏幕外
            return false;
        m_Pos += m_Dir * delta * LauncherSetting.LauncherDataList[m_LauncherType].Speed;

        m_Trans.position = m_Pos;
        m_ParEftTrans.position = m_Pos;
        CheckBoundary();
        return true;
    }
}
