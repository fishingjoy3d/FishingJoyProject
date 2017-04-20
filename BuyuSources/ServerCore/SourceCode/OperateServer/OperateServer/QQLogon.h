//QQ登陆
//上传用户的token数据 换取 OpenID 用户的唯一标记

#pragma once
#include <string>

struct tagQQUser
{
	UINT64		OnlyID;
	DWORD		ClientID;
	BYTE		LogonClientID;
	DWORD		TimeLog;//请求Token的时间 1分钟无反应 失败
	BYTE		PlateFormID;//平台的ID
	TCHAR		MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
};

class QQLogon
{
public:
	QQLogon();
	virtual ~QQLogon();

	void    HandleCode(ServerClientData* pClient, LO_Cmd_QQLogon* pMsg);//处理客户端发送来的code 数据
	void	OnHandleQQTokenResult(unsigned __int64 OnlyID, char* pData, DWORD Length);

	void	Update();
private:
	HashMap<UINT64, tagQQUser>			m_UserMap;
};