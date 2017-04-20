using UnityEngine;
using System.Collections.Generic;

class InitLogicUI:ILogicUI
{
    InitLogic m_Loigc;
    public bool Init(ILogic logic, object obj)
    {
        m_Loigc = (InitLogic)logic;
        //SceneMain.Instance.LogStr = "";
        SceneObjMgr.Instance.LoadInnerUpdateBackMat();
        GlobalLoading.Instance.Show();
        return true;
    }
    public void OnGUI(float delta)
    {
        return;
    }
    public void Update(float delta)
    {
        
    }
    public void Shutdown()
    {
       
    }

}
