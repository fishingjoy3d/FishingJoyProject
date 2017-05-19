using UnityEngine;
using System.Collections.Generic;

public class KnapsackItemInfo   : BaseWnd
{
    UISprite        m_ItemIcon;
    UILabel         m_ItemNum;
    uint            m_ItemID = 0;
    uint            m_ItemOnlyID;
    EItemType       m_ItemType;

    public uint ItemID
    {
        get { return m_ItemID; }
    }
    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);
        m_ItemIcon = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_ItemNum = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        UIEventListener.Get(m_BaseWndObject).onClick = OnShowDescTips;
    }

    public void ResetLoaclScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void SetItemInfo(uint itemID, uint itemNum, string iconName, uint OnlyItemID, EItemType type)
    {
        m_ItemIcon.gameObject.SetActive(true);
        m_ItemNum.gameObject.SetActive(true);
        m_ItemID = itemID;
        m_ItemOnlyID = OnlyItemID;
        m_ItemType = type;
        m_ItemIcon.spriteName = iconName;
        m_ItemNum.text = itemNum.ToString();
    }
    void OnShowDescTips(GameObject go)
    {
        if (m_ItemID == 0)
            return;
        if (m_ItemType == EItemType.IT_GlobelBag)
        {
            ShopRuntime.ShopLogicMgr.TreasurebowlWnd.ShowTreasurebowl(m_ItemOnlyID, m_ItemID);
        }
        else if (m_ItemID == FishConfig.Instance.m_SystemConfig.SendMessageItemID)
        {
            ShopRuntime.ShopLogicMgr.BugleWnd.Show();
        }
        else
        {
            ShopRuntime.PropertyDescUI.ShowPropertyDescWnd(m_ItemID, m_ItemOnlyID);
        }
           
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
    }
}


public class PropertyDescWnd : BaseWnd
{
    GameObject      m_OpenGiftBtn;
    UILabel         m_ItemName;
    UILabel         m_ItemDesc;
    UISprite        m_ItemIcon;
    uint            m_ItemID;
    uint            m_ItemOnltyID;

    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[4]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_BaseWndObject.SetActive(true);
        m_OpenGiftBtn = m_BaseTrans.GetChild(1).gameObject;
        m_ItemName = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        m_ItemDesc = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
        m_ItemIcon = m_BaseTrans.GetChild(4).GetComponent<UISprite>();

        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnClickClose;
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = OnClickOpenGift;
    }
    public void ShowPropertyDescWnd(uint ItemID, uint ItemOnltyID)
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseWndObject);
        }
        m_ItemID = ItemID;
        m_ItemOnltyID = ItemOnltyID;
        if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(m_ItemID) == false)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Item_No_Exist"), 0.5f, false);
            ShutDown();
            return;
        }
        tagItemConfig pItemConfig = FishConfig.Instance.m_ItemInfo.m_ItemMap[m_ItemID];
        m_ItemDesc.text = pItemConfig.ItemDec;
        m_ItemName.text = pItemConfig.ItemName;
        m_ItemIcon.spriteName = pItemConfig.ItemIcon;
        if (PlayerRole.Instance.ItemManager.IsCanUserItem(m_ItemID))
            m_OpenGiftBtn.SetActive(true);

    }
    void OnClickClose(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        ShutDown();
    }
    void OnClickOpenGift(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        PlayerRole.Instance.ItemManager.OnUseItem(m_ItemOnltyID, 1);
        ShutDown();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
        }
    }
}
