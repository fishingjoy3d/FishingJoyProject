#include "stdafx.h"
#include "RoleCache.h"
#include "FishServer.h"
RoleCache::RoleCache()
{

}
RoleCache::~RoleCache()
{

}
void RoleCache::OnAddRoleCache(DWORD dwUserID)
{
	HashMap<DWORD, DWORD>::iterator Iter = m_CacheRoleMap.find(dwUserID);
	if (Iter != m_CacheRoleMap.end())
		return;
	m_CacheRoleMap.insert(HashMap<DWORD, DWORD>::value_type(dwUserID, timeGetTime()));
}
void RoleCache::OnDleRoleCache(DWORD dwUserID)
{
	HashMap<DWORD, DWORD>::iterator Iter = m_CacheRoleMap.find(dwUserID);
	if (Iter == m_CacheRoleMap.end())
		return;
	m_CacheRoleMap.erase(Iter);
}
void RoleCache::UpdateByMin()
{
	if (m_CacheRoleMap.empty())
		return;
	DWORD Timer = timeGetTime();
	HashMap<DWORD, DWORD>::iterator Iter = m_CacheRoleMap.begin();
	for (; Iter != m_CacheRoleMap.end();)
	{
		if (Timer - Iter->second >= g_FishServer.GetFishConfig().GetSystemConfig().CacheUserMin*60000)//玩家通过缓存离线 无须设置玩家下线 因为玩家已经下线了
		{
			CRoleEx* pRole = g_FishServer.GetRoleManager()->QueryUser(Iter->first);
			if (pRole)
			{
				//pRole->ChangeRoleIsOnline(false);//先告诉其他玩家 离线了
				pRole->SendUserLeaveToCenter();//告诉中央服务器玩家离线
				g_FishServer.GetTableManager()->OnPlayerLeaveTable(pRole->GetUserID());
				g_FishServer.GetRoleManager()->OnDelUser(pRole->GetUserID(),true,true);//从内存中移除掉
			}
			Iter = m_CacheRoleMap.erase(Iter);
		}
		else
			++Iter;
	}
}
bool RoleCache::IsOpenRoleCache()//判断GameServer是否开启了缓存
{
	return (g_FishServer.GetFishConfig().GetSystemConfig().CacheUserMin != 0);
}