using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家公告类
class RoleAnnouncement
{
    private bool m_IsLoadByServer = false;
    private List<AnnouncementOnce> pList = new List<AnnouncementOnce>();//最新的在最前面
    private bool m_IsSendToServer = false;
    public RoleAnnouncement()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_IsLoadByServer = false;
        m_IsSendToServer = false;
        pList.Clear();
    }
    public void ResetInfo()
    {
        OnClear();
        //触发公告改变的事件
        tagAnnouncementChangeEvent pEvent = new tagAnnouncementChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }
    public List<AnnouncementOnce> GetAllAnnouncementList()
    {
        if(m_IsLoadByServer)
        {
            return pList;
        }
        else
        {
            if (!m_IsSendToServer)
            {
                m_IsSendToServer = true;
                pList.Clear();
                SendGetAllAnnouncement();
            }
            return null;
        }
    }
    private bool SendGetAllAnnouncement()
    {
        //向服务器端查询全部的公告
        CL_Cmd_GetAllAnnouncement ncb = new CL_Cmd_GetAllAnnouncement();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetAllAnnouncement);
        NetServices.Instance.Send<CL_Cmd_GetAllAnnouncement>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetAllAnnouncement:
                return HandleLoadAllAnnouncement(obj);
            //case NetCmdType.CMD_LC_GetAllAnnouncementFinish:
            //    return HandleLoadAllAnnouncementFinish(obj);
            case NetCmdType.CMD_LC_SendNewAnnouncementOnce:
                return HandleAddAnnouncement(obj);
        }
        return true;
    }
    private bool HandleLoadAllAnnouncement(NetCmdBase obj)
    {
        LC_Cmd_GetAllAnnouncement ncb = (LC_Cmd_GetAllAnnouncement)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            pList.Clear();
        }
        for(UInt16 i =0;i<ncb.Sum;++i)
        {
            pList.Add(ncb.Array[i]);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadByServer = true;
            //触发命令
            while (pList.Count > FishConfig.Instance.m_SystemConfig.AnnouncementSum)
            {
                pList.RemoveAt(pList.Count - 1);//移除最后一个
            }
            tagAnnouncementChangeEvent pEvent = new tagAnnouncementChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //private bool HandleLoadAllAnnouncementFinish(NetCmdBase obj)
    //{
      
    //    return true;
    //}
    private bool HandleAddAnnouncement(NetCmdBase obj)
    {
        LC_Cmd_SendNewAnnouncementOnce ncb = (LC_Cmd_SendNewAnnouncementOnce)obj;
        pList.Insert(0, ncb.pOnce);
        while (pList.Count > FishConfig.Instance.m_SystemConfig.AnnouncementSum)
        {
            pList.RemoveAt(pList.Count - 1);//移除最后一个
        }
        //魅力命令
        tagAnnouncementChangeEvent pEvent = new tagAnnouncementChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    //private string GetNewAnnouncementStr(string Str)
    //{
    //    //将转化符号转化为 字符串 可能有多个  我们从头开始处理
    //    string NewStr = HandleDateTime(HandleShopStr(Str));
    //    return NewStr;
    //}
    //private string HandleDateTime(string Str)
    //{
    //    DateTime time = SystemTime.Instance.GetSystemDateTime;
    //    string TimeSrt = time.ToString();
    //    Str = Str.Replace("$[datetime]$", TimeSrt);
    //    return Str;
    //}
    //private string HandleShopStr(string Str)
    //{
    //    //$[ShopID=%d,%d]$"
    //    string DestStr = new string(Str.ToCharArray());
    //    int BeginIndex = DestStr.IndexOf("$[ShopID=");
    //    int EndIndex = DestStr.IndexOf("]$");
    //    while (BeginIndex != -1 && EndIndex != -1 && EndIndex > (BeginIndex + "$[ShopID=".Length))
    //    {
    //        //切割出字符串
    //        //
    //        string InfoStr = DestStr.Substring(BeginIndex + "$[ShopID=".Length, EndIndex - BeginIndex - "$[ShopID=".Length - "]$".Length + 2);
    //        string[] Array = InfoStr.Split(',');
    //        if(Array.Length == 2)
    //        {
    //            Byte ShopID = Convert.ToByte(Array[0]);
    //            Byte ShopOnlyID = Convert.ToByte(Array[1]);

    //            tagShopConfig pShopConfig;
    //            tagShopItemStr pShopStr;
    //            if (FishConfig.Instance.m_ShopInfo.ShopMap.TryGetValue(ShopID, out pShopConfig) && pShopConfig.ShopItemStrMap.TryGetValue(ShopOnlyID, out pShopStr))
    //            {
    //                string Name = pShopStr.ItemName;
    //                string ReplaceStr = "$[ShopID=" + InfoStr + "]$";
    //                //替换到字符串里面去
    //                DestStr = DestStr.Replace(ReplaceStr, Name);//替换掉数据

    //            }
    //            else
    //            {
    //                return DestStr;
    //            }
    //        }
    //        else
    //        {
    //            return DestStr;
    //        }
    //        BeginIndex = DestStr.IndexOf("$[ShopID=");
    //        EndIndex = DestStr.IndexOf("]$");
    //    }
    //    return DestStr;
    //}
}