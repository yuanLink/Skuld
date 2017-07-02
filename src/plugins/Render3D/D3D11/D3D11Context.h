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
#ifdef __d3d11_1_h__
		protected:
			CComPtr<ID3D11Device1> mDevice1;
			CComPtr<ID3D11DeviceContext1> mContext1;
		public:
			inline ID3D11Device1* D3DDevice1() { return mDevice1; }
			inline ID3D11DeviceContext1* D3DContext1() { return mContext1; }
#endif
#ifdef __d3d11_2_h__
		protected:
			CComPtr<ID3D11Device2> mDevice2;
			CComPtr<ID3D11DeviceContext2> mContext2;
		public:
			inline ID3D11Device2* D3DDevice2() { return mDevice2; }
			inline ID3D11DeviceContext2* D3DContext2() { return mContext2; }
#endif
#ifdef __d3d11_3_h__
		protected:
			CComPtr<ID3D11Device3> mDevice3;
			CComPtr<ID3D11DeviceContext3> mContext3;
			CComPtr<ID3D11DeviceContext4> mContext4;
		public:
			inline ID3D11Device3* D3DDevice3() { return mDevice3; }
			inline ID3D11DeviceContext3* D3DContext3() { return mContext3; }
			inline ID3D11DeviceContext4* D3DContext4() { return mContext4; }
#endif
#ifdef __d3d11_4_h__
		protected:
			CComPtr<ID3D11Device4> mDevice4;
			CComPtr<ID3D11Device5> mDevice5;
		public:
			inline ID3D11Device4* D3DDevice4() { return mDevice4; }
			inline ID3D11Device5* D3DDevice5() { return mDevice5; }
#endif
		protected:

			friend class D3D11Factory;
			D3D11Context(const D3D11Factory* mFactory,
				D3D_DRIVER_TYPE mDriverType,
				D3D_FEATURE_LEVEL mFeatureLevel,
				CComPtr<ID3D11Device> mDevice,
				CComPtr<ID3D11DeviceContext> mContext,
				CComPtr<IDXGISwapChain> mSwapChain) :

				mFactory(mFactory),
				mDriverType(mDriverType),
				mFeatureLevel(mFeatureLevel),
				mDevice(mDevice),
				mContext(mContext),
				mSwapChain(mSwapChain)
			{
#ifdef __d3d11_1_h__
				mDevice->QueryInterface(&mDevice1);
				mContext->QueryInterface(&mContext1);
#endif
#ifdef __d3d11_2_h__
				mDevice->QueryInterface(&mDevice2);
				mContext->QueryInterface(&mContext2);
#endif
#ifdef __d3d11_3_h__
				mDevice->QueryInterface(&mDevice3);
				mContext->QueryInterface(&mContext3);
				mContext->QueryInterface(&mContext4);
#endif
#ifdef __d3d11_4_h__
				mDevice->QueryInterface(&mDevice4);
				mDevice->QueryInterface(&mDevice5);
#endif
			}
		public:
			virtual const Render3DFactory* GetFactory() const;

			virtual BufferObject* CreateBufferObject(const void* mData, size_t mBufferSize, AccessFlag mAccess, BufferBindFlag mBind);
			virtual ShaderObject* CreateShader(const uint8_t* mCode, size_t mCodeSize, ShaderType mType);
			virtual InputLayout* CreateInputLayout(const ShaderInputLayoutAttri* mAttri, size_t mSize, ShaderObject* mShader);
			virtual Texture* CreateTexture1D(const uint8_t * mPixels, uint32_t mWidth,
				PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind);
			virtual Texture* CreateTexture2D(const uint8_t * mPixels, uint32_t mWidth,
				uint32_t mHeight, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind);
			virtual Texture* CreateTexture3D(const uint8_t * mPixels, uint32_t mWidth,
				uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind);

			virtual void SetShader(ShaderType mType, ShaderObject* mShader);
			virtual void SetVertexBuffer(BufferObject** mBufferObject, const size_t* mStrides, size_t mCount);
			virtual void SetIndexBuffer(BufferObject* mBufferObject, size_t mStride);
			virtual void SetInputLayout(InputLayout* mInputLayout);

			virtual void DoRender();

			inline ID3D11Device* D3DDevice() { return mDevice; }
			inline ID3D11DeviceContext* D3DContext() { return mContext; }
		};
	}
}