using UnityEngine;
using System.Collections.Generic;

public class SDKDerived : ISDKInterface
{
    public void GlobalInit()
    {
        Init();
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
        PlayerRole.Instance.RoleRecharge.SendRecharge(uint.Parse(chargePointName));
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
