#pragma once

struct AE_CRC_PAIRS
{
	AE_CRC_PAIRS(){
	}
	AE_CRC_PAIRS(UINT c1, UINT c2) :Crc1(c1), Crc2(c2){
	}
	AE_CRC_PAIRS(const AE_CRC_PAIRS &crc){
		operator=(crc);
	}
	operator size_t()const{
		return Crc1;
	}
	bool operator==(const AE_CRC_PAIRS &crc)const{
		return Crc1 == crc.Crc1 && Crc2 == crc.Crc2;
	}
	bool operator!=(const AE_CRC_PAIRS &crc)const{
		return Crc1 != crc.Crc1 || Crc2 != crc.Crc2;
	}
	AE_CRC_PAIRS& operator=(const AE_CRC_PAIRS &crc){
		Crc1 = crc.Crc1;
		Crc2 = crc.Crc2;
		return *this;
	}

	UINT Crc1;
	UINT Crc2;
};
struct AE_CRC_THREE
{
	AE_CRC_THREE(){
	}
	AE_CRC_THREE(UINT c1, UINT c2, UINT c3) :Crc1(c1), Crc2(c2), Crc3(c3){
	}
	AE_CRC_THREE(const AE_CRC_THREE &crc){
		operator=(crc);
	}
	operator size_t()const{
		return Crc1;
	}
	bool operator==(const AE_CRC_THREE &crc)const{
		return Crc1 == crc.Crc1 && Crc2 == crc.Crc2 && Crc3 == crc.Crc3;
	}
	bool operator!=(const AE_CRC_THREE &crc)const{
		return Crc1 != crc.Crc1 || Crc2 != crc.Crc2 || Crc3 != crc.Crc3;
	}
	AE_CRC_THREE& operator=(const AE_CRC_THREE &crc){
		Crc1 = crc.Crc1;
		Crc2 = crc.Crc2;
		Crc3 = crc.Crc3;
		return *this;
	}

	UINT Crc1;
	UINT Crc2;
	UINT Crc3;
};
UINT	AECrc32(const void *pData, UINT size, UINT CrcValue);
BOOL	AECrc32(AE_CRC_PAIRS &retCrc, const void *pData, UINT size, UINT CrcValue1, UINT CrcValue2);
BOOL	AECrc32(AE_CRC_THREE &retCrc, const void *pData, UINT size, UINT CrcValue1, UINT CrcValue2, UINT CrcValue3);