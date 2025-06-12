// WordAddin.cpp: CWordAddin 的实现

#include "pch.h"
#include "WordAddin.h"


// CWordAddin

STDMETHODIMP CWordAddin::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IWordAddin
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CWordAddin::CountWords(LONG* pCount)
{
	//if (!pCount) return E_POINTER;

	//// 获取当前活动文档
	//_ApplicationPtr app;
	//app.CreateInstance(__uuidof(Application));

	//DocumentPtr doc = app->ActiveDocument;
	//if (doc == nullptr)
	//	return E_FAIL;

	//// 统计字数
	//*pCount = doc->Words->Count;

	return S_OK;
}


STDMETHODIMP_(HRESULT __stdcall) CWordAddin::raw_OnConnection(IDispatch* Application, ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom)
{
	return E_NOTIMPL;
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
