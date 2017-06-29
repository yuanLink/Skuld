#include "D3D11SamplerObject.h"

#include "D3D11Utility.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11SamplerObject * D3D11SamplerObject::Create(D3D11Context * mContext)
		{
			Ptr<D3D11SamplerObject> mRet = new D3D11SamplerObject(mContext);

			D3D11_SAMPLER_DESC mDesc;
			memset(&mDesc, 0, sizeof(mDesc));
			
			HRESULT hr = mContext->D3DDevice()->CreateSamplerState(&mDesc, &mRet->mSampler);
			RETURN_NULL_IF_FAILED(hr);

			return mRet.Detach();
		}

		const Render3DFactory * D3D11SamplerObject::GetFactory() const
		{
			return mContext->GetFactory();
		}
	}
}