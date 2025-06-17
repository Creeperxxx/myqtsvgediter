// dllmain.h: 模块类的声明。

class CExcelAddinProjectModule : public ATL::CAtlDllModuleT< CExcelAddinProjectModule >
{
public :
	DECLARE_LIBID(LIBID_ExcelAddinProjectLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EXCELADDINPROJECT, "{5535cf69-cdca-474a-9ce3-fe70852b9f5f}")
};

extern class CExcelAddinProjectModule _AtlModule;
