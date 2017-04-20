using UnityEngine;
using System.Collections.Generic;

public class KnapsackShopWnd : BaseWnd
{
    Scroll[]    m_ScrollView = new Scroll[2];
    List<KnapsackItemInfo> m_KnapList = new List<KnapsackItemInfo>();
   // List<ShopItemInfoUI>    m_ShopItemInfoList = new List<ShopItemInfoUI>();
 

    public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = go.transform;

        m_ScrollView[0].m_BaseChild = m_BaseTrans.GetChild(0).GetChild(0).gameObject;
        m_ScrollView[0].m_Grid = new UIGrid[1];
        m_ScrollView[0].m_Grid[0] = m_BaseTrans.GetChild(0).GetChild(1).GetComponent<UIGrid>();

        m_ScrollView[1].m_Grid = new UIGrid[7];
        m_ScrollView[1].m_BaseChild = m_BaseTrans.GetChild(1).GetChild(0).gameObject;
        for (byte i = 0; i < 7; ++i)
        {
            m_ScrollView[1].m_Grid[i] = m_BaseTrans.GetChild(1).GetChild(1 + i).GetComponent<UIGrid>();
        }
    }
    //背包中物品发生变化 更新事件触发
    public void UpdateKnapsackDate()
    {
        ShowKnapsackItemInfo();
        ShowShopGoodsListInfo();
    }
    public void UpdateKnapsackLauncherDate()
    {
        ShowShopGoodsListInfo();
    }
    void ShowShopGoodsListInfo()
    {
        ClearShopGoodsGird();

        for (byte i = 0; i < (byte)LauncherType.LAUNCHER_MAX; ++i )
        {
            
                uint ItemID = LauncherSetting.LauncherDataList[i].nItemid;

                if (ItemID != 0 && FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) == false)
                    continue;
                tagItemConfig pItem = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID];

                ShopItemInfoUI item = new ShopItemInfoUI();
                item.Init(m_ScrollView[0].m_BaseChild);
                item.InitItemGetFlag();
                
               // item.InitItemLifeTime();
                if (PlayerRole.Instance.RoleLauncher.IsCanUseLauncher(i))
                    item.SetShopItemInfo(pItem.ItemIcon, pItem.ItemName, 0, true);
                else
                   item.SetShopItemInfo(pItem.ItemIcon, pItem.ItemName, FishConfig.Instance.m_VipConfig.GetVipLevelBuLauncherType(i), false);

                m_ScrollView[0].m_Grid[0].AddChild(item.m_BaseTrans);
                item.ResetLocalScale();
            
           
        }
    }
    //显示背包物品
    void ShowKnapsackItemInfo()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        ClearKnapsackGird();
        int Idx = 0;
        if (PlayerRole.Instance.ItemManager.GetAllItemMap() == null)
        {
            InitEmptyKnapsack(Idx, 14);
            return;
        }
        m_KnapList.Clear();
        foreach (KeyValuePair<uint, tagItemInfo> map in PlayerRole.Instance.ItemManager.GetAllItemMap())
        {
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(map.Value.ItemID) == false)
                continue;

            tagItemConfig pItemConfig = FishConfig.Instance.m_ItemInfo.m_ItemMap[map.Value.ItemID];
            if (pItemConfig.ItemTypeID == EItemType.IT_Cannon)
                continue;

            KnapsackItemInfo knapsack = new KnapsackItemInfo();
            knapsack.Init(m_ScrollView[1].m_BaseChild);
            string IconName = "";
            IconName = pItemConfig.ItemIcon;
            knapsack.SetItemInfo(map.Value.ItemID, PlayerRole.Instance.ItemManager.GetItemSum(map.Value.ItemID, false), IconName, 
                map.Value.ItemOnlyID, pItemConfig.ItemTypeID);
            m_KnapList.Add(knapsack);
        }
        m_KnapList.Sort(CompareItemByID);
        for (byte i = 0; i < m_KnapList.Count; ++i )
        {
            m_ScrollView[1].m_Grid[Idx % 7].AddChild(m_KnapList[i].m_BaseTrans);
            m_KnapList[i].ResetLoaclScale();
            ++Idx;
        }
        if (Idx < 14)
            InitEmptyKnapsack(Idx, 14);
        else
        {
            if ((Idx % 7) != 0)
                InitEmptyKnapsack(Idx, (Idx / 7 + 1) * 7);
        }
        m_KnapList.Clear();
    }
    static int CompareItemByID(KnapsackItemInfo item1, KnapsackItemInfo item2)
    {
        if (item1.ItemID < item2.ItemID)
            return -1;
        else if (item1.ItemID > item2.ItemID)
            return 1;
        else
            return 0;
    }
    void InitEmptyKnapsack(int Idx, int MaxIdx)
    {
        for (int i = Idx; i < MaxIdx; ++i)
        {
            KnapsackItemInfo knapsack = new KnapsackItemInfo();
            knapsack.Init(m_ScrollView[1].m_BaseChild);
            m_ScrollView[1].m_Grid[i % 7].AddChild(knapsack.m_BaseTrans);
            knapsack.ResetLoaclScale();
        }
    }
    void ClearShopGoodsGird()
    {
        List<Transform> gridChid = m_ScrollView[0].m_Grid[0].GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_ScrollView[0].m_Grid[0].transform.DetachChildren();
    }
    void ClearKnapsackGird()
    {
        for (byte i = 0; i < 7; ++i)
        {
            List<Transform> gridChid = m_ScrollView[1].m_Grid[i].GetChildList();

            foreach (Transform tr in gridChid)
            {
                GameObject.Destroy(tr.gameObject);
            }
            m_ScrollView[1].m_Grid[i].transform.DetachChildren();
        }
    }
}

