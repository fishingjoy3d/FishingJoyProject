using UnityEngine;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Collections;
public class BulletTime
{
    public uint CurTime;
    public uint Count;
    public uint AvgTime;
    public uint MaxTime;
    public uint AllTime;
    public void AddTime(uint time)
    {
        CurTime = time;
        ++Count;
        if (time > MaxTime)
            MaxTime = time;
        AllTime += time;
        AvgTime = AllTime / Count;
    }
}
public class SceneMain : MonoBehaviour {

    public BulletTime bulletTime = new BulletTime();
    public static bool IsUserOldServer = false;
    public static SceneMain  Instance;
    ushort  m_FrameCount   = 0;
    ushort  m_FPS = 0;
    float   m_FrameTime   = 0;
    uint    m_GCTick;
    uint    m_FrameTick;
    float   m_Delta;
    uint    m_Ping;

    public uint Ping
    {
        get { return m_Ping; }
        set { m_Ping = value; }
    }

    public float Delta
    {
        get { return m_Delta; }
        set { m_Delta = value; }
    }
#if UNITY_EDITOR
    public static volatile bool Exited;
#endif
    void OnPress(GameObject go, bool state)
    {
        Debug.Log(go.name);
        if (state)
        {
#if UNITY_STANDALONE_WIN || UNITY_EDITOR
            SceneRuntime.HandleClickEvent = (go != SceneObjMgr.Instance.UIRoot) || DllTest.WaitExecuteResult;
#else
            SceneRuntime.HandleClickEvent = (go != SceneObjMgr.Instance.UIRoot);
#endif
        }
        else
            SceneRuntime.HandleClickEvent = false;
    }
    void GlobalInit()
    {
        Screen.sleepTimeout = SleepTimeout.NeverSleep;
#if UNITY_STANDALONE_WIN
        DllTest.GetProcessWnd();
#endif
        try
        {
            //全局的初始化
            LogMgr.GlobalInit();
            Utility.GlobalInit();
            RuntimeInfo.GlobalInit();
            SDKMgr.Instance.GlobalPreInit();

            Resolution.GlobalInit();
            StringTable.GlobalInit();
            SceneObjMgr.Instance.GlobalInit();
            ResManager.Instance.GlobalInit();   //Res需要在SDKMgr的前面。  
            ReportException.Instance.GlobalInit();
            SDKMgr.Instance.GlobalInit();
            ServerSetting.GlobalInit();

            GlobalUpdate.GlobalInit();
            NetCmdMapping.GlobalInit();
            LogicManager.Instance.GlobalInit();
            UICamera.onPress = OnPress;
            Utility.GlobalInit();
        }
        catch (System.Exception e)
        {
            ReportException.Instance.AddException(e.ToString());
        }
    }
	void Start () 
    {
#if UNITY_EDITOR
        Exited = false;
#endif
        GameObject.DontDestroyOnLoad(this);
        Instance = this;
        GlobalInit();
        m_FrameTick = m_GCTick = Utility.GetTickCount();
	}
    public string LogStr;
	void OnGUI()
    {
        try
        {
            
            LogicManager.Instance.OnGUI(m_Delta);
            if (ServerSetting.IS_TEST == false)
               return;
            int width = 110;
            int height = 60;
            GUI.skin.button.fontSize = 20;
            
#if UNITY_STANDALONE_WIN || UNITY_STANDALONE_OSX
           
            if (GUI.Button(new Rect(Screen.width - width * 1 - 10, height + 10 + 20, width, height), "分辨率"))
            {
                Resolution.SwitchFullscreen();
            }
            
            //if (GUI.Button(new Rect(Screen.width - width * 1 - 10, height * 3 + 15 + 20, width, height), "子弹时间"))
            //{
            //    m_bShowBulletTime = !m_bShowBulletTime;
            //}
            //if (GUI.Button(new Rect(Screen.width - width * 1 - 10, height * 4 + 15 + 20, width, height), "清除LOG"))
            //{
            //    LogStr = "";
            //}
#else
        //if (GUI.Button(new Rect(Screen.width - width * 1 - 10, 20 + 5, width, height), "子弹时间"))
        //{
        //    m_bShowBulletTime = !m_bShowBulletTime;
        //}
        //if (GUI.Button(new Rect(Screen.width - width * 1 - 10, height + 10 + 20, width, height), "清除LOG"))
        //{
        //    LogStr = "";
        //}
#endif
            LogStr += LogMgr.Update();
            string str = string.Format("宽:{0}, 高:{1}, 帧:{2}, 鱼:{3}",
                Screen.width,
                Screen.height,
                m_FPS,
                SceneRuntime.FishNum
                );

            if (LogStr.Length > 0)
                str += " : " + LogStr;
            SceneObjMgr.Instance.ShowLog(str);
        }
        catch(System.Exception e)
        {
            //异常提交
            ReportException.Instance.AddException(e.ToString());
        }
    }
    //焦点相关
    bool m_bPause = false;
    bool m_bFocus = true;
    uint m_LostTick;
    void CheckFocusState()
    {
#if UNITY_STANDALONE_WIN || UNITY_STANDALONE_OSX
        return;
#endif
        if(m_bPause && !m_bFocus)
        {
            //失去
            m_LostTick = Utility.GetTickCount();
        }
        else if(!m_bPause && m_bFocus)
        {
            uint seconds = Utility.GetTickCount() - m_LostTick;
            //if(NetServices.Instance.TcpClient != null)
            //     LogMgr.Log("回来了, ms:" + seconds +", count:" + NetServices.Instance.TcpClient.SendCount);
        }
    }
    void OnApplicationFocus()
    {
#if UNITY_STANDALONE_WIN || UNITY_EDITOR
        DllTest.SetExecuteFocus();
#endif
        if(m_bPause && m_bFocus)
            m_bFocus = false;   //失去
        else if(m_bPause)
            m_bFocus = true;    //获得
        else 
            return;
        CheckFocusState();
    }
    void OnApplicationPause()
    {
        //LogMgr.Log("Pause.");
        if(m_bFocus && !m_bPause)
            m_bPause = true;//失去
        else if(m_bFocus)
            m_bPause = false;//获得
        else 
            return;
        CheckFocusState();
    }
    void OnApplicationQuit()
    {
        Screen.sleepTimeout = SleepTimeout.SystemSetting;
#if UNITY_EDITOR
        NetServices.Instance.Disconnect();
#endif
        //NetServices.Instance.Disconnect();
    }
    void ComputeFPS(float delta)
    {
        ++m_FrameCount;
        m_FrameTime += delta;
        if (m_FrameTime >= 1.0f)
        {
            m_FPS           = m_FrameCount;
            m_FrameCount    = 0;
            m_FrameTime     = 0;
        }
    }
    void CehckEscape()
    {
        if(Input.GetKeyDown(KeyCode.Escape))
        {
            if(LogicManager.Instance.ActiveLogicType == LogicType.LOGIC_SCENE)
                SDKMgr.Instance.ExitCallback(true);
            else
                SDKMgr.Instance.SDK.Exit();
        }
    }
    void GCCollect()
    {
        return;
        //30秒进行一次GC释放
        if (Utility.GetTickCount() - m_GCTick > 30000)
        {
            m_GCTick = Utility.GetTickCount();
            System.GC.Collect(5, System.GCCollectionMode.Optimized);
        }
    }
    long m_LastTick;
	void Update () 
    {
#if UNITY_STANDALONE_WIN || UNITY_EDITOR
        DllTest.CheckExecuteResult();
#endif
#if !UNITY_EDITOR
        try
        {
#endif
            CehckEscape();
            long tick = System.DateTime.Now.Ticks;
            System.TimeSpan ts = new System.TimeSpan(tick - m_LastTick);
            m_LastTick = tick;
            uint curTick = Utility.GetTickCount();
            m_Delta = Utility.TickToFloat(curTick - m_FrameTick);
            if ((ts.TotalSeconds > 5 || m_Delta >= 5.0f) && LogicManager.Instance.IsPlaying)
            {
                //LogMgr.Log("Refresh,delta:" + m_Delta + ",tick:" + ts.TotalSeconds+",Socket:" + NetServices.Instance.IsSocketConnected);
                SceneRuntime.RefreshScene();
            }
            m_FrameTick = curTick;
        
                if (BlockLogic.Instance.Update(m_Delta))
                {
                    //如果当前没有阻塞
                    LogicManager.Instance.Update(m_Delta);
                }
        
            if (ServerSetting.IS_TEST)
            {
                WndManager.Instance.Check();
                ComputeFPS(m_Delta);
            }
#if !UNITY_EDITOR
        }
        catch (System.Exception e)
        {
            //异常提交
            ReportException.Instance.AddException(e.ToString());
        }
#endif
    }
    public void Shutdown()
    {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit();
#endif
    }
    public void StartInnerCoroutine(IEnumerator e)
    {
        StartCoroutine(e);
    }

