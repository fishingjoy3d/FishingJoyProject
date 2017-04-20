using UnityEngine;
using System.Collections.Generic;
using System.IO;
using System.Xml;
enum UpdateState
{
    UPDATE_CHECK_LOCALVER,
    UPDATE_UNZIPING_FILE,
    UPDATE_INIT,
    UPDATE_DOWNLOAD_VERSION,
    UPDATE_DOWNLOAD_RES,
    UPDATE_DOWNLOAD_CLIENT,
    UPDATE_COMPLETE,
    UPDATE_NEW_CLIENT,

}
public class DownResData
{
    public ResType ResType;
    public string ResUrl;
    public uint ResCrc;
    public uint ResSize;
    public uint ResUnzipSize;
}
struct VerData
{
    public string url;
    public uint crc;
    public uint resSize;
    public uint unzipResSize;
}
public class UpdateLogic : ILogic
{
    const int   MAX_RETRY_COUNT         = 3;
    const uint  UPDATE_VERSION_TIME_OUT = 30000;
    const uint  UPDATE_RES_TIME_OUT     = 1000 * 60 * 2;

    UpdateState m_State;
    DownloadData m_ActiveDown = null;
    List<DownResData> m_DownList = new List<DownResData>();
    int m_RetryCount;
    uint m_UpdateTick;
    UpdateLogicUI m_UpdateUI;
    uint m_DownloadBytes = 0;
    uint m_CurrentDownloadBytes = 0;
    uint m_AllDownloadBytes = 0;
    bool m_bNewClient = false;
    string m_NewClientURL = "";
    int m_RecvFileCount = 0;
    int[] m_WinSizeList;
    volatile ushort m_SaveCount = 0;
    volatile bool m_bRun = true;
    volatile bool m_bError = false;

