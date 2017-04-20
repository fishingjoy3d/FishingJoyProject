#pragma once

class Timer
{
public:
	Timer()
	{
		Elapsed = 0;
	}
	void Begin()
	{
		::QueryPerformanceCounter(&li1);
	}
	float Timer::Intecept()
	{
		::QueryPerformanceCounter(&li2);
		Elapsed = ((li2.QuadPart - li1.QuadPart) * 1000.0f / ms_PerformanceFrequency.QuadPart);
		li1.QuadPart = li2.QuadPart;
		return Elapsed;
	}

	float Elapsed;
protected:

	LARGE_INTEGER li1, li2;
	static LARGE_INTEGER ms_PerformanceFrequency;
};