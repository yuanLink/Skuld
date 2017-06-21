#pragma once

#include "Render3DFactory.h"

namespace Skuld
{
	namespace Render3D
	{
		class BufferObject : public IDisposable
		{
		protected:
			virtual ~BufferObject() {}
		public:
			virtual const Render3DFactory* GetFactory() const = 0;
		};
	}
}