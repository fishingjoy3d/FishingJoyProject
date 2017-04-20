using UnityEngine;
using System.Collections.Generic;

public class OpenParma
{
    public bool bOpen;  //是否开
    public bool bManual; //自动开,不需要效果
    public bool bImmediately; //立即开,没人和效果
    public float fDelayOpen;  //延迟

    public byte byOpenCount;  //开了几个
    public byte byOpenIndex;
    public byte byChestOnceID;
    public byte byChestTypeID;
    public byte byRewardid;
}

//路径直线移动
public class CLineMove
{
    public CLineMove(Vector3 vStart, Vector3 vEnd, float fDelaytime, float fValidTime)
    {
        m_vStart = vStart;
        m_vEnd = vEnd;
        m_fDelaytime = fDelaytime;
        m_fFlyTime = 0;
        m_fValidTime = fValidTime;
    }
    public Vector3 Update(float fTime)
    {
        if (m_fDelaytime > 0)
        {
            m_fDelaytime -= fTime;
            return m_vStart;
        }
        else
        {
            m_fFlyTime += fTime;
            return Vector3.Lerp(m_vStart, m_vEnd, m_fFlyTime / m_fValidTime);
        }
    }
    public bool IsEnd()
    {
        return m_fDelaytime <= 0 && m_fFlyTime >= m_fValidTime;
    }
    private Vector3 m_vStart;
    private Vector3 m_vEnd;
    private float m_fDelaytime;
    private float m_fFlyTime;
    private float m_fValidTime;
}


class BoxFly
{
    public GameObject m_goEffect1;
    Transform m_tfbox;
    bool m_bFly = false;

    float m_fShowDelay = 0f;

    CLineMove m_PathMove;
    public void Init(Vector3 vStart, Vector3 vEnd, Object objEff1)
    {
        m_PathMove = new CLineMove(vStart, vEnd, 0, 0.5f);
        m_goEffect1 = (GameObject)GameObject.Instantiate(objEff1);
        m_goEffect1.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_goEffect1.transform.position = vStart;
        m_tfbox = m_goEffect1.transform.GetChild(0).GetChild(0).GetChild(0).GetChild(0);
    }
    public void Destroy()
    {
        if (m_goEffect1)
        {
            GameObject.Destroy(m_goEffect1);
            m_goEffect1 = null;
        }
    }
    public bool Update(float fTime)
    {
        m_fShowDelay += fTime;
        if (m_fShowDelay >= 1.7f)
        {
            if (!m_bFly)
            {
                m_bFly = true;
                m_goEffect1.transform.GetChild(0).GetChild(0).GetComponent<Animator>().SetBool(Animator.StringToHash("Scale"), true);
            }
            m_tfbox.localRotation *= Quaternion.AngleAxis(-120f * fTime, Vector3.forward);

            m_goEffect1.transform.position = m_PathMove.Update(fTime);
            if (m_PathMove.IsEnd())
            {
                return false;
            }
        }
        return true;
    }
}
class BoxShow
{
    private SceneChestMgr m_pChest;
    private GameObject[] m_goBox = new GameObject[4];//第一个特殊判断
    private Transform[] m_tfBox = new Transform[4];

    OpenParma[] m_OpenParam = new OpenParma[4];

    private float m_fDelayMove;
    private float m_fInsideDlay = 0f;
    private bool m_bMoving = false;   //是否在移动
    private byte m_byMoveIndex = 0;   //当前正在移动哪个    
    const float InsideDealy = 0.2f;
    public void Init(SceneChestMgr pChest)
    {
        m_pChest = pChest;
    }
    private void BuildObject(byte nIndex, Object obj, Transform tfpos)
    {
        m_goBox[nIndex] = (GameObject)GameObject.Instantiate(obj);
        m_goBox[nIndex].SetActive(false);
        m_goBox[nIndex].transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_goBox[nIndex].transform.position = tfpos.GetChild(nIndex).GetChild(0).GetChild(0).position;
    }
    public void Show(bool bImmediately, GameObject goFirst, Object objOther, Transform tfpos, float fDelay)
    {
        if (!m_bMoving)//已经在移动
        {
            m_bMoving = true;
            m_fInsideDlay = InsideDealy;
            m_fDelayMove = fDelay;
            if (goFirst == null)
            {
                m_byMoveIndex = 0;
                BuildObject(0, objOther, tfpos);
            }
            else//已经
            {
                m_byMoveIndex = 1;
                m_goBox[0] = goFirst;
                m_pChest.ShowLabel(0);
            }
            //other
            for (byte i = 1; i < 4; i++)
            {
                if (m_goBox[i] == null)
                {
                    BuildObject(i, objOther, tfpos);
                }
            }
            for (byte i = 0; i < 4; i++)
            {
                m_tfBox[i] = m_goBox[i].transform.GetChild(0).GetChild(0).GetChild(0).GetChild(0);
            }
            if (bImmediately)//立即显示
            {
                ActiveWnd();
                ActiveWnd();
                ActiveWnd();
                ActiveWnd();
            }
        }
    }

