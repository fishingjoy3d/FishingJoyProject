using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
//玩家比赛的数据集合 用于设置当前比赛的信息 和 数据
class RoleMonth
{
    //用于记录比赛的数据
    private bool m_IsLoadInfo = false;
    private Dictionary<Byte, Byte> SignUpList = new Dictionary<Byte, Byte>();
    private Dictionary<Byte, UInt16> MonthPlayerSum = new Dictionary<Byte, UInt16>();
    //private bool IsLoadMonthPlayerSum = false;
    private tagMonthRankMap m_MonthRankMap = new tagMonthRankMap();//排行榜数据
    private bool m_IsLoadByServer = false;
    //用于下载比赛的排行榜数据
    private tagMonthRoleInfo[] m_MonthInfo= null;
    public RoleMonth()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_MonthRankMap.m_RankInfo.Clear();
        SignUpList.Clear();
        MonthPlayerSum.Clear();
        m_IsLoadInfo = false;
        m_IsLoadByServer = false;
        m_MonthInfo = null;
       // IsLoadMonthPlayerSum = false;
    }

    public void ResetInfo()
    {
        OnClear();

        //报名数据清空了
        tagMonthSignUpChangeEvent pEvnet = new tagMonthSignUpChangeEvent();
        MsgEventHandle.HandleMsg(pEvnet);
        //所有比赛排行榜清空了
       foreach(tagMonthConfig var in  FishConfig.Instance.m_MatchInfo.m_MonthVec)
       {
           tagMonthRankXmlChangeEvent pEventMonth = new tagMonthRankXmlChangeEvent(var.MonthID);
           MsgEventHandle.HandleMsg(pEventMonth);
       } 
    }

    //提供对外函数
    //public void GetMonthRankXml(Byte MonthID)
    //{
    //    //获取指定比赛的排行榜数据 比赛必须是已经结束了的 并且为开启的 就是上一局的数据
    //    //1根据比赛ID 获取上一场比赛的排行数据  进行读取 如果存在 直接进行获取 如果不存在通过FTP异步下载  在设置到UI上面去 没有BOOL控制下载不下载

    //}
    public tagMonthRoleInfo[] GetMonthList()
    {
        return m_MonthInfo;//有可能返回null
    }
    public void OnLeaveTable()
    {
        m_MonthInfo = null;
    }
    public Dictionary<Byte, Byte> GetSignUpList()
    {
        if(m_IsLoadInfo)
        {
            return SignUpList;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                SignUpList.Clear();
                SendGetSignUpInfo();
            }
            return null;
            //SignUpList.Clear();
            //SendGetSignUpInfo();
            //return null;
        }
    }
    public bool SignUpMonth(Byte Month)
    {
        return SendSignUpMonth(Month);
    }
    public bool JoinMonth(Byte Month)
    {
        return SendRoleJoinMonth(Month);
    }
    //public void OnUpdate()
    //{
    //    SendGetMonthSigupRoleSum();
    //}
    public bool AddMonthGlobel()
    {
        return SendRoleAddMonthGlobel();
    }
    public bool ResetNowMonth(Byte MonthID)
    {
        return SendResetMonthInfo(MonthID);
    }
    public tagMonthRankOnce GetMonthRankInfo(Byte MonthID)//根据战场ID 获取战场的排行榜数据
    {
        //获取指定的战场的配置文件的时候 无法确定战场的配置文件是否为最新的 所以每次都需要重新读取 无法进行缓存
        string FileName = GetMonthRankFileName(MonthID);
        //根据ID 和 名称 获取指定的数据
        if(m_MonthRankMap.m_RankInfo.ContainsKey(MonthID))
        {
            if(m_MonthRankMap.m_RankInfo[MonthID].FileName == FileName)
            {
                return m_MonthRankMap.m_RankInfo[MonthID];
            }
            else
            {
                if (LoadMonthRankConfig(MonthID, FileName))
                {
                    //读取成功了
                    return m_MonthRankMap.m_RankInfo[MonthID];
                }
                else
                {
                    return null;//等待FTP进行下载
                }
            }
        }
        else
        {
            //去读取 
            if (LoadMonthRankConfig(MonthID, FileName))
            {
                //读取成功了
                return m_MonthRankMap.m_RankInfo[MonthID];
            }
            else
            {
                return null;//等待FTP进行下载
            }
        }
    }
    public UInt16 GetMonthSigUpPlayerSum(Byte MonthID)
    {
        UInt16 PlayerSum = 0;
        if (MonthPlayerSum.TryGetValue(MonthID, out PlayerSum))
            return PlayerSum;
        else
            return 0;

        //if(IsLoadMonthPlayerSum)
        //{
           
        //}
        //else
        //{
        //   // SendGetMonthSigupRoleSum();
        //    return 0;
        //}
    }
    public bool GetMonthRankTime(Byte MonthID, ref DateTime BeginTime, ref DateTime EndTime)//获取当前排行榜的显示的比赛的开始和结束时间
    {
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.ContainsKey(MonthID))
            return false;
        FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].GetMaxNearEndMonthStarTime(10, out BeginTime, out EndTime);
        return true;
    }
    public void OnDownLoadMonthRankXml(string FilePath, bool IsErrror = false)
    {
        LoadMonthRankXml(FilePath, IsErrror);
        //执行事件
        Byte MonthID = GetMonthIDByFileName(FilePath);
        tagMonthRankXmlChangeEvent pEvent = new tagMonthRankXmlChangeEvent(MonthID);
        MsgEventHandle.HandleMsg(pEvent);
    }
    public void SaveMonthRankXml(Byte[] XmlInfo, string FilePath)
    {
        RuntimeInfo.SaveLocalFile(FilePath, XmlInfo);
    }
    //文件下载的一些处理函数
    private string GetMonthRankFileName(Byte MonthID)
    {
        //获取当前类型的 已经结束超过10分钟的比赛排行数据
        //获取最近的比赛的时间
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.ContainsKey(MonthID))
            return null;
        DateTime MonthBeginTime;
        DateTime MonthEndTime;
        FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].GetMaxNearEndMonthStarTime(10, out MonthBeginTime, out MonthEndTime);
        string FileName = string.Format("Month_{0}_{1}_{2}_{3}_{4}_{5}.xml", MonthID, MonthEndTime.Year, MonthEndTime.Month, MonthEndTime.Day, MonthEndTime.Hour, MonthEndTime.Minute);
        return FileName;
    }
    private Byte GetMonthIDByFileName(string FileName)
    {
        if (FileName == null)
            return 0;
        int StartIndex = FileName.IndexOf("_")+1;
        int Length = FileName.Substring(StartIndex).IndexOf("_");
        return Convert.ToByte(FileName.Substring(StartIndex, Length));
    }
    private bool LoadMonthRankConfig(Byte MonthID,string FileName)
    {
        //进行XML文件加载
        //1.判断文件是否存在
        try
        {
            LoadMonthRankXml(FileName);
            return true;
        }
        catch (Exception ex)
        {
            m_MonthRankMap.m_RankInfo.Remove(MonthID);
            //进行FTP下载 等待回调处理
            string FtpFileName = ServerSetting.RT_XML_DIR + FileName;
            FTPClient.Instance.AddDownload(DownloadType.DOWNLOAD_Month_Rank, FtpFileName, FileName);//
            return false;
        }
    }
    private bool LoadMonthRankXml(string FilePath, bool IsErrror = false)
    {
        Byte MonthID  =GetMonthIDByFileName(FilePath);
        if (IsErrror)
        {
            tagMonthRankOnce pOnce = new tagMonthRankOnce();
            pOnce.MonthID = MonthID;
            pOnce.FileName = FilePath;
            pOnce.m_RankInfo = null;
            m_MonthRankMap.m_RankInfo.Remove(MonthID);
            m_MonthRankMap.m_RankInfo.Add(MonthID, pOnce);
            return true;
        }
        //读取指定名称的战场的排行数据
        Byte[] FileText = RuntimeInfo.GetLocalFile(FilePath);
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(System.Text.Encoding.UTF8.GetString(FileText));
        XmlElement pFishMonth = doc.DocumentElement;
        Byte FileMonthID = Convert.ToByte(pFishMonth.Attributes["MonthID"].Value);
        tagMonthRankOnce pOnce2 = new tagMonthRankOnce();
        foreach (XmlNode var1 in pFishMonth.ChildNodes)
        {
            if (var1.Name == "RoleInfo")
            {
                tagMonthRank pRank = new tagMonthRank();
                pRank.Index = Convert.ToByte(var1.Attributes["Index"].Value);
                pRank.UserID = Convert.ToUInt32(var1.Attributes["UserID"].Value);
                pRank.NickName = Convert.ToString(var1.Attributes["NickName"].Value);
                pRank.Socres = Convert.ToUInt32(var1.Attributes["Socre"].Value);
                pRank.VipSocres = Convert.ToUInt32(var1.Attributes["VipSocre"].Value);
                pRank.FaceID = Convert.ToUInt32(var1.Attributes["FaceID"].Value);
                pOnce2.m_RankInfo.Add(pRank.Index, pRank);
            }
        }
        pOnce2.MonthID = FileMonthID;
        pOnce2.FileName = FilePath;
        if (m_MonthRankMap.m_RankInfo.ContainsKey(FileMonthID))
            m_MonthRankMap.m_RankInfo.Remove(FileMonthID);
        m_MonthRankMap.m_RankInfo.Add(FileMonthID, pOnce2);
        return true;
    }


    //比赛消息处理
    private void SendGetSignUpInfo() //获取玩家比赛的报名信息
    {
        CL_Cmd_LoadRoleSignUpInfo ncb = new CL_Cmd_LoadRoleSignUpInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_LoadRoleSignUpInfo);
        NetServices.Instance.Send<CL_Cmd_LoadRoleSignUpInfo>(ncb);
    }
    private bool SendSignUpMonth(Byte MonthID)
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetLevel() < FishConfig.Instance.m_MatchInfo.BeginMonthLevel)
        {
            //玩家点击不够 无法开始比赛
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_8);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //玩家报名参加比赛 
        //1.需要进行时间判断 当前是否在报名时间内
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.ContainsKey(MonthID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        if (FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].NowIsInSignTime() == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        if (SignUpList.ContainsKey(MonthID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        //1.判断物品是否足够
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].SignGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetMedal() < FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].SignMadel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].SignCurrey)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!PlayerRole.Instance.RoleInfo.IsCanUseRateIndex(FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].MinRateLimit))
        {
            //携带参数的提示
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_9, BulletSetting.BulletRate[FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].MinRateLimit].ToString());
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].SignItem.ItemID != 0 && FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].SignItem.ItemSum > 0)
        {
            if (PlayerRole.Instance.ItemManager.GetItemSum(FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].SignItem.ItemID) <
           FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].SignItem.ItemSum
           )
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_5);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            } 
        }
        Byte TableType = FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].TableTypeID;
        if (PlayerRole.Instance.TableManager.IsCanJoinTable(TableType, true) != TableError.TE_Sucess)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_6);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 

        CL_Cmd_SignUpMonth ncb = new CL_Cmd_SignUpMonth();
        ncb.SetCmdType(NetCmdType.CMD_CL_SignUpMonth);
        ncb.MonthID = MonthID;
        NetServices.Instance.Send<CL_Cmd_SignUpMonth>(ncb);
        return true;
    }
    private bool SendRoleJoinMonth(Byte MonthID)
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetLevel() < FishConfig.Instance.m_MatchInfo.BeginMonthLevel)
        {
            //玩家点击不够 无法开始比赛
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_JoinMonth_Failed_6);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.ContainsKey(MonthID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_JoinMonth_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].NowIsInStartTime() == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_JoinMonth_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!SignUpList.ContainsKey(MonthID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_JoinMonth_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        Byte TableType = FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].TableTypeID;
        if (PlayerRole.Instance.TableManager.IsCanJoinTable(TableType, true) != TableError.TE_Sucess)//比赛无须转化
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_JoinMonth_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        //1.如果玩家在桌子里 我们让玩家离开桌子
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetSeat() != 0xff)
        {
            CL_Cmd_LeaveTable ncbLeave = new CL_Cmd_LeaveTable();
            ncbLeave.SetCmdType(NetCmdType.CMD_CL_LeaveTable);
            NetServices.Instance.Send<CL_Cmd_LeaveTable>(ncbLeave);
        }
        CL_Cmd_JoinMonth ncb = new CL_Cmd_JoinMonth();
        ncb.SetCmdType(NetCmdType.CMD_CL_JoinMonth);
        ncb.MonthID = MonthID;
        NetServices.Instance.Send<CL_Cmd_JoinMonth>(ncb);
        return true;
    }
    //private void SendGetMonthSigupRoleSum()
    //{
    //    CL_Cmd_GetMonthRoleSum ncb = new CL_Cmd_GetMonthRoleSum();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_GetMonthRoleSum);
    //    NetServices.Instance.Send<CL_Cmd_GetMonthRoleSum>(ncb);
    //}
    private bool SendRoleAddMonthGlobel()
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_AddMonthGlobel_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        //玩家进行比赛的续币操作
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].m_ConvertMap.ContainsKey((Byte)(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthAddGlobelNum() + 1)))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_AddMonthGlobel_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        //判断玩家金币是否足够
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() <
            FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].m_ConvertMap[(Byte)(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthAddGlobelNum() + 1)].LostGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_AddMonthGlobel_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        CL_Cmd_ChangeUserAddMonthGlobelNum ncb = new CL_Cmd_ChangeUserAddMonthGlobelNum();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeUserAddMonthGlobelNum);
        NetServices.Instance.Send<CL_Cmd_ChangeUserAddMonthGlobelNum>(ncb);
        return true;
    }
    private bool SendResetMonthInfo(Byte MonthID)
    {
        //处理玩家当前的比赛数据
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.ContainsKey(MonthID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_ResetMonth_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!SignUpList.ContainsKey(MonthID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_ResetMonth_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (FishConfig.Instance.m_MatchInfo.m_MonthMap[MonthID].NowIsInStartTime() == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_ResetMonth_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].SignGlobel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_ResetMonth_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetMedal() < FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].SignMadel)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_ResetMonth_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].SignCurrey)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_ResetMonth_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        } 
        if (FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].SignItem.ItemID != 0 && FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].SignItem.ItemSum > 0)
        {
            if (PlayerRole.Instance.ItemManager.GetItemSum(FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].SignItem.ItemID) <
           FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].SignItem.ItemSum
           )
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_ResetMonth_Failed_5);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }
        CL_Cmd_ResetMonthInfo ncb = new CL_Cmd_ResetMonthInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_ResetMonthInfo);
        ncb.MonthID = MonthID;
        NetServices.Instance.Send<CL_Cmd_ResetMonthInfo>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_LoadRoleSignUpInfo:
                return HandleRoleSignUpInfo(obj);
            case NetCmdType.CMD_LC_SignUpMonthSucess:
                return HandleSignUpSucess(obj);
            case NetCmdType.CMD_LC_SignUpMonthFiled:
                return HandleSignUpFiled(obj);
            case NetCmdType.CMD_LC_JoinMonthSucess:
                return HandleJoinMonthSucess(obj);
            case NetCmdType.CMD_LC_JoinMonthFiled:
                return HandleJoinMonthFiled(obj);
            case NetCmdType.CMD_LC_ChangeUserAddMonthGlobelNum:
                return HandleChangeUserAddMonthGlobelNum(obj);
            case NetCmdType.CMD_LC_UserChangeIndex:
                return HandleRoleMonthIndexChange(obj);
            case NetCmdType.CMD_LC_UserMonthEnd:
                return HandleMonthEnd(obj);
            case NetCmdType.CMD_LC_GetMonthRoleSum:
                return HandleMonthPlayerSum(obj);
            //case NetCmdType.CMD_LC_GetMonthRoleSumFinish:
            //    return HandleMonthPlayerSumFinish(obj);
            case NetCmdType.CMD_LC_ResetMonthInfo:
                return HandleResetRoleMonth(obj);
            case NetCmdType.CMD_LC_SetRoleMonthInfo:
                return HandleSetRoleMonthInfo(obj);
            case NetCmdType.CMD_LC_ResetRoleMonthInfo:
                return HandleResetRoleMonthInfo(obj);
            case NetCmdType.CMD_LC_MonthList:
                return HandleMonthList(obj);
        }
        return true;
    }
    private bool HandleRoleSignUpInfo(NetCmdBase obj)
    {
        //加载玩家的全部的报名信息
        LC_Cmd_LoadRoleSignUpInfo ncb = (LC_Cmd_LoadRoleSignUpInfo)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            SignUpList.Clear();
        }
        for (int i = 0; i < ncb.Sum; ++i)
        {
            SignUpList.Add(ncb.Array[i], 1);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadInfo = true;

            tagMonthSignUpChangeEvent pEvnet = new tagMonthSignUpChangeEvent();
            MsgEventHandle.HandleMsg(pEvnet);
        }
        //UIWidgetManager.Instance.SetContestSetContestPeopleNumber();
        return true;
    }//
    private bool HandleSignUpSucess(NetCmdBase obj)
    {
        LC_Cmd_SignUpMonthSucess ncb = (LC_Cmd_SignUpMonthSucess)obj;
        //玩家报名成功了
        SignUpList.Add(ncb.MonthID, 1);

        PlayerRole.Instance.RoleGameData.OnHandleRoleSignUpMonth();

        tagMonthSignUpOnceChangeEvent pEvnet = new tagMonthSignUpOnceChangeEvent(ncb.MonthID);
        MsgEventHandle.HandleMsg(pEvnet);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Sucess);
        MsgEventHandle.HandleMsg(pUOM);
       
        //UIWidgetManager.Instance.SetContestSetContestPeopleNumber();
        return true;
    }
    private bool HandleSignUpFiled(NetCmdBase obj)
    {
        LC_Cmd_SignUpMonthFiled ncb = (LC_Cmd_SignUpMonthFiled)obj;

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_SignUpMonth_Failed_7);
        MsgEventHandle.HandleMsg(pUOM);

        //
        if (HallRunTimeInfo.Instance_UI != null && HallRunTimeInfo.Instance_UI.m_loginUi != null)
            HallRunTimeInfo.Instance_UI.m_loginUi.UpdateMatchData();

        return true;
    }
    private bool HandleJoinMonthSucess(NetCmdBase obj)
    {
        LC_Cmd_JoinMonthSucess ncb = (LC_Cmd_JoinMonthSucess)obj;
        //if (FishClientData.Instance.RoleMonthInfo.ContainsKey(ncb.MonthInfo.dwUserID))
        //{
        //    FishClientData.Instance.RoleMonthInfo[ncb.MonthInfo.dwUserID] = ncb.MonthInfo;
        //}
        //else
        //    FishClientData.Instance.RoleMonthInfo.Add(ncb.MonthInfo.dwUserID, ncb.MonthInfo);
        if(ncb.MonthInfo.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthInfo(ncb.MonthInfo);

            PlayerRole.Instance.RoleGameData.SetRoleJoinMonth();//玩家进入比赛

            tagMonthAddEvent pEvent = new tagMonthAddEvent(ncb.MonthInfo.dwUserID);
            MsgEventHandle.HandleMsg(pEvent);

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_JoinMonth_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            TableRole pRole = PlayerRole.Instance.TableManager.GetTableRole(ncb.MonthInfo.dwUserID);
            if(pRole != null)
            {
                pRole.SetMonthInfo(ncb.MonthInfo);

                tagMonthAddEvent pEvent = new tagMonthAddEvent(ncb.MonthInfo.dwUserID);
                MsgEventHandle.HandleMsg(pEvent);
            }
        }

        return true;
    }
    private bool HandleJoinMonthFiled(NetCmdBase obj)
    {
        LC_Cmd_JoinMonthFiled ncb = (LC_Cmd_JoinMonthFiled)obj;

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_JoinMonth_Failed_5);
        MsgEventHandle.HandleMsg(pUOM);

        if (HallRunTimeInfo.Instance_UI != null && HallRunTimeInfo.Instance_UI.m_loginUi != null)
            HallRunTimeInfo.Instance_UI.m_loginUi.UpdateMatchData();

        return true;
    }
    private bool HandleChangeUserAddMonthGlobelNum(NetCmdBase obj) //玩家续币成功的时候
    {
        LC_Cmd_ChangeUserAddMonthGlobelNum ncb = (LC_Cmd_ChangeUserAddMonthGlobelNum)obj;
        //桌子上一个玩家进行续币了 修改数据
        if (ncb.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            //if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() <= FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].m_ConvertMap[(Byte)(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthAddGlobelNum() + 1)].LostGlobel)
            //    PlayerRole.Instance.RoleInfo.RoleMe.SetGlobel(0);
            //else
            //    PlayerRole.Instance.RoleInfo.RoleMe.SetGlobel(PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() - FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].m_ConvertMap[(Byte)(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthAddGlobelNum() + 1)].LostGlobel);
            if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()))
                return true;
            if (!FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].m_ConvertMap.ContainsKey((Byte)(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthAddGlobelNum() + 1)))
                return true;
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthGlobel(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthGlobel() + FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].m_ConvertMap[(Byte)(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthAddGlobelNum() + 1)].AddMonthGlobel);
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthAddGlobelNum(Convert.ToByte(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthAddGlobelNum() + 1));

            tagRoleChangeEvent pRoleEvent = new tagRoleChangeEvent();
            MsgEventHandle.HandleMsg(pRoleEvent);

            tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.dwUserID);
            MsgEventHandle.HandleMsg(pEvent);

            tagMatchContiueGoldEvent pContinueEvent = new tagMatchContiueGoldEvent();
            MsgEventHandle.HandleMsg(pContinueEvent);

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Month_AddMonthGlobel_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        else
        {
            TableRole pRole = PlayerRole.Instance.TableManager.GetTableRole(ncb.dwUserID);
            if (pRole != null)
            {
                //if (pRole.GetGlobel() <= FishConfig.Instance.m_MatchInfo.m_MonthMap[pRole.GetMonthID()].m_ConvertMap[(Byte)(pRole.GetMonthAddGlobelNum() + 1)].LostGlobel)
                //    pRole.SetGlobel(0);
                //else
                //    pRole.SetGlobel(pRole.GetGlobel() - FishConfig.Instance.m_MatchInfo.m_MonthMap[pRole.GetMonthID()].m_ConvertMap[(Byte)(pRole.GetMonthAddGlobelNum() + 1)].LostGlobel);
                if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.ContainsKey(pRole.GetMonthID()))
                    return true;
                if (!FishConfig.Instance.m_MatchInfo.m_MonthMap[PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()].m_ConvertMap.ContainsKey((Byte)(pRole.GetMonthAddGlobelNum() + 1)))
                    return true;
                pRole.SetMonthGlobel(pRole.GetMonthGlobel() + FishConfig.Instance.m_MatchInfo.m_MonthMap[pRole.GetMonthID()].m_ConvertMap[(Byte)(pRole.GetMonthAddGlobelNum() + 1)].AddMonthGlobel);
                pRole.SetMonthAddGlobelNum(Convert.ToByte(pRole.GetMonthAddGlobelNum() +1));

                tagRoleChangeEvent pRoleEvent = new tagRoleChangeEvent();
                MsgEventHandle.HandleMsg(pRoleEvent);

                tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.dwUserID);
                MsgEventHandle.HandleMsg(pEvent);
            }
        }
        return true;
    }
    private bool HandleRoleMonthIndexChange(NetCmdBase obj)
    {
        //玩家排名变化了
        LC_Cmd_UserChangeIndex ncb = (LC_Cmd_UserChangeIndex)obj;
        if (ncb.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthUpperSocre(ncb.dwUpperSocre);
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthIndex(ncb.dwIndex);

            tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.dwUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            TableRole pRole = PlayerRole.Instance.TableManager.GetTableRole(ncb.dwUserID);
            if (pRole != null)
            {
                pRole.SetMonthUpperSocre(ncb.dwUpperSocre);
                pRole.SetMonthIndex(ncb.dwIndex);

                tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.dwUserID);
                MsgEventHandle.HandleMsg(pEvent);
            }
        }
        return true;
    }
    private bool HandleMonthEnd(NetCmdBase obj)
    {
        //玩家报名的一场比赛结束了 我们将报名信息关闭掉
        LC_Cmd_UserMonthEnd ncb = (LC_Cmd_UserMonthEnd)obj;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() == ncb.MonthID)
        {
            if (SceneRuntime.Instance != null)
                SceneRuntime.SceneLogic.BackToHall();
            PlayerRole.Instance.OnUserLeaveTable();//玩家离开桌子的触发函数
        }

        //因为一场比赛结束了 我们想要移除自己的报名信息 
        if (SignUpList.ContainsKey(ncb.MonthID))//移除比赛数据
        {
            SignUpList.Remove(ncb.MonthID);
            tagMonthSignUpChangeEvent pEvnet = new tagMonthSignUpChangeEvent();
            MsgEventHandle.HandleMsg(pEvnet);
        }

        PlayerRole.Instance.RoleGameData.OnHandleRoleMonthReward(Convert.ToUInt32(ncb.MonthIndex));

        tagMonthEndEvent pEvent = new tagMonthEndEvent(ncb.MonthID, ncb.MonthIndex,ncb.MonthScores,ncb.VipScores);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    private bool HandleMonthPlayerSum(NetCmdBase obj)
    {
        LC_Cmd_GetMonthRoleSum ncb = (LC_Cmd_GetMonthRoleSum)obj;
        if ((ncb.States & FishDataInfo.MsgBegin)!= 0)
        {
            MonthPlayerSum.Clear();
        }
        for (UInt16 i = 0; i < ncb.Sum; ++i)
        {
            //将数据存储起来
            if (MonthPlayerSum.ContainsKey(ncb.Array[i].MonthID))
                MonthPlayerSum[ncb.Array[i].MonthID] = ncb.Array[i].MonthUserSum;
            else
                MonthPlayerSum.Add(ncb.Array[i].MonthID, ncb.Array[i].MonthUserSum);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            tagMonthSignUpSumChangeEvent pEvent = new tagMonthSignUpSumChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //private bool HandleMonthPlayerSumFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_GetMonthRoleSumFinish ncb = (LC_Cmd_GetMonthRoleSumFinish)obj;

    //    //IsLoadMonthPlayerSum = true;

    //    tagMonthSignUpSumChangeEvent pEvent = new tagMonthSignUpSumChangeEvent();
    //    MsgEventHandle.HandleMsg(pEvent);

    //    return true;
    //}
    private bool HandleSetRoleMonthInfo(NetCmdBase obj)
    {
        LC_Cmd_SetRoleMonthInfo ncb = (LC_Cmd_SetRoleMonthInfo)obj;
        if (ncb.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {

            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthGlobel(Convert.ToUInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthGlobel() + ncb.MonthGlobel));
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthScore(Convert.ToUInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthScore() + ncb.MonthSocre));

            tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.dwUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            TableRole pRole = PlayerRole.Instance.TableManager.GetTableRole(ncb.dwUserID);
            if (pRole != null)
            {
                pRole.SetMonthGlobel(Convert.ToUInt32(pRole.GetMonthGlobel() + ncb.MonthGlobel));
                pRole.SetMonthScore(Convert.ToUInt32(pRole.GetMonthScore() + ncb.MonthSocre));

                tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.dwUserID);
                MsgEventHandle.HandleMsg(pEvent);
            }
        }
        return true;
    }
    private bool HandleResetRoleMonthInfo(NetCmdBase obj)
    {
        LC_Cmd_ResetRoleMonthInfo ncb = (LC_Cmd_ResetRoleMonthInfo)obj;
        if (ncb.MonthInfo.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthInfo(ncb.MonthInfo);

            tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.MonthInfo.dwUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            TableRole pRole = PlayerRole.Instance.TableManager.GetTableRole(ncb.MonthInfo.dwUserID);
            if (pRole != null)
            {
                pRole.SetMonthInfo(ncb.MonthInfo);

                tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.MonthInfo.dwUserID);
                MsgEventHandle.HandleMsg(pEvent);
            }
        }
        return true;
    }
    private bool HandleResetRoleMonth(NetCmdBase obj)
    {
        LC_Cmd_ResetMonthInfo ncb = (LC_Cmd_ResetMonthInfo)obj;
        if (ncb.dwUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Month_ResetMonth_Sucess : UserOperateMessage.UOM_Month_ResetMonth_Failed_6));
            MsgEventHandle.HandleMsg(pUOM);

            if (ncb.MonthID != PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() || !ncb.Result)
                return true;

            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthGlobel(FishConfig.Instance.m_MatchInfo.m_MonthMap[ncb.MonthID].InitMonthGlobel);
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthScore(0);
            PlayerRole.Instance.RoleInfo.RoleMe.SetMonthAddGlobelNum(0);

            tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.dwUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        else
        {
            if (ncb.MonthID != PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID()  || !ncb.Result)
                return true;

            TableRole pRole = PlayerRole.Instance.TableManager.GetTableRole(ncb.dwUserID);
            if (pRole != null)
            {
                pRole.SetMonthGlobel(FishConfig.Instance.m_MatchInfo.m_MonthMap[ncb.MonthID].InitMonthGlobel);
                pRole.SetMonthScore(0);
                pRole.SetMonthAddGlobelNum(0);

                tagMonthChangeEvent pEvent = new tagMonthChangeEvent(ncb.dwUserID);
                MsgEventHandle.HandleMsg(pEvent);
            }
        }
        return true;
    }
    private bool HandleMonthList(NetCmdBase obj)
    {
        LC_Cmd_MonthList ncb = (LC_Cmd_MonthList)obj;
        if (ncb.MonthID != PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID())
            return false;
        //处理比赛的列表
        m_MonthInfo = ncb.MonthInfo;
        //触发事件
        tagMatchListChange pEvent = new tagMatchListChange();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    //比赛的一些处理函数
    public bool MonthIsInSignUp(Byte Month)//是否在报名
    {
        tagMonthConfig MonthConfig;
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.TryGetValue(Month, out MonthConfig))
            return false;
        return (MonthConfig.NowIsInSignTime() !=0);
    }
    public bool MonthIsInStar(Byte Month)//是否在比赛
    {
        tagMonthConfig MonthConfig;
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.TryGetValue(Month, out MonthConfig))
            return false;
        return (MonthConfig.NowIsInStartTime() != 0);
    }
    public bool MonthIsBeginStar(Byte Month)
    {
        //判断当前比赛是否正好开始
        tagMonthConfig MonthConfig;
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.TryGetValue(Month, out MonthConfig))
            return false;
        return MonthConfig.NowIsBeginStar();
    }
    public bool MonthIsStop(Byte Month)//是否已经结束 等待下一次
    {
        tagMonthConfig MonthConfig;
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.TryGetValue(Month, out MonthConfig))
            return false;
        return (MonthConfig.NowIsInSignTime() == 0) && (MonthConfig.NowIsInStartTime() == 0);
    }
    public bool GetMonthNowOrNextSignTime(Byte Month, out DateTime DateBeginTime, out DateTime DateEndTime)
    {
        //获取最近的一次的报名时间 如果当前处在报名节点 返回当前正在进行的时间段用于显示
        //获取下次报名的开始时间 已经持续的分钟数
        tagMonthConfig MonthConfig;
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.TryGetValue(Month, out MonthConfig))
        {
            DateBeginTime = new DateTime();
            DateEndTime = new DateTime();
            return false;
        }
        Byte TimeID= MonthConfig.NowIsInSignTime();
        if (TimeID != 0)
        {
            //获取当前的
            tagMonthTime pTimeInfo;
            if (!MonthConfig.m_MonthTime.TryGetValue(TimeID, out pTimeInfo))
            {
                DateBeginTime = new DateTime();
                DateEndTime = new DateTime();
                return false;
            }
            DateBeginTime = pTimeInfo.GetNowOrNextSignTime();
            DateEndTime = DateBeginTime.AddMinutes(pTimeInfo.SignLastMin);
            return true;
        }
        else
        {
            //获取最近的
            //获取当前比赛 最近的一个 比赛范围
            DateTime LogTime = new DateTime();
            UInt16 LastMin = 0;
            foreach (tagMonthTime var in MonthConfig.m_MonthTime.Values)
            {
                if(LastMin == 0)
                {
                    LogTime = var.GetNowOrNextSignTime();
                    LastMin = var.SignLastMin;
                    continue;
                }
                DateTime SignTime= var.GetNowOrNextSignTime();
                if((SignTime - LogTime).TotalSeconds > 0)
                {
                    LogTime = SignTime;
                    LastMin = var.SignLastMin;
                    continue;
                }
            }
            DateBeginTime = LogTime;
            DateEndTime = LogTime.AddMinutes(LastMin);
            return true;
        }
    }
    public bool GetMonthNowOrNextStarTime(Byte Month, out DateTime DateBeginTime, out DateTime DateEndTime)
    {
        //获取最近的一次的报名时间 如果当前处在报名节点 返回当前正在进行的时间段用于显示
        //获取下次报名的开始时间 已经持续的分钟数
        tagMonthConfig MonthConfig;
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.TryGetValue(Month, out MonthConfig))
        {
            DateBeginTime = new DateTime();
            DateEndTime = new DateTime();
            return false;
        }
        Byte TimeID = MonthConfig.NowIsInStartTime();
        if (TimeID != 0)
        {
            //获取当前的
            tagMonthTime pTimeInfo;
            if (!MonthConfig.m_MonthTime.TryGetValue(TimeID, out pTimeInfo))
            {
                DateBeginTime = new DateTime();
                DateEndTime = new DateTime();
                return false;
            }
            DateBeginTime = pTimeInfo.GetNowOrNextStarTime();
            DateEndTime = DateBeginTime.AddMinutes(pTimeInfo.SratLastMin);
            return true;
        }
        else
        {
            //获取最近的
            //获取当前比赛 最近的一个 比赛范围
            DateTime LogTime = new DateTime();
            UInt16 LastMin = 0;
            foreach (tagMonthTime var in MonthConfig.m_MonthTime.Values)
            {
                if (LastMin == 0)
                {
                    LogTime = var.GetNowOrNextStarTime();
                    LastMin = var.SratLastMin;
                    continue;
                }
                DateTime StarTime = var.GetNowOrNextStarTime();
                if ((StarTime - LogTime).TotalSeconds > 0)
                {
                    LogTime = StarTime;
                    LastMin = var.SratLastMin;
                    continue;
                }
            }
            DateBeginTime = LogTime;
            DateEndTime = LogTime.AddMinutes(LastMin);
            return true;
        }
    }
    public DateTime GetMonthGameEndTime(Byte MonthID)
    {
        tagMonthConfig MonthConfig;
        if (!FishConfig.Instance.m_MatchInfo.m_MonthMap.TryGetValue(MonthID, out MonthConfig))
        {
            DateTime pTime = new DateTime();
            return pTime;
        }
        Byte TimeID = MonthConfig.NowIsInStartTime();
        if (TimeID != 0)
        {
            //获取当前的
            tagMonthTime pTimeInfo;
            if (!MonthConfig.m_MonthTime.TryGetValue(TimeID, out pTimeInfo))
            {
                DateTime pTime = new DateTime();
                return pTime;
            }
            DateTime EndTime = pTimeInfo.GetNowOrNextStarTime().AddMinutes(pTimeInfo.SratLastMin);
            //EndTime = EndTime.AddMilliseconds(EndTime.Millisecond * -1).AddSeconds(EndTime.Second * -1);
            return FishConfig.Instance.GetMinDateTime(EndTime);
        }
        else
        {
            DateTime pTime = new DateTime();
            return pTime;
        }
    }
}