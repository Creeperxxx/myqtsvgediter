// ExcelAddIn.cpp: CExcelAddIn 的实现

#include "pch.h"
#include "ExcelAddIn.h"

#include "ExcelAddInEvents.h"
#include <exception>

#include "nonemptycellscountdialog.h"

#include <vector>

#include "Resource.h"

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
	//CreateCommandBar();
	initializeExcelEvents();
	RegisterApplicationEvents(m_spExcelApp);
	//CreateCommandBar();
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
	//CreateCommandBar();
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::raw_OnBeginShutdown(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}


STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::raw_GetCustomUI(BSTR RibbonID, BSTR* RibbonXml)
{
	UNREFERENCED_PARAMETER(RibbonID);

	HRSRC hResource = FindResourceW(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCEW(IDR_XML_RESOURCE1), L"XML_RESOURCE");
	if (hResource == nullptr)
	{
		return E_FAIL;
	}

	HGLOBAL hLoadedResource = LoadResource(_AtlBaseModule.GetModuleInstance(), hResource);
	if (hLoadedResource == nullptr)
	{
		return E_FAIL;
	}

	LPVOID pResourceData = LockResource(hLoadedResource);
	if (pResourceData == nullptr)
	{
		return E_FAIL;
	}

	DWORD size = SizeofResource(_AtlBaseModule.GetModuleInstance(), hResource);
	if (size == 0)
	{
		FreeResource(hLoadedResource);
		return E_FAIL;
	}

	LPWSTR ribbonXmlContent = static_cast<LPWSTR>(pResourceData);

	// 分配并复制字符串，因为 SysAllocString 需要一个可以持久存在的字符串
	*RibbonXml = SysAllocString(ribbonXmlContent);

	// 解锁和释放资源
	UnlockResource(hLoadedResource);
	FreeResource(hLoadedResource);

	return S_OK;





	//// 假设 XML 文件与 DLL 在同一目录下
	//WCHAR path[MAX_PATH];
	//GetModuleFileNameW(_AtlBaseModule.GetModuleInstance(), path, MAX_PATH);
	//PathRemoveFileSpecW(path);  // 获取模块路径
	//wcscat_s(path, L"\\myRibbon.xml");

	//HANDLE hFile = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL,
	//	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//if (hFile == INVALID_HANDLE_VALUE)
	//{
	//	return E_FAIL;
	//}

	//DWORD size = GetFileSize(hFile, NULL);
	//std::vector<char> buffer(size + 1);
	//DWORD read;
	//ReadFile(hFile, buffer.data(), size, &read, NULL);
	//CloseHandle(hFile);

	//buffer[read] = '\0';

	//// 将 UTF-8 转换为 Unicode
	//int wlen = MultiByteToWideChar(CP_UTF8, 0, buffer.data(), -1, NULL, 0);
	//std::vector<WCHAR> wbuffer(wlen);
	//MultiByteToWideChar(CP_UTF8, 0, buffer.data(), -1, wbuffer.data(), wlen);

	//*RibbonXml = SysAllocString(wbuffer.data());
	//return S_OK;
}


