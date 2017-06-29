#pragma once

#include <Skuld/Render3D/Texture.h>
#include "D3D11Factory.h"
#include <Skuld/PixelFormat.h>
#include <Skuld/Render3D/Render3DContext.h>

namespace Skuld
{
	namespace Render3D
	{
		class D3D11Texture : public Texture
		{
		protected:
			virtual ~D3D11Texture() {}

			D3D11Context* mContext;
			CComPtr<ID3D11ShaderResourceView> mSRV;

			D3D11Texture(D3D11Context* mContext) : mContext(mContext) {}
		public:
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}