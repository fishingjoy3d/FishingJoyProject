using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// 冰趣渠道
/// </summary>
#if UNITY_ANDROID
public class SDKChannelDome : SDKChannel
{
    const string APP_CODE = "";

    public override void GlobalInit()
    {
        m_AndroidContext = SDKMgr.Instance.AndroidObj;
        Init();
        SetUserCallback();
    }
    public override void SetUserCallback()
    {
        m_AndroidContext.Call("setUserCallback", SDKMgr.Instance.CallbackObjName, "LoginCallback");
    }
    public override void Init()
    {
        m_AndroidContext.Call("init", SDKMgr.Instance.CallbackObjName, "InitCallback");
    }
    public override void Login(string customparms)
    {
        if (SDKMgr.Instance.LoginData.Result != LoginState.LOGIN_OK)
        {
            SDKMgr.Instance.SetLogonStateBegin();
            //m_AndroidContext.Call("login", customparms);

            DomePayAndroid.Instance.DomePayCSInit();
            string loginNo = GenerateRandomNumber(20);
            DomePayAndroid.Instance.InitializeApp(APP_CODE, loginNo, "LoginManager", "LoginCangQiongCallBack");
        }
    }
    public override void Logout(string customparms)
    {
        m_AndroidContext.Call("logout", customparms);
    }
    public override void Pay(int amount, string itemName, int count, string chargePointName, string customParams, int itemID)
    {
        m_AndroidContext.Call("pay", SDKMgr.Instance.CallbackObjName, "PayCallback", amount, itemName, count, chargePointName, customParams, ServerSetting.CALLBACK_URL);
    }
    public override void SetExtraData(string id, string roleId, string roleName, int roleLevel, int zoneId, string zoneName, int balance, int vip, string partyName)
    {
        m_AndroidContext.Call("setExtData", id, roleId, roleName, roleLevel, zoneId, zoneName, balance, vip, partyName);
    }
    public override string GetChannelLabel()
    {
        return m_AndroidContext.Call<string>("getChannelLabel");
    }
    public override void Exit()
    {
        m_AndroidContext.Call("exit", SDKMgr.Instance.CallbackObjName, "ExitCallback");
    }

    public override void ShowAndroidToast(string info)
    {
        m_AndroidContext.Call("showToast", info);
    }
    public override void ReleaseResource()
    {
        m_AndroidContext.Call("releaseResource");
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

}
#else

#endif
