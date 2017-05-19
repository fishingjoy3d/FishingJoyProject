using System;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;

public class TypeData
{
    public int              HashCode;
    public Type             ClassType;
    public short            Size;       //数组的大小
    public short            OrgSize;    //原始的大小
    public short            ArrayCount;
    public bool             VarType;
    public FieldInfo        Handle;
    public TypeData[]       Fields;
}
public class TypeFiledPack
{
    public FieldInfo    Field;
    public short        Idx;
    public short        Count;
    public bool         VarType;
    public TypeFiledPack(short idx, short count, bool varType, FieldInfo fi)
    {
        Idx = idx;
        Count = count;
        Field = fi;
        VarType = varType;
    }
}
public class TypeSortedField
{
    public List<TypeFiledPack> Fields = new List<TypeFiledPack>();
    public bool AddField(FieldInfo fi, short idx, short count,bool varType)
    {
        for (int i = 0; i < Fields.Count; ++i)
        {
            if (idx < Fields[i].Idx)
            {
                Fields.Insert(i, new TypeFiledPack(idx, count, varType, fi));
                return true;
            }
            if(idx == Fields[i].Idx)
            {
                Debug.Log("字段的特性索引TypeInfo不能重复:" + fi.ToString());
                return false;
            }
        }
        Fields.Add(new TypeFiledPack(idx, count, varType, fi));
        return true;
    }
}
public class TypeMapping
{
    public static int STRING_HASH = Utility.GetHash(typeof(string).ToString());
    static Dictionary<int, TypeData> ms_FieldList = new Dictionary<int, TypeData>();
    static TypeInfo GetFieldTypeInfo(FieldInfo fi)
    {
        object[] attri = fi.GetCustomAttributes(false);
        if (attri != null && attri.Length > 0)
        {
            TypeInfo arr = attri[0] as TypeInfo;
            if (arr == null)
            {
                LogMgr.Log("未知的特性:" + attri[0].ToString());
            }
            return arr;
        }
        return null;
    }
    public static List<TypeSortedField> SortFields(FieldInfo[] t, out int count)
    {
        count = 0;
        List<TypeSortedField> fields = new List<TypeSortedField>();
        TypeSortedField sortField = new TypeSortedField();
        foreach (FieldInfo fi in t)
        {
            if (fi.IsStatic) continue;

            TypeInfo ti = GetFieldTypeInfo(fi);
            if(ti == null)
                return null;
            ++count;
            if (sortField.Fields.Count == 0)
            {
                if (!sortField.AddField(fi, ti.Index, ti.Count, ti.VarType))
                    return null;
                fields.Add(sortField);
                continue;
            }
            if(sortField.Fields[0].Field.DeclaringType != fi.DeclaringType)
            {
                sortField = new TypeSortedField();
                if (!sortField.AddField(fi, ti.Index, ti.Count, ti.VarType))
                    return null;
                fields.Insert(0, sortField);
            }
            else
            {
                if (!sortField.AddField(fi, ti.Index, ti.Count, ti.VarType))
                    return null;
            }
        }
        return fields;
    }
    public static TypeData RegisterClass(Type t)
    {
        int fieldCount;
        List<TypeSortedField> sortFields = SortFields(t.GetFields(), out fieldCount);
        if (sortFields == null)
            return null;

        TypeData td = new TypeData();
        td.ClassType = t;
        td.HashCode = Utility.GetHash(td.ClassType.ToString());
        td.Handle = null;
        td.Size = 0;
        td.ArrayCount = 1;
        td.Fields = new TypeData[fieldCount];
        int idx = 0;
        for (int n = 0; n < sortFields.Count; ++n)
        {
            for (int i = 0; i < sortFields[n].Fields.Count; ++i)
            {
                TypeFiledPack field = sortFields[n].Fields[i];
                FieldInfo fi = field.Field;
                TypeData curData = null;

                if (fi.FieldType.IsPrimitive)
                {
                    curData = new TypeData();
                    curData.Handle = fi;
                    curData.ArrayCount = 1;
                    curData.ClassType = fi.FieldType;
                    curData.HashCode = Utility.GetHash(curData.ClassType.ToString());
                    curData.OrgSize = curData.Size = TypeReaderMapping.GetReaderFromHash(curData.HashCode).TypeSize();
                }
                else if (fi.FieldType.IsArray)
                {
                    //获取原始类型
                    string strType = fi.FieldType.ToString();
                    strType = strType.Remove(strType.Length - 2, 2);
                    Type orgType = Type.GetType(strType);
                    if (orgType == null)
                    {
                        LogMgr.Log("未知的数组类型:" + strType);
                        return null;
                    }
                    if(field.Count < 1 && field.VarType == false)
                    {
                        LogMgr.Log("数组的特性TypeInfo必须大于0，或者是变长的数组");
                        return null;
                    }
                    if (orgType.IsPrimitive)
                    {
                        curData = new TypeData();
                        curData.ClassType = orgType;
                        curData.HashCode = Utility.GetHash(curData.ClassType.ToString());
                        curData.ArrayCount = field.Count;
                        curData.VarType = field.VarType;
                        curData.OrgSize = curData.Size = TypeReaderMapping.GetReaderFromHash(curData.HashCode).TypeSize();
                        if (curData.VarType)
                            curData.Size = 0;
                        else
                            curData.Size = (short)(curData.Size * field.Count);
                        curData.Handle = fi;
                    }
                    else if (orgType.IsClass/* || orgType.IsValueType*/)
                    {
                        curData = RegisterClass(orgType);
                        if (curData == null) return null;
                        curData.ArrayCount = field.Count;
                        curData.VarType = field.VarType;
                        
                        if (curData.VarType)
                            curData.Size = 0;
                        else
                            curData.Size = (short)(curData.Size * field.Count);
                        curData.Handle = fi;
                    }
                    else
                    {
                        LogMgr.Log("未知的数组原始类型:" + strType);
                        return null;
                    }
                }
                else if (fi.FieldType == typeof(string))
                {
                    curData = new TypeData();
                    curData.ArrayCount = field.Count;
                    curData.OrgSize = (short)sizeof(char);
                    curData.VarType = field.VarType;
                    if (curData.VarType)
                        curData.Size = 0;
                    else
                        curData.Size = (short)(sizeof(char) * field.Count);
                    curData.ClassType = fi.FieldType;
                    curData.HashCode = STRING_HASH;
                    curData.Handle = fi;
                }
                else if (fi.FieldType.IsClass /*|| fi.FieldType.IsValueType*/)
                {
                    curData = RegisterClass(fi.FieldType);
                    if (curData == null) return null;
                    curData.Handle = fi;
                }
                else
                {
                    LogMgr.Log("未知的类型:" + fi.FieldType.ToString());
                    return null;
                }
                td.Fields[idx ++] = curData;
                td.Size += curData.Size;
            }
        }
        td.OrgSize = td.Size;
        return td;
    }
    public static bool RegisterClassFromType<T>()
    {
        Type t = typeof(T);
        int hash = Utility.GetHash(t.ToString());
        if(ms_FieldList.ContainsKey(hash))
            return true;
        TypeData td = RegisterClass(t);
        if (td != null)
        {
            //注册
            ms_FieldList.Add(td.HashCode, td);
            return true;
        }
        else
            return false;
    }
    public static TypeData FindType<T>()
    {
        return FindType(TypeSize<T>.HASH);
    }
    public static TypeData FindType(int hash)
    {
        TypeData td;
        if (ms_FieldList.TryGetValue(hash, out td))
            return td;
        else
            return null;
    }
    public static TypeData FindType(Type t)
    {
        int hash = Utility.GetHash(t.ToString());
        return FindType(hash);
    }
    public static bool GlobalInit()
    {
        if (!TypeReaderMapping.Init())
            return false;

        bool bRet = true;
        //在此注册要反序列化的类或结构名称
        //===================================================
        //bRet &= RegisterClassFromType<C>();
       
        
        //===================================================
        if(!bRet)
        {
            LogMgr.Log("TypeMapping初始化失败!");
        }
        return bRet;
    }
}
