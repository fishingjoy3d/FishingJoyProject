using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HallLogicUI_RotateBtns
{
    Transform m_BaseTran;
    Transform[] m_Btns;
    RatateBtnInfo[] m_RotateInfos;

    AnimationCurve m_Curve;
    float m_RotateSpeed = 1f;
    int m_CurrentIndex = 0;
    public void Init(Transform tran)
    {
        if (tran == null) return;
        m_BaseTran = tran;

        int count = m_BaseTran.childCount;
        m_Btns = new Transform[count];
        m_RotateInfos = new RatateBtnInfo[count];

        for (int i = 0; i < count; i++)
        {
            if (i == 0)
            {
                m_Btns[i] = m_BaseTran.GetChild(1);
            }
            else if (i == 1)
            {
                m_Btns[i] = m_BaseTran.GetChild(0);
            }
            else
            {
                m_Btns[i] = m_BaseTran.GetChild(i);
            }
        }

        for (int i = 0; i < count; i++)
        {
            m_RotateInfos[i].m_FromPos = m_Btns[i].localPosition;
            m_RotateInfos[i].m_FromScale = m_Btns[i].localScale;
            m_RotateInfos[i].m_FromRotation = m_Btns[i].localRotation;

            if (i == 0)
            {
                m_RotateInfos[count - 1].m_ToPos = m_Btns[i].localPosition;
                m_RotateInfos[count - 1].m_ToScale = m_Btns[i].localScale;
                m_RotateInfos[count - 1].m_ToRotation = m_Btns[i].localRotation;
            }
            else
            {
                m_RotateInfos[i - 1].m_ToPos = m_Btns[i].localPosition;
                m_RotateInfos[i - 1].m_ToScale = m_Btns[i].localScale;
                m_RotateInfos[i - 1].m_ToRotation = m_Btns[i].localRotation;
            }
        }
        m_Curve = AnimationCurve.EaseInOut(0, 0, 1, 1);
    }

    bool m_DragStart = false;
    float m_Accumulate = 0;
    public void OnDrag(GameObject go, Vector2 delta)
    {
        if (!m_DragStart)
        {
            m_DragStart = true;
            m_Accumulate = 0;
        }
        m_Accumulate += delta.x / Screen.width;
        float t = m_Accumulate * m_RotateSpeed;
        Slerp(t);
    }

    public void OnDragEnd(GameObject go)
    {
        Debug.Log("OnDragEnd");
        ResetBtns();
        m_DragStart = false;
        m_Accumulate = 0;
    }

    int Slerp(float t)
    {
        int addIndex = 0;
        int btn_count = m_Btns.Length;

        if (t < 0)
        {
            int times = Mathf.CeilToInt(-t / btn_count);
            addIndex = Mathf.FloorToInt(t + times * btn_count) % btn_count;
            t = t + times * btn_count;
        }

        if (t >= 0)
        {
            addIndex = Mathf.FloorToInt(t) % btn_count;
            t = t - Mathf.FloorToInt(t);
        }

        float val = m_Curve.Evaluate(t);

        for (int i = 0; i < btn_count; i++)
        {
            int index = (m_CurrentIndex + addIndex + i) % btn_count;
            RatateBtnInfo info = m_RotateInfos[index];

            Vector3 pos = (1 - val) * info.m_FromPos + val * info.m_ToPos;
            Vector3 scale = (1 - val) * info.m_FromScale + val * info.m_ToScale;
            Quaternion quat = Quaternion.Slerp(info.m_FromRotation, info.m_ToRotation, val);

            m_Btns[i].localPosition = pos;
            m_Btns[i].localScale = scale;
            m_Btns[i].localRotation = quat;
        }
        return addIndex;
    }

    void ResetBtns()
    {
        float t = m_Accumulate * m_RotateSpeed;
        int round = Mathf.RoundToInt(t);
        m_CurrentIndex += Slerp(round);
    }
}

struct RatateBtnInfo
{
    public Vector3 m_FromPos;
    public Vector3 m_FromScale;
    public Quaternion m_FromRotation;

    public Vector3 m_ToPos;
    public Vector3 m_ToScale;
    public Quaternion m_ToRotation;
}