using UnityEngine;
using System.Collections.Generic;

public enum LotteryLevel
{
    Level1 = 1,
    Level2,
    Level3,
    Level4,
    Level5,
    Level6
}

public class SceneLotteryDraw : BaseWnd
{

    GameObject m_LotteryWndObj;
    Transform m_LotteryTrans;
    GameObject m_CardWndObj;
    Transform m_CardTrans;
    UnityEngine.Object m_obj = null;
    GameObject[] m_LotteryLevelBtns = new GameObject[6];
    GameObject[] m_LotteryLevelBtnsCheck = new GameObject[6];
    GameObject m_BtnObj;
    LotteryLevel level;
    LotteryLevel chooseLevel;
    UILabel CurrentAward;
    UILabel CurrentKill;
    UILabel MaxKill;
    GameObject[] m_Sprite = new GameObject[2];
    UISprite ProgressBar_Light;
    GameObject ItemObj;
    UIGrid Grid;
    GameObject GridObj;
    public CardChoose m_cardChoose = new CardChoose();

    Transform m_NoticeTrans;
    GameObject m_NoticeWndObj;
    UILabel m_NoticeLabel;
    GameObject m_NoticeCloseBtn;
    GameObject m_NoticeOKBtn;

    public void Init()
    {
        m_obj = ResManager.Instance.LoadObject("LotteryDraw", "SceneRes/Prefab/UI/LotteryDraw/", ResType.SceneRes);
    }

    public void ShowLottertDraw()
    {
        m_BaseWndObject = (GameObject)GameObject.Instantiate(m_obj);
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        InitBtn();
        InitTopBtn();
        m_BaseWndObject.SetActive(true);
        CurrentAward = m_LotteryTrans.GetChild(2).GetChild(0).GetComponent<UILabel>();
        CurrentKill = m_LotteryTrans.GetChild(3).GetChild(0).GetComponent<UILabel>();
        MaxKill = m_LotteryTrans.GetChild(3).GetChild(1).GetComponent<UILabel>();
        m_Sprite[0] = m_LotteryTrans.GetChild(3).GetChild(4).gameObject;
        m_Sprite[1] = m_LotteryTrans.GetChild(3).GetChild(5).gameObject;
        ProgressBar_Light = m_LotteryTrans.GetChild(3).GetChild(2).GetChild(0).GetComponent<UISprite>();
        m_BtnObj = m_LotteryTrans.GetChild(4).gameObject;
        UIEventListener.Get(m_BtnObj.transform.GetChild(0).gameObject).onClick = ShowFishShow;
        UIEventListener.Get(m_BtnObj.transform.GetChild(1).gameObject).onClick = ShowNotice;
        ItemObj = m_LotteryTrans.GetChild(1).GetChild(0).gameObject;
        GridObj = m_LotteryTrans.GetChild(1).GetChild(1).gameObject;
        Grid = GridObj.GetComponent<UIGrid>();
        OnOpenCheck();

        m_NoticeTrans = m_BaseTrans.GetChild(2);
        m_NoticeWndObj = m_NoticeTrans.gameObject;
        m_NoticeCloseBtn = m_NoticeTrans.GetChild(0).gameObject;
        UIEventListener.Get(m_NoticeCloseBtn).onClick = CloseNotice;
        m_NoticeOKBtn = m_NoticeTrans.GetChild(1).gameObject;
        UIEventListener.Get(m_NoticeOKBtn).onClick = ShowChooseCard;
        m_NoticeLabel = m_NoticeTrans.GetChild(2).GetComponent<UILabel>();
    }

    void InitTopBtn()
    {
        Transform BaseTopTrans = m_LotteryTrans.GetChild(0);
        for (byte i = 0; i < 6; i++)
        {
            m_LotteryLevelBtns[i] = BaseTopTrans.GetChild(i).gameObject;
            m_LotteryLevelBtnsCheck[i] = BaseTopTrans.GetChild(i + 6).gameObject;
            UIEventListener.Get(m_LotteryLevelBtns[i]).onClick = OnClickChangeLevel;
        }
    }

