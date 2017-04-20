#pragma once

#ifndef FD_SETSIZE
#define FD_SETSIZE  10000
#endif

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>
#include <MSWSock.h>  
#include <process.h>
#include <algorithm>
#include <tchar.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string>
#include <assert.h>
#include <mmsystem.h>
#include <sstream>
#include <time.h>

struct NetCmd;
#undef ASSET
using namespace std;

class ILogHandler
{
public:
	virtual void LogHandlerW(const WCHAR *pcStr) = 0;
	virtual void LogHandlerA(const CHAR *pcStr) = 0;
};

//全局函数
extern void Log(const WCHAR *pcStr, ...);
extern void Log(const CHAR *pcStr, ...);
extern void SetLogHandler(ILogHandler *handler);
extern const WCHAR* Sprintf(const WCHAR *pcStr, ...);

//全局的参数设置
enum
{
	DB_SERVER_PORT		= 14212,
	GAME_SERVER_PORT	= 14213,
	SCENE_SERVER_PORT	= 14214,
	CENTRAL_SERVER_PORT = 14215,
	ACCOUNT_NAME_LENGTH = 32,
	TABLE_NAME_LENGTH	= 32,
};

//常用宏定义
#define null NULL
#define SAFE_FREE(X)		if(X) {::free (X); (X) = NULL;}
#define SAFE_DELETE(X)		if(X) {delete (X); (X) = NULL;}
#define SAFE_DELETE_ARR(X)	if(X) {delete[] (X); (X) = NULL;}
#define SAFE_RELEASE(X)		if(X) {(X)->Release(); (X) = NULL;}
#define ASSET(X)			if(!X){Log(#X);}

#define GetTickCount		timeGetTime
#define FD_ADD(S, SET)		((fd_set FAR *)(SET))->fd_array[((fd_set FAR *)(SET))->fd_count++] = (S)
#define FD_COUNT(SET)		(((fd_set FAR *)(SET))->fd_count)
#define ARR_COUNT(X)		(sizeof(X) / sizeof((X)[0]))
#define WriteStream(X,Y,S)	{memcpy(X, Y, S); X += S;}
#define ReadStream(X,Y,S)	{memcpy(Y, X, S); X += S;}

#define MAXSQL_LENGTH 4096
#define MAXBLOB_LENGTH 256


typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned __int64	uint64;
typedef __int64				int64;
typedef UINT				PlayerID;	//玩家的ID类型


#if _MSC_VER < 1600	// VS2010
#include <hash_map>
template<typename K, typename T>
class HashMap: public hash_map<K, T>
{
};
#else
#include <unordered_map>
template<typename K, typename T>
class HashMap: public unordered_map<K, T>
{
};
#endif
 





//#ifdef _DEBUG
//#define ASSERT assert
//#else
static void LogErrorToFile(bool IsError, const char* FileName, const char* FuntionName, int Len)
{
	if (IsError)
		return;
	//写入到文件里面去 Log.txt
	FILE* pFile = NULL;
	errno_t Error = fopen_s(&pFile, "Log.txt", "a");
	if (!pFile)
	{
		Error = fopen_s(&pFile, "Log.txt", "w+");
	}
	if (pFile)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		fprintf_s(pFile, "断言[%d %d %d %d:%d:%d]: %s %s %d\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, FileName, FuntionName, Len);
		fclose(pFile);
	}
};
#define ASSERT(_Expression) LogErrorToFile(_Expression,__FILE__,__FUNCTION__,__LINE__)
//#endif



