#pragma once
class SqlTable;
enum
{
	OPT_NONE,
	OPT_AND,
	OPT_OR
};
//查询条件
enum SqlQueryType
{
	QUERY_EQUAL,				//==
	QUERY_NOT_EQUAL,			//!=
	QUERY_LESS,					//<
	QUERY_LESS_EQUAL,			//<=
	QUERY_GRATER,				//>
	QUERY_GRATER_EQUAL,			//>=

	QUERY_BETWEEN,				// min <  X && X <  max
	QUERY_BETWEEN_LEFT,			// min <= X && X <  max
	QUERY_BETWEEN_RIGHT,		// min <  X && X <= max
	QUERY_BETWEEN_EQUAL,		// min <= X && X <= max

	QUERY_BETWEEN_INVERT,		// min >  X || X >  max
	QUERY_BETWEEN_INVERT_LEFT,	// min >= X || X >  max
	QUERY_BETWEEN_INVERT_RIGHT,	// min >  X || X >= max
	QUERY_BETWEEN_INVERT_EQUAL,	// min >= X || X >= max

	QUERY_STRING_LIKE,			//%string%
};
struct SqlUnion{
	SqlUnion(){}
	SqlUnion(BYTE tableName, UINT64 mask, BYTE mainColumn, BYTE unionColumn){
		UnionTable = tableName;
		ColumnMask = mask;
		MainColumn = mainColumn;
		UnionColumn = unionColumn;
	}
	UINT64	ColumnMask;		//列的集合
	BYTE	MainColumn;		//当前表的列

	BYTE	UnionTable;		//联合表
	BYTE	UnionColumn;	//联合表的列
};
struct SqlQuery{
	BYTE		Column;
	BYTE		QueryType;
	BYTE		Value[QUERY_VALUE_COUNT];	//可保存7个字符, 2个UINT64
};
class SqlCondition
{
public:
	enum { MAX_CONDITION_BYTES = 256 };
	struct Node
	{
		BYTE Opt;
		USHORT Left;
		USHORT Right;
	};
	struct Condition
	{
		UINT64		ColumnMask;
		USHORT		Offset;
		USHORT		RootOffset;
		SqlUnion	UnionData1;
		SqlUnion	UnionData2;
	};
	SqlCondition(){
		Reset();
	}
	SqlCondition(const SqlCondition& a){
		operator=(a);
	}
	SqlCondition(const SqlQuery &q);
	USHORT AddQuery(const SqlQuery &q);
	USHORT AddNode(BYTE opt);

	SqlCondition& operator=(const SqlCondition& a){
		memcpy(m_Data, a.m_Data, a.GetOffset());
		return *this;
	}
	const Condition* GetCondition()const{
		return (Condition*)m_Data;
	}
	const SqlQuery* GetQuery(USHORT offset)const{
		return (SqlQuery*)(GetStartAddr() + offset + sizeof(BYTE));
	}
	Node* GetNode(USHORT offset)const{
		return (Node*)(GetStartAddr() + offset);
	}
	
