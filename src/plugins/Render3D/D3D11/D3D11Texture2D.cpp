#include "D3D11Texture2D.h"

#include "D3D11Utility.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11Texture2D * D3D11Texture2D::Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory,
			const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, PixelFormat mPixelFormat)
		{
			CComPtr<ID3D11Texture2D> mTexture2D;
			CComPtr<ID3D11ShaderResourceView> mSRV;
			D3D11_TEXTURE2D_DESC mDesc;
			memset(&mDesc, 0, sizeof(mDesc));
			mDesc.Height = static_cast<UINT>(mHeight);
			mDesc.Width = static_cast<UINT>(mWidth);
			mDesc.Format = PixelFormatToDXGIFormat(mPixelFormat);

			mDesc.ArraySize = 1;
			mDesc.SampleDesc.Count = 1;
			mDesc.MipLevels = 1;
			mDesc.Usage = D3D11_USAGE_IMMUTABLE;
			mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			if (mPixels != nullptr)
			{
				D3D11_SUBRESOURCE_DATA mInitialData;
				memset(&mInitialData, 0, sizeof(mInitialData));

				mInitialData.pSysMem = mPixels;
				mInitialData.SysMemPitch = PixelFormatDepth(mPixelFormat) * mWidth;
				mDevice->CreateTexture2D(&mDesc, &mInitialData, &mTexture2D);
			}
			else mDevice->CreateTexture2D(&mDesc, nullptr, &mTexture2D);

			mDevice->CreateShaderResourceView(mTexture2D, nullptr, &mSRV);

			return new D3D11Texture2D(mFactory, mTexture2D, mSRV);
		}
	}
}