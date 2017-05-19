using UnityEngine;
using System.Collections.Generic;

public enum Acoount_MgrUI_Type
{
    PasswordMgr_UI = 0,        //密码管理
    BindMobile_UI,            //手机绑定
    Phone_Password,           //手机密码
    Certification_UI,        //实名认证   
    ReceiverAddress_UI,       //收货地址
}

public struct Account_MgrBtn
{
    public UIButton m_UIBtn;  
    public bool m_IsChecked;
    public byte m_Tag;
}

public class Account_MgrUI : BaseWnd
{
    Vector3[] m_VecPos=new Vector3[5];
    Account_MgrBtn[] m_MgrBtn = new Account_MgrBtn[5];
    Transform[] m_AccountMgrTrans = new Transform[5];
    PasswordMgr m_PrsswordMgrUI = new PasswordMgr();
    BindMobile m_BindMobile = new BindMobile();
    PhonePasswordMgr m_phoneps = new PhonePasswordMgr();
    Certification m_Certification = new Certification();   
    ReceiverAddress m_ReceiverAddress = new ReceiverAddress();
    Acoount_MgrUI_Type m_MgrUITyp;


    void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.GlobalUIObject[0]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        for (byte i = 0; i < m_MgrBtn.Length; ++i)
        {
            m_MgrBtn[i].m_IsChecked = false;
            m_MgrBtn[i].m_Tag = i;
            m_MgrBtn[i].m_UIBtn = m_BaseTrans.GetChild(i + 1).GetComponent<UIButton>();           
            m_AccountMgrTrans[i] = m_BaseTrans.GetChild(i + 1);
            UIEventListener.Get(m_BaseTrans.GetChild(i + 1).gameObject).onClick = ChangeWndUI;
        }
        m_MgrBtn[(int)m_MgrUITyp].m_IsChecked = true;

        m_PrsswordMgrUI.Init(m_BaseTrans.GetChild(6).gameObject);
        m_BindMobile.Init(m_BaseTrans.GetChild(7).gameObject);
        m_phoneps.Init(m_BaseTrans.GetChild(8).gameObject);
        m_Certification.Init(m_BaseTrans.GetChild(9).gameObject);       
        m_ReceiverAddress.Init(m_BaseTrans.GetChild(10).gameObject);
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnCloseWnd;     

