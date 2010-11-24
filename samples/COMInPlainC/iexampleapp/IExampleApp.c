// This is a C example that tests the IExample COM component (in IExample.dll).

#include <windows.h>
#include <objbase.h>
#include <stdio.h>
#include "../IExample/IExample.h"

int main(int argc, char **argv)
{
	IExample		*exampleObj;
	IClassFactory	*classFactory;
	HRESULT			hr;

	// We must initialize OLE before we do anything with COM objects. NOTE:
	// some COM components, such as the IE browser engine, require that you
	// use OleInitialize() instead. But our COM component doesn't require this
	if (!CoInitialize(0))
	{
		// Get IExample.DLL's IClassFactory
		if ((hr = CoGetClassObject(&CLSID_IExample, CLSCTX_INPROC_SERVER, 0, &IID_IClassFactory, &classFactory)))
			MessageBox(0, "Can't get IClassFactory", "CoGetClassObject error", MB_OK|MB_ICONEXCLAMATION);
		else
		{
			// Create an IExample object
			if ((hr = classFactory->lpVtbl->CreateInstance(classFactory, 0, &IID_IExample, &exampleObj)))
			{
				classFactory->lpVtbl->Release(classFactory);
				MessageBox(0, "Can't create IExample object", "CreateInstance error", MB_OK|MB_ICONEXCLAMATION);
			}
			else
			{
				char	buffer[80];

				// Release the IClassFactory. We don't need it now that we have the one
				// IExample we want
				classFactory->lpVtbl->Release(classFactory);

				// Call SetString to set the text "Hello world!"
				exampleObj->lpVtbl->SetString(exampleObj, "Hello world!");

				// Retrieve the string to make sure we get "Hello world!"
				exampleObj->lpVtbl->GetString(exampleObj, buffer, sizeof(buffer));

				printf("string = %s (should be Hello World!)\n", buffer);

				// Release the IExample now that we're done with it
				exampleObj->lpVtbl->Release(exampleObj);
			}
		}

		// When finally done with OLE, free it
		CoUninitialize();
	}
	else
		MessageBox(0, "Can't initialize COM", "CoInitialize error", MB_OK|MB_ICONEXCLAMATION);

	return(0);
}
