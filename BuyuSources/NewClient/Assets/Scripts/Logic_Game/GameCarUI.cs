using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class PtInfo
{
    public int nIndex;
    public Vector2 pt;
    public float fstartpos;
    public float fendpos;
    public float fRadians;
}

public class CarMove
{
    public List<PtInfo> m_outside = new List<PtInfo>();
    public List<PtInfo> m_Inside = new List<PtInfo>();
    public bool m_bspeedup = false;
    public bool m_bSustain = false;
    public bool m_bspeeddown = false;
    public float m_facceleration;
    public float m_fspeed;
    public float m_fcurpos;
    public float m_fstopdis;

    const int nHCount = 7;
    const int nVCount = 9;
    Vector2 m_vectopleft;//左上角
    Vector2 m_vevbottomright;//右下角     
    //bool m_bfirstround = false;
    public void Init(float x1, float y1, float x2, float y2, float foffset)
    {
        m_bspeedup = false;
        m_bSustain = false;
        m_bspeeddown = false;

        m_vectopleft.Set(x1, y1);
        m_vevbottomright.Set(x2, y2);

        Vector2 vtl = m_vectopleft;
        Vector2 vbr = m_vevbottomright;
        vtl.y += foffset;
        vbr.y -= foffset;

        m_outside.Clear();
        m_Inside.Clear();

        LoGolPos(m_outside, vtl, vbr);
        LoGolPos(m_Inside, m_vectopleft, m_vevbottomright);
    }
    void LoGolPos(List<PtInfo> ptList, Vector2 v1, Vector2 v2)
    {
        float rectanglew = v2.x - v1.x;
        float rectangleh = v1.y - v2.y;
        float fHalfCirle = Mathf.PI * rectangleh / 2;
        float fSeg = rectanglew / (nHCount - 1);
        float fSegCircle = fHalfCirle / (nVCount + 1);
        int nIndex = 0;

        //上
        for (int i = 0; i < nHCount; i++)
        {
            PtInfo item = new PtInfo();
            item.nIndex = nIndex++;
            item.pt.x = v1.x + (i) * fSeg;
            item.pt.y = v1.y;
            item.fRadians = 0;
            item.fstartpos = i * fSeg;
            item.fendpos = item.fstartpos + (i != nHCount - 1 ? fSeg : fSegCircle);
            ptList.Add(item);
        }

        //右边
        float fRadians = Mathf.PI / (nVCount + 1);
        for (int i = 0; i < nVCount; i++)
        {
            Vector2 ptOrigin = v2;
            ptOrigin.y += rectangleh / 2;

            PtInfo item = new PtInfo();
            item.nIndex = nIndex++;
            item.pt.x = ptOrigin.x + rectangleh / 2 * Mathf.Sin((i + 1) * fRadians);
            item.pt.y = ptOrigin.y + rectangleh / 2 * Mathf.Cos((i + 1) * fRadians);
            item.fRadians = (i + 1) * fRadians;
            item.fstartpos = rectanglew + (i + 1) * fSegCircle;
            item.fendpos = item.fstartpos + fSegCircle;
            ptList.Add(item);
        }

        //下
        for (int i = 0; i < nHCount; i++)
        {
            PtInfo item = new PtInfo();
            item.nIndex = nIndex++;
            item.pt.x = v2.x - (i) * fSeg; ;
            item.pt.y = v2.y;

            item.fRadians = 0;
            item.fstartpos = rectanglew + fHalfCirle + i * fSeg;
            item.fendpos = item.fstartpos + (i != nHCount - 1 ? fSeg : fSegCircle);
            if (i == nHCount - 1)
            {
                item.fendpos = item.fstartpos + fSegCircle;
            }
            ptList.Add(item);
        }

        //左边
        for (int i = 0; i < nVCount; i++)
        {
            Vector2 ptOrigin = v1;
            ptOrigin.y -= rectangleh / 2;

            PtInfo item = new PtInfo();
            item.nIndex = nIndex++;
            item.pt.x = ptOrigin.x - rectangleh / 2 * Mathf.Sin((i + 1) * fRadians); ;
            item.pt.y = ptOrigin.y - rectangleh / 2 * Mathf.Cos((i + 1) * fRadians);
            item.fRadians = (i + 1) * fRadians;
            item.fstartpos = 2 * rectanglew + fHalfCirle + (i + 1) * fSegCircle;
            item.fendpos = item.fstartpos + fSegCircle;
            ptList.Add(item);
        }

    }
    public void CarPos(ref Vector2 vec, ref int nIndex, ref float fAngle)
    {
        float rectanglew = m_vevbottomright.x - m_vectopleft.x;
        float rectangleh = m_vectopleft.y - m_vevbottomright.y;

        float fHalfCirlce = Mathf.PI * rectangleh / 2;
        float fLength = rectanglew + rectanglew + 2 * fHalfCirlce;


        float fmindis = Mathf.Min(fLength - m_fcurpos, m_fcurpos);
        nIndex=0;
        for (int i = 1; i < m_Inside.Count;i++ )
        {

            if (Mathf.Abs(m_fcurpos - m_Inside[i].fstartpos) < fmindis)//更靠近
            {
                nIndex=i;
                fmindis = Mathf.Abs(m_fcurpos - m_Inside[i].fstartpos);
            }
        }
            //float fDis = m_fcurpos - (int)(m_fcurpos / fLength) * fLength;

            //for (nIndex = m_Inside.Count - 1; nIndex >= 0; nIndex--)
            //{
            //    if (m_fcurpos >= m_Inside[nIndex].fstartpos)
            //    {
            //        break;
            //    }
            //}
        if (m_fcurpos < rectanglew)//上
        {
            vec = m_Inside[0].pt;
            vec.x += m_fcurpos;
            fAngle = 0;
        }
        else if (m_fcurpos < rectanglew + fHalfCirlce)//右
        {
            float farclength = m_fcurpos - rectanglew;
            float fRadians = farclength / fHalfCirlce * Mathf.PI;
            Vector2 ptCircle = m_vevbottomright;
            ptCircle.y += rectangleh / 2;
            vec.x = ptCircle.x + rectangleh / 2 * Mathf.Sin(fRadians);
            vec.y = ptCircle.y + rectangleh / 2 * Mathf.Cos(fRadians);
            fAngle = -fRadians * 180 / Mathf.PI;
        }
        else if (m_fcurpos < rectanglew + fHalfCirlce + rectanglew)//下
        {
            vec = m_Inside[nHCount + nVCount].pt;
            vec.x -= (m_fcurpos - rectanglew - fHalfCirlce);
            fAngle = 180;
        }
        else//左
        {
            float farclength = m_fcurpos - rectanglew - fHalfCirlce - rectanglew;
            float fRadians = farclength / fHalfCirlce * Mathf.PI;
            Vector2 ptCircle = m_vectopleft;
            ptCircle.y -= rectangleh / 2;
            vec.x = ptCircle.x - rectangleh / 2 * Mathf.Sin(fRadians);
            vec.y = ptCircle.y - rectangleh / 2 * Mathf.Cos(fRadians);
            fAngle = -180 - fRadians * 180 / Mathf.PI;
        }
    }
    public void Run(byte byWinpos)
    {
        float rectanglew2 = m_vevbottomright.x - m_vectopleft.x;
        float rectangleh2 = m_vectopleft.y - m_vevbottomright.y;

        m_bspeedup = true;
        m_facceleration = 10;
        m_fspeed = 0;
        m_fstopdis = 3 * (2 * rectanglew2 + 2 * Mathf.PI * rectangleh2 / 2);//2圈 
        m_fstopdis += (m_Inside[byWinpos].fstartpos - m_fcurpos);
        
    }

