using UnityEngine;
using System.Collections.Generic;

public class ThirdLogon : Singleton<ThirdLogon>
{
    public bool m_blogon=false;
}
public class LogonInitWnd
{
    GameObject      m_Handle;
    Transform       m_BaseTransform;
    GameObject[]    m_BtnObject = new GameObject[4];
    //Transform[]     m_BtnTrans = new Transform[2];
 //   GameObject      m_goEffect;

    GameObject      m_LabelObject;
    UILabel         m_UITips;

    GameObject      m_NoticeObj;
    Transform       m_NoticeTrans;
    GameObject      m_NoticeLabelItem;
    string          m_NoticeContent;   
    public void Init(Object obj)
    {
        if (m_Handle != null)
            return;
        ThirdLogon.Instance.m_blogon = false;

        m_Handle = GameObject.Instantiate(obj) as GameObject;
        m_BaseTransform = m_Handle.transform;
        m_BaseTransform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_NoticeTrans = m_BaseTransform.GetChild(6);
        m_NoticeObj = m_NoticeTrans.gameObject;
        m_NoticeLabelItem = m_NoticeTrans.GetChild(1).GetChild(0).gameObject;
        for (byte i = 0; i < m_BtnObject.Length; ++i)
        {
            m_BtnObject[i] = m_BaseTransform.GetChild(i).gameObject;
           // if (i > 0)
               // m_BtnTrans[i - 1] = m_BtnObject[i].transform;
            UIEventListener.Get(m_BtnObject[i]).onClick = OnBtnOnclickMsg;
        }
        m_LabelObject = m_BaseTransform.GetChild(7).gameObject;
        m_UITips = m_LabelObject.GetComponent<UILabel>();
        UIEventListener.Get(m_LabelObject).onClick = OnClickChannelLogon;
        UIEventListener.Get(m_BaseTransform.GetChild(4).gameObject).onClick = ContentServiceEvent;
        UIEventListener.Get(m_BaseTransform.GetChild(5).gameObject).onClick = ContentNoticeEvnet;
        UIEventListener.Get(m_NoticeTrans.GetChild(0).gameObject).onClick = CloseNoticeEvent;
        UIEventListener.Get(m_NoticeTrans.GetChild(2).GetChild(0).gameObject).onClick = BackNoticeEvent;


        ////return;
        //if (SDKMgr.IS_SDK_CHANNEL || ServerSetting.ShowThirdLoginBtn == false || RuntimeInfo.GetPlatform() == PlatformType.Windows)
        //{
        //    m_BaseTransform.GetChild(2).gameObject.SetActive(false);
        //    m_BaseTransform.GetChild(3).gameObject.SetActive(false);
        //}


     
        if (SDKMgr.IS_SDK_CHANNEL)
        {
            m_LabelObject.SetActive(true);
            m_UITips.text = StringTable.GetString("sdkchannel");

            for (byte i = 0; i < m_BtnObject.Length; ++i)
            {
                m_BtnObject[i].SetActive(false);
            }
            return;
        }
        else if (ServerSetting.ShowThirdLoginBtn == false || RuntimeInfo.GetPlatform() == PlatformType.Windows)//隐藏后面两个,并调整前面两个位置
        {
            m_BaseTransform.GetChild(2).gameObject.SetActive(false);
            m_BaseTransform.GetChild(3).gameObject.SetActive(false);

            m_BaseTransform.GetChild(0).position = Vector3.Lerp(m_BaseTransform.GetChild(0).position, m_BaseTransform.GetChild(1).position, 0.5f);
            m_BaseTransform.GetChild(1).position = Vector3.Lerp(m_BaseTransform.GetChild(2).position, m_BaseTransform.GetChild(3).position, 0.5f);
        }
        else
        {
            if (!m_BaseTransform.GetChild(3).gameObject.activeSelf)//根据第4个调整位置
            {
                m_BaseTransform.GetChild(1).position = Vector3.Lerp(m_BaseTransform.GetChild(0).position, m_BaseTransform.GetChild(3).position, 0.5f);
                m_BaseTransform.GetChild(2).position = m_BaseTransform.GetChild(3).position;
            }
        }


        //        m_BackgroundTex = m_BaseTransform.GetChild(3).GetComponent<UITexture>();
        //       m_BackgroundTex.mainTexture = SceneObjMgr.Instance.BackgroundTex;
        //effect 
        //Object objEffect = ResManager.Instance.LoadObject("UIEf_Start", "LogonRes/Effect/", ResType.LogonRes);
        //m_goEffect = GameObject.Instantiate(objEffect) as GameObject;
        //m_goEffect.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        if (GlobalLogon.Instance.AccountData != null)
        {
            //m_BtnObject[0].SetActive(false);
            //m_BtnTrans[1].localPosition = m_BtnTrans[0].localPosition;
            //m_BtnTrans[0].localPosition = new Vector3(m_BtnTrans[0].localPosition.x, m_BtnTrans[0].localPosition.y + 75, m_BtnTrans[0].localPosition.z);
        }
    }
    public void ShutDown()
    {
        //if (m_goEffect != null)
        //{
        //    GameObject.Destroy(m_goEffect);
        //    m_goEffect = null;
        //}
        if (m_Handle != null)
        {
            GameObject.Destroy(m_Handle);
            m_Handle = null;
        }
    }
    void OnBtnOnclickMsg(GameObject go)
    {
        ShutDown();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);    
        //快速登录按纽
       // return;
        if (go == m_BtnObject[0])//乐多
        {
            LogonRuntime.LogonLogicUI.ChangeLogonWnd(LogonWnd_Status.LogonWnd_Account);
            //GlobalEffectMgr.Instance.ShowLoadingMessage();
           // LogonRuntime.LogonLogic.GuestLogon();
        }
        else if (go == m_BtnObject[1])//手机
        {
            LogonRuntime.LogonLogicUI.ChangeLogonWnd(LogonWnd_Status.LogonWnd_Phone);
        }
        else if (go == m_BtnObject[2])//we chat
        {
            ThirdLogon.Instance.m_blogon = true;
            LogonRuntime.LogonLogic.OnBeginWeiXinLogon();
        }
        else if (go == m_BtnObject[3])//QQ
        {
            ThirdLogon.Instance.m_blogon = true;
            LogonRuntime.LogonLogic.OnBeginQQLogon();
        }

