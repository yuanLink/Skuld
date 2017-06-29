#include <Skuld/Plugin.h>

extern "C"
{
	extern Skuld::CreateAudioFactoryFunc* GetAudioFactoryEntry();

	SKULD_PLUGIN_EXPORT void *Skuld_GetPluginEntry()
	{
		return (void*)GetAudioFactoryEntry();
	}

	SKULD_PLUGIN_EXPORT int Skuld_GetPluginSig()
	{
		return Skuld::PluginType::tAudio;
	}
}