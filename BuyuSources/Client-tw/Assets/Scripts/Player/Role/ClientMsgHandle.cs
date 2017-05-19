using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//客户端的消息处理类
class ClientMsgHandle : ICmdHandler
{
    private PlayerRole m_pPlayerRole = null;
    public ClientMsgHandle(PlayerRole pRole)
    {
        m_pPlayerRole = pRole;
    }
    public bool CanProcessCmd()
    {
        return true;
    }
    public bool Handle(NetCmdPack cmd)
    {
        if (GlobalLogon.Instance.Handle(cmd))
            return true;

        //接收来自服务器端的命令 我们开始进行处理 
        MainCmdType MainType = (MainCmdType)((UInt16)cmd.cmd.GetCmdType()>>8);
        switch (MainType)
        {
            case MainCmdType.CMD_MAIN_Logon:
                return m_pPlayerRole.RoleInfo.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Table:
                return m_pPlayerRole.TableManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Item:
                return m_pPlayerRole.ItemManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Relation:
                return m_pPlayerRole.RelationManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Mail:
                return m_pPlayerRole.MailManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Role:
                return m_pPlayerRole.RoleInfo.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Query:
                return m_pPlayerRole.QueryManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Check:
                return m_pPlayerRole.CheckManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Task:
                return m_pPlayerRole.TaskManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Achievement:
                return m_pPlayerRole.AchievementManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Month:
                return m_pPlayerRole.MonthManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Title:
                return m_pPlayerRole.TitleManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Rank:
                return m_pPlayerRole.RankManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Chest:
                return m_pPlayerRole.ChestManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Charm:
                return m_pPlayerRole.CharmManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Shop:
                return m_pPlayerRole.ShopManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Entity:
                return m_pPlayerRole.EntityManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Action:
                return m_pPlayerRole.ActionManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Giff:
                return m_pPlayerRole.GiffManager.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_OnlineReward:
                return m_pPlayerRole.RoleOnlineReward.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_GameData:
                return m_pPlayerRole.RoleGameData.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_MAIN_Package:
                return m_pPlayerRole.RolePackage.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_Launcher:
                return m_pPlayerRole.RoleLauncher.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_Message:
                return m_pPlayerRole.RoleMessage.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_Recharge:
                return m_pPlayerRole.RoleRecharge.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_Announcement:
                return m_pPlayerRole.RoleAnnouncement.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_Operate:
                return m_pPlayerRole.RoleOperate.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_RoleMessage:
                return m_pPlayerRole.RoleStatesMessage.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_Exchange:
                return m_pPlayerRole.RoleExChange.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_Lottery:
                return m_pPlayerRole.RoleLottery.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_MiniGame:
            case MainCmdType.CMD_Main_NiuNiu:
            case MainCmdType.CMD_Main_Dial:
            case MainCmdType.CMD_Main_Car:
                return m_pPlayerRole.RoleMiNiGame.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_Char:
                return m_pPlayerRole.RoleChar.HandleCmd(cmd.cmd);
            case MainCmdType.CMD_Main_RelationRequest:
                return m_pPlayerRole.RoleRelationRequest.HandleCmd(cmd.cmd);
        }
        return true;
    }
    public void StateChanged(NetState state)
    {
        //当网络库状态发生变化的时候 我们进行处理

    }
}