    public bool Update(float ftime)
    {
         float rectanglew = m_vevbottomright.x - m_vectopleft.x;
        float rectangleh = m_vectopleft.y - m_vevbottomright.y;

        float fHalfCirlce = Mathf.PI * rectangleh / 2;
        float fLength = rectanglew + rectanglew + 2 * fHalfCirlce;
       // float fDis = m_fcurpos - (int)(m_fcurpos / fLength) * fLength;

        if (m_bspeedup)
        {
            float fspeedold = m_fspeed;
            m_fspeed += m_facceleration * ftime;
            float fdis = (Mathf.Pow(m_fspeed, 2) - Mathf.Pow(fspeedold, 2)) / (2 * m_facceleration);
            m_fcurpos += fdis;
            m_fstopdis -= fdis;
            if (m_fspeed >= 5)
            {
                m_bspeedup = false;
                m_bSustain = true;
            }
        }
        else if (m_bSustain)
        {
            //float rectanglew = m_vevbottomright.x - m_vectopleft.x;
            //float rectangleh = m_vectopleft.y - m_vevbottomright.y;

            float fdis = m_fspeed * ftime;
            m_fcurpos += fdis;
            m_fstopdis -= fdis;
            if (m_fstopdis <= (rectanglew + Mathf.PI * rectangleh / 2))
            {
                m_bSustain = false;
                m_bspeeddown = true;
                m_facceleration = -Mathf.Pow(m_fspeed, 2) / (2 * m_fstopdis);
            }
        }
        else if (m_bspeeddown)
        {
            float fspeedold = m_fspeed;
            m_fspeed += m_facceleration * ftime;
            if (m_fspeed < 0)
            {
                m_fspeed = 0;
                m_bspeeddown = false;
            }
            float fdis = (Mathf.Pow(m_fspeed, 2) - Mathf.Pow(fspeedold, 2)) / (2 * m_facceleration);
            m_fcurpos += fdis;
            m_fstopdis -= fdis;
        }
        if (m_fcurpos > fLength)
        {
            m_fcurpos -= fLength;
        }

        return m_bspeedup || m_bSustain || m_bspeeddown;
    }
    public bool IsMoving()
    {
        return m_bspeedup || m_bSustain || m_bspeeddown;
    }

    public void EnterPos(byte byIndex)
    {
        m_fcurpos=m_Inside[byIndex].fstartpos;       
    }
}
public class GameCarUI
{
    uint[] nJetton = new uint[] { 1000, 10000, 100000, 500000, 1000000 };
    byte[] goldArr = new byte[] { 1, 2, 5, 7, 10 };
    Object m_objcar = null;
    Object m_objhosteff = null;
    CarMove m_carmove = new CarMove();
    AreaInfo[] m_areainfo = new AreaInfo[8];
    GameObject m_goBase = null;
    GameObject m_gohosteff = null;

    GameObject m_goReturn;
    GameObject m_gocar;
    UISprite m_sprflash;
    GameObject m_goapplist;//申请列表
   

    //自己
    UILabel m_labelmygold;
    UILabel m_labelnmynickname;
    UITexture m_texmyavatar;
    GameObject m_goGoldIcon;

    //tip
    GameObject[] m_gotip=new GameObject[2];
    UILabel m_labeltip;
    float m_ftiptime;

    int m_betbuttonIndex;
    byte m_byareasIndex;
    GameObject[] m_gobetbutton = new GameObject[5];
    GameObject m_gobetselect;


    uint[] m_nTotalJetton = new uint[8];
    uint[] m_nMyJetton = new uint[8];
    CarTableStates m_tablestatus;
    
    //庄家
    GameObject[] hostBtnObj = new GameObject[2];
    GameObject hostHeadObj;
    UILabel hostNameLabel;
    UILabel hostGoldNumLabel;
    GameObject m_goSystem;
    bool isMasterHost;
    bool isInLine;
    bool isHost;
    //申请列表
    UILabel hostNameLineLabel;
    UILabel hostGameRestLabel;
    UILabel myLineNumLabel;
    GameObject[] hostBtn = new GameObject[4];
    uint hostGoldSum = 0;
    byte hostGameSum = 0;
    GameObject hostItem;
    UIGrid hostListGrid;
   
    //
    GameObject m_goplayerlist; //大众席
    UILabel m_labelPlayerCount;
    UILabel m_labelListPage;
    UIGrid m_gridPlayerlist;
    GameObject m_goPlayerListItem;
    GameObject m_gobtnPlayerList;
     
    byte m_byPlayerListPage = 0;
    byte m_byPlayerListTotal = 0;
    //
    //List<MoveCoin>[] m_listcoin = new List<MoveCoin>[8];
    //GameObject m_goMoveCoin;
    float m_fflahtime = 0;
    //结果
    byte m_bywinpos;
    GameObject m_goResult;
    UILabel m_labelresult;
    GameObject m_goresultchar;
    bool m_bshowresult=false;
    //历史记录
    GameObject m_goHistory;

