#include "D3D11FrameBufferObject.h"
#include "D3D11Context.h"

#include "D3D11Utility.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11FrameBufferObject * D3D11FrameBufferObject::Create(
			D3D11Context* mContext,
			CComPtr<ID3D11Texture2D> mColorBuffer,
			uint32_t mMSAAQuality)
		{
			Ptr<D3D11FrameBufferObject> mRet = new D3D11FrameBufferObject(mContext);
			D3D11_TEXTURE2D_DESC mColorBufferDesc;
			D3D11_TEXTURE2D_DESC mDepthBufferDesc;
			mColorBuffer->GetDesc(&mColorBufferDesc);

			mDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			mDepthBufferDesc.Width = mColorBufferDesc.Width;
			mDepthBufferDesc.Height = mColorBufferDesc.Height;
			mDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			mDepthBufferDesc.MipLevels = 1;
			mDepthBufferDesc.ArraySize = 1;
			mDepthBufferDesc.CPUAccessFlags = 0;
			mDepthBufferDesc.SampleDesc.Count = mMSAAQuality < 1 ? 1 : 4;
			mDepthBufferDesc.SampleDesc.Quality = mMSAAQuality < 1 ? 0 : mMSAAQuality - 1;
			mDepthBufferDesc.MiscFlags = 0;
			mDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;

			mContext->D3DDevice()->CreateTexture2D(&mDepthBufferDesc, nullptr, &mRet->mDepthBuffer);

			D3D11_DEPTH_STENCIL_VIEW_DESC mDSVDesc;
			memset(&mDSVDesc, 0, sizeof(mDSVDesc));
			mDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			mDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			mDSVDesc.Texture2D.MipSlice = 0;

			RETURN_NULL_IF_FAILED(mContext->D3DDevice()->CreateDepthStencilView(mRet->mDepthBuffer,
				&mDSVDesc, &mRet->mDepthStencilView));
			RETURN_NULL_IF_FAILED(mContext->D3DDevice()->CreateRenderTargetView(mColorBuffer, 
				nullptr, &mRet->mRenderTargetView));

			mRet->mColorBuffer = mColorBuffer;

			return new D3D11FrameBufferObject(mContext);
		}

		const Render3DFactory * D3D11FrameBufferObject::GetFactory() const
		{
			return mContext->GetFactory();
		}
	}
}