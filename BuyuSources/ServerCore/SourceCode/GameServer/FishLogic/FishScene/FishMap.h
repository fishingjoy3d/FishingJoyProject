#pragma once
#include "FishResData.h"
#include <map>
#include <vector>
#include "TArray.h"
using namespace std;

struct FishPackageData
{
	float	StartSeconds;		//起始时间
	float	EndSeconds;			//结束时间
	float	IntervalSeconds;	//每次出现的间隔
	USHORT	Chance;				//机率
	USHORT	MaxCount;			//红包最大次数
	BYTE	PackageType;		//红包类型
	vector<BYTE> FishGroup;
};
struct FlowIndexData
{
	USHORT	FlowIdx;
	bool	LastIdx;
};
struct FlowOrderData
{
	vector<USHORT>	OrderIndex;
	vector<USHORT>	FlowIndex;
};
struct FishMapData
{
	WCHAR			FileName[MAX_MAP_NAME];		//地图文件名称
	WCHAR			MapName[MAX_MAP_NAME];		//地图名称
	bool			Repeat;						//是否所有流程完成之后重新开始
	BYTE			BackgroudImage;				//背景图片
	float			MaxTime;					//整个流程走完的时间
	vector<USHORT>	FLowList;					//流程
	vector<FishPackageData> PackageList;		//红包
	FlowOrderData	FlowOrderData;
};
typedef HashMap<UINT, FishMapData*> FISHMAP;
typedef vector<FlowIndexData*> FlowDataList;
class FishMap
{
public:
	FishMap();
	~FishMap();
	
	static bool LoadAllMap(const WCHAR *pcDir);
	static void ReleaseAllMap();
	bool IsLoadMap()const
	{
		return m_pMap != NULL;
	}
	bool LoadMap(const WCHAR *mapName);
	void ReleaseMap();
	USHORT GetFlowIndex(bool &bFixedGroup);
	bool HasFlowIndex()
	{
		return m_FlowList.size() > 0;
	}
	void Reset();
	bool IsRepeat()
	{
		return m_pMap->Repeat;
	}
	//返回-1表示不出红包
	UINT Update(float deltaTime);

	float MaxTime()const
	{
		return m_pMap->MaxTime;
	}
	byte BackgroundType()const
	{
		return m_pMap->BackgroudImage;
	}
	const WCHAR *GetMapName()const
	{
		return m_pMap->MapName;
	}
	static FISHMAP& GetAllMapList()
	{
		return ms_AllMapList;
	}
protected:
	byte GetPackageGroupID(FishPackageData &fpd);
	int				m_nIndex;
	vector<USHORT>	m_PackageCount;
	byte			m_LastPackageGroupID;
	float			m_Time;
	float			m_Interval;
	FishMapData		*m_pMap;
	FlowDataList	m_FlowList;
	FlowDataList	m_FlowData;
	static FISHMAP	ms_AllMapList;

};