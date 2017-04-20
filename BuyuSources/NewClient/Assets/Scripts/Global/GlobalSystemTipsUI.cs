using UnityEngine;
using System.Collections.Generic;

struct SystemTips
{
    public GameObject   m_Obj;
    public UILabel      m_DescribeUI;
    public UITexture    m_UITexture;
    public TweenAlpha   m_TweenAlpha;
}

struct ToUpgrade
{
    public GameObject   m_Obj;
    public Transform    m_Trans;
    public UILabel      m_Level;
    public UILabel      m_RewardDec;
    public UISprite     m_RewardIcon;
    public GameObject   m_RewardObj;
}

struct FinishTips
{
    public GameObject   m_Obj;
    public UILabel      m_Name;
    public UILabel      m_TipsTitle;
    public UISprite     m_TaskIcon;
    public UISprite     m_AchivementIcon;
}

public enum TipsType
{
    SystTemTips = 0,
    ToUpgradeTips,
    FinishTips,
}

public class GlobalSystemTipsUI : BaseWnd
{
    float               m_LifeTime;
    SystemTips          m_SystemTips;
    ToUpgrade           m_ToUpgrade;
    FinishTips          m_FinishTips;
    GameObject          m_OnOKBtnObj;
    Animator[]          m_Anim = new Animator[8];
    TipsType            m_TipsType;
    bool                m_bMask = false;
    float               m_AnimTime = 0;
    float               m_AnimBtnTime = 0;
                    
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.GlobalUIObject[1]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        //系统提示框
        m_SystemTips.m_Obj = m_BaseTrans.GetChild(0).gameObject;
        m_SystemTips.m_TweenAlpha = m_BaseTrans.GetChild(0).GetComponent<TweenAlpha>();
        m_SystemTips.m_DescribeUI = m_BaseTrans.GetChild(0).GetChild(0).GetComponent<UILabel>();
        m_SystemTips.m_UITexture = m_BaseTrans.GetChild(0).GetChild(1).GetComponent<UITexture>();

