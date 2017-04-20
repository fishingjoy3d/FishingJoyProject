using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

//客户端的核心类 表示玩家自己  我们用于处理玩家的函数  封装服务器 以及对服务器发送的一些命令的封装处理
class PlayerRole : Singleton<PlayerRole>
{
    //private bool m_IsNeedResetTable = false;

    //public bool IsNeedResetTable
    //{
    //    set { m_IsNeedResetTable = value; }
    //}

    //public void CheckIsResetTableInfo()
    //{
    //    if(m_IsNeedResetTable)
    //    {
    //        m_IsNeedResetTable = false;
    //        PlayerRole.Instance.RoleInfo.ResetSceneInfo();
    //    }
    //}


    public PlayerRole()
    {
        m_NetMsgHandle = new ClientMsgHandle(this);
        NetServices.Instance.FinalHandler = m_NetMsgHandle;
    }
    public void OnInit()
    {
        //进行初始化操作
        OnClearTempFile();
    }
    public void OnClearTempFile()
    {
        //RankXml
        DateTime time = SystemTime.Instance.GetSystemDateTime;
        UInt32 WriteSec = FishConfig.Instance.GetWriteSec();
        time = time.AddSeconds(WriteSec * -1);
        string NowDayRankFileName = string.Format("Rank_{0}_{1}_{2}.xml", time.Year, time.Month, time.Day);
        //MonthXml
        //比赛的排行榜文件 更加复杂 删除一个星期以前的文件 取日期 判断时间
        //1.排行榜文件
        string[] FileList =RuntimeInfo.GetLocalFiles();
        for(int i =0;i<FileList.Length;++i)
        {
            string FileName = FileList[i];
            if (FileName.Length == 0 || FileName == "")
                continue;
            //判断文件的类型
            //1.每天排行榜文件 Rank_xxx_xxx
            if(FileName.IndexOf("Rank_") != -1  && FileName.IndexOf(".xml") != -1)
            {
                if(FileName != NowDayRankFileName)
                {
                    try
                    {
                        File.Delete(FileName);
                    }
                    catch { }
                    //RuntimeInfo.DeleteLocalFile(FileName);//将文件移除掉
                    continue;
                }
                else
                    continue;
            }
            //2.周排行榜文件
            if(FileName.IndexOf("Month_") != -1 && FileName.IndexOf(".xml") != -1)
            {
                string strName = FileName.Substring(0, FileName.IndexOf(".xml"));
               string[] Info = strName.Split('_');
                if(Info.Length != 7)
                    continue;
                
                //Convert.ToInt32(Info[1]),
                DateTime data = new DateTime(Convert.ToInt32(Info[2]),Convert.ToInt32(Info[3]),
                    Convert.ToInt32(Info[4]),Convert.ToInt32(Info[5]),Convert.ToInt32(Info[6]),0);
                TimeSpan span = SystemTime.Instance.GetSystemDateTime -data ;
                if(span.TotalSeconds >= 7*24*3600)//超过一个星期了
                {
                    try
                    {
                        File.Delete(FileName);
                    }
                    catch { }
                     //RuntimeInfo.DeleteLocalFile(FileName);//将文件移除掉
                    continue;
                }
            }
        }
    }
    //核心管理类 管理器 提供 Send函数 封装Handle函数 提供对外的事件接口函数
    //消息处理类
    private ClientMsgHandle m_NetMsgHandle = null;
    //0.玩家自身管理器 
    private RoleInfo m_RoleInfo = new RoleInfo();
    internal RoleInfo RoleInfo
    {
        get { return m_RoleInfo; }
    }
    //1.物品管理器
    private RoleItem m_ItemManager = new RoleItem();
    internal RoleItem ItemManager
    {
        get { return m_ItemManager; }
    }
    //2.关系管理器
    private RoleRelation m_RelationManager = new RoleRelation();
    internal RoleRelation RelationManager
    {
        get { return m_RelationManager; }
    }
    //3.邮件管理器
    private RoleMail m_MailManager = new RoleMail();
    internal RoleMail MailManager
    {
      get { return m_MailManager; }
    }
    //4.任务管理器
    private RoleTaskManager m_TaskManager = new RoleTaskManager();
    internal RoleTaskManager TaskManager
    {
        get { return m_TaskManager; }
    }
    //5.签到管理器
    private RoleCheck m_CheckManager = new RoleCheck();
    internal RoleCheck CheckManager
    {
        get { return m_CheckManager; }
    }
    //6.成就管理器
    private RoleAchievementManager m_AchievementManager = new RoleAchievementManager();
    internal RoleAchievementManager AchievementManager
    {
        get { return m_AchievementManager; }
    }
    //7.活动管理器
    private RoleActionManager m_ActionManager = new RoleActionManager();
    internal RoleActionManager ActionManager
    {
        get { return m_ActionManager; }
    }
    //8.比赛管理器
    private RoleMonth m_MonthManager = new RoleMonth();
    internal RoleMonth MonthManager
    {
        get { return m_MonthManager; }
    }
    //9.实体数据管理器
    private RoleEntity m_EntityManager = new RoleEntity();
    internal RoleEntity EntityManager
    {
        get { return m_EntityManager; }
    }
    //10.赠送管理器
    private RoleGiff m_GiffManager = new RoleGiff();
    internal RoleGiff GiffManager
    {
        get { return m_GiffManager; }
    }
    //11.魅力管理器
    private RoleCharm m_CharmManager = new RoleCharm();
    internal RoleCharm CharmManager
    {
        get { return m_CharmManager; }
    }
    //12.排行榜管理器
    private RoleRank m_RankManager = new RoleRank();
    internal RoleRank RankManager
    {
        get { return m_RankManager; }
    }
    //13.查询管理器
    private RoleQuery m_QueryManager = new RoleQuery();
    internal RoleQuery QueryManager
    {
        get { return m_QueryManager; }
    }
    //14.事件管理器
    //private EventHandleBase m_EventManager = new EventHandleBase();
    //internal EventHandleBase EventManager
    //{
    //    get { return m_EventManager; }
    //}
    //15.桌子管理器
    private RoleTable m_TableManager = new RoleTable();
    internal RoleTable TableManager
    {
        get { return m_TableManager; }
    }
    //16.称号管理器
    private RoleTitle m_TitleManager = new RoleTitle();
    internal RoleTitle TitleManager
    {
        get { return m_TitleManager; }
    }
    //17.宝箱管理器
    private RoleChest m_ChestManager = new RoleChest();
    internal RoleChest ChestManager
    {
        get { return m_ChestManager; }
    }
    //18.商店管理器
    private RoleShop m_ShopManager = new RoleShop();
    internal RoleShop ShopManager
    {
        get { return m_ShopManager; }
    }
    //20 在线奖励管理器
    private RoleOnlineReward m_RoleOnlineReward = new RoleOnlineReward();
    internal RoleOnlineReward RoleOnlineReward
    {
        get { return m_RoleOnlineReward; }
    }
    //21 Gamedata
    private RoleGameData m_RoleGameData = new RoleGameData();
    internal RoleGameData RoleGameData
    {
        get { return m_RoleGameData; }
    }
    //22.Package
    private RolePackage m_RolePackage = new RolePackage();
    internal RolePackage RolePackage
    {
        get { return m_RolePackage; }
    }
    //23.炮台管理器
    private RoleLauncher m_RoleLauncher = new RoleLauncher();
    internal RoleLauncher RoleLauncher
    {
        get { return m_RoleLauncher; }
    }
    //24.消息显示
    private RoleMessage m_RoleMessage = new RoleMessage();
    internal RoleMessage RoleMessage
    {
        get { return m_RoleMessage; }
    }
    //25
    private RoleRecharge m_RoleRecharge = new RoleRecharge();
    internal RoleRecharge RoleRecharge
    {
        get { return m_RoleRecharge; }
    }
    //26
    private RoleAnnouncement m_RoleAnnouncement = new RoleAnnouncement();
    internal RoleAnnouncement RoleAnnouncement
    {
        get { return m_RoleAnnouncement; }
    }
    //27
    private RoleOperate m_RoleOperate = new RoleOperate();
    internal RoleOperate RoleOperate
    {
        get { return m_RoleOperate; }
    }

