using UnityEngine;
using System.Collections.Generic;

public enum NameCardsUIType
{
    NameCards_UI = 0,
    Task_UI,
}

public enum CardsParentUIType
{
    Scene_Head = 0,
    Friend_Head,
    Other_Head,
}


public struct CardsInfo
{
    public UITexture        m_UIFace;
    public UISprite         m_UIGender;
    public UIButton         m_UIAddFriendBtn;
    public UILabel[]        m_UICardsInfo;
    public UILabel          m_UIWarring;
    public UISprite[]       m_UIGift;
    public UILabel[]        m_UIGiftNum;
    public UILabel[]        m_UIGifPrice;
    public UISprite[]       m_UIGifPriceIcon;
    public GameObject[]     m_GifGoldObj;
    public UILabel          m_labelgameid;
}
public class EffItem
{
    public GameObject goEff;
    public float fLife;
}
public class TaskItem : BaseWnd
{
    UITexture        m_UIBGTexture;
    UISprite         m_UITaskprogerss;
    UISprite         m_UIFishSprite;     //目标鱼
    UISprite         m_UIGoldSprite;
    UISprite         m_UIDiamondSprite;
    UILabel          m_UITaskprogerssLabel;
    UILabel          m_UITitle;
    UILabel          m_UICondition;      //任务条件
    UILabel          m_UIDiamond;
    UILabel          m_UIGold;
    UISprite         m_UIReceiverBtnBg;
    UIButton         m_UIReceiverBtn;
    Transform        m_MiddlePos;
    byte             m_TaskID;
    bool             m_bSceneTask = true;       //默认为游戏场景中名片里的任务
    bool            m_bFinish;

    public bool bFinish
    {
        get { return m_bFinish; }
    }

    public byte TaskID
    {
        get { return m_TaskID; }
    }
    public void Init(GameObject go, bool bscenetask = true)
    {
        m_bSceneTask = bscenetask;
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseWndObject.SetActive(true);

        m_UIBGTexture = m_BaseTrans.GetChild(0).GetComponent<UITexture>();
        m_UITaskprogerss = m_BaseTrans.GetChild(1).GetComponent<UISprite>();
        m_UIFishSprite = m_BaseTrans.GetChild(2).GetComponent<UISprite>();
        m_UITaskprogerssLabel = m_BaseTrans.GetChild(1).GetChild(0).GetComponent<UILabel>();
        m_UITitle = m_BaseTrans.GetChild(3).GetComponent<UILabel>();
        m_UICondition = m_BaseTrans.GetChild(4).GetComponent<UILabel>();
        m_UIDiamond = m_BaseTrans.GetChild(5).GetComponent<UILabel>();
        m_UIGold = m_BaseTrans.GetChild(6).GetComponent<UILabel>();
        m_UIGoldSprite = m_BaseTrans.GetChild(6).GetChild(0).GetComponent<UISprite>();
        m_UIDiamondSprite = m_BaseTrans.GetChild(5).GetChild(0).GetComponent<UISprite>();
        m_UIReceiverBtn = m_BaseTrans.GetChild(7).GetComponent<UIButton>();
        m_UIReceiverBtn.isEnabled = false;
        m_UIReceiverBtnBg = m_BaseTrans.GetChild(7).GetChild(1).GetComponent<UISprite>();
        m_UIReceiverBtnBg.spriteName = "Receiver_Btn1";
        UIEventListener.Get(m_UIReceiverBtn.gameObject).onClick = OnReceiverBtn;

        m_MiddlePos = m_BaseTrans.GetChild(8);
    }

