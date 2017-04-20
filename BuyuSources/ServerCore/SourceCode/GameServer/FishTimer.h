#pragma once
class CFishTimer
{
public:
	CFishTimer();
	void StartTimer(int nSapce, DWORD dwRepeat);
	bool Update(float dt);
private:
	int m_nSpace;
	float m_fLeave;
	DWORD m_dwRepeat;
};

