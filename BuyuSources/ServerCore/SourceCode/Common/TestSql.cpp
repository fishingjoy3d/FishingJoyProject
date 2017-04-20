#include "stdafx.h"
char g_Temp[1024];
const WCHAR *g_TestStr = L"1234567890-=~!@#$%^&*()_+|qwertyuiop[]asdfghjkl;zxcvbnm,./QWERTYUIOPASDFGHJKLZXCVBNM{}:<>?我人有的和主不为这工要在地一上是中国国经以发了民同多从朋折笔方立水炎之式林大寺五止昌吕男比双妇子忆册";
const int g_TestStrSize = wcslen(g_TestStr);
UINT WINAPI ThreadFunc(void *p)
{
	((TestSql*)p)->Thread();
	return 0;
}

const char *RandFieldType(SqlDataType &ft, UINT &length, UINT idx)
{
	UINT type = RandRange(SQL_SBYTE, SQL_DATETIME + 1);
	//type = SQL_UINT64;
	ft = (SqlDataType)type;
	switch (ft)
	{
	case SQL_BYTE:
		length = 1;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "Byte", length);
		return g_Temp;
	case SQL_SBYTE:
		length = 1;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "SByte", length);
		return g_Temp;
	case SQL_SHORT:
		length = 2;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "Short", length);
		return g_Temp;
	case SQL_USHORT:
		length = 2;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "UShort", length);
		return g_Temp;
	case SQL_INT:
		length = 4;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "Int", length);
		return g_Temp;
	case SQL_UINT:
		length = 4;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "UInt", length);
		return g_Temp;
	case SQL_INT64:
		length = 8;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "Int64", length);
		return g_Temp;
	case SQL_UINT64:
		length = 8;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "UInt64", length);
		return g_Temp;
	case SQL_FLOAT:
		length = 4;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "Float", length);
		return g_Temp;
	case SQL_VARCHAR:
		length = static_cast<UINT>(RandRange(1, 400));
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "VarChar", length);
		return g_Temp;
	case SQL_DATETIME:
		length = 8;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "DateTime", length);
		return g_Temp;
	case SQL_BIT:
		length = 1;
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "bit", length);
		return g_Temp;
	case SQL_BLOB:
		length = (UINT)RandRange(1, 255);
		sprintf_s(g_Temp, "Column%d_%s_%d", idx, "Blob", length);
		return g_Temp;
	default:
		Log("未知的字段类型:%d", type);
		return "";
	}
}
void RandFieldData(SqlTable &t, UINT row, BYTE col)
{
	const SqlColumn &f = t.GetColumn(col);
	UINT64 mem;
	float fmem;
	UINT len;
	switch (f.Type)
	{
	case SQL_SBYTE:
		mem = RandRange(-127, 127);
		t.Update((CHAR)mem, row, col);
		break;
	case SQL_BYTE:
		mem = RandRange(0, 0xff);
		t.Update((BYTE)mem, row, col);
		break;
	case SQL_SHORT:
		mem =RandRange(-32767, 32767);
		t.Update((SHORT)mem, row, col);
		break;
	case SQL_USHORT:
		mem = RandRange(0, 0xffff);
		t.Update((USHORT)mem, row, col);
		break;
	case SQL_INT:
		mem = RandRange(INT_MIN, INT_MAX);
		t.Update((INT)mem, row, col);
		break;
	case SQL_UINT:
		mem = RandRange(0, 0xffffffff);
		t.Update((UINT)mem, row, col);
		break;
	case SQL_INT64:
		mem = (int64)(RandFloat() * MAXINT64);
		t.Update((int64)mem, row, col);
		break;
	case SQL_UINT64:
		mem = (uint64)(RandFloat() * MAXUINT64);
		t.Update((UINT64)mem, row, col);
		break;
	case SQL_FLOAT:
		fmem = RandFloat() * SHRT_MAX;
		mem = UINT(fmem * 100);
		fmem = mem / 100.0f;
		t.Update(fmem, row, col);
		break;
	case SQL_VARCHAR:
	{
		len = static_cast<UINT>(RandRange(1, f.Length));
		WCHAR *pc = new WCHAR[len + 1];
		for (UINT i = 0; i < len; ++i)
			pc[i] = g_TestStr[RandRange(0, g_TestStrSize)];
		pc[len] = 0;
		t.Update(pc, len, row, col);
		delete[](pc);
		break;
	}
	case SQL_DATETIME:
		mem = ToDateTime(static_cast<WORD>(1980 + RandRange(0, 30)), static_cast<BYTE>(RandRange(1, 13)), static_cast<BYTE>(RandRange(1, 26)), static_cast<BYTE>(RandRange(0, 24)), static_cast<BYTE>(RandRange(0, 60)), static_cast<BYTE>(RandRange(0, 60)));
		t.Update(mem, row, col);
		break;
	case SQL_BIT:
		mem = RandRange(0, 2);
		t.Update(mem==1, row, col);
		break;
	default:
		Log("未知的字段类型:%d", f.Type);
		break;
	}
}
void TestSql::Init()
{
	SQLInitData sid;
	strcpy_s(sid.DB, sizeof(sid.DB), "buyu");
	strcpy_s(sid.IP, sizeof(sid.IP), "localhost");
	strcpy_s(sid.UID, sizeof(sid.UID), "root");
	strcpy_s(sid.PWD, sizeof(sid.PWD), "123456");
	m_Sql.Init(sid);
	m_bRun = true;
	::_beginthreadex(0, 0, ThreadFunc, this, 0, 0);
}

