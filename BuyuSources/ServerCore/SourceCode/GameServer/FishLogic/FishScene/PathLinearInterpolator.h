#pragma once
#include "FishResData.h"
#include "RuntimeInfo.h"
struct SplineSampleData
{
	SplineSampleData(const Vector3 &p, const Quaternion &q, float scaling, int idx)
	{
		pos = p;
		rot = q;
		timeScaling = scaling;
		nodeIdx = idx;
	}
	Vector3 pos;
	Quaternion rot;
	float timeScaling;
	int nodeIdx;
};

class PathLinearInterpolator
{
public:
	PathData *pPathData;

	void InitFromPathData(PathData *pPath)
	{
		pPathData = pPath;
	}

	void GetSplineDataUnzip(int idx, Vector3 &pos, Quaternion &rot, float &timeScaling)
	{
		LinearSplineNodeUnzip &nodezip = pPathData->pSplineDataListUnzip[idx];
		pos = nodezip.Pos;
		rot = nodezip.Rot;
		timeScaling = nodezip.ts;
		return;

		LinearSplineNode &node = pPathData->pSplineDataList[idx];
		pos.x = node.x * RuntimeInfo::InvShortMaxValue;
		pos.y = node.y * RuntimeInfo::InvShortMaxValue;
		pos.z = node.z * RuntimeInfo::InvShortMaxValue;

		rot.x = node.qx * RuntimeInfo::InvShortMaxValue;
		rot.y = node.qy * RuntimeInfo::InvShortMaxValue;
		rot.z = node.qz * RuntimeInfo::InvShortMaxValue;
		rot.w = node.qw * RuntimeInfo::InvShortMaxValue;

		pos = MulAdd(pos, pPathData->RangePos, pPathData->MinPos);
		rot = MulAdd(rot, pPathData->RangeRot, pPathData->MinRot);

		timeScaling = pPathData->pSplineDataList[idx].ts * RuntimeInfo::InvShortMaxValue * pPathData->MaxTimeScaling;
	}
	void GetSplineDataUnzip(int idx, Vector3 &pos)
	{
		pos = pPathData->pSplineDataListUnzip[idx].Pos;
		return;
		LinearSplineNode &node = pPathData->pSplineDataList[idx];
		pos.x = node.x * RuntimeInfo::InvShortMaxValue;
		pos.y = node.y * RuntimeInfo::InvShortMaxValue;
		pos.z = node.z * RuntimeInfo::InvShortMaxValue;

		pos = MulAdd(pos, pPathData->RangePos, pPathData->MinPos);
	}
	float GetSplineDataUnzip(int idx)
	{
		return pPathData->pSplineDataListUnzip[idx].ts;
	}
	Vector3 GetPos(float time)
	{
		Vector3 pos;
		float t = pPathData->SplineDataCount * time;
		int idx = (int)t;
		t -= idx;
		if (idx < pPathData->SplineDataCount - 1)
		{
			Vector3 pos2;
			GetSplineDataUnzip(idx, pos);
			GetSplineDataUnzip(idx + 1, pos2);
			pos = Lerp(pos, pos2, t);
		}
		else
		{
			GetSplineDataUnzip(pPathData->SplineDataCount - 1, pos);
		}
		return pos;
	}
	float GetTimeScaling(float time, int &nodeIdx)
	{
		float t = pPathData->SplineDataCount * time;
		int idx = (int)t;
		t -= idx;
		if (idx < pPathData->SplineDataCount - 1)
		{
			float t1 = GetSplineDataUnzip(idx);
			float t2 = GetSplineDataUnzip(idx + 1);
			nodeIdx = pPathData->pSplineDataListUnzip[idx].nodeIdx;
			return LerpFloat(t1, t2, t);
		}
		else
		{
			nodeIdx = pPathData->pSplineDataListUnzip[pPathData->SplineDataCount - 1].nodeIdx;
			return GetSplineDataUnzip(pPathData->SplineDataCount - 1);
		}
	}
	int GetPosRotTimeScaling(float time, Vector3 &pos, Quaternion &rot, float &timeScaling)
	{
		float t = pPathData->SplineDataCount * time;
		int idx = (int)t;
		t -= idx;
		if (idx < pPathData->SplineDataCount - 1)
		{
			Vector3 pos2;
			Quaternion rot2;
			float timeScaling2;
			GetSplineDataUnzip(idx,  pos,  rot,  timeScaling);
			GetSplineDataUnzip(idx + 1,  pos2,  rot2,  timeScaling2);
			pos = Lerp(pos, pos2, t);
			rot = Slerp(rot, rot2, t);
			timeScaling = LerpFloat(timeScaling, timeScaling2, t);
			return pPathData->pSplineDataListUnzip[idx].nodeIdx;
		}
		else
		{
			GetSplineDataUnzip(pPathData->SplineDataCount - 1, pos, rot, timeScaling);
			return pPathData->pSplineDataListUnzip[pPathData->SplineDataCount - 1].nodeIdx;
		}
	}
};
