// WordAddin.cpp: CWordAddin 的实现

#include "pch.h"
#include <atlbase.h>
#include <atlstr.h>

#include "WordAddin.h"

#include "WordEvents.h"


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


STDMETHODIMP CWordAddin::CountWords(IDispatch* pDoc)
{
	if (!pDoc)
		return E_POINTER; // 必须返回 HRESULT

	// 获取文档对象
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

	// 释放BSTR
	SysFreeString(bstrText);

	//// 显示统计结果（可替换为你自己的函数）
	//std::wstring msg = std::to_wstring(chineseCount) + L" 个中文字符\n" +
	//	std::to_wstring(englishWordCount) + L" 个英文单词";
	//MessageBox(NULL, msg.c_str(), L"字数统计", MB_OK);

	return S_OK; // 成功返回
}


//STDMETHODIMP CWordAddin::CountWords(LONG* pCount)
//{
//	if (!pCount) return E_POINTER;
//
//	// 获取 Word Application 对象
//	CComPtr<IDispatch> spApp;
//	HRESULT hr = GetSite(__uuidof(Word::_Application), (void**)&spApp);
//	if (FAILED(hr)) return hr;
//
//	// 获取 Active Document
//	CComDispatchDriver app(spApp);
//	CComVariant result;
//	hr = app.GetPropertyByName(L"ActiveDocument", &result);
//	if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal)
//		return hr;
//
//	CComDispatchDriver doc(result.pdispVal);
//
//	// 获取内容文本
//	CComVariant textVar;
//	hr = doc.GetPropertyByName(L"Content", &textVar);
//	if (FAILED(hr) || textVar.vt != VT_DISPATCH || !textVar.pdispVal)
//		return hr;
//
//	CComDispatchDriver content(textVar.pdispVal);
//	hr = content.GetPropertyByName(L"Text", &textVar);
//	if (FAILED(hr)) return hr;
//
//	if (textVar.vt != VT_BSTR)
//		return E_FAIL;
//
//	// CountWords() 函数中的内容获取方式不完整
//	// 建议： 如果需要更全面的统计，应该遍历整个文档对象模型（DOM）中的所有 Range 对象
//	CComBSTR text = textVar.bstrVal;
//	// 可能内存泄露
//	// 建议：明确使用 text.Attach(textVar.Detach()) 来避免不必要的复制或泄漏。
//
//
//
//	int chineseChars = 0;
//	int englishWords = 0;
//
//	// 简单的中英文词数统计逻辑
//	bool inWord = false;
//	for (int i = 0; i < text.Length(); ++i)
//	{
//		WCHAR ch = text[i];
//		if ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z'))
//		{
//			if (!inWord)
//			{
//				englishWords++;
//				inWord = true;
//			}
//		}
//		else if (ch >= 0x4E00 && ch <= 0x9FFF) // 中文字符范围
//		{
//			chineseChars++;
//			inWord = false;
//		}
//		else
//		{
//			inWord = false;
//		}
//	}
//	//上述逻辑过于简单。
//	//建议：使用正则表达式库（如 C++ <regex> 或 ICU）进行更精确的分词。
//	//     或者调用 Word 自带的.Words.Count 属性来获取英文单词数。
//
//	//CString msg;
//	//CStringW msg;
//	CAtlString msg;
//	wchar_t buffer[512];
//	wprintf_s(buffer, L"中文字符数：%d\n英文单词数：%d", chineseChars, englishWords);
//	MessageBox(NULL, buffer, L"字数统计", MB_OK | MB_ICONINFORMATION);
//	/*
//	7. UI 线程问题
//		你在 CountWords() 中直接调用了 MessageBox(NULL, ...)，这可能会导致以下问题：
//
//		如果 Word 插件运行在非 UI 线程，会导致界面阻塞甚至崩溃。
//		NULL 作为 HWND 参数可能不符合预期行为。
//		✅ 建议：
//
//		使用 Word 提供的 UI 接口显示提示信息，比如通过 Application.Alert()。
//		或者确保操作发生在主线程（可以通过 CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED) + 主线程调度）。
//	*/
//
//	//msg.Format(L"中文字符数：%d\n英文单词数：%d", chineseChars, englishWords);
//	//MessageBox(NULL, msg, L"字数统计", MB_OK | MB_ICONINFORMATION);
//
//	*pCount = chineseChars + englishWords;
//	return S_OK;
//}

// 虽然代码调用了 raw_OnConnection 来尝试注册事件，但没有实现正确的事件处理机制，如 IDispatch::Invoke 方法。

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnConnection(IDispatch* Application, ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom)
{
	// 获取Word应用程序对象
	m_spWordApp = Application;

	// 注册文档打开事件
	if (m_spWordApp)
	{
		// 这里需要注册DocumentOpen事件
		RegisterDocumentOpenEvent();
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
	//// 保存 Application 接口
	//m_spApplication = Application;

	//// 获取 Documents 集合对象
	//CComVariant result;
	//HRESULT hr = m_spApplication.GetPropertyByName(L"Documents", &result);
	//if (FAILED(hr) || result.vt != VT_DISPATCH)
	//	return hr;

	//CComPtr<IDispatch> spDocs = result.pdispVal;

	//// 获取 IConnectionPointContainer 接口
	//CComQIPtr<IConnectionPointContainer> spCPContainer(spDocs);
	//if (!spCPContainer)
	//	return E_NOINTERFACE;

	//// 查找事件接口 __uuidof(Word::DocumentsEvents)
	//CComPtr<IConnectionPoint> spCP;
	//hr = spCPContainer->FindConnectionPoint(__uuidof(Word::DocumentEvents), &spCP);
	//// 事件注册有问题 你监听的是 Documents 集合的事件，而不是单个文档。
	//// 建议：如果你想监听某个文档的打开/关闭事件，应该绑定到具体的 Document 对象，而非 Documents 集合。
	////      可以在 OnConnection 后订阅 Application 的 DocumentOpen 事件，或者在 raw_OnStartupComplete 中绑定。
	//if (FAILED(hr))
	//	return hr;


	//// 注册事件处理器，this 必须实现 Invoke() 或者 Sink 接口
	//IDispatch* pDisp = reinterpret_cast<IDispatch*>(
	//	static_cast<IWordAddin*>(this)  // 确保我们走的是 IWordAddin 的 COM 路径
	//	);
	//hr = spCP->Advise(pDisp, &m_cookie);
	//if (FAILED(hr))
	//	return hr;

	//return S_OK;
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
	if (!m_spWordApp) 
		return;

	CComObject<CWordEvents>* pWordEvents = nullptr;
	HRESULT hr = CComObject<CWordEvents>::CreateInstance(&pWordEvents);
	if (SUCCEEDED(hr))
	{
		pWordEvents->AddRef();
		pWordEvents->m_pAddIn = this;
		m_spWordEvents = pWordEvents;
		m_spWordEvents->DispEventAdvise(m_spWordApp);
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
