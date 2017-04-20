#include "stdafx.h"

USHORT CopyCondition(SqlCondition *a, const SqlCondition *b, const SqlCondition::Node *node)
{
	if (node->Opt == OPT_NONE)
		return a->AddQuery(*b->GetQuery(node));

	USHORT nodeOffset = a->AddNode(node->Opt);
	SqlCondition::Node *pnew = a->GetNode(nodeOffset);
	pnew->Left = CopyCondition(a, b, b->GetLeft(node));
	pnew->Right = CopyCondition(a, b, b->GetRight(node));
	return nodeOffset;
}
bool TryConbineCondition_And(const SqlCondition &a, const SqlCondition &b)
{
	if (a.GetRoot()->Opt != OPT_NONE || b.GetRoot()->Opt != OPT_NONE)
		return false;
	SqlQuery *pa = (SqlQuery*)a.GetQuery(a.GetRoot());
	SqlQuery *pb = (SqlQuery*)b.GetQuery(b.GetRoot());

	if (pa->Column != pb->Column)
		return false;
	// a < column && column < b
	if ((pa->QueryType == QUERY_GRATER && pb->QueryType == QUERY_LESS) || (pb->QueryType == QUERY_GRATER && pa->QueryType == QUERY_LESS))
	{
		if (pa->QueryType != QUERY_GRATER)
			COPY_VALUE_RIGHT(pa, pa);
		COPY_VALUE_LEFT(pa, pb);
		pa->QueryType = QUERY_BETWEEN;
		return true;
	}
	// a <= column && column < b
	if ((pa->QueryType == QUERY_GRATER_EQUAL && pb->QueryType == QUERY_LESS) || (pb->QueryType == QUERY_GRATER_EQUAL && pa->QueryType == QUERY_LESS))
	{
		if (pa->QueryType != QUERY_GRATER_EQUAL)
			COPY_VALUE_RIGHT(pa, pa);
		COPY_VALUE_LEFT(pa, pb);
		pa->QueryType = QUERY_BETWEEN_LEFT;
		return true;
	}
	// a < column && column <= b
	if ((pa->QueryType == QUERY_GRATER && pb->QueryType == QUERY_LESS_EQUAL) || (pb->QueryType == QUERY_GRATER && pa->QueryType == QUERY_LESS_EQUAL))
	{
		if (pa->QueryType != QUERY_GRATER)
			COPY_VALUE_RIGHT(pa, pa);
		COPY_VALUE_LEFT(pa, pb);
		pa->QueryType = QUERY_BETWEEN_RIGHT;
		return true;
	}
	// a <= column && column <= b
	if ((pa->QueryType == QUERY_GRATER_EQUAL && pb->QueryType == QUERY_LESS_EQUAL) || (pb->QueryType == QUERY_GRATER_EQUAL && pa->QueryType == QUERY_LESS_EQUAL))
	{
		if (pa->QueryType != QUERY_GRATER_EQUAL)
			COPY_VALUE_RIGHT(pa, pa);
		COPY_VALUE_LEFT(pa, pb);
		pa->QueryType = QUERY_BETWEEN_EQUAL;
		return true;
	}
	return false;
}
bool TryConbineCondition_Or(const SqlCondition &a, const SqlCondition &b)
{
	if (a.GetRoot()->Opt != OPT_NONE || b.GetRoot()->Opt != OPT_NONE)
		return false;
	SqlQuery *pa = (SqlQuery*)a.GetQuery(a.GetRoot());
	SqlQuery *pb = (SqlQuery*)b.GetQuery(b.GetRoot());

	if (pa->Column != pb->Column)
		return false;
	// a > column || column > b
	if ((pa->QueryType == QUERY_LESS && pb->QueryType == QUERY_GRATER) || (pb->QueryType == QUERY_LESS && pa->QueryType == QUERY_GRATER))
	{
		if (pa->QueryType != QUERY_LESS)
			COPY_VALUE_RIGHT(pa, pa);
		COPY_VALUE_LEFT(pa, pb);
		pa->QueryType = QUERY_BETWEEN_INVERT;
		return true;
	}
	// a >= column || column > b
	if ((pa->QueryType == QUERY_LESS_EQUAL && pb->QueryType == QUERY_GRATER) || (pb->QueryType == QUERY_LESS_EQUAL && pa->QueryType == QUERY_GRATER))
	{
		if (pa->QueryType != QUERY_LESS_EQUAL)
			COPY_VALUE_RIGHT(pa, pa);
		COPY_VALUE_LEFT(pa, pb);
		pa->QueryType = QUERY_BETWEEN_INVERT_LEFT;
		return true;
	}
	// a > column || column >= b
	if ((pa->QueryType == QUERY_LESS && pb->QueryType == QUERY_GRATER_EQUAL) || (pb->QueryType == QUERY_LESS && pa->QueryType == QUERY_GRATER_EQUAL))
	{
		if (pa->QueryType != QUERY_LESS)
			COPY_VALUE_RIGHT(pa, pa);
		COPY_VALUE_LEFT(pa, pb);
		pa->QueryType = QUERY_BETWEEN_INVERT_RIGHT;
		return true;
	}
	// a <= column && column <= b
	if ((pa->QueryType == QUERY_LESS_EQUAL && pb->QueryType == QUERY_GRATER_EQUAL) || (pb->QueryType == QUERY_LESS_EQUAL && pa->QueryType == QUERY_GRATER_EQUAL))
	{
		if (pa->QueryType != QUERY_LESS_EQUAL)
			COPY_VALUE_RIGHT(pa, pa);
		COPY_VALUE_LEFT(pa, pb);
		pa->QueryType = QUERY_BETWEEN_INVERT_EQUAL;
		return true;
	}
	return false;
}

SqlCondition operator&&(const SqlCondition &a, const SqlCondition &b)
{
	if (!a.HasNode() || !b.HasNode())
	{
		Log("未设置条件");
		return SqlCondition();
	}
	if (TryConbineCondition_And(a, b))
		return a;

	USHORT leftOffset, rightOffset;
	SqlCondition *pa = (SqlCondition*)&a;
	leftOffset = pa->GetRootOffset();
	USHORT rootOffset = pa->AddNode(OPT_AND);
	SqlCondition::Node *pRoot = pa->GetNode(rootOffset);

	rightOffset = CopyCondition(pa, &b, b.GetRoot());
	pRoot->Left = leftOffset;
	pRoot->Right = rightOffset;
	pa->SetRoot(rootOffset);
	return *pa;
}
SqlCondition operator||(SqlCondition a, SqlCondition b)
{
	if (!a.HasNode() || !b.HasNode())
	{
		Log("未设置条件");
		return SqlCondition();
	}
	if (TryConbineCondition_Or(a, b))
		return a;
	USHORT leftOffset, rightOffset;
	SqlCondition *pa = (SqlCondition*)&a;
	leftOffset = pa->GetRootOffset();
	USHORT rootOffset = pa->AddNode(OPT_OR);
	SqlCondition::Node *pRoot = pa->GetNode(rootOffset);

	rightOffset = CopyCondition(pa, &b, b.GetRoot());
	pRoot->Left = leftOffset;
	pRoot->Right = rightOffset;
	pa->SetRoot(rootOffset);

	return *pa;
}