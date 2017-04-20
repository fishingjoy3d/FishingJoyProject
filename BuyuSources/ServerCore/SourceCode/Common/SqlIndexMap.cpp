#include "stdafx.h"
SqlCondition::SqlCondition(const SqlQuery &q)
{
	Reset();
	AddQuery(q);
}
USHORT SqlCondition::AddQuery(const SqlQuery &q){
	Condition *pdata = GetCondition();
	USHORT offset = pdata->Offset;
	BYTE *p = GetCurrentAddr();
	*p++ = OPT_NONE;
	memcpy(p, &q, sizeof(SqlQuery));
	pdata->Offset += sizeof(BYTE) + sizeof(SqlQuery);
	return offset;
}
USHORT SqlCondition::AddNode(BYTE opt){
	Condition *pdata = GetCondition();
	USHORT offset = pdata->Offset;
	BYTE *p = GetCurrentAddr();
	*p = opt;
	pdata->Offset += sizeof(Node);
	return offset;
}
bool SqlStringIndexMap::OnInsert(const SqlTable *pTable, const SqlRow *pRow)
{
	m_StrMap[(UINT_PTR)pRow] = (const WCHAR*)pTable->GetField(pRow, m_pColumn->Index);
	return true;
}
	