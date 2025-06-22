// dllmain.cpp: DllMain 的实现。

#include "../headers/pch.h"
#include "../headers/framework.h"
#include "../headers/resource.h"
#include "../headers/creeperofficeaddini.h"
#include "../headers/dllmain.h"
#include "../headers/compreg.h"
#include "../headers/xdlldata.h"

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