    void InitBtn()
    {
        m_LotteryTrans = m_BaseTrans.GetChild(0);
        m_LotteryWndObj = m_LotteryTrans.gameObject;
        m_CardTrans = m_BaseTrans.GetChild(1);
        m_CardWndObj = m_CardTrans.gameObject;
        UIEventListener.Get(m_LotteryWndObj.transform.GetChild(5).gameObject).onClick = OnCloseCheck;
    }

    void OnOpenCheck()
    {
        WndManager.Instance.Push(m_LotteryTrans);
        uint LotteryScore = PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryScore();
        if (LotteryScore > FishConfig.Instance.m_LotteryConfig.LotteryMap[6].NeedUseScore)
        {
            LotteryScore = FishConfig.Instance.m_LotteryConfig.LotteryMap[6].NeedUseScore;
        }
        uint FishSum = PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryFishSum();
        uint Maxfish = FishConfig.Instance.m_LotteryConfig.MaxLotteryFishSum;
        int lotteryLevel = PlayerRole.Instance.RoleLottery.GetLotteryID();
        if (FishSum >= Maxfish)
        {
            //FishSum = Maxfish;
            FishSum = LotteryScore;
            m_Sprite[1].SetActive(true);
            m_Sprite[0].SetActive(false);
            if (lotteryLevel == 0)
                Maxfish = FishConfig.Instance.m_LotteryConfig.LotteryMap[1].NeedUseScore;
            else if (lotteryLevel == 6)
                Maxfish = FishConfig.Instance.m_LotteryConfig.LotteryMap[(byte)lotteryLevel].NeedUseScore;
            else
                Maxfish = FishConfig.Instance.m_LotteryConfig.LotteryMap[(byte)(lotteryLevel + 1)].NeedUseScore;
        }
        else
        {
            m_Sprite[1].SetActive(false);
            m_Sprite[0].SetActive(true);
        }
        if (lotteryLevel > 0)
        {
            m_BtnObj.transform.GetChild(1).gameObject.SetActive(true);
            m_BtnObj.transform.GetChild(0).gameObject.SetActive(false);
            OnClickChangeLevel(m_LotteryLevelBtns[lotteryLevel - 1]);
        }
        else
        {
            m_BtnObj.transform.GetChild(1).gameObject.SetActive(false);
            m_BtnObj.transform.GetChild(0).gameObject.SetActive(true);
            OnClickChangeLevel(m_LotteryLevelBtns[0]);
        }
        CurrentAward.text = LotteryScore.ToString();
        CurrentKill.text = FishSum.ToString();
        MaxKill.text = Maxfish.ToString();
        ProgressBar_Light.fillAmount = ((float)FishSum) / Maxfish;
        level = (LotteryLevel)lotteryLevel;
        m_LotteryWndObj.SetActive(true);
    }

    void OnCloseCheck(GameObject go)
    {
        CurrentAward.text = "";
        CurrentKill.text = "";
        m_LotteryWndObj.SetActive(false);
        ShutDown();
    }
    public void ShutDown()
    {
        WndManager.Instance.Pop(m_NoticeTrans);
        WndManager.Instance.Pop(m_LotteryTrans);
        WndManager.Instance.Pop(m_CardTrans);
        DestroySelf();
        ResManager.Instance.UnloadObject(m_BaseTrans);
    }
    public void DestroySelf()
    {
        if (m_BaseTrans != null)
        {
            GameObject.Destroy(m_BaseWndObject);
            m_BaseTrans = null;
        }
    }
    public void HideLotteryDraw()
    {
        m_BaseWndObject.SetActive(false);
    }

    void OnClickChangeLevel(GameObject go)
    {
        ClearGrid();
        for (byte i = 0; i < m_LotteryLevelBtns.Length; ++i)
        {
            if (go == m_LotteryLevelBtns[i])
            {
                m_LotteryLevelBtnsCheck[i].SetActive(true);
                m_LotteryLevelBtns[i].SetActive(false);
                chooseLevel = (LotteryLevel)(i + 1);
                SetScrollViewDate();
            }
            else
            {
                m_LotteryLevelBtnsCheck[i].SetActive(false);
                m_LotteryLevelBtns[i].SetActive(true);
            }
        }
    }

