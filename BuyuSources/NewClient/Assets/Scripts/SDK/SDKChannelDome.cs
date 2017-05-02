using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

/// <summary>
/// 冰趣渠道
/// </summary>
#if UNITY_ANDROID
public class SDKChannelDome : SDKChannel
{
    const string APP_CODE = "D0000356";
    const string PRIVATE_KEY = "MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBAIwOcaDOCOFqd+9M5Sus5/DH7DRwcQKanC11f1KJxLqEAdEpS5Swu7hvtPJR4a97kYD4oNpbszQUHmWTsyGnqMo/ucTORSogrej/tEBAAltIpyC+Rp6qsGuwdhhBZ+yz8NS0fuHMCsRKZ55XD37riz4HQHPAspz4NNdJYI/NPC11AgMBAAECgYAM4u9Vil+Kzg8G955GbHxSzTJQiN/9C1i/XgY/A+oT9z1rj08i+Tfsemq9uQb47Hew1C+Ip9NPQWKimfpraE/BWOBSX4/BJIDLfOdoMgTns7/3ICIuhj5fTR8XMX4qfpnZTu9/opHCRXFPV42bop1BxZTjYnTwO9LdWdOsmCirwQJBAN2vp8OihiF7uzjynX10baIeuV8GGixdlM2BpJIevDUob+wwkzsIRo//ICIzMdAOXxKbzm6Aj3hJ4pimf4A6r3kCQQChvDLz9ZENH6tIzHUaeSMzPaWbJGKDCli7y5jvVZYu9DXhJmAjwgJ5eIHn5JJiLc3wMyqn2tJ0qcfp/ylbIcLdAkBiXqSp+f9c8kOkxHvABJ71dn65PML2dtwlyOZW9I59ZCEuBGwAoO52zTXcFy1+bjIf1sVmYWPIc7i6Ff+zzda5AkACorqB10Kh4B4+dXaDE+5K63pDaPhiAk0n4k1/uPlVko0+Og3fB05bBGe5i7QG/ZAZlfvf+GTtmca0PYBDB+3RAkEAhGnxthd0zdcXULmqMbk6s18OlrHzhQrCKhf3nlus+YV6oopZHUvjnJHxzdDzS69OsLMzWSsaez3lbYERIZ+qbQ==";
    const string channel_name = "Dome";
    public const string URL_CALLBACK = "http://121.43.187.210:1680/DomePay";
    public static bool isDebugDome = true;

    public override void GlobalInit()
    {
        m_AndroidContext = SDKMgr.Instance.AndroidObj;
        Init();
        SetUserCallback();
    }
    public override void SetUserCallback()
    {
        //m_AndroidContext.Call("setUserCallback", SDKMgr.Instance.CallbackObjName, "LoginCallback");
    }
    public override void Init()
    {
        //m_AndroidContext.Call("init", SDKMgr.Instance.CallbackObjName, "InitCallback");
        SDKMgr.Instance.SDKCallback.InitCallback(true);
    }
    public override void Login(string customparms)
    {
        if (SDKMgr.Instance.LoginData.Result != LoginState.LOGIN_OK)
        {
            SDKMgr.Instance.SetLogonStateBegin();
            //m_AndroidContext.Call("login", customparms);

            DomePayAndroid.Instance.DomePayCSInit();
            string loginNo = GenerateRandomNumber(20);
            DomePayAndroid.Instance.InitializeApp(APP_CODE, loginNo, SDKMgr.Instance.CallbackObjName, "LoginDomeCallBack");
        }
    }
    public override void Logout(string customparms)
    {
        //m_AndroidContext.Call("logout", customparms);
    }

    public override void Pay(int itemID, string chargePointName, string orderID, string url, string secret_key)
    {
        //m_AndroidContext.Call("pay", SDKMgr.Instance.CallbackObjName, "PayCallback", amount, itemName, count, chargePointName, customParams, ServerSetting.CALLBACK_URL);

        string url_encoder = AN_UrlEncoderProxy.Encode(url);
        if (isDebugDome)
        {
            AN_PoupsProxy.ShowToast("orderID " + orderID + "url " + url_encoder);
        }

        string info = GetPayInfo(APP_CODE, chargePointName, orderID, url_encoder);
        info += GetSignCode(info, PRIVATE_KEY);
        DomePayAndroid.Instance.pay(info, SDKMgr.Instance.CallbackObjName, "PayDomeCallBack");
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
        DomePayAndroid.Instance.ExitWithUI(SDKMgr.Instance.CallbackObjName, "ExitCallback");
    }

    public override void ShowAndroidToast(string info)
    {
        //m_AndroidContext.Call("showToast", info);
        AN_PoupsProxy.ShowToast(info);
    }
    public override void ReleaseResource()
    {
        //m_AndroidContext.Call("releaseResource");
    }

    // 随机数函数
    static char[] constant = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    static public string GenerateRandomNumber(int length)
    {
        System.Text.StringBuilder newRandom = new System.Text.StringBuilder();
        System.Random rd = new System.Random();
        for (int i = 0; i < length; i++)
        {
            newRandom.Append(constant[rd.Next(10)]);
        }
        return newRandom.ToString();
    }

    static public string GetPayInfo(string appCode, string payCode, string orderNo, string notifyUrl)
    {
        System.Text.StringBuilder sBuilder = new System.Text.StringBuilder();
        sBuilder.Append("appCode=");
        sBuilder.Append(appCode);
        sBuilder.Append("&orderNo=");
        sBuilder.Append(orderNo);//GenerateRandomNumber (20)
        sBuilder.Append("&payCode=");
        sBuilder.Append(payCode);
        sBuilder.Append("&payNotifyUrl=");
        sBuilder.Append(notifyUrl);

        return sBuilder.ToString();
    }

    static public string GetSignCode(string payInfo, string secret_key)
    {
        AndroidJavaClass rsaClass = new AndroidJavaClass("com.sdk.util.RSACoder");
        string sign_code = "";
        sign_code = rsaClass.CallStatic<string>("sign", payInfo, secret_key);
        return sign_code;
    }
}
#else

#endif