        if(SDKMgr.IS_SDK_CHANNEL||ThirdLogon.Instance.m_blogon)
        {
            //(1,2)3,4,往前移一格
            m_AccountMgrTrans[0].gameObject.SetActive(false);
            m_AccountMgrTrans[4].position = m_AccountMgrTrans[3].position;
            m_AccountMgrTrans[3].position = m_AccountMgrTrans[2].position;
            m_AccountMgrTrans[2].position = m_AccountMgrTrans[1].position = m_AccountMgrTrans[0].position;
        }
        else
        {      
   
        }
        SetBindBtn();
    }
    void SetBindBtn()
    {
        if (PlayerRole.Instance.EntityManager.GetRoleEntityInfo() == null)
            return;
        tagRoleAddressInfo pInfo = PlayerRole.Instance.EntityManager.GetRoleEntityInfo();
        bool bBind=FishDataInfo.IsCanUsePhone(pInfo.Phone);
        m_AccountMgrTrans[1].gameObject.SetActive(!bBind);
        m_AccountMgrTrans[2].gameObject.SetActive(bBind);
    }
    public void Update(float delta)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        if (m_MgrUITyp == Acoount_MgrUI_Type.BindMobile_UI)
        {
            m_BindMobile.Update(delta);
        }
    }
    public void UpdateAccountMgrDate()
    {       
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        switch (m_MgrUITyp)
        {
            case Acoount_MgrUI_Type.PasswordMgr_UI:
                m_PrsswordMgrUI.UpdatePasswordDate();
                break;
            case Acoount_MgrUI_Type.BindMobile_UI:
                {
                    m_BindMobile.UpdateBingMobileDate();                   
                }
                break;
            case Acoount_MgrUI_Type.Phone_Password:
                m_phoneps.UpdatePasswordDate();
                break;
            case Acoount_MgrUI_Type.Certification_UI:
                m_Certification.UpdateCertificationDate();
                break;       
            case Acoount_MgrUI_Type.ReceiverAddress_UI:
                m_ReceiverAddress.UpdateReceiverAddressDate();
                break;
        }
        SetBindBtn();
    }
    public void UpdateAccountMindState(bool bResult, bool bFrist, byte type)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        if (m_MgrUITyp == Acoount_MgrUI_Type.BindMobile_UI)
        {
            m_BindMobile.UpdateAccountMindState(bResult, bFrist, type);
            if (bResult && type==(byte)RoleOperateError.ROE_BindPhone_Sucess)//绑定成功
            {
                ChangeWndUI(m_MgrBtn[2].m_UIBtn.gameObject);//切换到手机器密码
            }
        }
    }
    public void ShowAccount_MgrUI(Acoount_MgrUI_Type type)
    {
        if (m_BaseWndObject != null)
            return;
        m_MgrUITyp = type;
        Init();
        ShowChildeUI(true, m_MgrUITyp);
        ChangeButtonBg();
        UpdateAccountMgrDate();
        WndManager.Instance.Push(m_BaseWndObject);
    }
    public void ChangeAccountSeccess()
    {
        m_PrsswordMgrUI.ChangeAccountSeccess();
    }
    public void PhonePsChanged()
    {
        m_phoneps.PhonePsChanged();
    }
    void ShowChildeUI(bool bShow, Acoount_MgrUI_Type type)
    {
        switch (type)
        {
            case Acoount_MgrUI_Type.PasswordMgr_UI:
                m_PrsswordMgrUI.ShowPasswordMgrUI(bShow);
                break;
            case Acoount_MgrUI_Type.BindMobile_UI:
                m_BindMobile.ShowBingMobileUI(bShow);
                break;
            case Acoount_MgrUI_Type.Phone_Password:
                m_phoneps.ShowPasswordMgrUI(bShow);
                break;
            case Acoount_MgrUI_Type.Certification_UI:
                m_Certification.ShowCertificationUI(bShow);
                break;           
            case Acoount_MgrUI_Type.ReceiverAddress_UI:
                m_ReceiverAddress.ShowReceiverAddressUI(bShow);
                break;
        }
    }
    void ChangeWndUI(GameObject go)
    {
        byte tagBtn = 255;
        for (byte i = 0; i < m_MgrBtn.Length; ++i)
        {
            if (m_MgrBtn[i].m_UIBtn.gameObject == go)
            {
                m_MgrBtn[i].m_IsChecked = true;
                tagBtn = m_MgrBtn[i].m_Tag;
            }
            else
                m_MgrBtn[i].m_IsChecked = false;
        }
        switch (tagBtn)
        {
            case 0:   //密码管理
                {
                    if (m_MgrUITyp == Acoount_MgrUI_Type.PasswordMgr_UI)
                        return;
                    ShowChildeUI(false, m_MgrUITyp);
                    m_MgrUITyp = Acoount_MgrUI_Type.PasswordMgr_UI;
                    ShowChildeUI(true, m_MgrUITyp);
                    break;
                }
            case 1:   //绑定手机
                {
                    if (m_MgrUITyp == Acoount_MgrUI_Type.BindMobile_UI)
                        return;
                    ShowChildeUI(false, m_MgrUITyp);
                    m_MgrUITyp = Acoount_MgrUI_Type.BindMobile_UI;
                    ShowChildeUI(true, m_MgrUITyp);
                    break;
                } 
            case 2://手机密码
                {
                    if (m_MgrUITyp == Acoount_MgrUI_Type.Phone_Password)
                        return;
                    ShowChildeUI(false, m_MgrUITyp);
                    m_MgrUITyp = Acoount_MgrUI_Type.Phone_Password;
                    ShowChildeUI(true, m_MgrUITyp);
                    break;
                }
            case 3:   //实名认证
                {
                    if (m_MgrUITyp == Acoount_MgrUI_Type.Certification_UI)
                        return;
                    ShowChildeUI(false, m_MgrUITyp);
                    m_MgrUITyp = Acoount_MgrUI_Type.Certification_UI;
                    ShowChildeUI(true, m_MgrUITyp);
                    break;
                }                              
            case 4:   //收货地址
                {
                    if (m_MgrUITyp == Acoount_MgrUI_Type.ReceiverAddress_UI)
                        return;
                    ShowChildeUI(false, m_MgrUITyp);
                    m_MgrUITyp = Acoount_MgrUI_Type.ReceiverAddress_UI;
                    ShowChildeUI(true, m_MgrUITyp);
                    break;
                }               
        }
        ChangeButtonBg();
        UpdateAccountMgrDate();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    public void ChangeButtonBg()
    {
        for (byte i = 0; i < m_MgrBtn.Length; ++i)
        {
            string str = m_MgrBtn[i].m_UIBtn.normalSprite;
            if (m_MgrBtn[i].m_IsChecked)
                str = str.Replace("0", "3");
            else
                str = str.Replace("3", "0");
            m_MgrBtn[i].m_UIBtn.hoverSprite = str;
            m_MgrBtn[i].m_UIBtn.normalSprite = str;
            m_MgrBtn[i].m_UIBtn.pressedSprite = str;
            m_MgrBtn[i].m_UIBtn.disabledSprite = str;
        }
    }
    void OnCloseWnd(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        if (ShopRuntime.Instance != null)
        {
            ShopRuntime.ShopLogicMgr.SetMaskEft(true);
        }

        ShutDown();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }

}