    void ShowFishShow(GameObject go)
    {
        GlobalHallUIMgr.Instance.ShowFishShow(true);
    }
    void SetScrollViewDate()
    {
        tagLotteryOnce tagLottery = new tagLotteryOnce();
        if (FishConfig.Instance.m_LotteryConfig.LotteryMap.ContainsKey((byte)chooseLevel) == true)
        {
            tagLottery = FishConfig.Instance.m_LotteryConfig.LotteryMap[(byte)chooseLevel];
        }
        for (byte i = 0; i < tagLottery.RewardVec.Count; ++i)
        {
            tagRewardOnce pReward = FishConfig.Instance.m_RewardConfig.RewardMap[tagLottery.RewardVec[i].RewardID];
            if (pReward == null)
                continue;
            uint itemID = pReward.RewardItemVec[0].ItemID;
            tagItemConfig pItme = FishConfig.Instance.m_ItemInfo.m_ItemMap[itemID];
            if (pItme == null)
                continue;
            LotteryDrawItem item = new LotteryDrawItem();
            item.m_IconName = pItme.ItemIcon;
            item.m_NumStr += pReward.RewardItemVec[0].ItemSum;
            if (pItme.ItemTypeID == EItemType.IT_Globel)
            {
                item.m_NumStr += "金币";
            }
            else if (pItme.ItemTypeID == EItemType.IT_Currey)
            {
                item.m_NumStr += "钻石";
            }
            else
            {
                item.m_NumStr += pItme.ItemName;
            }
            Transform Item = item.Init(ItemObj);
            Grid.AddChild(Item);
            Item.localScale = Vector3.one;
        }

    }
    void ClearGrid()
    {
        List<Transform> gridChid = Grid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        Grid.transform.DetachChildren();
    }
    void ShowNotice(GameObject go)
    {
        if (PlayerRole.Instance.RoleLottery.IsCanStarLottery((byte)level))
        {
            if ((byte)level < 6)
            {
                m_NoticeWndObj.SetActive(true);
                WndManager.Instance.Push(m_NoticeTrans);
                string levelStr = null;
                uint nextScore = FishConfig.Instance.m_LotteryConfig.LotteryMap[(byte)(level + 1)].NeedUseScore;
                uint currentScore = PlayerRole.Instance.RoleInfo.RoleMe.GetLotteryScore();
                switch (level)
                {
                    case LotteryLevel.Level1:
                        levelStr = "青铜";
                        break;
                    case LotteryLevel.Level2:
                        levelStr = "白银";
                        break;
                    case LotteryLevel.Level3:
                        levelStr = "黄金";
                        break;
                    case LotteryLevel.Level4:
                        levelStr = "白金";
                        break;
                    case LotteryLevel.Level5:
                        levelStr = "至尊";
                        break;
                }
                string str = string.Format(StringTable.GetString("UOM_Lottery_Notice"), levelStr, nextScore - currentScore);
                m_NoticeLabel.text = str;

            }
            else
            {
                ShowChooseCard(null);
            }
        }
    }
    void CloseNotice(GameObject go)
    {
        m_NoticeWndObj.SetActive(false);
        WndManager.Instance.Pop(m_NoticeTrans);
    }
    void ShowChooseCard(GameObject go)
    {
        if (go != null)
        {
            m_NoticeWndObj.SetActive(false);
            WndManager.Instance.Pop(m_NoticeTrans);
        }
        m_LotteryWndObj.SetActive(false);
        WndManager.Instance.Pop(m_LotteryTrans);
        m_cardChoose.Init(m_CardWndObj, (byte)level, m_BaseWndObject);
    }

    public void Update(float deltaTime)
    {
        m_cardChoose.Update(deltaTime);
        if (m_cardChoose.UpdateGetCard(deltaTime))
        {
            m_cardChoose.ShutDownChooseCard();
        }
    }
}

public class LotteryDrawItem : BaseWnd
{
    UISprite m_Type;
    UILabel m_Num;
    public string m_IconName;
    public string m_NumStr;

