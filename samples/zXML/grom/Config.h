// Free open-source Grom library
//
// This header file must be included first by all other header files.
//
// CFG_WIN32                - Win32 configuration
// CFG_POSIX                - Posix configuration
// CFG_OS_WINDOWS           - Windows operating system
// CFG_OS_LINUX             - Linux operating system
// CFG_OS_SOLARIS           - Solaris operating system
// CFG_OS_CYGWIN			- Cygwin
// CFG_COMPILER_GCC         - GCC
// CFG_COMPILER_MSC_ANY     - Any version of Microsoft Visual C++
// CFG_COMPILER_MSC_60_1998 - Microsoft Visual C++ 98 (6.0)
// CFG_COMPILER_MSC_71_2003 - Microsoft Visual C++ 2003 (7.1)
// CFG_COMPILER_MSC_80_2005 - Microsoft Visual C++ 2005 (8.0)

#include "stdafx.h"

#ifndef __Grom_System_Config_h__
#define __Grom_System_Config_h__

// Configure compiler version
#ifdef _MSC_VER
#  define CFG_OS_WINDOWS
#  define CFG_COMPILER_MSC_ANY
#  if _MSC_VER <= 1200
#    define CFG_COMPILER_MSC_60_1998
#  elif _MSC_VER <= 1310
#    define CFG_COMPILER_MSC_70_2003
#  else _MSC_VER <= 1400
#    define CFG_COMPILER_MSC_80_2005
#  endif
#  ifdef _WIN32
#    define CFG_WIN32
#  endif
#endif // _MSC_VER
#ifdef __GNUC__
#  define CFG_COMPILER_GCC
#  ifdef __unix__
#    define CFG_POSIX
#  endif
#  ifdef linux
#    define CFG_OS_LINUX
#    ifndef RECURSIVE_MUTEXES_SUPPORTED
#      define RECURSIVE_MUTEXES_SUPPORTED 1
#    endif
#  endif // linux
#  ifdef __SVR4
#    define CFG_OS_SOLARIS
#    ifndef RECURSIVE_MUTEXES_SUPPORTED
#      define RECURSIVE_MUTEXES_SUPPORTED 1
#    endif
#  endif // __SVR4
#  ifdef __CYGWIN__
#    define CFG_OS_CYGWIN
#    ifndef RECURSIVE_MUTEXES_SUPPORTED
#      define RECURSIVE_MUTEXES_SUPPORTED 1
#    endif
#  endif // __CYGWIN__
#endif // __GNUC__

#ifdef CFG_POSIX
#  include <unistd.h>
#  ifndef TRUE
#    define TRUE 1
#  endif
#  ifndef FALSE
#    define FALSE 0
#  endif
#  define STDCALL
#endif
#ifdef CFG_WIN32
//#  define WIN32_LEAN_AND_MEAN  // exclude rarely-used stuff from Windows headers
#  define VC_EXTRALEAN         // exclude rarely-used stuff from Windows headers
#  define _INC_MINMAX          // do not include <minmax.h>
#  define NOMINMAX             // disable min and max in <windef.h>
#  define STDCALL WINAPI
#endif // CFG_WIN32

#ifdef CFG_COMPILER_MSC_ANY
#  ifdef _AFXDLL
#    include <afxwin.h>
#  else
#    include <windows.h>
#  endif
#endif

#include <cstdlib>

#define COMPILE_TIME_CHECK_BEGIN if (false) {
#define COMPILE_TIME_CHECK_END }
#define COMPILE_TIME_CHECK(STATEMENT) COMPILE_TIME_CHECK_BEGIN STATEMENT COMPILE_TIME_CHECK_END

#define __STRING2__(x) #x
#define __STRING__(x) __STRING2__(x)
#define __LOCATION__ __FILE__ "("__STRING__(__LINE__)") : "
#define __WARNING__ __FILE__ "("__STRING__(__LINE__)") : warning: "

#endif //__Grom_System_Config_h__