    public void ShutDown()
    {
        for (byte i = 0; i < m_goBox.Length; i++)
        {
            if (m_goBox[i] != null)
            {
                GameObject.Destroy(m_goBox[i]);
            }
        }
    }
    public void Hide()
    {
        m_bMoving = false;
        if (m_goBox != null)
        {
            GameObject.Destroy(m_goBox[0]);
            m_goBox[0] = null;
        }
        for (byte i = 1; i < 4; i++)
        {
            if (m_goBox[i] != null)
            {
                m_goBox[i].SetActive(false);
            }
        }

        for (byte i = 0; i < m_OpenParam.Length; i++)
        {
            m_OpenParam[i] = null;
        }
    }
    void ActiveWnd()
    {
        if (!m_goBox[m_byMoveIndex].activeSelf)
        {
            m_pChest.ShowLabel(m_byMoveIndex);
            m_goBox[m_byMoveIndex].SetActive(true);
            m_goBox[m_byMoveIndex].transform.GetChild(0).GetChild(0).GetComponent<Animator>().SetBool(Animator.StringToHash("Fly"), true);
        }
        m_byMoveIndex++;
        if (m_byMoveIndex == m_goBox.Length)
        {
            m_pChest.SetButton(true);
            m_bMoving = false;
        }
    }
    public void Update(float fTime)
    {
        if (m_bMoving)
        {
            if (m_fDelayMove > 0f)//总的延迟
            {
                m_fDelayMove -= fTime;
            }
            else
            {
                if (m_fInsideDlay > 0)//每个条目延迟
                {
                    m_fInsideDlay -= fTime;
                }
                else //(m_fInsideDlay < 0)
                {
                    m_fInsideDlay = InsideDealy;
                    ActiveWnd();
                }
            }
        }

        for (byte i = 0; i < m_goBox.Length; i++)
        {
            if (m_goBox[i] && m_goBox[i].activeSelf)
            {
                m_tfBox[i].localRotation *= Quaternion.AngleAxis(-120f * fTime, Vector3.forward);

                if (m_OpenParam[i] != null && m_OpenParam[i].bOpen)
                {
                    m_OpenParam[i].fDelayOpen -= fTime;
                    if (m_OpenParam[i].fDelayOpen < 0f)
                    {
                        OnBoxRocked(i);
                    }
                }
            }

        }
    }
    void OnBoxRocked(byte nIndex)
    {
        m_OpenParam[nIndex].bOpen = false;
        m_goBox[nIndex].SetActive(false);
        m_pChest.BoxRocked(m_OpenParam[nIndex]);
    }
    //void 
    public void RockBox(OpenParma pParam)
    {
        m_OpenParam[pParam.byOpenIndex] = pParam;//映射相对位置
        if (m_goBox[pParam.byOpenIndex])
        {
            if (pParam.bImmediately)//立即显示
            {
                OnBoxRocked(pParam.byOpenIndex);
            }
            else
            {
                if (pParam.bManual)
                {
                    m_goBox[pParam.byOpenIndex].transform.GetChild(0).GetChild(0).GetComponent<Animator>().SetBool(Animator.StringToHash("Rock"), true);
                }
                else
                {
                    m_goBox[pParam.byOpenIndex].transform.GetChild(0).GetChild(0).GetComponent<Animator>().SetBool(Animator.StringToHash("BoxScale"), true);
                }
            }
        }
    }
}

enum MsgType
{
    SHOW = 0,
    OPEN = 1,
    HIDE = 2
}

class Message
{
    public MsgType m_msgtype;
    public uint m_byparam1;
    public uint m_byparam2;
    public uint m_byparam3;
}
public class SceneChestMgr
{
    Object m_objEff1;
    Object m_ObjeffTurn;
    Object m_objWnd;
  

