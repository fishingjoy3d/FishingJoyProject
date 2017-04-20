using UnityEngine;
using System.Collections.Generic;

public class ForgetPassWord
{
    GameObject m_gownd;
    bool m_byPhone = true;
    GameObject m_goSelect;
    UIInput[] m_inputInfo = new UIInput[4];

    UIButton m_GetSecruityCodeBtn;
    UILabel m_SilderLabel;
    Transform m_SilderBtnTrans;
    Vector3 m_OrgPos;
    Vector3 m_EndPos;
    bool isCountCD = false;
    float cd_Time = 240.0f;
    bool m_BtnPress;
    float m_FristX;
    public void Init(Object obj,bool btIndex)
    {
        m_gownd=GameObject.Instantiate(obj) as GameObject;
        m_gownd.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        UIEventListener.Get(m_gownd.transform.GetChild(0).gameObject).onClick=EventClose;
        UIEventListener.Get(m_gownd.transform.GetChild(6).gameObject).onClick = EventAccount;
        UIEventListener.Get(m_gownd.transform.GetChild(7).gameObject).onClick = EventPhone;
        UIEventListener.Get(m_gownd.transform.GetChild(8).gameObject).onClick = EventSubmit;
        //UIEventListener.Get(m_gownd.transform.GetChild(1).GetChild(0).gameObject).onClick = OnGetSecruityCodePress;

        m_goSelect = m_gownd.transform.GetChild(9).gameObject;
        m_goSelect.transform.position = m_gownd.transform.GetChild(6).position;
        m_byPhone = btIndex;
        if (m_byPhone)
        {
            m_goSelect.transform.position = m_gownd.transform.GetChild(7).position;
        }

        m_inputInfo[0] = m_gownd.transform.GetChild(2).GetComponent<UIInput>();//号码
        m_inputInfo[1] = m_gownd.transform.GetChild(3).GetComponent<UIInput>();//验证码
        m_inputInfo[2] = m_gownd.transform.GetChild(4).GetComponent<UIInput>();//密码1
        m_inputInfo[3] = m_gownd.transform.GetChild(5).GetComponent<UIInput>();//密码2

        m_GetSecruityCodeBtn = m_gownd.transform.GetChild(1).GetChild(0).GetComponent<UIButton>();
        UIEventListener.Get(m_GetSecruityCodeBtn.gameObject).onPress = OnGetSecruityCodePress;

        m_SilderLabel = m_gownd.transform.GetChild(1).GetChild(1).GetComponent<UILabel>();
        m_SilderBtnTrans = m_GetSecruityCodeBtn.transform;

        m_FristX = m_SilderBtnTrans.position.x;
        m_OrgPos = m_SilderBtnTrans.position;
        m_EndPos = m_gownd.transform.GetChild(1).GetChild(2).position;
        


    }
    void EventClose(GameObject go)
    {
        ShutDown();
        LogonRuntime.LogonLogicUI.ChangeLogonWnd(LogonWnd_Status.LogonWnd_Init);
    }
    void EventAccount(GameObject go)
    {
        m_byPhone = false;
        m_goSelect.transform.position = go.transform.position;
    }
    void EventPhone(GameObject go)
    {
        m_byPhone = true;
        m_goSelect.transform.position = go.transform.position;
    }
    void EventSubmit(GameObject go)
    {
        if(m_inputInfo[0].value.Length==0)
        {
            return;
        }
        if (m_inputInfo[1].value.Length == 0)
        {
            return;
        }
        if (m_inputInfo[2].value.Length == 0)
        {
            return;
        }
        if (m_inputInfo[3].value.Length == 0)
        {
            return;
        }
        if (m_inputInfo[2].value != m_inputInfo[3].value)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("UOM_FetchPs_Notsame"), 1.5f);           
            return;
        }

        ulong nNum = 0;
        try
        {
            nNum = System.Convert.ToUInt64(m_inputInfo[0].value);
        }
        catch
        {

        }

        uint nCheckCode = 0;
        try
        {
            nCheckCode = System.Convert.ToUInt32(m_inputInfo[1].value);
        }
        catch
        {

        }

        LogonRuntime.LogonLogic._PhoneLogon(nNum, nCheckCode, m_inputInfo[2].value, !m_byPhone);

        //if(m_byBtnIndex==0)//帐号密码
        //{

           
        //    //_GetPhoneLogonCheck
        //}
        //else//手机密码
        //{

        //}
    }
    public void Update(float delta)
    {
        if (m_gownd)
        {
            if (isCountCD)
            {
                cd_Time -= delta;
                m_SilderLabel.text = string.Format(StringTable.GetString("Ec_BindPhone_GetCode"), (int)cd_Time);
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
        if (m_inputInfo[0].value.Length == 0)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("EC_BindPhone_PhoneIsEmpty"), 1.5f);
            m_SilderBtnTrans.position = m_OrgPos;
            m_FristX = 0;
            return;
        }
        ulong nNum = 0;
        try
        {
            nNum = System.Convert.ToUInt64(m_inputInfo[0].value);
        }
        catch
        {

        }
        if(LogonRuntime.LogonLogic._GetPhoneLogonCheck(nNum))       
        {
            m_FristX = 0;
            cd_Time = 240.0f;
            m_GetSecruityCodeBtn.isEnabled = false;
            m_SilderBtnTrans.position = m_EndPos;            
            isCountCD = true;
            m_SilderLabel.text = string.Format(StringTable.GetString("Ec_BindPhone_GetCode"), cd_Time);
        }
        else
        {
            m_SilderBtnTrans.position = m_OrgPos;
            m_FristX = 0;
        }
    }
    public void ShutDown()
    {
        if(m_gownd)
        {
            GameObject.Destroy(m_gownd);
            m_gownd = null;
        }
    }
}