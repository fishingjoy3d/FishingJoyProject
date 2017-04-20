#include "Stdafx.h"
#include"oxrobot.h"
#include "math.h"
#include "..\FishServer.h"
#include"NiuNiuManager.h"
#include"..\RoleManager.h"

//////////////////////////////////////////////////////////////////////////
//最大下注次数
#define MAX_CHIP_TIME			  50
//时间标识
#define IDI_BANK_OPERATE			3									//银行定时
#define IDI_CHECK_BANKER			108									//检查上庄
#define IDI_REQUEST_BANKER			101									//申请定时
#define IDI_GIVEUP_BANKER			102									//下庄定时
#define IDI_PLACE_JETTON			110									//下注定义 (预留110-160)

DWORD COxRobot::m_dwApplistChangeTime;
BYTE	COxRobot::m_byAppCountLimit;

DWORD	COxRobot::m_dwRobotCountChangeTime;
BYTE	COxRobot::m_byRobotCount;

//////////////////////////////////////////////////////////////////////////
//构造函数
COxRobot::COxRobot(DWORD dwRobotid)
{	
	//游戏变量
	m_bAction = rand() % 100 < GameConfig().RobotActionRate;
	m_bBnkerRobot = rand() % 100 < GameConfig().RobotBankerRate;
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;	
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//上庄变量
	//m_bMeApplyBanker = false;
	m_nWaitBanker = 0;
	m_nRobotWaitBanker = ClipNum(GameConfig().RobotWaitBankerMin, GameConfig().RobotWaitBankerMax);
	m_nBankerCount = 0;
	m_dwRobotid = dwRobotid;
	m_nGameTime = ClipNum(GameConfig().RobotMinGameTime, GameConfig().RobotMaxGameTime) * 60;	
	m_nRobotBankerCount = ClipNum(GameConfig().RobotBankerCountMin, GameConfig().RobotBankerCountMax);
	return;
}

//析构函数
COxRobot::~COxRobot()
{
}

