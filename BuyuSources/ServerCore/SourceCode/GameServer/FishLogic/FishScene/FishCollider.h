#pragma once
#include "BulletManager.h"
#include "FishManager.h"
#include "../NetCmd.h"
class Bullet;
class Fish;
struct NetCmd;
class FishSendInterface;
struct NetCmdSkillLaser;
class CTableRoleManager;
struct FishCatchData
{
	FishCatchData()
	{
		nReward = 0;
		lithingFishID = 0;
	}
	FishCatchData(FISHID id)
	{
		fishID = id;
		nReward = 0;
		lithingFishID = 0;
	}
	FishCatchData(FISHID id, FishCatchEventType e)
	{
		fishID = id;
		eventType = e;
		nReward = 0;
		lithingFishID = 0;
	}
	FISHID fishID;
	FishCatchEventType eventType;
	ushort nReward;
	ushort lithingFishID;
};
struct DoublePoint
{
	DoublePoint(int x, int y)
	{
		a = x;
		b = y;
	}
	int a;
	int b;
};

struct LineData
{
	LineData(const Vector2 p1, const Vector2 p2, float radius, float radius2)
	{
		StartPt = p1;
		EndPt = p2;
		Dir = EndPt - StartPt;
		Dist = Vec2Length(Dir);
		Dir /= Dist;
		Radius = radius;
		RadiusSq = radius2;
	}
	float	RadiusSq;
	float	Radius;
	Vector2 StartPt;
	Vector2 EndPt;
	float	Dist;
	Vector2 Dir;
};

class FishCollider
{
public :
	static void Collide(CTableRoleManager *pm, BulletManager *pBulletMgr, FishManager *pFishMgr, FishSendInterface *pSend);
	static void CollideByDir(vector<FishInfo> &fishList, byte laserType, NetCmdSkillLaser* pCmd, const Vector3 &pos, const Vector3 &dir, FishManager *pFishMgr);

};
extern bool CollideLine(const LineData &ld, Fish *pFish);
extern bool CollideRadius(float radius, float radius2, const Vector2 &scrCenter, Fish *pFish);
extern bool Intersect(const Vector3 &pos, const Matrix &mat, const Vector3 &size, const Ray &ray, Vector3 &retPoint);