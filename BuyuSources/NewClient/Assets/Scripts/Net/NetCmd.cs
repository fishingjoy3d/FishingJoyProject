using System;
using UnityEngine;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public enum NetCmdLimit
{
    CMD_MAX_SIZE = 1024 //命令最大字节长度
}
//网络命令的接口
public interface INetCmdBase
{
    NetCmdType GetCmdType();
    bool IsVarType();
    Type GetVarType();
    ushort GetCmdSize();
    void SetCmdType(NetCmdType nct);
    void SetCmdSize(ushort size);
}

public class NetCmdBase 
{
    [TypeInfo(0)]
    public ushort   CmdSize;    //命令长度
    [TypeInfo(1)]
    public byte     SubCmdType; //子命令ID
    [TypeInfo(2)]
    public byte     CmdType;    //主命令ID
    //[TypeInfo(3)]
    //public ushort   Rand;       //随机标识符
    //[TypeInfo(4)]
    //public ushort   Identity;   //生成的识别符
    public void CreateRandMask()
    {
       // Rand = (ushort)Utility.Range(0, ushort.MaxValue);
        //Identity = (ushort)((~Rand) ^ (~CmdSize) ^ (~SubCmdType) ^ (~CmdType));
    }
    public void CreateRandMask(ushort rand)
    {
        //Rand = rand;
        //Identity = (ushort)((~Rand) ^ (~CmdSize) ^ (~SubCmdType) ^ (~CmdType));
    }
    public NetCmdType GetCmdType()
    {
        return (NetCmdType)(((int)CmdType) << 8 | SubCmdType);
    }
    public byte GetSubCmdType()
    {
        return SubCmdType;
    }
    public virtual bool IsVarType()
    {
        return false;
    }
    public virtual Type GetVarType()
    {
        return typeof(NetCmdBase);
    }
    public virtual ushort GetVarTypeSize()
    {
        return 0;
    }
    public ushort GetCmdSize()
    {
        return CmdSize;
    }
    public void SetCmdType(NetCmdType nct)
    {
        int cmd     = (int)nct;
        CmdType     = (byte)(cmd >> 8);
        SubCmdType  = (byte)cmd;
    }
    public virtual int GetVarTypeHash()
    {
        return 0;
    }
}
public class NetCmd : NetCmdBase
{
    public NetCmd()
    {

    }
    public NetCmd(NetCmdType t)
    {
        SetCmdType(t);
    }
}

