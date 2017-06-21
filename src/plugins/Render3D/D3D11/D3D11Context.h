#pragma once

#include <Skuld/Render3D/Render3DContext.h>
#include "D3D11Factory.h"
#include "D3D11FrameBufferObject.h"
#include <Skuld/Ptr.hpp>

namespace Skuld
{
	namespace Render3D
	{
		class D3D11Context : public Render3DContext
		{
		protected:
			virtual ~D3D11Context() {}
			const D3D11Factory* mFactory;

			D3D_DRIVER_TYPE mDriverType;
			D3D_FEATURE_LEVEL mFeatureLevel;
			CComPtr<ID3D11Device> mDevice;
			CComPtr<ID3D11DeviceContext> mContext;
			CComPtr<IDXGISwapChain> mSwapChain;
			Ptr<D3D11FrameBufferObject> mFBO;

			friend class D3D11Factory;
			D3D11Context(const D3D11Factory* mFactory,
				D3D_DRIVER_TYPE mDriverType,
				D3D_FEATURE_LEVEL mFeatureLevel,
				CComPtr<ID3D11Device> mDevice,
				CComPtr<ID3D11DeviceContext> mContext,
				CComPtr<IDXGISwapChain> mSwapChain,
				Ptr<D3D11FrameBufferObject> mFBO) :

				mFactory(mFactory),
				mDriverType(mDriverType),
				mFeatureLevel(mFeatureLevel),
				mDevice(mDevice),
				mContext(mContext),
				mSwapChain(mSwapChain),
				mFBO(mFBO)
			{}
		public:
			virtual const Render3DFactory* GetFactory() const;
			virtual Shader* CreateShader(const uint8_t* mCode, size_t mCodeSize, ShaderType mType);
			virtual InputLayout* CreateInputLayout(const ShaderInputLayoutAttri* mAttri, size_t mSize, Shader* mShader);
			virtual Texture* CreateTexture1D(const uint8_t * mPixels, uint32_t mWidth, PixelFormat mPixelFormat);
			virtual Texture* CreateTexture2D(const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, PixelFormat mPixelFormat);
			virtual Texture* CreateTexture3D(const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat);

			virtual void SetShader(ShaderType mType, Shader* mShader);
		};
	}
}