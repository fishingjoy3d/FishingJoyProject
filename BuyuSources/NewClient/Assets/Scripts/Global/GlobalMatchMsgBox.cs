using UnityEngine;
using System;
using System.Collections.Generic;

public enum MatchMsgBoxType
{
    Match_Start = 0,          //比赛开始
    Match_Leave,         //比赛中途离开
    Match_End,          //比赛结束
    Match_Good,         //太棒了
    Match_BuyGold,       //续币
    Match_ProtectNew,         //新手保护
}

struct MatchMsgBoxBtn
{
    public GameObject   m_BtnObj;
    public bool         m_bCheck;
}

public class GlobalMatchMsgBox : BaseWnd
{
    UILabel             m_MsgDecLabel;
    UISprite            m_MsgTitleSprite;
    UISprite            m_FishSprite;
    UILabel             m_WarningTipsLabel;
    MatchMsgBoxBtn[]    m_MsgBtn = new MatchMsgBoxBtn[5];
    float               m_LifeTime;
    float               m_dltalTime = 1;
    byte                m_MatchID;
    PayType             m_PayType;

    void Init()
    {
        m_BaseWndObject = GameObject.Instantiate(GlobalHallUIMgr.Instance.GlobalUIObject[3]) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_MsgTitleSprite = m_BaseTrans.GetChild(0).GetComponent<UISprite>();
        m_MsgDecLabel = m_BaseTrans.GetChild(1).GetComponent<UILabel>();
        m_FishSprite = m_BaseTrans.GetChild(8).GetComponent<UISprite>();
        m_WarningTipsLabel = m_BaseTrans.GetChild(9).GetComponent<UILabel>();
        for (byte i = 0; i < m_MsgBtn.Length; ++i )
        {
            m_MsgBtn[i].m_BtnObj = m_BaseTrans.GetChild(2 + i).gameObject;
            m_MsgBtn[i].m_bCheck = false;
            UIEventListener.Get(m_MsgBtn[i].m_BtnObj).onClick = onClickMsgBtn;
        }
        
        UIEventListener.Get(m_BaseTrans.GetChild(7).gameObject).onClick = onClickOnOk;

    }
	public void Update(float deltaTime)
    {
        if (HallRunTimeInfo.Instance_UI == null && SceneRuntime.Instance == null)
            return;
        if (m_dltalTime < 2)
            m_dltalTime += deltaTime;
        if ( SystemTime.Instance.GetSystemDateTime.Second == 0 && m_dltalTime > 1)
        {
            if (PlayerRole.Instance.MonthManager.GetSignUpList() != null)
            {
                foreach (KeyValuePair<Byte, Byte> map in PlayerRole.Instance.MonthManager.GetSignUpList())
                {
                    if (PlayerRole.Instance.MonthManager.MonthIsBeginStar(map.Key) && 
                        PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() == 0)     //比赛开始了
                    {
                        ShowMatchMsgBox(StringTable.GetString("Match_Start_Warning"), map.Key, MatchMsgBoxType.Match_Start);
                        m_dltalTime = 0;
                    }
                }
            }
        }
        //if (m_BaseWndObject != null)
        //{
        //    m_LifeTime -= deltaTime;
        //    if (m_LifeTime <= 0)
        //        ShutDown();
        //}
        
    }
    public void ShowMatchMsgBox(string strDec, byte MatchID, MatchMsgBoxType type)
    {
        if (m_BaseWndObject != null)
        {
            ShutDown();
        }

        Init();
        //m_LifeTime = 10;
        m_MatchID = MatchID;
        m_MsgDecLabel.text = strDec;
        switch (type)
        {
            case MatchMsgBoxType.Match_Start:           //比赛开始
                {
                    m_MsgBtn[0].m_BtnObj.SetActive(true);
                }
                break;
            case MatchMsgBoxType.Match_Leave:           //中途离开
                {
                    m_MsgBtn[1].m_BtnObj.SetActive(true);
                    m_MsgBtn[2].m_BtnObj.SetActive(true);
                }
                break;
            case MatchMsgBoxType.Match_End:             //正常的比赛结束
                {
                    m_MsgBtn[4].m_BtnObj.SetActive(true);
                    m_MsgTitleSprite.gameObject.SetActive(true);
                    m_FishSprite.gameObject.SetActive(false);
                }
                break;
            case MatchMsgBoxType.Match_Good:            //比赛结束获得好成绩
                {
                    m_MsgBtn[4].m_BtnObj.SetActive(true);
                    m_MsgTitleSprite.gameObject.SetActive(true);
                    m_FishSprite.gameObject.SetActive(false);
                    m_MsgTitleSprite.spriteName = "Inner_MsgBox_Title1";
                }
                break;
            case MatchMsgBoxType.Match_BuyGold:         //续币
                {
                    m_FishSprite.spriteName = "Inner_Message_Box_Bg1";
                    m_MsgDecLabel.text = GetMatchContinueGoldStr();
                    
                }
                break;
        }
        WndManager.Instance.Push(m_BaseWndObject);
    }
    string GetMatchContinueGoldStr()
    {
        byte matchID = PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID();
        byte continueSum = PlayerRole.Instance.RoleInfo.RoleMe.GetMonthAddGlobelNum();
        uint MaxtSum = FishConfig.Instance.m_MatchInfo.m_MonthMap[matchID].MaxConvertSum;//(uint)continueSum + 1;
        string strDec = "";
        if (continueSum>=MaxtSum || !FishConfig.Instance.m_MatchInfo.m_MonthMap[matchID].m_ConvertMap.ContainsKey((byte)(continueSum + 1)))
        {
            strDec = StringTable.GetString("Match_BuyGold_Warning2");
            m_MsgBtn[1].m_BtnObj.SetActive(true);
            m_MsgBtn[1].m_BtnObj.transform.localPosition = m_MsgBtn[4].m_BtnObj.transform.localPosition;
        }
        else
        {
            tagMonthConvert pContinue = FishConfig.Instance.m_MatchInfo.m_MonthMap[matchID].m_ConvertMap[(byte)(continueSum + 1)];
            if (pContinue.LostCurrey > 0)
            {
                strDec = string.Format(StringTable.GetString("Match_BuyGold_Warning"), 
                    FishConfig.Instance.m_MatchInfo.m_MonthMap[matchID].MaxConvertSum - continueSum, 
                    pContinue.LostCurrey, "钻石", pContinue.AddMonthGlobel);
            }
            else if (pContinue.LostGlobel > 0)
            {
                strDec = string.Format(StringTable.GetString("Match_BuyGold_Warning"), 
                    FishConfig.Instance.m_MatchInfo.m_MonthMap[matchID].MaxConvertSum - continueSum,
                    pContinue.LostGlobel, "金币", pContinue.AddMonthGlobel);
            }
            else
            {
                strDec = string.Format(StringTable.GetString("Match_BuyGold_Warning"), 
                    FishConfig.Instance.m_MatchInfo.m_MonthMap[matchID].MaxConvertSum - continueSum,
                    pContinue.LostMedal, "奖币", pContinue.AddMonthGlobel);
            }
            m_MsgBtn[1].m_BtnObj.SetActive(true);
            m_MsgBtn[3].m_BtnObj.SetActive(true);
            IsGoldEnough();
        }

        return strDec;
    }
    void IsGoldEnough()
    {
        byte matchID = PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID();
        byte addGoldNum  = (byte)(PlayerRole.Instance.RoleInfo.RoleMe.GetMonthAddGlobelNum() + 1);
        string strDec = "";
        if (addGoldNum > FishConfig.Instance.m_MatchInfo.m_MonthMap[matchID].MaxConvertSum)
        {
            //无法续币

        }
        else if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() < FishConfig.Instance.m_MatchInfo.m_MonthMap[matchID].m_ConvertMap[addGoldNum].LostGlobel)
        {
            strDec = string.Format(StringTable.GetString("Match_BuyGold_Warning3"), "金币");
            m_PayType = PayType.Gold;
            m_WarningTipsLabel.gameObject.SetActive(true);
        } 
        else if (PlayerRole.Instance.RoleInfo.RoleMe.GetCurrency() < FishConfig.Instance.m_MatchInfo.m_MonthMap[matchID].m_ConvertMap[addGoldNum].LostCurrey)
        {
            strDec = string.Format(StringTable.GetString("Match_BuyGold_Warning3"), "钻石");
            m_PayType = PayType.Diamond;
            m_WarningTipsLabel.gameObject.SetActive(true);
        }
        m_WarningTipsLabel.text = strDec;
    }
    void onClickMsgBtn(GameObject go)
    {
        byte tag = 0;
        for (byte i = 0; i < m_MsgBtn.Length; ++i)
        {
            if (go == m_MsgBtn[i].m_BtnObj)
            {
                tag = i;
            }
        }
        switch (tag)
        {
            case 0:         //立即进入按纽
                PlayerRole.Instance.MonthManager.JoinMonth(m_MatchID);
                ShutDown();
                break;
            case 1:         //离开游戏按纽
                if (SceneRuntime.Instance != null)
                {
                    SceneRuntime.SceneLogic.BackToHall();
                    //int bg = UnityEngine.Random.Range(4, 7);
                    //int bg = 1;
                    //GlobalAudioMgr.Instance.PlayerBGMusic(bg);
                }
                ShutDown();
                break;
            case 2:         //继续游戏按纽
                ShutDown();
                break;
            case 3:         //立即购买
                if (m_WarningTipsLabel.gameObject.activeSelf)
                    GlobalHallUIMgr.Instance.ShowPayWnd(m_PayType);
                else
                    PlayerRole.Instance.MonthManager.AddMonthGlobel();
                ShutDown();
                break;
            case 4:         //确认
                ShutDown();
                break;
        }
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

    }
    void onClickOnOk(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        ShutDown();
    }
    public void ShutDown()
    {

        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }

    }
}
