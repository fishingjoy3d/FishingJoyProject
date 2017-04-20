#include "stdafx.h"
#include "mysql.h"

bool SqlTable::InnerUpdate(const WCHAR *pcStr, UINT size, UINT row, BYTE col)
{
	if (row >= Rows() || col >= Columns())
		return false;
	USHORT length = GetColumn(col).Length;
	if (size > length)
	{
		Log("字符串长度超出范围:%d", size);
		return false;
	}
	WCHAR *pstr = (WCHAR*)GetField(row, col);
	memcpy_s(pstr, (rsize_t)length << 1, pcStr, (rsize_t)size << 1);
	pstr[size] = 0;
	if (IsOrgRow(row))
		SetFieldUdpate(InnerGetRow(row), col);
	return true;
}
void SqlTable::SetFieldUdpate(SqlRow*pRow, BYTE col)
{
	if (pRow->ColumnMask == 0)
	{
		m_UpdateList[(UINT_PTR)pRow] = 1;
		SetNeedCommit();
	}
	pRow->ColumnMask |= 1LL << col;
}
bool SqlTable::InnerDelete(UINT row)
{
	if (row >= Rows())
		return false;
	SqlRow *pRow = InnerGetRow(row);
	if (row < m_TableData.OrgRowCount)
	{
		--m_TableData.OrgRowCount;
		m_DeleteList.push_back(GetRowID(row));
	}
	m_UpdateList.erase((UINT_PTR)pRow);
	m_RowList.erase(m_RowList.begin() + row);

	//提交到索引
	m_Index.OnDelete(pRow, this);

	//释放
	free(pRow);
	SetNeedCommit();
	return true;
}
bool SqlTable::InnerInsert(UINT num)
{
	if (m_TableData.Flags & TABLE_CHUNK)
	{
		BYTE *pData = (BYTE*)malloc(num * GetRowSize());
		if (!pData)
			return false;
		memset(pData, 0, num * GetRowSize());
		for (uint i = 0; i < num; ++i)
		{
			SqlRow *pRow = (SqlRow*)(pData);
			m_RowList.push_back(pRow);
			pData += GetRowSize();
		}
	}
	else
	{
		for (UINT i = 0; i < num; ++i)
		{
			SqlRow *pRow = (SqlRow*)malloc(GetRowSize());
			if (pRow == NULL)
				return false;
			memset(pRow, 0, GetRowSize());
			m_RowList.push_back(pRow);
			SetRowID(m_RowList.size() - 1, m_TableData.RowIncID++);
		}
	}
	SetNeedCommit();
	return true;
}
bool SqlTable::BeginCreate(BYTE tableName, BYTE columns)
{
	if (HasFlag()) 
		return false;
	m_TableData.TableName = tableName;
	m_TableData.Columns = columns;
	m_pColumns = (SqlColumn*)malloc(columns * sizeof(SqlColumn));
	SetFlag(TABLE_CREATING);
	return true;
}
bool SqlTable::SetColumn(BYTE column, const char *pcColumns, SqlDataType ft, USHORT length)
{
	if (column >= Columns())
		return false;
	SqlColumn &col = m_pColumns[column];
	col.Type = static_cast<BYTE>(ft);
	col.Index	= column;
	col.Length	= length;
	strcpy_s(col.Name, sizeof(col.Name), pcColumns);
	return true;
}
bool SqlTable::EndCreate()
{
	m_TableData.RowSize = sizeof(SqlRow);
	for (BYTE i = 0; i < Columns(); ++i)
	{
		SqlColumn &col = InnerGetColumn(i);
		col.Offset = static_cast<WORD>(m_TableData.RowSize);
		if (IS_WCHAR_TYPE(col.Type))
			m_TableData.RowSize += (col.Length + 1) << 1;
		else
			m_TableData.RowSize += col.Length;
	}
	ClearFlag(TABLE_CREATING);
	SetFlag(TABLE_CREATED);
	m_Index.Create(&GetColumn(0));
	return true;
}
void SqlTable::Clear()
{
	if (m_TableData.Flags & TABLE_CHUNK)
	{
		if (Rows() > 0)
			free(InnerGetRow(0));
		memset(&m_TableData, 0, sizeof(m_TableData));
	}
	else
	{
		memset(&m_TableData, 0, sizeof(m_TableData));
		for (UINT i = 0; i < Rows(); ++i)
			free(InnerGetRow(i));
	}
	if (IsAttachColumn())
	{
		m_pColumns = NULL;
	}
	else
	{
		SAFE_FREE(m_pColumns);
	}
	m_RowList.clear();
	m_UpdateList.clear();
	m_DeleteList.clear();
	m_Index.Clear();
}

