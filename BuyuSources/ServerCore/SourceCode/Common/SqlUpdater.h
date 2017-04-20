#pragma once

struct SqlUpdateStr
{
	UINT	Size;
	char	Str[1];
};

class SqlUpdater
{
public :
	static bool CreateUpdateStr(SqlTable &table, vector<SqlUpdateStr*>& strList);
protected:
	static SqlUpdateStr* CreateNewTableStr(SqlTable &table);
	static SqlUpdateStr* CreateUpdateStr(SqlTable &table);
	static SqlUpdateStr* CreateDeleteStr(SqlTable &table);
	static SqlUpdateStr* CreateInsertStr(SqlTable &table);
};
