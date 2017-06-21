#include "D3D11FrameBufferObject.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11FrameBufferObject * D3D11FrameBufferObject::Create(
			const D3D11Factory* mFactory,
			CComPtr<ID3D11Device> mDevice,
			CComPtr<ID3D11Texture2D> mColorBuffer,
			uint32_t mMSAAQuality)
		{
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

			CComPtr<ID3D11Texture2D> mDepthBuffer;
			mDevice->CreateTexture2D(&mDepthBufferDesc, nullptr, &mDepthBuffer);

			D3D11_DEPTH_STENCIL_VIEW_DESC mDSVDesc;
			memset(&mDSVDesc, 0, sizeof(mDSVDesc));
			mDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			mDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			mDSVDesc.Texture2D.MipSlice = 0;

			CComPtr<ID3D11DepthStencilView> mDepthStencilView;
			mDevice->CreateDepthStencilView(mDepthBuffer, &mDSVDesc, &mDepthStencilView);

			CComPtr<ID3D11RenderTargetView> mRenderTargetView;
			mDevice->CreateRenderTargetView(mColorBuffer, nullptr, &mRenderTargetView);

			return new D3D11FrameBufferObject(mFactory,
				mColorBuffer, mDepthBuffer, mRenderTargetView, mDepthStencilView);
		}

		const Render3DFactory * D3D11FrameBufferObject::GetFactory() const
		{
			return mFactory;
		}
	}
}