using UnityEngine;
using System.Collections.Generic;

public class Announcement
{
    public static void GetAnnouncement()
    {
        string pn = SDKMgr.PACKAGE_NAME;
        if (pn.Length > 0)
            pn = "_" + pn;
        string file = ServerSetting.ResFtp + "/Announcement/" + RuntimeInfo.GetPlatformString() + pn +".xml";
        FTPClient.Instance.AddDownload(DownloadType.DOWNLOAD_ANNOUNCEMENT, file, null);
    }
}
