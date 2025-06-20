// pptaddin.h: CPPTAddIn 的声明

#pragma once
#include "resource.h"       // 主符号

#import "libid:91493440-5A91-11CF-8700-00AA0060263B" \
    auto_search \
    rename_namespace("PPT") \
    rename("RGB", "PPT_RGB") \
    rename("DocumentProperties", "PPT_DocumentProperties") \
    rename("PageSetup", "PPT_PageSetup") \
    rename("Shape", "PPT_Shape") \
    rename("TextRange", "PPT_TextRange")

#import "libid:AC0714F2-3D04-11D1-AE7D-00A0C90F26F4" \
    named_guids, auto_search \
    rename_namespace("AddInDesignerObjects") \

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
	public IDispatchImpl<IPPTAddIn, &IID_IPPTAddIn, &LIBID_PPTAddinProjectLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<AddInDesignerObjects::_IDTExtensibility2, &__uuidof(AddInDesignerObjects::_IDTExtensibility2), &AddInDesignerObjects::LIBID_AddInDesignerObjects,1,0>,
	public IDispatchImpl<Office::IRibbonExtensibility, &__uuidof(Office::IRibbonExtensibility), &__uuidof(Office::__Office), 2,8>
{
public:
	CPPTAddIn();

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CPPTAddIn)
	COM_INTERFACE_ENTRY(IPPTAddIn)
	COM_INTERFACE_ENTRY(AddInDesignerObjects::_IDTExtensibility2)
	COM_INTERFACE_ENTRY(Office::IRibbonExtensibility)
	COM_INTERFACE_ENTRY2(IDispatch, IPPTAddIn)
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

	STDMETHOD(OnCountNonEmptyTextShapes)(IDispatch* ribbonPtr) override;
	STDMETHOD(OnInsertSlideAndSetThemeBackground)(IDispatch* ribbonPtr) override;
private:
	bool getNonEmptyTextShapesCount(uint64_t& count);
	void showNonEmptyTextShapesCount(uint64_t count);

	PPT::_ApplicationPtr m_spPPTApp;

};

OBJECT_ENTRY_AUTO(__uuidof(PPTAddIn), CPPTAddIn)
