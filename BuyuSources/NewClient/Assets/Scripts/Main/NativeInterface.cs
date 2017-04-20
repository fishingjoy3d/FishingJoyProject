using UnityEngine;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
//调用外部接口
public class NativeInterface
{
    public static bool ComputeCrc(string str, string pwd, out uint crc1, out uint crc2, out uint crc3)
    {
        //crc1 = 123;
        //crc2 = 123;
        //crc3 = 123;
        string pwd1 = (pwd + "OnePwd").ToLower().Trim();
        string pwd2 = (pwd + "SecodPwd").ToLower().Trim();
        string pwd3 = (pwd + "ThreePwd").ToLower().Trim();

        crc1 = Crc.Crc32(Encoding.Default.GetBytes(pwd1.ToCharArray()), 0, pwd1.Length);
        crc2 = Crc.Crc32(Encoding.Default.GetBytes(pwd2.ToCharArray()), 0, pwd2.Length);
        crc3 = Crc.Crc32(Encoding.Default.GetBytes(pwd3.ToCharArray()), 0, pwd3.Length);
        return true;
    }
    public static void Block_DownNewClientVersion(string url)
    {
#if UNITY_ANDROID
        string msg = StringTable.GetString("need_ver_update");
        SDKMgr.Instance.AndroidObj.CallStatic("_DownloadNewClient", msg, RuntimeInfo.GetTitle(), url);
#elif UNITY_IOS
        _DownloadNewClient("", RuntimeInfo.GetTitle(), url);
#elif UNITY_STANDALONE_WIN
        string msg = StringTable.GetString("need_ver_update");
        Resolution.SwitchFullscreen();
        MessageBox(DllTest.WinPtr, msg, RuntimeInfo.GetTitle(), 1);
        SceneMain.Instance.Shutdown();
#endif
    }
    public static void Block_ShowMsg(string msgid, ushort code, bool bclose = false)
    {
        string msg = StringTable.GetString(msgid);
        if(code != 0)
            msg += " ErrCode:" + code.ToString();
#if UNITY_ANDROID
        SDKMgr.Instance.AndroidObj.CallStatic("_ShowMsgAndQuit", msg, RuntimeInfo.GetTitle(), bclose ? "close" : "none");

#elif UNITY_IOS
        _ShowMsgAndQuit(msg, RuntimeInfo.GetTitle(), bclose ? "close" : "none");

#elif UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
        MessageBox(DllTest.WinPtr, msg, RuntimeInfo.GetTitle(), 0);
        if (bclose)
            SceneMain.Instance.Shutdown();
#endif

    }
    public static void Block_ShowMsgAndExit(string msgid, ushort code)
    {
        Block_ShowMsg(msgid, code, true);
    }
    public static void DownNewClientVersion(string url)
    {
#if UNITY_IOS
        ShowMsg("need_ver_update", 0, false, true);
#endif
        BlockLogic.Instance.AddBlock(url, 0, true, true, true);
    }
    public static void ShowMsg(string msgid, ushort code, bool bclose = false, bool bblock = true)
    {
        BlockLogic.Instance.AddBlock(msgid, code, bclose, bblock, false);
    }
    public static void ShowMsgAndExit(string msgid, ushort code)
    {
        ShowMsg(msgid, code, true, true);
    }
#if (UNITY_STANDALONE_WIN)
    [DllImport("user32.dll", EntryPoint = "MessageBoxW", CharSet = CharSet.Unicode)]
    public static extern int MessageBox(int hwnd, string text, string caption, uint type);
#endif
    [DllImport("__Internal")]
    public static extern void _OpenCamera();
    [DllImport("__Internal")]
    public static extern void _OpenWeb(string url);
    [DllImport("__Internal")]
    public static extern void _OpenPhoto();
    [DllImport("__Internal")]
    public static extern void _ShowMsgAndQuit(string str, string title, string close);
    [DllImport("__Internal")]
    public static extern void _DownloadNewClient(string str, string title, string url);
    [DllImport("__Internal")]
    public static extern string _GetMac();
    [DllImport("__Internal")]
    public static extern string _GetChannelCode();
    [DllImport("__Internal")]
    public static extern void _SelfPay(string id, string itemID);

