// dllmain.cpp: DllMain 的实现。

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "CreeperOfficeAddIn_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "xdlldata.h"

CCreeperOfficeAddInModule _AtlModule;

class CCreeperOfficeAddInApp : public CWinApp
{
public:

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CCreeperOfficeAddInApp, CWinApp)
END_MESSAGE_MAP()

CCreeperOfficeAddInApp theApp;

BOOL CCreeperOfficeAddInApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, nullptr))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CCreeperOfficeAddInApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
