// dllmain.cpp: DllMain 的实现。

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "PPTAddinProject_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "xdlldata.h"

CPPTAddinProjectModule _AtlModule;

class CPPTAddinProjectApp : public CWinApp
{
public:

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CPPTAddinProjectApp, CWinApp)
END_MESSAGE_MAP()

CPPTAddinProjectApp theApp;

BOOL CPPTAddinProjectApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, nullptr))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CPPTAddinProjectApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
