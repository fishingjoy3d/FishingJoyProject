#pragma once
#include "FishUtility.h"
#pragma pack(push)
#pragma pack(1)
typedef UINT uint;
typedef USHORT ushort;

enum SubMainCmdType
{
	SUB_HEARBEAT,

	//登陆相关
	SUB_FAST_LOGON,
	SUB_NORMAL_LOGON,
	SUB_REGISTER_LOGON,

	SUB_LOGON_RESULT,
	SUB_LOGON_RESULT_SIMPLE,
	SUB_LOGON_HALL,
	SUB_LOGON_HALL_RESULT,

	SUB_JOIN_ROOM,
	SUB_JOIN_ROOM_RESULT,

	SUB_PLAYER_READY,

	SUB_BULLET,
	SUB_FISH,
	SUB_CATCHED,
	SUB_PLAYER_JOIN,
	SUB_PLAYER_LEAVE,
	SUB_CHANGE_LAUNCHER,
	SUB_CHANGR_LAUNCHER_TYPE,
	SUB_CHANGE_CLIENT_RATE,
	SUB_CHANGE_RATE,
	SUB_CHANGE_RATE_TYPE,
	SUB_SYNC_FISH,
	SUB_ROOM_INFO,
	SUB_SYNC_BULLET,
	SUB_PING,
	SUB_REDUCTION,

	SUB_USE_SKILL,

	//技能相关命令                      
	SUB_SKILL_TORNADO_REQUEST,
	SUB_SKILL_TORNADO_RESPONSE,
	SUB_SKILL_FREEZE_REQUEST,
	SUB_SKILL_FREEZE_RESPONSE,
	SUB_SKILL_LIGHTING_REQUEST,
	SUB_SKILL_LIGHTING_RESPONSE,
	SUB_SKILL_LASER_REQUEST,
	SUB_SKILL_LASER_RESPONSE,
	SUB_SKILL_DISASTER_REQUEST,
	SUB_SKILL_DISASTER_RESPONSE,
	SUB_SKILL_LOCK_REQUEST,
	SUB_SKILL_LOCK_RESPONSE,
	SUB_BULLET_REDUCTION,
	SUB_UPLOADING_REQUEST,
	SUB_UPLOADING_RESPONSE,
	SUB_UPLOADING_CHUNK,
	SUB_UPLOADING_CHUNK_RESPONSE,
	SUB_UPLOADING_COMPLETE,
	SUB_CHANGE_IMG,
	SUB_FISH_PACKAGE,

	SUB_CHECK_FISH_POS,
	SUB_CHECK_BULLET_POS,
	SUB_CLEAR_SCENE,
	SUB_LAUNCH_LASER_FAILED,
	SUB_SKILL_FAILED,

	CMD_SUB_DOUBLEHIT_EFFECT,
};
enum NetCmdType
{
	CMD_HEARTBEAT = SubMainCmdType::SUB_HEARBEAT,
	//登陆相关
	CMD_FAST_LOGON = Main_Game << 8 | SubMainCmdType::SUB_FAST_LOGON,
	CMD_NORMAL_LOGON = Main_Game << 8 | SubMainCmdType::SUB_NORMAL_LOGON,
	CMD_REGISTER_LOGON = Main_Game << 8 | SubMainCmdType::SUB_REGISTER_LOGON,
	CMD_LOGON_RESULT = Main_Game << 8 | SubMainCmdType::SUB_LOGON_RESULT,
	CMD_LOGON_RESULT_SIMPLE = Main_Game << 8 | SubMainCmdType::SUB_LOGON_RESULT_SIMPLE,
	CMD_LOGON_HALL = Main_Game << 8 | SubMainCmdType::SUB_LOGON_HALL,
	CMD_LOGON_HALL_RESULT = Main_Game << 8 | SubMainCmdType::SUB_LOGON_HALL_RESULT,
	CMD_JOIN_ROOM = Main_Game << 8 | SubMainCmdType::SUB_JOIN_ROOM,
	CMD_JOIN_ROOM_RESULT = Main_Game << 8 | SubMainCmdType::SUB_JOIN_ROOM_RESULT,
	CMD_PLAYER_READY = Main_Game << 8 | SubMainCmdType::SUB_PLAYER_READY,
	CMD_BULLET = Main_Game << 8 | SubMainCmdType::SUB_BULLET,
	CMD_FISH = Main_Game << 8 | SubMainCmdType::SUB_FISH,
	CMD_CATCHED = Main_Game << 8 | SubMainCmdType::SUB_CATCHED,
	CMD_PLAYER_JOIN = Main_Game << 8 | SubMainCmdType::SUB_PLAYER_JOIN,
	CMD_PLAYER_LEAVE = Main_Game << 8 | SubMainCmdType::SUB_PLAYER_LEAVE,
	CMD_CHANGE_LAUNCHER = Main_Game << 8 | SubMainCmdType::SUB_CHANGE_LAUNCHER,
	CMD_CHANGE_LAUNCHER_TYPE = Main_Game << 8 | SubMainCmdType::SUB_CHANGR_LAUNCHER_TYPE,
	CMD_CHANGE_CLIENT_RATE = Main_Game << 8 | SubMainCmdType::SUB_CHANGE_CLIENT_RATE,
	CMD_CHANGE_RATE = Main_Game << 8 | SubMainCmdType::SUB_CHANGE_RATE,
	
