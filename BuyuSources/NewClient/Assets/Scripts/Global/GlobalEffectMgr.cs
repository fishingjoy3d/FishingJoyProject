using UnityEngine;
using System.Collections.Generic;
public interface ILifeTimer
{
    float LifeTime
    {
        get;
    }
    bool IsEnd
    {
        get;
    }
}
public interface IGlobalEffect
{
    Vector3 Position
    {
        get;
    }
}
public class GlobalEffectPosConverter
{
    public static Vector3 LightingPosConvert(Vector3 pos)
    {
        pos = Camera.main.WorldToScreenPoint(pos);
        pos.z = 110;
        pos = Camera.main.ScreenToWorldPoint(pos);
        return pos;
    }
    public static Vector3 FreezePosConvert(Vector3 pos)
    {
        pos.z -= 20;
        return pos;
    }
    public static Vector3 LaserPosConvert(Vector3 pos)
    {
        pos.z -= 20;
        return pos;
    }
}
public class GlobalEffectData : ILifeTimer
{
    public delegate Vector3 PosConverter(Vector3 pos);

    public GlobalEffectData(GameObject effect, float delay = 0.0f, float life = float.MaxValue, float scaling = 1.0f)
    {
        EffectHost  = null;
        EffectObj   = effect;
        Delay       = delay;
        Life        = life;
        Offset      = Vector3.zero;
        Active      = delay <= 0;
        effect.SetActive(Active);
        if (Active)
            SetScaling(scaling);
        else
            Scaling = scaling;
    }
    public GlobalEffectData(IGlobalEffect host, GameObject effect, float delay = 0.0f, float life = float.MaxValue, PosConverter converter = null, float scaling = 1.0f)
    {
        EffectHost = host;
        EffectObj = effect;
        Offset = Vector3.zero;
        Delay = delay;
        Life = life;
        Active = delay <= 0;
        effect.SetActive(Active);
        PosConvert = converter;
        if (Active)
            SetScaling(scaling);
        else
            Scaling = scaling;
    }
    public GlobalEffectData(IGlobalEffect host, GameObject effect, Vector3 offset, float delay = 0.0f, float life = float.MaxValue, PosConverter converter = null, float scaling = 1.0f)
    {
        EffectHost  = host;
        EffectObj   = effect;
        Offset      = offset;
        Delay       = delay;
        Life        = life;
        Active      = delay <= 0;
        effect.SetActive(Active);
        PosConvert = converter;
        if (Active)
            SetScaling(scaling);
        else
            Scaling = scaling;
    }
    public void SetScaling(float scl)
    {
        ParticleSystem[] ps = EffectObj.GetComponentsInChildren<ParticleSystem>();
        for (int i = 0; i < ps.Length; ++i)
        {
            ps[i].startSize *= scl;
        }
    }
    public float LifeTime
    {
        get
        {
            return Time;
        }
    }
    public bool IsEnd
    {
        get
        {
            return Time >= Life + Delay;
        }
    }
    public float        Scaling;
    public PosConverter  PosConvert;
    public IGlobalEffect EffectHost;    //此特效关联的对象。
    public GameObject   EffectObj;      //特效
    public Vector3      Offset;
    public float        Delay;          //延迟多少秒开始播放
    public float        Life;           //持续播放时间
    public float        Time;
    public bool         Active;
}

