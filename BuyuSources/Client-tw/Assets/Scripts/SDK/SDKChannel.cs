using UnityEngine;
using System.Collections.Generic;
using System.Runtime.InteropServices;
#if UNITY_ANDROID
public class SDKChannel:ISDKInterface
{
    protected AndroidJavaObject   m_AndroidContext = null;
    public virtual void GlobalInit()
    {
        m_AndroidContext = SDKMgr.Instance.AndroidObj;
        Init();
        SetUserCallback();
    }
    public virtual void SetUserCallback()
    {
        m_AndroidContext.Call("setUserCallback", SDKMgr.Instance.CallbackObjName, "LoginCallback");
    }
    public virtual void Init()
    {
        m_AndroidContext.Call("init", SDKMgr.Instance.CallbackObjName, "InitCallback");
    }
    public virtual void Login(string customparms)
    {
        if(SDKMgr.Instance.LoginData.Result != LoginState.LOGIN_OK)
        {
            SDKMgr.Instance.SetLogonStateBegin();
            m_AndroidContext.Call("login", customparms);
        }
    }
    public virtual void Logout(string customparms)
    {
        m_AndroidContext.Call("logout", customparms);
    }
    public virtual void Pay(int amount, string itemName, int count, string chargePointName, string customParams, int itemID)
    {
        m_AndroidContext.Call("pay", SDKMgr.Instance.CallbackObjName, "PayCallback", amount, itemName, count, chargePointName, customParams, ServerSetting.CALLBACK_URL);
    }

    public virtual void Pay(int itemID, string chargePointName, int orderID, string url, string signCode)
    {
        m_AndroidContext.Call("pay", SDKMgr.Instance.CallbackObjName, "PayCallback", itemID, chargePointName, orderID, chargePointName, url, signCode);
    }

    public virtual void SetExtraData(string id, string roleId, string roleName, int roleLevel, int zoneId, string zoneName, int balance, int vip, string partyName)
    {
        m_AndroidContext.Call("setExtData", id, roleId, roleName, roleLevel, zoneId, zoneName, balance, vip, partyName);
    }
    public virtual string GetChannelLabel()
    {
        return m_AndroidContext.Call<string>("getChannelLabel");
    }
    public virtual void Exit()
    {
        m_AndroidContext.Call("exit", SDKMgr.Instance.CallbackObjName, "ExitCallback");
    }
    public string GetChannelName()
    {
        return GetChannelLabel();
    }
    public virtual void ShowAndroidToast(string info)
    {
        m_AndroidContext.Call("showToast", info);
    }
    public virtual void ReleaseResource()
    {
        m_AndroidContext.Call("releaseResource");
    }
    public virtual void Share(string path)
    {

    }
    public virtual string GetHeadUrl()
    {
        return null;
    }
    public virtual bool HaveNickname()
    {
        return false;
    }
    public virtual void GetAppLink()
    {

    }
}
#else
public class SDKChannel:ISDKInterface
{
    [DllImport("__Internal")]
    private static extern void _init(string gameObject, string callbackMethod);
    [DllImport("__Internal")]
    private static extern void _setUserCallback(string gameObject, string callbackMethod);
    [DllImport("__Internal")]
    private static extern void _login(string customparams);
    [DllImport("__Internal")]
    private static extern void _pay(string gameObject, string callbackMethod, int amount, string itemName, int count, string chargePointName, string customParams, string callbackUrl);
    [DllImport("__Internal")]
    private static extern void _logout(string customparams);
    [DllImport("__Internal")]
    private static extern void _setExtData(string id, string roleId, string roleName, int roleLevel, int zoneId, string zoneName, int balance, int vip, string partyName);
    [DllImport("__Internal")]
    private static extern void _showIOSDialog(string title, string content);
    [DllImport("__Internal")]
    private static extern string _getChannelLabel();
    public void GlobalInit()
    {
        Init();
        SetUserCallback();
    }
    public void SetUserCallback()
    {
        _setUserCallback(SDKMgr.Instance.CallbackObjName, "LoginCallback");
    }
    public void Init()
    {
        _init(SDKMgr.Instance.CallbackObjName, "InitCallback");
    }
    public void Login(string customparms)
    {
        if(SDKMgr.Instance.LoginData.Result != LoginState.LOGIN_OK)
        {
            SDKMgr.Instance.SetLogonStateBegin();
            _login(customparms);
        }
    }
    public void Logout(string customparms)
    {
        _logout(customparms);
    }
    public void Pay(int amount, string itemName, int count, string chargePointName, string customParams, int itemID)
    {
        _pay(SDKMgr.Instance.CallbackObjName, "PayCallback", amount, itemName, count, chargePointName, customParams, ServerSetting.CALLBACK_URL);
    }
    public void Pay(int itemID, string chargePointName, string orderID, string url, string secret_key)
    {
        //_pay(SDKMgr.Instance.CallbackObjName, "PayCallback", amount, itemName, count, chargePointName, customParams, ServerSetting.CALLBACK_URL);
    }
    public void SetExtraData(string id, string roleId, string roleName, int roleLevel, int zoneId, string zoneName, int balance, int vip, string partyName)
    {
        _setExtData(id, roleId, roleName, roleLevel, zoneId, zoneName, balance, vip, partyName);
    }
    public string GetChannelLabel()
    {
        return _getChannelLabel();
    }
    public string GetChannelName()
    {
        return GetChannelLabel();
    }
    public void ShowAndroidToast(string info)
    {
        _showIOSDialog("乐多捕鱼3D", info);
    }
    public void Exit()
    {
    }
    public void ReleaseResource()
    {
        
    }
}
#endif