//密码管理类
class PasswordMgr : BaseWnd
{
    UIInput[] m_UIInput = new UIInput[3];
    UIInput[] m_VisitorInput = new UIInput[3];
    GameObject[] m_Obj = new GameObject[2];
    public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;
        m_Obj[0] = m_BaseTrans.GetChild(1).gameObject;
        m_Obj[1] = m_BaseTrans.GetChild(2).gameObject;
        for (byte i = 0; i < m_UIInput.Length; ++i)
        {
            m_UIInput[i] = m_BaseTrans.GetChild(1).GetChild(i).GetComponent<UIInput>();
            m_VisitorInput[i] = m_BaseTrans.GetChild(2).GetChild(i).GetComponent<UIInput>();
        }
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnOK;
    }
    void ChangeChildWnd()
    {
        //该玩家为游戏客
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetIsCanResetAccount())
        {
            m_Obj[1].SetActive(true);
            m_Obj[0].SetActive(false);
        }
        else
        {
            m_Obj[0].SetActive(true);
            m_Obj[1].SetActive(false);
        }
    }
    public void ChangeAccountSeccess()
    {
        //if (GlobalLogon.Instance.AccountData != null && GlobalLogon.Instance.AccountData.SavePassword)
        {
            //GlobalLogon.Instance.AccountData.TAG = 0;
            //GlobalLogon.Instance.AccountData.UID = m_VisitorInput[0].value;
            //NativeInterface.ComputeCrc(GlobalLogon.Instance.AccountData.UID, m_VisitorInput[1].value, out GlobalLogon.Instance.AccountData.CRC1,
            //    out GlobalLogon.Instance.AccountData.CRC2, out GlobalLogon.Instance.AccountData.CRC3);
            //GlobalLogon.Instance.SaveAccountData(GlobalLogon.Instance.AccountData);

        }
    }
    public void ShowPasswordMgrUI(bool bShow)
    {
        if (m_BaseWndObject.activeSelf != bShow)
            m_BaseWndObject.SetActive(bShow);
        if (bShow)
        {
            ChangeChildWnd();
        }

    }
    public void UpdatePasswordDate()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        ChangeChildWnd();
    }
    //确认按纽
    void OnOK(GameObject go)
    {
        //GlobalHallUIMgr.Instance.ShowSystemTipsUI("", 2, true);
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetIsCanResetAccount())
        {
            if (m_VisitorInput[0].value.Length == 0)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ResetAccount_Error1"), 1);
                return;
            }
            if (m_VisitorInput[1].value.Length == 0)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error2"), 1);
                return;
            }
            if (m_VisitorInput[1].value != m_VisitorInput[2].value)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error"), 1);
                return;
            }

            PlayerRole.Instance.RoleInfo.ResetAccountInfo(m_VisitorInput[0].value, m_VisitorInput[1].value);
        }
        else
        {
            if (m_UIInput[0].value.Length == 0)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error1"), 1);
                return;
            }
            if (m_UIInput[1].value.Length == 0)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error2"), 1);
                return;
            }
            if (m_UIInput[1].value != m_UIInput[2].value)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error"), 1);
                return;
            }
            PlayerRole.Instance.RoleInfo.ResetAccountPassword(m_UIInput[0].value, m_UIInput[1].value);
        }
    }
}

