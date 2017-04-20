using UnityEngine;
using System.Collections.Generic;
using System.IO;
using System.Xml;
public struct ResVersion
{
    public uint     ResCrc;
    public uint     ResSize;
}

public class VersionManager
{
    bool        m_bHasVerFile = true;
    float       m_Progress = 0;
    string      m_Error = null;
    ResVersion[] m_ResVer = new ResVersion[ResManager.RES_NUM];
    public void SetResVersion(ResType res, uint crc, uint ressize)
    {
        int idx = (int)res;
        m_ResVer[idx].ResSize = ressize;
        m_ResVer[idx].ResCrc = crc;
    }
    public bool CheckVerAndCrc(ResType res, uint crc, uint ressize)
    {
        int idx = (int)res;
        return m_ResVer[idx].ResCrc != crc || m_ResVer[idx].ResSize != ressize;
    }
    public void LoadInnerVersion()
    {
        TextAsset ta = (TextAsset)Resources.Load("InnerRes/Version", typeof(TextAsset));
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(ta.text);
        XmlElement ele = doc.DocumentElement;
        string verstr = ele.SelectSingleNode("Params/Version").FirstChild.Value;
        string resverstr = ele.SelectSingleNode("Params/Resource").FirstChild.Value;
        uint editor = uint.Parse(ele.SelectSingleNode("Params/Editor").FirstChild.Value);
        ServerSetting.SHOW_PING = uint.Parse(ele.SelectSingleNode("Params/Ping").FirstChild.Value) != 0;
        ServerSetting.RES_VERSION = Utility.VersionToUint(resverstr);
        ServerSetting.ClientVer = Utility.VersionToUint(verstr);
        SceneObjMgr.Instance.SetVerText("Version:" + verstr.Replace("_", ".").Remove(0, 2));
        int sdkType = int.Parse(ele.SelectSingleNode("Params/Sdk").FirstChild.Value);
#if UNITY_EDITOR
        editor = 1;
#endif
        if (int.Parse(ele.SelectSingleNode("Params/Test").FirstChild.Value) != 0)
            ServerSetting.IS_TEST = true;
        else
            ServerSetting.IS_TEST = false;
        XmlNode serverNode;
        if (editor == 1)
        {
            SDKMgr.SDK_TYPE = SdkType.SDK_SELF;
            serverNode = ele.SelectSingleNode("Server/Editor");
            ResManager.ENABLE_RESOURCES = true;
        }
        else
        {
            SDKMgr.SDK_TYPE = (SdkType)sdkType;
            if (ServerSetting.IS_TEST)
                serverNode = ele.SelectSingleNode("Server/Test");
            else
                serverNode = ele.SelectSingleNode("Server/Publish");
        }
        ServerSetting.VERSION_DIR = serverNode.Attributes["verdir"].Value;
        string resftp = serverNode.Attributes["resftp"].Value;
        string runftp = serverNode.Attributes["runftp"].Value;
        XmlNode reporterNode = ele.SelectSingleNode("Reporter");
        if (reporterNode != null)
        {
            ServerSetting.ReporterIP = reporterNode.FirstChild.Value;
            ServerSetting.ReporterPort = ushort.Parse(reporterNode.Attributes["port"].Value);
        }
        ServerSetting.SetResAndRunFTP(resftp, runftp);
        ServerSetting.SERVICES_URL = ele.SelectSingleNode("Services").FirstChild.Value;
        for(int i = 0; i < serverNode.ChildNodes.Count; ++i)
        {
            XmlNode node = serverNode.ChildNodes[i];
            if (node.Name == "IP")
            {
                ServerIPData sid = new ServerIPData();
                sid.IP = node.FirstChild.Value;
                sid.Port = ushort.Parse(node.Attributes["port"].Value);
                ServerSetting.ServerList.Add(sid);
            }
        }
        Resources.UnloadAsset(ta);
    }

