using UnityEngine;
using System.Collections.Generic;

public class CBugleWnd
{
    GameObject m_gobugle;
    UILabel m_labelmessage;
    public void Show()
    {
        m_gobugle = (GameObject)GameObject.Instantiate(GlobalHallUIMgr.Instance.ShopSysObj[7]);
        m_gobugle.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);     

        m_labelmessage = m_gobugle.transform.GetChild(2).GetChild(0).GetChild(0).GetComponent<UILabel>();
        UIEventListener.Get(m_gobugle.transform.GetChild(0).gameObject).onClick = EventClose;
        UIEventListener.Get(m_gobugle.transform.GetChild(1).gameObject).onClick = EventUse;
        
        WndManager.Instance.Push(m_gobugle);
    }
    void EventClose(GameObject go)
    {
        ShutDown();
    }
    void EventUse(GameObject go)
    {
        if(m_labelmessage.text.Length!=0)
        {
            PlayerRole.Instance.RoleMessage.SendMessageToServer(m_labelmessage.text, 16776960, 2, 1);
        }
        ShutDown();   
    }
    void ShutDown()
    {       
        WndManager.Instance.Pop(m_gobugle);
        GameObject.Destroy(m_gobugle);
    }
}