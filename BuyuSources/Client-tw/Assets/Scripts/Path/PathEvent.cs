using UnityEngine;
using System.Collections.Generic;

public enum PathEventType
{
    EVENT_NONE = 0,
    EVENT_STAY,     //停留
    EVENT_LAUGH,    //嘲讽
    EVENT_ATTACK,   //受击
}
public class PathEvent
{
    public int      m_NodeIdx;
    PathEventType   m_EventType;
    public float    m_CurElapsedTime;
    float           m_LaughTime;
    public int      m_StayTimes;
    bool            m_DeactiveLaugh;//结束嘲讽
    bool            m_bEnableEvent;
    public bool IsActiveEvent
    {
        get
        {
            return m_EventType != PathEventType.EVENT_NONE;
        }
    }
    public bool IsActiveLaugh
    {
        get
        {
            return m_EventType == PathEventType.EVENT_LAUGH;
        }
    }
    public bool CheckDeactiveLaugh()
    {
        if (m_DeactiveLaugh)
        {
            m_DeactiveLaugh = false;
            return true;
        }
        else
            return false;
    }
    public float LaughTime
    {
        get
        {
            return m_LaughTime;
        }
    }
    public void Reset(float laughTime, bool bEnableEvent = true)
    {
        m_NodeIdx = 0;
        m_EventType = PathEventType.EVENT_NONE;
        m_CurElapsedTime = 0;
        m_LaughTime = laughTime;
        m_StayTimes = 0;
        m_DeactiveLaugh = false;
        m_bEnableEvent = bEnableEvent;
    }
    public PathEventType EventType
    {
        get { return m_EventType; }
        set { m_EventType = value; }
    }
    //服务器同步时，可能已经过了嘲讽时间，使用valid进行判断
    public bool CheckCurrentEvent(short nodeIdx, LinearNodeData sd, bool bValid = true)
    {
        if (m_bEnableEvent == false)
            return false;
        if (m_NodeIdx != nodeIdx)
        {
            m_NodeIdx = nodeIdx;
            if (bValid)
            {
                if (sd.EventType == (byte)PathEventType.EVENT_STAY)
                {
                    m_EventType = (PathEventType)sd.EventType;
                    m_StayTimes = sd.Times;
                }
                else if (sd.EventType == (byte)PathEventType.EVENT_LAUGH && m_LaughTime != 0)
                {
                    m_EventType = (PathEventType)sd.EventType;
                    m_StayTimes = sd.Times;
                    return true;
                }
            }
        }
        return false;
    }

    public float Update(float fDelta)
    {
        if (m_EventType == PathEventType.EVENT_NONE)
            return fDelta;

        m_CurElapsedTime += fDelta;
        if (m_EventType == PathEventType.EVENT_LAUGH)
        {
            int count = (int)(m_CurElapsedTime / m_LaughTime);
            if (count >= m_StayTimes)
            {
                //已经结束嘲笑了
                m_EventType = PathEventType.EVENT_NONE;
                fDelta = m_CurElapsedTime % m_LaughTime;
                m_CurElapsedTime = 0;
                m_DeactiveLaugh = true;
            }
        }
        else
        {
            //停留时间计时
            float stayTime = m_StayTimes * 0.001f;
            if (m_CurElapsedTime >= stayTime)
            {
                m_EventType = PathEventType.EVENT_NONE;
                fDelta = m_CurElapsedTime % stayTime;
                m_CurElapsedTime = 0;
            }
        }
        return fDelta;
    }
}
