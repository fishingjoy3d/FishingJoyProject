//微信登陆设置
//微信基本登陆流程
//1.通过code获取access_token  code 客户端发送过来
//2.通过 access_token 获取玩家基本数据
#pragma once
#include <string>

struct tagWeiXinUser
{
	UINT64		OnlyID;
	DWORD		ClientID;
	BYTE		LogonClientID;
	DWORD		TimeLog;//请求Token的时间 1分钟无反应 失败
	BYTE		PlateFormID;//平台的ID
	TCHAR		MacAddress[MAC_ADDRESS_LENGTH + 1];//Mac地址
};

class WeiXinLogon
{
public:
	WeiXinLogon();
	virtual ~WeiXinLogon();

	void    HandleCode(ServerClientData* pClient, LO_Cmd_WeiXinLogon* pMsg);//处理客户端发送来的code 数据
	void	OnHandleWeiXinTokenResult(unsigned __int64 OnlyID, char* pData, DWORD Length);

	void	Update();
private:
	HashMap<UINT64, tagWeiXinUser>			m_UserMap;
	DWORD									m_dwOpenIPCrc32;
};