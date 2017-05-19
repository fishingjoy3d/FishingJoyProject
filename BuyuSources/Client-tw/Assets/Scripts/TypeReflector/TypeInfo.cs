using System;
using System.Collections.Generic;
using System.Reflection;
public class TypeInfo : System.Attribute
{
    public short    Count;
    public short    Index;
    public bool     VarType;
    public TypeInfo(short index)
    {
        Index = index;
        Count = 0;
        VarType = false;
    }
    public TypeInfo(short index, bool bVarType)
    {
        Index = index;
        Count = 1;
        VarType = bVarType;
    }
    public TypeInfo(short index, short count)
    {
        if (count < 1)
            Count = 0;
        else
            Count = count;
        Index = index;
        VarType = false;
    }
}

public class TypeSize<T>
{
    public static ushort SIZE = (ushort)TypeMapping.FindType(typeof(T)).Size;
    public static int HASH = Utility.GetHash(typeof(T).ToString());
    public static Type RUN_TYPE = typeof(T);
}