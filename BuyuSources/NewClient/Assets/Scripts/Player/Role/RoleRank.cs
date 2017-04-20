using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
//用户排行榜管理器
class RoleRank
{
    private bool m_IsLoadXmlInfo = false;
    private Dictionary<Byte, RankInfo> RankXmlMap = new Dictionary<Byte, RankInfo>();  //XMl配置文件的数据处理
    private bool m_IsLoadInfo = false;
    private Dictionary<Byte, tagRankWeekRankClient> WeekRandIndex = new Dictionary<Byte, tagRankWeekRankClient>();
    private bool m_IsLoadByServer = false;
   //private bool m_IsExistsWeekReward = false;
    public RoleRank()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_IsLoadInfo = false;
        m_IsLoadXmlInfo = false;
        WeekRandIndex.Clear();
        m_IsLoadByServer = false;
        //m_IsExistsWeekReward = false;
        RankXmlMap.Clear();
    }

    //public bool GetIsExistsWeekRankReward()
    //{
    //    return m_IsExistsWeekReward;
    //}
    //public void SetExistsWeekRankReward(bool States)
    //{
    //    if (m_IsExistsWeekReward != States)
    //    {
    //        m_IsExistsWeekReward = States;
    //        //发送事件
    //        tagRankExistsRewardEvent pEvent = new tagRankExistsRewardEvent();
    //        MsgEventHandle.HandleMsg(pEvent);
    //    }
    //    else
    //        m_IsExistsWeekReward = States;
    //}
    //private void CheckIsExistsRankReward()
    //{
    //    foreach (tagRankWeekRankClient var in WeekRandIndex.Values)
    //    {
    //        if(!var.IsReward)
    //        {
    //            SetExistsWeekRankReward(true);
    //            return;
    //        }
    //    }
    //    SetExistsWeekRankReward(false);
    //}

    public void ResetInfo()
    {
        OnClear();
        //清理排行榜的数据 在排行榜打开的情况下 
        tagRankChangeEvent pEvent = new tagRankChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }

    //public void RankTest()
    //{
    //    //排行榜测试数据
    //    tagRankWeekReward pRank1 = new tagRankWeekReward();
    //    pRank1.IndexInfo.RankID = 1;
    //    pRank1.IndexInfo.RankIndex = 1;
    //    pRank1.IsReward = false;
    //    WeekRandIndex.Add(pRank1.IndexInfo.RankID, pRank1);

    //    tagRankWeekReward pRank2 = new tagRankWeekReward();
    //    pRank2.IndexInfo.RankID = 2;
    //    pRank2.IndexInfo.RankIndex = 1;
    //    pRank2.IsReward = true;
    //    WeekRandIndex.Add(pRank2.IndexInfo.RankID, pRank2);

    //    m_IsLoadInfo = true;

    //    tagRankChangeEvent pEvent = new tagRankChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);
    //}
    public Dictionary<Byte, tagRankWeekRankClient> GetWeekRankInfo()
    {
        if (m_IsLoadInfo)
        {
            return WeekRandIndex;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                SendGetRankInfo();
            }
            //RankTest();
            return null;
        }
    }
    public bool GetWeekRankReward(Byte RankID)
    {
        return SendGetRankReward(RankID);
    }


    public Dictionary<Byte, RankInfo> GetRankInfo()
    {
        if (m_IsLoadXmlInfo)
        {
            return RankXmlMap;
        }
        else
        {
            //为加载过数据
            if (LoadRankXmlFile())
            {
                return RankXmlMap;
            }
            else
                return null;
        }
    }
    public void OnDayChange()
    {
        //当天数发生变化的时候 我们处理排行榜的数据
        m_IsLoadXmlInfo = false;//重新读取
        RankXmlMap.Clear();
        LoadRankXmlFile();

        //当天数发生变化的时候 我们想要判断是否为星期天 是的话 我们想要重新获取玩家的周排行榜的数据 
        //扣除时间
        DateTime pTime = SystemTime.Instance.GetSystemDateTime.AddSeconds(FishConfig.Instance.GetWriteSec() * -1);
        if ((Byte)pTime.DayOfWeek == FishConfig.Instance.m_RankInfo.RewardWeekDay)
        {
            m_IsLoadByServer = false;
            m_IsLoadInfo = false;
            GetWeekRankInfo();
        }
    }
    public void DownLoadReadRankXml(bool IsError = false)
    {
        LoadRoleRankXml(IsError);
        tagRankXmlChangeEvent pEvent = new tagRankXmlChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);//添加排行榜XML文件读取成功的函数 
    }
    public void SaveRankXml(Byte[] FileText)
    {
        DateTime time = SystemTime.Instance.GetSystemDateTime;
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        time = time.AddSeconds(WriteSec * -1);
        string FileName = string.Format("Rank_{0}_{1}_{2}.xml", time.Year, time.Month, time.Day);
        RuntimeInfo.SaveLocalFile(FileName, FileText);
    }
    private bool LoadRankXmlFile()
    {
        //判断文件是否存在
        DateTime time = SystemTime.Instance.GetSystemDateTime;
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        time = time.AddSeconds(WriteSec * -1);
        string FileName = string.Format("Rank_{0}_{1}_{2}.xml", time.Year, time.Month, time.Day);
        //判断客户端是否存在这个文件
        try
        {
            LoadRoleRankXml();
            return true;
        }
        catch (Exception ex)
        {

            string FtpFileName = ServerSetting.RT_XML_DIR + FileName;
            //从FTP下载文件
            FTPClient.Instance.AddDownload(DownloadType.DOWNLOAD_RANK_INFO, FtpFileName, null);
            return false;
        }

    }
    private bool LoadRoleRankXml(bool IsError = false)
    {
        if (IsError)
        {
            m_IsLoadXmlInfo = true;
            RankXmlMap.Clear();//清空数据
            tagRankXmlChangeEvent pEvent = new tagRankXmlChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);//添加排行榜XML文件读取成功的函数 
            return true;
        }
        //读取配置文件 并且加载到数据集合里面去
        DateTime time = SystemTime.Instance.GetSystemDateTime;
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        time = time.AddSeconds(WriteSec * -1);
        string FileName = string.Format("Rank_{0}_{1}_{2}.xml", time.Year, time.Month, time.Day);

        Byte[] FileText = RuntimeInfo.GetLocalFile(FileName);
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(System.Text.Encoding.UTF8.GetString(FileText));
        XmlElement pRankConfig = doc.DocumentElement;
        for (int i = 0; i < doc.ChildNodes.Count; ++i)
        {
            if (doc.ChildNodes[i].Name == "FishRanks")
            {
                for (int k = 0; k < doc.ChildNodes[i].ChildNodes.Count; ++k)
                {
                    if (doc.ChildNodes[i].ChildNodes[k].Name == "FishRook")
                    {
                        RankInfo pRankInfo = new RankInfo();
                        pRankInfo.RankID = Convert.ToByte(doc.ChildNodes[i].ChildNodes[k].Attributes["TypeID"].Value);

                        for (int j = 0; j < doc.ChildNodes[i].ChildNodes[k].ChildNodes.Count; ++j)
                        {
                            if (doc.ChildNodes[i].ChildNodes[k].ChildNodes[j].Name == "Info")
                            {
                                tagRankInfo pInfo = new tagRankInfo();
                                pInfo.bIndex = Convert.ToByte(doc.ChildNodes[i].ChildNodes[k].ChildNodes[j].Attributes["Index"].Value);
                                pInfo.dwFaceID = Convert.ToUInt32(doc.ChildNodes[i].ChildNodes[k].ChildNodes[j].Attributes["FaceID"].Value);
                                pInfo.dwUserID = Convert.ToUInt32(doc.ChildNodes[i].ChildNodes[k].ChildNodes[j].Attributes["UserID"].Value);
                                pInfo.Gender = Convert.ToByte(doc.ChildNodes[i].ChildNodes[k].ChildNodes[j].Attributes["Gender"].Value) == 1 ? true : false;
                                pInfo.NickName = Convert.ToString(doc.ChildNodes[i].ChildNodes[k].ChildNodes[j].Attributes["Name"].Value);
                                pInfo.TitleID = Convert.ToByte(doc.ChildNodes[i].ChildNodes[k].ChildNodes[j].Attributes["TitleID"].Value);
                                pInfo.Param = Convert.ToInt64(doc.ChildNodes[i].ChildNodes[k].ChildNodes[j].Attributes["Param"].Value);

                                pRankInfo.RankMap.Add(pInfo.bIndex, pInfo);
                            }
                        }
                        RankXmlMap.Add(pRankInfo.RankID, pRankInfo);
                    }
                }
                m_IsLoadXmlInfo = true;
                return true;
            }
        }
        return false;
    }
    private void SendGetRankInfo()
    {
        CL_Cmd_GetRankInfo ncb = new CL_Cmd_GetRankInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetRankInfo);
        NetServices.Instance.Send<CL_Cmd_GetRankInfo>(ncb);
    }
    public bool SendGetRankReward(Byte RankID)
    {
        //获取指定的排行榜的奖励
        //判断指定排行榜是否存在
        if (!FishConfig.Instance.m_RankInfo.m_RankMap.ContainsKey(RankID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Rank_GetWeekReward_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //玩家是否上榜
        if (!WeekRandIndex.ContainsKey(RankID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Rank_GetWeekReward_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (WeekRandIndex[RankID].IsReward)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Rank_GetWeekReward_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_GetRankReward ncb = new CL_Cmd_GetRankReward();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetRankReward);
        ncb.RankID = RankID;
        NetServices.Instance.Send<CL_Cmd_GetRankReward>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetRankReward:
                return HandleGetRankReward(obj);
            //case NetCmdType.CMD_LC_GetRankInfoFinish:
            //    return HandleGetRankInfoFinish(obj);
            case NetCmdType.CMD_LC_GetWeekRankInfo:
                return HandleGetRankWeekInfo(obj);
        }
        return true;
    }

    bool HandleGetRankReward(NetCmdBase obj)
    {
        LC_Cmd_GetRankReward ncb = (LC_Cmd_GetRankReward)obj;
        if (!WeekRandIndex.ContainsKey(ncb.RankID))
            return false;
        WeekRandIndex[ncb.RankID].IsReward = true;
        //UIWidgetManager.Instance.UpdateUserInfRankInf(false);

        //CheckIsExistsRankReward();

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Rank_GetWeekReward_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        tagRankGetRewardEvent pEvent = new tagRankGetRewardEvent(ncb.RankID);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleGetRankWeekInfo(NetCmdBase obj)
    {
        LC_Cmd_GetWeekRankInfo ncb = (LC_Cmd_GetWeekRankInfo)obj;
        if((ncb.States & FishDataInfo.MsgBegin)!= 0)
        {
            WeekRandIndex.Clear();
        }
        for (int i = 0; i < ncb.Sum; ++i)
        {
            WeekRandIndex.Add(ncb.Array[i].RankID, ncb.Array[i]);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadInfo = true;
            tagRankChangeEvent pEvent = new tagRankChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //bool HandleGetRankInfoFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_GetRankInfoFinish ncb = (LC_Cmd_GetRankInfoFinish)obj;
    //    //UIWidgetManager.Instance.UpdateUserInfRankInf();
        

    //    return true;
    //}
}