using UnityEngine;
using System.Collections.Generic;

class CDInfo
{
    public float    m_CDTime;
    public float    m_CurTime;
    public byte     m_CDIndx;
    public bool     m_bUseing;
}

struct LeftBtnUnfoldInfo
{
    public TweenScale   m_TweenAnim;
    public UISprite     m_ProgressSprite;
    public UILabel      m_ProgressLabel;
    public UILabel      m_Label;
    public UILabel      m_RewardLabel;
    public bool         m_bShowLockedUI;
    public float        m_Elapsed;
    public bool         m_bFish;        //是否完成条件
    public bool         m_bPop;
}

public class SkillBar : BaseWnd
{
    public ushort[]         m_nCountRecord = new ushort[(byte)SkillType.SKILL_MAX];
    UISprite[]              m_CDSprite = new UISprite[2];
    UIButton[]              m_UIButton = new UIButton[2];
    Transform[]             m_RightBtnTrans = new Transform[2]; //技能按纽和锁定按纽
    Vector3[]               m_RightBtnPos = new Vector3[2];
    GameObject[]            m_LockObj = new GameObject[2];
    GameObject[]            m_BarAndReward = new GameObject[2];
    List<CDInfo>            m_CDList = new List<CDInfo>();
    LeftBtnUnfoldInfo[]     m_LeftUnfoldInfo = new LeftBtnUnfoldInfo[2];
    GameObject              m_UnLockBtnObj;
    GameObject              m_LeftBtnObj;
    byte                    m_CurSkillType;
    bool[]                  m_bUseing = new bool[2];
    Benefit                 m_benefit = new Benefit();
    GameObject[]            m_EftObj = new GameObject[3];
    float                   m_EftElapsed = 0;

