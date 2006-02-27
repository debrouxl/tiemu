/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Feb 27 16:44:59 2006
 */
/* Compiler settings for oleaut.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __oleaut_h__
#define __oleaut_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ITiEmuOLE_FWD_DEFINED__
#define __ITiEmuOLE_FWD_DEFINED__
typedef interface ITiEmuOLE ITiEmuOLE;
#endif 	/* __ITiEmuOLE_FWD_DEFINED__ */


#ifndef __TiEmuOLE_FWD_DEFINED__
#define __TiEmuOLE_FWD_DEFINED__

#ifdef __cplusplus
typedef class TiEmuOLE TiEmuOLE;
#else
typedef struct TiEmuOLE TiEmuOLE;
#endif /* __cplusplus */

#endif 	/* __TiEmuOLE_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_oleaut_0000 */
/* [local] */ 

typedef /* [public] */ 
enum __MIDL___MIDL_itf_oleaut_0000_0001
    {	TIEMU_CALC_TI92	= 1,
	TIEMU_CALC_TI89	= 2,
	TIEMU_CALC_TI92p	= 4,
	TIEMU_CALC_V200	= 8,
	TIEMU_CALC_TI89t	= 16
    }	TiEmuCalcModels;



extern RPC_IF_HANDLE __MIDL_itf_oleaut_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_oleaut_0000_v0_0_s_ifspec;

#ifndef __ITiEmuOLE_INTERFACE_DEFINED__
#define __ITiEmuOLE_INTERFACE_DEFINED__

/* interface ITiEmuOLE */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITiEmuOLE;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e9373bd9-7363-427f-a2a6-1e8ba91ffb3e")
    ITiEmuOLE : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE image_loaded( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE emulated_calc_type( 
            /* [retval][out] */ int __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE emulated_hw_version( 
            /* [retval][out] */ int __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE emulated_os_version( 
            /* [retval][out] */ BSTR __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ready_for_transfers( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE send_file( 
            /* [in] */ BSTR filename,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE debug_file( 
            /* [in] */ BSTR filename,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE reset_calc( 
            /* [in] */ VARIANT_BOOL clearmem,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE execute_command( 
            /* [in] */ BSTR command,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE turn_calc_on( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE enter_debugger( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITiEmuOLEVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITiEmuOLE __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITiEmuOLE __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *image_loaded )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *emulated_calc_type )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *emulated_hw_version )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *emulated_os_version )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ready_for_transfers )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *send_file )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [in] */ BSTR filename,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *debug_file )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [in] */ BSTR filename,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *reset_calc )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL clearmem,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *execute_command )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [in] */ BSTR command,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *turn_calc_on )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *enter_debugger )( 
            ITiEmuOLE __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);
        
        END_INTERFACE
    } ITiEmuOLEVtbl;

    interface ITiEmuOLE
    {
        CONST_VTBL struct ITiEmuOLEVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITiEmuOLE_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITiEmuOLE_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITiEmuOLE_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITiEmuOLE_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITiEmuOLE_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITiEmuOLE_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITiEmuOLE_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITiEmuOLE_image_loaded(This,ret)	\
    (This)->lpVtbl -> image_loaded(This,ret)

#define ITiEmuOLE_emulated_calc_type(This,ret)	\
    (This)->lpVtbl -> emulated_calc_type(This,ret)

#define ITiEmuOLE_emulated_hw_version(This,ret)	\
    (This)->lpVtbl -> emulated_hw_version(This,ret)

#define ITiEmuOLE_emulated_os_version(This,ret)	\
    (This)->lpVtbl -> emulated_os_version(This,ret)

#define ITiEmuOLE_ready_for_transfers(This,ret)	\
    (This)->lpVtbl -> ready_for_transfers(This,ret)

#define ITiEmuOLE_send_file(This,filename,ret)	\
    (This)->lpVtbl -> send_file(This,filename,ret)

#define ITiEmuOLE_debug_file(This,filename,ret)	\
    (This)->lpVtbl -> debug_file(This,filename,ret)

#define ITiEmuOLE_reset_calc(This,clearmem,ret)	\
    (This)->lpVtbl -> reset_calc(This,clearmem,ret)

#define ITiEmuOLE_execute_command(This,command,ret)	\
    (This)->lpVtbl -> execute_command(This,command,ret)

#define ITiEmuOLE_turn_calc_on(This,ret)	\
    (This)->lpVtbl -> turn_calc_on(This,ret)

#define ITiEmuOLE_enter_debugger(This,ret)	\
    (This)->lpVtbl -> enter_debugger(This,ret)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ITiEmuOLE_image_loaded_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_image_loaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_emulated_calc_type_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_emulated_calc_type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_emulated_hw_version_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_emulated_hw_version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_emulated_os_version_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_emulated_os_version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_ready_for_transfers_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_ready_for_transfers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_send_file_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [in] */ BSTR filename,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_send_file_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_debug_file_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [in] */ BSTR filename,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_debug_file_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_reset_calc_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL clearmem,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_reset_calc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_execute_command_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [in] */ BSTR command,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_execute_command_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_turn_calc_on_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_turn_calc_on_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITiEmuOLE_enter_debugger_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret);


void __RPC_STUB ITiEmuOLE_enter_debugger_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITiEmuOLE_INTERFACE_DEFINED__ */



#ifndef __TiEmuOLELib_LIBRARY_DEFINED__
#define __TiEmuOLELib_LIBRARY_DEFINED__

/* library TiEmuOLELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TiEmuOLELib;

EXTERN_C const CLSID CLSID_TiEmuOLE;

#ifdef __cplusplus

class DECLSPEC_UUID("b2a17b13-9d6f-4dd4-a2a9-6fe06adc1d33")
TiEmuOLE;
#endif
#endif /* __TiEmuOLELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
