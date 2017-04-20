using UnityEngine;
using System.Collections;

public enum MssageType
{
    PayScuess_Mag_0 = 0, //充值成功消息
    GameRoom_Msg_1,         //进入房间提示1
    GameRoom_Msg_2,         //进入房间提示2 
    GameRoom_Msg_3,         //进入房间倍率不足
    GoldLack_Msg_4,         //金币不足提示框
    Exit_Msg_5,             //退出确认框
    BeneFit_Msg_6,      //救济金消息
    VIP_UnLock_Msg_7,   //VIP解锁
    NiuNiu_Exit_Tips,    //牛牛强退提示
    NiuNiu_UpHostFail_Tips,  //牛牛上庄失败提示
    NiuNiu_SantchHost_Tips,  //抢庄扣除金币提示
    Del_Friend_Msg,         //删除好友确认框
    ForestDance_Exit_Tips,   //森林舞会强退
    ForestDance_SnatchHost_Tips, //森林舞会抢庄扣除金币提示
    Car_SantchHost_Tips, //车抢庄
    Car_Exit_Tips,//强退
}


public class HallMessage : HallLoginUI_BaseWind
{
    GameObject[]    m_MessageWind = new GameObject[2];
    GameObject[]    m_ChildMsgObj = new GameObject[3];
    GameObject      m_MaskObj;
    UILabel[]       m_MsgBoxLabel = new UILabel[3];
    TweenScale      m_NormalPopAnim;               //比赛祥情弹出动画

    MssageType      m_MsgType;
    PayType         m_Paytype;
    bool            MessageLoading = false;
    byte            m_RoomID;
    uint            m_ConsumeSum;           //解锁到目标倍率所需要的消耗数
    uint            m_DesRateIdx;
    uint            m_UserID;

