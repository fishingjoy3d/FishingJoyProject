using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RoleGameData
{
    private bool m_IsLoadInfo = false;
    private tagRoleGameData m_GameData = new tagRoleGameData();
    private bool m_IsLoadByServer = false;
    private bool m_IsInMonth;
    private UInt32 m_LogJoinTableTime;

    public RoleGameData()
    {
        OnClear();
    }
    public void OnClear()
    {
        //清理数据
        m_IsLoadInfo = false;
        m_IsLoadByServer = false;
        m_GameData = new tagRoleGameData();
    }
    public void ResetInfo()
    {
        OnClear();
        tagGameDataChangeEvent pEvent = new tagGameDataChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
    }
    public tagRoleGameData GetRoleGameData()
    {
        if (m_IsLoadInfo)
        {
            return m_GameData;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                SendLoadRoleGameData();
            }
            return null;
        }
    }

    //根据服务器端 一些特殊处理的函数
    public void OnHandleCatchFish(Byte FishID)
    {
	    //玩家捕获指定ID的鱼.
        if (!m_IsLoadInfo)
            return;
	    m_GameData.RoleCatchFishTotalSum += 1;
        if (FishID == 9)
        {
            m_GameData.CatchFishSum_9 += 1;
        }
        else if (FishID == 18)
        {
            m_GameData.CatchFishSum_18 += 1;
        }
        else if (FishID == 20)
        {
            m_GameData.CatchFishSum_20 += 1;
        }
        else if (FishID == 1)
        {
            m_GameData.CatchFishSum_1 += 1;
        }
        else if (FishID == 3)
        {
            m_GameData.CatchFishSum_3 += 1;
        }
        else if (FishID == 19)
        {
            m_GameData.CatchFishSum_19 += 1;
        }
    }
    public void OnHandleRoleGetGlobel(UInt32 AddGlobel)
    {
        if (!m_IsLoadInfo)
            return;
	    m_GameData.RoleGetGlobelSum += AddGlobel;
    }

    public string GetTimeStrBySec(UInt32 Sec)
    {
        //根据秒数显示时间  %D %h %m %
        UInt32 Hour = Sec /  3600;
        UInt32 Min = (Sec % 3600) / 60;
        //将4个参数拼接为字符串
        string TimeStr = "";
        if (Hour > 0)
            TimeStr = string.Format("" + Hour.ToString() + "小时+");
        else
            TimeStr = string.Format("" + Min.ToString() + "分钟");
        return TimeStr;
    }
    public string GetGoldStrBygold(UInt32 gold)
    {
        string strGold;
        //100W以上缩为简写
        if (gold >= 1000000)
        {
            gold /= 10000;
            strGold = string.Format("" + gold.ToString() + "万+");
        }
        else
        {
            strGold = gold.ToString();
        }

        return strGold;
    }
    public void OnHandleRoleMonthReward(uint RewardIndex)
    {
        if (!m_IsLoadInfo)
            return;
        m_GameData.RoleMonthRewardSum += 1;
	    if (RewardIndex == 1)
            m_GameData.RoleMonthFirstSum += 1;
	    else if (RewardIndex == 2)
            m_GameData.RoleMonthSecondSum += 1;
	    else if (RewardIndex == 3)
            m_GameData.RoleMonthThreeSum += 1;
    }
    public void OnHandleRoleJoinTable()
    {
        if (!m_IsLoadInfo)
            return;
        m_IsInMonth = false ;
        m_LogJoinTableTime = Utility.GetTickCount();
    }
    public void SetRoleJoinMonth()
    {
        m_IsInMonth = true;
    }
    public void OnHandleRoleLeaveTable()
    {
        if (!m_IsLoadInfo)
            return;
	    //当用户离开桌子的时候 
	    //1.判断玩家是否离开比赛的桌子 并且 判断玩家在桌子上的游戏时间 用于统计数据
        UInt32 GameSec = (Utility.GetTickCount() - m_LogJoinTableTime) / 1000;
        m_GameData.TotalGameSec += GameSec;
	    if (!m_IsInMonth)
            m_GameData.NonMonthGameSec += GameSec;
    }
    public void OnHandleRoleSignUpMonth()
    {
        if (!m_IsLoadInfo)
            return;
	    //玩家报名参加比赛
        m_GameData.RoleMonthSigupSum += 1;
    }
    public void OnHandleRoleCombo(ushort ComboCount)
    {
        if (!m_IsLoadInfo)
            return;
        UInt16 Combo = 0;
        if (m_GameData.MaxCombSum >= Combo)
            return;
        m_GameData.MaxCombSum = Combo;
    }


    private void SendLoadRoleGameData()
    {
        CL_Cmd_GetGameData ncb = new CL_Cmd_GetGameData();
        ncb.SetCmdType(NetCmdType.CMD_CL_GetGameData);
        NetServices.Instance.Send<CL_Cmd_GetGameData>(ncb);
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_GendGameData:
                return HandleLoadRoleGameData(obj);
        }
        return true;
    }
    bool HandleLoadRoleGameData(NetCmdBase obj)
    {
        LC_Cmd_GendGameData ncb = (LC_Cmd_GendGameData)obj;
        m_GameData = ncb.GameData;
        m_IsLoadInfo = true;
        tagGameDataChangeEvent pEvent = new tagGameDataChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}