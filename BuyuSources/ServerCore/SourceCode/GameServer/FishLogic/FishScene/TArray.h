//#pragma once
//#if _MSC_VER < 1600	// VS2010
//#include <hash_map>
//template<typename K, typename T>
//class HashMap : public stdext::hash_map<K, T>
//{
//};
//#else
//#include <unordered_map>
//template<typename K, typename T>
//class HashMap : public unordered_map<K, T>
//{
//};
//#endif
//
//template<typename T, int ItemCount>
//class TArrayV
//{
//public:
//
//	TArrayV()
//	{
//		Count = 0;
//		Index = 0;
//		memset(DataPtr, 0, sizeof(DataPtr));
//	}
//	
//	T	DataPtr[ItemCount];
//	int Count;
//	int Index;
//	int ItemSize()const
//	{
//		return ItemCount;
//	}
//	void Add(const T &t)
//	{
//		DataPtr[Index] = t;
//		++Count;
//		if (++Index == ItemCount)
//			Index = 0;
//	}
//	void RemoveSwapLast(int idx)
//	{
//		if (idx != Count - 1)
//		{
//			DataPtr[idx] = DataPtr[Count - 1];
//			DataPtr[Count - 1] = NULL;
//		}
//		--Count;
//	}
//	void RemoveSwapLastDelete(int idx)
//	{
//		if (idx != Count - 1)
//		{
//			T temp = DataPtr[idx];
//			DataPtr[idx] = DataPtr[Count - 1];
//			DataPtr[Count - 1] = temp;
//		}
//		delete(DataPtr[Count - 1]);
//		DataPtr[Count - 1] = NULL;
//		--Count;
//	}
//	bool Find(int idx)
//	{
//		return DataPtr[idx] != NULL;
//	}
//	void Insert(int idx, const T &t)
//	{
//		DataPtr[idx] = t;
//		++Count;
//	}
//	void ClearAndDelete()
//	{
//		for (int i = 0; i < ItemCount; ++i)
//			SAFE_DELETE(DataPtr[i]);
//		Count = 0;
//		Index = 0;
//	}
//	void Clear()
//	{
//		Count = 0;
//		Index = 0;
//	}
//	void Remove(int idx)
//	{
//		if (DataPtr[idx] != NULL)
//		{
//			DataPtr[idx] = NULL;
//			--Count;
//		}
//		else
//		{
//			InnerFishLog(L"不存在的数组索引ID:%d", idx);
//		}
//	}
//	bool RemoveAndDelete(int idx)
//	{
//		if (DataPtr[idx] != NULL)
//		{
//			delete(DataPtr[idx]);
//			DataPtr[idx] = NULL;
//			--Count;
//			return true;
//		}
//		else
//			return false;
//	}
//};