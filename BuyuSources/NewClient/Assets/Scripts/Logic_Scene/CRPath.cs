using UnityEngine;
using System.Collections.Generic;
public class SimplePath
{
    Vector3 m_Start, m_End;
    float m_DistFactor;
    float m_Speed;
    float m_Time;
    float m_Acceleration;
    public float Acceleration
    {
        get { return m_Acceleration; }
        set { m_Acceleration = value; }
    }
    public SimplePath(Vector3 startPos, Vector3 endPos)
    {
        m_Time = 0;
        m_Acceleration = 1;
        m_Speed = 1;
        m_Start = startPos;
        m_End = endPos;
        m_DistFactor = 2.0f / (m_Start - m_End).sqrMagnitude;
    }
    public bool Finished
    {
        get
        {
            return PathTime >= 1.0f;
        }
    }
    public float PathTime
    {
        get
        {
            return m_Time;
        }
    }
    public float Speed
    {
        get
        {
            return m_Speed;
        }
        set
        {
            m_Speed = value;
        }
    }
    public Vector3 Update(float delta)
    {
        m_Speed += m_Speed * Acceleration * delta;
        m_Time += delta * m_Speed * m_DistFactor;
        return Vector3.Lerp(m_Start, m_End, m_Time);
    }

}
public class CRPath
{
    static Matrix4x4 GetCRMatrix()
    {
        Matrix4x4 hermite = new Matrix4x4();
        hermite.SetRow(0, new Vector4(0, 2f, 0, 0));
        hermite.SetRow(1, new Vector4(-1f, 0, 1f, 0));
        hermite.SetRow(2, new Vector4(2f, -5f, 4f, -1f));
        hermite.SetRow(3, new Vector4(-1f, 3f, -3f, 1f));
        return hermite;
    }
    static Matrix4x4 CRMatrix = GetCRMatrix();
    float   Time;
    Vector3 LerpPosition;
    Matrix4x4 CR1, CR2, CR3;
    float PathSpeed;
    float Acceleration;
    float DistFactor;
   // List<Vector3> m_PosList = new List<Vector3>();
    public CRPath(Vector3 startPos, Vector3 startControl, Vector3 endPos, Vector3 endControl)
    {
        PathSpeed = 1.0f;
        Acceleration = 1.0f;
        //第一个和最后一个节点的辅助线
        Vector3 p1 = startPos + (startPos - startControl);
        Vector3 p2 = endPos + (endPos - endControl);

        CR1.SetRow(0, new Vector4(p1.x, p1.y, p1.z, 0));
        CR1.SetRow(1, new Vector4(startPos.x, startPos.y, startPos.z, 0));
        CR1.SetRow(2, new Vector4(startControl.x, startControl.y, startControl.z, 0));
        CR1.SetRow(3, new Vector4(endControl.x, endControl.y, endControl.z, 0));

        CR2.SetRow(0, new Vector4(startPos.x, startPos.y, startPos.z, 0));
        CR2.SetRow(1, new Vector4(startControl.x, startControl.y, startControl.z, 0));
        CR2.SetRow(2, new Vector4(endControl.x, endControl.y, endControl.z, 0));
        CR2.SetRow(3, new Vector4(endPos.x, endPos.y, endPos.z, 0));

        CR3.SetRow(0, new Vector4(startControl.x, startControl.y, startControl.z, 0));
        CR3.SetRow(1, new Vector4(endControl.x, endControl.y, endControl.z, 0));
        CR3.SetRow(2, new Vector4(endPos.x, endPos.y, endPos.z, 0));
        CR3.SetRow(3, new Vector4(p2.x, p2.y, p2.z, 0));

        CR1 = (CRMatrix * CR1).transpose;
        CR2 = (CRMatrix * CR2).transpose;
        CR3 = (CRMatrix * CR3).transpose;

        float dist =
            (startControl - startPos).magnitude +
            (endPos - startControl).magnitude +
            (endControl - endPos).magnitude;
        DistFactor = 1.0f / dist;
        /*
        m_PosList.Add(p1);
        m_PosList.Add(startPos);
        m_PosList.Add(startControl);
        m_PosList.Add(endControl);
        m_PosList.Add(endPos);
        m_PosList.Add(p2);
        */
    }
    public float PathAcceleration
    {
        get { return Acceleration; }
        set { Acceleration = value; }
    }
    public Vector3 Position
    {
        get
        {
            return LerpPosition;
        }
    }
    public bool Finished
    {
        get
        {
            return Time >= 1.0f;
        }
    }
    public float PathTime
    {
        get
        {
            return Time;
        }
    }
    public float Speed
    {
        get
        {
            return PathSpeed;
        }
        set
        {
            PathSpeed = value;
        }
    }
    public Vector3 GetPos(float t, ref Matrix4x4 mat)
    {
        float t2 = t * t;
        Vector4 tt = new Vector4(1.0f, t, t2, t * t2);
        tt *= 0.5f;
        tt = mat * tt;
        return new Vector3(tt.x, tt.y, tt.z);
    }
    //这个是优化的算法。
    public Vector3 Update(float delta)
    {
        PathSpeed += PathSpeed * Acceleration * delta;
        Time += delta * PathSpeed * DistFactor;
        if (Time < 0.3333333f)
            LerpPosition = GetPos(Time / 0.3333333f, ref CR1);
        else if (Time < 0.6666666f)
            LerpPosition = GetPos((Time - 0.3333333f) / 0.3333333f, ref CR2);
        else if (Time < 0.9999999f)
            LerpPosition = GetPos((Time - 0.6666666f) / 0.3333333f, ref CR3);

        return LerpPosition;
    }
    //这个是标准的CR算法。
    /*
    public Vector3 CRSpline(float t)
    {
        int numSections = m_PosList.Count - 3;
        int currPt = Mathf.Min(Mathf.FloorToInt(t * (float)numSections), numSections - 1);
        float u = t * (float)numSections - (float)currPt;

        Vector3 a = m_PosList[currPt];
        Vector3 b = m_PosList[currPt + 1];
        Vector3 c = m_PosList[currPt + 2];
        Vector3 d = m_PosList[currPt + 3];

        return .5f * (
          (-a + 3f * b - 3f * c + d) * (u * u * u)
          + (2f * a - 5f * b + 4f * c - d) * (u * u)
          + (-a + c) * u
          + 2f * b
          );
    }
    */
}
