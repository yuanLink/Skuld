#include "D3D11Context.h"
#include "D3D11Factory.h"
#include "D3D11Utility.h"

#include "D3D11ShaderObject.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11ShaderObject * D3D11ShaderObject::CreateD3D11Shader(const uint8_t * mCode, size_t mCodeSize, ShaderType mType,
			D3D11Context* mContext)
		{
			Ptr<D3D11ShaderObject> mRet = new D3D11ShaderObject(mCode, mCodeSize, mType, mContext);
			HRESULT hr = -1;
			switch (mType)
			{
			case Render3D::ShaderType_VertexShader:
				hr = mContext->D3DDevice()->CreateVertexShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11VertexShader**)&mRet->mShader);
				break;
			case Render3D::ShaderType_PixelShader:
				hr = mContext->D3DDevice()->CreatePixelShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11PixelShader**)&mRet->mShader);
				break;
			case Render3D::ShaderType_GeometryShader:
				hr = mContext->D3DDevice()->CreateGeometryShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11GeometryShader**)&mRet->mShader);
				break;
			case Render3D::ShaderType_HullShader:
				hr = mContext->D3DDevice()->CreateHullShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11HullShader**)&mRet->mShader);
				break;
			case Render3D::ShaderType_DomainShader:
				hr = mContext->D3DDevice()->CreateDomainShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11DomainShader**)&mRet->mShader);
				break;
			case Render3D::ShaderType_ComputeShader:
				hr = mContext->D3DDevice()->CreateComputeShader(mCode, static_cast<SIZE_T>(mCodeSize), nullptr, (ID3D11ComputeShader**)&mRet->mShader);
				break;
			default:
				break;
			}
			RETURN_NULL_IF_FAILED(hr);
			return mRet.Detach();
		}

		D3D11ShaderObject::D3D11ShaderObject(const uint8_t * mCode, size_t mCodeSize, ShaderType mType, D3D11Context* mContext)
			:mCode(mCodeSize), mType(mType), mContext(mContext)
		{
			memcpy(this->mCode.data(), mCode, mCodeSize);
		}

		const uint8_t * D3D11ShaderObject::GetCode() const
		{
			return mCode.data();
		}

		const size_t D3D11ShaderObject::GetCodeSize() const
		{
			return mCode.size();
		}

		ShaderType D3D11ShaderObject::GetShaderType() const
		{
			return mType;
		}

		const Render3DFactory * D3D11ShaderObject::GetFactory() const
		{
			return mContext->GetFactory();
		}
	}
}
