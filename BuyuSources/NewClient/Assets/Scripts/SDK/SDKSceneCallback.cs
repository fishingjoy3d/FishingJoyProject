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
}
