using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GameForestDanceUI : BaseWnd
{
    Object m_Obj;
    Object m_EffectStep;
    Object m_EffectHostObj;
    Object m_ResultEffectObj;
    GameObject m_EffectObj;
    GameObject m_HostEffectWnd;
    GameObject m_ResultEffectWnd;
    GameForestDanceWnd m_ForestWnd = new GameForestDanceWnd();

    GameObject m_HeadWnd;
    Transform m_HeadTrans;
    UILabel m_GoldLabel;
    Texture2D m_HeadTex;

    GameObject m_WeagerWnd;
    Transform m_WeagerTrans;
    UILabel[] totalWeagerLabel;
    UILabel[] weagerRateLabel;
    GameObject[] weagerChooseObj;
    WeagerWnd[] weagerWnd;
    int weagerIndex;

    bool isWeager = false;
    float weagerCountDown;
    bool isWait = false;
    float waitCountDown;
    bool isAnim = false;
    float animCountDown;

    GameObject[] m_CountDownWnd;
    Transform[] m_CountDownTrans;
    UILabel[] m_CountDownLabel;

    GameObject m_ResultWnd;
    Transform m_ResultTrans;
    TweenPosition m_ResultWndTween;
    TweenPosition m_ResultTitleTween;
    TweenPosition m_ResultRewardKindTween;
    TweenPosition m_ResultBGKindTween;
    WeagerResult[] weagerResult;
    UISprite resultKindSprite;
    UISprite resultColorSprite;
    UILabel resultRateLabel;
    UILabel myResultLabel;
    UILabel hostResultLabel;
    bool isShowMyWeager = false;
    float showMyWeagerCountDown;
    bool isAnimalMove = false;
    float animalMoveCountDown;
    bool isAnimalMoveBack = false;
    float animalMoveBackCountDown;
    bool isEffect = false;

    GameObject m_TrendWnd;
    Transform m_TrendTrans;
    UISprite[] trendColorSprite;
    UISprite[] trendKindSprite;
    TweenScale trendTween;
    bool isTrendOpen=false;
    float trendOpenCountDown;

    GameObject m_VipSeatWnd;
    Transform m_VipSeatTrans;
    ForestDanceVipSeat[] vipSeat;
    ForestDanceVipInfo[] vipInfo = new ForestDanceVipInfo[8];

    GameObject m_TitleWnd;
    Transform m_TitleTrans;
    GameObject[] hostBtnObj;
    UILabel hostNameLabel;
    UILabel hostGoldNumLabel;
    GameObject hostHeadObj;
    Texture2D hostHeadTex;

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
    DialBankerUserInfo hostInfo;

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

    GameObject m_DesWnd;
    Transform m_DesTrans;

    //不提早显示结果
    DialGameLog m_pGameLog=null;
    DialBankerUserInfo m_pHostInfo=null;

    public uint[] weagerArr;
    int animalIndex;
    int resultIndex;
    bool isPush = false;
    public void InitObj()
    {
        if (m_Obj != null)
            return;
        m_Obj = ResManager.Instance.LoadObject("ForestDancePartyWnd", "GameRes/ForestDance/Prefabs/", ResType.GameRes);
        m_EffectStep = ResManager.Instance.LoadObject("Ef_step", "GameRes/ForestDance/", ResType.GameRes);
        m_EffectHostObj = ResManager.Instance.LoadObject("Niuniu_Host", "GameRes/Niuniu/Prefab/Effect/", ResType.GameRes);
        m_ResultEffectObj = ResManager.Instance.LoadObject("Ef_ForestChoiceUI", "GameRes/ForestDance/", ResType.GameRes);
        m_ForestWnd.InitObj();
        
    }

    void Init()
    {
        m_ForestWnd.Init();

        m_BaseWndObject = (GameObject)GameObject.Instantiate(m_Obj);
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_HeadTrans = m_BaseTrans.GetChild(0);
        m_HeadWnd = m_HeadTrans.gameObject;
        m_WeagerTrans = m_BaseTrans.GetChild(4);
        m_WeagerWnd = m_WeagerTrans.gameObject;
        m_ResultTrans = m_BaseTrans.GetChild(5);
        m_ResultWnd = m_ResultTrans.gameObject;
        m_TrendTrans = m_BaseTrans.GetChild(3);
        m_TrendWnd = m_TrendTrans.gameObject;
        m_VipSeatTrans = m_BaseTrans.GetChild(2);
        m_VipSeatWnd = m_VipSeatTrans.gameObject;
        m_TitleTrans = m_BaseTrans.GetChild(1);
        m_TitleWnd = m_TitleTrans.gameObject;
        m_HostLineTrans = m_BaseTrans.GetChild(11);
        m_HostLineWnd = m_HostLineTrans.gameObject;
        m_NormalTrans = m_BaseTrans.GetChild(12);
        m_NormalWnd = m_NormalTrans.gameObject;
        m_DesTrans = m_BaseTrans.GetChild(13);
        m_DesWnd = m_DesTrans.gameObject;
        SceneObjMgr.Instance.UIPanelTransform.GetChild(5).gameObject.SetActive(false);
        SceneObjMgr.Instance.UIPanelTransform.GetChild(6).gameObject.SetActive(false);
        SceneObjMgr.Instance.UIPanelTransform.GetChild(7).gameObject.SetActive(false);

        UIEventListener.Get(m_BaseTrans.GetChild(8).gameObject).onClick = EventBack;
        UIEventListener.Get(m_BaseTrans.GetChild(9).gameObject).onClick = EventOpenTrend;
        UIEventListener.Get(m_BaseTrans.GetChild(10).gameObject).onClick = EventOpenNormal;
        UIEventListener.Get(m_BaseTrans.GetChild(7).gameObject).onClick = EventOpenDes;
        UIEventListener.Get(m_DesTrans.GetChild(0).gameObject).onClick = EventCloseDes;

        InitHead();
        InitCountDownWnd();
        InitWeagerWnd();
        InitResultWnd();
        InitTrendWnd();
        InitVipSeat();
        InitTitle();
        InitHostLine();
        InitNormal();

        {//重置
            isWeager = false;
            isWait = false;
            isAnim = false;
            isShowMyWeager = false;
            isAnimalMove = false;
            isAnimalMoveBack = false;
            isEffect = false;
            isTrendOpen = false;
            isInLine = false;
            isHost = false;
            isMasterHost = true;
        }        
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
        m_HostEffectWnd = (GameObject)GameObject.Instantiate(m_EffectHostObj);
        Transform trans = m_HostEffectWnd.transform;
        trans.SetParent(m_BaseTrans, false);
        trans.GetChild(0).GetChild(0).localPosition = new Vector3(-444, -8, 0);
        GlobalEffectMgr.SetTrailEffectOnTopWnd(m_HostEffectWnd);
        GlobalEffectMgr.SetMeshRendererOnTopWnd(m_HostEffectWnd);

        hostBtnObj = new GameObject[2];
        hostBtnObj[0] = m_TitleTrans.GetChild(0).gameObject;
        hostBtnObj[1] = m_TitleTrans.GetChild(1).gameObject;
        UIEventListener.Get(hostBtnObj[0]).onClick = EventUpHost;
        UIEventListener.Get(hostBtnObj[1]).onClick = EventDownHost;
        UIEventListener.Get(trans.GetChild(0).GetChild(0).gameObject).onClick = EventOpenHostLine;
        hostNameLabel = m_TitleTrans.GetChild(3).GetComponent<UILabel>();
        hostGoldNumLabel = m_TitleTrans.GetChild(3).GetChild(0).GetComponent<UILabel>();
        hostHeadObj = m_TitleTrans.GetChild(2).gameObject;

        hostBtnObj[0].SetActive(true);
        hostBtnObj[1].SetActive(false);
        hostNameLabel.gameObject.SetActive(false);
        hostHeadObj.SetActive(false);
    }

    void InitVipSeat()
    {
        vipSeat = new ForestDanceVipSeat[8];
        for (byte i = 0; i < vipSeat.Length; i++)
        {
            vipSeat[i] = new ForestDanceVipSeat();
            vipSeat[i].Init(m_VipSeatTrans.GetChild(i), i);
            vipInfo[i] = new ForestDanceVipInfo();
            for (byte j = 0; j < 4; j++)
            {
                vipInfo[i].weagerIndex[j] = false;
            }
        }
    }

    void InitTrendWnd()
    {
        trendColorSprite = new UISprite[7];
        trendKindSprite = new UISprite[7];
        for (byte i = 0; i < 7; i++)
        {
            trendColorSprite[i] = m_TrendTrans.GetChild(0).GetChild(i).GetComponent<UISprite>();
            trendKindSprite[i] = m_TrendTrans.GetChild(0).GetChild(i).GetChild(0).GetComponent<UISprite>();
        }
        trendTween = m_TrendTrans.GetChild(0).GetComponent<TweenScale>();
    }

    void InitResultWnd()
    {
        m_ResultWndTween = m_ResultTrans.GetComponent<TweenPosition>();
        m_ResultTitleTween = m_ResultTrans.GetChild(0).GetComponent<TweenPosition>();
        m_ResultRewardKindTween = m_ResultTrans.GetChild(1).GetComponent<TweenPosition>();
        m_ResultBGKindTween = m_ResultTrans.GetChild(2).GetComponent<TweenPosition>();
        weagerResult = new WeagerResult[12];
        resultColorSprite = m_ResultTrans.GetChild(1).GetComponent<UISprite>();
        resultKindSprite = m_ResultTrans.GetChild(1).GetChild(0).GetComponent<UISprite>();
        resultRateLabel = m_ResultTrans.GetChild(1).GetChild(2).GetComponent<UILabel>();
        myResultLabel = m_ResultTrans.GetChild(1).GetChild(3).GetChild(0).GetComponent<UILabel>();
        hostResultLabel = m_ResultTrans.GetChild(1).GetChild(3).GetChild(1).GetComponent<UILabel>();
        for (byte i = 0; i < 12; i++)
        {
            weagerResult[i] = new WeagerResult();
            weagerResult[i].Init(m_ResultTrans.GetChild(2).GetChild(i));
        }
    }

    void InitWeagerWnd()
    {
        totalWeagerLabel = new UILabel[12];
        weagerRateLabel = new UILabel[12];
        weagerChooseObj = new GameObject[5];
        weagerWnd = new WeagerWnd[12];
        weagerArr = new uint[] { 1000, 10000, 100000, 500000, 1000000 };
        for (byte i = 0; i < 12; i++)
        {
            totalWeagerLabel[i] = m_WeagerTrans.GetChild(1).GetChild(i).GetChild(0).GetComponent<UILabel>();
            weagerRateLabel[i] = m_WeagerTrans.GetChild(2).GetChild(i).GetChild(0).GetComponent<UILabel>();
            weagerWnd[i] = new WeagerWnd();
            weagerWnd[i].Init(m_WeagerTrans.GetChild(2).GetChild(i), i);
        }
        UIEventListener.Get(m_WeagerTrans.GetChild(0).gameObject).onClick = EventCloseWeagerWnd;
        for (byte i = 0; i < 5; i++)
        {
            weagerChooseObj[i] = m_WeagerTrans.GetChild(3).GetChild(i + 5).gameObject;
            UIEventListener.Get(m_WeagerTrans.GetChild(3).GetChild(i).gameObject).onClick = EventChangeWeagerIndex;
        }
        UIEventListener.Get(m_WeagerTrans.GetChild(3).GetChild(10).gameObject).onClick = EventContinue;
    }

    void InitCountDownWnd()
    {
        m_CountDownTrans = new Transform[2];
        m_CountDownTrans[0] = m_BaseTrans.GetChild(6).GetChild(0);
        m_CountDownTrans[1] = m_BaseTrans.GetChild(6).GetChild(1);
        m_CountDownWnd = new GameObject[2];
        m_CountDownWnd[0] = m_CountDownTrans[0].gameObject;
        m_CountDownWnd[1] = m_CountDownTrans[1].gameObject;
        m_CountDownLabel = new UILabel[2];
        m_CountDownLabel[0] = m_CountDownTrans[0].GetChild(0).GetComponent<UILabel>();
        m_CountDownLabel[1] = m_CountDownTrans[1].GetChild(0).GetComponent<UILabel>();
    }

    void InitHead()
    {
        m_GoldLabel = m_HeadTrans.GetChild(2).GetComponent<UILabel>();
        SetPlayerGold();
        m_HeadTrans.GetChild(1).GetComponent<UILabel>().text = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();
        uint ficeID = PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID();
        if (ficeID < ConstValue.CUSTOM_HEADER)
        {
            m_HeadTex = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
            m_HeadTrans.GetChild(0).GetComponent<UITexture>().mainTexture = m_HeadTex;
        }
        else
        {
            HeaderManager.Instance.GetPlayerHeader(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), ficeID, HeaderOptType.HEADER_FORESTDANCE, null);
        }

        UIEventListener.Get(m_HeadTrans.GetChild(3).gameObject).onClick = EventAddGoldClick;
        UIEventListener.Get(m_HeadTrans.GetChild(4).gameObject).onClick = EventWeagerClick;
    }

    public void ShowWnd()
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseWndObject);
        }
    }

    public void EventBack(GameObject go)
    {
        bool bBet = false;
        for (byte i = 0; i < 12; i++)
        {
            if(weagerWnd[i].isWeager)
            {
                bBet = true;
                break;
            }
        }

        if (bBet && isWeager)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_LeaveRoom"), MssageType.ForestDance_Exit_Tips);
        }
        else if (isInLine && !isHost)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_LineLeave"), MssageType.ForestDance_Exit_Tips);
        }
        else if (isHost && !isInLine)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_HostLeave"), MssageType.ForestDance_Exit_Tips);
        }
        else
        {
            PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnLeaveDialTable();
            BackToMain();
            ShutDown();
        }              
    }

    void EventOpenTrend(GameObject go)
    {
        if (isTrendOpen)
        {
            trendTween.PlayReverse();
            isTrendOpen = false;
            return;
        }
        trendTween.PlayForward();
        trendOpenCountDown = 0;
        isTrendOpen = true;
    }

    public void SetPlayerHead(Texture2D tex)
    {
        m_HeadTex = tex;
        m_HeadTrans.GetChild(0).GetComponent<UITexture>().mainTexture = m_HeadTex;
    }

    public void SetPlayerGold()
    {      
        if(IsDelayUpate())
        {
            return;
        }
        uint goldNum = PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel();
        if (goldNum >= 10000)
        {
            m_GoldLabel.text = (goldNum / 10000) + "." + (goldNum / 1000 % 10) + " 万";
        }
        else
        {
            m_GoldLabel.GetComponent<UILabel>().text = goldNum.ToString();
        }
    }

    void EventAddGoldClick(GameObject go)
    {
        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Gold);
    }

    void EventWeagerClick(GameObject go)
    {
        m_WeagerWnd.SetActive(true);
        WndManager.Instance.Push(m_WeagerWnd);
    }

    void EventCloseWeagerWnd(GameObject go)
    {
        if (m_WeagerWnd.activeSelf)
        {
            WndManager.Instance.Pop(m_WeagerWnd);
            m_WeagerWnd.SetActive(false);
        }
    }

    void EventContinue(GameObject go)
    {
        PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnAddGlobelByLog();
    }

    void EventChangeWeagerIndex(GameObject go)
    {
        for (byte i = 0; i < 5; i++)
        {
            if (go == m_WeagerTrans.GetChild(3).GetChild(i).gameObject)
            {
                weagerChooseObj[i].SetActive(true);
                weagerIndex = i;
            }
            else
                weagerChooseObj[i].SetActive(false);
        }

        for (byte i = 0; i < 12; i++)
        {
            weagerWnd[i].SetGlobal(weagerArr[weagerIndex]);
        }
    }

    public void SetWeagerBtn()
    {
        uint myGold = PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel();
        if (myGold < weagerArr[weagerIndex])
        {
            for (int i = (byte)weagerIndex; i >= 0; --i)
            {
                if (myGold >= weagerArr[i])
                {
                    EventChangeWeagerIndex(m_WeagerTrans.GetChild(3).GetChild(i).gameObject);
                    break;
                }
            }
        }
    }

    //进入桌子
    public void GetTableState(byte tableUpdateSec, DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo,
        DialTableAreaData AreaData, DialBankerUserInfo TableBankerInfo, DialWriteBankerList TableWriteBankerList,
        DialVipSeatList TableVipSeatInfo, DialGameLog TableGameLog, uint tableResult)
    {
        
        SetForestWndBegin(tableResult);
        UpdateHost(TableBankerInfo);
        GetHostLine(TableWriteBankerList);
        UpdateVipInfo(TableVipSeatInfo);
        SetTrend(TableGameLog);
        SetGameBegin();
        SetTableAreaData(AreaData.TableAreaData,false);
        SetTableAreaRate(AreaData.TableAreaDataRate);
        UpdateTotalWeager(TableGlobelInfo, TableStates);
        if (TableStates == DialTableStates.DTS_Begin)
        {
            m_CountDownWnd[0].SetActive(true);
            m_CountDownWnd[1].SetActive(false);
            weagerCountDown = tableUpdateSec;
            isWeager = true;           
        }
        else if (TableStates == DialTableStates.DTS_End)
        {
            m_CountDownWnd[0].SetActive(false);
            m_CountDownWnd[1].SetActive(true);
            waitCountDown = tableUpdateSec;
            isWait = true;         
        }
        PlayBkMusic(TableStates);
    }

    //状态改变
    public void StateChange(DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo,
        DialTableAreaData AreaData, DialVipSeatList OldVipSeatInfo, DialVipSeatList TableVipSeatInfo,
        DialWriteBankerList TableWriteBankerList, DialBankerUserInfo TableBankerUserInfo,
        DialGameLog TableGameLog, uint tableResult)
    {                    
        if (TableStates == DialTableStates.DTS_Begin)
        {
            SetPlayerGold();
            GameObject.Destroy(m_EffectObj);
            GameObject.Destroy(m_ResultEffectWnd);
            CloseResult();
            SetGameBegin();
            m_CountDownWnd[0].SetActive(true);
            m_CountDownWnd[1].SetActive(false);
            weagerCountDown = FishConfig.Instance.m_MiNiGameConfig.dialConfig.BeginWriteSec;
            isWeager = true;
        }
        else if (TableStates == DialTableStates.DTS_End)
        {
            SetResultValue(tableResult, AreaData, TableGlobelInfo);
            m_pGameLog = TableGameLog;            
            animalIndex = (byte)tableResult;
            if (animalIndex > 17)
                animalIndex = 41 - animalIndex;
            else
                animalIndex = 17 - animalIndex;
            //Debug.Log(animalIndex);
            m_CountDownWnd[0].SetActive(false);
            EventCloseWeagerWnd(null);
            BeginRotate(tableResult);
        }
        UpdateTotalWeager(TableGlobelInfo, TableStates);
        SetTableAreaData(AreaData.TableAreaData, true);
        SetTableAreaRate(AreaData.TableAreaDataRate);
        UpdateHost(TableBankerUserInfo);
        PlayBkMusic(TableStates);
    }

    void SetForestWndBegin(uint tableResult)
    {
        m_ForestWnd.SetBegin(tableResult);
    }

    public void UpdateTotalWeager(DialTableUpdateInfo TableGlobelInfo, DialTableStates TableStates)
    {
        for (byte i = 0; i < 12; i++)
        {
            totalWeagerLabel[i].text = TableGlobelInfo.GlobelNum[i].ToString();
        }
    }

    public void UpdateMyWeager(uint[] TableGlobelInfo)
    {
        for (byte i = 0; i < 12; i++)
        {
            weagerWnd[i].myWeagerLabel.text = TableGlobelInfo[i].ToString();
        }
        SetPlayerGold();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_onchip);
    }

    void SetTableAreaRate(byte[] areaRate)
    {
        for (byte i = 0; i < 12; i++)
        {
            weagerRateLabel[i].text = areaRate[i].ToString();
            weagerResult[i].rateLabel.text = areaRate[i].ToString();
        }
    }

    void SetTableAreaData(byte[] areaDate, bool bShowAnimation)
    {
        m_ForestWnd.SetTableAreaDate(areaDate, bShowAnimation);
    }

    void BeginRotate(uint tableResult)
    {
        m_ForestWnd.BeginRotate(tableResult);
        animCountDown = 3.5f;
        isAnim = true;
        animalMoveCountDown = 0;
        animalMoveBackCountDown = 0;
        isAnimalMove = true;
        isEffect = true;
    }

    void SetResultValue(uint tableResult, DialTableAreaData AreaData, DialTableUpdateInfo TableGlobelInfo)
    {
        byte index1 = (byte)(tableResult >> 8);
        byte index2 = (byte)tableResult;
        byte rank = (byte)(AreaData.TableAreaData[index1] / 4);
        byte value = (byte)(FishConfig.Instance.m_MiNiGameConfig.dialConfig.DialAreaData[index2]);
        if (rank == 0)
            resultColorSprite.spriteName = "RedCircle";
        else if (rank == 1)
            resultColorSprite.spriteName = "YellowCircle";
        else if (rank == 2)
            resultColorSprite.spriteName = "GreenCircle";
        if (value == 0)
            resultKindSprite.spriteName = "Xiongmao";
        else if (value == 1)
            resultKindSprite.spriteName = "Houzi";
        else if (value == 2)
            resultKindSprite.spriteName = "Tuzi";
        else if (value == 3)
            resultKindSprite.spriteName = "Shizi";

        resultIndex = rank * 4 + value;

        byte rateIndex = (byte)(rank * 4 + value);
        byte rate = AreaData.TableAreaDataRate[rateIndex];
        resultRateLabel.text = rate.ToString();

        int myResult = 0;
        int hostResult = 0;
        for (byte i = 0; i < 12; i++)
        {
            hostResult += (int)TableGlobelInfo.GlobelNum[i];
            myResult -= (int)weagerWnd[i].myWeagerNum;
            if (i == rateIndex)
            {
                myResult += rate * (int)weagerWnd[i].myWeagerNum;
                hostResult -= rate * (int)TableGlobelInfo.GlobelNum[i];
            }
        }
        if (!isHost)
        {
            myResultLabel.text = myResult.ToString();
            if (myResult >= 0)
                myResultLabel.text = "+" + myResult;
            hostResultLabel.text = hostResult.ToString();
            if (hostResult >= 0)
                hostResultLabel.text = "+" + hostResult;
        }
        else
        {
            myResult = hostResult;//same
            hostResultLabel.text = hostResult.ToString();
            myResultLabel.text = hostResult.ToString();
            if (hostResult >= 0)
            {
                hostResultLabel.text = "+" + hostResult;
                myResultLabel.text = "+" + hostResult;
            }
        }
        GlobalHallUIMgr.Instance.GameShare.AddGlod(myResult);
    }

    void ShowResult()
    {
        if (isPush == false)
        {
            WndManager.Instance.Push(m_ResultWnd);
            isPush = true;
        }
        m_ResultTitleTween.gameObject.SetActive(true);
        m_ResultWnd.SetActive(true);
        m_ResultWndTween.ResetToBeginning();
        m_ResultTitleTween.ResetToBeginning();
        m_ResultRewardKindTween.ResetToBeginning();
        m_ResultBGKindTween.ResetToBeginning();
        m_ResultWndTween.PlayForward();
        for (byte i = 0; i < 12; i++)
        {
            if (weagerWnd[i].isWeager)
                weagerResult[i].SetBG(1);
            else
                weagerResult[i].SetBG(0.5f);
            weagerResult[i].weagerLabel.text = weagerWnd[i].myWeagerLabel.text;
        }
        showMyWeagerCountDown = 2.5f;
        isShowMyWeager = true;
    }

    void SetTrend(DialGameLog TableGameLog)
    {
        byte gameSum = TableGameLog.GameSum;
        byte[] logArr = GetUIntArray(TableGameLog.GameLog);
        int BegIndex=0;
        int EndIndex = (int)((gameSum <= 7) ? gameSum - 1 : 7);
        if (EndIndex==7)
        {
            BegIndex = 1;
        }
        int nCurrentIndex = EndIndex;
           
        //for (byte i = EndIndex; i < 7; i++)
        //{
        //    logArr[i] = (byte)Random.Range(0, 11);
        //}
        for (int i = 6; i >= 0; i--)
        {
            if (nCurrentIndex >= BegIndex)
            {
                byte rank = (byte)(logArr[nCurrentIndex] / 4);
                byte value = (byte)(logArr[nCurrentIndex] % 4);

                if (rank == 0)
                    trendColorSprite[i].spriteName = "RedCircle";
                else if (rank == 1)
                    trendColorSprite[i].spriteName = "YellowCircle";
                else if (rank == 2)
                    trendColorSprite[i].spriteName = "GreenCircle";
                if (value == 0)
                    trendKindSprite[i].spriteName = "Xiongmao";
                else if (value == 1)
                    trendKindSprite[i].spriteName = "Houzi";
                else if (value == 2)
                    trendKindSprite[i].spriteName = "Tuzi";
                else if (value == 3)
                    trendKindSprite[i].spriteName = "Shizi";
                nCurrentIndex--;
            }
            else
            {
                trendColorSprite[i].spriteName = "";
                trendKindSprite[i].spriteName = "";
            }
        }
    }
    byte[] GetUIntArray(uint Value)
    {
        byte[] Array = new byte[8];
        for (int i = 0; i < 8; ++i)
        {
            Array[i] = (byte) ( (Value >>(i * 4)  ) & (uint)0x0f);
            //byte ValueByte = (byte)(Value >> ((i) * 8));//高位在前面 低位在后面
            //byte Value1 = (byte)(ValueByte >> 4);
            //byte Value2 = (byte)(ValueByte - (Value1 << 4));
            //Array[2 * i] = Value1;
            //Array[2 * i + 1] = Value2;
        }
        return Array;
    }

    void CloseResult()
    {
        m_ResultTitleTween.gameObject.SetActive(false);
        if (isPush)
        {
            WndManager.Instance.Pop(m_ResultWnd);
            isPush = false;
        }
        m_ResultWndTween.PlayReverse();
    }

    void UpdateVipInfo(DialVipSeatList list)
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
                    HeaderManager.Instance.GetPlayerHeader(list.List[i].dwUserID, ficeID, HeaderOptType.HEADER_FORESTDANCEVIP, i);
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

    public void VipSeatChange(bool result, DialVipSeatList list)
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

    public void BeLeaveVip(DialRoleInfo info)
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
        uint globalSum = FishConfig.Instance.m_MiNiGameConfig.dialConfig.JoinBankerGlobelSum;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < globalSum)
        {
            string str = StringTable.GetString("UOM_Niuniu_UpHostFail");
            string newStr = str.Replace("@", globalSum.ToString());
            GlobalEffectMgr.Instance.ShowMessageBox(newStr, MssageType.NiuNiu_UpHostFail_Tips);
        }
        if (PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnRoleJoinWriteBankerList())
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

    public void GetHostLine(DialWriteBankerList list)
    {
        ClearGrid(hostListGrid);
        for (byte i = 0; i < FishDataInfo.MAX_Dial_BankerShowSum; i++)
        {
            if (list.List[i].dwUserID == 0)
            {
                break;
            }
            DialRoleInfo info = list.List[i];
            ForestDanceHostItem item = new ForestDanceHostItem((uint)i + 1, info.NickName, info.GlobelSum);
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
        if (PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnRoleLeaveWriteBankerList())
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
        if (PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnRoleCanelBanker())
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

    public void UpdateHost(DialBankerUserInfo info)
    {
        if(IsDelayUpate())
        {
            m_pHostInfo = info;
            return;
        }
        if (info.UserInfo.dwUserID == 0)
        {
            hostHeadObj.SetActive(false);
            hostNameLabel.gameObject.SetActive(false);
            m_TitleTrans.GetChild(4).gameObject.SetActive(true);
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
            m_TitleTrans.GetChild(4).gameObject.SetActive(false);
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
                HeaderManager.Instance.GetPlayerHeader(info.UserInfo.dwUserID, ficeID, HeaderOptType.HEADER_FORESTDANCEHOST, null);
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
        uint needGlobal = FishConfig.Instance.m_MiNiGameConfig.dialConfig.GetNextBankerNeedGlobel;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() > needGlobal)
        {
            string str = StringTable.GetString("UOM_Niuniu_SnatchHost");
            string newStr = str.Replace("@", needGlobal.ToString());
            GlobalEffectMgr.Instance.ShowMessageBox(newStr, MssageType.ForestDance_SnatchHost_Tips);
        }
    }

    public void SetHostHeader(Texture2D tex)
    {
        hostHeadTex = tex;
        hostHeadObj.transform.GetChild(0).GetComponent<UITexture>().mainTexture = hostHeadTex;
    }

    void EventOpenNormal(GameObject go)
    {
        WndManager.Instance.Push(m_NormalWnd);
        m_NormalWnd.SetActive(true);

        PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnGetNormalInfo(0);
        currentPage = 0;
        totalPage = 0;
    }

    public void GetNormalPeople(uint page, uint sum, DialRoleInfo[] array)
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
            ForestDanceNormalItem item = new ForestDanceNormalItem(array[i].NickName, array[i].GlobelSum);
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
                HeaderManager.Instance.GetPlayerHeader(array[i].dwUserID, ficeID, HeaderOptType.HEADER_FORESTDANCENORMAL, i);
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
            PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnGetNormalInfo(currentPage);
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
            PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnGetNormalInfo(currentPage);
        }
    }

    void EventOpenDes(GameObject go)
    {
        m_DesWnd.SetActive(true);
        WndManager.Instance.Push(m_DesWnd);
    }

    void EventCloseDes(GameObject go)
    {
        m_DesWnd.SetActive(false);
        WndManager.Instance.Pop(m_DesWnd);
    }

    void SetGameBegin()
    {
        for (byte i = 0; i < 12; i++)
        {
            totalWeagerLabel[i].text = "0";
            weagerWnd[i].myWeagerLabel.text = "0";
            weagerWnd[i].isWeager = false;
            weagerResult[i].SetBG(1);
            weagerResult[i].weagerLabel.text = "0";
            weagerWnd[i].myWeagerNum = 0;
        }
    }

    Vector3 animalVector;
    Vector3 animalQua;

    const float MOVE_TIME = 1.0f;//动物的移动时间（秒）
    public void Update(float deltaTime)
    {
        if (m_BaseTrans == null)
            return;
        m_ForestWnd.Update(deltaTime);
        //下注阶段倒计时
        if (isWeager)
        {
            float fOldTime = weagerCountDown;
            weagerCountDown -= deltaTime;
            m_CountDownLabel[0].text = ((int)weagerCountDown).ToString();
            if (weagerCountDown <= 0)
            {
                m_CountDownWnd[0].SetActive(false);
                isWeager = false;
            }
            if ((int)fOldTime - (int)weagerCountDown == 1)
            {
                if ((int)weagerCountDown == 0)
                {
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_cdover);
                }
                else if ((int)weagerCountDown <= 5)
                {
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_cd);
                }
            }
        }
        //结算阶段倒计时
        if (isWait)
        {
            waitCountDown -= deltaTime;
            m_CountDownLabel[1].text = ((int)waitCountDown).ToString();
            if (waitCountDown <= 0)
            {
                m_CountDownWnd[1].SetActive(false);
                isWait = false;
            }
        }

        if (isAnim && m_ForestWnd.isPointResult && m_ForestWnd.isResult)
        {
            if (isEffect)
            {
                m_EffectObj = (GameObject)GameObject.Instantiate(m_EffectStep);
                GlobalEffectMgr.SetTrailEffectOnTopWnd(m_EffectObj);
                GlobalEffectMgr.SetMeshRendererOnTopWnd(m_EffectObj);
                m_EffectObj.transform.SetParent(m_ForestWnd.m_AnimalTrans.GetChild(animalIndex), false);
                isEffect = false;
                animalVector = m_ForestWnd.m_AnimalTrans.GetChild(animalIndex).GetChild(1).localPosition;
                animalQua = m_ForestWnd.m_AnimalTrans.GetChild(animalIndex).GetChild(1).localRotation.eulerAngles;
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_redpanda + resultIndex);
            }
            if (isAnimalMove)
            {
                animalMoveCountDown += deltaTime;
                if (animalMoveCountDown >= MOVE_TIME)
                {
                    m_ForestWnd.animalAnim[animalIndex].SetBool("idle", false);
                    animalMoveCountDown = MOVE_TIME;
                    isAnimalMove = false;
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_panda + resultIndex%4);
                }
                Vector3 vector = Vector3.Lerp(animalVector, new Vector3(0, 0.6f, 0), animalMoveCountDown / MOVE_TIME);
                Vector3 qua = Vector3.Lerp(animalQua, new Vector3(0,
                    -(m_ForestWnd.m_AnimalTrans.GetChild(animalIndex).localRotation.eulerAngles.y + 180)
                    - m_ForestWnd.m_AnimalTrans.localRotation.eulerAngles.y, 0), animalMoveCountDown / MOVE_TIME);
                m_ForestWnd.m_AnimalTrans.GetChild(animalIndex).GetChild(1).localPosition = vector;
                m_ForestWnd.m_AnimalTrans.GetChild(animalIndex).GetChild(1).localRotation = Quaternion.Euler(qua);
            }
            animCountDown -= deltaTime;
            if (animCountDown <= 0)
            {
                m_ForestWnd.animalAnim[animalIndex].SetBool("idle", true);
                animalVector = m_ForestWnd.m_AnimalTrans.GetChild(animalIndex).GetChild(1).localPosition;
                animalQua = m_ForestWnd.m_AnimalTrans.GetChild(animalIndex).GetChild(1).localRotation.eulerAngles;

               
                ShowResult();
                isAnim = false;
                m_ForestWnd.isResult = false;
                m_ForestWnd.isPointResult = false;
                m_ForestWnd.cameraAnim.SetBool("isCameraFront", false);
                m_ForestWnd.cameraAnim.SetBool("isCameraBack", true);
                
            }
        }

        if (isAnimalMoveBack)
        {
            animalMoveBackCountDown += deltaTime;
            if (animalMoveBackCountDown >= MOVE_TIME)
            {
                animalMoveBackCountDown = MOVE_TIME;
                isAnimalMoveBack = false;

               
            }
            Vector3 vector = Vector3.Lerp(animalVector, new Vector3(0, 0.6f, 39.5f), animalMoveBackCountDown / MOVE_TIME);
            Vector3 qua = Vector3.Lerp(animalQua, new Vector3(0, -180, 0), animalMoveBackCountDown / MOVE_TIME);
            m_ForestWnd.m_AnimalTrans.GetChild(animalIndex).GetChild(1).localPosition = vector;
            m_ForestWnd.m_AnimalTrans.GetChild(animalIndex).GetChild(1).localRotation = Quaternion.Euler(qua);
        }

        if (isShowMyWeager)
        {
            showMyWeagerCountDown -= deltaTime;
            if (showMyWeagerCountDown <= 0)
            {
                if (m_ResultEffectWnd != null)
                    GameObject.Destroy(m_ResultEffectWnd);
                m_ResultEffectWnd = (GameObject)GameObject.Instantiate(m_ResultEffectObj);
                GlobalEffectMgr.SetEffectOnUI(m_ResultEffectWnd);
                m_ResultEffectWnd.transform.SetParent(m_ResultTrans.GetChild(2).GetChild(resultIndex), false);
                m_ResultTitleTween.PlayForward();
                m_ResultRewardKindTween.PlayForward();
                m_ResultBGKindTween.PlayForward();
                isShowMyWeager = false;

                isAnimalMoveBack = true;
                m_ForestWnd.areaAnim.SetBool("isAreaOpen", false);
                m_ForestWnd.areaAnim.SetBool("isAreaClose", true);
               
                {//延迟刷新
                    if (m_pGameLog!=null)
                    {
                        SetTrend(m_pGameLog);
                        m_pGameLog = null;
                    }              
                    if(m_pHostInfo!=null)
                    {
                        UpdateHost(m_pHostInfo);
                        m_pHostInfo = null;
                    }
                    SetPlayerGold();                     
                }
            }
        }

        if (isTrendOpen)
        {
            trendOpenCountDown += deltaTime;
            if (trendOpenCountDown >= 3.0f)
            {
                isTrendOpen = false;
                trendTween.PlayReverse();
            }
        }
    }
    void PlayBkMusic(DialTableStates TableStates)
    {
        if (TableStates == DialTableStates.DTS_Begin)
        {
            GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound6);
        }
        else if (TableStates == DialTableStates.DTS_End)
        {
            GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound7);
        }
        
    }
    public void BackToMain()
    {
        SceneObjMgr.Instance.UIPanelTransform.GetChild(5).gameObject.SetActive(true);
        SceneObjMgr.Instance.UIPanelTransform.GetChild(6).gameObject.SetActive(true);
        SceneObjMgr.Instance.UIPanelTransform.GetChild(7).gameObject.SetActive(true);
    }
    public void ShutDown()
    {
      
        if (m_BaseTrans != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            WndManager.Instance.Pop(m_WeagerWnd);
            WndManager.Instance.Pop(m_HostLineWnd);
            WndManager.Instance.Pop(m_ResultWnd);
            WndManager.Instance.Pop(m_DesWnd);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
            GlobalHallUIMgr.Instance.GameShare.LveaveGame();
        }
        m_ForestWnd.ShutDown();
        GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound2);
        //SceneObjMgr.Instance.SceneCamera.SetActive(true);
        //SceneObjMgr.Instance.SceneBackground.SetActive(true);
    }
    private  bool IsDelayUpate()
    {
        if(isAnim||isShowMyWeager)
        {
            return true;
        }
        return false;
    }


}