    public void SkillFail(NetCmdSkillFailed pSkill)
    {
        switch((SkillFailedType)pSkill.FailedType)
        {
            case SkillFailedType.SFT_CD:
                {
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Skill_Tip_cd"), 1, false);
                    break;
                }
            case SkillFailedType.SFT_COUNT:
                {
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Skill_Tip_count"), 1, false);
                    break;
                }
            case SkillFailedType.SFT_INVALID:
                {
                    GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Skill_Tip_invalid"), 1, false);
                    break;
                }
        }       
    }
    public void RecordUsed(SkillType index,byte byseat)
    {
         if (byseat == SceneRuntime.SceneLogic.PlayerMgr.MyClientSeat)
        {
            m_nCountRecord[(byte)index]++;
        }
    }
    public void Init()
    {
        Object skillgo = ResManager.Instance.LoadObject("Skill_Bar", "SceneRes/Prefab/UI/Skill/", ResType.SceneRes);

        m_BaseWndObject = GameObject.Instantiate(skillgo) as GameObject;
        if (m_BaseWndObject == null)
            return;

        m_BaseWndObject.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_BaseTrans = m_BaseWndObject.transform;

        for (int i = 0; i < 4; i++)
        {
            if (i >= 2)
            {
                m_RightBtnTrans[i - 2] = m_BaseTrans.GetChild(i);
                m_RightBtnPos[i - 2] = m_RightBtnTrans[i - 2].localPosition;
                m_UIButton[i-2] = m_BaseTrans.GetChild(i).GetComponent<UIButton>();
                m_CDSprite[i-2] = m_BaseTrans.GetChild(i).GetChild(1).gameObject.GetComponent<UISprite>();
                m_LockObj[i - 2] = m_BaseTrans.GetChild(i).GetChild(2).gameObject;
            }
            switch(i)
            {
                case 0:     //VIP特权
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnClickVIP;
                    break;
                case 1:     //充值
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnClickPay;
                    break;
                case 2:
                    {
                        UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnClickSkill;
                        break;
                    }
                case 3:
                    UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onClick = OnClickLocked;
                    break;
            }
            UIEventListener.Get(m_BaseTrans.GetChild(i).gameObject).onPress = OnPressMsg;
        }
        ResetFillAmount(.0f);
        InitLeftBtn();
        ResManager.Instance.UnloadObject(skillgo);
        UpdateLockedSkillSate();
       
    }
    void InitLeftBtn()
    {
        m_LeftBtnObj = m_BaseTrans.GetChild(4).gameObject;
        for (byte i = 0; i < 5; ++i )
        {
            UIEventListener.Get(m_BaseTrans.GetChild(4).GetChild(i).gameObject).onPress = OnPressMsg;
            if (i >= 2 && i < 4)
            {
                m_LeftUnfoldInfo[i - 2].m_TweenAnim = m_BaseTrans.GetChild(4).GetChild(i).GetComponent<TweenScale>();
                m_LeftUnfoldInfo[i - 2].m_Label = m_BaseTrans.GetChild(4).GetChild(i).GetChild(0).GetComponent<UILabel>();
                m_LeftUnfoldInfo[i - 2].m_ProgressSprite = m_BaseTrans.GetChild(4).GetChild(i).GetChild(1).GetComponent<UISprite>();
                m_LeftUnfoldInfo[i - 2].m_ProgressLabel = m_BaseTrans.GetChild(4).GetChild(i).GetChild(1).GetChild(0).GetComponent<UILabel>();
                m_LeftUnfoldInfo[i - 2].m_bPop = false;
                m_LeftUnfoldInfo[i - 2].m_bFish = false;
                m_LeftUnfoldInfo[i - 2].m_bShowLockedUI = false;
            }
            switch(i)
            {
                case 0:
                    m_UnLockBtnObj = m_BaseTrans.GetChild(4).GetChild(i).gameObject;
                    UIEventListener.Get(m_UnLockBtnObj).onClick = OnClickUnLocked;
                    break;
                case 1:
                    UIEventListener.Get(m_BaseTrans.GetChild(4).GetChild(i).gameObject).onClick = OnClickLottery;
                    break;
                case 2:
                    {
                        for (byte j = 0; j < m_BarAndReward.Length; ++j)
                        {
                            m_BarAndReward[j] = m_BaseTrans.GetChild(4).GetChild(i).GetChild(j + 1).gameObject;
                        }
                        m_LeftUnfoldInfo[0].m_RewardLabel = m_BaseTrans.GetChild(4).GetChild(i).GetChild(2).GetComponent<UILabel>();
                        UIEventListener.Get(m_BaseTrans.GetChild(4).GetChild(i).gameObject).onClick = OnClickUnlockedUnfoldUI;
                    }
                    break;
                case 3:
                    UIEventListener.Get(m_BaseTrans.GetChild(4).GetChild(i).gameObject).onClick = OnClickLotteryUnfoldUI;
                    break;
                case 4:
                    UIEventListener.Get(m_BaseTrans.GetChild(4).GetChild(i).GetChild(1).gameObject).onClick = OnClickBenefit;
                    break;
            }
        }
        m_benefit.Init(m_BaseTrans.GetChild(4).GetChild(4).transform,true);

        for (byte j = 0; j < m_EftObj.Length; ++j)
        {
            m_EftObj[j] = m_BaseTrans.GetChild(4).GetChild(5+j).gameObject;
        }
    }

