// creeperaddin.h: CCreeperAddIn 的声明

#pragma once
#include "resource.h"       // 主符号

#include "import.h"


#include "CreeperOfficeAddIn_i.h"
#include "_ICreeperAddInEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

class CCreeperAddInEvent;

// CCreeperAddIn

class ATL_NO_VTABLE CCreeperAddIn :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCreeperAddIn, &CLSID_CreeperAddIn>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CCreeperAddIn>,
	public CProxy_ICreeperAddInEvents<CCreeperAddIn>,
	public IObjectWithSiteImpl<CCreeperAddIn>,
	public IDispatchImpl<ICreeperAddIn, &IID_ICreeperAddIn, &LIBID_CreeperOfficeAddInLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<AddInDesignerObjects::_IDTExtensibility2, &__uuidof(AddInDesignerObjects::_IDTExtensibility2), &AddInDesignerObjects::LIBID_AddInDesignerObjects, 1, 0>,
	public IDispatchImpl<Office::IRibbonExtensibility, &__uuidof(Office::IRibbonExtensibility), &__uuidof(Office::__Office), 2, 8>
{
public:
	CCreeperAddIn();

	DECLARE_REGISTRY_RESOURCEID(106)


	BEGIN_COM_MAP(CCreeperAddIn)
		COM_INTERFACE_ENTRY(ICreeperAddIn)
		COM_INTERFACE_ENTRY(_IDTExtensibility2)
		COM_INTERFACE_ENTRY(Office::IRibbonExtensibility)
		COM_INTERFACE_ENTRY2(IDispatch, ICreeperAddIn)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IObjectWithSite)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CCreeperAddIn)
		CONNECTION_POINT_ENTRY(__uuidof(_ICreeperAddInEvents))
	END_CONNECTION_POINT_MAP()
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct();

	void FinalRelease();

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(raw_OnConnection)(IDispatch* Application, AddInDesignerObjects::ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom) override;
	STDMETHOD(raw_OnDisconnection)(AddInDesignerObjects::ext_DisconnectMode RemoveMode, SAFEARRAY** custom) override;
	STDMETHOD(raw_OnAddInsUpdate)(SAFEARRAY** custom) override;
	STDMETHOD(raw_OnStartupComplete)(SAFEARRAY** custom) override;
	STDMETHOD(raw_OnBeginShutdown)(SAFEARRAY** custom) override;

	STDMETHOD(raw_GetCustomUI)(BSTR RibbonID, BSTR* RibbonXml) override;

	bool initApp(IDispatch* application);
	bool isWordAppValid();
	bool isExcelAppValid();
	bool isPPTAppValid();
	Word::_ApplicationPtr m_spWordApp;
	Excel::_ApplicationPtr m_spExcelApp;
	PPT::_ApplicationPtr m_spPPTApp;

};

OBJECT_ENTRY_AUTO(__uuidof(CreeperAddIn), CCreeperAddIn)
