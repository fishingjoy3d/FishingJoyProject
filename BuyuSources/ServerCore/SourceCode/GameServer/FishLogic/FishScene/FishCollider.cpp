#include "stdafx.h"
#include "FishResData.h"
#include "FishCollider.h"
#include "BulletManager.h"
#include "FishManager.h"
#include "../FishCallbak.h"
#include "../NetCmd.h"
#include "Random.h"
#include "FishSendInterface.h"
#include "RuntimeInfo.h"
#include "Timer.h"
#define LOCK_COLLIDE_RADIUS 2.0f
#define PADDING_WIDTH 0.95f
const float RADIUS_WIDTH = 0.5f;
const Vector3 VEPSILON(0.00001f, 0.00001f, 0.00001f);
const float EPSILON = 0.00001f;
const Vector3 X_PLANE(1, 0, 0);
const Vector3 Y_PLANE(0, 1, 0);
const Vector3 Z_PLANE(0, 0, 1);
const Vector3 X_PLANE_BACK(-1, 0, 0);
const Vector3 Y_PLANE_BACK(0, -1, 0);
const Vector3 Z_PLANE_BACK(0, 0, -1);
const int MAX_CATCH_FISH_COUNT = 6;
#define CROSS_BULLET 4

Vector3 GetRotVec3(float angle);

const Vector3 DISK_RADIUS[]=
{
	Vector3(-0.5765095f, 0.4561417f, 0),
	Vector3(0.2875572f, 0.05799772f, 0),
	Vector3(-0.637768f, -0.5819039f, 0),
	Vector3(-0.8844957f, -0.002817995f, 0),
	Vector3(-0.2067623f, 0.9034358f, 0),
	Vector3(-0.3271674f, -0.1095892f, 0),
	Vector3(0.2348688f, -0.8478827f, 0),
	Vector3(0.6960751f, -0.4042498f, 0),
	Vector3(0.6137383f, 0.6096074f, 0),
	Vector3(0.936384f, 0.1196922f, 0),
	/*
	GetRotVec3(0),
	GetRotVec3(72),
	GetRotVec3(144),
	GetRotVec3(216),
	GetRotVec3(288),
	*/
};
const int DISK_RAD_COUNT = sizeof(DISK_RADIUS) / sizeof(DISK_RADIUS[0]);


Vector3 GetRotVec3(float angle)
{
	angle = D3DXToRadian(angle);
	float fs = sinf(angle);
	float fc = cosf(angle);
	Vector3 pos(fc, fs, 0);
	return pos;
}
/*
	   4              5
	  *-------------*
	 /|           1/|
   0*-------------* |
	| |7          | |
	| *-----------|-*6
	|/            |/
   3*-------------*2

*/
static DoublePoint g_BoxDoublePoint[12] =
{
	DoublePoint(0, 1),
	DoublePoint(1, 2),
	DoublePoint(2, 3),
	DoublePoint(3, 0),

	DoublePoint(4, 5),
	DoublePoint(5, 6),
	DoublePoint(6, 7),
	DoublePoint(7, 4),

	DoublePoint(0, 4),
	DoublePoint(7, 3),

	DoublePoint(1, 5),
	DoublePoint(2, 6),
};

