#include "stdafx.h"
#define TEMP_STR_LENGTH 1000
#define STR_MAX_LENGTH 1000000
#define STR_CHECK_SIZE STR_MAX_LENGTH - 2000
static char g_buff[STR_MAX_LENGTH];

#define stream_init g_buff[0] = 0; int str_size = 0;char str_temp[256];(str_temp);
#define stream(X) {int n = strlen(X); if(n + str_size > STR_MAX_LENGTH) Log("´óÐ¡³¬³ö·¶Î§:%d", str_size); memcpy_s(g_buff + str_size, sizeof(g_buff), X, n); str_size += n; g_buff[str_size] = 0;}
#define stream_size str_size
#define stream_str g_buff
#define stream_clear g_buff[0] = 0; str_size = 0;
#define stream_int(X) sprintf_s(str_temp, sizeof(str_temp), "%d", X); stream(str_temp);
#define stream_return \
	SqlUpdateStr *str_update = (SqlUpdateStr*)malloc(stream_size + sizeof(SqlUpdateStr)); \
	if (str_update == null) return null; \
	memcpy_s(str_update->Str, stream_size, stream_str, stream_size); \
	str_update->Str[stream_size] = 0; \
	str_update->Size = stream_size;   \
	return str_update;
bool SqlUpdater::CreateUpdateStr(SqlTable &table, vector<SqlUpdateStr*>& strList)
{
	SqlUpdateStr *pcStr = NULL;
	Timer t;
	t.Begin();
	if (table.IsCreated())
	{
		pcStr = CreateNewTableStr(table);
		if (pcStr == NULL)
			return false;
		strList.push_back(pcStr);
	}
	//Log("create time:%f", t.Intecept());
	if (table.GetInsertRowCount())
	{
		pcStr = CreateInsertStr(table);
		if (pcStr == NULL)
			return false;
		strList.push_back(pcStr);
	}
	//Log("insert time:%f", t.Intecept());
	if (table.GetUpdateRowCount())
	{
		pcStr = CreateUpdateStr(table);
		if (pcStr == NULL)
			return false;
		strList.push_back(pcStr);
	}
	//Log("update time:%f", t.Intecept());
	if (table.GetDeleteRowCount())
	{
		pcStr = CreateDeleteStr(table);
		if (pcStr == NULL)
			return false;
		strList.push_back(pcStr);
	}
	//Log("delete time:%f", t.Intecept());
	return true;
}


SqlUpdateStr* SqlUpdater::CreateNewTableStr(SqlTable &table)
{
	char temp[TEMP_STR_LENGTH];
	stream_init;
	stream("drop table if exists ");
	stream(table.GetTableName());
	stream("; create table if not exists ");
	stream(table.GetTableName());
	stream(" (");
	for (BYTE i = 0; i < table.Columns(); ++i)
	{
		const SqlColumn &f = table.GetColumn(i);
		if (i == 0)
		{
			stream(GetColumnStr(f, temp, (UINT)sizeof(temp)));
			stream(" unique not null primary key auto_increment ");
		}
		else
		{
			stream(",");
			stream(GetColumnStr(f, temp, sizeof(temp)));
		}
	}
	stream(");");
	stream_return;
}
SqlUpdateStr* SqlUpdater::CreateUpdateStr(SqlTable &table)
{
	char temp[TEMP_STR_LENGTH];
	UINT64 mask = 1;
	stream_init;
	SqlUpdateHashMap &updateList = table.InnerGetUpdateList();
	SqlUpdateHashMap::iterator it;
	for (it = updateList.begin(); it != updateList.end(); ++it)
	{
		SqlRow *pData = (SqlRow*)it->first;
		UINT id = table.GetRowID(pData);
		stream("update ");
		stream(table.GetTableName());
		stream(" set ");
		bool bfirst = true;
		for (BYTE i = 0; i < COLUMN_MAX_COUNT; ++i)
		{
			if (pData->ColumnMask & (mask << i))
			{
				const SqlColumn &f = table.GetColumn(i);
				if (bfirst)
					bfirst = false;
				else
					stream(",");
				stream(f.Name);
				stream("='");
				stream(table.CopyFieldToStr(pData, i, temp, sizeof(temp)));
				stream("'");
			}
		}
		stream(" where id='");
		stream_int(id);
		stream("';");
		
	}
	stream_return;
}
SqlUpdateStr* SqlUpdater::CreateDeleteStr(SqlTable &table)
{
	stream_init;
	stream("delete from ");
	stream(table.GetTableName());
	stream(" where id in (");
	bool bFirst = true;
	for (UINT i = 0; i < table.InnerGetDeleteList().size(); ++i)
	{
		UINT id = table.InnerGetDeleteList()[i];
		if (bFirst)
			bFirst = false;
		else
			stream(",");
		stream("'");
		stream_int(id);
		stream("'");
				
	}
	stream(");");
	stream_return;
	
}
SqlUpdateStr* SqlUpdater::CreateInsertStr(SqlTable &table)
{
	stream_init;
	char temp[TEMP_STR_LENGTH];
	bool bStart = true;
	Timer t;
	t.Begin();
	char cols[TEMP_STR_LENGTH];
	cols[0] = 0;
	BYTE startColumns = (BYTE)table.IsEnableDatabaseID();
	for (BYTE i = startColumns; i < table.Columns(); ++i)
	{
		if (bStart)	
			bStart = false;
		else	
			strcat_s(cols, sizeof(cols), ",");
		strcat_s(cols, sizeof(cols), table.GetColumn(i).Name);
	}
	stream("insert into ");
	stream(table.GetTableName());
	stream("(");
	stream(cols);
	stream(") values ");

	bool bfirst = true;
	for (UINT i = table.GetOrgRowCount(); i < table.Rows(); ++i)
	{
		if (bfirst)
			bfirst = false;
		else
			stream(",");
		//const BYTE *pRow = (BYTE*)table.GetRow(i);
		bool bStart = true;
		stream("(");
		for (BYTE j = startColumns; j < table.Columns(); ++j)
		{
			if (bStart)
				bStart = false;
			else
				stream(",");
			const char *pv = table.CopyFieldToStr(i, j, temp, sizeof(temp));
			stream("'");
			stream(pv);
			stream("'");
		}
		stream(")");
	}
	stream(";");

	stream_return;
}
