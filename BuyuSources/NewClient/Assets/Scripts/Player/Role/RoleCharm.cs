using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//用户魅力管理器
class RoleCharm
{
    public RoleCharm()
    {
        OnClear();
    }
    public void OnClear()
    {

    }
    public Int32 GetCharmValue()
    {
        Int32 charmSun = 0;
        for (byte i = 0; i < PlayerRole.Instance.RoleInfo.RoleMe.GetCharmInfo().Length; ++i)
        {
            tagCharmOnce charmOnce;
            if (FishConfig.Instance.m_CharmInfo.CharmIndexMap.TryGetValue(i, out charmOnce))
                charmSun += Convert.ToInt32(PlayerRole.Instance.RoleInfo.RoleMe.GetCharmInfo()[i] * charmOnce.ChangeCharmValue);
        }
        return charmSun;
    }
    public Int32 GetCharmValue(UInt32 dwUserID)
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetUserID() == dwUserID)
            return GetCharmValue();
        else
        {
            TableRole pRole = PlayerRole.Instance.TableManager.GetTableRole(dwUserID);
            if (pRole == null)
                return 0;
            else
            {
                Int32 charmSun = 0;
                for (byte i = 0; i < pRole.GetCharmInfo().Length; ++i)
                {
                    tagCharmOnce charmOnce;
                    if (FishConfig.Instance.m_CharmInfo.CharmIndexMap.TryGetValue(i, out charmOnce))
                        charmSun +=  Convert.ToInt32(pRole.GetCharmInfo()[i] * (int)charmOnce.ChangeCharmValue);//有可能为-数
                }
                return charmSun;
            }
        }
    }
    public Int32 GetCharmValue(UInt32[] charmInfo)
    {
        int charmSum = 0;
        for (byte i = 0; i < charmInfo.Length; ++i)
        {
            tagCharmOnce charmOnce;
            if (FishConfig.Instance.m_CharmInfo.CharmIndexMap.TryGetValue(i, out charmOnce))
                charmSum += Convert.ToInt32(charmInfo[i] * (int)charmOnce.ChangeCharmValue);//有可能为-数
        }
        return charmSum;
    }
    public bool AddTargetCharm(UInt32 dwDestUserID, Byte CharmIndex, UInt32 ItemSum)
    {
        return SendAddTargetUserCharm(dwDestUserID, CharmIndex, ItemSum);
    }

    private bool SendAddTargetUserCharm(UInt32 dwDestUserID, Byte CharmIndex, UInt32 ItemSum)
    {
        //给目标添加魅力 使用指定物品
        if (dwDestUserID == PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Charm_SendCharm_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_CharmInfo.CharmIndexMap.ContainsKey(CharmIndex))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Charm_SendCharm_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if( (PlayerRole.Instance.ItemManager.GetItemSum(FishConfig.Instance.m_CharmInfo.CharmIndexMap[CharmIndex].ItemID, false) < FishConfig.Instance.m_CharmInfo.CharmIndexMap[CharmIndex].ItemSum * ItemSum) &&
             (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_CharmInfo.CharmIndexMap[CharmIndex].UserGlobel * ItemSum ||
            PlayerRole.Instance.RoleInfo.RoleMe.GetMedal() < FishConfig.Instance.m_CharmInfo.CharmIndexMap[CharmIndex].UserMedal * ItemSum ||
            PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_CharmInfo.CharmIndexMap[CharmIndex].UserCurrcey * ItemSum))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Charm_SendCharm_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_ChangeOtherUserCharm ncb = new CL_Cmd_ChangeOtherUserCharm();
        ncb.SetCmdType(NetCmdType.CMD_CL_ChangeOtherUserCharm);
        ncb.CharmIndex = CharmIndex;
        ncb.dwDestUserID = dwDestUserID;
        ncb.ItemSum = ItemSum;
        NetServices.Instance.Send<CL_Cmd_ChangeOtherUserCharm>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_ChangeRoleCharmResult:
                return HandleAddTargetCharmResult(obj);
            case NetCmdType.CMD_LC_TableRoleCharmInfo:
                return HandleTableRoleCharmInfo(obj);
        }
        return true;
    }
    bool HandleAddTargetCharmResult(NetCmdBase obj)
    {
        LC_Cmd_ChangeRoleCharmResult ncb = (LC_Cmd_ChangeRoleCharmResult)obj;
        if(ncb.Result)
        {
            tagUserChangeEvent pEvent = new tagUserChangeEvent(ncb.dwDestUserID, ncb.dwDestUserCharmValue);
            MsgEventHandle.HandleMsg(pEvent);
        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Charm_SendCharm_Sucess : UserOperateMessage.UOM_Charm_SendCharm_Failed_4));
        MsgEventHandle.HandleMsg(pUOM);
        return true;
    }
    bool HandleTableRoleCharmInfo(NetCmdBase obj)
    {
        LC_Cmd_TableRoleCharmInfo ncb = (LC_Cmd_TableRoleCharmInfo)obj;
        tagCharmRewardOnce pEvent = new tagCharmRewardOnce(ncb.SrcUserID, ncb.DestUserID, ncb.CharmIndex, ncb.ItemSum);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}