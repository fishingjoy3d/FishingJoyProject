using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//玩家的物品管理
class RoleItem  //玩家的物品背包
{
    public RoleItem()
    {
        OnClear();
    }
    private bool m_IsLoadItem = false;
    private bool m_IsLoadByServer = false;
    //Key 为ItemOnlyID
    private Dictionary<UInt32, tagItemInfo> ItemMap = new Dictionary<UInt32, tagItemInfo>();
    //Key 为ItemID
    private Dictionary<UInt32, UInt32> ItemCountMap = new Dictionary<UInt32, UInt32>();
    public void OnClear()
    {
        m_IsLoadItem = false;
        m_IsLoadByServer = false;
        ItemMap.Clear();
        ItemCountMap.Clear();
    }
    public void ResetInfo()
    {
        OnClear();
        tagItemChangeEvent Event = new tagItemChangeEvent();
        MsgEventHandle.HandleMsg(Event);
    }
    //public void ItemTest()
    //{
    //    tagItemInfo pItem1 = new tagItemInfo();
    //    pItem1.ItemID = 1;
    //    pItem1.ItemSum = 100;
    //    pItem1.ItemOnlyID = 1;
    //    pItem1.EndTime = 0;
    //    ItemMap.Add(pItem1.ItemOnlyID, pItem1);
    //    ItemCountMap.Add(pItem1.ItemID, pItem1.ItemSum);

    //    tagItemInfo pItem2 = new tagItemInfo();
    //    pItem2.ItemID = 2;
    //    pItem2.ItemSum = 100;
    //    pItem2.ItemOnlyID = 1;
    //    pItem2.EndTime = 0;
    //    ItemMap.Add(pItem2.ItemOnlyID, pItem2);
    //    ItemCountMap.Add(pItem2.ItemID, pItem2.ItemSum);

    //    tagItemInfo pItem3 = new tagItemInfo();
    //    pItem3.ItemID = 3;
    //    pItem3.ItemSum = 100;
    //    pItem3.ItemOnlyID = 1;
    //    pItem3.EndTime = 0;
    //    ItemMap.Add(pItem3.ItemOnlyID, pItem3);
    //    ItemCountMap.Add(pItem3.ItemID, pItem3.ItemSum);

    //    m_IsLoadItem = true;

