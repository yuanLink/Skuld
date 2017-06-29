#pragma once

#include "Plugin.h"

namespace Skuld
{
	const uint32_t AvaliableArch_AMD64 = 1 << 31;
	const uint32_t AvaliableArch_X86 = 1 << 30;
	const uint32_t AvaliableArch_ARM64 = 1 << 29;
	const uint32_t AvaliableArch_ARM = 1 << 28;

	const uint32_t AvaliableRender3DAPI_D3D11 = 1 << 27;
	const uint32_t AvaliableRender3DAPI_D3D12 = 1 << 26;
	const uint32_t AvaliableRender3DAPI_OpenGL = 1 << 25;
	const uint32_t AvaliableRender3DAPI_Vulkan = 1 << 24;
	const uint32_t AvaliableRender3DAPI_Metal = 1 << 23;
	const uint32_t AvaliableRender3DAPI_OpenGLES = 1 << 22;

	const uint32_t AvaliableAudioAPI_XAudio2 = 1 << 13;
	const uint32_t AvaliableAudioAPI_OpenAL = 1 << 14;
	const uint32_t AvaliableAudioAPI_OpenSLES = 1 << 15;

	const uint32_t AvaliableRender2DAPI_D2D = 1 << 17;
	const uint32_t AvaliableRender2DAPI_OpenGL = 1 << 18;
	const uint32_t AvaliableRender2DAPI_Metal = 1 << 19;
	const uint32_t AvaliableRender2DAPI_OpenGLES = 1 << 20;
	const uint32_t AvaliableRender2DAPI_Vulkan = 1 << 21;
	
	const uint32_t AvaliablePlatform_Windows = 1 << 0;
	const uint32_t AvaliablePlatform_Linux = 1 << 1;
	const uint32_t AvaliablePlatform_OSX = 1 << 2;
	const uint32_t AvaliablePlatform_Android = 1 << 3;
	const uint32_t AvaliablePlatform_iOS = 1 << 4;
	const uint32_t AvaliablePlatform_WindowsPhone = 1 << 5;
	const uint32_t AvaliablePlatform_UWP = 1 << 6;
	const uint32_t AvaliablePlatform_Tizen = 1 << 7;
	const uint32_t AvaliablePlatform_Fuchsia = 1 << 8;
	const uint32_t AvaliablePlatform_Switch = 1 << 9;
	const uint32_t AvaliablePlatform_PS4 = 1 << 10;
	const uint32_t AvaliablePlatform_XboxOne = 1 << 11;
	const uint32_t AvaliablePlatform_PSV = 1 << 12;

	class CheckFlags
	{
	public:
		uint32_t mFlag;

		CheckFlags(uint32_t mFlag = 0xFFFFFFFF) : mFlag(mFlag) {}

		CheckFlags Set(CheckFlags mFlag) const {
			return (this->mFlag | mFlag.mFlag);
		}
		CheckFlags Unset(CheckFlags mFlag) const {
			return (this->mFlag & (~mFlag.mFlag));
		}
		CheckFlags SetAllPlatform() const {
			return (this->mFlag | 0x00001FFF);
		}
		CheckFlags SetAllRender3DAPI() const {
			return (this->mFlag | 0x0FC00000);
		}
		CheckFlags SetAllRender2DAPI() const {
			return (this->mFlag | 0x003E0000);
		}
		CheckFlags SetAllAudioAPI() const {
			return (this->mFlag | 0x0000E000);
		}
		CheckFlags SetAllArch() const {
			return (this->mFlag | 0xF0000000);
		}

		CheckFlags UnsetAllPlatform() const {
			return (this->mFlag & 0xFFFFE000);
		}
		CheckFlags UnsetAllRender3DAPI() const {
			return (this->mFlag & 0xF03FFFFF);
		}
		CheckFlags UnsetAllRender2DAPI() const {
			return (this->mFlag & 0xFFC1FFFF);
		}
		CheckFlags UnsetAllAudioAPI() const {
			return (this->mFlag & 0xFFFF1FFF);
		}
		CheckFlags UnsetAllArch() const {
			return (this->mFlag & 0x0FFFFFFF);
		}
	};

	SKULD_EXPORT CheckFlags GetCurrentCheckFlags(const SkuldEngine*);

	static inline bool IsCheckFlagsAvaliable(CheckFlags mCurrentFlag, CheckFlags mFlag)
	{
		return (mCurrentFlag.mFlag & mFlag.mFlag) == mCurrentFlag.mFlag;
	}
}