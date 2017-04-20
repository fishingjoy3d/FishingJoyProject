#pragma once
#include "FishResData.h"
enum
{
	FILE_END_MAGIC = 732425123
};
class FishResManager
{
public:
	static FishResManager* Inst();

	bool _OldInit(const WCHAR *pcDir);
	bool Init(const WCHAR *pcDir);
	void Shutdown();
	UINT GetGroupCount()
	{
		return m_GroupList.size();
	}
	GroupData* GetGroup(UINT idx)
	{
		if (idx >= m_GroupList.size())
		{
			InnerFishLog(L"鱼群索引不正确:%d, %d", idx, m_GroupList.size());
			return NULL;
		}
		else
			return m_GroupList[idx];
	}
	FlowData* GetFlow(UINT idx)
	{
		if (idx >= m_FlowList.size())
		{
			InnerFishLog(L"流程索引不正确:%d, %d", idx, m_FlowList.size());
			return NULL;
		}
		else
			return m_FlowList[idx];
	}
	PathData* GetPath(UINT idx)
	{
		if (idx >= m_PathList.size())
		{
			InnerFishLog(L"路径索引不正确:%d, %d", idx, m_PathList.size());
			return NULL;
		}
		else
			return m_PathList[idx];
	}
	PathData* GetPathInv(UINT idx)
	{
		if (idx >= m_PathInvList.size())
		{
			InnerFishLog(L"路径索引不正确:%d, %d", idx, m_PathInvList.size());
			return NULL;
		}
		else
			return m_PathInvList[idx];
	}
	bool HasAttackAction(USHORT fishIdx)
	{
		return m_FishDataList[fishIdx]->ClipLength[CLIP_GONGJI] != 0;
	}
	FishData* GetFishData(UINT idx)
	{
		if (idx >= m_FishDataList.size())
		{
			InnerFishLog(L"鱼数据索引不正确:%d, %d", idx, m_FishDataList.size());
			return NULL;
		}
		else
			return m_FishDataList[idx];
	}
	PathGroupData* GetPathGroup(UINT idx)
	{
		if (idx >= m_PathGroupList.size())
		{
			InnerFishLog(L"路径群索引不正确:%d, %d", idx, m_PathGroupList.size());
			return NULL;
		}
		else
			return m_PathGroupList[idx];
	}
	PathGroupData* GetPathGroupInv(UINT idx)
	{
		if (idx >= m_PathGroupInvList.size())
		{
			InnerFishLog(L"路径群索引不正确:%d, %d", idx, m_PathGroupInvList.size());
			return NULL;
		}
		else
			return m_PathGroupInvList[idx];
	}
	const DeadStayTime &GetDeadStayTime()const
	{
		return m_DeadStayTime;
	}
	UINT GetPathCount()const
	{
		return m_PathList.size();
	}
	UINT GetPathGroupCount()const
	{
		return m_PathGroupList.size();
	}
	void SetDeadStayTime(DeadStayTime &dst)
	{
		m_DeadStayTime = dst;
	}
	UINT GetFishDataCount()const
	{
		return m_FishDataList.size();
	}
	PathData* GetBoLang()
	{
		return m_BoLang;
	}
	PathData* GetDouDong()
	{
		return m_DouDong;
	}
	PathGroupData* GetLongJuanFeng()
	{
		return m_LongJuanFeng;
	}
	UINT GetFishDataCrc()
	{
		return m_FishDataCrc;
	}
protected:
	bool InitGroup(const WCHAR *pcDir);
	bool InitFlow(const WCHAR *pcDir);
	bool InitPath(const WCHAR *pcDir);
	bool InitFishData(const WCHAR *pcDir);
	bool InitPathGroup(const WCHAR *pcDir);
	bool InitDeadStayTime(const WCHAR *pcDir);
	bool InitSkillPath(const WCHAR *pcDir);
	bool InitNewData(const WCHAR *pcDir);
	UINT				m_FishDataCrc;
	DeadStayTime		m_DeadStayTime;
	vector<PathGroupData*> m_PathGroupList;
	vector<PathGroupData*> m_PathGroupInvList;
	vector<GroupData*>	m_GroupList;
	vector<FlowData*>	m_FlowList;
	vector<PathData*>	m_PathList;
	vector<PathData*>	m_PathInvList;
	vector<FishData*>	m_FishDataList;
	PathData			*m_BoLang;
	PathData			*m_DouDong;
	PathGroupData		*m_LongJuanFeng;
};