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
			CComPtr<ID3D11ShaderResourceView> mSRV;

			friend class D3D11Context;
			static D3D11Texture1D* Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory,
				const uint8_t * mPixels, uint32_t mWidth, PixelFormat mPixelFormat);

			D3D11Texture1D(const D3D11Factory* mFactory, CComPtr<ID3D11Texture1D> mTexture1D,
				CComPtr<ID3D11ShaderResourceView> mSRV) :
				D3D11Texture(mFactory),
				mTexture1D(mTexture1D),
				mSRV(mSRV) {}
		public:
		};
	}
}