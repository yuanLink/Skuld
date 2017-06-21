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
			CComPtr<ID3D11ShaderResourceView> mSRV;
			const Render3DFactory* mFactory;

			friend class D3D11Context;
			static D3D11Texture3D* Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory,
				const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat);

			D3D11Texture3D(const D3D11Factory* mFactory, CComPtr<ID3D11Texture3D> mTexture3D,
				CComPtr<ID3D11ShaderResourceView> mSRV) :
				D3D11Texture(mFactory),
				mTexture3D(mTexture3D),
				mSRV(mSRV) {}
		public:
		};
	}
}