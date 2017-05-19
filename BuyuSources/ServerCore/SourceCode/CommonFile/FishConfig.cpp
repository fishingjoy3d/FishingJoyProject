#include "Stdafx.h"
#include "FishConfig.h"
FishConfig::FishConfig()
{

}
FishConfig::~FishConfig()
{
	OnDestroy();
}


bool FishConfig::LoadFishChannelConfig()
{
	WHXml pXml;
	if (!pXml.LoadXMLFilePath(TEXT("OperatorConfig.xml")))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishConfig = pXml.GetChildNodeByName(TEXT("OperatorConfig"));
	WHXmlNode* pFishItem = pFishConfig->GetChildNodeByName(TEXT("Operator"));
	while (pFishItem)
	{
		tagChannelConfig entry;
		if (!pFishItem->GetAttribute(TEXT("channel_name"), entry.channel_name, CountArray(entry.channel_name) / 2))
		{
			return false;
		}

		
		if (!pFishItem->GetAttribute(TEXT("pay_notify_url"), entry.notify_pay_url, CountArray(entry.notify_pay_url)))
		{
			UINT count = 0;
			WCharToChar(entry.notify_pay_url, count);
			//UrlEncode()

			return false;
		}
		if (!pFishItem->GetAttribute(TEXT("channel"), entry.channel_id))
		{
			return false;
		}

		m_ChannelConfig[entry.channel_id] = entry;
		pFishItem = pFishItem->GetNextSignelNode();
	}
	return true;
}

