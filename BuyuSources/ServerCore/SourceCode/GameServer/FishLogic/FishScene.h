#pragma once
#include "FishScene\FishCollider.h"
class FishManager;
class FishLauncher;
class BulletManager;
class FishSendInterface;
class FishMap;
class CTableRoleManager;
struct NetCmd;

#include <list>
class FishScene
{
public:
	FishScene();
	~FishScene();

	bool Init(CTableRoleManager *pm, FishSendInterface *pSend);
	void Shutdown();
	void Update(float deltaTime);
	bool SetMap(const WCHAR *pcMapName);
	void Reset();
	BulletManager * GetBulletMgr()
	{
		return m_pBulletMgr;
	}
	FishManager * GetFishMgr()
	{
		return m_pFishMgr;
	}
	FishLauncher * GetFishLauncher()
	{
		return m_pFishLauncher;
	}
	bool IsEndScene()const
	{
		return m_bSceneEnd && m_pFishMgr->FishCount() == 0;
	}
	const WCHAR *GetMapName()const;
	void Clear();
	byte GetSceneBackground()const;

	USHORT GetAngleByFish(WORD& LoackFishID, BYTE SeatID, Vector2& Pos, Vector2& pCenter);
protected:
	bool				m_bFishTide;
	CTableRoleManager*  m_pRoleMgr;
	bool				m_bFlowInterval;
	float				m_SwapInterval;
	bool				m_SwapScene;
	UINT				m_FlowInterval;
	USHORT				m_FlowIndex;
	bool				m_bSceneEnd;	//场景中的鱼发射结束。
	FishSendInterface	*m_pSender;
	FishManager			*m_pFishMgr;
	FishLauncher		*m_pFishLauncher;
	BulletManager		*m_pBulletMgr;
	FishMap				*m_pFishMap;
};