void TestSql::Thread()
{
	BYTE cols = static_cast<BYTE>(RandRange(12, 24));
	m_Table.BeginCreate(TABLE_USERTABLE, cols);
	m_Table.SetColumn(0, "id", SQL_INT, 4);
	vector<BYTE> dateList;
	for (BYTE i = 1; i < cols; ++i)
	{
		UINT len;
		SqlDataType ft;
		const char *pcName = RandFieldType(ft, len, i);
		m_Table.SetColumn(i, pcName, ft, static_cast<WORD>(len));
		if (ft == SQL_DATETIME)
			dateList.push_back(i);
	}
	m_Table.EndCreate();
	Log("表名:%s, 列数:%d", m_Table.GetTableName(), m_Table.Columns());
	const INT addNum = 100;
	const INT updateNum = 100;
	UINT saveCount = 0;
	UINT rowCount = 0;
	while (m_bRun)
	{
		cout << ".";
		UINT t = RandRange(0, 4);
		cout << t;
		if (t == 0)
		{
			//添加
			UINT n = RandRange(1, addNum);
			m_Table.Insert(n);
			rowCount += n;
			for (UINT i = m_Table.Rows() - n; i < m_Table.Rows(); ++i)
			{
				for (UINT j = 0; j < dateList.size(); ++j)
				{
					RandFieldData(m_Table, i, dateList[j]);
				}
			}
		}
		else if (t == 1 && m_Table.Rows() > 10)
		{
			//删除
			UINT n = RandRange(1, m_Table.Rows() / 10);
			for (UINT i = 0; i < n; ++i)
			{
				UINT idx = RandRange(0, m_Table.Rows());
				m_Table.Delete(idx);
				--rowCount;
			}
		}
		else if (t == 2 && m_Table.Rows() > 0)
		{
			//修改
			UINT n = RandRange(1, updateNum);
			UINT n2 = RandRange(1, updateNum);
			for (UINT i = 0; i < n; ++i)
			{
				for (UINT j = 0; j < n2; ++j)
				{
					UINT row = RandRange(0, m_Table.Rows());
					BYTE col = static_cast<BYTE>(RandRange(1, m_Table.Columns()));
					RandFieldData(m_Table, row, col);
				}
			}
		}
		else if (t == 3 && RandRange(0, 100) > 90 && rowCount > 0)
		{
			cout << (">1.");
			//UINT size;
			//BYTE *pData = m_Table.ToBytes(size);

			++saveCount;
			//Log("保存次数:%d, 行数:%d", saveCount, m_Table.Rows());
			//保存
			vector<SqlUpdateStr*> strList;
			if (!m_Table.CreateSqlStr(strList, true))
			{
				Log("保存失败.");
			}
			else
			{
				cout << (">2.");
				for (UINT i = 0; i < strList.size(); ++i)
				{
					m_Sql.RealExcute(strList[i]->Str, strList[i]->Size);
					free(strList[i]);
				}
				cout << (">21.");
				//读取出来比较内容
				SqlTable t;
				if (!m_Sql.Select("select * from TestTable", TABLE_USERTABLE, t, true))
					Log("读取表失败");
				else
				{
					if (t.Rows() != m_Table.Rows() )
						Log("表的数据行不匹配:%d, %d, %d",t.Rows(), m_Table.Rows(), rowCount);
					else if (t.Columns() != m_Table.Columns())
						Log("表的数据列不匹配");
					else
					{
						cout << (">22.");
						for (BYTE i = 0; i < t.Columns(); ++i)
						{
							if (IS_WCHAR_TYPE(t.GetColumn(i).Type))
							{
								if (t.GetColumn(i).Length != m_Table.GetColumn(i).Length)
									Log("表的列信息不匹配1");
							}
							else if (t.GetColumn(i).Length != m_Table.GetColumn(i).Length)
							{
								Log("表的列信息不匹配2");
							}
						}
						cout << (">23.");
						for (UINT i = 0; i < t.Rows(); ++i)
						{
							for (BYTE j = 0; j < t.Columns(); ++j)
							{
								const BYTE *p1 = t.GetField(i, j);
								const BYTE *p2 = m_Table.GetField(i, j);
								
								bool bRet;
								if (IS_WCHAR_TYPE(t.GetColumn(j).Type))
								{
									bRet = wcscmp((WCHAR*)p1, (WCHAR*)p2) == 0;
								}
								else
									bRet = memcmp(p1, p2, t.GetColumn(j).Length) == 0;

								if (!bRet)
								{
									Log("数据不相等, row:%d, col:%d , ID1:%d, id2:%d:", i, j, t.GetRowID(i), m_Table.GetRowID(i));
									switch (t.GetColumn(j).Type)
									{
									case SQL_BYTE:
										Log("%u, %u", *p1, *p2);
										break;
									case SQL_SBYTE:
										Log("%d, %d", *(char*)p1, *(char*)p2);
										break;
									case SQL_SHORT:
										Log("%d, %d", *(SHORT*)p1, *(SHORT*)p2);
										break;
									case SQL_USHORT:
										Log("%u, %u", *(USHORT*)p1, *(USHORT*)p2);
										break;
									case SQL_INT:
										Log("%d, %d", *(INT*)p1, *(INT*)p2);
										break;
									case SQL_UINT:
										Log("%u, %u", *(UINT*)p1, *(UINT*)p2);
										break;
									case SQL_INT64:
										Log("%lld, %lld", *(INT64*)p1, *(INT64*)p2);
										break;
									case SQL_UINT64:
										Log("%llu, %llu", *(UINT64*)p1, *(UINT64*)p2);
										break;
									case SQL_FLOAT:
										Log("%f, %f", *(FLOAT*)p1, *(FLOAT*)p2);
										break;
									case SQL_VARCHAR:
										Log(L"%s, %s", (WCHAR*)p1, (WCHAR*)p2);
										break;
									case SQL_DATETIME:
										Log(L"%s, %s", (WCHAR*)p1, (WCHAR*)p2);
										break;
									case SQL_BIT:
										Log("%d, %d", *(char*)p1, *(char*)p2);
										break;
									case SQL_BLOB:
										Log("%d, %d", *(char*)p1, *(char*)p2);
										break;
									}
									//
								}
							}
						}
						cout << (">24.");
					}
				}
				cout<<(">3.");
			}
		}

		Sleep(10);
	}
	m_bExit = true;
}