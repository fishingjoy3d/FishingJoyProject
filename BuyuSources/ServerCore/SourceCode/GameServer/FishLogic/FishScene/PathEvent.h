#pragma once
#include "RuntimeInfo.h"
#include "FishResData.h"

class PathEvent
{
public:
	int             m_NodeIdx;
	PathEventType   m_EventType;
	float           m_CurElapsedTime;
	float           m_LaughTime;
	int             m_StayTimes;
	bool            m_DeactiveLaugh;//结束嘲讽
	bool			m_bEnableEvent;
	bool IsActiveEvent()
	{
		return m_EventType != EVENT_NONE;
	}
	bool IsActiveLaugh()
	{
		return m_EventType != EVENT_LAUGH;
	}
	bool CheckDeactiveLaugh()
	{
		if (m_DeactiveLaugh)
		{
			m_DeactiveLaugh = false;
			return true;
		}
		else
			return false;
	}
	void Reset(float laughTime, bool EnableEvent = true)
	{
		m_NodeIdx = 0;
		m_EventType = EVENT_NONE;
		m_CurElapsedTime = 0;
		m_LaughTime = laughTime;
		m_StayTimes = 0;
		m_DeactiveLaugh = false;
		m_bEnableEvent = EnableEvent;
	}
	bool CheckCurrentEvent(int nodeIdx, const LinearNodeData &sd)
	{
		if (m_NodeIdx != nodeIdx)
		{
			m_NodeIdx = nodeIdx;
			if (sd.EventType == EVENT_STAY)
			{
				m_EventType = (PathEventType)sd.EventType;
				m_StayTimes = sd.Times;
			}
			else  if (sd.EventType == EVENT_LAUGH && m_LaughTime != 0)
			{
				m_EventType = (PathEventType)sd.EventType;
				m_StayTimes = sd.Times;
				return true;
			}
		}
		return false;
	}

	float Update(float fDelta)
	{
		if (!m_bEnableEvent)
			return fDelta;
		if (m_EventType == EVENT_NONE)
			return fDelta;

		m_CurElapsedTime += fDelta;
		if (m_EventType == EVENT_LAUGH)
		{
			int count = int(m_CurElapsedTime / m_LaughTime);
			if (count >= m_StayTimes)
			{
				//已经结束嘲笑了
				m_EventType = EVENT_NONE;
				fDelta = fmodf(m_CurElapsedTime, m_LaughTime);
				m_CurElapsedTime = 0.0f;
				m_DeactiveLaugh = true;
			}
		}
		else
		{
			//停留时间计时
			float stayTime = m_StayTimes * 0.001f;
			if (m_CurElapsedTime >= stayTime)
			{
				m_EventType = EVENT_NONE;
				fDelta = fmodf(m_CurElapsedTime, stayTime);
				m_CurElapsedTime = 0.0f;
			}
		}
		return fDelta;
	}
};
