using UnityEngine;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net.NetworkInformation;
public class Utility
{
    
    public static float InvShortMax             = 1.0f / short.MaxValue;
    public static Vector3 NearLeftTopPoint           = new Vector3();                   ///LeftTop  RightBottom 两个点
    public static Vector3 NearRightBottomPoint = new Vector3();
    public static Vector3 FarLeftTopPoint = new Vector3();
    public static Vector3 FarRightBottomPoint = new Vector3();
    public static Vector3 NearLeftTopFlagPoint = new Vector3();
    public static Vector3 NearRightBottomFlagPoint = new Vector3();
    public static void GlobalInit()
    {
        Vector3 pt0 = new Vector3(-1, 1, -1.0f);
        Vector3 pt1 = new Vector3(1, -1, -1.0f);
        Vector3 pt2 = new Vector3(-1, 1, 1.0f);
        Vector3 pt3 = new Vector3(1, -1, 1.0f);
        Matrix4x4 mat = Camera.main.projectionMatrix.inverse;
        NearLeftTopPoint = mat.MultiplyPoint(pt0);
        NearRightBottomPoint = mat.MultiplyPoint(pt1);
        FarLeftTopPoint = mat.MultiplyPoint(pt2);
        FarRightBottomPoint = mat.MultiplyPoint(pt3);

        NearLeftTopPoint.z = -NearLeftTopPoint.z;
        NearRightBottomPoint.z = -NearRightBottomPoint.z;
        FarLeftTopPoint.z = -FarLeftTopPoint.z;
        FarRightBottomPoint.z = -FarRightBottomPoint.z;

        NearLeftTopFlagPoint.x = NearLeftTopPoint.x + 2.0f;
        NearLeftTopFlagPoint.y = NearLeftTopPoint.y - 0.6f;
        NearLeftTopFlagPoint.z = NearLeftTopPoint.z;

        NearRightBottomFlagPoint.x = NearRightBottomPoint.x - 2.0f;
        NearRightBottomFlagPoint.y = NearRightBottomPoint.y + 0.6f;
        NearRightBottomFlagPoint.z = NearRightBottomPoint.z;

    }
    public static Texture2D ScaleTexture(Texture2D source, float scaling)
    {
        int targetWidth = (int)(source.width * scaling);
        int targetHeight = (int)(source.height * scaling);
        Texture2D result = new Texture2D(targetWidth, targetHeight, source.format, false);

        float incX = (1.0f / (float)targetWidth);
        float incY = (1.0f / (float)targetHeight);

        for (int i = 0; i < result.height; ++i)
        {
            for (int j = 0; j < result.width; ++j)
            {
                Color newColor = source.GetPixelBilinear((float)j / (float)result.width, (float)i / (float)result.height);
                result.SetPixel(j, i, newColor);
            }
        }
        result.Apply();
        return result;
    }
    public static Rect GetUIScreenRect(UIWidget tex)
    {
        Vector3 pos1 = UICamera.currentCamera.WorldToScreenPoint(tex.worldCorners[0]);
        Vector3 pos2 = UICamera.currentCamera.WorldToScreenPoint(tex.worldCorners[2]);
        return new Rect(pos1.x, pos1.y, (int)(pos2.x - pos1.x), (int)(pos2.y - pos1.y));
    }
    public static Rect GetUIScreenRect(UIPanel tex)
    {
        Vector3 pos1 = UICamera.currentCamera.WorldToScreenPoint(tex.worldCorners[0]);
        Vector3 pos2 = UICamera.currentCamera.WorldToScreenPoint(tex.worldCorners[2]);
        return new Rect(pos1.x, pos1.y, (int)(pos2.x - pos1.x), (int)(pos2.y - pos1.y));
    }
    public static ReductionDataByte PackReductionData(ReductionData rdu)
    {
        ReductionDataByte rd = new ReductionDataByte();
        rd.Speed = FloatToByte(rdu.Speed);
        rd.Duration1 = FloatToByte(rdu.Duration1);
        rd.Duration2 = FloatToByte(rdu.Duration2);
        rd.Duration3 = FloatToByte(rdu.Duration3);
        return rd;
    }
    public static bool IsInScreen(Vector3 scrPos)
    {
        if (scrPos.x < 0 || scrPos.x > Screen.width || scrPos.y < 0 || scrPos.y > Screen.height)
        {
            return false;
        }
        return true;
    }
    public static ReductionData PackReductionData(ReductionDataByte rdu)
    {
        ReductionData rd = new ReductionData();
        rd.Speed = ByteToFloat(rdu.Speed);
        rd.Duration1 = ByteToFloat(rdu.Duration1);
        rd.Duration2 = ByteToFloat(rdu.Duration2);
        rd.Duration3 = ByteToFloat(rdu.Duration3);
        return rd;
    }
    public static float ByteToFloat(byte t)
    {
        return t * ConstValue.MIN_REDUCTION;
    }
    public static byte FloatToByte(float f)
    {
        return (byte)(f / ConstValue.MIN_REDUCTION);
    }
    public static void ReductionToFloat(byte scl, byte d1, byte d2, byte d3, out float fscl, float[] duration)
    {
        fscl = scl * ConstValue.INV255;
        duration[0] = d1 * ConstValue.MIN_REDUCTION;
        duration[1] = d2 * ConstValue.MIN_REDUCTION;
        duration[2] = d3 * ConstValue.MIN_REDUCTION;
    }
    public static float TickToFloat(uint tick)
    {
        return tick * 0.001f;
    }
    public static float TickSpan(uint tick)
    {
        return TickToFloat(GetTickCount() - tick);
    }
    public static uint GetTickCount()
    {
        return (uint)(GlobalTimer.Ticks);// 转换成毫秒
    }
    public static int GetHash(string str)
    {
        //return str.GetHashCode();
        return UnityEngine.Animator.StringToHash(str);
    }
    //计算字符串字节中字符个数。
    public static int GetByteCharCount(byte[] bytes)
    {
        for (int i = 0; i < bytes.Length; i += 2)
        {
            if (bytes[i] == 0 && bytes[i + 1] == 0)
                return i;
        }
        return bytes.Length;
    }
    public static Vector3 MulAdd(ref Vector3 p1, ref Vector3 p2, ref Vector3 p3)
    {
        return new Vector3(
            p1.x * p2.x + p3.x,
            p1.y * p2.y + p3.y,
            p1.z * p2.z + p3.z);
    }
    public static NetCmdVector3 ToNetCmdVec3(Vector3 v)
    {
        return new NetCmdVector3(v.x, v.y, v.z);
    }
    public static NetCmdVector4 ToNetCmdVec4(Vector4 v)
    {
        return new NetCmdVector4(v.x, v.y, v.z, v.w);
    }
    public static NetCmdVector4 ToNetCmdVec4(Quaternion v)
    {
        return new NetCmdVector4(v.x, v.y, v.z, v.w);
    }
    public static Vector3 RandVec3()
    {
        return new Vector3(
            UnityEngine.Random.Range(-1.0f, 1.0f), 
            UnityEngine.Random.Range(-1.0f, 1.0f), 
            UnityEngine.Random.Range(-1.0f, 1.0f));
    }
    public static Vector3 RandVec3XY()
    {
        return new Vector3(
            UnityEngine.Random.Range(-1.0f, 1.0f),
            UnityEngine.Random.Range(-1.0f, 1.0f),
            0);
    }
    public static int Range(int min, int max)
    {
        return UnityEngine.Random.Range(min, max);
    }
    public static float Range(float min, float max)
    {
        return UnityEngine.Random.Range(min, max);
    }
    public static float RandFloat()
    {
        return UnityEngine.Random.Range(-1.0f, 1.0f);
    }
    public static float RandPosFloat()
    {
        return UnityEngine.Random.Range(0.0f, 1.0f);
    }
    public static Vector3 MulVec(Vector3 p1, Vector3 p2)
    {
        return new Vector3(
            p1.x * p2.x,
            p1.y * p2.y ,
            p1.z * p2.z);
    }
    public static Quaternion MulAdd(ref Quaternion p1, ref Quaternion p2, ref Quaternion p3)
    {
        return new Quaternion(
            p1.x * p2.x + p3.x,
            p1.y * p2.y + p3.y,
            p1.z * p2.z + p3.z,
            p1.w * p2.w + p3.w);
    }
    public static short ToShort(float f)
    {
        return (short)(f + 0.5f);
    }
    public static short FloatToShort(float f)
    {
        return (short)(f / 90.0f * short.MaxValue);
    }
    public static float ShortToFlaot(short s)
    {
        return (float)s * ConstValue.InvShortMaxValue * 90;
    }
    public static Vector3 DivVec3(ref Vector3 v1, ref Vector3 v2)
    {
        return new Vector3(
            v1.x / v2.x,
            v1.y / v2.y,
            v1.z / v2.z);
    }
    public static Quaternion DivQ(ref Quaternion q1, ref Quaternion q2)
    {
        return new Quaternion(
            q1.x / q2.x,
            q1.y / q2.y,
            q1.z / q2.z,
            q1.w / q2.w);
    }
    public static Quaternion SubQ(ref Quaternion q1, ref Quaternion q2)
    {
        return new Quaternion(
            q1.x - q2.x,
            q1.y - q2.y,
            q1.z - q2.z,
            q1.w - q2.w);
    }
    public static Quaternion MinQ(ref Quaternion q1, ref Quaternion q2)
    {
        return new Quaternion(
            Math.Min(q1.x, q2.x),
            Math.Min(q1.y, q2.y),
            Math.Min(q1.z, q2.z),
            Math.Min(q1.w, q2.w)
            );
    }
    public static Quaternion MaxQ(ref Quaternion q1, ref Quaternion q2)
    {
        return new Quaternion(
            Math.Max(q1.x, q2.x),
            Math.Max(q1.y, q2.y),
            Math.Max(q1.z, q2.z),
            Math.Max(q1.w, q2.w)
            );
    }
    public static Vector3 MaxVec3(ref Vector3 v1, ref Vector3 v2)
    {
        return new Vector3(
            Math.Max(v1.x, v2.x),
            Math.Max(v1.y, v2.y),
            Math.Max(v1.z, v2.z)
            );
    }
    public static Vector3 MinVec3(ref Vector3 v1, ref Vector3 v2)
    {
        return new Vector3(
            Math.Min(v1.x, v2.x),
            Math.Min(v1.y, v2.y),
            Math.Min(v1.z, v2.z)
            );
    }
    public static void ListRemoveAt<T>(List<T> list, int idx)
    {
        if(idx != list.Count - 1)
        {
            list[idx] = list[list.Count - 1];
        }
        list.RemoveAt(list.Count - 1);
    }
    public static int ComputePathSampleCount(float dist)
    {
        //int count = (int)(dist * GlobalSetting.SampleFactor);
        //count = Mathf.Max(count, 50);
        //return count;
        return 1;
    }
    public static float LerpFloat(float a, float b, float t)
    {
        return a + (b - a) * t;
    }
    public static Vector3 ReadVec3(BinaryReader br)
    {
        return new Vector3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
    }
    public static Vector4 ReadVec4(BinaryReader br)
    {
        return new Vector4(br.ReadSingle(), br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
    }
    public static Quaternion ReadQuaternion(BinaryReader br)
    {
        return new Quaternion(br.ReadSingle(), br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
    }
    public static Matrix4x4 ReadMatrix4x4(BinaryReader br)
    {
        Matrix4x4 mat = new Matrix4x4();
        mat.SetRow(0, ReadVec4(br));
        mat.SetRow(1, ReadVec4(br));
        mat.SetRow(2, ReadVec4(br));
        mat.SetRow(3, ReadVec4(br));
        return mat;
    }
    public static uint VersionToUint(string ver)
    {
        int r1 = ver.IndexOf('_') + 1;
        int r2 = ver.IndexOf('_', r1) + 1;
        int r3 = ver.IndexOf('_', r2) + 1;
        string n1 = ver.Substring(0, r1 - 1);
        string n2 = ver.Substring(r1, r2 - r1 - 1);
        string n3 = ver.Substring(r2, r3 - r2 - 1);
        string n4 = ver.Substring(r3, ver.Length - r3);

        uint v1 = uint.Parse(n1);
        uint v2 = uint.Parse(n2);
        uint v3 = uint.Parse(n3);
        uint v4 = uint.Parse(n4);

        return (v1 << 24) | (v2 << 16) | (v3 << 8) | v4;
    }
    public static byte[] CopyArray(byte[] a, ushort offset, int length)
    {
        byte[] data = new byte[length];
        System.Array.Copy(a, offset, data, 0, length);
        return data;
    }
    public static string IPToString(uint ip)
    {
        return string.Format("{0}.{1}.{2}.{3}", (byte)(ip >> 0), (byte)(ip >> 8), (byte)(ip >> 16), (byte)(ip >> 24));
    }
    public static uint StringToIP(string str)
    {
        int k1 = str.IndexOf('.') + 1;
        int k2 = str.IndexOf('.', k1) + 1;
        int k3 = str.IndexOf('.', k2) + 1;
        uint a = uint.Parse(str.Substring(0, k1 - 1));
        uint b = uint.Parse(str.Substring(k1, k2 - k1 - 1));
        uint c = uint.Parse(str.Substring(k2, k3 - k2 - 1));
        uint d = uint.Parse(str.Substring(k3, str.Length - k3));
        return (a << 24) | (b << 16) | (c << 8) | d;
    }
    public static float GetPathTimeByDist(float startX, float curX, PathLinearInterpolator pi)
    {
        float dist = startX - curX;
        float dist2 = pi.GetPos(0).x - pi.GetPos(0.01666f).x;
        return dist / dist2 * 0.01666f;
    }
    public static string VerToStr(uint ver)
    {
        return string.Format("{0}_{1}_{2}_{3}", ver >> 24, ver >> 16 & 0xff, ver >> 8 & 0xff, ver & 0xff);
    }
    public static string VerToDotStr(uint ver)
    {
        return string.Format("{0}.{1}.{2}", ver >> 16 & 0xff, ver >> 8 & 0xff, ver & 0xff);
    }
    
    public static string GetMacAddress()
    {
        try
        {
            return NativeInterface.GetMac();
        }
        catch
        {
            return "MAC_NONE";
        }
    }
    //如果角度在指定范围内
    const float LOCK_FISH_DIST_SQR = 0.07f * 0.07f;
    public static bool CheckLauncherAngle(Fish fish, Vector3 scrStartPoint, Vector3 viewStartPoint)
    {
        Vector3 dir = fish.ScreenPos - scrStartPoint;
        dir.Normalize();
        float dot = Vector2.Dot(dir, Vector2.up);
        if (dot < -0.5f && (fish.ViewPos - viewStartPoint).sqrMagnitude > LOCK_FISH_DIST_SQR)
        {
            return false;
        }
        return true;
    }
}
