using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Collections;
using System.Threading;
public class NewUDPClient : INetClient
{
    //---------------------------------------
    volatile uint m_Delay = 0;
    volatile bool m_bConnected = false;
    volatile bool m_bNotify = false;
    uint m_SendID = 1;
    uint m_SendTick = 0;
    uint m_RecvID = 1;
    uint m_SendCmdTick = 0;
    uint m_RecvTick = 0;
    byte[] BUFF = new byte[ClientSetting.RECV_BUFF_SIZE];
    byte[] HEARBEAT_CMD = new byte[4] { 0xff, 0xff, 0xff, 0xff };
    byte[] PING_CMD = System.BitConverter.GetBytes(0x8fffffff);
    bool m_bSendPing = false;
    bool m_bSendBackID = false;
    uint m_InSceneMask = 0x80000000;
    Socket m_Socket;
    INetHandler m_Handler;
    SafeArray<SendCmdPack> m_SendList = new SafeArray<SendCmdPack>(64);
    byte[] m_SendBuff;
    //---------------------------------------
    public NewUDPClient(INetHandler handler)
    {
        m_Handler = handler;
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
    public void JoinScene(bool bjoin)
    {
        if(bjoin)
            m_InSceneMask = 0xc0000000;
        else
            m_InSceneMask = 0x80000000;
    }
    void ThreadConnect(object obj)
    {
        m_Socket.Connect((EndPoint)obj);
        m_bConnected = m_Socket.Connected;
    }
    public bool Connect(string ip, int port, uint gameIP, ushort gamePort, bool bThread = true)
    {
        const uint CONNECT_TIME_OUT = 3000;
        try
        {
            m_bConnected = false;
            CloseSocket();
            m_Socket = TCPClient.CreateSocket(true, true);
            IPEndPoint endpoit = new IPEndPoint(IPAddress.Parse(ip), port);
            Thread th = new Thread(new ParameterizedThreadStart(ThreadConnect));
            th.Start(endpoit);
            //连接TCP
            //===============================================
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
            //接收TCP返回数据
            //===============================================
            byte[] buff = new byte[64];
            int ret = m_Socket.Receive(buff, SocketFlags.None);
            CloseSocket();

            if (ret != 16 || System.BitConverter.ToUInt32(buff, 0) != ClientSetting.CONNECT_RESULT)
                return false;
            uint Rand1 = System.BitConverter.ToUInt32(buff, 4) | 0xc0000000;
            uint Rand2 = System.BitConverter.ToUInt32(buff, 8) | 0xc0000000;
            int ServerPort = System.BitConverter.ToInt32(buff, 12);
            System.Array.Copy(System.BitConverter.GetBytes(0x8fffffff), 0, buff, 0, 4);
            System.Array.Copy(System.BitConverter.GetBytes(Rand1), 0, buff, 4, 4);
            System.Array.Copy(System.BitConverter.GetBytes(Rand2), 0, buff, 8, 4);
            System.Array.Copy(System.BitConverter.GetBytes(gameIP), 0, buff, 12, 4);
            System.Array.Copy(System.BitConverter.GetBytes(gamePort), 0, buff, 16, 2);

            m_Socket = TCPClient.CreateSocket(false, true);
            endpoit = new IPEndPoint(IPAddress.Parse(ip), ServerPort);
            m_Socket.Connect(endpoit);
            m_Socket.Blocking = false;
            //等待UDP双向确认。
            //===============================================
            bool bind = false;
            tick = GetTickCount();
            while (GetTickCount() - tick < CONNECT_TIME_OUT)
            {
                try
                {
                    m_Socket.Send(buff, 0, 18, SocketFlags.None);
                    ret = m_Socket.Receive(BUFF, 0, BUFF.Length, SocketFlags.None);
                }
                catch
                {
                    ret = 0;
                }

                if (ret == 4 && System.BitConverter.ToUInt32(BUFF, 0) == ConstValue.HEARBEAT_ID)
                {
                    //与服务器绑定成功, 发送4次心跳。
                    for (int i = 0; i < 4; ++i)
                    {
                        try
                        {
                            m_Socket.Send(HEARBEAT_CMD, SocketFlags.None);
                        }
                        catch (System.Exception e)
                        {
                            //LogMgr.Log("UDP最后确认异常:" + e.ToString());
                        }
                        Thread.Sleep(10);
                    }
                    bind = true;
                    break;
                }
                Thread.Sleep(50);
            }
            if (bind == false)
            {
                CloseSocket();
                return false;
            }
            //UDP连接成功。
            //===============================================
            m_bNotify = false;
            m_RecvTick = GetTickCount();
            m_SendTick = GetTickCount();
            m_SendID = 1;
            m_RecvID = 1;
            m_bConnected = true;
            th = new Thread(new ThreadStart(ThreadRecv));
            th.Start();
            return true;
        }
        catch (System.Exception e)
        {
            LogMgr.Log("UDP连接失败:" + e.ToString());
        }
        return false;
    }
    public void Update()
    {
        if (m_bNotify)
        {
            m_bNotify = false;
            m_Handler.StateChanged(NetState.NET_DISCONNECT);
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
        m_bConnected = false;
    }
    public uint Ping
    {
        get
        {
            return m_Delay;
        }
    }
    bool IS_ANSWER(uint X)
    {
        return (X & 0x80000000) != 0;
    }
    uint SET_ANSWER(uint X)
    {
        return X | m_InSceneMask;
    }
    uint RESET_ANSWER(uint X)
    {
        return X & 0x7fffffff;
    }
    bool RecvData(byte[] buff, int recvSize, Socket soekt)
    {

        return true;
    }
    bool Send(Socket s, byte[] data)
    {
        try
        {
            int ret = s.Send(data, SocketFlags.None);
            return ret == data.Length;
        }
        catch
        {
            return false;
        }
    }
    void ToBytes(uint id, byte[] buff, int offset)
    {
        buff[offset + 0] = (byte)(id & 0xff);
        buff[offset + 1] = (byte)((id >> 8) & 0xff);
        buff[offset + 2] = (byte)((id >> 16) & 0xff);
        buff[offset + 3] = (byte)((id >> 24) & 0xff);
    }
    bool RecvData(Socket socket, int recvSize)
    {
        int offset = 0;
        while (offset < recvSize)
        {
            uint recvID = System.BitConverter.ToUInt32(BUFF, offset);
            offset += 4;
            if (recvID == ConstValue.HEARBEAT_ID)	//心跳
            {
                // Debug.Log("recv hearbeat.");
            }
            else if (recvID == ConstValue.PING_ID)
            {
                //m_bSendPing = true;
            }
            else if (IS_ANSWER(recvID))
            {
                recvID = RESET_ANSWER(recvID);
                if (recvID == m_SendID && m_SendBuff != null)
                    m_SendBuff = null;
            }
            else
            {
                byte cmdCount = BUFF[offset++];
                uint maxID = recvID + cmdCount;
                if (maxID > m_RecvID)
                {
                    for (uint id = recvID; id < maxID; ++id)
                    {
                        ushort cmdSize = System.BitConverter.ToUInt16(BUFF, offset);
                        if (cmdSize > recvSize - offset)
                            return false;
                        ushort cmdType = System.BitConverter.ToUInt16(BUFF, offset + 2);
                        if (id >= m_RecvID && cmdType != 0)
                        {
                            m_Handler.Handle(BUFF, (ushort)offset, (ushort)cmdSize);
                        }
                        offset += cmdSize;
                    }
                    m_RecvID = maxID;
                }
                m_bSendBackID = true;
                break;
            }
        }
        return true;
    }
    public void ThreadRecv()
    {
        Socket socket = m_Socket;
        int timeout = ClientSetting.TIMEOUT >> 1;
        int recvSize = 0;
        while (m_bConnected)
        {
            uint tick = GetTickCount();
#if UNITY_EDITOR
            if (SceneMain.Exited)
            {
                CloseSocket();
                break;
            }
#endif
            for (int i = 0; i < 2; ++i)
            {
                try
                {
                    recvSize = socket.Receive(BUFF, 0, BUFF.Length, SocketFlags.None);
                    if (recvSize > 0)
                    {
                        if (RecvData(socket, recvSize) == false)
                        {
                            CloseSocket();
                            m_bNotify = true;
                            return;
                        }
                        else
                            m_RecvTick = tick;
                    }
                    else
                        break;
                }
                catch
                {
                    break;
                }
            }
            if (tick - m_RecvTick > ClientSetting.TIMEOUT)
            {
                CloseSocket();
                m_bNotify = true;
                return;
            }
            if (m_SendBuff != null)
            {
                if (tick - m_SendCmdTick > ClientSetting.UDP_RESEND_TICK)
                {
                    ToBytes(SET_ANSWER(m_RecvID), m_SendBuff, 0);
                    if (Send(socket, m_SendBuff))
                    {
                        m_bSendBackID = false;
                        m_SendTick = m_SendCmdTick = tick;
                    }
                }
                else if (m_bSendBackID)
                {
                    ToBytes(SET_ANSWER(m_RecvID), HEARBEAT_CMD, 0);
                    if (Send(socket, HEARBEAT_CMD))
                    {
                        m_bSendBackID = false;
                        m_SendTick = tick;
                    }
                }
            }
            else if (m_SendList.HasItem())
            {
                m_SendBuff = NetCmdHelper.CmdToBytes(m_SendList.GetItem(), 8);
                ++m_SendID;
                ToBytes(SET_ANSWER(m_RecvID), m_SendBuff, 0);
                ToBytes(m_SendID, m_SendBuff, 4);
                if (Send(socket, m_SendBuff))
                {
                    m_bSendBackID = false;
                    m_SendTick = m_SendCmdTick = tick;
                }
                else
                    m_SendCmdTick = 0;
            }
            else if (m_bSendBackID || tick - m_SendTick > timeout)
            {
                ToBytes(SET_ANSWER(m_RecvID), HEARBEAT_CMD, 0);
                if (Send(socket, HEARBEAT_CMD))
                {
                    m_SendTick = tick;
                    m_bSendBackID = false;
                }
            }
            
            Thread.Sleep(1);
        }
        //Debug.Log("exit recv thread");
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
