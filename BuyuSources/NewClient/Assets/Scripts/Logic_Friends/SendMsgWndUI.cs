/*
 * 该文件主要实现好友聊天信息框功能
*/
using UnityEngine;
using System.Collections.Generic;


public struct ChatInfo
{
    public byte   m_IsMySelf;
    public uint   m_UserID;
    public string m_DataTime;
    public string m_ChatStr;
}

public class SendMsgWndUI : BaseWnd
{
    GameObject          m_SendBtn;
    UIInput             m_InputContent;         //内容输入框
    UIButton            m_SendUIButton;
    UILabel             m_ChatName;
    UISprite            m_GenderSprite;
    Texture2D           m_FaceTex;
    UIGrid              m_UIGrid;
    GameObject          m_ItemObj;
    UIScrollBar         m_ScrollBar;
    uint                m_UserID;
    SystemTime          m_SysTime = new SystemTime();
    List<ChatInfo>      m_ChatLogList = new List<ChatInfo>();



    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.FriendSysObj[2]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        UIEventListener.Get(m_BaseTrans.GetChild(2).gameObject).onClick = OnCloseMsg;
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = OnOpenNameCardMsg;

        m_SendBtn = m_BaseTrans.GetChild(0).gameObject;
        UIEventListener.Get(m_SendBtn).onClick = OnSendMsg;
        m_SendUIButton = m_SendBtn.GetComponent<UIButton>();

        m_InputContent = m_BaseTrans.GetChild(3).gameObject.GetComponent<UIInput>();
        m_ChatName = m_BaseTrans.GetChild(4).gameObject.GetComponent<UILabel>();
        m_GenderSprite = m_BaseTrans.GetChild(5).gameObject.GetComponent<UISprite>();

        m_ItemObj = m_BaseTrans.GetChild(6).GetChild(0).gameObject;
        m_UIGrid = m_BaseTrans.GetChild(6).GetChild(1).GetComponent<UIGrid>();

        m_ScrollBar = m_BaseTrans.GetChild(7).GetComponent<UIScrollBar>();
    }

    public void ShowWnd(uint PlayerID, string strName, Texture2D tex,bool Gender)
    {
        if (m_BaseWndObject != null)
            return;
        m_ChatLogList.Clear();
        Init();
        WndManager.Instance.Push(m_BaseWndObject);
        m_InputContent.value = "";
        m_UserID = PlayerID;

        if (!Gender)
            m_GenderSprite.spriteName = "male";
        else
            m_GenderSprite.spriteName = "female";

        if (PlayerRole.Instance.RelationManager.GetFriendMap() != null && PlayerRole.Instance.RelationManager.GetFriendMap().ContainsKey(PlayerID) == true)
        {
            m_ChatName.text = PlayerRole.Instance.RelationManager.GetFriendMap()[PlayerID].GetNickName();
        }
        m_FaceTex = tex;

        LoadChatLogInfo();
      //  Debug.Log(m_ChatLogList.Count);
        CheckIsHaveNewMsg();
     //   Debug.Log(":: " + m_ChatLogList.Count.ToString());

        InitScrollView();
    }
    void CheckIsHaveNewMsg()
    {
        if (PlayerRole.Instance.RoleChar.GetCharList() == null)
            return ;
        foreach(KeyValuePair<uint, tagRoleCharInfo> map in PlayerRole.Instance.RoleChar.GetCharList())
        {
            if (map.Key == m_UserID)
            {
                PlayerRole.Instance.RoleChar.LoadAllCharInfoByUserID(m_UserID);
                return;
            }
        }
        UpdateChateInfo();
    }
    public void UpdateChateInfo()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        if (PlayerRole.Instance.RoleChar.m_CharArrayList.ContainsKey(m_UserID))
        {
            tagRoleCharArray pChat = PlayerRole.Instance.RoleChar.m_CharArrayList[m_UserID];
            for (byte i = 0; i < pChat.Array.Count; ++i)
            {
                AddMsg(pChat.Array[i]);
            }
            SaveChatLogInfo();
            InitScrollView();
            PlayerRole.Instance.RoleChar.ClearCharInfo(m_UserID);
        }
    }
    public bool AddNewMsg(tagRoleCharInfo pChat)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return false;
        if (pChat.SrcUserID != m_UserID)
            return false;
        AddMsg(pChat);
        SaveChatLogInfo();
        InitScrollView();
        PlayerRole.Instance.RoleChar.ClearCharInfo(m_UserID);
    //    Debug.Log("AddNews");
        return true;
    }
    void AddMsg(tagRoleCharInfo pChat)
    {
        ChatInfo item = new ChatInfo();
        item.m_ChatStr = pChat.MessageInfo;
        item.m_UserID = pChat.SrcUserID;
        System.DateTime dataTime = SystemTime.Instance.GetDateTimeByTotalSec(pChat.LogTime);
        string strTime = dataTime.Year.ToString() +"/"+ dataTime.Month.ToString("D2") +"/"+ dataTime.Day.ToString("D2")+"  "+ 
            dataTime.Hour.ToString("D2") +":"+ dataTime.Minute.ToString("D2");
        item.m_DataTime = strTime;
        item.m_IsMySelf = 0;
        //聊天记录达上限
        if (m_ChatLogList.Count > ConstValue.MAX_CHATLOG_NUM)
        {
            m_ChatLogList.RemoveAt(0);
        }
        m_ChatLogList.Add(item);
    }
    void AddMySelfNews(ChatInfo item)
    {
        //聊天记录达上限
        if (m_ChatLogList.Count > ConstValue.MAX_CHATLOG_NUM)
        {
            m_ChatLogList.RemoveAt(0);
        }
        m_ChatLogList.Add(item);
        SaveChatLogInfo();
        InitScrollView();
    }
    //聊天信息列表实例出来
    void InitScrollView()
    {
        ClearGridChild();
        for (byte i = 0; i < m_ChatLogList.Count; ++i)
        {
            ChatItemInfo item = new ChatItemInfo();
            item.Init(m_ItemObj);
            item.ShowInfo(m_ChatLogList[i], m_ChatName.text);
            m_UIGrid.AddChild(item.m_BaseTrans);
            item.m_BaseTrans.localScale = Vector3.one;
        }
        if (m_ChatLogList.Count > 3 && m_ScrollBar.value < 0.99f)
        {
            m_ScrollBar.value = 1;
        }
    }
    public void Update(float deltaTime)
    {
        if (m_BaseWndObject == null && m_BaseWndObject.activeSelf != true)
            return;
    }
   
    void OnCloseMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        ShutDown();
       // FriendRuntime.FriendSysMgr.SetWndMask(true);

    }
    void OnOpenNameCardMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Friend_Head, m_UserID, m_FaceTex, false, true);
    }
    void OnSendMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (m_InputContent.value.Length == 0)
            return;
        if (PlayerRole.Instance.RoleChar.SendMessaggChar(m_UserID, m_InputContent.value) == false)
            return;
        ChatInfo pInfo = new ChatInfo();
        pInfo.m_IsMySelf = 1;
        pInfo.m_ChatStr = m_InputContent.value;
        pInfo.m_UserID = PlayerRole.Instance.RoleInfo.RoleMe.GetUserID();
        pInfo.m_DataTime = System.DateTime.Now.Year.ToString() + "/" + System.DateTime.Now.Month.ToString("D2") + "/" +
            System.DateTime.Now.Day.ToString("D2") + "  " + System.DateTime.Now.Hour.ToString("D2") + ":" +
            System.DateTime.Now.Minute.ToString("D2");
        AddMySelfNews(pInfo);
        m_InputContent.value = "";
    }
    void ClearGridChild()
    {
        List<Transform> gridChid = m_UIGrid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_UIGrid.transform.DetachChildren();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
            m_ChatLogList.Clear();
        }
    }

    void LoadChatLogInfo()
    {
        byte[] data = RuntimeInfo.GetLocalChatFile(RuntimeInfo.GetChatLogFileName(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), m_UserID));
        if (data == null)
            return;
        System.IO.MemoryStream ms = new System.IO.MemoryStream(data);
        System.IO.BinaryReader br = new System.IO.BinaryReader(ms);
        uint logNum = br.ReadUInt32();
        for (int i = 0; i < logNum; ++i)
        {
            ChatInfo info = new ChatInfo();
            info.m_IsMySelf = br.ReadByte();
            info.m_UserID = br.ReadUInt32();
            info.m_DataTime = br.ReadString();
            info.m_ChatStr = br.ReadString();
            m_ChatLogList.Add(info);
        }

        ms.Close();
        br.Close();
    }

    void SaveChatLogInfo()
    {
        if (m_ChatLogList.Count == 0)
            return;

        System.IO.MemoryStream ms = new System.IO.MemoryStream();
        System.IO.BinaryWriter bw = new System.IO.BinaryWriter(ms);

        bw.Write(m_ChatLogList.Count);
        for (int i = 0; i < m_ChatLogList.Count; ++i)
        {
            bw.Write(m_ChatLogList[i].m_IsMySelf);
            bw.Write(m_ChatLogList[i].m_UserID);
            bw.Write(m_ChatLogList[i].m_DataTime);
            bw.Write(m_ChatLogList[i].m_ChatStr);
        }
        RuntimeInfo.SaveLocalChatFile(RuntimeInfo.GetChatLogFileName(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), m_UserID), ms.ToArray());
        bw.Close();
    }
}

