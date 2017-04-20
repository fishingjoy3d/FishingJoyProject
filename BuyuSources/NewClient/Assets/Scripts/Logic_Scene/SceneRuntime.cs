using UnityEngine;
using System.Collections.Generic;
public class JoinRoomData
{
    public byte Result;
    public byte RoomID;
    public byte Seat;
    public byte LauncherType;
    public byte BackgroundImage;
    public byte RateIndex;
    public int  Energy;
}
public class BulletStartPosData
{
    public Vector3  Dir;
    public Vector3  Pos;
    public Vector3  Center;
    public float    Length;
}
class SceneRuntime
{
    //场景相关的属性
    static public SceneRuntime  Instance;
    public bool                 SeatInversion;  //是否反转
    public SceneLogic           Scene;
    public byte[]               SeatMapping = new byte[] { 3, 2, 1, 0 };
    public byte[]               IndxMapping = new byte[] { 1, 1, 2, 3 };
    public byte                 m_BackgroundIndex;
    public float                DelayTime = 0.01f;//10毫秒的网络延迟
    public BulletStartPosData[] m_BulletPosData = new BulletStartPosData[ConstValue.PLAYER_MAX_NUM];
    public bool                 m_MouseValid = true;        //判断鼠标是否处于有效区
    public byte                 m_DelayCount = 1;           //按纽点击延时
    public Vector3[]            GlodPosMapping = new Vector3[ConstValue.PLAYER_MAX_NUM+2];
    static bool                 ms_HandleClickEvent = false;
    public static Vector3       LauncherScrStartPos1 = new Vector3();
    public static Vector3       LauncherScrStartPos2 = new Vector3();
    public static Vector3       LauncherViewStartPos1 = new Vector3();
    public static Vector3       LauncherViewStartPos2 = new Vector3();
    public static bool HandleClickEvent
    {
        get { return SceneRuntime.ms_HandleClickEvent; }
        set { SceneRuntime.ms_HandleClickEvent = value; }
    }

    //服务器位置到客户端位置转换
    public static float NetDelayTime
    {
        get
        {
            return Instance.DelayTime;
        }
    }
    public static short AngleInversion(short angle)
    {
        return Inversion ?(short) -angle : (short)angle;    
    }
    public static byte ServerToClientSeat(byte seat)
    {
        return Inversion ? Instance.SeatMapping[seat] : seat;
    }
    
    public static byte ClientToServerSeat(byte seat)
    {
        return Inversion ? Instance.SeatMapping[seat] : seat;
    }
    public static byte LauncherPrefabIndx(byte seat, bool bMyself = false)
    {
        return !bMyself ?  Instance.IndxMapping[seat] : (byte)0;
    }
    public static void CheckLauncherValid(byte serverLauncherType, out byte clientLauncherType, out bool valid)
    {
        valid = (serverLauncherType & 128) != 0;
        clientLauncherType = (byte)(serverLauncherType & 127);
    }
    public static void Send<T>(NetCmdBase ncb)
    {
        SceneLogic.Send<T>(ncb);
    }
    public static SceneLogic SceneLogic
    {
        get
        {
            if (Instance == null)
                return null;
            return Instance.Scene;
        }
    }
    public static int FishNum
    {
        get
        {
            if (Instance == null || FishMgr == null)
                return 0;
            else
                return SceneLogic.FishMgr.FishNum;
        }
    }
    public static bool Inversion
    {
        get
        {
            return Instance.SeatInversion;
        }
        set
        {
            Instance.SeatInversion = value;
        }
    }

