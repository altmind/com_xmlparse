//
// Client.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream>

#include <initguid.h>
#include "..\server\imath.h"

using namespace std;

int main( int argc, char *argv[] )
{
   cout << "Initializing COM" << endl;

   if ( FAILED( CoInitialize( NULL )))
   {
      cout << "Unable to initialize COM" << endl;
      return -1;
   }

   char* szProgID = "Math.Component.1";
   WCHAR  szWideProgID[128];
   CLSID  clsid;
   long lLen = MultiByteToWideChar( CP_ACP,
                        0,
                        szProgID,
                        strlen( szProgID ),
                        szWideProgID,
                        sizeof( szWideProgID ) );

   szWideProgID[ lLen ] = '\0';
   HRESULT hr = ::CLSIDFromProgID( szWideProgID, &clsid );
   if ( FAILED( hr ))
   {
      cout.setf( ios::hex, ios::basefield );
      cout << "Unable to get CLSID from ProgID. HR = " << hr << endl;
      return -1;
   }

   IClassFactory* pCF;
   // Get the class factory for the Math class
   hr = CoGetClassObject( clsid,
                          CLSCTX_INPROC,
                          NULL,
                          IID_IClassFactory,
                          (void**) &pCF );
   if ( FAILED( hr ))
   {
      cout.setf( ios::hex, ios::basefield );
      cout << "Failed to GetClassObject server instance. HR = " << hr << endl;
      return -1;
   }

   // using the class factory interface create an instance of the
   // component and return the IExpression interface.
   IUnknown* pUnk;
   hr = pCF->CreateInstance( NULL, IID_IUnknown, (void**) &pUnk );

   // Release the class factory
   pCF->Release();

   if ( FAILED( hr ))
   {
      cout.setf( ios::hex, ios::basefield );
      cout << "Failed to create server instance. HR = " << hr << endl;
      return -1;
   }

   cout << "Instance created" << endl;

   IMath* pMath = NULL;
   hr = pUnk->QueryInterface( IID_IMath, (LPVOID*)&pMath );
   pUnk->Release();
   if ( FAILED( hr ))
   {
      cout << "QueryInterface() for IMath failed" << endl;
      return -1;
   }

   long result;
   pMath->Multiply( 100, 8, &result );
   cout << "100 * 8 is " << result << endl;

   pMath->Subtract( 1000, 333, &result );
   cout << "1000 - 333 is " << result << endl;

   cout << "Releasing instance" << endl;
   pMath->Release();

   cout << "Shuting down COM" << endl;
   CoUninitialize();

   scanf("");
   return 0;
}
