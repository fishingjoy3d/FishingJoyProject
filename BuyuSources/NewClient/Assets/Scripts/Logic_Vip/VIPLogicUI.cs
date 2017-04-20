using UnityEngine;
using System.Collections.Generic;

public class VIPLogicUI : BaseWnd
{
    Object              m_Obj = null;
    GameObject[]        m_ChildObj = new GameObject[2];
    UILabel[]           m_VipLevel = new UILabel[2];             //VIP等级
    UILabel             m_VipPayProgressLabel;  //Vip充值进度
    UISprite            m_VipPayProgressSprite; //Vip充值进度
    UILabel             m_VipTips;
    GameObject[]        m_GridBaseItem = new GameObject[2];
    UIGrid[]            m_UIGrid = new UIGrid[2];
    TweenScale[]        m_WndTweenAnim = new TweenScale[2];

    public void InitObj()
    {
        m_Obj = ResManager.Instance.LoadObject("VipPrivilege", "ExtraRes/UI/Predab/Vip/", ResType.ExtraRes, false);
    }
    void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(m_Obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        for (byte i = 0; i < m_ChildObj.Length; ++i)
            m_ChildObj[i] = m_BaseTrans.GetChild(i).gameObject;

        InitVipPrivilege();
        InitVipPrivilegeDetail();

        UpdateVipPayProgressInfo();
        UpdateVipScrollData();
    }
    void InitVipPrivilege()
    {
        Transform baseTrans = m_ChildObj[0].transform;
        m_WndTweenAnim[0] = baseTrans.GetComponent<TweenScale>();
        m_VipLevel[0] = baseTrans.GetChild(4).GetComponent<UILabel>();
        m_VipLevel[1] = baseTrans.GetChild(5).GetComponent<UILabel>();
        m_VipPayProgressLabel = baseTrans.GetChild(3).GetChild(0).GetComponent<UILabel>();
        m_VipPayProgressSprite = baseTrans.GetChild(3).GetComponent<UISprite>();
        m_VipTips = baseTrans.GetChild(6).GetComponent<UILabel>();
        m_GridBaseItem[0] = baseTrans.GetChild(7).GetChild(0).gameObject;
        m_UIGrid[0] = baseTrans.GetChild(7).GetChild(1).GetComponent<UIGrid>();

        UIEventListener.Get(baseTrans.GetChild(0).gameObject).onClick = OnCloseVipPrivilege;
        UIEventListener.Get(baseTrans.GetChild(1).gameObject).onClick = OnOpenVipPrivilegeDetail;
        UIEventListener.Get(baseTrans.GetChild(2).gameObject).onClick = OnClickPay;
    }
    void InitVipPrivilegeDetail()
    {
        Transform baseTrans = m_ChildObj[1].transform;
        m_WndTweenAnim[1] = baseTrans.GetComponent<TweenScale>();
        m_GridBaseItem[1] = baseTrans.GetChild(1).GetChild(0).gameObject;
        m_UIGrid[1] = baseTrans.GetChild(1).GetChild(1).GetComponent<UIGrid>();
        UIEventListener.Get(baseTrans.GetChild(0).gameObject).onClick = OnCloseVipPrivilegeDetail;
    }
    public void UpdateVipPayProgressInfo()
    {
        if (m_BaseWndObject == null)
            return;
        UpdateVipScrollData();
        m_VipLevel[0].text = PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel().ToString();
        tagVipOnce pVip = null;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() == 0)
        {
            pVip = FishConfig.Instance.m_VipConfig.VipMap[1];
            m_VipLevel[1].text = pVip.VipLevel.ToString();
            uint needPay = FishConfig.Instance.m_VipConfig.VipMap[pVip.VipLevel].NeedRechatgeRMBSum;
            m_VipPayProgressLabel.text = string.Format("{0}/{1}", PlayerRole.Instance.RoleInfo.RoleMe.GetTotalRechargeSum(), needPay);
            m_VipPayProgressSprite.fillAmount = PlayerRole.Instance.RoleInfo.RoleMe.GetTotalRechargeSum() * 1.0f / needPay * 1.0f;
            uint sub = 0;
            if (needPay >= PlayerRole.Instance.RoleInfo.RoleMe.GetTotalRechargeSum())
                sub = needPay - PlayerRole.Instance.RoleInfo.RoleMe.GetTotalRechargeSum();
            m_VipTips.text = string.Format(StringTable.GetString("VIP_Pay_Sum"), sub, pVip.VipLevel);
            return;
        }
        else
            pVip = FishConfig.Instance.m_VipConfig.VipMap[PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel()];

