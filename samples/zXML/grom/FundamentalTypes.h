// Free open-source Grom library

#ifndef __Grom_System_FundamentalTypes_h__
#define __Grom_System_FundamentalTypes_h__

#include "Config.h"

#include <sys/types.h>
#include <limits>

#ifdef CFG_COMPILER_MSC_ANY
  typedef signed char Int8;
  typedef unsigned char UInt8;
  typedef signed short Int16;
  typedef unsigned short UInt16;
  typedef signed int Int32;
  typedef unsigned int UInt32;
  typedef __int64 Int64;
  typedef unsigned __int64 UInt64;
#endif // CFG_COMPILER_MSC_ANY

#ifdef CFG_COMPILER_GCC
#  ifdef CFG_OS_LINUX
     typedef int8_t Int8;
     typedef u_int8_t UInt8;
     typedef int16_t Int16;
     typedef u_int16_t UInt16;
     typedef int32_t Int32;
     typedef u_int32_t UInt32;
     typedef int64_t Int64;
     typedef u_int64_t UInt64;
#  endif // CFG_OS_LINUX
#  ifdef CFG_OS_SOLARIS
     typedef int8_t Int8;
     typedef uint8_t UInt8;
     typedef int16_t Int16;
     typedef uint16_t UInt16;
     typedef int32_t Int32;
     typedef uint32_t UInt32;
     typedef int64_t Int64;
     typedef uint64_t UInt64;
#  endif // CFG_OS_SOLARIS
#  ifdef CFG_OS_CYGWIN
     typedef int8_t Int8;
     typedef u_int8_t UInt8;
     typedef int16_t Int16;
     typedef u_int16_t UInt16;
     typedef int32_t Int32;
     typedef u_int32_t UInt32;
     typedef int64_t Int64;
     typedef u_int64_t UInt64;
#  endif // CFG_OS_CYGWIN
#endif // CFG_COMPILER_GCC

#endif //__Grom_System_FundamentalTypes_h__
