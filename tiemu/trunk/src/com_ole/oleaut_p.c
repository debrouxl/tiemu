/* oleaut_p.c edited by Kevin Kofler to build with MinGW
 *
 *  OLE Automation interface for TiEmu
 *
 *  Copyright (c) 2006 Kevin Kofler
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Feb 03 11:12:41 2006
 */
/* Compiler settings for oleaut.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )

#include "rpcproxy.h"

#include "oleaut.h"

#ifdef __MINGW32__
#include <excpt.h>
/* CAUTION: Ugly SEH hacks (technically invalid inline ASM) below!
            Use -O0 -fno-omit-frame-pointer to minimize breakage! */
#define RpcTryExcept \
  { \
    asm volatile ("pushl %esp; pushl %ebp; pushl $100f\n" \
                  "pushl %fs:0; movl %esp,%fs:0; subl $32,%esp");
#define RpcExcept(guard) \
    asm volatile ("movl %fs:0,%esp; popl %fs:0; addl $12,%esp"); \
  } \
  asm volatile ("jmp 101f\n" \
                "100: pushl %ebp; movl %fs:0,%eax; movl 8(%eax),%ebp; movl %esp,8(%eax)"); \
  if ((guard)) { \
    volatile int _exception_code; \
    asm volatile ("movl 8(%%esp),%%eax; movl %%fs:0,%%esp; popl %%fs:0; addl $8,%%esp; popl %%esp; movl %%eax,%0":"=m"(_exception_code)::"eax");
#define RpcEndExcept \
  } else { \
    asm volatile ("movl %fs:0,%eax; movl 8(%eax),%esp; movl %ebp,8(%eax); popl %ebp; movl $1,%eax; ret");\
  } \
  asm volatile ("101:");
#define RpcTryFinally \
  { \
    asm volatile ("pushl $0; pushl %esp; pushl %ebp; pushl $200f\n" \
                  "pushl %fs:0; movl %esp,%fs:0; subl $32,%esp");
#define RpcFinally \
  } \
  asm volatile ("movl %fs:0,%eax; jmp 201f\n" \
                "200: movl %fs:0,%eax; movl $1,16(%eax)\n" \
                "201: pushl %ebp; movl 8(%eax),%ebp; movl %esp,8(%eax)");
#define RpcEndFinally \
  asm volatile ("movl %%fs:0,%%eax; movl 8(%%eax),%%esp; movl %%ebp,8(%%eax); popl %%ebp; movl 16(%%eax),%%eax; testl %%eax,%%eax; je 202f; ret\n" \
                "202: movl %%fs:0,%%esp; popl %%fs:0; addl $16,%%esp":::"eax");
#define RpcExceptionCode() (_exception_code)
#ifndef CINTERFACE_PROXY_VTABLE
#define CINTERFACE_PROXY_VTABLE(n) \
  struct \
  { \
    CInterfaceProxyHeader header; \
    void *Vtbl[n]; \
  }
#endif

#endif

