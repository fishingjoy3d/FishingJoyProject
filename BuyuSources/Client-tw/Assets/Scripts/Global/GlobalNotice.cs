using UnityEngine;
using System.Collections.Generic;

public class NoticeInfo
{
    public string   StrNotic;
    public uint     TextColor;
    public byte     RollCount;
    public float    RollInterval;
    public float    Elapsed;
}
public class NoticMgr
{
    List<NoticeInfo> m_NoticList = new List<NoticeInfo>();
    public void AddNotic(NoticeInfo info)
    {
        info.Elapsed = 0;// 3600 * 24;
        m_NoticList.Add(info);
    }
    public NoticeInfo Update(float delta)
    {
        NoticeInfo curNotice = null;
        for (int i = 0; i < m_NoticList.Count; )
        {
            NoticeInfo info = m_NoticList[i];
            info.Elapsed += delta;
            if(info.Elapsed >= info.RollInterval && curNotice == null)
            {
                curNotice = info;
                if (--info.RollCount == 0)
                {
                    Utility.ListRemoveAt(m_NoticList, i);
                    continue;
                }
                else
                {
                    info.Elapsed = 0;
                }
            }
            ++i;
        }
        return curNotice;
    }
    public void ClearNotice()
    {
        m_NoticList.Clear();
    }
}
public class GlobalNotice : BaseWnd
{
    Object              m_Obj;
    TweenPosition       m_TweenAnim; 
    UILabel             m_NoticeLable;
    UISprite            m_spriteIcon;
    Vector3             m_OriginalPos;
    float               m_RollSpeed;
    float               m_LifeTime;


    public void InitObj()
    {
        m_Obj = ResManager.Instance.LoadObject("SystyemNotice", "ExtraRes/UI/Predab/", ResType.ExtraRes, false);
    }
    void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(m_Obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        m_NoticeLable = m_BaseTrans.GetChild(0).GetComponent<UILabel>();
        m_TweenAnim = m_BaseTrans.GetChild(0).GetComponent<TweenPosition>();
        m_spriteIcon = m_BaseTrans.GetChild(0).GetChild(0).GetComponent<UISprite>();

        m_OriginalPos = m_BaseTrans.localPosition;

        m_RollSpeed = 120;
  
        AdjustNoticePos();
    }
 
    public void ShowNotice(NoticeInfo info)
    {       
        if (m_BaseWndObject == null)
            Init();
        string strX = info.TextColor.ToString("X6");
        string colorStr = "[" + strX + "]";


        if(info.StrNotic.Contains("/lb"))
        {
            info.StrNotic=info.StrNotic.Replace("/lb","");
            m_spriteIcon.gameObject.SetActive(true);
        }
      
        m_NoticeLable.text = colorStr + info.StrNotic + "[-]";

        Vector2 size = m_NoticeLable.localSize;
        float Duration = (size.x + 800) / m_RollSpeed;
        Vector3 ToPos = m_TweenAnim.to;
        ToPos.x = -(size.x + 400);
        m_TweenAnim.to = ToPos;
        m_TweenAnim.duration = Duration;
        m_TweenAnim.ResetToBeginning();
        m_TweenAnim.PlayForward();
        m_LifeTime = Duration;

    }
    public void AdjustNoticePos()
    {
        if (m_BaseWndObject == null)
            return;
        Vector3 hallPos = m_OriginalPos;
        if (HallRunTimeInfo.Instance_UI != null)
        {
            hallPos.y += 68;
            m_BaseTrans.localPosition = hallPos;
        }
        else
        {
            m_BaseTrans.localPosition = m_OriginalPos;
        }
    }
    public void Update(float deltal)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf == false)
            return;

        m_LifeTime -= deltal;
        if (m_LifeTime <= 0)
            ShutDown();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
}

