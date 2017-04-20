using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家邮件管理器
class RoleMail
{
    public RoleMail()
    {
        OnClear();
    }
    public void OnClear()
    {
        NormalMailTitleList.Clear();
        NormalMailMap.Clear();
        SystemMailTitleList.Clear();
        SystemMailMap.Clear();
        m_IsLoadDB = false;
        m_IsLoadByServer = false;
        //m_IsExistsNonReadMail = false;
    }
    public void ResetInfo()
    {
        OnClear();
        //邮件栏全部物品
        tagMailTitleChangeEvent pEvent = new tagMailTitleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagMailAllContextChangeEvent pEvent2 = new tagMailAllContextChangeEvent();
        MsgEventHandle.HandleMsg(pEvent2);
    }
    //玩家邮件
    private bool m_IsLoadDB = false;
    private List<tagNormalMailTitle> NormalMailTitleList = new List<tagNormalMailTitle>();
    private Dictionary<UInt32, tagNormalMail> NormalMailMap = new Dictionary<UInt32, tagNormalMail>();
    private List<tagSystemMailTitle> SystemMailTitleList = new List<tagSystemMailTitle>();
    private Dictionary<UInt32, tagSystemMail> SystemMailMap = new Dictionary<UInt32, tagSystemMail>();
    private bool m_IsLoadByServer = false;

    //供外部使用的函数 邮件方面的
    public void UpdateByNewDay()
    {
        if (NormalMailTitleList.Count == 0 && SystemMailTitleList.Count == 0)
            return;
        for (int i = 0; i < NormalMailTitleList.Count; ++i)
        {
            NormalMailTitleList[i].bDiffTime = Convert.ToByte((NormalMailTitleList[i].bDiffTime + 1 >= 250) ? 250 : (NormalMailTitleList[i].bDiffTime + 1));
        }
        for (int i = 0; i < NormalMailMap.Count; ++i)
        {
            NormalMailMap.Values.ElementAt<tagNormalMail>(i).bDiffTime = Convert.ToByte((NormalMailMap.Values.ElementAt<tagNormalMail>(i).bDiffTime + 1 >= 250) ? 250 : (NormalMailMap.Values.ElementAt<tagNormalMail>(i).bDiffTime + 1));
        }
        for (int i = 0; i < SystemMailTitleList.Count; ++i)
        {
            SystemMailTitleList[i].bDiffTime = Convert.ToByte((SystemMailTitleList[i].bDiffTime + 1 >= 250) ? 250 : (SystemMailTitleList[i].bDiffTime + 1));
        }
        for (int i = 0; i < SystemMailMap.Count; ++i)
        {
            SystemMailMap.Values.ElementAt<tagSystemMail>(i).bDiffTime = Convert.ToByte((SystemMailMap.Values.ElementAt<tagSystemMail>(i).bDiffTime + 1 >= 250) ? 250 : (SystemMailMap.Values.ElementAt<tagSystemMail>(i).bDiffTime + 1));
        }
        tagMailTitleChangeEvent pEvent = new tagMailTitleChangeEvent();//邮件发送天数的变化
        MsgEventHandle.HandleMsg(pEvent);
    }

