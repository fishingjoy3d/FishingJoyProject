using System.IO;
using System.Collections;
using UnityEngine;
using System.Collections.Generic;
using System;

public class ResourceProcess : Singleton<ResourceProcess>
{
    public void AddDownloadFile(DownloadType dt, string fileURL)
    {
        SceneMain.Instance.StartInnerCoroutine(DownloadFileFromRemote(dt, fileURL));
    }
    private IEnumerator DownloadFileFromRemote(DownloadType dt, string fileURL)
    {
        DownloadData dd = new DownloadData(null, null, dt);
        dd.DownState = DownloadState.DS_DOWNLOADING;

        WWW w = new WWW(fileURL);
        yield return w;

        if (w.isDone && w.error == null)
        {
            try
            {
                dd.DownState = DownloadState.DS_DOWNLOADED_OK;
                dd.SetBytes(System.Text.Encoding.UTF8.GetBytes(w.text));
                w.Dispose();
            }
            catch
            {
                dd.DownState = DownloadState.DS_DOWNLOADED_ERROR;
            }
        }
        else
        {
            dd.DownState = DownloadState.DS_DOWNLOADED_ERROR;
        }
        DownloadHandler.Handle(dd);
    }

}