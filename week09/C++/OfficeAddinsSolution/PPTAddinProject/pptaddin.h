// pptaddin.h: CPPTAddIn 的声明

#pragma once
#include "resource.h"       // 主符号



#include "PPTAddinProject_i.h"
#include "_IPPTAddInEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CPPTAddIn

class ATL_NO_VTABLE CPPTAddIn :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPPTAddIn, &CLSID_PPTAddIn>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CPPTAddIn>,
	public CProxy_IPPTAddInEvents<CPPTAddIn>,
	public IObjectWithSiteImpl<CPPTAddIn>,
	public IDispatchImpl<IPPTAddIn, &IID_IPPTAddIn, &LIBID_PPTAddinProjectLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPPTAddIn()
	{
		m_pUnkMarshaler = nullptr;
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CPPTAddIn)
	COM_INTERFACE_ENTRY(IPPTAddIn)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPPTAddIn)
	CONNECTION_POINT_ENTRY(__uuidof(_IPPTAddInEvents))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:



};

OBJECT_ENTRY_AUTO(__uuidof(PPTAddIn), CPPTAddIn)
