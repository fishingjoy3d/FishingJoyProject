#include "Stdafx.h"
#include "RoleChest.h"
#include "Role.h"
#include "FishServer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
RoleChestManager::RoleChestManager()
{
	m_ChestList.clear();//空的宝箱集合
	m_pRole = NULL;
	m_RpValue = 0;
	m_BeginChestID = 0;
}
RoleChestManager::~RoleChestManager()
{

}
void RoleChestManager::AddChest(BYTE ChestID)
{
	if (!m_pRole || !m_pRole->IsActionUser())
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, tagChestConfig>::iterator Iter= g_FishServer.GetFishConfig().GetChestConfig().ChestMap.find(ChestID);
	if (Iter == g_FishServer.GetFishConfig().GetChestConfig().ChestMap.end())
	{
		ASSERT(false);
		return;
	}
	if ((*Iter).second.ImmediateRewardid != 0)
	{		
		m_pRole->GetRoleExInfo()->OnAddRoleRewardByRewardID((*Iter).second.ImmediateRewardid, TEXT("卡片奖励"));
		return;
	}
	++m_BeginChestID;
	//获取到宝箱的ID后 我们加入到集合里面去
	tagChestInfo pChest(m_BeginChestID, ChestID, Iter->second);//生成一个新的宝箱 ID 和 类型
	m_ChestList.push_back(pChest);
	//判断是否需要激活宝箱
	if (m_ChestList.size() == 1)
	{
		ActionChest();
	}
}
void RoleChestManager::ActionChest()//激活最前面的一个宝箱
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (m_ChestList.empty())
		return;
	if (!m_pRole->IsActionUser())
	{
		ASSERT(false);
		return;
	}
	if (m_ChestList.front().IsAction())//已经激活的宝箱无法再次激活
	{
		ASSERT(false);
		return;
	}
	//正式激活最头上的宝箱
	m_ChestList.front().OnAction(m_pRole->GetRoleExInfo(), ConvertDWORDToBYTE(m_ChestList.size()));//激活宝箱
}
void RoleChestManager::ChestClose()
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (m_ChestList.empty())
	{
		ASSERT(false);
		return;
	}
	if (!m_pRole->IsActionUser())
	{
		ASSERT(false);
		return;
	}
	if (m_ChestList.front().OnEndChest(m_pRole->GetRoleExInfo()))
	{
		m_ChestList.pop_front();
		if (!m_ChestList.empty())//重新激活一个宝箱
		{
			ActionChest();
		}
	}
	else
	{
		ASSERT(false);
		return;
	}
}
void RoleChestManager::OpenChest(BYTE ChestOnlyID, BYTE ChestIndex)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (m_ChestList.empty())
	{
		ASSERT(false);
		return;
	}
	if (m_ChestList.front().ChestOnlyID != ChestOnlyID)
	{
		ASSERT(false);
		return;
	}
	if (!m_ChestList.front().IsAction())
	{
		ASSERT(false);
		return;
	}
	if (!m_pRole->IsActionUser())
	{
		ASSERT(false);
		return;
	}
	LC_Cmd_GetChestReward msg;
	SetMsgInfo(msg,GetMsgType(Main_Chest, LC_GetChestReward), sizeof(LC_Cmd_GetChestReward));
	msg.ChestOnlyID = ChestOnlyID;
	msg.ChestStates.Index = ChestIndex;
	msg.ChestStates.RewardID = 0;
	msg.ChestStates.RewardOnlyID = 0;
	msg.Result = false;
	//玩家开启宝箱
	if (!m_ChestList.front().IsCanUseIndex(ChestIndex))
	{
		ASSERT(false);
		m_pRole->GetRoleExInfo()->SendDataToClient(&msg);
		return;
	}
	if (m_ChestList.front().OpenSum >= m_ChestList.front().ChestConfig.CostInfo.MaxCostNum)
	{
		ASSERT(false);
		m_pRole->GetRoleExInfo()->SendDataToClient(&msg);
		return;
	}
	//1.判断当前的花费
	HashMap<BYTE, DWORD>::iterator IterCost = m_ChestList.front().ChestConfig.CostInfo.CostMap.find(m_ChestList.front().OpenSum + 1);
	if (IterCost == m_ChestList.front().ChestConfig.CostInfo.CostMap.end())
	{
		ASSERT(false);
		m_pRole->GetRoleExInfo()->SendDataToClient(&msg);
		return;
	}
	DWORD UserCurrcey= IterCost->second;
	//2.扣除玩家的金钱
	if (!m_pRole->GetRoleExInfo()->ChangeRoleCurrency(UserCurrcey*-1,TEXT("开启鱼群宝箱 扣子乐币")))
	{
		//ASSERT(false);
		m_pRole->GetRoleExInfo()->SendDataToClient(&msg);
		return;
	}
	////3.添加次数
	//m_ChestList.front().OpenSum += 1;
	//4.进行抽奖 4个奖励不可以重复
	BYTE RewardTypeID = m_ChestList.front().GetRankTypeID(m_RpValue);//获取随机的数据
	if (RewardTypeID == 0)
	{
		m_pRole->GetRoleExInfo()->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	//获取到奖励ID后进行处理
	HashMap<BYTE, tagChestsReward>::iterator IterReward = m_ChestList.front().ChestConfig.RewardInfo.RewardMap.find(RewardTypeID);
	if (IterReward == m_ChestList.front().ChestConfig.RewardInfo.RewardMap.end())
	{
		m_pRole->GetRoleExInfo()->SendDataToClient(&msg);
		ASSERT(false);
		return;
	}
	//获得特殊奖励后 读取奖励的内容
	bool IsSendReward = false;
	DWORD RandValue = RandUInt() % IterReward->second.MaxOnceChances;
	std::vector<tagChestsRewardOnce>::iterator IterOnce = IterReward->second.RewardMap.begin();
	for (; IterOnce != IterReward->second.RewardMap.end(); ++IterOnce)
	{
		if (RandValue <= IterOnce->Chances)
		{
			//处理RP值
			if (RewardTypeID == m_ChestList.front().ChestConfig.RewardInfo.SpecialRewardTypeID)
			{
				if (m_RpValue >= m_ChestList.front().ChestConfig.RewardInfo.SpecialRewardUseRp)
					m_RpValue -= m_ChestList.front().ChestConfig.RewardInfo.SpecialRewardUseRp;
				else
					m_RpValue = 0;
			}
			else
			{
				m_RpValue += m_ChestList.front().ChestConfig.RewardInfo.OtherAddRp;
			}
			m_pRole->GetRoleExInfo()->OnAddRoleRewardByRewardID(IterOnce->RewardID,TEXT("领取鱼群宝箱奖励"));
			//发送命令到客户端 玩家获得宝箱的奖励 了
			msg.Result = true;
			msg.ChestStates.RewardID = RewardTypeID;
			msg.ChestStates.RewardOnlyID = IterOnce->OnceID;
			m_pRole->GetRoleExInfo()->SendDataToClient(&msg);

			m_ChestList.front().OnAddOpenID(RewardTypeID, ChestIndex, IterOnce->OnceID);//

			IsSendReward = true;
			break;
		}
	}
	if (!IsSendReward)
	{
		ASSERT(false);
		return;
	}
	//奖励获得完毕 我们想要进行继续处理
	if (m_ChestList.front().OpenSum >= m_ChestList.front().ChestConfig.CostInfo.MaxCostNum)//需要结束了
	{
		//当前宝箱已经使用完毕了 无法继续使用了 可以自动关闭了
		ChestClose();
	}
}
void RoleChestManager::Update(DWORD dwTimer)
{
	//用于便利玩家身上的宝箱
	if (!m_pRole || m_ChestList.empty() || !m_pRole->IsActionUser())
		return;
	//玩家开启宝箱
	if ((dwTimer - m_ChestList.front().ActionTime) >= (DWORD)(m_ChestList.front().ChestConfig.ExisteSec * 1000))
	{
		//表示当前宝箱过期了
		if (m_ChestList.front().OpenSum == 0)
		{
			//开启一个
			OpenChest(m_ChestList.front().ChestOnlyID, 0);//在宝箱关闭的时候 如果发现宝箱为开启 就自动帮忙开启一个
		}
		ChestClose();//关闭最前面激活的宝箱
	}
}
void RoleChestManager::CloseChest(BYTE ChestOnlyID)
{
	//关闭当前激活的宝箱
	//玩家不想继续处理了
	if (m_ChestList.empty())
	{
		ASSERT(false);
		return;
	}
	if (m_ChestList.front().ChestOnlyID != ChestOnlyID)
	{
		ASSERT(false);
		return;
	}
	ChestClose();
}
void RoleChestManager::OnInit(CRole* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	m_pRole = pRole;
}
void RoleChestManager::SendAllChestToClient()
{
	if (!m_pRole || !m_pRole->GetRoleExInfo())
	{
		ASSERT(false);
		return;
	}
	LC_Cmd_ResetChest msg;
	SetMsgInfo(msg, GetMsgType(Main_Chest, LC_ResetChest), sizeof(LC_Cmd_ResetChest));
	m_pRole->GetRoleExInfo()->SendDataToClient(&msg);
	if (m_ChestList.empty())
	{
		return;
	}
	HashMap<BYTE, tagChestConfig>::iterator Iter = g_FishServer.GetFishConfig().GetChestConfig().ChestMap.find(m_ChestList.front().GetChestTypeID());
	if (Iter == g_FishServer.GetFishConfig().GetChestConfig().ChestMap.end())
	{
		ASSERT(false);
		return;
	}
	//发送激活命令
	DWORD PageSize = sizeof(LC_Cmd_ActionChest)+sizeof(ChestOnceStates)*(m_ChestList.front().ChestArray.size() - 1);
	DWORD UserTime = timeGetTime() - m_ChestList.front().ActionTime;
	if (static_cast<DWORD>(Iter->second.ExisteSec * 1000) <= PageSize)
	{
		ASSERT(false);
		return;
	}
	DWORD EndSec = Iter->second.ExisteSec * 1000 - UserTime;
	LC_Cmd_ActionChest* msgAction = (LC_Cmd_ActionChest*)malloc(PageSize);
	if (!msgAction)
	{
		ASSERT(false);
		return;
	}
	CheckMsgSize(PageSize);
	msgAction->SetCmdSize(ConvertDWORDToWORD(PageSize));
	msgAction->SetCmdType(GetMsgType(Main_Chest, LC_ActionChest));
	msgAction->ChestOnlyID = m_ChestList.front().ChestOnlyID;
	msgAction->ChestTypeID = m_ChestList.front().GetChestTypeID();
	msgAction->EndSec = EndSec;
	msgAction->IsReset = true;
	//msgAction->ActionOpenStates = m_ChestList.front().ActionOpenStates;
	//msgAction->ActionRewardStates = m_ChestList.front().ActionStates;
	msgAction->ChestSum = ConvertDWORDToBYTE(m_ChestList.size());
	std::vector<ChestOnceStates>::iterator IterVec = m_ChestList.front().ChestArray.begin();
	for (int i = 0; IterVec != m_ChestList.front().ChestArray.end(); ++i, ++IterVec)
	{
		msgAction->ChestArray[i] = *IterVec;
	}
	msgAction->OpenSum = m_ChestList.front().OpenSum;
	m_pRole->GetRoleExInfo()->SendDataToClient(msgAction);
	free(msgAction);
}
void RoleChestManager::Clear()
{
	//清理掉当前的宝箱 玩家离开的时候 将宝箱管理器全部清理掉
	m_RpValue = 0;
	m_ChestList.clear();
}


