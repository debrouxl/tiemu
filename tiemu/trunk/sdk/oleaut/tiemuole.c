/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Feb 27 16:44:59 2006
 */
/* Compiler settings for oleaut.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ITiEmuOLE = {0xe9373bd9,0x7363,0x427f,{0xa2,0xa6,0x1e,0x8b,0xa9,0x1f,0xfb,0x3e}};


const IID LIBID_TiEmuOLELib = {0x6140dc7c,0x3d74,0x41f9,{0xa7,0x87,0x59,0x46,0x99,0x4a,0xa8,0xf2}};


const CLSID CLSID_TiEmuOLE = {0xb2a17b13,0x9d6f,0x4dd4,{0xa2,0xa9,0x6f,0xe0,0x6a,0xdc,0x1d,0x33}};


#ifdef __cplusplus
}
#endif

