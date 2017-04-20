#include "stdafx.h"
#include "../FishCallbak.h"
#include "FishResManager.h"
#include "RuntimeInfo.h"
#include <algorithm>
#include "FishMap.h"

FishResManager* FishResManager::Inst()
{
	static FishResManager fishResMgr;
	return &fishResMgr;
}
bool FishResManager::Init(const WCHAR *pcDir)
{
	m_FishDataCrc = 0;
	InnerFishLog(L"目录文件:%s", pcDir);

	if (!InitNewData(pcDir))
	{
		return false;
	}
	
	if (!InitFlow(pcDir))
	{
		InnerFishLog(L"流程加载失败!");
		return false;
	}
	//if (!InitDeadStayTime(pcDir))
	//{
	//	InnerFishLog(L"时间加载失败!");
	//	return false;
	//}
	CHAR tempDir[MAX_PATH], configDir[MAX_PATH];
	::WideCharToMultiByte(CP_ACP, 0, pcDir, -1, tempDir, sizeof(tempDir), NULL, 0);
	sprintf_s(configDir, sizeof(configDir), "%s/Config", tempDir);
	if (FishCallback::GetFishSetting())
	if (!FishCallback::GetFishSetting()->LoadConfig(configDir))
	{
		InnerFishLog(L"加载配置文件失败!");
		return false;
	}
	RuntimeInfo::Init();
	wstring mapdir = pcDir;
	mapdir += L"/FishMap";
	if (!FishMap::LoadAllMap(mapdir.c_str()))
	{
		InnerFishLog(L"加载地图失败!");
		return false;
	}

	return true;
}
bool FishResManager::_OldInit(const WCHAR *pcDir)
{
	InnerFishLog(L"目录文件:%s", pcDir);
	if (!InitPath(pcDir))
	{
		InnerFishLog(L"路径加载失败!");
		return false;
	}
	if (!InitPathGroup(pcDir))
	{
		InnerFishLog(L"路径群加载失败!");
		return false;
	}
	if (!InitFishData(pcDir))
	{
		InnerFishLog(L"鱼数据加载失败!");
		return false;
	}
	if (!InitGroup(pcDir))
	{
		InnerFishLog(L"鱼群加载失败!");
		return false;
	}
	if (!InitFlow(pcDir))
	{
		InnerFishLog(L"流程加载失败!");
		return false;
	}
	if (!InitSkillPath(pcDir))
	{
		InnerFishLog(L"技能路径加载失败!");
		return false;
		
	}
	//if (!InitDeadStayTime(pcDir))
	//{
	//	InnerFishLog(L"时间加载失败!");
	//	return false;
	//}
	CHAR tempDir[MAX_PATH], configDir[MAX_PATH];
	::WideCharToMultiByte(CP_ACP, 0, pcDir, -1, tempDir, sizeof(tempDir), NULL, 0);
	sprintf_s(configDir, sizeof(configDir), "%s/Config", tempDir);
	if (FishCallback::GetFishSetting())
	if (!FishCallback::GetFishSetting()->LoadConfig(configDir))
	{
		InnerFishLog(L"加载配置文件失败!");
		return false;
	}
	RuntimeInfo::Init();
	wstring mapdir = pcDir;
	mapdir += L"/FishMap";
	if (!FishMap::LoadAllMap(mapdir.c_str()))
	{
		InnerFishLog(L"加载地图失败!");
		return false;
	}
	
	return true;
}
void FishResManager::Shutdown()
{
	//path
	for (UINT i = 0; i < m_PathList.size(); ++i)
	{
		SAFE_DELETE_ARR(m_PathList[i]->pSplineDataList);
		SAFE_DELETE_ARR(m_PathInvList[i]->pSplineDataList);
	}
	m_PathList.clear();
	m_PathInvList.clear();

	//group
	for (UINT i = 0; i < m_GroupList.size(); ++i)
	{
		if (m_GroupList[i]->pGroupDataArray)
		{
			for (int j = 0; j < m_GroupList[i]->GroupDataCount; ++j)
			{
				SAFE_DELETE_ARR(m_GroupList[i]->pGroupDataArray[j].pFishPosList);
			}
			SAFE_DELETE_ARR(m_GroupList[i]->pGroupDataArray);
		}
		SAFE_DELETE_ARR(m_GroupList[i]->pPathList);
	}
	m_GroupList.clear();

	//flow
	for (UINT i = 0; i < m_FlowList.size(); ++i)
	{
		SAFE_DELETE_ARR(m_FlowList[i]->pFlowGroupList);
		SAFE_DELETE_ARR(m_FlowList[i]->pFlowTimeList);
	}
	m_FlowList.clear();

	//fishData
	for (UINT i = 0; i < m_FishDataList.size(); ++i)
	{
		SAFE_DELETE(m_FishDataList[i]);
	}
	m_FishDataList.clear();

	for (UINT i = 0; i < m_PathGroupList.size(); ++i)
	{
		PathGroupData *pgd = m_PathGroupList[i];

		for (int j = 0; j < pgd->PathCount; ++j)
		{
			SAFE_DELETE_ARR(pgd->pPathList[j].pNodeList);
			SAFE_DELETE_ARR(pgd->pPathList[j].pSplineDataList);
			SAFE_DELETE_ARR(pgd->pPathList[j].pSplineDataListUnzip);
		}
		SAFE_DELETE(m_PathGroupList[i]);
		SAFE_DELETE(m_PathGroupInvList[i]);
	}
	m_PathGroupList.clear();
	m_PathGroupInvList.clear();
	SAFE_DELETE(m_BoLang);
	SAFE_DELETE(m_DouDong);
	SAFE_DELETE(m_LongJuanFeng);
	FishMap::ReleaseAllMap();

}
bool FishResManager::InitGroup(const WCHAR *pcDir)
{
	WCHAR pathDir[MAX_PATH];
	swprintf_s(pathDir, MAX_PATH, L"%s/Group", pcDir);
	int idx = 0;
	while (1)
	{
		FILE *pFile;
		WCHAR path[MAX_PATH];
		swprintf_s(path, MAX_PATH, L"%s/group%d.bytes", pathDir, idx++);
		_wfopen_s(&pFile, path, L"rb");
		if (pFile == NULL)
			break;
		int version;
		READFILE(version);
		GroupData *groupData = new GroupData;
		m_GroupList.push_back(groupData);
		GroupData &pd = *groupData;

		READFILE(pd.FishGroupByPathGroup);
		if (pd.FishGroupByPathGroup)
		{
			READFILE(pd.PathGroupIndex);
			READFILE(pd.Speed);
			READFILE(pd.FishIndex);
			READFILE(pd.FishScaling);
			READFILE(pd.ActionSpeed);
			READFILE(pd.RepeatNum);
			READFILE(pd.Interval);
			READFILE(pd.ActionUnite);
			if (GetFishData(pd.FishIndex) == NULL)
			{
				InnerFishLog(L"鱼群中不存在的鱼索引:%d : %d", idx - 1, pd.FishIndex);
				return false;
			}
			if (GetPathGroup(pd.PathGroupIndex) == NULL)
			{
				InnerFishLog(L"鱼群中不存在的路径群索引:%d : %d", idx - 1, pd.PathGroupIndex);
				return false;
			}
		}
		else
		{
			READFILE(pd.FrontPosition);
			READFILE(pd.PathCount);
			pd.pPathList = new USHORT[pd.PathCount];
			if (pd.pPathList == NULL)
			{
				Shutdown();
				return false;
			}
			for (int i = 0; i < pd.PathCount; ++i)
			{
				READFILE(pd.pPathList[i]);
				if (GetPath(pd.pPathList[i]) == NULL)
				{
					InnerFishLog(L"2鱼群中不存在的路径索引:%d : %d", idx - 1, pd.pPathList[i]);
					return false;
				}
			}
			READFILE(pd.GroupDataCount);
			pd.pGroupDataArray = new GroupData::GroupItem[pd.GroupDataCount];
			if (pd.pGroupDataArray == NULL)
			{
				Shutdown();
				return false;
			}
			for (int i = 0; i < pd.GroupDataCount; ++i)
			{
				GroupData::GroupItem &gd = pd.pGroupDataArray[i];
				READFILE(gd.FishIndex);
				READFILE(gd.FishNum);
				READFILE(gd.FishScaling);
				READFILE(gd.SpeedScaling);
				READFILE(gd.Action);
				READFILE(gd.ActionUnite);
				gd.pFishPosList = new Vector3[gd.FishNum];
				if (GetFishData(gd.FishIndex) == NULL)
				{
					InnerFishLog(L"2鱼群中不存在的鱼索引:%d : %d", idx - 1, gd.FishIndex);
					return false;
				}
				if (gd.pFishPosList == NULL)
				{
					Shutdown();
					return false;
				}
				for (int j = 0; j < gd.FishNum; ++j)
				{
					READFILE(gd.pFishPosList[j]);
				}
			}
		}// end if (pd.FishGroupByPathGroup)
		UINT magic;
		READFILE(magic);
		if (magic != FILE_END_MAGIC)
		{
			InnerFishLog(L"Group文件结尾不正确!");
		}
		fclose(pFile);
	}
	InnerFishLog(L"加载鱼群数:%d ", m_GroupList.size());
	return true;
}
bool SortFlowTime(const FlowTimeData &f1, const FlowTimeData &f2)
{
	if (f1.StartTime < f2.StartTime)
		return true;
	if (f1.StartTime == f2.StartTime)
		return f1.EndTime < f2.EndTime;

	return false;
}
bool FishResManager::InitFlow(const WCHAR *pcDir)
{
	WCHAR pathDir[MAX_PATH];
	swprintf_s(pathDir, MAX_PATH, L"%s/Flow", pcDir);
	int idx = 0;
	while (1)
	{
		FILE *pFile;
		WCHAR path[MAX_PATH];
		swprintf_s(path, MAX_PATH, L"%s/flow%d.bytes", pathDir, idx++);
		_wfopen_s(&pFile, path, L"rb");
		if (pFile == NULL)
			break;
		int version;
		READFILE(version);

		FlowData *flowData = new FlowData;
		m_FlowList.push_back(flowData);
		FlowData &fd = *flowData;
		READFILE(fd.LauncherInterval);
		READFILE(fd.LoopCount);
		READFILE(fd.MaxTime);
		READFILE(fd.FlowTimeCount);
		fd.pFlowTimeList = new FlowTimeData[fd.FlowTimeCount];
		if (fd.pFlowTimeList == NULL)
		{
			Shutdown();
			return FALSE;
		}
		for (int i = 0; i < fd.FlowTimeCount; ++i)
		{
			READFILE(fd.pFlowTimeList[i].GroupID);
			READFILE(fd.pFlowTimeList[i].Probability);
			READFILE(fd.pFlowTimeList[i].StartTime);
			READFILE(fd.pFlowTimeList[i].EndTime);
			READFILE(fd.pFlowTimeList[i].ClearAll);
			if (fd.pFlowTimeList[i].ClearAll == false)
			{

				READFILE(fd.pFlowTimeList[i].LaunchInterval);
				READFILE(fd.pFlowTimeList[i].MaxCount);
			}
			if (GetGroup(fd.pFlowTimeList[i].GroupID) == NULL)
			{
				InnerFishLog(L"流程中不存在的鱼群ID:%d, %d", idx - 1, fd.pFlowTimeList[i].GroupID);
				return false;
			}
		}

		::sort(fd.pFlowTimeList, fd.pFlowTimeList + fd.FlowTimeCount, SortFlowTime);

		READFILE(fd.GroupCount);
		fd.pFlowGroupList = new FlowGroupData[fd.GroupCount];
		if (fd.pFlowGroupList == NULL)
		{
			Shutdown();
			return FALSE;
		}
		for (int i = 0; i < fd.GroupCount; ++i)
		{
			READFILE(fd.pFlowGroupList[i].GroupID);
			READFILE(fd.pFlowGroupList[i].MaxCount);
			READFILE(fd.pFlowGroupList[i].Probability);
			if (GetGroup(fd.pFlowGroupList[i].GroupID) == NULL)
			{
				InnerFishLog(L"2流程中不存在的鱼群ID:%d, %d", idx - 1, fd.pFlowGroupList[i].GroupID);
				return false;
			}
		}
		UINT magic;
		READFILE(magic);
		if (magic != FILE_END_MAGIC)
		{
			InnerFishLog(L"Flow文件结尾不正确!");
		}
		fclose(pFile);
	}
	InnerFishLog(L"加载流程数:%d ", m_FlowList.size());
	return true;
}
bool ReadPathData(PathData &pd, FILE *pFile)
{
	READFILE(pd.MaxTimeScaling);
	READFILE(pd.MinPos);
	READFILE(pd.RangePos);
	READFILE(pd.MinRot);
	READFILE(pd.RangeRot);
	READFILE(pd.WorldRot);
	READFILE(pd.WorldRTS);

	READFILE(pd.MaxDistance);
	READFILE(pd.SampleMaxDistance);
	READFILE(pd.HasPathEvent);
	READFILE(pd.NodeCount);
	D3DXMatrixTranspose(&pd.WorldRTS, &pd.WorldRTS);

	pd.pNodeList = new LinearNodeData[pd.NodeCount];
	for (int i = 0; i < pd.NodeCount; ++i)
	{
		READFILE(pd.pNodeList[i].EventType);
		READFILE(pd.pNodeList[i].Times);
		if (pd.pNodeList[i].Times > USHRT_MAX)
			pd.pNodeList[i].Times = USHRT_MAX;
	}

	READFILE(pd.SplineDataCount);
	pd.pSplineDataList = new LinearSplineNode[pd.SplineDataCount];
	pd.pSplineDataListUnzip = new LinearSplineNodeUnzip[pd.SplineDataCount];
	if (pd.pSplineDataList == NULL)
	{
		return false;
	}
	for (int j = 0; j < pd.SplineDataCount; ++j)
	{
		LinearSplineNode & node = pd.pSplineDataList[j];
		LinearSplineNodeUnzip & nodezip = pd.pSplineDataListUnzip[j];

		READFILE(node.x);
		READFILE(node.y);
		READFILE(node.z);

		READFILE(node.qx);
		READFILE(node.qy);
		READFILE(node.qz);
		READFILE(node.qw);

		READFILE(node.ts);
		READFILE(node.nodeIdx);

		nodezip.Pos.x = node.x * RuntimeInfo::InvShortMaxValue;
		nodezip.Pos.y = node.y * RuntimeInfo::InvShortMaxValue;
		nodezip.Pos.z = node.z * RuntimeInfo::InvShortMaxValue;

		nodezip.Rot.x = node.qx * RuntimeInfo::InvShortMaxValue;
		nodezip.Rot.y = node.qy * RuntimeInfo::InvShortMaxValue;
		nodezip.Rot.z = node.qz * RuntimeInfo::InvShortMaxValue;
		nodezip.Rot.w = node.qw * RuntimeInfo::InvShortMaxValue;

		nodezip.Pos = MulAdd(nodezip.Pos, pd.RangePos, pd.MinPos);
		nodezip.Rot = MulAdd(nodezip.Rot, pd.RangeRot, pd.MinRot);

		nodezip.ts = node.ts * RuntimeInfo::InvShortMaxValue * pd.MaxTimeScaling;

		nodezip.nodeIdx = node.nodeIdx;
	}

	return true;
}
bool FishResManager::InitPath(const WCHAR *pcDir)
{
	WCHAR pathDir[MAX_PATH];
	swprintf_s(pathDir, MAX_PATH, L"%s/Path", pcDir);
	int idx = 0;
	while (1)
	{
		{
			FILE *pFile;
			WCHAR path[MAX_PATH];
			swprintf_s(path, MAX_PATH, L"%s/path%d.bytes", pathDir, idx);
			_wfopen_s(&pFile, path, L"rb");
			if (pFile == NULL)
				break;
			int version;
			READFILE(version);

			PathData *pathData = new PathData;
			m_PathList.push_back(pathData);
			PathData &pd = *pathData;

			if (!ReadPathData(pd, pFile))
			{
				Shutdown();
				return false;
			}
			UINT magic;
			READFILE(magic);
			if (magic != FILE_END_MAGIC)
			{
				InnerFishLog(L"Path文件结尾不正确!");
			}
			::fclose(pFile);
		}
		{
			FILE *pFile;
			WCHAR path[MAX_PATH];
			swprintf_s(path, MAX_PATH, L"%s/path%d_inv.bytes", pathDir, idx++);
			_wfopen_s(&pFile, path, L"rb");
			if (pFile == NULL)
				break;
			int version;
			READFILE(version);

			PathData *pathData = new PathData;
			m_PathInvList.push_back(pathData);
			PathData &pd = *pathData;

			if (!ReadPathData(pd, pFile))
			{
				Shutdown();
				return false;
			}
			UINT magic;
			READFILE(magic);
			if (magic != FILE_END_MAGIC)
			{
				InnerFishLog(L"Path文件结尾不正确!");
			}
			::fclose(pFile);
		}
	}
	InnerFishLog(L"加载路径数:%d ", m_PathList.size());
	return true;
}
bool FishResManager::InitPathGroup(const WCHAR *pcDir)
{
	WCHAR pathDir[MAX_PATH];
	swprintf_s(pathDir, MAX_PATH, L"%s/PathGroup", pcDir);
	int idx = 0;
	while (1)
	{
		{
			FILE *pFile;
			WCHAR path[MAX_PATH];
			swprintf_s(path, MAX_PATH, L"%s/pathgroup%d.bytes", pathDir, idx);
			_wfopen_s(&pFile, path, L"rb");
			if (pFile == NULL)
				break;
			int version;
			READFILE(version);
			short pathCount;
			READFILE(pathCount);
			PathGroupData *pPathGroup = new PathGroupData;
			pPathGroup->PathCount = pathCount;
			pPathGroup->pPathList = new PathData[pathCount];
			m_PathGroupList.push_back(pPathGroup);

			for (int n = 0; n < pathCount; ++n)
			{
				PathData &pd = pPathGroup->pPathList[n];

				if (!ReadPathData(pd, pFile))
				{
					Shutdown();
					return false;
				}
			}

			UINT magic;
			READFILE(magic);
			if (magic != FILE_END_MAGIC)
			{
				InnerFishLog(L"PathGroup文件结尾不正确!");
			}
			::fclose(pFile);
		}
		{
			FILE *pFile;
			WCHAR path[MAX_PATH];
			swprintf_s(path, MAX_PATH, L"%s/pathgroup%d_inv.bytes", pathDir, idx++);
			_wfopen_s(&pFile, path, L"rb");
			if (pFile == NULL)
				break;
			int version;
			READFILE(version);
			short pathCount;
			READFILE(pathCount);
			PathGroupData *pPathGroup = new PathGroupData;
			pPathGroup->PathCount = pathCount;
			pPathGroup->pPathList = new PathData[pathCount];
			m_PathGroupInvList.push_back(pPathGroup);

			for (int n = 0; n < pathCount; ++n)
			{
				PathData &pd = pPathGroup->pPathList[n];

				if (!ReadPathData(pd, pFile))
				{
					Shutdown();
					return false;
				}
			}

			UINT magic;
			READFILE(magic);
			if (magic != FILE_END_MAGIC)
			{
				InnerFishLog(L"PathGroup文件结尾不正确!");
			}
			::fclose(pFile);
		}
	}
	InnerFishLog(L"加载路径群数:%d ", m_PathGroupList.size());
	return true;
}
bool FishResManager::InitSkillPath(const WCHAR *pcDir)
{
	WCHAR pathDir[MAX_PATH];
	swprintf_s(pathDir, MAX_PATH, L"%s/PathGroup", pcDir);
	//波浪
	{
		FILE *pFile;
		WCHAR path[MAX_PATH];
		swprintf_s(path, MAX_PATH, L"%s/EffectPath/bolang.bytes", pcDir);
		_wfopen_s(&pFile, path, L"rb");
		if (pFile == NULL)
		{
			InnerFishLog(L"未找到波浪路径.");
			return false;
		}
		int version;
		READFILE(version);

		PathData *pathData = new PathData;
		m_BoLang = pathData;
		PathData &pd = *pathData;
		if (!ReadPathData(pd, pFile))
		{
			Shutdown();
			return false;
		}
		UINT magic;
		READFILE(magic);
		if (magic != FILE_END_MAGIC)
		{
			InnerFishLog(L"Path文件结尾不正确!");
		}
		::fclose(pFile);
	}

	//抖动
	{
		FILE *pFile;
		WCHAR path[MAX_PATH];
		swprintf_s(path, MAX_PATH, L"%s/EffectPath/doudong.bytes", pcDir);
		_wfopen_s(&pFile, path, L"rb");
		if (pFile == NULL)
		{
			InnerFishLog(L"未找到抖动路径.");
			return false;
		}
		int version;
		READFILE(version);

		PathData *pathData = new PathData;
		m_DouDong = pathData;
		PathData &pd = *pathData;

		if (!ReadPathData(pd, pFile))
		{
			Shutdown();
			return false;
		}
		UINT magic;
		READFILE(magic);
		if (magic != FILE_END_MAGIC)
		{
			InnerFishLog(L"Path文件结尾不正确!");
		}
		::fclose(pFile);
	}
	//龙卷风
	{
		FILE *pFile;
		WCHAR path[MAX_PATH];
		swprintf_s(path, MAX_PATH, L"%s/EffectPath/longjuanfeng.bytes", pcDir);
		_wfopen_s(&pFile, path, L"rb");
		if (pFile == NULL)
		{
			InnerFishLog(L"未找到龙卷风路径.");
			return false;
		}
		int version;
		READFILE(version);
		short pathCount;
		READFILE(pathCount);
		PathGroupData *pPathGroup = new PathGroupData;
		pPathGroup->PathCount = pathCount;
		pPathGroup->pPathList = new PathData[pathCount];
		m_LongJuanFeng = pPathGroup;

		for (int n = 0; n < pathCount; ++n)
		{
			PathData &pd = pPathGroup->pPathList[n];

			if (!ReadPathData(pd, pFile))
			{
				Shutdown();
				return false;
			}
		}

		UINT magic;
		READFILE(magic);
		if (magic != FILE_END_MAGIC)
		{
			InnerFishLog(L"PathGroup文件结尾不正确!");
		}
		::fclose(pFile);
	}
	return true;
}
bool FishResManager::InitFishData(const WCHAR *pcDir)
{
	WCHAR pathDir[MAX_PATH];
	swprintf_s(pathDir, MAX_PATH, L"%s/fishData", pcDir);
	int idx = 0;
	while (1)
	{
		FILE *pFile;
		WCHAR path[MAX_PATH];
		swprintf_s(path, MAX_PATH, L"%s/fishData%d.bytes", pathDir, idx++);
		_wfopen_s(&pFile, path, L"rb");
		if (pFile == NULL)
			break;
		int version;
		bool bHasBox;

		FishData *fishData = new FishData;
		m_FishDataList.push_back(fishData);
		FishData &fb = *fishData;
		READFILE(version);
		READFILE(bHasBox);
		if (bHasBox)
		{
			READFILE(fb.Size);
			READFILE(fb.Rot);
			fb.Size.x = fabsf(fb.Size.x);
			fb.Size.y = fabsf(fb.Size.y);
			fb.Size.z = fabsf(fb.Size.z);
		}
		else
		{
			fb.Size = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
			D3DXQuaternionIdentity(&fb.Rot);
		}

		for (int i = 0; i < CLIP_MAX; ++i)
			READFILE(fb.ClipLength[i]);

		UINT magic;
		READFILE(magic);
		if (magic != FILE_END_MAGIC)
		{
			InnerFishLog(L"FishData文件结尾不正确!");
		}
		fclose(pFile);
	}
	InnerFishLog(L"加载鱼信息数:%d ", m_FishDataList.size());
	return true;
}
USHORT ConvertToUSHORT(float time)
{
	USHORT t = USHORT(time * 1000);
	return t;
}
bool FishResManager::InitDeadStayTime(const WCHAR *pcDir)
{
	WCHAR pathDir[MAX_PATH];
	swprintf_s(pathDir, MAX_PATH, L"%s/timeSetting", pcDir);
	FILE *pFile;
	WCHAR path[MAX_PATH];
	swprintf_s(path, MAX_PATH, L"%s/DeadStayTime.bytes", pathDir);
	_wfopen_s(&pFile, path, L"rb");
	if (pFile == NULL)
		return false;
	int version;
	READFILE(version);


	READFILE(m_DeadStayTime.TianZai_DouDong_Time);

	READFILE(m_DeadStayTime.TianZai_Stay_Time1);
	READFILE(m_DeadStayTime.TianZai_Stay_Time2);

	READFILE(m_DeadStayTime.TianZai_Dead_Time1);
	READFILE(m_DeadStayTime.TianZai_Dead_Time2);

	READFILE(m_DeadStayTime.Bullet_BingDong_Dead_Time1);
	READFILE(m_DeadStayTime.Bullet_BingDong_Dead_Time2);

	READFILE(m_DeadStayTime.BingDong_Dead_Time1);
	READFILE(m_DeadStayTime.BingDong_Dead_Time2);

	READFILE(m_DeadStayTime.ShanDian_Dead_Time1);
	READFILE(m_DeadStayTime.ShanDian_Dead_Time2);

	READFILE(m_DeadStayTime.LongJuanFeng_Dead_Time1);
	READFILE(m_DeadStayTime.LongJuanFeng_Dead_Time2);

	READFILE(m_DeadStayTime.JiGuang_Dead_Time1);
	READFILE(m_DeadStayTime.JiGuang_Dead_Time2);

	READFILE(m_DeadStayTime.ShandDian_Speed);
	READFILE(m_DeadStayTime.JiGuang_Speed);
	READFILE(m_DeadStayTime.JiGuang_BingDong_Speed);
	READFILE(m_DeadStayTime.TianZai_Speed);
	READFILE(m_DeadStayTime.BingDong_Speed);

	//临时设置属性
	m_DeadStayTime.TianZai_DouDong_Time = 0.1f;

	m_DeadStayTime.TianZai_Stay_Time1 = 0.8f;
	m_DeadStayTime.TianZai_Stay_Time2 = 1.2f;

	m_DeadStayTime.TianZai_Dead_Time1 = 0.8f;
	m_DeadStayTime.TianZai_Dead_Time2 = 1.2f;

	m_DeadStayTime.Bullet_BingDong_Dead_Time1 = 0.2f;
	m_DeadStayTime.Bullet_BingDong_Dead_Time2 = 0.2f;

	m_DeadStayTime.BingDong_Dead_Time1 = 1.0f;
	m_DeadStayTime.BingDong_Dead_Time2 = 1.2f;

	m_DeadStayTime.ShanDian_Dead_Time1 = 0.5f;
	m_DeadStayTime.ShanDian_Dead_Time2 = 1.0f;

	m_DeadStayTime.LongJuanFeng_Dead_Time1 = 0.3f;
	m_DeadStayTime.LongJuanFeng_Dead_Time2 = 0.6f;

	m_DeadStayTime.JiGuang_Stay_Time = 0.2f;
	m_DeadStayTime.JiGuang_Dead_Time1 = 1.0f;
	m_DeadStayTime.JiGuang_Dead_Time2 = 1.0f;

	m_DeadStayTime.ShandDian_Speed = 0.0f;
	m_DeadStayTime.JiGuang_Speed = 0.0f;
	m_DeadStayTime.JiGuang_BingDong_Speed = 0.0f;
	m_DeadStayTime.TianZai_Speed = 0.0f;
	m_DeadStayTime.BingDong_Speed = 0.0f;

	UINT magic;
	READFILE(magic);
	if (magic != FILE_END_MAGIC)
	{
		InnerFishLog(L"TimeSetting文件结尾不正确!");
	}
	fclose(pFile);
	return true;
}

