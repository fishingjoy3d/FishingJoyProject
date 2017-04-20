//公告管理
#pragma once
#include <deque>
class AnnouncementManager
{
public:
	AnnouncementManager();
	virtual ~AnnouncementManager();
	void OnConnectionCenter();//当连接上中央服务器的时候 我们做处理
	void OnLoadAllAnnouncementInfoByCenter(CG_Cmd_GetAllAnnouncement* pMsg);
	//void OnLoadAllAnnouncementInfoFinish();

	void OnAddNewAnnouncementOnce(const TCHAR *pNickName,BYTE ShopID,BYTE ShopOnlyID);//添加一条公告 并且上传到Center去 在下发到客户端
	void OnAddNewAnnouncementOnceByCenter(AnnouncementOnce& pOnce);//中央服务器发送来的新的公告
	void SendNewAnnouncementToClent(DWORD dwUserID);//当客户端
private:
	bool								m_IsFinish;
	std::deque<AnnouncementOnce>		m_AnnouncementList;//双端队列
	std::vector<DWORD>					m_RoleVec;
};