bool SqlTable::InitFromSqlData(BYTE tableName, void *psql, void *pres)
{
	UNREFERENCED_PARAMETER(psql);
	//MYSQL		*sql		= (MYSQL*)psql;
	MYSQL_RES	*res		= (MYSQL_RES*)pres;
	UINT		rowCount	= (UINT)mysql_num_rows(res);
	BYTE		colCount	= (BYTE)mysql_num_fields(res);
	MYSQL_FIELD	*fields		= mysql_fetch_fields(res);

	if (colCount > COLUMN_MAX_COUNT)
		return false;
	
	if (!BeginCreate(tableName, colCount))
		return false;

	SetFlag(TABLE_CHUNK);
	for (BYTE i = 0; i < Columns(); ++i)
	if (!InitColumnFromSqlData(&fields[i], &InnerGetColumn(i), i))
			return false;

	if (!EndCreate())
		return false;
	if (!Insert(rowCount))
		return false;

	UINT rowIdx = 0;
	MYSQL_ROW row = mysql_fetch_row(res);
	while (row)
	{
		//ULONG *lengths = mysql_fetch_lengths(res);
		for (BYTE i = 0; i < Columns(); ++i)
		{
			if (!ConvertStrToFieldData((SqlDataType)GetColumn(i).Type, GetColumn(i).Length, InnerGetField(rowIdx, i), row[i]))
				return false;
		}
		++rowIdx;
		row = mysql_fetch_row(res);
	}
	m_TableData.OrgRowCount = Rows();
	m_TableData.Flags = GetFixedFlags() | TABLE_DATABASE | TABLE_CHUNK;
	//m_Index.Create(&GetColumn(0));
	return true;
}
bool SqlTable::CreateSqlStr(vector<SqlUpdateStr*> &strList, bool bReset)
{
	if (!IsNeedCommit())
		return true;
	if (!SqlUpdater::CreateUpdateStr(*this, strList))
		return false;

	if (bReset == false)
		return true;

	SetCommited();
	return true;
}

