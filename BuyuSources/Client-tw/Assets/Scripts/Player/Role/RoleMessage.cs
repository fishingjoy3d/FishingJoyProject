using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RoleMessage
{
    //消息类
    public bool SendMessageToServer(string StrMessage,UInt32 MessageColor,Byte StepNum,Byte StepSec)
    {
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(FishDataInfo.MIN_MESSAGE_LENGTH, FishDataInfo.MAX_MESSAGE_LENGTH,StrMessage,StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Message_SendMessage_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        //判断玩家物品是否足够
        UInt32 ItemID = FishConfig.Instance.m_SystemConfig.SendMessageItemID;
        UInt16 ItemSum = FishConfig.Instance.m_SystemConfig.SendMessageItemSum;
        if(ItemSum > PlayerRole.Instance.ItemManager.GetItemSum(ItemID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_Message_SendMessage_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        string NewMessage = string.Format("[{0}]/lb:{1}", PlayerRole.Instance.RoleInfo.RoleMe.GetNickName(), StrMessage);

        CL_Cmd_SendMessage ncb = new CL_Cmd_SendMessage();
        ncb.SetCmdType(NetCmdType.CMD_CL_SendMessage);
        ncb.Message = NewMessage;
        ncb.MessageType = (Byte)MessageType.MT_Center;
        ncb.MessageColor = MessageColor;
        ncb.MessageSize = Convert.ToUInt16(NewMessage.Length);
        ncb.StepNum = StepNum;
        ncb.StepSec = StepSec;
        ncb.Param = 0;
        NetServices.Instance.Send<CL_Cmd_SendMessage>(ncb);

        tagUserOperationEvent pUOM2 = new tagUserOperationEvent(UserOperateMessage.UOM_Message_SendMessage_Sucess);
        MsgEventHandle.HandleMsg(pUOM2);
        return true;
    }
    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_SendMessage:
                return HandMessage(obj);
        }
        return true;
    }
    bool HandMessage(NetCmdBase obj)
    {
        LC_Cmd_SendMessage ncb = (LC_Cmd_SendMessage)obj;
        //将消息显示出来
        //GlobalHallUIMgr.Instance.ShowSystemNotice(ncb.Message);

        tagNoticeOnceEvent pEvent = new tagNoticeOnceEvent((MessageType)ncb.MessageType,ncb.MessageColor, ncb.Message, ncb.StepNum, ncb.StepSec);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}