    public void ResetLocalScale()
    {
        m_BaseTrans.localScale = Vector3.one;
    }
    public void UpdateReceiverBtnSate()
    {
        m_UIReceiverBtn.isEnabled = true;
        string str = "";
       // if (m_bSceneTask)
       // {
            str = "Receiver_Btn0";
            m_UIReceiverBtnBg.spriteName = str;

       // }
        //else
        //    str = "Information_GetAward0";
        //m_UIReceiverBtn.normalSprite = str;
        //m_UIReceiverBtn.hoverSprite = str;
        //m_UIReceiverBtn.pressedSprite = str;
        //m_UIReceiverBtn.disabledSprite = str;
    }
    public void SetTaskDate(tagRewardOnce pReward, byte taskID, string taskDec, string taskName, string taskIcon)
    {
        for (byte i = 0; i < pReward.RewardItemVec.Count; ++i )
        {
            uint ItemID = pReward.RewardItemVec[i].ItemID;
            if (FishConfig.Instance.m_ItemInfo.m_ItemMap.ContainsKey(ItemID) != true)
                continue;
            if (i == 0)
            {
                m_UIGoldSprite.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon; 
                m_UIGold.text = pReward.RewardItemVec[i].ItemSum.ToString();
            }
            else if (i == 1)
            {
                m_UIDiamondSprite.spriteName = FishConfig.Instance.m_ItemInfo.m_ItemMap[ItemID].ItemIcon;
                m_UIDiamond.text = pReward.RewardItemVec[i].ItemSum.ToString();
            }
        }
        if (pReward.RewardItemVec.Count == 1)
            m_UIDiamond.gameObject.SetActive(false);
        m_TaskID = taskID;
        m_UITitle.text = taskName;
        m_UICondition.text = taskDec;
        m_UIFishSprite.spriteName = taskIcon;
    }
    public void UpdateTaskProgress(uint curTaskValue, uint taskValu /*完成任务的上限值*/, bool bComplete)
    {
        if (curTaskValue >= taskValu)
            curTaskValue = taskValu;
        m_UITaskprogerssLabel.text = curTaskValue.ToString() + "/" + taskValu.ToString();
        m_UITaskprogerss.fillAmount = (float)(curTaskValue * 1.0f) / (float)(taskValu * 1.0f);
        m_bFinish = bComplete;
       // Debug.Log("ID: " + m_TaskID.ToString() + "  bFinish: " + bComplete.ToString());
        if (bComplete)
        {
            UpdateReceiverBtnSate();
        }
    }
    public void UpdateTaskBg(Texture2D tex)
    {
        m_UIBGTexture.mainTexture = tex;
    }
    void OnReceiverBtn(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

       // Debug.Log("ID: " + m_TaskID.ToString());
        PlayerRole.Instance.TaskManager.GetTaskReward(m_TaskID);
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
            GameObject.Destroy(m_BaseWndObject);
    }
}

public class GlobalNameCards : BaseWnd
{
    bool                m_bMysSelf;
    uint                m_PlayerID;
    GameObject          m_CardsUI;      //名片UIGameObject
    GameObject          m_TaskUI;       //任务UIGameObject
    GameObject          m_TaskItemObj;  //任务条
    NameCardsUIType     m_UIType;
    CardsParentUIType   m_ParentType;   //它的上一级界面
    UIButton[]          m_ChangeUIBtn = new UIButton[2];
    GameObject          m_WarnintObj;
    CardsInfo           m_CardsInfo = new CardsInfo();
    UIGrid              m_UITaskGrid = null;
    UIPanel             m_UIPanel;
    UIScrollView        m_UIScrollView;
    Transform           m_ScrollTrans;
    Texture2D           m_FaceTex;
    Texture2D           m_TaskBgTex;
    UISprite            m_VipLevelIcon;
    List<TaskItem>      m_TaskList = new List<TaskItem>();
    byte[]              m_CharmIndex = new byte[5];
    List<EffItem>       m_ListBtneff=new List<EffItem>();//按钮效果
    float               m_CharmChangeTime;          //魅力变化持续时间
    bool                m_bCharmChange = false;    //播放魅力变化特效

    void  Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.GlobalUIObject[2]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        m_CardsUI = m_BaseTrans.GetChild(3).gameObject;
        m_TaskUI = m_BaseTrans.GetChild(4).gameObject;
        InitCards();
        if (m_bMysSelf)
            InitTask();
        else
        {
            m_ChangeUIBtn[0].isEnabled = false;
            m_BaseTrans.GetChild(2).gameObject.SetActive(false);
        }
       UIEventListener.Get(m_BaseTrans.GetChild(0).GetChild(0).gameObject).onClick = OnCloseUI;

