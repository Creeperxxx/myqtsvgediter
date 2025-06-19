// ExcelAddIn.h: CExcelAddIn 的声明

#pragma once
#include "resource.h"       // 主符号


// Microsoft Add-in Designer (MSADDNDR.DLL)
#import "libid:AC0714F2-3D04-11D1-AE7D-00A0C90F26F4" \
    named_guids, auto_search \
    rename_namespace("AddInDesignerObjects") \

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
#include "_IExcelAddInEvents_CP.h"

#include <memory>
#include "nonemptycellscountdialog.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;
using namespace AddInDesignerObjects;
using namespace Excel;

class CExcelAddInEvents;

// CExcelAddIn

class ATL_NO_VTABLE CExcelAddIn :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CExcelAddIn, &CLSID_ExcelAddIn>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CExcelAddIn>,
	public CProxy_IExcelAddInEvents<CExcelAddIn>,
	public IObjectWithSiteImpl<CExcelAddIn>,
	public IDispatchImpl<IExcelAddIn, &IID_IExcelAddIn, &LIBID_ExcelAddinProjectLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<AddInDesignerObjects::_IDTExtensibility2, &__uuidof(AddInDesignerObjects::_IDTExtensibility2), &LIBID_AddInDesignerObjects, 1, 0>,
	public IDispatchImpl<Office::IRibbonExtensibility, &__uuidof(Office::IRibbonExtensibility), &__uuidof(Office::__Office), 2,0>
{
public:
	CExcelAddIn()
		:m_spExcelApp(nullptr)
		, m_pUnkMarshaler(nullptr)
		, m_pExcelEvents(nullptr)
		, m_countDialog(new Cnonemptycellscountdialog(), [](Cnonemptycellscountdialog* p)
			{
				if (p->IsWindow())
				{
					p->m_isdelete = true;
					p->DestroyWindow();
				}
				else
				{
					delete p;
				}
			})
	{
	}

	DECLARE_REGISTRY_RESOURCEID(106)


	BEGIN_COM_MAP(CExcelAddIn)
		COM_INTERFACE_ENTRY(IExcelAddIn)
		COM_INTERFACE_ENTRY(_IDTExtensibility2)
		COM_INTERFACE_ENTRY(Office::IRibbonExtensibility)
		COM_INTERFACE_ENTRY2(IDispatch, IExcelAddIn)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IObjectWithSite)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CExcelAddIn)
		CONNECTION_POINT_ENTRY(__uuidof(_IExcelAddInEvents))
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

	STDMETHOD(raw_OnConnection)(IDispatch* Application, ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom);
	STDMETHOD(raw_OnDisconnection)(ext_DisconnectMode RemoveMode, SAFEARRAY** custom);
	STDMETHOD(raw_OnAddInsUpdate)(SAFEARRAY** custom);
	STDMETHOD(raw_OnStartupComplete)(SAFEARRAY** custom);
	STDMETHOD(raw_OnBeginShutdown)(SAFEARRAY** custom);

	STDMETHOD(raw_GetCustomUI)(BSTR RibbonID, BSTR* RibbonXml) override;

public:
	STDMETHOD(onGenerateChartButtonClick)(IDispatch* ribbonPtr) override;


	//STDMETHOD(initializeExcelEvents)();
	void initializeExcelEvents();

	Excel::_ApplicationPtr m_spExcelApp;
	CComObject<CExcelAddInEvents>* m_pExcelEvents;



	void ShowNonEmptyCells(LONG count);
	void RegisterApplicationEvents(Excel::_ApplicationPtr pApp);

	void OnSheetActivate(CComPtr<IDispatch> pSheet);
	void OnWorkBookActivate(Excel::_WorkbookPtr pBook);
	void OnSheetChange(IDispatchPtr sh, Excel::RangePtr rg);
	void CountAndShowNonEmptyCells(Excel::_WorksheetPtr pSheet);
	void CountNonEmptyCells(Excel::_WorksheetPtr pSheet, LONG* count);

	void InitializeCountDialog();
	std::unique_ptr<Cnonemptycellscountdialog, void(*)(Cnonemptycellscountdialog*)> m_countDialog;

	HWND getActivateWindow();

	void SetupCountDialog();

	bool generateChart();

	//void CreateCommandBar();
	//STDMETHOD(OnSheetActivate)(IDispatch* sheet);
	//STDMETHOD(OnWorkBookActivate)(Excel::_Workbook* pBook);
	//STDMETHOD(CountAndShowNonEmptyCells)(Excel::_WorksheetPtr pSheet);
	//STDMETHOD(countNonEmptyCells)(Excel::_WorksheetPtr pSheet, LONG* count);
};

OBJECT_ENTRY_AUTO(__uuidof(ExcelAddIn), CExcelAddIn)
