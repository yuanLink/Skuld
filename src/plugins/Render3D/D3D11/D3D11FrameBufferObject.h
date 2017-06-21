#pragma once

#include <Skuld/Render3D/FrameBufferObject.h>
#include "D3D11Factory.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11FrameBufferObject : public FrameBufferObject
		{
		protected:
			virtual ~D3D11FrameBufferObject() {}

			const D3D11Factory* mFactory;
			CComPtr<ID3D11Texture2D> mColorBuffer;
			CComPtr<ID3D11Texture2D> mDepthBuffer;
			CComPtr<ID3D11RenderTargetView> mRenderTargetView;
			CComPtr<ID3D11DepthStencilView> mDepthStencilView;

			friend class D3D11Factory;
			friend class D3D11Context;
			static D3D11FrameBufferObject* Create(
				const D3D11Factory* mFactory,
				CComPtr<ID3D11Device> mDevice,
				CComPtr<ID3D11Texture2D> mResource,
				uint32_t mMSAAQuality);

			D3D11FrameBufferObject(
				const D3D11Factory* mFactory,
				CComPtr<ID3D11Texture2D> mColorBuffer,
				CComPtr<ID3D11Texture2D> mDepthBuffer,
				CComPtr<ID3D11RenderTargetView> mRenderTargetView,
				CComPtr<ID3D11DepthStencilView> mDepthStencilView) :
				mFactory(mFactory),
				mColorBuffer(mColorBuffer),
				mDepthBuffer(mDepthBuffer),
				mRenderTargetView(mRenderTargetView),
				mDepthStencilView(mDepthStencilView)
			{}
		public:
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}