#include "D3D11Context.h"
#include "D3D11Factory.h"
#include "D3D11Utility.h"

#include "D3D11Shader.h"
#include "D3D11InputLayout.h"
#include "D3D11Texture1D.h"
#include "D3D11Texture2D.h"
#include "D3D11Texture3D.h"
#include "D3D11BufferObject.h"
#include "D3D11RenderState.h"

#include <Skuld/Exception.h>

namespace Skuld
{
	namespace Render3D
	{
		const Render3DFactory * D3D11Context::GetFactory() const
		{
			return this->mFactory;
		}

		Texture* D3D11Context::CreateTexture2D(const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, PixelFormat mPixelFormat)
		{
			if (mPixelFormat == PixelFormat_RGB_888 || mPixelFormat == PixelFormat_BGR_888)
				throw Exception("不支持的格式");
			return D3D11Texture2D::Create(mDevice, mFactory, mPixels, mWidth, mHeight, mPixelFormat);
		}

		Texture * D3D11Context::CreateTexture3D(const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat)
		{
			if (mPixelFormat == PixelFormat_RGB_888 || mPixelFormat == PixelFormat_BGR_888)
				throw Exception("不支持的格式");
			return D3D11Texture3D::Create(mDevice, mFactory, mPixels, mWidth, mHeight, mDepth, mPixelFormat);
		}

		void D3D11Context::SetShader(ShaderType mType, Shader * mShader)
		{
			if (mShader && mShader->GetFactory() != mFactory) throw Exception("Factory not same.");
			D3D11Shader* _Shader = static_cast<D3D11Shader*>(mShader);
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

		Shader * D3D11Context::CreateShader(const uint8_t * mCode, size_t mCodeSize, ShaderType mType)
		{
			return D3D11Shader::CreateD3D11Shader(mCode, mCodeSize, mType, mDevice, mFactory);
		}

		InputLayout * D3D11Context::CreateInputLayout(const ShaderInputLayoutAttri * mAttri, size_t mSize, Shader * mShader)
		{
			return D3D11InputLayout::CreateD3D11InputLayout(mAttri, mSize, mShader, mDevice, mFactory);
		}

		Texture * D3D11Context::CreateTexture1D(const uint8_t * mPixels, uint32_t mWidth, PixelFormat mPixelFormat)
		{
			if (mPixelFormat == PixelFormat_RGB_888 || mPixelFormat == PixelFormat_BGR_888)
				throw Exception("不支持的格式");
			return D3D11Texture1D::Create(mDevice, mFactory, mPixels, mWidth, mPixelFormat);
		}
	}
}