    //28
    private RoleStatesMessage m_RoleStatesMessage = new RoleStatesMessage();
    internal RoleStatesMessage RoleStatesMessage
    {
        get { return m_RoleStatesMessage; }
    }
    //29.ExChange
    private RoleExChange m_RoleExChange = new RoleExChange();
    internal RoleExChange RoleExChange
    {
        get { return m_RoleExChange; }
    }
    //30.Lottery
    private RoleLottery m_RoleLottery = new RoleLottery();
    internal RoleLottery RoleLottery
    {
        get { return m_RoleLottery; }
    }
    //31.Vip
    private RoleVip m_RoleVip = new RoleVip();
    internal RoleVip RoleVip
    {
        get { return m_RoleVip; }
    }
    //32.MonthCard
    private RoleMonthCard m_RoleMonthCard = new RoleMonthCard();
    internal RoleMonthCard RoleMonthCard
    {
        get { return m_RoleMonthCard; }
    }
    //33.MiniGame
    private RoleMiNiGame m_RoleMiNiGame = new RoleMiNiGame();
    internal RoleMiNiGame RoleMiNiGame
    {
        get { return m_RoleMiNiGame; }
    }
    //RoleChar
    private RoleChar m_RoleChar = new RoleChar();
    internal RoleChar RoleChar
    {
        get { return m_RoleChar; }
    }

