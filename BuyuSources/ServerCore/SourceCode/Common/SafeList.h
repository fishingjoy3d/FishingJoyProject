#pragma once

template<typename T>
struct SafeListNode
{
	SafeListNode()
	{
		Next = NULL;
	}
	T	Item;
	volatile SafeListNode<T>  *Next;
};

template<typename T>
class SafeList
{
public:
	typedef SafeListNode<T> Node;
	int Count()
	{
		return m_Count;
	}
	SafeList()
	{
		m_Last = m_Current = new Node;
		m_Count = 0;
	}
	~SafeList()
	{
		while (m_Current != m_Last)
		{
			volatile Node *pItem = m_Current;
			m_Current = m_Current->Next;
			delete(pItem);
		}
		delete m_Last;

	}
	bool HasItem()
	{
		return m_Current->Next != NULL;
	}
	T GetItemNoRemove()
	{
		if (m_Current->Next == NULL)
			return NULL;
		return m_Current->Next->Item;
	}
	T GetItem()
	{
		if (m_Current->Next == NULL)
			return NULL;

		volatile Node *pItem = m_Current;
		m_Current = pItem->Next;
		T item = ((Node*)m_Current)->Item;
		delete(pItem);
		::InterlockedDecrement(&m_Count);
		return item;
	}
	void AddItem(T item)
	{
		Node *pNode = new Node;
		pNode->Item = item;
		m_Last->Next = pNode;
		m_Last = pNode;
		::InterlockedIncrement(&m_Count);
	}

protected:
	CACHELINE(1);
	volatile Node *m_Current;
	CACHELINE(2);
	volatile Node *m_Last;
	CACHELINE(3);
	volatile long m_Count;
	CACHELINE(4);
};
