#pragma once
#define INIT_STR_LENGTH 128
#define SQL_TEST 0

enum
{
	MAX_PACKET_SIZE = 1024 * 1024 * 64
};
struct SQLInitData
{
	char	IP[INIT_STR_LENGTH];
	char	DB[INIT_STR_LENGTH];
	char	UID[INIT_STR_LENGTH];
	char	PWD[INIT_STR_LENGTH];
	int		PORT;
};

class SQL
{
public:
	SQL();
	~SQL();
	bool	Init(const SQLInitData& data);
	void	Shutdown();
	bool	GetResult(SqlTable &table);
	bool	Select(const char *sql, BYTE table, SqlTable &tableResult, bool singleTable);
	bool	Excute(const char *sql,	UINT &rowCount);
	bool	RealExcute(const char *sql, UINT size);
	bool	Insert(const char *psql, UINT size, vector<UINT>& idList);
	bool	IsConnected()const
	{
		return m_bConnected;
	}
	void	GetMySqlEscapeString(char* pArray, UINT Size, char* OutStr);

	void	TryConnectMySql();
protected:
	UINT	GetIncrementID(const char *table);
	void	*m_pMySQL;
	bool	m_bConnected;
	SQLInitData m_MySQlConnectData;

};