#define IN_MIN (-2147483647-1)
//转化函数 Byte Bool int DWORD WORD short int64 float double 
//从大到小
inline int ConvertInt64ToInt(int64 Value)
{
	if (Value > 2147483647)
	{
		ASSERT(false);
		return 2147483647;
	}
	else if (Value < IN_MIN)
	{
		ASSERT(false);
		return IN_MIN;
	}
	else
	{
		return static_cast<int>(Value);
	}
}
inline DWORD ConvertInt64ToDWORD(int64 Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xffffffff)
	{
		ASSERT(false);
		return 0xffffffff;
	}
	else
	{
		return static_cast<DWORD>(Value);
	}
}
inline short ConvertInt64ToShort(int64 Value)
{
	//-32768  32767
	if (Value < -32768)
	{
		ASSERT(false);
		return -32768;
	}
	else if (Value > 32767)
	{
		ASSERT(false);
		return 32767;
	}
	else
	{
		return static_cast<short>(Value);
	}
}
inline WORD ConvertInt64ToWORD(int64 Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xffff)
	{
		ASSERT(false);
		return 0xffff;
	}
	else
	{
		return static_cast<WORD>(Value);
	}
}
inline BYTE ConvertInt64ToBYTE(int64 Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xff)
	{
		ASSERT(false);
		return 0xff;
	}
	else
	{
		return static_cast<BYTE>(Value);
	}
}
inline char ConvertInt64ToChar(int64 Value)
{
	if (Value < -128)
	{
		ASSERT(false);
		return -128;
	}
	else if (Value > 127)
	{
		ASSERT(false);
		return 127;
	}
	else
	{
		return static_cast<char>(Value);
	}
}
//DWORD 往下转
inline int ConvertDWORDToInt(DWORD Value)
{
	if (Value > 2147483647)
	{
		ASSERT(false);
		return 2147483647;
	}
	else
	{
		return static_cast<int>(Value);
	}
}
inline short ConvertDWORDToShort(DWORD Value)
{
	//-32768  32767
	if (Value > 32767)
	{
		ASSERT(false);
		return 32767;
	}
	else
	{
		return static_cast<short>(Value);
	}
}
inline WORD ConvertDWORDToWORD(DWORD Value)
{
	if (Value > 0xffff)
	{
		ASSERT(false);
		return 0xffff;
	}
	else
	{
		return static_cast<WORD>(Value);
	}
}
inline BYTE ConvertDWORDToBYTE(DWORD Value)
{
	if (Value > 0xff)
	{
		ASSERT(false);
		return 0xff;
	}
	else
	{
		return static_cast<BYTE>(Value);
	}
}
inline char ConvertDWORDToChar(DWORD Value)
{
	if (Value > 127)
	{
		ASSERT(false);
		return 127;
	}
	else
	{
		return static_cast<char>(Value);
	}
}
//int
inline DWORD ConvertIntToDWORD(int Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else
	{
		return static_cast<DWORD>(Value);
	}
}
inline short ConvertIntToShort(int Value)
{
	//-32768  32767
	if (Value < -32768)
	{
		ASSERT(false);
		return -32768;
	}
	else if (Value > 32767)
	{
		ASSERT(false);
		return 32767;
	}
	else
	{
		return static_cast<short>(Value);
	}
}
inline WORD ConvertIntToWORD(int Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xffff)
	{
		ASSERT(false);
		return 0xffff;
	}
	else
	{
		return static_cast<WORD>(Value);
	}
}
inline BYTE ConvertIntToBYTE(int Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xff)
	{
		ASSERT(false);
		return 0xff;
	}
	else
	{
		return static_cast<BYTE>(Value);
	}
}
inline char ConvertIntToChar(int Value)
{
	if (Value < -128)
	{
		ASSERT(false);
		return -128;
	}
	else if (Value > 127)
	{
		ASSERT(false);
		return 127;
	}
	else
	{
		return static_cast<char>(Value);
	}
}
//WORD
inline short ConvertWORDToShort(WORD Value)
{
	//-32768  32767
	if (Value > 32767)
	{
		ASSERT(false);
		return 32767;
	}
	else
	{
		return static_cast<short>(Value);
	}
}
inline BYTE ConvertWORDToBYTE(WORD Value)
{
	if (Value > 0xff)
	{
		ASSERT(false);
		return 0xff;
	}
	else
	{
		return static_cast<BYTE>(Value);
	}
}
inline char ConvertWORDToChar(WORD Value)
{
	if (Value > 127)
	{
		ASSERT(false);
		return 127;
	}
	else
	{
		return static_cast<char>(Value);
	}
}
//short
inline WORD ConvertShortToWORD(short Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else
	{
		return static_cast<WORD>(Value);
	}
}
inline BYTE ConvertShortToBYTE(short Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xff)
	{
		ASSERT(false);
		return 0xff;
	}
	else
	{
		return static_cast<BYTE>(Value);
	}
}
inline char ConvertShortToChar(short Value)
{
	if (Value < -128)
	{
		ASSERT(false);
		return -128;
	}
	else if (Value > 127)
	{
		ASSERT(false);
		return 127;
	}
	else
	{
		return static_cast<char>(Value);
	}
}
//BYTE 
inline char ConvertBYTEToChar(BYTE Value)
{
	if (Value > 127)
	{
		ASSERT(false);
		return 127;
	}
	else
	{
		return static_cast<char>(Value);
	}
}
//char 
inline BYTE ConvertCharToBYTE(char Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else
	{
		return static_cast<BYTE>(Value);
	}
}
//浮点数
inline int ConvertDoubleToInt(double Value)
{
	if (Value > 2147483647)
	{
		ASSERT(false);
		return 2147483647;
	}
	else if (Value < IN_MIN)
	{
		ASSERT(false);
		return IN_MIN;
	}
	else
	{
		return static_cast<int>(Value);
	}
}
inline DWORD ConvertDoubleToDWORD(double Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xffffffff)
	{
		ASSERT(false);
		return 0xffffffff;
	}
	else
	{
		return static_cast<DWORD>(Value);
	}
}
inline short ConvertDoubleToShort(double Value)
{
	//-32768  32767
	if (Value < -32768)
	{
		ASSERT(false);
		return -32768;
	}
	else if (Value > 32767)
	{
		ASSERT(false);
		return 32767;
	}
	else
	{
		return static_cast<short>(Value);
	}
}
inline WORD ConvertDoubleToWORD(double Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xffff)
	{
		ASSERT(false);
		return 0xffff;
	}
	else
	{
		return static_cast<WORD>(Value);
	}
}
inline BYTE ConvertDoubleToBYTE(double Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xff)
	{
		ASSERT(false);
		return 0xff;
	}
	else
	{
		return static_cast<BYTE>(Value);
	}
}
inline char ConvertDoubleToChar(double Value)
{
	if (Value < -128)
	{
		ASSERT(false);
		return -128;
	}
	else if (Value > 127)
	{
		ASSERT(false);
		return 127;
	}
	else
	{
		return static_cast<char>(Value);
	}
}
//float
inline int ConvertFloatToInt(float Value)
{
	if (Value > 2147483647)
	{
		ASSERT(false);
		return 2147483647;
	}
	else if (Value < IN_MIN)
	{
		ASSERT(false);
		return IN_MIN;
	}
	else
	{
		return static_cast<int>(Value);
	}
}
inline DWORD ConvertFloatToDWORD(float Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xffffffff)
	{
		ASSERT(false);
		return 0xffffffff;
	}
	else
	{
		return static_cast<DWORD>(Value);
	}
}
inline short ConvertFloatToShort(float Value)
{
	//-32768  32767
	if (Value < -32768)
	{
		ASSERT(false);
		return -32768;
	}
	else if (Value > 32767)
	{
		ASSERT(false);
		return 32767;
	}
	else
	{
		return static_cast<short>(Value);
	}
}
inline WORD ConvertFloatToWORD(float Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xffff)
	{
		ASSERT(false);
		return 0xffff;
	}
	else
	{
		return static_cast<WORD>(Value);
	}
}
inline BYTE ConvertFloatToBYTE(float Value)
{
	if (Value < 0)
	{
		ASSERT(false);
		return 0;
	}
	else if (Value > 0xff)
	{
		ASSERT(false);
		return 0xff;
	}
	else
	{
		return static_cast<BYTE>(Value);
	}
}
inline char ConvertFloatToChar(float Value)
{
	if (Value < -128)
	{
		ASSERT(false);
		return -128;
	}
	else if (Value > 127)
	{
		ASSERT(false);
		return 127;
	}
	else
	{
		return static_cast<char>(Value);
	}
}



