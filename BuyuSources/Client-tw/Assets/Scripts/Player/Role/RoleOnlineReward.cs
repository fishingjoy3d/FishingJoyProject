using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家在线奖励的控制类
class RoleOnlineReward
{
    //玩家在线奖励的类 用于后台控制
    public RoleOnlineReward()
    {
        OnClear();
    }
    public void OnClear()
    {

    }
    public bool GetOnlineReward(Byte ID)//领取指定奖励
    {
        return SendGetReward(ID);
    }
    public tagOnceOnlienReward FindValiedReward()
    {
        Dictionary<byte, tagOnceOnlienReward>.KeyCollection keys = FishConfig.Instance.m_OnlineRewardConfig.m_OnlineRewardMap.Keys;
        foreach (byte map_Key in keys)
        {
            tagOnceOnlienReward reward;
            if (FishConfig.Instance.m_OnlineRewardConfig.m_OnlineRewardMap.TryGetValue(map_Key, out reward))
            {
                if ( (PlayerRole.Instance.RoleInfo.RoleMe.GetOnlineRewardStates() & (1 << (map_Key - 1) ) )==0)//第一个有效奖励
                {
                    return reward;
                }
            }
        }
        return null;
    }
    public bool IsCanGetReward(Byte ID)//判断奖励是否可以领取
    {
        if (ID < 1 || ID > 32)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_OnlineReward_GetReward_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        tagOnceOnlienReward pReward = null;
        if (!FishConfig.Instance.m_OnlineRewardConfig.m_OnlineRewardMap.TryGetValue(ID, out pReward))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_OnlineReward_GetReward_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetOnlineMin() < pReward.OnlineMin)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_OnlineReward_GetReward_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (((PlayerRole.Instance.RoleInfo.RoleMe.GetOnlineRewardStates() & (1 << (ID - 1))) == 1 ? true : false))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_OnlineReward_GetReward_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        return true;
    }

    private bool SendGetReward(Byte ID)
    {
        if (!IsCanGetReward(ID))
            return false;
        //发送命令
        CL_Cmd_GetOnlineReward msg = new CL_Cmd_GetOnlineReward();
        msg.SetCmdType(NetCmdType.CMD_CL_GetOnlineReward);
        msg.ID = ID;
        NetServices.Instance.Send<CL_Cmd_GetOnlineReward>(msg);
        return true;
    }

    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetOnlineReward:
                return HandleGetOnlineReward(obj);
        }
        return true;
    }
    bool HandleGetOnlineReward(NetCmdBase obj)
    {
        LC_Cmd_GetOnlineReward ncb = (LC_Cmd_GetOnlineReward)obj;

        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result?UserOperateMessage.UOM_OnlineReward_GetReward_Sucess:UserOperateMessage.UOM_OnlineReward_GetReward_Failed_5));
        MsgEventHandle.HandleMsg(pUOM);

        if(ncb.Result)
        {
            //玩家领取奖励成功了
            PlayerRole.Instance.RoleInfo.RoleMe.SetOnlineRewardStates(ncb.States);//设置玩家的领取状
            //触发事件
            tagOnlineRewardOnceChangeEvent pEvent = new tagOnlineRewardOnceChangeEvent(ncb.RewardID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
}