bool IntersectPlane(const Ray &ray, const Vector3 &normal, const Vector3 &point, Vector3 &retPoint)
{
	float t = (Dot(normal, point) - Dot(normal, ray.Pos)) / Dot(normal, ray.Dir);
	if (t <= 0.f)
	{
		return false;
	}
	else
	{
		retPoint = ray.Pos + VEPSILON + ray.Dir * t;
		return true;
	}
}
bool	IsInside(const Vector3 &minPoint, const Vector3 &maxPoint, const Vector3 &point)
{
	return maxPoint >= point && point >= minPoint;
}
bool	IsInsideYZ(const Vector3 &minPoint, const Vector3 &maxPoint, const Vector3 &point)
{
	return maxPoint.y >= point.y && point.y >= minPoint.y &&
		maxPoint.z >= point.z && point.z >= minPoint.z;
}
bool	IsInsideXY(const Vector3 &minPoint, const Vector3 &maxPoint, const Vector3 &point)
{
	return maxPoint.y >= point.y && point.y >= minPoint.y &&
		maxPoint.x >= point.x && point.x >= minPoint.x;
}
bool	IsInsideXZ(const Vector3 &minPoint, const Vector3 &maxPoint, const Vector3 &point)
{
	return maxPoint.x >= point.x && point.x >= minPoint.x &&
		maxPoint.z >= point.z && point.z >= minPoint.z;
}
bool IntersectBox(const Vector3  &minPoint, const Vector3 &maxPoint, const Ray &ray, Vector3 &retPoint)
{
	if (IsInside(minPoint, maxPoint, ray.Pos))
		return true;

	if (Dot(ray.Dir, X_PLANE) < 0.f)
	{
		//与X垂直平面，确定交点是否在BOX内
		float t = (maxPoint.x - ray.Pos.x) / ray.Dir.x;
		if (t > 0)
		{
			retPoint = ray.Pos + ray.Dir * t;
			if (IsInsideYZ(minPoint, maxPoint, retPoint))
				return true;
		}
	}
	else
	{
		//与反面相交
		float t = (minPoint.x - ray.Pos.x) / ray.Dir.x;
		if (t > 0)
		{
			retPoint = ray.Pos + ray.Dir * t;
			if (IsInsideYZ(minPoint, maxPoint, retPoint))
				return true;
		}
	}

	if (Dot(ray.Dir, Y_PLANE) < 0.f)
	{
		//Y垂直平面有相交的可能
		float t = (maxPoint.y - ray.Pos.y) / ray.Dir.y;
		if (t > 0)
		{
			retPoint = ray.Pos + ray.Dir * t;
			if (IsInsideXZ(minPoint, maxPoint, retPoint))
				return true;
		}
	}
	else
	{
		float t = (minPoint.y - ray.Pos.y) / ray.Dir.y;
		if (t > 0)
		{
			retPoint = ray.Pos + ray.Dir * t;
			if (IsInsideXZ(minPoint, maxPoint, retPoint))
				return true;
		}
	}

	if (Dot(ray.Dir, Z_PLANE) < 0.f)
	{
		//Z垂直平面有相交的可能
		float t = (maxPoint.z - ray.Pos.z) / ray.Dir.z;
		if (t > 0)
		{
			retPoint = ray.Pos + ray.Dir * t;
			if (IsInsideXY(minPoint, maxPoint, retPoint))
				return true;
		}
	}
	else
	{
		float t = (minPoint.z - ray.Pos.z) / ray.Dir.z;
		if (t > 0)
		{
			retPoint = ray.Pos + ray.Dir * t;
			if (IsInsideXY(minPoint, maxPoint, retPoint))
				return true;
		}
	}

	return false;
}
bool Intersect(const Vector3 &pos, const Matrix &mat, const Vector3& boxSize, const Ray &ray, Vector3 &retPoint)
{
	//碰撞检测
	//对射线应用旋转，使OBB检测变成FishBox检测
	const int POINT_DIST = 10;
	Ray newRay;

	Vector3 frontPoint = ray.Pos + ray.Dir * POINT_DIST;
	D3DXVec3TransformCoord(&newRay.Pos, &ray.Pos, &mat);
	D3DXVec3TransformCoord(&frontPoint, &frontPoint, &mat);
	newRay.Dir = (frontPoint - newRay.Pos) / POINT_DIST;

	Vector3 minPoint = pos - boxSize  * 0.5f;
	Vector3 maxPoint = pos + boxSize  * 0.5f;
	return IntersectBox(minPoint, maxPoint, newRay, retPoint);
}
//计算线段与点的半径距离
bool CheckLineRadius( const LineData &ld, Fish *pFish)
{
	Vector2 castLine = pFish->ScreenPos - ld.StartPt;
	float castDist = D3DXVec2Dot(&ld.Dir, &castLine);

	Vector2 finalpt = castDist * ld.Dir + ld.StartPt;
	Vector2 finalDist = pFish->ScreenPos - finalpt;
	float centertolinedist = Vec2Length(finalDist);

	return ld.Radius + pFish->ScreenRadius > centertolinedist;
}
//计算点是否在线段的半径内
bool CollideLine2(const LineData &ld, const Vector2 &pt)
{
	Vector2 castLine = pt - ld.StartPt;
	float castDist = D3DXVec2Dot(&ld.Dir, &castLine);
	if (castDist < 0)
		return false;

	if (castDist > ld.Dist)
		return false;
	
	Vector2 finalpt = castDist * ld.Dir + ld.StartPt;
	Vector2 finalDist = pt - finalpt;

	float dist2 = Vec2LengthSq(finalDist);
	return dist2 < ld.RadiusSq;
}
bool LineIntersectSide(const Vector2 & A, const Vector2 & B, const Vector2 & C, const Vector2 & D)
{
	float fC = (C.y - A.y) * (A.x - B.x) - (C.x - A.x) * (A.y - B.y);
	float fD = (D.y - A.y) * (A.x - B.x) - (D.x - A.x) * (A.y - B.y);

	if (fC * fD > 0)
		return false;

	return true;
}

