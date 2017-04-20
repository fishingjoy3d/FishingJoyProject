using UnityEngine;
using System.Collections.Generic;

enum ComboType
{
    Combo_0 = 0,    //连击
    Combo_1,        //必杀
}

struct ComboContent
{
    public GameObject   m_Object;
    public UISprite     m_Star;
    public UILabel[]    m_label;
    public UISprite     m_HitBg;
    public UISprite     m_Energy;
    public UISprite     m_ZiTi;     //连击提示字
    public TweenScale   m_Scale_Num;
    public TweenAlpha   m_Alpha_Num;
    public TweenScale   m_Scale_ZiTi;
    public TweenAlpha   m_Alpha_ZiTi;
    public UIPanel      m_Panle_Energy;
}

public class SceneComboEft : BaseWnd
{
    ComboContent[]      m_Combo = new ComboContent[2];
    Transform[]         m_ChildTrans = new Transform[2];
    Vector3             m_Pos;
    bool                m_IsMySelf;
    ushort              m_ComboCount;
    float               m_Speed;
    float               m_LifeTime;
    bool                m_bCRI;/*是否必杀*/
    ComboType           m_CobmoTyp;
    public void Init(Object obj)
    {
        m_BaseWndObject = GameObject.Instantiate(obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        Vector3 pos = m_Pos;
        
        for (byte i = 0; i < 2; ++i)
        {
            m_ChildTrans[i] = m_BaseTrans.GetChild(i);
            m_Combo[i].m_Object = m_BaseTrans.GetChild(i).gameObject;

            m_Combo[i].m_HitBg = m_ChildTrans[i].GetChild(1).GetComponent<UISprite>();
            m_Combo[i].m_ZiTi = m_ChildTrans[i].GetChild(3).GetComponent<UISprite>();
            m_Combo[i].m_Alpha_Num = m_ChildTrans[i].GetChild(0).GetComponent<TweenAlpha>();
            m_Combo[i].m_Alpha_ZiTi = m_ChildTrans[i].GetChild(3).GetComponent<TweenAlpha>();
            m_Combo[i].m_Scale_Num = m_ChildTrans[i].GetChild(0).GetComponent<TweenScale>();
            m_Combo[i].m_Scale_ZiTi = m_ChildTrans[i].GetChild(3).GetComponent<TweenScale>();
            
            if (m_IsMySelf)
            {
                m_Pos.x = -1.25f;
                m_Pos.y = 0.42f;
                m_ChildTrans[i].localScale = new Vector3(1f, 1f, 1f);
                m_Combo[i].m_Panle_Energy = m_ChildTrans[i].GetChild(1).GetChild(0).GetComponent<UIPanel>();
                m_Combo[i].m_Energy = m_ChildTrans[i].GetChild(1).GetChild(0).GetChild(0).GetComponent<UISprite>();
                Vector2 Panle_set = m_Combo[i].m_Panle_Energy.clipOffset;
                Panle_set.x = 0;
                m_Combo[i].m_Panle_Energy.clipOffset = Panle_set;
                m_Speed = m_Combo[i].m_Panle_Energy.width;
            }
            else
            {
                m_Pos = pos;
                if (m_Pos.y > 0)
                    m_Pos.y -= 0.235f;
                else
                    m_Pos.y += 0.235f;
                m_Pos.x -= 0.59f;
                m_ChildTrans[i].localScale = new Vector3(0.5f, 0.5f, 0.5f);
                if (m_Combo[i].m_HitBg.gameObject.activeSelf != false) 
                    m_Combo[i].m_HitBg.gameObject.SetActive(false);

            }
            m_ChildTrans[i].position = m_Pos;
        }
        m_Combo[0].m_Star = m_ChildTrans[0].GetChild(2).GetComponent<UISprite>();
        m_Combo[0].m_label = new UILabel[4];
        for (byte j = 0; j < 4; ++j)
        {
            m_Combo[0].m_label[j] = m_ChildTrans[0].GetChild(0).GetChild(j).GetComponent<UILabel>();
        }

        if (!m_IsMySelf)
            m_Combo[0].m_Star.gameObject.SetActive(false);
        else
        {
            GameObject go = GameObject.Instantiate(SceneRuntime.PlayerMgr.ComboEftObj) as GameObject;
            go.transform.SetParent(m_ChildTrans[1].GetChild(2), false);
            GlobalEffectMgr.SetEffectOnUI(go);
            if (go.transform.GetChild(0).gameObject.activeSelf != true)
                go.transform.GetChild(0).gameObject.SetActive(true);
        }

        //调整位置
        if (m_IsMySelf && SceneRuntime.LogicUI.UnLockIsShow())
            SetCobmoPos(false);
    }
    public void ShowCombo(ushort ComboCount, bool bCRI/*必杀*/)
    {
        if (m_BaseWndObject.activeSelf != true)
            m_BaseWndObject.SetActive(true);
        m_LifeTime = DoubleHit.IntervalTime;
        m_ComboCount = ComboCount;
        m_bCRI = bCRI;

        if (!bCRI)
        {
            if (m_Combo[1].m_Object.activeSelf != false)
            {
                m_Combo[1].m_Object.SetActive(false);
            }
            m_Combo[0].m_Object.SetActive(true);
            m_CobmoTyp = ComboType.Combo_0;
            ExchangeBg(ComboCount);
        }
        else
        {
            if (m_Combo[0].m_Object.activeSelf != false)
            {
                m_Combo[0].m_Object.SetActive(false);
            }
            m_Combo[1].m_Object.SetActive(true);

            m_CobmoTyp = ComboType.Combo_1;
            GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_KillBuff);
        }
        if (m_IsMySelf)
        {
            Vector2 Panle_set = m_Combo[(int)m_CobmoTyp].m_Panle_Energy.clipOffset;
            Panle_set.x = 0;
            m_Combo[(int)m_CobmoTyp].m_Panle_Energy.clipOffset = Panle_set;
        }
        PlayTween();
    }
    public void SetCobmoPos(bool  bOriginal)
    {
        if (m_BaseWndObject == null)
            return;
        Vector3 pos = m_Pos;
        if (bOriginal)
            pos = m_Pos;
        else
        {
            pos.x += 0.55f;  
        }
        for (byte i = 0; i < m_ChildTrans.Length; ++i)
            m_ChildTrans[i].position = pos;
    }
    public bool Updata(float dTime)
    {
        if (SceneRuntime.SceneLogic.bClearScene)
        {
            return true;
        }
        m_LifeTime -= dTime;
        if (m_LifeTime <= 0)
        {
            ShutActive();
            return false;
        }
        if (m_IsMySelf)
        {
            Vector2 Panle_set = m_Combo[(int)m_CobmoTyp].m_Panle_Energy.clipOffset;
            float distance = (m_Speed / DoubleHit.IntervalTime) * dTime;
            Panle_set.x -= distance;
            m_Combo[(int)m_CobmoTyp].m_Panle_Energy.clipOffset = Panle_set;
        }
       
        return true;
    }
    void ExchangeBg(ushort comboCount)
    {
        if (comboCount <= 10)
            comboCount = 1;
        else if (comboCount > 10 && comboCount <= 20)
            comboCount = 2;
        else
            comboCount = 3;

        for (byte i = 0; i < 4; ++i)
        {
            if (i == comboCount - 1)
            {
                if (m_Combo[0].m_label[i].gameObject.activeSelf != true)
                    m_Combo[0].m_label[i].gameObject.SetActive(true);
            }
            else
            {
                if (m_Combo[0].m_label[i].gameObject.activeSelf != false)
                    m_Combo[0].m_label[i].gameObject.SetActive(false);
            }
        }

        if (m_IsMySelf)
        {
            m_Combo[0].m_Star.spriteName = "DoubleHit_Star_" + comboCount.ToString();
            m_Combo[0].m_Energy.spriteName = "DoubleHit_energy_" + comboCount.ToString();
            m_Combo[0].m_HitBg.spriteName = "DoubleHit_bg_" + comboCount.ToString();
            m_Combo[0].m_ZiTi.spriteName = "DoubleHit_ZT_" + comboCount.ToString();
        }
        else
            m_Combo[0].m_ZiTi.spriteName = "DoubleHit_ZT_" + comboCount.ToString();

        m_Combo[0].m_label[comboCount - 1].text = m_ComboCount.ToString();
    }
 
