#include "stdafx.h"
#include "FishPlayer.h"
#include "FishScene\FTPUpload.h"
void FishPlayer::RandName()
{
	m_Name[0] = 0;
	m_RateIndex = 0;
}

void FishPlayer::SaveImg()
{
	static int MASK1 = GetTickCount();
	static int MASK2 = MASK1 + rand();

	UINT t = GetTickCount();
	UINT crc = AECrc32(&t, sizeof(t), 0);
	t = m_ImgSize;
	crc = AECrc32(&t, sizeof(t), crc);
	t = ++MASK1;
	crc = AECrc32(&t, sizeof(t), crc);
	t = ++MASK2;
	crc = AECrc32(&t, sizeof(t), crc);
	char xx[50];
	sprintf_s(xx, sizeof(xx), "%u_%u.jpg", GetID(), crc);
	FTPUpload::Inst()->UploadToFTP(m_pImgData, m_ImgSize, xx, GetID(), crc);
	
	SAFE_DELETE_ARR(m_pImgData);
}
bool FishPlayer::AddImgData(USHORT idx, USHORT size, byte *pData)
{
	if (m_pImgData && (idx + size) <= m_ImgSize && (m_UploadSize + size) <= m_ImgSize)
	{
		memcpy(m_pImgData + idx, pData, size);
		m_UploadSize += size;
		//上传完成
		if (m_UploadSize == m_ImgSize)
		{
			SaveImg();
		}
		return true;
	}
	else
	{
		SAFE_DELETE_ARR(m_pImgData);
		InnerFishLog(L"上传失败.");
		return false;
	}
}