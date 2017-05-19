using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
class PingData
{
    public string       IP;
    public ushort       Port;
    public uint         Ping;
    public uint         Online;
    public Socket       Socket;
    public IPEndPoint   endPoint;
    public volatile bool Completion;
}

class TCPPing
{
    public static void ThreadConnect(object obj)
    {
        PingData pd = (PingData)obj;
        byte[] buff =new byte[32];
        try
        {
            pd.Socket.Connect(pd.endPoint);
            int k = pd.Socket.Receive(buff, SocketFlags.None);
            if(k >= 4)
            {
                pd.Ping = Utility.GetTickCount() - pd.Ping;
                pd.Online = System.BitConverter.ToUInt32(buff, 0);
                pd.Socket.Close();
            }
        }
        catch
        {

        }
        pd.Socket = null;
        pd.Completion = true;
    }
    public static bool Ping(PingData[] PingList, uint timeout)
    {
        uint tick = Utility.GetTickCount();
        for (int i = 0; i < PingList.Length; ++i)
        {
            PingData pd = PingList[i];
            pd.Socket   = TCPClient.CreateSocket(true, true);
            pd.Ping     = tick;
            pd.Completion = false; 
            pd.endPoint = new IPEndPoint(IPAddress.Parse(pd.IP), pd.Port);
            Thread th = new Thread(new ParameterizedThreadStart(ThreadConnect));
            th.Start(pd);
        }
        while(Utility.GetTickCount() - tick < timeout)
        {
            uint k = 0;
            for (int i = 0; i < PingList.Length; ++i)
            {
                if(PingList[i].Completion)
                    ++k;
            }
            if (k == PingList.Length)
                return true;
        }
        return false;
    }
}