bool SideIntersectSide(const Vector2 &A, const Vector2 &B, const Vector2 &C, const Vector2 &D)
{
	if (!LineIntersectSide(A, B, C, D))
		return false;
	if (!LineIntersectSide(C, D, A, B))
		return false;

	return true;
}
//直线相交判断
bool CollideLine(const LineData &lineData, Fish *pFish)
{
	if (CheckLineRadius(lineData, pFish) == false)
		return false;

	//顶点是否在半径内
	for (int i = 0; i < 8; ++i)
	{
		if (CollideLine2(lineData, pFish->ScreenBoxPoint[i]))
			return true;
	}
	//判断线段是否相交
	for (int i = 0; i < 12; ++i)
	{
		if (SideIntersectSide(lineData.StartPt, lineData.EndPt, pFish->ScreenBoxPoint[g_BoxDoublePoint[i].a], pFish->ScreenBoxPoint[g_BoxDoublePoint[i].b]))
		{
			return true;
		}
	}
	return false;
}
bool CheckInRadius(float radius, const Vector2 center, Fish *pFish)
{
	float dist = Vec2Length(pFish->ScreenPos - center);
	return radius + pFish->ScreenRadius > dist;
}
//计算8个点是否在半径内
bool CollideRadius(float radius, float radius2, const Vector2 &scrCenter, Fish *pFish)
{
	if (CheckInRadius(radius, scrCenter, pFish) == false)
		return false;
	//判断点是否在半径内
	for (int i = 0; i < 8; ++i)
	{
		Vector2 dist = scrCenter - pFish->ScreenBoxPoint[i];
		float distLen = D3DXVec2LengthSq(&dist);
		if (distLen < radius2)
		{
			//InnerFishLog(L"捕获半径:%f, 设定的半径:%f",distLen, radius2);
			return true;
		}
	}
	//判断边是否在半径内
	for (int i = 0; i < 12; ++i)
	{
		const Vector2 & startpt = pFish->ScreenBoxPoint[g_BoxDoublePoint[i].a];
		const Vector2 & endpt = pFish->ScreenBoxPoint[g_BoxDoublePoint[i].b];
		LineData ld(startpt, endpt, radius, radius2);
		if (CollideLine2(ld, scrCenter))
			return true;
	}
	return false;
}
typedef HashMap<ushort, FishCatchData> FishCatchedMap;
typedef FishCatchedMap::iterator FishCatchedMapIt;
struct CatchData
{
	Bullet *pBullet;
	float	radius;
	float	catchRadius;
	UINT	maxCatch;	//最大碰撞数量。
	UINT	goldNum;	//获得的金币数量
	FishCatchedMap fishCatchedMap;
	IFishSetting *pSetting;
	bool isCollide;
	bool forceActionReduction;
	PlayerID playerID;
	FISH_DELAY_TYPE DelayType;
};
bool CheckCatchFish(Fish *pFish, CatchData &catchData, FishManager *pMgr, Bullet *pBullet)	//HasActionReduction :必须有动作才减速
{

	FishCatchData fc(pFish->FishID);
	bool bRet = false;
	if (!pFish->IsInFrustum || pFish->IsDead)
	{
		fc.eventType = CATCH_EVENT_EFFECT;
	}
	else
	{
		bool bValidReduction;
		if (catchData.forceActionReduction)
			bValidReduction = pFish->HasAttackAction;
		else
			bValidReduction = true;
		if (catchData.pBullet->CollideCount >= catchData.maxCatch)
		{
			if (bValidReduction && pFish->Controller.CheckDelayLevel(catchData.DelayType))
				fc.eventType = CATCH_EVENT_ATTACK;
			else
				fc.eventType = CATCH_EVENT_EFFECT;
		}
		else
		{
			float catchChance = catchData.pSetting->CatchChance(catchData.playerID, catchData.pBullet->BulletID, (byte)catchData.pBullet->BulletType, (BYTE)pFish->FishType, catchData.pBullet->CollideCount, catchData.maxCatch, pFish->PackageType, catchData.pBullet->LockFishID!=0);
			++catchData.pBullet->CollideCount;
			//计算概率
			float chance = RandFloat();
			if (chance < catchChance)
			{
				//通知外部		
				catchData.goldNum += catchData.pSetting->CatchFish(catchData.playerID, pFish->FishType, CATCH_BULLET, (BYTE)catchData.pBullet->BulletType, pFish->GetPackage(), catchData.pBullet->RateIndex);
				fc.eventType = CATCH_EVENT_CATCHED;
				fc.nReward = catchData.pSetting->FishRewardDrop(catchData.playerID,pFish->GetPackage(), pFish->FishType);
				bRet = true;
			}
			else
			{
				if (bValidReduction && pFish->Controller.CheckDelayLevel(catchData.DelayType))
					fc.eventType = CATCH_EVENT_ATTACK;
				else
					fc.eventType = CATCH_EVENT_EFFECT;
			}
		}
	}
	catchData.fishCatchedMap.insert(make_pair(pFish->FishID, fc));
	if (pBullet)
		pBullet->AddCollideFishID(pFish->FishID, bRet);
	return bRet;
}
void CatchFishByRay(const Ray& ray, FishVecList *pFishMapList, CatchData& catchData, FishManager *pMgr)
{
	Vector3 retPt;
	for (UINT i = 0; i < pFishMapList->size();)
	{
		Fish *pFish = pFishMapList->operator[](i);
		if (Intersect(pFish->WorldPos, pFish->InvWorldMat, pFish->FishBoxSize, ray, retPt))
		{
			catchData.isCollide = true;
			if (CheckCatchFish(pFish, catchData, pMgr, NULL))
			{
				ListRemoveAt(*pFishMapList, i);
				pMgr->RemoveFishImmediate(pFish->FishID);
			}
			return;
		}
		else
		{
			++i;
		}
	}
}
static float LITHING_DELAY_TIME[3] = { 0, 1, 0 };

