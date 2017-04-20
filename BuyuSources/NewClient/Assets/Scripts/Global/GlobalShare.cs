using UnityEngine;
using System.Collections.Generic;

public enum ShareType
{
    SHARE_EXCHANGE,    
    SHARE_GOLD,    
}

public class  GameShare
{
    GameObject m_gownd;
    UITexture m_texturbg;
    GameObject m_goWndExchage;
    UISprite m_sprItemExchage;
    UILabel m_labelEcchageCount;

    GameObject m_goWndGold;
    UILabel m_labelWin;
    UILabel m_labelgametime;
    UILabel m_labelmaxwin;

    //
    UITexture m_avatar;
    UILabel m_labellv;
    UISprite m_sprvip;
    UILabel m_labelname;
    UILabel m_labeldate;
    //
    ShareType m_sharetype;

    int m_nGold;
    System.DateTime m_EnterTime;
    bool m_bshow=false;
    bool m_bSkipShow = false;
    void InIt()
    {
        //SystemTime.Instance.GetSystemDateTime
        m_gownd = GameObject.Instantiate(GlobalHallUIMgr.Instance.GlobalUIObject[5]) as GameObject;
        m_gownd.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_texturbg = m_gownd.transform.GetChild(0).GetComponent<UITexture>();
        UIEventListener.Get(m_gownd.transform.GetChild(1).gameObject).onClick = EventClose;
        UIEventListener.Get(m_gownd.transform.GetChild(2).gameObject).onClick = EventFriend;
        UIEventListener.Get(m_gownd.transform.GetChild(3).gameObject).onClick = EventFriends;
        m_goWndExchage = m_gownd.transform.GetChild(4).gameObject;       
        m_sprItemExchage = m_goWndExchage.transform.GetChild(3).GetComponent<UISprite>();
        m_labelEcchageCount = m_goWndExchage.transform.GetChild(4).GetChild(0).GetComponent<UILabel>();

        m_goWndGold = m_gownd.transform.GetChild(5).gameObject;
        m_labelWin = m_goWndGold.transform.GetChild(3).GetComponent<UILabel>();
        m_labelgametime = m_goWndGold.transform.GetChild(4).GetChild(0).GetComponent<UILabel>();
        m_labelmaxwin = m_goWndGold.transform.GetChild(5).GetChild(0).GetComponent<UILabel>();

        //
        m_goWndExchage.SetActive(m_sharetype == ShareType.SHARE_EXCHANGE);
        m_goWndGold.SetActive(m_sharetype == ShareType.SHARE_GOLD);

        //设置静态信息
        {
            Transform tfavatar = m_gownd.transform.GetChild(6);
            m_avatar=tfavatar.GetChild(0).GetComponent<UITexture>();
            m_labellv=tfavatar.GetChild(5).GetChild(0).GetComponent<UILabel>();
            m_sprvip = tfavatar.GetChild(6).GetComponent<UISprite>();
            m_labelname = tfavatar.transform.GetChild(3).GetComponent<UILabel>();
            m_labeldate = tfavatar.transform.GetChild(4).GetComponent<UILabel>();

            m_labeldate.text = SystemTime.Instance.GetSystemDateTime.ToShortDateString();//ToString("yyyy/MM/dd/HH:mm:ss");
            m_labelname.text = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();

            if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() > 0)
            {                
                m_sprvip.spriteName = string.Format("Vip_Icon_{0}", PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel());
            }
            else
            {
                m_sprvip.gameObject.SetActive(false);
            }
            m_labellv.text = PlayerRole.Instance.RoleInfo.RoleMe.GetLevel().ToString();


            uint ficeID = PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID();
            if (ficeID < ConstValue.CUSTOM_HEADER)
            {
                m_avatar.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
            }
            else
            {
                HeaderManager.Instance.GetPlayerHeader(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), ficeID, HeaderOptType.HEADER_GAME_SHARE, null);
            }
        }
        WndManager.Instance.Push(m_gownd);
    }
    public void SetAvatar(Texture2D texture)
    {
        m_avatar.mainTexture = texture;
    }
    public void ShowExchage(uint nItemid, uint nCount)
    {
        m_sharetype = ShareType.SHARE_EXCHANGE;
        InIt();
        m_sprItemExchage.spriteName = nItemid.ToString();
        m_labelEcchageCount.text = nCount.ToString()+"次";       
    }
      string NumtoString(uint ngold)
    {
        if (ngold<10000)
        {
            return ngold.ToString();
        }
        else
        {
            return ((ngold - ngold % 1000)/10000f).ToString() + "万";
        }         
    }
    public void ShowGold(uint nGold, uint nTime)
    {
        m_sharetype = ShareType.SHARE_GOLD;
        InIt();
        m_labelWin.text = NumtoString(nGold); 
        m_labelgametime.text = nTime.ToString()+"分钟";

        uint nMaxWin = 0;
        byte[] data = RuntimeInfo.GetLocalFile(RuntimeInfo.GameShareMaxWin());
        if (data!=null)//第一次
        {
            nMaxWin = System.BitConverter.ToUInt32(data, 0);
        }

        if (nGold > nMaxWin)
        {
            System.IO.MemoryStream ms = new System.IO.MemoryStream();
            System.IO.BinaryWriter bw = new System.IO.BinaryWriter(ms);
            bw.Write(nGold);
            RuntimeInfo.SaveLocalFile(RuntimeInfo.GameShareMaxWin(), ms.ToArray());
        }

        m_labelmaxwin.text = NumtoString(nMaxWin);
    }
    public void ShutDown()
   {
       WndManager.Instance.Pop(m_gownd);
       GameObject.Destroy(m_gownd);
       m_gownd = null;
   }
    public void EventClose(GameObject go)
    {
        ShutDown();
    }
    void EventFriend(GameObject go)
    {
        SceneMain.Instance.CaptureAndShare(true,m_texturbg);      
    }
    void EventFriends(GameObject go)
    {
        SceneMain.Instance.CaptureAndShare(false,m_texturbg);
    }
    public void EnterGame()
    {
        m_EnterTime = SystemTime.Instance.GetSystemDateTime;
        m_nGold = 0;
        m_bSkipShow = false;
    }
    public void SkipShow()
    {
        m_bSkipShow = true;
    }
    public void AddGlod(int nGold)
    {
        m_nGold += nGold;
    }
    public void LveaveGame()
    {
        m_bshow = true;
    }
    public void Update()
    {
        if (m_bshow)
        {
            if (m_nGold > 0 && !m_bSkipShow)
            {
                System.TimeSpan span = SystemTime.Instance.GetSystemDateTime - m_EnterTime;
                ShowGold((uint)m_nGold, (uint)(span.TotalSeconds + 59) / 60);
            }
            m_bshow = false;
            m_nGold = 0;
        }
    }
}