#include <EngineName/Support/Plugin.h>

extern "C"
{
	extern ENGINENAME::CreateRender2DFactoryFunc GetRender2DFactoryEntry();

	ENGINENAME_PLUGIN_EXPORT void *ENGINENAME_GetPluginEntry()
	{
		return (void*)GetRender2DFactoryEntry();
	}

	ENGINENAME_PLUGIN_EXPORT int ENGINENAME_GetPluginSig()
	{
		return ENGINENAME::PluginType::tRender2D;
	}
}