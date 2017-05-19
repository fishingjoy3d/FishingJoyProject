using UnityEngine;
using System.Collections.Generic;



public class PhoneLogon : LogonNormalWnd
{
    public override void Logon(GameObject go)
    {
        if (m_inputAccount.value.Length != 0 && m_inputps.value.Length != 0)
        {
            GlobalEffectMgr.Instance.ShowLoadingMessage();
            System.UInt64 moblieIphone = 0;
            try
            {
                moblieIphone = System.UInt64.Parse(m_inputAccount.value);
            }
            catch (System.Exception e)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Ec_PhoneLogon_Error"), 2);
                GlobalEffectMgr.Instance.CloseLoadingMessage();
                return;
            }
            //uint pawssad = 0;
            //try
            //{
            //    pawssad = uint.Parse(m_inputps.value);
            //}
            //catch (System.Exception e)
            //{
            //    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Ec_PhoneLogon_Error"), 2);
            //    GlobalEffectMgr.Instance.CloseLoadingMessage();
            //    return;
            //}

            ShutDown();
            if (GlobalLogon.Instance.AccountData != null && m_inputps.value == "********" && moblieIphone == GlobalLogon.Instance.AccountData.PhoneInfo.PhoneNumber)
             {
                 PhoneAccountInfo ad = new PhoneAccountInfo();
                 ad.PhoneNumber = moblieIphone;
                 ad.phoneCRC1 = GlobalLogon.Instance.AccountData.PhoneInfo.phoneCRC1;
                 ad.phoneCRC2 = GlobalLogon.Instance.AccountData.PhoneInfo.phoneCRC2;
                 ad.phoneCRC3 = GlobalLogon.Instance.AccountData.PhoneInfo.phoneCRC3;
                 LogonRuntime.LogonLogic.SecPasswordLogon(ad);
             }
             else
             {
                 AccountInfo rd = new AccountInfo();
                 rd.UID = m_inputAccount.value;
                 rd.PWD = m_inputps.value;
                 LogonRuntime.LogonLogic.SecPasswordLogon(rd);
             }                      
        }      
    }
    public override void FillLogonInfo()
    {
        if (GlobalLogon.Instance.AccountData != null && GlobalLogon.Instance.AccountData.PhoneInfo.PhoneNumber!=0)
        {
            m_inputAccount.value = GlobalLogon.Instance.AccountData.PhoneInfo.PhoneNumber.ToString();
            if (GlobalLogon.Instance.AccountData.bIsSavePhonePassword &&
                GlobalLogon.Instance.AccountData.PhoneInfo.phoneCRC1 != 0 &&
                GlobalLogon.Instance.AccountData.PhoneInfo.phoneCRC2 != 0 &&
                GlobalLogon.Instance.AccountData.PhoneInfo.phoneCRC3 != 0)
            {
                m_inputps.value = "********";
            }
        }
    }
    public override void ChangeState()
    {
        if (GlobalLogon.Instance.AccountData == null)
        {
            GlobalLogon.Instance.AccountData = new AccountData();
        }
        GlobalLogon.Instance.AccountData.bIsSavePhonePassword = !GlobalLogon.Instance.AccountData.bIsSavePhonePassword;
        GlobalLogon.Instance.AccountData.SaveAccountData();
        SetSprite();
    }
   public override void SetSprite()
    {
        if (GlobalLogon.Instance.AccountData != null)
        {
            m_spriteremember.gameObject.SetActive(GlobalLogon.Instance.AccountData.bIsSavePhonePassword);
        }
    }
}
