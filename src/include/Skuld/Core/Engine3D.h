#pragma once

#include "../IDisposable.h"
#include "../UI/UI.h"
#include "../UI/Displayable.h"
#include "../Plugin.h"
#include "Effect.h"

namespace Skuld
{
	namespace Core
	{
		class Engine3D : public IDisposable
		{
		protected:
			virtual ~Engine3D() { }
		public:
			static SKULD_EXPORT Engine3D* Create3DEngine(SkuldEngine* mEngine,
				UI::Displayable* mDisplayable);
			virtual void LoadEffect(Effect* mEffect) = 0;
		};
	}
}