    private RoleRelationRequest m_RoleRelationRequest = new RoleRelationRequest();
    internal RoleRelationRequest RoleRelationRequest
    {
        get { return m_RoleRelationRequest; }
    }
    

    //PlayerRole 上非管理器的一些函数
    //public void UpdateByMin()
    //{
    //    //if( LogPlayerRoleUpdate == 0 || Utility.GetTickCount() - LogPlayerRoleUpdate >= 60000)//一分钟更新一次  需要进行处理
    //    //{
    //    //    LogPlayerRoleUpdate = Utility.GetTickCount();

    //    //    tagOnlineRewardChangeEvent pEventOnline = new tagOnlineRewardChangeEvent();
    //    //    MsgEventHandle.HandleMsg(pEventOnline);
    //    //}
    //}
    public void OnUserLeaveTable()
    {
        //玩家离开房间
        RoleInfo.RoleMe.SetSeat(0xff);//玩家离开桌子 位置清空
        m_TableManager.OnClear();
        m_ChestManager.OnClear();
        m_MonthManager.OnLeaveTable();
      
       // m_MonthManager.OnClear();
        RoleInfo.RoleMe.SetMonthInfo(null);//设置比赛数据未空
    }
    public void OnUserLogout()
    {
        //玩家登出
        m_RoleInfo.OnClear();//玩家自己在离开游戏的时候清空下
        m_ItemManager.OnClear();//玩家离开游戏的时候 物品清空
        m_RelationManager.OnClear();//关系在离开游戏的时候 清空下
        m_MailManager.OnClear();
        m_TaskManager.OnClear();
        m_AchievementManager.OnClear();
        m_ActionManager.OnClear();
        m_EntityManager.OnClear();
        m_GiffManager.OnClear();
        m_CharmManager.OnClear();
        m_QueryManager.OnClear();
        m_TableManager.OnClear();
        m_TitleManager.OnClear();
        m_ChestManager.OnClear();
        m_ShopManager.OnClear();
        m_MonthManager.OnClear();
        m_RankManager.OnClear();//玩家离开游戏的时候 排行榜数据清空 防止玩家离线 但是过12点 导致排行榜数据是旧的
        m_RoleOnlineReward.OnClear();
        m_RoleGameData.OnClear();
        m_RolePackage.OnClear();
        m_RoleLauncher.OnClear();
        m_RoleAnnouncement.OnClear();
        m_RoleOperate.OnClear();
        m_RoleStatesMessage.OnClear();
        m_RoleLottery.OnClear();
        RoleChar.OnClear();
        RoleRelationRequest.OnClear();
    }
    public void UpdateDayByServer() //按配置文件时间进行更新
    {
        m_RoleInfo.UpdateDayByServer();//玩家本事的每天数据清空 按服务器更新时间来处理
        //邮件更新
        //m_MailManager.UpdateByDay();
        //赠送更新
        m_GiffManager.OnDayChange();
        //排行榜更新
        m_RankManager.OnDayChange();//每天下载新的排行榜数据
        //OnClearTempFile();
    }
    public void UpdateRoleEventHandle()
    {
        //一些被动事件 需要刷新下
        PlayerRole.Instance.HandleEvent(EventTargetType.ET_MaxGlobelSum,0, RoleInfo.RoleMe.GetGlobel());
        PlayerRole.Instance.HandleEvent(EventTargetType.ET_ToLevel,0, RoleInfo.RoleMe.GetLevel());
        PlayerRole.Instance.HandleEvent(EventTargetType.ET_MaxCurren,0, RoleInfo.RoleMe.GetCurrency());
    }
    public void HandleLaunchBullet(Byte Seat, byte bulletType, byte rateIndex)
    {
        //计算需要扣除的金币的数量
        Seat = SceneRuntime.ClientToServerSeat(Seat);//客户端转服务器端的位置
    }
    public void HandleEvent(EventTargetType EventID,UInt32 BindParam,UInt32 Param)
    {
        TaskManager.OnHandleEvent(EventID, BindParam, Param);
        AchievementManager.OnHandleEvent(EventID, BindParam, Param);
        ActionManager.OnHandleEvent(EventID, BindParam, Param);
    }
    public void HandeCatchFishData(CatchedData cd)  //捕获鱼的结果
    {
        if (cd == null )//判断是否是自己
            return;
        int Exp = 0;
        if(SceneRuntime.ClientToServerSeat(cd.ClientSeat) == PlayerRole.Instance.RoleInfo.RoleMe.GetSeat())
        {
            if(cd.FishList.Count > 0)
            {
                for (int i = 0; i < cd.FishList.Count; ++i)
                {
                    //添加经验
                    Exp += FishSetting.FishDataList[cd.FishList[i].FishType].Gold;

                    if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() == 0 && PlayerRole.Instance.RoleInfo.RoleMe.GetSeat() != 0xff)//在桌子里 并且不在比赛之中
                        PlayerRole.Instance.RoleLottery.OnRoleCatchByLottery(cd.FishList[i].FishType,cd);

                    if(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() == 0)
                        HandleEvent(EventTargetType.ET_CatchFish, cd.FishList[i].FishType, 1);

                    if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() == 0 && PlayerRole.Instance.RoleInfo.RoleMe.GetSeat() != 0xff)//在桌子里 并且不在比赛之中
                        RoleGameData.OnHandleCatchFish(cd.FishList[i].FishType);//捕获指定的鱼
                }
            }
            if (cd.CatchType == (Byte)CatchedType.CATCHED_BULLET || cd.CatchType == (Byte)CatchedType.CATCHED_LASER)//技能不算开炮获得金币
                if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() == 0)
                    HandleEvent(EventTargetType.ET_LauncherGlobel, Convert.ToUInt32(cd.GoldNum), 1);//一炮获得多少金币
            if (cd.CatchType == (Byte)CatchedType.CATCHED_SKILL)
            {
                HandleEvent(EventTargetType.ET_UseSkill, cd.SubType, 1);
            }
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() == 0 && PlayerRole.Instance.RoleInfo.RoleMe.GetSeat() != 0xff)//在桌子里 并且不在比赛之中
        {
            PlayerRole.Instance.OnAddUserExp(Exp);
        }
        //根据位置 获得桌子上玩家的对象 
        //Byte Seat = SceneRuntime.ClientToServerSeat(cd.ClientSeat);
        //bool IsInMonth = (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() != 0);
        ////如果在战场的话 进行处理
        //RoleBase pRole = null;
        //if (PlayerRole.Instance.RoleInfo.RoleMe.GetSeat() == Seat)
        //    pRole = PlayerRole.Instance.RoleInfo.RoleMe;
        //else
        //{
        //    pRole = PlayerRole.Instance.TableManager.GetTableRole(Seat);
        //}
        //if (pRole == null)
        //    return;
        //添加金币 
        //if (IsInMonth)
        //{
        //    pRole.SetMonthScore((UInt32)(pRole.GetMonthScore() + cd.GoldNum));

        //    tagMonthChangeEvent pEvent = new tagMonthChangeEvent(pRole.GetUserID());
        //    MsgEventHandle.HandleMsg(pEvent);
        //}
        //else
        //{
        //    pRole.SetGlobel((UInt32)(pRole.GetGlobel() + cd.GoldNum));

        //    if(pRole == PlayerRole.Instance.RoleInfo.RoleMe)
        //    {
        //        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        //        MsgEventHandle.HandleMsg(pEvent);
        //    }
        //    else
        //    {
        //        tagTableChangeEvent pEvent = new tagTableChangeEvent(pRole.GetUserID());
        //        MsgEventHandle.HandleMsg(pEvent);
        //    }
        //}
    }
    public UInt32 GetPlayerSocreBySeat(Byte Seat)
    {
        Seat = SceneRuntime.ClientToServerSeat(Seat);
        //或者指定位置玩家的积分 
        bool IsInMonth = (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() != 0);
        if (!IsInMonth)
            return 0;
        RoleBase pRole = null;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetSeat() == Seat)
            pRole = PlayerRole.Instance.RoleInfo.RoleMe;
        else
        {
            pRole = PlayerRole.Instance.TableManager.GetTableRole(Seat);
        }
        if (pRole == null)
            return 0;
        return pRole.GetMonthScore();
    }
    public UInt32 GetPlayerGlobelBySeat(Byte Seat)
    {
        Seat = SceneRuntime.ClientToServerSeat(Seat);
        RoleBase pRole = null;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetSeat() == Seat)
            pRole = PlayerRole.Instance.RoleInfo.RoleMe;
        else
        {
            pRole = PlayerRole.Instance.TableManager.GetTableRole(Seat);
        }
        if (pRole == null)
            return 0;
        bool IsInMonth = (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() != 0);
        if(IsInMonth)
        {
            return pRole.GetMonthGlobel();
        }
        else
        {
            return pRole.GetGlobel();
        }
    }
    public void OnRoleResetOtherInfo()//在玩家进行刷新 或者是 断线重新连接后 进行的处理
    {
        //客户端重新设置下全部的外围数据
        AchievementManager.ResetInfo();
        ActionManager.ResetInfo();
        //CheckManager.ResetInfo();
        EntityManager.ResetInfo();
        RoleGameData.ResetInfo();
        ItemManager.ResetInfo();
        MailManager.ResetInfo();
        RankManager.ResetInfo();
        RelationManager.ResetInfo();
        TaskManager.ResetInfo();
        RoleAnnouncement.ResetInfo();
        MonthManager.ResetInfo();
        RoleChar.ResetInfo();
    }
    public void OnAddUserExp(int Exp)
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe == null || Exp== 0)
            return;
        PlayerRole.Instance.RoleInfo.RoleMe.SetExp(Convert.ToUInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetExp() + Exp));
        tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }
    public void SaveStringToFile(string str,string FilePath)
    {
        //将字符串 写入到指定文件里面去
        str = str + "\r\n";
        try
        {
            FileStream fs = new FileStream(FilePath, FileMode.Append);
            byte[] data = new UTF8Encoding().GetBytes(str);
            fs.Write(data, 0, data.Length);
            fs.Flush();
            fs.Close();
        }
       catch(Exception ex)
        {
            FileStream fs = new FileStream(FilePath, FileMode.OpenOrCreate);
            byte[] data = new UTF8Encoding().GetBytes(str);
            fs.Write(data, 0, data.Length);
            fs.Flush();
            fs.Close();
        }
    }
    public bool OnAddUserGlobelByCatchedData(Byte Seat,int  GlobelSum)//修改指定位置上玩家的金币 或者是积分 必须是 与鱼群相关的
    {
        //因为捕获鱼 添加金币
         Seat = SceneRuntime.ClientToServerSeat(Seat);
         bool IsInMonth = (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() != 0);
         if (PlayerRole.Instance.RoleInfo.RoleMe.GetSeat() == Seat)
         {
             if (PlayerRole.Instance.RoleInfo.RoleMe == null)
                 return false;
             if (IsInMonth)
             {
                 if (GlobelSum >= 0)
                     PlayerRole.Instance.RoleInfo.RoleMe.SetMonthScore(Convert.ToUInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthScore() + GlobelSum));
                 else
                 {
                     if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthGlobel() < GlobelSum * -1)
                         return false;
                     PlayerRole.Instance.RoleInfo.RoleMe.SetMonthGlobel(Convert.ToUInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthGlobel() + GlobelSum));
                 }
                 //tagMonthChangeEvent pEvent = new tagMonthChangeEvent(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
                 //MsgEventHandle.HandleMsg(pEvent);
                 tagMatchScoreAddEvent pEvent = new tagMatchScoreAddEvent(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
                 MsgEventHandle.HandleMsg(pEvent);
                 return true;
             }    
             else
             {
                 if (GlobelSum < 0 && PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < -1*GlobelSum)
                     return false;
                 PlayerRole.Instance.RoleInfo.RoleMe.SetGlobel(Convert.ToUInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() + GlobelSum));
                 tagRoleChangeEvent pEvent = new tagRoleChangeEvent();
                 MsgEventHandle.HandleMsg(pEvent);
                 return true;
             }
         }
         else
         {
             if (IsInMonth)
             {
                 TableRole pTableRole = PlayerRole.Instance.TableManager.GetTableRole(Seat);
                 if (pTableRole == null)
                     return false;
                 if (GlobelSum >= 0)
                     pTableRole.SetMonthScore(Convert.ToUInt32(pTableRole.GetMonthScore() + GlobelSum));
                 else
                 {
                     if (pTableRole.GetMonthGlobel() < GlobelSum * -1)
                         return false;
                     pTableRole.SetMonthGlobel(Convert.ToUInt32(pTableRole.GetMonthGlobel() + GlobelSum));
                 }
                 tagMonthChangeEvent pEvent = new tagMonthChangeEvent(pTableRole.GetUserID());
                 MsgEventHandle.HandleMsg(pEvent);
                 return true;
             } 
             else
             {
                 if (GlobelSum < 0 && PlayerRole.Instance.TableManager.GetTableRole(Seat).GetGlobel() < -1 * GlobelSum)
                     return false;
                 TableRole pTableRole = PlayerRole.Instance.TableManager.GetTableRole(Seat);
                 if (pTableRole == null)
                     return false;
                 pTableRole.SetGlobel(Convert.ToUInt32(pTableRole.GetGlobel() + GlobelSum));

                 tagTableChangeEvent pEvent = new tagTableChangeEvent(pTableRole.GetUserID());
                 MsgEventHandle.HandleMsg(pEvent);
                 return true;
             }     
         }
    }
}