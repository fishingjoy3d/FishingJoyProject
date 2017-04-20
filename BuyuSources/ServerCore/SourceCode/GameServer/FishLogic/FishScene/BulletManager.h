#pragma once
#include "Bullet.h"
#include "TArray.h"
#include "../FishCallbak.h"
typedef HashMap<byte, Bullet*>	BulletMapList;	//当前玩家的子弹列表
typedef BulletMapList::iterator BulletMapIt;
typedef vector<Bullet*> BulletVecList;
class BulletManager
{
public:
	bool Init()
	{
		return true;
	}
	void Shutdown()
	{
		ClearAllBullet();
	}
	void ClearAllBullet()
	{
		for (int i = 0; i < MAX_SEAT_NUM; ++i)
		for (BulletMapIt it = m_BulletList[i].begin(); it != m_BulletList[i].end();)
		{
			delete it->second;
			it = m_BulletList[i].erase(it);
		}
	}

	static Vector3 GetBulletDir(short x, short y)
	{
		return Vector3(x * InvShortMaxValue, y * InvShortMaxValue, 0);
	}
	static Vector3 GetBulletStartPos(short x, short y)
	{
		Vector3 pos;
		pos.x = (x * InvShortMaxValue) * RuntimeInfo::NearRightBottomPoint.x;
		pos.y = (y * InvShortMaxValue) * RuntimeInfo::NearLeftTopPoint.y;
		pos.z = RuntimeInfo::NearLeftTopPoint.z;
		return pos;
	}
	static void BulletPosToShort(const Vector3& pos , short &x, short &y)
	{
		float a = pos.x / RuntimeInfo::NearRightBottomPoint.x;
		float b = pos.y / RuntimeInfo::NearLeftTopPoint.y;
		a = Clamp(a, -1.0f, 1.0f);
		b = Clamp(b, -1.0f, 1.0f);
		x = short(a * SHRT_MAX);
		y = short(b * SHRT_MAX);
	}
	static void BulletDirToShort(const Vector3& dir, short &x, short &y)
	{
		x = short(dir.x  * SHRT_MAX);
		y = short(dir.y  * SHRT_MAX);
	}
	void Update(FishManager *pmgr, float delta)
	{
		m_BulletVecList.clear();
		for (int i = 0; i < MAX_SEAT_NUM; ++i)
		for (BulletMapIt it = m_BulletList[i].begin(); it != m_BulletList[i].end();)
		{
			Bullet *pBullet = it->second;
			if (!pBullet->Update(pmgr, delta))
			{
				USHORT id = pBullet->BulletID;
				FishCallback::GetFishSetting()->BulletGain(pBullet->Player, (BYTE)pBullet->BulletType, pBullet->CatchGold);
				delete pBullet;
				it = m_BulletList[i].erase(it);
			}
			else
			{
				m_BulletVecList.push_back(it->second);
				++it;
			}
		}
	}
	void RemoveBySeat(byte seat)
	{
		for (BulletMapIt it = m_BulletList[seat].begin(); it != m_BulletList[seat].end();)
		{
			delete it->second;
			it = m_BulletList[seat].erase(it);
		}
	}
	bool AddBullet(Bullet *pBullet)
	{
		BYTE seat = GetSeat(pBullet);
		BYTE idx = GetIdx(pBullet);
		if (idx >= MAX_BULLET_NUM)
		{
			InnerFishLog(L"子弹的索引不正确:%d", idx);
			return false;
		}
		if(!m_BulletList[seat].insert(make_pair(idx, pBullet)).second)
		{
			InnerFishLog(L"子弹添加失败，存在相同的ID:%d", pBullet->BulletID);
			return false;
		}
		else
		{
			return true;
		}
	}
	bool FindByID(USHORT bulletID)
	{
		BYTE seat = GetSeat(bulletID);
		BYTE idx = GetIdx(bulletID);
		return m_BulletList[seat].find(idx) != m_BulletList[seat].end();
	}
	Bullet *FindBullet(USHORT bulletID)
	{
		BYTE seat = GetSeat(bulletID);
		BYTE idx = GetIdx(bulletID);
		BulletMapIt it = m_BulletList[seat].find(idx);
		if (it != m_BulletList[seat].end())
			return it->second;
		else
			return NULL;
	}
	void RemoveBullet(USHORT bullet)
	{
		byte seat = GetSeat(bullet);
		BulletMapList & bml = m_BulletList[seat];
		BulletMapIt it = bml.find(GetIdx(bullet));
		if (it == bml.end())
		{
			InnerFishLog(L"子弹移除失败, Seat:%d, Idx:%d" + GetSeat(bullet), GetIdx(bullet));
		}
		else
		{
			delete it->second;
			bml.erase(it);
		}
	}
	bool Launch(UINT playerID, USHORT bulletID, USHORT bulletType, byte rateIndex, float speed, short degree, const Vector3 &dir, const Vector3 &startPos, BYTE ReboundCount = 0, USHORT lockFishID = 0)
	{

		Bullet *pBullet = new Bullet;
		pBullet->CollideCount = 0;
		pBullet->CatchedNum = 0;
		pBullet->MaxCatchNum = 0;
		pBullet->CatchGold = 0;
		pBullet->BulletID = bulletID;
		pBullet->Player = playerID; 
		pBullet->BulletType = bulletType;
		pBullet->RateIndex = rateIndex;
		pBullet->Dir = dir;
		pBullet->Pos = pBullet->StartPos = startPos;
		pBullet->Degree = degree;
		pBullet->Speed = speed;
		pBullet->Time = 0;
		pBullet->ReboundCount = ReboundCount;
		pBullet->LockFishID = lockFishID;
		pBullet->OrgDir = pBullet->Dir;
		bool bRet = AddBullet(pBullet);
		if (!bRet)
			delete pBullet;
		return bRet;
	}
	BulletVecList* GetBulletMapList()
	{
		return &m_BulletVecList;
	}
	BYTE GetIdx(USHORT BulletID)
	{
		return BulletID & 0xff;
	}
	BYTE GetIdx(Bullet *pBullet)
	{
		return pBullet->BulletID & 0xff;
	}
	BYTE GetSeat(Bullet *pBullet)
	{
		return pBullet->BulletID >> 8;
	}
	BYTE GetSeat(USHORT bulletID)
	{
		return bulletID >> 8;
	}
protected:
	BulletMapList m_BulletList[MAX_SEAT_NUM];		//当前的子弹，ID对应
	BulletVecList m_BulletVecList;
};