//时间消息
bool  COxRobot::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_CHECK_BANKER:		//检查上庄
	{
									

									if (BankerId() == UserId())
										return false;

									if (ApplyListLength()<COxRobot::m_byAppCountLimit&&!g_FishServer.GetNiuNiuManager().HaveApply(UserId()))//BankerId() == 0 ||
									{
										//空庄
										m_nWaitBanker++;

										//MyDebug(_T("机器人上庄(End) %d [%d %d] [%d %d]"), m_pIAndroidUserItem->GetChairID(), m_nWaitBanker, 
										//	m_nRobotWaitBanker, m_stlApplyBanker, m_nRobotApplyBanker);

										//机器人上庄
										if (m_bBnkerRobot&& m_nWaitBanker > m_nRobotWaitBanker)
										{
											//合法判断											
											if (UserGold() > GameConfig().JoinBankerGlobelSum)
											{
												//机器人上庄
												m_nBankerCount = 0;//被动提
												m_nWaitBanker = 0;
												g_FishServer.GetNiuNiuManager().OnRoleJoinWriteBankerList(UserId());												
											}
										}
									}
									//else if (BankerId() != 0)
									//{
									//	//其他人坐庄
									//	
									//}

									return false;
	}
	case IDI_GIVEUP_BANKER:		//申请下庄
	{									
									g_FishServer.GetNiuNiuManager().OnRoleCanelBanker(UserId());								
									return false;
	}
	case IDI_BANK_OPERATE:		//银行操作
	{
									if (m_bBnkerRobot)
									{
										if (UserGold() < GameConfig().RobotBankerScoreMin || UserGold() < GameConfig().RobotBankerScoreMax)
										{
											AdjustGold(ClipNum(GameConfig().RobotBankerScoreMin, GameConfig().RobotBankerScoreMax));																			 											
										}
									}
									else
									{
										if (UserGold() < GameConfig().RobotPlayerScoreMin || UserGold() < GameConfig().RobotPlayerScoreMax)
										{																															
											AdjustGold(ClipNum(GameConfig().RobotPlayerScoreMin, GameConfig().RobotPlayerScoreMax));
										}
									}																
									return false;
	}
	default:
	{
			   if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON + MAX_CHIP_TIME)
			   {				  				 
				   //变量定义
				   int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
				   LONGLONG lMaxChipLmt = __min(m_lMaxChipBanker, m_lMaxChipUser);			//最大可下注值				   
				   for (int i = 0; i <MAX_NIUNIU_ClientSum; i++)
					   nACTotal += m_RobotInfo.nAreaChance[i];

				   //统计次数
				   m_nChipTimeCount++;

				   //检测退出
				   if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])	return false;
				   for (int i = 0; i < MAX_NIUNIU_ClientSum; i++)
				   {
					   if (m_lAreaChip[i] >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
					   if (i == MAX_NIUNIU_ClientSum - 1)	return false;
				   }

				   //下注区域
				   ASSERT(nACTotal>0);
				   static int nStFluc = 1;				//随机辅助
				   if (nACTotal <= 0)	return false;
				   do {
					   nRandNum = (rand()  + nStFluc * 3) % nACTotal;
					   for (int i = 0; i < MAX_NIUNIU_ClientSum; i++)
					   {
						   nRandNum -= m_RobotInfo.nAreaChance[i];
						   if (nRandNum < 0)
						   {
							   nChipArea = i;
							   break;
						   }
					   }
				   } while (m_lAreaChip[nChipArea] < m_RobotInfo.nChip[m_nChipLimit[0]]);
				   nStFluc = nStFluc % 3 + 1;

				   //下注大小
				   if (m_nChipLimit[0] == m_nChipLimit[1])
					   nCurChip = m_nChipLimit[0];
				   else
				   {
					   //设置变量
					   lMaxChipLmt = __min(lMaxChipLmt, m_lAreaChip[nChipArea]);
					   nCurJetLmt[0] = m_nChipLimit[0];
					   nCurJetLmt[1] = m_nChipLimit[0];

					   //计算当前最大筹码
					   for (int i = m_nChipLimit[1]; i > m_nChipLimit[0]; i--)
					   {
						   if (lMaxChipLmt > m_RobotInfo.nChip[i])
						   {
							   nCurJetLmt[1] = i;
							   break;
						   }
					   }

					   //随机下注
					   nRandNum = (rand()) % (nCurJetLmt[1] - nCurJetLmt[0] + 1);
					   nCurChip = nCurJetLmt[0] + nRandNum;

					   //多下控制 (当庄家金币较少时会尽量保证下足次数)
					   if (m_nChipTimeCount < m_nChipTime)
					   {
						   LONGLONG lLeftJetton = LONGLONG((lMaxChipLmt - m_RobotInfo.nChip[nCurChip]) / (m_nChipTime - m_nChipTimeCount));

						   //不够次数 (即全用最小限制筹码下注也少了)
						   if (lLeftJetton < m_RobotInfo.nChip[m_nChipLimit[0]] && nCurChip > m_nChipLimit[0])
							   nCurChip--;
					   }
				   }
				   int nJetton = m_RobotInfo.nChip[nCurChip];
				   g_FishServer.GetNiuNiuManager().OnRoleBetGlobel(UserId(), nChipArea, nJetton);

				   //设置变量
				   m_lMaxChipBanker -= nJetton;
				   m_lAreaChip[nChipArea] -= nJetton;
				   if (BankerId() == UserId())
				   {
					   m_lMaxChipUser -= nJetton;
				   }					   

				   /*ASSERT( MyDebug(_T("机器人下注 %d 下注次数 [%d/%d] 下注 [%d %d] 范围 [%d %d] 限制 [%I64d %I64d %I64d]"), wMyID, nTimerID-IDI_PLACE_JETTON, m_nChipTime,
				   nChipArea, m_RobotInfo.nChip[nCurChip], m_nChipLimit[0], m_nChipLimit[1], m_lMaxChipBanker, m_lMaxChipUser, lMaxChipLmt) );*/

				   //变量定义
				   //CMD_C_PlaceJetton PlaceJetton = {};

				   ////构造变量
				   //PlaceJetton.cbJettonArea = nChipArea + 1;		//区域宏从1开始
				   //PlaceJetton.lJettonScore = m_RobotInfo.nChip[nCurChip];

				   ////发送消息
				   //m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			   }

			   //m_pIAndroidUserItem->KillGameTimer(nTimerID);
			   return false;
	}
	}
	return false;
}


