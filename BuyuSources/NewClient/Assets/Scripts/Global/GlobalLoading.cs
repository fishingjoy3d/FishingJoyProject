using UnityEngine;
using System.Collections.Generic;

public class GlobalLoading:Singleton<GlobalLoading>
{
    GameObject m_EffectObj;
    GameObject m_Progressbar;
    GameObject m_ProgressEffect;
    UISlider m_ProgressSlider;
    UILabel m_ProgressLabel;
    float m_ProgressValue;
    bool m_bShow;
    public void GlobalInit()
    {
        m_ProgressValue = 0;
        m_bShow = false;
        Object go = Resources.Load("InnerRes/UI/Progress_bar");
        Object eft = Resources.Load("InnerRes/Effect/Progressbar_Effect");
        m_EffectObj = GameObject.Instantiate(eft) as GameObject;
        m_Progressbar = GameObject.Instantiate(go) as GameObject;
        m_Progressbar.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_EffectObj.transform.SetParent(m_Progressbar.transform.GetChild(2), false);
        m_ProgressEffect = m_EffectObj.transform.GetChild(0).gameObject;
        m_ProgressEffect.SetActive(false);
        GlobalEffectMgr.SetEffectOnUI(m_EffectObj);
        m_ProgressSlider = m_Progressbar.transform.GetComponent<UISlider>();
        m_ProgressLabel = m_Progressbar.transform.GetChild(1).GetComponent<UILabel>();
        m_ProgressLabel.text = StringTable.GetString("res_loading");
        m_Progressbar.SetActive(false);
        m_EffectObj.SetActive(false);
    }
    public bool IsShow
    {
        get
        {
            return m_bShow && m_Progressbar != null;
        }
    }
    public void Show()
    {
        m_bShow = true;
        m_Progressbar.SetActive(true);
        m_EffectObj.SetActive(true);
    }
    public void Update(float delta)
    {
        if (m_Progressbar == null || m_bShow == false) 
            return;

        float progress;
        m_ProgressValue += delta * 0.1f;
        progress = m_ProgressValue;
        if (progress > 1.0f)
            progress = 1.0f;
        m_ProgressSlider.value = progress;
        if (progress >= 0.1f && !m_ProgressEffect.activeSelf)
            m_ProgressEffect.SetActive(true);
        else if (progress < 0.1f && m_ProgressEffect.activeSelf)
            m_ProgressEffect.SetActive(false);
    }
    public void Hide()
    {
        if (m_EffectObj != null)
        {
            GameObject.Destroy(m_EffectObj);
            m_EffectObj = null;
        }
        if (m_Progressbar != null)
        {
            GameObject.Destroy(m_Progressbar);
            m_Progressbar = null;
        }
    }
}
