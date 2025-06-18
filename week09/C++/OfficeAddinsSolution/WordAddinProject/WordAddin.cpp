// WordAddin.cpp: CWordAddin 的实现

#include "pch.h"
#include <atlbase.h>
#include <atlstr.h>
#include <regex>

#include "WordAddin.h"

#include "WordEvents.h"

#include "wordcountatldialog.h"


// CWordAddin

CWordAddin::~CWordAddin()
{
	if (m_countDialog != nullptr)
	{
		if (m_countDialog->IsWindow())
		{
			m_countDialog->m_isDelete = true;
			m_countDialog->DestroyWindow();
			m_countDialog->myDestroy();
		}
		else
		{
			delete m_countDialog;
		}
	}
}

STDMETHODIMP CWordAddin::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =
	{
		&IID_IWordAddin
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CWordAddin::CountWords(IDispatch* pDoc, LONG* pChineseCount, LONG* pEnglishCount)
{
	if (!pDoc)
		return E_POINTER; // 必须返回 HRESULT

	*pChineseCount = 0;
	*pEnglishCount = 0;

	//获取文档对象
	CComPtr<Word::_Document> spDocument;
	HRESULT hr = pDoc->QueryInterface(__uuidof(Word::_Document), (void**)&spDocument);
	if (FAILED(hr) || !spDocument)
		return hr; // 返回错误码

	// 获取文档内容
	CComPtr<Word::Range> spRange = nullptr;
	hr = spDocument->get_Content(&spRange);
	if (FAILED(hr) || !spRange)
		return hr;

	// 获取文本
	CComBSTR bstrText = nullptr;
	hr = spRange->get_Text(&bstrText);
	if (FAILED(hr))
		return hr;

	//// 统计字数
	//int chineseCount = 0;
	//int englishWordCount = 0;
	//bool inEnglishWord = false;

	//// 遍历文本，统计中文字数和英文字数
	//for (int i = 0; bstrText[i] != L'\0'; i++)
	//{
	//	// 中文字符范围：0x4E00-0x9FFF
	//	if (bstrText[i] >= 0x4E00 && bstrText[i] <= 0x9FFF)
	//	{
	//		chineseCount++;
	//		inEnglishWord = false; // 结束英文词
	//	}
	//	// 英文字符范围：A-Z, a-z
	//	else if ((bstrText[i] >= L'A' && bstrText[i] <= L'Z') ||
	//		(bstrText[i] >= L'a' && bstrText[i] <= L'z'))
	//	{
	//		if (!inEnglishWord)
	//		{
	//			englishWordCount++;
	//			inEnglishWord = true;
	//		}
	//	}
	//	else
	//	{
	//		inEnglishWord = false; // 非字母，结束英文词
	//	}
	//}

	std::wstring text(bstrText);

	std::wregex chineseRegex(L"[\\u4E00-\\u9FFF]");
	auto chineseBegin = std::wsregex_iterator(text.begin(), text.end(), chineseRegex);
	auto chineseEnd = std::wsregex_iterator();
	*pChineseCount = static_cast<int>(std::distance(chineseBegin, chineseEnd));

	std::wregex englishRegex(L"\\b[A-Za-z]+\\b");
	auto englishBegin = std::wsregex_iterator(text.begin(), text.end(), englishRegex);
	auto englishEnd = std::wsregex_iterator();
	*pEnglishCount = static_cast<int>(std::distance(englishBegin, englishEnd));

	return S_OK;

}


STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnConnection(IDispatch* Application, ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom)
{
	// 获取Word应用程序对象
	HRESULT hr = Application->QueryInterface(__uuidof(Word::_Application), (void**)&m_spWordApp);

	if (SUCCEEDED(hr))
	{
		RegisterDocumentOpenEvent();
		CreateCustomToolbar();
		//initializeCountDialog();
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}

	//// 注册文档打开事件
	//if (m_spWordApp)
	//{
		// 这里需要注册DocumentOpen事件
		//RegisterDocumentOpenEvent();
		//return S_OK;
	//}
	//else
	//{
		//return E_FAIL;
	//}
}

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnDisconnection(ext_DisconnectMode RemoveMode, SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnAddInsUpdate(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnStartupComplete(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnBeginShutdown(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

void CWordAddin::countAndShow(_Document* doc)
{
	LONG chineseCount = 0;
	LONG englishCount = 0;

	CountWords(doc, &chineseCount, &englishCount);
	m_countDialog->showCount(chineseCount, englishCount);
}


void CWordAddin::RegisterDocumentOpenEvent()
{

	CComObject<CWordEvents>* pWordEvents = nullptr;
	HRESULT hr = CComObject<CWordEvents>::CreateInstance(&pWordEvents);
	if (SUCCEEDED(hr) && pWordEvents != nullptr)
	{
		m_pWordEvents = pWordEvents;
		pWordEvents->m_pAddIn = this;
		hr = pWordEvents->IDispEventImpl<1, CWordEvents, &__uuidof(ApplicationEvents4), &__uuidof(__Word), 8, 6>::DispEventAdvise(m_spWordApp);
		if (FAILED(hr))
		{
			pWordEvents->Release();
		}
	}
	else
	{
		ATLTRACE("CWordAddin::RegisterDocumentOpenEvent() failed to create CWordEvents instance");
	}
}

void CWordAddin::RegisterFormatButtonClickEvent(CComPtr<CommandBarControl> spCtrls)
{
	HRESULT hr = m_pWordEvents->IDispEventImpl<2, CWordEvents, &__uuidof(Office::_CommandBarButtonEvents), &__uuidof(__Office), 2, 0>::DispEventAdvise(spCtrls);
	if (SUCCEEDED(hr))
	{
		int a = 0;
	}
}

HWND CWordAddin::getDocWindow()
{
	CComPtr<Word::Window> spWindow;
	HRESULT hr = m_spWordApp->get_ActiveWindow(&spWindow);
	if (SUCCEEDED(hr))
	{
		//long hwnd;
		LONG_PTR hwnd;
		hr = spWindow->get_Hwnd(reinterpret_cast<long*>(&hwnd));
		if (SUCCEEDED(hr))
		{
			return reinterpret_cast<HWND>(hwnd);
		}
	}
	return nullptr;
}

void CWordAddin::CreateCustomToolbar()
{
	if (m_spWordApp == nullptr)
	{
		return;
	}
	CComPtr<Office::_CommandBars> spCommandBars;
	HRESULT hr = m_spWordApp->get_CommandBars(&spCommandBars);
	if (FAILED(hr))
	{
		return;
	}
	CComVariant name(L"格式化");
	CComPtr<CommandBar> spCommandBar;
	CComVariant vartemp;
	spCommandBar = spCommandBars->Add(name, msoBarTop, vartemp, VARIANT_TRUE);
	if (spCommandBar == nullptr)
	{
		return;
	}
	m_spFormatToolbar = spCommandBar;
	CComPtr<CommandBarControls> spCtrls;
	hr = spCommandBar->get_Controls(&spCtrls);
	if (FAILED(hr))
	{
		return;
	}
	CComPtr<CommandBarControl> spCtrl;
	spCtrl = spCtrls->Add(CComVariant(1), vtMissing, vtMissing, vtMissing, vtMissing);
	if (spCtrl == nullptr)
	{
		return;
	}

	spCtrl->put_Caption(CComBSTR(L"格式化"));

	spCtrl->put_Tag(CComBSTR(L"FormatButton"));
	spCtrl->put_Enabled(VARIANT_TRUE);


	//ATLTRACE(L"Button Caption: %s\n", spCtrl->GetCaption().GetBSTR());
	//ATLTRACE(L"Button Width: %d\n", spCtrl->GetWidth());
	//ATLTRACE(L"Button Height: %d\n", spCtrl->GetHeight());
	//ATLTRACE(L"Button Visible: %d\n", spCtrl->GetVisibleD() == VARIANT_TRUE ? 1 : 0);
	//ATLTRACE(L"Button Enabled: %d\n", spCtrl->GetEnabled() == VARIANT_TRUE ? 1 : 0);
	//spCtrl->put_OnAction(CComBSTR(L"?Click")); //当这样绑定动作时，点击会触发无法找到宏
	//弄清楚了，这里动作绑定的是名为”？Click"的vba宏，不是c++中的。
	spCommandBar->put_Visible(VARIANT_TRUE);

	RegisterFormatButtonClickEvent(spCtrl);
}

void CWordAddin::initializeCountDialog()
{
	if (m_countDialog == nullptr)
	{
		m_countDialog = new Cwordcountatldialog();
		m_countDialog->m_pAddIn = this;
		m_countDialog->Create(getDocWindow());
		m_countDialog->ShowWindow(SW_SHOW);
		return;
	}
	else
	{
		if (m_countDialog->IsWindow())
		{
			m_countDialog->SetParent(getDocWindow());
			return;
		}
		else
		{
			HWND hwnd = getDocWindow();
			hwnd = m_countDialog->Create(hwnd);
			m_countDialog->ShowWindow(SW_SHOW);
			return;
		}
	}
}

void CWordAddin::formatSelectionText()
{
	if (m_spWordApp == nullptr)
	{
		return;
	}
	CComPtr<Word::Selection> spSelection;
	HRESULT hr = m_spWordApp->get_Selection(&spSelection);
	if (FAILED(hr))
	{
		return;
	}

	CComPtr<Word::Range> spRange;
	hr = spSelection->get_Range(&spRange);
	if (FAILED(hr))
	{
		return;
	}
	float indent = 24;
	CComPtr<Word::_ParagraphFormat> spParaFormat;
	hr = spRange->get_ParagraphFormat(&spParaFormat);
	if (FAILED(hr))
	{
		return;
	}
	hr = spParaFormat->put_FirstLineIndent(indent);
	if (FAILED(hr))
	{
		return;
	}
	hr = spParaFormat->put_LineSpacingRule(Word::wdLineSpace1pt5);
	if (FAILED(hr))
	{
		return;
	}
	hr = m_spWordApp->Activate();
	if (FAILED(hr))
	{
		return;
	}
}


