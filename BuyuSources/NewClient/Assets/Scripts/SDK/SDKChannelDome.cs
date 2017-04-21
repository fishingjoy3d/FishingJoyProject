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
    string secret_key = "";
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

    //支付
    /*
    注意：支付收到成功的回调后，请到游戏服务器查询订单状态再进行道具发放，请勿直接做支付成功的UI提示。支付收到失败的回调后，可以进行明确的充值失败提示。
     
    amount 定额支付总金额，单位为人民币分
    itemName 游戏币名称，如金币、钻石等
    count 购买商品数量，如100钻石，传入100；10魔法石，传入10
    chargePointName 计费点名称，没有时可传空
    customParams 游戏开发者自定义字段，会与支付结果一起通知到游戏服务器，游戏服务器可通过该字段判断交易的详细内容（金额、角色等）
    callBackUrl 支付结果通知地址，支付完成后我方后台会向该地址发送支付通知
     */
    public override void Pay(int amount, string itemName, int count, string chargePointName, string customParams, int itemID)
    {
        //m_AndroidContext.Call("pay", SDKMgr.Instance.CallbackObjName, "PayCallback", amount, itemName, count, chargePointName, customParams, ServerSetting.CALLBACK_URL);

        string orderId = customParams;
        string url = ServerSetting.CALLBACK_URL;
        string productId = chargePointName;
        string sec = secret_key;

        string info = GetPayInfo(APP_CODE, productId, orderId, url, sec);

        DomePayAndroid.Instance.pay(info, "PAYManager", "GetPayInfo");

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
