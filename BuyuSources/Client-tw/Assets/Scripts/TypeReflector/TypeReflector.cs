using System;
using System.Collections.Generic;
using System.Reflection;
using System.IO;
public class TypeReflector
{
    static object CreateObj(TypeData td, BinaryReader bin)
    {
        Type t = td.ClassType;
        int size = (int)(bin.BaseStream.Length - bin.BaseStream.Position);
        if (size < td.Size / td.ArrayCount)
        {
            LogMgr.Log("字节流的长度小于类的长度, Bytes Size:" + bin.BaseStream.Length.ToString() +", Class Size:" + (td.Size / td.ArrayCount).ToString());
            return null;
        }
        object obj = Activator.CreateInstance(t);
        if (obj == null)
            return null;

        for (int i = 0; i < td.Fields.Length; ++i)
        {
            TypeData curData = td.Fields[i];
            if(curData.Handle.FieldType.IsPrimitive)
            {
                ITypeReader iReader = TypeReaderMapping.GetReaderFromHash(curData.HashCode);
                curData.Handle.SetValue(obj, iReader.Read(bin, 0));
            }
            else if (curData.Handle.FieldType.IsArray)
            {
                if (curData.VarType)
                {
                    //变长 
                    int curSize = (int)(bin.BaseStream.Length - bin.BaseStream.Position);
                    if (curData.VarType && (curSize % curData.OrgSize != 0))
                    {
                        LogMgr.Log("字节长度不是变长类型的整数倍, Bytes Size:" + curSize.ToString() + ", Class Size:" + td.OrgSize.ToString());
                        return null;
                    }
                    if (curSize > 0)
                    {
                        curSize /= curData.OrgSize;
                        if (curData.ClassType.IsPrimitive)
                        {
                            ITypeReader iReader = TypeReaderMapping.GetReaderFromHash(curData.HashCode);
                            curData.Handle.SetValue(obj, iReader.Read(bin, curSize));
                        }
                        else
                        {
                            object[] arr = (object[])Array.CreateInstance(curData.ClassType, curSize);
                            for (int j = 0; j < curSize; ++j)
                            {
                                arr[j] = CreateObj(curData, bin);
                                if (arr[j] == null)
                                    return null;
                            }
                            curData.Handle.SetValue(obj, arr);
                        }
                    }
                }
                else
                {
                    //固定长度
                    if (curData.ClassType.IsPrimitive)
                    {
                        ITypeReader iReader = TypeReaderMapping.GetReaderFromHash(curData.HashCode);
                        curData.Handle.SetValue(obj, iReader.Read(bin, curData.ArrayCount));
                    }
                    else
                    {
                        object[] arr = (object[])Array.CreateInstance(curData.ClassType, curData.ArrayCount);
                        for (int j = 0; j < curData.ArrayCount; ++j)
                        {
                            arr[j] = CreateObj(curData, bin);
                            if (arr[j] == null)
                                return null;
                        }
                        curData.Handle.SetValue(obj, arr);
                    }
                }
            }
            else if (curData.HashCode == TypeMapping.STRING_HASH)
            {
                ITypeReader iReader = TypeReaderMapping.GetReaderFromHash(curData.HashCode);
                if (curData.VarType)
                {
                    int curSize = (int)(bin.BaseStream.Length - bin.BaseStream.Position);
                    if(curSize % 2 != 0)
                    {
                        LogMgr.Log("字符串长度不是字符的整数倍:"+ curSize.ToString());
                        return null;
                    }
                    if(curSize > 0)
                        curData.Handle.SetValue(obj, iReader.Read(bin, curSize / 2));
                }
                else
                    curData.Handle.SetValue(obj, iReader.Read(bin, curData.ArrayCount));
            }
            else if (curData.Handle.FieldType.IsClass /*|| curData.FieldHandle.FieldType.IsValueType*/)
            {
                object subObj = CreateObj(curData, bin);
                if (subObj == null) return null;
                curData.Handle.SetValue(obj, subObj);
            }
            else
            {
                LogMgr.Log("未知的类型:" + curData.Handle.FieldType.ToString());
                return null;
            }
        }
        return obj;

    }
    public static object BytesToObj<T>(byte[] data, int offset, int length)
    {
        TypeData td = TypeMapping.FindType(TypeSize<T>.HASH);
        if (td == null)
        {
            LogMgr.Log("未注册的类型:" + TypeSize<T>.RUN_TYPE.ToString());
            return null;
        }
        System.IO.MemoryStream ms = new MemoryStream(data, offset, length);
        BinaryReader bin = new BinaryReader(ms);
        return CreateObj(td, bin);
    }
    public static object BytesToObj(int hash, byte[] data, int offset, int length)
    {
        TypeData td = TypeMapping.FindType(hash);
        if (td == null)
        {
            LogMgr.Log("未注册的类型:" + hash.ToString());
            return null;
        }
        System.IO.MemoryStream ms = new MemoryStream(data, offset, length);
        BinaryReader bin = new BinaryReader(ms);
        return CreateObj(td, bin);
    }
    public static byte[] ObjToBytes<T>(object obj, int prefixLength)
    {
        return ObjToBytes(obj, TypeSize<T>.HASH, prefixLength);
    }
    public static byte[] ObjToBytes(object obj, int hash, int prefixLength)
    {
        TypeData td = TypeMapping.FindType(hash);
        if (td == null)
        {
            LogMgr.Log("未注册的类型:" + hash.ToString());
            return null;
        }
        MemoryStream ms = new MemoryStream();
        BinaryWriter bin = new BinaryWriter(ms);
        if (!ConvertToBytes(td, obj, bin))
            return null;
        byte[] bytes = new byte[ms.Length + prefixLength];
        byte[] data = ms.GetBuffer();
        Array.Copy(data, 0, bytes, prefixLength, ms.Length);
        return bytes;
    }
    static bool ConvertToBytes(TypeData td, object obj, BinaryWriter bin)
    {
        for(int i = 0; i < td.Fields.Length; ++i)
        {
            TypeData curData = td.Fields[i];
            if (curData.Handle.FieldType.IsPrimitive)
            {
                ITypeReader iReader = TypeReaderMapping.GetReaderFromHash(curData.HashCode);
                object f = curData.Handle.GetValue(obj);
                iReader.Write(bin, f, 0);
            }
            else if (curData.Handle.FieldType.IsArray)
            {
                if (curData.ClassType.IsPrimitive)
                {
                    ITypeReader iReader = TypeReaderMapping.GetReaderFromHash(curData.HashCode);
                    object f = curData.Handle.GetValue(obj);
                    if (curData.VarType)//写入变长
                    {
                        iReader.Write(bin, f, -1);
                    }
                    else
                    {
                        iReader.Write(bin, f, curData.ArrayCount);
                    }
                }
                else
                {
                    object[] f = (object[])curData.Handle.GetValue(obj);
                    if (curData.VarType)//写入变长
                    {
                        if (f != null && f.Length > 0)
                        {
                            for (int j = 0; j < f.Length; ++j)
                                ConvertToBytes(curData, f[j], bin);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < curData.ArrayCount; ++j)
                            ConvertToBytes(curData, f[j], bin);
                    }
                    
                }
            }
            else if (curData.HashCode == TypeMapping.STRING_HASH)
            {
                ITypeReader iReader = TypeReaderMapping.GetReaderFromHash(curData.HashCode);
                object f = curData.Handle.GetValue(obj);
                if (curData.VarType)
                    iReader.Write(bin, f, -1);
                else
                    iReader.Write(bin, f, curData.ArrayCount);
            }
            else if (curData.Handle.FieldType.IsClass/* || curData.Handle.FieldType.IsValueType*/)
            {
                object f = curData.Handle.GetValue(obj);
                ConvertToBytes(curData, f, bin);
            }
            else
            {
                LogMgr.Log("未知的类型:" + curData.Handle.FieldType.ToString());
                return false;
            }
        }

        return true;
    }
}

