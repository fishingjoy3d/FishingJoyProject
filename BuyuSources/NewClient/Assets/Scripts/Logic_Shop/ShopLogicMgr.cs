using UnityEngine;
using System.Collections.Generic;

public enum Shop_Type
{
    Shop_Property,       //道具
    Shop_Launcher,       //大炮
    Shop_Material,      //实物
    Shop_Submarine,     //潜艇
    Shop_Knapsack,      //背包
}

struct  Scroll
{
    public UIGrid[]     m_Grid;
    public GameObject   m_BaseChild;
  //  public UIScrollView m_BaseScroll;
}

struct WndBottomInfo
{
    public UILabel              m_MeadlLabel;
    public UILabel              m_DiamondLabel;     //钻石
    public UILabel              m_GoldLabel;
}

public class ShopLogicMgr : BaseWnd
{

    PropertyShopWnd         m_PropertyShopWnd = new PropertyShopWnd();     //道具商店
    DaPaoShopWnd            m_DaPaoShopWnd = new DaPaoShopWnd();           //大炮商店
    MaterailShopWnd         m_MaterailShopWnd = new MaterailShopWnd();      //实物商店
    KnapsackShopWnd         m_KnapsackShopWnd = new KnapsackShopWnd();    //背包商店
    ConfirmBuyLogicUI       m_ConfirmBryWnd = new ConfirmBuyLogicUI();   //商品购买确订窗口
    PropertyDescWnd         m_PropertyDesc = new PropertyDescWnd();         //背包中道具描述
    WndBottomInfo           m_WndTopInfo = new WndBottomInfo();          //商城系统窗口的最上面部分
    TreasurebowlLogic       m_TreasurebowlWnd = new TreasurebowlLogic();    //聚宝盆
    CBugleWnd               m_buglewnd = new CBugleWnd();
    Shop_Type               m_ShopType;
    UIButton                m_UIBackBtn;
    GameObject[]            m_GoldEft = new GameObject[3];
    GameObject[]            m_ShopWndBtns = new GameObject[5];
    GameObject[]            m_ShopWndBtnsCheckBg = new GameObject[5];   //商城类型按纽选中
    GameObject[]            m_ShopWndObj = new GameObject[5];

    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[0]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);
        
        InitBottomInfo();
        InitWndTopInfo();
        InitChildWnd();
        //管理器初始化
        ShopRuntime.Init(this);
        m_UIBackBtn = m_BaseTrans.GetChild(3).GetChild(0).GetComponent<UIButton>();
        UIEventListener.Get(m_BaseTrans.GetChild(3).GetChild(0).gameObject).onClick = OnClickBackMsg;   //返回按纽

    }
    void InitBottomInfo()
    {
        Transform BaseBottomTrans = m_BaseTrans.GetChild(1);
        m_WndTopInfo.m_MeadlLabel = BaseBottomTrans.GetChild(2).GetChild(1).GetComponent<UILabel>();
        m_WndTopInfo.m_DiamondLabel = BaseBottomTrans.GetChild(1).GetChild(1).GetComponent<UILabel>();
        m_WndTopInfo.m_GoldLabel = BaseBottomTrans.GetChild(0).GetChild(1).GetComponent<UILabel>();
        UIEventListener.Get(BaseBottomTrans.GetChild(1).GetChild(2).gameObject).onClick = OnDiamondPayMsg;
        UIEventListener.Get(BaseBottomTrans.GetChild(0).GetChild(2).gameObject).onClick = OnGoldPayMsg ;
        InitGoldEffect(BaseBottomTrans);

        UpdateUserGold();

    }
    void InitWndTopInfo()
    {
        Transform BaseTopTrans = m_BaseTrans.GetChild(0);
        for (byte i = 0; i < m_ShopWndBtns.Length; ++i )
        {
            m_ShopWndBtns[i] = BaseTopTrans.GetChild(i).gameObject;
            m_ShopWndBtnsCheckBg[i] = BaseTopTrans.GetChild(i + 5).gameObject ;
            UIEventListener.Get(m_ShopWndBtns[i]).onClick = onClickChangeShop;
        }
        //IOS审核屏蔽兑换处理
        if (ServerSetting.ShowExchange == false)
        {
            m_ShopWndBtns[4].transform.localPosition = m_ShopWndBtns[2].transform.localPosition;
            m_ShopWndBtnsCheckBg[4].transform.localPosition = m_ShopWndBtnsCheckBg[2].transform.localPosition;
            m_ShopWndBtns[2].SetActive(false);
        }
    }
    void InitChildWnd()
    {
        for (byte i = 0; i < m_ShopWndObj.Length; ++i )
        {
            m_ShopWndObj[i] = m_BaseTrans.GetChild(2).GetChild(i).gameObject;
            switch (i)
            {
                case (byte)Shop_Type.Shop_Property:
                    m_PropertyShopWnd.Init(m_ShopWndObj[i]);
                    break;
                case (byte)Shop_Type.Shop_Launcher:
                    m_DaPaoShopWnd.Init(m_ShopWndObj[i]);
                    break;
                case (byte)Shop_Type.Shop_Material:
                    m_MaterailShopWnd.Init(m_ShopWndObj[i]);
                    break;
                case (byte)Shop_Type.Shop_Submarine:
                    break;
                case (byte)Shop_Type.Shop_Knapsack:
                    m_KnapsackShopWnd.Init(m_ShopWndObj[i]);
                    break;
            }
            
        }
    }
    void InitGoldEffect(Transform parent)
    {
        for (byte i = 0; i < m_GoldEft.Length; ++i)
        {
            m_GoldEft[i] = GameObject.Instantiate(GlobalEffectMgr.Instance.UIGoldEffect[i]) as GameObject;
            m_GoldEft[i].transform.SetParent(parent.GetChild(i).GetChild(0), false);
            GlobalEffectMgr.SetEffectOnUI(m_GoldEft[i]);
        }
    }
    public void Update(float delta)
    {
        if (m_BaseWndObject == null && m_ConfirmBryWnd.m_BaseWndObject == null)
            return;

        m_ConfirmBryWnd.Update(delta);
    }
    public void UpateShopDate()
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf == false)
            return;
        ChangeShopWndUI();
    }
    public void ShowShopWndUI(Shop_Type type)
    {
        if (m_BaseWndObject != null)
            return;

        Init();
        SetUIStatue(type);
        WndManager.Instance.Push(m_BaseWndObject);
    }
    public void ShowConfirmBuyWnd(byte onlyID, byte shopID, uint itemSum = 1, bool bNewGide = false)
    {
        m_ConfirmBryWnd.ShowConfirmBuyWnd(onlyID, shopID,itemSum, bNewGide);
    }
    void SetUIStatue(Shop_Type tType)
    {
        m_ShopType = tType;
        ChangeTopBtnBg();
        ChangeShopWndUI();
    }
    void ChangeShopWndUI()
    {
        switch (m_ShopType)
        {
            case Shop_Type.Shop_Property:
                m_PropertyShopWnd.UpdatePropertyDate();
                break;
            case Shop_Type.Shop_Launcher:
                m_DaPaoShopWnd.UpdateDaPaoDate();
                break;
            case Shop_Type.Shop_Material:
                m_MaterailShopWnd.UpateMaterailDate();
                break;
            case Shop_Type.Shop_Submarine:
                break;
            case Shop_Type.Shop_Knapsack:
                m_KnapsackShopWnd.UpdateKnapsackDate();
                break;
        }
    }
    void ChangeTopBtnBg()
    {
        for (byte i = 0; i < m_ShopWndBtns.Length; ++i)
        {
            if (i == (byte)Shop_Type.Shop_Submarine || i == (byte)Shop_Type.Shop_Launcher)
                continue;
            if (ServerSetting.ShowExchange == false && i == (byte)Shop_Type.Shop_Material)
                continue;
            if ( (byte)m_ShopType == i)
            {
                m_ShopWndBtnsCheckBg[i].SetActive(true);
                m_ShopWndBtns[i].SetActive(false);
                m_ShopWndObj[i].SetActive(true);
            }
            else
            {
                m_ShopWndBtnsCheckBg[i].SetActive(false);
                m_ShopWndBtns[i].SetActive(true);
                m_ShopWndObj[i].SetActive(false);
            }
        }
    }
    void onClickChangeShop(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        for (byte i = 0; i < m_ShopWndBtns.Length; ++i)
        {
            if (i == (byte)Shop_Type.Shop_Submarine || i == (byte)Shop_Type.Shop_Launcher)
                continue;
            if (ServerSetting.ShowExchange == false && i == (byte)Shop_Type.Shop_Material)
                continue;
            if (go == m_ShopWndBtns[i])
            {
                m_ShopWndBtnsCheckBg[i].SetActive(true);
                m_ShopWndBtns[i].SetActive(false);
                m_ShopWndObj[i].SetActive(true);
                m_ShopType = (Shop_Type)i;
            }
            else
            {
                m_ShopWndBtnsCheckBg[i].SetActive(false);
                m_ShopWndBtns[i].SetActive(true);
                m_ShopWndObj[i].SetActive(false);
            }
        }
        ChangeShopWndUI();
    }
    public void UpdateUserGold()
    {
        m_WndTopInfo.m_DiamondLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency().ToString();
        m_WndTopInfo.m_GoldLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel().ToString();
        m_WndTopInfo.m_MeadlLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetMedal().ToString();
    }
    void OnClickBackMsg(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        ShutDown();
        return;
    }
    void OnGoldPayMsg(GameObject go)
    {
        SetMaskEft(false);
        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Gold);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void OnDiamondPayMsg(GameObject go)
    {
        SetMaskEft(false);

        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    public void SetMaskEft(bool bMask)
    {
        ShowGoldEffect(bMask);
    }
    void ShowGoldEffect(bool bShow)
    {
        for (byte i = 0; i < m_GoldEft.Length; ++i)
        {
            m_GoldEft[i].SetActive(bShow);
        }
    }
    public void ShutDown()
    { 
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_ConfirmBryWnd.ShutDown();
            ShopRuntime.Shutdown();
            if (HallRunTimeInfo.Instance_UI != null)
            {
                if (HallRunTimeInfo.Login_UI.GetWindStatue().m_state != HallLogicUIStatue.Hall_State.Hall_Contest)
                {
                    HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(true);
                }
            }
            m_BaseWndObject = null;
        }
    }

    public ConfirmBuyLogicUI ConfirmBryWnd
    {
        get 
        { 
            return m_ConfirmBryWnd;
        }
    }
    public KnapsackShopWnd KnapsackShopWnd
    {
        get 
        {
            return m_KnapsackShopWnd;
        }
    }
    public DaPaoShopWnd DaPaoShopWnd
    {
        get { return m_DaPaoShopWnd; }
    }
    public MaterailShopWnd MaterailShopWnd
    {
        get { return m_MaterailShopWnd; }
    }
    public PropertyDescWnd PropertyDesc
    {
        get { return m_PropertyDesc; }
    }
    public TreasurebowlLogic TreasurebowlWnd
    {
        get { return m_TreasurebowlWnd; }
        set { m_TreasurebowlWnd = value; }
    }
    public CBugleWnd BugleWnd
    {
        get { return m_buglewnd; }
    }
}
