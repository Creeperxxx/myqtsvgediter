// WordAddin.cpp: CWordAddin 的实现

#include "pch.h"
#include "resource.h"
#include <atlbase.h>
#include <atlstr.h>
#include <regex>

#include "WordAddin.h"

#include "WordEvents.h"

#include "wordcountatldialog.h"


// CWordAddin

CWordAddin::CWordAddin()
	: m_spWordApp(nullptr)
	, m_countDialog(new Cwordcountatldialog(), [](Cwordcountatldialog* p)
		{
			if (p->IsWindow())
			{
				p->m_isDelete = true;
				p->DestroyWindow();
			}
			else
			{
				delete p;
			}
		})
{
}

CWordAddin::~CWordAddin()
{
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


//STDMETHODIMP CWordAddin::CountWords(IDispatch* pDoc, LONG* pChineseCount, LONG* pEnglishCount)
//{
//	if (!pDoc)
//		return E_POINTER; // 必须返回 HRESULT
//
//	*pChineseCount = 0;
//	*pEnglishCount = 0;
//
//	//获取文档对象
//	CComPtr<Word::_Document> spDocument;
//	HRESULT hr = pDoc->QueryInterface(__uuidof(Word::_Document), (void**)&spDocument);
//	if (FAILED(hr) || !spDocument)
//		return hr; // 返回错误码
//
//	// 获取文档内容
//	CComPtr<Word::Range> spRange = nullptr;
//	hr = spDocument->get_Content(&spRange);
//	if (FAILED(hr) || !spRange)
//		return hr;
//
//	// 获取文本
//	CComBSTR bstrText = nullptr;
//	hr = spRange->get_Text(&bstrText);
//	if (FAILED(hr))
//		return hr;
//
//	//// 统计字数
//	//int chineseCount = 0;
//	//int englishWordCount = 0;
//	//bool inEnglishWord = false;
//
//	//// 遍历文本，统计中文字数和英文字数
//	//for (int i = 0; bstrText[i] != L'\0'; i++)
//	//{
//	//	// 中文字符范围：0x4E00-0x9FFF
//	//	if (bstrText[i] >= 0x4E00 && bstrText[i] <= 0x9FFF)
//	//	{
//	//		chineseCount++;
//	//		inEnglishWord = false; // 结束英文词
//	//	}
//	//	// 英文字符范围：A-Z, a-z
//	//	else if ((bstrText[i] >= L'A' && bstrText[i] <= L'Z') ||
//	//		(bstrText[i] >= L'a' && bstrText[i] <= L'z'))
//	//	{
//	//		if (!inEnglishWord)
//	//		{
//	//			englishWordCount++;
//	//			inEnglishWord = true;
//	//		}
//	//	}
//	//	else
//	//	{
//	//		inEnglishWord = false; // 非字母，结束英文词
//	//	}
//	//}
//
//	std::wstring text(bstrText);
//
//	std::wregex chineseRegex(L"[\\u4E00-\\u9FFF]");
//	auto chineseBegin = std::wsregex_iterator(text.begin(), text.end(), chineseRegex);
//	auto chineseEnd = std::wsregex_iterator();
//	*pChineseCount = static_cast<int>(std::distance(chineseBegin, chineseEnd));
//
//	std::wregex englishRegex(L"\\b[A-Za-z]+\\b");
//	auto englishBegin = std::wsregex_iterator(text.begin(), text.end(), englishRegex);
//	auto englishEnd = std::wsregex_iterator();
//	*pEnglishCount = static_cast<int>(std::distance(englishBegin, englishEnd));
//
//	return S_OK;
//
//}


STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnConnection(IDispatch* Application, ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom)
{
	// 获取Word应用程序对象
	HRESULT hr = Application->QueryInterface(__uuidof(Word::_Application), (void**)&m_spWordApp);
	if (FAILED(hr))
	{
		ATLASSERT("查询app接口失败");
		return hr;
	}
	return S_OK;
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
	//RegisterDocumentOpenEvent();
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnBeginShutdown(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_GetCustomUI(BSTR RibbonID, BSTR* RibbonXml)
{
	UNREFERENCED_PARAMETER(RibbonID);

	HRSRC hResource = FindResourceW(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCEW(IDR_XML_RESOURCE1), L"XML_RESOURCE");
	if (hResource == nullptr)
	{
		return E_FAIL;
	}

	HGLOBAL hLoadedResource = LoadResource(_AtlBaseModule.GetModuleInstance(), hResource);
	if (hLoadedResource == nullptr)
	{
		return E_FAIL;
	}

	LPVOID pResourceData = LockResource(hLoadedResource);
	if (pResourceData == nullptr)
	{
		return E_FAIL;
	}

	DWORD size = SizeofResource(_AtlBaseModule.GetModuleInstance(), hResource);
	if (size == 0)
	{
		FreeResource(hLoadedResource);
		return E_FAIL;
	}

	LPWSTR ribbonXmlContent = static_cast<LPWSTR>(pResourceData);

	*RibbonXml = SysAllocString(ribbonXmlContent);

	UnlockResource(hLoadedResource);
	FreeResource(hLoadedResource);

	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::OnFormatSelection(IDispatch* ribbonPtr)
{
	if (formatSelectionText())
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}

}

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::OnCountWords(IDispatch* ribbonPtr)
{
	uint64_t chineseCount = 0;
	uint64_t englishCount = 0;
	if (!MyGetCountResult(&chineseCount, &englishCount))
	{
		return E_FAIL;
	}

	MyShowCountResult(chineseCount, englishCount);

	return S_OK;
}


//bool CWordAddin::OnDocumentChange()
//{
//	Word::_DocumentPtr spDoc;
//	HRESULT hr = m_spWordApp->get_ActiveDocument(&spDoc);
//	if (FAILED(hr))
//	{
//		return false;
//	}
//	return countAndShow(spDoc);
//
//}

bool CWordAddin::MyGetCountResult(uint64_t* pChineseCount, uint64_t* pEnglishCount)
{
	//验证参数
	if (pChineseCount == nullptr || pEnglishCount == nullptr)
	{
		ATLTRACE("输入无效参数");
		return false;
	}

	//获取文档
	Word::_DocumentPtr activeDoc = MyGetActiveDoc();
	if (activeDoc == nullptr)
	{
		ATLTRACE("获取活跃文档失败");
		return false;
	}
	//计算
	return MyCalculateWords(activeDoc, pChineseCount, pEnglishCount);
}

Word::_DocumentPtr CWordAddin::MyGetActiveDoc()
{
	Word::_DocumentPtr spActiveDoc;
	HRESULT hr = m_spWordApp->get_ActiveDocument(&spActiveDoc);
	if (FAILED(hr))
	{
		ATLTRACE("获取活跃文档失败");
	}
	return spActiveDoc;
}

bool CWordAddin::MyCalculateWords(Word::_DocumentPtr spDocument, uint64_t* pChineseCount, uint64_t* pEnglishCount)
{
	if (spDocument == nullptr
		|| pChineseCount == nullptr
		|| pEnglishCount == nullptr)
	{
		ATLTRACE("传入的doc为空");
		return false;
	}

	*pChineseCount = 0;
	*pEnglishCount = 0;

	Word::RangePtr spRange = nullptr;
	HRESULT hr = spDocument->get_Content(&spRange);
	if (FAILED(hr) || !spRange)
	{
		ATLTRACE("获取内容失败");
		return false;
	}

	// 获取文本
	CComBSTR bstrText = nullptr;
	hr = spRange->get_Text(&bstrText);
	if (FAILED(hr) || bstrText == nullptr)
	{
		ATLTRACE("获取文本失败");
		return false;
	}

	std::wstring text(bstrText);

	std::wregex chineseRegex(L"[\\u4E00-\\u9FFF]");
	auto chineseBegin = std::wsregex_iterator(text.begin(), text.end(), chineseRegex);
	auto chineseEnd = std::wsregex_iterator();
	*pChineseCount = static_cast<uint64_t>(std::distance(chineseBegin, chineseEnd));

	std::wregex englishRegex(L"\\b[A-Za-z]+\\b");
	auto englishBegin = std::wsregex_iterator(text.begin(), text.end(), englishRegex);
	auto englishEnd = std::wsregex_iterator();
	*pEnglishCount = static_cast<uint64_t>(std::distance(englishBegin, englishEnd));

	return true;
}

void CWordAddin::MyShowCountResult(uint64_t chineseCount, uint64_t englishCount)
{
	SetupCountDialog();
	m_countDialog->showCount(chineseCount, englishCount);
}

//bool CWordAddin::countAndShow(Word::_DocumentPtr doc)
//{
//	uint64_t chineseCount = 0;
//	uint64_t englishCount = 0;
//	if (!CountWords(doc, &chineseCount, &englishCount))
//	{
//		ATLTRACE("中英文计数失败");
//		return false;
//	}
//
//	SetupCountDialog();
//	ShowWordsCount(chineseCount, englishCount);
//
//	return true;
//}

//bool CWordAddin::CountWords(Word::_DocumentPtr doc, uint64_t* pChineseCount, uint64_t* pEnglishCount)
//{
//	if (doc == nullptr)
//	{
//		ATLTRACE("传入的doc为空");
//		return false;
//	}
//
//	*pChineseCount = 0;
//	*pEnglishCount = 0;
//
//	Word::RangePtr spRange = nullptr;
//	HRESULT hr = doc->get_Content(&spRange);
//	if (FAILED(hr) || !spRange)
//	{
//		ATLTRACE("获取内容失败");
//		return false;
//	}
//
//	// 获取文本
//	CComBSTR bstrText = nullptr;
//	hr = spRange->get_Text(&bstrText);
//	if (FAILED(hr) || bstrText == nullptr)
//	{
//		ATLTRACE("获取文本失败");
//		return false;
//	}
//
//	std::wstring text(bstrText);
//
//	std::wregex chineseRegex(L"[\\u4E00-\\u9FFF]");
//	auto chineseBegin = std::wsregex_iterator(text.begin(), text.end(), chineseRegex);
//	auto chineseEnd = std::wsregex_iterator();
//	*pChineseCount = static_cast<uint64_t>(std::distance(chineseBegin, chineseEnd));
//
//	std::wregex englishRegex(L"\\b[A-Za-z]+\\b");
//	auto englishBegin = std::wsregex_iterator(text.begin(), text.end(), englishRegex);
//	auto englishEnd = std::wsregex_iterator();
//	*pEnglishCount = static_cast<uint64_t>(std::distance(englishBegin, englishEnd));
//
//	return true;
//}

//void CWordAddin::ShowWordsCount(uint64_t chineseCount, uint64_t englishCount)
//{
//	m_countDialog->showCount(chineseCount, englishCount);
//}


//void CWordAddin::RegisterDocumentOpenEvent()
//{
//
//	CComObject<CWordEvents>* pWordEvents = nullptr;
//	HRESULT hr = CComObject<CWordEvents>::CreateInstance(&pWordEvents);
//	if (SUCCEEDED(hr) && pWordEvents != nullptr)
//	{
//		m_pWordEvents = pWordEvents;
//		pWordEvents->m_pAddIn = this;
//		hr = pWordEvents->IDispEventImpl<1, CWordEvents, &__uuidof(ApplicationEvents4), &__uuidof(__Word), 8, 6>::DispEventAdvise(m_spWordApp);
//		if (FAILED(hr))
//		{
//			pWordEvents->Release();
//		}
//	}
//	else
//	{
//		ATLTRACE("注册失败");
//	}
//}

//void CWordAddin::RegisterFormatButtonClickEvent(CComPtr<CommandBarControl> spCtrls)
//{
//	HRESULT hr = m_pWordEvents->IDispEventImpl<2, CWordEvents, &__uuidof(Office::_CommandBarButtonEvents), &__uuidof(__Office), 2, 0>::DispEventAdvise(spCtrls);
//	if (SUCCEEDED(hr))
//	{
//		int a = 0;
//	}
//}

HWND CWordAddin::GetActiveDocWindow()
{
	CComPtr<Word::Window> spWindow = nullptr;
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

//void CWordAddin::CreateCustomToolbar()
//{
//	if (m_spWordApp == nullptr)
//	{
//		return;
//	}
//	CComPtr<Office::_CommandBars> spCommandBars;
//	HRESULT hr = m_spWordApp->get_CommandBars(&spCommandBars);
//	if (FAILED(hr))
//	{
//		return;
//	}
//	CComVariant name(L"格式化");
//	CComPtr<CommandBar> spCommandBar;
//	CComVariant vartemp;
//	spCommandBar = spCommandBars->Add(name, msoBarTop, vartemp, VARIANT_TRUE);
//	if (spCommandBar == nullptr)
//	{
//		return;
//	}
//	m_spFormatToolbar = spCommandBar;
//	CComPtr<CommandBarControls> spCtrls;
//	hr = spCommandBar->get_Controls(&spCtrls);
//	if (FAILED(hr))
//	{
//		return;
//	}
//	CComPtr<CommandBarControl> spCtrl;
//	spCtrl = spCtrls->Add(Office::msoControlButton, 1, vtMissing, 1, VARIANT_TRUE);
//	if (spCtrl == nullptr)
//	{
//		return;
//	}
//
//	spCtrl->put_Caption(CComBSTR(L"格式化"));
//
//	spCtrl->put_Tag(CComBSTR(L"FormatButton"));
//	spCtrl->put_Enabled(VARIANT_TRUE);
//
//
//	//ATLTRACE(L"Button Caption: %s\n", spCtrl->GetCaption().GetBSTR());
//	//ATLTRACE(L"Button Width: %d\n", spCtrl->GetWidth());
//	//ATLTRACE(L"Button Height: %d\n", spCtrl->GetHeight());
//	//ATLTRACE(L"Button Visible: %d\n", spCtrl->GetVisibleD() == VARIANT_TRUE ? 1 : 0);
//	//ATLTRACE(L"Button Enabled: %d\n", spCtrl->GetEnabled() == VARIANT_TRUE ? 1 : 0);
//	//spCtrl->put_OnAction(CComBSTR(L"?Click")); //当这样绑定动作时，点击会触发无法找到宏
//	//弄清楚了，这里动作绑定的是名为”？Click"的vba宏，不是c++中的。
//	spCommandBar->put_Visible(VARIANT_TRUE);
//
//	RegisterFormatButtonClickEvent(spCtrl);
//}

//void CWordAddin::initializeCountDialog()
//{
//	if (m_countDialog == nullptr || m_countDialog.get() == nullptr)
//	{
//		ATLASSERT("对话框未构造");
//		return;
//	}
//	if (m_countDialog->IsWindow())
//	{
//		ATLTRACE("对话框初始化时已create窗口");
//		return;
//	}
//	else
//	{
//		m_countDialog->Create(nullptr);
//		m_countDialog->ShowWindow(SW_SHOW);
//		return;
//	}
//
//
//	//if (m_countDialog == nullptr)
//	//{
//	//	m_countDialog = new Cwordcountatldialog();
//	//	m_countDialog->m_pAddIn = this;
//	//	m_countDialog->Create(getDocWindow());
//	//	m_countDialog->ShowWindow(SW_SHOW);
//	//	return;
//	//}
//	//else
//	//{
//	//	if (m_countDialog->IsWindow())
//	//	{
//	//		m_countDialog->SetParent(getDocWindow());
//	//		return;
//	//	}
//	//	else
//	//	{
//	//		HWND hwnd = getDocWindow();
//	//		hwnd = m_countDialog->Create(hwnd);
//	//		m_countDialog->ShowWindow(SW_SHOW);
//	//		return;
//	//	}
//	//}
//}

void CWordAddin::SetupCountDialog()
{
	HWND docWindow = GetActiveDocWindow();
	if (m_countDialog->IsWindow())
	{
		m_countDialog->SetParent(docWindow);
	}
	else
	{
		//m_countDialog->Create(docWindow); 
		//这里不知道为什么，对话框最初显示时总是会卡好半天,所以以桌面创建算了，防止卡word。
		m_countDialog->Create(nullptr);
		m_countDialog->ShowWindow(SW_SHOW);
		return;
	}
}


bool CWordAddin::formatSelectionText()
{
	if (m_spWordApp == nullptr)
	{
		ATLTRACE("app指针为空");
		return false;
	}
	Word::SelectionPtr spSelection;
	HRESULT hr = m_spWordApp->get_Selection(&spSelection);
	if (FAILED(hr) || spSelection == nullptr)
	{
		ATLTRACE("选择为空");
		return false;
	}

	Word::RangePtr spRange;
	hr = spSelection->get_Range(&spRange);
	if (FAILED(hr) || spRange == nullptr)
	{
		ATLTRACE("获取Range失败");
		return false;
	}
	float indent = 24;
	CComPtr<Word::_ParagraphFormat> spParaFormat;
	hr = spRange->get_ParagraphFormat(&spParaFormat);
	if (FAILED(hr) || spParaFormat == nullptr)
	{
		ATLTRACE("获取ParagraphFormat失败");
		return false;
	}
	hr = spParaFormat->put_FirstLineIndent(indent);
	if (FAILED(hr))
	{
		ATLTRACE("putFirstLineIndent失败");
		return false;
	}
	hr = spParaFormat->put_LineSpacingRule(Word::wdLineSpace1pt5);
	if (FAILED(hr))
	{
		ATLTRACE("putLineSpaceingRule失败");
		return false;
	}
	hr = m_spWordApp->Activate();
	if (FAILED(hr))
	{
		ATLTRACE("激活app失败");
		return false;
	}
	return true;
}



