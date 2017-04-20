using System;
using System.Collections.Generic;
using UnityEngine;
public struct SendCmdPack
{
    public NetCmdBase Cmd;
    public int Hash;
}
public class NetCmdHelper
{
    public static bool CheckCmdType(NetCmdType cmdType)
    {
        if (NetCmdMapping.IsRegisterCmd(cmdType))
            return true;
        else
            return false;
    }
    public static byte[] CmdToBytes<T>(NetCmdBase cmd, int prefixLength)
    {
        byte[] data = TypeReflector.ObjToBytes<T>(cmd, prefixLength);
        if (data == null)
        {
            LogMgr.Log("Unregister cmd type:" + cmd.GetCmdType());
        }
        else
        {
            data[0] = (byte)data.Length;
            data[1] = (byte)(data.Length >> 8);
        }
        return data;
    }
    public static byte[] CmdToBytes(SendCmdPack pack, int prefixSize)
    {
        byte[] data = TypeReflector.ObjToBytes(pack.Cmd, pack.Hash, prefixSize);
        if (data == null)
        {
            LogMgr.Log("Unregister cmd type:" + pack.Cmd.GetCmdType());
        }
        else
        {
            int length = data.Length - prefixSize;
            data[prefixSize] = (byte)length;
            data[prefixSize + 1] = (byte)(length >> 8);
        }
        return data;
    }
    public static NetCmdBase ByteToCmd(byte[] byteData, int offset, int length)
    {
        if (length < 4)
            return null;
        int t1 = (int)byteData[offset + 3];
        int t2 = (int)byteData[offset + 2];
        t1 = t1 << 8 | t2;

        NetCmdType cmdType = (NetCmdType)(t1);
        if (cmdType >= NetCmdType.CMD_MAX)
            return null;
        NetTypeInfo typeInfo = NetCmdMapping.GetTypeInfo(cmdType);
        if(typeInfo == null)
        {
            LogMgr.Log("未注册的命令：" + cmdType.ToString());
            return null;
        }
        if (length < typeInfo.StructSize)
        {
            LogMgr.Log("命令大小与结构体大小不匹配：" + length.ToString() + " : " + typeInfo.StructSize.ToString() + " :" + cmdType.ToString());
            return null;
        }
        NetCmdBase cmd = (NetCmdBase)TypeReflector.BytesToObj(
            typeInfo.TypeHashCode,
            byteData,
            offset,
            length
            );
        return cmd;
    }
}