#define TYPE_FORMAT_STRING_SIZE   63                                
#define PROC_FORMAT_STRING_SIZE   37                                

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Standard interface: __MIDL_itf_oleaut_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ITiEmuOLE, ver. 0.0,
   GUID={0xe9373bd9,0x7363,0x427f,{0xa2,0xa6,0x1e,0x8b,0xa9,0x1f,0xfb,0x3e}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

HRESULT STDMETHODCALLTYPE ITiEmuOLE_image_loaded_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0] );
            
            *ret = *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++;
            
            _StubMsg.Buffer += 2;
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[2],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_image_loaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    VARIANT_BOOL _M0;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    VARIANT_BOOL __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( VARIANT_BOOL __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        ret = &_M0;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> image_loaded((ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 2U + 6U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++ = *ret;
        
        _StubMsg.Buffer += 2;
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT STDMETHODCALLTYPE ITiEmuOLE_emulated_calc_type_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[6] );
            
            *ret = *(( int __RPC_FAR * )_StubMsg.Buffer)++;
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[6],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_emulated_calc_type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    int _M1;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    int __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( int __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        ret = &_M1;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> emulated_calc_type((ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U + 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( int __RPC_FAR * )_StubMsg.Buffer)++ = *ret;
        
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT STDMETHODCALLTYPE ITiEmuOLE_emulated_hw_version_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      9);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[6] );
            
            *ret = *(( int __RPC_FAR * )_StubMsg.Buffer)++;
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[6],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_emulated_hw_version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    int _M2;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    int __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( int __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        ret = &_M2;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> emulated_hw_version((ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U + 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( int __RPC_FAR * )_StubMsg.Buffer)++ = *ret;
        
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT STDMETHODCALLTYPE ITiEmuOLE_emulated_os_version_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(ret)
        {
        MIDL_memset(
               ret,
               0,
               sizeof( BSTR  ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      10);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[12] );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&ret,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[38],
                                      (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[10],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_emulated_os_version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    BSTR _M3;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    BSTR __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( BSTR __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        ret = &_M3;
        MIDL_memset(
               ret,
               0,
               sizeof( BSTR  ));
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> emulated_os_version((ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 8U + 11U;
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)ret,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[38] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)ret,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[38] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)ret,
                        &__MIDL_TypeFormatString.Format[10] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT STDMETHODCALLTYPE ITiEmuOLE_ready_for_transfers_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      11);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0] );
            
            *ret = *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++;
            
            _StubMsg.Buffer += 2;
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[2],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_ready_for_transfers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    VARIANT_BOOL _M4;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    VARIANT_BOOL __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( VARIANT_BOOL __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        ret = &_M4;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ready_for_transfers((ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 2U + 6U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++ = *ret;
        
        _StubMsg.Buffer += 2;
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT STDMETHODCALLTYPE ITiEmuOLE_send_file_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [in] */ BSTR filename,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      12);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&filename,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[52] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&filename,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[52] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[18] );
            
            *ret = *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++;
            
            _StubMsg.Buffer += 2;
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[2],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_send_file_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    VARIANT_BOOL _M5;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_filename;
    BSTR filename;
    VARIANT_BOOL __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_filename = &filename;
    MIDL_memset(
               _p_filename,
               0,
               sizeof( BSTR  ));
    ( VARIANT_BOOL __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[18] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_filename,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[52],
                                  (unsigned char)0 );
        
        ret = &_M5;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> send_file(
             (ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,
             filename,
             ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 2U + 6U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++ = *ret;
        
        _StubMsg.Buffer += 2;
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&filename,
                            &__MIDL_TypeFormatString.Format[52] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT STDMETHODCALLTYPE ITiEmuOLE_debug_file_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [in] */ BSTR filename,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      13);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&filename,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[52] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&filename,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[52] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[18] );
            
            *ret = *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++;
            
            _StubMsg.Buffer += 2;
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[2],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_debug_file_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    VARIANT_BOOL _M6;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_filename;
    BSTR filename;
    VARIANT_BOOL __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_filename = &filename;
    MIDL_memset(
               _p_filename,
               0,
               sizeof( BSTR  ));
    ( VARIANT_BOOL __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[18] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_filename,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[52],
                                  (unsigned char)0 );
        
        ret = &_M6;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> debug_file(
              (ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,
              filename,
              ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 2U + 6U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++ = *ret;
        
        _StubMsg.Buffer += 2;
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&filename,
                            &__MIDL_TypeFormatString.Format[52] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT STDMETHODCALLTYPE ITiEmuOLE_reset_calc_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL clearmem,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      14);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 2U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++ = clearmem;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[28] );
            
            *ret = *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++;
            
            _StubMsg.Buffer += 2;
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[2],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_reset_calc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    VARIANT_BOOL _M7;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    VARIANT_BOOL clearmem;
    VARIANT_BOOL __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( VARIANT_BOOL __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[28] );
        
        clearmem = *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++;
        
        ret = &_M7;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> reset_calc(
              (ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,
              clearmem,
              ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 2U + 6U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++ = *ret;
        
        _StubMsg.Buffer += 2;
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT STDMETHODCALLTYPE ITiEmuOLE_execute_command_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [in] */ BSTR command,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      15);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&command,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[52] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&command,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[52] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[18] );
            
            *ret = *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++;
            
            _StubMsg.Buffer += 2;
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[2],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_execute_command_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    VARIANT_BOOL _M8;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_command;
    BSTR command;
    VARIANT_BOOL __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_command = &command;
    MIDL_memset(
               _p_command,
               0,
               sizeof( BSTR  ));
    ( VARIANT_BOOL __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[18] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_command,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[52],
                                  (unsigned char)0 );
        
        ret = &_M8;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> execute_command(
                   (ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,
                   command,
                   ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 2U + 6U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++ = *ret;
        
        _StubMsg.Buffer += 2;
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&command,
                            &__MIDL_TypeFormatString.Format[52] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT STDMETHODCALLTYPE ITiEmuOLE_turn_calc_on_Proxy( 
    ITiEmuOLE __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *ret)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      16);
        
        
        
        if(!ret)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0] );
            
            *ret = *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++;
            
            _StubMsg.Buffer += 2;
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[2],
                         ( void __RPC_FAR * )ret);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITiEmuOLE_turn_calc_on_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    VARIANT_BOOL _M9;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    VARIANT_BOOL __RPC_FAR *ret;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( VARIANT_BOOL __RPC_FAR * )ret = 0;
    RpcTryFinally
        {
        ret = &_M9;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITiEmuOLE*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> turn_calc_on((ITiEmuOLE *) ((CStdStubBuffer *)This)->pvServerObject,ret);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 2U + 6U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( VARIANT_BOOL __RPC_FAR * )_StubMsg.Buffer)++ = *ret;
        
        _StubMsg.Buffer += 2;
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1];

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

CINTERFACE_PROXY_VTABLE(17) _ITiEmuOLEProxyVtbl = 
{
    &IID_ITiEmuOLE,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    ITiEmuOLE_image_loaded_Proxy ,
    ITiEmuOLE_emulated_calc_type_Proxy ,
    ITiEmuOLE_emulated_hw_version_Proxy ,
    ITiEmuOLE_emulated_os_version_Proxy ,
    ITiEmuOLE_ready_for_transfers_Proxy ,
    ITiEmuOLE_send_file_Proxy ,
    ITiEmuOLE_debug_file_Proxy ,
    ITiEmuOLE_reset_calc_Proxy ,
    ITiEmuOLE_execute_command_Proxy ,
    ITiEmuOLE_turn_calc_on_Proxy
};


static const PRPC_STUB_FUNCTION ITiEmuOLE_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    ITiEmuOLE_image_loaded_Stub,
    ITiEmuOLE_emulated_calc_type_Stub,
    ITiEmuOLE_emulated_hw_version_Stub,
    ITiEmuOLE_emulated_os_version_Stub,
    ITiEmuOLE_ready_for_transfers_Stub,
    ITiEmuOLE_send_file_Stub,
    ITiEmuOLE_debug_file_Stub,
    ITiEmuOLE_reset_calc_Stub,
    ITiEmuOLE_execute_command_Stub,
    ITiEmuOLE_turn_calc_on_Stub
};

