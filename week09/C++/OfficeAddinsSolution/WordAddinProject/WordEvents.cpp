// WordEvents.cpp: CWordEvents 的实现

#include "pch.h"
#include "WordEvents.h"

#include "WordAddin.h"

// CWordEvents

STDMETHODIMP CWordEvents::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =
	{
		&IID_IWordEvents
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}

//STDMETHODIMP CWordEvents::OnDocumentOpen(IDispatch* Doc)
//{
//	if (m_pAddIn)
//	{
//		// 文档打开后，调用插件的统计字数函数
//		//m_pAddIn->CountWords();
//		//m_pAddin->CountWordsInDocument(Doc);
//		m_pAddIn->initializeCountDialog();
//		m_pAddIn->countAndShow(Doc);
//		return S_OK;
//	}
//	else
//	{
//		return E_FAIL;
//	}
//}

STDMETHODIMP CWordEvents::OnWindowActivate(Word::_Document* Doc, Word::Window* Wn)
{
	HRESULT hr = S_OK;

	if (!Doc)
	{
		return E_POINTER;
	}

	// 先解绑旧文档
	if (m_pCurrentActivateDoc != nullptr)
	{
		hr = IDispEventImpl<2, CWordEvents, &__uuidof(DocumentEvents2), &__uuidof(__Word), 8, 6>::DispEventUnadvise(m_pCurrentActivateDoc);
		if (FAILED(hr))
		{
			OutputDebugStringW(L"[Error] DispEventUnadvise failed.\n");
		}
		//m_pCurrentActivateDoc->Release();
		//m_pCurrentActivateDoc = nullptr;
	}

	// 绑定新文档
	hr = IDispEventImpl<2, CWordEvents, &__uuidof(DocumentEvents2), &__uuidof(__Word), 8, 6>::DispEventAdvise(Doc);
	if (SUCCEEDED(hr))
	{
		m_pCurrentActivateDoc = Doc;
	}

	return hr;
}

STDMETHODIMP_(HRESULT __stdcall) CWordEvents::OnDocumentOpen()
{
	if (m_pAddIn != nullptr)
	{
		m_pAddIn->initializeCountDialog();
		CComPtr<Word::_Document> doc;
		HRESULT hr = m_pAddIn->m_spWordApp->get_ActiveDocument(&doc);
		if (SUCCEEDED(hr))
		{
			m_pAddIn->countAndShow(doc);
			return S_OK;
		}
		//if (SUCCEEDED(m_pCurrentActivateDoc->AddRef()))
		//{
			//m_pCurrentActivateDoc->Release();
			//m_pAddIn->countAndShow(m_pCurrentActivateDoc);
			//return S_OK;
		//}
	}
	return E_FAIL;
}

//STDMETHODIMP_(HRESULT __stdcall) CWordEvents::OnContentControlBeforeStoreUpdate(ContentControl* ContentControl, BSTR* Content)
//{
//	if (m_pAddIn != nullptr)
//	{
//		m_pAddIn->initializeCountDialog();
//		CComPtr<Word::_Document> doc;
//		HRESULT hr = m_pAddIn->m_spWordApp->get_ActiveDocument(&doc);
//		if (SUCCEEDED(hr))
//		{
//			m_pAddIn->countAndShow(doc);
//			return S_OK;
//		}
//		//m_pAddIn->countAndShow(m_pCurrentActivateDoc);
//		//return S_OK;
//	}
//	return E_FAIL;
//}

STDMETHODIMP_(HRESULT __stdcall) CWordEvents::OnDocumentChange()
{
	if (m_pAddIn != nullptr)
	{
		CComPtr<_Document> doc;
        HRESULT hr = m_pAddIn->m_spWordApp->get_ActiveDocument(&doc);
		if (SUCCEEDED(hr))
		{
			m_pAddIn->initializeCountDialog();
			m_pAddIn->countAndShow(doc);
		}
		return S_OK;
	}
	return E_FAIL;

}

STDMETHODIMP_(HRESULT __stdcall) CWordEvents::OnFormatButtonClick(Office::_CommandBarButton* ctrl, VARIANT_BOOL* CancelDefault)
{
	if (m_pAddIn != nullptr)
	{
		m_pAddIn->formatSelectionText();
	}
	return S_OK;
}
