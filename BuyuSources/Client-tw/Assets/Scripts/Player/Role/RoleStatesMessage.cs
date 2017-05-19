using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RoleStatesMessage
{
    enum RoleMessageType
    {
        RMT_Mail = 1,//未读的邮件状态
        RMT_WeekRank = 2,//未领取的排行榜奖励
        RMT_Giff = 4,//未领取的赠送 
        RMT_Task = 8,
        RMT_Achievement = 16,
        RMT_Action = 32,
        RMT_Check = 64,
        RMT_Char = 128,
        RMT_Relation = 256,
    };

    private UInt32 StatesData;
    public UInt32 GetStatesData()
    {
        return StatesData;
    }
    public RoleStatesMessage()
    {
        OnClear();
    }
    public bool GetMailStates()
    {
        return (StatesData & (uint)RoleMessageType.RMT_Mail) != 0;
    }
    public bool GetWeekRankStates()
    {
        return (StatesData & (uint)RoleMessageType.RMT_WeekRank) != 0;
    }
    public bool GetGiffStates()
    {
        return (StatesData & (uint)RoleMessageType.RMT_Giff) != 0;
    }
    public bool GetTaskStates()
    {
        return (StatesData & (uint)RoleMessageType.RMT_Task) != 0;
    }
    public bool GetAchievementStates()
    {
        return (StatesData & (uint)RoleMessageType.RMT_Achievement) != 0;
    }
    public bool GetActionStates()
    {
        return (StatesData & (uint)RoleMessageType.RMT_Action) != 0;
    }
    public bool GetCheckStates()
    {
        return (StatesData & (uint)RoleMessageType.RMT_Check) != 0;
    }
    public bool GetCharSrates()
    {
        return (StatesData & (uint)RoleMessageType.RMT_Char) != 0;
    }
    public bool GetRelationStates()
    {
        return (StatesData & (uint)RoleMessageType.RMT_Relation) != 0;
    }
    public void OnClear()
    {
        StatesData = 0;
    }

    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_RoleMessageChange:
                return HandleRoleMessageChange(obj);
        }
        return true;
    }
    bool HandleRoleMessageChange(NetCmdBase obj)
    {
        LC_Cmd_RoleMessageChange ncb = (LC_Cmd_RoleMessageChange)obj;
        StatesData = ncb.RoleMessageData;
        //触发事件
        tagRoleStatesChangeEvent pEvent = new tagRoleStatesChangeEvent(StatesData);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}