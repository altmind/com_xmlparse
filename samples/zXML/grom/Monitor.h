// Free open-source Grom library

#ifndef __Grom_System_Monitor_h__
#define __Grom_System_Monitor_h__

#include "Config.h"

#include "Object.h"

namespace Sys
{

class ThreadMutex;

class Monitor
{
friend class Object;

public:
  static void Enter(Object* obj);
  static void Exit(Object* obj);

private:
  Monitor(); // forbidden
  ~Monitor(); // forbidden

  static ThreadMutex* CreateThreadMutex(Object* obj);
  static void DestroyThreadMutex(Object* obj);
  static ThreadMutex* AcquireThreadMutex(Object* obj);
  static ThreadMutex* GetThreadMutex(Object* obj);
};

class SynchronizationLockException : public SystemException
{
public:
  SynchronizationLockException(const std::string& message);
  SynchronizationLockException(const std::string& message, Exception* innerException);
};

SynchronizationLockException* CreateSynchronizationLockException();

int GetSyncBlockCount();

} // namespace Sys

#endif // __Grom_System_Monitor_h__
