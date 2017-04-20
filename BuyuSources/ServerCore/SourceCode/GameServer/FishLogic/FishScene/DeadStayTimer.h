#pragma once
#include "Random.h"
#include "RuntimeInfo.h"
#define INV_255 0.00392156f
class DeadStayTimer
{
public:
	static BYTE RandByte()
	{
		return (BYTE)RandRange(1, 256);
	}
	static float ByteToFloat(BYTE t)
	{
		return t * INV_255;
	}
	static BYTE GetLaserDeadTime(byte LauncherType, float &speedScaling, float deadTime[3]);
	static BYTE GetSkillDeadTime(SkillType skillType, float &speedScaling, float deadTime[3]);
	static BYTE GetBulletFreezeDeadTime(float &deadTime);
};