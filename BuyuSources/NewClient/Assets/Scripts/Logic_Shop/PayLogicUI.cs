using UnityEngine;
using System.Collections.Generic;


public enum PayType
{
    Diamond = 0,
    Gold,
}

public struct PayTopBtn
{
    public GameObject   m_BtnObj;
    public Transform    m_BtnTrans;
}

public class PayLogicUI : BaseWnd
{
    PayTopBtn[]         m_TopBtn = new PayTopBtn[2];
    ButtonMask          m_MaskObj;
    PayType             m_PayType;
    Scroll              m_Scroll = new Scroll();
    List<PayItemInfo>   m_PayInfoList = new List<PayItemInfo>();

    
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[2]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        InitTopBtn();
        InitView();
        CheckHighLight(m_PayType);
    }
    void InitTopBtn()
    {
        Transform BaseTr = m_BaseTrans.GetChild(0);
        m_TopBtn[0].m_BtnObj = BaseTr.GetChild(0).gameObject;
        m_TopBtn[0].m_BtnTrans = BaseTr.GetChild(0);
        m_TopBtn[1].m_BtnObj = BaseTr.GetChild(1).gameObject;
        m_TopBtn[1].m_BtnTrans = BaseTr.GetChild(1);

        m_MaskObj.m_Mask = BaseTr.GetChild(2).gameObject;
        m_MaskObj.m_MaskTrans = BaseTr.GetChild(2);
        m_MaskObj.m_TweenAnim = m_MaskObj.m_MaskTrans.gameObject.GetComponent<TweenPosition>();
        
        UIEventListener.Get(m_TopBtn[0].m_BtnObj).onClick = OnDiamondBtnMsg;
        UIEventListener.Get(m_TopBtn[1].m_BtnObj).onClick = OnGoldBtnMsg;

        UIEventListener.Get(m_BaseTrans.GetChild(2).gameObject).onClick = OnCloseBtnMsg;
    }
    void InitView()
    {
        Transform baseTR = m_BaseTrans.GetChild(1);
        m_Scroll.m_Grid = new UIGrid[1];
        m_Scroll.m_BaseChild = baseTR.GetChild(0).gameObject;
        
        m_Scroll.m_Grid[0] = baseTR.GetChild(1).GetComponent<UIGrid>();
    }
    public void ShowPayWndUI(PayType type)
    {
        if (m_BaseWndObject != null)
            return;
        m_PayType = type;
        Init();
        WndManager.Instance.Push(m_BaseWndObject);
    }
    void SetScrollViewDate()
    {
        ClearPayGird();
        if (m_PayType == PayType.Diamond)
        {
            foreach (KeyValuePair<uint, tagFishRechargeInfo> map in FishConfig.Instance.m_FishRecharge.m_FishRechargeMap)
            {
                if (!map.Value.IsAddCurrcey())
                    continue;
                if (map.Value.IsFirstAdd() && !PlayerRole.Instance.RoleInfo.RoleMe.GetIsFirstPayCurrcey())
                    continue;
                PayItemInfo item = new PayItemInfo();
                item.Init(m_Scroll.m_BaseChild);
                item.ShowGoodsInfo(map.Key, map.Value, m_PayType);
                m_Scroll.m_Grid[0].AddChild(item.m_BaseTrans);
                item.ResetLocalScale();
            }
        }
        else
        {
            foreach (KeyValuePair<uint, tagFishRechargeInfo> map in FishConfig.Instance.m_FishRecharge.m_FishRechargeMap)
            {
                if (!map.Value.IsAddGlobel())
                    continue;
                if (map.Value.IsFirstAdd() && !PlayerRole.Instance.RoleInfo.RoleMe.GetIsFirstPayGlobel())
                    continue;
                PayItemInfo item = new PayItemInfo();
                item.Init(m_Scroll.m_BaseChild);
                item.ShowGoodsInfo(map.Key, map.Value, m_PayType);
                m_Scroll.m_Grid[0].AddChild(item.m_BaseTrans);
                item.ResetLocalScale();
            }
        }

    }
  
    void CheckHighLight(PayType type)
    {
        Vector3 FromPos = m_TopBtn[(int)m_PayType].m_BtnTrans.localPosition;
        FromPos.y += 4;
        m_PayType = type;
        Vector3 ToPos = m_TopBtn[(int)m_PayType].m_BtnTrans.localPosition;
        ToPos.y += 4;
        m_MaskObj.m_TweenAnim.ResetToBeginning();
        m_MaskObj.m_TweenAnim.from = FromPos;
        m_MaskObj.m_TweenAnim.to = ToPos;
        m_MaskObj.m_TweenAnim.PlayForward();
        SetScrollViewDate();
    }
    void OnDiamondBtnMsg(GameObject go)
    {
        CheckHighLight(PayType.Diamond);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void OnGoldBtnMsg(GameObject go)
    {
        CheckHighLight(PayType.Gold);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void OnCloseBtnMsg(GameObject go)
    {
        ShutDown();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
    }
    void ClearPayGird()
    {

        List<Transform> gridChid = m_Scroll.m_Grid[0].GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_Scroll.m_Grid[0].transform.DetachChildren();
    }
    public void CloseWnd()
    {
        OnCloseBtnMsg(null);
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            m_PayInfoList.Clear();
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
	
}

public class PayItemInfo : BaseWnd
{
    UILabel             m_DesItemSum;
    UILabel             m_ItemCurPrice;         //货物现价
    UILabel             m_ItemOldPrice;         //货物原价
    UILabel             m_ItemName;             //货物名称
    UISprite            m_ItemIcon;           //目标货物图
    UISprite            m_ItemPriceIcon;     //需要消耗货币图
    UISprite            m_DesItemIcon;       //目标货币数量图
    UISprite            m_DisCountIcon;     //折扣图标
    GameObject          m_OverLine;         //下划线
    PayType             m_Paytype;
    uint                m_ItemNum;
    uint                m_ItemID;

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_DesItemIcon = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_DesItemSum = m_BaseTrans.GetChild(0).GetChild(0).GetComponent<UILabel>();
        m_ItemPriceIcon = m_BaseTrans.GetChild(1).GetComponent<UISprite>();
        m_ItemCurPrice = m_BaseTrans.GetChild(1).GetChild(1).GetComponent<UILabel>();
        m_ItemOldPrice = m_BaseTrans.GetChild(1).GetChild(0).GetComponent<UILabel>();
        m_OverLine = m_BaseTrans.GetChild(1).GetChild(2).gameObject;
        m_ItemIcon = m_BaseTrans.GetChild(2).GetComponent<UISprite>();
        m_DisCountIcon = m_BaseTrans.GetChild(3).GetComponent<UISprite>();
        m_ItemName = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        UIEventListener.Get(m_BaseWndObject).onClick = OnClickBuy;
    }
    public void ShowGoodsInfo(uint ItemID, tagFishRechargeInfo payInfo, PayType type)
    {
        m_Paytype = type;
        m_ItemID = ItemID;
        m_ItemIcon.spriteName = payInfo.Icon;
        m_ItemName.text = payInfo.Name;
        if (type == PayType.Gold)
        {
            m_DesItemIcon.spriteName = "HallBtn_Gold";
            m_ItemPriceIcon.spriteName = "rmb_logo";
        }
        else
        {
            m_DesItemIcon.spriteName = "HallBtn_Diamond";
            m_ItemPriceIcon.spriteName = "rmb_logo";
        }
        m_DesItemSum.text = payInfo.AddMoney.ToString();
        m_DisCountIcon.spriteName = payInfo.sDisCountPicName;
        //不打折扣
        if (payInfo.dDisCountPrice == payInfo.dPreDisCountPrice)
        {
            m_ItemOldPrice.text = payInfo.dDisCountPrice.ToString();
            m_ItemCurPrice.text = payInfo.dDisCountPrice.ToString();
            m_ItemCurPrice.gameObject.SetActive(false);
            m_OverLine.SetActive(false);
        }
        else
        {
            m_ItemOldPrice.text = payInfo.dPreDisCountPrice.ToString();
            m_ItemCurPrice.text = payInfo.dDisCountPrice.ToString();
            m_OverLine.SetActive(true);

        }

    }
  
    public void ResetLocalScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    void OnClickBuy(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        uint amount = uint.Parse(m_ItemCurPrice.text);
        uint count = uint.Parse(m_DesItemSum.text);
        if (m_Paytype == PayType.Gold)
            GlobalHallUIMgr.Instance.ShowmPayBuyConfirm((byte)m_ItemID, amount, count, PayType.Gold);
        else
            GlobalHallUIMgr.Instance.ShowmPayBuyConfirm((byte)m_ItemID, amount, count, PayType.Diamond);
       
    }
}
