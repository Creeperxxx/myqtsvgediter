// creeperaddin.cpp: CCreeperAddIn 的实现

#include "pch.h"
#include "creeperaddin.h"

#include "resource.h"

#include <regex>

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
	UNREFERENCED_PARAMETER(RibbonID);
	HRSRC hResource;
	if (m_spWordApp != nullptr)
	{
		hResource = FindResourceW(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCEW(IDR_XML_RESOURCE1), L"XML_RESOURCE");
	}
	else if (m_spExcelApp != nullptr)
	{
		hResource = FindResourceW(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCEW(IDR_XML_RESOURCE2), L"XML_RESOURCE");
	}
	else if (m_spPPTApp != nullptr)
	{
		hResource = FindResourceW(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCEW(IDR_XML_RESOURCE3), L"XML_RESOURCE");
	}
	else
	{
		ATLASSERT("app指针全为空");
		return E_FAIL;
	}

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
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::OnCountWords(IDispatch* ribbonPtr)
{
	uint64_t chineseCount = 0;
	uint64_t englishCount = 0;
	if (!calculateActiveDocWords(chineseCount, englishCount))
	{
		ATLTRACE("计算doc中中英文词数失败");
		return E_FAIL;
	}
	showDocWords(chineseCount, englishCount);
	return S_OK;
}

bool CCreeperAddIn::calculateActiveDocWords(uint64_t& chineseCount, uint64_t& englishCount)
{
	if (m_spWordApp == nullptr)
	{
		ATLTRACE("wordapp指针为空");
		return false;
	}

	Word::_DocumentPtr spActiveDoc;
	HRESULT hr = m_spWordApp->get_ActiveDocument(&spActiveDoc);
	if (FAILED(hr) || spActiveDoc == nullptr)
	{
		ATLTRACE("获取活跃文档失败");
		return false;
	}

	chineseCount = 0;
	englishCount = 0;

	Word::RangePtr spRange = nullptr;
	hr = spActiveDoc->get_Content(&spRange);
	if (FAILED(hr) || !spRange)
	{
		ATLTRACE("获取内容失败");
		return false;
	}

	// 获取文本
	CComBSTR bstrText = nullptr;
	hr = spRange->get_Text(&bstrText);
	if (FAILED(hr) || bstrText == nullptr)
	{
		ATLTRACE("获取文本失败");
		return false;
	}

	std::wstring text(bstrText);

	std::wregex chineseRegex(L"[\\u4E00-\\u9FFF]");
	auto chineseBegin = std::wsregex_iterator(text.begin(), text.end(), chineseRegex);
	auto chineseEnd = std::wsregex_iterator();
	chineseCount = static_cast<uint64_t>(std::distance(chineseBegin, chineseEnd));

	std::wregex englishRegex(L"\\b[A-Za-z]+\\b");
	auto englishBegin = std::wsregex_iterator(text.begin(), text.end(), englishRegex);
	auto englishEnd = std::wsregex_iterator();
	englishCount = static_cast<uint64_t>(std::distance(englishBegin, englishEnd));

	return true;
}

