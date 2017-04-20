#pragma once
#define FISH_DURATION_TIME	0.1f		//最小单位0.5秒持续时间
#define MAX_FISH_NUM		USHRT_MAX	//每个场景中同时最多的鱼数量
#define MAX_BULLET_NUM		UCHAR_MAX	//每个场景中同时最多的子弹数量
enum PathEventType
{
	EVENT_NONE = 0,
	EVENT_STAY,     //停留
	EVENT_LAUGH,    //嘲讽
	EVENT_ATTACK,   //受击
};

enum FishClipType
{
	CLIP_YOUYONG = 0,
	CLIP_SIWANG,
	CLIP_CHAOFENG,
	CLIP_GONGJI,
	CLIP_MAX
};
//结构定义
struct Ray
{
	Vector3 Pos;
	Vector3 Dir;
};
struct FishData
{
	Vector3		Size;
	Quaternion	Rot;	//鱼模型的旋转
	float		ClipLength[CLIP_MAX];	//游泳、死亡、嘲讽动作的时间长度（秒）
};

struct GroupData
{
	struct GroupItem
	{
		GroupItem()
		{
			pFishPosList = NULL;
		}
		bool	 ActionUnite;
		float	 Action;
		float    SpeedScaling;
		USHORT   FishNum;
		BYTE	 FishIndex;
		float    FishScaling;
		Vector3  *pFishPosList;
	};
	GroupData()
	{
		pGroupDataArray = NULL;
		pPathList = NULL;
		TotalFishCount = 0;
	}
	USHORT  TotalFishCount;
	USHORT	PathCount;
	USHORT	GroupDataCount;
	Vector3 FrontPosition;
	GroupItem *pGroupDataArray;
	USHORT *pPathList;

	//鱼群（路径群）

	bool	FishGroupByPathGroup;
	USHORT  PathGroupIndex;
	float   Speed;
	BYTE    FishIndex;
	float   FishScaling;
	float   ActionSpeed;
	USHORT	RepeatNum;
	USHORT  Interval;//毫秒
	bool	ActionUnite;
};
struct FlowGroupData
{
	int		GroupID;
	int		Probability;		//出现的概率0 - 1000, 越大出现机率越大
	int		MaxCount;			//最多出现次数,-1无限

};
struct FlowTimeData
{
	int      GroupID;
	float    StartTime;          //0 - 1
	float    EndTime;            //0 - 1
	int      Probability;        //出现的概率0 - 100, 100必出现
	int      MaxCount;           //最多出现次数
	float    LaunchInterval;     //发射间隔，毫秒
	bool     ClearAll;           //清空场景
};
struct FlowData
{
	FlowData()
	{
		pFlowGroupList = NULL;
		pFlowTimeList = NULL;
	}
	int		MaxTime;                //秒
	int		LoopCount;
	float	LauncherInterval;		//秒
	int		GroupCount;
	int		FlowTimeCount;
	FlowGroupData *pFlowGroupList;
	FlowTimeData *pFlowTimeList;

	int		GetMaxTimeSeconds()
	{
		return MaxTime;
	}
};

struct LinearSplineNode
{
	short x, y, z;
	short qx, qy, qz, qw;
	short ts;
	short nodeIdx;
};
struct LinearSplineNodeUnzip
{
	Vector3 Pos;
	Quaternion Rot;
	float ts;
	short nodeIdx;
};
struct LinearNodeData
{
	byte	EventType;
	int 	Times;
};
struct PathData
{
	
	PathData()
	{
		pSplineDataList = NULL;
	}
	bool		HasPathEvent;
	float		SampleMaxDistance;
	float		MaxDistance;
	float		MaxTimeScaling;
	Matrix		WorldRTS;
	Rotation	WorldRot;

	Vector3		MinPos;
	Vector3		RangePos;
	Rotation	MinRot;
	Rotation	RangeRot;
	int			SplineDataCount;
	float		Speed;
	int			NodeCount;
	LinearNodeData   *pNodeList;
	LinearSplineNode *pSplineDataList;
	LinearSplineNodeUnzip *pSplineDataListUnzip;
};

struct PathGroupData
{
	short PathCount;
	PathData *pPathList;
};
struct DeadStayTime
{
	float TianZai_DouDong_Time;           //天灾抖动时间

	float TianZai_Stay_Time1;             //天灾等待陨石降落停留最短时间
	float TianZai_Stay_Time2;             //天灾等待陨石降落停留最长时间

	float TianZai_Dead_Time1;             //天灾死亡停留最短时间
	float TianZai_Dead_Time2;             //天灾死亡停留最长时间

	float Bullet_BingDong_Dead_Time1;     //冰冻炮击中死亡停留最短时间
	float Bullet_BingDong_Dead_Time2;     //冰冻炮击中死亡停留长短时间

	float BingDong_Dead_Time1;            //冰冻技能死亡最短时间
	float BingDong_Dead_Time2;            //冰冻技能死亡最长时间

	float ShanDian_Dead_Time1;            //闪电技能死亡最短时间
	float ShanDian_Dead_Time2;            //闪电技能死亡最长时间

	float LongJuanFeng_Dead_Time1;        //龙卷风技能死亡最短时间
	float LongJuanFeng_Dead_Time2;        //龙卷风技能死亡最长时间

	float JiGuang_Stay_Time;
	float JiGuang_Dead_Time1;             //激光大招死亡最短时间
	float JiGuang_Dead_Time2;             //激光大招死亡最长时间

	float ShandDian_Speed;
	float JiGuang_Speed;
	float JiGuang_BingDong_Speed;
	float TianZai_Speed;
	float BingDong_Speed;
};