class PhonePasswordMgr : BaseWnd
{

    UIInput[] m_UIInput = new UIInput[3];
    UIInput[] m_VisitorInput = new UIInput[3];
    GameObject[] m_Obj = new GameObject[2];
    UILabel m_label;//phone num;
    public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;
        m_Obj[0] = m_BaseTrans.GetChild(1).gameObject;
        m_Obj[1] = m_BaseTrans.GetChild(2).gameObject;
        for (byte i = 0; i < m_UIInput.Length; ++i)
        {
            m_UIInput[i] = m_BaseTrans.GetChild(1).GetChild(i).GetComponent<UIInput>();
            m_VisitorInput[i] = m_BaseTrans.GetChild(2).GetChild(i).GetComponent<UIInput>();
        }
        m_label = m_BaseTrans.GetChild(1).GetChild(4).GetComponent<UILabel>();
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnOK;
    }
    void ChangeChildWnd()
    {
        //该玩家为游戏客
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetIsCanResetAccount())
        {
            m_Obj[1].SetActive(true);
            m_Obj[0].SetActive(false);
        }
        else
        {
            m_Obj[0].SetActive(true);
            m_Obj[1].SetActive(false);
        }
    }
    public void PhonePsChanged()
    {  
        GlobalLogon.Instance.SavePhonePassWord(m_UIInput[1].value);                    
    }
    public void ShowPasswordMgrUI(bool bShow)
    {
        if (m_BaseWndObject.activeSelf != bShow)
            m_BaseWndObject.SetActive(bShow);
        if (bShow)
        {
            ChangeChildWnd();
        }

    }
    public void UpdatePasswordDate()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        ChangeChildWnd();
        {
            if (PlayerRole.Instance.EntityManager.GetRoleEntityInfo() != null)
            {
                string strPhone=PlayerRole.Instance.EntityManager.GetRoleEntityInfo().Phone.ToString();
                m_label.text = strPhone.Substring(0, 3) + "****" + strPhone.Substring(strPhone.Length-2,2);
            }                          
        }      
    }
    //确认按纽
    void OnOK(GameObject go)
    {
        //GlobalHallUIMgr.Instance.ShowSystemTipsUI("", 2, true);
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetIsCanResetAccount())
        {
            if (m_VisitorInput[0].value.Length == 0)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ResetAccount_Error1"), 1);
                return;
            }
            if (m_VisitorInput[1].value.Length == 0)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error2"), 1);
                return;
            }
            if (m_VisitorInput[1].value != m_VisitorInput[2].value)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error"), 1);
                return;
            }

            PlayerRole.Instance.RoleInfo.ResetAccountInfo(m_VisitorInput[0].value, m_VisitorInput[1].value);
        }
        else
        {
            if (m_UIInput[0].value.Length == 0)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error1"), 1);
                return;
            }
            if (m_UIInput[1].value.Length == 0)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error2"), 1);
                return;
            }
            if (m_UIInput[1].value != m_UIInput[2].value)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_ChangePassword_Error"), 1);
                return;
            }
            PlayerRole.Instance.RoleInfo.ChangeSecPassword(m_UIInput[0].value, m_UIInput[1].value);
        }
    }
}


//实名认证类
class Certification : BaseWnd
{
    UIInput[] m_UIInput = new UIInput[2];
    GameObject m_NoCertification;      //未认证
    GameObject m_CertificationOK;      //认证成功

    public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;

