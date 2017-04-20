#include "stdafx.h"
#include "mysql.h"
#pragma comment(lib, "libmysql.lib")
#define m_MYSQL (MYSQL*)m_pMySQL

SQL::SQL()
{
	m_pMySQL = NULL;
	m_bConnected = false;
}
SQL::~SQL()
{
	Shutdown();
}
bool	SQL::Init(const SQLInitData& data)
{
	m_pMySQL = mysql_init(new MYSQL);
	if (mysql_real_connect(m_MYSQL, data.IP, data.UID, data.PWD, data.DB, data.PORT, NULL, CLIENT_MULTI_STATEMENTS))
	{
		char value = 1;
		mysql_options(m_MYSQL, MYSQL_OPT_RECONNECT, &value);//设置MySQL 自动连接 不会断开
		int ret = mysql_set_character_set(m_MYSQL, "gbk");
		//mysql_set_server_option(m_MYSQL, MYSQL_OPTION_MULTI_STATEMENTS_ON);
		m_bConnected = true;
		m_MySQlConnectData = data;
		return true;
	}
	else
	{
		Log("Connect mysql error:%s", mysql_error(m_MYSQL));
		return false;
	}
}
void SQL::TryConnectMySql()
{
	//判断当前Mysql  连接是否开启 未开启的话 根据保存的MySQL 数据 重新连接下
	if (mysql_ping(m_MYSQL) != 0)
	{
		m_pMySQL = mysql_init(new MYSQL);
		if (mysql_real_connect(m_MYSQL, m_MySQlConnectData.IP, m_MySQlConnectData.UID, m_MySQlConnectData.PWD, m_MySQlConnectData.DB, 3306, NULL, CLIENT_MULTI_STATEMENTS))
		{
			char value = 1;
			mysql_options(m_MYSQL, MYSQL_OPT_RECONNECT, &value);//设置MySQL 自动连接 不会断开
			int ret = mysql_set_character_set(m_MYSQL, "gbk");
			m_bConnected = true;
			return;
		}
		else
		{
			m_bConnected = false;
			Log("Connect mysql error:%s", mysql_error(m_MYSQL));
			return;
		}
	}
}
void	SQL::Shutdown()
{
	if (m_MYSQL)
	{
		mysql_close(m_MYSQL);
		delete(m_MYSQL);
		m_pMySQL = NULL;
		m_bConnected = false;
	}
}
UINT	SQL::GetIncrementID(const char *table)
{
	char str[512];
	sprintf_s(str, sizeof(str), "SELECT AUTO_INCREMENT FROM information_schema.TABLES  WHERE TABLE_NAME = '%s'", table);
	if (mysql_query(m_MYSQL, str) == 0)
	{
		MYSQL_RES *pRes = mysql_store_result(m_MYSQL);
		UINT id = (UINT)atoi(mysql_fetch_row(pRes)[0]);
		mysql_free_result(pRes);
		return id;
	}
	else
		return 0xffffffff;
}
bool SQL::GetResult(SqlTable &table)
{
	if (mysql_next_result(m_MYSQL))
	{
		MYSQL_RES *pRes;
		pRes = mysql_store_result(m_MYSQL);
		bool bRet = table.InitFromSqlData(TABLE_USERTABLE, m_pMySQL, pRes);
		mysql_free_result(pRes);
		return true;
	}
	else
		return false;

}
bool SQL::Select(const char *sql, BYTE tableName, SqlTable &tableResult, bool singleTable)
{
	UNREFERENCED_PARAMETER(singleTable);
	TryConnectMySql();
	if (mysql_query(m_MYSQL, sql) == 0)
	{
		MYSQL_RES *pRes;
		pRes = mysql_store_result(m_MYSQL);
		if (pRes == NULL)
		{
			if (mysql_error(m_MYSQL) != NULL)
			{
				Log("Select Error:%s, msg:%s", sql, mysql_error(m_MYSQL));
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			bool bRet = tableResult.InitFromSqlData(tableName, m_pMySQL, pRes);
			mysql_free_result(pRes);

			while (mysql_next_result(m_MYSQL))
			{
				MYSQL_RES *pRes2;
				pRes2 = mysql_store_result(m_MYSQL);
				if (!pRes2)
					break;
				mysql_free_result(pRes2);
			}
			return bRet;
		}
	}
	else
	{
		Log("SelectSql Error:%s, msg:%s", sql, mysql_error(m_MYSQL));
		return false;
	}
}
bool	SQL::Excute(const char *sql, UINT &rowCount)
{
	if (mysql_query(m_MYSQL, sql) == 0)
	{
		rowCount = (UINT)mysql_affected_rows(m_MYSQL);
		while (!mysql_next_result(m_MYSQL));
		return true;
	}
	else
	{
		Log("ExcuteSql Error:%s, msg:%s", sql, mysql_error(m_MYSQL) );
		return false;
	}
}
bool	SQL::RealExcute(const char *sql, UINT size)
{
	//Log("excute str:%d", size);
	if (mysql_real_query(m_MYSQL, sql, size) == 0)
	{
		while (!mysql_next_result(m_MYSQL));

		return true;
	}
	else
	{
		Log("ExcuteSql Error:%s, msg:%s", sql, mysql_error(m_MYSQL));
		return false;
	}
}
bool	SQL::Insert(const char *sql, UINT size, vector<UINT>& idList)
{
	//Log("Insert size:%d, str:%s", size, sql);
	if (mysql_real_query(m_MYSQL, sql, size) == 0)
	{
		do
		{
			idList.push_back((uint)mysql_insert_id(m_MYSQL));
		} while (!mysql_next_result(m_MYSQL));
		return true;
	}
	else
	{
		Log("ExcuteSql Error(%d):, msg:%s", size, mysql_error(m_MYSQL));
		return false;
	}
}
void SQL::GetMySqlEscapeString(char* pArray, UINT Size, char* OutStr)
{
	//进行字符串转化
	mysql_real_escape_string(m_MYSQL, OutStr, pArray, Size);
}