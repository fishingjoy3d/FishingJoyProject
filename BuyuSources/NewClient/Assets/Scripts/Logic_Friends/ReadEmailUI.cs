using UnityEngine;
using System.Collections.Generic;

//系统邮件所带附件
public struct SysMailAttachment
{
    public GameObject   AttachmentObj;      //附件对象
    public GameObject   AlreadyGet;         //附件已经对象
    public UISprite     AttachmentSprite;
    public UILabel      AttachmentName;
    public UILabel      AttachmentNum;      //附件数量
}

public class ReadEmailUI : BaseWnd
{

    GameObject[]            m_BtnObj = new GameObject[2];
    UILabel                 m_MailContent;
    //UILabel                 m_DateTime;
    //UILabel                 m_NickName;
    SysMailAttachment[]     m_PresentObj = new SysMailAttachment[3];
    EmailType               m_EmailType = EmailType.Sys_Email;
    uint                    m_PlayerID;
    uint                    m_MailID;

  
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.FriendSysObj[3]) as GameObject; ;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_MailContent = m_BaseTrans.GetChild(2).GetChild(0).GetComponent<UILabel>();
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnCloseMsg;
        for (byte i = 0; i < 2; ++i )
        {
            m_BtnObj[i] = m_BaseTrans.GetChild(2).GetChild(4 + i).gameObject;
            if (i == 0)
                UIEventListener.Get(m_BtnObj[i]).onClick = OnGetReward;
            else
                UIEventListener.Get(m_BtnObj[i]).onClick = OnCloseMsg;

        }

        if (m_EmailType == EmailType.Sys_Email)
        {
            for (int i = 0; i < 3; i++)
            {
                m_PresentObj[i].AttachmentObj = m_BaseTrans.GetChild(2).GetChild(i+1).gameObject;
                m_PresentObj[i].AttachmentSprite = m_BaseTrans.GetChild(2).GetChild(i+1).GetChild(0).GetComponent<UISprite>();
                m_PresentObj[i].AlreadyGet = m_BaseTrans.GetChild(2).GetChild(i+1).GetChild(1).gameObject;
                m_PresentObj[i].AttachmentName = m_BaseTrans.GetChild(2).GetChild(i + 1).GetChild(2).GetComponent<UILabel>();
                m_PresentObj[i].AttachmentNum = m_BaseTrans.GetChild(2).GetChild(i+1).GetChild(3).GetComponent<UILabel>();
            }
        }
    }
    void OnCloseMsg(GameObject go)
    {
        ShutDown();
    }
    void OnGetReward(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        PlayerRole.Instance.MailManager.GetSystemMailItem(m_MailID);
    }
	public void Update (float deltaTime) 
    {
	
	}
    public void ShowReadWndUI(bool bShow, uint mailID)
    {
        if (m_BaseWndObject == null)
            Init();

        WndManager.Instance.Push(m_BaseWndObject);
        ShowSysMailContext(mailID);
    }

    public void ShowSysMailContext(uint mailID)
    {
        List<tagNormalMailTitle> pNormalList = null;
        List<tagSystemMailTitle> pSystemList = null;
        if (!PlayerRole.Instance.MailManager.GetAllUserMail(ref pNormalList, ref pSystemList)) 
            return;


        tagSystemMail pSystemMail = PlayerRole.Instance.MailManager.GetSystemMailContext(mailID);
        if ( pSystemMail != null)
        {
            m_MailContent.text = ServerStringHandle.HandleServerString(pSystemMail.Context);
            ShowSysItem(mailID, pSystemMail);
        }
        m_MailID = mailID;

    }
    //系统邮件的附件物品
    void ShowSysItem(uint mailID, tagSystemMail pSystemMail)
    {
        ClearSysItemInfo();

        tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID(pSystemMail.RewardID);
        if (pReward == null)
        {
            m_BtnObj[1].SetActive(true);
           // FriendRuntime.EmailUI.SysMailGetItemEvent();    //无附件去掉领取按纽
            return;
        }
        for (byte i = 0; i < pReward.RewardItemVec.Count; ++i )
        {
            uint ItemID = pReward.RewardItemVec[i].ItemID;
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                continue;
            m_PresentObj[i].AttachmentObj.SetActive(true);
            uint itemSum = pReward.RewardItemVec[i].ItemSum*pSystemMail.RewardSum;
            if ( itemSum >= 10000)
                m_PresentObj[i].AttachmentNum.text = string.Format("{0}k", (itemSum / 1000) );
            else
                m_PresentObj[i].AttachmentNum.text = string.Format("{0}", itemSum);
            m_PresentObj[i].AttachmentSprite.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon;
            m_PresentObj[i].AttachmentName.text = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemName;
            m_BtnObj[0].SetActive(true);

        }
        if (!pSystemMail.bIsExistsReward)
        {
         //   FriendRuntime.EmailUI.SysMailGetItemEvent();    //无附件去掉领取按纽
            m_BtnObj[0].SetActive(false);
            m_BtnObj[1].SetActive(true);
            UpdateReceiveState();
        }
    }
    public void SetHeader(Texture2D tex)
    {
        //if (tex == null)
        //    m_FaceTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        //else
        //    m_FaceTexture.mainTexture = tex;
    }
    public void UpdateReceiveState()
    {
        for (byte i = 0; i < 3; ++i)
        {
            if (m_PresentObj[i].AttachmentObj.activeSelf == true)
            {
                m_PresentObj[i].AlreadyGet.SetActive(true);
                m_BtnObj[0].SetActive(false);
                m_BtnObj[1].SetActive(true);
            }
        }
    }
 
    void ClearSysItemInfo()
    {
        for (byte i = 0; i < 3; ++i)
        {
            if (m_PresentObj[i].AlreadyGet.activeSelf != false)
                m_PresentObj[i].AlreadyGet.SetActive(false);
            if (m_PresentObj[i].AttachmentObj.activeSelf != false)
                m_PresentObj[i].AttachmentObj.SetActive(false);
        }
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
