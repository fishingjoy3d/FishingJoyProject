using UnityEngine;
using System.Collections.Generic;

public class ShopItemInfoUI : BaseWnd
{
    UISprite            m_ItemIcon;            //商品图片
    UILabel             m_ItemName;            //商品名称
    UILabel             m_ItemSum;             //商品数量
    UILabel             m_ItemDesc;            //商品描述
    UILabel             m_ItemPrice;           //商品价格
    UILabel             m_ItemTime;             //商品剩余时间
    UISprite            m_ItemPriceIcon;        //购买商品所用币种
    GameObject          m_ItemGetFlagObj;      //获取的标志
    tagShopItemConfig   m_ItemInfo; 
    byte                m_ShopID;
    byte                m_OnlyID;   
    bool                m_bBuy = true;

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_ItemIcon = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_ItemName = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        m_ItemPrice = m_BaseTrans.GetChild(2).GetChild(0).GetComponent<UILabel>();
        m_ItemPriceIcon = m_BaseTrans.GetChild(2).GetComponent<UISprite>();
        UIEventListener.Get(m_BaseWndObject).onClick = OnClickBuyItem;
    }
    public void InitItemGetFlag()
    {
        m_ItemGetFlagObj = m_BaseTrans.GetChild(3).gameObject;
    }
    public void InitItemDesc()
    {
        m_ItemDesc = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
    }
    public void InitItemSum()
    {
        m_ItemSum = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
    }
    public void InitItemLifeTime()
    {
        m_ItemTime = m_BaseTrans.GetChild(2).GetChild(1).GetComponent<UILabel>();
    }
    public void ResetLocalScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void SetShopItemInfo(byte shopID, tagShopItemConfig ItemInfo, tagShopItemStr ItemStr)
    {
        m_ShopID = shopID;
        m_OnlyID = ItemInfo.ItemInde;
        m_ItemInfo = ItemInfo;
        m_ItemName.text = ItemStr.ItemName;
        m_ItemIcon.spriteName = ItemStr.ItemIcon;
        if (m_ItemDesc != null)
        {
            string str = ItemStr.ItemDec;
            str = str.Replace("/n", "\n");
            m_ItemDesc.text = str;
        }
        if (m_ItemSum != null)
            m_ItemSum.text = "x "+ItemInfo.ItemInfo.ItemSum.ToString();
        SetGoodsPriceType(ItemInfo);
    }
    //适用背包界面中炮的显示
    public void SetShopItemInfo(string ItemIcon, string ItemName, byte VipLevel, bool isCanUser)
    {
        if (isCanUser)
        {
            m_ItemGetFlagObj.SetActive(true);
            m_ItemPriceIcon.gameObject.SetActive(false);
            m_bBuy = false;
        }
        else
        {
            m_bBuy = false;
            m_ItemGetFlagObj.SetActive(false);
            m_ItemPriceIcon.gameObject.SetActive(true);
            m_ItemPrice.text = VipLevel.ToString();
        }
        m_ItemName.text = ItemName;
        m_ItemIcon.spriteName = ItemIcon;
    }
    public void SetIsGetLauncher()
    {
        m_ItemGetFlagObj.SetActive(true);
        m_ItemPriceIcon.gameObject.SetActive(false);
        m_bBuy = false;
    }
    void SetGoodsPriceType(tagShopItemConfig ItemInfo)
    {
        if (ItemInfo.PriceGlobel > 0)
        {
            m_ItemPrice.text = ItemInfo.PriceGlobel.ToString();
            m_ItemPriceIcon.spriteName = "HallBtn_Gold";
        }
        else if (ItemInfo.PriceCurrey > 0)
        {
            m_ItemPrice.text = ItemInfo.PriceCurrey.ToString();
            m_ItemPriceIcon.spriteName = "HallBtn_Diamond";
        }
        else
        {
            m_ItemPrice.text = ItemInfo.PriceMabel.ToString();
            m_ItemPriceIcon.spriteName = "HallBtn_Medal";
        }
    }
    void OnClickBuyItem(GameObject go)
    {
        if (!m_bBuy)
            return;
        ShopRuntime.ConfirmBuyUI.ShowConfirmBuyWnd(m_OnlyID, m_ShopID);
        ShopRuntime.ShopLogicMgr.SetMaskEft(false);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
	
}
