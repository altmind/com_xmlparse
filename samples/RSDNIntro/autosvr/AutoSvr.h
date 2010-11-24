

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Nov 19 21:27:39 2010
 */
/* Compiler settings for AutoSvr.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __AutoSvr_h__
#define __AutoSvr_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMath_FWD_DEFINED__
#define __IMath_FWD_DEFINED__
typedef interface IMath IMath;
#endif 	/* __IMath_FWD_DEFINED__ */


#ifndef __MathComponent_FWD_DEFINED__
#define __MathComponent_FWD_DEFINED__

#ifdef __cplusplus
typedef class MathComponent MathComponent;
#else
typedef struct MathComponent MathComponent;
#endif /* __cplusplus */

#endif 	/* __MathComponent_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IMath_INTERFACE_DEFINED__
#define __IMath_INTERFACE_DEFINED__

/* interface IMath */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMath;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8C30BC10-B8F2-11D0-A756-B04A12000000")
    IMath : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ long __MIDL__IMath0000,
            /* [in] */ long __MIDL__IMath0001,
            /* [retval][out] */ long *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Subtract( 
            /* [in] */ long __MIDL__IMath0002,
            /* [in] */ long __MIDL__IMath0003,
            /* [retval][out] */ long *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Multiply( 
            /* [in] */ long __MIDL__IMath0004,
            /* [in] */ long __MIDL__IMath0005,
            /* [retval][out] */ long *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Divide( 
            /* [in] */ long __MIDL__IMath0006,
            /* [in] */ long __MIDL__IMath0007,
            /* [retval][out] */ long *pResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMathVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMath * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMath * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMath * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMath * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMath * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMath * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMath * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *Add )( 
            IMath * This,
            /* [in] */ long __MIDL__IMath0000,
            /* [in] */ long __MIDL__IMath0001,
            /* [retval][out] */ long *pResult);
        
        HRESULT ( STDMETHODCALLTYPE *Subtract )( 
            IMath * This,
            /* [in] */ long __MIDL__IMath0002,
            /* [in] */ long __MIDL__IMath0003,
            /* [retval][out] */ long *pResult);
        
        HRESULT ( STDMETHODCALLTYPE *Multiply )( 
            IMath * This,
            /* [in] */ long __MIDL__IMath0004,
            /* [in] */ long __MIDL__IMath0005,
            /* [retval][out] */ long *pResult);
        
        HRESULT ( STDMETHODCALLTYPE *Divide )( 
            IMath * This,
            /* [in] */ long __MIDL__IMath0006,
            /* [in] */ long __MIDL__IMath0007,
            /* [retval][out] */ long *pResult);
        
        END_INTERFACE
    } IMathVtbl;

    interface IMath
    {
        CONST_VTBL struct IMathVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMath_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMath_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMath_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMath_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMath_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMath_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMath_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMath_Add(This,__MIDL__IMath0000,__MIDL__IMath0001,pResult)	\
    ( (This)->lpVtbl -> Add(This,__MIDL__IMath0000,__MIDL__IMath0001,pResult) ) 

#define IMath_Subtract(This,__MIDL__IMath0002,__MIDL__IMath0003,pResult)	\
    ( (This)->lpVtbl -> Subtract(This,__MIDL__IMath0002,__MIDL__IMath0003,pResult) ) 

#define IMath_Multiply(This,__MIDL__IMath0004,__MIDL__IMath0005,pResult)	\
    ( (This)->lpVtbl -> Multiply(This,__MIDL__IMath0004,__MIDL__IMath0005,pResult) ) 

#define IMath_Divide(This,__MIDL__IMath0006,__MIDL__IMath0007,pResult)	\
    ( (This)->lpVtbl -> Divide(This,__MIDL__IMath0006,__MIDL__IMath0007,pResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMath_INTERFACE_DEFINED__ */



#ifndef __AUTOSVRLib_LIBRARY_DEFINED__
#define __AUTOSVRLib_LIBRARY_DEFINED__

/* library AUTOSVRLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_AUTOSVRLib;

EXTERN_C const CLSID CLSID_MathComponent;

#ifdef __cplusplus

class DECLSPEC_UUID("8C30BC11-B8F2-11D0-A756-B04A12000000")
MathComponent;
#endif
#endif /* __AUTOSVRLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


