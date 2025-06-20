// creeperaddin.cpp: CCreeperAddIn 的实现

#include "pch.h"
#include "creeperaddin.h"



// CCreeperAddIn

CCreeperAddIn::CCreeperAddIn()
{
	m_pUnkMarshaler = nullptr;
}

STDMETHODIMP CCreeperAddIn::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =
	{
		&IID_ICreeperAddIn
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CCreeperAddIn::FinalConstruct()
{
	return CoCreateFreeThreadedMarshaler(
		GetControllingUnknown(), &m_pUnkMarshaler.p);
}

void CCreeperAddIn::FinalRelease()
{
	m_pUnkMarshaler.Release();
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::raw_OnConnection(IDispatch* Application, AddInDesignerObjects::ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom)
{
	if (!initApp(Application))
	{
		return E_FAIL;
	}

	MessageBox(nullptr, L"Connected!", L"Info", MB_OK);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::raw_OnDisconnection(AddInDesignerObjects::ext_DisconnectMode RemoveMode, SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::raw_OnAddInsUpdate(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::raw_OnStartupComplete(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::raw_OnBeginShutdown(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::raw_GetCustomUI(BSTR RibbonID, BSTR* RibbonXml)
{
	return E_NOTIMPL;
}

bool CCreeperAddIn::initApp(IDispatch* application)
{
	HRESULT hr = application->QueryInterface(__uuidof(Word::_Application), (void**)&m_spWordApp);
	if (SUCCEEDED(hr) && m_spWordApp != nullptr)
	{
		return true;
	}

	hr = application->QueryInterface(__uuidof(Excel::_Application), (void**)&m_spExcelApp);
	if (SUCCEEDED(hr) && m_spExcelApp != nullptr)
	{
		return true;
	}

	hr = application->QueryInterface(__uuidof(PPT::_Application), (void**)&m_spPPTApp);
	if (SUCCEEDED(hr) && m_spPPTApp != nullptr)
	{
		return true;
	}

	return false;
}

bool CCreeperAddIn::isWordAppValid()
{
	if (m_spWordApp == nullptr)
	{
		return false;
	}

	bool flag = true;
	try
	{
		BSTR version = nullptr;
		HRESULT hr = m_spWordApp->get_Version(&version);
		if (FAILED(hr))
		{
			flag = false;
		}
		if (version)
		{
			SysFreeString(version);
		}
	}
	catch (_com_error e)
	{
		flag = false;
	}

	if (flag)
	{
		return true;
	}
	else
	{
		m_spWordApp = nullptr;
		return false;
	}
}

bool CCreeperAddIn::isExcelAppValid()
{
	if (m_spExcelApp == nullptr)
	{
		return false;
	}

	bool flag = true;
	try
	{
		BSTR version = nullptr;
		HRESULT hr = m_spExcelApp->get_Version(LOCALE_USER_DEFAULT, &version);
		if (FAILED(hr))
		{
			flag = false;
		}
		if (version)
		{
			SysFreeString(version);
		}
	}
	catch (_com_error e)
	{
		flag = false;
	}
	
	if (flag)
	{
		return true;
	}
	else
	{
		m_spExcelApp = nullptr;
		return false;
	}
}

bool CCreeperAddIn::isPPTAppValid()
{
	if (m_spPPTApp == nullptr)
	{
		return false;
	}

	bool flag = true;
	try
	{
		BSTR version = nullptr;
		HRESULT hr = m_spPPTApp->get_Version(&version);
		if (FAILED(hr) || version == nullptr)
		{
			flag = false;
		}
		if (version)
		{
			SysFreeString(version);
		}
	}
	catch (_com_error e)
	{
		flag = false;
	}

	if (flag)
	{
		return true;
	}
	else
	{
		m_spPPTApp = nullptr;
		return false;
	}

}