int CatchFileLighting(FishManager *pMgr, FishVecList *pFishMapList, Fish *pCatchFish, CatchData &catchData, int num)
{
	for (UINT i = 0; i < pFishMapList->size() && num > 0;)
	{
		Fish *pFish = pFishMapList->operator[](i);
		float dist = Vec2Length(pCatchFish->ScreenPos - pFish->ScreenPos);
		if(catchData.pSetting->CanCatchLightingFish(pFish->FishType, catchData.playerID, dist) == false)
		{
			++i;
			continue;
		}
		FishCatchData fc(pFish->FishID);
		catchData.goldNum += catchData.pSetting->CatchFish(catchData.playerID, pFish->FishType, CATCH_BULLET, (BYTE)catchData.pBullet->BulletType, pFish->GetPackage(), catchData.pBullet->RateIndex);
		fc.eventType = CATCH_EVENT_CATCHED_LIGHTING;
		fc.nReward = catchData.pSetting->FishRewardDrop(catchData.playerID, pFish->GetPackage(), pFish->FishType);
		fc.lithingFishID = pCatchFish->FishID;
		catchData.fishCatchedMap[pFish->FishID] = fc;
		ListRemoveAt(*pFishMapList, i);
		pMgr->RemoveFish(pFish->FishID, 0, LITHING_DELAY_TIME);
		pFish->Controller.StopLaugh();
		--num;
	}
	return num;
}
void CatchFish(Bullet *pBullet, FishVecList *pFishMapList, CatchData &catchData, FishManager *pMgr)
{
	float radius = catchData.radius;
	float radius2 = radius * radius;
	//穿透才加collide
	Bullet *pCrossBullet = pBullet->BulletType == CROSS_BULLET ? pBullet : NULL;
	for (UINT i = 0; i < pFishMapList->size();)
	{
		Fish *pFish = pFishMapList->operator[](i);
		if (pCrossBullet == null && pBullet->LockFishID != 0 && pFish->FishID != pBullet->LockFishID)
		{
			++i;
			continue;
		}
		//在屏幕边缘也可以碰撞，只产生网
		if (/*pFish->IsInFrustum == false
			|| */catchData.fishCatchedMap.find(pFish->FishID) != catchData.fishCatchedMap.end()
			)
		{
			++i;
			continue;
		}
		if (pCrossBullet && pCrossBullet->CheckCollideFish(pFish->FishID))
		{
			++i;
			continue;
		}
		if (!CollideRadius(radius, radius2, pBullet->ScreenPos, pFish))
		{
			//再判断锁定的子弹和鱼是否在锁定半径内
			if (pBullet->LockFishID == pFish->FishID && Vec2Length(pBullet->ScreenPos - pFish->ScreenPos) < LOCK_COLLIDE_RADIUS)
			{
				//继续下面的执行
				//Log("在半径范围内, Bulet:%d, Radius:%f", pBullet->BulletID, Vec2Length(pBullet->ScreenPos - pFish->ScreenPos));
			}
			else
			{
				++i;
				continue;
			}
		}
		if (pCrossBullet && pBullet->LockFishID == pFish->FishID)
		{
			pBullet->ClearLockFishID();
		}
		//是否被捕获
		if (CheckCatchFish(pFish, catchData, pMgr, pCrossBullet))
		{
			ListRemoveAt(*pFishMapList, i);
			//是否是闪电鱼
			int num = catchData.pSetting->IsLightingFish(pFish->FishType, catchData.playerID);
			if (num > 0)
			{
				if (num != CatchFileLighting(pMgr, pFishMapList, pFish, catchData, num))
				{
					//发生闪电捕获，重新开始;
					i = 0;
				}
				catchData.fishCatchedMap[pFish->FishID].eventType = CATCH_EVENT_CATCHED_LIGHTING_MAIN;
				pMgr->RemoveFish(pFish->FishID, 0, LITHING_DELAY_TIME);
			}
			else
				pMgr->RemoveFishImmediate(pFish->FishID);
		}
		else
			++i;

		if (catchData.isCollide == false)
		{
			//第一次碰撞，展开捕获半径
			catchData.isCollide = true;
			//穿透子弹不展开。
			if (pCrossBullet == NULL)
			{
				radius = catchData.catchRadius;
				radius2 = radius * radius;
				i = 0;
				continue;
			}
		}
	}//end for 

	if (catchData.isCollide == false || pCrossBullet)
	{
		//是否和死亡的鱼发生了碰撞
		for (UINT i = 0; i < pMgr->GetDeadFishList().size(); ++i)
		{
			Fish *pFish = pMgr->GetDeadFishList()[i];
			if (!pFish)
			{
				ASSERT(false);
				break;
			}
			if (pCrossBullet && pCrossBullet->CheckCollideFish(pFish->FishID))
				continue;

			if (CollideRadius(radius, radius2, pBullet->ScreenPos, pFish))
			{
				CheckCatchFish(pFish, catchData, pMgr, pCrossBullet);
				catchData.isCollide = true;
				break;
			}
		}
	}
}
#include "../../Role.h"
#include "../../RoleEx.h"
void FishCollider::Collide(CTableRoleManager *pm, BulletManager *pBulletMgr, FishManager *pFishMgr, FishSendInterface *pSend)
{
	FishVecList *pFishMapList = pFishMgr->GetFishVecList();
	BulletVecList *pBulletMapList = pBulletMgr->GetBulletMapList();
	IFishSetting *pSetting = FishCallback::GetFishSetting();
	CatchData cd;


	for (UINT i = 0; i < pBulletMapList->size();)
	{
		Bullet *pBullet = pBulletMapList->operator[](i);
		UINT maxCatch = pSetting->MaxCatch(pBullet->BulletType);
		BulletType bt = pSetting->GetBulletType(pBullet->BulletType);
		maxCatch = maxCatch;// RandRange(1, maxCatch + 1);

		if (pBullet->BulletType == CROSS_BULLET)
		{
			if (pBullet->MaxCatchNum == 0)
				pBullet->MaxCatchNum = ConvertDWORDToBYTE(maxCatch);
			else
				maxCatch = pBullet->MaxCatchNum;
		}
		cd.pBullet = pBullet;
		cd.maxCatch = maxCatch;
		cd.pSetting = pSetting;
		cd.radius = pSetting->BulletRadius(pBullet->BulletType);
		cd.catchRadius = pSetting->BulletCatchRadius(pBullet->BulletType);
		cd.isCollide = false;
		cd.goldNum = 0;
		cd.fishCatchedMap.clear();
		cd.playerID = pBullet->Player;
		if (bt == BULLET_TYPE_FREEZE)
		{
			cd.DelayType = DELAY_BULLET;
			cd.forceActionReduction = false;
		}
		else
		{
			cd.DelayType = DELAY_ATTACK;
			cd.forceActionReduction = false;
		}
		CatchFish(pBullet, pFishMapList, cd, pFishMgr);
		if (cd.isCollide == false)
		{
			++i;
			continue;
		}
		//有碰撞发生
		if (bt == BULLET_TYPE_FREEZE)
		{
			//冰冻子弹
			int fishCount = cd.fishCatchedMap.size();
			UINT size = (fishCount - 1) * sizeof(FishCatched)+sizeof(NetCmdBulletReduction);
			CheckMsgSize(size);
			NetCmdBulletReduction *pCmd = (NetCmdBulletReduction*)malloc(size);
			if (!pCmd)
				return;

			pCmd->CmdSize = ConvertDWORDToWORD(size);
			pCmd->SetCmdType(CMD_BULLET_REDUCTION);
			pCmd->BulletID = pBullet->BulletID;
			pCmd->Combo = cd.goldNum==0?0:pSetting->GetCombo(cd.playerID, pBullet->BulletID);
			byte scal;
			byte d1, d2, d3;
			FishCallback::GetFishSetting()->GetBulletFreezeReduction(scal, d1, d2, d3);
			
			float fSpdScl;
			float fDurationList[3];
			ReductionToFloat(scal, d1, d2, d3, fSpdScl, fDurationList);
			FishCatchedMapIt it = cd.fishCatchedMap.begin();
			for (int j = 0; j < fishCount; ++it, ++j)
			{
				FishCatchData& fcd = it->second;
				pCmd->FishID[j].FishID = fcd.fishID;
				pCmd->FishID[j].CatchEvent = (byte)fcd.eventType;
				pCmd->FishID[j].nReward = fcd.nReward;
				pCmd->FishID[j].LightingFishID = fcd.lithingFishID;
				if (pCmd->FishID[j].CatchEvent == CATCH_EVENT_ATTACK)
				{
					Fish *pFindFish = pFishMgr->GetFish(fcd.fishID);
					if (pFindFish)
						pFindFish->Controller.AddSkillTimeScaling(fSpdScl, fDurationList, DELAY_BULLET);
				}
			}
			pCmd->GoldNum = cd.goldNum;
			pCmd->TotalNum = pm->GetRoleByUserID(pBullet->Player)->GetRoleExInfo()->GetRoleInfo().dwGlobeNum;
			pSend->SendAll(pCmd);
			::free(pCmd);
		}
		else
		{
			//普通子弹
			int fishCount = cd.fishCatchedMap.size();
			UINT size = (fishCount - 1) * sizeof(FishCatched)+sizeof(NetCmdCatched);
			CheckMsgSize(size);
			NetCmdCatched *pCmd = (NetCmdCatched*)malloc(size);
			if (!pCmd)
				return;
			pCmd->CmdSize = ConvertDWORDToWORD(size);
			pCmd->SetCmdType(CMD_CATCHED);
			pCmd->BulletID = pBullet->BulletID;
			pCmd->Combo = cd.goldNum == 0 ? 0 : pSetting->GetCombo(cd.playerID, pBullet->BulletID);
			FishCatchedMapIt it = cd.fishCatchedMap.begin();
			for (int j = 0; j < fishCount; ++j, ++it)
			{
				FishCatchData& fcd = it->second;
				pCmd->Fishs[j].FishID = fcd.fishID;
				pCmd->Fishs[j].CatchEvent = (byte)fcd.eventType;
				pCmd->Fishs[j].nReward = fcd.nReward;
				pCmd->Fishs[j].LightingFishID = fcd.lithingFishID;

				if (pCmd->Fishs[j].CatchEvent == CATCH_EVENT_ATTACK)
				{
					Fish *pFindFish = pFishMgr->GetFish(fcd.fishID);
					if (pFindFish)
					{
						pFindFish->Controller.AddSkillTimeScaling(0.5f, 0.5f, DELAY_ATTACK);
					}
				}
			}
			pCmd->GoldNum  = cd.goldNum;
			pCmd->TotalNum = pm->GetRoleByUserID(pBullet->Player)->GetRoleExInfo()->GetRoleInfo().dwGlobeNum;
			pSend->SendAll(pCmd);
			::free(pCmd);
		}
		if (pBullet->BulletType != CROSS_BULLET)
		{
			pSetting->BulletGain(cd.playerID, (BYTE)pBullet->BulletType, cd.goldNum);
			pBulletMgr->RemoveBullet(pBullet->BulletID);
		}
		else
		{
			pBullet->CatchGold += cd.goldNum;
		}

		ListRemoveAt(*pBulletMapList, i);
	}
}

