// ExcelAddInEvents.h: CExcelAddInEvents 的声明

#pragma once
#include "resource.h"       // 主符号

#import "libid:00020813-0000-0000-C000-000000000046" \
    rename("RGB", "ExcelRGB") \
    rename("DocumentProperties", "ExcelDocumentProperties") \
    rename("ExitWindows", "ExcelExitWindows") \
    rename("FindText", "ExcelFindText") \
    rename("CopyFile", "ExcelCopyFile") \
    rename("ReplaceText", "ExcelReplaceText") \
    rename("SearchPath", "ExcelSearchPath") \
    rename("FontNames", "ExcelFontNames") \
    rename("Rectangle", "ExcelRectangle") \
    rename("DialogBox", "ExcelDialogBox") \
    rename("Window", "ExcelWindow") \
    auto_search \
    rename_namespace("Excel")


#include "ExcelAddinProject_i.h"
#include "_IExcelAddInEventsEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;
using namespace Excel;


// CExcelAddInEvents
class CExcelAddIn;

class ATL_NO_VTABLE CExcelAddInEvents :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CExcelAddInEvents, &CLSID_ExcelAddInEvents>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CExcelAddInEvents>,
	public CProxy_IExcelAddInEventsEvents<CExcelAddInEvents>,
	public IObjectWithSiteImpl<CExcelAddInEvents>,
	public IDispatchImpl<IExcelAddInEvents, &IID_IExcelAddInEvents, &LIBID_ExcelAddinProjectLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventImpl<1, CExcelAddInEvents, &__uuidof(Excel::AppEvents), &__uuidof(Excel::__Excel), 1, 9>
{
public:
	CExcelAddInEvents()
		:m_pUnkMarshaler(nullptr)
		, m_pAddIn(nullptr)
	{
	}

DECLARE_REGISTRY_RESOURCEID(107)

BEGIN_SINK_MAP(CExcelAddInEvents)
	SINK_ENTRY_EX(1, __uuidof(Excel::AppEvents), 1568, OnWorkBookActivate) //1568
	SINK_ENTRY_EX(1, __uuidof(Excel::AppEvents), 1561, OnSheetActivate)
	SINK_ENTRY_EX(1, __uuidof(Excel::AppEvents), 1564, OnSheetChange)
END_SINK_MAP()
STDMETHOD(OnWorkBookActivate)(Excel::_Workbook* wk);
STDMETHOD(OnSheetActivate)(IDispatch* sheet);
STDMETHOD(OnSheetChange)(IDispatch* sheet, Range* Target);

BEGIN_COM_MAP(CExcelAddInEvents)
	COM_INTERFACE_ENTRY(IExcelAddInEvents)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CExcelAddInEvents)
	CONNECTION_POINT_ENTRY(__uuidof(_IExcelAddInEventsEvents))
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

	CExcelAddIn* m_pAddIn;


};

OBJECT_ENTRY_AUTO(__uuidof(ExcelAddInEvents), CExcelAddInEvents)
