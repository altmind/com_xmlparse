// Free open-source Grom library

#ifndef __Grom_System_Synchronization_h__
#define __Grom_System_Synchronization_h__

#include "Config.h"

#ifdef CFG_WIN32
# include <windows.h>
#endif
#ifdef CFG_POSIX
# include <pthread.h>
#endif
#include "Exception.h"

#ifdef CFG_POSIX
long InterlockedIncrement(long volatile* addend);
long InterlockedDecrement(long volatile* addend);
long InterlockedExchange(long volatile* target, long value);
#endif // CFG_POSIX

namespace Sys
{

// The ThreadMutex class provides mutex that can be used to synchronize
// different threads of the same process.
class ThreadMutex
{
public:
  ThreadMutex();
  ~ThreadMutex();

  void Lock();
  void Unlock();

private:
#ifdef CFG_WIN32
  CRITICAL_SECTION m_Mutex;
#endif // CFG_WIN32
#ifdef CFG_POSIX
  pthread_mutex_t m_Mutex;
#endif // CFG_POSIX
};

// The ThreadMutexLockScope class provides provides a reliable, exception-safe 
// locking technique. When created, a Lock object locks specified mutex. 
// The mutex is unlocked by the Lock destructor. The Lock object can be 
// also explicitly controlled.
// Example:
//   ThreadMutex mutex;
//   ...
//   void DoAtomicAction()
//   {
//     ThreadMutexLockScope lock(mutex);
//     // Any atomic actions
//     ...
//   }
//
//   void DoAtomicAction2(void* args)
//   {
//     ThreadMutexLockScope lock(mutex, false);
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
class ThreadMutexLockScope
{
public:
  ThreadMutexLockScope(ThreadMutex* mutex, bool lockNow = true);
  ThreadMutexLockScope(ThreadMutex& mutex, bool lockNow = true);
  ~ThreadMutexLockScope();

  void LockNow();
  void UnlockNow();

private:
  // Copying and assignment are prohibited.
  ThreadMutexLockScope(const ThreadMutexLockScope&);
  ThreadMutexLockScope& operator=(const ThreadMutexLockScope&);

  ThreadMutex* m_Mutex;
  bool m_IsLocked; // shows that mutex should be unlocked by destructor.
};

SystemException* CreateSystemResourceException();

extern ThreadMutex g_GlobalMutex;

} // namespace Sys

#endif // __Grom_System_Synchronization_h__