    public new void Init()
    {
        if (WindObj != null)
            return;
        Object obj = Resources.Load("InnerRes/ui/Inner_MessageWind");
        
        SetParent(((GameObject)GameObject.Instantiate(obj)).transform);
        InitLoading();
        if (WindObj.activeSelf) 
            WindObj.SetActive(false);
        InitBoxMessage();
    }
    void InitLoading()
    {
        Object obj = Resources.Load("InnerRes/Effect/Pregress_Bar_Anim");

        m_MessageWind[0] = GameObject.Instantiate(obj) as GameObject;
        m_MessageWind[0].transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_MessageWind[0].SetActive(false);
    }
    void InitBoxMessage()
    {
        m_MessageWind[1] = BaseTranF.GetChild(1).gameObject;
        m_MaskObj = BaseTranF.GetChild(2).gameObject;
        for (byte j = 0; j < m_MsgBoxLabel.Length; ++j )
        {
            m_MsgBoxLabel[j] = BaseTranF.GetChild(1).GetChild(j).GetChild(0).GetComponent<UILabel>() ;
            UIEventListener.Get(BaseTranF.GetChild(1).GetChild(j).GetChild(1).gameObject).onClick = OnClickClose;
        }

        for (byte i = 0; i < m_ChildMsgObj.Length; ++i)
        {
            m_ChildMsgObj[i] = BaseTranF.GetChild(1).GetChild(i).gameObject;
        }
        m_NormalPopAnim = m_MessageWind[1].GetComponent<TweenScale>();
        UIEventListener.Get(BaseTranF.GetChild(1).GetChild(0).GetChild(2).gameObject).onClick = OnckeckBtnOk;
        UIEventListener.Get(BaseTranF.GetChild(1).GetChild(1).GetChild(2).gameObject).onClick = OnClickBuyGold;
        UIEventListener.Get(BaseTranF.GetChild(1).GetChild(1).GetChild(3).gameObject).onClick = OnClickRecommendRoom;
        UIEventListener.Get(BaseTranF.GetChild(1).GetChild(2).GetChild(2).gameObject).onClick = OnOKExit;
        UIEventListener.Get(BaseTranF.GetChild(1).GetChild(2).GetChild(3).gameObject).onClick = OnClickClose;


    }
    public bool ShowLoadingMessage()
    {

        if (WindObj == null)
            return false;
        else if (MessageLoading == false)
        {
            MessageLoading = true;
            if (!WindObj.activeSelf) 
                WindObj.SetActive(true);
            if (!m_MessageWind[0].activeSelf) 
                m_MessageWind[0].SetActive(true);

            if (m_MessageWind[1].activeSelf)
            {
                WndManager.Instance.Pop(BaseTranF);
                m_MaskObj.SetActive(false);
                m_MessageWind[1].SetActive(false);
            }
         
            return true;
        }
        else
            return true;
    }
    public void ShowMessageBox(string strMsg, MssageType type, byte RoomID = 0, uint ExData = 0)
    {
        if (WindObj == null) 
            return ;
        if (m_ChildMsgObj[2].activeSelf == true && type == MssageType.Exit_Msg_5)
        {
            ShutDown();
            return;
        }
        for (byte i = 0; i < m_ChildMsgObj.Length; ++i )
        {
            if (m_ChildMsgObj[i].activeSelf == true)
                ShutDown();
        }
        WndManager.Instance.Push(BaseTranF);
        m_MsgType = type;
        m_RoomID = RoomID;
        WindObj.SetActive(true);
        m_MaskObj.SetActive(true);
        m_MessageWind[1].SetActive(true);
        m_NormalPopAnim.ResetToBeginning();
        m_NormalPopAnim.PlayForward();
        //for (byte i = 0; i < m_ChildMsgObj.Length; ++i)
        //{
        //    m_ChildMsgObj[i].SetActive(false);
        //}

        switch (type)
        {
            case MssageType.PayScuess_Mag_0:                 //
                {
                    m_ChildMsgObj[0].SetActive(true);
                    m_MsgBoxLabel[0].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }             
            case MssageType.GameRoom_Msg_1:       
                {
                    m_ChildMsgObj[0].SetActive(true);
                    m_MsgBoxLabel[0].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }
              
            case MssageType.GameRoom_Msg_2:   
                {
                    m_ChildMsgObj[1].SetActive(true);
                    m_MsgBoxLabel[1].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }
               
            case MssageType.GameRoom_Msg_3:
                {
                    m_ChildMsgObj[0].SetActive(true);
                    GetDstUnlockRateConsume(strMsg);
                    break;
                }               
            case MssageType.GoldLack_Msg_4:
                {
                    m_ChildMsgObj[0].SetActive(true);
                    string strDec = "";
                    if (strMsg == "金币")
                    {
                        strDec = string.Format(StringTable.GetString("Ec_RoleCharm_GlodNotEnough"), strMsg);
                        m_Paytype = PayType.Gold;
                    }
                    else if (strMsg == "钻石")
                    {
                        strDec = string.Format(StringTable.GetString("Ec_RoleCharm_GlodNotEnough"), strMsg);
                        m_Paytype = PayType.Diamond;
                    }
                    m_MsgBoxLabel[0].text = "[242858FF]" + strDec + "[-]";
                    break;
                }               
            case MssageType.Exit_Msg_5:
                {
                    m_ChildMsgObj[2].SetActive(true);
                    m_MsgBoxLabel[2].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }               
            case MssageType.BeneFit_Msg_6:
                {
                    m_ChildMsgObj[0].SetActive(true);
                    m_MsgBoxLabel[0].text = strMsg;
                    break;
                }              
            case MssageType.VIP_UnLock_Msg_7:
                {
                    m_ChildMsgObj[0].SetActive(true);
                    m_MsgBoxLabel[0].text = strMsg;
                    break;
                }                
            case MssageType.NiuNiu_Exit_Tips:
                {
                    m_ChildMsgObj[2].SetActive(true);
                    m_MsgBoxLabel[2].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }               
            case MssageType.NiuNiu_UpHostFail_Tips:
                {
                    m_ChildMsgObj[2].SetActive(true);
                    m_MsgBoxLabel[2].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }              
            case MssageType.NiuNiu_SantchHost_Tips:
                {
                    m_ChildMsgObj[0].SetActive(true);
                    m_MsgBoxLabel[0].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }                
            case MssageType.Del_Friend_Msg:
                {
                    m_UserID = ExData;
                    m_ChildMsgObj[0].SetActive(true);
                    m_MsgBoxLabel[0].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }              
            case MssageType.ForestDance_SnatchHost_Tips:
                {
                    m_ChildMsgObj[0].SetActive(true);
                    m_MsgBoxLabel[0].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }                
            case MssageType.ForestDance_Exit_Tips:
                {
                    m_ChildMsgObj[2].SetActive(true);
                    m_MsgBoxLabel[2].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }
            case MssageType.Car_SantchHost_Tips:
                {
                    m_ChildMsgObj[0].SetActive(true);
                    m_MsgBoxLabel[0].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }
            case MssageType.Car_Exit_Tips:
                {
                    m_ChildMsgObj[2].SetActive(true);
                    m_MsgBoxLabel[2].text = "[242858FF]" + strMsg + "[-]";
                    break;
                }
               
        }

    }
    public void Update(float dTime)
    {
        //if(MessageLoading)
        //{
        //    if (m_LoadingPropress != null)
        //    {
        //        m_LoadingPropress.transform.Rotate(0, 0, m_PropressSpeed * 0.02f);
        //    }
        //}       
    }
    public void ShutDown(HallMessage.MessageType type)
    {
        if (type == MessageType.loading)
        {
            MessageLoading = false;
            if (m_MessageWind[0].activeSelf) 
                m_MessageWind[0].SetActive(false);
        }
        else
            if (m_MessageWind[1].activeSelf) 
                m_MessageWind[1].SetActive(false);

    }
    void GetDstUnlockRateConsume(string strMsg)
    {
        m_DesRateIdx = uint.Parse(strMsg);
        uint desRate = BulletSetting.BulletRate[m_DesRateIdx];
        uint curIdx = PlayerRole.Instance.RoleInfo.GetRoleUnLockRateIndex();
        uint curRate = 0;
        m_ConsumeSum = 0;
        if (curIdx != 0)
        {
            curRate = BulletSetting.BulletRate[curIdx - 1];
            for (uint i = curIdx; i <= m_DesRateIdx; ++i)
            {
                m_ConsumeSum += BulletSetting.RateUnlock[i];
            }
        }
        else
            curRate = 0;
        string strDec = string.Format(StringTable.GetString("join_room_failed3"), desRate, curRate, m_ConsumeSum, desRate);
        m_MsgBoxLabel[0].text = "[242858FF]" + strDec + "[-]";

    }
    void OnckeckBtnOk(GameObject go)
    {
        ShutDown();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        switch(m_MsgType)
        {
            case MssageType.PayScuess_Mag_0:

                break;
            case MssageType.GameRoom_Msg_1:
                {
                    byte roomid = PlayerRole.Instance.TableManager.ConvertTableID(m_RoomID);
                    if (roomid == m_RoomID)
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("join_scene_error"), 2, false);
                    else
                    {
                        HallRunTimeInfo.Instance.m_login.JoinRoom(roomid);
                    }
                }
                break;
            case MssageType.GameRoom_Msg_2:
                break;
            case MssageType.GameRoom_Msg_3:
                {
                    if (m_ConsumeSum > PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency())
                        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Diamond);
                    else
                    {
                        //暂时用这个
                        PlayerRole.Instance.RoleInfo.ChangeRoleRateValue((byte)m_DesRateIdx);
                    }
                }
                break;
            case MssageType.GoldLack_Msg_4:
                GlobalHallUIMgr.Instance.ShowPayWnd(m_Paytype);
                break;
            case MssageType.VIP_UnLock_Msg_7:
                GlobalHallUIMgr.Instance.ShowVipWnd();
                break;
            case MssageType.NiuNiu_SantchHost_Tips:
                GlobalHallUIMgr.Instance.NiuniuSnatchHost();
                break;
            case MssageType.Car_SantchHost_Tips:
                {
                    GlobalHallUIMgr.Instance.CarSnatchHost();
                    break;
                }
            case MssageType.Del_Friend_Msg:
                PlayerRole.Instance.RelationManager.DelRelation(m_UserID);
                break;
            case MssageType.ForestDance_SnatchHost_Tips:
                PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnRoleGetWriteBankerFirstSeat();
                break;
        }
    }
    void OnClickBuyGold(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Gold);
        ShutDown();
    }
    void OnClickRecommendRoom(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        ShutDown();
        byte roomid = PlayerRole.Instance.TableManager.ConvertTableID(m_RoomID);
        if (roomid == m_RoomID)
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("join_scene_error"), 2, false);
        else
        {
            HallRunTimeInfo.Instance.m_login.JoinRoom(roomid);
        }
    }
    void OnClickClose(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        ShutDown();

    }
    void OnOKExit(GameObject go)
    {
        if (m_MsgType == MssageType.NiuNiu_UpHostFail_Tips)
        {
            ShutDown();
            GlobalHallUIMgr.Instance.ShowPayWnd(PayType.Gold);
            return;
        }

        if (m_MsgType == MssageType.ForestDance_Exit_Tips)
        {
            PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnLeaveDialTable();
            GlobalHallUIMgr.Instance.ShowBackToMain();
            GlobalHallUIMgr.Instance.ShutForset();
            ShutDown();
            return;  
        }

        if (m_MsgType == MssageType.NiuNiu_Exit_Tips)
        {
            PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnLeaveNiuNiuTable();        
            GlobalHallUIMgr.Instance.NiuniuShutDown();
            ShutDown();
            return;            
        }

        if (m_MsgType == MssageType.Car_Exit_Tips)
        {
            PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnLeaveCarTable();
            GlobalHallUIMgr.Instance.CarShutDown();
            ShutDown();
            return;
        }

        if (SceneRuntime.Instance != null)
        {
            SceneRuntime.SceneLogic.BackToHall();
            ShutDown();
            return;
        }
        
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
        SDKMgr.Instance.ReleaseResource();
#endif
    }
    public void ShutDownMsgBox()
    {
        if (m_MessageWind[1] != null && m_MessageWind[1].activeSelf)
            ShutDown();
    }
    void ShutDown()
    {
        if(WindObj != null && WindObj.activeSelf)
        {
            for (byte i = 0; i < m_ChildMsgObj.Length; ++i)
            {
                m_ChildMsgObj[i].SetActive(false);
            }
            WndManager.Instance.Pop(BaseTranF);
            WindObj.SetActive(false);
            if (m_MaskObj.activeSelf)
                m_MaskObj.SetActive(false);
        }
    }
    public enum MessageType
    { 
        loading = 0,
        MessageWind,
    }
}
