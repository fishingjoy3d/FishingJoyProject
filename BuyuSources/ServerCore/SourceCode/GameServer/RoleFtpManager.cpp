#include "Stdafx.h"
#include "RoleFtpManager.h"
#include "RoleEx.h"
#include "FishLogic\NetCmd.h"
#include "FishServer.h"
RoleFtpManager::RoleFtpManager()
{
	m_pRole = NULL;
}
RoleFtpManager::~RoleFtpManager()
{
	if (m_FaceData)
	{
		free(m_FaceData);
		m_FaceData = NULL;
	}
	m_DataLength = 0;
}
bool RoleFtpManager::OnInit(CRoleEx* pRole)
{
	if (!pRole)
	{
		ASSERT(false);
		return false;
	}
	m_pRole = pRole;
	m_FaceData = NULL;
	m_DataLength = 0;
	return true;
}
void RoleFtpManager::OnBeginUpLoadFaceData(WORD DataLength)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (DataLength == 0)
		return;
	if (m_FaceData)
		free(m_FaceData);
	m_DataLength = DataLength;
	m_FaceData = (BYTE*)malloc(DataLength);

	NetCmdUploadImgResponse pCmd;
	SetMsgInfo(pCmd,CMD_UPLOADING_RESPONSE, sizeof(NetCmdUploadImgResponse));
	pCmd.AllowUpload = true;
	m_pRole->SendDataToClient(&pCmd);
}
void RoleFtpManager::OnUpLoadFaceData(WORD StarIndex, WORD Length, BYTE* pData)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	//开始处理数据
	if (StarIndex + Length > m_DataLength || !pData)
		return;
	memcpy_s((VOID*)(m_FaceData + StarIndex), Length, pData, Length);
	bool bret = (StarIndex + Length == m_DataLength);
	if (bret)
	{
		//玩家自定义头像上传完毕 我们将头像保存到FTP去
		//1.发送命令到FTP去  并且等待FTP的返回结果处理
		//SaveImage
		static int MASK1 = GetTickCount();
		static int MASK2 = MASK1 + rand();

		UINT t = GetTickCount();
		UINT crc = AECrc32(&t, sizeof(t), 0);
		t = m_DataLength;
		crc = AECrc32(&t, sizeof(t), crc);
		t = ++MASK1;
		crc = AECrc32(&t, sizeof(t), crc);
		t = ++MASK2;
		crc = AECrc32(&t, sizeof(t), crc);

		char xx[50];
		sprintf_s(xx, sizeof(xx), "%u_%u.jpg", (UINT)m_pRole->GetUserID(), crc);//创建路径
		//将数据发送到FTP去
		DWORD cmdSize = sizeof(GF_Cmd_SaveImageData)+(m_DataLength - 1);
		CheckMsgSize(cmdSize);
		GF_Cmd_SaveImageData *pCmd = (GF_Cmd_SaveImageData*)malloc(cmdSize);
		pCmd->SetCmdSize(ConvertDWORDToWORD(cmdSize));
		pCmd->SetCmdType(GetMsgType(Main_Role, GF_SaveImageData));//命令类型
		pCmd->ID = m_pRole->GetUserID();
		strncpy_s(pCmd->ImgName, sizeof(pCmd->ImgName), xx, CountArray(xx));
		pCmd->ImgSize = m_DataLength;
		pCmd->Size = ConvertDWORDToWORD(cmdSize);
		pCmd->ImgCrc = crc;
		memcpy_s(pCmd->ImgData, m_DataLength, m_FaceData, m_DataLength);
		g_FishServer.SendNetCmdToFTP(pCmd); 
		free(pCmd);
		if (m_FaceData)
		{
			free(m_FaceData);
			m_FaceData = NULL;
		}
		m_DataLength = 0;
	}
	NetCmdUploadImgChunkResponse pCmd;
	SetMsgInfo(pCmd,CMD_UPLOADING_CHUNK_RESPONSE, sizeof(NetCmdUploadImgChunkResponse));
	pCmd.UploadResult = byte(byte(bret << 7) | 1);
	m_pRole->SendDataToClient(&pCmd);
}
void RoleFtpManager::OnUpLoadFaceDataResult(DWORD Crc, bool Result)
{
	if (!m_pRole)
	{
		ASSERT(false);
		return;
	}
	if (Result)
	{
		m_pRole->ChangeRoleFaceID(Crc);
	}
	NetCmdUploadImgComplete cmdResult;
	SetMsgInfo(cmdResult,CMD_UPLOADING_COMPLETE, sizeof(NetCmdUploadImgComplete));
	cmdResult.ImgCrcValue = Crc;
	cmdResult.Result = Result;
	m_pRole->SendDataToClient(&cmdResult);//发送到客户端去
}