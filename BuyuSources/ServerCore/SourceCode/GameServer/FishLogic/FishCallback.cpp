#include "stdafx.h"
#include "FishCallbak.h"
#include "FishScene\FishResData.h"
//临时的子弹属性代替
class MyFishSetting : public IFishSetting
{
public:
	bool LoadConfig(char *pc)
	{
		return true;
	}
	UINT  GetVersion()
	{
		return 0;//0.0.0.0;
	}
	float BulletRadius(USHORT id)
	{
		switch (id)
		{
		case 3:
			return 0.3f;
		default:
			return 0.3f;
		}
	}
	float BulletCatchRadius(USHORT id)
	{
		switch (id)
		{
		case 3:
			return 3.0f;
		default:
			return 2.0f;
		}
	}
	UINT  MaxCatch(USHORT id){
		if (id == 3)
			return 100;
		else
			return 100;
	}
	float CatchChance(PlayerID player, USHORT bulletID, byte BulletType, byte fishType, int num, int maxnum, BYTE byPackageType, bool bLocked)
	{
		if (bulletID == 3)
			return 0.50f;
		else
			return 0.50f;
	}
	SkillFailedType  UseSkill(PlayerID playerID, SkillType skill)
	{
		return SFT_OK;
	}
	void  GetLaserRange(byte laser, int &minNum, int &maxNum)
	{
		minNum = 30;
		maxNum = 70;
	}
	void  GetSkillRange(SkillType skill, int &minNum, int &maxNum)
	{
		minNum = 30;
		maxNum = 70;
	}
	void GetSkillFreezeReduction(byte &speedScaling, byte &duration1, byte& duration2, byte &duration3)
	{
		speedScaling = byte(0.2f * 255);
		duration1 = byte(0 / FISH_DURATION_TIME);
		duration2 = byte(3 / FISH_DURATION_TIME);
		duration3 = byte(1 / FISH_DURATION_TIME);
	}
	float GetLaserChance(byte launcherType, USHORT fishIndex, BYTE byPackageType)
	{
		return 1.0f;
	}
	float GetSkillChance(SkillType skill, USHORT fishIndex, BYTE byPackageType)
	{
		return 0.75f;
	}
	float GetLaserRadius(byte launcherType)
	{
		return 1.5f;
	}
	
	void  GetLaserReduction(byte launcherType, byte &speedScaling, byte &duration1, byte& duration2, byte &duration3)
	{
		speedScaling = byte(0.2f * 255);
		duration1 = byte(0 / FISH_DURATION_TIME);
		duration2 = byte(1 / FISH_DURATION_TIME);
		duration3 = byte(0 / FISH_DURATION_TIME);
	}
	BulletType GetBulletType(USHORT bulletID)
	{
		switch (bulletID)
		{
			default:
				return BULLET_TYPE_NORMAL;
			case 3:
				return BULLET_TYPE_FREEZE;
		}
	}
	void GetBulletFreezeReduction(byte &speedScaling, byte &duration1, byte& duration2, byte &duration3)
	{
		speedScaling = byte(0.2f * 255);
		duration1 = byte(0 / FISH_DURATION_TIME);
		duration2 = byte(1 / FISH_DURATION_TIME);
		duration3 = byte(0 / FISH_DURATION_TIME);
	}
	UINT   CatchFish(PlayerID playerID, USHORT fishIndx, CatchType catchType, byte subType, byte packge, byte rate)
	{
		return 3;
	}
	bool CanUploadImg(PlayerID playerID, USHORT imgSize)
	{
		return true;
	}
	
	byte  GetBulletSpeed(byte luancherType)
	{
		if(luancherType == 4)
			return 60;
		else
			return 50;
	}
	byte  GetLauncherInterval(byte LauncherType)
	{
		return byte(0.3f / FISH_DURATION_TIME);
	}
	byte  GetLaserCDTime(byte LauncherType)
	{
		return byte(1.0f / FISH_DURATION_TIME);
	}
	byte  GetSkillCDTime(SkillType st)
	{
		return byte(3.0f / FISH_DURATION_TIME);
	}
	LaunchFailedType  UseLaser(PlayerID playerID, byte launcherType)
	{
		return LFT_OK;
	}
	UINT  LaunchPackage(UINT fishid_packageid) 
	{
		return 0;
	}
	byte  GetRateIndex(byte seat , PlayerID id)
	{
		return 0;
	}
	void  BulletGain(PlayerID id, byte BulletType, UINT goldNum)
	{
	}
	ushort    GetCombo(PlayerID playerID, USHORT bulletID)
	{
		return 0;
	}

	USHORT FishRewardDrop(PlayerID id, BYTE byPackageType, USHORT byType)
	{
		return 0;
	}
};

SendDataCallback	g_SendDataCallback = NULL;
RecvDataCallback	g_RecvDataCallback = NULL;
FishLogCallback		g_FishLogCallback = NULL;
KickPlayerCallback  g_KickPlayerCallback = NULL;
BulletDataCallback	g_BulletDataCallback = NULL;
IFishSetting		*g_pFishSetting = NULL;

//外部提供给FishScene使用的接口
//===============================================
void FishCallback::SetSendDataFunc(SendDataCallback pFunc)
{
	g_SendDataCallback = pFunc;
}

void FishCallback::SendData(PlayerID id, NetCmd *pCmd)
{
	if (g_SendDataCallback)
		(g_SendDataCallback)(id, pCmd);
}

//FishScene提供给外部的接口
//===============================================
void FishCallback::SetRecvDataFunc(RecvDataCallback pFunc)
{
	g_RecvDataCallback = pFunc;
}
void FishCallback::RecvData(PlayerID id, NetCmd *pCmd)
{
	if (g_RecvDataCallback)
		(g_RecvDataCallback)(id, pCmd);
}

void FishCallback::SetFishLogFunc(FishLogCallback pFunc)
{
	g_FishLogCallback = pFunc;
}
void FishCallback::FishLog(const WCHAR *pcStr)
{
	if (g_FishLogCallback)
		(g_FishLogCallback)(pcStr);
	else
	{
		wcout << pcStr << endl;
		OutputDebugStringW(pcStr);
		OutputDebugStringA("\n");
	}
}

void FishCallback::SetKickPlayerFunc(KickPlayerCallback pFunc)
{
	g_KickPlayerCallback = pFunc;
}
void FishCallback::KickPlayer(PlayerID id)
{
	if (g_KickPlayerCallback)
		(g_KickPlayerCallback)(id);
}
void FishCallback::SetFishSetting(IFishSetting *pSetting)
{
	SAFE_DELETE(g_pFishSetting);
	g_pFishSetting = pSetting;
}
IFishSetting* FishCallback::GetFishSetting()
{
	return g_pFishSetting;
}