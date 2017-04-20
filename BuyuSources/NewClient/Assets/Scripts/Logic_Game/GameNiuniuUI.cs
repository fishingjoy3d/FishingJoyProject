using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GameNiuniuUI : BaseWnd
{
    Object m_obj;
    Object m_EffectObj;
    GameObject m_HostEffectWnd;

    GameObject m_HeadWnd;
    Transform m_HeadTrans;
    GameObject[] m_LevelBtn = new GameObject[5];
    UIButton[] m_LevelUIBtn = new UIButton[5];
    Texture2D m_HeadTex;
    int[] levelArr;
    int level;
    int chooseLevel;

    GameObject m_PoolWnd;
    Transform m_PoolTrans;
    GameObject[] m_PoolBtn = new GameObject[4];
    byte poorIndex;
    bool isInWeager = false;
    float weagerCountDown;
    bool isAnim = false;
    float animCountDown;
    bool isShow = false;
    float showCountDown;
    int showIndex = 0;
    float normalWeagerCount;
    bool isNormalWeager = false;
    int normalGoldCount = 0;
    int normalIndex = 0;
    bool[] isNormal = new bool[4];
    float vipWeagerCount;
    bool isVipWeager = false;
    int vipGoldCount = 0;
    int seatIndex;
    int vipWeagerIndex;

    GameObject m_CalculateWnd;
    Transform m_CalculateTrans;

    GameObject m_TitleWnd;
    Transform m_TitleTrans;
    GameObject[] hostBtnObj;
    UILabel hostNameLabel;
    UILabel hostGoldNumLabel;
    GameObject hostHeadObj;
    Texture2D hostHeadTex;

    GameObject m_DescripWnd;
    Transform m_DescripTrans;

    GameObject m_VipSeatWnd;
    Transform m_VipSeatTrans;
    NiuniuVipSeat[] vipSeat;
    VipInfo[] vipInfo = new VipInfo[8];


    GameObject m_HostLineWnd;
    Transform m_HostLineTrans;
    GameObject closeBtn;
    UIGrid hostListGrid;
    GameObject hostItem;
    GameObject[] hostBtn;
    UILabel hostNameLineLabel;
    UILabel hostGameRestLabel;
    UILabel myLineNumLabel;
    string hostName;
    byte hostGameSum = 0;
    bool isInLine = false;
    bool isHost = false;
    int gameState = 0;
    bool isMasterHost = true;
    uint hostGoldSum;
    BankerUserInfo hostInfo;

    GameObject m_NormalWnd;
    Transform m_NormalTrans;
    GameObject closeNormalBtn;
    UIGrid normalGrid;
    GameObject normalItem;
    UILabel totalNormalLabel;
    GameObject nextPageBtn;
    GameObject frontPageBtn;
    UILabel pageLabel;
    uint currentPage = 0;
    uint totalPage = 0;
    bool stopNormalWeager = false;

    GameObject m_TrendWnd;
    Transform m_TrendTrans;
    GameObject closeTrendBtn;
    UISprite[] resultSprite;

    int[] myWeager = new int[4];            //四个我的下注
    ulong[] totalNum = new ulong[4];          //四个总下注
    byte[] cardResult = new byte[25];       //25张卡牌
    ushort[] result = new ushort[5];        //5个结果，未左移
    byte[] resultRate = new byte[5];        //5个倍率
    int[] resultLevel = new int[5];         //5个结果，左移后
    int[] resultNum = new int[4];           //4个输赢的数目
    long[] hostResultNum = new long[4];       //庄家的4个输赢数目
    uint[] poorGold = new uint[4];          //奖池中金币对象
    uint[] goldArr = new uint[5];           //每个下注等级对应的金币数量

    List<GameObject> weagerGold = new List<GameObject>();    //金币动画列表
    PoorGold[] m_poorGold = new PoorGold[4];

    bool isReturnToSystem = false;
    float returnSystemCount;
    bool isReturnToDesk = false;
    float returnDestCount;
    bool isReturnToPlayer = false;
    float returnPlayerCount;

    bool isRest = false;
    float restCount;

    bool isNext = false;
    float nextCount;

    bool isNewGold = false;
    float newGoldCount;
    int goldNum;

    bool isPress = false;
    float pressCount;
    float pressCounta;    
    public void InitObj()
    {
        if (m_obj != null)
            return;
        m_obj = ResManager.Instance.LoadObject("Niuniu", "GameRes/Niuniu/Prefab/UI/", ResType.GameRes);
        m_EffectObj = ResManager.Instance.LoadObject("Niuniu_Host", "GameRes/Niuniu/Prefab/Effect/", ResType.GameRes);
    }

    void Init()
    {
        m_BaseWndObject = (GameObject)GameObject.Instantiate(m_obj);
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_HeadTrans = m_BaseTrans.GetChild(0);
        m_HeadWnd = m_HeadTrans.gameObject;
        m_TitleTrans = m_BaseTrans.GetChild(1);
        m_TitleWnd = m_TitleTrans.gameObject;
        m_PoolTrans = m_BaseTrans.GetChild(2);
        m_PoolWnd = m_PoolTrans.gameObject;
        m_CalculateTrans = m_BaseTrans.GetChild(3);
        m_CalculateWnd = m_CalculateTrans.gameObject;
        m_DescripTrans = m_BaseTrans.GetChild(8);
        m_DescripWnd = m_DescripTrans.gameObject;
        m_VipSeatTrans = m_BaseTrans.GetChild(11);
        m_VipSeatWnd = m_VipSeatTrans.gameObject;
        m_HostLineTrans = m_BaseTrans.GetChild(12);
        m_HostLineWnd = m_HostLineTrans.gameObject;
        m_TrendTrans = m_BaseTrans.GetChild(13);
        m_TrendWnd = m_TrendTrans.gameObject;
        m_NormalTrans = m_BaseTrans.GetChild(14);
        m_NormalWnd = m_NormalTrans.gameObject;
        UIEventListener.Get(m_CalculateTrans.GetChild(4).gameObject).onClick = EventCloseResult;
        UIEventListener.Get(m_BaseTrans.GetChild(7).gameObject).onClick = EventOpenDesWnd;
        UIEventListener.Get(m_DescripTrans.GetChild(0).gameObject).onClick = EventCloseDesWnd;
        UIEventListener.Get(m_HeadTrans.GetChild(11).gameObject).onClick = EventAddGold;
        UIEventListener.Get(m_BaseTrans.GetChild(10).gameObject).onClick = EventOpenTrend;
        UIEventListener.Get(m_BaseTrans.GetChild(9).gameObject).onClick = EventOpenNormal;
        InitLevelBtn();
        InitPlayerInfo();
        InitPoolBtn();
        InitTitle();
        InitVipSeat();
        InitHostLine();
        InitTrend();
        InitNormal();
        UIEventListener.Get(m_BaseTrans.GetChild(4).gameObject).onClick = EventCloseGameWnd;
    }

    void InitNormal()
    {
        closeNormalBtn = m_NormalTrans.GetChild(0).gameObject;
        normalGrid = m_NormalTrans.GetChild(1).GetChild(1).GetComponent<UIGrid>();
        normalItem = m_NormalTrans.GetChild(1).GetChild(0).gameObject;
        totalNormalLabel = m_NormalTrans.GetChild(2).GetChild(0).GetComponent<UILabel>();
        nextPageBtn = m_NormalTrans.GetChild(4).gameObject;
        frontPageBtn = m_NormalTrans.GetChild(3).gameObject;
        pageLabel = m_NormalTrans.GetChild(5).GetComponent<UILabel>();
        UIEventListener.Get(closeNormalBtn).onClick = EventCloseNormalWnd;
        UIEventListener.Get(nextPageBtn).onClick = EventNextPage;
        UIEventListener.Get(frontPageBtn).onClick = EventFrontPage;
        stopNormalWeager = false;
    }

    void InitTrend()
    {
        closeTrendBtn = m_TrendTrans.GetChild(0).gameObject;
        UIEventListener.Get(closeTrendBtn).onClick = EventCloseTren;
        resultSprite = new UISprite[32];
        for (byte i = 0; i < m_TrendTrans.GetChild(1).childCount; i++)
        {
            resultSprite[i] = m_TrendTrans.GetChild(1).GetChild(i).GetComponent<UISprite>();
        }
    }

    void InitVipSeat()
    {
        vipSeat = new NiuniuVipSeat[8];
        for (byte i = 0; i < vipSeat.Length; i++)
        {
            vipSeat[i] = new NiuniuVipSeat();
            vipSeat[i].Init(m_VipSeatTrans.GetChild(i), i);
            vipInfo[i] = new VipInfo();
            for (byte j = 0; j < 4; j++)
            {
                vipInfo[i].weagerIndex[j] = false;
            }
        }
    }

    void InitHostLine()
    {
        closeBtn = m_HostLineTrans.GetChild(1).gameObject;
        UIEventListener.Get(closeBtn).onClick = EventCloseHostLine;
        hostBtn = new GameObject[4];
        hostBtn[0] = m_HostLineTrans.GetChild(5).gameObject;
        hostBtn[1] = m_HostLineTrans.GetChild(6).gameObject;
        hostBtn[2] = m_HostLineTrans.GetChild(7).gameObject;
        hostBtn[3] = m_HostLineTrans.GetChild(8).gameObject;

        UIEventListener.Get(hostBtn[0]).onClick = EventSnatchHost;
        UIEventListener.Get(hostBtn[1]).onClick = EventCancelHostLine;
        UIEventListener.Get(hostBtn[2]).onClick = EventDownHost;
        UIEventListener.Get(hostBtn[3]).onClick = EventUpHost;
        hostListGrid = m_HostLineTrans.GetChild(0).GetChild(1).GetComponent<UIGrid>();
        hostItem = m_HostLineTrans.GetChild(0).GetChild(0).gameObject;

        hostNameLineLabel = m_HostLineTrans.GetChild(2).GetComponent<UILabel>();
        hostGameRestLabel = m_HostLineTrans.GetChild(3).GetComponent<UILabel>();
        myLineNumLabel = m_HostLineTrans.GetChild(4).GetComponent<UILabel>();

        hostBtn[0].SetActive(false);
        hostBtn[1].SetActive(false);
        hostBtn[2].SetActive(false);
        hostBtn[3].SetActive(true);
        isHost = false;
        isInLine = false;
        hostInfo = null;
        isMasterHost = true;
    }

    void InitTitle()
    {
        m_HostEffectWnd = (GameObject)GameObject.Instantiate(m_EffectObj);
        Transform trans = m_HostEffectWnd.transform;
        trans.SetParent(m_BaseTrans, false);
        GlobalEffectMgr.SetTrailEffectOnTopWnd(m_HostEffectWnd);
        GlobalEffectMgr.SetMeshRendererOnTopWnd(m_HostEffectWnd);

        hostBtnObj = new GameObject[2];
        hostBtnObj[0] = m_TitleTrans.GetChild(2).gameObject;
        hostBtnObj[1] = m_TitleTrans.GetChild(3).gameObject;
        UIEventListener.Get(hostBtnObj[0]).onClick = EventUpHost;
        UIEventListener.Get(hostBtnObj[1]).onClick = EventDownHost;
        UIEventListener.Get(trans.GetChild(0).GetChild(0).gameObject).onClick = EventOpenHostLine;
        hostNameLabel = m_TitleTrans.GetChild(5).GetComponent<UILabel>();
        hostGoldNumLabel = m_TitleTrans.GetChild(5).GetChild(0).GetComponent<UILabel>();
        hostHeadObj = m_TitleTrans.GetChild(4).gameObject;

        hostBtnObj[0].SetActive(true);
        hostBtnObj[1].SetActive(false);
        hostNameLabel.gameObject.SetActive(false);
        hostHeadObj.SetActive(false);
    }

    void InitLevelBtn()
    {
        for (byte i = 0; i < m_LevelBtn.Length; i++)
        {
            m_LevelBtn[i] = m_HeadTrans.GetChild(i).gameObject;
            UIEventListener.Get(m_LevelBtn[i]).onClick = EventChangeLevel;
            m_LevelUIBtn[i] = m_LevelBtn[i].GetComponent<UIButton>();
        }
        levelArr = new int[] { 1000, 10000, 100000, 500000, 1000000 };
        goldArr = new uint[] { 1, 2, 5, 7, 10 };
        level = 0;
    }

    public void SetMyGoldNum()
    {
        if (!isAnim)
        {
            uint goldNum = PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel();
            if (goldNum >= 10000)
            {
                m_HeadTrans.GetChild(8).GetComponent<UILabel>().text = (goldNum / 10000) + "." + (goldNum / 1000 % 10) + " 万";
            }
            else
            {
                m_HeadTrans.GetChild(8).GetComponent<UILabel>().text = goldNum.ToString();
            }
        }
    }
    void InitPlayerInfo()
    {
        SetMyGoldNum();
        m_HeadTrans.GetChild(7).GetComponent<UILabel>().text = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();
        uint ficeID = PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID();
        if (ficeID < ConstValue.CUSTOM_HEADER)
        {
            m_HeadTex = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
            m_HeadTrans.GetChild(6).GetChild(0).GetComponent<UITexture>().mainTexture = m_HeadTex;
        }
        else
        {
            //获取头像
            //  m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[0];
            HeaderManager.Instance.GetPlayerHeader(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), ficeID, HeaderOptType.HEADER_NIUNIU, null);
        }
    }

    void InitPoolBtn()
    {
        for (byte i = 0; i < m_PoolBtn.Length; i++)
        {
            m_PoolBtn[i] = m_PoolTrans.GetChild(i).gameObject;
            UIEventListener.Get(m_PoolBtn[i]).onClick = EventWagerChoose;
            UIEventListener.Get(m_PoolBtn[i]).onPress = EventPressChoose;
            m_poorGold[i] = new PoorGold();
            myWeager[i] = 0;
            resultNum[i] = 0;
            hostResultNum[i] = 0;
            poorGold[i] = 0;
        }
    }

    public void ShowNiuniuWnd()
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseWndObject);
        }
        GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound5);
    }

    void EventCloseGameWnd(GameObject go)
    {
        if (myWeager[0] + myWeager[1] + myWeager[2] + myWeager[3] > 0 && isInWeager)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_LeaveRoom"), MssageType.NiuNiu_Exit_Tips);
        }
        else if (isInLine && !isHost)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_LineLeave"), MssageType.NiuNiu_Exit_Tips);
        }
        else if (isHost && !isInLine)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_HostLeave"), MssageType.NiuNiu_Exit_Tips);
        }
        else
        {
            PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnLeaveNiuNiuTable();
            ShutDown();
        }
    }

    void EventChangeLevel(GameObject go)
    {
        m_HeadTrans.GetChild(5).position = go.transform.position;
        for (byte i = 0; i < m_LevelBtn.Length; i++)
        {
            if (go == m_LevelBtn[i])
            {
                level = i;
                break;
            }
        }
    }

    void EventWagerChoose(GameObject go)
    {
        if (isInWeager)
        {
            for (byte i = 0; i < 4; i++)
            {
                if (go == m_PoolTrans.GetChild(i).gameObject)
                {
                    poorIndex = i;
                    bool isSucess = PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnAddGlobel((uint)levelArr[level], poorIndex);
                    chooseLevel = level;
                    if (isSucess)
                        break;
                    else
                        isPress = false;
                }
            }
        }
    }
    void EventPressChoose(GameObject go, bool state)
    {
        if (state)
        {
            if (isInWeager)
            {
                isPress = true;
                pressCount = 0f;
                pressCounta = 0f;
                for (byte i = 0; i < 4; i++)
                {
                    if (go == m_PoolTrans.GetChild(i).gameObject)
                    {
                        poorIndex = i;
                        break;
                    }
                }
            }
        }
        else
        {
            isPress = false;
        }

    }
    void EventAddGold(GameObject go)
    {
        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Gold);
    }

    void ShowResult()                   //显示结算ui
    {
        double dGold = 0;
        if (myWeager[0] + myWeager[1] + myWeager[2] + myWeager[3] != 0)
        {
            m_CalculateTrans.GetComponent<TweenScale>().ResetToBeginning();
            m_CalculateTrans.GetComponent<TweenScale>().PlayForward();
            double total = resultNum[0] + resultNum[1] + resultNum[2] + resultNum[3];
            dGold = total;
            if (total >= 0)
            {
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu18);
                m_CalculateTrans.GetChild(2).gameObject.SetActive(true);
                m_CalculateTrans.GetChild(3).gameObject.SetActive(false);
                if (total > 10000)
                {
                    total = total / 10000;
                    m_CalculateTrans.GetChild(1).GetChild(0).gameObject.SetActive(true);
                }
                else
                    m_CalculateTrans.GetChild(1).GetChild(0).gameObject.SetActive(false);
                m_CalculateTrans.GetChild(1).GetComponent<UILabel>().text = "+" + total;
            }
            else if (total < 0)
            {
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu19);
                m_CalculateTrans.GetChild(2).gameObject.SetActive(false);
                m_CalculateTrans.GetChild(3).gameObject.SetActive(true);
                if (total < -10000)
                {
                    total = total / 10000;
                    m_CalculateTrans.GetChild(1).GetChild(0).gameObject.SetActive(true);
                }
                else
                    m_CalculateTrans.GetChild(1).GetChild(0).gameObject.SetActive(false);
                m_CalculateTrans.GetChild(1).GetComponent<UILabel>().text = total.ToString();
            }
            m_CalculateTrans.GetChild(0).GetChild(0).GetComponent<UITexture>().mainTexture = m_HeadTex;
            WndManager.Instance.Push(m_CalculateWnd);
            m_CalculateWnd.SetActive(true);
        }

        if (isHost)
        {
            if (totalNum[0] + totalNum[1] + totalNum[2] + totalNum[3] > 0)
            {
                m_CalculateTrans.GetComponent<TweenScale>().ResetToBeginning();
                m_CalculateTrans.GetComponent<TweenScale>().PlayForward();
                double total = hostResultNum[0] + hostResultNum[1] + hostResultNum[2] + hostResultNum[3];
                dGold = total;
                if (total >= 0)
                {
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu18);
                    m_CalculateTrans.GetChild(2).gameObject.SetActive(true);
                    m_CalculateTrans.GetChild(3).gameObject.SetActive(false);
                    if (total > 10000)
                    {
                        total = total / 10000;
                        m_CalculateTrans.GetChild(1).GetChild(0).gameObject.SetActive(true);
                    }
                    else
                        m_CalculateTrans.GetChild(1).GetChild(0).gameObject.SetActive(false);
                    m_CalculateTrans.GetChild(1).GetComponent<UILabel>().text = "+" + total;
                }
                else if (total < 0)
                {
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu19);
                    m_CalculateTrans.GetChild(2).gameObject.SetActive(false);
                    m_CalculateTrans.GetChild(3).gameObject.SetActive(true);
                    if (total < -10000)
                    {
                        total = total / 10000;
                        m_CalculateTrans.GetChild(1).GetChild(0).gameObject.SetActive(true);
                    }
                    else
                        m_CalculateTrans.GetChild(1).GetChild(0).gameObject.SetActive(false);
                    m_CalculateTrans.GetChild(1).GetComponent<UILabel>().text = total.ToString();
                }
                m_CalculateTrans.GetChild(0).GetChild(0).GetComponent<UITexture>().mainTexture = m_HeadTex;
                WndManager.Instance.Push(m_CalculateWnd);
                m_CalculateWnd.SetActive(true);
            }
        }
        GlobalHallUIMgr.Instance.GameShare.AddGlod((int)dGold);
    }

    void EventCloseResult(GameObject go)        //关闭结算UI
    {
        WndManager.Instance.Pop(m_CalculateWnd);
        m_CalculateWnd.SetActive(false);

        //SetMyGoldNum();
        isAnim = false;
        isInWeager = false;
        if (!isHost)
        {
            m_PoolTrans.GetChild(4).gameObject.SetActive(false);
            m_PoolTrans.GetChild(5).gameObject.SetActive(true);
        }
        m_BaseTrans.GetChild(6).gameObject.SetActive(true);
        ShowNiuniuBegin();

    }

    void EventOpenDesWnd(GameObject go)
    {
        WndManager.Instance.Push(m_DescripWnd);
        m_DescripWnd.SetActive(true);
    }

    void EventCloseDesWnd(GameObject go)
    {
        WndManager.Instance.Pop(m_DescripWnd);
        m_DescripWnd.SetActive(false);
    }

    void EventOpenHostLine(GameObject go)
    {
        WndManager.Instance.Push(m_HostLineWnd);
        m_HostLineWnd.SetActive(true);

        if (!isInLine && !isHost)
        {
            hostBtn[0].SetActive(false);
            hostBtn[1].SetActive(false);
            hostBtn[2].SetActive(false);
            hostBtn[3].SetActive(true);
            return;
        }

        if (isInLine)
        {
            hostBtn[0].SetActive(true);
            hostBtn[1].SetActive(true);
            hostBtn[2].SetActive(false);
            hostBtn[3].SetActive(false);
            return;
        }

        if (isHost)
        {
            hostBtn[0].SetActive(false);
            hostBtn[1].SetActive(false);
            hostBtn[2].SetActive(true);
            hostBtn[3].SetActive(false);
            return;
        }

    }

    void EventUpHost(GameObject go)
    {
        //判断是否满足上庄条件
        uint globalSum = FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.JoinBankerGlobelSum;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < globalSum)
        {
            string str = StringTable.GetString("UOM_Niuniu_UpHostFail");
            string newStr = str.Replace("@", globalSum.ToString());
            GlobalEffectMgr.Instance.ShowMessageBox(newStr, MssageType.NiuNiu_UpHostFail_Tips);
        }
        if (PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnRoleJoinWriteBankerList())
        {
            isInLine = true;
            if (go == hostBtnObj[0])
                EventOpenHostLine(null);
            else
            {
                hostBtn[0].SetActive(true);
                hostBtn[1].SetActive(true);
                hostBtn[2].SetActive(false);
                hostBtn[3].SetActive(false);
            }
            hostBtnObj[0].SetActive(false);
        }
    }

    public void GetHostLine(WriteBankerList list)
    {
        ClearGrid(hostListGrid);
        for (byte i = 0; i < FishDataInfo.MAX_NIUNIU_BankerShowSum; i++)
        {
            if (list.List[i].dwUserID == 0)
            {
                break;
            }
            NiuNiuRoleInfo info = list.List[i];
            HostItem item = new HostItem((uint)i + 1, info.NickName, info.GlobelSum);
            Transform trans = item.InitItem(hostItem.transform);
            hostListGrid.AddChild(trans);
            trans.localScale = Vector3.one;

        }
        if (list.MySeatIndex == 0xffffffff && !isHost)
        {
            myLineNumLabel.text = "未排队";
        }
        else
        {
            if (isInLine)
            {
                myLineNumLabel.text = (list.MySeatIndex + 1).ToString();
                hostBtnObj[0].SetActive(false);
                hostBtn[0].SetActive(true);
                hostBtn[1].SetActive(true);
                hostBtn[2].SetActive(false);
                hostBtn[3].SetActive(false);
            }
            else if (isHost)
                myLineNumLabel.text = "正在上庄";
        }
    }

    void EventCancelHostLine(GameObject go)
    {
        if (PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnRoleLeaveWriteBankerList())
        {
            isInLine = false;
            isHost = false;
            hostBtn[0].SetActive(false);
            hostBtn[1].SetActive(false);
            hostBtn[2].SetActive(false);
            hostBtn[3].SetActive(true);

            hostBtnObj[0].SetActive(true);
            hostBtnObj[1].SetActive(false);
        }
    }

    void EventDownHost(GameObject go)
    {
        if (PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnRoleCanelBanker())
        {
            isInLine = false;
            isHost = false;
            hostBtn[0].SetActive(false);
            hostBtn[1].SetActive(false);
            hostBtn[2].SetActive(false);
            hostBtn[3].SetActive(true);

            hostBtnObj[0].SetActive(true);
            hostBtnObj[1].SetActive(false);
        }
    }

    public void UpdateHost(BankerUserInfo info)
    {
        if (info.UserInfo.dwUserID == 0)
        {
            hostHeadObj.SetActive(false);
            hostNameLabel.gameObject.SetActive(false);
            m_TitleTrans.GetChild(7).gameObject.SetActive(true);
            hostName = "解闷大师";
            hostNameLineLabel.text = hostName;
            hostGameRestLabel.text = "";
            isMasterHost = true;
            if (isInLine)
            {
                hostBtnObj[0].SetActive(false);
                hostBtnObj[1].SetActive(false);
                hostBtn[0].SetActive(true);
                hostBtn[1].SetActive(true);
                hostBtn[2].SetActive(false);
                hostBtn[3].SetActive(false);
            }
            else
            {
                hostBtnObj[0].SetActive(true);
                hostBtnObj[1].SetActive(false);
                hostBtn[0].SetActive(false);
                hostBtn[1].SetActive(false);
                hostBtn[2].SetActive(false);
                hostBtn[3].SetActive(true);
            }
        }
        else
        {
            hostHeadObj.SetActive(true);
            hostNameLabel.gameObject.SetActive(true);
            m_TitleTrans.GetChild(7).gameObject.SetActive(false);
            isMasterHost = false;
            hostGoldSum = info.UserInfo.GlobelSum;

            hostName = info.UserInfo.NickName;
            hostGameSum = info.GameSum;
            hostNameLabel.text = info.UserInfo.NickName;
            uint goldNum = info.UserInfo.GlobelSum;
            hostGoldNumLabel.text = goldNum.ToString();
            if (goldNum > 10000)
            {
                hostGoldNumLabel.text = (goldNum / 10000) + "." + (goldNum / 1000 % 10) + "万";
            }

            hostNameLineLabel.text = hostName;
            hostGameRestLabel.text = (FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.BankerGameSum - hostGameSum).ToString();

            if (info.UserInfo.NickName.Length > 4)
                hostNameLabel.text = info.UserInfo.NickName.Substring(0, 4) + "...";

            uint ficeID = info.UserInfo.dwFaceID;
            if (ficeID < ConstValue.CUSTOM_HEADER)
            {
                hostHeadTex = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
                hostHeadObj.transform.GetChild(0).GetComponent<UITexture>().mainTexture = hostHeadTex;
            }
            else
            {
                //获取头像
                HeaderManager.Instance.GetPlayerHeader(info.UserInfo.dwUserID, ficeID, HeaderOptType.HEADER_NIUNIUHOST, null);
            }

            if (hostName.Equals(PlayerRole.Instance.RoleInfo.RoleMe.GetNickName()))
            {
                myLineNumLabel.text = "正在上庄";
                isInLine = false;
                isHost = true;
                hostBtnObj[0].gameObject.SetActive(false);
                hostBtnObj[1].gameObject.SetActive(true);

                hostBtn[0].SetActive(false);
                hostBtn[1].SetActive(false);
                hostBtn[2].SetActive(true);
                hostBtn[3].SetActive(false);
            }
            else
            {
                isHost = false;
                if (isInLine)
                {
                    hostBtnObj[0].SetActive(false);
                    hostBtnObj[1].SetActive(false);
                    hostBtn[0].SetActive(true);
                    hostBtn[1].SetActive(true);
                    hostBtn[2].SetActive(false);
                    hostBtn[3].SetActive(false);
                }
                else
                {
                    hostBtnObj[0].SetActive(true);
                    hostBtnObj[1].SetActive(false);
                    hostBtn[0].SetActive(false);
                    hostBtn[1].SetActive(false);
                    hostBtn[2].SetActive(false);
                    hostBtn[3].SetActive(true);
                }
            }
        }
    }

    void ClearGrid(UIGrid grid)
    {
        List<Transform> gridChid = grid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        grid.transform.DetachChildren();
    }

    void EventCloseHostLine(GameObject go)
    {
        WndManager.Instance.Pop(m_HostLineWnd);
        m_HostLineWnd.SetActive(false);
    }

    void EventSnatchHost(GameObject go)
    {
        uint needGlobal = FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.GetNextBankerNeedGlobel;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() > needGlobal)
        {
            string str = StringTable.GetString("UOM_Niuniu_SnatchHost");
            string newStr = str.Replace("@", needGlobal.ToString());
            GlobalEffectMgr.Instance.ShowMessageBox(newStr, MssageType.NiuNiu_SantchHost_Tips);
        }
    }

    void EventOpenTrend(GameObject go)
    {
        WndManager.Instance.Push(m_TrendWnd);
        m_TrendWnd.SetActive(true);
    }

    void EventCloseTren(GameObject go)
    {
        WndManager.Instance.Pop(m_TrendWnd);
        m_TrendWnd.SetActive(false);
    }

    public void SnatchHost()
    {
        PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnRoleGetWriteBankerFirstSeat();
    }

    void UpdateVipInfo(VipSeatList list)
    {
        for (byte i = 0; i < list.List.Length; i++)
        {
            if (list.List[i].dwUserID != 0)
            {
                vipSeat[i].sitDownBtn.SetActive(false);
                vipSeat[i].headObj.SetActive(true);
                uint moneyNum = list.List[i].GlobelSum;
                vipSeat[i].moneyLabel.text = moneyNum.ToString();
                if (moneyNum > 10000)
                    vipSeat[i].moneyLabel.text = (moneyNum / 10000) + "." + (moneyNum / 1000 % 10) + "万";
                vipSeat[i].moneyLabel.gameObject.SetActive(true);

                uint ficeID = list.List[i].dwFaceID;
                if (ficeID < ConstValue.CUSTOM_HEADER)
                {
                    vipSeat[i].headTex = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
                    vipSeat[i].headObj.transform.GetChild(0).GetComponent<UITexture>().mainTexture = vipSeat[i].headTex;
                }
                else
                {
                    //获取头像
                    HeaderManager.Instance.GetPlayerHeader(list.List[i].dwUserID, ficeID, HeaderOptType.HEADER_NIUNIUVIP, i);
                }

                vipSeat[i].userId = list.List[i].dwUserID;
            }
            else
            {
                vipSeat[i].sitDownBtn.SetActive(true);
                vipSeat[i].headObj.SetActive(false);
                vipSeat[i].moneyLabel.gameObject.SetActive(false);
                vipSeat[i].getSeatBtn.SetActive(false);
                vipSeat[i].postCardBtn.SetActive(false);
            }
        }
    }

    public void VipSeatChange(bool result, VipSeatList list)
    {
        if (!result)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_JoinVip_Fail2"), MssageType.NiuNiu_UpHostFail_Tips);
        }
        else
        {
            UpdateVipInfo(list);
            for (byte i = 0; i < 8; i++)
            {
                if (list.List[i].dwUserID != vipInfo[i].userId)
                {
                    for (byte j = 0; j < 4; j++)
                    {
                        vipInfo[j].weagerIndex[j] = false;
                    }
                }
            }
        }
    }

    public void SetVipHead(byte id, Texture2D tex)
    {
        vipSeat[id].headObj.transform.GetChild(0).GetComponent<UITexture>().mainTexture = tex;
        vipSeat[id].headTex = tex;
    }

    public void BeLeaveVip(NiuNiuRoleInfo info)
    {
        for (byte i = 0; i < vipSeat.Length; i++)
        {
            if (vipSeat[i].userId == info.dwUserID)
            {
                string str = StringTable.GetString("UOM_Niuniu_JoinVip_Fail3");
                string strNew = str.Replace("@", info.NickName);
                GlobalEffectMgr.Instance.ShowMessageBox(strNew, MssageType.NiuNiu_UpHostFail_Tips);
            }
        }
    }

    void UpdateTrend(NiuNiuGameLog log)
    {
        int start = 0;

        if (log.GameSum <= 8)
        {
            start = (8 - log.GameSum) * 4;
            for (byte i = 0; i < start; i++)
            {
                resultSprite[i].gameObject.SetActive(false);
            }
            for (int i = start; i < 32; i++)
            {
                resultSprite[i].gameObject.SetActive(true);
            }
        }
        for (int i = start, j = 0; i < 32; i++, j++)
        {
            bool result = (log.GameLog & (1 << j)) != 0;
            if (result)
            {
                resultSprite[i].spriteName = "Fail";
            }
            else
            {
                resultSprite[i].spriteName = "Win";
            }
        }
    }

    void EventOpenNormal(GameObject go)
    {
        WndManager.Instance.Push(m_NormalWnd);
        m_NormalWnd.SetActive(true);

        PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnGetNormalInfo(0);
        currentPage = 0;
        totalPage = 0;
    }

    public void GetNormalPeople(uint page, uint sum, NiuNiuRoleInfo[] array)
    {
        ClearGrid(normalGrid);
        if (sum % 9 == 0)
            totalPage = sum / 9;
        else
            totalPage = sum / 9 + 1;
        string str = StringTable.GetString("UOM_Niuniu_Normal_Sum");
        string newStr = str.Replace("@", sum.ToString());
        totalNormalLabel.text = newStr;
        pageLabel.text = (currentPage + 1) + "/" + totalPage;
        if (totalPage == 0)
            pageLabel.text = (currentPage + 1) + "/1";
        for (byte i = 0; i < array.Length; i++)
        {
            if (array[i].dwUserID == 0)
            {
                break;
            }
            NormalItem item = new NormalItem(array[i].NickName, array[i].GlobelSum);
            Transform itemTrans = item.Init(normalItem.transform);
            normalGrid.AddChild(itemTrans);
            itemTrans.localScale = Vector3.one;

            uint ficeID = array[i].dwFaceID;
            if (ficeID < ConstValue.CUSTOM_HEADER)
            {
                item.headTex = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
                item.headObj.GetComponent<UITexture>().mainTexture = item.headTex;
            }
            else
            {
                //获取头像
                HeaderManager.Instance.GetPlayerHeader(array[i].dwUserID, ficeID, HeaderOptType.HEADER_NIUNIUNORMAL, i);
            }
        }
    }

    public void SetNormalHead(Texture2D tex, uint id, byte index)
    {
        normalGrid.GetChild(index).GetChild(0).GetComponent<UITexture>().mainTexture = tex;
    }


    void EventCloseNormalWnd(GameObject go)
    {
        WndManager.Instance.Pop(m_NormalWnd);
        m_NormalWnd.SetActive(false);

    }

    void EventNextPage(GameObject go)
    {
        if (currentPage + 1 < totalPage)
        {
            currentPage++;
            PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnGetNormalInfo(currentPage);
        }
        else
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("UOM_Niuniu_NextPage_Fail"), 1.0f, false);
    }

    void EventFrontPage(GameObject go)
    {
        if (currentPage == 0)
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("UOM_Niuniu_FrontPage_Fail"), 1.0f, false);
        else
        {
            currentPage--;
            PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnGetNormalInfo(currentPage);
        }
    }

    public void VipWeager(byte seat, uint addGlobal, byte index, uint allGlobal)
    {
        if (vipSeat[seat].userId == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            return;
        if (vipSeat[seat].userId != 0)
            vipInfo[seat].weagerIndex[index] = true;
        vipSeat[seat].moneyLabel.text = allGlobal.ToString();
        if (allGlobal > 10000)
            vipSeat[seat].moneyLabel.text = (allGlobal / 10000) + "." + (allGlobal / 1000 % 10) + "万";
        vipWeagerIndex = index;
        seatIndex = seat;
        vipWeagerCount = 0;
        vipGoldCount = 0;
        isVipWeager = true;
    }

    public void VipGlobalChange(uint id, byte seat, uint globalSum)
    {
        if (id != 0)
        {
            vipSeat[seat].moneyLabel.text = globalSum.ToString();
            if (globalSum > 10000)
                vipSeat[seat].moneyLabel.text = (globalSum / 10000) + "." + (globalSum / 1000 % 10) + "万";
        }
    }

    public void StopWeager()
    {
        stopNormalWeager = true;
    }

    uint m_RemoveTick;
    public void Update(float deltaTime)
    {
        if (m_BaseTrans == null)
            return;

        if (isInWeager)
        {
            float fOldTime = weagerCountDown;
            weagerCountDown -= deltaTime;
            m_PoolTrans.GetChild(4).GetChild(0).GetComponent<UILabel>().text = ((int)weagerCountDown).ToString();
            if (weagerCountDown <= 0)
            {
                isInWeager = false;
            }
                     
            if((int)fOldTime-(int)weagerCountDown==1)
            {               
                if ((int)weagerCountDown == 0)
                {
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_cdover);
                }
                else if ((int)weagerCountDown <=5)
                {
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_cd);
                }
            }
        }

        if (isRest)
        {
            restCount -= deltaTime;
            m_PoolTrans.GetChild(5).GetChild(0).GetComponent<UILabel>().text = ((int)restCount).ToString();
            if (restCount <= 0)
            {
                isRest = false;
            }
        }

        if (isNext)
        {
            nextCount -= deltaTime;
            m_PoolTrans.GetChild(6).GetChild(0).GetComponent<UILabel>().text = ((int)nextCount).ToString();
            if (nextCount <= 0)
            {
                isNext = false;
            }
        }

        if (isAnim)
        {
            animCountDown -= deltaTime;
            if (animCountDown <= 0)
            {
                isShow = true;
                isAnim = false;
                showIndex = 0;
                showCountDown = 0.5f;
            }
        }
        if (isShow)
        {
            showCountDown -= deltaTime;
            if (showCountDown <= 0)
            {
                if (showIndex == 0)
                {
                    ShowOneResult(m_TitleTrans, 0);
                }
                else if (showIndex < 5)
                {
                    ShowOneResult(m_PoolBtn[showIndex - 1].transform, showIndex);
                }
                showIndex++;
                showCountDown = 0.8f;
                if (showIndex == 5)
                {
                    showCountDown = 1.0f;
                }
                else if (showIndex == 6)
                {
                    showCountDown = 5.0f;
                    GoldFlyToSystem();
                }
                else if (showIndex == 7)
                {
                    ShowResult();
                    m_BaseTrans.GetComponent<Animator>().SetBool("isAnim", false);
                    isShow = false;
                    isAnim = false;
                }
            }
        }
        if (isReturnToSystem)
        {
            returnSystemCount -= deltaTime;
            if (returnSystemCount <= 0)
            {
                int flag = 1;
                for (byte i = 1; i < 5; i++)
                {
                    //if (result[i] > result[0] && myWeager[i - 1] == 0)
                    //{
                    //    if (goldNum < m_poorGold[i - 1].weagerGold.Count)
                    //    {
                    //        m_poorGold[i - 1].weagerGold[goldNum].SetActive(false);
                    //        flag = 0;
                    //    }
                    //}
                    if (result[i] < result[0] && totalNum[i - 1] > 0)
                    {
                        if (goldNum < m_poorGold[i - 1].weagerGold.Count)
                        {
                            m_poorGold[i - 1].goldTween[goldNum].from = m_poorGold[i - 1].weagerGold[goldNum].transform.localPosition;
                            m_poorGold[i - 1].goldTween[goldNum].to = new Vector3(125, 518, 0);
                            m_poorGold[i - 1].goldTween[goldNum].duration = 0.5f;
                            if (m_poorGold[i - 1].weagerGold.Count > 30)
                            {
                                m_poorGold[i - 1].goldTween[goldNum].duration = 0.3f;
                            }
                            m_poorGold[i - 1].goldTween[goldNum].enabled = true;
                            m_poorGold[i - 1].goldTween[goldNum].ResetToBeginning();
                            m_poorGold[i - 1].goldTween[goldNum].PlayForward();
                            flag = 0;
                            GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);
                        }
                    }
                }
                returnSystemCount = 0.01f;
                goldNum++;
                if (flag == 1)
                {
                    if (myWeager[0] + myWeager[1] + myWeager[2] + myWeager[3] == 0)
                        m_PoolTrans.GetChild(6).gameObject.SetActive(true);
                    GoldFlyToDesk();
                    isReturnToSystem = false;
                }

            }
        }
        if (isReturnToDesk)
        {
            returnDestCount -= deltaTime;
            if (returnDestCount <= 0)
            {
                for (byte i = 1; i < 5; i++)
                {
                    if (result[i] > result[0] && totalNum[i - 1] > 0)
                    {
                        Vector3 beginPos = new Vector3(187, 518, 0);
                        GameObject goldItem = GameObject.Instantiate(m_BaseTrans.GetChild(5).gameObject);
                        goldItem.transform.SetParent(m_BaseTrans.GetChild(6), false);
                        goldItem.SetActive(true);
                        m_poorGold[i - 1].weagerGold.Add(goldItem);
                        m_poorGold[i - 1].goldTween.Add(goldItem.GetComponent<TweenPosition>());
                        Vector3 endPos;
                        if (i == 1)
                            endPos = new Vector3(Random.Range(7, 107), Random.Range(320, 388), 0);
                        else if (i == 2)
                            endPos = new Vector3(Random.Range(167, 267), Random.Range(320, 388), 0);
                        else if (i == 3)
                            endPos = new Vector3(Random.Range(337, 437), Random.Range(320, 388), 0);
                        else
                            endPos = new Vector3(Random.Range(497, 597), Random.Range(320, 388), 0);
                        goldItem.GetComponent<TweenPosition>().from = beginPos;
                        goldItem.GetComponent<TweenPosition>().to = endPos;
                        m_poorGold[i - 1].goldTween[goldNum].duration = 0.3f;
                        goldItem.GetComponent<TweenPosition>().enabled = true;
                        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);
                    }
                }
                goldNum++;
                returnDestCount = 0.01f;
                if (goldNum == 10)
                {
                    GoldFlyToPlayer();
                    isReturnToDesk = false;
                }
            }
        }
        if (isReturnToPlayer)
        {
            returnPlayerCount -= deltaTime;
            if (returnPlayerCount <= 0)
            {
                int flag = 1;
                for (byte i = 1; i < 5; i++)
                {
                    if (result[i] > result[0])
                    {
                        int flaga = 1;
                        for (byte j = 0; j < 8; j++)
                        {
                            if (vipInfo[j].userId != 0 && vipInfo[j].userId != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
                            {
                                if (vipInfo[j].weagerIndex[i - 1])
                                {
                                    if (goldNum < 5)
                                    {
                                        m_poorGold[i - 1].goldTween[goldNum].from = m_poorGold[i - 1].weagerGold[goldNum].transform.localPosition;
                                        Vector3 vector;
                                        switch (seatIndex)
                                        {
                                            case 0: vector = new Vector3(-97, 465, 0);
                                                break;
                                            case 1: vector = new Vector3(-176, 368, 0);
                                                break;
                                            case 2: vector = new Vector3(-168, 29, 0);
                                                break;
                                            case 3: vector = new Vector3(-89, 163, 0);
                                                break;
                                            case 4: vector = new Vector3(698, 471, 0);
                                                break;
                                            case 5: vector = new Vector3(772, 368, 0);
                                                break;
                                            case 6: vector = new Vector3(772, 262, 0);
                                                break;
                                            case 7: vector = new Vector3(693, 165, 0);
                                                break;
                                            default: vector = Vector3.zero;
                                                break;
                                        }
                                        m_poorGold[i - 1].goldTween[goldNum].to = vector;
                                        m_poorGold[i - 1].goldTween[goldNum].duration = 0.5f;
                                        m_poorGold[i - 1].goldTween[goldNum].enabled = true;
                                        m_poorGold[i - 1].goldTween[goldNum].ResetToBeginning();
                                        m_poorGold[i - 1].goldTween[goldNum].PlayForward();
                                        flag = 0;
                                        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);
                                        flaga = 0;
                                    }
                                }
                            }
                        }
                        if (flaga == 0)
                            continue;
                    }

                    if (result[i] > result[0] && myWeager[i - 1] > 0)
                    {
                        if (goldNum < m_poorGold[i - 1].weagerGold.Count / 3)
                        {
                            m_poorGold[i - 1].goldTween[goldNum].from = m_poorGold[i - 1].weagerGold[goldNum].transform.localPosition;
                            m_poorGold[i - 1].goldTween[goldNum].to = new Vector3(0, 0, 0);
                            m_poorGold[i - 1].goldTween[goldNum].duration = 0.5f;
                            if (m_poorGold[i - 1].weagerGold.Count > 30)
                            {
                                m_poorGold[i - 1].goldTween[goldNum].duration = 0.3f;
                                returnSystemCount = 0.01f;
                            }
                            m_poorGold[i - 1].goldTween[goldNum].enabled = true;
                            m_poorGold[i - 1].goldTween[goldNum].ResetToBeginning();
                            m_poorGold[i - 1].goldTween[goldNum].PlayForward();
                            flag = 0;
                            GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);
                        }
                        else if (goldNum < m_poorGold[i - 1].weagerGold.Count)
                        {
                            m_poorGold[i - 1].goldTween[goldNum].from = m_poorGold[i - 1].weagerGold[goldNum].transform.localPosition;
                            m_poorGold[i - 1].goldTween[goldNum].to = new Vector3(-165, 13, 0);
                            m_poorGold[i - 1].goldTween[goldNum].duration = 0.5f;
                            if (m_poorGold[i - 1].weagerGold.Count > 30)
                            {
                                m_poorGold[i - 1].goldTween[goldNum].duration = 0.3f;
                                returnSystemCount = 0.01f;
                            }
                            m_poorGold[i - 1].goldTween[goldNum].enabled = true;
                            m_poorGold[i - 1].goldTween[goldNum].ResetToBeginning();
                            m_poorGold[i - 1].goldTween[goldNum].PlayForward();
                            flag = 0;
                            GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);
                        }
                    }
                    else if (result[i] > result[0] && myWeager[i - 1] == 0)
                    {
                        if (goldNum < m_poorGold[i - 1].weagerGold.Count)
                        {
                            m_poorGold[i - 1].goldTween[goldNum].from = m_poorGold[i - 1].weagerGold[goldNum].transform.localPosition;
                            m_poorGold[i - 1].goldTween[goldNum].to = new Vector3(-165, 13, 0);
                            m_poorGold[i - 1].goldTween[goldNum].duration = 0.5f;
                            if (m_poorGold[i - 1].weagerGold.Count > 30)
                            {
                                m_poorGold[i - 1].goldTween[goldNum].duration = 0.3f;
                                returnSystemCount = 0.01f;
                            }
                            m_poorGold[i - 1].goldTween[goldNum].enabled = true;
                            m_poorGold[i - 1].goldTween[goldNum].ResetToBeginning();
                            m_poorGold[i - 1].goldTween[goldNum].PlayForward();
                            flag = 0;
                            GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);
                        }
                    }

                }
                returnPlayerCount = 0.01f;
                goldNum++;
                if (flag == 1)
                {
                    SetMyGoldNum();
                    if (hostInfo != null)
                        UpdateHost(hostInfo);
                    isReturnToPlayer = false;
                }
            }
        }

        if (isNewGold)
        {
            newGoldCount -= deltaTime;
            if (newGoldCount <= 0)
            {
                InitOneGold(new Vector3(0, 0, 0), poorIndex);
                newGoldCount = 0.03f;
                goldNum++;
                if (goldNum >= goldArr[chooseLevel])
                    isNewGold = false;
            }
        }

        if (isNormalWeager && !stopNormalWeager)
        {
            normalWeagerCount -= deltaTime;
            if (normalWeagerCount <= 0)
            {
                for (byte i = 0; i < 4; i++)
                {
                    if (isNormal[i])
                    {
                        InitOneGold(new Vector3(-176, 57, 0), i);
                        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);
                    }
                }
                newGoldCount = 0.03f;
                normalGoldCount++;
                if (normalGoldCount >= 10)
                    isNormalWeager = false;
            }
        }

        if (isVipWeager)
        {
            vipWeagerCount -= deltaTime;
            if (vipWeagerCount <= 0)
            {
                Vector3 vector;
                switch (seatIndex)
                {
                    case 0: vector = new Vector3(-97, 465, 0);
                        break;
                    case 1: vector = new Vector3(-176, 368, 0);
                        break;
                    case 2: vector = new Vector3(-168, 29, 0);
                        break;
                    case 3: vector = new Vector3(-89, 163, 0);
                        break;
                    case 4: vector = new Vector3(698, 471, 0);
                        break;
                    case 5: vector = new Vector3(772, 368, 0);
                        break;
                    case 6: vector = new Vector3(772, 262, 0);
                        break;
                    case 7: vector = new Vector3(693, 165, 0);
                        break;
                    default: vector = Vector3.zero;
                        break;
                }
                InitOneGold(vector, vipWeagerIndex);
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);
                vipWeagerCount = 0.03f;
                vipGoldCount++;
                if (vipGoldCount >= 10)
                    isVipWeager = false;
            }
        }

        if (isPress)
        {
            pressCount += deltaTime;
            if (pressCount > 0.1f)
            {
                pressCounta += deltaTime;
                if (pressCounta > 0.3f)
                {
                    EventWagerChoose(m_PoolBtn[poorIndex]);
                    pressCounta = 0f;
                }
            }
        }

        //1.先删除前面的。
        //2.暂停数量20以上
        if (Utility.GetTickCount() - m_RemoveTick > 4000)
        {
            m_RemoveTick = Utility.GetTickCount();
            const int REMOVE_NUM = 30;
            const int MAX_REMOVE_COUNT = 100;
            for (byte j = 0; j < 4; j++)
            {
                int count = m_poorGold[j].weagerGold.Count - 1;
                int deactiveCount = 0;
                int removeCount = 0;
                for (int i = count; i >= 0; --i)
                {
                    if (!m_poorGold[j].goldTween[i].isActiveAndEnabled)
                    {
                        if (++deactiveCount > REMOVE_NUM)
                        {
                            GameObject.Destroy(m_poorGold[j].weagerGold[i]);
                            m_poorGold[j].weagerGold.RemoveAt(i);
                            m_poorGold[j].goldTween.RemoveAt(i);
                            if (++removeCount > MAX_REMOVE_COUNT)
                                break;
                        }
                    }
                }
            }
        }
    }

    void GoldFlyToDesk()
    {
        isReturnToDesk = true;
        returnDestCount = 1.2f;
        goldNum = 0;
    }
    void GoldFlyToSystem()
    {
        m_BaseTrans.GetChild(6).gameObject.SetActive(true);
        isReturnToSystem = true;
        returnSystemCount = 0f;
        goldNum = 0;
    }

    void GoldFlyToPlayer()
    {
        m_BaseTrans.GetChild(6).gameObject.SetActive(true);
        isReturnToPlayer = true;
        returnPlayerCount = 1.2f;
        goldNum = 0;
    }

    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            WndManager.Instance.Pop(m_CalculateWnd);
            WndManager.Instance.Pop(m_DescripWnd);
            WndManager.Instance.Pop(m_HostLineWnd);
            WndManager.Instance.Pop(m_TrendWnd);
            GameObject.Destroy(m_BaseWndObject);
            GameObject.Destroy(m_HostEffectWnd);
            GameObject.Destroy(m_NormalWnd);
            m_BaseWndObject = null;
            GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound2);
            GlobalHallUIMgr.Instance.GameShare.LveaveGame();
        }
    }

    public void SetPlayerHeader(Texture2D tex)
    {
        m_HeadTex = tex;
        m_HeadTrans.GetChild(6).GetChild(0).GetComponent<UITexture>().mainTexture = m_HeadTex;
    }

    public void SetHostHeader(Texture2D tex)
    {
        hostHeadTex = tex;
        hostHeadObj.transform.GetChild(0).GetComponent<UITexture>().mainTexture = hostHeadTex;
    }

    public void GetTableState(NiuNiuGameLog log, VipSeatList list, BankerUserInfo TableBankerInfo, WriteBankerList TableWriteBankerList,
        NiuNiuTableStates state, byte time, byte[] card, ushort[] result, ulong[] globalNum)   //响应进入桌子的事件
    {
        UpdateHost(TableBankerInfo);
        GetHostLine(TableWriteBankerList);
        UpdateVipInfo(list);
        UpdateTrend(log);

        if (state == NiuNiuTableStates.NNTS_Begin)
        {
            weagerCountDown = time;
            isInWeager = true;
            m_PoolTrans.GetChild(4).gameObject.SetActive(true);
            for (byte i = 0; i < totalNum.Length; i++)
            {
                totalNum[i] = 0;
            }
            UpdateTotalMoney(globalNum, 1);
        }

        if (state == NiuNiuTableStates.NNTS_End)
        {
            for (byte i = 0; i < card.Length; i++)
            {
                cardResult[i] = card[i];
            }
            for (byte i = 0; i < result.Length; i++)
            {
                this.result[i] = result[i];
            }
            for (byte i = 0; i < totalNum.Length; i++)
            {
                totalNum[i] = 0;
            }
            UpdateTotalMoney(globalNum, 2);
            m_PoolTrans.GetChild(4).gameObject.SetActive(false);
            nextCount = time;
            isNext = true;
            isShow = false;
            m_PoolTrans.GetChild(6).gameObject.SetActive(true);
            isInWeager = false;
            ShowOneResult(m_TitleTrans, 0);
            for (byte i = 1; i < 5; i++)
            {
                ShowOneResult(m_PoolBtn[i - 1].transform, i);
            }
        }
    }

    public void GetResult(BankerUserInfo tableBankInfo, VipSeatList oldVipInfo, VipSeatList newVipInfo, NiuNiuGameLog log, int state, TableBrandResult result)   //响应状态改变事件
    {
        if (state == 1)
        {
            UpdateHost(tableBankInfo);
            SetMyGoldNum();
            gameState = 1;
            if (m_CalculateWnd.activeSelf)
            {
                EventCloseResult(null);
            }
            m_BaseTrans.GetChild(6).gameObject.SetActive(true);
            m_PoolTrans.GetChild(6).gameObject.SetActive(false);
            m_PoolTrans.GetChild(5).gameObject.SetActive(false);
            m_PoolTrans.GetChild(4).gameObject.SetActive(true);
            weagerCountDown = FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.BeginWriteSec;
            isInWeager = true;
            ShowNiuniuBegin();
        }
        if (state == 2)
        {
            hostInfo = tableBankInfo;
            for (byte i = 0; i < oldVipInfo.List.Length; i++)
            {
                if (oldVipInfo.List[i] == null)
                    vipInfo[i].userId = 0;
                else
                {
                    vipInfo[i].userId = oldVipInfo.List[i].dwUserID;
                    vipInfo[i].newGoldNum = newVipInfo.List[i].GlobelSum;
                    vipInfo[i].oldGoldnum = oldVipInfo.List[i].GlobelSum;
                    uint global = oldVipInfo.List[i].GlobelSum;
                    vipSeat[i].moneyLabel.text = global.ToString();
                    if (global > 10000)
                        vipSeat[i].moneyLabel.text = (global / 10000) + "." + (global / 1000 % 10) + "万";
                }
            }

            gameState = 2;
            hostGameSum++;
            if (hostGameSum == FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.BankerGameSum
                && hostName == PlayerRole.Instance.RoleInfo.RoleMe.GetNickName())
            {
                isHost = false;
            }
            if (hostName == PlayerRole.Instance.RoleInfo.RoleMe.GetNickName())
                hostGameRestLabel.text = (FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.BankerGameSum - hostGameSum).ToString();
            isNewGold = false;
            isInWeager = false;
            animCountDown = 1.5f;
            isAnim = true;
            restCount = FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.EndWriteSec;
            nextCount = restCount;
            isRest = true;
            isNext = true;
            m_PoolTrans.GetChild(4).gameObject.SetActive(false);
            for (byte i = 0; i < result.BrandArray.Length; i++)
            {
                cardResult[i] = result.BrandArray[i];
            }
            for (byte i = 0; i < result.BrandResult.Length; i++)
            {
                this.result[i] = result.BrandResult[i];
            }
            ShowNiuniuEnd();
            UpdateTrend(log);
        }
    }

    void UpdateTotalWeagerGold(int index, uint length, int state)
    {
        if (state == 1)
        {
            normalWeagerCount = 0f;
            isNormalWeager = true;
            normalGoldCount = 0;
            normalIndex = index;
        }
        else
        {
            if (poorGold[index] < 50)
            {
                poorGold[index] += goldArr[length];
                for (byte j = 0; j < goldArr[length]; j++)
                {
                    GameObject goldItem = GameObject.Instantiate(m_BaseTrans.GetChild(5).gameObject);
                    goldItem.transform.SetParent(m_BaseTrans.GetChild(6), false);
                    goldItem.SetActive(true);
                    m_poorGold[index].weagerGold.Add(goldItem);
                    m_poorGold[index].goldTween.Add(goldItem.GetComponent<TweenPosition>());
                    Vector3 endPos;

                    if (index == 0)
                        endPos = new Vector3(Random.Range(7, 107), Random.Range(320, 388), 0);
                    else if (index == 1)
                        endPos = new Vector3(Random.Range(167, 267), Random.Range(320, 388), 0);
                    else if (index == 2)
                        endPos = new Vector3(Random.Range(337, 437), Random.Range(320, 388), 0);
                    else
                        endPos = new Vector3(Random.Range(497, 597), Random.Range(320, 388), 0);
                    goldItem.transform.localPosition = endPos;
                }
            }
        }
    }

    public void UpdateTotalMoney(ulong[] date, int state = 1)          //更新总下注
    {
        for (byte i = 0; i < totalNum.Length; i++)
        {
            if (totalNum[i] < date[i])
            {
                uint num;
                uint length = (uint)date[i] - (uint)totalNum[i];
                if (length < 1001)
                    num = 0;
                else if (length < 5001)
                    num = 1;
                else if (length < 10001)
                    num = 2;
                else if (length < 100001)
                    num = 3;
                else
                    num = 4;
                isNormal[i] = true;
                UpdateTotalWeagerGold(i, num, state);
            }
            else
                isNormal[i] = false;
            totalNum[i] = date[i];

            if (totalNum[i] > 10000)
            {
                m_PoolBtn[i].transform.GetChild(2).GetComponent<UILabel>().text = ((float)totalNum[i] / 10000).ToString();
                m_PoolBtn[i].transform.GetChild(2).localPosition = new Vector3(-9, 65, 0);
                m_PoolBtn[i].transform.GetChild(4).gameObject.SetActive(true);
            }
            else
            {
                m_PoolBtn[i].transform.GetChild(2).GetComponent<UILabel>().text = totalNum[i].ToString();
                m_PoolBtn[i].transform.GetChild(2).localPosition = new Vector3(3, 65, 0);
                m_PoolBtn[i].transform.GetChild(4).gameObject.SetActive(false);
            }
        }
    }

    void InitOneGold(Vector3 beginPos, int index)
    {
        GameObject goldItem = GameObject.Instantiate(m_BaseTrans.GetChild(5).gameObject);
        goldItem.transform.SetParent(m_BaseTrans.GetChild(6), false);
        goldItem.SetActive(true);
        m_poorGold[index].weagerGold.Add(goldItem);
        TweenPosition tween = goldItem.GetComponent<TweenPosition>();
        m_poorGold[index].goldTween.Add(tween);
        Vector3 endPos;

        if (index == 0)
            endPos = new Vector3(Random.Range(7, 107), Random.Range(320, 388), 0);
        else if (index == 1)
            endPos = new Vector3(Random.Range(167, 267), Random.Range(320, 388), 0);
        else if (index == 2)
            endPos = new Vector3(Random.Range(337, 437), Random.Range(320, 388), 0);
        else
            endPos = new Vector3(Random.Range(497, 597), Random.Range(320, 388), 0);
        tween.from = beginPos;
        tween.to = endPos;
        tween.enabled = true;
    }

    void UpdateMyWeagerGold(int poorIndex)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu15);
        poorGold[poorIndex] += goldArr[chooseLevel];
        newGoldCount = 0f;
        isNewGold = true;
        goldNum = 0;
    }


    public void UpdateMyWeagerNum()                     //更新我的下注
    {
        myWeager[poorIndex] += levelArr[chooseLevel];
        if (myWeager[poorIndex] > 10000)
            m_PoolTrans.GetChild(poorIndex).GetChild(3).GetChild(0).GetComponent<UILabel>().text
                = ((float)myWeager[poorIndex] / 10000).ToString() + " 万";
        else
            m_PoolTrans.GetChild(poorIndex).GetChild(3).GetChild(0).GetComponent<UILabel>().text = myWeager[poorIndex].ToString();

        totalNum[poorIndex] += (uint)levelArr[chooseLevel];

        if (totalNum[poorIndex] > 10000)
        {
            m_PoolBtn[poorIndex].transform.GetChild(2).GetComponent<UILabel>().text = ((float)totalNum[poorIndex] / 10000).ToString();
            m_PoolBtn[poorIndex].transform.GetChild(2).localPosition = new Vector3(-9, 65, 0);
            m_PoolBtn[poorIndex].transform.GetChild(4).gameObject.SetActive(true);
        }
        else
        {
            m_PoolBtn[poorIndex].transform.GetChild(2).GetComponent<UILabel>().text = totalNum[poorIndex].ToString();
            m_PoolBtn[poorIndex].transform.GetChild(2).localPosition = new Vector3(3, 65, 0);
            m_PoolBtn[poorIndex].transform.GetChild(4).gameObject.SetActive(false);
        }

        m_PoolTrans.GetChild(poorIndex).GetChild(3).gameObject.SetActive(true);

        UpdateMyWeagerGold(poorIndex);

        SetMyGoldNum();
    }

    void ShowNiuniuBegin()
    {
        m_TitleTrans.GetChild(0).gameObject.SetActive(false);
        m_TitleTrans.GetChild(1).gameObject.SetActive(false);
        for (byte i = 0; i < 4; i++)
        {
            m_PoolBtn[i].transform.GetChild(0).gameObject.SetActive(false);
            m_PoolBtn[i].transform.GetChild(1).gameObject.SetActive(false);
            m_PoolBtn[i].transform.GetChild(3).gameObject.SetActive(false);
            m_PoolBtn[i].transform.GetChild(2).GetComponent<UILabel>().text = "0";
            myWeager[i] = 0;
            resultNum[i] = 0;
            hostResultNum[i] = 0;
            poorGold[i] = 0;
        }
        SetCardBegin();
    }

    void ShowNiuniuEnd()
    {
        m_PoolTrans.GetChild(4).gameObject.SetActive(false);
        m_TitleTrans.GetChild(0).gameObject.SetActive(true);
        for (byte i = 0; i < 4; i++)
        {
            m_PoolBtn[i].SetActive(true);
            m_PoolBtn[i].transform.GetChild(0).gameObject.SetActive(true);
        }
        m_BaseTrans.GetComponent<Animator>().SetBool("isAnim", true);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu16);
    }

    void ShowOneResult(Transform trans, int index)
    {
        trans.GetChild(0).gameObject.SetActive(true);
        for (byte i = 0; i < 5; i++)
        {
            trans.GetChild(0).GetChild(i).gameObject.SetActive(false);
            trans.GetChild(0).GetChild(i + 5).gameObject.SetActive(true);
            trans.GetChild(0).GetChild(i + 5).GetComponent<UISprite>().spriteName = "Card_" + cardResult[index * 5 + i];
        }
        resultLevel[index] = result[index] >> 8;
        int niuNiuLevel = resultLevel[index];
        resultRate[index] = FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.NiuNiuLevelRate[(byte)niuNiuLevel];
        trans.GetChild(1).gameObject.SetActive(true);
        Transform NiuBg = trans.GetChild(1).GetChild(0);
        if (niuNiuLevel == 1)           //无牛
        {
            trans.GetChild(1).GetChild(0).gameObject.SetActive(false);
            trans.GetChild(1).GetChild(1).gameObject.SetActive(true);

            if (isShow)
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu20);

        }
        else if (niuNiuLevel < 11)      //牛N
        {
            trans.GetChild(0).GetChild(5).localPosition -= new Vector3(5, 0, 0);
            trans.GetChild(0).GetChild(6).localPosition -= new Vector3(5, 0, 0);
            trans.GetChild(0).GetChild(7).localPosition -= new Vector3(5, 0, 0);
            trans.GetChild(0).GetChild(8).localPosition += new Vector3(5, 0, 0);
            trans.GetChild(0).GetChild(9).localPosition += new Vector3(5, 0, 0);

            trans.GetChild(1).GetChild(0).GetChild(2).GetComponent<UILabel>().text = (niuNiuLevel - 1).ToString();
            NiuBg.GetChild(0).gameObject.SetActive(true);
            NiuBg.GetChild(1).gameObject.SetActive(false);
            NiuBg.GetChild(2).gameObject.SetActive(true);
            NiuBg.GetChild(3).gameObject.SetActive(false);
            NiuBg.GetChild(4).gameObject.SetActive(false);
            NiuBg.GetChild(5).gameObject.SetActive(false);

            trans.GetChild(1).GetChild(0).gameObject.SetActive(true);
            trans.GetChild(1).GetChild(1).gameObject.SetActive(false);
            if (isShow)
            {
                if (niuNiuLevel == 2)
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu1);
                else if (niuNiuLevel == 3)
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu2);
                else if (niuNiuLevel == 4)
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu3);
                else if (niuNiuLevel == 5)
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu4);
                else if (niuNiuLevel == 6)
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu5);
                else if (niuNiuLevel == 7)
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu6);
                else if (niuNiuLevel == 8)
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu7);
                else if (niuNiuLevel == 9)
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu8);
                else if (niuNiuLevel == 10)
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu9);
            }
        }
        else if (niuNiuLevel == 11)     //牛牛
        {
            trans.GetChild(0).GetChild(5).localPosition -= new Vector3(5, 0, 0);
            trans.GetChild(0).GetChild(6).localPosition -= new Vector3(5, 0, 0);
            trans.GetChild(0).GetChild(7).localPosition -= new Vector3(5, 0, 0);
            trans.GetChild(0).GetChild(8).localPosition += new Vector3(5, 0, 0);
            trans.GetChild(0).GetChild(9).localPosition += new Vector3(5, 0, 0);

            NiuBg.GetChild(0).gameObject.SetActive(true);
            NiuBg.GetChild(1).gameObject.SetActive(true);
            NiuBg.GetChild(2).gameObject.SetActive(false);
            NiuBg.GetChild(3).gameObject.SetActive(false);
            NiuBg.GetChild(4).gameObject.SetActive(false);
            NiuBg.GetChild(5).gameObject.SetActive(false);

            trans.GetChild(1).GetChild(0).gameObject.SetActive(true);
            trans.GetChild(1).GetChild(1).gameObject.SetActive(false);

            if (isShow)
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu10);
        }
        else if (niuNiuLevel == 12)     //五花牛
        {
            NiuBg.GetChild(0).gameObject.SetActive(false);
            NiuBg.GetChild(1).gameObject.SetActive(true);
            NiuBg.GetChild(2).gameObject.SetActive(false);
            NiuBg.GetChild(3).gameObject.SetActive(false);
            NiuBg.GetChild(4).gameObject.SetActive(false);
            NiuBg.GetChild(5).gameObject.SetActive(true);

            trans.GetChild(1).GetChild(0).gameObject.SetActive(true);
            trans.GetChild(1).GetChild(1).gameObject.SetActive(false);
            if (isShow)
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu11);
        }
        else if (niuNiuLevel == 13)     //炸弹
        {
            NiuBg.GetChild(0).gameObject.SetActive(false);
            NiuBg.GetChild(1).gameObject.SetActive(false);
            NiuBg.GetChild(2).gameObject.SetActive(false);
            NiuBg.GetChild(3).gameObject.SetActive(false);
            NiuBg.GetChild(4).gameObject.SetActive(true);
            NiuBg.GetChild(5).gameObject.SetActive(false);

            trans.GetChild(1).GetChild(0).gameObject.SetActive(true);
            trans.GetChild(1).GetChild(1).gameObject.SetActive(false);
            if (isShow)
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu12);
        }
        else if (niuNiuLevel == 14)     //五小牛
        {
            NiuBg.GetChild(0).gameObject.SetActive(false);
            NiuBg.GetChild(1).gameObject.SetActive(true);
            NiuBg.GetChild(2).gameObject.SetActive(false);
            NiuBg.GetChild(3).gameObject.SetActive(true);
            NiuBg.GetChild(4).gameObject.SetActive(false);
            NiuBg.GetChild(5).gameObject.SetActive(false);

            trans.GetChild(1).GetChild(0).gameObject.SetActive(true);
            trans.GetChild(1).GetChild(1).gameObject.SetActive(false);
            if (isShow)
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu13);
        }

        if (index != 0)
        {
            if (myWeager[index - 1] == 0)
            {
                trans.GetChild(1).GetChild(2).gameObject.SetActive(true);
                trans.GetChild(1).GetChild(3).gameObject.SetActive(false);
                trans.GetChild(1).GetChild(4).gameObject.SetActive(false);
            }
            else
            {
                if (result[index] > result[0])
                {
                    resultNum[index - 1] = resultRate[index] * myWeager[index - 1];
                    trans.GetChild(1).GetChild(4).GetComponent<UILabel>().text = "+" + resultNum[index - 1];
                    trans.GetChild(1).GetChild(3).GetComponent<UILabel>().text = "*" + resultRate[index];
                }
                else
                {
                    resultNum[index - 1] = -resultRate[0] * myWeager[index - 1];
                    trans.GetChild(1).GetChild(4).GetComponent<UILabel>().text = resultNum[index - 1].ToString();
                    trans.GetChild(1).GetChild(3).GetComponent<UILabel>().text = "*" + resultRate[0];
                }
                trans.GetChild(1).GetChild(2).gameObject.SetActive(false);
                trans.GetChild(1).GetChild(3).gameObject.SetActive(true);
                trans.GetChild(1).GetChild(4).gameObject.SetActive(true);
            }
            if (isHost)
            {
                if (result[index] > result[0])
                    hostResultNum[index - 1] = resultRate[index] * (long)totalNum[index - 1] * -1;
                else
                    hostResultNum[index - 1] = resultRate[0] * (long)totalNum[index - 1];
                trans.GetChild(1).GetChild(2).gameObject.SetActive(false);
                trans.GetChild(1).GetChild(3).gameObject.SetActive(false);
                trans.GetChild(1).GetChild(4).gameObject.SetActive(false);
            }
        }
    }

    void SetCardBegin()
    {
        for (byte i = 0; i < 5; i++)
        {
            m_TitleTrans.transform.GetChild(0).GetChild(i + 5).gameObject.SetActive(false);
            m_TitleTrans.transform.GetChild(0).GetChild(i).gameObject.SetActive(true);
            m_TitleTrans.transform.GetChild(0).GetChild(i + 5).GetComponent<UISprite>().spriteName = "CardBack";
            m_TitleTrans.transform.GetChild(0).GetChild(i + 5).localPosition =
                m_TitleTrans.transform.GetChild(0).GetChild(i).localPosition;
        }
        for (byte i = 0; i < 4; i++)
        {
            for (byte j = 0; j < 5; j++)
            {
                m_PoolBtn[i].transform.GetChild(0).GetChild(j + 5).gameObject.SetActive(false);
                m_PoolBtn[i].transform.GetChild(0).GetChild(j).gameObject.SetActive(true);
                m_PoolBtn[i].transform.GetChild(0).GetChild(j + 5).GetComponent<UISprite>().spriteName = "CardBack";
                m_PoolBtn[i].transform.GetChild(0).GetChild(j + 5).localPosition =
                    m_PoolBtn[i].transform.GetChild(0).GetChild(j).localPosition;
            }
        }

        for (byte i = 0; i < m_poorGold.Length; i++)
        {
            for (byte j = 0; j < m_poorGold[i].weagerGold.Count; j++)
            {
                GameObject.Destroy(m_poorGold[i].weagerGold[j]);
            }
            m_poorGold[i].weagerGold.Clear();
            m_poorGold[i].goldTween.Clear();
        }
        m_BaseTrans.GetChild(6).gameObject.SetActive(true);
        isPress = false;
        stopNormalWeager = false;
        for (byte i = 0; i < 8; i++)
        {
            for (byte j = 0; j < 4; j++)
                vipInfo[i].weagerIndex[j] = false;
        }
    }

    public void SetWeagerBtn()
    {
        if (!isAnim)
        {

            byte maxRate = FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.MaxRate;
            long maxAddGlobel = FishConfig.Instance.m_MiNiGameConfig.niuniuConfig.MaxAddGlobel;

            int myTotalWeager = myWeager[0] + myWeager[1] + myWeager[2] + myWeager[3];
            uint myGold = PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel();
            if (isMasterHost)
            {
                for (int i = levelArr.Length - 1; i >= 0; i--)
                {
                    if (myTotalWeager + levelArr[i] > maxAddGlobel || (myTotalWeager + myGold) < (levelArr[i] + myTotalWeager) * maxRate
                        || levelArr[i] > myGold)
                    {
                        if (i == 0)
                        {
                            m_HeadTrans.GetChild(5).gameObject.SetActive(false);
                        }
                        m_LevelUIBtn[i].isEnabled = false;
                        if (i == level)
                        {
                            if (i > 0)
                                EventChangeLevel(m_LevelBtn[i - 1]);
                        }
                    }
                    else
                    {
                        m_HeadTrans.GetChild(5).gameObject.SetActive(true);
                        m_LevelUIBtn[i].isEnabled = true;
                    }
                }
            }
            else 
            {
                uint weagerSum = (uint)(totalNum[0] + totalNum[1] + totalNum[2] + totalNum[3]);
                for (int i = levelArr.Length - 1; i >= 0; i--)
                {
                    if ((weagerSum + levelArr[i]) * maxRate > hostGoldSum || myTotalWeager + levelArr[i] > maxAddGlobel || (myTotalWeager + myGold) < (levelArr[i] + myTotalWeager) * maxRate
                    || levelArr[i] > myGold)
                    {
                        if (i == 0)
                        {
                            m_HeadTrans.GetChild(5).gameObject.SetActive(false);
                        }
                        m_LevelUIBtn[i].isEnabled = false;
                        if (i == level)
                        {
                            if (i > 0)
                                EventChangeLevel(m_LevelBtn[i - 1]);
                        }
                    }
                    else
                    {
                        m_HeadTrans.GetChild(5).gameObject.SetActive(true);
                        m_LevelUIBtn[i].isEnabled = true;
                    }
                }
            }
        }
    }
}

