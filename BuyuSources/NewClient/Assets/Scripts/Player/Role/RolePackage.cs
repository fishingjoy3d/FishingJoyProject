using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class RolePackage
{
    public void OnClear()
    {

    }
    //public bool OpenPackageItem(UInt32 PackageItemID)
    //{
    //    //打开一个礼包
    //    UInt32 ItemSum = PlayerRole.Instance.ItemManager.GetItemSum(PackageItemID);
    //    if(ItemSum<= 0)
    //    {
    //        tagErrorCodeEvent pCode = new tagErrorCodeEvent(ErrorCode.EC_Package_BagNonItem);
    //        MsgEventHandle.HandleMsg(pCode);
    //        return false;
    //    }
    //    //判断是否为礼包
    //    if(!FishConfig.Instance.m_FishPackageConfig.m_PackageMap.ContainsKey(PackageItemID))
    //    {
    //        tagErrorCodeEvent pCode = new tagErrorCodeEvent(ErrorCode.EC_Package_NonItem);
    //        MsgEventHandle.HandleMsg(pCode);
    //        return false;
    //    }
    //    CL_Cmd_OpenPackage ncb = new CL_Cmd_OpenPackage();
    //    ncb.SetCmdType(NetCmdType.CMD_CL_OpenPackage);
    //    ncb.PackageItemID = PackageItemID;
    //    NetServices.Instance.Send<CL_Cmd_OpenPackage>(ncb);
    //    return true;
    //}

    public bool HandleCmd(NetCmdBase obj)
    {
        switch (obj.GetCmdType())
        {
            case NetCmdType.CMD_LC_OpenPackage:
                return HandleOpenPackageResult(obj);
        }
        return true;
    }
    bool HandleOpenPackageResult(NetCmdBase obj)
    {
        LC_Cmd_OpenPackage ncb = (LC_Cmd_OpenPackage)obj;
        if (ncb.Result)
        {
            //成功了
            tagPackageResultEvent pEvent = new tagPackageResultEvent(true, ncb.RewardID);
            MsgEventHandle.HandleMsg(pEvent);

            return true;
        }
        else
        {
            tagPackageResultEvent pEvent = new tagPackageResultEvent(false, null);
            MsgEventHandle.HandleMsg(pEvent);

            return true;
        }
    }
}