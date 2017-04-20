using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class ServerStringHandle
{
    //public static bool CheckString(string str)//字母 +  数组  用于检测 账号 密码 
    //{
    //    if(str == null )
    //         return false;
    //    string NewStr = str.Trim();
    //    if (NewStr == null || NewStr.Length == 0 || NewStr == "")
    //        return false;
    //    //基本判断完毕 判断一些特殊的符号
    //    foreach(char ch in NewStr)
    //    {
    //        if (!IsCanUseChar(ch))
    //            return false;
    //    }
    //    return true;
    //}
    //public static bool CheckUserString(string str)
    //{
    //    //检测中文 或者 玩家自行定义的字符串
    //    if (str == null)
    //        return false;
    //    string NewStr = str.Trim();
    //    if (NewStr == null || NewStr.Length == 0 || NewStr == "")
    //        return false;

    //    return true;
    //}
    //private static bool IsCanUseChar(char ch)
    //{
    //    byte nValue = (byte)ch;
    //    if ((nValue >= 48 && nValue <= 57) || (nValue>=65 && nValue<= 90) || (nValue >= 97 && nValue <= 122))
    //        return true;
    //    else
    //        return false;
    //}
    public static string HandleServerString(string str)
    {
        //处理服务器发送来的字符串 转化为客户端可以识别的格式
        return HandleItemName(HandleMonthName(str));
    }

    private static string HandleMonthName(string str)
    {
        //{MonthName:MonthID=%d}
        if (str.IndexOf("{MonthName:MonthID=") == -1 || str.IndexOf("}") == -1)
            return str;
        int BeginIndex = str.IndexOf("{MonthName:MonthID=") + ("{MonthName:MonthID=").Length;
        int EndIndex = str.Substring(BeginIndex).IndexOf("}") + BeginIndex;
        int MonthID = Convert.ToInt32(str.Substring(BeginIndex, EndIndex - BeginIndex));
        string replaceStr =  str.Substring(str.IndexOf("{MonthName:MonthID="),str.IndexOf("}")-str.IndexOf("{MonthName:MonthID=") +1);
        tagMonthStr pInfo;
        if(!FishConfig.Instance.m_MatchInfo.m_MonthStr.TryGetValue((Byte)MonthID,out pInfo))
        {
            return str.Replace(replaceStr," ");
        }
        string MonthName = pInfo.MonthName;
        return str.Replace(replaceStr,MonthName);
    }
    private static string HandleItemName(string str)
    {
        //{MonthName:MonthID=%d}
        if (str.IndexOf("{ItemName:ItemID=") == -1 || str.IndexOf("}") == -1)
            return str;
        int BeginIndex = str.IndexOf("{ItemName:ItemID=") + ("{ItemName:ItemID=").Length;
        int EndIndex = str.Substring(BeginIndex).IndexOf("}") + BeginIndex;
        UInt32 ItemID = Convert.ToUInt32(str.Substring(BeginIndex, EndIndex - BeginIndex));
        string replaceStr = str.Substring(str.IndexOf("{ItemName:ItemID="), str.IndexOf("}") - str.IndexOf("{ItemName:ItemID=") + 1);
        tagItemConfig pInfo;
        if (!FishConfig.Instance.m_ItemInfo.m_ItemMap.TryGetValue(ItemID, out pInfo))
        {
            return str.Replace(replaceStr, " ");
        }
        string ItemName = pInfo.ItemName;
        return str.Replace(replaceStr, ItemName);
    }
}