    //外部消息
    public void SetPackageName(string name)
    {
        //LogMgr.Log("PackageName:" + name);
        SDKMgr.SetPackageName(name);
    }
    public void OnAppStorePayCallback(string str)
    {
        //服务器验证数据。
        PlayerRole.Instance.RoleRecharge.OnSendIOSRecharge(str);
    }
    public void OnPayCallback(string str)
    {
        SDKMgr.Instance.PayCallback(str == "success");
    }
    public void OnPhotoErrMsg(string err)
    {
        LogMgr.Log("OhPhotoErrMsg:" + err);
    }
    public void OnPhotoMsg(string path)
    {
        if(path.Length > 0)
            HeaderManager.Instance.UploadHeader(RuntimeInfo.GetLocalDirectory() + "/images/" + path);
    }
    public void OnHandleMsg()
    {
        BlockLogic.Instance.HandleMsg();
    }
    bool m_bToFriend = true;
    public void CaptureAndShare(bool bToFriend, UIWidget uiTexture)
    {
        m_bToFriend = bToFriend;
        StartInnerCoroutine(CaptureScreenAndShare(uiTexture));
    }
    public IEnumerator CaptureScreenAndShare(object uiTexture)
    {
        bool toFriend = m_bToFriend;
        Rect rect;
        if (uiTexture is UIPanel)
            rect = Utility.GetUIScreenRect((UIPanel)uiTexture);
        else
            rect = Utility.GetUIScreenRect((UIWidget)uiTexture);
        yield return new WaitForEndOfFrame();
        Texture2D cutImage = new Texture2D((int)rect.width, (int)rect.height, TextureFormat.RGB24, false);
        cutImage.ReadPixels(rect, 0, 0, false);
        cutImage.Apply();

        const int MAX_WIDTH = 1920;
        const int MAX_HEIGHT = (int)(MAX_WIDTH / 1.7777777f);
        const int JPG_QUALITY = 100;
        const float Scaling = 0.8f;
        const int MAX_SIZE = 1024 * 1024 * 1;
        const int MIN_SIZE = 1024 * 32;
        if (cutImage.width > MAX_WIDTH || cutImage.height > MAX_HEIGHT)
        {
            float x = (float)MAX_WIDTH / cutImage.width;
            float y = (float)MAX_HEIGHT / cutImage.height;
            cutImage = Utility.ScaleTexture(cutImage, Mathf.Max(x, y));
        }
        byte[] byt = cutImage.EncodeToJPG(JPG_QUALITY);

        //保存原图
        while (byt.Length > MAX_SIZE) //最大1M
        {
            //图片太大，继续缩放
            cutImage = Utility.ScaleTexture(cutImage, Scaling);
            byt = cutImage.EncodeToJPG(JPG_QUALITY);
        }
        string imgstr = "cutimage.jpg";
        string path = RuntimeInfo.GetLocalPath(imgstr);
        System.IO.File.WriteAllBytes(path, byt);

        /*
        //保存缩略图
        while (byt.Length > MIN_SIZE) //最大32 k
        {
            //图片太大，继续缩放
            cutImage = Utility.ScaleTexture(cutImage, Scaling);
            byt = cutImage.EncodeToJPG(JPG_QUALITY);
        }
        string path = RuntimeInfo.GetLocalPath(imgstr);
        System.IO.File.WriteAllBytes(path, byt);
        */
        NativeInterface.BeginShare(toFriend, ServerSetting.ShareTxt, path);
        yield break;
    }
    public void OnThirdLogin(string openid_access_token)
    {
        //用户第三方登录成功回调。
        char[] splitchar = new char[1];
        splitchar[0] = ',';
        string[] values = openid_access_token.Split(splitchar);

        //获取openid和access_token;
        string openid = values[0];
        string access_token = values[1];
        LogMgr.Log("<第三方登录回调:" + openid+">");
        //将OpenID  发送到服务器端去
        LogonRuntime.LogonLogic.OnHandleQQOrWeiXinLogonResult(openid, access_token);
    }
    public void OnShare(string str)
    {
        GlobalHallUIMgr.Instance.GameShare.ShutDown();
        //分享回调
        //str分享平台:SINA, QZONE，QQ，WEIXIN，WEIXIN_CIRCLE
        PlayerRole.Instance.RoleInfo.ChangeRoleShareSatate(true);
    }