        m_NoCertification = m_BaseTrans.GetChild(0).gameObject;
        for (byte i = 0; i < m_UIInput.Length; ++i)
        {
            m_UIInput[i] = m_BaseTrans.GetChild(0).GetChild(i + 1).GetComponent<UIInput>();
        }
        m_CertificationOK = m_BaseTrans.GetChild(1).gameObject;

        UIEventListener.Get(m_BaseTrans.GetChild(0).GetChild(0).gameObject).onClick = OnOK;
    }
    public void ShowCertificationUI(bool bShow)
    {
        if (m_BaseWndObject.activeSelf != bShow)
            m_BaseWndObject.SetActive(bShow);
    }
    public void UpdateCertificationDate()
    {
        if (PlayerRole.Instance.EntityManager.GetRoleEntityInfo() == null)
            return;

        tagRoleAddressInfo pInfo = PlayerRole.Instance.EntityManager.GetRoleEntityInfo();
        if (pInfo.Name != null && pInfo.Name.Length > 0 && pInfo.Name != "" && pInfo.IdentityID != null && pInfo.IdentityID.Length > 0 && pInfo.IdentityID != "")
        {
            m_CertificationOK.SetActive(true);
            m_NoCertification.SetActive(false);
        }
    }
    //实名认证成功
    public void CertificationSucceed()
    {
        m_CertificationOK.SetActive(true);
    }
    void OnOK(GameObject go)
    {
        if (m_UIInput[0].value.Length == 0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_Verification_NameError"), 1);
            return;
        }
        if (m_UIInput[1].value.Length == 0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_Verification_NameError2"), 1);
            return;
        }
        PlayerRole.Instance.RoleOperate.OnRealNameVerification(m_UIInput[0].value, m_UIInput[1].value);
    }
}

public struct Reward
{
    public UISprite m_UIReward;
    public GameObject m_RewardObj;

}
//绑定手机
class BindMobile : BaseWnd
{
    UIInput[]       m_UIInput = new UIInput[4];
    Reward[]        m_Reward = new Reward[3];
    GameObject      m_NoBindObj;        //未绑定
    //GameObject      m_BindSucceedObj;   //已绑定

    UIButton        m_GetSecruityCodeBtn;
    UILabel         m_SilderLabel;
    TweenPosition   m_TweenAnim;
    Transform       m_SilderBtnTrans;
    Vector3         m_OrgPos;
    Vector3         m_EndPos;
    bool            isCountCD = false;
    float           cd_Time = 240.0f;
    bool            m_BtnPress;
    float           m_FristX;
    public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;
        m_NoBindObj = m_BaseTrans.GetChild(0).gameObject;

        for (byte i = 0; i < m_UIInput.Length; ++i)
        {
            m_UIInput[i] = m_BaseTrans.GetChild(0).GetChild(i + 2).GetComponent<UIInput>();
        }

        for (byte j = 0; j < m_Reward.Length; ++j)
        {
            m_Reward[j].m_RewardObj = m_BaseTrans.GetChild(0).GetChild(6 + j).gameObject;
            m_Reward[j].m_UIReward = m_Reward[j].m_RewardObj.GetComponent<UISprite>();
        }
       // m_CDGameObj = m_BaseTrans.GetChild(0).GetChild(2).GetChild(3).gameObject;
        //m_CDLabel = m_CDGameObj.GetComponent<UILabel>();
        m_GetSecruityCodeBtn = m_BaseTrans.GetChild(0).GetChild(0).GetComponent<UIButton>();
       // m_BindSucceedObj = m_BaseTrans.GetChild(1).gameObject;
        m_SilderLabel = m_BaseTrans.GetChild(0).GetChild(9).GetComponent<UILabel>();
        m_TweenAnim = m_BaseTrans.GetChild(0).GetChild(0).GetComponent<TweenPosition>();
        m_SilderBtnTrans = m_BaseTrans.GetChild(0).GetChild(0);
        m_FristX = m_SilderBtnTrans.position.x;
        m_OrgPos = m_SilderBtnTrans.position;
        m_EndPos = m_BaseTrans.GetChild(0).GetChild(10).position;
       // UIEventListener.Get(m_BaseTrans.GetChild(0).GetChild(0).gameObject).onClick = OnGetSecruityCode;
        UIEventListener.Get(m_BaseTrans.GetChild(0).GetChild(1).gameObject).onClick = OnBindMobile;
        UIEventListener.Get(m_BaseTrans.GetChild(1).GetChild(0).gameObject).onClick = OnChange;
        UIEventListener.Get(m_BaseTrans.GetChild(0).GetChild(0).gameObject).onPress = OnGetSecruityCodePress;

