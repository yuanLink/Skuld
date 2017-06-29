#include "D3D11Context.h"
#include "D3D11Factory.h"
#include "D3D11Utility.h"

#include "D3D11InputLayout.h"

#include <memory>

namespace Skuld
{
	namespace Render3D
	{
		D3D11InputLayout * D3D11InputLayout::CreateD3D11InputLayout(const ShaderInputLayoutAttri * mAttri,
			size_t mSize, ShaderObject * mShader, D3D11Context* mContext)
		{
			Ptr<D3D11InputLayout> mRet = new D3D11InputLayout(mContext);
			std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> mDesc = std::make_unique<D3D11_INPUT_ELEMENT_DESC[]>(mSize);
			memset(mDesc.get(), 0, sizeof(D3D11_INPUT_ELEMENT_DESC) * mSize);

			std::vector<std::vector<char> > mAutoRelease(mSize);
			UINT mOffset = 0;
			for (size_t i = 0; i < mSize; i++)
			{
				std::string mName = mAttri[i].mName.GetStr();
				mAutoRelease[i].resize(mName.size() + 1);
				memcpy(mAutoRelease[i].data(), mName.c_str(), mName.size() + 1);

				mDesc[i].SemanticName = mAutoRelease[i].data();
				mDesc[i].SemanticIndex = 0;
				mDesc[i].InputSlot = 0;
				mDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				mDesc[i].InstanceDataStepRate = 0;

				std::tuple<DXGI_FORMAT, UINT>&& mFormat = SelectDXGIFormat(mAttri[i].mType, mAttri[i].mSize);

				mDesc[i].Format = std::get<0>(mFormat);
				mOffset += std::get<1>(mFormat);

				mDesc[i].AlignedByteOffset = mOffset;
			}

			RETURN_NULL_IF_FAILED(mContext->D3DDevice()->CreateInputLayout(mDesc.get(), static_cast<UINT>(mSize),
				mShader->GetCode(), static_cast<SIZE_T>(mShader->GetCodeSize()), &mRet->mInputLayout));

			return mRet.Detach();
		}
		const Render3DFactory * D3D11InputLayout::GetFactory() const
		{
			return mContext->GetFactory();
		}
	}
}
