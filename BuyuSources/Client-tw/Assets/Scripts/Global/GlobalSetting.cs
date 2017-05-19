using UnityEngine;
using System.Collections.Generic;

class SettingWind : GlobalBaseWind
{
    SettIngWindScrollBar m_BgVBar = new SettIngWindScrollBar();
    SettIngWindScrollBar m_VBg = new SettIngWindScrollBar();
    WindBtn[] Btns = new WindBtn[4];
    float m_BgValue = 1;          //背景音乐音量
    float m_Value = 1;            //音效音量
    public Object Init()
    {
        // m_WindId = GlobalBtnTouchBoxPool.GlobalWindStatue.GLobal_Setting;
        Object obj = Initobj("SettingWind");
        return obj;
    }
    public void InitGameObjcet()
    {
        Init_GameObj(false);
        InitSettingBtns();
    }
    void InitSettingBtns()
    {
        m_BgValue = GlobalAudioMgr.Instance.BgValue;
        m_Value = GlobalAudioMgr.Instance.Value;

        m_BgVBar.m_Bar = BaseTranF.GetChild(1).GetChild(0).GetComponent<UIScrollBar>();
        m_BgVBar.m_ValueBg = BaseTranF.GetChild(1).GetChild(0).GetChild(2).GetComponent<UIPanel>();
        m_BgVBar.m_Bar.value = m_BgValue;
        m_BgVBar.m_SpWidth = -m_BgVBar.m_ValueBg.GetViewSize().x;
        m_BgVBar.render = m_BgVBar.m_Bar.transform.GetComponent<Renderer>();

        m_VBg.m_Bar = BaseTranF.GetChild(2).GetChild(0).GetComponent<UIScrollBar>();
        m_VBg.m_ValueBg = BaseTranF.GetChild(2).GetChild(0).GetChild(2).GetComponent<UIPanel>();
        m_VBg.m_Bar.value = m_Value;
        m_VBg.m_SpWidth = -m_VBg.m_ValueBg.GetViewSize().x;
        m_VBg.render = m_VBg.m_Bar.transform.GetComponent<Renderer>();

        for (byte i = 0; i < Btns.Length; ++i)//0 鱼类图鉴  1 关于我们 2 返回上一级 3 关闭窗口
        {
            Btns[i].m_BtnObj = BaseTranF.GetChild((i + 4)).gameObject;
            Btns[i].m_Btn = Btns[i].m_BtnObj.GetComponent<UIButton>();
            Btns[i].m_IsChecked = false;
            Btns[i].m_Tag = i;
            GetBtnLister(Btns[i].m_BtnObj).onClick = OnClickBtns;
        }
        SetBgValueOffset();
        SetValueOffset();
    }
    void SetBgValueOffset()
    {
        Vector2 offset = m_BgVBar.m_ValueBg.clipOffset;
        if (m_BgValue > 0.75f)
            offset.x = (m_BgVBar.m_SpWidth * (1 - m_BgValue)) * 1.1f;
        else
            offset.x = (m_BgVBar.m_SpWidth * (1 - m_BgValue));
        m_BgVBar.m_ValueBg.clipOffset = offset;
    }
    void SetValueOffset()
    {
        Vector2 offset = m_VBg.m_ValueBg.clipOffset;
        if (m_Value > 0.75f)
            offset.x = (m_VBg.m_SpWidth * (1 - m_Value)) * 1.1f;
        else
            offset.x = (m_VBg.m_SpWidth * (1 - m_Value));
        m_VBg.m_ValueBg.clipOffset = offset;
    }
    public void Update(float delattime)
    {
        if (m_BgValue != (1 - m_BgVBar.m_Bar.value) || m_BgVBar.m_Bar.value == 0 || m_BgVBar.m_Bar.value == 1)
        {
            m_BgValue = m_BgVBar.m_Bar.value;
            SetBgValueOffset();
            GlobalAudioMgr.Instance.SetBGMusicVolue(m_BgValue);
        }
        if (m_Value != (1 - m_VBg.m_Bar.value) || m_VBg.m_Bar.value == 0 || m_VBg.m_Bar.value == 1)
        {
            m_Value = m_VBg.m_Bar.value;
            SetValueOffset();
            GlobalAudioMgr.Instance.SetMusicVolue(m_Value);
        }
    }
    //读取SD卡的音效

