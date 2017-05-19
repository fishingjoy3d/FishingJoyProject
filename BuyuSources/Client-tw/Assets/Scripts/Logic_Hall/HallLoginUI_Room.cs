using UnityEngine;
using System.Collections;

public class HallLoginUI_Room : HallLoginUI_BaseWind
{
    ViewInf m_CenterViewInf = new ViewInf();
    RoomInF[] m_RoomInf;
    public Object Init()
    {
        Object obj = Initobj("HallRoomWind");
        WindID.m_state = HallLogicUIStatue.Hall_State.Hall_SelectRoom;
        WindID.m_SmailWind = 0;
        return obj;
    }
    public void InitGameObj()
    {
        Init_GameObj();
        InitCenterView();
    }
    void InitCenterView()
    {
        m_CenterViewInf.m_grid = BaseTranF.GetChild(0).GetChild(0).GetComponent<UIGrid>();
        m_CenterViewInf.m_BaseChild = BaseTranF.GetChild(0).GetChild(1).gameObject;
        m_RoomInf = new RoomInF[4];
        for (byte i = 0; i < m_RoomInf.Length; ++i)
        {
            GameObject obj = GridInstantiateObj(m_CenterViewInf.m_grid, m_CenterViewInf.m_BaseChild);
            m_RoomInf[i].m_Btn.m_BtnObj = obj;
            m_RoomInf[i].m_Btn.m_Btn = m_RoomInf[i].m_Btn.m_BtnObj.GetComponent<UIButton>();
            m_RoomInf[i].m_Btn.m_IsChecked = false;
            m_RoomInf[i].m_Btn.m_Tag = i;
            ChangeBtnName(m_RoomInf[i].m_Btn.m_Btn, ("Hall_Room" + i.ToString()));
            GetBtnLister(m_RoomInf[i].m_Btn.m_BtnObj).onClick = OnCheckRoomBtn;
            m_RoomInf[i].m_NumLabel = obj.transform.GetChild(1).GetChild(0).GetComponent<UILabel>();
        }
        if (m_CenterViewInf.m_BaseChild.activeSelf) m_CenterViewInf.m_BaseChild.SetActive(false);
        //SetOnLinePeopleNum();
    }
    public void SetOnLinePeopleNum()
    {
        for (byte i = 0; i < m_RoomInf.Length; ++i)
        {
            m_RoomInf[i].m_NumLabel.text = (1212 + i * 3).ToString();
        }
    }
    void OnCheckRoomBtn(GameObject go)
    {
        PlayBtnMusic();
        byte checktag = 0;
        for (byte i = 0; i < m_RoomInf.Length; ++i)
        {
            if (m_RoomInf[i].m_Btn.m_BtnObj == go)
            {
                checktag = m_RoomInf[i].m_Btn.m_Tag;
                m_RoomInf[i].m_Btn.m_IsChecked = true;
            }
            else
                m_RoomInf[i].m_Btn.m_IsChecked = false;
        }
        if (FishConfig.Instance.m_TableInfo.m_TableConfig.ContainsKey(ExtraSetting.RoomDataList[checktag].RoomID))
        {
            HallRunTimeInfo.Instance.m_login.JoinRoom(ExtraSetting.RoomDataList[checktag].RoomID);
            
        }
    }
    public void Update(float dTime)
    { }
    struct RoomInF
    {
        public WindBtn m_Btn;
        public UILabel m_NumLabel;
    }
}