class PoorGold
{
    public List<GameObject> weagerGold = new List<GameObject>();    //金币动画列表
    public List<TweenPosition> goldTween = new List<TweenPosition>();
}

class HostItem : BaseWnd
{
    uint lineNum;
    string name;
    uint goldNum;

    UILabel lineNumLabel;
    UILabel nameLabel;
    UILabel goldNumLabel;

    public HostItem(uint lineNum, string name, uint goldNum)
    {
        this.lineNum = lineNum;
        this.name = name;
        this.goldNum = goldNum;
    }

    public Transform InitItem(Transform trans)
    {
        m_BaseWndObject = GameObject.Instantiate(trans.gameObject);
        m_BaseTrans = m_BaseWndObject.transform;
        if (!m_BaseWndObject.activeSelf)
            m_BaseWndObject.SetActive(true);
        lineNumLabel = m_BaseTrans.GetChild(0).GetComponent<UILabel>();
        nameLabel = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        goldNumLabel = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        lineNumLabel.text = lineNum.ToString();
        nameLabel.text = name;
        goldNumLabel.text = (goldNum / 10000) + "." + (goldNum / 1000 % 10) + "万";
        return m_BaseTrans;
    }
}

class NiuniuVipSeat
{
    public byte index;
    public GameObject sitDownBtn;
    public GameObject headObj;
    public UILabel moneyLabel;
    public GameObject getSeatBtn;
    public GameObject postCardBtn;
    public Texture2D headTex;
    bool isShowBtn = true;
    public uint userId;

