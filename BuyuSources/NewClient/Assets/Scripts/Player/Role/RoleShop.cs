using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//用户商店管理器
class RoleShop //商店开启的前置条件 就是客户端已经有了玩家的实体数据 必须先获得实体数据才可以打开UI
{

    public RoleShop()
    {
        OnClear();
    }
    public void OnClear()
    {
    }
    public bool ShopItem(Byte ShopID, Byte ItemIndex, UInt32 ItemSum)
    {
        return SendShopItem(ShopID, ItemIndex, ItemSum);
    }
    public bool IsNeedShare(ShopItemType ItemType)
    {
        if(ItemType != ShopItemType.SIT_PhonePay && ItemType != ShopItemType.SIT_Entity)
            return false;
        if (!PlayerRole.Instance.RoleInfo.RoleMe.GetShareStates() && FishConfig.Instance.m_ShareConfig.PackageMap.ContainsKey(SDKMgr.PACKAGE_NAME))
            return true;
        else
            return false;
    }
    private bool SendShopItem(Byte ShopID, Byte ItemIndex, UInt32 ItemSum)
    {
        //玩家购买物品
        //1.判断商店是否存在
        if (!FishConfig.Instance.m_ShopInfo.ShopMap.ContainsKey(ShopID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Shop_BuyItem_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //2.判断物品是否存在
        if (!FishConfig.Instance.m_ShopInfo.ShopMap[ShopID].ShopItemMap.ContainsKey(ItemIndex))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Shop_BuyItem_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (ItemSum > 1 && !FishConfig.Instance.m_ShopInfo.ShopMap[ShopID].ShopItemMap[ItemIndex].IsCanPile)
        {
            //不可以堆叠的物品购买多个 
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Shop_BuyItem_Failed_8);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //3.判断物品是否已经激活
        if (!FishConfig.Instance.m_ShopInfo.ShopMap[ShopID].ShopItemMap[ItemIndex].IsInTime())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Shop_BuyItem_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (FishConfig.Instance.m_ShopInfo.ShopMap[ShopID].ShopItemMap[ItemIndex].ShopType == ShopItemType.SIT_Entity)//实体物品购买
        {
            tagRoleAddressInfo pEntity = PlayerRole.Instance.EntityManager.GetRoleEntityInfo();
            if (pEntity == null || !PlayerRole.Instance.EntityManager.CheckEntityInfoIsCanUser())
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Shop_BuyItem_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }
        if (FishConfig.Instance.m_ShopInfo.ShopMap[ShopID].ShopItemMap[ItemIndex].ShopType == ShopItemType.SIT_PhonePay)
        {
            tagRoleAddressInfo pEntity = PlayerRole.Instance.EntityManager.GetRoleEntityInfo();
            if (pEntity == null || !PlayerRole.Instance.EntityManager.CheckIsCanPhonePay())
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Shop_BuyItem_Failed_7);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }

        //4.判断物品的价格 玩家是否可以购买  
        if (
            PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_ShopInfo.ShopMap[ShopID].ShopItemMap[ItemIndex].PriceGlobel * ItemSum ||
            PlayerRole.Instance.RoleInfo.RoleMe.GetMedal() < FishConfig.Instance.m_ShopInfo.ShopMap[ShopID].ShopItemMap[ItemIndex].PriceMabel * ItemSum ||
            PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_ShopInfo.ShopMap[ShopID].ShopItemMap[ItemIndex].PriceCurrey * ItemSum
            )
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Shop_BuyItem_Failed_5);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }

        //判断玩家兑换次数是否符合要求
        ShopItemType ItemType= FishConfig.Instance.m_ShopInfo.ShopMap[ShopID].ShopItemMap[ItemIndex].ShopType;
        if(ItemType == ShopItemType.SIT_PhonePay || ItemType == ShopItemType.SIT_Entity )
        {
            if(PlayerRole.Instance.RoleInfo.RoleMe.GetCashSum() >= PlayerRole.Instance.RoleVip.GetUseMedalSum())
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Shop_BuyItem_Failed_9);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
            //兑换次数符合要求 我们可以继续判断 部分平台上玩家 必须先点击分享才可以使用
            //1.配置文件判断 当前平台是否需要分享 (写入配置文件 360 官网包等 ) 
            if (IsNeedShare(ItemType))
            {
                //必须先分享才可以兑换物品
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Shop_BuyItem_Failed_10);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }
        //5.发送命令

        CL_Cmd_ShopItem ncb = new CL_Cmd_ShopItem();
        ncb.SetCmdType(NetCmdType.CMD_CL_ShopItem);
        ncb.ShopItemIndex = ItemIndex;
        ncb.ItemSum = ItemSum;
        ncb.ShopID = ShopID;
        NetServices.Instance.Send<CL_Cmd_ShopItem>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_ShopItemResult:
                return HandleShopItemResult(obj);
        }
        return true;
    }
    bool HandleShopItemResult(NetCmdBase obj)
    {
        LC_Cmd_ShopItemResult ncb = (LC_Cmd_ShopItemResult)obj;

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Shop_BuyItem_Sucess : UserOperateMessage.UOM_Shop_BuyItem_Failed_6));
        MsgEventHandle.HandleMsg(pUOM);

        tagShopResultEvent pEvent = new tagShopResultEvent(ncb.Result);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
}