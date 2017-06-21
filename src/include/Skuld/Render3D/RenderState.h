#pragma once
#include "Render3DFactory.h"

namespace Skuld
{
	namespace Render3D
	{
		class RenderState : public IDisposable
		{
		protected:
			virtual ~RenderState() {}
		public:
			virtual const Render3DFactory *GetFactory() const = 0;
		};
	}
}