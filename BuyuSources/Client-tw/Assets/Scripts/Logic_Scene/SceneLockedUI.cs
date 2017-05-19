using UnityEngine;
using System.Collections.Generic;

public class SceneLockedUI : BaseWnd
{
    TweenScale      m_TweenScaleAnim;
    float           m_LifeTime;


	
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(SceneRuntime.PlayerMgr.LockedObj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        m_BaseTrans.position = FishScreenToNGUIPos();
        m_TweenScaleAnim = m_BaseTrans.GetComponent<TweenScale>();
        m_TweenScaleAnim.enabled = true;
    }
    Vector3 FishScreenToNGUIPos()
    {
        Vector2 scernPos = new Vector2();
        SceneRuntime.FishMgr.GetFishScreenPos(SceneRuntime.PlayerMgr.LockedFishID, out scernPos);

        Vector3 worldPos = new Vector3(scernPos.x, scernPos.y, 0);
        return SceneObjMgr.Instance.UICamera.ScreenToWorldPoint(worldPos);
    }
    public void UpdateLockedUI()
    {
        if (SceneRuntime.PlayerMgr.AutoLocked)
        {
            if (m_BaseWndObject != null)
            {
                if (SceneRuntime.PlayerMgr.LockedFishID == 0)
                    m_BaseWndObject.SetActive(false);
                else
                {
                    m_BaseWndObject.SetActive(true);
                    m_BaseTrans.position = FishScreenToNGUIPos();
                }
            }
            else
            {
                Init();
            }
        }
        else
            DestoryUI();

    }
    public void Update(float deltaTime)
    {
        if (m_LifeTime > 0)
        {
            m_LifeTime -= deltaTime;
        }
        else
            DestoryUI();
    }
    public void ShowOtherUserLocked(byte clientSeat)
    {
        m_BaseWndObject = GameObject.Instantiate(SceneRuntime.PlayerMgr.LockedObj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (clientSeat < 2)
        {
            m_BaseTrans.position = new Vector3(SceneRuntime.PlayerMgr.GetPlayer(clientSeat).Launcher.LauncherPos.x, 
                SceneRuntime.PlayerMgr.GetPlayer(clientSeat).Launcher.LauncherPos.y + 0.1f, 0);
        }
        else
        {
            m_BaseTrans.position = new Vector3(SceneRuntime.PlayerMgr.GetPlayer(clientSeat).Launcher.LauncherPos.x,
                SceneRuntime.PlayerMgr.GetPlayer(clientSeat).Launcher.LauncherPos.y - 0.1f, 0);
        }

        m_LifeTime = SkillSetting.SkillDataList[(byte)SkillType.SKILL_LOCK].CDTime;
    }
    public void DestoryUI()
    {
        if (m_BaseWndObject != null)
        {
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
    public void ShutDown()
    {
        DestoryUI();
    }
}
