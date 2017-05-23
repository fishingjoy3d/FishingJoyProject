using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Facebook.Unity;

/// <summary>
/// 台湾繁体版
/// Facebook登录
/// GooglePlay支付
/// </summary>
#if UNITY_ANDROID
public class SDKChannelTW : SDKChannel
{
    const string channel_name = "TW";
    const bool mDebug = true;

    public override void GlobalInit()
    {
        m_AndroidContext = SDKMgr.Instance.AndroidObj;
        Init();
        SetUserCallback();
    }
    public override void SetUserCallback()
    {

    }
    public override void Init()
    {
        if (!FB.IsInitialized)
        {
            // Initialize the Facebook SDK
            FB.Init(InitCallback, OnHideUnity);
        }
        else
        {
            // Already initialized, signal an app activation App Event
            FB.ActivateApp();
        }

        SDKMgr.Instance.SDKCallback.InitCallback(true);
    }

    private void InitCallback()
    {
        if (FB.IsInitialized)
        {
            // Signal an app activation App Event
            FB.ActivateApp();
            // Continue with Facebook SDK
            // ...
        }
        else
        {
            Debug.Log("Failed to Initialize the Facebook SDK");
        }
    }

    private void OnHideUnity(bool isGameShown)
    {
        if (!isGameShown)
        {
            // Pause the game - we will need to hide
            Time.timeScale = 0;
        }
        else
        {
            // Resume the game - we're getting focus again
            Time.timeScale = 1;
        }
    }

    public override void Login(string customparms)
    {
        if (SDKMgr.Instance.LoginData.Result != LoginState.LOGIN_OK)
        {
            SDKMgr.Instance.SetLogonStateBegin();

            var perms = new List<string>() { "public_profile", "email", "user_friends" };
            FB.LogInWithReadPermissions(perms, AuthCallback);
        }
    }

    private void AuthCallback(ILoginResult result)
    {
        SDKLoginResult lr = new SDKLoginResult();
        if (FB.IsLoggedIn)
        {
            // AccessToken class will have session details
            var aToken = Facebook.Unity.AccessToken.CurrentAccessToken;

            if(mDebug)
            {
                AN_PoupsProxy.ShowToast("User login " + aToken.UserId + " token " + aToken.TokenString);
                string permissions = string.Empty;
                // Print current access token's granted permissions
                foreach (string perm in aToken.Permissions)
                {
                    permissions += perm + " ";
                }
                AN_PoupsProxy.ShowToast(permissions);
            }
            lr.Result = LoginState.LOGIN_OK;
            lr.UID = aToken.UserId;
            lr.Token = aToken.TokenString;
        }
        else
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("User cancelled login");
            }
            //登陆失败
            lr.Result = LoginState.LOGIN_FAILED;
        }
        SDKMgr.Instance.SDKCallback.LoginCallback(lr);
    }

    public override void Logout(string customparms)
    {
        FB.LogOut();
        SDKMgr.Instance.GlobalInit();
    }

    public override void Pay(int itemID, string chargePointName, string orderID, string url, string secret_key)
    {

    }
    public override void SetExtraData(string id, string roleId, string roleName, int roleLevel, int zoneId, string zoneName, int balance, int vip, string partyName)
    {
        //m_AndroidContext.Call("setExtData", id, roleId, roleName, roleLevel, zoneId, zoneName, balance, vip, partyName);
    }
    public override string GetChannelLabel()
    {
        return channel_name;
    }
    //public override void Exit()
    //{
    //    DomePayAndroid.Instance.ExitWithUI(SDKMgr.Instance.CallbackObjName, "ExitCallback");
    //}

    public override void ShowAndroidToast(string info)
    {
        AN_PoupsProxy.ShowToast(info);
    }
    public override void ReleaseResource()
    {
        //m_AndroidContext.Call("releaseResource");
    }
}
#else

#endif