	CMD_CHANGE_RATE_TYPE = Main_Game << 8 | SubMainCmdType::SUB_CHANGE_RATE_TYPE,
	CMD_SYNC_FISH = Main_Game << 8 | SubMainCmdType::SUB_SYNC_FISH,
	CMD_ROOM_INFO = Main_Game << 8 | SubMainCmdType::SUB_ROOM_INFO,
	CMD_SYNC_BULLET = Main_Game << 8 | SubMainCmdType::SUB_SYNC_BULLET,
	CMD_PING = Main_Game << 8 | SubMainCmdType::SUB_PING,
	CMD_REDUCTION = Main_Game << 8 | SubMainCmdType::SUB_REDUCTION,
	CMD_USE_SKILL = Main_Game << 8 | SubMainCmdType::SUB_USE_SKILL,
	//技能相关命令                              
	CMD_SKILL_TORNADO_REQUEST = Main_Game << 8 | SubMainCmdType::SUB_SKILL_TORNADO_REQUEST,
	CMD_SKILL_TORNADO_RESPONSE = Main_Game << 8 | SubMainCmdType::SUB_SKILL_TORNADO_RESPONSE,
	CMD_SKILL_FREEZE_REQUEST = Main_Game << 8 | SubMainCmdType::SUB_SKILL_FREEZE_REQUEST,
	CMD_SKILL_FREEZE_RESPONSE = Main_Game << 8 | SubMainCmdType::SUB_SKILL_FREEZE_RESPONSE,
	CMD_SKILL_LIGHTING_REQUEST = Main_Game << 8 | SubMainCmdType::SUB_SKILL_LIGHTING_REQUEST,
	CMD_SKILL_LIGHTING_RESPONSE = Main_Game << 8 | SubMainCmdType::SUB_SKILL_LIGHTING_RESPONSE,
	CMD_SKILL_LASER_REQUEST = Main_Game << 8 | SubMainCmdType::SUB_SKILL_LASER_REQUEST,
	CMD_SKILL_LASER_RESPONSE = Main_Game << 8 | SubMainCmdType::SUB_SKILL_LASER_RESPONSE,
	CMD_SKILL_DISASTER_REQUEST = Main_Game << 8 | SubMainCmdType::SUB_SKILL_DISASTER_REQUEST,
	CMD_SKILL_DISASTER_RESPONSE = Main_Game << 8 | SubMainCmdType::SUB_SKILL_DISASTER_RESPONSE,

	CMD_SKILL_LOCK_REQUEST = Main_Game << 8 | SubMainCmdType::SUB_SKILL_LOCK_REQUEST,
	CMD_SKILL_LOCK_RESPONSE = Main_Game << 8 | SubMainCmdType::SUB_SKILL_LOCK_RESPONSE,