    void OnClickBtns(GameObject go)
    {
        byte clickTag = 0;
        bool HaveClicked = false;
        for (byte i = 0; i < Btns.Length; ++i)
        {
            if (Btns[i].m_BtnObj == go)
            {
                Btns[i].m_IsChecked = true;
                clickTag = Btns[i].m_Tag;
                HaveClicked = true;
            }
            else
                Btns[i].m_IsChecked = false;
        }
        if (HaveClicked)//0 鱼类图鉴  1 返回上一级 2  关于我们3 关闭窗口
        {
            switch (clickTag)
            {
                case 0:
                    {
                        PlayBtnMusic();
                        GlobalHallUIMgr.Instance.CloseSettingWind();
                        GlobalHallUIMgr.Instance.ShowFishShow(false);

                        //if (HallRunTimeInfo.Instance_UI != null)
                        //{
                        //     HallRunTimeInfo.Login_UI.ShowBtnWindEffect(false);
                        //}
                    }
                    break;
                case 1:
                    {
                        PlayBtnMusic();
                        GlobalHallUIMgr.Instance.CloseSettingWind();
                        if (SceneRuntime.Instance != null)
                        {
                            if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() != 0)      //处于比赛中
                            {

                                GlobalHallUIMgr.Instance.ShowMatchMsgBox(StringTable.GetString("Match_Leave_Warning"),
                                    PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID(), MatchMsgBoxType.Match_Leave);
                            }
                            else
                                SceneRuntime.SceneLogic.BackToHall();
                        }
                        else if (HallRunTimeInfo.Instance_UI != null)
                            LogicManager.Instance.LogOff();
                    }
                    break;
                case 2:     //关于我们
                    GlobalHallUIMgr.Instance.CloseSettingWind();
                    GlobalHallUIMgr.Instance.ShowAboutOurWnd();
                    break;
                case 3:
                    {
                        GlobalAudioMgr.Instance.SaveAudioValue();
                        GlobalHallUIMgr.Instance.CloseSettingWind();
                    }
                    break;
                default:
                    break;
            }
        }
    }
    struct SettIngWindScrollBar
    {
        public UIPanel m_ValueBg;
        public UIScrollBar m_Bar;
        public float m_SpWidth;
        public Renderer render;
    }
}
class FishShow : GlobalBaseWind
{
    UIGrid[] m_Grid = new UIGrid[3];
    RenderTarget m_RT;
    UITexture m_RTTex;
    UILabel m_TopFishName;
    UILabel m_FishIndex;
    UILabel m_FishDecl;
    byte m_FishObjIndex = 0;
    byte m_WaitLoadPath = 0;
    bool isLottery;
    int m_LotteryFishIndex = -1;
    bool isShowTip = false;
    uint[] LotteryFishIDs;
    public Object Init()
    {
        // m_WindId = GlobalBtnTouchBoxPool.GlobalWindStatue.Global_FishShow;
        Object obj = Initobj("FishShowWind");
        return obj;
    }
    public void InitGameObjcet(bool isLottery)
    {
        Init_GameObj(false);
        this.isLottery = isLottery;
        GetLotteryFishID();
        if (isLottery)
        {
            GetBtnLister(BaseTranF.GetChild(1).gameObject).onClick = OnClickCloseBtnOnLottery;
            BaseTranF.GetChild(9).gameObject.SetActive(true);
            GetBtnLister(BaseTranF.GetChild(9).GetChild(0).gameObject).onClick = OnShowTip;
            BaseTranF.GetChild(9).GetChild(1).GetChild(0).GetComponent<UILabel>().text = StringTable.GetString("UOM_LOttery_GoldFishDes");
        }
        else
        {
            GetBtnLister(BaseTranF.GetChild(1).gameObject).onClick = OnClickCloseBtn;
        }
        m_RTTex = BaseTranF.GetChild(2).GetComponent<UITexture>();

        m_TopFishName = BaseTranF.GetChild(5).GetComponent<UILabel>();
        m_FishIndex = BaseTranF.GetChild(6).GetComponent<UILabel>();
        m_FishDecl = BaseTranF.GetChild(7).GetComponent<UILabel>();
        GetBtnLister(BaseTranF.GetChild(3).gameObject).onClick = OnNextFish;
        GetBtnLister(BaseTranF.GetChild(4).gameObject).onClick = OnPrefFish;
        if (m_RT == null)
            m_RT = new RenderTarget();

        m_FishObjIndex = 0;
        if (isLottery)
            GetLotteryFishIndex(false);
        LoadFishByIndex();
    }
    void OnShowTip(GameObject go)
    {
        isShowTip = !isShowTip;
        BaseTranF.GetChild(9).GetChild(1).gameObject.SetActive(isShowTip);
    }
    void GetLotteryFishID()
    {
        byte i = 0;
        LotteryFishIDs = new uint[FishConfig.Instance.m_LotteryConfig.FishScore.Count];
        foreach (KeyValuePair<byte, uint> map in FishConfig.Instance.m_LotteryConfig.FishScore)
        {
            LotteryFishIDs[i] = map.Key;
            i++;
        }
    }
    void GetLotteryFishIndex(bool count)
    {
        if (count)
        {
            --m_LotteryFishIndex;
            if (m_LotteryFishIndex == -1)
            {
                m_LotteryFishIndex = FishConfig.Instance.m_LotteryConfig.FishScore.Count - 1;
            }
        }
        else
        {
            ++m_LotteryFishIndex;
            if (m_LotteryFishIndex == FishConfig.Instance.m_LotteryConfig.FishScore.Count)
            {
                m_LotteryFishIndex = 0;
            }
        }
        for (byte i = 0; i < FishSetting.SortFishList.Count; i++)
        {
            if (LotteryFishIDs[m_LotteryFishIndex] == FishSetting.SortFishList[i].FishID)
            {
                m_FishObjIndex = i;
                break;
            }
        }
    }
    void OnNextFish(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (isLottery)
        {
            GetLotteryFishIndex(false);
        }
        else
        {
            ++m_FishObjIndex;
            if (m_FishObjIndex >= FishSetting.SortFishList.Count)
            {
                m_FishObjIndex = 0;
            }
        }
        LoadFishByIndex();
    }
    void OnPrefFish(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);