inline SYSTEMTIME TimeTToSystemTime(time_t t)
{
	tm pTime;
	errno_t Error = localtime_s(&pTime, &t);
	if (Error == 0)
	{
		SYSTEMTIME pst;
		pst.wYear = static_cast<WORD>(pTime.tm_year + 1900);
		pst.wMonth = static_cast<WORD>(pTime.tm_mon + 1);
		pst.wDay = static_cast<WORD>(pTime.tm_mday);
		pst.wHour = static_cast<WORD>(pTime.tm_hour);
		pst.wMinute = static_cast<WORD>(pTime.tm_min);
		pst.wSecond = static_cast<WORD>(pTime.tm_sec);
		pst.wMilliseconds = 0;
		pst.wDayOfWeek = static_cast<WORD>(pTime.tm_wday);
		return pst;
	}
	ASSERT(false);
	SYSTEMTIME pst;
	GetLocalTime(&pst);
	return pst;
}
inline time_t SystemTimeToTimeT(SYSTEMTIME st)
{
	time_t pt = time(NULL);
	tm pTime;
	errno_t Error = localtime_s(&pTime, &pt);
	if (Error == 0)
	{
		pTime.tm_year = st.wYear - 1900;
		pTime.tm_mon = st.wMonth - 1;
		pTime.tm_mday = st.wDay;
		pTime.tm_hour = st.wHour;
		pTime.tm_min = st.wMinute;
		pTime.tm_sec = st.wSecond;
		pTime.tm_wday = st.wDayOfWeek;
		pt = mktime(&pTime);
	}
	return pt;
}
inline DWORD GetDiffDay(time_t TimeLog, time_t DestTime, DWORD WriteSec)
{
	DestTime -= WriteSec;
	if (DestTime < 0)
		DestTime = 0;
	tm pDestTime;
	errno_t Error = localtime_s(&pDestTime, &DestTime);
	if (Error != 0)
	{
		ASSERT(false);
		return 0;
	}
	TimeLog -= WriteSec;
	if (TimeLog < 0)
		TimeLog = 0;
	tm pTimeLog;
	Error = localtime_s(&pTimeLog, &TimeLog);
	if (Error != 0)
	{
		ASSERT(false);
		return 0;
	}
	if (DestTime >= TimeLog)
	{
		DWORD TimeLogDaySec = static_cast<DWORD>(pTimeLog.tm_hour * 3600 + pTimeLog.tm_min * 60 + pTimeLog.tm_sec);
		DWORD DestDaySec = static_cast<DWORD>(pDestTime.tm_hour * 3600 + pDestTime.tm_min * 60 + pDestTime.tm_sec);
		DWORD diffSec = static_cast<DWORD>(DestTime - TimeLog);
		if (DestDaySec < TimeLogDaySec)
		{
			//结果 +1 
			return ((diffSec % 86400) == 0 ? (diffSec / 86400) : (diffSec / 86400 + 1));
		}
		else
		{
			//直接返回结果
			return diffSec / 86400;
		}
	}
	else
	{
		DWORD TimeLogDaySec = static_cast<DWORD>(pTimeLog.tm_hour * 3600 + pTimeLog.tm_min * 60 + pTimeLog.tm_sec);
		DWORD DestDaySec = static_cast<DWORD>(pDestTime.tm_hour * 3600 + pDestTime.tm_min * 60 + pDestTime.tm_sec);
		DWORD diffSec = static_cast<DWORD>(TimeLog - DestTime);
		if (DestDaySec < TimeLogDaySec)
		{
			//结果 +1 
			return ((diffSec % 86400) == 0 ? (diffSec / 86400) : (diffSec / 86400 + 1));
		}
		else
		{
			//直接返回结果
			return diffSec / 86400;
		}
	}
}
inline DWORD GetDiffDay(time_t TimeLog, DWORD WriteSec)
{
	time_t Now = time(NULL);
	Now -= WriteSec;
	if (Now < 0)
		Now = 0;
	tm pNow;
	errno_t Error = localtime_s(&pNow, &Now);
	if (Error != 0)
	{
		ASSERT(false);
		return 0;
	}
	TimeLog -= WriteSec;
	if (TimeLog < 0)
		TimeLog = 0;
	tm pTimeLog;
	Error = localtime_s(&pTimeLog, &TimeLog);
	if (Error != 0)
	{
		ASSERT(false);
		return 0;
	}
	//判断2个事件差距的天数 不是 秒数
	//差距的天数 
	//可以得到2个时间  差距的秒数 如何求出 差距的天数
	//根据 当天内的数据  Now 8 TimeLog 10
	if (Now >= TimeLog)
	{
		DWORD TimeLogDaySec = static_cast<DWORD>(pTimeLog.tm_hour * 3600 + pTimeLog.tm_min * 60 + pTimeLog.tm_sec);
		DWORD NowDaySec = static_cast<DWORD>(pNow.tm_hour * 3600 + pNow.tm_min * 60 + pNow.tm_sec);
		DWORD diffSec = static_cast<DWORD>(Now - TimeLog);
		if (NowDaySec < TimeLogDaySec)
		{
			//结果 +1 
			return ((diffSec % 86400) == 0 ? (diffSec / 86400) : (diffSec / 86400 + 1));
		}
		else
		{
			//直接返回结果
			return diffSec / 86400;
		}
	}
	else
	{
		DWORD TimeLogDaySec = static_cast<DWORD>(pTimeLog.tm_hour * 3600 + pTimeLog.tm_min * 60 + pTimeLog.tm_sec);
		DWORD NowDaySec = static_cast<DWORD>(pNow.tm_hour * 3600 + pNow.tm_min * 60 + pNow.tm_sec);
		DWORD diffSec = static_cast<DWORD>(TimeLog - Now);
		if (TimeLogDaySec < NowDaySec)
		{
			//结果 +1 
			return ((diffSec % 86400) == 0 ? (diffSec / 86400) : (diffSec / 86400 + 1));
		}
		else
		{
			//直接返回结果
			return diffSec / 86400;
		}
	}
}
inline char* UnicodeToutf8(const char* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, NULL, 0, NULL, NULL);
	char* szUTF8 = (char*)malloc(static_cast<size_t>(len + 1));
	if (!szUTF8)
		return NULL;
	memset(szUTF8, 0, static_cast<size_t>(len + 1));
	WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, szUTF8, len, NULL, NULL);
	return szUTF8;
}
inline bool CheckChangeDWORDValue(DWORD nSrcValue, int ChangeValue)
{
	//修改DWORD的值 判断是否可以进行修改 取值范围判断
	//0-0xffffffff
	if (ChangeValue == 0)
		return true;
	else if (ChangeValue > 0)
	{
		__int64 DestValue = nSrcValue + ChangeValue;
		if (DestValue > 0xffffffff)
			return false;
		return true;
	}
	else
	{
		return static_cast<DWORD>(ChangeValue*-1) <= nSrcValue;
		/*ChangeValue = ChangeValue*-1;
		if (nSrcValue - ChangeValue  >= 0)
		return true;
		else
		return false;*/
	}
}

