using UnityEngine;
using System.Collections;

public class MonthCardLogicUI : BaseWnd
{
    Object m_Obj = null;
    UIButton m_UIButton;

    public void InitObj()
    {
        m_Obj = ResManager.Instance.LoadObject("MonthCardWnd", "ExtraRes/UI/Predab/Vip/", ResType.ExtraRes, false);

    }
    void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(m_Obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_UIButton = m_BaseTrans.GetChild(1).GetComponent<UIButton>();
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnCloseWnd;
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = OnGetReward;
        UpdateMonthCardData();
    }
    void OnGetReward(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (PlayerRole.Instance.RoleMonthCard.IsInMonthCard())
        {
            PlayerRole.Instance.RoleMonthCard.GetRoleMonthCardReward();
        }
        else
        {
            uint ItemID = 13;
            if (SDKMgr.IS_SDK_CHANNEL || SDKMgr.IS_APP_STORE_VER)
            {
                System.UInt64 user_item_id = PlayerRole.Instance.RoleInfo.RoleMe.GetUserID();
                user_item_id = (user_item_id << 32) | ItemID;
                if (FishConfig.Instance.m_FishRecharge.m_FishRechargeMap.ContainsKey(ItemID))
                {
                    int price = (int)FishConfig.Instance.m_FishRecharge.m_FishRechargeMap[13].dDisCountPrice * 100;
                    SDKMgr.Instance.SDK.Pay(price, "月卡", 1, ItemID.ToString(), user_item_id.ToString(), (int)ItemID);
                }
            }
            else
            {
                PlayerRole.Instance.RoleRecharge.SendRecharge(ItemID, SDKMgr.Instance.IsServerOrder);
            }

        }
    }
    void OnCloseWnd(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        DestoryWnd();
    } 
    void ChangeButtonBg(string str)
    {
        m_UIButton.normalSprite = str;
        m_UIButton.hoverSprite = str;
        m_UIButton.pressedSprite = str;
        m_UIButton.disabledSprite = str;
    }
    public void UpdateMonthCardData()
    {
        if (m_BaseWndObject == null)
            return;
        if (PlayerRole.Instance.RoleMonthCard.IsInMonthCard())
        {
            if (PlayerRole.Instance.RoleMonthCard.IsCanGetRoleMonthCardReward())
            {
                m_UIButton.isEnabled = true;
                ChangeButtonBg("MonthCard_GetRewardBtn0");
            }
            else
            {
                m_UIButton.isEnabled = false;
                ChangeButtonBg("MonthCard_GetRewardBtn1");
            }
        }
    }
    public void ShowMonthCardWnd()
    {
        if (m_BaseWndObject == null)
        {
            Init();
            WndManager.Instance.Push(m_BaseTrans);
        }
    }
    void DestoryWnd()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseTrans);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
    public void ShutDown()
    {
        DestoryWnd();
    }

}
