// pptddin.cpp: CPPTAddIn 的实现

#include "pch.h"
#include "pptaddin.h"


// CPPTAddIn

STDMETHODIMP CPPTAddIn::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IPPTAddIn
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
