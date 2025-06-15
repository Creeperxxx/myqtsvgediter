// WordAddin.cpp: CWordAddin 的实现

#include "pch.h"
#include <atlbase.h>
#include <atlstr.h>

#include "WordAddin.h"

#include "WordEvents.h"

#include "wordcountatldialog.h"


// CWordAddin

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
	CComPtr<Word::Range> spRange;
	hr = spDocument->get_Content(&spRange);
	if (FAILED(hr) || !spRange)
		return hr;

	// 获取文本
	CComBSTR bstrText = NULL;
	hr = spRange->get_Text(&bstrText);
	if (FAILED(hr))
		return hr;

	//// 获取文本
	//BSTR bstrText = NULL;
	//hr = spRange->get_Text(&bstrText);
	//if (FAILED(hr) || !bstrText)
	//	return hr;

	// 统计字数
	int chineseCount = 0;
	int englishWordCount = 0;
	bool inEnglishWord = false;

	// 遍历文本，统计中文字数和英文字数
	for (int i = 0; bstrText[i] != L'\0'; i++)
	{
		// 中文字符范围：0x4E00-0x9FFF
		if (bstrText[i] >= 0x4E00 && bstrText[i] <= 0x9FFF)
		{
			chineseCount++;
			inEnglishWord = false; // 结束英文词
		}
		// 英文字符范围：A-Z, a-z
		else if ((bstrText[i] >= L'A' && bstrText[i] <= L'Z') ||
			(bstrText[i] >= L'a' && bstrText[i] <= L'z'))
		{
			if (!inEnglishWord)
			{
				englishWordCount++;
				inEnglishWord = true;
			}
		}
		else
		{
			inEnglishWord = false; // 非字母，结束英文词
		}
	}

	*pChineseCount = chineseCount;
	*pEnglishCount = englishWordCount;

	// 释放BSTR
	//SysFreeString(bstrText);

	//// 显示统计结果（可替换为你自己的函数）
	//std::wstring msg = std::to_wstring(chineseCount) + L" 个中文字符\n" +
	//	std::to_wstring(englishWordCount) + L" 个英文单词";
	//MessageBox(NULL, msg.c_str(), L"字数统计", MB_OK);

		// 使用 CAtlString 构建消息
	//CAtlString strMsg;
	//strMsg.Format(L"统计结果:\n中文字符: %d 个\n英文单词: %d 个",
	//	chineseCount, englishWordCount);

	//// 显示消息框
	//MessageBoxW(NULL, strMsg, L"字数统计", MB_OK);
	return S_OK;

	//CAtlString statusStrMsg;
	//statusStrMsg.Format(L"中文字符: %d | 英文单词: %d",
		//chineseCount, englishWordCount);
	//hr = m_spWordApp->put_StatusBar(CComBSTR(statusStrMsg));
	//hr = pApp->put_StatusBar(CComBSTR(statusStrMsg));
	//return hr;
}


STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnConnection(IDispatch* Application, ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom)
{
	// 获取Word应用程序对象
	HRESULT hr = Application->QueryInterface(__uuidof(Word::_Application), (void**)&m_spWordApp);

	if (SUCCEEDED(hr))
	{
		RegisterDocumentOpenEvent();
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

// 这里的调用并不安全
// 这个方法是从事件回调中调用的，没有检查当前是否有 ActiveDocument。
// 如果没有打开任何文档就触发此函数，可能导致访问空指针。
//✅ 建议：

// 在调用前判断是否存在活动文档。
// CComVariant result;
// HRESULT hr = m_spApplication.GetPropertyByName(L"ActiveDocument", &result);
// if (SUCCEEDED(hr) && result.vt == VT_DISPATCH && result.pdispVal) {
// 	CountWords(&wordCount);
// }

//void CWordAddin::OnDocumentOpen()
//{
//	LONG wordCount = 0;
//	CountWords(&wordCount);
//}

void CWordAddin::RegisterDocumentOpenEvent()
{
	CComObject<CWordEvents>* pWordEvents = nullptr;
	HRESULT hr = CComObject<CWordEvents>::CreateInstance(&pWordEvents);
	if (SUCCEEDED(hr) && pWordEvents != nullptr)
	{
		//pWordEvents->AddRef();
		pWordEvents->m_pAddIn = this;
		//hr = pWordEvents->DispEventAdvise(m_spWordApp);

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
	//// 创建事件接收器
	//m_spWordEvents = new CWordEvents(this);

	//// 连接事件接收器到Word应用程序
	//if (m_spWordEvents)
	//{
	//	m_spWordEvents->DispEventAdvise(m_spWordApp);
	//}
}

HWND CWordAddin::getDocWindow()
{
	CComPtr<Word::Window> spWindow;
	HRESULT hr = m_spWordApp->get_ActiveWindow(&spWindow);
	if (SUCCEEDED(hr))
	{
		long hwnd;
		hr = spWindow->get_Hwnd(&hwnd);
		if (SUCCEEDED(hr))
		{
			return reinterpret_cast<HWND>(static_cast<ULONG_PTR>(hwnd));
		}
	}
	return nullptr;
}

void CWordAddin::CreateCustomToolbar()
{

}

void CWordAddin::initializeCountDialog()
{
	//if (m_countDialog == nullptr || m_countDialog.get() == nullptr)
	//{
	//	m_countDialog = std::make_unique<Cwordcountatldialog>();
	//	m_countDialog->Create(getDocWindow());
	//	//m_countDialog->Create(nullptr);
	//	//m_countDialog->ShowWindow(SW_SHOW);
	//	m_countDialog->ShowWindowAsync(SW_SHOW);
	//}
	//else if (m_countDialog->m_hWnd == nullptr)
	//{
	//	//m_countDialog->Create(nullptr);
	//	m_countDialog->Create(getDocWindow());
	//	m_countDialog->ShowWindowAsync(SW_SHOW);
	//}

	//	// 销毁旧窗口（如果存在）
	//if (m_countDialog && m_countDialog->m_hWnd) {
	//	m_countDialog->DestroyWindow();
	//}

	//// 创建新窗口
	//m_countDialog = std::make_unique<Cwordcountatldialog>();
	//HWND hWndParent = getDocWindow();  // 确保父窗口有效
	//if (!hWndParent) 
	//	hWndParent = ::GetDesktopWindow();  // 回退到桌面

	//m_countDialog->Create(hWndParent);
	//m_countDialog->ShowWindow(SW_SHOW);  // 使用同步显示

	if (m_countDialog != nullptr)
	{
		if (m_countDialog->m_hWnd != nullptr)
		{
			m_countDialog->DestroyWindow();
		}
	}

	m_countDialog = new Cwordcountatldialog();
	if (m_countDialog != nullptr)
	{
		m_countDialog->Create(getDocWindow());
		m_countDialog->ShowWindow(SW_SHOW);
	}
}


