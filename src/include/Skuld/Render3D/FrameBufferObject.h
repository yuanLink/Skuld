#pragma once

#include "Render3DFactory.h"

namespace Skuld
{
	namespace Render3D
	{
		class FrameBufferObject : public IDisposable
		{
		protected:
			virtual ~FrameBufferObject() {}
		public:
			virtual const Render3DFactory* GetFactory() const = 0;
		};
	}
}