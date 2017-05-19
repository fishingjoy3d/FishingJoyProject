using UnityEngine;
using System.Collections.Generic;

public enum GameModel
{
    Relax_Model = 0,    //休闲模式
    Match_Model,        //竞赛模式
}
public struct Experience_Bar
{
    public UISprite         m_ExperienceBg;
    public UILabel          m_LevelLabel;
}

public struct GameModelInfo
{
    public UILabel[]        m_GameRecordLabel;
    public UILabel[]        m_HonorInfoLabel;
    public UISprite[]       m_HonorInfoSprite;
}

public class HallLoginUI_UserInfo : HallLoginUI_BaseWind
{
    UITexture                   m_HeadTex;
    UILabel                     m_UINickName;       //
    UILabel                     m_labelgameid;
    UILabel                     m_MeiLiLabel;
    UILabel                     m_AchievementLabel;
    UILabel                     m_AddressLabel;
    UIButton                    m_ModifiedInfoBtn;    //修改资料按纽
    UISprite                    m_Gender;
    UISprite[]                  m_GameModelBtn = new UISprite[2];      //休闲模式按纽 & 竞赛模式按纽  
    GameObject[]                m_GameModelObj = new GameObject[2];    //休闲模式 & 竞赛模式
    GameModelInfo[]             m_GameModelInfo = new GameModelInfo[2];
    UserTitle                   m_UserTitle = new UserTitle();        //玩家称号
    Experience_Bar              m_Experience = new Experience_Bar();       //经验条
    GameModel                   m_GameModel = GameModel.Relax_Model;

