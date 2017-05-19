using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家的桌子管理器  和  玩家比赛管理器相关 比赛数据在玩家数据的后面发送  
//1.接收玩家自身的比赛数据
//2.接收桌子远程玩家数据
//3.接收桌子远程玩家比赛数据
enum TableError
{
    TE_Sucess = 1,//可以进入
    TE_MaxCurrcey = 2,//乐币太多
    TE_MinCurrcey = 3,//乐币太少
    TE_MaxGlobel = 4,//金币太多
    TE_MinGlobel = 5,//金币太少
    TE_ItemError = 6,//缺少进入物品
    TE_IsNotExists = 7,//桌子不存在
    TE_RateError = 8,
    TE_MinLevel = 9,
    TE_MaxLevel = 10,
}

class RoleTable  //桌子管理器 包含了 比赛的数据 和 桌子的处理等操作
{
    //private bool m_IsIntable = false;
    private Dictionary<UInt32, TableRole> m_TableRole = new Dictionary<UInt32, TableRole>();//桌子上的玩家处理 一定是先获得玩家桌子数据 
    private Dictionary<Byte, TableRole> m_TableRoleSeat = new Dictionary<Byte, TableRole>();
    public RoleTable()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_TableRole.Clear();
        m_TableRoleSeat.Clear();
        //m_IsIntable = false;
    }
    public void ClearTableInfo()
    {
        m_TableRole.Clear();
        m_TableRoleSeat.Clear();
    }

    //提供公用函数供外部使用
    //public bool IsInTable()
    //{
    //    return m_IsIntable;//玩家是否在桌子里面
    //}
    //public bool JoinTable(Byte TableType)
    //{
    //    return SendJoinTable(TableType);
    //}
    //public void LeaveTable(bool IsSendMsg)
    //{
    //    if (!IsInTable())
    //        return;
    //    SendLeaveTable(IsSendMsg);
    //}
    public TableRole GetTableRole(UInt32 UserID)
    {
        if(m_TableRole.ContainsKey(UserID))
            return m_TableRole[UserID];
        return null;
    }
    public TableRole GetTableRole(Byte SeatID)
    {
        if (m_TableRoleSeat.ContainsKey(SeatID))
            return m_TableRoleSeat[SeatID];
        return null;
    }
    public Byte ConvertTableID(Byte TableType)
    {
        if(IsCanJoinTable(TableType,false) ==  TableError.TE_Sucess)
        {
            return TableType;
        }
        else
        {
            List<Byte> pTableList = FishConfig.Instance.m_FishScriptMap.GetTableGroupList(TableType);
            for(int i =0;i<pTableList.Count;++i)
            {
                if (IsCanJoinTable(pTableList[i], false) == TableError.TE_Sucess)
                    return pTableList[i];
            }   
            return TableType;
        }
    }
    public UInt32 GetTableErrorParam(Byte TableType,TableError Error)
    {
        if (!FishConfig.Instance.m_TableInfo.m_TableConfig.ContainsKey(TableType))
            return 0;

        switch(Error)
        {
            case TableError.TE_MinGlobel:
                {
                    return FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinGlobelSum;
                }
                break;
            case TableError.TE_MaxGlobel:
                {
                    return FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxGlobelSum;
                }
                break;
            case TableError.TE_MinCurrcey:
                {
                    return FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinCurreySum;
                }
                break;
            case TableError.TE_MaxCurrcey:
                {
                    return FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxCurreySum;
                }
                break;
            case TableError.TE_ItemError:
                {
                    if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Count > 0)
                    {
                        for (int i = 0; i < FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Count; ++i)
                        {
                            UInt32 ItemID = FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Keys.ElementAt<UInt32>(i);
                            UInt32 ItemSum = FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Values.ElementAt<UInt32>(i);
                            if (PlayerRole.Instance.ItemManager.GetItemSum(ItemID) < ItemSum)
                            {
                                return ItemID;
                            }
                        }
                    }
                }
                break;
            case TableError.TE_RateError:
                {
                    return FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinRate;//最低倍率不符合桌子的要求
                }
                break;
            case TableError.TE_MinLevel:
                {
                    return FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinLevel;
                }
                break;
            case TableError.TE_MaxLevel:
                {
                    return FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxLevel;
                }
                break;
        }

        return 0;
    }
    public TableError IsCanJoinTable(Byte TableType, bool IsShowErrorID)
    {
        if (!FishConfig.Instance.m_TableInfo.m_TableConfig.ContainsKey(TableType))
        {
            if (!IsShowErrorID)
                return TableError.TE_IsNotExists;
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Table_JoinTable_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);

            return TableError.TE_IsNotExists;
        }

        if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinLevel != 0xffffffff && PlayerRole.Instance.RoleInfo.RoleMe.GetLevel() < FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinLevel)
        {
            if (!IsShowErrorID)
                return TableError.TE_MinLevel;
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Table_JoinTable_Failed_9);
            MsgEventHandle.HandleMsg(pUOM);
            return TableError.TE_MinLevel;
        }
        if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxLevel != 0xffffffff && PlayerRole.Instance.RoleInfo.RoleMe.GetLevel() > FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxLevel)
        {
            if (!IsShowErrorID)
                return TableError.TE_MaxLevel;
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Table_JoinTable_Failed_10);
            MsgEventHandle.HandleMsg(pUOM);
            return TableError.TE_MaxLevel;
        }

        if (!int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetRateValue(), FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinRate))
        {
            if (!IsShowErrorID)
                return TableError.TE_RateError;
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Table_JoinTable_Failed_8);
            MsgEventHandle.HandleMsg(pUOM);
            return TableError.TE_RateError;
        }
        if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinGlobelSum != 0xffffffff && PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinGlobelSum)
        {
            if (!IsShowErrorID)
                return TableError.TE_MinGlobel;
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Table_JoinTable_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return TableError.TE_MinGlobel;
        }
        if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxGlobelSum != 0xffffffff && PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() > FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxGlobelSum)
        {
            if (!IsShowErrorID)
                return TableError.TE_MaxGlobel;
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Table_JoinTable_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return TableError.TE_MaxGlobel;
        }
        if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinCurreySum != 0xffffffff && PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinCurreySum)
        {
            if (!IsShowErrorID)
                return TableError.TE_MinCurrcey;
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Table_JoinTable_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return TableError.TE_MinCurrcey;
        }
        if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxCurreySum != 0xffffffff && PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() > FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxCurreySum)
        {
            if (!IsShowErrorID)
                return TableError.TE_MaxCurrcey;
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Table_JoinTable_Failed_5);
            MsgEventHandle.HandleMsg(pUOM);
            return TableError.TE_MaxCurrcey;
        }
        if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Count > 0)
        {
            for (int i = 0; i < FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Count; ++i)
            {
                UInt32 ItemID = FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Keys.ElementAt<UInt32>(i);
                UInt32 ItemSum = FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Values.ElementAt<UInt32>(i);
                if (PlayerRole.Instance.ItemManager.GetItemSum(ItemID) < ItemSum)
                {
                    if (!IsShowErrorID)
                        return TableError.TE_ItemError;
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Table_JoinTable_Failed_6);
                    MsgEventHandle.HandleMsg(pUOM);
                    return TableError.TE_ItemError;
                }
            }
        }
        return TableError.TE_Sucess;
    }

    //进行处理 桌子上的数据

    //private bool SendJoinTable(Byte TableType)
    //{
    //    //玩家进入一个房间判断
    //    if (!FishConfig.Instance.m_TableInfo.m_TableConfig.ContainsKey(TableType))
    //        return false;

    //    if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinGlobelSum != 0xff && PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinGlobelSum)
    //        return false;
    //    if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxGlobelSum != 0xff && PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() > FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxGlobelSum)
    //        return false;
    //    if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinCurreySum != 0xff && PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MinCurreySum)
    //        return false;
    //    if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxCurreySum != 0xff && PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() > FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].MaxCurreySum)
    //        return false;
    //    if (FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Count > 0)
    //    {
    //        for (int i = 0; i < FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Count; ++i)
    //        {
    //            UInt32 ItemID = FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Keys.ElementAt<UInt32>(i);
    //            UInt16 ItemSum = FishConfig.Instance.m_TableInfo.m_TableConfig[TableType].JoinItemMap.Values.ElementAt<UInt16>(i);
    //            if (PlayerRole.Instance.ItemManager.GetItemSum(ItemID) < ItemSum)
    //                return false;
    //        }
    //    }
    //    CL_Cmd_JoinTable ncb = new CL_Cmd_JoinTable();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_JoinTable);
    //    ncb.bTableType = TableType;
    //    NetServices.Instance.Send<CL_Cmd_JoinTable>(ncb);
    //    return true;
    //}
    //private void SendLeaveTable(bool IsSendMsg)
    //{
    //    if(IsSendMsg)
    //    {
    //        CL_Cmd_LeaveTable ncb = new CL_Cmd_LeaveTable();
    //        ncb.SetCmdType(NetCmdType.CMD_CL_LeaveTable);
    //        NetServices.Instance.Send<CL_Cmd_LeaveTable>(ncb);
    //    }
    //    OnClear();
    //    PlayerRole.Instance.OnRoleLeaveTable();//玩家离开桌子

    //    tagTableDelEvent pEvent = new tagTableDelEvent(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
    //    MsgEventHandle.HandleMsg(pEvent);
    //}
    public bool HandleCmd(NetCmdBase obj)
    {
        //处理命令
        switch (obj.GetCmdType())
        {
            //case NetCmdType.CMD_LC_JoinTable:
            //    return HandleJoinTableResult(obj);
            case NetCmdType.CMD_LC_OtherUserInfo:
                return HandleOtherUserInfo(obj);
            case NetCmdType.CMD_LC_UserLeave:
                return HandleOtherRoleLeave(obj);
            case NetCmdType.CMD_LC_MeSeatID:
                return HandleMeSeatID(obj);
            case NetCmdType.CMD_LC_TableChangeRoleLevel:
                return HandleOtherRoleLevelChange(obj);
            case NetCmdType.CMD_LC_TableChangeRoleGender:
                return HandleOtherRoleGenderChange(obj);
            case NetCmdType.CMD_LC_TableChangeRoleFaceID:
                return HandleOtherRoleFaceIDChange(obj);
            case NetCmdType.CMD_LC_TableChangeRoleNickName:
                return HandleOtherRoleNickNameChange(obj);
            case NetCmdType.CMD_LC_TableChangeRoleGlobe:
                return HandleOtherRoleGobleNumChange(obj);
            //case NetCmdType.CMD_LC_TableChangeRoleExp:
            //    return HandleOtherRoleExpChange(obj);
            case NetCmdType.CMD_LC_TableChangeRoleTitle:
                return HandleOtherRoleTitleIDChange(obj);
            case NetCmdType.CMD_LC_TableChangeRoleAchievementPoint:
                return HandleOtherRoleAchievementPointChange(obj);
            case NetCmdType.CMD_LC_TableChangeRoleCharmValue:
                return HandleOtherRoleCharmChanger(obj);
            case NetCmdType.CMD_LC_TableChangeRoleIpAddress:
                return HandleOtherRoleIpAddressChange(obj);
            case NetCmdType.CMD_LC_ResetOtherUserInfo:
                return HandleResetOtherUser(obj);
            case NetCmdType.CMD_LC_LeaveTableByServer:
                return HandleLeaveTableByServer(obj);
            case NetCmdType.CMD_LC_TableChangeRoleVipLevel:
                return HandleOtherRoleVipLevelChange(obj);
            case NetCmdType.CMD_LC_TableChangeRoleIsInMonthCard:
                return HandleOtherRoleIsInMonthCardChange(obj);
        }
        return true;
    }
    //bool HandleJoinTableResult(NetCmdBase obj)
    //{
    //    LC_Cmd_JoinTableResult ncb = (LC_Cmd_JoinTableResult)obj;
    //    if (ncb.Result)
    //    {
    //        //进入桌子成功
    //        m_IsIntable = true;

    //        tagTableAddEvent pEvent = new tagTableAddEvent(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
    //        MsgEventHandle.HandleMsg(pEvent);

    //        ClientLogonControl.Instance.HandleJoinRoomSucess();
            
    //    }
    //    else
    //    {
    //        //进入桌子失败
    //        m_IsIntable = false;
    //        ClientLogonControl.Instance.HandleJoinRoomFailed();
    //    }

    //    return true;
    //}
    bool HandleOtherUserInfo(NetCmdBase obj)
    {
        LC_Cmd_OtherUserInfo ncb = (LC_Cmd_OtherUserInfo)obj;
        TableRole pRole = new TableRole(ncb.UserInfo);

        if (m_TableRole.ContainsKey(ncb.UserInfo.dwUserID))
            m_TableRole.Remove(ncb.UserInfo.dwUserID);
        m_TableRole.Add(pRole.GetUserID(), pRole);
        if (m_TableRoleSeat.ContainsKey(pRole.GetSeat()))
            m_TableRoleSeat.Remove(pRole.GetSeat());
        m_TableRoleSeat.Add(pRole.GetSeat(), pRole);

        tagTableAddEvent pEvent = new tagTableAddEvent(pRole.GetUserID());
        MsgEventHandle.HandleMsg(pEvent);

        //tagTableChangeEvent pEvent = new tagTableChangeEvent(pRole.GetUserID());
        //MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleOtherRoleLeave(NetCmdBase obj)
    {
        LC_Cmd_OtherUserLeave ncb = (LC_Cmd_OtherUserLeave)obj;
        if(m_TableRole.ContainsKey(ncb.dwUserID))
        {
            m_TableRoleSeat.Remove(m_TableRole[ncb.dwUserID].GetSeat());
            m_TableRole.Remove(ncb.dwUserID);

            tagTableDelEvent pEvent = new tagTableDelEvent(ncb.dwUserID);
            MsgEventHandle.HandleMsg(pEvent);

        }
       
        

        //tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwUserID);
        //MsgEventHandle.HandleMsg(pEvent);

        
        return true;
    }
    bool HandleMeSeatID(NetCmdBase obj)
    {
        LC_Cmd_MeSeatID ncb = (LC_Cmd_MeSeatID)obj;
        PlayerRole.Instance.RoleInfo.RoleMe.SetSeat(ncb.SeatID);
        return true;
    }
    bool HandleOtherRoleLevelChange(NetCmdBase obj)
    {
        LC_Cmd_TableChangeRoleLevel ncb = (LC_Cmd_TableChangeRoleLevel)obj;
        //修改远程玩家的等级
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            UInt16 OldLevel = m_TableRole[ncb.dwDestUserID].GetLevel();
            m_TableRole[ncb.dwDestUserID].SetLevel(ncb.wLevel);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);

            tagTableLevelChangeEvent pLevelEvent = new tagTableLevelChangeEvent(ncb.dwDestUserID, OldLevel,ncb.wLevel);
            MsgEventHandle.HandleMsg(pLevelEvent);
        }
        return true;
    }
    bool HandleOtherRoleGenderChange(NetCmdBase obj)
    {
        LC_Cmd_TableChangeRoleGender ncb = (LC_Cmd_TableChangeRoleGender)obj;
        //修改远程玩家的等级
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetGender(ncb.bGender);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleOtherRoleFaceIDChange(NetCmdBase obj)
    {
        LC_Cmd_TableChangeRoleFaceID ncb = (LC_Cmd_TableChangeRoleFaceID)obj;
        //修改远程玩家的等级
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetFaceID(ncb.dwDestFaceID);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleOtherRoleNickNameChange(NetCmdBase obj)
    {
        LC_Cmd_TableChangeRoleNickName ncb = (LC_Cmd_TableChangeRoleNickName)obj;
        //修改远程玩家的等级
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetNickName(ncb.NickName);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleOtherRoleGobleNumChange(NetCmdBase obj)
    {
        LC_Cmd_TableChangeRoleGlobe ncb = (LC_Cmd_TableChangeRoleGlobe)obj;
        //修改远程玩家的等级
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetGlobel(Convert.ToUInt32(ncb.dwGlobelSum + m_TableRole[ncb.dwDestUserID].GetGlobel()));

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //bool HandleOtherRoleExpChange(NetCmdBase obj)
    //{
    //    LC_Cmd_TableChangeRoleExp ncb = (LC_Cmd_TableChangeRoleExp)obj;
    //    if (m_TableRole.ContainsKey(ncb.dwDestUserID))
    //    {
    //        m_TableRole[ncb.dwDestUserID].SetExp(ncb.dwExp);

    //        tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
    //        MsgEventHandle.HandleMsg(pEvent);
    //    }
    //    return true;
    //}
    bool HandleOtherRoleTitleIDChange(NetCmdBase obj)
    {
        LC_Cmd_TableChangeRoleTitle ncb = (LC_Cmd_TableChangeRoleTitle)obj;
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetTitleID(ncb.TitleID);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleOtherRoleAchievementPointChange(NetCmdBase obj)
    {
        LC_Cmd_TableChangeRoleAchievementPoint ncb = (LC_Cmd_TableChangeRoleAchievementPoint)obj;
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetAchievementPoint(ncb.dwAchievementPoint);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleOtherRoleCharmChanger(NetCmdBase obj)
    {
        //修改桌子上玩家的魅力数据
        LC_Cmd_TableChangeRoleCharmValue ncb = (LC_Cmd_TableChangeRoleCharmValue)obj;
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetCharmInfo(ncb.CharmArray);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleOtherRoleIpAddressChange(NetCmdBase obj)
    {
        LC_Cmd_TableChangeRoleIpAddress ncb = (LC_Cmd_TableChangeRoleIpAddress)obj;
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetIPAddress(ncb.IpAddress);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleResetOtherUser(NetCmdBase obj)
    {
        LC_Cmd_ResetOtherUserInfo ncb = (LC_Cmd_ResetOtherUserInfo)obj;

        TableRole pTableRole = new TableRole(ncb.UserInfo);
        m_TableRole.Add(pTableRole.GetUserID(), pTableRole);
        m_TableRoleSeat.Add(pTableRole.GetSeat(), pTableRole);

        tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.UserInfo.dwUserID);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleLeaveTableByServer(NetCmdBase obj)
    {
        LC_Cmd_LeaveTableByServer ncb = (LC_Cmd_LeaveTableByServer)obj;
        if (ncb.IsReturnLogon)
        {
            //玩家直接返回登陆界面
            LogicManager.Instance.LogOff();
        }
        else
        {
            if (SceneRuntime.Instance != null)
                SceneRuntime.SceneLogic.BackToHall();
            PlayerRole.Instance.OnUserLeaveTable();//玩家离开桌子的触发函数
        }
        return true;
    }
    bool HandleOtherRoleVipLevelChange(NetCmdBase obj)
    {
        //桌子上玩家VIP变化了
        LC_Cmd_TableChangeRoleVipLevel ncb = (LC_Cmd_TableChangeRoleVipLevel)obj;
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetVipLevel(ncb.VipLevel);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleOtherRoleIsInMonthCardChange(NetCmdBase obj)
    {
        LC_Cmd_TableChangeRoleIsInMonthCard ncb = (LC_Cmd_TableChangeRoleIsInMonthCard)obj;
        if (m_TableRole.ContainsKey(ncb.dwDestUserID))
        {
            m_TableRole[ncb.dwDestUserID].SetIsInMonthCard(ncb.IsInMonthCard);

            tagTableChangeEvent pEvent = new tagTableChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
}