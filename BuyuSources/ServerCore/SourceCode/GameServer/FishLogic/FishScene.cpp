#include "stdafx.h"
#include "FishScene.h"
#include "../Role.h"
#include "FishScene\FishManager.h"
#include "FishScene\BulletManager.h"
#include "FishScene\FishLauncher.h"
#include "NetCmd.h"
#include "FishScene\FishCollider.h"
//#include "FishScene\Timer.h"
#include "FishScene\FishMap.h"
#include <math.h>
#define INTERVAL_FLOW 3500
FishScene::FishScene()
{
	m_pFishMgr = NULL;
	m_pFishLauncher = NULL;
	m_pBulletMgr = NULL;
	m_pFishMap = NULL;
	m_bSceneEnd = false;
	m_bFlowInterval = false;
}
FishScene::~FishScene()
{

}
void FishScene::Reset()
{
	m_pFishMap->Reset();
	int idx = m_pFishMap->GetFlowIndex(m_bFishTide);
	FlowData *pFlow = FishResManager::Inst()->GetFlow(idx);
	m_pFishLauncher->Init(pFlow);
	m_pFishMgr->ClearAllFish(true);
	m_pBulletMgr->ClearAllBullet();
	m_bFlowInterval = false;
	m_FlowInterval = 0;
}
bool FishScene::Init(CTableRoleManager *pm,  FishSendInterface *pSend)
{
	m_bFishTide = false;
	m_pRoleMgr = pm;
	m_pFishMap = new FishMap();
	m_pFishMgr = new FishManager;
	m_pFishLauncher = new FishLauncher(m_pFishMgr);
	m_pBulletMgr = new BulletManager;
	m_pSender = pSend;
	
	bool bRet = true;
	bRet &= m_pFishMgr->Init();
	bRet &= m_pBulletMgr->Init();
	bRet &= m_pFishLauncher->Init(NULL, m_pSender);
	m_SwapInterval = 0;
	m_SwapScene = false;
	if (!bRet)
	{
		Shutdown();
		return false;
	}
	else
		return true;
}
bool FishScene::SetMap(const WCHAR *pcMapName)
{
	if (m_pFishMap->LoadMap(pcMapName))
	{
		Reset();
		return true;
	}
	else
		return false;
}
void FishScene::Shutdown()
{
	m_pSender = NULL;
	m_pFishMgr->Shutdown();
	m_pBulletMgr->Shutdown();
	m_pFishLauncher->Shutdown();
	
	SAFE_DELETE(m_pFishMap);
	SAFE_DELETE(m_pFishLauncher);
	SAFE_DELETE(m_pFishMgr);
	SAFE_DELETE(m_pBulletMgr);
}
void FishScene::Update(float deltaTime)
{
	bool bClearScene = false;
	UINT package = 0xffffffff;
	if(m_bFlowInterval == false)
	{
		package = m_pFishMap->Update(deltaTime);
		//流程使用完了,更换新的。
		if (m_pFishLauncher->IsActive() == false)
		{
			if (m_pFishMap->HasFlowIndex() == false)
			{
				if (m_pFishMap->IsRepeat())
					m_pFishMap->Reset();
				else
				{
					//结束了。
					m_bSceneEnd = true;
				}
			}
			if (m_bSceneEnd == false)
			{
				int idx = m_pFishMap->GetFlowIndex(m_bFlowInterval);
				m_bFishTide = m_bFlowInterval;
				if(m_bFlowInterval)
				{
					m_pFishMgr->ClearAllFish(false);
					m_FlowIndex = ConvertIntToWORD(idx);
					m_FlowInterval = GetTickCount();
					//InnerFishLog(L"切换鱼阵，间隔3.5秒:%d", idx);

					NetCmdClearScene nt;
					SetMsgInfo(nt,CMD_CLEAR_SCENE, sizeof(NetCmdClearScene));
					//nt.SetCmdType(CMD_CLEAR_SCENE);
					nt.ClearType = rand() % 2;
					//nt.SetCmdSize(sizeof(nt));
					m_pSender->SendAll(&nt); 
					m_pRoleMgr->SwitchFishTide();
					bClearScene = true;
				}
				else
				{
					//InnerFishLog(L"切换流程:%d", idx);
					m_pFishLauncher->Init(FishResManager::Inst()->GetFlow(idx));
				}
			}
		}
	}
	if(m_bFlowInterval == false)
	{
		if (m_bFishTide)
			package = -1;
		UINT fishID_PackageType = m_pFishLauncher->Update(deltaTime, package);
		if (fishID_PackageType != -1)
		{
			//发射了红包
			USHORT fishID = fishID_PackageType >> 16;
			m_pFishMgr->GetFish(fishID)->SetPackage(fishID_PackageType & 0xff);
			//FishCallback::GetFishSetting()->LaunchPackage(fishID_PackageType);
		}
	}
	else
	{
		if(GetTickCount() - m_FlowInterval > INTERVAL_FLOW)
		{
			//InnerFishLog(L"切换流程:%d", m_FlowIndex);
			m_pFishLauncher->Init(FishResManager::Inst()->GetFlow(m_FlowIndex));
			m_bFlowInterval = false;
		}
	}
	
	m_pFishMgr->Update(deltaTime);
	m_pBulletMgr->Update(m_pFishMgr, deltaTime);
	UINT count = m_pFishMgr->GetFishCount();
	if (count == 0)
	{
		if (m_SwapScene == false)
		{
			if (m_pFishLauncher->HasLaunchFish() == false)
			{
				m_SwapScene = true;
				m_SwapInterval = 0;
			}
		}
		else
		{
			m_SwapInterval += deltaTime;
			if (m_SwapInterval >= 5)
			{
				//重置场景
				//Log("鱼已经被打完了，切换场景鱼!");
				m_SwapScene = false;
				m_SwapInterval = 0;
				m_pFishLauncher->SetDeactive();
			}
		}
	}
	FishCollider::Collide(m_pRoleMgr, m_pBulletMgr, m_pFishMgr, m_pSender);
	
}
void FishScene::Clear()
{
	m_pFishLauncher->InitFlowData(true);
	m_pFishMgr->ClearAllFish(true);
	m_pBulletMgr->ClearAllBullet();
}
byte FishScene::GetSceneBackground()const
{
	return m_pFishMap->BackgroundType();
}
const WCHAR *FishScene::GetMapName()const
{
	return m_pFishMap->GetMapName();
}

