using UnityEngine;
using System.Collections.Generic;

public class ConfirmBuyLogicUI : BaseWnd
{
    GameObject[]        m_BtnObj = new GameObject[4];
    UIButton[]          m_UIButton = new UIButton[3];
    UISprite            m_GoodsSprite;
    UISprite            m_MoneyTypeSprite;
    GameObject          m_FishIcon;
    UILabel             m_ItemSumLabel;
    UILabel             m_ItemPriceLabel;
    UILabel             m_ItemDescribeLabel;
    UILabel             m_GoodsSum;
    UILabel             m_ItemName;
    TweenScale          m_TweenAnim;
    byte                m_ShopID;
    byte                m_ItemOnlyID;
    uint                m_ItemPrice;
    uint                m_ItemSum;
    float               m_PressTime;
    float               m_PressInterval;
    bool                m_PressSub;
    bool                m_PressAdd;
    byte                m_PriceType;        //购买货币的种类 ‘1’为金币 ‘2’为钻石 '3'为奖牌

    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[1]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        for (byte i = 0; i < m_BtnObj.Length; ++i)
        {
            m_BtnObj[i] = m_BaseTrans.GetChild(i).gameObject;
            if (i > 0)
                m_UIButton[i - 1] = m_BtnObj[i].GetComponent<UIButton>();
 
            UIEventListener.Get(m_BtnObj[i]).onClick = OnClickBtnMsg;
            UIEventListener.Get(m_BtnObj[i]).onPress = OnPressBtnMsg;
        }
        m_TweenAnim = m_BaseWndObject.GetComponent<TweenScale>();
        m_ItemSumLabel = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        m_ItemPriceLabel = m_BaseTrans.GetChild(5).GetComponent<UILabel>();
        m_ItemDescribeLabel = m_BaseTrans.GetChild(6).GetComponent<UILabel>();
        m_GoodsSum = m_BaseTrans.GetChild(8).GetComponent<UILabel>();
        m_GoodsSprite = m_BaseTrans.GetChild(7).GetComponent<UISprite>();
        m_MoneyTypeSprite = m_BaseTrans.GetChild(5).GetChild(0).GetComponent<UISprite>();
        m_ItemName = m_BaseTrans.GetChild(9).GetComponent<UILabel>();
        m_FishIcon = m_BaseTrans.GetChild(10).gameObject;
    }
    public void ShowConfirmBuyWnd(byte onlyID, byte shopID, uint itemSum = 1, bool bNewGide = false)
    {
        {
            ShopItemType byShopType = FishConfig.Instance.m_ShopInfo.ShopMap[shopID].ShopItemMap[onlyID].ShopType;
            if (PlayerRole.Instance.ShopManager.IsNeedShare(byShopType))
            {
                GlobalHallUIMgr.Instance.GameShare.ShowExchage(FishConfig.Instance.m_ShopInfo.ShopMap[shopID].ShopItemMap[onlyID].ItemInfo.ItemID, PlayerRole.Instance.RoleInfo.RoleMe.GetTotalCashSum());
                return;
            }
        }

        if (m_BaseWndObject != null)
            return;
        Init();
        if (bNewGide)
            m_FishIcon.SetActive(true);
      //  GlobalHallUIMgr.Instance.ShowMask(true, 30);
        m_TweenAnim.ResetToBeginning();
        m_TweenAnim.PlayForward();
        m_ItemOnlyID = onlyID;
        m_ShopID = shopID;

        tagShopItemStr pItemStr = FishConfig.Instance.m_ShopInfo.ShopMap[shopID].ShopItemStrMap[onlyID];     
        string str = pItemStr.ItemDec;
        str = str.Replace("/n", "\n");
        m_ItemDescribeLabel.text = str;
        m_GoodsSprite.spriteName = pItemStr.ItemIcon;
        if (FishConfig.Instance.m_ShopInfo.ShopMap[shopID].ShopItemMap[onlyID].ShopType == ShopItemType.SIT_Entity||
            FishConfig.Instance.m_ShopInfo.ShopMap[shopID].ShopItemMap[onlyID].ShopType == ShopItemType.SIT_PhonePay)
        //if (m_ShopID == (byte)(Shop_Type.Shop_Material + 1))
        {
            m_BtnObj[2].SetActive(false);
            m_BtnObj[3].SetActive(false);
            m_ItemName.gameObject.SetActive(true);

            byte exchangeSum = PlayerRole.Instance.RoleInfo.GetCanUseCashSum();//(byte)(PlayerRole.Instance.RoleVip.GetUseMedalSum() - PlayerRole.Instance.RoleInfo.RoleMe.GetCashSum());
            if (exchangeSum == 0)
                m_UIButton[0].isEnabled = false;
            m_ItemName.text = string.Format(StringTable.GetString("Shop_Exchange_Sum"), exchangeSum);
            m_ItemSum = 1;
        }
        else
        {
            m_GoodsSum.text = "x " + FishConfig.Instance.m_ShopInfo.ShopMap[shopID].ShopItemMap[onlyID].ItemInfo.ItemSum.ToString();
            m_ItemSum = itemSum;
            m_ItemSumLabel.text = m_ItemSum.ToString();
        }
        GetItemPrice();
        m_ItemPriceLabel.text = (m_ItemSum*m_ItemPrice).ToString();

        m_UIButton[2].isEnabled = false;
        WndManager.Instance.Push(m_BaseWndObject);
    }
    void GetItemPrice()
    {
        if (FishConfig.Instance.m_ShopInfo.ShopMap[m_ShopID].ShopItemMap[m_ItemOnlyID].PriceGlobel > 0)
        {
            m_ItemPrice = FishConfig.Instance.m_ShopInfo.ShopMap[m_ShopID].ShopItemMap[m_ItemOnlyID].PriceGlobel;
            //显示响应的货币图标
            m_MoneyTypeSprite.spriteName = "HallBtn_Gold";
            m_PriceType = 1;
        }
        else if (FishConfig.Instance.m_ShopInfo.ShopMap[m_ShopID].ShopItemMap[m_ItemOnlyID].PriceMabel > 0)
        {
            m_ItemPrice = FishConfig.Instance.m_ShopInfo.ShopMap[m_ShopID].ShopItemMap[m_ItemOnlyID].PriceMabel;
            //
            m_MoneyTypeSprite.spriteName = "HallBtn_Medal";
            m_PriceType = 3;
        }
        else
        {
            m_ItemPrice = FishConfig.Instance.m_ShopInfo.ShopMap[m_ShopID].ShopItemMap[m_ItemOnlyID].PriceCurrey;
            //
            m_MoneyTypeSprite.spriteName = "HallBtn_Diamond";
            m_PriceType = 2;

        }
    }
    bool IsMoneyEnough()
    {
        switch (m_PriceType)
        {
            case 1:
                if ((m_ItemPrice * m_ItemSum) > PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel())
                {
                    GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Gold);
                    return false;
                }
                break;
            case 2:
                if ((m_ItemPrice * m_ItemSum) > PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency())
                {
                    GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
                    return false;
                }
                break;
            case 3:
                if ((m_ItemPrice * m_ItemSum) > PlayerRole.Instance.RoleInfo.RoleMe.GetMedal())
                {
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("GlobelShop_Shop_Field1"), 1.5f, false);
                    return false;
                }
                break;
        }
        return true;
    }
    public void Update(float deltaTime)
    {
        if (m_BaseWndObject == null)
            return;
        if (SceneRuntime.Instance != null && (m_PressAdd || m_PressSub))
        {
            if (SceneRuntime.SceneLogic.bClearScene && ShopRuntime.Instance == null)
            {
                m_PressAdd = false;
                m_PressSub = false;
            }
        }
        
 

        if (m_PressAdd)
        {
            m_PressTime += deltaTime;
            if (m_PressTime < 0.5f)
                return;
            m_PressInterval += deltaTime;
            if (m_PressInterval >= 0.1f)
            {
                ++m_ItemSum;
                if (!IsMoneyEnough())
                {
                    m_ItemSum--;
                    m_UIButton[1].isEnabled = false;
                    m_PressAdd = false;
                }
                m_PressInterval = 0;
                m_ItemSumLabel.text = m_ItemSum.ToString();
                m_ItemPriceLabel.text = (m_ItemPrice * m_ItemSum).ToString();
                
                if (m_ItemSum > 1 && m_UIButton[2].isEnabled == false)
                    m_UIButton[2].isEnabled = true;
            }
        }
        else if (m_PressSub)
        {
            m_PressTime += deltaTime;
            if (m_PressTime < 0.5f)
                return;
            m_PressInterval += deltaTime;
            if (m_PressInterval >= 0.1f)
            {
                --m_ItemSum;
                m_PressInterval = 0;
                m_ItemSumLabel.text = m_ItemSum.ToString();
                m_ItemPriceLabel.text = (m_ItemPrice * m_ItemSum).ToString();
              //  Debug.Log(m_ItemSum.ToString());
                if (IsMoneyEnough())
                {
                    m_UIButton[1].isEnabled = true;
                }
                if (m_ItemSum <= 1)
                {
                    m_UIButton[2].isEnabled = false;
                    m_PressSub = false;
                }
            }
        }
    }
    void OnClickBtnMsg(GameObject go)
    {
        //关闭按纽
        if (go == m_BtnObj[0])
        {
            //WndManager.Instance.Pop();
            ShutDown();
            if (SceneRuntime.Instance == null)
                ShopRuntime.ShopLogicMgr.SetMaskEft(true);
   
            GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
            return;
        }
        //确认购买
        else if (go == m_BtnObj[1])
        {
            ShutDown();
            if (!IsMoneyEnough())
            {
                return;
            }
            if (PlayerRole.Instance.ShopManager.ShopItem(m_ShopID, m_ItemOnlyID, m_ItemSum))
            {
                if (FishConfig.Instance.m_ShopInfo.ShopMap[m_ShopID].ShopItemMap[m_ItemOnlyID].ShopType == ShopItemType.SIT_PhonePay)
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("GlobelShop_Shop_phone"), 1.5f);
            }
            
            if (SceneRuntime.Instance == null)
                ShopRuntime.ShopLogicMgr.SetMaskEft(true);
            GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        }
        //“+”按纽
        else if (go == m_BtnObj[2])
        {
            ++m_ItemSum;
            if (!IsMoneyEnough())
            {
                m_ItemSum--;
                m_UIButton[1].isEnabled = false;
            }
            uint priceSum = m_ItemPrice * m_ItemSum;
            
            m_ItemSumLabel.text = m_ItemSum.ToString();
            m_ItemPriceLabel.text = priceSum.ToString();
            if (m_ItemSum > 1 && m_UIButton[2].isEnabled == false)
                m_UIButton[2].isEnabled = true;
        }
        //“-”按纽
        else
        {
            --m_ItemSum;
            if (IsMoneyEnough())
            {
                m_UIButton[1].isEnabled = true;
            }
            m_ItemSumLabel.text = m_ItemSum.ToString();
            m_ItemPriceLabel.text = (m_ItemPrice * m_ItemSum).ToString();
            if (m_ItemSum <= 1)
                m_UIButton[2].isEnabled = false;
        }
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
    }
    void OnPressBtnMsg(GameObject go, bool state)
    {
        if (state)
        {
            if (go == m_BtnObj[2])
            {
                m_PressAdd = true;
                m_PressInterval = 0;
                m_PressTime = 0;
            }
            else if (go == m_BtnObj[3])
            {
                m_PressSub = true;
                m_PressInterval = 0;
                m_PressTime = 0;

            }
        }
        else
        {
            m_PressAdd = false;
            m_PressSub = false;
            m_PressInterval = 0;
            m_PressTime = 0;

        }

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

