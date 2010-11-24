// Free open-source Grom library

#include "stdafx.h"
#ifndef __Grom_System_Exception_h__
#define __Grom_System_Exception_h__

#include "Config.h"

#include <exception>
#include <string>
#include "FundamentalTypes.h"
#include "StringUtils.h"
#include "ErrorMessages.h"

#ifdef _DEBUG
#  if defined(CFG_COMPILER_MSC_ANY) && !defined(CFG_COMPILER_MSC_60_1998)
#    define ERROR_LOCATION (__FUNCTION__)
#  else
#    define ERROR_LOCATION (__FILE__)
#  endif
#else
#  define ERROR_LOCATION ("")
#endif

namespace Sys
{

const int EXTERNAL_ERROR = -100;

// The Exception class is the base class of exceptions. Exceptions
// are intended to be created dynamically on the heap in order to
// allow exception chaining.

class Exception : public std::exception
{
public:
  Exception(const std::string& message);
  Exception(const std::string& message, Exception* innerException);
  virtual ~Exception() throw(); // override
  const char* Message() const;
  const Exception* InnerException() const;
  const char* ClassName() const;

private:
  virtual const char* what() const throw(); // override

  std::string m_Message;
  Exception* m_InnerException;
};

class SystemException : public Exception
{
public:
  SystemException(const std::string& message);
  SystemException(const std::string& message, Exception* innerException);
};

class RtlException : public SystemException
{
public:
  RtlException(int errNo, const std::string& message);
  RtlException(int errNo, const std::string& message, Exception* innerException);

  int ErrNo();

private:
  int m_ErrNo;
};

class ApplicationException : public Exception
{
public:
  ApplicationException(int errorCode, const std::string& message);
  ApplicationException(int errorCode, const std::string& message, Exception* innerException);
  int ErrorCode() const;

private:
  int m_ErrorCode;
};

class InvalidCastException : public SystemException
{
public:
  InvalidCastException(const std::string& message);
  InvalidCastException(const std::string& message, Exception* innerException);
};

class ArgumentException : public SystemException
{
public:
  ArgumentException(const std::string& message);
  ArgumentException(const std::string& message, Exception* innerException);
};

class IOException : public SystemException
{
public:
  IOException(const std::string& message);
  IOException(const std::string& message, Exception* innerException);
};

class ExternalException : public SystemException
{
public:
  ExternalException(const std::string& message);
  ExternalException(const std::string& message, Exception* innerException);
};

class ExceptionString : public FormattedString
{
public:
  ExceptionString(const Exception* e, const char* delimiter = "\n");

private:
  void DoFormat(const Exception* e, const char* delimiter);
};

// Exception inlines

inline
const char* Exception::Message() const
{
  return what();
}

inline
const Exception* Exception::InnerException() const
{
  return m_InnerException;
}

inline
int ApplicationException::ErrorCode() const
{
  return m_ErrorCode;
}

// RtlException inlines

inline
int RtlException::ErrNo()
{
  return m_ErrNo;
}

// Utilities

SystemException* CreateNotImplementedException();
RtlException* CreateRtlException(const char* hint);

#define NOT_IMPLEMENTED    \
  throw Sys::CreateNotImplementedException();

InvalidCastException* CreateInvalidCastException(const char* from, const char* to);
ArgumentException* CreateIndexOutOfBoundsException(int index);
ArgumentException* CreateRangeOutOfBoundsException(int index, int count);
ArgumentException* CreateArgumentNullException();
ArgumentException* CreateArgumentOutOfRangeException();
ArgumentException* CreateItemCountNegativeException(int value);

#ifdef CFG_WIN32
void ThrowWin32FileException(const char* filePath);
#endif

#ifndef CFG_WIN32
#  define HRESULT int
#endif

// On Windows platform the CheckSafeCallResult function checks for the
// result code after a call to safe-call function. If result code indicates
// a COM error, then the SafeCallError function is called to throw C++
// exception, which is created from the COM error object. On Unix platform
// the function simply returns an argument.
// Example:
//   bool DoSomething()
//   {
//     bool result;
//     CheckSafeCallResult(SafecallDoSomething(&result));
//     return result;
//   }
HRESULT CheckSafeCallResult(HRESULT resultCode);

HRESULT ConvertExceptionToComError(Exception* e, const char* source = NULL);

#ifdef CFG_WIN32
ExternalException* CreateExternalException();
void CreateComErrorInfo(const char* description, const char* source = NULL,
  const char* helpFile = NULL, UInt32 helpContext = 0);
#endif // CFG_WIN32

// BEGIN_SAFECALL and END_SAFECALL macros should be used to wrap function
// bodies which use "safecall" calling convention. Safecall calling convention
// means that the function returns error code as its result and uses COM
// ErrorInfo mechanism to pass exception information to the caller.
// Example:
//   HRESULT SafecallDoSomething(bool* result)
//   {
//     BEGIN_SAFECALL
//       result = DoSomethingImpl();
//     END_SAFECALL
//   }
#define BEGIN_SAFECALL \
  try\
  {

#define END_SAFECALL \
  }\
  catch (Sys::Exception* e)\
  {\
    return Sys::ConvertExceptionToComError(e, ERROR_LOCATION);\
  }\
  catch (...)\
  {\
    return Sys::ConvertExceptionToComError(new Sys::Exception(\
      Sys::ERRMSG_UNKNOWN_INTERNAL_ERROR), ERROR_LOCATION);\
  }\
  return S_OK;

} // namespace Sys

#endif //__Grom_System_Exception_h__