    //===================================================
    //测试鱼路径
    const float delta = 0.022222f;
    void WriteToList(Fish f, System.IO.BinaryWriter bw)
    {
        List<float> time = new List<float>();
        List<Vector3> pos = new List<Vector3>();
        List<Quaternion> rot = new List<Quaternion>();

        int[] randList = new int[] { 131, 343, 645, 1333, 2003, 3123, 1623, 3500, 15, 33, 66, 88, 123, 654, 346, 1233, 1555, 1777, 1999, 2333, 2545 };
        float[] duration = new float[] { 0.15f, 1.5f, 0.1f };
        while (f.Time < 1.0f)
        {
            for (int i = 0; i < randList.Length; ++i)
            {
                if(time.Count == randList[i])
                {
                    f.Controller.TimeController.AddSkillTimeScaling(0, duration, FISH_DELAY_TYPE.DELAY_SKILL); 
                }
            }
            f.TestUpdate(delta);
            time.Add(f.Time);
            pos.Add(f.Position);
            rot.Add(f.Rotation);
        }
        Debug.Log("sample count:" + pos.Count);
        bw.Write(pos.Count);
        for (int j = 0; j < pos.Count; ++j)
        {
            bw.Write(time[j]);
            bw.Write(pos[j].x);
            bw.Write(pos[j].y);
            bw.Write(pos[j].z);
            bw.Write(rot[j].x);
            bw.Write(rot[j].y);
            bw.Write(rot[j].z);
            bw.Write(rot[j].w);
        }
    }
    System.Collections.IEnumerator CreateFishGroupData(object obj)
    {
        const float TIME = 0.0f;
        System.IO.File.Delete("e:/fishGroup.dat");
        System.IO.FileStream fs = new System.IO.FileStream("e:/fishGroup.dat", System.IO.FileMode.CreateNew, System.IO.FileAccess.Write);
        System.IO.BinaryWriter bw = new System.IO.BinaryWriter(fs);
        int groupCount = FishResManager.Instance.GetGroupCount();
       // groupCount = 4;
        bw.Write(delta);
        bw.Write(groupCount);
        
        for (int i = 0; i < groupCount; ++i)
        {
            GroupDataList gdl = FishResManager.Instance.GetFishGroup((ushort)i);
            bw.Write(gdl.PathGroupData != null);
            if (gdl.PathGroupData != null)
            {
                FishPathGroupData pathgroup = gdl.PathGroupData;
                PathLinearInterpolator[] interpList = PathManager.Instance.GetPathGroup(pathgroup.PathGroupIndex, false);
                bw.Write(interpList.Length);
                foreach (PathLinearInterpolator interp in interpList)
                {
                    Fish fish = new Fish();
                    fish.Init(0, pathgroup.FishIndex, pathgroup.FishScaling, TIME, pathgroup.ActionSpeed, pathgroup.ActionUnite, pathgroup.Speed, interp);
                    WriteToList(fish, bw);
                }
            }
            else
            {
                bw.Write(gdl.PathList.Length);
                for (int t = 0; t < gdl.PathList.Length; ++t)
                {
                    PathLinearInterpolator pi = PathManager.Instance.GetPath(gdl.PathList[t]);

                    bw.Write(gdl.GroupDataArray.Length);
                    for (int j = 0; j < gdl.GroupDataArray.Length; ++j)
                    {
                        GroupData gd = gdl.GroupDataArray[j];
                        bw.Write(gd.FishNum);
                        for (int n = 0; n < gd.FishNum; ++n)
                        {
                            float time = Utility.GetPathTimeByDist(gdl.FrontPosition.x, gd.PosList[n].x, pi) + TIME;
                            Fish fish = new Fish();
                            fish.Init(0, gd.FishIndex, gd.FishScaling, time, gd.ActionSpeed, gd.ActionUnite, gd.SpeedScaling, pi);
                            fish.SetOffset(new Vector3(0, gd.PosList[n].y, gd.PosList[n].z));
                            WriteToList(fish, bw);
                        }
                    }
                }
            }
            yield return new WaitForEndOfFrame();
            Debug.Log("groupIndex:" + i + "/" + groupCount);
        }
        Debug.Log("完成.");
        bw.Close();
        fs.Close();

    }
    System.Collections.IEnumerator CreateFishPathData(object obj)
    {
        System.IO.File.Delete("e:/fish.dat");
        System.IO.FileStream fs = new System.IO.FileStream("e:/fish.dat", System.IO.FileMode.CreateNew, System.IO.FileAccess.Write);
        System.IO.BinaryWriter bw = new System.IO.BinaryWriter(fs);
        bw.Write(delta);
        bw.Write(PathManager.Instance.PathCount);
        bw.Write(PathManager.Instance.PathGroupCount);
        Debug.Log("PathCount:" + PathManager.Instance.PathCount + ", GroupCount:" + PathManager.Instance.PathGroupCount);

        for (int i = 0; i < PathManager.Instance.PathCount; ++i)
        {
            PathLinearInterpolator pi = PathManager.Instance.GetPath(i);
            Fish f = new Fish();
            f.Init(0, 0, 1.0f, 0.0f, 1.0f, false, 1.5f, pi);
            WriteToList(f, bw);
            Debug.Log("Path:" + i);
            yield return new WaitForEndOfFrame();
        }

        for (int i = 0; i < PathManager.Instance.PathGroupCount; ++i)
        {
            PathLinearInterpolator[] pi = PathManager.Instance.GetPathGroup(i);
            bw.Write(pi.Length);
            for (int j = 0; j < pi.Length; ++j)
            {
                Fish f = new Fish();
                f.Init(0, 0, 1.0f, 0.0f, 1.0f, false, 1.5f, pi[j]);
                WriteToList(f, bw);
            }
           
            Debug.Log("Group:" + i);
            yield return new WaitForEndOfFrame();
        }
        Debug.Log("完成.");
        bw.Close();
        fs.Close();
    }
    Vector3 ScreenToCenterPoint(Vector3 pos)
    {
        Vector3 ScrSize = new Vector3(Screen.width >> 1, Screen.height >> 1, 0);
        Vector3 newPos = new Vector3();
        newPos.x = (pos.x - ScrSize.x) / ScrSize.x * ( 1024 >> 1);
        newPos.y = (pos.y - ScrSize.y) / ScrSize.y * (576 >> 1);
        newPos.z = 0;
        return newPos;
    }
    public void ProcessLightingFish(GameObject go1, GameObject go2)
    {
        Object obj = ResManager.Instance.LoadObject("Ef_thunderFishMain", "SceneRes/Prefab/Effect/ThunderFish/", ResType.SceneRes);
        Object obj2 = ResManager.Instance.LoadObject("Ef_thunderFishLight", "SceneRes/Prefab/Effect/ThunderFish/", ResType.SceneRes);

        Vector3 p1 = Camera.main.WorldToScreenPoint(go1.transform.position);
        Vector3 p2 = Camera.main.WorldToScreenPoint(go2.transform.position);
        p1.z = 0;
        p2.z = 0;
        Vector3 pv1 = ScreenToCenterPoint(p1);
        Vector3 pv2 = ScreenToCenterPoint(p2);
        
        Vector3 dir = (pv2 - pv1);
        float dist = dir.magnitude;
        dir /= dist;
        float dot = Vector3.Dot(Vector3.right, dir);
        float angle = Mathf.Acos(Mathf.Clamp(dot, -1.0f, 1.0f)) * Mathf.Rad2Deg;
        if (dir.y < 0)
            angle = -angle;

        GameObject gov1 = (GameObject)GameObject.Instantiate(obj);
        GameObject gov2 = (GameObject)GameObject.Instantiate(obj2);
        GameObject gov3 = (GameObject)GameObject.Instantiate(obj2);
        GlobalEffectData efc = new GlobalEffectData(gov1, 0, 2.0f);
        GlobalEffectMgr.Instance.AddEffect(efc);
        efc = new GlobalEffectData(gov2, 0, 2.0f);
        GlobalEffectMgr.Instance.AddEffect(efc);
        efc = new GlobalEffectData(gov3, 0, 2.0f);
        GlobalEffectMgr.Instance.AddEffect(efc);

        gov1.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        gov2.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        gov3.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        Vector3 ss = gov1.transform.localScale;
        ss.x = dist;
        gov2.transform.localPosition = pv1;
        gov3.transform.localPosition = pv2;
        gov1.transform.localPosition = pv1;
        gov1.transform.localScale = ss;
        gov1.transform.localRotation = Quaternion.AngleAxis(angle, Vector3.forward);

        FishOptAction fod = new FishOptAction(FishClipType.CLIP_SIWANG, 1.0f);
    }
}
