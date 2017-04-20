using UnityEngine;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;
class NetCmdCheckVer : NetCmd
{
    [TypeInfo(0)]
    public uint Version;
    [TypeInfo(1)]
    public byte Plateform;
    [TypeInfo(2)]
    public ushort ScreenWidth;
    [TypeInfo(3)]
    public ushort ScreenHeight;
    [TypeInfo(4, 48)]
    public byte[] MacAddress;
    [TypeInfo(5, 48)]
    public byte[] Package;
    [TypeInfo(6)]
    public ushort Length;
    [TypeInfo(7, true)]
    public byte[] FileName;
}
class NetCmdFileRequest : NetCmd
{
    [TypeInfo(0)]
    public ushort Length;
    [TypeInfo(1)]
    public ushort Count; //文件个数
    [TypeInfo(2, true)]
    public byte[] FileName;
};
public class MultiFileOK
{
    public DownResData Drd;
    public byte[] Data;
}
public class RecvFileDataFlag
{
    public byte[] FileData;
    public uint RecvSize;
}
public class CheckServerIP
{
    public CheckServerIP(byte idx)
    {
        ServerIdx = idx;
    }
    public uint UseCount;
    public uint OkCount;
    public uint FailedCount;
    public byte ServerIdx;
    public void BeginUse()
    {
        ++UseCount;
    }
    public void EndUse(bool bOK)
    {
        --UseCount;
        if (bOK) ++OkCount;
        else ++FailedCount;
    }
}
public enum ChunkState
{
    CHUNK_CONNECT,
    CHUNK_SEND_CMD,
    CHUNK_RECV_SIZE,
    CHUNK_RECV_DATA,
    CHUNK_COMPLETION,
}
public class DownChunkData
{
    public ChunkState State;
    public byte FileIdx;
    public uint ChunkIdx;
    public byte RetryCount;
    public int Offset;
    public int Length;
    public int RecvSize;
    public uint RecvTick;
    public byte XOR;
    public byte[] RecvData;
    public byte[] SendCmdData;
    public FTPConnectData Fcd;
    public CheckServerIP checkServer;
}
public class MultiFileDownData
{
    public List<DownResData> FileList;
    public List<ServerIPData> FTPIPList;
    public volatile MultiFileOK[] CompletionList;
    public RecvFileDataFlag[] RecvFileList;
    public int OutsideRecvIndex;
    public volatile int RecvCount;
    public int CurrentRecvSize;
    public object ExtraData;
}
public class FTPConnectData
{
    public Socket socket;
    public IPEndPoint ippoint;
    public volatile bool connected;
}

public class FTPClient : Singleton<FTPClient>
{
    public static byte USE_NEW_FTP = 1;
    public static byte MAX_CHUNK_COUNT = 10;    //同时下载的数量
    public static int CHUNK_SIZE = 1024 * 1024; //块的大小
    const int MAX_RETRY_COUNT = 2;
    const int MAX_XOR_SIZE = 1024 * 512;
    const byte XOR_MASK = 0xAA;
    static byte ISP_TYPE = 0;

    const uint CompletionData = 0xffffffff;
    byte[] CompletionMask = System.BitConverter.GetBytes(CompletionData);
    byte[] HearbeatMask = System.BitConverter.GetBytes(0xefffffff);
    const uint TIME_OUT = 3000;
    SafeArray<DownloadData> m_DownList = new SafeArray<DownloadData>(64);
    SafeArray<DownloadData> m_CompletionList = new SafeArray<DownloadData>(64);
    string m_PackageName;
    string m_MacAddress;