void CCreeperAddIn::showDocWords(uint64_t chineseCount, uint64_t englishCount)
{
	// 创建并格式化要显示的消息
	CAtlString message;
	message.Format(_T("中文词汇数量: %llu\n英文词汇数量: %llu"), static_cast<unsigned long long>(chineseCount), static_cast<unsigned long long>(englishCount));

	// 显示消息框
	AfxMessageBox(message, MB_ICONINFORMATION);
	return;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::OnFormatSelection(IDispatch* ribbonPtr)
{
	if (m_spWordApp == nullptr)
	{
		ATLTRACE("app指针为空");
		return E_FAIL;
	}
	Word::SelectionPtr spSelection;
	HRESULT hr = m_spWordApp->get_Selection(&spSelection);
	if (FAILED(hr) || spSelection == nullptr)
	{
		ATLTRACE("选择为空");
		return E_FAIL;
	}

	Word::RangePtr spRange;
	hr = spSelection->get_Range(&spRange);
	if (FAILED(hr) || spRange == nullptr)
	{
		ATLTRACE("获取Range失败");
		return E_FAIL;
	}
	float indent = 24;
	CComPtr<Word::_ParagraphFormat> spParaFormat;
	hr = spRange->get_ParagraphFormat(&spParaFormat);
	if (FAILED(hr) || spParaFormat == nullptr)
	{
		ATLTRACE("获取ParagraphFormat失败");
		return E_FAIL;
	}
	hr = spParaFormat->put_FirstLineIndent(indent);
	if (FAILED(hr))
	{
		ATLTRACE("putFirstLineIndent失败");
		return E_FAIL;
	}
	hr = spParaFormat->put_LineSpacingRule(Word::wdLineSpace1pt5);
	if (FAILED(hr))
	{
		ATLTRACE("putLineSpaceingRule失败");
		return E_FAIL;
	}
	hr = m_spWordApp->Activate();
	if (FAILED(hr))
	{
		ATLTRACE("激活app失败");
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::OnCountNonEmptyTextCells(IDispatch* ribbonPtr)
{
	uint64_t cellsCount = 0;
	if (!calcuActiveSheetNonEmptyCells(cellsCount))
	{
		ATLTRACE("获取非空单元格失败");
		return E_FAIL;
	}
	showSheetNonEmptyCellsCount(cellsCount);
	return S_OK;
}

bool CCreeperAddIn::calcuActiveSheetNonEmptyCells(uint64_t& cellsCount)
{
	IDispatchPtr dpSheet;
	HRESULT hr = m_spExcelApp->get_ActiveSheet(&dpSheet);
	if (FAILED(hr) || dpSheet == nullptr)
	{
		ATLTRACE("获取活跃的sheet失败");
		return false;
	}

	Excel::_WorksheetPtr spSheet;
	hr = dpSheet->QueryInterface(__uuidof(Excel::_Worksheet), (void**)&spSheet);
	if (FAILED(hr) || spSheet == nullptr)
	{
		ATLTRACE("查询sheet接口失败");
		return false;
	}

	Excel::RangePtr spUsedRange;
	hr = spSheet->get_UsedRange(LOCALE_USER_DEFAULT, &spUsedRange);
	if (FAILED(hr))
	{
		ATLTRACE(_T("获取使用过的范围失败"));
		return false;
	}

	try
	{
		Excel::RangePtr nonEmptyRanges = spUsedRange->SpecialCells(Excel::xlCellTypeConstants, Excel::xlTextValues | Excel::xlNumbers | Excel::xlLogical);
		if (nonEmptyRanges != nullptr)
		{
			cellsCount = nonEmptyRanges->GetCount();
		}
		else
		{
			cellsCount = 0;
		}
	}
	catch (_com_error& e)
	{
		//ATLTRACE(_T("COM Error: Code=0x%08X, Description=%s\n"), e.Error(), e.ErrorMessage());
		ATLTRACE("计数cells失败");
		cellsCount = 0;
	}
	return true;
}

void CCreeperAddIn::showSheetNonEmptyCellsCount(uint64_t cellsCount)
{
	// 创建并格式化要显示的消息
	CAtlString message;
	message.Format(_T("非空单元格数量: %llu"), static_cast<unsigned long long>(cellsCount));

	// 显示消息框
	AfxMessageBox(message, MB_ICONINFORMATION);
	return;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::OnGenerateChart(IDispatch* ribbonPtr)
{
	IDispatchPtr dpActiveSheet;
	HRESULT hr = m_spExcelApp->get_ActiveSheet(&dpActiveSheet);
	if (FAILED(hr))
	{
		ATLTRACE("获取活跃sheet失败");
		return E_FAIL;
	}
	Excel::_WorksheetPtr spActiveSheet;
	hr = dpActiveSheet->QueryInterface(__uuidof(Excel::_Worksheet), (void**)&spActiveSheet);
	if (FAILED(hr))
	{
		ATLTRACE("查询sheet接口失败");
		return E_FAIL;
	}

	IDispatchPtr dpSelectedRange;
	Excel::RangePtr spSelectedRange;
	hr = m_spExcelApp->get_Selection(LOCALE_USER_DEFAULT, &dpSelectedRange);
	if (FAILED(hr))
	{
		ATLTRACE("获取选中区域失败");
		return E_FAIL;
	}
	hr = dpSelectedRange->QueryInterface(__uuidof(Excel::Range), (void**)&spSelectedRange);
	if (FAILED(hr))
	{
		ATLTRACE("查询range接口失败");
		return E_FAIL;
	}

	IDispatchPtr dpChartObjs;
	hr = spActiveSheet->raw_ChartObjects(vtMissing, LOCALE_USER_DEFAULT, &dpChartObjs);
	if (FAILED(hr))
	{
		ATLTRACE("获取ChartObjects失败");
		return E_FAIL;
	}

	Excel::ChartObjectsPtr spChartObjs;
	hr = dpChartObjs->QueryInterface(__uuidof(Excel::ChartObjects), (void**)&spChartObjs);
	if (FAILED(hr))
	{
		ATLTRACE("查询ChartObjects接口失败");
		return E_FAIL;
	}

	double left = spSelectedRange->GetLeft();
	double top = spSelectedRange->GetTop();
	double width = spSelectedRange->GetWidth();
	double height = spSelectedRange->GetHeight();

	Excel::ChartObjectPtr spChartObj = spChartObjs->Add(left, top, width, height);
	if (spChartObj == nullptr)
	{
		ATLTRACE("添加ChartObj失败");
		return E_FAIL;
	}

	Excel::_ChartPtr spChart = spChartObj->GetChart();
	if (spChart == nullptr)
	{
		ATLTRACE("获取图表失败");
		return E_FAIL;
	}

	hr = spChart->SetSourceData(spSelectedRange, Excel::xlColumns);
	if (FAILED(hr))
	{
		ATLTRACE("设置源数据失败");
		return E_FAIL;
	}

	hr = spChart->put_ChartType(Excel::xlColumnClustered);
	if (FAILED(hr))
	{
		ATLTRACE("设置图表类型失败");
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::OnCountNonEmptyTextShapes(IDispatch* ribbonPtr)
{
	uint64_t shapesCount = 0;
	if (!calcuActiveSlideNonEmptyTextShapes(shapesCount))
	{
		ATLTRACE("计算幻灯片的非空文本框失败");
		return E_FAIL;
	}
	showShapesCount(shapesCount);
	return S_OK;
}

bool CCreeperAddIn::calcuActiveSlideNonEmptyTextShapes(uint64_t& shapesCount)
{
	shapesCount = 0;

	if (m_spPPTApp == nullptr)
	{
		ATLTRACE("app指针为空");
		return false;
	}

	PPT::_PresentationPtr spActivePres;
	HRESULT hr = m_spPPTApp->get_ActivePresentation(&spActivePres);
	if (FAILED(hr) || spActivePres == nullptr)
	{
		ATLTRACE("获取活跃presentation失败");
		return false;
	}

	PPT::DocumentWindowPtr spDocWindow;
	hr = m_spPPTApp->get_ActiveWindow(&spDocWindow);
	if (FAILED(hr) || spDocWindow == nullptr)
	{
		ATLTRACE("获取活跃窗口失败");
		return false;
	}

	PPT::ViewPtr spView;
	hr = spDocWindow->get_View(&spView);
	if (FAILED(hr) || spView == nullptr)
	{
		ATLTRACE("获取当前视图失败");
		return false;
	}

	IDispatchPtr dpSlide;
	hr = spView->get_Slide(&dpSlide);
	if (FAILED(hr) || dpSlide == nullptr)
	{
		ATLTRACE("获取当前幻灯片失败");
		return false;
	}

	PPT::_SlidePtr spSlide;
	hr = dpSlide->QueryInterface(__uuidof(PPT::_Slide), (void**)&spSlide);
	if (FAILED(hr) || spSlide == nullptr)
	{
		ATLTRACE("查询slide接口失败");
		return false;
	}

	PPT::ShapesPtr spShapes;
	hr = spSlide->get_Shapes(&spShapes);
	if (FAILED(hr) || spShapes == nullptr)
	{
		ATLTRACE("获取幻灯片所有形状失败");
		return false;
	}

	int shapeCount = 0;
	hr = spShapes->get_Count(&shapeCount);
	if (FAILED(hr))
	{
		ATLTRACE("获取幻灯片的形状的数量失败");
		return false;
	}

	for (int i = 1; i < shapeCount + 1; i++)
	{
		PPT::PPT_ShapePtr spShape = spShapes->Item(CComVariant(i));
		if (spShape == nullptr)
		{
			ATLTRACE("获取形状失败");
			return false;
		}

		Office::MsoShapeType shapeType;
		hr = spShape->get_Type(&shapeType);
		if (FAILED(hr))
		{
			ATLTRACE("获取shape的type失败");
			return false;
		}

		if (shapeType != Office::msoTextBox && shapeType != Office::msoPlaceholder)
		{
			continue;
		}

		PPT::TextFramePtr spTextFrame;
		hr = spShape->get_TextFrame(&spTextFrame);
		if (FAILED(hr) || spTextFrame == nullptr)
		{
			ATLTRACE("获取shape的textFrame失败");
			return false;
		}

		PPT::PPT_TextRangePtr spTextRange;
		hr = spTextFrame->get_PPT_TextRange(&spTextRange);
		if (FAILED(hr) || spTextRange == nullptr)
		{
			ATLTRACE("获取textFrame的textRange失败");
			return false;
		}

		BSTR text = nullptr;
		hr = spTextRange->get_Text(&text);
		if (FAILED(hr) || text == nullptr)
		{
			ATLTRACE("获取textRange的text失败");
			return false;
		}

		if (wcslen(text) > 0)
		{
			shapesCount++;
		}
		SysFreeString(text);
	}
	return true;
}

void CCreeperAddIn::showShapesCount(uint64_t shapesCount)
{
	CAtlString message;
	message.Format(_T("非空文本框数量: %llu"), static_cast<unsigned long long>(shapesCount));

	// 显示消息框
	AfxMessageBox(message, MB_ICONINFORMATION);
	return;
}

STDMETHODIMP_(HRESULT __stdcall) CCreeperAddIn::OnInsertSlideAndSetThemeBackground(IDispatch* ribbonPtr)
{
	if (m_spPPTApp == nullptr)
	{
		ATLTRACE("app指针为空");
		return E_FAIL;
	}

	PPT::_PresentationPtr spPresentation;
	HRESULT hr = m_spPPTApp->get_ActivePresentation(&spPresentation);
	if (FAILED(hr) || spPresentation == nullptr)
	{
		PPT::PresentationsPtr spPresentations;
		hr = m_spPPTApp->get_Presentations(&spPresentations);
		if (FAILED(hr) || spPresentations == nullptr)
		{
			ATLTRACE("获取presentations失败");
			return E_FAIL;
		}

		spPresentation = spPresentations->Add(Office::MsoTriState::msoTrue);
		if (spPresentation == nullptr)
		{
			ATLTRACE("创建presentation失败");
			return E_FAIL;
		}
	}

	PPT::SlidesPtr spSlides;
	hr = spPresentation->get_Slides(&spSlides);
	if (FAILED(hr) || spSlides == nullptr)
	{
		ATLTRACE("获取slides失败");
		return E_FAIL;
	}

	long slideCount = 0;
	hr = spSlides->get_Count(&slideCount);
	if (FAILED(hr))
	{
		ATLTRACE("获取slides的count失败");
		return E_FAIL;
	}

	PPT::_SlidePtr spNewSlide = spSlides->Add(slideCount + 1, PPT::PpSlideLayout::ppLayoutBlank);
	if (spNewSlide == nullptr)
	{
		ATLTRACE("新建slide失败");
		return E_FAIL;
	}

	PPT::ShapeRangePtr spBackground = nullptr;
	hr = spNewSlide->get_Background(&spBackground);
	if (FAILED(hr) || spBackground == nullptr)
	{
		ATLTRACE("获取slide的background失败");
		return E_FAIL;
	}

	PPT::FillFormatPtr spFill;
	hr = spBackground->get_Fill(&spFill);
	if (FAILED(hr) || spFill == nullptr)
	{
		ATLTRACE("获取background的Fill失败");
		return E_FAIL;
	}

	hr = spFill->OneColorGradient(Office::msoGradientHorizontal, 1, 1.0);
	if (FAILED(hr))
	{
		ATLTRACE("为fill设置oneColorGradient失败");
		return E_FAIL;
	}

	PPT::ColorFormatPtr spColor;
	hr = spFill->get_ForeColor(&spColor);
	if (FAILED(hr) || spColor == nullptr)
	{
		ATLTRACE("获取colorFormat失败");
		return E_FAIL;
	}

	hr = spColor->put_ObjectThemeColor(Office::MsoThemeColorIndex::msoThemeColorAccent1);
	if (FAILED(hr))
	{
		ATLTRACE("设置color的themeColor失败");
		return E_FAIL;
	}

	return S_OK;
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



