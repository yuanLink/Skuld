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

extern "C" Skuld::CreateAudioFactoryFunc* GetAudioFactoryEntry() {
	return Skuld::Audio::CreateAudio_XAudio2Factory;
}