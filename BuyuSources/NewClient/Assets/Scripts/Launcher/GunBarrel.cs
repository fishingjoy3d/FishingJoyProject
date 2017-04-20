using UnityEngine;
using System.Collections.Generic;


public struct GunBarrelDate
{
    public bool PlaySpriteAnim;
    public bool LightDot;
    public bool MoveLight;
    public bool MuzzleEft;
    public int StandbyType;
    public int MoveParts;
    public int ComponentCount;
}

public struct MuzzleEffectAnim
{
    public float DelayTime;
    public float DelayInteval;
    public bool bMoveEffect;
}

public class GunBarrel
{
    GameObject          m_HandleObj;
    List<Transform>     m_BarrelTransList = new List<Transform>();
    List<Animator>      m_AnimatorList = new List<Animator>();
    GameObject[]        m_BarrelLightdot = new GameObject[3];
    GameObject          m_MuzzleEft = null;             //冰炮炮口冰球特效
    Transform           m_BaseTransform;
    Transform           m_GaiTransfom;                      //盖子变换
    Transform           m_MuzzleEftTrans;                  //冰炮炮口冰球特效
    UILabel             m_BulletConsumeLabel;              //子弹基础消耗
    Vector2             m_GunPivot = new Vector2();        //炮管中心轴点的位置
    float               m_LightDotDelay;
    float               m_LightDotInterval;
    int                 m_LightDotFlag;
    float               m_AnimTimes;
    float               m_AnimInterval;                 //动画播放频率
    GunBarrelDate       m_GunBarrelDate;
    MuzzleEffectAnim    m_MuzzleEftAnim;
    bool                m_IsPlayAnim;            //
    int                 m_LightCount;
    ushort              m_BulletConsume;        //炮的发出子弹的基础伤害
    byte                m_Seat;

    public GunBarrel()
    {
        m_AnimTimes = 0.0f;
        m_AnimInterval = 0.2f;
        m_IsPlayAnim = false;
        m_LightDotDelay = 0.0f;
        m_LightDotInterval = 0.2f;
        m_LightDotFlag = 0;
        m_LightCount = 0;

        m_MuzzleEftAnim.bMoveEffect = false;
        m_MuzzleEftAnim.DelayInteval = 0.1f;
        m_MuzzleEftAnim.DelayTime = 0.0f;
    }
    public void Reset(byte type, byte seat)
    {
        m_Seat = seat;
        m_LightDotFlag = 0;
        m_LightCount = 0;
        m_IsPlayAnim = false;
        m_AnimInterval = LauncherSetting.LauncherDataList[type].Interval / 2;
        m_MuzzleEftAnim.bMoveEffect = false;
        m_MuzzleEftAnim.DelayInteval = 0.1f;
        m_MuzzleEftAnim.DelayTime = 0.0f;

        for (int j = 0; j < m_BarrelTransList.Count; )
            Utility.ListRemoveAt(m_BarrelTransList, j);
        for (int k = 0; k < m_AnimatorList.Count; )
            Utility.ListRemoveAt(m_AnimatorList, k);

    }

    public void Init()
    {
        InitGunBarrelDate();
        m_BaseTransform = this.HandleObj.transform;
        m_GaiTransfom = m_BaseTransform.GetChild(0).GetChild(0);
        int indx = m_GunBarrelDate.ComponentCount;

        for (int i = 0; i < m_GunBarrelDate.ComponentCount; i++)
        {
            GameObject go = null;
            go = m_BaseTransform.GetChild(i).gameObject;

            Transform trans = null;
            trans = go.transform;
            m_BarrelTransList.Add(trans);

            Animator animator = null;
            animator = trans.GetComponent<Animator>();
            animator.SetBool("Attack", false);
            m_AnimatorList.Add(animator);

        }
        if (m_GunBarrelDate.LightDot)
        {
            Transform transform = m_BarrelTransList[3].GetChild(0).gameObject.transform;
            m_LightCount = transform.childCount;
            for (int j = 0; j < m_LightCount; j++)
            {
                m_BarrelLightdot[j] = transform.GetChild(j).gameObject;
                if (m_BarrelLightdot[j] == null)
                    Debug.Log("Lightdot is null");
            }
        }
   
        if (m_GunBarrelDate.MuzzleEft)
        {
            m_MuzzleEft = m_BaseTransform.GetChild(indx + 2).gameObject;
            m_MuzzleEftTrans = m_MuzzleEft.transform;
            //加入到特效列表中为后面屏蔽管理方便
            if (m_MuzzleEft != null)
                m_MuzzleEft.SetActive(true);
           // SceneRuntime.LauncherEftMgr.EffectList.Add(m_MuzzleEft);
        }
        m_BulletConsumeLabel = m_BaseTransform.GetChild(indx).GetChild(0).gameObject.GetComponent<UILabel>();
        if (m_BulletConsumeLabel == null)
            Debug.Log("bulletconsume is null");
        ShowBulletConsume();
    }
    void InitGunBarrelDate()
    {
        LauncherComponent obj = m_HandleObj.GetComponent<LauncherComponent>();
        m_GunBarrelDate.LightDot = obj.m_LightDot;
        m_GunBarrelDate.MoveLight = obj.m_MoveLight;
        m_GunBarrelDate.MuzzleEft = obj.m_MuzzleEft;
        m_GunBarrelDate.PlaySpriteAnim = obj.m_PlaySpriteAnim;
        m_GunBarrelDate.ComponentCount = obj.m_ComponentCount;
        GameObject.Destroy(m_HandleObj.GetComponent<LauncherComponent>());
    }