    GameObject m_Obj_Chest;
    ItemInfo[] m_BtnInfo = new ItemInfo[4];

    BoxShow m_BoxShow = new BoxShow();

    UILabel m_LabelTime;
    GameObject m_goBoxNum;
    UILabel m_LabelBoxNum;
    GameObject m_goCancel;
    UIButton m_btnCancel;

    float m_fDealTime = 0;
    List<Message> m_MessageList = new List<Message>();
    float m_fMessageDelay = 0f;  //消息队列延迟
    byte m_byChestOnlyID = 0;
    byte m_byChestTypeID = 0;
    List<byte> m_nChestRewardidList = new List<byte>();//还剩下没开的  
    List<BoxFly> m_FlyBoxs = new List<BoxFly>();
    byte m_byStoreBoxs = 0;//已经完成飞行的宝箱数目
    const float DELAY_BT_EFFECT = 0.2f;
    float m_fWndHide;//窗口隐藏记时


    public void Init()
    {        
        m_objEff1 = ResManager.Instance.LoadObject("UIEf_RewardBox", "SceneRes/Prefab/Effect/Chest/", ResType.SceneRes); //ResManager.Instance.LoadObject();

        m_ObjeffTurn = ResManager.Instance.LoadObject("UIEf_ChestTurn", "SceneRes/Prefab/Effect/Chest/", ResType.SceneRes);

        m_objWnd = ResManager.Instance.LoadObject("ChestWind", "SceneRes/Prefab/UI/Chest/", ResType.SceneRes);
        m_Obj_Chest = GameObject.Instantiate(m_objWnd) as GameObject;
        m_Obj_Chest.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);


        m_goBoxNum = m_Obj_Chest.transform.GetChild(0).GetChild(0).GetChild(1).gameObject;
        m_LabelBoxNum = m_goBoxNum.transform.GetChild(0).GetChild(0).GetComponent<UILabel>();

        m_goCancel = m_Obj_Chest.transform.GetChild(0).GetChild(0).GetChild(2).gameObject;
        m_btnCancel = m_goCancel.GetComponent<UIButton>();

        Transform BaseT = m_Obj_Chest.transform.GetChild(0).GetChild(0);
        m_LabelTime = BaseT.GetChild(0).gameObject.GetComponent<UILabel>();



        BaseT = m_Obj_Chest.transform.GetChild(0).GetChild(1);
        for (byte i = 0; i < m_BtnInfo.Length; i++)
        {
            GameObject goItem = BaseT.GetChild(i).gameObject.transform.GetChild(0).gameObject;

            m_BtnInfo[i].m_goItem = goItem.transform.GetChild(0).gameObject;
            m_BtnInfo[i].m_sprItem = m_BtnInfo[i].m_goItem.GetComponent<UISprite>();

            m_BtnInfo[i].m_goBtn = goItem.transform.GetChild(1).gameObject;
            m_BtnInfo[i].m_BtnInfo = m_BtnInfo[i].m_goBtn.GetComponent<UIButton>();


            m_BtnInfo[i].m_gobg = m_Obj_Chest.transform.GetChild(0).GetChild(2).GetChild(7 + i).gameObject; //goItem.transform.GetChild(3).gameObject;
            m_BtnInfo[i].m_sprBg = m_BtnInfo[i].m_gobg.transform.GetChild(0).GetComponent<UISprite>();


            m_BtnInfo[i].m_goLabelNormal = m_BtnInfo[i].m_gobg.transform.GetChild(0).GetChild(0).gameObject;
            m_BtnInfo[i].m_labelNormal = m_BtnInfo[i].m_goLabelNormal.GetComponent<UILabel>();

            m_BtnInfo[i].m_goDiamond = m_BtnInfo[i].m_gobg.transform.GetChild(0).GetChild(1).gameObject;// goItem.transform.GetChild(3).gameObject;
            m_BtnInfo[i].m_labeldiamond = m_BtnInfo[i].m_goDiamond.transform.GetChild(0).GetComponent<UILabel>();


            UIEventListener.Get(m_BtnInfo[i].m_goBtn).onClick = OnClickBtn;
            UIEventListener.Get(m_BtnInfo[i].m_goBtn).onPress = OnPressMsg;

            m_BtnInfo[i].m_goOpenEff = (GameObject)GameObject.Instantiate(m_ObjeffTurn);
            m_BtnInfo[i].m_goOpenEff.gameObject.transform.SetParent(m_BtnInfo[i].m_goItem.transform, false);
        }

