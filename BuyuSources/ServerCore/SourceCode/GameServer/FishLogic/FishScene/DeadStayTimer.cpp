#include "stdafx.h"
#include "FishResData.h"
#include "DeadStayTimer.h"
#include "FishResManager.h"
BYTE DeadStayTimer::GetLaserDeadTime(byte LauncherType,float &speedScaling, float deadTime[3])
{
	const DeadStayTime &dst = FishResManager::Inst()->GetDeadStayTime();
	BYTE bt = RandByte();
	float t = ByteToFloat(bt);
	deadTime[0] = 0;
	deadTime[1] = 0;
	deadTime[2] = 0;
	switch (LauncherType)
	{
	case 0:
	case 1:
	case 2:
	case 4:
		deadTime[0] = dst.JiGuang_Stay_Time;
		deadTime[1] = LerpFloat(dst.JiGuang_Dead_Time1, dst.JiGuang_Dead_Time2, t);
		speedScaling = dst.JiGuang_Speed;
		break;
	case 3:
		deadTime[1] = LerpFloat(dst.JiGuang_Dead_Time1, dst.JiGuang_Dead_Time2, t);
		speedScaling = dst.JiGuang_BingDong_Speed;
		break;
	default:
		speedScaling = 0;
		break;
	}
	return bt;
}
BYTE DeadStayTimer::GetSkillDeadTime(SkillType skillType, float &speedScaling, float deadTime[3])
{
	const DeadStayTime &dst = FishResManager::Inst()->GetDeadStayTime();
	BYTE bt = RandByte();
	float t = ByteToFloat(bt);
	deadTime[0] = 0;
	deadTime[1] = 0;
	deadTime[2] = 0;
	switch (skillType)
	{
	case SKILL_FREEZE:
		deadTime[1] = LerpFloat(dst.BingDong_Dead_Time1, dst.BingDong_Dead_Time2, t);
		speedScaling = dst.BingDong_Speed;
		break;
	case SKILL_LIGHTING:
		deadTime[0] = LerpFloat(dst.ShanDian_Dead_Time1, dst.ShanDian_Dead_Time2, t);
		deadTime[1] = 1.5f;
		speedScaling = dst.ShandDian_Speed;
		break;
	case SKILL_DISASTER:
		deadTime[0] = LerpFloat(dst.TianZai_Dead_Time1, dst.TianZai_Dead_Time2, t) + dst.TianZai_DouDong_Time;
		deadTime[1] = LerpFloat(dst.TianZai_Stay_Time1, dst.TianZai_Stay_Time2, t);
		speedScaling = dst.TianZai_Speed;
		break;
	default:
		speedScaling = 0;
		break;
	}
	return bt;
}
BYTE DeadStayTimer::GetBulletFreezeDeadTime(float &deadTime)
{
	const DeadStayTime &dst = FishResManager::Inst()->GetDeadStayTime();
	BYTE bt = RandByte();
	float t = ByteToFloat(bt);
	deadTime = LerpFloat(dst.Bullet_BingDong_Dead_Time1, dst.Bullet_BingDong_Dead_Time2, t);
	return bt;
}
