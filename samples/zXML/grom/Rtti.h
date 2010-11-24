// Free open-source Grom library

#ifndef __Grom_System_Rtti_h__
#define __Grom_System_Rtti_h__

#include "Config.h"

#include <typeinfo>
#include "Exception.h"

namespace Sys
{

class Rtti
{
public:
  virtual ~Rtti(); // provides RTTI
};

} // namespace Sys

template <class T>
inline
T as(Sys::Rtti* object)
{
  return dynamic_cast<T>(object);
}

template <class T>
T cast(Sys::Rtti* object)
{
  T p = as<T>(object);
  if (p == NULL && object != NULL)
    throw Sys::CreateInvalidCastException(
      typeid(object).name(), typeid(T).name());
  return p;
}

template <class T>
inline
bool is(Sys::Rtti* object)
{
  return as<T>(object) != NULL;
}

#endif //__Grom_System_Rtti_h__