    public void Init(GameObject go)
    {
        WindObj = go;
        BaseTranF = WindObj.transform;
        m_HeadTex = BaseTranF.GetChild(0).GetChild(0).GetComponent<UITexture>();
        GetBtnLister(BaseTranF.GetChild(1).gameObject, true).onClick = OnChangeHeader;
       // UIEventListener.Get(BaseTranF.GetChild(1).gameObject).onClick = OnChangeHeader;
        m_UINickName = BaseTranF.GetChild(2).GetComponent<UILabel>();
        m_labelgameid = BaseTranF.GetChild(12).GetComponent<UILabel>();
        m_Gender = BaseTranF.GetChild(2).GetChild(0).GetComponent<UISprite>();
        m_Experience.m_LevelLabel = BaseTranF.GetChild(3).GetChild(0).GetComponent<UILabel>();
        m_Experience.m_ExperienceBg = BaseTranF.GetChild(3).GetComponent<UISprite>();
        m_UserTitle.Init(BaseTranF.GetChild(4).gameObject);
        m_MeiLiLabel = BaseTranF.GetChild(5).GetComponent<UILabel>();
        m_AchievementLabel = BaseTranF.GetChild(6).GetComponent<UILabel>();
        m_AddressLabel = BaseTranF.GetChild(7).GetComponent<UILabel>();
        m_ModifiedInfoBtn = BaseTranF.GetChild(8).GetComponent<UIButton>();
        GetBtnLister(m_ModifiedInfoBtn.gameObject, true).onClick = OnModifiedInfo;
      //  UIEventListener.Get(m_ModifiedInfoBtn.gameObject).onClick = OnModifiedInfo;

        for (byte i = 0; i < 2; ++i)
        {
            m_GameModelBtn[i] = BaseTranF.GetChild(9).GetChild(i).GetComponent<UISprite>();
            m_GameModelObj[i] = BaseTranF.GetChild(9).GetChild(i + 2).gameObject;
            Transform GameTrans = m_GameModelObj[i].transform;
            if (i == (int)GameModel.Relax_Model)
            {
                InitRealx(GameTrans);
                GetBtnLister(m_GameModelBtn[i].gameObject).onClick = OnRelaxModel;
            }
            else
            {
                InitMatch(GameTrans);
                GetBtnLister(m_GameModelBtn[i].gameObject).onClick = OnMatchModel;
            }
        }
        ChangeGameModelBg();
    }
    void InitRealx(Transform Trans)
    {
        m_GameModelInfo[(int)GameModel.Relax_Model].m_GameRecordLabel = new UILabel[3];
        m_GameModelInfo[(int)GameModel.Relax_Model].m_HonorInfoLabel = new UILabel[3];
        m_GameModelInfo[(int)GameModel.Relax_Model].m_HonorInfoSprite = new UISprite[3];
        for (int i = 0; i < 3; ++i)
        {
            m_GameModelInfo[(int)GameModel.Relax_Model].m_GameRecordLabel[i] = Trans.GetChild(i).GetComponent<UILabel>();
            m_GameModelInfo[(int)GameModel.Relax_Model].m_HonorInfoLabel[i] = Trans.GetChild(i + 3).GetComponent<UILabel>();
            m_GameModelInfo[(int)GameModel.Relax_Model].m_HonorInfoSprite[i] = Trans.GetChild(i + 3).GetChild(0).GetComponent<UISprite>();
        }

    }
    void InitMatch(Transform Trans)
    {
        m_GameModelInfo[(int)GameModel.Match_Model].m_GameRecordLabel = new UILabel[2];
        m_GameModelInfo[(int)GameModel.Match_Model].m_HonorInfoLabel = new UILabel[3];
        m_GameModelInfo[(int)GameModel.Match_Model].m_HonorInfoSprite = new UISprite[3];
        for (int i = 0; i < 3; ++i)
        {
            if(i < 2)
                m_GameModelInfo[(int)GameModel.Match_Model].m_GameRecordLabel[i] = Trans.GetChild(i).GetComponent<UILabel>();
            m_GameModelInfo[(int)GameModel.Match_Model].m_HonorInfoLabel[i] = Trans.GetChild(i + 2).GetComponent<UILabel>();
            m_GameModelInfo[(int)GameModel.Match_Model].m_HonorInfoSprite[i] = Trans.GetChild(i + 2).GetChild(0).GetComponent<UISprite>();
        }
    }
    public void UpdateUserInfo()
    {
        if (WindObj.activeSelf != true)
            return;
        m_UINickName.text = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();
        m_labelgameid.text = PlayerRole.Instance.RoleInfo.RoleMe.GetGameID().ToString();
        m_Gender.spriteName = GetUserSEX();
        //获取头像
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID() < ConstValue.CUSTOM_HEADER)
            m_HeadTex.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID()];
        else
            HeaderManager.Instance.GetPlayerHeader(PlayerRole.Instance.RoleInfo.RoleMe.GetUserID(), PlayerRole.Instance.RoleInfo.RoleMe.GetFaceID(), HeaderOptType.HEADER_UPLOAD, null);

        //经验显示
        m_Experience.m_LevelLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetLevel().ToString();
        m_Experience.m_ExperienceBg.fillAmount = GetCurExperience();
        
        //称号
        m_UserTitle.ShowUserCurTitle();
        //魅力
        m_MeiLiLabel.text = PlayerRole.Instance.CharmManager.GetCharmValue().ToString();
        m_AchievementLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementPoint().ToString();

        //地区
        m_AddressLabel.text = PlayerRole.Instance.RoleInfo.RoleMe.GetIPAddress();

        UpdatePlayerGameDate();
    }
    public void UpdatePlayerGameDate()
    {
        if (WindObj == null || WindObj.activeSelf != true)
            return;
        if (PlayerRole.Instance.RoleGameData.GetRoleGameData() == null)
            return;

        if (m_GameModel == GameModel.Relax_Model)
        {
            uint gameTime = PlayerRole.Instance.RoleGameData.GetRoleGameData().NonMonthGameSec;
            uint gold = PlayerRole.Instance.RoleGameData.GetRoleGameData().RoleGetGlobelSum;
            uint fishSum = PlayerRole.Instance.RoleGameData.GetRoleGameData().CatchFishSum_1;
            m_GameModelInfo[(int)GameModel.Relax_Model].m_GameRecordLabel[0].text =
                PlayerRole.Instance.RoleGameData.GetTimeStrBySec(gameTime);
            m_GameModelInfo[(int)GameModel.Relax_Model].m_GameRecordLabel[1].text = 
                PlayerRole.Instance.RoleGameData.GetRoleGameData().RoleCatchFishTotalSum.ToString();
            m_GameModelInfo[(int)GameModel.Relax_Model].m_GameRecordLabel[2].text =
                PlayerRole.Instance.RoleGameData.GetGoldStrBygold(gold);
            m_GameModelInfo[(int)GameModel.Relax_Model].m_HonorInfoLabel[0].text =
                PlayerRole.Instance.RoleGameData.GetGoldStrBygold(fishSum);
            m_GameModelInfo[(int)GameModel.Relax_Model].m_HonorInfoLabel[1].text =
                PlayerRole.Instance.RoleGameData.GetRoleGameData().CatchFishSum_3.ToString();
            m_GameModelInfo[(int)GameModel.Relax_Model].m_HonorInfoLabel[2].text =
                PlayerRole.Instance.RoleGameData.GetRoleGameData().CatchFishSum_19.ToString();
        }
        else
        {
            m_GameModelInfo[(int)GameModel.Match_Model].m_GameRecordLabel[0].text = 
                PlayerRole.Instance.RoleGameData.GetRoleGameData().RoleMonthSigupSum.ToString();
            m_GameModelInfo[(int)GameModel.Match_Model].m_GameRecordLabel[1].text =
                PlayerRole.Instance.RoleGameData.GetRoleGameData().RoleMonthRewardSum.ToString();
            m_GameModelInfo[(int)GameModel.Match_Model].m_HonorInfoLabel[0].text =
                PlayerRole.Instance.RoleGameData.GetRoleGameData().RoleMonthFirstSum.ToString();
            m_GameModelInfo[(int)GameModel.Match_Model].m_HonorInfoLabel[1].text =
                PlayerRole.Instance.RoleGameData.GetRoleGameData().RoleMonthSecondSum.ToString();
            m_GameModelInfo[(int)GameModel.Match_Model].m_HonorInfoLabel[2].text =
                PlayerRole.Instance.RoleGameData.GetRoleGameData().RoleMonthThreeSum.ToString();
        }
    }
    public void UpdateUserTitle()
    {
        m_UserTitle.UpdateUserTitle();
    }
    public void SetHeader(Texture2D tex)
    {
        if (tex == null)
            m_HeadTex.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        else
            m_HeadTex.mainTexture = tex;
    }
    void OnChangeHeader(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        HallRunTimeInfo.Instance_UI.m_loginUi.InfomationWind.OnChangeUserHead(true);
    }
    void OnModifiedInfo(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        HallRunTimeInfo.Instance_UI.m_loginUi.InfomationWind.OnChangeUserInfo(true);
    }
    void OnRelaxModel(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_GameModel = GameModel.Relax_Model;
        ChangeGameModelBg();
    }
    void OnMatchModel(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
        m_GameModel = GameModel.Match_Model;
        ChangeGameModelBg();
    }
    void ChangeGameModelBg()
    {
        for (int i = 0; i < 2; ++i)
        {
            if (i == (int)m_GameModel)
            {
                string strName = m_GameModelBtn[i].spriteName;
                strName = strName.Replace("1", "0");
                m_GameModelBtn[i].spriteName = strName;
                m_GameModelObj[i].SetActive(true);
            }
            else
            {
                string strName = m_GameModelBtn[i].spriteName;
                strName = strName.Replace("0", "1");
                m_GameModelBtn[i].spriteName = strName;
                m_GameModelObj[i].SetActive(false);
            }
        }
        UpdatePlayerGameDate();
    }
    float GetCurExperience()
    {
        uint experineceSum = 0;
        ushort level = PlayerRole.Instance.RoleInfo.RoleMe.GetLevel();
        if (level == 30)
            return 1.0f;
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetLevel() < Experience.Exp.Length)
        {
            experineceSum = Experience.Exp[level + 1] - Experience.Exp[level];
            return (float)(PlayerRole.Instance.RoleInfo.RoleMe.GetExp() * 1.0f) / (float)(experineceSum * 1.0f);
        }
        else
            return 1.0f;
        
    }
    string GetUserSEX()
    {
        if (!PlayerRole.Instance.RoleInfo.RoleMe.GetGender())
            return "male";
        else
            return "female";
    }
}

