//玩家好友信息
#pragma once
#include "Stdafx.h"
#include <vector>
//关系应该与中央服务器进行通讯处理
class CRoleEx;
class RoleManager;
class RoleRelationManager //玩家的关系管理类
{
public:
	RoleRelationManager();
	virtual ~RoleRelationManager();

	bool	OnInit(CRoleEx* pUser, RoleManager* pManager);
	bool    OnLoadUserRelation();
	void	OnLoadUserRelationResult(DBO_Cmd_LoadUserRelation* pDB);
	void	OnLoadBeUserRelationResult(DBO_Cmd_LoadBeUserRelation* pDB);
	//void	OnLoadBeUserRelationFinish();

	bool	OnAddUserRelation(CL_Cmd_AddUserRelation* pMsg);
	void	OnAddUserRelationResult(DBO_Cmd_AddUserRelation* pDB);

	bool	OnDelUserRelation(CL_Cmd_DelUserRelation* pDB);

	//bool    OnChangeUserRelation(CL_Cmd_ChangeUserRelation* pMsg);

	void   OnAddBeUserRelation(tagBeRoleRelation* pInfo);
	void   OnDelBeUserRelation(DWORD dwDestUserID);
	void   OnChagneBeUserRelation(DWORD dwDestUserID, BYTE bRelationType);

	bool   OnGetUserRelation();
	void   SendRoleRelationToCenter();
	void   OnChangeUserOnline(DWORD dwUserID, bool IsOnline);

	//中央服务器发送命令修改关系数据
	void   OnChangeRelationLevel(DWORD dwUserID, WORD wLevel);
	void   OnChangeRelationFaceID(DWORD dwUserID, DWORD FaceID);
	void   OnChangeRelationGender(DWORD dwUserID, bool bGender);
	void   OnChangeRelationNickName(DWORD dwUserID, LPTSTR pNickName);
	void   OnChangeRelationTitle(DWORD dwUserID, BYTE TitleID);
	void   OnChangeRelationAchievementPoint(DWORD dwUserID, DWORD dwAchievementPoint);
	void   OnChangeRelationCharmValue(DWORD dwUserID, DWORD pCharm[MAX_CHARM_ITEMSUM]);
	void   OnChangeRelationCLientIP(DWORD dwUserID, DWORD ClientIP);
	void   OnChangeRelationIsShowIpAddress(DWORD dwUserID, bool States);
	void   OnChangeRelationVipLevel(DWORD dwUserID, BYTE VipLevel);
	void   OnChangeRelationIsInMonthCard(DWORD dwUserID, bool IsInMonthCard);

	bool IsLoadDB(){ return m_IsLoadDB; }

	void ResetClientInfo(){ m_IsLoadToClient = false; }

	tagRoleRelation* QueryRelationInfo(DWORD dwDestUserID);
private:
	bool							m_IsLoadToClient;
	//指针
	RoleManager*					m_RoleManager;
	CRoleEx*						m_pUser;

	HashMap<DWORD, tagRoleRelation>	 m_RelationMap;
	HashMap<DWORD, tagBeRoleRelation> m_BeRelationMap;

	//std::vector<tagRoleRelation>	m_RelationVec;//玩家全部的关系数据
	//HashMap<DWORD, RelationType>   m_BeRelationMap;//被动关系的列表
	bool							m_IsLoadDB;
};