class WeagerWnd : BaseWnd
{
    public UILabel myWeagerLabel;
    public GameObject weagerBtnObj;
    uint global = 1000;
    public byte index;
    public bool isWeager = false;
    public uint myWeagerNum = 0;

    public void Init(Transform trans, byte index)
    {
        myWeagerLabel = trans.GetChild(3).GetComponent<UILabel>();
        weagerBtnObj = trans.GetChild(1).gameObject;
        this.index = index;
        myWeagerNum = 0;
        UIEventListener.Get(trans.GetChild(1).gameObject).onClick = EventWeager;
    }

    public void SetGlobal(uint global)
    {
        this.global = global;
    }

    void EventWeager(GameObject go)
    {
        if (PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnAddGlobel(global, index))
        {
            isWeager = true;
            myWeagerNum += global;
        }
    }
}

class WeagerResult : BaseWnd
{
    public UISprite bg;
    public UILabel rateLabel;
    public UILabel weagerLabel;

    public void Init(Transform trans)
    {
        m_BaseTrans = trans;
        bg = m_BaseTrans.GetChild(1).GetComponent<UISprite>();
        rateLabel = m_BaseTrans.GetChild(0).GetComponent<UILabel>();
        weagerLabel = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
    }

    public void SetBG(float alpha)
    {
        bg.alpha = alpha;
    }
}

class ForestDanceHostItem : BaseWnd
{
    uint lineNum;
    string name;
    uint goldNum;

    UILabel lineNumLabel;
    UILabel nameLabel;
    UILabel goldNumLabel;

    public ForestDanceHostItem(uint lineNum, string name, uint goldNum)
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

class ForestDanceVipSeat
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
        PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnRoleJoinVipSeat(index);
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

class ForestDanceNormalItem : BaseWnd
{
    public GameObject headObj;

    public Texture2D headTex;
    public UILabel nameLabel;
    public UILabel goldLabel;

    public uint id;
    string name;
    uint gold;

    public ForestDanceNormalItem(string name, uint gold)
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

class VipInfo
{
    public uint userId = 0;
    public uint oldGoldnum;
    public uint newGoldNum;
    public bool[] weagerIndex = new bool[4];
}