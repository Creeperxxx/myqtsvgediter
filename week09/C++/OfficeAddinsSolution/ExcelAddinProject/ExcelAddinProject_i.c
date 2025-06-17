

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for ExcelAddinProject.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

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

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IComponentRegistrar,0xa817e7a2,0x43fa,0x11d0,0x9e,0x44,0x00,0xaa,0x00,0xb6,0x77,0x0a);


MIDL_DEFINE_GUID(IID, IID_IExcelAddIn,0x3ed0be35,0x1b1c,0x4880,0xb4,0xa2,0x55,0xce,0xd6,0x45,0x84,0x43);


MIDL_DEFINE_GUID(IID, IID_IExcelAddInEvents,0x588c6341,0xd823,0x4ad8,0xaa,0x66,0xcf,0x9c,0xe4,0xc4,0x66,0xe5);


MIDL_DEFINE_GUID(IID, LIBID_ExcelAddinProjectLib,0x5535cf69,0xcdca,0x474a,0x9c,0xe3,0xfe,0x70,0x85,0x2b,0x9f,0x5f);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0xc90e825b,0xd7e8,0x45eb,0xbb,0xea,0xb2,0x3c,0x00,0x7c,0x96,0x4d);


MIDL_DEFINE_GUID(IID, DIID__IExcelAddInEvents,0x6d1beac0,0xe95c,0x4c40,0xb7,0x66,0xd6,0xe0,0x0f,0xfd,0x68,0x60);


MIDL_DEFINE_GUID(CLSID, CLSID_ExcelAddIn,0xd7103993,0xef1b,0x405c,0xa9,0x40,0xa3,0x58,0x4e,0x82,0xa7,0x60);


MIDL_DEFINE_GUID(IID, DIID__IExcelAddInEventsEvents,0x99ca7653,0x3df2,0x47cd,0x84,0xd7,0x2e,0x8c,0xf8,0x8e,0xb2,0xfc);


MIDL_DEFINE_GUID(CLSID, CLSID_ExcelAddInEvents,0x558fc52b,0x415a,0x4f44,0xac,0xe9,0xa0,0xde,0x26,0xea,0x80,0xe3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



