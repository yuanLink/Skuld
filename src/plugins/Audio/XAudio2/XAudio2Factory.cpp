#include <Skuld/Plugin.h>

namespace Skuld
{
	namespace Audio
	{
		AudioFactory* CreateAudio_XAudio2Factory()
		{
			return nullptr;
		}
	}
}

#ifdef SKULD_BUILD_DYNAMIC

extern "C" Skuld::CreateAudioFactoryFunc* GetAudioFactoryEntry() {
	return Skuld::Audio::CreateAudio_XAudio2Factory;
}

#endif