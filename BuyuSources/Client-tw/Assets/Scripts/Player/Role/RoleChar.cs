using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//玩家聊天的类
public class tagRoleCharArray
{
    public UInt32 SrcUserID;
    public List<tagRoleCharInfo> Array = new List<tagRoleCharInfo>();
}
class RoleChar
{
    public bool m_IsLoadList = false;
    public Dictionary<UInt32, tagRoleCharInfo> m_CharList = new Dictionary<UInt32, tagRoleCharInfo>();

    public Dictionary<UInt32, tagRoleCharArray> m_CharArrayList = new Dictionary<UInt32, tagRoleCharArray>();
    public Dictionary<UInt32, tagRoleCharInfo> GetCharList()
    {
        if(m_IsLoadList)
        {
            return m_CharList;
        }
        else
        {
            LoadDBCharList();
            return null;
        }
    }
    public void OnClear()
    {
        m_CharList.Clear();
        m_CharArrayList.Clear();
        m_IsLoadList = false;
    }
    public void ResetInfo()
    {
        OnClear();
        //产生事件
        tagCharListChange pEvent = new tagCharListChange();
        MsgEventHandle.HandleMsg(pEvent);

        tagCharContextChange pEvent2 = new tagCharContextChange(0);
        MsgEventHandle.HandleMsg(pEvent2);
    }

    public bool SendMessaggChar(UInt32 DestUserID,string MessageInfo)
    {
        //发送聊天数据到指定玩家去
        if(!PlayerRole.Instance.RelationManager.IsExitesRelation(DestUserID))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RoleChar_SendChar_Failed_1);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        if (!FishConfig.Instance.m_ErrorString.CheckStringIsError(1, FishDataInfo.MAX_ROLECHAR_LENGTH, MessageInfo, StringCheckType.SCT_Normal))
        {
            tagUserOperationEvent pUOM = new tagUserOperationEvent(UserOperateMessage.UOM_RoleChar_SendChar_Failed_2);
            MsgEventHandle.HandleMsg(pUOM);
            return false;
        }
        CL_Cmd_SendCharInfo ncb = new CL_Cmd_SendCharInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_SendCharInfo);
        ncb.MessageInfo = new tagRoleCharInfo();
        ncb.MessageInfo.SrcUserID = PlayerRole.Instance.RoleInfo.RoleMe.GetUserID();
        ncb.MessageInfo.DestUserID = DestUserID;
        ncb.MessageInfo.MessageInfo = MessageInfo;
        NetServices.Instance.Send<CL_Cmd_SendCharInfo>(ncb);
        return true;
    }
    private void LoadDBCharList()
    {
        //加载数据库的聊天的列表数据
        CL_Cmd_LoadAllCharInfo ncb = new CL_Cmd_LoadAllCharInfo();
        ncb.SetCmdType(NetCmdType.CMD_CL_LoadAllCharInfo);
        NetServices.Instance.Send<CL_Cmd_LoadAllCharInfo>(ncb);
    }
    public bool LoadAllCharInfoByUserID(UInt32 SrcUserID)
    {
        if (!PlayerRole.Instance.RelationManager.IsExitesRelation(SrcUserID))
        {

            return false;
        }
        CL_Cmd_LoadCharListByUserID ncb = new CL_Cmd_LoadCharListByUserID();
        ncb.SetCmdType(NetCmdType.CMD_CL_LoadCharListByUserID);
        ncb.SrcUserID = SrcUserID;
        NetServices.Instance.Send<CL_Cmd_LoadCharListByUserID>(ncb);
        return true;
    }
    public void ClearCharInfo(UInt32 DestUserID)
    {
        m_CharList.Remove(DestUserID);
        m_CharArrayList.Remove(DestUserID);
    }

    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_LoadAllCharInfo:
                return HandleLoadAllCharInfo(obj);
            case NetCmdType.CMD_LC_LoadCharListByUserID:
                return HandleLoadCharInfoByUserID(obj);
            case NetCmdType.CMD_LC_SendCharInfo:
                return HandleAddCharInfo(obj);
        }
        return true;
    }

    bool HandleLoadAllCharInfo(NetCmdBase obj)
    {
        LC_Cmd_LoadAllCharInfo ncb = (LC_Cmd_LoadAllCharInfo)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            m_CharList.Clear();
        }
        for (int i = 0; i < ncb.Sum; ++i)
        {
            m_CharList.Add(ncb.Array[i].SrcUserID, ncb.Array[i]);
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            m_IsLoadList = true;
           
            //产生事件
            tagCharListChange pEvent = new tagCharListChange();
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleLoadCharInfoByUserID(NetCmdBase obj)
    {
        LC_Cmd_LoadCharListByUserID ncb = (LC_Cmd_LoadCharListByUserID)obj;
        if ((ncb.States & FishDataInfo.MsgBegin) != 0)
        {
            m_CharArrayList.Clear();
        }
        for (int i = 0; i < ncb.Sum; ++i)
        {
            if (m_CharArrayList.ContainsKey(ncb.Array[i].SrcUserID))
            {
                m_CharArrayList[ncb.Array[i].SrcUserID].Array.Add(ncb.Array[i]);
            }
            else
            {
                tagRoleCharArray pArray = new tagRoleCharArray();
                pArray.SrcUserID = ncb.Array[i].SrcUserID;
                pArray.Array.Add(ncb.Array[i]);
                m_CharArrayList.Add(ncb.Array[i].SrcUserID, pArray);
            }
        }
        if ((ncb.States & FishDataInfo.MsgEnd) != 0)
        {
            //产生事件
            m_CharList.Remove(ncb.SrcUserID);//已经获取了全部的数据 无须缓存列表了
            tagCharContextChange pEvent = new tagCharContextChange(ncb.SrcUserID);
            MsgEventHandle.HandleMsg(pEvent);
        }
        return true;
    }
    bool HandleAddCharInfo(NetCmdBase obj)
    {
        LC_Cmd_SendCharInfo ncb = (LC_Cmd_SendCharInfo)obj;
        if(m_CharList.ContainsKey(ncb.MessageInfo.SrcUserID))
        {
            m_CharList[ncb.MessageInfo.SrcUserID] = ncb.MessageInfo;
        }
        else
        {
            tagRoleCharInfo pInfo = new tagRoleCharInfo();
            pInfo.DestUserID = ncb.MessageInfo.DestUserID;
            pInfo.LogTime = ncb.MessageInfo.LogTime;
            pInfo.MessageInfo = ncb.MessageInfo.MessageInfo;
            pInfo.SrcUserID = ncb.MessageInfo.SrcUserID;
            m_CharList.Add(ncb.MessageInfo.SrcUserID, pInfo);
        }
        if (m_CharArrayList.ContainsKey(ncb.MessageInfo.SrcUserID))
        {
            m_CharArrayList[ncb.MessageInfo.SrcUserID].Array.Add(ncb.MessageInfo);
        }
        else
        {
            tagRoleCharArray pInfo = new tagRoleCharArray();
            pInfo.SrcUserID = ncb.MessageInfo.SrcUserID;
            pInfo.Array.Add(ncb.MessageInfo);
            m_CharArrayList.Add(ncb.MessageInfo.SrcUserID, pInfo);
        }

        //产生事件
        tagCharAddCharEvent pEvent = new tagCharAddCharEvent(ncb.MessageInfo);
        MsgEventHandle.HandleMsg(pEvent);
        return true;
    }
}