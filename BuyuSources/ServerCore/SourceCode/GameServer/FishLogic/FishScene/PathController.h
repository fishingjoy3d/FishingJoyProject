#pragma once
#include "RuntimeInfo.h"
#include "FishResData.h"
#include "PathLinearInterpolator.h"
#include "PathTimeController.h"
#include "PathEvent.h"
#define SPEED_UNIT 0.02222222f
class PathController
{
public:
	PathLinearInterpolator m_Interp;
	PathTimeController m_TimeController;
	PathEvent	m_PathEvent;
	Vector3     m_Position;    //当前插值的位置
	Quaternion  m_Rotation;   //当前的持续旋转
	int			m_NodeIdx;

	float EventElapsedTime()
	{
		return m_PathEvent.m_CurElapsedTime;
	}
	Matrix4x4& GetWorldMatrix()
	{
		return m_Interp.pPathData->WorldRTS;
	}
	Quaternion& GetWorldRotation()
	{
		return m_Interp.pPathData->WorldRot;
	}
	Vector3 &GetPosition()
	{
		return m_Position;
	}
	Quaternion& GetRotation()
	{
		return m_Rotation;
	}
	bool IsActiveEvent()
	{
		return m_PathEvent.IsActiveEvent();
	}
	bool IsActiveLaugh()
	{
		return m_PathEvent.IsActiveLaugh();
	}
	void StopLaugh()
	{
		m_PathEvent.Reset(0, false);
	}
	bool HasPathEvent()
	{
		return m_Interp.pPathData->HasPathEvent;
	}
	bool CheckDeactiveLaugh()
	{
		return m_PathEvent.CheckDeactiveLaugh();
	}
	bool CheckCurrentEvent()
	{
		return m_PathEvent.CheckCurrentEvent(m_NodeIdx, GetCurrentNode());
	}
	
	LinearNodeData &GetCurrentNode()
	{
		return m_Interp.pPathData->pNodeList[m_NodeIdx];
	}

	void ResetController(PathData *pPath, float speed, float time, float laughTime)
	{
		m_PathEvent.Reset(laughTime);
		m_Interp.InitFromPathData(pPath);
		m_TimeController.Reset(m_Interp.pPathData->SampleMaxDistance, speed, time);
		GetPRT();
	}
	float GetTime()
	{
		return m_TimeController.m_Time;
	}
	float SetTime(float value)
	{
		m_TimeController.m_Time = value;
		GetPRT();
	}
	void GetPRT()
	{
		float timeScaling;
		float time = Clamp(m_TimeController.m_Time, 0.0f, 1.0f);
		m_NodeIdx = m_Interp.GetPosRotTimeScaling(time, m_Position, m_Rotation, timeScaling);
	}
	float GetCurrentSpeed()
	{
		return m_TimeController.GetCurrentSpeed();
	}
	float GetOrignalSpeed()
	{
		return m_TimeController.GetOrignalSpeed();
	}
	void SetOrignalSpeed(float value)
	{
		m_TimeController.SetOrignalSpeed(value);
	}
	void AddElapsedTime(float deltaTime)
	{
		//按照每秒45秒的速度计算
		while (deltaTime > SPEED_UNIT)
		{
			AddDelta(SPEED_UNIT);
			deltaTime -= SPEED_UNIT;
		}
		AddDelta(deltaTime);
	}
	void AddDelta(float deltaTime)
	{
		if (HasPathEvent())
		{
			deltaTime = m_PathEvent.Update(deltaTime);
			if (m_PathEvent.IsActiveEvent())
				return;
			if (CheckCurrentEvent())
			{
				//播放嘲讽
			}
			if (CheckDeactiveLaugh())
			{
				//结束嘲讽
			}
		}
		if (m_TimeController.Update(deltaTime) > 0.0f)
			m_TimeController.SetPathTimeScaling(m_Interp.GetTimeScaling(m_TimeController.m_Time, m_NodeIdx));
	}
	bool Update(float deltaTime)
	{
		AddElapsedTime(deltaTime);
		if ((deltaTime > SPEED_UNIT || !m_PathEvent.IsActiveEvent()) && m_TimeController.m_Time > 0.0f)
		{
			GetPRT();
			return true;
		}
		else
			return false;
	}
	bool HasDelay()
	{
		return m_TimeController.HasDelay();
	}
	bool CheckDelayLevel(FISH_DELAY_TYPE type)
	{
		return m_TimeController.CheckDelayLvl(type);
	}
	void AddSkillTimeScaling(float scaling, float duration, FISH_DELAY_TYPE delay)
	{
		return m_TimeController.AddSkillTimeScaling(scaling, duration, delay);
	}
	void AddSkillTimeScaling(float scaling, float duration[3], FISH_DELAY_TYPE delay)
	{
		return m_TimeController.AddSkillTimeScaling(scaling, duration, delay);
	}
	FishDelayData& GetDelayData()
	{
		return m_TimeController.m_DelayData;
	}
};
