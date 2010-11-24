

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Nov 19 17:22:57 2010
 */
/* Compiler settings for mfc.odl:
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


#ifndef __mfc_h_h__
#define __mfc_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMathComponent_FWD_DEFINED__
#define __IMathComponent_FWD_DEFINED__
typedef interface IMathComponent IMathComponent;
#endif 	/* __IMathComponent_FWD_DEFINED__ */


#ifndef __MFCMathComponent_FWD_DEFINED__
#define __MFCMathComponent_FWD_DEFINED__

#ifdef __cplusplus
typedef class MFCMathComponent MFCMathComponent;
#else
typedef struct MFCMathComponent MFCMathComponent;
#endif /* __cplusplus */

#endif 	/* __MFCMathComponent_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Mfc_LIBRARY_DEFINED__
#define __Mfc_LIBRARY_DEFINED__

/* library Mfc */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Mfc,0xC13D1F09,0x93FA,0x11D0,0xA6,0xC4,0x00,0x00,0x83,0x7E,0x31,0x00);

#ifndef __IMathComponent_DISPINTERFACE_DEFINED__
#define __IMathComponent_DISPINTERFACE_DEFINED__

/* dispinterface IMathComponent */
/* [uuid] */ 


DEFINE_GUID(DIID_IMathComponent,0xC13D1F0C,0x93FA,0x11D0,0xA6,0xC4,0x00,0x00,0x83,0x7E,0x31,0x00);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C13D1F0C-93FA-11D0-A6C4-0000837E3100")
    IMathComponent : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMathComponentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMathComponent * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMathComponent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMathComponent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMathComponent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMathComponent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMathComponent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMathComponent * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IMathComponentVtbl;

    interface IMathComponent
    {
        CONST_VTBL struct IMathComponentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMathComponent_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMathComponent_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMathComponent_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMathComponent_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMathComponent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMathComponent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMathComponent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMathComponent_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MFCMathComponent,0xC13D1F0D,0x93FA,0x11D0,0xA6,0xC4,0x00,0x00,0x83,0x7E,0x31,0x00);

#ifdef __cplusplus

class DECLSPEC_UUID("C13D1F0D-93FA-11D0-A6C4-0000837E3100")
MFCMathComponent;
#endif
#endif /* __Mfc_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


