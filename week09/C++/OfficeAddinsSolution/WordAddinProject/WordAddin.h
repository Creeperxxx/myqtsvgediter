// WordAddin.h: CWordAddin 的声明

#ifndef WORDADDIN_H
#define WORDADDIN_H

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

// Microsoft Add-in Designer (MSADDNDR.DLL)
#import "libid:AC0714F2-3D04-11D1-AE7D-00A0C90F26F4" \
    named_guids, auto_search \
    rename_namespace("AddInDesignerObjects") \

#include "resource.h"       // 主符号
#include "WordAddinProject_i.h"
#include "_IWordAddinEvents_CP.h"

#include <memory>
//#include "wordcountatldialog.h"
//#include "myresource.h"


using namespace AddInDesignerObjects;
using namespace Office;
using namespace Word;



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


class Cwordcountatldialog;

class CWordEvents;
// CWordAddin

class ATL_NO_VTABLE CWordAddin :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWordAddin, &CLSID_WordAddin>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CWordAddin>,
	public CProxy_IWordAddinEvents<CWordAddin>,
	public IObjectWithSiteImpl<CWordAddin>,
	public IDispatchImpl<IWordAddin, &IID_IWordAddin, &LIBID_WordAddinProjectLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<AddInDesignerObjects::_IDTExtensibility2, &__uuidof(AddInDesignerObjects::_IDTExtensibility2), &LIBID_AddInDesignerObjects, 2, 0>
	//public IDispatchImpl<Office::_CommandBarButtonEvents, &__uuidof(Office::_CommandBarButtonEvents), &__uuidof(__Office), 2, 0>
{
public:
	CWordAddin()
		:m_countDialog(nullptr)
		, m_spWordApp(nullptr)
	{
	}
	~CWordAddin();

	DECLARE_REGISTRY_RESOURCEID(106)


	BEGIN_COM_MAP(CWordAddin)
		COM_INTERFACE_ENTRY(IWordAddin)
		COM_INTERFACE_ENTRY(_IDTExtensibility2)
		//COM_INTERFACE_ENTRY(_CommandBarButtonEvents)
		COM_INTERFACE_ENTRY2(IDispatch, IWordAddin)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IObjectWithSite)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CWordAddin)
		CONNECTION_POINT_ENTRY(__uuidof(_IWordAddinEvents))
	END_CONNECTION_POINT_MAP()
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(CountWords)(IDispatch* doc, LONG* pChineseCount, LONG* pEnglishCount);

	STDMETHOD(raw_OnConnection)(IDispatch* Application, ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom);
	STDMETHOD(raw_OnDisconnection)(ext_DisconnectMode RemoveMode, SAFEARRAY** custom);
	STDMETHOD(raw_OnAddInsUpdate)(SAFEARRAY** custom);
	STDMETHOD(raw_OnStartupComplete)(SAFEARRAY** custom);
	STDMETHOD(raw_OnBeginShutdown)(SAFEARRAY** custom);


	//void countAndShow(IDispatch* doc);
	void countAndShow(_Document* doc);
	void initializeCountDialog();
	CComPtr<Word::_Application> m_spWordApp;
	void formatSelectionText();
private:
	//void OnDocumentOpen();
	void RegisterDocumentOpenEvent();
	void RegisterFormatButtonClickEvent(CComPtr<CommandBarControl> spCtrls);
	//DWORD m_dwButtonEventCookie;

	HWND getDocWindow();


	void CreateCustomToolbar();
	
	Cwordcountatldialog* m_countDialog;
	//std::unique_ptr<Cwordcountatldialog> m_countDialog;

	CComPtr<CommandBar> m_spFormatToolbar;

	CComObject<CWordEvents>* m_pWordEvents;
};

OBJECT_ENTRY_AUTO(__uuidof(WordAddin), CWordAddin)

#endif // WORDADDIN_H
