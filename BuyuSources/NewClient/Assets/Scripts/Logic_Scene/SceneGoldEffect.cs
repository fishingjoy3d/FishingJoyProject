using UnityEngine;
using System.Collections.Generic;

public class MatchJudge
{
   public static bool IsMatch()
    {
        return PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() != 0; 
    }
}
//金币结束一刻放大效果
public class GoldEndLargenData
{
    public GameObject   m_GameObj;
    public Transform    m_TrnasGold;
    public float        m_LifeTime;

    public void Init()
    {
        m_TrnasGold = m_GameObj.transform;
        m_LifeTime = 0.1f;
    }
    public bool Update(float delatime)
    {
        m_LifeTime -= delatime;
        if (m_LifeTime <= 0)
            return false;
        else
        {
            GoldTrnasScale(delatime);
            return true;
        }
    }
    void GoldTrnasScale(float delatime)
    {
        Vector3 TempScale = m_TrnasGold.localScale;
        TempScale += new Vector3(4.0f * delatime, 4.0f * delatime, 4.0f * delatime);
        m_TrnasGold.localScale = TempScale;
    }
    public void DestorySelf()
    {
        if (m_GameObj != null)
        {
            GameObject.Destroy(m_GameObj);
            m_GameObj = null;
        }
    }
}

//金币数字Label
public class GoldEffectLabelData
{
    public GameObject   GameObj;
    public Transform    m_BaseTrans;
    TweenPosition       m_TweenPosAnim;
    float               m_LifeTime = 1.01f;
    UILabel             GoldLLabel;
    bool                m_bScaleAnim = true;

