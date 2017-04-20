//排队器
//需要在配置文件里添加 新的排队配置
//<FishSystem>
#pragma once
struct tagRoleQueueOnce
{
	DWORD		dwUserID;
	DWORD		ClientID;
	bool		LogonByGameServer;
};
class RoleQueueManager
{
public:
	RoleQueueManager();
	virtual ~RoleQueueManager();

	bool	 OnAddRoleToQueue(DWORD dwUserID, DWORD ClientID, bool LogonByGameServer);

	void	 OnUpdateQueue(DWORD dwTimer);

	DWORD	  GetWriteRoleSum(){ return m_QueueList.size(); }
private:
	DWORD							m_UpdateTimeLog;
	DWORD							m_QueueMsgUpate;
	std::list<tagRoleQueueOnce>		m_QueueList;
};