	USHORT GetRootOffset()const{
		return GetCondition()->RootOffset;
	}
	const Node* GetRoot()const{
		return (Node*)(GetStartAddr() + GetRootOffset());
	}
	const Node* GetLeft(const Node* p)const{
		return (Node*)(GetStartAddr() + p->Left);
	}
	const Node* GetRight(const Node* p)const{
		return (Node*)(GetStartAddr() + p->Right);
	}
	const SqlQuery* GetQuery(const Node* p)const{
		return (SqlQuery*)((BYTE*)p + 1);
	}
	void Reset()
	{
		memset(&GetCondition()->ColumnMask, 0xff, sizeof(GetCondition()->ColumnMask));
		memset(&GetCondition()->UnionData1, 0, sizeof(SqlUnion));
		memset(&GetCondition()->UnionData2, 0, sizeof(SqlUnion));
		GetCondition()->Offset = sizeof(Condition);
		GetCondition()->RootOffset = GetOffset();
	}
	void SetRoot(USHORT offset){
		GetCondition()->RootOffset = offset;
	}
	bool HasNode()const{
		return GetCondition()->Offset != 0;
	}
	UINT_PTR GetStartUintPtr()const{
		return (UINT_PTR)GetStartAddr();
	}
	USHORT GetOffset()const{
		return GetCondition()->Offset;
	}
	bool IsEmpty()const{
		return GetOffset() == sizeof(Condition);
	}
	void SetColumn(UINT column){
		GetCondition()->ColumnMask = column;
	}
	UINT64 GetColumnMask()const{
		return GetCondition()->ColumnMask;
	}
	void AddUnion(const SqlUnion& ud){
		if (GetCondition()->UnionData1.UnionTable == 0)
			GetCondition()->UnionData1 = ud;
		else if (GetCondition()->UnionData2.UnionTable == 0)
			GetCondition()->UnionData2 = ud;
		else
			Log("联合查询最多两个表.");
	}
	const SqlUnion& GetUnion1()const{
		return GetCondition()->UnionData1;
	}
	const SqlUnion& GetUnion2()const{
		return GetCondition()->UnionData2;
	}
protected:
	Condition* GetCondition(){
		return (Condition*)m_Data;
	}
	BYTE *GetCurrentAddr(){
		return GetStartAddr() + GetOffset();
	}
	BYTE *GetStartAddr(){
		return m_Data;
	}
	const BYTE *GetStartAddr()const{
		return m_Data;
	}
	BYTE	m_Data[MAX_CONDITION_BYTES];
};

//索引map接口
class ISqlIndexMap{
public:
	virtual bool OnInsert(const SqlTable *pTable, const SqlRow *pRow) = 0;
	virtual void OnUpdate(const SqlTable *pTable, const SqlRow *pRow) = 0;
	virtual void OnDelete(const SqlTable *pTable, const SqlRow *pRow) = 0;
	virtual UINT Query(const SqlQuery *baseKey, BYTE opt, SqlIDList &idlist)const = 0;
	virtual void Clear() = 0;
	virtual BYTE ColumnIndex()const = 0;
	virtual SqlRow* QueryByID(UINT id)const = 0;
};