    public Transform Init(GameObject go)
    {
        m_BaseWndObject = GameObject.Instantiate(go) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        if (m_BaseWndObject.activeSelf != true)
        {
            m_BaseWndObject.SetActive(true);
        }
        m_Num = m_BaseTrans.GetChild(0).GetComponent<UILabel>();
        m_Type = m_BaseTrans.GetChild(1).GetComponent<UISprite>();
        m_Num.text = m_NumStr;
        m_Type.spriteName = m_IconName;
        return m_BaseTrans;
    }
}

public class CardChoose : BaseWnd
{
    GameObject[] cardsObj = new GameObject[6];
    UILabel CountDown;
    GameObject m_CoundDownObj;
    GameObject[] m_Btn = new GameObject[2];
    GameObject m_LotteryWnd;
    GameObject[] m_Effect = new GameObject[2];
    Object m_EffectLoad;
    Object m_EffectCardTrunLoad;
    Object m_GetCardLoad;
    GameObject chooseCard;
    GameObject m_mask;

    float count;
    bool isCount = false;
    int numCard = 0;
    float countCard;
    bool cardTurnBack = false;
    byte level;
    bool isShowOtherCard = false;
    int chooseID;
    List<int> RewardIDList = new List<int>();
    float ShowOtherCount;
    bool isShowShuffleCard = false;
    float showShuffleCount;
    bool isShowEffect = false;
    float showEffectCount;
    int cardIndex;

    GameObject lotteryCard;
    CLineMove m_path;
    bool isGetCard = false;
    float cardAnimCount;
    float cardGetCount;

    public void Init(GameObject go, byte LotteryID, GameObject LotteryWnd)
    {
        m_LotteryWnd = LotteryWnd;
        m_BaseWndObject = go;
        m_BaseTrans = go.transform;
        go.SetActive(true);
        WndManager.Instance.Push(m_BaseTrans);
        SetCardDate(LotteryID);
        m_CoundDownObj = m_BaseTrans.GetChild(6).gameObject;
        CountDown = m_BaseTrans.GetChild(6).GetChild(0).GetComponent<UILabel>();
        m_BaseTrans.GetChild(7).gameObject.SetActive(true);
        m_Effect[0] = m_BaseTrans.GetChild(7).GetChild(0).gameObject;
        m_Effect[1] = m_BaseTrans.GetChild(7).GetChild(1).gameObject;
        m_mask = m_BaseTrans.GetChild(2).gameObject;
        isCount = true;
        count = 11;
        m_Btn[0] = m_BaseTrans.GetChild(4).gameObject;
        UIEventListener.Get(m_Btn[0]).onClick = BeginChooseCard;
        m_Btn[1] = m_BaseTrans.GetChild(5).gameObject;
        UIEventListener.Get(m_Btn[1]).onClick = CloseClick;
        m_Effect[0].SetActive(true);
        m_Effect[1].SetActive(true);
        SetTurnToFrontAnim(true);
        level = LotteryID;
        InitEffect();
    }

    void InitEffect()
    {
        m_EffectLoad = ResManager.Instance.LoadObject("Ef_CardSurface", "SceneRes/Prefab/effect/Lottery/", ResType.SceneRes);
        m_EffectCardTrunLoad = ResManager.Instance.LoadObject("Ef_CardTurn", "SceneRes/Prefab/effect/Lottery/", ResType.SceneRes);
        m_GetCardLoad = ResManager.Instance.LoadObject("MoveCardDm", "SceneRes/Prefab/effect/Lottery/", ResType.SceneRes);
    }
    void SetTurnToFrontAnim(bool isPlay)
    {
        for (byte i = 0; i < cardsObj.Length; i++)
        {
            cardsObj[i].transform.GetChild(0).GetComponent<Animator>().SetBool("backToFront", isPlay);
        }
    }

