using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RoleRelationRequest
{
    //用户的关系请求类
    public Dictionary<UInt32, tagRelationRequest> m_RequestMap = new Dictionary<UInt32, tagRelationRequest>(); //客户端关系的列表
    private bool m_IsLoadInfo = false;
    private bool m_IsLoadByServer = false;
    
    public void OnClear()
    {
        m_IsLoadInfo = false;
        m_IsLoadByServer = false;
        m_RequestMap.Clear();
    }
    public Dictionary<UInt32, tagRelationRequest> GetAllInfo()
    {
        if (m_IsLoadInfo)
        {
            return m_RequestMap;
        }
        else
        {
            if (!m_IsLoadByServer)
            {
                m_IsLoadByServer = true;
                SendLoadRequestInfo();
            }
            return null;
        }
    }

    private void SendLoadRequestInfo() //请求加载全部的请求数据
    {
        CL_Cmd_LoadRelationRequest ncb = new CL_Cmd_LoadRelationRequest();
        ncb.SetCmdType(NetCmdType.CMD_CL_LoadRelationRequest);
        NetServices.Instance.Send<CL_Cmd_LoadRelationRequest>(ncb);
    }

    public bool SendRelationRequest(UInt32 DestUserID,Byte RelationType,string MessageInfo)
    {
        //玩家试图发送请求 到目标玩家 
        //1.目标玩家不存在关系
        //2.字符串 符合要求 并且 校验非法字符 
        //3.关系类型正确
        //4.发送命令
        if(RelationType != 1)
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(1, FishDataInfo.MAX_RelationRequest_MsgLength, MessageInfo, StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if(PlayerRole.Instance.RelationManager.IsExitesRelation(DestUserID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_3);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_SendRelationRequest ncb = new CL_Cmd_SendRelationRequest();
        ncb.SetCmdType(NetCmdType.CMD_CL_SendRelationRequest);
        ncb.Info = new tagRelationRequest();
        ncb.Info.DestUserID = DestUserID;
        ncb.Info.MessageInfo = MessageInfo;
        ncb.Info.SrcUserID = PlayerRole.Instance.RoleInfo.RoleMe.GetUserID();
        ncb.Info.RelationType = RelationType;
        NetServices.Instance.Send<CL_Cmd_SendRelationRequest>(ncb);
        return true;
    }

    public bool HandleRelationRequest(UInt32 ID ,bool Result)
    {
        if(!m_RequestMap.ContainsKey(ID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_HandleRequest_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_HandleRelationRequest ncb = new CL_Cmd_HandleRelationRequest();
        ncb.SetCmdType(NetCmdType.CMD_CL_HandleRelationRequest);
        ncb.ID = ID;
        ncb.Result = Result;
        NetServices.Instance.Send<CL_Cmd_HandleRelationRequest>(ncb);
        return true;
    }

    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_LoadRelationRequest:
                return OnHandleLoadRelationRequest(obj);
            case NetCmdType.CMD_LC_SendRelationRequest:
                return OnHandleSendRelationRequest(obj);
            case NetCmdType.CMD_LC_BeAddRelationRequest:
                return OnHandleBeAddRelationRequest(obj);
            case NetCmdType.CMD_LC_HandleRelationRequest:
                return OnHandleRelationRequest(obj);
            case NetCmdType.CMD_LC_DelRelationRequest:
                return OnHandleDelRelationRequest(obj);
        }
        return false;
    }


    bool OnHandleLoadRelationRequest(NetCmdBase obj)
    {
        LC_Cmd_LoadRelationRequest ncb = (LC_Cmd_LoadRelationRequest)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            m_RequestMap.Clear();
        }
        for (UInt16 i = 0; i < ncb.Sum;++i )
        {
            m_RequestMap.Add(ncb.Array[i].ID, ncb.Array[i]);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadInfo = true;

            //产生事件
            tagRelationRequestChangeEvent pEvent = new tagRelationRequestChangeEvent();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }

    bool OnHandleSendRelationRequest(NetCmdBase obj)
    {
        LC_Cmd_SendRelationRequest ncb = (LC_Cmd_SendRelationRequest)obj;
        //用户发送请求的结果
        if(!ncb.Result)
        {
            switch(ncb.ErrorID)
            {
                case 1:
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_4);
                        MsgEventHandle.HandleMsg(pUOM);
                    }
                    break;
                case 2:
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_5);
                        MsgEventHandle.HandleMsg(pUOM);
                    }
                    break;
                case 3:
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_6);
                        MsgEventHandle.HandleMsg(pUOM);
                    }
                    break;
                case 4:
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_7);
                        MsgEventHandle.HandleMsg(pUOM);
                    }
                    break;
                case 5:
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_8);
                        MsgEventHandle.HandleMsg(pUOM);
                    }
                    break;
                case 6:
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_9);
                        MsgEventHandle.HandleMsg(pUOM);
                    }
                    break;
                case 7:
                    {
                        tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Failed_10);
                        MsgEventHandle.HandleMsg(pUOM);
                    }
                    break;
            }
        }
        else
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RelationRequest_SendRequest_Sucess);
            MsgEventHandle.HandleMsg(pUOM);
        }
        //事件
        tagRelationRequestSendResultEvent pEvent = new tagRelationRequestSendResultEvent(ncb.Info, ncb.Result);
        MsgEventHandle.HandleMsg(pEvent);

        return true;
    }

    bool OnHandleBeAddRelationRequest(NetCmdBase obj)
    {
        LC_Cmd_BeAddRelationRequest ncb = (LC_Cmd_BeAddRelationRequest)obj;
        m_RequestMap.Add(ncb.Info.ID,ncb.Info);
        //产生事件
        tagRelationRequestChangeEvent pEvent = new tagRelationRequestChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }

    bool OnHandleRelationRequest(NetCmdBase obj)
    {
        LC_Cmd_HandleRelationRequest ncb = (LC_Cmd_HandleRelationRequest)obj;
        //处理结果
        tagRelationRequestHandleResultEvent pEvent = new tagRelationRequestHandleResultEvent(ncb.Info, ncb.Result);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
    bool OnHandleDelRelationRequest(NetCmdBase obj)
    {
        LC_Cmd_DelRelationRequest ncb = (LC_Cmd_DelRelationRequest)obj;
        m_RequestMap.Remove(ncb.ID);

        tagRelationRequestChangeEvent pEvent = new tagRelationRequestChangeEvent();
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}