using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家进行查询的管理器 处理
class RoleQuery
{
    private List<QueryRole> m_QueryResultList = new List<QueryRole>();//最近一次查询的结果
    //private Dictionary<UInt32, QueryRole> m_QueryMap = new Dictionary<UInt32, QueryRole>();
    public RoleQuery()
    {
        OnClear();
    }
    public void OnClear()
    {
       // m_QueryMap.Clear();
        m_QueryResultList.Clear();
    }

    //对外提供的命令
    public bool QueryUserListByNickName(string NickName,Byte Page,bool IsOnline)
    {
        return SendQueryRoleByNickName(NickName, Page, IsOnline);
    }
    public bool QueryUserByUserID(UInt32 dwQueryUserID)
    {

        return SendQueryRoleByUserID(dwQueryUserID);
    }
    public bool QueryUserByGameID(UInt32 GameID)
    {
        return SendQueryRoleByGameID(GameID);
    }
    public  List<QueryRole> GetQueryList()
    {
        return m_QueryResultList;//获取上传查询的数据
    }

    //继续命令处理
    private bool SendQueryRoleByNickName(string NickName, Byte Page, bool IsOnline)
    {
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(0, FishDataInfo.NickNameLength,NickName,StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Query_QueryName_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_QueryUserByNickName ncb = new CL_Cmd_QueryUserByNickName();
        ncb.SetCmdType(NetCmdType.CMD_CL_QueryUserByNickName);
        ncb.NickName = NickName;
        ncb.Page = Page;
        ncb.IsOnline = IsOnline;
        NetServices.Instance.Send<CL_Cmd_QueryUserByNickName>(ncb);
        m_QueryResultList.Clear();
        return true;
    }
    private bool SendQueryRoleByUserID(UInt32 UserID)
    {
        if(UserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Query_QueryID_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_QueryUserByUserID ncb = new CL_Cmd_QueryUserByUserID();
        ncb.SetCmdType(NetCmdType.CMD_CL_QueryUserByUserID);
        ncb.dwDestUserID = UserID;
        NetServices.Instance.Send<CL_Cmd_QueryUserByUserID>(ncb);
        m_QueryResultList.Clear();
        return true;
    }
    private bool SendQueryRoleByGameID(UInt32 GameID)
    {
        if (GameID == PlayerRole.Instance.RoleInfo.RoleMe.GetGameID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Query_QueryID_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_QueryUserByGameID ncb = new CL_Cmd_QueryUserByGameID();
        ncb.SetCmdType(NetCmdType.CMD_CL_QueryUserByGameID);
        ncb.GameID = GameID;
        NetServices.Instance.Send<CL_Cmd_QueryUserByGameID>(ncb);
        m_QueryResultList.Clear();
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_QueryUserInfo:
                return HandleQueryRoleInfo(obj);
            case NetCmdType.CMD_LC_QueryUserOnce:
                return HandleQueryRoleInfoOnce(obj);
        }
        return true;
    }
    bool HandleQueryRoleInfo(NetCmdBase obj)
    {
        LC_Cmd_QueryUserInfo ncb = (LC_Cmd_QueryUserInfo)obj;
        if((ncb.States & FishDataInfo.MsgBegin)!= 0)
        {
            m_QueryResultList.Clear();
        }
        for (int i = 0; i < ncb.Sum; ++i)
        {
            QueryRole pRole = new QueryRole(ncb.Array[i]);
            m_QueryResultList.Add(pRole);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            tagQueryChangeEvent pEvent = new tagQueryChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);

            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Query_QueryName_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        return true;
    }
    bool HandleQueryRoleInfoOnce(NetCmdBase obj)
    {
        LC_Cmd_QueryUserOnce ncb = (LC_Cmd_QueryUserOnce)obj;
        if (ncb.RoleInfo.dwUserID == 0)
        {
            return false;
        }
        QueryRole pRole = new QueryRole(ncb.RoleInfo);
        m_QueryResultList.Clear();
        m_QueryResultList.Add(pRole);

        tagQueryRoleEvent pEvent = new tagQueryRoleEvent(pRole);
        MsgEventHandle.HandleMsg(pEvent);

        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Query_QueryID_Sucess);
        MsgEventHandle.HandleMsg(pUOM);
        return true;
    }
}