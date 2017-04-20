//玩家的关系处理
#pragma once
#include "Stdafx.h"
#include <vector>
class CenterRole;
class CenterRoleManager;
class RoleRelationManager
{
public:
	RoleRelationManager();
	virtual ~RoleRelationManager();

	bool OnInit(CenterRole* pRole, CenterRoleManager* pManager);
	void OnLoadUserRelation(CC_Cmd_GetUserRelation* pMsg);
	void OnLoadUserBeRelation(CC_Cmd_LoadBeUserRelation* pMsg);
	//void OnLoadUSerRelationFinish(CC_Cmd_GetUserRelationFinish* pMsg);

	void OnAddUserRelation(CC_Cmd_AddUserRelation* pMsg);
	void OnDelUserRelation(CC_Cmd_DelUserRelation* pMsg);
	void OnChangeUserRelation(CC_Cmd_ChangeUserRelation* pMsg);

	void OnAddBeUserRelation(tagBeRoleRelation* pInfo);
	void OnDelBeUserRelation(DWORD dwSrcUserID);
	void OnChangeBeUserRelation(DWORD dwDestUserID, BYTE bRelationType);

	void OnRoleOnlinChange(bool IsOnline);
	void OnChangeBeRoleOnlineStateus(DWORD dwUserID, bool IsOnline);

	HashMap<DWORD, tagBeRoleRelation>& GetAllBeRelationInfo(){ return m_BeRelationMap; }

	//当前自己的关系发生改变的时候
	void OnChangeRelationUserLevel(DWORD dwUserID, WORD wLevel);
	void OnChangeReltaionUserFaceID(DWORD dwUserID, DWORD FaceID);
	void OnChanfeRelationUserNickName(DWORD dwUserID, LPTSTR pNickName);
	void OnChanfeRelationUserGender(DWORD dwUserID, bool bGender);
	void OnChangeRelationUserTitle(DWORD dwUserID, BYTE TitleID);
	void OnChangeRelationUserAchievementPoint(DWORD dwUserID, DWORD dwAchievementPoint);
	void OnChangeRelationUserCharmValue(DWORD dwUserID, DWORD CharmInfo[MAX_CHARM_ITEMSUM]);
	void OnChangeRelationUserClientIP(DWORD dwUserID, DWORD ClientIP);
	void OnChangeRelationUserIsShowIpAddress(DWORD dwUserID, bool States);
	void OnChangeRelationUserVipLevel(DWORD dwUserID, BYTE VipLevel);
	void OnChangeRelatuonUserIsInMonthCard(DWORD dwUserID, bool IsInMonthCard);
private:
	HashMap<DWORD, tagRoleRelation>  m_RelationMap;
	HashMap<DWORD, tagBeRoleRelation>  m_BeRelationMap;

	CenterRole*						m_pRole;
	CenterRoleManager*				m_RoleManager;
};