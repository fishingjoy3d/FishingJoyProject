using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//玩家关系管理器 用于关系方面的处理 已经管理
class RoleRelation
{
    //玩家的关系数据
    private bool m_IsLoad = false;
    private Dictionary<UInt32, RelationRole> FriendMap = new Dictionary<UInt32, RelationRole>();
    private Dictionary<UInt32, RelationRole> BlackListMap = new Dictionary<UInt32, RelationRole>();
    private bool m_IsLoadByServer = false;
    public RoleRelation()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_IsLoad = false;
        m_IsLoadByServer = false;
        FriendMap.Clear();
        BlackListMap.Clear();
    }
    public void ResetInfo()
    {
        OnClear();
        //清理排行榜的数据 在排行榜打开的情况下 
        tagFriendLoadEvent pEvent = new tagFriendLoadEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagBlackLoadEvent pEvent2 = new tagBlackLoadEvent();
        MsgEventHandle.HandleMsg(pEvent2);
    }
    public void UpdateByDay()//每天12点进行更新
    {
        if(FriendMap.Count > 0)
        {
            bool IsChange = false;
            for(int i =0;i<FriendMap.Count;++i)
            {
                if(FriendMap.Values.ElementAt<RelationRole>(i).GetLeaveOnlineDay() != 0xff && FriendMap.Values.ElementAt<RelationRole>(i).GetLeaveOnlineDay()<250)
                {
                    FriendMap.Values.ElementAt<RelationRole>(i).SetLeaveOnlineDay(Convert.ToByte(FriendMap.Values.ElementAt<RelationRole>(i).GetLeaveOnlineDay() + 1));//离线天数加1
                    IsChange = true;
                }
            }
            if(IsChange)
            {
                //tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent();
                //MsgEventHandle.HandleMsg(pFirendEvent);

                tagFriendLoadEvent pFirendEvent = new tagFriendLoadEvent();
                MsgEventHandle.HandleMsg(pFirendEvent);
            }
        }
        if (BlackListMap.Count > 0)
        {
            bool IsChange = false;
            for (int i = 0; i < BlackListMap.Count; ++i)
            {
                if (BlackListMap.Values.ElementAt<RelationRole>(i).GetLeaveOnlineDay() != 0xff && BlackListMap.Values.ElementAt<RelationRole>(i).GetLeaveOnlineDay() < 250)
                {
                    BlackListMap.Values.ElementAt<RelationRole>(i).SetLeaveOnlineDay(Convert.ToByte(BlackListMap.Values.ElementAt<RelationRole>(i).GetLeaveOnlineDay() + 1));//离线天数加1
                    IsChange = true;
                }
            }
            if (IsChange)
            {
                tagBlackLoadEvent pBlackEvent = new tagBlackLoadEvent();
                MsgEventHandle.HandleMsg(pBlackEvent);
            }
        }
    }
    public Dictionary<UInt32, RelationRole> GetFriendMap()
    {
        if(m_IsLoad)
        {
            //if (FriendMap.Count == 0)
            //    RelationTest();
            return FriendMap;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = false;
                SendGetUserRelation();
            }
            //RelationTest();
            return null;
        }
    }
    public Dictionary<UInt32, RelationRole> GetBlackMap()
    {
        if (m_IsLoad)
        {
            return BlackListMap;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = false;
                SendGetUserRelation();
            }
            //RelationTest();
            return null;
        }
    }
    public bool IsExitesRelation(UInt32 dwUserID)
    {
        if (GetFriendMap() == null || GetBlackMap() == null)
            return false;
        if (QueryRelationRole(dwUserID) == null)
            return false;
        else
            return true;
    }
    public bool AddOtherRoleRelation(UInt32 dwDestUserID,Byte RelationType)
    {
        return SendAddUserRelation(dwDestUserID, RelationType);
    }
    public bool DelRelation(UInt32 dwDestUserID)
    {
        return SendDelUserRelation(dwDestUserID);
    }
    //public bool ChangeOtherRoleRelation(UInt32 dwDestUserID,Byte RelationType)
    //{    
    //    return SendChangeUserRelation(dwDestUserID, RelationType);
    //}
    public RelationRole QueryRelationRole(UInt32 dwDestUserID)
    {
        if (FriendMap.ContainsKey(dwDestUserID))
            return FriendMap[dwDestUserID];
        else if (BlackListMap.ContainsKey(dwDestUserID))
            return BlackListMap[dwDestUserID];
        else
            return null;
    }

    //关系命令处理
    private void SendGetUserRelation()
    {
        CL_Cmd_GetUserRelation ncb = new CL_Cmd_GetUserRelation();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetUserRelation);
        NetServices.Instance.Send<CL_Cmd_GetUserRelation>(ncb);
    }
    private bool SendAddUserRelation(UInt32 dwDestRoleID, Byte RelationType)
    {
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID() == dwDestRoleID)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Relation_AddRelation_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if ((FriendMap.ContainsKey(dwDestRoleID) && FriendMap[dwDestRoleID].GetRelationType() == RelationType) || BlackListMap.ContainsKey(dwDestRoleID) && BlackListMap[dwDestRoleID].GetRelationType() == RelationType)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Relation_AddRelation_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (FriendMap.Count + BlackListMap.Count >= FishConfig.Instance.m_RelationInfo.MaxRelationSum && !FriendMap.ContainsKey(dwDestRoleID) && !BlackListMap.ContainsKey(dwDestRoleID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Relation_AddRelation_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_AddUserRelation ncb = new CL_Cmd_AddUserRelation();
        ncb.SetCmdType(NetCmdType.CMD_CL_AddUserRelation);
        ncb.dwDestRoleID = dwDestRoleID;
        ncb.bRelationType = RelationType;
        NetServices.Instance.Send<CL_Cmd_AddUserRelation>(ncb);
        return true;
    }
    private bool SendDelUserRelation(UInt32 dwDestRoleID)
    {
        if (!FriendMap.ContainsKey(dwDestRoleID) && !BlackListMap.ContainsKey(dwDestRoleID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Relation_DelRelation_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_DelUserRelation ncb = new CL_Cmd_DelUserRelation();
        ncb.SetCmdType(NetCmdType.CMD_CL_DelUserRelation);
        ncb.dwDestRoleID = dwDestRoleID;
        NetServices.Instance.Send<CL_Cmd_DelUserRelation>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetUserRelation:
                return HandGetRoleRelation(obj);
            case NetCmdType.CMD_LC_AddUserRelation:
                return HandleAddRelation(obj);
            case NetCmdType.CMD_LC_DelUserRelation:
                return HandleDelRelation(obj);
            case NetCmdType.CMD_LC_ChangeUserRelation:
                return HandleChangeRelation(obj);
            case NetCmdType.CMD_LC_RoleChangeOnline:
                return HandleChangeRoleOnline(obj);
            case NetCmdType.CMD_LC_ChangeRelationLevel:
                return HandleChangeRelationLevel(obj);
            case NetCmdType.CMD_LC_ChangeRelationFaceID:
                return HandleChangeRelationFaceID(obj);
            case NetCmdType.CMD_LC_ChangeRelationNickName:
                return HandleChangeRelationNickName(obj);
            case NetCmdType.CMD_LC_ChangeRelationGender:
                return HandleChangeRelationGender(obj);
            case NetCmdType.CMD_LC_ChangeRelationTitle:
                return HandleChangeRelationTitle(obj);
            case NetCmdType.CMD_LC_ChangeRelationAchievementPoint:
                return HandleChangeRelationAchievementPoint(obj);
            case NetCmdType.CMD_LC_ChangeRelationCharmValue:
                return HandleChangeRelationCharm(obj);
            case NetCmdType.CMD_LC_ChangeRelationIP:
                return HandleChangeRelationClientIP(obj);
            case NetCmdType.CMD_LC_ChangeRelationVipLevel:
                return HandleChangeRelationVipLevel(obj);
            case NetCmdType.CMD_LC_ChangeRelationIsInMonthCard:
                return HandleChangeRelationIsInMonthCard(obj);
            default:
                return false;
        }
    }
    bool HandGetRoleRelation(NetCmdBase obj)
    {
        LC_Cmd_GetUserRelation ncb = (LC_Cmd_GetUserRelation)obj;
        //向客户端添加关系集合
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            FriendMap.Clear();
            BlackListMap.Clear();
        }
        for (UInt16 i = 0; i < ncb.Sum; ++i)
        {
            RelationRole pBase = new RelationRole(ncb.Array[i]);
            if (ncb.Array[i].bRelationType == 1)
            {
                if (FriendMap.ContainsKey(pBase.GetUserID()))
                    return false;
                FriendMap.Add(pBase.GetUserID(), pBase);
            }
            else if (ncb.Array[i].bRelationType == 2)
            {
                if (BlackListMap.ContainsKey(pBase.GetUserID()))
                    return false;
                BlackListMap.Add(pBase.GetUserID(), pBase);
            }
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoad = true;

            tagFriendLoadEvent pFirendEvent = new tagFriendLoadEvent();
            MsgEventHandle.HandleMsg(pFirendEvent);

            tagBlackLoadEvent pBlackEvent = new tagBlackLoadEvent();
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }

    bool HandleAddRelation(NetCmdBase obj)
    {
        LC_Cmd_AddUserRelation ncb = (LC_Cmd_AddUserRelation)obj;

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Relation_AddRelation_Sucess : UserOperateMessage.UOM_Relation_AddRelation_Failed_4));
        MsgEventHandle.HandleMsg(pUOM);

        if (ncb.Result == false)
        {
            return false;
        }
        RelationRole pBase = new RelationRole(ncb.RelationInfo);
        if (ncb.RelationInfo.bRelationType == 1)
        {
            FriendMap.Add(pBase.GetUserID(), pBase);
               
            //FriendSysManager.Instance.OnAddFriend(ncb.RelationInfo.dwDestRoleID);
            //tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent();
            //MsgEventHandle.HandleMsg(pFirendEvent);
            tagFriendAddEvent pFriendEvent = new tagFriendAddEvent(ncb.RelationInfo.dwDestRoleID);
            MsgEventHandle.HandleMsg(pFriendEvent);
        }
        else if (ncb.RelationInfo.bRelationType == 2)
        {
            BlackListMap.Add(pBase.GetUserID(), pBase);

            //tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent();
            //MsgEventHandle.HandleMsg(pBlackEvent);

            tagBlackAddEvent pBlackEvent = new tagBlackAddEvent(ncb.RelationInfo.dwDestRoleID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleDelRelation(NetCmdBase obj)
    {
        LC_Cmd_DelUserRelation ncb = (LC_Cmd_DelUserRelation)obj;

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Relation_DelRelation_Sucess : UserOperateMessage.UOM_Relation_DelRelation_Failed_2));
        MsgEventHandle.HandleMsg(pUOM);

        if (ncb.Result)
        {
            if (FriendMap.ContainsKey(ncb.dwDestRoleID) == true)
            {
                FriendMap.Remove(ncb.dwDestRoleID);
               // FriendSysManager.Instance.OnDeleteListMsg(ncb.dwDestRoleID, WndType.Friend_Wnd);

                //tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent();
                //MsgEventHandle.HandleMsg(pFirendEvent);

                tagFriendDelEvent pFirendEvent = new tagFriendDelEvent(ncb.dwDestRoleID);
                MsgEventHandle.HandleMsg(pFirendEvent);
            }
            else if (BlackListMap.ContainsKey(ncb.dwDestRoleID) == true)
            {
                BlackListMap.Remove(ncb.dwDestRoleID);

                //tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent();
                //MsgEventHandle.HandleMsg(pBlackEvent);
                tagBlackDelEvent pBlackEvent = new tagBlackDelEvent(ncb.dwDestRoleID);
                MsgEventHandle.HandleMsg(pBlackEvent);
            }
        }
        return true;
    }
    bool HandleChangeRelation(NetCmdBase obj)
    {
        LC_Cmd_ChangeUserRelation ncb = (LC_Cmd_ChangeUserRelation)obj;

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Relation_ChangeRelation_Sucess : UserOperateMessage.UOM_Relation_ChangeRelation_Failed_1));
        MsgEventHandle.HandleMsg(pUOM);

        if (ncb.Result && FriendMap.ContainsKey(ncb.dwDestRoleID) && ncb.bRelationType == 2)    //加入黑名单
        {
            RelationRole pRole = FriendMap[ncb.dwDestRoleID];
            pRole.SetRelationType(ncb.bRelationType);
            BlackListMap.Add(ncb.dwDestRoleID, pRole);
            FriendMap.Remove(ncb.dwDestRoleID);

            //tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent();
            //MsgEventHandle.HandleMsg(pFirendEvent);

            //tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent();
            //MsgEventHandle.HandleMsg(pBlackEvent);

            tagFriendDelEvent pFriendEvent = new tagFriendDelEvent(ncb.dwDestRoleID);
            MsgEventHandle.HandleMsg(pFriendEvent);

            tagBlackAddEvent pBlackEvent = new tagBlackAddEvent(ncb.dwDestRoleID);
            MsgEventHandle.HandleMsg(pBlackEvent);

        }
        else if (ncb.Result && BlackListMap.ContainsKey(ncb.dwDestRoleID) && ncb.bRelationType == 1)   //黑名单
        {
            RelationRole pRole = BlackListMap[ncb.dwDestRoleID];
            pRole.SetRelationType(ncb.bRelationType);
            FriendMap.Add(ncb.dwDestRoleID, pRole);
            BlackListMap.Remove(ncb.dwDestRoleID);

            //tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent();
            //MsgEventHandle.HandleMsg(pFirendEvent);

            //tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent();
            //MsgEventHandle.HandleMsg(pBlackEvent);

            tagFriendAddEvent pFriendEvent = new tagFriendAddEvent(ncb.dwDestRoleID);
            MsgEventHandle.HandleMsg(pFriendEvent);

            tagBlackDelEvent pBlackEvent = new tagBlackDelEvent(ncb.dwDestRoleID);
            MsgEventHandle.HandleMsg(pBlackEvent);

        }
        return true;
    }
    bool HandleChangeRoleOnline(NetCmdBase obj)
    {
        //修改玩家在线 玩家离线
        LC_Cmd_ChangeUserOline ncb = (LC_Cmd_ChangeUserOline)obj;
        if (FriendMap.ContainsKey(ncb.dwDestRoleID))
        {
            Byte Value = 0;
            if (ncb.IsOnline)
                Value = 0xFF;
            else
                Value = 0;
            FriendMap[ncb.dwDestRoleID].SetLeaveOnlineDay(Value);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestRoleID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestRoleID))
        {
            Byte Value = 0;
            if (ncb.IsOnline)
                Value = 0xFF;
            else
                Value = 0;
            BlackListMap[ncb.dwDestRoleID].SetLeaveOnlineDay(Value);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestRoleID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationLevel(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationLevel ncb = (LC_Cmd_ChangeRelationLevel)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
            FriendMap[ncb.dwDestUserID].SetLevel(ncb.wLevel);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetLevel(ncb.wLevel);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationFaceID(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationFaceID ncb = (LC_Cmd_ChangeRelationFaceID)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
            FriendMap[ncb.dwDestUserID].SetFaceID(ncb.dwFaceID);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetFaceID(ncb.dwFaceID);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationNickName(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationNickName ncb = (LC_Cmd_ChangeRelationNickName)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
           FriendMap[ncb.dwDestUserID].SetNickName(ncb.cNickName);

           tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
           MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetNickName(ncb.cNickName);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationGender(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationGender ncb = (LC_Cmd_ChangeRelationGender)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
            FriendMap[ncb.dwDestUserID].SetGender(ncb.bGender);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetGender(ncb.bGender);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationTitle(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationTitle ncb = (LC_Cmd_ChangeRelationTitle)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
            FriendMap[ncb.dwDestUserID].SetTitleID(ncb.TitleID);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetTitleID(ncb.TitleID);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationAchievementPoint(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationAchievementPoint ncb = (LC_Cmd_ChangeRelationAchievementPoint)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
            FriendMap[ncb.dwDestUserID].SetAchievementPoint(ncb.dwAchievementPoint);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetAchievementPoint(ncb.dwAchievementPoint);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationCharm(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationCharmValue ncb = (LC_Cmd_ChangeRelationCharmValue)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
            FriendMap[ncb.dwDestUserID].SetCharmInfo(ncb.CharmArray);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetCharmInfo(ncb.CharmArray);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationClientIP(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationIP ncb = (LC_Cmd_ChangeRelationIP)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
            FriendMap[ncb.dwDestUserID].SetIPAddress(ncb.IPAddress);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetIPAddress(ncb.IPAddress);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationVipLevel(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationVipLevel ncb = (LC_Cmd_ChangeRelationVipLevel)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
            FriendMap[ncb.dwDestUserID].SetVipLevel(ncb.VipLevel);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetVipLevel(ncb.VipLevel);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
    bool HandleChangeRelationIsInMonthCard(NetCmdBase obj)
    {
        LC_Cmd_ChangeRelationIsInMonthCard ncb = (LC_Cmd_ChangeRelationIsInMonthCard)obj;
        if (FriendMap.ContainsKey(ncb.dwDestUserID))
        {
            FriendMap[ncb.dwDestUserID].SetIsInMonthCard(ncb.IsInMonthCard);

            tagFriendChangeEvent pFirendEvent = new tagFriendChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pFirendEvent);
        }
        else if (BlackListMap.ContainsKey(ncb.dwDestUserID))
        {
            BlackListMap[ncb.dwDestUserID].SetIsInMonthCard(ncb.IsInMonthCard);

            tagBlackChangeEvent pBlackEvent = new tagBlackChangeEvent(ncb.dwDestUserID);
            MsgEventHandle.HandleMsg(pBlackEvent);
        }
        return true;
    }
}