        //升级提示
        m_ToUpgrade.m_Obj = m_BaseTrans.GetChild(1).gameObject;
        m_ToUpgrade.m_Trans = m_BaseTrans.GetChild(1);
        m_ToUpgrade.m_Level = m_BaseTrans.GetChild(1).GetChild(0).GetComponent<UILabel>();
        m_ToUpgrade.m_RewardIcon = m_BaseTrans.GetChild(1).GetChild(1).GetChild(0).GetComponent<UISprite>();
        m_ToUpgrade.m_RewardDec = m_BaseTrans.GetChild(1).GetChild(1).GetChild(1).GetComponent<UILabel>();
        m_ToUpgrade.m_RewardObj = m_BaseTrans.GetChild(1).GetChild(1).gameObject;
        m_OnOKBtnObj = m_BaseTrans.GetChild(1).GetChild(1).GetChild(2).gameObject;
        UIEventListener.Get(m_OnOKBtnObj).onClick = clickOnOK;
        InitAnimator();
        //完成提示
        m_FinishTips.m_Obj = m_BaseTrans.GetChild(2).gameObject;
        m_FinishTips.m_Name = m_BaseTrans.GetChild(2).GetChild(0).GetComponent<UILabel>();
        m_FinishTips.m_TipsTitle = m_BaseTrans.GetChild(2).GetChild(3).GetChild(0).GetChild(0).GetComponent<UILabel>();
        m_FinishTips.m_TaskIcon = m_BaseTrans.GetChild(2).GetChild(1).GetComponent<UISprite>();
        m_FinishTips.m_AchivementIcon = m_BaseTrans.GetChild(2).GetChild(2).GetComponent<UISprite>();
    }
    void InitAnimator()
    {
        m_Anim[0] = m_BaseTrans.GetChild(1).GetChild(0).GetComponent<Animator>();
        m_Anim[1] = m_BaseTrans.GetChild(1).GetChild(1).GetChild(0).GetComponent<Animator>();
        m_Anim[2] = m_BaseTrans.GetChild(1).GetChild(1).GetChild(1).GetComponent<Animator>();
        m_Anim[3] = m_BaseTrans.GetChild(1).GetChild(1).GetChild(2).GetChild(0).GetComponent<Animator>();
        m_Anim[4] = m_BaseTrans.GetChild(1).GetChild(1).GetChild(3).GetComponent<Animator>();
        m_Anim[5] = m_BaseTrans.GetChild(1).GetChild(2).GetComponent<Animator>();
        m_Anim[6] = m_BaseTrans.GetChild(1).GetChild(3).GetComponent<Animator>();
        m_Anim[7] = m_BaseTrans.GetChild(1).GetChild(4).GetComponent<Animator>();
    }
    public void Update(float delta)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        m_LifeTime -= delta;
        if (m_LifeTime <= 0)
            ShutDown();
        if (m_AnimTime > 0)
        {
            m_AnimTime -= delta;
            if (m_AnimTime <= 0)
            {
                ShutDown();
                m_AnimTime = 0;
            }
        }
        if (m_AnimBtnTime > 0)
        {
            m_AnimBtnTime -= delta;
            if (m_AnimBtnTime <= 0)
                m_OnOKBtnObj.SetActive(true);
        }
    }
    public void UpdateSystemTime(float t)
    {
        if (m_BaseWndObject != null && m_SystemTips.m_TweenAlpha != null)
        {
            m_LifeTime = t;
            m_SystemTips.m_TweenAlpha.duration = t;
            m_SystemTips.m_TweenAlpha.ResetToBeginning();
        }
    }
    public void ShowSystemTips(string describe, float lifetime, bool bMask = false)
    {
        m_bMask = bMask;
        if (m_BaseWndObject == null)
        {
            Init();
            if (!bMask && LogonRuntime.Instance == null)
                WndManager.Instance.Push(m_BaseWndObject);
        }
        m_TipsType = TipsType.SystTemTips;
        m_SystemTips.m_Obj.SetActive(true);
        m_LifeTime = lifetime;
        m_SystemTips.m_TweenAlpha.duration = lifetime;
        m_SystemTips.m_TweenAlpha.ResetToBeginning();
        m_SystemTips.m_DescribeUI.text = describe;

    }
    public void ShowToUpgrade(byte Level, float lifetime, uint RewardID)
    {
        if (m_BaseWndObject != null)
        {
            ShutDown();
            // WndManager.Instance.Push(m_BaseWndObject);
        }
        Init();
        m_TipsType = TipsType.ToUpgradeTips;
        m_ToUpgrade.m_Obj.SetActive(true);
        m_LifeTime = lifetime;
        m_ToUpgrade.m_Level.text = Level.ToString();

        if (RewardID > 0)
        {
            tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID((ushort)RewardID);
            uint ItemID;
            
            if (pReward != null)
            {
                for (byte i = 0; i < pReward.RewardItemVec.Count; ++i)
                {
                    ItemID = pReward.RewardItemVec[i].ItemID;
                    if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                        continue;
                    for (byte j = 0; j < ConstValue.MAX_LAUNCHER_NUM; ++j )
                    {
                        if ((ushort)ItemID == LauncherSetting.LauncherDataList[j].nItemid )
                        {
                            //如果是已经获得该炮就不需要新手引导
                            if (PlayerRole.Instance.RoleLauncher.IsCanUseLauncherByAllTime(j))
                                return;
                        }
                    }
                    Vector3 pos =  m_ToUpgrade.m_Trans.localPosition;
                    pos.y += 60;
                    m_ToUpgrade.m_Trans.localPosition = pos;
                    m_AnimTime = 0;
                    m_LifeTime = 30;
                    m_AnimBtnTime = 0.5f;
                    m_ToUpgrade.m_RewardObj.SetActive(true);
                    m_OnOKBtnObj.SetActive(false);
                   m_ToUpgrade.m_RewardIcon.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon;
                   string strMsg = StringTable.GetString("LimitTime_Launcher_Tips");
                   strMsg = strMsg.Replace("@", pReward.RewardItemVec[i].LastMin.ToString());
                   m_ToUpgrade.m_RewardDec.text = strMsg;
                }
            }
            else
                Debug.Log("null");
        }
        
    }
    public void ShowFinishTips(uint ID, float lifetime, byte type /*'1'为成就 ‘2’为任务*/)
    {
        if (m_BaseWndObject == null)
        {
            Init();
            //WndManager.Instance.Push(m_BaseWndObject);

        }
        else
            return;
        m_FinishTips.m_Obj.SetActive(true);
        m_LifeTime = lifetime;
        m_TipsType = TipsType.FinishTips;
        //成就完成
        if (type == 1)
        {
            tagAchievementStrInfo achievementStrInfo = FishConfig.Instance.m_AchievementInfo.m_AchievementStr[(byte)ID];
            m_FinishTips.m_Name.text = achievementStrInfo.AchievementName;
            m_FinishTips.m_AchivementIcon.spriteName = achievementStrInfo.AchievementIcon;
            m_FinishTips.m_AchivementIcon.gameObject.SetActive(true);
            m_FinishTips.m_TipsTitle.text = StringTable.GetString("Achievement_Finish_Tips");

        }
        else if (type == 2)
        {
            tagTaskStrInfo taskStrInfo = FishConfig.Instance.m_TaskInfo.m_TaskStr[(byte)ID];
            m_FinishTips.m_Name.text = taskStrInfo.TaskName;
            m_FinishTips.m_TaskIcon.spriteName = taskStrInfo.TaskIcon;
            m_FinishTips.m_TaskIcon.gameObject.SetActive(true);
            m_FinishTips.m_TipsTitle.text = StringTable.GetString("Task_Finish_Tips");
        }

    }
    void clickOnOK(GameObject go)
    {
        m_AnimTime = 0.5f;
        for (byte i = 0; i < m_Anim.Length; ++i)
        {
            m_Anim[i].SetBool("UpperClick", true);
        }
     //  ShutDown();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            if (m_TipsType == TipsType.SystTemTips && !m_bMask)
                WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
	
}


