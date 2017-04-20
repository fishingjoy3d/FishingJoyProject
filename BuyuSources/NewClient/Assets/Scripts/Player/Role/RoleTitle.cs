using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家称号管理器
class RoleTitle
{
    private bool m_IsLoadInfo = false;
    private Dictionary<Byte, Byte> TitleManager = new Dictionary<Byte, Byte>();
    private bool m_IsLoadByServer = false;
    public RoleTitle()
    {
        OnClear();
    }
    public void OnClear()
    {
        m_IsLoadInfo = false;
        m_IsLoadByServer = false;
        TitleManager.Clear();
    }
    //公共函数
    public Dictionary<Byte,Byte> GetAllTitle()
    {
        if (m_IsLoadInfo)
        {
            return TitleManager;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                TitleManager.Clear();
                SendLoadRoleTitle();
            }
            return null;
        }
    }
    public bool  ChangeCurreyTitle(Byte TitleID)
    {
        return SendChangeRoleTitleID(TitleID);
    }

    private void SendLoadRoleTitle()
    {
        CL_Cmd_LoadRoleTitle ncb = new CL_Cmd_LoadRoleTitle();
        ncb.SetCmdType(NetCmdType.CMD_CL_LoadRoleTitle);
        NetServices.Instance.Send<CL_Cmd_LoadRoleTitle>(ncb);
    }
    private bool SendChangeRoleTitleID(Byte TitleID)
    {
        //判断称号是否存在
        if (!TitleManager.ContainsKey(TitleID) || TitleManager[TitleID] != 1)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Title_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //判断玩家当前称号是否和当前相同
        CL_Cmd_ChangeRoleCurrTitle ncb = new CL_Cmd_ChangeRoleCurrTitle();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeRoleCurrTitle);
        ncb.TitleID = TitleID;
        NetServices.Instance.Send<CL_Cmd_ChangeRoleCurrTitle>(ncb);
        return true;
    }
    public void ResetInfo()
    {
        OnClear();
        //清理排行榜的数据 在排行榜打开的情况下 
        tagTitleChangeEvent pEvent = new tagTitleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_LoadRoleTitle:
                return HandleLoadRoleTitle(obj);
            //case NetCmdType.CMD_LC_LoadRoleTitleFinish:
            //    return HandleLoadRoleTitleFinish(obj);
            case NetCmdType.CMD_LC_AddRoleTitle:
                return HandleAddRoleTitle(obj);
            case NetCmdType.CMD_LC_DelRoleTitle:
                return HandleDelRoleTitle(obj);
        }
        return true;
    }
    bool HandleLoadRoleTitle(NetCmdBase obj)
    {
        LC_Cmd_LoadRoleTitle ncb = (LC_Cmd_LoadRoleTitle)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            TitleManager.Clear();
        }
        for (int i = 0; i < ncb.Sum; ++i)
        {
            TitleManager.Add(ncb.Array[i], 1);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadInfo = true;

            tagTitleChangeEvent pEvent = new tagTitleChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    //bool HandleLoadRoleTitleFinish(NetCmdBase obj)
    //{
    //    LC_Cmd_LoadRoleTitleFinish ncb = (LC_Cmd_LoadRoleTitleFinish)obj;
       

    //    //UIWidgetManager.Instance.ChangeUserInfHeadContent();
    //    return true;
    //}
    bool HandleAddRoleTitle(NetCmdBase obj)
    {
        LC_Cmd_AddRoleTitle ncb = (LC_Cmd_AddRoleTitle)obj;
        TitleManager.Add(ncb.AddTitleID, 1);

       tagTitleChangeEvent pEvent = new tagTitleChangeEvent();
       MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
    bool HandleDelRoleTitle(NetCmdBase obj)
    {
        LC_Cmd_DelRoleTitle ncb = (LC_Cmd_DelRoleTitle)obj;
        TitleManager.Remove(ncb.DelTitleID);

        tagTitleChangeEvent pEvent = new tagTitleChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
}