//对齐到N次方
template<typename T>
inline size_t AEAlignSize(T *p, size_t uAlignCount)
{
	return (UINT_PTR)p & (uAlignCount - 1);
}
//移除vector元素，最后一个填充。
template<typename T>
inline void ListRemoveAt(vector<T>&list, size_t idx)
{
	if (static_cast<DWORD>(idx + 1) != list.size())
		list[idx] = list.back();
	list.pop_back();
}
//外部负责free
inline WCHAR* CharToWChar(const char *pc, UINT &count)
{
	count = strlen(pc);
	UINT size = (count + 1) << 1;
	WCHAR *str = (WCHAR*)malloc(size);
	if (!str)
		return NULL;
	int nChar = MultiByteToWideChar(CP_ACP, 0, pc, static_cast<int>(count), str, static_cast<int>(size));
	str[nChar] = 0;
	count = static_cast<UINT>(nChar);
	return str;
}
inline WCHAR* CharToWChar(const char *pc,UINT pcLength, UINT &count)
{
	UINT size = (pcLength + 1) << 1;
	WCHAR *str = (WCHAR*)malloc(size);
	if (!str)
		return NULL;
	int nChar = MultiByteToWideChar(CP_ACP, 0, pc, static_cast<int>(pcLength), str, static_cast<int>(size));
	str[nChar] = 0;
	count = static_cast<UINT>(nChar);
	return str;
}
//外部负责free
inline char* WCharToChar(const wchar_t *pc, UINT &count)
{
	count = wcslen(pc);
	UINT size = (count + 1) << 1;
	char *str = (char*)malloc(size);
	if (!str)
		return NULL;
	int nChar = WideCharToMultiByte(CP_ACP, 0, pc, static_cast<int>(count), str, static_cast<int>(size), NULL, NULL);
	str[nChar] = 0;
	count = static_cast<UINT>(nChar);
	return str;
}
inline char* WCharToChar(const wchar_t *pc, UINT pcLength, UINT &count)
{
	UINT size = (pcLength + 1) << 1;
	char *str = (char*)malloc(size);
	if (!str)
		return NULL;
	int nChar = WideCharToMultiByte(CP_ACP, 0, pc, static_cast<int>(pcLength), str, static_cast<int>(size), NULL, NULL);
	str[nChar] = 0;
	count = static_cast<UINT>(nChar);
	return str;
}