//游戏空闲
void COxRobot::EndGame()
{
	if (!m_bAction)
	{
		return;
	}
	m_VecOxTimer.push_back(new OxTimer(IDI_BANK_OPERATE, (WORD)rand() % (GameConfig().EndWriteSec / 2) + 1));
	bool bMeGiveUp = false;
	if (BankerId() == UserId())
	{
		m_nBankerCount++;
		if (m_nBankerCount >= m_nRobotBankerCount)
		{			
			m_nBankerCount = 0;//机器人走庄
			m_VecOxTimer.push_back(new OxTimer(IDI_GIVEUP_BANKER,(WORD) rand() % (GameConfig().EndWriteSec / 2)+1));
			bMeGiveUp = true;
		}
	}
	//检查上庄
	if (BankerId() != UserId() || bMeGiveUp)
	{
		
		m_VecOxTimer.push_back(new OxTimer(IDI_CHECK_BANKER, (WORD)rand() % (GameConfig().EndWriteSec / 2) + 1));
	}
}
void COxRobot::AdjustGold(DWORD dwGold)
{
	Role *pRole = g_FishServer.GetRoleManager().QueryRole(m_dwRobotid);
	if (pRole)
	{
		pRole->OnChangeRoleGlobelSum(dwGold,true);
	}
}

//游戏开始
void COxRobot::StartGame()
{
	if (!m_bAction)
	{
		return;
	}
	//自己当庄或无下注机器人
	if (UserId() == BankerId())
		return;

	if (BankerId() == 0)
	{
		m_lMaxChipBanker = GameConfig().MaxAddGlobel;// / GameConfig().MaxRate;
	}
	else
	{
		m_lMaxChipBanker = BankerGold() / GameConfig().MaxRate;
	}
	m_lMaxChipUser = UserGold() / GameConfig().MaxRate;
	m_lMaxChipUser = __min(m_lMaxChipUser, m_lMaxChipBanker);
	//设置变量
	
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	for (int i = 0; i <MAX_NIUNIU_ClientSum; i++)
		m_lAreaChip[i] = m_lMaxChipUser;

	//计算下注次数
	int nElapse = 0;

	if (GameConfig().RobotMinBetTime == GameConfig().RobotMaxBetTime)
		m_nChipTime = GameConfig().RobotMinBetTime;
	else
		m_nChipTime = (rand()) % (GameConfig().RobotMaxBetTime - GameConfig().RobotMinBetTime + 1) + GameConfig().RobotMinBetTime;
	ASSERT(m_nChipTime >= 0);
	if (m_nChipTime <= 0)	return ;								//的确,2个都带等于
	if (m_nChipTime > MAX_CHIP_TIME)	m_nChipTime = MAX_CHIP_TIME;	//限定MAX_CHIP次下注

	LONGLONG lRobotJettonLimit[2] = {0};
	lRobotJettonLimit[0] = GameConfig().RobotMinJetton;
	lRobotJettonLimit[1] = GameConfig().RobotMaxJetton;

	//计算范围
	if (!CalcJettonRange(__min(m_lMaxChipBanker, m_lMaxChipUser), lRobotJettonLimit, m_nChipTime, m_nChipLimit))
		return ;

	//设置时间
	int nTimeGrid = int(GameConfig().BeginWriteSec - 2) * 800 / m_nChipTime;		//时间格,前2秒不下注,所以-2,800表示机器人下注时间范围千分比
	for (int i = 0; i < m_nChipTime; i++)
	{
		int nRandRage = int(nTimeGrid * i / (1500 * sqrt((double)m_nChipTime))) + 1;		//波动范围
		nElapse = 2 + (nTimeGrid*i) / 1000 + ((rand()) % (nRandRage * 2) - (nRandRage - 1));
		//ASSERT(nElapse >= 2 && nElapse <= pGameStart->cbTimeLeave);
		if (nElapse < 2 || nElapse > (int)GameConfig().BeginWriteSec)	continue;

		m_VecOxTimer.push_back(new OxTimer(IDI_PLACE_JETTON + i + 1, nElapse));		
	}

	//ASSERT( MyDebug(_T("机器人 %d 下注次数 %d 范围 [%d %d] 总人数 %d 限制 [%I64d %I64d] 上庄 [%d %d]"), wMyID, m_nChipTime, m_nChipLimit[0], m_nChipLimit[1], 
	//	pGameStart->nChipRobotCount, m_lMaxChipBanker, m_lMaxChipUser, m_stlApplyBanker, m_nRobotApplyBanker) );

	return ;
}

