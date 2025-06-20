// dllmain.h: 模块类的声明。

class CCreeperOfficeAddInModule : public ATL::CAtlDllModuleT< CCreeperOfficeAddInModule >
{
public :
	DECLARE_LIBID(LIBID_CreeperOfficeAddInLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CREEPEROFFICEADDIN, "{6589578e-3253-4ecf-835e-9fb71496a44b}")
};

extern class CCreeperOfficeAddInModule _AtlModule;
