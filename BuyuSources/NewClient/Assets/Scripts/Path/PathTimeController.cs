using UnityEngine;
using System.Collections.Generic;

public enum FISH_DELAY_TYPE
{
    DELAY_NONE = 0,
    DELAY_ATTACK,	//受击减速,低级
    DELAY_SKILL,	//技能减速，高级
}
public class FishDelayData
{
    public FishDelayData()
    {
        DelayType = FISH_DELAY_TYPE.DELAY_NONE;
        Duration = new float[3];
        DelayFactor = 1.0f;
    }
    public void ComputeAllDelayTime()
    {
        Duration[2] += 0.000001f;
        DurationTime2 = Duration[0] + Duration[1];
        AllTime = DurationTime2 + Duration[2];
        if(Duration[0] != 0)
            invDuration0 = 1.0f / Duration[0];
        if(Duration[2] != 0)
            invDuration2 = 1.0f / Duration[2];
    }
    public float GetAllDelayTime()
    {
        return AllTime;
    }
	public float ComputeScaling(float delta)
	{
        if (DelayType == FISH_DELAY_TYPE.DELAY_NONE)
			return 1.0f;

		CurrentTime += delta;
		float t;
		if (CurrentTime < Duration[0])
		{
            t = CurrentTime * invDuration0;
            DelayFactor = 1.0f - t;
			return Utility.LerpFloat(1.0f, Scaling, t);
		}
        else if (CurrentTime < DurationTime2)
		{
            DelayFactor = 0.0f;
			return Scaling;
		}
        else if (CurrentTime < AllTime)
		{
            t = (CurrentTime - DurationTime2) * invDuration2;
            t = Mathf.Min(t, 1);
            DelayFactor = t;
            return Utility.LerpFloat(Scaling, 1.0f, t);
		}
		else
		{
            DelayType = FISH_DELAY_TYPE.DELAY_NONE;
			return 1.0f;
		}
	}
    public float invDuration0;
    public float invDuration2;
    public float DelayFactor;
    public float AllTime;
    public float DurationTime2;
	public float Scaling;			//技能造成的时间缩放
	public float[] Duration;
	public float CurrentTime;
	public FISH_DELAY_TYPE DelayType;
};
//影响速度的因素:
//1.距离的长短，使不同长短的路径上，保持恒定的速度。
//2.路径上路径点的速度缩放.
//3.鱼的速度。
//4.技能的速度缩放。
public class PathTimeController
{
    public static int DIST_TIME_SCALING = 30; //距离30 进行时间标准缩放。
    float m_Time;               //当前的时间
    float m_OrignalSpeed;       //原始的速度
    float m_PathTimeScaling;    //路径点的缩放.
    float m_Speed;              //最终的速度
    bool  m_bDirty;
    bool  m_bEnablePathTimeScaling;
    float m_DistTimeScaling;    //按照距离进行时间缩放，距离越长，耗费的时间越多。  
    public float DistTimeScaling
    {
        get { return m_DistTimeScaling; }
        set { m_DistTimeScaling = value; }
    }
    FishDelayData m_DelayData = new FishDelayData();

    public float DelayFactor
    {
        get
        {
            return m_DelayData.DelayFactor;
        }
    }
    public FishDelayData DelayData
    {
        get { return m_DelayData; }
        set { m_DelayData = value; }
    }
    public bool HasDelay()
    {
        return m_DelayData.DelayType != FISH_DELAY_TYPE.DELAY_NONE;
    }
    public float DelayTimePercent
    {
        get
        {
            return m_DelayData.CurrentTime / m_DelayData.AllTime;
        }
    }
    public bool EnablePathTimeScaling
    {
        get { return m_bEnablePathTimeScaling; }
        set {
            if (m_bEnablePathTimeScaling != value)
            {
                m_bEnablePathTimeScaling = value;
                m_bDirty = true;
            }
        }
    }
    public void AddSkillTimeScaling(float scaling, float duration, FISH_DELAY_TYPE type)
    {
        m_bDirty = true;
        m_DelayData.DelayType   = type;
        m_DelayData.Scaling     = scaling;
        m_DelayData.Duration[0] = 0;
        m_DelayData.Duration[1] = 0;
        m_DelayData.Duration[2] = duration;
        m_DelayData.CurrentTime = 0;

        m_DelayData.ComputeAllDelayTime();
    }
    public void AddSkillTimeScaling(float scaling, float[] duration, FISH_DELAY_TYPE type, float curTime = 0)
    {
        m_bDirty = true;
        m_DelayData.DelayType   = type;
        m_DelayData.Scaling     = scaling;
        m_DelayData.Duration[0] = duration[0];
        m_DelayData.Duration[1] = duration[1];
        m_DelayData.Duration[2] = duration[2];
        m_DelayData.CurrentTime = curTime;
        m_DelayData.ComputeAllDelayTime();
    }
    public void AddSkillTimeScaling(ReductionData reduction, FISH_DELAY_TYPE type)
    {
        m_bDirty = true;
        m_DelayData.DelayType = type;
        m_DelayData.Scaling = reduction.Speed;
        m_DelayData.Duration[0] = reduction.Duration1;
        m_DelayData.Duration[1] = reduction.Duration2;
        m_DelayData.Duration[2] = reduction.Duration3;
        m_DelayData.CurrentTime = 0;
        m_DelayData.ComputeAllDelayTime();
    }
    public void Reset(float maxDist, float speed, float time)
    {
        m_DelayData.DelayType = FISH_DELAY_TYPE.DELAY_NONE;
        m_PathTimeScaling = 1.0f;
        m_Time = time;
        m_OrignalSpeed = speed;
        m_bDirty = true;
        m_bEnablePathTimeScaling = true;
        m_DistTimeScaling = DIST_TIME_SCALING / maxDist;
        ComputeTimeSpeed(0);
    }
    public float Time
    {
        get
        {
            return m_Time;
        }
        set
        {
            m_Time = value;
        }
    }
    public float PathTimeScaling
    {
        get
        {
            return m_PathTimeScaling;
        }
        set
        {
            if(m_PathTimeScaling != value)
            { 
                m_PathTimeScaling = value;
                m_bDirty = true;
            }
        }
    }
    void ComputeTimeSpeed(float delta)
    {
        m_Speed = m_OrignalSpeed;
        if (m_bEnablePathTimeScaling)
            m_Speed *= m_PathTimeScaling * m_DistTimeScaling;

        if(HasDelay())  //减速效果
            m_Speed *= m_DelayData.ComputeScaling(delta);
        m_bDirty = false;
    }
    public float OrignalSpeed
    {
        get
        {
            return m_OrignalSpeed;
        }
        set
        {
            m_OrignalSpeed = value;
            m_bDirty = true;
        }
    }
    public float CurrentSpeed
	{
        get
        {
            return m_Speed;
        }
	}
    public float Update(float delta)
    {
        if (HasDelay() || m_bDirty)
            ComputeTimeSpeed(delta);
        m_Time += delta * m_Speed;
        return m_Time;
    }
}