        if (pVip.UpperVipLevel == 0)
        {
            m_VipLevel[1].text = pVip.VipLevel.ToString();
            m_VipPayProgressLabel.text = string.Format("{0}/{1}", PlayerRole.Instance.RoleInfo.RoleMe.GetTotalRechargeSum(), pVip.NeedRechatgeRMBSum );
            m_VipPayProgressSprite.fillAmount = 1;
            m_VipTips.text = StringTable.GetString("VIP_Pay_Sum1");
        }
        else
        {
            m_VipLevel[1].text = pVip.UpperVipLevel.ToString();
            uint needPay = FishConfig.Instance.m_VipConfig.VipMap[pVip.UpperVipLevel].NeedRechatgeRMBSum;
            m_VipPayProgressLabel.text = string.Format("{0}/{1}", PlayerRole.Instance.RoleInfo.RoleMe.GetTotalRechargeSum(), needPay);
            m_VipPayProgressSprite.fillAmount = PlayerRole.Instance.RoleInfo.RoleMe.GetTotalRechargeSum() * 1.0f / needPay * 1.0f;
            uint sub = 0;
            if (needPay >= PlayerRole.Instance.RoleInfo.RoleMe.GetTotalRechargeSum())
                sub = needPay - PlayerRole.Instance.RoleInfo.RoleMe.GetTotalRechargeSum();
            m_VipTips.text = string.Format(StringTable.GetString("VIP_Pay_Sum"), sub, pVip.UpperVipLevel);

        }
    }
    void UpdateVipScrollData()
    {
        ClearGird(0);
        foreach(KeyValuePair<byte, tagVipOnce>map in FishConfig.Instance.m_VipConfig.VipMap)
        {
            VIPLaunchItem item = new VIPLaunchItem();
            item.Init(m_GridBaseItem[0]);
            item.ShowVipInfo(map.Value);
            m_UIGrid[0].AddChild(item.m_BaseTrans);
            item.m_BaseTrans.localScale = Vector3.one;
        }
    }
    void UpdateVipDetailScrollData()
    {
        ClearGird(1);
        foreach (KeyValuePair<byte, tagVipOnce> map in FishConfig.Instance.m_VipConfig.VipMap)
        {
            VIPDetailItem item = new VIPDetailItem();
            item.Init(m_GridBaseItem[1]);
            item.ShowVipInfo(map.Value);
            m_UIGrid[1].AddChild(item.m_BaseTrans);
            item.m_BaseTrans.localScale = Vector3.one;
        }
    }
    public void ShowVipWnd()
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseWndObject);
        }
    }
    public void ShowVipDetail()
    {
        m_ChildObj[0].SetActive(false);
        m_ChildObj[1].SetActive(true);
        WndManager.Instance.Push(m_ChildObj[1]);
        UpdateVipDetailScrollData();
        m_WndTweenAnim[1].ResetToBeginning();
        m_WndTweenAnim[1].PlayForward();
    }
    void OnCloseVipPrivilege(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        ShutDown();
    }
    void OnOpenVipPrivilegeDetail(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        ShowVipDetail();
    }
    void OnClickPay(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
    }
    void OnCloseVipPrivilegeDetail(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        m_ChildObj[0].SetActive(true);
        m_ChildObj[1].SetActive(false);
        WndManager.Instance.Pop(m_ChildObj[1]);
        m_WndTweenAnim[0].ResetToBeginning();
        m_WndTweenAnim[0].PlayForward();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
       
    }
    void ClearGird(byte Idx)
    {
        List<Transform> gridChid = m_UIGrid[Idx].GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_UIGrid[Idx].transform.DetachChildren();
    }
}

