// Free open-source Grom library

#ifndef __Grom_System_Object_h__
#define __Grom_System_Object_h__

#include "Config.h"

#include <new>
#include "Rtti.h"

namespace Sys
{

class PointerArray;

// The Object class is the base class for all reference-type objects.
// It provides:
//   - atomatic memory initialization upon object creation;
//   - owner-based memory management;
//   - delayed free of object memory;
//   - fully working virtual methods during object creation and destruction.
// Objects of this class have to be created dynamically on the heap,
// using the Create function:
//
//   Object* foo = Create<Object>(NULL);
//
// The Create function gets reference to an owner object, which becomes
// responsible for destroying all owned objects. You may specify NULL
// as the owner. In this case you are responsible for manual destruction.
// To destroy an object manually call the Destroy method.
//
//   Object* foo = Create<Object>(NULL);
//   Object* bar = Create<Object>(foo); // bar is owned by foo
//   foo->Destroy(); // destroy bar object, then foo object
//
// You may destroy object manually even if owner is specified. In this case
// object will be correctly removed from the owner's list.
// To initialize aggregated fields you have to override Initialize method.
// To perform finalization you should override Finalize method in which
// you may safely call any virtual method (unlike standard C++ destructor).
// Never override destructor for such purposes. You may call IsDestroying
// method to determine whether the object is being destroyed and refuse
// from handling notifications in this case.

class Object : public Rtti
{
template <class T> friend T* Create(Object* owner);
friend class DestroyScope;
friend class Monitor;
friend class Collection;

public:
  void Destroy();
  bool IsDestroying();
  Object* Owner();
  void SetOwner(Object* value);

protected:
  Object();
  virtual ~Object(); // override
  virtual void Initialize(Object* owner); // new
  virtual void Finalize(); // new
  virtual bool CanBeOwnerOf(Object* object); // new
  void ValidateOwner(Object* owner);

private:
  Object(const Object&); // forbidden
  Object& operator=(const Object&); // forbidden
  PointerArray* AcquireObjectArray();
  void TakeOwnership(Object* object);
  void DropOwnership(Object* object);
  Object* DropOwnershipAt(int index);
  int CyclicSearch(Object* object);
  void FreeMemory();
  static void FreeAllDestroyedObjects();
  // Fields
  Object* m_Owner;
  PointerArray* m_OwnedObjects;
  int m_SyncBlockIndex;
  bool m_IsDestroying; // should be m_State (or m_Flags)
};

// Collection

class Collection : public Object
{
public:
  typedef int STDCALL CompareProc(Object* item1, Object* item2, void* userData);

  // Capacity/Count/Item
  int Capacity();
  void SetCapacity(int capacity);
  int Count();
  Object* Item(int index);

  // Add/Insert/Remove/Move/Clear
  void Add(Object* item);
  void Insert(int index, Object* item);
  void Remove(Object* item);
  Object* RemoveAt(int index);
  void Move(int sourceIndex, int targetIndex);
  void MoveRange(int sourceIndex, int count, int targetIndex);
  static void Clear(Collection*& collection); // clear through recreation

  // Linear search
  int IndexOf(Object* item);
  int IndexOf(Object* item, int startIndex);
  int IndexOf(Object* item, int startIndex, int count);
  int LastIndexOf(Object* item);
  int LastIndexOf(Object* item, int startIndex);
  int LastIndexOf(Object* item, int startIndex, int count);
  bool Contains(Object* item);

  // Sort and binary search
  void Sort();
  void Sort(CompareProc* comparer, void* userData);
  void Sort(int startIndex, int count, CompareProc* comparer, void* userData);
  int BinarySearch(Object* item);
  int BinarySearch(Object* item, CompareProc* comparer, void* userData);
  int BinarySearch(Object* item, int startIndex, int count);
  int BinarySearch(Object* item, int startIndex, int count, CompareProc* comparer, void* userData);

  // Auxiliary
  void Reverse();
  void Reverse(int startIndex, int count);
  void TrimToSize();
};

// Create<T>

template <class T>
T* Create(Object* owner)
{
  const size_t n = sizeof(T);
  Object* p = (Object*)malloc(n);
  if (p != NULL)
  {
    try
    {
      memset(p, 0, n);
      p = new (p) T; // placement new configures v-table
      try
      {
        p->Initialize(owner);
      }
      catch (...)
      {
        // p->Finalize(); // it has to be called even for partially initialized objects
        p->~Object();
        throw;
      }
    }
    catch (...)
    {
      free(p);
      throw;
    }
  }
  return (T*)p;
}

// Object

inline
Object* Object::Owner()
{
  return m_Owner;
}

inline
bool Object::IsDestroying()
{
  return m_IsDestroying;
}

// NearestOwner

template <class T>
T* NearestOwner(Object* object)
{
  object = object->Owner();
  while (object != NULL && !is<T*>(object))
    object = object->Owner();
  return (T*)object; // object is either a valid pointer to T, or NULL
}

// The DestroyScope class provides thread-safe and exception-safe destroy
// of multiple objects.

class DestroyScope
{
public:
  DestroyScope();
  ~DestroyScope();

private:
  // Copying, assignment and in-heap creation are prohibited.
  DestroyScope(const DestroyScope&);
  DestroyScope& operator=(const DestroyScope&);
  void* operator new(size_t size);
};

// The LockScope class provides provides a reliable, exception-safe 
// locking technique. When created, a LockScope variable locks specified 
// object. The object is unlocked by the LockScope destructor. The LockScope 
// variable can be also explicitly controlled.
// Example:
//   Object* obj = Create<Object>(GlobalOwner());
//   ...
//   void DoAtomicAction()
//   {
//     LockScope lock(obj);
//     // Any atomic actions
//     ...
//   }
//
//   void DoAtomicAction2(void* args)
//   {
//     LockScope lock(obj, false);
//     if (args == NULL)
//       throw CreateNullArgumentException();
//     else
//     {
//       lock.LockNow();
//       // Any atomic actions
//       ...
//     }
//   }
//
class LockScope
{
public:
  LockScope(Object* obj, bool lockNow = true);
  ~LockScope();

  void LockNow();
  void UnlockNow();

private:
  // Copying, assignment and in-heap creation are prohibited.
  LockScope(const LockScope&);
  LockScope& operator=(const LockScope&);
  void* operator new(size_t size);

  Object* m_Object;
  bool m_IsLocked; // shows that object should be unlocked by destructor.
};

// Global functions

void Initialize();
void Finalize();
Object* GlobalOwner();

extern int g_MaxSyncBlockCount; // you may change it before calling Initialize

} // namespace Sys

template <class T>
void DestroyAndNull(T*& var)
{
  var->Destroy();
  var = NULL;
}

#endif //__Grom_System_Object_h__
