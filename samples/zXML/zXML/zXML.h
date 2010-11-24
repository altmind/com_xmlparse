#pragma once

#include "resource.h"
#define catchErrorMB(x) {x; if (GetLastError()>0) {MessageBoxExW(hWnd, printErrorA(GetLastError(),__FILE__,__LINE__) ,L"Error",0,0);}}
#define catchError(x) x; if (GetLastError()>0) {fwprintf(stderr, printErrorA(GetLastError(),__FILE__,__LINE__) ,L"Error",0,0);}

wchar_t* printErrorW(DWORD errorCode, LPWSTR file, DWORD line);
wchar_t* printErrorA(DWORD errorCode, LPSTR file, DWORD line);
