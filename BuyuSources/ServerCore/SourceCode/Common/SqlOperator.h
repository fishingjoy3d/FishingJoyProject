#pragma once
#define COPY_VALUE_LEFT(X, Y) memcpy(X->Value, Y->Value, 8)
#define COPY_VALUE_RIGHT(X, Y) memcpy(X->Value + 8, Y->Value, 8)
#define LIKE %
#define DECL_OPERATOR(TYPE,QUERY) \
SqlQuery q;	\
q.Column = static_cast<BYTE>(a.ColumnIndex);\
q.QueryType = QUERY; \
*(TYPE*)q.Value = b; \
return SqlCondition(q);

struct SqlQueryColumn{
	UINT ColumnIndex;
	SqlQueryColumn(UINT i){
		ColumnIndex = i;
	}
	SqlCondition operator%(const WCHAR *pc)const{
		SqlQuery q;
		q.Column = static_cast<BYTE>(ColumnIndex);
		q.QueryType = QUERY_STRING_LIKE;
		wcscpy_s((WCHAR*)q.Value, QUERY_VARCHAR_COUNT, pc);
		return SqlCondition(q);
	}
};
extern USHORT	CopyCondition(SqlCondition *a, const SqlCondition *b, const SqlCondition::Node *node);
extern bool		TryConbineCondition_And(const SqlCondition &a, const SqlCondition &b);
extern bool		TryConbineCondition_Or(const SqlCondition &a, const SqlCondition &b);
extern SqlCondition operator&&(const SqlCondition &a, const SqlCondition &b);
extern SqlCondition operator||(SqlCondition a, SqlCondition b);

#define DECL_OPERATOR_LIST(TYPE) \
	inline SqlCondition operator<(SqlQueryColumn a, TYPE b){\
DECL_OPERATOR(TYPE, QUERY_LESS);\
}\
inline SqlCondition  operator>(SqlQueryColumn a, TYPE b){\
	DECL_OPERATOR(TYPE, QUERY_GRATER);\
}\
inline SqlCondition operator<=(SqlQueryColumn a, TYPE b){\
	DECL_OPERATOR(TYPE, QUERY_LESS_EQUAL);\
}\
inline SqlCondition  operator>=(SqlQueryColumn a, TYPE b){\
	DECL_OPERATOR(TYPE, QUERY_GRATER_EQUAL);\
}\
inline SqlCondition  operator==(SqlQueryColumn a, TYPE b){\
	DECL_OPERATOR(TYPE, QUERY_EQUAL);\
}\
inline SqlCondition  operator!=(SqlQueryColumn a, TYPE b){\
	DECL_OPERATOR(TYPE, QUERY_NOT_EQUAL);\
}\
inline SqlCondition  operator<(TYPE b, SqlQueryColumn a){\
	return operator>(a, b);\
}\
inline SqlCondition  operator>(TYPE b, SqlQueryColumn a){\
	return operator<(a, b);\
}\
inline SqlCondition  operator<=(TYPE b, SqlQueryColumn a){\
	return operator>=(a, b);\
}\
inline SqlCondition  operator>=(TYPE b, SqlQueryColumn a){\
	return operator<=(a, b);\
}\
inline SqlCondition  operator==(TYPE b, SqlQueryColumn a){\
	return operator==(a, b);\
}\
inline SqlCondition  operator!=(TYPE b, SqlQueryColumn a){\
	return operator!=(a, b);\
}

DECL_OPERATOR_LIST(CHAR);
DECL_OPERATOR_LIST(BYTE);
DECL_OPERATOR_LIST(SHORT);
DECL_OPERATOR_LIST(USHORT);
DECL_OPERATOR_LIST(INT);
DECL_OPERATOR_LIST(UINT);
DECL_OPERATOR_LIST(INT64);
DECL_OPERATOR_LIST(UINT64);
DECL_OPERATOR_LIST(float);

