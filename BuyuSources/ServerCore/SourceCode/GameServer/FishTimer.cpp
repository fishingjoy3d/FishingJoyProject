#include "stdafx.h"
#include "FishTimer.h"

CFishTimer::CFishTimer()
{
	m_nSpace = 0;
	m_fLeave = 0;
	m_dwRepeat = 0;
}

void CFishTimer::StartTimer(int nSapce, DWORD dwRepeat)
{
	m_nSpace = nSapce;
	m_fLeave = nSapce * 1.0f;
	m_dwRepeat = dwRepeat;
}

bool CFishTimer::Update(float dt)
{
	if (m_dwRepeat != 0)
	{
		m_fLeave -= dt;
		if (m_fLeave <= 0)
		{
			m_dwRepeat--;
			m_fLeave = m_nSpace* 1.0f;
			return true;
		}
	}
	return false;
}