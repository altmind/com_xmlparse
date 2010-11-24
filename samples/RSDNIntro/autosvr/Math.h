// Math.h : Declaration of the CMath

#ifndef __MATH_H_
#define __MATH_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMath
class ATL_NO_VTABLE CMath : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMath, &CLSID_MathComponent>,
	public IDispatchImpl<IMath, &IID_IMath, &LIBID_AUTOSVRLib>
{
public:
	CMath()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MATH)

BEGIN_COM_MAP(CMath)
	COM_INTERFACE_ENTRY(IMath)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IMath
public:
   STDMETHOD(Add)( long, long, long* );
   STDMETHOD(Subtract)( long, long, long* );
   STDMETHOD(Multiply)( long, long, long* );
   STDMETHOD(Divide)( long, long, long* );
};

#endif //__MATH_H_
