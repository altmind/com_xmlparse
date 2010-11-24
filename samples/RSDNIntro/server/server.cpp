//
// server.cpp : Defines the initialization routines for the DLL.
//

#include <windows.h>

#include <initguid.h>
#include "math.h"

long    g_lObjs = 0;
long    g_lLocks = 0;

STDAPI DllGetClassObject( REFCLSID rclsid, REFIID riid, void** ppv )
{
   HRESULT             hr;
   MathClassFactory    *pCF;

   pCF = 0;

   // Make sure the CLSID is for our Expression component
   if ( rclsid != CLSID_Math )
      return( E_FAIL );

   pCF = new MathClassFactory;

   if ( pCF == 0 )
      return( E_OUTOFMEMORY );

   hr = pCF->QueryInterface( riid, ppv );

   // Check for failure of QueryInterface
   if ( FAILED( hr ) )
   {
      delete pCF;
      pCF = 0;
   }

   return hr;
}

STDAPI DllCanUnloadNow(void)
{
    if ( g_lObjs || g_lLocks )
       return( S_FALSE );
    else
       return( S_OK );
}