BYTE*	SqlTable::ToBytes(UINT &size, bool bContainsColumn)const
{
	if (!HasFlag())
	{
		Log("Table is null.");
		return null;
	}
	size =
		sizeof(bool) +		//contains column
		sizeof(UINT) +		//Rows
		sizeof(UINT) +		//DeleteCount
		sizeof(TableData) +		
		(bContainsColumn ? Columns() * sizeof(SqlColumn) : 0) + 
		Rows() * GetRowSize() + 
		GetDeleteRowCount() * sizeof(UINT);

	BYTE *pData = (BYTE*)malloc(size);
	if (pData == NULL)
		return NULL;
	BYTE *pcursor = pData;
	UINT rows = Rows();
	UINT deletes = GetDeleteRowCount();
	WriteStream(pcursor, &bContainsColumn, sizeof(bContainsColumn));
	WriteStream(pcursor, &rows, sizeof(rows));
	WriteStream(pcursor, &deletes, sizeof(deletes));
	WriteStream(pcursor, &m_TableData, sizeof(TableData));
	if (bContainsColumn)
		WriteStream(pcursor, m_pColumns, Columns() * sizeof(SqlColumn));

	for (UINT i = 0; i < rows; ++i)
		WriteStream(pcursor, GetRow(i), GetRowSize());
	for (UINT i = 0; i < m_DeleteList.size(); ++i)
		WriteStream(pcursor, &m_DeleteList[i], sizeof(m_DeleteList[i]));
	return pData;
}
bool	SqlTable::FromBytes(const BYTE* pBytes, UINT size, SqlColumn *pAttachColumn)
{
	if (HasFlag())
	{
		Log("Table is not null.");
		return false;
	}
	const BYTE *pTemp = pBytes;
	UINT rows, deleteCount;
	bool bContainsColumn;
	ReadStream(pTemp, &bContainsColumn, sizeof(bContainsColumn));
	ReadStream(pTemp, &rows, sizeof(rows));
	ReadStream(pTemp, &deleteCount, sizeof(deleteCount));
	ReadStream(pTemp, &m_TableData, sizeof(TableData));
	if (bContainsColumn)
	{
		m_pColumns = (SqlColumn*)malloc(Columns() * sizeof(SqlColumn));
		if (m_pColumns == NULL)
		{
			Clear();
			return false;
		}
		ReadStream(pTemp, m_pColumns, Columns() * sizeof(SqlColumn));
		ClearFlag(TABLE_ATTACHCOLUMN);
	}
	else
	{
		m_pColumns = pAttachColumn;
		SetFlag(TABLE_ATTACHCOLUMN);
	}
	if (!Insert(rows))
	{
		Clear();
		return false;
	}
	for (UINT i = 0; i < GetOrgRowCount(); ++i)
	{
		SqlRow *pRow = InnerGetRow(i);
		ReadStream(pTemp, pRow, GetRowSize());
		if (pRow->ColumnMask != 0)
			m_UpdateList[(UINT_PTR)pRow] = 1;
		m_Index.OnInsert(pRow, this);
	}
	for (UINT i = GetOrgRowCount(); i < Rows(); ++i)
	{
		SqlRow *pRow = InnerGetRow(i);
		ReadStream(pTemp, pRow, GetRowSize());
		if (pRow->ColumnMask != 0)
			m_UpdateList[(UINT_PTR)pRow] = 1;
	}
	for (UINT i = 0; i < deleteCount; ++i)
	{
		UINT id;
		ReadStream(pTemp, &id, sizeof(id));
		m_DeleteList.push_back(id);
	}
	if (pTemp != pBytes + size)
		Log("FromBytes 结尾不正确");
	return true;
}
void SqlTable::SetCommited()
{
	//提交到索引
	for (UINT i = GetOrgRowCount(); i < Rows(); ++i)
		m_Index.OnInsert(GetRow(i), this);

	for (SqlUpdateHashMap::iterator it = m_UpdateList.begin(); it != m_UpdateList.end(); ++it)
	{
		SqlRow *pRow = (SqlRow*)it->first;
		m_Index.OnUpdate(pRow, pRow->ColumnMask, this);
		pRow->ColumnMask = 0;
	}
	//提交结束
	m_UpdateList.clear();
	m_DeleteList.clear();
	m_TableData.OrgRowCount = Rows();
	m_TableData.Flags = GetFixedFlags() | TABLE_DATABASE;
}
int CompareRow(const SqlRow * pa, const SqlRow *pb)
{
	UINT id1 = *(UINT*)(pa + 1/*sizeof(SqlRow)*/);
	UINT id2 = *(UINT*)(pb + 1/*sizeof(SqlRow)*/);
	if (id1 > id2)
		return 1;
	else if (id1 < id2)
		return -1;
	else
		return 0;
}
bool SqlTable::CombineFromBytes(const BYTE* pBytes, UINT size)
{
	//合并到当前表中
	const BYTE *pTemp = pBytes;
	UINT rows, deleteCount;
	bool bContainsColumn;
	TableData td;
	ReadStream(pTemp, &bContainsColumn, sizeof(bContainsColumn));
	ReadStream(pTemp, &rows, sizeof(rows));
	ReadStream(pTemp, &deleteCount, sizeof(deleteCount));
	ReadStream(pTemp, &td, sizeof(TableData));
	if (td.RowSize != GetRowSize())
		return false;

	if (bContainsColumn)
	{
		pTemp += (Columns() * sizeof(SqlColumn));
	}
	SqlRow *pReadRow = (SqlRow*)malloc(sizeof(GetRowSize()));
	if (!pReadRow)
		return false;
	for (UINT i = 0; i < td.OrgRowCount; ++i)
	{
		ReadStream(pTemp, pReadRow, GetRowSize());
		UINT id = GetRowID(pReadRow);
		SqlRow *pRow = (SqlRow*)m_Index.GetRowByID(id);
		if (pRow == NULL)
			continue;

		if (pRow->ColumnMask != 0)
		{
			memcpy(pRow, pReadRow, GetRowSize());
			m_UpdateList[(UINT_PTR)pRow] = 1;
			SetNeedCommit();
		}
	}
	for (UINT i = td.OrgRowCount; i < rows; ++i)
	{
		ReadStream(pTemp, pReadRow, GetRowSize());
		if (!InnerInsert(1))
			return false;
		
		SqlRow *pRow = InnerGetRow(Rows() - 1);
		memcpy(pRow, pReadRow, GetRowSize());
		SetNeedCommit();
	}
	for (UINT i = 0; i < deleteCount; ++i)
	{
		UINT id;
		ReadStream(pTemp, &id, sizeof(id));
		*((UINT*)GetField(pReadRow, 0)) = id;
		vector<SqlRow*>::iterator it = ::lower_bound(m_RowList.begin(), m_RowList.end(), pReadRow, CompareRow);
		if (it != m_RowList.end())
		{
			--m_TableData.OrgRowCount;
			m_UpdateList.erase((UINT_PTR)*it);
			m_Index.OnDelete(*it, this);
			m_RowList.erase(it);
			m_DeleteList.push_back(id);
			SetNeedCommit();
		}
	}
	free(pReadRow);
	if (pTemp != pBytes + size)
		Log("FromBytes 结尾不正确");

	return true;
}
bool SqlTable::InitFromResult(SqlIDList &rows, UINT64 columnMask, BYTE tableName, BYTE columnCount, USHORT rowSize, SqlColumn *pColumns)
{
	m_TableData.Columns = GetColumnCount(columnMask);
	m_TableData.Flags = TABLE_DATABASE;
	m_TableData.OrgRowCount = rows.size();
	m_TableData.TableName = tableName;
	const UINT64 mask = 1;
	bool newColumns = false;
	if (m_TableData.Columns != columnCount)
	{
		newColumns = true;
		m_pColumns = (SqlColumn*)malloc(m_TableData.Columns);
		UINT idx = 0;
		USHORT offset = 0;
		for (UINT i = 0; i < columnCount; ++i)
		{
			if ((mask << i) & columnMask)
			{
				m_pColumns[idx] = pColumns[i];
				m_pColumns[idx].Offset = offset;
				if (IS_WCHAR_TYPE(pColumns[i].Type))
					offset += (pColumns[i].Length + 1) << 1;
				else
					offset += pColumns[i].Length;
				++idx;
			}
		}
		m_TableData.RowSize = offset;
	}
	else
	{
		m_TableData.RowSize = rowSize;
		m_pColumns = pColumns;
		SetFlag(TABLE_ATTACHCOLUMN);
	}
	UINT row = 0;
	for (SqlIDList::iterator it = rows.begin(); it != rows.end(); ++it)
	{
		Insert(1);
		SqlRow *p = InnerGetRow(Rows() - 1);
		if (!newColumns)
			memcpy(p, (SqlRow*)it->first, m_TableData.RowSize);
		else
		{
			BYTE idx = 0;
			for (BYTE i = 0; i < columnCount; ++i)
			{
				if ((mask << i) & columnMask)
				{
					BYTE *pdata = InnerGetField(row, idx);
					if (IS_WCHAR_TYPE(pColumns[i].Type))
						memcpy(pdata, ((BYTE*)it->first + pColumns[i].Offset), (size_t)(pColumns[i].Length + 1) << 1);
					else
						memcpy(pdata, ((BYTE*)it->first + pColumns[i].Offset), (size_t)pColumns[i].Length);
				}
			}
		}
		m_TableData.RowIncID = GetRowID(p);
		++row;
	}

	m_Index.Create(&GetColumn(0));
	return true;
}
bool SqlTable::Select(const SqlCondition &d, SqlTable &table)
{
	SqlIDList idlist;
	if (!m_Index.Query(d, idlist))
		return false;

	return table.InitFromResult(idlist, d.GetColumnMask(), m_TableData.TableName, Columns(), static_cast<WORD>(GetRowSize()), m_pColumns);
}
bool SqlTable::SelectUnion(const SqlUnion&ud, SqlTable &maintable, SqlTable &retResult)
{
	UNREFERENCED_PARAMETER(retResult);
	UNREFERENCED_PARAMETER(ud);
	UNREFERENCED_PARAMETER(maintable);
	/*if (GetColumn(ud.UnionColumn).Length != sizeof(UINT) ||
		maintable.GetColumn(ud.MainColumn).Length != sizeof(UINT))
	{
		Log("必须是UINT的列可做联合查询");
		return false;
	}
	SqlIDList idlist;
	for (UINT i = 0; i < maintable.Rows(); ++i)
	{
		
		const BYTE *pData = maintable.GetField(i, ud.MainColumn);
		SqlCondition di = COLUMN(ud.UnionColumn) == (*(UINT*)pData);
		m_Index.Query(di, idlist);
	}*/
	return true;
}