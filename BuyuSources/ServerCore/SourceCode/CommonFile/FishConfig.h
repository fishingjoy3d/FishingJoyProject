//捕鱼的配置文件的集中读取
#pragma once
#include "Stdafx.h"
#include "TinyXml\XmlReader.h"
#include <set>
//#define MAX_Group 256
enum StringCheckType
{
	SCT_AccountName = 1,
	SCT_Password    = 2,
	SCT_Normal		= 3,
};


struct tgGMToolConfig
{
	int NetID_;
	std::string PassWord_;
	std::string Account_;
};

class FishConfig
{
public:
	FishConfig();
	virtual ~FishConfig();

	bool LoadConfigFilePath();
	void OnDestroy();
	void TCHAR2STRING(TCHAR *STR, std::string& out);


	RoleCheckConfig& GetCheckConfig(){ return m_CheckConfig; }
	tagTaskMap&		 GetTaskConfig(){ return m_TaskConfig; }
	tagAchievementMap& GetAchievementConfig(){ return m_AchievementConfig; }
	tagMonthMap& GetMonthConfig(){ return m_MonthMap; }
	tagRankMap& GetRankConfig(){ return m_RankMap; }
	tagChestMap& GetChestConfig(){ return m_ChestMap; }
	tagCharmConfig& GetCharmConfig(){ return m_CharmMap; };
	tagShopConfigMap& GetShopConfig(){ return m_ShopMap; }
	int GetCharmValue(DWORD pInfo[MAX_CHARM_ITEMSUM]);
	tagActionGroupMap& GetActionConfig(){ return m_ActionConfig; }
	tagGiffConfig& GetGiffConfig(){ return m_GiffConfig; }
	tagTableConfig& GetTableConfig(){ return m_TableConfig; }
	//BYTE GetTaskIDByGroupID(BYTE GroupID){ return m_TaskGroupArray[GroupID]; }
	tagRelationConfig& GetRelation(){ return m_RelationConfig; }
	tagItemConvertMap& GetItemConvertConfig(){ return m_ItemConvertConfig; }
	tagFishServerUpdate& GetFishUpdateConfig(){ return m_FishUpdateConfig; }
	tagMailConfig& GetFishMailConfig(){ return m_FishMailConfig; }
	//tagGlobelShop& GetGlobelShopConfig(){ return m_GlobelShopConfig; }
	tagOnlineReward& GetOnlineRewardConfig(){ return m_OnlineRewardConfig; }
	tagFishPckageMap& GetFishPackageConfig(){ return m_FishPackageConfig; }
	bool ItemIsExists(DWORD ItemID){ return m_ItemMap.count(ItemID) == 1; }
	BYTE GetItemType(DWORD ItemID);
	DWORD GetItemParam(DWORD ItemID);
	tagItemConfig* GetItemInfo(DWORD ItemID);
	bool TitleIsExists(BYTE TitleID){ return m_TitleSet.count(TitleID) == 1; }
	tagRewardMap& GetFishRewardConfig(){ return m_FishRewardConfig; }
	tagFishSystemInfo& GetSystemConfig(){ return m_SystemConfig; }
	tagFishRechargesMap& GetFishRechargesConfig(){ return m_FishRechargeMap; }
	LevelRewardMap& GetFishLevelRewardConfig(){ return m_LevelRewardMap; }
	tagExChangeMap& GetExChangeMap(){ return m_ExChangeMap; }
	tagRoleProtectConfig& GetRoleProtectConfig(){ return m_RoleProctectMap; }
	tagLotteryConfig& GetLotteryConfig(){ return m_LotteryMap; }
	tagVipConfig& GetVipConfig(){ return m_VipMap; }
	tagMonthCardConfig& GetMonthCardConfig(){ return m_MonthCardMap; }
	tagFishDropConfig& GetFishDropConfig(){ return m_FishDropConfig; }
	tagMiNiGameConfig& GetFishMiNiGameConfig(){ return m_MiniGameConfig; }
	tagGameRobotInfo& GetFishGameRobotConfig(){ return m_GameRobotConfig; }
	
	tgGMToolConfig& GetGMToolConfig() { return m_GMTool; }
	DWORD GetWriteSec();
	bool CheckVersionAndPathCrc(DWORD VersionID, DWORD PathCrc);

	bool CheckStringIsError(TCHAR* pStr, DWORD MinLength, DWORD MaxLength, StringCheckType pType);

