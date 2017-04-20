using UnityEngine;
using System.Collections.Generic;


public class ActivityRewardWnd : BaseWnd
{
    GameObject m_CloseBtn;
    GameObject m_PayGoBtn;
    GameObject      m_GetRewardBtn;
    GameObject      m_OnOkBtn;
    GameObject      m_RewardCaiDai;
    UISprite        m_RewardTitle;
    UISprite[]      m_Reward = new UISprite[6];
    UILabel[]       m_RewardNum = new UILabel[6];
    TweenScale      m_TweenAnim;
    float           m_LifeTime;
    uint            m_ActivityID;   //活动ID
    uint            m_NodeID;       //活动下面的结点ID

    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.GlobalUIObject[4]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_CloseBtn = m_BaseTrans.GetChild(3).gameObject;
        m_PayGoBtn = m_BaseTrans.GetChild(0).gameObject;
        m_GetRewardBtn = m_BaseTrans.GetChild(1).gameObject;
        m_RewardCaiDai = m_BaseTrans.GetChild(4).gameObject;
        m_OnOkBtn = m_BaseTrans.GetChild(2).gameObject;
        m_RewardTitle = m_BaseTrans.GetChild(4).GetChild(0).GetComponent<UISprite>();

        for (byte i = 0; i < m_Reward.Length; ++i)
        {
            m_Reward[i] = m_BaseTrans.GetChild(5).GetChild(i).GetComponent<UISprite>();
            m_RewardNum[i] = m_BaseTrans.GetChild(5).GetChild(i).GetChild(0).GetComponent<UILabel>();
        }
        UIEventListener.Get(m_PayGoBtn).onClick = OnClickPayGo;
        UIEventListener.Get(m_GetRewardBtn).onClick = OnGetReward;
        UIEventListener.Get(m_OnOkBtn).onClick = OnClickCloseWnd;
        UIEventListener.Get(m_CloseBtn).onClick = OnClickCloseWnd;


    }
    public void Upate(float deltaTime)
    {
        if (m_BaseWndObject == null)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        //m_LifeTime -= deltaTime;
        //if (m_LifeTime <= 0)
        //    ShutDown();
    }
    //预览奖励物品
    public void PreviewRewardWnd(uint rewardID, ActivityType wndType)
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseWndObject);
        }

        m_LifeTime = 3.0f;
        m_CloseBtn.SetActive(true);
        if (wndType == ActivityType.LevelActivity_UI || wndType == ActivityType.SignIn_UI)
            m_PayGoBtn.SetActive(false);
        else
            m_PayGoBtn.SetActive(true);

        ShowItem(rewardID);

    }
    //领取奖励物品
    public void GetRewardWnd(uint rewardID, uint activityID, uint nodeID, ActivityType wndType)
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseWndObject);
        }
        m_LifeTime = 3.0f;

        m_RewardCaiDai.SetActive(true);
        m_GetRewardBtn.SetActive(true);
        if (wndType == ActivityType.PayActivity_UI)
            m_RewardTitle.spriteName = "Activity_PayReward_Chests";
        else
            m_RewardTitle.spriteName = "Activity_LevelReward_Chests";
        m_ActivityID = activityID;
        m_NodeID = nodeID;
        ShowItem(rewardID);
    }
    public void ShowRewardWnd(ushort[] RewardID)
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseWndObject);
        }
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_GoldJump);
        m_LifeTime = 3.0f;
        m_OnOkBtn.SetActive(true);
        m_RewardCaiDai.SetActive(true);
        m_RewardTitle.spriteName = "Activity_PayReward_Chests";
        byte Idx = 0;
        for (byte i = 0; i < RewardID.Length; ++i)
        {
            tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID((ushort)RewardID[i]);
            if (pReward == null)
                continue;
            for (byte j = 0; j < pReward.RewardItemVec.Count; ++j)
            {
                uint ItemID = pReward.RewardItemVec[j].ItemID;
                if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                    continue;
                m_Reward[Idx].enabled = true;
                m_Reward[Idx].spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon;
                if (FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemTypeID == EItemType.IT_Cannon)
                {
                    m_Reward[Idx].width = 55;
                    m_Reward[Idx].height = 68;
                    Idx++;
                    continue;
                }
                if (pReward.RewardItemVec[j].ItemSum >= 10000)
                    m_RewardNum[Idx].text = (pReward.RewardItemVec[j].ItemSum / 1000).ToString() + "k";
                else
                    m_RewardNum[Idx].text = pReward.RewardItemVec[j].ItemSum.ToString();
                Idx++;

            }
        }
    }
    void ShowItem(uint RewardID)
    {
        tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID((ushort)RewardID);
        if (pReward == null)
            return;
        for (byte i = 0; i < pReward.RewardItemVec.Count; ++i)
        {
            uint ItemID = pReward.RewardItemVec[i].ItemID;
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                continue;
            m_Reward[i].enabled = true;
            m_Reward[i].spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon;
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemTypeID == EItemType.IT_Cannon)
            {
                m_Reward[i].width = 55;
                m_Reward[i].height = 68;
                continue;
            }
            if (pReward.RewardItemVec[i].ItemSum >= 10000)
                m_RewardNum[i].text = (pReward.RewardItemVec[i].ItemSum / 1000).ToString() + "k";
            else
                m_RewardNum[i].text = pReward.RewardItemVec[i].ItemSum.ToString();
        }
    }
    void OnClickPayGo(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
        ShutDown();
    }
    void OnGetReward(GameObject go)
    {
        // GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_GoldJump);

        PlayerRole.Instance.ActionManager.GetActionReward((byte)m_ActivityID, m_NodeID);
        ShutDown();
    }
    void OnClickCloseWnd(GameObject go)
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
