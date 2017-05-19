using UnityEngine;
using System.Collections.Generic;

public class SplineSampleData
{
    public Vector3 pos;
    public Quaternion rot;
    public float timeScaling;
    public short nodeIdx;
}
public class LinearNodeData
{
    public byte EventType;
    public ushort  Times;   //停留秒数或者嘲笑次数
}
public class PathLinearInterpolator
{
    public SplineSampleData[] m_SplineDataList;
    public LinearNodeData[] m_NodeList;
    public float m_MaxDistance;
    public float m_SampleMaxDistance; //采样得到的最大长度。
    public bool m_HasPathEvent;
    public Quaternion m_WorldRotation;
    public Matrix4x4 m_WorldMatrix;

    public bool HasPathEvent
    {
        get { return m_HasPathEvent; }
        set { m_HasPathEvent = value; }
    }
    public float SampleMaxDistance
    {
        get { return m_SampleMaxDistance; }
        set { m_SampleMaxDistance = value; }
    }
    public void SetWorldRotate(Quaternion rot)
    {
        m_WorldRotation *= rot;
    }
    public void SetWorldMatrix(Matrix4x4 mat)
    {
        m_WorldMatrix = mat;
    }
    public void SetWorldPosition(Vector3 pos)
    {
        m_WorldMatrix.m03 = pos.x;
        m_WorldMatrix.m13 = pos.y;
        m_WorldMatrix.m23 = pos.z;
    }
    public float MaxDistance
    {
        get { return m_MaxDistance; }
        set { m_MaxDistance = value; }
    } 
    public LinearNodeData GetNode(int n)
    {
        return m_NodeList[n];
    }
    float GetSplineDataUnzip(int idx)
    {
        return m_SplineDataList[idx].timeScaling;
    }
    public Vector3 GetPos(float time)
    {
        Vector3 pos;
        float t = m_SplineDataList.Length * time;
        int idx = (int)t;
        t -= idx;
        if (idx < m_SplineDataList.Length - 1)
        {
            pos = Vector3.Lerp(m_SplineDataList[idx].pos, m_SplineDataList[idx + 1].pos, t);
        }
        else
        {
            pos = m_SplineDataList[m_SplineDataList.Length - 1].pos;
        }
        return pos;
    }
    public float GetTimeScaling(float time, out short nodeIdx)
    {
        float t = m_SplineDataList.Length * time;
        int idx = (int)t;
        t -= idx;
        if (idx < m_SplineDataList.Length - 1)
        {
            nodeIdx = m_SplineDataList[idx].nodeIdx;
            return Utility.LerpFloat(m_SplineDataList[idx].timeScaling, m_SplineDataList[idx + 1].timeScaling, t); 
        }
        else
        {
            SplineSampleData node1 = m_SplineDataList[m_SplineDataList.Length - 1];
            nodeIdx = node1.nodeIdx;
            return node1.timeScaling;
        }
    }
    public short GetPosRotTimeScaling(float time, out Vector3 pos, out Quaternion rot, out float timeScaling)
    {
        float t = m_SplineDataList.Length * time;
        int idx = (int)t;
        t -= idx;
        if (idx < m_SplineDataList.Length - 1)
        {
            SplineSampleData node1 = m_SplineDataList[idx];
            SplineSampleData node2 = m_SplineDataList[idx + 1];
            pos = Vector3.Lerp(node1.pos, node2.pos, t);
            rot = Quaternion.Slerp(node1.rot, node2.rot, t);
            timeScaling = Utility.LerpFloat(node1.timeScaling, node2.timeScaling, t);
            return m_SplineDataList[idx].nodeIdx;
        }
        else
        {
            SplineSampleData node1 = m_SplineDataList[m_SplineDataList.Length - 1];
            timeScaling = node1.timeScaling;
            pos = node1.pos;
            rot = node1.rot;
            return node1.nodeIdx;
        }
    }
    public short GetPosRotTimeScaling(float time, out Vector3 pos, out Quaternion rot)
    {
        float t = m_SplineDataList.Length * time;
        int idx = (int)t;
        t -= idx;
        if (idx < m_SplineDataList.Length - 1)
        {
            SplineSampleData node1 = m_SplineDataList[idx];
            SplineSampleData node2 = m_SplineDataList[idx + 1];
            pos = Vector3.Lerp(node1.pos, node2.pos, t);
            rot = Quaternion.Slerp(node1.rot, node2.rot, t);
            return m_SplineDataList[idx].nodeIdx;
        }
        else
        {
            SplineSampleData node1 = m_SplineDataList[m_SplineDataList.Length - 1];
            pos = node1.pos;
            rot = node1.rot;
            return node1.nodeIdx;
        }
    }
}
