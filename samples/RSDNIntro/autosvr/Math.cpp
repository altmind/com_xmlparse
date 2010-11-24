//
// Math.cpp : Implementation of CMath
//

#include "stdafx.h"
#include "AutoSvr.h"
#include "Math.h"

//////////////////
// CMath
//////////////////
STDMETHODIMP CMath::Add( long op1, long op2, long* pResult )
{
   *pResult = op1 + op2;
   return S_OK;
}

STDMETHODIMP CMath::Subtract( long op1, long op2, long* pResult )
{
   *pResult = op1 - op2;
   return S_OK;
}

STDMETHODIMP CMath::Multiply( long op1, long op2, long* pResult )
{
   *pResult = op1 * op2;
   return S_OK;
}

STDMETHODIMP CMath::Divide( long op1, long op2, long* pResult )
{
   *pResult = op1 / op2;
   return S_OK;
}

