using UnityEngine;
using System.Collections.Generic;
using System.IO;
//只存放本地运行时数据
public enum PlatformType
{
    Windows,
    Android,
    IOS,
    OSX,
    MAX
}
public class RuntimeInfo
{
    static string LocalDir;
    static string DataDir;
    public static string GetDataDir()
    {
        return DataDir;
    }
    public static void GlobalInit()
    {
        DataDir = Application.dataPath;
        LocalDir = GetLocalDirectory();
        CheckResDirExists();
    }
    public static string GetTitle()
    {
        return "乐多捕鱼3D";
    }
    public static string GetLocalDirectory()
    {
#if UNITY_EDITOR || UNITY_STANDALONE_WIN || UNITY_STANDALONE_OSX
        return Application.dataPath;
#else
        return Application.persistentDataPath;
#endif
    }
    public static string GetAssetDirectory()
    {
#if UNITY_EDITOR || UNITY_STANDALONE_WIN || UNITY_STANDALONE_OSX
        return LocalDir + "/StreamingAssets/";
#else
        return LocalDir + "/StreamingAssets/";
#endif
    }

    public static string GetPlatformString()
    {
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        return "Windows";
#elif UNITY_ANDROID
        return "Android";
#elif UNITY_IOS
        return "IOS";
#elif UNITY_STANDALONE_OSX
        return "OSX";
#endif
    }
    public static PlatformType GetPlatform()
    {
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        return PlatformType.Windows;
#elif UNITY_ANDROID
        return PlatformType.Android;
#elif UNITY_IOS
        return PlatformType.IOS;
#elif UNITY_STANDALONE_OSX
        return PlatformType.OSX;
#endif
    }
    public static string GetBusinessPlatform()
    {
#if PLATFORM_XIAOMI
        return "Xiaomi";
#elif PLATFORM_WEIXIN
        return "Weixin";
#elif PLATFORM_360
        return "360";
#elif PLATFORM_BAIDU
        return "Baidu";
#elif PLATFORM_UC
        return "UC";
#elif PLATFORM_91
        return "91";
#else
        return "Free";
#endif
    }
    public static string GetExt(PlatformType pt)
    {
        switch(pt)
        {
            case PlatformType.Android:
                return ".apk";
            case PlatformType.IOS:
                return ".ipa";
            case PlatformType.Windows:
                return ".exe";
            case PlatformType.OSX:
                return ".pkg";
        }
        return ".exe";
    }
    public static string GetFTPVersionURL()
    {
        string package = SDKMgr.PACKAGE_NAME;
        if(package != "")
            package = "_" + package;
        return string.Format("{0}/{1}/{2}_{3}_Version{4}.xml", ServerSetting.ResFtp, ServerSetting.VERSION_DIR, GetPlatformString(), GetResolutionLvl(), package);
    }
    public static string GetLogFilePath()
    {
        return LocalDir + "/data/log.txt";
    }
    public static string GetResPersistentPath(ResType res)
    {
        return string.Format("{0}/data/{1}_{2}.dat", LocalDir, GetPlatformString(), res.ToString());
    }
    public static string GetResStreamingPath(ResType res)
    {
#if UNITY_EDITOR || UNITY_STANDALONE_WIN || UNITY_STANDALONE_OSX
        return string.Format("file:///{0}/{1}_{2}.dat", Application.streamingAssetsPath, RuntimeInfo.GetPlatformString(), res.ToString());
#elif UNITY_ANDROID
        return string.Format("{0}/{1}_{2}.dat", Application.streamingAssetsPath, RuntimeInfo.GetPlatformString(), res.ToString());
#else
        return string.Format("file:///{0}/{1}_{2}.dat", Application.streamingAssetsPath, RuntimeInfo.GetPlatformString(), res.ToString());
#endif
    }
    public static bool CheckResDirExists()
    {
        if (System.IO.Directory.Exists(LocalDir) == false)
        {
            if (!System.IO.Directory.CreateDirectory(LocalDir).Exists)
                return false;
        }
        string dir = LocalDir + "/data";
        if (System.IO.Directory.Exists(dir) == false)
            System.IO.Directory.CreateDirectory(dir);
        string dir2 = LocalDir + "/images";
        if (System.IO.Directory.Exists(dir2) == false)
            System.IO.Directory.CreateDirectory(dir2);
        string dir3 = LocalDir + "/chat";
        if (System.IO.Directory.Exists(dir3) == false)
            System.IO.Directory.CreateDirectory(dir3);

        return true;
    }
    public static string GetAccountFileName()
    {
        return ("localdata.dat");
    }
    public static string GetVersionFileName()
    {
        return ("localnewver.dat");
    }
    public static string GetAudioFileName()
    {
        return ("localaudio.dat");
    }
    public static string GameShareMaxWin()
    {
        return ("gamesharemaxwin.dat");
    }
    public static string GetChatLogFileName(uint MyUserID, uint userID)
    {
        return string.Format("localchatlog_{0}_{1}", MyUserID, userID);
    }
    public static string GetGameConfig()
    {
        return ("gameconfig.dat");
    }
    public static string GetSubmarinePosFileName()
    {
        return ("localsubmarine.dat");
    }
    public static string GetNewGuideFileName()
    {
        return ("localnewguide.dat");
    }
    public static string GetMatchGuideFileName()
    {
        return ("localmatchguide.dat");
    }
    public static string GetSceneMathcGideFileName()
    {
        return ("localscenematchguide.dat");
    }
    public static string GetResolutionLvl()
    {
        if (Resolution.IsHD)
            return "HD";
        else
            return "SD";
    }
    public static string GetLocalPath(string fileName)
    {
        return string.Format("{0}/data/{1}", LocalDir, fileName);
    }
    public static string GetLocalChatPath(string fileName)
    {
        return string.Format("{0}/chat/{1}", LocalDir, fileName);
    }
    public static string[] GetLocalFiles()
    {
       return Directory.GetFiles(string.Format("{0}/data", LocalDir));
    }
    public static void DeleteLocalFile(string fileName)
    {
        string path = GetLocalPath(fileName);
        if(File.Exists(path))
            File.Delete(path);
    }
    public static void DeleteLocalChatFile(string fileName)
    {
        string path = GetLocalChatPath(fileName);
        if (File.Exists(path))
            File.Delete(path);
    }
    public static void DeleteLocalChatDir()
    {
        string path = string.Format("{0}/chat");
        if (File.Exists(path))
            File.Delete(path);
    }
    public static bool SaveLocalFile(string fileName, byte[] data)
    {
        string path = GetLocalPath(fileName);
        if(File.Exists(path))
            File.Delete(path);
        FileStream fs = new FileStream(path, FileMode.CreateNew);
        if (fs == null)
            return false;

        fs.Write(data, 0, data.Length);
        fs.Close();
        return true;
    }
    public static byte[] GetLocalFile(string fileName)
    {
        string path = GetLocalPath(fileName);
        if(File.Exists(path) == false)
            return null;
        FileStream fs = new FileStream(path, FileMode.Open);
        if (fs == null || fs.Length == 0)
            return null;
        byte[] data = new byte[fs.Length];
        fs.Read(data, 0, data.Length);
        fs.Close();
        return data;
    }

    public static bool SaveLocalChatFile(string fileName, byte[] data)
    {
        string path = GetLocalChatPath(fileName);
        if (File.Exists(path))
            File.Delete(path);
        FileStream fs = new FileStream(path, FileMode.CreateNew);
        if (fs == null)
            return false;

        fs.Write(data, 0, data.Length);
        fs.Close();
        return true;
    }
    public static byte[] GetLocalChatFile(string fileName)
    {
        string path = GetLocalChatPath(fileName);
        if (File.Exists(path) == false)
            return null;
        FileStream fs = new FileStream(path, FileMode.Open);
        if (fs == null || fs.Length == 0)
            return null;
        byte[] data = new byte[fs.Length];
        fs.Read(data, 0, data.Length);
        fs.Close();
        return data;
    }
}
