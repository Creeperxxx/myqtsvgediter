// dllmain.h: 模块类的声明。

class CPPTAddinProjectModule : public ATL::CAtlDllModuleT< CPPTAddinProjectModule >
{
public :
	DECLARE_LIBID(LIBID_PPTAddinProjectLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PPTADDINPROJECT, "{ed6bc220-018c-4b8f-9f88-8121de3881fc}")
};

extern class CPPTAddinProjectModule _AtlModule;
