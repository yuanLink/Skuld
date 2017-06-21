#pragma once

#include <Skuld/Render3D/Texture.h>
#include "D3D11Factory.h"
#include <Skuld/PixelFormat.h>

namespace Skuld
{
	namespace Render3D
	{
		class D3D11Texture : public Texture
		{
		protected:
			virtual ~D3D11Texture() {}

			const D3D11Factory* mFactory;

			D3D11Texture(const D3D11Factory* mFactory) : mFactory(mFactory) {}
		public:
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}