        isCountCD = false;

    }
    public void ShowBingMobileUI(bool bShow)
    {
        if (m_BaseWndObject.activeSelf != bShow)
            m_BaseWndObject.SetActive(bShow);
        if (bShow)
        {
            ushort RewardID = FishConfig.Instance.m_SystemConfig.FirstBindPhoneRewardID;
            tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID(RewardID);
            if (pReward == null)
                return;
            for (byte i = 0; i < pReward.RewardItemVec.Count; ++i)
            {
                uint ItemID = pReward.RewardItemVec[i].ItemID;
                if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                    continue;
                if (i >= 3)
                    break;
                m_Reward[i].m_UIReward.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon;
                m_Reward[i].m_RewardObj.SetActive(true);
            }
        }
    }
    public void Update(float deltaTime)
    {
        if (isCountCD)
        {
            cd_Time -= deltaTime;
            m_SilderLabel.text = string.Format(StringTable.GetString("Ec_BindPhone_GetCode"), (int)cd_Time);
           // m_CDLabel.text = ((int)cd_Time).ToString() + "s";
            if (cd_Time <= 0)
            {
                m_SilderLabel.text = string.Format(StringTable.GetString("Ec_BindPhone_GetCode1"));
                isCountCD = false;
                m_GetSecruityCodeBtn.isEnabled = true;
                m_SilderBtnTrans.position = m_OrgPos;
            }
        }
        if (m_BtnPress && Input.GetMouseButton(0))
        {
            Vector3 tempPos = m_OrgPos;
            Vector3 pos = SceneObjMgr.Instance.UICamera.ScreenToWorldPoint(Input.mousePosition);
            m_FristX = tempPos.x - pos.x;
            tempPos.x -= m_FristX;
            if (tempPos.x > m_EndPos.x)
                tempPos = m_EndPos;
            if (tempPos.x < m_OrgPos.x)
                tempPos = m_OrgPos;
            m_SilderBtnTrans.position = tempPos;
        }


    }
    public void UpdateAccountMindState(bool bResult, bool bFrist, byte type)
    {
        if (bResult == false && type > 50 && type < 53)
        {
            isCountCD = false;
            m_SilderBtnTrans.position = m_OrgPos;
            m_FristX = 0;
            m_GetSecruityCodeBtn.isEnabled = true;
            m_SilderLabel.text = string.Format(StringTable.GetString("Ec_BindPhone_GetCode1"));
        }
        if (bResult && type==(byte)RoleOperateError.ROE_BindPhone_Sucess)//帮定成功,
        {                       
            GlobalLogon.Instance.SavePhoneNum(ulong.Parse(m_UIInput[0].value));//保存手机号
            GlobalLogon.Instance.SavePhonePassWord(m_UIInput[1].value);//保存密码                        
        }
    }
    public void UpdateBingMobileDate()
    {
        //if (PlayerRole.Instance.EntityManager.GetRoleEntityInfo() == null)
        //    return;
        //tagRoleAddressInfo pInfo = PlayerRole.Instance.EntityManager.GetRoleEntityInfo();
        //if (FishDataInfo.IsCanUsePhone(pInfo.Phone))
        //{
        //    //m_BindSucceedObj.SetActive(true);
        //    //m_NoBindObj.SetActive(false);
        //}
    }
    void OnGetSecruityCodePress(GameObject go, bool state)
    {
        if (state)
        {
            m_BtnPress = true;
        }
        else
        {
            if (Mathf.Abs(m_FristX) < 0.6f)
            {
                m_SilderBtnTrans.position = m_OrgPos;
            }
            else
            {
                OnGetSecruityCode();
            }
            m_BtnPress = false;
        }
    }
    void OnGetSecruityCode()
    {
        if (m_UIInput[0].value.Length == 0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_BindPhone_PhoneIsEmpty"), 1.5f);
            m_SilderBtnTrans.position = m_OrgPos;
            m_FristX = 0;
            return;
        }
        ulong nNum = 0;
        try 
        {
            nNum = System.Convert.ToUInt64(m_UIInput[0].value);
        }
        catch
        { }
        if (PlayerRole.Instance.RoleOperate.OnGetPhoneVerificationNum(nNum))
        {
            m_FristX = 0;
            cd_Time = 240.0f;
            m_GetSecruityCodeBtn.isEnabled = false;
            m_SilderBtnTrans.position = m_EndPos;
          //  m_CDGameObj.SetActive(true);
            isCountCD = true;
            m_SilderLabel.text = string.Format(StringTable.GetString("Ec_BindPhone_GetCode"), cd_Time);
        }
        else
        {
            m_SilderBtnTrans.position = m_OrgPos;
            m_FristX = 0;
        }
    }
    void OnBindMobile(GameObject go)
    {        
        if (m_UIInput[1].value.Length == 0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Ec_BindPhone_psemtpy"), 1.5f);
            return;
        }
        
        if (m_UIInput[2].value.Length == 0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Ec_BindPhone_ps2emtpy"), 1.5f);
            return;
        }
        if(m_UIInput[1].value!=m_UIInput[2].value)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Ec_BindPhone_psnotsame"), 1.5f);
            return;
        }

        if (m_UIInput[3].value.Length == 0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Ec_BindPhone_codeempty"), 1.5f);
            return;
        }
        uint nCode=0;
        try
        {
            nCode=uint.Parse(m_UIInput[3].value);
        }
        catch
        {

        }
        if(nCode==0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Ec_BindPhone_invaliedcode"), 1.5f);
            return;
        }
        PlayerRole.Instance.RoleOperate.OnBindPhone(nCode, m_UIInput[1].value);
    }
    void OnChange(GameObject go)
    {
        //m_BindSucceedObj.SetActive(false);
       // m_NoBindObj.SetActive(true);
    }
}

