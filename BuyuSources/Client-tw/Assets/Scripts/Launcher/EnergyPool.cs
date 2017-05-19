using UnityEngine;
using System.Collections.Generic;

public class EnergyPool
{
    int         m_EnergyMax;                //能量值
    int         m_CurEnergy;                //当前能量值
    float       m_RollbackInterval;         //回滚频率
    float       m_RollbackTime;
    float       m_LaserCDTime;              //大招CD
    float       m_CurCDTime;
    float       m_HitRecoverTime;           //硬直时间
    float       m_CurHitRecover;
    float       m_FillAmount;
    bool        m_bFull;
    bool        m_bLaserCDState;            //是否为CD状态  
    bool        m_bHitRecoverState;         //是否为硬直状态
    bool        m_bRollback;
    bool        m_bSendXPSkill;



    public void InitEnergy(byte type)
    {
        m_bFull = false;
        m_bRollback = false;
        m_bSendXPSkill = false;
        m_bLaserCDState = false;
        m_bHitRecoverState = false;
        m_CurEnergy = 0;
        m_CurCDTime = 0;
        m_FillAmount = 0;
        m_RollbackInterval = 0.3f;
        m_RollbackTime = 0;
        m_HitRecoverTime = 1.0f;
        m_EnergyMax = LauncherSetting.LauncherDataList[type].Energy;
        m_LaserCDTime = LauncherSetting.LauncherDataList[type].LaserCDTime;
        
    }
    public void Update(float delta)
    {
        m_CurHitRecover += delta;
        if (m_CurHitRecover >= m_HitRecoverTime)
        {
            m_CurHitRecover = 0;
            m_bHitRecoverState = false;
            m_bSendXPSkill = true;
        }
    }
    public int MaxEnergy
    {
        get { return m_EnergyMax; }
        set { m_EnergyMax = value; }
    }
    public int CurEnergy
    {
        get { return m_CurEnergy; }
        set { m_CurEnergy = value; }
    }
    public void UpdateEnergyPool(byte type, byte rateIdx,uint Energy)
    {
        m_CurEnergy += LauncherSetting.LauncherDataList[type].Consume * BulletSetting.BulletRate[rateIdx];
        m_CurEnergy = (int)Energy;
        m_FillAmount = (float)m_CurEnergy / (m_EnergyMax * BulletSetting.BulletRate[rateIdx]);

        if (m_FillAmount >= 1.0f)
        {
            m_bFull = true;
            m_bHitRecoverState = true;
        }
    }
    public void UpdateEnergyPool(byte rateIdx)
    {
        m_FillAmount = (float)m_CurEnergy / (m_EnergyMax * BulletSetting.BulletRate[rateIdx]);

        if (m_FillAmount >= 1.0f)
        {
            m_bFull = true;
            m_bHitRecoverState = true;
        }
        else
        {
            m_bFull = false;
            m_CurHitRecover = 0;
            m_bHitRecoverState = false;
            m_bSendXPSkill = false;
        }
    }
    public void PlayRollbackPoolUI(float delta)
    {
        if (!m_bRollback)
            return;
        m_RollbackTime += delta;
        float fillAmount = m_RollbackTime / m_RollbackInterval;
        m_FillAmount = 1 - fillAmount;

        if (m_FillAmount <= 0.0f)
        {
            m_FillAmount = 0;
            m_bRollback = false;
            m_bFull = false;
            m_CurEnergy = 0;
            m_RollbackTime = 0;
        }
    }
    public void PlayCD(float delta)
    {
        if (!m_bLaserCDState)
            return;
        m_CurCDTime += delta;
        if (m_CurCDTime >= m_LaserCDTime)
        {
            m_CurCDTime = 0;
            m_bLaserCDState = false;
        }
    }
    public bool LaserCDState
    {
        get { return m_bLaserCDState; }
        set { m_bLaserCDState = value; }
    }
    public float LaserCDTime
    {
        get { return m_LaserCDTime; }
        set { m_LaserCDTime = value; }
    }
    public float FillAmount
    {
        get { return m_FillAmount; }
        set { m_FillAmount = value; }
    }
  
    public bool Full
    {
        get { return m_bFull; }
        set { m_bFull = value; }
    }
    public bool Rollback
    {
        get { return m_bRollback; }
        set { m_bRollback = value; }
    }
    public bool HitRecoverState
    {
        get { return m_bHitRecoverState; }
        set { m_bHitRecoverState = value; }
    }
    public bool bSendXPSkill
    {
        get { return m_bSendXPSkill; }
        set { m_bSendXPSkill = value; }
    }
}