       m_TaskBgTex = (Texture2D)ResManager.Instance.LoadObject("NameCards_task_compelte", "ExtraRes/UI/Image/NameCards/", ResType.ExtraRes, typeof(Texture2D), false);
    }
    void InitCards()
    {
        m_ChangeUIBtn[0] = m_BaseTrans.GetChild(1).GetComponent<UIButton>();
        UIEventListener.Get(m_ChangeUIBtn[0].gameObject).onClick = OnCardsBtn;
        if (m_bMysSelf)
            m_ChangeUIBtn[0].isEnabled = true;

        Transform CardsTrans = m_CardsUI.transform;
        m_CardsInfo.m_UICardsInfo = new UILabel[7];
        m_CardsInfo.m_UIGift = new UISprite[5];
        m_CardsInfo.m_UIGiftNum = new UILabel[5];
        m_CardsInfo.m_GifGoldObj = new GameObject[5];
        m_CardsInfo.m_UIGifPrice = new UILabel[5];
        m_CardsInfo.m_UIGifPriceIcon = new UISprite[5]; 
        for (byte i = 0; i < m_CardsInfo.m_UICardsInfo.Length; ++i)
        {
            m_CardsInfo.m_UICardsInfo[i] = CardsTrans.GetChild(i).GetComponent<UILabel>();
        }
        for (byte j = 0; j < m_CardsInfo.m_UIGift.Length; ++j)
        {
            m_CardsInfo.m_UIGift[j] = CardsTrans.GetChild(j + 7).GetChild(0).GetComponent<UISprite>();
            m_CardsInfo.m_UIGiftNum[j] = CardsTrans.GetChild(j + 7).GetChild(1).GetComponent<UILabel>();
            m_CardsInfo.m_UIGifPrice[j] = CardsTrans.GetChild(j + 7).GetChild(2).GetComponent<UILabel>();
            m_CardsInfo.m_UIGifPriceIcon[j] = CardsTrans.GetChild(j + 7).GetChild(2).GetChild(0).GetComponent<UISprite>();
            m_CardsInfo.m_GifGoldObj[j] = CardsTrans.GetChild(j + 7).GetChild(2).gameObject;
            UIEventListener.Get(m_CardsInfo.m_UIGift[j].gameObject).onClick = OnSendGift; 
        }
        m_CardsInfo.m_UIWarring = CardsTrans.GetChild(6).GetComponent<UILabel>();
        m_CardsInfo.m_UIGender = CardsTrans.GetChild(13).GetComponent<UISprite>();
        m_CardsInfo.m_UIFace = CardsTrans.GetChild(12).GetComponent<UITexture>();
        m_VipLevelIcon = CardsTrans.GetChild(15).GetComponent<UISprite>();
        m_CardsInfo.m_UIAddFriendBtn = CardsTrans.GetChild(14).GetComponent<UIButton>();
        UIEventListener.Get(CardsTrans.GetChild(14).gameObject).onClick = OnAddFriends;
        if (m_bMysSelf)
        {
            m_CardsInfo.m_UIAddFriendBtn.gameObject.SetActive(false);
           // SetAddFriendsState(false);
        }
        m_CardsInfo.m_labelgameid = CardsTrans.GetChild(16).GetComponent<UILabel>();
        SetCardsInfo();
    }
    void InitTask()
    {

        m_TaskItemObj = m_TaskUI.transform.GetChild(0).GetChild(0).gameObject;
        m_UITaskGrid = m_TaskUI.transform.GetChild(0).GetChild(1).GetComponent<UIGrid>();
        m_UIPanel = m_TaskUI.transform.GetChild(0).GetComponent<UIPanel>();
        m_UIScrollView = m_TaskUI.transform.GetChild(0).GetComponent<UIScrollView>();
        m_ScrollTrans = m_TaskUI.transform.GetChild(0);
        m_ChangeUIBtn[1] = m_BaseTrans.GetChild(2).GetComponent<UIButton>();
        m_WarnintObj = m_BaseTrans.GetChild(2).GetChild(1).gameObject;
        UIEventListener.Get(m_ChangeUIBtn[1].gameObject).onClick = OnTaskBtn;
        UpdateCardsInfoWarningState();
    }

    public void ShowNameCardsUI(NameCardsUIType type, CardsParentUIType parent, uint PlayerID, Texture2D tex, bool bMyself = false, bool bFriends = false)
    {
        if (m_BaseWndObject != null)
            return;
        m_bMysSelf = bMyself;
        m_PlayerID = PlayerID;
        m_UIType = type;
        m_ParentType = parent;
        m_FaceTex = tex;
        Init();
        WndManager.Instance.Push(m_BaseTrans);
        UpateCardsFriendBtnStatus();
    }
    void SetCardsInfo()
    {
        //m_CardsInfo.m_UIAddFriendBtn.enabled = false;
        if (m_bMysSelf)
        {
            if (!PlayerRole.Instance.RoleInfo.RoleMe.GetGender())
                m_CardsInfo.m_UIGender.spriteName = "male";
            else
                m_CardsInfo.m_UIGender.spriteName = "female";
            m_CardsInfo.m_UICardsInfo[0].text = PlayerRole.Instance.RoleInfo.RoleMe.GetNickName();
            m_CardsInfo.m_UICardsInfo[1].text = "Lv." + PlayerRole.Instance.RoleInfo.RoleMe.GetLevel().ToString();
            if (FishConfig.Instance.m_TitleInfo.m_TileMap.ContainsKey(PlayerRole.Instance.RoleInfo.RoleMe.GetTitleID()))
                m_CardsInfo.m_UICardsInfo[2].text = FishConfig.Instance.m_TitleInfo.m_TileMap[PlayerRole.Instance.RoleInfo.RoleMe.GetTitleID()].TitleName;
            m_CardsInfo.m_UICardsInfo[4].text = PlayerRole.Instance.RoleInfo.RoleMe.GetAchievementPoint().ToString();
            m_CardsInfo.m_UICardsInfo[3].text = PlayerRole.Instance.CharmManager.GetCharmValue().ToString();
            m_CardsInfo.m_UICardsInfo[5].text = PlayerRole.Instance.RoleInfo.RoleMe.GetIPAddress();
            m_CardsInfo.m_labelgameid.text = PlayerRole.Instance.RoleInfo.RoleMe.GetGameID().ToString();
            m_CardsInfo.m_UIWarring.text = "我收到的魅力道具";
            ShowGif();
            m_CardsInfo.m_UIFace.mainTexture = m_FaceTex;
            if (PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel() > 0)
            {
                m_VipLevelIcon.gameObject.SetActive(true);
                m_VipLevelIcon.spriteName = string.Format("Vip_Icon_{0}", PlayerRole.Instance.RoleInfo.RoleMe.GetVipLevel());
            }

        }
        else if (m_ParentType == CardsParentUIType.Scene_Head)
        {
            if (PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID) == null)
            {
                ShutDown();
                return;
            }
            if (PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetFaceID() < ConstValue.CUSTOM_HEADER)
                m_CardsInfo.m_UIFace.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetFaceID()];
            else
                HeaderManager.Instance.GetPlayerHeader(m_PlayerID, PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetFaceID(), HeaderOptType.HEADER_CARDS, null);
            if (!PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetGender())
                m_CardsInfo.m_UIGender.spriteName = "male";
            else
                m_CardsInfo.m_UIGender.spriteName = "female";
            m_CardsInfo.m_UICardsInfo[0].text = PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetNickName();
            m_CardsInfo.m_UICardsInfo[1].text = "Lv." + PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetLevel().ToString();
            if (FishConfig.Instance.m_TitleInfo.m_TileMap.ContainsKey(PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetTitleID()))
            {
                m_CardsInfo.m_UICardsInfo[2].text = FishConfig.Instance.m_TitleInfo.m_TileMap[PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetTitleID()].TitleName;
            }
            m_CardsInfo.m_UICardsInfo[4].text = PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetAchievementPoint().ToString();
            ChangeCharmColor(PlayerRole.Instance.CharmManager.GetCharmValue(m_PlayerID));
            m_CardsInfo.m_UICardsInfo[5].text = PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetIPAddress();
            m_CardsInfo.m_labelgameid.text = PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetGameID().ToString();

            m_CardsInfo.m_UIWarring.text = StringTable.GetString("Charm_Tips");
            ShowGif();
            if (PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetVipLevel() > 0)
            {
                m_VipLevelIcon.gameObject.SetActive(true);
                m_VipLevelIcon.spriteName = string.Format("Vip_Icon_{0}", PlayerRole.Instance.TableManager.GetTableRole(m_PlayerID).GetVipLevel());
            }   
        }
        else if (m_ParentType == CardsParentUIType.Friend_Head)
        {
           // m_CardsInfo.m_UIAddFriendBtn.isEnabled = true;
            RelationRole pRelation = PlayerRole.Instance.RelationManager.GetFriendMap()[m_PlayerID];
            if (pRelation.GetFaceID() < ConstValue.CUSTOM_HEADER)
                m_CardsInfo.m_UIFace.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[pRelation.GetFaceID()];
            else
                HeaderManager.Instance.GetPlayerHeader(m_PlayerID, pRelation.GetFaceID(), HeaderOptType.HEADER_CARDS, null);
            if (!pRelation.GetGender())
                m_CardsInfo.m_UIGender.spriteName = "male";
            else
                m_CardsInfo.m_UIGender.spriteName = "female";
            m_CardsInfo.m_UICardsInfo[0].text = pRelation.GetNickName();
            m_CardsInfo.m_UICardsInfo[1].text = "Lv." + pRelation.GetLevel().ToString();
            if (FishConfig.Instance.m_TitleInfo.m_TileMap.ContainsKey(pRelation.GetTitleID()))
            {
                m_CardsInfo.m_UICardsInfo[2].text = FishConfig.Instance.m_TitleInfo.m_TileMap[pRelation.GetTitleID()].TitleName;
            }
            m_CardsInfo.m_UICardsInfo[4].text = pRelation.GetAchievementPoint().ToString();
           // m_CardsInfo.m_UICardsInfo[3].text = PlayerRole.Instance.CharmManager.GetCharmValue(pRelation.GetCharmInfo()).ToString();
            ChangeCharmColor(PlayerRole.Instance.CharmManager.GetCharmValue(pRelation.GetCharmInfo()));

            m_CardsInfo.m_UICardsInfo[5].text = pRelation.GetIPAddress();
            m_CardsInfo.m_labelgameid.text = pRelation.GetGameID().ToString();
            m_CardsInfo.m_UIWarring.text = StringTable.GetString("Charm_Tips");
            ShowGif();
            if (pRelation.GetVipLevel() > 0)
            {
                m_VipLevelIcon.gameObject.SetActive(true);
                m_VipLevelIcon.spriteName = string.Format("Vip_Icon_{0}", pRelation.GetVipLevel());
            }
        }
        else
        {
            m_CardsInfo.m_UIFace.mainTexture = m_FaceTex;
            ShowGif();
        }
    }
    void ShowGif()
    {
        for (byte i = 0; i < FishConfig.Instance.m_CharmInfo.CharmMaxItem; ++i)
        {
            if (i >= FishDataInfo.MAX_CHARM_ITEMSUM && i >= m_CardsInfo.m_UIGift.Length)
                break;
            tagCharmOnce pCharm = FishConfig.Instance.m_CharmInfo.CharmIndexMap[i];
            m_CardsInfo.m_UIGift[i].spriteName = pCharm.PicName;
            if (m_bMysSelf)
            {
                m_CardsInfo.m_UIGiftNum[i].text =  PlayerRole.Instance.RoleInfo.RoleMe.GetCharmInfo()[pCharm.Index].ToString();
                m_CardsInfo.m_GifGoldObj[i].SetActive(false);
            }
            else
            {
                if (PlayerRole.Instance.ItemManager.GetAllItemMap() == null)
                {
                    m_CardsInfo.m_UIGiftNum[i].gameObject.SetActive(false);
                    m_CardsInfo.m_GifGoldObj[i].SetActive(true);
                    GetGifPriceIcon(i, FishConfig.Instance.m_CharmInfo.CharmIndexMap[pCharm.Index].UserGlobel, FishConfig.Instance.m_CharmInfo.CharmIndexMap[pCharm.Index].UserMedal, FishConfig.Instance.m_CharmInfo.CharmIndexMap[pCharm.Index].UserCurrcey);
                }
                else
                {
                    if (PlayerRole.Instance.ItemManager.GetItemSum(pCharm.ItemID) == 0)
                    {
                        m_CardsInfo.m_UIGiftNum[i].gameObject.SetActive(false);
                        m_CardsInfo.m_GifGoldObj[i].SetActive(true);
                        GetGifPriceIcon(i, FishConfig.Instance.m_CharmInfo.CharmIndexMap[pCharm.Index].UserGlobel, FishConfig.Instance.m_CharmInfo.CharmIndexMap[pCharm.Index].UserMedal, FishConfig.Instance.m_CharmInfo.CharmIndexMap[pCharm.Index].UserCurrcey);
                    }
                    else
                    {
                        m_CardsInfo.m_GifGoldObj[i].SetActive(false);
                        m_CardsInfo.m_UIGiftNum[i].gameObject.SetActive(true);
                        m_CardsInfo.m_UIGiftNum[i].text = PlayerRole.Instance.ItemManager.GetItemSum(pCharm.ItemID).ToString();
                    }
                }

            }
            
            m_CharmIndex[i] = pCharm.Index;
        }
    }
    void GetGifPriceIcon(byte Indx, uint gold, uint medal, uint diamond)
    {
        if (gold > 0)
        {
            m_CardsInfo.m_UIGifPrice[Indx].text = gold.ToString();
            m_CardsInfo.m_UIGifPriceIcon[Indx].spriteName = "100";
        }
        else if (medal > 0)
        {
            m_CardsInfo.m_UIGifPrice[Indx].text = medal.ToString();
            m_CardsInfo.m_UIGifPriceIcon[Indx].spriteName = "200";
        }
        else
        {
            m_CardsInfo.m_UIGifPrice[Indx].text = diamond.ToString();
            m_CardsInfo.m_UIGifPriceIcon[Indx].spriteName = "500";
        }
            
    }
    void SetAddFriendsState(bool isEnable)
    {
        string str = m_CardsInfo.m_UIAddFriendBtn.normalSprite;
        if (isEnable)
            str = str.Replace("1", "0");
        else
            str = str.Replace("0", "1");
        m_CardsInfo.m_UIAddFriendBtn.normalSprite = str;
        m_CardsInfo.m_UIAddFriendBtn.disabledSprite = str;
        m_CardsInfo.m_UIAddFriendBtn.isEnabled = isEnable ;
    }
    public void SetHeader(uint UserID, Texture2D tex)
    {
        if (UserID != m_PlayerID)
            return;
        if (tex == null)
            m_CardsInfo.m_UIFace.mainTexture = GlobalHallUIMgr.Instance.m_HeadTextureUI[5];
        else
            m_CardsInfo.m_UIFace.mainTexture = tex;
    }
    public void UpateCardsFriendBtnStatus()
    {
        if (m_BaseWndObject == null)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (m_ParentType == CardsParentUIType.Friend_Head)
            SetAddFriendsState(false);
        if (PlayerRole.Instance.RelationManager.IsExitesRelation(m_PlayerID))
            SetAddFriendsState(false);
    }
    public void UpdateOtherQueryUserCardsInfo(QueryRole pQuery)
    {
        if (m_BaseWndObject == null)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (!pQuery.GetGender())
            m_CardsInfo.m_UIGender.spriteName = "male";
        else
            m_CardsInfo.m_UIGender.spriteName = "female";
        m_CardsInfo.m_UICardsInfo[0].text = pQuery.GetNickName();
        m_CardsInfo.m_UICardsInfo[1].text = "Lv." + pQuery.GetLevel().ToString();
        if (FishConfig.Instance.m_TitleInfo.m_TileMap.ContainsKey(pQuery.GetTitleID()))
        {
            m_CardsInfo.m_UICardsInfo[2].text = FishConfig.Instance.m_TitleInfo.m_TileMap[pQuery.GetTitleID()].TitleName;
        }
        m_CardsInfo.m_UICardsInfo[4].text = pQuery.GetAchievementPoint().ToString();
       // m_CardsInfo.m_UICardsInfo[3].text = PlayerRole.Instance.CharmManager.GetCharmValue(pQuery.GetCharmInfo()).ToString();
        ChangeCharmColor(PlayerRole.Instance.CharmManager.GetCharmValue(pQuery.GetCharmInfo()));
        m_CardsInfo.m_UICardsInfo[5].text = pQuery.GetIPAddress();
        m_CardsInfo.m_UIWarring.text = StringTable.GetString("Charm_Tips");
        if (pQuery.GetVipLevel() > 0)
        {
            m_VipLevelIcon.gameObject.SetActive(true);
            m_VipLevelIcon.spriteName = string.Format("Vip_Icon_{0}", pQuery.GetVipLevel());
        }
        
    }
    public void UpdateCardsInfoCharm(uint UserID, int CharmValue)
    {
        if (m_BaseWndObject == null)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (UserID != m_PlayerID)
            return ;
        ChangeCharmColor(CharmValue);
    }
    public void UpdateCardsInfoDate(uint UserID)
    {
        if (m_BaseWndObject == null)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (UserID != m_PlayerID)
            return;
        SetCardsInfo();
    }
    public void UpdateOtherCardsInfoData()
    {
        if (m_BaseWndObject == null)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (m_bMysSelf)
            return;
        SetCardsInfo();
    }
    public void UpdateCardsInfoWarningState()
    {
        if (m_BaseWndObject == null )
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (!m_bMysSelf)
            return;
        m_WarnintObj.SetActive(PlayerRole.Instance.RoleStatesMessage.GetTaskStates());
    }
    public void UpdateCardsInfoDel(uint userID)
    {
        if (m_BaseWndObject == null)
            return;
        if (m_BaseWndObject.activeSelf != true)
            return;
        if (userID != m_PlayerID)
            return;
        ShutDown();
    }
    public void UpdateTaskDate()
    {
        if (m_TaskUI.activeSelf != true)
            return;
        if (PlayerRole.Instance.TaskManager.GetTaskArray() == null)
            return ;
        m_TaskList.Clear();
        ClearGrid();
        m_ScrollTrans.localPosition = new Vector3(0, 2, 0);
        m_UIPanel.clipOffset = new Vector2(0, -2);
        m_UIScrollView.ResetPosition();
        foreach (KeyValuePair<byte, RoleTaskBase> map in PlayerRole.Instance.TaskManager.GetTaskArray())
        {
            TaskItem item = new TaskItem();
            item.Init(m_TaskItemObj);
            string taskName = FishConfig.Instance.m_TaskInfo.m_TaskStr[map.Value.pConfig.TaskID].TaskName;
            string taskDes = FishConfig.Instance.m_TaskInfo.m_TaskStr[map.Value.pConfig.TaskID].TaskDec;
            string taskIcon = FishConfig.Instance.m_TaskInfo.m_TaskStr[map.Value.pConfig.TaskID].TaskIcon;
            uint curTaskValue = PlayerRole.Instance.TaskManager.GetTask(map.Value.pConfig.TaskID).pTaskInfo.TaskValue;
            uint TaskValueMax = PlayerRole.Instance.TaskManager.GetTask(map.Value.pConfig.TaskID).pConfig.TaskEventInfo.FinishValue;
            tagRewardOnce pOnce = FishConfig.Instance.GetRewardOnceByID(map.Value.pConfig.RewardID);
            item.SetTaskDate(pOnce, map.Value.pConfig.TaskID, taskDes, taskName, taskIcon);
            item.UpdateTaskProgress(curTaskValue, TaskValueMax, map.Value.m_TaskEventIsFinish);
            if (map.Value.m_TaskEventIsFinish)
                item.UpdateTaskBg(m_TaskBgTex);
            m_TaskList.Add(item);
        }
        m_TaskList.Sort(CompareItemBySate);
        for (int i = 0 ; i < m_TaskList.Count; ++i)
        {
            m_UITaskGrid.AddChild(m_TaskList[i].m_BaseTrans);
            m_TaskList[i].ResetLocalScale();
        }
    }
    static int CompareItemBySate(TaskItem item1, TaskItem item2)
    {
        if (item1.bFinish && !item2.bFinish)
            return -1;
        else if (!item1.bFinish && item2.bFinish)
            return 1;
        else
            return 0;

    }
    public void UpdateTaskProgress(byte taskID)
    {
        if (PlayerRole.Instance.TaskManager.GetTaskArray() == null)
            return;
        for (byte i = 0; i < m_TaskList.Count; )
        {
            if (m_TaskList[i].TaskID == taskID)
            {
                if (PlayerRole.Instance.TaskManager.GetTaskArray().ContainsKey(taskID) == false)    //领取奖励后删除任务
                {
                    if (int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetTaskStates(), taskID))
                    {
                        GlobalHallUIMgr.Instance.ShowTipBox_0(TipBox_0.TipBox_0_ZiTi.Mission, 2f, FishConfig.Instance.m_TaskInfo.m_TaskMap[taskID].RewardID);
                    }
                    TaskItem item = m_TaskList[i];
                    m_UITaskGrid.RemoveChild(item.m_BaseTrans);
                    item.ShutDown();
                    Utility.ListRemoveAt(m_TaskList, i);
                    UpdateTaskDate();
                }
                else
                {
                    RoleTaskBase pBase = PlayerRole.Instance.TaskManager.GetTaskArray()[taskID];
                    uint curTaskValue = PlayerRole.Instance.TaskManager.GetTask(taskID).pTaskInfo.TaskValue;
                    uint TaskValueMax = pBase.pConfig.TaskEventInfo.FinishValue;
                    if (pBase.m_TaskEventIsFinish)
                        m_TaskList[i].UpdateTaskBg(m_TaskBgTex);
                    m_TaskList[i].UpdateTaskProgress(curTaskValue, TaskValueMax, pBase.m_TaskEventIsFinish);
                }

                break;
            }
            else
            {
                i++;
                //新增一个任务
                if ((i == m_TaskList.Count) && (PlayerRole.Instance.TaskManager.GetTaskArray().ContainsKey(taskID)))
                {
                    UpdateTaskDate();
                }
            }
        }
    }
    //玩家名片按纽
    void OnCardsBtn(GameObject go)
    {
        if (m_UIType == NameCardsUIType.NameCards_UI)
            return;
        m_UIType = NameCardsUIType.NameCards_UI;
        ChangeUI();
        m_CardsUI.SetActive(true);
        m_TaskUI.SetActive(false);
    }
    //玩家任务按纽
    void OnTaskBtn(GameObject go)
    {
        if (m_UIType == NameCardsUIType.Task_UI)
            return;
        m_UIType = NameCardsUIType.Task_UI;
        ChangeUI();
        m_CardsUI.SetActive(false);
        m_TaskUI.SetActive(true);
        UpdateTaskDate();
    }
    void ChangeUI()
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        for (byte i = 0; i < 2; ++i)
        {
            string str;
            if ((byte)m_UIType == i)
            {
                str = m_ChangeUIBtn[i].normalSprite;
                str = str.Replace("0", "1");
                m_CardsUI.SetActive(true);
            }
            else
            {
                str = m_ChangeUIBtn[i].normalSprite;
                str = str.Replace("1", "0");
            }
            m_ChangeUIBtn[i].normalSprite = str;
            m_ChangeUIBtn[i].hoverSprite = str;
            m_ChangeUIBtn[i].pressedSprite = str;
            m_ChangeUIBtn[i].disabledSprite = str;
        }

    }
    void OnAddFriends(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
       // FriendRuntime.ApplyCodeUI.ShowApplyCodeWnd(m_PlayerID);
        GlobalHallUIMgr.Instance.ApplyAddFriend(m_PlayerID);
       // PlayerRole.Instance.RelationManager.AddOtherRoleRelation(m_PlayerID, 1);
    }
    void OnSendGift(GameObject go)
    {
        if (m_bMysSelf)
            return;
        byte Index = 0;
        for (byte i = 0; i < m_CardsInfo.m_UIGift.Length; ++i )
        {
            if (go == m_CardsInfo.m_UIGift[i].gameObject)
            {
                Index = m_CharmIndex[i];
                break;
            }
        }
        bool bSumEnough = true;
        bool bGoldEnough = true;
        if (PlayerRole.Instance.ItemManager.GetItemSum(FishConfig.Instance.m_CharmInfo.CharmIndexMap[Index].ItemID, false) < FishConfig.Instance.m_CharmInfo.CharmIndexMap[Index].ItemSum * 1)
        {
            bSumEnough = false;
        }
        if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_CharmInfo.CharmIndexMap[Index].UserGlobel ||
            PlayerRole.Instance.RoleInfo.RoleMe.GetMedal() < FishConfig.Instance.m_CharmInfo.CharmIndexMap[Index].UserMedal ||
            PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_CharmInfo.CharmIndexMap[Index].UserCurrcey)
        {
            bGoldEnough = false;
        }
        if ((!bSumEnough) && (!bGoldEnough))
        {
            string str = "";
            if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_CharmInfo.CharmIndexMap[Index].UserGlobel)
                str = "金币";
            else if (PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_CharmInfo.CharmIndexMap[Index].UserCurrcey)
                str = "钻石";
            else
                str = "奖牌";
            GlobalEffectMgr.Instance.ShowMessageBox(str, MssageType.GoldLack_Msg_4);
            return;
        }
        if (PlayerRole.Instance.CharmManager.AddTargetCharm(m_PlayerID, Index, 1) == false)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("Charm_Field"), 0.5f, false);
        }
        else
        {
            if (m_ParentType == CardsParentUIType.Friend_Head || m_ParentType == CardsParentUIType.Other_Head)
            {
                GlobalAudioMgr.Instance.PlayCharmMusic(Audio.OrdianryMusic.m_Gift0 + Index);
            }
            EffItem pItem = new EffItem();
            pItem.fLife=0.4f;
            pItem.goEff=GameObject.Instantiate(GlobalHallUIMgr.Instance.GiftBtnEff) as GameObject;
            pItem.goEff.GetComponent<UISprite>().spriteName = m_CardsInfo.m_UIGift[Index].spriteName;
            pItem.goEff.transform.SetParent(m_CardsInfo.m_UIGift[Index].gameObject.transform, false);
            m_ListBtneff.Add(pItem);
        }
            
    }
    void OnCloseUI(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        ShutDown();
    }
    void ClearGrid()
    {
        List<Transform> gridChid = m_UITaskGrid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_UITaskGrid.transform.DetachChildren();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseTrans);
            m_TaskList.Clear();
            GameObject.Destroy(m_BaseWndObject);
            ResManager.Instance.UnloadObject(m_TaskBgTex);
            m_BaseWndObject = null;
        }
    }
    void ChangeCharmColor(int CharmValue)
    {
        int sub = 0;
        if (m_CardsInfo.m_UICardsInfo[3].text.Length != 0)
            sub = int.Parse(m_CardsInfo.m_UICardsInfo[3].text) - CharmValue;
        if (sub > 0)
        {
            m_CharmChangeTime = 0.3f;
            m_bCharmChange = true;
            m_CardsInfo.m_UICardsInfo[3].color = new Color(1f, 0f, 0f);

        }
        else if (sub < 0)
        {
            m_CharmChangeTime = 0.3f;
            m_bCharmChange = true;
            m_CardsInfo.m_UICardsInfo[3].color = new Color(0f, 1f, 0f);

        }
        m_CardsInfo.m_UICardsInfo[3].text = CharmValue.ToString();

    }
    public void Update(float fTime)
    {
        if (m_BaseWndObject == null || !m_BaseWndObject.activeSelf)
            return;
        if (m_bCharmChange)
        {
            m_CharmChangeTime -= fTime;
            if (m_CharmChangeTime <=  0)
            {
                m_bCharmChange = false;
                m_CardsInfo.m_UICardsInfo[3].color = new Color(1, 1, 1);
            }

        }
        for(int i=0;i<m_ListBtneff.Count;)
        {
            EffItem pItem = m_ListBtneff[i];
            pItem.fLife -= fTime;
            if(pItem.fLife <0)
            {
                GameObject.Destroy(pItem.goEff);
                Utility.ListRemoveAt(m_ListBtneff,i);
            }
            else
            {
                i++;
            }
        }
    }
}
