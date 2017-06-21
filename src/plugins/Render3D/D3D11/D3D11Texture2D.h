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
			CComPtr<ID3D11ShaderResourceView> mSRV;

			friend class D3D11Context;
			static D3D11Texture2D* Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory,
				const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, PixelFormat mPixelFormat);

			D3D11Texture2D(const D3D11Factory* mFactory, CComPtr<ID3D11Texture2D> mTexture2D,
				CComPtr<ID3D11ShaderResourceView> mSRV) :
				D3D11Texture(mFactory),
				mTexture2D(mTexture2D),
				mSRV(mSRV) {}
		public:
		};
	}
}