    //获取炮台的金币数量(屏幕坐标)
    public static Vector2 GetLauncherGoldNumPos(byte clientSeat)
    {
        return Vector2.zero;
    }
    //获取炮台的金币图标(NGUI坐标)
    public static Vector3 GetLauncherGoldIconPos(byte clientSeat)
    {
        if (clientSeat >= Instance.GlodPosMapping.Length)
        {
            Debug.Log("clientSeat is Out of amount！");
            return Instance.GlodPosMapping[0];
        }
        if (clientSeat != PlayerMgr.MyClientSeat)
            return Instance.GlodPosMapping[clientSeat];
        else
            return Instance.GlodPosMapping[ConstValue.PLAYER_MAX_NUM + clientSeat];

    }
    public static void GetBulletPosAndDir(byte clientSeat, short angle, out  Vector3 dir, out Vector3 pos)
    {
        float degree = angle * ConstValue.InvShortMaxValue * 90;
        BulletStartPosData pd = Instance.m_BulletPosData[clientSeat];
        Quaternion q = Quaternion.Euler(0, 0, degree);
        Vector3 dir3 = q * pd.Dir;
        Vector3 pos3 = dir3 * pd.Length + pd.Center;
        dir.x = dir3.x;
        dir.y = dir3.y;
        dir.z = 0;
        pos.x = pos3.x;
        pos.y = pos3.y;
        pos.z = ConstValue.NEAR_Z + 0.1f;
    }
    //
    public static Vector3 WorldToNGUI(Vector3 pos)
    {
        pos = Camera.main.WorldToScreenPoint(pos);
        pos.z = 0;
        return SceneObjMgr.Instance.UICamera.ScreenToWorldPoint(pos);
    }
    public static Vector3 NGUIToWorld(Vector3 pos, float z = 0)
    {
        pos.z = 0;
        pos = SceneObjMgr.Instance.UICamera.WorldToScreenPoint(pos);              //NGUI坐标转到Screen标
        pos.z = ConstValue.NEAR_Z + z;                  //Z轴等与主像机近裁剪面
        return Camera.main.ScreenToWorldPoint(pos);
    }
    float ComputeAngle(float angle, byte seat)
    {
        bool bDown;
        if (seat > 1)
            bDown = Instance.SeatInversion;
        else
            bDown = !Instance.SeatInversion;
        if (!bDown)
            angle = 180 - angle;
        return angle;
    }
    public static void Shutdown()
    {
        Instance = null;
    }
    public static void BuuletIDToSeat(ushort bulletId, out byte clientSeat, out byte id)
    {
        id = (byte)bulletId;
        clientSeat = ServerToClientSeat((byte)(bulletId >> 8));
    }
    //返回炮台0和炮台1的起始位置
    public static Vector3 GetLauncherScrStartPos(byte clientIdx)
    {
        if (clientIdx == 0)
            return LauncherScrStartPos1;
        else
            return LauncherScrStartPos2;
    }
    public static Vector3 GetLauncherViewStartPos(byte clientIdx)
    {
        if (clientIdx == 0)
            return LauncherViewStartPos1;
        else
            return LauncherViewStartPos2;
    }
    public static void Init(SceneLogic logic)
    {
        Instance = new SceneRuntime();
        Instance.Scene = logic;

        float   LAUNCHER_X = 15.5f;
        float   LAUNCHER_X_RIGHT = 10.5f;
        float   LAUNCHER_Y = 11.5f;

        //子弹的位置
        //		3	2
        //		0,	1
        float height = ConstValue.NEAR_HALF_HEIGHT;
        BulletStartPosData pd = new BulletStartPosData();
        pd.Center = new Vector3(-LAUNCHER_X, -height, 0);
        pd.Pos = new Vector3(-LAUNCHER_X, -LAUNCHER_Y, 0);
        pd.Dir = pd.Pos - pd.Center;
        pd.Length = pd.Dir.magnitude;
        pd.Dir /= pd.Length;
        Instance.m_BulletPosData[0] = pd;

        pd = new BulletStartPosData();
        pd.Center = new Vector3(LAUNCHER_X_RIGHT, -height, 0);
        pd.Pos = new Vector3(LAUNCHER_X_RIGHT, -LAUNCHER_Y, 0);
        pd.Dir = pd.Pos - pd.Center;
        pd.Length = pd.Dir.magnitude;
        pd.Dir /= pd.Length;
        Instance.m_BulletPosData[1] = pd;

        pd = new BulletStartPosData();
        pd.Center = new Vector3(LAUNCHER_X_RIGHT, height, 0);
        pd.Pos = new Vector3(LAUNCHER_X_RIGHT, LAUNCHER_Y, 0);
        pd.Dir = pd.Pos - pd.Center;
        pd.Length = pd.Dir.magnitude;
        pd.Dir /= pd.Length;
        Instance.m_BulletPosData[2] = pd;

        pd = new BulletStartPosData();
        pd.Center = new Vector3(-LAUNCHER_X, height, 0);
        pd.Pos = new Vector3(-LAUNCHER_X, LAUNCHER_Y, 0);
        pd.Dir = pd.Pos - pd.Center;
        pd.Length = pd.Dir.magnitude;
        pd.Dir /= pd.Length;
        Instance.m_BulletPosData[3] = pd;

        //炮台金币的位置
        Instance.GlodPosMapping[0] = new Vector3(-0.54270833f, -0.9461806f, 0.0f);
        Instance.GlodPosMapping[1] = new Vector3(0.938773155f, -0.9461806f, 0.0f);
        Instance.GlodPosMapping[2] = new Vector3(0.917939842f, 0.9458333f, 0.0f);
        Instance.GlodPosMapping[3] = new Vector3(-0.563541651f, 0.9458333f, 0.0f);
        Instance.GlodPosMapping[4] = new Vector3(-0.580940962f, -0.8208333f, 0.0f);
        Instance.GlodPosMapping[5] = new Vector3(0.902314842f, -0.8208333f, 0.0f);


        Vector3 dir1, dir2;
        SceneRuntime.GetBulletPosAndDir(0, 0, out dir1, out LauncherScrStartPos1);
        SceneRuntime.GetBulletPosAndDir(1, 0, out dir2, out LauncherScrStartPos2);
        LauncherScrStartPos1 = Camera.main.WorldToScreenPoint(LauncherScrStartPos1);
        LauncherScrStartPos2 = Camera.main.WorldToScreenPoint(LauncherScrStartPos2);
        LauncherScrStartPos1.z = 0;
        LauncherScrStartPos2.z = 0;

        LauncherViewStartPos1 = Camera.main.ScreenToViewportPoint(LauncherScrStartPos1);
        LauncherViewStartPos2 = Camera.main.ScreenToViewportPoint(LauncherScrStartPos2);
        LauncherViewStartPos1.z = 0;
        LauncherViewStartPos2.z = 0;
    }
    public static SceneBulletMgr BulletMgr
    {
        get
        {
            return SceneLogic.BulletMgr;
        }
    }
    public static SceneLogicUI LogicUI
    {
        get
        {
            return SceneLogic.LogicUI;
        }
    }
    public static SceneSkillMgr SkillMgr
    {
        get
        {
            return SceneLogic.SkillMgr;
        }
    }
    public static SceneFishMgr FishMgr
    {
        get
        {
            return SceneLogic.FishMgr;
        }
    }
    public static ScenePlayerMgr PlayerMgr
    {
        get
        {
            return SceneLogic.PlayerMgr;
        }
    }
    public static SceneEffectMgr EffectMgr
    {
        get
        {
            return SceneLogic.EffectMgr;
        }
    }
    public static LauncherEffectMgr LauncherEftMgr
    {
        get 
        {
            return SceneLogic.LauncherEftMgr;
        }
    }
    public static byte BackgroundIndex
    {
        get
        {
            return Instance.m_BackgroundIndex;
        }
        set
        {
            Instance.m_BackgroundIndex = value;
        }
    }
    public static byte MyClientSeat
    {
        get
        {
            return Instance.Scene.PlayerMgr.MyClientSeat;
        }
    }

