using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public enum InfmationWind
{
        Wind_UserInF = 0,
        Wind_Achievement,
        Wind_Task,
        Wind_Rank,
        Wind_ChangeUserInf,
        Wind_ChangeUserHead,
        Wind_MaxChild = 6,
}

public class HallLoginUI_Infomation : HallLoginUI_BaseWind
{
    WindBtn[]                                   m_TopBtns = new WindBtn[4];

    GameObject[]                                m_TopBtnsCheckbg = new GameObject[4];  //顶部按纽选中状态
    GameObject[]                                m_SmailBg = new GameObject[3];//俩小黑背景
    GameObject[]                                m_InfoChildWndObj = new GameObject[(int)InfmationWind.Wind_MaxChild]; 
    WindBtn[]                                   m_ChangeUserInFBtns = new WindBtn[10];
    Transform[]                                 m_AccountMgrTrans = new Transform[5];
    UIInput                                     m_ChangeUserInFInPut = null;
    ChangeUserHeadInf[]                         m_changeUserHeadContentArray = new ChangeUserHeadInf[6];
    HallLoginUI_Rank                            m_RankUI = new HallLoginUI_Rank();
    HallLoginUI_Task                            m_TaskUI = new HallLoginUI_Task();
    HallLoginUI_Achievement                     m_AchievementUI = new HallLoginUI_Achievement();
    HallLoginUI_UserInfo                        m_UserInfoUI = new HallLoginUI_UserInfo();
    Transform[]                                 m_ShowHideBtnTrans = new Transform[2];      //显示、隐藏按纽
    TweenPosition                               m_ShowHideBtnAnim;
    byte                                        m_Gender;
    byte                                        m_Address;
    bool m_bInit;

    public bool IsInit
    {
        get { return m_bInit; }
    }
    public UnityEngine.Object Init()
    {
        UnityEngine.Object obj = Initobj("InformationWind");
        WindID.m_state = HallLogicUIStatue.Hall_State.Hall_Information;
        WindID.m_SmailWind = 0;
        return obj;
    }
    