//基本类型查询
template<typename T>
class SqlIndexMap: public ISqlIndexMap
{
public:
	typedef multimap<T, UINT_PTR> IndexMap;
	typedef typename IndexMap::iterator IndexMapIt;
	typedef map<UINT_PTR, IndexMapIt>	IndexEntryMap;
	typedef typename IndexEntryMap::iterator EntryMapIt;


public:
	SqlIndexMap(const SqlColumn *pColumn)
	{
		m_pColumn = pColumn;
	}
	virtual ~SqlIndexMap(){}
	virtual bool OnInsert(const SqlTable *pTable, const SqlRow *pRow){
		const T *pData =(const T*)pTable->GetField(pRow, m_pColumn->Index);
		T key = *pData;
		IndexMapIt insertIt = m_Map.insert(make_pair(key, (UINT_PTR)pRow));
		if (m_EntryMap.insert(make_pair((UINT_PTR)pRow, insertIt)).second)
			return true;
		else
		{
			m_Map.erase(insertIt);
			Log("索引行入口插入失败.");
			return false;
		}
		
	}
	virtual void OnUpdate(const SqlTable *pTable, const SqlRow *pRow){
		const T *pData = (const T*)pTable->GetField(pRow, m_pColumn->Index);
		T key = *pData;
		EntryMapIt entryIt = m_EntryMap.find((UINT_PTR)pData);
		if (entryIt == m_EntryMap.end())
		{
			Log("不正确的索引行");
			return;
		}
		IndexMapIt indexIt = entryIt->second;
		if (key == indexIt->first)
			return;

		m_Map.erase(indexIt);
		IndexMapIt insertIt = m_Map.insert(make_pair(key, (UINT_PTR)pRow));
		entryIt->second = insertIt;

	}
	virtual void OnDelete(const SqlTable *pTable, const SqlRow *pRow){
		UNREFERENCED_PARAMETER(pTable);
		EntryMapIt entryIt = m_EntryMap.find((UINT_PTR)pRow);
		if (entryIt == m_EntryMap.end())
		{
			Log("删除索引行失败");
			return;
		}
		m_Map.erase(entryIt->second);
		m_EntryMap.erase(entryIt);
	}
	virtual SqlRow* QueryByID(UINT id)const
	{
		IndexMap::const_iterator it = m_Map.find(static_cast<T>(id));
		if (it != m_Map.end())
			return (SqlRow*)it->second;
		else
			return NULL;
	}
	virtual UINT Query(const SqlQuery *key, BYTE opt, SqlIDList &idlist)const
	{
		T v1 = *(T*)key->Value;
		T v2 = *(T*)&key->Value[8];
		IndexMap::const_iterator it, itstart, itend;
		switch (key->QueryType)
		{
		case QUERY_EQUAL:
			it		= m_Map.lower_bound(v1);
			itend	= m_Map.upper_bound(v1);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; it != itend; ++it)
					idlist[it->second] = 1;
			}
			else if (opt == OPT_AND)
			{
				SqlIDList f;
				for (; it != itend; ++it)
					f[it->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_NOT_EQUAL:
			itstart = m_Map.begin();
			it		= m_Map.lower_bound(v1);
			itend	= m_Map.upper_bound(v1);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; itstart != it; ++itstart)
					idlist[itstart->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					idlist[itend->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (; itstart != it; ++itstart)
					f[itstart->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					f[itend->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_LESS:
			itstart = m_Map.begin();
			it = m_Map.lower_bound(v1);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; itstart != it; ++itstart)
					idlist[itstart->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (; itstart != it; ++itstart)
					f[itstart->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_LESS_EQUAL:
			itstart = m_Map.begin();
			itend = m_Map.upper_bound(v1);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; itstart != itend; ++itstart)
					idlist[itstart->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (; itstart != itend; ++itstart)
					f[itstart->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_GRATER:
			itend = m_Map.upper_bound(v1);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; itend != m_Map.end(); ++itend)
					idlist[itend->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (; itend != m_Map.end(); ++itend)
					f[itend->second] =1;
				Combine(f, idlist);
			}
			break;
		case QUERY_GRATER_EQUAL:
			it = m_Map.lower_bound(v1);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; it != m_Map.end(); ++it)
					idlist[it->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (; it != m_Map.end(); ++it)
					f[it->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_BETWEEN:
			itstart = m_Map.upper_bound(v1);
			itend	= m_Map.lower_bound(v2);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; itstart != itend; ++itstart)
					idlist[itstart->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (; itstart != itend; ++itstart)
					f[itstart->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_BETWEEN_EQUAL:
			itstart = m_Map.lower_bound(v1);
			itend	= m_Map.upper_bound(v2);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; itstart != itend; ++itstart)
					idlist[itstart->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (; itstart != itend; ++itstart)
					f[itstart->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_BETWEEN_INVERT:
			itstart = m_Map.lower_bound(v1);
			itend = m_Map.upper_bound(v2);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (it = m_Map.begin(); it != itstart; ++it)
					idlist[it->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					idlist[itend->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (it = m_Map.begin(); it != itstart; ++it)
					f[it->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					f[itend->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_BETWEEN_INVERT_EQUAL:
			itstart = m_Map.upper_bound(v1);
			itend = m_Map.lower_bound(v2);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (it = m_Map.begin(); it != itstart; ++it)
					idlist[it->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					idlist[itend->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (it = m_Map.begin(); it != itstart; ++it)
					f[it->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					f[itend->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_BETWEEN_LEFT:
			itstart = m_Map.lower_bound(v1);
			itend = m_Map.lower_bound(v2);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; itstart != itend; ++itstart)
					idlist[itstart->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (; itstart != itend; ++itstart)
					f[itstart->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_BETWEEN_RIGHT:
			itstart = m_Map.upper_bound(v1);
			itend = m_Map.upper_bound(v2);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (; itstart != itend; ++itstart)
					idlist[itstart->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (; itstart != itend; ++itstart)
					f[itstart->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_BETWEEN_INVERT_LEFT:
			itstart = m_Map.upper_bound(v1);
			itend = m_Map.upper_bound(v2);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (it = m_Map.begin(); it != itstart; ++it)
					idlist[it->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					idlist[itend->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (it = m_Map.begin(); it != itstart; ++it)
					f[it->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					f[itend->second] = 1;
				Combine(f, idlist);
			}
			break;
		case QUERY_BETWEEN_INVERT_RIGHT:
			itstart = m_Map.lower_bound(v1);
			itend = m_Map.lower_bound(v2);
			if (opt == OPT_NONE || opt == OPT_OR)
			{
				for (it = m_Map.begin(); it != itstart; ++it)
					idlist[it->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					idlist[itend->second] = 1;
			}
			else
			{
				SqlIDList f;
				for (it = m_Map.begin(); it != itstart; ++it)
					f[it->second] = 1;
				for (; itend != m_Map.end(); ++itend)
					f[itend->second] = 1;
				Combine(f, idlist);
			}
			break;
		}
		return idlist.size();
	}
	virtual void Clear()
	{
		m_Map.clear();
		m_EntryMap.clear();
		m_pColumn = NULL;
	}
	virtual BYTE ColumnIndex()const
	{
		return m_pColumn->Index;
	}
protected:
	void Combine(const SqlIDList &f, SqlIDList &b)const
	{
		if (f.size() == 0)
		{
			b.clear();
		}
		else
		{
			for (SqlIDList::iterator it = b.begin(); it != b.end();)
			{
				if (f.find(it->first) == f.end())
					it = b.erase(it);
				else
					++it;
			}
		}
	}
	IndexMap		m_Map;
	IndexEntryMap	m_EntryMap;
	const SqlColumn *m_pColumn;
};


class SqlStringIndexMap: public ISqlIndexMap
{
public:
	typedef HashMap<UINT_PTR, const WCHAR*> HashStrMap;
	SqlStringIndexMap(const SqlColumn *pc)
	{
		m_pColumn = pc;
	}
	virtual ~SqlStringIndexMap(){}
	virtual bool OnInsert(const SqlTable *pTable, const SqlRow *pRow);
	virtual void OnUpdate(const SqlTable *pTable, const SqlRow *pRow)
	{
		UNREFERENCED_PARAMETER(pRow);
		UNREFERENCED_PARAMETER(pTable);
	}
	virtual void OnDelete(const SqlTable *pTable, const SqlRow *pRow)
	{
		UNREFERENCED_PARAMETER(pTable);
		m_StrMap.erase((UINT_PTR)pRow);
	}
	virtual UINT Query(const SqlQuery *baseKey, BYTE opt, SqlIDList &idlist)const
	{
		const WCHAR *pc = (const WCHAR*)baseKey->Value;
		if (opt == OPT_AND)
		{
			SqlIDList f;
			for (HashStrMap::const_iterator it = m_StrMap.begin(); it != m_StrMap.end(); ++it)
			{
				const WCHAR *pfind = it->second;
				if (wcsstr(pfind, pc) != NULL)
				{
					f[it->first] = 1;
				}
			}
			if (f.size() > 0)
			{
				for (SqlIDList::iterator it = idlist.begin(); it != idlist.end();)
				{
					if (f.find(it->first) == f.end())
					{
						it = idlist.erase(it);
					}
					else ++it;
				}
			}
			else
			{
				idlist.clear();
			}
		}
		else
		{
			for (HashStrMap::const_iterator it = m_StrMap.begin(); it != m_StrMap.end(); ++it)
			{
				const WCHAR *pfind = it->second;
				if (wcsstr(pfind, pc) != NULL)
				{
					idlist[it->first] = 1;
				}
			}
		}
		return idlist.size();
	}
	virtual void Clear()
	{
		m_pColumn = NULL;
		m_StrMap.clear();
	}
	virtual BYTE ColumnIndex()const
	{
		return m_pColumn->Index;
	}
	virtual SqlRow* QueryByID(UINT id)const
	{
		UNREFERENCED_PARAMETER(id);
		return NULL;
	}
protected:
	HashStrMap			m_StrMap;
	const SqlColumn		*m_pColumn;
};