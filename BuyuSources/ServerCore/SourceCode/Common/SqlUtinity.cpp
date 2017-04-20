#include "stdafx.h"
#include "mysql.h"
bool InitColumnFromSqlData(void *sqlcol, SqlColumn *column, BYTE index)
{
	column->Index = index;
	MYSQL_FIELD *sqlColumn = (MYSQL_FIELD*)sqlcol;
	strcpy_s(column->Name, sizeof(column->Name), sqlColumn->name);
	bool bunsigned = (sqlColumn->flags & 0x20) != 0;
	switch (sqlColumn->type)
	{
	case MYSQL_TYPE_TINY:
		column->Type = static_cast<BYTE>(bunsigned ? SQL_BYTE : SQL_SBYTE);
		column->Length = sizeof(BYTE);
		break;
	case MYSQL_TYPE_SHORT:
		column->Type = static_cast<BYTE>(bunsigned ? SQL_USHORT : SQL_SHORT);
		column->Length = sizeof(USHORT);
		break;
	case MYSQL_TYPE_LONG:
		column->Type = static_cast<BYTE>(bunsigned ? SQL_UINT : SQL_INT);
		column->Length = sizeof(INT);
		break;
	case MYSQL_TYPE_LONGLONG:
		column->Type = static_cast<BYTE>(bunsigned ? SQL_UINT64 : SQL_INT64);
		column->Length = sizeof(int64);
		break;
	case MYSQL_TYPE_DECIMAL:
	case MYSQL_TYPE_NEWDECIMAL:
	case MYSQL_TYPE_FLOAT:
		column->Type = static_cast<BYTE>(SQL_FLOAT);
		column->Length = sizeof(float);
		break;
	case MYSQL_TYPE_DATETIME:
		column->Type = static_cast<BYTE>(SQL_DATETIME);
		column->Length = sizeof(int64);
		break;
	case MYSQL_TYPE_VAR_STRING:
		column->Type = static_cast<BYTE>(SQL_VARCHAR);
		column->Length = (USHORT)(sqlColumn->length >> 1);
		break;
	case MYSQL_TYPE_BIT:
		column->Type = static_cast<BYTE>(SQL_BIT);
		column->Length = sizeof(bool);
		break;
	case MYSQL_TYPE_BLOB:
		column->Type = static_cast<BYTE>(SQL_BLOB);
		column->Length = MAXBLOB_LENGTH;// (USHORT)sqlColumn->max_length;
		break;
	default:
		Log("Î´ÖªµÄ×Ö¶ÎÀàÐÍ%s:%d", sqlColumn->name, sqlColumn->type);
		return false;
	}
	return true;
}

bool ConvertStrToFieldData(SqlDataType dt, USHORT length, BYTE *pCurData, const char *pcstr)
{
	switch (dt)
	{
	case SQL_BYTE:
		if (pcstr)
			*((BYTE*)pCurData) = (BYTE)strtoul(pcstr, NULL, 10);
		else
			*((BYTE*)pCurData) = 0;
		break;
	case SQL_SBYTE:
		if (pcstr)
			*((char*)pCurData) = (char)strtol(pcstr, NULL, 10);
		else
			*((char*)pCurData) = 0;
		break;
	case SQL_SHORT:
		if (pcstr)
			*((SHORT*)pCurData) = (SHORT)strtol(pcstr, NULL, 10);
		else
			*((SHORT*)pCurData) = 0;
		break;
	case SQL_USHORT:
		if (pcstr)
			*((USHORT*)pCurData) = (USHORT)strtoul(pcstr, NULL, 10);
		else
			*((USHORT*)pCurData) = 0;
		break;
	case SQL_INT:
		if (pcstr)
			*((INT*)pCurData) = (INT)strtol(pcstr, NULL, 10);
		else
			*((INT*)pCurData) = 0;
		break;
	case SQL_UINT:
		if (pcstr)
			*((UINT*)pCurData) = (UINT)strtoul(pcstr, NULL, 10);
		else
			*((UINT*)pCurData) = 0;
		break;
	case SQL_FLOAT:
		if (pcstr)
			*((float*)pCurData) = (float)strtof(pcstr, NULL);
		else
			*((float*)pCurData) = 0.0f;
		break;
	case SQL_INT64:
		if (pcstr)
			*((int64*)pCurData) = (int64)strtoll(pcstr, NULL, 10);
		else
			*((int64*)pCurData) = 0;
		break;
	case SQL_UINT64:
	
		if (pcstr)
			*((uint64*)pCurData) = (uint64)strtoull(pcstr, NULL, 10);
		else
			*((uint64*)pCurData) = 0;
		break;
	case SQL_DATETIME:
		if (pcstr)
		{
			//DateTime ->time_t
			tm pTime;
			sscanf_s(pcstr, "%4d-%2d-%2d %2d:%2d:%2d", &pTime.tm_year, &pTime.tm_mon, &pTime.tm_mday, &pTime.tm_hour, &pTime.tm_min, &pTime.tm_sec);
			pTime.tm_year -= 1900;
			pTime.tm_mon -= 1;
			time_t time = mktime(&pTime);
			if (time < 0)
				time = 0;
			*((int64*)pCurData) = (int64)time;
		}
		else
			*((int64*)pCurData) = 0;
		break;
	case SQL_VARCHAR:
		if (pcstr)
		{
			UINT count;
			WCHAR *pWchar = CharToWChar(pcstr, count);
			if (count > 19 && dt == SQL_DATETIME)
			{
				Log("shit");
			}
			WCHAR *pCurStr = (WCHAR*)pCurData;
			if (pWchar == null)
			{
				Log(L"×ª»»×Ö·û´®Ê§°Ü.");
				return false;
			}

			memcpy_s(pCurStr, length << 1, pWchar, min(length, count) * sizeof(WCHAR));
			pCurStr[count] = 0;
			free(pWchar);
		}
		else
		{
			*(WCHAR*)pCurData = 0;
		}
		break;
	case SQL_BIT:
		{
			if (pcstr)
				*((bool*)pCurData) = ((pcstr[0] == 1) ? true : false);
			else
				*((bool*)pCurData) = false;
		}
		break;
	case SQL_BLOB:
		if (pcstr)
		{
			memcpy_s(pCurData, length, pcstr, length);
		}
		else
		{
			*(BYTE*)pCurData = 0;
		}
		break;
	default:
		return false;
	}
	return true;
}

