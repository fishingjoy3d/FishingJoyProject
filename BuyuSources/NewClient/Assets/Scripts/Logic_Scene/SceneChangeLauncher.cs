using UnityEngine;
using System.Collections.Generic;

public enum LaunchState
{
    AlreadyEquip = 0,       //已装备
    AlreadyGet,             //已拥有
    WithOutGet,             //未获得
}

public class SceneChangeLauncher : BaseWnd
{
    UIGrid      m_UIGrid;
    GameObject  m_BaseChild;
    Object      m_Obj;

    public void InitObj()
    {
        m_Obj = ResManager.Instance.LoadObject("ChangeLauncherWnd", "SceneRes/Prefab/UI/ChangeLauncher/", ResType.SceneRes);

    }
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(m_Obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_BaseChild = m_BaseTrans.GetChild(1).GetChild(0).gameObject;
        m_UIGrid = m_BaseTrans.GetChild(1).GetChild(1).GetComponent<UIGrid>();
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnCloseWnd;
        InitLauncherList();
        WndManager.Instance.Push(m_BaseTrans);
    }
    public void ShowChangeLauncherWnd()
    {
        if (m_BaseWndObject == null)
            Init();
    }
    public void UpdateLaunchData()
    {
        if (m_BaseWndObject == null)
            return;
        InitLauncherList();
    }
    void InitLauncherList()
    {
        ClearGird();
        for (byte i = 0; i < (byte)LauncherType.LAUNCHER_MAX; ++i)
        {
            
                uint ItemID = LauncherSetting.LauncherDataList[i].nItemid;

                if (ItemID != 0 && FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) == false)
                    continue;
                tagItemConfig pItem = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID];
                //uint validTime = 0;
                //永久有效的
                //if (PlayerRole.Instance.RoleLauncher.IsCanUseLauncherByAllTime(i))
                //    validTime = 0;
                ////限时有效
                //else
                //{
                //    if (!PlayerRole.Instance.RoleLauncher.GetLauncherEndTime(i, out validTime))
                //        continue;
                //}
                
            
                LauncherItem item = new LauncherItem();
                item.Init(m_BaseChild);
                if (PlayerRole.Instance.RoleLauncher.IsCanUseLauncher(i))
                {
                    if (i == SceneRuntime.PlayerMgr.MySelf.Launcher.LauncherType)
                        item.ShowLaunchInfo(LaunchState.AlreadyEquip, pItem.ItemIcon, i);
                    else
                        item.ShowLaunchInfo(LaunchState.AlreadyGet, pItem.ItemIcon, i);
                }
                else
                    item.ShowLaunchInfo(LaunchState.WithOutGet, pItem.ItemIcon, i);

                m_UIGrid.AddChild(item.m_BaseTrans);
                item.ResetScale();
          
        }
    }
    void OnCloseWnd(GameObject go)
    {
        DestoryWnd();
    }
    void ClearGird()
    {
        List<Transform> gridChid = m_UIGrid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_UIGrid.transform.DetachChildren();
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
        ResManager.Instance.UnloadObject(m_Obj);
    }
}

public class LauncherItem : BaseWnd
{
    UISprite        m_LauncherIcon;
    UISprite        m_LauncherName;
    GameObject      m_Locked;
    UIButton        m_UIButton;
    UISprite        m_ButtonBg;
    LaunchState     m_State;
    byte            m_LaunchType;

    public void Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseWndObject.SetActive(true);
        m_LauncherIcon = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_LauncherName = m_BaseTrans.GetChild(1).GetComponent<UISprite>();
        m_Locked = m_BaseTrans.GetChild(2).gameObject;
        m_UIButton = m_BaseTrans.GetChild(3).GetComponent<UIButton>();
        m_ButtonBg = m_BaseTrans.GetChild(3).GetChild(0).GetComponent<UISprite>();
        UIEventListener.Get(m_UIButton.gameObject).onClick = OnClickButton;
    }
    public void ResetScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void ShowLaunchInfo(LaunchState state, string LaunchIcon, byte LaunchType)
    {
        m_State = state;
        m_LaunchType = LaunchType;
        m_LauncherIcon.spriteName = LaunchIcon;
        m_LauncherName.spriteName = string.Format("Launch_Name{0}", LaunchType);
        if (state == LaunchState.AlreadyEquip)
        {
            m_UIButton.isEnabled = false;
            m_ButtonBg.spriteName = "Change_Launch_Btn2";
            ChangeButtonBg("Btn_Bg_2");
            m_Locked.SetActive(false);
        }
        else if (state == LaunchState.AlreadyGet)
        {
            m_Locked.SetActive(false);
            m_UIButton.isEnabled = true;
            m_ButtonBg.spriteName = "Change_Launch_Btn1";
            ChangeButtonBg("Btn_Bg_0");
        }
        else
        {
            m_UIButton.isEnabled = true;
            m_Locked.SetActive(true);
            m_ButtonBg.spriteName = "Change_Launch_Btn0";
            ChangeButtonBg("Btn_Bg_1");
        }
    }
    void ChangeButtonBg(string strName)
    {
        m_UIButton.normalSprite = strName;
        m_UIButton.hoverSprite = strName;
        m_UIButton.pressedSprite = strName;
        m_UIButton.disabledSprite = strName;
    }
    void OnClickButton(GameObject go)
    {
        if (m_State == LaunchState.WithOutGet)
        {
            GlobalHallUIMgr.Instance.ShowVipWnd();
        }
        else if (m_State == LaunchState.AlreadyGet)
        {
            SceneRuntime.SceneLogic.ChangeDestLauncher(m_LaunchType);
        }
    }
}