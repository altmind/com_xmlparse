//
// Math.cpp
//

#include <windows.h>
#include "math.h"

//
// Math class implementation
//
// Constructors
Math::Math()
{
   m_lRef = 0;

   // Increment the global object count
   InterlockedIncrement( &g_lObjs ); 
}

// The destructor
Math::~Math()
{
   // Decrement the global object count
   InterlockedDecrement( &g_lObjs ); 
}

STDMETHODIMP Math::QueryInterface( REFIID riid, void** ppv )
{
    *ppv = 0;

    if ( riid == IID_IUnknown || riid == IID_IMath )
        *ppv = this;

    if ( *ppv )
    {
        AddRef();
        return( S_OK );
    }
    return (E_NOINTERFACE);
}

STDMETHODIMP_(ULONG) Math::AddRef()
{
   return InterlockedIncrement( &m_lRef );
}

STDMETHODIMP_(ULONG) Math::Release()
{
   if ( InterlockedDecrement( &m_lRef ) == 0 )
   {
      delete this;
      return 0;
   }

   return m_lRef;
}

STDMETHODIMP Math::Add( long lOp1, long lOp2, long* pResult )
{
   *pResult = lOp1 + lOp2;
   return S_OK;
}

STDMETHODIMP Math::Subtract( long lOp1, long lOp2, long* pResult )
{
   *pResult = lOp1 - lOp2;
   return S_OK;
}

STDMETHODIMP Math::Multiply( long lOp1, long lOp2, long* pResult )
{
   *pResult = lOp1 * lOp2;
   return S_OK;
}

STDMETHODIMP_(long) Math::Divide( long lOp1, long lOp2, long* pResult )
{
   *pResult = lOp1 / lOp2;
   return S_OK;
}

MathClassFactory::MathClassFactory()
{
    m_lRef = 0;
}

MathClassFactory::~MathClassFactory()
{
}

STDMETHODIMP MathClassFactory::QueryInterface( REFIID riid, void** ppv )
{
   *ppv = 0;

   if ( riid == IID_IUnknown || riid == IID_IClassFactory )
      *ppv = this;

   if ( *ppv )
   {
      AddRef();
      return S_OK;
   }

   return(E_NOINTERFACE);
}

STDMETHODIMP_(ULONG) MathClassFactory::AddRef()
{
   return InterlockedIncrement( &m_lRef );
}

STDMETHODIMP_(ULONG) MathClassFactory::Release()
{
   if ( InterlockedDecrement( &m_lRef ) == 0 )
   {
      delete this;
      return 0;
   }

   return m_lRef;
}

STDMETHODIMP MathClassFactory::CreateInstance
     ( LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj )
{
   Math*      pMath;
   HRESULT    hr;

   *ppvObj = 0;

   pMath = new Math;

   if ( pMath == 0 )
      return( E_OUTOFMEMORY );

   hr = pMath->QueryInterface( riid, ppvObj );

   if ( FAILED( hr ) )
      delete pMath;

   return hr;
}

STDMETHODIMP MathClassFactory::LockServer( BOOL fLock )
{
   if ( fLock )
      InterlockedIncrement( &g_lLocks ); 
   else
      InterlockedDecrement( &g_lLocks );

    return S_OK;
}

