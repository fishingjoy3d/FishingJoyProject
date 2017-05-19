using UnityEngine;
using System.Collections.Generic;

public enum TreasurebowlType
{
    Treasurebowl_Present,
    Treasurebowl_Useing,
}

public class TreasurebowlLogic : BaseWnd
{
    UILabel         m_DescLabel;
    UILabel         m_SumLabel;
    UIButton[]      m_UIButton = new UIButton[3];
    uint            m_ItemSum;
    uint            m_ItemID;
    uint            m_ItemOltyID;
    uint            m_MaxSum;
    TreasurebowlInputWnd m_Treasurebowl = new TreasurebowlInputWnd();
    TreasurebowReceipt m_receipt = new TreasurebowReceipt();
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[5]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_DescLabel = m_BaseTrans.GetChild(6).GetComponent<UILabel>();
        m_SumLabel = m_BaseTrans.GetChild(5).GetComponent<UILabel>();

        for (byte i = 0; i < 5; ++i)
        {
            switch(i)
            {
                case 0:     //关闭按纽
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnCloseWnd;
                    break;
                case 1:     //赠送按纽
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnPresentMsg;
                    m_UIButton[2] = m_BaseTrans.GetChild(i).GetComponent<UIButton>();
                    break;
                case 2:     //使用按纽
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnUserMsg;
                    break;
                case 3:     //减
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnSubMsg;
                    m_UIButton[0] = m_BaseTrans.GetChild(i).GetComponent<UIButton>();
                    break;
                case 4:     //加
                    m_UIButton[1] = m_BaseTrans.GetChild(i).GetComponent<UIButton>();
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnAddMsg;
                    break;
            }
        }

        WndManager.Instance.Push(m_BaseTrans);
    }
    public void ShowRectipt(uint userid,uint itemid,uint uCount)
    {
        m_receipt.Show(m_Treasurebowl.Receiver(), itemid, uCount);
    }
    public void ShowTreasurebowl(uint ItemOnlyID, uint ItemID)
    {
        if (m_BaseWndObject == null)
            Init();
        //如果为渠道版本不能赠送
        if (SDKMgr.Instance.IsOfficialVersion == false)
            m_UIButton[2].gameObject.SetActive(false);
        //FishConfig.Instance.m_ItemInfo.m_ItemMap[1700].ItemIcon;
        m_UIButton[0].isEnabled = false;
        m_MaxSum = PlayerRole.Instance.ItemManager.GetItemSum(ItemID, false);
        if (m_MaxSum < 2)
            m_UIButton[1].isEnabled = false;
        m_ItemID = ItemID;
        m_ItemOltyID = ItemOnlyID;
        m_ItemSum = 1;
        m_SumLabel.text = m_ItemSum.ToString();
        if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(m_ItemID))
        {
            m_DescLabel.text = FishConfig.Instance.m_ItemInfo.m_ItemMap[m_ItemID].ItemDec;
        }


    }
    void OnCloseWnd(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        ShutDown();
    }
    void OnPresentMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() < 2)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("VIP_Present_Tips"), 1.5f);
            return;
        }
        m_Treasurebowl.ShowWnd(m_ItemOltyID, m_ItemSum, TreasurebowlType.Treasurebowl_Present);

    }

    void OnUserMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_Treasurebowl.ShowWnd(m_ItemOltyID, m_ItemSum, TreasurebowlType.Treasurebowl_Useing);
    }
    void OnSubMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_ItemSum--;
        if (m_ItemSum < 2)
        {
            m_UIButton[0].isEnabled = false;
            m_ItemSum = 1;
        }

        if (m_ItemSum < m_MaxSum)
            m_UIButton[1].isEnabled = true;
        m_SumLabel.text = m_ItemSum.ToString();

    }
    void OnAddMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_ItemSum++;
        if (m_ItemSum >= m_MaxSum)
        {
            m_ItemSum = m_MaxSum;
            m_UIButton[1].isEnabled = false;
        }
        if (m_ItemSum > 1)
            m_UIButton[0].isEnabled = true;
        m_SumLabel.text = m_ItemSum.ToString();

    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseTrans);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
}

