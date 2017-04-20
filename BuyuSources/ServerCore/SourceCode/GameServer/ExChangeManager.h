//兑换码管理器
#pragma once
class CRoleEx;
class ExChangeManager
{
public:
	ExChangeManager();
	virtual ~ExChangeManager();

	void OnUseExChangeCode(CRoleEx* pRole, CL_Cmd_RoleUseExChangeCode* pMsg);//使用兑换码
	void OnUseExChangeCodeDBResult(DBO_Cmd_QueryExChange* pMsg);
};