    //按照服务器的失败次数、使用情况、成功次数排序。
    static int SortCheckServer(CheckServerIP a, CheckServerIP b)
    {
        if (a.FailedCount + 3 < b.FailedCount)
            return -1;
        else if (a.FailedCount > b.FailedCount + 3)
            return 1;
        else if (a.UseCount < b.UseCount)
            return -1;
        else if (a.UseCount > b.UseCount)
            return 1;
        else if (a.OkCount > b.OkCount)
            return -1;
        else if (a.OkCount < b.OkCount)
            return 1;
        else
            return 0;
    }
    public void GlobalInit()
    {
        m_PackageName = SDKMgr.PACKAGE_NAME;
        m_MacAddress = NativeInterface.GetMac();
        Thread t = new Thread(new ThreadStart(_FtpThread));
        t.Start();

    }
    public void Test()
    {
        return;
        List<ServerIPData> ipList = new List<ServerIPData>();
        List<DownResData> resList = new List<DownResData>();
        ServerIPData sip = new ServerIPData();
        sip.IP = "127.0.0.1"; //"61.153.110.167";//"192.168.1.150";//
        sip.Port = 45045;
        sip.ISP = 0;
        ipList.Add(sip);
        for (int i = 1; i <= 6; ++i)
        {
            DownResData drd = new DownResData();
            drd.ResUrl = "ftp://127.0.0.1/FishPublish/rar/" + i + ".rar";
            resList.Add(drd);
        }

        AddMultiResDownload(DownloadType.DOWNLOAD_MULTI_FILE, ipList, resList);
        //AddDownload(DownloadType.DOWNLOAD_DEFAULT, "ftp://127.0.0.1/FishPublish/test.rar", null);
        //AddDownload(DownloadType.DOWNLOAD_DEFAULT, "ftp://101.64.233.10/FishPublish/test.rar", null);
        //AddDownload(DownloadType.DOWNLOAD_DEFAULT, "ftp://61.153.110.167/FishPublish/test.rar", null);
    }
    //资源有带前缀"ftp://"
    public DownloadData AddMultiResDownload(DownloadType dt, List<ServerIPData> serverList, List<DownResData> resList, object obj = null)
    {
        if (m_DownList.HasSpace() == false)
            return null;
        MultiFileDownData mfdd = new MultiFileDownData();
        mfdd.CompletionList = new MultiFileOK[resList.Count];
        mfdd.FTPIPList = new List<ServerIPData>();
        mfdd.FileList = new List<DownResData>();
        mfdd.OutsideRecvIndex = 0;
        mfdd.RecvCount = 0;
        mfdd.CurrentRecvSize = 0;
        mfdd.ExtraData = obj;
        mfdd.RecvFileList = new RecvFileDataFlag[resList.Count];
        List<ServerIPData> ipList1 = new List<ServerIPData>();
        List<ServerIPData> ipList2 = new List<ServerIPData>();
        for (int i = 0; i < serverList.Count; ++i)
        {
            if (serverList[i].ISP == ISP_TYPE || serverList[i].ISP == (byte)ISPType.ISP_YD || ISP_TYPE == (byte)ISPType.ISP_YD)
                ipList1.Add(serverList[i]);
            //else
            //  ipList2.Add(sip);
        }
        if(ipList1.Count == 0)
        {
            for (int i = 0; i < serverList.Count; ++i)
                ipList1.Add(serverList[i]);
        }
        for (int i = 0; i < resList.Count; ++i)
        {
            mfdd.RecvFileList[i] = new RecvFileDataFlag();
            DownResData drd = resList[i];
            string url = drd.ResUrl.ToLower();
            int idx = url.IndexOf("fishpublish");
            if (idx == -1)
                return null;
            drd.ResUrl = url.Substring(idx, url.Length - idx);
            mfdd.FileList.Add(drd);
        }
        //将FTP排队
        for (int i = 0; i < ipList1.Count; )
        {
            int idx = Utility.Range(0, ipList1.Count);
            if (idx >= ipList1.Count)
                idx = ipList1.Count - 1;
            mfdd.FTPIPList.Add(ipList1[idx]);
            ipList1.RemoveAt(idx);
        }
        for (int i = 0; i < ipList2.Count; )
        {
            int idx = Utility.Range(0, ipList2.Count);
            if (idx >= ipList2.Count)
                idx = ipList2.Count - 1;
            mfdd.FTPIPList.Add(ipList2[idx]);
            ipList2.RemoveAt(idx);
        }
        DownloadData dd = new DownloadData(null, mfdd, dt);
        dd.DownState = DownloadState.DS_DOWNLOADING;
        m_DownList.AddItem(dd);
        return dd;
    }
    public DownloadData AddDownload(DownloadType dt, string fileURL, object obj, string ip = null, ushort port = ushort.MaxValue)
    {
        //fileURL = "ftp://61.153.110.167/FishPublish/test/test_version/android_hd_version_com.leduo.buyu.test.xml";
        if (m_DownList.HasSpace() == false)
            return null;
        try
        {
            if (ip == null)
            {
                //兼容旧格式，要从fileURL中解析网址
                int idx = fileURL.IndexOf("ftp://");
                if (idx == -1)
                    return null;
                idx += 6;
                int idx2 = fileURL.IndexOf("/", idx);
                if (idx2 == -1)
                    return null;
                ip = fileURL.Substring(idx, idx2 - idx);
                port = ServerSetting.ResFtpPort;
                fileURL = fileURL.Substring(idx2 + 1);
                //fileURL = "fishpublish/test/test_version/windows_hd_version.xml";
            }
            List<ServerIPData> serverList = new List<ServerIPData>();
            List<DownResData> resList = new List<DownResData>();
            ServerIPData sid = new ServerIPData();
            sid.IP = ip;
            sid.Port = port;
            serverList.Add(sid);
            DownResData drd = new DownResData();
            drd.ResUrl = fileURL;
            drd.ResSize = 0;
            resList.Add(drd);
            return AddMultiResDownload(dt, serverList, resList, obj);
        }
        catch (System.Exception e)
        {
            ReportException.Instance.AddException("AddDownload Exception:" + e.ToString());
            return null;
        }
    }
    void SetPcakgeMac(NetCmdCheckVer cmd, string package, string macaddr)
    {
        byte[] parr = System.Text.Encoding.ASCII.GetBytes(package);
        byte[] pmac = System.Text.Encoding.ASCII.GetBytes(macaddr);

        cmd.Package = new byte[48];
        cmd.MacAddress = new byte[48];

        int minarr = Mathf.Min(parr.Length, cmd.Package.Length);
        int idx = 0;
        for (; idx < minarr; ++idx)
            cmd.Package[idx] = parr[idx];

        minarr = Mathf.Min(pmac.Length, cmd.MacAddress.Length);
        idx = 0;
        for (; idx < minarr; ++idx)
            cmd.MacAddress[idx] = pmac[idx];
    }
    public void Update(float delta)
    {
        while (m_CompletionList.HasItem())
        {
            DownloadData dd = m_CompletionList.GetItem();
            if (dd.Type != DownloadType.DOWNLOAD_DEFAULT)
            {
                DownloadHandler.Handle(dd);
            }
        }
    }
    void ThreadConnect(object obj)
    {
        FTPConnectData fcd = (FTPConnectData)obj;
        try
        {
            fcd.socket.Connect(fcd.ippoint);
            fcd.connected = fcd.socket.Connected;
        }
        catch
        {

        }
    }
    void DownMultiFile_OldFTP(DownloadData dd)
    {
        byte[] buffer = new byte[65536];
        MultiFileDownData mfdd = (MultiFileDownData)dd.Data;
        for (int i = 0; i < mfdd.FTPIPList.Count; ++i)
        {
            ServerIPData sid = mfdd.FTPIPList[i];
            if (sid.ISP == (int)ISPType.ISP_DX)
                LogMgr.Log("<电信FTP:" + sid.IP + ">");
            else if (sid.ISP == (int)ISPType.ISP_LT)
                LogMgr.Log("<联通FTP:" + sid.IP + ">");
            else
                LogMgr.Log("<移动FTP:" + sid.IP + ">");
            int retryCount = 0;
            while (mfdd.RecvCount < mfdd.FileList.Count)
            {
                DownResData drd = mfdd.FileList[mfdd.RecvCount];
                string url = "ftp://" + sid.IP + "/" + drd.ResUrl;
                int readSize = 0;
                try
                {
                    FtpWebRequest ftpRequest = (FtpWebRequest)FtpWebRequest.Create(url);
                    if (ftpRequest == null)
                    {
                        if (retryCount >= MAX_RETRY_COUNT)
                        {
                            break;
                        }
                        else
                        {
                            ++retryCount;
                            Thread.Sleep(500);
                            continue;
                        }
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
                            break;
                        }
                        else
                        {
                            ++retryCount;
                            Thread.Sleep(500);
                            continue;
                        }
                    }
                    MemoryStream ms = new MemoryStream();
                    while (true)
                    {
                        int s = ftpStream.Read(buffer, 0, buffer.Length);
                        if (s <= 0)
                            break;
                        ms.Write(buffer, 0, s);
                        readSize += s;
                        mfdd.CurrentRecvSize += s;
                        dd.DownloadBytes += (uint)s;
                    }
                    try
                    {
                        ftpStream.Close();
                        response.Close();
                    }
                    catch
                    {

                    }
                    if (mfdd.CurrentRecvSize == drd.ResSize)
                    {
                        MultiFileOK mfo = new MultiFileOK();
                        mfo.Data = ms.ToArray();
                        mfo.Drd = mfdd.FileList[mfdd.RecvCount];
                        mfdd.CompletionList[mfdd.RecvCount] = mfo;
                        ++mfdd.RecvCount;
                        if (mfdd.RecvCount == mfdd.FileList.Count)
                        {
                            dd.DownState = DownloadState.DS_DOWNLOADED_OK;
                            if (dd.Type != DownloadType.DOWNLOAD_DEFAULT)
                                m_CompletionList.AddItem(dd);
                            return;
                        }
                        else
                        {
                            mfdd.CurrentRecvSize = 0;
                        }
                    }
                }
                catch (System.Exception e)
                {
                    dd.DownloadBytes -= (uint)readSize;
                    LogMgr.Log("OLD_FTP下载失败:" + e.ToString());
                    if (retryCount >= MAX_RETRY_COUNT)
                    {
                        break;
                    }
                    else
                    {
                        ++retryCount;
                        Thread.Sleep(500);
                        continue;
                    }
                }
            }
        }
        dd.DownState = DownloadState.DS_DOWNLOADED_ERROR;
        if (dd.Type != DownloadType.DOWNLOAD_DEFAULT)
            m_CompletionList.AddItem(dd);
    }

    FTPConnectData CreateConnectData(ServerIPData sid)
    {
        FTPConnectData fcd = new FTPConnectData();
        Socket s = TCPClient.CreateSocket(true, true);
        s.ReceiveBufferSize = 65536 * 2;
        s.ReceiveTimeout = 10000000;
        fcd.connected = false;
        fcd.socket = s;
        fcd.ippoint = new IPEndPoint(IPAddress.Parse(sid.IP), sid.Port);
        Thread th = new Thread(new ParameterizedThreadStart(ThreadConnect));
        th.Start(fcd);
        return fcd;
    }
    bool CreateChunkCmdData(DownloadData dd, DownChunkData dcd)
    {
        MultiFileDownData mfdd = (MultiFileDownData)dd.Data;
        DownResData curDrd = mfdd.FileList[dcd.FileIdx];
        try
        {
            //1.封装命令
            if (dd.Type == DownloadType.DOWNLOAD_CHECK_VER)
            {
                NetCmdCheckVer cmd = new NetCmdCheckVer();
                cmd.SetCmdType(NetCmdType.CMD_EXTRA_CHECK_VER);
                cmd.Version = ServerSetting.ClientVer;
                cmd.Plateform = (byte)RuntimeInfo.GetPlatform();
                cmd.ScreenWidth = (ushort)Resolution.GetScreenWidth();
                cmd.ScreenHeight = (ushort)Resolution.GetScreenHeight();
                cmd.FileName = System.Text.Encoding.ASCII.GetBytes(curDrd.ResUrl);
                cmd.Length = (ushort)mfdd.FileList[0].ResUrl.Length;
                SetPcakgeMac(cmd, m_PackageName, m_MacAddress);
                dcd.SendCmdData = NetCmdHelper.CmdToBytes<NetCmdCheckVer>(cmd, 0);
            }
            else
            {
                byte[] data1 = System.BitConverter.GetBytes(dcd.Offset);
                byte[] data2 = System.BitConverter.GetBytes(dcd.Length);
                byte[] dataUrl = System.Text.Encoding.ASCII.GetBytes(curDrd.ResUrl);
                byte[] sendData = new byte[data1.Length + data2.Length + dataUrl.Length + 1];
                System.Array.Copy(data1, 0, sendData, 0, 4);
                System.Array.Copy(data2, 0, sendData, 4, 4);
                System.Array.Copy(dataUrl, 0, sendData, 8, dataUrl.Length);

                NetCmdFileRequest ncf = new NetCmdFileRequest();
                ncf.FileName = sendData;
                ncf.Count = (ushort)(mfdd.FileList.Count | ((int)dcd.XOR << 15)); //高位是1表示要异或
                ncf.Length = (ushort)sendData.Length;
                ncf.SetCmdType(NetCmdType.CMD_EXTRA_FILE_EX);
                dcd.SendCmdData = NetCmdHelper.CmdToBytes<NetCmdFileRequest>(ncf, 0);
            }
        }
        catch (System.Exception e)
        {
            throw new System.Exception("CreateChunkCmdData Err:" + e.ToString());
            return false;
        }
        return true;
    }
    void CopyXorData(byte[] srcdata, int srcoffset, byte[] destdata, int size, bool bxor)
    {
        if (bxor)
        {
            for (int i = 0; i < size; ++i)
                destdata[i] = (byte)(srcdata[srcoffset + i] ^ XOR_MASK);
        }
        else
        {
            for (int i = 0; i < size; ++i)
                destdata[i] = srcdata[srcoffset + i];
        }
    }
    bool DownChunk(DownloadData dd, DownChunkData dcd, uint tick)
    {
        int ret = 0;
        switch (dcd.State)
        {
            case ChunkState.CHUNK_CONNECT:
                if (dcd.Fcd.connected)
                {
                    dcd.Fcd.socket.Blocking = false;
                    dcd.State = ChunkState.CHUNK_SEND_CMD;
                    dcd.RecvTick = tick;
                    return true;
                }
                return tick - dcd.RecvTick < 3000;
                break;
            case ChunkState.CHUNK_SEND_CMD:
                try
                {
                    ret = dcd.Fcd.socket.Send(dcd.SendCmdData, SocketFlags.None);
                    if (ret == dcd.SendCmdData.Length)
                    {
                        dcd.State = ChunkState.CHUNK_RECV_SIZE;
                        dcd.SendCmdData = null;
                        dcd.RecvTick = tick;

                        return true;
                    }
                }
                catch
                {
                    if (dcd.Fcd.socket.Connected == false)
                        return false;
                }
                return tick - dcd.RecvTick < TIME_OUT;
                break;
            case ChunkState.CHUNK_RECV_SIZE:
                try
                {
                    ret = dcd.Fcd.socket.Receive(dcd.RecvData, dcd.RecvSize, dcd.RecvData.Length - dcd.RecvSize, SocketFlags.None);
                }
                catch
                {
                    if (dcd.Fcd.socket.Connected == false)
                        return false;
                }
                if (ret <= 0)
                    return tick - dcd.RecvTick < TIME_OUT;
                dcd.RecvTick = tick;
                dcd.RecvSize += ret;
                int retIdx = 0;
                while (dcd.RecvSize >= 4)
                {
                    uint recvID = System.BitConverter.ToUInt32(dcd.RecvData, retIdx);
                    retIdx += 4;
                    dcd.RecvSize -= 4;
                    if (recvID == 0xEFFFFFFF)
                    {
                        //正在等待打开文件中
                    }
                    else if ((recvID & 0xF0000000) == 0xF0000000)
                    {
                        //文件大小
                        recvID &= 0x0fffffff;
                        if (dcd.Length == 0)
                        {
                            dcd.Length = (int)recvID;
                        }
                        else if (dcd.Length != recvID)
                        {
                            throw new System.Exception("Recv FileSize Error:ServerSize:" + recvID + ", LocalSize:" + dcd.Length);
                            return false;
                        }
                        if (dcd.RecvData.Length < dcd.Length)
                        {
                            byte[] recvdata = new byte[dcd.Length];
                            CopyXorData(dcd.RecvData, retIdx, recvdata, dcd.RecvSize, dcd.XOR == 1);
                            retIdx = 0;
                            dcd.RecvData = recvdata;
                        }
                        //接收完成
                        dd.Queue = 0xffffffff;
                        dcd.State = ChunkState.CHUNK_RECV_DATA;
                        dcd.RecvTick = tick;
                        dd.DownloadBytes += (uint)dcd.RecvSize;
                        break;
                    }
                    else if ((recvID & 0xC0000000) == 0xC0000000)
                    {
                        //文件个数
                        byte isp = (byte)((recvID >> 28) & 0x3);
                        if (dd.Type == DownloadType.DOWNLOAD_CHECK_VER)
                        {
                            ISP_TYPE = isp;
                           // LogMgr.Log("<ISP:" + ((ISPType)ISP_TYPE) + ">");
                        }
                        recvID &= 0xff;
                        if (recvID != 1)
                        {
                            throw new System.Exception("File Count Error,ServerCount:" + recvID + ", LocalCount:1");
                            return false;
                        }
                    }
                    else if ((recvID & 0x80000000) != 0)
                    {
                        //排队中
                        dd.Queue = (recvID) & 0x7fffffff;
                    }
                    else
                    {
                        throw new System.Exception("未知的RecvSize数据:" + recvID);
                        return false;
                    }
                }
                //移动数据到最开始
                if (retIdx != 0)
                {
                    CopyXorData(dcd.RecvData, retIdx, dcd.RecvData, dcd.RecvSize, (dcd.XOR == 1 && dcd.State == ChunkState.CHUNK_RECV_DATA));
                }
                if (dcd.State == ChunkState.CHUNK_RECV_DATA && dcd.RecvSize == dcd.Length)
                    dcd.State = ChunkState.CHUNK_COMPLETION;
                return true;
                break;
            case ChunkState.CHUNK_RECV_DATA:
                try
                {
                    ret = dcd.Fcd.socket.Receive(dcd.RecvData, dcd.RecvSize, dcd.Length - dcd.RecvSize, SocketFlags.None);
                }
                catch (SocketException e)
                {
                    if (dcd.Fcd.socket.Connected == false)
                    {
                        return false;
                    }
                }
                if (ret <= 0)
                {
                    if (tick - dcd.RecvTick < TIME_OUT)
                        return true;
                    else
                    {
                        return false;
                    }
                }
                if (dcd.XOR == 1)
                {
                    int endsize = dcd.RecvSize + ret;
                    for (int i = dcd.RecvSize; i < endsize; ++i)
                        dcd.RecvData[i] ^= XOR_MASK;
                }
                dcd.RecvTick = tick;
                dcd.RecvSize += ret;
                dd.DownloadBytes += (uint)ret;
                if (dcd.RecvSize == dcd.Length)
                    dcd.State = ChunkState.CHUNK_COMPLETION;
                return true;
            default:
                throw new System.Exception("未知的Recv状态:" + dcd.State);
                return false;
        }
        return false;
    }

    void CopyBuffToData(MultiFileDownData mfdd, DownChunkData chunk)
    {
        RecvFileDataFlag cfd = mfdd.RecvFileList[chunk.FileIdx];
        if (cfd.FileData == null)
            cfd.FileData = new byte[mfdd.FileList[chunk.FileIdx].ResSize];
        System.Array.Copy(chunk.RecvData, 0, cfd.FileData, chunk.Offset, chunk.RecvSize);
        cfd.RecvSize += (uint)chunk.RecvSize;
    }
    void CloseSocket(DownChunkData chunk)
    {
        try
        {
            chunk.Fcd.socket.Close();
        }
        catch
        {
        }
        chunk.Fcd = null;
    }
    bool InitChunk(DownloadData dd, DownChunkData dcd, ServerIPData sid)
    {
        if (dcd.SendCmdData == null)
        {
            if (!CreateChunkCmdData(dd, dcd))
            {
                Debug.Log("创建FTP命令失败1!");
                return false;
            }
        }
        dcd.Fcd = CreateConnectData(sid);
        dcd.State = ChunkState.CHUNK_CONNECT;
        if (dcd.RecvData == null || dcd.RecvData.Length < dcd.Length)
            dcd.RecvData = new byte[dcd.Length];
        return true;
    }
    public void _FtpThread()
    {
        while (true)
        {
            if (m_DownList.HasItem() == false)
            {
                Thread.Sleep(100);
                continue;
            }

            DownloadData dd = m_DownList.GetItem();
            if (USE_NEW_FTP == 0)
            {
                DownMultiFile_OldFTP(dd);
            }
            else
            {
                bool bRet = false;
                try
                {
                    uint recvTick = Utility.GetTickCount();
                    if (dd.Type == DownloadType.DOWNLOAD_MULTI_FILE)
                        bRet = DownMultiFiles(dd);
                    else
                        bRet = DownSingleFiles(dd);
                    recvTick = Utility.GetTickCount() - recvTick;
                }
                catch (System.Exception e)
                {
                    ReportException.Instance.AddException("FTPDown Err:" + e.ToString());
                }
                dd.DownState = bRet ? DownloadState.DS_DOWNLOADED_OK : DownloadState.DS_DOWNLOADED_ERROR;
                if (dd.Type != DownloadType.DOWNLOAD_DEFAULT)
                    m_CompletionList.AddItem(dd);
            }
            
            Thread.Sleep(100);
        }
    }
    bool DownSingleFiles(DownloadData dd)
    {
        MultiFileDownData mfdd = (MultiFileDownData)dd.Data;
        DownChunkData chunk = new DownChunkData();
        chunk.FileIdx = 0;
        chunk.Length = 0;
        chunk.Offset = 0;
        chunk.RecvSize = 0;
        chunk.RecvData = new byte[128];
        chunk.State = ChunkState.CHUNK_CONNECT;
        if (!InitChunk(dd, chunk, mfdd.FTPIPList[0]))
            return false;
        chunk.RecvTick = Utility.GetTickCount();
        while (true)
        {
            uint tick = Utility.GetTickCount();
            if (!DownChunk(dd, chunk, tick))
            {
                dd.Data = mfdd.ExtraData;
                CloseSocket(chunk);
                return false;
            }
            else if (chunk.State == ChunkState.CHUNK_COMPLETION)
            {
                dd.Data = mfdd.ExtraData;
                CloseSocket(chunk);
                if (chunk.RecvData.Length != chunk.Length)
                {
                    dd.m_Bytes = new byte[chunk.Length];
                    System.Array.Copy(chunk.RecvData, dd.m_Bytes, chunk.Length);
                }
                else
                {
                    dd.m_Bytes = chunk.RecvData;
                }
                return true;
            }
            Thread.Sleep(1);
        }
    }
    void SplitChunk(ref uint idx, DownChunkData drd, out DownChunkData dcd1, out DownChunkData dcd2)
    {
        int halfSize = drd.Length / 2;
        dcd1 = new DownChunkData();
        dcd2 = new DownChunkData();
        dcd1.Offset = drd.Offset;
        dcd1.RecvSize = 0;
        dcd1.RecvTick = 0;
        dcd1.Length = halfSize;
        dcd1.ChunkIdx = idx++;
        dcd1.RetryCount = drd.RetryCount;
        dcd1.XOR = drd.XOR;
        dcd1.FileIdx = drd.FileIdx;

        dcd2.Offset = dcd1.Offset + dcd1.Length;
        dcd2.RecvSize = 0;
        dcd2.RecvTick = 0;
        dcd2.Length = halfSize;
        dcd2.ChunkIdx = idx++;
        dcd2.RetryCount = drd.RetryCount;
        dcd1.XOR = drd.XOR;
        dcd2.FileIdx = drd.FileIdx;
    }
    bool DownMultiFiles(DownloadData dd)
    {
        MultiFileDownData mfdd = (MultiFileDownData)dd.Data;
        List<DownChunkData> chunkList = new List<DownChunkData>();
        List<DownChunkData> waitList = new List<DownChunkData>();
        List<DownChunkData> retryList = new List<DownChunkData>();
        List<CheckServerIP> serverList = new List<CheckServerIP>();
        bool bHasNoYD = false;
        for (byte i = 0; i < mfdd.FTPIPList.Count; ++i)
        {
            CheckServerIP ssi = new CheckServerIP(i);
            if (mfdd.FTPIPList[i].ISP == (byte)ISPType.ISP_YD)
                ssi.FailedCount = (uint)(MAX_CHUNK_COUNT * 2);
            serverList.Add(ssi);
            if (mfdd.FTPIPList[i].ISP != (byte)ISPType.ISP_YD)
                bHasNoYD = true;
        }
        //1.划分Chunk
        //================================
        uint tick = Utility.GetTickCount();
        byte ipidx = 0;
        int CHUNK_SIZE2 = CHUNK_SIZE << 1;
        uint chunkIdx = 0;
        for (byte i = 0; i < mfdd.FileList.Count; ++i)
        {
            int resSize = (int)mfdd.FileList[i].ResSize;
            int resOffset = 0;
            while (resSize > 0)
            {
                DownChunkData dcd = new DownChunkData();
                dcd.FileIdx = i;
                if (resSize >= CHUNK_SIZE2)
                    dcd.Length = CHUNK_SIZE;
                else
                    dcd.Length = resSize;
                dcd.ChunkIdx = chunkIdx++;
                dcd.Offset = resOffset;
                dcd.RecvSize = 0;
                dcd.RecvTick = 0;
                resOffset += dcd.Length;
                resSize -= dcd.Length;
                if (chunkList.Count < MAX_CHUNK_COUNT)
                {
                    do
                    {
                        ipidx = (byte)((ipidx + 1) % serverList.Count);
                    } while (bHasNoYD && mfdd.FTPIPList[ipidx].ISP == (byte)ISPType.ISP_YD);
                    dcd.checkServer = serverList[ipidx];
                    dcd.checkServer.BeginUse();
                    if (!InitChunk(dd, dcd, mfdd.FTPIPList[ipidx]))
                        return false;
                    dcd.RecvTick = tick;
                    chunkList.Add(dcd);
                }
                else
                {
                    waitList.Add(dcd);
                }
            }
        }
        //1.接收Chunk
        //================================
        while (true)
        {
            tick = Utility.GetTickCount();
            for (int i = 0; i < chunkList.Count; )
            {
                DownChunkData chunk = chunkList[i];
                if (!DownChunk(dd, chunk, tick))
                {
                    CloseSocket(chunk);
                    chunk.checkServer.EndUse(chunk.State != ChunkState.CHUNK_CONNECT && chunk.RecvSize != 0);
                    if (chunk.State == ChunkState.CHUNK_RECV_DATA)
                    {
                        //将接收的数据coy到data
                        CopyBuffToData(mfdd, chunk);
                        chunk.Offset += chunk.RecvSize;
                        chunk.Length -= chunk.RecvSize;
                        chunk.RecvSize = 0;
                        ++chunk.RetryCount;
                        chunk.SendCmdData = null;
                    }
                    if (chunk.RetryCount >= MAX_RETRY_COUNT)
                    {
                        //split
                        if ((chunk.Length > MAX_XOR_SIZE || chunk.XOR == 1) && chunk.Length > 16)
                        {
                            DownChunkData dcd1, dcd2;
                            SplitChunk(ref chunkIdx, chunk, out dcd1, out dcd2);
                            waitList.Add(dcd1);
                            waitList.Add(dcd2);
                        }
                        else
                        {
                            chunk.RetryCount = 0;
                            chunk.XOR = 1;
                            chunk.SendCmdData = null;
                            retryList.Add(chunk);
                        }
                    }
                    else
                    {
                        retryList.Add(chunk);
                    }
                    Utility.ListRemoveAt(chunkList, i);
                    continue;
                }
                else if (chunk.State == ChunkState.CHUNK_COMPLETION)
                {
                    CopyBuffToData(mfdd, chunk);
                    CloseSocket(chunk);
                    chunk.checkServer.EndUse(true);
                    RecvFileDataFlag cfd = mfdd.RecvFileList[chunk.FileIdx];
                    if (cfd.RecvSize == cfd.FileData.Length)
                    {
                        //完成
                        MultiFileOK mfo = new MultiFileOK();
                        mfo.Data = cfd.FileData;
                        mfo.Drd = mfdd.FileList[chunk.FileIdx];
                        mfdd.CompletionList[mfdd.RecvCount] = mfo;
                        if (++mfdd.RecvCount == mfdd.FileList.Count)
                        {
                            Debug.Log("所有文件完成!");
                            return true;
                        }
                    }
                    Utility.ListRemoveAt(chunkList, i);
                    continue;
                }
                ++i;
            }//end for

            if (chunkList.Count < MAX_CHUNK_COUNT)
            {
                tick = Utility.GetTickCount();
                DownChunkData dcd = null;
                if (waitList.Count > 0)
                {
                    dcd = waitList[0];
                    waitList.RemoveAt(0);
                }
                else if (retryList.Count > 0)
                {
                    dcd = retryList[0];
                    retryList.RemoveAt(0);
                }
                if (dcd != null)
                {
                    serverList.Sort(SortCheckServer);
                    serverList[0].BeginUse();
                    dcd.checkServer = serverList[0];
                    if (!InitChunk(dd, dcd, mfdd.FTPIPList[dcd.checkServer.ServerIdx]))
                        return false;
                    dcd.RecvTick = Utility.GetTickCount();
                    chunkList.Add(dcd);
                }
            }
            if (mfdd.RecvCount == mfdd.FileList.Count)
                break;
            Thread.Sleep(1);
        };//end while
        return true;
    }
}
