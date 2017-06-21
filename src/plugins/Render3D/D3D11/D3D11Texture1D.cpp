#include "D3D11Texture1D.h"

#include "D3D11Utility.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11Texture1D * D3D11Texture1D::Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory,
			const uint8_t * mPixels, uint32_t mWidth, PixelFormat mPixelFormat)
		{
			CComPtr<ID3D11Texture1D> mTexture1D;
			CComPtr<ID3D11ShaderResourceView> mSRV;
			D3D11_TEXTURE1D_DESC mDesc;
			memset(&mDesc, 0, sizeof(mDesc));
			mDesc.Width = static_cast<UINT>(mWidth);
			mDesc.Format = PixelFormatToDXGIFormat(mPixelFormat);

			mDesc.ArraySize = 1;
			mDesc.MipLevels = 1;
			mDesc.Usage = D3D11_USAGE_IMMUTABLE;
			mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			if (mPixels != nullptr)
			{
				D3D11_SUBRESOURCE_DATA mInitialData;
				memset(&mInitialData, 0, sizeof(mInitialData));

				mInitialData.pSysMem = mPixels;
				mInitialData.SysMemPitch = PixelFormatDepth(mPixelFormat) * mWidth;
				mDevice->CreateTexture1D(&mDesc, &mInitialData, &mTexture1D);
			}
			else mDevice->CreateTexture1D(&mDesc, nullptr, &mTexture1D);

			mDevice->CreateShaderResourceView(mTexture1D, nullptr, &mSRV);

			return new D3D11Texture1D(mFactory, mTexture1D, mSRV);
		}
	}
}