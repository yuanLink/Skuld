#pragma once

#include "../IDisposable.h"
#include "../UI/Displayable.h"
#include "../GlobalConfig.h"
#include "../Check.h"

namespace Skuld
{
	namespace Render3D
	{
		class Render3DContext;
		class Render3DFactory : public IDisposable
		{
		protected:
			virtual ~Render3DFactory() {}
		public:
			virtual Render3DContext* CreateContext(const WindowInfo* mWindow, const Config* mConfig) const = 0;
			virtual CheckFlags GetCheckFlag() const = 0;
		};
	}
}