#pragma once
#include <deque>
class AnnouncementManager
{
public:
	AnnouncementManager();
	virtual ~AnnouncementManager();

	//从数据库加载公告
	void OnLoadAllAnnouncementByDB();
	void OnLoadAllAnnouncementByDBResult(DBO_Cmd_LoadALlAnnouncement* pMsg);

	void OnSendAllAnnouncementToGameServer(BYTE GameID);//将公告发送给GameServer
	void OnAddNewAnnouncementOnce(AnnouncementOnce& pOnce);//添加新的公告
private:
	bool								m_IsFinish;
	std::vector<BYTE>					m_GameServerVec;
	std::deque<AnnouncementOnce>		m_AnnouncementList;//双端队列
};
