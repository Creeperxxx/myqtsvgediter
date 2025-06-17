// ExcelAddInEvents.cpp: CExcelAddInEvents 的实现

#include "pch.h"
#include "ExcelAddInEvents.h"
#include "ExcelAddIn.h"


// CExcelAddInEvents

STDMETHODIMP_(HRESULT __stdcall) CExcelAddInEvents::OnWorkBookOpen(Excel::_Workbook* wk)
{
	if (wk == nullptr || m_pAddIn == nullptr)
	{
		return E_FAIL;
	}
	return m_pAddIn->OnWorkBookActivate(wk);
}

STDMETHODIMP CExcelAddInEvents::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IExcelAddInEvents
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