        //刷新按钮
        UIEventListener.Get(m_goCancel).onClick = OnClickBtn;
        UIEventListener.Get(m_goCancel).onPress = OnPressMsg;

        m_BoxShow.Init(this);
        HideChestWindow(true);
    }

    public void ShutDown()
    {
        ResManager.Instance.UnloadObject(m_objEff1);
        ResManager.Instance.UnloadObject(m_ObjeffTurn);
        ResManager.Instance.UnloadObject(m_objWnd);

        if (m_Obj_Chest != null)
        {
            GameObject.Destroy(m_Obj_Chest);
            m_Obj_Chest = null;
        }

        for (byte i = 0; i < m_BtnInfo.Length; i++)
        {
            GameObject.Destroy(m_BtnInfo[i].m_goOpenEff);
        }
       
        DelFlyBoxs();
        m_BoxShow.ShutDown();

    }
    void DelFlyBoxs()
    {
        for (byte i = 0; i < m_FlyBoxs.Count; i++)
        {
            m_FlyBoxs[i].Destroy();
        }
        m_FlyBoxs.Clear();
    }
    byte CheckedCount()
    {
        byte byCount = 0;
        for (byte i = 0; i < m_BtnInfo.Length; i++)
        {
            if (m_BtnInfo[i].m_IsChecked)
            {
                byCount++;
            }
        }
        return byCount;
    }
    void SetBoxCount()
    {
        m_goBoxNum.SetActive(m_byStoreBoxs > 1);
        if (m_byStoreBoxs > 1)
        {
            m_LabelBoxNum.text = (m_byStoreBoxs - 1).ToString();
        }
    }
    public void ShowLabel(int nIndex)
    {
        m_BtnInfo[nIndex].m_gobg.SetActive(true);
        m_BtnInfo[nIndex].m_sprBg.spriteName = "ChestBoxNum";

        m_BtnInfo[nIndex].m_goLabelNormal.SetActive(true);
        m_BtnInfo[nIndex].m_labelNormal.text = "免费";
    }
    void ShowBg()
    {
        if (!m_Obj_Chest.activeSelf)
        {
            SetButton(false);
            m_Obj_Chest.SetActive(true);
            SetBoxCount();
            //Reset
            m_LabelTime.text = "";
            for (byte i = 0; i < m_BtnInfo.Length; i++)
            {
                m_BtnInfo[i].m_IsChecked = false;

                m_BtnInfo[i].m_goItem.SetActive(false);
                m_BtnInfo[i].m_goDiamond.SetActive(false);
                m_BtnInfo[i].m_goLabelNormal.SetActive(false);
                m_BtnInfo[i].m_gobg.SetActive(false);
            }
        }
    }

    public void MoveBox(Vector3 vecBox)
    {
        //立即显示背景
        ShowBg();
        BoxFly pBox = new BoxFly();
        pBox.Init(vecBox, m_Obj_Chest.gameObject.transform.GetChild(0).GetChild(1).GetChild(0).GetChild(0).GetChild(0).position, m_objEff1);  //0.1下第0个item目录   
        m_FlyBoxs.Add(pBox);
    }

    public void OnReset()
    {
        HideChestWindow(true);
        DelFlyBoxs();
        m_byStoreBoxs = 0;
        m_MessageList.Clear();
    }
    public void ShowChestWindow(tagChestActionEvent pChest)
    {
        tagChestInfo pChestInfo = PlayerRole.Instance.ChestManager.GetNowActionChest();
        if (pChestInfo == null)
        {
            return;
        }

        if (pChest.IsReset)
        {
            OnReset();
            m_byStoreBoxs = pChest.ChestSum;
            ShowBg();
            Transform tfTemp = m_Obj_Chest.transform.GetChild(0).GetChild(2);
            tfTemp.GetChild(0).GetComponent<Animator>().Play(Animator.StringToHash("ChestWindBg_Anim"), 0, 1.0f);
            tfTemp.GetChild(1).GetComponent<Animator>().Play(Animator.StringToHash("DownSprite_Anim"), 0, 1.0f);
            tfTemp.GetChild(2).GetComponent<Animator>().Play(Animator.StringToHash("UpperSprite_Anim"), 0, 1.0f);

            tfTemp.GetChild(3).GetChild(0).GetComponent<Animator>().Play(Animator.StringToHash("Dummy(1)_Anim"), 0, 1.0f);
            tfTemp.GetChild(4).GetChild(0).GetComponent<Animator>().Play(Animator.StringToHash("Item2_Anim"), 0, 1.0f);
            tfTemp.GetChild(5).GetChild(0).GetComponent<Animator>().Play(Animator.StringToHash("Item3_Anim"), 0, 1.0f);
            tfTemp.GetChild(6).GetChild(0).GetComponent<Animator>().Play(Animator.StringToHash("Item4_Anim"), 0, 1.0f);

            tfTemp.GetChild(7).GetChild(0).GetComponent<Animator>().Play(Animator.StringToHash("FreeAlpha_Anim"), 0, 1.0f);
            tfTemp.GetChild(8).GetChild(0).GetComponent<Animator>().Play(Animator.StringToHash("FreeAlpha_Anim"), 0, 1.0f);
            tfTemp.GetChild(9).GetChild(0).GetComponent<Animator>().Play(Animator.StringToHash("FreeAlpha_Anim"), 0, 1.0f);
            tfTemp.GetChild(10).GetChild(0).GetComponent<Animator>().Play(Animator.StringToHash("FreeAlpha_Anim"), 0, 1.0f);

            //显示未开的            
            DoShow(pChest.ActionChest.ChestTypeID, pChest.ActionChest.ActionTime, pChest.ActionChest.ChestOnlyID, true);
            //开过的
            if (pChest.ActionChest.ChestArray != null)
            {
                for (byte i = 0; i < pChest.ActionChest.ChestArray.Count; i++)
                {
                    ChestOnceStates pOpen = pChest.ActionChest.ChestArray[i];
                    OnOpenBox(true, pOpen.Index, pOpen.RewardID, pOpen.RewardOnlyID, false);
                }
            }
        }
        else
        {
            Message pMessage = new Message();
            pMessage.m_msgtype = MsgType.SHOW;
            pMessage.m_byparam1 = pChestInfo.ChestTypeID;
            pMessage.m_byparam2 = pChestInfo.ActionTime;// - Utility.GetTickCount())/1000);
            pMessage.m_byparam3 = pChestInfo.ChestOnlyID;
            m_MessageList.Add(pMessage);
        }
    }
    public void HideChestWindow(bool bForce = false)
    {
        if (bForce)//清理界面
        {
            m_Obj_Chest.SetActive(false);
            m_BoxShow.Hide();
            HideOpenEff();
        }
        else//把剩下的开掉
        {
            byte nIndex = 0;
            Message pMessage = null;
            for (byte i = 0; i < m_BtnInfo.Length; i++)
            {
                if (!m_BtnInfo[i].m_IsChecked && nIndex < m_nChestRewardidList.Count)
                {
                    pMessage = new Message();
                    pMessage.m_msgtype = MsgType.OPEN;
                    pMessage.m_byparam1 = i;
                    pMessage.m_byparam2 = m_nChestRewardidList[nIndex++];
                    List<byte> listKeys=new List<byte>(FishConfig.Instance.m_ChestInfo.ChestMap[m_byChestTypeID].RewardInfo.RewardMap[(byte)pMessage.m_byparam2].RewardLog.Keys);
                    pMessage.m_byparam3 = listKeys[Utility.Range(0, listKeys.Count-1)];              
                    m_MessageList.Add(pMessage);
                }
            }
            pMessage = new Message();
            pMessage.m_msgtype = MsgType.HIDE;
            m_MessageList.Add(pMessage);
        }
    }
    void DoShow(uint byChestid, uint byTime, uint uChestOnlyID, bool bImmediately)
    {
        ShowBg();
        m_BoxShow.Show(bImmediately, null, m_objEff1, m_Obj_Chest.gameObject.transform.GetChild(0).GetChild(1), 1.0f);
        m_nChestRewardidList.Clear();
        tagChestConfig pConfig = FishConfig.Instance.m_ChestInfo.ChestMap[(byte)byChestid];
        foreach (byte byid in pConfig.RewardInfo.RewardMap.Keys)
        {
            m_nChestRewardidList.Add(byid);
        }
        m_fDealTime = (float)(pConfig.ExisteSec - ((Utility.GetTickCount() - byTime) / 1000));
        m_byChestTypeID = (byte)byChestid;
        m_byChestOnlyID = (byte)uChestOnlyID;
    }
    public void BoxRocked(OpenParma pParam)
    {
        ShowReward(pParam.bImmediately, pParam.byOpenIndex, pParam.byRewardid, pParam.byChestOnceID, pParam.bManual);
        SetCostInfo(pParam.byOpenCount, pParam.byChestTypeID);
    }
    public void SetButton(bool bEnable)
    {
        for (byte i = 0; i < m_BtnInfo.Length; i++)
        {
            m_BtnInfo[i].m_BtnInfo.isEnabled = bEnable;
        }
        m_btnCancel.isEnabled = bEnable;
        m_LabelTime.enabled = bEnable;
    }
    public void SetFreeInfo()
    {
        for (byte i = 0; i < m_BtnInfo.Length; i++)
        {
            if (!m_BtnInfo[i].m_IsChecked)
            {
                m_BtnInfo[i].m_goDiamond.SetActive(false);
                m_BtnInfo[i].m_goLabelNormal.SetActive(true);
                // m_BtnInfo[i].m_labelNormal.text = "免费";
            }
        }
    }
    public void SetCostInfo(byte byOpen, byte byChestTypeID)
    {
        if (byOpen < FishConfig.Instance.m_ChestInfo.ChestMap[byChestTypeID].CostInfo.MaxCostNum)
        {
            uint uConst = (uint)FishConfig.Instance.m_ChestInfo.ChestMap[byChestTypeID].CostInfo.CostMap[(byte)(byOpen + 1)];
            if (uConst == 0)
            {
                SetFreeInfo();
            }
            else
            {
                for (byte i = 0; i < m_BtnInfo.Length; i++)
                {
                    if (!m_BtnInfo[i].m_IsChecked)
                    {
                        m_BtnInfo[i].m_goDiamond.SetActive(true);
                        m_BtnInfo[i].m_goLabelNormal.SetActive(false);
                        m_BtnInfo[i].m_labeldiamond.text = uConst.ToString(); ;
                    }
                }
            }
        }
    }
    void ShowReward(bool bImmediately, byte byIndex, byte byRewardid, byte byChestOnceID, bool bOpened)
    {
        tagChestsRewardOnce pOnce = FishConfig.Instance.m_ChestInfo.ChestMap[m_byChestTypeID].RewardInfo.RewardMap[byRewardid].RewardLog[byChestOnceID];
        tagRewardOnce pReward = FishConfig.Instance.m_RewardConfig.RewardMap[pOnce.RewardID];

        if (pReward.RewardItemVec.Count != 1)
        {
            Debug.Log("物品不唯一");
        }
        tagItemOnce pItem = pReward.RewardItemVec[0];
        tagItemConfig pItemConfig = FishConfig.Instance.m_ItemInfo.m_ItemMap[pItem.ItemID];

        m_BtnInfo[byIndex].m_sprBg.spriteName = "ChestPropertyNum";

        m_BtnInfo[byIndex].m_goItem.SetActive(true);
        m_BtnInfo[byIndex].m_sprItem.spriteName = pItemConfig.ItemIcon.ToString();
        if (!bImmediately)
        {
            if (bOpened)
            {
                m_BtnInfo[byIndex].m_goOpenEff.SetActive(true);
            }
            else
            {
                m_BtnInfo[byIndex].m_goItem.GetComponent<Animator>().SetBool(Animator.StringToHash("ItemScale"), true);
            }
        }
        m_BtnInfo[byIndex].m_goDiamond.SetActive(false);
        m_BtnInfo[byIndex].m_goLabelNormal.SetActive(true);
        m_BtnInfo[byIndex].m_labelNormal.text = pItem.ItemSum.ToString();
    }
    public void OnOpenBox(bool bImmediately, uint byIndex, uint byRewardid, uint byChestOnceID, bool bEffect)
    {

        if (m_BtnInfo[byIndex].m_IsChecked)
        {
            return;
        }
        if (!m_nChestRewardidList.Remove((byte)byRewardid))
        {
            return;
        }
        m_BtnInfo[byIndex].m_IsChecked = true;
        byte byOpenCount = CheckedCount();
        OpenParma pParam = new OpenParma();

        pParam.bManual = bEffect;
        pParam.bOpen = true;
        pParam.bImmediately = bImmediately;
        pParam.fDelayOpen = 0.5f;

        pParam.byOpenCount = byOpenCount;
        pParam.byChestTypeID = m_byChestTypeID;

        pParam.byOpenIndex = (byte)byIndex;
        pParam.byRewardid = (byte)byRewardid;
        pParam.byChestOnceID = (byte)byChestOnceID;

        m_BoxShow.RockBox(pParam);

        if (byOpenCount > FishConfig.Instance.m_ChestInfo.ChestMap[m_byChestTypeID].CostInfo.MaxCostNum)
        {
            m_fMessageDelay = 3.0f;
        }
        else
        {
            m_fMessageDelay = 0.2f;
        }
    }


    public bool Update(float dTime)
    {
        m_fWndHide += dTime;
        m_BoxShow.Update(dTime);
    
        for (byte i = 0; i < m_FlyBoxs.Count; )
        {
            BoxFly pBox = m_FlyBoxs[i];
            if (!pBox.Update(dTime))
            {
                if (m_byStoreBoxs == 0)//the first one
                {
                    m_BoxShow.Show(false, pBox.m_goEffect1, m_objEff1, m_Obj_Chest.gameObject.transform.GetChild(0).GetChild(1), 0f);//延迟，先出背景                   
                    pBox.m_goEffect1 = null;
                }
                else//nothing
                {

                }
                m_byStoreBoxs++;
                SetBoxCount();
                pBox.Destroy();
                Utility.ListRemoveAt(m_FlyBoxs, i);
                continue;
            }
            else
            {
                i++;
            }
        }

        if (m_fMessageDelay > 0f)
        {
            m_fMessageDelay -= dTime;
        }
        if (m_MessageList.Count > 0 && m_byStoreBoxs > 0 && m_fMessageDelay <= 0f)
        {
            Message pMessage = m_MessageList[0];
            m_MessageList.RemoveAt(0);
            if (pMessage.m_msgtype == MsgType.SHOW)
            {
                DoShow(pMessage.m_byparam1, pMessage.m_byparam2, pMessage.m_byparam3, false);
            }
            else if (pMessage.m_msgtype == MsgType.OPEN)
            {
                OnOpenBox(false, pMessage.m_byparam1, pMessage.m_byparam2, pMessage.m_byparam3, false);
            }
            else if (pMessage.m_msgtype == MsgType.HIDE)
            {
                HideChestWindow(true);
                m_byStoreBoxs--;
                SetBoxCount();
                m_fWndHide = 0f;
            }
        }

        m_fDealTime -= dTime;
        if (m_fDealTime >= 0)
        {
            m_LabelTime.text = ((int)m_fDealTime).ToString() + '秒';
        }
        return true;
    }
    void HideOpenEff()
    {
        for (byte i = 0; i < m_BtnInfo.Length; i++)
        {
            m_BtnInfo[i].m_goOpenEff.SetActive(false);
        }
    }
    void OnPressMsg(GameObject obj, bool state)
    {

    }
    void OnClickBtn(GameObject go)
    {
        tagChestInfo pChestInfo = PlayerRole.Instance.ChestManager.GetNowActionChest();
        if (pChestInfo != null && pChestInfo.ChestOnlyID == m_byChestOnlyID)//是否过期
        {
            for (byte i = 0; i < m_BtnInfo.Length; ++i)
            {
                if (m_BtnInfo[i].m_goBtn == go && !m_BtnInfo[i].m_IsChecked)
                {
                    if (m_BtnInfo[i].m_goDiamond.activeSelf)
                    {
                        uint uCost = System.Convert.ToUInt32(m_BtnInfo[i].m_labeldiamond.text);
                        if (uCost != 0 && uCost > PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency())
                        {
                            GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
                            return;
                        }
                    }
                    PlayerRole.Instance.ChestManager.GetChestRewardByIndex(i);
                    return;
                }
            }
            if (go == m_goCancel)
            {
                PlayerRole.Instance.ChestManager.CloseNowChest();
            }
        }
    }
    public struct ItemInfo
    {
        public GameObject m_goItem;
        public UISprite m_sprItem;

        public GameObject m_goBtn;
        public UIButton m_BtnInfo;

        public GameObject m_goLabelNormal;
        public UILabel m_labelNormal;

        public GameObject m_goDiamond;
        public UILabel m_labeldiamond;

        public GameObject m_gobg;
        public UISprite m_sprBg;
        public bool m_IsChecked;

        public GameObject m_goOpenEff;

    }
}