const char * GetColumnStr(const SqlColumn &f, char *tempStr, UINT size)
{
	switch (f.Type)
	{
	case SQL_SBYTE:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " tinyint ");
		break;
	case SQL_BYTE:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " tinyint unsigned ");
		break;
	case SQL_SHORT:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " smallint ");
		break;
	case SQL_USHORT:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " smallint unsigned ");
		break;
	case SQL_INT:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " int ");
		break;
	case SQL_UINT:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " int unsigned ");
		break;
	case SQL_INT64:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " bigint ");
		break;
	case SQL_UINT64:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " bigint unsigned ");
		break;
	case SQL_FLOAT:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " decimal(10, 2) ");
		break;
	case SQL_VARCHAR:
		sprintf_s(tempStr, size, " %s varchar(%d) ", f.Name, f.Length);
		break;
	case SQL_DATETIME:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " bigint ");
		break;
	case SQL_BIT:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " bit");
		break;
	case SQL_BLOB:
		strcpy_s(tempStr, size, f.Name);
		strcat_s(tempStr, size, " Blob");
		break;
	default:
		return tempStr;
	}
	return tempStr;
}
const WCHAR *GetSqlFieldValueStr(const SqlColumn &f, WCHAR *tempStr, uint size, const BYTE *pRow)
{
	switch (f.Type)
	{
	case SQL_SBYTE:
		_itow_s(*(char*)pRow, tempStr, size, 10);
		break;
	case SQL_BYTE:
		_ultow_s(*(BYTE*)pRow, tempStr, size, 10);
		break;
	case SQL_SHORT:
		_itow_s(*(SHORT*)pRow, tempStr, size, 10);
		break;
	case SQL_USHORT:
		_ultow_s(*(USHORT*)pRow, tempStr, size, 10);
		break;
	case SQL_INT:
		_itow_s(*(INT*)pRow, tempStr, size, 10);
		break;
	case SQL_UINT:
		_ultow_s(*(UINT*)pRow, tempStr, size, 10);
		break;
	case SQL_INT64:
		_i64tow_s(*(INT64*)pRow, tempStr, size, 10);
		break;
	case SQL_UINT64:
		_ui64tow_s(*(UINT64*)pRow, tempStr, size, 10);
		break;
	case SQL_FLOAT:
		swprintf_s(tempStr, size, L"%f", *(float*)pRow);
		break;
	case SQL_VARCHAR:
		wcscpy_s(tempStr, size, (WCHAR*)pRow);
		break;
	case SQL_DATETIME:
		_i64tow_s(*(INT64*)pRow, tempStr, size, 10);
		break;
	case SQL_BIT:
		_ultow_s(*(bool*)pRow, tempStr, size, 10);
		break;
	case SQL_BLOB:
		memcpy_s(tempStr, size, pRow, size);
		break;
	default:
		tempStr[0] = 0;
		return tempStr;
	}
	return tempStr;
}
const char * GetSqlFieldValueStr(const SqlColumn &f, char *tempStr, uint size, const BYTE *pRow)
{
	switch (f.Type)
	{
	case SQL_SBYTE:
		_itoa_s(*(char*)pRow, tempStr, size, 10);
		break;
	case SQL_BYTE:
		_ultoa_s(*(BYTE*)pRow, tempStr, size, 10);
		break;
	case SQL_SHORT:
		_itoa_s(*(SHORT*)pRow, tempStr, size, 10);
		break;
	case SQL_USHORT:
		_ultoa_s(*(USHORT*)pRow, tempStr, size, 10);
		break;
	case SQL_INT:
		_itoa_s(*(INT*)pRow, tempStr, size, 10);
		break;
	case SQL_UINT:
		_ultoa_s(*(UINT*)pRow, tempStr, size, 10);
		break;
	case SQL_INT64:
		_i64toa_s(*(INT64*)pRow, tempStr, size, 10);
		break;
	case SQL_UINT64:
		_ui64toa_s(*(UINT64*)pRow, tempStr, size, 10);
		break;
	case SQL_FLOAT:
		sprintf_s(tempStr, size, "%f", *(float*)pRow);
		break;
	case SQL_DATETIME:
		_i64toa_s(*(INT64*)pRow, tempStr, size, 10);
		break;
	case SQL_VARCHAR:
	{
		UINT count;
		char *pStr = WCharToChar((WCHAR*)pRow, count);
		memcpy_s(tempStr, size, pStr, count);
		tempStr[count] = 0;
		free(pStr);
		break;
	}
	case SQL_BIT:
		{
			_ultoa_s(*(bool*)pRow, tempStr, size, 10);
			break;
		}
		break;
	case SQL_BLOB:
		{
			memcpy_s(tempStr, size, pRow, size);
			break;
		}
	default:
		tempStr[0] = 0;
		return tempStr;
	}
	return tempStr;
}
