#include "D3D11Texture.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11Texture1D :public D3D11Texture
		{
		protected:
			virtual ~D3D11Texture1D() {}
			CComPtr<ID3D11Texture1D> mTexture1D;

			friend class D3D11Context;
			static D3D11Texture1D* Create(D3D11Context* mContext,
				const uint8_t * mPixels, uint32_t mWidth, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind);

			D3D11Texture1D(D3D11Context* mContext) : D3D11Texture(mContext) {}
		public:
		};
	}
}