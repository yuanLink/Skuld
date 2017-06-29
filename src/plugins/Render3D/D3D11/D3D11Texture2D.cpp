#include "D3D11Texture2D.h"

#include "D3D11Utility.h"
#include "D3D11Context.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11Texture2DHelper
		{
		public:
			static inline HRESULT CreateShaderResourceView(D3D11Context* mContext, D3D11Texture2D* mRet)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC mDesc;
				memset(&mDesc, 0, sizeof(mDesc));
				return mContext->D3DDevice()->CreateShaderResourceView(mRet->mTexture2D,
					nullptr, &mRet->mSRV);
			}
		};

		D3D11Texture2D * D3D11Texture2D::Create(D3D11Context* mContext,
			const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight,
			PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind)
		{
			Ptr<D3D11Texture2D> mRet = new D3D11Texture2D(mContext);
			D3D11_TEXTURE2D_DESC mDesc;
			memset(&mDesc, 0, sizeof(mDesc));
			mDesc.Height = static_cast<UINT>(mHeight);
			mDesc.Width = static_cast<UINT>(mWidth);
			mDesc.Format = PixelFormatToDXGIFormat(mPixelFormat);

			mDesc.ArraySize = 1;
			mDesc.SampleDesc.Count = 1;
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
			}
			HRESULT hr = mContext->D3DDevice()->CreateTexture2D(&mDesc, mInitialData.get(), &mRet->mTexture2D);

			RETURN_NULL_IF_FAILED(hr);

			if (mBind & TextureBind_ShaderResource)
				RETURN_NULL_IF_FAILED(D3D11Texture2DHelper::CreateShaderResourceView(mContext, mRet));

			return mRet.Detach();
		}
	}
}