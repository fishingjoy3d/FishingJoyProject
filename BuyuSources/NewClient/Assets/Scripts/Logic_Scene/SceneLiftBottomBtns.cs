using UnityEngine;
using System.Collections.Generic;

public class SceneBtnsMrg
{
    GameWindBtns[]      m_LiftBtnsList = new GameWindBtns[4];
    GameWindBtns        m_BaseLiftBtn = new GameWindBtns();
    Transform           m_BaseTranf = null;
    Object              m_obj = null;
    GameObject[]        m_WarningObj = new GameObject[3];
    Transform           m_BaseBGTransF;
    Transform           m_BaseBGTransF_1;
    
    public void Init()
    {
        m_obj = ResManager.Instance.LoadObject("SceneBtnWind", "SceneRes/Prefab/UI/Btns/", ResType.SceneRes);
        m_BaseTranf = ((GameObject)GameObject.Instantiate(m_obj)).transform;
        ResManager.Instance.UnloadObject(m_obj);
        m_BaseTranf.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        InitBtns();
    }
    public void InitBtns()
    {
        m_BaseLiftBtn.m_Obj = m_BaseTranf.GetChild(0).GetChild(0).gameObject;
        m_BaseLiftBtn.m_IsChecked = false;
        UIEventListener.Get(m_BaseLiftBtn.m_Obj).onClick = OnLiftBaseBtnChecked;
        m_BaseBGTransF = m_BaseTranf.GetChild(1);
        m_BaseBGTransF_1 = m_BaseTranf.GetChild(2);
        for (byte i = 0; i < m_BaseBGTransF.childCount; i++)
        {
            m_LiftBtnsList[i].m_Obj = m_BaseBGTransF.GetChild(i).gameObject;
            m_LiftBtnsList[i].m_IsChecked = false;
            m_LiftBtnsList[i].m_Obj.SetActive(true);
            m_LiftBtnsList[i].m_Tag = i;
            UIEventListener.Get(m_LiftBtnsList[i].m_Obj).onClick = OnLiftBtnCheck;
        }
        for (byte j = 0; j < m_WarningObj.Length; ++j)
        {
            m_WarningObj[j] = m_BaseTranf.GetChild(j + 3).gameObject;
        }
        UpdateWarningState();
    }
    public void Update(float deltaTime)
    {
        int ScaleSpeed = 4;
        if (m_BaseLiftBtn.m_IsChecked == true)
        {
            Vector3 scale = m_BaseBGTransF.localScale;
            if (scale.x < 1)
            {
                scale.x += deltaTime * ScaleSpeed; scale.y += deltaTime * ScaleSpeed; scale.z += deltaTime * ScaleSpeed;
                if (scale.x > 1)
                    scale = new Vector3(1, 1, 1);
                m_BaseBGTransF.localScale = scale;
                m_BaseBGTransF_1.localScale = scale;
                if (scale.x > 0.7)
                {
                    if (m_BaseBGTransF.gameObject.activeSelf != true) m_BaseBGTransF.gameObject.SetActive(true);
                    if (m_BaseBGTransF_1.gameObject.activeSelf != true) m_BaseBGTransF_1.gameObject.SetActive(true);
                }
                else
                {
                    if (m_BaseBGTransF.gameObject.activeSelf != false) m_BaseBGTransF.gameObject.SetActive(false);
                    if (m_BaseBGTransF_1.gameObject.activeSelf != false) m_BaseBGTransF_1.gameObject.SetActive(false);
                }
                if (scale.x > 0.95)
                {
                    for (int i = 0; i < 4; ++i)
                    {
                        if (m_LiftBtnsList[i].m_Obj.activeSelf != true)
                        {
                            m_LiftBtnsList[i].m_Obj.SetActive(true);
                        }
                    }
                    UpdateWarningState();
                }
                else
                {
                    for (int i = 0; i < 4; ++i)
                    {
                        if (m_LiftBtnsList[i].m_Obj.activeSelf != false)
                        {
                            m_LiftBtnsList[i].m_Obj.SetActive(false);
                        }
                    }
                }

            }
        }
        else
        {
            Vector3 scale = m_BaseBGTransF.localScale;
            if (scale.x > 0)
            {
                scale.x -= deltaTime * ScaleSpeed; 
                scale.y -= deltaTime * ScaleSpeed; 
                scale.z -= deltaTime * ScaleSpeed;
                if (scale.x > 1)
                    scale = new Vector3(0, 0, 0);
                m_BaseBGTransF.localScale = scale;
                m_BaseBGTransF_1.localScale = scale;
                if (scale.x < 0.7)
                {
                    if (m_BaseBGTransF.gameObject.activeSelf != false) m_BaseBGTransF.gameObject.SetActive(false);
                    if (m_BaseBGTransF_1.gameObject.activeSelf != false) m_BaseBGTransF_1.gameObject.SetActive(false);
                    UpdateWarningState();
                }
            }
            else
            {
                if (m_BaseBGTransF.gameObject.activeSelf != false) m_BaseBGTransF.gameObject.SetActive(false);
                if (m_BaseBGTransF_1.gameObject.activeSelf != false) m_BaseBGTransF_1.gameObject.SetActive(false);
                UpdateWarningState();
            }
        }
    }
    void OnLiftBtnCheck(GameObject go)
    {
        PlayerBtnMusic();
        byte MyTag = 0;
        for (byte i = 0; i < m_LiftBtnsList.Length;++i )
        {
            if (m_LiftBtnsList[i].m_Obj == go)
            {
                MyTag = m_LiftBtnsList[i].m_Tag;
                m_LiftBtnsList[i].m_IsChecked = true;
            }
        }
        switch (MyTag)
        {
            case 0://返回
                {
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic, false, true);
                    if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() != 0)      //处于比赛中
                    {

                        GlobalHallUIMgr.Instance.ShowMatchMsgBox(StringTable.GetString("Match_Leave_Warning"), PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID(), MatchMsgBoxType.Match_Leave);
                    }
                    else
                    {
                        //这里返回大厅会出现服务器不知道的情况，所以直接返回登录
                        SceneRuntime.SceneLogic.BackToHall();
                        //int bg = Random.Range(4, 7);
                        int bg = 1;
                        GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound2);
                    }
                    //     LogicManager.Instance.Back(null);
                } break;
            case 1://好友
                {
                    GlobalHallUIMgr.Instance.ShowFriendSysWnd();
                    break;
                }
            case 2://shop
                {
                    GlobalHallUIMgr.Instance.ShowShopSysWnd(Shop_Type.Shop_Property);
                } break;
            case 3://设置
                {
                    GlobalHallUIMgr.Instance.ShowSettingWind();
                } break;
        }

        if (m_BaseLiftBtn.m_IsChecked == true)
        {
            m_BaseLiftBtn.m_IsChecked = false;
        }
    }
    public void UpdateWarningState()
    {
        for (byte i = 0; i < m_WarningObj.Length; ++i )
        {
            m_WarningObj[i].SetActive(false);
        }
        if (!m_BaseLiftBtn.m_IsChecked)
        {
            if (PlayerRole.Instance.RoleStatesMessage.GetMailStates() || PlayerRole.Instance.RoleStatesMessage.GetGiffStates())
            {
                m_WarningObj[0].SetActive(true);

            }
        }
        else
        {
            if (PlayerRole.Instance.RoleStatesMessage.GetMailStates() || PlayerRole.Instance.RoleStatesMessage.GetGiffStates())
                m_WarningObj[2].SetActive(true);
        }
        
    }
    public void BaseBtnHide()
    {
        if (!m_BaseLiftBtn.m_IsChecked)
            return;
        m_BaseLiftBtn.m_IsChecked = false;
    }
    void OnLiftBaseBtnChecked(GameObject go)//左侧底部基按钮
    {
        PlayerBtnMusic();
        if (m_BaseLiftBtn.m_IsChecked == false)
        {
            m_BaseLiftBtn.m_IsChecked = true;
        }
        else
        {
            m_BaseLiftBtn.m_IsChecked = false;
        }
     //   ExchangeBaseBtnBg();
    }
    void ExchangeBaseBtnBg()//更换按钮背景
    {
        if (m_BaseLiftBtn.m_IsChecked == true)
        {
            if (m_BaseBGTransF.gameObject.activeSelf != true)
                m_BaseBGTransF.gameObject.SetActive(true);
            UIButton Btn = m_BaseLiftBtn.m_Obj.transform.GetComponent<UIButton>();
            ChangeBtnSpName(Btn, "SceneWind_MenusBtn1");
        }
        else
        {
            UIButton Btn = m_BaseLiftBtn.m_Obj.transform.GetComponent<UIButton>();
            ChangeBtnSpName(Btn, "SceneWind_MenusBtn0"); ;
        }
    }
    void PlayerBtnMusic()
    { 
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic); 
    }
    public void ChangeBtnSpName(UIButton btn, string str)
    {
        btn.normalSprite = str;
        btn.hoverSprite = str;
        btn.pressedSprite = str;
        btn.disabledSprite = str;
    }
    public void ShutDown()
    {
        if (m_BaseTranf != null)
            GameObject.Destroy(m_BaseTranf.gameObject);
        m_BaseTranf = null;
    }
    struct GameWindBtns
    {
        public GameObject m_Obj;
        public byte m_Tag;
        public bool m_IsChecked;
    }
}