	CMD_BULLET_REDUCTION = Main_Game << 8 | SubMainCmdType::SUB_BULLET_REDUCTION,
	CMD_UPLOADING_REQUEST = Main_Game << 8 | SubMainCmdType::SUB_UPLOADING_REQUEST,
	CMD_UPLOADING_RESPONSE = Main_Game << 8 | SubMainCmdType::SUB_UPLOADING_RESPONSE,
	CMD_UPLOADING_CHUNK = Main_Game << 8 | SubMainCmdType::SUB_UPLOADING_CHUNK,
	CMD_UPLOADING_CHUNK_RESPONSE = Main_Game << 8 | SubMainCmdType::SUB_UPLOADING_CHUNK_RESPONSE,
	CMD_UPLOADING_COMPLETE = Main_Game << 8 | SubMainCmdType::SUB_UPLOADING_COMPLETE,
	CMD_CHANGE_IMG = Main_Game << 8 | SubMainCmdType::SUB_CHANGE_IMG,
	CMD_FISH_PACKAGE = Main_Game << 8 | SubMainCmdType::SUB_FISH_PACKAGE,
	CMD_CHECK_FISH_POS = Main_Game << 8 | SubMainCmdType::SUB_CHECK_FISH_POS,
	CMD_CHECK_BULLET_POS = Main_Game << 8 | SubMainCmdType::SUB_CHECK_BULLET_POS,
	CMD_CLEAR_SCENE = Main_Game << 8 | SubMainCmdType::SUB_CLEAR_SCENE,
	CMD_LAUNCH_LASER_FAILED = Main_Game << 8 | SubMainCmdType::SUB_LAUNCH_LASER_FAILED,
	CMD_SKILL_FAILLED = Main_Game << 8 | SubMainCmdType::SUB_SKILL_FAILED,
};

//enum NetCmdType
//{
//	CMD_HEARTBEAT,
//
//	//登陆相关
//	CMD_FAST_LOGON,
//	CMD_NORMAL_LOGON,
//	CMD_REGISTER_LOGON,
//
//	CMD_LOGON_RESULT,
//	CMD_LOGON_RESULT_SIMPLE,
//	
//	CMD_LOGON_HALL,
//	CMD_LOGON_HALL_RESULT,
//
//	CMD_JOIN_ROOM,
//	CMD_JOIN_ROOM_RESULT,
//
//	CMD_PLAYER_READY,
//	
//	CMD_BULLET,
//	CMD_FISH,
//	CMD_CATCHED,
//	CMD_PLAYER_JOIN,
//	CMD_PLAYER_LEAVE,
//	CMD_CHANGE_LAUNCHER,
//	CMD_CHANGE_RATE,
//	CMD_SYNC_FISH,
//	CMD_ROOM_INFO,
//	CMD_SYNC_BULLET,
//	CMD_PING,
//	CMD_REDUCTION,
//
//	CMD_USE_SKILL,
//	//技能相关命令                                 
//	CMD_SKILL_TORNADO_REQUEST,
//	CMD_SKILL_TORNADO_RESPONSE,
//	CMD_SKILL_FREEZE_REQUEST,
//	CMD_SKILL_FREEZE_RESPONSE,
//	CMD_SKILL_LIGHTING_REQUEST,
//	CMD_SKILL_LIGHTING_RESPONSE,
//	CMD_SKILL_LASER_REQUEST,
//	CMD_SKILL_LASER_RESPONSE,
//	CMD_SKILL_DISASTER_REQUEST,
//	CMD_SKILL_DISASTER_RESPONSE,
//	CMD_BULLET_REDUCTION,
//	CMD_UPLOADING_REQUEST,
//	CMD_UPLOADING_RESPONSE,
//	CMD_UPLOADING_CHUNK,
//	CMD_UPLOADING_CHUNK_RESPONSE,
//	CMD_UPLOADING_COMPLETE,
//	CMD_CHANGE_IMG,
//	CMD_FISH_PACKAGE,
//
//	
//
//	CMD_CHECK_FISH_POS,
//	CMD_CHECK_BULLET_POS,
//	CMD_CLEAR_SCENE,
//
//	CMD_INNER_JOIN,
//	CMD_INNER_LEAVE,
//	CMD_INNER_FTP_RESULT,
//
//	CMD_MAX 
//};


typedef unsigned char byte;
typedef unsigned short ushort;
struct PlayerBaseInfo
{
	UINT	ID;
	byte	Lvl;
	UINT	ImgCrc;
	bool	Sex;
	UINT	GoldNum;
	WCHAR	Name[7];
};

struct NetCmdBullet : public NetCmd
{
	ushort  BulletID;
	short	Angle;
	byte    LauncherType;
	uint	Energy;
	byte	ReboundCount;
	ushort	LockFishID;
};

struct NetCmdFish : public NetCmd
{
	ushort GroupID;
	ushort PathID;
	ushort StartID;
};

struct NetFishList
{
	ushort FishID;
	ushort FishTime;
};
struct NetCmdCollide : public NetCmd
{
	ushort BulletID;
	ushort BulletTime;
	NetFishList Fishs[1];
};

