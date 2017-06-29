#include "D3D11Texture.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11Texture2D :public D3D11Texture
		{
		protected:
			virtual ~D3D11Texture2D() {}
			CComPtr<ID3D11Texture2D> mTexture2D;

#ifdef __d3d11_3_h__
			CComPtr<ID3D11Texture2D1> mTexture2D1;
#endif
			friend class D3D11Texture2DHelper;
			friend class D3D11Context;
			static D3D11Texture2D* Create(D3D11Context* mContext,
				const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight,
				PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind);

			D3D11Texture2D(D3D11Context* mContext) : D3D11Texture(mContext) {}
		public:
		};
	}
}