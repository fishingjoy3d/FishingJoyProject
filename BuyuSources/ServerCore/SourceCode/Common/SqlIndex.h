#pragma once

inline ISqlIndexMap* GetIndexMapFromType(const SqlColumn *pColumn){
	switch (pColumn->Type){
	case SQL_BYTE:		return new SqlIndexMap<BYTE>(pColumn);
	case SQL_SBYTE:		return new SqlIndexMap<CHAR>(pColumn);
	case SQL_SHORT:		return new SqlIndexMap<SHORT>(pColumn);
	case SQL_USHORT:	return new SqlIndexMap<USHORT>(pColumn);
	case SQL_INT:		return new SqlIndexMap<INT>(pColumn);
	case SQL_UINT:		return new SqlIndexMap<UINT>(pColumn);
	case SQL_INT64:		return new SqlIndexMap<INT64>(pColumn);
	case SQL_UINT64:	return new SqlIndexMap<UINT64>(pColumn);
	case SQL_FLOAT:		return new SqlIndexMap<FLOAT>(pColumn);
	case SQL_DATETIME:  return new SqlIndexMap<UINT64>(pColumn);
	case SQL_VARCHAR:	return new SqlStringIndexMap(pColumn);
	case SQL_BIT:		return new SqlIndexMap<BYTE>(pColumn);
	case SQL_BLOB:		return new SqlStringIndexMap(pColumn);
	default:			return NULL;
	}
}


//表索引, 一张表最多支持TABLE_INDEX_MAX个索引
class SqlIndex
{
public:
	SqlIndex()
	{
		m_IndexCount = 0;
	}
	~SqlIndex()
	{
		Clear();
	}
	void Clear()
	{
		for (UINT i = 0; i < m_IndexCount; ++i)
		{
			delete m_IndexMap[i];
			m_IndexMap[i] = NULL;
		}
		m_IndexCount = 0;
	}
	UINT IndexCount()const{
		return m_IndexCount;
	}
	bool HasColumnIndex(const SqlColumn *pColumn)const{
		return NULL != GetIndexFromColumn(pColumn->Index);
	}
	bool Create(const SqlColumn *pColumn)
	{
		if (HasColumnIndex(pColumn))
			return false;
		m_IndexMap[m_IndexCount] = GetIndexMapFromType(pColumn);
		if (m_IndexMap[m_IndexCount] != null)
		{
			++m_IndexCount;
			return true;
		}
		else
		{
			Log("创建索引失败.");
			return false;
		}
	}
	bool OnInsert(const SqlRow *pRow, const SqlTable *pTable)
	{
		for (UINT i = 0; i < m_IndexCount; ++i)
		{
			if (!m_IndexMap[i]->OnInsert(pTable, pRow))
				return false;
		}
		return true;
	}
	void OnDelete(const SqlRow *pRow, SqlTable *pTable)
	{
		for (UINT i = 0; i < m_IndexCount; ++i)
		{
			m_IndexMap[i]->OnDelete(pTable, pRow);
		}
	}
	void OnUpdate(const SqlRow *pRow, UINT64 colmask, SqlTable *pTable)
	{
		UINT64 key = 1;
		for (UINT i = 0; i < m_IndexCount; ++i)
		{
			if ((key << m_IndexMap[i]->ColumnIndex()) & colmask)
				m_IndexMap[i]->OnUpdate(pTable, pRow);
		}
	}
	bool Query(const SqlCondition &d, SqlIDList &idList)
	{
		const SqlCondition::Node *p = d.GetRoot();
		InnerQuery(p, d, idList, OPT_NONE);
		return true;
	}
	const SqlRow *GetRowByID(UINT id)
	{
		if (m_IndexCount && m_IndexMap[0]->ColumnIndex() == 0)
			return m_IndexMap[0]->QueryByID(id);
		else
			return NULL;
	}
protected:
	void InnerQuery(const SqlCondition::Node *p, const SqlCondition &d, SqlIDList & idlist, BYTE opt)
	{
		if (p->Opt == OPT_NONE)
		{
			const SqlQuery *pq = d.GetQuery(p);
			ISqlIndexMap *pm =GetIndexFromColumn(pq->Column);
			pm->Query(pq, opt, idlist);
		}
		else
		{
			const SqlCondition::Node *pleft = d.GetLeft(p);
			const SqlCondition::Node *pright = d.GetLeft(p);
			if (opt == OPT_NONE)
			{
				InnerQuery(pleft, d, idlist, OPT_NONE);
				InnerQuery(pright, d, idlist, p->Opt);
			}
			else
			{	
				SqlIDList sublist;
				InnerQuery(pleft, d, sublist, OPT_NONE);
				InnerQuery(pright, d, sublist, p->Opt);
				if (opt == OPT_AND)
				{
					for (SqlIDList::iterator it = idlist.begin(); it != idlist.end();)
					{
						if (sublist.find(it->first) == sublist.end())
						{
							it = idlist.erase(it);
						}
						else
							++it;
					}
				}
				else
				{
					for (SqlIDList::iterator it = sublist.begin(); it != sublist.end(); ++it)
					{
						idlist[it->first] = 1;
					}
				}
			}
		}
	}
	ISqlIndexMap *GetIndexFromColumn(UINT column)const
	{
		for (UINT i = 0; i < m_IndexCount; ++i)
		{
			if (m_IndexMap[i]->ColumnIndex() == column)
				return m_IndexMap[i];
		}
		return NULL;
	}

protected:

	UINT			m_IndexCount;
	ISqlIndexMap	*m_IndexMap[TABLE_INDEX_MAX];
};