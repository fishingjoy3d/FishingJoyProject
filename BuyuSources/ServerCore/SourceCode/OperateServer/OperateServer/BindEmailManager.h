#pragma once
//∞Û∂®” œ‰
class BindEmailManager
{
public:
	BindEmailManager();
	virtual ~BindEmailManager();

	bool OnBindEmail(ServerClientData* pClient, GO_Cmd_BindEmail* pMsg);
	void OnBindEmailResult(BYTE ErrorID,bool Result, DWORD dwUserID, const TCHAR* pEmail);
};