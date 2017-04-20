using UnityEngine;
using System.Collections.Generic;

public class HallLoginUI_CDKey : BaseWnd
{
    Object      m_Obj;
    UIInput     m_UIInput;
    public void InitObj()
    {
        m_Obj = ResManager.Instance.LoadObject("CDKey_Wnd", "HallRes/Prefab/UI/", ResType.HallRes);
    }
    void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(m_Obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_BaseWndObject.SetActive(true);
        m_UIInput = m_BaseTrans.GetChild(2).GetComponent<UIInput>();
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = OnCloseWnd;
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = OnOK;
    }
    public void ShowCDKeyWnd()
    {
        Init();
        WndManager.Instance.Push(m_BaseTrans);

    }
    void OnOK(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        if (m_UIInput!= null)
            PlayerRole.Instance.RoleExChange.OnUseExChangeCode(m_UIInput.value);
        //DestoryWnd();
    }
    void OnCloseWnd(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        DestoryWnd();
    }
    public void DestoryWnd()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseTrans);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
    public void ShutDown()
    {
        DestoryWnd();
        ResManager.Instance.UnloadObject(m_Obj);
    }
}
