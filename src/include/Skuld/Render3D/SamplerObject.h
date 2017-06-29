#pragma once

#include "Render3DFactory.h"

namespace Skuld
{
	namespace Render3D
	{
		class SamplerObject : public IDisposable
		{
		protected:
			virtual ~SamplerObject() {}
		public:
			virtual const Render3DFactory* GetFactory() const = 0;
		};
	}
}