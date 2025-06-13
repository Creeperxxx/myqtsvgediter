

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for WordAddinProject.idl:
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


MIDL_DEFINE_GUID(IID, IID_IWordAddin,0x30112008,0xa740,0x4786,0x96,0x38,0x05,0x97,0x45,0xfc,0xde,0x53);


MIDL_DEFINE_GUID(IID, IID_IWordEvents,0xc7b4ecd2,0x4283,0x4751,0xa4,0x21,0x85,0xf7,0x36,0xee,0xcf,0x89);


MIDL_DEFINE_GUID(IID, LIBID_WordAddinProjectLib,0x69001a9b,0x9ab6,0x4e30,0xb5,0x6f,0xc9,0x39,0xd4,0x3d,0xba,0x8a);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0x7a62e261,0x5fb4,0x4f08,0x9e,0xa7,0x50,0xf6,0x54,0x96,0xdd,0xcf);


MIDL_DEFINE_GUID(IID, DIID__IWordAddinEvents,0x136c8886,0x1eb2,0x4096,0x8d,0x27,0xc1,0xf8,0xc1,0x9e,0xc8,0x84);


MIDL_DEFINE_GUID(CLSID, CLSID_WordAddin,0x45e9f4bf,0x9077,0x433e,0xb3,0xb1,0x01,0xc6,0x60,0xc1,0xcd,0x07);


MIDL_DEFINE_GUID(IID, DIID__IWordEventsEvents,0xce3a6373,0x18e5,0x4429,0xb3,0xee,0x6e,0x5d,0xf7,0x7c,0x6d,0xe6);


MIDL_DEFINE_GUID(CLSID, CLSID_WordEvents,0x93657f52,0x68e1,0x45f4,0xa0,0xaf,0x6b,0x09,0x37,0x5a,0x0e,0xd3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



