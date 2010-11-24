// MathComponent.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CMathComponent command target

class CMathComponent : public CCmdTarget
{
	DECLARE_DYNCREATE(CMathComponent)

	CMathComponent();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMathComponent)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMathComponent();

	// Generated message map functions
	//{{AFX_MSG(CMathComponent)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CMathComponent)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMathComponent)
	afx_msg long Add(long lOp1, long lOp2);
	afx_msg long Multiply(long lOp1, long lOp2);
	afx_msg long Subtract(long lOp1, long lOp2);
	afx_msg long Divide(long lOp1, long lOp2);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
