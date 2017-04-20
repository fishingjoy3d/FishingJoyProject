using System.Collections.Generic;
using UnityEngine;
public class UpdateLogicUI:ILogicUI
{
    string m_Info;
    Color m_LineColor = new Color(0.2f, 0.7f, 0.2f, 0.5f);
    Color m_BackColor = new Color(0.1f, 0.1f, 0.1f, 0.5f);
    Color m_FrontColor = new Color(0.8f, 0.4f, 0.0f, 0.8f);
    bool m_bBeginDown;
    UpdateLogic m_Logic;

    GameObject      m_Progressbar;
    GameObject      m_ProgressEffect;
    UISlider        m_ProgressSlider;
    UILabel         m_ProgressLabel;
    string          m_str;
    float           m_ProgressValue;
    bool            m_IsInQueue = false;
    public void SetLoadingType( bool unzpping)
    {
        m_ProgressValue = 0;
        m_ProgressSlider.value = 0.05f;
        m_ProgressEffect.SetActive(true);
        if (unzpping)
        {
            m_bBeginDown = true;
            m_str = StringTable.GetString("fistunzpping");
        }
        else
        {
            m_bBeginDown = false;
            m_str = StringTable.GetString("check_wait");
        }
    }
    public bool Init(ILogic logic, object obj)
    {
        //Screen.sleepTimeout = SleepTimeout.NeverSleep;
        m_bBeginDown = false;
        m_Logic = (UpdateLogic)logic;
        SceneObjMgr.Instance.LoadInnerUpdateBackMat();

        Object go = Resources.Load("InnerRes/UI/Progress_bar");
        Object eft = Resources.Load("InnerRes/Effect/Progressbar_Effect");
        GameObject eftObj = GameObject.Instantiate(eft) as GameObject;
        m_Progressbar = GameObject.Instantiate(go) as GameObject;
        m_Progressbar.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        eftObj.transform.SetParent(m_Progressbar.transform.GetChild(2), false);
        m_ProgressEffect = eftObj.transform.GetChild(0).gameObject;
        m_ProgressEffect.SetActive(false);
        GlobalEffectMgr.SetEffectOnUI(eftObj);
        m_ProgressSlider = m_Progressbar.transform.GetComponent<UISlider>();
        m_ProgressLabel = m_Progressbar.transform.GetChild(1).GetComponent<UILabel>();
        m_str = StringTable.GetString("check_wait");
        return true;
    }
    public void SetIsQueue(bool bQueue, bool checkver)
    {
        if (m_IsInQueue == bQueue)
            return;
        m_IsInQueue = bQueue;
        if (m_IsInQueue)
        {
            if (checkver)
                m_str = StringTable.GetString("downver_queue");
            else
                m_str = StringTable.GetString("downres_queue");
        }
        else
        {
            if(checkver == false)
                m_str = StringTable.GetString("updating");
        }
    }
    public void BeginDown(ResType rt)
    {
        //m_Info = string.Format(StringTable.GetString("res_downloading"), GetStrFromType(rt));
        if (m_bBeginDown == false)
        {
            m_str = StringTable.GetString("updating");
            m_bBeginDown = true;
        }
    }
    public void OnGUI(float delta)
    {
        return;
        int Y = 100;
        int height = Resolution.IsHD ? 25 : 20;
        int fontSize = Resolution.IsHD ? 50 : 40;
        int profontSize = Resolution.IsHD ? 25 : 22;
        int y = Screen.height / 2 + height + Y;
        uint downBytes = 0;
        uint allBytes = 0;
        int xpadding = 100;
        if (m_bBeginDown)
        {
            m_Logic.GetDownBytes(out downBytes, out allBytes);
            float progress;
            progress = (float)downBytes / (allBytes + 0.00001f);
            Rect rect = new Rect(xpadding, y, Screen.width - xpadding * 2, height);
            Rect outline = rect;
            outline.x -= 2;
            outline.width += 4;
            outline.y -= 2;
            outline.height += 4;
            GUIHelper.ShowImg(Texture2D.whiteTexture, outline, m_LineColor, true);
            GUIHelper.ShowImg(Texture2D.whiteTexture, rect, m_BackColor, true);
            rect.width = (int)(rect.width * progress);
            GUIHelper.ShowImg(Texture2D.whiteTexture, rect, m_FrontColor, true);
            string tex = (int)(progress * 100) +"%";
            GUIHelper.ShowLabel(tex, outline, Color.black, profontSize, TextAnchor.MiddleCenter);
        }
        y -= 170;
        Rect strRect = new Rect(0, y, Screen.width, 160);
        GUIHelper.ShowLabel(m_Info, strRect, Color.black, fontSize, TextAnchor.MiddleCenter);
    }
    public void Update(float delta)
    {
        if(m_IsInQueue)
        {
            //下载资源排队中。
            m_ProgressLabel.text = string.Format(m_str, m_Logic.DownResQueue);
            return;
        }
        float progress;
        if (m_bBeginDown == false)
        {
            m_ProgressValue += delta * 0.2f;
            progress = m_ProgressValue;
        }
        else
            progress = m_Logic.DownloadProgress;
        m_ProgressSlider.value = 0.05f + progress;
        string tex = string.Format(m_str, (int)(progress * 100));
        m_ProgressLabel.text = tex;
        if (progress >= 0.1f && !m_ProgressEffect.activeSelf)
            m_ProgressEffect.SetActive(true);
        else if (progress < 0.1f && m_ProgressEffect.activeSelf)
            m_ProgressEffect.SetActive(false);
    }
    public void Shutdown()
    {
        //Screen.sleepTimeout = SleepTimeout.SystemSetting;
        if (m_Progressbar != null)
            GameObject.Destroy(m_Progressbar);
    }

}