    CarBankerUserInfo m_BankerInfocache;
    CarGameLog m_nHistorcache;



    public bool IsAcitve()
    {
        return m_goBase != null;
    }
    public void ShowWnd()
    {
        if (m_objcar == null)
        {
            m_objcar = ResManager.Instance.LoadObject("Car", "GameRes/Car/Prefab/UI/", ResType.GameRes);
            m_objhosteff = ResManager.Instance.LoadObject("Niuniu_Host", "GameRes/Niuniu/Prefab/Effect/", ResType.GameRes);
        }


        Transform tfTemp = null;
        m_goBase = (GameObject)GameObject.Instantiate(m_objcar);
        m_goBase.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        //庄标志
        m_gohosteff = (GameObject)GameObject.Instantiate(m_objhosteff);
        m_gohosteff.transform.SetParent(m_goBase.transform, false);
        tfTemp = m_gohosteff.transform.GetChild(0).GetChild(0);
        tfTemp.localPosition = new Vector3(-180, -160, 0);
        UIEventListener.Get(tfTemp.gameObject).onClick = EventOpenApplyList;
        GlobalEffectMgr.SetTrailEffectOnTopWnd(m_gohosteff);
        GlobalEffectMgr.SetMeshRendererOnTopWnd(m_gohosteff);


        for (int i = 0; i < 8; i++)
        {
            m_areainfo[i] = new AreaInfo();
            m_areainfo[i].gobutton = m_goBase.transform.GetChild(0).GetChild(i).gameObject;
            m_areainfo[i].labeltotal = m_areainfo[i].gobutton.transform.GetChild(0).GetComponent<UILabel>();
            m_areainfo[i].labelmy = m_areainfo[i].gobutton.transform.GetChild(1).GetComponent<UILabel>();
            m_areainfo[i].goCeter = m_areainfo[i].gobutton.transform.GetChild(2).gameObject;
            UIEventListener.Get(m_areainfo[i].gobutton.gameObject).onClick = EventBet;

        }
        UIEventListener.Get(m_goBase.transform.GetChild(1).gameObject).onClick = EventReturn;


        m_gobtnPlayerList=m_goBase.transform.GetChild(2).gameObject;
        UIEventListener.Get(m_gobtnPlayerList).onClick = EventPlayerList;


        UIEventListener.Get(m_goBase.transform.GetChild(3).gameObject).onClick = EventHistory;

        //自己的信息
        tfTemp = m_goBase.transform.GetChild(4);
        for (int i = 0; i < 5; i++)
        {
            m_gobetbutton[i] = tfTemp.GetChild(i).gameObject;
            UIEventListener.Get(m_gobetbutton[i]).onClick = EventBetButton;
        }
        m_gobetselect = tfTemp.GetChild(5).gameObject;
        m_gobetselect.SetActive(false);
        m_betbuttonIndex = -1;

        m_texmyavatar = tfTemp.GetChild(6).GetComponent<UITexture>();
        m_labelnmynickname = tfTemp.GetChild(7).GetComponent<UILabel>();
        m_goGoldIcon = tfTemp.GetChild(8).gameObject;
        m_labelmygold = tfTemp.GetChild(9).GetComponent<UILabel>();
        UIEventListener.Get(tfTemp.GetChild(11).gameObject).onClick = EventBuy;


 
        //大众席
        m_goplayerlist = m_goBase.transform.GetChild(5).gameObject;
        UIEventListener.Get(m_goplayerlist.transform.GetChild(6).gameObject).onClick=EventPlayerListLeft;
        UIEventListener.Get(m_goplayerlist.transform.GetChild(7).gameObject).onClick = EventPlayerListRight;

        m_labelPlayerCount = m_goplayerlist.transform.GetChild(5).GetChild(0).GetComponent<UILabel>();
        m_labelListPage = m_goplayerlist.transform.GetChild(8).GetComponent<UILabel>();
        
        Transform tflist = m_goplayerlist.transform.GetChild(9);
        Transform tfitem = tflist.GetChild(0);
        UIGrid grid = tflist.GetChild(1).GetComponent<UIGrid>();
        m_gridPlayerlist = grid;
        m_goPlayerListItem = tfitem.gameObject;
        UIEventListener.Get(m_goplayerlist.transform.GetChild(4).gameObject).onClick = EventClosePlayerList;


        //汽车一圈位置
        Transform tflogo = m_goBase.transform.GetChild(6);
        Vector3 v1 = tflogo.GetChild(0).GetChild(0).position;
        Vector3 v2 = tflogo.GetChild(0).GetChild(1).position;
        Vector3 v3 = tflogo.GetChild(0).GetChild(2).position;
        m_carmove.Init(v2.x, v2.y, v3.x, v3.y, (v1.y - v2.y));
        for (int i = 0; i < 32; i++)
        {

            tflogo.GetChild(1).GetChild(i).GetComponent<UISprite>().spriteName = (FishConfig.Instance.m_MiNiGameConfig.carConfig.CarValueVec[i].TypeID ).ToString();
            tflogo.GetChild(1).GetChild(i).position = new Vector3(m_carmove.m_outside[i].pt.x, m_carmove.m_outside[i].pt.y, 0);
            // tflogo.GetChild(2).GetChild(i).position = new Vector3(m_carmove.m_Inside[i].pt.x, m_carmove.m_Inside[i].pt.y, 0);
            //tflogo.GetChild(1).GetChild(i).position = v3;// new Vector3(m_carmove.m_outside[i].pt.x, m_carmove.m_outside[i].pt.y, 0);
        }
        //庄家
        tfTemp = m_goBase.transform.GetChild(9);

        hostHeadObj = tfTemp.GetChild(0).gameObject;
        hostNameLabel = tfTemp.GetChild(1).GetComponent<UILabel>();
        hostGoldNumLabel = tfTemp.GetChild(1).GetChild(0).GetComponent<UILabel>();
        m_goSystem = tfTemp.GetChild(2).gameObject;

        hostBtnObj = new GameObject[2];
        hostBtnObj[0] = tfTemp.GetChild(3).gameObject;
        hostBtnObj[1] = tfTemp.GetChild(4).gameObject;

        UIEventListener.Get(hostBtnObj[0]).onClick = EventApply;
        UIEventListener.Get(hostBtnObj[1]).onClick = EventGiveup;
       

        //申请列表
        m_goapplist = m_goBase.transform.GetChild(10).gameObject;
        hostNameLineLabel = m_goapplist.transform.GetChild(4).GetComponent<UILabel>();
        hostGameRestLabel = m_goapplist.transform.GetChild(6).GetComponent<UILabel>();
        myLineNumLabel = m_goapplist.transform.GetChild(8).GetComponent<UILabel>();
        hostBtn[0]=m_goapplist.transform.GetChild(11).gameObject; //抢
        hostBtn[1]=m_goapplist.transform.GetChild(10).gameObject; //取消
        hostBtn[2]=m_goapplist.transform.GetChild(12).gameObject;//下庄
        hostBtn[3] = m_goapplist.transform.GetChild(9).gameObject;//上庄

        UIEventListener.Get(hostBtn[0]).onClick=EventScramble;
        UIEventListener.Get(hostBtn[1]).onClick=EventCancel;
        UIEventListener.Get(hostBtn[2]).onClick=EventGiveup;
        UIEventListener.Get(hostBtn[3]).onClick=EventApply;


         tflist = m_goapplist.transform.GetChild(13);//
         tfitem = tflist.GetChild(0);
         grid = tflist.GetChild(1).GetComponent<UIGrid>();
         hostItem = tfitem.gameObject;
         hostListGrid = grid;       
        UIEventListener.Get(m_goapplist.transform.GetChild(2).gameObject).onClick = EventCloseApplyList;
        
        //tip
        tfTemp = m_goBase.transform.GetChild(11);
        m_gotip[0] = tfTemp.GetChild(0).gameObject;
        m_gotip[1] = tfTemp.GetChild(1).gameObject;      
        //跑车
        m_gocar = m_goBase.transform.GetChild(7).gameObject;
        m_sprflash = m_goBase.gameObject.transform.GetChild(8).GetComponent<UISprite>();

        m_goResult = m_goBase.transform.GetChild(13).gameObject;
        m_labelresult = m_goResult.transform.GetChild(5).GetComponent<UILabel>();
        m_goresultchar = m_goResult.transform.GetChild(5).GetChild(0).gameObject;
        UIEventListener.Get(m_goResult.transform.GetChild(3).gameObject).onClick = EventCloseResult;

        m_goHistory = m_goBase.transform.GetChild(14).gameObject;
        //UIEventListener
       // m_goMoveCoin = m_goBase.transform.GetChild(12).gameObject;

        {//重置
            isMasterHost = false;
            isInLine = false;
            isHost = false;
        }
        WndManager.Instance.Push(m_goBase);


    }
    public void ShutDown()
    {
        if (m_goBase != null)
        {
            WndManager.Instance.Pop(m_goBase);
            GameObject.Destroy(m_goBase);
            m_goBase = null;
            GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound2);
            GlobalHallUIMgr.Instance.GameShare.LveaveGame();
        }
    }
    public void Update(float deltaTime)
    {
        if(!IsAcitve())
        {
            return;
        }
        //for (int i = 0; i < m_listcoin.Length; i++)
        //{
        //    for (int j = m_listcoin[i].Count-1; j>=0 ; j--)
        //    {
        //        if(m_listcoin[i][j].Update(deltaTime))
        //        {
        //            m_listcoin[i][j].Destroy();
        //            m_listcoin[i].RemoveAt(j);
        //        }
                
        //    }
        //}


        float fOldTime = m_ftiptime;
        m_ftiptime -= deltaTime;
        if (CanBet()&&(int)fOldTime - (int)m_ftiptime == 1)
        {
            if ((int)m_ftiptime == 0)
            {
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_cdover);
            }
            else if ((int)m_ftiptime <= 5)
            {
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_cd);
            }
        }

        if (m_labeltip && m_ftiptime>=0)
        {
            m_labeltip.gameObject.transform.parent.gameObject.SetActive(!m_carmove.IsMoving());
            //m_labeltip.parent.gameObject.SetActive(!m_carmove.IsMoving());
            m_labeltip.text = ((int)m_ftiptime).ToString();
        }
                   
       // if 
        m_carmove.Update(deltaTime);

        Vector2 veccar = Vector2.zero;
        int nIndex = 0;
        float fAngle = 0;
        m_carmove.CarPos(ref veccar, ref nIndex, ref fAngle);
        m_gocar.transform.position = new Vector3(veccar.x, veccar.y, 0);
        m_gocar.transform.localRotation = Quaternion.AngleAxis(fAngle, Vector3.forward);
                 
        if(m_sprflash)
        {
            if(CanBet())
            {
                m_fflahtime -= deltaTime;
                if (m_fflahtime < 0)
                {
                    m_sprflash.gameObject.SetActive(true);
                    m_fflahtime = 0.3f;                   
                    byte bypos=(byte)Random.Range(0, FishConfig.Instance.m_MiNiGameConfig.carConfig.CarValueVec.Count-1);
                    m_sprflash.spriteName = "s" + (FishConfig.Instance.m_MiNiGameConfig.carConfig.CarValueVec[bypos].TypeID).ToString();
                    m_sprflash.gameObject.transform.position = new Vector3(m_carmove.m_outside[bypos].pt.x, m_carmove.m_outside[bypos].pt.y, 0);
                }
            }
            else if (m_carmove.IsMoving())
            {
                m_sprflash.gameObject.SetActive(true);
                m_sprflash.spriteName = "s" + (FishConfig.Instance.m_MiNiGameConfig.carConfig.CarValueVec[nIndex].TypeID).ToString();
                m_sprflash.gameObject.transform.position = new Vector3(m_carmove.m_outside[nIndex].pt.x, m_carmove.m_outside[nIndex].pt.y, 0);
              
            }
            else
            {
                m_fflahtime -= deltaTime;
                if (m_fflahtime < 0)
                {
                    m_fflahtime = 0.5f;
                    m_sprflash.gameObject.SetActive(!m_sprflash.gameObject.activeSelf);                 
                }
                m_sprflash.spriteName = "s" + (FishConfig.Instance.m_MiNiGameConfig.carConfig.CarValueVec[nIndex].TypeID).ToString();
                m_sprflash.gameObject.transform.position = new Vector3(m_carmove.m_outside[nIndex].pt.x, m_carmove.m_outside[nIndex].pt.y, 0);
            }                        
        }
        if(!(CanBet())&&!m_carmove.IsMoving()&&m_bshowresult)
        {
            if (m_BankerInfocache!=null)
            {               
                UpdateCarHost(m_BankerInfocache);
                m_BankerInfocache = null;
            }
            if (m_nHistorcache!=null)
            {               
                SetHistory(m_nHistorcache);
                m_nHistorcache = null;
            }
            SetCarPlayerGold();
            m_bshowresult = false;
            m_goResult.SetActive(true);
            WndManager.Instance.Push(m_goResult);
        }
    }
    void ResetData()
    {
        for (int i = 0; i < m_nTotalJetton.Length; i++)
        {
            m_nTotalJetton[i] = 0;
        }

        for (int i = 0; i < m_nMyJetton.Length; i++)
        {
            m_nMyJetton[i] = 0;
        }
        m_bshowresult = false;
        m_nHistorcache = null;
        m_BankerInfocache = null;
    }
    void EventOpenApplyList(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_goapplist.SetActive(true);
        WndManager.Instance.Push(m_goapplist);

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
    void EventCloseApplyList(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        m_goapplist.SetActive(false);
        WndManager.Instance.Pop(m_goapplist);
    }
    void EventCloseResult(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        m_goResult.SetActive(false);
        WndManager.Instance.Pop(m_goResult);
    }
    void EventBuy(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Gold);
    }
    void EventApply(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        //判断是否满足上庄条件
        uint globalSum = FishConfig.Instance.m_MiNiGameConfig.carConfig.JoinBankerGlobelSum;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < globalSum)
        {
            string str = StringTable.GetString("UOM_Niuniu_UpHostFail");
            string newStr = str.Replace("@", globalSum.ToString());
            GlobalEffectMgr.Instance.ShowMessageBox(newStr, MssageType.NiuNiu_UpHostFail_Tips);
        }
        if (PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnRoleJoinWriteBankerList())
        {
            isInLine = true;
            if (go == hostBtnObj[0])
                EventOpenApplyList(null);
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
    void EventCancel(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnRoleLeaveWriteBankerList())
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
    void EventScramble(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        uint needGlobal = FishConfig.Instance.m_MiNiGameConfig.carConfig.GetNextBankerNeedGlobel;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() > needGlobal)
        {
            string str = StringTable.GetString("UOM_Niuniu_SnatchHost");
            string newStr = str.Replace("@", needGlobal.ToString());
            GlobalEffectMgr.Instance.ShowMessageBox(newStr, MssageType.Car_SantchHost_Tips);
        }
    }
    void EventGiveup(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        if (PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnRoleCanelBanker())
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
    void EventBetButton(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        for (int i = 0; i < 5; i++)
        {
            if (go == m_gobetbutton[i])
            {
                m_betbuttonIndex = i;
                m_gobetselect.SetActive(true);
                m_gobetselect.transform.position = m_gobetbutton[i].gameObject.transform.position;
                break;
            }
        }
    }
    void EventBet(GameObject go)
    {
        if (CanBet())
        {
            if (m_betbuttonIndex == -1)
            {
                return;
            }
            for (byte i = 0; i < m_areainfo.Length; i++)
            {
                if (go == m_areainfo[i].gobutton)
                {                   
                    m_byareasIndex = i;
                    PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnAddGlobel(nJetton[m_betbuttonIndex], (byte)(i));
                    break;
                  
                }
            }
        }
    }
    void EventReturn(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        uint myJetton = 0;
        for (int i = 0; i < m_nMyJetton.Length;i++ )
        {
            myJetton += m_nMyJetton[i];
        }

        if (myJetton > 0 && CanBet())
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_LeaveRoom"), MssageType.Car_Exit_Tips);
        }
        else if (isInLine && !isHost)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_LineLeave"), MssageType.Car_Exit_Tips);
        }
        else if (isHost && !isInLine)
        {
            GlobalEffectMgr.Instance.ShowMessageBox(StringTable.GetString("UOM_Niuniu_HostLeave"), MssageType.Car_Exit_Tips);
        }
        else
        {
            PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnLeaveCarTable();
            ShutDown();
        }        
    }
    void EventPlayerList(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        m_goplayerlist.SetActive(true);
        WndManager.Instance.Push(m_goplayerlist);
        PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnGetNormalInfo(0);
        m_byPlayerListPage = 0;
        m_byPlayerListTotal = 0;
    }
    void EventClosePlayerList(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        m_goplayerlist.SetActive(false);
        WndManager.Instance.Pop(m_goplayerlist);
    }
    void EventPlayerListLeft(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (m_byPlayerListPage == 0)
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("UOM_Niuniu_FrontPage_Fail"), 1.0f, false);
        else
        {
            m_byPlayerListPage--;
            PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnGetNormalInfo(m_byPlayerListPage);
        }
    }
    void EventPlayerListRight(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        if (m_byPlayerListPage + 1 < m_byPlayerListTotal)
        {
            m_byPlayerListPage++;
            PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnGetNormalInfo(m_byPlayerListPage);
        }
        else
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("UOM_Niuniu_NextPage_Fail"), 1.0f, false);
       
    }
    void EventHistory(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_goHistory.SetActive(!m_goHistory.activeSelf);
        //ShutDown();
    }
    public void EnterGameCar(byte TableUpdateSec,
        CarTableStates TableStates,
        CarTableUpdateInfo TableGlobelInfo,
        CarBankerUserInfo TableBankerInfo,
        CarWriteBankerList TableWriteBankerList,
        CarVipSeatList TableVipSeatInfo,
        CarGameLog TableGameLog,
        byte TableResult)
    {
        ResetData();

        m_tablestatus = TableStates;
        m_ftiptime = TableUpdateSec;
        m_bywinpos = TableResult;
        m_carmove.EnterPos(m_bywinpos);

        UpdateCarHost(TableBankerInfo);
        SetCarPlayerGold();
        SetPlayerName();
        SetTip();
       
        UpateTotal(TableGlobelInfo.GlobelNum);
        SetHistory(TableGameLog);
        PlayBgMusic();
   
    }
    public void CarSnatchHost()
    {
        PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnRoleGetWriteBankerFirstSeat();
    }
    public void CarStatusChange(CarTableStates TableStates,
        CarTableUpdateInfo TableGlobelInfo,
        CarVipSeatList OldVipSeatInfo,
        CarVipSeatList TableVipSeatInfo,
        CarWriteBankerList TableWriteBankerList,
        CarBankerUserInfo TableBankerUserInfo,
        CarGameLog TableGameLog,
        byte TableResult)
    {
        m_tablestatus = TableStates;
        SetTip();
              
        if (CanBet())
        {
            ResetData();
            SetDeskGold();           
            if(m_goResult.activeSelf)
            {
                EventCloseResult(null);
            }
            m_ftiptime = FishConfig.Instance.m_MiNiGameConfig.carConfig.BeginWriteSec;
        }
        else
        {
          
            m_bshowresult = true; 
            m_bywinpos = TableResult;          
            m_ftiptime = FishConfig.Instance.m_MiNiGameConfig.carConfig.EndWriteSec;
            m_carmove.Run(m_bywinpos);
            UpateTotal(TableGlobelInfo.GlobelNum);            
             SetResult();
             SetHistory(TableGameLog);           
             //Debug.Log(m_bywinpos);           
        }
        UpdateCarHost(TableBankerUserInfo);
        PlayBgMusic();
    }
    public void SetCarAvetarSelf(Texture2D tex)
    {
        if(m_texmyavatar)
        {
            m_texmyavatar.mainTexture = tex;
        }
    }
    public void SetCarAvetarHost(Texture2D tex)
    {
        hostHeadObj.transform.GetChild(0).GetComponent <UITexture>().mainTexture= tex;
    }
    public void SetCarAvetarPlayList(Texture2D tex, uint id, byte data)
    {
        Transform item = m_gridPlayerlist.GetChild(data);
        if (item)
        {
            item.GetChild(0).GetComponent<UITexture>().mainTexture = tex;
        } 
    }
    public void SetCarBetBtn()
    {
        byte maxRate = 0;
        foreach (tagCarType item in FishConfig.Instance.m_MiNiGameConfig.carConfig.CarTypeMap.Values)
        {
            if (item.RateValue > maxRate)
            {
                maxRate = item.RateValue;
            }
        }


        long maxAddGlobel = FishConfig.Instance.m_MiNiGameConfig.carConfig.MaxAddGlobel;

        uint myTotalWeager =0;
        for( int i=0;i<m_nMyJetton.Length;i++)
        {
            myTotalWeager+=m_nMyJetton[i];
        }
        uint myGold = PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel();
        if (isMasterHost)
        {
            for (int i = nJetton.Length - 1; i >= 0; i--)
            {
                //if (myTotalWeager + nJetton[i] > maxAddGlobel || (myTotalWeager + myGold) < (nJetton[i] + myTotalWeager) * maxRate    || nJetton[i] > myGold)
                if (myTotalWeager + nJetton[i] > maxAddGlobel|| nJetton[i] > myGold)
                {
                    if (i == 0)
                    {
                        m_gobetselect.SetActive(false);
                    }
                    m_gobetbutton[i].GetComponent<UIButton>().isEnabled = false;
                    if (i == m_betbuttonIndex)
                    {
                        if (i > 0)
                            EventBetButton(m_gobetbutton[i - 1]);
                    }
                }
                else
                {
                    m_gobetselect.SetActive(true);
                    m_gobetbutton[i].GetComponent<UIButton>().isEnabled = true;
                }
            }
        }
        else
        {
            uint weagerSum = 0;
           
            for (byte i = 0; i < m_nTotalJetton.Length; i++)
            {
                weagerSum += m_nTotalJetton[i];       
            }
           
            for (int i = nJetton.Length - 1; i >= 0; i--)
            { 
                //(weagerSum + nJetton[i]) * maxRate > (hostGoldSum)
                bool bcanbet=false;
                for(int j=0;j<m_nTotalJetton.Length;j++)
                {
                    if ((m_nTotalJetton[j] + nJetton[i]) * maxRate < hostGoldSum + weagerSum - m_nTotalJetton[j])//至少一个位置能下
                    {
                        bcanbet=true;
                        break;
                    }
                }

                if (!bcanbet || myTotalWeager + nJetton[i] > maxAddGlobel || nJetton[i] > myGold)
                {
                    if (i == 0)
                    {
                        m_gobetselect.SetActive(false);
                    }
                    m_gobetbutton[i].GetComponent<UIButton>().isEnabled = false;
                    if (i == m_betbuttonIndex)
                    {
                        if (i > 0)
                            EventBetButton(m_gobetbutton[i - 1]);
                    }
                }
                else
                {
                    m_gobetselect.SetActive(true);
                    m_gobetbutton[i].GetComponent<UIButton>().isEnabled = true;
                }
            }
        }
    }

    void SetResult()
    {
        byte byWinType = FishConfig.Instance.m_MiNiGameConfig.carConfig.CarValueVec[m_bywinpos].TypeID;
        byte byWinMul = FishConfig.Instance.m_MiNiGameConfig.carConfig.CarTypeMap[byWinType].RateValue;
        int nNum = 0;
        if (isHost)
        {
            for (int i = 0; i < m_nTotalJetton.Length; i++)
            {
                //if (i != byWinType)
                {
                    nNum += (int)m_nTotalJetton[i];
                }
                if (i == byWinType)      
                {
                    nNum -= (int)m_nTotalJetton[i] * byWinMul;
                }
            }
        }
        else
        {
            for (int i = 0; i < m_nMyJetton.Length; i++)
            {
                //if (i != byWinType)
                {
                    nNum -= (int)m_nMyJetton[i];
                }
                if (i == byWinType)             
                {
                    nNum += (int)m_nMyJetton[i] * byWinMul;
                }
            }
        }
       
        m_labelresult.text = FormatNum(nNum);
        m_goresultchar.SetActive(Mathf.Abs(nNum) >= 10000);
        GlobalHallUIMgr.Instance.GameShare.AddGlod(nNum);
    }
    void SetHistory(CarGameLog nHistory)
    {
        if(m_carmove.IsMoving())
        {
            m_nHistorcache = nHistory;
            return;
        }
        else
        {
            m_nHistorcache=null;
        }
        int nCurrent=nHistory.GameSum-1;
        for (int i = 7; i >= 0; i--)
        {
            if (nCurrent >= 0)
            {
                byte byType = (byte)((nHistory.GameLog >> nCurrent * 4) & 0x0f);
                m_goHistory.transform.GetChild(i).GetChild(0).gameObject.SetActive(byType % 2 == 0);
                m_goHistory.transform.GetChild(i).GetChild(0).GetComponent<UISprite>().spriteName = (byType).ToString();
                m_goHistory.transform.GetChild(i).GetChild(1).gameObject.SetActive(byType % 2 != 0);
                m_goHistory.transform.GetChild(i).GetChild(1).GetComponent<UISprite>().spriteName = (byType).ToString();
                nCurrent--;
            }
            else
            {
                m_goHistory.transform.GetChild(i).GetChild(0).gameObject.SetActive(false);
                m_goHistory.transform.GetChild(i).GetChild(1).gameObject.SetActive(false);
            }
        }
    }
    //public void  Bet(Vector3 from,byte byarea)
    //{
    //    GameObject goItem = GameObject.Instantiate(m_goMoveCoin);
    //    goItem.transform.SetParent(m_goBase.transform, false);
    //    goItem.SetActive(true);

    //    Vector3 vecto = m_areainfo[byarea].goCeter.transform.position;
    //    const float radius = 0.1f;
    //    vecto.x += Random.Range(-radius, radius);
    //    vecto.y += Random.Range(-radius, radius);

    //    MoveCoin coin = new MoveCoin(goItem, from, vecto, 0.2f);
    //    m_listcoin[m_byareasIndex].Add(coin);
    //    if (m_listcoin[byarea].Count > 20)//最多20个
    //    {
    //        m_listcoin[byarea][0].Destroy();
    //        m_listcoin[byarea].RemoveAt(0);
    //    }
    //}
    void UpateTotal(System.UInt64[] GlobelNum)
    {
        for (int i = 0; i < m_nTotalJetton.Length; i++)
        {
            m_nTotalJetton[i] = (uint)GlobelNum[i];
        }
        SetDeskGold();
    }
    void SetDeskGold()
    {
        for (byte i = 0; i < m_areainfo.Length; i++)
        {
            m_areainfo[i].labelmy.gameObject.SetActive(m_nMyJetton[i] != 0);
            m_areainfo[i].labelmy.text = FormatNum((int)m_nMyJetton[i]);

            m_areainfo[i].labeltotal.gameObject.SetActive(m_nTotalJetton[i] != 0);
            m_areainfo[i].labeltotal.text = FormatNum((int)m_nTotalJetton[i]);
        }
    }
    public void UpdateCarMyWeagerNum()
    {
        m_nMyJetton[m_byareasIndex]+=nJetton[m_betbuttonIndex];
        m_nTotalJetton[m_byareasIndex] += nJetton[m_betbuttonIndex];
        SetDeskGold();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu15);
        //够造金币
        //for(byte i=0;i<goldArr[m_betbuttonIndex];i++)
        //{
        //    Bet(m_goGoldIcon.transform.position,m_byareasIndex);
        //}
    }
    public void UpdateCarDate(ulong[] date)
    {
        UpateTotal(date);
        //for(byte i=0;i<m_areainfo.Length;i++)
        //{
        //    m_areainfo[i].labeltotal.text = FormatNum((int)date[i]);
        //    int ngap = (int)(date[i] - m_nTotalJetton[i]);
        //    for(int j=nJetton.Length-1;j>=0;j--)
        //    {
        //        if(ngap>=nJetton[j])//大众席
        //        {
        //           // Bet(m_gobtnPlayerList.transform.position,i);
        //            break;
        //        }
        //    }           
        //}       
    }
    public void UpdateCarHost(CarBankerUserInfo info)
    {
        if(m_carmove.IsMoving())
        {
            m_BankerInfocache = info;
            return;
        }
        else
        {
            m_BankerInfocache = null;
        }
        if (info.UserInfo.dwUserID == 0)
        {
            hostHeadObj.SetActive(false);
            hostNameLabel.gameObject.SetActive(false);
            m_goSystem.SetActive(true);
                    
            hostNameLineLabel.text = "解闷大师";
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
            m_goSystem.SetActive(false);
            isMasterHost = false;
            hostGoldSum = info.UserInfo.GlobelSum;
           
            hostGameSum = info.GameSum;
            hostNameLabel.text = info.UserInfo.NickName;
            uint goldNum = info.UserInfo.GlobelSum;
            hostGoldNumLabel.text = FormatNum((int)goldNum); 
            //goldNum.ToString();
            //if (goldNum > 10000)
            //{
            //    hostGoldNumLabel.text = g(goldNum / 10000) + "." + (goldNum / 1000 % 10) + "万";
            //}

            hostNameLineLabel.text = info.UserInfo.NickName;
            hostGameRestLabel.text = (FishConfig.Instance.m_MiNiGameConfig.carConfig.BankerGameSum - hostGameSum).ToString();

            if (info.UserInfo.NickName.Length > 4)
                hostNameLabel.text = info.UserInfo.NickName.Substring(0, 4) + "...";

            uint ficeID = info.UserInfo.dwFaceID;
            if (ficeID < ConstValue.CUSTOM_HEADER)
            {
                hostHeadObj.transform.GetChild(0).GetComponent<UITexture>().mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
            }
            else
            {
                //获取头像
                HeaderManager.Instance.GetPlayerHeader(info.UserInfo.dwUserID, ficeID, HeaderOptType.HEADER_CAR_HOST, null);
            }

            if (info.UserInfo.NickName.Equals(PlayerRole.Instance.RoleInfo.RoleMe.GetNickName()))
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
    public void GetCarHostline(CarWriteBankerList list)
    {
        ClearGrid(hostListGrid);
        for (byte i = 0; i < FishDataInfo.MAX_NIUNIU_BankerShowSum; i++)
        {
            if (list.List[i].dwUserID == 0)
            {
                break;
            }
            CarRoleInfo info = list.List[i];
            Transform itemTrans = CloneApplyListItem(hostItem.transform, (byte)(i + 1), info.NickName, (int)info.GlobelSum);
            hostListGrid.AddChild(itemTrans);
            itemTrans.localScale = Vector3.one;

          
          //  HostItem item = new HostItem((uint)i + 1, info.NickName, info.GlobelSum);
           // Transform trans = item.InitItem(hostItem.transform);
           // hostListGrid.AddChild(trans);
            //trans.localScale = Vector3.one;

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
    
    public void CarGetNormalPeople(uint page, uint sum, CarRoleInfo[] array)
    {
        ClearGrid(m_gridPlayerlist);                    
        if (sum % 9 == 0)
            m_byPlayerListTotal = (byte)(sum / 9);
        else
            m_byPlayerListTotal = (byte)(sum / 9 + 1);
        string str = StringTable.GetString("UOM_Niuniu_Normal_Sum");
        string newStr = str.Replace("@", sum.ToString());
        m_labelPlayerCount.text = newStr;
        m_labelListPage.text = (m_byPlayerListPage + 1) + "/" + m_byPlayerListTotal;    

        for (byte i = 0; i < array.Length; i++)
        {
            if (array[i].dwUserID == 0)
            {
                break;
            }
            Transform itemTrans = ClonePlayerListItem(m_goPlayerListItem.transform, array[i].NickName, (int)array[i].GlobelSum);
            m_gridPlayerlist.AddChild(itemTrans);
            itemTrans.localScale = Vector3.one;

            uint ficeID = array[i].dwFaceID;
            if (ficeID < ConstValue.CUSTOM_HEADER)
            {
                //item.headTex = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
                itemTrans.GetChild(0).GetComponent<UITexture>().mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
               // itemTrans.headObj.GetComponent<UITexture>().mainTexture = item.headTex;
            }
            else
            {
                //获取头像
                HeaderManager.Instance.GetPlayerHeader(array[i].dwUserID, ficeID, HeaderOptType.HEADER_CAR_PlAYER_LIST, i);
            }
        }
    }
    public void CarStopWeager()
    {

    }
    string FormatNum(int nNum)
    {
        string strNum="";       
        if(Mathf.Abs(nNum)<10000)
        {
            strNum=nNum.ToString();
        }
        else 
        {
            //strNum = ((float)nNum / 10000).ToString();
            strNum=string.Format("{0:F1}万",nNum/10000.0f);
        }
        return strNum;
    }
   public void SetCarPlayerGold()
    {
        if(m_labelmygold&&!m_carmove.IsMoving())
        {
            m_labelmygold.text = FormatNum((int)PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel());          
        }
    }
    public void SetPlayerName()
   {
        if(m_labelnmynickname)
        {
            m_labelnmynickname.text = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();
        }

        uint ficeID = PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID();
        if (ficeID < ConstValue.CUSTOM_HEADER)
        {           
            m_texmyavatar.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[ficeID];
        }
        else
        {            
            HeaderManager.Instance.GetPlayerHeader(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), ficeID, HeaderOptType.HEADER_CAR_SELF, null);
        }
   }
    void SetTip()
    {
        m_gotip[0].SetActive(CanBet());
        m_gotip[1].SetActive(!CanBet());
        if (m_gotip[0].activeSelf)
        {
             m_labeltip= m_gotip[0].transform.GetChild(2).GetComponent<UILabel>();
        }
        if (m_gotip[1].activeSelf)
        {
             m_labeltip= m_gotip[1].transform.GetChild(2).GetComponent<UILabel>();
        }       
    }
    Transform CloneApplyListItem(Transform trans, byte byOrder, string name, int gold)
    {
        GameObject newitem = GameObject.Instantiate(trans.gameObject);
        newitem.SetActive(true);
        newitem.transform.GetChild(0).GetComponent<UILabel>().text = byOrder.ToString();
        newitem.transform.GetChild(1).GetComponent<UILabel>().text = name;
        newitem.transform.GetChild(2).GetComponent<UILabel>().text = FormatNum(gold);
        return newitem.transform;
    }
    Transform ClonePlayerListItem(Transform trans, string name, int gold)
    {
        GameObject newitem=GameObject.Instantiate(trans.gameObject);
        newitem.SetActive(true);
        newitem.transform.GetChild(1).GetComponent<UILabel>().text=name;
        newitem.transform.GetChild(2).GetComponent<UILabel>().text=FormatNum(gold);
        return newitem.transform;
    }
//    class PlayListItem 
//{
//    public GameObject headObj;

//    public Texture2D headTex;
//    public UILabel nameLabel;
//    public UILabel goldLabel;

//    public NormalItem(Transform trans, string name, string gold)
//    {
//        this.name = name;
//        this.gold = gold;
//    }

//    public Transform Init()
//    {
//        m_BaseWndObject = GameObject.Instantiate(trans.gameObject);
//        m_BaseTrans = m_BaseWndObject.transform;
//        if (!m_BaseWndObject.activeSelf)
//            m_BaseWndObject.SetActive(true);
//        headObj = m_BaseTrans.GetChild(0).gameObject;
//        nameLabel = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
//        goldLabel = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
//        nameLabel.text = name;
//        goldLabel.text = gold.ToString();
//        if (gold > 10000)
//            goldLabel.text = (gold / 10000) + "." + (gold / 1000 % 10) + "万";
//        return m_BaseTrans;
//    }
//}


    void ClearGrid(UIGrid grid)
    {
        List<Transform> gridChid = grid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        grid.transform.DetachChildren();
    }
    bool CanBet()
    {
        return m_tablestatus== CarTableStates.CTS_Begin;      
    }
    void PlayBgMusic()
    {
        if(CanBet())
        {
            GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound8);
        }
        else
        {
            GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound9);
        }
    }
}

class AreaInfo
{
    public GameObject gobutton;
    public UILabel labeltotal;
    public UILabel labelmy;
    public GameObject goCeter;
    // int nTotal;
    //int nmy;
}
//class MoveCoin
//{
//    public MoveCoin(GameObject goItem, Vector3 vfrom, Vector3 to, float ftimesetup)
//    {
//        m_goItem = goItem;// GameObject.Instantiate(goItem);
//        m_vecfrom=vfrom;
//        m_vecto=to;
//        m_fTime = 0;
//        m_ftimesetup = ftimesetup;
//    }
//    public bool Update(float fTime)
//    {
//        m_fTime += fTime;
//        m_goItem.transform.position = Vector3.Lerp(m_vecfrom, m_vecto, m_fTime / m_ftimesetup);
//        return m_bdelete&&m_fTime>m_ftimesetup;            
//    }
//    public void Destroy()
//    {
//        GameObject.Destroy(m_goItem);
//    }
//    Vector3 m_vecfrom;
//    Vector3 m_vecto;
//    float m_fTime;
//    float m_ftimesetup;
//    bool m_bdelete;
    
//    GameObject m_goItem;
//}