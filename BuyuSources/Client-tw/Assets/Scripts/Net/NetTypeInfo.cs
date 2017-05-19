using System;
using System.Collections.Generic;

public interface ICmdFactory
{
    NetCmdBase CreateNetCmd();
}
public class CmdFactory<T> : ICmdFactory where T : NetCmdBase, new()
{
    public NetCmdBase CreateNetCmd()
    {
        return (NetCmdBase)new T();
    }
}
public class NetTypeInfo
{
    public NetCmdType   CmdType;
    public Type         StructType;
    public ushort       StructSize;
    public int          TypeHashCode;
}
public class NetTypeCreater<T> where T : NetCmdBase, new()
{
    public static NetTypeInfo GetTypeInfo(NetCmdType type)
    {
        ushort size = (ushort)TypeSize<T>.SIZE;
        NetTypeInfo nti = new NetTypeInfo();
        nti.CmdType     = type;
        nti.StructType  = TypeSize<T>.RUN_TYPE;
        nti.StructSize  = size;
        nti.TypeHashCode = TypeSize<T>.HASH;
        return nti;
    }
}

