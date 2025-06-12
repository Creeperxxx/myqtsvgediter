// WordAddin.cpp: CWordAddin 的实现

#include <atlstr.h>
#include "pch.h"
#include "WordAddin.h"


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

STDMETHODIMP CWordAddin::CountWords(LONG* pCount)
{
	if (!pCount) return E_POINTER;

	// 获取 Word Application 对象
	CComPtr<IDispatch> spApp;
	HRESULT hr = GetSite(__uuidof(Word::_Application), (void**)&spApp);
	if (FAILED(hr)) return hr;

	// 获取 Active Document
	CComDispatchDriver app(spApp);
	CComVariant result;
	hr = app.GetPropertyByName(L"ActiveDocument", &result);
	if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal)
		return hr;

	CComDispatchDriver doc(result.pdispVal);

	// 获取内容文本
	CComVariant textVar;
	hr = doc.GetPropertyByName(L"Content", &textVar);
	if (FAILED(hr) || textVar.vt != VT_DISPATCH || !textVar.pdispVal)
		return hr;

	CComDispatchDriver content(textVar.pdispVal);
	hr = content.GetPropertyByName(L"Text", &textVar);
	if (FAILED(hr)) return hr;

	if (textVar.vt != VT_BSTR)
		return E_FAIL;

	ATL::CStringW text = textVar.bstrVal;

	int chineseChars = 0;
	int englishWords = 0;

	// 简单的中英文词数统计逻辑
	bool inWord = false;
	for (int i = 0; i < text.GetLength(); ++i)
	{
		WCHAR ch = text[i];
		if ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z'))
		{
			if (!inWord)
			{
				englishWords++;
				inWord = true;
			}
		}
		else if (ch >= 0x4E00 && ch <= 0x9FFF) // 中文字符范围
		{
			chineseChars++;
			inWord = false;
		}
		else
		{
			inWord = false;
		}
	}

	CString msg;

	msg.Format(L"中文字符数：%d\n英文单词数：%d", chineseChars, englishWords);
	MessageBox(NULL, msg, L"字数统计", MB_OK | MB_ICONINFORMATION);

	*pCount = chineseChars + englishWords;
	return S_OK;
}


STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnConnection(IDispatch* Application, ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom)
{
	// 保存 Application 接口
	m_spApplication = Application;

	// 获取 Documents 集合对象
	CComVariant result;
	HRESULT hr = m_spApplication.GetPropertyByName(L"Documents", &result);
	if (FAILED(hr) || result.vt != VT_DISPATCH)
		return hr;

	CComPtr<IDispatch> spDocs = result.pdispVal;

	// 获取 IConnectionPointContainer 接口
	CComQIPtr<IConnectionPointContainer> spCPContainer(spDocs);
	if (!spCPContainer)
		return E_NOINTERFACE;

	// 查找事件接口 __uuidof(Word::DocumentsEvents)
	CComPtr<IConnectionPoint> spCP;
	hr = spCPContainer->FindConnectionPoint(__uuidof(Word::DocumentEvents), &spCP);
	if (FAILED(hr))
		return hr;

	// 注册事件处理器，this 必须实现 Invoke() 或者 Sink 接口
	IDispatch* pDisp = reinterpret_cast<IDispatch*>(
		static_cast<IWordAddin*>(this)  // 确保我们走的是 IWordAddin 的 COM 路径
		);
	hr = spCP->Advise(pDisp, &m_cookie);
	if (FAILED(hr))
		return hr;

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
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnBeginShutdown(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

void CWordAddin::OnDocumentOpen()
{
	LONG wordCount = 0;
	CountWords(&wordCount);
}
