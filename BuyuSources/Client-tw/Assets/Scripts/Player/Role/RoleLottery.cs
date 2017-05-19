using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家抽奖类
class RoleLottery
{
    public RoleLottery()
    {
        OnClear();
    }
    public void OnClear()
    {

    }
    public void OnRoleCatchByLottery(Byte FishTypeID, CatchedData cd)
    {
        //if (PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryFishSum() >= FishConfig.Instance.m_LotteryConfig.MaxLotteryFishSum)
        //    return;
        UInt16 rateIndex = 1;
        if(cd.CatchType == (Byte)CatchedType.CATCHED_BULLET || cd.CatchType == (Byte)CatchedType.CATCHED_LASER)
            rateIndex = BulletSetting.BulletRate[SceneRuntime.PlayerMgr.MySelf.RateIndex];
        else if (cd.CatchType == (Byte)CatchedType.CATCHED_SKILL)
            rateIndex = SkillSetting.SkillDataList[cd.SubType].multiple;
        UInt32 Score = FishConfig.Instance.m_LotteryConfig.GetFishLotteryScore(FishTypeID) * rateIndex;
        if (Score == 0)
            return;
        if(PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryFishSum() < Byte.MaxValue)
            PlayerRole.Instance.RoleInfo.RoleMe.SetLotteryFishSum(Convert.ToByte(PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryFishSum() + 1));
        PlayerRole.Instance.RoleInfo.RoleMe.SetLotteryScore(PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryScore() + Score);

        tagLotteryScoreChangeEvent pEventLottery = new tagLotteryScoreChangeEvent();
        MsgEventHandle.HandleMsg(pEventLottery);
    }
    public Byte GetLotteryID()
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryFishSum() >= FishConfig.Instance.m_LotteryConfig.MaxLotteryFishSum)
        {
            //判断玩家的积分
            UInt32 Score = PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryScore();
            UInt32 LogScore = Score;
            Byte ID = 0;
            foreach(tagLotteryOnce var in FishConfig.Instance.m_LotteryConfig.LotteryMap.Values)
            {
                if(Score >= var.NeedUseScore)
                {
                    if (ID !=0 && LogScore > var.NeedUseScore)
                        continue;
                    LogScore = var.NeedUseScore;
                    ID = var.LotteryID;
                }
            }
            return ID;
        }
        else
        {
            return 0;
        }
    }
    public bool IsCanStarLottery(Byte LotteryID) //是否可以开始抽奖
    {
        //开始抽奖
        if (!FishConfig.Instance.m_LotteryConfig.LotteryMap.ContainsKey(LotteryID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Lottery_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryFishSum() < FishConfig.Instance.m_LotteryConfig.MaxLotteryFishSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Lottery_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        UInt32 LotteryScore = PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryScore();
        if (LotteryScore < FishConfig.Instance.m_LotteryConfig.LotteryMap[LotteryID].NeedUseScore)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Lottery_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        return true;
    }
    public bool GetLotteryReward(Byte LotteryID)
    {
        //获取抽奖的奖励 获取指定抽奖的奖励
        if (!FishConfig.Instance.m_LotteryConfig.LotteryMap.ContainsKey(LotteryID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Lottery_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryFishSum() < FishConfig.Instance.m_LotteryConfig.MaxLotteryFishSum)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Lottery_Failed_4);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        UInt32 LotteryScore = PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryScore();
        if (LotteryScore < FishConfig.Instance.m_LotteryConfig.LotteryMap[LotteryID].NeedUseScore)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Lottery_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //发送命令
        CL_Cmd_GetLotteryReward ncb = new CL_Cmd_GetLotteryReward();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetLotteryReward);
        ncb.LotteryID = LotteryID;
        NetServices.Instance.Send<CL_Cmd_GetLotteryReward>(ncb);
        return true;
    }
    public void RoleChangeLotteryUIStates(bool IsOpenOrClose)
    {
        CL_Cmd_LotteryUIStates ncb = new CL_Cmd_LotteryUIStates();
        ncb.SetCmdType(NetCmdType.CMD_CL_LotteryUIStates);
        ncb.dwUserID = PlayerRole.Instance.RoleInfo.RoleMe.GetUserID();
        ncb.IsOpen = IsOpenOrClose;
        NetServices.Instance.Send<CL_Cmd_LotteryUIStates>(ncb);
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GetLotteryReward:
                return HandleGetLotteryReward(obj);
            case NetCmdType.CMD_LC_LotteryUIStates:
                return HandleLotteryStatesChange(obj);
        }
        return true;
    }
    bool HandleGetLotteryReward(NetCmdBase obj)
    {
        LC_Cmd_GetLotteryReward ncb = (LC_Cmd_GetLotteryReward)obj;
        if (ncb.Result)
        {
            //玩家领取奖励成功了 
            if(!FishConfig.Instance.m_LotteryConfig.LotteryMap.ContainsKey(ncb.LotteryID))
                return false;
            //扣除玩家积分
            PlayerRole.Instance.RoleInfo.RoleMe.SetLotteryScore(0);
            PlayerRole.Instance.RoleInfo.RoleMe.SetLotteryFishSum(0);

            tagLotteryScoreChangeEvent pEventLottery = new tagLotteryScoreChangeEvent();
            MsgEventHandle.HandleMsg(pEventLottery);
            //提示
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Lottery_Sucess);//系统错误
            MsgEventHandle.HandleMsg(pUOM);
            //告诉玩家 事件
            tagLotteryResultEvent pEvent = new tagLotteryResultEvent(ncb.LotteryID, ncb.Result, ncb.RewardID);
            MsgEventHandle.HandleMsg(pEvent);
            return true;
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Lottery_Failed_3);//系统错误
            MsgEventHandle.HandleMsg(pUOM);

            tagLotteryResultEvent pEvent = new tagLotteryResultEvent(ncb.LotteryID, ncb.Result, ncb.RewardID);
            MsgEventHandle.HandleMsg(pEvent);
            return true;
        }
    }
    bool HandleLotteryStatesChange(NetCmdBase obj)
    {
        LC_Cmd_LotteryUIStates ncb = (LC_Cmd_LotteryUIStates)obj;

        //抽奖UI状态改变
        tagTableLotteryUIChangeEvent pEvent = new tagTableLotteryUIChangeEvent(ncb.dwUserID, ncb.IsOpen);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
}