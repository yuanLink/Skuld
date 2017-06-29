#include "D3D11Texture.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11Texture3D :public D3D11Texture
		{
		protected:
			virtual ~D3D11Texture3D() {}
			CComPtr<ID3D11Texture3D> mTexture3D;

			friend class D3D11Context;
			static D3D11Texture3D* Create(D3D11Context* mContext,
				const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, uint32_t mDepth,
				PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind);

			D3D11Texture3D(D3D11Context* mContext) : D3D11Texture(mContext) {}
		public:
		};
	}
}