STDMETHODIMP_(HRESULT __stdcall) CExcelAddIn::onGenerateChartButtonClick(IDispatch* ribbonPtr)
{
	if (generateChart())
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
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
	SetupCountDialog();
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

void CExcelAddIn::SetupCountDialog()
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

bool CExcelAddIn::generateChart()
{
	IDispatchPtr dpActiveSheet;
	HRESULT hr = m_spExcelApp->get_ActiveSheet(&dpActiveSheet);
	if (FAILED(hr))
	{
		ATLTRACE("获取活跃sheet失败");
		return false;
	}
	Excel::_WorksheetPtr spActiveSheet;
	hr = dpActiveSheet->QueryInterface(__uuidof(Excel::_Worksheet), (void**)&spActiveSheet);
	if (FAILED(hr))
	{
		ATLTRACE("查询sheet接口失败");
		return false;
	}

	IDispatchPtr dpSelectedRange;
	Excel::RangePtr spSelectedRange;
	hr = m_spExcelApp->get_Selection(LOCALE_USER_DEFAULT, &dpSelectedRange);
	if (FAILED(hr))
	{
		ATLTRACE("获取选中区域失败");
		return false;
	}
	hr = dpSelectedRange->QueryInterface(__uuidof(Excel::Range), (void**)&spSelectedRange);
	if (FAILED(hr))
	{
		ATLTRACE("查询range接口失败");
		return false;
	}

	IDispatchPtr dpChartObjs;
	hr = spActiveSheet->raw_ChartObjects(vtMissing, LOCALE_USER_DEFAULT, &dpChartObjs);
	if (FAILED(hr))
	{
		ATLTRACE("获取ChartObjects失败");
		return false;
	}

	Excel::ChartObjectsPtr spChartObjs;
	hr = dpChartObjs->QueryInterface(__uuidof(Excel::ChartObjects), (void**)&spChartObjs);
	if (FAILED(hr))
	{
		ATLTRACE("查询ChartObjects接口失败");
		return false;
	}

	double left = spSelectedRange->GetLeft();
	double top = spSelectedRange->GetTop();
	double width = spSelectedRange->GetWidth();
	double height = spSelectedRange->GetHeight();

	Excel::ChartObjectPtr spChartObj = spChartObjs->Add(left, top, width, height);
	if (spChartObj == nullptr)
	{
		ATLTRACE("添加ChartObj失败");
		return false;
	}

	Excel::_ChartPtr spChart = spChartObj->GetChart();
	if (spChart == nullptr)
	{
		ATLTRACE("获取图表失败");
		return false;
	}

	hr = spChart->SetSourceData(spSelectedRange, xlColumns);
	if (FAILED(hr))
	{
		ATLTRACE("设置源数据失败");
		return false;
	}

	hr = spChart->put_ChartType(xlColumnClustered);
	if (FAILED(hr))
	{
		ATLTRACE("设置图表类型失败");
		return false;
	}
	return true;
}

//void CExcelAddIn::CreateCommandBar()
//{
//	Office::_CommandBarsPtr spCommandBars;
//	HRESULT hr = m_spExcelApp->get_CommandBars(&spCommandBars);
//	if (FAILED(hr))
//	{
//		ATLASSERT("获取命令栏失败");
//		return;
//	}
//	CComVariant vtName("aaaaa");
//	CComVariant vtPosition(Office::msoBarFloating);
//	CComVariant vtMenuBar(VARIANT_TRUE);
//	CComVariant vtTemporary(VARIANT_TRUE);
//	Office::CommandBarPtr spCommandBar = spCommandBars->Add(vtName, vtPosition, vtMenuBar, vtTemporary);
//	if (spCommandBar == nullptr)
//	{
//		ATLASSERT("创建命令栏错误");
//		return;
//	}
//
//	Office::CommandBarControlsPtr spCommandBarControls;
//	hr = spCommandBar->get_Controls(&spCommandBarControls);
//	if (FAILED(hr))
//	{
//		ATLASSERT("获取命令栏的controls错误");
//		return;
//	}
//	CComVariant vtType(Office::msoControlButton);
//	CComVariant vtId(1);
//	CComVariant vtInsertBefore(1);
//	CComVariant vtTemporaryTrue(VARIANT_TRUE);
//
//	Office::CommandBarControlPtr spButton = spCommandBarControls->Add(vtType, vtId, vtMissing, vtInsertBefore, vtTemporaryTrue);
//	if (spButton == nullptr)
//	{
//		ATLASSERT("按钮错误");
//		return;
//	}
//
//	spButton->put_Caption(CComBSTR(L"a"));
//	spButton->put_Visible(VARIANT_TRUE);
//}