void tagChestInfo::OnAction(CRoleEx* pRole, BYTE ChestSum)
{
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	if (IsAction())
	{
		ASSERT(false);
		return;
	}
	//激活当前宝箱
	OpenSum = 0;
	ActionTime = timeGetTime();//设置激活的时间
	//将已经激活的宝箱的数据发送到客户端去
	DWORD PageSize = sizeof(LC_Cmd_ActionChest)+sizeof(ChestOnceStates)*(ChestArray.size() - 1);
	LC_Cmd_ActionChest* msg = (LC_Cmd_ActionChest*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	CheckMsgSize(PageSize);
	msg->SetCmdSize(ConvertDWORDToWORD(PageSize));
	msg->SetCmdType(GetMsgType(Main_Chest, LC_ActionChest));
	msg->IsReset = false;
	msg->ChestOnlyID = ChestOnlyID;
	msg->ChestTypeID = GetChestTypeID();
	msg->EndSec = ChestConfig.ExisteSec * 1000;
	/*msg->ActionOpenStates = m_ChestList.front().ActionOpenStates;
	msg->ActionRewardStates = m_ChestList.front().ActionStates;*/
	msg->ChestSum = ChestSum;
	std::vector<ChestOnceStates>::iterator IterVec = ChestArray.begin();
	for (int i = 0; IterVec != ChestArray.end(); ++i, ++IterVec)
	{
		msg->ChestArray[i] = *IterVec;
	}
	msg->OpenSum = OpenSum;
	pRole->SendDataToClient(msg);
	free(msg);
}
bool tagChestInfo::OnEndChest(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	//关闭掉当前这个已经激活的宝箱
	if (!IsAction())
	{
		ASSERT(false);
		return false;//未激活的宝箱无法开启
	}
	LC_Cmd_ChestEnd msg;
	SetMsgInfo(msg, GetMsgType(Main_Chest, LC_ChestEnd), sizeof(LC_Cmd_ChestEnd));
	msg.ChestOnlyID = ChestOnlyID;
	pRole->SendDataToClient(&msg);
	return true;
}