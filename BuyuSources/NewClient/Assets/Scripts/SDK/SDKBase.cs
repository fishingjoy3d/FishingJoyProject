using UnityEngine;
using System.Collections.Generic;

public class SDKBase:ISDKInterface
{
    #if UNITY_ANDROID
        AndroidJavaObject m_AndroidContext;
    #endif
    public void GlobalInit()
    {
        Init();
    }
    public void Init()
    {
        #if UNITY_ANDROID
            m_AndroidContext = SDKMgr.Instance.AndroidObj;
        #endif
    }
    public void Login(string customparms)
    {

    }
    public void Logout(string customparms)
    {

    }
    public void Pay(int amount, string itemName, int count, string chargePointName, string customParams, int itemID)
    {
        
        //void BeginPay(String Uid, String orderID, String itemID, String prices, String callbackInfo, String callbackURL)
#if UNITY_EDITOR
        string urlData = string.Format("{{transid:\"{0}\",redirecturl:\"{1}\",cpurl:\"\"}}", chargePointName, ServerSetting.CALLBACK_URL);
        string url = string.Format("https://web.iapppay.com/pc/exbegpay?transdata={0}&sign={1}&signtype=RSA", System.Web.HttpUtility.UrlEncode(urlData), System.Web.HttpUtility.UrlEncode(customParams));
        Application.OpenURL(url);
        //DllTest.Execute(Application.dataPath + "/myweb.exe " + url + " 1100 800");
        //PlayerRole.Instance.RoleRecharge.SendRecharge(uint.Parse(chargePointName));
#elif UNITY_ANDROID
        m_AndroidContext.CallStatic("_SelfPay", chargePointName);
#elif UNITY_STANDALONE_WIN
        //打开网页
        string urlData = string.Format("{{transid:\"{0}\",redirecturl:\"{1}\",cpurl:\"\"}}", chargePointName, ServerSetting.CALLBACK_URL);
        string url = string.Format("https://web.iapppay.com/pc/exbegpay?transdata={0}&sign={1}&signtype=RSA", System.Web.HttpUtility.UrlEncode(urlData), System.Web.HttpUtility.UrlEncode(customParams));
        Application.OpenURL(url);
#elif UNITY_IOS
        NativeInterface._SelfPay(chargePointName, (itemID + 100).ToString());
#else
        PlayerRole.Instance.RoleRecharge.SendRecharge(uint.Parse(chargePointName));
#endif
    }
    public void SetExtraData(string id, string roleId, string roleName, int roleLevel, int zoneId, string zoneName, int balance, int vip, string partyName)
    {

    }
    public string GetChannelLabel()
    {
        return "changlei";
    }
    public string GetChannelName()
    {
        return "changlei";
    }

    public void Exit()
    {
        SDKMgr.Instance.ExitCallback(true);
    }
    public void ShowAndroidToast(string info)
    {

    }
    public void ReleaseResource()
    {
        Application.Quit();
    }
}


public class SDKBaseNoSDK : ISDKInterface
{
    public void GlobalInit()
    {
    }
    public void Init()
    {
    }
    public void Login(string customparms)
    {

    }
    public void Logout(string customparms)
    {

    }
    public void Pay(int amount, string itemName, int count, string chargePointName, string customParams, int itemID)
    {
    }
    public void SetExtraData(string id, string roleId, string roleName, int roleLevel, int zoneId, string zoneName, int balance, int vip, string partyName)
    {

    }
    public string GetChannelLabel()
    {
        return "test";
    }
    public string GetChannelName()
    {
        return "test";
    }

    public void Exit()
    {
        SDKMgr.Instance.ExitCallback(true);
    }
    public void ShowAndroidToast(string info)
    {

    }
    public void ReleaseResource()
    {
        Application.Quit();
    }
}

