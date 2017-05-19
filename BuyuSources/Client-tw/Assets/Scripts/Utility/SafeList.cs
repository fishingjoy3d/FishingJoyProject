using System;
using System.Collections.Generic;

public class SafeListNode<T>
{
    public T                Item;
    volatile public SafeListNode<T> Next;
}

public class SafeList<T>
{
    volatile SafeListNode<T> m_Current;
    volatile SafeListNode<T> m_Last;
    public SafeList()
    {
        m_Last = m_Current = new SafeListNode<T>();
    }
    public bool HasItem()
    {
        return m_Current.Next != null;
    }
    public T GetItem()
    {
        m_Current = m_Current.Next;
        T item = m_Current.Item;
        m_Current.Item = default(T);
        return item;
    }
    public void AddItem(T item)
    {
        SafeListNode<T> node = new SafeListNode<T>();
        node.Item = item;
        m_Last.Next = node;
        m_Last = node;
    }
    public void Clear()
    {
        while (HasItem())
            GetItem();
    }
}