const tagChannelConfig* FishConfig::GetChannelConfig(int channel_id)
{
	tagChannelConfig* entry = NULL;
	HashMap<DWORD, tagChannelConfig>::iterator it = m_ChannelConfig.find(channel_id);
	if (it != m_ChannelConfig.end())
	{
		entry = &it->second;
	}
	return entry;
}
bool FishConfig::LoadConfigFilePath()
{
	if (!LoadFishChannelConfig())
	{
		return false;			 
	}
	//1.加载配置文件
	WHXml pXml;
	if (!pXml.LoadXMLFilePath(TEXT("FishConfig.xml")))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishConfig = pXml.GetChildNodeByName(TEXT("FishConfig"));
	if (!pFishConfig)
	{
		ASSERT(false);
		return false;
	}
	//读取物品
	if (!LoadFishItemConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//奖励
	if (!LoadFishRewardConfig(pFishConfig))//奖励的读取应该放在前面
	{
		ASSERT(false);
		return false;
	}
	//1.Check
	if(!LoadFishCheck(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//2.Task
	if (!LoadFishTask(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//成就
	if (!LoadFishAchievement(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//3.Month
	if (!LoadFishMonth(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//4.称号
	if (!LoadFishTitle(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//5.排行榜
	if (!LoadFishRank(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//6.宝箱
	if (!LoadFishChest(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//7.魅力
	if (!LoadFishCharm(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//8.商店
	if (!LoadFishShop(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//9.活动
	if (!LoadFishAction(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//10.赠送
	if (!LoadFishGiff(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//11.桌子参数
	if (!LoadFishTableConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//12.关系配置
	if (!LoadFishRelationConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//13.物品转换
	if (!LoadFishItemConvertConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//服务器更新时间
	if (!LoadFishUpdateConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//邮件
	if (!LoadFishMailConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//金币商店
	/*if (!LoadFishGlobelShopConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}*/
	//在线奖励
	if (!LoadFishOnlineRewardConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//礼包
	if (!LoadFishPackageConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishSystemConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishRechargeConfig(pFishConfig))//加载RMB充值配置
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishLevelReward(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishExChangeConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishRoleProtectConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishLotteryConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishMonthCardConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishVipConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishDropConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishMiniGameConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	if (!LoadFishGameRobotConfig(pFishConfig))
	{
		ASSERT(false);
		return false;
	}
	//过滤关键字
	if (!LoadFishErrorStringFile("ErrorString.txt"))
	{
		ASSERT(false);
		return false;
	}

	if (!LoadFishNoticeConfig(TEXT("FishNoticeConfig.xml")))//加载公告的数据
	{
		ASSERT(false);
		return false;
	}
	if (!LoadRobotConfig(TEXT("robotconfig.xml")))//
	{
		ASSERT(false);
		return false;
	}

	if (!LoadGMToolConfig(TEXT("GMToolServerConfig.xml")))
	{
		ASSERT(false);
		return false;
	}
	return true;
}
void FishConfig::OnDestroy()
{
}
bool FishConfig::LoadFishCheck(WHXmlNode* pFishConfig)
{
	WHXmlNode* pCheck = pFishConfig->GetChildNodeByName(TEXT("FishCheck"));
	if (!pCheck)
	{
		ASSERT(false);
		return false;
	}
	if (!pCheck->GetAttribute(TEXT("CheckOtherUser"), m_CheckConfig.CheckOtherUser))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pCheckDay = pCheck->GetChildNodeByName(TEXT("CheckDay"));
	if (!pCheckDay)
	{
		ASSERT(false);
		return false;
	}
	if (!pCheckDay->GetAttribute(TEXT("MaxDaySum"), m_CheckConfig.MaxCheckDaySum))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pDaySum = pCheckDay->GetChildNodeByName(TEXT("DaySum"));
	while (pDaySum)
	{
		DWORD Day = 0;
		WORD RewardID = 0;
		if (!pDaySum->GetAttribute(TEXT("Day"), Day))
		{
			ASSERT(false);
			return false;
		}
		if (!pDaySum->GetAttribute(TEXT("RewardID"), RewardID))
		{
			ASSERT(false);
			return false;
		}
		m_CheckConfig.CheckDayReward.insert(HashMap<DWORD, WORD>::value_type(Day, RewardID));
		pDaySum = pDaySum->GetNextSignelNode();
	}
	WHXmlNode* pMonthCheck = pCheck->GetChildNodeByName(TEXT("MonthCheck"));
	if (!pMonthCheck)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pMonth = pMonthCheck->GetChildNodeByName(TEXT("DaySum"));
	while (pMonth)
	{
		DWORD Day = 0;
		WORD RewardID = 0;
		if (!pMonth->GetAttribute(TEXT("Day"), Day))
		{
			ASSERT(false);
			return false;
		}
		if (!pMonth->GetAttribute(TEXT("RewardID"), RewardID))
		{
			ASSERT(false);
			return false;
		}
		m_CheckConfig.MonthCheckReward.insert(HashMap<DWORD, WORD>::value_type(Day, RewardID));

		pMonth = pMonth->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishTask(WHXmlNode* pFishConfig)
{
	WHXmlNode* pTasks = pFishConfig->GetChildNodeByName(TEXT("FishTasks"));
	if (!pTasks)
	{
		ASSERT(false);
		return false;
	}
	if (!pTasks->GetAttribute(TEXT("MaxJoinTaskSum"), m_TaskConfig.m_MaxJoinTaskSum))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pTask = pTasks->GetChildNodeByName(TEXT("Task"));
	while (pTask)
	{
		tagTaskConfig pInfo;
		pInfo.LowerTaskVec.clear();
		if (!pTask->GetAttribute(TEXT("ID"), pInfo.TaskID))
		{
			return false;
		}

		BYTE IsClear = 0;
		if (!pTask->GetAttribute(TEXT("IsDayClear"), IsClear))
		{
			return false;
		}
		pInfo.IsDayClear = (IsClear == 1 ? true : false);

		if (!pTask->GetAttribute(TEXT("RewardID"), pInfo.RewardID))
			return false;
		if (!pTask->GetAttribute(TEXT("UpperTaskID"), pInfo.UpperTaskID))
			return false;
		if (!pTask->GetAttribute(TEXT("GroupID"), pInfo.GroupID))
			return false;
		if (!pTask->GetAttribute(TEXT("JoinLevel"), pInfo.JoinLevel))
			return false;
		if (pInfo.GroupID != 0)
		{
			HashMap<BYTE, std::vector<BYTE>>::iterator Iter = m_TaskConfig.m_TaskGroup.find(pInfo.GroupID);
			if (Iter == m_TaskConfig.m_TaskGroup.end())
			{
				std::vector<BYTE> pVec;
				pVec.push_back(pInfo.TaskID);
				m_TaskConfig.m_TaskGroup.insert(HashMap<BYTE, std::vector<BYTE>>::value_type(pInfo.GroupID, pVec));
			}
			else
			{
				Iter->second.push_back(pInfo.TaskID);
			}
		}

		if (pInfo.UpperTaskID != 0 && pInfo.UpperTaskID <= pInfo.TaskID)
		{
			ASSERT(false);
			return false;
		}

		WHXmlNode* pEvent = pTask->GetChildNodeByName(TEXT("Event"));
		if (pEvent)
		{
			if (!pEvent->GetAttribute(TEXT("OnlyID"), pInfo.EventInfo.OnlyID))
			{
				return false;
			}
			if (!pEvent->GetAttribute(TEXT("ID"), pInfo.EventInfo.EventID))
			{
				return false;
			}
			if (!pEvent->GetAttribute(TEXT("BindParam"), pInfo.EventInfo.BindParam))
			{
				return false;
			}
			if (!pEvent->GetAttribute(TEXT("FinishParam"), pInfo.EventInfo.FinishValue))
			{
				return false;
			}
		}

		//m_TaskConfig.m_TaskVec.push_back(pInfo);
		m_TaskConfig.m_TaskMap.insert(HashMap<BYTE, tagTaskConfig>::value_type(pInfo.TaskID,pInfo));
		pTask = pTask->GetNextSignelNode();
	}
	//所有任务完毕后 我们开始 设置任务的一些相互引用
	HashMap<BYTE, tagTaskConfig>::iterator Iter = m_TaskConfig.m_TaskMap.begin();
	for (; Iter != m_TaskConfig.m_TaskMap.end(); ++Iter)
	{
		if (Iter->second.UpperTaskID == 0)
			continue;
		HashMap<BYTE, tagTaskConfig>::iterator IterFind = m_TaskConfig.m_TaskMap.find(Iter->second.UpperTaskID);
		if (IterFind != m_TaskConfig.m_TaskMap.end())
		{
			IterFind->second.LowerTaskVec.push_back(Iter->second.TaskID);
		}
		else
		{
			ASSERT(false);
			return false;
		}
	}
	return true;
}
bool FishConfig::LoadFishAchievement(WHXmlNode* pFishConfig)
{
	//加载成就数据
	WHXmlNode* pAchievements = pFishConfig->GetChildNodeByName(TEXT("FishAchievements"));
	if (!pAchievements)
	{
		ASSERT(false);
		return false;
	}
	if (!pAchievements->GetAttribute(TEXT("MaxJoinAchievementSum"), m_AchievementConfig.m_MaxJoinAchievementSum))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pAchievement = pAchievements->GetChildNodeByName(TEXT("Achievement"));
	while (pAchievement)
	{
		tagAchievementConfig pInfo;
		pInfo.LowerAchievementVec.clear();
		if (!pAchievement->GetAttribute(TEXT("ID"), pInfo.AchievementID))
		{
			return false;
		}

		if (!pAchievement->GetAttribute(TEXT("RewardID"), pInfo.RewardID))
			return false;
		if (!pAchievement->GetAttribute(TEXT("UpperFishAchievement"), pInfo.UpperAchievementID))
			return false;
		if (!pAchievement->GetAttribute(TEXT("JoinLevel"), pInfo.JoinLevel))
			return false;
		

		if (pInfo.UpperAchievementID != 0 && pInfo.UpperAchievementID <= pInfo.AchievementID)
		{
			ASSERT(false);
			return false;
		}

		WHXmlNode* pEvent = pAchievement->GetChildNodeByName(TEXT("Event"));
		if (pEvent)
		{
			if (!pEvent->GetAttribute(TEXT("OnlyID"), pInfo.AchievementEventInfo.OnlyID))
			{
				return false;
			}
			if (!pEvent->GetAttribute(TEXT("ID"), pInfo.AchievementEventInfo.EventID))
			{
				return false;
			}
			if (!pEvent->GetAttribute(TEXT("BindParam"), pInfo.AchievementEventInfo.BindParam))
			{
				return false;
			}
			if (!pEvent->GetAttribute(TEXT("FinishParam"), pInfo.AchievementEventInfo.FinishValue))
			{
				return false;
			}
		}
		//m_AchievementConfig.m_AchievementVec.push_back(pInfo);
		m_AchievementConfig.m_AchievementMap.insert(HashMap<BYTE, tagAchievementConfig>::value_type(pInfo.AchievementID, pInfo));

		pAchievement = pAchievement->GetNextSignelNode();
	}
	//所有任务完毕后 我们开始 设置任务的一些相互引用
	HashMap<BYTE, tagAchievementConfig>::iterator Iter = m_AchievementConfig.m_AchievementMap.begin();
	for (; Iter != m_AchievementConfig.m_AchievementMap.end(); ++Iter)
	{
		if (Iter->second.UpperAchievementID == 0)
			continue;
		HashMap<BYTE, tagAchievementConfig>::iterator IterFind = m_AchievementConfig.m_AchievementMap.find(Iter->second.UpperAchievementID);
		if (IterFind != m_AchievementConfig.m_AchievementMap.end())
		{
			IterFind->second.LowerAchievementVec.push_back(Iter->second.AchievementID);
		}
		else
		{
			ASSERT(false);
			return false;
		}
	}
	return true;
}
bool FishConfig::LoadFishMonth(WHXmlNode* pFishConfig)
{
	//加载比赛的信息到集合里面去
	WHXmlNode* pFishMonths = pFishConfig->GetChildNodeByName(TEXT("FishMonths"));
	if (!pFishMonths)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishMonths->GetAttribute(TEXT("BeginMonthLevel"), m_MonthMap.m_MonthBeginLevel))
		return false;
	WHXmlNode* pMonth = pFishMonths->GetChildNodeByName(TEXT("Month"));
	while (pMonth)
	{
		//比赛数据
		tagMonthConfig pInfo;
		if (!pMonth->GetAttribute(TEXT("ID"), pInfo.MonthID))
			return false;
		if (!pMonth->GetAttribute(TEXT("SignGlobel"), pInfo.SignGlobel))
			return false;
		if (!pMonth->GetAttribute(TEXT("SignMedal"), pInfo.SignMadel))
			return false;
		if (!pMonth->GetAttribute(TEXT("SignCurrey"), pInfo.SignCurrey))
			return false;
		if (!pMonth->GetAttribute(TEXT("SignItemID"), pInfo.SignItem.ItemID))
			return false;
		if (!pMonth->GetAttribute(TEXT("SignItemSum"), pInfo.SignItem.ItemSum))
			return false;
		if (!pMonth->GetAttribute(TEXT("InitMatchGlobel"), pInfo.InitMonthGlobel))
			return false;
		if (!pMonth->GetAttribute(TEXT("TableTypeID"), pInfo.TableTypeID))
			return false;
		if (!pMonth->GetAttribute(TEXT("RankSum"), pInfo.RankSum))
			return false;
		if (!pMonth->GetAttribute(TEXT("MonthMinRateLimit"), pInfo.MinRateLimit))
			return false;
		if (!pMonth->GetAttribute(TEXT("TypeID"), pInfo.TypeID))
			return false;
		/*if (!pMonth->GetAttribute(TEXT("RobotSum"), pInfo.RobotSum))
			return false;*/
		//续币的数据
		WHXmlNode* pConverts = pMonth->GetChildNodeByName(TEXT("Converts"));
		if (pConverts)
		{
			WHXmlNode* pConvert = pConverts->GetChildNodeByName(TEXT("Convert"));
			while (pConvert)
			{
				tagMonthConvert pConvertInfo;

				BYTE BeginSum = 0, EndSum = 0;
				if (!pConvert->GetAttribute(TEXT("BeginSum"), BeginSum))
					return false;
				if (!pConvert->GetAttribute(TEXT("EndSum"), EndSum))
					return false;
				if (!pConvert->GetAttribute(TEXT("LostGlobel"), pConvertInfo.LostGlobel))
					return false;
				if (!pConvert->GetAttribute(TEXT("LostCurrey"), pConvertInfo.LostCurrey))
					return false;
				if (!pConvert->GetAttribute(TEXT("LostMedal"), pConvertInfo.LostMadle))
					return false;
				if (!pConvert->GetAttribute(TEXT("AddMonthGlobel"), pConvertInfo.AddMonthGlobel))
					return false;
				//加入到集合里面去
				for (BYTE i = BeginSum; i <= EndSum; ++i)
				{
					pConvertInfo.ConvertSum = i;
					pInfo.m_ConvertMap.insert(HashMap<BYTE, tagMonthConvert>::value_type(i, pConvertInfo));
				}
				pConvert = pConvert->GetNextSignelNode();
			}
		}
		//时间
		//因为时间段 很长 我们想要进行 重新读取 我们想要预先设置好时间 不要在运行的时候 处理
		WHXmlNode* pTime = pMonth->GetChildNodeByName(TEXT("Time"));
		//提前设置好一个月的 时间
		while (pTime)
		{
			tagMonthTime pTimeInfo;
			if (!pTime->GetAttribute(TEXT("ID"), pTimeInfo.ID))
				return false;
			if (!pTime->GetAttribute(TEXT("WriteNextMin"), pTimeInfo.WriteLastMin))
				return false;
			if (!pTime->GetAttribute(TEXT("BeginWeek"), pTimeInfo.BeginWeek))
				return false;
			if (!pTime->GetAttribute(TEXT("BeginHour"), pTimeInfo.BeginHour))
				return false;
			if (!pTime->GetAttribute(TEXT("BeginMin"), pTimeInfo.BeginMin))
				return false;
			if (!pTime->GetAttribute(TEXT("MonthLastMin"), pTimeInfo.AllLastMin))
				return false;
			BYTE IsContinue;
			if (!pTime->GetAttribute(TEXT("IsContinue"), IsContinue))
				return false;
			pTimeInfo.IsContinue = (IsContinue == 0 ? false : true);
			if (!pTime->GetAttribute(TEXT("RunSum"), pTimeInfo.RunSum))
				return false;

			WHXmlNode* pSign = pTime->GetChildNodeByName(TEXT("SignTime"));
			if (!pSign)
				return false;
			if (!pSign->GetAttribute(TEXT("BeginWriteMin"), pTimeInfo.SignWriteMin))
				return false;
			if (!pSign->GetAttribute(TEXT("LastMin"), pTimeInfo.SignLastMin))
				return false;

			WHXmlNode* pStart = pTime->GetChildNodeByName(TEXT("StartTime"));
			if (!pStart)
				return false;
			if (!pStart->GetAttribute(TEXT("BeginWriteMin"), pTimeInfo.StarWriteMin))
				return false;
			if (!pStart->GetAttribute(TEXT("LastMin"), pTimeInfo.SratLastMin))
				return false;
			pInfo.m_MonthTime.insert(HashMap<BYTE, tagMonthTime>::value_type(pTimeInfo.ID, pTimeInfo));
			pTime = pTime->GetNextSignelNode();
		}
		//奖励
		WHXmlNode* pRewards = pMonth->GetChildNodeByName(TEXT("Rewards"));
		if (!pRewards)
			return false;
		WHXmlNode* pReward = pRewards->GetChildNodeByName(TEXT("Reward"));
		while (pReward)
		{
			tagMonthReward pRewardInfo;

			WORD BeginIndex = 0, EndIndex = 0;
			if (!pReward->GetAttribute(TEXT("BeginIndex"), BeginIndex))
				return false;
			if (!pReward->GetAttribute(TEXT("EndIndex"), EndIndex))
				return false;
			if (!pReward->GetAttribute(TEXT("RewardID"), pRewardInfo.RewardID))
				return false;

			for (WORD i = BeginIndex; i <= EndIndex; ++i)
			{
				pRewardInfo.Index = i;
				pInfo.m_MonthReward.insert(HashMap<WORD, tagMonthReward>::value_type(i, pRewardInfo));
			}
			pReward = pReward->GetNextSignelNode();
		}
		//机器人
		pInfo.m_RobotMap.clear();
		WHXmlNode* pRobots = pMonth->GetChildNodeByName(TEXT("Robots"));
		if (pRobots)
		{
			WHXmlNode* pRobot = pRobots->GetChildNodeByName(TEXT("Robot"));
			while (pRobot)
			{
				tagMonthRobot pRobotInfo;

				if (!pRobot->GetAttribute(TEXT("Level"), pRobotInfo.Level))
					return false;
				if (!pRobot->GetAttribute(TEXT("AddScore"), pRobotInfo.AddScore))
					return false;
				if (!pRobot->GetAttribute(TEXT("Rate"), pRobotInfo.Rate))
					return false;
				if (!pRobot->GetAttribute(TEXT("Sum"), pRobotInfo.RobotSum))
					return false;
				if (!pRobot->GetAttribute(TEXT("MaxScore"), pRobotInfo.MaxScore))
					return false;
				if (!pRobot->GetAttribute(TEXT("SumRate"), pRobotInfo.SumRate))
					return false;

				if (!pRobot->GetAttribute(TEXT("SkillSum"), pRobotInfo.SkillSum))
					pRobotInfo.SkillSum = 0;
				if (!pRobot->GetAttribute(TEXT("SkillUseRate"), pRobotInfo.SkillUseRate))
					pRobotInfo.SkillUseRate = 0;
				if (!pRobot->GetAttribute(TEXT("SkillScore"), pRobotInfo.SkillScore))
					pRobotInfo.SkillScore = 0;
				if (!pRobot->GetAttribute(TEXT("SkillScoreRate"), pRobotInfo.SkillScoreRate))
					pRobotInfo.SkillScoreRate = 0;
				
				pInfo.m_RobotMap.insert(HashMap<DWORD, tagMonthRobot>::value_type(pRobotInfo.Level, pRobotInfo));

				pRobot = pRobot->GetNextSignelNode();
			}
		}
		m_MonthMap.m_MonthMap.insert(HashMap<BYTE, tagMonthConfig>::value_type(pInfo.MonthID, pInfo));
		pMonth = pMonth->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishTitle(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishTitles = pFishConfig->GetChildNodeByName(TEXT("FishTitles"));
	if (!pFishTitles)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pTitle = pFishTitles->GetChildNodeByName(TEXT("Title"));
	while (pTitle)
	{
		BYTE ID = 0;
		if (!pTitle->GetAttribute(TEXT("ID"), ID))
		{
			ASSERT(false);
			return false;
		}
		m_TitleSet.insert(set<BYTE>::value_type(ID));
		pTitle = pTitle->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishRank(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishRanks = pFishConfig->GetChildNodeByName(TEXT("FishRanks"));
	if (!pFishRanks)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishRanks->GetAttribute(TEXT("ReawrdWeekDay"), m_RankMap.RankWeekDay))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pRank = pFishRanks->GetChildNodeByName(TEXT("Rank"));
	while (pRank)
	{
		tagRankConfig pInfo;
		if (!pRank->GetAttribute(TEXT("ID"), pInfo.RankID))
		{
			ASSERT(false);
			return false;
		}
		if (!pRank->GetAttribute(TEXT("MaxRowSum"), pInfo.RowCount))
		{
			ASSERT(false);
			return false;
		}

		WHXmlNode* pReward = pRank->GetChildNodeByName(TEXT("Reward"));
		while (pReward)
		{
			BYTE BeginIndex = 0, EndIndex = 0;
			tagRankReward pRewardInfo;
			if (!pReward->GetAttribute(TEXT("BeginIndex"), BeginIndex))
			{
				ASSERT(false);
				return false;
			}
			if (!pReward->GetAttribute(TEXT("EndIndex"), EndIndex))
			{
				ASSERT(false);
				return false;
			}
			if (!pReward->GetAttribute(TEXT("RewardID"), pRewardInfo.RewardID))
			{
				ASSERT(false);
				return false;
			}

			for (BYTE i = BeginIndex; i <= EndIndex; ++i)
			{
				pInfo.RewardMap.insert(HashMap<BYTE, tagRankReward>::value_type(i, pRewardInfo));
			}
			pReward = pReward->GetNextSignelNode();
		}

		m_RankMap.m_RankMap.insert(HashMap<BYTE, tagRankConfig>::value_type(pInfo.RankID, pInfo));
		pRank = pRank->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishChest(WHXmlNode* pFishConfig)
{
	//加载宝箱配置数据
	WHXmlNode* pFishChests = pFishConfig->GetChildNodeByName(TEXT("FishChests"));
	if (!pFishChests)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishChest = pFishChests->GetChildNodeByName(TEXT("Chest"));
	while (pFishChest)
	{
		tagChestConfig pChestConfig;
		if (!pFishChest->GetAttribute(TEXT("ID"), pChestConfig.ChestID))
			return false;
		if (!pFishChest->GetAttribute(TEXT("ExistsSec"), pChestConfig.ExisteSec))
			return false;
		if (!pFishChest->GetAttribute(TEXT("ImmediateRewardid"), pChestConfig.ImmediateRewardid))
			return false;
		if (!pFishChest->GetAttribute(TEXT("CatchChance"), pChestConfig.CatchChance))
			return false;

		WHXmlNode* pFishCosts = pFishChest->GetChildNodeByName(TEXT("Costs"));
		if (!pFishCosts)
			return false;
		if (!pFishCosts->GetAttribute(TEXT("MaxCostSum"), pChestConfig.CostInfo.MaxCostNum))
			return false;
		WHXmlNode* pFishCost = pFishCosts->GetChildNodeByName(TEXT("Cost"));
		while (pFishCost)
		{
			BYTE CostNum = 0;
			DWORD UseCurrcey = 0;
			if (!pFishCost->GetAttribute(TEXT("Num"), CostNum))
				return false;
			if (!pFishCost->GetAttribute(TEXT("UserCurrcey"), UseCurrcey))//花费的乐币数量
				return false;
			pChestConfig.CostInfo.CostMap.insert(HashMap<BYTE, DWORD>::value_type(CostNum, UseCurrcey));
			pFishCost = pFishCost->GetNextSignelNode();
		}
		//提取奖励
		WHXmlNode* pRewards = pFishChest->GetChildNodeByName(TEXT("Rewards"));
		if (!pRewards)
			return false;
		pChestConfig.RewardInfo.MaxRewardChances = 0;
		if (!pRewards->GetAttribute(TEXT("SpecialRewardTypeID"), pChestConfig.RewardInfo.SpecialRewardTypeID))
			return false;
		if (!pRewards->GetAttribute(TEXT("SpecialRewardUseRp"), pChestConfig.RewardInfo.SpecialRewardUseRp))
			return false;
		if (!pRewards->GetAttribute(TEXT("OtherAddRp"), pChestConfig.RewardInfo.OtherAddRp))
			return false;
		WHXmlNode* pReward = pRewards->GetChildNodeByName(TEXT("Reward"));
		while (pReward)
		{
			tagChestsReward pRewardInfo;
			if (!pReward->GetAttribute(TEXT("ID"), pRewardInfo.RewardID))
				return false;
			if (!pReward->GetAttribute(TEXT("Chances"), pRewardInfo.Chances))
				return false;
			pChestConfig.RewardInfo.MaxRewardChances += pRewardInfo.Chances;
			//pRewardInfo.Chances = pChestConfig.RewardInfo.MaxRewardChances;
			
			//设置下面的随机的情况
			pRewardInfo.MaxOnceChances = 0;
			WHXmlNode* pOnce = pReward->GetChildNodeByName(TEXT("Once"));
			while (pOnce)
			{
				tagChestsRewardOnce pOnceConfig;
				if (!pOnce->GetAttribute(TEXT("ID"), pOnceConfig.OnceID))
					return false;
				if (!pOnce->GetAttribute(TEXT("Chances"), pOnceConfig.Chances))
					return false;
				pRewardInfo.MaxOnceChances += pOnceConfig.Chances;
				pOnceConfig.Chances = pRewardInfo.MaxOnceChances;
				if (!pOnce->GetAttribute(TEXT("RewardID"), pOnceConfig.RewardID))
					return false;

				pRewardInfo.RewardMap.push_back(pOnceConfig);
				pOnce = pOnce->GetNextSignelNode();
			}
			//pChestConfig.RewardInfo.RewardVec.push_back(pRewardInfo);
			pChestConfig.RewardInfo.RewardMap.insert(HashMap<BYTE, tagChestsReward>::value_type(pRewardInfo.RewardID, pRewardInfo));
			pReward = pReward->GetNextSignelNode();
		}
		m_ChestMap.ChestMap.insert(HashMap<BYTE, tagChestConfig> ::value_type(pChestConfig.ChestID, pChestConfig));
		pFishChest = pFishChest->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishCharm(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishCharms = pFishConfig->GetChildNodeByName(TEXT("FishCharms"));
	if (!pFishCharms)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishCharms->GetAttribute(TEXT("MaxSum"), m_CharmMap.CharmMaxItem))
		return false;
	WHXmlNode* pFishCharm = pFishCharms->GetChildNodeByName(TEXT("Charm"));
	while (pFishCharm)
	{
		tagCharmOnce pOnce;
		if (!pFishCharm->GetAttribute(TEXT("Index"), pOnce.Index))
			return false;
		if (!pFishCharm->GetAttribute(TEXT("LostGlobel"), pOnce.UserGlobel))
			return false;
		if (!pFishCharm->GetAttribute(TEXT("LostMedal"), pOnce.UserMedal))
			return false;
		if (!pFishCharm->GetAttribute(TEXT("LostCurrcey"), pOnce.UserCurrcey))
			return false;
		if (!pFishCharm->GetAttribute(TEXT("ChangeCharmValue"), pOnce.ChangeCharmValue))
			return false;
		if (!pFishCharm->GetAttribute(TEXT("ItemID"), pOnce.ItemID))
			return false;
		if (!pFishCharm->GetAttribute(TEXT("ItemSum"), pOnce.ItemSum))
			return false;
		m_CharmMap.CharmIndexMap.insert(HashMap<BYTE, tagCharmOnce>::value_type(pOnce.Index, pOnce));
		pFishCharm = pFishCharm->GetNextSignelNode();
	}
	return true;
}
int FishConfig::GetCharmValue(DWORD pInfo[MAX_CHARM_ITEMSUM])
{
	if (!pInfo)
		return 0;
	int Value = 0;
	for (BYTE i = 0; i < MAX_CHARM_ITEMSUM; ++i)
	{
		HashMap<BYTE, tagCharmOnce>::iterator Iter = m_CharmMap.CharmIndexMap.find(i);
		if (Iter != m_CharmMap.CharmIndexMap.end())
		{
			Value += (pInfo[i] * Iter->second.ChangeCharmValue);
		}
	}
	return Value;
}
bool FishConfig::LoadFishShop(WHXmlNode* pFishConfig)
{
	//加载商店的信息 商店是全局公用的 包括全部的GameServer之间 需要放到Center服务器进行
	//商店分2级 无限制的物品直接GamsServer进行购买 限制的物品 通过Center购买 
	WHXmlNode* pFishShops = pFishConfig->GetChildNodeByName(TEXT("FishShops"));
	if (!pFishShops)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pShop = pFishShops->GetChildNodeByName(TEXT("Shop"));
	while (pShop)
	{
		tagShopConfig pShopConfig;
		if (!pShop->GetAttribute(TEXT("ID"), pShopConfig.ShopID))
			return false;
		WHXmlNode* pItem = pShop->GetChildNodeByName(TEXT("Item"));
		while (pItem)
		{
			tagShopItemConfig pItemConfig;
			if (!pItem->GetAttribute(TEXT("ID"), pItemConfig.ShopItemIndex))
				return false;
			if (!pItem->GetAttribute(TEXT("ItemID"), pItemConfig.ItemInfo.ItemID))
				return false;
			if (!pItem->GetAttribute(TEXT("ItemSum"), pItemConfig.ItemInfo.ItemSum))
				return false;
			if (!pItem->GetAttribute(TEXT("LastMin"), pItemConfig.ItemInfo.LastMin))
				return false;
			if (!pItem->GetAttribute(TEXT("PriceGlobel"), pItemConfig.PriceGlobel))
				return false;
			if (!pItem->GetAttribute(TEXT("PriceMabel"), pItemConfig.PriceMabel))
				return false;
			if (!pItem->GetAttribute(TEXT("PriceCurrey"), pItemConfig.PriceCurrey))
				return false; 
			if (!pItem->GetAttribute(TEXT("ShopType"), pItemConfig.ShopType))
			{
				pItemConfig.ShopType = SIT_Normal;
			}
			BYTE IsCanPaile = 0;
			if (!pItem->GetAttribute(TEXT("IsCanPaile"), IsCanPaile))
			{
				pItemConfig.IsCanPile = true;
			}
			else
			{
				pItemConfig.IsCanPile = (IsCanPaile == 1 ? true : false);
			}		
			//读取时间
			WHXmlNode* pTime = pItem->GetChildNodeByName(TEXT("Time"));
			while (pTime)
			{
				tagShopItemTimeConfig pTimeConfig;
				SYSTEMTIME pSysteTime;
				pSysteTime.wMilliseconds = 0;
				pSysteTime.wSecond = 0;
				if (!pTime->GetAttribute(TEXT("BeginYear"), pSysteTime.wYear))
				{
					return false;
				}
				if (!pTime->GetAttribute(TEXT("BeginMonth"), pSysteTime.wMonth))
				{
					return false;
				}
				if (!pTime->GetAttribute(TEXT("BeginDay"), pSysteTime.wDay))
				{
					return false;
				}
				if (!pTime->GetAttribute(TEXT("BeginHour"), pSysteTime.wHour))
				{
					return false;
				}
				if (!pTime->GetAttribute(TEXT("BeginMin"), pSysteTime.wMinute))
				{
					return false;
				}
				if (!pTime->GetAttribute(TEXT("LastHour"), pTimeConfig.LastHour))
				{
					return false;
				}
				pTimeConfig.BeginTime = SystemTimeToTimeT(pSysteTime);
				pItemConfig.TimeVec.push_back(pTimeConfig);
				pTime = pTime->GetNextSignelNode();
			}
			pShopConfig.ShopItemMap.insert(HashMap<BYTE, tagShopItemConfig>::value_type(pItemConfig.ShopItemIndex, pItemConfig));
			pShopConfig.ShopItemVec.push_back(pItemConfig);
			pItem = pItem->GetNextSignelNode();
		}
		m_ShopMap.ShopMap.insert(HashMap<BYTE, tagShopConfig>::value_type(pShopConfig.ShopID, pShopConfig));
		m_ShopMap.ShopVec.push_back(pShopConfig);
		pShop = pShop->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishAction(WHXmlNode* pFishConfig)
{
	//加载成就数据
	//加载成就数据
	WHXmlNode* pActions = pFishConfig->GetChildNodeByName(TEXT("FishActions"));
	if (!pActions)
	{
		ASSERT(false);
		return false;
	}
	if (!pActions->GetAttribute(TEXT("MaxJoinActionSum"), m_ActionConfig.m_MaxJoinActionSum))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishAction = pActions->GetChildNodeByName(TEXT("Action"));
	while (pFishAction)
	{
		tagActionConfig pAction;

		if (!pFishAction->GetAttribute(TEXT("ID"), pAction.ActionID))
		{
			return false;
		}
		time_t Time;
		SYSTEMTIME pSysteTime;
		pSysteTime.wMilliseconds = 0;
		pSysteTime.wSecond = 0;
		if (!pFishAction->GetAttribute(TEXT("BeginYear"), pSysteTime.wYear))
		{
			return false;
		}
		if (!pFishAction->GetAttribute(TEXT("BeginMonth"), pSysteTime.wMonth))
		{
			return false;
		}
		if (!pFishAction->GetAttribute(TEXT("BeginDay"), pSysteTime.wDay))
		{
			return false;
		}
		if (!pFishAction->GetAttribute(TEXT("BeginHour"), pSysteTime.wHour))
		{
			return false;
		}
		pSysteTime.wMinute = 0;
		/*if (!pFishAction->GetAttribute(TEXT("BeginMin"), pSysteTime.wMinute))
		{
			delete pAction;
			return false;
		}*/
		if (!pFishAction->GetAttribute(TEXT("LastHour"), pAction.LastHour))
		{
			return false;
		}
		Time = SystemTimeToTimeT(pSysteTime);
		pAction.BeginTime = Time;

		WHXmlNode* pEventInfo = pFishAction->GetChildNodeByName(TEXT("Event"));
		while (pEventInfo)
		{
			if (!pEventInfo->GetAttribute(TEXT("ID"), pAction.EventID))
				return false;
			if (!pEventInfo->GetAttribute(TEXT("BindParam"), pAction.BindParam))
				return false;

			WHXmlNode* pOnce = pEventInfo->GetChildNodeByName(TEXT("Once"));
			pAction.FinishValue = 0;
			while (pOnce)
			{
				tagActionEventConfig pOnceConfig;
				if (!pOnce->GetAttribute(TEXT("ID"), pOnceConfig.ID))
					return false;
				if (!pOnce->GetAttribute(TEXT("RewardID"), pOnceConfig.RewardID))
					return false;
				if (!pOnce->GetAttribute(TEXT("FinishParam"), pOnceConfig.FinishValue))
					return false;
				pAction.FinishValue |= pOnceConfig.ID;
				pAction.EventMap.insert(HashMap<DWORD, tagActionEventConfig>::value_type(pOnceConfig.ID, pOnceConfig));
				pOnce = pOnce->GetNextSignelNode();
			}
			pEventInfo = pEventInfo->GetNextSignelNode();
		}
		m_ActionConfig.m_ActionMap.insert(HashMap<BYTE, tagActionConfig>::value_type(pAction.ActionID, pAction));
		pFishAction = pFishAction->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishGiff(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishGiff = pFishConfig->GetChildNodeByName(TEXT("FishGiff"));
	if (!pFishGiff)
	{
		ASSERT(false);
		return false;
	}
	//m_GiffConfig
	if (!pFishGiff->GetAttribute(TEXT("LimitDay"), m_GiffConfig.LimitDay))
	{
		ASSERT(false);
		return false;
	}
	if (!pFishGiff->GetAttribute(TEXT("SendSumByDay"), m_GiffConfig.SendSumLimitByDay))
	{
		ASSERT(false);
		return false;
	}
	if (!pFishGiff->GetAttribute(TEXT("AcceptSumByDay"), m_GiffConfig.AcceptSubLimitByDay))
	{
		ASSERT(false);
		return false;
	}
	if (!pFishGiff->GetAttribute(TEXT("SendOnceUserLimitByDay"), m_GiffConfig.SendOnceUserLimitByDay))
	{
		ASSERT(false);
		return false;
	}
	if (!pFishGiff->GetAttribute(TEXT("MaxUserGiffSum"), m_GiffConfig.MaxUserGiffSum))
	{
		ASSERT(false);
		return false;
	}
	if (!pFishGiff->GetAttribute(TEXT("RewardID"), m_GiffConfig.RewardID))
	{
		ASSERT(false);
		return false;
	}
	return true;
}
bool FishConfig::LoadFishTableConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishTables = pFishConfig->GetChildNodeByName(TEXT("FishTables"));
	if (!pFishTables)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pTable = pFishTables->GetChildNodeByName(TEXT("Table"));
	while (pTable)
	{
		TableInfo pTableInfo;

		if (!pTable->GetAttribute(TEXT("ID"), pTableInfo.TableTypeID))
		{
			ASSERT(false);
			return false;
		}

		int iValue = 0;
		if (!pTable->GetAttribute(TEXT("MinGlobelSum"), iValue))
		{
			ASSERT(false);
			return false;
		}
		if (iValue == -1)
			pTableInfo.MinGlobelSum = 0xffffffff;
		else
			pTableInfo.MinGlobelSum = ConvertIntToDWORD(iValue);

		if (!pTable->GetAttribute(TEXT("MaxGlobelSum"), iValue ))
		{
			ASSERT(false);
			return false;
		}
		if (iValue == -1)
			pTableInfo.MaxGlobelSum = 0xffffffff;
		else
			pTableInfo.MaxGlobelSum = ConvertIntToDWORD(iValue);

		if (!pTable->GetAttribute(TEXT("MinCurrey"), iValue ))
		{
			ASSERT(false);
			return false;
		}
		if (iValue == -1)
			pTableInfo.MinCurreySum = 0xffffffff;
		else
			pTableInfo.MinCurreySum = ConvertIntToDWORD(iValue);
		if (!pTable->GetAttribute(TEXT("MaxCurrey"), iValue ))
		{
			ASSERT(false);
			return false;
		}
		if (iValue == -1)
			pTableInfo.MaxCurreySum = 0xffffffff;
		else
			pTableInfo.MaxCurreySum = ConvertIntToDWORD(iValue);


		if (!pTable->GetAttribute(TEXT("MinLevel"), iValue))
		{
			ASSERT(false);
			return false;
		}
		if (iValue == -1)
			pTableInfo.MinLevel = 0xffffffff;
		else
			pTableInfo.MinLevel = ConvertIntToDWORD(iValue);
		if (!pTable->GetAttribute(TEXT("MaxLevel"), iValue))
		{
			ASSERT(false);
			return false;
		}
		if (iValue == -1)
			pTableInfo.MaxLevel = 0xffffffff;
		else
			pTableInfo.MaxLevel = ConvertIntToDWORD(iValue);

		if (!pTable->GetAttribute(TEXT("MaxPlayerSum"), pTableInfo.wMaxPlayerSum))
		{
			ASSERT(false);
			return false;
		}
		if (!pTable->GetAttribute(TEXT("MinRate"), pTableInfo.MinRate))
		{
			ASSERT(false);
			return false;
		}
		if (!pTable->GetAttribute(TEXT("MaxRate"), pTableInfo.MaxRate))
		{
			ASSERT(false);
			return false;
		}
		if (!pTable->GetAttribute(TEXT("TableRate"), pTableInfo.TableRate))
		{
			ASSERT(false);
			return false;
		}
		if (!pTable->GetAttribute(TEXT("MapName"), pTableInfo.cMapName,CountArray(pTableInfo.cMapName)))
		{
			ASSERT(false);
			return false;
		}

		WHXmlNode* pJoinItem = pTable->GetChildNodeByName(TEXT("pTable"));
		while (pJoinItem)
		{
			tagTableJoinItem pItem;
			if (!pJoinItem->GetAttribute(TEXT("ItemID"), pItem.ItemID))
			{
				ASSERT(false);
				return false;
			}
			if (!pJoinItem->GetAttribute(TEXT("ItemSum"), pItem.ItemSum))
			{
				ASSERT(false);
				return false;
			}
			pTableInfo.JoinItemVec.push_back(pItem);
			pJoinItem = pJoinItem->GetNextSignelNode();
		}

		m_TableConfig.m_TableConfig.insert(HashMap<BYTE, TableInfo>::value_type(pTableInfo.TableTypeID, pTableInfo));
		pTable = pTable->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishRelationConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishRelations = pFishConfig->GetChildNodeByName(TEXT("FishRelations"));
	if (!pFishRelations)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishRelations->GetAttribute(TEXT("MaxSum"), m_RelationConfig.MaxRelationSum))
		return false;
	if (!pFishRelations->GetAttribute(TEXT("RequestMaxSum"), m_RelationConfig.MaxRelationRequestSum))
		return false;
	if (!pFishRelations->GetAttribute(TEXT("RequestLimitDay"), m_RelationConfig.RelationRequestLimitDay))
		return false;
	return true;
}
bool FishConfig::LoadFishItemConvertConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishItems = pFishConfig->GetChildNodeByName(TEXT("FishConvertItems"));
	if (!pFishItems)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishItem = pFishItems->GetChildNodeByName(TEXT("FishItem"));
	while (pFishItem)
	{
		tagItemConvert pItem;
		if (!pFishItem->GetAttribute(TEXT("ItemID"), pItem.ItemID))
			return false;
		if (!pFishItem->GetAttribute(TEXT("Globel"), pItem.Globel))
			return false;
		if (!pFishItem->GetAttribute(TEXT("Medal"), pItem.Medal))
			return false;
		if (!pFishItem->GetAttribute(TEXT("Currey"), pItem.Currey))
			return false;
		m_ItemConvertConfig.m_ConvertMap.insert(HashMap<DWORD, tagItemConvert>::value_type(pItem.ItemID,pItem));
		pFishItem = pFishItem->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishUpdateConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishUpdate = pFishConfig->GetChildNodeByName(TEXT("FishServerUpdateTime"));
	if (!pFishUpdate)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishUpdate->GetAttribute(TEXT("UpdateHour"), m_FishUpdateConfig.UpdateHour))
		return false;
	if (!pFishUpdate->GetAttribute(TEXT("UpdateMin"), m_FishUpdateConfig.UpdateMin))
		return false;
	return true;
}
bool FishConfig::LoadFishMailConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishMail = pFishConfig->GetChildNodeByName(TEXT("FishMail"));
	if (!pFishMail)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishMail->GetAttribute(TEXT("LimitDay"), m_FishMailConfig.MailLimitDay))
		return false;
	if (!pFishMail->GetAttribute(TEXT("UserMaxMailSum"), m_FishMailConfig.MaxUserMailSum))
		return false;
	if (!pFishMail->GetAttribute(TEXT("ClientShowMailSum"), m_FishMailConfig.ClientShowMailSum))
		return false;
	return true;
}
//bool FishConfig::LoadFishGlobelShopConfig(WHXmlNode* pFishConfig)
//{
//	WHXmlNode* pFishGlobelShop = pFishConfig->GetChildNodeByName(TEXT("FishGlobelShop"));
//	if (!pFishGlobelShop)
//	{
//		ASSERT(false);
//		return false;
//	}
//	WHXmlNode* pShopItem = pFishGlobelShop->GetChildNodeByName(TEXT("Item"));
//	while (pShopItem)
//	{
//		tagGlobelShopItem pItem;
//
//		if (!pShopItem->GetAttribute(TEXT("ID"), pItem.ID))
//			return false;
//		if (!pShopItem->GetAttribute(TEXT("GlobelSum"), pItem.AddGlobel))
//			return false;
//		if (!pShopItem->GetAttribute(TEXT("LoseCurrey"), pItem.LostCurrey))
//			return false;
//
//		m_GlobelShopConfig.m_ShopItem.insert(HashMap<BYTE, tagGlobelShopItem>::value_type(pItem.ID,pItem));
//
//		pShopItem = pShopItem->GetNextSignelNode();
//	}
//	return true;
//}
bool FishConfig::LoadFishItemConfig(WHXmlNode* pFishConfig)
{
	m_ItemMap.clear();
	WHXmlNode* pFishItems = pFishConfig->GetChildNodeByName(TEXT("FishItems"));
	if (!pFishItems)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishItem = pFishItems->GetChildNodeByName(TEXT("Item"));
	while (pFishItem)
	{
		tagItemConfig pItemConfig;

		if (!pFishItem->GetAttribute(TEXT("ID"), pItemConfig.ItemID))
		{
			return false;
		}
		if (!pFishItem->GetAttribute(TEXT("TypeID"), pItemConfig.ItemType))
		{
			return false;
		}
		if (!pFishItem->GetAttribute(TEXT("Name"), pItemConfig.ItemName, CountArray(pItemConfig.ItemName) / 2))
		{


		}

		if (!pFishItem->GetAttribute(TEXT("ItemDec"), pItemConfig.ItemDesc, CountArray(pItemConfig.ItemDesc) / 2))
		{

		}
		if (!pFishItem->GetAttribute(TEXT("ItemParam"), pItemConfig.ItemParam))
		{
			pItemConfig.ItemParam = 0;
		}



		m_ItemMap.insert(HashMap<DWORD, tagItemConfig>::value_type(pItemConfig.ItemID, pItemConfig));
		pFishItem = pFishItem->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishOnlineRewardConfig(WHXmlNode* pFishConfig)
{
	//获取玩家在线奖励的数据
	WHXmlNode* pFishOnlineReward = pFishConfig->GetChildNodeByName(TEXT("FishOnlineReward"));
	if (!pFishOnlineReward)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pOnce = pFishOnlineReward->GetChildNodeByName(TEXT("Once"));
	while (pOnce)
	{
		tagOnceOnlienReward pOnceReward;

		if (!pOnce->GetAttribute(TEXT("ID"), pOnceReward.ID))
			return false;
		if (pOnceReward.ID < 1 || pOnceReward.ID>32)//ID 范围为1到32
			return false;
		if (!pOnce->GetAttribute(TEXT("OnlineMin"), pOnceReward.OnlineMin))
			return false;
		if (!pOnce->GetAttribute(TEXT("RewardID"), pOnceReward.RewardID))
			return false;
		m_OnlineRewardConfig.m_OnlineRewardMap.insert(HashMap<BYTE, tagOnceOnlienReward>::value_type(pOnceReward.ID, pOnceReward));
		pOnce = pOnce->GetNextSignelNode();
	}
	if (m_OnlineRewardConfig.m_OnlineRewardMap.size() > 32)
	{
		ASSERT(false);
		return false;
	}
	return true;
}
bool FishConfig::LoadFishPackageConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishPackages = pFishConfig->GetChildNodeByName(TEXT("FishPackages"));
	if (!pFishPackages)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishPackage = pFishPackages->GetChildNodeByName(TEXT("FishPackage"));
	while (pFishPackage)
	{
		tagFishPackage pPack;
		if (!pFishPackage->GetAttribute(TEXT("ItemID"), pPack.ItemID))
			return false;
		BYTE IsNeed = 0;
		if (!pFishPackage->GetAttribute(TEXT("IsAutoOpen"), IsNeed))
			return false;
		pPack.IsAutoOpen = (IsNeed == 1);

		if (!ItemIsExists(pPack.ItemID))
		{
			ASSERT(false);
			return false;
		}

		WHXmlNode* pFishGroup = pFishPackage->GetChildNodeByName(TEXT("Group"));
		while (pFishGroup)
		{
			tagFishPackageGroup pGroup;
			if (!pFishGroup->GetAttribute(TEXT("ID"), pGroup.GroupID))
				return false;

			WHXmlNode* pFishItem = pFishGroup->GetChildNodeByName(TEXT("Item"));
			DWORD TotalValue = 0;
			while (pFishItem)
			{
				tagFishPackageItem pItem;
				if (!pFishItem->GetAttribute(TEXT("RewardID"), pItem.RewardID))
					return false;
				if (!pFishItem->GetAttribute(TEXT("RankValue"), pItem.RankValue))
					return false;	

				TotalValue += pItem.RankValue;
				pItem.RankValue = TotalValue;
				pGroup.PackageItemVec.push_back(pItem);
				pFishItem = pFishItem->GetNextSignelNode();
			}
			pGroup.MaxRankValue = TotalValue;
			pPack.ItemVec.push_back(pGroup);
			pFishGroup = pFishGroup->GetNextSignelNode();
		}
		m_FishPackageConfig.m_PackageMap.insert(HashMap<DWORD, tagFishPackage>::value_type(pPack.ItemID, pPack));
		pFishPackage = pFishPackage->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishRewardConfig(WHXmlNode* pFishConfig)//游戏内部奖励的集合 货币 称号 成就的 以及物品(物品可能是宝箱等情况) 发送方式分为邮件 和 直接给予2种
{
	//加载玩家奖励的配置
	WHXmlNode* pFishRewards = pFishConfig->GetChildNodeByName(TEXT("FishRewardInfo"));
	if (!pFishRewards)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishReward = pFishRewards->GetChildNodeByName(TEXT("Reward"));
	while (pFishReward)
	{
		tagRewardOnce pReward;

		if (!pFishReward->GetAttribute(TEXT("RewardID"), pReward.RewardID))
			return false;	
		WHXmlNode* pFishItem = pFishReward->GetChildNodeByName(TEXT("Item"));
		while (pFishItem)
		{
			tagItemOnce pItem;
			if (!pFishItem->GetAttribute(TEXT("ItemID"), pItem.ItemID))
				return false;
			if (!ItemIsExists(pItem.ItemID))
				return false;
			if (!pFishItem->GetAttribute(TEXT("ItemSum"), pItem.ItemSum))
				return false;
			if (!pFishItem->GetAttribute(TEXT("LastMin"), pItem.LastMin))
				return false;
			pReward.RewardItemVec.push_back(pItem);
			pFishItem = pFishItem->GetNextSignelNode();
		}
		m_FishRewardConfig.RewardMap.insert(HashMap<WORD, tagRewardOnce>::value_type(pReward.RewardID, pReward));
		pFishReward = pFishReward->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishSystemConfig(WHXmlNode* pFishConfig)
{
	//m_SystemConfig
	WHXmlNode* pFishSystem = pFishConfig->GetChildNodeByName(TEXT("FishSystem"));
	if (!pFishSystem)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishSystem->GetAttribute(TEXT("Version"), m_SystemConfig.VersionID))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("PathCrc"), m_SystemConfig.PathCrc))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("MaxAchievementRankSum"), m_SystemConfig.AchievementRankSum))
		return false;
	if (m_SystemConfig.AchievementRankSum > 12000)
		return false;
	if (!pFishSystem->GetAttribute(TEXT("CacheMin"), m_SystemConfig.CacheUserMin))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("RsgInitGlobel"), m_SystemConfig.RsgInitGlobelSum))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("AnnouncementSum"), m_SystemConfig.AnnouncementSum))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("FirstBindPhoneRewardID"), m_SystemConfig.FirstBindPhoneRewardID))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("UserQueueHandleSumBySec"), m_SystemConfig.UserQueueHandleSumBySec))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("ChangeNickNameNeedItemID"), m_SystemConfig.ChangeNickNameNeedItemID))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("ChangeNickNameNeedItemSum"), m_SystemConfig.ChangeNickNameNeedItemSum))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("ChangeGenderNeedItemID"), m_SystemConfig.ChangeGenderNeedItemID))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("ChangeGenderNeedItemSum"), m_SystemConfig.ChangeGenderNeedItemSum))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("LeaveTableNonLauncherMin"), m_SystemConfig.LeaveTableNonLauncherMin))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("InitRateValue"), m_SystemConfig.InitRateValue))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("MaxGobelSum"), m_SystemConfig.MaxGobelSum))
	{
		m_SystemConfig.MaxGobelSum = 4200000000;
	}
	if (!pFishSystem->GetAttribute(TEXT("EmailGlobelRewardID"), m_SystemConfig.EmailGlobelRewardID))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("EmailCurrceyRewardID"), m_SystemConfig.EmailCurrceyRewardID))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("EmailMedalRewradID"), m_SystemConfig.EmailMedalRewradID))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("RoleSaveMin"), m_SystemConfig.RoleSaveMin))
		return false;

	{
		TCHAR TimeLog[32] = { 0 };
		if (!pFishSystem->GetAttribute(TEXT("RateInitRewardTime"), TimeLog, CountArray(TimeLog)))
			return false;

		int Year = 0;
		int Month = 0;
		int Day = 0;
		int Hour = 0;
		int Min = 0;
		int Sec = 0;

		if (_stscanf_s(TimeLog, TEXT("%d-%d-%d %d:%d:%d"), &Year, &Month, &Day, &Hour, &Min, &Sec) != 6)
			return false;

		tm pTime;
		pTime.tm_year = Year - 1900;
		pTime.tm_mon = Month - 1;
		pTime.tm_mday = Day;
		pTime.tm_hour = Hour;
		pTime.tm_min = Min;
		pTime.tm_sec = Sec;

		m_SystemConfig.RateInitRewardTime = mktime(&pTime);
	}

	if (!pFishSystem->GetAttribute(TEXT("SendMessageItemID"), m_SystemConfig.SendMessageItemID))
		return false;
	if (!pFishSystem->GetAttribute(TEXT("SendMessageItemSum"), m_SystemConfig.SendMessageItemSum))
		return false;

	return true;
}
bool FishConfig::LoadFishRechargeConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishRecharges = pFishConfig->GetChildNodeByName(TEXT("FishRecharges"));
	if (!pFishRecharges)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishRecharge = pFishRecharges->GetChildNodeByName(TEXT("FishRecharge"));
	while (pFishRecharge)
	{
		tagFishRechargeInfo pInfo;

		if (!pFishRecharge->GetAttribute(TEXT("ID"), pInfo.ID))
			return false;
		if (!pFishRecharge->GetAttribute(TEXT("DisCountPrice"), pInfo.dDisCountPrice))
			return false;
		if (!pFishRecharge->GetAttribute(TEXT("PreDisCountPrice"), pInfo.dPreDisCountPrice))
			return false;
		if (!pFishRecharge->GetAttribute(TEXT("AddMoney"), pInfo.AddMoney))
			return false;
		if (!pFishRecharge->GetAttribute(TEXT("AddRewardID"), pInfo.RewardID))
			return false;
		//BYTE IsCurrcey = 1;
		if (!pFishRecharge->GetAttribute(TEXT("RechargeType"), pInfo.RechargeType))
			return false;

		if (!pFishRecharge->GetAttribute(TEXT("Name"), pInfo.Name, CountArray(pInfo.Name)))
			return false;

		if (!pFishRecharge->GetAttribute(TEXT("PayNO"), pInfo.PayNO, CountArray(pInfo.PayNO)))
			return false;
		if (!pFishRecharge->GetAttribute(TEXT("Icon"), pInfo.Icon, CountArray(pInfo.Icon)))
			return false;
		if (!pFishRecharge->GetAttribute(TEXT("DisCountPicName"), pInfo.DisCountPicName, CountArray(pInfo.DisCountPicName)))
			return false;

		/*pInfo.IsCurreyOrGlobel = (IsCurrcey == 1 ? true : false);
		BYTE IsFistPay;
		if (!pFishRecharge->GetAttribute(TEXT("IsFirstPay"), IsFistPay))
			return false;
		pInfo.IsFirstPay = (IsFistPay == 1 ? true : false);*/
		
		m_FishRechargeMap.m_FishRechargeMap.insert(HashMap<DWORD, tagFishRechargeInfo>::value_type(pInfo.ID,pInfo));
		pFishRecharge = pFishRecharge->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishLevelReward(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishLevelReward = pFishConfig->GetChildNodeByName(TEXT("FishLevelReward"));
	if (!pFishLevelReward)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pLevelReward = pFishLevelReward->GetChildNodeByName(TEXT("LevelReward"));
	while (pLevelReward)
	{
		WORD Level = 0;
		WORD RewardID = 0;

		if (!pLevelReward->GetAttribute(TEXT("Level"), Level))
			return false;
		if (!pLevelReward->GetAttribute(TEXT("RewardID"), RewardID))
			return false;
		m_LevelRewardMap.m_LevelRewardMap.insert(HashMap<WORD, WORD>::value_type(Level, RewardID));
		pLevelReward = pLevelReward->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishExChangeConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishExChanges = pFishConfig->GetChildNodeByName(TEXT("FishExChanges"));
	if (!pFishExChanges)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pExChange = pFishExChanges->GetChildNodeByName(TEXT("ExChange"));
	while (pExChange)
	{
		tagExChange pOnce;

		if (!pExChange->GetAttribute(TEXT("ID"), pOnce.ID))
			return false;
		if (!pExChange->GetAttribute(TEXT("TypeID"), pOnce.TypeID))
			return false;
		
		if (!pExChange->GetAttribute(TEXT("RewardID"), pOnce.RewardID))
			return false;
		//平台ID 为字符串 我们想要进行处理 携带的字符串CRC转平台ID
		TCHAR pChannel[256];
		if (!pExChange->GetAttribute(TEXT("ChannelID"), pChannel, CountArray(pChannel)))
			return false;
		//先转小写
		int len = _tcslen(pChannel);
		if (len != 0)
		{
			for (int i = 0; i < len; ++i)
			{
				pChannel[i] = ::towlower(pChannel[i]);
			}
			pOnce.ChannelID = AECrc32(pChannel, sizeof(TCHAR)*len, 0);

		}
		else
		{
			pOnce.ChannelID = 0;
		}

		m_ExChangeMap.m_ExChangeMap.insert(HashMap<BYTE, tagExChange>::value_type(pOnce.ID, pOnce));

		pExChange = pExChange->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishRoleProtectConfig(WHXmlNode* pFishConfig)
{
	//加载玩家新手保护的配置文件
	WHXmlNode* pFishRoleProtect = pFishConfig->GetChildNodeByName(TEXT("FishRoleProtect"));
	if (!pFishRoleProtect)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishRoleProtect->GetAttribute(TEXT("ProtectSum"), m_RoleProctectMap.ProtectSum))
		return false;
	if (!pFishRoleProtect->GetAttribute(TEXT("ProtectTimeMin"), m_RoleProctectMap.ProtectCDMin))
		return false;
	if (!pFishRoleProtect->GetAttribute(TEXT("AddGlobel"), m_RoleProctectMap.AddGlobel))
		return false;
	if (!pFishRoleProtect->GetAttribute(TEXT("MaxGlobelSum"), m_RoleProctectMap.MaxGlobelSum))
		return false;
	if (!pFishRoleProtect->GetAttribute(TEXT("OpenProtectGlobelSum"), m_RoleProctectMap.OpenProtectGlobelSum))
		return false;
	m_RoleProctectMap.TableMap.clear();
	WHXmlNode* pTable = pFishRoleProtect->GetChildNodeByName(TEXT("Table"));
	while (pTable)
	{
		BYTE TableTypeID = 0;
		if (!pTable->GetAttribute(TEXT("ID"), TableTypeID))
			return false;
		m_RoleProctectMap.TableMap.insert(HashMap<BYTE, BYTE>::value_type(TableTypeID, 0));
		pTable = pTable->GetNextSignelNode();
	}
	return true;
}



bool FishConfig::LoadFishLotteryConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishLottery = pFishConfig->GetChildNodeByName(TEXT("FishLottery"));
	if (!pFishLottery)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishLottery->GetAttribute(TEXT("MaxLotteryFishSum"), m_LotteryMap.MaxLotteryFishSum))
		return false;

	WHXmlNode* pLotterys = pFishLottery->GetChildNodeByName(TEXT("Lotterys"));
	if (!pLotterys)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pLottery = pLotterys->GetChildNodeByName(TEXT("Lottery"));
	
	while (pLottery)
	{
		tagLotteryOnce pLotteryOnce;
		DWORD TatalRate = 0;
		if (!pLottery->GetAttribute(TEXT("ID"), pLotteryOnce.LotteryID))
			return false;
		if (!pLottery->GetAttribute(TEXT("UpperID"), pLotteryOnce.UpperLotteryID))
			return false;
		if (!pLottery->GetAttribute(TEXT("OpenNeedScore"), pLotteryOnce.NeedUseScore))
			return false;

		WHXmlNode* pReward = pLottery->GetChildNodeByName(TEXT("Reward"));
		while (pReward)
		{
			WORD RewardID = 0;
			if (!pReward->GetAttribute(TEXT("RewardID"), RewardID))
				return false;
			DWORD Rate = 0;
			if (!pReward->GetAttribute(TEXT("RewardRate"), Rate))
				return false;
			TatalRate += Rate;

			tagLotteryReward pRewardOnce;
			pRewardOnce.Rate = TatalRate;
			pRewardOnce.RewardID = RewardID;
			pLotteryOnce.RewardVec.push_back(pRewardOnce);

			pReward = pReward->GetNextSignelNode();
		}
		pLotteryOnce.TotalRate = TatalRate;
		m_LotteryMap.LotteryMap.insert(HashMap<BYTE, tagLotteryOnce>::value_type(pLotteryOnce.LotteryID, pLotteryOnce));

		pLottery = pLottery->GetNextSignelNode();
	}
	WHXmlNode* pFishScore = pFishLottery->GetChildNodeByName(TEXT("FishScore"));
	if (!pFishScore)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFish = pFishScore->GetChildNodeByName(TEXT("Fish"));
	while (pFish)
	{
		BYTE FishTypeID = 0;
		if (!pFish->GetAttribute(TEXT("TypeID"), FishTypeID))
			return false;
		DWORD Score = 0;
		if (!pFish->GetAttribute(TEXT("Score"), Score))
			return false;
		m_LotteryMap.FishScore.insert(HashMap<BYTE, DWORD>::value_type(FishTypeID,Score));
		pFish = pFish->GetNextSignelNode();
	}

	return true;
}
bool FishConfig::LoadFishMonthCardConfig(WHXmlNode* pFishConfig)
{
	//加载月卡的配置文件
	WHXmlNode* pFishMonthCards = pFishConfig->GetChildNodeByName(TEXT("FishMonthCards"));
	if (!pFishMonthCards)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pMonthCard = pFishMonthCards->GetChildNodeByName(TEXT("MonthCard"));
	while (pMonthCard)
	{
		tagMonthCardOnce pOnce;

		if (!pMonthCard->GetAttribute(TEXT("ID"), pOnce.MonthCardID))
			return false;
		if (!pMonthCard->GetAttribute(TEXT("CardLastMin"), pOnce.UseLastMin))
			return false;
		if (!pMonthCard->GetAttribute(TEXT("OnceDayRewardID"), pOnce.OnceDayRewardID))
			return false;
		BYTE IsCanAutoFrie = 0;
		if (!pMonthCard->GetAttribute(TEXT("IsCanAutoFire"), IsCanAutoFrie))
			return false;
		pOnce.IsCanAutoFire = IsCanAutoFrie == 1 ? true : false;
		if (!pMonthCard->GetAttribute(TEXT("AddLotteryRate"), pOnce.AddLotteryRate))
			return false;

		m_MonthCardMap.MonthCardMap.insert(HashMap<BYTE, tagMonthCardOnce>::value_type(pOnce.MonthCardID, pOnce));

		pMonthCard = pMonthCard->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishVipConfig(WHXmlNode* pFishConfig)
{
	//加载VIP配置数据
	WHXmlNode* pFishVips = pFishConfig->GetChildNodeByName(TEXT("FishVips"));
	if (!pFishVips)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishVips->GetAttribute(TEXT("DefaultLauncherReBoundNum"), m_VipMap.DefaultLauncherReBoundNum))
		return false;
	if (!pFishVips->GetAttribute(TEXT("DefaultAlmsSum"), m_VipMap.DefaultAlmsSum))
		return false;
	if (!pFishVips->GetAttribute(TEXT("DefaultUseMedalSum"), m_VipMap.DefaultUseMedalSum))
		return false;

	WHXmlNode* pFishVip = pFishVips->GetChildNodeByName(TEXT("Vip"));
	tagVipOnce pTotalOnce;
	pTotalOnce.AddAlmsRate = 0;
	pTotalOnce.AddAlmsSum = 0;
	pTotalOnce.AddMonthScoreRate = 0;
	pTotalOnce.AddReChargeRate = 0;
	pTotalOnce.CanUseLauncherMap.clear();
	//pTotalOnce.IsCanLauncherLocking = false;
	pTotalOnce.LauncherReBoundNum = 0;
	pTotalOnce.NeedRechatgeRMBSum = 0;
	pTotalOnce.UpperVipLevel = 0;
	pTotalOnce.VipLevel = 0;
	while (pFishVip)
	{
		tagVipOnce pCurrceyOnce;

		if (!pFishVip->GetAttribute(TEXT("VipLevel"), pCurrceyOnce.VipLevel))
			return false;
		if (!pFishVip->GetAttribute(TEXT("UpperVipLevel"), pCurrceyOnce.UpperVipLevel))
			return false;
		if (!pFishVip->GetAttribute(TEXT("LauncherReBoundNum"), pCurrceyOnce.LauncherReBoundNum))
			return false;
		/*BYTE IsCanLauncherLocking = 0;
		if (!pFishVip->GetAttribute(TEXT("IsCanLauncherLocking"), IsCanLauncherLocking))
			return false;
		pCurrceyOnce.IsCanLauncherLocking = (IsCanLauncherLocking == 1 ? true : false);*/

		if (!pFishVip->GetAttribute(TEXT("AddAlmsSum"), pCurrceyOnce.AddAlmsSum))
			return false;
		if (!pFishVip->GetAttribute(TEXT("AddAlmsRate"), pCurrceyOnce.AddAlmsRate))
			return false;
		if (!pFishVip->GetAttribute(TEXT("AddMonthScoreRate"), pCurrceyOnce.AddMonthScoreRate))
			return false;
		if (!pFishVip->GetAttribute(TEXT("AddReChargeRate"), pCurrceyOnce.AddReChargeRate))
			return false;
		if (!pFishVip->GetAttribute(TEXT("RechargeRMBSum"), pCurrceyOnce.NeedRechatgeRMBSum))
			return false;
		if (!pFishVip->GetAttribute(TEXT("AddUseMedalSum"), pCurrceyOnce.AddUseMedalSum))
			return false;
		if (!pFishVip->GetAttribute(TEXT("AddCatchFishRate"), pCurrceyOnce.AddCatchFishRate))
			return false;
		BYTE AddLauncherID = 0;
		if (!pFishVip->GetAttribute(TEXT("AddLauncherID"), AddLauncherID))
			return false;

		//加入到总等级里面
		pTotalOnce.VipLevel = pCurrceyOnce.VipLevel;
		pTotalOnce.UpperVipLevel = pCurrceyOnce.UpperVipLevel;
		pTotalOnce.LauncherReBoundNum += pCurrceyOnce.LauncherReBoundNum;
		//pTotalOnce.IsCanLauncherLocking = (pTotalOnce.IsCanLauncherLocking || pCurrceyOnce.IsCanLauncherLocking);
		pTotalOnce.AddAlmsRate += pCurrceyOnce.AddAlmsRate;
		pTotalOnce.AddAlmsSum += pCurrceyOnce.AddAlmsSum;
		pTotalOnce.AddMonthScoreRate += pCurrceyOnce.AddMonthScoreRate;
		pTotalOnce.AddReChargeRate += pCurrceyOnce.AddReChargeRate;
		pTotalOnce.NeedRechatgeRMBSum = pCurrceyOnce.NeedRechatgeRMBSum;
		pTotalOnce.AddUseMedalSum += pCurrceyOnce.AddUseMedalSum;
		pTotalOnce.AddCatchFishRate += pCurrceyOnce.AddCatchFishRate;
		pTotalOnce.CanUseLauncherMap.insert(set<BYTE>::value_type(AddLauncherID));

		m_VipMap.VipMap.insert(HashMap<BYTE, tagVipOnce>::value_type(pTotalOnce.VipLevel,pTotalOnce));
		pFishVip = pFishVip->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishDropConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishDrop = pFishConfig->GetChildNodeByName(TEXT("FishDrop"));
	if (!pFishDrop)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode*  pType = pFishDrop->GetChildNodeByName(TEXT("type"));
	while (pType)
	{
		tagFishDropOnce pFishOnce;

		if (!pType->GetAttribute(TEXT("ID"), pFishOnce.FishTypeID))
			return false;

		pFishOnce.TotalRateValue = 0;
		WHXmlNode*  pOnce = pType->GetChildNodeByName(TEXT("Once"));
		while (pOnce)
		{
			DropInfo pDrop;
			if (!pOnce->GetAttribute(TEXT("Chances"), pDrop.RateValue))
				return false;

			if (!pOnce->GetAttribute(TEXT("RewardID"), pDrop.RewardID))
				return false;

			pFishOnce.TotalRateValue += pDrop.RateValue;
			pDrop.RateValue = pFishOnce.TotalRateValue;
			pFishOnce.DroupVec.push_back(pDrop);

			pOnce = pOnce->GetNextSignelNode();
		}
		m_FishDropConfig.FishDropMap.insert(HashMap<BYTE, tagFishDropOnce>::value_type(pFishOnce.FishTypeID, pFishOnce));

		pType = pType->GetNextSignelNode();
	}
	return true;
}
bool FishConfig::LoadFishMiniGameConfig(WHXmlNode* pFishConfig)
{
	WHXmlNode* pFishMiniGame = pFishConfig->GetChildNodeByName(TEXT("FishMiniGame"));
	if (!pFishMiniGame)
	{
		ASSERT(false);
		return false;
	}

	//1.niuniu
	WHXmlNode* pFishNiuNiu = pFishMiniGame->GetChildNodeByName(TEXT("niuniu"));
	if (!pFishNiuNiu)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishNiuNiu->GetAttribute(TEXT("BeginWriteSec"), m_MiniGameConfig.niuniuConfig.BeginWriteSec))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("EndWriteSec"), m_MiniGameConfig.niuniuConfig.EndWriteSec))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("TableUpdateSec"), m_MiniGameConfig.niuniuConfig.UpdateSec))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("InitTableGlobel"), m_MiniGameConfig.niuniuConfig.InitGlobelSum))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("JoinRoomGlobel"), m_MiniGameConfig.niuniuConfig.JoinGlobelSum))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("MaxAddGlobel"), m_MiniGameConfig.niuniuConfig.MaxAddGlobel))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("ChangeGlobelRate"), m_MiniGameConfig.niuniuConfig.ChangeGlobelRate))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("JoinBankerGlobelSum"), m_MiniGameConfig.niuniuConfig.JoinBankerGlobelSum))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("GetNextBankerNeedGlobel"), m_MiniGameConfig.niuniuConfig.GetNextBankerNeedGlobel))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("BankerGameSum"), m_MiniGameConfig.niuniuConfig.BankerGameSum))
		return false;
	if (!pFishNiuNiu->GetAttribute(TEXT("VipNonBetSum"), m_MiniGameConfig.niuniuConfig.VipNonBetSum))
		return false;

	m_MiniGameConfig.niuniuConfig.NiuNiuLevelRate.clear();
	WHXmlNode* pFishNiuNiuLevel = pFishNiuNiu->GetChildNodeByName(TEXT("Level"));
	m_MiniGameConfig.niuniuConfig.MaxRate = 0;
	while (pFishNiuNiuLevel)
	{
		BYTE ID = 0;
		BYTE Rate = 0;
		if (!pFishNiuNiuLevel->GetAttribute(TEXT("ID"), ID))
			return false;
		if (!pFishNiuNiuLevel->GetAttribute(TEXT("Rate"), Rate))
			return false;
		m_MiniGameConfig.niuniuConfig.NiuNiuLevelRate.insert(HashMap<BYTE, BYTE>::value_type(ID,Rate));

		if (m_MiniGameConfig.niuniuConfig.MaxRate == 0 || m_MiniGameConfig.niuniuConfig.MaxRate < Rate)
			m_MiniGameConfig.niuniuConfig.MaxRate = Rate;

		pFishNiuNiuLevel = pFishNiuNiuLevel->GetNextSignelNode();
	}

	//2.Dial
	WHXmlNode* pFishDial = pFishMiniGame->GetChildNodeByName(TEXT("dial"));
	if (!pFishDial)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishDial->GetAttribute(TEXT("BeginWriteSec"), m_MiniGameConfig.dialConfig.BeginWriteSec))
		return false;
	if (!pFishDial->GetAttribute(TEXT("EndWriteSec"), m_MiniGameConfig.dialConfig.EndWriteSec))
		return false;
	if (!pFishDial->GetAttribute(TEXT("TableUpdateSec"), m_MiniGameConfig.dialConfig.UpdateSec))
		return false;
	if (!pFishDial->GetAttribute(TEXT("InitTableGlobel"), m_MiniGameConfig.dialConfig.InitGlobelSum))
		return false;
	if (!pFishDial->GetAttribute(TEXT("JoinRoomGlobel"), m_MiniGameConfig.dialConfig.JoinGlobelSum))
		return false;
	if (!pFishDial->GetAttribute(TEXT("MaxAddGlobel"), m_MiniGameConfig.dialConfig.MaxAddGlobel))
		return false;
	if (!pFishDial->GetAttribute(TEXT("ChangeGlobelRate"), m_MiniGameConfig.dialConfig.ChangeGlobelRate))
		return false;
	if (!pFishDial->GetAttribute(TEXT("JoinBankerGlobelSum"), m_MiniGameConfig.dialConfig.JoinBankerGlobelSum))
		return false;
	if (!pFishDial->GetAttribute(TEXT("GetNextBankerNeedGlobel"), m_MiniGameConfig.dialConfig.GetNextBankerNeedGlobel))
		return false;
	if (!pFishDial->GetAttribute(TEXT("BankerGameSum"), m_MiniGameConfig.dialConfig.BankerGameSum))
		return false;
	if (!pFishDial->GetAttribute(TEXT("VipNonBetSum"), m_MiniGameConfig.dialConfig.VipNonBetSum))
		return false;
	if (!pFishDial->GetAttribute(TEXT("MaxRate"), m_MiniGameConfig.dialConfig.MaxRate))
		return false;
	
	m_MiniGameConfig.dialConfig.DialAreaData.clear();
	m_MiniGameConfig.dialConfig.DialAreaData.resize(MAX_DIAL_GameSum);
	WHXmlNode* pFishDialLevel = pFishDial->GetChildNodeByName(TEXT("Area"));
	while (pFishDialLevel)
	{
		BYTE ID = 0;
		BYTE Value = 0;
		if (!pFishDialLevel->GetAttribute(TEXT("ID"), ID))
			return false;
		if (!pFishDialLevel->GetAttribute(TEXT("Value"), Value))
			return false;
		m_MiniGameConfig.dialConfig.DialAreaData[ID] = Value;
		pFishDialLevel = pFishDialLevel->GetNextSignelNode();
	}
	if (m_MiniGameConfig.dialConfig.DialAreaData.size() != MAX_DIAL_GameSum)
	{
		ASSERT(false);
		return false;
	}

	WHXmlNode* pTypeRates = pFishDial->GetChildNodeByName(TEXT("TypeRates"));
	if (!pTypeRates)
		return false;
	WHXmlNode* pTypeRate = pTypeRates->GetChildNodeByName(TEXT("TypeRate"));
	while (pTypeRate)
	{
		BYTE ID = 0;
		if (!pTypeRate->GetAttribute(TEXT("ID"), ID))
			return false;
		tagDialRate pRate;
		pRate.ID = ID;
		pRate.MaxRateSum = 0;
		WHXmlNode* pType = pTypeRate->GetChildNodeByName(TEXT("Type"));
		BYTE AllSum = 0;
		while (pType)
		{
			BYTE ID = 0;
			BYTE Rate = 0;
			if (!pType->GetAttribute(TEXT("ID"), ID))
				return false;
			if (!pType->GetAttribute(TEXT("Rate"), Rate))
				return false;
			if (!pType->GetAttribute(TEXT("Sum"), pRate.SumArray[ID]))
				return false;
			AllSum += pRate.SumArray[ID];
			pRate.RateArray[ID] = Rate;
			pRate.MaxRateSum += Rate;
			pType = pType->GetNextSignelNode();
		}
		if (AllSum != MAX_DIAL_GameSum)
		{
			return false;
		}
		m_MiniGameConfig.dialConfig.RateMap.push_back(pRate);
		pTypeRate = pTypeRate->GetNextSignelNode();
	}
	for (BYTE i = 0; i < GetFishMiNiGameConfig().dialConfig.RateMap.size(); ++i)
	{
		for (BYTE x = 0; x < 4; ++x)
		{
			BYTE Sum1 = GetFishMiNiGameConfig().dialConfig.RateMap[i].SumArray[x];
			BYTE Sum2 = GetFishMiNiGameConfig().dialConfig.RateMap[i].SumArray[x + 4];
			BYTE Sum3 = GetFishMiNiGameConfig().dialConfig.RateMap[i].SumArray[x + 8];
			BYTE Sum = Sum1 + Sum2 + Sum3;

			BYTE Check = 0;
			for (BYTE k = 0; k < MAX_DIAL_GameSum; ++k)
			{
				if (GetFishMiNiGameConfig().dialConfig.DialAreaData[k] % 4 == x % 4)
				{
					Check++;
				}
			}

			if (Check != Sum)
			{
				return false;
			}
		}
	}


	//3.Car
	//车牌游戏
	WHXmlNode* pFishCar = pFishMiniGame->GetChildNodeByName(TEXT("car"));
	if (!pFishCar)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishCar)
	{
		ASSERT(false);
		return false;
	}
	if (!pFishCar->GetAttribute(TEXT("BeginWriteSec"), m_MiniGameConfig.carConfig.BeginWriteSec))
		return false;
	if (!pFishCar->GetAttribute(TEXT("EndWriteSec"), m_MiniGameConfig.carConfig.EndWriteSec))
		return false;
	if (!pFishCar->GetAttribute(TEXT("TableUpdateSec"), m_MiniGameConfig.carConfig.UpdateSec))
		return false;
	if (!pFishCar->GetAttribute(TEXT("InitTableGlobel"), m_MiniGameConfig.carConfig.InitGlobelSum))
		return false;
	if (!pFishCar->GetAttribute(TEXT("JoinRoomGlobel"), m_MiniGameConfig.carConfig.JoinGlobelSum))
		return false;
	if (!pFishCar->GetAttribute(TEXT("MaxAddGlobel"), m_MiniGameConfig.carConfig.MaxAddGlobel))
		return false;
	if (!pFishCar->GetAttribute(TEXT("ChangeGlobelRate"), m_MiniGameConfig.carConfig.ChangeGlobelRate))
		return false;
	if (!pFishCar->GetAttribute(TEXT("JoinBankerGlobelSum"), m_MiniGameConfig.carConfig.JoinBankerGlobelSum))
		return false;
	if (!pFishCar->GetAttribute(TEXT("GetNextBankerNeedGlobel"), m_MiniGameConfig.carConfig.GetNextBankerNeedGlobel))
		return false;
	if (!pFishCar->GetAttribute(TEXT("BankerGameSum"), m_MiniGameConfig.carConfig.BankerGameSum))
		return false;
	if (!pFishCar->GetAttribute(TEXT("VipNonBetSum"), m_MiniGameConfig.carConfig.VipNonBetSum))
		return false;
	//Types
	WHXmlNode* pFishCarTypes = pFishCar->GetChildNodeByName(TEXT("Types"));
	if (!pFishCarTypes)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishCarType = pFishCarTypes->GetChildNodeByName(TEXT("Type"));
	while (pFishCarType)
	{
		tagCarType pType;
		if (!pFishCarType->GetAttribute(TEXT("ID"), pType.TypeID))
			return false;
		if (!pFishCarType->GetAttribute(TEXT("RateValue"), pType.RateValue))
			return false;
		if (!pFishCarType->GetAttribute(TEXT("RateF"), pType.RateF))
			return false;
		m_MiniGameConfig.carConfig.CarTypeMap.insert(HashMap<BYTE, tagCarType>::value_type(pType.TypeID,pType));
		pFishCarType = pFishCarType->GetNextSignelNode();
	}
	if (m_MiniGameConfig.carConfig.CarTypeMap.size() != MAX_CAR_ClientSum)
	{
		ASSERT(false);
		return false;
	}
	//Values
	WHXmlNode* pFishCarValues = pFishCar->GetChildNodeByName(TEXT("Values"));
	if (!pFishCarValues)
	{
		ASSERT(false);
		return false;
	}
	DWORD MaxRate = 0;
	WHXmlNode* pFishCarValue = pFishCarValues->GetChildNodeByName(TEXT("Value"));
	while (pFishCarValue)
	{
		tagCarValue pType;
		if (!pFishCarValue->GetAttribute(TEXT("ID"), pType.ID))
			return false;
		if (!pFishCarValue->GetAttribute(TEXT("TypeID"), pType.TypeID))
			return false;

		pType.MinRate = MaxRate;
		//进行权重 累加
		HashMap<BYTE, tagCarType>::iterator Iter =  m_MiniGameConfig.carConfig.CarTypeMap.find(pType.TypeID);
		if (Iter == m_MiniGameConfig.carConfig.CarTypeMap.end())
		{
			ASSERT(false);
			return false;
		}
		MaxRate += Iter->second.RateF;
		pType.MaxRate = MaxRate;

		m_MiniGameConfig.carConfig.CarValueVec.push_back(pType);


		pFishCarValue = pFishCarValue->GetNextSignelNode();
	}
	if (m_MiniGameConfig.carConfig.CarValueVec.size() < MAX_CAR_ClientSum)
	{
		ASSERT(false);
		return false;
	}
	//我们 重新计算下 全局的数据
	m_MiniGameConfig.carConfig.MaxRate = MaxRate;
	return true;
}
bool FishConfig::LoadFishGameRobotConfig(WHXmlNode* pFishConfig)
{
	//加载游戏机器人的一些配置文件数据
	if (!pFishConfig)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pFishRobot = pFishConfig->GetChildNodeByName(TEXT("FishRobot"));
	if (!pFishRobot)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pGameRobot = pFishRobot->GetChildNodeByName(TEXT("GameRobot"));
	if (!pGameRobot)
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pRobot = pGameRobot->GetChildNodeByName(TEXT("Robot"));
	while (pRobot)
	{
		tagGameRobotConfig pRobotConfig;

		if (!pRobot->GetAttribute(TEXT("RobotID"), pRobotConfig.RobotID))
			return false;
		if (!pRobot->GetAttribute(TEXT("MonthRobotSum"), pRobotConfig.MonthRobotSum))
			return false;
		if (!pRobot->GetAttribute(TEXT("AddGlobelSum"), pRobotConfig.AddGlobelSum))
			return false;
		if (!pRobot->GetAttribute(TEXT("InitGlobel"), pRobotConfig.InitGlobelSum))
			return false;
		if (!pRobot->GetAttribute(TEXT("InitCurrceySum"), pRobotConfig.InitCurrceySum))
			return false;
		if (!pRobot->GetAttribute(TEXT("AddCurrceySum"), pRobotConfig.AddCurrceySum))
			return false;
		if (!pRobot->GetAttribute(TEXT("MaxLevel"), pRobotConfig.MaxLevel))
			return false;
		if (!pRobot->GetAttribute(TEXT("MinLevel"), pRobotConfig.MinLevel))
			return false;
		if (!pRobot->GetAttribute(TEXT("GlobelRateF"), pRobotConfig.GlobelRateF))
			return false;
		if (!pRobot->GetAttribute(TEXT("CurrceyRateF"), pRobotConfig.CurrceyRateF))
			return false;
		//开炮控制
		WHXmlNode* pOpenFire = pRobot->GetChildNodeByName(TEXT("OpenFire"));
		if (!pOpenFire)
		{
			ASSERT(false);
			return false;
		}

		BYTE TempBool = 0;
		if (!pOpenFire->GetAttribute(TEXT("IsOpen"), TempBool))
			return false;
		pRobotConfig.OpenFireInfo.IsOpen = (TempBool == 1 ? true : false);

		if (!pOpenFire->GetAttribute(TEXT("LastOpenFireMinSec"), pRobotConfig.OpenFireInfo.FireInfo.OpenMinSec))
			return false;
		if (!pOpenFire->GetAttribute(TEXT("LastOpenFireMaxSec"), pRobotConfig.OpenFireInfo.FireInfo.OpenMaxSec))
			return false;
		if (!pOpenFire->GetAttribute(TEXT("StopFireMinSec"), pRobotConfig.OpenFireInfo.FireInfo.StopMinSec))
			return false;
		if (!pOpenFire->GetAttribute(TEXT("StopFireMaxSec"), pRobotConfig.OpenFireInfo.FireInfo.StopMaxSec))
			return false;

		//切换炮台控制
		WHXmlNode* pLauncher = pRobot->GetChildNodeByName(TEXT("Launcher"));
		if (!pLauncher)
		{
			ASSERT(false);
			return false;
		}

		if (!pLauncher->GetAttribute(TEXT("IsOpen"), TempBool))
			return false;
		pRobotConfig.LauncherInfo.IsOpen = (TempBool == 1 ? true : false);

		if (!pLauncher->GetAttribute(TEXT("ChangeLauncherMinSec"), pRobotConfig.LauncherInfo.LauncherInfo.OpenMinSec))
			return false;
		if (!pLauncher->GetAttribute(TEXT("ChangeLauncherMaxSec"), pRobotConfig.LauncherInfo.LauncherInfo.OpenMaxSec))
			return false;
		/*if (!pLauncher->GetAttribute(TEXT("ChangeLauncherCDMinSec"), pRobotConfig.LauncherInfo.LauncherInfo.StopMinSec))
			return false;
		if (!pLauncher->GetAttribute(TEXT("ChangeLauncherCDMaxSec"), pRobotConfig.LauncherInfo.LauncherInfo.StopMaxSec))
			return false;*/
		WHXmlNode*  pLauncherType = pLauncher->GetChildNodeByName(TEXT("Once"));
		pRobotConfig.LauncherInfo.MaxRateF = 0;
		while (pLauncherType)
		{
			tagRobotLauncherType pType;

			if (!pLauncherType->GetAttribute(TEXT("LauncherTypeID"), pType.LauncherType))
				return false;
			if (!pLauncherType->GetAttribute(TEXT("RateF"), pType.RateF))
				return false;

			pRobotConfig.LauncherInfo.MaxRateF += pType.RateF;
			pType.RateF = pRobotConfig.LauncherInfo.MaxRateF;
			pRobotConfig.LauncherInfo.LauncherVec.push_back(pType);
			pLauncherType = pLauncherType->GetNextSignelNode();
		}

		//Rate 倍率切换
		WHXmlNode* pRate = pRobot->GetChildNodeByName(TEXT("Rate"));
		if (!pRate)
		{
			ASSERT(false);
			return false;
		}

		if (!pRate->GetAttribute(TEXT("IsOpen"), TempBool))
			return false;
		pRobotConfig.RateInfo.IsOpen = (TempBool == 1 ? true : false);

		if (!pRate->GetAttribute(TEXT("ChangeRateMinSec"), pRobotConfig.RateInfo.RateInfo.OpenMinSec))
			return false;
		if (!pRate->GetAttribute(TEXT("ChangeRateMaxSec"), pRobotConfig.RateInfo.RateInfo.OpenMaxSec))
			return false;
		/*if (!pRate->GetAttribute(TEXT("ChangeRateCDMinSec"), pRobotConfig.RateInfo.RateInfo.StopMinSec))
			return false;
		if (!pRate->GetAttribute(TEXT("ChangeRateCDMaxSec"), pRobotConfig.RateInfo.RateInfo.StopMaxSec))
			return false;*/
		WHXmlNode*  pRateType = pRate->GetChildNodeByName(TEXT("Once"));
		pRobotConfig.RateInfo.MaxRateF = 0;
		while (pRateType)
		{
			tagRobotRateType pType;

			if (!pRateType->GetAttribute(TEXT("RateTypeID"), pType.RateType))
				return false;
			if (!pRateType->GetAttribute(TEXT("RateF"), pType.RateF))
				return false;

			pRobotConfig.RateInfo.MaxRateF += pType.RateF;
			pType.RateF = pRobotConfig.RateInfo.MaxRateF;
			pRobotConfig.RateInfo.RateVec.push_back(pType);
			pRateType = pRateType->GetNextSignelNode();
		}

		//Skill
		WHXmlNode* pSkill = pRobot->GetChildNodeByName(TEXT("Skill"));
		if (!pSkill)
		{
			ASSERT(false);
			return false;
		}

		if (!pSkill->GetAttribute(TEXT("IsOpen"), TempBool))
			return false;
		pRobotConfig.SkillInfo.IsOpen = (TempBool == 1 ? true : false);

		if (!pSkill->GetAttribute(TEXT("UseSkillMinSec"), pRobotConfig.SkillInfo.SkillInfo.OpenMinSec))
			return false;
		if (!pSkill->GetAttribute(TEXT("UseSkillMaxSec"), pRobotConfig.SkillInfo.SkillInfo.OpenMaxSec))
			return false;
	/*	if (!pSkill->GetAttribute(TEXT("SkillCDMinSec"), pRobotConfig.SkillInfo.SkillInfo.StopMinSec))
			return false;
		if (!pSkill->GetAttribute(TEXT("SkillCDMaxSec"), pRobotConfig.SkillInfo.SkillInfo.StopMaxSec))
			return false;*/

		//Room
		WHXmlNode* pRoom = pRobot->GetChildNodeByName(TEXT("Room"));
		if (!pRoom)
		{
			ASSERT(false);
			return false;
		}

		if (!pRoom->GetAttribute(TEXT("IsOpen"), TempBool))
			return false;
		pRobotConfig.RoomInfo.IsOpen = (TempBool == 1 ? true : false);

		if (!pRoom->GetAttribute(TEXT("JoinTableMinSec"), pRobotConfig.RoomInfo.RoomInfo.OpenMinSec))
			return false;
		if (!pRoom->GetAttribute(TEXT("JoinTableMaxSec"), pRobotConfig.RoomInfo.RoomInfo.OpenMaxSec))
			return false;
		if (!pRoom->GetAttribute(TEXT("WriteMinSec"), pRobotConfig.RoomInfo.RoomInfo.StopMinSec))
			return false;
		if (!pRoom->GetAttribute(TEXT("WriteMaxSec"), pRobotConfig.RoomInfo.RoomInfo.StopMaxSec))
			return false;

		//charm
		WHXmlNode* pCharm = pRobot->GetChildNodeByName(TEXT("Charm"));
		if (!pCharm)
		{
			ASSERT(false);
			return false;
		}
		if (!pCharm->GetAttribute(TEXT("IsOpen"), TempBool))
			return false;
		pRobotConfig.CharmInfo.IsOpen = (TempBool == 1 ? true : false);

		if (!pCharm->GetAttribute(TEXT("SendCharmMinCDSec"), pRobotConfig.CharmInfo.SendCharmMinCDSec))
			return false;
		if (!pCharm->GetAttribute(TEXT("SendCharmMaxCDSec"), pRobotConfig.CharmInfo.SendCharmMaxCDSec))
			return false;
		if (!pCharm->GetAttribute(TEXT("BeChangeCharmValue"), pRobotConfig.CharmInfo.BeChangeCharmValue))
			return false;
		if (!pCharm->GetAttribute(TEXT("BeChangeCharmMaxSec"), pRobotConfig.CharmInfo.BeChangeCharmSec))
			return false;

		if (!pCharm->GetAttribute(TEXT("NonRateF"), pRobotConfig.CharmInfo.NonRateF))
			return false;

		WHXmlNode*  pCharmSend = pCharm->GetChildNodeByName(TEXT("Send"));
		pRobotConfig.CharmInfo.SendInfo.dwSendCharmArrayMaxRateF = 0;
		while (pCharmSend)
		{
			tagRobotSendCharmInfo pInfo;

			if (!pCharmSend->GetAttribute(TEXT("CharmIndexID"), pInfo.CharmIndexID))
				return false;
			if (!pCharmSend->GetAttribute(TEXT("MinItemSum"), pInfo.MinItemSum))
				return false;
			if (!pCharmSend->GetAttribute(TEXT("MaxItemSum"), pInfo.MaxItemSum))
				return false;
			if (!pCharmSend->GetAttribute(TEXT("RateF"), pInfo.RateF))
				return false;

			pRobotConfig.CharmInfo.SendInfo.dwSendCharmArrayMaxRateF += pInfo.RateF;
			pInfo.RateF = pRobotConfig.CharmInfo.SendInfo.dwSendCharmArrayMaxRateF;
			pRobotConfig.CharmInfo.SendInfo.vecSendCharmArray.push_back(pInfo);

			pCharmSend = pCharmSend->GetNextSignelNode();
		}

		WHXmlNode*  pBeSend = pCharm->GetChildNodeByName(TEXT("BeSend"));
		while (pBeSend)
		{
			tagRobotBeSendCharm pBeSendInfo;
			if (!pBeSend->GetAttribute(TEXT("CharmIndexID"), pBeSendInfo.CharmIndexID))
				return false;

			if (!pBeSend->GetAttribute(TEXT("NonRateF"), pBeSendInfo.NonRateF))
				return false;

			WHXmlNode*  pCharmSend = pBeSend->GetChildNodeByName(TEXT("Send"));
			pBeSendInfo.SendInfo.dwSendCharmArrayMaxRateF = 0;
			while (pCharmSend)
			{
				tagRobotSendCharmInfo pInfo;

				if (!pCharmSend->GetAttribute(TEXT("CharmIndexID"), pInfo.CharmIndexID))
					return false;
				if (!pCharmSend->GetAttribute(TEXT("MinItemSum"), pInfo.MinItemSum))
					return false;
				if (!pCharmSend->GetAttribute(TEXT("MaxItemSum"), pInfo.MaxItemSum))
					return false;
				if (!pCharmSend->GetAttribute(TEXT("RateF"), pInfo.RateF))
					return false;

				pBeSendInfo.SendInfo.dwSendCharmArrayMaxRateF += pInfo.RateF;
				pInfo.RateF = pBeSendInfo.SendInfo.dwSendCharmArrayMaxRateF;
				pBeSendInfo.SendInfo.vecSendCharmArray.push_back(pInfo);

				pCharmSend = pCharmSend->GetNextSignelNode();
			}

			pRobotConfig.CharmInfo.BeSendInfoMap.insert(HashMap<BYTE, tagRobotBeSendCharm>::value_type(pBeSendInfo.CharmIndexID, pBeSendInfo));

			pBeSend = pBeSend->GetNextSignelNode();
		}

		WHXmlNode*  pCharmChange = pCharm->GetChildNodeByName(TEXT("CharmChangeEvent"));
		while (pCharmChange)
		{
			if (!pCharmChange->GetAttribute(TEXT("LeaveRoomRateF"), pRobotConfig.CharmInfo.CharmChangeEvent.LeaveRoomRateF))
				return false;

			if (!pCharmChange->GetAttribute(TEXT("NonRateF"), pRobotConfig.CharmInfo.CharmChangeEvent.NonRateF))
				return false;

			WHXmlNode*  pCharmAdd = pCharmChange->GetChildNodeByName(TEXT("Add"));
			if (!pCharmAdd)
				return false;
			{
				WHXmlNode*  pCharmSend = pCharmAdd->GetChildNodeByName(TEXT("Send"));
				pRobotConfig.CharmInfo.CharmChangeEvent.AddSendInfo.dwSendCharmArrayMaxRateF = 0;
				while (pCharmSend)
				{
					tagRobotSendCharmInfo pInfo;

					if (!pCharmSend->GetAttribute(TEXT("CharmIndexID"), pInfo.CharmIndexID))
						return false;
					if (!pCharmSend->GetAttribute(TEXT("MinItemSum"), pInfo.MinItemSum))
						return false;
					if (!pCharmSend->GetAttribute(TEXT("MaxItemSum"), pInfo.MaxItemSum))
						return false;
					if (!pCharmSend->GetAttribute(TEXT("RateF"), pInfo.RateF))
						return false;

					pRobotConfig.CharmInfo.CharmChangeEvent.AddSendInfo.dwSendCharmArrayMaxRateF += pInfo.RateF;
					pInfo.RateF = pRobotConfig.CharmInfo.CharmChangeEvent.AddSendInfo.dwSendCharmArrayMaxRateF;
					pRobotConfig.CharmInfo.CharmChangeEvent.AddSendInfo.vecSendCharmArray.push_back(pInfo);

					pCharmSend = pCharmSend->GetNextSignelNode();
				}
			}
			
			WHXmlNode*  pCharmDel = pCharmChange->GetChildNodeByName(TEXT("Del"));
			if (!pCharmDel)
				return false;
			{
				WHXmlNode*  pCharmSend = pCharmDel->GetChildNodeByName(TEXT("Send"));
				pRobotConfig.CharmInfo.CharmChangeEvent.DelSendInfo.dwSendCharmArrayMaxRateF = 0;
				while (pCharmSend)
				{
					tagRobotSendCharmInfo pInfo;

					if (!pCharmSend->GetAttribute(TEXT("CharmIndexID"), pInfo.CharmIndexID))
						return false;
					if (!pCharmSend->GetAttribute(TEXT("MinItemSum"), pInfo.MinItemSum))
						return false;
					if (!pCharmSend->GetAttribute(TEXT("MaxItemSum"), pInfo.MaxItemSum))
						return false;
					if (!pCharmSend->GetAttribute(TEXT("RateF"), pInfo.RateF))
						return false;

					pRobotConfig.CharmInfo.CharmChangeEvent.DelSendInfo.dwSendCharmArrayMaxRateF += pInfo.RateF;
					pInfo.RateF = pRobotConfig.CharmInfo.CharmChangeEvent.DelSendInfo.dwSendCharmArrayMaxRateF;
					pRobotConfig.CharmInfo.CharmChangeEvent.DelSendInfo.vecSendCharmArray.push_back(pInfo);

					pCharmSend = pCharmSend->GetNextSignelNode();
				}
			}

			pCharmChange = pCharmChange->GetNextSignelNode();
		}

		m_GameRobotConfig.RobotMap.insert(HashMap<DWORD, tagGameRobotConfig>::value_type(pRobotConfig.RobotID, pRobotConfig));
		pRobot = pRobot->GetNextSignelNode();
	}
	m_GameRobotConfig.RobotIndexMap.clear();
	//机器人映射表
	WHXmlNode* pOnceTable = pGameRobot->GetChildNodeByName(TEXT("OnceTable"));
	while (pOnceTable)
	{
		BYTE TableID = 0;
		BYTE MonthID = 0;
		if (!pOnceTable->GetAttribute(TEXT("TableID"), TableID))
			return false;
		if (!pOnceTable->GetAttribute(TEXT("MonthID"), MonthID))
			return false;
		DWORD Key = (TableID << 16) + MonthID;

		WHXmlNode* pRobot = pOnceTable->GetChildNodeByName(TEXT("Robot"));
		while (pRobot)
		{
			DWORD RobotID = 0;
			if (!pRobot->GetAttribute(TEXT("ID"), RobotID))
				return false;
			if (m_GameRobotConfig.RobotMap.count(RobotID) != 1)
				return false;
			m_GameRobotConfig.RobotIndexMap.insert(multimap<DWORD, DWORD>::value_type(Key, RobotID));
			pRobot = pRobot->GetNextSignelNode();
		}
		pOnceTable = pOnceTable->GetNextSignelNode();
	}
	return true;
}
DWORD FishConfig::GetWriteSec()
{
	return m_FishUpdateConfig.UpdateHour * 3600 + m_FishUpdateConfig.UpdateMin * 60;
}
BYTE FishConfig::GetItemType(DWORD ItemID)
{
	tagItemConfig* pConfig = GetItemInfo(ItemID);
	if (!pConfig)
		return 0;
	else
		return pConfig->ItemType;
}
DWORD FishConfig::GetItemParam(DWORD ItemID)
{
	tagItemConfig* pConfig = GetItemInfo(ItemID);
	if (!pConfig)
		return 0;
	else
		return pConfig->ItemParam;
}
tagItemConfig* FishConfig::GetItemInfo(DWORD ItemID)
{
	HashMap<DWORD, tagItemConfig>::iterator Iter = m_ItemMap.find(ItemID);
	if (Iter == m_ItemMap.end())
		return NULL;
	else
		return &Iter->second;
}
bool FishConfig::CheckVersionAndPathCrc(DWORD VersionID, DWORD PathCrc)
{
	if (GetSystemConfig().VersionID == 0 && GetSystemConfig().PathCrc == 0)
	{
		return true;
	}
	if (GetSystemConfig().VersionID > VersionID || GetSystemConfig().PathCrc > PathCrc)
		return false;
	else
		return true;
}
bool FishConfig::LoadFishErrorStringFile(const char* FilePath)
{
	//读取指定文件 将里面的屏蔽字全部读取出来
	m_ErrorStr.ErrorStrVec.clear();
	FILE* fp;
	errno_t Error= fopen_s(&fp, FilePath, "rb");
	if (Error != 0 || !fp)
	{
		ASSERT(false);
		return false;
	}
	fseek(fp, 0, SEEK_SET);

	char pSrcStr[512] = { 0 };

	while (!feof(fp))
	{
		fgets(pSrcStr, sizeof(pSrcStr), fp);//读取每一行
		//移除\n 换行符
		size_t Length = strlen(pSrcStr);
		if (pSrcStr[Length - 1] == '\n' || pSrcStr[Length - 1] == '\r')
		{
			pSrcStr[Length - 1] = '\0';
		}
		if (pSrcStr[Length - 2] == '\n' || pSrcStr[Length - 2] == '\r')
		{
			pSrcStr[Length - 2] = '\0';
		}
		//转化为 TCHAR
		UINT Count = 0;
		TCHAR* pDestChar = CharToWChar(pSrcStr,Count);
		if (Count == 0)
		{
			ASSERT(false);
			return false;
		}
		UINT uStrLength = _tcslen(pDestChar);
		if (uStrLength > MAX_ERRORSTR_LENGTH)
			continue;

		tagErrorStr pStr;
		TCHARCopy(pStr.ErrorStr, CountArray(pStr.ErrorStr), pDestChar, Count);
		m_ErrorStr.ErrorStrVec.push_back(pStr);

		free(pDestChar);
	}
	fclose(fp);
	return true;
}
bool FishConfig::CheckStringIsError(TCHAR* pStr,DWORD MinLength,DWORD MaxLength, StringCheckType pType)
{
	//空指针 和 长度过滤
	if (!pStr)
	{
		return false;
	}
	DWORD ArrayLength = _tcsclen(pStr);
	if (MinLength !=0 && ArrayLength == 0)
	{
		return false;
	}
	if (ArrayLength< MinLength || ArrayLength>MaxLength)
	{
		return false;
	}
	vector<tagErrorStr>::iterator Iter = m_ErrorStr.ErrorStrVec.begin();
	for (; Iter != m_ErrorStr.ErrorStrVec.end(); ++Iter)
	{
		if (GetStrIndeof(pStr, Iter->ErrorStr))
		{
			return false;
		}
	}
	//根据类型在进行过滤
	if (pType == SCT_AccountName || pType == SCT_Password)
	{
		//只可以包含 0-9  A-Z  _  a-z 这些数据 
		for (size_t i = 0; i < ArrayLength; ++i)
		{
			if (
				pStr[i] == 95 ||
				(pStr[i] >= 48 && pStr[i] <= 57) ||
				(pStr[i] >= 65 && pStr[i] <= 90) ||
				(pStr[i] >= 97 && pStr[i] <= 122)
				)
			{
				continue;
			}
			return false;
		}
		return true;
	}
	else if (pType == SCT_Normal)
	{
		return true;
	}
	else
		return false;
}
bool FishConfig::GetStrIndeof(TCHAR* pStr, TCHAR* FindStr)
{
	if (!pStr || !FindStr)
	{
		ASSERT(false);
		return false;
	}
	if (_tcsstr(pStr, FindStr) != null)
		return true;
	else
		return false;
}