class JBPSetting
{
    public static  bool ShowJBP(uint itemID)
    {
        if (itemID == 1700)
        {
            if (ServerSetting.ShowJBP == false || SDKMgr.Instance.IsOfficialVersion == false || PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() < 2)
                return false;
        }
        return true;
    }
}
public class PropertyShopWnd : BaseWnd
{
    Scroll                  m_ScrollView = new Scroll();
    byte                    m_ShopID;

    public void Init(GameObject go)
    {
        m_BaseTrans = go.transform;
        m_ScrollView.m_BaseChild = m_BaseTrans.GetChild(0).gameObject;
        m_ScrollView.m_Grid = new UIGrid[5];
        for (byte i = 0; i < 5; ++i)
            m_ScrollView.m_Grid[i] = m_BaseTrans.GetChild(i + 1).GetComponent<UIGrid>();
    }
    
    public void UpdatePropertyDate()
    {
        ClearGird();
        m_ShopID = (byte)(Shop_Type.Shop_Property + 1);
        if (FishConfig.Instance.m_ShopInfo.ShopMap.ContainsKey(m_ShopID) == false)
            return;
        tagShopConfig shopItemMap = FishConfig.Instance.m_ShopInfo.ShopMap[m_ShopID];
        if (shopItemMap == null)
            return;
        byte Idx = 0;
        foreach (KeyValuePair<byte, tagShopItemConfig> map in shopItemMap.ShopItemMap)
        {
            if (JBPSetting.ShowJBP(map.Value.ItemInfo.ItemID) == false)
                continue;

            ShopItemInfoUI item = new ShopItemInfoUI();
            item.Init(m_ScrollView.m_BaseChild);
            item.InitItemSum();
            item.SetShopItemInfo(m_ShopID, map.Value, shopItemMap.ShopItemStrMap[map.Key]);
            m_ScrollView.m_Grid[Idx % 5].AddChild(item.m_BaseTrans);
            item.ResetLocalScale();
            Idx++;
        }
    }
    void ClearGird()
    {
        for (byte i = 0; i < 5; ++i)
        {
            List<Transform> gridChid = m_ScrollView.m_Grid[i].GetChildList();

            foreach (Transform tr in gridChid)
            {
                GameObject.Destroy(tr.gameObject);
            }
            m_ScrollView.m_Grid[i].transform.DetachChildren();
        }
    }
}

public class DaPaoShopWnd : BaseWnd
{
    Scroll  m_ScrollView = new Scroll();
    byte    m_ShopID;

    public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = go.transform;
        m_ScrollView.m_BaseChild = m_BaseTrans.GetChild(0).gameObject;
        m_ScrollView.m_Grid = new UIGrid[1];
        m_ScrollView.m_Grid[0] = m_BaseTrans.GetChild(1).GetComponent<UIGrid>();        
    }
    public void UpdateDaPaoDate()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf == false)
            return;
        if (PlayerRole.Instance.ItemManager.GetAllItemMap() == null)
        {
            return;
        }
        ClearGird();
        m_ShopID = (byte)(Shop_Type.Shop_Launcher + 1);
        if (FishConfig.Instance.m_ShopInfo.ShopMap.ContainsKey(m_ShopID) == false)
            return;
        tagShopConfig shopItemMap = FishConfig.Instance.m_ShopInfo.ShopMap[m_ShopID];
        if (shopItemMap == null)
            return;
        foreach (KeyValuePair<byte, tagShopItemConfig> map in shopItemMap.ShopItemMap)
        {
            if (JBPSetting.ShowJBP(map.Value.ItemInfo.ItemID) == false)
                continue;

            ShopItemInfoUI item = new ShopItemInfoUI();
            item.Init(m_ScrollView.m_BaseChild);
            item.InitItemDesc();
            item.InitItemGetFlag();
            item.SetShopItemInfo(m_ShopID, map.Value, shopItemMap.ShopItemStrMap[map.Key]);
            if (IsAlreadyGetLauncher(map.Value.ItemInfo.ItemID, map.Value.ItemInfo.ItemSum))
                item.SetIsGetLauncher();
            m_ScrollView.m_Grid[0].AddChild(item.m_BaseTrans);
            item.ResetLocalScale();
        }
    }
    bool IsAlreadyGetLauncher(uint ItemID, uint ItemSum)
    {
        for (byte i = 0; i < (byte)LauncherType.LAUNCHER_MAX; ++i)
        {
            if (ItemID == LauncherSetting.LauncherDataList[i].nItemid
                && ItemSum == LauncherSetting.LauncherDataList[i].nItemcount)
            {
                if (PlayerRole.Instance.RoleLauncher.IsCanUseLauncherByAllTime(i))
                    return true;
            }
        }
        return false;
    }
    void ClearGird()
    {
        List<Transform> gridChid = m_ScrollView.m_Grid[0].GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_ScrollView.m_Grid[0].transform.DetachChildren();
    }
}

