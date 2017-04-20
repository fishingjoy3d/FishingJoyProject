#include "stdafx.h"
#include "Timer.h"

LARGE_INTEGER GetFrequency()
{
	LARGE_INTEGER li;
	::QueryPerformanceFrequency(&li);
	return li;
}
LARGE_INTEGER Timer::ms_PerformanceFrequency = GetFrequency();