void FishCollider::CollideByDir(vector<FishInfo> &fishList, byte laserType, NetCmdSkillLaser* pCmd, const Vector3 &pos, const Vector3 &dir, FishManager *pFishMgr)
{
	FishVecList *pFishMapList = pFishMgr->GetFishVecList();
	IFishSetting *pSetting = FishCallback::GetFishSetting();
	float radius = pSetting->GetLaserRadius(laserType);

	Vector3 endpos = pos + dir * 40;
	
	int minNum, maxNum;
	pSetting->GetLaserRange(laserType, minNum, maxNum);
	int fishNum = RandRange(minNum, maxNum);
	USHORT goldNum = 0;
	Vector2 startpt = WorldToViewport(pos);
	Vector2 endpt = WorldToViewport(endpos);
	LineData ld(startpt,endpt , radius, radius * radius);
	FishInfo fi;
	for (UINT i = 0; i < pFishMapList->size();)
	{
		Fish *pFish = pFishMapList->operator[](i);
		if (FishCallback::GetFishSetting()->IsLightingFish(pFish->FishType) || pFish->IsInFrustum == false)
		{
			++i;
			continue;
		}
		if (CollideLine(ld, pFish))
		{
			if (RandFloat() < pSetting->GetLaserChance(laserType, pFish->FishType, pFish->PackageType))
			{
				fi.pFish = pFish;
				fishList.push_back(fi);
				ListRemoveAt(*pFishMapList, i);
				if (fishList.size() >= (UINT)fishNum)
					break;
				else
					continue;
			}
		}
		++i;
	}
	return;
	
}