        if (isLottery)
        {
            GetLotteryFishIndex(true);
        }
        else
        {
            --m_FishObjIndex;
            if (m_FishObjIndex == 255)
            {
                m_FishObjIndex = (byte)(FishSetting.SortFishList.Count - 1);
            }
        }
        LoadFishByIndex();
    }
    System.Collections.IEnumerator WaitLoadPath(Object obj)
    {
        while (PathManager.Instance.IsInitOK == false)
            yield return new WaitForEndOfFrame();
        m_WaitLoadPath = 0;
        GlobalHallUIMgr.Instance.ShutDownSystemTipsUI();
        LoadFishByIndex();
    }
    void LoadFishByIndex()
    {
        if (m_WaitLoadPath == 1)
            return;
        if (PathManager.Instance.IsInitOK == false)
        {
            GlobalHallUIMgr.Instance.ShowSystemTipsUI(StringTable.GetString("WaitLoadFishRes"), 60.0f, false);
            m_WaitLoadPath = 1;
            SceneMain.Instance.StartInnerCoroutine(WaitLoadPath(null));
            return;
        }
        byte idx = FishSetting.SortFishList[m_FishObjIndex].FishID;
        if (idx == 255)
            return;

        m_RT.RenderSingleFish(idx);
        m_RTTex.mainTexture = m_RT.SingleRenderTex;
        m_TopFishName.text = FishSetting.FishDataList[idx].Name;
        m_FishDecl.text = FishSetting.FishDataList[idx].Decl;
        m_FishIndex.text = (m_FishObjIndex + 1).ToString() + "/" + FishSetting.SortFishList.Count;
        BaseTranF.GetChild(10).GetChild(1).GetComponent<UILabel>().text = FishSetting.FishDataList[idx].Gold.ToString();
        if (isLottery)
        {
            m_FishIndex.text = (m_LotteryFishIndex + 1).ToString() + "/" + FishConfig.Instance.m_LotteryConfig.FishScore.Count;
        }
    }
    void OnClickCloseBtn(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);

        m_RTTex.mainTexture = null;
        m_RT.Shutdown();
        m_RT = null;
        GlobalHallUIMgr.Instance.CloseFishShow();
        GlobalHallUIMgr.Instance.ShowSettingWind();
    }

    void OnClickCloseBtnOnLottery(GameObject go)
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_CloseUI);
        GlobalHallUIMgr.Instance.ShowMask(false);
        m_RTTex.mainTexture = null;
        m_RT.Shutdown();
        m_RT = null;
        GlobalHallUIMgr.Instance.CloseFishShow();
        // WndManager.Instance.Pop(m_SettingWind.WindObj);        
    }
}

public class AboutOur : GlobalBaseWind
{
    public Object Init()
    {
        // m_WindId = GlobalBtnTouchBoxPool.GlobalWindStatue.GLobal_Setting;
        Object obj = Initobj("AboutOurWnd");
        return obj;
    }
    public void InitGameObjcet()
    {
        Init_GameObj(false);
        UIEventListener.Get(BaseTranF.GetChild(0).gameObject).onClick = OnCloseWnd;
    }
    void OnCloseWnd(GameObject go)
    {
        GlobalHallUIMgr.Instance.CloseAboutOurWnd();
    }
}