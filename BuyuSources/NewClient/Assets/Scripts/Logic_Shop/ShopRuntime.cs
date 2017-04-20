using UnityEngine;
using System.Collections;

public class ShopRuntime 
{
    static public ShopRuntime Instance;
    public ShopLogicMgr Shop;


    public static void Init(ShopLogicMgr logic)
    {
        Instance = new ShopRuntime();
        Instance.Shop = logic;
    }
    public static ShopLogicMgr ShopLogicMgr
    {
        get
        {
            return Instance.Shop;
        }
    }
    public static KnapsackShopWnd ShopUI
    {
        get
        {
            return ShopLogicMgr.KnapsackShopWnd;
        }
    }
    public static DaPaoShopWnd DaPaoShop
    {
        get
        {
            return ShopLogicMgr.DaPaoShopWnd;
        }
    }
    public static MaterailShopWnd MaterailShop
    {
        get
        {
            return ShopLogicMgr.MaterailShopWnd;
        }
    }
    public static ConfirmBuyLogicUI ConfirmBuyUI
    {
        get
        {
            return ShopLogicMgr.ConfirmBryWnd;
        }
    }
    public static PropertyDescWnd PropertyDescUI
    {
        get
        {
            return ShopLogicMgr.PropertyDesc;
        }
    }
    public static void Shutdown()
    {
        Instance = null;
    }
}