    public void Init()
    {
        //读取自带的版本号
        LoadInnerVersion();

        //读取SD卡的版本
        byte[] data = RuntimeInfo.GetLocalFile(RuntimeInfo.GetVersionFileName());
        if (data == null || data.Length != ResManager.RES_NUM * 4 + 4)
            return;
        uint crc = Crc.Crc32(data, 4, data.Length - 4);
        if (crc != System.BitConverter.ToUInt32(data, 0))
            return;

        for (int i = 0, idx = 4; i < ResManager.RES_NUM; ++i, idx += 4)
        {
            uint resCrc = System.BitConverter.ToUInt32(data, idx);
            string path = RuntimeInfo.GetResPersistentPath((ResType)i);
            FileInfo fi = new FileInfo(path);
            if (!fi.Exists)
                continue;
            m_ResVer[i].ResSize = (uint)fi.Length;
            m_ResVer[i].ResCrc = resCrc;
        }
        m_bHasVerFile = true;
    }
    public float Progress
    {
        get
        {
            return m_Progress;
        }
    }
    public string UnzipError
    {
        get
        {
            return m_Error;
        }
    }
    public bool HasVersionFile
    {
        get
        {
            return m_bHasVerFile;
        }
    }
    
    public System.Collections.IEnumerator UnzipProcess(object obj)
    {
        float step = 0.5f / ResManager.RES_NUM;
        for (int i = 0; i < ResManager.RES_NUM; ++i)
        {
            ResType rt = (ResType)i;
            string path = RuntimeInfo.GetResStreamingPath(rt);
            string destPath = RuntimeInfo.GetResPersistentPath(rt);
            string destPathTemp = destPath + ".temp";
            WWW www = new WWW(path);
            while (www.isDone == false)
                yield return new WaitForEndOfFrame();
            if (www.bytes == null || www.bytes.Length == 0)
            {
                m_Error = "资源解压失败:" + path;
                LogMgr.Log("解压失败，未找到文件:" + www.error + ", Path:" + path);
                yield break;
            }
            try
            {
                FileInfo fi = new FileInfo(destPathTemp);
                if (fi.Exists)
                    fi.Delete();
                FileStream fss = fi.Create();
                fss.Write(www.bytes, 0, www.bytes.Length);
                fss.Flush();
                fss.Close();
                LZMA.DecompressFile(destPathTemp, destPath);
                if (fi.Exists)
                    fi.Delete();
                m_Progress += step;
            }
            catch(System.Exception e)
            {
                m_Error = "资源解压失败:" + path;
                ReportException.Instance.AddException("Exception:解压文件失败: " + e.ToString());
                yield break;
            }
            yield return new WaitForEndOfFrame();
            try
            {
                m_ResVer[i].ResCrc = Crc.Crc32FromFile(destPathTemp);
                FileInfo ff = new FileInfo(destPath);
                m_ResVer[i].ResSize = (uint)ff.Length;
                m_Progress += step;
            }
            catch(System.Exception e)
            {
                m_Error = "计算CRC失败:" + path;
                ReportException.Instance.AddException("Exception:计算文件CRC失败: " + e.ToString());
                yield break;
            }
            File.Delete(destPathTemp);
            yield return new WaitForEndOfFrame();
        }
        m_bHasVerFile = true;
        m_Progress = 1.0f;
        if (!SaveVersion())
        {
            ReportException.Instance.AddException("Exception:保存版本文件失败:");
        }
        yield break;
    }
    public void FirstUnzipFile()
    {
        SceneMain.Instance.StartInnerCoroutine(UnzipProcess(null));
    }
    public bool SaveVersion()
    {
        byte[] data;
        MemoryStream ms = new MemoryStream(ResManager.RES_NUM * 4 + 4);
        for (int i = 0; i < ResManager.RES_NUM; ++i)
        {
            data = System.BitConverter.GetBytes(m_ResVer[i].ResCrc);
            ms.Write(data, 0, data.Length);
        }
        data = ms.ToArray();
        ms.Close();

        uint crc = Crc.Crc32(data, 0, data.Length);
        byte[] crcValue = System.BitConverter.GetBytes(crc);
        MemoryStream ms2 = new MemoryStream(data.Length + crcValue.Length);
        ms2.Write(crcValue, 0, crcValue.Length);
        ms2.Write(data, 0, data.Length);
        bool bRet = RuntimeInfo.SaveLocalFile(RuntimeInfo.GetVersionFileName(), ms2.ToArray());
        ms2.Close();
        return bRet;
    }
    public ResVersion[] ResVer
    {
        get { return m_ResVer; }
        set { m_ResVer = value; }
    }
}