public class MaterailShopWnd : BaseWnd
{
    Scroll      m_ScrollView = new Scroll();
    //List<ShopItemInfoUI> m_ShopItemInfoList = new List<ShopItemInfoUI>();
    UITextList m_NoticeLabel;
    byte        m_ShopID;

    public void Init(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = go.transform;
        m_ScrollView.m_Grid = new UIGrid[3];
        m_ScrollView.m_BaseChild = m_BaseTrans.GetChild(0).GetChild(0).gameObject;
        for (byte i = 0; i < 3; ++i)
            m_ScrollView.m_Grid[i] = m_BaseTrans.GetChild(0).GetChild(i + 1).GetComponent<UIGrid>();
        m_NoticeLabel = m_BaseTrans.GetChild(1).GetComponent<UITextList>();
        UIEventListener.Get(m_BaseTrans.GetChild(2).gameObject).onClick = onClickChangeAddress;
    }
    public void UpateMaterailDate()
    {
        ClearGird();
        m_ShopID = (byte)(Shop_Type.Shop_Material + 1);

        if (FishConfig.Instance.m_ShopInfo.ShopMap.ContainsKey(m_ShopID) == false)
            return;
        tagShopConfig shopItemMap = FishConfig.Instance.m_ShopInfo.ShopMap[m_ShopID];
        if (shopItemMap == null)
            return;
        byte Idx = 0;
        foreach (KeyValuePair<byte, tagShopItemConfig> map in shopItemMap.ShopItemMap)
        {
            //对聚宝盆特殊处理
            //IsOfficialVersion true 官网
            if (JBPSetting.ShowJBP(map.Value.ItemInfo.ItemID) == false)
                continue;

            //if (SDKMgr.Instance.IsOfficialVersion == false && map.Value.ItemInfo.ItemID == 1700) //为渠道版
            //{
            //    continue;
            //}
            //else if (SDKMgr.Instance.IsOfficialVersion && PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() < 3)    //VIP等级小于3不显示
            //    continue;
            ShopItemInfoUI item = new ShopItemInfoUI();
            item.Init(m_ScrollView.m_BaseChild);
            item.InitItemSum();
            item.SetShopItemInfo(m_ShopID, map.Value, shopItemMap.ShopItemStrMap[map.Key]);
            m_ScrollView.m_Grid[Idx % 3].AddChild(item.m_BaseTrans);
            item.ResetLocalScale();
            Idx++;
        }
        tagRoleAddressInfo pEntity = PlayerRole.Instance.EntityManager.GetRoleEntityInfo();//获取一次实体数据
        UpdateNoticeDate();
    }
    public void Upate(float delta)
    {
        
    }
    public void UpdateNoticeDate()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        if (PlayerRole.Instance.RoleAnnouncement.GetAllAnnouncementList() == null)
            return;
        m_NoticeLabel.Clear();
        List<AnnouncementOnce> pNoticeList = PlayerRole.Instance.RoleAnnouncement.GetAllAnnouncementList();
        for (byte i = 0; i < pNoticeList.Count; ++i)
        {
            if (FishConfig.Instance.m_ShopInfo.ShopMap[pNoticeList[i].ShopID].ShopItemStrMap.ContainsKey(pNoticeList[i].ShopOnlyID) == false)
                continue;
            //m_NoticeLabel.Add("[ffcc00]Enter[-]" + i.ToString());
            string strName = FishConfig.Instance.m_ShopInfo.ShopMap[pNoticeList[i].ShopID].ShopItemStrMap[pNoticeList[i].ShopOnlyID].ItemName;
            string StrText = "[000000FF]" + "玩家  " + "[-]" + pNoticeList[i].NickName + "\n" + "[000000FF]" + "换取了  " + "[-]" + "[FF0000FF]" + strName + "[-]";
            m_NoticeLabel.Add(StrText);
        }
    }
    void onClickChangeAddress(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        GlobalHallUIMgr.Instance.ShowAccountMgrWnd(Acoount_MgrUI_Type.ReceiverAddress_UI);
        ShopRuntime.ShopLogicMgr.SetMaskEft(false);
        //if (HallRunTimeInfo.Instance_UI != null)
        //{
        //    HallRunTimeInfo.Login_UI.ShowBtnWindEffect(false);
        //}
    }
    void ClearGird()
    {
        for (byte i = 0; i < 3; ++i)
        {
            List<Transform> gridChid = m_ScrollView.m_Grid[i].GetChildList();

            foreach (Transform tr in gridChid)
            {
                GameObject.Destroy(tr.gameObject);
            }
            m_ScrollView.m_Grid[i].transform.DetachChildren();
        }
    }
}