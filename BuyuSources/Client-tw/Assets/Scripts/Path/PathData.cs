using UnityEngine;
using System;
using System.Collections.Generic;

[Serializable]
public class PathPoint
{
    public PathPoint()
    {
        Position = Vector3.zero;
        Rotation = Vector3.zero;
        TimeFactor = 1.0f;
        EnableSpeedRandomOffset = 0;
        EnableStayTimesRandomOffset = 0;
        DisableOffset = false;
    }
    public PathPoint( Vector3 pos, float timeFactor)
    {
        Position = pos;
        Rotation = Vector3.zero;
        TimeFactor = timeFactor;
        EnableSpeedRandomOffset = 0;
        EnableStayTimesRandomOffset = 0;
        DisableOffset = false;
    }
    public PathPoint(Vector3 pos, Vector3 rot)
    {
        Position = pos;
        Rotation = rot;
        TimeFactor = 1.0f;
        EnableSpeedRandomOffset = 0;
        EnableStayTimesRandomOffset = 0;
        DisableOffset = false;
    }
    public Vector3  Position;
    public Vector3  Rotation;
    public float    TimeFactor;
    public int      StayType;
    public int      StayTimes;                      //停留秒数或者嘲笑次数
    public bool     Straight;                       //直线

    //编辑器使用
    public int      EnableSpeedRandomOffset;         //是否启用随机时间偏移,0无偏移
    public int      EnableStayTimesRandomOffset;    //是否启用随机停留时间偏移,0无偏移
    public bool     DisableOffset;//禁用此节点的扰动
}
[Serializable]
public class PathData
{
    public List<PathPoint> m_Points = new List<PathPoint>();
    public float m_Speed = 10.0f;
    public float m_FadeLength = 0.3f;
    public bool m_Smooth = false;

    public bool Smooth
    {
        get { return m_Smooth; }
        set { m_Smooth = value; }
    }
    public float Speed
    {
        get { return m_Speed; }
        set { m_Speed = value; }
    }
    public float FadeLength
    {
        get { return m_FadeLength; }
        set { m_FadeLength = value; }
    }
    public void ResetOrientation()
    {
        for (int i = 1; i < m_Points.Count; ++i)
            m_Points[i].Rotation = Vector3.zero;
    }
    public void AddPoint( Vector3 position, float time)
    {
        m_Points.Add(new PathPoint( position, time));
    }
    public void RemovePoint(int index)
    {
        m_Points.RemoveAt(index);
    }
    public List<PathPoint> Points
    {
        get { return m_Points; }
        set { m_Points = value; }
    }
}