public struct PopupItem
{
    public byte             m_TitleID;
    public GameObject       m_Obj;
    public UILabel          m_Title;
    public GameObject       m_SelectBg;
//    public string m_StrTitle;
}
public class UserTitle : HallLoginUI_BaseWind
{
    UILabel             m_UserCurTitle;
    UILabel             m_UITitleTips;
    GameObject          m_PopupList;
    GameObject          m_PopupListItem;
    UIGrid              m_UIGrid;
    List<PopupItem>     m_PopupItemList = new List<PopupItem>();
    bool                m_bShowPopup = false;

    public void Init(GameObject go)
    {
        WindObj = go;
        BaseTranF = WindObj.transform;
        m_UserCurTitle = BaseTranF.GetChild(0).GetComponent<UILabel>();
        m_PopupList = BaseTranF.GetChild(2).gameObject;
        m_PopupListItem = BaseTranF.GetChild(2).GetChild(0).GetChild(0).gameObject;
        m_UIGrid = BaseTranF.GetChild(2).GetChild(0).GetChild(1).GetComponent<UIGrid>();
        m_UITitleTips = BaseTranF.GetChild(2).GetChild(0).GetChild(2).GetComponent<UILabel>();
        GetBtnLister(BaseTranF.GetChild(1).gameObject, true).onClick = OnPopupListWnd;
       // GetBtnLister(m_PopupListItem, true).onClick = OnClickTitle;
       // UIEventListener.Get(BaseTranF.GetChild(1).gameObject).onClick = OnPopupListWnd;
       // UIEventListener.Get(m_PopupList).onClick = OnClickTitle;
    }
    void OnClickTitle(GameObject go)
    {
        for (byte i = 0;  i < m_PopupItemList.Count; ++i)
        {
            if ( go == m_PopupItemList[i].m_Obj)
            {
                PlayerRole.Instance.TitleManager.ChangeCurreyTitle(m_PopupItemList[i].m_TitleID);
                m_UserCurTitle.text = m_PopupItemList[i].m_Title.text;
                m_PopupList.SetActive(false);
                m_bShowPopup = !m_bShowPopup;
                break;
            }
        }
    }
    void OnPopupListWnd(GameObject go)
    {
        m_bShowPopup = !m_bShowPopup;
        if (m_PopupList.activeSelf != m_bShowPopup)
            m_PopupList.SetActive(m_bShowPopup);
        if (m_bShowPopup)
            UpdateUserTitle();
    }
    public void ShowUserCurTitle()
    {
        if (FishConfig.Instance.m_TitleInfo.m_TileMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetTitleID()))
        {
            m_UserCurTitle.text = FishConfig.Instance.m_TitleInfo.m_TileMap[PlayerRole.Instance.RoleInfo.RoleMe.GetTitleID()].TitleName;

        }
    }
    public void UpdateUserTitle()
    {
        if (m_PopupList.activeSelf != true)
            return;
        if (PlayerRole.Instance.TitleManager.GetAllTitle() == null)
            return;
        ClearGrid();
        m_PopupItemList.Clear();
        foreach(KeyValuePair<byte , byte> map in PlayerRole.Instance.TitleManager.GetAllTitle())
        {
            if (FishConfig.Instance.m_TitleInfo.m_TileMap.ContainsKey(map.Key) != true)
                continue;
            PopupItem item = new PopupItem();
            item.m_Obj = GameObject.Instantiate(m_PopupListItem) as GameObject;
            item.m_Obj.SetActive(true);
            item.m_Title = item.m_Obj.transform.GetChild(0).GetComponent<UILabel>();
            item.m_SelectBg = item.m_Obj.transform.GetChild(1).gameObject;
            item.m_Title.text = FishConfig.Instance.m_TitleInfo.m_TileMap[map.Key].TitleName;
            item.m_TitleID = FishConfig.Instance.m_TitleInfo.m_TileMap[map.Key].TitleID;
            m_UIGrid.AddChild(item.m_Obj.transform);
            item.m_Obj.transform.localScale = Vector3.one;
            m_PopupItemList.Add(item);
            UIEventListener.Get(item.m_Obj).onClick = OnClickTitle;
        }
        if (m_PopupItemList.Count == 0)
        {
            m_UITitleTips.text = StringTable.GetString("Title_Tips");
            m_UITitleTips.gameObject.SetActive(true);
        }
        else
            m_UITitleTips.gameObject.SetActive(false);
    }
    void ClearGrid()
    {
        List<Transform> gridChid = m_UIGrid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_UIGrid.transform.DetachChildren();
    }
    public void ShutDown()
    {
        if (WindObj != null)
            GameObject.Destroy(WindObj);
    }
}