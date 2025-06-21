

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for CreeperOfficeAddIn.idl:
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


MIDL_DEFINE_GUID(IID, IID_ICreeperAddIn,0x76a166b0,0xd95b,0x47d9,0x8a,0x5d,0xc4,0xb7,0xa3,0x42,0xce,0x49);


MIDL_DEFINE_GUID(IID, LIBID_CreeperOfficeAddInLib,0x6589578e,0x3253,0x4ecf,0x83,0x5e,0x9f,0xb7,0x14,0x96,0xa4,0x4b);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0x49277317,0xa6fe,0x4148,0x91,0x6d,0xa6,0xc4,0x86,0xf8,0xde,0x6b);


MIDL_DEFINE_GUID(IID, DIID__ICreeperAddInEvents,0x4165c139,0xd17c,0x472b,0xbc,0xf5,0x82,0x11,0xf1,0x2f,0xae,0x8e);


MIDL_DEFINE_GUID(CLSID, CLSID_CreeperAddIn,0xbe85fdc6,0x3d20,0x4d9f,0x9a,0x91,0xd5,0xbf,0xe2,0x99,0xec,0x1b);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



