#include "D3D11Context.h"
#include "D3D11Factory.h"
#include "D3D11Utility.h"

#include "D3D11ShaderObject.h"
#include "D3D11InputLayout.h"
#include "D3D11Texture1D.h"
#include "D3D11Texture2D.h"
#include "D3D11Texture3D.h"
#include "D3D11BufferObject.h"
#include "D3D11RenderState.h"
#include "D3D11SamplerObject.h"

#include <Skuld/Exception.h>

namespace Skuld
{
	namespace Render3D
	{
		BufferObject * D3D11Context::CreateBufferObject(const void * mData, size_t mBufferSize, 
			AccessFlag mAccess, BufferBindFlag mBind)
		{
			return D3D11BufferObject::Create(this, mData, mBufferSize, mAccess, mBind);
		}

		const Render3DFactory * D3D11Context::GetFactory() const
		{
			return this->mFactory;
		}

		Texture * D3D11Context::CreateTexture1D(const uint8_t * mPixels, uint32_t mWidth,
			PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind)
		{
			if (mPixelFormat == PixelFormat_RGB_888 || mPixelFormat == PixelFormat_BGR_888)
				throw Exception("不支持的格式");
			return D3D11Texture1D::Create(this, mPixels, mWidth, mPixelFormat, mAccess, mBind);
		}

		Texture* D3D11Context::CreateTexture2D(const uint8_t * mPixels, uint32_t mWidth,
			uint32_t mHeight, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind)
		{
			if (mPixelFormat == PixelFormat_RGB_888 || mPixelFormat == PixelFormat_BGR_888)
				throw Exception("不支持的格式");
			return D3D11Texture2D::Create(this, mPixels, mWidth, mHeight, mPixelFormat, mAccess, mBind);
		}

		Texture * D3D11Context::CreateTexture3D(const uint8_t * mPixels, uint32_t mWidth,
			uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind)
		{
			if (mPixelFormat == PixelFormat_RGB_888 || mPixelFormat == PixelFormat_BGR_888)
				throw Exception("不支持的格式");
			return D3D11Texture3D::Create(this, mPixels, mWidth, mHeight, mDepth, mPixelFormat, mAccess, mBind);
		}

		void D3D11Context::SetShader(ShaderType mType, ShaderObject * mShader)
		{
			if (mShader && mShader->GetFactory() != mFactory) throw Exception("Factory not same.");
			D3D11ShaderObject* _Shader = static_cast<D3D11ShaderObject*>(mShader);
			if (ShaderType_VertexShader == mType)
			{
				CComPtr<ID3D11VertexShader> mVS;
				if (mShader != nullptr)
					_Shader->mShader.QueryInterface(&mVS);
				this->mContext->VSSetShader(mVS, nullptr, 0);
			}
			else if (ShaderType_PixelShader == mType)
			{
				CComPtr<ID3D11PixelShader> mPS;
				if (mShader != nullptr)
					_Shader->mShader.QueryInterface(&mPS);
				this->mContext->PSSetShader(mPS, nullptr, 0);
			}
			else if (ShaderType_GeometryShader == mType)
			{
				CComPtr<ID3D11GeometryShader> mGS;
				if (mShader != nullptr)
					_Shader->mShader.QueryInterface(&mGS);
				this->mContext->GSSetShader(mGS, nullptr, 0);
			}
			else if (ShaderType_HullShader == mType)
			{
				CComPtr<ID3D11HullShader> mHS;
				if (mShader != nullptr)
					_Shader->mShader.QueryInterface(&mHS);
				this->mContext->HSSetShader(mHS, nullptr, 0);
			}
			else if (ShaderType_DomainShader == mType)
			{
				CComPtr<ID3D11DomainShader> mDS;
				if (mShader != nullptr)
					_Shader->mShader.QueryInterface(&mDS);
				this->mContext->DSSetShader(mDS, nullptr, 0);
			}
			else if (ShaderType_ComputeShader == mType)
			{
				CComPtr<ID3D11ComputeShader> mCS;
				if (mShader != nullptr)
					_Shader->mShader.QueryInterface(&mCS);
				this->mContext->CSSetShader(mCS, nullptr, 0);
			}
		}

		void D3D11Context::SetVertexBuffer(BufferObject ** mBufferObject, const size_t* mStrides, size_t mCount)
		{
			std::vector<ID3D11Buffer*> mTemp(mCount);
			std::vector<UINT> mStride(mCount);
			std::vector<UINT> mOffset(mCount);

			for (size_t i = 0; i < mCount; i++)
			{
				if (mBufferObject[i] == nullptr) throw Exception("Invalid arguments");
				if (mBufferObject[i]->GetFactory() != this->mFactory) throw Exception("Invalid factory");

				D3D11BufferObject* mD3D11Buffer = static_cast<D3D11BufferObject*>(mBufferObject[i]);
				mTemp[i] = mD3D11Buffer->mBuffer;
				mStride[i] = static_cast<UINT>(mStrides[i]);
				mOffset[i] = 0;
			}

			mContext->IASetVertexBuffers(0, static_cast<UINT>(mCount), mTemp.data(), mStride.data(), mOffset.data());
		}

		void D3D11Context::SetIndexBuffer(BufferObject * mBufferObject, size_t mStride)
		{
			if (mBufferObject == nullptr) throw Exception("Invalid arguments");
			if (mBufferObject->GetFactory() != this->mFactory) throw Exception("Invalid factory");

			D3D11BufferObject* mD3D11Buffer = static_cast<D3D11BufferObject*>(mBufferObject);

			DXGI_FORMAT mFormat;
			switch (mStride)
			{
			case 2:mFormat = DXGI_FORMAT_R16_UINT; break;
			case 4:mFormat = DXGI_FORMAT_R32_UINT; break;
			default:throw Exception("Invalid arguments");
			}

			mContext->IASetIndexBuffer(mD3D11Buffer->mBuffer, mFormat, 0);
		}

		void D3D11Context::SetInputLayout(InputLayout * mInputLayout)
		{
			if (mInputLayout == nullptr) throw Exception("Invalid arguments");
			if (mInputLayout->GetFactory() != this->mFactory) throw Exception("Invalid factory");

			D3D11InputLayout* mD3D11InputLayout = static_cast<D3D11InputLayout*>(mInputLayout);
			mContext->IASetInputLayout(mD3D11InputLayout->mInputLayout);
		}

		void D3D11Context::DoRender()
		{
			mContext->DrawIndexed(3, 0, 0);
		}

		ShaderObject * D3D11Context::CreateShader(const uint8_t * mCode, size_t mCodeSize, ShaderType mType)
		{
			return D3D11ShaderObject::CreateD3D11Shader(mCode, mCodeSize, mType, this);
		}

		InputLayout * D3D11Context::CreateInputLayout(const ShaderInputLayoutAttri * mAttri, size_t mSize, ShaderObject * mShader)
		{
			return D3D11InputLayout::CreateD3D11InputLayout(mAttri, mSize, mShader, this);
		}
	}
}