public class VIPLaunchItem : BaseWnd
{
    UILabel         m_VipLevel;
    UISprite        m_LaunchIcon;
    UISprite        m_LaunchName;
    UILabel         m_DescLabel;
    UILabel         m_PayLabel;
    UISprite[]      m_SkillIcon = new UISprite[3];
    GameObject      m_Locked;

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseWndObject.SetActive(true);

        m_LaunchIcon = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_VipLevel = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        m_LaunchName = m_BaseTrans.GetChild(2).GetComponent<UISprite>();
        m_DescLabel = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
        m_PayLabel = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        m_Locked = m_BaseTrans.GetChild(5).gameObject;
        for (byte i = 0; i < m_SkillIcon.Length; ++i)
        {
            m_SkillIcon[i] = m_BaseTrans.GetChild(6).GetChild(i).GetComponent<UISprite>();
        }
        UIEventListener.Get(m_BaseWndObject).onClick = OnOpenDetail;
    }
    public void ShowVipInfo(tagVipOnce pVipInfo)
    {
        m_VipLevel.text = pVipInfo.VipLevel.ToString();
        m_DescLabel.text = pVipInfo.VipDec;
        m_PayLabel.text = pVipInfo.VipName ;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() >= pVipInfo.VipLevel)
            m_Locked.SetActive(false);
        
        if (pVipInfo.VipLevel >= 5)
        {
            m_LaunchIcon.spriteName = "1305";
            m_LaunchName.spriteName = "Launch_Name5";
            m_DescLabel.text = "敬请期待";
            m_PayLabel.text = "累计充值?元";
            return ;
        }
        //获取炮台类型
        byte LaunchType = 0;
        foreach(KeyValuePair<byte , byte>map in pVipInfo.CurrceyLevel.CanUseLauncher)
        {
            LaunchType = map.Key;
            break;
        }
        if (LaunchType >= ConstValue.MAX_LAUNCHER_NUM)
            return;
        m_LaunchName.spriteName = string.Format("Launch_Name{0}", LaunchType);
        uint ItemID = LauncherSetting.LauncherDataList[LaunchType].nItemid;

        if (ItemID != 0 && FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) == false)
            return;
        tagItemConfig pItem = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID];
        m_LaunchIcon.spriteName = pItem.ItemIcon;
        
        //VIP解锁的功能
        byte Indx = 0;
        //是否有反弹特性
        if (pVipInfo.CurrceyLevel.LauncherReBoundNum > 0)
        {
            m_SkillIcon[Indx].spriteName = "Rebound";
            m_SkillIcon[Indx].gameObject.SetActive(true);
            Indx++;
        }
        //是否有锁定
        m_SkillIcon[Indx].spriteName = "Locked";
        m_SkillIcon[Indx].gameObject.SetActive(true);
        Indx++;

        //if (pVipInfo.CurrceyLevel.IsCanLauncherLocking)
        //{
            
        //}
        if (LauncherSetting.LauncherDataList[LaunchType].nSkillBind != 255)
        {
            m_SkillIcon[Indx].spriteName = string.Format("skill_{0}", LauncherSetting.LauncherDataList[LaunchType].nSkillBind);
            m_SkillIcon[Indx].gameObject.SetActive(true);
            Indx++;
        }
        
    }
    void OnOpenDetail(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        GlobalHallUIMgr.Instance.ShowVipDetailWnd();
    }
}

public class VIPDetailItem : BaseWnd
{
    UILabel     m_VipLevel;
    UISprite    m_LaunchIcon;
    UILabel[]  m_DescLabel = new UILabel[6];
   

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseWndObject.SetActive(true);

