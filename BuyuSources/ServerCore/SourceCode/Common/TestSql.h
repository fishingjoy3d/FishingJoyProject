#pragma once

class TestSql
{
public:
	TestSql()
	{
		m_bRun = false;
	}
	~TestSql()
	{
	}
	void Init();
	void Thread();
	void Shutdown()
	{
		m_bExit = false;
		m_bRun = false;
		while (m_bExit == false)
			Sleep(10);
	}
protected:
	volatile bool	m_bExit;
	volatile bool	m_bRun;
	SQL				m_Sql;
	SqlTable		m_Table;


};