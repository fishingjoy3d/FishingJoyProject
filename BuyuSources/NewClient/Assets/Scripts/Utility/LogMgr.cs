using UnityEngine;
using System.Collections.Generic;
using System.IO;
public class LogMgr {
    static SafeList<string> ms_StrList = new SafeList<string>();
    public static void GlobalInit()
    {
    
    }
    //static void SaveStr(string str)
    //{
    //    FileStream fs = new FileStream(ms_LogFilePath, FileMode.Append);
    //    StreamWriter sw = new StreamWriter(fs, new System.Text.UTF8Encoding(false));
    //    sw.Write(str);
    //    sw.Close();
    //    fs.Close();
    //}
    public static string Update()
    {
        string str = "";
        while (ms_StrList.HasItem())
        {
            str += ms_StrList.GetItem() + "|";
        }
        if (str == "")
            return str;

        //SaveStr(str);
        return str;
    }
    public static void Log(string str)
    {
        if (ServerSetting.IS_TEST == false)
            return;
#if UNITY_EDITOR
        Debug.Log(str);
#endif
        ms_StrList.AddItem(str);
        //SaveStr(str+"\r\n");
       // Debug.Log(str);
    }
}
