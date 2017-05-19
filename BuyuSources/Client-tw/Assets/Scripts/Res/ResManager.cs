using UnityEngine;
using System.Collections.Generic;
using System.IO;
using System.Collections;

//不要改动枚举变量名称，与Version.xml中一致
public enum ResType
{
    GlobalRes,         //全局配置相关资源           \
    FishRes,           //鱼资源                    -- 全局加载      
    PathRes,           //路径资源                   /
    
    LogonRes,          //登陆资源
    HallRes,           //大厅资源
    SceneRes,          //场景资源

    ExtraRes,          //附加资源
    GameRes,           //游戏资源
    MAX
}
public enum ResLocation
{
    StreamingAsset,
    Persistent,
    Resources,
    MAX,
}
public class ResObj:Object
{
    public byte     RefCount;
}
public class ResManager:Singleton<ResManager>
{
    //开发选项，是否使用Editor/Resources资源，“是”不进行更新。
#if UNITY_EDITOR
    public static bool ENABLE_RESOURCES = CheckEnableRes();
#else
    public static bool ENABLE_RESOURCES = false;
#endif
    public const ushort RES_NUM = (int)ResType.MAX;
    VersionManager m_VerMgr = new VersionManager();
    Dictionary<int, Object> m_LoadObjList = new Dictionary<int, Object>();
    AssetBundle[] m_AssetList = new AssetBundle[RES_NUM];
    bool[] m_AssetManager = new bool[RES_NUM];
    bool m_bLoadedRes;
    public static bool CheckEnableRes()
    {
        return System.IO.Directory.Exists(Application.dataPath + "/Resources/FishRes");
    }
    public VersionManager VersionMgr
    {
        get { return m_VerMgr; }
        set { m_VerMgr = value; }
    }
    public void GlobalInit()
    {
        m_VerMgr.Init();
    }
    public bool BeginLoadRes(ResType dir, bool bManager)
    {
//#if UNITY_EDITOR
        if (ENABLE_RESOURCES)
            return true;
//#endif
        m_bLoadedRes = true;
        int idx = (int)dir;
        if (m_AssetList[idx] != null)
            return true;

        m_AssetManager[idx] = bManager;
        
        string path;
        path = RuntimeInfo.GetResPersistentPath(dir);
#if UNITY_EDITOR
        if (!File.Exists(path))
        {
            return true;
        }
#endif
        m_AssetList[idx] = AssetBundle.LoadFromFile(path);
        if(m_AssetList[idx] == null)
        {
#if UNITY_EDITOR
            return true;
#endif
            LogMgr.Log("Asset not found:" + path + "exists:" + File.Exists(path));
        }
        if(m_AssetList[idx] != null)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    public void EndLoadRes()
    {
//#if UNITY_EDITOR
        if (ENABLE_RESOURCES)
            return;
//#endif
        m_bLoadedRes = false;
        for (int i = 0; i < RES_NUM; ++i)
        {
            if (m_AssetList[i] != null)
            {
               //m_AssetList[i].Unload(false);
               //m_AssetList[i] = null;
            }
        }
        Resources.UnloadUnusedAssets();
    }
    public void UnloadManagerObjects()
    {
        foreach(Object obj in m_LoadObjList.Values)
        {
            Resources.UnloadAsset(obj);
        }
        m_LoadObjList.Clear();
        Resources.UnloadUnusedAssets();
    }
    public Object LoadObject(string id, string path, ResType res, bool bManager = true)
    {
       
        Object obj;
        int idx = (int)res;
        if (ENABLE_RESOURCES || m_AssetList[idx] == null)
        {
            obj = Resources.Load(path + id);
        }
        else
        {
            //if (m_bLoadedRes == false)
            //{
            //    LogMgr.Log("资源加载必须在Init中进行, id:" + id + ", path:" + path);
            //}
            obj = m_AssetList[idx].LoadAsset(id);
#if !UNITY_EDITOR
            if (bManager && m_AssetManager[idx] && obj != null)
            {
                //m_LoadObjList.Add(obj.GetHashCode(), obj);
            }
#endif
        }
        if (obj == null && res != ResType.FishRes)
            LogMgr.Log("加载失败, id:" + id + ", path:" + path);
        return obj;
    }
    public Object LoadObject(string id, string path, ResType res, System.Type type, bool bManager = true)
    {
        
        Object obj;
        int idx = (int)res;
        if (ENABLE_RESOURCES || m_AssetList[idx] == null)
        {
            obj = Resources.Load(path + id, type);
        }
        else
        {
            //if (m_bLoadedRes == false)
            //{
            //    LogMgr.Log("资源加载必须在Init中进行, id:" + id + ", path:" + path);
            //}
            obj = m_AssetList[idx].LoadAsset(id, type);
#if !UNITY_EDITOR
            if (bManager && m_AssetManager[idx] && obj != null)
            {
                //m_LoadObjList.Add(obj.GetHashCode(), obj);
            }
#endif
        }
        if (obj == null && res != ResType.FishRes && res != ResType.PathRes)
            LogMgr.Log("加载失败, id:" + id + ", path:" + path);
        return obj;
    }
    public void UnloadObject(Object obj)
    {
#if !UNITY_EDITOR
        if(ENABLE_RESOURCES == false)
        {
            //m_LoadObjList.Remove(obj.GetHashCode());
            //Resources.UnloadAsset(obj);
        }
#endif
    }
}