inline unsigned char ToHex2(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}


inline unsigned char FromHex2(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	//else assert(0);  
	return y;
}

inline std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex2(static_cast<unsigned char>(str[i]) >> 4);
			strTemp += ToHex2(static_cast<unsigned char>(str[i]) % 16);
		}
	}
	return strTemp;
}

inline std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			//				assert(i + 2 < length);  
			unsigned char high = FromHex2((unsigned char)str[++i]);
			unsigned char low = FromHex2((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}



inline fd_set* CreateFDSet()
{
	return new fd_set;
}
inline void DeleteFDSet(fd_set *pset)
{
	delete pset;
}
//IPV4整形转换为字符串
inline void IPToString(UINT ip, WCHAR str[20])
{
	swprintf_s(str, 20, L"%d.%d.%d.%d", BYTE(ip), BYTE(ip >> 8), BYTE(ip >> 16), BYTE(ip >> 24));
}


//使用模板函数
#define  MsgBegin 1
#define  MsgEnd   2
#define  MaxMsgLength 60000  //内部命令 最大为 6W
#define  MaxMsgLength_Client 1000  //对外的命令 最大为 1000 

template<typename T>
inline void SqlitMsg(T* OldMsg, DWORD OldMsgSize, DWORD ArraySum, bool IsSendToClient, std::vector<T*>& pVec)
{
	//分包函数
	pVec.clear();

	DWORD MsgLength = 0;
	if (IsSendToClient)
		MsgLength = MaxMsgLength_Client;
	else
		MsgLength = MaxMsgLength;

	if (MaxMsgLength >= 0xffff)
	{
		ASSERT(false);
		return;
	}
	if (OldMsgSize <= MsgLength)
	{
		OldMsg->SetCmdSize(ConvertDWORDToWORD(OldMsgSize));
		OldMsg->Sum = ConvertDWORDToWORD(ArraySum);
		OldMsg->States = (MsgBegin | MsgEnd);
		//复制一次
		T* NewMsg = (T*)malloc(OldMsgSize);
		memcpy_s(NewMsg, OldMsgSize, OldMsg, OldMsgSize);
		pVec.push_back(NewMsg);
		return;
	}
	else
	{
		if (OldMsgSize <= (ArraySum * sizeof(OldMsg->Array[0])))
		{
			ASSERT(false);
			return;
		}
		DWORD HeadSize = (OldMsgSize - (ArraySum * sizeof(OldMsg->Array[0])));
		if (HeadSize >= MsgLength)
		{
			ASSERT(false);
			return;
		}
		DWORD CanUserLength = MsgLength - HeadSize;
		WORD  MsgMaxArraySum = ConvertDWORDToWORD(CanUserLength / sizeof(OldMsg->Array[0]));
		DWORD SqlitSum = ((ArraySum%MsgMaxArraySum == 0) ? (ArraySum / ArraySum) : (ArraySum / MsgMaxArraySum + 1));
		WORD PageSum = ConvertDWORDToWORD(HeadSize + MsgMaxArraySum * sizeof(OldMsg->Array[0]));
		for (DWORD i = 0; i < SqlitSum; ++i)
		{
			T* NewMsg = (T*)malloc(PageSum);
			memcpy_s(NewMsg, HeadSize, OldMsg, HeadSize);
			NewMsg->SetCmdSize(PageSum);
			NewMsg->SetCmdType(OldMsg->GetCmdType());
			NewMsg->Sum = MsgMaxArraySum;
			NewMsg->States = 0;
			if (i == 0)
			{
				NewMsg->States |= MsgBegin;
			}
			if (i == (SqlitSum - 1))
			{
				NewMsg->States |= MsgEnd;
				//因为是最后一个  长度需要重新定义下
				if (ArraySum%MsgMaxArraySum != 0)
				{
					PageSum = ConvertDWORDToWORD(HeadSize + (ArraySum%MsgMaxArraySum) * sizeof(OldMsg->Array[0]));
					NewMsg->SetCmdSize(PageSum);
					NewMsg->Sum = ArraySum%MsgMaxArraySum;
				}
			}
			for (WORD j = 0; j < NewMsg->Sum; ++j)
			{
				if (j >= ArraySum || i*MsgMaxArraySum + j >= ArraySum)
				{
					ASSERT(false);
					return;
				}
				NewMsg->Array[j] = OldMsg->Array[i*MsgMaxArraySum + j];
			}
			pVec.push_back(NewMsg);
		}
		return;
	}
}

struct ReplaceStrItem
{
	ReplaceStrItem(const char *pcOrg, const char *pcNew, UINT orgCount = -1, UINT newCount = -1)
	{
		_pcORG = pcOrg;
		_pcNew = pcNew;
		_cOrg = orgCount == -1 ? strlen(_pcORG) : orgCount;
		_cNew = newCount == -1 ? strlen(_pcNew) : newCount;
	}
	const char *_pcORG;
	const char *_pcNew;
	UINT		_cOrg;
	UINT		_cNew;
};
#define REPLACE_ERR -1
//如果新的BUFF空间不够大，返回REPLACE_ERR
inline int ReplaceStr(const char *pstr, UINT size, char *pNewBuff, UINT newBuffSize, const ReplaceStrItem *pItems, UINT itemCount)
{
	const char *pstr_end	= pstr + size;
	char *pstr_new			= pNewBuff;
	char *pstr_new_end		= pstr_new + newBuffSize;
	while (pstr < pstr_end)
	{
		UINT strcount = static_cast<UINT>(pstr_end - pstr);
		for (UINT i = 0; i < itemCount; ++i)
		{
			UINT cmpCount = min(strcount, pItems[i]._cOrg);
			if (memcmp((void*)pstr, pItems[i]._pcORG, static_cast<size_t>(cmpCount)) == 0)
			{
				if (UINT(pstr_new_end - pstr_new) < pItems[i]._cNew)
					return REPLACE_ERR;
				memcpy(pstr_new, pItems[i]._pcNew, pItems[i]._cNew);
				pstr_new += pItems[i]._cNew;
				pstr += pItems[i]._cOrg;
				goto WHILE_LOOP;
			}
		}
		if (pstr_new == pstr_new_end)
			return REPLACE_ERR;
		*pstr_new++ = *pstr++;
	WHILE_LOOP:
		;
	}
	*pstr_new = 0;
	return pstr_new - pNewBuff;
}
bool SplitStrByIpay(const char *pcstr, UINT size, HashMap<UINT, std::string> & strList);
//对齐到2的N次方
inline UINT AlignToPow2(UINT nSize)
{
	UINT k = nSize & (nSize - 1);
	if (k != 0)
	{
		k = 0;
		while (nSize)
		{
			nSize >>= 1;
			++k;
		}
		nSize = static_cast<UINT>(1 << k);
	}
	return nSize;
}
#define CACHELINE32(x)	char ve_cacheline_##x [32];
#define CACHELINE(x)	char ve_cacheline_##x [64];

struct CreateSocketData
{
	SOCKET			Socket;
	USHORT			Port;
	SOCKADDR_IN		Addr;
	int				AddrSize;
};
enum CreateSocketType
{
	CST_UDP = 0x1,
	CST_TCP = 0x2,
	CST_TCP_IOCP = 0x4,
	CST_BIND = 0x8
};
extern bool CreateSocket(UINT type, USHORT port, uint recvBuffSize, uint sendBuffSize, CreateSocketData &csd, UINT localIP = 0);
extern void InitSocket(SOCKET s, UINT sendBuffSize, UINT recvBuffSize, bool bTCP);
#include "Encoder.h"
#include "NetCmd.h"
#include "NetCmd_DBServer.h"
#include "Random.h"
#include "SafeList.h"
#include "MyArray.h"
#include "SafeArray.h"
#include "SafeArrayMT.h"
#include "Crc32.h"
#include "Timer.h"
#include "SqlUtinity.h"
#include "SqlIndexMap.h"
#include "SqlOperator.h"
#include "SqlIndex.h"
#include "SqlTable.h"
#include "SqlUpdater.h"
#include "SQL.h"
#include "TestSql.h"
#include "LogHandler.h"
#include "Dump.h"
#include "NetServer.h"
#include "ServerData.h"
#include "newServer.h"
#include "TCPPing.h"
#include "HttpServer.h"
#include "HttpClient.h"
#include "NetClient.h"
#include "TCPClient.h"
#include "TCPServer.h"
#include "FTPServer.h"
#include "FTPClient.h"
#include "TCPServerOld.h"
#include "UDPClient.h"
#include "UDPServer.h"
#include "Gateway.h"

#include "TestClient.h"
#include "Cmd_Fish.h"

inline DWORD GetCrc32(TCHAR* pStr)
{
	if (!pStr)
	{
		ASSERT(false);
		return 0;
	}
	size_t len = _tcslen(pStr);
	TCHAR* DestStr = (TCHAR*)malloc(sizeof(TCHAR)*(len + 1));
	for (size_t i = 0; i < len; ++i)
	{
		DestStr[i] = towlower(pStr[i]);
	}
	DestStr[len] = 0;
	DWORD Crc32 = AECrc32(DestStr, sizeof(TCHAR)* len, 0);
	free(DestStr);
	return Crc32;
}
inline DWORD GetCrc32(TCHAR* pStr, DWORD len)
{
	if (!pStr)
	{
		ASSERT(false);
		return 0;
	}
	TCHAR* DestStr = (TCHAR*)malloc(sizeof(TCHAR)*(len + 1));
	for (size_t i = 0; i < len; ++i)
	{
		DestStr[i] = towlower(pStr[i]);
	}
	DestStr[len] = 0;
	DWORD Crc32 = AECrc32(DestStr, sizeof(TCHAR)* len, 0);
	free(DestStr);
	return Crc32;
}


inline string GBKToUTF8(const std::string& strGBK)
{
	string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[static_cast<unsigned int>(n)];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[static_cast<unsigned int>(n)];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}


//公共定义的宏
//#ifdef CENTRALSERVER
//	#define CLIENT_CMD_SIZE 10240
//	#define MAX_SEND_CMD_COUNT 10240
//	#define USE_ACCEPT_RECV_DATA 1
//#endif
//
//#ifdef DBSERVER
//	#define CLIENT_CMD_SIZE 10240
//	#define MAX_SEND_CMD_COUNT 10240
//	#define USE_ACCEPT_RECV_DATA 1
//#endif
//
//#ifdef FTPSERVER
//	#define CLIENT_CMD_SIZE 10240
//	#define MAX_SEND_CMD_COUNT 10240
//	#define USE_ACCEPT_RECV_DATA 1
//#endif
//
//#ifdef LOGONSERVER
//	#define CLIENT_CMD_SIZE 32
//	#define MAX_SEND_CMD_COUNT 32
//	#define USE_ACCEPT_RECV_DATA 1
//#endif
//
//#ifdef OPERATESERVER
//	#define CLIENT_CMD_SIZE 10240
//	#define MAX_SEND_CMD_COUNT 10240
//	#define USE_ACCEPT_RECV_DATA 1
//#endif
//
//#ifdef GAMESERVER
//	#define CLIENT_CMD_SIZE 128
//	#define MAX_SEND_CMD_COUNT 128
//	#define USE_ACCEPT_RECV_DATA 1
//#endif
//
//#ifdef RANKSERVER
//	#define CLIENT_CMD_SIZE 1024
//	#define MAX_SEND_CMD_COUNT 1024
//#endif

#define TCHARCopy(DestTChar,DestTCharLength,SrcTChar,SrcTCharLength) DestTCharLength >= (SrcTCharLength + 1) ? _tcsncpy_s(DestTChar, DestTCharLength, SrcTChar, SrcTCharLength) : ASSERT(false)


static void LogInfoToFile(const char* FileName,const WCHAR *pcStr, ...)
{
	
	va_list var;
	va_start(var, pcStr);
	UINT nCount = _vscwprintf(pcStr, var);
	WCHAR *pBuffer = new WCHAR[nCount + 1];
	if (pBuffer)
	{
		vswprintf_s(pBuffer, nCount + 1, pcStr, var);
		
		FILE* pFile = NULL;
		errno_t Error = fopen_s(&pFile, FileName, "a");
		if (!pFile || Error!=0)
		{
			Error = fopen_s(&pFile, FileName, "w+");
		}
		if (!pFile || Error != 0)
		{
			return;
		}

		SYSTEMTIME pTime;
		GetLocalTime(&pTime);
		fwprintf_s(pFile, TEXT("%s %04d-%02d-%02d %02d:%02d:%02d\n"), pBuffer, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
		fclose(pFile);
		SAFE_DELETE_ARR(pBuffer);
	}
	va_end(var);
}
//"LogLogonInfo.txt"
// 2015-12-12 24:59:59
static void LogInfoToFile(const char* FileName, const char *pcStr, ...)
{
	va_list var;
	va_start(var, pcStr);
	UINT nCount = _vscprintf(pcStr, var);
	char *pBuffer = new char[nCount + 1];
	if (pBuffer)
	{
		vsprintf_s(pBuffer, nCount + 1, pcStr, var);
		FILE* pFile = NULL;
		errno_t Error = fopen_s(&pFile, FileName, "a");
		if (!pFile || Error != 0)
		{
			Error = fopen_s(&pFile, FileName, "w+");
		}
		if (!pFile || Error != 0)
		{
			return;
		}

		SYSTEMTIME pTime;
		GetLocalTime(&pTime);

		fprintf_s(pFile, "%s %04d-%02d-%02d %02d:%02d:%02d\n", pBuffer, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
		fclose(pFile);
		SAFE_DELETE_ARR(pBuffer);
	}
	va_end(var);
}