    //    tagItemChangeEvent Event = new tagItemChangeEvent();
    //    MsgEventHandle.HandleMsg(Event);
    //}
    public Dictionary<UInt32, tagItemInfo> GetAllItemMap()
    {
        if(m_IsLoadItem)
        {
            return ItemMap;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                ItemMap.Clear();
                ItemCountMap.Clear();
                SendGetAllUserItem();
            }
            //ItemTest();
            return null;
        }
    }
    public bool IsCanUserItem(UInt32 ItemID)
    {
        tagItemConfig pConfig;
        if (!FishConfig.Instance.m_ItemInfo.m_ItemMap.TryGetValue(ItemID, out pConfig))
            return false;
        if (pConfig.ItemTypeID != EItemType.IT_Package && pConfig.ItemTypeID != EItemType.IT_GlobelBag &&  pConfig.ItemTypeID != EItemType.IT_MonthCard)
            return false; 
        return true;
    }
    public bool IsCanAcceptItem(UInt32 ItemID)
    {
        tagItemConfig pConfig;
        if (!FishConfig.Instance.m_ItemInfo.m_ItemMap.TryGetValue(ItemID, out pConfig))
            return false;
        if (pConfig.ItemTypeID != EItemType.IT_GlobelBag)
            return false;
        return true;
    }
    public bool OnUseItem(UInt32 ItemOnlyID,UInt32 ItemSum)
    {
        //使用指定物品
        if (ItemSum == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_UseItem_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        tagItemInfo pInfo;
        if (!ItemMap.TryGetValue(ItemOnlyID, out pInfo))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_UseItem_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (pInfo.ItemOnlyID != ItemOnlyID || pInfo.ItemSum < ItemSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_UseItem_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        tagItemConfig pConfig;
        if (!FishConfig.Instance.m_ItemInfo.m_ItemMap.TryGetValue(pInfo.ItemID, out pConfig))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_UseItem_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (pConfig.ItemTypeID != EItemType.IT_Package && pConfig.ItemTypeID != EItemType.IT_MonthCard && pConfig.ItemTypeID != EItemType.IT_GlobelBag)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_UseItem_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(pConfig.ItemTypeID == EItemType.IT_MonthCard && ItemSum >1)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_UseItem_Failed_6);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(pConfig.ItemTypeID ==  EItemType.IT_MonthCard && PlayerRole.Instance.RoleMonthCard.IsInMonthCard())
        {
            //在月卡中 不可以在使用月卡了
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_UseItem_Failed_7);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_OnUseItem ncb = new CL_Cmd_OnUseItem();
        ncb.SetCmdType(NetCmdType.CMD_CL_OnUseItem);
        ncb.ItemOnlyID = ItemOnlyID;
        ncb.ItemID = pInfo.ItemID;
        ncb.ItemSum = ItemSum;
        NetServices.Instance.Send<CL_Cmd_OnUseItem>(ncb);
        return true;
    }
    public bool OnAcceptItemByGameID(UInt32 GameID,UInt32 ItemOnlyID,UInt32 ItemSum)
    {
        if (ItemSum == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        tagItemInfo pInfo;
        if (!ItemMap.TryGetValue(ItemOnlyID, out pInfo))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (pInfo.ItemOnlyID != ItemOnlyID || pInfo.ItemSum < ItemSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        tagItemConfig pConfig;
        if (!FishConfig.Instance.m_ItemInfo.m_ItemMap.TryGetValue(pInfo.ItemID, out pConfig))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (pConfig.ItemTypeID != EItemType.IT_GlobelBag)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_OnAcceptItem ncb = new CL_Cmd_OnAcceptItem();
        ncb.SetCmdType(NetCmdType.CMD_CL_OnAcceptItem);
        ncb.ItemOnlyID = ItemOnlyID;
        ncb.ItemID = pInfo.ItemID;
        ncb.ItemSum = ItemSum;
        ncb.dwID = GameID;
        ncb.GameIDorUserID = true; 
        NetServices.Instance.Send<CL_Cmd_OnAcceptItem>(ncb);
        return true;
    }
    //public bool OnAcceptItem(UInt32 DestUserID,UInt32 ItemOnlyID,UInt32 ItemSum)
    //{
    //    //RelationRole pFriend = PlayerRole.Instance.RelationManager.QueryRelationRole(DestUserID);
    //    //if (pFriend == null || pFriend.GetRelationType() != 1)//RT_Friend
    //    //{ 
    //    //    //指定好友不存在
    //    //    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_6);
    //    //    MsgEventHandle.HandleMsg(pUOM);
    //    //    return false;  
    //    //}
    //    //赠送指定物品给指定玩家
    //    if (ItemSum == 0)
    //    {
    //        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_1);
    //        MsgEventHandle.HandleMsg(pUOM);
    //        return false;
    //    }
    //    tagItemInfo pInfo;
    //    if (!ItemMap.TryGetValue(ItemOnlyID, out pInfo))
    //    {
    //        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_2);
    //        MsgEventHandle.HandleMsg(pUOM);
    //        return false;
    //    }
    //    if (pInfo.ItemOnlyID != ItemOnlyID || pInfo.ItemSum < ItemSum)
    //    {
    //        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_3);
    //        MsgEventHandle.HandleMsg(pUOM);
    //        return false;
    //    }
    //    tagItemConfig pConfig;
    //    if (!FishConfig.Instance.m_ItemInfo.m_ItemMap.TryGetValue(pInfo.ItemID, out pConfig))
    //    {
    //        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_2);
    //        MsgEventHandle.HandleMsg(pUOM);
    //        return false;
    //    }
    //    if (pConfig.ItemTypeID != EItemType.IT_GlobelBag)
    //    {
    //        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Item_AcceptItem_Failed_4);
    //        MsgEventHandle.HandleMsg(pUOM);
    //        return false;
    //    }
    //    CL_Cmd_OnAcceptItem ncb = new CL_Cmd_OnAcceptItem();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_OnAcceptItem);
    //    ncb.ItemOnlyID = ItemOnlyID;
    //    ncb.ItemID = pInfo.ItemID;
    //    ncb.ItemSum = ItemSum;
    //    ncb.dwID = DestUserID;
    //    ncb.GameIDorUserID = false; 
    //    NetServices.Instance.Send<CL_Cmd_OnAcceptItem>(ncb);
    //    return true;
    //}
    public bool GetItemEndTime(UInt32 ItemID, UInt32 ItemSum,out UInt32 EndSec)
    {
        //ItemID ItemSum == 0
        EndSec = 0;
        if (ItemID == 0 || !ItemCountMap.ContainsKey(ItemID) || ItemSum == 0 || ItemCountMap[ItemID] < ItemSum)
            return false;
        else
        {
            //数量够了 我们获取时间
            //向获取集合 然后进行排序 无限制 限制最长  限制最短的 排序
            List<tagItemInfo> pList = new List<tagItemInfo>();
            foreach(tagItemInfo var in ItemMap.Values)
            {
                if (var.ItemID != ItemID)
                    continue;
                pList.Add(var);
            }
            pList.Sort(SortCompareIteminfo);
            //排序完毕后 我们进行处理
            UInt32 NeedSum = ItemSum;
            foreach(tagItemInfo var in pList)
            {
                if(var.ItemSum >= NeedSum)
                {
                    //返回持续时间
                    if (var.EndTime == 0)
                    {
                        EndSec = 0;
                        return true;
                    }
                    else
                    {
                        if (var.EndTime < SystemTime.Instance.GetSystemTimeTotalSecond())
                        {
                            //特殊情况 
                            EndSec = 60;
                            return true;
                        }
                        EndSec =  Convert.ToUInt32(var.EndTime - SystemTime.Instance.GetSystemTimeTotalSecond());
                        return true;
                    }
                }
                else
                {
                    NeedSum -= var.ItemSum;
                }
            }
            return false;
        }
    }
    private static int SortCompareIteminfo(tagItemInfo var1,tagItemInfo var2)
    {
        //从大到小排序
        //
        UInt32 TotalSeconds = SystemTime.Instance.GetSystemTimeTotalSecond();

        UInt32 EndSecVar1 = Convert.ToUInt32(var1.EndTime ==0 ? 0xffffffff : (var1.EndTime - TotalSeconds));
        UInt32 EndSecVar2 = Convert.ToUInt32(var2.EndTime ==0 ? 0xffffffff : (var2.EndTime - TotalSeconds));
        if (EndSecVar1 == EndSecVar2)
        {
            return 0;
        }
        else if (EndSecVar1 < EndSecVar2)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    public UInt32 GetItemSum(UInt32 ItemID,bool IsConvert = true)
    {
        UInt32 ItemSum = 0;
        if (ItemCountMap.ContainsKey(ItemID))
            ItemSum = ItemCountMap[ItemID];
        //判断物品转换
        if (IsConvert && FishConfig.Instance.m_ItemConvertInfo.m_ConvertMap.ContainsKey(ItemID))
        {
            UInt32 GlobelItemSum = 0;
            UInt32 MedalItemSum = 0;
            UInt32 CurreyItemSum = 0;
            //获取玩家的金钱 奖牌 乐币
            if(FishConfig.Instance.m_ItemConvertInfo.m_ConvertMap[ItemID].Globel != 0)
            {
                GlobelItemSum = Convert.ToUInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() / FishConfig.Instance.m_ItemConvertInfo.m_ConvertMap[ItemID].Globel);
            }
            if (FishConfig.Instance.m_ItemConvertInfo.m_ConvertMap[ItemID].Medal != 0)
            {
                MedalItemSum = Convert.ToUInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetMedal() / FishConfig.Instance.m_ItemConvertInfo.m_ConvertMap[ItemID].Medal);
            }
            if (FishConfig.Instance.m_ItemConvertInfo.m_ConvertMap[ItemID].Currey != 0)
            {
                CurreyItemSum = Convert.ToUInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() / FishConfig.Instance.m_ItemConvertInfo.m_ConvertMap[ItemID].Currey);
            }
            UInt32 ItemConvertSum = (GlobelItemSum < MedalItemSum ? GlobelItemSum : MedalItemSum) < CurreyItemSum ? (GlobelItemSum < MedalItemSum ? GlobelItemSum : MedalItemSum) : CurreyItemSum;
            return ItemConvertSum + ItemSum;
        }
        else
            return ItemSum;
    }

    //不应该被外部函数使用的 
    private void OnAddItemMap(tagItemInfo pInfo)
    {
        ItemMap.Add(pInfo.ItemOnlyID, pInfo);
        if (ItemCountMap.ContainsKey(pInfo.ItemID))
            ItemCountMap[pInfo.ItemID] += pInfo.ItemSum;
        else
            ItemCountMap.Add(pInfo.ItemID, pInfo.ItemSum);
        return;
    }
    private bool DelItem(UInt32 ItemOnlyID)
    {
        if(ItemMap.ContainsKey(ItemOnlyID))
        {
            if(!ItemCountMap.ContainsKey( ItemMap[ItemOnlyID].ItemID))
                return false;
            ItemCountMap[ItemMap[ItemOnlyID].ItemID] -= ItemMap[ItemOnlyID].ItemSum;
            if (ItemCountMap[ItemMap[ItemOnlyID].ItemID] == 0)
                ItemCountMap.Remove(ItemMap[ItemOnlyID].ItemID);
            ItemMap.Remove(ItemOnlyID);
            return true;
        }
        return false;
    }
    private bool ChangeItem(UInt32 ItemOnlyID, UInt32 ItemSum)
    {
        if (ItemMap.ContainsKey(ItemOnlyID))
        {
            if (!ItemCountMap.ContainsKey(ItemMap[ItemOnlyID].ItemID))
                return false;
            if (ItemSum >= ItemMap[ItemOnlyID].ItemSum)
                ItemCountMap[ItemMap[ItemOnlyID].ItemID] += Convert.ToUInt32(ItemSum - ItemMap[ItemOnlyID].ItemSum);
            else
                ItemCountMap[ItemMap[ItemOnlyID].ItemID] -= Convert.ToUInt32(ItemMap[ItemOnlyID].ItemSum - ItemSum);
            if (ItemCountMap[ItemMap[ItemOnlyID].ItemID] == 0)
                ItemCountMap.Remove(ItemMap[ItemOnlyID].ItemID);
            ItemMap[ItemOnlyID].ItemSum = ItemSum;
            return true;
        }
        return false;
    }
    //命令处理函数
    private void SendGetAllUserItem()
    {
        CL_Cmd_GetUserItem ncb = new CL_Cmd_GetUserItem();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetUserItem);
        NetServices.Instance.Send<CL_Cmd_GetUserItem>(ncb);
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetUserItem:
                  return HandleGetUserItem(obj);
            //case NetCmdType.CMD_LC_GetUserItemFinish:
            //    {
            //        return HandleGetUserItemFinish(obj);
            //    }
            case NetCmdType.CMD_LC_AddUserItem:
                 return HandleAddUsetItem(obj);
            case NetCmdType.CMD_LC_DelUserItem:
                 return HandleDelUserItem(obj);
            case NetCmdType.CMD_LC_ChangeUserItem:
                 return HandleChangeUserItem(obj);
            case NetCmdType.CMD_LC_OnUseItem:
                return HandleOnUseItemResult(obj);
            case NetCmdType.CMD_LC_OnAcceptItem:
                return HandleOnAcceptItemResult(obj);
            default:
                return false;
        }
    }
    private bool HandleGetUserItem(NetCmdBase obj)
    {
        //获得全部的物品
        LC_Cmd_GetUserItem ncb = (LC_Cmd_GetUserItem)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            ItemMap.Clear();
            ItemCountMap.Clear();
        }
        for (UInt16 i = 0; i < ncb.Sum; ++i)
        {
            OnAddItemMap(ncb.Array[i]);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadItem = true;
            tagItemChangeEvent Event = new tagItemChangeEvent();
            MsgEventHandle.HandleMsg(Event);
        }
        return true;   
    }
    //private bool HandleGetUserItemFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_GetUserItemFinish ncb = (LC_Cmd_GetUserItemFinish)obj;
    //    m_IsLoadItem = true;
    //    //触发函数 用于 对外输出 玩家背包改变的函数
    //    tagItemChangeEvent Event = new tagItemChangeEvent();
    //    MsgEventHandle.HandleMsg(Event);
    //    return true;
    //}
    private bool HandleAddUsetItem(NetCmdBase obj)
    {
        LC_Cmd_AddUserItem ncb = (LC_Cmd_AddUserItem)obj;
        OnAddItemMap(ncb.ItemInfo);
        tagItemChangeEvent Event = new tagItemChangeEvent();
        MsgEventHandle.HandleMsg(Event);
        return true;
    }
    private  bool HandleDelUserItem(NetCmdBase obj)
    {
        LC_Cmd_DelUserItem ncb = (LC_Cmd_DelUserItem)obj;
        DelItem(ncb.ItemOnlyID);
        tagItemChangeEvent Event = new tagItemChangeEvent();
        MsgEventHandle.HandleMsg(Event);
        return true;
    }
    private bool HandleChangeUserItem(NetCmdBase obj)
    {
        LC_Cmd_ChangeUserItem ncb = (LC_Cmd_ChangeUserItem)obj;
        ChangeItem(ncb.ItemOnlyID, ncb.ItemSum);
        tagItemChangeEvent Event = new tagItemChangeEvent();
        MsgEventHandle.HandleMsg(Event);
        return true;
    }
    private bool HandleOnUseItemResult(NetCmdBase obj)
    {
        LC_Cmd_OnUseItem ncb = (LC_Cmd_OnUseItem)obj;
        tagItemUseEvent pEvent = new tagItemUseEvent(ncb.Result, ncb.ItemOnlyID, ncb.ItemID, ncb.ItemSum);
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result?UserOperateMessage.UOM_Item_UseItem_Sucess:UserOperateMessage.UOM_Item_UseItem_Failed_5));
        MsgEventHandle.HandleMsg(pUOM);
        return true;
    }
    private bool HandleOnAcceptItemResult(NetCmdBase obj)
    {
        LC_Cmd_OnAcceptItem ncb = (LC_Cmd_OnAcceptItem)obj;

        tagItemAcceptEvent pEvent = new tagItemAcceptEvent(ncb.dwDestUserID,ncb.Result, ncb.ItemOnlyID, ncb.ItemID, ncb.ItemSum);
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Item_AcceptItem_Sucess : UserOperateMessage.UOM_Item_AcceptItem_Failed_5));
        MsgEventHandle.HandleMsg(pUOM);
        return true;
    }
}