using UnityEngine;
using System.Collections.Generic;

public class HallLoginUI_MatchGuide : BaseWnd
{
    Object              m_Obj = null;
    GameObject[]        m_ChildWnd = new GameObject[3];
    UILabel             m_UIMsgLabel;
    Animator[]            m_LockAnim = new Animator[2];
    byte                m_IsMatchGuide = 1;
    float               m_AnimTime;

	public void InitObj()
    {
        m_Obj = ResManager.Instance.LoadObject("Ef_JingsaiYindao", "HallRes/Prefab/Effect/UIJingsaijiesuo/", ResType.HallRes);
    }
    void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(m_Obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_BaseWndObject.SetActive(true);

        for (byte i = 0; i < m_ChildWnd.Length; ++i )
        {
            m_ChildWnd[i] = m_BaseTrans.GetChild(i).gameObject;
            if (i == 0)
            {
                for (byte j = 0; j < 2; ++j)
                {
                    UIEventListener.Get(m_BaseTrans.GetChild(i).GetChild(j).gameObject).onClick = OnCloseBtn;
                }
                m_UIMsgLabel = m_BaseTrans.GetChild(i).GetChild(2).GetComponent<UILabel>();
            }
            else if (i == 1)
            {
                UIEventListener.Get(m_ChildWnd[i]).onClick = OnOpenLock;
                for (byte j = 0; j < 2; ++j)
                    m_LockAnim[j] = m_BaseTrans.GetChild(i).GetChild(j).GetComponent<Animator>();
            }
            else 
                UIEventListener.Get(m_ChildWnd[i]).onClick = OnEnteryRoom;
        }

       WndManager.Instance.Push(m_BaseTrans);

    }
    public void Update(float deltal)
    {
        if (m_BaseWndObject == null || m_BaseWndObject.activeSelf != true)
            return;
        if (m_AnimTime > 0)
        {
            m_AnimTime -= deltal;
            if (m_AnimTime <= 0)
            {
                HallRunTimeInfo.Login_UI.MainWind.SetLeftCenterBtn(false);
                m_ChildWnd[2].SetActive(true);
            }
        }
    }
    public void ShowMatchGuide()
    {
        if (m_IsMatchGuide == 0)
            return;
        if (m_BaseWndObject == null)
            Init();
        m_UIMsgLabel.text = StringTable.GetString("Match_OpenLock_Success");
    }
    public bool GetMathGuide()
    {
        LoadGuide();
        if (m_IsMatchGuide > 0)
        {
            ShowMatchGuide();
            return true;
        }

        return false;
    }
    void OnCloseBtn(GameObject go)
    {
        m_ChildWnd[0].SetActive(false);
        m_ChildWnd[1].SetActive(true);
    }
    void OnOpenLock(GameObject go)
    {
        if (m_ChildWnd[2].activeSelf)
            return;
        HallRunTimeInfo.Login_UI.MainWind.OpenRightLock() ;
        for (byte i = 0; i < 2; ++i)
        {
            m_LockAnim[i].SetBool("LockClick", true);
        }
        m_AnimTime = 1.5f;
    }
    void OnEnteryRoom(GameObject go)
    {
        HallRunTimeInfo.Login_UI.MainWind.SetLeftCenterBtn(true);
        m_IsMatchGuide = 0;
        SaveGuide();
        DestorySelf();
        HallRunTimeInfo.Login_UI.ChangeHallWind(HallLogicUIStatue.Hall_State.Hall_Contest);
        HallRunTimeInfo.Login_UI.ShowMainWindCenterInf(false);
    }
    void DestorySelf()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseTrans);
            GameObject.Destroy(m_BaseWndObject);
        }
    }
    public void ShutDown()
    {
        if (m_BaseWndObject  != null)
            GameObject.Destroy(m_BaseWndObject);
        ResManager.Instance.UnloadObject(m_Obj);
    }
    public bool LoadGuide()
    {
        //读取SD卡的潜艇位置
        byte[] data = RuntimeInfo.GetLocalFile(RuntimeInfo.GetMatchGuideFileName());
        if (data == null)
            return false;

        System.IO.MemoryStream ms = new System.IO.MemoryStream(data);
        System.IO.BinaryReader br = new System.IO.BinaryReader(ms);
        m_IsMatchGuide = br.ReadByte();
        uint UserID = br.ReadUInt32();
        ms.Close();
        br.Close();
        if (UserID != PlayerRole.Instance.RoleInfo.RoleMe.GetUserID())
            m_IsMatchGuide = 1;
        return true;
    }
    //位置保存到SDK里
    public void SaveGuide()
    {
        //byte data;
        System.IO.MemoryStream ms = new System.IO.MemoryStream();
        System.IO.BinaryWriter bw = new System.IO.BinaryWriter(ms);
        bw.Write(m_IsMatchGuide);
        bw.Write(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID());
        RuntimeInfo.SaveLocalFile(RuntimeInfo.GetMatchGuideFileName(), ms.ToArray());
        bw.Close();
    }
}
