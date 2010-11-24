// Free open-source Grom library

#include "stdafx.h"
#ifndef __Grom_System_Array_h__
#define __Grom_System_Array_h__

#include "Config.h"

#include "Object.h"

namespace Sys
{

int STDCALL ComparePointers(void* p1, void* p2, void* userData);

// The PointerArray class is an implementation for type-safe TArray<T> class.
// Main purpose is to avoid code bloating during template instantiation.
// It provides (unlike std::vector<void*>):
//   - automatic memory initialization upon array resize;
//   - mandatory range checking even in release builds;
//   - function pointer as comparer instead of class with operator() overloaded;
//   - .NET-like API for easy migration to the .NET Framework.

// PointerArray

class PointerArray
{
public:
  typedef int STDCALL CompareProc(void* item1, void* item2, void* userData);

  PointerArray();
  ~PointerArray();

  // Capacity/Count
  int Capacity() const;
  void SetCapacity(int capacity);
  int Count() const;
  void SetCount(int count);

  // Get/Set
  void* Item(int index) const;
  void* operator[](int index) const;
  void SetItem(int index, void* item);
  void SetRange(int index, void** items, int count); 

  // Add/Remove/Move/Clear
  void Add(void* item);
  void AddRange(void** items, int count);
  void Insert(int index, void* item);
  void InsertRange(int index, void** items, int count);
  bool Remove(void* item);
  void* RemoveAt(int index);
  void RemoveRange(int index, int count); 
  void Move(int sourceIndex, int targetIndex);
  void MoveRange(int sourceIndex, int count, int targetIndex);
  void Clear();

  // Copy
  void CopyFrom(const PointerArray& sourceArray);
  void CopyFrom(const PointerArray& sourceArray, int sourceIndex);
  void CopyFrom(const PointerArray& sourceArray, int sourceIndex, int targetIndex, int count);

  // Linear search
  int IndexOf(void* item) const;
  int IndexOf(void* item, int startIndex) const;
  int IndexOf(void* item, int startIndex, int count) const;
  int LastIndexOf(void* item) const;
  int LastIndexOf(void* item, int startIndex) const;
  int LastIndexOf(void* item, int startIndex, int count) const;
  bool Contains(void* item) const;

  // Binary search
  int BinarySearch(void* item) const;
  int BinarySearch(void* item, CompareProc* comparer, void* userData) const;
  int BinarySearch(void* item, int startIndex, int count) const;
  int BinarySearch(void* item, int startIndex, int count, CompareProc* comparer, void* userData) const;

  // Auxiliary
  void Reverse();
  void Reverse(int startIndex, int count);
  void Sort();
  void Sort(CompareProc* comparer, void* userData);
  void Sort(int startIndex, int count, CompareProc* comparer, void* userData);
  void TrimToSize();

private:
  PointerArray(const PointerArray&); // forbidden
  PointerArray& operator=(const PointerArray&); // forbidden

  void Grow();
  int ForwardSearch(void* item, int index, int count) const;
  int BackwardSearch(void* item, int index, int count) const;
  void DoSort(int fromIndex, int toIndex, CompareProc* comparer, void* userData);

  void** m_Data;
  int m_Capacity;
  int m_Count;
};

// PointerArray

inline
int PointerArray::Capacity() const
{
  return m_Capacity;
}

inline
int PointerArray::Count() const
{
  return m_Count;
}

inline
void PointerArray::Clear()
{
  SetCount(0);
}

inline
void PointerArray::CopyFrom(const PointerArray& sourceArray)
{
  CopyFrom(sourceArray, 0, 0, sourceArray.m_Count);
}

inline
void PointerArray::CopyFrom(const PointerArray& sourceArray, int sourceIndex)
{
  CopyFrom(sourceArray, sourceIndex, 0, sourceArray.m_Count);
}

inline
int PointerArray::IndexOf(void* item) const
{
  return IndexOf(item, 0, m_Count);
}

inline
int PointerArray::IndexOf(void* item, int startIndex) const
{
  return IndexOf(item, startIndex, m_Count - startIndex);
}

inline
int PointerArray::LastIndexOf(void* item) const
{
  return LastIndexOf(item, 0, m_Count);
}

inline
int PointerArray::LastIndexOf(void* item, int startIndex) const
{
  return LastIndexOf(item, startIndex, m_Count - startIndex);
}

inline
bool PointerArray::Contains(void* item) const
{
  return IndexOf(item) != -1;
}

inline
int PointerArray::BinarySearch(void* item) const
{
  return BinarySearch(item, ComparePointers, (void*)NULL);
}

inline
int PointerArray::BinarySearch(void* item, CompareProc* comparer, void* userData) const
{
  if (m_Count > 0)
    return BinarySearch(item, (int)0, m_Count, comparer, userData);
  else
    return ~0;
}

inline
int PointerArray::BinarySearch(void* item, int startIndex, int count) const
{
  return BinarySearch(item, startIndex, count, ComparePointers, (void*)NULL);
}

inline
void PointerArray::Reverse()
{
  Reverse(0, m_Count);
}

inline
void PointerArray::Sort()
{
  Sort(ComparePointers, (void*)NULL);
}

inline
void PointerArray::Sort(CompareProc* comparer, void* userData)
{
  Sort(0, m_Count, comparer, userData);
}

inline
void PointerArray::TrimToSize()
{
  SetCapacity(m_Count);
}

// The TArray<T> class is just type-safe wrapper over the PointerArray class.
// It provides (unlike std::vector<T*>):
//   - single implementation for all reference-type objects
//     (unlike std::vector there is no code bloating);
//   - automatic memory initialization upon array resize;
//   - mandatory range checking even in release builds;
//   - function pointer as comparer instead of class with operator() overloaded;
//   - .NET-like API for easy migration to the .NET Framework.

// TArray

template <class T>
class TArray
{
public:
  typedef int STDCALL CompareProc(T* item1, T* item2, void* userData);