CInterfaceStubVtbl _ITiEmuOLEStubVtbl =
{
    &IID_ITiEmuOLE,
    0,
    17,
    &ITiEmuOLE_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER) && !defined(__MINGW32__)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   [wire_marshal] or [user_marshal] attribute.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {
			
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/*  2 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/*  4 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/*  6 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/*  8 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */
/* 10 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 12 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 14 */	NdrFcShort( 0xa ),	/* Type Offset=10 */
/* 16 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 18 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 20 */	NdrFcShort( 0x34 ),	/* Type Offset=52 */
/* 22 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 24 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/* 26 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 28 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/* 30 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 32 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */
/* 34 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  8 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 10 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 12 */	NdrFcShort( 0x1a ),	/* Offset= 26 (38) */
/* 14 */	
			0x13, 0x0,	/* FC_OP */
/* 16 */	NdrFcShort( 0xc ),	/* Offset= 12 (28) */
/* 18 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 20 */	NdrFcShort( 0x2 ),	/* 2 */
/* 22 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 24 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 26 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 28 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 30 */	NdrFcShort( 0x8 ),	/* 8 */
/* 32 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (18) */
/* 34 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 36 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 38 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 40 */	NdrFcShort( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x4 ),	/* 4 */
/* 44 */	NdrFcShort( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (14) */
/* 48 */	
			0x12, 0x0,	/* FC_UP */
/* 50 */	NdrFcShort( 0xffffffea ),	/* Offset= -22 (28) */
/* 52 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x4 ),	/* 4 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (48) */

			0x0
        }
    };

const CInterfaceProxyVtbl * _oleaut_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ITiEmuOLEProxyVtbl,
    0
};

const CInterfaceStubVtbl * _oleaut_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ITiEmuOLEStubVtbl,
    0
};

PCInterfaceName const _oleaut_InterfaceNamesList[] = 
{
    "ITiEmuOLE",
    0
};

const IID *  _oleaut_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _oleaut_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _oleaut, pIID, n)

int __stdcall _oleaut_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_oleaut_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo oleaut_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _oleaut_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _oleaut_StubVtblList,
    (const PCInterfaceName * ) & _oleaut_InterfaceNamesList,
    (const IID ** ) & _oleaut_BaseIIDList,
    & _oleaut_IID_Lookup, 
    1,
    1,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
