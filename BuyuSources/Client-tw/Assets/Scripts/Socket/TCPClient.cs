using UnityEngine;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Collections;
using System.IO;
using System.Threading;
public interface INetHandler
{
    bool Handle(byte[] data, ushort offset, ushort length);
    void StateChanged(NetState state);
}
public enum ConnectResult
{
    CONNECT_OK,
    CONNECT_ERROR,
    CONNECT_VERSION,
}
public struct ConnectResultData
{
    public uint Ver;
    public uint PathCrc;
    public ConnectResult Result;
}
struct ClientSetting
{
    public const uint CONNECT_RESULT = 0x1343316f;
    public const int CONNECT_TIMEOUT = 5000;
    public const int TIMEOUT = 5000;
    public const int SOCKET_RECV_BUFF_SIZE = 1024 * 8;
    public const int SOCKET_SEND_BUFF_SIZE = 1024 * 8;
    public const int RECV_BUFF_SIZE = 1024 * 8;
    public const int CMD_MIN_SIZE = 4;
    public const int SLEEP_TIME = 1;
    public const int UDP_RESEND_TICK = 35;
}
public interface INetClient
{
    bool Connect(string ip, int port, uint newip, ushort newport, bool bThread = true);
    void Disconnect();
    bool IsConnected { get; }
    void Update();
    void Send<T>(NetCmdBase dd);
}
public enum NetState
{
    NET_DISCONNECT,
    NET_CONNECTED,
    NET_ERROR,
}
public enum ConnectState
{
    CONNECT_WAIT,
    CONNECT_ERROR,
    CONNECT_OK,
}
public class ConnectData
{
    public Socket ConnectSocket;
    public IPEndPoint ConnectIP;
    public volatile ConnectState ConnectState;
}
public class RecvBuff
{
    public byte[] Buff = new byte[2048];
    public ushort Offset;
    public ushort RecvLength;
}


