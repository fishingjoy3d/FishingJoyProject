using UnityEngine;
using System.Collections.Generic;

public enum SdkType
{
    SDK_SELF = 0,    //自有
    SDK_CHANNEL,    //渠道
    SDK_DERIVED,    //派生
}
public class SDKMgr : Singleton<SDKMgr>, ISDKCallback
{
    public static bool IS_DISABLED_SDK      = true;
    public static bool IS_APP_STORE_VER     = false;
    public static SdkType SDK_TYPE          = SdkType.SDK_SELF;
    public static bool IS_SDK_CHANNEL       = SDK_TYPE == SdkType.SDK_SELF;
    public static string PACKAGE_NAME       = "";
    public static uint PACKAGE_NAME_CRC     = 0;
    GameObject          m_SceneCallbackObj  = null;
    SDKSceneCallback    m_SceneCallback     = null;

    ISDKInterface       m_Interface         = null;
#if UNITY_ANDROID
    AndroidJavaObject   m_AndroidObj        = null;
#endif
    bool                m_bSDKInitOK        = false;
    SDKLoginResult      m_LoginData         = new SDKLoginResult();
    public static void SetPackageName(string name)
    {
        PACKAGE_NAME = name;
    }
    //第三方登录,true是QQ、false是微信 
    public void ThirdLogin(bool isQQPlatform)
    {
        //渠道版本无法使用第三方登录
        if (IS_SDK_CHANNEL)
            return ;
        NativeInterface.BeginThirdLogin(isQQPlatform);
    }
    public void GlobalPreInit()
    {
        if (IS_DISABLED_SDK)
            return;
#if UNITY_ANDROID
        
        m_AndroidObj = new AndroidJavaClass("com.unity3d.player.UnityPlayer").GetStatic<AndroidJavaObject>("currentActivity");
#endif
    }
    public void GlobalInit()
    {
        if (IS_DISABLED_SDK)
        {
            IS_SDK_CHANNEL = false;
            SDK_TYPE = SdkType.SDK_SELF;
            PACKAGE_NAME = "com.leduo.buyu.self";
            byte[] data2 = System.Text.Encoding.ASCII.GetBytes(PACKAGE_NAME);
            PACKAGE_NAME_CRC = Crc.Crc32(data2, 0, data2.Length);
            m_Interface = null;
            m_Interface = new SDKBaseNoSDK();
            return;
        }
        IS_SDK_CHANNEL = SDK_TYPE == SdkType.SDK_CHANNEL;
        if (IS_SDK_CHANNEL)
        {
            m_Interface = new SDKChannel();
            m_SceneCallbackObj = new GameObject();
            m_SceneCallbackObj.name = "SDKCallbackObj";
            m_SceneCallback = m_SceneCallbackObj.AddComponent<SDKSceneCallback>();
        }
        else
        {
            if (SDK_TYPE == SdkType.SDK_DERIVED)
            {
                m_Interface = new SDKDerived();
            }
            else
            {
                m_Interface = new SDKBase();
            }
        }
        PACKAGE_NAME = NativeInterface.GetPackageName();
        byte[] data = System.Text.Encoding.ASCII.GetBytes(PACKAGE_NAME);
        PACKAGE_NAME_CRC = Crc.Crc32(data, 0, data.Length);
        
        //SDK初始化
#if UNITY_IOS
        IS_APP_STORE_VER = NativeInterface.IsAppStoreVer();
#endif
        LogMgr.Log("包名:" + PACKAGE_NAME + "|类型:" + SDK_TYPE+"|官网:" + IsOfficialVersion);
        m_LoginData.Result = LoginState.LOGIN_NONE;
        m_Interface.GlobalInit();
    }
#if UNITY_ANDROID
    public AndroidJavaObject AndroidObj
    {
        get
        {
            return m_AndroidObj;
        }
    }
#endif
    //是否是官方版本
    public bool IsOfficialVersion
    {
        get
        {
#if UNITY_IOS
            return IS_SDK_CHANNEL == false && IS_APP_STORE_VER == false;
#else
            return IS_SDK_CHANNEL == false;
#endif
        }
    }
    public bool IsServerOrder
    {
        get
        {
            return IS_SDK_CHANNEL == false;
        }
    }
    public string CallbackObjName
    {
        get
        {
            return m_SceneCallbackObj.name;
        }
    }
    public ISDKInterface SDK
    {
        get
        {
            return m_Interface;
        }
    }
    public ISDKCallback SDKCallback
    {
        get
        {
            return this;
        }
    }
    public SDKLoginResult LoginData
    {
        get
        {
            return m_LoginData;
        }
    }
    public bool InitOK
    {
        get
        {
            return m_bSDKInitOK;
        }
    }
    public void InitCallback(bool bResult)
    {
        if (bResult)
        {
            m_bSDKInitOK = true;
        }
        else
        {
            BlockLogic.Instance.AddBlock(StringTable.GetString("sdkiniterr"), 118, true, true, false);
        }
    }
    public void LoginCallback(SDKLoginResult result)
    {
        GlobalEffectMgr.Instance.CloseLoadingMessage();
        if (result.Result == LoginState.LOGIN_FAILED)
        {
            m_LoginData.Result = LoginState.LOGIN_NONE;
            LogonRuntime.LogonLogic.SDKLogoned(false);
            LogicManager.Instance.LogOff();
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("sdklogonfailed"), 3, false);
        }
        else
        {
            m_LoginData = result;
            if(LogonRuntime.LogonLogic != null)
                LogonRuntime.LogonLogic.SDKLogoned(true);
        }
    }
    public void SDKLogin()
    {
        if (m_LoginData.Result == LoginState.LOGIN_OK)
        {
            LogonRuntime.LogonLogic.SDKLogoned(true); 
        }
        else
            m_Interface.Login("");
    }
    public void LogoutCallback(string customParams)
    {
        //登出,返回登陆界面。
        m_LoginData.Result = LoginState.LOGIN_NONE;
        LogicManager.Instance.LogOff();
    }
    
    public bool IsLogon
    {
        get
        {
            return m_LoginData.Result == LoginState.LOGIN_OK;
        }
    }
    public void SetLogonStateNone()
    {
        m_LoginData.Result = LoginState.LOGIN_NONE;
    }
    public void SetLogonStateBegin()
    {
        m_LoginData.Result = LoginState.LOGIN_BEGIN;
    }
    public void ExitCallback(bool showExitUI)
    {
        if(showExitUI)
        {
            //显示退出确认UI
            if (SceneRuntime.Instance != null)
                GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("Exit_Tips1"), MssageType.Exit_Msg_5);
            else
                GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("Exit_Tips"), MssageType.Exit_Msg_5);

        }
        else
        {
            //直接退出释放资源
            ReleaseResource();
        }
    }
    public void Logout()
    {
        if(IsLogon)
        {
            m_Interface.Logout("");
        }
    }