    void SetCardDate(byte LotteryID)
    {
        tagLotteryOnce tagLottery = new tagLotteryOnce();
        if (FishConfig.Instance.m_LotteryConfig.LotteryMap.ContainsKey(LotteryID) == true)
        {
            tagLottery = FishConfig.Instance.m_LotteryConfig.LotteryMap[LotteryID];
        }
        for (byte i = 0; i < tagLottery.RewardVec.Count; ++i)
        {
            cardsObj[i] = m_BaseTrans.GetChild(0).GetChild(i).gameObject;
            tagRewardOnce pReward = FishConfig.Instance.m_RewardConfig.RewardMap[tagLottery.RewardVec[i].RewardID];
            RewardIDList.Add(tagLottery.RewardVec[i].RewardID);
            if (pReward == null)
                continue;
            uint itemID = pReward.RewardItemVec[0].ItemID;
            tagItemConfig pItme = FishConfig.Instance.m_ItemInfo.m_ItemMap[itemID];
            if (pItme == null)
                continue;
            CardItem item = new CardItem();
            item.m_IconName = pItme.ItemIcon;
            if (pItme.ItemTypeID == EItemType.IT_Globel)
            {
                item.m_NameStr = "金币";
            }
            else if (pItme.ItemTypeID == EItemType.IT_Currey)
            {
                item.m_NameStr = "钻石";
            }
            else
            {
                item.m_NameStr = pItme.ItemName;
            }
            item.m_NumStr = pReward.RewardItemVec[0].ItemSum.ToString();
            item.InitItem(cardsObj[i].transform.GetChild(0).GetChild(0).gameObject);
            cardsObj[i].transform.GetChild(0).GetChild(0).gameObject.SetActive(true);
        }
    }

    public void Update(float deltaTime)
    {
        if (isCount && m_CoundDownObj != null)
        {
            m_CoundDownObj.SetActive(true);
            count -= deltaTime;
            CountDown.text = ((int)count).ToString();
            if (count <= 0)
            {
                isCount = false;
                count = 11;
                m_CoundDownObj.SetActive(false);
                if (m_Btn[0].activeSelf)
                {
                    BeginChooseCard(m_Btn[0]);
                }
                else if (m_Btn[1].activeSelf)
                {
                    CloseClick(null);
                }
                else
                {
                    ChooseCard(cardsObj[0].transform.GetChild(0).gameObject);
                }
            }
        }

        if (cardTurnBack)
        {
            countCard += deltaTime;
            if (countCard >= 0.1f)
            {
                cardsObj[numCard].transform.GetChild(0).GetComponent<Animator>().SetBool("frontToBack", true);
                cardsObj[numCard].transform.GetChild(0).GetChild(0).gameObject.SetActive(false);
                numCard++;
                countCard = 0;
                //if (numCard==1)
                {
                    GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_lottery2);
                }
                if (numCard > 5)
                {
                    cardTurnBack = false;
                    showShuffleCount = 0.0f;
                    isShowShuffleCard = true;
                    numCard = 0;
                }
            }
        }

        if (isShowOtherCard)
        {
            ShowOtherCount += deltaTime;
            if (ShowOtherCount >= 0.3f)
            {
                m_Btn[1].SetActive(true);
                ShowOtherCard(chooseID);
                isShowOtherCard = false;
                GameObject go = (GameObject)GameObject.Instantiate(m_EffectLoad);
                GameObject effect = (GameObject)GameObject.Instantiate(m_EffectCardTrunLoad);
                GlobalEffectMgr.SetTrailEffectOnTopWnd(go);
                GlobalEffectMgr.SetMeshRendererOnTopWnd(go);
                go.transform.SetParent(chooseCard.transform, false);
                go.transform.localPosition = Vector3.zero;
                effect.transform.SetParent(chooseCard.transform, false);
                effect.transform.localPosition = Vector3.zero;

                UISprite[] sprite = chooseCard.transform.GetChild(0).GetComponentsInChildren<UISprite>();
                UILabel[] label = chooseCard.transform.GetComponentsInChildren<UILabel>();
                foreach (UISprite s in sprite)
                {
                    s.depth = 121;
                }
                chooseCard.transform.GetChild(0).GetComponent<UISprite>().depth = 120;
                foreach (UILabel l in label)
                {
                    l.depth = 121;
                }
                m_mask.GetComponent<UITexture>().depth = 119;
                isCount = true;
                count = 11;
            }
        }

