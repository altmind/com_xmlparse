// Free open-source Grom library

#include "stdafx.h"
#ifndef __Grom_System_Event_h__
#define __Grom_System_Event_h__

#include "Config.h"

#include "Rtti.h"

namespace Sys
{

struct EventArgs;
struct Delegate;

// EventArgs

struct EventArgs : public Rtti
{
};

// The EventImpl class is implementation for the type-safe Event class.
// Main purpose is to avoid code bloating during templates instantiation.

typedef void STDCALL DelegateProcedure(void* args, void* userData, Delegate* delegateChain);

class EventImpl
{
public:
  EventImpl();
  void Attach(DelegateProcedure* procedure, void* userData);
  void Detach(DelegateProcedure* procedure, void* userData);
  void Invoke(void* args);

protected:
  Delegate** Find(DelegateProcedure* procedure, void* userData);
  void DeleteDetachedDelegates();

private:
  Delegate* m_Handlers;
  bool m_InvokeInProgress;
};

// The TEvent<T> class is a thin type-safe wrapper over the EventImpl class.
// It provides:
//   - multicasting (multiple handlers can be attached);
//   - faultless attach/detach during event handling;
//   - each handler explicitly control the invocation of the
//     previous handlers through InvokeDefaultHandler (like a
//     virtual method controls invocation of the overriden
//     method of the base class through BaseClass::DoSomething);
//   - nested events (invoking same event during handling);

// TEvent<T>

template <class T>
class TEvent
{
public:
  template <class A, typename D>
  void Attach(void (STDCALL *procedure)(A* args, D* userData, Delegate* delegateChain), D* userData)
  {
    COMPILE_TIME_CHECK(T* derived = NULL; A* base = derived;);
    m_Impl.Attach((Sys::DelegateProcedure*)procedure, userData);
  }

  template <class A, typename D>
  void Detach(void (STDCALL *procedure)(A* args, D* userData, Delegate* delegateChain), D* userData)
  {
    COMPILE_TIME_CHECK(T* derived = NULL; A* base = derived;);
    m_Impl.Detach((Sys::DelegateProcedure*)procedure, userData);
  }

  void Invoke(T* args)
  {
    m_Impl.Invoke(args);
  }

private:
  EventImpl m_Impl;
};

void InvokeDefaultHandler(Delegate* delegateChain, void* args);

} // namespace Sys

#endif //__Grom_System_Event_h__