#if UNITY_ANDROID
    public System.Collections.IEnumerator ExitThread(Object obj)
    {
        int k = 0;
        while(++k < 2)
        {
            yield return  new WaitForEndOfFrame();
        }
        m_AndroidObj.Call("androidKillProcess");
        Application.Quit();
        yield break;
    }
#endif
    public void ReleaseResource()
    {
        m_Interface.ReleaseResource();
#if UNITY_ANDROID
        if(SDKMgr.IS_SDK_CHANNEL)
            SceneMain.Instance.StartInnerCoroutine(ExitThread(null));
#endif
    }
    public void PayCallback(bool bResult)
    {
        if (bResult == false)
        {
            //支付失败，提示玩家支付失败。
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_Recharge_Failed"), 2.0f, false);
            //GlobalHallUIMgr.Instance.ShutDownPayBuyConfirm();
            //GlobalHallUIMgr.Instance.ShutDownPayWnd();
        }
        else
        {
            //支付成功，等待服务器返回支付结果。
            //GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("EC_Recharge_Sucess"), MssageType.Mag_Type_0);
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_Recharge_Sucess"), 2.0f, false);
            //GlobalHallUIMgr.Instance.ShutDownPayBuyConfirm();
            //GlobalHallUIMgr.Instance.ShutDownPayWnd();
        }
    }
}
