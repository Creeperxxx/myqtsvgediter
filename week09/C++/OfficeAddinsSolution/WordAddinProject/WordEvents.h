// WordEvents.h: CWordEvents 的声明

#pragma once

#include <atlbase.h>   // 包含基本的 ATL 支持
#include <atlcom.h>    // 包含 COM 接口和支持函数
#include <atlctl.h>
#include "resource.h"       // 主符号

#import "libid:00020905-0000-0000-C000-000000000046" \
    rename("RGB", "WordRGB") \
    rename("DocumentProperties", "WordDocumentProperties") \
    rename("ExitWindows", "WordExitWindows") \
    rename("FindText", "WordFindText") \
    rename("CopyFile", "WordCopyFile") \
    rename("ReplaceText", "WordReplaceText") \
    rename("SearchPath", "WordSearchPath") \
    rename("FontNames", "WordFontNames") \
    rename("Rectangle", "WordRectangle") \
    auto_search \
    rename_namespace("Word")

#include "WordAddinProject_i.h"
#include "_IWordEventsEvents_CP.h"

class CWordAddin;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;
using namespace Word;


// CWordEvents

class ATL_NO_VTABLE CWordEvents :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWordEvents, &CLSID_WordEvents>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CWordEvents>,
	public CProxy_IWordEventsEvents<CWordEvents>,
	public IDispatchImpl<IWordEvents, &IID_IWordEvents, &LIBID_WordAddinProjectLib, /*wMajor =*/ 1, /*wMinor =*/ 0>, 
	public IDispEventImpl<1, CWordEvents, &__uuidof(ApplicationEvents4), &__uuidof(__Word), 8, 6>,
	//public IDispEventImpl<2, CWordEvents, &__uuidof(DocumentEvents2), &__uuidof(__Word), 8, 6>,
	public IDispEventImpl<2, CWordEvents, &__uuidof(Office::_CommandBarButtonEvents), &__uuidof(Office::__Office), 2, 0>
{
public:
	CWordEvents()
	{
		m_pUnkMarshaler = nullptr;
		m_pCurrentActivateDoc = nullptr;
	}
	CWordEvents(CWordAddin* pAddIn)
		:m_pAddIn(pAddIn)
		, m_pUnkMarshaler(nullptr)
	{

	}

DECLARE_REGISTRY_RESOURCEID(107)

BEGIN_SINK_MAP(CWordEvents)
	//SINK_ENTRY_EX(1, __uuidof(ApplicationEvents4), 4, OnDocumentOpen)
	//SINK_ENTRY_EX(1, __uuidof(ApplicationEvents4), 10, OnWindowActivate)
	//SINK_ENTRY_EX(2, __uuidof(DocumentEvents2), 5, OnDocumentOpen)
	//SINK_ENTRY_EX(2, __uuidof(DocumentEvents2), 17, OnContentControlBeforeStoreUpdate)
	SINK_ENTRY_EX(1, __uuidof(ApplicationEvents4), 3, OnDocumentChange)
	SINK_ENTRY_EX(2, __uuidof(Office::_CommandBarButtonEvents), 1, OnFormatButtonClick)
END_SINK_MAP()


//STDMETHOD(OnDocumentOpen)(IDispatch* Doc);
//STDMETHOD(OnWindowActivate)(_Document* Doc, Window* Wn);
//STDMETHOD(OnDocumentOpen)();
//STDMETHOD(OnContentControlBeforeStoreUpdate)(ContentControl* ContentControl, BSTR* Content);
STDMETHOD(OnDocumentChange)();
STDMETHOD(OnFormatButtonClick)(Office::_CommandBarButton* Ctrl, VARIANT_BOOL* CancelDefault);

BEGIN_COM_MAP(CWordEvents)
	COM_INTERFACE_ENTRY(IWordEvents)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CWordEvents)
	CONNECTION_POINT_ENTRY(__uuidof(_IWordEventsEvents))
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



	CWordAddin* m_pAddIn;

	//_Document* m_pCurrentActivateDoc;
	CComPtr<_Document> m_pCurrentActivateDoc;
};

OBJECT_ENTRY_AUTO(__uuidof(WordEvents), CWordEvents)
