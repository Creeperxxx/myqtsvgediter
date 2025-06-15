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

STDMETHODIMP CWordEvents::OnDocumentOpen(IDispatch* Doc)
{
	if (m_pAddIn)
	{
		// 文档打开后，调用插件的统计字数函数
		//m_pAddIn->CountWords();
		//m_pAddin->CountWordsInDocument(Doc);
		m_pAddIn->initializeCountDialog();
		m_pAddIn->countAndShow(Doc);
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}
