#include "D3D11Texture3D.h"

#include "D3D11Utility.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11Texture3D * D3D11Texture3D::Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory,
			const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat)
		{
			CComPtr<ID3D11Texture3D> mTexture3D;
			CComPtr<ID3D11ShaderResourceView> mSRV;
			D3D11_TEXTURE3D_DESC mDesc;
			memset(&mDesc, 0, sizeof(mDesc));
			mDesc.Height = static_cast<UINT>(mHeight);
			mDesc.Width = static_cast<UINT>(mWidth);
			mDesc.Depth = static_cast<UINT>(mDepth);
			mDesc.Format = PixelFormatToDXGIFormat(mPixelFormat);

			mDesc.MipLevels = 1;
			mDesc.Usage = D3D11_USAGE_IMMUTABLE;
			mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			if (mPixels != nullptr)
			{
				D3D11_SUBRESOURCE_DATA mInitialData;
				memset(&mInitialData, 0, sizeof(mInitialData));

				mInitialData.pSysMem = mPixels;
				mInitialData.SysMemPitch = PixelFormatDepth(mPixelFormat) * mWidth;
				mDevice->CreateTexture3D(&mDesc, &mInitialData, &mTexture3D);
			}
			else mDevice->CreateTexture3D(&mDesc, nullptr, &mTexture3D);

			mDevice->CreateShaderResourceView(mTexture3D, nullptr, &mSRV);

			return new D3D11Texture3D(mFactory, mTexture3D, mSRV);
		}
	}
}