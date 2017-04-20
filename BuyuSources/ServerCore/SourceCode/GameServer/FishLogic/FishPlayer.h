#pragma once
#include "NetCmd.h"
#include "FishScene\Crc32.h"
#include "fishscene/fishresdata.h"
class FishPlayer
{
public:

	FishPlayer(PlayerID id, int seatIndex)
	{
		RandName();
		m_PlayerID = id;
		m_SeatIndex = seatIndex;
		m_BulletIdx = 0;
		m_LauncherType = 0;
		m_pImgData = NULL;
		m_ImgCrc = 0;
		m_bOK = false;
		m_Combo = 0;
		
	}
	~FishPlayer()
	{

	}
	byte GetBulletIdx()
	{
		if(++m_BulletIdx == MAX_BULLET_NUM)
			m_BulletIdx = 0;
		return m_BulletIdx;
	}
	PlayerID GetID()const
	{
		return m_PlayerID;
	}
	int		GetSeat()const
	{
		return m_SeatIndex;
	}
	const WCHAR *GetName()const
	{
		return m_Name;
	}
	void RandName();
	byte GetLauncherType()const
	{
		return m_LauncherType;
	}
	void AddLauncher(bool badd)
	{
		if(badd)
		{
			if(m_LauncherType == 4)
				m_LauncherType = 0;
			else
				++m_LauncherType;
		}
		else
		{
			if(m_LauncherType == 0)
				m_LauncherType = 4;
			else
				--m_LauncherType;
		}
	}
	void BeginUploadImg(USHORT size)
	{
		m_ImgSize = size;
		m_pImgData = new byte[size];
		m_UploadSize = 0;
	}
	bool AddImgData(USHORT idx, USHORT size, byte *pData);
	bool IsUploadComplete()const
	{
		return m_UploadSize == m_ImgSize;
	}
	uint GetImgCrc()const
	{
		return m_ImgCrc;
	}
	void SetNewImgCrc(UINT crc)
	{
		m_ImgCrc = crc;
	}
	bool IsOK()const
	{
		return m_bOK;
	}
	void SetOK()
	{
		m_bOK = true;
	}
	void SaveImg();
	void ChangeRateIndex(byte idx, byte roomRateIndex)
	{
		if(++m_RateIndex == 4)
			m_RateIndex = roomRateIndex;
	}
	byte GetRateIndex()
	{
		return m_RateIndex;
	}
	BYTE AddCombo()
	{
		if(++m_Combo == 6)
			m_Combo = 0;
		return m_Combo;
	}
	BYTE GetCombo()const
	{
		return m_Combo;
	}
	void SetImgCrc(uint crc)
	{
		m_ImgCrc = crc;
	}
protected:
	byte		m_Combo;
	byte		m_RateIndex;
	bool		m_bOK;
	byte		m_BulletIdx;
	PlayerID	m_PlayerID;
	int			m_SeatIndex;
	WCHAR		m_Name[4];
	byte		m_LauncherType;
	byte		*m_pImgData;
	USHORT		m_ImgSize;
	USHORT		m_UploadSize;
	uint		m_ImgCrc;
};