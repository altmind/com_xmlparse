// MathComponent.cpp : implementation file
//

#include "stdafx.h"
#include "mfc.h"
#include "MathComponent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMathComponent

IMPLEMENT_DYNCREATE(CMathComponent, CCmdTarget)

CMathComponent::CMathComponent()
{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	
	AfxOleLockApp();
}

CMathComponent::~CMathComponent()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}


void CMathComponent::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMathComponent, CCmdTarget)
	//{{AFX_MSG_MAP(CMathComponent)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMathComponent, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CMathComponent)
	DISP_FUNCTION(CMathComponent, "Add", Add, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CMathComponent, "Multiply", Multiply, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CMathComponent, "Subtract", Subtract, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CMathComponent, "Divide", Divide, VT_I4, VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMathComponent to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {C13D1F0C-93FA-11D0-A6C4-0000837E3100}
static const IID IID_IMathComponent =
{ 0xc13d1f0c, 0x93fa, 0x11d0, { 0xa6, 0xc4, 0x0, 0x0, 0x83, 0x7e, 0x31, 0x0 } };

BEGIN_INTERFACE_MAP(CMathComponent, CCmdTarget)
	INTERFACE_PART(CMathComponent, IID_IMathComponent, Dispatch)
END_INTERFACE_MAP()

// {C13D1F0D-93FA-11D0-A6C4-0000837E3100}
IMPLEMENT_OLECREATE(CMathComponent, "MFC.MATHCOMPONENT", 0xc13d1f0d, 0x93fa, 0x11d0, 0xa6, 0xc4, 0x0, 0x0, 0x83, 0x7e, 0x31, 0x0)

/////////////////////////////////////////////////////////////////////////////
// CMathComponent message handlers

long CMathComponent::Add(long lOp1, long lOp2) 
{
	return lOp1 + lOp2;
}

long CMathComponent::Multiply(long lOp1, long lOp2) 
{
	return lOp1 * lOp2;
}

long CMathComponent::Subtract(long lOp1, long lOp2) 
{
	return lOp1 - lOp2;
}

long CMathComponent::Divide(long lOp1, long lOp2) 
{
	return lOp1 / lOp2;
}