    public bool GetAllUserMail(ref  List<tagNormalMailTitle> pNormalList,ref List<tagSystemMailTitle> pSystemList)
    {
        //pNormalList.Clear();
        //pSystemList.Clear();
        if(m_IsLoadDB)
        {
            //if (NormalMailTitleList.Count == 0 || SystemMailTitleList.Count == 0)
            //    MailTest();
            if (!CheckIsNeedGetServerMail())
            {
                pNormalList = NormalMailTitleList;
                pSystemList = SystemMailTitleList;
                return true;
            }
            else
            {
                //客户端邮件错误了 需要重新向服务器进行查询
                m_IsLoadByServer = true;
                NormalMailTitleList.Clear();
                NormalMailMap.Clear();
                SystemMailTitleList.Clear();
                SystemMailMap.Clear();
                SendGetUserAllMail();
                pNormalList = null;
                pSystemList = null;
                m_IsLoadDB = false;
                return false;
            }
            
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                NormalMailTitleList.Clear();
                NormalMailMap.Clear();
                SystemMailTitleList.Clear();
                SystemMailMap.Clear();
                SendGetUserAllMail();
                pNormalList = null;
                pSystemList = null;
            }
            return false;
        }
    }
    private bool CheckIsNeedGetServerMail()
    {
        if(!PlayerRole.Instance.RoleStatesMessage.GetMailStates())
            return false;
        if ((NormalMailTitleList.Count + SystemMailTitleList.Count) < FishConfig.Instance.m_MailConfig.ClientShowMailSum)
        {
            //有未读的邮件
            foreach (tagNormalMailTitle var in NormalMailTitleList)
            {
                if (!var.bIsRead)
                    return false;
            }
            foreach (tagSystemMailTitle var in SystemMailTitleList)
            {
                if (!var.bIsRead)
                    return false;
            }
            //重新向服务器请求邮件数据
            SendGetUserAllMail();
            return true;
        }
        else
            return false;
    }
    public bool IsSystemMail(UInt32 MailID)
    {
        return (SystemMailMap.ContainsKey(MailID));
    }
    public bool IsNormalMail(UInt32 MailID)
    {
        return (NormalMailMap.ContainsKey(MailID));
    }
    public bool GetMailContext(UInt32 MailID,ref tagNormalMail NormalMail,ref tagSystemMail SystemMail)
    {
        NormalMail = null;
        SystemMail = null;
        if (NormalMailMap.ContainsKey(MailID))
        {
            NormalMail = NormalMailMap[MailID];
            return true;
        }
        else if (SystemMailMap.ContainsKey(MailID))
        {
            SystemMail = SystemMailMap[MailID];
            return true;
        }
        else
        {
            SendGetMailContext(MailID);
            return false;
        }
    }
    public tagNormalMail GetNormalMailContext(UInt32 MailID)
    {
        if (NormalMailMap.ContainsKey(MailID))
            return NormalMailMap[MailID];
        else
        {
            if (!IsSystemMail(MailID))
                SendGetMailContext(MailID);
            return null;
        }            
    }
    public tagSystemMail GetSystemMailContext(UInt32 MailID)
    {
        if (SystemMailMap.ContainsKey(MailID))
            return SystemMailMap[MailID];
        else
        {
            if (!IsNormalMail(MailID))
                SendGetMailContext(MailID);
            return null;
        }
    }
    public bool DelMail(UInt32 MailID)
    {
        SendDelUserMail(MailID);
        return true;
    }
    public bool SendNewMailToOtherUser(UInt32 DestUserID,string pMailContext)
    {
        return SendNewUserMail(DestUserID, pMailContext);
    }
    public bool GetSystemMailItem(UInt32 MailID)
    {
        tagSystemMail pSystemMail = GetSystemMailContext(MailID);
        if (pSystemMail == null)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Mail_GetSystemMailItem_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (pSystemMail.RewardID == 0 || pSystemMail.RewardSum == 0)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Mail_GetSystemMailItem_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(!pSystemMail.bIsExistsReward)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Mail_GetSystemMailItem_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        SendGetMailItem(MailID);
        return true;
    }


    private void SendGetUserAllMail()
    {
        CL_Cmd_GetUserMail ncb = new CL_Cmd_GetUserMail();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetUserMail);
        NetServices.Instance.Send<CL_Cmd_GetUserMail>(ncb);
    }
    private bool SendNewUserMail(UInt32 dwDestUserID, string pMailContext)
    {
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MinMailContexeLength, FishDataInfo.MaxMailContexeLength, pMailContext,StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Mail_SendMail_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(dwDestUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            //不可以给直接发送邮件
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Mail_SendMail_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_SendUserMail ncb = new CL_Cmd_SendUserMail();
        ncb.SetCmdType(NetCmdType.CMD_CL_SendUserMail);
        ncb.dwDestUserID = dwDestUserID;
        ncb.Context = pMailContext;
        NetServices.Instance.Send<CL_Cmd_SendUserMail>(ncb);
        return true;
    }
    private void SendDelUserMail(UInt32 MailID)
    {
        CL_Cmd_DelMail ncb = new CL_Cmd_DelMail();
        ncb.SetCmdType(NetCmdType.CMD_CL_DelUserMail);
        ncb.dwMailID = MailID;
        NetServices.Instance.Send<CL_Cmd_DelMail>(ncb);
    }
    private void SendGetMailContext(UInt32 MailID)
    {
        CL_Cmd_GetMailContext ncb = new CL_Cmd_GetMailContext();
        ncb.SetCmdType(NetCmdType.CMD_CL_ReadMail);
        ncb.dwMailID = MailID;
        NetServices.Instance.Send<CL_Cmd_GetMailContext>(ncb);
    }
    private void SendGetMailItem(UInt32 MailID)
    {
        CL_Cmd_GetMailItem ncb = new CL_Cmd_GetMailItem();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetMailItem);
        ncb.dwMailID = MailID;
        NetServices.Instance.Send<CL_Cmd_GetMailItem>(ncb);
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetUserSystemMail:
                return HandleLoadUserSystemMail(obj);
            case NetCmdType.CMD_LC_GetUserNormalMail:
                return HandleLoadUserNormalMail(obj);
            //case NetCmdType.CMD_LC_GetUserMailFinish:
            //    return HandleLoadUserMailFinish(obj);
            case NetCmdType.CMD_LC_SendUserMailResult:
                return HandleSendNewUserNormalMail(obj);
            case NetCmdType.CMD_LC_DelUserMail:
                return HandleDelUserMail(obj);
            case NetCmdType.CMD_LC_GetMailItem:
                return HandleGetUserMailItem(obj);
            case NetCmdType.CMD_LC_ReadNormalMail:
                return HandleGetUserNormalMail(obj);
            case NetCmdType.CMD_LC_ReadSystemMail:
                return HandleGetUserSystemMail(obj);
            case NetCmdType.CMD_LC_ReadMailError:
                return HandleGetUserMailError(obj);
            case NetCmdType.CMD_LC_AddNormalTitle:
                return HandleAddUserNormalMail(obj);
            case NetCmdType.CMD_LC_AddSystemTitle:
                return HandleAddUserSystemMail(obj);
        }
        return true;
    }
    private bool HandleLoadUserSystemMail(NetCmdBase obj)
    {
        LC_Cmd_SystemMail ncb = (LC_Cmd_SystemMail)obj;
        if((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            SystemMailTitleList.Clear();
        }
        for (UInt16 i = 0; i < ncb.Sum; ++i)
        {
            SystemMailTitleList.Add(ncb.Array[i]);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadDB = true;
            tagMailTitleChangeEvent pEvent = new tagMailTitleChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    private bool HandleLoadUserNormalMail(NetCmdBase obj)
    {
        LC_Cmd_NormalMail ncb = (LC_Cmd_NormalMail)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            NormalMailTitleList.Clear();
        }
        for (UInt16 i = 0; i < ncb.Sum; ++i)
        {
            NormalMailTitleList.Add(ncb.Array[i]);
        }
        return true;
    }
    //private bool HandleLoadUserMailFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_GetMailFinish ncb = (LC_Cmd_GetMailFinish)obj;
    //   // FriendSysManager.Instance.UpdateEmailList();
        
    //    //CheckMailStates();

       
    //    return true;
    //}
    private bool HandleSendNewUserNormalMail(NetCmdBase obj)
    {
        LC_Cmd_SendUserMailResult ncb = (LC_Cmd_SendUserMailResult)obj;

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Mail_SendMail_Sucess : UserOperateMessage.UOM_Mail_SendMail_Failed_3));
        MsgEventHandle.HandleMsg(pUOM);

        return true;//提示发送邮件成功 或者失败
    }
    private bool HandleDelUserMail(NetCmdBase obj)
    {
        LC_Cmd_DelMail ncb = (LC_Cmd_DelMail)obj;
        if (ncb.Result)
        {
            //移除邮件
            bool IsRemove = false;
            foreach (tagNormalMailTitle var in NormalMailTitleList)
            {
                if (var.MailID == ncb.dwMailID)
                {
                    //移除
                    NormalMailTitleList.Remove(var);
                    NormalMailMap.Remove(var.MailID);
                    IsRemove = true;

                    //tagMailTitleChangeEvent pEvent = new tagMailTitleChangeEvent();
                    //MsgEventHandle.HandleMsg(pEvent);
                    tagMailTitleDelEvent pEvent = new tagMailTitleDelEvent(ncb.dwMailID);
                    MsgEventHandle.HandleMsg(pEvent);

                    tagMailContextDelEvent pContextEvent = new tagMailContextDelEvent(ncb.dwMailID);
                    MsgEventHandle.HandleMsg(pContextEvent);

                    break;
                }
            }
            if (!IsRemove)
            {
                foreach (tagSystemMailTitle var in SystemMailTitleList)
                {
                    if (var.MailID == ncb.dwMailID)
                    {
                        //移除
                        SystemMailTitleList.Remove(var);
                        SystemMailMap.Remove(var.MailID);

                        tagMailTitleDelEvent pEvent = new tagMailTitleDelEvent(ncb.dwMailID);
                        MsgEventHandle.HandleMsg(pEvent);

                        tagMailContextDelEvent pContextEvent = new tagMailContextDelEvent(ncb.dwMailID);
                        MsgEventHandle.HandleMsg(pContextEvent);

                        break;
                    }
                }
            }
            //移除成功
            //FriendSysManager.Instance.OnDeleteListMsg(ncb.dwMailID, WndType.Emanil_Wnd)
        }

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Mail_DelMail_Sucess : UserOperateMessage.UOM_Mail_DelMail_Failed_1));
        MsgEventHandle.HandleMsg(pUOM);

        return true;
    }
    private bool HandleGetUserMailItem(NetCmdBase obj)
    {
        LC_Cmd_GetMailItem ncb = (LC_Cmd_GetMailItem)obj;
        if (ncb.Result)
        {
            //获取邮件物品成功 我们修改物品的状态
            for (int i = 0; i < SystemMailTitleList.Count; ++i)
            {
                if (SystemMailTitleList[i].MailID == ncb.dwMailID)
                {
                    SystemMailTitleList[i].bIsRead = true;
                    SystemMailTitleList[i].bIsExistsItem = false;
                    if (SystemMailMap.ContainsKey(ncb.dwMailID))
                    {
                        SystemMailMap[ncb.dwMailID].bIsRead = true;
                        SystemMailMap[ncb.dwMailID].bIsExistsReward = false;
                        //SystemMailMap[ncb.dwMailID].RewardID = 0;//无须屏蔽奖励ID 
                    }

                    tagMailContextChangeEvent pContextEvent = new tagMailContextChangeEvent(ncb.dwMailID);
                    MsgEventHandle.HandleMsg(pContextEvent);

                    tagMailTitleChangeEvent pEvent = new tagMailTitleChangeEvent();
                    MsgEventHandle.HandleMsg(pEvent);

                    tagMailGetItemEvent pGetItem = new tagMailGetItemEvent();
                    MsgEventHandle.HandleMsg(pGetItem);

                    break;
                }
            }
        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Mail_GetSystemMailItem_Sucess : UserOperateMessage.UOM_Mail_GetSystemMailItem_Failed_4));
        MsgEventHandle.HandleMsg(pUOM);

        return true;
    }
    private bool HandleGetUserNormalMail(NetCmdBase obj)
    {
        //获取到一封普通的邮件
        LC_Cmd_GetNormalMail ncb = (LC_Cmd_GetNormalMail)obj;
        NormalMailMap.Add(ncb.MailInfo.MailID, ncb.MailInfo);
        for (byte i = 0; i < NormalMailTitleList.Count; ++i)
        {
            if (NormalMailTitleList[i].MailID == ncb.MailInfo.MailID)
            {
                NormalMailTitleList[i].bIsRead = true;
                //FriendSysManager.Instance.UpdateEmailReadState(ncb.MailInfo.MailID);
            }
        }

        //CheckMailStates();

        tagMailTitleChangeEvent pEvent = new tagMailTitleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagMailContextAddEvent pContextEvent = new tagMailContextAddEvent(ncb.MailInfo.MailID);
        MsgEventHandle.HandleMsg(pContextEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Mail_GetNormalContext_Sucess);
        MsgEventHandle.HandleMsg(pUOM);
        //FriendSysManager.Instance.ReciveReadEmailMsg((byte)EmailType.Normal_Email, ncb.MailInfo.MailID, true);

        return true;
    }
    private bool HandleGetUserSystemMail(NetCmdBase obj)
    {
        LC_Cmd_GetSystemMail ncb = (LC_Cmd_GetSystemMail)obj;
        if (SystemMailMap.ContainsKey(ncb.MailInfo.MailID))
            SystemMailMap.Remove(ncb.MailInfo.MailID);
        SystemMailMap.Add(ncb.MailInfo.MailID, ncb.MailInfo);
        for (byte i = 0; i < SystemMailTitleList.Count; ++i)
        {
            if (SystemMailTitleList[i].MailID == ncb.MailInfo.MailID)
            {
                SystemMailTitleList[i].bIsRead = true;
                //FriendSysManager.Instance.UpdateEmailReadState(ncb.MailInfo.MailID);
            }
        }

        //CheckMailStates();

        tagMailTitleChangeEvent pEvent = new tagMailTitleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        tagMailContextAddEvent pContextEvent = new tagMailContextAddEvent(ncb.MailInfo.MailID);
        MsgEventHandle.HandleMsg(pContextEvent);
        //FriendSysManager.Instance.ReciveReadEmailMsg((byte)EmailType.Sys_Email, ncb.MailInfo.MailID, true);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Mail_GetSystemContext_Sucess);
        MsgEventHandle.HandleMsg(pUOM);

        return true;
    }
    private bool HandleGetUserMailError(NetCmdBase obj)
    {
        //打开邮件失败
        LC_Cmd_GetMailError ncb = (LC_Cmd_GetMailError)obj;

        //FriendSysManager.Instance.ReciveReadEmailMsg((byte)EmailType.Normal_Email, 0, false);


        return true;
    }
    private bool HandleAddUserNormalMail(NetCmdBase obj)
    {
        LC_Cmd_AddNormalMail ncb = (LC_Cmd_AddNormalMail)obj;
        NormalMailTitleList.Add(ncb.MailInfo);

        //CheckMailStates();

        tagMailTitleAddEvent pEvent = new tagMailTitleAddEvent(ncb.MailInfo.MailID,false);
        MsgEventHandle.HandleMsg(pEvent);

       // FriendSysManager.Instance.AddNormalMail(ncb.MailInfo);
        return true;
    }
    private bool HandleAddUserSystemMail(NetCmdBase obj)
    {
        LC_Cmd_AddSystemMail ncb = (LC_Cmd_AddSystemMail)obj;
        SystemMailTitleList.Add(ncb.MailInfo);

        //CheckMailStates();

        tagMailTitleAddEvent pEvent = new tagMailTitleAddEvent(ncb.MailInfo.MailID,true);
        MsgEventHandle.HandleMsg(pEvent);

        //FriendSysManager.Instance.AddSysMail(ncb.MailInfo);

        return true;
    }
}