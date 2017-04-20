#include "stdafx.h"
#include "SkillInfo.h"
#include<timeapi.h>
#include<algorithm>

CSkillInfo::CSkillInfo()
{
	m_dwCdTime = 0;
}
bool CSkillInfo::IsCoolDown()
{
	return  timeGetTime() >= m_dwCdTime ;
}

void CSkillInfo::EmitSkill(float nCdtime)
{
	m_dwCdTime = timeGetTime() + ConvertFloatToDWORD(nCdtime * 1000);//
}

USHORT CSkillInfo::UsedTimes(byte bySkillid)
{
	return ConvertDWORDToWORD(std::count(m_vecConsume.begin(), m_vecConsume.end(), bySkillid));
}

void CSkillInfo::RecordSkill(byte bySkillid)
{
	m_vecConsume.push_back(bySkillid);
}
void CSkillInfo::Reset()
{
	m_vecConsume.clear();
}