using UnityEngine;
using System.Collections.Generic;

class SDKSceneCallback:MonoBehaviour
{
    public void InitCallback(string str)
    {
        JsonData jd = JsonMapper.ToObject(str);
        uint ret;
        try
        {
            ret = uint.Parse(jd["resultCode"].ToString());
        }
        catch
        {
            ret = SDK.RESULT_FAILED;
        }
        SDKMgr.Instance.SDKCallback.InitCallback(ret == SDK.RESULT_OK);
    }
    public void LoginCallback(string str)
    {
        JsonData jd = JsonMapper.ToObject(str);
        uint retCode = uint.Parse(jd["resultCode"].ToString());
        if(retCode == SDK.LOGOUT)
        {
            //登出
            SDKMgr.Instance.SDKCallback.LogoutCallback("");
            return;
        }
        SDKLoginResult lr = new SDKLoginResult();
        try
        {
            if(retCode == (uint)SDK.RESULT_OK)
                lr.Result = LoginState.LOGIN_OK;
            else
                lr.Result = LoginState.LOGIN_FAILED;

            JsonData data   = jd["data"];
            lr.UID          = data["uid"].ToString();
            lr.ChannelLabel = data["channelLabel"].ToString();
            lr.ChannelUid   = data["channelUid"].ToString();
            lr.ProductCode  = data["productCode"].ToString();
            lr.ChannelCode  = data["channelCode"].ToString();
            lr.Token        = data["token"].ToString();
            lr.CustomParams = data["customParams"].ToString();
            lr.UserName     = data["userName"].ToString();
        }
        catch
        {
            lr.Result = LoginState.LOGIN_FAILED;
        }
        SDKMgr.Instance.SDKCallback.LoginCallback(lr);
    }
    public void LogoutCallback(string str)
    {
        JsonData jd = JsonMapper.ToObject(str);
        uint ret;
        string strParams = "";
        try
        {
            ret = uint.Parse(jd["resultCode"].ToString());
            strParams = jd["data"]["customParams"].ToString();
        }
        catch
        {
            ret = SDK.RESULT_FAILED;
        }
        SDKMgr.Instance.SDKCallback.LogoutCallback(strParams);
    }
    public void PayCallback(string str)
    {
        JsonData jd = JsonMapper.ToObject(str);
        uint ret;
        try
        {
            ret = uint.Parse(jd["resultCode"].ToString());
        }
        catch
        {
            ret = SDK.RESULT_FAILED;
        }
        SDKMgr.Instance.SDKCallback.PayCallback(ret == SDK.RESULT_OK);
    }
    public void ExitCallback(string str)
    {
        JsonData jd = JsonMapper.ToObject(str);
        uint ret;
        try
        {
            ret = uint.Parse(jd["resultCode"].ToString());
        }
        catch
        {
            ret = SDK.RESULT_FAILED;
        }
        SDKMgr.Instance.SDKCallback.ExitCallback(ret != SDK.CHANNEL_HAS_EXIT);
    }

#if UNITY_ANDROID
    public void LoginDomeCallBack(string str)
    {
        if (SDKChannelDome.isDebugDome)
        {
            AN_PoupsProxy.ShowToast("Login Result " + str);
        }

        string[] results = str.Split('|');

        string userId = results[0];
        string resultCode = results[1];

        SDKLoginResult lr = new SDKLoginResult();

        if (results[1]== "1")
        {
            lr.Result = LoginState.LOGIN_OK;
            lr.UID = userId;
        }
        else
        {
            //登陆失败
            lr.Result = LoginState.LOGIN_FAILED;
        }
        SDKMgr.Instance.SDKCallback.LoginCallback(lr);
    }

    public void PayDomeCallBack(string str)
    {
        if (SDKChannelDome.isDebugDome)
        {
            AN_PoupsProxy.ShowToast("Pay Result " + str);
        }
        string[] results = str.Split('|');
        string resultCode = results[1];
        uint ret;
        if (resultCode == "1")
        {
            ret = SDK.RESULT_OK;
        }
        else
        {
            // 支付失败
            ret = SDK.RESULT_FAILED;
        }
        SDKMgr.Instance.SDKCallback.PayCallback(ret == SDK.RESULT_OK);
    }

    public void GPCallback(string str)
    {
        bool isValid = false;
        if (str == "0")
        {
            isValid = true;
        }
        SDKMgr.Instance.SDKCallback.PayCallback(isValid);

        if(isValid)
        {
            CG_Cmd_VerifyOrder ncb = new CG_Cmd_VerifyOrder();
            ncb.SetCmdType(NetCmdType.CMD_CG_VerifyOrder);
            ncb.OrderID = SDKMgr.Instance.PayData.OrderID;
            NetServices.Instance.Send<CG_Cmd_VerifyOrder>(ncb);
        }
    }
#endif
}
