// ExcelAddIn.cpp: CExcelAddIn 的实现

#include "pch.h"
#include "ExcelAddIn.h"

#include "ExcelAddInEvents.h"
#include <exception>

#include "nonemptycellscountdialog.h"


// CExcelAddIn

STDMETHODIMP CExcelAddIn::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =
	{
		&IID_IExcelAddIn
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
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
	initializeExcelEvents();
	RegisterApplicationEvents(m_spExcelApp);
	//InitializeCountDialog();


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
	InitializeCountDialog();
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::raw_OnBeginShutdown(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}


//STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::initializeExcelEvents()
//{
//	HRESULT hr = CComObject<CExcelAddInEvents>::CreateInstance(&m_pExcelEvents);
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//	m_pExcelEvents->m_pAddIn = this;
//	return S_OK;
//}

//STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::OnSheetActivate(IDispatch* sheet)
//{
//	CComPtr<IDispatch> cSheet(sheet);
//	if (sheet == nullptr)
//	{
//		return E_FAIL;
//	}
//	Excel::_WorksheetPtr spSheetPtr;
//	HRESULT hr = cSheet->QueryInterface(__uuidof(Excel::_Worksheet), (void**)&spSheetPtr);
//	if (FAILED(hr))
//	{
//		return E_FAIL;
//	}
//	return CountAndShowNonEmptyCells(spSheetPtr);
//}
//
//STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::OnWorkBookActivate(Excel::_Workbook* pBook)
//{
//	CComPtr<IDispatch> spDispatch;
//	HRESULT hr = pBook->get_ActiveSheet(&spDispatch);
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//	Excel::_WorksheetPtr spSheetPtr;
//	hr = spDispatch->QueryInterface(__uuidof(Excel::_Worksheet),(void**)&spSheetPtr);
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//	return CountAndShowNonEmptyCells(spSheetPtr);
//}

//STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::countNonEmptyCells(Excel::_WorksheetPtr pSheet, LONG* count)
//{
//	if (pSheet == nullptr)
//	{
//		return E_FAIL;
//	}
//	Excel::RangePtr spUsedRange;
//	HRESULT hr = pSheet->get_UsedRange(0, &spUsedRange);
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//
//	try
//	{
//		Excel::RangePtr nonEmptyRanges = spUsedRange->SpecialCells(Excel::xlCellTypeConstants, Excel::xlTextValues | Excel::xlNumbers | Excel::xlLogical);
//		if (nonEmptyRanges != nullptr)
//		{
//			*count = nonEmptyRanges->GetCount();
//		}
//		else
//		{
//			*count = 0;
//		}
//	}
//	catch (_com_error& e)
//	{
//		ATLTRACE(_T("COM Error: Code=0x%08X, Description=%s\n"), e.Error(), e.ErrorMessage());
//		*count = 0;
//	}
//
//	return S_OK;
//
//}



//STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::ShowNonEmptyCells(LONG* count)
//{
//	CString msg;
//	msg.Format(L"非空单元格数量：%ld", *count);
//	MessageBox(NULL, msg, L"统计结果", MB_OK);
//	return S_OK;
//}

//STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::CountAndShowNonEmptyCells(Excel::_WorksheetPtr pSheet)
//{
//	HRESULT hr = E_FAIL;
//	if (pSheet == nullptr)
//	{
//		return hr;
//	}
//	LONG count = 0;
//	hr = countNonEmptyCells(pSheet, &count);
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//	ShowNonEmptyCells(count);
//	return;
//}


void CExcelAddIn::initializeExcelEvents()
{
	HRESULT hr = CComObject<CExcelAddInEvents>::CreateInstance(&m_pExcelEvents);
	if (FAILED(hr))
	{
		ATLASSERT(_T("创建实例失败"));
		return;
	}
	m_pExcelEvents->m_pAddIn = this;
}

void CExcelAddIn::ShowNonEmptyCells(LONG count)
{
	if (m_countDialog == nullptr || m_countDialog.get() == nullptr)
	{
		return;
	}
	m_countDialog->ShowCount(count);
	return;
}

void CExcelAddIn::RegisterApplicationEvents(Excel::_ApplicationPtr pApp)
{
	if (m_pExcelEvents == nullptr || m_spExcelApp == nullptr)
	{
		ATLASSERT("指针为空");
		return;
	}
	HRESULT hr = m_pExcelEvents->IDispEventImpl<1, CExcelAddInEvents, &__uuidof(Excel::AppEvents), &__uuidof(Excel::__Excel), 1, 9>::DispEventAdvise(m_spExcelApp);
	if (FAILED(hr))
	{
		ATLASSERT("注册事件失败");
		return;
	}
}

void CExcelAddIn::OnSheetActivate(CComPtr<IDispatch> pSheet)
{
	if (pSheet == nullptr)
	{
		ATLTRACE(_T("sheet指针为空"));
		return;
	}
	Excel::_WorksheetPtr spSheetPtr;
	HRESULT hr = pSheet->QueryInterface(__uuidof(Excel::_Worksheet), (void**)&spSheetPtr);
	if (FAILED(hr))
	{
		ATLTRACE(_T("sheet查询接口失败"));
		return;
	}
	CountAndShowNonEmptyCells(spSheetPtr);
}

void CExcelAddIn::OnWorkBookActivate(Excel::_WorkbookPtr pBook)
{
	CComPtr<IDispatch> spDispatch;
	HRESULT hr = pBook->get_ActiveSheet(&spDispatch);
	if (FAILED(hr))
	{
		ATLTRACE(_T("获取活动sheet失败"));
		return;
	}
	Excel::_WorksheetPtr spSheetPtr;
	hr = spDispatch->QueryInterface(__uuidof(Excel::_Worksheet), (void**)&spSheetPtr);
	if (FAILED(hr))
	{
		ATLTRACE(_T("查询接口失败"));
		return;
	}
	CountAndShowNonEmptyCells(spSheetPtr);
	return;
}

void CExcelAddIn::OnSheetChange(IDispatchPtr sh, Excel::RangePtr rg)
{
	if (sh == nullptr || rg == nullptr)
	{
		ATLASSERT("sheet指针和range指针为空");
		return;
	}
	Excel::_WorksheetPtr spSheetPtr;
	HRESULT hr = sh->QueryInterface(__uuidof(Excel::_Worksheet), (void**)&spSheetPtr);
	if (FAILED(hr))
	{
		ATLASSERT("查询接口失败");
		return;
	}
	CountAndShowNonEmptyCells(spSheetPtr);
	return;
}

void CExcelAddIn::CountAndShowNonEmptyCells(Excel::_WorksheetPtr pSheet)
{
	if (pSheet == nullptr)
	{
		ATLTRACE(_T("psheet指针为空"));
		return;
	}
	LONG count = 0;
	CountNonEmptyCells(pSheet, &count);
	setupCountDialog();
	ShowNonEmptyCells(count);
	return;
}

void CExcelAddIn::CountNonEmptyCells(Excel::_WorksheetPtr pSheet, LONG* count)
{
	if (pSheet == nullptr)
	{
		ATLTRACE(_T("pSheet指针为空"));
		return;
	}
	Excel::RangePtr spUsedRange;
	HRESULT hr = pSheet->get_UsedRange(0, &spUsedRange);
	if (FAILED(hr))
	{
		ATLTRACE(_T("获取使用过的范围失败"));
		return;
	}

	try
	{
		Excel::RangePtr nonEmptyRanges = spUsedRange->SpecialCells(Excel::xlCellTypeConstants, Excel::xlTextValues | Excel::xlNumbers | Excel::xlLogical);
		if (nonEmptyRanges != nullptr)
		{
			*count = nonEmptyRanges->GetCount();
		}
		else
		{
			*count = 0;
		}
	}
	catch (_com_error& e)
	{
		ATLTRACE(_T("COM Error: Code=0x%08X, Description=%s\n"), e.Error(), e.ErrorMessage());
		*count = 0;
	}
	return;
}

void CExcelAddIn::InitializeCountDialog()
{
	if (m_countDialog == nullptr || m_countDialog.get() == nullptr)
	{
		ATLASSERT("对话框指针为空");
	}
	m_countDialog->Create(nullptr);
	m_countDialog->ShowWindow(SW_SHOW);
	return;
}

HWND CExcelAddIn::getActivateWindow()
{
	if (m_spExcelApp == nullptr)
	{
		return nullptr;
	}
	Excel::ExcelWindowPtr spExcelWindow;
	HRESULT hr = m_spExcelApp->get_ActiveWindow(&spExcelWindow);
	if (FAILED(hr))
	{
		return nullptr;
	}
	return reinterpret_cast<HWND>((LONG_PTR)spExcelWindow->GetHwnd());
}

void CExcelAddIn::setupCountDialog()
{
	if (m_countDialog == nullptr || m_countDialog.get() == nullptr)
	{
		ATLASSERT("对话框指针为空");
		return;
	}
	if (m_countDialog->IsWindow())
	{
		m_countDialog->SetParent(getActivateWindow());
		return;
	}
	else
	{
		m_countDialog->Create(getActivateWindow());
		m_countDialog->ShowWindow(SW_SHOW);
		return;
	}
}