    public void UpdatePivot()
    {
        if (m_BaseTransform == null)
            return;
        Vector3 pos =   SceneObjMgr.Instance.UICamera.WorldToScreenPoint(m_BaseTransform.position);
        m_GunPivot.x = pos.x;
        m_GunPivot.y = pos.y;

    }
    public void Update(float deltaTime)
    {

        if (m_IsPlayAnim && !m_MuzzleEftAnim.bMoveEffect && m_GunBarrelDate.MuzzleEft)
        {

            m_MuzzleEftAnim.DelayTime += deltaTime;
            if (m_MuzzleEftAnim.DelayTime >= m_MuzzleEftAnim.DelayInteval)
            {
                m_MuzzleEftAnim.DelayTime = 0;
                m_MuzzleEftAnim.bMoveEffect = true;
                m_MuzzleEft.SetActive(true);
                m_MuzzleEftTrans.localPosition = new Vector3(0.0f, -20.0f, 0.0f);
            }
        }

        m_AnimTimes += deltaTime;
        if (m_IsPlayAnim && m_AnimTimes >= m_AnimInterval)
            StopAnimation();
        if (!m_IsPlayAnim)
            StandbyAnimation(deltaTime);

        if (m_MuzzleEftAnim.bMoveEffect)
            MoveBallEffect(deltaTime);

        UpdatePivot();

    }
    public void ShowBulletConsume()
    {
        m_BulletConsumeLabel.text = m_BulletConsume.ToString();
    }
    
    void StandbyAnimation(float deltaTime)
    {
        if (m_GunBarrelDate.LightDot)
            LightDotFlicker(deltaTime);

    }
    void MoveBallEffect(float deltaTime)
    {
        Vector3 localPos = new Vector3();
        localPos = m_MuzzleEftTrans.localPosition;
        localPos.y += 150.0f * deltaTime;
        if (localPos.y >= 0.0f)
        {
            localPos.y = 0.0f;
            m_MuzzleEftAnim.bMoveEffect = false;
        }
        m_MuzzleEftTrans.localPosition = localPos;

    }
    void LightDotFlicker(float deltaTime)
    {
        m_LightDotDelay += deltaTime;

        if (m_LightDotDelay >= m_LightDotInterval)
        {
            m_LightDotDelay = 0.0f;
            for (int i = 0; i < 3; i++)
                m_BarrelLightdot[i].SetActive(false);

            m_BarrelLightdot[m_LightDotFlag++].SetActive(true);
            m_LightDotFlag = (m_LightDotFlag >= 3) ? 0 : m_LightDotFlag;
        }
    }
    public void UpdateAngle(float angle)
    {
        m_BaseTransform.localEulerAngles = new Vector3(0, 0, angle);
    }
    public void PlayAnimation()
    {
        try
        {
            for (int i = 0; i < m_LightCount; i++)
                m_BarrelLightdot[i].SetActive(false);
            ShowActive(false);
            m_IsPlayAnim = true;
            m_AnimTimes = 0;

            if (m_GunBarrelDate.MoveLight)
                SceneRuntime.LauncherEftMgr.SetActiveMoveLight(false, m_Seat);

            if (m_GunBarrelDate.MuzzleEft && m_MuzzleEft != null)
                m_MuzzleEft.SetActive(false);

            for (int j = 0; j < m_GunBarrelDate.ComponentCount; j++)
            {
                if (j < m_AnimatorList.Count && m_AnimatorList[j] != null)
                    m_AnimatorList[j].SetBool("Attack", true);
            }
        }
        catch(System.Exception e)
        {
            ReportException.Instance.AddException("PlayAnimation Exception:" + e.ToString());
        }
    }
    void StopAnimation()
    {
        try
        {
            //停止动画 
            m_IsPlayAnim = false;
            for (int i = 0; i < m_LightCount; i++)
                m_BarrelLightdot[i].SetActive(true);

            // if (!SceneRuntime.MaskEffect)
            ShowActive(true);

            if (m_GunBarrelDate.MoveLight)
                SceneRuntime.LauncherEftMgr.SetActiveMoveLight(true, m_Seat);

            for (int j = 0; j < m_GunBarrelDate.ComponentCount; j++)
            {
                if (j < m_AnimatorList.Count && m_AnimatorList[j] != null)
                    m_AnimatorList[j].SetBool("Attack", false);
            }
        }
        catch(System.Exception e)
        {
            ReportException.Instance.AddException("StopAnimation Exception:" + e.ToString());
        }
    }
    void ShowActive(bool active)
    {
        //if (SceneRuntime.MaskEffect)
        //{
            if (m_GunBarrelDate.MuzzleEft && m_MuzzleEft != null)
                m_MuzzleEft.SetActive(active);
      //  }
    }
    //屏蔽冰冻炮的球
    public void MaskMuzzleEft(bool bMask)
    {
        if (m_MuzzleEft != null )
            m_MuzzleEft.SetActive(false);
      //  SceneRuntime.LauncherEftMgr.EffectList.Add(m_MuzzleEft);

        //else if (m_MuzzleEft != null && SceneRuntime.MaskEffect)
        //    m_MuzzleEft.SetActive(bMask);
    }
    public GameObject HandleObj
    {
        get { return m_HandleObj; }
        set { m_HandleObj = value; }
    }
    public Transform BaseTransform
    {
        get { return m_BaseTransform; }
    }
    public Vector2 GunPivot
    {
        get { return m_GunPivot; }
        set { m_GunPivot = value; }
    }
    public Transform GaiTransfom
    {
        get { return m_GaiTransfom; }
    }
    public ushort BulletConsume
    {
        get { return m_BulletConsume; }
        set { m_BulletConsume = value; }
    }
    public void ShutDown()
    {
        if (m_HandleObj != null)
            GameObject.Destroy(m_HandleObj);
    }
}

   

    
     