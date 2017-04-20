#pragma once
#include "Stdafx.h"
#include <map>
#include <vector>
struct tagRankInfo;
class RankManager
{
public:
	RankManager();
	virtual ~RankManager();
	void OnInit(bool IsNeedLoadNowDayRank);
	void OnLoadRankInfoByDB();
	void OnLoadRankInfoResult(DBO_Cmd_LoadRankInfo* pDB);
	void OnDayChange();
private:
	void SaveNowDayRankInfo();
	void SaveFileToFtp(const char* FilePath,const char* FileName);
private:
	//当天的排行榜数据
	int													m_LoadSum;//用于判断排行榜是否读取完毕了
	HashMap<BYTE, std::vector<tagRankInfo>>				m_RankArray;//加载后不可能进行改变的数据
};