using UnityEngine;
using System.Collections.Generic;

public class SceneMatchGuide :BaseWnd
{
    Object      m_obj;
    UILabel     m_UIDescLable;
    byte        m_bNeedGuide = 0;
    public void InitObj()
    {
        m_obj = ResManager.Instance.LoadObject("Scen_MatchGuide_Wnd", "SceneRes/Prefab/UI/MatchGuide/", ResType.SceneRes);
    }
    public void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(m_obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_UIDescLable = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = LevelScene;
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = LevelScene;
        
        m_bNeedGuide = 1;
        SaveSceneMatchGuide();
        WndManager.Instance.Push(m_BaseTrans);
    }
    public void ShowSceneMatchGuide()
    {
        LoadSceneMatchGuide();

        if (m_bNeedGuide == 0)
        {
            if (m_BaseWndObject != null)
                return;
            Init();
            m_UIDescLable.text = StringTable.GetString("Match_Scene_Guide");

        }
        
    }
    void LevelScene(GameObject go)
    {
        GlobalHallUIMgr.Instance.GameShare.SkipShow();
        SceneRuntime.SceneLogic.BackToHall();
        //int bg = Random.Range(4, 7);
        int bg = 1;
        GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound2);

        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseTrans);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseTrans);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
        ResManager.Instance.UnloadObject(m_obj);
    }
    public bool LoadSceneMatchGuide()
    {
        //读取SD卡
        byte[] data = RuntimeInfo.GetLocalFile(RuntimeInfo.GetSceneMathcGideFileName());
        if (data == null)
            return false;

        System.IO.MemoryStream ms = new System.IO.MemoryStream(data);
        System.IO.BinaryReader br = new System.IO.BinaryReader(ms);
        m_bNeedGuide = br.ReadByte();
        uint UserID = br.ReadUInt32();
        ms.Close();
        br.Close();
        if (UserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
           m_bNeedGuide = 0;
        return true;
    }
    //位置保存到SDK里
    public void SaveSceneMatchGuide()
    {
        //byte data;
        System.IO.MemoryStream ms = new System.IO.MemoryStream();
        System.IO.BinaryWriter bw = new System.IO.BinaryWriter(ms);
        bw.Write(m_bNeedGuide);
        bw.Write(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
        RuntimeInfo.SaveLocalFile(RuntimeInfo.GetSceneMathcGideFileName(), ms.ToArray());
        bw.Close();
    }
}
