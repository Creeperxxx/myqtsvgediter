// ExcelAddIn.cpp: CExcelAddIn 的实现

#include "pch.h"
#include "ExcelAddIn.h"

#include "ExcelAddInEvents.h"


// CExcelAddIn

STDMETHODIMP CExcelAddIn::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IExcelAddIn
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::raw_OnConnection(IDispatch* Application, ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom)
{
	HRESULT hr = Application->QueryInterface(__uuidof(Excel::_Application), (void**)&m_spExcelApp);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = initializeExcelEvents();
	if (FAILED(hr))
	{
		return hr;
	}
	RegisterApplicationEvents(m_spExcelApp);


	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::raw_OnDisconnection(ext_DisconnectMode RemoveMode, SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::raw_OnAddInsUpdate(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::raw_OnStartupComplete(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::raw_OnBeginShutdown(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::initializeExcelEvents()
{
	HRESULT hr = CComObject<CExcelAddInEvents>::CreateInstance(&m_pExcelEvents);
	if (FAILED(hr))
	{
		return hr;
	}
	m_pExcelEvents->m_pAddIn = this;
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::OnWorkBookActivate(Excel::_Workbook* pBook)
{
	//Excel::_WorksheetPtr spSheetPtr;
	CComPtr<IDispatch> spDispatch;
	HRESULT hr = pBook->get_ActiveSheet(&spDispatch);
	if (FAILED(hr))
	{
		return hr;
	}
	Excel::_WorksheetPtr spSheetPtr;
	hr = spDispatch->QueryInterface(__uuidof(Excel::_Worksheet),(void**)&spSheetPtr);
	if (FAILED(hr))
	{
		return hr;
	}
	LONG count = 0;
	return countNonEmptyCells(spSheetPtr, &count);
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::countNonEmptyCells(Excel::_WorksheetPtr pSheet, LONG* count)
{
	if (pSheet == nullptr)
	{
		return E_FAIL;
	}
	Excel::RangePtr spUsedRange;
	HRESULT hr = pSheet->get_UsedRange(0, &spUsedRange);
	if (FAILED(hr))
	{
		return hr;
	}
	LONG rowCount = 0;
	LONG colCount = 0;
	rowCount = spUsedRange->GetRow();
	colCount = spUsedRange->GetColumn();

	long NonEmptyCellsCount = 0;
	for (LONG i = 1; i <= rowCount; i++)
	{
		for (LONG j = 1; j <= colCount; j++)
		{
			Excel::RangePtr cell;
			CComVariant index = spUsedRange->GetItem(CComVariant(i), CComVariant(j));
			if (index.vt != VT_EMPTY && index.vt != VT_ERROR)
			{
				NonEmptyCellsCount++;
			}
		}
	}

	CString msg;
	msg.Format(L"非空单元格数量：%ld", NonEmptyCellsCount);
	MessageBox(NULL, msg, L"统计结果", MB_OK);
	return S_OK;

}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::RegisterApplicationEvents(Excel::_ApplicationPtr pApp)
{

	if (m_pExcelEvents == nullptr || m_spExcelApp == nullptr)
	{
		return E_FAIL;
	}
	HRESULT hr = m_pExcelEvents->IDispEventImpl<1, CExcelAddInEvents, &__uuidof(Excel::AppEvents), &__uuidof(Excel::__Excel), 1, 9>::DispEventAdvise(m_spExcelApp);
	return hr;
}

