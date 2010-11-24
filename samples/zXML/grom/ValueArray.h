// Free open-source Grom library

#ifndef __Grom_System_ValueArray_h__
#define __Grom_System_ValueArray_h__

#include "Config.h"

#include "Exception.h"
#include "Utils.h"
#include "StringUtils.h"

namespace Sys
{

template <class T>
int STDCALL CompareValues(const T& value1, const T& value2, void* userData)
{
  if (value1 > value2)
    return 1;
  else if (value1 < value2)
    return -1;
  else
    return 0;
}

template <>
inline
int STDCALL CompareValues(const std::string& value1, const std::string& value2, void* userData)
{
  return value1.compare(value2);
}

// The TValueArray<T> class provides:
//   - mandatory range checking even in release builds;
//   - function pointer as comparer instead of class with operator() overloaded;
//   - .NET-like API for easy migration to the .NET Framework.

template <class T> 
class TValueArray
{
public:
  typedef int STDCALL CompareProc(const T& item1, const T& item2, void* userData);

  TValueArray();
  TValueArray(const TValueArray<T>& sourceArray);
  TValueArray<T>& operator=(const TValueArray<T>& sourceArray);
  ~TValueArray();

  // Capacity/Count
  int Capacity() const;
  void SetCapacity(int capacity);
  int Count() const;
  void SetCount(int count);

  // Get/Set
  const T& Item(int index) const;
  T& Item(int index);
  const T& operator[](int index) const;
  T& operator[](int index);
  void SetItem(int index, const T& item);
  void SetRange(int index, T* items, int count);  

  // Add/Remove/Clear
  void Add(const T& item);
  void AddRange(T* items, int count);
  void Insert(int index, const T& item);
  void InsertRange(int index, T* items, int count);
  bool Remove(const T& item);
  void RemoveAt(int index);
  void RemoveRange(int index, int count);  
  void Clear(); // == SetCount(0);

  // Copy
  void CopyFrom(const TValueArray<T>& sourceArray);
  void CopyFrom(const TValueArray<T>& sourceArray, int sourceIndex);
  void CopyFrom(const TValueArray<T>& sourceArray, int sourceIndex, int targetIndex, int count);

  // Linear search
  int IndexOf(const T& item) const;
  int IndexOf(const T& item, int startIndex) const;
  int IndexOf(const T& item, int startIndex, int count) const;
  int LastIndexOf(const T& item) const;
  int LastIndexOf(const T& item, int startIndex) const;
  int LastIndexOf(const T& item, int startIndex, int count) const;
  bool Contains(const T& item) const; // perfoms linear search

  // Binary search
  int BinarySearch(const T& item) const;
  int BinarySearch(const T& item, CompareProc* comparer, void* userData) const;
  int BinarySearch(const T& item, int startIndex, int count) const;
  int BinarySearch(const T& item, int startIndex, int count, CompareProc* comparer, void* userData) const;

  // Auxiliary
  void Reverse();
  void Reverse(int startIndex, int count);
  void Sort();
  void Sort(CompareProc* comparer, void* userData);
  void Sort(int startIndex, int count, CompareProc* comparer, void* userData);
  void TrimToSize();

private:
  void Grow();
  int ForwardSearch(const T& item, int index, int count) const;
  int BackwardSearch(const T& item, int index, int count) const;
  static int SizeByCount(int count);
  void DoSort(int fromIndex, int toIndex, CompareProc* comparer, void* userData);

