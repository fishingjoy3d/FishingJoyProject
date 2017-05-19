using UnityEngine;
using System.Collections.Generic;

public enum ShopItem_Type
{
    All_Item,
    Entity_Item,
    Virtual_Item,
}
/*

public class ExchangeLogicUI : BaseWnd
{

  //  ExchangeShopBtns[]      m_LeftBtns = new ExchangeShopBtns[4];
    Scroll                  m_ScrollView = new Scroll();
    List<ShopItemInfoUI>    m_ShopItemInfoList = new List<ShopItemInfoUI>();
    ShopItem_Type           m_ShopItemType = ShopItem_Type.All_Item;

	public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = m_BaseWndObject.transform;
        Transform baseT = m_BaseTrans.GetChild(1);
        for (byte i = 0; i < baseT.childCount; ++i)
        {
            m_LeftBtns[i].m_Btn = baseT.GetChild(i).GetComponent<UIButton>();
            if (i < 3)
                m_LeftBtns[i].m_btnBg = m_LeftBtns[i].m_Btn.gameObject.transform.GetChild(0).GetComponent<UISprite>();
            m_LeftBtns[i].m_IsChecked = false;
            m_LeftBtns[i].m_Tag = (byte)(222 + i);
            UIEventListener.Get(m_LeftBtns[i].m_Btn.gameObject).onClick = OnCheckLeftBtns;
        }
        m_LeftBtns[0].m_IsChecked = true;
        InitScroll();

    }
    void InitScroll()
    {
        m_ScrollView.m_Grid = new UIGrid[4];
        Transform BaseT = m_BaseTrans.GetChild(0);
        m_ScrollView.m_BaseScroll = BaseT.GetComponent<UIScrollView>();
        m_ScrollView.m_BaseChild = BaseT.GetChild(0).gameObject;
        if (m_ScrollView.m_BaseChild.activeSelf != false)
            m_ScrollView.m_BaseChild.SetActive(false);
        for (int i = 0; i < 4; ++i)
            m_ScrollView.m_Grid[i] = BaseT.GetChild(i + 1).GetComponent<UIGrid>();

        SetScrollViewInfo(ShopItem_Type.All_Item);
    }
    void InitShopItem(tagShopItemConfig ItemInfo, tagShopItemStr ItemStr)
    {
        ShopItemInfoUI item = new ShopItemInfoUI();
        item.Init(m_ScrollView.m_BaseChild);
        item.SetShopItemInfo(1, ItemInfo, ItemStr);
        m_ShopItemInfoList.Add(item);
    }
    void InitShopItemList()
    {
        for (int i = 0; i < m_ShopItemInfoList.Count; ++i)
        {
            m_ScrollView.m_Grid[i % 4].AddChild(m_ShopItemInfoList[i].m_BaseTrans);
            m_ShopItemInfoList[i].ResetLocalScale();
        }
    }
    public void ShowExchangeLogicUI(bool bShow)
    {
        if (m_BaseWndObject.activeSelf != bShow)
            m_BaseWndObject.SetActive(bShow);
    }
    void SetScrollViewInfo(ShopItem_Type type)
    {

        m_ShopItemType = type;

        m_ShopItemInfoList.Clear();
        if (FishConfig.Instance.m_ShopInfo.ShopMap.ContainsKey(1) == false)
            return;
        tagShopConfig shopItemMap = FishConfig.Instance.m_ShopInfo.ShopMap[1];
        if (type == ShopItem_Type.All_Item)
        {
            foreach (KeyValuePair<byte, tagShopItemConfig> map in shopItemMap.ShopItemMap)
            {
                InitShopItem(map.Value, shopItemMap.ShopItemStrMap[map.Key]);
            }
        }
        else if (type == ShopItem_Type.Entity_Item)
        {
            foreach (KeyValuePair<byte, tagShopItemConfig> map in shopItemMap.ShopItemMap)
            {
                if (map.Value.IsEntityItem == true)
                    InitShopItem(map.Value, shopItemMap.ShopItemStrMap[map.Key]);
            }
        }
        else
        {
            foreach (KeyValuePair<byte, tagShopItemConfig> map in shopItemMap.ShopItemMap)
            {
                if (map.Value.IsEntityItem == false)
                    InitShopItem(map.Value, shopItemMap.ShopItemStrMap[map.Key]);
            }
        }

        InitShopItemList();
    }
    void OnCheckLeftBtns(GameObject go)
    {
        //if (!ShopRuntime.ShopLogicMgr.bMaskWnd)
        //    return;
        int CheckTag = 0;
        for (int i = 0; i < m_LeftBtns.Length; ++i)
        {
            if (m_LeftBtns[i].m_Btn.gameObject == go)
            {
                m_LeftBtns[i].m_IsChecked = true;
                CheckTag = m_LeftBtns[i].m_Tag;
            }
            else
            {
                m_LeftBtns[i].m_IsChecked = false;
            }
        }
        switch (CheckTag)
        {
            case 222:       //全部物品
                {
                    if (m_ShopItemType == ShopItem_Type.All_Item)
                        return;
                    ClearGird();
                    SetScrollViewInfo(ShopItem_Type.All_Item);
                    ExchangeLeftBtnBg();
                }
                break;
            case 223:       //实体物品
                {
                    if (m_ShopItemType == ShopItem_Type.Entity_Item)
                        return;
                    ClearGird();
                    SetScrollViewInfo(ShopItem_Type.Entity_Item);
                    ExchangeLeftBtnBg();
                }
                break;
            case 224:       //虚拟物品
                {
                    if (m_ShopItemType == ShopItem_Type.Virtual_Item)
                        return;
                    ClearGird();
                    SetScrollViewInfo(ShopItem_Type.Virtual_Item);
                    ExchangeLeftBtnBg();
                }
                break;
            case 225:       //收货地址
                {
                    GlobalHallUIMgr.Instance.ShowAccountMgrWnd(Acoount_MgrUI_Type.ReceiverAddress_UI);
                    ShopRuntime.ShopLogicMgr.SetMaskEft(false);
                    if (HallRunTimeInfo.Instance_UI != null)
                    {
                        HallRunTimeInfo.Login_UI.ShowBtnWindEffect(false);
                    }
                } 
                break;
        }
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void ExchangeLeftBtnBg()
    {
        for (int i = 0; i < m_LeftBtns.Length - 1; ++i)
        {
            if (m_LeftBtns[i].m_IsChecked)
            {
                m_LeftBtns[i].m_btnBg.spriteName = "LeftBtn_Check";
            }
            else
            {
                m_LeftBtns[i].m_btnBg.spriteName = "LeftBtn_UnCheck";
            }
        }
    }
    void ClearGird()
    {
        for (byte i = 0; i < 4; ++i)
        {
            List<Transform> gridChid = m_ScrollView.m_Grid[i].GetChildList();

            foreach (Transform tr in gridChid)
            {
                GameObject.Destroy(tr.gameObject);
            }
            m_ScrollView.m_Grid[i].transform.DetachChildren();
        }
    }
    public void ShutDown()
    {
        m_ShopItemInfoList.Clear();
        ClearGird();
    }
}
*/