

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sun Dec 27 00:13:44 2015
 */
/* Compiler settings for packageCap20151226.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
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


#ifndef __packageCap20151226_h_h__
#define __packageCap20151226_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IpackageCap20151226_FWD_DEFINED__
#define __IpackageCap20151226_FWD_DEFINED__
typedef interface IpackageCap20151226 IpackageCap20151226;

#endif 	/* __IpackageCap20151226_FWD_DEFINED__ */


#ifndef __packageCap20151226_FWD_DEFINED__
#define __packageCap20151226_FWD_DEFINED__

#ifdef __cplusplus
typedef class packageCap20151226 packageCap20151226;
#else
typedef struct packageCap20151226 packageCap20151226;
#endif /* __cplusplus */

#endif 	/* __packageCap20151226_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __packageCap20151226_LIBRARY_DEFINED__
#define __packageCap20151226_LIBRARY_DEFINED__

/* library packageCap20151226 */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_packageCap20151226;

#ifndef __IpackageCap20151226_DISPINTERFACE_DEFINED__
#define __IpackageCap20151226_DISPINTERFACE_DEFINED__

/* dispinterface IpackageCap20151226 */
/* [uuid] */ 


EXTERN_C const IID DIID_IpackageCap20151226;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6420E762-D6D8-434D-8977-1ECA80CEA9AC")
    IpackageCap20151226 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IpackageCap20151226Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IpackageCap20151226 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IpackageCap20151226 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IpackageCap20151226 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IpackageCap20151226 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IpackageCap20151226 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IpackageCap20151226 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IpackageCap20151226 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IpackageCap20151226Vtbl;

    interface IpackageCap20151226
    {
        CONST_VTBL struct IpackageCap20151226Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IpackageCap20151226_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IpackageCap20151226_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IpackageCap20151226_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IpackageCap20151226_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IpackageCap20151226_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IpackageCap20151226_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IpackageCap20151226_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IpackageCap20151226_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_packageCap20151226;

#ifdef __cplusplus

class DECLSPEC_UUID("F52E4DB7-08D9-4099-91CA-CD5A9ACB1E9B")
packageCap20151226;
#endif
#endif /* __packageCap20151226_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