    [DllImport("__Internal")]
    public static extern string _IsAppStoreVer();
    [DllImport("__Internal")]
    public static extern void _InitShare(string wxid, string wxpwd, string sinaid, string sinapwd, string qqid, string qqpwd);
    [DllImport("__Internal")]
    public static extern void _BeginShare(string isfriend, string webUrl, string text, string imgUrl);
    [DllImport("__Internal")]
    public static extern void _BeginThirdLogin(string plateform);
    public static bool IsAppStoreVer()
    {
        return _IsAppStoreVer() == "true";
    }
    public static void BeginThirdLogin(bool isQQPlatform)
    {
#if UNITY_ANDROID
        SDKMgr.Instance.AndroidObj.CallStatic("_BeginThirdLogin", isQQPlatform ? "qq":"weixin");
#elif UNITY_IOS
        _BeginThirdLogin(isQQPlatform ? "qq":"weixin");
#else
        //win平台直接模拟测试
        SceneMain.Instance.OnThirdLogin("testid,test_access_token");
#endif
    }
    public static void OpenWeb(string url)
    {
        Application.OpenURL(url);
#if UNITY_ANDROID
        SDKMgr.Instance.AndroidObj.CallStatic("_OpenWeb", url);
#elif UNITY_IOS
        _OpenWeb(url);
#else
        Application.OpenURL(url);
#endif
    }
    public static string GetPackageName()
    {
        if (SDKMgr.IS_DISABLED_SDK)
            return "Test";
#if UNITY_ANDROID
        return SDKMgr.Instance.AndroidObj.CallStatic<string>("_GetPackageName");
#elif UNITY_IOS
        return _GetChannelCode();
#else
        return "com.leduo.buyu.self";
#endif
    }
    public static void OpenCamera()
    {
#if UNITY_ANDROID
        SDKMgr.Instance.AndroidObj.CallStatic("_OpenCamera", Application.persistentDataPath + "/images");
#elif UNITY_IOS
        _OpenCamera();
#endif
    }

    public static void OpenPhoto()
    {
#if UNITY_ANDROID
        SDKMgr.Instance.AndroidObj.CallStatic("_OpenPhoto", Application.persistentDataPath + "/images");
#elif UNITY_IOS
        _OpenPhoto();
#else
        DllTest.GetFile();
#endif
    }
    public static void InitShare(string wxid, string wxpwd, string sinaid, string sinapwd, string qqid, string qqpwd)
    {
#if UNITY_ANDROID
        SDKMgr.Instance.AndroidObj.CallStatic("_InitShare", wxid, wxpwd, sinaid, sinapwd, qqid, qqpwd);
#elif UNITY_IOS
        _InitShare(wxid, wxpwd, sinaid, sinapwd, qqid, qqpwd);
#endif
    }
    public static void BeginShare(bool toFriend, string txt, string path)
    {
        string istofriend = toFriend ? "yes" : "no";
#if UNITY_ANDROID
        SDKMgr.Instance.AndroidObj.CallStatic("_BeginShare", istofriend, ServerSetting.ShareWebUrl,txt, path);
#elif UNITY_IOS
        _BeginShare(istofriend, ServerSetting.ShareWebUrl,txt, path);
#else
        SceneMain.Instance.OnShare("");
#endif
    }

    public static string GetMac()
    {
        if (SDKMgr.IS_DISABLED_SDK)
            return "Test";
        string str;
#if UNITY_ANDROID
        str = SDKMgr.Instance.AndroidObj.CallStatic<string>("_GetMac");
#elif UNITY_IOS
        str = _GetMac();
#else
        str = "Windows_None";
#endif
        if (str == null)
            return "MAC_NONE";
        else
            return str;
    }
}