void FishConfig::TCHAR2STRING(const TCHAR *STR, std::string& out)
{

	int iLen = WideCharToMultiByte(CP_ACP, 0,STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
	out = chRtn;
	
}
bool FishConfig::LoadWord()
{
	WHXml pXml;
	if (!pXml.LoadXMLFilePath(TEXT("FishServerCharactersConfig.xml")))
	{
		ASSERT(false);
		return false;
	}

	WCHAR temp_characters[1024];
	WHXmlNode* pInfo = pXml.GetChildNodeByName(TEXT("Info"));
	while (pInfo)
	{
		int ID = 0;
		if (pInfo->GetAttribute(TEXT("ID"), ID) == false)
		{
			ASSERT(false);
			return false;
		}

		if (!pInfo->GetAttribute(TEXT("Characters"), temp_characters, CountArray(temp_characters)))
			return false;
		std::wstring str = temp_characters;
		m_configCharacters[ID] = str;
	}
	return true;
}

const WCHAR* FishConfig::GetConfigCharacters(int id)
{
	std::wstring entry;
	HashMap<DWORD, std::wstring>::iterator it = m_configCharacters.find(id);
	if (it != m_configCharacters.end())
	{
		return it->second.c_str();
	}
	return NULL;
}

bool FishConfig::LoadGMToolConfig(const TCHAR* FilePath)
{
	WHXml pXml;
	if (!pXml.LoadXMLFilePath(FilePath))
	{
		ASSERT(false);
		return false;
	}
	m_GMTool.PassWord_ = "";
	m_GMTool.Account_ = "";
	TCHAR sz[1024];
	WHXmlNode* pGMTool = pXml.GetChildNodeByName(TEXT("GMToolClient"));
	if (pGMTool)
	{
		if (pGMTool->GetAttribute(TEXT("NetworkID"), m_GMTool.NetID_) == false)
		{
			ASSERT(false);
			return false;
		}

		
		if (pGMTool->GetAttribute(TEXT("password"), sz, sizeof(sz) / 3) == false)
		{
			ASSERT(false);
			return false;
		}
		
	    TCHAR2STRING(sz, m_GMTool.PassWord_);
		if (pGMTool->GetAttribute(TEXT("account"), sz, sizeof(sz) / 3) == false)
		{
			ASSERT(false);
			return false;
		}
		
		TCHAR2STRING(sz, m_GMTool.Account_);
	}
	else
	{
		return false;
	}
	return true;
}

bool FishConfig::LoadFishNoticeConfig(const TCHAR* FilePath)
{
	WHXml pXml;
	if (!pXml.LoadXMLFilePath(FilePath))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pNotices = pXml.GetChildNodeByName(TEXT("Notices"));
	if (!pNotices)
	{
		ASSERT(false);
		return false;
	}
	m_NoticeMap.clear();
	WHXmlNode* pNotice = pNotices->GetChildNodeByName(TEXT("Notice"));
	DWORD Index = 1;
	while (pNotice)
	{
		tagNotice pNoticeOnce;
		//单条公告的读取
		WORD BeginYear = 0;
		if (!pNotice->GetAttribute(TEXT("BeginYear"), BeginYear))
			return false;
		WORD BeginMonth = 0;
		if (!pNotice->GetAttribute(TEXT("BeginMonth"), BeginMonth))
			return false;
		WORD BeginDay = 0;
		if (!pNotice->GetAttribute(TEXT("BeginDay"), BeginDay))
			return false;
		WORD BeginHour = 0;
		if (!pNotice->GetAttribute(TEXT("BeginHour"), BeginHour))
			return false;
		WORD BeginMin = 0;
		if (!pNotice->GetAttribute(TEXT("BeginMin"), BeginMin))
			return false;
		tm pTime;
		pTime.tm_year = BeginYear - 1900;
		pTime.tm_mon = BeginMonth - 1;
		pTime.tm_mday = BeginDay;
		pTime.tm_hour = BeginHour;	
		pTime.tm_min = BeginMin;
		pTime.tm_sec = 0;
		pNoticeOnce.BeginTime = mktime(&pTime);
		//
		if (!pNotice->GetAttribute(TEXT("StepNum"), pNoticeOnce.StepNum))
			return false;
		if (!pNotice->GetAttribute(TEXT("StepMin"), pNoticeOnce.StepMin))
			return false;
		if (!pNotice->GetAttribute(TEXT("MessageInfo"), pNoticeOnce.MessageInfo, CountArray(pNoticeOnce.MessageInfo)))
			return false;
		if (!pNotice->GetAttribute(TEXT("OnceStepNum"), pNoticeOnce.OnceStepNum))
			return false;
		if (!pNotice->GetAttribute(TEXT("OnceStepSec"), pNoticeOnce.OnceStepSec))
			return false;
		if (!pNotice->GetAttribute(TEXT("MessageInfoColor"), pNoticeOnce.MessageInfoColor))
			return false;
		pNoticeOnce.ID = Index;
		m_NoticeMap.insert(HashMap<DWORD, tagNotice>::value_type(Index, pNoticeOnce));

		pNotice = pNotice->GetNextSignelNode();
		++Index;
	}
	return true;
}

bool FishConfig::LoadRobotConfig(const TCHAR* FilePath)
{
	WHXml pXml;
	if (!pXml.LoadXMLFilePath(FilePath))
	{
		ASSERT(false);
		return false;
	}
	WHXmlNode* pconfig = pXml.GetChildNodeByName(TEXT("config"));
	if (!pconfig)
	{
		ASSERT(false);
		return false;
	}

	{//load xo
		WHXmlNode* pRobot = pconfig->GetChildNodeByName(TEXT("ox"));

		m_MiniGameConfig.niuniuConfig.VecRobotCount.clear();
		WHXmlNode* pCount = pRobot->GetChildNodeByName(TEXT("RobotCountControl"));
		while (pCount)
		{
			RobotOxCountControl item;
			if (!pCount->GetAttribute(TEXT("StartTime"), item.StartTime))
				return false;
			if (!pCount->GetAttribute(TEXT("EndTime"), item.EndTime))
				return false;
			if (!pCount->GetAttribute(TEXT("RobotCountMin"), item.RobotCountMin))
				return false;
			if (!pCount->GetAttribute(TEXT("RobotCountMax"), item.RobotCountMax))
				return false;
			pCount = pCount->GetNextSignelNode();
			m_MiniGameConfig.niuniuConfig.VecRobotCount.push_back(item);
		}

		if (!pRobot->GetAttribute(TEXT("RobotCountInterval"), m_MiniGameConfig.niuniuConfig.RobotCountInterval))
			return false;

		if (!pRobot->GetAttribute(TEXT("RobotMinGameTime"), m_MiniGameConfig.niuniuConfig.RobotMinGameTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMaxGameTime"), m_MiniGameConfig.niuniuConfig.RobotMaxGameTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotActionRate"), m_MiniGameConfig.niuniuConfig.RobotActionRate))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerRate"), m_MiniGameConfig.niuniuConfig.RobotBankerRate))
			return false;

		if (!pRobot->GetAttribute(TEXT("RobotWaitBankerMin"), m_MiniGameConfig.niuniuConfig.RobotWaitBankerMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotWaitBankerMax"), m_MiniGameConfig.niuniuConfig.RobotWaitBankerMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerCountMin"), m_MiniGameConfig.niuniuConfig.RobotBankerCountMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerCountMax"), m_MiniGameConfig.niuniuConfig.RobotBankerCountMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotListCountMin"), m_MiniGameConfig.niuniuConfig.RobotListCountMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotListCountMax"), m_MiniGameConfig.niuniuConfig.RobotListCountMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotListCountInterval"), m_MiniGameConfig.niuniuConfig.RobotListCountInterval))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMinBetTime"), m_MiniGameConfig.niuniuConfig.RobotMinBetTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMaxBetTime"), m_MiniGameConfig.niuniuConfig.RobotMaxBetTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMinJetton"), m_MiniGameConfig.niuniuConfig.RobotMinJetton))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMaxJetton"), m_MiniGameConfig.niuniuConfig.RobotMaxJetton))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotAreaLimit"), m_MiniGameConfig.niuniuConfig.RobotAreaLimit))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerScoreMin"), m_MiniGameConfig.niuniuConfig.RobotBankerScoreMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerScoreMax"), m_MiniGameConfig.niuniuConfig.RobotBankerScoreMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotPlayerScoreMin"), m_MiniGameConfig.niuniuConfig.RobotPlayerScoreMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotPlayerScoreMax"), m_MiniGameConfig.niuniuConfig.RobotPlayerScoreMax))
			return false;
	}

	{//load animal
		WHXmlNode* pRobot = pconfig->GetChildNodeByName(TEXT("animal"));

		m_MiniGameConfig.dialConfig.VecRobotCount.clear();
		WHXmlNode* pCount = pRobot->GetChildNodeByName(TEXT("RobotCountControl"));
		while (pCount)
		{
			RobotAnimalCountControl item;
			if (!pCount->GetAttribute(TEXT("StartTime"), item.StartTime))
				return false;
			if (!pCount->GetAttribute(TEXT("EndTime"), item.EndTime))
				return false;
			if (!pCount->GetAttribute(TEXT("RobotCountMin"), item.RobotCountMin))
				return false;
			if (!pCount->GetAttribute(TEXT("RobotCountMax"), item.RobotCountMax))
				return false;
			pCount = pCount->GetNextSignelNode();
			m_MiniGameConfig.dialConfig.VecRobotCount.push_back(item);
		}

		if (!pRobot->GetAttribute(TEXT("RobotCountInterval"), m_MiniGameConfig.dialConfig.RobotCountInterval))
			return false;

		if (!pRobot->GetAttribute(TEXT("RobotMinGameTime"), m_MiniGameConfig.dialConfig.RobotMinGameTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMaxGameTime"), m_MiniGameConfig.dialConfig.RobotMaxGameTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotActionRate"), m_MiniGameConfig.dialConfig.RobotActionRate))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerRate"), m_MiniGameConfig.dialConfig.RobotBankerRate))
			return false;

		if (!pRobot->GetAttribute(TEXT("RobotWaitBankerMin"), m_MiniGameConfig.dialConfig.RobotWaitBankerMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotWaitBankerMax"), m_MiniGameConfig.dialConfig.RobotWaitBankerMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerCountMin"), m_MiniGameConfig.dialConfig.RobotBankerCountMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerCountMax"), m_MiniGameConfig.dialConfig.RobotBankerCountMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotListCountMin"), m_MiniGameConfig.dialConfig.RobotListCountMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotListCountMax"), m_MiniGameConfig.dialConfig.RobotListCountMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotListCountInterval"), m_MiniGameConfig.dialConfig.RobotListCountInterval))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMinBetTime"), m_MiniGameConfig.dialConfig.RobotMinBetTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMaxBetTime"), m_MiniGameConfig.dialConfig.RobotMaxBetTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMinJetton"), m_MiniGameConfig.dialConfig.RobotMinJetton))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMaxJetton"), m_MiniGameConfig.dialConfig.RobotMaxJetton))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotAreaLimit"), m_MiniGameConfig.dialConfig.RobotAreaLimit))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerScoreMin"), m_MiniGameConfig.dialConfig.RobotBankerScoreMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerScoreMax"), m_MiniGameConfig.dialConfig.RobotBankerScoreMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotPlayerScoreMin"), m_MiniGameConfig.dialConfig.RobotPlayerScoreMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotPlayerScoreMax"), m_MiniGameConfig.dialConfig.RobotPlayerScoreMax))
			return false;
	}

	{//load car
		WHXmlNode* pRobot = pconfig->GetChildNodeByName(TEXT("car"));

		m_MiniGameConfig.carConfig.VecRobotCount.clear();
		WHXmlNode* pCount = pRobot->GetChildNodeByName(TEXT("RobotCountControl"));
		while (pCount)
		{
			RobotCarCountControl item;
			if (!pCount->GetAttribute(TEXT("StartTime"), item.StartTime))
				return false;
			if (!pCount->GetAttribute(TEXT("EndTime"), item.EndTime))
				return false;
			if (!pCount->GetAttribute(TEXT("RobotCountMin"), item.RobotCountMin))
				return false;
			if (!pCount->GetAttribute(TEXT("RobotCountMax"), item.RobotCountMax))
				return false;
			pCount = pCount->GetNextSignelNode();
			m_MiniGameConfig.carConfig.VecRobotCount.push_back(item);
		}

		if (!pRobot->GetAttribute(TEXT("RobotCountInterval"), m_MiniGameConfig.carConfig.RobotCountInterval))
			return false;

		if (!pRobot->GetAttribute(TEXT("RobotMinGameTime"), m_MiniGameConfig.carConfig.RobotMinGameTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMaxGameTime"), m_MiniGameConfig.carConfig.RobotMaxGameTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotActionRate"), m_MiniGameConfig.carConfig.RobotActionRate))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerRate"), m_MiniGameConfig.carConfig.RobotBankerRate))
			return false;

		if (!pRobot->GetAttribute(TEXT("RobotWaitBankerMin"), m_MiniGameConfig.carConfig.RobotWaitBankerMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotWaitBankerMax"), m_MiniGameConfig.carConfig.RobotWaitBankerMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerCountMin"), m_MiniGameConfig.carConfig.RobotBankerCountMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerCountMax"), m_MiniGameConfig.carConfig.RobotBankerCountMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotListCountMin"), m_MiniGameConfig.carConfig.RobotListCountMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotListCountMax"), m_MiniGameConfig.carConfig.RobotListCountMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotListCountInterval"), m_MiniGameConfig.carConfig.RobotListCountInterval))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMinBetTime"), m_MiniGameConfig.carConfig.RobotMinBetTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMaxBetTime"), m_MiniGameConfig.carConfig.RobotMaxBetTime))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMinJetton"), m_MiniGameConfig.carConfig.RobotMinJetton))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotMaxJetton"), m_MiniGameConfig.carConfig.RobotMaxJetton))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotAreaLimit"), m_MiniGameConfig.carConfig.RobotAreaLimit))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerScoreMin"), m_MiniGameConfig.carConfig.RobotBankerScoreMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotBankerScoreMax"), m_MiniGameConfig.carConfig.RobotBankerScoreMax))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotPlayerScoreMin"), m_MiniGameConfig.carConfig.RobotPlayerScoreMin))
			return false;
		if (!pRobot->GetAttribute(TEXT("RobotPlayerScoreMax"), m_MiniGameConfig.carConfig.RobotPlayerScoreMax))
			return false;
	}
	return true;
}