        m_LaunchIcon = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_VipLevel = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        for (byte i = 0; i < m_DescLabel.Length; ++i)
        {
            m_DescLabel[i] = m_BaseTrans.GetChild(i + 2).GetComponent<UILabel>();
        }

    }
    public void ShowVipInfo(tagVipOnce pVipInfo)
    {
        m_VipLevel.text = pVipInfo.VipLevel.ToString();

        if (pVipInfo.VipLevel >= 5)
        {
            m_LaunchIcon.spriteName = "1305";
            m_DescLabel[0].text = "免费获得 ? 炮";
            m_DescLabel[1].text = "开启 ? 技能";
            return;
        }

        //获取炮台类型
        byte LaunchType = 0;
        foreach (KeyValuePair<byte, byte> map in pVipInfo.CurrceyLevel.CanUseLauncher)
        {
            LaunchType = map.Key;
            break;
        }
        uint ItemID = LauncherSetting.LauncherDataList[LaunchType].nItemid;

        if (ItemID != 0 && FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) == false)
            return;
        tagItemConfig pItem = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID];
        m_LaunchIcon.spriteName = pItem.ItemIcon;
        byte Idx = 0; 

        m_DescLabel[Idx++].text = string.Format(StringTable.GetString("VIP_Privilege_Describe0"), LauncherSetting.LauncherDataList[LaunchType].name);
        //if (pVipInfo.CurrceyLevel.IsCanLauncherLocking)
        //{
            
        //}
        if (LaunchType == 1)
            m_DescLabel[Idx++].text = string.Format(StringTable.GetString("VIP_Privilege_Describe1"));

        if (LauncherSetting.LauncherDataList[LaunchType].nSkillBind != 255)
        {
            byte SikllIdx = LauncherSetting.LauncherDataList[LaunchType].nSkillBind;
            m_DescLabel[Idx++].text = string.Format(StringTable.GetString("VIP_Privilege_Describe2"), SkillSetting.SkillDataList[SikllIdx].name);
        }
        if (pVipInfo.TotalLevel.LauncherReBoundNum > 0)
        {
            m_DescLabel[Idx++].text = string.Format(StringTable.GetString("VIP_Privilege_Describe3"), pVipInfo.TotalLevel.LauncherReBoundNum);
        }
        if (pVipInfo.TotalLevel.AddAlmsSum > 0)
        {
            m_DescLabel[Idx++].text = string.Format(StringTable.GetString("VIP_Privilege_Describe4"), pVipInfo.TotalLevel.AddAlmsSum);
        }
        if (pVipInfo.TotalLevel.AddAlmsRate > 0)
        {
            m_DescLabel[Idx++].text = string.Format(StringTable.GetString("VIP_Privilege_Describe5"), (1 + (pVipInfo.TotalLevel.AddAlmsRate * 1.0f / 100 * 1.0f)));
        }
        if (Idx >= m_DescLabel.Length)
            return;
        if (pVipInfo.TotalLevel.AddMonthScoreRate > 0)
        {
            m_DescLabel[Idx++].text = string.Format(StringTable.GetString("VIP_Privilege_Describe6"), pVipInfo.TotalLevel.AddMonthScoreRate);
        }
        if (Idx >= m_DescLabel.Length)
            return;
        if (pVipInfo.TotalLevel.AddUseMedalSum > 0)
        {
            m_DescLabel[Idx++].text = string.Format(StringTable.GetString("VIP_Privilege_Describe7"), pVipInfo.TotalLevel.AddUseMedalSum);

        }
        if (Idx >= m_DescLabel.Length)
            return;
        if (pVipInfo.TotalLevel.AddReChargeRate > 0)
        {
            m_DescLabel[Idx++].text = string.Format(StringTable.GetString("VIP_Privilege_Describe8"), pVipInfo.TotalLevel.AddReChargeRate);

        }
    }
}