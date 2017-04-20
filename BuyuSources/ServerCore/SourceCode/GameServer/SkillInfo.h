#pragma once
#include<vector>

class CSkillInfo
{
public:
	CSkillInfo();
	 bool IsCoolDown();
	 void EmitSkill(float nCdtime);
	 USHORT UsedTimes(byte bySkillid);
	 void RecordSkill(byte bySkillid);
	 void Reset();
	 
private:
	 DWORD m_dwCdTime;
	 std::vector<byte> m_vecConsume;
};