    void PlayTween()
    {
        int Idx = m_bCRI ? 1 : 0;
        m_Combo[Idx].m_Scale_Num.ResetToBeginning();
        m_Combo[Idx].m_Scale_Num.PlayForward();
        m_Combo[Idx].m_Alpha_Num.ResetToBeginning();
        m_Combo[Idx].m_Alpha_Num.PlayForward();
        m_Combo[Idx].m_Scale_ZiTi.ResetToBeginning();
        m_Combo[Idx].m_Scale_ZiTi.PlayForward();
        m_Combo[Idx].m_Alpha_ZiTi.ResetToBeginning();
        m_Combo[Idx].m_Alpha_ZiTi.PlayForward();
    }
    public void MaskComboEffect(bool bMask)
    {
        if (m_BaseWndObject != null)
            m_BaseWndObject.SetActive(bMask); 
    }
    public void ShutActive()
    {
        if (m_BaseWndObject.activeSelf != false)
            m_BaseWndObject.SetActive(false);
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
            GameObject.Destroy(m_BaseWndObject);
    }

    public Vector3 Pos
    {
        get { return m_Pos; }
        set { m_Pos = value; }
    }
    public bool IsMySelf
    {
        get { return m_IsMySelf; }
        set { m_IsMySelf = value; }
    }
    public float LifeTime
    {
        get { return m_LifeTime; }
        set { m_LifeTime = value; }
    }
    public ushort ComboCount
    {
        get { return m_ComboCount; }
        set { m_ComboCount = value; }
    }
    public bool bCRI
    {
        get { return m_bCRI; }
        set { m_bCRI = value; }
    }
}
