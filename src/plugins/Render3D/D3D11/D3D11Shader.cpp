#include "D3D11Context.h"
#include "D3D11Factory.h"
#include "D3D11Utility.h"

#include "D3D11Shader.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11Shader * D3D11Shader::CreateD3D11Shader(const uint8_t * mCode, size_t mCodeSize, ShaderType mType,
			ID3D11Device* mDevice, const D3D11Factory* mFactory)
		{
			CComPtr<ID3D11DeviceChild> mShader;
			HRESULT hr = -1;
			switch (mType)
			{
			case Render3D::ShaderType_VertexShader:
				hr = mDevice->CreateVertexShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11VertexShader**)&mShader);
				break;
			case Render3D::ShaderType_PixelShader:
				hr = mDevice->CreatePixelShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11PixelShader**)&mShader);
				break;
			case Render3D::ShaderType_GeometryShader:
				hr = mDevice->CreateGeometryShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11GeometryShader**)&mShader);
				break;
			case Render3D::ShaderType_HullShader:
				hr = mDevice->CreateHullShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11HullShader**)&mShader);
				break;
			case Render3D::ShaderType_DomainShader:
				hr = mDevice->CreateDomainShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11DomainShader**)&mShader);
				break;
			case Render3D::ShaderType_ComputeShader:
				hr = mDevice->CreateComputeShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11ComputeShader**)&mShader);
				break;
			default:
				break;
			}
			RETURN_NULL_IF_FAILED(hr);
			return new D3D11Shader(mCode, mCodeSize, mType, mShader, mFactory);
		}

		D3D11Shader::D3D11Shader(const uint8_t * mCode, size_t mCodeSize, ShaderType mType,
			CComPtr<ID3D11DeviceChild> mShader, const D3D11Factory * mFactory)

			:mCode(mCodeSize), mType(mType), mShader(mShader), mFactory(mFactory)
		{
			memcpy(this->mCode.data(), mCode, mCodeSize);
		}

		const uint8_t * D3D11Shader::GetCode() const
		{
			return mCode.data();
		}

		const size_t D3D11Shader::GetCodeSize() const
		{
			return mCode.size();
		}

		ShaderType D3D11Shader::GetShaderType() const
		{
			return mType;
		}

		const Render3DFactory * D3D11Shader::GetFactory() const
		{
			return mFactory;
		}
	}
}
