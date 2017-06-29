#include <Skuld/Plugin.h>

extern "C"
{
	extern Skuld::CreateRender2DFactoryFunc* GetRender2DFactoryEntry();

	SKULD_PLUGIN_EXPORT void *Skuld_GetPluginEntry()
	{
		return (void*)GetRender2DFactoryEntry();
	}

	SKULD_PLUGIN_EXPORT int Skuld_GetPluginSig()
	{
		return Skuld::PluginType::tRender2D;
	}
}