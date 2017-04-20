#pragma once
enum  CatchType
{
	CATCH_SKILL,
	CATCH_LASER,
	CATCH_BULLET,
};
enum LaunchFailedType
{
	LFT_OK,
	LFT_CD,
	LFT_INVALID,
	LFT_ENERGY,
};

enum SkillFailedType
{
	SFT_OK,
	SFT_CD,
	SFT_INVALID,
	SFT_COUNT,
	SFT_UNLOCK,
	SFT_BIND
};
//配置属性接口
class IFishSetting{
public:
	virtual bool LoadConfig(char szDir[]) = 0;
	virtual UINT  GetVersion() = 0;
	virtual float BulletRadius(USHORT bulletID) = 0;					//子弹基本半径
	virtual float BulletCatchRadius(USHORT bulletID) = 0;					//子弹碰撞之后的捕获半径
	virtual UINT  MaxCatch(USHORT bulletID) = 0;				//子弹的最大捕鱼数量
	virtual float CatchChance(PlayerID player, USHORT bulletID, byte BulletType, byte fishType, int catchNum, int maxCatch, BYTE byPackageType, bool bLocked) = 0;	//子弹的捕获机率(0 - 1)
	virtual SkillFailedType  UseSkill(PlayerID playerID, SkillType skill) = 0;
	virtual LaunchFailedType  UseLaser(PlayerID playerID, byte launcherType) = 0;
	virtual void  GetSkillRange(SkillType skill, int &minNum, int &maxNum) = 0;
	virtual void  GetLaserRange(byte laser, int &minNum, int &maxNum) = 0;
	virtual void  GetSkillFreezeReduction(byte &speedScaling, byte &duration1, byte& duration2, byte &duration3) = 0;
	virtual float GetSkillChance(SkillType skill, USHORT fishIndex, BYTE byPackageType) = 0;
	virtual float GetLaserChance(byte launcherType, USHORT fishIndex, BYTE byPackageType) = 0;
	virtual float GetLaserRadius(byte launcherType) = 0;
	//LauncherType炮台类型
	virtual BulletType GetBulletType(USHORT bulletID) = 0;
	virtual void  GetBulletFreezeReduction(byte &speedScaling, byte &duration1, byte& duration2, byte &duration3) = 0;
	//捕获鱼，返回金币数量
	virtual UINT  CatchFish(PlayerID playerID, USHORT fishIndx, CatchType catchType, byte subType, byte packge, byte rate) = 0;
	virtual USHORT    GetCombo(PlayerID playerID, USHORT bulletID) = 0;
	//是否允许上传头像
	virtual bool CanUploadImg(PlayerID playerID, USHORT imgSize) = 0;
	//技能的cd时间
	virtual byte  GetSkillCDTime(SkillType st) = 0;
	//炮台大招的减速参数
	virtual void  GetLaserReduction(byte LauncherType, byte &speedScaling, byte &duration1, byte& duration2, byte &duration3) = 0;
	//炮台的子弹速度
	virtual byte  GetBulletSpeed(byte launcherType) = 0;
	//炮台的发射间隔
	virtual byte  GetLauncherInterval(byte LauncherType) = 0;
	//大招的CD时间
	virtual byte  GetLaserCDTime(byte LauncherType) = 0;
	virtual UINT  LaunchPackage(UINT fishid_packageid)  = 0;
	virtual byte  GetRateIndex(byte seat, PlayerID id) = 0;
	virtual void  BulletGain(PlayerID id, byte BulletType, UINT goldNum) = 0;
	virtual USHORT FishRewardDrop(PlayerID id, BYTE byPackageType,USHORT byType) = 0;

	//是否是闪电鱼，单纯的判断一下。
	virtual bool IsLightingFish(USHORT fishIndex) = 0;
	//是否是闪电鱼，并返回可以捕获的数量，fishIndex鱼的ID，玩家id,返回0 表示不是，返回大于0表示可以闪电的总数量
	virtual int  IsLightingFish(USHORT fishIndex, PlayerID id) = 0;
	//某一条鱼是否可以被闪电捕获，fishIndex鱼ID，玩家ID，dist:屏幕距离。
	virtual bool CanCatchLightingFish(USHORT fishIndex, PlayerID id, float dist) = 0;
	
};

struct NetCmd;
typedef void(*SendDataCallback)(PlayerID id, NetCmd *pCmd);
typedef void(*RecvDataCallback)(PlayerID id, NetCmd *pCmd);
typedef void(*FishLogCallback)(const WCHAR *pcStr);
typedef void(*KickPlayerCallback)(PlayerID id);
typedef IFishSetting* (*BulletDataCallback)(USHORT bulletID);

class FishCallback
{
public:

	//外部提供给FishScene使用的接口
	//===============================================
	static void SetSendDataFunc(SendDataCallback pFunc);
	static void SendData(PlayerID id, NetCmd *pCmd);
	
	static void SetFishLogFunc(FishLogCallback pFunc);
	static void FishLog(const WCHAR *pcStr);


	static void SetKickPlayerFunc(KickPlayerCallback pFunc);
	static void KickPlayer(PlayerID id);

	static void SetFishSetting(IFishSetting *pSetting);
	static IFishSetting* GetFishSetting();

	//FishScene提供给外部的接口
	//===============================================
	static void SetRecvDataFunc(RecvDataCallback pFunc);
	static void RecvData(PlayerID id, NetCmd *pCmd);

};