//收货地址
class ReceiverAddress : BaseWnd
{
    UIInput[] m_UIInput = new UIInput[2];

    public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;

        for (byte i = 0; i < m_UIInput.Length; ++i)
        {
            m_UIInput[i] = m_BaseTrans.GetChild(i).GetComponent<UIInput>();
        }

        UIEventListener.Get(m_BaseTrans.GetChild(2).gameObject).onClick = OnSave;
    }
    public void ShowReceiverAddressUI(bool bShow)
    {
        if (m_BaseWndObject.activeSelf != bShow)
            m_BaseWndObject.SetActive(bShow);
    }
    public void UpdateReceiverAddressDate()
    {
        if (PlayerRole.Instance.EntityManager.GetRoleEntityInfo() == null)
            return;
        tagRoleAddressInfo pInfo = PlayerRole.Instance.EntityManager.GetRoleEntityInfo();
        if (pInfo.EntityItemUseName != null && pInfo.EntityItemUseName.Length > 0 && pInfo.EntityItemUseName != "" && pInfo.EntityItemUseAddres != null && pInfo.EntityItemUseAddres.Length > 0 && pInfo.EntityItemUseAddres != "")
        {
            m_UIInput[0].value = pInfo.EntityItemUseName;
            m_UIInput[1].value = pInfo.EntityItemUseAddres;
        }
    }
    void OnSave(GameObject go)
    {
        if (m_UIInput[0].value.Length == 0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Ec_Change_Address_Failed"), 1);
            return;
        }
        if (m_UIInput[1].value.Length == 0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Ec_Change_Address_Failed1"), 1);
            return;
        }
        //string str = m_UIInput[0].value + m_UIInput[1].value;
        PlayerRole.Instance.EntityManager.ChangeRoleEntityEntityItemUseName(m_UIInput[0].value);
        PlayerRole.Instance.EntityManager.ChangeRoleEntityEntityItemUseAddress(m_UIInput[1].value);
    }
}