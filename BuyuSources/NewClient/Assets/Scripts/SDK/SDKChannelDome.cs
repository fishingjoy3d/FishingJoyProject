using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// 冰趣渠道
/// </summary>
#if UNITY_ANDROID
public class SDKChannelDome : SDKChannel
{
    const string APP_CODE = "D0000356";
    string channel_name = "Dome";

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

    public override void Pay(int itemID, string chargePointName, string orderID, string url, string signCode)
    {
        //m_AndroidContext.Call("pay", SDKMgr.Instance.CallbackObjName, "PayCallback", amount, itemName, count, chargePointName, customParams, ServerSetting.CALLBACK_URL);

        string info = GetPayInfo(APP_CODE, chargePointName, orderID, url, signCode);

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

    static public string GetPayInfo(string appCode, string payCode, string orderNo, string notifyUrl, string sec)
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

        //sBuilder.Append(notifyUrl);
        sBuilder.Append("&signCode=");
        sBuilder.Append(sec);

        return sBuilder.ToString();
    }

}
#else

#endif
