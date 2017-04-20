using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RoleRecharge
{
    //玩家充值类
    public RoleRecharge()
    {
        
    }
    public bool IsCanPayByID(UInt32 ID)
    {
        if (!FishConfig.Instance.m_FishRecharge.m_FishRechargeMap.ContainsKey(ID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Recharge_SendRecharge_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (FishConfig.Instance.m_FishRecharge.m_FishRechargeMap[ID].IsFirstAdd())
        {
            if (FishConfig.Instance.m_FishRecharge.m_FishRechargeMap[ID].IsAddCurrcey())
            {
                if (!PlayerRole.Instance.RoleInfo.RoleMe.GetIsFirstPayCurrcey())
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Recharge_SendRecharge_Failed_3);
                    MsgEventHandle.HandleMsg(pUOM);
                    return false;
                }
            }
            else if (FishConfig.Instance.m_FishRecharge.m_FishRechargeMap[ID].IsAddGlobel())
            {
                if (!PlayerRole.Instance.RoleInfo.RoleMe.GetIsFirstPayGlobel())
                {
                    tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Recharge_SendRecharge_Failed_3);
                    MsgEventHandle.HandleMsg(pUOM);
                    return false;
                }
            }
        }
        //判断金币是否最大
        if(FishConfig.Instance.m_FishRecharge.m_FishRechargeMap[ID].IsAddGlobel())
        {
            //充值金币 需要进行判断
            UInt64 SrcAllGlobelSum = FishConfig.Instance.m_FishRecharge.m_FishRechargeMap[ID].AddMoney + PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel();
            if (SrcAllGlobelSum >= FishConfig.Instance.m_SystemConfig.MaxGobelSum)
            {
                tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Recharge_SendRecharge_Failed_4);
                MsgEventHandle.HandleMsg(pUOM);
                return false;
            }
        }
        return true;
    }
    public bool SendRecharge(UInt32 ID,bool IsPC = false)
    {
        if (!IsCanPayByID(ID))
            return false;
        //组织命令发送到 外部去
        //...
        CL_Cmd_Recharge ncb = new CL_Cmd_Recharge();
        ncb.SetCmdType(NetCmdType.CMD_CL_Recharge);
        ncb.ID = ID;
        ncb.IsPC = IsPC;
        NetServices.Instance.Send<CL_Cmd_Recharge>(ncb);
        return true;
    }
    public bool OnSendIOSRecharge(string OrderInfo)
    {
        if (OrderInfo.Length == 0)
            return false;
        //处理IOS的充值
        CL_Cmd_IOSRecharge ncb = new CL_Cmd_IOSRecharge();
        ncb.OrderInfo = new IOSRechargeInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_IOSRecharge);
        ByteData[] bytedata = new ByteData[]
        {
            new ByteData(System.Text.Encoding.ASCII.GetBytes(OrderInfo)),
        };
        ncb.OrderInfo.HandleSum = 1;
        ncb.OrderInfo.Sum = 0;
        for (int i = 0; i < bytedata.Length; ++i)
            ncb.OrderInfo.Sum += (uint)(bytedata[i].strData.Length + 2);
        ncb.OrderInfo.Array = new byte[ncb.OrderInfo.Sum];
        int idx = 0;
        for (int i = 0; i < bytedata.Length; ++i)
        {
            byte[] len = System.BitConverter.GetBytes((ushort)bytedata[i].strData.Length);
            ncb.OrderInfo.Array[idx++] = len[0];
            ncb.OrderInfo.Array[idx++] = len[1];
            System.Array.Copy(bytedata[i].strData, 0, ncb.OrderInfo.Array, idx, bytedata[i].strData.Length);
            idx += bytedata[i].strData.Length;
        }
        NetServices.Instance.Send<CL_Cmd_IOSRecharge>(ncb);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_Recharge:
                return HandleRechargeResult(obj);
            case NetCmdType.CMD_LC_GetOrderID:
                return HandleRechargeOrderID(obj);
        }
        return true;
    }
    bool HandleRechargeResult(NetCmdBase obj)
    {
        LC_Cmd_Recharge ncb = (LC_Cmd_Recharge)obj;
        if(ncb.Result)
        {
            tagFishRechargeInfo pInfo;
            if (!FishConfig.Instance.m_FishRecharge.m_FishRechargeMap.TryGetValue(ncb.ID, out pInfo))
                return false;
            UInt32 LostRMB = (UInt32)pInfo.dDisCountPrice;
            //更新进度条
            PlayerRole.Instance.HandleEvent(EventTargetType.ET_Recharge, 0, LostRMB);
        }
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result?UserOperateMessage.UOM_Recharge_SendRecharge_Sucess:UserOperateMessage.UOM_Recharge_SendRecharge_Failed_1));
        MsgEventHandle.HandleMsg(pUOM);

        tagRechargeResultEvent pEvent = new tagRechargeResultEvent(ncb.ID, ncb.Result);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool HandleRechargeOrderID(NetCmdBase obj)
    {
        LC_Cmd_GetOrderID ncb = (LC_Cmd_GetOrderID)obj;
        tagUserOperationEvent pUOM = new tagUserOperationEvent((ncb.Result ? UserOperateMessage.UOM_Recharge_GetOrderID_Success : UserOperateMessage.UOM_Recharge_GetOrderID_Failed_1));
        MsgEventHandle.HandleMsg(pUOM);

        tagRechargeGetOrderIDEvent pEvent = new tagRechargeGetOrderIDEvent(ncb.OrderID,ncb.ShopID, ncb.Result);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }
}