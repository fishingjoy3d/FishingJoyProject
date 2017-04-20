using UnityEngine;
using System.Collections.Generic;
using System.Threading;
using System.Net;
using System.IO;
public enum DownloadType
{
    DOWNLOAD_DEFAULT,
    DOWNLOAD_RANK_INFO,
    DOWNLOAD_Month_Rank,
    DOWNLOAD_CHECK_VER,
    DOWNLOAD_MULTI_FILE,
    DOWNLOAD_ANNOUNCEMENT,
}
public enum DownloadState
{
    DS_DOWNLOADING,
    DS_DOWNLOADED_OK,
    DS_DOWNLOADED_ERROR,
}
public class DownloadData
{
    public DownloadData(string url, object obj, DownloadType dt = DownloadType.DOWNLOAD_DEFAULT)
    {
        m_DownType  = dt;
        m_ObjData   = obj;
        m_Url       = url;
        Queue       = 0xffffffff;
    }
    public      string          IP;
    public      ushort          Port;
    public      volatile uint   Queue;
    volatile    DownloadType    m_DownType;
    volatile    byte            m_State;
    volatile    object          m_ObjData;
    volatile    string          m_Url;
    volatile    public byte[]   m_Bytes;
    volatile    uint            m_DownloadBytes;
    public void SetBytes(byte[] data)
    {
        m_Bytes = data;
        m_DownloadBytes = (uint)data.Length;
    }
    public bool IsQueue
    {
        get
        {
            return Queue != 0xffffffff;
        }
    }
    public string URL
    {
        get
        {
            return m_Url;
        }
    }
    public DownloadState DownState
    {
        get
        {
            return (DownloadState)m_State;
        }
        set
        {
            m_State = (byte)value;
        }
    }
    public DownloadType Type
    {
        get
        {
            return m_DownType;
        }
    }
    public object Data
    {
        get
        {
            return m_ObjData;
        }
        set
        {
            m_ObjData = value;
        }
    }
    public uint DownloadBytes
    {
        get
        {
            return m_DownloadBytes;
        }
        set
        {
            m_DownloadBytes = value;
        }
    }
    public string Text
    {
        get
        {
            return System.Text.Encoding.UTF8.GetString(m_Bytes);
        }
    }
    public byte[] Bytes
    {
        get
        {
            return m_Bytes;
        }
    }
    public bool IsCompletion
    {
        get
        {
            return (DownState == DownloadState.DS_DOWNLOADED_OK) ||
            (DownState == DownloadState.DS_DOWNLOADED_ERROR);
        }
    }
    public bool IsOK
    {
        get
        {
            return (DownState == DownloadState.DS_DOWNLOADED_OK);
        }
    }
    public bool HasError
    {
        get
        {
            return (DownState == DownloadState.DS_DOWNLOADED_ERROR);
        }
    }
}

//不再使用此类
/*
class HttpDownload:Singleton<HttpDownload>
{
    public SafeList<DownloadData> m_DownList = new SafeList<DownloadData>();
    public SafeList<DownloadData> m_CompletionList = new SafeList<DownloadData>();
    public void GlobalInit()
    {
        Thread t = new Thread(new ThreadStart(_DownThread));
        t.Start();
    }
    public void Update(float delta)
    {
        while(m_CompletionList.HasItem())
        {
            DownloadData dd = m_CompletionList.GetItem();
            if (dd.Type != DownloadType.DOWNLOAD_DEFAULT)
                DownloadHandler.Handle(dd);
        }
    }
    public void _DownThread()
    {
        const int MAX_RETRY_COUNT = 5;
        DownloadData curDown = null;
        byte[] buffer = new byte[2048];
        uint retryCount = 0;
        while(true)
        {
            if (curDown == null && m_DownList.HasItem() == false)
            {
                Thread.Sleep(100);
                continue;
            }
            while (true)
            {
                if (curDown == null)
                {
                    curDown = m_DownList.GetItem();
                    retryCount = 0;
                }
                else
                {
                    ++retryCount;
                }
                try
                {
                    FtpWebRequest ftpRequest = (FtpWebRequest)FtpWebRequest.Create(curDown.URL);
                    if (ftpRequest == null)
                    {
                        if (retryCount >= MAX_RETRY_COUNT)
                        {
                            curDown.DownState = DownloadState.DS_DOWNLOADED_ERROR;
                            m_CompletionList.AddItem(curDown);
                            ReportException.Instance.AddException("HttpWebRequest RetryCount:" + retryCount + ", Create Failed! URL:" + curDown.URL);
                            curDown = null;
                        }
                        break;
                    }
                    ftpRequest.Method = WebRequestMethods.Ftp.DownloadFile;
                    ftpRequest.UseBinary = true;
                    ftpRequest.KeepAlive = false;
                    FtpWebResponse response = (FtpWebResponse)ftpRequest.GetResponse();
                    Stream ftpStream = null;

                    if (response == null || (ftpStream = response.GetResponseStream()) == null)
                    {
                        if (retryCount >= MAX_RETRY_COUNT)
                        {
                            curDown.DownState = DownloadState.DS_DOWNLOADED_ERROR;
                            m_CompletionList.AddItem(curDown);
                            //ReportException.Instance.AddException("HttpWebRequest RetryCount:" + retryCount + ", GetResponse Failed! URL:" + curDown.URL);
                            curDown = null;
                        }
                        break;
                    }
                    MemoryStream ms = new MemoryStream();
                    int readSize = 0;
                    while (true)
                    {
                        int s = ftpStream.Read(buffer, 0, buffer.Length);
                        if (s <= 0)
                            break;
                        ms.Write(buffer, 0, s);
                        readSize += s;
                        curDown.DownloadBytes = (uint)readSize;
                    }

                    byte[] data = ms.ToArray();
                    curDown.SetBytes(data);
                    curDown.DownState = DownloadState.DS_DOWNLOADED_OK;
                    m_CompletionList.AddItem(curDown);
                    curDown = null;

                    ftpStream.Close();
                    response.Close();
                }
                catch (System.Exception e)
                {
                    //if (curDown != null)
                    //    ReportException.Instance.AddException("HttpWebRequest RetryCount:" + retryCount + ", Exception:" + e.ToString() + ", URL:" + curDown.URL);
                    //else
                    //    ReportException.Instance.AddException("HttpWebRequest RetryCount:" + retryCount + ", Exception:" + e.ToString() + ", URL:null");
                    if (retryCount >= MAX_RETRY_COUNT)
                    {
                        curDown.DownState = DownloadState.DS_DOWNLOADED_ERROR;
                        m_CompletionList.AddItem(curDown);
                        curDown = null;
                    }
                }
                break;
            }//end while
            Thread.Sleep(100);
        }
    }
    public DownloadData NewDownload(DownloadType type,  string url, object data)
    {
        DownloadData dd = new DownloadData(url, data, type);
        m_DownList.AddItem(dd);
        return dd;
    }
}*/
