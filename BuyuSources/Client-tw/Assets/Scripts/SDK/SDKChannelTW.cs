using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Facebook.Unity;

/// <summary>
/// 台湾繁体版
/// Facebook登录、支付
/// GooglePlay支付
/// </summary>
#if UNITY_ANDROID
public class SDKChannelTW : SDKChannel
{
    const string channel_name = "TW";
    const bool mDebug = false;
    const string Share_Uri = "https://www.facebook.com/517655241715082";
    const string FB_Link = "https://www.facebook.com/517655241715082";

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

            if (FB.IsLoggedIn)
            {
                OnLoginSuccess();
            }
            else
            {
                var perms = new List<string>() { "public_profile", "email", "user_friends" };
                FB.LogInWithReadPermissions(perms, AuthCallback);
            }
        }
    }

    private void AuthCallback(ILoginResult result)
    {
        if (FB.IsLoggedIn)
        {
            OnLoginSuccess();
        }
        else
        {
            OnLoginFail();
        }
    }

    private void OnLoginSuccess()
    {
        // AccessToken class will have session details
        var aToken = Facebook.Unity.AccessToken.CurrentAccessToken;

        if (mDebug)
        {
            AN_PoupsProxy.ShowToast("User login id " + aToken.UserId + " token " + aToken.TokenString);
        }
        SDKLoginResult lr = new SDKLoginResult();
        lr.Result = LoginState.LOGIN_OK;
        lr.UID = aToken.UserId;
        lr.Token = aToken.TokenString;
        SDKMgr.Instance.SDKCallback.LoginCallback(lr);
        //QueryNickname();
    }

    private void OnLoginFail()
    {
        if (mDebug)
        {
            AN_PoupsProxy.ShowToast("User cancelled login");
        }
        //登陆失败
        SDKLoginResult lr = new SDKLoginResult();
        lr.Result = LoginState.LOGIN_FAILED;
        SDKMgr.Instance.SDKCallback.LoginCallback(lr);
    }

    public override void Logout(string customparms)
    {
        //FB.LogOut();
        SDKMgr.Instance.GlobalInit();
    }

    public override void Pay(int itemID, string productID, int orderID, string url, string signCode)
    {
        switch (SDKMgr.Instance.PayData.PayChannel)
        {
            case PayChannelType.Facebook:
                {
                    FBPay(productID, orderID);
                    break;
                }
            case PayChannelType.GooglePlay:
                {
                    GPPay(productID);
                    break;
                }
        }
    }

    private void FBPay(string productID, int orderID)
    {
        if (mDebug)
        {
            AN_PoupsProxy.ShowToast("FBPay productID " + productID + " orderID " + orderID);
        }
        FB.Canvas.PayWithProductId(productID, requestId: orderID.ToString(), callback: this.HandlePayResult);
    }

    private void GPPay(string buyKey)
    {
        m_AndroidContext.Call("Pay", buyKey, SDKMgr.Instance.CallbackObjName, "GPCallback");

        //SDKSceneCallback sceneCallback = GameObject.FindObjectOfType<SDKSceneCallback>();
        //if (sceneCallback != null)
        //{
        //    sceneCallback.GPCallback("0|GPA.3678-3274-3426-267890");
        //}
    }

    protected void HandlePayResult(IPayResult result)
    {
        bool bSuccess = false;
        if (result == null)
        {
            //this.Status = "Null";
        }

        // Some platforms return the empty string instead of null.
        if (!string.IsNullOrEmpty(result.Error))
        {
            //this.Status = "Error";
        }
        else if (result.Cancelled)
        {
            //this.Status = "Cancelled";
        }
        else if (!string.IsNullOrEmpty(result.RawResult))
        {
            //this.Status = "Success";
            bSuccess = true;
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast(result.RawResult);
            }
        }
        else
        {
            //this.Status = "Empty";
        }
        SDKMgr.Instance.SDKCallback.PayCallback(bSuccess);
    }

    public override void SetExtraData(string id, string roleId, string roleName, int roleLevel, int zoneId, string zoneName, int balance, int vip, string partyName)
    {
        //m_AndroidContext.Call("setExtData", id, roleId, roleName, roleLevel, zoneId, zoneName, balance, vip, partyName);
    }
    public override string GetChannelLabel()
    {
        return channel_name;
    }
    public override void Exit()
    {
        SDKMgr.Instance.ExitCallback(true);
    }

    public override void ShowAndroidToast(string info)
    {
        AN_PoupsProxy.ShowToast(info);
    }
    public override void ReleaseResource()
    {
        //m_AndroidContext.Call("releaseResource");
    }

    public override void Share(string path)
    {
        FB.ShareLink(new System.Uri(Share_Uri), callback: ShareCallback);
    }

    private void ShareCallback(IShareResult result)
    {
        if (result == null)
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("Share null");
            }
        }

        // Some platforms return the empty string instead of null.
        if (!string.IsNullOrEmpty(result.Error))
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("Share Error " + result.Error);
            }
        }
        else if (result.Cancelled)
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("Share Cancelled");
            }
        }
        else if (!string.IsNullOrEmpty(result.RawResult))
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("Share Success RawResult " + result.RawResult);
            }
        }
        else
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("Share Success RawResult IsNullOrEmpty");
            }
        }
    }

    public override string GetHeadUrl()
    {
        return null;
        //string userId = SDKMgr.Instance.LoginData.UID;
        //string url = "http://graph.facebook.com/" + userId + "/picture?type=large";
        //return url;
    }
    public override bool HaveNickname()
    {
        return false;
    }

    public void QueryNickname()
    {
        FB.API("me", HttpMethod.GET, QueryNicknameCallback);
    }
    private void QueryNicknameCallback(IGraphResult result)
    {
        if (result == null)
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("QueryNickname null");
            }
        }

        // Some platforms return the empty string instead of null.
        if (!string.IsNullOrEmpty(result.Error))
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("QueryNickname Error " + result.Error);
            }
        }
        else if (result.Cancelled)
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("QueryNickname Cancelled");
            }
        }
        else if (!string.IsNullOrEmpty(result.RawResult))
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("QueryNickname Success " + result.RawResult);
            }
        }
        else
        {
            if (mDebug)
            {
                AN_PoupsProxy.ShowToast("QueryNickname Success RawResult IsNullOrEmpty ");
            }
        }
    }

    public override void OpenFacebook()
    {
        m_AndroidContext.Call("openFacebookIntent");
    }
}
#else

#endif