public class PayBuyConfirmUI : BaseWnd
{
    UILabel m_DescLabel;
    PayType m_PayType;
    byte m_ItemID;
    int m_Amount;
    int m_Count;

    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[3]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);

        m_DescLabel = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnClickOnOK;
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = OnClickClose;
    }
    public void ShowPayBuyConfirm(byte ID, uint amount, uint count, PayType type)
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseWndObject);
        }
        m_ItemID = ID;
        m_PayType = type;
        m_Amount = (int)amount;
        m_Count = (int)count;
        if (m_PayType == PayType.Gold)
            m_DescLabel.text = StringTable.GetString("Shop_OnOKBuy");
        else
            m_DescLabel.text = StringTable.GetString("Shop_OnOKBuy");


        
    }
    void OnClickOnOK(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (SDKMgr.IS_SDK_CHANNEL || SDKMgr.IS_APP_STORE_VER)
        {
            System.UInt64 user_item_id = PlayerRole.Instance.RoleInfo.RoleMe.GetUserID();
            user_item_id = (user_item_id << 32) | m_ItemID;
            const int change = 100;
            if (m_PayType == PayType.Diamond)
            {
                SDKMgr.Instance.SDK.Pay(m_Amount * change, "钻石", m_Count, m_ItemID.ToString(), user_item_id.ToString(), m_ItemID);
            }
            else if (m_PayType == PayType.Gold)
            {
                SDKMgr.Instance.SDK.Pay(m_Amount * change, "金币", m_Count, m_ItemID.ToString(), user_item_id.ToString(), m_ItemID);
            }
        }
        else
        {
            PlayerRole.Instance.RoleRecharge.SendRecharge(m_ItemID, SDKMgr.Instance.IsServerOrder);
        }

        ShutDown();
        GlobalHallUIMgr.Instance.ShutDownPayWnd();
        //SDKMgr.Instance.SDK.Pay();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
       // ShutDown();
    }
    void OnClickClose(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        ShutDown();
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