    void ResetFillAmount(float amount)
    {
        for (int i = 0; i < 2; i++)
            m_CDSprite[i].fillAmount = amount;
    }
    public void Update(float deltaTime)
    {
        for (byte i = 0; i < m_CDList.Count;  )
        {
            if (UpdateCD(deltaTime, i))
                ++i;
        }
        for (byte j = 0; j < m_LeftUnfoldInfo.Length; ++j)
        {
            if (m_LeftUnfoldInfo[j].m_bShowLockedUI)
            {
                m_LeftUnfoldInfo[j].m_Elapsed -= deltaTime;
                if (m_LeftUnfoldInfo[j].m_Elapsed <= 0)
                {
                   // m_LeftUnfoldInfo[j].m_bShowLockedUI = false;
                    if (j == 0)
                    {
                        OnClickUnLocked(null);
                    }
                    else
                        OnClickLottery(null);
                }
            }
        }
        m_benefit.Update(deltaTime);
        if (m_EftElapsed > 0)
        {
            m_EftElapsed -= deltaTime;
            if (m_EftElapsed <= 0)
                m_EftObj[1].SetActive(true);

        }
    }
    bool UpdateCD(float deltaTime, byte Indx)
    {
        m_CDList[Indx].m_CurTime -= deltaTime;
        if (m_CDList[Indx].m_CurTime < 0)
            m_CDList[Indx].m_CurTime = 0;
        m_CDSprite[m_CDList[Indx].m_CDIndx].fillAmount = m_CDList[Indx].m_CurTime / m_CDList[Indx].m_CDTime;

        if (m_CDList[Indx].m_CurTime <= 0)
        {
            if (m_CDList[Indx].m_CDIndx == 1)
                SceneRuntime.PlayerMgr.SetLocked(false);
           // m_UIButton[m_CDList[Indx].m_CDIndx].isEnabled = true;
            if (m_CDList[Indx].m_bUseing)
                m_bUseing[m_CDList[Indx].m_CDIndx] = true;

            Utility.ListRemoveAt(m_CDList, Indx);
            return false;
        }
        return true;
    }
    void ShowRateReward()
    {
        tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID(BulletSetting.RateReward[PlayerRole.Instance.RoleInfo.GetRoleUnLockRateIndex()]);
        if (pReward != null)
        {
            for (byte j = 0; j < pReward.RewardItemVec.Count; ++j)
            {
                uint ItemID = pReward.RewardItemVec[j].ItemID;
                if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                    continue;
                m_LeftUnfoldInfo[0].m_RewardLabel.text = pReward.RewardItemVec[j].ItemSum.ToString();

            }
        }
    }
    public void UpdateUnLockRateReward(uint RewardID)
    {
        SceneRuntime.SceneLogic.EffectMgr.ShowUnLockRateReward(m_UnLockBtnObj.transform.position);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);

    }
    public void UpdateUnLockDataInfo()
    {
        //已经是最高倍率了没有要解锁的了
        if (PlayerRole.Instance.RoleInfo.GetRoleUnLockRateIndex() == 0)
        {
            if (m_UnLockBtnObj.activeSelf)
            {
                m_UnLockBtnObj.SetActive(false);
                m_LeftUnfoldInfo[0].m_TweenAnim.gameObject.SetActive(false);
                m_EftObj[1].SetActive(false);
                m_EftElapsed = 0;
            }
            return ;
        }
        m_LeftUnfoldInfo[0].m_Label.text = BulletSetting.BulletRate[PlayerRole.Instance.RoleInfo.GetRoleUnLockRateIndex()].ToString();
        uint DestDimond = BulletSetting.RateUnlock[PlayerRole.Instance.RoleInfo.GetRoleUnLockRateIndex()];
        uint CurDimond = PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency();
        if (CurDimond >= DestDimond)
        {
            CurDimond = DestDimond;
            m_LeftUnfoldInfo[0].m_bFish = true;
            if (!m_LeftUnfoldInfo[0].m_bShowLockedUI && !m_LeftUnfoldInfo[0].m_bPop)
            {
                OnClickUnLocked(null);
            }

            if (m_LeftUnfoldInfo[0].m_bShowLockedUI && !m_EftObj[1].activeSelf)
                m_EftObj[1].SetActive(true);

            m_BarAndReward[0].SetActive(false);
            m_BarAndReward[1].SetActive(true);
            ShowRateReward();
        }
        else
        {
            m_LeftUnfoldInfo[0].m_bFish = false;
            m_EftElapsed = 0;
            if (m_EftObj[1].activeSelf)
                m_EftObj[1].SetActive(false);
            m_BarAndReward[0].SetActive(true);
            m_BarAndReward[1].SetActive(false);
        }
        m_LeftUnfoldInfo[0].m_ProgressLabel.text = CurDimond.ToString() + "/" + DestDimond.ToString();
        m_LeftUnfoldInfo[0].m_ProgressSprite.fillAmount = (CurDimond*1.0f) / (DestDimond*1.0f);

    }
    public void UpdateLotteryInfo()
    {
        m_LeftUnfoldInfo[1].m_Label.text = PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryScore().ToString();
        byte curFishCount = PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryFishSum();
        byte MaxFishCout = FishConfig.Instance.m_LotteryConfig.MaxLotteryFishSum;
        if (curFishCount >= MaxFishCout)
        {
            curFishCount = MaxFishCout;
            m_LeftUnfoldInfo[1].m_bFish = true;
            if (!m_LeftUnfoldInfo[1].m_bShowLockedUI && m_LeftUnfoldInfo[1].m_bPop)
                OnClickLottery(null);
        }
        else
        {
            m_LeftUnfoldInfo[1].m_bFish = false;
            if (m_EftObj[2].activeSelf)
                m_EftObj[2].SetActive(false);
        }
        m_LeftUnfoldInfo[1].m_ProgressLabel.text = curFishCount.ToString() + "/" + MaxFishCout.ToString();
        m_LeftUnfoldInfo[1].m_ProgressSprite.fillAmount = curFishCount * 1.0f / MaxFishCout * 1.0f;
    }
    public void UpdateLockedSkillSate()
    {
        if (m_BaseWndObject == null)
            return;
        m_LockObj[1].SetActive(false);
        m_bUseing[1] = true;
        //if (PlayerRole.Instance.RoleVip.IsCanLauncherLocking())
        //{
        //   // m_UIButton[1].isEnabled = true;
        //    m_LockObj[1].SetActive(false);
        //    m_bUseing[1] = true;
        //}
        //else
        //{
        //    m_LockObj[1].SetActive(true);
        //   // m_UIButton[1].isEnabled = false;
        //    m_bUseing[1] = false;

        //}
    }
    //普通技能
    public void UpdateSkillState()
    {
        if (m_BaseWndObject == null)
            return;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() >= 2)
        {
            m_LockObj[0].SetActive(false);
            if (SceneRuntime.PlayerMgr.MySelf.Launcher.LauncherType >= 2)
            {
                byte Idx = LauncherSetting.LauncherDataList[SceneRuntime.PlayerMgr.MySelf.Launcher.LauncherType].nSkillBind;
                if (Idx != 255)
                {
                    m_CurSkillType = Idx;
                   // m_UIButton[0].isEnabled = true;
                    m_bUseing[0] = true;
                    if (m_UIButton[0].gameObject.activeSelf == false)
                    {
                        m_UIButton[0].gameObject.SetActive(true);
                        m_RightBtnTrans[0].localPosition = m_RightBtnPos[0];
                        m_RightBtnTrans[1].localPosition = m_RightBtnPos[1];
                    }
                    ChangeButtonBg(string.Format("Skill_Btn{0}", m_CurSkillType));
                }
            }
            else
            {
               // m_UIButton[0].isEnabled = false;
                m_bUseing[0] = false;
                //ChangeButtonBg("Skill_Btn2");
                m_UIButton[0].gameObject.SetActive(false);
                m_RightBtnTrans[1].localPosition = m_RightBtnPos[0];
            }
        }
        else
        {
            m_LockObj[0].SetActive(true);
            //m_UIButton[0].isEnabled = false;
            m_bUseing[0] = false;

        }
    }
    public void PlayCD(float time, byte Indx)
    {
        CDInfo cdInfo = new CDInfo();
        cdInfo.m_CDIndx = Indx;
        cdInfo.m_CDTime = time;
        cdInfo.m_CurTime = time;
        m_CDSprite[Indx].fillAmount = 1;
        //m_UIButton[Indx].isEnabled = false;
        if (m_bUseing[Indx])
        {
            cdInfo.m_bUseing = true;
            m_bUseing[Indx] = false;
        }
        else
            cdInfo.m_bUseing = false;

        m_CDList.Add(cdInfo);

    }
    public void HideLeftBtn()
    {
        m_LeftBtnObj.SetActive(false);
    }
    public void ChangeSkill(byte type)
    {

    }
    ushort ConsumeCount(SkillType skill)
    {
        byte byBaseCount = 0;
        List<SkillConsume> pRequire = SkillSetting.SkillDataList[(byte)skill].NumRequire;
        if (pRequire.Count != 0)
        {
            byte byIndex = 0;
            for (; byIndex < pRequire.Count; byIndex++)
            {
                if (m_nCountRecord[(byte)skill] < pRequire[byIndex].byorder)
                {
                    break;
                }
            }
            if (byIndex == pRequire.Count)
            {
                byIndex = (byte)(pRequire.Count - 1);
            }
            byBaseCount = pRequire[byIndex].byCount;
        }
        return (ushort)(byBaseCount);
    }
    public bool NumRequire(SkillType skill)
    {
        if (PlayerRole.Instance.ItemManager.GetAllItemMap() == null)//背包没数据
        {
            return false;
        }

        if (PlayerRole.Instance.ItemManager.GetItemSum(SkillSetting.SkillDataList[(byte)skill].ItemId, false) < ConsumeCount(skill))//个数不够
        {
            FishSkillToShop pSkillShop = FishConfig.Instance.m_FishScriptMap.GetSkillToShopInfoByID((byte)skill, 0);
            if (pSkillShop != null)
            {

                byte shopID = pSkillShop.ShopID;
                byte OnlyID = pSkillShop.ShopOnlyID;
                uint itemSum = ConsumeCount(skill) - PlayerRole.Instance.ItemManager.GetItemSum(SkillSetting.SkillDataList[(byte)skill].ItemId, false);
                tagShopConfig pShop = new tagShopConfig();
                FishConfig.Instance.m_ShopInfo.ShopMap.TryGetValue(shopID, out pShop);
                if (pShop != null)
                {
                    if (pShop.ShopItemMap.ContainsKey(OnlyID))
                        GlobalHallUIMgr.Instance.ShowConfirmBuyWnd(OnlyID, shopID, itemSum);
                }
            }
          //  GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Skill_Tip_count"), 1, false);          
            return false;
        }
        return true;        
    }
    public void RevertLockedFish()
    {
        if (m_CDSprite[1].fillAmount < 0.999f && m_CDSprite[1].fillAmount > 0.001f)
        {
            SceneRuntime.PlayerMgr.SetLocked(true);
        }
    }
    void OnClickSkill(GameObject go)
    {
        if (m_bUseing[0] == false)
        {
            if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() < 2)
            {
                GlobalEffectMgr.Instance.ShowMessageBox(string.Format(StringTable.GetString("VIP_UnLock_Tips"), 2), MssageType.VIP_UnLock_Msg_7);
            }
            return;
        }
        if (NumRequire((SkillType)m_CurSkillType) == false)
            return;
        SceneRuntime.SceneLogic.UseSkill((SkillType)m_CurSkillType);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_ClickSkill);

    }
    void OnClickVIP(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        GlobalHallUIMgr.Instance.ShowVipWnd();
    }
    void OnClickPay(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
    }
    void OnClickLocked(GameObject go)
    {
        if (m_bUseing[1] == false)
        {
            if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() < 2)
            {
                GlobalEffectMgr.Instance.ShowMessageBox(string.Format(StringTable.GetString("VIP_UnLock_Tips"), 2), MssageType.VIP_UnLock_Msg_7);
            }
            return;
        }

        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        if (NumRequire(SkillType.SKILL_LOCK) == false)
            return;
        if (SceneRuntime.PlayerMgr.MySelf.Launcher != null )
        {
            if (SceneRuntime.PlayerMgr.MySelf.Launcher.IsBankruptcy())
                return;
        }
        SceneRuntime.SceneLogic.UseSkill(SkillType.SKILL_LOCK);
    }
    void OnClickUnLocked(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        //不能解锁
        if (m_LeftUnfoldInfo[0].m_bShowLockedUI)
        {
            m_LeftUnfoldInfo[0].m_Elapsed = 0;
            m_LeftUnfoldInfo[0].m_TweenAnim.from = new Vector3(1, 1, 1);
            m_LeftUnfoldInfo[0].m_TweenAnim.to = new Vector3(0, 1, 1);
            if (SceneRuntime.PlayerMgr.MySelf.ComboEft != null)
                SceneRuntime.PlayerMgr.MySelf.ComboEft.SetCobmoPos(true);
            if (m_EftObj[1].activeSelf)
                m_EftObj[1].SetActive(false);
            if (!m_LeftUnfoldInfo[0].m_bFish)
                m_LeftUnfoldInfo[0].m_bPop = false;
        }
        else
        {
            m_LeftUnfoldInfo[0].m_TweenAnim.from = new Vector3(0, 1, 1);
            m_LeftUnfoldInfo[0].m_TweenAnim.to = new Vector3(1, 1, 1);
            m_LeftUnfoldInfo[0].m_Elapsed = 10;
            if (SceneRuntime.PlayerMgr.MySelf.ComboEft != null)
                SceneRuntime.PlayerMgr.MySelf.ComboEft.SetCobmoPos(false);

            if (m_LeftUnfoldInfo[0].m_bFish)
            {
                m_LeftUnfoldInfo[0].m_bPop = true;
                if (m_EftElapsed <= 0)
                    m_EftObj[1].SetActive(true);
            }
            else
            {
                m_EftObj[1].SetActive(false);
                m_EftElapsed = 0;
            }
            //UpdateUnLockDataInfo();
        }
        m_LeftUnfoldInfo[0].m_TweenAnim.ResetToBeginning();
        m_LeftUnfoldInfo[0].m_TweenAnim.PlayForward();
        m_LeftUnfoldInfo[0].m_bShowLockedUI = !m_LeftUnfoldInfo[0].m_bShowLockedUI;
    }
    void OnClickLottery(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        //不能解锁
        if (m_LeftUnfoldInfo[1].m_bShowLockedUI)
        {
            m_LeftUnfoldInfo[1].m_Elapsed = 0;
            m_LeftUnfoldInfo[1].m_TweenAnim.from = new Vector3(1, 1, 1);
            m_LeftUnfoldInfo[1].m_TweenAnim.to = new Vector3(0, 1, 1);
            if (m_EftObj[2].activeSelf)
                m_EftObj[2].SetActive(false);
        }
        else
        {
            m_LeftUnfoldInfo[1].m_TweenAnim.from = new Vector3(0, 1, 1);
            m_LeftUnfoldInfo[1].m_TweenAnim.to = new Vector3(1, 1, 1);
            m_LeftUnfoldInfo[1].m_Elapsed = 10;
            m_LeftUnfoldInfo[1].m_bPop = true;
            if (m_LeftUnfoldInfo[1].m_bFish)
                m_EftObj[2].SetActive(true);
            else
                m_EftObj[2].SetActive(false);
           // UpdateLotteryInfo();
        }
        m_LeftUnfoldInfo[1].m_TweenAnim.ResetToBeginning();
        m_LeftUnfoldInfo[1].m_TweenAnim.PlayForward();
        m_LeftUnfoldInfo[1].m_bShowLockedUI = !m_LeftUnfoldInfo[1].m_bShowLockedUI;
    }
    void OnClickUnlockedUnfoldUI(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        //能领解锁
        if (m_LeftUnfoldInfo[0].m_bFish)
        {
            PlayerRole.Instance.RoleInfo.ChangeRoleRateValue(PlayerRole.Instance.RoleInfo.GetRoleUnLockRateIndex());
            m_LeftUnfoldInfo[0].m_bPop = false;
            m_EftObj[0].SetActive(false);
            m_EftObj[0].SetActive(true);
            m_LeftUnfoldInfo[0].m_bShowLockedUI = false;
            m_EftElapsed = 0.2f;
            m_EftObj[1].SetActive(false);
            OnClickUnLocked(null);
            return;
        }
        else
        {
            //弹提示钻石不够框示框
            GlobalEffectMgr.Instance.ShowMessageBox("钻石", MssageType.GoldLack_Msg_4);

        }
    }
    void OnClickLotteryUnfoldUI(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        SceneRuntime.SceneLogic.LogicUI.ShowLotteryDraw();
        m_LeftUnfoldInfo[1].m_bPop = false;

    }
    void OnClickBenefit(GameObject go)
    {
        m_benefit.OnClick();
    }
    void OnPressMsg(GameObject obj, bool state)
    {
        if (state)
        {

        }
        else
        {
            SceneRuntime.SceneLogic.BtnsMgr.BaseBtnHide();

        }
    }
    void ChangeButtonBg(string strName)
    {
        m_UIButton[0].normalSprite = strName;
        m_UIButton[0].hoverSprite = strName;
        m_UIButton[0].pressedSprite = strName;
        m_UIButton[0].disabledSprite = strName;
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }

    }
    public void UpdateData()
    {
        if (PlayerRole.Instance.ItemManager.GetAllItemMap() == null)
        {
                 //m_sprCountTip.fillAmount = 0;                          
        }
        else
        {
            ushort nConsume = ConsumeCount((SkillType)m_CurSkillType);
            //m_SkillBtnLabel[i].text = 'x' + nConsume.ToString();
            if (nConsume == 0)
            {
               // m_sprCountTip.fillAmount = 1;
            }
            else
            {
               // m_sprCountTip.fillAmount = PlayerRole.Instance.ItemManager.GetItemSum(SkillSetting.SkillDataList[m_CurSkillType].ItemId, false) * 1.0f / nConsume;
            }                
        }  
    }
    public bool UnLockIsShow()
    {
        if (m_LeftUnfoldInfo[0].m_bShowLockedUI)
            return true;
        
        return false;
    }
    public int BarItemIndex(ushort nCardid)
    {
        for (int i = 0; i < SkillSetting.SkillDataList.Length; i++)
        {
            if (nCardid == SkillSetting.SkillDataList[i].ItemId)//是技能物品
            {
                return i;             
            }
        }
        return -1;
    }
    public byte CurSkillType
    {
        get { return m_CurSkillType; }
        set { m_CurSkillType = value; }
    }
}
