

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for PPTAddinProject.idl:
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


MIDL_DEFINE_GUID(IID, IID_IPPTAddIn,0x76858161,0x98c4,0x4635,0xa0,0x2b,0x69,0x0d,0xf9,0x9d,0xcc,0xfd);


MIDL_DEFINE_GUID(IID, LIBID_PPTAddinProjectLib,0xed6bc220,0x018c,0x4b8f,0x9f,0x88,0x81,0x21,0xde,0x38,0x81,0xfc);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0x467bb58e,0x9b24,0x4a6c,0x8d,0x95,0x5e,0xca,0xae,0x0a,0x7b,0x10);


MIDL_DEFINE_GUID(IID, DIID__IPPTAddInEvents,0x04e8b9f6,0xb0c3,0x40e6,0x97,0x93,0x69,0x5b,0xe9,0x50,0xfa,0xe7);


MIDL_DEFINE_GUID(CLSID, CLSID_PPTAddIn,0x834eea5e,0x726d,0x465e,0x9b,0x5c,0x00,0x1b,0x86,0x9d,0xc4,0x49);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



