#pragma once
//双向关系的时候 我们在添加关系的时候 需要先确认
struct tagRelationResult
{
	tagRelationRequest	Info;
	bool				IsSrcOrDest;
};
class CRoleEx;
class RoleRelationRequest // 关系请求
{
public:
	RoleRelationRequest();
	virtual ~RoleRelationRequest();

	bool OnInit(CRoleEx* pRole);

	void OnUpdateByDay();

	void OnLoadRelationRequestResult(DBO_Cmd_LoadRelationRequest* pMsg);
	bool IsLoadDB(){ return m_IsLoadDB; }
	//
	void OnSendNewRequest(BYTE RelationType, DWORD DestUserID, LPTSTR MessageInfo);
	void OnSendNewRequestDBResult(DBO_Cmd_AddRelationRequest* pMsg);
	//
	void OnBeAddNewrequest(tagRelationRequest& pRequest);
	//
	void OnHandleRequest(DWORD ID, bool Result);
	void OnBeHandleRequest(tagRelationRequest& pRequest,bool Result);//被动的接收处理
	//客户端的请求
	void SendInfoToClient();
	//
	void OnAddRelationResult(DWORD SrcUserID, DWORD DestUserID, BYTE RelationType, bool Result);
	void OnDelRelation(DWORD DestUserID, BYTE RelationType);
	//
	void OnDelDestRelation(tagRelationRequest& pInfo);

	bool GetRelationRequestMessageStates();
private:
	bool CheckRelationRequestIsTimeOut(tagRelationRequest& pInfo);
	void OnDelRelationRequest(DWORD ID);
private:
	CRoleEx*								m_pRole;
	bool									m_IsLoadDB;
	HashMap<DWORD, tagRelationRequest>		m_RequestMap;//全部请求的列表
	HashMap<DWORD, tagRelationResult>		m_WriteHandleMap;
};