enum FishCatchEventType
{
	// 1 捕获， 2，受击，3.播放特效,4.闪电击中
	CATCH_EVENT_CATCHED = 1,
	CATCH_EVENT_ATTACK,
	CATCH_EVENT_EFFECT,
	CATCH_EVENT_CATCHED_LIGHTING,
	CATCH_EVENT_CATCHED_LIGHTING_MAIN,
};
struct FishCatched
{
	byte	CatchEvent; 
	ushort	FishID;
	ushort  nReward;
	ushort  LightingFishID;	//闪电鱼ID
};

struct NetCmdCatched : public NetCmd
{
	ushort BulletID;
	ushort	Combo;
	UINT	GoldNum;
	UINT	TotalNum;
	FishCatched Fishs[1];
};

struct NetCmdPlayerJoin : public NetCmd
{
	byte	Seat;
	byte	LauncherType;
	byte	rateIndex;//倍率
	PlayerBaseInfo	PlayerInfo;
};
struct NetCmdRoomInfo : public NetCmd
{
	byte	BackgroundType;
	WCHAR	Name[MAX_MAP_NAME + 1];
};

struct NetCmdPlayerLeave : public NetCmd
{
	byte Seat;
};
struct NetCmdChangeLauncher : public NetCmd
{
	byte Seat;
	byte LauncherType;
};
struct NetCmdChangeLauncherType : public NetCmd
{
	byte Seat;
	byte LauncherType;
};

struct NetCmdSceneFishData
{
	ushort	GroupID;
	ushort	PathIdx;
	ushort  StartID;
	ushort	ElapsedTime;	//毫秒
	UINT	FishMask;	//鱼群中鱼的位标记，0表示已被击中销毁。
};

struct NetCmdSceneFish : public NetCmd
{
	NetCmdSceneFishData fishs[1];
};

struct NetCmdSyncFish :public NetCmd
{
	ushort FishID;
	ushort GroupID;
	float  FishTime;
	ushort PathGroup;
	ushort PathIdx;
	//嘲讽时间
	bool   IsActiveEvent;
	ushort ElapsedTime;
	byte   DelayType;		//减速类型
	byte   DelayScaling;	//减速百分比
	byte   DelayDuration1;
	byte   DelayDuration2;
	byte   DelayDuration3;
	ushort DelayCurrentTime;
	byte   Package;
};

struct SyncBulletData
{
	ushort  BulletID;
	short	Degree;
	ushort  Time;
	byte	BulletType;
	byte	RateIdx;
	byte	ReboundCount;
	ushort  LockFishID;
};
struct NetCmdSyncBullet : public NetCmd
{
	SyncBulletData Bullets[1];
};
struct FishDeadTime
{
	FISHID	fishID;
	byte	deadTime;
	ushort  nReward;
	ushort  lightingFishID;
};
struct NetCmdSkillTornado : public NetCmd
{
	byte   Seat;
	UINT	GoldNum;
	UINT	TotalNum;
	FishDeadTime FishID[1];
};

struct NetCmdSkillFreeze : public NetCmd
{
	byte	Seat;
	UINT	GoldNum;
	UINT	TotalNum;
	FishDeadTime  FishID[1];
};
struct NetCmdSkillLighting : public NetCmd
{
	byte   Seat;
	UINT	GoldNum;
	UINT	TotalNum;
	FishDeadTime FishID[1];
};
struct NetCmdSkillLaser : public NetCmd
{
	short	Degree;
};
struct NetCmdSkillLaserResponse : public NetCmd
{
	byte Seat;
	byte LaserType;	//激光的类型
	short Degree;
	UINT	GoldNum;
	UINT	TotalNum;
	FishDeadTime FishID[1];
};
struct NetCmdReduction : public NetCmd
{
	byte		ReductionType;// 1: Laser,
	byte		LaserType;
	FISHID		FishID[1];
};
struct NetCmdSkillDisaster : public NetCmd
{
	byte    Seat;
	UINT    GoldNum;
	UINT	TotalNum;
	FishDeadTime FishID[1];
};
struct NetCmdSkillLock : public NetCmd
{
	byte    Seat;
};
struct NetCmdBulletReduction : public NetCmd
{
	ushort	BulletID;
	ushort	Combo;
	UINT    GoldNum;
	UINT	TotalNum;
	FishCatched	FishID[1];
};
struct NetCmdUploadImgRequest : public NetCmd
{
	ushort Size;
};
struct NetCmdUploadImgResponse : public NetCmd
{
	byte AllowUpload;
};
struct NetCmdUploadImgChunk : public NetCmd
{
	ushort	StartIndex;
	ushort	Size;
	byte	ImgData[1];
};
struct NetCmdUploadImgChunkResponse : public NetCmd
{
	byte UploadResult;
};