  TArray();
  TArray(const TArray<T>& sourceArray);
  TArray<T>& operator=(const TArray<T>& sourceArray);
  ~TArray();

  // Capacity/Count
  int Capacity();
  void SetCapacity(int capacity);
  int Count();
  void SetCount(int count);

  // Get/Set
  T* Item(int index);
  void SetItem(int index, T* item);
  //TArray<T>* Range(int index, int count) const;
  void SetRange(int index, T** items, int count); 

  // Add/Remove/Clear
  void Add(T* item);
  void AddRange(T** items, int count);
  void Insert(int index, T* item);
  void InsertRange(int index, T** items, int count);
  bool Remove(T* item);
  T* RemoveAt(int index);
  void RemoveRange(int index, int count); 
  void Clear();

  // Copy
  void CopyFrom(TArray<T>* sourceArray);
  void CopyFrom(TArray<T>* sourceArray, int sourceIndex);
  void CopyFrom(TArray<T>* sourceArray, int sourceIndex, int targetIndex, int count);

  // Linear search
  int IndexOf(T* item);
  int IndexOf(T* item, int startIndex);
  int IndexOf(T* item, int startIndex, int count);
  int LastIndexOf(T* item);
  int LastIndexOf(T* item, int startIndex);
  int LastIndexOf(T* item, int startIndex, int count);
  bool Contains(T* item);

  // Binary search
  int BinarySearch(T* item);
  int BinarySearch(T* item, CompareProc* comparer, void* userData);
  int BinarySearch(T* item, int startIndex, int count);
  int BinarySearch(T* item, int startIndex, int count, CompareProc* comparer, void* userData);

