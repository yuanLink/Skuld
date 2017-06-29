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

			D3D11Context* mContext;
			CComPtr<ID3D11Texture2D> mColorBuffer;
			CComPtr<ID3D11Texture2D> mDepthBuffer;
			CComPtr<ID3D11RenderTargetView> mRenderTargetView;
			CComPtr<ID3D11DepthStencilView> mDepthStencilView;

			friend class D3D11Factory;
			friend class D3D11Context;
			static D3D11FrameBufferObject* Create(D3D11Context* mContext,
				CComPtr<ID3D11Texture2D> mResource,
				uint32_t mMSAAQuality);

			D3D11FrameBufferObject(D3D11Context* mContext) : mContext(mContext) {}
		public:
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}