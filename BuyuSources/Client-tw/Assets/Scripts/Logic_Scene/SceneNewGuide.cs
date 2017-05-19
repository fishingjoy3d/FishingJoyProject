using UnityEngine;
using System.Collections.Generic;

public class SceneNewGuide : BaseWnd
{
    Object              m_Obj = null;
    UISprite            m_ItemIcon;
    UISprite            m_TimeProgress;
    UILabel             m_UITimeLabel;
    System.DateTime     m_EndTime = new System.DateTime();
    byte                m_LaunchType;
    bool                m_bItemChange = false;
    uint                m_RewardGuide = 0;
    float               m_LifeTime;
    float               m_LastTime;

    public void InitObj()
    {
        m_Obj = ResManager.Instance.LoadObject(("NewGuide0"), "SceneRes/Prefab/UI/NewGuide/", ResType.SceneRes);
    }
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(m_Obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_BaseWndObject.SetActive(true);
        m_TimeProgress = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_ItemIcon = m_BaseTrans.GetChild(1).GetComponent<UISprite>();
        m_UITimeLabel = m_BaseTrans.GetChild(2).GetComponent<UILabel>();

    }
    public void Update(float deltaTime)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        //System.TimeSpan Span = m_EndTime - SystemTime.Instance.GetSystemDateTime;
        //int min = (int)(Span.TotalSeconds % 3600) / 60;
        //int second = (int)(Span.TotalSeconds % 3600) % 60;
        //string strTime =  min.ToString("D2") + ":" + second.ToString("D2");
        //m_UITimeLabel.text = strTime;
        m_LastTime -= deltaTime;
        float fill = m_LastTime / m_LifeTime;
        m_TimeProgress.fillAmount = fill;
       // Debug.Log(m_LifeTime.ToString() + ": : " + m_LastTime.ToString() + "::"+ fill.ToString());

        if (m_LastTime <= 0)
        {
            //byte shopID = FishConfig.Instance.m_FishScriptMap.m_LauncherToShopMap[m_LaunchType].ShopID;
            //byte OnlyID = FishConfig.Instance.m_FishScriptMap.m_LauncherToShopMap[m_LaunchType].ShopOnlyID;
            //tagShopConfig pShop = new tagShopConfig();
            //FishConfig.Instance.m_ShopInfo.ShopMap.TryGetValue(shopID, out pShop);
            //if (pShop != null)
            //{
            //    if (pShop.ShopItemMap.ContainsKey(OnlyID))
            //        GlobalHallUIMgr.Instance.ShowConfirmBuyWnd(OnlyID, shopID, true);
            //}
            DestorySelf();
        }
    }
    public void CheckIsLauncherGuide()
    {
        if (!LoadGuide())
            m_RewardGuide = 0;
      //  Debug.Log(m_RewardGuide.ToString());

        if (m_RewardGuide > 0)
        {
            if (PlayerRole.Instance.ItemManager.GetAllItemMap() == null)
            {
                m_bItemChange = true;
                return;
            }
            ShowLaunchCountDown();
        }
    }
    public void SetLauncherGuide(uint RewardID)
    {
        tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID((ushort)RewardID);
        uint ItemID;
        if (pReward != null)
        {
            for (byte i = 0; i < pReward.RewardItemVec.Count; ++i)
            {
                ItemID = pReward.RewardItemVec[i].ItemID;
                if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) == true)
                {
                    for (byte j = 0; j < (byte)LauncherType.LAUNCHER_MAX; ++j)
                    {
                        if ((ushort)ItemID == LauncherSetting.LauncherDataList[j].nItemid)
                        {
                            //如果有效时间为0也不需要新手引导
                            m_LaunchType = j;
                            m_RewardGuide = RewardID;
                            SaveGuide();
                            SceneRuntime.SceneLogic.ChangeDestLauncher(m_LaunchType);
                            break;
                        }
                    }

                    break;
                }
            }
        }
    }
    public void ShowLaunchCountDown(bool bItemChange = false)
    {
        if (m_RewardGuide == 0)
            return;
        //只有当需要获取物品才接受物品改变消息
        if (bItemChange && !m_bItemChange)
            return;
        //该玩家不拥有这个炮台
        if (!PlayerRole.Instance.RoleLauncher.IsCanUseLauncher(m_LaunchType))
        {
            DestorySelf();
            return;
        }
        if (PlayerRole.Instance.RoleLauncher.IsCanUseLauncherByAllTime(m_LaunchType))
        {
            DestorySelf();
            return;
        }
        uint EndSec = 0;
        if(!PlayerRole.Instance.RoleLauncher.GetLauncherEndTime(m_LaunchType,out EndSec))
        {
            DestorySelf();
            return;
        }
        m_LastTime = EndSec;
        if (m_LastTime == 0)
        {
            DestorySelf();
            return;
        }
        if (m_BaseWndObject == null)
            Init();
        tagRewardOnce pReward = FishConfig.Instance.GetRewardOnceByID((ushort)m_RewardGuide);
        for (byte i = 0; i < pReward.RewardItemVec.Count; ++i)
        {
            m_LifeTime = pReward.RewardItemVec[i].LastMin* 60;
        }
        ushort ItemID = LauncherSetting.LauncherDataList[m_LaunchType].nItemid;
        tagItemConfig pItem = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID];
        m_ItemIcon.spriteName = pItem.ItemIcon;
        m_EndTime = System.DateTime.Now.AddSeconds(m_LifeTime);
        SceneRuntime.SceneLogic.ChangeDestLauncher(m_LaunchType);
        SaveGuide();
        
    }
    void DestorySelf()
    {
        m_RewardGuide = 0;
        m_LaunchType = 0;
        SaveGuide();
        if (m_BaseWndObject != null)
        {
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
        ResManager.Instance.UnloadObject(m_Obj);
    }

    public byte LaunchType
    {
        get { return m_LaunchType; }
        set { m_LaunchType = value; }
    }
    public bool LoadGuide()
    {
        //读取SD卡
        byte[] data = RuntimeInfo.GetLocalFile(RuntimeInfo.GetNewGuideFileName());
        if (data == null)
            return false;

        System.IO.MemoryStream ms = new System.IO.MemoryStream(data);
        System.IO.BinaryReader br = new System.IO.BinaryReader(ms);
        m_LaunchType = br.ReadByte();
        m_RewardGuide = br.ReadUInt32();
        uint UserID = br.ReadUInt32();
        ms.Close();
        br.Close();
        if (UserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            return false;
        return true;
    }
    //位置保存到SDK里
    public void SaveGuide()
    {
        //byte data;
        System.IO.MemoryStream ms = new System.IO.MemoryStream();
        System.IO.BinaryWriter bw = new System.IO.BinaryWriter(ms);
        bw.Write(m_LaunchType);
        bw.Write(m_RewardGuide);
        bw.Write(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
        RuntimeInfo.SaveLocalFile(RuntimeInfo.GetNewGuideFileName(), ms.ToArray());
        bw.Close();
    }

}
