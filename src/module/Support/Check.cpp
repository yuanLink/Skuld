#include <Skuld/Check.h>
#include <Skuld/Render3D/Render3DFactory.h>
#include "PluginEntry.h"

#ifndef SKULD_SYSTEM_NAME
#error "未定义SKULD_SYSTEM_NAME"
#endif
#ifndef SKULD_ARCH_NAME
#error "未定义SKULD_ARCH_NAME"
#endif

#define _SKULD_SYSTEM_FLAG(sys) AvaliablePlatform_ ## sys
#define SKULD_SYSTEM_FLAG(sys) _SKULD_SYSTEM_FLAG(sys)

#define _SKULD_ARCH_FLAG(arch) AvaliableArch_ ## arch
#define SKULD_ARCH_FLAG(arch) _SKULD_ARCH_FLAG(arch)

namespace Skuld
{
	CheckFlags GetCurrentCheckFlags(const SkuldEngine* mEngine)
	{
		uint32_t mCurrentFlag = SKULD_SYSTEM_FLAG(SKULD_SYSTEM_NAME);
		mCurrentFlag |= SKULD_ARCH_FLAG(SKULD_ARCH_NAME);

		if (mEngine->CreateRender3DFactory)
		{
			Ptr<Render3D::Render3DFactory> mFactory(mEngine->CreateRender3DFactory());
			mCurrentFlag |= mFactory->GetCheckFlag().mFlag;
		}
		return mCurrentFlag;
	}
}