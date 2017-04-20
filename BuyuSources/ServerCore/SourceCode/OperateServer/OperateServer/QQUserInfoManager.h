#pragma once
//QQ 获取QQ玩家的数据
struct tagQQUserInfoOnce
{
	UINT64  nOnlyID;
	DWORD	dwUserID;
	DWORD	dwClientID;
	DWORD	dwTimeLog;
	//外部数据
	TCHAR	chNickName[MAX_NICKNAME + 1];
	DWORD	dwFaceID;
	bool	bGender;
	tagQQUserInfoOnce()
	{
		nOnlyID = 0;
		dwUserID = 0;
		dwClientID = 0;
		dwTimeLog = 0;
		ZeroMemory(chNickName, sizeof(chNickName));
		dwFaceID = 0;
		bGender = false;
	}
};
class QQUserInfoManager
{
public:
	QQUserInfoManager();
	~QQUserInfoManager();

	//1.
	void HandleGetQQUserInfo(const ServerClientData* pClient, DWORD dwUserID, const char* pchToken, const char* pchOpenID);//根据数据发送命令到Web服务器去
	//2.
	void HandleQQUserInfoResult(UINT64 nOnlyID, char* pchData, DWORD dwLength);//处理微信玩家的返回结果
	//3.
	void HandleGetQQUserImg(UINT64 nOnlyID, const char* pchURL);
	//4.
	void HandleQQUserImgResult(UINT64 nOnlyID, char* pchData, DWORD dwLength);//处理图片的结果返回
	//5.
	void HandleSaveRoleFaceImgResult(DWORD dwUserID, bool Result, DWORD FaceID);
private:
	DWORD SendFaceImgToFTP(DWORD dwUserID, char* pFaceImgData, DWORD dwLength);
private:
	HashMap<DWORD, tagQQUserInfoOnce>		m_HashMapUserInfo;
};