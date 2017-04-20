#pragma once

template<typename T>
struct SafeListNode
{
	SafeListNode()
	{
		Next = NULL;
	}
	T                Item;
	SafeListNode<T>  *Next;
};

template<typename T>
class SafeList
{
public:
	int Count()
	{
		return m_Count;
	}
	SafeList()
	{
		m_Last = m_Current = new SafeListNode<T>;
		m_Count = 0;
	}
	bool HasItem()
	{
		return m_Current->Next != NULL;
	}
	T GetItem()
	{
		if (m_Current->Next == NULL)
			return NULL;

		SafeListNode<T> *pItem = m_Current;
		m_Current = pItem->Next;
		T item = m_Current->Item;
		delete(pItem);

		--m_Count;
		return item;
	}
	void AddItem(T item)
	{
		SafeListNode<T> *pNode = new SafeListNode<T>;
		pNode->Item = item;
		m_Last->Next = pNode;
		m_Last = pNode;
		++m_Count;
	}

protected:
	SafeListNode<T> *m_Current;
	SafeListNode<T> *m_Last;
	volatile int m_Count;
};
