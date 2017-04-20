#include "Stdafx.h"
#include "RoleProtect.h"
#include "FishServer.h"
#include "FishLogic\NetCmd.h"
RoleProtect::RoleProtect()
{

}
RoleProtect::~RoleProtect()
{

}
bool RoleProtect::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	return true;
}
bool RoleProtect::IsWaiting()
{
	if (m_pRole->GetRoleMonth().IsInMonthTable())
	{
		return false;
	}
	DWORD nMinGold = 0;
	CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());
	if (pRole)
	{
		nMinGold = pRole->MinBulletCosume();
	}
	else
	{
		nMinGold = g_FishServer.GetFishConfig().GetRoleProtectConfig().OpenProtectGlobelSum;
	}

	if (m_pRole->GetRoleInfo().dwGlobeNum > nMinGold)
	{		
		return false;
	}
	if (m_pRole->GetRoleServerInfo().RoleProtectSum >= m_pRole->GetRoleVip().AddAlmsSum())
	{		
		return false;
	}
	return time(null) - m_pRole->GetRoleServerInfo().RoleProtectLogTime < g_FishServer.GetFishConfig().GetRoleProtectConfig().ProtectCDMin * 60;
}

bool RoleProtect::CanFetch()
{
	if (m_pRole->GetRoleMonth().IsInMonthTable())
	{
		return false;
	}
	DWORD nMinGold = 0;
	CRole* pRole = g_FishServer.GetTableManager()->SearchUser(m_pRole->GetUserID());
	if (pRole)
	{
		nMinGold = pRole->MinBulletCosume();	
	}
	else
	{
		nMinGold = g_FishServer.GetFishConfig().GetRoleProtectConfig().OpenProtectGlobelSum;
	}

	if (m_pRole->GetRoleInfo().dwGlobeNum > nMinGold)
	{
		ASSERT(false);
		return false;
	}	
	if (m_pRole->GetRoleServerInfo().RoleProtectSum >= m_pRole->GetRoleVip().AddAlmsSum())
	{
		ASSERT(false);
		return false;
	}
		
	if (time(null) - m_pRole->GetRoleServerInfo().RoleProtectLogTime < g_FishServer.GetFishConfig().GetRoleProtectConfig().ProtectCDMin * 60)
	{
		ASSERT(false);
		return false;
	}		
	return true;
}

void RoleProtect::OnUserNonGlobel()
{
	//当玩家没有金币的时候
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//LC_Cmd_RoleProtectStatus msg;
	//msg.setat = byseat;
	//msg.poor = bpoor;
	//SetMsgInfo(msg, GetMsgType(Main_Role, LC_RoleProtectStatus), sizeof(LC_Cmd_RoleProtectStatus));
	//m_pRole->send
	//m_pRole->SendDataToClient(&msg);



	//m_pRole->GetRoleVip().AddAlmsSum();
	//m_pRole->GetRoleVip().AddAlmsRate();

	//4.发送命令到客户端去 通知客户端 已经触发保护了 可以弹窗口了
	
	

	
}
void RoleProtect::Request()
{
	if (CanFetch())
	{
		m_pRole->AddRoleProtectSum();
		m_pRole->ChangeRoleGlobe((int)(g_FishServer.GetFishConfig().GetRoleProtectConfig().AddGlobel*m_pRole->GetRoleVip().AddAlmsRate()), true);

		LC_Cmd_RoleProtect msg;
		SetMsgInfo(msg, GetMsgType(Main_Role, LC_RoleProtect), sizeof(LC_Cmd_RoleProtect));
		m_pRole->SendDataToClient(&msg);
	}
}