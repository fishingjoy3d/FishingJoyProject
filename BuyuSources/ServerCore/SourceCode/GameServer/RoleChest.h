//玩家的宝箱控制类
#pragma once
#include "Stdafx.h"
#include <set>
class CRoleEx;
struct tagChestInfo  //单个宝箱的结
{
	BYTE		ChestOnlyID;
	BYTE		OpenSum;//已经开启的次数
	DWORD		ActionTime;//激活的时间
	DWORD		ActionStates;//活动的状态
	DWORD		ActionOpenStates;//开启的状态
	std::vector<ChestOnceStates> ChestArray;
	tagChestConfig ChestConfig;
	tagChestInfo(BYTE ChestOnlyID, BYTE ChestTypeID, tagChestConfig& pConfig)
	{
		this->ChestOnlyID = ChestOnlyID;
		ChestConfig = pConfig;
		OpenSum = 0;
		ActionTime = 0;
		ActionStates = 0;
		ActionOpenStates = 0;
		ChestArray.clear();
	}
	BYTE GetChestTypeID()
	{
		return ChestConfig.ChestID;
	}
	bool IsAction()
	{
		return (ActionTime != 0);
	}

	void OnAction(CRoleEx* pRole, BYTE ChestSum);
	bool OnEndChest(CRoleEx* pRole);
	
	BYTE GetRankTypeID(BYTE RPValue)
	{
		if (OpenSum >= ChestConfig.CostInfo.MaxCostNum)
		{
			ASSERT(false);
			return 0;
		}
		//获取一个奖励类型ID 从配置文件中读取 读取的时候不删除
		if (RPValue >= ChestConfig.RewardInfo.SpecialRewardUseRp && ChestConfig.RewardInfo.RewardMap.count(ChestConfig.RewardInfo.SpecialRewardTypeID) == 1)
		{
			return ChestConfig.RewardInfo.SpecialRewardTypeID;
		}
		else
		{
			DWORD RankValue = RandUInt() % ChestConfig.RewardInfo.MaxRewardChances;//最大的数值
			DWORD AddValue = 0;

			HashMap<BYTE, tagChestsReward>::iterator Iter = ChestConfig.RewardInfo.RewardMap.begin();
			for (; Iter != ChestConfig.RewardInfo.RewardMap.end(); ++Iter)
			{
				if (RankValue <= (Iter->second.Chances + AddValue))
				{
					return Iter->second.RewardID;
				}
				else
				{
					AddValue += Iter->second.Chances;
				}
			}
			ASSERT(false);
			return 0;
		}
	}
	void OnAddOpenID(BYTE TypeID, BYTE Index, BYTE RewardOnlyID)
	{
		ActionStates |= TypeID;
		ActionOpenStates |= (1<<Index);

		ChestOnceStates pOnce;
		pOnce.Index = Index;
		pOnce.RewardID = TypeID;
		pOnce.RewardOnlyID = RewardOnlyID;
		ChestArray.push_back(pOnce);

		OpenSum += 1;

		//因为获得指定的奖励 我们进行处理 将奖励从Map里面删除掉
		HashMap<BYTE, tagChestsReward>::iterator Iter = ChestConfig.RewardInfo.RewardMap.find(TypeID);
		if (Iter != ChestConfig.RewardInfo.RewardMap.end())
		{
			ChestConfig.RewardInfo.MaxRewardChances -= Iter->second.Chances;//扣除权重
			ChestConfig.RewardInfo.RewardMap.erase(Iter);
		}
		else
		{
			ASSERT(false); 
		}
	}
	bool IsCanUseTypeID(BYTE TypeID)
	{
		return (ActionStates & TypeID) == 0;
	}
	bool IsCanUseIndex(BYTE Index)
	{
		return (ActionOpenStates & (1<<Index)) == 0;
	}
};
class CRole;
class RoleChestManager  //玩家身上的宝箱管理类 用于管理玩家在游戏过程中的宝箱
{
public:
	RoleChestManager();
	virtual ~RoleChestManager();

	void	AddChest(BYTE ChestID);//添加一个宝箱
	void	OpenChest(BYTE ChestOnlyID,BYTE ChestIndex);//开启一个宝箱 最新的
	void    Update(DWORD dwTimer);
	void	CloseChest(BYTE ChestOnlyID);

	void	OnInit(CRole* pRole);
	void    Clear();

	//void	OnRoleAFK();

	void    SendAllChestToClient();
private:
	void	ActionChest();//激活一个最前面的宝箱
	void	ChestClose();//最新的宝箱已经到期了
	//BYTE	GetRankTypeID();
private:
	CRole*			m_pRole;		//宝箱属于的玩家
	BYTE			m_RpValue;		//玩家开启宝箱的Rp值
	BYTE			m_BeginChestID;
	std::list<tagChestInfo>  m_ChestList;//宝箱的集合
};