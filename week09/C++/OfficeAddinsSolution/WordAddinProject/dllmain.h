// dllmain.h: 模块类的声明。

class CWordAddinProjectModule : public ATL::CAtlDllModuleT< CWordAddinProjectModule >
{
public :
	DECLARE_LIBID(LIBID_WordAddinProjectLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WORDADDINPROJECT, "{69001a9b-9ab6-4e30-b56f-c939d43dba8a}")
};

extern class CWordAddinProjectModule _AtlModule;