    public static int GetFishGold(byte fishType, byte rateIndex)
    {
        return BulletSetting.BulletRate[rateIndex] * FishSetting.FishDataList[fishType].Gold;
    }
    public static void ComputeGoldNum(CatchedData cd)
    {
        cd.GoldNum = 0;
        ushort rate = BulletSetting.BulletRate[cd.RateIndex];
        for (int i = 0; i < cd.FishList.Count; ++i)
        {
            if (cd.FishList[i].FishType >= FishSetting.FishDataList.Length)
                Debug.Log("错误的鱼类型:" + cd.FishList[i].FishType);
            if (cd.CatchType == (byte)CatchedType.CATCHED_SKILL)
            {
                cd.GoldNum +=  FishSetting.FishDataList[cd.FishList[i].FishType].Gold * SkillSetting.SkillDataList[cd.SubType].multiple;
            }
            else
                cd.GoldNum += rate * FishSetting.FishDataList[cd.FishList[i].FishType].Gold;
        }
    }
    public static float ComputeEffectScaling(Fish fish, float scaling)
    {
        return fish.Scaling * Mathf.Max(1.0f, (fish.Position.z / 1000)) * scaling;
    }
    public static float ComputeZScaling(Fish fish, float scaling)
    {
        return Mathf.Max(1.0f, (fish.Position.z / 1000)) * scaling;
    }
    public static void RefreshScene()
    {
        if(Instance != null && Instance.Scene != null)
            Instance.Scene.RefreshScene(true);
    }
}
