#include "stdafx.h"
#include "BindEmailManager.h"
#include "FishServer.h"
BindEmailManager::BindEmailManager()
{

}
BindEmailManager::~BindEmailManager()
{

}
bool BindEmailManager::OnBindEmail(ServerClientData* pClient, GO_Cmd_BindEmail* pMsg)
{
	//绑定邮箱的数据.
	if (!pClient || !pMsg)
	{
		ASSERT(false);
		return false;
	}
	//发送一封确认邮件到对方邮箱去 等待操作

	//xuda 
	OnBindEmailResult(ROE_BindMail_Sucess,true, pMsg->dwUserID, pMsg->EMail);
	return true;
}
void BindEmailManager::OnBindEmailResult(BYTE ErrorID, bool Result, DWORD dwUserID, const TCHAR* pEmail)
{
	//UserID Email  玩家有可能不在 或者离线 我们想要进行处理 将命令转发到中央服务器去 做处理 
	OC_Cmd_BindEmail msg;
	SetMsgInfo(msg, GetMsgType(Main_Operate, OC_BindEmail), sizeof(OC_Cmd_BindEmail));
	msg.dwUserID = dwUserID;
	msg.ErrorID = ErrorID;
	msg.Result = Result;
	TCHARCopy(msg.EMail, CountArray(msg.EMail), pEmail, _tcslen(pEmail));
	g_FishServer.SendNetCmdToCenterServer(&msg);
}