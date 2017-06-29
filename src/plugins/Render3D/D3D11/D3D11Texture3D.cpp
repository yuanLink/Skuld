#include "D3D11Texture3D.h"

#include "D3D11Utility.h"
#include "D3D11Context.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11Texture3D * D3D11Texture3D::Create(D3D11Context* mContext,
			const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, uint32_t mDepth,
			PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind)
		{
			Ptr<D3D11Texture3D> mRet = new D3D11Texture3D(mContext);
			D3D11_TEXTURE3D_DESC mDesc;
			memset(&mDesc, 0, sizeof(mDesc));
			mDesc.Height = static_cast<UINT>(mHeight);
			mDesc.Width = static_cast<UINT>(mWidth);
			mDesc.Depth = static_cast<UINT>(mDepth);
			mDesc.Format = PixelFormatToDXGIFormat(mPixelFormat);

			mDesc.MipLevels = 1;

			mDesc.Usage = AccessFlagToUsage(mAccess);

			if (mDesc.Usage == D3D11_USAGE_STAGING)
			{
				if (mAccess & Access_CPURead) mDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
				if (mAccess & Access_CPUWrite) mDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
			}
			mDesc.BindFlags = TextureBindFlagToD3D11BindFlag(mBind);

			std::unique_ptr<D3D11_SUBRESOURCE_DATA> mInitialData = nullptr;
			if (mPixels != nullptr)
			{
				mInitialData = std::make_unique<D3D11_SUBRESOURCE_DATA>();
				memset(mInitialData.get(), 0, sizeof(D3D11_SUBRESOURCE_DATA));

				mInitialData->pSysMem = mPixels;
				mInitialData->SysMemPitch = PixelFormatDepth(mPixelFormat) / 8 * mWidth;
				mInitialData->SysMemSlicePitch = mInitialData->SysMemPitch * mHeight;
			}
			HRESULT hr = mContext->D3DDevice()->CreateTexture3D(&mDesc, mInitialData.get(), &mRet->mTexture3D);

			RETURN_NULL_IF_FAILED(hr);

			if (mBind & TextureBind_ShaderResource)
				RETURN_NULL_IF_FAILED(mContext->D3DDevice()->CreateShaderResourceView(mRet->mTexture3D,
					nullptr, &mRet->mSRV));

			return mRet.Detach();
		}
	}
}