void ReadNewPath(PathData *pd, FILE *pFile)
{
	//transform
	for (int i = 0; i < 4; ++i)
	{
		READFILE(pd->WorldRTS.m[i][0]);
		READFILE(pd->WorldRTS.m[i][1]);
		READFILE(pd->WorldRTS.m[i][2]);
		READFILE(pd->WorldRTS.m[i][3]);
	}
	D3DXMatrixTranspose(&pd->WorldRTS, &pd->WorldRTS);
	READFILE(pd->WorldRot);
	READFILE(pd->MaxDistance);
	READFILE(pd->SampleMaxDistance);
	READFILE(pd->HasPathEvent);
	READFILE(pd->SplineDataCount);
	READFILE(pd->NodeCount);

	pd->pSplineDataList = NULL;
	pd->pNodeList = new LinearNodeData[pd->NodeCount];
	pd->pSplineDataListUnzip = new LinearSplineNodeUnzip[pd->SplineDataCount];
	USHORT times;
	for (int i = 0; i < pd->NodeCount; ++i)
	{
		READFILE(pd->pNodeList[i].EventType);
		READFILE(times);
		pd->pNodeList[i].Times = times;
	}
	for (int j = 0; j < pd->SplineDataCount; ++j)
	{
		READFILE(pd->pSplineDataListUnzip[j].Pos);
		READFILE(pd->pSplineDataListUnzip[j].Rot);
		READFILE(pd->pSplineDataListUnzip[j].ts);
		READFILE(pd->pSplineDataListUnzip[j].nodeIdx);
	}
}
bool FishResManager::InitNewData(const WCHAR *pcDir)
{
	WCHAR pathDir[MAX_PATH];
	swprintf_s(pathDir, MAX_PATH, L"%s/NewFishData.bytes", pcDir);
	FILE *pFile;
	_wfopen_s(&pFile, pathDir, L"rb");
	if (pFile == NULL)
		return false;

	USHORT pathCount;
	USHORT pathGroupCount;
	USHORT fishGroupCount;
	USHORT fishDataCount;

	READFILE(m_FishDataCrc);
	READFILE(pathCount);
	READFILE(pathGroupCount);
	READFILE(fishGroupCount);
	READFILE(fishDataCount);

	for (UINT i = 0; i < pathCount; ++i)
	{
		PathData *pd = new PathData;
		PathData pdinv;
		ReadNewPath(pd, pFile);
		ReadNewPath(&pdinv, pFile);
		m_PathList.push_back(pd);
		SAFE_DELETE_ARR(pdinv.pSplineDataListUnzip);
		SAFE_DELETE_ARR(pdinv.pNodeList);
	}
	InnerFishLog(L"加载路径:%d", pathCount);
	for (UINT i = 0; i < pathGroupCount; ++i)
	{
		PathGroupData *pgd = new PathGroupData();
		READFILE(pgd->PathCount);
		pgd->pPathList = new PathData[pgd->PathCount];

		PathGroupData pgdInv;
		pgdInv.pPathList = new PathData[pgd->PathCount];
		for (int j = 0; j < pgd->PathCount; ++j)
		{
			ReadNewPath(&pgd->pPathList[j], pFile);
		}

		for (int j = 0; j < pgd->PathCount; ++j)
		{
			ReadNewPath(&pgdInv.pPathList[j], pFile);
			SAFE_DELETE_ARR(pgdInv.pPathList[j].pSplineDataListUnzip);
			SAFE_DELETE(pgdInv.pPathList[j].pNodeList);
		}
		SAFE_DELETE_ARR(pgdInv.pPathList);
		m_PathGroupList.push_back(pgd);
	}
	InnerFishLog(L"加载路径群:%d", pathGroupCount);
	for (USHORT i = 0; i < fishDataCount; ++i)
	{
		FishData *fd = new FishData;
		READFILE(fd->Size);
		READFILE(fd->Rot);
		for (int j = 0; j < CLIP_MAX; ++j)
			READFILE(fd->ClipLength[j]);
		m_FishDataList.push_back(fd);
	}
	InnerFishLog(L"加载鱼信息:%d", fishDataCount);
	for (USHORT i = 0; i < fishGroupCount; ++i)
	{
		GroupData *ppd = new GroupData;
		GroupData &pd = *ppd;
		m_GroupList.push_back(ppd);

		READFILE(pd.FishGroupByPathGroup);
		if (pd.FishGroupByPathGroup)
		{
			READFILE(pd.PathGroupIndex);
			READFILE(pd.Speed);
			READFILE(pd.FishIndex);
			READFILE(pd.FishScaling);
			READFILE(pd.ActionSpeed);
			READFILE(pd.RepeatNum);
			READFILE(pd.Interval);
			READFILE(pd.ActionUnite);
		}
		else
		{
			READFILE(pd.FrontPosition);
			READFILE(pd.PathCount);
			pd.pPathList = new USHORT[pd.PathCount];
			for (int i = 0; i < pd.PathCount; ++i)
			{
				READFILE(pd.pPathList[i]);
			}
			READFILE(pd.GroupDataCount);
			pd.pGroupDataArray = new GroupData::GroupItem[pd.GroupDataCount];
			for (int i = 0; i < pd.GroupDataCount; ++i)
			{
				GroupData::GroupItem &gd = pd.pGroupDataArray[i];
				READFILE(gd.FishIndex);
				READFILE(gd.FishNum);
				READFILE(gd.FishScaling);
				READFILE(gd.SpeedScaling);
				READFILE(gd.Action);
				READFILE(gd.ActionUnite);
				gd.pFishPosList = new Vector3[gd.FishNum];
				for (int j = 0; j < gd.FishNum; ++j)
					READFILE(gd.pFishPosList[j]);
			}
		}// end if (pd.FishGroupByPathGroup)
	}
	InnerFishLog(L"加载鱼群:%d", fishGroupCount);
	PathData pd;

	//波浪 
	ReadNewPath(&pd, pFile);
	SAFE_DELETE_ARR(pd.pSplineDataListUnzip);
	SAFE_DELETE(pd.pNodeList);
	//抖动
	ReadNewPath(&pd, pFile);
	SAFE_DELETE_ARR(pd.pSplineDataListUnzip);
	SAFE_DELETE(pd.pNodeList);

	//龙卷风
	PathGroupData pgd;
	READFILE(pgd.PathCount);
	pgd.pPathList = new PathData[pgd.PathCount];
	for (int j = 0; j < pgd.PathCount; ++j)
	{
		ReadNewPath(&pgd.pPathList[j], pFile);

		SAFE_DELETE_ARR(pgd.pPathList[j].pSplineDataListUnzip);
		SAFE_DELETE(pgd.pPathList[j].pNodeList);
	}
	SAFE_DELETE_ARR(pgd.pPathList);

	UINT magic;
	READFILE(magic);
	if (magic != FILE_END_MAGIC)
	{
		InnerFishLog(L"NewFishData文件结尾不正确!");
	}

	fclose(pFile);
	return true;
}