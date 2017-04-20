using System.Collections.Generic;
using UnityEngine;
using System.Xml;

struct StringTableOnce
{
    public string ShowInfo;
    public byte ShowTime;
}
public class StringTable
{
    static Dictionary<int, StringTableOnce> m_StringList = new Dictionary<int, StringTableOnce>();
    public static string GetString(string name)
    {
        int k = Utility.GetHash(name);
        StringTableOnce text;
        if (m_StringList.TryGetValue(k, out text))
        {
            return text.ShowInfo;
        }
        else
            return "";
    }
    public static byte GetStringShowTime(string name, out string info)
    {
        int k = Utility.GetHash(name);
        StringTableOnce text;

        if (m_StringList.TryGetValue(k, out text))
        {
            info = text.ShowInfo;
            return text.ShowTime;
        }
        else
        {
            info = "";
            return 0;
        }
    }
    public static bool GlobalInit()
    {
        Object obj = Resources.Load("InnerRes/InnerStringTable", typeof(TextAsset));
        if (obj == null)
            return false;
        TextAsset ta = obj as TextAsset;
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(ta.text);
        XmlElement ele = doc.DocumentElement;
        foreach (XmlNode node in ele.ChildNodes)
        {
            if(node.Name == "#comment")
                continue;

            int k = Utility.GetHash(node.Name);
            StringTableOnce pOnce = new StringTableOnce();
            if (node.Attributes["IsShow"] == null)
                pOnce.ShowTime = 1;
            else
                pOnce.ShowTime = System.Convert.ToByte(node.Attributes["IsShow"].Value);
            if(pOnce.ShowTime == 0)
                pOnce.ShowInfo = "";
            else
                pOnce.ShowInfo = node.FirstChild.Value;
            m_StringList.Add(k, pOnce);
        }
        Resources.UnloadAsset(obj);
        return true;
    }
}
