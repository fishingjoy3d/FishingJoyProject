#pragma once
class CRoleEx;
class RoleProtect
{
public:
	RoleProtect();
	virtual ~RoleProtect();
	bool OnInit(CRoleEx* pRole);
	bool CanFetch();
	bool IsWaiting();
	void OnUserNonGlobel();
	void Request();
private:
	CRoleEx*		m_pRole;
};