  // Auxiliary
  void Reverse();
  void Reverse(int startIndex, int count);
  void Sort();
  void Sort(CompareProc* comparer, void* userData);
  void Sort(int startIndex, int count, CompareProc* comparer, void* userData);
  void TrimToSize();

private:
  PointerArray m_Items;
};

// TArray

template <class T>
TArray<T>::TArray() : 
  m_Items()
{
}

template <class T>
TArray<T>::TArray(const TArray<T>& sourceArray) : 
  m_Items()
{
  CopyFrom(sourceArray);
}

template <class T>
TArray<T>& TArray<T>::operator=(const TArray<T>& sourceArray)
{
  CopyFrom(sourceArray);
}

template <class T>
TArray<T>::~TArray()
{
}

template <class T>
inline
int TArray<T>::Capacity()
{
  return m_Items.Capacity();
}

template <class T>
inline
void TArray<T>::SetCapacity(int capacity)
{
  m_Items.SetCapacity(capacity);
}

template <class T>
inline
int TArray<T>::Count()
{
  return m_Items.Count();
}

template <class T>
inline
void TArray<T>::SetCount(int count)
{
  m_Items.SetCount(count);
}

template <class T>
inline
T* TArray<T>::Item(int index)
{
  return static_cast<T*>(m_Items.Item(index));
}

template <class T>
inline
void TArray<T>::SetItem(int index, T* item)
{
  m_Items.SetItem(index, item);
}

template <class T>
inline
void TArray<T>::SetRange(int index, T** items, int count)
{
  m_Items.SetRange(index, items, count);
}

template <class T>
inline
void TArray<T>::Add(T* item)
{
  m_Items.Add(item);
}

template <class T>
inline
void TArray<T>::AddRange(T** items, int count)
{
  m_Items.AddRange((void**)items, count);
}

template <class T>
inline
void TArray<T>::Insert(int index, T* item)
{
  m_Items.Insert(index, item);
}

template <class T>
inline
void TArray<T>::InsertRange(int index, T** items, int count)
{
  m_Items.InsertRange(index, (void**)items, count);
}

template <class T>
inline
bool TArray<T>::Remove(T* item)
{
  return m_Items.Remove(item);
}

template <class T>
inline
T* TArray<T>::RemoveAt(int index)
{
  return static_cast<T*>(m_Items.RemoveAt(index));
}

template <class T>
inline
void TArray<T>::RemoveRange(int index, int count)
{
  m_Items.RemoveRange(index, count);
}

template <class T>
inline
void TArray<T>::Clear()
{
  m_Items.Clear();
}

template <class T>
inline
void TArray<T>::CopyFrom(TArray<T>* sourceArray)
{
  m_Items.CopyFrom(sourceArray->m_Items);
}

template <class T>
inline
void TArray<T>::CopyFrom(TArray<T>* sourceArray, int sourceIndex)
{
  m_Items.CopyFrom(sourceArray->m_Items, sourceIndex);
}

template <class T>
inline
void TArray<T>::CopyFrom(TArray<T>* sourceArray, int sourceIndex, int targetIndex, int count)
{
  m_Items.CopyFrom(sourceArray, sourceIndex, targetIndex, count);
}

template <class T>
inline
int TArray<T>::IndexOf(T* item)
{
  return m_Items.IndexOf(item);
}

template <class T>
inline
int TArray<T>::IndexOf(T* item, int startIndex)
{
  return m_Items.IndexOf(item, startIndex);
}

template <class T>
inline
int TArray<T>::IndexOf(T* item, int startIndex, int count)
{
  return m_Items.IndexOf(item, startIndex, count);
}

template <class T>
inline
int TArray<T>::LastIndexOf(T* item)
{
  return m_Items.LastIndexOf(item);
}

template <class T>
inline
int TArray<T>::LastIndexOf(T* item, int startIndex)
{
  return m_Items.LastIndexOf(item, startIndex);
}

template <class T>
inline
int TArray<T>::LastIndexOf(T* item, int startIndex, int count)
{
  return m_Items.LastIndexOf(item, startIndex, count);
}

template <class T>
inline
bool TArray<T>::Contains(T* item)
{
  return m_Items.Contains(item);
}

template <class T>
inline
int TArray<T>::BinarySearch(T* item)
{
  return m_Items.BinarySearch(item);
}

template <class T>
inline
int TArray<T>::BinarySearch(T* item, int startIndex, int count)
{
  return m_Items.BinarySearch(item, startIndex, count);
}

template <class T>
inline
int TArray<T>::BinarySearch(T* item, CompareProc* comparer, void* userData)
{
  return m_Items.BinarySearch(item, comparer, userData);
}

template <class T>
inline
int TArray<T>::BinarySearch(T* item, int startIndex, int count, CompareProc* comparer, void* userData)
{
  return m_Items.BinarySearch(item, startIndex, count, comparer, userData);
}

template <class T>
inline
void TArray<T>::Reverse()
{
  m_Items.Reverse();
}

template <class T>
inline
void TArray<T>::Reverse(int startIndex, int count)
{
  m_Items.Reverse(startIndex, count);
}

template <class T>
inline
void TArray<T>::Sort()
{
  m_Items.Sort();
}

template <class T>
inline
void TArray<T>::Sort(CompareProc* comparer, void* userData)
{
  m_Items.Sort(CastToPointerComparer(comparer), userData);
}

template <class T>
inline
void TArray<T>::Sort(int startIndex, int count, CompareProc* comparer, void* userData)
{
  m_Items.Sort(startIndex, count, CastToPointerComparer(comparer), userData);
}

template <class T>
inline
void TArray<T>::TrimToSize()
{
  m_Items.TrimToSize();
}

template <class T>
inline
PointerArray::CompareProc* CastToPointerComparer(int (STDCALL *comparer)(T* item1, T* item2, void* userData))
{
  PointerArray::CompareProc* result = (PointerArray::CompareProc*)comparer;
  COMPILE_TIME_CHECK_BEGIN
    // Ensure that CompareProc and TArray<Object>::CompareProc are binary compatible.
    void* userData = NULL;
    int a = result((void*)NULL, (void*)NULL, userData);
    a = result((Object*)NULL, (Object*)NULL, userData);
    a = comparer((Object*)NULL, (Object*)NULL, userData);
  COMPILE_TIME_CHECK_END
  return result;
}

} // namespace Sys

#endif // __Grom_System_Array_h__
