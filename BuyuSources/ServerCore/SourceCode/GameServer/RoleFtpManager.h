//玩家的FTP管理器
//用于处理客户端的数据
#pragma once
#include "Stdafx.h"
class CRoleEx;
class RoleFtpManager
{
public:
	RoleFtpManager();
	virtual ~RoleFtpManager();

	bool OnInit(CRoleEx* pRole);

	void OnBeginUpLoadFaceData(WORD DataLength);
	void OnUpLoadFaceData(WORD StarIndex, WORD Length, BYTE* pData);
	void OnUpLoadFaceDataResult(DWORD Crc,bool Result);//上传头像最终的结果
private:
	CRoleEx*		m_pRole;
	BYTE*			m_FaceData;
	WORD			m_DataLength;
};