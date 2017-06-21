#include <EngineName/Support/Plugin.h>

extern "C"
{
	extern ENGINENAME::CreateAudioDeviceFunc GetAudioDeviceEntry();

	ENGINENAME_PLUGIN_EXPORT void *ENGINENAME_GetPluginEntry()
	{
		return (void*)GetAudioDeviceEntry();
	}

	ENGINENAME_PLUGIN_EXPORT int ENGINENAME_GetPluginSig()
	{
		return ENGINENAME::PluginType::tAudio;
	}
}