public class TreasurebowlInputWnd : BaseWnd
{
    uint            m_ItemID;
    uint            m_ItemSum;
    uint            m_UserID;
    UIInput         m_UIInput;
    UILabel         m_TipsLabel;
    GameObject      m_InputObj;
    GameObject      m_TipsObj;
    TweenScale      m_ScaleAnim;
    TreasurebowlType    m_Type;
    public uint Receiver()
    {
        return m_UserID;
    }
    void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[6]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        m_ScaleAnim = m_BaseTrans.GetComponent<TweenScale>();
        m_InputObj = m_BaseTrans.GetChild(2).gameObject;
        m_TipsObj = m_BaseTrans.GetChild(3).gameObject;
        m_UIInput = m_BaseTrans.GetChild(2).GetChild(0).GetComponent<UIInput>();
        m_TipsLabel = m_BaseTrans.GetChild(3).GetChild(0).GetComponent<UILabel>();
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnCloseWnd;
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = OnOk;
        WndManager.Instance.Push(m_BaseTrans);

    }
    public void ShowWnd(uint ItemID, uint ItemSum, TreasurebowlType type)
    {
        if (m_BaseWndObject == null)
            Init();
        if (type == TreasurebowlType.Treasurebowl_Useing)
        {
            m_TipsObj.SetActive(true);
            m_InputObj.SetActive(false);
            m_TipsLabel.text = string.Format(StringTable.GetString("Treasurebowl_Tips1"), ItemSum);
        }

        PlayerRole.Instance.RelationManager.GetFriendMap();
        m_ItemSum = ItemSum;
        m_ItemID = ItemID;
        m_Type = type;
    }
    void OnOk(GameObject go)
    {
        if (m_InputObj.activeSelf == true)
        {
            //if (PlayerRole.Instance.RelationManager.GetFriendMap() == null)
            //{
            //    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Get_Relation_Field"), 1.5f);
            //    return;
            //}
            if(m_UIInput.value.Length<6)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Treasurebowl_InputId"), 1.5f);                
                return;
            }
            System.UInt32 GameID = 0;
            try
            {
               GameID = System.Convert.ToUInt32(m_UIInput.value);
            }
            catch (System.Exception ex)
            {
                GameID = 0;
            }
            if(GameID ==0)
            {
                GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Treasurebowl_InputId2"), 1.5f);
                return;
            }
            m_UserID = GameID;
            ShowTips();
            return;
            //foreach (KeyValuePair<uint, RelationRole> map in PlayerRole.Instance.RelationManager.GetFriendMap())
            //{
            //    //if (map.Value.GetNickName() == m_UIInput.value)
            //    if (map.Value.GetGameID() == GameID)
            //    {
            //        m_UserID = map.Value.GetUserID();
            //        ShowTips();
            //        return;
            //    }
            //}
            //GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Find_Relation_Field"), 1.5f);
        }
        else
        {
            if (m_Type == TreasurebowlType.Treasurebowl_Present)
                PlayerRole.Instance.ItemManager.OnAcceptItemByGameID(m_UserID, m_ItemID, m_ItemSum);
            else
                PlayerRole.Instance.ItemManager.OnUseItem(m_ItemID, m_ItemSum);
            ShutDown();
        }

    }
    void ShowTips()
    {
        m_InputObj.SetActive(false);
        m_TipsObj.SetActive(true);
        m_ScaleAnim.ResetToBeginning();
        m_ScaleAnim.PlayForward();
        m_TipsLabel.text = string.Format(StringTable.GetString("Treasurebowl_Tips2"), m_UIInput.value, m_ItemSum);
    }
    void OnCloseWnd(GameObject go)
    {
        ShutDown();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseTrans);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }         
}
public class TreasurebowReceipt
{
    GameObject m_goReceiptWnd;
    public void Show(uint ngameid, uint itemid, uint uCount)
    {
        
        m_goReceiptWnd = GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[8]) as GameObject;
        m_goReceiptWnd.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_goReceiptWnd.transform.GetChild(0).GetChild(1).GetChild(0).GetComponent<UILabel>().text = SystemTime.Instance.GetSystemDateTime.ToString("yyyy/MM/dd/HH:mm:ss");
        m_goReceiptWnd.transform.GetChild(1).GetChild(1).GetChild(0).GetComponent<UILabel>().text=PlayerRole.Instance.RoleInfo.RoleMe.GetGameID().ToString();
        m_goReceiptWnd.transform.GetChild(2).GetChild(1).GetChild(0).GetComponent<UILabel>().text = ngameid.ToString(); //PlayerRole.Instance.RelationManager.GetFriendMap()[userid].GetGameID().ToString();//userid.ToString();
        m_goReceiptWnd.transform.GetChild(3).GetChild(1).GetChild(0).GetComponent<UILabel>().text = "x"+uCount.ToString();
        UIEventListener.Get(m_goReceiptWnd.transform.GetChild(4).gameObject).onClick=EventClose; 
        WndManager.Instance.Push(m_goReceiptWnd);
    }
    void EventClose(GameObject go)
    {
        ShutDown();
    }
    public void ShutDown()
    {
        WndManager.Instance.Pop(m_goReceiptWnd);
        GameObject.Destroy(m_goReceiptWnd);
        m_goReceiptWnd = null;
    }
}