/*聊天信息条目中包含的组件*/
struct ChatItem
{
    public UILabel      m_NameLabel;
    public UILabel      m_ContentLabel;
    public UISprite     m_ContentBg;
    public UILabel      m_TimeLabel;
}

/*该类功能是聊天框中玩家每一条信息处理*/
class ChatItemInfo : BaseWnd
{
    GameObject[]    m_Obj = new GameObject[2];
    ChatItem[]      m_ChatItem = new ChatItem[2];
    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseWndObject.SetActive(true);


        for (byte j = 0; j < m_ChatItem.Length; ++j)
        {
            m_Obj[j] = m_BaseTrans.GetChild(j).gameObject;
            m_ChatItem[j].m_NameLabel = m_BaseTrans.GetChild(j).GetChild(0).GetComponent<UILabel>();
            m_ChatItem[j].m_ContentLabel = m_BaseTrans.GetChild(j).GetChild(1).GetComponent<UILabel>();
            m_ChatItem[j].m_ContentBg = m_BaseTrans.GetChild(j).GetChild(2).GetComponent<UISprite>();
            m_ChatItem[j].m_TimeLabel = m_BaseTrans.GetChild(j).GetChild(3).GetComponent<UILabel>();
        }
    }
    public void ShowInfo(ChatInfo pInfo, string Name)
    {
        if (pInfo.m_IsMySelf == 1)
        {
            m_Obj[1].SetActive(true);
            Name = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();
        }
        else
        {
            m_Obj[0].SetActive(true);
        }
        m_ChatItem[pInfo.m_IsMySelf].m_NameLabel.text = Name;
        m_ChatItem[pInfo.m_IsMySelf].m_TimeLabel.text = pInfo.m_DataTime;
        m_ChatItem[pInfo.m_IsMySelf].m_ContentLabel.text = pInfo.m_ChatStr;
        m_ChatItem[pInfo.m_IsMySelf].m_ContentBg.width = m_ChatItem[pInfo.m_IsMySelf].m_ContentLabel.width + 30;
    }
}