  T* m_Data;
  int m_Capacity;
  int m_Count;
};

// TValueArray

template <class T>
TValueArray<T>::TValueArray() : 
  m_Data(NULL), m_Capacity(0), m_Count(0)
{
}

template <class T>
TValueArray<T>::TValueArray(const TValueArray<T>& sourceArray) : 
  m_Data(NULL), m_Capacity(0), m_Count(0)
{
  CopyFrom(sourceArray);
}

template <class T>
TValueArray<T>& TValueArray<T>::operator=(const TValueArray<T>& sourceArray)
{
  CopyFrom(sourceArray);
}

template <class T>
TValueArray<T>::~TValueArray()
{
  for (int i = 0; i < m_Count; ++i)
  {
    T* p = &m_Data[i];
    p->~T(); // placement destructor
  }
  if (m_Data != NULL)
  {
    free(m_Data);
    m_Data = NULL;
  }
}

template <class T> 
inline
int TValueArray<T>::Capacity() const
{
  return m_Capacity;
}

template <class T>
void TValueArray<T>::SetCapacity(int capacity)
{
  if (m_Capacity != capacity)
  {
    if (capacity >= m_Count)
    {
      void* t = realloc(m_Data, SizeByCount(capacity));
      if (t == NULL && capacity > 0)
        throw new SystemException("out of memory");
      m_Data = (T*)t;
      m_Capacity = capacity;
    }
    else
      throw new ArgumentException("capacity cannot be less than count");
  }
}

template <class T> 
inline
int TValueArray<T>::Count() const
{
  return m_Count;
}

template <class T>
void TValueArray<T>::SetCount(int count)
{
  if (m_Count != count)
  {
    if (count >= 0)
    {
      if (count > m_Capacity)
        SetCapacity(count);
      if (count > m_Count)
      {
        memset(m_Data + m_Count, 0, SizeByCount(count - m_Count));
        for (int i = m_Count; i < count; ++i)
          new (m_Data + i) T; // placement constructor
      }
      else // count < m_Count
      {
        for (int i = m_Count - 1; i >= count; --i)
          (m_Data + i)->~T(); // placement destructor
      }
      m_Count = count;
      if (count == 0)
        SetCapacity(0); // free m_Data
    }
    else
      throw CreateItemCountNegativeException(count);
  }
}

template <class T>
const T& TValueArray<T>::Item(int index) const
{
  ValidateIndex(index, m_Count);
  return m_Data[index];
}

template <class T>
T& TValueArray<T>::Item(int index)
{
  ValidateIndex(index, m_Count);
  return m_Data[index];
}

template <class T> 
inline
const T& TValueArray<T>::operator[](int index) const
{
  return Item(index);
}

template <class T> 
inline
T& TValueArray<T>::operator[](int index)
{
  return Item(index);
}

template <class T>
void TValueArray<T>::SetItem(int index, const T& item)
{
  ValidateIndex(index, m_Count);
  m_Data[index] = item;
}

template <class T>
void TValueArray<T>::SetRange(int index, T* items, int count)
{
  ValidateRange(index, count, m_Count);
  if (count > 0)
  {
    if (items != NULL)
    {
      T* start = m_Data + index;
      for (int i = count - 1; i >= 0; --i)
        (start + i)->~T(); // placement destructor
      for (int i = 0; i < count; ++i)
        new (start + i) T(items[i]); // placement copy constructor
    }
    else
      throw CreateArgumentNullException();
  }
}

template <class T>
void TValueArray<T>::Add(const T& item)
{
  if (m_Capacity == m_Count)
    Grow();
  new (m_Data + m_Count) T(item); // placement copy constructor
  ++m_Count;
}

template <class T>
void TValueArray<T>::AddRange(T* items, int count)
{
  if (count > 0)
  {
    if (items != NULL)
    {
      if (m_Count + count > m_Capacity)
        SetCapacity(m_Count + count);
      T* start = m_Data + m_Count;
      for (int i = 0; i < count; ++i)
        new (start + i) T(items[i]); // placement copy constructor
      m_Count += count;
    }
    else
      throw CreateArgumentNullException();
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
}

template <class T>
void TValueArray<T>::Insert(int index, const T& item)
{
  ValidateIndex(index, m_Count + 1);
  if (m_Capacity == m_Count)
    Grow();
  if (index < m_Count)
  {
    T* start = m_Data + index;
    memmove(start + 1, start, SizeByCount(m_Count - index));
  }
  new (m_Data + index) T(item); // placement copy constructor
  ++m_Count;
}

template <class T>
void TValueArray<T>::InsertRange(int index, T* items, int count)
{
  ValidateIndex(index, m_Count + 1);
  if (count > 0)
  {
    if (items != NULL)
    {
      if (m_Count + count > m_Capacity)
        SetCapacity(m_Count + count);    
      T* start = m_Data + index;
      if (index < m_Count)
        memmove(start + count, start, SizeByCount(m_Count - index));
      for (int i = 0; i < count; ++i)
        new (start + i) T(items[i]); // placement copy constructor
      m_Count += count;
    }
    else
      throw CreateArgumentNullException();
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
}

template <class T> 
bool TValueArray<T>::Remove(const T& item)
{
  int i = IndexOf(item);
  if (i >= 0)
    RemoveAt(i);
  return i >= 0;
}

template <class T>
void TValueArray<T>::RemoveAt(int index)
{
  ValidateIndex(index, m_Count);
  T* start = m_Data + index;
  start->~T(); // placement destructor
  memmove(start, start + 1, SizeByCount(m_Count - 1 - index));
  --m_Count;
}

template <class T>
void TValueArray<T>::RemoveRange(int index, int count)
{
  ValidateIndex(index, m_Count);
  count = NormalizeRange(index, count, m_Count);
  if (count > 0)
  {
    if (index + count != m_Count)
    {
      T* start = m_Data + index;
      for (int i = count - 1; i >= 0; --i)
        (start + i)->~T(); // placement destructor
      memmove(start, start + count, SizeByCount(m_Count - count - index));
    }
    m_Count -= count;
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
}

template <class T> 
inline
void TValueArray<T>::Clear() 
{
  SetCount(0);
}

template <class T> 
inline
void TValueArray<T>::CopyFrom(const TValueArray<T>& sourceArray)
{
  CopyFrom(sourceArray, 0, 0, sourceArray.m_Count);
}

template <class T> 
inline
void TValueArray<T>::CopyFrom(const TValueArray<T>& sourceArray, int sourceIndex)
{
  CopyFrom(sourceArray, sourceIndex, 0, sourceArray.m_Count);
}

template <class T>
void TValueArray<T>::CopyFrom(const TValueArray<T>& sourceArray, int sourceIndex, int targetIndex, int count)
{
  ValidateRange(sourceIndex, count, sourceArray.m_Count);
  ValidateIndex(targetIndex, m_Count + 1);
  SetCount(targetIndex);
  if (m_Count + count > m_Capacity)
    SetCapacity(m_Count + count);    
  T* source = sourceArray.m_Data + sourceIndex;
  T* target = m_Data + targetIndex;
  for (int i = 0; i < count; ++i)
    new (target + i) T(source[i]); // placement copy constructor
  m_Count += count;
}

template <class T>
int TValueArray<T>::ForwardSearch(const T& item, int index, int count) const
{
  int result = -1;
  if (count > 0)
  {
    ValidateIndex(index, m_Count);
    ValidateRange(index, count, m_Count);
    for (int i = index; i < index + count; ++i)
    {
      if (m_Data[i] == item)
      {
        result = i;
        break;
      }
    }
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
  return result;
}

template <class T>
int TValueArray<T>::BackwardSearch(const T& item, int index, int count) const
{
  int result = -1;
  if (count > 0)
  {
    ValidateIndex(index, m_Count);
    ValidateRange(index, count, m_Count);
    for (int i = index + count - 1; i >= index; --i)
    {
      if (m_Data[i] == item)
      {
        result = i;
        break;
      }
    }
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
  return result;
}

template <class T> 
inline
int TValueArray<T>::IndexOf(const T& item) const
{
  return IndexOf(item, 0, m_Count);
}

template <class T> 
inline
int TValueArray<T>::IndexOf(const T& item, int startIndex) const
{
  return IndexOf(item, startIndex, m_Count - startIndex);
}

template <class T>
int TValueArray<T>::IndexOf(const T& item, int startIndex, int count) const
{
  return ForwardSearch(item, startIndex, count);
}

template <class T> 
inline
int TValueArray<T>::LastIndexOf(const T& item) const
{
  return LastIndexOf(item, 0, m_Count);
}

template <class T> 
inline
int TValueArray<T>::LastIndexOf(const T& item, int startIndex) const
{
  return LastIndexOf(item, startIndex, m_Count - startIndex);
}

template <class T>
int TValueArray<T>::LastIndexOf(const T& item, int startIndex, int count) const
{
  return BackwardSearch(item, startIndex, count);
}

template <class T> 
inline
bool TValueArray<T>::Contains(const T& item) const
{
  return IndexOf(item) != -1;
}

template <class T> 
inline
int TValueArray<T>::BinarySearch(const T& item) const
{
  return BinarySearch(item, CompareValues<T>, (void*)NULL);
}

template <class T> 
inline
int TValueArray<T>::BinarySearch(const T& item, CompareProc* comparer, void* userData) const
{
  if (m_Count > 0)
    return BinarySearch(item, (int)0, m_Count, comparer, userData);
  else
    return ~0;
}

template <class T>
inline
int TValueArray<T>::BinarySearch(const T& item, int startIndex, int count) const
{
  return BinarySearch(item, startIndex, count, CompareValues<T>, (void*)NULL);
}

template <class T>
int TValueArray<T>::BinarySearch(const T& item, int startIndex, int count, CompareProc* comparer, void* userData) const
{
  int result = ~startIndex;
  if (count > 0)
  {
    ValidateIndex(startIndex, m_Count);
    count = NormalizeRange(startIndex, count, m_Count);
    int low = startIndex;
    int high = startIndex + count - 1;
    while (low <= high) 
    {
      int mid = (low + high)/2;
      int comp = comparer(m_Data[mid], item, userData);
      if (comp < 0)
        low = mid + 1;
      else if (comp > 0)
        high = mid - 1;
      else
      {
        result = mid;
        break;
      }
    }
    if (result < 0)
      result = ~low;
  }
  else if (count < 0)
    throw CreateItemCountNegativeException(count);
  return result;
}

template <class T> 
inline
void TValueArray<T>::Reverse()
{
  Reverse(0, m_Count);
}

template <class T>
void TValueArray<T>::Reverse(int startIndex, int count)
{
  ValidateIndex(startIndex, m_Count);
  count = NormalizeRange(startIndex, count, m_Count);
  if (count > 1)
  { 
    T* start = m_Data + startIndex;
    T* end = start + count -1;
    for (; start < end; start++, end--)
    {
      T val = *start;
      *start = *end;
      *end = val;
    }
  }
}

template <class T> 
inline
void TValueArray<T>::Sort()
{
  Sort(CompareValues<T>, (void*)NULL);
}

template <class T> 
inline
void TValueArray<T>::Sort(CompareProc* comparer, void* userData)
{
  Sort(0, m_Count, comparer, userData);
}

template <class T>
void TValueArray<T>::Sort(int startIndex, int count, CompareProc* comparer, void* userData)
{
  ValidateIndex(startIndex, m_Count);
  count = NormalizeRange(startIndex, count, m_Count);
  DoSort(startIndex, startIndex + count - 1, comparer, userData);
}

template <class T> 
inline
void TValueArray<T>::TrimToSize()
{
  SetCapacity(m_Count);
}

template <class T>
void TValueArray<T>::Grow()
{
  int delta;
  if (m_Capacity > 64)
    delta = m_Capacity / 4;
  else if (m_Capacity > 8)
    delta = 16;
  else
    delta = 4;
  SetCapacity(m_Capacity + delta);
}

template <class T> 
inline
int TValueArray<T>::SizeByCount(int count)
{
  return sizeof(T) * count;
}

template <class T>
void TValueArray<T>::DoSort(int fromIndex, int toIndex, CompareProc* comparer, void* userData)
{
  int i, j, m;
  do 
  {
    i = fromIndex;
    j = toIndex;
    m = (fromIndex + toIndex)/2;
    T midItem = m_Data[m];
    do
    {
      while (comparer(m_Data[i], midItem, userData) < 0)
        ++i;
      while (comparer(midItem, m_Data[j], userData) < 0)
        --j;
      if (i <= j)
      {
        T t = m_Data[i];
        m_Data[i] = m_Data[j];
        m_Data[j] = t;
        if (m == i)
          m = j;
        else if (m == j)
          m = i;
        ++i;
        --j;
      }
    }
    while (i <= j);
    if (fromIndex < j)
      DoSort(fromIndex, j, comparer, userData);
    fromIndex = i;
  }
  while (i < toIndex);
}

} // namespace Sys

#endif // __Grom_System_ValueArray_h__
