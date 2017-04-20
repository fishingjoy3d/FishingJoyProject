#pragma once
#include "FishScene\FishSendInterface.h"
struct NetCmd;
class FishScene;
class CRole;
class CTableRoleManager;
struct NetCmdCheckFishPos;
struct NetCmdCheckBulletPos;
struct NetCmdSkillLaser;
class FishDesk : public FishSendInterface
{
public:

	FishDesk();
	~FishDesk();

	bool Init(int index, NetSendInterface *pNetSend, const WCHAR *pcMapName, CTableRoleManager* pManager);
	void Shutdown();
	//返回要踢除的玩家个数
	void Update(float deltaTime);
	bool HandleNetCmd(PlayerID id, NetCmd *pCmd);	//返回否，踢队玩家
	bool HasFreeSeat();
	bool PlayerJoin(PlayerID id, byte&seat);
	bool PlayerLeave(PlayerID id);
	void SendAll(USHORT groupIndex, USHORT pathIndex, USHORT startID);
	void SendAll(USHORT groupIndex, USHORT startID);
	void SendAll(NetCmd *pCmd);
	bool IsEndScene()const;
	CRole *GetPlayer(byte seat);
	CRole* GetPlayerFromID(PlayerID id);

	void OnGameStar();
	void OnGameEnd();

	byte GetSceneBackground();
	void AsyncPlayerJoin(CRole *pPlayer, bool bAsyncScene = false);

	void ResetLauncher(PlayerID id);

	WORD GetAngleByFish(WORD& LoackFishID, BYTE SeatID);
	DWORD GetFishSum();
protected:
	void CheckClientBulletPos(NetCmdCheckBulletPos *pos);
	void SendGlobalSetting(CRole *player);
	void SendFishToClient(CRole* pp);
	void ProcessSkillDisaster(CRole *player);
	void ProcessSkillLock(CRole *player);
	void ProcessSkillLaser(NetCmdSkillLaser *pCmd, CRole *player);
	void ProcessSkillLighting(CRole *player);
	void ProcessSkillFreeze(CRole *player);
	void ProcessSkillTornado(CRole *player);
	void CheckFishPos(NetCmdCheckFishPos *pos);
	bool FindPlayerByName(const WCHAR *pcName);
	void RemovePlayer(PlayerID id);
	void SendFrom(PlayerID id, NetCmd *pCmd);
	void SendTo(PlayerID id, NetCmd *pCmd);

	

	NetSendInterface *m_pNetSend;
	FishScene*		m_pScene;
	int				m_nDeskIndex;
	bool			m_bPause;
	int				g_BulletId;
	float			g_DeltaTime;
	bool			m_bResetScene;

	CTableRoleManager*	m_PlayerManager;//用户管理器 当前桌子的
};