struct NetCmdUploadImgComplete : public NetCmd
{
	byte Result;
	UINT ImgCrcValue;
};
struct NetCmdImgChange : public NetCmd
{
	byte Seat;
	UINT PlayerID;
	UINT ImgCrcValue;
};


//内部使用命令
struct NetCmdInnerImgResult: public NetCmd
{
	UINT	Crc;
	UINT	ImgResult;
};
struct NetCmdFishPackage : public NetCmd
{
	USHORT	FishID;
	byte	PackageType;
};
struct ReductionData
{
	byte Speed;
	byte Reduction1;
	byte Reduction2;
	byte Reduction3;
};
//全局设置
struct NetCmdGlobalSetting :public NetCmd
{
	UINT Version;
	ushort Fish_MaxNum;
	byte Bullet_MaxNum;
	byte Bullet_Speed;
	ReductionData SkillFreezeReduction;
	ReductionData BulletFreezeReduction;
	ReductionData LaserReduction[MAX_LAUNCHER_NUM];	//4种炮台的减速
	byte Launcher_Interval[MAX_LAUNCHER_NUM];			//发射间隔
	byte Laser_CDTime[MAX_LAUNCHER_NUM];				//大招CD
	byte Skill_CDTime[SKILL_MAX];				//技能CD
	
};
struct NetCmdJoinRoom :public NetCmd
{
	byte RoomType;
};


struct NetCmdRegisterLogon : public NetCmd
{
	uint CRC1;
	uint CRC2;
	uint CRC3;
	WCHAR UID[16];
};
struct NetCmdFastLogon : public NetCmd
{

};
struct NetCmdNormalLogon : public NetCmd
{
	uint CRC1;
	uint CRC2;
	uint CRC3;
	WCHAR UID[16];
};
struct NetCmdLogonResultSimple : public NetCmd
{
	byte	Result;
	uint	ServerIP;
	ushort	ServerPort;
	UINT	PlayerID;
};
struct NetCmdLogonResult : public NetCmd
{
	uint	CRC1;
	uint	CRC2;
	uint	CRC3;
	byte	Result;
	uint	ServerIP;
	ushort	ServerPort;
	UINT	PlayerID;
	WCHAR	UID[16];
};

struct NetCmdLogonHall : public NetCmd
{
	uint	CRC1;
	uint	CRC2;
	uint	CRC3;
	WCHAR	UID[16];
};

struct NetCmdLogonHallResult : public NetCmd
{
	byte	Result;
};
struct NetCmdJoinRoomResult :public NetCmd
{
	byte Result;
	byte RoomID;
	byte Seat;
	byte LauncherType;
	byte BackImage;
};
struct CheckFishPosData
{
	ushort		FishID;
	float		Time;
	float		Speed;
	Vector3		Pos;
	Quaternion	Rot;
};
struct NetCmdCheckFishPos :public NetCmd
{
	ushort FishNum;
	CheckFishPosData Fish[1];
};
struct NetCmdUseSkill:public NetCmd
{
    byte SkillID;
};
struct NetCmdClientChangeRate : public NetCmd
{
	bool UpRoDownRate;//客户端改变倍率
};
struct NetCmdChangeRate:public NetCmd
{
	byte Seat;
	byte RateIndex;
	bool IsCanUseRate;
};
struct NetCmdChangeRateType :public NetCmd
{
	byte Seat;
	byte RateIndex;
};

struct CheckBulletPos
{
	ushort id;
	Vector3 pos;
};
struct NetCmdCheckBulletPos:public NetCmd
{
	ushort Count;
	CheckBulletPos Bullets[1];
};
struct NetCmdClearScene:public NetCmd
{
	byte ClearType;//0从左到右，1从右到左，2重连
};
struct NetCmdLaunchFailed :public NetCmd
{
	byte FailedType;
	uint Energy;
};

struct NetCmdSkillFailed :public NetCmd
{
	byte FailedType;
	byte bySkillType;
	ushort uTimes;
};


#pragma pack(pop)