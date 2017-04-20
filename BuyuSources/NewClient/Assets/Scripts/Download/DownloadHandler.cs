using UnityEngine;
using System.Collections.Generic;
using System;
using System.Xml;

public struct AnnouncementData
{
    public string Title;
    public string Date;
    public string Content;
}
public class DownloadHandler
{
    public static void Handle(DownloadData dd)
    {
        switch (dd.Type)
        {
            case DownloadType.DOWNLOAD_RANK_INFO:
                {
                    //下载普通的排行榜文件
                    if (dd.DownState == DownloadState.DS_DOWNLOADED_OK)
                    {
                        PlayerRole.Instance.RankManager.SaveRankXml(dd.Bytes);
                        PlayerRole.Instance.RankManager.DownLoadReadRankXml();
                    }
                    else if (dd.DownState == DownloadState.DS_DOWNLOADED_ERROR)
                    {
                        PlayerRole.Instance.RankManager.DownLoadReadRankXml(true);
                    }
                    return;
                }
            case DownloadType.DOWNLOAD_Month_Rank:
                {
                    if (dd.DownState == DownloadState.DS_DOWNLOADED_OK)
                    {
                        PlayerRole.Instance.MonthManager.SaveMonthRankXml(dd.Bytes, (string)dd.Data);//保存XML文件
                        PlayerRole.Instance.MonthManager.OnDownLoadMonthRankXml((string)dd.Data);
                    }
                    else if (dd.DownState == DownloadState.DS_DOWNLOADED_ERROR)
                    {
                        PlayerRole.Instance.MonthManager.OnDownLoadMonthRankXml((string)dd.Data, true);
                    }
                    return;
                }
            case DownloadType.DOWNLOAD_ANNOUNCEMENT:
                {
                    if(dd.DownState == DownloadState.DS_DOWNLOADED_OK)
                    {
                        XmlDocument doc = new XmlDocument();
                        doc.LoadXml(dd.Text);
                        XmlElement ele = doc.DocumentElement;
                        XmlNode title = ele.SelectSingleNode("items");
                        List<AnnouncementData> annoucesList = new List<AnnouncementData>();
                        for(int i = 0; i < title.ChildNodes.Count; ++i)
                        {
                            AnnouncementData ad = new AnnouncementData();
                            ad.Title = title.ChildNodes[i].Attributes["title"].Value;
                            ad.Date = title.ChildNodes[i].Attributes["date"].Value;
                            ad.Content = title.ChildNodes[i].Attributes["content"].Value;
                            annoucesList.Add(ad);
                        }
                        //获取公告列表
                        LogonRuntime.LogonLogicUI.UpdateNotice(annoucesList);
                    }
                    else
                    {
                        //获取公告失败。
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("UOM_Login_GetNoticeError"), 2.0f, false);
                    }
                    break;
                }
        }
    }
}
