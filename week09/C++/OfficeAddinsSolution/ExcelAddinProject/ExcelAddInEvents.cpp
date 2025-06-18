// ExcelAddInEvents.cpp: CExcelAddInEvents 的实现

#include "pch.h"
#include "ExcelAddInEvents.h"
#include "ExcelAddIn.h"


// CExcelAddInEvents

STDMETHODIMP_(HRESULT __stdcall) CExcelAddInEvents::OnWorkBookActivate(Excel::_Workbook* wk)
{
	if (wk == nullptr || m_pAddIn == nullptr)
	{
		return E_FAIL;
	}
	m_pAddIn->OnWorkBookActivate(Excel::_WorkbookPtr(wk));
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddInEvents::OnSheetActivate(IDispatch* sheet)
{
	if (sheet == nullptr || m_pAddIn == nullptr)
	{
		return E_FAIL;
	}
	m_pAddIn->OnSheetActivate(CComPtr<IDispatch>(sheet));
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddInEvents::OnSheetChange(IDispatch* sheet, Range* Target)
{
	if (sheet == nullptr || Target == nullptr)
	{
		return E_FAIL;
	}
	m_pAddIn->OnSheetChange(IDispatchPtr(sheet), Excel::RangePtr(Target));
	return S_OK;
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