//计算范围	(返回值表示是否可以通过下降下限达到下注)
bool COxRobot::CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[])
{
	//定义变量
	bool bHaveSetMinChip = false;

	//不够一注
	if (lMaxScore < m_RobotInfo.nChip[0])	return false;

	//配置范围
	for (int i = 0; i < CountArray(m_RobotInfo.nChip); i++)
	{
		if (!bHaveSetMinChip && m_RobotInfo.nChip[i] >= lChipLmt[0])
		{
			lJetLmt[0] = i;
			bHaveSetMinChip = true;
		}
		if (m_RobotInfo.nChip[i] <= lChipLmt[1])
			lJetLmt[1] = i;
	}
	if (lJetLmt[0] > lJetLmt[1])	lJetLmt[0] = lJetLmt[1];

	//是否降低下限
	if (m_bReduceJettonLimit)
	{
		if (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
		{
			//是否降低下注次数
			if (nChipTime * m_RobotInfo.nChip[0] > lMaxScore)
			{
				nChipTime = int(lMaxScore / m_RobotInfo.nChip[0]);
				lJetLmt[0] = 0;
				lJetLmt[1] = 0;
			}
			else
			{
				//降低到合适下限
				while (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
				{
					lJetLmt[0]--;
					ASSERT(lJetLmt[0] >= 0);
				}
			}
		}
	}

	return true;
}

bool COxRobot::Update()
{
	for (vector<OxTimer*>::iterator it = m_VecOxTimer.begin(); it != m_VecOxTimer.end();)
	{		
		if (--(*it)->m_nTime == 0)
		{
			OnEventTimer((*it)->m_byTimerid);
			delete (*it);
			it = m_VecOxTimer.erase(it);
		}
		else
		{
			it++;
		}
	}
	if (m_nGameTime > 0)
	{
		m_nGameTime--;
	}	
	return m_nGameTime != 0;
}

DWORD COxRobot::UserId()
{
	return m_dwRobotid;
}
DWORD COxRobot::UserGold()
{
	Role* pRole=g_FishServer.GetRoleManager().QueryRole(m_dwRobotid);
	if (pRole)
	{
		return pRole->GetGlobelSum();
	}
	return 0;
}
DWORD COxRobot::BankerId()
{
	return g_FishServer.GetNiuNiuManager().BankerId();
}

DWORD COxRobot::BankerGold()
{
	return g_FishServer.GetNiuNiuManager().BankerGold();
}

WORD  COxRobot::ApplyListLength()
{
	return g_FishServer.GetNiuNiuManager().ApplyListLength();
}


tagNiuNiuConfig COxRobot::GameConfig()
{
	return g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig;
}
/////////////

COxRobotManager::COxRobotManager()
{
	srand(GetTickCount());
}

COxRobotManager::~COxRobotManager()
{

}
//void COxRobotManager::Init(NiuNiuManager *pgame)
//{
//	m_pGame = pgame;
//}

void COxRobotManager::Update()
{
	if ((GetTickCount() - COxRobot::m_dwRobotCountChangeTime) / 1000 > (DWORD)(g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.RobotCountInterval * 60))//总人数控制
	{
		COxRobot::m_dwRobotCountChangeTime = GetTickCount();
		SYSTEMTIME st;
		GetLocalTime(&st);
		for (vector<RobotOxCountControl>::iterator it = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.VecRobotCount.begin(); it != g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.VecRobotCount.end(); it++)
		{
			if (st.wHour >= (*it).StartTime&&st.wHour <= (*it).EndTime)
			{
				BYTE byCount1 = (*it).RobotCountMin;
				BYTE byCount2 = (*it).RobotCountMax;
				if (byCount1<=byCount2)
				{
					COxRobot::m_byRobotCount = byCount1 + rand() % (byCount2 - byCount1 + 1);
				}					
				break;
			}
		}
	}

	if ((GetTickCount() - COxRobot::m_dwApplistChangeTime) / 1000>(DWORD)(g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.RobotListCountInterval * 60))//申请列表人数限制
	{
		COxRobot::m_dwApplistChangeTime = GetTickCount();
		BYTE  bymlistmin = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.RobotListCountMin;
		BYTE  bylistmax = g_FishServer.GetFishConfig().GetFishMiNiGameConfig().niuniuConfig.RobotListCountMax;
		if (bymlistmin <= bylistmax)
		{
			COxRobot::m_byAppCountLimit = bymlistmin + rand() % (bylistmax - bymlistmin + 1);
		}
	}

	if (m_allrobot.size() < COxRobot::m_byRobotCount)
	{
		RobotUserInfo *pInfo=g_FishServer.GetRobotManager().GetNonUseRobot();
		if (pInfo)
		{	
			COxRobot *probot = new COxRobot(pInfo->dwUserID);//分钟到秒
			g_FishServer.GetNiuNiuManager().OnRoleJoinNiuNiuTable(pInfo->dwUserID);
			m_allrobot.push_back(probot);
		}		
	}

	for (vector<COxRobot*>::iterator it = m_allrobot.begin(); it != m_allrobot.end();)
	{		
		if (!(*it)->Update() && g_FishServer.GetNiuNiuManager().IsGameEnd())
		{		
			g_FishServer.GetRobotManager().OnNonUseRobot((*it)->UserId());
			g_FishServer.GetNiuNiuManager().OnRoleLeaveNiuNiuTable((*it)->UserId());
			delete (*it);
			it=m_allrobot.erase(it);
		}
		else
		{
			it++;
		}
	}	
}

void COxRobotManager::StartGame()
{
	for (vector<COxRobot*>::iterator it = m_allrobot.begin(); it != m_allrobot.end();it++)
	{
		(*it)->StartGame();
	}
}

void COxRobotManager::EndGame()
{
	for (vector<COxRobot*>::iterator it = m_allrobot.begin(); it != m_allrobot.end();it++)
	{
		(*it)->EndGame();
	}
}
bool COxRobotManager::IsRobot(DWORD dwUserid)
{
	for (vector<COxRobot*>::iterator it = m_allrobot.begin(); it != m_allrobot.end(); it++)
	{
		if ((*it)->UserId() == dwUserid)
		{
			return true;
		}
	}
	return false;
}
USHORT COxRobotManager::RobotCount()
{
	return m_allrobot.size();
}