	bool LoadFishNoticeConfig(const TCHAR* FilePath);
	bool LoadGMToolConfig(const TCHAR* FilePath);
	bool LoadRobotConfig(const TCHAR* FilePath);
	HashMap<DWORD, tagNotice>& GetAllNoticeInfo(){ return m_NoticeMap; }
	const HashMap<DWORD, tagItemConfig>& GetAllItemConfigs() { return m_ItemMap; }
private:
	//void OnCreateGlobelTaskInfo();
	bool LoadFishCheck(WHXmlNode* pFishConfig);//加载签到的数据
	bool LoadFishTask(WHXmlNode* pFishConfig);
	bool LoadFishAchievement(WHXmlNode* pFishConfig);
	bool LoadFishMonth(WHXmlNode* pFishConfig);
	bool LoadFishTitle(WHXmlNode* pFishConfig);
	bool LoadFishRank(WHXmlNode* pFishConfig);
	bool LoadFishChest(WHXmlNode* pFishConfig);
	bool LoadFishCharm(WHXmlNode* pFishConfig);
	bool LoadFishShop(WHXmlNode* pFishConfig);
	bool LoadFishAction(WHXmlNode* pFishConfig);
	bool LoadFishGiff(WHXmlNode* pFishConfig);
	bool LoadFishTableConfig(WHXmlNode* pFishConfig);
	bool LoadFishRelationConfig(WHXmlNode* pFishConfig);
	bool LoadFishItemConvertConfig(WHXmlNode* pFishConfig);
	bool LoadFishUpdateConfig(WHXmlNode* pFishConfig);
	bool LoadFishMailConfig(WHXmlNode* pFishConfig);
	//bool LoadFishGlobelShopConfig(WHXmlNode* pFishConfig);
	bool LoadFishOnlineRewardConfig(WHXmlNode* pFishConfig);
	bool LoadFishItemConfig(WHXmlNode* pFishConfig);
	bool LoadFishPackageConfig(WHXmlNode* pFishConfig);
	bool LoadFishRewardConfig(WHXmlNode* pFishConfig);
	bool LoadFishSystemConfig(WHXmlNode* pFishConfig);
	bool LoadFishRechargeConfig(WHXmlNode* pFishConfig);
	bool LoadFishLevelReward(WHXmlNode* pFishConfig);
	bool LoadFishExChangeConfig(WHXmlNode* pFishConfig);
	bool LoadFishErrorStringFile(const char* FilePath);
	bool GetStrIndeof(TCHAR* pStr, TCHAR* FindStr);
	bool LoadFishRoleProtectConfig(WHXmlNode* pFishConfig);
	bool LoadFishLotteryConfig(WHXmlNode* pFishConfig);
	bool LoadFishMonthCardConfig(WHXmlNode* pFishConfig);
	bool LoadFishVipConfig(WHXmlNode* pFishConfig);
	bool LoadFishDropConfig(WHXmlNode* pFishConfig);
	bool LoadFishMiniGameConfig(WHXmlNode* pFishConfig);
	bool LoadFishGameRobotConfig(WHXmlNode* pFishConfig);
private:
	RoleCheckConfig				m_CheckConfig;
	tagTaskMap					m_TaskConfig;//任务配置文件
	tagAchievementMap			m_AchievementConfig;
	tagMonthMap					m_MonthMap;
	std::set<BYTE>				m_TitleSet;
	tagRankMap					m_RankMap;
	tagChestMap					m_ChestMap;
	tagCharmConfig				m_CharmMap;
	tagShopConfigMap			m_ShopMap;
	tagActionGroupMap			m_ActionConfig;
	tagGiffConfig				m_GiffConfig;
	tagTableConfig				m_TableConfig;
	tagRelationConfig			m_RelationConfig;
	tagItemConvertMap			m_ItemConvertConfig;
	tagFishServerUpdate			m_FishUpdateConfig;
	tagMailConfig				m_FishMailConfig;
	//tagGlobelShop				m_GlobelShopConfig;
	tagOnlineReward				m_OnlineRewardConfig;
	tagFishPckageMap			m_FishPackageConfig;
	tagRewardMap				m_FishRewardConfig;
	HashMap<DWORD, tagItemConfig>	m_ItemMap;
	tagFishSystemInfo			m_SystemConfig;
	tagFishRechargesMap			m_FishRechargeMap;
	//std::vector<TCHAR*>			m_ErrorStr;//屏蔽字
	tagErrorMap					m_ErrorStr;

	LevelRewardMap				m_LevelRewardMap;
	tagExChangeMap				m_ExChangeMap;
	tagRoleProtectConfig		m_RoleProctectMap;
	tagLotteryConfig			m_LotteryMap;
	tagVipConfig				m_VipMap;
	tagMonthCardConfig			m_MonthCardMap;
	tagFishDropConfig			m_FishDropConfig;

	tagMiNiGameConfig			m_MiniGameConfig;

	tagGameRobotInfo			m_GameRobotConfig;
	tgGMToolConfig				m_GMTool;
	HashMap<DWORD,tagNotice>	m_NoticeMap;//公告的结构
};