public class TCPClient:INetClient
{
    //---------------------------------------
    volatile bool m_bConnected = false;
    volatile bool m_bNotify = false;
    uint    m_SendTick = 0;
    uint    m_RecvTick = 0;
    Socket m_Socket;
    INetHandler m_Handler;
    SafeArray<SendCmdPack> m_SendList = new SafeArray<SendCmdPack>(128);
    byte[]  m_Buff;
    int     m_Offset;
    int     m_RecvSize;
    //---------------------------------------
    public static Socket CreateSocket(bool bTCP, bool blocking)
    {
        Socket s;
        if (bTCP)
        {
            s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            s.ExclusiveAddressUse = false;
            s.LingerState = new LingerOption(true, 1);
            s.NoDelay = true;
            s.SetSocketOption(SocketOptionLevel.Tcp, SocketOptionName.NoDelay, 1);
        }
        else
            s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        s.ExclusiveAddressUse = false;
        s.ReceiveBufferSize = ClientSetting.SOCKET_RECV_BUFF_SIZE;
        s.SendBufferSize = ClientSetting.SOCKET_SEND_BUFF_SIZE;
        s.ReceiveTimeout = ClientSetting.CONNECT_TIMEOUT;
        s.SendTimeout = ClientSetting.TIMEOUT;
        s.Blocking = blocking;
        return s;
    }
    public TCPClient(INetHandler cmd)
    {
        m_Handler = cmd;
    }
    uint GetTickCount()
    {
        return Utility.GetTickCount();
    }
    public bool IsConnected
    {
        get
        {
            return m_Socket != null && m_Socket.Connected;
        }
    }
    public void Disconnect()
    {
        if (m_Socket != null && m_Socket.Connected)
        {
            CloseSocket();
            m_Handler.StateChanged(NetState.NET_DISCONNECT);
        }
    }
    void ThreadConnect(object obj)
    {
        try
        {
            m_Socket.Connect((EndPoint)obj);
            m_bConnected = m_Socket.Connected;
        }
        catch
        {
            UnityEngine.Debug.Log("failed to connect[" + ((EndPoint)obj).ToString() + "]");
        }
    }
    public bool Connect(string ip, int port, uint newip, ushort newport, bool bThread = true)
    {
        UnityEngine.Debug.Log("connect ip[" + ip + "] port [" + port + "]");
        const uint CONNECT_TIME_OUT = 3000;
        try
        {
            m_bConnected = false;
            CloseSocket();
            m_Socket = CreateSocket(true, true);
            IPEndPoint endpoit = new IPEndPoint(IPAddress.Parse(ip), port);
            Thread th = new Thread(new ParameterizedThreadStart(ThreadConnect));
            th.Start(endpoit);
            uint tick = GetTickCount();
            while (GetTickCount() - tick < CONNECT_TIME_OUT && m_bConnected == false)
            {
                Thread.Sleep(50);
            }
            if (m_Socket.Connected == false)
            {
                CloseSocket();
                return false;
            }
            m_Socket.Blocking = false;
            m_Buff = new byte[ClientSetting.RECV_BUFF_SIZE];
            m_Offset = 0;
            m_RecvSize = 0;
            m_RecvTick = GetTickCount();
            m_SendTick = GetTickCount();
            m_Socket.ReceiveTimeout = ClientSetting.TIMEOUT;
            m_SendTick = GetTickCount();
            if (bThread)
            {
                th = new Thread(new ThreadStart(ThreadRecv));
                th.Start();
            }
            //Debug.Log("Connected");
            return true;
        }
        catch
        {
            return false;
        }
    }
    void CloseSocket()
    {
        if (m_Socket != null)
        {
            try
            {
                m_Socket.Close();
                m_Socket = null;
            }
            catch
            {

            }
        }
    }
    public void ConnectCallback(System.IAsyncResult ar)
    {
        Socket s = (Socket)ar.AsyncState;
        s.EndConnect(ar);
    }
    public uint Ping
    {
        get
        {
            return 0;
        }
    }
    bool RecvData(byte[] buff)
    {
        while (m_RecvSize >= ClientSetting.CMD_MIN_SIZE)
        {
            uint recvID = System.BitConverter.ToUInt32(buff, m_Offset);
            if (recvID == ConstValue.HEARBEAT_ID)
            {
                m_Offset += 4;
                m_RecvSize -= 4;
            }
            else if (recvID == ConstValue.PING_ID)
            {
                m_Offset += 4;
                m_RecvSize -= 4;
            }
            else
            {
                ushort cmdSize = System.BitConverter.ToUInt16(buff, m_Offset);
                if (cmdSize > buff.Length)
                    return false;
                if (m_RecvSize >= cmdSize)
                {
                    if (!m_Handler.Handle(buff, (ushort)m_Offset, (ushort)cmdSize))
                    {
                        return false;
                    }
                    m_Offset += cmdSize;
                    m_RecvSize -= cmdSize;
                }
                else
                    break;
            }
        }
        int freeBuffSize = buff.Length - (m_Offset + m_RecvSize);
        if (freeBuffSize < 128)
        {
            System.Array.Copy(buff, m_Offset, buff, 0, m_RecvSize);
            m_Offset = 0;
        }
        return true;
    }
    public bool Send(Socket socket, byte[] sendData)
    {
        try
        {
            return socket.Send(sendData, SocketFlags.None) == sendData.Length;
            
        }
        catch
        {
        }
        return false;
    }
    public bool Send(byte[] sendData)
    {
        try
        {
            return m_Socket.Send(sendData, SocketFlags.None) == sendData.Length;

        }
        catch
        {
        }
        return false;
    }
    public void ThreadRecv()
    {
        Socket socket = m_Socket;
        int ret = 0;
        byte[] sendData = null;
        const uint timeout = 1000;
        byte[] HEARBEAT_CMD = new byte[4] { 0xff, 0xff, 0xff, 0xff };
        while (m_bConnected)
        {
            try
            {
                int idx = m_Offset + m_RecvSize;
                ret = socket.Receive(m_Buff, idx, m_Buff.Length - idx, SocketFlags.None);
            }
            catch
            {
                ret = 0;
                if (socket.Connected == false)
                {
                    CloseSocket();
                    m_bNotify = true;
                    break;
                }
            }
            uint tick = GetTickCount();
            if (ret > 0)
            {
                m_RecvSize += ret;
                if (!RecvData(m_Buff))
                    break;
                m_RecvTick = tick;
            }
            else if(tick - m_RecvTick > ClientSetting.TIMEOUT)
            {
                m_bNotify = true;
                CloseSocket();
                break;
            }
            if (sendData != null)
            {
                if (Send(socket, sendData))
                {
                    sendData = null;
                    m_SendTick = tick;
                }
            }
            if (m_SendList.HasItem())
            {
                sendData = NetCmdHelper.CmdToBytes(m_SendList.GetItem(), 0);
                if (Send(socket, sendData))
                {
                    sendData = null;
                    m_SendTick = tick;
                }
            }
            else if (tick - m_SendTick > timeout)
            {
                if(Send(socket, HEARBEAT_CMD))
                    m_SendTick = tick;
            }
            Thread.Sleep(ClientSetting.SLEEP_TIME);
        }
    }
    public void Update()
    {
        if (m_bNotify)
        {
            m_bNotify = false;
            m_Handler.StateChanged(NetState.NET_DISCONNECT);
        }
    }
    public void Send<T>(NetCmdBase ncb)
    {
        SendCmdPack scp;
        scp.Cmd = ncb;
        scp.Hash = TypeSize<T>.HASH;
        if (m_SendList.HasSpace())
            m_SendList.AddItem(scp);
        else
            LogMgr.Log("发送命令队列已满");
    }
}