    public System.Collections.IEnumerator InitGameObj(object objtype)
    {
        InfmationWind WindType = (InfmationWind )objtype;
        m_bInit = true;
        if (WindObj != null)
        {
            WindObj.SetActive(true);
            for (byte i = 0; i < 4; ++i)
            {
                if (i == (byte)WindType)
                    m_TopBtns[i].m_IsChecked = true;
                else
                    m_TopBtns[i].m_IsChecked = false;
            }
        }
        else
        {
            Init_GameObj();
            WindObj.SetActive(false);
            yield return new WaitForEndOfFrame();
            m_SmailBg[0] = BaseTranF.GetChild(1).GetChild(0).gameObject;
            for (byte i = 0; i < 2; ++i)
                m_SmailBg[i + 1] = m_SmailBg[0].transform.GetChild(i).gameObject;
            InitTopBtns(WindType); yield return new WaitForEndOfFrame();
            InitAchievement(); yield return new WaitForEndOfFrame();
            InitUserInf(); yield return new WaitForEndOfFrame();
            InitTask(); yield return new WaitForEndOfFrame();
            InitRank(); yield return new WaitForEndOfFrame();
            InitChangeHead(); yield return new WaitForEndOfFrame();
            InitChangeUserInf(); yield return new WaitForEndOfFrame();
        }
        ExchangeTopBtnsBg();
        m_bInit = false;
        yield break;
    }
    void InitTopBtns(InfmationWind WindType)
    {
        for (byte i = 0; i < 4; ++i)
        {
            m_TopBtns[i].m_BtnObj = BaseTranF.GetChild(0).GetChild(i).gameObject;
            m_TopBtnsCheckbg[i] = BaseTranF.GetChild(0).GetChild(i + 4).gameObject;
            //m_TopBtns[i].m_Btn = m_TopBtns[i].m_BtnObj.GetComponent<UIButton>();
            if (i == (byte)WindType)
                m_TopBtns[i].m_IsChecked = true;
            else
                m_TopBtns[i].m_IsChecked = false;
            m_TopBtns[i].m_Tag = i;
            GetBtnLister(m_TopBtns[i].m_BtnObj).onClick = OnClickTopBtns;
        }
    }
    void InitUserInf()
    {
        m_UserInfoUI.Init(BaseTranF.GetChild(3).gameObject);
        m_InfoChildWndObj[(byte)InfmationWind.Wind_UserInF] = m_UserInfoUI.WindObj;
       
    }
    void InitAchievement()//初始化成就
    {
        m_AchievementUI.Init(BaseTranF.GetChild(2).gameObject);
        m_InfoChildWndObj[(byte)InfmationWind.Wind_Achievement] = m_AchievementUI.WindObj;

    }
    void InitTask()
    {
        m_TaskUI.Init(BaseTranF.GetChild(4).gameObject);
        m_InfoChildWndObj[(byte)InfmationWind.Wind_Task] = m_TaskUI.WindObj;
    }
    void InitRank()
    {
        m_RankUI.Init(BaseTranF.GetChild(5).gameObject);
        m_InfoChildWndObj[(byte)InfmationWind.Wind_Rank] = m_RankUI.WindObj;
      
    }
    void InitChangeHead()
    {
        Transform BaseTR = BaseTranF.GetChild(6);
        m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserHead] = BaseTR.gameObject;
        UIGrid[] M_Grid = new UIGrid[4];
        for (byte i = 1; i <= 4; ++i)
            M_Grid [i-1] = BaseTR.GetChild(0).GetChild(i).GetComponent<UIGrid>();
        GameObject BaseChild = BaseTR.GetChild(0).GetChild(0).gameObject;
        if(BaseChild.activeSelf)
            BaseChild.SetActive(false);
        for (byte i = 0; i < 6; ++i)//最好为偶数
        {
            GameObject obj = GridInstantiateObj(M_Grid[(i % 4)], BaseChild);
            m_changeUserHeadContentArray[i].m_btn = new WindBtn();
            m_changeUserHeadContentArray[i].m_btn.m_BtnObj = obj;
            m_changeUserHeadContentArray[i].m_btn.m_Btn = obj.GetComponent<UIButton>();
            m_changeUserHeadContentArray[i].m_btn.m_Tag = i;
            GetBtnLister(obj,false).onClick = OnClickChangeHeadBtns;
            m_changeUserHeadContentArray[i].m_ChoseSp = obj.transform.GetChild(1).GetComponent<UISprite>();
            m_changeUserHeadContentArray[i].m_HeadTexture = obj.transform.GetChild(0).GetChild(0).GetComponent<UITexture>();
            m_changeUserHeadContentArray[i].m_AddownHead = obj.transform.GetChild(2).GetComponent<UISprite>();
            if (PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID() == (i - 1))
            {
                m_changeUserHeadContentArray[i].m_btn.m_IsChecked = true;
                if (!m_changeUserHeadContentArray[i].m_ChoseSp.gameObject.activeSelf)
                    m_changeUserHeadContentArray[i].m_ChoseSp.gameObject.SetActive(true);
            }
            else
            {
                m_changeUserHeadContentArray[i].m_btn.m_IsChecked = false;
                if (m_changeUserHeadContentArray[i].m_ChoseSp.gameObject.activeSelf)
                    m_changeUserHeadContentArray[i].m_ChoseSp.gameObject.SetActive(false);
            }
            if(i < 1)
            {
                if(!m_changeUserHeadContentArray[i].m_AddownHead.gameObject.activeSelf)
                    m_changeUserHeadContentArray[i].m_AddownHead.gameObject.SetActive(true);
                if(m_changeUserHeadContentArray[i].m_HeadTexture.gameObject.activeSelf)
                    m_changeUserHeadContentArray[i].m_HeadTexture.gameObject.SetActive(false);
                //if(i ==0)
                //    m_changeUserHeadContentArray[i].m_AddownHead.spriteName = "Information_Changehead_CameraBtn";
                //else if( i ==1)
                    m_changeUserHeadContentArray[i].m_AddownHead.spriteName = "Information_Changehead_Localpicture";
            }
            else
            {
                if(m_changeUserHeadContentArray[i].m_AddownHead.gameObject.activeSelf)
                    m_changeUserHeadContentArray[i].m_AddownHead.gameObject.SetActive(false);
                if(!m_changeUserHeadContentArray[i].m_HeadTexture.gameObject.activeSelf)
                    m_changeUserHeadContentArray[i].m_HeadTexture.gameObject.SetActive(true);
                m_changeUserHeadContentArray[i].m_HeadTexture.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[i-1];
            }
        }
        GetBtnLister(BaseTR.GetChild(1).gameObject,false).onClick = OnClickChangeHeadCloseBtn;
        GetBtnLister(BaseTR.GetChild(3).gameObject, false).onClick = OnClickChangeHeadCloseBtn;
    }
    void InitChangeUserInf()
    {
        Transform BaseTR = BaseTranF.GetChild(7);
        m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserInf] = BaseTR.gameObject;
        GetBtnLister(BaseTR.GetChild(6).gameObject,false).onClick = OnClickChangeUserInfCloseBtn;
        m_ChangeUserInFInPut = BaseTR.GetChild(0).GetComponent<UIInput>();
        for (byte i = 0; i < 9; ++i)
        {
            if (i < 4)
                m_ChangeUserInFBtns[i].m_BtnObj = BaseTR.GetChild(i + 1).gameObject;
            else
            {
                m_ChangeUserInFBtns[i].m_BtnObj = BaseTR.GetChild(5).GetChild(i - 4).gameObject;
                m_AccountMgrTrans[i - 4] = BaseTR.GetChild(5).GetChild(i - 4);
            }
            m_ChangeUserInFBtns[i].m_Btn = m_ChangeUserInFBtns[i].m_BtnObj.GetComponent<UIButton>();
            GetBtnLister(m_ChangeUserInFBtns[i].m_BtnObj,false).onClick = OnCheckChangeUserInFBtns;
            m_ChangeUserInFBtns[i].m_IsChecked = false;
            m_ChangeUserInFBtns[i].m_Tag = i;
        }
        m_ChangeUserInFBtns[9].m_BtnObj = m_ChangeUserInFInPut.transform.GetChild(2).gameObject;
        m_ChangeUserInFBtns[9].m_Btn = m_ChangeUserInFBtns[9].m_BtnObj.GetComponent<UIButton>();
        m_ChangeUserInFBtns[9].m_IsChecked = false;
        m_ChangeUserInFBtns[9].m_Tag = 9;
        GetBtnLister(m_ChangeUserInFBtns[9].m_BtnObj, false).onClick = OnCheckChangeUserInFBtns;
        m_ShowHideBtnTrans[0] = m_ChangeUserInFBtns[2].m_BtnObj.transform;      //显示
        m_ShowHideBtnTrans[1] = m_ChangeUserInFBtns[3].m_BtnObj.transform;      //隐藏
        m_ShowHideBtnAnim = BaseTR.GetChild(7).GetComponent<TweenPosition>();
        InitGender();
        InitShowHideAddress();
        m_ChangeUserInFInPut.value = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();
        // 4  5 6 7 8 
        if (SDKMgr.IS_SDK_CHANNEL||ThirdLogon.Instance.m_blogon)
        {
            //(1,2)3,4,//往前移动一个位置
            m_AccountMgrTrans[0].gameObject.SetActive(false);
            m_AccountMgrTrans[4].position = m_AccountMgrTrans[3].position;
            m_AccountMgrTrans[3].position = m_AccountMgrTrans[2].position;
            m_AccountMgrTrans[2].position = m_AccountMgrTrans[1].position = m_AccountMgrTrans[0].position;

            if (ServerSetting.ShowExtraBtn == false)
            {
                m_AccountMgrTrans[1].gameObject.SetActive(false);
                m_AccountMgrTrans[2].gameObject.SetActive(false);
                m_AccountMgrTrans[3].gameObject.SetActive(false);
                m_AccountMgrTrans[4].gameObject.SetActive(false);
            }
        }
        else
        {

        }       
    }
    public void AccountmgrBtnPos()
    {
        SetBindBtn();
    }
    //public void AccountmgrBtnPos()
    //{

    //}
    void SetBindBtn()
    {
        if (PlayerRole.Instance.EntityManager.GetRoleEntityInfo() == null)
            return;
        tagRoleAddressInfo pInfo = PlayerRole.Instance.EntityManager.GetRoleEntityInfo();
        bool bBind = FishDataInfo.IsCanUsePhone(pInfo.Phone);

        m_AccountMgrTrans[1].gameObject.SetActive(!bBind);
        m_AccountMgrTrans[2].gameObject.SetActive(bBind);
    }


    public void InitGender()
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGender())        //女的
        {
            m_Gender = 1;
            ChangeBtnName(m_ChangeUserInFBtns[0].m_Btn, "Infrmation_ChangeUserInf_CheckedBtn");
            ChangeBtnName(m_ChangeUserInFBtns[1].m_Btn, "Infrmation_ChangeUserInf_UnCheckBtn");
        }
        else    //男的
        {
            m_Gender = 0;
            ChangeBtnName(m_ChangeUserInFBtns[1].m_Btn, "Infrmation_ChangeUserInf_CheckedBtn");
            ChangeBtnName(m_ChangeUserInFBtns[0].m_Btn, "Infrmation_ChangeUserInf_UnCheckBtn");
        }
    }
    public void InitBtnState()
    {
        for (byte i = 0; i < m_ChangeUserInFBtns.Length; ++i)
        {
            m_ChangeUserInFBtns[i].m_IsChecked = false;
        }
    }
    void InitShowHideAddress()
    {
        if (PlayerRole.Instance.RoleInfo.RoleMe.IsShowIpAddress())
        {
            //ChangeBtnName(m_ChangeUserInFBtns[2].m_Btn, "Infrmation_ChangeUserInf_Show1");
            //ChangeBtnName(m_ChangeUserInFBtns[3].m_Btn, "Infrmation_ChangeUserInf_UnShow0");
            m_ShowHideBtnAnim.from = m_ShowHideBtnTrans[0].localPosition;
            m_ShowHideBtnAnim.to = m_ShowHideBtnTrans[0].localPosition;
            m_Address = 1;
        }
        else
        {
            m_Address = 0;
            m_ShowHideBtnAnim.from = m_ShowHideBtnTrans[1].localPosition;
            m_ShowHideBtnAnim.to = m_ShowHideBtnTrans[1].localPosition;
        }
    }
    public void UpDateUserInfContent()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_UserInfoUI.UpdateUserInfo();
    }
    public void UpdatePlayerGameDate()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_UserInfoUI.UpdatePlayerGameDate();
    }
    public void UpdateUserTitle()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_UserInfoUI.UpdateUserTitle();
    }
    public void UpdateAchievementDate()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_AchievementUI.UpdateAchievementDate();
        m_AchievementUI.UpdateMyAchievemetnData();
    }
    public void UpdateMyAchievemetnData()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_AchievementUI.UpdateMyAchievemetnData();
    }
    public void UpdateOnlyAchievementData(byte achieveID)
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_AchievementUI.UpdateOnlyAchievementDate(achieveID);
    }
    public void UpdateFinishAchievementDate()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_AchievementUI.UpdateFinishAchievementDate();
    }
    public void UpdateTaskContent()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_TaskUI.UpdateTaskDate();
    }
    public void UpdateTaskProgress(byte taskID)
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_TaskUI.UpdateTaskProgress(taskID);
    }
    public void UpdateRankContent()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;

        m_RankUI.UpdateRankContent();
    }
    public void UpdateRankFriendState(uint userID)
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        m_RankUI.UpdateRankFriendState(userID);
    }
    public void UpdateMyRankContent()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
         m_RankUI.UpdateMyRankContent();
    }
    public void SetRankHeader(uint id, byte flag, Texture2D tex)
    {
        if(m_RankUI == null)
        {
            LogMgr.Log("m_RankUI == null.");
        }
        else
            m_RankUI.SetHeader(id, flag, tex);
    }
    void OnClickChangeHeadCloseBtn(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
     //   ShowWind(InfmationWind.Wind_UserInF);
        OnChangeUserHead(false);
    }
    void OnClickChangeHeadBtns(GameObject go)
    {
        PlayBtnMusic();
        int checkTag = 0;
        for (byte i = 0; i < m_changeUserHeadContentArray.Length; ++i)
        {
            if (m_changeUserHeadContentArray[i].m_btn.m_Btn.gameObject == go)
            {
                m_changeUserHeadContentArray[i].m_btn.m_IsChecked = true;
                checkTag = m_changeUserHeadContentArray[i].m_btn.m_Tag;
                if (m_changeUserHeadContentArray[i].m_ChoseSp.gameObject.activeSelf != true) m_changeUserHeadContentArray[i].m_ChoseSp.gameObject.SetActive(true);
                if (checkTag > 0)
                {
                    //PlayerRole.Instance.RoleInfo.RoleMe.SetFaceID((uint)i - 1);
                    PlayerRole.Instance.RoleInfo.ChangeSystemFaceID((uint)i - 1);
                }
            }
            else
            {
                m_changeUserHeadContentArray[i].m_btn.m_IsChecked = false;
                if (m_changeUserHeadContentArray[i].m_ChoseSp.gameObject.activeSelf != false) m_changeUserHeadContentArray[i].m_ChoseSp.gameObject.SetActive(false);
            }
        }
    
        switch (checkTag)
        {
            case 0:
                //打开摄像机
                //NativeInterface.OpenCamera();
                NativeInterface.OpenPhoto();
                break;
            case 1:
                //打开相册
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                break;
        }
    }
    void OnClickChangeUserInfCloseBtn(GameObject go)
    {
        if (m_Gender == 1 && (PlayerRole.Instance.RoleInfo.RoleMe.GetGender() == false) )
        {
            PlayerRole.Instance.RoleInfo.ChangeGender();
        }
        else if (m_Gender == 0 && (PlayerRole.Instance.RoleInfo.RoleMe.GetGender() == true) )
        {
            PlayerRole.Instance.RoleInfo.ChangeGender();
        }

        if ( m_Address == 1 && (PlayerRole.Instance.RoleInfo.RoleMe.IsShowIpAddress() == false))
        {
            PlayerRole.Instance.RoleInfo.SendIsShowIpAddress(true); 
        }
        else if (m_Address == 0 && (PlayerRole.Instance.RoleInfo.RoleMe.IsShowIpAddress() == true))
        {
            PlayerRole.Instance.RoleInfo.SendIsShowIpAddress(false); 
        }
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        OnChangeUserInfo(false);
       // ShowWind(InfmationWind.Wind_UserInF);
         
    }
    void OnCheckChangeUserInFBtns(GameObject go)
    {
        PlayBtnMusic();
        byte checkTag = 0;
        for (int i = 0; i < m_ChangeUserInFBtns.Length; ++i)
        {
            if (go == m_ChangeUserInFBtns[i].m_BtnObj)
            {
                checkTag = m_ChangeUserInFBtns[i].m_Tag;
                if (m_ChangeUserInFBtns[i].m_IsChecked && i < 4)
                    return;
                m_ChangeUserInFBtns[i].m_IsChecked = true;
            }
            else
                m_ChangeUserInFBtns[i].m_IsChecked = false;
        }
        switch (checkTag)
        {
            case 0: //女的
                {
                    m_Gender = 1;
                    ChangeBtnName(m_ChangeUserInFBtns[0].m_Btn, "Infrmation_ChangeUserInf_CheckedBtn");
                    ChangeBtnName(m_ChangeUserInFBtns[1].m_Btn, "Infrmation_ChangeUserInf_UnCheckBtn");
                   // PlayerRole.Instance.RoleInfo.RoleMe.SetGender(true);

                } break;
            case 1:     //男的
                {
                    m_Gender = 0;
                    ChangeBtnName(m_ChangeUserInFBtns[1].m_Btn, "Infrmation_ChangeUserInf_CheckedBtn");
                    ChangeBtnName(m_ChangeUserInFBtns[0].m_Btn, "Infrmation_ChangeUserInf_UnCheckBtn");
                   // PlayerRole.Instance.RoleInfo.RoleMe.SetGender(false);
                } break;
            case 2:     //显示
                {
                    //if (PlayerRole.Instance.RoleInfo.RoleMe.IsShowIpAddress())
                    //    return;
                    m_Address = 1;
                    m_ShowHideBtnAnim.ResetToBeginning();
                    m_ShowHideBtnAnim.from = m_ShowHideBtnTrans[1].localPosition;
                    m_ShowHideBtnAnim.to = m_ShowHideBtnTrans[0].localPosition;
                    m_ShowHideBtnAnim.PlayForward();
                    //ChangeBtnName(m_ChangeUserInFBtns[2].m_Btn, "Infrmation_ChangeUserInf_Show1");
                    //ChangeBtnName(m_ChangeUserInFBtns[3].m_Btn, "Infrmation_ChangeUserInf_UnShow0");
                } break;
            case 3:     //隐藏
                {
                    //if (!PlayerRole.Instance.RoleInfo.RoleMe.IsShowIpAddress())
                    //    return;
                    m_Address = 0;
 
                    m_ShowHideBtnAnim.ResetToBeginning();
                    m_ShowHideBtnAnim.from = m_ShowHideBtnTrans[0].localPosition;
                    m_ShowHideBtnAnim.to = m_ShowHideBtnTrans[1].localPosition;
                    m_ShowHideBtnAnim.PlayForward();
                } break;
            case 4:     //账号管理
                {
                    GlobalHallUIMgr.Instance.ShowAccountMgrWnd(Acoount_MgrUI_Type.PasswordMgr_UI);
                    break;
                }                 
            case 5:     //绑定手机
                {
                    GlobalHallUIMgr.Instance.ShowAccountMgrWnd(Acoount_MgrUI_Type.BindMobile_UI);
                    break;

                }
            case 6: //手机密码    
                {
                    GlobalHallUIMgr.Instance.ShowAccountMgrWnd(Acoount_MgrUI_Type.Phone_Password);
                    break;
                }
            case 7:    //实名认证
                {

                    GlobalHallUIMgr.Instance.ShowAccountMgrWnd(Acoount_MgrUI_Type.Certification_UI);
                    break;
                }              
            case 8: //收货地址
                {
                    GlobalHallUIMgr.Instance.ShowAccountMgrWnd(Acoount_MgrUI_Type.ReceiverAddress_UI);
                    break;
                }                
            case 9:     //修改昵称
                {
                    if (m_ChangeUserInFInPut.value.Length < 1)
                    {
                        GlobalHallUIMgr.Instance.ShowSystemTipsUI("昵称不能为空", 1, false);
                        return;
                    }
                    PlayerRole.Instance.RoleInfo.ChangeNickName(m_ChangeUserInFInPut.value);
                    break;
                }                        
        }
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
    }
   
    void OnClickTopBtns(GameObject go)
    {
        byte ClickTag = 0;
        for (byte i = 0; i < m_TopBtns.Length; ++i)
        {
            if (m_TopBtns[i].m_BtnObj == go)
            {
                ClickTag = m_TopBtns[i].m_Tag;
                m_TopBtns[i].m_IsChecked = true;
            }
            else
                m_TopBtns[i].m_IsChecked = false;
        }
        ExchangeTopBtnsBg();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
    }
    public void OnChangeUserHead(bool bShow)
    {
        if (bShow)
        {
            GlobalHallUIMgr.Instance.AnimationEffect(m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserHead]);
            WndManager.Instance.Push(m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserHead]);
        }
        else
            WndManager.Instance.Pop(m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserHead]);
        m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserHead].SetActive(bShow);
    }
    public void OnChangeUserInfo(bool bShow)
    {
        
        if (bShow)
        {
            if (PlayerRole.Instance.ItemManager.GetAllItemMap() == null)
            {
                //m_ChangeUserInFBtns[8].m_Btn.isEnabled = false;
            }
            GlobalHallUIMgr.Instance.AnimationEffect(m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserInf]);
            WndManager.Instance.Push(m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserInf]);
            InitGender();
            InitBtnState();
            InitShowHideAddress();
            m_ChangeUserInFInPut.value = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();            
        }
        else
            WndManager.Instance.Pop(m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserInf]);
        m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserInf].SetActive(bShow);
        SetBindBtn();
    }
   public void UpdateChangeInfoBtnState()
    {
        if (m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserInf] == null)
            return;
        if (m_InfoChildWndObj[(byte)InfmationWind.Wind_ChangeUserInf].activeSelf != true)
            return;
        //m_ChangeUserInFBtns[8].m_Btn.isEnabled = true;
    }
    void ExchangeTopBtnsBg()
    {
        for (byte i = 0; i < m_TopBtns.Length; ++i)
        {
            if (m_TopBtns[i].m_IsChecked == true)
            {
                m_TopBtns[i].m_BtnObj.SetActive(false);
                m_TopBtnsCheckbg[i].SetActive(true);
                 if (i == 0)
                 {
                     m_SmailBg[0].SetActive(false);
                 }
                else if (i == 3)
                {
                    m_SmailBg[0].SetActive(true);

                    m_SmailBg[0].transform.localPosition = new Vector3(115.6f, 0, 0);
                    m_SmailBg[0].transform.localScale = new Vector3(0.925f, 1, 1);
                    m_SmailBg[2].transform.localPosition = new Vector3(-225f, 0, 0);
                    m_SmailBg[1].transform.localPosition = new Vector3(225f, 0, 0);
                }
                else
                {
                    m_SmailBg[0].SetActive(true);

                    m_SmailBg[0].transform.localPosition = new Vector3(0, 0, 0);
                    m_SmailBg[0].transform.localScale = new Vector3(1, 1, 1);
                    m_SmailBg[2].transform.localPosition = new Vector3(-266f, 0, 0);
                    m_SmailBg[1].transform.localPosition = new Vector3(266f, 0, 0);
                }
                ShowWind(i);
            }
            else
            {
                m_TopBtns[i].m_BtnObj.SetActive(true);
                m_TopBtnsCheckbg[i].SetActive(false);
            }
        }
    }
    
    void ShowWind(byte Type)
    {
        for (byte i = 0; i < (byte)InfmationWind.Wind_ChangeUserInf; ++i)
        {
            if (i == Type)
            {
                m_InfoChildWndObj[i].SetActive(true);
                UpdateInformationUIDate((InfmationWind)i);
            }
            else
                m_InfoChildWndObj[i].SetActive(false);
        }
    }
    //显示界面时请求最新数据
    void UpdateInformationUIDate(InfmationWind type)
    {
        switch (type)
        {
            case InfmationWind.Wind_Rank:
                {
                    UpdateRankContent();
                    UpdateMyRankContent();
                }
                break;
            case InfmationWind.Wind_Task:
                UpdateTaskContent();
                break;
            case InfmationWind.Wind_Achievement:
                UpdateAchievementDate();
                break;
            case InfmationWind.Wind_UserInF:
                UpDateUserInfContent();
                break;
        }
    }
    
    public void SetMyHeader(Texture2D tex)
    {
        m_UserInfoUI.SetHeader(tex);
    }
    public void ShutHide()
    {
        if (WindObj != null)
        {
            WindObj.SetActive(false);
        }
    }
    public void ShutDown()
    {
        if (WindObj != null)
            GameObject.Destroy(WindObj);
        m_TaskUI.ShutDown();
        m_AchievementUI.ShutDown();
    }

    public struct ChangeUserHeadInf
    {
        public WindBtn m_btn;
        public UISprite m_ChoseSp;
        public UISprite m_AddownHead;
        public UITexture m_HeadTexture;
    }

}
