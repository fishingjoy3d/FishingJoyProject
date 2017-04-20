using System;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public interface ITypeReader
{
    object Read(BinaryReader bin, int count);
    void Write(BinaryWriter bin, object obj, int count);
    short TypeSize();
}
public class BinBoolReader:ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if(count > 0)
        {
            bool[] arr = new bool[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadBoolean();
            return arr;
        }
        else
            return bin.ReadBoolean();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if(count == -1)
        {
            if (obj == null)
                return;
            bool[] arr = (bool[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            bool[] arr = (bool[])obj;
            if (arr == null)
                arr = new bool[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            for (; i < count; ++i)
                bin.Write(false);
        }
        else
            bin.Write((bool)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(bool);
    }
}
public class BinByteReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            return bin.ReadBytes(count);
        }
        else
            return bin.ReadByte();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            bin.Write((byte[])obj);
        }
        else if (count > 0)
        {
            byte[] arr = (byte[])obj;
            if (arr == null)
                arr = new byte[count];
            int minCount = Math.Min(arr.Length, count);
            bin.Write(arr, 0, minCount);
            const byte b = 0;
            for (int i = minCount; i < count; ++i)
                bin.Write(b);
            
        }
        else
            bin.Write((byte)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(byte);
    }
}
public class BinCharReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            byte[] ss2 = bin.ReadBytes(count * 2);
            return System.Text.Encoding.Unicode.GetChars(ss2);
        }
        else
        {
            byte[] ss = new byte[2];
            ss[0] = bin.ReadByte();
            ss[1] = bin.ReadByte();
            return System.Text.Encoding.Unicode.GetChars(ss)[0];
        }
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            char[] arr = (char[])obj;
            byte[] data = System.Text.Encoding.Unicode.GetBytes(arr);
            bin.Write(data);
        }
        else if (count > 0)
        {
            char[] arr = (char[])obj;
            if (arr == null)
                arr = new char[count];
            int minCount = Math.Min(arr.Length, count);
            byte[] data = System.Text.Encoding.Unicode.GetBytes(arr, 0, minCount);
            bin.Write(data);
            ushort b = 0;
            for (int i = minCount; i < count; ++i)
                bin.Write(b);
        }
        else
        {
            byte[] data = System.Text.Encoding.Unicode.GetBytes(new char[1]{(char)obj});
            bin.Write(data);
        }
    }
    public short TypeSize()
    {
        return (short)sizeof(char);
    }
}
public class BinDoubleReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            double[] arr = new double[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadDouble();
            return arr;
        }
        else
            return bin.ReadDouble();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            double[] arr = (double[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            double[] arr = (double[])obj;
            if (arr == null)
                arr = new double[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            for (; i < count; ++i)
                bin.Write(0.0);
        }
        else
            bin.Write((double)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(double);
    }
}
public class BinFloatReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            float[] arr = new float[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadSingle();
            return arr;
        }
        else
            return bin.ReadSingle();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            float[] arr = (float[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            float[] arr = (float[])obj;
            if (arr == null)
                arr = new float[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            for (; i < count; ++i)
                bin.Write(0.0f);
        }
        else
            bin.Write((float)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(float);
    }
}
public class BinShortReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            short[] arr = new short[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadInt16();
            return arr;
        }
        else
            return bin.ReadInt16();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            short[] arr = (short[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            short[] arr = (short[])obj;
            if (arr == null)
                arr = new short[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            const short b = 0;
            for (; i < count; ++i)
                bin.Write(b);
        }
        else
            bin.Write((short)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(short);
    }
}
public class BinUShortReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            ushort[] arr = new ushort[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadUInt16();
            return arr;
        }
        else
            return bin.ReadUInt16();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            ushort[] arr = (ushort[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            ushort[] arr = (ushort[])obj;
            if (arr == null)
                arr = new ushort[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            const ushort b = 0;
            for (; i < count; ++i)
                bin.Write(b);
        }
        else
            bin.Write((ushort)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(ushort);
    }
}
public class BinIntReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            int[] arr = new int[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadInt32();
            return arr;
        }
        else
            return bin.ReadInt32();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            int[] arr = (int[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            int[] arr = (int[])obj;
            if (arr == null)
                arr = new int[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            const int b = 0;
            for (; i < count; ++i)
                bin.Write(b);
        }
        else
            bin.Write((int)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(int);
    }
}
public class BinUIntReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            uint[] arr = new uint[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadUInt32();
            return arr;
        }
        else
            return bin.ReadUInt32();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            uint[] arr = (uint[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            uint[] arr = (uint[])obj;
            if (arr == null)
                arr = new uint[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            const uint b = 0;
            for (; i < count; ++i)
                bin.Write(b);
        }
        else
            bin.Write((uint)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(uint);
    }
}
public class BinInt64Reader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            Int64[] arr = new Int64[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadInt64();
            return arr;
        }
        else
            return bin.ReadInt64();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            Int64[] arr = (Int64[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            Int64[] arr = (Int64[])obj;
            if (arr == null)
                arr = new Int64[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            const Int64 b = 0;
            for (; i < count; ++i)
                bin.Write(b);
        }
        else
            bin.Write((Int64)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(Int64);
    }
}
public class BinUint64Reader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            UInt64[] arr = new UInt64[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadUInt64();
            return arr;
        }
        else
            return bin.ReadUInt64();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            UInt64[] arr = (UInt64[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            UInt64[] arr = (UInt64[])obj;
            if (arr == null)
                arr = new UInt64[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            const UInt64 b = 0;
            for (; i < count; ++i)
                bin.Write(b);
        }
        else
            bin.Write((UInt64)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(UInt64);
    }
}
public class BinSByteReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            sbyte[] arr = new sbyte[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadSByte();
            return arr;
        }
        else
            return bin.ReadSByte();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            sbyte[] arr = (sbyte[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            sbyte[] arr = (sbyte[])obj;
            if (arr == null)
                arr = new sbyte[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            const sbyte b = 0;
            for (; i < count; ++i)
                bin.Write(b);
        }
        else
            bin.Write((sbyte)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(sbyte);
    }
}
public class BinStringReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        byte[] data = bin.ReadBytes(count * 2);
        count = Utility.GetByteCharCount(data);
        return System.Text.Encoding.Unicode.GetString(data, 0, count);
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            string str = (string)obj;
            byte[] data = System.Text.Encoding.Unicode.GetBytes(str);
            bin.Write(data);
        }
        else
        {
            string str = (string)obj;
            if (str == null)
                str = new string('\0', count);
            byte[] data = System.Text.Encoding.Unicode.GetBytes(str);
            int minCount = Mathf.Min(str.Length, count);
            bin.Write(data, 0, minCount * 2);
            const short endChar = 0;
            for (int i = minCount; i < count; ++i)
                bin.Write(endChar);
        }
    }
    public short TypeSize()
    {
        return 0;
    }
}
public class BinDecimalReader : ITypeReader
{
    public object Read(BinaryReader bin, int count)
    {
        if (count > 0)
        {
            decimal[] arr = new decimal[count];
            for (int i = 0; i < count; ++i)
                arr[i] = bin.ReadDecimal();
            return arr;
        }
        else
            return bin.ReadDecimal();
    }
    public void Write(BinaryWriter bin, object obj, int count)
    {
        if (count == -1)
        {
            if (obj == null)
                return;
            decimal[] arr = (decimal[])obj;
            for (int i = 0; i < arr.Length; ++i)
                bin.Write(arr[i]);
        }
        else if (count > 0)
        {
            decimal[] arr = (decimal[])obj;
            if (arr == null)
                arr = new decimal[count];
            int minCount = Math.Min(arr.Length, count);
            int i = 0;
            for (; i < minCount; ++i)
                bin.Write(arr[i]);
            const decimal b = 0;
            for (; i < count; ++i)
                bin.Write(b);
        }
        else
            bin.Write((decimal)obj);
    }
    public short TypeSize()
    {
        return (short)sizeof(decimal);
    }
}
public class TypeReaderMapping
{
    public static Dictionary<int, ITypeReader> ms_TypeInfoList = new Dictionary<int,ITypeReader>();

    static bool RegisterType(Type t, ITypeReader reader)
    {
        int hash = Utility.GetHash(t.ToString());
        ms_TypeInfoList.Add(hash, reader);
        return true;
    }
    public static ITypeReader GetReaderFromHash(int hash)
    {
        ITypeReader reader;
        if (ms_TypeInfoList.TryGetValue(hash, out reader))
        {
            return reader;
        }
        else
            return null;
    }
    public static bool Init()
    {
        bool bRet = true;
        bRet &= RegisterType(typeof(bool),      new BinBoolReader());
        bRet &= RegisterType(typeof(byte),      new BinByteReader());
        bRet &= RegisterType(typeof(char),      new BinCharReader());
        bRet &= RegisterType(typeof(short),     new BinShortReader());
        bRet &= RegisterType(typeof(ushort),    new BinUShortReader());
        bRet &= RegisterType(typeof(int),       new BinIntReader());
        bRet &= RegisterType(typeof(uint),      new BinUIntReader());
        bRet &= RegisterType(typeof(Int64),     new BinInt64Reader());
        bRet &= RegisterType(typeof(UInt64),    new BinUint64Reader());
        bRet &= RegisterType(typeof(float),     new BinFloatReader());
        bRet &= RegisterType(typeof(double),    new BinDoubleReader());
        bRet &= RegisterType(typeof(decimal),   new BinDecimalReader());
        bRet &= RegisterType(typeof(string),    new BinStringReader());
        bRet &= RegisterType(typeof(sbyte),     new BinSByteReader());

        return bRet;
    }
    
}