    SafeList<MultiFileOK> m_ThreadList = new SafeList<MultiFileOK>();
    List<ServerIPData> m_ResFtpList = new List<ServerIPData>();
    public bool SaveWindowClientRes(byte[] clientData)
    {
        string[] strList = new string[]{
        RuntimeInfo.GetDataDir() + "/Managed/Assembly-CSharp.dll",
        RuntimeInfo.GetDataDir() + "/maindata",
        RuntimeInfo.GetDataDir() + "/resources.assets",
        RuntimeInfo.GetDataDir() + "/screenselector.bmp",
        RuntimeInfo.GetDataDir() + "/sharedassets0.assets"
        };
        int offset = 0;
        for (int i = 0; i < strList.Length; ++i)
        {
            try
            {
                FileStream fsTemp = File.Create(strList[i]);
                fsTemp.Write(clientData, offset, m_WinSizeList[i]);
                fsTemp.Close();
            }
            catch(System.Exception e)
            {
                LogMgr.Log("Write Err:" + e.ToString());
            }
            offset += m_WinSizeList[i];
        }
        return true;
    }
    public bool Init(ILogicUI logicUI, object obj)
    {
        if (RuntimeInfo.CheckResDirExists() == false)
        {
            NativeInterface.ShowMsg("start_error", 100);
            return false;
        }
        System.Threading.Thread th = new System.Threading.Thread(new System.Threading.ThreadStart(DownLoadThread));
        th.Start();
        m_State = UpdateState.UPDATE_INIT;
        m_UpdateUI = (UpdateLogicUI)logicUI;
        return true;
    }
    public void GetDownBytes(out uint downBytes, out uint allbytes)
    {
        if (m_ActiveDown != null && m_ActiveDown.Data is DownResData)
        {
            downBytes = m_ActiveDown.DownloadBytes / 1024;
            allbytes = ((DownResData)m_ActiveDown.Data).ResSize / 1024 + 1;
        }
        else
        {
            downBytes = 0;
            allbytes = 0;
        }
    }
    public float DownloadProgress
    {
        get
        {
            if (m_State == UpdateState.UPDATE_UNZIPING_FILE)
            {
                return ResManager.Instance.VersionMgr.Progress;
            }
            else
            {
                return (float)(m_CurrentDownloadBytes + m_DownloadBytes) / (m_AllDownloadBytes + 0.00001f);
            }
        }
    }
    public void CheckLocalFile()
    {
        if (ResManager.Instance.VersionMgr.HasVersionFile == false)
        {
            //第一次解压文件
            m_State = UpdateState.UPDATE_UNZIPING_FILE;
            m_UpdateUI.SetLoadingType(true);
            ResManager.Instance.VersionMgr.FirstUnzipFile();
        }
        else
            m_State = UpdateState.UPDATE_INIT;
    }
    public void CheckUnzipping()
    {
        if (ResManager.Instance.VersionMgr.Progress == 1.0f)
        {
            m_UpdateUI.SetLoadingType(false);
            m_State = UpdateState.UPDATE_INIT;
        }
        else if (ResManager.Instance.VersionMgr.UnzipError != null)
        {
            //解压失败
            ReportException.Instance.AddException("解压失败:" + ResManager.Instance.VersionMgr.UnzipError);
            m_bError = true;
            NativeInterface.ShowMsgAndExit("unzipfileerr", 139);
            m_State = UpdateState.UPDATE_COMPLETE;
        }
    }
    public uint DownResQueue
    {
        get
        {
            return m_ActiveDown.Queue;
        }
    }
    public void Update(float delta)
    {
        if (m_bError)
        {
            NativeInterface.ShowMsgAndExit("res_save_error", 109);
            return;
        }
        if (m_ActiveDown != null)
        {
            if(m_ActiveDown.Type == DownloadType.DOWNLOAD_MULTI_FILE && m_ActiveDown.DownloadBytes != 0)
            {
            }
            else
                m_UpdateUI.SetIsQueue(m_ActiveDown.IsQueue, m_ActiveDown.Type == DownloadType.DOWNLOAD_CHECK_VER);
            if (m_ActiveDown.IsQueue)
                m_UpdateTick = Utility.GetTickCount();
        }
        switch (m_State)
        {
            case UpdateState.UPDATE_CHECK_LOCALVER:
                CheckLocalFile();
                break;
            case UpdateState.UPDATE_UNZIPING_FILE:
                CheckUnzipping();
                break;
            case UpdateState.UPDATE_INIT:
                m_UpdateTick = Utility.GetTickCount();
                string ftpurl = RuntimeInfo.GetFTPVersionURL();
                m_ActiveDown = FTPClient.Instance.AddDownload(DownloadType.DOWNLOAD_CHECK_VER, ftpurl, null);
                m_State = UpdateState.UPDATE_DOWNLOAD_VERSION;
                break;
            case UpdateState.UPDATE_DOWNLOAD_VERSION:
                if (m_ActiveDown.IsOK)
                {
                    m_RetryCount = 0;
                    string xml = m_ActiveDown.Text;
                    m_ActiveDown = null;
                    if (!CheckVersion(xml))
                        NativeInterface.ShowMsgAndExit("update_error", 101);
                }
                else if (m_ActiveDown.HasError)
                {
                    //出错了
                    if (++m_RetryCount > MAX_RETRY_COUNT)
                    {

                        NativeInterface.ShowMsgAndExit("res_connect_error", 102);
                    }
                    else
                    {
                        m_State = UpdateState.UPDATE_INIT;
                        m_ActiveDown = null;
                    }
                }
                else
                {
                    if (m_ActiveDown.IsQueue == false && Utility.GetTickCount() - m_UpdateTick > UPDATE_VERSION_TIME_OUT)
                    {
                        NativeInterface.ShowMsgAndExit("res_connect_error", 103);
                    }
                }
                break;
            case UpdateState.UPDATE_DOWNLOAD_RES:
                if (m_ActiveDown == null)
                {
                    if (m_DownList.Count > 0)
                    {
                        m_DownloadBytes = 0;
                        m_UpdateTick = Utility.GetTickCount();
                        m_UpdateUI.BeginDown(ResType.FishRes);
                        m_ActiveDown = FTPClient.Instance.AddMultiResDownload(DownloadType.DOWNLOAD_MULTI_FILE, m_ResFtpList, m_DownList);
                        m_RecvFileCount = m_DownList.Count;
                        m_DownList.Clear();
                        
                    }
                    else
                    {
                        m_State = UpdateState.UPDATE_COMPLETE;
                    }
                }
                else if (m_ActiveDown.IsOK)
                {
                    m_RetryCount = 0;
                    m_CurrentDownloadBytes += m_ActiveDown.DownloadBytes;
                    m_DownloadBytes = 0;
                    //m_ThreadList.AddItem(m_ActiveDown);
                    SaveDownloadData(m_ActiveDown);
                    
                    m_ActiveDown = null;
                }
                else if (m_ActiveDown.HasError)
                {
                    NativeInterface.ShowMsgAndExit("update_error", 105);
                }
                else
                {
                    if (m_ActiveDown.IsQueue)
                    {
                        //排队中
                    }
                    else if (m_DownloadBytes != m_ActiveDown.DownloadBytes)
                    {
                        m_DownloadBytes = m_ActiveDown.DownloadBytes;
                        m_UpdateTick = Utility.GetTickCount();
                        MultiFileDownData mfdd = (MultiFileDownData)m_ActiveDown.Data;
                        if (mfdd.OutsideRecvIndex < mfdd.RecvCount)
                        {
                            SaveDownloadData(m_ActiveDown);
                        }
                    }
                    else if (Utility.GetTickCount() - m_UpdateTick > UPDATE_RES_TIME_OUT)
                    {
                        NativeInterface.ShowMsgAndExit("update_error", 106);
                    }
                }
                break;
            case UpdateState.UPDATE_COMPLETE:
                //if (m_DownloadCompletionCount != m_DownloadOrgCount)
                //{
                //    m_UpdateUI.SetUnzipping();
                //    return;
                //}
                //更新完成，进入下一个逻辑
                if(m_SaveCount != m_RecvFileCount)
                {
                    return;
                }
                if (m_bNewClient)
                {
                    
                    if(RuntimeInfo.GetPlatform() == PlatformType.Android)
                    {
                        NativeInterface.DownNewClientVersion(GetClientPath());
                    }
                    else if(RuntimeInfo.GetPlatform() == PlatformType.Windows)
                    {
                        NativeInterface.ShowMsgAndExit("update_restart", 0);
                    }
                    else
                    {
                        NativeInterface.DownNewClientVersion(m_NewClientURL);
                    }
                }
                else
                    LogicManager.Instance.Forward(null);
                break;
            case UpdateState.UPDATE_NEW_CLIENT:
                LogicManager.Instance.Shutdown();
                break;
        }
    }
    public void Shutdown()
    {
        m_bRun = false;
    }
    string GetClientPath()
    {
        return RuntimeInfo.GetResPersistentPath(ResType.MAX);
    }
    bool SaveClient(MultiFileOK dd)
    {
        //验证crc
        DownResData drd = dd.Drd;
        if (drd.ResSize != dd.Data.Length)
        {
            LogMgr.Log("Client:" + drd.ResType + "大小不匹配,localSize:" + dd.Data.Length + ", serverSize:" + drd.ResSize);
            return false;
        }
        uint crc = Crc.Crc32(dd.Data, 0, dd.Data.Length);
        if (crc != drd.ResCrc)
        {
            LogMgr.Log("Client:" + drd.ResType + "资源检验失败, size:" + dd.Data.Length + ", localCrc:" + crc + ", serverCrc:" + drd.ResCrc);
            return false;
        }
        if(RuntimeInfo.GetPlatform() == PlatformType.Windows)
        {
            SaveWindowClientRes(dd.Data);
            return true;
        }
        string path = GetClientPath();
        try
        {
            File.Delete(path);
        }
        catch
        {
        }
        FileStream fs = File.Create(path);
        if (fs == null)
        {
            LogMgr.Log("Client文件创建失败:" + drd.ResType);
            return false;
        }
        fs.Write(dd.Data, 0, dd.Data.Length);
        fs.Flush();
        fs.Close();

        return true;
    }

