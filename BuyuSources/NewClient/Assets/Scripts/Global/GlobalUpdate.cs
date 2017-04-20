using UnityEngine;
using System.Collections.Generic;

//全局更新,只有必要更新放此
public class GlobalUpdate
{
    public static void GlobalInit()
    {
        FTPClient.Instance.GlobalInit();
        NetServices.Instance.GlobalInit();
        GlobalEffectMgr.Instance.GlobalInit();
        GlobalAudioMgr.Instance.GlobalInit();
        GlobalLoading.Instance.GlobalInit();
        //PlayerManager.Instance.GlobalInit();
        PlayerRole.Instance.OnInit();
        
    }
    public static void PreUpdate(float delta)
    {

    }
    public static void LateUpdate(float delta)
    {
        //网络命令的处理须在主Update之后，否则主Update会重复的Update新加入的鱼群，导致delta加两次。
        NetServices.Instance.Update(delta);
        FTPClient.Instance.Update(delta);
        GlobalEffectMgr.Instance.Update(delta);
        GlobalAudioMgr.Instance.Update(delta);
        //PlayerManager.Instance.Update(delta);
        GlobalHallUIMgr.Instance.Update(delta);
       // PlayerRole.Instance.UpdateByMin();
        HeaderManager.Instance.Update(delta);
        GlobalLoading.Instance.Update(delta);
        if (SceneRuntime.Instance != null)
            SceneRuntime.SceneLogic.LateUpdate(delta);
        SceneObjMgr.Instance.Update(delta);
    }
    //逻辑改变,清空全局特效和音效
    public static void OnLogicChange()
    {
        GlobalAudioMgr.Instance.Clear();
        GlobalEffectMgr.Instance.Clear();
    }
}
