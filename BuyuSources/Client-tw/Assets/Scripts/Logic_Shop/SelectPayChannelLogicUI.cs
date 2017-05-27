using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// 选择充值的支付方式
/// </summary>
public class SelectPayChannelLogicUI : BaseWnd
{
    Scroll m_Scroll = new Scroll();
    List<PayChannelItemInfo> m_PayInfoList = new List<PayChannelItemInfo>();
    Transform m_ScaleTrans;

    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[9]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_ScaleTrans = m_BaseTrans.GetChild(0);

        InitBtn();
        InitView();
    }
    void InitBtn()
    {
        UIEventListener.Get(m_ScaleTrans.GetChild(1).gameObject).onClick = OnCloseBtn;
    }
    void InitView()
    {
        Transform baseTR = m_ScaleTrans.GetChild(0);
        m_Scroll.m_Grid = new UIGrid[1];
        m_Scroll.m_BaseChild = baseTR.GetChild(0).gameObject;

        m_Scroll.m_Grid[0] = baseTR.GetChild(1).GetComponent<UIGrid>();

        SetScrollViewDate();
    }
    public void ShowSelectPayChannelWnd(uint itemID)
    {
        if (m_BaseWndObject != null)
            return;
        SDKMgr.Instance.PayData.ItemID = itemID;
        Init();
        WndManager.Instance.Push(m_BaseWndObject);
    }
    void SetScrollViewDate()
    {
        ClearPayGrid();
        List<PayChannelType> channelList = new List<PayChannelType>() { PayChannelType.Facebook, PayChannelType.GooglePlay };
        for (int i = 0; i < channelList.Count; i++)
        {
            PayChannelItemInfo item = new PayChannelItemInfo();
            item.Init(m_Scroll.m_BaseChild);
            item.ShowChannelInfo(channelList[i]);
            m_Scroll.m_Grid[0].AddChild(item.m_BaseTrans);
            item.ResetLocalScale();
        }
    }
    void OnCloseBtn(GameObject go)
    {
        ShutDown();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
    }
    void ClearPayGrid()
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
        OnCloseBtn(null);
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

public class PayChannelItemInfo : BaseWnd
{
    UISprite m_PayChannelIcon;
    PayChannelType m_PayChannel;

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_PayChannelIcon = m_BaseTrans.GetChild(1).GetComponent<UISprite>();
        UIEventListener.Get(m_BaseWndObject).onClick = OnClickSelect;
    }

    public void ShowChannelInfo(PayChannelType payChannel)
    {
        m_PayChannel = payChannel;

        if (m_PayChannel == PayChannelType.Facebook)
        {
            m_PayChannelIcon.spriteName = "facebook";
        }
        else if (m_PayChannel == PayChannelType.GooglePlay)
        {
            m_PayChannelIcon.spriteName = "googleplay";
        }
        m_PayChannelIcon.MakePixelPerfect();
    }

    public void ResetLocalScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }

    void OnClickSelect(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        SDKMgr.Instance.PayData.PayChannel = m_PayChannel;
        uint itemID = SDKMgr.Instance.PayData.ItemID;
        GlobalHallUIMgr.Instance.ShowmPayBuyConfirm(itemID);
    }
}
