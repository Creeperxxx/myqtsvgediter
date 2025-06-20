// pptddin.cpp: CPPTAddIn 的实现

#include "pch.h"
#include "pptaddin.h"

#include "resource.h"


// CPPTAddIn

CPPTAddIn::CPPTAddIn()
	:m_pUnkMarshaler(nullptr)
{

}


STDMETHODIMP CPPTAddIn::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =
	{
		&IID_IPPTAddIn
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CPPTAddIn::FinalConstruct()
{
	return CoCreateFreeThreadedMarshaler(
		GetControllingUnknown(), &m_pUnkMarshaler.p);
}

void CPPTAddIn::FinalRelease()
{
	m_pUnkMarshaler.Release();
}

STDMETHODIMP_(HRESULT __stdcall) CPPTAddIn::raw_OnConnection(IDispatch* Application, AddInDesignerObjects::ext_ConnectMode ConnectMode, IDispatch* AddInInst, SAFEARRAY** custom)
{
	HRESULT hr = Application->QueryInterface(__uuidof(PPT::_Application), (void**)&m_spPPTApp);
	if (FAILED(hr))
	{
		ATLASSERT("查询ppt Application接口失败");
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CPPTAddIn::raw_OnDisconnection(AddInDesignerObjects::ext_DisconnectMode RemoveMode, SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CPPTAddIn::raw_OnAddInsUpdate(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CPPTAddIn::raw_OnStartupComplete(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CPPTAddIn::raw_OnBeginShutdown(SAFEARRAY** custom)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CPPTAddIn::raw_GetCustomUI(BSTR RibbonID, BSTR* RibbonXml)
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
}

STDMETHODIMP_(HRESULT __stdcall) CPPTAddIn::OnCountNonEmptyTextShapes(IDispatch* ribbonPtr)
{
	uint64_t count = 0;
	if (!getNonEmptyTextShapesCount(count))
	{
		ATLTRACE("获取非空文档框数量失败");
		return E_FAIL;
	}
	showNonEmptyTextShapesCount(count);
	return S_OK;
}

bool CPPTAddIn::getNonEmptyTextShapesCount(uint64_t& count)
{
	count = 0;

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
			count++;
		}
		SysFreeString(text);
	}
	return true;
}

void CPPTAddIn::showNonEmptyTextShapesCount(uint64_t count)
{
	CString message;
	message.Format(L"非空文本框数量：%llu", static_cast<unsigned long long>(count));

	MessageBox(nullptr, message, L"统计结果", MB_ICONINFORMATION | MB_OK);
}

STDMETHODIMP_(HRESULT __stdcall) CPPTAddIn::OnInsertSlideAndSetThemeBackground(IDispatch* ribbonPtr)
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

