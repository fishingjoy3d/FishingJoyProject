#pragma once
#include "Fish.h"
#include "TArray.h"
typedef HashMap<USHORT, Fish*> FishMapList;
typedef FishMapList::iterator FishMapIt;
typedef vector<Fish*> FishVecList;
class FishManager
{
public:
	bool Init()
	{
		return true;
	}
	void Shutdown()
	{
		ClearAllFish(true);
	}
	void Update(float delta)
	{
		m_FishVecList.clear();
		for (FishMapIt it = m_FishMapList.begin(); it != m_FishMapList.end();)
		{
			Fish *pFish = it->second;
			if (!pFish->UpdateFish(delta))
			{
				delete pFish;
				it = m_FishMapList.erase(it);
			}
			else
			{
				if (pFish->IsActive && pFish->IsValidTime() && pFish->FishType != 24)
					m_FishVecList.push_back(pFish);
				++it;
			}
		}
		//死亡列表更新
		for (UINT i = 0; i < m_DeadFishList.size();)
		{
			Fish *pFish = m_DeadFishList[i];
			if (pFish->UpdateDeadTime(delta))
			{
				ListRemoveAt(m_DeadFishList, i);
				delete(pFish);
			}
			else
			{
				++i;
			}
		}
	}
	
	bool AddFish(Fish *pFish)
	{
		if (m_FishMapList.insert(make_pair(pFish->FishID, pFish)).second)
		{
			return true;
		}
		else
		{
			MessageBoxA(0, "添加鱼失败，存在相同的ID!", 0,0);
			//InnerFishLog(L"添加鱼失败，存在相同的ID:%d", pFish->FishID);
			return false;
		}
	}
	void RemoveFish(FISHID FishID, float speedScaling, float stayTime[])
	{
		FishMapIt it = m_FishMapList.find(FishID);
		if (it != m_FishMapList.end())
		{
			Fish *pFish = it->second;
			pFish->Controller.AddSkillTimeScaling(speedScaling, stayTime, DELAY_SKILL);
			pFish->SetDead(pFish->Controller.GetDelayData().AllTime);
			m_DeadFishList.push_back(pFish);
			m_FishMapList.erase(it);
		}
	}
	void RemoveFishImmediate(FISHID FishID)
	{
		FishMapIt it = m_FishMapList.find(FishID);
		if (it != m_FishMapList.end())
		{
			delete it->second;
			m_FishMapList.erase(it);
		}
	}
	void ClearAllFish(bool bReset)
	{
		for (UINT i = 0; i < m_DeadFishList.size(); ++i)
			delete(m_DeadFishList[i]);

		for (FishMapIt it = m_FishMapList.begin(); it != m_FishMapList.end(); )
		{
			delete(it->second);
			it = m_FishMapList.erase(it);
		}
		m_DeadFishList.clear();
		m_FishVecList.clear();
		
	}
	UINT GetFishCount()
	{
		return m_FishMapList.size() + m_DeadFishList.size();
	}
	Fish* GetFish(USHORT id)
	{
		FishMapIt it = m_FishMapList.find(id);
		if (it != m_FishMapList.end())
			return it->second;
		else
			return NULL;
	}
	//随机位置。
	void RandVecList()
	{
		UINT count = m_FishVecList.size();
		for (UINT i = 0; i < count; ++i)
		{
			int idx = RandRange(0, count);
			Fish *pt = m_FishVecList[i];
			m_FishVecList[i] = m_FishVecList[idx];
			m_FishVecList[idx] = pt;
		}
	}
	FishVecList* GetFishVecList()
	{
		return &m_FishVecList;
	}
	FishMapList* GetFishMapList()
	{
		return &m_FishMapList;
	}
	INT FishCount()const
	{
		return m_FishMapList.size();
	}
	vector<Fish*>& GetDeadFishList()
	{
		return m_DeadFishList;
	}
protected:
	FishMapList	m_FishMapList;
	FishVecList	m_DeadFishList;
	FishVecList	m_FishVecList;
};

