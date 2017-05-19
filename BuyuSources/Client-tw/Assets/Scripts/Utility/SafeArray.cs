using System;

class SafeArray<T>
{
	uint m_ArraySize;
	uint m_ArraySizeInv;
    uint _temp0;
	volatile T[] m_pData;
	uint _temp1;
	volatile uint m_PushIdx;
	uint _temp2;
	volatile uint m_PopIdx;
	uint _temp3;

	public SafeArray(uint nSize)
	{
		uint k = nSize & (nSize - 1);
		if (k != 0)
		{
			k = 0;
			while (nSize > 0)
			{
				nSize >>= 1;
				++k;
			}
			nSize = (uint)(1 << (int)k);
		}
        if(nSize < 8)
		    nSize = 8;

		m_ArraySize     = nSize;
		m_ArraySizeInv  = nSize - 1;
		m_pData         = new T[nSize];
		m_PushIdx       = 0;
		m_PopIdx        = 0;
	}
	~SafeArray()
	{
		m_pData = null;
	}
	public bool HasItem()
	{
		return m_PushIdx != m_PopIdx;
	}
	public T GetItem()
	{
		uint idx = m_PopIdx;
		m_PopIdx = (m_PopIdx + 1) & m_ArraySizeInv;
		return m_pData[idx];
	}
	public T GetItemNoRemove()
	{
		return m_pData[m_PopIdx];
	}
	public bool HasSpace()
	{
		return ((m_PushIdx + 1) & m_ArraySizeInv) != m_PopIdx;
	}
	public void AddItem(T t)
	{
		m_pData[m_PushIdx] = t;
		m_PushIdx = (m_PushIdx + 1) & m_ArraySizeInv;
	}
	public uint Count()
	{
		int d = (int)(m_PushIdx - m_PopIdx);
		return (uint)((d < 0) ? (m_ArraySize + d) : d);
	}
    public void Clear()
    {
        m_pData         = new T[m_ArraySize];
        m_PushIdx       = 0;
        m_PopIdx        = 0;
    }
	public uint ArraySize()
	{
		return m_ArraySize;
	}


};