    public void Init(Transform obj, byte i)
    {
        sitDownBtn = obj.GetChild(0).gameObject;
        headObj = obj.GetChild(1).gameObject;
        moneyLabel = obj.GetChild(2).GetComponent<UILabel>();
        getSeatBtn = obj.GetChild(3).gameObject;
        postCardBtn = obj.GetChild(4).gameObject;
        index = i;
        UIEventListener.Get(sitDownBtn).onClick = EventVipSitdown;
        UIEventListener.Get(headObj.transform.GetChild(0).gameObject).onClick = EventClickHead;
        UIEventListener.Get(postCardBtn).onClick = EventShowPostCard;
        UIEventListener.Get(getSeatBtn).onClick = EventVipSitdown;
    }

    void EventVipSitdown(GameObject go)
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_JoinVip_Fail"), MssageType.NiuNiu_UpHostFail_Tips);
        }
        PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnRoleJoinVipSeat(index);
    }

    void EventClickHead(GameObject go)
    {
        getSeatBtn.SetActive(isShowBtn);
        postCardBtn.SetActive(isShowBtn);
        isShowBtn = !isShowBtn;
    }

    void EventShowPostCard(GameObject go)
    {
        if (userId == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Other_Head,
                PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), headTex, true);
        }
        else
        {
            GlobalHallUIMgr.Instance.ShowNameCardsUI(NameCardsUIType.NameCards_UI, CardsParentUIType.Other_Head, userId,
                headTex, false, false);
            PlayerRole.Instance.QueryManager.QueryUserByUserID(userId);
        }
    }
}

class NormalItem : BaseWnd
{
    public GameObject headObj;

    public Texture2D headTex;
    public UILabel nameLabel;
    public UILabel goldLabel;

    public uint id;
    string name;
    uint gold;

    public NormalItem(string name, uint gold)
    {
        this.name = name;
        this.gold = gold;
    }

    public Transform Init(Transform trans)
    {
        m_BaseWndObject = GameObject.Instantiate(trans.gameObject);
        m_BaseTrans = m_BaseWndObject.transform;
        if (!m_BaseWndObject.activeSelf)
            m_BaseWndObject.SetActive(true);
        headObj = m_BaseTrans.GetChild(0).gameObject;
        nameLabel = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        goldLabel = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        nameLabel.text = name;
        goldLabel.text = gold.ToString();
        if (gold > 10000)
            goldLabel.text = (gold / 10000) + "." + (gold / 1000 % 10) + "万";
        return m_BaseTrans;
    }
}

class ForestDanceVipInfo
{
    public uint userId = 0;
    public uint oldGoldnum;
    public uint newGoldNum;
    public bool[] weagerIndex = new bool[4];
}