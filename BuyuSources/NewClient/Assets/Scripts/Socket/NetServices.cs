using UnityEngine;
using System.Collections.Generic;
public class NetCmdPack
{
    public NetCmdBase  cmd;
    public uint tick;
}
public class NetServices:Singleton<NetServices>, INetHandler
{
    INetClient  m_TCPClient;
    ICmdHandler m_Handler;
    ICmdHandler m_FinalHandler; //最终的命令处理器

    SafeArray<NetCmdPack> m_CmdList = new SafeArray<NetCmdPack>(256);

    public void GlobalInit()
    {
        
        m_Handler = null;
    }
    public ICmdHandler FinalHandler
    {
        get { return m_FinalHandler; }
        set { m_FinalHandler = value; }
    }
    public ICmdHandler CmdHandler
    {
        get { return m_Handler; }
        set { m_Handler = value; }
    }
    public void Disconnect()
    {
        if(m_TCPClient != null)
        {
            m_TCPClient.Disconnect();
            m_TCPClient = null;
        }
    }
    public INetClient TcpClient
    {
        get
        {
            return m_TCPClient;
        }
    }
    public bool Connect(bool bTCP, string ip, ushort port, uint newip = 0, ushort newport = 0)
    {
        if(m_TCPClient != null && m_TCPClient.IsConnected)
        {
            LogMgr.Log("TCP is connected.");
            return false;
        }
        INetClient tt;
        //bTCP = true;
        bool bret;
        
        if (bTCP)
        {
            tt = new TCPClient(this);
            bret = tt.Connect(ip, port, newip, newport);
        }
        else
        {
            tt = new UDPClient(this);
            bret = tt.Connect(ip, port, newip, newport);
        }
		
        m_TCPClient = tt;
        return bret;
    }
    public bool IsConnected
    {
        get
        {
            return m_TCPClient != null && m_TCPClient.IsConnected;
        }
    }
    public void StateChanged(NetState e)
    {
        if(e == NetState.NET_DISCONNECT)
        {
            if (m_Handler != null)
                m_Handler.StateChanged(e);
            if (m_FinalHandler != null)
                m_FinalHandler.StateChanged(e);
        }
    }
    public void Send<T>(NetCmdBase ncb)
    {
        if(typeof(T) != ncb.GetType())
        {
            LogMgr.Log("命令类型不相等:" + ncb.ToString());
            return;
        }
        if (IsConnected)
        {
            m_TCPClient.Send<T>(ncb);
        }
        //else
        //    LogMgr.Log("TCPClient don't connected, send cmd:" + ncb.ToString());
    }
    public bool Handle(byte[] data, ushort offset, ushort length)
    {
        NetCmdBase cmd = NetCmdHelper.ByteToCmd(data, offset, length);
        if (cmd == null)
        {
            byte cmdType = data[offset+3];
            byte cmdSubType = data[offset+2];
            LogMgr.Log(StringTable.GetString("cmd_error")+":" + cmdType + ":" + cmdSubType);
            NativeInterface.ShowMsgAndExit("cmd_error", data[offset+2]);
            return false;
        }
        if (cmd.GetCmdType() == NetCmdType.CMD_HEARTBEAT)
        {
            //NetCmd nc = new NetCmd();
            //nc.SetCmdType(NetCmdType.CMD_HEARTBEAT);
            //Send<NetCmd>(nc);
        }
        else
        {
            NetCmdPack ncp = new NetCmdPack();
            ncp.cmd = cmd;
            ncp.tick = Utility.GetTickCount();
            m_CmdList.AddItem(ncp);
        }
        return true;
    }
    public void ClearCmd()
    {
        m_CmdList.Clear();
    }
    public void ClearCmdAndHandler()
    {
        m_Handler = null;
        m_CmdList.Clear();
    }
    public bool Update(float delta)
    {
        if(m_TCPClient != null)
            m_TCPClient.Update();

        if (m_Handler != null && m_Handler.CanProcessCmd())
        {
            while (m_CmdList.HasItem() && m_Handler != null)
            {
                NetCmdPack nc = m_CmdList.GetItem();
                if (m_Handler.Handle(nc) == false)
                    if(HeaderManager.Instance.HandleCmd(nc) == false)
                        m_FinalHandler.Handle(nc);
            }
        }
        return true;
    }
}
