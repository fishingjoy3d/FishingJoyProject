using UnityEngine;
using System.Collections.Generic;
public enum ControllerEvent
{
    EVENT_NONE,
    EVENT_ACTIVE_LAUGH,
    EVENT_DEACTIVE_LAUGH,
    
}
public class PathController
{
    const float SPEED_UNIT = 0.02222222f;
    PathLinearInterpolator m_Interp = null;
    PathTimeController m_TimeController = new PathTimeController();
    PathEvent m_PathEvent = new PathEvent();

    public Vector3      m_Position;    //当前插值的位置
    public Quaternion   m_Rotation;   //当前的持续旋转
    short               m_NodeIdx;
    byte                m_CtrlEvent;

    public ControllerEvent CtrlEvent
    {
        get { return (ControllerEvent)m_CtrlEvent; }
        set { m_CtrlEvent = (byte)value; }
    }
    public void ResetCtrlEvent()
    {
        CtrlEvent = ControllerEvent.EVENT_NONE;
    }
    public PathEvent PathEvent
    {
        get { return m_PathEvent; }
        set { m_PathEvent = value; }
    }
    public bool IsActiveEvent
    {
        get
        {
            return m_PathEvent.IsActiveEvent;
        }
    }
    public bool IsActiveLaugh
    {
        get
        {
            return m_PathEvent.IsActiveLaugh;
        }
    }
    public bool HasPathEvent
    {
        get
        {
            return m_Interp.HasPathEvent;
        }
    }
    public bool CheckDeactiveLaugh()
    {
        return m_PathEvent.CheckDeactiveLaugh();
    }
    public bool CheckCurrentEvent(bool bValid = true)
    {
        return m_PathEvent.CheckCurrentEvent(CurrentNodeIndex, CurrentNode, bValid);
    }
    public PathTimeController TimeController
    {
        get { return m_TimeController; }
        set { m_TimeController = value; }
    }
    public short CurrentNodeIndex
    {
        get
        {
            return m_NodeIdx;
        }
    }
    public LinearNodeData CurrentNode
    {
        get
        {
            return m_Interp.GetNode(m_NodeIdx);
        }
    }
    public bool EnableTimeFactor
    {
        get { return m_TimeController.EnablePathTimeScaling; }
        set { m_TimeController.EnablePathTimeScaling = value; }
    }
    public PathLinearInterpolator Interpolator
    {
        get { return m_Interp; }
    }
    public float LaughTime
    {
        get
        {
            return m_PathEvent.LaughTime;
        }
    }
    public void ResetController(PathLinearInterpolator interpolator, float speed, float time, float laughTime)
    {
        CtrlEvent = ControllerEvent.EVENT_NONE;
        m_PathEvent.Reset(laughTime);
        m_Interp = interpolator;
        m_TimeController.Reset(m_Interp.SampleMaxDistance, speed, time);
        CheckGetPRT();
    }
    public float Time
    {
        get { return m_TimeController.Time; }
        set {
            m_TimeController.Time = value;
            CheckGetPRT();
        }
    }
    void CheckGetPRT()
    {
        m_NodeIdx = m_Interp.GetPosRotTimeScaling(
           Mathf.Clamp(m_TimeController.Time, 0, 1),
           out m_Position,
           out m_Rotation);
    }
    public float CurrentSpeed
    {
        get
        {
            return m_TimeController.CurrentSpeed;
        }
    }
    public float OrignalSpeed
    {
        get
        {
            return m_TimeController.OrignalSpeed;
        }
        set
        {
            m_TimeController.OrignalSpeed = value;
        }
    }
    public void AddElapsedTime(float deltaTime)
    {
        while (deltaTime > SPEED_UNIT)
        {
            AddDelta(SPEED_UNIT);
            deltaTime -= SPEED_UNIT;
        }
        if(deltaTime > 0.0001f)
            AddDelta(deltaTime);
    }
    void AddDelta(float deltaTime)
    {
        if (HasPathEvent)
        {
            deltaTime = m_PathEvent.Update(deltaTime);
            if (m_PathEvent.IsActiveEvent)
                return;
            if (CheckCurrentEvent())
                CtrlEvent = ControllerEvent.EVENT_ACTIVE_LAUGH;
            if (CheckDeactiveLaugh())
                CtrlEvent = ControllerEvent.EVENT_DEACTIVE_LAUGH;
        }
        if (m_TimeController.Update(deltaTime) > 0.0f)
            m_TimeController.PathTimeScaling = m_Interp.GetTimeScaling(m_TimeController.Time, out m_NodeIdx);
    }
    public bool Update(float deltaTime)
    {
        AddElapsedTime(deltaTime);
        //deltaTime > SPPED_UNIT说明时间有跳跃，必须更新位置
        if ((deltaTime > SPEED_UNIT || !m_PathEvent.IsActiveEvent) && m_TimeController.Time > 0.0f)
        {
            m_NodeIdx = m_Interp.GetPosRotTimeScaling(
            m_TimeController.Time,
            out m_Position,
            out m_Rotation
            );
            return true;
        }
        else
            return false;
    }
}