USHORT FishScene::GetAngleByFish(WORD& LoackFishID, BYTE SeatID, Vector2& Pos, Vector2& pCenter)
{
	if (!m_pFishMgr)
	{
		LoackFishID = 0;
		return 0xffff;
	}
	//瞄准一条鱼 获取炮的炮台方向
	Fish *pFish = m_pFishMgr->GetFish(LoackFishID);
	if (!pFish || !pFish->IsInFrustum || !pFish->IsActive || pFish->IsDead)
	{
		FishVecList* pVec = m_pFishMgr->GetFishVecList();
		if (pVec->empty())
		{
			LoackFishID = 0;
			return 0xffff;
		}
		UINT32 Index = RandUInt() % pVec->size();
		pFish = pVec->at(Index);
		if (!pFish)
		{
			LoackFishID = 0;
			return 0xffff;
		}
		if (!pFish->IsInFrustum || !pFish->IsActive || pFish->IsDead)
		{
			LoackFishID = 0;
			return 0xffff;
		}
	}
	LoackFishID = pFish->FishID;


	Vector2 UpDir(0.0f, SeatID > 1 ? -1.0f : 1.0f); 
	Vector2 pos = pFish->ScreenPos;//鱼的坐标
	Vector2 Direction = (pos - pCenter);
	if (SeatID <= 1 && Direction.y < 0.1f)
		Direction.y = 0.1f;
	else if (SeatID > 1 && Direction.y > -0.1f)
		Direction.y = 0.1f;
	if (Direction.x < 0.1f && Direction.x > 0.0f)
		Direction.x = 0.1f;
	else if (Direction.x > -0.1f && Direction.x < 0.0f)
		Direction.x = -0.1f;
	float dirlen = Vec2Length(Direction);
	Direction /= dirlen;
	float dot = D3DXVec2Dot(&UpDir, &Direction);
	float Angle = acos(Clamp(dot, 0.0f, 1.0f)) * 57.2958f;
	if (Direction.x >= 0)
		Angle = -Angle;
	Angle = Clamp(Angle, -85, 85);
	short TempAngle = (short)(Angle / 90.0f * 32767);
	if (SeatID == 1 || SeatID == 0)
		TempAngle = TempAngle;
	else
		TempAngle = TempAngle * -1;
	return TempAngle;
}