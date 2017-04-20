#pragma once
#include "FishResData.h"
#include "RuntimeInfo.h"
#include "TArray.h"
#include "FishManager.h"
class Bullet
{
public:
	Bullet()
	{
		ReboundCount = 0;
		LockFishID = 0;
	}
	bool UpdateLockFish(FishManager *pmgr)
	{
		Fish *pfish = pmgr->GetFish(LockFishID);
		if (pfish == NULL)
			return false;

		Vector2 dir = pfish->ScreenPos - ScreenPos;
		float dirlen = Vec2Length(dir);
		dir /= dirlen;
		Dir.x = dir.x;
		Dir.y = dir.y;
		return true;
	}
	bool Update(FishManager *pmgr, float deltaTime)
	{
		if (LockFishID != 0)
		{
			if (!UpdateLockFish(pmgr))
			{
				/*if (this->BulletType != 4)
					return false;
				else*/
				ClearLockFishID();
			}
		}
		Time += deltaTime;
		Pos += Dir * deltaTime * Speed;
		Vector3 p = WorldToProjection(Pos);
		ScreenPos = ProjectionToViewport(p);
		return CheckBoundary();
		
	}
	bool CheckBoundary()
	{
		//裁剪掉屏幕外的子弹
		if (Pos.x > RuntimeInfo::NearRightBottomPoint.x || Pos.x < RuntimeInfo::NearLeftTopPoint.x ||
			Pos.y > RuntimeInfo::NearLeftTopPoint.y || Pos.y < RuntimeInfo::NearRightBottomPoint.y)
		{
			if (ReboundCount > 0)
			{
				Vector2 dir = Dir;
				if (Pos.x > RuntimeInfo::NearRightBottomPoint.x)
				{
					Pos.x = RuntimeInfo::NearRightBottomPoint.x;
					Dir.x = -Dir.x;
				}
				else if (Pos.x < RuntimeInfo::NearLeftTopPoint.x)
				{
					Pos.x = RuntimeInfo::NearLeftTopPoint.x;
					Dir.x = -Dir.x;
				}
				if (Pos.y > RuntimeInfo::NearLeftTopPoint.y)
				{
					Pos.y = RuntimeInfo::NearLeftTopPoint.y;
					Dir.y = -Dir.y;

				}
				else if (Pos.y < RuntimeInfo::NearRightBottomPoint.y)
				{
					Pos.y = RuntimeInfo::NearRightBottomPoint.y;
					Dir.y = -Dir.y;
				}
				StartPos = Pos;
				Time = 0;
				--ReboundCount;
				return true;
			}
			else
				return false;
		}
		return true;
	} 
	void AddCollideFishID(USHORT id, bool bCatched)
	{
		if(CheckCollideFish(id))
		{
			InnerFishLog(L"存在相同的碰撞鱼ID:%d", id);
			return;
		}
		CatchedNum += bCatched;
		CollideFishList.insert(make_pair(id, bCatched));
	}
	bool CheckCollideFish(USHORT id)
	{
		return CollideFishList.find(id) != CollideFishList.end();
	}
	void ClearLockFishID()
	{
		LockFishID = 0;
		Dir = OrgDir;
	}
	HashMap<USHORT, byte> CollideFishList;
	
	Vector3		OrgDir;
	USHORT		LockFishID;
	BYTE		ReboundCount;	//反弹次数
	BYTE		CollideCount;	//已碰撞次数
	BYTE		CatchedNum;		//捕获数量
	BYTE		MaxCatchNum;	//最大碰撞次数
	PlayerID	Player;			//子弹所属玩家ID;
	USHORT		BulletID;		//子弹的ID
	USHORT		BulletType;		//子弹的类型
	Vector2		ScreenPos;
	Vector3		Dir;			//运行方向
	Vector3		StartPos;		//起始位置
	Vector3		Pos;
	float		Speed;			//速度
	float		Time;			//运行时间
	short		Degree;
	byte		RateIndex;
	UINT		CatchGold;

};