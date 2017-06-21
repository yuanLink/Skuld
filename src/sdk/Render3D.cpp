#include <Skuld/Plugin.h>

extern "C"
{
	extern Skuld::CreateRender3DFactoryFunc* GetRender3DFactoryEntry();

	SKULD_PLUGIN_EXPORT void *Skuld_GetPluginEntry()
	{
		return (void*)GetRender3DFactoryEntry();
	}

	SKULD_PLUGIN_EXPORT int Skuld_GetPluginSig()
	{
		return Skuld::PluginType::tRender3D;
	}
}