    bool SaveRes(MultiFileOK dd)
    {
        //验证crc
        DownResData drd = dd.Drd;
        if (drd.ResSize != dd.Data.Length)
        {
            LogMgr.Log("Res:" + drd.ResType + " 大小不匹配,localSize:" + dd.Data.Length + ", serverSize:" + drd.ResSize);
            return false;
        }
        uint crc = Crc.Crc32(dd.Data, 0, dd.Data.Length);
        if (crc != drd.ResCrc)
        {
            LogMgr.Log("Res:" + drd.ResType + "资源检验失败, size:" + dd.Data.Length + ", localCrc:" + crc + ", serverCrc:" + drd.ResCrc);
            return false;
        }

        System.Random rr = new System.Random();
        int name = rr.Next(100, 10000);
        string tempFile = RuntimeInfo.GetLocalPath(drd.ResType + name.ToString() + "_temp.dat");
        string path = RuntimeInfo.GetResPersistentPath(drd.ResType);
        try
        {
            File.Delete(tempFile);
        }
        catch
        {
        }
        try
        {
            File.Delete(path);
        }
        catch
        {
        }
        try
        {
            FileStream fsTemp = File.Create(tempFile);
            if (fsTemp == null)
            {
                LogMgr.Log("RES文件创建失败:" + drd.ResType);
                return false;
            }
            //解压
            fsTemp.Write(dd.Data, 0, dd.Data.Length);
            fsTemp.Flush();
            fsTemp.Close();
            fsTemp = null;
            LZMA.DecompressFile(tempFile, path);
        }
        catch (System.Exception e)
        {
            ReportException.Instance.AddException("文件解压失败:" + e.ToString());
        }
        try
        {
            File.Delete(tempFile);
        }
        catch
        {
        }
        if (File.Exists(path) == false)
        {
            LogMgr.Log("RES文件解压失败:" + drd.ResType);
            return false;
        }
        ResManager.Instance.VersionMgr.SetResVersion(drd.ResType, drd.ResCrc, drd.ResUnzipSize);
        return ResManager.Instance.VersionMgr.SaveVersion();
    }
    bool CheckClientFile(uint crc)
    {
        string path = GetClientPath();
        if (File.Exists(path))
            return Crc.Crc32FromFile(path) == crc;
        else
            return false;
    }
    public bool CheckVersion(string xml)
    {
        XmlDocument doc = new XmlDocument();
        if (doc == null)
            return false;
        doc.LoadXml(xml);
        XmlElement ele = doc.DocumentElement;
        if (ele == null)
            return false;
        XmlNode nodeVer = ele.SelectSingleNode("Net");
        if (nodeVer == null)
            return false;
        string resftp = nodeVer.Attributes["resftp"].Value;
        string runftp = nodeVer.Attributes["runftp"].Value;
        string clientUrl = nodeVer.Attributes["clienturl"].Value;
        if (nodeVer.Attributes["ftpport"] != null)
            ServerSetting.ResFtpPort = ushort.Parse(nodeVer.Attributes["ftpport"].Value);
        ServerSetting.SetRunFTP(runftp);
        ServerSetting.ServerList.Clear();
        for (int i = 0; i < nodeVer.ChildNodes.Count; ++i)
        {
            XmlNode node = nodeVer.ChildNodes[i];
            if (node.Name == "IP")
            {
                ServerIPData sid = new ServerIPData();
                sid.IP = node.FirstChild.Value;
                sid.Port = ushort.Parse(node.Attributes["port"].Value);
                ServerSetting.ServerList.Add(sid);
            }
            else if (node.Name == "FTP")
            {
                ServerIPData rfd = new ServerIPData();
                rfd.IP = node.FirstChild.Value;
                rfd.Port = ushort.Parse(node.Attributes["port"].Value);
                rfd.ISP = byte.Parse(node.Attributes["dx"].Value);
                m_ResFtpList.Add(rfd);
            }
        }

        ServerSetting.SERVICES_URL = "";// ele.SelectSingleNode("Services").FirstChild.Value;
        XmlNode reporterNode = ele.SelectSingleNode("Reporter");
        if (reporterNode != null)
        {
            ServerSetting.ReporterIP = reporterNode.FirstChild.Value;
            ServerSetting.ReporterPort = ushort.Parse(reporterNode.Attributes["port"].Value);
        }
        XmlNode ShareNode = ele.SelectSingleNode("Share");
        if (ShareNode != null)
        {
            string wxid = "", wxpwd = "", sinaid = "", sinapwd = "", qqid = "", qqpwd = "";
            XmlNode xn = ShareNode.ChildNodes[0];
            if (xn != null)
            {
                wxid = xn.Attributes["id"].Value;
                wxpwd = xn.Attributes["pwd"].Value;
            }
            xn = ShareNode.ChildNodes[1];
            if (xn != null)
            {
                sinaid = xn.Attributes["id"].Value;
                sinapwd = xn.Attributes["pwd"].Value;
            }
            xn = ShareNode.ChildNodes[2];
            if (xn != null)
            {
                qqid = xn.Attributes["id"].Value;
                qqpwd = xn.Attributes["pwd"].Value;
            }
            NativeInterface.InitShare(wxid, wxpwd, sinaid, sinapwd, qqid, qqpwd);
            xn = ShareNode.ChildNodes[3];
            if (xn != null)
            {
                ServerSetting.ShareWebUrl = xn.Attributes["weburl"].Value;
                ServerSetting.ShareTxt = xn.Attributes["txt"].Value;
                ServerSetting.ShareImgUrl = xn.Attributes["imgurl"].Value;
            }

        }
        //读取现有的资源，进行比较
        XmlNode clientNode = ele.SelectSingleNode("Client");
        string clientVer = clientNode.FirstChild.Value; ;
        uint clientCrc = uint.Parse(clientNode.Attributes["crc"].Value);
        uint clientSize = uint.Parse(clientNode.Attributes["size"].Value);

        if (clientNode.Attributes["resver"] != null)
            ServerSetting.RES_VERSION = uint.Parse(clientNode.Attributes["resver"].Value);
        if (clientNode.Attributes["ping"] != null)
            ServerSetting.SHOW_PING = uint.Parse(clientNode.Attributes["ping"].Value) != 0;
        if (clientNode.Attributes["extrabtn"] != null)
            ServerSetting.ShowExtraBtn = uint.Parse(clientNode.Attributes["extrabtn"].Value) != 0;
        if (clientNode.Attributes["ftp_newftp"] != null)
            FTPClient.USE_NEW_FTP = byte.Parse(clientNode.Attributes["ftp_newftp"].Value);
        if (clientNode.Attributes["showgame"] != null)
            ServerSetting.ShowGame = byte.Parse(clientNode.Attributes["showgame"].Value) != 0;
        if (clientNode.Attributes["ftp_chunkcount"] != null)
            FTPClient.MAX_CHUNK_COUNT = byte.Parse(clientNode.Attributes["ftp_chunkcount"].Value);
        if (clientNode.Attributes["ftp_chunksize"] != null)
            FTPClient.CHUNK_SIZE = int.Parse(clientNode.Attributes["ftp_chunksize"].Value);
        if (clientNode.Attributes["thirdbtn"] != null)
            ServerSetting.ShowHallThirdBtn = int.Parse(clientNode.Attributes["thirdbtn"].Value) != 0;
        if (clientNode.Attributes["exchange"] != null)
            ServerSetting.ShowExchange = int.Parse(clientNode.Attributes["exchange"].Value) != 0;
        if (clientNode.Attributes["showjbp"] != null)
            ServerSetting.ShowJBP = int.Parse(clientNode.Attributes["showjbp"].Value) != 0;
        if (clientNode.Attributes["showmatch"] != null)
            ServerSetting.ShowMatch = int.Parse(clientNode.Attributes["showmatch"].Value) != 0;
        if (clientNode.Attributes["showthirdlogin"] != null)
            ServerSetting.ShowThirdLoginBtn = int.Parse(clientNode.Attributes["showthirdlogin"].Value) != 0;

        m_State = UpdateState.UPDATE_COMPLETE;
        //检查客户端版本号
        if (clientVer == null)
            return false;
        uint ver = Utility.VersionToUint(clientVer);
        if (ver > ServerSetting.ClientVer)
        {
            //clientUrl = ReplaceFTPAddress(clientUrl);
            //版本更新
            if (RuntimeInfo.GetPlatform() == PlatformType.Android)
            {
                if(CheckClientFile(clientCrc))
                {
                    //文件已经下载完成。
                    //NativeInterface.DownNewClientVersion(GetClientPath());
                }
                else
                {
                    //下载资源文件
                    DownResData drd = new DownResData();
                    drd.ResType = ResType.MAX;
                    drd.ResUrl = clientUrl;
                    drd.ResCrc = clientCrc;
                    drd.ResSize = clientSize;
                    m_DownList.Add(drd);
                    m_State = UpdateState.UPDATE_DOWNLOAD_RES;
                    m_AllDownloadBytes += clientSize;
                }
                m_bNewClient = true;
            }
            else if (RuntimeInfo.GetPlatform() == PlatformType.Windows)
            {
                m_WinSizeList = new int[5];
                for (int i = 0; i < 5; ++i)
                {
                    string str = "winsize" + (i + 1).ToString();
                    if (clientNode.Attributes[str] != null)
                        m_WinSizeList[i] = int.Parse(clientNode.Attributes[str].Value);
                }
                DownResData drd = new DownResData();
                drd.ResType = ResType.MAX;
                drd.ResUrl = clientUrl;
                drd.ResCrc = clientCrc;
                drd.ResSize = clientSize;
                m_DownList.Add(drd);
                m_State = UpdateState.UPDATE_DOWNLOAD_RES;
                m_AllDownloadBytes += clientSize;
                m_bNewClient = true;
            }
            else
            {
                NativeInterface.DownNewClientVersion(clientUrl);
                m_State = UpdateState.UPDATE_NEW_CLIENT;
                return true;
            }
        }
        for (int i = 0; i < (int)ResManager.RES_NUM; ++i)
        {
            ResType rt = (ResType)i;
            XmlNode node = ele.SelectSingleNode(rt.ToString());
            DownResData vd = new DownResData();
            vd.ResCrc = uint.Parse(node.Attributes["crc"].Value);
            vd.ResUnzipSize = uint.Parse(node.Attributes["unzipsize"].Value);
            if (ResManager.Instance.VersionMgr.CheckVerAndCrc(rt, vd.ResCrc, vd.ResUnzipSize))
            {
                //下载资源文件
                vd.ResType = rt;
                vd.ResUrl = resftp + node.Attributes["url"].Value;
                vd.ResSize = uint.Parse(node.Attributes["size"].Value);
                m_DownList.Add(vd);
                m_AllDownloadBytes += vd.ResSize;
            }
        }

        //保存当前的RES_VERSION
        if (m_DownList.Count > 0)
        {
            m_State = UpdateState.UPDATE_DOWNLOAD_RES;
            return true;
        }
        else
        {
            return true;
        }
    }
    void SaveDownloadData(DownloadData dd)
    {
        MultiFileDownData mfdd = (MultiFileDownData)dd.Data;
        for (; mfdd.OutsideRecvIndex < mfdd.RecvCount; ++mfdd.OutsideRecvIndex)
        {
            MultiFileOK mfo = mfdd.CompletionList[mfdd.OutsideRecvIndex];
            m_ThreadList.AddItem(mfo);
            continue;
            if (mfo.Drd.ResType == ResType.MAX)
            {
                //新的客户端
                if (!SaveClient(mfo))
                {
                    LogMgr.Log("客户端保存失败.");
                    m_bError = true;
                }
            }
            else
            {
                if (!SaveRes(mfo))
                {
                    LogMgr.Log("资源保存失败:" + mfo.Drd.ResType);
                    m_bError = true;
                }
            }
        }
    }
    public void DownLoadThread()
    {
        while (m_bRun)
        {
            while (m_ThreadList.HasItem())
            {
                MultiFileOK mfo = m_ThreadList.GetItem();
                if (mfo.Drd.ResType == ResType.MAX)
                {
                    //新的客户端
                    if (!SaveClient(mfo))
                    {
                        LogMgr.Log("客户端保存失败.");
                        m_bError = true;
                        return;
                    }
                }
                else
                {
                    if (!SaveRes(mfo))
                    {
                        LogMgr.Log("资源保存失败:" + mfo.Drd.ResType);
                        m_bError = true;
                        return;
                    }
                }
                ++m_SaveCount;
            }
            System.Threading.Thread.Sleep(10);
        }
        
    }
}
