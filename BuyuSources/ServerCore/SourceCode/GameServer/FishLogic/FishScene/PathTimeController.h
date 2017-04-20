#pragma once
#include "RuntimeInfo.h"

//减速必须是0.5秒的整数倍
enum FISH_DELAY_TYPE
{
	DELAY_NONE = 0,
	DELAY_ATTACK,	//受击减速,低级
	DELAY_BULLET,	//冰冻炮减速，中级
	DELAY_SKILL,	//技能减速，高级
};
struct FishDelayData
{
	FishDelayData()
	{
		DelayType = DELAY_NONE;
	}
	void ComputeAllTime()
	{
		Duration[0] += 0.00001f;
		Duration[2] += 0.00001f;
		Time2 = Duration[0] + Duration[1];
		AllTime = Time2 + Duration[2];
	}
	float ComputeScaling(float delta)
	{
		if (DelayType == DELAY_NONE)
			return 1.0f;
		CurrentTime += delta;
		float t;
		if (CurrentTime < Duration[0])
		{
			t = CurrentTime / Duration[0];
			return LerpFloat(1.0f, Scaling, t);
		}
		else if (CurrentTime < Time2)
		{
			return Scaling;
		}
		else if (CurrentTime < AllTime)
		{
			float pretime = Time2;
			t = (CurrentTime - pretime) / Duration[2];
			t = min(t, 1);
			return LerpFloat(Scaling, 1.0f, t);
		}
		else
		{
			DelayType = DELAY_NONE;
			return 1.0f;
		}
	}
	float AllTime;
	float Time2;
	float Scaling;			//技能造成的时间缩放
	float Duration[3];
	float CurrentTime;
	FISH_DELAY_TYPE DelayType;
};

//影响速度的因素:
//1.距离的长短，使不同长短的路径上，保持恒定的速度。
//2.路径上路径点的速度缩放.
//3.鱼的速度。
//4.技能的速度缩放。
class PathTimeController
{
public:
	enum
	{
		DIST_TIME_SCALING  = 30
	};
	float m_Time;               //当前的时间
	float m_OrignalSpeed;       //原始的速度
	float m_PathTimeScaling;    //路径点的缩放.
	float m_Speed;              //最终的速度
	bool  m_bDirty;
	bool  m_bEnablePathTimeScaling;
	float m_DistTimeScaling;    //按照距离进行时间缩放，距离越长，耗费的时间越多。    
	FishDelayData m_DelayData;
	bool HasDelay()
	{
		return m_DelayData.DelayType != DELAY_NONE;
	}
	bool CheckDelayLvl(FISH_DELAY_TYPE type)
	{
		return type > m_DelayData.DelayType || type == DELAY_SKILL;
	}
	bool EnablePathTimeScaling(bool value)
	{
		if (m_bEnablePathTimeScaling != value)
		{
			m_bEnablePathTimeScaling = value;
			m_bDirty = true;
		}
	}
	float GetAllDelayTime()const
	{
		return m_DelayData.AllTime;
	}
	void AddSkillTimeScaling(float scaling, float duration, FISH_DELAY_TYPE type)
	{
		m_bDirty = true;
		m_DelayData.DelayType = type;
		m_DelayData.Scaling = scaling;
		m_DelayData.Duration[0] = 0;
		m_DelayData.Duration[1] = 0;
		m_DelayData.Duration[2] = duration;
		m_DelayData.CurrentTime = 0;
		m_DelayData.ComputeAllTime();
	}
	void AddSkillTimeScaling(float scaling, float duration[3], FISH_DELAY_TYPE delayType)
	{
		m_bDirty = true;
		m_DelayData.DelayType	= delayType;
		m_DelayData.Scaling		= scaling;
		memcpy(m_DelayData.Duration, duration, sizeof(m_DelayData.Duration));
		m_DelayData.CurrentTime = 0;
		m_DelayData.ComputeAllTime();
	}
	void Reset(float maxDist, float speed, float time)
	{
		m_DelayData.DelayType = DELAY_NONE;
		m_PathTimeScaling = 1.0f;
		m_Time = time;
		m_OrignalSpeed = speed;
		m_bDirty = true;
		m_bEnablePathTimeScaling = true;
		m_DistTimeScaling = DIST_TIME_SCALING / maxDist;
	}
	void SetPathTimeScaling(float value)
	{
		if (m_PathTimeScaling != value)
		{
			m_PathTimeScaling = value;
			m_bDirty = true;
		}
	}
	void ComputeTimeSpeed(float delta)
	{
		if (!HasDelay() && !m_bDirty)
			return;

		m_Speed = m_OrignalSpeed;
		if (m_bEnablePathTimeScaling)
			m_Speed *= m_PathTimeScaling * m_DistTimeScaling;

		if (HasDelay())  //减速效果
			m_Speed *= m_DelayData.ComputeScaling(delta);
		m_bDirty = false;
	}
	void SetOrignalSpeed(float value)
	{
		m_OrignalSpeed = value;
		m_bDirty = true;
	}
	float GetCurrentSpeed()const
	{
		return m_Speed;
	}
	float GetOrignalSpeed()const
	{
		return m_OrignalSpeed;
	}
	float Update(float delta)
	{
		ComputeTimeSpeed(delta);
		m_Time += delta * m_Speed;
		return m_Time;
	}
};
