#pragma once

#define COLUMN(X) ((UINT64)1 << X)
#define UION(TABLE, COLUMNMASK, SELFCOLUMN, UNIONCOLUMN) SqlUnion(TABLE, COLUMNMASK, SELFCOLUMN, UNIONCOLUMN)
#define IS_WCHAR_TYPE(X) ((X) == SQL_VARCHAR)
typedef HashMap<UINT_PTR, BYTE> SqlUpdateHashMap;
typedef HashMap<UINT_PTR, BYTE> SqlIDList;
class SQL;

//表名集合
enum SqlTableType
{
	TABLE_NONE		= 0,
	TABLE_USERTABLE,
	TABLE_FRIEND,
	TABLE_EMAIL,
};
enum
{
	QUERY_MAX_COUNT		= 4,
	TABLE_INDEX_MAX		= 4,
	COLUMN_MAX_COUNT	= 64,
	COLUMN_NAME_LENGTH	= 256,
	QUERY_VALUE_COUNT	= 16,
	QUERY_VARCHAR_COUNT = QUERY_VALUE_COUNT >> 1,
};
//数据类型
enum SqlDataType
{
	SQL_SBYTE,
	SQL_BYTE,
	SQL_SHORT,
	SQL_USHORT,
	SQL_INT,
	SQL_UINT,
	SQL_FLOAT,
	SQL_INT64,
	SQL_UINT64,
	SQL_VARCHAR,
	SQL_DATETIME,
	SQL_BIT,
	SQL_BLOB,
};
//行数据
struct SqlRow
{
	UINT64	ColumnMask;	//列修改标记
};
//列数据
struct SqlColumn
{
	char	Name[COLUMN_NAME_LENGTH];
	BYTE	Index;
	BYTE	Type;
	USHORT	Offset;		//在行中的偏移
	USHORT  Length;		//此列的字节大小, VARCHAR和DATETIME是字符个数
};

extern bool InitColumnFromSqlData(void *sqlColumn, SqlColumn *column, BYTE index);
extern bool ConvertStrToFieldData(SqlDataType dt, USHORT length, BYTE *pCurData, const char *pcstr);
extern const char * GetColumnStr(const SqlColumn &f, char *tempStr, UINT size);
extern const WCHAR *GetSqlFieldValueStr(const SqlColumn &f, WCHAR *tempStr, uint size, const BYTE *pRow);
extern const char * GetSqlFieldValueStr(const SqlColumn &f, char *tempStr, uint size, const BYTE *pRow);

inline UINT64 ToDateTime(USHORT year, BYTE month, BYTE day, BYTE hour, BYTE minute, BYTE second)
{
	UINT64 dt = 0;
	BYTE *pd = (BYTE*)&dt;
	*pd++	= second;
	*pd++	= minute;
	*pd++	= hour;
	*pd++	= day;
	*pd++	= month;
	*(USHORT*)pd = year;
	return dt;
}
inline UINT64 FromDateTime(UINT64 dt, USHORT &year, BYTE &month, BYTE &day, BYTE &hour, BYTE &minute, BYTE &second)
{
	BYTE *pd = (BYTE*)&dt;
	second	= *pd++;
	minute	= *pd++;
	hour	= *pd++;
	day		= *pd++;
	month	= *pd++;
	year	= *(ushort*)pd;
	return dt;
}
inline UINT64 StrToDateTime(const WCHAR *pcStr)
{
	//2013-12-21 12:21:12
	WCHAR flag[] = { L'-', L'-', L' ', L':', L':', L'\0' };
	const WCHAR *pcFindB, *pcFindA = (const WCHAR*)pcStr;
	UINT64 date = 0;
	BYTE *pdt = ((BYTE*)&date) + 5;
	for (UINT i = 0; i < 6; ++i)
	{
		pcFindB = wcschr(pcFindA, flag[i]);
		if (pcFindB == NULL)
			return date;
		*(USHORT*)pdt |= (ushort)_wtoi(pcFindA);
		--pdt;
		pcFindA = pcFindB + 1;
	}
	return date;
}
inline UINT64 StrToDateTime(const CHAR *pcStr)
{
	//2013-12-21 12:21:12
	char flag[] = { '-', '-', ' ', ':', ':', '\0' };
	const CHAR *pcFindB, *pcFindA = (const CHAR*)pcStr;
	UINT64 date = 0;
	BYTE *pdt = ((BYTE*)&date) + 5;
	for (UINT i = 0; i < 6; ++i)
	{
		pcFindB = strchr(pcFindA, flag[i]);
		if (pcFindB == NULL)
			return date;
		*(USHORT*)pdt |= (ushort)atoi(pcFindA);
		--pdt;
		pcFindA = pcFindB + 1;
	}
	return date;
}
inline const WCHAR* DateTimeToStr(UINT64 datetime, WCHAR *pcStr)
{
	ushort year;
	BYTE m, d, h, mm, s;
	FromDateTime(datetime, year, m, d, h, mm, s);
	swprintf_s(pcStr, 20, L"%d-%d-%d %d:%d:%d",	year, m, d, h, mm, s);
	return pcStr;
}
inline const CHAR* DateTimeToStr(UINT64 datetime, CHAR *pcStr)
{
	ushort year;
	BYTE m, d, h, mm, s;
	FromDateTime(datetime, year, m, d, h, mm, s);
	sprintf_s(pcStr, 20, "%d-%d-%d %d:%d:%d", year, m, d, h, mm, s);
	return pcStr;
}
inline const char *GetTableName(BYTE table)
{
	switch (table)
	{
	case TABLE_USERTABLE:
		return "usertable";
	case TABLE_FRIEND:
		return "friendtable";
	default:
		Log("未知的表类型:%d", table);
		return "";
	}
}
inline BYTE GetColumnCount(UINT64 ColumnMask)
{
	UINT64 mask = 1;
	BYTE c = 0;
	for (UINT i = 0; i < COLUMN_MAX_COUNT; ++i)
	{
		if ((mask << i) & ColumnMask)
			++c;
	}
	return c;
}
inline BYTE GetColumnCountAndRowSize(UINT64 ColumnMask, UINT &size, SqlColumn *pc)
{
	UINT64 mask = 1;
	BYTE c = 0;
	size = 0;
	for (UINT i = 0; i < COLUMN_MAX_COUNT; ++i)
	{
		if ((mask << i) & ColumnMask)
		{
			if (IS_WCHAR_TYPE(pc[i].Type))
				size += (pc[i].Length + 1) << 1;
			else
				size += pc[i].Length;
			++c;
		}
	}
	return c;
}