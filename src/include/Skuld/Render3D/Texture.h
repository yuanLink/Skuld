#pragma once

#include "Render3DFactory.h"

namespace Skuld
{
	namespace Render3D
	{
		class Texture : public IDisposable
		{
		protected:
			virtual ~Texture() {}
		public:
			virtual const Render3DFactory* GetFactory() const = 0;
		};
	}
}