    public bool bScaleAnim
    {
        get { return m_bScaleAnim; }
        set { m_bScaleAnim = value; }
    }
    public float LifeTime
    {
        get { return m_LifeTime; }
        set { m_LifeTime = value; }
    }
    public void Init()
    {
        m_BaseTrans = GameObj.transform;
        m_BaseTrans.localScale = Vector3.one;
        GoldLLabel = GameObj.transform.GetChild(0).GetComponent<UILabel>();
        if (!bScaleAnim)
            m_TweenPosAnim = m_BaseTrans.GetComponent<TweenPosition>();
    }
    public void ResetScale()
    {
        m_BaseTrans.localScale = new Vector3(0.7f, 0.7f, 0.7f);
    }
    public bool Update(float dTime)
    {
        LifeTime -= dTime;
        if (LifeTime <= 0)
            return false;
        else
        {
            if (m_bScaleAnim)
                TransFScale(dTime);
           return true;
        }
    }
    void TransFScale(float delatime)
    {
        if (LifeTime > 0.97f)
        {
            if (m_BaseTrans.localScale != Vector3.one)
                m_BaseTrans.localScale = Vector3.one;
        }
        else if (LifeTime < 0.97f && LifeTime > 0.85f)
        {
            Vector3 TempScale = m_BaseTrans.localScale;
            if (TempScale.x < 1.6f)
            {
                TempScale += new Vector3(4.5f * delatime, 4.5f * delatime, 4.5f * delatime);
                m_BaseTrans.localScale = TempScale;
            }
        }
        else
        {
            Vector3 TempScale = m_BaseTrans.localScale;
            if (TempScale.x > 1.0f)
            {
                TempScale -= new Vector3(4.5f * delatime, 4.5f * delatime, 4.5f * delatime);
                m_BaseTrans.localScale = TempScale;
            }
        }
       
    }
    public void SetText(string str)
    {
        GoldLLabel.text ="+"+ str;
    }
    public void SetLabelFontSize()
    {
        GoldLLabel.fontSize = 10;
    }
    public void SetPos(Vector3 pos)
    {
        pos.y += 0.1f;
        m_BaseTrans.position = pos;
    }
    public void SetTweenPos()
    {
        m_TweenPosAnim.from = m_BaseTrans.localPosition;
        Vector3 ToPos = m_BaseTrans.localPosition;
        ToPos.y += 40f;
        m_TweenPosAnim.to = ToPos;
    }
    public void DestorySelf()
    {
        if (GameObj != null)
        {
            GameObject.Destroy(GameObj);
            GameObj = null;
        }
    }
  
}
public class GoldEffectData
{   
    public GameObject       GameObj;
    public SimplePath       Path;
    public Vector3          m_vecpathend;
    public CatchedData      catchedData;
    public SceneGoldEffect.GoldTween m_Tween;
    public float            m_DelayTime = 1.0f;
    public bool             HaveChangeRoad = false;
    public uint GoldNum;
    public byte ClientSeat;
    public bool Update(float dTime)
    {
        m_DelayTime -= dTime;
        if (m_DelayTime > 0)
        {
            return true;
        }
        if (!HaveChangeRoad)
        {
            HaveChangeRoad = true;
            InitCRPath();
            PlayTween(false);           
        }
        GameObj.transform.position = Path.Update(dTime);
        return !Path.Finished;
        
    }
    public void PlayTween(bool bPlay)
    {
        if (bPlay)
        {
            if (!m_Tween.m_Pos.enabled) 
                m_Tween.m_Pos.enabled = true;
            if (!m_Tween.m_Sclae.enabled) 
                m_Tween.m_Sclae.enabled = true;
            m_Tween.m_Pos.ResetToBeginning();
            m_Tween.m_Sclae.ResetToBeginning();
            m_Tween.m_Pos.PlayForward();
            m_Tween.m_Sclae.PlayForward();
        }
        else
        {
            m_Tween.m_Pos.enabled = false;
            m_Tween.m_Sclae.enabled = false;
        }
    }
    public void InitCRPath()
    {

        Path = new SimplePath(GameObj.transform.position, m_vecpathend);
        Path.Speed = Utility.Range(2.0f, 3.0f);     

        //Vector3 PlayerPos = SceneRuntime.GetLauncherGoldIconPos(catchedData.ClientSeat);
        //Vector3 startControl = (((GameObj.transform.position + PlayerPos) * 0.5f) + GameObj.transform.position)*0.5f;
        //Vector3 endControl = (((GameObj.transform.position + PlayerPos) * 0.5f) + PlayerPos)*0.5f;
        //Path = new CRPath(GameObj.transform.position, startControl, PlayerPos, endControl);
        //Path.Speed = GetPathSpeed();
    }
    public void ScaleGoldTR(float fScalefrom,float fScaleto)
    {
        //uint GoldNum = this.catchedData.CatchType;
        this.GameObj.transform.localScale = new Vector3(fScalefrom, fScalefrom, fScalefrom);
        Vector3 Pos_ = GameObj.transform.localPosition;
        Pos_.z = 0;
        m_Tween.m_Pos.to = Pos_; 
        Pos_.y += 100;
        m_Tween.m_Pos.from = Pos_;
        m_Tween.m_Sclae.from = new Vector3(fScalefrom, fScalefrom, fScalefrom);// Vector3.one; ;
        m_Tween.m_Sclae.to = new Vector3(fScaleto, fScaleto, fScaleto);// new Vector3(fScale, fScale, fScale);
    }
    public void DsetorySelf()
    {
        if(GameObj!=null)
            GameObject.Destroy(GameObj);
    }
}
public class SceneGoldEffect
{
    List<GoldEffectData>                m_CatchedList = new List<GoldEffectData>();
    List<GoldEffectData>                m_UnlockRateList = new List<GoldEffectData>();//解锁奖励金币
    List<GoldEffectData>                m_diamondList = new List<GoldEffectData>();//钻石掉落
    List<GoldEffectLabelData>           m_CatchedLabelList = new List<GoldEffectLabelData>();
    List<GoldEffectLabelData>           m_GetGoldList = new List<GoldEffectLabelData>();
    GoldEndLargenData                   m_EndLargenGold = new GoldEndLargenData();
    Object                              m_GoldObj = null;
    Object                              m_ScoreObj = null;
    Object                               m_DiamondObj = null;
    Object                              m_LabelObj = null;
    Object                              m_LargenGoldObj = null;
    Object                              m_GetGoldLabelObj = null;
    Object                              m_BossFishSpecailEftObj = null;
    public void Init()
    {
        m_GoldObj = ResManager.Instance.LoadObject("SceneGoldEffect", "SceneRes/Prefab/UI/Gold/", ResType.SceneRes);
        m_ScoreObj = ResManager.Instance.LoadObject("SceneStarEffect", "SceneRes/Prefab/UI/Gold/", ResType.SceneRes);
        m_DiamondObj = ResManager.Instance.LoadObject("SceneDiamondEffect", "SceneRes/Prefab/UI/Gold/", ResType.SceneRes);

        m_LabelObj = ResManager.Instance.LoadObject("SceneGoldLabelEffect", "SceneRes/Prefab/UI/Gold/", ResType.SceneRes);
        m_LargenGoldObj = ResManager.Instance.LoadObject("SceneGoldLargen", "SceneRes/Prefab/UI/Gold/", ResType.SceneRes);
        m_GetGoldLabelObj = ResManager.Instance.LoadObject("SceneGetGoldLabel", "SceneRes/Prefab/UI/Gold/", ResType.SceneRes);
        m_BossFishSpecailEftObj = ResManager.Instance.LoadObject("Ef_Boss", "SceneRes/Prefab/Effect/Boss/", ResType.SceneRes); 
    }
    public void Clear()
    {
        for (byte i = 0; i < m_CatchedList.Count; ++i)
            m_CatchedList[i].DsetorySelf();
        for (byte i = 0; i < m_CatchedLabelList.Count; ++i)
            m_CatchedLabelList[i].DestorySelf();
        for (byte i = 0; i < m_GetGoldList.Count; ++i)
            m_GetGoldList[i].DestorySelf();
        m_CatchedList.Clear();
        m_CatchedLabelList.Clear();
        m_EndLargenGold.DestorySelf();
        m_GetGoldList.Clear();
    }
    public void ShowBossFishSpecailEft(CatchedData cd, Fish fish)
    {
        GameObject go = GameObject.Instantiate(m_BossFishSpecailEftObj) as GameObject;
        go.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        go.transform.position = SceneRuntime.WorldToNGUI(fish.Position);
        GlobalEffectData bossFishEft = new GlobalEffectData(go, 0, 5);
        GlobalEffectMgr.Instance.AddEffect(bossFishEft);
       // GlobalEffectMgr.SetEffectOnUI(go);

        SceneObjMgr.Instance.PlayBack(BackAnimType.BACK_ANIM_BOSS);

    }
    public void ShowDiamond(CatchedData cd, Fish fish)
    {
        GoldEffectData ged = new GoldEffectData();
        ged.GameObj = Initobj(m_DiamondObj);

        ged.GameObj.transform.position = SceneRuntime.WorldToNGUI(fish.Position) + (new Vector3(Utility.RandFloat(), Utility.RandFloat(), 0) * 0.45f);
        ged.m_Tween.m_Pos = ged.GameObj.transform.GetComponent<TweenPosition>();
        ged.m_Tween.m_Sclae = ged.GameObj.transform.GetComponent<TweenScale>();
        ged.ScaleGoldTR(1.0f,0.6f);
        ged.m_DelayTime = 2;
        ged.GoldNum = 1;
        ged.PlayTween(true);
        ged.m_vecpathend = SceneRuntime.GetLauncherGoldIconPos(cd.ClientSeat);        
        m_diamondList.Add(ged);
    }
    //单个金币
    public void ShowUnLockRateReward(Vector3 pos)
    {
        for (int i = 0; i < 3; i++)
        {
            GoldEffectData ged = new GoldEffectData();           
            ged.GameObj = Initobj(m_GoldObj);
            ged.GameObj.transform.position = pos+((new Vector3(Utility.RandFloat(), Utility.RandFloat(), 0)) * 0.15f);
            ged.m_Tween.m_Pos = ged.GameObj.transform.GetComponent<TweenPosition>();
            ged.m_Tween.m_Sclae = ged.GameObj.transform.GetComponent<TweenScale>();
            ged.ScaleGoldTR(0.6f,0.6f);
            ged.m_DelayTime = 0;
            ged.GoldNum = 1;
            ged.PlayTween(true);
            ged.m_vecpathend = SceneRuntime.GetLauncherGoldIconPos(SceneRuntime.SceneLogic.PlayerMgr.MySelf.ClientSeat);
            m_UnlockRateList.Add(ged);
        }
      //  if (SceneRuntime.PlayerMgr.GetPlayer(ged.catchedData.ClientSeat) == SceneRuntime.PlayerMgr.MySelf)
         //   GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_GoldJump);
        
    }
    public void ShowGoldEffect(CatchedData cd, Fish fish)
    {
        if(MatchJudge.IsMatch()&&cd.ClientSeat!=SceneRuntime.MyClientSeat)//比赛只显示自己的效果
        {
            return;
        }
        Vector3 vecGoldEndpos = Vector3.one;
        if (MatchJudge.IsMatch())
        {
            if (cd.ClientSeat == SceneRuntime.MyClientSeat)
            {
                vecGoldEndpos = GlobalHallUIMgr.Instance.MatchScorePos();
            }
            else
            {
                vecGoldEndpos = SceneRuntime.GetLauncherGoldIconPos(cd.ClientSeat);
            }                               
        }
        else//比赛
        {
            vecGoldEndpos = SceneRuntime.GetLauncherGoldIconPos(cd.ClientSeat);
        }

        const int perGoldNum = 30;
        //这里的金币用的还是鱼的类型
        Vector3 FishPos = SceneRuntime.WorldToNGUI(fish.Position);        
        //鱼的价值
        uint fishOrgGold = (uint)FishSetting.FishDataList[fish.FishType].Gold;
        uint fishGold = 0;
        if (cd.CatchType == (byte)CatchedType.CATCHED_SKILL)
            fishGold = fishOrgGold * SkillSetting.SkillDataList[cd.SubType].multiple;
        else
            fishGold = fishOrgGold * BulletSetting.BulletRate[cd.RateIndex];

        
        if(!MatchJudge.IsMatch()&&cd.ClientSeat==SceneRuntime.MyClientSeat)
        {
            GlobalHallUIMgr.Instance.GameShare.AddGlod((int)fishGold);
        }
        

        uint goldNum = fishOrgGold / perGoldNum;
        uint perNum = 0;
        uint lastNum = 0;
        if(goldNum == 0)
        {
            goldNum = 1;
            lastNum = fishGold;
        }
        else
        {
            perNum = fishGold / goldNum;
            lastNum = perNum + (fishGold - perNum * goldNum);
        }
        uint num = 0;
        for (byte i = 0; i < goldNum; ++i)
        {            
            GoldEffectData ged = new GoldEffectData();
            ged.catchedData = cd;        

            if (MatchJudge.IsMatch())
            {
                ged.GameObj = Initobj(m_ScoreObj);               
            }
            else
            {
                ged.GameObj = Initobj(m_GoldObj);                                
            }
  
         
            ged.GameObj.transform.position = FishPos + (new Vector3(Utility.RandFloat(), Utility.RandFloat(), 0)) * (fish.IsBossFish()?0.45f:0.15f);
            ged.m_Tween.m_Pos = ged.GameObj.transform.GetComponent<TweenPosition>();
            ged.m_Tween.m_Sclae = ged.GameObj.transform.GetComponent<TweenScale>();
            ged.ScaleGoldTR(1.0f,fish.IsBossFish()?1.0f:0.6f);
            if (i > 0 && !fish.IsBossFish())
                ged.m_DelayTime += i * 0.1f;
            if (fish.IsBossFish())
            {
                ged.m_DelayTime = 1.5f;
            }
         
            if (i == goldNum - 1)
                ged.GoldNum = lastNum;
            else
                ged.GoldNum = perNum;
            num += ged.GoldNum;
            ged.ClientSeat = cd.ClientSeat;
            ged.PlayTween(true);
            ged.m_vecpathend = vecGoldEndpos;
            if (SceneRuntime.PlayerMgr.GetPlayer(ged.catchedData.ClientSeat) == SceneRuntime.PlayerMgr.MySelf)
                GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_GoldJump);
            m_CatchedList.Add(ged);
        }
        ShowGoldNumLabel(fishGold, FishPos, fish);
                       
    }
    public void ShowGetGoldNum(int  GoldNum, byte ClientSeat)
    {
        //显示一炮获得的总的金币数
        //剔除掉时间间隔短的
        if (m_GetGoldList.Count > 0)
        {
            GoldEffectLabelData gold = m_GetGoldList[m_GetGoldList.Count - 1];
            if (gold.LifeTime > 0.48f)
                return;
        }

        Vector3 pos = SceneRuntime.PlayerMgr.GetPlayer(ClientSeat).Launcher.LauncherPos;
        pos.x += 0.4f;
        pos.y += 0.125f;
        ShowGetGold(GoldNum, pos);
    }
    void UpdateRoleGold(GoldEffectData ged)
    {
        if(ged != null)
        {
            SceneRuntime.PlayerMgr.UpdatePlayerGold(ged.ClientSeat, (int)ged.GoldNum);
            if (PlayerRole.Instance.RoleInfo.RoleMe.GetMonthID() != 0)
            {
                GlobalHallUIMgr.Instance.UpdateMyMatchScoreInfo();
            }
        }
        //if (ged.ClientSeat == SceneRuntime.PlayerMgr.MyClientSeat)
        //{
        //    uint gold = PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() + ged.GoldNum;
        //    PlayerRole.Instance.RoleInfo.RoleMe.SetGlobel(gold);
        //}
        //else
        //{
        //    ScenePlayer sp = SceneRuntime.PlayerMgr.GetPlayer(ged.ClientSeat);
        //    uint gold = PlayerRole.Instance.TableManager.GetTableRole(sp.Player.playerData.ID).GetGlobel() + ged.GoldNum;
        //    PlayerRole.Instance.TableManager.GetTableRole(sp.Player.playerData.ID).SetGlobel(gold);
        //}
    }
    void ShowGoldNumLabel(uint _Score, Vector3 Pos, Fish fish, bool bScale = true)
    {
        GoldEffectLabelData label = new GoldEffectLabelData();
        label.GameObj = Initobj(m_LabelObj);
        label.bScaleAnim = bScale;
        label.Init();
        if (!bScale)
        {
            label.ResetScale();
            label.LifeTime = 2.0f;
        }
        label.SetPos(Pos);
        label.SetText(_Score.ToString());
        if (fish.IsBossFish())
        {
            label.LifeTime = 3.0f;
            label.SetLabelFontSize();
        }
        m_CatchedLabelList.Add(label);
    }
   void ShowGetGold(int gold, Vector3 pos)
    {
        GoldEffectLabelData getGold = new GoldEffectLabelData();
        getGold.GameObj = Initobj(m_GetGoldLabelObj);
        getGold.bScaleAnim = false;
        getGold.Init();
      //  getGold.ResetScale();
        getGold.LifeTime = 0.5f;
        getGold.SetPos(pos);
        getGold.SetTweenPos();
        getGold.SetText(gold.ToString());
        m_GetGoldList.Add(getGold);
    }
    void ShowGoldEndLargen(byte clientSeat)
    {   
        if(MatchJudge.IsMatch())
        {
            return;
        }
        Vector3 pos = SceneRuntime.GetLauncherGoldIconPos(clientSeat);
        if (m_EndLargenGold.m_GameObj == null && m_EndLargenGold.m_LifeTime <= 0.03f) 
        {
            m_EndLargenGold.m_GameObj = Initobj(m_LargenGoldObj);
            m_EndLargenGold.Init();
            m_EndLargenGold.m_TrnasGold.position = pos;
        }
    }
    public void Update(float delta)
    {
        for (byte i = 0; i < m_CatchedList.Count; )
        {
            GoldEffectData ged = m_CatchedList[i];
            if (ged.Update(delta) == false)
            {                                
                    if (++ged.catchedData.GoldFinished == ged.catchedData.FishList.Count)
                    {

                    }// ShowGoldNumLabel(ged);
                    ShowGoldEndLargen(ged.catchedData.ClientSeat);      //金币结束动画后放大效果
                    UpdateRoleGold(ged);
                    if (SceneRuntime.PlayerMgr.GetPlayer(ged.catchedData.ClientSeat) == SceneRuntime.PlayerMgr.MySelf)
                        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_UserGetGold);
                
                ged.DsetorySelf();
                Utility.ListRemoveAt(m_CatchedList, i);
            }
            else
            {
                ++i;
            }
               
        }

        for (int i = 0; i < m_UnlockRateList.Count; )
        {
            GoldEffectData ged = m_UnlockRateList[i];
             if (ged.Update(delta) == false)
             {
                 ged.DsetorySelf();
                 Utility.ListRemoveAt(m_UnlockRateList, i);
                 GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_Niuniu17);
             }
             else
             {
                 i++;
             }
        }
        for (int i = 0; i < m_diamondList.Count; )
        {
            GoldEffectData ged = m_diamondList[i];
            if (ged.Update(delta) == false)
            {
                ged.DsetorySelf();
                Utility.ListRemoveAt(m_diamondList, i);
            }
            else
            {
                i++;
            }
        }

        for (byte i = 0; i < m_CatchedLabelList.Count; )
        {
            GoldEffectLabelData data = m_CatchedLabelList[i];
            if (data.Update(delta) == false)
            {
                data.DestorySelf();
                Utility.ListRemoveAt(m_CatchedLabelList, i);
            }
            else
                ++i;
        }
        for (byte j = 0; j < m_GetGoldList.Count; )
        {
            GoldEffectLabelData getGold = m_GetGoldList[j];
            if (getGold.Update(delta) == false)
            {
                getGold.DestorySelf();
                Utility.ListRemoveAt(m_GetGoldList, j);
            }
            else
                ++j;
        }
        if (m_EndLargenGold.m_GameObj != null)
        {
            if (m_EndLargenGold.Update(delta) == false)
            {
                m_EndLargenGold.DestorySelf();
            }
        }
    }
    public void Shutdown()
    {

        for (byte i = 0; i < m_CatchedList.Count;++i )
        {
            m_CatchedList[i].DsetorySelf();
        }
            
        for (byte i = 0; i < m_UnlockRateList.Count; ++i)
        {
            m_UnlockRateList[i].DsetorySelf();
        }

        for (byte i = 0; i < m_diamondList.Count; ++i)
        {
            m_diamondList[i].DsetorySelf();
        }

        for (byte i = 0; i < m_CatchedLabelList.Count; ++i)
        {
            m_CatchedLabelList[i].DestorySelf();
        }
            
        for (byte j = 0; j < m_GetGoldList.Count; ++j )
        {
            m_GetGoldList[j].DestorySelf();
        }
        m_CatchedList.Clear();
        m_UnlockRateList.Clear();
        m_diamondList.Clear();
        m_CatchedLabelList.Clear();
        m_EndLargenGold.DestorySelf();
        ResManager.Instance.UnloadObject(m_GoldObj);
        ResManager.Instance.UnloadObject(m_ScoreObj);
        ResManager.Instance.UnloadObject(m_LabelObj);
        ResManager.Instance.UnloadObject(m_LargenGoldObj);
        ResManager.Instance.UnloadObject(m_GetGoldLabelObj);
    }

    GameObject Initobj( Object obj)
    {
        return SetParent(((GameObject)GameObject.Instantiate(obj)).transform).gameObject;
    }
    Transform SetParent(Transform objtr)
    {
        objtr.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (!objtr.gameObject.activeSelf) objtr.gameObject.SetActive(true);
        return objtr;
    }
    public struct GoldTween
    {
        public TweenPosition m_Pos;
        public TweenScale m_Sclae;
    }
    public bool HaveFlyGold(byte byClientSeat)
    {
        foreach(GoldEffectData item in m_CatchedList)
        {
            if (item.ClientSeat == byClientSeat)
            {
                return true;
            }            
        }
        return false;
    }
}