        ////正常的账号登录
        //else if (go == m_BtnObject[1])
        //{
        //    if (GlobalLogon.Instance.AccountData != null && GlobalLogon.Instance.AccountData.TAG == 1)
        //    {
        //        //玩家为游客身份进入
        //        GlobalEffectMgr.Instance.ShowLoadingMessage();
        //        LogonRuntime.LogonLogic.GuestLogon();
        //    }
        //    else
        //        LogonRuntime.LogonLogicUI.ChangeLogonWnd(LogonWnd_Status.LogonWnd_Account);
        //}
        ////快速注册
        //else
        //{
        //    LogonRuntime.LogonLogicUI.ChangeLogonWnd(LogonWnd_Status.LogonWnd_Resgiter);
        //}
        

    }
    void ContentServiceEvent(GameObject go)
    {
        //GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        //NativeInterface.OpenWeb(ServerSetting.SERVICES_URL);
        ContentNoticeEvnet(go);
    }
    void ContentNoticeEvnet(GameObject go)
    {
        WndManager.Instance.Push(m_NoticeTrans);
        m_NoticeObj.SetActive(true);
        if (go == m_BaseTransform.GetChild(5).gameObject)
        {
            //打开公告
            Announcement.GetAnnouncement();
            m_NoticeTrans.GetChild(1).gameObject.SetActive(true);
            m_NoticeTrans.GetChild(2).gameObject.SetActive(false);
            m_NoticeTrans.GetChild(3).gameObject.SetActive(false);
            m_NoticeTrans.GetChild(5).GetChild(0).gameObject.SetActive(true);
        }
        else 
        {
            //打开联系客服
            m_NoticeTrans.GetChild(2).gameObject.SetActive(false);
            m_NoticeTrans.GetChild(3).gameObject.SetActive(true);
            m_NoticeTrans.GetChild(5).GetChild(0).gameObject.SetActive(false);
        }
    }

    public void UpdateNoticeDate(List<AnnouncementData> annoucesList)
    {
        m_NoticeTrans.GetChild(3).gameObject.SetActive(false);
        m_NoticeTrans.GetChild(2).gameObject.SetActive(false);
        for (byte i = 0; i < annoucesList.Count; i++)
        {
            AnnouncementData data = new AnnouncementData();
            data.Title = annoucesList[i].Title;
            data.Date = annoucesList[i].Date;
            data.Content = annoucesList[i].Content;
            GameObject go = GameObject.Instantiate(m_NoticeLabelItem);
            m_NoticeTrans.GetChild(1).GetChild(1).GetComponent<UIGrid>().AddChild(go.transform);
            go.GetComponent<UILabel>().text = data.Title + " " + data.Date;
            go.transform.localScale = Vector3.one;
            NoticeItem item = new NoticeItem(go, data.Content, m_NoticeTrans);
            go.SetActive(true);
        }
    }

    void BackNoticeEvent(GameObject go)
    {
        m_NoticeTrans.GetChild(2).GetComponent<UILabel>().text = "";
        m_NoticeTrans.GetChild(2).gameObject.SetActive(false);
        m_NoticeTrans.GetChild(1).gameObject.SetActive(true);
    }
  
    void ClearNotice()
    {
        List<Transform> gridChid = m_NoticeTrans.GetChild(1).GetChild(1).GetComponent<UIGrid>().GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_NoticeTrans.GetChild(1).GetChild(1).DetachChildren();
    }
    void CloseNoticeEvent(GameObject go)
    {
        ClearNotice();
        m_NoticeTrans.GetChild(2).GetComponent<UILabel>().text = "";
        m_NoticeTrans.GetChild(2).gameObject.SetActive(false);
        WndManager.Instance.Pop(m_NoticeTrans);
        m_NoticeObj.SetActive(false);
    }
    void OnClickChannelLogon(GameObject go)
    {
        if (SDKMgr.IS_SDK_CHANNEL)
        {
            if (LogonRuntime.LogonLogic.IsWaitLogon)
            {
                //点击登录
                m_LabelObject.SetActive(false);
                LogonRuntime.LogonLogic.SDKLogin();
            }
        }
    }
    public void IsShowLabel(bool IsShow)
    {
        m_LabelObject.SetActive(IsShow);
    }
}

class NoticeItem : BaseWnd
{
    GameObject Label;
    string m_Content;

    public NoticeItem(GameObject go, string content, Transform baseTrans)
    {
        Label = go;
        m_Content = content;
        m_BaseTrans = baseTrans;
        UIEventListener.Get(Label).onClick = EventShowLabel;
    }

    void EventShowLabel(GameObject go)
    {
        m_BaseTrans.GetChild(1).gameObject.SetActive(false);
        m_BaseTrans.GetChild(2).GetComponent<UILabel>().text = m_Content;
        m_BaseTrans.GetChild(2).gameObject.SetActive(true);
    }
}