        if (isShowShuffleCard)
        {
            showShuffleCount += deltaTime;
            if (showShuffleCount >= 0.5f)
            {
                ShuffleCard();
            }
            if (showShuffleCount >= 3)
            {
                isShowEffect = true;
                showEffectCount = 0.0f;
                cardIndex = 0;
                isCount = true;
                count = 11;
                isShowShuffleCard = false;
                m_Effect[0].SetActive(true);
            }
        }
        if (isShowEffect)
        {
            showEffectCount += deltaTime;
            if (showEffectCount >= 0.1f)
            {
                GameObject go = (GameObject)GameObject.Instantiate(m_EffectLoad);
                GlobalEffectMgr.SetTrailEffectOnTopWnd(go);
                GlobalEffectMgr.SetMeshRendererOnTopWnd(go);
                go.transform.SetParent(cardsObj[cardIndex].transform.GetChild(0), false);
                go.transform.localPosition = Vector3.zero;
                if (cardIndex == 5)
                {
                    for (byte i = 0; i < cardsObj.Length; i++)
                    {
                        UIEventListener.Get(cardsObj[i].transform.GetChild(0).gameObject).onClick = ChooseCard;
                    }
                    isShowEffect = false;
                }
                else
                {
                    cardIndex++;
                    showEffectCount = 0.0f;
                }
            }
        }
    }

    public bool UpdateGetCard(float delta)
    {
        if (isGetCard)
        {
            cardGetCount += delta;
            if (cardGetCount >= 1.5f)
            {
                lotteryCard.transform.position = m_path.Update(delta);
            }
            if (m_path.IsEnd())
                isGetCard = false;
            return m_path.IsEnd();
        }
        else
            return false;
    }
    void ShowEffect()
    {
        for (byte i = 0; i < cardsObj.Length; i++)
        {
            GameObject go = (GameObject)GameObject.Instantiate(m_EffectLoad);
            GlobalEffectMgr.SetTrailEffectOnTopWnd(go);
            GlobalEffectMgr.SetMeshRendererOnTopWnd(go);
            go.transform.SetParent(cardsObj[i].transform.GetChild(0), false);
            go.transform.localPosition = Vector3.zero;
        }
    }
    void ShuffleCard()
    {
        m_BaseTrans.GetChild(0).GetComponent<Animator>().enabled = true;
    }

    void BeginChooseCard(GameObject go)
    {
        PlayerRole.Instance.RoleLottery.RoleChangeLotteryUIStates(true);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_lottery1);
        SetTurnToFrontAnim(false);
        isCount = false;
        m_CoundDownObj.SetActive(false);
        m_Btn[0].SetActive(false);
        m_Effect[1].SetActive(false);
        cardTurnBack = true;
        countCard = 0;
        m_Effect[0].SetActive(false);
    }

    void CloseClick(GameObject go)
    {
        PlayerRole.Instance.RoleLottery.RoleChangeLotteryUIStates(false);
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_lottery4);       
        lotteryCard = null;
        if (chooseCard != null)
        {
            lotteryCard = (GameObject)GameObject.Instantiate(m_GetCardLoad);
            lotteryCard.transform.GetChild(0).GetChild(1).GetComponent<UISprite>().spriteName
                = chooseCard.transform.GetChild(0).GetChild(0).GetComponent<UISprite>().spriteName;
            lotteryCard.transform.GetChild(0).GetChild(0).GetChild(0).GetComponent<UILabel>().text
                = chooseCard.transform.GetChild(0).GetChild(1).GetComponent<UILabel>().text;
            lotteryCard.transform.GetChild(0).GetChild(0).GetChild(1).GetComponent<UILabel>().text
                = chooseCard.transform.GetChild(0).GetChild(2).GetComponent<UILabel>().text;
            lotteryCard.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
            lotteryCard.transform.position = chooseCard.transform.position;
            Vector2 vecPos = SceneRuntime.SceneLogic.PlayerMgr.MySelf.Launcher.LauncherPos;
            m_path = new CLineMove(lotteryCard.transform.position, new Vector3(vecPos.x, vecPos.y, 0), 0.0f, 0.5f);
            lotteryCard.SetActive(false);
        }
        ShutDown();
        if (lotteryCard != null)
        {
            cardGetCount = 0.0f;
            isGetCard = true;
            lotteryCard.SetActive(true);
        }
        
    }
    void ChooseCard(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_lottery3);
        isCount = false;
        m_CoundDownObj.SetActive(false);
        chooseCard = go;
        DestroyEffect();
        bool isLotterySuccess = PlayerRole.Instance.RoleLottery.GetLotteryReward(level);
        if (isLotterySuccess == false)
        {
            CloseClick(null);
        }
    }
    void DestroyEffect()
    {
        for (byte i = 0; i < cardsObj.Length; i++)
        {
            UIEventListener.Get(cardsObj[i].transform.GetChild(0).gameObject).onClick = null;
            if (cardsObj[i].transform.GetChild(0).childCount > 1)
            {
                GameObject card = cardsObj[i].transform.GetChild(0).GetChild(1).gameObject;
                if (card != null)
                    GameObject.Destroy(card);
                ResManager.Instance.UnloadObject(card.transform);
            }
        }
    }
    public void ShowChooseCard(ushort RewardID)
    {
        RewardIDList.Remove(RewardID);
        chooseID = RewardID;
        ShowOneCard(RewardID, chooseCard);
        isShowOtherCard = true;
        ShowOtherCount = 0;
    }

    void ShowOtherCard(int RewardID)
    {
        tagLotteryOnce tagLottery = new tagLotteryOnce();
        if (FishConfig.Instance.m_LotteryConfig.LotteryMap.ContainsKey(level) == true)
        {
            tagLottery = FishConfig.Instance.m_LotteryConfig.LotteryMap[level];
        }
        for (byte i = 0; i < tagLottery.RewardVec.Count; ++i)
        {
            int id = tagLottery.RewardVec[i].RewardID;
            if (cardsObj[i].transform.GetChild(0).gameObject != chooseCard && RewardIDList.Count != 0)
            {
                System.Random rand = new System.Random();
                int index = rand.Next(0, RewardIDList.Count);
                ShowOneCard((ushort)RewardIDList[index], cardsObj[i].transform.GetChild(0).gameObject);
                RewardIDList.RemoveAt(index);
            }
        }
    }
    public void ShowOneCard(ushort RewardID, GameObject go)
    {
        tagRewardOnce pReward = FishConfig.Instance.m_RewardConfig.RewardMap[RewardID];
        uint itemID = pReward.RewardItemVec[0].ItemID;
        tagItemConfig pItme = FishConfig.Instance.m_ItemInfo.m_ItemMap[itemID];
        go.transform.GetChild(0).GetChild(0).GetComponent<UISprite>().spriteName = pItme.ItemIcon;
        go.transform.GetChild(0).GetChild(1).GetComponent<UILabel>().text = pReward.RewardItemVec[0].ItemSum.ToString();
        string typeStr;
        if (pItme.ItemTypeID == EItemType.IT_Globel)
        {
            typeStr = "金币";
        }
        else if (pItme.ItemTypeID == EItemType.IT_Currey)
        {
            typeStr = "钻石";
        }
        else
        {
            typeStr = pItme.ItemName;
        }
        go.transform.GetChild(0).GetChild(2).GetComponent<UILabel>().text = typeStr;
        go.transform.GetChild(0).gameObject.SetActive(true);
        Animator anim = go.transform.GetComponent<Animator>();
        anim.SetBool("frontToBack", false);
        anim.SetBool("backToFront", true);
    }

    void ShutDown()
    {
        if (m_LotteryWnd != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_LotteryWnd);
        }
    }
    public void ShutDownChooseCard()
    {
        if (lotteryCard != null)
        {
            GameObject.Destroy(lotteryCard);
            lotteryCard = null;
        }
    }
}

public class CardItem : BaseWnd
{
    UISprite m_Type;
    UILabel m_Name;
    UILabel m_Num;
    public string m_IconName;
    public string m_NameStr;
    public string m_NumStr;

    public void InitItem(GameObject go)
    {
        m_BaseWndObject = go;
        m_BaseTrans = go.transform;
        m_Type = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_Name = m_BaseTrans.GetChild(2).GetComponent<UILabel>();
        m_Num = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        m_Type.spriteName = m_IconName;
        m_Name.text = m_NameStr;
        m_Num.text = m_NumStr;
    }
}