public class GlobalEffectMgr:Singleton<GlobalEffectMgr>
{
    List<GlobalEffectData>      m_EffectList = new List<GlobalEffectData>();
    HallMessage                 m_Message = new HallMessage();
    Shader                      m_UIGrayShader;
    Object[]                    m_UIGoldEffect = new Object[3];      //金币、钻石、奖牌扫光特效
    public bool Init()
    {
        for (byte i = 0; i < m_UIGoldEffect.Length; ++i)
        {
            if (i == 0)
                m_UIGoldEffect[i] = ResManager.Instance.LoadObject("UIEfGlodLight", "ExtraRes/Effect/Prefabs/UIGoldEffect/", ResType.ExtraRes, false);
            else if (i == 1)
                m_UIGoldEffect[i] = ResManager.Instance.LoadObject("UIEfDiamond", "ExtraRes/Effect/Prefabs/UIGoldEffect/", ResType.ExtraRes, false);
            else
                m_UIGoldEffect[i] = ResManager.Instance.LoadObject("UIEfMedal", "ExtraRes/Effect/Prefabs/UIGoldEffect/", ResType.ExtraRes, false);
        }
        m_UIGrayShader = Shader.Find("FishShader/UIGray");
        return true;
    }
    public void GlobalInit()
    {
        m_Message.Init();
    }
    public Shader UIGrayShader
    {
        get
        {
            return m_UIGrayShader;
        }
    }
    public void AddEffect(GlobalEffectData effect)
    {
        m_EffectList.Add(effect);
    }
    public void Clear()
    {
        for(int i = 0; i < m_EffectList.Count; ++i)
        {
            GameObject.Destroy(m_EffectList[i].EffectObj);
        }
        m_EffectList.Clear();
    }
    public static void SetEffectOnUI(GameObject effect)
    {
        Renderer renderer = effect.GetComponent<Renderer>();
        if (renderer != null && renderer.sharedMaterial != null)
            renderer.sharedMaterial.renderQueue = 30000;
    }
    public static void SetMeshRendererOnTopWnd(GameObject effect)
    {
        MeshRenderer[] tr = effect.GetComponentsInChildren<MeshRenderer>();
        if (tr != null)
        {
            foreach (MeshRenderer rr in tr)
                rr.sortingLayerName = "ParsLayer";
        }
    }
    public static void SetTrailEffectOnTopWnd(GameObject effect)
    {
        TrailRenderer[] tr = effect.GetComponentsInChildren<TrailRenderer>();
        if (tr != null)
        {
            foreach(TrailRenderer rr in tr)
                rr.sortingLayerName = "ParsLayer";
        }
    }
    void UpdateEffectPos(GlobalEffectData effect)
    {
        if (effect.PosConvert != null)
            effect.EffectObj.transform.position = effect.PosConvert(effect.EffectHost.Position) + effect.Offset;
        else
            effect.EffectObj.transform.position = effect.EffectHost.Position + effect.Offset;
    }
    public void Update(float delta)
    {
        m_Message.Update(delta);
        for(int i = 0; i < m_EffectList.Count; )
        {
            GlobalEffectData effect = m_EffectList[i];
            effect.Time += delta;
            if(!effect.Active)
            {
                if(effect.Time >= effect.Delay)
                {
                    if (effect.EffectHost != null)
                        UpdateEffectPos(effect);
                    effect.Active = true;
                    effect.EffectObj.SetActive(true);
                    effect.SetScaling(effect.Scaling);
                }
                else
                {
                    ++i;
                    continue;
                }
            }
            if(effect.IsEnd)
            {
                GameObject.Destroy(effect.EffectObj);
                Utility.ListRemoveAt(m_EffectList, i);
                continue;
            }
            if (effect.EffectHost != null)
            {
                UpdateEffectPos(effect);
            }
            ++i;
        }  
    }
    public void ShowLoadingMessage()
    {
        m_Message.ShowLoadingMessage();
        //if (!m_Message.ShowMessage(message))
        //{
        //    m_Message.Init();
        //    m_Message.ShowMessage(message);
        //}
    }
    public void ShowMessageBox(string strMsg, MssageType type, byte RoomID = 0, uint ExData = 0)
    {
        m_Message.ShowMessageBox(strMsg, type, RoomID, ExData);
    }
    public void CloseLoadingMessage()
    {
        m_Message.ShutDown(HallMessage.MessageType.loading);
    }
    public void ShutDownMsgBox()
    {
        m_Message.ShutDownMsgBox();
    }

    public Object[] UIGoldEffect
    {
        get { return m_UIGoldEffect; }
    }
}
